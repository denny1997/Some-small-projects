#include"record_manager.h"
//从catalog得到这张表的属性信息
Buffer buff;

//创建表单文件
void record_create_table_file(std::string table_name)
{
	std::ofstream f(table_name);
	std::cout << table_name;
	system("pause");
	f.close();
}

//删除表单文件
void record_delete_table_file(std::string table_name)
{
	buff.close_database();
	buff.BufferInit();
	remove(table_name.c_str());
}
//判断是不是字符串
bool record_is_char(const std::string & s)
{
	if (*(s.begin()) == '\''&&*(s.end() - 1) == '\'') return true;
	else return false;
}
//c++ 版split
void record_split_string(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string sub = s.substr(0, 100);
	std::string::size_type pos1, pos2;
	pos2 = sub.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(sub.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = sub.find(c, pos1);
	}
	if (pos1 != sub.length())
		v.push_back(sub.substr(pos1));
	for (auto iter = v.begin(); iter != v.end(); )
	{
		if (*iter == "") iter = v.erase(iter);
		else iter++;
	}
}
//获取文件占了几个block
int record_get_block_num(const std::string &table_name)
{

	bool flag;
	int block_num = -1;
	do {
		const char* p = buff.read_file_block(table_name, block_num + 1, DATAFILE);
		block_num++;
		flag = (p[0]>0);
	} while (flag);
	return block_num + 1;
}

//从一块里面读取成元组
std::vector<std::string> record_get_tuple(char* p)
{
	std::string s = p;
	std::vector<std::string> result;
	record_split_string(s, result, "*");
	return result;
}

