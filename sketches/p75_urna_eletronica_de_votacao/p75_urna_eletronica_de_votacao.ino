// ============================================================================
// Projeto 75 — Urna Eletrônica de Votação
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C
// Material: 80 Projetos Arduino
// Sistema de votação com 3 botões (Chapa A, Chapa B, Apuração), LCD I2C e Buzzer
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_BTN_A    = 2;  // Voto na Chapa A (INPUT_PULLUP)
const byte PINO_BTN_B    = 3;  // Voto na Chapa B (INPUT_PULLUP)
const byte PINO_BTN_APUR = 4;  // Botão Boletim de Urna / Apuração
const byte PINO_BUZZER   = 8;  // Buzzer Passivo (som "FIM")
const byte PINO_I2C_SDA  = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL  = A5; // Barramento I2C SCL

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned int votosA = 0;
unsigned int votosB = 0;

void tocarSomUrna() {
  // Alterna duas notas agudas reproduzindo o timbre clássico da urna
  for (byte i = 0; i < 6; i++) {
    tone(PINO_BUZZER, 1980, 70);
    delay(75);
    tone(PINO_BUZZER, 2090, 70);
    delay(75);
  }
  noTone(PINO_BUZZER);
}

void mostrarTelaPronta() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("URNA ELETRONICA"));
  lcd.setCursor(0, 1);
  lcd.print(F("D2=A D3=B D4=Fim"));
}

void confirmarVoto() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(F("VOTO COMPUTADO"));
  lcd.setCursor(6, 1);
  lcd.print(F("FIM!"));
  tocarSomUrna();

  Serial.print(F("Voto registrado com sucesso! Total de eleitores: "));
  Serial.println(votosA + votosB);

  delay(1500);
  mostrarTelaPronta();
}

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_BTN_A, INPUT_PULLUP);
  pinMode(PINO_BTN_B, INPUT_PULLUP);
  pinMode(PINO_BTN_APUR, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  mostrarTelaPronta();

  Serial.println(F("Sistema pronto! Urna Eletronica Escolar."));
}

void loop() {
  if (digitalRead(PINO_BTN_A) == LOW) {
    votosA++;
    confirmarVoto();
  } else if (digitalRead(PINO_BTN_B) == LOW) {
    votosB++;
    confirmarVoto();
  } else if (digitalRead(PINO_BTN_APUR) == LOW) {
    unsigned int total = votosA + votosB;
    unsigned int pctA = (total > 0) ? (unsigned int)((votosA * 100UL) / total) : 0;
    unsigned int pctB = (total > 0) ? (100 - pctA) : 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("A:"));
    lcd.print(votosA);
    lcd.print(F(" ("));
    lcd.print(pctA);
    lcd.print(F("%)"));

    lcd.setCursor(0, 1);
    lcd.print(F("B:"));
    lcd.print(votosB);
    lcd.print(F(" ("));
    lcd.print(pctB);
    lcd.print(F("%)"));

    Serial.println(F("=== BOLETIM DE URNA ==="));
    Serial.print(F("Chapa A: "));
    Serial.print(votosA);
    Serial.print(F(" votos ("));
    Serial.print(pctA);
    Serial.print(F("%) | Chapa B: "));
    Serial.print(votosB);
    Serial.print(F(" votos ("));
    Serial.print(pctB);
    Serial.println(F("%)"));

    delay(4000);
    mostrarTelaPronta();
  }
}
