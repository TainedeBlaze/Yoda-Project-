//Kernel for OpenCL implementation.

__kernel void MedianFilter(__global int* arr, 
						__global uint* w, 
						__global uint* l, 
						__global int* grayscale, 
						__global int* filtered){
	//work item numbers
	int n = get_global_id(0); //Work item ID
	int W=*w;
	int L=*l;
	int col=n%W;
	int row=(n-col)/L;
	int size=W*L;
	//printf("W: %d, L: %d",W,L);
	//short calculation: work Item Number x argument 1 + argument 2
    //This is a test output to see if the Kernel produces an output.
	//output[global_addr] = global_addr*arg1 + arg2;
	grayscale[n]=(arr[n*3]+arr[n*3+1]+arr[n*3+2])/3;
	//if (grayscale[n]!=0){
	//printf("output[%d] = %d\n", n, grayscale[n]);
	barrier(CLK_GLOBAL_MEM_FENCE); // Wait for summation of all RGB pixels
	// if ((row!=0 && row!=(L-1) && col!=0 && col!=(W-1))){
	if (n<W || n>(size-W) || col==0 || col==(W-1)){
		filtered[n]=grayscale[n];
	}
	else {
		int med[9]; // Filtering matrix is 3x3
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
	
		// int n = sizeof(filter); 
		// 	std::sort(filter,filter +n);
		//     filteredArray[i]=filter[4] ;
		filtered[n]=med[4];
		//printf("n=%d, m=%d\n",n,med[4]);
		if n=225
	}
	}
	//
	// for (int i=0;i<3;i++){
	// 	med[i+4]=filtered[i];
	// }
	//printf("Executed\n");
	// if (filtered[n]==0){
	// 	printf("filtered[%d]: %d\n",n,filtered[n]);
	// }