//search，支持有条件和无条件，返回的是元组信息的字符串
void record_search(std::vector<std::string> &result,
	const std::string &table_name, const std::string &attr ,
	const std::string &condition , const std::string &value)
{
	int block_num = record_get_block_num(table_name);
	block_num--;
	//无条件查询
	if (attr == "")
	{
		for (int i = 0; i < block_num; i++)
		{
			char* p = buff.write_file_block(table_name, i, DATAFILE);
			std::vector<std::string> temp_tuple = record_get_tuple(p);
			for (auto iter = temp_tuple.begin(); iter != temp_tuple.end(); iter++)
			{
				result.push_back(*iter);
			}
		}
	}
	//有条件查询
	else {
		std::vector<attr_type> attr_type_list = catalog_get_attr(table_name);//需要catolog manager
		std::vector<std::string> attr_list, condition_list, value_list;
		record_split_string(attr, attr_list, "#");
		record_split_string(condition, condition_list, "#");
		record_split_string(value, value_list, "#");

		for (int i = 0; i < block_num; i++)
		{
			char* p = buff.write_file_block(table_name, i, DATAFILE);//需要buffer_manager
			std::vector<std::string> temp_tuple = record_get_tuple(p);
			//遍历每个元组
			for (auto iter1 = temp_tuple.begin(); iter1 != temp_tuple.end(); iter1++)
			{
				std::vector<std::string> attrs_in_tuple;
				record_split_string(*iter1, attrs_in_tuple, "#");
				bool flag = true;
				//遍历元组中的每个元素
				for (int i = 0; i < attrs_in_tuple.size(); i++)
				{
					//遍历查找输入条件中 每个元素的名字
					for (int j = 0; j < attr_list.size(); j++)
					{
						//如果查找条件中的元素名字与当前元素的名字相同，就开始比较是否符合查找条件
						if (attr_list[j] == attr_type_list[i].attr_name)
						{
							//如果是char类型元素
							if (attr_type_list[i].type_name == "char")
							{
								if (condition_list[j] == "="&&value_list[j] != attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == "<>"&&value_list[j] == attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == "<="&&value_list[j] < attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == ">="&&value_list[j] > attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == ">"&&value_list[j] >= attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == "<"&&value_list[j] <= attrs_in_tuple[i])
									flag = false;
							}
							//如果不是char
							else {
								float float_value = std::stof(value_list[j]);
								float float_attr = std::stof(attrs_in_tuple[i]);
								if (condition_list[j] == "="&&float_value != float_attr)
									flag = false;
								if (condition_list[j] == "<>"&&float_value == float_attr)
									flag = false;
								if (condition_list[j] == "<="&&float_value < float_attr)
									flag = false;
								if (condition_list[j] == ">="&&float_value > float_attr)
									flag = false;
								if (condition_list[j] == ">"&&float_value >= float_attr)
									flag = false;
								if (condition_list[j] == "<"&&float_value <= float_attr)
									flag = false;
							}
						}
					}
				}
				if (flag) result.push_back(*iter1);
			}
		}

	}
}
//真正的search，可以返回想要的属性
void record_final_search(std::vector<std::string> &result,
	const std::string &table_name, const std::string aim_attr , const std::string &attr ,
	const std::string &condition , const std::string &value )
{
	std::vector<std::string> aim_attr_list;
	record_split_string(aim_attr, aim_attr_list, "#");
	if (aim_attr == "")
	{
		std::vector<attr_type> attr_type_list = catalog_get_attr(table_name);
		for (auto iter = attr_type_list.begin(); iter != attr_type_list.end(); iter++)
		{
			std::cout << (*iter).attr_name << " ";
			aim_attr_list.push_back((*iter).attr_name);
		}
		std::cout << endl;
	}
	
	std::vector<attr_type> attr_type_list = catalog_get_attr(table_name);
	std::vector<int> index;
	for (auto iter = aim_attr_list.begin(); iter != aim_attr_list.end(); iter++)
	{
		for (auto iter1 = attr_type_list.begin(); iter1 != attr_type_list.end(); iter1++)
		{
			if (*iter == (*iter1).attr_name) index.push_back(iter1 - attr_type_list.begin());
		}
	}
	std::vector<std::string> temp_result;
	record_search(temp_result, table_name, attr, condition, value);
	for (auto iter = temp_result.begin(); iter != temp_result.end(); iter++)
	{
		std::vector<std::string> result_list;
		record_split_string(*iter, result_list, "#");

		std::string result_s;
		for (auto iter2 = index.begin(); iter2 != index.end(); iter2++)
		{
			result_s = result_s + result_list[*iter2] + "#";
		}
		result.push_back(result_s);
	}
}
//输出字符串形式的元组
void output_tuple(const std::vector<std::string> list)
{
	for (auto iter = list.begin(); iter != list.end(); iter++)
	{
		std::vector<std::string> s;
		record_split_string(*iter, s, "#");
		for (auto iter1 = s.begin(); iter1 != s.end(); iter1++)
		{
			std::cout << *iter1 << " ";
		}
		std::cout << std::endl;
	}
}

