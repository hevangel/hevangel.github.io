/* eslint-disable no-bitwise */
/** Decode tokenized GW-BASIC .BAS files to ASCII listing text. */

const GW_TOKENS = {
  0x81: 'END',
  0x82: 'FOR',
  0x83: 'NEXT',
  0x84: 'DATA',
  0x85: 'INPUT',
  0x86: 'DIM',
  0x87: 'READ',
  0x88: 'LET',
  0x89: 'GOTO',
  0x8a: 'RUN',
  0x8b: 'IF',
  0x8c: 'RESTORE',
  0x8d: 'GOSUB',
  0x8e: 'RETURN',
  0x8f: 'REM',
  0x90: 'STOP',
  0x91: 'PRINT',
  0x92: 'CLEAR',
  0x93: 'LIST',
  0x94: 'NEW',
  0x95: 'ON',
  0x96: 'WAIT',
  0x97: 'DEF',
  0x98: 'POKE',
  0x99: 'CONT',
  0x9c: 'OUT',
  0x9d: 'LPRINT',
  0x9e: 'LLIST',
  0xa0: 'WIDTH',
  0xa1: 'ELSE',
  0xa2: 'TRON',
  0xa3: 'TROFF',
  0xa4: 'SWAP',
  0xa5: 'ERASE',
  0xa6: 'EDIT',
  0xa7: 'ERROR',
  0xa8: 'RESUME',
  0xa9: 'DELETE',
  0xaa: 'AUTO',
  0xab: 'RENUM',
  0xac: 'DEFSTR',
  0xad: 'DEFINT',
  0xae: 'DEFSNG',
  0xaf: 'DEFDBL',
  0xb0: 'LINE',
  0xb1: 'WHILE',
  0xb2: 'WEND',
  0xb3: 'CALL',
  0xb7: 'WRITE',
  0xb8: 'OPTION',
  0xb9: 'RANDOMIZE',
  0xba: 'OPEN',
  0xbb: 'CLOSE',
  0xbc: 'LOAD',
  0xbd: 'MERGE',
  0xbe: 'SAVE',
  0xbf: 'COLOR',
  0xc0: 'CLS',
  0xc1: 'MOTOR',
  0xc2: 'BSAVE',
  0xc3: 'BLOAD',
  0xc4: 'SOUND',
  0xc5: 'BEEP',
  0xc6: 'PSET',
  0xc7: 'PRESET',
  0xc8: 'SCREEN',
  0xc9: 'KEY',
  0xca: 'LOCATE',
  0xcc: 'TO',
  0xcd: 'THEN',
  0xce: 'TAB(',
  0xcf: 'STEP',
  0xd0: 'USR',
  0xd1: 'FN',
  0xd2: 'SPC(',
  0xd3: 'NOT',
  0xd4: 'ERL',
  0xd5: 'ERR',
  0xd6: 'STRING$',
  0xd7: 'USING',
  0xd8: 'INSTR',
  0xd9: "'",
  0xda: 'VARPTR',
  0xdb: 'CSRLIN',
  0xdc: 'POINT',
  0xdd: 'OFF',
  0xde: 'INKEY$',
  0xe6: '>',
  0xe7: '=',
  0xe8: '<',
  0xe9: '+',
  0xea: '-',
  0xeb: '*',
  0xec: '/',
  0xed: '^',
  0xee: 'AND',
  0xef: 'OR',
  0xf0: 'XOR',
  0xf1: 'EQV',
  0xf2: 'IMP',
  0xf3: 'MOD',
  0xf4: '\\',
  0xfd81: 'CVI',
  0xfd82: 'CVS',
  0xfd83: 'CVD',
  0xfd84: 'MKI$',
  0xfd85: 'MKS$',
  0xfd86: 'MKD$',
  0xfd8b: 'EXTERR',
  0xfe81: 'FILES',
  0xfe82: 'FIELD',
  0xfe83: 'SYSTEM',
  0xfe84: 'NAME',
  0xfe85: 'LSET',
  0xfe86: 'RSET',
  0xfe87: 'KILL',
  0xfe88: 'PUT',
  0xfe89: 'GET',
  0xfe8a: 'RESET',
  0xfe8b: 'COMMON',
  0xfe8c: 'CHAIN',
  0xfe8d: 'DATE$',
  0xfe8e: 'TIME$',
  0xfe8f: 'PAINT',
  0xfe90: 'COM',
  0xfe91: 'CIRCLE',
  0xfe92: 'DRAW',
  0xfe93: 'PLAY',
  0xfe94: 'TIMER',
  0xfe95: 'ERDEV',
  0xfe96: 'IOCTL',
  0xfe97: 'CHDIR',
  0xfe98: 'MKDIR',
  0xfe99: 'RMDIR',
  0xfe9a: 'SHELL',
  0xfe9b: 'ENVIRON',
  0xfe9c: 'VIEW',
  0xfe9d: 'WINDOW',
  0xfe9e: 'PMAP',
  0xfe9f: 'PALETTE',
  0xfea0: 'LCOPY',
  0xfea1: 'CALLS',
  0xfea5: 'PCOPY',
  0xfea6: 'TERM',
  0xfea7: 'LOCK',
  0xfea8: 'UNLOCK',
  0xff81: 'LEFT$',
  0xff82: 'RIGHT$',
  0xff83: 'MID$',
  0xff84: 'SGN',
  0xff85: 'INT',
  0xff86: 'ABS',
  0xff87: 'SQR',
  0xff88: 'RND',
  0xff89: 'SIN',
  0xff8a: 'LOG',
  0xff8b: 'EXP',
  0xff8c: 'COS',
  0xff8d: 'TAN',
  0xff8e: 'ATN',
  0xff8f: 'FRE',
  0xff90: 'INP',
  0xff91: 'POS',
  0xff92: 'LEN',
  0xff93: 'STR$',
  0xff94: 'VAL',
  0xff95: 'ASC',
  0xff96: 'CHR$',
  0xff97: 'PEEK',
  0xff98: 'SPACE$',
  0xff99: 'OCT$',
  0xff9a: 'HEX$',
  0xff9b: 'LPOS',
  0xff9c: 'CINT',
  0xff9d: 'CSNG',
  0xff9e: 'CDBL',
  0xff9f: 'FIX',
  0xffa0: 'PEN',
  0xffa1: 'STICK',
  0xffa2: 'STRIG',
  0xffa3: 'EOF',
  0xffa4: 'LOC',
  0xffa5: 'LOF',
};

