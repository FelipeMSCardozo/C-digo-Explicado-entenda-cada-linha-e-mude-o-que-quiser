import fs from 'node:fs';
import path from 'node:path';
import { execSync } from 'node:child_process';

const SKETCHES_DIR = path.resolve(process.cwd(), 'sketches');
const ARDUINO_CLI = process.env.ARDUINO_CLI || (fs.existsSync('/Users/felipecardozo/.local/bin/arduino-cli') ? '/Users/felipecardozo/.local/bin/arduino-cli' : 'arduino-cli');

console.log(`[VERIFICAÇÃO ARDUINO-CLI] Compilando sketches com ${ARDUINO_CLI} para placa arduino:avr:uno...\n`);

const entries = fs.readdirSync(SKETCHES_DIR).filter((d) => fs.statSync(path.join(SKETCHES_DIR, d)).isDirectory());
let passed = 0;
let failed = 0;
const errors: string[] = [];

for (const entry of entries) {
  const sketchFiles = fs.readdirSync(path.join(SKETCHES_DIR, entry)).filter((f) => f.endsWith('.ino'));
  if (sketchFiles.length === 0) continue;

  const inoPath = path.join(SKETCHES_DIR, entry, sketchFiles[0]);
  process.stdout.write(`Compilando ${entry}... `);

  try {
    execSync(`"${ARDUINO_CLI}" compile --fqbn arduino:avr:uno "${inoPath}"`, {
      encoding: 'utf8',
      stdio: ['ignore', 'pipe', 'pipe'],
    });
    console.log('\x1b[32mOK\x1b[0m');
    passed++;
  } catch (err: any) {
    console.log('\x1b[31mFALHA\x1b[0m');
    failed++;
    errors.push(`Erro em ${entry}: ${err.stderr || err.stdout || err.message}`);
  }
}

console.log(`\n========================================`);
console.log(`Total compilado: ${passed}/${passed + failed}`);
if (failed > 0) {
  console.error(`\x1b[31m[ERRO] ${failed} sketches falharam na compilação!\x1b[0m`);
  errors.forEach((e) => console.error(e));
  process.exit(1);
} else {
  console.log(`\x1b[32m[SUCESSO] Todos os 80 projetos Arduino compilam perfeitamente sem erros!\x1b[0m`);
}
