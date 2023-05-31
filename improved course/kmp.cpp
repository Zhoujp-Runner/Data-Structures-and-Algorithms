/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中的KMP算法，包含：
 * 笔记
 * KMP算法的实现
 * 最近修改日期：2023-05-31
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-05-31
 *
 */


/*******************笔记*******************
 * KMP算法是用来解决求一个序列是否是另一个序列的子序列问题（注意，子序列在原序列中是连续的）
 * 假设较长序列长度为N，较短序列长度为M，如果使用暴力比对，那么时间复杂度为O(N*M)，较大
 * 所以需要另外的算法来求解，即KMP
 *
 * KMP算法流程，用实际例子举例：
 * 查找'abbcsabbce'是否是'abbcsabbcsabbceacd'的子序列
 * 1. 对于较短的序列生成next数组，生成的方法如下：（注意，肯定是较短的是子序列，如果发现子序列长度超过原序列，直接返回false就行）
 *      next数组的长度和子序列长度一致，next[i]的值对应着序列中第i个元素的前缀序列中（0 -- i-1），满足前缀等于后缀的最大长度
 *      以i=9为例，那需要算的是0 -- 8的序列，即'abbcsabbc'
 *      该序列中，前缀等于后缀的情况只有'abbc-s-abbc'这一种情况，也就是说next[9] = 4
 *      需要注意，前缀等于后缀这一要求不能包含序列本身，即不能出现前缀为'abbcsabbc'后缀也为'abbcsabbc'
 *      同理，i=8时，满足'abb-cs-abb'，next[8] = 3
 *      next[0]由于是序列的第一个字母，前面没有字母了，所以一律为-1
 *      next[1]由于满足前缀等于后缀的只有前缀序列本身，即'a'，但是规定不能有这种情况
 *      那么对于1位置来说，最大满足前缀等于后缀的序列长度为0，所以next[1] = 0
 *      根据这个规则，就可以生成一个next数组
 * 2. 如何进行比对：
 *      暴力的情况：
 *          'abbcsabbcsabbceacd'
 *          'abbcsabbce'
 *          一个字符一个字符比对，发现不符合，下面序列往后推一格
 *          'abbcsabbcsabbceacd'
 *           'abbcsabbce'
 *           发现不符合，继续推一格
 *          'abbcsabbcsabbceacd'
 *            'abbcsabbce'
 *            ...(直到都比对完毕或者出现相同的情况)
 *      KMP算法：
 *          还是从左往右遍历，但是从原序列的从j位置到j+1，变成了j位置到j+n位置，这个n就是由next数组决定的
 *          具体流程为：
 *          先从j=0位置开始比对
 *          'abbcsabbcsabbceacd'
 *          'abbcsabbce'
 *          发现当j=9的时候，两个序列第一次出现不同，且next[9]=4
 *          那么i=0就从j=0位置，跳到9-4位置，再进行比对，直到找到符合或者所有比对完毕
 *          'abbcsabbcsabbceacd'
 *               'abbcsabbce'
 *          上述说法只是为了方便理解,实际上是j=9对应i=4位置（i是子序列的位置编号），然后直接从j=9位置开始比对，因为前面的肯定相等了
 *          如果出现第一个元素就不等，即i=0位置和对应的j位置不匹配，那就自动将子序列往后推一格
 *          另一视角就是子序列不动，原序列动，其实都是一样的
 * 3. 为何KMP是有效的？
 *      1. 首先，假设j=9位置是第一次出现不同字符的位置，那么意味着9位置之前的字符都是和子序列相应位置之前的字符都是相同的
 *         那么，对应next[9]=4，意思是，子序列的前4和字符和原序列9位置之前的4个字符是相等的，所以可以直接跳过
 *      2. 那么如何证明j=0--5之间不会有与子序列相同的部分？
 *         其实很简单，假设j=3处发现又可以进行匹配，那么意味着j=3-8的序列是和子序列对应相等的
 *         但是此时，就和next数组产生矛盾，因为next数组记录的是最长的前缀等于后缀的长度
 *         如果j=3可以进行匹配，那只有一种情况，那就是前缀和后缀长度比next的要小
 *         如果比next小，那么意味着，这一个位置是能直接进行匹配的
 *         因为j-n更大了，中间跳过了很多可能可以匹配的位置，所以需要排除
 *
 * 归根结底，暴力尝试过程中，假如不匹配，那么i -> 0, j -> j+1
 * 而KMP中，如果不匹配, 且next[i]不为-1，那么i -> next[i], j不变
 * 中间就省去了很多比对的步骤
 */

#include <iostream>
#include <string>

using namespace std;

/**
 * @brief KMP算法的实现
 */
class KMP {
public:
    KMP(const string &s1, const string &s2) : source_seq(s1), child_seq(s1) {}

    int get_first_index_of_same_seq() {
        if (source_seq.length() < child_seq.length() || source_seq.empty() || child_seq.empty()) return -1;

        int* next = get_next_arr();

        int i = 0;
        int j = 0;
        while (j < source_seq.length() || i < child_seq.length()) {
            if (child_seq[i] == source_seq[j]) {
                i++;
                j++;
            } else if (next[i] == -1) { // 如果next为-1，那么肯定i=0，且该位置不匹配，j往后挪一位
                j++;
            } else {
                // 这里看做是i回到next[i]处继续和j比对
                // 就是把字符串往后推，然后跳过前面相同的部分进行比对
                i = next[i];
            }
        }
        // 如果i越界了，说明存在相同的字符串，返回在原序列中子序列的起始位置；否则返回-1
        return i == child_seq.length() ? int(j - child_seq.length()) : -1;
    }

    int* get_next_arr() {
        int* next = new int [child_seq.length()];
        next[0] = -1;
        if (child_seq.length() == 1) {
            return next;
        }
        next[1] = 0;
        int i = 2;
        int j = 1;
        while (i < child_seq.length()) {
            if (child_seq[i - 1] == child_seq[next[j]]) {
                next[i] = next[j] + 1;
                j = i;
                i++;
            } else if (next[j] == -1) {
                next[i] = 0;
                j = i;
                i++;
            } else {
                j = next[j];
            }
        }
        return next;
    }

private:
    string source_seq;
    string child_seq;
};


int main() {
    string si = "123456";
    cout<<si[1];
    return 0;
}

