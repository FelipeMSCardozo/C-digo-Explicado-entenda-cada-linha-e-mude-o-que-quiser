/*
 * Projeto 27: Detector de Gás e Fumaça
 * O que faz: Lê a saída analógica do sensor MQ-2 em A0 e dispara LED + buzzer se passar do limiar.
 * Ligações: A0 (MQ-2 AO), D8 (LED Vermelho via resistor 220 Ω), D9 (Buzzer Ativo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_MQ2 = A0;    // Saída analógica AO do sensor MQ-2
const byte PINO_LED = 8;     // LED vermelho de alerta
const byte PINO_BUZZER = 9;  // Buzzer ativo 5V

const int LIMIAR_GAS = 380;  // AJUSTE AQUI: valor relativo (0-1023) para disparo do alerta

void setup() {
  Serial.begin(9600);
  pinMode(PINO_LED, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  Serial.println(F("Pré-aquecendo sensor MQ-2 (aguarde estabilização inicial)..."));
  for (byte i = 0; i < 10; i++) {
    digitalWrite(PINO_LED, HIGH);
    delay(100);
    digitalWrite(PINO_LED, LOW);
    delay(400);
  }
  Serial.println(F("Sistema pronto! Detector de Gás e Fumaça ativo."));
}

void loop() {
  int leitura = analogRead(PINO_MQ2);

  Serial.print(F("Leitura relativa A0: "));
  Serial.print(leitura);

  if (leitura >= LIMIAR_GAS) {
    Serial.println(F(" -> ALERTA: Gás ou vapor detectado!"));
    digitalWrite(PINO_LED, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(200);
    digitalWrite(PINO_BUZZER, LOW);
    delay(150);
  } else {
    Serial.println(F(" (Ar limpo)"));
    digitalWrite(PINO_LED, LOW);
    digitalWrite(PINO_BUZZER, LOW);
    delay(400);
  }
}
