/*
 * Projeto 52: Interruptor com Relé (Baixa Tensão)
 * O que faz: Alterna um módulo relé (comandando fita LED / lâmpada 12V DC) a cada clique no botão.
 * Ligações: D2 (Botão ao GND), D8 (Pino IN do Módulo Relé)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_BOTAO = 2; // Botão ligado ao GND (INPUT_PULLUP)
const byte PINO_RELE = 8;  // Entrada IN do módulo relé

const bool RELE_ATIVO_LOW = true; // AJUSTE AQUI: true se o relé atracar com LOW, false se atracar com HIGH

bool cargaLigada = false;
bool botaoAnterior = HIGH;

void aplicarRele(bool ligar) {
  if (RELE_ATIVO_LOW) {
    digitalWrite(PINO_RELE, ligar ? LOW : HIGH);
  } else {
    digitalWrite(PINO_RELE, ligar ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PINO_BOTAO, INPUT_PULLUP);
  pinMode(PINO_RELE, OUTPUT);
  aplicarRele(false); // Inicia desligado

  Serial.println(F("Sistema pronto! Interruptor com Relé (Baixa Tensão 12V DC)."));
}

void loop() {
  bool leitura = digitalRead(PINO_BOTAO);

  if (botaoAnterior == HIGH && leitura == LOW) {
    cargaLigada = !cargaLigada;
    aplicarRele(cargaLigada);

    Serial.print(F("Botão pressionado -> Relé: "));
    Serial.println(cargaLigada ? F("LIGADO (Carga 12V ativa)") : F("DESLIGADO"));
    delay(60); // Debounce
  }

  botaoAnterior = leitura;
  delay(10);
}
