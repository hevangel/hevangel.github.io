# AGENTS.md

This file provides guidance for AI agents working with this repository.

## Project Overview

Personal programming journal website (hevangel.github.io), a static site documenting a programming journey from GW-BASIC and dBASE through C++, Win16, and hardware-description languages.

## Project Structure

```
.
├── index.html          # Main data-driven timeline page
├── data/               # Timeline content (languages.json)
├── gwbasic.html        # GW-BASIC interpreter page
├── win.html            # Windows 3.1 Appleby Marking System showcase
├── js/                 # GW-BASIC interpreter modules
├── gwbasic/            # Example BASIC programs + GWBASIC.EXE
├── win/                # Win16 source/binaries, disks, data, and PCjs assets
├── scripts/            # Utility scripts
└── .gitignore
```

## Key Files

- **index.html** - Responsive timeline shell and per-entry visual styles
- **data/languages.json** - Ordered timeline entries, content, and showcase links
- **gwbasic.html** - Interactive GW-BASIC runner using a vanilla JavaScript interpreter
- **win.html** - PCjs-hosted COMPAQ DeskPro 386 running the original Appleby Marking System under Windows 3.1
- **win/machine.xml** - PCjs hardware, ROM, floppy, and hosted hard-disk configuration
- **win/AMS-BOOT.IMG / win/AMS-DATA.IMG** - Bootable DOS program disk and restored class-data disk
- **js/** - Tokenizer, parser, interpreter, and runtime for GW-BASIC

## Development

```bash
# Local development
npx serve .
# or
python -m http.server
```

No build step is required. The Windows runner needs HTTP serving; opening `win.html` directly with `file://` will not load its machine assets.

## Deployment

GitHub Pages deploys from the `main` branch root automatically on push. The Windows machine downloads its stock Windows 3.1 hard disk from PCjs at runtime; the PCjs runtime/ROMs and custom AMS floppy images are served locally.

## Agent Guidelines

- This is a static site: do not introduce build tools, bundlers, frameworks, or production dependencies.
- Edit HTML, CSS, JavaScript, JSON, and PCjs XML directly.
- Keep `data/languages.json` valid and update it when adding timeline entries.
- Preserve the original Win16 binaries and restored disk images unless the task explicitly requires rebuilding them.
- Retain PCjs attribution and `win/pcjs/LICENSE.txt` when modifying vendored emulator assets.
- Test locally with a static server before pushing.
- Keep changes minimal and focused.
