/*
 * Projeto 16: Campainha com Botão
 * O que faz: Toca o som clássico "Ding-Dong" em um buzzer passivo quando o botão é pressionado.
 * Ligações: D2 (Botão ao GND), D8 (Buzzer Passivo)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BOTAO = 2;  // Botão da campainha ligado ao GND
const byte PINO_BUZZER = 8; // Buzzer passivo

const unsigned int NOTA_DING_HZ = 659; // AJUSTE AQUI: nota Mi5 (aguda)
const unsigned int NOTA_DONG_HZ = 523; // AJUSTE AQUI: nota Dó5 (grave)

void tocarDingDong() {
  Serial.println(F("Campainha acionada: DING-DONG!"));
  tone(PINO_BUZZER, NOTA_DING_HZ, 350);
  delay(400);
  tone(PINO_BUZZER, NOTA_DONG_HZ, 650);
  delay(700);
  noTone(PINO_BUZZER);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);

  Serial.println(F("Sistema pronto! Campainha Ding-Dong aguardando toque no botão."));
}

void loop() {
  if (digitalRead(PINO_BOTAO) == LOW) {
    tocarDingDong();
    delay(300); // Evita disparos repetidos imediatos
  }
}
