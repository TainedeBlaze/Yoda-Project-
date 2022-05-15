//Kernel for OpenCL implementation.

//Edge Detection kernel which will be executed on the target device (GPU).
//Still needs to be edited according to edgeDetection.cpp
__kernel void Edge(__global int* arr, __global uint* w, __global uint* l, __global int* grayscale, __global int* filtered){
	//work item and work groups numbers
	uint n = get_global_id(0); //Work item ID
	//int workGroupNum = get_group_id(0); //Work group ID
	//int localGroupID = get_local_id(0); //Work items ID within each work group
	
	//memory buffers
	//int arg1 = *test_arr;
	//int arg2 = *argument2;
	uint W=*w;
	uint L=*l;
	uint col=n%W;
	uint row=(n-col)/L;

	//short calculation: work Item Number x argument 1 + argument 2
    //This is a test output to see if the Kernel produces an output.
	//output[global_addr] = global_addr*arg1 + arg2;
    //printf("output[%d] = %d\n", workItemNum, output[workItemNum]);
	//printf("w = %d, l=%d\n",W,L);
	grayscale[n]=arr[n*3]+arr[n*3+1]+arr[n*3+2];
	barrier(CLK_GLOBAL_MEM_FENCE); // Wait for summation of all RGB pixels
	uint med[9];
	//printf("Executed\n");
	if (row!=0 && col!=0 && row!=L-1 && col!=W-1){
		//printf("Executed\n");
	 	for (int i=-1;i<2;i++){
			med[i+1] = grayscale[n+i];
			med[i+4] = grayscale[n-W+i];
			med[i+7] = grayscale[n+W+i];
		}
		
		//filtered[n]=(int)(med[4]+med[5])/2;

	}

	// for (int i=0;i<3;i++){
	// 	med[i+4]=filtered[i];
	// }
	//printf("Executed\n");
	//printf("Row: %d\nCol: %d\ngrayscale[%d]: %d\n\n",row,col,n,grayscale[n]);
}