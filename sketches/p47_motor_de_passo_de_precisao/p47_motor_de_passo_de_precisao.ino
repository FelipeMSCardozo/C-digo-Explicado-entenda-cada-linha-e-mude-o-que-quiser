/*
 * Projeto 47: Motor de Passo de Precisão
 * O que faz: Comanda o motor de passo 28BYJ-48 + driver ULN2003 dando 1 volta horária e 1 anti-horária.
 * Ligações: D8 (IN1), D9 (IN2), D10 (IN3), D11 (IN4), 5V (+), GND (-)
 * Bibliotecas: Stepper (Arduino)
 * Material: 80 Projetos Arduino
 */

#include <Stepper.h>

const byte PINO_IN1 = 8;  // Bobina 1 no driver ULN2003
const byte PINO_IN2 = 9;  // Bobina 2 no driver ULN2003
const byte PINO_IN3 = 10; // Bobina 3 no driver ULN2003
const byte PINO_IN4 = 11; // Bobina 4 no driver ULN2003

const int PASSOS_POR_VOLTA = 2048;
const int VELOCIDADE_RPM = 12; // AJUSTE AQUI: mantenha entre 5 e 15 RPM

// Importante: no 28BYJ-48 com a biblioteca Stepper, a ordem dos pinos no construtor é IN1, IN3, IN2, IN4!
Stepper motor(PASSOS_POR_VOLTA, PINO_IN1, PINO_IN3, PINO_IN2, PINO_IN4);

void setup() {
  Serial.begin(9600);
  motor.setSpeed(VELOCIDADE_RPM);
  Serial.println(F("Sistema pronto! Motor de Passo 28BYJ-48 iniciado."));
}

void loop() {
  Serial.println(F("Girando 1 volta completa no sentido HORÁRIO (+2048 passos)..."));
  motor.step(PASSOS_POR_VOLTA);
  delay(1000);

  Serial.println(F("Girando 1 volta completa no sentido ANTI-HORÁRIO (-2048 passos)..."));
  motor.step(-PASSOS_POR_VOLTA);
  delay(1000);
}
