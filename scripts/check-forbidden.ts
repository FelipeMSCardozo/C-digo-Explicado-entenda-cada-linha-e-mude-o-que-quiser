import fs from 'node:fs';
import path from 'node:path';

const FORBIDDEN_WORDS = [
  'lorem',
  'todo',
  'tbd',
  'placeholder',
  'exemplo aqui',
  'xxx',
];

const IGNORE_DIRS = new Set([
  'node_modules',
  '.git',
  'dist',
  'build',
  'coverage',
]);

const EXTENSIONS = new Set(['.ts', '.tsx', '.js', '.jsx', '.html', '.md', '.json']);

function scanDir(dir: string, errors: string[]) {
  const entries = fs.readdirSync(dir, { withFileTypes: true });

  for (const entry of entries) {
    const fullPath = path.join(dir, entry.name);

    if (entry.isDirectory()) {
      if (!IGNORE_DIRS.has(entry.name)) {
        scanDir(fullPath, errors);
      }
    } else if (entry.isFile()) {
      const ext = path.extname(entry.name).toLowerCase();
      if (
        entry.name === 'check-forbidden.ts' ||
        entry.name === 'package-lock.json' ||
        !EXTENSIONS.has(ext)
      ) {
        continue;
      }

      const content = fs.readFileSync(fullPath, 'utf8');
      const lines = content.split('\n');

      lines.forEach((line, index) => {
        const lower = line.toLowerCase();
        for (const word of FORBIDDEN_WORDS) {
          if (lower.includes(word)) {
            // Distinguish HTML/React attribute name `placeholder=` or `placeholder?:` or `placeholder:` css from literal placeholder text
            if (word === 'placeholder') {
              // Ignore input attribute declaration and css pseudo-class
              const cleaned = line
                .replace(/placeholder\??:\s*string/g, '')
                .replace(/placeholder\s*=\s*['"][^'"]*['"]/g, '')
                .replace(/placeholder=\{[^}]+\}/g, '')
                .replace(/placeholder:[a-z0-9-]+/gi, '')
                .replace(/inputProps/g, '').replace(/getByPlaceholder/g, '');
              if (!/placeholder/i.test(cleaned)) {
                continue;
              }
            }

            if (word === 'xxx' && !/\bxxx\b/i.test(line)) {
              continue;
            }

            if (word === 'todo') {
              // Portuguese "todo" vs uppercase TODO or TODO:
              if (!/\bTODO\b/.test(line) && !/\bTODO:/.test(line) && !/\/\/.*\bTODO\b/.test(line)) {
                continue;
              }
            }

            errors.push(`${fullPath}:${index + 1} contains prohibited token [${word}]: "${line.trim()}"`);
          }
        }
      });
    }
  }
}

const errors: string[] = [];
scanDir(process.cwd(), errors);

if (errors.length > 0) {
  console.error('\x1b[31m[ERRO DE QUALIDADE] Encontrados textos de exemplo/proibidos no código:\x1b[0m');
  errors.forEach((err) => console.error(`  -> ${err}`));
  process.exit(1);
} else {
  console.log('\x1b[32m[SUCESSO] Nenhum texto proibido encontrado no projeto!\x1b[0m');
}
