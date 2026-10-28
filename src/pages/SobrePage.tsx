import React from 'react';
import { Card, AlertBox } from '../components/ui';

export const SobrePage: React.FC = () => {
  return (
    <div className="max-w-4xl mx-auto px-4 py-8 pb-24 md:pb-12 space-y-8">
      {/* Header */}
      <div className="text-center space-y-3 max-w-2xl mx-auto">
        <div className="inline-flex items-center gap-1.5 px-3 py-1 rounded-full bg-violet-500/10 border border-violet-500/30 text-violet-400 text-xs font-semibold uppercase tracking-wider">
          <span>📖</span> Manual de Uso
        </div>
        <h1 className="text-3xl sm:text-4xl font-black text-ink tracking-tight">
          Como Usar o Código Explicado
        </h1>
        <p className="text-ink-muted text-sm sm:text-base">
          Este aplicativo é o seu copiloto de programação para os 80 Projetos Arduino. Aprenda a tirar o máximo proveito dele na sua bancada.
        </p>
      </div>

      <div className="space-y-6">
        {/* Pilar 1: Linha por linha */}
        <Card className="space-y-3 border-violet-500/30">
          <div className="flex items-center gap-2">
            <span className="w-7 h-7 rounded-lg bg-violet-500/20 text-violet-400 font-bold text-sm flex items-center justify-center">
              1
            </span>
            <h2 className="text-lg font-bold text-ink">
              Visualizador Linha por Linha
            </h2>
          </div>
          <p className="text-sm text-ink-muted leading-relaxed">
            Abra o projeto que você está montando na sua bancada. Toque ou clique em qualquer linha de código. O app destaca a linha e mostra:
          </p>
          <ul className="text-xs sm:text-sm text-ink-muted space-y-1.5 list-disc pl-5">
            <li><strong>O que a linha faz:</strong> explicação em português claro, sem jargões desnecessários.</li>
            <li><strong>Se você mudar este valor:</strong> o que acontece na prática se você aumentar um tempo, trocar um pino ou inverter uma comparação.</li>
            <li><strong>Erros comuns:</strong> os deslizes clássicos de sintaxe (falta de ponto e vírgula, letras maiúsculas/minúsculas).</li>
          </ul>
        </Card>

        {/* Pilar 2: 3 Mudanças Fáceis */}
        <Card className="space-y-3 border-violet-500/30">
          <div className="flex items-center gap-2">
            <span className="w-7 h-7 rounded-lg bg-violet-500/20 text-violet-400 font-bold text-sm flex items-center justify-center">
              2
            </span>
            <h2 className="text-lg font-bold text-ink">
              3 Mudanças Fáceis por Projeto
            </h2>
          </div>
          <p className="text-sm text-ink-muted leading-relaxed">
            Montou o projeto e viu funcionar? Não pare por aí! Cada um dos 80 projetos conta com 3 modificações seguras já testadas na bancada. Mude a velocidade, adicione um status no monitor serial ou personalize a sensibilidade usando o botão &quot;Copiar alteração&quot;.
          </p>
        </Card>

        {/* Pilar 3: Dicionário & Trilha */}
        <Card className="space-y-3 border-violet-500/30">
          <div className="flex items-center gap-2">
            <span className="w-7 h-7 rounded-lg bg-violet-500/20 text-violet-400 font-bold text-sm flex items-center justify-center">
              3
            </span>
            <h2 className="text-lg font-bold text-ink">
              Dicionário Rápido e Trilha do Zero
            </h2>
          </div>
          <p className="text-sm text-ink-muted leading-relaxed">
            Se encontrou um comando que nunca viu antes, consulte o <strong>Dicionário</strong> com 16 comandos fundamentais do Arduino. E se você nunca programou na vida, faça a <strong>Trilha do Zero</strong> com 6 lições curtas e analógicas.
          </p>
        </Card>

        {/* Dica de bancada */}
        <AlertBox type="tip" title="Como usar no celular ao lado da placa">
          Você pode adicionar este aplicativo à tela inicial do seu celular (PWA). Ele funciona 100% offline depois do primeiro carregamento, perfeito para consultar na bancada mesmo sem conexão com a internet.
        </AlertBox>

        {/* Rodapé / Créditos */}
        <div className="text-center pt-4 text-xs text-ink-muted border-t border-bench-border/50">
          <p className="font-semibold text-ink">Código Explicado — 80 Projetos Arduino</p>
          <p className="text-[11px] mt-0.5">Desenvolvido com foco no estudante e hobbista brasileiro.</p>
        </div>
      </div>
    </div>
  );
};
