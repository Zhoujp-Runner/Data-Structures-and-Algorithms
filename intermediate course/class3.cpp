/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class3.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.右上角开始查找的模型(两道例题)
 * 2.打包机器问题（洗衣机问题）
 * 3.用宏观的方式解决局部问题(两道例题)
 * 最近修改日期：2023-11-03
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-03
 *
 */

#include <iostream>
#include <vector>
#include <functional>

using namespace std;

/**
 * 右上角开始查找的模型（可以从其他角点开始查找，具体题目可以具体尝试）
 * 一般适用于在matrix中的查找问题
 */
/**
 * 给定一个非负整数二维matrix，该matrix的行和列都是升序，但整体无序
 * 给定一个数
 * 请在matrix中找到这个数，有返回true，没有返回false
 */
bool process1(vector<vector<int>>& matrix, int target) {
    // 从右上角，从上往下，从右往左找
    int n = matrix.size();
    int m = matrix[0].size();
    int i = 0;
    int j = m - 1;
    while (i >= 0 && i < n && j >= 0 && j < m) {
        if (matrix[i][j] == target) {
            return true;
        }
        if (matrix[i][j] > target) { // 如果比目标值大，说明同一列中，下面的行不会有target存在，所以往左走
            j--;
        } else if (matrix[i][j] < target) { // 同理，此时同一行中，左边的数不会有target存在，所以往下走
            i++;
        }
    }
    return false;
}

/**
 * 给定一个二维数组，每一行中，0肯定在1的左边
 * 每一行的0和1单独的个数不一定相同，但是总数相同
 * 求拥有最多1的行，如果有多行都有最多的1，那么都需要返回
 */
vector<int> process1_1(vector<vector<int>>& matrix) {
    // 从右上角，从上往下，从左往右找
    int n = matrix.size();
    int m = matrix[0].size();
    int i = 0;
    int j = m - 1;
    vector<int> res;
    while (i >= 0 && i < n && j >= 0 && j < m) {
        if (matrix[i][j] == 0) {
            i++;
        } else {
            if (j == 0) {
                i++;
                continue;
            }
            int count = 0;
            while (j - 1 >= 0 && matrix[i][j - 1] == 1) {
                count++;
                j--;
            }
            if (count > 0) {
                res.clear();
                res.emplace_back(i);
            } else if (count == 0) {
                res.emplace_back(i);
            }
            i++;
        }
    }

    return res;
}


/**
 * 打包机器问题（洗衣机问题）
 * 有n个打包机器从左到右依次排开，每个打包机器上的物品数量有多有少
 * 必须保证n个打包机器上的物品数量一致时才能进行打包
 * 由于物品重量大，每次只能搬运一个物品，且只能将一个物品搬运到相邻的机器上
 * 所有机器都对应一个搬运工人，每一轮中所有机器对应的工人同时进行搬运
 * 请计算能够使每个打包机器上的物品数量的相等的最小移动轮数
 * 如果不能使每个机器上的数量相等，返回-1
 */
/**
 * 思路：单独考虑一台机器左右两边，为了达到均值所需要经过该台机器的流动物品数量，记为流量
 * 所有机器的流量最大值即为答案
 */
int minTimes_PackageMachine(int n, vector<int> items) {
    if (n == 1) return 0;

    // 计算物品总数
    int sum = 0;
    for (auto item : items) {
        sum += item;
    }

    if (sum % n != 0) return -1;  // 如果不能被整除，说明不能达到每个机器上的物品相同的情况

    int average = sum / n, left_sum = 0, max_flow = 0;
    for (int i = 0; i < n; i++) {
        // 计算左右两边各自需要多少物品，正数为需要搬出的物品，负数为需要搬入的物品
        int left_need = left_sum - average * i;
        int right_need = sum - left_sum - items[i] - average * (n - i - 1);
        if (left_need < 0 && right_need < 0) {  // 如果两边都需要搬入，但是一个机器一轮只能搬出一件，所以是求和
            max_flow = max(abs(left_need) + abs(right_need), max_flow);
        } else {  // 其他情况下都是取最大值
            max_flow = max(max(abs(left_need), abs(right_need)), max_flow);
        }
    }
    return max_flow;
}


