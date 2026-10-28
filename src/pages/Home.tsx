import React, { useState, useMemo } from 'react';
import { Link } from 'react-router-dom';
import { Card, LevelBadge, ProgressBar } from '../components/ui';
import { PROJETOS_EXPLICADOS } from '../content/projetosData';
import { useLocalStorage } from '../hooks/useLocalStorage';

export const Home: React.FC = () => {
  const [busca, setBusca] = useState('');
  const [nivelFiltro, setNivelFiltro] = useState<string>('Todos');
  const [statusFiltro, setStatusFiltro] = useState<'todos' | 'pendentes' | 'entendidos'>('todos');
  const [entendidos, setEntendidos] = useLocalStorage<number[]>('codigos-entendidos', []);

  const total = PROJETOS_EXPLICADOS.length;
  const concluidosCount = entendidos.length;

  const toggleEntendido = (num: number, e: React.MouseEvent) => {
    e.preventDefault();
    e.stopPropagation();
    setEntendidos((prev) =>
      prev.includes(num) ? prev.filter((id) => id !== num) : [...prev, num]
    );
  };

  const projetosFiltrados = useMemo(() => {
    const q = busca.toLowerCase().trim();
    return PROJETOS_EXPLICADOS.filter((p) => {
      // Filtro de nível
      if (nivelFiltro !== 'Todos' && p.nivel !== nivelFiltro) return false;

      // Filtro de status
      const isEntendido = entendidos.includes(p.numero);
      if (statusFiltro === 'pendentes' && isEntendido) return false;
      if (statusFiltro === 'entendidos' && !isEntendido) return false;

      // Busca textual
      if (!q) return true;

      // Busca por número exato (#1, 1, p01, etc.)
      const numMatch = q.match(/\d+/);
      if (numMatch && parseInt(numMatch[0], 10) === p.numero) return true;

      // Busca em título, subtítulo, categoria
      if (p.titulo.toLowerCase().includes(q)) return true;
      if (p.subtitulo.toLowerCase().includes(q)) return true;
      if (p.categoria.toLowerCase().includes(q)) return true;

      // Busca nos componentes
      if (p.componentes.some((c) => c.toLowerCase().includes(q))) return true;

      // Busca no código-fonte por comando (ex: analogRead, millis)
      if (p.codigoFonte.toLowerCase().includes(q)) return true;

      return false;
    });
  }, [busca, nivelFiltro, statusFiltro, entendidos]);

  return (
    <div className="max-w-6xl mx-auto px-4 py-8 pb-24 md:pb-12 space-y-8">
      {/* Hero Section */}
      <div className="text-center space-y-4 max-w-3xl mx-auto">
        <div className="inline-flex items-center gap-2 px-3 py-1 rounded-full bg-accent/10 border border-accent/30 text-accent-text dark:text-accent text-xs font-semibold uppercase tracking-wider">
          <span className="w-2 h-2 rounded-full bg-accent animate-pulse"></span>
          80 Projetos Arduino • Código 100% Explicado
        </div>
        <h1 className="text-3xl sm:text-4xl lg:text-5xl font-black text-ink tracking-tight">
          O código parece grego?{' '}
          <span className="text-accent-text dark:text-accent">Entenda cada linha.</span>
        </h1>
        <p className="text-ink-muted text-base sm:text-lg">
          Toque em qualquer comando para ver o que ele faz em português simples, descubra o que
          acontece ao mudar valores e teste 3 alterações seguras em cada projeto.
        </p>

        {/* Barra de Progresso Geral */}
        <div className="pt-2 max-w-xl mx-auto">
          <ProgressBar value={concluidosCount} max={total} label="Códigos compreendidos na bancada" />
        </div>
      </div>

      {/* Painel de Busca e Filtros */}
      <div className="bg-bench-card border border-bench-border rounded-2xl p-4 sm:p-6 space-y-4">
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
            placeholder="Busque por comando (ex: analogRead), peça (ex: servo), título ou nº do projeto..."
            className="w-full bg-bench-elevated border border-bench-border rounded-xl pl-11 pr-10 py-3 text-sm text-ink placeholder:text-ink-muted focus:outline-none focus:ring-2 focus:ring-accent/50"
          />
          {busca && (
            <button
              onClick={() => setBusca('')}
              className="absolute right-3 top-1/2 -translate-y-1/2 text-ink-muted hover:text-ink text-xs p-1"
            >
              Limpar
            </button>
          )}
        </div>

        {/* Filtros em linha */}
        <div className="flex flex-wrap items-center justify-between gap-3 pt-2 border-t border-bench-border/50 text-xs">
          <div className="flex flex-wrap items-center gap-1.5">
            <span className="text-ink-muted font-medium mr-1">Nível:</span>
            {['Todos', 'Iniciante', 'Intermediário', 'Avançado'].map((lvl) => (
              <button
                key={lvl}
                onClick={() => setNivelFiltro(lvl)}
                className={`px-3 py-1.5 rounded-lg font-medium transition-colors ${
                  nivelFiltro === lvl
                    ? 'bg-accent text-white dark:text-slate-950 font-bold shadow-sm'
                    : 'bg-bench-elevated text-ink-muted hover:text-ink hover:bg-bench-border'
                }`}
              >
                {lvl}
              </button>
            ))}
          </div>

          <div className="flex items-center gap-1.5">
            <span className="text-ink-muted font-medium mr-1">Status:</span>
            <button
              onClick={() => setStatusFiltro('todos')}
              className={`px-2.5 py-1 rounded-md font-medium ${
                statusFiltro === 'todos' ? 'bg-accent/20 text-accent-text dark:text-accent font-semibold' : 'text-ink-muted hover:text-ink'
              }`}
            >
              Todos
            </button>
            <button
              onClick={() => setStatusFiltro('pendentes')}
              className={`px-2.5 py-1 rounded-md font-medium ${
                statusFiltro === 'pendentes' ? 'bg-accent/20 text-accent-text dark:text-accent font-semibold' : 'text-ink-muted hover:text-ink'
              }`}
            >
              Pendentes ({total - concluidosCount})
            </button>
            <button
              onClick={() => setStatusFiltro('entendidos')}
              className={`px-2.5 py-1 rounded-md font-medium ${
                statusFiltro === 'entendidos' ? 'bg-accent/20 text-accent-text dark:text-accent font-semibold' : 'text-ink-muted hover:text-ink'
              }`}
            >
              Entendidos ({concluidosCount})
            </button>
          </div>
        </div>
      </div>

      {/* Lista / Grid de Projetos */}
      {projetosFiltrados.length === 0 ? (
        <div className="text-center py-16 bg-bench-card rounded-2xl border border-bench-border space-y-3">
          <svg className="w-12 h-12 mx-auto text-ink-muted" fill="none" viewBox="0 0 24 24" stroke="currentColor">
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={1.5} d="M9.172 16.172a4 4 0 015.656 0M9 10h.01M15 10h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
          </svg>
          <p className="text-ink font-semibold">Nenhum projeto encontrado</p>
          <p className="text-ink-muted text-sm max-w-md mx-auto">
            Não encontramos nenhum projeto com os termos buscados. Tente procurar por comando básico como &quot;pinMode&quot; ou limpe os filtros.
          </p>
          <button
            onClick={() => {
              setBusca('');
              setNivelFiltro('Todos');
              setStatusFiltro('todos');
            }}
            className="px-4 py-2 bg-accent/20 text-accent-text dark:text-accent rounded-lg text-xs font-semibold hover:bg-accent/30 transition-colors"
          >
            Redefinir filtros
          </button>
        </div>
      ) : (
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4">
          {projetosFiltrados.map((p) => {
            const isEntendido = entendidos.includes(p.numero);
            return (
              <Link
                key={p.numero}
                to={`/projeto/${p.numero}`}
                className="group block"
              >
                <Card
                  interactive
                  highlight={isEntendido}
                  className="h-full flex flex-col justify-between"
                >
                  <div className="space-y-3">
                    <div className="flex items-center justify-between gap-2">
                      <div className="flex items-center gap-2">
                        <span className="font-mono text-xs font-bold px-2 py-0.5 rounded bg-bench-elevated text-ink">
                          #{String(p.numero).padStart(2, '0')}
                        </span>
                        <LevelBadge level={p.nivel} />
                      </div>

                      <button
                        type="button"
                        onClick={(e) => toggleEntendido(p.numero, e)}
                        title={isEntendido ? 'Marcado como entendido' : 'Marcar como entendido'}
                        className={`p-1.5 rounded-lg border text-xs font-medium transition-colors ${
                          isEntendido
                            ? 'bg-led/20 border-emerald-500 text-emerald-600 dark:text-emerald-400 font-bold'
                            : 'bg-bench-elevated border-bench-border text-ink-muted hover:text-ink'
                        }`}
                      >
                        {isEntendido ? (
                          <span className="flex items-center gap-1">
                            <svg className="w-3.5 h-3.5" viewBox="0 0 20 20" fill="currentColor">
                              <path fillRule="evenodd" d="M16.707 5.293a1 1 0 010 1.414l-8 8a1 1 0 01-1.414 0l-4-4a1 1 0 011.414-1.414L8 12.586l7.293-7.293a1 1 0 011.414 0z" clipRule="evenodd" />
                            </svg>
                            Entendido
                          </span>
                        ) : (
                          <span className="flex items-center gap-1 text-[11px]">
                            <span className="w-2 h-2 rounded-full border border-ink-muted"></span>
                            Pendente
                          </span>
                        )}
                      </button>
                    </div>

                    <div>
                      <h3 className="font-bold text-base text-ink group-hover:text-accent-text dark:group-hover:text-accent transition-colors line-clamp-1">
                        {p.titulo}
                      </h3>
                      <p className="text-ink-muted text-xs line-clamp-2 mt-1">
                        {p.subtitulo}
                      </p>
                    </div>
                  </div>

                  <div className="pt-4 mt-3 border-t border-bench-border/50 flex items-center justify-between text-xs text-ink-muted">
                    <span className="truncate max-w-[180px]">{p.categoria}</span>
                    <span className="text-accent-text dark:text-accent font-semibold group-hover:translate-x-0.5 transition-transform flex items-center gap-1">
                      Ver código →
                    </span>
                  </div>
                </Card>
              </Link>
            );
          })}
        </div>
      )}
    </div>
  );
};
