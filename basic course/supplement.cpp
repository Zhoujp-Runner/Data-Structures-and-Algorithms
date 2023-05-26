/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hanoi.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * 该文件写的是左程云算法视频的补充视频中除了用堆加速dijkstra和hanoi之外的算法, 包括：
 * 1. 求给定字符串的所有子序列
 * 2. 求给定字符的所有全排列
 * 3. 两个人分别先后取一个数组中最左或最右的数，返回两人中拿到数字之和的最大值，两人都会使对方拿到的数尽可能小
 * 4. 逆序一个栈
 * 5. 返回一个字符串对应字母的所有可能性
 * 6. 背包问题
 * 7. N皇后问题(自己写了一遍，和左程云写的有一点不同)
 * 最近修改日期：2023-05-25
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-01-03
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stack>
#include <new>
#include <algorithm>

using namespace std;

void question1(string str); // 求给定字符串的所有子序列（子集）
vector<string> process1(string res, int i, int len, bool choose); // 问题1的递归函数
void question2(string str); // 求给定字符的所有全排列
void process2(char str_arr[], int j, int len, vector<string> & res); // 问题2的递归函数
void swap_char(char arr[], int i, int j); // 交换字符数组中的两个元素
int question3(int arr[], int len); // 两个人分别先后取一个数组中最左或最右的数，返回两人中拿到数字之和的最大值，两人都会使对方拿到的数尽可能小
int first(int arr[], int left, int right); // 先手函数
int second(int arr[], int left, int right); //后手函数
void reverse(stack<int> &s); // 逆序一个栈
int get_bottom_element(stack<int> &s); //弹出栈底元素，不改变栈序
void question5(string str); // 返回一个字符串对应字母的所有可能性
int process5(string str, int i); // 问题5的递归函数
void question6_1(float weight[], float value[], int len, float max_weight); // 背包问题 解法1
void question6_2(float weight[], float value[], int len, float max_weight); // 背包问题 解法2
float process6_2(float weight[], float value[], int i, int len, float current_weight, float max_weight, bool choose); // 背包问题解法2的递归函数
void question7(int N); // N皇后问题
int process7(int arr[], int N, int i, int j); // 问题7的递归函数
void question7_bit(int N); // N皇后问题用位运算加速
int process7_bit(int limit, int colLimit, int leftLimit, int rightLimit); // N皇后位运算加速的递归函数

// 问题1的递归函数
vector<string> process1(string res, int i, int len, bool choose) {
    if (i >= len) {
        if (choose == true) {
            res = res + "1";
        } else {
            res = res + "0";
        }
        vector<string> re;
        re.emplace_back(res);
        return re;
    }

    if (choose == true) {
        res = res + "1";
    } else {
        res = res + "0";
    }

    vector<string> re0 = process1(res, i + 1, len, false);
    vector<string> re1 = process1(res, i + 1, len, true);
    re0.insert(re0.end(), re1.begin(), re1.end());
    return re0;
}

// 求给定字符串的所有子序列（子集）
void question1(string str) {
    int len = strlen(str.c_str());
    vector<string> re1 = process1("", 1, len, true);
    vector<string> re0 = process1("", 1, len, false);
    re1.insert(re1.end(), re0.begin(), re0.end());

    char * str_c = new char[len + 1]; // 需要考虑到终止符
    strcpy(str_c, str.c_str());
    for (auto & i : re1) {
        char * choice = new char[len + 1];  // 考虑终止符
        strcpy(choice, i.c_str());
        string out;
        for (int j = 0; j < len; j++) {
            if (choice[j] - '0') {
                out += str_c[j];
            }
        }
        if (out == "") {
            cout<<"none"<<endl;
        } else {
            cout<<out<<endl;
        }
    }
}

// 求给定字符的所有全排列
void question2(string str) {
    int len = strlen(str.c_str());
    cout << len << endl;
    char* str_arr = new char [len + 1]; // 算上终止符
    strcpy(str_arr, str.c_str());
    vector<string> res;
    process2(str_arr, 0, len, res);
//    for (auto i : res) {
//        cout<< i << endl;
//    }
}

