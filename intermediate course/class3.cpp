/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class3.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.右上角开始查找的模型(两道例题)
 * 2.打包机器问题（洗衣机问题）
 * 3.用宏观的方式解决局部问题(三道例题)
 * 4.质数因子分解问题
 * 5.如何判断一个数是否是质数（目前找到的最快的方法）
 * 6.如何找到一个数的质数因子组合（仔细想一下，为什么i++却可以保证能整除n的i肯定是质数）
 * 7.top-k问题（包括一个简单版和一个手改堆的进阶版）
 * 最近修改日期：2023-11-16
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-03
 *
 */

#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include <unordered_map>
#include <queue>

using namespace std;

/**
 * 右上角开始查找的模型（可以从其他角点开始查找，具体题目可以具体尝试）
 * 一般适用于在matrix中的查找问题
 */
/**
 * 给定一个非负整数二维matrix，该matrix的行和列都是升序，但整体无序
 * 给定一个数
 * 请在matrix中找到这个数，有返回true，没有返回false
 */
bool process1(vector<vector<int>>& matrix, int target) {
    // 从右上角，从上往下，从右往左找
    int n = matrix.size();
    int m = matrix[0].size();
    int i = 0;
    int j = m - 1;
    while (i >= 0 && i < n && j >= 0 && j < m) {
        if (matrix[i][j] == target) {
            return true;
        }
        if (matrix[i][j] > target) { // 如果比目标值大，说明同一列中，下面的行不会有target存在，所以往左走
            j--;
        } else if (matrix[i][j] < target) { // 同理，此时同一行中，左边的数不会有target存在，所以往下走
            i++;
        }
    }
    return false;
}

/**
 * 给定一个二维数组，每一行中，0肯定在1的左边
 * 每一行的0和1单独的个数不一定相同，但是总数相同
 * 求拥有最多1的行，如果有多行都有最多的1，那么都需要返回
 */
vector<int> process1_1(vector<vector<int>>& matrix) {
    // 从右上角，从上往下，从左往右找
    int n = matrix.size();
    int m = matrix[0].size();
    int i = 0;
    int j = m - 1;
    vector<int> res;
    while (i >= 0 && i < n && j >= 0 && j < m) {
        if (matrix[i][j] == 0) {
            i++;
        } else {
            if (j == 0) {
                i++;
                continue;
            }
            int count = 0;
            while (j - 1 >= 0 && matrix[i][j - 1] == 1) {
                count++;
                j--;
            }
            if (count > 0) {
                res.clear();
                res.emplace_back(i);
            } else if (count == 0) {
                res.emplace_back(i);
            }
            i++;
        }
    }

    return res;
}


/**
 * 打包机器问题（洗衣机问题）
 * 有n个打包机器从左到右依次排开，每个打包机器上的物品数量有多有少
 * 必须保证n个打包机器上的物品数量一致时才能进行打包
 * 由于物品重量大，每次只能搬运一个物品，且只能将一个物品搬运到相邻的机器上
 * 所有机器都对应一个搬运工人，每一轮中所有机器对应的工人同时进行搬运
 * 请计算能够使每个打包机器上的物品数量的相等的最小移动轮数
 * 如果不能使每个机器上的数量相等，返回-1
 */
/**
 * 思路：单独考虑一台机器左右两边，为了达到均值所需要经过该台机器的流动物品数量，记为流量
 * 所有机器的流量最大值即为答案
 */
int minTimes_PackageMachine(int n, vector<int> items) {
    if (n == 1) return 0;

    // 计算物品总数
    int sum = 0;
    for (auto item : items) {
        sum += item;
    }

    if (sum % n != 0) return -1;  // 如果不能被整除，说明不能达到每个机器上的物品相同的情况

    int average = sum / n, left_sum = 0, max_flow = 0;
    for (int i = 0; i < n; i++) {
        // 计算左右两边各自需要多少物品，正数为需要搬出的物品，负数为需要搬入的物品
        int left_need = left_sum - average * i;
        int right_need = sum - left_sum - items[i] - average * (n - i - 1);
        if (left_need < 0 && right_need < 0) {  // 如果两边都需要搬入，但是一个机器一轮只能搬出一件，所以是求和
            max_flow = max(abs(left_need) + abs(right_need), max_flow);
        } else {  // 其他情况下都是取最大值
            max_flow = max(max(abs(left_need), abs(right_need)), max_flow);
        }
    }
    return max_flow;
}


/**
 * 用宏观的方式解决局部问题
 */
/**
 * 用螺旋的方式打印矩阵（顺时针打印），例如
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 * 打印结果为：0 1 2 3 7 11 15 14 13 12 8 4 5 6 10 9
 */
