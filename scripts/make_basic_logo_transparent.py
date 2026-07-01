"""
Make the BASIC logo background transparent while preserving any white that is
enclosed *inside* the character outlines.

Technique: flood-fill from every border pixel that is "near-white". Only the
border-connected background becomes transparent; white enclosed within the
letter outlines (not reachable from the border) stays fully opaque. This is the
"magic wand, contiguous" approach and avoids hollowing out the characters.

Pure PIL + pure Python (no numpy needed).
"""
from PIL import Image
from collections import deque

SRC = "basic_logo_orig.jpg"
OUT = "images/basic_logo.png"

# A pixel is considered "background-coloured" when all channels are near-white.
# Pure background is (248,248,248). Transition/anti-alias pixels around the
# coloured letters have lower G/B and so stay opaque (preserving letter edges).
WHITE_THRESH = 238


def is_bg(r, g, b):
    return r >= WHITE_THRESH and g >= WHITE_THRESH and b >= WHITE_THRESH


def main():
    im = Image.open(SRC).convert("RGB")
    w, h = im.size
    px = im.load()

    # bg_mask[y][x] == True  -> pixel looks like background colour
    bg = [[is_bg(*px[x, y]) for x in range(w)] for y in range(h)]

    # Flood-fill (4-connected) the background-coloured region that is reachable
    # from the image border. Those pixels are the TRUE outer background.
    outside = [[False] * w for _ in range(h)]
    q = deque()

    # Seed from every border pixel that is background-coloured.
    for x in range(w):
        for y in (0, h - 1):
            if bg[y][x] and not outside[y][x]:
                outside[y][x] = True
                q.append((x, y))
    for y in range(h):
        for x in (0, w - 1):
            if bg[y][x] and not outside[y][x]:
                outside[y][x] = True
                q.append((x, y))

    while q:
        x, y = q.popleft()
        for nx, ny in ((x + 1, y), (x - 1, y), (x, y + 1), (x, y - 1)):
            if 0 <= nx < w and 0 <= ny < h and bg[ny][nx] and not outside[ny][nx]:
                outside[ny][nx] = True
                q.append((nx, ny))

    # Build RGBA: outside background -> transparent, everything else -> opaque.
    rgba = im.convert("RGBA")
    a = rgba.load()
    transparent = enclosed_kept = 0
    for y in range(h):
        for x in range(w):
            if outside[y][x]:
                a[x, y] = (255, 255, 255, 0)
                transparent += 1
            elif bg[y][x]:
                # White but NOT reachable from border -> inside an outline. Keep it.
                enclosed_kept += 1

    rgba.save(OUT)
    total = w * h
    print(f"saved {OUT}  ({w}x{h})")
    print(f"  background made transparent : {transparent} px ({100*transparent/total:.1f}%)")
    print(f"  enclosed white kept opaque  : {enclosed_kept} px ({100*enclosed_kept/total:.1f}%)")


if __name__ == "__main__":
    main()