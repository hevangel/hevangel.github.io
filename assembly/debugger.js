(() => {
'use strict';
const MODULES = {
  main:{label:'MAIN.ASM',file:'MAIN'}, calc:{label:'CALC.ASM',file:'CALC'}, input:{label:'INPUT.ASM',file:'INPUT'}, parallel:{label:'PARALLEL.ASM',file:'PARALLEL'}, op:{label:'OP.ASM',file:'OP'}, util:{label:'UTIL.ASM',file:'UTIL'}, inche:{label:'INCHE.ASM',file:'INCHE'}, out:{label:'OUT.ASM',file:'OUT'}, paratest:{label:'PARATEST.ASM',file:'PARATEST'}, test:{label:'TEST.ASM',file:'TEST'}, test2:{label:'TEST2.ASM',file:'TEST2'}
};
const BASE = {main:0x2000,calc:0x2500,input:0x3000,parallel:0x3200,out:0x3400,op:0x3500,util:0x3600,inche:0x3700};
const MODEL_LINES = {
  main:[70,71,73,74,76,78,80,81,82,83,84,85,86,87,89,90,92,93,94,95,97,98,99,114,116,118,119,120,121,123,125],
  calc:[65,66,67,69,71,72,73,75,78,79,80,82,83,84,85,86,87,88,90,91,92,94,96,97,98,99,101,102,103,105,108,109,110,111,113,114,115,116,117,118,120,121,122,123,124,125,126,127,128,129,130,131,133,134,135,137,138,140,141,142,144,145,146,147,148,149,150,151,152,154,155,156,157,158,159,160,161,162,163,164,166,168,169,170,171,173,175,176,177,179,180,182,183,185,186,188,189,190,192,194,195],
  input:[24,25,28,29,30,32,34,35,37,38,40,41,42,43,44,45,46,47,48,49,51,52,54,55,56,57,59,60,62,63,64,65,67,68,70,71,72,73,75,77,78,80,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,100,101],
  parallel:[43,45,46,47,48,49,50,52,53,54,56,57,58,59,61,62,63,64,66,67,69,70,72,73,74,76,77,78,79,81,82],
  op:[20,21,35,36,50,51,66,67,68,69,70,71,72], util:[21,22,23,24,25,27,28,29,30,31,32,34,36,37,38,40,41,43,44,45,47,48,50,52,53,67,68,69,70,72,73,74,75,76,78,79,80,82,83,85,86,87,89,90,91,92,94,95,96,97,99,100,102,103,104,105,106],
  inche:[23,24,25,26,27,28,29,30,31,33,34], out:[31,32,34,36,37,38,39,41,42,64,65,67,68,70,71,72,74,75,95,96,98,100,101,103,104,106,107]
};
const $ = selector => document.querySelector(selector);
const dom = {run:$('#runButton'),step:$('#stepButton'),reset:$('#resetButton'),module:$('#moduleSelect'),speed:$('#speedSelect'),follow:$('#followToggle'),clear:$('#clearBreakpoints'),source:$('#sourceCode'),sourceTitle:$('#sourceTitle'),asm:$('#asmLink'),lst:$('#lstLink'),terminal:$('#terminal'),terminalState:$('#terminalState'),keypad:$('#keypad'),registers:$('#registers'),programState:$('#programState'),count:$('#instructionCount'),status:$('#status'),location:$('#location')};
const sourceCache = new Map();
const breakpoints = new Set();
let selectedModule = 'main', renderToken = 0;
let regs, memory, calc, terminalText, inputQueue, queue, running, waiting, halted, instructionCount, changed, timer, currentKey;
const menuText='\r\n\r\nMenu\r\nBy Horace Chan & Grace Du\r\n\r\n1.  Enter Calculater Mode:\r\n2.  Enter Clock Set Mode:\r\n3.  Clock Display:\r\n4.  Return to TUTOR.\r\n\r\nPlease enter the mode you would like to choose,\r\nend it with <Enter>: ';
const errorText='\r\n\r\nSorry, Functions for Clock Set and Clock Display\r\nare not available at the moment.\r\nPlease try another mode. ';
const titleText='\r\n\r\nE&CE 222 Calculator\r\nby Y.Chan & X.Du\r\n';
const hex=(value,width=8)=>(value>>>0).toString(16).toUpperCase().padStart(width,'0');
const signed=value=>value|0;
const keyFor=(module,line)=>`${module}:${line}`;
const escapeHtml=value=>value.replace(/[&<>]/g,char=>({'&':'&amp;','<':'&lt;','>':'&gt;'}[char]));
function modelAddress(module,line){return (BASE[module]||0x4000)+(line*2);}
function setReg(name,value){value=value>>>0;if(regs[name]!==value){regs[name]=value;changed.add(name);}}
function setTerminal(text){terminalText+=text.replace(/\r\n/g,'\n').replace(/\r/g,'\n');dom.terminal.textContent=terminalText;dom.terminal.scrollTop=dom.terminal.scrollHeight;}
function point(module,line,asm,action={},options={}){return {module,line,asm,action,address:options.address??modelAddress(module,line),wait:options.wait||false};}
function schedule(...points){queue.unshift(...points.flat());}
function setStatus(message){dom.status.textContent=message;}
function currentPoint(){return queue[0]||null;}
async function loadSource(module,scroll=true){
  selectedModule=module;dom.module.value=module;const meta=MODULES[module];dom.sourceTitle.textContent=meta.label;dom.asm.href=`/assembly/${meta.file}.ASM`;dom.lst.href=`/assembly/${meta.file}.LST`;const token=++renderToken;
  try{if(!sourceCache.has(module)){const response=await fetch(`/assembly/${meta.file}.ASM`);if(!response.ok)throw new Error(`HTTP ${response.status}`);sourceCache.set(module,await response.text());}if(token!==renderToken)return;renderSource(module,sourceCache.get(module),scroll);}catch(error){dom.source.innerHTML=`<p class="source-error">Could not load ${meta.label}: ${escapeHtml(error.message)}</p>`;}
}
function renderSource(module,text,scroll=true){
  const active=currentPoint();const modeled=new Set(MODEL_LINES[module]||[]);dom.source.innerHTML=text.replace(/\r/g,'').split('\n').map((source,index)=>{const line=index+1;const id=keyFor(module,line);const classes=['source-line'];if(modeled.has(line))classes.push('modeled');if(breakpoints.has(id))classes.push('breakpoint');if(active&&active.module===module&&active.line===line)classes.push('current');const comment=source.trim().startsWith('*')?' comment':'';return `<div class="${classes.join(' ')}" data-line="${line}" role="${modeled.has(line)?'button':'presentation'}" ${modeled.has(line)?'tabindex="0"':''}><span class="break-gutter">●</span><span class="line-number">${line}</span><span class="source-text${comment}">${escapeHtml(source)||' '}</span></div>`;}).join('');
  if(scroll&&active&&active.module===module)requestAnimationFrame(()=>dom.source.querySelector('.current')?.scrollIntoView({block:'center'}));
}
function refreshSource(scroll=true){if(sourceCache.has(selectedModule))renderSource(selectedModule,sourceCache.get(selectedModule),scroll);}
function followCurrent(){const active=currentPoint();if(!active)return;if(dom.follow.checked&&active.module!==selectedModule){loadSource(active.module,true);}else refreshSource(true);}
function renderRegisters(){
  const names=['D0','D1','D2','D3','D4','D5','D6','D7','A0','A1','A2','A3','A4','A5','A6','A7'];dom.registers.innerHTML=names.map(name=>`<span class="register${changed.has(name)?' changed':''}"><strong>${name}</strong><span>${hex(regs[name])}</span></span>`).join('');
  const active=currentPoint();const pc=active?active.address:regs.PC;dom.programState.innerHTML=`<span>PC <b>$${hex(pc,6)}</b></span><span>SR <b>$${hex(regs.SR,4)}</b></span><span>RESULT <b>${signed(memory.RESULT)}</b></span><span>STATE <b>${memory.STATE}</b></span><span>OPR <b>${memory.OPR}</b></span><span>BUF <b>${escapeHtml(calc.buffer||'—')}</b></span><span>ACIA <b>${waiting?'WAIT':'READY'}</b></span><span>LINK <b>MODEL</b></span>`;
  dom.count.textContent=`${instructionCount} instruction${instructionCount===1?'':'s'}`;dom.location.textContent=active?`${MODULES[active.module].label}:${active.line} · PC $${hex(active.address,6)}`:'Execution complete';dom.terminalState.textContent=halted?'HALTED':waiting?'WAITING':running?'RUNNING':'PAUSED';dom.run.textContent=running?'Ⅱ Pause':'▶ Run';
}
function renderAll(scroll=true){renderRegisters();followCurrent();}
function toggleBreakpoint(line){if(!(MODEL_LINES[selectedModule]||[]).includes(line))return;const id=keyFor(selectedModule,line);breakpoints.has(id)?breakpoints.delete(id):breakpoints.add(id);refreshSource(false);setStatus(`${breakpoints.has(id)?'Set':'Cleared'} breakpoint at ${MODULES[selectedModule].label}:${line}.`);}
function outputFlow(text){return [
  point('out',64,'OUTPUT  movem.l D0-D7/A1-A4,-(A7)',()=>setReg('A7',regs.A7-48)),
  point('out',65,'move.b  #$15,ACIASC'),point('out',67,'STROUT  cmp.l A6,A5'),
  point('out',70,'move.b  (A5)+,D0',()=>{if(text.length)setReg('D0',text.charCodeAt(text.length-1));setReg('A5',regs.A6);setTerminal(text);}),
  point('out',71,'bsr OUTCH'),point('out',74,'RETURN  movem.l (A7)+,D0-D7/A1-A4',()=>setReg('A7',regs.A7+48)),point('out',75,'rts')
];}
function outCharFlow(char){return [point('out',31,'OUTCH  movem.l D1-D7/A1-A6,-(A7)',()=>setReg('A7',regs.A7-52)),point('out',32,'move.b #$15,ACIASC'),point('out',34,'CKTDRE move.b ACIASC,D1',()=>setReg('D1',2)),point('out',36,'andi.b #$02,D1'),point('out',37,'cmp.b #$0,D1'),point('out',39,'move.b D0,DATARX',()=>setTerminal(char)),point('out',41,'movem.l (A7)+,D1-D7/A1-A6',()=>setReg('A7',regs.A7+52)),point('out',42,'rts')];}
function getModeFlow(){return [
  point('main',114,'GETMODE clr D0',()=>setReg('D0',0),{address:0x206A}),
  point('main',116,'jsr INCHE',()=>schedule(...incheFlow()),{address:0x206C})
];}
function incheFlow(){return [
  point('inche',23,'INCHE movem.l D1,-(SP)',()=>setReg('A7',regs.A7-4)),point('inche',24,'clr.l D0',()=>setReg('D0',0)),point('inche',25,'move.b #$15,ACIAREG'),
  point('inche',26,'LOOP move.b ACIAREG,D1',()=>setReg('D1',1),{wait:true}),point('inche',27,'andi.b #$01,D1'),point('inche',28,'cmp.b #$0,D1'),point('inche',29,'beq LOOP'),
  point('inche',30,'move.b ACIADATA,D0',()=>{currentKey=inputQueue.shift();setReg('D0',currentKey.charCodeAt(0));}),point('inche',31,'andi.b #%01111111,D0',()=>setReg('D0',regs.D0&0x7f)),point('inche',33,'movem.l (SP)+,D1',()=>setReg('A7',regs.A7+4)),point('inche',34,'rts',()=>schedule(...menuValidationFlow()))
];}
function menuValidationFlow(){return [
  point('main',118,"cmp.b #'4',D0",null,{address:0x2072}),point('main',119,'bgt GETMODE',null,{address:0x2076}),point('main',120,"cmp.b #'1',D0",null,{address:0x2078}),
  point('main',121,'blt GETMODE',()=>{const value=String.fromCharCode(regs.D0&0xff);if(value<'1'||value>'4'){schedule(...getModeFlow());return;}schedule(point('main',123,'jsr OUTCH',()=>schedule(...outCharFlow(value)),{address:0x207E}),point('main',125,'rts',()=>dispatchMenu(value),{address:0x2084}));},{address:0x207C})
];}
function showMenuFlow(){return [
  point('main',73,'LOOP lea PROMPT,A5',()=>setReg('A5',0x4000),{address:0x200C}),point('main',74,'lea EPROMPT,A6',()=>setReg('A6',0x40D7),{address:0x2012}),
  point('main',76,'jsr OUTPUT',()=>schedule(...outputFlow(menuText)),{address:0x2018}),point('main',78,'bsr GETMODE',()=>schedule(...getModeFlow()),{address:0x201E})
];}
function dispatchMenu(value){
  if(value==='1')schedule(point('main',80,"cmp.b #'1',D0",null,{address:0x2022}),point('main',81,'beq CAL',null,{address:0x2026}),point('main',89,'CAL jsr CALC',()=>schedule(...startCalculatorFlow()),{address:0x2042}));
  else if(value==='2'||value==='3')schedule(point('main',value==='2'?82:84,`cmp.b #'${value}',D0`,null,{address:value==='2'?0x202A:0x2032}),point('main',value==='2'?83:85,'beq PRTERR',null,{address:value==='2'?0x202E:0x2036}),point('main',92,'PRTERR lea ERRMSG,A5',()=>setReg('A5',0x40D7),{address:0x204A}),point('main',93,'lea ENDMSG,A6',()=>setReg('A6',0x414C),{address:0x2050}),point('main',94,'jsr OUTPUT',()=>schedule(...outputFlow(errorText)),{address:0x2056}),point('main',95,'bra LOOP',()=>schedule(...showMenuFlow()),{address:0x205C}));
  else schedule(point('main',86,"cmp.b #'4',D0",null,{address:0x203A}),point('main',87,'beq EXIT',null,{address:0x203E}),point('main',97,'EXIT move.l TUTSP,A7',()=>setReg('A7',memory.TUTSP),{address:0x205E}),point('main',98,'move.b #TUTOR,D7',()=>setReg('D7',228),{address:0x2064}),point('main',99,'trap #14',()=>{halted=true;running=false;setTerminal('\r\n\r\nReturned to TUTOR monitor.\r\n> ');setStatus('Program returned to the TUTOR monitor through TRAP #14.');},{address:0x2068}));
}
function startCalculatorFlow(){return [
  point('calc',65,'CALC movem.l D0-D7/A0-A6,-(SP)',()=>setReg('A7',regs.A7-60),{address:0x2500}),point('calc',66,'lea TITLE,A5',()=>setReg('A5',0x4500),{address:0x2504}),point('calc',67,'lea ETITLE,A6',()=>setReg('A6',0x4529),{address:0x250A}),
  point('calc',69,'jsr OUT1CR',()=>schedule(...outputFlow(titleText)),{address:0x2510}),point('calc',71,'clr.l RESULT',()=>memory.RESULT=0,{address:0x2516}),point('calc',72,'clr.b STATE',()=>memory.STATE=0,{address:0x251C}),point('calc',73,'clr.b OPR',()=>{memory.OPR=0;schedule(...calculatorDisplayFlow());},{address:0x2522})
];}
function calculatorDisplayFlow(){
  const result=signed(memory.RESULT);const flow=[point('calc',75,'main_loop cmp.b #1,STATE',null,{address:0x2528}),point('calc',78,'dis_result lea SIGNEQ,A5',()=>setReg('A5',0x452E),{address:0x2534}),point('calc',79,'lea ESIGNEQ,A6',()=>setReg('A6',0x4532),{address:0x253A}),point('calc',80,"jsr OUTPUT ; '= '",()=>schedule(...outputFlow('\r\n= ')),{address:0x2540}),point('calc',82,'move.l RESULT,D0',()=>setReg('D0',result),{address:0x2546}),point('calc',83,'cmp.l #9999,D0',null,{address:0x254C}),point('calc',85,'cmp.l #-9999,D0',null,{address:0x2556})];
  if(result>9999||result<-9999){flow.push(point('calc',101,'prn_err move.b #3,STATE',()=>memory.STATE=3,{address:0x2594}),point('calc',102,'lea ERROR,A5',()=>setReg('A5',0x453E),{address:0x259C}),point('calc',103,'lea EERROR,A6',()=>setReg('A6',0x4543),{address:0x25A2}),point('calc',105,'prn_end jsr OUTPUT',()=>schedule(...outputFlow('Error')),{address:0x25A8}));}
  else{flow.push(point('calc',87,'cmp.l #0,D0',null,{address:0x2560}));if(result<0)flow.push(point('calc',90,'lea SIGNNEG,A5',()=>setReg('A5',0x4529),{address:0x256A}),point('calc',91,'lea ESIGNNEG,A6',()=>setReg('A6',0x452A),{address:0x2570}),point('calc',92,"jsr OUTPUT ; '-'",()=>schedule(...outputFlow('-')),{address:0x2576}),point('calc',94,'neg.l D0',()=>setReg('D0',Math.abs(result)),{address:0x257C}));flow.push(point('calc',96,'prn_result move.l #BUFFER,A5',()=>setReg('A5',0x5500),{address:0x257E}),point('calc',97,'move.l #BUFFER,A6',()=>setReg('A6',0x5500),{address:0x2584}),point('calc',98,'jsr HEX2DEC',()=>schedule(...hex2decFlow(Math.abs(result))),{address:0x258A}),point('calc',99,'bra prn_end',null,{address:0x2590}),point('calc',105,'prn_end jsr OUTPUT',()=>schedule(...outputFlow(String(Math.abs(result)))),{address:0x25A8}));}
  flow.push(point('calc',108,'cmp.b #0,STATE',null,{address:0x25AE}),point('calc',110,'cmp.b #3,STATE',()=>{if(memory.STATE!==0&&memory.STATE!==3)schedule(...operatorDisplayFlow());else schedule(...beginInputFlow());},{address:0x25BA}));return flow;
}
function hex2decFlow(value){const digits=String(value);return [point('util',67,'HEX2DEC movem.l D0-D3,-(SP)',()=>setReg('A7',regs.A7-16)),point('util',68,'move.l #1,D1',()=>setReg('D1',digits.length)),point('util',69,'h2d_loop cmp.l #10,D0'),point('util',89,'h2d_lt16b divu #10,D3',()=>setReg('D3',value%10)),point('util',99,'end_h2d_loop add.l #48,D0'),point('util',102,'h2d_loop2 move.b (SP)+,(A6)+',()=>setReg('A6',0x5500+digits.length)),point('util',105,'movem.l (SP)+,D0-D3',()=>setReg('A7',regs.A7+16)),point('util',106,'rts')];}
function operatorDisplayFlow(){const config={1:[120,121,' + ',0x4532,0x4535],2:[123,124,' - ',0x4535,0x4538],3:[126,127,' * ',0x4538,0x453B],4:[129,130,' / ',0x453B,0x453E]}[memory.OPR]||[120,121,' + ',0x4532,0x4535];return [point('calc',113,'end_result cmp.b #4,OPR',null,{address:0x25C6}),point('calc',config[0],`lea SIGN${config[2].trim()},A5`,()=>setReg('A5',config[3])),point('calc',config[1],'lea end sign,A6',()=>setReg('A6',config[4])),point('calc',131,'dis_op jsr OUTPUT',()=>schedule(...outputFlow(config[2])),{address:0x2626}),...beginInputFlow()];}
function beginInputFlow(){return [point('calc',133,'end_dis move.l #BUFFER,A5',()=>{calc.buffer='';setReg('A5',0x5500);},{address:0x262C}),point('calc',134,'move.l #BUFFER,A6',()=>setReg('A6',0x5500),{address:0x2632}),point('calc',135,'jsr INPUT',()=>schedule(...inputStartFlow()),{address:0x2638})];}
function inputStartFlow(){return [point('input',24,'INPUT movem.l A0/D1/D2/D7,-(SP)',()=>setReg('A7',regs.A7-16)),point('input',25,'clr.l D2',()=>setReg('D2',0)),point('input',28,'cmp.b #1,STATE'),point('input',29,'bne input_loop'),...(memory.STATE===1?[point('input',30,'move.b #2,STATE',()=>memory.STATE=2)]:[]),point('input',32,'input_loop jsr PARALLEL',()=>schedule(...parallelFlow()))];}
function parallelFlow(){return [
  point('parallel',43,'PARALLEL movem.l A0/D1-D3,-(SP)',()=>setReg('A7',regs.A7-16)),point('parallel',45,'clr.l D0',()=>setReg('D0',0)),point('parallel',46,'move.l #CHAR,A0',()=>setReg('A0',0x3200)),point('parallel',47,'move.b #$0,PGCR'),point('parallel',48,'move.b #$80,PBDDR'),point('parallel',49,'move.b #$F0,PBDDR'),point('parallel',50,'move.b #$0,PBDR'),
  point('parallel',52,'PRESSLOOP move.b PBDR,D1',()=>setReg('D1',0x07),{wait:true}),point('parallel',53,'cmp.b #$0F,D1'),point('parallel',56,'move #DELAY,D3',()=>setReg('D3',0xA0)),point('parallel',57,'DELAYLOOP subq #1,D3',()=>setReg('D3',0)),point('parallel',61,'move.b PBDR,D1'),point('parallel',62,'move.b #$0F,PBDDR'),point('parallel',63,'move.b #$0,PBDR'),point('parallel',64,'move.b PBDR,D2',()=>setReg('D2',0x70)),point('parallel',66,'or.b D2,D1',()=>setReg('D1',(regs.D1|regs.D2)&0xff)),point('parallel',67,'subi.b #$77,D1',()=>setReg('D1',(regs.D1-0x77)&0xff)),point('parallel',69,'and.l #$000000FF,D1',()=>setReg('D1',regs.D1&0xff)),
  point('parallel',70,'move.b (A0,D1),D0',()=>{currentKey=inputQueue.shift();setReg('D0',currentKey.charCodeAt(0));}),point('parallel',72,'RELEASELOOP move.b PBDR,D2',()=>setReg('D2',0xF0)),point('parallel',73,'cmp.b #$F0,D2'),point('parallel',76,'move #DELAY,D3',()=>setReg('D3',0xA0)),point('parallel',77,'DELAYLOOP2 subq #1,D3',()=>setReg('D3',0)),point('parallel',81,'input_rts movem.l (SP)+,A0/D1-D3',()=>setReg('A7',regs.A7+16)),point('parallel',82,'rts',()=>schedule(...classifyInputFlow(currentKey)))
];}
function classifyInputFlow(char){const comparisons=[point('input',34,"cmp.b #$43,D0"),point('input',37,'cmp.b #3,STATE'),point('input',40,"cmp.b #$2B,D0"),point('input',42,"cmp.b #$2D,D0"),point('input',44,"cmp.b #$2A,D0"),point('input',46,"cmp.b #$2F,D0"),point('input',48,'cmp.b #$0D,D0')];
  if(char==='C')return [comparisons[0],point('input',35,'beq in_clr'),...finishInputFlow(5,90)];
  if(memory.STATE===3)return [comparisons[0],comparisons[1],point('input',38,'beq input_loop',()=>schedule(point('input',32,'input_loop jsr PARALLEL',()=>schedule(...parallelFlow()))))];
  const operators={'+':[1,82],'-':[2,84],'*':[3,86],'/':[4,88]};if(operators[char]){const [code,line]=operators[char];return [...comparisons.slice(0,{'+':3,'-':4,'*':5,'/':6}[char]),point('input',line,`move #${code},D0`,()=>setReg('D0',code)),...finishInputFlow(code,line+1)];}
  if(char==='\r')return [...comparisons,point('input',92,'in_ent cmp.b #0,STATE'),point('input',94,'cmp #0,D2'),...(memory.STATE===0&&!calc.buffer?[point('input',96,'move #6,D0',()=>setReg('D0',6)),...finishInputFlow(6,97)]:[point('input',98,'in_ent2 move #0,D0',()=>setReg('D0',0)),...finishInputFlow(0,100)])];
  if(!/^[0-9]$/.test(char))return [...comparisons,point('input',56,'cmp.b #48,D0'),point('input',57,'blt input_loop',()=>schedule(point('input',32,'input_loop jsr PARALLEL',()=>schedule(...parallelFlow()))))];
  const digitFlow=[...comparisons,point('input',51,'cmp #4,D2'),point('input',54,'cmp.b #57,D0'),point('input',56,'cmp.b #48,D0')];if(calc.buffer.length>=4){digitFlow.push(point('input',52,'bge input_loop',()=>schedule(point('input',32,'input_loop jsr PARALLEL',()=>schedule(...parallelFlow())))));return digitFlow;}
  if(memory.STATE===0)digitFlow.push(point('input',59,'cmp.b #0,STATE'),point('input',62,'movem.l A5/A6,-(SP)',()=>setReg('A7',regs.A7-8)),point('input',63,'lea SIGNSPACE,A5',()=>setReg('A5',0x452A)),point('input',64,'lea ESIGNSPACE,A6',()=>setReg('A6',0x452E)),point('input',65,'jsr OUTPUT',()=>schedule(...outputFlow('\r\n  '))),point('input',67,'move.b #1,STATE',()=>memory.STATE=1),point('input',68,'movem.l (SP)+,A5/A6',()=>{setReg('A7',regs.A7+8);setReg('A5',0x5500);setReg('A6',0x5500+calc.buffer.length);}));
  digitFlow.push(point('input',70,'check_zero cmp.b #48,D0'));if(char!=='0'||calc.buffer.length>0)digitFlow.push(point('input',75,'add_count addq #1,D2',()=>setReg('D2',regs.D2+1)));digitFlow.push(point('input',77,'prn_ch move.b D0,(A6)+',()=>{calc.buffer+=char;setReg('A6',0x5500+calc.buffer.length);}),point('input',78,'jsr OUTCH',()=>schedule(...outCharFlow(char))),point('input',80,'bra input_loop',()=>schedule(point('input',32,'input_loop jsr PARALLEL',()=>schedule(...parallelFlow())))));return digitFlow;
}
function finishInputFlow(code,line){return [point('input',line,`bra input_rts ; code ${code}`),point('input',100,'input_rts movem.l (SP)+,A0/D1/D2/D7',()=>setReg('A7',regs.A7+16)),point('input',101,'rts',()=>schedule(...calculatorAfterInputFlow(code)))];}
function getnumFlow(value){return [point('util',21,'GETNUMD movem.l D6/D7,-(SP)',()=>setReg('A7',regs.A7-8)),point('util',22,'clr.l D1',()=>setReg('D1',0)),point('util',23,'gnd_loop clr.l D7',()=>setReg('D7',0)),point('util',27,'move.b (A5)+,D7',()=>setReg('D7',value%10)),point('util',47,'gnd_lt16b mulu #10,D1',()=>setReg('D1',value)),point('util',52,'gnd_rts movem.l (SP)+,D6/D7',()=>setReg('A7',regs.A7+8)),point('util',53,'rts')];}
function calculatorAfterInputFlow(code){
  const hasNumber=calc.buffer.length>0;const value=hasNumber?Number(calc.buffer):0;const flow=[point('calc',137,'cmpa.l A5,A6',null,{address:0x263E}),point('calc',138,'beq do_none',null,{address:0x2640})];
  if(hasNumber){flow.push(point('calc',140,'jsr GETNUMD',()=>schedule(...getnumFlow(value)),{address:0x2644}),point('calc',141,'move.l RESULT,D7',()=>setReg('D7',memory.RESULT),{address:0x264A}),point('calc',142,'move.l D1,D6',()=>setReg('D6',value),{address:0x2650}),point('calc',144,'cmp.b #1,OPR',null,{address:0x2652}),point('calc',146,'cmp.b #2,OPR',null,{address:0x265E}),point('calc',148,'cmp.b #3,OPR',null,{address:0x266A}),point('calc',150,'cmp.b #4,OPR',()=>{calc.returnCode=code;if(memory.OPR)schedule(...operationFlow(memory.OPR));else schedule(...afterOperationFlow(code,value));},{address:0x2676}));}else flow.push(...doNoneFlow(code,value));return flow;
}
function operationFlow(operator){
  const config={1:[154,'L2ADD',20],2:[156,'L2SUB',35],3:[158,'L2MUL',50],4:[164,'L2DIV',66]}[operator];
  const calcAddress={1:0x2686,2:0x2690,3:0x269A,4:0x26B6}[operator];
  const flow=[];
  if(operator===4)flow.push(point('calc',160,'do_div cmp.b #0,D6',null,{address:0x26A4}),point('calc',161,'bne no_zero',null,{address:0x26A8}));
  if(operator===4&&signed(regs.D6)===0){
    flow.push(point('calc',162,'move.l #10000,D7',()=>setReg('D7',10000),{address:0x26AC}),point('calc',163,'bra end_do',()=>schedule(...afterOperationFlow(calc.returnCode,Number(calc.buffer))),{address:0x26B2}));
    return flow;
  }
  flow.push(point('calc',config[0],`jsr ${config[1]}`,()=>{
    schedule(
      point('op',config[2],`${config[1]} arithmetic`,()=>{const a=signed(regs.D7),b=signed(regs.D6);const answer=operator===1?a+b:operator===2?a-b:operator===3?Math.imul(a,b):Math.trunc(a/b);setReg('D7',answer);}),
      point('op',config[2]+1,'rts'),
      point('calc',166,'end_do move.l D7,RESULT',()=>{memory.RESULT=regs.D7;schedule(...doNoneFlow(calc.returnCode,Number(calc.buffer)));},{address:0x26BC})
    );
  },{address:calcAddress}));
  return flow;
}
function afterOperationFlow(code,value){return [point('calc',166,'end_do move.l D7,RESULT',()=>{memory.RESULT=regs.D7;schedule(...doNoneFlow(code,value));},{address:0x26BC})];}
function doNoneFlow(code,value){
  const flow=[point('calc',168,'do_none cmp #6,D0',null,{address:0x26C2})];
  if(code===6){flow.push(point('calc',169,'beq calc_rts',null,{address:0x26C6}),point('calc',194,'calc_rts movem.l (SP)+,D0-D7/A0-A6',()=>setReg('A7',regs.A7+60),{address:0x2724}),point('calc',195,'rts',()=>schedule(point('main',90,'bra LOOP',()=>schedule(...showMenuFlow()),{address:0x2048})),{address:0x2728}));return flow;}
  flow.push(point('calc',170,'cmp #5,D0',null,{address:0x26CA}));if(code===5){flow.push(point('calc',171,'beq is_clr',null,{address:0x26CE}),point('calc',188,'is_clr clr.l RESULT',()=>memory.RESULT=0,{address:0x270A}),point('calc',189,'move.b #0,STATE',()=>memory.STATE=0,{address:0x2710}),point('calc',190,'move.b #0,OPR',()=>memory.OPR=0,{address:0x2718}),point('calc',192,'bra main_loop',()=>schedule(...calculatorDisplayFlow()),{address:0x2720}));return flow;}
  flow.push(point('calc',173,'move.b D0,OPR',()=>memory.OPR=code,{address:0x26D2}),point('calc',175,'cmp.b #1,STATE',null,{address:0x26D8}));if(memory.STATE===1)flow.push(point('calc',177,'move.l D1,RESULT',()=>memory.RESULT=value,{address:0x26E4}));flow.push(point('calc',179,'not_first cmp.b #0,D0',null,{address:0x26EA}));if(code===0)flow.push(point('calc',180,'beq is_ent',null,{address:0x26EE}),point('calc',185,'is_ent move.b #0,STATE',()=>memory.STATE=0,{address:0x26FE}),point('calc',186,'bra main_loop',()=>schedule(...calculatorDisplayFlow()),{address:0x2706}));else flow.push(point('calc',182,'move.b #2,STATE',()=>memory.STATE=2,{address:0x26F2}),point('calc',183,'bra main_loop',()=>schedule(...calculatorDisplayFlow()),{address:0x26FA}));return flow;
}
function canExecute(active){return !(active.wait&&inputQueue.length===0);}
function stepOne(ignoreBreakpoint=false){
  if(halted){setStatus('The program is halted. Reset to run it again.');return false;}const active=currentPoint();if(!active){running=false;setStatus('No more modeled instructions. Reset to restart.');renderAll();return false;}
  const breakpoint=breakpoints.has(keyFor(active.module,active.line));if(breakpoint&&!ignoreBreakpoint){running=false;setStatus(`Breakpoint hit at ${MODULES[active.module].label}:${active.line}.`);renderAll();return false;}
  if(!canExecute(active)){waiting=true;setStatus(active.module==='inche'?'Waiting for menu input on the ACIA. Press 1–4.':'Waiting for a key on the 4×4 parallel keypad.');renderAll(false);return false;}
  queue.shift();waiting=false;changed=new Set();regs.PC=active.address>>>0;if(typeof active.action==='function')active.action();instructionCount+=1;renderAll();return true;
}
function runLoop(skipBreakpoint=false){
  clearTimeout(timer);if(!running||halted)return;const count=Number(dom.speed.value);let progressed=false;for(let i=0;i<count&&running&&!halted;i+=1){const didStep=stepOne(skipBreakpoint&&i===0);skipBreakpoint=false;if(!didStep)break;progressed=true;}
  if(running&&!halted&&!waiting)timer=setTimeout(()=>runLoop(false),80);else if(progressed&&waiting)setStatus(currentPoint()?.module==='inche'?'Running · waiting for ACIA input (1–4).':'Running · waiting for keypad input.');
}
function reset(){
  clearTimeout(timer);regs={D0:0,D1:0,D2:0,D3:0,D4:0,D5:0,D6:0,D7:0,A0:0,A1:0,A2:0,A3:0,A4:0,A5:0,A6:0,A7:0x9000,PC:0x2000,SR:0x2700};memory={TUTSP:0x9000,RESULT:0,STATE:0,OPR:0};calc={buffer:'',returnCode:0};terminalText='TUTOR 1.3 READY\n> GO 2000\n';inputQueue=[];queue=[];running=false;waiting=false;halted=false;instructionCount=0;changed=new Set();currentKey='';dom.terminal.textContent=terminalText;
  schedule(point('main',70,'move.l A7,TUTSP',()=>memory.TUTSP=regs.A7,{address:0x2000}),point('main',71,'move.l #$8000,A7',()=>setReg('A7',0x8000),{address:0x2006}),...showMenuFlow());setStatus('Reset complete at $002000. Press Run or Step.');renderAll();
}
function feedKey(char){if(halted)return;inputQueue.push(char);const label=char==='\r'?'Enter':char;setStatus(`Key ${label} latched on ${currentPoint()?.module==='inche'?'ACIA':'parallel port'}.`);const button=[...dom.keypad.querySelectorAll('button')].find(item=>item.dataset.key===char);if(button){button.classList.add('pressed');setTimeout(()=>button.classList.remove('pressed'),100);}if(running&&waiting){waiting=false;runLoop(false);}else renderRegisters();}
function buildKeypad(){const keys=['7','8','9','/','4','5','6','*','1','2','3','-','C','0','\r','+'];dom.keypad.innerHTML=keys.map(key=>{const label=key==='\r'?'ENTER':key;const kind=key==='C'?'clear':key==='\r'?'enter':'+-*/'.includes(key)?'operator':'';return `<button type="button" class="${kind}" data-key="${key==='\r'?'&#13;':key}" aria-label="${label}">${label}</button>`;}).join('');}
Object.entries(MODULES).forEach(([id,meta])=>dom.module.add(new Option(meta.label,id)));
buildKeypad();
dom.run.addEventListener('click',()=>{if(halted)return setStatus('Reset the halted program before running.');if(running){running=false;clearTimeout(timer);setStatus('Execution paused.');renderAll(false);return;}running=true;waiting=false;const active=currentPoint();const skip=active&&breakpoints.has(keyFor(active.module,active.line));setStatus('Running source model…');runLoop(skip);});
dom.step.addEventListener('click',()=>{running=false;clearTimeout(timer);stepOne(true);});dom.reset.addEventListener('click',reset);
dom.module.addEventListener('change',()=>loadSource(dom.module.value,false));dom.clear.addEventListener('click',()=>{breakpoints.clear();refreshSource(false);setStatus('All breakpoints cleared.');});
dom.source.addEventListener('click',event=>{const row=event.target.closest('.source-line.modeled');if(row)toggleBreakpoint(Number(row.dataset.line));});dom.source.addEventListener('keydown',event=>{if((event.key==='Enter'||event.key===' ')&&event.target.matches('.source-line.modeled')){event.preventDefault();toggleBreakpoint(Number(event.target.dataset.line));}});
dom.keypad.addEventListener('click',event=>{const button=event.target.closest('button[data-key]');if(button)feedKey(button.dataset.key);});
window.addEventListener('keydown',event=>{
  if(event.ctrlKey||event.metaKey||event.altKey)return;
  const target=event.target;
  if(target.matches('input,select,textarea,[contenteditable="true"]'))return;
  if(target.matches('button,a')&&(event.key==='Enter'||event.key===' '))return;
  let key=event.key;
  if(key==='Enter')key='\r';
  if(key.toUpperCase()==='C')key='C';
  if(/^[0-9+\-*/]$/.test(key)||key==='\r'||key==='C'){
    event.preventDefault();
    feedKey(key);
  }
});
reset();loadSource('main',true);
})();
