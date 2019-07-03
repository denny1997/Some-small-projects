#include "index.h"
#include <algorithm>

int MaxBlocknum(string filename) {
	FILE* fp = fopen(filename.c_str(), "rb+");
	fseek(fp, 0, SEEK_END);

	int max_blocknum = ftell(fp) / BLOCK_LEN;//返回最大块序号
	fclose(fp);
	return max_blocknum;
}

Index::Index(std::string filename, type t): t(t), filename(filename)
{ }

Index::Index(std::string filename) : filename(filename)
{
	t = this->getType();
}

void Index::createIndex()               //new树根节点
{
	if (t == CHAR) {
		treeString = new Tree<std::string>(filename);
	}
	else if (t == INT) {
		treeInt = new Tree<int>(filename);
	}
	else if (t == FLOAT) {
		treeFloat = new Tree<float>(filename);
	}
}

void Index::insertIndex(std::vector<std::string>attribute)
{
	/*-------获得key 和ofs， 调用Insert####Index-------*/
	if (t == CHAR) {
		if (attribute.size() == 0)
			return;
		for (int i = 0; i < attribute.size(); i++) {
			InsertStringIndex(i, attribute.at(i));	
		}
	}
	else if (t == INT) {
		if (attribute.size() == 0)
			return;
		for (int i = 0; i < attribute.size(); i++) {
			InsertIntIndex(i, atoi(attribute[i].c_str()));
			//cout << "ofs" << i << " " << attribute[i] << endl;
		}
	}
	else if (t == FLOAT) {
		if (attribute.size() == 0)
			return;
		for (int i = 0; i < attribute.size(); i++) {
			InsertFloatIndex(i, atof(attribute[i].c_str()));
		}
	}
}

void Index::search(std::vector<int> &re, std::string s1, std::string s2, range ra1, range ra2) const
{
	if (t == CHAR) {
		vector<int>v1, v2;
		searchStringRange(v1, s1, ra1, 1);
		searchStringRange(v2, s2, ra2, 1);
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(re));
		v1.clear();
		v2.clear();
	}
	else if (t == INT) {
		vector<int>v1, v2;
		searchIntRange(v1, stoi(s1), ra1, 1);
		searchIntRange(v2, stoi(s2), ra2, 1);
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(re));
		v1.clear();
		v2.clear();
	}
	else {
		vector<int>v1, v2;
		searchFloatRange(v1, stof(s1), ra1, 1);
		searchFloatRange(v2, stof(s2), ra2, 1);
		sort(v1.begin(), v1.end());
		sort(v2.begin(), v2.end());
		set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(re));
		v1.clear();
		v2.clear();
	}
}

void Index::search(std::vector<int> &re, std::string s1, range ra1) const
{
	if (t == CHAR) {
		if (ra1 == EQ/* || ra1 == NEQ*/)
			searchStringVal(re, s1, 0);
		else
			searchStringRange(re, s1, ra1, 1);
	}
	else if (t == INT) {
		if (ra1 == EQ/* || ra1 == NEQ*/)
			searchIntVal(re, stoi(s1), 0);
		else
			searchIntRange(re, stoi(s1), ra1, 1);
	}
	else {
		if (ra1 == EQ/* || ra1 == NEQ*/)
			searchFloatVal(re, stof(s1), 0);
		else
			searchFloatRange(re, stof(s1), ra1, 1);
	}
}

void Index::dlt(const std::string key)
{
	switch (t)
	{
	case CHAR:
		deleteStringNode(key, 0);
		deleteStringLeaf(key, 1);
		break;
	case INT:
		deleteIntNode(stoi(key), 0);
		deleteIntLeaf(stoi(key), 1);
		break;
	case FLOAT:
		deleteFloatNode(stof(key), 0);
		deleteFloatLeaf(stof(key), 1);
		break;
	default:
		break;
	}

}

void Index::update(std::vector<std::string> attribute)
{
	this->createIndex();
	this->insertIndex(attribute);
	this->save();
}

type Index::getType()
{
	Buffer Bufferload;
	const string currentBlock = Bufferload.read_file_block("index_leaf" + filename, 0, INDEXFILE);
	int T = stoi(currentBlock.substr(0, 1));
	switch (T) {
	case CHAR:
		return CHAR;
	case INT:
		return INT;
	case FLOAT:
		return FLOAT;
	}
}

void Index::save() const
{
	if (t == CHAR) {
		std::vector<val<string> > valsS;
		std::vector<node<string>* >nodesS;
		int countBlock = 0;
		treeString->numberingBlock(treeString->getRoot(), countBlock);
		treeString->setNumber(treeString->getRoot(), nodesS);
		int countS = 0;
		treeString->numberingVal(treeString->getRoot(), countS);
		treeString->getAllValue(treeString->getRoot(), valsS);
		saveStringLeaf(valsS, countS);
		saveStringNode(nodesS, countBlock);
	}
	else if (t == INT) {
		std::vector<val<int> > valsI;
		std::vector<node<int> *> nodesI;
		int countBlock = 0;
		treeInt->numberingBlock(treeInt->getRoot(), countBlock);
		treeInt->setNumber(treeInt->getRoot(), nodesI);
		int countI = 0;
		treeInt->numberingVal(treeInt->getRoot(), countI);
		treeInt->getAllValue(treeInt->getRoot(), valsI);
		saveIntLeaf(valsI, countI);
		saveIntNode(nodesI, countBlock);
	}
	else if (t == FLOAT) {
		std::vector<val<float> > valsF;
		std::vector<node<float>* >nodesF;
		int countBlock = 0;
		treeFloat->numberingBlock(treeFloat->getRoot(), countBlock);
		treeFloat->setNumber(treeFloat->getRoot(), nodesF);
		int countF = 0;
		treeFloat->numberingVal(treeFloat->getRoot(), countF);
		treeFloat->getAllValue(treeFloat->getRoot(), valsF);
		saveFloatLeaf(valsF, countF);
		saveFloatNode(nodesF, countBlock);
	}
}



