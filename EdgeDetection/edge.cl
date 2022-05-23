//Kernel for OpenCL implementation.

//Edge Detection kernel which will be executed on the target device (GPU).
//Still needs to be edited according to edgeDetection.cpp
__kernel void Edge(__global int* arr,
					 __global uint* w, 
					 __global uint* l,  
					 __global int* detected){
	//work item and work groups numbers
	uint n = get_global_id(0); //Work item ID
	//Width and height
	uint W=*w; 
	uint L=*l;

	uint col=n%W; // Col number
	uint row=(n-col)/L; // Row number
	bool side=(row==0 || row==(L-1) || col==0 || col==(W-1)); // Side pixel
	
	//Sobel windows:
	int Gy[9]={-1,-2,-1,0,0,0,1,2,1};
	int Gx[9]={-1,0,1,-2,0,2,-1,0,1};

	float v=0;//Vertical edge component
	float h=0;//Horizontal edge component
	float pix;//Output pixel value

	if (side){
		detected[n]=(arr[3*n]+arr[3*n+1]+arr[3*n+2])/3; // Grayscale side pixels - no edge-detection
	}
	else{
		int surr[9];
		for (int i=-1;i<2;i++){ // Create 3x3 matrix with pixel n and surrounding pixels
			surr[i+1]=(arr[3*(n-W+i)] + arr[3*(n+i-W+1)] + arr[3*(n+i-W)+2])/3;
			surr[i+4]=(arr[3*(n+i)] + arr[3*(n+i)+1] + arr[3*(n+i)+2])/3;
			surr[i+7]=(arr[3*(n+W+i)] + arr[3*(n+i+W)+1] + arr[3*(n+i+W)+2])/3;
		}
		for (int j=0;j<9;j++){ // Apply Sobel windows
				h=h+Gx[j]*surr[j];
				v=v+Gy[j]*surr[j];
			}
		pix=sqrt(v*v+h*h); // Magnitude of output pixel
		if (pix<70){ // Apply threshold
			pix=0;
		}
		detected[n]=(int)pix; // Write output
		
	}
}
