/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     dynamic_planning.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的基础提升课程中关于由暴力递归到动态规划的内容：
 * 总结一下
 * 1. 二维表
 *      a. 机器人行走路线
 *      b. 兑换零钱问题
 *      c. 先后手问题（零和博弈）
 * 2. 三维表
 *      a. 棋子“马”步数问题
 *      b. 生存几率问题（里面还有一个求最大公约数的方法）
 * 3. 优化动态规划
 *      a. 组出某个钱数的方法数（背包问题）---斜率优化
 * 最近修改日期：2023-09-01
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-8-28
 *
 */

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

/*******************************总结一下*******************************
 * 首先尝试（尝试方法一般有从左往右和范围上尝试，这两种尝试方法可以解决面试过程中基本上7成的递归问题）
 * 其次根据尝试改出记忆化搜索
 * 然后根据记忆化搜索改出动态规划（初始化+依赖+更新顺序）
 * 根据动态规划，观察依赖的特点，优化动态规划（这里只展示了枚举行为的斜率优化，中高级班会讲其他的优化）
 *
 * 如何确定一个尝试是好的尝试？（无法确定，但是我们需要用尝试改到动态规划，这时候依据这一点我们可以有一些评价标准）
 * 1. 单个可变参数的维度（最好是1维）：可变参数的维度决定着dp表中当前维度的深度，如果是1维，那么这个数的大小范围就是当前维度的深度，
 *      但是如果是一个数组（大于1维），如果没有一些约束，那么变化情况就无限了，深度不可估计
 * 2. 可变参数的个数（越少越好）：可变参数的个数决定着dp表的维度数量
 **********************************************************************/

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
 * 尝试方法：通过点将问题分割为子问题
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
 * 尝试方法：自左向右
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


/**
 * 先手后手问题
 * 给你一个数组，表示一堆价值不一的东西
 * 有A和B两个人，每个人轮流从数组中拿走一样东西
 * 但是有一个规则，拿的时候只能拿数组两端的东西
 * 假设A和B都绝顶聪明，A能决定先后手
 * 注意，数组中的数都是正数，是零和博弈，也就是说你的收益和对手的损失是相等的
 * 问A能拿到的最大总价值数
 */
/**
 * 暴力递归的方法
 * 尝试方法：范围型尝试
 * 在left,right范围上先后手
 */
int first(vector<int> arr, int left, int right);  // 先手函数
int second(vector<int> arr, int left, int right);  // 后手函数

int first(vector<int> arr, int left, int right) {
    if (left == right) {  // 如果此时范围中只有一个物品，作为先手，当然直接拿
        return arr[left];
    }

    // 为什么是max，因为此刻你具有选择权，你必定会选择最大的价值
    return max(arr[left] + second(arr, left + 1, right),  // 如果选left，那么所获得的价值就是当前点和在选完之后区间上的后手所得到的价值
               arr[right] + second(arr, left, right - 1));  // 选right也同理
}

int second(vector<int> arr, int left, int right) {
    if (left == right) {  // 如果此时范围中只有一个物品，作为后手，当然不能拿
        return 0;
    }

    // 为什么是min，因为你此时是后手，在这个区间上，对手是先手，对手肯定会使得你在下一个区间上先手所能获得的价值最小
    return min(first(arr, left + 1, right),
               first(arr, left, right - 1));
}

int process3(vector<int> arr) {  // 返回你在整个区间上先手和后手所能获得的价值的最大值
    return max(first(arr, 0, arr.size() - 1),
               second(arr, 0, arr.size() - 1));
}

/**
 * 记忆化搜索版本
 * 两个dp表
 */
int first_memory(vector<int> arr, int left, int right, vector<vector<int>>& dp_first, vector<vector<int>>& dp_second);  // 先手函数
int second_memory(vector<int> arr, int left, int right, vector<vector<int>>& dp_first, vector<vector<int>>& dp_second);  // 后手函数
int first_memory(vector<int> arr, int left, int right, vector<vector<int>>& dp_first, vector<vector<int>>& dp_second) {
    if (dp_first[left][right] != -1) {
        return dp_first[left][right];
    }

    if (left == right) {  // 如果此时范围中只有一个物品，作为先手，当然直接拿
        dp_first[left][right] = arr[left];
    } else {
        // 为什么是max，因为此刻你具有选择权，你必定会选择最大的价值
        dp_first[left][right] =  max(arr[left] + second_memory(arr, left + 1, right, dp_first, dp_second),  // 如果选left，那么所获得的价值就是当前点和在选完之后区间上的后手所得到的价值
                                    arr[right] + second_memory(arr, left, right - 1, dp_first, dp_second));  // 选right也同理
    }
    return dp_first[left][right];

}

