/***********************************************************************
 * Copyright 2022 by Zhou Junping
 *
 * @file     linked_list_question.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 * 目前，该文件内包含的题目有：
 * 1.判断一个链表是否是回文结构
 * 2.将一个链表小于某个数的放头部，等于某个数的放中间，大于某个数的放尾部
 * 3.使用空间复杂度O(1)，复制一份rand链表
 * 4.判断两个链表（分别有环或无环）是否相交，是则返回第一个相交节点，否则返回null
 * 最近修改日期：2022-10-19
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2022-10-19（这里本该写创建文件日期，但是此处是添加注释日期）
 *
 */
#include<iostream>
#include<cstdio>

using namespace std;

//链表结构体
struct ListNode{
    int value;
    ListNode* next;
    ListNode(int val, ListNode* nex){
        value = val;
        next = nex;
    }
    ListNode(int val){
        value = val;
        next = nullptr;
    }
};

struct randListNode{
    randListNode(int x){
        value = x;
    }
    int value;
    randListNode* next;
    randListNode* rand;
};

bool question1(ListNode* head);//判断一个链表是否是回文结构
ListNode* question2(ListNode* head, int value);//将一个链表小于某个数的放头部，等于某个数的放中间，大于某个数的放尾部
randListNode* question3(randListNode* head);//使用空间复杂度O(1)，复制一份rand链表
int Buffercmp(void* Tx_Buffer, void* Rx_Buffer, int Num);
ListNode* is_Loop(ListNode* head);//判断一个链表是否有环
ListNode* no_Loop(ListNode* head1, ListNode* head2);//判断两个无环链表是否相交，是则返回第一个相交节点，否则返回null
ListNode* Loop(ListNode* head1, ListNode* head2, ListNode* node1, ListNode* node2);//判断两个有环链表是否相交，是则返回第一个相交点，否则返回null
ListNode* question4(ListNode* head1, ListNode* head2);//判断两个链表（分别有环或无环）是否相交，是则返回第一个相交节点，否则返回null

//判断一个链表是否是回文结构，空间复杂度为O(1)，使用判断指针
bool question1(ListNode* head){
    if(head == nullptr || head->next == nullptr){
        cout<<"linked-list is null or only have one node..."<<endl;
        return false;
    }
    ListNode* pointer_slower = head;
    ListNode* pointer_quicker = head;
    while(pointer_quicker->next != nullptr && pointer_quicker->next->next != nullptr){//循环结束后，slower指针在链表的中心节点
        pointer_slower = pointer_slower->next;
        pointer_quicker = pointer_quicker->next->next;
    }
    if (pointer_quicker->next != nullptr){//针对于偶数节点数的链表
        pointer_quicker = pointer_quicker->next;
    }
    ListNode* n1 = nullptr;
    ListNode* n2 = nullptr;
    n1 = pointer_slower->next;
    pointer_slower->next = nullptr;
    while(n1 != nullptr){
        n2 = n1->next;
        n1->next = pointer_slower;
        pointer_slower = n1;
        n1 = n2;
    }
    n2 = head;
    n1 = pointer_quicker;
    bool res = true;
    while(n1 != nullptr && n2 != nullptr){
        if (n1->value != n2->value){
            res = false;
            return res;
        }
        n1 = n1->next;
        n2 = n2->next;
    }
    n1 = pointer_slower->next;
    pointer_slower->next = nullptr;
    while (n1 != nullptr){// 还原链表
        n2 = n1->next;
        n1->next = pointer_slower;
        pointer_slower = n1;
        n1 = n2;
    }
    return res;
}

