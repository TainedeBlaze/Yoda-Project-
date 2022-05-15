import numpy as np
from PIL import Image 
import re 

def convertToImage(filename,width,height): 
    textfile = open(filename, 'r')
    data = textfile.read().splitlines() 
    width=data.pop(0)
    height=data.pop(1)
    print ("size of data: " +str(len((data))))  
    #convert to array 
    data_array = np.array(data)
    width=data[0]
    height=data[1]
    data_array = np.reshape(data_array, (int(width),int(height))) 
    #parse through and find all digits 
    newimage =Image.fromarray(np.uint8(data_array))  
    newimage.save(filenamq.replace(".txt","") +".png") 

Running = True 
while (Running == True): 
    filename = input("Please enter txt file to convert to image or type exit: \n")
    if (filename == 'exit'):
        quit() 
    else: 
        try: 
            convertToImage(filename) 
        except FileNotFoundError:
            print("This file does not exist, try again") 
