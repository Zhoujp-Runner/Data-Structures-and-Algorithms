/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     bit_operation.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的基础提升课程中关于位运算题目的内容：
 * 1. 不使用比较判断，返回两数中较大的数
 * 2. 判断一个32位正数是不是2的幂、4的幂
 * 3. 用位运算实现加减乘除
 * 最近修改日期：2023-08-27
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-8-26
 *
 */

#include <iostream>
#include <bitset>

using namespace std;

int isPos(int x);  // 判断一个数是否是负数
int flip(int x);  // 翻转 1->0  0->1
int Max_number(int a, int b);  // 不使用比较判断 返回两个数中较大的数
bool isTwoPow(int x);  // 判断一个数是否是2的幂
bool isFourPow(int x);  // 判断一个数是否是4的幂
int add_recursion(int a, int b);  // 用位运算实现a+b(递归版本)
int add(int a, int b);  // 用位运算实现a+b
int my_minus(int a, int b);  // 用位运算实现a-b
int to_opposite(int x);  // 返回一个数的相反数
int product(int a, int b);  // 用位运算实现a*b
int my_divide(int a, int b);  // 用位运算实现a/b（自己的版本）
int divide_(int a, int b);  // 用位运算实现a/b（左神的版本）
int divide(int a, int b);  // 用位运算实现a/b（考虑0与最小值）

/**
 * @details 判断一个数是否是正数
 * @param
 *      int x:被判断的数
 * @return 负数返回0，正数返回1
 */
int isPos(int x) {
    return flip((x>>31) & 1);
}

int flip(int x) {
    return (x ^ 1);
}


/**
 * @details 不使用比较，返回两个数中较大的那一个
 *          并且，该方法不单独通过差值的正负判断大小
 *          不会受到数值溢出的影响
 * @return 较大的那一个数
 */
int Max_number(int a, int b) {
    int c = a - b;  // 先将两者作差
    int aP = isPos(a);  // 判断a是否是正数
    int bP = isPos(b);  // 判断b是否是正数
    int cP = isPos(c);  // 判断c是否是正数
    int Diff = aP ^ bP;  // 判断a，b是否同号
    int Same = flip(Diff);  // 判断a，b是否异号
    // 返回a的情况是：当a，b异号时，a为正数（可以防止溢出）；或者当a，b同号时，c为正数
    int retrunA = Diff * aP + Same * cP;
    // 返回b的情况：与返回a相反
    int returnB = flip(retrunA);
    return retrunA * a + returnB * b;
}

/**
 * @details 判断一个正数数是否是2的幂
 *          如果是2的某次幂，那么这个数的二进制表示中只能有一个1
 */
bool isTwoPow(int x) {
    return ((x -  1) & x) == 0;
}

/**
 * @details 判断一个正数是否是4的幂
 *          如果是4的某次幂，那么该数首先得保证只能有一个1，其次这个1必须出现在偶数位上（最低位为0位）
 */
bool isFourPow(int x) {
    return isTwoPow(x) && (x & 0x55555555) != 0;
}

/**
 * @details 用位运算实现加法(由于负数是用其补码存储的，所以该加法对负数也成立) \n
 *          a和b按位^，表示两者无进位相加的结果 \n
 *          a和b按位&，表示两者的进位信息 \n
 *          从而，将按位^的结果 和 按位&且左移一位的结果 相加 就是两者相加的答案 \n
 *          而上述的相加又可以继续按位^和按位& \n
 *          直到进位信息全部为0为止 \n
 *          add_recursion是递归版本 \n
 *          add是非递归版本
 */
int add_recursion(int a, int b) {
    if (b == 0) {
        return a;
    }
    return add(a ^ b, (a & b) << 1);
}

int add(int a, int b) {
    int sum = a;
    while (b != 0) {
//        cout << bitset<32>(sum) << endl;
//        cout << bitset<32>(b) << endl;
        sum = a ^ b;  // 无进位相加
        b = (a & b) << 1;  // 进位
        a = sum;
    }
    return sum;
}

/**
 * @details 用位运算实现减法 \n
 *          减法其实就是一个数加上另一个数的相反数 \n
 *          相反数怎么求？就是对其取反然后加一
 */
int my_minus(int a, int b) {
    return add(a, to_opposite(b));
}

int to_opposite(int x) {
    return add(~x, 1);
}

/**
 * @details 用位运算实现乘法 \n
 *          二进制乘法的实现方式和我们手动计算十进制乘法的方式一样 \n
 *          例如 001010 * 010010  (10*18) \n
 *              001010 \n
 *              010010 \n
 *          --------------
 *              000000
 *             001010
 *            000000
 *           000000
 *          001010
 *      +  000000
 *      --------------------
 *         00010110100  (180)
 */