/*-------------------flag!key#编号#类型#几个指针#第一个#第二个-------------------*/
void Index::saveStringLeaf(const std::vector<val<string>>& valsS, const int &countS) const
{
	Buffer Bufferload;
	fileInfoPointer p = Bufferload.create_newfile("index_leaf" + filename, INDEXFILE, countS + 1);
	blockInfoPointer newblock = Bufferload.BlockFind();
	newblock->blockNum = 0;
	newblock->dirtyBit = DIRTY;
	string Type = to_string(t);
	//cout << Type << endl;
	strcpy(newblock->cBlock, Type.c_str());
	for (int j = Type.size(); j < BLOCK_LEN; j++)
		newblock->cBlock[j] = 0;
	fileInfoPointer file = Bufferload.get_file_info("index_leaf" + filename, INDEXFILE);  //申请文件进缓冲区
	Bufferload.replace(file, newblock);       //将新的块连接到文件
	for (int i = 0; i < countS; i++) {
		blockInfoPointer newblock = Bufferload.BlockFind();
		newblock->blockNum = i + 1;
		newblock->dirtyBit = DIRTY;
		string ss = "1!";
		ss += valsS[i].key + "#" + to_string(valsS[i].valNum) + "#" + to_string(CHAR) + "#";
		//cout << ss << " " << valsS[i].key << endl;
		int total = 0;
		for (int j = 0; j < valsS[i].eff.size(); j++) {
			if (valsS[i].eff[j])
				total++;
		}
		ss += to_string(total) + "#";
		for (int j = 0; j < valsS[i].eff.size(); j++) {
			if (valsS[i].eff[j]) {
				ss += to_string(valsS[i].ofs[j]) + "#";
			}
		}
		strcpy(newblock->cBlock, ss.c_str());
		for (int j = ss.size(); j < BLOCK_LEN; j++)
			newblock->cBlock[j] = 0;
		fileInfoPointer file = Bufferload.get_file_info("index_leaf" + filename, INDEXFILE);  //申请文件进缓冲区
		Bufferload.replace(file, newblock);       //将新的块连接到文件
	}
	Bufferload.close_file("index_leaf" + filename);
}

void Index::saveIntLeaf(const std::vector<val<int>>& valsI, const int &countI) const
{
	Buffer Bufferload;
	fileInfoPointer p = Bufferload.create_newfile("index_leaf" + filename, INDEXFILE, countI + 1);
	blockInfoPointer newblock = Bufferload.BlockFind();
	newblock->blockNum = 0;
	newblock->dirtyBit = DIRTY;
	string Type = to_string(t);
	strcpy(newblock->cBlock, Type.c_str());
	for (int j = Type.size(); j < BLOCK_LEN; j++)
		newblock->cBlock[j] = 0;
	fileInfoPointer file = Bufferload.get_file_info("index_leaf" + filename, INDEXFILE);  //申请文件进缓冲区
	Bufferload.replace(file, newblock);       //将新的块连接到文件
	for (int i = 0; i < countI; i++) {
		blockInfoPointer newblock = Bufferload.BlockFind();
		newblock->blockNum = i + 1;
		newblock->dirtyBit = DIRTY;
		string ss = "1!";
		ss += to_string(valsI[i].key) + "#" + to_string(valsI[i].valNum) + "#" + to_string(INT) + "#";
		int total = 0;
		for (int j = 0; j < valsI[i].eff.size(); j++) {
			if (valsI[i].eff[j])
				total++;
		}
		ss += to_string(total) + "#";
		for (int j = 0; j < valsI[i].eff.size(); j++) {
			if (valsI[i].eff[j]) {
				ss += to_string(valsI[i].ofs[j]) + "#";
			}
		}
		strcpy(newblock->cBlock, ss.c_str());
		for (int j = ss.size(); j < BLOCK_LEN; j++)
			newblock->cBlock[j] = 0;
		fileInfoPointer file = Bufferload.get_file_info("index_leaf" + filename, INDEXFILE);  //申请文件进缓冲区
		Bufferload.replace(file, newblock);       //将新的块连接到文件
	}
	Bufferload.close_file("index_leaf" + filename);
}

