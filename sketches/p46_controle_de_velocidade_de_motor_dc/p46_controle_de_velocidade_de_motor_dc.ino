/*
 * Projeto 46: Controle de Velocidade de Motor DC
 * O que faz: Controla velocidade (PWM) e sentido de giro (horário/anti-horário) de um motor DC via L298N.
 * Ligações: A0 (Potenciômetro), D5 (L298N ENA - PWM), D6 (L298N IN1), D7 (L298N IN2); GND comum
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_POT = A0; // Pino central do potenciômetro
const byte PINO_ENA = 5;  // Pino PWM (~) ligado ao ENA do L298N (sem o jumper!)
const byte PINO_IN1 = 6;  // Entrada de direção IN1 do L298N
const byte PINO_IN2 = 7;  // Entrada de direção IN2 do L298N

unsigned long ultimoLog = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_ENA, OUTPUT);
  pinMode(PINO_IN1, OUTPUT);
  pinMode(PINO_IN2, OUTPUT);

  Serial.println(F("Sistema pronto! Controle de Motor DC com L298N."));
}

void loop() {
  int leitura = analogRead(PINO_POT);
  int pwm = 0;

  // Zona neutra no centro do potenciômetro (470 a 554)
  if (leitura > 554) {
    // Sentido Horário
    digitalWrite(PINO_IN1, HIGH);
    digitalWrite(PINO_IN2, LOW);
    pwm = map(leitura, 555, 1023, 70, 255);
    analogWrite(PINO_ENA, pwm);
  } else if (leitura < 470) {
    // Sentido Anti-horário
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, HIGH);
    pwm = map(leitura, 469, 0, 70, 255);
    analogWrite(PINO_ENA, pwm);
  } else {
    // Motor Parado
    digitalWrite(PINO_IN1, LOW);
    digitalWrite(PINO_IN2, LOW);
    analogWrite(PINO_ENA, 0);
  }

  if (millis() - ultimoLog >= 400) {
    ultimoLog = millis();
    Serial.print(F("Pot A0: "));
    Serial.print(leitura);
    if (pwm == 0) {
      Serial.println(F(" -> Motor PARADO"));
    } else if (leitura > 554) {
      Serial.print(F(" -> Sentido HORÁRIO | PWM: "));
      Serial.println(pwm);
    } else {
      Serial.print(F(" -> Sentido ANTI-HORÁRIO | PWM: "));
      Serial.println(pwm);
    }
  }

  delay(20);
}
