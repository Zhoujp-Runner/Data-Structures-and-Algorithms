/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class5.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.相邻数之积为4的倍数的问题
 * 2.快速幂算法（数的快速幂和矩阵的快速幂）
 * 3.斐波那契类似问题的优化（使用矩阵的快速幂进行优化，加了三道示例）
 * 4.背包问题
 * 5.每个人最优的工作岗位
 * 6.将一个string转换为int
 * 最近修改日期：2023-11-22
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-11-18
 *
 */
#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <map>

using namespace std;


/**
 * 相邻数之积为4的倍数的问题
 * 给定一个数组arr，如果通过调整可以做到arr中任意两个相邻的数字相乘是4的倍数，返回true，否则返回false
 */
/**
 * 方法：统计奇数、只含有一个2因子的偶数以及含有两个及两个以上2因子的偶数
 */
bool nearbyProduct(vector<int>& nums) {
    vector<int> count(3, 0);
    for (int num: nums) {
        if (num % 2 != 0) {  // 奇数
            count[0]++;
        } else if (num % 4 == 0) {  // 含有两个及两个以上2因子的数
            count[2]++;
        } else {  // 只含有一个2因子的数
            count[1]++;
        }
    }

    if (count[1] == 0) {  // 当不存在只含有一个2因子的数时，摆放形式为02020202...或者2020202...（0代表奇数，2代表含有两个及两个以上2因子的数）
        if (count[0] == 1) return count[2] >= 1;  // 如果只含有一个奇数，那么有两个及两个以上2因子的数只需一个就够
        return count[2] >= count[0] - 1;  // 如果含有两个及两个以上的奇数，那么有两个及两个以上2因子的数的个数最少可以是奇数个数-1
    }
    // 当只存在一个只含有2因子的数时，那么4因子的数至少要有1个，且4因子的个数满足第三种情况的讨论
    // （其实这种情况不用考虑，因为数组长度肯定大于等于2，如果count[2]没有，那么必不满足第三种情况的条件）
    if (count[1] == 1) return (count[2] >= 1) && (count[2] >= count[0]);
    // 否则，摆放形式为111...111202020...(0代表奇数，1代表只含有一个2因子的数，2代表含有两个及两个以上2因子的数)
    // 此时，有两个及两个以上2因子的数的个数最少可以是奇数个数
    return count[2] >= count[0];
}


/**
 * 快速幂算法
 * 将一个幂运算的时间复杂度从O(N)优化到O(logN)
 * 核心是利用已有的运算结果，假设要求16次方，我们不再是一次循环乘一次初值，而是考虑16次方可以由两个8次方得到
 * 进一步，将幂用二进制展开，也就是说x^{N}的N可以用二进制表示，
 * 那么假设可以将 N 表示为 abcd 四位二进制，那么x^{N}就可以划为 x^{d*1} * x^{c*2} * x^{b*4} * x^{a*8}
 * 可以看到x平方可以得到x^{2}，x^{2}的平方可以得到x^{4}，以此类推
 * 那么我们可以在每次循环不断使一个数更新为自己的平方，然后查看当前位数的二进制上是0还是1，如果是1，就将结果乘上该数，如果是0结果不变
 * **注意：如果数很大的话还需要取模运算，博客里要提一下，这里为了展示清晰的逻辑就不改了**
 * 博客地址：https://blog.csdn.net/weixin_45610907/article/details/134547741?spm=1001.2014.3001.5502
 */
int fast_pow(int x, int y) {
    int res = 1;  // 结果初始化为1，如果是矩阵的快速幂，那就初始化为单位矩阵
    while (y) {  // 判断是否遍历完
        if (y & 1) {  // 如果幂次当前位上是1
            res *= x;  // 如果是矩阵的快速幂，需要自己实现一个两个矩阵相乘的函数
        }
        x *= x;  // 将x更新为自己的平方，即x^{n} = x^{2*n}，如果是矩阵的快速幂，这里同样使用矩阵相乘的函数
        y >>= 1;  // 下一次遍历下一位
    }
    return res;
}

