import numpy as np
import sys
from PIL import Image 

def readimage(imagename):
    print("Reading image " ,str(imagename)) 
    img = Image.open(filename) 
    width , height = img.size
    pixelarray = np.array(img) 
    pixelarray=pixelarray.reshape(-1)
    # Adding dimensions to beginning of array
    np.insert(pixelarray,0,height) 
    np.insert(pixelarray,0,width)    
    
    print("Image had width of" , width , " and height of" , height) 
    
    #writing width, height and array to text file 
    img1d = np.reshape(pixelarray,-1)     

    print(np.size(img1d))
    outfile = imagename.split('.')[0] + ".txt" 
    #header = str(width) + ' , '+str(height) 
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