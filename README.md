# Programming Journal

A personal programming journal hosted at [hevangel.github.io](https://hevangel.github.io), documenting my programming journey through different languages and eras.

## Structure

- `index.html` - Main timeline page with four parallel tracks: GW-BASIC, Pascal, dBASE IV+, and a placeholder for future entries
- `gwbasic.html` - Interactive GW-BASIC runner/playground
- `js/` - JavaScript modules for the GW-BASIC interpreter
- `gwbasic/` - Example GW-BASIC programs
- `scripts/` - Utility scripts

## Features

- **Parallel Timeline Layout** - Four parallel tracks showing programming eras side-by-side
- **GW-BASIC Runner** - Interactive GW-BASIC interpreter in the browser
- **Responsive Design** - Responsive grid layout (4 columns → 2 columns → 1 column)
- **Dark theme** with custom CSS variables

## Tech Stack

- Plain HTML/CSS/JS (no build step)
- Vanilla JavaScript GW-BASIC interpreter
- GitHub Pages deployment

## Development

```bash
# Serve locally
npx serve .
# or
python -m http.server
```

Then open `http://localhost:8000` or `http://localhost:3000`

## Deployment

Push to `main` branch → GitHub Pages deploys automatically from root.

## Structure

```
.
├── index.html          # Main timeline page
├── gwbasic.html        # GW-BASIC interactive runner
├── js/                 # JS modules (tokenizer, parser, interpreter, etc.)
├── gwbasic/            # Sample GW-BASIC programs
├── scripts/            # Build/utilities
└── .gitignore
```