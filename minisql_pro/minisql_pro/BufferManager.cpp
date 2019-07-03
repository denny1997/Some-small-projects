#include "BufferManager.h" 

/*
初始化一个块的函数
*/

blockInfoPointer Buffer::BlockInit(int blockorder) {
	blockInfoPointer NewBlock;

	NewBlock = new blockInfo;
	NewBlock->blockNum = 0;
	NewBlock->blockOrder = blockorder;
	NewBlock->isActive = IDLE;
	NewBlock->nextinfile = NULL;
	NewBlock->cBlock = new char[BLOCK_LEN];
	NewBlock->dirtyBit = FLASE;
	NewBlock->file = NULL;
	NewBlock->lock = UNLOCKED;
	NewBlock->next = NULL;

	return NewBlock;
}

/*
初始化整个缓冲区的函数，初始化失败返回FALSE
*/

bool Buffer::BufferInit() {
	for (int i = 0; i < MAX_BLOCK; i++) {
		Block[i] = BlockInit(i);
		if (NULL == Block[i])			//有某个块未被初始化
			return false;
	}

	CurrentBlock = new blockHandle;		//新建被占用块信息链表
	CurrentBlock->CurrentBlockNum = 0;  //初始化时缓冲区中所有块均处于空闲状态
	CurrentBlock->firstBlock = NULL;

	CurrentFile = new fileHandle;		//新建缓冲区中活跃文件链表
	CurrentFile->CurrentFileNum = 0;	//初始化时缓冲区中没有文件读入
	CurrentFile->firstFile = NULL;

	return true;
}

/*
找到一个可用的内存块并将它放到blockhandle链表尾端
如果内存块都被占用，则用LRU找到一个可用内存块
*/

blockInfoPointer Buffer::BlockFind() {
	int i = 0;

	while (Block[i]->isActive != IDLE && i < 40)
		i++;								 //找到一个可用的缓存块

	if (i != 40) {
		//cout << "the " << i << "th block is active!" << endl;
		BlocktoTail(Block[i]);				 //将内存块放至链表尾端
		return Block[i];
	}
	else
		return LRU();
}

/*
将一个指定块放到blockhandle的链表尾部
*/

bool Buffer::BlocktoTail(blockInfoPointer TailBlock) {
	if (CurrentBlock->CurrentBlockNum == 40) {			//缓冲区块全部被占用，不能进行插入
		//cout << "The buffer is full!" << endl;
		return false;
	}
	else {
		CurrentBlock->CurrentBlockNum++;
		if (CurrentBlock->firstBlock == NULL) {			//blockhandle链表为空
			CurrentBlock->firstBlock = TailBlock;		//第一个块即为TailBlock
			TailBlock->isActive = ACTIVE;
			TailBlock->next = NULL;
			return true;								//插入成功
		}
		blockInfoPointer temp = CurrentBlock->firstBlock;
		while (temp->next != NULL)						//找到Blockhandle的最后一块
			temp = temp->next;
		temp->next = TailBlock;
		TailBlock->isActive = ACTIVE;
		TailBlock->next = NULL;
		return true;
	}
}

/*
找到blockhandle中最不活跃的一个块进行替换
*/

blockInfoPointer Buffer::LRU() {
	if (CurrentBlock->CurrentBlockNum == 0)
		return NULL;
	else {
		blockInfoPointer BlocktoReplace = CurrentBlock->firstBlock;
		while (BlocktoReplace->lock == LOCKED && BlocktoReplace != NULL)
			BlocktoReplace = BlocktoReplace->next;	  //找到第一个未锁定的最不活跃的块的

		if (BlockHandleUse(BlocktoReplace) == true) { //将最不活跃的块放至末尾
			//cout << "缓冲区里第 " << BlocktoReplace->blockOrder << "个块最不活跃，被放到末尾替换" << endl;
			write_block_to_disk(BlocktoReplace);
			return BlocktoReplace;
		}
		else                                          //没有未锁定的可用替换块
			return NULL;
	}
}

/*
使用BlockHandle中的某一块，并将它放至链表尾部
*/