void process2_1(vector<vector<int>> matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // 只顺时针打印以（a，b）和（c，d）为角点的边界
    function<void(int, int, int, int)> print_edge = [&](int a, int b, int c, int d) {
        if (a == c) {  // 如果两个角点在同一行
            for (int i = b; i <= d; i++) {
                cout << matrix[a][i] << ' ';
            }
        } else if (b == d) {  // 如果两个角点在同一列
            for (int i = a; i <= c; i++) {
                cout << matrix[i][b] << ' ';
            }
        } else {  // 两个角点不同行不同列，能构成一个矩形
            for (int i = b; i <= d; i++) {
                cout << matrix[a][i] << ' ';
            }
            for (int i = a + 1; i <= c; i++) {
                cout << matrix[i][d] << ' ';
            }
            for (int i = d - 1; i >= b; i--) {
                cout << matrix[c][i] << ' ';
            }
            for (int i = c - 1; i > a; i--) {
                cout << matrix[i][b] << ' ';
            }
        }
    };

    int a0 = 0, b0 = 0, c0 = m - 1, d0 = n - 1;
    while (a0 <= c0 && b0 <= d0) {  // 保证abcd的大小顺序
        print_edge(a0++, b0++, c0--, d0--);  // 以斜45角度从矩阵边缘向矩阵中心遍历
    }
}

/**
 * 将方阵顺时针旋转90度，例如
 * 0  1  2  3
 * 4  5  6  7
 * 8  9  10 11
 * 12 13 14 15
 * 结果为：
 * 12 8  4  0
 * 13 9  5  1
 * 14 10 6  2
 * 15 11 7  3
 */
vector<vector<int>> process2_2(vector<vector<int>> matrix) {
    int m = matrix.size();
    int n = matrix[0].size();

    function<void(int, int, int, int)> rotate_edge = [&](int a, int b, int c, int d) {
        // 左右对称
        for (int i = a; i <= c; i++) {
            int temp = matrix[i][b];
            matrix[i][b] = matrix[i][d];
            matrix[i][d] = temp;
        }
        for (int i = 1; i <= (d - b - 1) / 2; i++) {
            int temp1 = matrix[a][b + i];
            matrix[a][b + i] = matrix[a][d - i];
            matrix[a][d - i] = temp1;

            int temp2 = matrix[c][b + i];
            matrix[c][b + i] = matrix[c][d - i];
            matrix[c][d - i] = temp2;
        }

        // 负对角线对称
        for (int i = 0; i <= d - b; i++) {
            int temp = matrix[a][b + i];
            matrix[a][b + i] = matrix[c - i][d];
            matrix[c - i][d] = temp;
        }
        for (int i = 1; i <= d - b; i++) {
            int temp = matrix[a + i][b];
            matrix[a + i][b] = matrix[c][d - i];
            matrix[c][d - i] = temp;
        }
    };

    int a0 = 0, b0 = 0, c0 = m - 1, d0 = n - 1;
    while (a0 < c0 && b0 < d0) {
        rotate_edge(a0++, b0++, c0--, d0--);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << ' ';
        }
        cout << endl;
    }

    return matrix;
}

/**
 * 用zigzag的方式打印矩阵
 * 1  2  3  4  5
 * 6  7  8  9  10
 * 11 12 13 14 15
 * 16 17 18 19 20
 * 21 22 23 24 25
 * 打印结果：
 * 1 2 6 11 7 3 4 8 12 16 21 17 13 9 5 10 14 18 22 23 19 15 20 24 25
 */
void process2_3(vector<vector<int>> matrix) {
 int m = matrix.size(), n = matrix[0].size();
 function<void(int, int, int, int, int)> print = [&](int x0, int y0, int x1, int y1, int flag) {
     if (flag) {
         while (x0 < m && y0 >= 0) {
             cout << matrix[x0][y0] << ' ';
             x0++;
             y0--;
         }
     } else {
         while (y1 < n && x1 >= 0) {
             cout << matrix[x1][y1] << ' ';
             x1--;
             y1++;
         }
     }
 };

 int x0 = 0, y0 = 0, x1 = 0, y1 = 0, flag = 0;
 while(x0 != m - 1 || x1 != m - 1 || y0 != n - 1 || y1 != n - 1) {
     print(x0, y0, x1, y1, flag);
     flag ^= 1;
     if (y0 < n - 1) {
         y0++;
     } else {
         x0++;
     }

     if (x1 < m - 1) {
         x1++;
     } else {
         y1++;
     }
 }
 cout << matrix[m - 1][n - 1] << endl;
}


/**
 * 质数因子分解
 *
 * 给定两个字符串s1和s2，初始状态下s1 = s2 = "a"
 * 给定两个操作：
 * 1. s2 = s1; s1 = s1 + s1;
 * 2. s1 = s1 + s2;
 * 给定一个数 N
 * 请问，如何调用上述的两个操作，使得字符串s1的长度达到 N，且使用的操作数最小
 */
