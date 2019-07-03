#include "BufferManager.h" 

/*
��ʼ��һ����ĺ���
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
��ʼ�������������ĺ�������ʼ��ʧ�ܷ���FALSE
*/

bool Buffer::BufferInit() {
	for (int i = 0; i < MAX_BLOCK; i++) {
		Block[i] = BlockInit(i);
		if (NULL == Block[i])			//��ĳ����δ����ʼ��
			return false;
	}

	CurrentBlock = new blockHandle;		//�½���ռ�ÿ���Ϣ����
	CurrentBlock->CurrentBlockNum = 0;  //��ʼ��ʱ�����������п�����ڿ���״̬
	CurrentBlock->firstBlock = NULL;

	CurrentFile = new fileHandle;		//�½��������л�Ծ�ļ�����
	CurrentFile->CurrentFileNum = 0;	//��ʼ��ʱ��������û���ļ�����
	CurrentFile->firstFile = NULL;

	return true;
}

/*
�ҵ�һ�����õ��ڴ�鲢�����ŵ�blockhandle����β��
����ڴ�鶼��ռ�ã�����LRU�ҵ�һ�������ڴ��
*/

blockInfoPointer Buffer::BlockFind() {
	int i = 0;

	while (Block[i]->isActive != IDLE && i < 40)
		i++;								 //�ҵ�һ�����õĻ����

	if (i != 40) {
		//cout << "the " << i << "th block is active!" << endl;
		BlocktoTail(Block[i]);				 //���ڴ���������β��
		return Block[i];
	}
	else
		return LRU();
}

/*
��һ��ָ����ŵ�blockhandle������β��
*/

bool Buffer::BlocktoTail(blockInfoPointer TailBlock) {
	if (CurrentBlock->CurrentBlockNum == 40) {			//��������ȫ����ռ�ã����ܽ��в���
		//cout << "The buffer is full!" << endl;
		return false;
	}
	else {
		CurrentBlock->CurrentBlockNum++;
		if (CurrentBlock->firstBlock == NULL) {			//blockhandle����Ϊ��
			CurrentBlock->firstBlock = TailBlock;		//��һ���鼴ΪTailBlock
			TailBlock->isActive = ACTIVE;
			TailBlock->next = NULL;
			return true;								//����ɹ�
		}
		blockInfoPointer temp = CurrentBlock->firstBlock;
		while (temp->next != NULL)						//�ҵ�Blockhandle�����һ��
			temp = temp->next;
		temp->next = TailBlock;
		TailBlock->isActive = ACTIVE;
		TailBlock->next = NULL;
		return true;
	}
}

/*
�ҵ�blockhandle�����Ծ��һ��������滻
*/

blockInfoPointer Buffer::LRU() {
	if (CurrentBlock->CurrentBlockNum == 0)
		return NULL;
	else {
		blockInfoPointer BlocktoReplace = CurrentBlock->firstBlock;
		while (BlocktoReplace->lock == LOCKED && BlocktoReplace != NULL)
			BlocktoReplace = BlocktoReplace->next;	  //�ҵ���һ��δ���������Ծ�Ŀ��

		if (BlockHandleUse(BlocktoReplace) == true) { //�����Ծ�Ŀ����ĩβ
			//cout << "��������� " << BlocktoReplace->blockOrder << "�������Ծ�����ŵ�ĩβ�滻" << endl;
			write_block_to_disk(BlocktoReplace);
			return BlocktoReplace;
		}
		else                                          //û��δ�����Ŀ����滻��
			return NULL;
	}
}

/*
ʹ��BlockHandle�е�ĳһ�飬��������������β��
*/