int product(int a, int b) {
    int res = 0;
    unsigned int c = unsigned(b);  // 将b变为无符号整型，实现无符号右移，使得负数乘法能够有效
    while( c != 0) {
        if ((c & 1) == 1) {
            res = add(res, a);
        }
        cout << bitset<32>(a) << endl;
        a = a << 1;
        c = c >> 1;
    }
    return res;
}

/**
 * @details 用位运算实现除法（支持有符号除法） a / b \n
 *          除法其实就是乘法的逆序，a / b = c => b * c = a \n
 *          这样说可能会蒙圈，换一种思路，这里的除法其实也是和我们手动计算十进制的除法一样 \n
 *          由于除法不太好演示，就用文字表述，不明白可以看视频 \n
 *          假设我们要进行3600 / 2 \n
 *          我们是先将2与3600最高位进行对比，如果2小于最高位，那就用最高位除2，余下的继续操作 \n
 *          我们换个表示，与其说让2和最高位对比，不如说让2*1000，然后与被除数进行对比 \n
 *          因为此时得到的结果是在千分位上面的，所以，其实就是2*1000，然后 3600 - 2000 之后继续重复刚才的步骤就能得到结果 \n
 *          现在我们将其应用到二进制中 很简单 1000 / 0011 \n
 *          我们的做法是，先将除数不断左移，左移到小于等于1000的最大位置（这里就等同于2*1000），即0110 \n
 *          我们发现左移了1位，那就说明最终结果在从最低位左移1位的位置上就是1，然后将1000和0110相减，\n
 *          继续上述操作，直到0011左移0位还是比余下的数大为止 \n
 *          \n
 *          my_divide是我前一天看完视频，第二天自己写的代码 \n
 *          divide是左神的代码，可以看出确实很简洁，而且还考虑了0和INT_MIN这两个值
 */
int my_divide(int a, int b) {
    // 先将a和b都变为正数
    int x = isPos(a) ? a : to_opposite(a);
    int y = isPos(b) ? b : to_opposite(b);
    int res = 0;
    while (x >= y) {
        int i = 1;
        // x右移去和y比 其实就等价于 y左移和x比，
        // 参照左神的代码，好处在于不用担心溢出，即使左移1位也有溢出的可能
        while ((x >> i) >= y) {
            i = add(i, 1);
        }
        // 由于每次置1的位肯定都不相同，所以可以直接进行相加
        res = add(res, (1 << (i - 1)));
        // x更新为剩余的数
        x = my_minus(x, (y << (i - 1)));
    }
    return isPos(a) ^ isPos(b) ? to_opposite(res) : res;  // 如果a和b异号，就返回结果的相反数；否则返回结果本身
}

int divide_(int a, int b) {
    int x = isPos(a) ? a : to_opposite(a);
    int y = isPos(b) ? b : to_opposite(b);
    int res = 0;
    /**
     * 他这里是从31位开始试，只需要遍历一次就能知道后续每次移动的位数
     * 因为如果当前是x右移第6位大于y，右移第7位就小于y
     * 那么下一次循环，由于已经做了一次减法，所以下一次右移的位数必定小于6
     */
    for (int i = 31; i > -1; i = my_minus(i, 1)) {
        if ((x >> i) >= y) {  // 他这里让x右移的原因在于这样做不会溢出，如果是y左移31位，会发生溢出
            res |= (1 << i);  // 直接用或运算将某一位置1
            x = my_minus(x, (y << i));
        }
    }
    return isPos(a) ^ isPos(b) ? to_opposite(res) : res;
}

int divide(int a, int b) {
    if (b == 0) {
        cout << "b must not be zero" << endl;
        return 0;
    }

    // 考虑a和b是最小值的情况
    // 因为a和b要是是最小值的话，在除法的代码中的前两行，最小值会由于to_opposite操作变为0，答案就错了
    if (a == INT_MIN && b == INT_MIN) {
        return 1;
    } else if (a == INT_MIN) {
        // 当a为最小值时，先将a+1，这时，转变为正数就是正数的最大值，再进行除法得到res
        // 但是有一个问题，由于符号位的存在，负数的最小值的绝对值是要比正数的最大值大1的（以8位二进制为例，最小值是-128，最大值是127）
        // 所以此时的除法还不是真的除法，需要 res + ((a - b*res) / b)
        // 为什么这样做，还是以8位二进制为例
        // -128可以被2整除，但是127除以2会余1，所以我们需要将127/2的剩余部分+1再除以2，此时为1，说明-128可以被2整除，需要将127/2的结果加上1
        // 第二行的return就是这么做的
        int res = divide_(add(a, 1), b);
        return add(res, divide_(my_minus(a, product(res, b)), b));
    } else if (b == INT_MIN) {
        return 0;
    } else {
        return divide_(a, b);
    }
}

int main() {
    int a = INT_MIN;
    int b = 3;
    cout << divide(a, b) << endl;
    return 0;
}

