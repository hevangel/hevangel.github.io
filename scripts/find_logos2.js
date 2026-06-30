const https = require('https');
const opt = { headers: { 'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36' } };

function fetch(url) {
  return new Promise((resolve) => {
    https.get(url, opt, (res) => {
      let data = '';
      res.on('data', (c) => data += c);
      res.on('end', () => resolve({ status: res.statusCode, body: data, headers: res.headers }));
    }).on('error', (e) => resolve({ error: e.message }));
  });
}

async function main() {
  // 1. GitHub vhdl org avatar
  console.log('=== GitHub vhdl org ===');
  let r = await fetch('https://api.github.com/orgs/vhdl');
  if (r.body) {
    try {
      const j = JSON.parse(r.body);
      console.log('avatar: ' + j.avatar_url);
    } catch (e) { console.log('parse error'); }
  } else { console.log(r.error || r.status); }

  // 2. Check assembly on simpleicons/devicon
  console.log('\n=== Assembly logo candidates ===');
  const asmUrls = [
    'https://cdn.jsdelivr.net/gh/devicons/devicon/icons/assembly/assembly-original.svg',
    'https://cdn.simpleicons.org/assembly/0066CC',
  ];
  for (const u of asmUrls) {
    const rr = await fetch(u);
    console.log(rr.status + '  ' + u + (rr.error ? ' ERR:' + rr.error : ''));
  }

  // 3. SystemVerilog - try to find logo
  console.log('\n=== SystemVerilog candidates ===');
  const svUrls = [
    'https://cdn.jsdelivr.net/gh/devicons/devicon/icons/verilog/verilog-original.svg',
  ];
  for (const u of svUrls) {
    const rr = await fetch(u);
    console.log(rr.status + '  ' + u + (rr.error ? ' ERR:' + rr.error : ''));
  }

  // 4. Check GitHub orgs for specman
  console.log('\n=== GitHub specman ===');
  r = await fetch('https://api.github.com/search/repositories?q=specman+in:name&sort=stars&per_page=1');
  if (r.body) {
    try {
      const j = JSON.parse(r.body);
      if (j.items && j.items[0]) console.log('owner avatar: ' + j.items[0].owner.avatar_url);
      else console.log('no results');
    } catch (e) { console.log('parse error'); }
  }

  // 5. Check BASIC sticker image from redbubble (og:image)
  console.log('\n=== Redbubble BASIC sticker ===');
  r = await fetch('https://www.redbubble.com/i/sticker/Basic-Programming-Language-by-microbmen/122239501/7sgk');
  if (r.body) {
    const m = r.body.match(/og:image["'\s]+content=["']([^"']+)["']/i) || r.body.match(/content=["']([^"']*rebubble[^"']*)["']/i);
    console.log(m ? 'found: ' + m[1].substring(0, 200) : 'no og:image found (len=' + r.body.length + ')');
  } else { console.log(r.error || r.status); }
}
main();