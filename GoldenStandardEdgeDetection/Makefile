driver.exe: driver.o edgeDetector.o
	g++ driver.o edgeDetector.o -o driver.exe 

driver.o: driver.cpp
	g++ -c driver.cpp -o driver.o 

edgeDetector.o : edgeDetector.cpp
	g++ -c edgeDetector.cpp -o edgeDetector.o

clean: 
	rm *.o *.png *.txt driver.exe  
