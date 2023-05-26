/***********************************************************************
 * Copyright 2022 by Zhou Junping
 *
 * @file     graph.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * 该文件写的是一些关于图的代码,包括：
 * 1.定义了一种图的表示结构
 * 2.宽度优先遍历（无向图）
 * 3.深度优先遍历
 * 4.拓扑排序
 * 5.一种简单的并查集的类
 * 6.（问题1）生成最小生成树（kruskal和prim算法）--要求的是无向图，这里的代码前提是图是联通的
 * 7.（问题2）Dijkstra算法
 * 最近修改日期：2022-11-07
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2022-10-31
 *
 */

#include<iostream>
//#include<hash_set>
//#include<hash_map>
#include<unordered_map>
#include<unordered_set>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

struct Node;

struct Edge{
    int weight;
    Node* from;
    Node* to;

    Edge(int w, Node* f, Node* t){
        weight = w;
        from = f;
        to = t;
    }

    bool operator < (const Edge& other) const {
        return this->weight > other.weight;
    }
};

struct Node{
    int value;
    int in;//入度
    int out;//出度
    vector<Node*> next_nodes;
    vector<Edge*> edges;

    Node(int v){
        value = v;
        in = 0;
        out = 0;
    }

    Node(int v, int i, int o){
        value = v;
        in = i;
        out = o;
    }
};

class Graph{
public:
    unordered_map<int, Node*> nodes;
    unordered_set<Edge*> edges;
};

class Sets{
public:
    unordered_map<Node*, int> sets;
    Sets(unordered_map<int, Node*> nodes){
        int set_ID = 1;
        for (auto i : nodes) {
            sets[i.second] = set_ID;
            set_ID++;
        }
    }

    bool isSameSet(Node* node1, Node* node2) {
        if (int(sets[node1]) == 0 || int(sets[node2] == 0)) {
            cout<<"node is not existing"<<endl;
            return false;
        }
        return sets[node1] == sets[node2];
    }

    void uni_Set(Node* node1, Node* node2) {
        int node1_ID = sets[node1];
        int node2_ID = sets[node2];
        for (auto& i : sets) {
            if (i.second == node2_ID) {
                i.second = node1_ID;
//                sets[i.first] = node1_ID;
            }
        }
    }
};

void BFS(Node* node);//宽度优先遍历（无向图）
void DFS(Node* node);//深度优先遍历
void TopologySort(Graph graph);//拓扑排序
vector<Edge> question1_k(Graph graph);//生成最小生成树（kruskal算法）
vector<Edge> question1_p(Graph graph);//生成最小生成树（prim算法）
unordered_map<Node*, int> question2(Node* start);//Dijkstra算法
Node* get_minNode(unordered_map<Node*, int> pri_path, unordered_set<Node*> lock);//在pri_path中寻找不存在于lock中的最小值点


//宽度优先遍历（无向图）
void BFS(Node* node){
    if (node == nullptr) {
        return;
    }

    queue<Node*> nodes;
    unordered_set<Node*> set_nodes;
    nodes.push(node);
    set_nodes.insert(node);
    while (!nodes.empty()) {
        node = nodes.front();
        nodes.pop();
        cout<<node->value<<endl;
//        auto iter = node->next_nodes.begin();
//        while (iter != node->next_nodes.end()) {
        for (auto i: node->next_nodes){
//            iter++;
            if (set_nodes.find(i) == set_nodes.end()) {//保证输出不重复
                nodes.push(i);
                set_nodes.insert(i);
            }
        }
    }
}

//深度优先遍历
void DFS(Node* node){
    if (node == nullptr) {
        return;
    }
    stack<Node*> node_stack;
    unordered_set<Node*> node_set;
    node_stack.push(node);
    node_set.insert(node);
    cout<<node->value<<endl;
    while (!node_stack.empty()) {
        node = node_stack.top();
        node_stack.pop();
        for (auto i : node->next_nodes) {
            if (node_set.find(i) == node_set.end()) {
                node_stack.push(node);
                node_stack.push(i);
                node_set.insert(i);
                cout<<i->value<<endl;
                break;//保证每次只在一条路径上压入节点
            }
        }

    }
}

//拓扑排序
void TopologySort(Graph graph){
    unordered_map<Node*, int> node_in;//key是节点，value是当前剩余的入度
    queue<Node*> node_sort;//当入度为0时，进队列
    for (auto i : graph.nodes) {
        node_in[i.second] = i.second->in;
        if (i.second->in == 0) {
            node_sort.push(i.second);
//            node_in.erase(i.second);
        }
    }

//    while (!node_sort.empty()) {
//        Node* cur = node_sort.front();
//        node_sort.pop();
//        cout<<cur->value<<endl;
//        for (auto i : cur->next_nodes) {
//            node_in[i]--;
//        }
//        for (auto j : node_in) {
//            if (j.second == 0) {
//                node_sort.push(j.first);
//                node_in.erase(j.first);
//            }
//        }
//    }
    while (!node_sort.empty()) {
        Node* cur = node_sort.front();
        node_sort.pop();
        cout<<cur->value<<endl;
        for (auto i : cur->next_nodes) {
            node_in[i]--;
            if (node_in[i] == 0) {
                node_sort.push(i);
            }
        }
    }
}

//生成最小生成树（kruskal算法）
vector<Edge> question1_k(Graph graph){
    priority_queue<Edge> pri_edge;
    vector<Edge> result;
    Sets sets(graph.nodes);
    for (auto i : graph.edges) {
        pri_edge.push(*i);
    }
    while (!pri_edge.empty()) {
        Edge cur_edge = pri_edge.top();
        pri_edge.pop();
        if (!sets.isSameSet(cur_edge.from, cur_edge.to)) {
            result.push_back(cur_edge);
            sets.uni_Set(cur_edge.from, cur_edge.to);
        }
    }
    return result;
}

