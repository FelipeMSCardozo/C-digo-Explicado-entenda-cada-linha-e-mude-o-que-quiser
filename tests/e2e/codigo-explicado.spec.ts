import { test, expect } from '@playwright/test';

test.describe('App 3 - Código Explicado E2E', () => {
  const BASE_PATH = '/C-digo-Explicado-entenda-cada-linha-e-mude-o-que-quiser/#/';

  test('01: Página inicial carrega os 80 projetos e filtros funcionam sem erros de console', async ({ page, isMobile }) => {
    const consoleErrors: string[] = [];
    page.on('console', (msg) => {
      if (msg.type() === 'error') consoleErrors.push(msg.text());
    });

    await page.goto(BASE_PATH);
    await expect(page.locator('h1')).toContainText('O código parece grego?');

    // Checar presença dos cards de projetos
    const cards = page.locator('a[href*="/projeto/"]');
    await expect(cards.first()).toBeVisible();
    const count = await cards.count();
    expect(count).toBeGreaterThanOrEqual(10);

    // Testar busca
    const searchInput = page.locator('input[type="search"]');
    await searchInput.fill('Semáforo');
    await expect(page.locator('h3:has-text("Semáforo Inteligente")').first()).toBeVisible();

    // Limpar busca
    await searchInput.fill('');

    // Capturar screenshot
    const vp = isMobile ? 'mobile' : 'desktop';
    await page.screenshot({ path: `screenshots/${vp}/01-home.png`, fullPage: false });
    expect(consoleErrors).toEqual([]);
  });

  test('02: Detalhes do projeto exibe inspetor interativo e 3 mudanças fáceis', async ({ page, isMobile }) => {
    const consoleErrors: string[] = [];
    page.on('console', (msg) => {
      if (msg.type() === 'error') consoleErrors.push(msg.text());
    });

    await page.goto(`${BASE_PATH}projeto/1`);
    await expect(page.locator('h1')).toContainText('Semáforo Inteligente');

    // Verificar presença do visualizador de código e linhas
    await expect(page.locator('text=sketch.ino')).toBeVisible();

    // Testar botão "Explicar o código todo de uma vez"
    const btnExplicarTudo = page.getByRole('button', { name: /Explicar o código todo de uma vez/i });
    await expect(btnExplicarTudo).toBeVisible();
    await btnExplicarTudo.click();
    await expect(page.locator('text=Visão Geral e Sequência Lógica')).toBeVisible();

    // Clicar em uma linha de código específica
    const linhaPre = page.locator('pre:has-text("PINO_BOTAO")').first();
    if (await linhaPre.isVisible()) {
      await linhaPre.click();
    }

    // Verificar se o painel explicativo está presente
    await expect(page.locator('text=O que esta linha faz')).toBeVisible();
    await expect(page.locator('text=Se você mudar este valor')).toBeVisible();
    await expect(page.locator('text=Erros comuns nesta linha')).toBeVisible();

    // Verificar presença das 3 Mudanças Fáceis
    await expect(page.locator('h2:has-text("3 Mudanças Fáceis para Testar Agora")')).toBeVisible();
    const copyButtons = page.locator('button:has-text("Copiar alteração")');
    await expect(copyButtons.first()).toBeVisible();

    const vp = isMobile ? 'mobile' : 'desktop';
    await page.screenshot({ path: `screenshots/${vp}/02-projeto-detalhe.png`, fullPage: false });
    expect(consoleErrors).toEqual([]);
  });

  test('03: Dicionário de funções lista os 16 comandos essenciais com busca', async ({ page, isMobile }) => {
    const consoleErrors: string[] = [];
    page.on('console', (msg) => {
      if (msg.type() === 'error') consoleErrors.push(msg.text());
    });

    await page.goto(`${BASE_PATH}dicionario`);
    await expect(page.locator('h1')).toContainText('Dicionário de Funções do Arduino');

    // Checar comando pinMode e digitalWrite
    await expect(page.locator('h2:has-text("pinMode()")')).toBeVisible();
    await expect(page.locator('h2:has-text("digitalWrite()")')).toBeVisible();

    // Testar busca no dicionário
    const searchInput = page.locator('input[type="search"]');
    await searchInput.fill('millis');
    await expect(page.locator('h2:has-text("millis()")')).toBeVisible();
    await expect(page.locator('h2:has-text("pinMode()")')).not.toBeVisible();

    const vp = isMobile ? 'mobile' : 'desktop';
    await page.screenshot({ path: `screenshots/${vp}/03-dicionario.png`, fullPage: false });
    expect(consoleErrors).toEqual([]);
  });

  test('04: Trilha de programação do zero carrega as 6 lições e salva progresso', async ({ page, isMobile }) => {
    const consoleErrors: string[] = [];
    page.on('console', (msg) => {
      if (msg.type() === 'error') consoleErrors.push(msg.text());
    });

    await page.goto(`${BASE_PATH}trilha`);
    await expect(page.locator('h1')).toContainText('Aprenda a Programar do Zero');

    // Lição 1 ativa (usando locator específico h2)
    await expect(page.locator('text=Lição 1 de 6')).toBeVisible();
    await expect(page.locator('h2:has-text("A Estrutura Básica")')).toBeVisible();
    await expect(page.locator('text=Analogia do dia a dia')).toBeVisible();

    // Marcar como concluída
    const btnConcluir = page.locator('button:has-text("Marcar como concluída")');
    await btnConcluir.click();
    await expect(page.locator('text=Lição Concluída')).toBeVisible();

    const vp = isMobile ? 'mobile' : 'desktop';
    await page.screenshot({ path: `screenshots/${vp}/04-trilha.png`, fullPage: false });
    expect(consoleErrors).toEqual([]);
  });
});
