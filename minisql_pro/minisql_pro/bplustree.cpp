#include "bplustree.h"

#include <iostream>
#include <queue>
using namespace std;

template<class Type>
node<Type>* Tree<Type>::getRoot() const
{
	return root;
}

template <class Type>
Tree<Type>::Tree(string filename) :filename(filename), root(NULL)
{ }

template<class Type>
void Tree<Type>::insert(const val<Type> vals)  
{
	if (root == NULL)
		root = init(vals);
	else if (isLeaf(root)) {
		node<Type> *t = root;
		if (root->leftVal.key == vals.key) {
			int i = 0;
			for (; i < t->leftVal.ofs.size(); i++) {
				if (t->leftVal.eff.at(i) == false)
					break;
			}
			if (i < t->leftVal.ofs.size()) {
				t->leftVal.ofs.at(i) = vals.ofs.at(0);
				t->leftVal.eff.at(i) = true;
			}
			else {
				t->leftVal.ofs.push_back(vals.ofs.at(0));
				t->leftVal.eff.push_back(true);
			}
			return;
		}
		if (root->totalNum == 1) {
			insertSecond(root, vals);
		}
		else {
			if (root->rightVal.key == vals.key) {
				int i = 0;
				for (; i < t->rightVal.ofs.size(); i++) {
					if (t->rightVal.eff.at(i) == false)
						break;
				}
				if (i < t->rightVal.ofs.size()) {
					t->rightVal.ofs.at(i) = vals.ofs.at(0);
					t->rightVal.eff.at(i) = true;
				}
				else {
					t->rightVal.ofs.push_back(vals.ofs.at(0));
					t->rightVal.eff.push_back(true);
				}
				return;
			}else
				root = splitNode(root, init(vals), 0);
		}
	}

	else {
		node<Type>* t = find(root, vals.key);
		if (t == NULL)
			auxilary(root, vals);
		else {                                      //already exist
			if (t->leftVal.key == vals.key) {       //left
				int i = 0;
				for (; i < t->leftVal.ofs.size(); i++) {
					if (t->leftVal.eff.at(i) == false)
						break;
				}
				if (i < t->leftVal.ofs.size()) {
					t->leftVal.ofs.at(i) = vals.ofs.at(0);
					t->leftVal.eff.at(i) = true;
				}
				else {
					t->leftVal.ofs.push_back(vals.ofs.at(0));
					t->leftVal.eff.push_back(true);
				}
			}
			else {                                  //right
				int i = 0;
				for (; i < t->rightVal.ofs.size(); i++) {
					if (t->rightVal.eff.at(i) == false)
						break;
				}
				if (i < t->rightVal.ofs.size()) {
					t->rightVal.ofs.at(i) = vals.ofs.at(0);
					t->rightVal.eff.at(i) = true;
				}
				else {
					t->rightVal.ofs.push_back(vals.ofs.at(0));
					t->rightVal.eff.push_back(true);
				}
			}
		}
	}
}

template<class Type>
bool Tree<Type>::search(node<Type>* n, Type key, vector<int>&ofs) const
{
	node<Type>* t = find(n, key);
	if (t == NULL) {
		return false
	}
	else {
		if (key == t->leftVal.key && t->leftVal.eff) {
			ofs = t->leftVal.ofs;
			return true;
		}
			//return t->leftVal.ofs;
		else if (key == t->rightVal.key && t->rightVal.eff) {
			ofs = t->rightVal.ofs;
			return true;
		}
			//return t->rightVal.ofs;
		else
			return false;
	}
}

template<class Type>
void Tree<Type>::print(node<Type>* n) const
{
	vector<node<Type>*> now;
	vector<node<Type>*> child;
	now.push_back(n);
	while (now.empty() == false) {
		for (int i = 0; i < now.size(); i++) {
			printNode(now[i]);
		}
		for (int i = 0; i < now.size(); i++) {
			if (now[i]->left)
				child.push_back(now[i]->left);
			if (now[i]->middle)
				child.push_back(now[i]->middle);
			if (now[i]->right)
				child.push_back(now[i]->right);
		}
		now.clear();
		now.swap(child);
		child.clear();
	}
}

template<class Type>
void Tree<Type>::printNode(node<Type>* n) const
{
	if (n->totalNum == 1) {
		cout << "--1--  valnum:" << n->leftVal.valNum << " key:" << n->leftVal.key << endl;
	}
	else {
		cout << "--2--  valnum:" << n->leftVal.valNum << " key:" << n->leftVal.key << "   valnum" <<
			n->rightVal.valNum << " key:" << n->rightVal.key << endl;
	}
	
}