//insert
bool record_insert(std::string &table_name, std::string &tuple)
{
	std::vector<std::string> temp_attr;
	std::vector<attr_type> attr_list = catalog_get_attr(table_name);//catolog 模块
	if (attr_list.empty()) return false;
	bool format_error = false;
	bool not_unique = false;
	record_split_string(tuple, temp_attr, ",");

	for (int i = 0; i < attr_list.end() - attr_list.begin(); i++)
	{
		//是否格式错误
		if (attr_list[i].type_name == "char"&&temp_attr.size() > attr_list[i].size + 2)
			format_error = true;
		else if (attr_list[i].type_name != "char"&&temp_attr.size() > attr_list[i].size)
			format_error = true;
		//是否主键、unique冲突
		if (attr_list[i].is_primary_key || attr_list[i].is_unique)
		{
			std::vector<std::string> temp_result;
			record_search(temp_result, table_name, attr_list[i].attr_name, "=", temp_attr[i]);
			if (!temp_result.empty()) {
				not_unique = true;
				if (attr_list[i].is_primary_key) std::cout << "主键冲突" << std::endl;
				else if (attr_list[i].is_unique) std::cout << attr_list[i].attr_name << "冲突" << std::endl;
			}

		}
	}
	if (format_error)
	{
		std::cout << "format_error";
		return false;
	}
	if (not_unique)
	{
		std::cout << "the vale has exist";
		return false;
	}
	//开始插入

	int block_num = record_get_block_num(table_name);
	char* p = buff.write_file_block(table_name, block_num - 1, DATAFILE);
	std::string s;
	int i_p = 0;
	while (p[i_p] > 0)
	{
		s[i_p] = p[i_p];
		i_p++;
	}
	if (s.size() == BLOCKSIZE)
	{
		p = buff.write_file_block(table_name, block_num, DATAFILE);
		s = p;
	}


	std::string insert_s;
	for (auto iter = temp_attr.begin(); iter != temp_attr.end(); iter++)
	{
		insert_s = insert_s + *iter + "#";
	}
	while (insert_s.size() < TUPLESIZE)
	{
		insert_s = insert_s + "*";
	}
	s = s + insert_s;
	strcpy(p, s.c_str());
	std::cout << p[4095];
	buff.close_database();
	return true;
}
//找到所有的元组然后逐个删除
void record_delete(const std::string &table_name, const std::string &attr ,
	const std::string &condition , const std::string &value )
{
	//删除所有记录
	
		std::vector<std::string> record_to_delete;
		record_search(record_to_delete, table_name, attr, condition, value);
		for (auto iter = record_to_delete.begin(); iter != record_to_delete.end(); iter++)
		{
			record_delete_one_tuple(table_name, *iter);
			buff.close_database();
		}
	
	buff.close_database();

}
//逐个删除
void record_delete_one_tuple(const std::string &table_name, const std::string &tuple)
{
	int block_num = record_get_block_num(table_name);
	block_num--;
	//
	char* p_end = buff.write_file_block(table_name, block_num - 1, DATAFILE);
	std::vector<std::string> end_tuple_list = record_get_tuple(p_end);
	auto end_iter = end_tuple_list.end()--;
	std::string s_end;
	for (auto iter = end_tuple_list.begin(); iter != end_iter; iter++)
	{
		while ((*iter).size() < TUPLESIZE)
		{
			*iter = *iter + "*";
		}
		s_end = s_end + *iter;
	}
	//


	for (int i = 0; i < block_num; i++)
	{

		char* p = buff.write_file_block(table_name, i, DATAFILE);
		std::vector<std::string> tuple_list = record_get_tuple(p);
		{
			for (auto iter = tuple_list.begin(); iter != tuple_list.end(); iter++)
			{

				if (*iter == tuple)
				{
					p_end = buff.write_file_block(table_name, block_num - 1, DATAFILE);
					strcpy(p, s_end.c_str());
					for (int i = 0; i < 4096; i++)
					{
						p_end[i] = 0;
					}
					return;
				}
			}
		}
	}
	buff.close_database();
}

void record_final_search_with_index(std::vector<int> index_tuple, std::vector<std::string> &result,
	const std::string &table_name, const std::string aim_attr, const std::string &attr,
	const std::string &condition, const std::string &value)
{
	std::vector<std::string> aim_attr_list;
	record_split_string(aim_attr, aim_attr_list, "#");
	if (aim_attr == "")
	{
		std::vector<attr_type> attr_type_list = catalog_get_attr(table_name);
		for (auto iter = attr_type_list.begin(); iter != attr_type_list.end(); iter++)
		{
			std::cout << (*iter).attr_name << std::endl;
			aim_attr_list.push_back((*iter).attr_name);
		}
	}

	std::vector<attr_type> attr_type_list = catalog_get_attr(table_name);
	std::vector<int> index;
	for (auto iter = aim_attr_list.begin(); iter != aim_attr_list.end(); iter++)
	{
		for (auto iter1 = attr_type_list.begin(); iter1 != attr_type_list.end(); iter1++)
		{
			if (*iter == (*iter1).attr_name) index.push_back(iter1 - attr_type_list.begin());
		}
	}
	std::vector<std::string> temp_result;
	record_search_with_index(index_tuple,temp_result, table_name, attr, condition, value);
	for (auto iter = temp_result.begin(); iter != temp_result.end(); iter++)
	{
		std::vector<std::string> result_list;
		record_split_string(*iter, result_list, "#");

		std::string result_s;
		for (auto iter2 = index.begin(); iter2 != index.end(); iter2++)
		{
			result_s = result_s + result_list[*iter2] + "#";
		}
		result.push_back(result_s);
	}
}

