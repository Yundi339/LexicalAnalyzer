#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <stack>
#include <unordered_set>
#include <regex>
using namespace std;

/*
 * 1.������/������:key
 * 2.��ʶ��:��ǳ��������顢���͡����������̡�������
 * 3.����:���͡�ʵ�͡�����
 * 4.�����:+-*%/><
 * 5.���/�ָ���:,;(){}
 */

 // 1.������/������:key
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

// 2.��ʶ��:��ǳ��������顢���͡����������̡�������
// unordered_set <string> identifiers;

// 3.����:���͡�ʵ�͡�����;
//unordered_set <string> constants;

// 4.�����:+-*%/><
const string op[] = { "+","-","*","/","%","&","|","&&","||","=","!=","<",">","<=",">=","++","--", "+=" };
const unordered_set <string> operators(op, op + 17);

// 5.���/�ָ���:,;(){}
const string sepa[] = { ",", ";", "(", ")", "{", "}" };
const unordered_set <string> separators(sepa, sepa + 6);

// number���ֹ���
// ����
const string number_rule = "^([\\+\\-]?\\d+\\.\\d+)|([\\+\\-]?\\d+)|([\\+\\-]?\\.\\d+)$";
const regex pattern_number(number_rule, regex::icase);
// ��ѧ����
const string scientific_rule = "^[+-]?((\\d+\\.?\\d*)|(\\.\\d+))[Ee][+-]?\\d+$";
const regex pattern_scientific(scientific_rule, regex::icase);
// ʮ������^(0x | 0X)��[a-fA-F0-9] $
const string hex_rule = "^[\+\-]?0[xX]([A-Fa-f0-9])+$";
const regex pattern_hex(hex_rule, regex::icase);
// �˽���^(0x | 0X)��[a-fA-F0-9] $
//const string oct_rule = "^0([0-7])+$";
//const regex pattern_oct(oct_rule, regex::icase);


// �ж�����
bool isDigit(char unknown);
// �ж���ĸ
bool isLetter(char unknown);
// �жϹؼ���
bool isReserved(string unknown);
// �ж�Ԥ����
bool isPretreatment(string unknown);
// �ж������
bool isOperators(string unknown);
// �жϷָ���
bool isSeparators(string unknown);
// �жϱ�ʶ��
bool isIdentifiers(string unknown);
// �ж���������
bool isScientificCounting(string unknown);
// �жϳ���
bool isConstant(string unknown);



//�ʷ�������
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

	//// �Ǽ����͡����������̡�������
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

// �ж�����
bool isDigit(char unknown)
{
	return unknown >= '0' && unknown <= '9';
}

// �ж���ĸ
bool isLetter(char unknown)
{
	return (unknown >= 'A' && unknown <= 'Z') || (unknown >= 'a' && unknown <= 'z');
}

// �жϹؼ���
bool isReserved(string unknown)
{
	return keys.count(unknown);
}

// �ж�Ԥ����
bool isPretreatment(string unknown)
{
	return unknown[0] == '#';
}

// �ж������
bool isOperators(string unknown)
{
	return operators.count(unknown);
}

// �жϷָ���
bool isSeparators(string unknown)
{
	return separators.count(unknown);
}


// �жϱ�ʶ��
bool isIdentifiers(string unknown)
{
	if (!(isLetter(unknown[0]) || unknown[0] == '_'))
		return false;
	for (int i = 1; i < unknown.length(); i++)
		if (!(isLetter(unknown[0]) || isDigit(unknown[0]) || unknown[0] == '_'))
			return false;
	return true;
}

// �ж���������
bool isScientificCounting(string unknown)
{
	return regex_match(unknown, pattern_number) || regex_match(unknown, pattern_scientific) || regex_match(unknown, pattern_hex);
}

// �жϳ���
bool isConstant(string unknown)
{
	return 0;
}