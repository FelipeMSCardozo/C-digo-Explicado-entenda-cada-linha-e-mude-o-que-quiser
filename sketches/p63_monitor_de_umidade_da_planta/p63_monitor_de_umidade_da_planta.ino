// ============================================================================
// Projeto 63 — Monitor de Umidade da Planta
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C
// Material: 80 Projetos Arduino
// Sensor de solo com alimentação pulsada (D8), LCD 16x2 I2C e LED RGB
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_SOLO       = A0; // Saída analógica (AO) do sensor de solo
const byte PINO_VCC_SENSOR = 8;  // Alimenta o sensor apenas durante a leitura
const byte PINO_LED_R      = 9;  // Canal Vermelho do LED RGB
const byte PINO_LED_G      = 10; // Canal Verde do LED RGB
const byte PINO_LED_B      = 11; // Canal Azul do LED RGB
const byte PINO_I2C_SDA    = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL    = A5; // Barramento I2C SCL

// AJUSTE AQUI: Calibre conforme os valores brutos da sua terra no Monitor Serial
const int LEITURA_SECO    = 1000;
const int LEITURA_MOLHADO = 350;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void definirCor(byte r, byte g, byte b) {
  analogWrite(PINO_LED_R, r);
  analogWrite(PINO_LED_G, g);
  analogWrite(PINO_LED_B, b);
}

int lerUmidadeSolo(int &brutoOut) {
  digitalWrite(PINO_VCC_SENSOR, HIGH);
  delay(20); // Aguarda estabilização elétrica do módulo
  brutoOut = analogRead(PINO_SOLO);
  digitalWrite(PINO_VCC_SENSOR, LOW); // Desliga para evitar eletrólise nas hastes!

  int pct = map(brutoOut, LEITURA_SECO, LEITURA_MOLHADO, 0, 100);
  return constrain(pct, 0, 100);
}

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_VCC_SENSOR, OUTPUT);
  digitalWrite(PINO_VCC_SENSOR, LOW);
  pinMode(PINO_LED_R, OUTPUT);
  pinMode(PINO_LED_G, OUTPUT);
  pinMode(PINO_LED_B, OUTPUT);

  lcd.init();
  lcd.backlight();

  Serial.println(F("Sistema pronto! Monitor de Umidade da Planta (anti-oxidacao)."));
}

void loop() {
  int bruto = 0;
  int umidade = lerUmidadeSolo(bruto);

  lcd.setCursor(0, 0);
  lcd.print(F("Umidade: "));
  if (umidade < 10) lcd.print(' ');
  lcd.print(umidade);
  lcd.print(F("%    "));

  lcd.setCursor(0, 1);
  if (umidade < 30) {
    lcd.print(F("Regar Agora!    "));
    definirCor(255, 0, 0); // Vermelho
  } else if (umidade <= 75) {
    lcd.print(F("Planta Feliz!   "));
    definirCor(0, 220, 0); // Verde
  } else {
    lcd.print(F("Solo Encharcado "));
    definirCor(0, 80, 255); // Azul
  }

  Serial.print(F("Bruto A0: "));
  Serial.print(bruto);
  Serial.print(F(" | Umidade Relativa do Solo: "));
  Serial.print(umidade);
  Serial.println(F("%"));

  delay(2000);
}
