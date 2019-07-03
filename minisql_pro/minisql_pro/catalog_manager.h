#pragma once
//һ�ű����32��attribute
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
	short num;               //table��������
	std::string name[32];  //ÿ�����Ե�����
	std::string type[32];  //ÿ�����Ե����ͣ�int\float\char
	short size[32];        //ÿ�����Եĳ���
	bool unique[32];       //�ж�ÿ�������Ƿ�unique����Ϊtrue
	short primary_key;     //�ж��Ƿ��������,-1Ϊ�����ڣ�������Ϊ����������λ��
	bool has_index[32];    //�ж��Ƿ��������
};

struct attr_type {
	std::string attr_name;	//��������
	std::string type_name;	//�������ƣ�int\float\char
	short size;				//���Գ���
	bool is_primary_key;
	bool is_unique;
	bool has_index;
};

//��������һ�ű����ֻ����10��index
struct catalog_Index {
	int num;                    //index������
	short location[10];         //ÿ��index��Attribute��name�������ǵڼ���
	std::string indexname[10];  //ÿ��index������
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

void catalog_ReadIndex(catalog_index_table *index, int &number);//���ڽ�һ���������Ӧ�ļ������ݶ����ڲ��ṹ�У���������
void catalog_WriteIndex(catalog_index_table *index, int number);//����дһ���������Ӧ�ļ�
//���ڽ�һ�����ļ������ݶ����ڲ��ṹ��
int ReadTable(catalog_Table &table);
//���ڽ�һ���������д�������ļ���
void WriteTable(catalog_Table table);
/*
�����½�һ����
������������Խṹ�������ṹ
������������0���ظ�������catalog_table_exist����
*/
int catalog_CreateTable(std::string table_name, struct Attribute attribute, catalog_Index index);
/*
����ɾ��һ����
�������
����ɾ������0�������ڷ���catalog_table_not_exist����
*/
int catalog_DropTable(std::string table_name);
/*
���ڴ�������
���������������������������������
������������0�������ڷ���catalog_table_not_exist�������Բ����ڷ���catalog_attr_not_exist
�����Ѿ����ڷ���catalog_index_exist���������ظ�����catalog_index_name_exist
*/
int catalog_CreateIndex(std::string table_name, std::string attr_name, std::string index_name);
/*
����ɾ������
���������ɾ����������
����ɾ������0�������ڷ���catalog_table_not_exist�������Բ����ڷ���catalog_attr_not_exist
���������ڷ���catalog_index_not_exist
*/
int catalog_DropIndex(std::string table_name, std::string index_name);
/*
����ɾ������(����)
����ɾ����������
����ɾ������0
���������ڷ���catalog_index_not_exist,���Բ����ڷ���catalog_attr_not_exist
�������ظ�����catalog_index_name_exist
*/
int catalog_DropIndex( std::string index_name);

/*
���ڲ���һ�����ȫ��������
�������
������������vector����
*/
std::vector<std::string> catalog_get_index(const std::string &table_name);
/*
Ϊrecord���ֵ���׼���Ľӿ�
�����������ȡһ���������
*/
std::vector<attr_type> catalog_get_attr(const std::string &table_name);
/*
���������������������ڵı���
*/
std::string catalog_Index_to_table_name(std::string index_name);
/*
ɾ��ʱ���ã�����ɾ���ñ��������index_table�ļ��ļ�¼
*/
void catalog_DropTable_delete_indexs(std::string table_name);