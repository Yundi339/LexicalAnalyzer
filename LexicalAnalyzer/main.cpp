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
const string op[21] = { "+","-","*","/","%","&","|","&&","||","=","!=","<",">","<=",">=","++","--", "+=", "-=", "<<", ">>" };
const unordered_set <string> operators(op, op + 21);

// 5.届符/分隔符:,;(){}
const string sepa[9] = { ",", ";", "(", ")", "{", "}", "[", "]" ,"\""};
const unordered_set <string> separators(sepa, sepa + 8);

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
bool isSeparators(string unknown);
// 判断常数
bool isConstant(string unknown);



//词法分析器
class LexicalAnalyzer
{
private:
	ifstream fin;
	ofstream fout;
	int a = 2;
	// 2.标识符:标记常量、数组、类型、变量、过程、函数名
	// unordered_set <string> identifiers;
public:
	LexicalAnalyzer(string input, string output) {
		fin.open(input);
		if (!fin) { cout << "Cannot open file" << endl; return; }
		fout.open(output);
		if (!fout) { cout << "Cannot out file" << endl; return; }
	}

	void run()
	{
		// mode状态：
		// 0: 指令清空
		// 1: 首字符为标识符
		// 2: 首字符为数字
		// 3: 读字符串模式
		// 4: 寻找括号模式
		// 5: 寻找结束符
		char ch;
		string s = "";
		while (fin.peek() != EOF) {
			fin.get(ch);
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
			// 判断标识符
			else if (isLetter(ch) || ch == '_')
			{
				s = ch;
				while (fin.peek() != EOF)
				{
					fin.get(ch);
					// 判断字符串是否满足命名规则
					if (!isVariable(s += ch))
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
					}
					s += ch;
				}
				continue;
			}
			//判断数字
			else if (isDigit(ch))
			{
				s = ch;
				while (fin.peek() != EOF)
				{
					fin.get(ch);
					if (isRedundancy(ch)) continue;
					// 判断字符串是否满足命名规则
					if (!isConstant(s += ch))
					{
						fin.seekg(-1, ios::cur);
						fout << "(3,\"" << s << "\")" << endl;
					}
					s += ch;
				}
				continue;

			}
			// 判断运算符
			else if (isOperators(to_string(ch)))
			{

			}
			else
			{
				cout << "异常" << endl;
			}
			continue;



			// 匹配注释
			if (s == "/")
			{
				// 单行注释
				if (ch == '/')
				{
					getline(fin, s);
					s = "";
					mode = 0;
					continue;
				}
				// 多行注释
				else if (ch == '*')
				{
					// 匹配*/
					bool flag_com = true;
					while (flag_com)
					{
						// 匹配到'*'
						if (getline(fin, s, '*') && fin.peek() != EOF)
						{
							fin.get(ch);
							if (ch == '/') {
								flag_com = false;
							}
						}
						else {
							break;
						}
					}
					if (flag_com)
					{
						//error 未匹配
						cout << "未匹配到注释/**/" << endl;
					}
					else
					{
						cout << "匹配注释/**/" << endl;
					}
				}
			}

			//不读字符串或字符
			if (mode != 3)
			{
				// 吃冗余
				if (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
				{
					continue;
				}
				else if (ch == '/')
				{
					s = "/";

				}
			}


		}
	}

	//// 登记类型、变量、过程、函数名
	//void addIdentifiers(string unknown)
	//{
	//	if (identifiers.count(unknown))
	//	{
	//		identifiers.insert(unknown);
	//	}
	//}

	~LexicalAnalyzer() {
		fin.close();
		fout.close();
	}

};

int main()
{
	//while (1) {
	//	string n;
	//	cin >> n;
	//	cout << isVariable(n) << endl;
	//}
	string input = "input1.txt";
	string output = "output.txt";
	LexicalAnalyzer p(input, output);
	p.run();
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
	return operators.count(unknown);
}

// 判断分隔符
bool isSeparators(string unknown)
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