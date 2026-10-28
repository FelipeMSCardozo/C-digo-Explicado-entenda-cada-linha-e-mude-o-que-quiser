/*
 * Projeto 41: Leitor de Códigos do Controle Remoto
 * O que faz: Decodifica os sinais infravermelhos de qualquer controle remoto usando a API IRremote 4.x.
 * Ligações: D2 (Pino OUT/S do receptor IR), 5V (VCC), GND (GND)
 * Bibliotecas: IRremote (Armin Joachimsmeyer - API 4.x)
 * Material: 80 Projetos Arduino
 */

#include <IRremote.hpp>

const byte PINO_IR = 2; // Saída de sinal do módulo receptor infravermelho

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(PINO_IR, ENABLE_LED_FEEDBACK);
  Serial.println(F("Sistema pronto! Leitor de Controle Remoto (IRremote 4.x)."));
  Serial.println(F("Aponte o controle para o sensor e pressione qualquer tecla..."));
}

void loop() {
  if (IrReceiver.decode()) {
    // Ignora quadros de repetição contínua quando o botão permanece segurado
    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
      Serial.print(F("Tecla recebida -> Comando HEX: 0x"));
      Serial.print(IrReceiver.decodedIRData.command, HEX);
      Serial.print(F(" | Endereço: 0x"));
      Serial.println(IrReceiver.decodedIRData.address, HEX);
    }
    IrReceiver.resume(); // Prepara o receptor para o próximo código
  }
}
