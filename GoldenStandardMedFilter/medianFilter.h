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
		//writes filtered array to tet file
		void getFilteredArray(std::string inputfile) ; 

}; 
#endif 
