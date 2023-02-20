import { mkdtemp, readFile, rm, readdir } from 'fs/promises';
import { describe, before, it } from 'mocha';
import { join } from 'path';
import assert from 'assert/strict';
import { fork } from 'child_process';

describe('e2e', function () {
  before(async function () {
    const files = await readdir('sample');
    for (const file of files) {
      if (/\.(EXE|OBJ|TXT)/g.test(file)) {
        await rm(join('sample', file));
      }
    }
  });

  async function invoke(run, programs) {
    await new Promise((resolve, reject) => {
      const proc = fork('./src/index.js', {
        env: {
          ...process.env,
          INPUT_RUN: run,
          INPUT_PROGRAMS: programs,
        },
        stdio: 'ignore',
      });
      proc.on('exit', (code) => {
        if (code === 0) {
          resolve();
        } else {
          reject(new Error(`Unexpected exit code ${code}`));
        }
      });
    });
    const txt = await readFile('./sample/RUN.TXT', 'utf8');
    return txt;
  }

  describe('turbo-asm', function () {
    describe('2.01', function () {
      it('should compile and run program', async function () {
        const run = `
          C:
          CD SRC
          TASM /zi MAIN.ASM
          TLINK /v MAIN.OBJ
          MAIN.EXE > .\\RUN.TXT
        `;
        const programs = `
          $/turbo-asm/2.01:C:\\TASM
          sample:C:\\SRC
        `;

        const expected = 'Merry Christmas!';
        const actual = await invoke(run, programs);

        assert.strictEqual(actual, expected);
      });
    });
  });

  describe('turbo-c++', function () {
    describe('3.00', function () {
      it('should compile and run program', async function () {
        const run = `
          C:
          CD SRC
          TCC MAIN.C
          MAIN.EXE a b c > .\\RUN.TXT
        `;
        const programs = `
          $/turbo-c++/3.00:C:\\TC
          sample:C:\\SRC
        `;

        const expected = 'C:\\SRC\\MAIN.EXE\r\na\r\nb\r\nc\r\n';
        const actual = await invoke(run, programs);

        assert.strictEqual(actual, expected);
      });
    });
  });
});
