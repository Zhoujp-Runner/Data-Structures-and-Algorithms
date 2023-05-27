/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中的与哈希函数有关的结构，包含：
 * 笔记
 * 1. 问题 1 设计RandomPool结构 （line72-139）
 * 最近修改日期：2023-05-27
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-05-26
 *
 */

/**********笔记***********
 * ************性质************ *
 * 1.   哈希函数输入域是无穷的，输出域是有限的
 *      输出域：
 *          MD5 返回值 0 -- (2^64 - 1)
 *          SHa1返回值 0 -- (2^128 - 1)
 *          Java返回值 0 -- (2^32 - 1)
 * 2.   相同的输入参数一定会返回相同的输出值
 * 3.   不同的输入可能导致相同的输出（哈希碰撞/哈希冲突，概率比较低）
 * 4.   （最重要的性质）哈希函数的映射均匀离散。
 *      即哈希函数的输出在哈希函数值域上是均匀离散的
 *      即使输入是有规律的连续，输出也是无规律的离散，且分布是均匀的
 * 5.   如果一个哈希函数的输出符合均匀离散的性质，且值域大小为 0 -- y
 *      那么将该函数的输出模上(%)一个数m，那么最终的输出也是均匀离散的，且值域大小为 0 -- (m - 1)
 *
 *
 * 使用哈希函数的例子
 * 问题:统计40亿个数中出现次数最多的数，最多使用1GB的内存
 * 分析:如果直接使用哈希表计算去统计，极端情况下（40亿个数都是不相同的）需要占用约32GB的内存，是不行的
 *     所以，就不能直接使用哈希表去存。
 *     可以考虑使用哈希函数，先使用哈希函数将40亿个参数映射到 0 -- y 的范围内，再对其取模，使其值域变为 0 -- m
 *     为什么要这样做？两个目的，原本的40亿个参数被均匀且离散地映射到了 0 -- m 范围之内；相同的数必定对应于同一个值
 *     然后分开使用哈希表，例如，对所有值为 0 的数使用哈希表进行统计，得到出现次数最大的数，再对值为 1 的数进行统计
 *     直到所有的数都统计完（注意，当统计完一个数之后，就将哈希表释放掉，只保留出现次数最多的数）
 *     最后从 m + 1 个出现次数最多的数中选出次数最大的
 *     极端情况下，最多也只会使用 32/m GB 的空间
 *
 *
 * 哈希表的结构
 * 经典设计：链表法 哈希表主要是存储key和value，可以假设取一个容器，大小为 m，里面存放了m个桶，编号为 0 -- m-1，桶内存放链表
 *         key 经过哈希函数计算再对m取模，映射到 0 -- m-1 ，对应到相应的桶，在桶内的链表上插入一个节点，将key与value存入节点
 *         当链表过长时，触发自动扩容，扩容为0 -- 2m-1，重新上述的计算，那么所有链表的长度大约会缩短到原来的 1/2
 *
 *         时间复杂度；
 *                  哈希函数，取模，他们的时间复杂度都是O(1)的
 *                  扩容，假设存入N个key-value，单条链表的长度不能超过2，那么扩容的次数k为 2^k=N --> k=log_2 N
 *                  所以扩容本身的时间复杂度为O(logN)
 *                  由于每次扩容都要重新计算哈希函数，所以如果有N个key-value，那么需要O(N*logN)
 *                  但是对于单次的插入，O(NlogN)/N --> O(logN) 均摊时间复杂度
 *         既然时间复杂度为O(logN)，为何说哈希表的时间复杂度为O(1)?
 *         1. 可以将单条链表的长度增大，时间复杂度逼近O(1)
 *         2. 对于虚拟机语言，使用离线扩容技术
 *            用户查询是在线的，当哈希表需要扩容时，离线扩容，不干扰在线查询，扩容完成后加载到在线状态
 * 开放地址法：当空间不够，需要扩容的时候，新开辟一个空间，原空间最后的地址指向新空间的首地址
 */

#include <iostream>
#include <unordered_map>
#include <random>

using namespace std;

/**
 * 问题 1
 * 设计一种结构，在该结构中有如下三个功能：\n
 * insert(key):将某个key加入到该结构，做到不重复加入 \n
 * delete(key):将原本在结构中的某个key移除 \n
 * getRandom():等概率随机返回结构中的任何一个key \n
 * 要求：insert,delete,getRandom时间复杂度都是O(1) \n
 * \n
 * 思路：由于需要随机返回key且时间复杂度为O(1) \n
 * 建立正反两张表，一张由key->value，一张value->key \n
 * 其中value可以当做插入的顺序，例如规定key为26个大写英文字母，\n
 * ‘A’第一个插入，则正表：'A'-> 0，反表：0 -> 'A' \n
 * 注意，该顺序是指在删除操作之前，一旦调用删除操作，那就不能简单地认为是顺序 \n
 * 对于删除操作，正表中，当删除某个key之后，将最后一个key的value修改为删除key的value，反表也同理 \n
 * 对于随机返回，只需要生成哈希表size范围内的一个随机整数，根据反表去查找就行 \n
 */
 class Question1{
 public:
     Question1(){
         // 表的初始大小
         map_size = 0;

     }

     void insert_key(char key) {
         positive_map.insert({key, map_size});
         negative_map.insert({map_size, key});
         map_size++;
     }

     void delete_key(char key) {
         int index = positive_map[key];
         char last_key = negative_map[map_size - 1];
         //删除key
         positive_map.erase(key);
         negative_map.erase(index);
         //删除map_size-1的记录
         positive_map.erase(last_key);
         negative_map.erase(map_size - 1);
         //将index为map_size-1的记录替换为已删除掉的记录
         positive_map[last_key] = index;
         negative_map[index] = last_key;
         map_size--;
     }

     char getRandom() {
         // 生成随机数引擎
         std::default_random_engine e(rd());
         // 0 - size-1 的均匀分布
         std::uniform_int_distribution<int> uniform(0, map_size - 1);
         int random_index = uniform(e);
         return negative_map[random_index];
     }

     int getIndex(char key) {
         return positive_map[key];
     }

     char getKey(char index) {
         return negative_map[index];
     }

 private:
     std::random_device rd;
     int map_size;
     std::unordered_map<char, int> positive_map;
     std::unordered_map<int, char> negative_map;
 };


 int main() {
     Question1 question1;
     question1.insert_key('A');
     question1.insert_key('B');
     question1.insert_key('C');
     question1.insert_key('D');
     question1.insert_key('E');
     question1.delete_key('B');
     std::cout << question1.getKey(1) << std::endl;
     return 0;
 }

