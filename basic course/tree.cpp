/***********************************************************************
 * Copyright 2022 by Zhou Junping
 *
 * @file     tree.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * 该文件写的是一些关于树的代码,包括：
 * 1.树的节点类：Node；
 * 2.树的三种基本遍历方式（递归与非递归）：先序遍历，中序遍历，后序遍历；
 * 3.宽度优先遍历
 * 4.（问题1）指出某个树的最大宽度--使用哈希表
 * 5.（问题2）指出某个树的最大宽度--不使用哈希表
 * 6.（问题3）如何判断一棵树是搜索二叉树
 * 7.（问题4）如何判断一棵树是完全二叉树
 * 8.（问题5）如何判断一棵树是满二叉树
 * 9.（问题6）如何判断一棵树是平衡二叉树
 *      只要是先求解子树状态再返回根节点状态的，都可以用递归去解决，即二叉树的DP（动态规划）问题
 * 10.（问题7）给定两个二叉树节点node1和node2，找到他们的最低公共点（递归与非递归）
 * 11.（问题8）求某一个节点的后继节点
 * 12.（问题9）树的序列化和反序列化
 * 13.（问题10）微软原题：折纸问题——将一张纸对折n次，打印折痕
 * 最近修改日期：2022-10-31
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2022-10-22
 *
 */
#include<iostream>
#include<stack>
#include<queue>
#include<unordered_map>
#include<cstring>
#include<string>
#include<cmath>

using namespace std;

class Node{
public:
    Node(int val){
        value = val;
    }

    int value;
    Node *left = nullptr;
    Node *right = nullptr;
};

struct ReturnData{
    int height;
    int nodes;
    ReturnData(int h, int n){
        height = h;
        nodes = n;
    }
};

void preorder(Node* head);//先序遍历（递归）
void inorder(Node* head);//中序遍历（递归）
void postorder(Node* head);//后续遍历（递归）

void preorder_no_recur(Node* head);//先序遍历（非递归）
void inorder_no_recur(Node* head);//中序遍历（非递归）
void postorder_no_recur(Node* head);//后序遍历（非递归）

void width(Node* head);//宽度优先遍历（层次遍历）

void question1(Node* head);//指出某个树的最大宽度--使用哈希表
void question2(Node* head);//指出某个树的最大宽度--不使用哈希表
bool question3(Node* head);//如何判断一棵树是搜索二叉树
bool question4(Node* head);//如何判断一棵树是完全二叉树
bool question5(Node* head);//如何判断一棵树是满二叉树
ReturnData* process(Node* head);
int question6(Node* head);//如何判断一棵树是平衡二叉树
Node* question7(Node* head, Node* node1, Node* node2);//给定两个二叉树节点node1和node2，找到他们的最低公共点
void process(Node* head, unordered_map<Node*, Node*> &parent);//遍历整棵树，储存父节点
Node* question7_recur(Node* head, Node* node1, Node* node2);//给定两个二叉树节点node1和node2，找到他们的最低公共点(recursion)
Node* question8(Node* head, Node* node1);//找到某个节点的后继节点
string question9(Node* head);//树的序列化
Node* question9_re(string seq);//树的反序列化
Node* process9(queue<string> &strings);//递归
void question10(int n);//折纸问题——将一张纸对折n次，打印折痕
void process10(int layers, int cur_layers, bool flag);//递归

//先序遍历（递归）
void preorder(Node* head){
    if(head == nullptr){
        return;
    }
    cout<<head->value<<endl;
    preorder(head->left);
    preorder(head->right);
}

//中序遍历（递归）
void inorder(Node* head){
    if(head == nullptr){
        return;
    }
    inorder(head->left);
    cout<<head->value<<endl;
    inorder(head->right);
}

//后序遍历（递归）
void postorder(Node* head){
    if(head == nullptr){
        return;
    }
    postorder(head->left);
    postorder(head->right);
    cout<<head->value<<endl;
}

