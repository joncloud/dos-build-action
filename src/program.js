import { join } from 'path';
import { mkdir, rm, readFile, symlink } from 'fs/promises';

/**
 * @param {string} programPath
 * @returns {Promise<{
 *   PATH: string
 * }>}
 */
export async function loadProgramConfig(programPath) {
  const configPath = join(programPath, 'config.json');
  const json = await readFile(configPath, 'utf8');
  const config = JSON.parse(json);
  if (!('PATH' in config)) {
    throw new Error(`${configPath} is missing .PATH`);
  }
  if (typeof config.PATH !== 'string') {
    throw new Error(`${configPath} has invalid .PATH`);
  }
  return config;
}

/**
 * @param {string} text
 * @returns {string}
 */
function parseMountDrive(text) {
  const colonIndex = text.indexOf(':');
  if (colonIndex === -1) {
    throw new Error(`Invalid mount point ${text}`);
  }

  const drive = text.slice(0, colonIndex);
  if (drive.length > 1) {
    throw new Error(`Invalid drive letter: ${text}`);
  }

  return drive;
}

/**
 * @typedef {{
 *   source: string,
 *   mountDrive: string,
 *   destination: string,
 *   config?: {
 *     PATH: string,
 *   },
 * }} ParsedProgram
 * @param {string} text
 * @param {string} programRoot
 * @returns {Promise<ParsedProgram>}
 */
export async function parseProgram(text, programRoot) {
  const colonIndex = text.indexOf(':');
  if (colonIndex === -1) {
    return null;
  }

  const destination = text.slice(colonIndex + 1);
  const mountDrive = parseMountDrive(destination);
  if (text[0] === '$') {
    const programPath = join(
      programRoot,
      text.slice(1, colonIndex)
    );
    const config = await loadProgramConfig(programPath);

    return {
      source: programPath,
      mountDrive,
      destination,
      config,
    };
  }

  if (text[0] !== '/' && text[0] !== '.') {
    const source = join(process.cwd(), text.slice(0, colonIndex));
    return {
      source,
      mountDrive,
      destination,
    };
  }
  return {
    source: text.slice(0, colonIndex),
    mountDrive,
    destination,
  };
}

/**
 * @param {ParsedProgram[]} programs
 * @param {string} workingRoot
 */
export async function createMountPaths(programs, workingRoot) {
  /** @type {Set<string>} */
  const mountedDrives = new Set();

  /** @type {Set<string>} */
  const envPaths = new Set();

  const workingDrives = join(workingRoot, 'drives');

  // Make / clean ./working/drives
  try {
    await rm(workingDrives, { recursive: true, force: true });
  } catch (_err) { }
  await mkdir(workingDrives, { recursive: true });

  for (const program of programs) {
    const mountDrive = program.mountDrive.toLowerCase();
    const currentDrive = join(workingDrives, mountDrive);
    if (!mountedDrives.has(mountDrive)) {
      await mkdir(currentDrive);
      mountedDrives.add(mountDrive);
    }
    const destination = program.destination.slice(3).split('\\');
    const root = destination.slice(0, destination.length - 1);
    const target = join(currentDrive, ...destination);
    if (root.length > 0) {
      await mkdir(join(currentDrive, ...root));
    }

    console.log('symlink', program.source, target);
    await symlink(program.source, target);

    if (program.config && typeof program.config.PATH === 'string') {
      const path = [program.destination, program.config.PATH].join('\\');
      if (!envPaths.has(path)) {
        envPaths.add(path);
      }
    }
  }

  return {
    workingDrivesPath: workingDrives,
    drives: [...mountedDrives],
    envPaths: `SET PATH=${[...envPaths].concat('%PATH%').join(';')}`
  };
}