//将一个链表小于某个数的放头部，等于某个数的放中间，大于某个数的放尾部
ListNode* question2(ListNode* head, int value){
    if(head->next == nullptr || head == nullptr){
        return head;
    }
    //申请六个指针
    ListNode* s_h = nullptr;
    ListNode* s_t = nullptr;
    ListNode* e_h = nullptr;
    ListNode* e_t = nullptr;
    ListNode* b_h = nullptr;
    ListNode* b_t = nullptr;

    //申请一个遍历的指针
    ListNode* iter = head;

    while(iter != nullptr){
        if(iter->value < value){
            if(s_h == nullptr && s_t == nullptr){
                s_h = iter;
                s_t = iter;
            }else{
                s_t->next = iter;
                s_t = iter;
            }
        }else if(iter->value == value){
            if(e_h == nullptr && e_t == nullptr){
                e_h = iter;
                e_t = iter;
            }else{
                e_t->next = iter;
                e_t = iter;
            }
        }else if(iter->value > value){
            if(b_h == nullptr && b_t == nullptr){
                b_h = iter;
                b_t = iter;
            }else{
                b_t->next = iter;
                b_t = iter;
            }
        }
        iter= iter->next;
    }

    //让三个链表的尾部都指向null，防止构成循环链表
    //必须确保该块链表有节点才能进行操作
    if(s_h != nullptr){
        s_t->next = nullptr;
    }
    if(e_h != nullptr){
        e_t->next = nullptr;
    }
    if(b_h != nullptr){
        b_t->next = nullptr;
    }

    if(s_t != nullptr){
        if(e_h != nullptr){
            s_t->next = e_h;
            e_t->next = b_h == nullptr ? nullptr : b_h;
        } else{
            s_t->next = b_h == nullptr ? nullptr : b_h;
        }
        return s_h;
    } else{
        if(e_h != nullptr){
            e_t->next = b_h == nullptr ? nullptr : b_h;
            return e_h;
        } else{
            return b_h;
        }
    }
}

//使用空间复杂度O(1)，复制一份rand链表
randListNode* question3(randListNode* head){
    if(head == nullptr || head->next == nullptr){
        return head;
    }

    randListNode* iter = head;
    randListNode* memory = nullptr;
    //每个节点复制一次，将其串联在当前节点与下一个节点之间
    while(iter != nullptr){
        int value = iter->value;
        randListNode* copy = new randListNode(value);
        memory = iter->next;
        iter->next = copy;
        copy->next = memory;
        iter = copy->next;
    }

    //遍历原链表的节点，复制出rand指针
    randListNode* src = head;
    randListNode* dst = nullptr;
    while(src != nullptr){
        memory = src->next->next;//他这样写就很强，直接防止溢出
        dst = src->next;
        dst->rand = src->rand == nullptr ? nullptr : src->rand->next;
        src = memory;
//        src = src->next->next;
//        dst = dst->next->next;//我这样写就很low，dst会溢出
    }

    //最后将两个链表拆开
    randListNode* res_head = head->next;
    src = head;
    while (src != nullptr){
        memory = src->next->next;
        dst = src->next;
        src->next = memory;
        dst->next = memory == nullptr ? nullptr : memory->next;
        src = memory;
    }

    return res_head;
}

//数据对比
int Buffercmp(void* Tx_Buffer, void* Rx_Buffer, int Num)
{
    int i=0;
    for(i=0;i<Num;i++)
    {
        if(*((char*)Tx_Buffer) != *((char*)Rx_Buffer))
        {
            printf("\n%d ",i);
            return 0;
        }
        Tx_Buffer = (char*)Tx_Buffer + 1;
        Rx_Buffer = (char*)Rx_Buffer + 1;
    }
    return 1;
}

//判断一个链表是否有环,并返回第一个入环节点
//如果链表无环，则返回null
ListNode* is_Loop(ListNode* head){
    if(head == nullptr || head->next == nullptr){
        return nullptr;
    }

    ListNode* quick = head->next->next;
    ListNode* slow = head->next;
    while (quick != slow){
        if(quick->next == nullptr || quick->next->next == nullptr){
            return nullptr;
        }

        slow = slow->next;
        quick = quick->next->next;
    }

    quick = head;
    while (quick != slow){
        quick = quick->next;
        slow = slow->next;
    }
    return quick;
}

