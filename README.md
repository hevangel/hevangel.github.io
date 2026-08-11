# Programming Journal

A personal programming journal hosted at [hevangel.github.io](https://hevangel.github.io), documenting my programming journey across languages and eras.

## Structure

- `index.html` / `data/languages.json` - Data-driven timeline shell, ordered entries, and showcase links
- `gwbasic.html` / `gwbasic/` / `js/` - Interactive GW-BASIC playground, examples, and interpreter
- `assembly.html` / `assembly/` - 1997 MC68000 source-level workbench, original sources, and assembler listings
- `vhdl.html` / `vhdl/` - Static AM2901 source-level simulator, waveform viewer, and preserved 1991–1992 VHDL benchmark
- `javascript.html` / `javascript/` - Operation Minmay showcase and preserved Netscape-era game
- `java.html` / `java/` - Macross Tetris applet showcase and preserved Java files
- `win.html` / `win/` - Appleby Marking System showcase, Win16 files, data, and PCjs assets
- `scripts/` - Utility scripts

## Features

- **Programming Timeline** - Responsive, themed entries rendered from `data/languages.json`
- **GW-BASIC Runner** - Interactive browser-based GW-BASIC interpreter
- **68000 Lab Workbench** - Runs the 1997 E&CE 222 calculator behavior from its preserved MC68000 source flow, with source-line breakpoints, stepping, keypad/ACIA I/O, and live D/A registers
- **AM2901 VHDL Lab** - Parses the original 1992 VHDL vectors in-browser, executes the preserved falling-edge AM2901 behavior, checks the original assertions, renders interactive waveforms, and exports VCD
- **Operation Minmay** - A Netscape-era JavaScript space shooter restored on a modern Canvas runtime with its original artwork and mission flow
- **Macross Tetris** - The original two-player Java applet bytecode running unchanged in a Java 8-compatible browser JVM
- **Windows 3.1 Runner** - The original 1996 Appleby Marking System binaries running in PCjs on an emulated COMPAQ DeskPro 386
- **Restored Sample Database** - Teachers, classes, students, faculties, grades, and houses recovered from the original `SAMPLE.AMS` backup
- **Dark, Responsive Design** - Plain HTML and CSS that adapts to desktop and mobile layouts

The assembly corpus contains eleven original `.ASM` files and their zero-error 2500 A.D. assembler `.LST` outputs from June 1997. Its object files, final linked image, linker map, and TUTOR board ROM did not survive. `assembly.html` therefore uses an explicitly labeled source-level execution model: it follows the original control flow and hardware addresses while exposing reconstructed link addresses, rather than claiming binary- or cycle-accurate emulation.

The VHDL corpus preserves the UC Irvine CADLAB AM2901 benchmark and component labs from 1991–1992, including MVL7 support packages, algorithmic and functional-block architectures, translated test vectors, documentation, and ZYCAD command files. `vhdl.html` leaves that corpus unchanged: its dependency-free compatibility engine derives behavior from the algorithmic architecture and parses the original testbench as stimulus and assertion oracle. It is not presented as a general VHDL compiler. The design follows the static, browser-local direction demonstrated by [ghdl-browser](https://github.com/UnsignedChad/ghdl-browser), [ghdl-wasm](https://github.com/UnsignedChad/ghdl-wasm), and [VHDLive](https://vhdl.ai/vhdlive), whose current experimental runtime does not yet fully schedule or decode this legacy MVL7 design.

The Windows showcase boots custom MS-DOS 3.31 program/data disks and uses PCjs's hosted Windows 3.1 hard-disk image. No Win16 binaries were recompiled or reimplemented.

The JavaScript showcase keeps the original Netscape/Internet Explorer pages and artwork under `javascript/`, while `javascript/modern.html` recreates the game with current DOM, keyboard, and Canvas APIs. It uses no framework, build step, browser plug-in, or external runtime.

The Java showcase runs the original applet classes unchanged through [CheerpJ 4.3](https://cheerpj.com/docs/overview.html), a WebAssembly-based browser JVM loaded from its CDN. The original source, compiled classes, archive, MIDI, reference HTML, and artwork remain untouched; the integration is documented in [`java/CHEERPJ.md`](java/CHEERPJ.md).

## Tech Stack

- Plain HTML/CSS/JavaScript (no build step)
- Dependency-free MC68000 source-level execution/debugging model
- Dependency-free AM2901 VHDL testbench parser, source-level simulator, waveform viewer, and VCD exporter
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

Then open the URL printed by the selected server. HTTP serving is required for the assembly workbench's source fetches, the VHDL workbench's preserved source/testbench fetches, and the Windows runner's machine assets.

## Deployment

Push to `main` branch; GitHub Pages deploys automatically from the repository root.
