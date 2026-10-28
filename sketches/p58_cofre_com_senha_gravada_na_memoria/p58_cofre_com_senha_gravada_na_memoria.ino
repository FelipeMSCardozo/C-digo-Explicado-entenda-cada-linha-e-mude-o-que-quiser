/*
 * Projeto 58: Cofre com Senha Gravada na Memória
 * O que faz: Cofre eletrônico com teclado 4x4 e servo SG90 que salva a senha na EEPROM (troca pela tecla 'A').
 * Ligações: D2..D5 (Linhas), D6..D9 (Colunas), D10 (Servo), A0 (Buzzer), A1 (LED Verde), A2 (LED Vermelho)
 * Bibliotecas: EEPROM, Keypad, Servo
 * Material: 80 Projetos Arduino
 */

#include <EEPROM.h>
#include <Keypad.h>
#include <Servo.h>

const byte PINO_L1 = 2;           // Linha 1 do teclado
const byte PINO_L2 = 3;           // Linha 2 do teclado
const byte PINO_L3 = 4;           // Linha 3 do teclado
const byte PINO_L4 = 5;           // Linha 4 do teclado
const byte PINO_C1 = 6;           // Coluna 1 do teclado
const byte PINO_C2 = 7;           // Coluna 2 do teclado
const byte PINO_C3 = 8;           // Coluna 3 do teclado
const byte PINO_C4 = 9;           // Coluna 4 do teclado
const byte PINO_SERVO = 10;       // Servo motor da tranca
const byte PINO_BUZZER = A0;      // Buzzer ativo 5V
const byte PINO_LED_VERDE = A1;   // LED Verde (Cofre Aberto)
const byte PINO_LED_VERMELHO = A2; // LED Vermelho (Cofre Trancado)

const byte ASSINATURA_EEPROM = 0xA5; // AJUSTE AQUI: mude este byte se quiser resetar para "1234"

char teclas[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pinosL[4] = {PINO_L1, PINO_L2, PINO_L3, PINO_L4};
byte pinosC[4] = {PINO_C1, PINO_C2, PINO_C3, PINO_C4};

Keypad teclado = Keypad(makeKeymap(teclas), pinosL, pinosC, 4, 4);
Servo ferrolho;

char senhaSalva[5] = "1234";
char buffer[5];
byte pos = 0;
bool cofreAberto = false;
bool trocandoSenha = false;

void apitar(byte vezes, int ms) {
  for (byte i = 0; i < vezes; i++) {
    digitalWrite(PINO_BUZZER, HIGH);
    delay(ms);
    digitalWrite(PINO_BUZZER, LOW);
    delay(70);
  }
}

void carregarSenhaEeprom() {
  if (EEPROM.read(0) == ASSINATURA_EEPROM) {
    for (byte i = 0; i < 4; i++) {
      senhaSalva[i] = (char)EEPROM.read(1 + i);
    }
    senhaSalva[4] = '\0';
  } else {
    EEPROM.update(0, ASSINATURA_EEPROM);
    for (byte i = 0; i < 4; i++) {
      EEPROM.update(1 + i, senhaSalva[i]);
    }
  }
}

void salvarNovaSenha(const char* nova) {
  for (byte i = 0; i < 4; i++) {
    senhaSalva[i] = nova[i];
    EEPROM.update(1 + i, nova[i]);
  }
  senhaSalva[4] = '\0';
}

void atualizarEstadoCofre(bool abrir) {
  cofreAberto = abrir;
  trocandoSenha = false;
  pos = 0;
  ferrolho.write(abrir ? 90 : 0);
  digitalWrite(PINO_LED_VERDE, abrir ? HIGH : LOW);
  digitalWrite(PINO_LED_VERMELHO, abrir ? LOW : HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);

  ferrolho.attach(PINO_SERVO);
  carregarSenhaEeprom();
  atualizarEstadoCofre(false);

  Serial.println(F("Sistema pronto! Cofre com Senha na EEPROM ativo."));
  Serial.println(F("Digite a senha de 4 dígitos + [#] (ou [*] para trancar/limpar)."));
}

void loop() {
  char k = teclado.getKey();
  if (!k) return;

  apitar(1, 35);

  if (k == '*') {
    atualizarEstadoCofre(false);
    Serial.println(F("Cofre TRANCADO."));
    return;
  }

  if (k == 'A' && cofreAberto && !trocandoSenha) {
    trocandoSenha = true;
    pos = 0;
    Serial.println(F("Modo Troca de Senha: digite os 4 novos dígitos + [#]..."));
    return;
  }

  if (k == '#') {
    buffer[pos] = '\0';
    if (trocandoSenha) {
      if (pos == 4) {
        salvarNovaSenha(buffer);
        trocandoSenha = false;
        pos = 0;
        apitar(3, 90);
        Serial.println(F("Nova senha salva com sucesso na memória EEPROM!"));
      } else {
        Serial.println(F("A nova senha deve ter exatamente 4 dígitos!"));
        apitar(1, 500);
        pos = 0;
      }
    } else {
      if (strcmp(buffer, senhaSalva) == 0) {
        atualizarEstadoCofre(true);
        apitar(2, 100);
        Serial.println(F("Cofre ABERTO! Aperte [A] para trocar a senha ou [*] para trancar."));
      } else {
        pos = 0;
        apitar(1, 600);
        Serial.println(F("Senha incorreta!"));
      }
    }
    return;
  }

  if (k >= '0' && k <= '9' && pos < 4) {
    buffer[pos++] = k;
    Serial.print('*');
    if (pos == 4) Serial.println();
  }
}
