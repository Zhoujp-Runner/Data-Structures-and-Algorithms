/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中的并查集与有序表，包含：
 * 1. 并查集的实现
 * 最近修改日期：2023-05-31
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-05-29
 *
 */

/******************笔记*******************
 * ************并查集************ *
 * 1.   理论上来说，并查集的时间复杂度为O(f(N))
 *      f(N)是一个函数，N表示并查集内样本的数量，当N=10^80时，f(N)<6
 *      所以，可见，并查集的查询和合并都是常数时间的，前提是查询和合并的次数够多（每一次查询都会将一条路径缩短）
 * 2.   如何使用并查集实现并行加速？
 *      并查集实现并行加速的应用场景会有一定的限制
 *      岛问题(hash_structure.cpp)就可以用并查集进行加速，可以看做map_reduce过程，只不过更高级
 *      首先假设有两个CPU，那么将岛问题的矩阵左右均分成两块
 *      两块分别用两个CPU以DFS进行搜索岛的数量，并且记录边界的信息
 *      边界指的是矩阵的左右分界线
 *      信息指的是，当前位于边界处的1是属于哪个岛的，或者说由哪个点感染而来
 *      哪个点指的是infect函数的起始点，跟遍历策略有关
 *      两个CPU计算完之后，可以得到两边岛的数量以及左右两边关于中间分界线的信息
 *      由于岛的数量可能会因矩阵分开计算而增加，所以才需要统计边界信息
 *      如何处理边界信息呢？就需要用到并查集
 *      前面说到的，记录位于边界处的1属于哪个岛，意思就是，该边界处的1属于哪个集合
 *      如果发现边界另外一侧有1与其接触，且两者不属于同一个岛，那就合并岛屿，总岛屿数-1
 *      经此计算之后，就能得到最终岛屿数量
 *      这就是用并查集加速岛问题的解决办法
 */


#include <iostream>
#include <unordered_map>
#include <stack>

using namespace std;

struct Package {
    explicit Package(int v) : value(v) {}
    int value;
};
/**
 * @brief 用于实现并查集结构
 * @details 为何不用哈希表或者链表来实现并查集？
 *          因为并查集最主要的功能是查询以及合并集合
 *          如果使用哈希表，查询的时间复杂度是O(1)，合并的时间复杂度是O(N)
 *          如果使用链表，查询的时间复杂度是O(N)，合并的时间复杂度是O(1)
 *          两者都存在时间复杂度为O(N)的操作，所以不适合
 *
 *          下面将用三个哈希表以及一个封装结构来实现并查集
 */
class DSU {
public:
    //创建集合，初始时每个数单独作为集合，头结点为自身，集合大小为1
    void create_set(int value) {
        if (exist(value)) return;
        auto* set = new Package(value);
        head_map.emplace(set, set);
        check_map.emplace(value, set);
        set_size.emplace(set, 1);
    }

    // 寻找头结点
    Package* search_head(int value) {
        Package* current_head = check_map[value];
        Package* head = head_map[current_head];
        stack<Package*> path;

        // 向上找根节点
        while (current_head != head) {
            //将向上找过程中所经过的路径全部压入栈中
            path.push(current_head);
            current_head = head;
            head = head_map[current_head];
        }

        // 将路径上的所有节点的头节点都指向当前的根节点
        // 扁平化路径，防止一条路径过长
        while (!path.empty()) {
            head_map[path.top()] = head;
            path.pop();
        }

        return head;
    }

    bool isSameSet(int value1, int value2) {
        // 首先得确保两者都在某个集合当中
        if (!exist(value1) || !exist(value2)) {
            cout << "one of arguments is not in set!";
            return false;
        }

        //依次向上寻找头结点
        Package* head1 = search_head(value1);
        Package* head2 = search_head(value2);

        return head1 == head2;
    }

    void union_sets(int value1, int value2) {
        // 首先得确保两者都在某个集合当中
        if (!exist(value1) || !exist(value2)) {
            cout << "one of arguments is not in set!";
            return;
        }

        //依次向上寻找头结点
        Package* head1 = search_head(value1);
        Package* head2 = search_head(value2);

        if (head1 == head2) {
            cout << "these values are already in the same set";
            return;
        }

        // 让size小的集合的头结点指向size大的集合的头结点
        // 更新set_size
        // 在set_size 释放掉较小size的记录
        if (set_size[head1] > set_size[head2]) {
            head_map[head2] = head1;
            set_size[head1] = set_size[head1] + set_size[head2];
            set_size.erase(head2);
        } else {
            head_map[head1] = head2;
            set_size[head2] = set_size[head1] + set_size[head2];
            set_size.erase(head1);
        }

    }

    bool exist(int value) {
        return check_map.find(value) != check_map.end();
    }

private:
    unordered_map<Package*, Package*> head_map; // 查询该集合的头结点
    unordered_map<int, Package*> check_map; // 查询当前的value对应的是哪一个Package
    unordered_map<Package*, int> set_size; // 查询当前头结点所代表的集合的大小
};

int main() {
    DSU dsu_set;
    dsu_set.create_set(1);
    dsu_set.create_set(2);
    dsu_set.create_set(3);
    dsu_set.create_set(4);
    dsu_set.create_set(5);
    dsu_set.union_sets(1, 2);
    dsu_set.union_sets(3, 4);
    cout << dsu_set.isSameSet(1, 3) << endl;
    cout << dsu_set.isSameSet(2, 3) << endl;
    cout << dsu_set.isSameSet(4, 3) << endl;
    cout << dsu_set.isSameSet(5, 3) << endl;
    return 0;
}
