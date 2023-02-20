/**
 * @param {string} name
 */
export function getInput(name) {
  const envName = `INPUT_${name.replace(/ /g, '_').toUpperCase()}`;
  const value = process.env[envName];
  if (!value) {
    throw new Error(`Input required and not supplied: ${name}`);
  }
  return value;
}
