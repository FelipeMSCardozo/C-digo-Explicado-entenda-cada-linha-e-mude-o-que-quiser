import React, { useState } from 'react';
import {
  Check,
  Copy,
  AlertTriangle,
  Info,
  Flame,
  ChevronDown,
  Search,
  X,
  Sparkles,
} from 'lucide-react';

/* --- 1. BOTÃO (primário, secundário, fantasma) --- */
export interface ButtonProps extends React.ButtonHTMLAttributes<HTMLButtonElement> {
  variant?: 'primary' | 'secondary' | 'ghost' | 'accent';
  size?: 'sm' | 'md' | 'lg';
  icon?: React.ReactNode;
}

export const Button: React.FC<ButtonProps> = ({
  variant = 'primary',
  size = 'md',
  icon,
  children,
  className = '',
  ...props
}) => {
  const base =
    'inline-flex items-center justify-center gap-2 font-semibold rounded-xl transition-all duration-150 select-none active:scale-[0.98] disabled:opacity-50 disabled:pointer-events-none min-h-[44px]';
  const sizes = {
    sm: 'px-3.5 py-2 text-sm min-h-[44px]',
    md: 'px-4 py-2.5 text-base min-h-[44px]',
    lg: 'px-6 py-3.5 text-base min-h-[48px]',
  };
  const variants = {
    primary:
      'bg-led hover:bg-led-dark text-slate-950 shadow-sm shadow-led/20 border border-led/30',
    accent:
      'bg-accent hover:opacity-90 text-white dark:text-slate-950 shadow-sm border border-transparent',
    secondary:
      'bg-bench-elevated hover:bg-bench-border/50 text-ink border border-bench-border',
    ghost:
      'bg-transparent hover:bg-bench-elevated text-ink-muted hover:text-ink border border-transparent',
  };

  return (
    <button
      className={`${base} ${sizes[size]} ${variants[variant]} ${className}`}
      {...props}
    >
      {icon && <span className="shrink-0">{icon}</span>}
      {children}
    </button>
  );
};

/* --- 2. CARD --- */
export interface CardProps extends React.HTMLAttributes<HTMLDivElement> {
  interactive?: boolean;
  highlight?: boolean;
}

export const Card: React.FC<CardProps> = ({
  interactive = false,
  highlight = false,
  children,
  className = '',
  ...props
}) => {
  return (
    <div
      className={`rounded-2xl bg-bench-card border transition-all duration-150 p-5 ${
        highlight
          ? 'border-accent shadow-md'
          : 'border-bench-border'
      } ${
        interactive
          ? 'hover:border-accent/70 hover:shadow-md cursor-pointer'
          : ''
      } ${className}`}
      {...props}
    >
      {children}
    </div>
  );
};

/* --- 3. BADGE DE NÍVEL & STATUS --- */
export type NivelBadge = 'Iniciante' | 'Intermediário' | 'Avançado';

export const LevelBadge: React.FC<{ level: NivelBadge; className?: string }> = ({
  level,
  className = '',
}) => {
  const styles: Record<NivelBadge, string> = {
    Iniciante:
      'bg-emerald-500/15 text-emerald-700 dark:text-emerald-300 border-emerald-500/30',
    Intermediário:
      'bg-amber-500/15 text-amber-800 dark:text-amber-300 border-amber-500/30',
    Avançado:
      'bg-rose-500/15 text-rose-700 dark:text-rose-300 border-rose-500/30',
  };
  return (
    <span
      className={`inline-flex items-center gap-1.5 px-2.5 py-1 rounded-full text-xs font-bold border ${styles[level]} ${className}`}
    >
      <span
        className={`w-2 h-2 rounded-full ${
          level === 'Iniciante'
            ? 'bg-emerald-500'
            : level === 'Intermediário'
            ? 'bg-amber-500'
            : 'bg-rose-500'
        }`}
      />
      {level}
    </span>
  );
};

/* --- 4. LED REWARD INDICATOR (Microinteração de conclusão) --- */
export const LedIndicator: React.FC<{
  on: boolean;
  label?: string;
  size?: 'sm' | 'md';
}> = ({ on, label, size = 'md' }) => {
  const dotSize = size === 'sm' ? 'w-3 h-3' : 'w-4 h-4';
  return (
    <span
      className="inline-flex items-center gap-2 text-xs font-semibold"
      role="status"
      aria-label={label || (on ? 'Concluído (LED aceso)' : 'Pendente (LED apagado)')}
    >
      <span
        className={`${dotSize} rounded-full transition-all duration-200 border ${
          on
            ? 'bg-led border-emerald-300 shadow-[0_0_12px_#22C55E]'
            : 'bg-slate-600/30 border-slate-500/40'
        }`}
      />
      {label && (
        <span className={on ? 'text-emerald-600 dark:text-emerald-400' : 'text-ink-muted'}>
          {label}
        </span>
      )}
    </span>
  );
};