template<class Type>
void Tree<Type>::printInorder() const
{
	doInorder(root);
	cout << endl;
}

template<class Type>
node<Type>* Tree<Type>::find(node<Type>* n, Type key) const
{
	if (n == NULL)
		return NULL;
	if (n->totalNum == 1) {
		if (key < n->leftVal.key)
			return find(n->left, key);
		else if (key == n->leftVal.key)
			return n;
		else
			return find(n->right, key);
	}
	else if (n->totalNum == 2) {         //have two's
		if (key < n->leftVal.key)
			return find(n->left, key);
		else if (key == n->leftVal.key)
			return n;
		else if (key < n->rightVal.key)
			return find(n->middle, key);
		else if (key == n->rightVal.key)
			return n;
		else
			return find(n->right, key);
	}
}

template<class Type>
void Tree<Type>::deleteKey(const Type key)
{
	node<Type>* t = find(root, key);
	if (t == NULL)
		return;
	else if (t->leftVal.key == key) {
		for (int i = 0; i < t->leftVal.eff.size(); i++)
			t->leftVal.eff.at(i) = false;
	}
	else {
		for (int i = 0; i < t->rightVal.eff.size(); i++)
			t->rightVal.eff.at(i) = false;
	}
}

template<class Type>
void Tree<Type>::insertSecond(node<Type>* n, const val<Type> vals)
{
	if (vals.key < n->leftVal.key) {
		n->rightVal = n->leftVal;
		n->leftVal = vals;
	}
	else
		n->rightVal = vals;
	n->totalNum = 2;
}

template <class Type>
bool Tree<Type>::isLeaf(node<Type> *n)const
{
	if (!n->left && !n->middle && !n->right)
		return true;
	else
		return false;
}

template<class Type>
node<Type>* Tree<Type>::init(const val<Type> vals)
{
	node<Type>* newnode = new node<Type>;
	newnode->totalNum = 1;
	newnode->left = newnode->right = newnode->middle = NULL;
	newnode->leftVal = vals;
	return newnode;
}

template<class Type>
void Tree<Type>::doInorder(node<Type>* n) const
{
	if (root == NULL)
		return;
	else if (isLeaf(n))
		printNode(n);
	else {
		doInorder(n->left);

		cout << "--1--  valnum:" << n->leftVal.valNum << " key:" << n->leftVal.key << endl;

		if (n->middle) {
			doInorder(n->middle);
			cout << "--1--  valnum:" << n->rightVal.valNum << " key:" << n->rightVal.key << endl;

		}
		doInorder(n->right);
	}
}

template<class Type>
void Tree<Type>::numberingBlock(node<Type>* n, int &countBlock) const
{
	if (n == NULL)
		return;
	vector<node<Type>* > now;
	vector<node<Type>* > child;
	now.push_back(n);
	while (now.empty() == false) {
		for (int i = 0; i < now.size(); i++) {
			if (now[i] != NULL)
				now[i]->num = countBlock++;
		}
		for (int i = 0; i < now.size(); i++) {
			if (now[i]->left)
				child.push_back(now[i]->left);
			if (now[i]->middle)
				child.push_back(now[i]->middle);
			if (now[i]->right)
				child.push_back(now[i]->right);
		}
		now.clear();
		now.swap(child);
		child.clear();
	}
}

template<class Type>
void Tree<Type>::setNumber(node<Type>* n, std::vector<node<Type> *> &nodes) const
{
	if (n == NULL)
		return;
	queue<node<Type>*> q;
	q.push(n);
	while (q.empty() == false) {
		if (q.front() == NULL)
			break;
		if (q.front()->left) {
			q.front()->leftNum = q.front()->left->num;
			q.push(q.front()->left);
		}
		else {
			q.front()->leftNum = 0x80000000;
		}
		if (q.front()->middle) {
			q.front()->middleNum = q.front()->middle->num;
			q.push(q.front()->middle);
		}
		else {
			q.front()->middleNum = 0x80000000;
		}
		if (q.front()->right) {
			q.front()->rightNum = q.front()->right->num;
			q.push(q.front()->right);
		}
		else {
			q.front()->rightNum = 0x80000000;
		}
		nodes.push_back(q.front());
		q.pop();
	}
}

template<class Type>
void Tree<Type>::numberingVal(node<Type>* n, int & countVal) const
{
	if (n == NULL)
		return;
	else if (isLeaf(n)) {
		if (n->totalNum == 1) {
			n->leftVal.valNum = countVal++;
		}
		else {
			n->leftVal.valNum = countVal++;
			n->rightVal.valNum = countVal++;
		}
	}
	else {
		numberingVal(n->left, countVal);
		n->leftVal.valNum = countVal++;
		if (n->middle) {
			numberingVal(n->middle, countVal);
			n->rightVal.valNum = countVal++;
		}
		numberingVal(n->right, countVal);
	}
}

