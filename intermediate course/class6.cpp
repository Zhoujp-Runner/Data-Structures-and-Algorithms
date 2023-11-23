/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class6.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.打印文件目录
 * 2.树形DP相关题目（两道例题）
 * 3.假设答案法（两道例题）
 * 最近修改日期：2023-11-23
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-23
 *
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

using namespace std;

/**
 * 打印文件目录
 * 给定一个字符串类型的数组arr
 * 例如 {"b\\cst", "d\\", "a\\d\\e", "a\\b\\c"}
 * 数组中每个字符串表明文件的绝对路径
 * 请根据数组中的路径，将文件的目录打印出来
 * 要求，子目录直接在父目录下面，相对父目录缩进两格，且同级目录按照字母顺序排序
 * 例如上述的数组可以打印成
 * a
 *   b
 *     c
 *   d
 *     e
 * b
 *   cst
 * d
 */
// 前缀树的节点
struct PreTreeNode {
    int val;  // 第几级目录，也和打印的时候的空格数有关
    map<string, PreTreeNode*> next;  // 用有序表对string进行排序，达到同级目录按字母顺序排序的目的
    explicit PreTreeNode(int v): val(v) {}
};
// 往前缀树中添加一条路径
void insert (PreTreeNode* node, int index, vector<string>& path) {
    if (index == path.size()) return;
    auto it = node->next.find(path[index]);
    if (it != node->next.end()) {
        insert(node->next[path[index]], index + 1, path);
    } else {
        PreTreeNode* next_node = new PreTreeNode(node->val + 1);
        node->next[path[index]] = next_node;
        insert(next_node, index + 1, path);
    }
}
// 将一个字符串根据\拆分成不同的元素
vector<string> split(string& s) {
    vector<string> path;
    for (int i = 0; i < s.length();) {
        if (s[i] != '\\') {  // 需要转义，所以要两个斜杠
            int j = i;
            while (j < s.length() && s[j] != '\\') {
                j++;
            }
            path.push_back(s.substr(i, j - i));
            i = j + 1;
        } else {
            i++;
        }
    }
    return path;
}
void printDict(vector<string>& paths) {
    // 建立前缀树
    PreTreeNode* root = new PreTreeNode(0);
    for (auto path: paths) {
        vector<string> split_path = split(path);
        insert(root, 0, split_path);
    }
    // 深度优先遍历的打印函数
    function<void(PreTreeNode*)> dfs = [&](PreTreeNode* cur) {
        if (cur->next.empty()) return;
        for (auto [k, v]: cur->next) {
            int space_num = cur->val * 2;
            for (int i = 0; i < space_num; i++) cout << ' ';
            cout << k << endl;
            dfs(v);
        }
    };

    dfs(root);
}


/**
 * 树形DP相关题目
 * 将搜索二叉树转换为一条有序的双向链表
 * 双向链表和搜索二叉树的节点结构是一样的（把left认为是last，right认为是next）
 * 给定一个搜索二叉树的头结点head，请将其转化成一条有序的双向链表，并返回链表的头结点
 */
/**
 * 二叉树的递归套路，树形DP
 */
// 搜索二叉树的节点
struct BSTNode {
    int val;
    BSTNode *left, *right;
};
// 树形DP的返回值
struct Info_BST2DLL {
    BSTNode* head;  // 子树所构成的链表的头结点
    BSTNode* tail;  // 子树所构成的链表的尾节点
    Info_BST2DLL(BSTNode* h, BSTNode* t): head(h), tail(t) {}
};
// 搜索二叉树转双向链表
BSTNode* BST2DLL(BSTNode* root) {
    // 递归函数
    function<Info_BST2DLL(BSTNode*)> dfs = [&](BSTNode* cur) {
        // 如果节点为空，则形成的双向链表的头和尾都是空的
        // 如果这里是判断是否是叶子节点，那么后续就不用判断节点是否为空
        if (cur == nullptr) {
            return Info_BST2DLL(nullptr, nullptr);
        }
        // 分别获得该节点左子树的链表的头和尾，以及右子树的链表的头和尾
        Info_BST2DLL leftDLL = dfs(cur->left);
        Info_BST2DLL rightDLL = dfs(cur->right);
        // 将当前节点和左右子树形成的链表连接在一起
        // 连接的顺序是 左中右
        // 由于是搜索二叉树，所以 左中右 的连接方式默认是已经排好序了
        cur->left = leftDLL.tail;
        cur->right = rightDLL.head;
        if (leftDLL.tail != nullptr) {
            leftDLL.tail->right = cur;
        }
        if (rightDLL.head != nullptr) {
            rightDLL.head->left = cur;
        }
        // 返回以当前节点为根的整颗树所形成的链表的头和尾
        BSTNode* new_head = leftDLL.head == nullptr ? cur : leftDLL.head;
        BSTNode* new_tail = rightDLL.tail == nullptr ? cur : rightDLL.tail;
        return Info_BST2DLL(new_head, new_tail);
    };

    Info_BST2DLL res = dfs(root);
    return res.head;
}