/* --- 5. CAIXA DE ALERTA (dica, atenção, perigo) --- */
export interface AlertBoxProps {
  type: 'tip' | 'warning' | 'danger';
  title: string;
  children: React.ReactNode;
  className?: string;
}

export const AlertBox: React.FC<AlertBoxProps> = ({
  type,
  title,
  children,
  className = '',
}) => {
  const config = {
    tip: {
      bg: 'bg-emerald-500/10 border-emerald-500/30',
      titleColor: 'text-emerald-800 dark:text-emerald-300',
      icon: <Info className="w-5 h-5 text-emerald-600 dark:text-emerald-400 shrink-0 mt-0.5" />,
    },
    warning: {
      bg: 'bg-amber-500/10 border-amber-500/40',
      titleColor: 'text-amber-900 dark:text-amber-300',
      icon: <AlertTriangle className="w-5 h-5 text-amber-600 dark:text-amber-400 shrink-0 mt-0.5" />,
    },
    danger: {
      bg: 'bg-rose-500/10 border-rose-500/40',
      titleColor: 'text-rose-800 dark:text-rose-300',
      icon: <Flame className="w-5 h-5 text-rose-600 dark:text-rose-400 shrink-0 mt-0.5" />,
    },
  }[type];

  return (
    <div
      role={type === 'danger' ? 'alert' : 'note'}
      className={`rounded-xl border p-4 flex items-start gap-3 ${config.bg} ${className}`}
    >
      {config.icon}
      <div className="space-y-1 text-sm leading-relaxed">
        <p className={`font-bold ${config.titleColor}`}>{title}</p>
        <div className="text-ink">{children}</div>
      </div>
    </div>
  );
};

/* --- 6. STEPPER DE PASSOS --- */
export interface StepItem {
  title: string;
  description: string;
  alert?: string;
  tip?: string;
}

export const Stepper: React.FC<{
  steps: StepItem[];
  completedSteps?: number[];
  onToggleStep?: (idx: number) => void;
}> = ({ steps, completedSteps = [], onToggleStep }) => {
  return (
    <ol className="space-y-4">
      {steps.map((step, idx) => {
        const isDone = completedSteps.includes(idx);
        return (
          <li
            key={idx}
            className={`rounded-xl border p-4 transition-all duration-150 ${
              isDone
                ? 'bg-emerald-500/5 border-emerald-500/40'
                : 'bg-bench-card border-bench-border'
            }`}
          >
            <div className="flex items-start gap-3.5">
              {onToggleStep ? (
                <button
                  type="button"
                  onClick={() => onToggleStep(idx)}
                  aria-label={`Marcar passo ${idx + 1}: ${step.title}`}
                  className={`w-11 h-11 rounded-xl font-mono font-bold text-sm flex items-center justify-center shrink-0 transition-all border ${
                    isDone
                      ? 'bg-led text-slate-950 border-emerald-300 shadow-[0_0_10px_rgba(34,197,94,0.4)]'
                      : 'bg-bench-elevated text-ink border-bench-border hover:border-accent'
                  }`}
                >
                  {isDone ? <Check className="w-5 h-5 stroke-[2.5]" /> : idx + 1}
                </button>
              ) : (
                <span className="w-9 h-9 rounded-xl bg-bench-elevated border border-bench-border font-mono font-bold text-sm flex items-center justify-center shrink-0">
                  {idx + 1}
                </span>
              )}
              <div className="flex-1 min-w-0 space-y-1.5">
                <h4 className="font-bold text-base text-ink">{step.title}</h4>
                <p className="text-sm text-ink-muted leading-relaxed">{step.description}</p>
                {step.alert && (
                  <p className="text-xs font-semibold text-amber-700 dark:text-amber-300 bg-amber-500/10 border border-amber-500/30 rounded-lg px-3 py-2">
                    ⚠️ {step.alert}
                  </p>
                )}
                {step.tip && (
                  <p className="text-xs font-medium text-emerald-700 dark:text-emerald-300 bg-emerald-500/10 border border-emerald-500/30 rounded-lg px-3 py-2">
                    💡 {step.tip}
                  </p>
                )}
              </div>
            </div>
          </li>
        );
      })}
    </ol>
  );
};

/* --- 7. CHECKLIST COM MARCAÇÃO --- */
export interface ChecklistItemData {
  id: string;
  label: string;
  detail?: string;
  category?: string;
}

