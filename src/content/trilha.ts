export interface LicaoTrilha {
  id: number;
  titulo: string;
  subtitulo: string;
  analogia: string;
  explicacao: string;
  exemploCodigo: string;
  testePratico: string;
  dicaOuro: string;
}

export const TRILHA_PROGRAMACAO: LicaoTrilha[] = [
  {
    id: 1,
    titulo: 'A Estrutura Básica: setup() e loop()',
    subtitulo: 'O que roda apenas uma vez vs o que roda para sempre',
    analogia: 'Imagine que o Arduino é um funcionário em um restaurante. O "setup()" é quando ele chega de manhã, veste o avental e liga o fogão (só faz uma vez). O "loop()" é o expediente atendendo pedidos: ele atende um cliente, depois outro, depois outro, sem parar até a hora de desligar a luz.',
    explicacao: 'Todo programa de Arduino precisa ter obrigatoriamente duas funções principais:\n\n1. void setup(): é executada exatamente UMA vez no instante em que a placa é ligada na tomada ou recebe energia pelo USB. É onde preparamos o terreno (avisamos quais pinos são saídas, iniciamos a porta serial com o computador, etc).\n\n2. void loop(): assim que o setup termina, o Arduino entra no loop. Ele executa a primeira linha, a segunda, a última... e quando chega no fim, volta instantaneamente para a primeira linha do loop de novo, repetindo milhares de vezes por segundo.',
    exemploCodigo: `void setup() {
  // Roda UMA ÚNICA VEZ ao ligar
  pinMode(13, OUTPUT); // Avisa que o pino 13 vai ligar o LED
}

void loop() {
  // Roda SEM PARAR em ciclo infinito
  digitalWrite(13, HIGH); // Acende o LED
  delay(1000);            // Espera 1 segundo
  digitalWrite(13, LOW);  // Apaga o LED
  delay(1000);            // Espera 1 segundo
}`,
    testePratico: 'Envie esse código para a placa. Repare no LED "L" embutido na placa Uno: ele vai piscar em um ritmo calmo de 1 segundo aceso e 1 segundo apagado.',
    dicaOuro: 'Nunca coloque código que só precisa rodar uma vez (como pinMode) dentro do loop. Senão a placa vai gastar tempo reconfigurando o pino milhares de vezes por segundo à toa.'
  },
  {
    id: 2,
    titulo: 'Variáveis: Caixas com Nome para Guardar Números',
    subtitulo: 'int, float, bool e byte: onde guardar seus dados',
    analogia: 'Pense em uma variável como uma caixa organizadora com uma etiqueta colada por fora. Na etiqueta você escreve o nome (ex: "tempoDeEspera") e dentro da caixa você coloca um número (ex: 500). Toda vez que você disser esse nome, o Arduino vai olhar o que tem dentro da caixa.',
    explicacao: 'No Arduino, antes de usar uma caixa, você precisa avisar que tipo de coisa cabe nela:\n\n• int: guarda números inteiros normais, como -5, 0, 100, 1023.\n• byte: uma caixa menor que só guarda inteiros de 0 a 255 (ocupa metade da memória).\n• float: guarda números com casas decimais (vírgula), como 23.5 ou 3.14.\n• bool (booleano): só guarda duas opções: true (verdadeiro/1) ou false (falso/0).\n\nSe você colocar a palavra "const" antes (ex: const int pino = 13;), você avisa que o número nunca vai mudar durante o programa. É a melhor prática para números de pinos!',
    exemploCodigo: `const int pinoLed = 13;       // O pino nunca muda de lugar
int tempoPisca = 200;         // Podemos mudar a velocidade
bool sistemaAtivo = true;     // Liga/Desliga

void setup() {
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  if (sistemaAtivo) {
    digitalWrite(pinoLed, HIGH);
    delay(tempoPisca);
    digitalWrite(pinoLed, LOW);
    delay(tempoPisca);
  }
}`,
    testePratico: 'Mude o valor de tempoPisca de 200 para 50 no código e reenvie. Você verá o LED piscar quatro vezes mais rápido, sem precisar alterar nada dentro do loop!',
    dicaOuro: 'Dê nomes claros para suas variáveis em português (como pinoSensor ou tempoEspera). Fuja de nomes de uma letra só como "x" ou "a", que você vai esquecer o que significavam daqui a duas semanas.'
  },
  {
    id: 3,
    titulo: 'Tomando Decisões: if e else',
    subtitulo: 'Se isso acontecer, faça aquilo. Senão, faça outra coisa',
    analogia: 'É a lógica que você usa no dia a dia: "SE estiver chovendo, levo guarda-chuva. SENÃO (se fizer sol), levo óculos de sol". O Arduino pensa do mesmo jeito com os sensores.',
    explicacao: 'O comando "if" (se) avalia uma condição entre parênteses. Se o resultado for verdadeiro, o Arduino executa o que está dentro das chaves { }.\n\nPara comparar valores, usamos operadores especiais:\n• == (dois iguais): testa se é igual a algo (um único = serve para guardar valor, nunca use no if!)\n• != (exclamação e igual): diferente de\n• > e <: maior que e menor que\n• >= e <=: maior ou igual, menor ou igual\n\nSe a condição for falsa, o bloco "else" (senão) assume o comando.',
    exemploCodigo: `const int pinoBotao = 2;
const int pinoLed = 13;

void setup() {
  pinMode(pinoBotao, INPUT_PULLUP); // Botão ligado no pino 2 e no GND
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  int estadoBotao = digitalRead(pinoBotao);

  if (estadoBotao == LOW) {
    // Se o botão foi apertado (ligou no GND)
    digitalWrite(pinoLed, HIGH); // Acende o LED
  } else {
    // Se o botão está solto
    digitalWrite(pinoLed, LOW);  // Apaga o LED
  }
}`,
    testePratico: 'Conecte um fio jumper do pino 2 ao pino GND do Arduino. O LED vai acender no momento exato em que você encostar o fio e apagar no momento em que soltar.',
    dicaOuro: 'Cuidado clássico de iniciante: escrever "if (estado = LOW)" com um único sinal de igual. Isso força o valor de estado a virar LOW e sempre entra no if! Sempre use dois sinais de igual "==" para comparar.'
  },
  {
    id: 4,
    titulo: 'Repetições: for e while sem Medo',
    subtitulo: 'Como fazer tarefas repetitivas em apenas 3 linhas de código',
    analogia: 'Imagine que você precisa conferir 5 lâmpadas na casa. Em vez de dizer "conferir lâmpada 1, conferir lâmpada 2, conferir lâmpada 3...", você diz: "Para cada lâmpada de 1 a 5, confira se está funcionando". O loop for faz exatamente isso.',
    explicacao: 'O loop "for" é usado quando você sabe exatamente quantas vezes quer repetir algo:\n\nfor (int i = 0; i < 5; i++) {\n  // roda 5 vezes, com i valendo 0, 1, 2, 3 e 4\n}\n\nJá o loop "while" (enquanto) repete enquanto uma condição continuar sendo verdadeira, sem limite prévio:\n\nwhile (sensorLendo == HIGH) {\n  // fica aqui até o sensor mudar\n}',
    exemploCodigo: `const int pinoLed = 13;

void setup() {
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  // Pisca rápido 3 vezes (alerta)
  for (int i = 0; i < 3; i++) {
    digitalWrite(pinoLed, HIGH);
    delay(100);
    digitalWrite(pinoLed, LOW);
    delay(100);
  }

  delay(2000); // Pausa longa antes de repetir a salva de piscadas
}`,
    testePratico: 'Carregue o código. Veja como o LED dá 3 piscadas rápidas ("tique-tique-tique"), espera 2 segundos e dá 3 piscadas de novo. Tudo com 4 linhas no for!',
    dicaOuro: 'Nunca coloque ponto e vírgula depois do parêntese do for: "for (...); {}". Se você fizer isso, o Arduino executa o loop vazio sem fazer nada e só depois vai para as chaves!'
  },
  {
    id: 5,
    titulo: 'Criando suas Próprias Funções',
    subtitulo: 'Organize seu código em blocos fáceis de ler e reutilizar',
    analogia: 'Em uma receita de bolo, o autor não reescreve como bater claras em neve a cada página. Ele simplesmente escreve "bata as claras em neve". Você cria um procedimento com nome próprio e chama sempre que precisar.',
    explicacao: 'Conforme seu projeto cresce, o loop() começa a ficar gigante e confuso. A solução profissional é dividir seu código em pequenas funções com nomes descritivos.\n\nUma função tem:\n1. O tipo de retorno: "void" se ela só executa e não devolve número, ou "int"/"float" se ela calcula algo para você.\n2. O nome: descreve o que ela faz (ex: piscarAlerta).\n3. Os parâmetros entre parênteses: os ingredientes que ela precisa para trabalhar.',
    exemploCodigo: `const int pinoLed = 13;

void setup() {
  pinMode(pinoLed, OUTPUT);
}

// Nossa função personalizada
void piscar(int vezes, int velocidadeMs) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(pinoLed, HIGH);
    delay(velocidadeMs);
    digitalWrite(pinoLed, LOW);
    delay(velocidadeMs);
  }
}

void loop() {
  piscar(2, 500); // Pisca 2 vezes devagar
  delay(1000);
  piscar(5, 100); // Pisca 5 vezes super rápido
  delay(2000);
}`,
    testePratico: 'Observe como o loop() ficou limpo, legível e direto ao ponto: apenas duas linhas autoexplicativas.',
    dicaOuro: 'Se você copiou e colou o mesmo bloco de código duas ou mais vezes no mesmo projeto, pare: isso deve virar uma função!'
  },
  {
    id: 6,
    titulo: 'O Segredo do millis(): Multitarefa Real',
    subtitulo: 'Como fazer duas coisas ao mesmo tempo sem travar com delay()',
    analogia: 'Se você colocar um bolo no forno por 40 minutos, você não fica 40 minutos parado olhando para a parede sem respirar (isso é o delay). Você olha para o relógio de pulso de vez em quando enquanto lava a louça e responde mensagens no celular (isso é o millis).',
    explicacao: 'O grande defeito da função delay(1000) é que ela CONGELA o processador do Arduino. Durante aquele segundo inteiro, a placa não lê botões, não escuta o Bluetooth e não reage a sensores.\n\nA função millis() resolve isso: ela funciona como um relógio de pulso que conta os milissegundos desde que a placa ligou. Guardamos o tempo da última ação e, a cada passada do loop, verificamos se já passou o tempo necessário.',
    exemploCodigo: `const int pinoLed = 13;
unsigned long tempoAnterior = 0;
const long intervaloPisca = 500; // Piscar a cada 500 ms
int estadoLed = LOW;

void setup() {
  pinMode(pinoLed, OUTPUT);
}

void loop() {
  unsigned long tempoAtual = millis(); // Olha o relógio

  // Já se passaram 500 ms desde a última troca?
  if (tempoAtual - tempoAnterior >= intervaloPisca) {
    tempoAnterior = tempoAtual; // Marca o novo momento
    estadoLed = (estadoLed == LOW) ? HIGH : LOW;
    digitalWrite(pinoLed, estadoLed);
  }

  // AQUI o Arduino continua livre para ler sensores instantaneamente!
}`,
    testePratico: 'Mesmo com o LED piscando a cada 500 ms, o Arduino está executando o loop() centenas de milhares de vezes por segundo, livre para ler botões sem nenhum atraso.',
    dicaOuro: 'Sempre declare as variáveis que guardam millis() como "unsigned long". Se usar "int", a contagem estoura e fica negativa após meros 32 segundos de funcionamento!'
  }
];
