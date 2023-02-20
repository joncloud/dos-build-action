import { getInput } from './input.js';
import { parseProgram, createMountPaths } from './program.js';
import { join } from 'path';
import { rm, writeFile } from 'fs/promises';
import { invokeDosbox } from './dosbox.js';
import { URL } from 'url';

const __dirname = new URL('.', import.meta.url).pathname;

const run = getInput('run')
  .trim()
  .split(/[\n\r]/g)
  .map(x => x.trim())
  .join('\r\n');
const programs = await Promise.all(
  getInput('programs')
    .trim()
    .split(/\n/g)
    .map(x => parseProgram(
      x.trim(),
      join(__dirname, '..', 'programs'),
    ))
);
const conf = getInput('conf')
  .trim()
  .split(/[\r\n]/g)
  .map(x => x.trim())
  .join('\n');

const mountPaths = await createMountPaths(
  programs,
  join(__dirname, '..', 'working'),
);

await writeFile(
  join(mountPaths.workingDrivesPath, 'c', 'RUN.BAT'),
  run
);

console.log(`running: ${run}`);

let configFile;
if (conf) {
  configFile = join(__dirname, '..', 'working', 'run.conf');
  await writeFile(configFile, conf);
  console.log(`using config: ${conf}`);
}

const commands = mountPaths.drives
  .map(x => `MOUNT ${x.toUpperCase()} ${join(mountPaths.workingDrivesPath, x)}`)
  .concat([
    mountPaths.envPaths,
    'C:\\RUN.BAT'
  ]);

await invokeDosbox({
  commands,
  configFile,
  stdout: process.stdout,
  stderr: process.stderr,
});

await rm(join(__dirname, '..', 'working'), {
  force: true,
  recursive: true,
});
