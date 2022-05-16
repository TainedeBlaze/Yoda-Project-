//Kernel for OpenCL implementation.

__kernel void MedianFilter(__global int* arr, 
						__global uint* w, 
						__global uint* l, 
						__global int* grayscale, 
						__global int* filtered){
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
	//printf("W: %d, L: %d",W,L);
	//short calculation: work Item Number x argument 1 + argument 2
    //This is a test output to see if the Kernel produces an output.
	//output[global_addr] = global_addr*arg1 + arg2;
	grayscale[n]=(arr[n*3]+arr[n*3+1]+arr[n*3+2])/3;
	//if (grayscale[n]!=0){
	//printf("output[%d] = %d\n", n, grayscale[n]);}
	barrier(CLK_GLOBAL_MEM_FENCE); // Wait for summation of all RGB pixels
	
	filtered[n]=grayscale[n];

	int med[9]; // Filtering matrix is 3x3
	if ((row!=0 && row!=(L-1) && col!=0 && col!=(W-1))){
		
		for (int i=-1;i<2;i++){
			med[i+1] = grayscale[n+i];
			med[i+4] = grayscale[n-W+i];
			med[i+7] = grayscale[n+W+i];
		}

		for (int j=0;j<8;j++){
			int temp=med[j];
			if (med[j] > med[j+1]){
				med[j]=med[j+1];
				med[j+1]=temp;
				j=-1;
			}
    	}
		int m = med[4];
		filtered[n]=m;
		//printf("n=%d, m=%d\n",n,m);
	}

	// for (int i=0;i<3;i++){
	// 	med[i+4]=filtered[i];
	// }
	//printf("Executed\n");
	// if (filtered[n]==0){
	// 	printf("filtered[%d]: %d\n",n,filtered[n]);
	// }
}