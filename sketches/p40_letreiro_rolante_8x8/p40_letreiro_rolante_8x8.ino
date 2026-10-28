/*
 * Projeto 40: Letreiro Rolante 8x8
 * O que faz: Desliza horizontalmente a palavra "ARDUINO!" na matriz de LEDs 8x8 MAX7219.
 * Ligações: D11 (DIN), D10 (CS), D13 (CLK), 5V (VCC), GND (GND)
 * Bibliotecas: LedControl (Eberhard Fahle)
 * Material: 80 Projetos Arduino
 */

#include <LedControl.h>

const byte PINO_DIN = 11; // Entrada de dados DIN
const byte PINO_CS = 10;  // Chip Select CS
const byte PINO_CLK = 13; // Clock CLK

const unsigned int VELOCIDADE_SCROLL_MS = 85; // AJUSTE AQUI: menor valor = rolagem mais rápida

LedControl lc = LedControl(PINO_DIN, PINO_CLK, PINO_CS, 1);

// Cada letra tem 5 colunas de largura + 1 coluna vazia (0x00) de separação = 6 colunas por caractere
// Mensagem: "ARDUINO! " (9 caracteres = 54 colunas)
const byte COLUNAS_MENSAGEM[54] = {
  // A
  0b01111110, 0b10010000, 0b10010000, 0b10010000, 0b01111110, 0x00,
  // R
  0b11111110, 0b10010000, 0b10011000, 0b10010100, 0b01100010, 0x00,
  // D
  0b11111110, 0b10000010, 0b10000010, 0b10000010, 0b01111100, 0x00,
  // U
  0b11111100, 0b00000010, 0b00000010, 0b00000010, 0b11111100, 0x00,
  // I
  0b00000000, 0b10000010, 0b11111110, 0b10000010, 0b00000000, 0x00,
  // N
  0b11111110, 0b01000000, 0b00110000, 0b00001000, 0b11111110, 0x00,
  // O
  0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01111100, 0x00,
  // !
  0b00000000, 0b00000000, 0b11111010, 0b00000000, 0b00000000, 0x00,
  // Espaço final
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

byte bufferTela[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void empurrarColuna(byte novaColuna) {
  for (byte i = 0; i < 7; i++) {
    bufferTela[i] = bufferTela[i + 1];
  }
  bufferTela[7] = novaColuna;

  for (byte c = 0; c < 8; c++) {
    lc.setColumn(0, 7 - c, bufferTela[c]);
  }
}

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  Serial.println(F("Sistema pronto! Letreiro Rolante 8x8 ativo."));
}

void loop() {
  Serial.println(F("Rolando mensagem: ARDUINO!"));
  for (byte i = 0; i < 54; i++) {
    empurrarColuna(COLUNAS_MENSAGEM[i]);
    delay(VELOCIDADE_SCROLL_MS);
  }
  Serial.println(F("Ciclo da mensagem concluído."));
}
