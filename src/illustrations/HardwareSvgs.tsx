import React, { useState } from 'react';
import { ZoomIn, ZoomOut, RotateCcw } from 'lucide-react';

/* --- WRAPPER COM ZOOM POR TOQUE E CONTROLES ACESSÍVEIS --- */
export const ZoomableSvgWrapper: React.FC<{
  title: string;
  description: string;
  children: React.ReactNode;
}> = ({ title, description, children }) => {
  const [scale, setScale] = useState(1);

  return (
    <div className="rounded-2xl border border-bench-border bg-[#0A0E14] text-slate-100 overflow-hidden">
      <div className="flex flex-wrap items-center justify-between gap-2 px-4 py-2.5 bg-[#121822] border-b border-slate-800">
        <div>
          <p className="text-xs font-bold text-slate-200">{title}</p>
          <p className="text-[11px] text-slate-400">{description}</p>
        </div>
        <div className="flex items-center gap-1">
          <button
            type="button"
            onClick={() => setScale((s) => Math.max(1, Number((s - 0.25).toFixed(2))))}
            aria-label="Diminuir zoom do diagrama"
            className="p-2 rounded-lg bg-slate-800 hover:bg-slate-700 text-slate-200 min-w-[38px] min-h-[38px] flex items-center justify-center"
          >
            <ZoomOut className="w-4 h-4" />
          </button>
          <span className="font-mono text-xs px-2 text-slate-300">{Math.round(scale * 100)}%</span>
          <button
            type="button"
            onClick={() => setScale((s) => Math.min(2, Number((s + 0.25).toFixed(2))))}
            aria-label="Aumentar zoom do diagrama"
            className="p-2 rounded-lg bg-slate-800 hover:bg-slate-700 text-slate-200 min-w-[38px] min-h-[38px] flex items-center justify-center"
          >
            <ZoomIn className="w-4 h-4" />
          </button>
          {scale !== 1 && (
            <button
              type="button"
              onClick={() => setScale(1)}
              aria-label="Restaurar tamanho original"
              className="p-2 rounded-lg bg-slate-800 hover:bg-slate-700 text-slate-200 min-w-[38px] min-h-[38px] flex items-center justify-center"
            >
              <RotateCcw className="w-4 h-4" />
            </button>
          )}
        </div>
      </div>
      <div className="overflow-auto p-4 flex justify-center items-center">
        <div
          style={{
            transform: `scale(${scale})`,
            transformOrigin: 'top center',
            transition: 'transform 150ms ease-out',
          }}
          className="w-full max-w-[680px]"
        >
          {children}
        </div>
      </div>
      <div className="px-4 py-2 bg-[#121822] border-t border-slate-800 flex flex-wrap gap-4 text-[11px] text-slate-300">
        <span className="inline-flex items-center gap-1.5">
          <span className="w-3 h-1.5 rounded bg-[#EF4444]" /> Vermelho = 5V / 3.3V (Positivo)
        </span>
        <span className="inline-flex items-center gap-1.5">
          <span className="w-3 h-1.5 rounded bg-[#64748B]" /> Preto = GND (Negativo comum)
        </span>
        <span className="inline-flex items-center gap-1.5">
          <span className="w-3 h-1.5 rounded bg-[#22C55E]" /> Outras cores = Sinal digital/analógico
        </span>
      </div>
    </div>
  );
};

/* --- 1. RESISTOR COM FAIXAS DE CORES REAIS (4 ou 5 faixas) --- */
export const COLOR_HEX_MAP: Record<string, string> = {
  preto: '#111827',
  marrom: '#78350F',
  vermelho: '#DC2626',
  laranja: '#EA580C',
  amarelo: '#FACC15',
  verde: '#16A34A',
  azul: '#2563EB',
  violeta: '#7C3AED',
  cinza: '#6B7280',
  branco: '#F9FAFB',
  dourado: '#D97706',
  prateado: '#9CA3AF',
};

