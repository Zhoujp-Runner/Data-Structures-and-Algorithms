#include<iostream>
#include<cstdlib>
#include<ctime>
#include<algorithm>
#include<cmath>

using namespace std;

void insert_sort(int arr[], int len);//插入排序
void swap(int* arr, int i, int j);//交换两个数
void merge_sort(int arr[], int len);//归并排序
void process(int arr[], int left, int right);//归并排序的递归调用函数
int small_process(int arr[], int left, int right);//小和问题调用的变体归并递归
int* get_arr(int size);//获得动态数组
void merge_arr(int arr[], int left, int right);//归并排序的融合函数
int merge_arr(int arr[], int left, int mid, int right);//小和问题的变体融合函数
void small_sum(int arr[], int len);//小和问题
void quick_sort(int arr[], int len);//快速排序（可解荷兰国旗问题）
int* quick_partion(int arr[], int left, int right);//快速排序单次执行的任务
void quick_process(int arr[], int left, int right);//快速排序递归
void heap_sort(int arr[], int len);//堆排序
void heap_insert(int arr[], int index);//向上调整
void heapify(int arr[], int index, int heap_size);//向下调整
void count_sort(int arr[], int len);//计数排序
void bucket_sort(int arr[], int len);//桶排序
int count_num_max_len(int arr[], int len);//计算数组中最大数的长度（即有几位）

