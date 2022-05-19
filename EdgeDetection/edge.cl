//Kernel for OpenCL implementation.

//Edge Detection kernel which will be executed on the target device (GPU).
//Still needs to be edited according to edgeDetection.cpp
__kernel void Edge(__global int* arr,
					 __global uint* w, 
					 __global uint* l,  
					 __global int* detected){
	//work item and work groups numbers
	uint n = get_global_id(0); //Work item ID
	uint W=*w;
	uint L=*l;
	uint col=n%W;
	uint row=(n-col)/L;
	bool side=(row==0 || row==(L-1) || col==0 || col==(W-1));
	int Gy[9]={-1,-2,-1,0,0,0,1,2,1};
	int Gx[9]={-1,0,1,-2,0,2,-1,0,1};
	float v=0;
	float h=0;
	float pix;
	if (side){
		detected[n]=(arr[3*n]+arr[3*n+1]+arr[3*n+2])/3;
	}
	else{
		int surr[9];
		for (int i=-1;i<2;i++){
			surr[i+1]=(arr[3*(n-W+i)] + arr[3*(n+i-W+1)] + arr[3*(n+i-W)+2])/3;
			surr[i+4]=(arr[3*(n+i)] + arr[3*(n+i)+1] + arr[3*(n+i)+2])/3;
			surr[i+7]=(arr[3*(n+W+i)] + arr[3*(n+i+W)+1] + arr[3*(n+i+W)+2])/3;
		}
		for (int j=0;j<9;j++){
				h=h+Gx[j]*surr[j];
				//printf("Ex[%d]*temp[%d]: %d * %d = %f\n",j,j,Ex[j],temp[j],h);
				v=v+Gy[j]*surr[j];
				//printf("%f %f\n",v,h);
			}
		pix=sqrt(v*v+h*h);
		if (pix<70){
			pix=0;
		}
		detected[n]=(int)pix;
		
	}
	//grayscale[n]=(arr[n*3]+arr[n*3+1]+arr[n*3+2])/3;
	// printf("%d\n",grayscale[n]);
	//barrier(CLK_GLOBAL_MEM_FENCE); // Wait for summation of all RGB pixels
	//printf("Executed\n");
	
	//detected[n]=grayscale[n];
	
	// int Ex[9]={-1,0,1,-1,0,1,-1,0,1};
	// int temp[9];
	// int Ey[9]={1,1,1,0,0,0,-1,-1,-1};
	
	// int temp[9];
	
	// float h=0;
	// float v=0;
	// float pix;
	//printf("Executed\n");
	//
	//if (row!=0 && row!=(L-1) && col!=0 && col!=(W-1)){
		//printf("Executed\n");
		//printf("Row: %d Col: %d\n",row,col);
		// for (int i=-1;i<2;i++){
		// 	temp[i+1] = grayscale[n-W+i];
		// 	temp[i+4] = grayscale[n+i];
		// 	temp[i+7] = grayscale[n+W+i];
		// }
        	// for (int i=0;i<9;i++){
            // 	printf("%d\n",grayscale[i]);
    		// }
		// if (v!=0){
		// 	//printf("%d\n",v);
		// }
		//float y=powr(1,1);
		//printf("%f%f\n",h,v);
		// filtered[n]=pix;
		// pix=sqrt(v*v+h*h);
		// if ( pix<70){
		// 	pix=0;
		// }

		// //pix=abs(v)+abs(h);//sqrt(pix);
		// //barrier(CLK_GLOBAL_MEM_FENCE);
		// detected[n]=(int)pix;
		
	}
	

	//barrier(CLK_GLOBAL_MEM_FENCE); // Wait for x convolution to finish
	

	// for (int i=0;i<3;i++){
	// 	med[i+4]=filtered[i];
	// }
	//printf("Executed\n");
	//printf("Row: %d\nCol: %d\ngrayscale[%d]: %d\n\n",row,col,n,grayscale[n]);
