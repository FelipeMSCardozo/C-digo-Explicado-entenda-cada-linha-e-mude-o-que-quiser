import { describe, it, expect } from 'vitest';
import { PROJETOS_EXPLICADOS } from '../../src/content/projetosData';
import { DICIONARIO_FUNCOES } from '../../src/content/dicionario';
import { TRILHA_PROGRAMACAO } from '../../src/content/trilha';

describe('App 3 - Conteúdo dos 80 Projetos Arduino', () => {
  it('deve conter exatamente 80 projetos explicados', () => {
    expect(PROJETOS_EXPLICADOS).toHaveLength(80);
  });

  it('deve ter números de 1 a 80 em sequência sem repetições', () => {
    const numeros = PROJETOS_EXPLICADOS.map((p) => p.numero);
    for (let i = 1; i <= 80; i++) {
      expect(numeros).toContain(i);
    }
    const unicos = new Set(numeros);
    expect(unicos.size).toBe(80);
  });

  it('cada projeto deve ter título, subtítulo, nível válido e componentes', () => {
    const niveisValidos = ['Iniciante', 'Intermediário', 'Avançado'];
    for (const p of PROJETOS_EXPLICADOS) {
      expect(p.titulo.trim().length).toBeGreaterThan(0);
      expect(p.subtitulo.trim().length).toBeGreaterThan(0);
      expect(niveisValidos).toContain(p.nivel);
      expect(p.categoria.trim().length).toBeGreaterThan(0);
      expect(p.componentes.length).toBeGreaterThan(0);
      expect(p.codigoFonte).toContain('void setup()');
      expect(p.codigoFonte).toContain('void loop()');
    }
  });

  it('cada projeto deve ter exatamente 3 mudanças fáceis bem documentadas', () => {
    for (const p of PROJETOS_EXPLICADOS) {
      expect(p.mudancasFaceis).toHaveLength(3);
      for (const m of p.mudancasFaceis) {
        expect(m.titulo.trim().length).toBeGreaterThan(0);
        expect(m.descricao.trim().length).toBeGreaterThan(0);
        expect(m.antes.trim().length).toBeGreaterThan(0);
        expect(m.depois.trim().length).toBeGreaterThan(0);
        expect(m.resultado.trim().length).toBeGreaterThan(0);
      }
    }
  });

  it('as linhas explicadas devem corresponder a linhas reais do código', () => {
    for (const p of PROJETOS_EXPLICADOS) {
      const linhas = p.codigoFonte.split('\n');
      const numLinhas = Object.keys(p.linhasExplicadas).length;
      expect(numLinhas).toBeGreaterThan(0);
      expect(numLinhas).toBe(linhas.length);

      // Checar que a primeira linha tem explicação
      const primeira = p.linhasExplicadas[1];
      expect(primeira).toBeDefined();
      expect(primeira.oQueFaz.length).toBeGreaterThan(0);
      expect(primeira.seVoceMudar.length).toBeGreaterThan(0);
      expect(primeira.erroComum.length).toBeGreaterThan(0);
    }
  });
});

describe('App 3 - Dicionário de Funções', () => {
  it('deve conter as 16 funções essenciais requeridas', () => {
    expect(DICIONARIO_FUNCOES).toHaveLength(16);
    const ids = DICIONARIO_FUNCOES.map((f) => f.id);
    const requeridas = [
      'pinMode', 'digitalWrite', 'digitalRead', 'analogRead', 'analogWrite',
      'delay', 'millis', 'Serial.begin', 'Serial.println',
      'if', 'for', 'while', 'map', 'byte', 'int', 'unsigned long'
    ];
    for (const req of requeridas) {
      expect(ids).toContain(req);
    }
  });

  it('cada função do dicionário deve ter exemplo mínimo que contém código C++', () => {
    for (const f of DICIONARIO_FUNCOES) {
      expect(f.sintaxe.trim().length).toBeGreaterThan(0);
      expect(f.exemplo).toContain('void');
      expect(f.dicaPratica.trim().length).toBeGreaterThan(0);
      expect(f.erroComum.trim().length).toBeGreaterThan(0);
      expect(f.projetosExemplo.length).toBeGreaterThan(0);
    }
  });
});

describe('App 3 - Trilha de Programação do Zero', () => {
  it('deve conter as 6 lições obrigatórias em ordem', () => {
    expect(TRILHA_PROGRAMACAO).toHaveLength(6);
    for (let i = 0; i < 6; i++) {
      expect(TRILHA_PROGRAMACAO[i].id).toBe(i + 1);
      expect(TRILHA_PROGRAMACAO[i].analogia.length).toBeGreaterThan(20);
      expect(TRILHA_PROGRAMACAO[i].explicacao.length).toBeGreaterThan(20);
      expect(TRILHA_PROGRAMACAO[i].exemploCodigo).toContain('void');
      expect(TRILHA_PROGRAMACAO[i].testePratico.length).toBeGreaterThan(10);
      expect(TRILHA_PROGRAMACAO[i].dicaOuro.length).toBeGreaterThan(10);
    }
  });
});
