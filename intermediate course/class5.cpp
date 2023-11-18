/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     class5.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的中级提升课程中的内容：
 * 1.相邻数之积为4的倍数的问题
 * 最近修改日期：2023-11-18
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


int main() {
    vector<int> nums = {1, 3, 2};
    cout << nearbyProduct(nums);
    return 0;
}
