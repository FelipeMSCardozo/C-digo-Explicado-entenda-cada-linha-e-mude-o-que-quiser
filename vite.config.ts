import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import { VitePWA } from 'vite-plugin-pwa';

export default defineConfig({
  base: '/C-digo-Explicado-entenda-cada-linha-e-mude-o-que-quiser/',
  plugins: [
    react(),
    VitePWA({
      registerType: 'autoUpdate',
      includeAssets: ['favicon.svg'],
      manifest: {
        name: 'Código Explicado: entenda cada linha e mude o que quiser',
        short_name: 'Código Explicado',
        description: 'Visualizador interativo de código linha por linha, 3 mudanças fáceis e dicionário de funções do Arduino.',
        theme_color: '#0D1117',
        background_color: '#0D1117',
        display: 'standalone',
        start_url: '/C-digo-Explicado-entenda-cada-linha-e-mude-o-que-quiser/#/',
        icons: [
          {
            src: 'favicon.svg',
            sizes: 'any',
            type: 'image/svg+xml',
            purpose: 'any maskable',
          },
        ],
      },
      workbox: {
        globPatterns: ['**/*.{js,css,html,svg,png,woff2,woff}'],
        maximumFileSizeToCacheInBytes: 6 * 1024 * 1024,
      },
    }),
  ],
  build: {
    chunkSizeWarningLimit: 4000,
  },
  server: {
    port: 5175,
  },
});
