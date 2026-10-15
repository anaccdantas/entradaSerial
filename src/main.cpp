#include <Arduino.h>

void leituraNaoBloqueante();
void leituraBloqueante();
void processarFrase(char *texto);
int lerProximoNumero(char *&cursor);
void aplicaCor();

int vermelho = 0;
int verde = 0;
int azul = 0;
int brilho = 0;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  //* readStringUntil espera no máximo por caractere esse tempo (ms) pelo '\n'
  //* o padrao é 1000ms

  Serial.println();
}

void loop()
{
  leituraNaoBloqueante();
}

void leituraNaoBloqueante()
{
  const int TAMANHO_MAX = 50; //* tamanho do vetor (incluir '/0')
  static char frase[TAMANHO_MAX];
  static int position = 0;

  while (Serial.available() > 0)
  {
    char c = Serial.read(); // quando a serial é lida, ela volta para 0

    if (c == '\r')
      continue; //* o continue pula o restante do while naquela volta
    //* e passa pra leitura do proximo caractere. o \r é lido e simplesmente descartado
    //* sem entrar no vetor

    if (c == '\n')
    {
      frase[position] = '\0';
      if (position > 0)
        processarFrase(frase);
      position = 0;
    }

    else if (position < TAMANHO_MAX - 1)
    {
      frase[position] = c;
      position++;
    }
  }
}

void processarFrase(char *texto)
{
  Serial.printf("Recebido: [%s]\n\r", texto);

  char *comando = texto;
  char *parametro = NULL; // -> \0

  char *espaco = strchr(texto, ' ');

  if (espaco != NULL) // se ele encontrou um espaco...
  {
    parametro = espaco + 1;
    *espaco = '\0';
  }


  if (strcasecmp(comando, "VERMELHO") == 0)
  {
    vermelho = 255;
    verde = 0;
    azul = 0;
    brilho = 100;
  }

  else if (strcasecmp(comando, "VERDE") == 0)
  {
    vermelho = 0;
    verde = 255;
    azul = 0;
    brilho = 100;
  }

  else if (strcasecmp(comando, "AZUL") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 255;
    brilho = 100;
  }

  else if (strcasecmp(comando, "BRANCO") == 0)
  {
    vermelho = 255;
    verde = 255;
    azul = 255;
    brilho = 100;
  }

  else if (strcasecmp(comando, "DESLIGA") == 0)
  {
    vermelho = 0;
    verde = 0;
    azul = 0;
    brilho = 0;
  }

  else if (strcasecmp(comando, "RGB") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Use o comando: RGB r g b (0 a 255).");
      return;
    }

    char* cursor = parametro;

    int r = lerProximoNumero(cursor);
    int g = lerProximoNumero(cursor);
    int b = lerProximoNumero(cursor);

    vermelho = constrain(r, 0, 255); //* limita o valor ja convertido para int de 0 a 255.
    verde = constrain(g, 0, 255);
    azul = constrain(b, 0, 255);
  }

  else if (strcasecmp(comando, "BRILHO") == 0)
  {
    if (parametro == NULL)
    {
      Serial.println("Use o comando: BRILHO n (0 a 100).");
      return;
    }

    brilho = constrain(atoi(parametro), 0, 100);
  }

  else
  {
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Comando: [%s] nao existe.");
    Serial.println(buffer);
    return;
  }
  aplicaCor();
}

void aplicaCor()
{
  int r = vermelho * brilho / 100;
  int g = verde * brilho / 100;
  int b = azul * brilho / 100;

  rgbLedWrite(PIN_RGB_LED, r , g, b);

  char bufferMensagem[80];

  snprintf(bufferMensagem, sizeof(bufferMensagem), "Cor: R=%3d G=%3d B=%3d | Brilho: %3d %% | HEX: #%02X%02X%02X", vermelho, verde, azul, brilho, r, g, b);
           
  Serial.println(bufferMensagem);
}

//* cursor e uma referencia (&) para um ponteiro char (char*)
//* a funcao recebe a propria variavel de quem chamou e nao uma copia
//*
//*     PARAMETRO   | ALTERA O TEXTO APONTADO | ALTERA PARA ONDE O PONTEIRO DE FORA APONTA
//* char * p        |     sim (*p = '\0')     |     nao, pq ele e uma copia
//* const char * p  |     nao                 |     nao
//* char* p&        |     sim                 |     sim, é a propria variavel passada como parametro
//*
int lerProximoNumero(char *&cursor) //* & serve para sinalizar que so quer trabalhar com o conteudo do ponteiro, nao o endereco
{
  if(cursor == NULL) return 0; //* protege o estouro de memoria no caso atoi(NULLr)
  int valor = atoi(cursor);
  char *espaco = strchr(cursor, ' ');
  cursor = (espaco != NULL) ? espaco + 1 : NULL;
  return valor;
}

void leituraBloqueante()
{
  //* available() diz quantos bytes ja chegaram e estao esperando leitura
  if (Serial.available() > 0) //? há algo a ser recebido na serial?
  {
    String texto = Serial.readStringUntil('\n');
    //! tomar cuidado pois é uma funcao bloqueante

    texto.trim(); //? trim(); remove do fim do texto os espacos, '\r', '\t', '\n' que alguns terminais enviam

    if (texto.length() == 0)
      return; // clausula

    Serial.printf("Seu nome é %s \n\r", texto.c_str());
    Serial.printf("Quantidade de caracteres: %d \n\r", texto.length());

    Serial.printf("Primeira Letra: %c \n\r", texto.charAt(0));
    Serial.printf("Última Letra: %c \n\r", texto[texto.length() - 1]);

    String maiusculo = texto;
    maiusculo.toUpperCase();
    Serial.printf("Em maiúsculas: %s \n\r", maiusculo.c_str());

    if (maiusculo == "ANA")
      Serial.println("Este é o seu nome correto");
  }
}