void Index::saveFloatLeaf(const std::vector<val<float>>& valsF, const int &countF) const
{
	Buffer Bufferload;
	fileInfoPointer p = Bufferload.create_newfile("index_leaf" + filename, INDEXFILE, countF + 1);
	blockInfoPointer newblock = Bufferload.BlockFind();
	newblock->blockNum = 0;
	newblock->dirtyBit = DIRTY;
	string Type = to_string(t);
	strcpy(newblock->cBlock, Type.c_str());
	for (int j = Type.size(); j < BLOCK_LEN; j++)
		newblock->cBlock[j] = 0;
	fileInfoPointer file = Bufferload.get_file_info("index_leaf" + filename, INDEXFILE);  //申请文件进缓冲区
	Bufferload.replace(file, newblock);       //将新的块连接到文件
	for (int i = 0; i < countF; i++) {
		blockInfoPointer newblock = Bufferload.BlockFind();
		newblock->blockNum = i + 1;
		newblock->dirtyBit = DIRTY;
		string ss = "1!";
		ss += to_string(valsF[i].key) + "#" + to_string(valsF[i].valNum) + "#" + to_string(FLOAT) + "#";
		int total = 0;
		for (int j = 0; j < valsF[i].eff.size(); j++) {
			if (valsF[i].eff[j])
				total++;
		}
		ss += to_string(total) + "#";
		for (int j = 0; j < valsF[i].eff.size(); j++) {
			if (valsF[i].eff[j]) {
				ss += to_string(valsF[i].ofs[j]) + "#";
			}
		}
		strcpy(newblock->cBlock, ss.c_str());
		for (int j = ss.size(); j < BLOCK_LEN; j++)
			newblock->cBlock[j] = 0;
		fileInfoPointer file = Bufferload.get_file_info("index_leaf" + filename, INDEXFILE);  //申请文件进缓冲区
		Bufferload.replace(file, newblock);       //将新的块连接到文件
	}
	Bufferload.close_file("index_leaf" + filename);
}


/*-----------    1   2    3       4    5       6      7*/
/*-----------编号#类型#left#middle#right#几个Key#(flag!key_1#几个指针#指针_1...#(flag!key_2#几个指针#指针_1#...------*/
void Index::saveStringNode(const std::vector<node<string>*>& nodes, const int & countS) const
{
	Buffer Bufferload;
	fileInfoPointer p = Bufferload.create_newfile("index_node" + filename, INDEXFILE, countS);
	for (int i = 0; i < countS; i++) {
		blockInfoPointer newblock = Bufferload.BlockFind();
		newblock->blockNum = i;
		newblock->dirtyBit = DIRTY;
		string ss;
		ss += to_string(nodes[i]->num) + "#" + to_string(CHAR) + "#" +
			to_string(nodes[i]->leftNum) + "#" + to_string(nodes[i]->middleNum) + "#" + to_string(nodes[i]->rightNum)
			+ "#" + to_string(nodes[i]->totalNum) + "#(1!";
		if (nodes[i]->totalNum == 1) {
			ss += nodes[i]->leftVal.key + "#";
			int total = 0;
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++)
				if (nodes[i]->leftVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++) {
				if (nodes[i]->leftVal.eff[j])
					ss += to_string(nodes[i]->leftVal.ofs[j]) + "#";
			}
		}
		else {
			ss += nodes[i]->leftVal.key + "#";
			int total = 0;
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++)
				if (nodes[i]->leftVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++) {
				if (nodes[i]->leftVal.eff[j])
					ss += to_string(nodes[i]->leftVal.ofs[j]) + "#";
			}
			ss += "(1!";
			ss += nodes[i]->rightVal.key + "#";
			total = 0;
			for (int j = 0; j < nodes[i]->rightVal.eff.size(); j++)
				if (nodes[i]->rightVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->rightVal.eff.size(); j++) {
				if (nodes[i]->rightVal.eff[j])
					ss += to_string(nodes[i]->rightVal.ofs[j]) + "#";
			}
		}
		strcpy(newblock->cBlock, ss.c_str());
		for (int j = ss.size(); j < BLOCK_LEN; j++)
			newblock->cBlock[j] = 0;
		fileInfoPointer file = Bufferload.get_file_info("index_node" + filename, INDEXFILE);  //申请文件进缓冲区
		Bufferload.replace(file, newblock);
	}
	Bufferload.close_file("index_node" + filename);
}

void Index::saveIntNode(const std::vector<node<int>*>& nodes, const int & countI) const
{
	Buffer Bufferload;
	fileInfoPointer p = Bufferload.create_newfile("index_node" + filename, INDEXFILE, countI);
	for (int i = 0; i < countI; i++) {
		blockInfoPointer newblock = Bufferload.BlockFind();
		newblock->blockNum = i;
		newblock->dirtyBit = DIRTY;
		string ss;
		ss += to_string(nodes[i]->num) + "#" + to_string(INT) + "#" +
			to_string(nodes[i]->leftNum) + "#" + to_string(nodes[i]->middleNum) + "#" + to_string(nodes[i]->rightNum)
			+ "#" + to_string(nodes[i]->totalNum) + "#(1!";
		if (nodes[i]->totalNum == 1) {
			ss += to_string(nodes[i]->leftVal.key) + "#";
			int total = 0;
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++)
				if (nodes[i]->leftVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++) {
				if (nodes[i]->leftVal.eff[j])
					ss += to_string(nodes[i]->leftVal.ofs[j]) + "#";
			}
		}
		else {
			ss += to_string(nodes[i]->leftVal.key) + "#";
			int total = 0;
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++)
				if (nodes[i]->leftVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++) {
				if (nodes[i]->leftVal.eff[j])
					ss += to_string(nodes[i]->leftVal.ofs[j]) + "#";
			}
			ss += "(1!";
			ss += to_string(nodes[i]->rightVal.key) + "#";
			total = 0;
			for (int j = 0; j < nodes[i]->rightVal.eff.size(); j++)
				if (nodes[i]->rightVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->rightVal.eff.size(); j++) {
				if (nodes[i]->rightVal.eff[j])
					ss += to_string(nodes[i]->rightVal.ofs[j]) + "#";
			}
		}
		strcpy(newblock->cBlock, ss.c_str());
		for (int j = ss.size(); j < BLOCK_LEN; j++)
			newblock->cBlock[j] = 0;
		fileInfoPointer file = Bufferload.get_file_info("index_node" + filename, INDEXFILE);  //申请文件进缓冲区
		Bufferload.replace(file, newblock);
	}
	Bufferload.close_file("index_node" + filename);
}

