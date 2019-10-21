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
 * 5.���/�ָ���:,;(){}[]
 */

 // 1.������/������:key
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

// 3.����:���͡�ʵ�͡�����;
//unordered_set <string> constants;

// 4.�����:+-*%/><
const string op[21] = { "+","-","*","/","%","&","|","&&","||","=","!=","<",">","<=",">=","++","--", "+=", "-=", "<<", ">>" };
const unordered_set <string> operators(op, op + 21);

// 5.���/�ָ���:,;(){}
const string sepa[9] = { ",", ";", "(", ")", "{", "}", "[", "]" ,"\""};
const unordered_set <string> separators(sepa, sepa + 8);

// ������������
const string variable_name_rules = "^[A-Za-z_][\\dA-Za-z_]*$";
const regex pattern_variable(variable_name_rules, regex::icase);

// number���ֹ���
// ����
const string number_rule = "^([+-]?\\d+\\.\\d+)|([+-]?\\d+)|([+-]?\\.\\d+)$";
const regex pattern_number(number_rule, regex::icase);
// ��ѧ����
const string scientific_rule = "^[+-]?((\\d+\\.?\\d*)|(\\.\\d+))[Ee][+-]?\\d+$";
const regex pattern_scientific(scientific_rule, regex::icase);
// ʮ������
const string hex_rule = "^[+-]?0[xX]([A-Fa-f0-9])+$";
const regex pattern_hex(hex_rule, regex::icase);
// �˽���
//const string oct_rule = "^0([0-7])+$";
//const regex pattern_oct(oct_rule, regex::icase);

// �ж�����
bool isDigit(char unknown);
// �ж���ĸ
bool isLetter(char unknown);
// �жϺϷ�������
bool isVariable(string unknown);
// �жϹؼ���
bool isReserved(string unknown);
// �ж������
bool isOperators(string unknown);
// �жϷָ���
bool isSeparators(string unknown);
// �жϳ���
bool isConstant(string unknown);



//�ʷ�������
class LexicalAnalyzer
{
private:
	ifstream fin;
	ofstream fout;
	int a = 2;
	// 2.��ʶ��:��ǳ��������顢���͡����������̡�������
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
		// mode״̬��
		// 0: ָ�����
		// 1: ���ַ�Ϊ��ʶ��
		// 2: ���ַ�Ϊ����
		// 3: ���ַ���ģʽ
		// 4: Ѱ������ģʽ
		// 5: Ѱ�ҽ�����
		char ch;
		string s = "";
		while (fin.peek() != EOF) {
			fin.get(ch);
			// ��������
			if (isRedundancy(ch))
			{
				continue;
			}
			// Ԥ����
			else if (ch == '#')
			{
				// ������
				getline(fin, s);
				fout << "Ԥ����ָ��" << "#" + s << endl;
				continue;
			}
			// �жϱ�ʶ��
			else if (isLetter(ch) || ch == '_')
			{
				s = ch;
				while (fin.peek() != EOF)
				{
					fin.get(ch);
					// �ж��ַ����Ƿ�������������
					if (!isVariable(s += ch))
					{
						fin.seekg(-1, ios::cur);
						// �жϱ�����
						if (isReserved(s))
						{
							// �������
							if (s == "true" || s == "false")
							{
								fout << "(3,\"" << s << "\")" << endl;
							}
							// ������
							else
							{
								fout << "(1,\"" << s << "\")" << endl;
							}
						}
						// ��ʶ��
						else
						{
							fout << "(2,\"" << s << "\")" << endl;
						}
					}
					s += ch;
				}
				continue;
			}
			//�ж�����
			else if (isDigit(ch))
			{
				s = ch;
				while (fin.peek() != EOF)
				{
					fin.get(ch);
					if (isRedundancy(ch)) continue;
					// �ж��ַ����Ƿ�������������
					if (!isConstant(s += ch))
					{
						fin.seekg(-1, ios::cur);
						fout << "(3,\"" << s << "\")" << endl;
					}
					s += ch;
				}
				continue;

			}
			// �ж������
			else if (isOperators(to_string(ch)))
			{

			}
			else
			{
				cout << "�쳣" << endl;
			}
			continue;



			// ƥ��ע��
			if (s == "/")
			{
				// ����ע��
				if (ch == '/')
				{
					getline(fin, s);
					s = "";
					mode = 0;
					continue;
				}
				// ����ע��
				else if (ch == '*')
				{
					// ƥ��*/
					bool flag_com = true;
					while (flag_com)
					{
						// ƥ�䵽'*'
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
						//error δƥ��
						cout << "δƥ�䵽ע��/**/" << endl;
					}
					else
					{
						cout << "ƥ��ע��/**/" << endl;
					}
				}
			}

			//�����ַ������ַ�
			if (mode != 3)
			{
				// ������
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

// �жϺϷ�������
bool isVariable(string unknown)
{
	return regex_match(unknown, pattern_variable);
}


// �жϱ�����
bool isReserved(string unknown)
{
	return keys.count(unknown);
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

// �жϳ���
bool isConstant(string unknown)
{
	return regex_match(unknown, pattern_number) || regex_match(unknown, pattern_scientific) || regex_match(unknown, pattern_hex);
}

// �ж�����
bool isRedundancy(char unknown)
{
	return unknown == ' ' || unknown == '\r' || unknown == '\n' || unknown == '\t';
}