
import os
from PIL import Image
for f in  os.listdir("."):
    if f.endswith(".png"):
        # print(f)
        img = Image.open(f)
        img.save(f[:-4] + ".bmp")
# Dependencies: Pillow
# pip install Pillow
# and u r good to go