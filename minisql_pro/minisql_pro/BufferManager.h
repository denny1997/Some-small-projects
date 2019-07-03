#pragma once
#ifndef _BUFFER_MANAGER_H_
#define _BUFFER_MANAGER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <iostream>
#include <cstring>

#define BLOCK_LEN			4096	// ��Ĵ�С
#define MAX_FILE_ACTIVE 	5		// �����������Ŀ����ļ�����Ҳ���ļ�ͷ���������ֵ
#define MAX_BLOCK			40		// �����������Ŀ���
#define INDEXFILE           1		// �����ļ�����
#define DATAFILE            0		// �����ļ�����
#define LOCKED              1		// �鴦������״̬�����ܴӻ������滻��ȥ
#define UNLOCKED            0		// ��δ������
#define DIRTY               1		// �������ݱ��Ķ�������Ҫд�ص������ļ�
#define FLASE               0		// �������ݲ���д��
#define ACTIVE              1		// ����blockhandle������
#define IDLE                0		// �鲻��blockhandle������

typedef struct fileInfo* fileInfoPointer;
typedef struct blockInfo* blockInfoPointer;

using namespace std;

/*
�ļ�ͷ�ṹ���ڴ��е��ļ�ͷ����Ľڵ㣬��ʾ���������ڴ棩�еĿ���������Щ�ļ�
*/

static char blankblock[BLOCK_LEN] = { 0 };

struct fileInfo {
	int type;				// ��DATA FILE �� INDEX FILE ��������
	string fileName;		// �ļ���
	int recordAmount;		// �ļ��еļ�¼��
	int recordLength;		// �ļ���һ����¼���ֽڳ�
	fileInfoPointer next;			// ��һ���ļ���ָ��
	blockInfoPointer firstBlock;	// �ļ����ݵĵ�һ����
};

/*
����Ϣ�ṹ���ڴ��еĿ���Ϣ����Ľڵ㣬��¼�˻��������ڴ棩�еĿ���Ϣ����ָ��ʵ�ʵ����ݴ�
*/

struct blockInfo {
	int blockNum;			// �ÿ����ļ��еĿ���
	int blockOrder;         // �ÿ��������������е�˳��
	bool dirtyBit;			// 0��ʾδ���Ĺ���1��ʾ����飬��Ҫд���ļ�
	blockInfoPointer next;  		// ָ����һ�����ָ��
	fileInfoPointer file;     		// �ÿ������ļ���ָ��
	blockInfoPointer nextinfile;		// ��filehandle�����һ��
	bool isActive;	   		// ���Ƿ���blockhandle������
	char *cBlock;			// ������ſ������ݵ��ַ������飬��ʵ�ʵĿ�����
	bool lock;				// ��ʾ�Ƿ������ÿ�
};

struct blockHandle {
	blockInfoPointer firstBlock;  // ָ���һ�����ָ��
	int CurrentBlockNum;	// ��ǰ�ڻ��������ڴ������
};

struct fileHandle {
	fileInfoPointer firstFile;  	// ָ����һ�����ָ��
	int CurrentFileNum;		// ��ǰ�ڻ��������ڴ������
};

class Buffer {
public:
	blockInfoPointer Block[MAX_BLOCK];
	struct blockHandle* CurrentBlock;
	struct fileHandle* CurrentFile;

public:
	blockInfoPointer BlockInit(int blockorder); //���ڴ��г�ʼ��һ����������ĺ���
	bool BufferInit();			  //��ʼ������������
	blockInfoPointer BlockFind(); //�ڻ��������ҵ�һ�����õ��ڴ��
	blockInfoPointer LRU();       //�ҵ�BufferHandle�����Ծ��һ���ڴ��
	bool BlocktoTail(blockInfoPointer TailBlock);		   //��һ��ָ����ŵ�blockhandle������β��
	bool BlockHandleUse(blockInfoPointer BlocktoUse);	   //ʹ��BlockHandle�е�ĳһ�飬��������������β��
	bool replace(fileInfoPointer file, blockInfoPointer block);  //��block������fileβ��
	bool blockhandle_delete(blockInfoPointer BlocktoDelete);     //��blockhandle��ɾ��ĳ����
	bool filehandle_delete(fileInfoPointer filename, blockInfoPointer BlocktoDelete);//ɾ����filehandle�е�ĳ����
	fileInfoPointer get_file_info(string filename, int filetype);//��ȡfile��Ϣ
	fileInfoPointer get_table_info(string filename);			 //�Ӵ������ҵ�һ�������Ϣ,��������β��
	fileInfoPointer get_index_info(string filename);			 //�Ӵ������ҵ�һ�������Ϣ,��������β��
	blockInfoPointer get_file_block(string filename, int blocknum, int filetype);//���ļ��ĵ�blocknum����뻺������
	blockInfoPointer get_table_block(string filename, int blocknum);//�����ļ��ĵ�blocknum����뻺������
	blockInfoPointer get_index_block(string filename, int blocknum);//�������ļ��ĵ�blocknum����뻺������
	bool write_block_to_disk(blockInfoPointer);					 //��ĳ����д�ش���
	void close_file(string filename);							 //�ر�ĳ���ļ�	
	void close_database();										 //�ر�ȫ���ļ�
	const char* read_file_block(string filename, int blocknum, int filetype); //��ֻ����ʽ���ļ�����
	char* write_file_block(string filename, int blocknum, int filetype);//��ֻ����ʽ���ļ�����
	fileInfoPointer create_newfile(string filename, int filetype, int blocksize);		//����һ��blocksize���С�����ļ�
	fileInfoPointer read_file(string filename, int filetype);			//ֻ����ʽ���ļ�
	fileInfoPointer write_file(string filename, int filetype);			//��д��ʽ���ļ�
	Buffer();
	~Buffer();
};

#endif 

