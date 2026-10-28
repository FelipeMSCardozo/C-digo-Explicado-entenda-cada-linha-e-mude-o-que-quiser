# Código Explicado: entenda cada linha e mude o que quiser

Web App (PWA) de consulta e aprendizado desenvolvido como bônus oficial para o curso **80 Projetos Arduino**.

## Funcionalidades Principais
1. **Visualizador de Código Linha por Linha**: Cobre todos os 80 projetos do curso. Toque em qualquer linha para entender o que ela faz em português simples, o que acontece ao alterar valores e os erros comuns de sintaxe.
2. **3 Mudanças Fáceis**: Alterações seguras e testadas para cada um dos 80 projetos, com código antes/depois e resultado na bancada explicado.
3. **Dicionário de Funções do Arduino**: As 16 funções e estruturas essenciais (pinMode, digitalWrite, digitalRead, analogRead, analogWrite, delay, millis, Serial.begin, Serial.println, if, for, while, map, byte, int, unsigned long) com sintaxe, exemplo mínimo que compila e links para os projetos que as utilizam.
4. **Trilha "Aprenda a Programar do Zero com o Arduino"**: 6 lições sequenciais e analógicas para iniciantes totais.
5. **Busca Instantânea**: Por comando (ex: `analogRead`), componente (ex: `servo`) ou número do projeto (1 a 80).
6. **Progresso Salvo no Navegador**: Marcação "Já entendi este código" persistida em `localStorage`.
7. **PWA Offline**: Funciona na bancada sem internet após o primeiro acesso.

## Stack Tecnológica
- React 18 + TypeScript (Strict)
- Vite + Tailwind CSS + PWA (workbox)
- Vitest + Playwright E2E
- 100% de compilação Arduino verificada com `arduino-cli` (80/80)
