//Author: Christopher Hill For the EEE4120F course at UCT
//Edited for edge detection use-case: Michael Altshuler, Taine De Buys, Julian Zille
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
	

	int sz=sizeof(arr)/sizeof(arr[0]); //length of RGB array

	auto OH_start = std::chrono::high_resolution_clock::now(); // Start overhead timer
	cl_uint platformCount; //keeps track of the number of platforms you have installed on your device
	cl_platform_id *platforms;
	// get platform count
	clGetPlatformIDs(5, NULL, &platformCount); //sets platformCount to the number of platforms

	// get all platforms
	platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
	clGetPlatformIDs(platformCount, platforms, NULL); //saves a list of platforms in the platforms variable
    
	//select the platform you wish to use
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

	//get device ID
	cl_device_id device; //this is your deviceID
	cl_int err;
	
	// select target device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

	//------------------------------------------------------------------------
	
	//creates a context that allows devices to send and receive kernels and transfer data
	cl_context context;
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	//------------------------------------------------------------------------

	//get details about the kernel.cl file in order to create it (read the kernel.cl file and place it in a buffer)
	//read file in	
	FILE *program_handle;
	//TODO code 4: select the directory of the file that contains the kernel
	program_handle = fopen("edge.cl", "r");

	//get program size
	size_t program_size;//, log_size;
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);

	//sort buffer out
	char *program_buffer;//, *program_log;
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);
   	
	//------------------------------------------------------------------------

	//create the .cl program from the source code 
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, NULL);

	//------------------------------------------------------------------------

	//compile the source code obtained above
	cl_int err3= clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	printf("program ID = %i\n", err3);
	
	//------------------------------------------------------------------------

	//create the kernel
	cl_kernel kernel = clCreateKernel(program, "Edge", &err);

	//------------------------------------------------------------------------
	
	// create the command queue
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);

	//------------------------------------------------------------------------

	//Work-items 
	size_t global_size = w*l; //one work-item per output pixel
	size_t local_size = 1; //Size of each work group
	cl_int num_groups = global_size/local_size; //number of work groups needed
	int detected[global_size]; //output array

		
	//Initialize Buffers, memory space the allows for communication between the host and the target device
	cl_mem RGB_buffer, width_buffer, length_buffer, detected_buffer;
	RGB_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sz*sizeof(int), arr, &err);
	width_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint), &w, &err);
	length_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint), &l, &err);
    //Stores outputs from kernel so host can retrieve what the kernel has calculated.
	detected_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, global_size*sizeof(int), detected, &err);


	//------------------------------------------------------------------------

	//create the arguments for the kernel 
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &RGB_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &width_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &length_buffer);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), &detected_buffer);
	
	//------------------------------------------------------------------------
	auto OH_end = std::chrono::high_resolution_clock::now(); // End overhead timer
	auto OH_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(OH_end-OH_start); 
	std::cout<<"Overhead time: " << OH_elapsed.count() << " microseconds " <<std::endl ;
	// Deploy kernel and set work groups
	auto begin = std::chrono::high_resolution_clock::now(); // Time kernel execution
    cl_int err4 = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);	 	
	printf("\nKernel check: %i \n",err4);

	//------------------------------------------------------------------------
	//Read the output values from the output buffer
	err = clEnqueueReadBuffer(queue, detected_buffer, CL_TRUE, 0, sizeof(detected), detected, 0, NULL, NULL);
	
	auto end = std::chrono::high_resolution_clock::now(); 
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end-begin); 

	std::cout<<"Time taken to execute Median Filtering: " << elapsed.count() << " microseconds " <<std::endl ;
	
	//write array to txt file 
	ofstream outfile;
	outfile.open("out_"+filename);
	outfile << w<<endl;
	outfile << l<<endl;
	for (int j =0 ;  j < w*l; j++)
	{
		
		outfile << detected[j] <<std::endl; 
	}	
	outfile.close();

	// Deallocate resources
	clFinish(queue);	
	clReleaseKernel(kernel);
	clReleaseMemObject(RGB_buffer);
	clReleaseMemObject(detected_buffer);
	clReleaseMemObject(width_buffer);
	clReleaseMemObject(length_buffer);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);

	return 0;
}
