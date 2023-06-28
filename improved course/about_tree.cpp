
/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中关于树的一些算法，包含：
 * 笔记（树型DP、Morris遍历、Morris遍历和递归遍历的应用条件）
 * 用树型DP解两个问题
 * Morris遍历的代码实现、以及用其实现前序、中序、后序
 * 用Morris遍历判断是否是搜索二叉树
 * 最近修改日期：2023-06-28
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
 *
 * ********* Morris遍历（线索二叉树） **********
 * 步骤：
 *  假设来到当前节点cur，开始时cur来到头节点位置
 *      1）如果cur没有左孩子，cur向右移动（cur=cur.right）
 *      2）如果cur有左孩子，找到左子树上最右的节点mostRight：
 *          a.如果mostRight的右指针指向空，让其指向cur，然后cur向左移动（cur=cur.left）
 *          b.如果mostRight的右指针指向cur，让其指向null，然后cur向右移动（cur=cur.right）
 *      3）cur为空时遍历停止
 * Morris遍历通过利用空闲指针，来达到空间复杂度为O(1)的目的
 * Morris遍历需要找每个节点的左子树上的最右节点（会找两遍），所以每条寻找路径都是不重复的，时间复杂度为O(N)
 * Morris遍历中，如果一个节点有左子树，那么会经过这个节点两次
 *
 * 什么时候Morris遍历时最优解？换句话说，什么时候可以用Morris遍历，什么时候只能用递归遍历
 * 如果一个任务要求当前节点必须做第三次信息的强整合（例如必须收集左子树的信息以及右子树的信息之后才能得到当前节点的信息），那么只能使用二叉树的递归
 * 如果该任务不一定需要做第三次信息的强整合（例如下面写的判断一棵树是否是搜索二叉树），那么Morris遍历会是最优解
 * 因为递归遍历能够一个节点经过三次
 * 而Morris遍历，一个节点，如果有左子树能经过两次，如果没有左子树只能经过一次
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


/**
 * Morris遍历
 * 先序遍历：只经过一次的节点直接打印，经过两次的节点第一次打印
 * 中序遍历：只经过一次的节点直接打印，经过两次的节点第二次打印
 * 后序遍历：当某个节点经过第二次时，逆序打印其左子树的右边界，当完成遍历时，再逆序打印整棵树的右边界
 */
class Morris {
public:
    void preorder(Node* root) {
        if (root == nullptr) return;
        Node* current = root;
        Node* mostRight = nullptr;

        while (current != nullptr) {
            if (current->left != nullptr) {  // 如果有左子树
                mostRight = current->left;  // 初始时，左子树的根节点就是左树上的最右节点
                while (current != mostRight->right && mostRight->right != nullptr) {  // 最右节点的左孩子不指向空，或者不指向current
                    mostRight = mostRight->right;  // mostRight不断向右移动
                }

                if (mostRight->right == nullptr) {  // 第一次来到current
                    cout << current->value << endl;
                    mostRight->right = current;
                    current = current->left;
                    continue;
                }

                if (mostRight->right == current) {  // 第二次来到current
                    mostRight->right = nullptr;
                }
            } else {
                cout << current->value << endl;
            }

            current = current->right;
        }
    }

    void inorder(Node* root) {
        if (root == nullptr) return;
        Node* current = root;
        Node* mostRight = nullptr;

        while (current != nullptr) {
            if (current->left != nullptr) {  // 如果有左子树
                mostRight = current->left;  // 初始时，左子树的根节点就是左树上的最右节点
                while (current != mostRight->right && mostRight->right != nullptr) {  // 最右节点的左孩子不指向空，或者不指向current
                    mostRight = mostRight->right;  // mostRight不断向右移动
                }

                if (mostRight->right == nullptr) {  // 第一次来到current
                    mostRight->right = current;
                    current = current->left;
                    continue;
                }

                if (mostRight->right == current) {  // 第二次来到current
                    mostRight->right = nullptr;
                }
            }

            cout << current->value << endl;
            current = current->right;
        }
    }

    void postorder(Node* root) {
        if (root == nullptr) return;
        Node* current = root;
        Node* mostRight = nullptr;

        while (current != nullptr) {
            if (current->left != nullptr) {  // 如果有左子树
                mostRight = current->left;  // 初始时，左子树的根节点就是左树上的最右节点
                while (current != mostRight->right && mostRight->right != nullptr) {  // 最右节点的左孩子不指向空，或者不指向current
                    mostRight = mostRight->right;  // mostRight不断向右移动
                }

                if (mostRight->right == nullptr) {  // 第一次来到current
                    mostRight->right = current;
                    current = current->left;
                    continue;
                }

                if (mostRight->right == current) {  // 第二次来到current
                    mostRight->right = nullptr;
                    reverse_print_right_edge(current);
                    /**
                     * 这里把current->left改成current就会出现与我实际推的时候不相符的问题
                     * 解答：刚开始推导的时候，忘记了树最右节点的右指针不为空
                     * 也就是说，如果写的是current，那么树的最右此时以current为根节点的树的最右节点的指针是指向上一级有左子树的节点，所以右边界会一直往回找，直到找完原始树的右边界
                     */
                }
            }

            current = current->right;
        }
        reverse_print_right_edge(root);
    }

    // 逆序打印某个节点为根节点的树的右边界
    void reverse_print_right_edge(Node* node) {
        Node* reverse = reverse_edge(node);
        Node* cur = reverse;
        while (cur != nullptr) {
            cout << cur->value << endl;
            cur = cur->right;
        }
        reverse_edge(reverse);
    }

    // 逆序某个节点的右边界（就和链表的逆序一样）
    Node* reverse_edge(Node* node) {
        Node* pre = nullptr;
        Node* next = nullptr;
        while (node != nullptr) {
            next = node->right;
            node->right = pre;
            pre = node;
            node = next;
        }

        return pre;
    }

    // 用Morris遍历判断一棵树是否是搜索二叉树
    bool isBST(Node* node) {
        // 如果树为空，则认为是搜索二叉树
        if (node == nullptr) {
            return true;
        }
        Node* current = node;
        Node* mostRight = nullptr;
        int previous = INT_MIN;

        while (current != nullptr) {
            if (current->left != nullptr) {
                mostRight = current->left;
                while (mostRight->right != nullptr && mostRight->right != current) {
                    mostRight = mostRight->right;
                }

                if (mostRight->right == nullptr) {
                    mostRight->right = current;
                    current = current->left;
                    continue;
                } else {
                    mostRight->right = nullptr;
                }
            }
            if (current->value < previous) {
                return false;
            }
            previous = current->value;
            current = current->right;
        }
        return true;
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

    four->left = two;
    four->right = six;
    two->left = one;
    two->right = three;
    six->left = five;
    six->right = seven;
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
    Morris morris;
    cout << morris.isBST(four);

//    Stuff* a = new Stuff(70);
//    Stuff* b = new Stuff(80);
//    Stuff* c = new Stuff(3);
//    Stuff* d = new Stuff(20);
//    Stuff* e = new Stuff(20);
//    Stuff* f = new Stuff(30);
//    Stuff* g = new Stuff(10);
//    Stuff* h = new Stuff(20);
//    a->sub.emplace_back(b);
//    a->sub.emplace_back(c);
//    a->sub.emplace_back(d);
//    b->sub.emplace_back(e);
//    c->sub.emplace_back(f);
//    d->sub.emplace_back(g);
//    d->sub.emplace_back(h);
//
//    Party party;
//    cout << party.get_max_happy(a) << endl;
    return 0;
}