int second_memory(vector<int> arr, int left, int right, vector<vector<int>>& dp_first, vector<vector<int>>& dp_second) {
    if (dp_second[left][right] != -1) {
        return dp_second[left][right];
    }

    if (left == right) {  // 如果此时范围中只有一个物品，作为后手，当然不能拿
        dp_second[left][right] = 0;
    } else {
        // 为什么是min，因为你此时是后手，在这个区间上，对手是先手，对手肯定会使得你在下一个区间上先手所能获得的价值最小
        dp_second[left][right] = min(first_memory(arr, left + 1, right, dp_first, dp_second),
                                     first_memory(arr, left, right - 1, dp_first, dp_second));
    }
    return dp_second[left][right];

}

int process3_memory(vector<int> arr) {  // 返回你在整个区间上先手和后手所能获得的价值的最大值
    vector<vector<int>> dp_first(arr.size(), vector<int>(arr.size(), -1));
    vector<vector<int>> dp_second(arr.size(), vector<int>(arr.size(), -1));
    return max(first_memory(arr, 0, arr.size() - 1, dp_first, dp_second),
               second_memory(arr, 0, arr.size() - 1, dp_first, dp_second));
}

/**
 * 动态规划版本
 * 两个dp表依次更新
 * 一个元素不依赖于当前表中的任何元素，而是依赖于另一张表
 */
int process3_dp(vector<int> arr) {  // 返回你在整个区间上先手和后手所能获得的价值的最大值
    vector<vector<int>> dp_first(arr.size(), vector<int>(arr.size(), -1));
    vector<vector<int>> dp_second(arr.size(), vector<int>(arr.size(), -1));

    for (int i = 0; i < arr.size(); i++) {  // 根据basecase初始化dp表
        dp_first[i][i] = arr[i];
        dp_second[i][i] = 0;
    }

    int index = 1;
    while (index < arr.size()) {  // 分析依赖，按对角线更新
        for (int i = 0; i < arr.size(); i++) {
            int j = i + index;
            if (j > arr.size() - 1) {
                break;
            }
            dp_first[i][j] = max(arr[i] + dp_second[i + 1][j],
                                 arr[j] + dp_second[i][j - 1]);
            dp_second[i][j] = min(dp_first[i + 1][j], dp_first[i][j - 1]);
        }
        index++;
    }

    return max(dp_first[0][arr.size() - 1],
               dp_second[0][arr.size() - 1]);
}


//////////////////////////////////////////三维表////////////////////////////////////////

/**
 * 棋子“马”步数问题
 * 象棋棋盘规格是横9纵10，用一个shape为(9,10)的二维数组代替
 * 假设棋子马一开始在(0,0)位置
 * 棋子马走一步的规则按照象棋的规则来
 * 目的地是(x, y)，总步数是K
 * 请问一共有多少种方法走到目的地
 */
/**
 * 暴力递归
 * 我们将问题反过来思考，从(0,0)->(x,y)，其实和从(x,y)->(0,0)一样的
 * 这样递归函数的含义就是从(x,y)到(0,0)能花residual步数到达的方法数
 */
int process4(int x, int y, int residual) {
    if (residual == 0) {
        return (x == 0) && (y == 0) ? 1 : 0;
    }
    // 越界
    if (x < 0 || y < 0 || x > 8 || y > 9) {
        return 0;
    }

    // 返回八个方向上的方法之和
    return (process4(x - 1, y + 2, residual - 1) +
            process4(x - 2, y + 1, residual - 1) +
            process4(x - 2, y - 1, residual - 1) +
            process4(x - 1, y - 2, residual - 1) +
            process4(x + 1, y - 2, residual - 1) +
            process4(x + 2, y - 1, residual - 1) +
            process4(x + 2, y + 1, residual - 1) +
            process4(x + 1, y + 2, residual - 1)
            );
}

/**
 * 记忆化搜索
 * 三个可变参数，所以建立的缓存表需要有三个维度
 */
int process4_memory(int x, int y, int residual, vector<vector<vector<int>>>& dp) {
    // 越界
    if (x < 0 || y < 0 || x > 8 || y > 9) {
        return 0;
    }

    if(dp[residual][x][y] != -1) {
        return dp[residual][x][y];
    }
    if (residual == 0) {
        dp[residual][x][y] = (x == 0) && (y == 0) ? 1 : 0;
    } else {
        // 返回八个方向上的方法之和
        dp[residual][x][y] = process4(x - 1, y + 2, residual - 1) +
                             process4(x - 2, y + 1, residual - 1) +
                             process4(x - 2, y - 1, residual - 1) +
                             process4(x - 1, y - 2, residual - 1) +
                             process4(x + 1, y - 2, residual - 1) +
                             process4(x + 2, y - 1, residual - 1) +
                             process4(x + 2, y + 1, residual - 1) +
                             process4(x + 1, y + 2, residual - 1);
    }
    return dp[residual][x][y];
}

