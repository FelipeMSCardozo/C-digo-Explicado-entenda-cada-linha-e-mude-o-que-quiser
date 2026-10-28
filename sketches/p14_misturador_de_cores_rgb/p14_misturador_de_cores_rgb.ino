/*
 * Projeto 14: Misturador de Cores RGB
 * O que faz: Usa 3 potenciômetros (A0, A1, A2) para controlar manualmente os canais R, G e B de um LED RGB.
 * Ligações: A0 (Pot R), A1 (Pot G), A2 (Pot B), D9 (LED R), D10 (LED G), D11 (LED B) via resistores 220 Ω
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_POT_R = A0; // Potenciômetro do canal Vermelho
const byte PINO_POT_G = A1; // Potenciômetro do canal Verde
const byte PINO_POT_B = A2; // Potenciômetro do canal Azul
const byte PINO_LED_R = 9;  // Pino PWM do canal Vermelho
const byte PINO_LED_G = 10; // Pino PWM do canal Verde
const byte PINO_LED_B = 11; // Pino PWM do canal Azul

const bool LED_ANODO_COMUM = false; // AJUSTE AQUI: true apenas se o comum do LED for ligado em 5V

unsigned long ultimoPrint = 0;

byte lerCanal(byte pinoPot) {
  int val = map(analogRead(pinoPot), 0, 1023, 0, 255);
  if (val < 8) val = 0; // Zona morta para apagar 100% no fim do curso
  return (byte)val;
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_LED_R, OUTPUT);
  pinMode(PINO_LED_G, OUTPUT);
  pinMode(PINO_LED_B, OUTPUT);
  Serial.println(F("Sistema pronto! Misturador de Cores RGB ativo."));
}

void loop() {
  byte r = lerCanal(PINO_POT_R);
  byte g = lerCanal(PINO_POT_G);
  byte b = lerCanal(PINO_POT_B);

  analogWrite(PINO_LED_R, LED_ANODO_COMUM ? (255 - r) : r);
  analogWrite(PINO_LED_G, LED_ANODO_COMUM ? (255 - g) : g);
  analogWrite(PINO_LED_B, LED_ANODO_COMUM ? (255 - b) : b);

  if (millis() - ultimoPrint >= 400) {
    ultimoPrint = millis();
    Serial.print(F("Cor atual -> R: "));
    Serial.print(r);
    Serial.print(F(" | G: "));
    Serial.print(g);
    Serial.print(F(" | B: "));
    Serial.println(b);
  }

  delay(20);
}
