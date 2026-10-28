/*
 * Projeto 37: Cronômetro com LCD
 * O que faz: Cronômetro de minutos, segundos e décimos no LCD 16x2 I2C com botões Iniciar/Pausar e Zerar.
 * Ligações: D2 (Botão Iniciar/Pausar), D3 (Botão Zerar), A4 (LCD SDA), A5 (LCD SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_BTN_START = 2; // Botão Iniciar / Pausar
const byte PINO_BTN_RESET = 3; // Botão Zerar
const byte PINO_I2C_SDA = A4;  // LCD SDA
const byte PINO_I2C_SCL = A5;  // LCD SCL

const byte ENDERECO_LCD = 0x27; // AJUSTE AQUI: 0x27 ou 0x3F

LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);

bool rodando = false;
unsigned long tempoAcumuladoMs = 0;
unsigned long instantePartidaMs = 0;
unsigned long ultimoUpdateTela = 0;

bool startAnt = HIGH;
bool resetAnt = HIGH;

void atualizarCronometro(unsigned long totalMs) {
  unsigned int minutos = (unsigned int)(totalMs / 60000UL);
  byte segundos = (byte)((totalMs / 1000UL) % 60UL);
  byte decimos = (byte)((totalMs / 100UL) % 10UL);

  lcd.setCursor(0, 0);
  lcd.print(F("Tempo: "));
  if (minutos < 10) lcd.print('0');
  lcd.print(minutos);
  lcd.print(':');
  if (segundos < 10) lcd.print('0');
  lcd.print(segundos);
  lcd.print('.');
  lcd.print(decimos);
  lcd.print(F("  "));

  lcd.setCursor(0, 1);
  lcd.print(rodando ? F("[CORRENDO]      ") : F("[PAUSADO/PRONTO]"));
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  pinMode(PINO_BTN_START, INPUT_PULLUP);
  pinMode(PINO_BTN_RESET, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  atualizarCronometro(0);
  Serial.println(F("Sistema pronto! Cronômetro com LCD iniciado."));
}

void loop() {
  bool sAtual = digitalRead(PINO_BTN_START);
  bool rAtual = digitalRead(PINO_BTN_RESET);

  if (startAnt == HIGH && sAtual == LOW) {
    if (!rodando) {
      rodando = true;
      instantePartidaMs = millis();
      Serial.println(F("Cronômetro INICIADO!"));
    } else {
      tempoAcumuladoMs += (millis() - instantePartidaMs);
      rodando = false;
      Serial.print(F("Cronômetro PAUSADO em: "));
      Serial.print(tempoAcumuladoMs / 1000.0f, 1);
      Serial.println(F(" s"));
    }
    delay(50);
  }
  startAnt = sAtual;

  if (resetAnt == HIGH && rAtual == LOW) {
    rodando = false;
    tempoAcumuladoMs = 0;
    atualizarCronometro(0);
    Serial.println(F("Cronômetro ZERADO (00:00.0)."));
    delay(50);
  }
  resetAnt = rAtual;

  if (millis() - ultimoUpdateTela >= 100) {
    ultimoUpdateTela = millis();
    unsigned long total = tempoAcumuladoMs + (rodando ? (millis() - instantePartidaMs) : 0);
    atualizarCronometro(total);
  }
}
