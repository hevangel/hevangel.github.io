# Timeline project copy and authored dates

- Conversation UUID: `1532d05f-0be6-421b-992b-db37f6ecaad4`
- Captured: 2026-08-10 01:06:59 -07:00

## Goal
Rewrite the GW-BASIC, dBASE III+, C++, Java, and JavaScript timeline entries in the concrete project/preservation style used by Win16 and assembly, using the years the programs were written.

## Work completed
- Replaced generic language-history copy with project, design/preservation, and browser-runner descriptions.
- Documented Super Horse Racing and the surviving 1992 GW-BASIC collection.
- Documented the 1994 Appleby College Library System and its original dBASE modules/data.
- Rewrote C++ around the 1995 summer-vacation learning exercise, English Word Collector 1.5B: vocabulary records, CRUD/search/print features, quiz/game, linked-list sorting, binary persistence, menu system, and supporting UI exercises.
- Documented Macross Tetris, preserved source/bytecode/assets, and CheerpJ restoration.
- Documented Operation Minmay, its Netscape layer implementation, and modern Canvas compatibility edition.
- Set dates to GW-BASIC 1992, dBASE 1994, C++ 1995, Java 1996, and JavaScript 1997 following the user's corrections.

## Key decisions/debugging
Repository manifests, original sources, transfer logs, and showcase pages were inspected to keep descriptions concrete. The user's clarification that English Word Collector was a summer-vacation C++ learning exercise supersedes the existing showcase wording that called it a Grade 12 final project. The timeline now uses the corrected characterization.

## Files changed
- `data/languages.json`
- `session_history/2026-08-10-timeline-project-copy.md`

## Validation
- `data/languages.json` reports no IDE diagnostics.
- Node.js parsed the JSON successfully and confirmed the edited entries retain three chunks.
- Targeted checks confirmed Java uses 1996, JavaScript uses 1997, and C++ includes the summer-vacation English Word Collector description.

## Model usage
- `gpt-5.6-sol`, Vibe mode: 33 calls.
- `gpt-5.6-sol`, context-gatherer mode: 18 calls.
- `simple-task`, intent-classification mode: 6 calls.
- Exact per-model input/output token totals: Not exposed by Kiro; exact total unavailable.
- Generic unallocated ledger evidence: 13,253 prompt tokens and 0 generated tokens; not attributable to a named model.

## Visible-text token estimate
Stored visible user text: 287 characters, 53 words, 4 recoverable messages; estimated 69–90 tokens (midpoint 80) using the character-range method. Stored assistant entries were placeholders, so no assistant visible text was recoverable. This is not API usage; hidden instructions, tools, file context/results, cache traffic, and omitted assistant output are excluded.

## Wall-clock duration
Active session: 18m 7.7s at capture. Conversation chain: 18m 7.6s from the first exact logged event.

## Metadata method/limitations
Collected with the installed `collect-kiro-session.ps1` script, matching the workspace session index to exact `Sending GenerateAssistantResponse` log events. The active session may accumulate further calls after capture.

## Follow-up status
Complete; no known validation failures.