//先序遍历（非递归）
void preorder_no_recur(Node* head){
    if(head == nullptr){
        return;
    }
    stack<Node*> node_stack;
    node_stack.push(head);
    while(!node_stack.empty()){
        head = node_stack.top();
        node_stack.pop();
        cout<<head->value<<endl;
        if(head->right != nullptr){
            node_stack.push(head->right);
        }
        if(head->left != nullptr){
            node_stack.push(head->left);
        }

    }
}

//中序遍历（非递归）
void inorder_no_recur(Node* head){
    if(head == nullptr){
        return;
    }

    stack<Node*> node_stack;
    while (head != nullptr || !node_stack.empty()){
        if (head != nullptr) {
            node_stack.push(head);
            head = head->left;
        } else {
            head = node_stack.top();
            node_stack.pop();
            cout<<head->value<<" ";
            head = head->right;
        }
    }
}

//后序遍历（非递归）
void postorder_no_recur(Node* head){
    if (head == nullptr) {
        return;
    }

    stack<Node*> stack1;
    stack<Node*> stack2;
    stack1.push(head);
    while (!stack1.empty()) {
        head = stack1.top();
        stack1.pop();
        stack2.push(head);
        if (head->left != nullptr) {
            stack1.push(head->left);
        }
        if (head->right != nullptr) {
            stack1.push(head->right);
        }
    }

    while (!stack2.empty()){
        head = stack2.top();
        stack2.pop();
        cout<<head->value<<endl;
    }
}

//宽度优先遍历（层次遍历）
void width(Node* head){
    queue<Node*> node_queue;
    node_queue.push(head);
    while (!node_queue.empty()) {
        head = node_queue.front();
        node_queue.pop();
        cout<<head->value<<endl;
        if (head->left != nullptr) {
            node_queue.push(head->left);
        }
        if (head->right != nullptr) {
            node_queue.push(head->right);
        }
    }
}

//指出某个树的最大宽度--使用哈希表
void question1(Node* head){
    queue<Node*> node_queue;
    unordered_map<Node*, int> node_hash;//创建一个哈希表，用来存储节点与层数的对应关系
    int layer = 1;//记录当前层数
    int nodes = 0;//记录当前层的节点数
    int max = 0;
    node_queue.push(head);
    node_hash[head] = 1;
    while (!node_queue.empty()) {
        head = node_queue.front();
        node_queue.pop();
        int cur_layer = node_hash[head];
        if (layer == cur_layer) {
            nodes++;
            max = nodes > max ? nodes : max;
        } else {
            layer++;
            nodes = 1;
        }
        if (head->left != nullptr) {
            node_hash[head->left] = cur_layer + 1;
            node_queue.push(head->left);
        }
        if (head->right != nullptr) {
            node_hash[head->right] = cur_layer + 1;
            node_queue.push(head->right);
        }
    }
    cout<<max<<endl;
}

//指出某个树的最大宽度--不使用哈希表
void question2(Node* head){
    if (head == nullptr) {
        return;
    }
    queue<Node*> node_queue;
    Node* endNode = head;
    Node* nextNode = nullptr;
    int nodes = 0;
    int max = 0;
    node_queue.push(head);
    while (!node_queue.empty()) {
        head = node_queue.front();
        node_queue.pop();
        nodes++;
        if (head->left != nullptr) {
            node_queue.push(head->left);
            nextNode = head->left;
        }
        if (head->right != nullptr) {
            node_queue.push(head->right);
            nextNode = head->right;
        }
        if (head == endNode){
            max = nodes > max ? nodes : max;
            nodes = 0;
            endNode = nextNode;
        }
    }
    cout<<max<<endl;
}

//如何判断一棵树是搜索二叉树
bool question3(Node* head){
    if (head == nullptr) {
        cout<<"tree is null"<<endl;
        return false;
    }
    int flag = 1;
    int min = 0;
    stack<Node*> node_stack;
    while (!node_stack.empty() || head != nullptr) {
        if (head != nullptr) {
            node_stack.push(head);
            head = head->left;
        } else {
            head = node_stack.top();
            node_stack.pop();
            if (flag == 1) { //说明第一次赋值
                min = head->value;
                flag--;
            } else if (min <= head->value) {
                min = head->value;
            } else {
                return false;
            }
            head = head->right;
        }
    }
    return true;
}

