/*
 * Projeto 43: Servo Controlado por Joystick
 * O que faz: Controla o ângulo de um servo SG90 pelo eixo X do joystick (A0) e centraliza ao clicar em SW (D2).
 * Ligações: A0 (Joystick VRx), D2 (Joystick SW), D9 (Sinal do Servo SG90)
 * Bibliotecas: Servo
 * Material: 80 Projetos Arduino
 */

#include <Servo.h>

const byte PINO_VRX = A0;  // Eixo X do joystick
const byte PINO_SW = 2;    // Botão central do joystick (INPUT_PULLUP)
const byte PINO_SERVO = 9; // Sinal do servo motor SG90

Servo servo;
int ultimoAngulo = -1;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_SW, INPUT_PULLUP);
  servo.attach(PINO_SERVO);
  servo.write(90);
  Serial.println(F("Sistema pronto! Servo Controlado por Joystick ativo."));
}

void loop() {
  if (digitalRead(PINO_SW) == LOW) {
    servo.write(90);
    ultimoAngulo = 90;
    Serial.println(F("Botão SW pressionado -> Centralizando em 90 graus!"));
    delay(250);
    return;
  }

  int leituraX = analogRead(PINO_VRX);
  int angulo = map(leituraX, 0, 1023, 0, 180);

  if (abs(angulo - ultimoAngulo) >= 2) {
    servo.write(angulo);
    ultimoAngulo = angulo;

    Serial.print(F("Eixo X (A0): "));
    Serial.print(leituraX);
    Serial.print(F(" -> Ângulo: "));
    Serial.print(angulo);
    Serial.println(F(" graus"));
  }

  delay(25);
}
