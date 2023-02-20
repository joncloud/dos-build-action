import { spawn } from 'child_process';

/**
 * @param {{
 *   commands: string[],
 *   stdout: NodeJS.WritableStream,
 *   stderr: NodeJS.WritableStream,
 *   configFile?: string,
 * }} options
 */
export async function invokeDosbox(options) {
  const command = 'dosbox-x';
  const args = [
    ...options.commands.flatMap(x => ['-c', `'${x}'`]),
    '-nopromptfolder',
    '-fastlaunch',
    '-silent',
    '-exit',
  ];

  if (options.configFile) {
    args.push('-conf');
    args.push(options.configFile);
  }

  console.log(`spawning ${[command].concat(args).join(' ')}`);

  const proc = spawn(command, args, {
    shell: true,
  });
  const promise = new Promise((resolve, reject) => {
    proc.stderr.pipe(options.stderr);
    proc.stdout.pipe(options.stdout)
    proc.once('exit', (code) => {
      if (code === 0) {
        resolve();
      } else {
        reject(new Error(`Unexpected exit code: ${code}`));
      }
    });
  });

  await promise;
}
