/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     big_data_problem.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的基础提升课程中关于位运算题目的内容：
 * 1. 笔记
 * 2. 不使用比较判断，返回两数中较大的数
 * 最近修改日期：2023-08-26
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-8-26
 *
 */

#include <iostream>

using namespace std;

int isPos(int x);  // 判断一个数是否是负数
int flip(int x);  // 翻转 1->0  0->1
int Max_number(int a, int b);  // 不使用比较判断 返回两个数中较大的数

/**
 * @details 判断一个数是否是正数
 * @param
 *      int x:被判断的数
 * @return 负数返回0，正数返回1
 */
int isPos(int x) {
    return flip((x>>31) & 1);
}

int flip(int x) {
    return (x ^ 1);
}


/**
 * @details 不使用比较，返回两个数中较大的那一个
 *          并且，该方法不单独通过差值的正负判断大小
 *          不会受到数值溢出的影响
 * @return 较大的那一个数
 */
int Max_number(int a, int b) {
    int c = a - b;  // 先将两者作差
    int aP = isPos(a);  // 判断a是否是正数
    int bP = isPos(b);  // 判断b是否是正数
    int cP = isPos(c);  // 判断c是否是正数
    int Diff = aP ^ bP;  // 判断a，b是否同号
    int Same = flip(Diff);  // 判断a，b是否异号
    // 返回a的情况是：当a，b异号时，a为正数（可以防止溢出）；或者当a，b同号时，c为正数
    int retrunA = Diff * aP + Same * cP;
    // 返回b的情况：与返回a相反
    int returnB = flip(retrunA);
    return retrunA * a + returnB * b;
}

int main() {
    int a = -12;
    int b = 9;
    cout << Max_number(a, b) << endl;
    return 0;
}

