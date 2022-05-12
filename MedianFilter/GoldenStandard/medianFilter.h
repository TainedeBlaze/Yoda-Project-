#ifndef FILTER 
#define FILTER

#include <vector>
#include <string> 
#include <memory> 
#include <ostream> 
#include <iostream>

class medianFilter{

	public: 
		std::string filename ; 
		int width; //dimensions of image 
		int height;

		//constructor accepting filename 
		medianFilter(std::string filename) ; 
		
		//reads in file 
		bool readFile(std::string filename) ; 


}; 
#endif 