void Index::saveFloatNode(const std::vector<node<float>*>& nodes, const int & countF) const
{
	Buffer Bufferload;
	fileInfoPointer p = Bufferload.create_newfile("index_node" + filename, INDEXFILE, countF);
	for (int i = 0; i < countF; i++) {
		blockInfoPointer newblock = Bufferload.BlockFind();
		newblock->blockNum = i;
		newblock->dirtyBit = DIRTY;
		string ss;
		ss += to_string(nodes[i]->num) + "#" + to_string(INT) + "#" +
			to_string(nodes[i]->leftNum) + "#" + to_string(nodes[i]->middleNum) + "#" + to_string(nodes[i]->rightNum)
			+ "#" + to_string(nodes[i]->totalNum) + "#(1!";
		if (nodes[i]->totalNum == 1) {
			ss += to_string(nodes[i]->leftVal.key) + "#";
			int total = 0;
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++)
				if (nodes[i]->leftVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++) {
				if (nodes[i]->leftVal.eff[j])
					ss += to_string(nodes[i]->leftVal.ofs[j]) + "#";
			}
		}
		else {
			ss += to_string(nodes[i]->leftVal.key) + "#";
			int total = 0;
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++)
				if (nodes[i]->leftVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->leftVal.eff.size(); j++) {
				if (nodes[i]->leftVal.eff[j])
					ss += to_string(nodes[i]->leftVal.ofs[j]) + "#";
			}
			ss += "(1!";
			ss += to_string(nodes[i]->rightVal.key) + "#";
			total = 0;
			for (int j = 0; j < nodes[i]->rightVal.eff.size(); j++)
				if (nodes[i]->rightVal.eff[j])
					total++;
			ss += to_string(total) + "#";
			for (int j = 0; j < nodes[i]->rightVal.eff.size(); j++) {
				if (nodes[i]->rightVal.eff[j])
					ss += to_string(nodes[i]->rightVal.ofs[j]) + "#";
			}
		}
		strcpy(newblock->cBlock, ss.c_str());
		for (int j = ss.size(); j < BLOCK_LEN; j++)
			newblock->cBlock[j] = 0;
		fileInfoPointer file = Bufferload.get_file_info("index_node" + filename, INDEXFILE);  //申请文件进缓冲区
		Bufferload.replace(file, newblock);
	}
	Bufferload.close_file("index_node" + filename);
}

/*-----------         1   2    3       4    5       6      7*/
/*-----------编号#类型#left#middle#right#几个Key#(flag!key_1#几个指针#指针_1...#(flag!key_2#几个指针#指针_1#...------*/
bool Index::searchStringVal(std::vector<int>& re, std::string aim, int numberBlock)const
{
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return false;
	Buffer Bufferload;
	const string currentBlock = Bufferload.read_file_block("index_node" + filename, numberBlock, INDEXFILE);
	int s_2 = currentBlock.find('#', currentBlock.find('#') + 1);//left前
	int s_3 = currentBlock.find('#', s_2 + 1); //middle前
	int s_4 = currentBlock.find('#', s_3 + 1); //right前
	int s_5 = currentBlock.find('#', s_4 + 1); //几个Key前
	int leftP = stoi(currentBlock.substr(s_2 + 1, s_3 - s_2 - 1));
	int middleP = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	int rightP = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
	int f_1 = currentBlock.find('(');                   //!!!!!
	int b_1 = currentBlock.find('!');          //key1前  !!!!
	int flag_1 = stoi(currentBlock.substr(f_1 + 1, b_1 - f_1 - 1));
	int b_2 = currentBlock.find('(', b_1 + 1);
	int k_1 = currentBlock.find('#', b_1 + 1);
	int k_2 = currentBlock.find('#', k_1 + 1);  //指针_1前
	string key_1 = currentBlock.substr(b_1 + 1, k_1 - b_1 - 1);
	if (key_1 == aim) {
		if (flag_1 == 0)
			return false;
		int numberOfOfs = stoi(currentBlock.substr(k_1 + 1, k_2 - k_1 - 1));
		for (int i = 0; i < numberOfOfs; i++) {
			int k_3 = currentBlock.find('#', k_2 + 1);
			int ofs = stoi(currentBlock.substr(k_2 + 1, k_3 - k_2 - 1));
			re.push_back(ofs);
			k_2 = k_3;
		}
		return true;
	}
	if (b_2 == std::string::npos) {                 //only one key
		if (key_1 > aim)
			return searchStringVal(re, aim, leftP);
		else
			return searchStringVal(re, aim, rightP);
	}
	else {
		int kk_1 = currentBlock.find('#', b_2 + 1);
		int kk_2 = currentBlock.find('#', kk_1 + 1);  //指针_1前
		int f_2 = currentBlock.find('!', b_2);
		int flag = stoi(currentBlock.substr(b_2 + 1, f_2 - b_2 - 1));
		string key_2 = currentBlock.substr(f_2 + 1, kk_1 - f_2 - 1);
		if (key_2 == aim) {
			if (flag == 0)
				return false;
			int numberOfOfs = stoi(currentBlock.substr(kk_1 + 1, kk_2 - kk_1 - 1));
			for (int i = 0; i < numberOfOfs; i++) {
				int kk_3 = currentBlock.find('#', kk_2 + 1);
				int ofs = stoi(currentBlock.substr(kk_2 + 1, kk_3 - kk_2 - 1));
				re.push_back(ofs);
				kk_2 = kk_3;
			}
			return true;
		}
		else if (aim < key_1)
			return searchStringVal(re, aim, leftP);
		else if (aim < key_2)
			return searchStringVal(re, aim, middleP);
		else
			return searchStringVal(re, aim, rightP);
	}
	return false;
}