/**
 * 找到一颗二叉树中，最大的搜索二叉子树，返回最大搜索二叉子树的节点个数
 */
/**
 * 方法一：二叉树的递归套路，树形dp
 * 方法二：对整颗树进行中序遍历，找到最大的单调递增子序列，序列长度即为节点个数
 */
/**
 * 稍微复杂一点
 * 找到一颗二叉树中，最大的搜索二叉子树，返回最大搜索二叉子树的根节点（如果有多个答案，返回其中一个即可）
 * 题目改成这样，那么用树形dp就比树的遍历要简单一点
 * 因为中序遍历是无法确定某一段序列的根节点
 * 所以为了确定根节点，我们还需要先序遍历，这样判断根节点的方式稍微麻烦了一点（但其实也不难）
 * 所以对于稍微复杂一点的这个版本，这里采用树形DP来写
 */
struct TreeNode {
    int val;
    TreeNode *left, *right;
};

struct Info_MaxBSCT {
    TreeNode* maxBSCTRoot;  // 最大搜索二叉子树对应的根节点
    bool isBST;  // 当前子树是否是BST
    int max_val, min_val, maxBSCTSize;  // 三个变量分别是子树的最大值，子树的最小值，最大搜索二叉子树的大小
    Info_MaxBSCT(){
        maxBSCTRoot = nullptr;
        isBST = true;
        max_val = 0;
        min_val = 0;
        maxBSCTSize = 0;
    }
    Info_MaxBSCT(TreeNode* n, bool isbst, int max_v, int min_v, int size):
    maxBSCTRoot(n), isBST(isbst), max_val(max_v), min_val(min_v), maxBSCTSize(size) {}
};

TreeNode* MaxBSCT(TreeNode* root) {
    function<Info_MaxBSCT(TreeNode*)> dfs = [&](TreeNode* cur) {
        if (cur == nullptr) {
            return Info_MaxBSCT(cur, true, INT_MIN, INT_MAX, 0);
        }
        Info_MaxBSCT left_info = dfs(cur->left);
        Info_MaxBSCT right_info = dfs(cur->right);
        // 如果左右两边都是BST
        if (left_info.isBST && right_info.isBST) {
            // 判断左右子树和头节点是否能构成BST
            if (cur->val > left_info.max_val && cur->val < right_info.min_val) {
                // 这里是用来判断是否返回值是否是由空节点得到的
                int min_v = left_info.min_val == INT_MIN ? cur->val : left_info.min_val;
                int max_v = right_info.max_val == INT_MAX ? cur->val : right_info.max_val;
                int size = left_info.maxBSCTSize + right_info.maxBSCTSize + 1;
                return Info_MaxBSCT(cur, true, max_v, min_v, size);
            }
        }
        // 如果左右节点都不为空
        if (left_info.maxBSCTRoot != nullptr && right_info.maxBSCTRoot != nullptr) {
            int min_v = min(cur->val, min(left_info.min_val, right_info.min_val));
            int max_v = max(cur->val, max(left_info.max_val, right_info.max_val));
            int size = left_info.maxBSCTSize > right_info.maxBSCTSize ? left_info.maxBSCTSize : right_info.maxBSCTSize;
            TreeNode* node = left_info.maxBSCTSize > right_info.maxBSCTSize ? left_info.maxBSCTRoot : right_info.maxBSCTRoot;
            return Info_MaxBSCT(node, false, max_v, min_v, size);
        }
        // 如果左右节点中有一个为空（注意，左右节点都为空的情况在 base case 下的第一个if中已经处理过了）
        if (left_info.maxBSCTRoot == nullptr) {
            int min_v = min(cur->val, right_info.min_val);
            int max_v = min(cur->val, right_info.min_val);
            int size = right_info.maxBSCTSize;
            return Info_MaxBSCT(right_info.maxBSCTRoot, false, max_v, min_v, size);
        } else {
            int min_v = min(cur->val, left_info.min_val);
            int max_v = min(cur->val, left_info.min_val);
            int size = left_info.maxBSCTSize;
            return Info_MaxBSCT(left_info.maxBSCTRoot, false, max_v, min_v, size);
        }
    };
    return dfs(root).maxBSCTRoot;
}


