import numpy as np
from PIL import image 
import re 




def convertToImage(filename): 
    with open(filename) as f:
        file= file.read

    #parse through and find all digits 
    l = re.findall(r'\d+',s)

    #convert to numpy array 
   # data = np.array(1).reshape(width, height , 3))
    
    #convert to image 

    #img = Image.fromarray(data , "RGB") 
    img.save("output.png") 





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