export const Checklist: React.FC<{
  items: ChecklistItemData[];
  checkedIds: string[];
  onToggle: (id: string) => void;
  onReset?: () => void;
}> = ({ items, checkedIds, onToggle, onReset }) => {
  const total = items.length;
  const done = items.filter((i) => checkedIds.includes(i.id)).length;

  return (
    <div className="space-y-4">
      <div className="flex flex-wrap items-center justify-between gap-2 bg-bench-elevated p-3.5 rounded-xl border border-bench-border">
        <div className="flex items-center gap-3">
          <LedIndicator on={done === total && total > 0} />
          <span className="text-sm font-bold">
            {done} de {total} itens conferidos
          </span>
        </div>
        {onReset && done > 0 && (
          <Button variant="ghost" size="sm" onClick={onReset}>
            Desmarcar tudo
          </Button>
        )}
      </div>

      <ul className="space-y-2.5">
        {items.map((item) => {
          const checked = checkedIds.includes(item.id);
          return (
            <li key={item.id}>
              <button
                type="button"
                onClick={() => onToggle(item.id)}
                aria-pressed={checked}
                className={`w-full text-left rounded-xl p-3.5 border transition-all duration-150 flex items-start gap-3.5 min-h-[48px] ${
                  checked
                    ? 'bg-emerald-500/10 border-emerald-500/40'
                    : 'bg-bench-card border-bench-border hover:border-accent/60'
                }`}
              >
                <span
                  className={`w-6 h-6 mt-0.5 rounded-md flex items-center justify-center shrink-0 border transition-colors ${
                    checked
                      ? 'bg-led border-emerald-300 text-slate-950'
                      : 'bg-bench-elevated border-bench-border'
                  }`}
                >
                  {checked && <Check className="w-4 h-4 stroke-[3]" />}
                </span>
                <div className="flex-1">
                  <p
                    className={`font-semibold text-base ${
                      checked ? 'line-through opacity-75' : 'text-ink'
                    }`}
                  >
                    {item.label}
                  </p>
                  {item.detail && (
                    <p className="text-sm text-ink-muted mt-0.5">{item.detail}</p>
                  )}
                </div>
              </button>
            </li>
          );
        })}
      </ul>
    </div>
  );
};