//生成最小生成树（prim算法）
//如果图存在不连通区域时，需要在外面加一个遍历所有节点的循环
vector<Edge> question1_p(Graph graph){
    priority_queue<Edge> pri_edge;
    unordered_set<Node*> set;
    vector<Edge> result;
    Node* cur_node = graph.nodes[1];
    set.insert(cur_node);
    for (auto j: cur_node->edges) {
        pri_edge.push(*j);
    }
     while (!pri_edge.empty()) {
        Edge cur_edge = pri_edge.top();
        pri_edge.pop();
        if (set.find(cur_edge.to) == set.end() || set.find(cur_edge.from) == set.end()){
            cur_node = set.find(cur_edge.to) == set.end() ? cur_edge.to : cur_edge.from;
            set.insert(cur_node);
            for (auto i : cur_node->edges) {
                pri_edge.push(*i);
            }
            result.push_back(cur_edge);
        }
    }
    return result;

}

//在pri_path中寻找不存在于lock中的最小值点
Node* get_minNode(unordered_map<Node*, int> pri_path, unordered_set<Node*> lock){
    int min = 0;
    int flag = 0;
    Node* minNode = nullptr;
    for (auto i : pri_path) {
        if (lock.find(i.first) == lock.end()) {
            if (flag == 0) {
                min = i.second;
                flag = 1;
            }
            if (i.second < min) {
                min = i.second;
                minNode = i.first;
            }
        }
    }
    return minNode;
}

//Dijkstra算法
unordered_map<Node*, int> question2(Node* start){
    unordered_map<Node*, int> pri_path;
    unordered_set<Node*> lock;
    pri_path[start] = 0;
    Node* cur_node = start;

    //开始逐次搜索
    while (cur_node != nullptr) {
        int cur_length = pri_path[cur_node];
        for (auto cur_edge : cur_node->edges) {
            Node* toNode = cur_edge->from == cur_node ? cur_edge->to : cur_edge->from;
            int path_length = cur_length + cur_edge->weight;
            if (pri_path.find(toNode) == pri_path.end()) {
                pri_path[toNode] = path_length;
            } else if (path_length < int(pri_path[toNode]) && lock.find(toNode) != lock.end()) {
                pri_path[toNode] = path_length;
            }
        }
        lock.insert(cur_node);
        cur_node = get_minNode(pri_path, lock);
    }
    return pri_path;
}

int main(){
    ////////////////////////create graph/////////////////////
    Node* one = new Node(1, 0, 3);
    Node* two = new Node(2, 1, 1);
    Node* three = new Node(3, 1, 1);
    Node* four = new Node(4, 1, 1);
    Node* five = new Node(5, 3, 0);

    Edge* one_two = new Edge(1, one, two);
    Edge* one_three = new Edge(4, one, three);
    Edge* one_four = new Edge(5, one, four);
    Edge* two_five = new Edge(2, two, five);
    Edge* three_five = new Edge(3, three, five);
    Edge* four_five = new Edge(6, four, five);

    one->next_nodes.push_back(two);
    one->next_nodes.push_back(three);
    one->next_nodes.push_back(four);
    two->next_nodes.push_back(one);
    two->next_nodes.push_back(five);
    three->next_nodes.push_back(one);
    three->next_nodes.push_back(five);
    four->next_nodes.push_back(one);
    four->next_nodes.push_back(five);
    five->next_nodes.push_back(two);
    five->next_nodes.push_back(three);
    five->next_nodes.push_back(four);

    one->edges.push_back(one_two);
    one->edges.push_back(one_three);
    one->edges.push_back(one_four);
    two->edges.push_back(one_two);
    two->edges.push_back(two_five);
    three->edges.push_back(one_three);
    three->edges.push_back(three_five);
    four->edges.push_back(one_four);
    four->edges.push_back(four_five);
    five->edges.push_back(four_five);
    five->edges.push_back(three_five);
    five->edges.push_back(two_five);

    Graph graph;
    graph.nodes[1] = one;
    graph.nodes[2] = two;
    graph.nodes[3] = three;
    graph.nodes[4] = four;
    graph.nodes[5] = five;
    graph.edges.insert(one_two);
    graph.edges.insert(one_three);
    graph.edges.insert(one_four);
    graph.edges.insert(two_five);
    graph.edges.insert(three_five);
    graph.edges.insert(four_five);
//    graph.edges.insert(one_two);
    ////////////////////////create graph/////////////////////


    ////////////////////////Test code////////////////////////
//    Sets set(graph.nodes);
//    cout<<set.isSameSet(one, two)<<endl;
//    set.uni_Set(one, two);
//    cout<<set.isSameSet(one, two)<<endl;
//    TopologySort(graph);
//    unordered_map<int, char> test;
//    test[1] = 'a';
//    test[2] = 'b';
//    test[3] = 'c';
//    cout<<int(test[4])<<endl;
//    for (auto i : test) {
//        cout<< typeid(i).name() <<endl;
//    }
//    unordered_map<int, char>::iterator iter;
//    cout<< typeid(iter).name() <<endl;

//    priority_queue<Edge> que;
//    que.push(*one_two);
//    que.push(*one_four);
//    que.push(*one_three);
//    while (!que.empty()) {
//        cout<<que.top().weight<<endl;
//        que.pop();
//    }
//    vector<Edge> result = question1_p(graph);
//    for (auto i : result) {
//        cout<<i.weight<<endl;
//    }
    ////////////////////////Test code////////////////////////

    unordered_map<Node*, int>result = question2(one);
    for (auto i : result) {
        cout<<i.first->value<<':'<<i.second<<endl;
    }

    return 0;
}
