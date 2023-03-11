/**
 * @param {string} name
 * @param {{ required: boolean }} [options={ required: true }]
 */
export function getInput(name, { required } = { required: true }) {
  const envName = `INPUT_${name.replace(/ /g, '_').toUpperCase()}`;
  const value = process.env[envName];
  if (!value && required) {
    throw new Error(`Input required and not supplied: ${name} (${envName})`);
  }
  return value;
}