bool Buffer::BlockHandleUse(blockInfoPointer BlocktoUse) {
	if (BlocktoUse == NULL)
		return false;

	if (CurrentBlock->firstBlock == 0)					//BLOCKHANDLE������û�п����
		return false;

	blockInfoPointer temp = CurrentBlock->firstBlock;
	if (temp == BlocktoUse) {
		CurrentBlock->firstBlock = BlocktoUse->next;
		CurrentBlock->CurrentBlockNum--;
		BlocktoTail(BlocktoUse);
		return true;									//BLOCKHANDLE�е�һ�����ƶ���β��
	}

	while (temp->next != BlocktoUse) {
		temp = temp->next;
		if (temp->next == NULL)							//BLOCKHANDLE��û�н�Ҫʹ�õĿ�
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
��blockhandle��ɾ��ĳ����
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
		if (temp->next == NULL) {										//û�������Ŀ��ڻ�����
			BlocktoDelete->isActive = IDLE;
			return false;
		}
		if (temp->next->blockOrder == BlocktoDelete->blockOrder)	    //�ҵ��˴�ɾ���Ŀ�
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
ɾ����filehandle�е�ĳ����
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
��block������fileβ��
*/

bool Buffer::replace(fileInfoPointer file, blockInfoPointer block) {
	if (file->firstBlock == NULL) {			//�ļ��ṹ��һ���鲻����
		file->firstBlock = block;
		block->nextinfile = NULL;
		block->file = file;					//block��fileָ��ָ��file
		return true;
	}

	blockInfoPointer tailblock = file->firstBlock;
	while (tailblock->nextinfile != NULL)
		tailblock = tailblock->nextinfile;	//�ҵ��ļ��ṹ�����һ����
	tailblock->nextinfile = block;			//��block���ӵ��ļ�β��
	block->nextinfile = NULL;
	block->file = file;
	return true;
}

/*
��ȡfile��Ϣ
*/

fileInfoPointer Buffer::get_file_info(string filename, int filetype) {
	if (CurrentFile->firstFile == NULL) {					//filehandleΪ�գ�ֱ����Ϊfirstblock
		//cout << "firstfileΪ��" << endl;
		if (filetype == DATAFILE)
			CurrentFile->firstFile = get_table_info(filename);
		else if (filetype == INDEXFILE)
			CurrentFile->firstFile = get_index_info(filename);
		return CurrentFile->firstFile;
	}

	fileInfoPointer temp = CurrentFile->firstFile;
	while (true) {											//�ҵ�filehandle���Ƿ��и��ļ� 
		if (temp->fileName == filename)						//���ڴ����ҵ��˸��ļ�
			return temp;
		else
			temp = temp->next;
		if (temp == NULL)									//�ڴ��в����ڸ��ļ�
			break;
	}

	if (CurrentFile->CurrentFileNum == MAX_FILE_ACTIVE) {	//ȷ��filehandle�Ƿ����5���ļ�
		close_file(CurrentFile->firstFile->fileName);
		fileInfoPointer newfile = get_table_info(filename); //5���ļ����滻����һ���ļ����������ļ�����β��

		return newfile;
	}

	if (filetype == DATAFILE) {								//�Ӵ�������������ļ�
		fileInfoPointer newfile = get_table_info(filename);
		if (newfile == NULL) {								//�ñ��ļ�������
			//cout << "���ļ�������" << endl;
			return NULL;
		}
		return newfile;
	}

	if (filetype == INDEXFILE) {							//�Ӵ���������������ļ�
		fileInfoPointer newfile = get_index_info(filename);
		if (newfile == NULL) {								//�������ļ�������
			//cout << "�����ļ�������" << endl;
			return NULL;
		}
		return newfile;
	}
	return NULL;
}

/*
�Ӵ������ҵ�һ�������Ϣ
*/

fileInfoPointer Buffer::get_table_info(string filename) {
	FILE *fp = fopen(filename.c_str(), "rb+");
	if (fp == NULL) 										//û������һ���ļ�
		return NULL;

	fileInfoPointer newfile = new fileInfo;					//�ҵ��ļ������г�ʼ��
	newfile->fileName = filename;
	newfile->firstBlock = NULL;
	newfile->next = NULL;
	newfile->recordAmount = 4;
	newfile->recordLength = 1024;
	newfile->type = DATAFILE;

	if (CurrentFile->CurrentFileNum < MAX_FILE_ACTIVE)
		CurrentFile->CurrentFileNum++;						//FILEHANDLE��¼����һ

	fileInfoPointer temp = CurrentFile->firstFile;			//����filehandleβ��
	if (temp == NULL)
		CurrentFile->firstFile = newfile;
	else {
		while (temp->next != NULL)							//�ҵ�β�ļ�
			temp = temp->next;
		temp->next = newfile;
	}
	fclose(fp);

	return newfile;
}

/*
�Ӵ������ҵ�һ�������ļ�����Ϣ
*/

fileInfoPointer Buffer::get_index_info(string filename) {
	FILE *fp = fopen(filename.c_str(), "rb+");
	if (fp == NULL) 										//û������һ���ļ�
		return NULL;

	fileInfoPointer newfile = new fileInfo;					//�ҵ��ļ������г�ʼ��
	newfile->fileName = filename;
	newfile->firstBlock = NULL;
	newfile->next = NULL;
	newfile->recordAmount = 0;								//������û�����¼������ز���
	newfile->recordLength = 0;
	newfile->type = INDEXFILE;

	if (CurrentFile->CurrentFileNum < MAX_FILE_ACTIVE)
		CurrentFile->CurrentFileNum++;						//FILEHANDLE��¼����һ

	fileInfoPointer temp = CurrentFile->firstFile;			//����filehandleβ��
	if (temp == NULL)
		CurrentFile->firstFile = newfile;
	else {
		while (temp->next != NULL)							//�ҵ�β�ļ�
			temp = temp->next;
		temp->next = newfile;
	}
	fclose(fp);

	return newfile;
}

/*
���ڴ���ĳ����д�ش���,����������ļ��������
*/

bool Buffer::write_block_to_disk(blockInfoPointer block) {
	if (block == NULL)
		return false;
	if (block->isActive == IDLE)					//�ÿ�û���ڴ���
		return false;
	if (block->file == NULL)						//�ÿ鲻�����κ��ļ�
		return false;
	if (block->dirtyBit == FLASE) {					//�ÿ�û�б��޸Ĺ�
		filehandle_delete(block->file, block);
		blockhandle_delete(block);
		return true;
	}
	string filepath = block->file->fileName;		//�ж��ļ�����

	FILE *fp = fopen(filepath.c_str(), "rb+");
	if (fp == NULL) {
		//cout << filepath.c_str() << endl;
		//cout << "no such file!" << endl;
		return false;
	}
	else {
		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		if (filesize <= BLOCK_LEN * block->blockNum) {	//�ļ���С����
			for (int i = 0; i <= block->blockNum - (filesize / BLOCK_LEN); i++) {
				fseek(fp, 0, SEEK_END);
				fwrite(blankblock, BLOCK_LEN, 1, fp);
			}
		}
		fseek(fp, BLOCK_LEN * block->blockNum, SEEK_SET);
		if (1 != fwrite(block->cBlock, BLOCK_LEN, 1, fp)) {	//д��ʧ��
			//cout << "д��ʧ�ܣ�" << endl;
		}
		filehandle_delete(block->file, block);
		blockhandle_delete(block);
		fclose(fp);
		return true;
	}

	return true;
}

/*
���ļ��ĵ�blocknum����뻺������
*/

blockInfoPointer Buffer::get_file_block(string filename, int blocknum, int filetype) {
	fileInfoPointer FiletoUse = get_file_info(filename, filetype);
	if (FiletoUse == NULL)								//û������һ���ļ�
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

	string filepath = FiletoUse->fileName;				//�ж��ļ�����

	FILE* fp = fopen(filepath.c_str(), "rb+");
	if (fseek(fp, blocknum * BLOCK_LEN, SEEK_SET)) {
		//cout << "�����ļ���С��" << endl;
		fclose(fp);
		return NULL;
	}

	blockInfoPointer BlocktoUse = BlockFind();		//�ÿ鲻�ڻ�������
	BlocktoUse->blockNum = blocknum;				//��ʼ��
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
�����ļ��ĵ�blocknum����뻺������
*/

blockInfoPointer Buffer::get_table_block(string filename, int blocknum) {
	return get_file_block(filename, blocknum, DATAFILE);
}

/*
�������ļ��ĵ�blocknum����뻺������
*/

blockInfoPointer Buffer::get_index_block(string filename, int blocknum) {
	return get_file_block(filename, blocknum, INDEXFILE);
}

/*
��ֻ����ʽ���ļ�����
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
ֻ����ʽ���ļ�
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
��д��ʽ���ļ�
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
�Զ�д��ʽ���ļ�����
*/

char* Buffer::write_file_block(string filename, int blocknum, int filetype) {
	blockInfoPointer block = get_file_block(filename, blocknum, filetype);
	block->dirtyBit = DIRTY;
	return block->cBlock;
}

/*
�ر�ĳ���ļ�
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
����һ�����ļ�
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
�ر�ȫ���ļ�
*/

void Buffer::close_database() {
	while (CurrentFile->firstFile != NULL)
		close_file(CurrentFile->firstFile->fileName);
	CurrentFile->CurrentFileNum = 0;
	return;
}

/*
���������캯��
*/

Buffer::Buffer() {
	if (false == BufferInit()) {
		//cout << "Buffer initialization error!" << endl; //��������ʼ���쳣����
	}
}

/*
��������������
*/

Buffer::~Buffer() {
	for (int i = 0; i < MAX_BLOCK; i++) {
		delete[] Block[i]->cBlock;		//�ͷŻ������������
	}

	delete CurrentBlock;				//�ͷ�������Ϣ����ͷ
	delete CurrentFile;

	//cout << "Buffer is deleted!" << endl;
}