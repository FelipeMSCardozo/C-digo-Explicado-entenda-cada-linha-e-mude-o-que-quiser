/*
 * Projeto 05: Termômetro com LCD
 * O que faz: Lê temperatura e umidade do DHT11 a cada 2s e mostra no display LCD 16x2 I2C.
 * Ligações: D2 (DHT11 DATA), A4 (LCD SDA), A5 (LCD SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C (Frank de Brabander), DHT sensor library (Adafruit)
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

const byte PINO_DHT = 2;       // Pino DATA do sensor DHT11
const byte PINO_I2C_SDA = A4;  // Pino SDA do LCD 16x2 I2C
const byte PINO_I2C_SCL = A5;  // Pino SCL do LCD 16x2 I2C

const byte ENDERECO_LCD = 0x27; // AJUSTE AQUI: mude para 0x3F se o seu LCD usar chip PCF8574A
const unsigned long INTERVALO_LEITURA_MS = 2000;

LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);
DHT dht(PINO_DHT, DHT11);

unsigned long ultimaLeitura = 0;

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  dht.begin();
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(F("Termometro LCD"));
  lcd.setCursor(0, 1);
  lcd.print(F("Iniciando..."));
  Serial.println(F("Sistema pronto! Termômetro com LCD iniciado."));
  delay(1500);
  lcd.clear();
}

void loop() {
  unsigned long agora = millis();
  if (agora - ultimaLeitura >= INTERVALO_LEITURA_MS) {
    ultimaLeitura = agora;

    float umidade = dht.readHumidity();
    float temperatura = dht.readTemperature();

    if (isnan(umidade) || isnan(temperatura)) {
      Serial.println(F("Falha ao ler o sensor DHT11! Verifique o fio no pino D2."));
      lcd.setCursor(0, 0);
      lcd.print(F("Erro no DHT11!  "));
      lcd.setCursor(0, 1);
      lcd.print(F("Confira pino D2 "));
      return;
    }

    Serial.print(F("Temp: "));
    Serial.print(temperatura, 1);
    Serial.print(F(" C | Umidade: "));
    Serial.print((int)umidade);
    Serial.println(F(" %"));

    lcd.setCursor(0, 0);
    lcd.print(F("Temp: "));
    lcd.print(temperatura, 1);
    lcd.print(F(" "));
    lcd.write(223); // Símbolo de grau no LCD
    lcd.print(F("C   "));

    lcd.setCursor(0, 1);
    lcd.print(F("Umid: "));
    lcd.print((int)umidade);
    lcd.print(F(" %      "));
  }
}
