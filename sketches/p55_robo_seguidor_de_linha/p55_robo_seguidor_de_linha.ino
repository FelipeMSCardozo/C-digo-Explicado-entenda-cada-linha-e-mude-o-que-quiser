/*
 * Projeto 55: Robô Seguidor de Linha
 * O que faz: Guia um chassi 2WD automaticamente ao longo de uma faixa preta usando 2 sensores TCRT5000.
 * Ligações: D2 (TCRT5000 Esq), D3 (TCRT5000 Dir), D4/D7 (Motor Esq), D8/D9 (Motor Dir), D5/D6 (ENA/ENB PWM)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_SENSOR_ESQ = 2; // Saída DO do sensor TCRT5000 esquerdo
const byte PINO_SENSOR_DIR = 3; // Saída DO do sensor TCRT5000 direito
const byte PINO_IN1 = 4;        // Motor Esquerdo IN1
const byte PINO_ENA = 5;        // Motor Esquerdo PWM
const byte PINO_ENB = 6;        // Motor Direito PWM
const byte PINO_IN2 = 7;        // Motor Esquerdo IN2
const byte PINO_IN3 = 8;        // Motor Direito IN3
const byte PINO_IN4 = 9;        // Motor Direito IN4

const int LINHA_PRETA_SINAL = HIGH; // AJUSTE AQUI: nível lido quando o sensor está sobre a fita preta
const byte PWM_RETA = 145;          // AJUSTE AQUI: velocidade nas retas (110 a 180)
const byte PWM_CURVA = 165;         // Velocidade da roda externa durante a correção

unsigned long ultimoPrint = 0;

void comandarMotores(int velEsq, int velDir) {
  digitalWrite(PINO_IN1, velEsq > 0 ? HIGH : LOW);
  digitalWrite(PINO_IN2, LOW);
  analogWrite(PINO_ENA, (byte)constrain(velEsq, 0, 255));

  digitalWrite(PINO_IN3, velDir > 0 ? HIGH : LOW);
  digitalWrite(PINO_IN4, LOW);
  analogWrite(PINO_ENB, (byte)constrain(velDir, 0, 255));
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_SENSOR_ESQ, INPUT);
  pinMode(PINO_SENSOR_DIR, INPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);
  pinMode(PINO_IN3, OUTPUT);
  pinMode(PINO_IN4, OUTPUT);
  pinMode(PINO_ENA, OUTPUT);
  pinMode(PINO_ENB, OUTPUT);

  Serial.println(F("Sistema pronto! Robô Seguidor de Linha ativo."));
}

void loop() {
  bool esqNaLinha = (digitalRead(PINO_SENSOR_ESQ) == LINHA_PRETA_SINAL);
  bool dirNaLinha = (digitalRead(PINO_SENSOR_DIR) == LINHA_PRETA_SINAL);

  if (!esqNaLinha && !dirNaLinha) {
    // Ambos no branco -> segue reto
    comandarMotores(PWM_RETA, PWM_RETA);
  } else if (esqNaLinha && !dirNaLinha) {
    // Tocou a linha na esquerda -> vira para a esquerda
    comandarMotores(0, PWM_CURVA);
  } else if (!esqNaLinha && dirNaLinha) {
    // Tocou a linha na direita -> vira para a direita
    comandarMotores(PWM_CURVA, 0);
  } else {
    // Ambos na fita preta -> fim de pista / parada
    comandarMotores(0, 0);
  }

  if (millis() - ultimoPrint >= 400) {
    ultimoPrint = millis();
    Serial.print(F("Esq: "));
    Serial.print(esqNaLinha ? F("LINHA") : F("BRANCO"));
    Serial.print(F(" | Dir: "));
    Serial.println(dirNaLinha ? F("LINHA") : F("BRANCO"));
  }
}
