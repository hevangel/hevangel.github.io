# Session History: AM2901 VHDL browser lab

- Conversation UUID: `2308bb48-5751-4eef-abcb-ff7df5ecfe69`
- Captured: 2026-08-10T17:14:10.8485418-07:00 (active session; counts can increase after capture)

## Goal
Add a static `vhdl.html` showcase for the copied university VHDL lab, make its AM2901 simulation and waveform viewing work in-browser, and update the timeline, README, and agent guidance.

## Work completed
- Mapped the copied 1991–1992 UC Irvine AM2901 corpus, top entities, compile order, and 10 ns translated testbench.
- Researched `ghdl-browser`, `ghdl-wasm`, and VHDLive. Their current experimental browser path does not provide complete scheduling/signal decoding for this legacy multi-file MVL7 design; VHDLive also retains server compile/simulation endpoints.
- Added a dependency-free browser compatibility engine that parses the original VHDL cycles, models `a2901` falling-edge behavior, and uses the original assertions as its oracle.
- Added run, pause, reset, and step controls; source browsing; instruction decode; RAM/Q/port state; interactive scalar/bus waveform rendering; cursor/zoom controls; and VCD export.
- Enabled and rewrote the VHDL timeline entry, updated index metadata, and documented the runtime/preservation boundary.

## Key decisions/debugging
- Preserved every copied historical lab file unchanged; browser compatibility is isolated to new root/runtime files.
- Used an honestly labeled AM2901 source-level compatibility engine instead of claiming general GHDL execution.
- Corrected browser validation's timeline selector from `.launch-btn` to the repository's actual `.launch-link`; no product-code failure was involved.

## Files changed
- Added: `vhdl.html`, `vhdl/simulator.js`, `vhdl/simulator.css`.
- Updated: `index.html`, `data/languages.json`, `README.md`, `AGENTS.md`.
- Added bookkeeping: `session_history/2026-08-10-am2901-vhdl-browser-lab.md`.
- The copied historical files already present under `vhdl/2901/` were not modified.

## Validation
- `node --check vhdl/simulator.js`: passed.
- `JSON.parse(data/languages.json)`: passed.
- IDE diagnostics on all changed HTML/CSS/JS/JSON/Markdown files: no issues.
- `git diff --check`: passed (only Windows LF→CRLF notices).
- Headless Chromium over `python -m http.server 8765`: parsed 431 cycles; all 485 original assertions passed; no console/page errors; waveform canvas rendered; cursor and zoom worked; VCD export enabled; mobile width had no document overflow; timeline href was `/vhdl.html`.

## Model usage
- `gpt-5.6-sol`, vibe mode: 34 calls (17:00:05.389–17:14:02.432).
- `gpt-5.6-sol`, context-gatherer mode: 7 calls (17:00:17.881–17:01:32.369).
- `simple-task`, intent-classification mode: 1 call.
- Exact per-model input/output token totals: Not exposed by Kiro; exact total unavailable.
- Generic token ledger evidence: 13,350 prompt tokens and 0 generated tokens, unallocated and not attributable to a named model.

## Visible-text token estimate
- Stored user text: 1 recoverable message, 358 characters, 44 words; estimated 86–112 tokens (midpoint 99) using the skill's character-range method.
- Stored assistant text: no recoverable messages in GUI history at capture.
- This is stored visible-text only, not API usage; hidden instructions, steering, tools, file context/results, cache traffic, summaries, and omitted assistant output are excluded.

## Wall-clock duration
- Active session: 14m 05.743s at capture.
- Conversation chain: 14m 05.615s from the first exact logged event; one linked session ID.

## Metadata method/limitations
- Collected with the session-history PowerShell collector, matching workspace session indexes to `[q-developer-converse] Sending GenerateAssistantResponse` events in `Kiro Logs.log`.
- The active session continued after capture, so final response/bookkeeping calls are not included in the recorded counts.

## Follow-up status
Requested implementation and validation are complete. No known failures remain.