/*
 * Projeto 21: Dado Eletrônico
 * O que faz: Sorteia números de 1 a 6 ao pressionar o botão e exibe nos 7 LEDs dispostos como um dado.
 * Ligações: D2 (Botão), D3..D5 (Coluna Esq), D6 (Centro), D7..D9 (Coluna Dir) via resistores 220 Ω
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BOTAO = 2; // Botão de lançar o dado
const byte PINO_LED_1 = 3; // Superior esquerdo
const byte PINO_LED_2 = 4; // Meio esquerdo
const byte PINO_LED_3 = 5; // Inferior esquerdo
const byte PINO_LED_4 = 6; // Centro
const byte PINO_LED_5 = 7; // Superior direito
const byte PINO_LED_6 = 8; // Meio direito
const byte PINO_LED_7 = 9; // Inferior direito

const byte PINOS_DADO[7] = {
  PINO_LED_1, PINO_LED_2, PINO_LED_3, PINO_LED_4, PINO_LED_5, PINO_LED_6, PINO_LED_7
};

// Configuração dos 7 LEDs para cada face de 1 a 6
const byte FACES[6][7] = {
  {0, 0, 0, 1, 0, 0, 0}, // Face 1 (apenas o centro)
  {1, 0, 0, 0, 0, 0, 1}, // Face 2 (diagonal)
  {1, 0, 0, 1, 0, 0, 1}, // Face 3 (diagonal + centro)
  {1, 0, 1, 0, 1, 0, 1}, // Face 4 (4 cantos)
  {1, 0, 1, 1, 1, 0, 1}, // Face 5 (4 cantos + centro)
  {1, 1, 1, 0, 1, 1, 1}  // Face 6 (2 colunas de 3)
};

void exibirFace(byte numero) {
  byte idx = numero - 1;
  for (byte i = 0; i < 7; i++) {
    digitalWrite(PINOS_DADO[i], FACES[idx][i] ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  for (byte i = 0; i < 7; i++) {
    pinMode(PINOS_DADO[i], OUTPUT);
  }
  randomSeed(analogRead(A0));
  exibirFace(1);
  Serial.println(F("Sistema pronto! Dado Eletrônico aguardando lançamento."));
}

void loop() {
  if (digitalRead(PINO_BOTAO) == LOW) {
    randomSeed(micros());
    Serial.println(F("Rolando o dado..."));

    for (byte passo = 0; passo < 14; passo++) {
      exibirFace((byte)random(1, 7));
      delay(40 + passo * 15);
    }

    byte resultado = (byte)random(1, 7);
    exibirFace(resultado);
    Serial.print(F("Face sorteada: "));
    Serial.println(resultado);
    delay(400);
  }
}
