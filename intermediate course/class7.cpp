/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class7.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.求一颗完全二叉树的节点数（时间复杂度低于O(N)）
 * 最近修改日期：2023-11-24
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
