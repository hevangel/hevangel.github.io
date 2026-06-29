# AGENTS.md

This file provides guidance for AI agents working with this repository.

## Project Overview

Personal programming journal website (hevangel.github.io) - a static site documenting programming journey through GW-BASIC, Pascal, dBASE IV+, and future languages.

## Project Structure

```
.
├── index.html          # Main timeline page
├── gwbasic.html        # GW-BASIC interpreter page
├── js/                 # JavaScript modules for GW-BASIC interpreter
├── gwbasic/            # Example BASIC programs + GWBASIC.EXE
├── scripts/            # Utility scripts
└── .gitignore
```

## Key Files

- **index.html** - Main page with 4-column timeline layout (responsive: 4→2→1 columns)
- **gwbasic.html** - Interactive GW-BASIC runner using vanilla JS interpreter
- **js/** - Tokenizer, parser, interpreter, runtime for GW-BASIC

## Development

```bash
# Local development
npx serve .
# or
python -m http.server
```

No build step required - plain HTML/CSS/JS.

## Deployment

GitHub Pages deploys from `main` branch root automatically on push.

## Agent Guidelines

- This is a static site - no build tools, bundlers, or frameworks
- Edit HTML/CSS/JS directly
- Test locally with any static server before pushing
- Keep changes minimal and focused
- No package.json, no dependencies to install