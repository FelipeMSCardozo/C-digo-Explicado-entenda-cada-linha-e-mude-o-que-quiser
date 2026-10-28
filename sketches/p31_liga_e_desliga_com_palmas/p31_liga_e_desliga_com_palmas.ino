/*
 * Projeto 31: Liga e Desliga com Palmas
 * O que faz: Alterna o estado do LED no pino D8 a cada palma detectada pelo sensor de som em D2.
 * Ligações: D2 (Pino DO do sensor de som), D8 (LED via resistor 220 Ω)
 * Bibliotecas: Nenhuma biblioteca extra
 * Material: 80 Projetos Arduino
 */

const byte PINO_SOM = 2; // Saída digital DO do módulo sensor de som
const byte PINO_LED = 8; // LED indicador com resistor de 220 Ω

const unsigned long JANELA_BLOQUEIO_MS = 400; // AJUSTE AQUI: tempo para ignorar o eco da palma

bool luzAcesa = false;
unsigned long ultimaPalmaMs = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PINO_SOM, INPUT);
  pinMode(PINO_LED, OUTPUT);
  digitalWrite(PINO_LED, LOW);

  Serial.println(F("Sistema pronto! Interruptor por Palmas ativo."));
}

void loop() {
  int leitura = digitalRead(PINO_SOM);

  // Detecta pulso de som respeitando a janela contra eco
  if (leitura == HIGH && (millis() - ultimaPalmaMs > JANELA_BLOQUEIO_MS)) {
    ultimaPalmaMs = millis();
    luzAcesa = !luzAcesa;
    digitalWrite(PINO_LED, luzAcesa ? HIGH : LOW);

    Serial.print(F("Palma detectada! -> Luz: "));
    Serial.println(luzAcesa ? F("ACESA") : F("APAGADA"));
  }
}
