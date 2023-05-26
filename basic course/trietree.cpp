/***********************************************************************
 * Copyright 2022 by Zhou Junping
 *
 * @file     trietree.cpp
 * @brief    这个文件用于本人学习数据结构与算法
 *
 * 该文件写的是一些关于前缀树的代码,包括：
 * 1.前缀树节点的表示
 * 2.前缀树的插入
 * 3.前缀树的搜索
 * 4.前缀树的前缀搜索
 * 5.前缀树的删除
 * 最近修改日期：2022-11-20
 *
 * @author   Zhou Junping
 * @email    zhoujunpingnn@gmail.com
 * @version  1.0
 * @data     2022-11-08
 *
 */

#include<iostream>
#include<string>
#include<cstring>

using namespace std;

struct trie_Node{
    int pass;
    int end;
    trie_Node* nexts[26];//如果字符数特别多，可以使用哈希表或者有序表

    trie_Node(){
        pass = 0;
        end = 0;
        // if nexts[i] = nullptr, there is no path from this node to nexts[i]
        for (auto& i : nexts) {
            i = nullptr;
        }
    }

    ~trie_Node(){
        for (int i = 0; i < 26; ++i) {
            delete nexts[i];
            nexts[i] = nullptr;
        }
    }
};

void delete_node(trie_Node * node);//释放该节点之后所有节点的内存
//
class trie_Tree{
public:
    trie_Tree(){
        root = new trie_Node;
    }

    ~trie_Tree(){
        delete root;
    }

    //插入string数组
    void insert(std::string * words, int size) {
        if (words == nullptr || size == 0) {
            return;
        }
        for (int i = 0; i < size; ++i) {
            insert(words[i]);
        }
    }

    //在树中插入一个单词word
    void insert(std::string word){
        char * arr = new char [word.size() + 1];
        strcpy(arr, word.c_str());
        trie_Node* cur_node = root;
        cur_node->pass += 1;
        int index = 0;
        for (int i = 0; i < word.size(); ++i) {
            index = arr[i] - 'a';
            if (cur_node->nexts[index] == nullptr) {
                cur_node->nexts[index] = new trie_Node;
            }
            cur_node = cur_node->nexts[index];
            cur_node->pass += 1;
        }
        cur_node->end += 1;
        delete[] arr;
    }

    void show(){
        trie_Node* cur_node = root;
        cout<<cur_node->pass;
    }

    //查询word单词在树中出现了几次
    int search(std::string word){
        if (word.size() == 0) {
            return root->end;//如果查的是一个空字符串，就返回root的end值
        }
        char * arr = new char [word.size() + 1];
        strcpy(arr, word.c_str());
        trie_Node * cur_node = root;
        int index;
        for (int i = 0; i < word.size(); ++i) {
            index = arr[i] - 'a';
            if (cur_node->nexts[index] == nullptr) {
                return 0;
            }
            cur_node = cur_node->nexts[index];
        }
        delete[] arr;
        return cur_node->end;
    }

    int prefix_search(std::string prefix){
        if (prefix.empty()) {
            return root->pass;//所有字符串都是以空字符串为前缀的
        }
        char * arr = new char [prefix.size() + 1];
        strcpy(arr, prefix.c_str());
        trie_Node * cur_node = root;
        int index;
        for (int i = 0; i < prefix.size(); ++i) {
            index = arr[i] - 'a';
            if (cur_node->nexts[index] == nullptr) {
                return 0;
            }
            cur_node = cur_node->nexts[index];
        }
        delete[] arr;
        return cur_node->pass;
    }

    //这里还有一种情况是pass为0，析构之后所有的节点
    void delete_string(std::string word){
        if (search(word) != 0) {
            char * arr = new char [word.size() + 1];
            strcpy(arr, word.c_str());
            trie_Node * cur_node = root;

            //如果root节点的pass为0了
            if (--cur_node->pass == 0) {
//                delete cur_node;
//                cur_node = nullptr;
//                root = nullptr;
                cout<<"root's pass is equal to 0."<<endl;
                return;
            }

            int index;
            for (int i = 0; i < word.size(); ++i) {
                index = arr[i] - 'a';
                if (--cur_node->nexts[i]->pass == 0) {
                    delete cur_node->nexts[i];
//                    delete_node(cur_node->nexts[i]);
                    cur_node->nexts[i] = nullptr;
                    return;
                }
                cur_node = cur_node->nexts[index];
            }
            cur_node->end--;
        }
    }

private:
    trie_Node* root;
};

////释放该节点之后所有节点的内存
////使用这个递归去删除指针的话需要在整个递归结束之后，在递归函数外部对头结点进行置空，因为函数传递的是形参
//void delete_node(trie_Node * node){
//    if (node == nullptr) {
//        return;
//    }
//    for (int i = 0; i < 26; ++i) {
//        delete_node(node->nexts[i]);
//        node->nexts[i] = nullptr;
//    }
//    delete node;
//}

//void test(int * p) {
//    * p = 1;
//    cout<<"test &p:"<<&p<<endl;
//    cout<<"test p:"<<p<<endl;
//    cout<<"test *p:"<<*p<<endl;
//    p = nullptr;
//    cout<<"after null p:"<<p<<endl;
//}

int main(){
    std::string word[3] = {std::string("abdc"), string("abcd"), string("acde")};
//    std::string word[3] = {"abdc", "abcd", "acde"};
    trie_Tree tree;
    tree.insert(word, 3);
    tree.delete_string(string("abcd"));
//    tree.delete_string(string("abdc"));
    int a= tree.search(string("absd"));
    int b= tree.search(string("abcd"));
    int c= tree.prefix_search(string("ab"));
    cout<<a<<endl;
    cout<<b<<endl;
    cout<<c<<endl;

//    trie_Node * root = new trie_Node;
//    root->nexts[1] = new trie_Node;
//    root->nexts[2] = new trie_Node;
//    root->nexts[3] = new trie_Node;
//    delete_node(root);
//    delete root;
//    root = nullptr;
//    cout<<root->nexts[1]<<endl;

//    int * p = new int;
//    p = nullptr;
//    delete p;
//    cout<<"..."<<endl;

//    int * p;
//    int b = 3;
//    p = &b;
//    test(p);
//    cout<<"main &p:"<<&p<<endl;
//    cout<<"main p:"<<p<<endl;
//    cout<<"main *p:"<<*p<<endl;

    return 0;
}