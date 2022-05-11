from PIL import Image
import numpy as np
from numpy import savetxt

image=Image.open('yoda.jpg')
print(image.size) # Eg 220x229
print(image.mode) # RGB, grayscale
image_np=np.asarray(image) 
print("3D Array Size: ", image_np.size)
img1d = np.reshape(image_np,(1,-1))
print("1D Array Size: ", img1d.size)

savetxt('1d.csv',img1d,delimiter=',')