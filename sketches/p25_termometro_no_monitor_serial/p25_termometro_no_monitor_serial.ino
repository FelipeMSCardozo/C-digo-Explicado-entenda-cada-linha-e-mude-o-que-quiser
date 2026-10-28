/*
 * Projeto 25: Termômetro no Monitor Serial
 * O que faz: Lê temperatura (°C), umidade (%) e sensação térmica do DHT11 e exibe no Monitor Serial.
 * Ligações: D2 (Pino DATA/OUT do DHT11), 5V (VCC), GND (GND)
 * Bibliotecas: DHT sensor library (Adafruit)
 * Material: 80 Projetos Arduino
 */

#include <DHT.h>

const byte PINO_DHT = 2; // Pino de dados do sensor DHT11

DHT dht(PINO_DHT, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println(F("Sistema pronto! Termômetro no Monitor Serial (DHT11)."));
}

void loop() {
  delay(2000); // Intervalo mínimo obrigatório do DHT11 (2 segundos)

  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println(F("Erro de leitura no DHT11! Confira o fio no pino D2."));
    return;
  }

  // Calcula o índice de calor (sensação térmica) em Celsius (false = Celsius)
  float sensacao = dht.computeHeatIndex(temperatura, umidade, false);

  Serial.print(F("Temperatura: "));
  Serial.print(temperatura, 1);
  Serial.print(F(" C | Umidade: "));
  Serial.print((int)umidade);
  Serial.print(F(" % | Sensação Térmica: "));
  Serial.print(sensacao, 1);
  Serial.println(F(" C"));
}
