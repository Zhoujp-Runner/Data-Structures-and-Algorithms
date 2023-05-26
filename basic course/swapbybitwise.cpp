#include<iostream>

using namespace std;

void swap(int &a, int &b);
int find1(int arr[]);
void find2(int arr[]);

void swap(int &a, int &b){
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}


int main(){
//	int a = 10;
//	int b = 12;
//	swap(a, b);
//	cout<<a<<b<<endl;
//	return 0;
	int arr[12] = {1,1,1,1,1,2,2,2,3,3,2,2};
//	cout<<sizeof(arr)<<endl;
	find2(arr);
	return 0;
}


//（1）一个数组中存在一个数出现奇数次，其他数出现偶数次，请找出出现奇次数的数
//（2）一个数组中存在两个数出现奇数次，其他数出现偶数次，请找出出现奇次数的数
int find1(int arr[]){
	int error = 0;
	for(int i = 0; i < 12; i++){
		error = error ^ arr[i];
	}
	return error;
}


void find2(int arr[]){
	int error = 0;
	for(int i = 0; i < 12; i++){
		error = error ^ arr[i];
	}
	// 计算最右边不为0的位
	int rightOne = ~error + 1;
	rightOne = error & rightOne;

	int onlyOne = 0;
	for(int i = 0; i < 12; i++){
		if((arr[i] & rightOne) == 1){ // 只与上面计算同样位置不为0的数相与
			onlyOne = onlyOne ^ arr[i];
		}
	}
	cout<<"odd1:"<<onlyOne<<endl;
	onlyOne = error ^ onlyOne;
	cout<<"odd2:"<<onlyOne<<endl;
}



