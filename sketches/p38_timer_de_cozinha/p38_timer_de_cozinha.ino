/*
 * Projeto 38: Timer de Cozinha
 * O que faz: Temporizador regressivo no display de 4 dígitos TM1637 com botões +Tempo e Iniciar/Pausar.
 * Ligações: D2 (TM1637 CLK), D3 (TM1637 DIO), D4 (Botão +Tempo), D5 (Botão Iniciar), D8 (Buzzer Ativo)
 * Bibliotecas: TM1637 (Avishay Orpaz)
 * Material: 80 Projetos Arduino
 */

#include <TM1637Display.h>

const byte PINO_CLK = 2;       // Pino CLK do display TM1637
const byte PINO_DIO = 3;       // Pino DIO do display TM1637
const byte PINO_BTN_MAIS = 4;  // Botão para adicionar tempo
const byte PINO_BTN_START = 5; // Botão para iniciar / pausar
const byte PINO_BUZZER = 8;    // Buzzer ativo 5V

const int INCREMENTO_SEGUNDOS = 30; // AJUSTE AQUI: segundos adicionados por clique (30 ou 60)

TM1637Display display(PINO_CLK, PINO_DIO);

int segundosRestantes = 60; // Começa em 01:00
bool rodando = false;
unsigned long ultimoSegundoMs = 0;

void atualizarDisplay(bool doisPontosAceso) {
  int minutos = segundosRestantes / 60;
  int segundos = segundosRestantes % 60;
  int valorFormatado = (minutos * 100) + segundos;
  byte mascaraPontos = doisPontosAceso ? 0b01000000 : 0;
  display.showNumberDecEx(valorFormatado, mascaraPontos, true);
}

void tocarAlarmeFinal() {
  Serial.println(F("TEMPO ESGOTADO! (00:00)"));
  for (byte i = 0; i < 4; i++) {
    digitalWrite(PINO_BUZZER, HIGH);
    delay(120);
    digitalWrite(PINO_BUZZER, LOW);
    delay(80);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(120);
    digitalWrite(PINO_BUZZER, LOW);
    delay(350);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BTN_MAIS, INPUT_PULLUP);
  pinMode(PINO_BTN_START, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);

  display.setBrightness(0x0f); // Brilho máximo (0 a 7 / 0x0f)
  atualizarDisplay(true);
  Serial.println(F("Sistema pronto! Timer de Cozinha TM1637 iniciado em 01:00."));
}

void loop() {
  if (!rodando && digitalRead(PINO_BTN_MAIS) == LOW) {
    segundosRestantes += INCREMENTO_SEGUNDOS;
    if (segundosRestantes > 5999) segundosRestantes = 60;
    atualizarDisplay(true);
    Serial.print(F("Tempo ajustado para: "));
    Serial.print(segundosRestantes);
    Serial.println(F(" segundos"));
    delay(200);
  }

  if (digitalRead(PINO_BTN_START) == LOW) {
    if (segundosRestantes > 0) {
      rodando = !rodando;
      ultimoSegundoMs = millis();
      Serial.println(rodando ? F("Contagem regressiva iniciada!") : F("Timer pausado."));
    }
    delay(220);
  }

  if (rodando && (millis() - ultimoSegundoMs >= 1000)) {
    ultimoSegundoMs += 1000;
    segundosRestantes--;
    atualizarDisplay(segundosRestantes % 2 == 0);

    Serial.print(F("Restam: "));
    Serial.print(segundosRestantes / 60);
    Serial.print(':');
    if (segundosRestantes % 60 < 10) Serial.print('0');
    Serial.println(segundosRestantes % 60);

    if (segundosRestantes <= 0) {
      rodando = false;
      atualizarDisplay(true);
      tocarAlarmeFinal();
    }
  }
}
