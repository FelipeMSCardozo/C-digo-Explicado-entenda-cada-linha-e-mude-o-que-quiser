// ============================================================================
// Projeto 73 — Jogo de Pular Obstáculos no LCD
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Wire, LiquidCrystal I2C
// Material: 80 Projetos Arduino
// Jogo estilo Runner com sprites customizados no LCD 16x2 I2C e botão de pulo
// ============================================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const byte PINO_BOTAO_PULO = 2;  // Botão de Pulo (INPUT_PULLUP)
const byte PINO_I2C_SDA    = A4; // Barramento I2C SDA
const byte PINO_I2C_SCL    = A5; // Barramento I2C SCL

// AJUSTE AQUI: Tempo entre quadros em ms (menor = mais difícil)
const unsigned long QUADRO_MS = 180;

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte SPRITE_HEROI[8] = {
  0b01110,
  0b01110,
  0b00100,
  0b11111,
  0b00100,
  0b01010,
  0b10001,
  0b00000
};

byte SPRITE_CACTO[8] = {
  0b00100,
  0b00101,
  0b10101,
  0b10110,
  0b01100,
  0b00100,
  0b00100,
  0b00000
};

int colunaCacto = 15;
byte quadrosNoAr = 0;
unsigned int pontos = 0;
unsigned int recorde = 0;
unsigned long ultimoQuadro = 0;

void setup() {
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;
  Serial.begin(9600);

  pinMode(PINO_BOTAO_PULO, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.createChar(0, SPRITE_HEROI);
  lcd.createChar(1, SPRITE_CACTO);

  Serial.println(F("Sistema pronto! Jogo de Pular Obstaculos no LCD."));
}

void loop() {
  if (digitalRead(PINO_BOTAO_PULO) == LOW && quadrosNoAr == 0) {
    quadrosNoAr = 3; // Permanece 3 quadros na linha superior
    Serial.print(F("Pulo! Pontos: "));
    Serial.println(pontos);
  }

  if (millis() - ultimoQuadro >= QUADRO_MS) {
    ultimoQuadro = millis();

    lcd.clear();

    // Desenha o herói na coluna 1 (linha 0 se pulando, linha 1 se no chão)
    byte linhaHeroi = (quadrosNoAr > 0) ? 0 : 1;
    lcd.setCursor(1, linhaHeroi);
    lcd.write((byte)0);

    // Desenha o cacto na linha 1
    lcd.setCursor(colunaCacto, 1);
    lcd.write((byte)1);

    // Placar no canto superior direito
    lcd.setCursor(9, 0);
    lcd.print(F("P:"));
    lcd.print(pontos);

    // Verifica colisão na coluna 1
    if (colunaCacto == 1 && quadrosNoAr == 0) {
      if (pontos > recorde) recorde = pontos;
      Serial.print(F("Fim de Jogo! Pontuacao: "));
      Serial.print(pontos);
      Serial.print(F(" | Recorde: "));
      Serial.println(recorde);

      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print(F("FIM DE JOGO!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Pts:"));
      lcd.print(pontos);
      lcd.print(F(" Rec:"));
      lcd.print(recorde);
      delay(2000);

      pontos = 0;
      colunaCacto = 15;
      quadrosNoAr = 0;
      return;
    }

    // Atualiza física do pulo e avanço do cacto
    if (quadrosNoAr > 0) quadrosNoAr--;
    colunaCacto--;
    if (colunaCacto < 0) {
      colunaCacto = random(12, 16);
      pontos++;
    }
  }
}