//如何判断一棵树是完全二叉树
bool question4(Node* head){
    if (head == nullptr) {
        cout<<"tree is null"<<endl;
        return false;
    }

    queue<Node*> node_queue;
    node_queue.push(head);
    bool flag = false; //判断是否遇到左右节点不双全的根节点
    Node* left;
    Node* right;

    while (!node_queue.empty()) {
        head = node_queue.front();
        node_queue.pop();
        left = head->left;
        right = head->right;
        if ((left == nullptr && right != nullptr) ||             // 没有左孩子，但有右孩子
            (flag && (left != nullptr || right != nullptr))) {   // 在遇到孩子不双全之后还遇见了非叶子节点的节点
            return false;
        }
        if (left != nullptr) {
            node_queue.push(left);
        }
        if (right != nullptr) {
            node_queue.push(right);
        }
        if (left == nullptr || right == nullptr){
            flag = true;
        }
    }
    return true;
}

//如何判断一棵树是满二叉树
ReturnData* process(Node* head){
    if (head == nullptr) {
        return new ReturnData(0, 0);
    }

    ReturnData* left = process(head->left);
    ReturnData* right = process(head->right);

    int height = left->height > right->height ? left->height : right->height;
    int nodes = left->nodes + right->nodes;

    return new ReturnData(height + 1, nodes + 1);
}

//如何判断一棵树是满二叉树
bool question5(Node* head){
    if (head == nullptr) {
        return true;
    }

    ReturnData* total = process(head);
    cout<<total->height<<endl;
    cout<<total->nodes<<endl;

    return (total->nodes == ((1 << total->height) - 1));
}

//如何判断一棵树是平衡二叉树
int question6(Node* head){
    if (head == nullptr) {
        return 0;
    }

    int d_left = question6(head->left);
    int d_right = question6(head->right);

    if (d_left == -1 || d_right == -1) { // 表示左右树至少有一个不平衡
        return -1;
    } else {
        int max = d_left > d_right ? d_left : d_right;
        int min = max == d_right ? d_left : d_right;
        if (max - min >= 2) {
            return -1;
        } else {
            return max + 1 ; // 返回当前树的最大高度
        }
    }

}

//给定两个二叉树节点node1和node2，找到他们的最低公共点
Node* question7(Node* head, Node* node1, Node* node2){
    unordered_map<Node*, Node*> parent;
    process(head, parent);

    Node* p1 = node1;
    Node* p2 = node2;
    int count = 0;

    while (parent[p1] != head) {
        p1 = parent[p1];
        count++;
    }
    while (parent[p2] != head) {
        p2 = parent[p2];
        count--;
    }

    p1 = count > 0 ? node1 : node2;
    p2 = p1 == node2 ? node1 : node2;
    count = abs(count);

    while (count > 0) {
        p1 = parent[p1];
        count--;
    }

    while (p1 != p2) {
        p1 = parent[p1];
        p2 = parent[p2];
    }

    return p1;
}

//遍历整棵树，储存父节点
void process(Node* head, unordered_map<Node*, Node*> &parent){
    if (head == nullptr) {
        return;
    }

    stack<Node*> node;
    node.push(head);
    while (!node.empty()) {
        head = node.top();
        node.pop();
        if (head->right != nullptr) {
            node.push(head->right);
            parent[head->right] = head;
        }
        if (head->left != nullptr) {
            node.push(head->left);
            parent[head->left] = head;
        }
    }
}

//给定两个二叉树节点node1和node2，找到他们的最低公共点(recursion)
Node* question7_recur(Node* head, Node* node1, Node* node2){
    if (head == nullptr || head == node1 || head == node2) {//当碰到树底或者node1或者node2时返回节点
        return head;
    }

    Node* left = question7_recur(head->left, node1, node2);
    Node* right= question7_recur(head->right, node1, node2);

    if (left != nullptr && right != nullptr) {//当左右树都有返回值时，也就是说，node1和node2分别位于该节点的左右树，那么该节点就是最低公共点
        return head;
    }

    return (left != nullptr ? left : right);
}

