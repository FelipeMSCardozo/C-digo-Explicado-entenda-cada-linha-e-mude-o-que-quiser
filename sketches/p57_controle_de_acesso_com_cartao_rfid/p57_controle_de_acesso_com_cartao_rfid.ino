/*
 * Projeto 57: Controle de Acesso com Cartão RFID
 * O que faz: Lê cartões/tags no leitor RFID RC522 (3.3V) e libera a catraca (Servo SG90) para o UID autorizado.
 * Ligações: D9 (RST), D10 (SDA/SS), D11 (MOSI), D12 (MISO), D13 (SCK), D6 (Servo), D4 (LED Verde), D5 (LED Vermelho), D7 (Buzzer)
 * Bibliotecas: SPI, MFRC522 (GithubCommunity), Servo
 * Material: 80 Projetos Arduino
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

const byte PINO_LED_VERDE = 4;    // LED de acesso permitido
const byte PINO_LED_VERMELHO = 5; // LED de acesso negado
const byte PINO_SERVO = 6;        // Sinal do servo da catraca
const byte PINO_BUZZER = 7;       // Buzzer ativo 5V
const byte PINO_RST = 9;          // Pino RST do RC522
const byte PINO_SDA = 10;         // Pino SDA (SS) do RC522
const byte PINO_MOSI = 11;        // SPI MOSI do RC522
const byte PINO_MISO = 12;        // SPI MISO do RC522
const byte PINO_SCK = 13;         // SPI SCK do RC522

const bool AUTO_CADASTRAR_PRIMEIRO = true; // AJUSTE AQUI: cadastra o 1º cartão aproximado como Mestre

MFRC522 rfid(PINO_SDA, PINO_RST);
Servo catraca;

byte uidMestre[4] = {0xDE, 0xAD, 0xBE, 0xEF};
bool mestreDefinido = !AUTO_CADASTRAR_PRIMEIRO;

void imprimirUid(const byte* uid) {
  for (byte i = 0; i < 4; i++) {
    if (uid[i] < 0x10) Serial.print('0');
    Serial.print(uid[i], HEX);
    if (i < 3) Serial.print(' ');
  }
}

void setup() {
  Serial.begin(9600);
  (void)PINO_MOSI;
  (void)PINO_MISO;
  (void)PINO_SCK;

  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  catraca.attach(PINO_SERVO);
  catraca.write(0);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println(F("Sistema pronto! Controle de Acesso RFID RC522 ativo."));
  if (!mestreDefinido) {
    Serial.println(F("Aproxime o primeiro cartão para cadastrar como Mestre..."));
  }
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  if (!mestreDefinido) {
    for (byte i = 0; i < 4; i++) {
      uidMestre[i] = rfid.uid.uidByte[i];
    }
    mestreDefinido = true;
    Serial.print(F("Cartão Mestre cadastrado! UID: "));
    imprimirUid(uidMestre);
    Serial.println();

    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(100);
    digitalWrite(PINO_BUZZER, LOW);
    delay(100);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(100);
    digitalWrite(PINO_BUZZER, LOW);
    digitalWrite(PINO_LED_VERDE, LOW);

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return;
  }

  Serial.print(F("UID lido: "));
  imprimirUid(rfid.uid.uidByte);

  bool autorizado = true;
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != uidMestre[i]) {
      autorizado = false;
      break;
    }
  }

  if (autorizado) {
    Serial.println(F(" -> ACESSO LIBERADO!"));
    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(120);
    digitalWrite(PINO_BUZZER, LOW);
    catraca.write(90);
    delay(3000);
    catraca.write(0);
    digitalWrite(PINO_LED_VERDE, LOW);
  } else {
    Serial.println(F(" -> ACESSO NEGADO!"));
    digitalWrite(PINO_LED_VERMELHO, HIGH);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(600);
    digitalWrite(PINO_BUZZER, LOW);
    delay(500);
    digitalWrite(PINO_LED_VERMELHO, LOW);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
