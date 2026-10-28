/*
 * Projeto 48: Ventilador Automático por Temperatura
 * O que faz: Lê a temperatura no DHT11 e controla por PWM a velocidade de uma ventoinha 5V via transistor.
 * Ligações: D2 (DHT11 DATA), D9 (Base do Transistor via resistor 1 kΩ)
 * Bibliotecas: DHT sensor library (Adafruit)
 * Material: 80 Projetos Arduino
 */

#include <DHT.h>

const byte PINO_DHT = 2;       // Dados do sensor DHT11
const byte PINO_VENTOINHA = 9; // Pino PWM ligado à base do transistor

const float TEMP_LIGA_MEDIA = 26.0; // AJUSTE AQUI: liga velocidade média acima deste valor (°C)
const float TEMP_LIGA_MAX = 29.0;   // AJUSTE AQUI: liga velocidade máxima acima deste valor (°C)

DHT dht(PINO_DHT, DHT11);

void setup() {
  Serial.begin(9600);
  pinMode(PINO_VENTOINHA, OUTPUT);
  analogWrite(PINO_VENTOINHA, 0);
  dht.begin();
  Serial.println(F("Sistema pronto! Ventilador Automático por Temperatura."));
}

void loop() {
  delay(2000);
  float temp = dht.readTemperature();

  if (isnan(temp)) {
    Serial.println(F("Erro ao ler o DHT11! Verifique o pino D2."));
    return;
  }

  Serial.print(F("Temp: "));
  Serial.print(temp, 1);
  Serial.print(F(" C -> Ventoinha: "));

  if (temp >= TEMP_LIGA_MAX) {
    analogWrite(PINO_VENTOINHA, 255);
    Serial.println(F("VELOCIDADE MÁXIMA (PWM 255)"));
  } else if (temp >= TEMP_LIGA_MEDIA) {
    analogWrite(PINO_VENTOINHA, 160);
    Serial.println(F("VELOCIDADE MÉDIA (PWM 160)"));
  } else {
    analogWrite(PINO_VENTOINHA, 0);
    Serial.println(F("DESLIGADA (PWM 0)"));
  }
}
