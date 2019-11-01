#include <windows.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <stack>
#include <unordered_set>
#include <regex>
using namespace std;

/*
 * 1.保留字/基本字:key
 * 2.标识符:标记常量、数组、类型、变量、过程、函数名
 * 3.常数:整型、实型、布尔
 * 4.运算符:+-*%/><
 * 5.届符/分隔符:,;(){}[]
 */

 // 1.保留字/基本字:key
const string reserved[62] = {
	"asm", "auto", "bool", "break", "case", "catch", "char", "class",
	"const", "const_cast", "continue", "default", "delete", "do", "double", "dynamic_cast",
	"else", "enum", "explicit", "export", "exterm", "false", "float", "for",
	"friend", "goto", "if", "inline", "int", "long", "mutable", "namespace",
	"new", "operator", "private", "protected", "public", "register", "reinterpret_cast", "return",
	"short", "signed", "sizeof", "static", "static_cast", "struct", "switch", "template",
	"this", "throw", "true", "try", "typedef", "typeid", "typename", "union",
	"unsigned", "using", "virtual", "void", "volatile", "wchar_t"
};
const unordered_set<string> keys(reserved, reserved + 62);

// 3.常数:整型、实型、布尔;
//unordered_set <string> constants;

// 4.运算符:+-*%/><
const string op[30] = { "!", "+","-","*","/","%","&","|","&&","||","=","==", "!=","<",">","<=",">=","++","--", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "<<", ">>", ">>=", "<<=" };
const unordered_set <string> operators(op, op + 30);

// 5.届符/分隔符:,;(){}'"
const char sepa[11] = { ',',':', ';', '(', ')', '{', '}', '[', ']' ,'\"','\'' };
const unordered_set <char> separators(sepa, sepa + 11);

// 变量命名规则
const string variable_name_rules = "^[A-Za-z_][\\dA-Za-z_]*$";
const regex pattern_variable(variable_name_rules, regex::icase);

// number数字规则
// 数字
const string number_rule = "^([+-]?\\d+\\.\\d+)|([+-]?\\d+)|([+-]?\\.\\d+)$";
const regex pattern_number(number_rule, regex::icase);
// 科学计数
const string scientific_rule = "^[+-]?((\\d+\\.?\\d*)|(\\.\\d+))[Ee][+-]?\\d+$";
const regex pattern_scientific(scientific_rule, regex::icase);
// 十六进制
const string hex_rule = "^[+-]?0[xX]([A-Fa-f0-9])+$";
const regex pattern_hex(hex_rule, regex::icase);
// 八进制
//const string oct_rule = "^0([0-7])+$";
//const regex pattern_oct(oct_rule, regex::icase);

// 判断数字
bool isDigit(char unknown);
// 判断字母
bool isLetter(char unknown);
// 判断合法变量名
bool isVariable(string unknown);
// 判断关键字
bool isReserved(string unknown);
// 判断运算符
bool isOperators(string unknown);
// 判断分隔符
bool isSeparators(char unknown);
// 判断常数
bool isConstant(string unknown);
// 判断冗余
bool isRedundancy(char unknown);



//词法分析器
class LexicalAnalyzer
{
private:
	ifstream fin;//输入文件
	ofstream fout;//输出文件
	// 2.标识符:标记常量、数组、类型、变量、过程、函数名
	// unordered_set <string> identifiers;
	stack<char> brackets;//括号
	//// 登记类型、变量、过程、函数名
	//void addIdentifiers(string unknown)
	//{
	//	if (identifiers.count(unknown))
	//	{
	//		identifiers.insert(unknown);
	//	}
	//}

	//匹配括号
	bool matchBrackets(char unknown)
	{
		if (brackets.empty())
		{
			return false;
		}
		char ch = brackets.top();
		brackets.pop();
		if ((ch == '(' && unknown == ')') || (ch == '[' && unknown == ']') || (ch == '{' && unknown == '}'))
		{
			return true;
		}
		else
		{
			cout << "括号不匹配" << unknown << endl;
			return false;
		}
	}

public:
	LexicalAnalyzer(string input, string output) {
		fin.open(input);
		if (!fin) { cout << "Cannot open file" << endl; return; }
		fout.open(output);
		if (!fout) { cout << "Cannot out file" << endl; return; }
	}
	
