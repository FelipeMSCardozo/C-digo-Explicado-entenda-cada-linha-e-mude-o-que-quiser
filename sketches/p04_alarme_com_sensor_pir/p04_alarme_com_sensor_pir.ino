/*
 * Projeto 04: Alarme com Sensor PIR
 * O que faz: Detecta presença humana com o sensor PIR HC-SR501 e aciona LED + buzzer ativo.
 * Ligações: D2 (OUT do PIR), D8 (LED Vermelho com resistor 220 Ω), D9 (Buzzer Ativo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_PIR = 2;      // Saída digital OUT do sensor PIR HC-SR501
const byte PINO_LED = 8;      // LED vermelho indicador de disparo
const byte PINO_BUZZER = 9;   // Buzzer ativo 5V

bool movimentoAnterior = false;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_PIR, INPUT);
  pinMode(PINO_LED, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  Serial.println(F("Calibrando sensor PIR (aguarde 30s)..."));
  // Pisca o LED lentamente durante os primeiros segundos de estabilização
  for (byte i = 0; i < 10; i++) {
    digitalWrite(PINO_LED, HIGH);
    delay(150);
    digitalWrite(PINO_LED, LOW);
    delay(150);
  }
  Serial.println(F("Sistema pronto! Alarme PIR ativo."));
}

void loop() {
  bool movimentoAtual = (digitalRead(PINO_PIR) == HIGH);

  if (movimentoAtual) {
    if (!movimentoAnterior) {
      Serial.println(F("ALERTA: Movimento detectado!"));
      movimentoAnterior = true;
    }
    // Bipe intermitente de alarme
    digitalWrite(PINO_LED, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(150);
    digitalWrite(PINO_BUZZER, LOW);
    delay(150);
  } else {
    if (movimentoAnterior) {
      Serial.println(F("Ambiente seguro (sem movimento)."));
      movimentoAnterior = false;
    }
    digitalWrite(PINO_LED, LOW);
    digitalWrite(PINO_BUZZER, LOW);
    delay(100);
  }
}
