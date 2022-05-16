//Taine de Buys
//driver for sequential filter
//2022/05/12


#include "edgeDetector.h"
#include "string"
#include "cstring" 
#include <iostream> 
#include <ostream> 

//main function 


int main (int argc , char* argv []) {

	std::cout << "Yoda's Sequential Edge Detector Activated" << std:: endl ; 

	if (argc < 2) 
	{
		std::cout<< "Invalid execution, must enter filename when executing" << std :: endl ; 
		std::cout<< "Correct execution: ./driver.exe <filename> " <<std::endl ; 
		std::cout << "Try again. " << std::endl ; 
		return 0 ;
	}
	std:: string inFile ; //name of file inputted 
	inFile = argv[1] ; 
	std::cout<< "File inputted: " << inFile << std::endl ; 

	//make object of edgeDetector 
	edgeDetector filter(inFile) ; 

	//ensure its read properly 
	if (filter.readFile(inFile) == false)
	{
		return 0 ; 
	}
	else
	{	//get pixel array 
		std::vector <int> pixels ; 
		filter.getFilteredArray(inFile) ; 
		return 1; 
	}

}
