/*
 * Projeto 44: Radar Ultrassônico
 * O que faz: Varre o ambiente de 15 a 165 graus com o servo + HC-SR04 e plota no Plotter Serial.
 * Ligações: D6 (Trig), D7 (Echo), D9 (Sinal do Servo SG90)
 * Bibliotecas: Servo
 * Material: 80 Projetos Arduino
 */

#include <Servo.h>

const byte PINO_TRIG = 6;  // Trig do HC-SR04
const byte PINO_ECHO = 7;  // Echo do HC-SR04
const byte PINO_SERVO = 9; // Sinal do servo motor da torre

const byte PASSO_GRAUS = 3; // AJUSTE AQUI: graus por passo da varredura

Servo motorRadar;

long medirCmLimitado() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  unsigned long dur = pulseIn(PINO_ECHO, HIGH, 20000UL);
  if (dur == 0) return 100;
  long cm = (long)(dur * 0.0343 / 2.0);
  if (cm > 100) cm = 100;
  return cm;
}

void medirEImprimir(int angulo) {
  motorRadar.write(angulo);
  delay(35); // Aguarda o servo estabilizar no ângulo
  long cm = medirCmLimitado();

  // Formato compatível tanto com o Monitor Serial quanto com o Plotter Serial
  Serial.print(F("Distancia_cm:"));
  Serial.print(cm);
  Serial.print(F(" Angulo_graus:"));
  Serial.println(angulo);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  motorRadar.attach(PINO_SERVO);
}

void loop() {
  for (int ang = 15; ang <= 165; ang += PASSO_GRAUS) {
    medirEImprimir(ang);
  }
  for (int ang = 165; ang >= 15; ang -= PASSO_GRAUS) {
    medirEImprimir(ang);
  }
}
