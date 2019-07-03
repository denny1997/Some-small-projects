#include"Interpreter.h"
#include"catalog_manager.h"
#include"index.h"
#include"record_manager.h"
#include <algorithm>
void API(string);

int main()
{
	string SQL,sql;
	cout << endl;
	cout << "***********************************************" << endl;
	cout << "             欢迎使用 MiniSQL !" << endl;
	cout << "               Version (1.0)  " << endl;
	cout << "            " << endl;
	cout << "     copyright(2018) all right reserved !" << endl;
	cout << "***********************************************" << endl;
	cout << endl << endl;
	while (1)
	{
		cout << "MiniSQL-->> ";
		SQL = Interpreter(SQL);
		if (SQL == "99")
		{
			SQL = "";
			continue;
		}
		int start, end;
		end = SQL.find(';', 0);
		sql = SQL.substr(0, end + 1);
		start = end + 1;
		while (end != SQL.length() - 1)
		{
			API(sql);
			end = SQL.find(';', start);
			sql = SQL.substr(start, end + 1 - start);
			start = end + 1;
		}
		API(sql);
		SQL = "";
	}
}


void API(string sql)
{
	string temp, op, table, index_name, attribute, attr, condition, values,tuple;
	int start, end, mid, i;
	struct catalog_Table new_table;
	//vector<string> index_list;
	end = sql.find(' ', 0);
	start = end + 1;
	op = sql.substr(0, end);
	if (op == "01")
	{
		//建表
		new_table.attr.num = 0;
		new_table.attr.primary_key = -1;
		new_table.index.num = 0;//初始化
		end = sql.find(',', 0);
		new_table.table_name = sql.substr(start, end-start);
		start = end + 1;
		end = sql.find(',', start);
		while (end != -1)
		{
			temp = sql.substr(start, end - start);
			start = end + 1;
			end = temp.find(' ', 0);
			new_table.attr.name[new_table.attr.num] = temp.substr(0, end);
			mid = end + 1;
			if (temp[mid] == '#')
			{
				for (i = 0;i < new_table.attr.num;i++)
					if (new_table.attr.name[i] == new_table.attr.name[new_table.attr.num])
					{
						new_table.attr.primary_key = i;
						new_table.attr.unique[i] = true;
						break;
					}
				break;
			}
			else
			{
				end = temp.find(' ', mid);
				new_table.attr.type[new_table.attr.num] = temp.substr(mid, end - mid);
				if (new_table.attr.type[new_table.attr.num] == "+")
				{
					new_table.attr.type[new_table.attr.num] = "int";
					new_table.attr.size[new_table.attr.num] = 10;
				}
				else if (new_table.attr.type[new_table.attr.num] == "-")
				{
					new_table.attr.type[new_table.attr.num] = "float";
					new_table.attr.size[new_table.attr.num] = 15;
				}
				else
				{
					new_table.attr.size[new_table.attr.num] = (short)(std::stoi(new_table.attr.type[new_table.attr.num]));
					new_table.attr.type[new_table.attr.num] = "char";
				}
			}
			mid = end + 1;
			if (temp[mid] == '0')
			{
				new_table.attr.unique[new_table.attr.num] = false;
			}
			else
			{
				new_table.attr.unique[new_table.attr.num] = true;
			}
			new_table.attr.has_index[new_table.attr.num] = false;//初始化
			new_table.attr.num++;
			end = sql.find(',', start);
		}
		//创建数据表
		if (catalog_CreateTable(new_table.table_name, new_table.attr, new_table.index))
			return;
		record_create_table_file(new_table.table_name);
		//判断是否创建主键索引
		//if (new_table.attr.num!=0)
		//Create_Index(Table_Name, Table_Name, Attr_Name, DB_Name, length, offset, type);
		//cout << "create table" << endl;
		return;
	}
	else if (op == "11")
	{
		//删表
		end = sql.find(';', start);
		table = sql.substr(start, end - start);
		//delete_table(table)

		
		vector<string> index_list;
		index_list = catalog_get_index(table);
		if (catalog_DropTable(table))
			return;
		record_delete_table_file(table);
		for (auto iter = index_list.begin(); iter != index_list.end(); iter++)
		{
			record_delete_table_file("index_node"+*iter);
			record_delete_table_file("index_leaf" + *iter);
		}
		//cout << "delete table " << table << endl;
		cout << "delete table successfully!" << endl;
		return;
	}
	else if (op == "02")
	{
		//建索引
		end = sql.find(' ', start);
		index_name = sql.substr(start, end - start);
		start = end + 1;
		end = sql.find(' ', start);
		table = sql.substr(start, end - start);
		start = end + 1;
		end = sql.find(';', start);
		attribute = sql.substr(start, end - start);
		//建索引(index_name,table,attribute);
		if (catalog_CreateIndex(table, attribute, index_name))
			return;
		vector<attr_type> attr_list = catalog_get_attr(table);
		string attr_type;
		for (auto iter = attr_list.begin(); iter != attr_list.end(); iter++)
		{
			if ((*iter).attr_name == attribute) attr_type = (*iter).type_name;
		}
		type enum_attr;
		if (attr_type == "char") enum_attr = CHAR;
		else if (attr_type == "int") enum_attr = INT;
		else enum_attr = FLOAT;

		Index index(index_name, enum_attr);
		vector<string> temp_tuple;
		record_final_search(temp_tuple, table, attribute);
		for (int i = 0; i < temp_tuple.size(); i++) {
			temp_tuple[i] = temp_tuple[i].substr(0, temp_tuple[i].length() - 1);
			cout << "---" << temp_tuple[i] << endl;
		}
		index.createIndex();
		index.insertIndex(temp_tuple);
		index.save();
		//cout << "create index " << index_name << " " << table << " " << attribute << endl;
		cout << "create index successfully!" << endl;
		return;
	}
	else if (op == "12")
	{
		//删索引
		end = sql.find(';', start);
		index_name = sql.substr(start, end - start);

		//删索引(index_name);
		if (catalog_DropIndex(index_name))
			return;
		record_delete_table_file("index_node"+index_name );
		record_delete_table_file("index_leaf"+index_name );
		//cout << "delete index " << index_name << endl;
		cout << "delete index successfully!" << endl;
		return;
	}
	else if (op == "20")
	{
		//查询语句
		end = sql.find(',', start);
		temp = sql.substr(start, end - start);
		start = end + 1;
		if (temp == "*")
			attribute = "";
		else
		{
			temp = temp + ",";
			end = temp.find(' ', 0);
			mid = 0;
			while (end != -1)
			{
				attribute = attribute + temp.substr(mid, end - mid) + "#";
				mid = end + 1;
				end = temp.find(' ', mid);
			}
			end = temp.find(',', mid);
			attribute = attribute + temp.substr(mid, end - mid);
		}
		end = sql.find(',', start);
		table = sql.substr(start, end - start);
		start = end + 1;
		while (sql[start] == ' ')
			start++;
		if (sql[start] == '*')
		{
			attr = "";
			condition = "";
			values = "";
		}
		else
		{
			attr = "";
			condition = "";
			values = "";
			end = sql.find('&', start);
			while (end != -1)
			{
				temp = sql.substr(start, end - start);
				start = end + 1;
				while (sql[start] == ' ')
					start++;
				i = temp.length() - 1;
				while (temp[i] == ' ')
					i--;
				temp = temp.substr(0, i + 1);
				temp = temp + ";";
				end = temp.find(' ', 0);
				attr = attr + temp.substr(0, end) + "#";
				mid = end + 1;
				while (temp[mid] == ' ')
					mid++;
				end = temp.find(' ', mid);
				condition = condition + temp.substr(mid, end - mid) + "#";
				mid = end + 1;
				while (temp[mid] == ' ')
					mid++;
				end = temp.find(';', mid);
				values = values + temp.substr(mid, end - mid) + "#";
				end = sql.find('&', start);
			}
			end = sql.find(';', start);
			temp = sql.substr(start, end - start);
			i = temp.length() - 1;
			while (temp[i] == ' ')
				i--;
			temp = temp.substr(0, i + 1);
			temp = temp + ";";
			end = temp.find(' ', 0);
			attr = attr + temp.substr(0, end);
			mid = end + 1;
			while (temp[mid] == ' ')
				mid++;
			end = temp.find(' ', mid);
			condition = condition + temp.substr(mid, end - mid);
			mid = end + 1;
			while (temp[mid] == ' ')
				mid++;
			end = temp.find(';', mid);
			values = values + temp.substr(mid, end - mid);
		}
		//查找(table,attribute,attr,condition,values);
		if (attr == "")
		{
			cout << "search without index\n";
			vector<string> temp_result;
			record_final_search(temp_result, table, attribute, attr, condition, values);
			output_tuple(temp_result);
			return;
		}
		vector<attr_type> attr_type_list = catalog_get_attr(table);
		vector<string> search_attr_list;
		record_split_string(attr, search_attr_list, "#");
		int index_i = 0;
		for (auto iter = attr_type_list.begin(); iter != attr_type_list.end(); iter++)
		{
			
			if ((*iter).attr_name == search_attr_list[0])
			{
				if ((*iter).has_index)
				{
					vector<string>index_list = catalog_get_index(table);
					string index_name = index_list[index_i];
					Index index(index_name);
					vector<string> index_attr, index_condition, index_values;
					record_split_string(attr, index_attr, "#");
					record_split_string(condition, index_condition, "#");
					record_split_string(values, index_values, "#");
					vector<int>re;
					for (int i = 0; i < index_attr.size(); i++) {
						range rr;
						string condition = index_condition[i];
						if (condition == "=")
							rr = EQ;
						else if (condition == "<>")
							rr = NEQ;
						else if (condition == "<")
							rr = S;
						else if (condition == "<=")
							rr = SE;
						else if (condition == ">")
							rr = B;
						else
							rr = BE;
						if (i == 0) {
							index.search(re, index_values[i], rr);
						}
						else {
							vector<int>re_2;
							index.search(re_2, index_values[i], rr);
							sort(re.begin(), re.end());
							sort(re_2.begin(), re_2.end());
							set_intersection(re.begin(), re.end(), re_2.begin(), re_2.end(), back_inserter(re));
							re_2.clear();
						}
						cout << "search with index\n";
						for (auto iter_re = re.begin(); iter_re != re.end(); iter_re++)
						{
							cout << " " << (*iter_re);
						}
						cout << endl;
						vector<string> temp_result;
						record_final_search_with_index(re,temp_result, table, attribute, attr, condition, values);
						output_tuple(temp_result);
					}
				}
				else {
					cout << "search without index\n";
					vector<string> temp_result;
					record_final_search(temp_result, table, attribute, attr, condition, values);
					output_tuple(temp_result);
				}
				break;
			}
			if ((*iter).has_index) index_i++;
		}
		
		//cout << "select " << table << " " << attribute << " " << attr << " " << condition << " " << values << endl;
		return;
	}
	else if (op == "30")
	{
		//插入语句
		while (sql[start] == ' ')
			start++;
		end = sql.find(',', start);
		table = sql.substr(start, end - start);
		start = end + 1;
		while (sql[start] == ' ')
			start++;
		end = sql.find(',', start);
		tuple = sql.substr(start, end - start);
		start = end + 1;
		while (sql[start] == ' ')
			start++;
		while (sql[start] != ';')
		{
			end = sql.find(',', start);
			tuple = tuple + "," + sql.substr(start, end - start);
			start = end + 1;
			while (sql[start] == ' ')
				start++;
		}
		//插入(table,tuple);
		if (!record_insert(table, tuple))
			return;
		vector<string> index_list;
		index_list = catalog_get_index(table);
		int index_i=0;
		vector<attr_type> attr_type_list = catalog_get_attr(table);
		for (auto iter = attr_type_list.begin(); iter != attr_type_list.end(); iter++)
		{
			if ((*iter).has_index)
			{
				vector<string> temp_tuple;
				record_final_search(temp_tuple, table, (*iter).attr_name);
				Index index(index_list[index_i++]);
				index.update(temp_tuple);
			}
		}
		cout << "insert successfully!" << endl;
		return;
	}
	else if (op == "40")
	{
		//删除语句
		end = sql.find(',', start);
		table = sql.substr(start, end - start);
		start = end + 1;
		while (sql[start] == ' ')
			start++;
		if (sql[start] == '*')
		{
			attr = "";
			condition = "";
			values = "";
		}
		else
		{
			attr = "";
			condition = "";
			values = "";
			end = sql.find('&', start);
			while (end != -1)
			{
				temp = sql.substr(start, end - start);
				start = end + 1;
				while (sql[start] == ' ')
					start++;
				i = temp.length() - 1;
				while (temp[i] == ' ')
					i--;
				temp = temp.substr(0, i + 1);
				temp = temp + ";";
				end = temp.find(' ', 0);
				attr = attr + temp.substr(0, end) + "#";
				mid = end + 1;
				while (temp[mid] == ' ')
					mid++;
				end = temp.find(' ', mid);
				condition = condition + temp.substr(mid, end - mid) + "#";
				mid = end + 1;
				while (temp[mid] == ' ')
					mid++;
				end = temp.find(';', mid);
				values = values + temp.substr(mid, end - mid) + "#";
				end = sql.find('&', start);
			}
			end = sql.find(';', start);
			temp = sql.substr(start, end - start);
			i = temp.length() - 1;
			while (temp[i] == ' ')
				i--;
			temp = temp.substr(0, i + 1);
			temp = temp + ";";
			end = temp.find(' ', 0);
			attr = attr + temp.substr(0, end);
			mid = end + 1;
			while (temp[mid] == ' ')
				mid++;
			end = temp.find(' ', mid);
			condition = condition + temp.substr(mid, end - mid);
			mid = end + 1;
			while (temp[mid] == ' ')
				mid++;
			end = temp.find(';', mid);
			values = values + temp.substr(mid, end - mid);
		}
		//删除(table,attr,condition,values);
		record_delete(table, attr, condition, values);
		vector<string> index_list;
		index_list = catalog_get_index(table);
		int index_i = 0;
		vector<attr_type> attr_type_list = catalog_get_attr(table);
		for (auto iter = attr_type_list.begin(); iter != attr_type_list.end(); iter++)
		{
			if ((*iter).has_index)
			{
				vector<string> temp_tuple;
				record_final_search(temp_tuple, table, (*iter).attr_name);
				Index index(index_list[index_i++]);
				index.update(temp_tuple);
			}
		}
		//cout << "delete " << table << " " << attr << " " << condition << " " << values << endl;
		cout << "delete successfully!" << endl;
		return;
	}
	else if (op == "50")
	{
		//退出
		cout << "Thank you for using!----Minisql is closing..." << endl;
		exit(0);
	}
	else
	{
		//异常
		cout << "Unexpected errors!" << endl;
		return;
	}
}
