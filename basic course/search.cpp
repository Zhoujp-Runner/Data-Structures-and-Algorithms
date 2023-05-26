#include<iostream>

using namespace std;

int bin_search(int arr[], int len, int target);
int question1(int arr[]);

int bin_search(int arr[], int len, int target){
	if(len <= 1){
		return arr[0];
	}

	int right = len - 1;
	int left = 0;

	while(left <= right){
		int mid = (right + left)/2;
		if(arr[mid] > target){
			right = mid - 1;
		}
		else if(arr[mid] < target){
			left = mid + 1;
		}
		else if(arr[mid] == target){
			return mid;
		}
	}
	return -1;
}

//找一个无序数组的局部最小值
//还有点小问题
int question1(int arr[], int len){
	//先判断数组的左右两端是不是局部最小值
	if(arr[0] < arr[1] || len == 1){
		return arr[0];
	}
	else if(arr[len-1] < arr[len-2]){
		return arr[len-1];
	}

	int right = len - 1;
	int left = 0;
	while(left <= right){
		cout<<"cal..."<<endl;
		int mid = (left + right)/2;
		if((arr[mid] < arr[mid+1]) && (arr[mid] < arr[mid-1])){
			return arr[mid];
		}
		else if(!(arr[mid] < arr[mid+1])){
			left = mid + 1;
		}
		else if(!(arr[mid] < arr[mid-1])){
			right = mid - 1;
		}

		if(arr[left] == arr[left]+1){
			left = left+1;
		}
		else if(arr[right] == arr[right-1]){
			right = right+1;
		}
	}
	cout<<"done..."<<endl;
	return -1;
}


int main(){
	int arr[] = {3,3,3,4,5,6,8,7,9};
	int index = question1(arr, 9);
	cout<<index<<endl;
	return 0;
}




