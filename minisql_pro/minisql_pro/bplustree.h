#pragma once
#ifndef _BPLUSTREE_H_
#define _BPLUSTREE_H_

#include <string>
#include <vector>

template <class Type>
struct val {
	Type key;
	int valNum;
	std::vector<int>ofs;
	std::vector<bool>eff;
};

template <class Type>
struct node {
	val<Type> leftVal;
	val<Type> rightVal;
	short totalNum;
	node<Type> *left;
	node<Type> *middle;
	node<Type> *right;
	node<Type> *parent;
	int num;
	int leftNum;
	int rightNum;
	int middleNum;
};

template <class Type>
class Tree {
private:
	node<Type> *root;
	void auxilary(node<Type> *n, const val<Type> vals);
	val<Type>* promote(const val<Type> a, const val<Type> b, const val<Type> c);
	node<Type>* splitNode(node<Type>* n, node<Type>* x, const int pos);
	void insertSecond(node<Type>* n, const val<Type> vals);
	bool isLeaf(node<Type>* n)const;
	int posChild(const node<Type>* parent, const node<Type>* child)const;
	node<Type>* init(const val<Type> vals);
	std::string filename;
	void doInorder(node<Type> *n)const;
public:
	node<Type>* getRoot()const;
	Tree(std::string filename);
	void insert(const val<Type> vals);
	bool search(node<Type>* n, Type key, std::vector<int>&ofs)const;
	void print(node<Type>* n)const;
	void printNode(node<Type>* n)const;
	void printInorder()const;
	node<Type>* find(node<Type>* n, Type key)const;
	void deleteKey(const Type key);
	void numberingBlock(node<Type> *n, int &countBlock)const;       //给块编号
	void setNumber(node<Type> *n, std::vector<node<Type> *> &nodes)const;        //得到块儿子们的编号
	void numberingVal(node<Type> *n, int &countVal)const;
	void getAllValue(node<Type> *n, std::vector<val<Type> > &vals)const;
	//void getLeaf(std::vector<val<Type> > &vals, std::vector<node<Type>* > &re)const;
};
#endif