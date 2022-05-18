//Taine de Buys 
//Sequential Median Filter 
//2022/05/12

#include "medianFilter.h"
#include <fstream> 
#include <algorithm>
#include <chrono> 
medianFilter:: medianFilter(std::string filename):
	filename(filename)
{}


bool medianFilter::readFile(std:: string filename)
{
	std::ifstream inputfile(filename); 
	if (!inputfile){
		std::cout << "The file " << filename << " does not exist. Try again please. " << std::endl ; 
		return false ; 
	}
	else 
	{
		std::cout << "The file was read succesfully. " << std::endl ; 
		return true ; 
	}


}

void medianFilter:: getFilteredArray ( std:: string inputfile) {
	//start measuring time 
	auto begin = std::chrono::high_resolution_clock::now() ; 

	std::ifstream inputFile(inputfile) ; 
	std::string text ; //reads in lines from input buffer
   	getline(inputFile , text) ; //reads in dimensions 
	//store width and height as well as size  
	width = std::stoi(text) ; 
	std::cout<< "width: " << width << std::endl ;
	
	getline(inputFile , text) ;
	height = std:: stoi(text) ; 
	std::cout<< "height: " << height <<std::endl ; 
	
	int size = height * width; 
	std::cout<<"Size of image: " << size <<std::endl ; 		
	
	std::vector <int> tempArray ; 
	while (std::getline(inputFile , text)) 
	{
		tempArray.push_back(std::stoi(text)); 
	}	
	std::cout << "Temp Array size: " <<tempArray.size() << std::endl ; 
	//populate array with grayscale pixel values 
	std::vector<int>pixelArray ; 
	for (int i = 0 ; i <tempArray.size() ; i+=3) 
	{
		int pixel = 0 ; 
		pixel = tempArray[i]+tempArray[i+1] + tempArray[i+2] ;//sum RGB values 
		pixel = pixel/3 ; //divide into 1 value 
		pixelArray.push_back(pixel) ; 
	}
	std::cout << "pixel Array size: " <<pixelArray.size() << std::endl ; 
	//do the filtering and output to array
	int filteredArray[size] ; 
	for (int i =0 ; i < size ; i++)
	{	//dealing with outermost pixels 
		if ( i < width || i > (size-width)) {
			filteredArray[i]= pixelArray[i];  
		}
		else if ( i % width == 0 || i %width == (width-1)) {
			filteredArray[i] = pixelArray[i] ; 
		}
		else 
		{
			//create temp array to perform filtering (3 by 3) 
			int filter[9] ; 
			filter[0]=pixelArray[i-width-1];
			filter[1]=pixelArray[i-width];
			filter[2]=pixelArray[i-width+1];
			filter[3]=pixelArray[i-1];
			filter[4]=pixelArray[i];
			filter[5]=pixelArray[i+1];
			filter[6]=pixelArray[i+width-1];
			filter[7]=pixelArray[i+width];
			filter[8]=pixelArray[i+width+1];

			//appending median value
			int n = sizeof(filter); 
			std::sort(filter,filter +n);
		       	filteredArray[i]=filter[4] ;
		}


	}	
	//end measuring time and calculate runtime 
	auto end = std::chrono::high_resolution_clock::now(); 
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin); 

	std::cout<<"Time taken to execute Median Filtering: " << elapsed.count() << " microseconds " <<std::endl ; 

	//write array to txt file 
	std::ofstream outfile ;
  	outfile.open("out_"+filename);

	outfile << width<< std::endl;
	outfile << height<<std::endl;

	for (int j =0 ;  j < size ; j++)
	{
		outfile << filteredArray[j] <<std::endl ; 
	}	
	outfile.close() ; 
 	
} 
	