/**
 * 分析：
 *
 * 首先，我们假设 N 为质数，想一下，当目标数为质数时，该怎么调用上述的两个方法？
 * 先给答案：当目标数为质数时，只调用操作2就是最优解
 * 试想，第一次操作调用操作1和操作2是没有区别的，
 * 但是在之后的调用中，假设s1此时的长度为k(k > 1)，那么操作1之后，s1的长度为2k，s2的长度为k
 * 此时，无论你在之后的步骤中怎么调用两个操作，最终的结果必定存在因子k，因此最终的结果就肯定不可能是质数
 * 所以，当目标数为质数时，就只能调用操作2，且操作数为 N-1
 *
 * 其次，如果 N 不为质数，但是根据质数分解唯一性，任何大于1的整数都能分解成唯一一组质数的乘积
 * 假设 N 能分解为 x * y * z ，那么对于这个组合，是先将x组合出来再扩张y倍，再扩张z倍更快，还是先z再y再x？
 * 答案是，随意。
 * 假设 x y z 是最优顺序，将xy扩张z倍的代价就等于将"a"扩张z倍，就是调用z-1次操作2，同理x扩张y倍的代价为y-1，"a"扩张x倍的代价为x-1
 * 则总代价为 x+y+z-3
 * 可以发现，总代价和 x y z 的顺序是无关的
 * 所以对于不为质数的 N ，我们只需求解出其质数因子组合，并得到质数因子的个数（包括重复的），最终，将质数因子求和并减去因子个数就是答案
 */
/**
 * 判断一个数是否是质数
 * 这样写的原因详见网页：https://blog.csdn.net/sinat_26811377/article/details/96584293
 */
bool isPrime(int n) {
    if (n <= 3) return true;
    if (n % 6 != 1 && n % 6 != 5) return false;  // 只需要判断6x+1和6x-1是否是质数
    int k = int(sqrt(n));
    for (int i = 5; i <= k; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;  // 该数只能有可能被6x+1或6x-1整除
                // 举个例子，如果该数能够被6x+2整除，那么该数一定能被2整除，显然6x+1和6x-1不能被2整除，同理其他的数也能这样分析
    }
    return true;
}
/**
 * 计算一个数的质数因子和以及质数个数
 */
pair<int, int> prime_sum_and_count(int n) {
    int sum = 0, count = 0;
    for (int i = 2; i < n; i++) {
        while (n % i == 0) {
            sum += i;
            count++;
            n /= i;
        }
    }
    return {sum, count};
}
/**
 * 计算操作数
 */
int process3_1(int n) {
    if (n < 2) return 0;
    if (isPrime(n)) return n - 1;
    auto [sum, count] = prime_sum_and_count(n);
    return sum - count;
}


/**
 * top-k问题
 * 给定一个字符串型数组，求数组中出现次数最多的前k个字符串
 */
/**
 * 方法：
 * 建立词频统计表，然后排序
 *
 * 排序有两种方法，一种是对整个统计表进行排序，
 *
 * 一种是建立一个最多能容纳k个元素的小根堆，遍历一遍词频统计表，
 * 当堆大小为k时，若当前元素大于堆顶元素，则堆顶元素弹出，并将当前元素入堆
 *
 * 第一种方法复杂度相对于第二种更高一点，因为第一种需要将整个数组排序，而第二种只需要维护k个元素有序就行
 */
struct cmp {
    bool operator() (const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    }
};
vector<string> process4_1(vector<string>& strings, int k) {
    unordered_map<string, int> count;
    for (auto& s: strings) {
        count[s]++;
    }
    priority_queue<pair<string, int>, vector<pair<string, int>>, cmp> heap;
    for (auto& [key, value]: count) {
        if (heap.size() < k) {
            heap.emplace(key, value);
        } else {
            auto [item, num] = heap.top();
            if (value > num) {
                heap.pop();
                heap.emplace(key, value);
            }
        }
    }

    vector<string> res;
    while (!heap.empty()) {
        res.emplace_back(heap.top().first);
        cout << heap.top().first << endl;
        heap.pop();
    }
    return res;
}

/**
 * 进阶版top-k
 * 每个时刻可能会新加入一个字符串，每个时刻也可能会访问top-k个之前出现次数最多的字符串
 * 请设计一个算法，使得每次访问top-k的开销最小
 * 方法：手动改堆，使得已在堆中的元素可以变化，且堆能动态调整（还有一种方法是使用c++自带的map容器，底层是红黑树，也能实现上述的功能）
 * 其实这么一想，之前的用堆加速的dijkstra算法，也算是一个top-k问题(k=1)
 */
