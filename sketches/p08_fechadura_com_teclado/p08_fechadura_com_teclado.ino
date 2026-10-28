/*
 * Projeto 08: Fechadura com Teclado
 * O que faz: Lê senha de 4 dígitos no teclado 4x4, mostra no LCD I2C e abre a tranca (Servo SG90).
 * Ligações: D2..D5 (Linhas Teclado), D6..D9 (Colunas Teclado), D10 (Servo), A0 (Buzzer), A4 (SDA), A5 (SCL)
 * Bibliotecas: Wire, LiquidCrystal I2C, Keypad, Servo
 * Material: 80 Projetos Arduino
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

const byte PINO_L1 = 2;        // Linha 1 do teclado 4x4
const byte PINO_L2 = 3;        // Linha 2 do teclado 4x4
const byte PINO_L3 = 4;        // Linha 3 do teclado 4x4
const byte PINO_L4 = 5;        // Linha 4 do teclado 4x4
const byte PINO_C1 = 6;        // Coluna 1 do teclado 4x4
const byte PINO_C2 = 7;        // Coluna 2 do teclado 4x4
const byte PINO_C3 = 8;        // Coluna 3 do teclado 4x4
const byte PINO_C4 = 9;        // Coluna 4 do teclado 4x4
const byte PINO_SERVO = 10;    // Sinal do servo motor da tranca
const byte PINO_BUZZER = A0;   // Buzzer ativo 5V
const byte PINO_I2C_SDA = A4;  // LCD SDA
const byte PINO_I2C_SCL = A5;  // LCD SCL

const byte ENDERECO_LCD = 0x27;        // AJUSTE AQUI: 0x27 ou 0x3F
const char SENHA_CORRETA[] = "1234";   // AJUSTE AQUI: senha de 4 dígitos

char mapaTeclas[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinosLinhas[4] = {PINO_L1, PINO_L2, PINO_L3, PINO_L4};
byte pinosColunas[4] = {PINO_C1, PINO_C2, PINO_C3, PINO_C4};

Keypad teclado = Keypad(makeKeymap(mapaTeclas), pinosLinhas, pinosColunas, 4, 4);
LiquidCrystal_I2C lcd(ENDERECO_LCD, 16, 2);
Servo tranca;

char digitado[5];
byte indice = 0;

void bipes(byte qtd, int duracaoMs) {
  for (byte i = 0; i < qtd; i++) {
    digitalWrite(PINO_BUZZER, HIGH);
    delay(duracaoMs);
    digitalWrite(PINO_BUZZER, LOW);
    delay(80);
  }
}

void telaInicial() {
  indice = 0;
  digitado[0] = '\0';
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Digite a senha:"));
  lcd.setCursor(0, 1);
}

void setup() {
  Serial.begin(9600);
  (void)PINO_I2C_SDA;
  (void)PINO_I2C_SCL;

  pinMode(PINO_BUZZER, OUTPUT);
  tranca.attach(PINO_SERVO);
  tranca.write(0); // Trancado em 0 graus

  lcd.init();
  lcd.backlight();
  telaInicial();
  Serial.println(F("Sistema pronto! Fechadura com Teclado ativa."));
}

void loop() {
  char tecla = teclado.getKey();
  if (!tecla) return;

  bipes(1, 40);

  if (tecla == '*') {
    Serial.println(F("Entrada limpa pelo usuário (*)."));
    telaInicial();
  } else if (tecla == '#') {
    digitado[indice] = '\0';
    lcd.clear();
    if (strcmp(digitado, SENHA_CORRETA) == 0) {
      Serial.println(F("Acesso Liberado! Abrindo tranca..."));
      lcd.setCursor(0, 0);
      lcd.print(F("Acesso Liberado!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Porta Aberta"));
      bipes(2, 100);
      tranca.write(90);
      delay(4000);
      tranca.write(0);
      Serial.println(F("Tranca fechada."));
    } else {
      Serial.println(F("Senha Incorreta!"));
      lcd.setCursor(0, 0);
      lcd.print(F("Senha Incorreta!"));
      lcd.setCursor(0, 1);
      lcd.print(F("Tente novamente"));
      bipes(1, 600);
      delay(1500);
    }
    telaInicial();
  } else if (indice < 4) {
    digitado[indice] = tecla;
    lcd.setCursor(indice, 1);
    lcd.print('*');
    indice++;
    Serial.println(F("Tecla pressionada: *"));
  }
}
