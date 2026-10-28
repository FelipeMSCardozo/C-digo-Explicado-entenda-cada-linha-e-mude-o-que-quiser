/*
 * Projeto 09: Pisca LED Externo
 * O que faz: Pisca um LED externo na protoboard conectado ao pino D8 em intervalos regulares.
 * Ligações: D8 (Ânodo do LED via resistor 220 Ω), GND (Cátodo do LED)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_LED = 8; // Pino digital conectado ao LED via resistor de 220 Ω

const unsigned int INTERVALO_MS = 500; // AJUSTE AQUI: tempo aceso/apagado em milissegundos

void setup() {
  Serial.begin(9600);
  pinMode(PINO_LED, OUTPUT);
  Serial.println(F("Sistema pronto! Pisca LED Externo iniciado."));
}

void loop() {
  digitalWrite(PINO_LED, HIGH);
  Serial.println(F("LED: ACESO (HIGH)"));
  delay(INTERVALO_MS);

  digitalWrite(PINO_LED, LOW);
  Serial.println(F("LED: APAGADO (LOW)"));
  delay(INTERVALO_MS);
}
