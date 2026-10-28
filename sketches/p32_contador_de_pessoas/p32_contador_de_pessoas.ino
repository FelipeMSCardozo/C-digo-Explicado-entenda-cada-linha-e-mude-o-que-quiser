/*
 * Projeto 32: Contador de Pessoas
 * O que faz: Conta quantas pessoas passam em frente ao sensor HC-SR04 e exibe o total no LCD 16x2 I2C.
 * Ligações: D6 (Trig), D7 (Echo), A4 (LCD SDA), A5 (LCD SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_TRIG = 6;      // Trig do HC-SR04
const byte PINO_ECHO = 7;      // Echo do HC-SR04
const byte PINO_I2C_SDA = A4;  // LCD SDA
const byte PINO_I2C_SCL = A5;  // LCD SCL

const byte ENDERECO_LCD = 0x27;          // AJUSTE AQUI: 0x27 ou 0x3F
const int DISTANCIA_PASSAGEM_CM = 45;    // AJUSTE AQUI: distância do vão de passagem (cm)

LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);

unsigned int totalPessoas = 0;
bool pessoaPassando = false;

long medirCm() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  unsigned long dur = pulseIn(PINO_ECHO, HIGH, 30000UL);
  if (dur == 0) return 400;
  return (long)(dur * 0.0343 / 2.0);
}

void atualizarTela() {
  lcd.setCursor(0, 0);
  lcd.print(F("Visitantes: "));
  lcd.print(totalPessoas);
  lcd.print(F("    "));

  lcd.setCursor(0, 1);
  if (pessoaPassando) {
    lcd.print(F("Passando...     "));
  } else {
    lcd.print(F("Corredor livre  "));
  }
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);

  lcd.init();
  lcd.backlight();
  atualizarTela();
  Serial.println(F("Sistema pronto! Contador de Pessoas ativo."));
}

void loop() {
  long cm = medirCm();

  if (!pessoaPassando && cm > 2 && cm < DISTANCIA_PASSAGEM_CM) {
    pessoaPassando = true;
    totalPessoas++;
    atualizarTela();
    Serial.print(F("Passagem detectada! Total de pessoas: "));
    Serial.println(totalPessoas);
  } else if (pessoaPassando && cm > (DISTANCIA_PASSAGEM_CM + 10)) {
    pessoaPassando = false;
    atualizarTela();
  }

  delay(80);
}
