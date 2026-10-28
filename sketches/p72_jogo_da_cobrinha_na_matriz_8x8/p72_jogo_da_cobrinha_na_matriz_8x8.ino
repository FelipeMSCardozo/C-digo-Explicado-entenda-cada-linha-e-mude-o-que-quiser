// ============================================================================
// Projeto 72 — Jogo da Cobrinha na Matriz 8x8
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: LedControl
// Material: 80 Projetos Arduino
// Jogo Snake completo na Matriz de LEDs 8x8 MAX7219 controlado por Joystick
// ============================================================================

#include <LedControl.h>

const byte PINO_JOY_X = A0; // Eixo X do Joystick
const byte PINO_JOY_Y = A1; // Eixo Y do Joystick
const byte PINO_CS    = 10; // CS / LOAD do MAX7219
const byte PINO_CLK   = 11; // CLK do MAX7219
const byte PINO_DIN   = 12; // DIN do MAX7219

// AJUSTE AQUI: Tempo entre movimentos em ms (menor = mais rápido)
const unsigned long PASSO_MS = 260;

LedControl matriz = LedControl(PINO_DIN, PINO_CLK, PINO_CS, 1);

byte corpoX[64];
byte corpoY[64];
byte tamanho = 3;
int8_t dirX = 1;
int8_t dirY = 0;
byte macaX = 5;
byte macaY = 5;

unsigned long ultimoPasso = 0;

void sortearMaca() {
  bool ocupada = true;
  while (ocupada) {
    macaX = random(0, 8);
    macaY = random(0, 8);
    ocupada = false;
    for (byte i = 0; i < tamanho; i++) {
      if (corpoX[i] == macaX && corpoY[i] == macaY) {
        ocupada = true;
        break;
      }
    }
  }
}

void iniciarPartida() {
  tamanho = 3;
  corpoX[0] = 3; corpoY[0] = 4;
  corpoX[1] = 2; corpoY[1] = 4;
  corpoX[2] = 1; corpoY[2] = 4;
  dirX = 1;
  dirY = 0;
  sortearMaca();
}

void lerJoystick() {
  int x = analogRead(PINO_JOY_X);
  int y = analogRead(PINO_JOY_Y);

  if (x < 250 && dirX == 0) {
    dirX = -1; dirY = 0;
  } else if (x > 750 && dirX == 0) {
    dirX = 1; dirY = 0;
  } else if (y < 250 && dirY == 0) {
    dirX = 0; dirY = -1;
  } else if (y > 750 && dirY == 0) {
    dirX = 0; dirY = 1;
  }
}

void setup() {
  Serial.begin(9600);
  matriz.shutdown(0, false);
  matriz.setIntensity(0, 5);
  matriz.clearDisplay(0);

  randomSeed(analogRead(A2));
  iniciarPartida();
  Serial.println(F("Sistema pronto! Jogo da Cobrinha 8x8 iniciado."));
}

void loop() {
  lerJoystick();

  if (millis() - ultimoPasso >= PASSO_MS) {
    ultimoPasso = millis();

    byte novaX = (corpoX[0] + dirX + 8) % 8;
    byte novaY = (corpoY[0] + dirY + 8) % 8;

    // Verifica colisão com o próprio corpo
    for (byte i = 0; i < tamanho; i++) {
      if (corpoX[i] == novaX && corpoY[i] == novaY) {
        Serial.println(F("Fim de Jogo! Colisao com a cauda. Reiniciando..."));
        for (byte r = 0; r < 8; r++) matriz.setRow(0, r, 0xFF);
        delay(600);
        matriz.clearDisplay(0);
        iniciarPartida();
        return;
      }
    }

    // Verifica se comeu a maçã
    if (novaX == macaX && novaY == macaY) {
      if (tamanho < 60) tamanho++;
      Serial.print(F("Maca comida! Tamanho da cobrinha: "));
      Serial.println(tamanho);
      sortearMaca();
    }

    // Desloca os segmentos da cauda
    for (int i = tamanho - 1; i > 0; i--) {
      corpoX[i] = corpoX[i - 1];
      corpoY[i] = corpoY[i - 1];
    }
    corpoX[0] = novaX;
    corpoY[0] = novaY;

    // Desenha o quadro na matriz 8x8
    matriz.clearDisplay(0);
    for (byte i = 0; i < tamanho; i++) {
      matriz.setLed(0, corpoY[i], corpoX[i], true);
    }
    // Maçã acesa (pisca sutilmente a cada quadro par/ímpar)
    matriz.setLed(0, macaY, macaX, true);
  }
}
