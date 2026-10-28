/*
 * Projeto 50: Comedouro Automático para Pets
 * O que faz: Abre a comporta (Servo SG90) nos horários programados no RTC DS3231 ou pelo botão manual.
 * Ligações: D2 (Botão Manual), D9 (Servo SG90), A4 (RTC SDA), A5 (RTC SCL)
 * Bibliotecas: Wire, RTClib, Servo
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

const byte PINO_BOTAO = 2;    // Botão de dose manual imediata
const byte PINO_SERVO = 9;    // Servo motor da comporta de ração
const byte PINO_I2C_SDA = A4; // RTC SDA
const byte PINO_I2C_SCL = A5; // RTC SCL

const int HORA_REFEICAO_1 = 8;           // AJUSTE AQUI: hora da refeição (0 a 23)
const int MINUTO_REFEICAO_1 = 0;         // AJUSTE AQUI: minuto da refeição (0 a 59)
const unsigned int TEMPO_PORCAO_MS = 1200; // AJUSTE AQUI: tempo que a comporta fica aberta (ms)

RTC_DS3231 rtc;
Servo comporta;

int ultimoMinutoServido = -1;
unsigned long ultimoLogMs = 0;

void servirPorcao() {
  Serial.println(F("HORA DA REFEIÇÃO! Abrindo comporta..."));
  comporta.write(90); // Abre a comporta
  delay(TEMPO_PORCAO_MS);
  comporta.write(0);  // Fecha a comporta
  Serial.println(F("Comporta fechada. Porção servida com sucesso!"));
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  pinMode(PINO_BOTAO, INPUT_PULLUP);
  comporta.attach(PINO_SERVO);
  comporta.write(0); // Começa fechada

  if (rtc.begin() && rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println(F("Sistema pronto! Comedouro Automático para Pets ativo."));
}

void loop() {
  if (digitalRead(PINO_BOTAO) == LOW) {
    Serial.println(F("Botão manual acionado!"));
    servirPorcao();
    delay(500);
  }

  DateTime agora = rtc.now();

  if (agora.hour() == HORA_REFEICAO_1 && agora.minute() == MINUTO_REFEICAO_1 && agora.minute() != ultimoMinutoServido) {
    ultimoMinutoServido = agora.minute();
    servirPorcao();
  }

  if (agora.minute() != MINUTO_REFEICAO_1) {
    ultimoMinutoServido = -1;
  }

  if (millis() - ultimoLogMs >= 2000) {
    ultimoLogMs = millis();
    Serial.print(F("Hora atual: "));
    Serial.print(agora.hour());
    Serial.print(':');
    if (agora.minute() < 10) Serial.print('0');
    Serial.print(agora.minute());
    Serial.print(':');
    if (agora.second() < 10) Serial.print('0');
    Serial.println(agora.second());
  }
}
