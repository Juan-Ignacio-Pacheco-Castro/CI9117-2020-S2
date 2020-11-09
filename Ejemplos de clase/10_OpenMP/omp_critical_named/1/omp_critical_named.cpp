#include <omp.h>
#include <iostream>
using namespace std;

#define SIZE 16
int nums[] = {1,2,3,4,5,6,7,8,10,24,12,43,23,75,92,49};

int main(){
	int a,b;
	a = 0;
	b = 0;
	int num;
	#pragma omp parallel for private(num)
		for(int i = 0; i < SIZE; ++i){
			num = nums[i];
			if(i%2){
				#pragma omp critical(a)
				{
					a += num;
				}
			}
			else{
				#pragma omp critical(b)
				{
					b += num;
				}
			}
		}
		cout << a << " " << b << "\n";
}