function canonizeNumber(num) {
  return num
    .replace(/^(-?)0\./, '$1.')
    .replace(/\.0$/, '')
    .toUpperCase();
}

function parseFloat32(data, index) {
  if (data[index + 3] === 0) return '0';
  const exp = data[index + 3] - 152;
  const mantissa = ((data[index + 2] | 0x80) << 16) | (data[index + 1] << 8) | data[index];
  const signed = data[index + 2] & 0x80;
  const number = (signed ? -1 : 1) * mantissa * 2 ** exp;
  let numberStr = canonizeNumber(String(Number(number.toPrecision(6))));
  if (!numberStr.includes('.') && !numberStr.includes('E')) numberStr += '!';
  return numberStr;
}

function parseFloat64(data, index) {
  if (data[index + 7] === 0) return '0';
  const exp = data[index + 7] - 184;
  const mantissa =
    ((BigInt(data[index + 6]) | 0x80n) << 48n) |
    (BigInt(data[index + 5]) << 40n) |
    (BigInt(data[index + 4]) << 32n) |
    (BigInt(data[index + 3]) << 24n) |
    (BigInt(data[index + 2]) << 16n) |
    (BigInt(data[index + 1]) << 8n) |
    BigInt(data[index]);
  const number = Number(mantissa) * 2 ** exp;
  let numberStr = canonizeNumber(String(Number(number.toPrecision(16)))).replace(/E/g, 'D');
  if (!numberStr.includes('D')) numberStr += '#';
  return numberStr;
}

