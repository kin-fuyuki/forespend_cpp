from PIL import Image
import sys
import string
import os
def gen_macro_names(n):
	# generate a, b, c, ..., z, aa, ab, ac ...
	letters = string.ascii_lowercase
	for length in range(1, 3):  # support 1 or 2 letters
		for combo in (''.join(x) for x in __import__('itertools').product(letters, repeat=length)):
			yield combo
			n -= 1
			if n == 0:
				return

for filepath in sys.argv[1:]:
	
	img = Image.open(filepath).convert("P")
	name=os.path.splitext(os.path.basename(filepath))[0]
	assert img.size == (8,8), "Image must be 8x8 pixels"

	palette = img.getpalette()  # flat list of RGB triples (may be longer than palette entries)
	palette_colors = []
	# Extract palette colors (groups of 3)
	for i in range(0, len(palette), 3):
		r, g, b = palette[i:i+3]
		palette_colors.append( (r, g, b, 255) )  # assume full alpha

	# Find all indices used in image
	used_indices = sorted(set(img.getdata()))

	# Assign macro names only to used colors
	macro_gen = gen_macro_names(len(used_indices))
	idx_to_macro = {}
	for idx in used_indices:
		idx_to_macro[idx] = next(macro_gen)

	# Output C++ code
	print("// Colors from palette")
	for idx in used_indices:
		r, g, b, a = palette_colors[idx]
		varname = name+f"{idx}"
		print(f"const color {varname} = color{{ {r}, {g}, {b}, {a} }};")
	print()

	# Output macros
	for idx in used_indices:
		varname = name+f"{idx}"
		macro = idx_to_macro[idx]
		print(f"#define {macro} (&{varname})")
	print()

	# Output pointer array in 8x8 grid
	print("const color* "+name+"tx[64] = {")
	for y in range(8):
		line = "	"
		for x in range(8):
			pixel_idx = img.getpixel((x,y))
			macro = idx_to_macro[pixel_idx]
			line += f"{macro}, "
		print(line.rstrip())
	print("};")
	for idx in used_indices:
		varname = f"brick{idx}"
		macro = idx_to_macro[idx]
		print(f"#undef {macro}")
	print()
