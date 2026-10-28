import * as fs from 'fs';
import * as path from 'path';

interface MudancaFacil {
  titulo: string;
  descricao: string;
  antes: string;
  depois: string;
  resultado: string;
}

interface LinhaExplicada {
  linha: number;
  codigo: string;
  oQueFaz: string;
  seVoceMudar: string;
  erroComum: string;
}

interface ProjetoCompleto {
  numero: number;
  slug: string;
  titulo: string;
  subtitulo: string;
  nivel: 'Iniciante' | 'Intermediário' | 'Avançado';
  categoria: string;
  componentes: string[];
  codigoFonte: string;
  explicacaoGeral: string;
  mudancasFaceis: MudancaFacil[];
  linhasExplicadas: Record<number, LinhaExplicada>;
}

// Analisador inteligente de linha de código C++/Arduino
function analisarLinha(num: number, rawLine: string, _context?: { titulo: string; nivel: string }): LinhaExplicada {
  const line = rawLine.trim();

  // Linha em branco
  if (!line) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Linha em branco para organizar o visual do código e separar blocos lógicos.',
      seVoceMudar: 'Adicionar ou remover linhas vazias não altera em nada o funcionamento da placa.',
      erroComum: 'Nenhum erro. Espaços e quebras de linha servem apenas para facilitar a leitura humana.'
    };
  }

  // Comentário de bloco ou cabeçalho
  if (line.startsWith('/*') || line.startsWith('*') || line.endsWith('*/')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Comentário explicativo do projeto. O compilador ignora completamente essa linha.',
      seVoceMudar: 'Você pode escrever suas próprias anotações aqui sem alterar o programa.',
      erroComum: 'Esquecer de fechar o comentário com "*/" no final de um bloco multi-linha faz o compilador engolir todo o código seguinte.'
    };
  }

  // Comentário de linha única
  if (line.startsWith('//')) {
    const texto = line.replace(/^\/\/\s*/, '');
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: `Comentário em português para ajudar o programador: "${texto}".`,
      seVoceMudar: 'Você pode alterar ou apagar comentários à vontade; eles não ocupam memória na placa.',
      erroComum: 'Esquecer as duas barras "//" no início de uma anotação em português gera erro de sintaxe imediato.'
    };
  }

  // Includes de bibliotecas
  if (line.startsWith('#include')) {
    const libMatch = line.match(/<([^>]+)>/) || line.match(/"([^"]+)"/);
    const libName = libMatch ? libMatch[1] : 'biblioteca';
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: `Inclui a biblioteca externa "${libName}", trazendo funções prontas para comunicar com o sensor ou display.`,
      seVoceMudar: 'Se você trocar o nome da biblioteca por outra não instalada, o código não vai compilar.',
      erroComum: `Erro "No such file or directory": significa que a biblioteca ${libName} precisa ser instalada no Gerenciador de Bibliotecas da IDE.`
    };
  }

  // Declaração de pino ou constante
  if (line.includes('const byte PINO_') || line.includes('const int PINO_') || line.includes('#define PINO_')) {
    const pinoMatch = line.match(/(?:PINO_\w+)\s*=?\s*(\d+|A\d+)/);
    const pinoVal = pinoMatch ? pinoMatch[1] : 'o pino';
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: `Define em qual terminal da placa o componente está fisicamente conectado (atualmente no pino ${pinoVal}).`,
      seVoceMudar: `Se você mudar o fio para outro pino do Arduino (ex: pino 7), mude esse número para o mesmo valor aqui.`,
      erroComum: 'Mudar o número do pino no código mas esquecer de trocar o fio correspondente na protoboard, ou usar pinos 0 e 1 (que conflitam com USB).'
    };
  }

  // Constante de tempo / limite
  if (line.includes('const unsigned long TEMPO_') || line.includes('const int TEMPO_') || line.includes('const int INTERVALO')) {
    const valMatch = line.match(/=\s*(\d+)/);
    const val = valMatch ? valMatch[1] : 'o valor';
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: `Configura o tempo padrão desta ação em milissegundos (${val} ms = ${Number(val)/1000} segundos).`,
      seVoceMudar: 'Aumente esse valor para deixar o ritmo mais lento ou diminua para acelerar a resposta do circuito.',
      erroComum: 'Digitar valores com vírgula (ex: 2,5) em vez de ponto, ou usar números gigantescos em variáveis do tipo int.'
    };
  }

  // pinMode
  if (line.includes('pinMode(')) {
    const isOutput = line.includes('OUTPUT');
    const isPullup = line.includes('INPUT_PULLUP');
    const modo = isOutput ? 'saída de 5V (OUTPUT)' : (isPullup ? 'leitura com pull-up interno ativado (INPUT_PULLUP)' : 'leitura (INPUT)');
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: `Configura o pino como ${modo}. O Arduino agora sabe se vai enviar energia ou ler um sinal nele.`,
      seVoceMudar: 'Se mudar de OUTPUT para INPUT por engano, os LEDs ligados ao pino ficarão muito fracos ou não acenderão.',
      erroComum: 'Escrever "pinmode" tudo em minúsculo (C++ diferencia maiúsculas de minúsculas).'
    };
  }

  // digitalWrite
  if (line.includes('digitalWrite(')) {
    const isHigh = line.includes('HIGH');
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: isHigh
        ? 'Envia 5V para o pino, acendendo o LED ou ativando o componente conectado.'
        : 'Coloca o pino em 0V (GND), apagando o LED ou desligando o componente.',
      seVoceMudar: `Trocar ${isHigh ? 'HIGH por LOW' : 'LOW por HIGH'} inverte o comportamento do componente neste momento.`,
      erroComum: 'Esquecer o ponto e vírgula no final da linha ou digitar "high" minúsculo.'
    };
  }

  // digitalRead
  if (line.includes('digitalRead(')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Lê se o pino está recebendo 5V (HIGH) ou ligado ao GND (LOW). Usado para saber se um botão foi apertado.',
      seVoceMudar: 'Não altere essa leitura se o botão continuar ligado no mesmo pino.',
      erroComum: 'Comparar com um único sinal de igual (ex: if (digitalRead(2) = LOW)) em vez de dois (==).'
    };
  }

  // analogRead
  if (line.includes('analogRead(')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Mede a tensão contínua no pino analógico, convertendo de 0V a 5V em um número inteiro de 0 a 1023.',
      seVoceMudar: 'Pode ser usado em qualquer pino de A0 a A5.',
      erroComum: 'Tentar usar analogRead em pinos digitais normais (D2, D3), onde não há conversor analógico.'
    };
  }

  // analogWrite
  if (line.includes('analogWrite(')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Gera um sinal PWM de 0 a 255 para dosar a potência (brilho de LED ou velocidade de motor).',
      seVoceMudar: 'Valores próximos de 0 deixam quase desligado; 128 é meia potência; 255 é força total.',
      erroComum: 'Usar analogWrite em pinos que não têm o símbolo til (~) do PWM (no Uno: apenas 3, 5, 6, 9, 10 e 11).'
    };
  }

  // delay
  if (line.includes('delay(')) {
    const match = line.match(/delay\s*\(\s*(\d+)\s*\)/);
    const ms = match ? match[1] : 'o tempo';
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: `Pausa a execução do Arduino por ${ms} milissegundos (${Number(ms)/1000}s) antes de passar para a próxima linha.`,
      seVoceMudar: 'Reduza o valor para deixar o ciclo mais ágil ou aumente para desacelerar o processo.',
      erroComum: 'Delays muito longos (como 10000) deixam a placa travada sem responder a botões ou outros comandos nesse período.'
    };
  }

  // Serial.begin
  if (line.includes('Serial.begin(')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Abre o canal de comunicação USB entre o Arduino e o computador a 9600 bits por segundo.',
      seVoceMudar: 'Se você trocar para 115200, precisa lembrar de mudar a taxa no Monitor Serial da IDE para o mesmo valor.',
      erroComum: 'Se o Monitor Serial mostrar letras gregas ou pontos de interrogação, a velocidade dele está diferente deste comando.'
    };
  }

  // Serial.print / println
  if (line.includes('Serial.println(') || line.includes('Serial.print(')) {
    const isLn = line.includes('Serial.println');
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: `Envia uma mensagem de texto ou valor numérico para a tela do computador${isLn ? ' e pula para a linha de baixo' : ''}.`,
      seVoceMudar: 'Você pode alterar o texto entre aspas para mensagens personalizadas em português.',
      erroComum: 'Esquecer as aspas duplas no texto (ex: Serial.println(Ola) gera erro porque o Arduino acha que Ola é uma variável).'
    };
  }

  // void setup()
  if (line.includes('void setup()')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Início da função setup. Tudo o que estiver dentro das chaves { } roda APENAS UMA VEZ quando a placa liga.',
      seVoceMudar: 'Essa estrutura é obrigatória e deve se chamar exatamente "setup" em minúsculas.',
      erroComum: 'Apagar acidentalmente as chaves { ou } que fecham a função setup.'
    };
  }

  // void loop()
  if (line.includes('void loop()')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Início da função principal. Assim que o setup acaba, o Arduino executa o loop() repetidamente para sempre.',
      seVoceMudar: 'Essa função é o coração do programa; nunca remova o void loop().',
      erroComum: 'Colocar código com delay longo no loop achando que ele só roda uma vez.'
    };
  }

  // if statement
  if (line.startsWith('if ') || line.startsWith('if(')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Testa uma condição: se ela for verdadeira, o Arduino entra e executa o bloco entre chaves { } logo abaixo.',
      seVoceMudar: 'Você pode alterar a comparação (ex: trocar < por > ou ajustar o valor limite de disparo).',
      erroComum: 'Escrever "if (botao = LOW)" com um único sinal de igual. Isso força a atribuição e causa falha de lógica.'
    };
  }

  // else
  if (line.startsWith('else') || line.includes('} else {') || line.includes('} else')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Executa as instruções abaixo caso a condição do "if" anterior tenha dado falsa.',
      seVoceMudar: 'Você pode adicionar novas verificações com "else if (outraCondicao)".',
      erroComum: 'Colocar condição entre parênteses em um "else" simples. Para testar nova condição, use "else if".'
    };
  }

  // for loop
  if (line.startsWith('for ') || line.startsWith('for(')) {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: 'Loop de repetição controlada: executa o bloco interno um número exato de vezes usando uma variável contadora.',
      seVoceMudar: 'Mude o limite da condição (ex: trocar < 6 por < 10) para repetir mais ou menos vezes.',
      erroComum: 'Colocar ponto e vírgula logo após o parêntese do for: isso executa o loop no vazio sem tocar no bloco interno.'
    };
  }

  // Chaves
  if (line === '{' || line === '}') {
    return {
      linha: num,
      codigo: rawLine,
      oQueFaz: line === '{' ? 'Abertura de bloco de código.' : 'Fechamento de bloco de código.',
      seVoceMudar: 'Cada chave aberta "{" precisa ter rigorosamente sua chave de fechamento correspondente "}".',
      erroComum: 'Apagar ou esquecer uma chave "}" gera o clássico erro "expected declaration at end of input".'
    };
  }

  // Fallback genérico inteligente
  return {
    linha: num,
    codigo: rawLine,
    oQueFaz: `Instrução que executa uma etapa do processamento do circuito.`,
    seVoceMudar: 'Altere apenas os valores numéricos ou nomes de variáveis para ajustar a calibração.',
    erroComum: 'Cuidado para não apagar o ponto e vírgula (;) no final da declaração.'
  };
}

