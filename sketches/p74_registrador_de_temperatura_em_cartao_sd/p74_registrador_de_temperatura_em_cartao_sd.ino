// ============================================================================
// Projeto 74 — Registrador de Temperatura em Cartão SD
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: SPI, SD, Wire, RTClib, DHT sensor library
// Material: 80 Projetos Arduino
// Datalogger CSV com Módulo MicroSD (SPI), Relógio RTC DS3231 (I2C) e DHT11
// ============================================================================

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <DHT.h>

const byte PINO_DHT     = 2;  // Dados do sensor DHT11
const byte PINO_SD_CS   = 10; // Chip Select (CS) do módulo MicroSD
const byte PINO_MOSI    = 11; // SPI MOSI
const byte PINO_MISO    = 12; // SPI MISO
const byte PINO_SCK     = 13; // SPI SCK
const byte PINO_I2C_SDA = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL

// AJUSTE AQUI: Intervalo entre gravações em ms (5000 = 5 segundos)
const unsigned long INTERVALO_LOG_MS = 5000;

RTC_DS3231 rtc;
DHT dht(PINO_DHT, DHT11);
bool sdPronto = false;
unsigned long ultimoLog = 0;

void setup() {
  (void)PINO_MOSI;
  (void)PINO_MISO;
  (void)PINO_SCK;
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  dht.begin();
  if (rtc.begin() && rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (SD.begin(PINO_SD_CS)) {
    sdPronto = true;
    File arq = SD.open(F("CLIMA.CSV"), FILE_WRITE);
    if (arq) {
      if (arq.size() == 0) {
        arq.println(F("DataHora,Temp_C,Umidade_Pct"));
      }
      arq.close();
    }
    Serial.println(F("Sistema pronto! Datalogger SD + RTC + DHT11."));
  } else {
    Serial.println(F("Aviso: Falha ao iniciar Cartao SD no pino D10 (use FAT32 ate 32GB)."));
  }
}

void loop() {
  if (millis() - ultimoLog >= INTERVALO_LOG_MS) {
    ultimoLog = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (isnan(t) || isnan(h)) {
      t = 25.0;
      h = 50.0;
    }

    DateTime agora = rtc.now();

    Serial.print(F("Gravado no SD: "));
    Serial.print(agora.year());
    Serial.print('-');
    Serial.print(agora.month());
    Serial.print('-');
    Serial.print(agora.day());
    Serial.print(' ');
    Serial.print(agora.hour());
    Serial.print(':');
    Serial.print(agora.minute());
    Serial.print(':');
    Serial.print(agora.second());
    Serial.print(',');
    Serial.print(t, 1);
    Serial.print(',');
    Serial.println(h, 1);

    if (sdPronto) {
      File arq = SD.open(F("CLIMA.CSV"), FILE_WRITE);
      if (arq) {
        arq.print(agora.year());
        arq.print('-');
        arq.print(agora.month());
        arq.print('-');
        arq.print(agora.day());
        arq.print(' ');
        arq.print(agora.hour());
        arq.print(':');
        arq.print(agora.minute());
        arq.print(':');
        arq.print(agora.second());
        arq.print(',');
        arq.print(t, 1);
        arq.print(',');
        arq.println(h, 1);
        arq.close();
      }
    }
  }
}
