/*
 * Projeto 30: Detector de Inclinação
 * O que faz: Monitora a chave de inclinação (tilt switch SW-520D) no pino D2 e acende o LED ao inclinar.
 * Ligações: D2 (Chave Tilt ao GND), D8 (LED Vermelho via resistor 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_TILT = 2; // Chave tilt ligada entre D2 e GND (INPUT_PULLUP)
const byte PINO_LED = 8;  // LED vermelho indicador

const byte AMOSTRAS_CONFIRMACAO = 5; // AJUSTE AQUI: leituras seguidas para filtrar vibração

bool estadoInclinado = false;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_TILT, INPUT_PULLUP);
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);

  Serial.println(F("Sistema pronto! Detector de Inclinação (Chave Tilt) ativo."));
}

void loop() {
  // Conta quantas de N leituras rápidas indicam circuito aberto (HIGH = inclinado)
  byte contAberto = 0;
  for (byte i = 0; i < AMOSTRAS_CONFIRMACAO; i++) {
    if (digitalRead(PINO_TILT) == HIGH) {
      contAberto++;
    }
    delay(15);
  }

  bool inclinadoAgora = (contAberto == AMOSTRAS_CONFIRMACAO);

  if (inclinadoAgora != estadoInclinado) {
    estadoInclinado = inclinadoAgora;
    digitalWrite(PINO_LED, estadoInclinado ? HIGH : LOW);

    if (estadoInclinado) {
      Serial.println(F("ALERTA: Objeto INCLINADO / TOMBADO!"));
    } else {
      Serial.println(F("Posição: NIVELADO"));
    }
  }
}
