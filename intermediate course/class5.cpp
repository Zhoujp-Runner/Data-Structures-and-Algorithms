/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class5.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.相邻数之积为4的倍数的问题
 * 2.快速幂算法（数的快速幂和矩阵的快速幂）
 * 3.斐波那契类似问题的优化（使用矩阵的快速幂进行优化）
 * 最近修改日期：2023-11-21
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-18
 *
 */
#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;


/**
 * 相邻数之积为4的倍数的问题
 * 给定一个数组arr，如果通过调整可以做到arr中任意两个相邻的数字相乘是4的倍数，返回true，否则返回false
 */
/**
 * 方法：统计奇数、只含有一个2因子的偶数以及含有两个及两个以上2因子的偶数
 */
bool nearbyProduct(vector<int>& nums) {
    vector<int> count(3, 0);
    for (int num: nums) {
        if (num % 2 != 0) {  // 奇数
            count[0]++;
        } else if (num % 4 == 0) {  // 含有两个及两个以上2因子的数
            count[2]++;
        } else {  // 只含有一个2因子的数
            count[1]++;
        }
    }

    if (count[1] == 0) {  // 当不存在只含有一个2因子的数时，摆放形式为02020202...或者2020202...（0代表奇数，2代表含有两个及两个以上2因子的数）
        if (count[0] == 1) return count[2] >= 1;  // 如果只含有一个奇数，那么有两个及两个以上2因子的数只需一个就够
        return count[2] >= count[0] - 1;  // 如果含有两个及两个以上的奇数，那么有两个及两个以上2因子的数的个数最少可以是奇数个数-1
    }
    // 当只存在一个只含有2因子的数时，那么4因子的数至少要有1个，且4因子的个数满足第三种情况的讨论
    // （其实这种情况不用考虑，因为数组长度肯定大于等于2，如果count[2]没有，那么必不满足第三种情况的条件）
    if (count[1] == 1) return (count[2] >= 1) && (count[2] >= count[0]);
    // 否则，摆放形式为111...111202020...(0代表奇数，1代表只含有一个2因子的数，2代表含有两个及两个以上2因子的数)
    // 此时，有两个及两个以上2因子的数的个数最少可以是奇数个数
    return count[2] >= count[0];
}


/**
 * 快速幂算法
 * 将一个幂运算的时间复杂度从O(N)优化到O(logN)
 * 核心是利用已有的运算结果，假设要求16次方，我们不再是一次循环乘一次初值，而是考虑16次方可以由两个8次方得到
 * 进一步，将幂用二进制展开，也就是说x^{N}的N可以用二进制表示，
 * 那么假设可以将 N 表示为 abcd 四位二进制，那么x^{N}就可以划为 x^{d*1} * x^{c*2} * x^{b*4} * x^{a*8}
 * 可以看到x平方可以得到x^{2}，x^{2}的平方可以得到x^{4}，以此类推
 * 那么我们可以在每次循环不断使一个数更新为自己的平方，然后查看当前位数的二进制上是0还是1，如果是1，就将结果乘上该数，如果是0结果不变
 * **注意：如果数很大的话还需要取模运算，博客里要提一下，这里为了展示清晰的逻辑就不改了**
 */
int fast_pow(int x, int y) {
    int res = 1;  // 结果初始化为1，如果是矩阵的快速幂，那就初始化为单位矩阵
    while (y) {  // 判断是否遍历完
        if (y & 1) {  // 如果幂次当前位上是1
            res *= x;  // 如果是矩阵的快速幂，需要自己实现一个两个矩阵相乘的函数
        }
        x *= x;  // 将x更新为自己的平方，即x^{n} = x^{2*n}，如果是矩阵的快速幂，这里同样使用矩阵相乘的函数
        y >>= 1;  // 下一次遍历下一位
    }
    return res;
}

// 方阵的快速幂
vector<vector<int>> fast_pow_matrix(vector<vector<int>>& matrix, int y) {
    int m = matrix.size();
    // 初始化为单位矩阵
    vector<vector<int>> res(m, vector<int>(m, 0));
    for (int i = 0; i < m; i++) {
        res[i][i] = 1;
    }
    // 计算两个方阵相乘的结果
    function<vector<vector<int>>(vector<vector<int>>&, vector<vector<int>>&)> dot = [&](vector<vector<int>>& a, vector<vector<int>>& b) {
        vector<vector<int>> res(m, vector<int>(m, 0));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < m; k++) {
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return res;
    };
    // 逻辑和数的快速幂算法一致
    while (y) {
        if (y & 1) {
            res = dot(res, matrix);
        }
        matrix = dot(matrix, matrix);
        y >>= 1;
    }
    return res;
}


/**
 * 斐波那契数列问题的优化
 * 基础的斐波那契数列问题的时间复杂度为O(N)
 * 可以用矩阵的快速幂将时间复杂度优化到O(logN)
 * 更一般地，除了初值之外，具有严格递推关系式的问题（一般递推关系是加减），都可以利用矩阵的快速幂算法优化时间复杂度
 * 斐波那契数列问题的优化：
 * 递推关系式为（还可以扩展到更多项，以及每一项前的系数都是可变的） f(N) = f(N - 1) + f(N - 2)
 * 初值为 f(1) = 1, f(2) = 1
 * 那么我们可以构建矩阵的递推关系（用 | 代表矩阵）
 *
 * |f(3)|   | a  b |   |f(2)|
 * |    | = |      | * |    |   (1)
 * |f(2)|   | c  d |   |f(1)|
 *
 * |f(4)|   | a  b |   |f(3)|
 * |    | = |      | * |    |   (2)
 * |f(3)|   | c  d |   |f(2)|
 *
 * 通过(1)(2)两式可以求解出a,b,c,d，也就是系数矩阵
 * 那么递推关系式就变成了
 * M(N) = A^{N - 2} * M(2)      (3)
 * 式中
 *        |  f(N)  |        | a  b |
 * M(N) = |        |,   A = |      |
 *        |f(N - 1)|        | c  d |
 * 所以，其中A^{N - 2}这一项就可以用矩阵的快速幂以O(logN)的时间复杂度计算出来
 */
int Fibonacci_optimize(int N) {
    if (N == 1 || N == 2) return 1;
    if (N == 3 || N == 4) return N == 3 ? 2 : 3;
    // 通过(1)和(2)求解出的A矩阵
    vector<vector<int>> A = {{1, 1}, {1, 0}};
    // 计算出A矩阵的N-2次幂
    vector<vector<int>> APowNMinus2 = fast_pow_matrix(A, N - 2);
    // 最终由式(3)得到答案
    return APowNMinus2[0][0] + APowNMinus2[0][1];
}


int main() {
//    vector<int> nums = {1, 3, 2};
//    cout << nearbyProduct(nums);
    cout << Fibonacci_optimize(13);
    return 0;
}
