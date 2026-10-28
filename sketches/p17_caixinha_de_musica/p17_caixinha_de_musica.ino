/*
 * Projeto 17: Caixinha de Música
 * O que faz: Executa a melodia "Ode à Alegria" (Beethoven - domínio público) com LED sincronizado.
 * Ligações: D8 (Buzzer Passivo), D9 (LED Amarelo via resistor 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BUZZER = 8; // Buzzer passivo
const byte PINO_LED = 9;    // LED rítmico

const unsigned int TEMPO_BASE_MS = 1400; // AJUSTE AQUI: menor valor = música mais rápida

// Frequências das notas da 4ª oitava (Hz)
#define NOTA_DO4  262
#define NOTA_RE4  294
#define NOTA_MI4  330
#define NOTA_FA4  349
#define NOTA_SOL4 392

const unsigned int melodia[16] = {
  NOTA_MI4, NOTA_MI4, NOTA_FA4, NOTA_SOL4,
  NOTA_SOL4, NOTA_FA4, NOTA_MI4, NOTA_RE4,
  NOTA_DO4, NOTA_DO4, NOTA_RE4, NOTA_MI4,
  NOTA_MI4, NOTA_RE4, NOTA_RE4, 0
};

// 4 = semínima (1/4 de compasso), 2 = mínima (1/2 compasso)
const byte divisoes[16] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 2, 4
};

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(PINO_LED, OUTPUT);
  Serial.println(F("Sistema pronto! Caixinha de Música (Ode à Alegria - Beethoven)."));
}

void loop() {
  Serial.println(F("Tocando melodia (16 notas)..."));

  for (byte i = 0; i < 16; i++) {
    unsigned int duracaoNota = TEMPO_BASE_MS / divisoes[i];

    if (melodia[i] > 0) {
      digitalWrite(PINO_LED, HIGH);
      tone(PINO_BUZZER, melodia[i], duracaoNota);
    }

    delay(duracaoNota);
    digitalWrite(PINO_LED, LOW);
    noTone(PINO_BUZZER);
    delay(duracaoNota / 3); // Breve separação entre as notas
  }

  Serial.println(F("Melodia concluída! Aguardando 3s..."));
  delay(3000);
}
