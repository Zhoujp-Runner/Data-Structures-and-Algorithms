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
 * 4.前缀、后缀数组问题（两道题，包括优化空间的版本）
 * 5.判断两个字符串是否互为旋转词（我这里用滚动哈希进行字符串匹配）
 * 6.咖啡杯问题
 * 最近修改日期：2023-11-18
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
#include <cmath>
#include <functional>

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


/**
 * 前缀数组、后缀数组的问题
 * 且根据题目的要求，将空间复杂度优化到O(1)
 * 这个优化不是所有前后缀数组问题都可以使用的，得看具体的题目
 */
/**
 * 接雨水问题
 * 给定一个非负整数数组，代表一个容器，数组中每个位置的数字代表该容器在该位置的高度
 * 请问，用该容器最多能装多少水
 * 形象地，将数组画成直方图，每个元素对应直方图中的一个长方形，元素的大小为该长方形的高度
 * 用该直方图能装下多少面积的水
 */
// 借用辅助数组的方法（前缀和后缀）
int maxRain(vector<int>& container) {
    int n = container.size();
    vector<int> left(n, -1);
    vector<int> right(n, -1);
    int left_max = container[0], right_max = container[n - 1];
    for (int i = 1; i < n; i++) {
        left[i] = left_max;
        left_max = max(left_max, container[i]);
    }

    for (int i = n - 2; i >= 0; i--) {
        right[i] = right_max;
        right_max = max(right_max, container[i]);
    }

    int res = 0;
    for (int i = 1; i < n - 1; i++) {
        res += container[i] < min(left[i], right[i]) ? min(left[i], right[i]) - container[i] : 0;
    }

    return res;
}

// 额外空间复杂度为O(1)的方法（双指针）
int maxRain2(vector<int>& container) {
    int n = container.size(), left_max = container[0], right_max = container[n - 1];
    int left = 1, right = n - 2, res = 0;

    while(left <= right) {
        // 如果左指针左边的最大值大于右指针右边的最大值，那么结算右指针所对应区域所能接的雨水
        // 因为，此时，限制该位置所能接的雨水的数量的瓶颈为右边最大值，此时，无论左边最大值在遍历过程中是否会发生变化，都不会影响该位置的接水量
        // 此时右指针右边遍历区域的最大值已经确定了，所以该点的右边的最值不可能发生变化，且满足if条件的情况下可以直接结算
        // 试想，如果左指针的左边的最大值在后续过程中变大，但是仍然满足left_max > right_max，右边仍然是瓶颈
        // 如果左指针左边的最大值在后续过程中变小，这种情况是不可能出现的，因为最大值只可能变大，不可能变小
        if (left_max > right_max) {
            res += container[right] < right_max ? right_max - container[right] : 0;
            right_max = max(right_max, container[right]);
            right--;
        } else {
            res += container[left] < left_max ? left_max - container[left] : 0;
            left_max = max(left_max, container[left]);
            left++;
        }
    }
    return res;
}

/**
 * 给定一个数组，该数组可以分为左半部分和右半部分
 * 请返回左半部分最大值和右半部分最大值相减的 最大值
 */
// 前缀后缀数组版本
int maxLeftPartMinusRightPart(vector<int>& nums) {
    int n = nums.size();
    vector<int> left(n, nums[0]);
    vector<int> right(n, nums[n - 1]);
    for (int i = 1; i < n; i++) {
        left[i] = max(nums[i], left[i - 1]);
    }

    for (int i = n - 2; i >= 0; i--) {
        right[i] = max(nums[i], right[i + 1]);
    }

    int res = 0;
    for (int i = 0; i < n - 1; i++) {
        res = max(res, abs(left[i] - right[i + 1]));
    }

    return res;
}

// 空间复杂度为O(1)版本
int maxLeftPartMinusRightPart2(vector<int>& nums) {
    // 最大值一定会出现在左边部分或者右边部分，所以，最终的答案一定会是最大值减去某个值
    int max_num = *max_element(nums.begin(), nums.end());
    // 由于某一部分的最大值都是递增的，所以某一部分的最大值中的最小值只能是其初始值，对于左边来说是第一个元素，右边来说是最后一个元素
    // 最终结果是返回左边减右边的最大值，所以可以转换成最大值减去左右两边最大值中的最小值，所以就是取min(第一个元素, 最后一个元素)
    return max_num - min(nums[0], nums[nums.size() - 1]);
}


/**
 * 判断两个字符串是否互为旋转词
 * 对于一个字符串，将该字符串的前面任意部分挪到后面，所形成的字符串叫原字符串的旋转词
 * 现在给定两个字符串str1和str2，请判断两个字符串是否互为旋转词
 * 假设给定的字符串只包含26个英文小写字母
 * 注：有些字符串问题都涉及到将字符串拼接再求解，这道题也是这样的方法
 */
