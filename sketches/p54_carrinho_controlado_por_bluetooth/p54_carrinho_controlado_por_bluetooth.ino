/*
 * Projeto 54: Carrinho Controlado por Bluetooth
 * O que faz: Pilota um chassi 2WD com Ponte H L298N via comandos Bluetooth HC-05 ('F', 'B', 'L', 'R', 'S').
 * Ligações: D4/D7 (IN1/IN2), D8/D9 (IN3/IN4), D5 (ENA PWM), D6 (ENB PWM), D10 (BT TXD), D11 (BT RXD)
 * Bibliotecas: SoftwareSerial
 * Material: 80 Projetos Arduino
 */

#include <SoftwareSerial.h>

const byte PINO_IN1 = 4;    // Motor Esquerdo Direção 1
const byte PINO_ENA = 5;    // Motor Esquerdo Velocidade PWM
const byte PINO_ENB = 6;    // Motor Direito Velocidade PWM
const byte PINO_IN2 = 7;    // Motor Esquerdo Direção 2
const byte PINO_IN3 = 8;    // Motor Direito Direção 1
const byte PINO_IN4 = 9;    // Motor Direito Direção 2
const byte PINO_BT_RX = 10; // Recebe do TXD do HC-05
const byte PINO_BT_TX = 11; // Envia ao RXD do HC-05 (via divisor 1kΩ + 2kΩ)

const byte VELOCIDADE_NORMAL = 190; // AJUSTE AQUI: potência PWM (0 a 255)
const byte VELOCIDADE_CURVA = 160;

SoftwareSerial bt(PINO_BT_RX, PINO_BT_TX);

void pararMotores() {
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, LOW);
  analogWrite(PINO_ENA, 0);
  analogWrite(PINO_ENB, 0);
}

void moverFrente() {
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
  analogWrite(PINO_ENA, VELOCIDADE_NORMAL);
  analogWrite(PINO_ENB, VELOCIDADE_NORMAL);
}

void moverRe() {
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
  analogWrite(PINO_ENA, VELOCIDADE_NORMAL);
  analogWrite(PINO_ENB, VELOCIDADE_NORMAL);
}

void virarEsquerda() {
  digitalWrite(PINO_IN1, LOW);
  digitalWrite(PINO_IN2, HIGH);
  digitalWrite(PINO_IN3, HIGH);
  digitalWrite(PINO_IN4, LOW);
  analogWrite(PINO_ENA, VELOCIDADE_CURVA);
  analogWrite(PINO_ENB, VELOCIDADE_CURVA);
}

void virarDireita() {
  digitalWrite(PINO_IN1, HIGH);
  digitalWrite(PINO_IN2, LOW);
  digitalWrite(PINO_IN3, LOW);
  digitalWrite(PINO_IN4, HIGH);
  analogWrite(PINO_ENA, VELOCIDADE_CURVA);
  analogWrite(PINO_ENB, VELOCIDADE_CURVA);
}

void executarComando(char c) {
  if (c == '\r' || c == '\n' || c == ' ') return;
  switch (c) {
    case 'F':
    case 'f':
      moverFrente();
      Serial.println(F("Comando [F] -> Movendo para FRENTE"));
      break;
    case 'B':
    case 'b':
      moverRe();
      Serial.println(F("Comando [B] -> Movendo em RÉ"));
      break;
    case 'L':
    case 'l':
      virarEsquerda();
      Serial.println(F("Comando [L] -> Virando à ESQUERDA"));
      break;
    case 'R':
    case 'r':
      virarDireita();
      Serial.println(F("Comando [R] -> Virando à DIREITA"));
      break;
    case 'S':
    case 's':
      pararMotores();
      Serial.println(F("Comando [S] -> Motores PARADOS"));
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(9600);
  bt.begin(9600);

  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(PINO_ENA, OUTPUT);
  pinMode(PINO_ENB, OUTPUT);
  pararMotores();

  Serial.println(F("Sistema pronto! Carrinho Bluetooth 2WD ativo."));
  Serial.println(F("Comandos: F (Frente), B (Ré), L (Esquerda), R (Direita), S (Parar)"));
}

void loop() {
  if (bt.available() > 0) {
    executarComando((char)bt.read());
  }
  if (Serial.available() > 0) {
    executarComando((char)Serial.read());
  }
}
