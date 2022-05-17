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
using namespace std;

int main(void)
{	
	//Read pixels from .txt file
	int y=sqrt(4);
	printf("%d",y);
	string filename;
	// cout << "Enter .txt filename: ";
	// cin >> filename;
	filename="insta.txt";
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
	
	//int arr[]={2,4,7,30,46,23,23,65,86,34,57,3,34,87,94,123,143,67,23,43,197,33,76,97,34,78,54};
	//int sz=sizeof(arr)/sizeof(arr[0]);
	// int w=3;
	// int l=3;

	int sz=sizeof(arr)/sizeof(arr[0]);
	/* OpenCL structures you need to program*/
	//cl_device_id device; step 1 and 2 
	//cl_context context;  step 3
	//cl_program program;  steps 4,5 and 6
	//cl_kernel kernel; step 7
	//cl_command_queue queue; step 8
	
	//------------------------------------------------------------------------
	 
	 
	//Initialize Buffers, memory space the allows for communication between the host and the target device
	cl_mem RGB_buffer, width_buffer, length_buffer,grayscale_buffer, filtered_buffer;

	//***step 1*** Get the platform you want to use
	//cl_int clGetPlatformIDs(cl_uint num_entries,
	//				cl_platform_id *platforms, 
	//				cl_uint *num_platforms)
  	
    	//------------------------------------------------------------------------
    
	cl_uint platformCount; //keeps track of the number of platforms you have installed on your device
	cl_platform_id *platforms;
	// get platform count
	clGetPlatformIDs(5, NULL, &platformCount); //sets platformCount to the number of platforms

	// get all platforms
	platforms = (cl_platform_id*) malloc(sizeof(cl_platform_id) * platformCount);
	clGetPlatformIDs(platformCount, platforms, NULL); //saves a list of platforms in the platforms variable
    
	//TODO code 1: select the platform you wish to use
	//Select the platform you would like to use in this program (change index to do this). If you would like to see all available platforms run platform.cpp.
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

	//***step 2*** get device ID must first get platform
	//cl_int clGetDeviceIDs(cl_platform_id platform,
	//			cl_device_type device_type, 
	//			cl_uint num_entries, 
	//			cl_device_id *devices, 
	//			cl_uint *num_devices)
	
	cl_device_id device; //this is your deviceID
	cl_int err;
	
	/* Access a device */
	//The if statement checks to see if the chosen platform uses a GPU, if not it setups the device using the CPU
	//TODO code 2: select your device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

	//------------------------------------------------------------------------
	
	//***Step 3*** creates a context that allows devices to send and receive kernels and transfer data
	//cl_context clCreateContext(cl_context_properties *properties,
	//				cl_uint num_devices,
	//				const cl_device_id *devices,
	//				void *pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data),
	//				void *user_data,cl_int *errcode_ret)
	//TODO code 3: create the context
	cl_context context;
	context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
	//------------------------------------------------------------------------

	//***Step 4*** get details about the kernel.cl file in order to create it (read the kernel.cl file and place it in a buffer)
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

	//***Step 5*** create program from source because the kernel is in a separate file 'kernel.cl', therefore the compiler must run twice once on main and once on kernel
	//cl_program clCreateProgramWithSource (cl_context context,
	//						cl_uint count, 
	//						const char **strings, 
	//						const size_t *lengths, 
	//						cl_int *errcode_ret)	
	
	//TODO code 5: create the .cl program from the source code 
	cl_program program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, NULL);

	//------------------------------------------------------------------------

	//***Step 6*** build the program, this compiles the source code from above for the devices that the code has to run on (ie GPU or CPU)
	//cl_int clBuildProgram(cl_program program,
	//		cl_uint num_devices,
	//		const cl_device_id* device_list,
	//		const char* options,
	//		void (CL_CALLBACK* pfn_notify)(cl_program program, void* user_data),
	//		void* user_data);
	
	//TODO code 6: compile the source code obtained in step 5
	cl_int err3= clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	printf("program ID = %i\n", err3);
	
	//------------------------------------------------------------------------

	//***Step 7*** creates the kernel, this creates a kernel from one of the functions in the cl_program you just built
	//cl_kernel clCreateKernel(cl_program program,
	//			const char* kernel_name,
	//			cl_int* errcode_ret);
	//TODO code 7: create the kernel
	cl_kernel kernel = clCreateKernel(program, "Edge", &err);

	//------------------------------------------------------------------------
	
	//***Step 8*** create command queue to the target device. This is the queue that the kernels get dispatched too, to get the the desired device.
	//cl_command_queue clCreateCommandQueue(cl_context context,
	//						cl_device_id device, 
	//						cl_command_queue_properties properties,
	//						cl_int *errcode_ret)
	
	//TODO code 8: create the queue
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, NULL);

	//------------------------------------------------------------------------

	//***Step 9*** create data buffers for memory management between the host and the target device
	//TODO code 9.1: set the number of work items, size of the work items and determine the number of work groups

    //Where we determine how many work items and work groups we want for the program. 
	size_t global_size = w*l; //total number of work items
	size_t local_size = 1; //Size of each work group
	cl_int num_groups = global_size/local_size; //number of work groups needed

	int filtered[global_size]; //output array
	int grayscale[global_size];

	//Buffer (memory block) that both the host and target device can access 
		//cl_mem clCreateBuffer(cl_context context,
	//			cl_mem_flags flags,
	//			size_t size,
	//			void* host_ptr,
	//			cl_int* errcode_ret);
	
	//TODO code 9.2: Create the buffer for argument 1
	RGB_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sz*sizeof(int), arr, &err);
	width_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint), &w, &err);
	length_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(uint), &l, &err);
    //Stores outputs from kernel so host can retrieve what the kernel has calculated.
	grayscale_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, global_size*sizeof(int), grayscale, &err);
	filtered_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, global_size*sizeof(int), filtered, &err);


	//------------------------------------------------------------------------

	//***Step 10*** create the arguments for the kernel (link these to the buffers set above, using the pointers for the respective buffers)
	// cl_int clSetKernelArg (cl_kernel kernel, 
	//				cl_uint arg_index, 
	//				size_t arg_size, 
	//				const void *arg_value)
	//TODO code 10: create the arguments for the kernel. Will need to change these according to arguments for median filter
	clSetKernelArg(kernel, 0, sizeof(cl_mem), &RGB_buffer);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), &width_buffer);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), &length_buffer);
	clSetKernelArg(kernel, 3, sizeof(cl_mem), &grayscale_buffer);
	clSetKernelArg(kernel, 4, sizeof(cl_mem), &filtered_buffer);
	
	//------------------------------------------------------------------------

	//***Step 11*** enqueue kernel, deploys the kernels and determines the number of work-items that should be generated to execute the kernel (global_size) and the number of work-items in each work-group (local_size).
	
	// cl_int clEnqueueNDRangeKernel (cl_command_queue command_queue, 
	//					cl_kernel kernel, 
	//					cl_uint work_dim, 
	//					const size_t *global_work_offset, 
	//					const size_t *global_work_size, 
	//					const size_t *local_work_size, 
	//					cl_uint num_events_in_wait_list, 
	//					const cl_event *event_wait_list, 
	//					cl_event *event)
	//TODO code 11: deploy kernel and set work groups
    cl_int err4 = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);	 	
	printf("\nKernel check: %i \n",err4);

	//------------------------------------------------------------------------

	//***Step 12*** Allows the host to read from the buffer object 
	//TODO code 12: read the output values from the output buffer
	err = clEnqueueReadBuffer(queue, filtered_buffer, CL_TRUE, 0, sizeof(filtered), filtered, 0, NULL, NULL);
	
	//***Step 13*** Check that the host was able to retrieve the output data from the output buffer
	// printf("\nOutput in the filtered \n");
	//   for(int j=0; j<global_size; j++) {
	//   	printf("grayscale[%d]=%d\n",j ,grayscale[j]);
	//   }
	// for(int k=0;k<sz;k++){
		//printf("arr[%d] = %d\n",k,arr[k]);}
	//------------------------------------------------------------------------
	ofstream outfile;
	outfile.open("out_"+filename);
	outfile << w<<endl;
	outfile << l<<endl;
	for (int j =0 ;  j < w*l; j++)
	{
		outfile << filtered[j] <<std::endl; 
	}	
	outfile.close(); 
	//***Step 14*** Deallocate resources
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
