
/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中关于树的一些算法，包含：
 * 笔记（树型DP）
 * 用树型DP解两个问题
 * 最近修改日期：2023-06-19
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-6-19
 *
 */

/**********************笔记**********************
 * 树形DP套路：
 *      使用前提：如果题目求解目标是S规则，则求解流程可以定成以每一个节点为头节点的子树在S规则下的每一个答案，并且最终答案一定在其中
 *      第一步：以某个节点X为头节点的子树中，分析答案有哪些可能性，并且这种分析是以X的左子树、X的右子树和X整棵树的角度来考虑可能性
 *              常见的树形DP的分类为，包含头节点X  与  不包含头节点X  两类
 *      第二步：根据第一步的可能性分析，列出所有需要的信息
 *      第三步：合并第二步的信息，对左树和右树提出同样的要求，并写出信息结构
 *      第四步：设计递归函数，递归函数是处理以X为头节点的情况下的答案。
 *              包括设计递归的basecase，默认直接得到左树和右树的所有信息，以及把可能性做整合，并且要返回第三步的信息结构
 *
 */

#include <iostream>

using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;
    explicit Node(const int & v) {
        value = v;
        left = nullptr;
        right = nullptr;
    }
};

/**
 * 求一颗二叉树的任意两节点之间的最大距离
 * 分两类，1.最大距离的路径通过头节点，2.最大距离的路径不通过头节点
 * 第1类 当通过头节点时，最大距离就是左子树的高度加上右子树的高度然后再加1
 * 第2类 当不通过头节点时，最大距离就是左子树的最大距离和右子树的最大距离中较大值
 * 所以信息结构就需要包含以当前节点为根节点的树的最大距离以及当前树的高度
 * basecase就是当遍历到叶子节点时，直接返回最大距离和高度，两者都为0
 */
class MaxDistance {
private:
    Node* root;
    struct Info {
        int max_distance;
        int height;
        Info(const int & m, const int & h) : max_distance(m), height(h) {}
    };
public:
    explicit MaxDistance(Node* head) : root(head) {}

    int get_max_distance() {
        Info result = process(root);
        return result.max_distance;
    }

    Info process(Node* node) {
        if (node == nullptr) {
            return {0, 0};
        }

        Info right = process(node->right);
        Info left = process(node->left);
        int max_distance = max(right.height + left.height + 1, max(right.max_distance, left.max_distance));
        int height = max(right.height, left.height) + 1;
        return {max_distance, height};
    }
};


int main() {
    Node* one = new Node(1);
    Node* two = new Node(2);
    Node* three = new Node(3);
    Node* four = new Node(4);
    Node* five = new Node(5);
    Node* six = new Node(6);
    Node* seven = new Node(7);
    Node* eight = new Node(8);
    Node* nine = new Node(9);
    Node* ten = new Node(10);

    one->left = two;
    one->right = three;
    two->left = four;
    two->right = five;
    four->left = six;
    four->right = seven;
    five->right = eight;
    six->left = nine;
    eight->right = ten;

    MaxDistance m(one);
    cout << m.get_max_distance() << endl;
    return 0;
}