/**
 * 动态规划
 * 会发现本层的值只依赖于下一层的值
 */
int process4_dp(int x, int y, int K) {
    vector<vector<vector<int>>> dp(K + 1, vector<vector<int>>(9, vector<int>(10, 0)));
    dp[0][0][0] = 1;

    for (int residual = 1; residual <= K; residual++) {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 10; j++) {
                // 返回八个方向上的方法之和
                // 这里判断越界并取值可以另外写一个函数，就不用自己在这重复写了
                int p1 = i - 1 < 0 || j + 2 > 9 ? 0 : dp[residual - 1][i - 1][j + 2];
                int p2 = i - 2 < 0 || j + 1 > 9 ? 0 : dp[residual - 1][i - 2][j + 1];
                int p3 = i - 2 < 0 || j - 1 < 0 ? 0 : dp[residual - 1][i - 2][j - 1];
                int p4 = i - 1 < 0 || j - 2 < 0 ? 0 : dp[residual - 1][i - 1][j - 2];
                int p5 = i + 1 > 8 || j - 2 < 0 ? 0 : dp[residual - 1][i + 1][j - 2];
                int p6 = i + 2 > 8 || j - 1 < 0 ? 0 : dp[residual - 1][i + 2][j - 1];
                int p7 = i + 2 > 8 || j + 1 > 9 ? 0 : dp[residual - 1][i + 2][j + 1];
                int p8 = i + 1 > 8 || j + 2 > 9 ? 0 : dp[residual - 1][i + 1][j + 2];
                dp[residual][i][j] = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
            }
        }
    }
    return dp[K][x][y];
}


/**
 * 生存几率问题
 * 给定一个安全区域，大小为(x,y)
 * 一个人初始位置在(a, b)
 * 这个人上下左右随便走，一旦走出安全区域就会死亡
 * 一共可以走K步
 * 请问该人的生存几率有多大
 */
/**
 * 暴力递归版本
 * 求出生存的行走方法数
 * 然后在将能成存的行走方法数除以总的方法数，只在求方法数上进行递归求解
 */
int process5(int x, int y, int a, int b, int residual) {
    if (a < 0 || b < 0 || a >= x || b >= y) {
        return 0;
    }
    if (residual == 0) {
        return 1;
    }

    return process5(x, y, a - 1, b, residual - 1) +
           process5(x, y, a + 1, b, residual - 1) +
           process5(x, y, a, b - 1, residual - 1) +
           process5(x, y, a, b + 1, residual - 1);
}

// 求最大公约数（用的是辗转相除法）
int gcd(int a, int b) {  // 注意a>b
    return b == 0 ? a : gcd(b, a % b);
}

/**
 * 记忆化搜索版本
 */
int process5_memory(int x, int y, int a, int b, int residual, vector<vector<vector<int>>>& dp) {
    if (a < 0 || b < 0 || a >= x || b >= y) {
        return 0;
    }

    if (dp[residual][a][b] != -1) {
        return dp[residual][a][b];
    }

    if (residual == 0) {
        dp[residual][a][b] = 1;
    } else {
        dp[residual][a][b] = process5_memory(x, y, a - 1, b, residual - 1, dp) +
                             process5_memory(x, y, a + 1, b, residual - 1, dp) +
                             process5_memory(x, y, a, b - 1, residual - 1, dp) +
                             process5_memory(x, y, a, b + 1, residual - 1, dp);
    }

    return dp[residual][a][b];
}

/**
 * 动态规划版本
 */
int process5_dp(int x, int y, int a, int b, int K) {
    vector<vector<vector<int>>> dp(K + 1, vector<vector<int>>(x, vector<int>(y, 0)));

    for (int residual = 0; residual <= K; residual++) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {
                if (residual == 0) {
                    dp[residual][i][j] = 1;
                } else {
                    int p1 = i - 1 < 0 ? 0 : dp[residual - 1][i - 1][j];
                    int p2 = i + 1 >= x ? 0 : dp[residual - 1][i + 1][j];
                    int p3 = j - 1 < 0 ? 0 : dp[residual - 1][i][j - 1];
                    int p4 = j + 1 >= y ? 0 : dp[residual - 1][i][j + 1];
                    dp[residual][i][j] = p1 + p2 + p3 + p4;
                }
            }
        }
    }

    return dp[K][a][b];
}

