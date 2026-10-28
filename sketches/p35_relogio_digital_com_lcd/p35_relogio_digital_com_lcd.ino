/*
 * Projeto 35: Relógio Digital com LCD
 * O que faz: Lê data e hora exatas do módulo RTC DS3231 e exibe formatado no LCD 16x2 I2C.
 * Ligações: A4 (SDA compartilhado entre RTC e LCD), A5 (SCL compartilhado entre RTC e LCD)
 * Bibliotecas: Wire, LiquidCrystal I2C, RTClib
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

const byte PINO_I2C_SDA = A4; // Barramento I2C SDA (RTC + LCD)
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL (RTC + LCD)

const byte ENDERECO_LCD = 0x27;        // AJUSTE AQUI: 0x27 ou 0x3F
const bool FORCAR_AJUSTE_HORA = false; // AJUSTE AQUI: mude para true se quiser reajustar com a hora do PC

LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);
RTC_DS3231 rtc;

const char DIAS_SEMANA[7][4] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};
unsigned long ultimaAtualizacao = 0;

void imprimirDoisDigitos(int valor) {
  if (valor < 10) lcd.print('0');
  lcd.print(valor);
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    Serial.println(F("Erro no DS3231! Verifique as conexões A4 e A5."));
    lcd.setCursor(0, 0);
    lcd.print(F("Erro no DS3231!"));
    while (true) {
      delay(500);
    }
  }

  if (rtc.lostPower() || FORCAR_AJUSTE_HORA) {
    Serial.println(F("Ajustando RTC com a data/hora da compilação..."));
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println(F("Sistema pronto! Relógio Digital RTC DS3231 ativo."));
}

void loop() {
  if (millis() - ultimaAtualizacao >= 1000) {
    ultimaAtualizacao = millis();
    DateTime agora = rtc.now();

    lcd.setCursor(0, 0);
    lcd.print(F("Hora: "));
    imprimirDoisDigitos(agora.hour());
    lcd.print(':');
    imprimirDoisDigitos(agora.minute());
    lcd.print(':');
    imprimirDoisDigitos(agora.second());
    lcd.print(F("  "));

    lcd.setCursor(0, 1);
    imprimirDoisDigitos(agora.day());
    lcd.print('/');
    imprimirDoisDigitos(agora.month());
    lcd.print('/');
    lcd.print(agora.year());
    lcd.print(' ');
    lcd.print(DIAS_SEMANA[agora.dayOfTheWeek()]);
    lcd.print(F("  "));

    Serial.print(F("Hora: "));
    Serial.print(agora.hour());
    Serial.print(':');
    Serial.print(agora.minute());
    Serial.print(':');
    Serial.print(agora.second());
    Serial.print(F(" | Data: "));
    Serial.print(agora.day());
    Serial.print('/');
    Serial.print(agora.month());
    Serial.print('/');
    Serial.println(agora.year());
  }
}
