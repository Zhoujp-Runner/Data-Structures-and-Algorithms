/***********************************************************************
 * Copyright 2022 by Zhou Junping
 *
 * @file     dijkstra_heap.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * 该文件写的是使用堆加速的dijkstra算法
 * 最近修改日期：2023-01-01
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2022-12-12
 *
 */


#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

/////////////////构建图以及特殊的堆/////////////////////
struct Node;
struct Edge;

//定义节点结构体
struct Node {
    int value; //节点本身的值
    int in; //入度
    int out; //出度
    vector<Node*> next_nodes;
    vector<Edge*> edges;

    Node(int v, int i=0, int o=0) {
        value = v;
        in = i;
        out = o;
    }
};

//定义边结构体
struct Edge {
    Node* from;
    Node* to;
    int weight;

    Edge(int w, Node* f, Node* t) {
        from = f;
        to = t;
        weight = w;
    }
};

//定义图
class Graph {
public:
    unordered_map<int, Node*> nodes;
    unordered_set<Edge*> edges;
};

//定义一个结构体，用于当做堆节点删除时的返回值
struct H_return {
    Node* node;
    int path;

    H_return(Node* n, int p) {
        node = n;
        path = p;
    }

    H_return() {
        node = nullptr;
        path = 0;
    }
};

//重写一遍堆排序，使其服务于改进的Dijkstra
//要求该堆排序能够允许直接在堆上修改节点值，并做相应调整，使其重新成为一个堆
//该堆需要能够随意插入和删除节点
class Heap_advanced {
public:
    //初始化用于存放堆结构的数组
    Heap_advanced(int len) { //len即为图中的节点数
        heap_arr = new int[len]; ////注意！！！数组动态内存分配是用[]，不是()
        heap_len = len;
        cur_len = 0; //初始时堆的大小为0

        //将堆中元素初始化为0
        for (int i = 0; i < len; ++i) {
            heap_arr[i] = 0;
        }
    }

    //析构时释放heap_arr指针
    ~Heap_advanced() {
        delete[] heap_arr;
    }

    //交换heap_arr中的两个数（也可以使用位运算）
    void swap(int i, int j) {
        int temp = heap_arr[i];
        heap_arr[i] = heap_arr[j];
        heap_arr[j] = temp;
    }

    //向上调整（默认调节成小根堆）
    void heap_insert(Node* node) {
        int index = node_index[node]; // 通过哈希表查找索引值
        while (heap_arr[index] < heap_arr[(index - 1) / 2]) {
            swap(index, (index - 1) / 2);
            //更新图节点对应的索引值
            for(auto & i : node_index) {
                if (i.second == index) {
                    i.second = (index - 1) / 2;
                } else if (i.second == (index - 1) / 2) {
                    i.second = index;
                }
            }
            index = (index - 1) / 2;
        }
    }

    //向下调整（默认小根堆）
    void heapify(int cur_heap_size, Node* node) {
        int cur_index = node_index[node]; //通过哈希表找到索引值
        //判断条件是父节点还是否有左孩子（由于堆结构是完全二叉树，所以仅通过判断是否有左孩子就能够确定当前节点是否有孩子）
        while(cur_index * 2 + 1 < cur_heap_size) {
            //其实实际上不用写左右孩子的索引，因为都可以用父节点的索引表示
            int left_index = cur_index * 2 + 1;
            //防止右孩子越界
            int right_index = left_index + 1 >= cur_heap_size ? left_index : left_index + 1;

            //左右孩子中小的那一个
//            int max_child = heap_arr[left_index] > heap_arr[right_index] ? heap_arr[left_index] : heap_arr[right_index];
            int max_child_index = heap_arr[left_index] < heap_arr[right_index] ? left_index : right_index;
            //如果父节点大于较小的孩子，那么就交换，否则结束循环
            if (heap_arr[cur_index] > heap_arr[max_child_index]) {
                swap(cur_index, max_child_index);
                for(auto & i : node_index) { //更新图节点对应的索引值
                    if (i.second == cur_index) {
                        i.second = max_child_index;
                    } else if (i.second == max_child_index) {
                        i.second = cur_index;
                    }
                }
                cur_index = max_child_index;
            } else {
                break;
            }
        }
    }

    void print_arr() {
        for (int i = 0; i < cur_len; ++i) {
            cout<<heap_arr[i]<<",";
        }
        cout<<endl;
    }

    void get_min_node() {
        for (auto i : node_index) {
            if (i.second == 0) {
                Node* min_node = i.first;
                cout<<min_node->value<<endl;
            }
        }
    }

    void print_map() {
        for (auto i : node_index) {
            cout<<i.first<<":"<<i.second<<", ";
        }
    }

