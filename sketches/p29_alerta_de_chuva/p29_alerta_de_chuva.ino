/*
 * Projeto 29: Alerta de Chuva
 * O que faz: Detecta gotas de água na placa do sensor de chuva em A0 e dispara o buzzer em D8.
 * Ligações: A0 (Pino AO do sensor de chuva), D8 (Buzzer Ativo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_CHUVA = A0;  // Saída analógica AO do módulo de chuva
const byte PINO_BUZZER = 8;  // Buzzer ativo 5V

const int LIMIAR_GOTAS = 700; // AJUSTE AQUI: abaixo deste valor indica presença de gotas

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);
  Serial.println(F("Sistema pronto! Alerta de Chuva ativo."));
}

void loop() {
  int leitura = analogRead(PINO_CHUVA);

  Serial.print(F("Leitura A0: "));
  Serial.print(leitura);

  if (leitura < LIMIAR_GOTAS) {
    Serial.println(F(" -> ALERTA: Começou a chover!"));
    digitalWrite(PINO_BUZZER, HIGH);
    delay(200);
    digitalWrite(PINO_BUZZER, LOW);
    delay(200);
  } else {
    Serial.println(F(" -> Tempo SECO"));
    digitalWrite(PINO_BUZZER, LOW);
    delay(400);
  }
}
