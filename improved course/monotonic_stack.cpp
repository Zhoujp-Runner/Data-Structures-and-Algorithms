/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中关于单调栈的算法，包含：
 * 笔记
 * 单调栈算法的实现
 * 最近修改日期：2023-06-18
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-6-13
 *
 */


/*****************笔记*******************
 * 问题：寻找一个数组中，每一个元素，左右两侧比他大的最近元素的位置
 *
 * 普通解法：遍历每一个元素，然后向两侧开始比较，直到找到比他本身大的数
 *          时间复杂度为O(N^2)
 *
 * 单调栈解法：准备一个栈，要求栈中的元素维持栈底到栈顶依次由大到小的规则
 *          遍历元素，每个元素依次进栈
 *          当出现当前元素无法进栈（不满足由大到小的规则）时，依次从栈中弹出元素，直到当前元素可以进栈
 *          记录弹出元素的信息，弹出元素的左边比他大的是栈中下一个数，右边比他大的数是当前的数
 *          当遍历完数组之后，再把所有数弹出，依次记录信息，得到最终结果
 */

#include <stack>
#include <iostream>

using namespace std;

class Monotonic_stack {
public:
    void get_bigger_value(int arr[], int len) {
        if (len <= 1) return;
        if (!m_stack.empty()) m_stack.empty();

        int * right = new int [len];
        int * left = new int [len];
        int top_index;

        for (int i = 0; i < len; i++) {
            if (m_stack.empty()) {
                m_stack.push(i); // 注意这里是将索引入栈，以便于锁定元素位置
                continue;
            }

            while (arr[m_stack.top()] < arr[i]) {
                top_index = m_stack.top();
                m_stack.pop();

                right[top_index] = arr[i];
                if (m_stack.empty()) {
                    left[top_index] = -1;
                    m_stack.push(i);
                    break;
                } else {
                    left[top_index] = arr[m_stack.top()];
                }
            }

            m_stack.push(i);
        }

        while (!m_stack.empty()) {
            top_index = m_stack.top();
            m_stack.pop();

            right[top_index] = -1;
            if (m_stack.empty()) {
                left[top_index] = -1;
            } else {
                left[top_index] = arr[m_stack.top()];
            }
        }

        for (int j = 0; j < len; j++) {
            cout << "left:" << left[j] << " " << "current:" << arr[j] << " " << "right:" << right[j] << endl;
        }
    }
private:
    stack<int> m_stack;
};

int main() {
    int arr[8] = {1, 7, 4, 5, 3, 9, 0, 2};
    Monotonic_stack m;
    m.get_bigger_value(arr, 8);
    return 0;
}
