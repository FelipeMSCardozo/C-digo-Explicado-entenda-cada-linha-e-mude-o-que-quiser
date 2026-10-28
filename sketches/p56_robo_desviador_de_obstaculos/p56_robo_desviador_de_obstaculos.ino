/*
 * Projeto 56: Robô Desviador de Obstáculos
 * O que faz: Avança com chassi 2WD e, ao detectar obstáculo (< 25 cm), usa o servo + HC-SR04 para escolher o lado livre.
 * Ligações: D3 (Servo), D11 (Trig), D12 (Echo), D5 (ENA PWM), D6 (ENB PWM), D4/D7 (IN1/IN2), D8/A0 (IN3/IN4)
 * Bibliotecas: Servo
 * Material: 80 Projetos Arduino
 */

#include <Servo.h>

const byte PINO_SERVO = 3; // Servo do pescoço do radar
const byte PINO_IN1 = 4;   // Motor Esquerdo IN1
const byte PINO_ENA = 5;   // Motor Esquerdo PWM (Timer0 - não conflita com Servo!)
const byte PINO_ENB = 6;   // Motor Direito PWM (Timer0 - não conflita com Servo!)
const byte PINO_IN2 = 7;   // Motor Esquerdo IN2
const byte PINO_IN3 = 8;   // Motor Direito IN3
const byte PINO_IN4 = A0;  // Motor Direito IN4
const byte PINO_TRIG = 11; // Trig do HC-SR04
const byte PINO_ECHO = 12; // Echo do HC-SR04

const int DISTANCIA_MINIMA_CM = 25; // AJUSTE AQUI: distância para parar e desviar (cm)
const byte VEL_FRENTE = 175;
const byte VEL_GIRO = 185;

Servo pescoco;

long medirDistanciaCm() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  unsigned long dur = pulseIn(PINO_ECHO, HIGH, 25000UL);
  if (dur == 0) return 300;
  return (long)(dur * 0.0343 / 2.0);
}

void parar() {
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  analogWrite(PINO_ENA, 0);
  analogWrite(PINO_ENB, 0);
}

void frente() {
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
  analogWrite(PINO_ENA, VEL_FRENTE);
  analogWrite(PINO_ENB, VEL_FRENTE);
}

void recuar() {
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
  analogWrite(PINO_ENA, VEL_FRENTE);
  analogWrite(PINO_ENB, VEL_FRENTE);
}

void girarEsquerda() {
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
  analogWrite(PINO_ENA, VEL_GIRO);
  analogWrite(PINO_ENB, VEL_GIRO);
}

void girarDireita() {
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
  analogWrite(PINO_ENA, VEL_GIRO);
  analogWrite(PINO_ENB, VEL_GIRO);
}

long olharParaAngulo(int angulo) {
  pescoco.write(angulo);
  delay(350);
  long d = medirDistanciaCm();
  pescoco.write(90);
  delay(200);
  return d;
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(PINO_ENA, OUTPUT);
  pinMode(PINO_ENB, OUTPUT);

  pescoco.attach(PINO_SERVO);
  pescoco.write(90);
  parar();
  delay(1000);
  Serial.println(F("Sistema pronto! Robô Desviador de Obstáculos ativo."));
}

void loop() {
  long distFrente = medirDistanciaCm();

  if (distFrente < DISTANCIA_MINIMA_CM) {
    parar();
    Serial.print(F("Obstáculo a "));
    Serial.print(distFrente);
    Serial.println(F(" cm! Parando e avaliando rota..."));

    recuar();
    delay(300);
    parar();
    delay(200);

    long distEsq = olharParaAngulo(150);
    long distDir = olharParaAngulo(30);

    Serial.print(F("Distância Esquerda: "));
    Serial.print(distEsq);
    Serial.print(F(" cm | Distância Direita: "));
    Serial.print(distDir);
    Serial.println(F(" cm"));

    if (distEsq >= distDir) {
      Serial.println(F("Decisão: Virando para a ESQUERDA!"));
      girarEsquerda();
    } else {
      Serial.println(F("Decisão: Virando para a DIREITA!"));
      girarDireita();
    }
    delay(420);
    parar();
    delay(150);
  } else {
    frente();
  }

  delay(60);
}