// 方阵的快速幂
vector<vector<int>> fast_pow_matrix(vector<vector<int>>& matrix, int y) {
    int m = matrix.size();
    // 初始化为单位矩阵
    vector<vector<int>> res(m, vector<int>(m, 0));
    for (int i = 0; i < m; i++) {
        res[i][i] = 1;
    }
    // 计算两个方阵相乘的结果
    function<vector<vector<int>>(vector<vector<int>>&, vector<vector<int>>&)> dot = [&](vector<vector<int>>& a, vector<vector<int>>& b) {
        vector<vector<int>> res(m, vector<int>(m, 0));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                for (int k = 0; k < m; k++) {
                    res[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        return res;
    };
    // 逻辑和数的快速幂算法一致
    while (y) {
        if (y & 1) {
            res = dot(res, matrix);
        }
        matrix = dot(matrix, matrix);
        y >>= 1;
    }
    return res;
}


/**
 * 斐波那契数列问题的优化
 * 基础的斐波那契数列问题的时间复杂度为O(N)
 * 可以用矩阵的快速幂将时间复杂度优化到O(logN)
 * 更一般地，除了初值之外，具有严格递推关系式的问题（一般递推关系是加减），且没有条件转移，都可以利用矩阵的快速幂算法优化时间复杂度
 * 斐波那契数列问题的优化：
 * 递推关系式为（还可以扩展到更多项，以及每一项前的系数都是可变的） f(N) = f(N - 1) + f(N - 2)
 * 初值为 f(1) = 1, f(2) = 1
 * 那么我们可以构建矩阵的递推关系（用 | 代表矩阵）
 *
 * |f(3)|   | a  b |   |f(2)|
 * |    | = |      | * |    |   (1)
 * |f(2)|   | c  d |   |f(1)|
 *
 * |f(4)|   | a  b |   |f(3)|
 * |    | = |      | * |    |   (2)
 * |f(3)|   | c  d |   |f(2)|
 *
 * 通过(1)(2)两式可以求解出a,b,c,d，也就是系数矩阵
 * 那么递推关系式就变成了
 * M(N) = A^{N - 2} * M(2)      (3)
 * 式中
 *        |  f(N)  |        | a  b |
 * M(N) = |        |,   A = |      |
 *        |f(N - 1)|        | c  d |
 * 所以，其中A^{N - 2}这一项就可以用矩阵的快速幂以O(logN)的时间复杂度计算出来
 */
int Fibonacci_optimize(int N) {
    if (N == 1 || N == 2) return 1;
    if (N == 3 || N == 4) return N == 3 ? 2 : 3;
    // 通过(1)和(2)求解出的A矩阵
    vector<vector<int>> A = {{1, 1}, {1, 0}};
    // 计算出A矩阵的N-2次幂
    vector<vector<int>> APowNMinus2 = fast_pow_matrix(A, N - 2);
    // 最终由式(3)得到答案
    return APowNMinus2[0][0] + APowNMinus2[0][1];
}

/**
 * 斐波那契数列优化示例2，母牛的数量
 * 有一个农场，有一头母牛，每年可以生一只牛，每头生下来的牛都是母牛，
 * 生下来的母牛从生下来开始到成年需要3年，成年后就可以进行生育，同时也只能生母牛
 * 问，N年后（N年内没有牛死亡，且都可以正常生育），农场有几头牛？
 * 经过分析可以得到递推关系：
 * f(N) = f(N - 1) + f(N - 3)
 * 解释：第N年的牛的数量首先可以完全继承第N-1年的数量，其次，由于母牛会生产，且成年需要3年，所以第N-3年的牛在第N年都能生育，所以加上第N-3年的牛
 * 那么，写成矩阵的形式就是一个三阶矩阵
 * |f(4)|   | a d g |   |f(3)|
 * |f(3)| = | b e h | * |f(2)|
 * |f(2)|   | c f i |   |f(1)|
 * 然后做法和斐波那契数列一样就可以了
 */
int CowNumber(int N) {
    if (N <= 4) return N;
    // 这个A矩阵有一定规律，第一行是和递推式一样的，后面每一行都只有一个1，其余都是0
    vector<vector<int>> A = {{1, 0, 1}, {1, 0, 0}, {0, 1, 0}};
    vector<vector<int>> APowNMinus3 = fast_pow_matrix(A, N-3);
    return 3 * APowNMinus3[0][0] + 2 * APowNMinus3[0][1] +  APowNMinus3[0][2];
}

/**
 * 斐波那契数列问题示例3，统计达标字符串数量
 * 规定字符串只能由 0 和 1 构成，对于一个字符串，如果字符串中的 0 出现的位置的左边相邻元素为 1 ，那么该字符串就是达标的
 * 统计长度为N的，由0和1构成的所有可能字符串中，达标字符串的数量。（全为1的字符串也是达标的）
 */
/**
 * 直接先给出结论，该题就是初值不同的斐波那契数列，f(N) = f(N - 1) + f(N - 2), f(1) = 1, f(2) = 2
 * 如何得出该结论？
 * 1. 打表，通过暴力打表可以得到规律
 * 2.   定义 f(n) 为，长度n的所有字符串中，其左边第一个位置为1且达标的字符串（由分析可知，达标字符串的第一个元素一定是1）
 *      那么就要思考1后面n-1长度字符串中有哪些是合法的，分类讨论
 *      （1）n-1字符串中，假设第一个元素是1，那么根据f()的定义，就是f(n-1)
 *      （2）n-1字符串中，假设第一个元素是0，那么下一个元素必定不能是0，只能是1，这种情况可以看成n-2长度字符串中，第一个元素为1的情况，即f(n-2)
 *      故，f(n)=f(n-1)+f(n-2)
 * 从而，此题得解
 */
int LegalStringNumber(int N) {
    if (N == 1 || N == 2) return N;
    // 初值的不同不会影响A，直接使用斐波那契数列中的A
    vector<vector<int>> A = {{1, 1}, {1, 0}};
    vector<vector<int>> APowNMinus2 = fast_pow_matrix(A, N - 2);
    return 2 * APowNMinus2[0][0] + APowNMinus2[0][1];
}

/**
 * 斐波那契数列问题示例4，最少去掉木棍的数量
 * 给定一个数N，表示有N根木棍，编号为1~N，且木棍的长度等于其编号
 * 请问，至少要去掉几根木棍，才能使得任意的三根木棍无法拼成一个三角形
 */
/**
 * 这其实就是一个斐波那契数列问题
 * 看我的分析：
 * 第i根木根是否要去掉的依据就是，集合中，是否存在小于它的两根木棍长度之和大于它（构成三角形的基本条件）
 * 至少要去掉的就是所有满足上述条件的i
 * 常规做法就是，先取一个集合，集合中只包含最小的两根木棍，
 * 从这两根木棍开始，依次去掉满足条件的i，并将符合不能构成三角形的，且将等于该两根木棍长度之和的木棍加入集合（因为该木棍也无法构成三角形）
 * 接着，选取集合中最大的两根木棍，依次进行下去，最终就能得到最少需要删除的木棍的数量
 *
 * 仔细分析一下，这不就是斐波那契数列吗？
 * 问题转换成了，小于等于N的斐波那契数列的个数有几个，这些斐波那契数列构成的集合中，任意的三个数都不能组成三角形
 * 所以至少要去掉 N - 斐波那契数列个数 这么多根木棍
 */
int minRemoveNumber(int N) {
    // 使用二分找到斐波那契数列的个数
    int left = 0, right = N;
    while (left < right) {
        int mid = left + (right - left + 1) / 2; // 改成向上取整，因为这里左半边的区间才是合法的，如果向下取整可能会一直陷入left=mid的循环中
        if (Fibonacci_optimize(mid) <= N) {
            left = mid;
        } else {
            right = mid - 1;
        }
    }
    // 加1是因为斐波那契数列有两个1，所以要将它加回来
    return N - left + 1;
}


/**
 * 背包问题
 * 背包容量为w，一共有n袋零食，用v[i]表示第i袋零食的体积
 * 请问在总体积不超过w的情况下，一共有多少种零食放法（总体积为0也算是一种放法）
 */
int BagMethods(int w, vector<int>& v) {
    int n = v.size();
    vector<vector<int>> dp(n + 1, vector<int>(w + 1, -1));
    function<int(int, int)> dfs = [&](int index, int volume) {
        if (volume > w) return 0;
        if (dp[index][volume] != -1) return dp[index][volume];
        if (index == n) {
            dp[index][volume] = 1;
        } else {
            int p1 = dfs(index + 1, volume);
            int p2 = dfs(index + 1, volume + v[index]);
            dp[index][volume] = p1 + p2;
        }
        return dp[index][volume];
    };
    return dfs(0, 0);
}


/**
 * 每个人最优的工作岗位
 * 给定一个二维数组jobs，其中jobs[i][0]代表一份工作的难度，jobs[i][1]代表一份工作的报酬
 * 给定一个一维数组ability，其中ability[i]代表第i个人的能力
 * 当一个人的能力大于等于工作难度时，这个人才能胜任这个工作，且所有工作的岗位都是无限的
 * 请给出每个人最优的工作岗位（报酬越高，认为工作岗位越优）
 */
/**
 * 分析：使用有序表
 * 先根据工作难度从小到大给工作排序，如果工作难度一样，则薪酬高的排前面
 * 再将工作插入有序表中，插入的规则是：同样难度的工作，只插入薪资最高的；不同难度的工作，只插入随着工作难度增加，薪酬也增加的工作
 */
vector<int> BestJob(vector<vector<int>>& jobs, vector<int>& ability) {
    // 先对工作排序，排序规则先根据工作难度升序，如果工作难度一样，就根据工作薪酬降序
    sort(jobs.begin(), jobs.end(),
         [&](vector<int>& a, vector<int>& b) {
        if (a[0] == b[0]) return a[1] > b[1];
        return a[0] < b[0];
    });
    // 将工作加入有序表中，规则在上面的分析中
    map<int, int> BestJobList;
    BestJobList.emplace(jobs[0][0], jobs[0][1]);
    vector<int> pre_job = jobs[0];
    for (auto& job: jobs) {
        if (job[0] != pre_job[0] && job[1] > pre_job[1]) {
            BestJobList.emplace(job[0], job[1]);
            pre_job = job;
        }
    }
    // 最后根据每个人的能力，确定他们的最优工作
    vector<int> res(ability.size());
    for (int i = 0; i < ability.size(); i++) {
        auto it = BestJobList.upper_bound(ability[i]);
        res[i] = it == BestJobList.begin() ? 0 : (--it)->second;
    }
    return res;
}


/**
 * 将一个string转换为int
 * 注意：string可能是不符合正常书写习惯的（下面代码中会展示何为不正常书写习惯），string对应的数字也有可能会发生溢出
 * 上述注意事项如果出现了，表示string不能被转换，请抛出异常，否则返回被转换的值
 */
// 判断字符串是否符合正常书写习惯
bool isValid(string s) {
    // 如果首字符为0，且字符串长度大于1(一般人不会写 013)
    if (s[0] == '0' && s.length() > 1) return false;
    // 如果首字符为负号，后续没有字符或者后续跟着0(一般人不会写 - 或者 -01)
    if (s[0] == '-' && (s.length() == 1 || s[1] == '0')) return false;
    // 如果首字符不为负号，且首字母不是数字
    if (s[0] != '-' && (s[0] < '0' || s[0] > '9')) return false;
    // 如果后续字符串中出现非数字字符，就都返回false
    for (int i = 1; i < s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
}
// 转换字符串
int convert(string s) {
    if (!isValid(s)) {
        throw runtime_error("string is not valid");
    }

    bool neg = s[0] == '-';  // 判断一个数是否为负数
    // 下面两个变量用于判断是否会溢出
    int minq = INT_MIN / 10;
    int minr = INT_MIN % 10;
    int res = 0, cur = 0;
    for (int i = neg ? 1 : 0; i < s.length(); i++) {  // 如果是负数，迭代从1开始，反之从0开始
        // 这里先默认将字符串转换为负数
        // 因为负数所能表示的绝对值范围比正数要大1，也就是说，INT_MIN不可以用正数表示，但是INT_MAX可以用负数表示
        cur = '0' - s[i];
        // 判断是否会溢出
        // 如果 res < minq ，那么接下来 res * 10 就会溢出
        // 如果 res == minq && cur < minr ，那么接下来的 res * 10 + cur 就会溢出
        if (res < minq || (res == minq && cur < minr)) {
            throw runtime_error("out of range");
        }
        res = res * 10 + cur;
    }
    // 如果res是最小值，且string是一个正数，就会发生溢出
    if (!neg && res == INT_MIN) {
        throw runtime_error("out of range");
    }

    return neg ? res : -res;
}


int main() {
//    vector<int> nums = {1, 3, 2};
//    cout << nearbyProduct(nums);
//    cout << fast_pow(2, 30);
//    cout << Fibonacci_optimize(15);
//    cout << CowNumber(8);
//    cout << LegalStringNumber(8);
//    cout << minRemoveNumber(17);
//    vector<int> v = {1, 2, 5, 3, 6, 9, 8};
//    cout << BagMethods(3, v);
//    vector<vector<int>> jobs = {{1, 4}, {2, 5}, {2, 3}, {1, 6}, {5, 6}, {5, 4}, {8, 3}, {8, 9}, {9, 8}};
//    vector<int> ability = {4, 7, 2, 8, 3, 9};
//    BestJob(jobs, ability);
    string s = "-2147483648";
    cout << convert(s);
    return 0;
}
