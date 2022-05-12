//Taine de Buys 
//Sequential Median Filter 
//2022/05/12

#include "medianFilter.h"
#include <fstream> 

medianFilter:: medianFilter(std::string filename):
	filename(filename)
{}


bool medianFilter::readFile(std:: string filename)
{
	std::ifstream inputfile(filename) ; 
	if (!inputfile){
		std::cout << "The file " << filename << " does not exist. Try again please. " << std::endl ; 
		return false ; 
	}

	std::string text ; //reads in lines from input buffer
        getline(inputfile , text) ; //reads in dimensions 

	std::cout<< text << std::endl ;
	
	 	
	//store width and height 
	

	return true ; 
}
