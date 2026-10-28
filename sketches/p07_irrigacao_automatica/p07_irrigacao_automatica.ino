/*
 * Projeto 07: Irrigação Automática
 * O que faz: Energiza o sensor de solo no instante da leitura e aciona a mini bomba 5V via relé se a terra estiver seca.
 * Ligações: D7 (VCC do sensor de solo), A0 (AO do sensor de solo), D8 (IN do Relé), D9 (LED Verde)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_VCC_SENSOR = 7; // Alimenta o sensor só na hora de ler (evita corrosão)
const byte PINO_SENSOR_AO = A0; // Leitura analógica da umidade do solo
const byte PINO_RELE = 8;       // Sinal IN do módulo relé
const byte PINO_LED_VERDE = 9;  // LED indicador de solo úmido

const int LIMIAR_SOLO_SECO = 700;  // AJUSTE AQUI: acima deste valor (0-1023) o solo é considerado seco
const bool RELE_ATIVO_LOW = true;  // AJUSTE AQUI: true para relés que ligam com LOW, false para HIGH

void ajustarRele(bool ligarBomba) {
  if (RELE_ATIVO_LOW) {
    digitalWrite(PINO_RELE, ligarBomba ? LOW : HIGH);
  } else {
    digitalWrite(PINO_RELE, ligarBomba ? HIGH : LOW);
  }
}

int lerUmidadeSolo() {
  digitalWrite(PINO_VCC_SENSOR, HIGH);
  delay(20); // Aguarda estabilizar a tensão na sonda
  int valor = analogRead(PINO_SENSOR_AO);
  digitalWrite(PINO_VCC_SENSOR, LOW); // Desliga a sonda imediatamente
  return valor;
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_VCC_SENSOR, OUTPUT);
  digitalWrite(PINO_VCC_SENSOR, LOW);

  pinMode(PINO_RELE, OUTPUT);
  ajustarRele(false); // Garante que a bomba comece desligada

  pinMode(PINO_LED_VERDE, OUTPUT);
  Serial.println(F("Sistema pronto! Irrigação Automática iniciada."));
}

void loop() {
  int leitura = lerUmidadeSolo();

  Serial.print(F("Leitura A0: "));
  Serial.print(leitura);

  if (leitura > LIMIAR_SOLO_SECO) {
    Serial.println(F(" -> SOLO SECO! Acionando bomba por 2s..."));
    digitalWrite(PINO_LED_VERDE, LOW);
    ajustarRele(true);
    delay(2000); // Rega por 2 segundos
    ajustarRele(false);
    Serial.println(F("Aguardando a água infiltrar na terra..."));
    delay(4000);
  } else {
    Serial.println(F(" -> SOLO ÚMIDO. Bomba desligada."));
    ajustarRele(false);
    digitalWrite(PINO_LED_VERDE, HIGH);
    delay(3000);
  }
}
