"""Build a tokenized GW-BASIC launcher: CHAIN, SYSTEM."""

from __future__ import annotations

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from detokenize_bas import decode


def quote(text: str) -> bytes:
    return bytes([0x22]) + text.encode("ascii") + bytes([0x22])


def encode_program(lines: list[tuple[int, bytes]]) -> bytes:
    data = bytearray([0xFF])
    headers: list[int] = []
    for line_num, tokens in lines:
        headers.append(len(data))
        data.extend((0, 0, line_num & 0xFF, (line_num >> 8) & 0xFF))
        data.extend(tokens)
        data.append(0)
    for index, header_pos in enumerate(headers):
        next_pos = headers[index + 1] if index + 1 < len(headers) else len(data)
        data[header_pos] = next_pos & 0xFF
        data[header_pos + 1] = (next_pos >> 8) & 0xFF
    data.extend((0, 0))
    if not data or data[-1] != 0x1A:
        data.append(0x1A)
    return bytes(data)


def build_launcher(bas_name: str) -> bytes:
    load_name = bas_name
    if "." not in load_name:
        load_name = f"{load_name}.BAS"
    # LOAD replaces the launcher in memory, so RUN/SYSTEM never execute. CHAIN
    # runs the program and resumes at the next line when it ENDs.
    lines = [
        (10, bytes([0xFE, 0x8C]) + quote(load_name)),
        (20, bytes([0xFE, 0x83])),
    ]
    return encode_program(lines)


def main() -> int:
    bas_name = sys.argv[1] if len(sys.argv) > 1 else "DICE.BAS"
    encoded = build_launcher(bas_name)
    print(decode(encoded))
    out = Path(__file__).resolve().parents[1] / "dos" / "PLAY.BAS"
    out.write_bytes(encoded)
    print(f"Wrote {out} ({len(encoded)} bytes)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
