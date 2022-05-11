import numpy as np
import sys
from PIL import Image 


 
def readimage(imagename):
    print("Reading image " ,str(imagename)) 
    img = Image.open(filename) 
    pixelarray = np.array(img) 
    print(pixelarray) 

print("Welcome to our image processor. ") 
running = True 
while (running == True): 
    filename = input("Please enter name of file to process or type exit: \n") 
    if (filename == "exit"):  
        quit() 
    else: 
        try: 
            readimage(filename) 
        except FileNotFoundError: 
            print("The file does not exist, try again. ") 
