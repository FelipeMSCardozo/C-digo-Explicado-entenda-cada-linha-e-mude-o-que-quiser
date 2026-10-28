// ============================================================================
// Projeto 71 — Mini Estação de Clima no OLED
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, Adafruit GFX Library, Adafruit SSD1306, DHT sensor library
// Material: 80 Projetos Arduino
// Sensor DHT11 + Dashboard gráfico em 2 colunas no Display OLED SSD1306 128x64
// ============================================================================

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

const byte PINO_DHT     = 2;  // Pino DATA do DHT11
const byte PINO_I2C_SDA = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL

const byte ENDERECO_OLED = 0x3C;
const unsigned long INTERVALO_DHT_MS = 2000; // AJUSTE AQUI: mínimo 2000 ms para DHT11

Adafruit_SSD1306 tela(128, 64, &Wire, -1);
DHT dht(PINO_DHT, DHT11);

unsigned long ultimaLeitura = 0;
float tempC = 25.0;
float umidPct = 50.0;
bool leituraValida = false;

void desenharPainel() {
  tela.clearDisplay();
  tela.setTextColor(SSD1306_WHITE);

  if (!leituraValida) {
    tela.setTextSize(1);
    tela.setCursor(10, 24);
    tela.print(F("Lendo sensor DHT11"));
    tela.display();
    return;
  }

  // Cartão Esquerdo: Temperatura (0 a 62 px)
  tela.drawRoundRect(0, 0, 62, 48, 3, SSD1306_WHITE);
  tela.setTextSize(1);
  tela.setCursor(6, 4);
  tela.print(F("TEMP (C)"));
  tela.setTextSize(2);
  tela.setCursor(6, 16);
  tela.print((int)tempC);
  tela.setTextSize(1);
  tela.print(F("oC"));
  int barraT = constrain(map((int)tempC, 0, 50, 0, 50), 0, 50);
  tela.drawRect(6, 36, 50, 6, SSD1306_WHITE);
  tela.fillRect(6, 36, barraT, 6, SSD1306_WHITE);

  // Cartão Direito: Umidade (66 a 127 px)
  tela.drawRoundRect(66, 0, 62, 48, 3, SSD1306_WHITE);
  tela.setTextSize(1);
  tela.setCursor(72, 4);
  tela.print(F("UMIDADE"));
  tela.setTextSize(2);
  tela.setCursor(72, 16);
  tela.print((int)umidPct);
  tela.setTextSize(1);
  tela.print(F("%"));
  int barraU = constrain(map((int)umidPct, 0, 100, 0, 50), 0, 50);
  tela.drawRect(72, 36, 50, 6, SSD1306_WHITE);
  tela.fillRect(72, 36, barraU, 6, SSD1306_WHITE);

  // Rodapé de Conforto Climático
  tela.setTextSize(1);
  tela.setCursor(4, 54);
  if (umidPct < 35.0) {
    tela.print(F("Alerta: Ar Seco!"));
  } else if (tempC > 30.0 && umidPct > 70.0) {
    tela.print(F("Clima: Abafado"));
  } else {
    tela.print(F("Clima: Confortavel"));
  }

  tela.display();
}

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  dht.begin();
  if (!tela.begin(SSD1306_SWITCHCAPVCC, ENDERECO_OLED)) {
    Serial.println(F("Erro: OLED SSD1306 nao encontrado em 0x3C!"));
    while (true) { delay(100); }
  }

  Serial.println(F("Sistema pronto! Mini Estacao de Clima no OLED."));
  desenharPainel();
}

void loop() {
  if (millis() - ultimaLeitura >= INTERVALO_DHT_MS) {
    ultimaLeitura = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      tempC = t;
      umidPct = h;
      leituraValida = true;
      Serial.print(F("Temp: "));
      Serial.print(tempC, 1);
      Serial.print(F(" C | Umidade: "));
      Serial.print((int)umidPct);
      Serial.println(F(" %"));
    } else {
      Serial.println(F("Aviso: Falha na leitura do DHT11 no pino D2."));
    }
    desenharPainel();
  }
}
