# Programming Journal

A personal programming journal hosted at [hevangel.github.io](https://hevangel.github.io), documenting my programming journey across languages and eras.

## Structure

- `index.html` - Main, data-driven programming timeline
- `data/languages.json` - Ordered timeline entries and showcase links
- `gwbasic.html` - Interactive GW-BASIC runner/playground
- `win.html` - Appleby Marking System Windows 3.1 showcase
- `js/` - JavaScript modules for the GW-BASIC interpreter
- `gwbasic/` - Example GW-BASIC programs
- `win/` - Original Win16 C++ source/binaries, restored data, PCjs runtime, and boot disks
- `scripts/` - Utility scripts

## Features

- **Programming Timeline** - Responsive, themed entries rendered from `data/languages.json`
- **GW-BASIC Runner** - Interactive browser-based GW-BASIC interpreter
- **Windows 3.1 Runner** - The original 1996 Appleby Marking System binaries running in PCjs on an emulated COMPAQ DeskPro 386
- **Restored Sample Database** - Teachers, classes, students, faculties, grades, and houses recovered from the original `SAMPLE.AMS` backup
- **Dark, Responsive Design** - Plain HTML and CSS that adapts to desktop and mobile layouts

The Windows showcase boots custom MS-DOS 3.31 program/data disks and uses PCjs's hosted Windows 3.1 hard-disk image. No Win16 binaries were recompiled or reimplemented.

## Tech Stack

- Plain HTML/CSS/JavaScript (no build step)
- Vanilla JavaScript GW-BASIC interpreter
- Vendored PCjs runtime and ROM assets under the MIT License
- GitHub Pages deployment

## Development

```bash
# Serve locally
npx serve .
# or
python -m http.server
```

Then open the URL printed by the selected server.

## Deployment

Push to `main` branch; GitHub Pages deploys automatically from the repository root.
