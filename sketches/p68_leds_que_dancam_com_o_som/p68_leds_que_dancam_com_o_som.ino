// ============================================================================
// Projeto 68 — LEDs que Dançam com o Som
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Nenhuma (Nativo)
// Material: 80 Projetos Arduino
// VU Meter rítmico de 8 LEDs (3 verdes, 3 amarelos, 2 vermelhos) + Sensor Som
// ============================================================================

const byte PINO_SOM   = A0; // Saída analógica (AO) do sensor de som
const byte PINO_LED_1 = 2;  // Verde 1
const byte PINO_LED_2 = 3;  // Verde 2
const byte PINO_LED_3 = 4;  // Verde 3
const byte PINO_LED_4 = 5;  // Amarelo 1
const byte PINO_LED_5 = 6;  // Amarelo 2
const byte PINO_LED_6 = 7;  // Amarelo 3
const byte PINO_LED_7 = 8;  // Vermelho 1
const byte PINO_LED_8 = 9;  // Vermelho 2 (Pico)

const byte PINOS_BARRA[8] = {
  PINO_LED_1, PINO_LED_2, PINO_LED_3, PINO_LED_4,
  PINO_LED_5, PINO_LED_6, PINO_LED_7, PINO_LED_8
};

// AJUSTE AQUI: Limiar de silêncio e amplitude máxima para acender os 8 LEDs
const int RUIDO_FUNDO   = 14;
const int AMPLITUDE_MAX = 160;

int nivelExibido = 0;

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 8; i++) {
    pinMode(PINOS_BARRA[i], OUTPUT);
    digitalWrite(PINOS_BARRA[i], LOW);
  }
  Serial.println(F("Sistema pronto! VU Meter de 8 LEDs ativo."));
}

void loop() {
  unsigned long inicioJanela = millis();
  int sinalMax = 0;
  int sinalMin = 1023;

  // Amostra o áudio durante 25 ms para encontrar a amplitude pico a pico
  while (millis() - inicioJanela < 25) {
    int amostra = analogRead(PINO_SOM);
    if (amostra > sinalMax) sinalMax = amostra;
    if (amostra < sinalMin) sinalMin = amostra;
  }

  int amplitude = sinalMax - sinalMin;
  int nivelAlvo = 0;
  if (amplitude > RUIDO_FUNDO) {
    nivelAlvo = map(amplitude, RUIDO_FUNDO, AMPLITUDE_MAX, 1, 8);
    nivelAlvo = constrain(nivelAlvo, 0, 8);
  }

  // Subida rápida no ataque da batida, descida suave (decay)
  if (nivelAlvo >= nivelExibido) {
    nivelExibido = nivelAlvo;
  } else if (nivelExibido > 0) {
    nivelExibido--;
  }

  for (byte i = 0; i < 8; i++) {
    digitalWrite(PINOS_BARRA[i], (i < nivelExibido) ? HIGH : LOW);
  }

  Serial.print(F("Amplitude Som: "));
  Serial.print(amplitude);
  Serial.print(F(" | LEDs acesos: "));
  Serial.println(nivelExibido);
}
