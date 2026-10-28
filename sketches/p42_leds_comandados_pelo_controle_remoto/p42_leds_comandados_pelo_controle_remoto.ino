/*
 * Projeto 42: LEDs Comandados pelo Controle Remoto
 * O que faz: Liga/desliga 3 LEDs independentes usando as teclas 1, 2, 3 e 0 do controle remoto IR.
 * Ligações: D2 (Receptor IR), D8 (LED Vermelho), D9 (LED Verde), D10 (LED Azul) via resistores 220 Ω
 * Bibliotecas: IRremote (Armin Joachimsmeyer - API 4.x)
 * Material: 80 Projetos Arduino
 */

#include <IRremote.hpp>

const byte PINO_IR = 2;    // Sinal do receptor IR
const byte PINO_LED_1 = 8; // LED 1 (Vermelho)
const byte PINO_LED_2 = 9; // LED 2 (Verde)
const byte PINO_LED_3 = 10; // LED 3 (Azul)

// AJUSTE AQUI: códigos hexadecimais das teclas do seu controle remoto
const byte TECLA_1 = 0x0C; // Alterna LED 1
const byte TECLA_2 = 0x18; // Alterna LED 2
const byte TECLA_3 = 0x5E; // Alterna LED 3
const byte TECLA_0 = 0x16; // Apaga todos os LEDs

bool estado1 = false;
bool estado2 = false;
bool estado3 = false;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_LED_1, OUTPUT);
  pinMode(PINO_LED_2, OUTPUT);
  pinMode(PINO_LED_3, OUTPUT);

  IrReceiver.begin(PINO_IR, ENABLE_LED_FEEDBACK);
  Serial.println(F("Sistema pronto! LEDs Comandados por Controle Remoto."));
}

void loop() {
  if (IrReceiver.decode()) {
    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
      byte cmd = (byte)IrReceiver.decodedIRData.command;
      Serial.print(F("Comando recebido: 0x"));
      Serial.print(cmd, HEX);

      switch (cmd) {
        case TECLA_1:
          estado1 = !estado1;
          digitalWrite(PINO_LED_1, estado1 ? HIGH : LOW);
          Serial.println(F(" -> Alternando LED 1 (Vermelho)"));
          break;
        case TECLA_2:
          estado2 = !estado2;
          digitalWrite(PINO_LED_2, estado2 ? HIGH : LOW);
          Serial.println(F(" -> Alternando LED 2 (Verde)"));
          break;
        case TECLA_3:
          estado3 = !estado3;
          digitalWrite(PINO_LED_3, estado3 ? HIGH : LOW);
          Serial.println(F(" -> Alternando LED 3 (Azul)"));
          break;
        case TECLA_0:
          estado1 = false;
          estado2 = false;
          estado3 = false;
          digitalWrite(PINO_LED_1, LOW);
          digitalWrite(PINO_LED_2, LOW);
          digitalWrite(PINO_LED_3, LOW);
          Serial.println(F(" -> Todos os LEDs apagados!"));
          break;
        default:
          Serial.println(F(" (Tecla nao mapeada - veja ajustes no topo)"));
          break;
      }
    }
    IrReceiver.resume();
  }
}
