// ============================================================================
// Projeto 70 — Primeiros Passos com Display OLED
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, Adafruit GFX Library, Adafruit SSD1306
// Material: 80 Projetos Arduino
// Interface gráfica 128x64 pixels no Display OLED SSD1306 I2C (endereço 0x3C)
// ============================================================================

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte PINO_I2C_SDA = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL = A5; // Barramento I2C SCL

const byte LARGURA_OLED = 128;
const byte ALTURA_OLED  = 64;

// AJUSTE AQUI: Endereço I2C padrão do OLED SSD1306 é 0x3C
const byte ENDERECO_OLED = 0x3C;

Adafruit_SSD1306 tela(LARGURA_OLED, ALTURA_OLED, &Wire, -1);

byte progresso = 0;

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  if (!tela.begin(SSD1306_SWITCHCAPVCC, ENDERECO_OLED)) {
    Serial.println(F("Erro: Display OLED SSD1306 nao encontrado em 0x3C!"));
    while (true) { delay(100); }
  }

  Serial.println(F("Sistema pronto! Display OLED 128x64 inicializado."));
}

void loop() {
  unsigned long segundos = millis() / 1000;

  tela.clearDisplay();

  // Moldura externa arredondada
  tela.drawRoundRect(0, 0, 128, 64, 4, SSD1306_WHITE);

  // Cabeçalho
  tela.setTextColor(SSD1306_WHITE);
  tela.setTextSize(1);
  tela.setCursor(8, 5);
  tela.print(F("PAINEL GRAFICO OLED"));
  tela.drawLine(0, 15, 127, 15, SSD1306_WHITE);

  // Contador em fonte grande (tamanho 2)
  tela.setTextSize(2);
  tela.setCursor(8, 22);
  tela.print(segundos);
  tela.print(F(" s"));

  // Círculo pulsante à direita
  byte raio = 4 + (progresso / 20);
  tela.drawCircle(106, 29, raio, SSD1306_WHITE);

  // Barra de progresso inferior (0 a 100%)
  tela.drawRect(8, 46, 112, 11, SSD1306_WHITE);
  int larguraBarra = map(progresso, 0, 100, 0, 108);
  tela.fillRect(10, 48, larguraBarra, 7, SSD1306_WHITE);

  // Envia o buffer da RAM para a tela OLED via I2C
  tela.display();

  progresso += 4;
  if (progresso > 100) progresso = 0;

  delay(80);
}