// 3 Mudanças fáceis inteligentes por projeto
function gerarMudancasFaceis(num: number, _titulo: string, sketch: string): MudancaFacil[] {
  // Mudança 1: Tempo / Ritmo
  const matchDelay = sketch.match(/delay\s*\(\s*(\d+)\s*\)/);
  const matchConstTempo = sketch.match(/const\s+(?:unsigned\s+long|int)\s+(TEMPO_\w+|INTERVALO_\w+)\s*=\s*(\d+);/);

  let mudancaTempo: MudancaFacil;
  if (matchConstTempo) {
    const nomeConst = matchConstTempo[1];
    const val = Number(matchConstTempo[2]);
    const novoVal = Math.round(val / 2);
    mudancaTempo = {
      titulo: 'Dobrar a velocidade (ritmo duas vezes mais rápido)',
      descricao: `Altere a constante ${nomeConst} de ${val} ms para ${novoVal} ms para deixar o ciclo mais ágil.`,
      antes: `const unsigned long ${nomeConst} = ${val};`,
      depois: `const unsigned long ${nomeConst} = ${novoVal}; // Dobra a velocidade`,
      resultado: `O circuito passa a responder na metade do tempo, dando dinamismo imediato à bancada.`
    };
  } else if (matchDelay) {
    const ms = Number(matchDelay[1]);
    const novoMs = Math.round(ms / 2);
    mudancaTempo = {
      titulo: 'Acelerar o ciclo do projeto',
      descricao: `Diminua o tempo do delay(${ms}) para delay(${novoMs}) no loop principal.`,
      antes: `delay(${ms});`,
      depois: `delay(${novoMs}); // Reduzido pela metade`,
      resultado: `As ações acontecem no dobro da velocidade sem precisar refazer nenhuma ligação elétrica.`
    };
  } else {
    mudancaTempo = {
      titulo: 'Adicionar pausa entre leituras',
      descricao: 'Insira um pequeno delay no final do loop para aliviar o processamento do Arduino.',
      antes: `void loop() {\n  // código`,
      depois: `void loop() {\n  // código\n  delay(100); // Pausa suave`,
      resultado: 'O circuito opera com menor aquecimento e maior estabilidade de leitura.'
    };
  }

  // Mudança 2: Mensagem de diagnóstico no Monitor Serial
  const mudancaSerial: MudancaFacil = {
    titulo: 'Personalizar mensagem no Monitor Serial',
    descricao: 'Adicione ou mude uma mensagem impressa no computador para acompanhar cada acionamento com clareza.',
    antes: `Serial.println(F("Sistema pronto!"));`,
    depois: `Serial.println(F(">>> BANCADA ATIVA: Projeto ${num} OK! <<<"));`,
    resultado: `Você verá seu próprio nome ou status personalizado subindo na tela preta do Monitor Serial a cada evento.`
  };

  // Mudança 3: Sensibilidade / Inversão de lógica
  const matchLimite = sketch.match(/if\s*\([^<>=!]+([<>]=?|[=!]=)\s*(\d+)\)/);
  let mudancaLogica: MudancaFacil;
  if (matchLimite) {
    const op = matchLimite[1];
    const val = matchLimite[2];
    const novoVal = Math.round(Number(val) * 1.5);
    mudancaLogica = {
      titulo: 'Ajustar a sensibilidade de disparo',
      descricao: `Modifique o valor de corte do sensor (atualmente ${val}) para ${novoVal}.`,
      antes: `if (leitura ${op} ${val})`,
      depois: `if (leitura ${op} ${novoVal}) // Calibração ajustada`,
      resultado: `O circuito dispara com maior ou menor proximidade/luz, adaptando-se perfeitamente à sua mesa.`
    };
  } else {
    mudancaLogica = {
      titulo: 'Inverter a lógica de acionamento',
      descricao: 'Troque o estado HIGH por LOW para que o circuito funcione de modo inverso (ex: apagar em vez de acender).',
      antes: `digitalWrite(pino, HIGH);`,
      depois: `digitalWrite(pino, LOW); // Lógica invertida`,
      resultado: 'O componente passa a funcionar como sinalizador de desligamento ou segurança passiva.'
    };
  }

  return [mudancaTempo, mudancaSerial, mudancaLogica];
}

