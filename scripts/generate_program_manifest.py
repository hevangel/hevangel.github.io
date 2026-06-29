"""Regenerate gwbasic_programs/manifest.json from .BAS files in gwbasic_programs/."""

from __future__ import annotations

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PROGRAMS_DIR = ROOT / "gwbasic_programs"
MANIFEST_PATH = PROGRAMS_DIR / "manifest.json"
SUPPORT_SUFFIXES = {".dat", ".fil", ".pic"}
DEFAULT_PROGRAM = "gwbasic_programs/RACE2.BAS"

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

DESCRIPTIONS = {
    "CHECKID.BAS": (
        "Validates a Hong Kong ID card number. Homework from my first high school "
        "computer science class."
    ),
    "CONVERT1.BAS": (
        "Converts a number between bases 2 and 16. Homework from my first high "
        "school computer science class."
    ),
    "DICE.BAS": (
        "Guess big or small on a random dice roll. Homework from my first high "
        "school computer science class."
    ),
    "DRAW.BAS": (
        "Move a cursor and draw on the screen with the keyboard. Homework from my "
        "first high school computer science class."
    ),
    "RACE.BAS": (
        "Super Horse Racing, first version. Homework from my first high school "
        "computer science class."
    ),
    "RACE2.BAS": (
        "Super Horse Racing — my final class project and the very first game I ever "
        "wrote. Homework from my first high school computer science class."
    ),
    "RACE3.BAS": (
        "Super Chicken horse racing v2.0 with save/load — never finished. Homework "
        "from my first high school computer science class."
    ),
    "SONG1.BAS": (
        "Plays a tune with GW-BASIC PLAY statements. Homework from my first high "
        "school computer science class."
    ),
    "SONG2.BAS": (
        "Plays a longer song from stored note strings. Homework from my first high "
        "school computer science class."
    ),
}


def label_from_name(name: str) -> str:
    if name in LABELS:
        return LABELS[name]
    stem = Path(name).stem.replace("_", " ")
    if stem.isupper():
        return stem.title()
    return stem


def description_from_name(name: str) -> str:
    if name in DESCRIPTIONS:
        return DESCRIPTIONS[name]
    return (
        f"GW-BASIC program from my first high school computer science class ({name})."
    )


def main() -> None:
    bas_files = sorted(
        (p.name for p in PROGRAMS_DIR.iterdir() if p.is_file() and p.suffix.lower() == ".bas"),
        key=lambda name: (name.lower() == "gwbasic-placeholder.bas", name.lower()),
    )
    support_files = sorted(
        p.name
        for p in PROGRAMS_DIR.iterdir()
        if p.is_file() and p.suffix.lower() in SUPPORT_SUFFIXES
    )
    default_program = DEFAULT_PROGRAM
    if bas_files and DEFAULT_PROGRAM.split("/")[-1] not in bas_files:
        default_program = f"gwbasic_programs/{bas_files[0]}"

    manifest = {
        "defaultProgram": default_program,
        "programs": [
            {
                "path": f"gwbasic_programs/{name}",
                "label": label_from_name(name),
                "description": description_from_name(name),
            }
            for name in bas_files
        ],
        "supportFiles": [f"gwbasic_programs/{name}" for name in support_files],
    }
    MANIFEST_PATH.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")
    print(f"Wrote {len(manifest['programs'])} program(s) to {MANIFEST_PATH}")
    for entry in manifest["programs"]:
        print(f"  - {entry['label']}: {entry['path']}")
    if manifest["supportFiles"]:
        print(f"Support files ({len(manifest['supportFiles'])}):")
        for path in manifest["supportFiles"]:
            print(f"  - {path}")


if __name__ == "__main__":
    main()
