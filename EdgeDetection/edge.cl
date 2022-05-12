//Kernel for OpenCL implementation.

__kernel void Edge(__global int* argument1, __global int* argument2, __global int* output){
	//work item and work groups numbers
	int workItemNum = get_global_id(0); //Work item ID
	int workGroupNum = get_group_id(0); //Work group ID
	int localGroupID = get_local_id(0); //Work items ID within each work group
	
	//memory buffers
	int arg1 = *argument1;
	int arg2 = *argument2;
	uint global_addr = workItemNum;
	
	//short calculation: work Item Number x argument 1 + argument 2
    //This is a test output to see if the Kernel produces an output.
	output[global_addr] = global_addr*arg1 + arg2;
    printf("output[%d] = %d\n", workItemNum, output[workItemNum]);
	
}