bool Buffer::BlockHandleUse(blockInfoPointer BlocktoUse) {
	if (BlocktoUse == NULL)
		return false;

	if (CurrentBlock->firstBlock == 0)					//BLOCKHANDLE链表中没有块存在
		return false;

	blockInfoPointer temp = CurrentBlock->firstBlock;
	if (temp == BlocktoUse) {
		CurrentBlock->firstBlock = BlocktoUse->next;
		CurrentBlock->CurrentBlockNum--;
		BlocktoTail(BlocktoUse);
		return true;									//BLOCKHANDLE中第一个块移动到尾端
	}

	while (temp->next != BlocktoUse) {
		temp = temp->next;
		if (temp->next == NULL)							//BLOCKHANDLE中没有将要使用的块
			return false;
	}

	temp->next = temp->next->next;
	CurrentBlock->CurrentBlockNum--;
	if (BlocktoTail(BlocktoUse) == true)
		return true;
	else
		return false;
}

/*
从blockhandle中删除某个块
*/

bool Buffer::blockhandle_delete(blockInfoPointer BlocktoDelete) {
	blockInfoPointer temp = CurrentBlock->firstBlock;
	if (temp->blockOrder == BlocktoDelete->blockOrder) {
		CurrentBlock->firstBlock = BlocktoDelete->next;
		BlocktoDelete->isActive = IDLE;
		BlocktoDelete->dirtyBit = FLASE;
		CurrentBlock->CurrentBlockNum--;
		return true;
	}
	else while (true) {
		if (temp->next == NULL) {										//没有这样的块在缓冲区
			BlocktoDelete->isActive = IDLE;
			return false;
		}
		if (temp->next->blockOrder == BlocktoDelete->blockOrder)	    //找到了待删除的块
			break;
		temp = temp->next;
	}
	/*
	blockInfoPointer t = CurrentBlock->firstBlock;
	while (t != NULL) {
	cout << t->blockOrder << endl;
	t = t->next;
	}*/
	temp->next = BlocktoDelete->next;
	BlocktoDelete->isActive = IDLE;
	BlocktoDelete->dirtyBit = FLASE;
	CurrentBlock->CurrentBlockNum--;
	return true;
}

/*
删除在filehandle中的某个块
*/

bool Buffer::filehandle_delete(fileInfoPointer filetoDelete, blockInfoPointer BlocktoDelete) {
	fileInfoPointer temp = CurrentFile->firstFile;
	while (true) {
		if (temp == NULL)
			return false;
		else if (temp->fileName == filetoDelete->fileName)
			break;
		else
			temp = temp->next;
	}

	blockInfoPointer tmp = temp->firstBlock;
	if (tmp == NULL)
		return false;
	while (true) {
		if (tmp->blockNum == BlocktoDelete->blockNum) {
			filetoDelete->firstBlock = BlocktoDelete->nextinfile;
			BlocktoDelete->isActive = IDLE;
			BlocktoDelete->nextinfile = NULL;
			return true;
		}
		else if (tmp->nextinfile->blockNum == BlocktoDelete->blockNum) {
			tmp->nextinfile = BlocktoDelete->nextinfile;
			BlocktoDelete->isActive = IDLE;
			BlocktoDelete->nextinfile = NULL;
			return true;
		}
		tmp = tmp->nextinfile;
		if (tmp == NULL)
			return false;
	}
}

/*
将block连接至file尾端
*/

bool Buffer::replace(fileInfoPointer file, blockInfoPointer block) {
	if (file->firstBlock == NULL) {			//文件结构第一个块不存在
		file->firstBlock = block;
		block->nextinfile = NULL;
		block->file = file;					//block的file指针指向file
		return true;
	}

	blockInfoPointer tailblock = file->firstBlock;
	while (tailblock->nextinfile != NULL)
		tailblock = tailblock->nextinfile;	//找到文件结构的最后一个块
	tailblock->nextinfile = block;			//将block连接到文件尾端
	block->nextinfile = NULL;
	block->file = file;
	return true;
}

/*
获取file信息
*/

