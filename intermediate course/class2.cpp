/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class2.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.给定一个数组，求差值为k的去重数字对个数（利用哈希表，将查询的时间复杂度变为0(1)）
 * 2.集合中元素的变动（业务类型的问题）
 * 3.计算合法的括号字符串的深度
 * 4.最长的有效括号子串（解决子串问题的模型，计算每一个元素作为结尾的子串的答案）
 * 5.栈中元素升序排序
 * 6.数字转换不同字母字符串的个数（从左往右尝试的模型）
 * 7.二叉树的最大路径（二叉树的递归套路）
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
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
#include <stack>

using namespace std;


/**
 * 给定一个数组，求差值为k的去重数字对个数
 * 通过哈希表索引特定的值
 */
int process1(vector<int>& arr) {
    unordered_map<int, int> search;
    for (const auto& num : arr) {
        search[num] = 1;
    }
    int res = 0;
    for (auto& k_v : search) {
        int num = k_v.first;
        if (search.find(num + 2) != search.end()) {
            res++;
        }
    }
    return res;
}


/**
 * 集合中元素的变动
 * 给定两个集合，含有若干个元素
 * 定义一个操作magic为，将一个元素从一个集合中取出放入另一个集合中，操作过后，每个集合的平均值都大于操作前
 * 注意：不可以把集合元素取空，且新放入集合中的数不能是集合中原先有的数
 * 问，最多可以进行多少次magic操作
 */