bool Index::searchIntVal(std::vector<int>& re, int aim, int numberBlock)const
{
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return false ;
	Buffer Bufferload;
	const string currentBlock = Bufferload.read_file_block("index_node" + filename, numberBlock, INDEXFILE);
	int s_2 = currentBlock.find('#', currentBlock.find('#') + 1);//left前
	int s_3 = currentBlock.find('#', s_2 + 1); //middle前
	int s_4 = currentBlock.find('#', s_3 + 1); //right前
	int s_5 = currentBlock.find('#', s_4 + 1); //几个Key前
	int leftP = stoi(currentBlock.substr(s_2 + 1, s_3 - s_2 - 1));
	int middleP = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	int rightP = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
	int f_1 = currentBlock.find('(');                   //!!!!!
	int b_1 = currentBlock.find('!');          //key1前  !!!!
	int flag_1 = stoi(currentBlock.substr(f_1 + 1, b_1 - f_1 - 1));
	int b_2 = currentBlock.find('(', b_1 + 1);
	int k_1 = currentBlock.find('#', b_1 + 1);
	int k_2 = currentBlock.find('#', k_1 + 1);  //指针_1前
	int key_1 = stoi(currentBlock.substr(b_1 + 1, k_1 - b_1 - 1));
	if (key_1 == aim) {
		if (flag_1 == 0)
			return false;
		int numberOfOfs = stoi(currentBlock.substr(k_1 + 1, k_2 - k_1 - 1));
		for (int i = 0; i < numberOfOfs; i++) {
			int k_3 = currentBlock.find('#', k_2 + 1);
			int ofs = stoi(currentBlock.substr(k_2 + 1, k_3 - k_2 - 1));
			re.push_back(ofs);
			k_2 = k_3;
		}
		return true;
	}
	if (b_2 == std::string::npos) {                 //only one key
		if (key_1 > aim)
			return searchIntVal(re, aim, leftP);
		else
			return searchIntVal(re, aim, rightP);
	}
	else {
		int kk_1 = currentBlock.find('#', b_2 + 1);
		int kk_2 = currentBlock.find('#', kk_1 + 1);  //指针_1前
		int f_2 = currentBlock.find('!', b_2);
		int flag = stoi(currentBlock.substr(b_2 + 1, f_2 - b_2 - 1));
		int key_2 = stoi(currentBlock.substr(f_2 + 1, kk_1 - f_2 - 1));
		if (key_2 == aim) {
			if (flag == 0)
				return false;
			int numberOfOfs = stoi(currentBlock.substr(kk_1 + 1, kk_2 - kk_1 - 1));
			for (int i = 0; i < numberOfOfs; i++) {
				int kk_3 = currentBlock.find('#', kk_2 + 1);
				int ofs = stoi(currentBlock.substr(kk_2 + 1, kk_3 - kk_2 - 1));
				re.push_back(ofs);
				kk_2 = kk_3;
			}
			return true;
		}
		else if (aim < key_1)
			return searchIntVal(re, aim, leftP);
		else if (aim < key_2)
			return searchIntVal(re, aim, middleP);
		else
			return searchIntVal(re, aim, rightP);
	}
	return false;
}

