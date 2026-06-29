"""Decode a tokenized GW-BASIC .BAS file to ASCII."""

from __future__ import annotations

import math
import re
import sys
from pathlib import Path

from gwbasic_tokens import tokens


class GWBasicLine:
    def __init__(self, data: bytes, encoding: str, line_start: int):
        self._data = data
        self._encoding = encoding
        self._line_start = line_start
        self._line_num = 0
        self._line_buffer: list[object] = []
        self.is_eof = False
        self._pos = line_start

    def _parse_float32(self, index: int) -> str:
        if self._data[index + 3] == 0:
            return "0"
        exp = self._data[index + 3] - 152
        mantissa = ((self._data[index + 2] | 0x80) << 16) | (self._data[index + 1] << 8) | self._data[index]
        number = -math.ldexp(mantissa, exp) if self._data[index + 2] & 0x80 else math.ldexp(mantissa, exp)
        number_str = self._canonize_number(f"{float('%.6g' % number):g}")
        if "." not in number_str and "E" not in number_str:
            number_str += "!"
        return number_str

    def _parse_float64(self, index: int) -> str:
        if self._data[index + 7] == 0:
            return "0"
        exp = self._data[index + 7] - 184
        mantissa = (
            ((self._data[index + 6] | 0x80) << 48)
            | (self._data[index + 5] << 40)
            | (self._data[index + 4] << 32)
            | (self._data[index + 3] << 24)
            | (self._data[index + 2] << 16)
            | (self._data[index + 1] << 8)
            | self._data[index]
        )
        number_str = self._canonize_number(f"{float('%.16g' % math.ldexp(mantissa, exp)):g}").replace("E", "D")
        if "D" not in number_str:
            number_str += "#"
        return number_str

    def _canonize_number(self, num: str) -> str:
        num = re.sub(r"^([\-])*0\.", r"\1.", num)
        num = re.sub(r"\.0$", "", num)
        return num.upper()

    def get_consumed_byte_count(self) -> int:
        return self._pos - self._line_start

    def _check_boundary(self, required: int) -> None:
        if len(self._data) - self._pos - 1 < required:
            raise Exception(f"Unexpected end of file near line {self._line_num} at {hex(self._pos)}")

    def parse(self) -> None:
        self._check_boundary(2)
        if self._data[self._pos] == 0 and self._data[self._pos + 1] == 0:
            self.is_eof = True
            return
        self._pos += 2
        self._check_boundary(2)
        self._line_num = (self._data[self._pos + 1] << 8) | self._data[self._pos]
        self._pos += 2
        inside_rem = False
        inside_quotes = False
        while self._data[self._pos] != 0:
            self._check_boundary(1)
            code = self._data[self._pos]
            if code == 0x22 and not inside_rem:
                inside_quotes = not inside_quotes
                self._line_buffer.append('"')
                self._pos += 1
            elif (
                code == 0x3A
                and not (inside_quotes or inside_rem)
                and len(self._data) - self._pos - 1 > 2
                and self._data[self._pos + 1] == 0x8F
                and self._data[self._pos + 2] == 0xD9
            ):
                inside_rem = True
                self._line_buffer.append("'")
                self._pos += 3
            elif inside_quotes or inside_rem or (0x20 <= code <= 0x7E):
                self._line_buffer.append(bytes([code]).decode(self._encoding))
                self._pos += 1
            elif code == 0x8F:
                inside_rem = True
                self._line_buffer.append("REM")
                self._pos += 1
            elif code == 0x0B:
                self._check_boundary(2)
                value = (self._data[self._pos + 2] << 8) | self._data[self._pos + 1]
                numerals = []
                while value > 0:
                    numerals.append(str(value & 0x07))
                    value >>= 3
                if not numerals:
                    numerals.append("0")
                numerals.reverse()
                self._line_buffer.append("&O" + "".join(numerals))
                self._pos += 3
            elif code == 0x0C:
                self._check_boundary(2)
                val = hex(self._data[self._pos + 2] << 8 | self._data[self._pos + 1]).replace("0x", "&H")
                self._line_buffer.append(val.upper())
                self._pos += 3
            elif code == 0x0E:
                self._check_boundary(2)
                self._line_buffer.append((self._data[self._pos + 2] << 8) | self._data[self._pos + 1])
                self._pos += 3
            elif code == 0x0F:
                self._line_buffer.append(self._data[self._pos + 1])
                self._pos += 2
            elif 0x11 <= code <= 0x1B:
                self._line_buffer.append(code - 0x11)
                self._pos += 1
            elif code == 0x1C:
                self._check_boundary(2)
                val = ((self._data[self._pos + 2] & 0x7FFF) << 8) | self._data[self._pos + 1]
                self._line_buffer.append(-val if self._data[self._pos + 2] & 0x8000 else val)
                self._pos += 3
            elif code == 0x1D:
                self._check_boundary(4)
                self._line_buffer.append(self._parse_float32(self._pos + 1))
                self._pos += 5
            elif code == 0x1F:
                self._check_boundary(8)
                self._line_buffer.append(self._parse_float64(self._pos + 1))
                self._pos += 9
            elif code in tokens:
                self._line_buffer.append(tokens[code])
                self._pos += 1
            elif ((code << 8) | self._data[self._pos + 1]) in tokens:
                self._line_buffer.append(tokens[(code << 8) | self._data[self._pos + 1]])
                self._pos += 2
            else:
                raise ValueError(f"unexpected token: {code:#04x} at line {self._line_num}")
        self._pos += 1

    def __str__(self) -> str:
        return f"{self._line_num:5d} {''.join(str(x) for x in self._line_buffer)}"


def decode(data: bytes, encoding: str = "cp437") -> str:
    if not data or data[0] != 0xFF:
        raise ValueError("Not a tokenized GW-BASIC program")
    lines: list[str] = []
    pos = 1
    while pos < len(data) - 1:
        line = GWBasicLine(data, encoding, pos)
        line.parse()
        if line.is_eof:
            break
        pos += line.get_consumed_byte_count()
        lines.append(str(line))
    return "\n".join(lines)


def main() -> int:
    path = Path(sys.argv[1] if len(sys.argv) > 1 else "../gwbasic_programs/DICE.BAS")
    print(decode(path.read_bytes()))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
