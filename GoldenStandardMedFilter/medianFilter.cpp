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

	int filteredArray[size] ; 

	//start measuring time 
	auto begin = std::chrono::high_resolution_clock::now() ;
	for (int i =0 ; i < size ; i++)
	{
		int col=i%width;
		int row=(i-col)/height;
		bool side = (row==0 || row==(height-1) || col==0 || col==(width-1));
		if (side){
			filteredArray[i]=(tempArray[3*i]+tempArray[3*i+1]+tempArray[3*i+2])/3;
		}
		else 
		{	
			int surr[9];
			for (int k=-1;k<2;k++){
				surr[k+1]=tempArray[3*(i+k)] + tempArray[3*(i+k)+1] + tempArray[3*(i+k)+2];
				surr[k+4]=tempArray[3*(i+width+k)] + tempArray[3*(i+k+width)+1] + tempArray[3*(i+k+width)+2];
				surr[k+7]=tempArray[3*(i-width+k)] + tempArray[3*(i+k-width+1)] + tempArray[3*(i+k-width)+2];
			}
			for (int j=0;j<8;j++){
				int temp=surr[j];
				if (surr[j] > surr[j+1]){
					surr[j]=surr[j+1];
					surr[j+1]=temp;
					j=-1;
				}
			}
			filteredArray[i]=surr[4]/3;	
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