/*
 * Projeto 23: Jogo da Memória de Cores
 * O que faz: Sorteia sequências crescentes de 4 cores/sons e desafia o jogador a repetir nos 4 botões.
 * Ligações: D2..D5 (4 Botões), D6..D9 (4 LEDs via resistores 220 Ω), D10 (Buzzer Passivo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BTN_1 = 2; // Botão Vermelho
const byte PINO_BTN_2 = 3; // Botão Verde
const byte PINO_BTN_3 = 4; // Botão Azul
const byte PINO_BTN_4 = 5; // Botão Amarelo
const byte PINO_LED_1 = 6; // LED Vermelho
const byte PINO_LED_2 = 7; // LED Verde
const byte PINO_LED_3 = 8; // LED Azul
const byte PINO_LED_4 = 9; // LED Amarelo
const byte PINO_BUZZER = 10; // Buzzer passivo

const byte PINOS_BOTOES[4] = {PINO_BTN_1, PINO_BTN_2, PINO_BTN_3, PINO_BTN_4};
const byte PINOS_LEDS[4] = {PINO_LED_1, PINO_LED_2, PINO_LED_3, PINO_LED_4};
const unsigned int NOTAS[4] = {262, 330, 392, 523};

byte sequencia[32];
byte nivelAtual = 0;

void tocarCor(byte cor, unsigned int duracaoMs) {
  digitalWrite(PINOS_LEDS[cor], HIGH);
  tone(PINO_BUZZER, NOTAS[cor], duracaoMs);
  delay(duracaoMs);
  digitalWrite(PINOS_LEDS[cor], LOW);
  noTone(PINO_BUZZER);
  delay(120);
}

byte aguardarBotao() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      if (digitalRead(PINOS_BOTOES[i]) == LOW) {
        tocarCor(i, 250);
        while (digitalRead(PINOS_BOTOES[i]) == LOW) {
          delay(10);
        }
        return i;
      }
    }
  }
}

void efeitoErro() {
  Serial.println(F("Sequência incorreta! Fim de jogo. Reiniciando..."));
  for (byte r = 0; r < 3; r++) {
    for (byte i = 0; i < 4; i++) digitalWrite(PINOS_LEDS[i], HIGH);
    tone(PINO_BUZZER, 150, 250);
    delay(280);
    for (byte i = 0; i < 4; i++) digitalWrite(PINOS_LEDS[i], LOW);
    delay(100);
  }
  nivelAtual = 0;
  delay(1000);
}

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 4; i++) {
    pinMode(PINOS_BOTOES[i], INPUT_PULLUP);
    pinMode(PINOS_LEDS[i], OUTPUT);
  }
  pinMode(PINO_BUZZER, OUTPUT);
  randomSeed(analogRead(A0));
  Serial.println(F("Sistema pronto! Jogo da Memória de Cores iniciado."));
  delay(800);
}

void loop() {
  sequencia[nivelAtual] = (byte)random(0, 4);
  nivelAtual++;

  Serial.print(F("Nível "));
  Serial.print(nivelAtual);
  Serial.println(F(": observe a sequência..."));

  for (byte i = 0; i < nivelAtual; i++) {
    tocarCor(sequencia[i], 350);
  }

  Serial.println(F("Sua vez! Repita os passos."));
  for (byte i = 0; i < nivelAtual; i++) {
    byte escolha = aguardarBotao();
    if (escolha != sequencia[i]) {
      efeitoErro();
      return;
    }
  }

  Serial.println(F("Correto! Avançando para o próximo nível."));
  if (nivelAtual >= 30) nivelAtual = 0;
  delay(700);
}