int solution(int x, int y, int a, int b, int K) {
    // 实际上survive和total应该会很大，所以这里最好使用long 或者long long代替int
    int survive = process5_dp(x, y, a, b, K);
    int total = pow(4, K);
    int c = gcd(total, survive);  // 求最大公约数
    int prob = survive / total;
    cout << survive / c <<  '/' << total / c << endl;
    return prob;
}


///////////////////////////////////优化动态规划///////////////////////////////////////

/**
 * 组出某个钱数的方法数（背包问题）
 * 给定一个数组（无重复值），代表各种面值的货币
 * 再给定一个总面额
 * 要求使用数组中的货币组合出总面额K
 * 数组中的货币可以重复使用多次
 * 问一共有多少种组合
 */
/**
 * 暴力递归版本
 * 经典的从左往右尝试
 */
int process6(vector<int>& arr, int index, int residual) {
    if (index == arr.size()) {  // 如果数组中的货币都选过了
        return residual == 0 ? 1 : 0;
    }

    int res = 0;
    for (int i = 0; i * arr[index] <= residual; i++) {  // 这里限制了i*arr[index]不会超过residual，所以不需要判断residual小于0
        res += process6(arr, index + 1, residual - i * arr[index]);
    }
    return res;
}

/**
 * 记忆化搜索版本
 */
int process6_memory(vector<int>& arr, int index, int residual, vector<vector<int>>& dp) {
    if (dp[index][residual] != -1) {
        return dp[index][residual];
    }
    if (index == arr.size()) {  // 如果数组中的货币都选过了
        dp[index][residual] = residual == 0 ? 1 : 0;
    } else {
        dp[index][residual] = 0;
        for (int i = 0; i * arr[index] <= residual; i++) {  // 这里限制了i*arr[index]不会超过residual，所以不需要判断residual小于0
            dp[index][residual] += process6_memory(arr, index + 1, residual - i * arr[index], dp);
        }
    }
    return dp[index][residual];
}

/**
 * 动态规划版本
 */
int process6_dp(vector<int>& arr, int K) {
    vector<vector<int>> dp(arr.size() + 1, vector<int>(K + 1, 0));
    dp[arr.size()][0] = 1;
    for (int index = arr.size(); index >= 0; index--) {
        for (int residual = 0; residual <= K; residual++) {
            for (int i = 0; i * arr[index] <= residual; i++) {  // 这里限制了i*arr[index]不会超过residual，所以不需要判断residual小于
                dp[index][residual] += dp[index + 1][residual - i * arr[index]];
            }
        }
    }
    return dp[0][K];
}

/**
 * 优化版本的动态规划
 * 由于出现了枚举行为，使用斜率优化，看看邻近的格子是否可以代替枚举行为
 * 由分析上述的依赖可知（这里的分析需要画图去找）
 * 每一个格子的值都依赖于下一行中，小于当前格子列数，并且与当前格子相差arr[index]的倍数距离的格子
 * 也就是普通动态规划中，第三个for循环中的枚举行为
 * 但是简单分析一下，我们在枚举求和时，除了当前格子正下方的格子，之前格子的求和行为已经在上一次求解过了
 * 这个上一次指的是，与当前格子同一行，小于当前的格子列数且相差一个arr[index]的格子
 * 所以，我们只需要将当前格子正下方的数据加上之前已经求和过的结果，就节省了一大部分枚举的时间开销
 */
int process6_optimize_dp(vector<int>& arr, int K) {
    vector<vector<int>> dp(arr.size() + 1, vector<int>(K + 1, 0));
    dp[arr.size()][0] = 1;
    for (int index = arr.size(); index >= 0; index--) {
        for (int residual = 0; residual <= K; residual++) {
            if (residual - arr[index] >= 0) {
                dp[index][residual] = dp[index + 1][residual]  // 正下方的格子
                                      + dp[index][residual - arr[index]];  // 与当前格子同一行且列上差一个arr[index]的格子
            } else {
                dp[index][residual] = dp[index + 1][residual];
            }
        }
    }
    return dp[0][K];
}


int main() {
    vector<int> coins = {1, 2, 3, 5, 7, 9};
//    int target = 13;
//    vector<vector<int>> dp;
//    for (int i = 0; i < coins.size() + 1; i++) {
//        vector<int> row(target + 1, -2);
//        dp.emplace_back(row);
//    }
    vector<vector<vector<int>>> dp(6, vector<vector<int>>(9, vector<int>(10, -1)));
    cout << process4_dp(3, 4, 5);

    return 0;
}