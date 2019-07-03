#pragma once
//一张表最多32个attribute
#include<string>
#include<vector>
#include<io.h>
#include<iostream>
#define catalog_table_not_exist 1
#define catalog_table_exist 2
#define catalog_index_not_exist 3
#define catalog_index_exist 4
#define catalog_attr_not_exist 5
#define catalog_index_name_exist 6

#define Max_index_num 1000

struct Attribute {
	short num;               //table的属性数
	std::string name[32];  //每个属性的名字
	std::string type[32];  //每个属性的类型：int\float\char
	short size[32];        //每个属性的长度
	bool unique[32];       //判断每个属性是否unique，是为true
	short primary_key;     //判断是否存在主键,-1为不存在，其他则为主键的所在位置
	bool has_index[32];    //判断是否存在索引
};

struct attr_type {
	std::string attr_name;	//属性名称
	std::string type_name;	//类型名称：int\float\char
	short size;				//属性长度
	bool is_primary_key;
	bool is_unique;
	bool has_index;
};

//索引管理，一张表最多只能有10个index
struct catalog_Index {
	int num;                    //index的总数
	short location[10];         //每个index在Attribute的name数组中是第几个
	std::string indexname[10];  //每个index的名字
};

struct catalog_Table {
	std::string table_name;
	struct Attribute attr;
	struct catalog_Index index;
};
struct catalog_index_table {
	std::string index_name;
	std::string table_name;
};

void catalog_ReadIndex(catalog_index_table *index, int &number);//用于将一个索引表对应文件的内容读到内部结构中，返回数量
void catalog_WriteIndex(catalog_index_table *index, int number);//用于写一个索引表对应文件
//用于将一个表文件的内容读到内部结构中
int ReadTable(catalog_Table &table);
//用于将一个表的内容写出到表文件中
void WriteTable(catalog_Table table);
/*
用于新建一个表
输入表名，属性结构，索引结构
正常创建返回0，重复建表返回catalog_table_exist错误
*/
int catalog_CreateTable(std::string table_name, struct Attribute attribute, catalog_Index index);
/*
用于删除一个表
输入表名
正常删除返回0，表不存在返回catalog_table_not_exist错误
*/
int catalog_DropTable(std::string table_name);
/*
用于创建索引
输入表名，建立索引的属性名，索引名
正常创建返回0，表不存在返回catalog_table_not_exist错误，属性不存在返回catalog_attr_not_exist
索引已经存在返回catalog_index_exist，索引名重复返回catalog_index_name_exist
*/
int catalog_CreateIndex(std::string table_name, std::string attr_name, std::string index_name);
/*
用于删除索引
输入表名，删除的索引名
正常删除返回0，表不存在返回catalog_table_not_exist错误，属性不存在返回catalog_attr_not_exist
索引不存在返回catalog_index_not_exist
*/
int catalog_DropIndex(std::string table_name, std::string index_name);
/*
用于删除索引(重载)
输入删除的索引名
正常删除返回0
索引不存在返回catalog_index_not_exist,属性不存在返回catalog_attr_not_exist
索引名重复返回catalog_index_name_exist
*/
int catalog_DropIndex( std::string index_name);

/*
用于查找一个表的全部索引名
输入表名
返回索引名的vector数组
*/
std::vector<std::string> catalog_get_index(const std::string &table_name);
/*
为record部分调用准备的接口
输入表名，获取一个表的属性
*/
std::vector<attr_type> catalog_get_attr(const std::string &table_name);
/*
输入索引名，返回其所在的表名
*/
std::string catalog_Index_to_table_name(std::string index_name);
/*
删表时调用，用以删除该表的索引在index_table文件的记录
*/
void catalog_DropTable_delete_indexs(std::string table_name);