bool Index::searchFloatVal(std::vector<int>& re, float aim, int numberBlock)const
{
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return false;
	Buffer Bufferload;
	const string currentBlock = Bufferload.read_file_block("index_node" + filename, numberBlock, INDEXFILE);
	int s_2 = currentBlock.find('#', currentBlock.find('#') + 1);//left前
	int s_3 = currentBlock.find('#', s_2 + 1); //middle前
	int s_4 = currentBlock.find('#', s_3 + 1); //right前
	int s_5 = currentBlock.find('#', s_4 + 1); //几个Key前
	int leftP = stoi(currentBlock.substr(s_2 + 1, s_3 - s_2 - 1));
	int middleP = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	int rightP = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
	int f_1 = currentBlock.find('(');                   //!!!!!
	int b_1 = currentBlock.find('!');          //key1前  !!!!
	int flag_1 = stoi(currentBlock.substr(f_1 + 1, b_1 - f_1 - 1));
	int b_2 = currentBlock.find('(', b_1 + 1);
	int k_1 = currentBlock.find('#', b_1 + 1);
	int k_2 = currentBlock.find('#', k_1 + 1);  //指针_1前
	float key_1 = stof(currentBlock.substr(b_1 + 1, k_1 - b_1 - 1));
	if (key_1 == aim) {
		if (flag_1 == 0)
			return false;
		int numberOfOfs = stoi(currentBlock.substr(k_1 + 1, k_2 - k_1 - 1));
		for (int i = 0; i < numberOfOfs; i++) {
			int k_3 = currentBlock.find('#', k_2 + 1);
			int ofs = stoi(currentBlock.substr(k_2 + 1, k_3 - k_2 - 1));
			re.push_back(ofs);
			k_2 = k_3;
		}
		return true;
	}
	if (b_2 == std::string::npos) {                 //only one key
		if (key_1 > aim)
			return searchFloatVal(re, aim, leftP);
		else
			return searchFloatVal(re, aim, rightP);
	}
	else {
		int kk_1 = currentBlock.find('#', b_2 + 1);
		int kk_2 = currentBlock.find('#', kk_1 + 1);  //指针_1前
		int f_2 = currentBlock.find('!', b_2);
		int flag = stoi(currentBlock.substr(b_2 + 1, f_2 - b_2 - 1));
		float key_2 = stof(currentBlock.substr(f_2 + 1, kk_1 - b_2 - 1));
		if (key_2 == aim) {
			if (flag == 0)
				return false;
			int numberOfOfs = stoi(currentBlock.substr(kk_1 + 1, kk_2 - kk_1 - 1));
			for (int i = 0; i < numberOfOfs; i++) {
				int kk_3 = currentBlock.find('#', kk_2 + 1);
				int ofs = stoi(currentBlock.substr(kk_2 + 1, kk_3 - kk_2 - 1));
				re.push_back(ofs);
				kk_2 = kk_3;
			}
			return true;
		}
		else if (aim < key_1)
			return searchFloatVal(re, aim, leftP);
		else if (aim < key_2)
			return searchFloatVal(re, aim, middleP);
		else
			return searchFloatVal(re, aim, rightP);
	}
	return false;
}

/*                      1    2   3        4*/
/*-------------------key#编号#类型#几个指针#第一个#第二个-------------------*/
void Index::searchStringRange(std::vector<int>& re, std::string aim, range ra, int numberBlock) const
{
	Buffer Bufferload;
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return;
	const string currentBlock = Bufferload.read_file_block("index_leaf" + filename, numberBlock, INDEXFILE);
	if (currentBlock[0] == '0') {
		searchStringRange(re, aim, ra, numberBlock + 1);
		return;
	}
	int s_1 = currentBlock.find('#');	
	string key = currentBlock.substr(2, s_1 - 2);		
	int s_2 = currentBlock.find('#', s_1 + 1);
	int s_3 = currentBlock.find('#', s_2 + 1);
	int s_4 = currentBlock.find('#', s_3 + 1);
	int number = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	if (key < aim) {
		if (ra == SE || ra == S || ra == NEQ) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
		}
		searchStringRange(re, aim, ra, numberBlock + 1);
	}
	else if (key == aim) {
		if (ra == BE || ra==SE) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
		}

		switch (ra) {
		case S:
		case SE:
			return;
		case B:
		case BE:
		case NEQ:
			searchStringRange(re, aim, ra, numberBlock + 1);
			break;
		}


	}
	else {
		if (ra == BE || ra == B || ra == NEQ) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
			searchStringRange(re, aim, ra, numberBlock + 1);
		}
		else
			return;
	}
}

void Index::searchIntRange(std::vector<int>& re, int aim, range ra, int numberBlock) const
{
	Buffer Bufferload;
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return;
	const string currentBlock = Bufferload.read_file_block("index_leaf" + filename, numberBlock, INDEXFILE);
	if (currentBlock[0] == '0') {
		searchIntRange(re, aim, ra, numberBlock + 1);
		return;
	}
	int s_1 = currentBlock.find('#');
	int key = stoi(currentBlock.substr(2, s_1 - 2));
	int s_2 = currentBlock.find('#', s_1 + 1);
	int s_3 = currentBlock.find('#', s_2 + 1);
	int s_4 = currentBlock.find('#', s_3 + 1);
	int number = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	if (key < aim) {
		if (ra == SE || ra == S || ra == NEQ) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
		}
		searchIntRange(re, aim, ra, numberBlock + 1);
	}
	else if (key == aim) {
		if (ra == BE || ra == SE) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
		}
		switch (ra) {
		case S:
		case SE:
			return;
		case B:
		case BE:
		case NEQ:
			searchIntRange(re, aim, ra, numberBlock + 1);
			break;
		}
	}
	else {
		if (ra == BE || ra == B || ra == NEQ) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
			searchIntRange(re, aim, ra, numberBlock + 1);
		}
		else
			return;
	}
}

void Index::searchFloatRange(std::vector<int>& re, float aim, range ra, int numberBlock) const
{
	Buffer Bufferload;
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return;
	const string currentBlock = Bufferload.read_file_block("index_leaf" + filename, numberBlock, INDEXFILE);
	if (currentBlock[0] == '0') {
		searchFloatRange(re, aim, ra, numberBlock + 1);
		return;
	}
	int s_1 = currentBlock.find('#');
	float key = stof(currentBlock.substr(2, s_1 - 2));
	int s_2 = currentBlock.find('#', s_1 + 1);
	int s_3 = currentBlock.find('#', s_2 + 1);
	int s_4 = currentBlock.find('#', s_3 + 1);
	int number = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	if (key < aim) {
		if (ra == SE || ra == S || ra == NEQ) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
		}
		searchFloatRange(re, aim, ra, numberBlock + 1);
	}
	else if (key == aim) {
		if (ra == BE || ra == SE) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
		}
		switch (ra) {
		case S:
		case SE:
			return;
		case B:
		case BE:
		case NEQ:
			searchFloatRange(re, aim, ra, numberBlock + 1);
			break;
		}
	}
	else {
		if (ra == BE || ra == B || ra == NEQ) {
			for (int i = 0; i < number; i++) {
				int s_5 = currentBlock.find('#', s_4 + 1);
				int ofs = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));
				re.push_back(ofs);
				s_4 = s_5;
			}
			searchFloatRange(re, aim, ra, numberBlock + 1);
		}
		else
			return;
	}
}




