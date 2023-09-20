/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class1.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.滑动窗口的技巧
 *      a.绳子最多能覆盖几个点
 * 2.打表
 *      a.最少使用的袋子数量
 *      b.先后手吃草问题
 * 3.预处理技巧
 *      a.方块染色问题
 *      b.寻找最大正方形
 * 4.给定一个函数，能够按照一定概率返回一些数，要求使用该函数等概率返回另一些数
 * 5.二叉树结构的可能性（暴力递归、DP）
 * 6.合法的括号字符串
 * 最近修改日期：2023-09-20
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-9-10
 *
 */


#include <iostream>
#include <vector>

using namespace std;

/******************滑动窗口的技巧：左右两个边界都不会回退的题目********************
 * 绳子最多能覆盖几个点
 * 给定一个数组，数组中的每个元素表示一个点在数轴上的位置
 * 给定一个整数，代表一根绳子的长度
 * 绳子可以在数轴上来回移动
 * 请问绳子最多能够覆盖几个点
 */
int process1(vector<int> arr, int length) {
    int left = 0;  // 绳子的左端点（滑动窗口的左边界）
    int right = 1;  // 绳子的右端点（滑动窗口的右边界）
    int max_points = 0;  // 最大能被覆盖的点的数量
    while (right <= arr.size()) {
        if (right == arr.size()) {  // 如果右端点此时已经来到了末尾的下一个元素位置，说明绳子能把之前的点都包括进去
            max_points = max(max_points, right - left);  // 由于是最后一个点的下一个元素，所以不需要+1
            right++;
        }

        if (arr[right] - arr[left] < length) {  // 如果当前左右两端点之间的距离小于绳子的长度，那么就让右端点向右移动
            right++;
        } else if (arr[right] - arr[left] == length) {  // 如果此时距离正好等于绳子的长度，那么就让左端点向右移动
            max_points = max(max_points, left - right + 1);  // 此时，右端点应该被计入点的数量，所以要+1
            left++;
        } else {  // 如果此时距离大于绳子的长度，那么就让左端点向右移动
            max_points = max(max_points, left - right);  // 此时，右端点不应该被计入点的数量，所以不要+1
            left++;
        }

    }

    return max_points;
}


/*****************************打表*****************************
 * 先用常规的方法写出解法
 * 然后自己写样例进行测试（可以借鉴对数器）
 * 根据常规解法，找出输入与输出之间的规律
 * 最终直接根据规律写一个代码
 * 这样子做，时间复杂度直接为O(1)
 *
 * 该方法适用于部分输入单一输出也单一的题目
 * 其内部蕴含一定的数学规律，但是一般根本想不到
 * 所以通过先写出常规解法的方式，寻找输入与输出之间的关系
 */
/**
 * 最少使用的袋子数量
 * 给定一个整数N，表示一共有N个苹果
 * 现在，只有两种袋子，一种袋子能装8个苹果，一种袋子能装6个苹果，一种袋子能装8个苹果
 * 要求使用袋子恰好能够装完所有的苹果
 * 求最少的袋子使用数量，如果不存在恰好装完的情况则返回-1
 *
 * 结题思路也很简单，先尽可能用容量大的袋子装
 * 再看看剩下的苹果能否用容量小的袋子恰好装掉
 * 如果可以，那么返回总的袋子数；如果不可以就将大袋子数量减一，继续尝试
 */
int process2(int N) {
    if (N % 2 == 1) {  // 如果是奇数肯定不能装
        return -1;
    }

    int num8 = N / 8;
    int res =  N % 8;

    // 这里可以进行优化的，不需要将所有的8容量袋子全部减完
    // 如果用完8袋子之后，剩余的苹果数量已经大于24，那么就没有试下去的必要了，肯定不可能恰好装完
    // 因为剩余的苹果数量大于24意味着，已经将8袋子减去3个了，但是注意，这3个8袋子完全可以使用4个6袋子装完，所以此时没有试下去的必要了
    // 举个例子，假设此时剩余3个苹果，三次过后剩余27个苹果，那么其中24个苹果可以恰好被6装掉，又剩下3个，回到了最初的情况
    // 24是6和8的最小公倍数
    while (res < 24 && num8 >= 0) {
        if (res % 6 == 0) {  // 一旦第一次恰好可装，就可以直接返回了，因为此时使用的8袋子数量最大
            return num8 + res / 6;
        }
        num8--;
        res += 8;
    }

    return -1;
}

/**
 * 使用打表，上述的代码是常规代码
 * 将0-100作为输入，此时输出的规律就能发现了
 * 这里我直接将打表后的代码写出来，可以自己使用process2看一下规律
 */