template<class Type>
void Tree<Type>::getAllValue(node<Type>* n, std::vector<val<Type> >& vals) const
{
	if (n == NULL)
		return;
	else if (isLeaf(n)) {
		if (n->totalNum == 1)
			vals.push_back(n->leftVal);
		else {
			vals.push_back(n->leftVal);
			vals.push_back(n->rightVal);
		}
	}
	else {
		getAllValue(n->left, vals);
		vals.push_back(n->leftVal);
		if (n->middle) {
			getAllValue(n->middle, vals);
			vals.push_back(n->rightVal);
		}
		getAllValue(n->right, vals);
	}
}

template<class Type>
val<Type> * Tree<Type>::promote(const val<Type> a, const val<Type> b, const val<Type> c)
{
	val<Type>* mi = new val<Type>[3];
	if (c.key > b.key) {
		mi[0] = a;    mi[1] = b;    mi[2] = c;
	}
	else if (c.key < a.key) {
		mi[0] = c;    mi[1] = a;    mi[2] = b;
	}
	else {
		mi[0] = a;    mi[1] = c;    mi[2] = b;
	}
	return mi;
}

template<class Type>
void Tree<Type>::auxilary(node<Type>* n, const val<Type> vals)
{
	if (isLeaf(n)) {
		if (n->totalNum == 1)
			insertSecond(n, vals);
		else {
			int pos = posChild(n->parent, n);
			node<Type> *pro = splitNode(n, init(vals), pos);
			while (n->parent != root) {
				if (n->parent->totalNum == 1) {
					insertSecond(n->parent, pro->leftVal);
					int pos = posChild(n->parent, n);
					if (pos == 0) {
						n->parent->left = pro->left;
						n->parent->middle = pro->right;
					}
					else if (pos == 2) {
						n->parent->middle = pro->left;
						n->parent->right = pro->right;
					}
					pro->left->parent = pro->right->parent = n->parent;
					break;
				}
				else {
					pos = posChild(n->parent, n);
					n = n->parent;
					pro = splitNode(n, pro, pos);
				}
			}
			if (n->parent == root) {
				pos = posChild(n->parent, n);
				if (n->parent->totalNum == 1) {
					insertSecond(n->parent, pro->leftVal);
					if (pos == 0) {
						n->parent->left = pro->left;
						n->parent->middle = pro->right;
					}
					else if (pos == 2) {
						n->parent->middle = pro->left;
						n->parent->right = pro->right;
					}
					pro->left->parent = pro->right->parent = n->parent;
				}
				else {
					pos = posChild(n->parent, n);
					n = n->parent;
					root = splitNode(n, pro, pos);
				}
			}
		}
	}
	else {
		if (n->totalNum == 1) {
			if (vals.key < n->leftVal.key)
				auxilary(n->left, vals);
			else
				auxilary(n->right, vals);
		}
		else {
			if (vals.key < n->leftVal.key)
				auxilary(n->left, vals);
			else if (vals.key > n->rightVal.key)
				auxilary(n->right, vals);
			else
				auxilary(n->middle, vals);
		}
	}
}

template<class Type>
node<Type>* Tree<Type>::splitNode(node<Type>* n, node<Type>* x, const int pos)
{
	node<Type>* cache[4];
	if (pos == 0) {
		cache[0] = x->left;
		cache[1] = x->right;
		cache[2] = n->middle;
		cache[3] = n->right;
	}
	else if (pos == 2) {
		cache[0] = n->left;
		cache[1] = n->middle;
		cache[2] = x->left;
		cache[3] = x->right;
	}
	else {
		cache[0] = n->left;
		cache[1] = x->left;
		cache[2] = x->right;
		cache[3] = n->right;
	}
	val<Type> *middle = promote(n->leftVal, n->rightVal, x->leftVal);
	node<Type> *pro = init(middle[1]);
	node<Type> *l = init(middle[0]);
	node<Type> *r = init(middle[2]);
	pro->left = l;
	pro->right = r;
	l->parent = r->parent = pro;
	if (cache[0] != NULL) {
		l->left = cache[0];
		l->right = cache[1];
		cache[0]->parent = cache[1]->parent = l;
		r->left = cache[2];
		r->right = cache[3];
		cache[2]->parent = cache[3]->parent = r;
	}
	return pro;
}

template<class Type>
int Tree<Type>::posChild(const node<Type>* parent, const node<Type>* child) const
{
	if (parent->left == child)
		return 0;
	else if (parent->middle == child)
		return 1;
	else
		return 2;
}

