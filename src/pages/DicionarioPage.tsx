import React, { useState, useMemo } from 'react';
import { Link } from 'react-router-dom';
import { Card, AlertBox, CodeBlock } from '../components/ui';
import { DICIONARIO_FUNCOES, FuncaoDicionario } from '../content/dicionario';

export const DicionarioPage: React.FC = () => {
  const [busca, setBusca] = useState('');
  const [categoriaAtiva, setCategoriaAtiva] = useState<string>('Todas');

  const categorias = [
    'Todas',
    'Entrada/Saída',
    'Tempo',
    'Comunicação Serial',
    'Controle de Fluxo',
    'Matemática',
    'Tipos de Dados'
  ];

  const funcoesFiltradas = useMemo(() => {
    const q = busca.toLowerCase().trim();
    return DICIONARIO_FUNCOES.filter((f) => {
      if (categoriaAtiva !== 'Todas' && f.categoria !== categoriaAtiva) return false;
      if (!q) return true;
      return (
        f.nome.toLowerCase().includes(q) ||
        f.descricao.toLowerCase().includes(q) ||
        f.sintaxe.toLowerCase().includes(q) ||
        f.categoria.toLowerCase().includes(q)
      );
    });
  }, [busca, categoriaAtiva]);

  return (
    <div className="max-w-5xl mx-auto px-4 py-8 pb-24 md:pb-12 space-y-8">
      {/* Header */}
      <div className="text-center space-y-3 max-w-2xl mx-auto">
        <div className="inline-flex items-center gap-1.5 px-3 py-1 rounded-full bg-violet-500/10 border border-violet-500/30 text-violet-400 text-xs font-semibold uppercase tracking-wider">
          <span>📚</span> Consulta Rápida de Bancada
        </div>
        <h1 className="text-3xl sm:text-4xl font-black text-ink tracking-tight">
          Dicionário de Funções do Arduino
        </h1>
        <p className="text-ink-muted text-sm sm:text-base">
          As 16 funções e estruturas essenciais do Arduino explicadas em bom português, com sintaxe simples, exemplos que compilam e links para os projetos que as utilizam.
        </p>
      </div>

      {/* Busca e Abas de Categoria */}
      <div className="space-y-4">
        <div className="relative">
          <svg
            className="w-5 h-5 absolute left-3.5 top-1/2 -translate-y-1/2 text-ink-muted"
            fill="none"
            viewBox="0 0 24 24"
            stroke="currentColor"
          >
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M21 21l-6-6m2-5a7 7 0 11-14 0 7 7 0 0114 0z" />
          </svg>
          <input
            type="search"
            value={busca}
            onChange={(e) => setBusca(e.target.value)}
            placeholder="Busque por comando (ex: pinMode, analogRead, millis)..."
            className="w-full bg-bench-surface border border-bench-border rounded-xl pl-11 pr-4 py-3 text-sm text-ink placeholder:text-ink-muted focus:outline-none focus:ring-2 focus:ring-violet-500/50"
          />
        </div>

        <div className="flex flex-wrap items-center gap-1.5 pb-2 border-b border-bench-border/50 text-xs overflow-x-auto">
          {categorias.map((cat) => (
            <button
              key={cat}
              onClick={() => setCategoriaAtiva(cat)}
              className={`px-3 py-1.5 rounded-lg font-medium whitespace-nowrap transition-colors ${
                categoriaAtiva === cat
                  ? 'bg-violet-500 text-white shadow-sm'
                  : 'bg-bench-surface text-ink-muted hover:text-ink hover:bg-bench-border'
              }`}
            >
              {cat}
            </button>
          ))}
        </div>
      </div>

      {/* Lista de Funções */}
      <div className="space-y-6">
        {funcoesFiltradas.map((fn: FuncaoDicionario) => (
          <Card key={fn.id} className="space-y-4 border-bench-border/80 hover:border-violet-500/40 transition-colors">
            <div className="flex flex-wrap items-center justify-between gap-2 border-b border-bench-border/50 pb-3">
              <div className="flex items-center gap-2">
                <h2 className="text-xl font-mono font-bold text-violet-400">
                  {fn.nome}
                </h2>
                <span className="px-2.5 py-0.5 rounded-full text-xs font-semibold bg-accent/15 text-accent-text dark:text-accent border border-accent/30">{fn.categoria}</span>
              </div>
            </div>

            <p className="text-sm text-ink leading-relaxed font-sans">
              {fn.descricao}
            </p>

            {/* Bloco de Sintaxe */}
            <div className="space-y-1">
              <span className="text-[11px] font-bold uppercase tracking-wider text-ink-muted">
                Como escrever no código (sintaxe):
              </span>
              <div className="p-2.5 rounded-lg bg-bench border border-bench-border font-mono text-xs text-ink overflow-x-auto">
                <code>{fn.sintaxe}</code>
              </div>
            </div>

            {/* Código de exemplo mínimo */}
            <div className="space-y-1.5">
              <span className="text-[11px] font-bold uppercase tracking-wider text-ink-muted">
                Exemplo mínimo que compila:
              </span>
              <CodeBlock code={fn.exemplo} title={fn.nome} />
            </div>

            {/* Dica e Erro Comum */}
            <div className="grid grid-cols-1 md:grid-cols-2 gap-3 pt-2 text-xs">
              <div className="p-3 rounded-xl bg-bench border border-bench-border space-y-1">
                <span className="font-bold text-led-green flex items-center gap-1">
                  💡 Dica de ouro
                </span>
                <p className="text-ink-muted leading-relaxed">{fn.dicaPratica}</p>
              </div>

              <div className="p-3 rounded-xl bg-bench border border-red-500/20 space-y-1">
                <span className="font-bold text-red-400 flex items-center gap-1">
                  ⚠️ Erro mais comum
                </span>
                <p className="text-ink-muted leading-relaxed">{fn.erroComum}</p>
              </div>
            </div>

            {/* Projetos da lista dos 80 que usam essa função */}
            {fn.projetosExemplo && fn.projetosExemplo.length > 0 && (
              <div className="pt-2 flex flex-wrap items-center gap-2 text-xs text-ink-muted">
                <span className="font-semibold text-ink">Veja funcionando nos projetos:</span>
                {fn.projetosExemplo.map((pNum) => (
                  <Link
                    key={pNum}
                    to={`/projeto/${pNum}`}
                    className="px-2 py-0.5 rounded bg-violet-500/10 text-violet-400 border border-violet-500/30 hover:bg-violet-500/20 font-mono transition-colors"
                  >
                    #{String(pNum).padStart(2, '0')}
                  </Link>
                ))}
              </div>
            )}
          </Card>
        ))}

        {funcoesFiltradas.length === 0 && (
          <div className="text-center py-12 bg-bench-surface rounded-2xl border border-bench-border">
            <p className="text-ink font-semibold">Nenhuma função encontrada</p>
            <p className="text-ink-muted text-xs mt-1">
              Tente buscar por termos como &quot;digitalWrite&quot; ou &quot;millis&quot;.
            </p>
          </div>
        )}
      </div>

      <AlertBox type="tip" title="Precisa de mais detalhes?">
        Você pode tocar em qualquer comando dentro da tela de detalhes dos 80 projetos para ver exatamente o que ele está fazendo naquele circuito específico.
      </AlertBox>
    </div>
  );
};
