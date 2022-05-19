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
	
	
	bool side=(row==0 || row==(L-1) || col==0 || col==(W-1));
	if (side){
		filtered[n]=(arr[3*n]+arr[3*n+1]+arr[3*n+2])/3;
	}

	else{
		int surr[9];
		for (int i=-1;i<2;i++){
			surr[i+1]=arr[3*(n+i)] + arr[3*(n+i)+1] + arr[3*(n+i)+2];
			surr[i+4]=arr[3*(n+W+i)] + arr[3*(n+i+W)+1] + arr[3*(n+i+W)+2];
			surr[i+7]=arr[3*(n-W+i)] + arr[3*(n+i-W+1)] + arr[3*(n+i-W)+2];
		}
		for (int j=0;j<8;j++){
			int temp=surr[j];
			if (surr[j] > surr[j+1]){
				surr[j]=surr[j+1];
				surr[j+1]=temp;
				j=-1;
			}
		}
		filtered[n]=surr[4]/3;
	}
	//grayscale[n]=(arr[n*3]+arr[n*3+1]+arr[n*3+2])/3;
	//barrier(CLK_GLOBAL_MEM_FENCE);
	//filtered[n]=grayscale[n];
	 // Wait for summation of all RGB pixels
	
	//if ((row!=0 && row!=(L-1) && col!=0 && col!=(W-1))){
		
	//printf("grayscale[88+W+1]: %d",grayscale[88+W]);
	// if (side){
	// 	filtered[n]=grayscale[n];
	// }
	// else{

	
	// printf("Row: %d, Col: %d\n",row,col);
	// int med[9]={0,0,0,0,0,0,0,0,0}; // Filtering matrix is 3x3
	// for (int i=-1;i<2;i++){
	// 	med[i+1] = grayscale[n+i];
	// 	med[i+4] = grayscale[n-W+i];
	// 	med[i+7] = grayscale[n+W+i];
	// }

	// if (n==88){
	// 	printf("\n%d %d %d\n",grayscale[n],grayscale[(n+W+1)],grayscale[(n+W)]);
	// 	printf("\n%d\n",grayscale[n]);
	// 	for (int i=0;i<9;i++){
	// 		printf("%d ",med[i]);
	// 	}
	// }
	// for (int j=0;j<8;j++){
	// 	int temp=med[j];
	// 	if (med[j] > med[j+1]){
	// 		med[j]=med[j+1];
	// 		med[j+1]=temp;
	// 		j=-1;
	// 	}
	// }
	// filtered[n]=med[4];
		// int n = sizeof(filter); 
		// 	std::sort(filter,filter +n);
		//     filteredArray[i]=filter[4] ;
		
		//printf("n=%d, m=%d\n",n,med[4]);
		
	}
	
	//
	// for (int i=0;i<3;i++){
	// 	med[i+4]=filtered[i];
	// }
	//printf("Executed\n");
	// if (filtered[n]==0){
	// 	printf("filtered[%d]: %d\n",n,filtered[n]);
	// }
