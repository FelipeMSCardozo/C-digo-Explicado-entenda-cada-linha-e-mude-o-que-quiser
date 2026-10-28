/*
 * Projeto 34: Trena Digital com LCD
 * O que faz: Mede distância com média de 5 amostras no HC-SR04, mostra em cm e m no LCD e possui botão HOLD.
 * Ligações: D2 (Botão HOLD), D6 (Trig), D7 (Echo), A4 (LCD SDA), A5 (LCD SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_BOTAO = 2;     // Botão de congelar leitura (HOLD)
const byte PINO_TRIG = 6;      // Trig do HC-SR04
const byte PINO_ECHO = 7;      // Echo do HC-SR04
const byte PINO_I2C_SDA = A4;  // LCD SDA
const byte PINO_I2C_SCL = A5;  // LCD SCL

const byte ENDERECO_LCD = 0x27;      // AJUSTE AQUI: 0x27 ou 0x3F
const float OFFSET_CAIXA_CM = 0.0;   // AJUSTE AQUI: acrescente o tamanho da caixa se medir pela traseira

LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);

bool modoHold = false;
bool botaoAnterior = HIGH;
unsigned long ultimaMedidaMs = 0;

float medirMediaCm() {
  float soma = 0.0;
  byte validas = 0;

  for (byte i = 0; i < 5; i++) {
    digitalWrite(PINO_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PINO_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PINO_TRIG, LOW);

    unsigned long dur = pulseIn(PINO_ECHO, HIGH, 30000UL);
    if (dur > 0) {
      float cm = (dur * 0.0343f) / 2.0f;
      if (cm >= 2.0f && cm <= 400.0f) {
        soma += cm;
        validas++;
      }
    }
    delay(12);
  }

  if (validas == 0) return -1.0f;
  return (soma / validas) + OFFSET_CAIXA_CM;
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_TRIG, OUTPUT);
  pinMode(PINO_ECHO, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(F("Trena Digital"));
  Serial.println(F("Sistema pronto! Trena Digital com LCD iniciada."));
  delay(1000);
  lcd.clear();
}

void loop() {
  bool leituraBtn = digitalRead(PINO_BOTAO);
  if (botaoAnterior == HIGH && leituraBtn == LOW) {
    modoHold = !modoHold;
    Serial.println(modoHold ? F("Botão HOLD acionado -> Medida congelada na tela!") : F("Modo AO VIVO retomado."));
    delay(50);
  }
  botaoAnterior = leituraBtn;

  if (!modoHold && (millis() - ultimaMedidaMs >= 250)) {
    ultimaMedidaMs = millis();
    float cm = medirMediaCm();

    lcd.setCursor(0, 0);
    if (cm < 0) {
      lcd.print(F("Fora de alcance "));
      lcd.setCursor(0, 1);
      lcd.print(F("2 a 400 cm      "));
    } else {
      float metros = cm / 100.0f;
      lcd.print(F("Med: "));
      lcd.print(cm, 1);
      lcd.print(F(" cm    "));

      lcd.setCursor(0, 1);
      lcd.print(metros, 2);
      lcd.print(F("m [AO VIVO] "));

      Serial.print(F("Distância média: "));
      Serial.print(cm, 1);
      Serial.print(F(" cm ("));
      Serial.print(metros, 2);
      Serial.println(F(" m) [AO VIVO]"));
    }
  } else if (modoHold) {
    lcd.setCursor(6, 1);
    lcd.print(F("[TRAVADO] "));
  }
}