fileInfoPointer Buffer::get_file_info(string filename, int filetype) {
	if (CurrentFile->firstFile == NULL) {					//filehandle为空，直接作为firstblock
		//cout << "firstfile为空" << endl;
		if (filetype == DATAFILE)
			CurrentFile->firstFile = get_table_info(filename);
		else if (filetype == INDEXFILE)
			CurrentFile->firstFile = get_index_info(filename);
		return CurrentFile->firstFile;
	}

	fileInfoPointer temp = CurrentFile->firstFile;
	while (true) {											//找到filehandle中是否有该文件 
		if (temp->fileName == filename)						//在内存中找到了该文件
			return temp;
		else
			temp = temp->next;
		if (temp == NULL)									//内存中不存在该文件
			break;
	}

	if (CurrentFile->CurrentFileNum == MAX_FILE_ACTIVE) {	//确认filehandle是否多于5个文件
		close_file(CurrentFile->firstFile->fileName);
		fileInfoPointer newfile = get_table_info(filename); //5个文件则替换出第一个文件，并将新文件插入尾部

		return newfile;
	}

	if (filetype == DATAFILE) {								//从磁盘里找这个表文件
		fileInfoPointer newfile = get_table_info(filename);
		if (newfile == NULL) {								//该表文件不存在
			//cout << "表文件不存在" << endl;
			return NULL;
		}
		return newfile;
	}

	if (filetype == INDEXFILE) {							//从磁盘里找这个索引文件
		fileInfoPointer newfile = get_index_info(filename);
		if (newfile == NULL) {								//该索引文件不存在
			//cout << "索引文件不存在" << endl;
			return NULL;
		}
		return newfile;
	}
	return NULL;
}

/*
从磁盘中找到一个表的信息
*/

fileInfoPointer Buffer::get_table_info(string filename) {
	FILE *fp = fopen(filename.c_str(), "rb+");
	if (fp == NULL) 										//没有这样一个文件
		return NULL;

	fileInfoPointer newfile = new fileInfo;					//找到文件并进行初始化
	newfile->fileName = filename;
	newfile->firstBlock = NULL;
	newfile->next = NULL;
	newfile->recordAmount = 4;
	newfile->recordLength = 1024;
	newfile->type = DATAFILE;

	if (CurrentFile->CurrentFileNum < MAX_FILE_ACTIVE)
		CurrentFile->CurrentFileNum++;						//FILEHANDLE记录数加一

	fileInfoPointer temp = CurrentFile->firstFile;			//插入filehandle尾部
	if (temp == NULL)
		CurrentFile->firstFile = newfile;
	else {
		while (temp->next != NULL)							//找到尾文件
			temp = temp->next;
		temp->next = newfile;
	}
	fclose(fp);

	return newfile;
}

/*
从磁盘中找到一个索引文件的信息
*/

fileInfoPointer Buffer::get_index_info(string filename) {
	FILE *fp = fopen(filename.c_str(), "rb+");
	if (fp == NULL) 										//没有这样一个文件
		return NULL;

	fileInfoPointer newfile = new fileInfo;					//找到文件并进行初始化
	newfile->fileName = filename;
	newfile->firstBlock = NULL;
	newfile->next = NULL;
	newfile->recordAmount = 0;								//索引中没有与记录长度相关参数
	newfile->recordLength = 0;
	newfile->type = INDEXFILE;

	if (CurrentFile->CurrentFileNum < MAX_FILE_ACTIVE)
		CurrentFile->CurrentFileNum++;						//FILEHANDLE记录数加一

	fileInfoPointer temp = CurrentFile->firstFile;			//插入filehandle尾部
	if (temp == NULL)
		CurrentFile->firstFile = newfile;
	else {
		while (temp->next != NULL)							//找到尾文件
			temp = temp->next;
		temp->next = newfile;
	}
	fclose(fp);

	return newfile;
}

/*
将内存中某个块写回磁盘,这个块所属文件必须存在
*/

bool Buffer::write_block_to_disk(blockInfoPointer block) {
	if (block == NULL)
		return false;
	if (block->isActive == IDLE)					//该块没在内存中
		return false;
	if (block->file == NULL)						//该块不属于任何文件
		return false;
	if (block->dirtyBit == FLASE) {					//该块没有被修改过
		filehandle_delete(block->file, block);
		blockhandle_delete(block);
		return true;
	}
	string filepath = block->file->fileName;		//判断文件类型

	FILE *fp = fopen(filepath.c_str(), "rb+");
	if (fp == NULL) {
		//cout << filepath.c_str() << endl;
		//cout << "no such file!" << endl;
		return false;
	}
	else {
		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		if (filesize <= BLOCK_LEN * block->blockNum) {	//文件大小不足
			for (int i = 0; i <= block->blockNum - (filesize / BLOCK_LEN); i++) {
				fseek(fp, 0, SEEK_END);
				fwrite(blankblock, BLOCK_LEN, 1, fp);
			}
		}
		fseek(fp, BLOCK_LEN * block->blockNum, SEEK_SET);
		if (1 != fwrite(block->cBlock, BLOCK_LEN, 1, fp)) {	//写入失败
			//cout << "写入失败！" << endl;
		}
		filehandle_delete(block->file, block);
		blockhandle_delete(block);
		fclose(fp);
		return true;
	}

	return true;
}

