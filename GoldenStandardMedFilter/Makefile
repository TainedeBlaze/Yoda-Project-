driver.exe: driver.o medianFilter.o
	g++ driver.o medianFilter.o -o driver.exe 

driver.o: driver.cpp
	g++ -c driver.cpp -o driver.o 

medianFilter.o : medianFilter.cpp
	g++ -c medianFilter.cpp -o medianFilter.o

clean: 
	rm *.o *.png *.txt driver.exe  
