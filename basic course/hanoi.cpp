/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hanoi.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * 该文件写的是使用递归解决hanoi问题
 * 最近修改日期：2023-01-03
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-01-03
 *
 */

#include <iostream>
#include <string>

using namespace std;

void hanoi(int num, string from, string to, string other);

void hanoi(int num, string from, string to, string other) {
    //num代表要移动的盘子个数，（其实在递归中也代表编号：底部最大，最小的盘子为1，但是这样不易于理解递归）
    if (num == 0) {//当没有盘子可移动时，递归终止
        return;
    }

    hanoi(num - 1, from, other, to);//将前num-1个盘子从from移到other上
    cout<<"move "<<num<<" from "<<from<<" to "<<to<<endl;//将第num个盘子从from移到to上
    hanoi(num - 1, other, to, from);//将前num-1个盘子从other移到to上
}

int main() {
    hanoi(3, "left", "right", "middle");
    return 0;
}
