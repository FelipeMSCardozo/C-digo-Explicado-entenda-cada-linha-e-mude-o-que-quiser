/*
 * Projeto 45: Braço Robótico de 2 Eixos
 * O que faz: Controla 2 servos SG90 (Base e Ombro) de forma incremental com o joystick e fonte 5V externa.
 * Ligações: A0 (VRx), A1 (VRy), D2 (SW), D9 (Servo Base), D10 (Servo Ombro); Fonte 5V externa com GND comum
 * Bibliotecas: Servo
 * Material: 80 Projetos Arduino
 */

#include <Servo.h>

const byte PINO_VRX = A0;        // Eixo X do joystick (Base)
const byte PINO_VRY = A1;        // Eixo Y do joystick (Ombro)
const byte PINO_SW = 2;          // Botão central do joystick (Home)
const byte PINO_SERVO_BASE = 9;  // Sinal do Servo 1 (Base)
const byte PINO_SERVO_OMBRO = 10; // Sinal do Servo 2 (Ombro)

const int PASSO_GRAUS = 2; // AJUSTE AQUI: velocidade angular por ciclo

Servo servoBase;
Servo servoOmbro;

int angBase = 90;
int angOmbro = 90;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_SW, INPUT_PULLUP);

  servoBase.attach(PINO_SERVO_BASE);
  servoOmbro.attach(PINO_SERVO_OMBRO);
  servoBase.write(angBase);
  servoOmbro.write(angOmbro);

  Serial.println(F("Sistema pronto! Braço Robótico de 2 Eixos ativo."));
}

void loop() {
  if (digitalRead(PINO_SW) == LOW) {
    angBase = 90;
    angOmbro = 90;
    servoBase.write(angBase);
    servoOmbro.write(angOmbro);
    Serial.println(F("Botão SW pressionado -> Retornando para posição Home (90, 90)."));
    delay(300);
    return;
  }

  int x = analogRead(PINO_VRX);
  int y = analogRead(PINO_VRY);
  bool moveu = false;

  if (x > 620) {
    angBase = constrain(angBase + PASSO_GRAUS, 10, 170);
    moveu = true;
  } else if (x < 400) {
    angBase = constrain(angBase - PASSO_GRAUS, 10, 170);
    moveu = true;
  }

  if (y > 620) {
    angOmbro = constrain(angOmbro + PASSO_GRAUS, 15, 165);
    moveu = true;
  } else if (y < 400) {
    angOmbro = constrain(angOmbro - PASSO_GRAUS, 15, 165);
    moveu = true;
  }

  if (moveu) {
    servoBase.write(angBase);
    servoOmbro.write(angOmbro);
    Serial.print(F("Posição -> Base: "));
    Serial.print(angBase);
    Serial.print(F(" graus | Ombro: "));
    Serial.print(angOmbro);
    Serial.println(F(" graus"));
  }

  delay(30);
}
