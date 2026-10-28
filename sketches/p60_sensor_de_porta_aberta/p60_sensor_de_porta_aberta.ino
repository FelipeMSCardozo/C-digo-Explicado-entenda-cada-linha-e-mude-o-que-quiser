/*
 * Projeto 60: Sensor de Porta Aberta
 * O que faz: Monitora um sensor magnético Reed Switch em D2 e dispara o buzzer em D8 quando o ímã se afasta.
 * Ligações: D2 (Reed Switch ao GND), D8 (Buzzer Ativo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_REED = 2;   // Reed Switch ligado entre D2 e GND (INPUT_PULLUP)
const byte PINO_BUZZER = 8; // Buzzer ativo 5V

bool portaAbertaAnterior = false;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_REED, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);
  digitalWrite(PINO_BUZZER, LOW);

  Serial.println(F("Sistema pronto! Sensor Magnético de Porta Aberta ativo."));
}

void loop() {
  // Com o ímã próximo, o Reed Switch fecha para o GND (LOW).
  // Quando a porta abre e o ímã se afasta, o Reed Switch abre e o pull-up leva D2 para HIGH.
  bool portaAberta = (digitalRead(PINO_REED) == HIGH);

  if (portaAberta) {
    if (!portaAbertaAnterior) {
      Serial.println(F("ALERTA: PORTA ABERTA! (ímã afastado)"));
      portaAbertaAnterior = true;
    }
    digitalWrite(PINO_BUZZER, HIGH);
    delay(150);
    digitalWrite(PINO_BUZZER, LOW);
    delay(150);
  } else {
    if (portaAbertaAnterior) {
      Serial.println(F("Porta: FECHADA (ímã próximo)"));
      portaAbertaAnterior = false;
    }
    digitalWrite(PINO_BUZZER, LOW);
    delay(100);
  }
}