int process2_(int N) {
    if (N % 2 == 1) {  // 奇数直接返回
        return -1;
    }

    if (N < 18) {
        return N == 0 ? 0 :
               (N == 6 || N == 8) ? 1 :
               (N == 12 || N == 14 || N == 16) ? 2 : -1;
    }

    return (N - 18) / 8 + 3;
}


/**
 * 先后手吃草问题
 * 有两只羊，先后手吃草，每次吃草的数量需要是4的n次幂，每次吃草n可以由羊自己选择(1,4,16,64,...)
 * 哪只羊吃到最后一份草，哪只羊就会赢
 * 假设两只羊都绝顶聪明
 * 现在给定固定N份草
 * 问哪只羊会赢
 * 0是先手 1是后手
 */
int eat(int res) {
    // 0 1 2 3 4
    // 1 0 1 0 0
    if (res < 5) {  // 为什么0份草是后手赢？因为当前是0份草的时候，说明在上一个递归的先手把草全部吃完，上一个递归的先手就是当前递归的后手
        return (res == 0 || res == 2) ? 1 : 0;
    }

    int eat_num = 1;
    while (res -  eat_num >= 0) {
        // 因为两只羊都是绝顶聪明，所以在自己的回合都希望自己赢，也就是说都希望在当前回合的先手赢
        // 所以需要判断所有吃草的可能，一旦遇到一种自己能够赢的，就直接返回
        // 下面为什么判断等于1，因为当前回合的先手到下一回合就变成后手了，res-eat_num是下一回合的吃草数
        if (eat(res - eat_num) == 1) {
            return 0;
        }
        if (eat_num > res / 4) {  // 防止eat_num*4之后会溢出
            break;
        }
        eat_num = eat_num * 4;
    }

    return 1;
}

/**
 * 上述常规解法写完之后，那就打印0-50份草的可能性，然后打表
 */
int eat_(int N) {
    return (N % 5 == 0 || N % 5 == 2) ? 1 : 0;
}


/***********************预处理技巧*************************
 * 如果解法出来之后，发现解法中有频繁的查询或者验证的操作，且该操作时间复杂度不是O(1)
 * 那我们可以想办法把这个操作从循环中提出来，提前存好所有的值
 * 这样能够降低整体的时间复杂度
 */
/**
 * 方块染色问题
 * 牛牛有一排方块，每个方块的颜色要不是红色要不是绿色
 * 牛牛需要对方块进行染色
 * 要求，每一个红色的方块都要比任意一个绿色方块离左边更近
 * 现在给出一个字符串"RGRGRG"，表示一排方块的染色情况
 * 问牛牛最少需要染几个方块能完成上述的要求
 *
 * 普通的解法是，尝试将左边n个方块染为红色，右边m个方块染为绿色，m+n=string.length()
 * 遍历n=0~string.length()，找需要染色的最小值
 */
//统计left到right区间上，颜色为color的方块数
int count_num(string s, int left, int right, char color) {
    int count = 0;
    while (left <= right) {
        if (s[left] == color) {
            count++;
        }
        left++;
    }
    return count;
}
int process3(string s) {
    int res = INT_MAX;
    int n = 0;
    while (n <= s.length()) {
        int left = count_num(s, 0, n - 1, 'G');  // 统计左边有多少绿色方块
        int right = count_num(s, n, s.length() - 1, 'R');  // 统计右边有多少红色方块
        res = min(res, left + right);
        n++;
    }
    return res;
}

/**
 * 上述代码是O(n^2)的时间复杂度，因为每一次统计都等于遍历数组
 * 那么我们可以先遍历数组，记录下所有所需的东西，这样子时间复杂度就降为O(n)级别了
 */
int process3_1(string s) {
    vector<int> green(s.length());  // 统计0~i范围上有多少个绿色方块
    vector<int> red(s.length());  //  统计i~s.length()-1上有多少个红色方块
    int i = 0;
    int count = 0;
    while (i < s.length()) {
        if (s[i] == 'G') {
            count++;
        }
        green[i] = count;
        i++;
    }
    i--;
    count = 0;
    while (i >= 0) {
        if (s[i] == 'R') {
            count++;
        }
        red[i] = count;
        i--;
    }

    int res = INT_MAX;
    int n = 0;
    while (n <= s.length()) {
        if (n == 0) {
            res = min(res, red[n]);
        } else if (n == s.length()) {
            res = min(res, green[n - 1]);
        } else {
            res = min(res, green[n - 1] + red[n]);
        }
        n++;
    }
    return res;

}

