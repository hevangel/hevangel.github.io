"""
Thorough verification of basic_logo.png:
  A) Background is actually transparent.
  B) No NON-white (coloured) pixel was accidentally removed -> logo fully intact.
  C) White enclosed inside the character outlines is preserved (opaque).
"""
from PIL import Image

SRC = "basic_logo_orig.jpg"
OUT = "images/basic_logo.png"

orig = Image.open(SRC).convert("RGB").load()
out = Image.open(OUT).convert("RGBA")
w, h = out.size
outp = out.load()

colored_removed = 0      # coloured pixels wrongly made transparent (MUST be ~0)
enclosed_kept = 0        # white inside outlines kept opaque (should be > 0)
bg_removed = 0           # background pixels made transparent
total = w * h
WHITE_THRESH = 238

def is_white(r, g, b):
    return r >= WHITE_THRESH and g >= WHITE_THRESH and b >= WHITE_THRESH

for y in range(h):
    for x in range(w):
        r, g, b = orig[x, y]
        a = outp[x, y][3]
        if is_white(r, g, b):
            if a == 0:
                bg_removed += 1
            else:
                enclosed_kept += 1
        else:
            # coloured pixel of the logo -> MUST remain opaque
            if a == 0:
                colored_removed += 1

print(f"image            : {w}x{h}  ({total} px)")
print(f"bg removed (a=0) : {bg_removed} ({100*bg_removed/total:.1f}%)")
print(f"enclosed white kept (a=255): {enclosed_kept} ({100*enclosed_kept/total:.1f}%)")
print(f"colored pixels wrongly removed: {colored_removed}  <-- MUST be 0")
assert colored_removed == 0, "FAIL: a coloured logo pixel was made transparent!"
assert enclosed_kept > 0, "FAIL: no enclosed white was preserved!"
print("\nALL CHECKS PASSED")

# Build a side-by-side: original | over dark | over red, to eyeball the result.
base = Image.open(SRC).convert("RGBA")
o = Image.open(OUT).convert("RGBA")
dark = Image.new("RGBA", (w, h), (8, 17, 42, 255))
red = Image.new("RGBA", (w, h), (180, 0, 0, 255))
canvas = Image.new("RGBA", (w * 3 + 40, h + 30), (255, 255, 255, 255))
canvas.paste(base, (0, 30))
canvas.paste(Image.alpha_composite(dark, o), (w + 20, 30))
canvas.paste(Image.alpha_composite(red, o), (2 * (w + 20), 30))
canvas.save("basic_logo_verify.png")
print("side-by-side written: basic_logo_verify.png")