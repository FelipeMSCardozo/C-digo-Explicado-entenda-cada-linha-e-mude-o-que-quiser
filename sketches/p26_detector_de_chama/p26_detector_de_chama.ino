/*
 * Projeto 26: Detector de Chama
 * O que faz: Monitora a radiação infravermelha no sensor de chama em A0 e dispara o buzzer ativo em D8.
 * Ligações: A0 (Pino AO do sensor de chama), D8 (Buzzer Ativo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_CHAMA = A0;  // Saída analógica AO do sensor de chama
const byte PINO_BUZZER = 8;  // Buzzer ativo 5V

const int LIMIAR_CHAMA = 500; // AJUSTE AQUI: leituras menores que este valor disparam o alarme

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);
  Serial.println(F("Sistema pronto! Detector de Chama ativo."));
}

void loop() {
  int leitura = analogRead(PINO_CHAMA);

  Serial.print(F("Leitura IR (A0): "));
  Serial.print(leitura);

  if (leitura < LIMIAR_CHAMA) {
    Serial.println(F(" -> ALERTA DE CHAMA DETECTADA!"));
    digitalWrite(PINO_BUZZER, HIGH);
    delay(150);
    digitalWrite(PINO_BUZZER, LOW);
    delay(100);
  } else {
    Serial.println(F(" -> Ambiente normal."));
    digitalWrite(PINO_BUZZER, LOW);
    delay(300);
  }
}
