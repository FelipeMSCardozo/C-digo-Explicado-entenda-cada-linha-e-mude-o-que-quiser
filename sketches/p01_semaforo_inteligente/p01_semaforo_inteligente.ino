/*
 * Projeto 01: Semáforo Inteligente
 * O que faz: Mantém o sinal verde para os carros e abre a travessia de pedestres ao pressionar o botão.
 * Ligações: D2 (Botão ao GND), D8 (Verde Carro), D9 (Amarelo Carro), D10 (Vermelho Carro), D11 (Verde Ped), D12 (Vermelho Ped)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BOTAO = 2;              // Botão de pedestre ligado ao GND
const byte PINO_CARRO_VERDE = 8;        // LED verde dos carros
const byte PINO_CARRO_AMARELO = 9;      // LED amarelo dos carros
const byte PINO_CARRO_VERMELHO = 10;    // LED vermelho dos carros
const byte PINO_PED_VERDE = 11;         // LED verde dos pedestres
const byte PINO_PED_VERMELHO = 12;      // LED vermelho dos pedestres

const unsigned long TEMPO_TRAVESSIA_MS = 4000; // AJUSTE AQUI: duração da travessia aberta (ms)

void estadoNormalCarros() {
  digitalWrite(PINO_CARRO_VERDE, HIGH);
  digitalWrite(PINO_CARRO_AMARELO, LOW);
  digitalWrite(PINO_CARRO_VERMELHO, LOW);
  digitalWrite(PINO_PED_VERDE, LOW);
  digitalWrite(PINO_PED_VERMELHO, HIGH);
}

void executarCicloTravessia() {
  Serial.println(F("Botão acionado! Preparando travessia..."));
  delay(800);

  // Fase 1: Amarelo para os carros
  Serial.println(F("Carros: AMARELO"));
  digitalWrite(PINO_CARRO_VERDE, LOW);
  digitalWrite(PINO_CARRO_AMARELO, HIGH);
  delay(2000);

  // Fase 2: Vermelho para carros e Verde para pedestres
  Serial.println(F("Carros: VERMELHO | Pedestres: VERDE (atravesse!)"));
  digitalWrite(PINO_CARRO_AMARELO, LOW);
  digitalWrite(PINO_CARRO_VERMELHO, HIGH);
  digitalWrite(PINO_PED_VERMELHO, LOW);
  digitalWrite(PINO_PED_VERDE, HIGH);
  delay(TEMPO_TRAVESSIA_MS);

  // Fase 3: Alerta piscante para o pedestre terminar de atravessar
  Serial.println(F("Atenção pedestre: sinal fechando..."));
  digitalWrite(PINO_PED_VERDE, LOW);
  for (byte i = 0; i < 6; i++) {
    digitalWrite(PINO_PED_VERMELHO, HIGH);
    delay(250);
    digitalWrite(PINO_PED_VERMELHO, LOW);
    delay(250);
  }

  // Retorna ao fluxo normal
  estadoNormalCarros();
  Serial.println(F("Carros: VERDE | Pedestres: VERMELHO (aguardando botão)"));
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_CARRO_VERDE, OUTPUT);
  pinMode(PINO_CARRO_AMARELO, OUTPUT);
  pinMode(PINO_CARRO_VERMELHO, OUTPUT);
  pinMode(PINO_PED_VERDE, OUTPUT);
  pinMode(PINO_PED_VERMELHO, OUTPUT);

  estadoNormalCarros();
  Serial.println(F("Sistema pronto! Semáforo Inteligente iniciado."));
  Serial.println(F("Carros: VERDE | Pedestres: VERMELHO (aguardando botão)"));
}

void loop() {
  // Com INPUT_PULLUP, apertar o botão leva o pino para LOW (0V)
  if (digitalRead(PINO_BOTAO) == LOW) {
    delay(40); // Debounce simples
    if (digitalRead(PINO_BOTAO) == LOW) {
      executarCicloTravessia();
      delay(1500); // Pausa mínima antes de aceitar novo pedido
    }
  }
}
