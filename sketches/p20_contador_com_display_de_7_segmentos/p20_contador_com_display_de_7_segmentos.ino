/*
 * Projeto 20: Contador com Display de 7 Segmentos
 * O que faz: Exibe os números de 0 a 9 em um display de 7 segmentos e avança a cada clique no botão.
 * Ligações: D2 (Botão), D3..D9 (Segmentos a, b, c, d, e, f, g via resistores 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BOTAO = 2; // Botão de avanço ligado ao GND
const byte PINO_SEG_A = 3; // Segmento A (topo)
const byte PINO_SEG_B = 4; // Segmento B (superior direito)
const byte PINO_SEG_C = 5; // Segmento C (inferior direito)
const byte PINO_SEG_D = 6; // Segmento D (base)
const byte PINO_SEG_E = 7; // Segmento E (inferior esquerdo)
const byte PINO_SEG_F = 8; // Segmento F (superior esquerdo)
const byte PINO_SEG_G = 9; // Segmento G (centro)

const bool DISPLAY_ANODO_COMUM = false; // AJUSTE AQUI: true se o pino central for ligado em 5V

const byte PINOS_SEGMENTOS[7] = {
  PINO_SEG_A, PINO_SEG_B, PINO_SEG_C, PINO_SEG_D, PINO_SEG_E, PINO_SEG_F, PINO_SEG_G
};

// Tabela verdade dos segmentos: {a, b, c, d, e, f, g}
const byte DIGITOS[10][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

byte numeroAtual = 0;
bool botaoAnterior = HIGH;

void mostrarDigito(byte num) {
  for (byte i = 0; i < 7; i++) {
    bool aceso = (DIGITOS[num][i] == 1);
    if (DISPLAY_ANODO_COMUM) aceso = !aceso;
    digitalWrite(PINOS_SEGMENTOS[i], aceso ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  for (byte i = 0; i < 7; i++) {
    pinMode(PINOS_SEGMENTOS[i], OUTPUT);
  }
  mostrarDigito(numeroAtual);
  Serial.println(F("Sistema pronto! Contador 7 Segmentos iniciado em 0."));
}

void loop() {
  bool leitura = digitalRead(PINO_BOTAO);

  if (botaoAnterior == HIGH && leitura == LOW) {
    numeroAtual = (numeroAtual + 1) % 10;
    mostrarDigito(numeroAtual);
    Serial.print(F("Botão pressionado -> Mostrando dígito: "));
    Serial.println(numeroAtual);
    delay(60); // Debounce
  }

  botaoAnterior = leitura;
  delay(10);
}
