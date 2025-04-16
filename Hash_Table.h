#pragma once
#include<vector>
#include<string>
#include<iostream>
#include <iomanip> 
#include "Hash_Table.h"
#include <list> 
#include <iostream>
#include <fstream>

using namespace std;

class Hash_Table
{
private:
	int size;
	vector<pair<unsigned int, vector<string>>>	data;

	unsigned int crc32(string str, unsigned int len);
	bool Cheak_Elements(unsigned int hash);
	bool Cheak_In_Mass(vector<string> vector, string str);

public:
	Hash_Table();
	~Hash_Table();

	void Clear();
	void Add(string str);
	void Delete(string str);
	void Print();
	void Searc(string str);
	int Size();
	void To_txt();
	void From_txt();
};

