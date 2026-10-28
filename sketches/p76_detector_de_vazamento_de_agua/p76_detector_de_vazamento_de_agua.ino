// ============================================================================
// Projeto 76 — Detector de Vazamento de Água
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Nenhuma (Nativo)
// Material: 80 Projetos Arduino
// Alarme residencial de 2 estágios com sensor de água (A0) e Buzzer Ativo (D8)
// ============================================================================

const byte PINO_SENSOR_AGUA = A0; // Pino S do sensor de nível/vazamento
const byte PINO_BUZZER      = 8;  // Buzzer Ativo 5V

// AJUSTE AQUI: Limiares de detecção (0 a 1023)
const int LIMIAR_GOTAS      = 150;
const int LIMIAR_ALAGAMENTO = 400;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  Serial.println(F("Sistema pronto! Detector de Vazamento de Agua."));
}

void loop() {
  int leitura = analogRead(PINO_SENSOR_AGUA);

  Serial.print(F("Leitura A0: "));
  Serial.print(leitura);

  if (leitura >= LIMIAR_ALAGAMENTO) {
    Serial.println(F(" -> ALARME CRITICO: VAZAMENTO DETECTADO!"));
    digitalWrite(PINO_BUZZER, HIGH);
    delay(120);
    digitalWrite(PINO_BUZZER, LOW);
    delay(80);
  } else if (leitura >= LIMIAR_GOTAS) {
    Serial.println(F(" -> ATENCAO: Umidade / Primeiras gotas no piso."));
    digitalWrite(PINO_BUZZER, HIGH);
    delay(80);
    digitalWrite(PINO_BUZZER, LOW);
    delay(420);
  } else {
    Serial.println(F(" -> Status: PISO SECO"));
    digitalWrite(PINO_BUZZER, LOW);
    delay(500);
  }
}
