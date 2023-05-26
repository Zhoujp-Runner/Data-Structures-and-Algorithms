/***********************************************************************
 * Copyright 2023 by Zhou Junping
 *
 * @file     hash_structure.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * @details
 * 该文件写的是左程云算法视频的提升课程中的与哈希函数有关的结构，包含：
 * 笔记
 * 最近修改日期：2023-05-26
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2023-05-26
 *
 */

/**********笔记***********
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
 */
