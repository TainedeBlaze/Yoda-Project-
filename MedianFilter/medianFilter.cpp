//AAuthor: Christopher Hill For the EEE4120F course at UCT
//Edited for median filter use-case: 
// Michael Altshuler, Taine De Buys, Julian Zille
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(void)
{	
	//Read pixels from .txt file
	string filename;
	cout << "Enter .txt filename: ";
	cin >> filename;
	
	ifstream inputfile(filename);
	uint w;
	uint l;
	if (inputfile.is_open())
		inputfile >> w; // width
		inputfile >> l; // length
		int arr[w*l*3];
		for (int i=0;i<w*l*3;i++){
			inputfile >> arr[i];
		}
	cout << ".txt file imported to 1D array" << '\n';

	auto begin = std::chrono::high_resolution_clock::now();
	int sz=sizeof(arr)/sizeof(arr[0]); //Length of RGB array
	
	//Retreiving platform info:
	cl_uint platformCount;
	cl_platform_id *platforms;
	clGetPlatformIDs(5, NULL, &platformCount); 
	platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
	clGetPlatformIDs(platformCount, platforms, NULL); //saves a list of platforms in the platforms variable
	cl_platform_id platform = platforms[0];
	
	//Outputs the information of the chosen platform
	char* Info = (char*)malloc(0x1000*sizeof(char));
	clGetPlatformInfo(platform, CL_PLATFORM_NAME      , 0x1000, Info, 0);
	printf("Name      : %s\n", Info);
	clGetPlatformInfo(platform, CL_PLATFORM_VENDOR    , 0x1000, Info, 0);
	printf("Vendor    : %s\n", Info);
	clGetPlatformInfo(platform, CL_PLATFORM_VERSION   , 0x1000, Info, 0);
	printf("Version   : %s\n", Info);
	clGetPlatformInfo(platform, CL_PLATFORM_PROFILE   , 0x1000, Info, 0);
	printf("Profile   : %s\n", Info);
	
	//------------------------------------------------------------------------
	// Configuring target device
	cl_device_id device; //this is your deviceID
	cl_int err;
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

	//------------------------------------------------------------------------
	// Creating a context
	cl_context context;
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	//------------------------------------------------------------------------
	//Locatiing kernel file
	FILE *program_handle;
	program_handle = fopen("median.cl", "r");

	//get program size
	size_t program_size;
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);

	//sort program buffer 
	char *program_buffer;
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);
   	
	//------------------------------------------------------------------------
	//Create the .cl program from the source code 
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, NULL);

	//------------------------------------------------------------------------

	//Build the source program: compile the source code obtained above
	cl_int err3= clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	printf("program ID = %i\n", err3);
	
	//------------------------------------------------------------------------
	//create the kernel
	cl_kernel kernel = clCreateKernel(program, "MedianFilter", &err);

	//------------------------------------------------------------------------
	//Create the queue for kernels
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);
	//------------------------------------------------------------------------
	//create data buffers for memory management between the host and the target device
	//Initialize buffers, memory space the allows for communication between the host and the target device
	cl_mem RGB_buffer, width_buffer, length_buffer, filtered_buffer;
	size_t global_size = w*l; //total number of work items
	size_t local_size = 1; //Size of each work group
	cl_int num_groups = global_size/local_size; //number of work groups needed
	int filtered[global_size]; //output array

	// Create buffers
	RGB_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sz*sizeof(int), arr, &err);
	width_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint), &w, &err);
	length_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint), &l, &err);
    //Stores outputs from kernel so host can retrieve what the kernel has calculated.
	filtered_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, global_size*sizeof(int), filtered, &err);


	//------------------------------------------------------------------------
	//Set kernel arguments
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &RGB_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &width_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &length_buffer);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), &filtered_buffer);
	
	//------------------------------------------------------------------------
	// Deploy kernel
    cl_int err4 = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);	 	
	printf("\nKernel check: %i \n",err4);

	//------------------------------------------------------------------------

	// Allow the host to read from the buffer object 
	err = clEnqueueReadBuffer(queue, filtered_buffer, CL_TRUE, 0, sizeof(filtered), filtered, 0, NULL, NULL);

	// End timer
	auto end = std::chrono::high_resolution_clock::now(); 
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin); 

	std::cout<<"Time taken to execute Median Filtering: " << elapsed.count() << " microseconds " <<std::endl ;

	//Write to output text file
	ofstream outfile;
	outfile.open("out_"+filename);
	outfile << w<<endl;
	outfile << l<<endl;
	for (int j =0 ;  j < w*l; j++)
	{
		
		outfile << filtered[j] <<std::endl; 
	}	
	outfile.close(); 
	
	//  Deallocate resources
	clFinish(queue);	
	clReleaseKernel(kernel);
	clReleaseMemObject(RGB_buffer);
	clReleaseMemObject(filtered_buffer);
	clReleaseMemObject(width_buffer);
	clReleaseMemObject(length_buffer);
	clReleaseMemObject(grayscale_buffer);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	return 0;
}