//判断两个无环链表是否相交，是则返回第一个相交节点，否则返回null
ListNode* no_Loop(ListNode* head1, ListNode* head2){

    ListNode* cur1 = head1;
    ListNode* cur2 = head2;
    int n = 0;
    while(cur1->next != nullptr){
        cur1 = cur1->next;
        n++;//统计head1链表节点数
    }

    while(cur2->next != nullptr){
        cur2 = cur2->next;
        n--;//计算head1于head2节点数之差
    }

    if(cur1 != cur2){
        return nullptr;
    } else{
        cur1 = n > 0 ? head1 : head2;//将cur1指向较长的链表
        cur2 = cur1 == head1 ? head2 : head1;

        //先让cur1走差数步
        while(n > 0){
            cur1 = cur1->next;
            n--;
        }

        while (cur1 != cur2){
            cur1 = cur1->next;
            cur2  =cur2->next;
        }
        return cur1;
    }
}

//判断两个有环链表是否相交，是则返回第一个相交点，否则返回null
ListNode* Loop(ListNode* head1, ListNode* head2, ListNode* node1, ListNode* node2){
    ListNode* cur1 = head1;
    ListNode* cur2 = head2;
    if(node1 == node2){
        int n = 0;
        while(cur1->next != node1){
            cur1 = cur1->next;
            n++;
        }
        while (cur2->next != node1){
            cur2 = cur2->next;
            n--;
        }

        if(cur1 != cur2){
            return node1;
        } else{
            cur1 = n > 0 ? head1 : head2;
            cur2 = cur1 != head1 ? head1 : head2;
            while (n > 0){
                cur1 = cur1->next;
                n--;
            }
            while (cur1 != cur2){
                cur1 = cur1->next;
                cur2 = cur2->next;
            }
            return cur1;
        }
    } else{
        cur1 = node1->next;
        while (cur1 != node1){
            if (cur1 == node2){
                return cur1;
            }
            cur1 = cur1->next;
        }
        return nullptr;
    }
}

//判断两个链表（分别有环或无环）是否相交，是则返回第一个相交节点，否则返回null
ListNode* question4(ListNode* head1, ListNode* head2){
    if(head1 == nullptr || head2 == nullptr){
        return nullptr;
    }

    ListNode* node1 = is_Loop(head1);
    ListNode* node2 = is_Loop(head2);

    if ((node1 == nullptr && node2 != nullptr) || (node2 == nullptr && node1 != nullptr)){
        cout<<"one null"<<endl;
        return nullptr;
    } else if(node1 == nullptr && node2 == nullptr){
        cout<<"null"<<endl;
        return no_Loop(head1, head2);
    } else{
        cout<<"loop"<<endl;
        return Loop(head1, head2, node1, node2);
    }
}

int main(){

//    randListNode* head = new randListNode(1);
//    randListNode* one = new randListNode(2);
//    randListNode* two = new randListNode(3);
//    randListNode* three = new randListNode(4);
//    randListNode* four = new randListNode(5);
//
//    head->next = one;
//    one->next = two;
//    two->next = three;
//    three->next = four;
//    four->next = nullptr;
//
//    head->rand = three;
//    one->rand = four;
//    two->rand = nullptr;
//    three->rand = two;
//    four->rand = one;
//
//    randListNode* out = question3(head);
//    randListNode* iter = out;
//    while(iter != nullptr){
//        cout<<iter->rand<<endl;
//        iter= iter->next;
//    }
//    while(out != nullptr){
//        cout<<out<<endl;
//        out = out->next;
//    }
    ListNode* one2 = new ListNode(1);
//    ListNode* two2 = new ListNode(2, one2);
//    ListNode* three2 = new ListNode(3, two2);
//    ListNode* four2 = new ListNode(4, three2);
//    ListNode* five2 = new ListNode(5, four2);
//    ListNode* six2 = new ListNode(6, five2);

    ListNode* one1 = new ListNode(1);
//    ListNode* two1 = new ListNode(2, one1);
//    ListNode* three1 = new ListNode(3, two1);

//    one2->next = five2;
//    one1->next = two1;

    cout<<question4(one1, one2)<<endl;
//    cout<<five2<<endl;


    return 0;
}