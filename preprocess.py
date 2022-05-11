from PIL import Image
import numpy as np

image=Image.open('yoda.jpg')
print(image.size)
print(image.mode)
image_np=np.asarray(image)

img1d = np.reshape(image_np,(1,-1))
print(img1d.size)