/**
 * 其实还有一种优化方式，无需预处理
 * 可以发现，每次n+1的过程中，只有一个字符的左右区间归属发生变化
 * 所以如果该字符为R，那么左区间green数量不变，右区间上red数量-1
 * 如果该字符为G，那么左区间green数量+1，右区间red数量不变
 * 这样我们只需统计一次右区间上的red数量即可，代码如下
 */
int process3_2(string s) {
    int left = 0;
    int right = count_num(s, 0, s.length() - 1, 'R');
    int n = 0;
    int res = left + right;
    while (n < s.length()) {
        if (s[n] == 'R') {
            right--;
        } else if (s[n] == 'G') {
            left++;
        }
        res = min(res, left + right);
        n++;
    }
    return res;
}


/**
 * 寻找最大的正方形
 * 给定一个二维数组，里面只有0和1
 * 现在需要寻找一个正方形
 * 正方形的边长全部都是由1构成的
 * 请找到最大边长的正方形
 *
 * 一个二维数组中一共有多少个长方形？
 * 从二维数组中随意找一个点，有n^2的可能性，再找一个点，也有n^2的可能性
 * 这两个点作为长方形的对角两个点能够构成唯一的长方形，故长方形的数量应该是n^4级别的（实际上会有部分重复，但是不影响数量级）
 *
 * 一个二维数组中由多少个正方形？
 * 同样，随意找一个点，有n^2的可能性，但是另外一个点不是随意找了，由于是正方形，故同一行的两个点就能确定一个正方形
 * 所以第二个点的可能性是n，那么正方形的数量就是n^3
 *
 * 确定正方形之后，需要校验是否边都是由1构成的
 * 校验一条边的最坏情况是n，由于四条边是分开校验的，即不是嵌套关系，所以数量级还是n
 *
 * 则总的时间复杂度是O(n^4)
 */
// 检查正方形是否合法
bool check(vector<vector<int>>& matrix, int x, int y, int side_length) {
    for (int i = x; i < x + side_length; i++) {
        if (matrix[i][y] != 1) return false;
    }
    for (int i = y; i < y + side_length; i++) {
        if (matrix[x][i] != 1) return false;
    }
    for (int i = x; i < x + side_length; i++) {
        if (matrix[i][y + side_length] != 1) return false;
    }
    for (int i = y; i < y + side_length; i++) {
        if (matrix[x + side_length][i] != 1) return false;
    }
    return true;
}
int process4(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    int res = 0;
    // 找正方形的第一个顶点，从上往下，从左往右
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
            // 找正方形的第二个顶点，等价于找边长，从当前点开始往右寻找
            for (int side_length = 0; side_length < min(n - x, m - y); side_length++) {
                // 检查正方形是否合法
                if (check(matrix, x, y, side_length)) {
                    // 取最大
                    res = max(res, side_length);
                }
            }
        }
    }
    // 正方形的边长需要算上顶点自己
    return res + 1;
}

/**
 * 上述用法中，check函数的时间复杂度是O(n)
 * 嵌套在三层for循环内，导致整体复杂度又上了一个数量级
 * 所以可以使用预处理
 * 先将每个点的信息存下来
 * 之后直接查询即可
 * 总体时间复杂度为O(n^3)
 */
int process4_(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int m = matrix[0].size();
    // 为什么要计算右侧和下侧，因为我们的正方形构建在第一个顶点的右下方
    // 记录所有点右侧，有多少个连续的1，只计算包括该点的那一串连续的1的个数
    vector<vector<int>> right(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = m - 1; j >= 0; j--) {
            if (j == m - 1) {
                right[i][j] = matrix[i][j] == 1 ? 1 : 0;
            } else {
                int is_one = matrix[i][j] == 1 ? 1 : 0;
                right[i][j] = is_one + right[i][j + 1];
            }
        }
    }

    // 记录所有点下侧，有多少个连续的1，只计算包括该点的那一串连续的1的个数
    vector<vector<int>> down(n, vector<int>(m));
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            if (i == 0) {
                down[i][j] = matrix[i][j] == 1 ? 1 : 0;
            } else {
                int is_one = matrix[i][j] == 1 ? 1 : 0;
                down[i][j] = is_one + down[i - 1][j];
            }
        }
    }

    int res = 0;
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
            // 找正方形的第二个顶点，等价于找边长，从当前点开始往右寻找
            for (int side_length = 0; side_length < min(n - x, m - y); side_length++) {
                // 检查正方形是否合法
                if (right[x][y] > side_length &&
                right[x][y + side_length] > side_length &&
                down[x][y] > side_length &&
                down[x + side_length][y] > side_length) {
                    res = max(res, side_length);
                }
            }
        }
    }

    return res + 1;
}


