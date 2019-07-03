#pragma once
#ifndef _BUFFER_MANAGER_H_
#define _BUFFER_MANAGER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <iostream>
#include <cstring>

#define BLOCK_LEN			4096	// 块的大小
#define MAX_FILE_ACTIVE 	5		// 缓冲区中最多的可用文件数，也是文件头链表中最大值
#define MAX_BLOCK			40		// 缓冲区中最多的块数
#define INDEXFILE           1		// 索引文件类型
#define DATAFILE            0		// 数据文件类型
#define LOCKED              1		// 块处于锁定状态，不能从缓冲区替换出去
#define UNLOCKED            0		// 块未被锁定
#define DIRTY               1		// 块中数据被改动过，需要写回到磁盘文件
#define FLASE               0		// 块中数据不需写回
#define ACTIVE              1		// 块在blockhandle链表中
#define IDLE                0		// 块不在blockhandle链表中

typedef struct fileInfo* fileInfoPointer;
typedef struct blockInfo* blockInfoPointer;

using namespace std;

/*
文件头结构是内存中的文件头链表的节点，表示缓冲区（内存）中的块来自于哪些文件
*/

static char blankblock[BLOCK_LEN] = { 0 };

struct fileInfo {
	int type;				// 有DATA FILE 和 INDEX FILE 两种类型
	string fileName;		// 文件名
	int recordAmount;		// 文件中的记录数
	int recordLength;		// 文件中一条记录的字节长
	fileInfoPointer next;			// 下一个文件的指针
	blockInfoPointer firstBlock;	// 文件内容的第一个块
};

/*
块信息结构是内存中的块信息链表的节点，记录了缓冲区（内存）中的块信息，并指向实际的数据处
*/

struct blockInfo {
	int blockNum;			// 该块在文件中的块数
	int blockOrder;         // 该块在整个缓冲区中的顺序
	bool dirtyBit;			// 0表示未更改过，1表示是脏块，需要写回文件
	blockInfoPointer next;  		// 指向下一个块的指针
	fileInfoPointer file;     		// 该块所在文件的指针
	blockInfoPointer nextinfile;		// 在filehandle里的下一块
	bool isActive;	   		// 块是否在blockhandle链表中
	char *cBlock;			// 用来存放块中数据的字符型数组，即实际的块数据
	bool lock;				// 表示是否锁定该块
};

struct blockHandle {
	blockInfoPointer firstBlock;  // 指向第一个块的指针
	int CurrentBlockNum;	// 当前在缓冲区的内存块总数
};

struct fileHandle {
	fileInfoPointer firstFile;  	// 指向下一个块的指针
	int CurrentFileNum;		// 当前在缓冲区的内存块总数
};

class Buffer {
public:
	blockInfoPointer Block[MAX_BLOCK];
	struct blockHandle* CurrentBlock;
	struct fileHandle* CurrentFile;

public:
	blockInfoPointer BlockInit(int blockorder); //在内存中初始化一个缓冲区块的函数
	bool BufferInit();			  //初始化整个缓冲区
	blockInfoPointer BlockFind(); //在缓冲区中找到一个可用的内存块
	blockInfoPointer LRU();       //找到BufferHandle中最不活跃的一个内存块
	bool BlocktoTail(blockInfoPointer TailBlock);		   //将一个指定块放到blockhandle的链表尾部
	bool BlockHandleUse(blockInfoPointer BlocktoUse);	   //使用BlockHandle中的某一块，并将它放至链表尾部
	bool replace(fileInfoPointer file, blockInfoPointer block);  //将block连接至file尾端
	bool blockhandle_delete(blockInfoPointer BlocktoDelete);     //从blockhandle中删除某个块
	bool filehandle_delete(fileInfoPointer filename, blockInfoPointer BlocktoDelete);//删除在filehandle中的某个块
	fileInfoPointer get_file_info(string filename, int filetype);//获取file信息
	fileInfoPointer get_table_info(string filename);			 //从磁盘中找到一个表的信息,放入链表尾部
	fileInfoPointer get_index_info(string filename);			 //从磁盘中找到一个表的信息,放入链表尾部
	blockInfoPointer get_file_block(string filename, int blocknum, int filetype);//将文件的第blocknum块放入缓冲区中
	blockInfoPointer get_table_block(string filename, int blocknum);//将表文件的第blocknum块放入缓冲区中
	blockInfoPointer get_index_block(string filename, int blocknum);//将索引文件的第blocknum块放入缓冲区中
	bool write_block_to_disk(blockInfoPointer);					 //将某个块写回磁盘
	void close_file(string filename);							 //关闭某个文件	
	void close_database();										 //关闭全部文件
	const char* read_file_block(string filename, int blocknum, int filetype); //以只读方式打开文件数据
	char* write_file_block(string filename, int blocknum, int filetype);//以只读方式打开文件数据
	fileInfoPointer create_newfile(string filename, int filetype, int blocksize);		//创建一个blocksize块大小的新文件
	fileInfoPointer read_file(string filename, int filetype);			//只读方式打开文件
	fileInfoPointer write_file(string filename, int filetype);			//读写方式打开文件
	Buffer();
	~Buffer();
};

#endif 

