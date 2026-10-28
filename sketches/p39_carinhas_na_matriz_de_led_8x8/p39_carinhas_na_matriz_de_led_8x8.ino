/*
 * Projeto 39: Carinhas na Matriz de LED 8x8
 * O que faz: Exibe expressões faciais e um coração na matriz de LEDs 8x8 controlada pelo MAX7219.
 * Ligações: D11 (DIN), D10 (CS), D13 (CLK), 5V (VCC), GND (GND)
 * Bibliotecas: LedControl (Eberhard Fahle)
 * Material: 80 Projetos Arduino
 */

#include <LedControl.h>

const byte PINO_DIN = 11; // Dados (Data In) do MAX7219
const byte PINO_CS = 10;  // Seleção (Chip Select / LOAD)
const byte PINO_CLK = 13; // Clock do MAX7219

const byte BRILHO_MATRIZ = 5; // AJUSTE AQUI: brilho de 0 a 15

// Parâmetros: (DIN, CLK, CS, quantidade de matrizes)
LedControl lc = LedControl(PINO_DIN, PINO_CLK, PINO_CS, 1);

const byte CARINHA_FELIZ[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

const byte CARINHA_PISCANDO[8] = {
  0b00111100,
  0b01000010,
  0b10000101,
  0b10110001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

const byte CARINHA_SURPRESA[8] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10011001,
  0b10011001,
  0b01000010,
  0b00111100
};

const byte CORACAO[8] = {
  0b00000000,
  0b01100110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000
};

void desenharIcone(const byte icone[8]) {
  for (byte linha = 0; linha < 8; linha++) {
    lc.setRow(0, linha, icone[linha]);
  }
}

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);       // Sai do modo de economia de energia
  lc.setIntensity(0, BRILHO_MATRIZ);
  lc.clearDisplay(0);
  Serial.println(F("Sistema pronto! Carinhas na Matriz 8x8 iniciadas."));
}

void loop() {
  Serial.println(F("Exibindo: Carinha Feliz"));
  desenharIcone(CARINHA_FELIZ);
  delay(1500);

  Serial.println(F("Exibindo: Piscadela"));
  desenharIcone(CARINHA_PISCANDO);
  delay(800);

  desenharIcone(CARINHA_FELIZ);
  delay(1000);

  Serial.println(F("Exibindo: Surpreso"));
  desenharIcone(CARINHA_SURPRESA);
  delay(1500);

  Serial.println(F("Exibindo: Coração"));
  desenharIcone(CORACAO);
  delay(1500);
}
