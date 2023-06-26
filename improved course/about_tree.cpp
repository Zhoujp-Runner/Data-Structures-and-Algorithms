
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
#include <vector>

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

struct Stuff {
    int happy;
    vector<Stuff*> sub;
    explicit Stuff(const int & h) {
        happy = h;
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
    // 需要返回的返回值类型
    struct Info {
        int max_distance;
        int height;
        Info(const int & m, const int & h) : max_distance(m), height(h) {}
    };
public:
    explicit MaxDistance(Node* head) : root(head) {}  // 防止隐式转换

    int get_max_distance() {
        Info result = process(root);
        return result.max_distance;
    }

    Info process(Node* node) {
        if (node == nullptr) {
            return {0, 0};  // 叶子节点的最大距离和高度都是0
        }

        Info right = process(node->right);  // 计算右子树
        Info left = process(node->left);  // 计算左子树
        int max_distance = max(right.height + left.height + 1, max(right.max_distance, left.max_distance));  // 将左右子树的结果和通过当前节点的结果作对比
        int height = max(right.height, left.height) + 1;  // 当前节点为根节点的树的高度为左右子树中较大的高度+1
        return {max_distance, height};
    }
};


/**
 * 派对的最大快乐值问题
 * 公司中，每个员工都可以带来一定的快乐值，现在举行一个派对，规定如果一个员工参加了派对，那么他的直属下级就不能参加派对，求该派对的最大快乐值（派对的快乐值就是所有人快乐值的累加）
 * 那么同样分情况讨论
 * 1.如果当前员工来了，那么以当前员工为根节点的树的最大快乐值就是当前员工的快乐值加上直属下级没来的最大快乐值
 * 2.如果当前员工没来，那么最大快乐值就是直属下级来的最大快乐值和没来的最大快乐值中较大者
 */
class Party {
private:
    struct Info {
        int present_happy;  // 头节点参加派对的快乐值
        int absent_happy;  // 头节点不参加派对的快乐值
        Info(const int & p, const int & a) : present_happy(p), absent_happy(a) {}
    };

public:
    int get_max_happy(Stuff* boss) {
        Info happy = process(boss);
        return max(happy.present_happy, happy.absent_happy);
    }

    Info process(Stuff* stuff) {
        if (stuff->sub.empty()) {  // 当stuff没有员工列表时，说明是底层员工，来快乐值就是自己的快乐值，不来的快乐值就是0
           return {stuff->happy, 0};
        }

        int present_happy = stuff->happy;  // 当stuff来的时候，快乐值初始为stuff的快乐值
        int absent_happy = 0;  // 当stuff不来的时候，快乐值初始为0
        for (auto next : stuff->sub) {
            Info sub = process(next);
            present_happy += sub.absent_happy;  // stuff来时，只需要加上下级员工不来时的最大值
            absent_happy += max(sub.present_happy, sub.absent_happy);  // stuff不来时，需要加上下级员工来与不来之间的最大值
        }

        return {present_happy, absent_happy};
    }
};


int main() {
//    Node* one = new Node(1);
//    Node* two = new Node(2);
//    Node* three = new Node(3);
//    Node* four = new Node(4);
//    Node* five = new Node(5);
//    Node* six = new Node(6);
//    Node* seven = new Node(7);
//    Node* eight = new Node(8);
//    Node* nine = new Node(9);
//    Node* ten = new Node(10);
//
//    one->left = two;
//    one->right = three;
//    two->left = four;
//    two->right = five;
//    four->left = six;
//    four->right = seven;
//    five->right = eight;
//    six->left = nine;
//    eight->right = ten;
//
//    MaxDistance m(one);
//    cout << m.get_max_distance() << endl;

    Stuff* a = new Stuff(70);
    Stuff* b = new Stuff(80);
    Stuff* c = new Stuff(3);
    Stuff* d = new Stuff(20);
    Stuff* e = new Stuff(20);
    Stuff* f = new Stuff(30);
    Stuff* g = new Stuff(10);
    Stuff* h = new Stuff(20);
    a->sub.emplace_back(b);
    a->sub.emplace_back(c);
    a->sub.emplace_back(d);
    b->sub.emplace_back(e);
    c->sub.emplace_back(f);
    d->sub.emplace_back(g);
    d->sub.emplace_back(h);

    Party party;
    cout << party.get_max_happy(a) << endl;
    return 0;
}

