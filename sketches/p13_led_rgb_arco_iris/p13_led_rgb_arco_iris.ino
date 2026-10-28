/*
 * Projeto 13: LED RGB Arco-Íris
 * O que faz: Faz um LED RGB percorrer suavemente todas as cores do arco-íris usando 3 pinos PWM.
 * Ligações: D9 (Vermelho R), D10 (Verde G), D11 (Azul B), todos via resistor de 220 Ω; Comum no GND
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_R = 9;  // Canal Vermelho (PWM)
const byte PINO_G = 10; // Canal Verde (PWM)
const byte PINO_B = 11; // Canal Azul (PWM)

const bool LED_ANODO_COMUM = false; // AJUSTE AQUI: mude para true se a perna longa for no 5V
const unsigned int PASSO_ATRASO_MS = 10; // AJUSTE AQUI: velocidade da transição

void definirCor(byte r, byte g, byte b) {
  if (LED_ANODO_COMUM) {
    r = 255 - r;
    g = 255 - g;
    b = 255 - b;
  }
  analogWrite(PINO_R, r);
  analogWrite(PINO_G, g);
  analogWrite(PINO_B, b);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_R, OUTPUT);
  pinMode(PINO_G, OUTPUT);
  pinMode(PINO_B, OUTPUT);
  Serial.println(F("Sistema pronto! LED RGB Arco-Íris iniciado."));
}

void loop() {
  Serial.println(F("Fase: Vermelho -> Verde"));
  for (int i = 0; i <= 255; i += 5) {
    definirCor(255 - i, i, 0);
    delay(PASSO_ATRASO_MS);
  }

  Serial.println(F("Fase: Verde -> Azul"));
  for (int i = 0; i <= 255; i += 5) {
    definirCor(0, 255 - i, i);
    delay(PASSO_ATRASO_MS);
  }

  Serial.println(F("Fase: Azul -> Vermelho"));
  for (int i = 0; i <= 255; i += 5) {
    definirCor(i, 0, 255 - i);
    delay(PASSO_ATRASO_MS);
  }
}
