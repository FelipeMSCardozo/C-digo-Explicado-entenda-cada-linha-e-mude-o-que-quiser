// ============================================================================
// Projeto 78 — Painel de Comandos pelo Monitor Serial
// Placa: Arduino Uno R3 (ATmega328P) | Bibliotecas: Servo
// Material: 80 Projetos Arduino
// Interpretador de comandos CLI via USB para controlar 2 LEDs e Servo SG90
// ============================================================================

#include <Servo.h>

const byte PINO_LED1  = 4; // LED 1 Verde
const byte PINO_LED2  = 5; // LED 2 Vermelho
const byte PINO_SERVO = 9; // Servo Motor SG90

Servo meuServo;
bool estadoLed1 = false;
bool estadoLed2 = false;
int anguloAtual = 90;

void imprimirMenu() {
  Serial.println(F("================================================"));
  Serial.println(F("COMANDOS DISPONIVEIS (digite e tecle Enter):"));
  Serial.println(F("  LED1 ON  / LED1 OFF  -> Liga/desliga LED Verde"));
  Serial.println(F("  LED2 ON  / LED2 OFF  -> Liga/desliga LED Vermelho"));
  Serial.println(F("  SERVO <0 a 180>      -> Ex.: SERVO 45 ou SERVO 180"));
  Serial.println(F("  STATUS               -> Exibe estado atual"));
  Serial.println(F("  AJUDA                -> Mostra este menu"));
  Serial.println(F("================================================"));
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(150);

  pinMode(PINO_LED1, OUTPUT);
  pinMode(PINO_LED2, OUTPUT);
  digitalWrite(PINO_LED1, LOW);
  digitalWrite(PINO_LED2, LOW);

  meuServo.attach(PINO_SERVO);
  meuServo.write(anguloAtual);

  Serial.println(F("Sistema pronto! Painel de Comandos Serial (digite AJUDA)."));
  imprimirMenu();
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    if (cmd.length() == 0) return;

    Serial.print(F("> "));
    Serial.println(cmd);

    if (cmd == F("LED1 ON")) {
      estadoLed1 = true;
      digitalWrite(PINO_LED1, HIGH);
      Serial.println(F("[OK] LED 1 (Verde) ACESO."));
    } else if (cmd == F("LED1 OFF")) {
      estadoLed1 = false;
      digitalWrite(PINO_LED1, LOW);
      Serial.println(F("[OK] LED 1 (Verde) APAGADO."));
    } else if (cmd == F("LED2 ON")) {
      estadoLed2 = true;
      digitalWrite(PINO_LED2, HIGH);
      Serial.println(F("[OK] LED 2 (Vermelho) ACESO."));
    } else if (cmd == F("LED2 OFF")) {
      estadoLed2 = false;
      digitalWrite(PINO_LED2, LOW);
      Serial.println(F("[OK] LED 2 (Vermelho) APAGADO."));
    } else if (cmd.startsWith(F("SERVO "))) {
      int ang = cmd.substring(6).toInt();
      ang = constrain(ang, 0, 180);
      anguloAtual = ang;
      meuServo.write(anguloAtual);
      Serial.print(F("[OK] Servo posicionado em "));
      Serial.print(anguloAtual);
      Serial.println(F(" graus."));
    } else if (cmd == F("STATUS")) {
      Serial.println(F("--- STATUS DO SISTEMA ---"));
      Serial.print(F("LED1: "));
      Serial.print(estadoLed1 ? F("ON") : F("OFF"));
      Serial.print(F(" | LED2: "));
      Serial.print(estadoLed2 ? F("ON") : F("OFF"));
      Serial.print(F(" | Servo: "));
      Serial.print(anguloAtual);
      Serial.println(F(" graus"));
    } else if (cmd == F("AJUDA")) {
      imprimirMenu();
    } else {
      Serial.println(F("[ERRO] Comando desconhecido. Digite AJUDA."));
    }
  }
}
