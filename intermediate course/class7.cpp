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
 * 3.一个数能否被3整除
 * 4.找出未出现过的数字（额外空间复杂度为O(1)，时间复杂度为O(N)）
 * 5.使人气正好达标的最少花钱数（寻找额外的限制条件）
 * 最近修改日期：2023-12-23
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


/**
 * 一个数能否被3整除
 * 给定一个n，定义一个数为123...n
 * 意思是当n=2时，num=12
 * 当n=3时，num=123
 * 当n=13时，num=12345678910111213
 * 问，给定n的情况下，判断n对应数字是否能被3整除
 */
/**
 * 思路：
 * 第一种思路：直接根据n得到num，然后对3取模
 * 第二种思路：将num的每一位相加，然后对3取模
 * 第三种思路：直接求n的等差数列之和，然后对3取模
 * 很明显，第一种和第二种都不太可取，当n特别大时，第一种无法表示，第二种时间复杂度有点高
 * 第三种是根据第二种进一步得到的，试想10能否被3整除，我们可以直接10对3取模，也可以1+0然后对3取模，这两种是等价的
 * 那么910能否被3整除，我们可以9+1+0模3，也可以9+10模3
 */
bool CanBeDividedByThree(int n) {
    if (n == 2) return false;
    return ((1 + n) * n / 2) % 3 == 0;
}


/**
 * 找出未出现过的数字（额外空间复杂度为O(1)，时间复杂度为O(N)）
 * 给定一个数组，长度为n，数组中的元素都大于等于1，小于等于n，数组中的数允许重复
 * 请找出，1到n之间未在数组中出现的数（要求额外空间复杂度为O(1)，时间复杂度为O(N)）
 */
/**
 * 思路：in-place算法，即原地修改数组，规定下标为i的位置上存储的数为i+1
 * 遍历数组，当发现当前下标存的不是i+1，就将当前下标存的数放到规定的位置，循环此过程，直到回到开始交换的位置，或者需要被放置的位置已经有了规定的值
 * 第二次遍历判断那些位置上没有存规定的数，就缺少哪些数
 */
vector<int> AbsentNums(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        // 如果当前位置上已经是规定的数，那么跳过该位置
        if (arr[i] == i + 1) continue;
        int num = arr[i];  // 记录需要调整位置的数
        // 如果该数的对应位置上没有存放该数，并且该位置不是i
        while (arr[num - 1] != num && num - 1 != i) {
            // 将该数放在对应位置上，并将num更新为位置上原本的数
            int new_num = arr[num - 1];
            arr[num - 1] = num;
            num = new_num;
        }
    }
    vector<int> res;
    // 第二次遍历，找未出现的数
    for (int i = 0; i < n; i++) {
        if (arr[i] != i + 1) res.push_back(i + 1);
    }
    return res;
}


/**
 * 使人气正好达标的最少花钱数
 * 给定初始人气a和目标人气b，两者都是偶数
 * 规定三种操作
 * 操作一：花费x元，使人气+2
 * 操作二：花费y元，使人气*2
 * 操作三：花费z元，使人气-2
 * 问使人气正好等于b需要花费的最少钱数
 */
/**
 * 思路：
 * 乍一看，就是一个简单的动态规划题，可变参数就是当前的花费cur，递归含义是由cur到b花费的最少钱数，base case就是当cur==b时，返回0
 * 递归体内是三种情况，然后返回三种情况的最小花费（下面会给出无法终止的写法）
 * 但是实际写的过程中，如果按照上面去写，递归无法终止，因为缺少终止条件，单单上面的一个base case是不够的，试想，递归可能一直尝试-2，使得cur永远不能等于b
 * 所以需要额外的限制条件，本题的限制条件可以如下所示:
 * 首先假设我只进行操作1，需要花费多少钱，当cur超过这个钱数的时候，就不必继续递归了，这个钱数可以作为limit1
 * 其次，无论我怎么进行操作，人气都不可能大于2*b，因为如果我想大于2*b的话，那么肯定需要先到达b，所以这个2*b可以作为limit2
 * 最后，我的人气不可能小于0，作为limit3
 * 有这三个条件，就可以使递归正常终止
 *
 * 所以这道题有两种方法找限制
 * 第一种是找一个平凡解，即limit1
 * 第二种是从题目中所给信息，算出某些变量的变化范围，即limit2和limit3
 * 这两种找限制的方法需要灵活运用
 * （leetcode中有一题和这个类似，就是需要人为寻找限制条件，明天有时间找到复习一下，并把那道题贴到这里）
 */
// 无法终止的版本
int MinConsumptionEndless(int a, int b, int x, int y, int z) {
    if (a == b) return 0;
    int p1 = MinConsumptionEndless(a + 2, b, x, y, z) + x;
    int p2 = MinConsumptionEndless(a * 2, b, x, y, z) + y;
    int p3 = MinConsumptionEndless(a - 2, b, x, y, z) + z;
    return min(p1, min(p2, p3));
}

// 可以正常终止的版本
int MinConsumption(int a, int b, int x, int y, int z) {
    int limit1 = ((b - a) / 2) * x;
    // 注意，这里的递归含义有所不同，这里的递归表示尝试的含义，我当前试到人气为cur时，且当前花费为spend，达到b所能花费的最少总钱数
    function<int(int, int)> process = [&](int cur, int spend) {
        if (cur == b) return spend;
        if (spend >= limit1 || cur >= 2 * b || cur < 0) return INT_MAX;
        int p1 = process(cur + 2, spend + x);
        int p2 = process(cur * 2, spend + y);
        int p3 = process(cur - 2, spend + z);
        return min(p1, min(p2, p3));
    };
    return process(a, 0);
}


int main() {
    vector<int> a = {1, 1, 4, 3, 6, 7, 3};
//    AbsentNums(a);
    cout << MinConsumption(6, 10, 40, 50, 10);
    return 0;
}