//找到某个节点的后继节点
//思路是分该节点有无右孩子
Node* question8(Node* head, Node* node1){
    if (head == nullptr) {
        return nullptr;
    }

    if (node1->right != nullptr) {//如果有右树，那么他的后继节点就是右树上的最左节点
        while (node1->left != nullptr) {
            node1 = node1->left;
        }
        return node1;
    }

    stack<Node*> node_stack;
    unordered_map<Node*, Node*> parent;
    node_stack.push(head);
    parent[head] = nullptr;
    while (!node_stack.empty()) {
        head = node_stack.top();
        node_stack.pop();

        if (head->right != nullptr) {
            node_stack.push(head->right);
            parent[head->right] = head;
        }
        if (head->left != nullptr) {
            node_stack.push(head->left);
            parent[head->left] = head;
        }
    }

    while (parent[node1]->left != node1 && parent[node1] != nullptr) {
        node1 = parent[node1];
    }

    return parent[node1];

}

//树的序列化
string question9(Node* head){
    if (head == nullptr) {
        string seq = "#_";
        return seq;
    }

    string seq = "";
    stack<Node*> node_stack;
    node_stack.push(head);
    while (!node_stack.empty()) {
        head = node_stack.top();
        node_stack.pop();
        if (head != nullptr) {
            seq = seq + to_string(head->value) + string("_");
            node_stack.push(head->right);
            node_stack.push(head->left);
        } else {
            seq += "#_";
        }
    }

    return seq;
}

//树的反序列化
Node* question9_re(string seq){
    queue<string> strings;
    while (seq.size() != 0) {
        int index = seq.find("_");
        strings.push(seq.substr(0, index));
        seq.erase(0, index+1);
    }
//    while (!strings.empty()) {
//        if (strings.front() != "#") {
//            cout<<strings.front()<<" ";
//        }
//        strings.pop();
//    }
    Node* head = process9(strings);
    return head;
}

//递归
Node* process9(queue<string> &strings){
    if (strings.front() == "#") {
        strings.pop();
        return nullptr;
    }
    string s = strings.front();
    strings.pop();
    int val = atoi(s.c_str());
    Node* head = new Node(val);
    Node* left = process9(strings);
    Node* right = process9(strings);
    if (left != nullptr) {
        head->left = left;
    }
    if (right != nullptr) {
        head->right = right;
    }
    return head;
}

//折纸问题——将一张纸对折n次，打印折痕
void question10(int n){
    int layers = n;//二叉树的总层数

    process10(layers, 1, false);
}

//递归
void process10(int layers, int cur_layers, bool flag){ //flag false为凹， true为凸
    if (cur_layers > layers) {
        return;
    }

    process10(layers, cur_layers + 1, false);//左子树的根节点永远是凹
    cout << (flag ? "up" : "down") << endl;
    process10(layers, cur_layers + 1, true);//右子树的根节点永远是凸的
}

int main(){
    Node head = Node(4);
    Node two = Node(2);
    Node three = Node(6);
    Node four = Node(1);
    Node five = Node(3);
    Node six = Node(5);
    Node seven = Node(7);

    head.left = &two;
    head.right = &three;
    two.left = &four;
    two.right = &five;
    three.left = &six;
    three.right = &seven;

//    cout<<"------preorder-------"<<endl;
//    preorder(&head);
//    cout<<"------preorder-------"<<endl;
//    cout<<"------inorder-------"<<endl;
//    inorder(&head);
//    cout<<"------inorder-------"<<endl;
//    cout<<"------postorder-------"<<endl;
//    postorder(&head);
//    cout<<"------postorder-------"<<endl;

//    preorder_no_recur(&head);
//    inorder_no_recur(&head);
//    postorder(&head);

//    Node* test = nullptr;
//    string seq = question9(&head);
//    cout<<seq<<endl;
//    Node* res = question9_re(seq);
//    preorder(res);

//    string test = "012315";
//    test.erase(0, 1);
//    cout<<test<<endl;
//    cout<<test.size()<<endl;
//    cout<<test.find("1")<<endl;
//    cout<<test.substr(0, 1)<<endl;

    question10(3);

    return 0;
}