/*
将文件的第blocknum块放入缓冲区中
*/

blockInfoPointer Buffer::get_file_block(string filename, int blocknum, int filetype) {
	fileInfoPointer FiletoUse = get_file_info(filename, filetype);
	if (FiletoUse == NULL)								//没有这样一个文件
		return NULL;
	blockInfoPointer temp = FiletoUse->firstBlock;
	while (true) {
		if (temp == NULL)
			break;
		else if (temp->blockNum == blocknum)
			return temp;
		else
			temp = temp->nextinfile;
	}

	string filepath = FiletoUse->fileName;				//判断文件类型

	FILE* fp = fopen(filepath.c_str(), "rb+");
	if (fseek(fp, blocknum * BLOCK_LEN, SEEK_SET)) {
		//cout << "超出文件大小！" << endl;
		fclose(fp);
		return NULL;
	}

	blockInfoPointer BlocktoUse = BlockFind();		//该块不在缓冲区中
	BlocktoUse->blockNum = blocknum;				//初始化
	BlocktoUse->dirtyBit = FLASE;
	BlocktoUse->isActive = ACTIVE;
	BlocktoUse->lock = UNLOCKED;
	BlocktoUse->nextinfile = NULL;
	BlocktoUse->next = NULL;
	replace(FiletoUse, BlocktoUse);
	blockInfoPointer tmp = CurrentBlock->firstBlock;
	while (true) {
		if (tmp == NULL) {
			BlocktoTail(BlocktoUse);
			break;
		}
		else if (tmp->blockOrder == BlocktoUse->blockOrder)
			break;
		tmp = tmp->next;
	}

	fread(BlocktoUse->cBlock, BLOCK_LEN, 1, fp);
	fclose(fp);
	return BlocktoUse;
}

/*
将表文件的第blocknum块放入缓冲区中
*/

blockInfoPointer Buffer::get_table_block(string filename, int blocknum) {
	return get_file_block(filename, blocknum, DATAFILE);
}

/*
将索引文件的第blocknum块放入缓冲区中
*/

blockInfoPointer Buffer::get_index_block(string filename, int blocknum) {
	return get_file_block(filename, blocknum, INDEXFILE);
}

/*
以只读方式打开文件数据
*/

const char* Buffer::read_file_block(string filename, int blocknum, int filetype) {
	blockInfoPointer block = get_file_block(filename, blocknum, filetype);
	if (block != NULL) {
		block->dirtyBit = FLASE;
		return block->cBlock;
	}
	else
		return NULL;
}

/*
只读方式打开文件
*/

fileInfoPointer Buffer::read_file(string filename, int filetype) {
	if (NULL == get_file_info(filename, filetype))
		return NULL;

	string filepath = filename;

	FILE* fp = fopen(filepath.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp) / BLOCK_LEN;
	fclose(fp);

	int blocknum = 0;
	while (blocknum < filesize) {
		read_file_block(filename, blocknum, filetype);
		blocknum++;
	}
	return get_file_info(filename, filetype);
}

/*
读写方式打开文件
*/

fileInfoPointer Buffer::write_file(string filename, int filetype) {
	if (NULL == get_file_info(filename, filetype))
		return NULL;

	string filepath = filename;

	FILE* fp = fopen(filepath.c_str(), "rb");
	fseek(fp, 0, SEEK_END);
	int filesize = ftell(fp) / BLOCK_LEN;
	fclose(fp);

	int blocknum = 0;
	while (blocknum < filesize) {
		write_file_block(filename, blocknum, filetype);
		blocknum++;
	}
	return get_file_info(filename, filetype);
}

/*
以读写方式打开文件数据
*/

char* Buffer::write_file_block(string filename, int blocknum, int filetype) {
	blockInfoPointer block = get_file_block(filename, blocknum, filetype);
	block->dirtyBit = DIRTY;
	return block->cBlock;
}

/*
关闭某个文件
*/