int process2(vector<int>& set1, vector<int>& set2) {
    // 计算两个集合的平均值
    int sum1 = 0, sum2 = 0;
    for (const auto& num : set1) {
        sum1 += num;
    }
    for (const auto& num : set2) {
        sum2 += num;
    }
    double mean1 = double(sum1) / double(set1.size());
    double mean2 = double(sum2) / double(set2.size());
    double epi = 0.000001;
    if (mean1 - mean2 < epi) return 0;  // 浮点类型不能直接比较是否相等，这里用两者之差是否小于一个极小数来判断是否相等
                                        // 这里极小数取的不一定合理，只是为了用来做一个演示
    // 确定哪个集合平均值大，哪个集合平均值小
    double larger_mean, smaller_mean;
    int larger_sum, smaller_sum, larger_size, smaller_size;
    vector<int> larger_set, smaller_set;
    if (mean1 > mean2) {
        larger_mean = mean1;
        larger_sum = sum1;
        larger_size = set1.size();
        larger_set = set1;
        smaller_mean = mean2;
        smaller_sum = sum2;
        smaller_size = set2.size();
        smaller_set = set2;
    } else {
        larger_mean = mean2;
        larger_sum = sum2;
        larger_size = set2.size();
        larger_set = set2;
        smaller_mean = mean1;
        smaller_sum = sum1;
        smaller_size = set1.size();
        smaller_set = set1;
    }
    // 将较小集合存入哈希表，用于后续查询新插入的数是否在集合中
    unordered_set<int> smaller;
    for (const auto& num : smaller_set) {
        smaller.emplace(num);
    }

    // 将大集合排序，用于选择所有符合的数中最小的那一个
    // 因为这样操作能让大集合平均值尽可能更大，小集合平均值提升幅度尽可能小
    sort(larger_set.begin(), larger_set.end());
    // 二分查找大集合中第一个大于小集合均值的数
    // 这里也可以不用二分查找，直接在最后的循环中，i从0开始也可以
    int left = 0;
    int right = larger_size - 1;
    while(left < right) {
        int mid = left + (right - left) / 2;
        if (larger_set[mid] < smaller_mean) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    // 开始操作 统计操作数
    int res = 0;
    for (int i = left; i < larger_size; i++) {
        // 三个条件都需要满足
        // 比小集合均值大 比大集合均值小 且该数不在小集合中
        if (larger_set[i] > smaller_mean && larger_set[i] < larger_mean && smaller.count(larger_set[i]) == 0) {
            // 更新两个集合的表示参数
            larger_sum -= larger_set[i];
            larger_size--;
            larger_mean = double(larger_sum) / double(larger_size);
            smaller_sum += larger_set[i];
            smaller.emplace(larger_set[i]);
            smaller_size++;
            smaller_mean = double(smaller_sum) / double(smaller_size);
            // 操作数+1
            res++;
        }
    }
    return res;
}


/**
 * 计算合法的括号字符串的深度
 * 合法的括号匹配的字符串定义如下：
 * 1.空串是合法的
 * 2.字符串不为空时，如果X和Y是合法的，那么XY也是合法的
 * 3.字符串不为空时，如果X是合法的，那么(X)是合法的
 * 4.每个合法的括号序列都可以由以上规则生成
 * 对于一个合法的字符串，定义其深度：
 * 1.空串的深度为0
 * 2.如果X的深度为x，Y的深度为y，XY的深度为max(x,y)
 * 3.X的深度为x，(X)的深度为x+1
 * 例如，"()()()"深度为1，"((()))"深度为3
 */
int process3(string s) {
    // 其实这道题统计的就是左括号最多能够连续出现几次
    // 换而言之，就是class1中，那道括号题里面，count在遍历过程中的最大值
    int count = 0;
    int res = 0;
    for (const auto& c : s) {
        if (c == '(') {
            count++;
            res = max(res, count);
        } else {
            count--;
        }
    }
    return res;
}


/**
 * 最长的有效括号子串
 * 给定一个字符串，括号子串是否有效的判断和之前一样
 * 问，最长的有效子串长度
 *
 * 注意子串和子数组需要保证连续，子序列可以不连续
 * 面对子串这类问题（即连续的问题），都可以有一个试法
 * 即，求以每一个元素为结尾的子串的情况下，答案是多少
 */
int process4(string s) {
    // 记录每一个位置的答案，类似dp
    // 记录的是以每个元素为结尾的最大合法子串长度
    vector<int> depth(s.length(), 0);
    int res = 0;
    for (int i = 1; i < s.length(); i++) {
        // 合法字符串不可能以'('结尾，所以碰到左括号直接跳过，因为数组初始化为0
        if (s[i] == '(') continue;
        // 如果当前是右括号，那我需要结合上以前一个字符结尾的最长子串，看看能否再延伸一个长度，例子如下
        //  ( ) ( ( ) )
        //  0 2 0 0 2 ?
        // 要求以？位置结尾的最长子串，？以前的位置都已经求完了
        // 此时，我首先需要结合上？前一个元素的最长子串，发现是2，那我能否组合成新的子串呢？
        // 关键在于？往前2+1个位置上是否是左括号，如果是，那么此时就可以组合成新的子串，长度至少为2+2
        // 为何说至少？因为两个合法子串拼接后仍然是合法的，所以我们还要检查拼接的情况
        // 是否能拼接取决于？往前2+1+1位置上是否为0，为0说明不能拼接，反之是合法子串能拼接，所以直接加上？往前2+1+1位置上的结果就行
        // 其实上述的关系完全取决于题目中合法子串的定义，做题时需要分析清楚题意
        int pre = i - depth[i - 1] - 1;
        // 检查往前推的位置是否越界
        if (pre >= 0 && s[pre] == '(') {
            depth[i] = depth[i - 1] + 2;
        }
        // 检查往前推的位置是否越界
        if (pre - 1 >= 0) {
            depth[i] = depth[pre - 1];
        }
        res = max(depth[i], res);
    }
    return res;
}


/**
 * 栈中元素升序排序
 * 给定一个含有元素的栈，请编写一个程序，使得栈中的元素从栈底到栈顶升序排序
 * 最多额外使用一个栈存放临时数据，不能将元素复制到除了栈的别的数据结构中华
 */
void process5(stack<int> s) {
    // 额外准备一个辅助栈，该栈要保持从底往上从大到小的排列
    stack<int> s2;
    while (!s.empty()) {
        // 从原始栈中弹出一个元素
        int cur = s.top();
        s.pop();
        if (s2.empty()) {  // 如果此时辅助栈为空，则直接让cur入辅助栈
            s2.emplace(cur);
        } else {
            while (!s2.empty() && cur > s2.top()) {  // 如果cur大于辅助栈的栈顶元素，此时不满足辅助栈的要求
                // 需要不断从辅助栈中弹出元素，直到满足为止
                int temp = s2.top();
                s2.pop();
                s.emplace(temp);
            }
            s2.emplace(cur);
        }
    }

    // 当执行完上面的循环后，辅助栈中已经严格保证栈底到栈顶从大到小的顺序了
    // 此时只需要将所有元素依次重新压回原始栈
    while (!s2.empty()) {
        s.emplace(s2.top());
        s2.pop();
    }
}


/**
 * 数字转换不同字母字符串的个数
 * 规定，26个字母对应1~26个数字
 * 给定一串数字字符串，计算出该串数字能够转换的不同字母字符串的个数
 */
int process6(string nums, int index) {  // 从左往右尝试模型，只关心当前点以及往后能够有几种可能
    if (index == nums.length()) {
        return 1;
    }
    if (nums[index] == '0') { // 0没有对应的字母，如果是0直接返回0
        return 0;
    }
    int res = process6(nums, index + 1); // 只把自己单独转换为字母的情况
    if (index == nums.length() - 1) {  // 如果当前已经来到了最后一个字符
        return res;  // 直接返回只把自己单独作为字母的情况，因为没有选择两个数的情况
    }
    if ((nums[index] - '0') * 10 + (nums[index + 1] - '0') <= 26) { //如果当前字符与下一个字符组成的数小于等于26，则也是一种情况
        res += process6(nums, index + 2);
    }
    return res;
}
// dp版本
int process6_dp(string nums) {
    int n = nums.length();
    vector<int> dp(n + 1, 0);
    dp[n] = 1;
    dp[n - 1] = nums[n - 1] == '0' ? 0 : 1;
    for (int i = n - 2; i >= 0; i++) {
        if (nums[i] == '0') {
            dp[i] = 0;
            continue;
        }
        dp[i] = dp[i + 1];
        if ((nums[i] - '0') * 10 + (nums[i + 1] - '0') <= 26) {
            dp[i] += dp[i + 2];
        }
    }
    return dp[0];
}


/**
 * 二叉树的最大路径
 * 定义二叉树的路径为根节点到叶节点的路径，定义路径的值为沿途所有节点的值之和
 * 求出其中的最大路径
 *
 * 二叉树路径有很多，做题时需要仔细分析路径的定义
 * 有关路径的问题，用二叉树的递归套路基本能够解决问题
 */
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

int process7(TreeNode* cur) {
    if (cur->left == nullptr && cur->right == nullptr) {
        return cur->val;
    }
    int max_path = INT_MIN;
    if (cur->left != nullptr) {
        max_path = process7(cur->left);
    }
    if (cur->right != nullptr) {
        max_path = max(max_path, process7(cur->right));
    }
    return max_path + cur->val;
}





int main() {
    return 0;
}