/** @type {import('tailwindcss').Config} */
export default {
  darkMode: 'class',
  content: ['./index.html', './src/**/*.{ts,tsx}'],
  theme: {
    extend: {
      colors: {
        bench: {
          DEFAULT: 'var(--bg-bench)',
          card: 'var(--bg-card)',
          elevated: 'var(--bg-elevated)',
          border: 'var(--border-line)',
        },
        ink: {
          DEFAULT: 'var(--text-ink)',
          muted: 'var(--text-muted)',
        },
        led: {
          DEFAULT: '#22C55E',
          dark: '#16A34A',
          glow: 'rgba(34, 197, 94, 0.25)',
        },
        accent: {
          DEFAULT: 'var(--color-accent)',
          soft: 'var(--color-accent-soft)',
          text: 'var(--color-accent-text)',
        },
        wire: {
          red: '#EF4444',
          black: '#1F2937',
          yellow: '#F59E0B',
          blue: '#3B82F6',
          green: '#22C55E',
          purple: '#A855F7',
          cyan: '#06B6D4',
        },
      },
      fontFamily: {
        sans: ['"Inter"', 'system-ui', '-apple-system', 'sans-serif'],
        mono: ['"JetBrains Mono"', 'ui-monospace', 'SFMono-Regular', 'monospace'],
      },
    },
  },
  plugins: [],
};