/**
 * 给定一个函数，能够按照一定概率返回一些数，要求使用该函数等概率返回另一些数
 * 1. 给定函数f1，能够等概率返回1~5，请使用该函数加工出一个等概率返回1~7的函数g1
 * 2. 给定函数f2，能够等概率返回a~b，请使用该函数加工出一个等概率返回c~d的函数g2
 * 3. 给定函数f3，能够按照概率p返回0,1-p返回1，请使用该函数加工出一个等概率返回0,1的函数g3
 *
 * 三者的思路都是一样的，都是使用二进制去拼凑结果
 * 对于前两问，我们第一步需要使用函数f等概率返回0,1，再使用0,1作为二进制的位来构成输出
 * 对于第三问，只需要两位二进制01和10，两者的概率都是p-p^2，所以可以将两者映射为0,1等概率返回
 * ******* 其实第三问的解法可能有一点问题，如果p和1-p相差过大，可能需要较长的时间才能等概率生成0和1 *******
 */
// 下面两个函数分别对应1和3两个问题
int f1();
int f3();

// 等概率返回0,1
int trans() {
    // 1和2返回0  3和4返回1
    // 如果等概率出现5则重新生成数字
    int x;
    do {
        x = f1();
    } while (x == 5);
    return (x == 1 || x == 2) ? 0 : 1;
}

int g1() {
    // 1~7 等价于 (0~6)+1
    // 0~6我们最少需要三位二进制位
    // 如果三位二进制构成的数大于6，那么就重新生成
    int res;
    do {
        res = (trans() << 2) + (trans() << 1) + trans();
    } while (res > 6);
    return res + 1;
}
/**
 * 第二问是第一问的一般化表达，这里也就使用笔记的方式进行解答
 * 1. 首先我们需要使用f2生成一个等概率返回0,1的函数trans()
 *      简而言之就是将f2能够生成的数一半一半分开，其中一半映射成0，另一半映射成1，
 *      如果f2能够生成的数的个数是奇数，那就随意抛弃一个数（如果生成该数就重新生成）
 * 2. 然后将目标生成的区间平移到左端点为0，记住平移的距离s
 * 3. 看看平移后的区间的最大值M，使用最少的二进制位表示该数，此时的位数就是我们需要调用trans()的次数
 * 4. 假设最少能用k个二进制位表示，那么就调用trans()k次，将每次的结果依次填入各个二进制位上
 * 5. 该k位二进制数如果大于M，那就重新生成，反之则返回 M+s
 */

int g3() {
    // 很简单，我只要01和10，因为这俩概率相等
    // 如果生成00和11，那么就重新生成
    // 最终将01 映射成 0 ，将10 映射成 1
    int res;
    do {
        res = (f3() << 1) + f3();
    } while (res == 0 || res == 3);
    return res == 1 ? 0 : 1;
}


/**
 * 二叉树结构的可能性
 * 给定一个非负整数，表示二叉树的节点个数，求能形成多少种不同的二叉树结构
 */
int process5(int n) {
    if (n == 1 || n == 0) {
        return 1;
    }
    if (n == 2) {
        return 2;
    }

    int res = 0;
    for (int node = 0; node <= n - 1; node++) {
        int left = process5(node);
        int right = process5(n - 1 - node);
        res += left * right;
    }
    return res;
}

int process5_dp(int N) {
    vector<int> dp(N + 1, 0);
    dp[0] = 1;
    dp[1] = 1;
    dp[2] = 2;
    for (int n = 3; n <= N; n++) {
        for (int node = 0; node <= n - 1; node++) {
            dp[n] += dp[node] * dp[n - 1 - node];
        }
    }
    return dp[N];
}


/**
 * 合法的括号字符串
 * 如果一个字符串中的每个右括号都能在其左边找到与之配对的左括号，每个左括号都能找到与之配对的右括号，则该字符串是合法的
 * 例如"()()()"合法  "(()())"合法   "(()"不合法
 * 牛牛拿到一个字符串
 * 请问牛牛至少需要添加几个括号能够使其变为合法的字符串
 */
int process6(string s) {
    int n = s.length();
    int res = 0, count = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == '(') {
            count++;
        } else if (s[i] == ')') {
            if (count == 0) {
                res++;
            } else {
                count--;
            }
        }
    }
    res += count;
    return res;
}


int main() {
    for (int i = 0; i < 10; ++i) {
        if (process5(i) != process5_dp(i)) {
            cout << "ops" << endl;
        }
    }
//    vector<vector<int>> matrix = {{1, 1, 0, 1},
//                                  {1, 1, 1, 1},
//                                  {0, 1, 0, 1},
//                                  {0, 1, 0, 1}};
//    cout << process4_(matrix);
    return 0;
}