void record_search_with_index(std::vector<int> index_tuple,std::vector<std::string> &result,
	const std::string &table_name, const std::string &attr,
	const std::string &condition, const std::string &value)
{
	int block_num = record_get_block_num(table_name);
	block_num--;
	//无条件查询
	if (attr == "")
	{
		for (int i = 0; i < block_num; i++)
		{
			char* p = buff.write_file_block(table_name, i, DATAFILE);
			std::vector<std::string> temp_tuple = record_get_tuple(p);
			for (auto iter = temp_tuple.begin(); iter != temp_tuple.end(); iter++)
			{
				result.push_back(*iter);
			}
		}
	}
	//有条件查询
	else {
		std::vector<attr_type> attr_type_list = catalog_get_attr(table_name);//需要catolog manager
		std::vector<std::string> attr_list, condition_list, value_list;
		record_split_string(attr, attr_list, "#");
		record_split_string(condition, condition_list, "#");
		record_split_string(value, value_list, "#");
		for (auto iter_index = index_tuple.begin(); iter_index!=index_tuple.end(); iter_index++)
		{
			int i = *iter_index;
			char* p = buff.write_file_block(table_name, i, DATAFILE);//需要buffer_manager
			std::vector<std::string> temp_tuple = record_get_tuple(p);
			//遍历每个元组
			for (auto iter1 = temp_tuple.begin(); iter1 != temp_tuple.end(); iter1++)
			{
				std::vector<std::string> attrs_in_tuple;
				record_split_string(*iter1, attrs_in_tuple, "#");
				bool flag = true;
				//遍历元组中的每个元素
				for (int i = 0; i < attrs_in_tuple.size(); i++)
				{
					//遍历查找输入条件中 每个元素的名字
					for (int j = 0; j < attr_list.size(); j++)
					{
						//如果查找条件中的元素名字与当前元素的名字相同，就开始比较是否符合查找条件
						if (attr_list[j] == attr_type_list[i].attr_name)
						{
							//如果是char类型元素
							if (attr_type_list[i].type_name == "char")
							{
								if (condition_list[j] == "="&&value_list[j] != attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == "<>"&&value_list[j] == attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == "<="&&value_list[j] < attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == ">="&&value_list[j] > attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == ">"&&value_list[j] >= attrs_in_tuple[i])
									flag = false;
								if (condition_list[j] == "<"&&value_list[j] <= attrs_in_tuple[i])
									flag = false;
							}
							//如果不是char
							else {
								float float_value = std::stof(value_list[j]);
								float float_attr = std::stof(attrs_in_tuple[i]);
								if (condition_list[j] == "="&&float_value != float_attr)
									flag = false;
								if (condition_list[j] == "<>"&&float_value == float_attr)
									flag = false;
								if (condition_list[j] == "<="&&float_value < float_attr)
									flag = false;
								if (condition_list[j] == ">="&&float_value > float_attr)
									flag = false;
								if (condition_list[j] == ">"&&float_value >= float_attr)
									flag = false;
								if (condition_list[j] == "<"&&float_value <= float_attr)
									flag = false;
							}
						}
					}
				}
				if (flag) result.push_back(*iter1);
			}
		}

	}
}