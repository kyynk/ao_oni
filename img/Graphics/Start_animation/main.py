
# import OS module
import os
from PIL import Image
# Get the list of all files and directories
for f in  os.listdir("C:\\Users\\Vincent\\Desktop\\game_framework\\img\\Graphics\\Start_animation"):
    if f.endswith(".png"):
        img = Image.open(f)
        img.save(f[:-4] + ".bmp")
        