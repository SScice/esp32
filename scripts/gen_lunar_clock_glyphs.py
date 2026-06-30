#!/usr/bin/env python3
"""Generate 20x28 1bpp lunar Han glyphs matching clock_font_data.inc layout."""
from PIL import Image, ImageDraw, ImageFont
import os

W, H = 20, 28
ROW_BYTES = (W + 7) // 8
BYTES = ROW_BYTES * H

# (char, codepoint) — lunar calendar label set
GLYPHS = [
    ("正", 0x6B63),
    ("二", 0x4E8C),
    ("三", 0x4E09),
    ("四", 0x56DB),
    ("五", 0x4E94),
    ("六", 0x516D),
    ("七", 0x4E03),
    ("八", 0x516B),
    ("九", 0x4E5D),
    ("十", 0x5341),
    ("腊", 0x814A),
    ("闰", 0x95F0),
    ("月", 0x6708),
    ("初", 0x521D),
    ("廿", 0x5EFF),
    ("卅", 0x5345),
    ("冬", 0x51AC),
]

FONT = "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc"


def render_char(ch: str) -> list[int]:
    scale = 8
    big = Image.new("L", (W * scale, H * scale), 255)
    draw = ImageDraw.Draw(big)
    font = ImageFont.truetype(FONT, int(H * scale * 0.72), index=0)
    bbox = draw.textbbox((0, 0), ch, font=font)
    tw, th = bbox[2] - bbox[0], bbox[3] - bbox[1]
    ox = (big.width - tw) // 2 - bbox[0]
    oy = (big.height - th) // 2 - bbox[1]
    draw.text((ox, oy), ch, fill=0, font=font)
    small = big.resize((W, H), Image.Resampling.LANCZOS)
    px = small.load()
    out = [0] * BYTES
    for y in range(H):
        for x in range(W):
            if px[x, y] < 200:
                byte_i = y * ROW_BYTES + (x >> 3)
                out[byte_i] |= 0x80 >> (x & 7)
    return out


def fmt_array(data: list[int]) -> str:
    hexes = ", ".join(f"0x{b:02x}" for b in data)
    return f"{{ {hexes} }}"


def main():
    lines = ["// lunar Han glyphs (20x28, same packing as clock digits)"]
    for ch, cp in GLYPHS:
        data = render_char(ch)
        lines.append(f"static const uint8_t glyph_{cp}[] PROGMEM = {fmt_array(data)};")
    path = os.path.join(os.path.dirname(__file__), "..", "src", "clock_font_lunar_data.inc")
    with open(path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("wrote", path, len(GLYPHS), "glyphs")


if __name__ == "__main__":
    main()