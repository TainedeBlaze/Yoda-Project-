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
	//store width and height 
	width = std::stoi(text) ; 
	std::cout<< "width: " << width << std::endl ;
	getline(inputfile , text) ;
	height = std:: stoi(text) ; 
	std::cout<< "height: " << height <<std::endl ; 
	 	
	 
	

	return true ; 
}
