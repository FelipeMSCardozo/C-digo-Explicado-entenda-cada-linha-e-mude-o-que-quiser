/*
 * Projeto 11: Botão Liga/Desliga com Memória
 * O que faz: Alterna o estado do LED (aceso/apagado) a cada clique no botão, com filtro de debounce.
 * Ligações: D2 (Botão ao GND), D8 (LED Verde via resistor 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BOTAO = 2; // Botão ligado ao GND (INPUT_PULLUP)
const byte PINO_LED = 8;   // LED verde com resistor de 220 Ω

const unsigned int TEMPO_DEBOUNCE_MS = 50; // AJUSTE AQUI: filtro contra repique do botão

bool estadoLed = false;
bool leituraAnterior = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);

  Serial.println(F("Sistema pronto! Botão Liga/Desliga com Memória ativo."));
}

void loop() {
  bool leituraAtual = digitalRead(PINO_BOTAO);

  // Detecta apenas o instante em que o botão acabou de ser pressionado (HIGH -> LOW)
  if (leituraAnterior == HIGH && leituraAtual == LOW) {
    estadoLed = !estadoLed;
    digitalWrite(PINO_LED, estadoLed ? HIGH : LOW);

    Serial.print(F("Clique detectado -> LED: "));
    Serial.println(estadoLed ? F("LIGADO") : F("DESLIGADO"));
    delay(TEMPO_DEBOUNCE_MS);
  }

  leituraAnterior = leituraAtual;
  delay(10);
}