/**
 * 假设答案法
 * 连续子数组的最大累加和
 * 给定一个数组，请求出其中连续子数组的最大累加和
 * leetcode的题解是使用动态规划或者分治法求解，
 * 建议看一下左程云的解释：https://www.bilibili.com/video/BV13g41157hK/?p=26&spm_id_from=333.1007.top_right_bar_window_history.content.click&vd_source=e3780c93bbfab1295672c1a3f1be54d5
 * 他使用假设答案法
 * 先假设答案是i到j的区间，如果有多个符合条件的区间，该区间是最长的一个区间
 * 根据上述的假设，通过分析可得到两个结论
 * 1) i到n区间（n < j），任意一个区间的累加和都不可能小于等于0，因此如果小于等于0，那么假设不成立，答案就是n+1, j
 * 2) m到i-1区间（0 <= m <= i-1），任意一个区间的累加和都小于0，如果大于等于0，那么答案就不是i到j，而是m到j
 * 基于上述的两个结论可以得到下面的解题流程
 * 其中cur在不断累加，累加和大于0的非负连续区间（如果区间中只有一个数，那么就不论正负），一旦累加和为负，就从下一个元素重新开始累加
 * max记录每一次累加的最大值，即寻找累加和最大的区间
 */
int maxSumSubArr(vector<int>& arr) {
    int cur = 0, sum = INT_MIN;
    for (auto num : arr) {
        cur += num;
        sum = max(cur, sum);
        cur = cur < 0 ? 0 : cur;
    }
    return sum;
}

/**
 * 寻找矩阵中，子矩阵的最大累加和
 * 这道题和上道题是一样的，只不过多了一个维度
 * 那么我们就可以固定死一个维度，然后使用上面的方法来计算
 * 具体而言，
 * 我们固定行，也就是说，我们遍历所有的连续行，
 * 假设有三行，那么我们就需要遍历，只包含0行的最大矩阵，只包含0,1行的最大矩阵，只包含0,1,2行的最大矩阵，只包含1行的最大矩阵，只包含1,2行的最大矩阵，只包含2行的最大矩阵
 * 当我们固定了行这一维之后，就是等于求解连续子数组的最大累加和，
 * 对于单行而言，不用解释，对于多行而言，可以将每一列的元素求和，使其压缩成一个单行的数组，也就等价于求解连续子数组的最大累加和
 */
int maxSumSubMatrix(vector<vector<int>>& matrix) {
    int m = matrix.size(), n = matrix[0].size(), res = INT_MIN;
    // 枚举所有的连续行组合
    // 实际过程中，为了减少时间复杂度，还可以比较行和列的大小，枚举较小的维度
    for (int i = 0; i < m; i++) {
        vector<int> sub_matrix = matrix[i];
        for (int j = i; j < m; j++) {
            // 如果j和i不同，说明现在是多行，需要将多行压缩成一行，即将每一列元素求和
            if (j != i) {
                for (int k = 0; k < n; k++) {
                    sub_matrix[k] += matrix[j][k];
                }
            }
            res = max(maxSumSubArr(sub_matrix), res);
        }
    }
    return res;
}


int main() {
//    vector<string> paths = {"b\\cst", "d\\", "a\\d\\e", "a\\b\\c"};
//    printDict(paths);
    vector<vector<int>> matrix = {{1, -2, 3, 4},
                                  {-5, 6, 2, 9},
                                  {3, -20, 1, 3}};
    cout << maxSumSubMatrix(matrix);
   return 0;
}
