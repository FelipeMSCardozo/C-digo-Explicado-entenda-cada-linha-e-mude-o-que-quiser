/*
 * Projeto 02: Sensor de Distância
 * O que faz: Mede a distância em cm com o HC-SR04, mostra no Monitor Serial e aciona 3 LEDs indicadores.
 * Ligações: D6 (Trig), D7 (Echo), D8 (LED Verde), D9 (LED Amarelo), D10 (LED Vermelho)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_TRIG = 6;          // Disparo do pulso ultrassônico
const byte PINO_ECHO = 7;          // Retorno do eco ultrassônico
const byte PINO_LED_VERDE = 8;     // LED Verde (longe)
const byte PINO_LED_AMARELO = 9;   // LED Amarelo (atenção)
const byte PINO_LED_VERMELHO = 10; // LED Vermelho (perto)

const int DISTANCIA_PERTO_CM = 15; // AJUSTE AQUI: limite para acender vermelho (cm)
const int DISTANCIA_MEDIA_CM = 30; // AJUSTE AQUI: limite para acender amarelo (cm)

long medirDistanciaCm() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  // Timeout de 30000 us (~5 metros) para nunca travar o programa
  unsigned long duracao = pulseIn(PINO_ECHO, HIGH, 30000UL);
  if (duracao == 0) {
    return -1; // Fora de alcance
  }
  return (long)(duracao * 0.0343 / 2.0);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_AMARELO, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);

  Serial.println(F("Sistema pronto! Sensor de Distância HC-SR04 iniciado."));
}

void loop() {
  long cm = medirDistanciaCm();

  if (cm < 2 || cm > 400) {
    Serial.println(F("Distância: Fora de alcance (2 a 400 cm)"));
    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_LED_AMARELO, LOW);
    digitalWrite(PINO_LED_VERMELHO, LOW);
  } else if (cm < DISTANCIA_PERTO_CM) {
    Serial.print(F("Distância: "));
    Serial.print(cm);
    Serial.println(F(" cm [PERTO - VERMELHO]"));
    digitalWrite(PINO_LED_VERDE, LOW);
    digitalWrite(PINO_LED_AMARELO, LOW);
    digitalWrite(PINO_LED_VERMELHO, HIGH);
  } else if (cm <= DISTANCIA_MEDIA_CM) {
    Serial.print(F("Distância: "));
    Serial.print(cm);
    Serial.println(F(" cm [ATENÇÃO - AMARELO]"));
    digitalWrite(PINO_LED_VERDE, LOW);
    digitalWrite(PINO_LED_AMARELO, HIGH);
    digitalWrite(PINO_LED_VERMELHO, LOW);
  } else {
    Serial.print(F("Distância: "));
    Serial.print(cm);
    Serial.println(F(" cm [SEGURO - VERDE]"));
    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_LED_AMARELO, LOW);
    digitalWrite(PINO_LED_VERMELHO, LOW);
  }

  delay(250);
}
