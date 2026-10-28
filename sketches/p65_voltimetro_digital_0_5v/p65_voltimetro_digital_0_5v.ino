// ============================================================================
// Projeto 65 — Voltímetro Digital 0–5 V
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C
// Material: 80 Projetos Arduino
// Medidor de tensão DC (0 a 5,00 V) e testador de pilhas AA/AAA no LCD I2C
// ATENÇÃO: Máximo absoluto de 5,0 V no pino A0!
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_VOLTIMETRO = A0; // Entrada de medição (0 a 5,0 V máx) com pull-down 10k
const byte PINO_I2C_SDA    = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL    = A5; // Barramento I2C SCL

// AJUSTE AQUI: Tensão exata do pino 5V do seu Arduino para calibração fina
const float TENSAO_REF_5V = 5.00;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  Serial.println(F("Sistema pronto! Voltimetro Digital 0-5V DC."));
}

void loop() {
  long soma = 0;
  for (byte i = 0; i < 16; i++) {
    soma += analogRead(PINO_VOLTIMETRO);
    delay(4);
  }
  float mediaAdc = soma / 16.0;
  float volts = (mediaAdc * TENSAO_REF_5V) / 1023.0;
  if (volts < 0.03) volts = 0.0;
  int milivolts = (int)(volts * 1000.0);

  lcd.setCursor(0, 0);
  lcd.print(F("Tensao: "));
  lcd.print(volts, 2);
  lcd.print(F(" V   "));

  lcd.setCursor(0, 1);
  if (volts < 0.05) {
    lcd.print(F("0 mV (Aberto)   "));
  } else if (volts >= 0.80 && volts <= 1.70) {
    if (volts >= 1.45) {
      lcd.print(F("Pilha AA: NOVA! "));
    } else if (volts >= 1.20) {
      lcd.print(F("Pilha: MeiaVida "));
    } else {
      lcd.print(F("Pilha: FRACA!   "));
    }
  } else {
    lcd.print(milivolts);
    lcd.print(F(" mV DC       "));
  }

  Serial.print(F("Tensao: "));
  Serial.print(volts, 3);
  Serial.print(F(" V ("));
  Serial.print(milivolts);
  Serial.println(F(" mV)"));

  delay(350);
}
