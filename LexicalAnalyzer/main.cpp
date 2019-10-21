#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <stack>
#include <unordered_set>
#include <regex>
using namespace std;

/*
 * 1.保留字/保留字:key
 * 2.标识符:标记常量、数组、类型、变量、过程、函数名
 * 3.常数:整型、实型、布尔
 * 4.运算符:+-*%/><
 * 5.届符/分隔符:,;(){}
 */

 // 1.保留字/保留字:key
const string reserved[] = {
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

// 2.标识符:标记常量、数组、类型、变量、过程、函数名
// unordered_set <string> identifiers;

// 3.常数:整型、实型、布尔;
//unordered_set <string> constants;

// 4.运算符:+-*%/><
const string op[] = { "+","-","*","/","%","&","|","&&","||","=","!=","<",">","<=",">=","++","--", "+=" };
const unordered_set <string> operators(op, op + 17);

// 5.届符/分隔符:,;(){}
const string sepa[] = { ",", ";", "(", ")", "{", "}" };
const unordered_set <string> separators(sepa, sepa + 6);

// number数字规则
// 数字
const string number_rule = "^([\\+\\-]?\\d+\\.\\d+)|([\\+\\-]?\\d+)|([\\+\\-]?\\.\\d+)$";
const regex pattern_number(number_rule, regex::icase);
// 科学计数
const string scientific_rule = "^[+-]?((\\d+\\.?\\d*)|(\\.\\d+))[Ee][+-]?\\d+$";
const regex pattern_scientific(scientific_rule, regex::icase);
// 十六进制^(0x | 0X)？[a-fA-F0-9] $
const string hex_rule = "^[\+\-]?0[xX]([A-Fa-f0-9])+$";
const regex pattern_hex(hex_rule, regex::icase);
// 八进制^(0x | 0X)？[a-fA-F0-9] $
//const string oct_rule = "^0([0-7])+$";
//const regex pattern_oct(oct_rule, regex::icase);


// 判断数字
bool isDigit(char unknown);
// 判断字母
bool isLetter(char unknown);
// 判断关键字
bool isReserved(string unknown);
// 判断预处理
bool isPretreatment(string unknown);
// 判断运算符
bool isOperators(string unknown);
// 判断分隔符
bool isSeparators(string unknown);
// 判断标识符
bool isIdentifiers(string unknown);
// 判断所有数字
bool isScientificCounting(string unknown);
// 判断常数
bool isConstant(string unknown);



//词法分析器
class LexicalAnalyzer
{
private:
	ifstream fin;
	ofstream fout;
	int a = 2;
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
		while (fin.peek() != EOF) {
			fin.get(ch);
			fout << ch;
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
	while (1) {
		string n;
		cin >> n;
		cout << isScientificCounting(n) << endl;
	}
	//string input = "input1.txt";
	//string output = "output.txt";
	//LexicalAnalyzer p(input, output);
	//p.run();
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

// 判断关键字
bool isReserved(string unknown)
{
	return keys.count(unknown);
}

// 判断预处理
bool isPretreatment(string unknown)
{
	return unknown[0] == '#';
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


// 判断标识符
bool isIdentifiers(string unknown)
{
	if (!(isLetter(unknown[0]) || unknown[0] == '_'))
		return false;
	for (int i = 1; i < unknown.length(); i++)
		if (!(isLetter(unknown[0]) || isDigit(unknown[0]) || unknown[0] == '_'))
			return false;
	return true;
}

// 判断所有数字
bool isScientificCounting(string unknown)
{
	return regex_match(unknown, pattern_number) || regex_match(unknown, pattern_scientific) || regex_match(unknown, pattern_hex);
}

// 判断常数
bool isConstant(string unknown)
{
	return 0;
}