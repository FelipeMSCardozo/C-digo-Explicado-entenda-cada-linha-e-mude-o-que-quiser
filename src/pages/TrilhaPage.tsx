import React, { useState } from 'react';
import { Card, Button, Stepper, AlertBox, CodeBlock, LedIndicator } from '../components/ui';
import { TRILHA_PROGRAMACAO, LicaoTrilha } from '../content/trilha';
import { useLocalStorage } from '../hooks/useLocalStorage';

export const TrilhaPage: React.FC = () => {
  const [licaoAtivaIdx, setLicaoAtivaIdx] = useState<number>(0);
  const [concluidas, setConcluidas] = useLocalStorage<number[]>('codigos-trilha-concluidos', []);

  const licao: LicaoTrilha = TRILHA_PROGRAMACAO[licaoAtivaIdx];
  const isConcluida = concluidas.includes(licao.id);

  const toggleConcluida = () => {
    setConcluidas((prev) =>
      prev.includes(licao.id) ? prev.filter((id) => id !== licao.id) : [...prev, licao.id]
    );
  };

  const steps = TRILHA_PROGRAMACAO.map((l) => ({
    title: l.titulo,
    description: l.subtitulo
  }));

  return (
    <div className="max-w-4xl mx-auto px-4 py-8 pb-24 md:pb-12 space-y-8">
      {/* Header */}
      <div className="text-center space-y-3 max-w-2xl mx-auto">
        <div className="inline-flex items-center gap-1.5 px-3 py-1 rounded-full bg-accent/10 border border-accent/30 text-accent-text dark:text-accent text-xs font-semibold uppercase tracking-wider">
          <span>🚀</span> Trilha Passo a Passo
        </div>
        <h1 className="text-3xl sm:text-4xl font-black text-ink tracking-tight">
          Aprenda a Programar do Zero com o Arduino
        </h1>
        <p className="text-ink-muted text-sm sm:text-base">
          6 lições curtas e sequenciais para quem nunca escreveu uma linha de código. Sem jargões complicados, com analogias do dia a dia e pequenos testes na sua bancada.
        </p>

        <div className="flex items-center justify-center gap-2 pt-2 text-xs">
          <LedIndicator on={concluidas.length > 0} label={`${concluidas.length} de 6 lições concluídas`} />
        </div>
      </div>

      {/* Stepper de Navegação */}
      <div className="bg-bench-card border border-bench-border rounded-2xl p-4 sm:p-6">
        <Stepper
          steps={steps}
          completedSteps={concluidas.map((id) => id - 1)}
          onToggleStep={(idx: number) => {
            setLicaoAtivaIdx(idx);
          }}
        />
      </div>

      {/* Conteúdo da Lição Ativa */}
      <Card className="space-y-6 border-accent/30 shadow-lg">
        <div className="flex flex-wrap items-start justify-between gap-4 border-b border-bench-border pb-4">
          <div className="space-y-1">
            <span className="font-mono text-xs font-bold text-accent-text dark:text-accent uppercase tracking-wider">
              Lição {licao.id} de 6
            </span>
            <h2 className="text-2xl font-black text-ink tracking-tight">
              {licao.titulo}
            </h2>
            <p className="text-sm text-ink-muted">
              {licao.subtitulo}
            </p>
          </div>

          <button
            onClick={toggleConcluida}
            className={`px-4 py-2 rounded-xl border text-xs font-semibold transition-all flex items-center gap-2 ${
              isConcluida
                ? 'bg-led/20 border-emerald-500 text-emerald-600 dark:text-emerald-400 font-bold'
                : 'bg-bench-elevated border-bench-border text-ink-muted hover:text-ink hover:border-accent'
            }`}
          >
            {isConcluida ? (
              <>
                <svg className="w-4 h-4" viewBox="0 0 20 20" fill="currentColor">
                  <path fillRule="evenodd" d="M16.707 5.293a1 1 0 010 1.414l-8 8a1 1 0 01-1.414 0l-4-4a1 1 0 011.414-1.414L8 12.586l7.293-7.293a1 1 0 011.414 0z" clipRule="evenodd" />
                </svg>
                Lição Concluída
              </>
            ) : (
              <>
                <span className="w-3 h-3 rounded-full border border-ink-muted"></span>
                Marcar como concluída
              </>
            )}
          </button>
        </div>

        {/* Analogia do Dia a Dia */}
        <div className="p-4 rounded-xl bg-accent/10 border border-accent/30 space-y-1.5">
          <span className="text-xs font-bold uppercase tracking-wider text-accent-text dark:text-accent flex items-center gap-1.5">
            <span>☕</span> Analogia do dia a dia
          </span>
          <p className="text-sm text-ink leading-relaxed">
            {licao.analogia}
          </p>
        </div>

        {/* Explicação Teórica Simples */}
        <div className="space-y-2">
          <h3 className="text-xs font-bold uppercase tracking-wider text-ink flex items-center gap-1.5">
            <span className="text-led">●</span> Como funciona na prática
          </h3>
          <div className="text-sm text-ink-muted whitespace-pre-line leading-relaxed font-sans">
            {licao.explicacao}
          </div>
        </div>

        {/* Exemplo de Código Mínimo */}
        <div className="space-y-2">
          <h3 className="text-xs font-bold uppercase tracking-wider text-ink flex items-center gap-1.5">
            <span>💻</span> Código de exemplo (teste na sua placa)
          </h3>
          <CodeBlock code={licao.exemploCodigo} title="Exemplo Mínimo" />
        </div>

        {/* Teste Prático de Bancada */}
        <div className="p-4 rounded-xl bg-bench-elevated border border-bench-border space-y-1.5">
          <span className="text-xs font-bold uppercase tracking-wider text-amber-500 flex items-center gap-1.5">
            <span>🧪</span> O que você vai ver na placa
          </span>
          <p className="text-sm text-ink-muted leading-relaxed">
            {licao.testePratico}
          </p>
        </div>

        {/* Dica de Ouro */}
        <AlertBox type="tip" title="Dica de Ouro">
          {licao.dicaOuro}
        </AlertBox>

        {/* Navegação Entre Lições */}
        <div className="flex items-center justify-between pt-4 border-t border-bench-border">
          <Button
            variant="ghost"
            size="sm"
            disabled={licaoAtivaIdx <= 0}
            onClick={() => setLicaoAtivaIdx((prev) => Math.max(0, prev - 1))}
          >
            ← Lição Anterior
          </Button>

          {licaoAtivaIdx < TRILHA_PROGRAMACAO.length - 1 ? (
            <Button
              variant="primary"
              size="sm"
              onClick={() => {
                if (!isConcluida) toggleConcluida();
                setLicaoAtivaIdx((prev) => Math.min(TRILHA_PROGRAMACAO.length - 1, prev + 1));
              }}
            >
              Próxima Lição →
            </Button>
          ) : (
            <Button
              variant="primary"
              size="sm"
              onClick={() => {
                if (!isConcluida) toggleConcluida();
              }}
            >
              🎉 Concluir Trilha!
            </Button>
          )}
        </div>
      </Card>
    </div>
  );
};