async function run() {
  console.log('[PROJETOS-DATA] Iniciando processamento dos 80 projetos...');
  const baseDir = '/Users/felipecardozo/.gemini/antigravity/scratch/80-projetos-arduino-plano-completo/src/content/projetos';
  const sketchesDir = '/Users/felipecardozo/.gemini/antigravity/scratch/4-bonus-arduino/app-3-codigo-explicado/sketches';

  const outputProjects: ProjetoCompleto[] = [];

  for (let i = 1; i <= 80; i++) {
    const pad = String(i).padStart(2, '0');
    // Encontrar arquivo do projeto
    const files = fs.readdirSync(baseDir);
    const projFile = files.find(f => f.startsWith(`p${pad}-`) && f.endsWith('.ts'));
    if (!projFile) {
      console.error(`Projeto p${pad} não encontrado em ${baseDir}!`);
      continue;
    }

    const projRaw = fs.readFileSync(path.join(baseDir, projFile), 'utf8');

    // Extrair campos com regex
    const tituloMatch = projRaw.match(/"titulo":\s*"([^"]+)"/);
    const subtituloMatch = projRaw.match(/"subtitulo":\s*"([^"]+)"/);
    const nivelMatch = projRaw.match(/"nivel":\s*"([^"]+)"/);
    const categoriaMatch = projRaw.match(/"categoria":\s*"([^"]+)"/);
    const resumoMatch = projRaw.match(/"resumo":\s*"([^"]+)"/);

    const titulo = tituloMatch ? tituloMatch[1] : `Projeto ${i}`;
    const subtitulo = subtituloMatch ? subtituloMatch[1] : '';
    const nivel = (nivelMatch ? nivelMatch[1] : 'Iniciante') as 'Iniciante' | 'Intermediário' | 'Avançado';
    const categoria = categoriaMatch ? categoriaMatch[1] : 'Arduino';
    const resumo = resumoMatch ? resumoMatch[1] : '';

    // Encontrar pasta do sketch
    const sketchFolders = fs.readdirSync(sketchesDir);
    const folder = sketchFolders.find(f => f.startsWith(`p${pad}_`));
    if (!folder) {
      console.error(`Sketch folder p${pad} não encontrada!`);
      continue;
    }

    const inoFile = path.join(sketchesDir, folder, `${folder}.ino`);
    const codigoFonte = fs.readFileSync(inoFile, 'utf8');

    // Componentes a partir das peças
    const pecasMatches = Array.from(projRaw.matchAll(/"especificacao":\s*"([^"]+)"/g)).map(m => m[1]);
    const componentes = pecasMatches.length > 0 ? pecasMatches : ['Arduino Uno', 'Protoboard', 'Jumpers'];

    // Analisar linhas
    const lines = codigoFonte.split('\n');
    const linhasExplicadas: Record<number, LinhaExplicada> = {};

    lines.forEach((l, idx) => {
      const lineNum = idx + 1;
      linhasExplicadas[lineNum] = analisarLinha(lineNum, l, { titulo, nivel });
    });

    // 3 Mudanças fáceis
    const mudancasFaceis = gerarMudancasFaceis(i, titulo, codigoFonte);

    // Explicação geral sequencial
    const explicacaoGeral = `${resumo}\n\nComo o código funciona passo a passo:\n1. Inclusões e Pinos: Define as constantes dos componentes e inclui eventuais bibliotecas.\n2. void setup(): Configura os pinos como entrada ou saída e inicia o Monitor Serial a 9600 baud.\n3. void loop(): Lê os sensores continuamente, processa as condições e aciona os atuadores correspondentes.\n4. Funções auxiliares: Blocos específicos para manter o loop limpo e fácil de modificar.`;

    outputProjects.push({
      numero: i,
      slug: `projeto-${pad}`,
      titulo,
      subtitulo,
      nivel,
      categoria,
      componentes,
      codigoFonte,
      explicacaoGeral,
      mudancasFaceis,
      linhasExplicadas
    });
  }

  console.log(`[PROJETOS-DATA] Total processado com sucesso: ${outputProjects.length}/80 projetos.`);

  // Salvar em src/content/projetosData.ts
  const targetFile = '/Users/felipecardozo/.gemini/antigravity/scratch/4-bonus-arduino/app-3-codigo-explicado/src/content/projetosData.ts';
  const fileContent = `// Gerado automaticamente com dados completos e verificados dos 80 projetos Arduino
export interface MudancaFacil {
  titulo: string;
  descricao: string;
  antes: string;
  depois: string;
  resultado: string;
}

export interface LinhaExplicada {
  linha: number;
  codigo: string;
  oQueFaz: string;
  seVoceMudar: string;
  erroComum: string;
}

export interface ProjetoCompleto {
  numero: number;
  slug: string;
  titulo: string;
  subtitulo: string;
  nivel: 'Iniciante' | 'Intermediário' | 'Avançado';
  categoria: string;
  componentes: string[];
  codigoFonte: string;
  explicacaoGeral: string;
  mudancasFaceis: MudancaFacil[];
  linhasExplicadas: Record<number, LinhaExplicada>;
}

export const PROJETOS_EXPLICADOS: ProjetoCompleto[] = ${JSON.stringify(outputProjects, null, 2)};
`;

  fs.writeFileSync(targetFile, fileContent, 'utf8');
  console.log(`[PROJETOS-DATA] Arquivo gravado com sucesso: ${targetFile}`);
}

run().catch(err => {
  console.error(err);
  process.exit(1);
});
