// ============================================================================
// Projeto 69 — Nível Eletrônico com Acelerômetro
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire
// Material: 80 Projetos Arduino
// Leitura direta dos registradores I2C do MPU6050 (0x68) + Régua de 5 LEDs
// ============================================================================

#include <Wire.h>

const byte PINO_LED_ESQ_FORTE = 2;  // Vermelho Esquerda
const byte PINO_LED_ESQ_LEVE  = 3;  // Amarelo Esquerda
const byte PINO_LED_CENTRO    = 4;  // Verde Centro (Nivelado!)
const byte PINO_LED_DIR_LEVE  = 5;  // Amarelo Direita
const byte PINO_LED_DIR_FORTE = 6;  // Vermelho Direita
const byte PINO_I2C_SDA       = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL       = A5; // Barramento I2C SCL

const byte ENDERECO_MPU = 0x68;

// AJUSTE AQUI: Compensação de zero caso o módulo esteja levemente inclinado na protoboard
const int OFFSET_INCLINACAO = 0;

void acenderUnicoLed(byte pinoAtivo) {
  const byte todos[5] = {
    PINO_LED_ESQ_FORTE, PINO_LED_ESQ_LEVE, PINO_LED_CENTRO,
    PINO_LED_DIR_LEVE, PINO_LED_DIR_FORTE
  };
  for (byte i = 0; i < 5; i++) {
    digitalWrite(todos[i], (todos[i] == pinoAtivo) ? HIGH : LOW);
  }
}

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_LED_ESQ_FORTE, OUTPUT);
  pinMode(PINO_LED_ESQ_LEVE, OUTPUT);
  pinMode(PINO_LED_CENTRO, OUTPUT);
  pinMode(PINO_LED_DIR_LEVE, OUTPUT);
  pinMode(PINO_LED_DIR_FORTE, OUTPUT);

  Wire.begin();
  // Acorda o MPU6050 escrevendo 0 no registrador PWR_MGMT_1 (0x6B)
  Wire.beginTransmission(ENDERECO_MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  Serial.println(F("Sistema pronto! Nivel Eletronico MPU6050 iniciado."));
}

void loop() {
  // Solicita 6 bytes de aceleração a partir do registrador ACCEL_XOUT_H (0x3B)
  Wire.beginTransmission(ENDERECO_MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(ENDERECO_MPU, (uint8_t)6, (uint8_t)true);

  int16_t acX = 0, acY = 0, acZ = 0;
  if (Wire.available() >= 6) {
    acX = (Wire.read() << 8) | Wire.read();
    acY = (Wire.read() << 8) | Wire.read();
    acZ = (Wire.read() << 8) | Wire.read();
  }

  // Na escala padrão ±2g, ~285 unidades equivalem a ~1 grau de inclinação
  int inclinacao = (acX / 285) + OFFSET_INCLINACAO;

  if (inclinacao < -12) {
    acenderUnicoLed(PINO_LED_ESQ_FORTE);
  } else if (inclinacao < -3) {
    acenderUnicoLed(PINO_LED_ESQ_LEVE);
  } else if (inclinacao <= 3) {
    acenderUnicoLed(PINO_LED_CENTRO);
  } else if (inclinacao <= 12) {
    acenderUnicoLed(PINO_LED_DIR_LEVE);
  } else {
    acenderUnicoLed(PINO_LED_DIR_FORTE);
  }

  Serial.print(F("Inclinacao (~graus): "));
  Serial.print(inclinacao);
  Serial.print(F(" | AcX: "));
  Serial.print(acX);
  Serial.print(F(" | AcY: "));
  Serial.print(acY);
  Serial.print(F(" | AcZ: "));
  Serial.println(acZ);

  delay(120);
}
