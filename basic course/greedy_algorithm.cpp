/***********************************************************************
 * Copyright 2022 by Zhou Junping
 *
 * @file     trietree.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * 该文件写的是一些关于贪心算法的代码,包括：
 * 1.（问题1）每场会议的开始时间和结束时间都是不一样的，如何对会议进行排序从而在有效的时间之内尽可能多开会议
 * 2.（问题2）切金条问题————哈夫曼编码问题
 * 3.（问题3）利润问题，已知初始资金，以及一堆已知投资金额与利润的项目，串行执行项目，求最后初始资金为多少
 * 4.（问题4）给一个数据流，用户会往数据流中加数，要求能够随时取得该数据流的中位数
 * 5.（问题5）N皇后问题
 * 最近修改日期：2022-11-26
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2022-11-17
 *
 */

#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;

struct Meeting{
    float start_time;
    float end_time;

    Meeting(float start, float end) {
        start_time = start;
        end_time = end;
    }

    bool operator < (const Meeting & meet) const {
        return this->end_time < meet.end_time;
    }
};

struct Project{
    int cost;//投资金额
    int profit;//利润

    Project(int c, int p){
        cost = c;
        profit = p;
    }
};

struct cmp_cost{//小根堆
    bool operator () (const Project & p1, const Project & p2) {
        return p1.cost > p2.cost;
    }
};

struct cmp_profit{//大根堆
    bool operator () (const Project & p1, const Project & p2) {
        return p1.profit < p2.profit;
    }
};

vector<Meeting> get_meetings();//问题1
vector<Meeting> question1(float start, float end);//（问题1）每场会议的开始时间和结束时间都是不一样的，如何对会议进行排序从而在有效的时间之内尽可能多开会议
int question2(int arr[], int len);//（问题2）切金条问题————哈夫曼编码问题
int question3(int fund, int num);//（问题3）利润问题，已知初始资金，以及一堆已知投资金额与利润的项目，串行执行项目，求最后初始资金为多少
vector<Project> get_project();
int question4(int arr[], int len);//（问题4）给一个数据流，用户会往数据流中加数，要求能够随时取得该数据流的中位数
int question5(int n);//（问题5）N皇后问题
int process(int n, int record[], int i);//N皇后问题的递归函数
bool isvalid(int record[], int i, int j);//判断该位置是否能放置皇后
int question5_bit(int n);//N皇后问题，使用位运算加速
int process2(int limit, int col, int left, int right);//使用位运算加速的递归表达

vector<Meeting> get_meetings() {
    vector<Meeting> meetings;
    meetings.emplace_back(Meeting(6.10, 7.15));
    meetings.emplace_back(Meeting(7.10, 9.30));
    meetings.emplace_back(Meeting(8.10, 14.00));
    meetings.emplace_back(Meeting(9.10, 16.45));
    meetings.emplace_back(Meeting(12.10, 13.45));
    meetings.emplace_back(Meeting(14.10, 16.45));
    meetings.emplace_back(Meeting(17.10, 18.00));
    stable_sort(meetings.begin(), meetings.end());

    return meetings;
}

//（问题1）每场会议的开始时间和结束时间都是不一样的，如何对会议进行排序从而在有效的时间之内尽可能多开会议
vector<Meeting> question1(float start, float end) {
    vector<Meeting> meetings = get_meetings();
    vector<Meeting> schedule;
    float t_flag = start;
    for (auto i : meetings) {
        if (i.start_time >= t_flag && i.end_time <= end) {
            schedule.emplace_back(i);
            t_flag = i.end_time;
            continue;
        }
    }
    return schedule;
}

//（问题2）切金条问题————哈夫曼编码问题
int question2(int arr[], int len) {
    priority_queue<int, vector<int>, greater<int>> small_heap;
    for (int i = 0; i < len; ++i) {
        small_heap.push(arr[i]);
    }

    int top1;
    int top2;
    int sum = 0;
    while (small_heap.size() > 1) {
        top1 = small_heap.top();
        small_heap.pop();
        top2 = small_heap.top();
        small_heap.pop();
        sum += (top1 + top2);
        small_heap.push(sum);
    }

    return sum;
}

bool compare_cost(const Project & p1, const Project & p2) {
    return p1.cost < p2.cost;
}

bool compare_profit(const Project & p1, const Project & p2) {
    return p1.profit > p2.profit;
}

vector<Project> get_project() {
    vector<Project> projects;
    projects.emplace_back(Project(1, 2));
    projects.emplace_back(Project(4, 3));
    projects.emplace_back(Project(2, 2));
    projects.emplace_back(Project(6, 5));
    projects.emplace_back(Project(9, 8));
    projects.emplace_back(Project(8, 2));

    return projects;
}

