/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中的Manacher算法，包含：
 * 笔记
 * Manacher算法的实现
 * 最近修改日期：2023-06-12
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-6-5
 *
 */

/****************笔记*****************
 * 假设现在需要求的是一个字符串中最大回文长度
 *
 * 首先需要明确几个概念：
 *      1.回文串：就是一个序列，正和逆是相等的，对应到字符串，举个例子 'abcba' 就是回文串
 *      2.回文数组：由回文串长度组成的数组，数组长度和原序列长度一致，数组中第i个位置的值就是序列中以第i个元素为中心的最大回文串长度
 *      3.最大右边界R：从左往右求回文数组中的值的时候，假设在求第i个元素的最大回文长度，
 *        此时前i-1个元素中，每个元素对应的回文串的最右的边界，就是R（注意，只考虑右边界的最大值，不考虑回文串长度）
 *      4.最大右边界中心C：即上述最大右边界的所属于的回文串的中心元素的位置（注意，R和C是同步更新的）
 *      5.回文串半径/直径：回文串半径就是当前回文串长度的一半(算上中心点)，直径就是整个回文串长度
 *
 * 传统求法，就是简单的遍历元素，以每一个元素为中心，向两边扩，比较字符是否相同，不相同就停止，记录一下长度，直到遍历完整个字符串，返回最大值
 * 但是这个有一个问题，就是如果回文串是偶数的话，就找不到，因为上述这种方法是默认有一个实际中心点字符的，而偶数回文中心点不是实际的字符
 * 就需要手动扩充一下字符串，在字符串的每两个相邻元素之间插入一个虚拟字符，包括头之前和尾之后
 * （这个虚拟字符没有要求，即使原字符串中有相同的字符也没有关系，因为始终都是两个实际的字符在进行比较，两个插入的字符在进行比较）
 * 例如 'abaaba' 扩充为 '#a#b#a#a#b#a#' 其中#为扩充的虚拟字符
 * 然后再进行遍历求解
 * 这种传统求法的时间复杂度为O(n^2)
 *
 * Manacher算法
 * 由于解释起来稍微有点复杂，所以直接上伪代码吧，如果要看解析，可以看左程云的视频
 * 伪代码：
 *      输入：需要求解的字符串s
 *      s -> str (str代表扩充虚拟字符后的字符串);
 *      int* pArr = new int [str.length()];
 *      int R = -1;
 *      int C = -1;
 *      for(int i = 0; i < str.length(); i ++) {
 *          if (i > R) {
 *              pArr[i] = 暴力求解最大回文串长度（从中心往两边扩）;
 *              更新R为当前回文串的右边界;
 *              C = i;
 *          }
 *          else {
 *              if (i'的回文串半径 < R - i) {  // i'就是i关于C对称过去的点
 *                  pArr[i] = pArr[i'];
 *              } else if (i'的回文串半径 > R - i) {
 *                  pArr[i] = R - i;
 *              } else {
 *                  不需要从i开始扩，直接从R开始扩，寻找最大回文串;
 *                  更新R;
 *              }
 *          }
 *      }
 *      pArr中最大的数就是该字符串的最大回文串长度
 */

#include <iostream>
#include <string>

using namespace std;
/**
 * @brief Manacher算法，和上面笔记上的伪代码有一点不同，但是中心思想一致
 */
class Manacher {
public:
    explicit Manacher(const string &s) {
        source_str = s;
        this->expand_str();
        pArr = new int [str.length()]();
        cout << source_str << endl;
        cout << str << endl;
    }

    // 扩充虚拟字符
    void expand_str() {
        str = "#";
        for (int i = 0; i < source_str.length(); i++) {
            str += source_str[i];
            str += '#';
        }
    }

    void maxlength() {
        if (source_str.empty()) {
            cout << "string is empty" << endl;
            return;
        }
        int R = -1;
        int C = -1;
        int max = 0;
        for (int i = 0; i < str.length(); i++) {
            // 首先算出最长的不用比较的区域
            // 如果i比R大，说明比较点在最右右边界之外，说明此时只有自己本身是不用比较的，所以不用比较区域为1
            // 如果i比R小，说明比较点在最右右边界之内，这时候会有两种情况
            // 第一种是i'点（注意是i'，不是i，i'含义在上面笔记中）的回文串半径小于R-i，即i'的回文串全在R内，此时不用比较的区域即为i'的回文串半径，即pArr[2 * C - i]
            // 第二种是i'点的回文串半径大于等于R-i，此时不用比较的区域大小是R-i（其实当大于R-i时，是不用进行比较的，当前位置的回文串半径就是R-i，这里为了统一代码，都比较了一下，反正比较一次就会退出比较循环）
            pArr[i] = i > R ? 1 : min(R - i, pArr[2 * C - i]);

            // 比较循环，用来比较两个位置的字符是否相等，相等便将回文串长度加1
            while (i + pArr[i] < str.length() && i - pArr[i] > -1) {  // 不越界
                if (str[i + pArr[i]] == str[i - pArr[i]]) {
                    pArr[i]++;
                } else {
                    break;
                }
            }

            // 如果发现当前i位置的回文串最右边界已经超过了R，那么更新R与C
            if (i + pArr[i] > R) {
                R = i + pArr[i];
                C = i;
            }

            // 记录最长回文串半径
            max = max > pArr[i] ? max : pArr[i];
        }

        // 最长回文串半径-1 就是题目所要求的，证明可以自己动手画一下
        cout << max - 1 << endl;
    }
private:
    string source_str;
    string str;
    int* pArr = nullptr;
};

int main() {
    Manacher m("2abcba2");
    m.maxlength();

    return 0;
}
