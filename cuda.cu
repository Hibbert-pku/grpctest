#include<cuda_runtime.h>

__global__ void kernel(int* a, int* b)
{
	a[threadIdx.x] += b[threadIdx.x];
}

void test_kernel(int* a_, int* b_)
{
	int* a;
	int* b;
	cudaMallocManaged(&a, sizeof(int)*10);
	cudaMallocManaged(&b, sizeof(int)*10);
	for(int i = 0; i < 10; i++) {
		a[i] = a_[i];
		b[i] = b_[i];
	}
	kernel<<< 1, 10>>>(a, b);
	cudaDeviceSynchronize();
	for(int i = 0; i < 10; i++) {
                a_[i] = a[i];
                b_[i] = b[i];
        }
}