// 左程云在这里用的是KMP算法
// 我觉得KMP还是有点繁琐，用滚动哈希来实现
bool isRotationString(string& str1, string& str2) {
    if (str1.size() != str2.size()) return false;
    // 解题的关键在于将其中一个字符串拼接上自己，这样任何的旋转词都会在拼接后的字符串上出现
    str1 = str1 + str1;
    // 记录字符串各个前缀子串的哈希值
    vector<int> hash(str1.size() + 1, 0);
    int target_hash = str2[0] - 'a', n = str2.size();
    // d为进制数
    int d = 131, mod = 1073741824;
    // 计算被匹配的字符串前缀子串的哈希值
    for (int i = 0; i < str1.size(); i++) {
        hash[i + 1] = (hash[i] * d + str1[i] - 'a') % mod;
    }
    // 计算目标字符串哈希值
    for (int i = 1; i < n; i++) {
        target_hash = (target_hash * d + str2[i] - 'a') % mod;
    }
    // 进行字符串匹配
    for (int i = 0; i < str1.size() - n; i++) {
        int sub_hash = ((hash[i + n] - hash[i] * int(pow(d, n))) % mod + mod) % mod;  // 被匹配字符串中某一段长度为n的子串哈希值
        if (sub_hash == target_hash) return true;
    }

    return false;
}


/**
 * 咖啡杯问题
 * 给定一个数组，表示一组咖啡机，数组中的元素代表当前标号的咖啡机制作一杯咖啡的时间
 * 每一台咖啡机一次只能制作一杯咖啡
 * 给定一个数N，代表需要喝咖啡的人的数量，一个人只喝一杯咖啡
 * 一个人喝咖啡的时间为0
 * 给定一个数a，表示洗咖啡杯的机器，一杯一杯洗，只有当上一杯洗完才能洗下一杯
 * 给定一个数b，表示咖啡杯不洗也能自动挥发变干净的时间
 * 问，从一个人开始制作咖啡开始，到最后所有人的咖啡杯都干净所花费的最短时间
 */
struct cmp {
    bool operator() (pair<int, int>& a, pair<int, int>& b) {
        return a.first + a.second > b.first + b.second;
    }
};
int CoffeeCupProblem(vector<int>& nums, int N, int a, int b) {
    // 首先，先不关心如何洗咖啡杯，先关心如何使用咖啡机能够在最短的时间内让所有人都喝到咖啡
    // 显然，这是一个贪心问题，即每次贪结束时间更早的咖啡机，可以建立一个小根堆来选择咖啡机
    // 小根堆中的元素用pair表示，pair中第一个数表示当前咖啡机在何时能被使用，第二个数表示当前咖啡机制作完一杯咖啡的时间，则某一台咖啡机的结束时间为两个数之和
    priority_queue<pair<int, int>, vector<pair<int, int>>, cmp> heap;
    for (int & num : nums) {
        heap.emplace(0, num);
    }
    vector<int> person(N);
    for (int i = 0; i < N; i++) {
        auto [start, spend] = heap.top();  // 拿到当前结束时间最早的咖啡机
        heap.pop();
        person[i] = start + spend;  // 第i个人何时能获得咖啡，由于喝咖啡时间为0，也表示何时喝完咖啡
        heap.emplace(start + spend, spend);
    }

    // 接下来关心何时咖啡杯能够完全干净
    // 每个人只有两个选择，即用机器洗和自然挥发
    // 这可以使用从左往右的模型，依次选择并计算结束时间，最终选择结束时间更快的（为了方便理解，这里用缓存写，不用dp写）
    int max_WashStart = person[0] + a;  // 用于计算最大可能的WashStart
    for (int i = 1; i < N; i++) {
        max_WashStart = max(max_WashStart, person[i]) + a;
    }
    vector<vector<int>> dp(N, vector<int>(max_WashStart, -1));  // 缓存
    // 该函数表示，index及之后的人杯子干净的最少时间
    // index表示人的编号，WashStart表示洗杯子机器最早什么时候可以使用
    function<int(int, int)> process = [&](int index, int WashStart) {
        if (dp[index][WashStart] != -1) return dp[index][WashStart];
        if (index == N - 1) {  // 当来到最后一个人做选择时，选择风干和机器两者中结束时间更小的那一个
            dp[index][WashStart] = min(person[index] + b, WashStart + a);
        } else {
            // 第一种情况，选择使用机器
            int p1 = max(WashStart, person[index]) + a;  // 当前index人洗完杯子的时间，为什么有max，因为虽然WashStart可以使用，但是此时该人可能还没有喝完咖啡
            int after1 = process(index + 1, p1);  // 之后的人杯子干净的最短时间
            int min1 = max(p1, after1);  // 所有杯子干净的时间
            // 第二种情况选择自然挥发
            int p2 = person[index] + b;
            int after2 = process(index + 1, WashStart);
            int min2 = max(p2, after2);
            dp[index][WashStart] = min(min1, min2);
        }

        return dp[index][WashStart];  // 选择花费时间最短的方法
    };

    return process(0, 0);  // 第0个人及之后杯子干净的最少时间，洗杯子机器在0时可用
}


int main() {
//    vector<vector<int>> m = {{1, 2, 3},
//                             {4, 5, 6},
//                             {7, 8, 9}};
//    cout << Optimize_dp(m) <<endl;
//    vector<int> nums = {2,6,7,2,4,7,3};
//    cout << maxLeftPartMinusRightPart(nums) << endl;
//    cout << maxLeftPartMinusRightPart2(nums) << endl;
//    string str1 = "abcd", str2 = "cdcb";
//    cout << isRotationString(str1, str2);
    vector<int> nums = {2, 4, 6, 3, 5};
    cout << CoffeeCupProblem(nums, 10, 3, 5);
    return 0;
}
