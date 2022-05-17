import numpy as np
from PIL import Image 
import re 

def convertToImage(filename): 
    textfile = open(filename, 'r')
    data = textfile.read().splitlines() 
    print ("size of data: " +str(len((data))))  
    #convert to array 
    width=data.pop(0)
    height=data.pop(0)
    # for i in range(98696,98712):
        # print(data[i])
    data_array = np.array(data)
    #data_array=np.uint(data_array)
    
    #for i in range(98696,98712):
     #   print(data_array[i])
    #data_array=np.interp(data_array, (data_array.min(), data_array.max()), (0, +255))
    # data_array=np.uint16(data_array)
    # for i in range(len(data_array)):
    #     if data_array[i]>255:
    #         data_array[i]=0
    #     else:
    #         data_array[i]=255
    #for i in range(98690,98712):
     #   print(data_array[i])
    #print(np.ptp(new_data))
    data_array = np.reshape(data_array, (int(width),int(height))) 
    #parse through and find all digits
    
    newimage =Image.fromarray(np.uint8(data_array))
    newimage.save(filename.replace(".txt","") +".png") 

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
