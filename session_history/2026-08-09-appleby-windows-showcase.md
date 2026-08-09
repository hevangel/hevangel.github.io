# Session History: Appleby Marking System Windows 3.1 Showcase

- **Workspace:** `B:\hevangel.github.io`
- **Linked Kiro conversations:** `d5e621b5-8c3b-4b78-a18d-e5251068dbfa`, `2471652f-03d5-4de1-9060-da36db76081b`
- **History captured:** 2026-08-09 13:18:19.149 -07:00
- **Status at capture:** Active continued conversation; counts, estimates, and duration are a point-in-time snapshot

## Goal

Showcase the original 1995–1996 Appleby Marking System as a single site-styled page with the genuine Win16 binaries running under Windows 3.1 in the browser. Link it from the main timeline, restore the sample database, document the new feature, and validate the complete emulator boot.

## Work completed

- Created `win.html` with a Windows 3.1-inspired interface, program history, usage notes, downloads, restart, fullscreen, and timeline navigation.
- Configured PCjs in `win/machine.xml` as a COMPAQ DeskPro 386 with 4 MB RAM and IBM VGA.
- Diagnosed the original 80286 startup failure as an illegal `0F 85` instruction in `AMSMAN.EXE`; the original binary requires an 80386.
- Built `win/AMS-BOOT.IMG` and `win/AMS-DATA.IMG` without recompiling the executables.
- Decoded `SAMPLE.AMS` and restored 12 teachers, 28 classes, 39 students, 3 faculties, 6 grades, and 6 houses.
- Installed the program, runtime DLL, restored database, and 28 class files automatically during DOS boot.
- Replaced the obsolete `win/index.html` experiment with a redirect to `/win.html`.
- Added a Win16/Windows 3.1 entry to `data/languages.json`, matching styles in `index.html`, and a direct footer link.
- Updated `README.md` and `AGENTS.md`.
- Added `win/pcjs/LICENSE.txt` and retained PCjs attribution.
- Removed unused `win/AMS-WIN310.json` and temporary `pcjs310.html`.
- Removed malformed CDATA/trailing wrapper text from the root `index.html`.

## Key debugging findings

1. `AMSMAN.EXE` is structurally valid and did not need recompilation.
2. The IBM AT/80286 machine faulted at `0010:0042` because opcode `0F 85` is a 386 near conditional branch.
3. A COMPAQ DeskPro 386 BIOS/chipset fixed startup.
4. `BC450RTL.DLL` must be copied to `C:\WINDOWS\SYSTEM`.
5. Windows must launch with `WIN /S AMSMAN.EXE`.
6. `C:\WINDOWS\AMS.INI` containing `Manager=1` proved that AMS Manager completed startup.
7. Pressing Enter dismisses the password dialog because password protection is disabled in this build.

## Files changed

- Site and documentation: `win.html`, `index.html`, `data/languages.json`, `README.md`, `AGENTS.md`.
- Emulator delivery: `win/machine.xml`, `win/index.html`, `win/AMS-BOOT.IMG`, `win/AMS-DATA.IMG`, and `win/pcjs/`.
- Session record: `session_history/2026-08-09-appleby-windows-showcase.md`.
- Global automation: `~/.kiro/skills/session-history/SKILL.md`, its `scripts/collect-kiro-session.ps1`, and `~/.kiro/steering/session-history.md`.

## Validation

- Parsed `data/languages.json`; 14 timeline entries were present and the Win16 launch URL was `/win.html`.
- Verified the timeline rendered 14 sections with no browser request or page errors.
- Verified `/win/` redirected to `/win.html`.
- Verified boot-disk root structure and all 28 class files.
- Booted production `win.html` for 241 seconds: PCjs mounted A:, B:, and C:, reached Windows/AMS, accepted Enter, and displayed the stable populated Manager interface.
- Verified no failed local requests, HTTP errors, or page errors during production boot.
- `git diff --check` passed; only Windows line-ending notices were emitted.
- Executed the installed session collector successfully for this workspace.

## Model usage at capture

Kiro's local GUI session JSON and extension logs do **not** contain structured per-model input/output token totals. Exact token expenditure therefore cannot be calculated from Kiro without inventing data.

| Model | Exact tokens spent | Logged model calls | Call breakdown |
|---|---:|---:|---|
| Claude Opus 5 (`claude-opus-5`) | **Not exposed by Kiro; exact total unavailable** | 175 | 175 Vibe calls |
| GPT 5.6 Sol (`gpt-5.6-sol`) | **Not exposed by Kiro; exact total unavailable** | 135 | 88 Vibe, 35 summarization, 12 context-gatherer |

The separate `dev_data/tokens_generated.jsonl` ledger contained 10,470 prompt tokens and 0 generated tokens under generic `provider: kiro`, `model: agent` rows. Those rows do not identify a conversation or distinguish Opus from Sol, so they are deliberately not attributed to either model.

## Visible-text token estimate

The linked GUI histories retained seven complete user messages totaling 11,187 characters and 1,662 whitespace-delimited words. Using the documented mixed prose/code heuristic of 3.2–4.2 characters per token gives:

- **Stored user text:** approximately **2,664–3,496 tokens**; midpoint **3,080**.
- **Stored assistant text:** unavailable. All six assistant history entries contain only Kiro's `On it.` placeholder, which was excluded; this means unknown output tokens, not zero output tokens.

This is intentionally labeled a **stored visible-text estimate**, not total API usage. It excludes system instructions, steering, tool schemas, attached file context, tool results, cache reads/writes, compaction content not retained in messages, and omitted assistant responses. The history has no reliable per-message timestamps, so the visible user-text estimate cannot be divided between Opus and Sol.

## Wall-clock duration at capture

- **Linked conversation chain:** 13:13:01.391, from the first exact logged event at 2026-08-09 00:05:17.758 -07:00 to capture at 13:18:19.149 -07:00.
- **Current continued Kiro record:** 5:10:22.157, from indexed creation at 2026-08-09 08:07:56.992 -07:00 to capture.
- The conversation was still active at capture, so these durations are snapshots rather than final closed-session totals.

## Metadata method and limitations

1. Located the workspace's session index under `%APPDATA%\Kiro\User\globalStorage\kiro.kiroagent\workspace-sessions\*\sessions.json` by matching `workspaceDirectory`.
2. Read the linked session UUIDs and indexed creation time.
3. Correlated those UUIDs with exact `[q-developer-converse] Sending GenerateAssistantResponse` events in the newest `Kiro Logs.log`.
4. Counted events by `modelId` and `agentMode`, excluding terminal log lines that merely repeated those strings.
5. Calculated elapsed wall time from timestamps, not model execution spans.
6. Searched session records, extension logs, the generic token ledger, and Kiro's state database. None exposed per-model token totals; Kiro's usage state reported invocation/credit units instead of tokens.
7. Extracted recoverable visible text from `history[].message.content`, excluded placeholders, and reported a character-based range rather than false precision.

## Follow-up status

The Windows showcase and history automation are complete. The global skill and collector now calculate stored visible-text token estimates while preserving the distinction between estimates and total API usage. The global steering rule asks Kiro to update one record per conversation chain after future non-trivial work. No commit or push was performed.
