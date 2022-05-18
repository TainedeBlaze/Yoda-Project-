//Taine de Buys & Michael Altshuler
//Sequential Edge Detection 
//2022/05/16

#include "edgeDetector.h"
#include <fstream> 
#include <algorithm>
#include <chrono> 
#include <iostream>
#include <cstdlib>
#include <cmath>

edgeDetector:: edgeDetector(std::string filename):
	filename(filename)
{}


bool edgeDetector::readFile(std:: string filename)
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

void edgeDetector:: getFilteredArray ( std:: string inputfile) {

	//defining Gx and Gy sobel filter arrays
	int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
	int Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

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
	float filteredArray[size] ; 
	int filteredInt[size];
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

			//Calculating the new pixel 			
			//filteredArray[i] = filter[0]*std::sqrt(Gx[0]*Gx[0]+Gy[0]*Gy[0]) + filter[1]*std::sqrt(Gx[1]*Gx[1]+Gy[1]*Gy[1]) 
			// + filter[2]*std::sqrt(Gx[2]*Gx[2]+Gy[2]*Gy[2]) + filter[3]*std::sqrt(Gx[3]*Gx[3]+Gy[3]*Gy[3]) 
			// + filter[4]*std::sqrt(Gx[4]*Gx[4]+Gy[4]*Gy[4]) + filter[5]*std::sqrt(Gx[5]*Gx[5]+Gy[5]*Gy[5]) 
			// + filter[6]*std::sqrt(Gx[6]*Gx[6]+Gy[6]*Gy[6]) + filter[7]*std::sqrt(Gx[7]*Gx[7]+Gy[7]*Gy[7]) 
			// + filter[8]*std::sqrt(Gx[8]*Gx[8]+Gy[8]*Gy[8]) ;
			int h=0;
			int v=0; 
			for (int j =0;j<9;j++){
				h=h+Gx[j]*filter[j];
				v=v+Gy[j]*filter[j];
			}
			int mag=std::sqrt(h*h+v*v);

			//Type casts values in filteredArray[i] to integer values in filteredInt[i].
			filteredInt[i] = (int)filteredArray[i];
			
			//Deals with pixels larger than 255 to wrap around to 0.
			if ( mag < 70){
				filteredInt[i] = 0; 
			}
			//If values do not exceed 255, they maintain their original result.
			else {
				filteredInt[i] = mag;	
			}

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
		outfile << filteredInt[j] <<std::endl ; 
	}	
	outfile.close() ; 
 	
} 