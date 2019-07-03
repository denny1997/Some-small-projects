#pragma once

#include "bplustree.cpp"
#include "BufferManager.h"

#ifndef _INDEX_H_
#define _INDEX_H_

typedef enum{CHAR, INT, FLOAT}type;
typedef enum { SE, S, B, BE, EQ, NEQ }range;

class Index {
public:
	Index(std::string filename, type t);
	Index(std::string filename);
	void createIndex();
	void insertIndex(std::vector<std::string>attribute);
	void save()const;
	void search(std::vector<int> &re, std::string s1, range ra)const;
	void search(std::vector<int> &re, std::string s1, std::string s2, range ra1, range ra2)const;
	void dlt(const std::string key);
	void update(std::vector<std::string>attribute);
	type getType();

private:
	void InsertStringIndex(const int ofs, const std::string key);
	void InsertIntIndex(const int ofs, const int key);
	void InsertFloatIndex(const int ofs, const float key);

	void deleteStringNode(const std::string key, int numberBlock);
	void deleteStringLeaf(const std::string key, int numberBlock);
	void deleteIntNode(const int key, int numberBlock);
	void deleteIntLeaf(const int key, int numberBlock);
	void deleteFloatNode(const float key, int numberBlock);
	void deleteFloatLeaf(const float key, int numberBlock);

	void saveStringLeaf(const std::vector<val<string> > &valsS, const int &countS)const;
	void saveIntLeaf(const std::vector<val<int> > &valsI, const int &countI)const;
	void saveFloatLeaf(const std::vector<val<float> > &valsF, const int &countF)const;
	void saveStringNode(const std::vector<node<string> *> &nodes, const int &countS)const;
	void saveIntNode(const std::vector<node<int> *> &nodes, const int &countI)const;
	void saveFloatNode(const std::vector<node<float> *> &nodes, const int &countF)const;

	bool searchStringVal(std::vector<int> &re, std::string aim, int numberBlock)const;
	bool searchIntVal(std::vector<int> &re, int aim, int numberBlock)const;
	bool searchFloatVal(std::vector<int> &re, float aim, int numberBlock)const;
	void searchStringRange(std::vector<int> &re, std::string aim, range ra, int numberBlock)const;
	void searchIntRange(std::vector<int> &re, int aim, range ra, int numberBlock)const;
	void searchFloatRange(std::vector<int> &re, float aim, range ra, int numberBlock)const;

	Tree<int> *treeInt;
	Tree<float> *treeFloat;
	Tree<std::string> *treeString;
	std::string filename;
	type t;
	//std::string atr;
};
#endif


