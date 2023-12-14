/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class7.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.求一颗完全二叉树的节点数（时间复杂度低于O(N)）
 * 2.最长递增子序列问题（时间复杂度低于O(N^2)）
 * 最近修改日期：2023-12-14
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-24
 *
 */

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>

using namespace std;

/**
 * 求一颗完全二叉树的节点数（时间复杂度低于O(N)）
 */
/**
 * 分析：
 * 利用完全二叉树的性质
 * 先求出整棵树的深度
 * 通过判断当前节点右子树的最左节点是否到了最深一层
 * 如果到了最深一层，那么可知左子树是满二叉树，直接求得节点数，然后递归右子树
 * 如果没到最深一层，那么可知右子树是满二叉树，直接求得节点数，然后递归左子树
 * 这样就能计算得到所有的节点数
 * 算法的时间复杂度为O(h^2)，其中h为树的高度
 * 分析可见视频：https://www.bilibili.com/video/BV13g41157hK?p=27&vd_source=e3780c93bbfab1295672c1a3f1be54d5
 */
// 完全二叉树的节点
struct CBTNode {
    int val;
    CBTNode *left, *right;
};
int NodeNumOfCBT(CBTNode* root) {
    // 计算整棵树的最大深度
    int max_depth = 0;
    CBTNode* node = root;
    while (node != nullptr) {
        max_depth++;
        node = node->left;
    }
    // 递归函数
    function<int(CBTNode*, int)> process = [&](CBTNode* cur, int depth) {
        // 如果当前来到了最后一层，那必定是叶子节点（这里也需要注意一下，如果用指针来判断叶子节点的话没有这样写清晰）
        if (depth == max_depth) {
            return 1;
        }
        // 计算当前树的右子树的最左节点的深度
        int cur_depth = depth;
        CBTNode* node = cur->right;
        while (node != nullptr) {
            cur_depth++;
            node = node->left;
        }
        // 先将节点数初始化为1，就是算上根节点
        int node_num = 1;
        // 如果到了最深一层，那么可知左子树是满二叉树，直接求得节点数，然后递归右子树
        if (cur_depth == max_depth) {
            // 这里2的几次方也可以用位运算左移实现，将1左移几位就是2的几次方
            node_num += int(pow(2, max_depth - depth)) - 1 + process(cur->right, depth + 1);
        } else {
            // 如果没到最深一层，那么可知右子树是满二叉树，直接求得节点数，然后递归左子树
            node_num += int(pow(2, max_depth - depth - 1)) - 1 + process(cur->left, depth + 1);
        }
        return node_num;
    };

    return process(root, 1);
}


/**
 * 最长递增子序列问题（子序列：可以不连续）
 * 给定一个数组，求其最长的递增子序列长度（通过删除任意元素得到子序列，子序列的值在原数组中可以不连续）
 */
/**
 * 常规思路：动态规划，dp[i]的含义可以是以当前元素结尾的子数组中的最长递增子序列，也可以是以当前元素开头
 * 常规思路的时间复杂度是O(N^2)
 *
 * 另外的思路：使用一个数组end，end数组第i个元素表示，原数组中，所有长度为i+1的子序列的最小结尾
 * 然后从左往右遍历，每当遇到一个元素，在end中二分查找小于该元素的最大的数x，以该元素结尾的递增子序列的长度即为x的下标加1
 * 然后更新x右边的数为当前的元素，那么由于二分查找，这道题的复杂度优化为O(NlogN)
 *
 * 第二个思路主要是利用题目中求解的要求，子序列是递增的，那么我每次记录某个长度为L的子序列的最小结尾e，
 * 如果新的元素比e大，那么该元素至少可以组成L+1长度的子序列
 * 如果新的元素比e小，那么该元素就小于所有长度为L的子序列的结尾，因为e是最小的结尾
 * end数组正是以这种单调性所构建的
 * 当dp需要枚举前边所有的dp值的时候，可以考虑是否能够进行优化
 * 其实这也就是建立了N个单调栈，每个单调栈栈顶表示某个长度子序列的最小结尾
 */
int LongestSubsequence(vector<int>& arr) {
    int n = arr.size();

    // 思路1，动态规划，dp[i]更新为arr[i]之前，所有比arr[i]小的数的最大dp值+1
    vector<int> dp(n, 1);
    int max_length = 0;
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                dp[i] = max(dp[i], 1 + dp[j]);
            }
        }
        max_length = max(max_length, dp[i]);
    }

    // 思路2
    vector<int> ends(n, arr[0]);
    int max_right = 1;  // 表示当前最长的子序列长度
    for (int i = 1; i < n; i++) {
        int left = 0, right = max_right;
        while (left < right) {
            int mid = left + (right - left + 1) / 2;
            if (ends[mid] < arr[i]) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }
        if (left == max_right) {
            max_right++;
        }
        ends[left] = arr[i];
    }

    return max_right;
}


int main() {
    vector<int> a = {1, 4, 3, 2, 6, 7, 4, 9, 8, 6, 29, 4, 0, 28, 8, 6};
    cout << LongestSubsequence(a);
    return 0;
}
