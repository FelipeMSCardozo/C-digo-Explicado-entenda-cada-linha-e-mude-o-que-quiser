export interface FuncaoDicionario {
  id: string;
  nome: string;
  categoria: 'Entrada/Saída' | 'Tempo' | 'Comunicação Serial' | 'Controle de Fluxo' | 'Matemática' | 'Tipos de Dados';
  descricao: string;
  sintaxe: string;
  exemplo: string;
  dicaPratica: string;
  erroComum: string;
  projetosExemplo: number[];
}

export const DICIONARIO_FUNCOES: FuncaoDicionario[] = [
  {
    id: 'pinMode',
    nome: 'pinMode()',
    categoria: 'Entrada/Saída',
    descricao: 'Avisa o Arduino se um determinado pino vai funcionar como saída de energia (OUTPUT) para acionar algo ou como entrada (INPUT ou INPUT_PULLUP) para ler um botão ou sensor.',
    sintaxe: 'pinMode(pino, modo); // modo: OUTPUT, INPUT ou INPUT_PULLUP',
    exemplo: `void setup() {
  pinMode(13, OUTPUT);       // Pino 13 vai mandar energia para o LED
  pinMode(2, INPUT_PULLUP);  // Pino 2 vai ler um botão ligado ao GND
}

void loop() {
  // código principal
}`,
    dicaPratica: 'Sempre coloque o pinMode() dentro de void setup(). Não precisa repetir no loop.',
    erroComum: 'Esquecer o M maiúsculo ("pinmode" em vez de "pinMode") ou esquecer de definir o pino antes de usar digitalWrite.',
    projetosExemplo: [1, 9, 11, 16]
  },
  {
    id: 'digitalWrite',
    nome: 'digitalWrite()',
    categoria: 'Entrada/Saída',
    descricao: 'Coloca 5V (HIGH) ou 0V (LOW) em um pino digital configurado como OUTPUT. Liga ou desliga LEDs, relés, buzzers e outros componentes.',
    sintaxe: 'digitalWrite(pino, valor); // valor: HIGH ou LOW',
    exemplo: `void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH); // Acende o LED (5V)
  delay(1000);
  digitalWrite(13, LOW);  // Apaga o LED (0V)
  delay(1000);
}`,
    dicaPratica: 'Se o LED não acender mesmo com HIGH, confira se colocou pinMode(pino, OUTPUT) no setup e se a perna longa do LED está no pino correto.',
    erroComum: 'Digitar HIGH ou LOW com letras minúsculas (high/low gera erro de compilação).',
    projetosExemplo: [1, 9, 10, 16]
  },
  {
    id: 'digitalRead',
    nome: 'digitalRead()',
    categoria: 'Entrada/Saída',
    descricao: 'Lê o estado elétrico de um pino digital. Retorna HIGH (se houver ~5V no pino) ou LOW (se estiver conectado ao GND / 0V). Muito usado para botões e sensores de presença.',
    sintaxe: 'int estado = digitalRead(pino);',
    exemplo: `const int pinoBotao = 2;
const int pinoLed = 13;

void setup() {
  pinMode(pinoBotao, INPUT_PULLUP);
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  int estado = digitalRead(pinoBotao);
  if (estado == LOW) {
    digitalWrite(pinoLed, HIGH); // Botão pressionado fecha com o GND
  } else {
    digitalWrite(pinoLed, LOW);
  }
}`,
    dicaPratica: 'Com INPUT_PULLUP, o botão solto lê HIGH e quando você aperta lê LOW. Parece invertido no começo, mas dispensa resistor externo!',
    erroComum: 'Deixar o pino em INPUT sem nenhum resistor ligado: o pino fica "flutuando" e lê HIGH e LOW aleatoriamente por ruído eletrostático.',
    projetosExemplo: [1, 11, 16, 22]
  },
  {
    id: 'analogRead',
    nome: 'analogRead()',
    categoria: 'Entrada/Saída',
    descricao: 'Mede a tensão contínua em um dos pinos analógicos (A0 a A5). Converte de 0V a 5V em um número inteiro entre 0 e 1023 (resolução de 10 bits). Perfeito para potenciômetros, LDR e sensores de temperatura analógicos.',
    sintaxe: 'int leitura = analogRead(pinoAnalogico); // pino: A0, A1, etc.',
    exemplo: `void setup() {
  Serial.begin(9600);
}

void loop() {
  int valor = analogRead(A0); // Lê de 0 a 1023
  Serial.print("Valor do sensor: ");
  Serial.println(valor);
  delay(200);
}`,
    dicaPratica: 'Pinos analógicos NÃO precisam de pinMode() para leitura analógica.',
    erroComum: 'Tentar usar analogRead() nos pinos digitais normais (D2, D3...). Ele só funciona nos pinos A0 a A5 no Uno.',
    projetosExemplo: [12, 14, 15, 19]
  },
  {
    id: 'analogWrite',
    nome: 'analogWrite()',
    categoria: 'Entrada/Saída',
    descricao: 'Simula uma tensão intermediária usando modulação por largura de pulso (PWM) em pinos com o símbolo "~" (3, 5, 6, 9, 10 e 11). Permite variar o brilho de um LED ou a velocidade de um motor DC de 0 a 255.',
    sintaxe: 'analogWrite(pinoPWM, valor); // valor de 0 a 255',
    exemplo: `const int pinoLed = 9; // Pino com PWM (tem o til ~)

void setup() {
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  analogWrite(pinoLed, 64);   // Brilho fraco (25%)
  delay(1000);
  analogWrite(pinoLed, 255);  // Brilho máximo (100%)
  delay(1000);
}`,
    dicaPratica: 'O valor vai de 0 (desligado) a 255 (máximo). Para mapear a leitura de um potenciômetro (0-1023) para o PWM (0-255), divida por 4 ou use map().',
    erroComum: 'Tentar passar valores acima de 255 (como 1000) achando que vai ficar mais forte. Acima de 255 o valor "dá a volta" no número de 8 bits.',
    projetosExemplo: [12, 13, 14, 46]
  },
  {
    id: 'delay',
    nome: 'delay()',
    categoria: 'Tempo',
    descricao: 'Pausa a execução do programa pelo número de milissegundos especificado. 1000 milissegundos = 1 segundo.',
    sintaxe: 'delay(milissegundos);',
    exemplo: `void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  digitalWrite(13, HIGH);
  delay(500); // Espera meio segundo
  digitalWrite(13, LOW);
  delay(500); // Espera meio segundo
}`,
    dicaPratica: 'Ótimo para projetos iniciais e temporizações simples.',
    erroComum: 'O delay() congela totalmente o Arduino: enquanto o delay corre, a placa não consegue ler botões ou reagir a sensores. Para multitarefa, aprenda a usar millis().',
    projetosExemplo: [1, 9, 10, 13]
  },
  {
    id: 'millis',
    nome: 'millis()',
    categoria: 'Tempo',
    descricao: 'Retorna a quantidade de milissegundos decorridos desde que o Arduino foi ligado ou reiniciado. É o relógio interno que permite piscar LEDs e ler sensores ao mesmo tempo sem travar a placa.',
    sintaxe: 'unsigned long tempoAtual = millis();',
    exemplo: `unsigned long tempoAnterior = 0;
const long intervalo = 1000;
int estadoLed = LOW;

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervalo) {
    tempoAnterior = tempoAtual;
    estadoLed = (estadoLed == LOW) ? HIGH : LOW;
    digitalWrite(13, estadoLed);
  }
  // Aqui o Arduino pode ler botões instantaneamente!
}`,
    dicaPratica: 'Sempre guarde o valor de millis() em variáveis do tipo "unsigned long". Variáveis "int" estouram após apenas 32 segundos!',
    erroComum: 'Usar "int" ou "long" normal em vez de "unsigned long", causando comportamentos bizarros quando a contagem ultrapassa 32.767 ms.',
    projetosExemplo: [7, 36, 37, 73]
  },
  {
    id: 'Serial.begin',
    nome: 'Serial.begin()',
    categoria: 'Comunicação Serial',
    descricao: 'Inicia a comunicação serial entre o Arduino e o computador via cabo USB, definindo a taxa de transmissão em bits por segundo (baud rate). A velocidade padrão mais usada é 9600.',
    sintaxe: 'Serial.begin(velocidade); // ex: 9600 ou 115200',
    exemplo: `void setup() {
  Serial.begin(9600);
  Serial.println("Arduino pronto e comunicando!");
}

void loop() {
}`,
    dicaPratica: 'O valor no Serial.begin(9600) precisa ser exatamente o mesmo configurado no canto inferior direito do Monitor Serial da IDE.',
    erroComum: 'Monitor Serial exibindo caracteres estranhos (como ?). Isso acontece quase sempre quando a velocidade na IDE está diferente do Serial.begin.',
    projetosExemplo: [1, 2, 4, 25]
  },
  {
    id: 'Serial.println',
    nome: 'Serial.println()',
    categoria: 'Comunicação Serial',
    descricao: 'Envia texto, números ou variáveis pela porta serial para o computador e pula uma linha no final. É a melhor ferramenta de diagnóstico do mundo Arduino para saber o que a placa está pensando.',
    sintaxe: 'Serial.println(dado);',
    exemplo: `int temperatura = 25;

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Temperatura atual: ");
  Serial.print(temperatura);
  Serial.println(" oC"); // Pula linha no final
  delay(1000);
}`,
    dicaPratica: 'Use Serial.print() para escrever partes de uma frase na mesma linha e Serial.println() no último pedaço para pular a linha.',
    erroComum: 'Esquecer o "ln" no final e ver todos os dados grudados em uma linha gigante e ilegível no monitor.',
    projetosExemplo: [1, 2, 6, 25]
  },
  {
    id: 'if',
    nome: 'if / else',
    categoria: 'Controle de Fluxo',
    descricao: 'Testa uma condição: se ela for verdadeira, executa o bloco de código dentro das chaves { }. Se for falsa e houver um bloco else, executa o código alternativo.',
    sintaxe: `if (condicao) {
  // executa se verdadeiro
} else {
  // executa se falso
}`,
    exemplo: `int distancia = 15;

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  if (distancia < 20) {
    digitalWrite(13, HIGH); // Alerta se muito perto
  } else {
    digitalWrite(13, LOW);  // Desliga se seguro
  }
}`,
    dicaPratica: 'Use == para testar igualdade. Um único = serve para atribuir valor a uma variável.',
    erroComum: 'Escrever "if (botao = HIGH)" com um único sinal de igual. Isso força a variável a virar HIGH em vez de comparar, e a condição sempre dá verdadeira!',
    projetosExemplo: [1, 2, 4, 15]
  },
  {
    id: 'for',
    nome: 'for',
    categoria: 'Controle de Fluxo',
    descricao: 'Repete um bloco de código um número exato de vezes, controlando um contador automático. Excelente para fazer sequenciais de LEDs, efeitos sonoros ou varrer ângulos de um servo.',
    sintaxe: `for (inicializacao; condicao; incremento) {
  // codigo repetido
}`,
    exemplo: `void setup() {
  for (int pino = 2; pino <= 6; pino++) {
    pinMode(pino, OUTPUT); // Configura pinos 2, 3, 4, 5 e 6
  }
}

void loop() {
  for (int pino = 2; pino <= 6; pino++) {
    digitalWrite(pino, HIGH);
    delay(100);
    digitalWrite(pino, LOW);
  }
}`,
    dicaPratica: 'Em vez de copiar e colar 10 linhas repetidas, um loop for de 3 linhas faz o mesmo trabalho de forma limpa e profissional.',
    erroComum: 'Colocar ponto e vírgula logo após o fechamento do parêntese: "for (...); {}". Isso faz o loop rodar no vazio sem executar o bloco!',
    projetosExemplo: [10, 13, 20, 24]
  },
  {
    id: 'while',
    nome: 'while',
    categoria: 'Controle de Fluxo',
    descricao: 'Repete o código continuamente ENQUANTO uma condição continuar sendo verdadeira. Para assim que a condição se tornar falsa.',
    sintaxe: `while (condicao) {
  // codigo repetido enquanto condicao for true
}`,
    exemplo: `void setup() {
  Serial.begin(9600);
  // Espera o usuário digitar algo na serial antes de continuar
  while (!Serial.available()) {
    // fica aqui esperando sem fazer nada
  }
  Serial.println("Dado recebido! Continuando...");
}

void loop() {
}`,
    dicaPratica: 'Tenha certeza de que algo dentro do loop while vai mudar a condição no futuro, senão o Arduino trava para sempre em loop infinito.',
    erroComum: 'Criar um loop infinito involuntário esquecendo de atualizar a variável dentro do bloco.',
    projetosExemplo: [8, 41, 57, 58]
  },
  {
    id: 'map',
    nome: 'map()',
    categoria: 'Matemática',
    descricao: 'Mapeia proporcionalmente um número de uma escala de valores para outra. Exemplo clássico: transformar a leitura do potenciômetro (0 a 1023) no ângulo de um servo motor (0 a 180 graus).',
    sintaxe: 'map(valor, minOrigem, maxOrigem, minDestino, maxDestino);',
    exemplo: `#include <Servo.h>
Servo meuServo;

void setup() {
  meuServo.attach(9);
}

void loop() {
  int leitura = analogRead(A0);           // De 0 a 1023
  int angulo = map(leitura, 0, 1023, 0, 180); // Converte para 0 a 180
  meuServo.write(angulo);
  delay(15);
}`,
    dicaPratica: 'O map() não limita os valores se a entrada sair da faixa original. Se precisar travar dentro dos limites, use constrain() junto.',
    erroComum: 'Trocar a ordem dos parâmetros. Lembre-se: valor, depois os dois limites de onde veio, depois os dois limites para onde vai.',
    projetosExemplo: [3, 12, 14, 43]
  },
  {
    id: 'byte',
    nome: 'byte',
    categoria: 'Tipos de Dados',
    descricao: 'Armazena um número inteiro sem sinal de 8 bits, de 0 a 255. Ocupa apenas 1 byte de memória RAM (metade de um int). Ideal para pinos, estados de LED e valores de PWM.',
    sintaxe: 'byte variavel = valor; // 0 a 255',
    exemplo: `const byte pinoLed = 13;
byte brilho = 128;

void setup() {
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  analogWrite(pinoLed, brilho);
}`,
    dicaPratica: 'O Arduino Uno tem apenas 2.048 bytes de RAM. Usar byte para números pequenos economiza preciosa memória.',
    erroComum: 'Tentar guardar números negativos ou maiores que 255 em um byte. O valor estoura e reinicia do zero.',
    projetosExemplo: [1, 9, 20, 39]
  },
  {
    id: 'int',
    nome: 'int',
    categoria: 'Tipos de Dados',
    descricao: 'O tipo numérico mais comum no Arduino. Armazena inteiros com sinal de 16 bits, variando de -32.768 a +32.767. Ocupa 2 bytes.',
    sintaxe: 'int variavel = valor;',
    exemplo: `int contador = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  contador = contador + 1;
  Serial.println(contador);
  delay(1000);
}`,
    dicaPratica: 'Perfeito para contadores, leituras de analogRead() (que vai até 1023) e distâncias em centímetros.',
    erroComum: 'Tentar contar milissegundos com int. Como o máximo é 32.767, após 32 segundos a variável fica negativa!',
    projetosExemplo: [1, 2, 4, 5]
  },
  {
    id: 'unsigned long',
    nome: 'unsigned long',
    categoria: 'Tipos de Dados',
    descricao: 'Armazena inteiros gigantes sem sinal de 32 bits, de 0 até 4.294.967.295. Ocupa 4 bytes. É o tipo obrigatório para armazenar contagens de tempo da função millis().',
    sintaxe: 'unsigned long tempo = millis();',
    exemplo: `unsigned long momentoInicial = 0;

void setup() {
  Serial.begin(9600);
  momentoInicial = millis();
}

void loop() {
  unsigned long decorrido = millis() - momentoInicial;
  Serial.println(decorrido);
  delay(5000);
}`,
    dicaPratica: 'Com unsigned long, o relógio do millis() pode rodar continuamente por quase 50 dias antes de dar a volta a zero!',
    erroComum: 'Usar int no lugar de unsigned long para controlar temporizadores com millis().',
    projetosExemplo: [7, 36, 37, 73]
  }
];
