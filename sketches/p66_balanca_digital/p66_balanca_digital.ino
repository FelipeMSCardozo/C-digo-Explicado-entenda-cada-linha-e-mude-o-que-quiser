// ============================================================================
// Projeto 66 — Balança Digital
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C, HX711 Arduino Library
// Material: 80 Projetos Arduino
// Célula de carga + Amplificador 24-bit HX711 + Botão TARA + LCD 16x2 I2C
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>

const byte PINO_HX_DT    = 2;  // Pino DT (DOUT) do módulo HX711
const byte PINO_HX_SCK   = 3;  // Pino SCK do módulo HX711
const byte PINO_BTN_TARA = 4;  // Botão de TARA (INPUT_PULLUP)
const byte PINO_I2C_SDA  = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL  = A5; // Barramento I2C SCL

// AJUSTE AQUI: Fator de calibração da sua célula de carga.
// Como calibrar: FATOR_CALIBRACAO = (Valor_Bruto_no_Serial) / (Peso_Real_em_Gramas)
// Exemplo típico: ~420.0 para célula de 5 kg ou ~2100.0 para célula de 1 kg.
const float FATOR_CALIBRACAO = 420.0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
HX711 balanca;

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_BTN_TARA, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Balanca Digital"));
  lcd.setCursor(0, 1);
  lcd.print(F("Zerando prato..."));

  balanca.begin(PINO_HX_DT, PINO_HX_SCK);
  balanca.set_scale(FATOR_CALIBRACAO);

  if (balanca.wait_ready_timeout(1500)) {
    balanca.tare(10); // Zera a balança com média de 10 leituras
    Serial.println(F("Sistema pronto! Balanca Digital inicializada e tarada."));
  } else {
    Serial.println(F("Aviso: Modulo HX711 nao respondeu em D2/D3."));
  }
  lcd.clear();
}

void loop() {
  // Verifica se o botão de TARA foi pressionado
  if (digitalRead(PINO_BTN_TARA) == LOW) {
    lcd.setCursor(0, 1);
    lcd.print(F(">> TARANDO... <<"));
    Serial.println(F("Aplicando TARA (zerando peso do recipiente)..."));
    if (balanca.wait_ready_timeout(1000)) {
      balanca.tare(10);
    }
    delay(300);
  }

  if (balanca.wait_ready_timeout(400)) {
    float gramas = balanca.get_units(5);
    long valorBruto = balanca.get_value(1);

    // Elimina oscilação residual menor que 0,3 g perto do zero
    if (gramas > -0.3 && gramas < 0.3) gramas = 0.0;

    lcd.setCursor(0, 0);
    lcd.print(F("Peso: "));
    lcd.print(gramas, 1);
    lcd.print(F(" g      "));

    lcd.setCursor(0, 1);
    lcd.print(F("D4=Tara         "));

    Serial.print(F("Peso: "));
    Serial.print(gramas, 1);
    Serial.print(F(" g | Valor Bruto (p/ calibrar): "));
    Serial.println(valorBruto);
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F("Aguardando HX711"));
    lcd.setCursor(0, 1);
    lcd.print(F("Cheque D2 e D3  "));
  }

  delay(150);
}
