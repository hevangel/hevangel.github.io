/** Build a tokenized GW-BASIC launcher: CHAIN, SYSTEM. */

function quoteBasName(name) {
  const bytes = new Uint8Array(name.length + 2);
  bytes[0] = 0x22;
  for (let i = 0; i < name.length; i += 1) {
    bytes[i + 1] = name.charCodeAt(i);
  }
  bytes[name.length + 1] = 0x22;
  return bytes;
}

function encodeLauncherBas(basName) {
  let loadName = basName;
  if (!loadName.includes('.')) {
    loadName = `${loadName}.BAS`;
  }

  // LOAD replaces the launcher in memory, so RUN/SYSTEM never execute. CHAIN
  // runs the program and resumes at the next line when it ENDs.
  const lines = [
    [10, Uint8Array.from([0xFE, 0x8C, ...quoteBasName(loadName)])],
    [20, Uint8Array.from([0xFE, 0x83])],
  ];

  const data = [0xFF];
  const headers = [];

  for (const [lineNum, tokens] of lines) {
    headers.push(data.length);
    data.push(0, 0, lineNum & 0xFF, (lineNum >> 8) & 0xFF);
    data.push(...tokens);
    data.push(0);
  }

  for (let i = 0; i < headers.length; i += 1) {
    const nextPos = i + 1 < headers.length ? headers[i + 1] : data.length;
    data[headers[i]] = nextPos & 0xFF;
    data[headers[i + 1]] = (nextPos >> 8) & 0xFF;
  }

  data.push(0, 0, 0x1A);
  return Uint8Array.from(data);
}

window.GwBasicLauncher = {
  encodeLauncherBas,
};
