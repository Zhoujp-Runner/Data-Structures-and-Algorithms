/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中的滑动窗口算法，包含：
 * 笔记
 * 滑动窗口算法的实现
 * 最近修改日期：2023-06-12
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-6-12
 *
 */

/*****************笔记*****************
 * 还是以问题引入这种结构
 * 问题：给定一个数组：[x1, x2, x3, ...]
 *      再给定一个窗口大小m，规定窗口每次只能向右滑动一个元素的位置
 *      窗口的初始位置为[[x1, x2, ..., xm,] xm+1, xm+2, ...]
 *      求每次滑动过程中，窗口中的最大值是多少
 *      要求返回一个数组
 *
 * 普通解法：暴力解法，从左到右，每当窗口滑动一次，就对窗口内的元素进行一次比较，求得最大值
 *          很显然，这种方式的时间复杂度是很大的，假设数组里面有N个元素，窗口大小为M，那么时间复杂度为O(M*N)
 *
 * 滑动窗口：首先，将窗口看成双指针，分别代表窗口的左边界L和右边界R（注意L不能大于R）
 *          准备一个双端队列，给双端队列中的数制定一个规则，在上面这个问题中，规则为，双端队列从头到尾需要保证从大到小的规则
 *          当R往右移时，往双端队列的尾部加入新的数，如果不满足规则，那么就从尾部弹出一个数（弹出的数不放回），直到满足规则为止
 *          当L往右移时，这时候，会有元素从窗口中消失，如果该元素在双端队列的头部，那么就从头部弹出该数，否则不进行操作
 *          为什么这样子就能保证每次取得最大值？
 *          首先，当前的最大值肯定是在头部的值，这个不用证明。
 *          那为什么当L往右移时只需要看该元素是不是双端队列的头部节点？
 *          因为R右移的时候保证了，双端队列保存的是哪个元素作为最大值的先后顺序；
 *          假设x1=3，x2=2，x3=4那么当R移动到x3时，前面的元素都已经被弹出了，
 *          那么就说明，我这个窗口里，R保持不动，只要L没有移动到x3，那么最大值就是x3，
 *          意思是，x3比x1与x2更迟从窗口中消失，还比两者都大，所以前面两者从窗口中消失并不会影响到最大值
 *          “我比你后过期，我值还比你大”
 */

#include <iostream>
#include <deque>

using namespace std;

class SlideWindow {

public:
    int* getMaxArray(int array[], int len, int window_size) {
        if (len == 0) return nullptr;
        if (!q.empty()) q.clear();

        left = 0;
        right = 0;

        int* max_array = new int [len - window_size + 1];

        // 首先根据滑动窗口大小先将right调整至指定位置
        while (right != window_size) {
            while (!q.empty() && array[q.back()] <= array[right]) {
                q.pop_back();
            }
            q.push_back(right);
            right++;
        }

        max_array[0] = array[q.front()];

        // right和left同时向右移动
        while (right < len) {
            while (!q.empty() && array[q.back()] <= array[right]) {
                q.pop_back();
            }
            q.push_back(right);
            right++;

            if (left == q.front()) {
                q.pop_front();
            }
            left++;

            max_array[left] = array[q.front()];
        }

        return max_array;
    }

private:
    deque<int> q;
    int left=0;
    int right=0;
};


int main() {

    SlideWindow s;
    int array[] = {1, 3, 5, 2, 1, 6, 9, 7, 8, 0};
    int* max = s.getMaxArray(array, 10, 3);
    for (int i = 0; i < 8; i++) {
        cout << max[i] << endl;
    }

    return 0;

}