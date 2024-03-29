## 基于C\+\+的C\+\+简单词法分析器

## 记住！这是词法分析器！！

#### 目标
	1.保留字/基本字:key, 编码id=1; (实现)
	2.标识符:标记常量、数组、类型、变量、过程、函数名, 编码id=2; (实现)
	3.常数:整型、实型、布尔, 编码id=3; (实现)
	4.运算符:+-*%/><, 编码id=4; (实现)
	5.届符/分隔符,;(){}'", 编码id=5; (实现)
	
	6.识别常量，包括有符号和无符号的整数和浮点数，八进制数，十六进制数，以及科学计数(实现)
	7.判断预处理，将以#开头识别成预处理指令(实现)
	8.消除多余的空格、制表符和注释(实现)
	9.括号匹配(实现)


## 目录
	一、基本识别(五大单词符号)
	二、全体函数
	三、预处理指令
	四、识别数字、字母
	五、识别常量
	六、识别保留字
	七、识别终结符
	七、消除冗余
	八、括号匹配
	九、异常
	错误问题


### 一、基本识别(五大单词符号)
	1.保留字/基本字:key, 编码id=1;
	枚举所有保留字/基本字，将其用hash编码，实现快速判断
	注意"true"、"false"是保留字，但主要是作为布尔常量
```c++
// 1.保留字/保留字:key
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
```


	2.标识符:标记常量、数组、类型、变量、函数名, 编码id=2;
	常量无法识别，详见“错误问题”1；
	数组，无法识别复杂数组，详见“错误问题”2；
	变量，函数名，考虑在词法分析器类中生成相应的表来储存（待实现）

```c++
// 2.标识符:标记常量、数组、类型、变量、过程、函数名
unordered_set <string> identifiers;
```


	3.常数:整型、实型、布尔, 编码id=3;
	注意布尔
	详见“识别常量”

	4.运算符:+-*%/><, 编码id=4;
	部分运算符无法分析，详见“错误问题”3；

```c++
const string op[30] = { "!", "+","-","*","/","%","&","|","&&","||","=","==", "!=","<",">","<=",">=","++","--", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "<<", ">>", ">>=", "<<=" };
const unordered_set <string> operators(op, op + 30);
```

	5.届符/分隔符:,;(){}[], 编码id=5;
	<>无法归纳到分隔符，详见“错误问题”4；
```c++
const char sepa[11] = { ',',':', ';', '(', ')', '{', '}', '[', ']' ,'\"','\'' };
const unordered_set <char> separators(sepa, sepa + 11);
```


### 二、全体函数
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
	//匹配括号
	bool matchBrackets(char unknown);

### 预处理指令
	读到"#"，读取整行，不判断预处理指令是否合法，原因详见”错误问题“1

### 冗余的判断
    读到空格、制表符、换行则跳过


### 错误问题
	1.常量无法识别
	已知const和#define均为定义常量的指令
	存在使用C++定义常量函数，例如#define fun(a,b){return a+b;}
	这样的情况暂时无法想出合理的词法分析，故不考虑
	未进行判断字符串、字符输入是否合法
	
	2.复杂数组无法识别
	例如在数组中使用复杂的计算

	3.部分运算符无法分析
	"<<"、">>"考虑到会出现cout << (2 >> 1)，词法分析器遇到这种情况只分析，不判断;
	"*"考虑到指针，该词法分析器不识别指针类型
	"<>"的情况存在一定的复杂程度，存在模板，例如<string>，词法分析器无法识别成括号，将会识别成运算符，这个问题打算留给语法分析器解决
	"?:"条件运算符无法识别

