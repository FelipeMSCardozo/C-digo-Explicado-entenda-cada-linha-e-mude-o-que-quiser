/*
 * Projeto 06: Estação Meteorológica
 * O que faz: Monitora Temperatura, Umidade (DHT11), Luz (LDR) e Chuva e exibe tudo no LCD 16x2 I2C.
 * Ligações: D2 (DHT11), A0 (LDR), A1 (Sensor de Chuva AO), A4 (LCD SDA), A5 (LCD SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C, DHT sensor library
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const byte PINO_DHT = 2;        // Dados do sensor DHT11
const byte PINO_LDR = A0;       // Divisor de tensão do LDR
const byte PINO_CHUVA = A1;     // Saída analógica AO do sensor de chuva
const byte PINO_I2C_SDA = A4;   // I2C SDA do LCD
const byte PINO_I2C_SCL = A5;   // I2C SCL do LCD

const byte ENDERECO_LCD = 0x27; // AJUSTE AQUI: 0x27 ou 0x3F
const int LIMIAR_CHUVA = 650;   // AJUSTE AQUI: leitura A1 abaixo disso indica chuva

LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);
DHT dht(PINO_DHT, DHT11);

unsigned long ultimaAtualizacao = 0;

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(F("Estacao Clima"));
  lcd.setCursor(0, 1);
  lcd.print(F("Iniciando..."));
  Serial.println(F("Sistema pronto! Estação Meteorológica ativa."));
  delay(1500);
  lcd.clear();
}

void loop() {
  if (millis() - ultimaAtualizacao >= 2000) {
    ultimaAtualizacao = millis();

    float temp = dht.readTemperature();
    float umid = dht.readHumidity();
    int leituraLuz = analogRead(PINO_LDR);
    int leituraChuva = analogRead(PINO_CHUVA);

    int pctLuz = map(leituraLuz, 0, 1023, 0, 100);
    bool chovendo = (leituraChuva < LIMIAR_CHUVA);

    lcd.setCursor(0, 0);
    if (isnan(temp) || isnan(umid)) {
      lcd.print(F("DHT11: verificar"));
    } else {
      lcd.print(F("T:"));
      lcd.print((int)temp);
      lcd.write(223);
      lcd.print(F("C Umid:"));
      lcd.print((int)umid);
      lcd.print(F("%   "));
    }

    lcd.setCursor(0, 1);
    lcd.print(F("Luz:"));
    lcd.print(pctLuz);
    lcd.print(F("% "));
    lcd.print(chovendo ? F("CHUVA   ") : F("SECO    "));

    Serial.print(F("T: "));
    Serial.print(temp, 1);
    Serial.print(F(" C | U: "));
    Serial.print((int)umid);
    Serial.print(F("% | Luz: "));
    Serial.print(pctLuz);
    Serial.print(F("% | Tempo: "));
    Serial.println(chovendo ? F("CHUVA") : F("SECO"));
  }
}
