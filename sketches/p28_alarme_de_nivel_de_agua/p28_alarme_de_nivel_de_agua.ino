/*
 * Projeto 28: Alarme de Nível de Água
 * O que faz: Mede a altura da água com o sensor analógico em A0 e dispara o buzzer quando cheio.
 * Ligações: D7 (VCC do sensor), A0 (Sinal S do sensor), D8 (Buzzer Ativo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_VCC_SENSOR = 7; // Alimenta o sensor apenas durante a leitura
const byte PINO_NIVEL = A0;     // Sinal analógico (S) do sensor de nível
const byte PINO_BUZZER = 8;     // Buzzer ativo 5V

const int LIMIAR_MEDIO = 180;   // AJUSTE AQUI: início do nível médio
const int LIMIAR_CHEIO = 450;   // AJUSTE AQUI: nível máximo (dispara o alarme)

int lerNivelAgua() {
  digitalWrite(PINO_VCC_SENSOR, HIGH);
  delay(15);
  int valor = analogRead(PINO_NIVEL);
  digitalWrite(PINO_VCC_SENSOR, LOW);
  return valor;
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_VCC_SENSOR, OUTPUT);
  digitalWrite(PINO_VCC_SENSOR, LOW);
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  Serial.println(F("Sistema pronto! Alarme de Nível de Água ativo."));
}

void loop() {
  int leitura = lerNivelAgua();

  Serial.print(F("Leitura A0: "));
  Serial.print(leitura);

  if (leitura >= LIMIAR_CHEIO) {
    Serial.println(F(" -> ALERTA: RESERVATÓRIO CHEIO!"));
    digitalWrite(PINO_BUZZER, HIGH);
    delay(180);
    digitalWrite(PINO_BUZZER, LOW);
    delay(180);
  } else if (leitura >= LIMIAR_MEDIO) {
    Serial.println(F(" -> Nível MÉDIO"));
    digitalWrite(PINO_BUZZER, LOW);
    delay(500);
  } else {
    Serial.println(F(" -> Reservatório VAZIO"));
    digitalWrite(PINO_BUZZER, LOW);
    delay(500);
  }
}
