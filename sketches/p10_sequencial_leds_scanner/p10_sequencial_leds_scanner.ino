/*
 * Projeto 10: Sequencial de LEDs (Efeito Scanner)
 * O que faz: Acende 6 LEDs em sequência de ida e volta continuamente usando array e laço for.
 * Ligações: D4, D5, D6, D7, D8, D9 nos 6 LEDs (cada um com resistor de 220 Ω ao GND)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_LED_1 = 4; // LED 1 da fileira
const byte PINO_LED_2 = 5; // LED 2 da fileira
const byte PINO_LED_3 = 6; // LED 3 da fileira
const byte PINO_LED_4 = 7; // LED 4 da fileira
const byte PINO_LED_5 = 8; // LED 5 da fileira
const byte PINO_LED_6 = 9; // LED 6 da fileira

const byte PINOS_LEDS[6] = {
  PINO_LED_1, PINO_LED_2, PINO_LED_3, PINO_LED_4, PINO_LED_5, PINO_LED_6
};

const unsigned int VELOCIDADE_MS = 90; // AJUSTE AQUI: menor valor = scanner mais rápido

void piscarPasso(byte indice) {
  digitalWrite(PINOS_LEDS[indice], HIGH);
  delay(VELOCIDADE_MS);
  digitalWrite(PINOS_LEDS[indice], LOW);
}

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 6; i++) {
    pinMode(PINOS_LEDS[i], OUTPUT);
    digitalWrite(PINOS_LEDS[i], LOW);
  }
  Serial.println(F("Sistema pronto! Sequencial Scanner de 6 LEDs iniciado."));
}

void loop() {
  // Ida: do LED 0 até o LED 5
  for (byte i = 0; i < 6; i++) {
    piscarPasso(i);
  }
  // Volta: do LED 4 até o LED 1 (sem repetir as pontas)
  for (int i = 4; i >= 1; i--) {
    piscarPasso((byte)i);
  }
  Serial.println(F("Ciclo completo de varredura concluído (ida e volta)."));
}
