/*
 * Projeto 24: Barra de Nível com LEDs
 * O que faz: Acende de 0 a 8 LEDs em formato de barra gráfica conforme a posição do potenciômetro em A0.
 * Ligações: A0 (Potenciômetro), D2..D9 (8 LEDs via resistores de 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_POT = A0;  // Pino central do potenciômetro
const byte PINO_LED_1 = 2; // LED 1 (Verde)
const byte PINO_LED_2 = 3; // LED 2 (Verde)
const byte PINO_LED_3 = 4; // LED 3 (Verde)
const byte PINO_LED_4 = 5; // LED 4 (Amarelo)
const byte PINO_LED_5 = 6; // LED 5 (Amarelo)
const byte PINO_LED_6 = 7; // LED 6 (Amarelo)
const byte PINO_LED_7 = 8; // LED 7 (Vermelho)
const byte PINO_LED_8 = 9; // LED 8 (Vermelho)

const byte PINOS_BARRA[8] = {
  PINO_LED_1, PINO_LED_2, PINO_LED_3, PINO_LED_4,
  PINO_LED_5, PINO_LED_6, PINO_LED_7, PINO_LED_8
};

int ultimoNivel = -1;

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 8; i++) {
    pinMode(PINOS_BARRA[i], OUTPUT);
  }
  Serial.println(F("Sistema pronto! Barra de Nível com 8 LEDs iniciada."));
}

void loop() {
  int leitura = analogRead(PINO_POT);
  int nivel = map(leitura, 0, 1000, 0, 8);
  nivel = constrain(nivel, 0, 8);

  for (byte i = 0; i < 8; i++) {
    digitalWrite(PINOS_BARRA[i], (i < nivel) ? HIGH : LOW);
  }

  if (nivel != ultimoNivel) {
    ultimoNivel = nivel;
    Serial.print(F("Leitura A0: "));
    Serial.print(leitura);
    Serial.print(F(" -> Nível: "));
    Serial.print(nivel);
    Serial.println(F("/8 LEDs"));
  }

  delay(25);
}
