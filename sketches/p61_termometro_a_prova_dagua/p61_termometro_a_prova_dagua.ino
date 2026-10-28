// ============================================================================
// Projeto 61 — Termômetro à Prova d'Água
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C, OneWire, DallasTemperature
// Material: 80 Projetos Arduino
// Sonda digital DS18B20 (protocolo 1-Wire com pull-up de 4,7 kΩ) + LCD 16x2 I2C
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const byte PINO_ONEWIRE = 2;  // Fio Amarelo (DQ) do DS18B20 + resistor 4,7k para 5V
const byte PINO_I2C_SDA = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL

// AJUSTE AQUI: Endereço I2C do LCD (0x27 ou 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(PINO_ONEWIRE);
DallasTemperature sensores(&oneWire);

float tempMin = 999.0;
float tempMax = -999.0;

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  sensores.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(F("Sonda DS18B20"));
  lcd.setCursor(0, 1);
  lcd.print(F("Iniciando..."));
  Serial.println(F("Sistema pronto! Termômetro à Prova d'Água DS18B20."));
  delay(1200);
  lcd.clear();
}

void loop() {
  sensores.requestTemperatures();
  float tempC = sensores.getTempCByIndex(0);

  if (tempC == DEVICE_DISCONNECTED_C || tempC < -55.0 || tempC > 125.0) {
    lcd.setCursor(0, 0);
    lcd.print(F("Sensor desconect"));
    lcd.setCursor(0, 1);
    lcd.print(F("Cheque pull-up! "));
    Serial.println(F("Erro: Sensor DS18B20 desconectado (-127 C). Verifique o resistor de 4,7 kOhms!"));
    delay(1000);
    return;
  }

  if (tempC < tempMin) tempMin = tempC;
  if (tempC > tempMax) tempMax = tempC;

  lcd.setCursor(0, 0);
  lcd.print(F("Temp: "));
  lcd.print(tempC, 1);
  lcd.print((char)223);
  lcd.print(F("C    "));

  lcd.setCursor(0, 1);
  lcd.print(F("Min:"));
  lcd.print(tempMin, 1);
  lcd.print(F(" Max:"));
  lcd.print(tempMax, 1);
  lcd.print(F("  "));

  Serial.print(F("Temperatura Liquido: "));
  Serial.print(tempC, 2);
  Serial.print(F(" C | Min: "));
  Serial.print(tempMin, 1);
  Serial.print(F(" C | Max: "));
  Serial.print(tempMax, 1);
  Serial.println(F(" C"));

  delay(800);
}