// 交换字符数组中的两个元素
void swap_char(char arr[], int i, int j) {
    char temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// 问题2的递归函数
void process2(char str_arr[], int j, int len, vector<string> & res) {
    if (j == len) {
        string str(str_arr);
        cout << str << endl;
        res.emplace_back(str); // 将结果存到res中
        return;
    }

    bool visit[26] = {0}; // 设置标志数组，防止重复的字符，分支限界
    for(int i = j; i < len; i++){
        if (!visit[str_arr[i] - 'a']){
            visit[str_arr[i] - 'a'] = true;
            swap_char(str_arr, i, j); // 将任意一个位置的字符换到j位置上，当做是一次排列
            process2(str_arr, j + 1, len, res);
            swap_char(str_arr, i, j); // 将字符串还原，使得下一次选择能够正常进行
        }
    }
}

/**
 * @brief 两个人分别先后取一个数组中最左或最右的数，返回两人中拿到数字之和的最大值，两人都会使对方拿到的数尽可能小
 * @param arr 从arr中取数
 * @param len   arr的长度
 */
int question3(int arr[], int len)
{
    int first_max_score = first(arr, 0, len - 1);
    int second_max_score = second(arr, 0, len - 1);
    cout << first_max_score << endl;
    cout << second_max_score << endl;
    return (first_max_score > second_max_score) ? first_max_score : second_max_score;
}

/**
 * @brief 先手函数 即先拿数字
 * @param arr   从arr中取数
 * @param left  取数范围的左边界
 * @param right 取数范围的右边界
 */
int first(int arr[], int left, int right)
{
    if (left == right)
    {
        return arr[left];
    }

    int left_score = arr[left] + second(arr, left + 1, right);
    int right_score = arr[right] + second(arr, left, right - 1);
    // 先手函数，尽可能使分数尽可能大
    return (left_score > right_score) ? left_score : right_score;
}

/**
 * @brief 后手函数，等先手函数拿完数字之后，才能拿数字
 * @param arr   从arr中取数字
 * @param left  左边界
 * @param right 右边界
 */
int second(int arr[], int left, int right)
{
    if (left == right)
    {
        return 0;
    }

    int left_score = first(arr, left + 1, right);
    int right_score = first(arr, left, right - 1);
    // 由于是后手函数，所以返回的值是最小值，因为先拿的人肯定会使得后拿的人的分数尽可能小
    return (left_score < right_score) ? left_score : right_score;
}

/**
 * @brief 使一个站逆序 不能调用额外的数据结构 思路是使用递归，借用系统栈
 * @param s 需要逆序的栈
 */
void reverse(stack<int> &s)
{
    if (s.empty())
        return;
    else
    {
        int i = get_bottom_element(s); // 取栈底元素
        reverse(s);
        s.push(i); // 当栈为空时，依次将元素压入栈
    }
}

/**
 * @brief 弹出栈底元素，并不改变栈序
 * @param s 目标栈
 *
 * @return 栈底元素
 */
int get_bottom_element(stack<int> &s)
{
    int i = s.top();  // 每次取栈顶元素 这里是为了将栈顶元素保存在系统栈中
    s.pop();
    if (s.empty())
    {
        return i;
    } else
    {
        int bottom = get_bottom_element(s);
        s.push(i);
        return bottom;
    }
}

/**
 * @brief 规定1和A对应，2和B对应，3和C对应，...
 * 那么一个数字字符串最多能转化成多少种字母序列
 * @param str   数字字符串
 */
void question5(string str)
{
    int result = process5(str, 0);
    cout << result << endl;
}

/**
 * @brief 问题5的递归函数
 * @param str   数字字符串
 * @param i     当前处于字符串的第几个字符处
 * @return 一共能有几种可能性
 */
int process5(string str, int i)
{
    if (str[i] == '0') return 0; // 0无法对应任何字符
    if (i == str.length()) return 1; // 整个字符已经选择完毕
    int res;
    if (str[i] == '1')
    {
        res = process5(str, i+1);
        if (i + 1 < str.length())
        {
            res += process5(str, i+2);
        }
    } else if (str[i] == '2')
    {
        res = process5(str, i+1);
        if (i + 1 < str.length() && str[i+1] <= '6' && str[i+1] >= '0')
        {
            res += process5(str, i+2);
        }
    } else
    {
        res = process5(str, i+1);
    }

    return res;
}

/**
 * @brief 背包问题(解法1)，有一堆物品，每个物品有两个属性，重量和价值，有一个背包，其所能承载的最大重量已定，求能得到的最大价值是多少
 * @param weight        重量数组，与价值数组对应
 * @param value         价值数组，与重量数组对应
 * @param len           数组的长度，即为物品的个数
 * @param max_weight    背包所能承载的最大重量
 */
void question6_1(float weight[], float value[], int len, float max_weight) {
    auto* unit_value = new float[len];
    for (int i = 0; i < len; i++) {
        unit_value[i] = value[i] / weight[i];
    }
    float current_weight = 0;
    float current_value = 0;
    while (true) {
        //下面找最大值索引应该另外写一个函数
        int max_index;
        float max = 0;
        float val = 0;
        for (int i = 0; i < len; i++) {
            if (unit_value[i] >= max && value[i] > val) {
                max = unit_value[i];
                max_index = i;
                val = value[i];
            }
        }

        current_weight += weight[max_index];
        if (current_weight > max_weight) break;
        current_value += value[max_index];
        unit_value[max_index] = 0;
    }
    cout << current_value << endl;
}

/**
 * @brief 背包问题(解法2)，有一堆物品，每个物品有两个属性，重量和价值，有一个背包，其所能承载的最大重量已定，求能得到的最大价值是多少
 * @param weight        重量数组，与价值数组对应
 * @param value         价值数组，与重量数组对应
 * @param len           数组的长度，即为物品的个数
 * @param max_weight    背包所能承载的最大重量
 */
void question6_2(float weight[], float value[], int len, float max_weight) {
    float val1 = process6_2(weight, value, 0, len, 0, max_weight, true);
    float val2 = process6_2(weight, value, 0, len, 0, max_weight, false);
    cout << val1 << endl << val2 << endl;
}

/**
 * @brief 背包问题（解法2）的递归函数
 * @param weight            重量数组，与价值数组对应
 * @param value             价值数组，与重量数组对应
 * @param i                 第i个物品
 * @param len               数组的长度，即为物品的个数
 * @param current_weight    当前累计重量
 * @param max_weight        背包所能承载的最大重量
 * @param choose            是否选择当前物品
 */
float process6_2(float weight[], float value[], int i, int len, float current_weight, float max_weight, bool choose) {
    if (i == len) return 0;  // 注意边界条件
    float val = 0;
    if (choose) {
        current_weight += weight[i];
        val = value[i];
    }
    if (current_weight > max_weight) {
        return 0;
    }
    float val1 = val + process6_2(weight, value, i+1, len, current_weight, max_weight, true);
    float val2 = val + process6_2(weight, value, i+1, len, current_weight, max_weight, false);

    return (val1 > val2) ? val1 : val2;
}

/**
 * @brief N皇后问题
 * @param N N-皇后
 */
void question7(int N) {
    int* arr = new int[N];
    int res = 0;
    for (int j = 0; j < N; j++) {
        res += process7(arr, N, 0, j);
    }
    cout << res << endl;
}

/**
 * @brief N皇后问题的递归函数
 * @param arr   用于存储皇后摆放位置
 * @param i     第i行
 * @param j     第j列
 */
int process7(int arr[], int N, int i, int j) {
    for (int k = 0; k < i; ++k) {
        if (arr[k] == j || arr[k] + i - k == j || arr[k] - i + k == j) {
            return 0;
        }
    }
    if (i == N - 1) return 1;  // 注意，当最后一行可以放置皇后时，应当直接返回，而不是继续进行第i+1行

    int res = 0;
    arr[i] = j;
    for (int v = 0; v < N; ++v) {
        res += process7(arr, N, i+1, v);
    }
    return res;
}

/**
 * @brief N皇后问题用位运算加速
 * @param N N-皇后问题
 * @note 请不要超过32皇后问题
 */
void question7_bit(int N) {
    int limit = N == 32 ? -1 : (1 << N) - 1;
    int res = process7_bit(limit, 0, 0, 0);
    cout << res << endl;
}

/**
 * @brief N皇后位运算加速的递归函数
 * @details 所有的限制就是列限制和左斜线和右斜线相与 \n
 * 下一行的列限制是当前行的列限制与上当前选择放置皇后的位置 \n
 * 下一行的左斜线限制是当前行的左斜线限制与上当前选择放置皇后的位置再左移一位 \n
 * 下一行的右斜线限制是当前行的右斜线限制与上当前选择放置皇后的位置再右移一位 \n
 * 所有限制的1表示不能放置皇后
 * @param limit         总的列限制
 * @param colLimit      当前的列限制
 * @param leftLimit     当前的左斜线限制
 * @param rightLimit    当前的右斜线限制
 */
int process7_bit(int limit, int colLimit, int leftLimit, int rightLimit) {
    if (limit == colLimit) return 1; // 如果每一列上都放置了一个皇后

    int current_possible_position = limit & ( ~ (colLimit | leftLimit | rightLimit)); // 当前行的哪些位置能够放置皇后 1为能放置

    int res = 0;
    while (current_possible_position != 0) {
        int rightOne = current_possible_position & (~ current_possible_position + 1); // 取出最右边的1
        current_possible_position -= rightOne; //取出1后，原来的数需要更新

        res += process7_bit(limit,
                            colLimit | rightOne,
                            (leftLimit | rightOne) << 1,
                            (rightLimit | rightOne) >> 1);
    }
    return res;
}

int main() {
//    question1("abc");
//    question2("abc");
//    int a[3] = { 2, 100, 4};
//    question3(a, 3);
//    stack<int> s;
//    s.push(1);
//    s.push(2);
//    s.push(3);
//    reverse(s);
//    while (!s.empty())
//    {
//        int i = s.top();
//        s.pop();
//        cout<<i<<endl;
//    }
//    string s = "111";
//    question5(s);
//    float a[3] = {1, 2, 3};
//    float b[3] = {1, 1, 3};
//    question6_2(a, b, 3, 3);
    question7_bit(8);
    return 0;
}