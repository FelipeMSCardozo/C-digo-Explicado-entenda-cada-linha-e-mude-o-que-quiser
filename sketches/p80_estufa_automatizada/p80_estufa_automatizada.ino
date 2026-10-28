// ============================================================================
// Projeto 80 — Estufa Automatizada
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C, DHT sensor library
// Material: 80 Projetos Arduino
// Controle integrado de Irrigação (Solo + Relé/Bomba), Clima (DHT11 + Ventoinha)
// e Luz Solar (LDR) com telemetria em duas páginas no Display LCD 16x2 I2C
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const byte PINO_SOLO       = A0; // Entrada analógica do sensor de solo
const byte PINO_LDR        = A1; // Entrada analógica do LDR
const byte PINO_DHT        = 2;  // Sensor DHT11
const byte PINO_VCC_SOLO   = 6;  // Alimentação pulsada do sensor de solo
const byte PINO_RELE_BOMBA = 7;  // Módulo Relé da Mini Bomba 5V
const byte PINO_VENTOINHA  = 9;  // Transistor NPN da Ventoinha 5V
const byte PINO_I2C_SDA    = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL    = A5; // Barramento I2C SCL

// AJUSTE AQUI: Metas agronômicas da estufa
const int   SOLO_SECO_PCT   = 35;   // Irriga abaixo de 35%
const int   SOLO_IDEAL_PCT  = 50;   // Desliga a bomba ao atingir 50%
const float TEMP_MAX_ESTUFA = 29.0; // Liga a ventoinha acima de 29,0 °C
const byte  NIVEL_RELE_ON   = LOW;
const byte  NIVEL_RELE_OFF  = HIGH;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(PINO_DHT, DHT11);

float tempAr = 25.0;
float umidAr = 55.0;
int umidSolo = 60;
int luzSolar = 50;
bool bombaAtiva     = false;
bool ventoinhaAtiva = false;
byte paginaLcd      = 0;

unsigned long ultimoCicloMs = 0;

int medirSoloPulsado() {
  digitalWrite(PINO_VCC_SOLO, HIGH);
  delay(20);
  int bruto = analogRead(PINO_SOLO);
  digitalWrite(PINO_VCC_SOLO, LOW);
  return constrain(map(bruto, 1000, 350, 0, 100), 0, 100);
}

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_VCC_SOLO, OUTPUT);
  digitalWrite(PINO_VCC_SOLO, LOW);
  pinMode(PINO_RELE_BOMBA, OUTPUT);
  digitalWrite(PINO_RELE_BOMBA, NIVEL_RELE_OFF);
  pinMode(PINO_VENTOINHA, OUTPUT);
  digitalWrite(PINO_VENTOINHA, LOW);

  dht.begin();
  lcd.init();
  lcd.backlight();

  Serial.println(F("Sistema pronto! Estufa Agricola Automatizada (Projeto 80)."));
}

void loop() {
  if (millis() - ultimoCicloMs >= 2500) {
    ultimoCicloMs = millis();

    // 1. Leitura dos 3 sensores
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t) && !isnan(h)) {
      tempAr = t;
      umidAr = h;
    }
    umidSolo = medirSoloPulsado();
    luzSolar = constrain(map(analogRead(PINO_LDR), 0, 1023, 0, 100), 0, 100);

    // 2. Controle da Irrigação com histerese
    if (!bombaAtiva && umidSolo < SOLO_SECO_PCT) {
      bombaAtiva = true;
      digitalWrite(PINO_RELE_BOMBA, NIVEL_RELE_ON);
    } else if (bombaAtiva && umidSolo >= SOLO_IDEAL_PCT) {
      bombaAtiva = false;
      digitalWrite(PINO_RELE_BOMBA, NIVEL_RELE_OFF);
    }

    // 3. Controle da Exaustão Térmica com histerese
    if (!ventoinhaAtiva && tempAr >= TEMP_MAX_ESTUFA) {
      ventoinhaAtiva = true;
      analogWrite(PINO_VENTOINHA, 220);
    } else if (ventoinhaAtiva && tempAr <= (TEMP_MAX_ESTUFA - 0.8)) {
      ventoinhaAtiva = false;
      digitalWrite(PINO_VENTOINHA, LOW);
    }

    // 4. Alterna páginas no LCD 16x2 I2C
    lcd.clear();
    if (paginaLcd == 0) {
      lcd.setCursor(0, 0);
      lcd.print(F("Ar:"));
      lcd.print(tempAr, 1);
      lcd.print((char)223);
      lcd.print(F("C "));
      lcd.print((int)umidAr);
      lcd.print(F("%"));

      lcd.setCursor(0, 1);
      lcd.print(F("Solo:"));
      lcd.print(umidSolo);
      lcd.print(F("% Luz:"));
      lcd.print(luzSolar);
      lcd.print(F("%"));
      paginaLcd = 1;
    } else {
      lcd.setCursor(0, 0);
      lcd.print(F("Bomba: "));
      lcd.print(bombaAtiva ? F("LIGADA ") : F("PARADA "));

      lcd.setCursor(0, 1);
      lcd.print(F("Exaust:"));
      lcd.print(ventoinhaAtiva ? F("LIGADO ") : F("PARADO "));
      paginaLcd = 0;
    }

    // 5. Relatório completo no Monitor Serial
    Serial.print(F("[ESTUFA] Temp: "));
    Serial.print(tempAr, 1);
    Serial.print(F("C | Ar: "));
    Serial.print((int)umidAr);
    Serial.print(F("% | Solo: "));
    Serial.print(umidSolo);
    Serial.print(F("% | Luz: "));
    Serial.print(luzSolar);
    Serial.print(F("% | Bomba: "));
    Serial.print(bombaAtiva ? F("ON") : F("OFF"));
    Serial.print(F(" | Ventoinha: "));
    Serial.println(ventoinhaAtiva ? F("ON") : F("OFF"));
  }
}