//（问题3）利润问题，已知初始资金，以及一堆已知投资金额与利润的项目，串行执行项目，求最后初始资金为多少
int question3(int fund, int num) {
     vector<Project> projects = get_project();
     priority_queue<Project, vector<Project>, cmp_cost> cost_heap;
     priority_queue<Project, vector<Project>, cmp_profit> profit_heap;

    for (auto i : projects) {
        cost_heap.push(i);
    }

    if (num < 0) {
        //假设能做无限个项目
        while (!cost_heap.empty() || !profit_heap.empty()) {
            if (fund >= cost_heap.top().cost && !cost_heap.empty()) {
                profit_heap.push(cost_heap.top());
                cost_heap.pop();
            } else {
                if (profit_heap.empty()) break;
                Project project = profit_heap.top();
                profit_heap.pop();
                fund += project.profit;
            }
        }
    } else {
        for (int i = 0; i < num; ++i) {
            //将所有投资花费小于资金的项目加入profit大根堆中
            while (!cost_heap.empty() && fund >= cost_heap.top().cost) {
                profit_heap.push(cost_heap.top());
                cost_heap.pop();
            }

            if (profit_heap.empty()) break;

            fund += profit_heap.top().profit;
            profit_heap.pop();
        }
    }

    return fund;
}

//（问题4）给一个数据流，用户会往数据流中加数，要求能够随时取得该数据流的中位数
int question4(int arr[], int len) {
    if (arr == nullptr || len <= 0) {
        cout<<"wrong arr"<<endl;
        return -1;
    }

    priority_queue<int, vector<int>, less<int>> big_heap;
    priority_queue<int, vector<int>, greater<int>> small_heap;
    big_heap.push(arr[0]);

    //通过大小根堆互相配合得到
    for (int i = 1; i < len; ++i) {
        if (arr[i] <= big_heap.top()) {
            big_heap.push(arr[i]);
        } else {
            small_heap.push(arr[i]);
        }

        ///这样写是错的，会发生整型提升，即有符号数转为无符号数，导致“0 <= -2”成立
//        bool flag = (small_heap.size() - big_heap.size()) <= (-2);
        int difference = small_heap.size() - big_heap.size();
        if (difference >= 2) {
            big_heap.push(small_heap.top());
            small_heap.pop();
        } else if (difference <= -2) {
            small_heap.push(big_heap.top());
            big_heap.pop();
        }
    }

    while (!small_heap.empty()) {
        cout<<small_heap.top()<<endl;
        small_heap.pop();
    }
    return big_heap.top();
}

//（问题5）N皇后问题
int question5(int n) {
    int * record = new int[n];
    return process(n, record, 0);
}

//N皇后问题的递归函数
int process(int n, int record[], int i) {
    //n代表最大行数
    //record[]用于存储之前行的放置情况：record的索引为行号，record对应索引的值为列号
    //i代表当前行
    if (i >= n) {
        return 1;
    }
    //检查当前行有哪些位置可以放置皇后
    int cur_result = 0;
    for (int j = 0; j < n; ++j) {
        if (isvalid(record, i, j)) {
            record[i] = j;
            int result = process(n, record, i + 1);
            cur_result += result;
        }
    }
    return cur_result;
}

//判断该位置是否能放置皇后
bool isvalid(int record[], int i, int j) {
    for (int k = 0; k < i; ++k) {
        if (record[k] == j || abs(i - k) == abs(j - record[k])) return false;//排除同一列的情况以及同一对角线的情况
    }
    return true;
}

//N皇后问题，使用位运算加速
int question5_bit(int n) {
    if (n <=0 || n > 32) {
        return 0;
    }
    int limit = n == 32 ? -1 : ((1 << n) - 1);
    return process2(limit, 0, 0, 0);
}

//使用位运算加速的递归表达
int process2(int limit, int col, int left, int right) {
    //limit:限制右边几位是用来放置皇后的
    //col:这一行上有哪些地方放置皇后会与前面同列
    //left:这一行上有哪些地方放置皇后会与前面同左斜线
    //right:这一行上有哪些地方放置皇后会与前面同右斜线
    if (col == limit) { //意味着所有列数都已经放置上皇后了
        return 1;
    }

    int pos = limit & (~(col | left | right)); //pos代表当前行有哪些位置是可以放置皇后的，1代表能放置皇后，0代表不能放置皇后
    int right_one = 0;
    int res = 0;

    while (pos != 0) {
        right_one = pos & (~pos + 1); // 提取出最右边的1
        pos = pos - right_one;
        res += process2(limit,
                        col | right_one,
                        (left | right_one) << 1,
                        (right_one | right) >> 1);
    }
    return res;
}

int main() {
//    vector<Meeting> schedule = question1(6.00, 18.00);
//    for (auto i : schedule) {
//        cout<<"start:"<<i.start_time<<"  end:"<<i.end_time<<endl;
//    }

//    int arr[3] = {10, 30, 20};
//    cout<<question2(arr, 3)<<endl;

//    int arr[10] = {1, 3, 4, 2, 6, 5, 7, 8, 0, 9};
//    cout<<question4(arr, 10)<<endl;

//    unsigned int i = 0;
//    unsigned int j = 0;
//    bool flag = (i - j) <= -2;
//    cout<<flag;
    cout<<question5_bit(8)<<endl;
    return 0;
}
