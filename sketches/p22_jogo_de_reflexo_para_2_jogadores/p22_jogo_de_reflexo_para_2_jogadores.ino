/*
 * Projeto 22: Jogo de Reflexo para 2 Jogadores
 * O que faz: Aguarda tempo aleatório, acende o LED verde e mede quem aperta o botão primeiro (ou queima a largada).
 * Ligações: D2 (Botão J1), D3 (Botão J2), D8 (LED J1), D9 (LED Verde Central), D10 (LED J2), D11 (Buzzer)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BOTAO_J1 = 2;  // Botão do Jogador 1 (esquerda)
const byte PINO_BOTAO_J2 = 3;  // Botão do Jogador 2 (direita)
const byte PINO_LED_J1 = 8;    // LED Azul do Jogador 1
const byte PINO_LED_SINAL = 9; // LED Verde central de largada
const byte PINO_LED_J2 = 10;   // LED Vermelho do Jogador 2
const byte PINO_BUZZER = 11;   // Buzzer passivo

int pontosJ1 = 0;
int pontosJ2 = 0;

void comemorar(byte pinoLed, byte jogador, unsigned long tempoMs, bool queimou) {
  digitalWrite(PINO_LED_SINAL, LOW);
  if (queimou) {
    tone(PINO_BUZZER, 200, 500);
    Serial.print(F("QUEIMOU A LARGADA! Ponto para o Jogador "));
    Serial.println(jogador);
  } else {
    tone(PINO_BUZZER, 880, 250);
    Serial.print(F("Vencedor: Jogador "));
    Serial.print(jogador);
    Serial.print(F("! Tempo de reação: "));
    Serial.print(tempoMs);
    Serial.println(F(" ms"));
  }

  for (byte i = 0; i < 6; i++) {
    digitalWrite(pinoLed, HIGH);
    delay(150);
    digitalWrite(pinoLed, LOW);
    delay(150);
  }

  Serial.print(F("Placar -> J1: "));
  Serial.print(pontosJ1);
  Serial.print(F(" | J2: "));
  Serial.println(pontosJ2);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO_J1, INPUT_PULLUP);
  pinMode(PINO_BOTAO_J2, INPUT_PULLUP);
  pinMode(PINO_LED_J1, OUTPUT);
  pinMode(PINO_LED_SINAL, OUTPUT);
  pinMode(PINO_LED_J2, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  randomSeed(analogRead(A0));
  Serial.println(F("Sistema pronto! Jogo de Reflexo para 2 Jogadores."));
}

void loop() {
  Serial.println(F("Nova rodada: Preparem-se (nao apertem ainda)..."));
  delay(1000);

  unsigned long espera = (unsigned long)random(2000, 5000);
  unsigned long inicioEspera = millis();

  // Monitora queimada de largada durante a espera
  while (millis() - inicioEspera < espera) {
    if (digitalRead(PINO_BOTAO_J1) == LOW) {
      pontosJ2++;
      comemorar(PINO_LED_J2, 2, 0, true);
      return;
    }
    if (digitalRead(PINO_BOTAO_J2) == LOW) {
      pontosJ1++;
      comemorar(PINO_LED_J1, 1, 0, true);
      return;
    }
  }

  // Sinal de largada!
  Serial.println(F("SINAL VERDE: AGORA!"));
  digitalWrite(PINO_LED_SINAL, HIGH);
  tone(PINO_BUZZER, 1000, 120);
  unsigned long instanteLargada = millis();

  while (true) {
    if (digitalRead(PINO_BOTAO_J1) == LOW) {
      unsigned long reacao = millis() - instanteLargada;
      pontosJ1++;
      comemorar(PINO_LED_J1, 1, reacao, false);
      break;
    }
    if (digitalRead(PINO_BOTAO_J2) == LOW) {
      unsigned long reacao = millis() - instanteLargada;
      pontosJ2++;
      comemorar(PINO_LED_J2, 2, reacao, false);
      break;
    }
  }

  delay(1500);
}
