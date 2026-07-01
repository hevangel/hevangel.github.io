"""
Make the ASM logo's checkerboard "fake transparency" preview background truly
transparent.

The downloaded PNG (images/assembly_logo.png) had no alpha channel at all —
its "transparency" was just a baked-in light-gray/white checkerboard pattern
from a preview tool. This flood-fills that checkerboard (and any near-white
pixels reachable from the image border) to real alpha transparency, the same
"magic wand, contiguous" technique used for basic_logo.png.

Pure PIL + pure Python (no numpy needed).
"""
from PIL import Image
from collections import deque

SRC = "images/assembly_logo.png"
OUT = "images/assembly_logo.png"

WHITE_THRESH = 220


def is_bg(r, g, b):
    return r >= WHITE_THRESH and g >= WHITE_THRESH and b >= WHITE_THRESH


def main():
    im = Image.open(SRC).convert("RGB")
    w, h = im.size
    px = im.load()

    bg = [[is_bg(*px[x, y]) for x in range(w)] for y in range(h)]

    outside = [[False] * w for _ in range(h)]
    q = deque()

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

    rgba = im.convert("RGBA")
    a = rgba.load()
    transparent = 0
    for y in range(h):
        for x in range(w):
            if outside[y][x]:
                a[x, y] = (255, 255, 255, 0)
                transparent += 1

    rgba.save(OUT)
    total = w * h
    print(f"saved {OUT}  ({w}x{h})")
    print(f"  background made transparent : {transparent} px ({100*transparent/total:.1f}%)")


if __name__ == "__main__":
    main()
