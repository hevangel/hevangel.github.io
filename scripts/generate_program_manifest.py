"""Regenerate programs/manifest.json from .BAS files in programs/."""

from __future__ import annotations

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROGRAMS_DIR = ROOT / "programs"
MANIFEST_PATH = PROGRAMS_DIR / "manifest.json"

LABELS = {
    "gwbasic-placeholder.bas": "Placeholder Demo",
    "WORD.BAS": "Word Processor",
    "MONEY.BAS": "Money Manager",
    "DICE.BAS": "Dice",
    "SNAKE.BAS": "Snake",
    "RACE.BAS": "Race",
    "RACE2.BAS": "Race 2",
    "RACE3.BAS": "Race 3",
    "HORSE.BAS": "Horse",
    "GFHORSE.BAS": "GF Horse",
    "DRAW.BAS": "Draw",
    "MORT.BAS": "Mort",
    "CHANGE.BAS": "Change",
    "CHECKID.BAS": "Check ID",
    "CONVERT1.BAS": "Convert",
    "PROGRAM1.BAS": "Program 1",
    "PROGRAM2.BAS": "Program 2",
    "PROGRAM3.BAS": "Program 3",
    "PROGRAM4.BAS": "Program 4",
    "PROJECT1.BAS": "Project 1",
    "PROJECT2.BAS": "Project 2",
    "SONG1.BAS": "Song 1",
    "SONG2.BAS": "Song 2",
}


def label_from_name(name: str) -> str:
    if name in LABELS:
        return LABELS[name]
    stem = Path(name).stem.replace("_", " ")
    if stem.isupper():
        return stem.title()
    return stem


def main() -> None:
    bas_files = sorted(
        (p.name for p in PROGRAMS_DIR.iterdir() if p.is_file() and p.suffix.lower() == ".bas"),
        key=lambda name: (name.lower() == "gwbasic-placeholder.bas", name.lower()),
    )
    programs = [{"path": f"programs/{name}", "label": label_from_name(name)} for name in bas_files]
    MANIFEST_PATH.write_text(json.dumps(programs, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {len(programs)} program(s) to {MANIFEST_PATH}")
    for entry in programs:
        print(f"  - {entry['label']}: {entry['path']}")


if __name__ == "__main__":
    main()
