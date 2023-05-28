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
 * 2. 布隆过滤器，bit map的实现
 * 最近修改日期：2023-05-27
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-05-26
 *
 */

/**********笔记***********
 * ************哈希函数的性质************ *
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
 * ************使用哈希函数的例子************ *
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
 * **************哈希表的结构************* *
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
 *
 * **************布隆过滤器************** *
 * 布隆过滤器是用来判断一个key是否在一个set中
 * 如果用哈希表来做，一旦set过大，那么哈希表占用的内存会过大，使用布隆过滤器能使空间大大减小，布隆过滤器使用bit map来实现
 * 但是布隆过滤器存在失误率，尽管失误率很低，但是还是存在的
 * 布隆过滤器的失误指的是会将set外的key判断为在set内，但是不会将set内的key判断为在set外
 * 布隆过滤器的做法是，将一个key分别经过k个哈希函数之后，得到k个值，将这k个值都对m取模，使得他们的取值范围在 0 -- (m-1)
 * 然后，将这k个值在bit map中对应的位置全部置为1，接着遍历下一个key，直到将set遍历完
 * 查询的时候，同样的操作，将需要查询的key经过哈希函数与取模，得到k个值，然后取bit map中查找，如果都为1，说明在set中，反之，不在
 *
 * 显然布隆过滤器的失误率与哈希函数的个数k，以及bit map的大小m有关
 * 对于k，不是说k的数量越大失误率越低，k与失误率的关系曲线类似于凸函数，存在一个极小值点
 * 对于m，理论上来说，m越大，失误率越低。但是需要注意的是，m越大，耗费的空间也越大
 *
 * 下面是用于计算k与m的公式
 * 注意：布隆过滤器应用的条件必须是允许失误率，且布隆过滤器与单个样本的大小无关
 * N=样本量 P=失误率
 * ..........................
 * m = - (N * lnP) / (ln2)^2
 * k = ln2 * m / N
 * ..........................
 * 通过上述两个公式可得到理论上的m与k，一般向上取整
 * 且由上述两个公式可知，布隆过滤器只与样本数量以及要求的失误率有关
 * 那么假如已经知道了m与k，怎么计算真实失误率，由下面公式可得
 * P_真 = 1 - e^(- (n * k_真) / m_真)
 *
 * *****************一致性哈希**************** *
 * `这里应该是和Redis cluster里的东西有关，就是集群分布`
 * 服务器分为逻辑服务器与数据服务器，其中逻辑服务器中的各个实体看作是等价的
 * 也就是说，逻辑服务器之间是没有专属实体的，而数据服务器是有专属数据的
 * 那么就有一个问题，服务器负载均衡的问题
 * 例如一共有三个数据服务器a,b,c，三者存放的数据量相当
 * 但是a服务器中的数据访问频次最大，也就是说a服务器负载最大，当负载超出界限，就会出现问题
 * 那么如何分配数据呢？以使其三个服务器的负载大致均衡，这就用到了一致性哈希
 *
 * 还是用上面的例子来说明
 * 首先，定义一个哈希函数F的值域为 0 -- y，将其看为环状结构，首尾相连，也就是y的下一个数为0
 * 分配的逻辑：
 *      a，b，c三个数据服务器，先将其服务器名字（也可以是其他的key）作为key，经过F计算，得到三个不同的值fa， fb， fc
 *      这就看做了环上有三个节点，由此，得到了三个区间(fa, fb),(fb, fc),(fc, fa)
 *      接着将数据的key也用F进行计算，得到了一个值f，如果f位于(fa, fb)这个区间，则该数据由b储存
 * 如果新增一个服务器：
 *      假设新增一个d服务器，那么由F算出来的值就是fd
 *      如果fd位于(fb, fc)区间，那么就将该区间内(fb, fd)的数据转移到d服务器上，其他服务器不用改变
 *
 * 存在几个问题：
 *      1. 数据（注意是数据而不是服务器）的key的选择会导致负载不均衡，因为同一个key的数据会被分配到同一个服务器中
 *      2. 如果初始服务器数量过少，那么不能保证三个区间是均分的，也就不能保证分配是均匀的
 *      3. 就算初始的时候能够均分值域，但是当插入一个服务器后，原服务器中有一个必定会少去一部分数据，而其他服务器不变，数据分配就不均衡了
 * 解决方式：
 *      1. 数据的key应当选择种类数量尽可能多的，各种类间不会十分不平衡的，正例：人名；反例：国家名。如果选择国家名，那么中美将会占据大部分
 *      2. 使用虚拟节点技术解决上述的2和3问题，类似于Redis cluster中的slot
 *         原先是根据服务器的特点选择一个key，计算一个f当做节点值
 *         现在给每个服务器分配N个key（假设N个字符串），计算N个f当做节点值
 *         参照上面的例子，则原先的值域被划分为 3 * N 个区间，每个服务器对应N个区间
 *         数据分配和服务器插入的规则不变
 *         只要保证N的数量足够，那么数据的分配就是均匀的
 *         对于服务器插入来说，新插入的服务器的N个区间大致会均分到原先三个服务器中
 *         就等于分别从三个服务器中选择数据作为新服务器的数据，也不会存在不平衡的问题
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


/**
 * @brief bit map的实现
 * @details 使用int数组实现bit map，初始化所有位上都为0\n
 * search函数用于查询第index位上的状态
 * set_one用于使index位上的二进制数变为1
 * set_zero用于使index位上的二进制数变为0
 */
class BitMap{
public:
    int search(int index) {
        int number_index = index / 32; // 由于一个整型占4个字节即32bit，所以该变量能够定位到index位于第几个整数内
        int bit_shift = index % 32; // 在该整型的第几位上
        int flag = (arr[number_index] >> bit_shift) & 1;
        return flag;
    }

    void set_one(int index) {
        int number_index =index / 32;
        int bit_shift = index % 32;
        arr[number_index] = arr[number_index] | (1 << bit_shift);
    }

    void set_zero(int index) {
        int number_index =index / 32;
        int bit_shift = index % 32;
        arr[number_index] = arr[number_index] & (~ (1 << bit_shift));
    }

private:
    int arr[100] = {0};
};


int main() {
//    Question1 question1;
//    question1.insert_key('A');
//    question1.insert_key('B');
//    question1.insert_key('C');
//    question1.insert_key('D');
//    question1.insert_key('E');
//    question1.delete_key('B');
//    std::cout << question1.getKey(1) << std::endl;
    BitMap bit_map;
    bit_map.set_one(100);
    cout << bit_map.search(101);
    return 0;
}

