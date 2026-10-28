import React, { useState, useEffect } from 'react';
import { useParams, Link } from 'react-router-dom';
import { Button, Card, LevelBadge, AlertBox } from '../components/ui';
import { PROJETOS_EXPLICADOS } from '../content/projetosData';
import { useLocalStorage } from '../hooks/useLocalStorage';

export const ProjetoDetalhe: React.FC = () => {
  const { numero } = useParams<{ numero: string }>();
  const numProj = parseInt(numero || '1', 10);

  const projeto = PROJETOS_EXPLICADOS.find((p) => p.numero === numProj) || PROJETOS_EXPLICADOS[0];
  const [linhaSelecionada, setLinhaSelecionada] = useState<number>(1);
  const [mostrarExplicacaoGeral, setMostrarExplicacaoGeral] = useState<boolean>(false);
  const [copiado, setCopiado] = useState<boolean>(false);
  const [copiouMudancaIdx, setCopiouMudancaIdx] = useState<number | null>(null);

  const [entendidos, setEntendidos] = useLocalStorage<number[]>('codigos-entendidos', []);
  const isEntendido = entendidos.includes(projeto.numero);

  // Linhas de código
  const linhasCodigo = projeto.codigoFonte.split('\n');

  // Inicializar na primeira linha que tenha código real (não comentário de cabeçalho)
  useEffect(() => {
    const primeiraLinhaReal = linhasCodigo.findIndex(
      (l) => l.trim().length > 0 && !l.trim().startsWith('/*') && !l.trim().startsWith('*')
    );
    setLinhaSelecionada(primeiraLinhaReal !== -1 ? primeiraLinhaReal + 1 : 1);
    setMostrarExplicacaoGeral(false);
  }, [projeto.numero]);

  const toggleEntendido = () => {
    setEntendidos((prev) =>
      prev.includes(projeto.numero)
        ? prev.filter((id) => id !== projeto.numero)
        : [...prev, projeto.numero]
    );
  };

  const copiarCodigo = async () => {
    try {
      await navigator.clipboard.writeText(projeto.codigoFonte);
      setCopiado(true);
      setTimeout(() => setCopiado(false), 2000);
    } catch {
      // fallback
    }
  };

  const copiarTrecho = async (texto: string, idx: number) => {
    try {
      await navigator.clipboard.writeText(texto);
      setCopiouMudancaIdx(idx);
      setTimeout(() => setCopiouMudancaIdx(null), 2000);
    } catch {
      // fallback
    }
  };

  const baixarIno = () => {
    const pad = String(projeto.numero).padStart(2, '0');
    const nomeArquivo = `projeto_${pad}_${projeto.slug}.ino`;
    const blob = new Blob([projeto.codigoFonte], { type: 'text/plain;charset=utf-8' });
    const url = URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;
    link.download = nomeArquivo;
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
    URL.revokeObjectURL(url);
  };

  const infoLinha = projeto.linhasExplicadas[linhaSelecionada] || {
    linha: linhaSelecionada,
    codigo: linhasCodigo[linhaSelecionada - 1] || '',
    oQueFaz: 'Linha de código do programa.',
    seVoceMudar: 'Altere com cautela e confira a sintaxe.',
    erroComum: 'Erros de ponto e vírgula ou parênteses não fechados.'
  };

  const projAnterior = projeto.numero > 1 ? projeto.numero - 1 : null;
  const projProximo = projeto.numero < PROJETOS_EXPLICADOS.length ? projeto.numero + 1 : null;

  return (
    <div className="max-w-6xl mx-auto px-4 py-6 pb-24 md:pb-12 space-y-6">
      {/* Top Bar com Navegação entre Projetos */}
      <div className="flex flex-wrap items-center justify-between gap-3 text-xs">
        <Link
          to="/"
          className="text-ink-muted hover:text-ink flex items-center gap-1.5 font-medium transition-colors"
        >
          <svg className="w-4 h-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M10 19l-7-7m0 0l7-7m-7 7h18" />
          </svg>
          Voltar para os 80 Projetos
        </Link>

        <div className="flex items-center gap-2">
          {projAnterior && (
            <Link
              to={`/projeto/${projAnterior}`}
              className="px-2.5 py-1 rounded bg-bench-surface border border-bench-border hover:bg-bench-border text-ink transition-colors"
            >
              ← #{String(projAnterior).padStart(2, '0')} Anterior
            </Link>
          )}
          {projProximo && (
            <Link
              to={`/projeto/${projProximo}`}
              className="px-2.5 py-1 rounded bg-bench-surface border border-bench-border hover:bg-bench-border text-ink transition-colors"
            >
              Próximo #{String(projProximo).padStart(2, '0')} →
            </Link>
          )}
        </div>
      </div>

      {/* Cabeçalho do Projeto */}
      <div className="bg-bench-surface border border-bench-border rounded-2xl p-5 sm:p-6 space-y-4">
        <div className="flex flex-wrap items-start justify-between gap-4">
          <div className="space-y-2 max-w-2xl">
            <div className="flex flex-wrap items-center gap-2">
              <span className="font-mono text-xs font-bold px-2 py-0.5 rounded bg-violet-500/20 text-violet-400 border border-violet-500/30">
                PROJETO #{String(projeto.numero).padStart(2, '0')}
              </span>
              <LevelBadge level={projeto.nivel} />
              <span className="text-xs text-ink-muted">• {projeto.categoria}</span>
            </div>
            <h1 className="text-2xl sm:text-3xl font-extrabold text-ink tracking-tight">
              {projeto.titulo}
            </h1>
            <p className="text-ink-muted text-sm sm:text-base leading-relaxed">
              {projeto.subtitulo}
            </p>
          </div>

          <div className="flex flex-col sm:flex-row items-stretch sm:items-center gap-2 w-full sm:w-auto">
            <button
              onClick={toggleEntendido}
              className={`px-4 py-2.5 rounded-xl border text-xs font-semibold transition-all flex items-center justify-center gap-2 ${
                isEntendido
                  ? 'bg-led-green/20 border-led-green text-led-green'
                  : 'bg-bench border-bench-border text-ink-muted hover:text-ink hover:border-violet-500/50'
              }`}
            >
              {isEntendido ? (
                <>
                  <svg className="w-4 h-4" viewBox="0 0 20 20" fill="currentColor">
                    <path fillRule="evenodd" d="M16.707 5.293a1 1 0 010 1.414l-8 8a1 1 0 01-1.414 0l-4-4a1 1 0 011.414-1.414L8 12.586l7.293-7.293a1 1 0 011.414 0z" clipRule="evenodd" />
                  </svg>
                  Já entendi este código
                </>
              ) : (
                <>
                  <span className="w-3 h-3 rounded-full border border-ink-muted"></span>
                  Marcar como entendido
                </>
              )}
            </button>
          </div>
        </div>

        {/* Toolbar de Ações */}
        <div className="flex flex-wrap items-center gap-2 pt-3 border-t border-bench-border/60">
          <Button
            variant="secondary"
            size="sm"
            onClick={() => setMostrarExplicacaoGeral(!mostrarExplicacaoGeral)}
            className="text-xs"
          >
            <svg className="w-3.5 h-3.5 mr-1.5 text-violet-400" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
            </svg>
            {mostrarExplicacaoGeral ? 'Ocultar visão geral' : 'Explicar o código todo de uma vez'}
          </Button>

          <Button
            variant="secondary"
            size="sm"
            onClick={copiarCodigo}
            className="text-xs"
          >
            <svg className="w-3.5 h-3.5 mr-1.5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M8 5H6a2 2 0 00-2 2v12a2 2 0 002 2h10a2 2 0 002-2v-1M8 5a2 2 0 002 2h2a2 2 0 002-2M8 5a2 2 0 012-2h2a2 2 0 012 2m0 0h2a2 2 0 012 2v3m2 4H10m0 0l3-3m-3 3l3 3" />
            </svg>
            {copiado ? 'Código copiado!' : 'Copiar código'}
          </Button>

          <Button
            variant="ghost"
            size="sm"
            onClick={baixarIno}
            className="text-xs"
          >
            <svg className="w-3.5 h-3.5 mr-1.5 text-led-green" fill="none" viewBox="0 0 24 24" stroke="currentColor">
              <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M4 16v1a3 3 0 003 3h10a3 3 0 003-3v-1m-4-4l-4 4m0 0l-4-4m4 4V4" />
            </svg>
            Baixar arquivo .ino
          </Button>
        </div>

        {/* Modal / Bloco de Explicação Geral Sequencial */}
        {mostrarExplicacaoGeral && (
          <div className="bg-bench border border-violet-500/30 rounded-xl p-4 sm:p-5 space-y-3 animate-fadeIn">
            <div className="flex items-center justify-between">
              <h4 className="font-bold text-sm text-violet-400 flex items-center gap-2">
                <span>📖</span> Visão Geral e Sequência Lógica
              </h4>
              <button
                onClick={() => setMostrarExplicacaoGeral(false)}
                className="text-ink-muted hover:text-ink text-xs p-1"
              >
                ✕ Fechar
              </button>
            </div>
            <div className="text-sm text-ink-muted space-y-2 whitespace-pre-line leading-relaxed font-sans">
              {projeto.explicacaoGeral}
            </div>
          </div>
        )}
      </div>

      {/* O CORAÇÃO DO APP: Visualizador de Código Linha por Linha */}
      <div className="grid grid-cols-1 lg:grid-cols-12 gap-6 items-start">
        {/* Lado Esquerdo: Bloco de Código com Linhas Clicáveis (7 colunas no Desktop) */}
        <div className="lg:col-span-7 bg-bench-surface border border-bench-border rounded-2xl overflow-hidden shadow-lg">
          <div className="bg-bench-border/40 px-4 py-2.5 flex items-center justify-between text-xs border-b border-bench-border">
            <div className="flex items-center gap-2 font-mono text-ink-muted">
              <span className="w-2.5 h-2.5 rounded-full bg-red-500/70"></span>
              <span className="w-2.5 h-2.5 rounded-full bg-yellow-500/70"></span>
              <span className="w-2.5 h-2.5 rounded-full bg-green-500/70"></span>
              <span className="ml-2 font-semibold text-ink">sketch.ino</span>
            </div>
            <span className="text-[11px] text-violet-400 font-medium">
              👉 Toque em qualquer linha para inspecionar
            </span>
          </div>

          <div className="overflow-x-auto max-h-[600px] overflow-y-auto font-mono text-xs sm:text-sm select-text">
            <div className="py-2">
              {linhasCodigo.map((linha, idx) => {
                const num = idx + 1;
                const isSelected = num === linhaSelecionada;
                const isEmpty = linha.trim().length === 0;

                return (
                  <div
                    key={num}
                    onClick={() => setLinhaSelecionada(num)}
                    className={`flex items-start px-3 py-1 cursor-pointer transition-colors ${
                      isSelected
                        ? 'bg-violet-500/20 text-white font-semibold border-l-4 border-violet-400'
                        : 'hover:bg-bench/80 text-ink/90 border-l-4 border-transparent'
                    }`}
                  >
                    <span
                      className={`w-8 sm:w-10 text-right pr-3 sm:pr-4 select-none shrink-0 ${
                        isSelected ? 'text-violet-400 font-bold' : 'text-ink-muted/50'
                      }`}
                    >
                      {num}
                    </span>
                    <pre className="whitespace-pre overflow-x-auto leading-relaxed font-mono">
                      {isEmpty ? ' ' : linha}
                    </pre>
                  </div>
                );
              })}
            </div>
          </div>
        </div>

        {/* Lado Direito: Painel Explicativo da Linha Selecionada (5 colunas no Desktop) */}
        <div className="lg:col-span-5 space-y-4 lg:sticky lg:top-20">
          <Card className="border-violet-500/40 shadow-lg space-y-4">
            <div className="flex items-center justify-between border-b border-bench-border pb-3">
              <div className="flex items-center gap-2">
                <span className="w-2.5 h-2.5 rounded-full bg-violet-400 animate-pulse"></span>
                <span className="text-xs font-mono font-bold text-violet-400 uppercase">
                  Linha {linhaSelecionada} de {linhasCodigo.length}
                </span>
              </div>
              <div className="flex items-center gap-1">
                <button
                  disabled={linhaSelecionada <= 1}
                  onClick={() => setLinhaSelecionada((prev) => Math.max(1, prev - 1))}
                  className="px-2 py-1 rounded bg-bench border border-bench-border text-xs text-ink-muted hover:text-ink disabled:opacity-30 disabled:cursor-not-allowed"
                >
                  ↑ Anterior
                </button>
                <button
                  disabled={linhaSelecionada >= linhasCodigo.length}
                  onClick={() => setLinhaSelecionada((prev) => Math.min(linhasCodigo.length, prev + 1))}
                  className="px-2 py-1 rounded bg-bench border border-bench-border text-xs text-ink-muted hover:text-ink disabled:opacity-30 disabled:cursor-not-allowed"
                >
                  Próxima ↓
                </button>
              </div>
            </div>

            {/* Trecho selecionado */}
            <div className="bg-bench rounded-xl p-3 border border-bench-border font-mono text-xs overflow-x-auto text-ink">
              <code>{infoLinha.codigo || '(linha vazia)'}</code>
            </div>

            {/* O que faz */}
            <div className="space-y-1.5">
              <h4 className="text-xs font-bold uppercase tracking-wider text-ink flex items-center gap-1.5">
                <span className="text-led-green">●</span> O que esta linha faz
              </h4>
              <p className="text-sm text-ink-muted leading-relaxed">
                {infoLinha.oQueFaz}
              </p>
            </div>

            {/* Se você mudar */}
            <div className="space-y-1.5 pt-2 border-t border-bench-border/60">
              <h4 className="text-xs font-bold uppercase tracking-wider text-amber-400 flex items-center gap-1.5">
                <span>⚡</span> Se você mudar este valor
              </h4>
              <p className="text-xs sm:text-sm text-ink-muted leading-relaxed">
                {infoLinha.seVoceMudar}
              </p>
            </div>

            {/* Erros comuns */}
            <div className="space-y-1.5 pt-2 border-t border-bench-border/60">
              <h4 className="text-xs font-bold uppercase tracking-wider text-red-400 flex items-center gap-1.5">
                <span>⚠️</span> Erros comuns nesta linha
              </h4>
              <p className="text-xs sm:text-sm text-ink-muted leading-relaxed">
                {infoLinha.erroComum}
              </p>
            </div>
          </Card>

          <AlertBox type="tip" title="Dica de bancada">
            Quando for alterar código no Arduino, mude sempre <strong>uma linha de cada vez</strong> e envie para a placa para ver o resultado. Se der erro, você saberá exatamente onde foi.
          </AlertBox>
        </div>
      </div>

      {/* SEÇÃO: 3 MUDANÇAS FÁCEIS PARA ESTE PROJETO */}
      <div className="pt-6 space-y-4">
        <div className="space-y-1">
          <div className="inline-flex items-center gap-1.5 text-xs font-bold uppercase tracking-wider text-violet-400">
            <span>🛠️</span> Laboratório Prático
          </div>
          <h2 className="text-2xl font-black text-ink tracking-tight">
            3 Mudanças Fáceis para Testar Agora
          </h2>
          <p className="text-sm text-ink-muted">
            Alterações seguras e testadas para você começar a programar do seu jeito sem medo de estragar o projeto.
          </p>
        </div>

        <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
          {projeto.mudancasFaceis.map((m, idx) => (
            <Card key={idx} className="flex flex-col justify-between space-y-3">
              <div className="space-y-2">
                <div className="flex items-center gap-2">
                  <span className="w-5 h-5 rounded-full bg-violet-500/20 text-violet-400 font-bold text-xs flex items-center justify-center">
                    {idx + 1}
                  </span>
                  <h3 className="font-bold text-sm text-ink leading-snug">
                    {m.titulo}
                  </h3>
                </div>
                <p className="text-xs text-ink-muted leading-relaxed">
                  {m.descricao}
                </p>

                <div className="space-y-1.5 pt-2 text-xs font-mono">
                  <div className="p-2 rounded bg-bench border border-bench-border text-ink-muted line-through opacity-70">
                    {m.antes}
                  </div>
                  <div className="p-2 rounded bg-violet-500/10 border border-violet-500/30 text-violet-400 font-semibold">
                    {m.depois}
                  </div>
                </div>

                <div className="p-2.5 rounded-lg bg-bench-surface border border-bench-border/70 text-xs text-ink-muted">
                  <strong className="text-ink block mb-0.5">Resultado esperado:</strong>
                  {m.resultado}
                </div>
              </div>

              <Button
                variant="secondary"
                size="sm"
                onClick={() => copiarTrecho(m.depois, idx)}
                className="w-full text-xs mt-2"
              >
                {copiouMudancaIdx === idx ? 'Copiado!' : 'Copiar alteração'}
              </Button>
            </Card>
          ))}
        </div>
      </div>
    </div>
  );
};
