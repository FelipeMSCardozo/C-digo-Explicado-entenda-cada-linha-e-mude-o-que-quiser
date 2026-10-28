/*
 * Projeto 53: Controle por Bluetooth pelo Celular
 * O que faz: Recebe comandos ('A', 'a', 'B', 'b', 'T', 't') via Bluetooth HC-05 (ou Monitor Serial) e aciona 2 LEDs.
 * Ligações: D10 (HC-05 TXD), D11 (HC-05 RXD via divisor 1kΩ+2kΩ), D8 (LED Verde), D9 (LED Azul)
 * Bibliotecas: SoftwareSerial (Arduino nativa)
 * Material: 80 Projetos Arduino
 */

#include <SoftwareSerial.h>

const byte PINO_BT_RX = 10;    // Recebe do TXD do módulo HC-05
const byte PINO_BT_TX = 11;    // Envia para o RXD do HC-05 (via divisor 1kΩ + 2kΩ)
const byte PINO_LED_VERDE = 8; // LED Canal A
const byte PINO_LED_AZUL = 9;  // LED Canal B

SoftwareSerial bluetooth(PINO_BT_RX, PINO_BT_TX);

void processarComando(char cmd) {
  if (cmd == '\r' || cmd == '\n' || cmd == ' ') return;

  switch (cmd) {
    case 'A':
      digitalWrite(PINO_LED_VERDE, HIGH);
      Serial.println(F("Comando [A] -> LED Verde LIGADO"));
      bluetooth.println(F("LED Verde: LIGADO"));
      break;
    case 'a':
      digitalWrite(PINO_LED_VERDE, LOW);
      Serial.println(F("Comando [a] -> LED Verde DESLIGADO"));
      bluetooth.println(F("LED Verde: DESLIGADO"));
      break;
    case 'B':
      digitalWrite(PINO_LED_AZUL, HIGH);
      Serial.println(F("Comando [B] -> LED Azul LIGADO"));
      bluetooth.println(F("LED Azul: LIGADO"));
      break;
    case 'b':
      digitalWrite(PINO_LED_AZUL, LOW);
      Serial.println(F("Comando [b] -> LED Azul DESLIGADO"));
      bluetooth.println(F("LED Azul: DESLIGADO"));
      break;
    case 'T':
      digitalWrite(PINO_LED_VERDE, HIGH);
      digitalWrite(PINO_LED_AZUL, HIGH);
      Serial.println(F("Comando [T] -> Ambos LIGADOS"));
      bluetooth.println(F("Todos os LEDs: LIGADOS"));
      break;
    case 't':
      digitalWrite(PINO_LED_VERDE, LOW);
      digitalWrite(PINO_LED_AZUL, LOW);
      Serial.println(F("Comando [t] -> Ambos DESLIGADOS"));
      bluetooth.println(F("Todos os LEDs: DESLIGADOS"));
      break;
    default:
      Serial.print(F("Comando desconhecido: "));
      Serial.println(cmd);
      break;
  }
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_AZUL, OUTPUT);

  Serial.println(F("Sistema pronto! Controle Bluetooth HC-05 ativo (9600 baud)."));
  Serial.println(F("Envie: A/a (LED Verde), B/b (LED Azul), T/t (Todos)"));
}

void loop() {
  if (bluetooth.available() > 0) {
    processarComando((char)bluetooth.read());
  }
  if (Serial.available() > 0) {
    processarComando((char)Serial.read());
  }
}