/* --- 8. BLOCO DE CÓDIGO COM COPIAR E NÚMEROS DE LINHA --- */
export const CodeBlock: React.FC<{
  code: string;
  title?: string;
  highlightLines?: number[];
  onLineClick?: (lineNumber: number) => void;
  selectedLine?: number | null;
  lineExplanations?: Record<number, string>;
  explainAll?: boolean;
  copyLabel?: string;
}> = ({
  code,
  title = 'Sketch Arduino (.ino)',
  highlightLines = [],
  onLineClick,
  selectedLine = null,
  lineExplanations,
  explainAll = false,
  copyLabel = 'Copiar código',
}) => {
  const [copied, setCopied] = useState(false);
  const lines = code.replace(/\r\n/g, '\n').split('\n');

  const handleCopy = async () => {
    try {
      await navigator.clipboard.writeText(code);
      setCopied(true);
      setTimeout(() => setCopied(false), 2000);
    } catch {
      const textarea = document.createElement('textarea');
      textarea.value = code;
      document.body.appendChild(textarea);
      textarea.select();
      document.execCommand('copy');
      document.body.removeChild(textarea);
      setCopied(true);
      setTimeout(() => setCopied(false), 2000);
    }
  };

  return (
    <div className="rounded-2xl overflow-hidden border border-bench-border bg-[#0B0F17] text-slate-100">
      <div className="flex flex-wrap items-center justify-between gap-2 px-4 py-2.5 bg-[#161B26] border-b border-slate-800">
        <div className="flex items-center gap-2">
          <span className="w-2.5 h-2.5 rounded-full bg-led" />
          <span className="font-mono text-xs font-semibold text-slate-300">{title}</span>
        </div>
        <button
          type="button"
          onClick={handleCopy}
          className="inline-flex items-center gap-1.5 px-3 py-1.5 rounded-lg text-xs font-semibold bg-slate-800 hover:bg-slate-700 text-slate-100 border border-slate-700 transition-colors min-h-[38px]"
        >
          {copied ? (
            <>
              <Check className="w-4 h-4 text-led" />
              <span className="text-led">Copiado!</span>
            </>
          ) : (
            <>
              <Copy className="w-4 h-4" />
              <span>{copyLabel}</span>
            </>
          )}
        </button>
      </div>
      <div className="overflow-x-auto p-3 font-mono text-xs sm:text-sm leading-relaxed">
        {lines.map((lineText, idx) => {
          const lineNum = idx + 1;
          const isHighlighted = highlightLines.includes(lineNum);
          const isSelected = selectedLine === lineNum;
          const explanation = lineExplanations?.[lineNum];
          const clickable = Boolean(onLineClick && explanation);

          return (
            <React.Fragment key={lineNum}>
              <div
                onClick={() => clickable && onLineClick?.(lineNum)}
                onKeyDown={(e) => {
                  if (clickable && (e.key === 'Enter' || e.key === ' ')) {
                    e.preventDefault();
                    onLineClick?.(lineNum);
                  }
                }}
                tabIndex={clickable ? 0 : undefined}
                role={clickable ? 'button' : undefined}
                aria-label={clickable ? `Linha ${lineNum}: clique para ver explicação` : undefined}
                className={`flex items-start gap-3 px-2 py-0.5 rounded transition-colors ${
                  clickable ? 'cursor-pointer hover:bg-violet-500/20' : ''
                } ${
                  isSelected
                    ? 'bg-violet-500/30 ring-1 ring-violet-400'
                    : isHighlighted
                    ? 'bg-amber-500/20 border-l-2 border-amber-400'
                    : ''
                }`}
              >
                <span className="w-7 select-none text-right text-slate-500 shrink-0">
                  {lineNum}
                </span>
                <pre className="whitespace-pre overflow-x-visible flex-1 text-slate-100">
                  {lineText || ' '}
                </pre>
                {clickable && (
                  <span className="text-[11px] px-1.5 py-0.5 rounded bg-violet-500/20 text-violet-300 shrink-0">
                    explicar
                  </span>
                )}
              </div>
              {(explainAll || isSelected) && explanation && (
                <div className="my-1.5 ml-9 mr-2 p-2.5 rounded-lg bg-violet-950/80 border border-violet-500/40 text-violet-100 font-sans text-xs sm:text-sm flex items-start gap-2">
                  <Sparkles className="w-4 h-4 text-violet-300 shrink-0 mt-0.5" />
                  <div>
                    <span className="font-bold text-violet-300">Linha {lineNum}: </span>
                    {explanation}
                  </div>
                </div>
              )}
            </React.Fragment>
          );
        })}
      </div>
    </div>
  );
};

