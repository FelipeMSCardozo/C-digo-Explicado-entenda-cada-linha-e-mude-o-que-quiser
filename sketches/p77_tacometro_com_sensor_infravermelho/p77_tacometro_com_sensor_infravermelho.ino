// ============================================================================
// Projeto 77 — Tacômetro com Sensor Infravermelho
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C
// Material: 80 Projetos Arduino
// Medidor de RPM com TCRT5000 por Interrupção Externa (pino D2) + LCD 16x2 I2C
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_SENSOR  = 2;  // Saída DO do TCRT5000 (Interrupção INT0 no D2!)
const byte PINO_MOTOR   = 9;  // PWM para o transistor do motor DC de teste
const byte PINO_I2C_SDA = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL

// AJUSTE AQUI: Quantidade de marcas brancas ou pás por volta completa
const byte MARCAS_POR_VOLTA = 1;
const byte PWM_MOTOR_TESTE  = 160;

LiquidCrystal_I2C lcd(0x27, 16, 2);

volatile unsigned long contadorPulsos = 0;
volatile unsigned long ultimoPulsoUs  = 0;
unsigned long ultimaMedicaoMs = 0;

// Rotina de Serviço de Interrupção (ISR) chamada instantaneamente pelo hardware
void contarPulso() {
  unsigned long agoraUs = micros();
  // Filtro anti-ruído de 2 ms (permite medir até 30.000 RPM sem repique)
  if (agoraUs - ultimoPulsoUs >= 2000UL) {
    contadorPulsos++;
    ultimoPulsoUs = agoraUs;
  }
}

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_SENSOR, INPUT_PULLUP);
  pinMode(PINO_MOTOR, OUTPUT);
  analogWrite(PINO_MOTOR, PWM_MOTOR_TESTE);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Tacometro Optico"));
  lcd.setCursor(0, 1);
  lcd.print(F("Medindo RPM...  "));

  attachInterrupt(digitalPinToInterrupt(PINO_SENSOR), contarPulso, FALLING);

  ultimaMedicaoMs = millis();
  Serial.println(F("Sistema pronto! Tacometro Digital por Interrupcao (D2)."));
}

void loop() {
  if (millis() - ultimaMedicaoMs >= 1000) {
    // Seção crítica: copia e zera o contador com interrupções pausadas
    noInterrupts();
    unsigned long pulsos = contadorPulsos;
    contadorPulsos = 0;
    interrupts();

    ultimaMedicaoMs += 1000;

    unsigned long rpm = (pulsos * 60UL) / MARCAS_POR_VOLTA;
    unsigned long hz  = pulsos / MARCAS_POR_VOLTA;

    lcd.setCursor(0, 0);
    lcd.print(F("Rotacao: "));
    lcd.print(rpm);
    lcd.print(F(" RPM   "));

    lcd.setCursor(0, 1);
    lcd.print(F("Freq: "));
    lcd.print(hz);
    lcd.print(F(" Hz      "));

    Serial.print(F("Pulsos/s (Hz): "));
    Serial.print(hz);
    Serial.print(F(" | Velocidade: "));
    Serial.print(rpm);
    Serial.println(F(" RPM"));
  }
}