void Index::InsertStringIndex(const int ofs, const std::string key)
{
	val<std::string>a;
	a.key = key;
	a.ofs.push_back(ofs);
	a.eff.push_back(true);
	treeString->insert(a);
}

void Index::InsertIntIndex(const int ofs, const int key)
{
	val<int>a;
	a.key = key;
	a.ofs.push_back(ofs);
	a.eff.push_back(true);
	treeInt->insert(a);
}

void Index::InsertFloatIndex(const int ofs, const float key)
{
	val<float>a;
	a.key = key;
	a.ofs.push_back(ofs);
	a.eff.push_back(true);
	treeFloat->insert(a);
}


void Index::deleteStringNode(const std::string key, int numberBlock)
{
	Buffer Bufferload;
	//string currentBlock = Bufferload.write_file_block("index_node" + filename, numberBlock, INDEXFILE);
	char * cb = Bufferload.write_file_block("index_node" + filename, numberBlock, INDEXFILE);
	string currentBlock = cb;
	int s_2 = currentBlock.find('#', currentBlock.find('#') + 1);//left前
	int s_3 = currentBlock.find('#', s_2 + 1); //middle前
	int s_4 = currentBlock.find('#', s_3 + 1); //right前
	int s_5 = currentBlock.find('#', s_4 + 1); //几个Key前
	int f_1 = currentBlock.find('(');                   //!!!!!
	int b_1 = currentBlock.find('!');          //key1前  !!!!
	int flag_1 = stoi(currentBlock.substr(f_1 + 1, b_1 - f_1 - 1));
	int b_2 = currentBlock.find('(', b_1 + 1);
	int k_1 = currentBlock.find('#', b_1 + 1);
	int k_2 = currentBlock.find('#', k_1 + 1);  //指针_1前

	int leftP = stoi(currentBlock.substr(s_2 + 1, s_3 - s_2 - 1));
	int middleP = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	int rightP = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));

	string key_1 = currentBlock.substr(b_1 + 1, k_1 - b_1 - 1);
	if (key_1 == key) {
		currentBlock[f_1 + 1] = '0';
		strcpy(cb, currentBlock.c_str());
		Bufferload.close_file("index_node" + filename);
		return;
	}
	if (b_2 == std::string::npos) {
		if (key_1 > key)
			deleteStringNode(key, leftP);
		else
			deleteStringNode(key, rightP);
	}
	else {
		int kk_1 = currentBlock.find('#', b_2 + 1);
		int kk_2 = currentBlock.find('#', kk_1 + 1);  //指针_1前
		int f_2 = currentBlock.find('!', b_2);
		int flag = stoi(currentBlock.substr(b_2 + 1, f_2 - b_2 - 1));
		string key_2 = currentBlock.substr(f_2 + 1, kk_1 - b_2 - 1);
		if (key_2 == key) {
			currentBlock[b_2 + 1] = '0';
			strcpy(cb, currentBlock.c_str());
			Bufferload.close_file("index_node" + filename);
			return;
		}
		else if (key < key_1)
			deleteStringNode(key, leftP);
		else if (key < key_2)
			deleteStringNode(key, middleP);
		else
			deleteStringNode(key, rightP);
	}
}

void Index::deleteStringLeaf(const std::string key, int numberBlock)
{
	Buffer Bufferload;
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return;
	char * cb = Bufferload.write_file_block("index_node" + filename, numberBlock, INDEXFILE);
	string currentBlock = cb;
	int s_1 = currentBlock.find('#');
	string keyL = currentBlock.substr(2, s_1);
	if (keyL == key)
	{
		currentBlock[0] = '0';
		strcpy(cb, currentBlock.c_str());
		Bufferload.close_file("index_leaf" + filename);
	}
	else
		deleteStringLeaf(key, numberBlock + 1);
}

