import urllib.request
import ssl

url = 'https://ih1.redbubble.net/image.4070246215.9501/st,small,507x507-pad,600x600,f8f8f8.jpg'
out = 'basic_logo_orig.jpg'

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

req = urllib.request.Request(url, headers={
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0 Safari/537.36',
    'Accept': 'image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8',
    'Referer': 'https://www.redbubble.com/',
})

with urllib.request.urlopen(req, context=ctx, timeout=30) as r, open(out, 'wb') as f:
    f.write(r.read())

print('downloaded', out)