export const ResistorSvg: React.FC<{
  bands: string[];
  valueLabel?: string;
  className?: string;
}> = ({ bands, valueLabel = 'Resistor', className = '' }) => {
  return (
    <svg
      viewBox="0 0 320 90"
      role="img"
      aria-label={`Ilustração de resistor ${valueLabel} com faixas: ${bands.join(', ')}`}
      className={`w-full max-w-[320px] h-auto ${className}`}
    >
      <title>{`Resistor ${valueLabel} (${bands.join(' - ')})`}</title>
      {/* Terminais metálicos */}
      <line x1="10" y1="45" x2="75" y2="45" stroke="#94A3B8" strokeWidth="6" strokeLinecap="round" />
      <line x1="245" y1="45" x2="310" y2="45" stroke="#94A3B8" strokeWidth="6" strokeLinecap="round" />
      {/* Corpo cerâmico bege/azul */}
      <rect
        x="70"
        y="22"
        width="180"
        height="46"
        rx="18"
        fill="#E2D1B3"
        stroke="#A18862"
        strokeWidth="2.5"
      />
      {/* Faixas de cor */}
      {bands.map((colorName, idx) => {
        const hex = COLOR_HEX_MAP[colorName.toLowerCase()] || '#DC2626';
        const isLast = idx === bands.length - 1;
        const xPos = isLast ? 222 : 95 + idx * 28;
        return (
          <g key={idx}>
            <rect
              x={xPos}
              y="23"
              width="14"
              height="44"
              fill={hex}
              stroke="rgba(0,0,0,0.25)"
              strokeWidth="1"
            />
          </g>
        );
      })}
      {valueLabel && (
        <text
          x="160"
          y="84"
          textAnchor="middle"
          fill="#94A3B8"
          fontSize="12"
          fontFamily="JetBrains Mono, monospace"
          fontWeight="bold"
        >
          {valueLabel}
        </text>
      )}
    </svg>
  );
};

/* --- 2. MULTÍMETRO DIGITAL INTERATIVO --- */
export type MultimeterMode =
  | 'OFF'
  | 'DCV_20'
  | 'ACV_200'
  | 'OHM_2K'
  | 'CONT'
  | 'DIODE'
  | 'DCA_200M'
  | 'DCA_10A';

