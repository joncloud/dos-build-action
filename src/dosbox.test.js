import { mkdtemp, readFile, rm, writeFile } from 'fs/promises';
import { invokeDosbox } from './dosbox.js';
import { describe, before, after, it } from 'mocha';
import { join } from 'path';
import assert from 'assert/strict';
import { PassThrough } from 'stream';

describe('dosbox', function () {
  /** @type {string} */
  let tempPath;
  /** @type {string} */
  let configFile;
  before(async function () {
    tempPath = await mkdtemp('dos-build-action', {
      encoding: 'utf8',
    });
    configFile = join(tempPath, 'dosbox.conf');
    await writeFile(configFile, `[dosbox]
memsize=256

[cpu]
cycles=max
turbo=true`);
  });

  after(async function () {
    if (!tempPath) {
      return;
    }
    await rm(tempPath, {
      force: true,
      recursive: true,
    });
  });

  it('should invoke commands', async function () {
    await invokeDosbox({
      commands: [
        `MOUNT C ${tempPath}`,
        'C:',
        'ECHO FOO BAR > TEMP.TXT',
      ],
      configFile,
      stderr: new PassThrough(),
      stdout: new PassThrough(),
    });

    const expected = 'FOO BAR\r\n';
    const actual = await readFile(join(tempPath, 'TEMP.TXT'), 'utf8');
    assert.strictEqual(actual, expected);
  });
});
