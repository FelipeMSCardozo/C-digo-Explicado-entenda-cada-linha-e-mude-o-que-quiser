/*
 * Projeto 36: Despertador Digital
 * O que faz: Relógio com RTC DS3231 e LCD I2C com ajuste de alarme em 3 botões e toque no buzzer.
 * Ligações: D2 (Botão Hora+), D3 (Botão Min+), D4 (Botão Liga/Parar), D8 (Buzzer), A4 (SDA), A5 (SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C, RTClib
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

const byte PINO_BTN_HORA = 2;   // Avança hora do alarme
const byte PINO_BTN_MIN = 3;    // Avança minuto do alarme
const byte PINO_BTN_ALARME = 4; // Ativa/Desativa ou silencia o despertador
const byte PINO_BUZZER = 8;     // Buzzer ativo 5V
const byte PINO_I2C_SDA = A4;   // Barramento I2C SDA
const byte PINO_I2C_SCL = A5;   // Barramento I2C SCL

const byte ENDERECO_LCD = 0x27; // AJUSTE AQUI: 0x27 ou 0x3F

LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);
RTC_DS3231 rtc;

int alarmeHora = 7;
int alarmeMin = 30;
bool alarmeAtivo = true;
bool tocando = false;
int minutoJaTocou = -1;

unsigned long ultimoRefresh = 0;

void print2d(int v) {
  if (v < 10) lcd.print('0');
  lcd.print(v);
}

void atualizarDisplay(const DateTime& agora) {
  lcd.setCursor(0, 0);
  lcd.print(F("Agora: "));
  print2d(agora.hour());
  lcd.print(':');
  print2d(agora.minute());
  lcd.print(':');
  print2d(agora.second());

  lcd.setCursor(0, 1);
  if (tocando) {
    lcd.print(F("ACORDE! APERTE! "));
  } else {
    lcd.print(F("Alarme "));
    print2d(alarmeHora);
    lcd.print(':');
    print2d(alarmeMin);
    lcd.print(alarmeAtivo ? F(" ON ") : F(" OFF"));
  }
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  pinMode(PINO_BTN_HORA, INPUT_PULLUP);
  pinMode(PINO_BTN_MIN, INPUT_PULLUP);
  pinMode(PINO_BTN_ALARME, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  if (rtc.begin() && rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println(F("Sistema pronto! Despertador Digital ativo."));
}

void loop() {
  DateTime agora = rtc.now();

  if (digitalRead(PINO_BTN_HORA) == LOW) {
    alarmeHora = (alarmeHora + 1) % 24;
    minutoJaTocou = -1;
    atualizarDisplay(agora);
    delay(180);
  }

  if (digitalRead(PINO_BTN_MIN) == LOW) {
    alarmeMin = (alarmeMin + 1) % 60;
    minutoJaTocou = -1;
    atualizarDisplay(agora);
    delay(180);
  }

  if (digitalRead(PINO_BTN_ALARME) == LOW) {
    if (tocando) {
      tocando = false;
      digitalWrite(PINO_BUZZER, LOW);
      Serial.println(F("Alarme silenciado pelo usuário."));
    } else {
      alarmeAtivo = !alarmeAtivo;
      Serial.print(F("Alarme ajustado para "));
      Serial.print(alarmeHora);
      Serial.print(':');
      Serial.print(alarmeMin);
      Serial.println(alarmeAtivo ? F(" [ON]") : F(" [OFF]"));
    }
    atualizarDisplay(agora);
    delay(250);
  }

  if (alarmeAtivo && !tocando && agora.hour() == alarmeHora && agora.minute() == alarmeMin && agora.minute() != minutoJaTocou) {
    tocando = true;
    minutoJaTocou = agora.minute();
    Serial.println(F("DESPERTADOR TOCANDO! Pressione o botão D4 para parar."));
  }

  if (agora.minute() != alarmeMin) {
    minutoJaTocou = -1;
  }

  if (tocando) {
    bool bip = ((millis() / 200) % 2 == 0);
    digitalWrite(PINO_BUZZER, bip ? HIGH : LOW);
  }

  if (millis() - ultimoRefresh >= 500) {
    ultimoRefresh = millis();
    atualizarDisplay(agora);
  }
}