void swap(int arr[], int i, int j){
//	arr[i] = arr[i] ^ arr[j];
//	arr[j] = arr[i] ^ arr[j];
//	arr[i] = arr[i] ^ arr[j];
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

// 插入排序
void insert_sort(int arr[], int len){
	if(len <= 1){
		return;
	}
	for(int i = 1; i < len; i++){
		for(int j = i - 1; j >= 0 && arr[j] > arr[j + 1]; j--){
			swap(arr, j, j + 1);
		}
	}
}

//归并排序
void merge_sort(int arr[], int len){
	if(len <= 1){
		return;
	}
	int right = len - 1;
	int left = 0;
	process(arr, left, right);
	cout<<"done..."<<endl;
}

//递归函数
void process(int arr[], int left, int right){
	if(left == right){
		return;
	}

	int mid = left + ((right - left) >> 1);
	process(arr, left, mid);//左边递归
	process(arr, mid+1, right);//右边递归
	merge_arr(arr, left, right);
}

//归并排序中融合左右两块数组
void merge_arr(int arr[], int left, int right){
	int mid = (left + right) / 2;
	int* help = get_arr(right - left + 1);

	int p_left = left;
	int p_right = mid + 1;
	int i = 0;
	while(p_right<=right && p_left <=mid){
		if(arr[p_right] > arr[p_left]){
			help[i++] = arr[p_left++];
		}
		else if(arr[p_right] < arr[p_left]){
			help[i++] = arr[p_right++];
		}
		else if(arr[p_right] == arr[p_left]){
			help[i++] = arr[p_right++];
		}
	}
	while(p_right<=right){
		help[i++] = arr[p_right++];
	}
	while(p_left<=mid){
		help[i++] = arr[p_left++];
	}
	for(int j = 0; j < (right - left + 1); j++){
		arr[left + j] = help[j];
	}
	free(help);
	help = NULL;
}

//得到动态数组
int* get_arr(int size){
	int* p = (int*)malloc(sizeof(int) * size);
	return p;
}

//小和问题：统计一个数组中每个元素之前比该元素小的数的和
//思路：使用归并排序的思想
int small_process(int arr[], int left ,int right){
	if(left == right){
		return 0;
	}

	int mid = left + ((right - left) >> 1);
	int sum_left = small_process(arr, left, mid);//左边递归
	int sum_right = small_process(arr, mid+1, right);//右边递归
	int sum_now = merge_arr(arr, left, (left+right)/2, right);
	return (sum_right + sum_left + sum_now);
}

int merge_arr(int arr[], int left, int mid, int right){
	int* help = get_arr(right - left + 1);

	int p_left = left;
	int p_right = mid + 1;
	int i = 0;
	int result = 0;
	while(p_right<=right && p_left <=mid){
		if(arr[p_right] > arr[p_left]){
			result += arr[p_left] * (right - p_right + 1);
			help[i++] = arr[p_left++];
		}
		else if(arr[p_right] < arr[p_left]){
			help[i++] = arr[p_right++];
		}
		else if(arr[p_right] == arr[p_left]){
			help[i++] = arr[p_right++];
		}
	}
	while(p_right<=right){
		help[i++] = arr[p_right++];
	}
	while(p_left<=mid){
		help[i++] = arr[p_left++];
	}
	for(int j = 0; j < (right - left + 1); j++){
		arr[left + j] = help[j];
	}
	free(help);
	help = NULL;
	return result;
}

void small_sum(int arr[], int len){
	if(len <= 1){
		return;
	}
	int right = len - 1;
	int left = 0;
	int mid = left + ((right - left) >> 1);
	int sum = small_process(arr, left, right);
	cout<<"sum = "<<sum<<endl;
	cout<<"done..."<<endl;
}

//快速排序（可解荷兰国旗问题）
void quick_sort(int arr[], int len){
	if(len<=1){
		return;
	}
	quick_process(arr, 0, len-1);
	cout<<"done..."<<endl;
}

//int* quick_partion(int arr[], int left, int right){
//	int p1 = left - 1;
//	int p2 = right;
//	int i = left;
//	while(i < p2-1){
//		if(arr[i] < arr[right]){
//			p1++;
//			i++;
//		}
//		else if(arr[i] > arr[right]){
//			swap(arr, i, p2-1);
//			p2--;
//		}
//		else if(arr[i] == arr[right]){
//			i++;
//		}
//	}
//	if(arr[i] < arr[right]){
//		p1++;
//		p2++;
//	}
//	else if(arr[i] > arr[right]){
//		p2++;
//	}
//	swap(arr, p2, right);
//	int p[2];
//	p[0] = p1;
//	p[1] = p2;
//	return p;
//}

int* quick_partion(int arr[], int left, int right){
	int less = left - 1;
	int more = right;
	int p = left;
	while(p < more){
		if(arr[p] < arr[right]){
			swap(arr, ++less, p++);
		}
		else if(arr[p] > arr[right]){
			swap(arr, --more, p);
		}
		else{
			p++;
		}
	}
	swap(arr, more, right);
//	int bound[]={less+1, more}; //不能这样写，因为bound在栈上，出了函数作用域就会销毁
    int* bound = get_arr(2);
    bound[0] = less + 1;
    bound[1] = more;
	return bound;
}

void quick_process(int arr[], int left, int right){
	if(left < right){
		int rand_num = ((rand() % 99)/100.0)* (right - left + 1);
		swap(arr, rand_num+left, right);
		int* p = quick_partion(arr, left, right);
		quick_process(arr, left, p[0] - 1);
		quick_process(arr, p[1] + 1, right);
	}
}

//堆排序
void heap_sort(int arr[], int len){
    if(len <= 1){
        return;
    }
    //先使用向上调整，使数组成为堆
    for(int i = 0; i < len; i++){
        heap_insert(arr, i);
    }
    //使用向下调整，进行排序
    int heap_size = len;
    while(heap_size > 0){
        swap(arr, heap_size - 1, 0);
        heap_size--;
        heapify(arr, 0, heap_size-1);
    }
}

//向上调整
void heap_insert(int arr[], int index){
    while(arr[index] > arr[(index - 1) / 2]){
        swap(arr, index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

//向下调整
void heapify(int arr[], int index, int heap_size){
    int left = index * 2 + 1;
    int largest = (left + 1) <= heap_size && arr[left] < arr[left + 1] ? (left + 1) : left;
    while(arr[index] < arr[largest]){
        if (left > heap_size){ //如果没有孩子了，就跳出循环
            break;
        }
        swap(arr, index, largest);
        index = largest;
        left = index * 2 + 1;
        if (left > heap_size){ //如果没有孩子了，就跳出循环
            break;
        }
        if((left + 1) <= heap_size){
            largest = arr[left] > arr[left + 1] ? left : (left + 1);
        }
        else{
            largest = left;
        }
    }
//简洁的代码
//    int left = index * 2 + 1;
//    while(left < heap_size){
//        int largest = (left + 1) <= heap_size && arr[left] > arr[left + 1] ? left : (left + 1);
//        largest = arr[largest] > arr[index] ? largest : index;
//        if(largest == index){
//            break;
//        }
//        swap(arr, largest, index);
//        index = largest;
//        left = index * 2 + 1;
//    }
}

//计数排序
void count_sort(int arr[], int len){
    if(len <= 1){
        return;
    }
    int bucket[10];
    for(int i = 0; i < 10; i++){
        bucket[i] = 0;
    }
    for(int i = 0; i < len; i++){
        int value = arr[i];
        bucket[value]++;
    }
    int index = 0;
    for(int j = 0; j < 10; j++){
        while(bucket[j] != 0){
            arr[index] = j;
            bucket[j]--;
            index++;
        }
    }
}

//桶排序
void bucket_sort(int arr[], int len){
    int max_len = count_num_max_len(arr, len);
    int* help = new int[len];
    for(int i = 0; i < max_len; i++){//根据最大数的长度进行循环
        int bucket[10] = {0};
        //根据该位上的数字进行统计
        for(int j = 0; j < len; j++){
            int element = int(arr[j] / (pow(10, i)));
            bucket[element % 10]++;
        }
        //将数组中的第j项变成前j项之和
        for(int j = 1; j < 10; j++){
            bucket[j] = bucket[j] + bucket[j - 1];
        }
        //根据bucket数组将桶中的数按顺序放入help数组
        for(int j = len - 1; j >= 0; j--){
            int element = int(arr[j] / (pow(10, i)));
            int radix = element % 10;
            help[bucket[radix] - 1] = arr[j];
            bucket[radix]--;
        }
        //将arr数组按help数组排列
        for(int j = 0; j < len; j++){
            arr[j] = help[j];
        }
    }
}

//计算最大数的位数
int count_num_max_len(int arr[], int len){
    int max = *max_element(arr, arr+len-1);
    int max_len = 0;
    while(max > 0){
        max /= 10;
        max_len++;
    }
    return max_len;
}

int main(){
	int arr[9] = {1,30,2,6,8,5,4,9,7};
	int len = sizeof(arr)/sizeof(int);
	srand(time(NULL));
    bucket_sort(arr, len);
//    cout<<count_num_max_len(arr, len)<<endl;
	for(int i = 0; i < len; i++){
		cout<<arr[i]<<endl;
	}
	cout<<"main done.."<<endl;
//	int* p = get_arr(8);
//	p[5] = 6;
//	p[0] = 1;
//	int size = sizeof(p)/sizeof(int*);
//	cout<<p[5]<<endl;
//	cout<<p[0]<<endl;
//	cout<<size<<endl;
//	srand(time(NULL));
//	for(int i = 0; i < 10; i++){
//
//		int num = ((rand() % 99)/100.0)*50;
//		cout<<num<<endl;
//	}
	return 0;
}



