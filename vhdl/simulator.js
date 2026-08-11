(() => {
  "use strict";

  const PATHS = {
    algorithmic: "/vhdl/2901/alg_beh/alg_beh2901.vhdl",
    functional: "/vhdl/2901/funct_blocks_alg_beh/funct_block_alg_beh2901.vhdl",
    testbench: "/vhdl/2901/test_vectors_2901.vhdl"
  };

  const SOURCE_NAMES = {
    algorithmic: "alg_beh2901.vhdl",
    functional: "funct_block_alg_beh2901.vhdl",
    testbench: "test_vectors_2901.vhdl"
  };

  const SOURCE_LABELS = ["A, Q", "A, B", "0, Q", "0, B", "0, A", "D, A", "D, Q", "D, 0"];
  const FUNCTION_LABELS = ["R + S + C0", "¬R + S + C0", "R + ¬S + C0", "R OR S", "R AND S", "¬R AND S", "R XOR S", "R XNOR S"];
  const DEST_LABELS = ["Q ← F", "no write", "RAM[B] ← F; Y ← A", "RAM[B] ← F", "shift RAM,Q right", "shift RAM right", "shift RAM,Q left", "shift RAM left"];
  const SIGNALS = [
    { name: "CLK", width: 1 }, { name: "I[8:0]", key: "I", width: 9 },
    { name: "Aadd", width: 4 }, { name: "Badd", width: 4 }, { name: "D[3:0]", key: "D", width: 4 },
    { name: "Y[3:0]", key: "Y", width: 4 }, { name: "Q[3:0]", key: "Q", width: 4 },
    { name: "C4", width: 1 }, { name: "Gbar", width: 1 }, { name: "Pbar", width: 1 },
    { name: "OVR", width: 1 }, { name: "F3", width: 1 }, { name: "F30", width: 1 }
  ];

  const elements = Object.fromEntries([
    "resetButton", "runButton", "stepButton", "downloadButton", "sourceSelect", "sourceTitle", "sourceLocation", "sourceCode",
    "status", "summary", "cycleReadout", "instruction", "ports", "registers", "assertions", "assertionCount", "timeReadout",
    "zoomOut", "zoomIn", "zoom", "cursor", "waveLabels", "waveScroll", "waveCanvas"
  ].map(id => [id, document.getElementById(id)]));

  const state = {
    sources: {}, vectors: [], machine: null, traces: [], assertionResults: [], nextCycle: 0,
    cursorCycle: 0, running: false, runToken: 0, scale: 24
  };

  function bit(value) {
    if (value === "0" || value === "L") return "0";
    if (value === "1" || value === "H") return "1";
    return value === "Z" ? "Z" : "X";
  }

  function vector(value, width) {
    const normalized = String(value ?? "").toUpperCase().split("").map(bit).join("");
    return normalized.padStart(width, "X").slice(-width);
  }

  function scalarNot(value) { return bit(value) === "0" ? "1" : bit(value) === "1" ? "0" : "X"; }
  function scalarAnd(a, b) { a = bit(a); b = bit(b); return a === "0" || b === "0" ? "0" : a === "1" && b === "1" ? "1" : "X"; }
  function scalarOr(a, b) { a = bit(a); b = bit(b); return a === "1" || b === "1" ? "1" : a === "0" && b === "0" ? "0" : "X"; }
  function scalarXor(a, b) { a = bit(a); b = bit(b); return /^[01]{2}$/.test(a + b) ? String(Number(a !== b)) : "X"; }
  function mapBits(a, b, operation) { return a.split("").map((value, index) => operation(value, b[index])).join(""); }
  function notBits(value) { return value.split("").map(scalarNot).join(""); }
  function numberBits(value, width) { return Number(value).toString(2).padStart(width, "0").slice(-width); }
  function binaryNumber(value) { return /^[01]+$/.test(value) ? parseInt(value, 2) : null; }

  function addBits(...values) {
    const width = values[0].length;
    const numbers = values.map(binaryNumber);
    return numbers.includes(null) ? "X".repeat(width) : numberBits(numbers.reduce((sum, value) => sum + value, 0), width);
  }

  function wiredOr(a, b) {
    a = String(a || "X").toUpperCase(); b = String(b || "X").toUpperCase();
    if (a === "1" || b === "1") return "1";
    if (a === "Z") return b; if (b === "Z") return a;
    if ((a === "0" || a === "L") && (b === "0" || b === "L")) return a === "0" || b === "0" ? "0" : "L";
    return "X";
  }

  function createMachine() {
    return {
      ram: Array.from({ length: 16 }, () => "XXXX"), q: "XXXX",
      drivers: { RAM0: "Z", RAM3: "Z", Q0: "Z", Q3: "Z" },
      outputs: { Y: "XXXX", C4: "X", Gbar: "X", Pbar: "X", OVR: "X", F3: "X", F30: "X", RAM0: "Z", RAM3: "Z", Q0: "Z", Q3: "Z" }
    };
  }

  function parseVectors(source) {
    const cyclePattern = /clk\s*<=\s*'1'\s*;\s*--[^\n]*Cycle No:\s*(\d+)/gi;
    const matches = [...source.matchAll(cyclePattern)];
    const current = { I: "XXXXXXXXX", D: "XXXX", C0: "X", OEbar: "X", Aadd: 0, Badd: 0, RAM0: "X", RAM3: "X", Q0: "X", Q3: "X" };

    return matches.map((match, index) => {
      const end = matches[index + 1]?.index ?? source.length;
      const segment = source.slice(match.index, end);
      const beforeFallingEdge = segment.split(/clk\s*<=\s*'0'/i)[0];
      const assignmentPattern = /\b(I|D|C0|OEbar|Aadd|Badd|RAM0|RAM3|Q0|Q3)\s*<=\s*(?:"([^"]+)"|'([^']+)'|(\d+))\s*;/gi;
      for (const assignment of beforeFallingEdge.matchAll(assignmentPattern)) {
        const name = assignment[1];
        const value = assignment[2] ?? assignment[3] ?? assignment[4];
        current[name] = name === "Aadd" || name === "Badd" ? Number(value) : value.toUpperCase();
      }

      const assertions = [];
      const assertionPattern = /assert\s*\(\s*(\w+)\s*=\s*(?:"([01XZWLH]+)"|'([01XZWLH])')\s*\)\s*report\s*"([^"]+)"/gi;
      for (const assertion of segment.matchAll(assertionPattern)) {
        assertions.push({ signal: assertion[1], expected: assertion[2] ?? assertion[3], message: assertion[4] });
      }

      return {
        cycle: Number(match[1]), line: source.slice(0, match.index).split("\n").length,
        inputs: { ...current }, assertions
      };
    });
  }

  function simulateCycle(machine, inputs) {
    const instruction = vector(inputs.I, 9);
    const word = binaryNumber(instruction);
    const sourceSelect = word === null ? -1 : word & 7;
    const functionSelect = word === null ? -1 : (word >> 3) & 7;
    const destinationSelect = word === null ? -1 : (word >> 6) & 7;
    const aAddress = Number(inputs.Aadd) & 15;
    const bAddress = Number(inputs.Badd) & 15;
    const A = machine.ram[aAddress];
    const B = machine.ram[bAddress];
    let R = "XXXX", S = "XXXX";

    switch (sourceSelect) {
      case 0: R = A; S = machine.q; break;
      case 1: R = A; S = B; break;
      case 2: R = "0000"; S = machine.q; break;
      case 3: R = "0000"; S = B; break;
      case 4: R = "0000"; S = A; break;
      case 5: R = vector(inputs.D, 4); S = A; break;
      case 6: R = vector(inputs.D, 4); S = machine.q; break;
      case 7: R = vector(inputs.D, 4); S = "0000"; break;
    }

    let rExtended = "0" + R;
    let sExtended = "0" + S;
    let result = "XXXXX";
    const carry = vector(inputs.C0, 1);
    switch (functionSelect) {
      case 0: result = addBits(rExtended, sExtended, "0000" + carry); break;
      case 1: rExtended = "0" + notBits(R); result = addBits(rExtended, sExtended, "0000" + carry); break;
      case 2: sExtended = "0" + notBits(S); result = addBits(rExtended, sExtended, "0000" + carry); break;
      case 3: result = mapBits(rExtended, sExtended, scalarOr); break;
      case 4: result = mapBits(rExtended, sExtended, scalarAnd); break;
      case 5: result = mapBits(notBits(rExtended), sExtended, scalarAnd); break;
      case 6: result = mapBits(rExtended, sExtended, scalarXor); break;
      case 7: result = notBits(mapBits(rExtended, sExtended, scalarXor)); break;
    }

    const F = result.slice(1);
    const p = mapBits(rExtended.slice(1), sExtended.slice(1), scalarOr);
    const g = mapBits(rExtended.slice(1), sExtended.slice(1), scalarAnd);
    const pAll = p.split("").reduce(scalarAnd, "1");
    const generate = scalarOr(g[0], scalarOr(scalarAnd(p[0], g[1]), scalarOr(scalarAnd(scalarAnd(p[0], p[1]), g[2]), scalarAnd(scalarAnd(scalarAnd(p[0], p[1]), p[2]), g[3]))));
    const outputs = {
      C4: result[0], Pbar: scalarNot(pAll), Gbar: scalarNot(generate),
      OVR: scalarAnd(scalarNot(scalarXor(rExtended[1], sExtended[1])), scalarXor(rExtended[1], result[1])),
      F3: result[1], F30: scalarNot(F.split("").reduce(scalarOr, "0"))
    };

    const external = { RAM0: inputs.RAM0, RAM3: inputs.RAM3, Q0: inputs.Q0, Q3: inputs.Q3 };
    const resolvedBefore = Object.fromEntries(Object.keys(external).map(name => [name, wiredOr(external[name], machine.drivers[name])]));
    const nextDrivers = { RAM0: "Z", RAM3: "Z", Q0: "Z", Q3: "Z" };
    let dataOut = F;

    switch (destinationSelect) {
      case 0: machine.q = F; break;
      case 1: break;
      case 2: dataOut = A; machine.ram[bAddress] = F; break;
      case 3: machine.ram[bAddress] = F; break;
      case 4:
        machine.ram[bAddress] = resolvedBefore.RAM3 + F.slice(0, 3);
        machine.q = resolvedBefore.Q3 + machine.q.slice(0, 3);
        nextDrivers.RAM0 = F[3]; nextDrivers.Q0 = machine.q[3];
        break;
      case 5:
        machine.ram[bAddress] = resolvedBefore.RAM3 + F.slice(0, 3);
        nextDrivers.RAM0 = F[3];
        break;
      case 6:
        machine.ram[bAddress] = F.slice(1) + resolvedBefore.RAM0;
        machine.q = machine.q.slice(1) + resolvedBefore.Q0;
        nextDrivers.RAM3 = F[0]; nextDrivers.Q3 = machine.q[0];
        break;
      case 7:
        machine.ram[bAddress] = F.slice(1) + resolvedBefore.RAM0;
        nextDrivers.RAM3 = F[0];
        break;
    }

    machine.drivers = nextDrivers;
    for (const name of Object.keys(external)) outputs[name] = wiredOr(external[name], nextDrivers[name]);
    outputs.Y = bit(inputs.OEbar) === "0" ? dataOut : "ZZZZ";
    machine.outputs = outputs;
    return { word, sourceSelect, functionSelect, destinationSelect, A, B, R, S, F, outputs };
  }

  function outputValue(result, signal) {
    if (signal.toUpperCase() === "Q") return state.machine.q;
    return result.outputs[signal] ?? result.outputs[Object.keys(result.outputs).find(key => key.toLowerCase() === signal.toLowerCase())] ?? "?";
  }

  function executeOne() {
    if (state.nextCycle >= state.vectors.length) return false;
    const vectorData = state.vectors[state.nextCycle];
    const result = simulateCycle(state.machine, vectorData.inputs);
    const trace = {
      cycle: vectorData.cycle, time: vectorData.cycle * 10 + 5,
      I: vector(vectorData.inputs.I, 9), D: vector(vectorData.inputs.D, 4), C0: bit(vectorData.inputs.C0), OEbar: bit(vectorData.inputs.OEbar),
      Aadd: numberBits(vectorData.inputs.Aadd, 4), Badd: numberBits(vectorData.inputs.Badd, 4),
      A: result.A, B: result.B, F: result.F, Y: result.outputs.Y, Q: state.machine.q, ram: [...state.machine.ram], ...result.outputs,
      decoded: result, sourceLine: vectorData.line
    };
    state.traces.push(trace);

    for (const assertion of vectorData.assertions) {
      const actual = outputValue(result, assertion.signal);
      state.assertionResults.push({
        cycle: vectorData.cycle, signal: assertion.signal, expected: assertion.expected,
        actual, pass: actual === assertion.expected, message: assertion.message
      });
    }

    state.nextCycle++;
    state.cursorCycle = state.nextCycle - 1;
    return true;
  }

  function clearNode(node) { while (node.firstChild) node.removeChild(node.firstChild); }

  function renderPorts(trace) {
    clearNode(elements.ports);
    const values = trace ? [
      ["I", trace.I], ["D", trace.D], ["A addr", trace.Aadd], ["B addr", trace.Badd],
      ["A", trace.A], ["B", trace.B], ["F", trace.F], ["Y", trace.Y],
      ["Q", trace.Q], ["C0", trace.C0], ["C4", trace.C4], ["OVR", trace.OVR],
      ["G̅", trace.Gbar], ["P̅", trace.Pbar], ["F3", trace.F3], ["F=0", trace.F30]
    ] : [["I", "—"], ["D", "—"], ["Y", "—"], ["Q", "—"]];

    for (const [name, value] of values) {
      const box = document.createElement("dl"); box.className = "port";
      const term = document.createElement("dt"); term.textContent = name;
      const description = document.createElement("dd"); description.textContent = value;
      box.append(term, description); elements.ports.append(box);
    }
  }

  function renderRegisters(trace) {
    clearNode(elements.registers);
    const ram = trace?.ram ?? Array.from({ length: 16 }, () => "XXXX");
    ram.forEach((value, address) => {
      const cell = document.createElement("div"); cell.className = "register";
      const label = document.createElement("b"); label.textContent = `R${address.toString(16).toUpperCase()}`;
      const output = document.createElement("output"); output.textContent = value;
      cell.append(label, output); elements.registers.append(cell);
    });
  }

  function renderAssertions() {
    clearNode(elements.assertions);
    const passed = state.assertionResults.filter(result => result.pass).length;
    const failed = state.assertionResults.length - passed;
    elements.assertionCount.textContent = state.assertionResults.length ? `${passed} pass · ${failed} fail` : "not run";
    elements.assertionCount.style.color = failed ? "var(--red)" : state.assertionResults.length ? "var(--green)" : "";

    if (!state.assertionResults.length) {
      const empty = document.createElement("p"); empty.className = "empty";
      empty.textContent = "Step or run the testbench to verify its expected outputs.";
      elements.assertions.append(empty); return;
    }

    for (const result of state.assertionResults.slice(-100).reverse()) {
      const row = document.createElement("div"); row.className = `assertion ${result.pass ? "pass" : "fail"}`;
      const mark = document.createElement("b"); mark.textContent = result.pass ? "PASS" : "FAIL";
      const detail = document.createElement("code"); detail.textContent = `${result.signal} = ${result.actual}`;
      const expected = document.createElement("small"); expected.textContent = `expected ${result.expected} · C${result.cycle}`;
      row.title = result.message; row.append(mark, detail, expected); elements.assertions.append(row);
    }
  }

  function decodedInstruction(trace) {
    if (!trace || trace.decoded.word === null) return "Instruction unavailable";
    const decoded = trace.decoded;
    return `I=${trace.I} · ${FUNCTION_LABELS[decoded.functionSelect]} · source ${SOURCE_LABELS[decoded.sourceSelect]} · ${DEST_LABELS[decoded.destinationSelect]}`;
  }

  function renderState() {
    const trace = state.traces[state.cursorCycle] ?? state.traces.at(-1);
    elements.cycleReadout.textContent = trace ? `cycle ${trace.cycle} · ${trace.time} ns` : "cycle —";
    elements.instruction.textContent = trace ? decodedInstruction(trace) : "Waiting for vectors";
    elements.timeReadout.textContent = trace ? `cursor ${trace.time} ns` : "0 ns";
    renderPorts(trace); renderRegisters(trace); renderAssertions();

    const passed = state.assertionResults.filter(result => result.pass).length;
    const failed = state.assertionResults.length - passed;
    elements.summary.textContent = `${state.nextCycle}/${state.vectors.length} cycles · ${passed} pass${failed ? ` · ${failed} fail` : ""}`;
    elements.downloadButton.disabled = !state.traces.length;
    elements.cursor.max = Math.max(0, state.traces.length - 1);
    elements.cursor.value = Math.min(state.cursorCycle, state.traces.length - 1);
    elements.cursor.disabled = !state.traces.length;
  }

  function renderSource() {
    const selected = elements.sourceSelect.value;
    elements.sourceTitle.textContent = SOURCE_NAMES[selected];
    if (elements.sourceCode.dataset.source !== selected) {
      elements.sourceCode.textContent = state.sources[selected] ?? "Loading…";
      elements.sourceCode.dataset.source = selected;
      elements.sourceCode.scrollTop = 0;
    }
    const trace = state.traces[state.cursorCycle];
    elements.sourceLocation.textContent = selected === "testbench" && trace ? `cycle ${trace.cycle} · line ${trace.sourceLine}` : selected === "algorithmic" ? "behavioral execution reference" : selected === "functional" ? "preserved equivalent architecture" : "original source";
    if (selected === "testbench" && trace) {
      requestAnimationFrame(() => {
        const lineCount = state.sources.testbench.split("\n").length;
        const ratio = Math.max(0, (trace.sourceLine - 8) / lineCount);
        elements.sourceCode.scrollTop = ratio * elements.sourceCode.scrollHeight;
      });
    } else elements.sourceCode.scrollTop = 0;
  }

  function initializeWaveLabels() {
    clearNode(elements.waveLabels);
    for (const signal of SIGNALS) {
      const label = document.createElement("div"); label.textContent = signal.name; elements.waveLabels.append(label);
    }
  }

  function logicLevel(value) {
    value = bit(value);
    return value === "1" ? 0 : value === "0" ? 1 : .5;
  }

  function drawWaveforms() {
    const canvas = elements.waveCanvas;
    const context = canvas.getContext("2d");
    const top = 30, rowHeight = 28, scale = state.scale;
    const width = Math.max(elements.waveScroll.clientWidth || 600, state.traces.length * scale + 42);
    const height = top + SIGNALS.length * rowHeight;
    canvas.width = width; canvas.height = height;
    canvas.style.width = `${width}px`; canvas.style.height = `${height}px`;
    context.fillStyle = "#050c11"; context.fillRect(0, 0, width, height);
    context.font = "10px Consolas, monospace"; context.textBaseline = "middle";

    context.strokeStyle = "#1c3039"; context.lineWidth = 1;
    for (let row = 0; row <= SIGNALS.length; row++) {
      const y = top + row * rowHeight + .5;
      context.beginPath(); context.moveTo(0, y); context.lineTo(width, y); context.stroke();
    }
    for (let index = 0; index <= state.traces.length; index++) {
      const x = index * scale + .5;
      context.strokeStyle = index % 5 === 0 ? "#28414c" : "#12252d";
      context.beginPath(); context.moveTo(x, top); context.lineTo(x, height); context.stroke();
      if (index < state.traces.length && (index % 5 === 0 || scale >= 38)) {
        context.fillStyle = "#708891"; context.fillText(`${state.traces[index].cycle * 10}ns`, x + 3, 14);
      }
    }

    SIGNALS.forEach((signal, row) => {
      const y = top + row * rowHeight;
      const key = signal.key ?? signal.name;
      if (signal.width === 1) {
        context.strokeStyle = key === "CLK" ? "#75bfff" : "#84e18d";
        context.lineWidth = 1.5; context.beginPath();
        state.traces.forEach((trace, index) => {
          const x0 = index * scale, x1 = x0 + scale / 2, x2 = x0 + scale;
          if (key === "CLK") {
            const high = y + 6, low = y + rowHeight - 6;
            if (!index) context.moveTo(x0, high); else context.lineTo(x0, high);
            context.lineTo(x1, high); context.lineTo(x1, low); context.lineTo(x2, low);
          } else {
            const value = trace[key];
            const level = y + 6 + logicLevel(value) * (rowHeight - 12);
            const previous = index ? y + 6 + logicLevel(state.traces[index - 1][key]) * (rowHeight - 12) : level;
            if (!index) context.moveTo(x0, level); else { context.lineTo(x0, previous); context.lineTo(x0, level); }
            context.lineTo(x2, level);
          }
        });
        context.stroke();
      } else {
        context.strokeStyle = "#57d3d3"; context.fillStyle = "#9ce7e7"; context.lineWidth = 1;
        state.traces.forEach((trace, index) => {
          const x = index * scale, value = trace[key] ?? "X".repeat(signal.width);
          context.strokeRect(x + .5, y + 5.5, scale, rowHeight - 11);
          const changed = !index || state.traces[index - 1][key] !== value;
          if ((scale >= 22 || changed) && context.measureText(value).width < scale - 4) context.fillText(value, x + 3, y + rowHeight / 2);
        });
      }
    });

    if (state.traces.length) {
      const cursorX = (state.cursorCycle + .5) * scale + .5;
      context.strokeStyle = "#f5ca72"; context.lineWidth = 1;
      context.beginPath(); context.moveTo(cursorX, 0); context.lineTo(cursorX, height); context.stroke();
      context.fillStyle = "#f5ca72"; context.beginPath(); context.moveTo(cursorX - 4, 0); context.lineTo(cursorX + 4, 0); context.lineTo(cursorX, 7); context.fill();
    }
  }

  function renderAll({ follow = false } = {}) {
    renderState(); drawWaveforms(); renderSource();
    if (follow && state.traces.length) {
      const target = Math.max(0, (state.cursorCycle + 1) * state.scale - elements.waveScroll.clientWidth + 60);
      elements.waveScroll.scrollLeft = target;
    }
  }

  function resetSimulator(message) {
    state.runToken++; state.running = false; state.machine = createMachine();
    state.traces = []; state.assertionResults = []; state.nextCycle = 0; state.cursorCycle = 0;
    elements.runButton.textContent = "▶ Run all"; elements.stepButton.disabled = false;
    elements.status.textContent = message ?? `Ready. ${state.vectors.length} preserved clock cycles loaded.`;
    renderAll();
  }

  function finishRun() {
    state.running = false; elements.runButton.textContent = "▶ Run all"; elements.stepButton.disabled = false;
    const failures = state.assertionResults.filter(result => !result.pass).length;
    elements.status.textContent = failures
      ? `Simulation complete with ${failures} assertion failure${failures === 1 ? "" : "s"}.`
      : `Simulation complete. All ${state.assertionResults.length} original assertions passed.`;
    renderAll({ follow: true });
  }

  function runAll() {
    if (state.running) {
      state.runToken++; state.running = false; elements.runButton.textContent = "▶ Continue"; elements.stepButton.disabled = false;
      elements.status.textContent = `Paused before cycle ${state.nextCycle}.`; return;
    }
    if (state.nextCycle >= state.vectors.length) resetSimulator();
    state.running = true; elements.runButton.textContent = "■ Pause"; elements.stepButton.disabled = true;
    const token = ++state.runToken;

    const chunk = () => {
      if (!state.running || token !== state.runToken) return;
      for (let count = 0; count < 12 && state.nextCycle < state.vectors.length; count++) executeOne();
      elements.status.textContent = `Running preserved testbench… cycle ${state.nextCycle}/${state.vectors.length}`;
      renderAll({ follow: true });
      if (state.nextCycle < state.vectors.length) requestAnimationFrame(chunk); else finishRun();
    };
    requestAnimationFrame(chunk);
  }

  function stepOne() {
    if (state.nextCycle >= state.vectors.length) { elements.status.textContent = "End of testbench reached. Reset to run it again."; return; }
    executeOne();
    const vectorData = state.vectors[state.nextCycle - 1];
    elements.status.textContent = `Executed falling edge for cycle ${vectorData.cycle}; checked ${vectorData.assertions.length} assertion${vectorData.assertions.length === 1 ? "" : "s"}.`;
    renderAll({ follow: true });
    if (state.nextCycle >= state.vectors.length) finishRun();
  }

  function vcdValue(value, width, id) {
    const normalized = String(value ?? "X").toLowerCase().replace(/[^01xz]/g, "x");
    return width === 1 ? `${normalized[0] ?? "x"}${id}` : `b${normalized.padStart(width, "x").slice(-width)} ${id}`;
  }

  function exportVcd() {
    if (!state.traces.length) return;
    const ids = Object.fromEntries(SIGNALS.map((signal, index) => [signal.key ?? signal.name, `s${index}`]));
    const lines = [
      "$date", `  ${new Date().toISOString()}`, "$end", "$version", "  AM2901 browser source model", "$end",
      "$timescale 1ns $end", "$scope module E $end", "$scope module AM1 $end"
    ];
    SIGNALS.forEach(signal => lines.push(`$var wire ${signal.width} ${ids[signal.key ?? signal.name]} ${signal.name.replace(/[^A-Za-z0-9_]/g, "_")} $end`));
    lines.push("$upscope $end", "$upscope $end", "$enddefinitions $end", "$dumpvars");
    SIGNALS.forEach(signal => lines.push(vcdValue("X".repeat(signal.width), signal.width, ids[signal.key ?? signal.name])));
    lines.push("$end");

    state.traces.forEach(trace => {
      const start = trace.cycle * 10;
      lines.push(`#${start}`, vcdValue("1", 1, ids.CLK));
      ["I", "Aadd", "Badd", "D"].forEach(key => {
        const signal = SIGNALS.find(item => (item.key ?? item.name) === key);
        lines.push(vcdValue(trace[key], signal.width, ids[key]));
      });
      lines.push(`#${trace.time}`, vcdValue("0", 1, ids.CLK));
      ["Y", "Q", "C4", "Gbar", "Pbar", "OVR", "F3", "F30"].forEach(key => {
        const signal = SIGNALS.find(item => (item.key ?? item.name) === key);
        lines.push(vcdValue(trace[key], signal.width, ids[key]));
      });
    });

    const url = URL.createObjectURL(new Blob([lines.join("\n") + "\n"], { type: "text/plain" }));
    const link = document.createElement("a"); link.href = url; link.download = "am2901-testbench.vcd";
    document.body.append(link); link.click(); link.remove(); setTimeout(() => URL.revokeObjectURL(url), 1000);
  }

  function setCursor(index) {
    if (!state.traces.length) return;
    state.cursorCycle = Math.max(0, Math.min(state.traces.length - 1, Number(index)));
    renderAll();
  }

  function setZoom(value) {
    state.scale = Math.max(10, Math.min(54, Number(value)));
    elements.zoom.value = state.scale; drawWaveforms();
  }

  function bindEvents() {
    elements.resetButton.addEventListener("click", () => resetSimulator());
    elements.runButton.addEventListener("click", runAll);
    elements.stepButton.addEventListener("click", stepOne);
    elements.downloadButton.addEventListener("click", exportVcd);
    elements.sourceSelect.addEventListener("change", renderSource);
    elements.zoom.addEventListener("input", event => setZoom(event.target.value));
    elements.zoomOut.addEventListener("click", () => setZoom(state.scale - 4));
    elements.zoomIn.addEventListener("click", () => setZoom(state.scale + 4));
    elements.cursor.addEventListener("input", event => setCursor(event.target.value));
    elements.waveCanvas.addEventListener("click", event => setCursor(Math.floor(event.offsetX / state.scale)));
    elements.waveCanvas.addEventListener("keydown", event => {
      if (event.key === "ArrowLeft" || event.key === "ArrowRight") {
        event.preventDefault(); setCursor(state.cursorCycle + (event.key === "ArrowRight" ? 1 : -1));
      }
    });
    window.addEventListener("resize", () => requestAnimationFrame(drawWaveforms));
  }

  async function loadSources() {
    elements.resetButton.disabled = elements.runButton.disabled = elements.stepButton.disabled = true;
    try {
      const entries = await Promise.all(Object.entries(PATHS).map(async ([key, path]) => {
        const response = await fetch(path, { cache: "no-store" });
        if (!response.ok) throw new Error(`${response.status} while loading ${path}`);
        return [key, await response.text()];
      }));
      state.sources = Object.fromEntries(entries);
      state.vectors = parseVectors(state.sources.testbench);
      if (!state.vectors.length) throw new Error("No clock cycles were found in test_vectors_2901.vhdl");
      elements.sourceCode.dataset.source = "";
      elements.resetButton.disabled = elements.runButton.disabled = elements.stepButton.disabled = false;
      resetSimulator(`Ready. Parsed ${state.vectors.length} cycles directly from the preserved VHDL testbench.`);
    } catch (error) {
      elements.status.textContent = `Unable to load the lab: ${error.message}. Serve the site over HTTP; file:// is unsupported.`;
      elements.summary.textContent = "load failed";
      elements.sourceCode.textContent = String(error.stack || error);
    }
  }

  initializeWaveLabels(); bindEvents(); renderPorts(); renderRegisters(); drawWaveforms(); loadSources();
})();