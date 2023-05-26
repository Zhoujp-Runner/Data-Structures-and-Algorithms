#include<iostream>

using namespace std;

int judge_pointer(int len, int mode);

int judge_pointer(int len, int mode){
    int pointer_slower = 0;
    int pointer_quicker = 0;
    if (mode == 0){//当长度为偶数时，指针指到中间靠后一个位置；当长度为奇数时，指针指到中间位置
        while(pointer_quicker < len-1){
            pointer_quicker += 2;
            pointer_slower += 1;
        }
    }
    else if (mode == 1){//当长度为偶数时，指针指到中间靠前一个位置；当长度为奇数时，指针指到中间位置
        pointer_quicker += 2;
        while(pointer_quicker < len){
            pointer_quicker += 2;
            pointer_slower += 1;
        }
    }
    return pointer_slower;
}


int main(){
    int len = 8;
    cout<<judge_pointer(len, 1)<<endl;
    return 0;
}