export const MultimeterInteractiveSvg: React.FC<{
  activeMode: MultimeterMode;
  onSelectMode?: (mode: MultimeterMode) => void;
  displayText?: string;
  use10AJack?: boolean;
}> = ({
  activeMode,
  onSelectMode,
  displayText = '5.01 V',
  use10AJack = false,
}) => {
  const positions: {
    mode: MultimeterMode;
    label: string;
    angle: number;
    lx: number;
    ly: number;
  }[] = [
    { mode: 'OFF', label: 'OFF', angle: -90, lx: 150, ly: 132 },
    { mode: 'DCV_20', label: '20V ⎓', angle: -45, lx: 218, ly: 155 },
    { mode: 'ACV_200', label: 'V~', angle: 0, lx: 242, ly: 205 },
    { mode: 'DCA_200M', label: '200mA', angle: 45, lx: 218, ly: 258 },
    { mode: 'DCA_10A', label: '10A', angle: 90, lx: 150, ly: 282 },
    { mode: 'DIODE', label: '➔| (LED)', angle: 135, lx: 82, ly: 258 },
    { mode: 'CONT', label: '))) Bip', angle: 180, lx: 58, ly: 205 },
    { mode: 'OHM_2K', label: '2k Ω', angle: -135, lx: 82, ly: 155 },
  ];

  const currentPos = positions.find((p) => p.mode === activeMode) || positions[1];

  return (
    <svg
      viewBox="0 0 300 410"
      role="img"
      aria-label={`Multímetro digital na escala ${currentPos.label}, visor mostrando ${displayText}`}
      className="w-full max-w-[300px] mx-auto h-auto select-none"
    >
      <title>{`Multímetro Digital - Escala ${currentPos.label}`}</title>
      {/* Carcaça âmbar de borracha */}
      <rect
        x="22"
        y="12"
        width="256"
        height="386"
        rx="28"
        fill="#F59E0B"
        stroke="#B45309"
        strokeWidth="4"
      />
      {/* Corpo interno cinza escuro */}
      <rect
        x="34"
        y="24"
        width="232"
        height="362"
        rx="20"
        fill="#1E293B"
        stroke="#0F172A"
        strokeWidth="3"
      />
      {/* Visor LCD */}
      <rect
        x="52"
        y="40"
        width="196"
        height="68"
        rx="8"
        fill="#A7F3D0"
        stroke="#064E3B"
        strokeWidth="3"
      />
      <text
        x="62"
        y="58"
        fill="#065F46"
        fontSize="10"
        fontFamily="JetBrains Mono, monospace"
        fontWeight="bold"
      >
        MODO: {currentPos.label}
      </text>
      <text
        x="236"
        y="94"
        textAnchor="end"
        fill="#064E3B"
        fontSize="28"
        fontFamily="JetBrains Mono, monospace"
        fontWeight="bold"
      >
        {displayText}
      </text>

      {/* Roda seletora */}
      <circle cx="150" cy="205" r="56" fill="#0F172A" stroke="#475569" strokeWidth="2.5" />

      {/* Botões de posição ao redor do seletor */}
      {positions.map((pos) => {
        const selected = pos.mode === activeMode;
        return (
          <g
            key={pos.mode}
            onClick={() => onSelectMode?.(pos.mode)}
            className={onSelectMode ? 'cursor-pointer' : ''}
          >
            <rect
              x={pos.lx - 30}
              y={pos.ly - 12}
              width="60"
              height="24"
              rx="6"
              fill={selected ? '#22C55E' : '#334155'}
              stroke={selected ? '#86EFAC' : '#475569'}
              strokeWidth="1.5"
            />
            <text
              x={pos.lx}
              y={pos.ly + 4}
              textAnchor="middle"
              fill={selected ? '#052E16' : '#F8FAFC'}
              fontSize="10"
              fontFamily="JetBrains Mono, monospace"
              fontWeight="bold"
            >
              {pos.label}
            </text>
          </g>
        );
      })}

      {/* Ponteiro giratório central */}
      <g transform={`translate(150, 205) rotate(${currentPos.angle})`}>
        <circle cx="0" cy="0" r="38" fill="#334155" stroke="#94A3B8" strokeWidth="2" />
        <rect x="-5" y="-34" width="10" height="46" rx="4" fill="#F59E0B" />
        <polygon points="0,-36 -6,-24 6,-24" fill="#FEF08A" />
      </g>

      {/* Bornes de entrada das pontas de prova */}
      {/* 10A */}
      <g>
        <circle
          cx="75"
          cy="338"
          r="18"
          fill="#0F172A"
          stroke={use10AJack ? '#EF4444' : '#64748B'}
          strokeWidth={use10AJack ? '5' : '3'}
        />
        <circle cx="75" cy="338" r="8" fill={use10AJack ? '#EF4444' : '#1E293B'} />
        <text
          x="75"
          y="370"
          textAnchor="middle"
          fill="#F87171"
          fontSize="10"
          fontWeight="bold"
          fontFamily="JetBrains Mono, monospace"
        >
          10A MAX
        </text>
      </g>

      {/* COM (Sempre ponta preta) */}
      <g>
        <circle cx="150" cy="338" r="18" fill="#0F172A" stroke="#94A3B8" strokeWidth="4" />
        <circle cx="150" cy="338" r="8" fill="#475569" />
        <text
          x="150"
          y="370"
          textAnchor="middle"
          fill="#E2E8F0"
          fontSize="10"
          fontWeight="bold"
          fontFamily="JetBrains Mono, monospace"
        >
          COM (Preta)
        </text>
      </g>

      {/* VΩmA (Ponta vermelha padrão) */}
      <g>
        <circle
          cx="225"
          cy="338"
          r="18"
          fill="#0F172A"
          stroke={!use10AJack ? '#EF4444' : '#64748B'}
          strokeWidth={!use10AJack ? '5' : '3'}
        />
        <circle cx="225" cy="338" r="8" fill={!use10AJack ? '#EF4444' : '#1E293B'} />
        <text
          x="225"
          y="370"
          textAnchor="middle"
          fill="#FCA5A5"
          fontSize="10"
          fontWeight="bold"
          fontFamily="JetBrains Mono, monospace"
        >
          VΩmA (Verm.)
        </text>
      </g>
    </svg>
  );
};

