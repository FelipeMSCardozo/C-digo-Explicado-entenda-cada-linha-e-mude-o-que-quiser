/*
 * Projeto 51: Lixeira Automática
 * O que faz: Detecta a aproximação da mão no HC-SR04 (< 20 cm), abre a tampa (Servo SG90) por 3s e fecha suave.
 * Ligações: D6 (Trig), D7 (Echo), D9 (Servo SG90)
 * Bibliotecas: Servo
 * Material: 80 Projetos Arduino
 */

#include <Servo.h>

const byte PINO_TRIG = 6;  // Trig do HC-SR04
const byte PINO_ECHO = 7;  // Echo do HC-SR04
const byte PINO_SERVO = 9; // Sinal do Servo SG90

const int DISTANCIA_ABRIR_CM = 20;          // AJUSTE AQUI: distância para abrir (cm)
const unsigned long TEMPO_ABERTA_MS = 3000; // AJUSTE AQUI: tempo aberta (ms)

Servo servoTampa;

long medirCm() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  unsigned long dur = pulseIn(PINO_ECHO, HIGH, 25000UL);
  if (dur == 0) return 400;
  return (long)(dur * 0.0343 / 2.0);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);

  servoTampa.attach(PINO_SERVO);
  servoTampa.write(0); // Tampa fechada
  Serial.println(F("Sistema pronto! Lixeira Automática iniciada."));
}

void loop() {
  long cm = medirCm();

  if (cm >= 2 && cm <= DISTANCIA_ABRIR_CM) {
    Serial.print(F("Mão detectada a "));
    Serial.print(cm);
    Serial.println(F(" cm -> Abrindo tampa!"));

    servoTampa.write(90);
    delay(TEMPO_ABERTA_MS);

    // Enquanto a mão continuar perto, mantém aberta
    while (medirCm() <= DISTANCIA_ABRIR_CM) {
      delay(300);
    }

    Serial.println(F("Fechando tampa suavemente..."));
    for (int ang = 90; ang >= 0; ang -= 2) {
      servoTampa.write(ang);
      delay(18);
    }
  }

  delay(100);
}