	void run()
	{
		char ch;
		string s = "";
		while (fin.peek() != EOF) 
		{
			fin.get(ch);

			cout << fin.tellg() << endl;
			// 处理冗余
			if (isRedundancy(ch))
			{
				continue;
			}
			// 预处理
			else if (ch == '#')
			{
				// 读整行
				getline(fin, s);
				fout << "预处理指令" << "#" + s << endl;
				continue;
			}
			else if (ch == '/' && fin.peek() != EOF)
			{
				// 单行注释
				fin.get(ch);
				if (ch == '/')
				{
					getline(fin, s);
					s = "";
					continue;
				}
				// 多行注释
				else if (ch == '*')
				{
					// 匹配*/
					bool flag = true;
					while (getline(fin, s, '/'))
					{
						// 匹配到'*'
						if (s[s.length() - 1] == '*')
						{
							fin.seekg(-1, ios::cur);
							fin.get(ch);
							if (ch == '/')
							{
								ch = ' ';
								flag = false;
								break;
							}
							else
							{
								continue;
							}
						}
					}
					if(flag)
					{
						fout << "未匹配到注释*/" << endl;
						return;
					}
					else
					{
						continue;
					}
				}
				else
				{
					fin.seekg(-1, ios::cur);
					ch = '/';
				}
			}
			// 处理冗余
			if (isRedundancy(ch))
			{
				continue;
			}
			//判断分隔符(括号匹配)
			else if (isSeparators(ch))
			{
				if (ch == '(' || ch == '[' || ch == '{')
				{
					brackets.push(ch);
					fout << "(5,\"" << ch << "\")" << endl;
				}
				else if (ch == ')' || ch == ']' || ch == '}')
				{
					if (!matchBrackets(ch))
					{
						// error
						fout << "匹配" << ch << "失败" << endl;
						return;
					}
					else
					{
						fout << "(5,\"" << ch << "\")" << endl;
					}
				}
				// 判断字符串
				else if (ch == '\"' || ch == '\'')
				{
					if (getline(fin, s, ch))
					{
						// 匹配到temp
						fout << "(3,\"" << ch + s + ch << "\")" << endl;
					}
				}
				else
				{
					fout << "(5,\"" << ch << "\")" << endl;
				}
			}
			// 判断标识符
			else if (isLetter(ch) || ch == '_')
			{
				s = ch;

				cout << fin.tellg() << endl;
				while (fin.peek() != EOF)
				{
					fin.get(ch);
					if (ch == '\'' || ch == '\"')
					{
						fin.seekg(-1, ios::cur);
						string ss;
						fin >> ss;
						fout << "变量名错误" << s + ss << endl;
						return;
					}

					// 判断字符串是否满足命名规则
					if (!isVariable(s + ch))
					{
						fin.seekg(-1, ios::cur);
						// 判断保留字
						if (isReserved(s))
						{
							// 特殊情况
							if (s == "true" || s == "false")
							{
								fout << "(3,\"" << s << "\")" << endl;
							}
							// 保留字
							else
							{
								fout << "(1,\"" << s << "\")" << endl;
							}
						}
						// 标识符
						else
						{
							fout << "(2,\"" << s << "\")" << endl;
						}
						break;
					}
					s += ch;
				}
			}
			//判断数字
			else if (isDigit(ch))
			{
				s = ch;
				while (fin.peek() != EOF)
				{
					fin.get(ch);
					// 判断特殊符号
					if ((isOperators(string(1,ch)) && !isConstant(s + ch + '1')) || isSeparators(ch))
					{
						ch = ' ';
					}
					// 判断冗余
					if (isRedundancy(ch))
					{
						fin.seekg(-1, ios::cur);
						// 判断字符串满足数字规则
						if (isConstant(s))
						{
							fout << "(3,\"" << s << "\")" << endl;
							break;
						}
						else
						{
							fout << "非常量" << s << endl;
							return;
						}
					}
					s += ch;
				}
				continue;
			}
			// 判断运算符
			else if (isOperators(string(1,ch)))
			{
				char temp = ch;
				if (fin.peek() != EOF)
				{
					fin.get(ch);
					if (isOperators(string(1,ch)) && isOperators(string(1,temp) + string(1,ch)))
					{
						char temp2 = ch;
						if (fin.peek() != EOF)
						{
							fin.get(ch);
							if (isOperators(string(1,ch)) && isOperators(string(1,temp) + string(1,temp2) + string(1,ch)))
							{
								fout << "(4,\"" << string(1,temp) + string(1,temp2) + string(1,ch) << "\")" << endl;
							}
							else
							{
								fin.seekg(-1, ios::cur);
								fout << "(4,\"" << string(1,temp) + string(1,temp2) << "\")" << endl;
							}
						}
						else
						{
							fout << "(4,\"" << string(1,temp) + string(1,temp2) << "\")" << endl;
						}
					}
					else
					{
						fin.seekg(-1, ios::cur);
						fout << "(4,\"" << string(1,temp) << "\")" << endl;
					}
				}
				else
				{
					fout << "(4,\"" << string(1,ch) << "\")" << endl;
				}
			}
			else if (ch == '.')
			{
				fout << "(4,\"" << string(1, ch) << "\")" << endl;
			}
			else
			{
				cout << "异常" << endl;
				return;
			}
		}
		if (!brackets.empty()) 
		{
			fout << "括号不匹配" << endl;
		}
	}

	~LexicalAnalyzer()
	{
		fin.close();
		fout.close();
	}

};

int main()
{
	while (true)
	{
		string input;
		cin >> input;
		if (input == "0")
			break;
		string output = "output" + input + ".txt";
		LexicalAnalyzer p("input" + input + ".txt", output);
		p.run();
	}
	system("pause");
	return 0;
}

// 判断数字
bool isDigit(char unknown)
{
	return unknown >= '0' && unknown <= '9';
}

// 判断字母
bool isLetter(char unknown)
{
	return (unknown >= 'A' && unknown <= 'Z') || (unknown >= 'a' && unknown <= 'z');
}

// 判断合法变量名
bool isVariable(string unknown)
{
	return regex_match(unknown, pattern_variable);
}


// 判断保留字
bool isReserved(string unknown)
{
	return keys.count(unknown);
}

// 判断运算符
bool isOperators(string unknown)
{
	return operators.find(unknown) != operators.end();
}

// 判断分隔符
bool isSeparators(char unknown)
{
	return separators.count(unknown);
}

// 判断常数
bool isConstant(string unknown)
{
	return regex_match(unknown, pattern_number) || regex_match(unknown, pattern_scientific) || regex_match(unknown, pattern_hex);
}

// 判断冗余
bool isRedundancy(char unknown)
{
	return unknown == ' ' || unknown == '\r' || unknown == '\n' || unknown == '\t';
}