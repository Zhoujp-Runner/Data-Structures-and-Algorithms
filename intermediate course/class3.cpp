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
 * 最近修改日期：2023-11-02
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-02
 *
 */

#include <iostream>
#include <vector>

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

int main() {
    vector<vector<int>> m = {{0,0,0,0,1},
                             {0,0,1,1,1},
                             {0,0,0,1,1},
                             {0,0,1,1,1},
                             {0,0,0,0,1}};
    vector<int> res = process1_1(m);
    for(const auto& i : res) {
        cout <<i << endl;
    }
    return 0;
}