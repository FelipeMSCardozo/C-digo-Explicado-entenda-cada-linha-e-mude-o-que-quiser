/*
 * Projeto 15: Luz Noturna Automática
 * O que faz: Mede a luz ambiente com um LDR em A0 e acende o LED em D8 automaticamente no escuro.
 * Ligações: A0 (Divisor LDR + 10 kΩ), D8 (LED via resistor 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_LDR = A0; // Ponto entre o LDR (5V) e o resistor de 10 kΩ (GND)
const byte PINO_LED = 8;  // LED da luz noturna com resistor de 220 Ω

const int LIMIAR_ESCURO = 400; // AJUSTE AQUI: abaixo deste valor o LED acende
const int HISTERESE = 50;      // Margem para não piscar na penumbra

bool luzAcesa = false;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);
  Serial.println(F("Sistema pronto! Luz Noturna Automática iniciada."));
}

void loop() {
  int leitura = analogRead(PINO_LDR);

  if (!luzAcesa && leitura < LIMIAR_ESCURO) {
    luzAcesa = true;
    digitalWrite(PINO_LED, HIGH);
  } else if (luzAcesa && leitura > (LIMIAR_ESCURO + HISTERESE)) {
    luzAcesa = false;
    digitalWrite(PINO_LED, LOW);
  }

  Serial.print(F("Claridade A0: "));
  Serial.print(leitura);
  if (luzAcesa) {
    Serial.println(F(" -> Ambiente ESCURO (LED aceso!)"));
  } else {
    Serial.println(F(" -> Ambiente CLARO (LED apagado)"));
  }

  delay(300);
}
