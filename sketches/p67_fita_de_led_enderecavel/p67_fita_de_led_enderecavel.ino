// ============================================================================
// Projeto 67 — Fita de LED Endereçável
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Adafruit NeoPixel
// Material: 80 Projetos Arduino
// Barra de 8 LEDs WS2812B (NeoPixel) com resistor em série no DIN e botão D2
// ============================================================================

#include <Adafruit_NeoPixel.h>

const byte PINO_BOTAO    = 2; // Botão seletor de efeitos (INPUT_PULLUP)
const byte PINO_NEOPIXEL = 6; // Entrada DIN da barra WS2812 (via resistor 220Ω)

const byte NUM_LEDS = 8;

// AJUSTE AQUI: Brilho limitado a 50 (de 255) para consumo seguro na porta USB
const byte BRILHO_SEGURO = 50;

Adafruit_NeoPixel fita(NUM_LEDS, PINO_NEOPIXEL, NEO_GRB + NEO_KHZ800);

byte modoEfeito = 0;
uint16_t passoMatiz = 0;
byte posCometa = 0;
unsigned long ultimoQuadro = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO, INPUT_PULLUP);

  fita.begin();
  fita.setBrightness(BRILHO_SEGURO);
  fita.clear();
  fita.show();

  Serial.println(F("Sistema pronto! Fita WS2812B (8 LEDs) iniciada."));
}

void loop() {
  if (digitalRead(PINO_BOTAO) == LOW) {
    modoEfeito = (modoEfeito + 1) % 4;
    fita.clear();
    Serial.print(F("Efeito alterado para modo: "));
    Serial.println(modoEfeito);
    delay(250);
  }

  if (millis() - ultimoQuadro >= 60) {
    ultimoQuadro = millis();

    if (modoEfeito == 0) {
      // Efeito 0: Arco-Íris Dinâmico
      for (byte i = 0; i < NUM_LEDS; i++) {
        uint16_t matiz = passoMatiz + (i * 65536L / NUM_LEDS);
        fita.setPixelColor(i, fita.gamma32(fita.ColorHSV(matiz)));
      }
      passoMatiz += 1024;
    } else if (modoEfeito == 1) {
      // Efeito 1: Cometa Azul Ciano
      fita.clear();
      fita.setPixelColor(posCometa, fita.Color(0, 220, 255));
      fita.setPixelColor((posCometa + NUM_LEDS - 1) % NUM_LEDS, fita.Color(0, 60, 90));
      fita.setPixelColor((posCometa + NUM_LEDS - 2) % NUM_LEDS, fita.Color(0, 15, 25));
      posCometa = (posCometa + 1) % NUM_LEDS;
    } else if (modoEfeito == 2) {
      // Efeito 2: Giroflex Bicolor
      bool fase = ((millis() / 250) % 2 == 0);
      for (byte i = 0; i < NUM_LEDS; i++) {
        if (i < NUM_LEDS / 2) {
          fita.setPixelColor(i, fase ? fita.Color(255, 0, 0) : fita.Color(0, 0, 0));
        } else {
          fita.setPixelColor(i, fase ? fita.Color(0, 0, 0) : fita.Color(0, 40, 255));
        }
      }
    } else {
      // Efeito 3: Luminária Branco Quente
      for (byte i = 0; i < NUM_LEDS; i++) {
        fita.setPixelColor(i, fita.Color(255, 180, 90));
      }
    }

    fita.show();
  }
}
