/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     dynamic_planning.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的基础提升课程中关于由暴力递归到动态规划的内容：
 * 1. 二维表
     a. 机器人行走路线
     b. 兑换零钱问题
 * 最近修改日期：2023-08-29
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-8-28
 *
 */

#include <iostream>
#include <vector>

using namespace std;

////////////////////////////////////////二维表//////////////////////////////////////////
/**
 * 展示  暴力递归（尝试）--->记忆化搜索（缓存）--->严格表依赖的动态规划
 * 改动态规划的方法：
 * 1.找可变参数范围
 * 2.定出最终位置，即最终答案对应的可变参数值
 * 3.通过basecase初始化表
 * 4.根据暴力递归寻找位置依赖
 * 5.根据最终位置以及位置依赖确定表的更新顺序
 */

/**
 * 机器人行走问题
 * 给定一个整数N，表示机器人能够行走的位置总数，即机器人可以行走的位置为 1,2,3,4,5,...,N
 * 给定两个整数S,E，表示机器人的起始位置和最终位置
 * 给定一个整数K，表示机器人一共走了K步
 * 已知机器人每次只能向左或向右移动一步，且当位于1时，只能向右走到2；当位于N时，只能向左走到N-1
 * 求一共有多少种走法
 */
/**
 * 首先采用暴力递归的方法，通过尝试求解
 * N和E都和题中的含义相同，cur表示当前所处的位置，residual表示剩余的步数
 */
int process1(int N, int E, int cur, int residual) {
    if (residual == 0) {  // 当前没有剩余步数可以走了
        return cur == E ? 1 : 0;  // 如果当前位置是终点，则说明有一种方法可以走到终点，返回1,；反之返回0
    }

    if (cur == 1) {  // 如果当前位于1位置，只能走到2位置
        return process1(N, E, 2, residual - 1);
    }
    if (cur == N) {  // 如果当前位于N位置，只能走到N-1位置
        return process1(N, E, N - 1, residual - 1);
    }

    // 除去上面的情况，当前位置的走法，是由向左走一步和向右走一步得到的走法之和
    return process1(N, E, cur - 1, residual - 1) + process1(N, E, cur + 1, residual - 1);
}

/**
 * 使用记忆化搜索优化一下上述的递归：
 * 对上述的递归过程分析一下（由于N和E都是固定的，所以这里省略N和E）
 * f(2, 4) = f(1, 3) + f(3, 3)
 * f(1, 3) = f(2, 2)
 * f(3, 3) = f(2, 2) + f(4, 2)
 * 到这里就可以知道了，f(2, 2)计算了两次，存在了额外的计算开销
 * 如果我们建立一个缓存，将计算结果存储下来，那么每当需要计算时，先去找一下缓存中是否已经有记录，如果有直接返回记录，反之再进行计算
 * 所以我们建立缓存需要考虑的只有两点，递归函数中有哪些量是在变化的，变化的范围是多少
 * 对应该题，cur和residual是变化的，变化范围分别为N和K，
 * 所以假设将缓存建立为二维数组，则 int dp[N+1][K+1];
 */
int process1_memory(int N, int E, int cur, int residual, vector<vector<int>> & dp) {
    if (dp[cur][residual] != -1) {  // 如果当前所处的情况之前计算过，则直接返回缓存中的结果
        return dp[cur][residual];
    }

    if (residual == 0) {  // 当前没有剩余步数可以走了
        dp[cur][residual] = cur == E ? 1 : 0;  // 如果当前位置是终点，则说明有一种方法可以走到终点，返回1,；反之返回0
    } else if (cur == 1) {  // 如果当前位于1位置，只能走到2位置
        process1(N, E, 2, residual - 1);
    } else if (cur == N) {  // 如果当前位于N位置，只能走到N-1位置
        dp[cur][residual] = process1(N, E, N - 1, residual - 1);
    } else {   // 除去上面的情况，当前位置的走法，是由向左走一步和向右走一步得到的走法之和
        dp[cur][residual] = process1(N, E, cur - 1, residual - 1) + process1(N, E, cur + 1, residual - 1);
    }
    return dp[cur][residual];
}

/**
 * 接着我们就可以由记忆化搜索优化到动态规划
 * 可以发现，记忆化搜索还是保留了递归的格式，那我们如何优化到动态规划呢
 * 其实只要考虑路径的依赖关系（这里的路径指的是dp表中的路径）
 * 我们发现，上述的缓存其实是一张二维表，如果弄清楚表中每个格子之间的依赖关系，就可以不用递归直接将表填完，省去了递归的步骤
 * 但是时间复杂度和使用缓存的递归是一样的，都是表的规模
 *
 * 由递归的basecase我们可以知道，表中，residual=0的条件下，只有cur=E才为1，其余情况下为0
 * 当cur=0时，所有数据都没有用，因为没有0位置可以走
 * 当cur=1时，(1, residual)等同于(2, residual - 1)
 * 当cur=N时，(N, residual)等同于(N-1, residual - 1)
 * 其余情况下，(cur, residual)等同于(cur - 1, residual - 1)+(cur + 1, residual + 1)
 * 这样我们就可以直接计算整张表的数值
 */
