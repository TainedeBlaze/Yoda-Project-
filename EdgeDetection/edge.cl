//Kernel for OpenCL implementation.

//Edge Detection kernel which will be executed on the target device (GPU).
//Still needs to be edited according to edgeDetection.cpp
__kernel void Edge(__global int* arr,
					 __global uint* w, 
					 __global uint* l, 
					 __global int* grayscale, 
					 __global int* detected){
	//work item and work groups numbers
	uint n = get_global_id(0); //Work item ID
	//int workGroupNum = get_group_id(0); //Work group ID
	//int localGroupID = get_local_id(0); //Work items ID within each work group
	//printf("Executed\n");
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
	grayscale[n]=(arr[n*3]+arr[n*3+1]+arr[n*3+2])/3;
	// printf("%d\n",grayscale[n]);
	barrier(CLK_GLOBAL_MEM_FENCE); // Wait for summation of all RGB pixels
	//printf("Executed\n");
	
	detected[n]=grayscale[n];
	
	// int Ex[9]={-1,0,1,-1,0,1,-1,0,1};
	// int temp[9];
	// int Ey[9]={1,1,1,0,0,0,-1,-1,-1};
	int Ex[9]={-1,0,1,-2,0,2,-1,0,1};
	int temp[9];
	int Ey[9]={-1,-2,-1,0,0,0,1,2,1};
	float h=0;
	float v=0;
	float pix;
	//printf("Executed\n");
	//
	if (row!=0 && row!=(L-1) && col!=0 && col!=(W-1)){
		//printf("Executed\n");
		//printf("Row: %d Col: %d\n",row,col);
		for (int i=-1;i<2;i++){
			temp[i+1] = grayscale[n-W+i];
			temp[i+4] = grayscale[n+i];
			temp[i+7] = grayscale[n+W+i];
		}
        	// for (int i=0;i<9;i++){
            // 	printf("%d\n",grayscale[i]);
    		// }

		for (int j=0;j<9;j++){
			h=h+Ex[j]*temp[j];
			//printf("Ex[%d]*temp[%d]: %d * %d = %f\n",j,j,Ex[j],temp[j],h);
			v=v+Ey[j]*temp[j];
			//printf("%f %f\n",v,h);
		}
		// if (v!=0){
		// 	//printf("%d\n",v);
		// }
		//float y=powr(1,1);
		//printf("%f%f\n",h,v);
		// filtered[n]=pix;
		pix=sqrt(pow(v,2.0f)+pow(h,2.0f));
		if (pix>255){
			pix=0;
		}
		else if (pix<110){
			pix=0;
		}

		//pix=abs(v)+abs(h);//sqrt(pix);
		//barrier(CLK_GLOBAL_MEM_FENCE);
		detected[n]=(int)pix;
		
	}
	
}	
	//barrier(CLK_GLOBAL_MEM_FENCE); // Wait for x convolution to finish
	

	// for (int i=0;i<3;i++){
	// 	med[i+4]=filtered[i];
	// }
	//printf("Executed\n");
	//printf("Row: %d\nCol: %d\ngrayscale[%d]: %d\n\n",row,col,n,grayscale[n]);