class Topk{
private:
    // 堆的节点
    struct Node{
        string s;
        int times;
        Node(string& c, int t): s(c), times(t) {}
    };
    vector<Node*> heap;  // 开辟的堆空间
    int cur_size, max_size;  // 当前堆内的元素大小和最大能容纳的元素大小
    unordered_map<string, Node*> string2Node;  // 通过字符串找到节点
    unordered_map<Node*, int> Node2index;  // 通过节点找到该节点在堆中的索引
public:
    Topk(int k) {
        heap = vector<Node*>(k);
        cur_size = 0;
        max_size = k;
    }

    void add(string s) {
        Node* node = nullptr;
        int index = -1;  // 节点在堆中的索引，-1表示节点不在堆中
        if (string2Node.find(s) == string2Node.end()) {  // 如果字符串s在之前没有出现过
            node = new Node(s, 1);  // 通过字符串新建节点
            string2Node[s] = node;  // 在哈希表中注册一下
            Node2index[node] = -1;  // 新建的节点肯定不在堆中，所以初始化为-1
        } else {  // 如果字符串s在之前出现过
            node = string2Node[s];  // 获取字符串s对应的节点
            node->times++;  // 出现次数+1
            index = Node2index[node];  // 获取该节点在堆中的索引，-1为不在堆中
        }

        if (index == -1) {  // 如果节点不在堆中
            if (cur_size == max_size && heap[0]->times < node->times) {  // 如果堆已满，且当前字符串出现的次数大于堆顶字符串出现的次数
                // 交换两者的索引，即将原堆顶元素弹出且将node放入堆顶
                Node2index[heap[0]] = -1;
                Node2index[node] = 0;
                heap[0] = node;
                // 向下更新
                heapify(0);
            } else if (cur_size < max_size) {  // 如果堆未满
                // 将节点至于堆的末尾
                Node2index[node] = cur_size;
                heap[cur_size] = node;
                // 向上调整
                heap_insert(cur_size);
                // 堆当前的大小+1
                cur_size++;
            }
        } else {  // 如果节点在堆中
            // 直接向下调整，因为time只可能增加，不可能减小
            heapify(index);
        }
    }

    void printTopk() {
        cout << "---------------------------" << endl;
        for (int i = 0; i < cur_size; i++) {
            cout << heap[i]->s << ": " <<heap[i]->times << endl;
        }
    }

    // 向上调整
    void heap_insert(int index) {
        int parent = (index - 1) / 2;
        while (heap[index]->times < heap[parent]->times) {
            // 先更新哈希表中的索引值
            Node2index[heap[index]] = parent;
            Node2index[heap[parent]] = index;
            // 将两者交换
            swap(index, parent);
            // 更新index和parent
            index = parent;
            parent = (index - 1) / 2;
        }
    }

    // 向下调整
    void heapify(int index) {
        while (2 * index + 1 < cur_size) {  // 直接判断是否拥有左孩子，因为堆是一颗完全二叉树，所以只需要判断是否有左孩子即可
            int left = 2 * index + 1;
            int right = left + 1 >= cur_size ? left : left + 1;
            // 找到左右孩子中较小的那一个
            int min_index = heap[left]->times <= heap[right]->times ? left : right;
            // 如果当前值比较小的还小，那么不用继续往下调整了，直接跳出循环
            if (heap[index]->times <= heap[min_index]->times) break;
            // 交换哈希表中的索引值
            Node2index[heap[index]] = min_index;
            Node2index[heap[min_index]] = index;
            // 将两者交换
            swap(index, min_index);
            // 更新index
            index = min_index;
        }
    }

    // 交换堆中的两个元素，索引分别为i和j
    void swap(int i, int j) {
        Node* item = heap[i];
        heap[i] = heap[j];
        heap[j] = item;
    }
};

int main() {
//    vector<vector<int>> m = {{0,1,2,3},
//                             {4,5,6,7},
//                             {8,9,10,11},
//                             {12,13,14,15}};
//    process2_3(m);
//    cout << isPrime(4);
//    vector<vector<int>> m = {{0,0,0,0,1},
//                             {0,0,1,1,1},
//                             {0,0,0,1,1},
//                             {0,0,1,1,1},
//                             {0,0,0,0,1}};
//    vector<int> res = process1_1(m);
//    for(const auto& i : res) {
//        cout <<i << endl;
//    }
//    cout << process3_1(64) << endl;
//    vector<string> ss = {"abc", "abc", "a", "a", "a", "d", "bdc", "abc", "d"};
//    process4_1(ss, 2);
    Topk item = Topk(3);
    item.add("a");
    item.add("ab");
    item.add("ab");
    item.add("ab");
    item.printTopk();
    item.add("ac");
    item.add("a");
    item.add("a");
    item.printTopk();
    item.add("c");
    item.add("ac");
    item.printTopk();
    item.add("aa");
    item.add("aa");
    item.add("aa");
    item.add("aa");
    item.printTopk();
    return 0;
}