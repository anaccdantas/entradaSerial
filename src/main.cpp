#include <Arduino.h>

void setup() 
{
  Serial.begin(9600);
  Serial.setTimeout(2000);
  //* readStringUntil espera no máximo esse tempo (ms) pelo '\n'
  //* o padrao é 1000ms

  Serial.println();
  Serial.println("Digite o seu nome e pressione ENTER: ");
}

void loop() 
{
  //* available() diz quantos bytes ja chegaram e estao esperando leitura
  if(Serial.available() > 0) //? há algo a ser recebido na serial?
  {
    String texto = Serial.readStringUntil('\n');

    Serial.printf("Seu nome é %s", texto.c_str());
  }
}
