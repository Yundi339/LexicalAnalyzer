#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <stack>
#include <unordered_set>
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
unordered_set <string> identifiers;

// 3.����:���͡�ʵ�͡�����;
unordered_set <string> constants;

// 4.�����:+-*%/><
const string op[] = {
	"+","-","*","/","%","&","|","&&","||","=","!=","<",">","<=",">="
};
unordered_set <string> operators;

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

	~LexicalAnalyzer() {
		fin.close();
		fout.close();
	}

};


int main()
{
	//cout << "hello world" << endl;
	string input = "input1.txt";
	string output = "output.txt";
	LexicalAnalyzer p(input, output);
	p.run();
	return 0;
}