void Buffer::close_file(string filename) {
	fileInfoPointer temp = CurrentFile->firstFile;
	char temp_block[BLOCK_LEN];
	if (temp == NULL)
		return;
	else if (temp->fileName == filename) {
		while (temp->firstBlock != NULL)
			write_block_to_disk(temp->firstBlock);
		CurrentFile->firstFile = CurrentFile->firstFile->next;
		CurrentFile->CurrentFileNum--;
		delete temp;

		FILE* fp = fopen(filename.c_str(), "rb+");
		fseek(fp, 0, SEEK_END);
		int blocksize = ftell(fp) / BLOCK_LEN - 1;
		int blocksize_copy = blocksize;
		while (true) {
			fseek(fp, blocksize * BLOCK_LEN, SEEK_SET);
			fread(temp_block, BLOCK_LEN, 1, fp);
			if ((strcmp(temp_block, blankblock) == 0) && blocksize != 0)
				blocksize--;
			else
				break;
		}

		if (blocksize == blocksize_copy) {
			fclose(fp);
			return;
		}

		char* pchar = new char[BLOCK_LEN * (blocksize + 1)];
		fseek(fp, 0, SEEK_SET);
		fread(pchar, BLOCK_LEN * (blocksize + 1), 1, fp);
		fclose(fp);
		fp = fopen(filename.c_str(), "wb+");
		fseek(fp, 0, SEEK_SET);
		fwrite(pchar, BLOCK_LEN * (blocksize + 1), 1, fp);
		fclose(fp);
		delete pchar;

		return;
	}
	while (true) {
		if (temp->next == NULL)
			return;
		else if (temp->next->fileName == filename)
			break;
		else
			temp = temp->next;
	}

	while (temp->next->firstBlock != NULL)
		write_block_to_disk(temp->next->firstBlock);
	fileInfoPointer temp_file = temp->next;
	temp->next = temp_file->next;
	CurrentFile->CurrentFileNum--;
	delete temp_file;

	FILE* fp = fopen(filename.c_str(), "rb+");
	fseek(fp, 0, SEEK_END);
	int blocksize = ftell(fp) / BLOCK_LEN - 1;
	int blocksize_copy = blocksize;
	while (true) {
		fseek(fp, blocksize * BLOCK_LEN, SEEK_SET);
		fread(temp_block, BLOCK_LEN, 1, fp);
		if ((strcmp(temp_block, blankblock) == 0) && blocksize != 0)
			blocksize--;
		else
			break;
	}

	if (blocksize == blocksize_copy) {
		fclose(fp);
		return;
	}

	char* pchar = new char[BLOCK_LEN * (blocksize + 1)];
	fseek(fp, 0, SEEK_SET);
	fread(pchar, BLOCK_LEN * (blocksize + 1), 1, fp);
	fclose(fp);
	fp = fopen(filename.c_str(), "wb+");
	fseek(fp, 0, SEEK_SET);
	fwrite(pchar, BLOCK_LEN * (blocksize + 1), 1, fp);
	fclose(fp);
	delete pchar;

	return;
}

/*
创建一个新文件
*/

fileInfoPointer Buffer::create_newfile(string filename, int filetype, int blocksize) {
	fileInfoPointer newfile = new fileInfo;
	string filepath = filename;

	newfile->fileName = filename;
	newfile->firstBlock = NULL;
	newfile->next = NULL;
	newfile->type = filetype;
	if (filetype == DATAFILE) {
		newfile->recordAmount = 4;
		newfile->recordLength = 1024;
	}
	else {
		newfile->recordAmount = 0;
		newfile->recordLength = 0;
	}

	FILE* fp = fopen(filepath.c_str(), "wb+");
	for (int i = 0; i < blocksize; i++)
		fwrite(blankblock, BLOCK_LEN, 1, fp);
	fclose(fp);

	return newfile;
}
/*
关闭全部文件
*/

void Buffer::close_database() {
	while (CurrentFile->firstFile != NULL)
		close_file(CurrentFile->firstFile->fileName);
	CurrentFile->CurrentFileNum = 0;
	return;
}

/*
缓冲区构造函数
*/

Buffer::Buffer() {
	if (false == BufferInit()) {
		//cout << "Buffer initialization error!" << endl; //缓冲区初始化异常报错
	}
}

/*
缓冲区析构函数
*/

Buffer::~Buffer() {
	for (int i = 0; i < MAX_BLOCK; i++) {
		delete[] Block[i]->cBlock;		//释放缓冲区块的数据
	}

	delete CurrentBlock;				//释放两个信息链表头
	delete CurrentFile;

	//cout << "Buffer is deleted!" << endl;
}