    //修改堆中的某个节点的数值
    void modify(Node* node, const int value) {
        //通过哈希表查找图节点对应的索引值
        int index = node_index[node];
        //记住堆中节点原先的值
        int pre_value = heap_arr[index];
        //修改堆中节点的值
        heap_arr[index] = value;

        if (pre_value == value) { //如果原先的值和修改之后的值一样大，那么直接返回
            return;
        } else if (value < pre_value) { //如果修改之后的值比原先的小，那么向上调整
            heap_insert(node);
        } else if (value > pre_value) { //如果修改之后的值比原先的大，那么向下调整
            heapify(cur_len, node);
        }
    }

    //在堆中新插入一个节点
    void insert(Node* node, const int value) {
        if (node_index.find(node) != node_index.end() || cur_len >= heap_len) { //确保新插入的图节点之前没有插入过 || 确保插入的图节点数小于堆的大小
            cout<<"node have existed or heap is full!"<<endl;
            return;
        }

        node_index[node] = cur_len; // 记住图节点对应的索引值
        heap_arr[cur_len] = value; //在堆中插入值
        heap_insert(node); //由于每次是在堆的最后插入新节点，所以只需要进行一次向上调整就可以
        cur_len += 1; //当前堆的大小加1
    }

    //向堆中进行写的操作（包括插入和修改）
    void write(Node* node, const int value) {
        if (node_index.find(node) == node_index.end() || node_index[node] == -1) { // 事实上等于-1根本不可能出现，因为一个节点被删掉之后就会进入lock
            insert(node, value);
        } else {
            modify(node, value);
        }
    }

    //删除堆顶的节点，并返回堆顶数值
    H_return delete_node() {
        swap(0, cur_len - 1);
        Node* adjust_node = nullptr; // 记录需要向下调整的node
        Node* return_node = nullptr; // 记录需要返回的node
        for (auto & i : node_index) { // 更新索引值
            if (i.second == 0) {
                i.second = - 1; // 变成-1代表该节点不存在与堆上
                return_node = i.first;
            } else if (i.second == cur_len - 1) {
                i.second = 0;
                adjust_node = i.first;
            }
        }
        cur_len -= 1;
        if (adjust_node != nullptr) { // 排除堆中只有一个节点时弹出节点，导致没有需要向下调整的节点
            heapify(cur_len, adjust_node); //进行一次向下调整
        }
        return H_return(return_node, heap_arr[cur_len]);
    }

    //判断堆是否为空
    bool is_empty(){
        return cur_len == 0;
    }

    //查找某个图节点在堆上对应的值
    int search(Node* node) {
//        bool condition1 = node_index[node] == -1;
//        bool condition2 = node_index.find(node) == node_index.end();
        if (node_index.find(node) == node_index.end()){
            cout<<"node does not exist!"<<endl;
            return 100000; //本意上是返回一个无穷大
            // 这里有点问题，这个数虽然很大，但是可能出现比他更大的数，这时候dij方法就会出错
        }
        return heap_arr[node_index[node]];
    }

protected:
    int * heap_arr;
    int heap_len;
    int cur_len; //表示当前堆的大小，也表示新插入的图节点在数组中的索引值
    unordered_map<Node*, int> node_index; //用于存放图节点对应的索引值
};
/////////////////构建图以及特殊的堆/////////////////////

//Dijkstra算法
unordered_map<Node*, int> dijkstra(Node* start) {
    unordered_map<Node*, int> lock; // 用于记录哪些节点的路径已经被锁住了
    Heap_advanced heap_dij(5);
    heap_dij.insert(start, 0);
    H_return min_path;

    while (!heap_dij.is_empty()) {
        min_path = heap_dij.delete_node();//弹出最小路径
        int pri_path = min_path.path;
        Node* cur_node = min_path.node;
        lock[cur_node] = pri_path; // 锁住该节点

        for (auto i : cur_node->edges) {
            Node* next_node = i->from == cur_node ? i->to : i->from; //确定下一个节点
            int cur_path = pri_path + i->weight;
            bool condition1 = lock.find(next_node) == lock.end();
//            pri_path = heap_dij.search(next_node);
            bool condition2 = heap_dij.search(next_node) > cur_path;
            //如果节点不存在于锁中 且 当前的路径长度
            if (condition1 && condition2) {
                heap_dij.write(next_node, cur_path);
            }
        }
    }
    return lock;
}

int main() {
///////////////////////////创建图//////////////////////////
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
///////////////////////////创建图//////////////////////////
//    Heap_advanced heap_dij(5);
//    heap_dij.insert(one, 5);
//    heap_dij.insert(two, 3);
//    heap_dij.insert(three, 2);
//    heap_dij.insert(four, 8);
//    heap_dij.insert(five, 1);
//    heap_dij.print_map();
//    H_return r = heap_dij.delete_node();
//    cout<<r.node<<endl;
//    cout<<five<<endl;
    unordered_map<Node*, int> res = dijkstra(one);
    for (auto i : res) {
        cout<<i.first->value<<":"<<i.second<<",";
    }
    return 0;
}