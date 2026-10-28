// ============================================================================
// Projeto 79 — Casa Inteligente Integrada
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C, DHT sensor library
// Material: 80 Projetos Arduino
// Central Domótica: PIR + LDR + DHT11 + Relé (Clima) + LED (Luz) + Buzzer + LCD
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const byte PINO_LDR     = A0; // Sensor de luz LDR + resistor 10k
const byte PINO_DHT     = 2;  // Sensor DHT11
const byte PINO_PIR     = 3;  // Sensor de presença PIR
const byte PINO_LED_LUZ = 5;  // LED de iluminação da casa
const byte PINO_RELE_AC = 7;  // Módulo Relé (Climatização 5V/12V DC)
const byte PINO_BUZZER  = 8;  // Buzzer Ativo
const byte PINO_I2C_SDA = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL

// AJUSTE AQUI: Parâmetros de automação da casa
const int   LIMIAR_ESCURO = 400;  // Abaixo disso considera noite
const float TEMP_LIGAR_AC = 28.0; // Acima disso liga o relé de climatização
const byte  NIVEL_RELE_ON  = LOW;
const byte  NIVEL_RELE_OFF = HIGH;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(PINO_DHT, DHT11);

float tempCasa = 25.0;
float umidCasa = 50.0;
bool pirAnterior = false;
unsigned long ultimaLeituraDht = 0;
unsigned long ultimaTelaMs     = 0;

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_PIR, INPUT);
  pinMode(PINO_LED_LUZ, OUTPUT);
  pinMode(PINO_RELE_AC, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  digitalWrite(PINO_RELE_AC, NIVEL_RELE_OFF);
  digitalWrite(PINO_BUZZER, LOW);

  dht.begin();
  lcd.init();
  lcd.backlight();

  Serial.println(F("Sistema pronto! Central Casa Inteligente Integrada."));
}

void loop() {
  // 1. Leitura imediata de presença e luminosidade
  bool presenca = (digitalRead(PINO_PIR) == HIGH);
  int luzBruta  = analogRead(PINO_LDR);
  bool escuro   = (luzBruta < LIMIAR_ESCURO);

  // Acende a luz se estiver escuro OU se houver presença no ambiente
  bool luzAcesa = (escuro || presenca);
  digitalWrite(PINO_LED_LUZ, luzAcesa ? HIGH : LOW);

  // Bipe curto de boas-vindas quando alguém entra no cômodo
  if (presenca && !pirAnterior) {
    digitalWrite(PINO_BUZZER, HIGH);
    delay(40);
    digitalWrite(PINO_BUZZER, LOW);
  }
  pirAnterior = presenca;

  // 2. Leitura periódica de clima no DHT11 a cada 2 segundos
  if (millis() - ultimaLeituraDht >= 2000) {
    ultimaLeituraDht = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      tempCasa = t;
      umidCasa = h;
    }
  }

  // Controle automático do Relé de Climatização com histerese de 0,5 °C
  static bool acLigado = false;
  if (!acLigado && tempCasa >= TEMP_LIGAR_AC) {
    acLigado = true;
    digitalWrite(PINO_RELE_AC, NIVEL_RELE_ON);
  } else if (acLigado && tempCasa <= (TEMP_LIGAR_AC - 0.5)) {
    acLigado = false;
    digitalWrite(PINO_RELE_AC, NIVEL_RELE_OFF);
  }

  // 3. Atualização do painel LCD a cada 500 ms
  if (millis() - ultimaTelaMs >= 500) {
    ultimaTelaMs = millis();

    lcd.setCursor(0, 0);
    lcd.print((int)tempCasa);
    lcd.print((char)223);
    lcd.print(F("C "));
    lcd.print((int)umidCasa);
    lcd.print(F("% "));
    lcd.print(presenca ? F("[MOV]  ") : F("[VAZIO]"));

    lcd.setCursor(0, 1);
    lcd.print(F("Luz:"));
    lcd.print(luzAcesa ? F("ON ") : F("OFF"));
    lcd.print(F(" AC:"));
    lcd.print(acLigado ? F("ON   ") : F("OFF  "));

    Serial.print(F("Temp: "));
    Serial.print(tempCasa, 1);
    Serial.print(F("C Umid: "));
    Serial.print((int)umidCasa);
    Serial.print(F("% | Luz: "));
    Serial.print(luzAcesa ? F("ON") : F("OFF"));
    Serial.print(F(" | PIR: "));
    Serial.print(presenca ? 1 : 0);
    Serial.print(F(" | AC Rele: "));
    Serial.println(acLigado ? F("ON") : F("OFF"));
  }
}
