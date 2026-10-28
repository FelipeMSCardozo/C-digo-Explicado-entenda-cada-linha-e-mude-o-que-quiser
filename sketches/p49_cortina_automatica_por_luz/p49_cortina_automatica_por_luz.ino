/*
 * Projeto 49: Cortina Automática por Luz
 * O que faz: Abre a cortina (motor de passo 28BYJ-48) ao clarear e fecha ao escurecer, desligando as bobinas em repouso.
 * Ligações: A0 (Divisor LDR + 10 kΩ), D8..D11 (IN1..IN4 do ULN2003)
 * Bibliotecas: Stepper
 * Material: 80 Projetos Arduino
 */

#include <Stepper.h>

const byte PINO_LDR = A0; // Divisor de tensão do LDR
const byte PINO_IN1 = 8;  // IN1 do ULN2003
const byte PINO_IN2 = 9;  // IN2 do ULN2003
const byte PINO_IN3 = 10; // IN3 do ULN2003
const byte PINO_IN4 = 11; // IN4 do ULN2003

const int LIMIAR_DIA = 600;   // AJUSTE AQUI: acima disso abre a cortina
const int LIMIAR_NOITE = 350; // AJUSTE AQUI: abaixo disso fecha a cortina
const int PASSOS_CURSO = 2048; // 1 volta completa

Stepper motorCortina(2048, PINO_IN1, PINO_IN3, PINO_IN2, PINO_IN4);
bool cortinaAberta = false;

void desligarBobinas() {
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
}

void setup() {
  Serial.begin(9600);
  motorCortina.setSpeed(12);
  Serial.println(F("Sistema pronto! Cortina Automática por Luz iniciada."));
}

void loop() {
  int luz = analogRead(PINO_LDR);

  Serial.print(F("Luz A0: "));
  Serial.print(luz);

  if (!cortinaAberta && luz > LIMIAR_DIA) {
    Serial.println(F(" -> Amanheceu! Abrindo a cortina..."));
    motorCortina.step(PASSOS_CURSO);
    desligarBobinas();
    cortinaAberta = true;
    Serial.println(F("Estado atual: CORTINA ABERTA"));
  } else if (cortinaAberta && luz < LIMIAR_NOITE) {
    Serial.println(F(" -> Anoiteceu! Fechando a cortina..."));
    motorCortina.step(-PASSOS_CURSO);
    desligarBobinas();
    cortinaAberta = false;
    Serial.println(F("Estado atual: CORTINA FECHADA"));
  } else {
    Serial.println(cortinaAberta ? F(" (Mantendo ABERTA)") : F(" (Mantendo FECHADA)"));
  }

  delay(1000);
}
