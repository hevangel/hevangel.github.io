# Programming Journal

A personal programming journal hosted at [hevangel.github.io](https://hevangel.github.io), documenting my programming journey across languages and eras.

## Structure

- `index.html` - Main, data-driven programming timeline
- `data/languages.json` - Ordered timeline entries and showcase links
- `gwbasic.html` - Interactive GW-BASIC runner/playground
- `javascript.html` - Modern-browser Operation Minmay game showcase
- `java.html` - Original Macross Tetris Java applet running through a browser JVM
- `win.html` - Appleby Marking System Windows 3.1 showcase
- `js/` - JavaScript modules for the GW-BASIC interpreter
- `gwbasic/` - Example GW-BASIC programs
- `javascript/` - Preserved Netscape-era game pages/artwork and modern compatibility runtime
- `java/` - Preserved Java applet, artwork, and CheerpJ launch page
- `win/` - Original Win16 C++ source/binaries, restored data, PCjs runtime, and boot disks
- `scripts/` - Utility scripts

## Features

- **Programming Timeline** - Responsive, themed entries rendered from `data/languages.json`
- **GW-BASIC Runner** - Interactive browser-based GW-BASIC interpreter
- **Operation Minmay** - A Netscape-era JavaScript space shooter restored on a modern Canvas runtime with its original artwork and mission flow
- **Macross Tetris** - The original two-player Java applet bytecode running unchanged in a Java 8-compatible browser JVM
- **Windows 3.1 Runner** - The original 1996 Appleby Marking System binaries running in PCjs on an emulated COMPAQ DeskPro 386
- **Restored Sample Database** - Teachers, classes, students, faculties, grades, and houses recovered from the original `SAMPLE.AMS` backup
- **Dark, Responsive Design** - Plain HTML and CSS that adapts to desktop and mobile layouts

The Windows showcase boots custom MS-DOS 3.31 program/data disks and uses PCjs's hosted Windows 3.1 hard-disk image. No Win16 binaries were recompiled or reimplemented.

The JavaScript showcase keeps the original Netscape/Internet Explorer pages and artwork under `javascript/`, while `javascript/modern.html` recreates the game with current DOM, keyboard, and Canvas APIs. It uses no framework, build step, browser plug-in, or external runtime.

The Java showcase runs the original applet classes unchanged through [CheerpJ 4.3](https://cheerpj.com/docs/overview.html), a WebAssembly-based browser JVM loaded from its CDN. The original source, compiled classes, archive, MIDI, reference HTML, and artwork remain untouched; the integration is documented in [`java/CHEERPJ.md`](java/CHEERPJ.md).

## Tech Stack

- Plain HTML/CSS/JavaScript (no build step)
- Vanilla JavaScript GW-BASIC interpreter
- CheerpJ 4.3 browser JVM for the original Java applet bytecode
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
