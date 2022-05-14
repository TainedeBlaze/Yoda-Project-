import numpy as np
from PIL import Image 
import re 




def convertToImage(filename,width,height): 
    textfile = open(filename, 'r')
    data = textfile.read().splitlines() 
    print ("size of data: " +str(len((data))))  
    #convert to array 
    data_array = np.array(data)
    data_array = np.reshape(data_array, (int(width),int(height))) 
    #parse through and find all digits 
    newimage =Image.fromarray(np.uint8(data_array))  
    newimage.save("output.png") 

Running = True 
while (Running == True): 
    filename = input("Please enter txt file to convert to image or type exit: \n")
    if (filename == 'exit'):
        quit() 
    else: 
        try:
            width = input("enter image width \n ") 
            height = input("Enter image height: \n") 

            convertToImage(filename,width,height) 
        except FileNotFoundError:
            print("This file does not exist, try again") 
