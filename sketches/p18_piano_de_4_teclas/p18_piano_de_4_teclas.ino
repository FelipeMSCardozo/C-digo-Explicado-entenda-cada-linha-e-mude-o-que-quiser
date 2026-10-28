/*
 * Projeto 18: Piano de 4 Teclas
 * O que faz: Toca as notas Dó, Ré, Mi e Fá no buzzer passivo enquanto os botões correspondentes são pressionados.
 * Ligações: D2 (Botão Dó), D3 (Botão Ré), D4 (Botão Mi), D5 (Botão Fá), D8 (Buzzer Passivo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_TECLA_DO = 2; // Botão 1: Nota Dó (262 Hz)
const byte PINO_TECLA_RE = 3; // Botão 2: Nota Ré (294 Hz)
const byte PINO_TECLA_MI = 4; // Botão 3: Nota Mi (330 Hz)
const byte PINO_TECLA_FA = 5; // Botão 4: Nota Fá (349 Hz)
const byte PINO_BUZZER = 8;   // Buzzer passivo

int notaAtual = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_TECLA_DO, INPUT_PULLUP);
  pinMode(PINO_TECLA_RE, INPUT_PULLUP);
  pinMode(PINO_TECLA_MI, INPUT_PULLUP);
  pinMode(PINO_TECLA_FA, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);

  Serial.println(F("Sistema pronto! Piano de 4 Teclas (Dó, Ré, Mi, Fá) ativo."));
}

void loop() {
  int novaNota = 0;

  if (digitalRead(PINO_TECLA_DO) == LOW) {
    novaNota = 262;
  } else if (digitalRead(PINO_TECLA_RE) == LOW) {
    novaNota = 294;
  } else if (digitalRead(PINO_TECLA_MI) == LOW) {
    novaNota = 330;
  } else if (digitalRead(PINO_TECLA_FA) == LOW) {
    novaNota = 349;
  }

  if (novaNota > 0) {
    if (novaNota != notaAtual) {
      tone(PINO_BUZZER, novaNota);
      notaAtual = novaNota;
      Serial.print(F("Tocando frequência: "));
      Serial.print(novaNota);
      Serial.println(F(" Hz"));
    }
  } else if (notaAtual != 0) {
    noTone(PINO_BUZZER);
    notaAtual = 0;
  }

  delay(10);
}
