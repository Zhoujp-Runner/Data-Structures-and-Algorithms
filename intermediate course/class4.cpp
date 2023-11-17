/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class4.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.最小栈设计
 * 2.栈和队列的互相实现
 * 3.动态规划的空间压缩技巧
 * 最近修改日期：2023-11-17
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-17
 *
 */
#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace std;


/**
 * 最小栈设计
 * 实现一个特殊的栈，在具备栈的基础功能的基础上，能够返回栈中的最小元素
 * 方法：使用两个栈，一个正常存放数据，另一个存放第一个栈中的最小值
 */
class MinStack {
private:
    stack<int> data;
    stack<int> min_num;
public:
    MinStack() = default;
    void push(int x) {
        if (data.empty()) {
            data.push(x);
            min_num.push(x);
        } else {
            data.push(x);
            int top_item = min_num.top();
            if (x < top_item) {  // 当新入栈的元素小于最小值栈栈顶元素
                min_num.push(x);  // 将新入栈的元素压入最小值栈
            } else {  // 当新入栈的元素大于等于最小值栈的栈顶
                min_num.push(top_item);  // 再次将最小值栈栈顶的值入栈
            }
        }
    }

    void pop() {
        if (data.empty()) return;
        // pop时两者需要同时pop()
        data.pop();
        min_num.pop();
    }

    int top() {
        if (data.empty()) {
            cout << "stack is empty!" << endl;
            return -1;
        }
        return data.top();
    }

    int getMin() {
        if (min_num.empty()) {
            cout << "stack is empty!" << endl;
            return INT_MIN;
        }
        return min_num.top();
    }
};


/**
 * 栈和队列互相实现
 * 使用栈实现队列功能：使用两个栈
 * 使用队列实现栈功能：使用两个队列
 */
class Stack2Queue {
private:
    stack<int> in;
    stack<int> out;
public:
    Stack2Queue() = default;
    void push(int x) {
        in.push(x);
    }

    void pop() {
        // 判断out是否为空，不为空就直接pop，为空才需要从in中拿数据
        // 这样的写法 当out pop完一次后，不需要将数据放回in中
        if (!out.empty()) {
            out.pop();
            return;
        }
        while (!in.empty()) {
            out.push(in.top());
            in.pop();
        }
        out.pop();
    }

    int front() {
        if (!out.empty()) {
            return out.top();
        }
        while (!in.empty()) {
            out.push(in.top());
            in.pop();
        }
        return out.top();
    }
};

class Queue2Stack {
private:
    queue<int> q0;
    queue<int> q1;
    int main_queue = 0;  //标记当前存储数据的队列是哪一个队列
public:
    Queue2Stack() = default;
    void push(int x) {
        if (main_queue == 0) {
            q0.push(x);
        } else {
            q1.push(x);
        }
    }

    void pop() {
        if (main_queue == 0) {
            while (q0.size() > 1) {
                q1.push(q0.front());
                q0.pop();
            }
            q0.pop();
        } else {
            while (q1.size() > 1) {
                q0.push(q1.front());
                q1.pop();
            }
            q1.pop();
        }
        // pop 一次之后就需要更新标记
        main_queue ^= 1;
    }

    int top() {
        if (main_queue == 0) return q0.back();
        return q1.back();
    }
};


/**
 * 动态规划的空间压缩技巧
 * 例题：给定一个二维数组matrix，其中每个数都是正数，要求从左上角走到右下角。
 * 每一步只能向右，或者着向下走，沿途经过的数字要累加起来。最后请返回最小的路径和。
 */
int Normal_dp(vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    vector<vector<int>> dp(m, vector<int>(n, matrix[m - 1][n - 1]));
    for (int j = n - 2; j >= 0; j--) {
        dp[m - 1][j] = matrix[m - 1][j] + dp[m - 1][j + 1];
    }
    for (int i = m - 2; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            if (j == n - 1) {
                dp[i][j] = matrix[i][j] + dp[i + 1][j];
            } else {
                dp[i][j] = matrix[i][j] + min(dp[i + 1][j], dp[i][j + 1]);
            }
        }
    }
    return dp[0][0];
}
// 将原本的二维数组优化为一维数组
int Optimize_dp(vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size();
    // 这里可以先判断m和n的大小，选择小的来新建数组
    // 这道题可以这样做，是因为依赖关系，任意一个点依赖右边与下方的点，所以我们可以选择先完成最后一行还是先完成最右边一列
    vector<int> dp(n, matrix[m - 1][n - 1]);
    // 先完成最后一行的初始化
    for (int j = n - 2; j >= 0; j--) {
        dp[j] = matrix[m - 1][j] + dp[j + 1];
    }
    // 再在数组中滚动更新
    for (int i = m - 2; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            if (j == n - 1) {
                dp[j] = matrix[i][j] + dp[j];
            } else {
                dp[j] = matrix[i][j] + min(dp[j], dp[j + 1]);
            }
        }
    }
    return dp[0];
}


int main() {
    vector<vector<int>> m = {{1, 2, 3},
                             {4, 5, 6},
                             {7, 8, 9}};
    cout << Optimize_dp(m) <<endl;
    return 0;
}
