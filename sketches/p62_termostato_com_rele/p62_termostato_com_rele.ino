// ============================================================================
// Projeto 62 — Termostato com Relé
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C, OneWire, DallasTemperature
// Material: 80 Projetos Arduino
// Controle térmico com histerese (±0,5 °C), DS18B20, botões, LCD I2C e Relé 5V
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const byte PINO_ONEWIRE   = 2;  // Dados do DS18B20 (com pull-up de 4,7k para 5V)
const byte PINO_BTN_MAIS  = 3;  // Botão +0,5 °C (INPUT_PULLUP)
const byte PINO_BTN_MENOS = 4;  // Botão -0,5 °C (INPUT_PULLUP)
const byte PINO_RELE      = 7;  // Módulo Relé 5V
const byte PINO_I2C_SDA   = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL   = A5; // Barramento I2C SCL

// AJUSTE AQUI: Módulos relé comuns ligam em LOW; mude para HIGH se necessário
const byte NIVEL_RELE_LIGADO    = LOW;
const byte NIVEL_RELE_DESLIGADO = HIGH;
const float HISTERESE = 0.5; // Janela de ±0,5 °C

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(PINO_ONEWIRE);
DallasTemperature sensores(&oneWire);

float tempAlvo = 28.0;
bool releAtivo = false;
unsigned long ultimaConversao = 0;
float tempAtual = 25.0;

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_BTN_MAIS, INPUT_PULLUP);
  pinMode(PINO_BTN_MENOS, INPUT_PULLUP);
  pinMode(PINO_RELE, OUTPUT);
  digitalWrite(PINO_RELE, NIVEL_RELE_DESLIGADO);

  sensores.begin();
  lcd.init();
  lcd.backlight();

  Serial.println(F("Sistema pronto! Termostato Digital com Histerese."));
}

void loop() {
  // Leitura dos botões de ajuste de Setpoint
  if (digitalRead(PINO_BTN_MAIS) == LOW) {
    tempAlvo += 0.5;
    if (tempAlvo > 60.0) tempAlvo = 60.0;
    delay(180);
  }
  if (digitalRead(PINO_BTN_MENOS) == LOW) {
    tempAlvo -= 0.5;
    if (tempAlvo < 5.0) tempAlvo = 5.0;
    delay(180);
  }

  // Atualiza leitura do DS18B20 a cada 600 ms
  if (millis() - ultimaConversao >= 600) {
    ultimaConversao = millis();
    sensores.requestTemperatures();
    float lida = sensores.getTempCByIndex(0);
    if (lida != DEVICE_DISCONNECTED_C && lida > -50.0 && lida < 125.0) {
      tempAtual = lida;
    }

    // Controle com histerese (modo resfriamento: liga acima de Alvo + 0.5, desliga abaixo de Alvo - 0.5)
    if (!releAtivo && tempAtual >= (tempAlvo + HISTERESE)) {
      releAtivo = true;
      digitalWrite(PINO_RELE, NIVEL_RELE_LIGADO);
    } else if (releAtivo && tempAtual <= (tempAlvo - HISTERESE)) {
      releAtivo = false;
      digitalWrite(PINO_RELE, NIVEL_RELE_DESLIGADO);
    }

    lcd.setCursor(0, 0);
    lcd.print(F("Atual: "));
    lcd.print(tempAtual, 1);
    lcd.print((char)223);
    lcd.print(F("C   "));

    lcd.setCursor(0, 1);
    lcd.print(F("Meta:"));
    lcd.print(tempAlvo, 1);
    lcd.print(F("C "));
    lcd.print(releAtivo ? F("[ON] ") : F("[OFF]"));

    Serial.print(F("Atual: "));
    Serial.print(tempAtual, 1);
    Serial.print(F(" C | Meta: "));
    Serial.print(tempAlvo, 1);
    Serial.print(F(" C | Rele: "));
    Serial.println(releAtivo ? F("LIGADO") : F("DESLIGADO"));
  }
}
