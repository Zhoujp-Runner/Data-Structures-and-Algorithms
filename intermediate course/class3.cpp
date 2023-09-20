/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class1.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.右上角开始查找的模型(两道例题)
 * 最近修改日期：2023-09-20
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-9-20
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