int process1_dp(int N, int E, int S, int K) {
    vector<vector<int>> dp;
    for (int i = 0; i < N + 1; i ++) {
        vector<int> row(K+1, 0);
        if (i != 0 && i == E) {  // residual=0的条件下，只有cur=E才为1，其余情况下为0
            row[0] = 1;
        }
        dp.emplace_back(row);
    }

    for (int j = 1; j < K+1; j++) {
        for (int i = 1; i < N+1; i++) {  // cur=0的数据没有用，所以从1开始遍历
            if (i == 1) {  // 当cur=1时，(1, residual)等同于(2, residual - 1)
                dp[i][j] = dp[i + 1][j - 1];
            } else if (i == N) {  // 当cur=N时，(N, residual)等同于(N-1, residual - 1)
                dp[i][j] = dp[i - 1][j - 1];
            } else {  // 其余情况下，(cur, residual)等同于(cur - 1, residual - 1)+(cur + 1, residual + 1)
                dp[i][j] = dp[i - 1][j - 1] + dp [i + 1][j - 1];
            }
        }
    }

//    // 打印dp表
//    for (int i = 0; i < N + 1; i++) {
//        for (int j = 0; j < K + 1; j++) {
//            cout << dp[i][j] << ' ';
//        }
//        cout << '\n';
//    }

    return dp[S][K];
}


/**
 * 兑换零钱问题
 * 给你一个vector，代表一堆硬币（存在重复的可能）
 * 给你一个目标值target，你需要用vector中的硬币组合成target
 * 求硬币数量最少的组合方案
 */
/**
 * 暴力递归的方法
 * 自左向右的尝试方法
 * 就是从vector第一个元素开始，分别探讨要和不要，直到vector被遍历完
 */
int process2(vector<int> coins, int index, int residual) {
    // index 表示当前遍历到第几个硬币
    // residual表示剩余还有多少钱需要换成硬币
    if (residual < 0) {  // 如果当前剩余钱的面值已经小于0，说明这种组合方式已经行不通了
        return -1;
    }
    if (index == coins.size()) {  // 如果当前所有硬币已经选完
        return residual == 0 ? 0 : -1;  // 此时如果residual为0，说明该种组合符合要求
    }
    int select = process2(coins, index + 1, residual - coins[index]); // 选当前硬币
    int unselect = process2(coins, index + 1, residual);  // 不选当前硬币
    if (select == -1 && unselect == -1) {  // 如果选和不选都行不通，说明此时已经行不通了
        return -1;
    }
    // 如果两者中有一个为-1 就返回另外一个
    if (select == -1) {
        return unselect;
    }
    if (unselect == -1) {
        return 1 + select;
    }
    // 如果两者都可以组合成功，就返回硬币数少的（1+ 表示选当前硬币要算上当前硬币的数量）
    return min(unselect,  1 + select);
}

/**
 * 记忆化搜索
 * 缓存的范围是index:0~N，residual:0~target
 */
int process2_memory(vector<int> coins, int index, int residual, vector<vector<int>> & dp) {
    // index 表示当前遍历到第几个硬币
    // residual表示剩余还有多少钱需要换成硬币
    if (residual < 0) {
        return -1;
    }
    if (dp[index][residual] != -2) {
        return dp[index][residual];
    }
    if (residual == 0) {
        dp[index][residual] = 0;
    } else if (index == coins.size()) {  // 如果当前所有硬币已经选完
        dp[index][residual] = -1;
    } else {
        int select = process2(coins, index + 1, residual - coins[index]); // 选当前硬币
        int unselect = process2(coins, index + 1, residual);  // 不选当前硬币
        if (select == -1 && unselect == -1) {  // 如果选和不选都行不通，说明此时已经行不通了
            dp[index][residual] = -1;
        } else if (select == -1) {
            dp[index][residual] = unselect;
        } else if (unselect == -1) {
            dp[index][residual] = 1 + select;
        } else {  // 如果两者都可以组合成功，就返回硬币数少的（1+ 表示选当前硬币要算上当前硬币的数量）
            dp[index][residual] = min(unselect,  1 + select);
        }
    }
    return dp[index][residual];
}

/**
 * 动态规划版本
 * 根据basecase初始化
 * 依赖关系可以直接由暴力递归或者记忆化搜索版本修改得到
 */
int process2_dp(vector<int> coins, int target) {
    vector<vector<int>> dp;
    for (int i = 0; i <= coins.size(); i++) {
        if (i == coins.size()) {
            vector<int> row(target + 1, -1);
            row[0] = 0;
            dp.emplace_back(row);
            break;
        }
        vector<int> row(target + 1, -2);
        row[0] = 0;
        dp.emplace_back(row);
    }

    for (int index = coins.size() - 1; index >= 0; index--) {
        for (int residual = 1; residual <= target; residual++) {
            int select = residual - coins[index] >= 0 ? dp[index + 1][residual - coins[index]] : -1;
            int unselect = dp[index + 1][residual];
            if (select == -1 && unselect == -1) {
                dp[index][residual] = -1;
            } else if (select == -1) {
                dp[index][residual] = unselect;
            } else if (unselect == -1) {
                dp[index][residual] = 1 + select;
            } else {
                dp[index][residual] = min(unselect, 1 + select);
            }
        }
    }

//    // 打印dp表
//    for (int i = 0; i <= coins.size(); i++) {
//        for (int j = 0; j <= target; j++ ) {
//            cout << dp[i][j] << ' ';
//        }
//        cout << endl;
//    }

    return dp[0][target];
}

int main() {
    vector<int> coins = {1, 2, 3, 5, 7, 9};
    int target = 13;
    vector<vector<int>> dp;
    for (int i = 0; i < coins.size() + 1; i++) {
        vector<int> row(target + 1, -2);
        dp.emplace_back(row);
    }
    cout << process2_dp(coins, target);

    return 0;
}