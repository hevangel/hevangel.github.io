# Running the original Macross Tetris applet

**Integration date:** 2026-08-09

The browser showcase runs the original compiled Java applet unchanged. It does not translate, recompile, or port the game to JavaScript.

## Runtime

Modern browsers removed NPAPI Java plug-ins and the native `<applet>` implementation. [`java/applet.html`](applet.html) loads the pinned [CheerpJ 4.3](https://cheerpj.com/docs/overview.html) runtime and declares the original `mactetris.class` in a `<cheerpj-applet>` element. CheerpJ supplies a Java 8-compatible OpenJDK runtime, AWT, Java threads, class loading, and bytecode execution through WebAssembly/JavaScript inside the browser.

The helper page intentionally lives beside the historical class files. The source loads artwork with `getImage(getDocumentBase(), "pic/...")`; serving the applet from `/java/applet.html` preserves `/java/` as its document base, so all original `java/pic/` paths work without source edits or copied assets. Root [`java.html`](../java.html) embeds that fixed 500×380 runtime page and scales it responsively.

## Preserved files

All historical files remain unchanged: `code/*.java`, all `.class` files, `mactetris.zip`, `tetris.html`, `tetris01.mid`, `pic/*`, and the transfer logs. `tetris.html` is reference material only and is not used by the showcase.

The only applet integration file added inside the historical folder is `applet.html`. No Java source or bytecode was modified.

## Known historical limitations

- The applet's score screen calls obsolete plain-HTTP Waterloo CGI endpoints. Modern mixed-content and cross-origin protections may prevent online score retrieval/submission; this behavior remains in the original bytecode.
- The applet's Sound Effect option was never implemented by the original Java code. The preserved `tetris01.mid` is instead played by `java.html` through the pinned [html-midi-player 1.6.0](https://github.com/cifkao/html-midi-player) browser synthesizer. Music is enabled by default but begins only when the user clicks the original Start button, and the page toggle can mute or resume it.
- Initial startup downloads the CheerpJ/OpenJDK runtime from Leaning Technologies' CDN and can take several seconds. An internet connection is required.
- The applet uses deprecated immediate AWT painting and event APIs. CheerpJ executes them for compatibility; focus the game before using its keyboard controls.

## Local testing

Serve the repository root over HTTP:

```powershell
python -m http.server 8000
```

Open `http://127.0.0.1:8000/java.html`. Do not use `file://`; CheerpJ requires HTTP or HTTPS. Check the browser console and network panel if the CDN runtime or applet classes do not load.

## Sources

- [CheerpJ: Run a Java Applet](https://cheerpj.com/docs/getting-started/Java-applet.html)
- [CheerpJ applet tutorial](https://cheerpj.com/docs/tutorials/applet)
- [CheerpJ overview and licensing summary](https://cheerpj.com/docs/overview.html)

Content from these sources was rephrased for compliance with licensing restrictions.