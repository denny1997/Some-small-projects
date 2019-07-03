#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<vector>
#include"BufferManager.h";
#include"index.h"
#include"catalog_manager.h"

const int BLOCKSIZE = 128;
const int TUPLESIZE = 4095;


//创建表单文件
void record_create_table_file(std::string table_name);
//删除表单文件
void record_delete_table_file(std::string table_name);
//判断是不是字符串
bool record_is_char(const std::string & s);
//c++ 版split
void record_split_string(const std::string& s, std::vector<std::string>& v, const std::string& c);
//获取文件占了几个block
int record_get_block_num(const std::string &table_name);
//从一块里面读取成元组
std::vector<std::string> record_get_tuple(char* p);
//search，支持有条件和无条件，返回的是元组信息的字符串
void record_search(std::vector<std::string> &result,
	const std::string &table_name, const std::string &attr = "",
	const std::string &condition = "", const std::string &value = "");
//真正的search，可以返回想要的属性
void record_final_search(std::vector<std::string> &result,
	const std::string &table_name, const std::string aim_attr = "", const std::string &attr = "",
	const std::string &condition = "", const std::string &value = "");
//输出字符串形式的元组
void output_tuple(const std::vector<std::string> list);
//insert
bool record_insert(std::string &table_name, std::string &tuple);
//找到所有的元组然后逐个删除
void record_delete(const std::string &table_name, const std::string &attr = "",
	const std::string &condition = "", const std::string &value = "");
//逐个删除
void record_delete_one_tuple(const std::string &table_name, const std::string &tuple);

void record_final_search_with_index(std::vector<int> index_tuple, std::vector<std::string> &result,
	const std::string &table_name, const std::string aim_attr, const std::string &attr,
	const std::string &condition, const std::string &value);

void record_search_with_index(std::vector<int> index_tuple, std::vector<std::string> &result,
	const std::string &table_name, const std::string &attr,
	const std::string &condition, const std::string &value);

