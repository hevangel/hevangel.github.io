"""Build a js-dos bundle for GW-BASIC programs."""

import json
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
DOS_DIR = ROOT / "dos"
PROGRAMS_DIR = ROOT / "programs"
BUNDLE_PATH = DOS_DIR / "gwbasic.jsdos"

DOSBOX_CONF = """[sdl]
output=surface
fullscreen=false
windowresolution=640x400
autolock=true

[dosbox]
machine=vga
memsize=16

[render]
scaler=none
aspect=false

[cpu]
cycles=max

[autoexec]
@echo off
mount c .
c:
gwbasic
"""


def main() -> None:
    gwbasic_exe = DOS_DIR / "GWBASIC.EXE"
    if not gwbasic_exe.exists():
        raise SystemExit(f"Missing {gwbasic_exe}")

    with zipfile.ZipFile(BUNDLE_PATH, "w", compression=zipfile.ZIP_DEFLATED) as bundle:
        bundle.writestr(".jsdos/dosbox.conf", DOSBOX_CONF)
        bundle.writestr(
            ".jsdos/jsdos.json",
            json.dumps(
                {
                    "name": "GW-BASIC",
                    "description": "GW-BASIC 3.x via DOSBox for hevangel.github.io",
                },
                indent=2,
            )
            + "\n",
        )
        bundle.write(gwbasic_exe, "GWBASIC.EXE")

        for path in sorted(PROGRAMS_DIR.iterdir()):
            if path.is_file() and path.suffix.lower() in {".bas", ".dat", ".fil", ".pic"}:
                bundle.write(path, f"PROGRAMS/{path.name}")

    print(f"Wrote {BUNDLE_PATH}")


if __name__ == "__main__":
    main()
