// ============================================================================
// Projeto 64 — Medidor de Luminosidade
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C
// Material: 80 Projetos Arduino
// Medição relativa de luz (0% a 100%) com LDR e barra gráfica no LCD 16x2 I2C
// Nota técnica: Leitura relativa comparativa (não calibrada em Lux absoluto).
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_LDR     = A0; // Divisor de tensão LDR + resistor 10k
const byte PINO_I2C_SDA = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  Serial.println(F("Sistema pronto! Medidor de Luminosidade Relativa (0 a 100%)."));
}

void loop() {
  long soma = 0;
  for (byte i = 0; i < 8; i++) {
    soma += analogRead(PINO_LDR);
    delay(5);
  }
  int leitura = (int)(soma / 8);
  int pct = map(leitura, 0, 1023, 0, 100);
  pct = constrain(pct, 0, 100);

  lcd.setCursor(0, 0);
  lcd.print(F("Luz Relat: "));
  if (pct < 100) lcd.print(' ');
  if (pct < 10) lcd.print(' ');
  lcd.print(pct);
  lcd.print(F("% "));

  lcd.setCursor(0, 1);
  if (pct < 15) {
    lcd.print(F("Escuro  "));
  } else if (pct < 45) {
    lcd.print(F("Penumbra"));
  } else if (pct < 80) {
    lcd.print(F("Claro   "));
  } else {
    lcd.print(F("Forte   "));
  }

  int blocos = map(pct, 0, 100, 0, 8);
  for (int i = 0; i < 8; i++) {
    lcd.write(i < blocos ? (byte)255 : (byte)' ');
  }

  Serial.print(F("ADC A0: "));
  Serial.print(leitura);
  Serial.print(F(" | Luminosidade Relativa: "));
  Serial.print(pct);
  Serial.println(F("%"));

  delay(350);
}
