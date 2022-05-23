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
}