function decodeGwBasicLine(data, lineStart) {
  let pos = lineStart;
  if (pos + 2 > data.length - 1) {
    return { eof: true, consumed: 0, lineNum: 0, text: '' };
  }
  if (data[pos] === 0 && data[pos + 1] === 0) {
    return { eof: true, consumed: 2, lineNum: 0, text: '' };
  }

  const start = pos;
  pos += 2;
  const lineNum = (data[pos + 1] << 8) | data[pos];
  pos += 2;

  const parts = [];
  let insideRem = false;
  let insideQuotes = false;

  while (data[pos] !== 0) {
    const code = data[pos];
    if (code === 0x22 && !insideRem) {
      insideQuotes = !insideQuotes;
      parts.push('"');
      pos += 1;
    } else if (
      code === 0x3a &&
      !insideQuotes &&
      !insideRem &&
      data[pos + 1] === 0x8f &&
      data[pos + 2] === 0xd9
    ) {
      insideRem = true;
      parts.push("'");
      pos += 3;
    } else if (insideQuotes || insideRem || (code >= 0x20 && code <= 0x7e)) {
      parts.push(String.fromCharCode(code));
      pos += 1;
    } else if (code === 0x8f) {
      insideRem = true;
      parts.push('REM');
      pos += 1;
    } else if (code === 0x0b) {
      let value = (data[pos + 2] << 8) | data[pos + 1];
      const numerals = [];
      while (value > 0) {
        numerals.push(String(value & 0x07));
        value >>= 3;
      }
      if (numerals.length === 0) numerals.push('0');
      parts.push(`&O${numerals.reverse().join('')}`);
      pos += 3;
    } else if (code === 0x0c) {
      const val = (data[pos + 2] << 8) | data[pos + 1];
      parts.push(`&H${val.toString(16).toUpperCase()}`);
      pos += 3;
    } else if (code === 0x0e) {
      parts.push(String((data[pos + 2] << 8) | data[pos + 1]));
      pos += 3;
    } else if (code === 0x0f) {
      parts.push(String(data[pos + 1]));
      pos += 2;
    } else if (code >= 0x11 && code <= 0x1b) {
      parts.push(String(code - 0x11));
      pos += 1;
    } else if (code === 0x1c) {
      let val = ((data[pos + 2] & 0x7fff) << 8) | data[pos + 1];
      if (data[pos + 2] & 0x8000) val = -val;
      parts.push(String(val));
      pos += 3;
    } else if (code === 0x1d) {
      parts.push(parseFloat32(data, pos + 1));
      pos += 5;
    } else if (code === 0x1f) {
      parts.push(parseFloat64(data, pos + 1));
      pos += 9;
    } else if (GW_TOKENS[code]) {
      parts.push(GW_TOKENS[code]);
      pos += 1;
    } else {
      const pair = (code << 8) | data[pos + 1];
      if (GW_TOKENS[pair]) {
        parts.push(GW_TOKENS[pair]);
        pos += 2;
      } else {
        throw new Error(`Unexpected token 0x${code.toString(16)} at line ${lineNum}`);
      }
    }
  }

  pos += 1;
  return {
    eof: false,
    consumed: pos - start,
    lineNum,
    text: parts.join(''),
  };
}

function decodeTokenizedGwBasic(bytes) {
  if (!bytes.length || bytes[0] !== 0xff) {
    throw new Error('Not a tokenized GW-BASIC program.');
  }
  const lines = [];
  let pos = 1;
  while (pos < bytes.length - 1) {
    const line = decodeGwBasicLine(bytes, pos);
    if (line.eof) break;
    pos += line.consumed;
    lines.push({ lineNum: line.lineNum, text: line.text });
  }
  return lines;
}

function isTokenizedGwBasic(bytes) {
  return bytes.length > 0 && bytes[0] === 0xff;
}

function bytesToGwBasicListing(bytes) {
  if (isTokenizedGwBasic(bytes)) {
    return decodeTokenizedGwBasic(bytes);
  }

  const text = new TextDecoder('iso-8859-1').decode(bytes).replace(/\r\n/g, '\n').replace(/\r/g, '\n');
  const lines = [];
  for (const raw of text.split('\n')) {
    if (!raw.trim()) continue;
    const match = raw.match(/^\s*(\d+)\s+(.*)$/);
    if (match) {
      lines.push({ lineNum: Number(match[1]), text: match[2] });
    } else {
      lines.push({ lineNum: null, text: raw.trimEnd() });
    }
  }
  return lines;
}

window.GwBasicSource = {
  bytesToGwBasicListing,
  isTokenizedGwBasic,
  decodeTokenizedGwBasic,
};