/* --- 9. TABELA RESPONSIVA --- */
export const ResponsiveTable: React.FC<{
  headers: string[];
  rows: React.ReactNode[][];
  caption?: string;
}> = ({ headers, rows, caption }) => {
  return (
    <div className="overflow-x-auto rounded-xl border border-bench-border bg-bench-card">
      <table className="w-full text-left border-collapse text-sm">
        {caption && <caption className="sr-only">{caption}</caption>}
        <thead>
          <tr className="bg-bench-elevated border-b border-bench-border">
            {headers.map((h, i) => (
              <th key={i} className="py-3 px-4 font-bold text-ink whitespace-nowrap">
                {h}
              </th>
            ))}
          </tr>
        </thead>
        <tbody className="divide-y divide-bench-border">
          {rows.map((row, rIdx) => (
            <tr key={rIdx} className="hover:bg-bench-elevated/50 transition-colors">
              {row.map((cell, cIdx) => (
                <td key={cIdx} className="py-3 px-4 align-top">
                  {cell}
                </td>
              ))}
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

/* --- 10. BUSCA COM RESULTADO INSTANTÂNEO --- */
export const SearchBar: React.FC<{
  value: string;
  onChange: (val: string) => void;
  placeholder?: string;
  label?: string;
}> = ({
  value,
  onChange,
  placeholder = 'Digite para buscar...',
  label = 'Buscar no aplicativo',
}) => {
  return (
    <div className="relative">
      <label className="sr-only">{label}</label>
      <Search className="w-5 h-5 text-ink-muted absolute left-3.5 top-1/2 -translate-y-1/2 pointer-events-none" />
      <input
        type="search"
        value={value}
        onChange={(e) => onChange(e.target.value)}
        placeholder={placeholder}
        aria-label={label}
        className="w-full pl-11 pr-10 py-3 rounded-xl bg-bench-card border border-bench-border text-ink placeholder:text-ink-muted text-base focus:outline-none focus:border-accent min-h-[48px]"
      />
      {value && (
        <button
          type="button"
          onClick={() => onChange('')}
          aria-label="Limpar busca"
          className="absolute right-2.5 top-1/2 -translate-y-1/2 p-1.5 rounded-lg text-ink-muted hover:text-ink"
        >
          <X className="w-4 h-4" />
        </button>
      )}
    </div>
  );
};

/* --- 11. ABAS (TABS) --- */
export interface TabItem {
  id: string;
  label: string;
  badge?: string | number;
}

export const Tabs: React.FC<{
  tabs: TabItem[];
  activeId: string;
  onChange: (id: string) => void;
}> = ({ tabs, activeId, onChange }) => {
  return (
    <div
      role="tablist"
      className="flex overflow-x-auto gap-1.5 p-1.5 rounded-xl bg-bench-elevated border border-bench-border"
    >
      {tabs.map((tab) => {
        const active = tab.id === activeId;
        return (
          <button
            key={tab.id}
            role="tab"
            type="button"
            aria-selected={active}
            onClick={() => onChange(tab.id)}
            className={`px-4 py-2 rounded-lg font-semibold text-sm whitespace-nowrap transition-all min-h-[44px] flex items-center gap-2 ${
              active
                ? 'bg-bench-card text-ink shadow-sm border border-bench-border'
                : 'text-ink-muted hover:text-ink'
            }`}
          >
            <span>{tab.label}</span>
            {tab.badge !== undefined && (
              <span className="px-2 py-0.5 text-xs rounded-full bg-accent-soft text-accent-text font-mono">
                {tab.badge}
              </span>
            )}
          </button>
        );
      })}
    </div>
  );
};

/* --- 12. ACORDEÃO --- */
export const Accordion: React.FC<{
  items: { id: string; title: string; subtitle?: string; content: React.ReactNode }[];
}> = ({ items }) => {
  const [openId, setOpenId] = useState<string | null>(items[0]?.id ?? null);

  return (
    <div className="space-y-2.5">
      {items.map((item) => {
        const isOpen = openId === item.id;
        return (
          <div
            key={item.id}
            className="rounded-xl border border-bench-border bg-bench-card overflow-hidden"
          >
            <button
              type="button"
              aria-expanded={isOpen}
              onClick={() => setOpenId(isOpen ? null : item.id)}
              className="w-full px-4 py-3.5 text-left flex items-center justify-between gap-3 font-bold text-ink hover:bg-bench-elevated/50 transition-colors min-h-[48px]"
            >
              <div>
                <p className="text-base">{item.title}</p>
                {item.subtitle && (
                  <p className="text-xs font-normal text-ink-muted mt-0.5">{item.subtitle}</p>
                )}
              </div>
              <ChevronDown
                className={`w-5 h-5 text-ink-muted shrink-0 transition-transform duration-200 ${
                  isOpen ? 'rotate-180' : ''
                }`}
              />
            </button>
            {isOpen && (
              <div className="px-4 pb-4 pt-2 border-t border-bench-border text-sm text-ink space-y-2">
                {item.content}
              </div>
            )}
          </div>
        );
      })}
    </div>
  );
};

/* --- 13. BARRA DE PROGRESSO --- */
export const ProgressBar: React.FC<{
  value: number;
  max: number;
  label?: string;
}> = ({ value, max, label }) => {
  const pct = max > 0 ? Math.min(100, Math.round((value / max) * 100)) : 0;
  return (
    <div className="space-y-1.5">
      <div className="flex items-center justify-between text-xs font-semibold">
        <span className="text-ink">{label || 'Seu progresso na bancada'}</span>
        <span className="font-mono text-ink-muted">
          {value}/{max} ({pct}%)
        </span>
      </div>
      <div
        className="h-2.5 w-full rounded-full bg-bench-elevated overflow-hidden border border-bench-border"
        role="progressbar"
        aria-valuenow={value}
        aria-valuemin={0}
        aria-valuemax={max}
        aria-label={label || 'Progresso'}
      >
        <div
          className="h-full bg-led transition-all duration-200 rounded-full"
          style={{ width: `${pct}%` }}
        />
      </div>
    </div>
  );
};

/* --- 14. ESTADO VAZIO --- */
export const EmptyState: React.FC<{
  title: string;
  description: string;
  actionLabel?: string;
  onAction?: () => void;
}> = ({ title, description, actionLabel, onAction }) => {
  return (
    <div className="rounded-2xl border border-dashed border-bench-border bg-bench-card/60 p-8 text-center space-y-3">
      <p className="text-lg font-bold text-ink">{title}</p>
      <p className="text-sm text-ink-muted max-w-md mx-auto">{description}</p>
      {actionLabel && onAction && (
        <div className="pt-2">
          <Button variant="secondary" size="sm" onClick={onAction}>
            {actionLabel}
          </Button>
        </div>
      )}
    </div>
  );
};
