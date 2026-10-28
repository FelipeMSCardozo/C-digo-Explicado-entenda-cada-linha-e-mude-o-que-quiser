/*
 * Projeto 59: Alarme Residencial com Senha
 * O que faz: Central de alarme com PIR, teclado 4x4, LCD 16x2 I2C e sirene com estados Desarmado/Armado/Disparado.
 * Ligações: D2..D9 (Teclado 4x4), A0 (PIR OUT), A1 (Buzzer), A4 (LCD SDA), A5 (LCD SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C, Keypad
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte PINO_L1 = 2;       // Linha 1 do teclado
const byte PINO_L2 = 3;       // Linha 2 do teclado
const byte PINO_L3 = 4;       // Linha 3 do teclado
const byte PINO_L4 = 5;       // Linha 4 do teclado
const byte PINO_C1 = 6;       // Coluna 1 do teclado
const byte PINO_C2 = 7;       // Coluna 2 do teclado
const byte PINO_C3 = 8;       // Coluna 3 do teclado
const byte PINO_C4 = 9;       // Coluna 4 do teclado
const byte PINO_PIR = A0;     // Saída OUT do sensor PIR
const byte PINO_BUZZER = A1;  // Buzzer ativo (sirene)
const byte PINO_I2C_SDA = A4; // LCD SDA
const byte PINO_I2C_SCL = A5; // LCD SCL

const byte ENDERECO_LCD = 0x27;      // AJUSTE AQUI: 0x27 ou 0x3F
const char SENHA_CENTRAL[] = "1234"; // AJUSTE AQUI: senha de 4 dígitos

char mapa[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pL[4] = {PINO_L1, PINO_L2, PINO_L3, PINO_L4};
byte pC[4] = {PINO_C1, PINO_C2, PINO_C3, PINO_C4};

Keypad teclado = Keypad(makeKeymap(mapa), pL, pC, 4, 4);
LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);

// 0 = Desarmado, 1 = Armado Vigiando, 2 = Disparado
byte estado = 0;
char entrada[5];
byte idx = 0;

void mostrarCabecalho() {
  idx = 0;
  entrada[0] = '\0';
  lcd.clear();
  lcd.setCursor(0, 0);
  if (estado == 0) {
    lcd.print(F("DESARMADO"));
  } else if (estado == 1) {
    lcd.print(F("ARMADO VIGIANDO"));
  } else {
    lcd.print(F("INVASAO! SENHA:"));
  }
  lcd.setCursor(0, 1);
  lcd.print(F("Senha: "));
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  pinMode(PINO_PIR, INPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  mostrarCabecalho();
  Serial.println(F("Sistema pronto! Central de Alarme Residencial iniciada (DESARMADA)."));
}

void loop() {
  if (estado == 1 && digitalRead(PINO_PIR) == HIGH) {
    estado = 2;
    Serial.println(F("MOVIMENTO DETECTADO! Alarme DISPARADO!"));
    mostrarCabecalho();
  }

  if (estado == 2) {
    bool apito = ((millis() / 180) % 2 == 0);
    digitalWrite(PINO_BUZZER, apito ? HIGH : LOW);
  }

  char k = teclado.getKey();
  if (!k) return;

  if (k == '*') {
    mostrarCabecalho();
  } else if (k == '#') {
    entrada[idx] = '\0';
    if (strcmp(entrada, SENHA_CENTRAL) == 0) {
      if (estado == 0) {
        estado = 1;
        Serial.println(F("Senha correta -> Central ARMADA!"));
      } else {
        estado = 0;
        digitalWrite(PINO_BUZZER, LOW);
        Serial.println(F("Senha correta -> Alarme DESARMADO."));
      }
      mostrarCabecalho();
    } else {
      Serial.println(F("Senha incorreta!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Senha Incorreta!"));
      delay(900);
      mostrarCabecalho();
    }
  } else if (idx < 4) {
    entrada[idx] = k;
    lcd.setCursor(7 + idx, 1);
    lcd.print('*');
    idx++;
  }
}
