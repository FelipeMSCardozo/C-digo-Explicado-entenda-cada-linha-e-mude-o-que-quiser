import React from 'react';
import { NavLink } from 'react-router-dom';
import { Sun, Moon } from 'lucide-react';
import { useTheme } from '../hooks/useTheme';
import { LedIndicator } from './ui';

interface NavigationProps {
  progressCount?: number;
  totalCount?: number;
}

export const Navigation: React.FC<NavigationProps> = ({ progressCount = 0, totalCount = 80 }) => {
  const { theme, toggleTheme } = useTheme();

  const navItemClass = ({ isActive }: { isActive: boolean }) =>
    `px-3 py-2 rounded-xl text-sm font-semibold transition-all flex items-center gap-1.5 min-h-[44px] ${
      isActive
        ? 'bg-accent/15 text-accent-text dark:text-accent font-bold'
        : 'text-ink-muted hover:text-ink hover:bg-bench-elevated'
    }`;

  const mobileNavItemClass = ({ isActive }: { isActive: boolean }) =>
    `flex flex-col items-center justify-center py-1 px-2 rounded-xl text-[11px] font-semibold transition-all min-h-[48px] min-w-[56px] ${
      isActive ? 'text-accent font-bold scale-105' : 'text-ink-muted hover:text-ink'
    }`;

  return (
    <>
      <header className="sticky top-0 z-40 bg-bench-card/90 backdrop-blur-md border-b border-bench-border px-4 lg:px-8 py-3 transition-colors">
        <div className="max-w-7xl mx-auto flex items-center justify-between gap-4">
          <NavLink to="/" className="flex items-center gap-3 group">
            <div className="w-9 h-9 rounded-xl bg-accent flex items-center justify-center text-white dark:text-slate-950 font-bold shadow-sm shadow-accent/20 group-hover:scale-105 transition-transform">
              <svg className="w-5 h-5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M10 20l4-16m4 4l4 4-4 4M6 16l-4-4 4-4" />
              </svg>
            </div>
            <div>
              <div className="flex items-center gap-2">
                <span className="font-extrabold text-base sm:text-lg text-ink tracking-tight">
                  Código Explicado
                </span>
                <LedIndicator on={progressCount > 0} size="sm" />
              </div>
              <p className="text-[11px] text-ink-muted leading-none hidden sm:block">
                80 Projetos Arduino • Entenda e Modifique
              </p>
            </div>
          </NavLink>

          <nav className="hidden md:flex items-center gap-1">
            <NavLink to="/" end className={navItemClass}>
              <svg className="w-4 h-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M19 11H5m14 0a2 2 0 012 2v6a2 2 0 01-2 2H5a2 2 0 01-2-2v-6a2 2 0 012-2m14 0V9a2 2 0 00-2-2M5 11V9a2 2 0 012-2m0 0V5a2 2 0 012-2h6a2 2 0 012 2v2M7 7h10" />
              </svg>
              Projetos ({totalCount})
            </NavLink>
            <NavLink to="/dicionario" className={navItemClass}>
              <svg className="w-4 h-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M12 6.253v13m0-13C10.832 5.477 9.246 5 7.5 5S4.168 5.477 3 6.253v13C4.168 18.477 5.754 18 7.5 18s3.332.477 4.5 1.253m0-13C13.168 5.477 14.754 5 16.5 5c1.747 0 3.332.477 4.5 1.253v13C19.832 18.477 18.247 18 16.5 18c-1.746 0-3.332.477-4.5 1.253" />
              </svg>
              Dicionário
            </NavLink>
            <NavLink to="/trilha" className={navItemClass}>
              <svg className="w-4 h-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M9 12l2 2 4-4M7.835 4.697a3.42 3.42 0 001.946-.806 3.42 3.42 0 014.438 0 3.42 3.42 0 001.946.806 3.42 3.42 0 013.138 3.138 3.42 3.42 0 00.806 1.946 3.42 3.42 0 010 4.438 3.42 3.42 0 00-.806 1.946 3.42 3.42 0 01-3.138 3.138 3.42 3.42 0 00-1.946.806 3.42 3.42 0 01-4.438 0 3.42 3.42 0 00-1.946-.806 3.42 3.42 0 01-3.138-3.138 3.42 3.42 0 00-.806-1.946 3.42 3.42 0 010-4.438 3.42 3.42 0 00.806-1.946 3.42 3.42 0 013.138-3.138z" />
              </svg>
              Trilha do Zero
            </NavLink>
            <NavLink to="/sobre" className={navItemClass}>
              <svg className="w-4 h-4" fill="none" viewBox="0 0 24 24" stroke="currentColor">
                <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
              </svg>
              Como Usar
            </NavLink>
          </nav>

          <div className="flex items-center gap-3">
            <div className="flex items-center gap-1.5 px-3 py-1.5 rounded-xl bg-bench-elevated border border-bench-border text-xs">
              <span className="font-semibold text-ink">{progressCount}/{totalCount}</span>
              <span className="text-ink-muted">entendidos</span>
            </div>
            <button
              type="button"
              onClick={toggleTheme}
              aria-label="Alternar modo claro e escuro"
              className="p-2.5 rounded-xl border border-bench-border bg-bench-elevated text-ink hover:border-accent transition-all min-h-[44px] min-w-[44px] flex items-center justify-center active:scale-95"
            >
              {theme === 'dark' ? (
                <Sun className="w-5 h-5 text-amber-400" />
              ) : (
                <Moon className="w-5 h-5 text-slate-700" />
              )}
            </button>
          </div>
        </div>
      </header>

      {/* Navegação inferior mobile */}
      <nav aria-label="Navegação móvel" className="md:hidden fixed bottom-0 left-0 right-0 z-40 bg-bench-card/95 backdrop-blur-lg border-t border-bench-border px-2 py-1.5 flex items-center justify-around shadow-lg">
        <NavLink to="/" end className={mobileNavItemClass}>
          <svg className="w-5 h-5 mb-0.5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M19 11H5m14 0a2 2 0 012 2v6a2 2 0 01-2 2H5a2 2 0 01-2-2v-6a2 2 0 012-2m14 0V9a2 2 0 00-2-2M5 11V9a2 2 0 012-2m0 0V5a2 2 0 012-2h6a2 2 0 012 2v2M7 7h10" />
          </svg>
          Projetos
        </NavLink>
        <NavLink to="/dicionario" className={mobileNavItemClass}>
          <svg className="w-5 h-5 mb-0.5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M12 6.253v13m0-13C10.832 5.477 9.246 5 7.5 5S4.168 5.477 3 6.253v13C4.168 18.477 5.754 18 7.5 18s3.332.477 4.5 1.253m0-13C13.168 5.477 14.754 5 16.5 5c1.747 0 3.332.477 4.5 1.253v13C19.832 18.477 18.247 18 16.5 18c-1.746 0-3.332.477-4.5 1.253" />
          </svg>
          Dicionário
        </NavLink>
        <NavLink to="/trilha" className={mobileNavItemClass}>
          <svg className="w-5 h-5 mb-0.5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M9 12l2 2 4-4M7.835 4.697a3.42 3.42 0 001.946-.806 3.42 3.42 0 014.438 0 3.42 3.42 0 001.946.806 3.42 3.42 0 013.138 3.138 3.42 3.42 0 00.806 1.946 3.42 3.42 0 010 4.438 3.42 3.42 0 00-.806 1.946 3.42 3.42 0 01-3.138 3.138 3.42 3.42 0 00-1.946.806 3.42 3.42 0 01-4.438 0 3.42 3.42 0 00-1.946-.806 3.42 3.42 0 01-3.138-3.138z" />
          </svg>
          Trilha
        </NavLink>
        <NavLink to="/sobre" className={mobileNavItemClass}>
          <svg className="w-5 h-5 mb-0.5" fill="none" viewBox="0 0 24 24" stroke="currentColor">
            <path strokeLinecap="round" strokeLinejoin="round" strokeWidth={2} d="M13 16h-1v-4h-1m1-4h.01M21 12a9 9 0 11-18 0 9 9 0 0118 0z" />
          </svg>
          Ajuda
        </NavLink>
      </nav>
    </>
  );
};