/**
 * 用宏观的方式解决局部问题
 */
/**
 * 用螺旋的方式打印矩阵（顺时针打印），例如
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 * 打印结果为：0 1 2 3 7 11 15 14 13 12 8 4 5 6 10 9
 */
void process2_1(vector<vector<int>> matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // 只顺时针打印以（a，b）和（c，d）为角点的边界
    function<void(int, int, int, int)> print_edge = [&](int a, int b, int c, int d) {
        if (a == c) {  // 如果两个角点在同一行
            for (int i = b; i <= d; i++) {
                cout << matrix[a][i] << ' ';
            }
        } else if (b == d) {  // 如果两个角点在同一列
            for (int i = a; i <= c; i++) {
                cout << matrix[i][b] << ' ';
            }
        } else {  // 两个角点不同行不同列，能构成一个矩形
            for (int i = b; i <= d; i++) {
                cout << matrix[a][i] << ' ';
            }
            for (int i = a + 1; i <= c; i++) {
                cout << matrix[i][d] << ' ';
            }
            for (int i = d - 1; i >= b; i--) {
                cout << matrix[c][i] << ' ';
            }
            for (int i = c - 1; i > a; i--) {
                cout << matrix[i][b] << ' ';
            }
        }
    };

    int a0 = 0, b0 = 0, c0 = m - 1, d0 = n - 1;
    while (a0 <= c0 && b0 <= d0) {  // 保证abcd的大小顺序
        print_edge(a0++, b0++, c0--, d0--);  // 以斜45角度从矩阵边缘向矩阵中心遍历
    }
}

/**
 * 将方阵顺时针旋转90度，例如
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 * 结果为：
 * 12 8  4  0
 * 13 9  5  1
 * 14 10 6  2
 * 15 11 7  3
 */
vector<vector<int>> process2_2(vector<vector<int>> matrix) {
    int m = matrix.size();
    int n = matrix[0].size();

    function<void(int, int, int, int)> rotate_edge = [&](int a, int b, int c, int d) {
        // 左右对称
        for (int i = a; i <= c; i++) {
            int temp = matrix[i][b];
            matrix[i][b] = matrix[i][d];
            matrix[i][d] = temp;
        }
        for (int i = 1; i <= (d - b - 1) / 2; i++) {
            int temp1 = matrix[a][b + i];
            matrix[a][b + i] = matrix[a][d - i];
            matrix[a][d - i] = temp1;

            int temp2 = matrix[c][b + i];
            matrix[c][b + i] = matrix[c][d - i];
            matrix[c][d - i] = temp2;
        }

        // 负对角线对称
        for (int i = 0; i <= d - b; i++) {
            int temp = matrix[a][b + i];
            matrix[a][b + i] = matrix[c - i][d];
            matrix[c - i][d] = temp;
        }
        for (int i = 1; i <= d - b; i++) {
            int temp = matrix[a + i][b];
            matrix[a + i][b] = matrix[c][d - i];
            matrix[c][d - i] = temp;
        }
    };

    int a0 = 0, b0 = 0, c0 = m - 1, d0 = n - 1;
    while (a0 < c0 && b0 < d0) {
        rotate_edge(a0++, b0++, c0--, d0--);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << ' ';
        }
        cout << endl;
    }

    return matrix;
}

int main() {
    vector<vector<int>> m = {{0,1,2,3,0},
                             {4,5,6,7,0},
                             {8,9,10,11,0},
                             {12,13,14,15,0},
                             {1,2,3,4,5}};
    process2_2(m);
//    vector<vector<int>> m = {{0,0,0,0,1},
//                             {0,0,1,1,1},
//                             {0,0,0,1,1},
//                             {0,0,1,1,1},
//                             {0,0,0,0,1}};
//    vector<int> res = process1_1(m);
//    for(const auto& i : res) {
//        cout <<i << endl;
//    }
    return 0;
}