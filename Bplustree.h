#pragma once
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <queue>
#include<algorithm>
#include <windows.h> 
using namespace std;
#define M 3//число, m*2+1=значение; 
//вставьте полное заполнение сетки, удаление узла не может быть меньше, чем (M+1)/2, если zhi равно 5, не может быть меньше 2
//Дерево 5-го порядка, до 4 ключевых слов

class Inter_Node;




class Node
{
public:
    
    Node();
    virtual ~Node();
    Node* GetBrother(int& flag);
    Inter_Node* Parent;
    int key[M * 2];
    int count;
    int isLeaf;
    void Print(vector<pair<int, string>> key_words);
};


class Inter_Node : public Node
{
public:
    Inter_Node();
    virtual ~Inter_Node();
    bool Insert(int value, Node* pNode);
    bool Delete(int value);
    int Split(Inter_Node* pNode, int key);
    bool Merge(Inter_Node* pNode);
    bool Slib(Inter_Node* pNode);
    Node* Child[M * 2 + 1];
};


//листовой узел
class Leaf_Node : public Node
{
public:
    Leaf_Node();
    virtual ~Leaf_Node();
    bool Insert(int value);
    bool Delete(int value);
    int Split(Leaf_Node* pNode);
    bool Merge(Leaf_Node* pNode);
    Leaf_Node* Pre_Node;
    Leaf_Node* Next_Node;
};


//B+ Дерево
class Bplus
{
public:
    Bplus();
    virtual ~Bplus();
    bool Search(int data);
    bool Insert(int data);
    bool Delete(int data);
    void Print(vector<pair<int, string>> key_words);
    
    Leaf_Node* Find(int data);
    bool Add_Node(Inter_Node* pNode, int key, Node* New_Node);
    bool Remove_Node(Inter_Node* pNode, int key);
    Node* Root;
};


void Search(Bplus* bplus, string s);
void Add(Bplus* bplus, string s, vector<pair<int, string>>& key_words);
void Del(Bplus* bplus, string s);
void Print_tree(Bplus* bplus, vector<pair<int, string>> key_words);
void From_txt(Bplus* bplus, vector<pair<int, string>> &key_words);
unsigned int crc32(string str, unsigned int len);
