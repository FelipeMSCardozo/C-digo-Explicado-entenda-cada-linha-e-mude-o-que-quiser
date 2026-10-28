/*
 * Projeto 03: Controle de Servo Motor
 * O que faz: Lê a posição de um potenciômetro em A0 e posiciona o servo SG90 entre 0 e 180 graus.
 * Ligações: A0 (Pino central do potenciômetro), D9 (Sinal do Servo SG90)
 * Bibliotecas: Servo (nativa / Arduino)
 * Material: 80 Projetos Arduino
 */

#include <Servo.h>

const byte PINO_POT = A0;   // Pino central do potenciômetro 10 kΩ
const byte PINO_SERVO = 9;  // Fio laranja/amarelo de sinal do servo SG90

Servo meuServo;
int ultimoAngulo = -1;

void setup() {
  Serial.begin(9600);
  meuServo.attach(PINO_SERVO);
  Serial.println(F("Sistema pronto! Controle de Servo Motor iniciado."));
}

void loop() {
  int leitura = analogRead(PINO_POT);
  int angulo = map(leitura, 0, 1023, 0, 180);

  // Atualiza e imprime quando houver mudança significativa para evitar trepidação
  if (abs(angulo - ultimoAngulo) >= 1) {
    meuServo.write(angulo);
    ultimoAngulo = angulo;

    Serial.print(F("Leitura A0: "));
    Serial.print(leitura);
    Serial.print(F(" -> Ângulo do Servo: "));
    Serial.print(angulo);
    Serial.println(F(" graus"));
  }

  delay(25);
}
