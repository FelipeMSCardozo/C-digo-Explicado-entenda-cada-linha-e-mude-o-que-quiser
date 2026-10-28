/*
 * Projeto 12: Dimmer de LED com Potenciômetro
 * O que faz: Controla o brilho de um LED no pino PWM D9 proporcionalmente ao giro do potenciômetro em A0.
 * Ligações: A0 (Pino central do potenciômetro), D9 (LED via resistor 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_POT = A0; // Entrada analógica do potenciômetro (0 a 1023)
const byte PINO_LED = 9;  // Saída PWM (~) para o LED (0 a 255)

int ultimoPwm = -1;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_LED, OUTPUT);
  Serial.println(F("Sistema pronto! Dimmer de LED com PWM iniciado."));
}

void loop() {
  int leitura = analogRead(PINO_POT);
  int pwm = map(leitura, 0, 1023, 0, 255);

  analogWrite(PINO_LED, pwm);

  if (abs(pwm - ultimoPwm) >= 5) {
    ultimoPwm = pwm;
    int porcentagem = map(pwm, 0, 255, 0, 100);
    Serial.print(F("A0: "));
    Serial.print(leitura);
    Serial.print(F(" -> PWM: "));
    Serial.print(pwm);
    Serial.print(F(" (Brilho: "));
    Serial.print(porcentagem);
    Serial.println(F("%)"));
  }

  delay(30);
}
