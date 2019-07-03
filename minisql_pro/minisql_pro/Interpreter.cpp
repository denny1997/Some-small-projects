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
//��ȡ�û�����
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
	//��������
	/*for(int i=0; i<SQL.length()-1; i++)
	if((SQL[i]==34)||(SQL[i]==39))
	SQL[i]=' ';*/
	//�����û�����
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create database����Ƿ���Ч
string create_database(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error12:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����drop database�����ڲ���ʽ
		else
			SQL = "00" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�������
string get_attribute(string temp, string sql)
{
	int start = 0, end, index;
	string T, C;
	temp += " ";
	//���������
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	sql += T + " ";
	//�����������
	while (temp[start] == ' ')
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (T.empty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//��Ϊint 
	else if (T == "int")
		sql += "+";
	//��Ϊfloat
	else if (T == "float")
		sql += "-";
	//����
	else
	{
		index = T.find('(');
		C = T.substr(0, index);
		index++;
		//�����󣬴�ӡ������Ϣ
		if (C.empty())
		{
			cout << "error: " << T << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//��Ϊchar
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
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << C << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//�Ƿ��и�����Ϣ
	while (start<temp.length() && temp[start] == ' ')
		start++;
	if (start<temp.length())
	{
		//��Ϊunique���壬������Ϣ
		end = temp.find(' ', start);
		T = temp.substr(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << temp << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//���޸�����Ϣ
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create table����Ƿ���Ч
string create_table(string SQL, int start)
{
	string temp, sql, T;
	int index, end, length;
	//��ȡ����
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
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
	else if (temp.find(' ') != -1)
	{
		cout << "error: " << temp << "---is not a valid table name!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		//��ȡÿ������
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���п����ԣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//��������
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL[start] == ' ')
				start++;
		}
		//����������
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp[length] != ')'&&length >= 0)
			length--;
		//���ޣ���ӡ������Ϣ
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//�洢����
		else
		{
			temp = temp.substr(0, length);
			end = SQL.find(' ', start);
			T = SQL.substr(start, end - start);
			start = end + 1;
			//��Ϊ��������
			if (T == "primary")
			{
				//�ж��Ƿ��йؼ���key
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
				//��Ϊ�գ���ӡ������Ϣ
				if (T.empty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word key!" << endl;
					SQL = "99";
					return SQL;
				}
				//���У�������֤
				else if (T == "key")
				{
					//��ȡ����������
					while (SQL[start] == ' ')
						start++;
					end = SQL.find(')', start);
					T = SQL.substr(start, end - start);
					length = T.length() - 1;
					while (T[length] == ' ')
						length--;
					T = T.substr(0, length + 1);
					//���ޣ���ӡ������Ϣ
					if (T.empty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					else if (T.find(' ') != -1)
					{
						cout << "error : " << T << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��������
					else
					{
						sql += T + " #,";
						SQL = "01 " + sql + ";";
					}
				}
				//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
				else
				{
					cout << "error : " << T << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//��Ϊһ������
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
//��֤create index on����Ƿ���Ч
string create_index_on(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	//��ȡ����
	while (SQL[start] == ' ')
		start++;
	end = SQL.find('(', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		cout << "\t missing ( !" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		//�����Ƿ�Ϊ��Ч�ļ���
		length = temp.length() - 1;
		while (temp[length] == ' ')
			length--;
		temp = temp.substr(0, length + 1);
		//��Ч
		if (temp.find(' ') == -1)
		{
			sql += " " + temp;
			//��ȡ������
			while (SQL[start] == ' ')
				start++;
			end = SQL.find(')', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���ޣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ) !" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				//�����Ƿ�Ϊ��Ч������
				length = temp.length() - 1;
				while (temp[length] == ' ')
					length--;
				temp = temp.substr(0, length + 1);
				//��Ч
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
					//����create index�����ڲ���ʽ
					else
						SQL = "02 " + sql + ";";
				}
				//��Ч,��ӡ������Ϣ
				else
				{
					cout << "error:" << " " << temp << "---is not a valid attribute name!" << endl;
					SQL = "99";
					return SQL;
				}
			}
		}
		//��Ч,��ӡ������Ϣ
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
//��֤create index����Ƿ���Ч
string create_index(string SQL, int start)
{
	string temp, sql;
	int end;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;
		//��ȡ���ĸ�����
		while (SQL[start] == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//��Ϊon,������֤
		else if (temp == "on")
			SQL = create_index_on(SQL, start, sql);
		//��Ϊ�Ƿ���Ϣ����ӡ�Ƿ���Ϣ
		else
		{
			cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create����Ƿ���Ч
string create_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//��Ϊdatabase,������֤
	else if (temp == "database")
		SQL = create_database(SQL, start);
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����create�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop database����Ƿ���Ч
string drop_database(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����drop database�����ڲ���ʽ
		else
			SQL = "10" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop table����Ƿ���Ч
string drop_table(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid table name!" << endl;
			SQL = "99";
		}
		//����drop table�����ڲ���ʽ
		else
			SQL = "11 " + temp + ";";
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop index����Ƿ���Ч
string drop_index(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL[start] == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: index name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL[start] == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL[start] != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2) << "---is not a valid index name!" << endl;
			SQL = "99";
		}
		//����drop index�����ڲ���ʽ
		else
			SQL = "12 " + temp + ";";
	}
	return SQL;
}

//��֤drop����Ƿ���Ч
string drop_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	//��Ϊdatabase,������֤
	else if (temp == "database")
		SQL = drop_database(SQL, start);
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = drop_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = drop_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����drop�����ڲ���ʽ
	return SQL;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//��֤select ����Ƿ���Ч
string select_clause(string SQL, int start)
{
	string temp, sql = "";
	int end;
	//��ȡ�ڶ�������
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
	//���ޣ���ӡ������Ϣ
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
//��ȡѡ������
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


//��֤insert ����Ƿ���Ч
string insert_clause(string SQL, int start)
{
	string temp, sql = " ";
	int end;
	//��ȡ�ڶ�������
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


//��֤delete����Ƿ���Ч
string delete_clause(string SQL, int start)
{
	string temp, sql = " ";
	int end;
	//��ȡ�ڶ�������
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



//��֤quit����Ƿ���Ч
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


//��֤execfile����Ƿ���Ч
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
//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
string Interpreter(string statement)
{
	string SQL;
	string temp;
	string sql;
	int start = 0, end;
	if (statement.empty())
		//��ȡ�û�����
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
	//��ȡ����ĵ�һ������
	while (SQL[start] == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//�������룬��ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//��Ϊcreate���
	else if (temp == "create")
		SQL = create_clause(SQL, start);
	//��Ϊdrop���
	else if (temp == "drop")
		SQL = drop_clause(SQL, start);
	//��Ϊselect���
	else if (temp == "select")
		SQL = select_clause(SQL, start);
	//��Ϊinsert���
	else if (temp == "insert")
		SQL = insert_clause(SQL, start);
	//��Ϊdelete���
	else if (temp == "delete")
		SQL = delete_clause(SQL, start);
	//��Ϊexecfile���
	else if (temp == "execfile")
		SQL = execfile_clause(SQL, start);
	//��Ϊquit���
	else if (temp == "quit")
		SQL = quit_clause(SQL, start);
	//��Ϊ�Ƿ����
	else
	{
		cout << "syntax error:" << " " << temp << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//�������������ڲ���ʽ
	return SQL;
}