/* --- 3. DIAGRAMA GENÉRICO ARDUINO UNO + PROTOBOARD + HC-05 COM DIVISOR DE TENSÃO --- */
export const BluetoothDividerDiagramSvg: React.FC<{
  extraComponentLabel?: string;
  extraPin?: string;
}> = ({ extraComponentLabel = 'LED + Resistor 220Ω', extraPin = 'D8' }) => {
  return (
    <ZoomableSvgWrapper
      title="Esquema elétrico: Arduino Uno + Módulo Bluetooth HC-05/HC-06 (com divisor 1kΩ + 2kΩ no RX)"
      description="O pino TX do Arduino sai em 5V. O divisor de tensão (1kΩ + 2kΩ) reduz o sinal para 3,3V no pino RXD do módulo Bluetooth, protegendo o chip."
    >
      <svg
        viewBox="0 0 680 320"
        role="img"
        aria-label="Diagrama de ligação entre Arduino Uno, divisor de tensão de 1k e 2k ohms, módulo Bluetooth HC-05 e periférico"
        className="w-full h-auto"
      >
        <title>Diagrama de ligação Arduino Uno + HC-05 com divisor de tensão 1kΩ e 2kΩ</title>
        {/* Placa Arduino Uno Genérica */}
        <rect
          x="20"
          y="30"
          width="210"
          height="260"
          rx="14"
          fill="#0F3A5D"
          stroke="#38BDF8"
          strokeWidth="2.5"
        />
        <rect x="10" y="65" width="24" height="42" rx="4" fill="#94A3B8" />
        <text x="40" y="62" fill="#E0F2FE" fontSize="14" fontWeight="bold" fontFamily="Inter, sans-serif">
          Placa Uno (5V)
        </text>
        <text x="40" y="80" fill="#7DD3FC" fontSize="11" fontFamily="JetBrains Mono, monospace">
          USB livre (não usa D0/D1)
        </text>

        {/* Pinos Arduino */}
        <g fontFamily="JetBrains Mono, monospace" fontSize="11" fontWeight="bold">
          <rect x="165" y="102" width="55" height="20" rx="4" fill="#1E293B" />
          <text x="192" y="116" textAnchor="middle" fill="#EF4444">5V</text>

          <rect x="165" y="132" width="55" height="20" rx="4" fill="#1E293B" />
          <text x="192" y="146" textAnchor="middle" fill="#CBD5E1">GND</text>

          <rect x="165" y="172" width="55" height="20" rx="4" fill="#1E293B" />
          <text x="192" y="186" textAnchor="middle" fill="#22C55E">D10 (RX)</text>

          <rect x="165" y="202" width="55" height="20" rx="4" fill="#1E293B" />
          <text x="192" y="216" textAnchor="middle" fill="#F59E0B">D11 (TX)</text>

          <rect x="165" y="242" width="55" height="20" rx="4" fill="#1E293B" />
          <text x="192" y="256" textAnchor="middle" fill="#38BDF8">{extraPin}</text>
        </g>

        {/* Bloco Divisor de Tensão na Protoboard */}
        <rect
          x="275"
          y="165"
          width="155"
          height="95"
          rx="10"
          fill="#1E293B"
          stroke="#F59E0B"
          strokeWidth="2"
          strokeDasharray="4 2"
        />
        <text x="352" y="184" textAnchor="middle" fill="#FBBF24" fontSize="11" fontWeight="bold">
          Divisor 5V → 3,3V
        </text>
        <rect x="292" y="198" width="56" height="18" rx="6" fill="#E2D1B3" stroke="#92400E" />
        <text x="320" y="211" textAnchor="middle" fill="#1E293B" fontSize="10" fontWeight="bold">
          1 kΩ (Série)
        </text>
        <rect x="360" y="224" width="56" height="18" rx="6" fill="#E2D1B3" stroke="#92400E" />
        <text x="388" y="237" textAnchor="middle" fill="#1E293B" fontSize="10" fontWeight="bold">
          2 kΩ (ao GND)
        </text>

        {/* Módulo Bluetooth HC-05 / HC-06 */}
        <rect
          x="475"
          y="45"
          width="180"
          height="165"
          rx="12"
          fill="#064E3B"
          stroke="#22C55E"
          strokeWidth="2.5"
        />
        <text x="565" y="72" textAnchor="middle" fill="#ECFDF5" fontSize="14" fontWeight="bold">
          Módulo HC-05 / HC-06
        </text>
        <text x="565" y="90" textAnchor="middle" fill="#A7F3D0" fontSize="10" fontFamily="JetBrains Mono, monospace">
          Sinal lógico 3,3V (9600 baud)
        </text>

        <g fontFamily="JetBrains Mono, monospace" fontSize="11" fontWeight="bold">
          <rect x="485" y="102" width="65" height="20" rx="4" fill="#022C22" />
          <text x="517" y="116" textAnchor="middle" fill="#EF4444">VCC (5V)</text>

          <rect x="485" y="132" width="65" height="20" rx="4" fill="#022C22" />
          <text x="517" y="146" textAnchor="middle" fill="#CBD5E1">GND</text>

          <rect x="485" y="162" width="65" height="20" rx="4" fill="#022C22" />
          <text x="517" y="176" textAnchor="middle" fill="#22C55E">TXD (3.3V)</text>

          <rect x="485" y="186" width="65" height="20" rx="4" fill="#022C22" />
          <text x="517" y="200" textAnchor="middle" fill="#F59E0B">RXD (3.3V)</text>
        </g>

        {/* Periférico do Projeto */}
        <rect
          x="475"
          y="230"
          width="180"
          height="60"
          rx="10"
          fill="#1E1B4B"
          stroke="#818CF8"
          strokeWidth="2"
        />
        <text x="565" y="255" textAnchor="middle" fill="#E0E7FF" fontSize="12" fontWeight="bold">
          {extraComponentLabel}
        </text>
        <text x="565" y="274" textAnchor="middle" fill="#A5B4FC" fontSize="10" fontFamily="JetBrains Mono, monospace">
          Ligado em {extraPin} e GND
        </text>

        {/* Fios de ligação */}
        {/* 5V Vermelho */}
        <path d="M 220 112 L 485 112" stroke="#EF4444" strokeWidth="3.5" fill="none" />
        {/* GND Preto */}
        <path d="M 220 142 L 485 142" stroke="#64748B" strokeWidth="3.5" fill="none" />
        <path d="M 388 242 L 388 142" stroke="#64748B" strokeWidth="2.5" fill="none" />
        {/* TXD do Bluetooth direto para D10 (RX Arduino) */}
        <path d="M 220 182 L 255 182 L 255 156 L 460 156 L 460 172 L 485 172" stroke="#22C55E" strokeWidth="3" fill="none" />
        {/* D11 (TX Arduino 5V) passa pelo divisor 1k/2k antes de entrar no RXD do Bluetooth */}
        <path d="M 220 212 L 292 207" stroke="#F59E0B" strokeWidth="3" fill="none" />
        <path d="M 348 207 L 450 207 L 450 196 L 485 196" stroke="#F59E0B" strokeWidth="3" fill="none" />
        {/* Periférico */}
        <path d="M 220 252 L 260 252 L 260 278 L 475 262" stroke="#38BDF8" strokeWidth="3" fill="none" />
      </svg>
    </ZoomableSvgWrapper>
  );
};
