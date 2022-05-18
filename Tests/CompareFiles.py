import sys


def comparefile(filename1 , filename2) : 
    f1 = open(filename1, 'r')
    f2 = open(filename2 , 'r')
    
    i=0 
    
    for line1 in f1:
        i+=1 

        for line2 in f2: 

            if line1 == line2: 
                pass 
            else:
                print("File1: ", i , line1, end = '')
                print("File2: " , i , line2, end='') 
            break 
    f1.close()
    f2.close()





print("welcome to our Vaders File comparer") 
running = True 
while (running ==True):
    filename1 = input("Please enter the name of 1st text file: ") 
    filename2= input("Please enter the name of 2nd text fle: ") 
    if(filename1 == "exit" ):
        quit()
         
    else:
        try:
            comparefile(filename1 , filename2)
        except FileNotFoundError:
            print("The files do not exist, try again. ") 
