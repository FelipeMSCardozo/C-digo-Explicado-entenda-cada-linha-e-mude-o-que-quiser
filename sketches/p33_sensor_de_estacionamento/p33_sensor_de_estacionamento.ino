/*
 * Projeto 33: Sensor de Estacionamento
 * O que faz: Mede a distância no HC-SR04, acende 3 LEDs e acelera os bipes do buzzer conforme se aproxima.
 * Ligações: D6 (Trig), D7 (Echo), D8 (LED Verde), D9 (LED Amarelo), D10 (LED Vermelho), D11 (Buzzer Ativo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_TRIG = 6;          // Trig do HC-SR04
const byte PINO_ECHO = 7;          // Echo do HC-SR04
const byte PINO_LED_VERDE = 8;     // LED Verde
const byte PINO_LED_AMARELO = 9;   // LED Amarelo
const byte PINO_LED_VERMELHO = 10; // LED Vermelho
const byte PINO_BUZZER = 11;       // Buzzer ativo 5V

long medirDistanciaCm() {
  digitalWrite(PINO_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PINO_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PINO_TRIG, LOW);

  unsigned long dur = pulseIn(PINO_ECHO, HIGH, 30000UL);
  if (dur == 0) return 400;
  return (long)(dur * 0.0343 / 2.0);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_AMARELO, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  Serial.println(F("Sistema pronto! Sensor de Estacionamento ativo."));
}

void loop() {
  long cm = medirDistanciaCm();

  if (cm > 50) {
    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_LED_AMARELO, LOW);
    digitalWrite(PINO_LED_VERMELHO, LOW);
    digitalWrite(PINO_BUZZER, LOW);
    delay(150);
  } else if (cm > 30) {
    Serial.print(F("Distância: "));
    Serial.print(cm);
    Serial.println(F(" cm [Manobra livre - bipes lentos]"));
    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_LED_AMARELO, LOW);
    digitalWrite(PINO_LED_VERMELHO, LOW);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(60);
    digitalWrite(PINO_BUZZER, LOW);
    delay((unsigned long)(cm * 9));
  } else if (cm > 10) {
    Serial.print(F("Distância: "));
    Serial.print(cm);
    Serial.println(F(" cm [Atenção - bipes rápidos]"));
    digitalWrite(PINO_LED_VERDE, LOW);
    digitalWrite(PINO_LED_AMARELO, HIGH);
    digitalWrite(PINO_LED_VERMELHO, LOW);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(60);
    digitalWrite(PINO_BUZZER, LOW);
    delay((unsigned long)(cm * 7));
  } else {
    Serial.print(F("Distância: "));
    Serial.print(cm);
    Serial.println(F(" cm [PARE IMEDIATAMENTE!]"));
    digitalWrite(PINO_LED_VERDE, LOW);
    digitalWrite(PINO_LED_AMARELO, LOW);
    digitalWrite(PINO_LED_VERMELHO, HIGH);
    digitalWrite(PINO_BUZZER, HIGH); // Apito contínuo de parada
    delay(120);
  }
}
