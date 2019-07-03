//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Interpreter_Module                   ///
///       Produced by: LTC                             ///
///       Description: Produced to deal with SQL parse ///
///       date: 2018/6/16                              ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

#include"Interpreter.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//读取用户输入
string read_input()
{
	string SQL;
	string temp;
	char str[100];
	while (1)
	{
		cin.getline(str, 1000);
		temp = str;
		SQL = SQL + " " + temp;
		if (SQL[SQL.length() - 1] == ';')
		{
			SQL[SQL.length() - 1] = ' ';
			SQL += ";";
			break;
		}
		cout << "MiniSQL-->> ";
	}
	//引号消除
	/*for(int i=0; i<SQL.length()-1; i++)
	if((SQL[i]==34)||(SQL[i]==39))
	SQL[i]=' ';*/
	//返回用户输入
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create database语句是否有效
string create_database(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error12:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//返回drop database语句的内部形式
		else
			SQL = "00" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获得属性
string get_attribute(string temp, string sql)
{
	int start = 0, end, index;
	string T, C;
	temp += " ";
	//获得属性名
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	sql += T + " ";
	//获得数据类型
	while (temp[start] == ' ')
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (T.empty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//若为int 
	else if (T == "int")
		sql += "+";
	//若为float
	else if (T == "float")
		sql += "-";
	//其他
	else
	{
		index = T.find('(');
		C = T.substr(0, index);
		index++;
		//若有误，打印出错信息
		if (C.empty())
		{
			cout << "error: " << T << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//若为char
		else if (C == "char")
		{
			C = T.substr(index, T.length() - index - 1);
			if (C.empty())
			{
				cout << "error: the length of the data type char has not been specified!" << endl;
				sql = "99";
				return sql;
			}
			else
				sql += C;
		}
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << C << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//是否有附加信息
	while (start<temp.length() && temp[start] == ' ')
		start++;
	if (start<temp.length())
	{
		//若为unique定义，保存信息
		end = temp.find(' ', start);
		T = temp.substr(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << temp << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//若无附加信息
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create table语句是否有效
string create_table(string SQL, int start)
{
	string temp, sql, T;
	int index, end, length;
	//获取表名
	while (SQL[start] == ' ')
		start++;
	index = start;
	if ((end = SQL.find('(', start)) == -1)
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	temp = SQL.substr(start, end - start);
	start = end + 1;
	if (!temp.empty())
	{
		while (SQL[start] == ' ')
			start++;
		length = temp.length() - 1;
		while (temp[length] == ' ')
			length--;
		temp = temp.substr(0, length + 1);
	}
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//若为非法信息，打印出错信息
	else if (temp.find(' ') != -1)
	{
		cout << "error: " << temp << "---is not a valid table name!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		//获取每个属性
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//若有空属性，打印出错信息
			if (temp.empty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//保存属性
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL[start] == ' ')
				start++;
		}
		//获得最后属性
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp[length] != ')'&&length >= 0)
			length--;
		//若无，打印出错信息
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//存储属性
		else
		{
			temp = temp.substr(0, length);
			end = SQL.find(' ', start);
			T = SQL.substr(start, end - start);
			start = end + 1;
			//若为主键定义
			if (T == "primary")
			{
				//判断是否有关键字key
				temp += ")";
				while (SQL[start] == ' ')
					start++;
				end = SQL.find('(', start);
				T = SQL.substr(start, end - start);
				start = end + 1;
				length = T.length() - 1;
				while (T[length] == ' ')
					length--;
				T = T.substr(0, length + 1);
				//若为空，打印出错信息
				if (T.empty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word key!" << endl;
					SQL = "99";
					return SQL;
				}
				//若有，继续验证
				else if (T == "key")
				{
					//获取主键属性名
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(')', start);
					T = SQL.substr(start, end - start);
					length = T.length() - 1;
					while (T[length] == ' ')
						length--;
					T = T.substr(0, length + 1);
					//若无，打印出错信息
					if (T.empty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//若为非法信息，打印出错信息
					else if (T.find(' ') != -1)
					{
						cout << "error : " << T << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//保存主键
					else
					{
						sql += T + " #,";
						SQL = "01 " + sql + ";";
					}
				}
				//若为非法信息，打印出错信息
				else
				{
					cout << "error : " << T << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//若为一般属性
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
				{
					SQL = "99";
					return SQL;
				}
				else
					SQL = "01 " + sql + ";";
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create index on语句是否有效
string create_index_on(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	//获取表名
	while (SQL[start] == ' ')
		start++;
	end = SQL.find('(', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		cout << "\t missing ( !" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		//检验是否为有效文件名
		length = temp.length() - 1;
		while (temp[length] == ' ')
			length--;
		temp = temp.substr(0, length + 1);
		//有效
		if (temp.find(' ') == -1)
		{
			sql += " " + temp;
			//获取属性名
			while (SQL[start] == ' ')
				start++;
			end = SQL.find(')', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//若无，打印出错信息
			if (temp.empty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ) !" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				//检验是否为有效属性名
				length = temp.length() - 1;
				while (temp[length] == ' ')
					length--;
				temp = temp.substr(0, length + 1);
				//有效
				if (temp.find(' ') == -1)
				{
					sql += " " + temp;
					while (SQL[start] == ' ')
						start++;
					if (SQL[start] != ';' || start != SQL.length() - 1)
					{
						cout << "syntax error: syntax error for quit!" << endl;
						SQL = "99";
						return SQL;
					}
					//返回create index语句的内部形式
					else
						SQL = "02 " + sql + ";";
				}
				//无效,打印出错信息
				else
				{
					cout << "error:" << " " << temp << "---is not a valid attribute name!" << endl;
					SQL = "99";
					return SQL;
				}
			}
		}
		//无效,打印出错信息
		else
		{
			cout << "error:" << " " << temp << "---is not a valid table name!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create index语句是否有效
string create_index(string SQL, int start)
{
	string temp, sql;
	int end;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;
		//获取第四个单词
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//若无，打印出错信息
		if (temp.empty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//若为on,继续验证
		else if (temp == "on")
			SQL = create_index_on(SQL, start, sql);
		//若为非法信息，打印非法信息
		else
		{
			cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create语句是否有效
string create_clause(string SQL, int start)
{
	string temp;
	int end;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//若为database,继续验证
	else if (temp == "database")
		SQL = create_database(SQL, start);
	//若为table,继续验证
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//若为index,继续验证
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回create语句的内部形式
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop database语句是否有效
string drop_database(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//返回drop database语句的内部形式
		else
			SQL = "10" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop table语句是否有效
string drop_table(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid table name!" << endl;
			SQL = "99";
		}
		//返回drop table语句的内部形式
		else
			SQL = "11 " + temp + ";";
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop index语句是否有效
string drop_index(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: index name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid index name!" << endl;
			SQL = "99";
		}
		//返回drop index语句的内部形式
		else
			SQL = "12 " + temp + ";";
	}
	return SQL;
}

//验证drop语句是否有效
string drop_clause(string SQL, int start)
{
	string temp;
	int end;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	//若为database,继续验证
	else if (temp == "database")
		SQL = drop_database(SQL, start);
	//若为table,继续验证
	else if (temp == "table")
		SQL = drop_table(SQL, start);
	//若为index,继续验证
	else if (temp == "index")
		SQL = drop_index(SQL, start);
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回drop语句的内部形式
	return SQL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//验证select 语句是否有效
string select_clause(string SQL, int start)
{
	string temp, sql = "";
	int end;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(',', start);
	while (end != -1)
	{
		temp = SQL.substr(start, end - start);
		start = end + 1;
		if (temp.empty())
		{
			cout << "syntax error: syntax error for select statement!" << endl;
			SQL = "99";
		}
		sql = sql + " " + temp;
		end = SQL.find(',', start);
	}
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = sql + " " + temp + ",";
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		if (temp.empty())
		{
			cout << "syntax error: syntax error for select statement!" << endl;
			SQL = "99";
		}
		else if (temp != "from")
		{
			cout << "syntax error: syntax error for missing the keyword from!" << endl;
			SQL = "99";
		}
		else
		{
			while (SQL[start] == ' ')
				start++;
			end = SQL.find(' ', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			if (temp.empty())
			{
				cout << "syntax error: syntax error for select statement!" << endl;
				SQL = "99";
			}
			else
			{
				sql = sql + temp + ",";
				while (SQL[start] == ' ')
					start++;
				if (SQL[start] == ';'&&start == SQL.length() - 1)
				{
					SQL = "20" + sql + "*;";
					return SQL;
				}
				end = SQL.find(' ', start);
				temp = SQL.substr(start, end - start);
				start = end + 1;
				if (temp.empty())
				{
					cout << "syntax error: syntax error for select statement!" << endl;
					SQL = "99";
				}
				else if (temp != "where")
				{
					cout << "syntax error: syntax error for missing the keyword where!" << endl;
					SQL = "99";
				}
				else
				{
					SQL = select_condition(SQL, start);
					if (SQL == "99")
						return SQL;
					else
					{
						SQL = "20" + sql + SQL + ";";
						return SQL;
					}
				}
			}
		}
	}
	return SQL;
}


///////////////////////////////////////////////////////////////////////////////////////////
//获取选择条件
string select_condition(string SQL, int start)
{
	string temp, res = " ";
	int end, mid;
	end = SQL.find("and", start);
	while (end != -1)
	{
		temp = SQL.substr(start, end - start);
		if ((mid = temp.find("<>", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " <> ";
			res += temp.substr(mid + 2, temp.length() - mid - 2) + "&";
			start = end + 4;
			end = SQL.find("and", start);
		}
		else if ((mid = temp.find("<=", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " <= ";
			res += temp.substr(mid + 2, temp.length() - mid - 2) + "&";
			start = end + 4;
			end = SQL.find("and", start);
		}
		else if ((mid = temp.find(">=", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " >= ";
			res += temp.substr(mid + 2, temp.length() - mid - 2) + "&";
			start = end + 4;
			end = SQL.find("and", start);
		}
		else if ((mid = temp.find("<", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " < ";
			res += temp.substr(mid + 1, temp.length() - mid - 1) + "&";
			start = end + 4;
			end = SQL.find("and", start);
		}
		else if ((mid = temp.find(">", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " > ";
			res += temp.substr(mid + 1, temp.length() - mid - 1) + "&";
			start = end + 4;
			end = SQL.find("and", start);
		}
		else if ((mid = temp.find("=", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " = ";
			res += temp.substr(mid + 1, temp.length() - mid - 1) + "&";
			start = end + 4;
			end = SQL.find("and", start);
		}
		else
		{
			cout << "syntax error: syntax error for op!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	end = SQL.find(";", start);
	if (end != -1)
	{
		temp = SQL.substr(start, end - start);
		if ((mid = temp.find("<>", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " <> ";
			res += temp.substr(mid + 2, temp.length() - mid - 2);
		}
		else if ((mid = temp.find("<=", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " <= ";
			res += temp.substr(mid + 2, temp.length() - mid - 2);
		}
		else if ((mid = temp.find(">=", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " >= ";
			res += temp.substr(mid + 2, temp.length() - mid - 2);
		}
		else if ((mid = temp.find("<", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " < ";
			res += temp.substr(mid + 1, temp.length() - mid - 1);
		}
		else if ((mid = temp.find(">", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " > ";
			res += temp.substr(mid + 1, temp.length() - mid - 1);
		}
		else if ((mid = temp.find("=", 0)) != -1)
		{
			res = res + temp.substr(0, mid);
			res += " = ";
			res += temp.substr(mid + 1, temp.length() - mid - 1);
		}
		else
		{
			cout << "syntax error: syntax error for op!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	else
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
		return SQL;
	}
	if (end != SQL.length() - 1)
	{
		cout << "syntax error: syntax error for select statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		return res;
	}
}


//验证insert 语句是否有效
string insert_clause(string SQL, int start)
{
	string temp, sql = " ";
	int end;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	if (temp != "into")
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		if (temp.empty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			sql = sql + temp;
			while (SQL[start] == ' ')
				start++;
			end = SQL.find('(', start);
			int t = end;
			while (SQL[end - 1] == ' ')
				end--;
			temp = SQL.substr(start, end - start);
			start = t + 1;
			if (temp != "values")
			{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				while (SQL[start] == ' ')
					start++;
				end = SQL.find(',', start);
				while (end != -1)
				{
					temp = SQL.substr(start, end - start);
					start = end + 1;
					if (temp.empty())
					{
						cout << "syntax error: syntax error for insert statement!" << endl;
						SQL = "99";
						return SQL;
					}
					else
					{
						sql = sql + ',' + temp;
					}
					end = SQL.find(',', start);
				}
				end = SQL.find(')', start);
				temp = SQL.substr(start, end - start);
				start = end + 1;
				if (temp.empty())
				{
					cout << "syntax error: syntax error for insert statement!" << endl;
					SQL = "99";
					return SQL;
				}
				else
				{
					sql = sql + ',' + temp;
				}
				while (SQL[start] == ' ')
					start++;
				if (SQL[start] != ';')
				{
					cout << "syntax error: syntax error for insert statement!" << endl;
					SQL = "99";
					return SQL;
				}
				else
					return "30" + sql + ",;";
			}
		}
	}
}


//验证delete语句是否有效
string delete_clause(string SQL, int start)
{
	string temp, sql = " ";
	int end;
	//获取第二个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	if (temp != "from")
	{
		cout << "syntax error: syntax error for delete statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		if (temp.empty())
		{
			cout << "syntax error: syntax error for delete statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			sql = sql + temp + ",";
			while (SQL[start] == ' ')
				start++;
			if (SQL[start] == ';')
			{
				sql = "40" + sql + "*" + ";";
				return sql;

			}
			else
			{
				end = SQL.find(' ', start);
				temp = SQL.substr(start, end - start);
				start = end + 1;
				if (temp != "where")
				{
					cout << "syntax error: syntax error for delete statement!" << endl;
					SQL = "99";
					return SQL;
				}
				else
				{
					while (SQL[start] == ' ')
						start++;
					SQL = select_condition(SQL, start);
					if (SQL == "99")
					{
						cout << "syntax error: syntax error for delete statement!" << endl;
						return SQL;
					}
					else
					{
						sql = "40" + sql + SQL + ";";
						return sql;
					}
				}
			}
		}
	}
}



//验证quit语句是否有效
string quit_clause(string SQL, int start)
{
	while (SQL[start] == ' ')
		start++;
	if (SQL[start] != ';')
	{
		cout << "syntax error: syntax error for quit statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		return "50 ;";
	}
}


//验证execfile语句是否有效
string execfile_clause(string SQL, int start)
{
	string temp, res, sqll,sqll2;
	int end, state;
	char filename[100], sql[200];
	FILE *fp;
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	if (temp.empty())
	{
		cout << "syntax error: syntax error for delete statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		if (SQL[start] != ';')
		{
			cout << "syntax error: syntax error for execfile statement!" << endl;
			SQL = "99";
			return SQL;
		}
		strcpy_s(filename, temp.c_str());
		state = fopen_s(&fp, filename, "r");
		if (!state)
		{
			sqll2 = "";
			while (!feof(fp))
			{
				fgets(sql, 200, fp);
				sqll = sql;
				if ((sqll[sqll.length() - 2] != ';')&&(sqll[sqll.length() - 1] != ';'))
				{
					sqll[sqll.length() - 1] = ' ';
					sqll2 += sqll;
					sqll = "";
					continue;
				}
				else
				{
					sqll2 += sqll;
					SQL = Interpreter(sqll2);
					res += SQL;
					sqll2 = "";
					sqll = "";
				}				
			}
			fclose(fp);
			return res;
		}
		else
		{
			cout << "cannot open the file!" << endl;
			SQL = "99";
			return SQL;
		}

	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
//获取用户输入，并对输入作有效性检查，若正确，返回语句的内部表示形式
string Interpreter(string statement)
{
	string SQL;
	string temp;
	string sql;
	int start = 0, end;
	if (statement.empty())
		//获取用户输入
		SQL = read_input();
	else
	{
		SQL = statement;
		if (SQL[SQL.length() - 1] == ';')
		{
			SQL[SQL.length() - 1] = ' ';
			SQL = SQL + ";";
		}
		else
		{
			SQL[SQL.length() - 1] = ';';
			SQL[SQL.length() - 2] = ' ';
		}
	}
	//获取输入的第一个单词
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无输入，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//若为create语句
	else if (temp == "create")
		SQL = create_clause(SQL, start);
	//若为drop语句
	else if (temp == "drop")
		SQL = drop_clause(SQL, start);
	//若为select语句
	else if (temp == "select")
		SQL = select_clause(SQL, start);
	//若为insert语句
	else if (temp == "insert")
		SQL = insert_clause(SQL, start);
	//若为delete语句
	else if (temp == "delete")
		SQL = delete_clause(SQL, start);
	//若为execfile语句
	else if (temp == "execfile")
		SQL = execfile_clause(SQL, start);
	//若为quit语句
	else if (temp == "quit")
		SQL = quit_clause(SQL, start);
	//若为非法语句
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回输入语句的内部形式
	return SQL;
}
