from PIL import Image
import numpy as np

image=Image.open('yoda.jpg')
print(image.size)
print(image.mode)
image_np=np.asarray(image)
print("3D Array Size: ", image_np.size)
img1d = np.reshape(image_np,(1,-1))
print("1D Array Size: ", img1d.size)
