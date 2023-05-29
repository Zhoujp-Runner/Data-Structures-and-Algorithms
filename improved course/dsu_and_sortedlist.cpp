/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中的并查集与有序表，包含：
 * 1. 并查集的实现
 * 最近修改日期：2023-05-29
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-05-29
 *
 */


#include <iostream>
#include <unordered_map>

using namespace std;

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
    struct Package {
        Package(int v) : value(v) {}
        int value;
    };

    void create_set(int value) {
        Package set(value);
        head_map.emplace(set, set);
    }

private:
    unordered_map<Package, Package> head_map; // 查询该集合的头结点
    unordered_map<int, Package> check_map; // 查询当前的value对应的是哪一个Package
    unordered_map<Package, int> set_size; // 查询当前头结点所代表的集合的大小
};
