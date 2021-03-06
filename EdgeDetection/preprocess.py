import numpy as np
import sys
from PIL import Image 

def readimage(imagename):
    print("Reading image " ,str(imagename)) 
    img = Image.open(imagename) 
    width , height = img.size
    pixelarray = np.array(img) 
    pixelarray=pixelarray.reshape(-1)
    dim=np.array([width,height])
    # Adding dimensions to beginning of array
    pixelarray = np.concatenate((dim,pixelarray))    
    print("Image had width of" , width , " and height of" , height)
    print(pixelarray[0], pixelarray[1]) 
     
    img1d = np.reshape(pixelarray,-1)     
    print("Size: ", len(img1d))
    outfile = imagename.split('.')[0] + ".txt" 

    np.savetxt(outfile, img1d, fmt = '%d', delimiter = ",")   
    print( "Output written to .txt file \n ") 

#functionality allowing for multiple files to be read in 
print("Welcome to our image processor. ") 
running = True 
while (running == True): 
    filename = input("Please enter name of file to process or type exit: \n") 
    if (filename == "exit"):  
        quit() 
    else: 
        try: 
            imagearray=readimage(filename)
        except FileNotFoundError: 
            print("The file does not exist, try again. ") 
