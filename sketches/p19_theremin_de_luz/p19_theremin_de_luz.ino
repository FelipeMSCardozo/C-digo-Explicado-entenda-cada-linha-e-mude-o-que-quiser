/*
 * Projeto 19: Theremin de Luz
 * O que faz: Calibra a luz ambiente nos primeiros 3s e converte a sombra da mão no LDR em notas musicais.
 * Ligações: A0 (Divisor LDR + 10 kΩ), D8 (Buzzer Passivo), D9 (LED Verde via resistor 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_LDR = A0;    // Divisor de tensão do LDR
const byte PINO_BUZZER = 8;  // Buzzer passivo
const byte PINO_LED = 9;     // LED indicador

const int FREQ_MIN_HZ = 220;  // AJUSTE AQUI: nota mais grave (Hz)
const int FREQ_MAX_HZ = 1200; // AJUSTE AQUI: nota mais aguda (Hz)

int luzMin = 1023;
int luzMax = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(PINO_LED, OUTPUT);

  Serial.println(F("Calibrando luz ambiente por 3s (mova a mão sobre o LDR)..."));
  digitalWrite(PINO_LED, HIGH);

  unsigned long inicio = millis();
  while (millis() - inicio < 3000) {
    int v = analogRead(PINO_LDR);
    if (v > luzMax) luzMax = v;
    if (v < luzMin) luzMin = v;
    delay(10);
  }
  digitalWrite(PINO_LED, LOW);

  // Garante faixa mínima caso o aluno não tenha movido a mão na calibração
  if (luzMax - luzMin < 80) {
    luzMin = 150;
    luzMax = 800;
  }

  Serial.print(F("Sistema pronto! Theremin de Luz calibrado (Min: "));
  Serial.print(luzMin);
  Serial.print(F(" | Max: "));
  Serial.print(luzMax);
  Serial.println(F(")."));
}

void loop() {
  int leitura = analogRead(PINO_LDR);

  // Se não houver sombra da mão (perto do máximo de luz), mantém em silêncio
  if (leitura > (luzMax - 35)) {
    noTone(PINO_BUZZER);
    digitalWrite(PINO_LED, LOW);
  } else {
    int freq = map(leitura, luzMin, luzMax - 35, FREQ_MIN_HZ, FREQ_MAX_HZ);
    freq = constrain(freq, FREQ_MIN_HZ, FREQ_MAX_HZ);
    tone(PINO_BUZZER, (unsigned int)freq);
    digitalWrite(PINO_LED, HIGH);

    Serial.print(F("Luz A0: "));
    Serial.print(leitura);
    Serial.print(F(" -> Frequência: "));
    Serial.print(freq);
    Serial.println(F(" Hz"));
  }

  delay(35);
}