void Index::deleteIntNode(const int key, int numberBlock)
{
	Buffer Bufferload;
	char * cb = Bufferload.write_file_block("index_node" + filename, numberBlock, INDEXFILE);
	string currentBlock = cb;
	int s_2 = currentBlock.find('#', currentBlock.find('#') + 1);//left前
	int s_3 = currentBlock.find('#', s_2 + 1); //middle前
	int s_4 = currentBlock.find('#', s_3 + 1); //right前
	int s_5 = currentBlock.find('#', s_4 + 1); //几个Key前
	int f_1 = currentBlock.find('(');                   //!!!!!
	int b_1 = currentBlock.find('!');          //key1前  !!!!
	int flag_1 = stoi(currentBlock.substr(f_1 + 1, b_1 - f_1 - 1));
	int b_2 = currentBlock.find('(', b_1 + 1);
	int k_1 = currentBlock.find('#', b_1 + 1);
	int k_2 = currentBlock.find('#', k_1 + 1);  //指针_1前

	int leftP = stoi(currentBlock.substr(s_2 + 1, s_3 - s_2 - 1));
	int middleP = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	int rightP = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));

	int key_1 = stoi(currentBlock.substr(b_1 + 1, k_1 - b_1 - 1));
	if (key_1 == key) {
		currentBlock[f_1 + 1] = '0';
		strcpy(cb, currentBlock.c_str());
		//cout << "node " << key << endl;
		Bufferload.close_file("index_node" + filename);
		return;
	}
	if (b_2 == std::string::npos) {
		if (key_1 > key)
			deleteIntNode(key, leftP);
		else
			deleteIntNode(key, rightP);
	}
	else {
		int kk_1 = currentBlock.find('#', b_2 + 1);
		int kk_2 = currentBlock.find('#', kk_1 + 1);  //指针_1前
		int f_2 = currentBlock.find('!', b_2);
		int flag = stoi(currentBlock.substr(b_2 + 1, f_2 - b_2 - 1));
		int key_2 = stoi(currentBlock.substr(f_2 + 1, kk_1 - b_2 - 1));
		if (key_2 == key) {
			currentBlock[b_2 + 1] = '0';
			//cout << "node " << key << endl;
			strcpy(cb, currentBlock.c_str());
			Bufferload.close_file("index_node" + filename);
			return;
		}
		else if (key < key_1)
			deleteIntNode(key, leftP);
		else if (key < key_2)
			deleteIntNode(key, middleP);
		else
			deleteIntNode(key, rightP);
	}
}

void Index::deleteIntLeaf(const int key, int numberBlock)
{
	Buffer Bufferload;
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return;
	char * cb = Bufferload.write_file_block("index_leaf" + filename, numberBlock, INDEXFILE);
	string currentBlock = cb;
	int s_1 = currentBlock.find('#');
	int keyL = stoi(currentBlock.substr(2, s_1));
	if (keyL == key)
	{
		currentBlock[0] = '0';
		//cout << "leaf " << key << endl;
		strcpy(cb, currentBlock.c_str());
		Bufferload.close_file("index_leaf" + filename);
	}
	else
		deleteIntLeaf(key, numberBlock + 1);
}

void Index::deleteFloatNode(const float key, int numberBlock)
{
	Buffer Bufferload;
	//string currentBlock = Bufferload.write_file_block("index_node" + filename, numberBlock, INDEXFILE);
	char * cb = Bufferload.write_file_block("index_node" + filename, numberBlock, INDEXFILE);
	string currentBlock = cb;
	int s_2 = currentBlock.find('#', currentBlock.find('#') + 1);//left前
	int s_3 = currentBlock.find('#', s_2 + 1); //middle前
	int s_4 = currentBlock.find('#', s_3 + 1); //right前
	int s_5 = currentBlock.find('#', s_4 + 1); //几个Key前
	int f_1 = currentBlock.find('(');                   //!!!!!
	int b_1 = currentBlock.find('!');          //key1前  !!!!
	int flag_1 = stoi(currentBlock.substr(f_1 + 1, b_1 - f_1 - 1));
	int b_2 = currentBlock.find('(', b_1 + 1);
	int k_1 = currentBlock.find('#', b_1 + 1);
	int k_2 = currentBlock.find('#', k_1 + 1);  //指针_1前

	int leftP = stoi(currentBlock.substr(s_2 + 1, s_3 - s_2 - 1));
	int middleP = stoi(currentBlock.substr(s_3 + 1, s_4 - s_3 - 1));
	int rightP = stoi(currentBlock.substr(s_4 + 1, s_5 - s_4 - 1));

	float key_1 = stof(currentBlock.substr(b_1 + 1, k_1 - b_1 - 1));
	if (key_1 == key) {
		currentBlock[f_1 + 1] = '0';
		strcpy(cb, currentBlock.c_str());
		Bufferload.close_file("index_node" + filename);
		return;
	}
	if (b_2 == std::string::npos) {
		if (key_1 > key)
			deleteFloatNode(key, leftP);
		else
			deleteFloatNode(key, rightP);
	}
	else {
		int kk_1 = currentBlock.find('#', b_2 + 1);
		int kk_2 = currentBlock.find('#', kk_1 + 1);  //指针_1前
		int f_2 = currentBlock.find('!', b_2);
		int flag = stoi(currentBlock.substr(b_2 + 1, f_2 - b_2 - 1));
		float key_2 = stof(currentBlock.substr(f_2 + 1, kk_1 - b_2 - 1));
		if (key_2 == key) {
			currentBlock[b_2 + 1] = '0';
			strcpy(cb, currentBlock.c_str());
			Bufferload.close_file("index_node" + filename);
			return;
		}
		else if (key < key_1)
			deleteFloatNode(key, leftP);
		else if (key < key_2)
			deleteFloatNode(key, middleP);
		else
			deleteFloatNode(key, rightP);
	}
}

void Index::deleteFloatLeaf(const float key, int numberBlock)
{
	Buffer Bufferload;
	if (numberBlock == MaxBlocknum("index_leaf" + filename))
		return;
	char * cb = Bufferload.write_file_block("index_leaf" + filename, numberBlock, INDEXFILE);
	string currentBlock = cb;
	int s_1 = currentBlock.find('#');
	float keyL = stof(currentBlock.substr(2, s_1));
	if (keyL == key)
	{
		currentBlock[0] = '0';
		//cout << "leaf " << key << endl;
		strcpy(cb, currentBlock.c_str());
		Bufferload.close_file("index_leaf" + filename);
	}
	else
		deleteFloatLeaf(key, numberBlock + 1);
}
