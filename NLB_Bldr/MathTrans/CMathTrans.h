#ifndef _CMATHTRANS_H
#define _CMATHTRANS_H
// Транcлятор математических и логических выражений. Формулы записываются по правилам C++, т.е.
// && - Логическое И
// || - Логическое ИЛМ 
// ! - Логическое НЕ
// ^ - Логическое исключающее ИЛМ 
// ПОБИТОВЫХ ОПЕРАЦИЙ НЕТ!!!
// Математические операции -- все стандартные

#include <tchar.h>	// Поддержка UNICODE
#include "..\ASD\CStack.h"	// Стек
#include "..\ASD\CL1List.h"	// Список

#define MAX_LOADSTRING 255
#define MAX_VAR_LENGTH 32	// Максимально допустимая величина имени переменной (включая завершающий нуль). 
							// По стандарту ANSI C максимальная длина идентификатора - 31 символ

#define	IERR_ERROR_SUCCESS					0	// Нет ошибки
#define	IERR_SYNTAX_ERROR					1	// Синтаксическая ошибка
#define	IERR_BADSYMB_BEFORE_VAR				2	// Недопустимый символ перед идентификатором переменной
#define	IERR_BADSYMB_BEFORE_OPERATION       3	// Недопустимый символ перед идентификатором операции
#define	IERR_UNKNOWN_OPERATION				4	// Неизвестная операция
#define	IERR_BADSYMB_BEFORE_OPAR			5	// Недопустимый символ перед открывающей скобкой
#define	IERR_BADSYMB_BEFORE_CPAR			6	// Недопустимый символ перед закрывающей скобкой или в конце формулы
#define	IERR_PARENTHESES_NOT_BALANCED		7	// Нарушен баланс скобок
#define	IERR_UNKNOWN_SYMBOL					8	// Неизвестный символ в формуле
#define	IERR_UNKNOWN_OPERATION_SIGN			9	// Неизвестный знак операции либо нарушен баланс скобок
#define	IERR_DIGIT_STARTS_VAR				10	// Цифра в начале имени переменной
#define	IERR_VAR_TOO_LONG					11	// Длина имени переменной превышает максимально допустимую величину MAX_VAR_LENGTH

#define	IERR_NOT_MATCH_ARGS_FOR_AND			12	// Type mismatch for '&&'
#define	IERR_NOT_MATCH_ARGS_FOR_OR			13	// Type mismatch for '||'
#define	IERR_NOT_MATCH_ARGS_FOR_NOT			14	// Type mismatch for '!'
#define	IERR_NOT_MATCH_ARGS_FOR_XOR			15	// Type mismatch for '^'
#define	IERR_NOT_MATCH_ARGS_FOR_PLUS		16	// Type mismatch for '+'
#define	IERR_NOT_MATCH_ARGS_FOR_MINUS		17	// Type mismatch for '-'
#define	IERR_NOT_MATCH_ARGS_FOR_MUL			18	// Type mismatch for '*'
#define	IERR_NOT_MATCH_ARGS_FOR_DIV			19	// Type mismatch for '/'
#define	IERR_NOT_MATCH_ARGS_FOR_UNPLUS		20	// Type mismatch for unary '+'
#define	IERR_NOT_MATCH_ARGS_FOR_UNMINUS		21	// Type mismatch for unary '-'
#define	IERR_NOT_MATCH_ARGS_FOR_EQ			22	// Type mismatch for '=='
#define	IERR_NOT_MATCH_ARGS_FOR_NEQ			23	// Type mismatch for '!='
#define	IERR_NOT_MATCH_ARGS_FOR_G			24	// Type mismatch for '>'
#define	IERR_NOT_MATCH_ARGS_FOR_L			25	// Type mismatch for '<'
#define	IERR_NOT_MATCH_ARGS_FOR_GE			26	// Type mismatch for '>='
#define	IERR_NOT_MATCH_ARGS_FOR_LE			27	// Type mismatch for '<='
#define	IERR_NOT_ENOUGH_ARGS_FOR_AND		28	// Не хватает аргументов для '&&'
#define	IERR_NOT_ENOUGH_ARGS_FOR_OR		    29	// Не хватает аргументов для '||'
#define	IERR_NOT_ENOUGH_ARGS_FOR_NOT		30	// Не хватает аргументов для '!'
#define	IERR_NOT_ENOUGH_ARGS_FOR_XOR		31	// Не хватает аргументов для '^'
#define	IERR_NOT_ENOUGH_ARGS_FOR_PLUS		32	// Not enough arguments for '+'
#define	IERR_NOT_ENOUGH_ARGS_FOR_MINUS		33	// Not enough arguments for '-'
#define	IERR_NOT_ENOUGH_ARGS_FOR_MUL		34	// Not enough arguments for '*'
#define	IERR_NOT_ENOUGH_ARGS_FOR_DIV		35	// Not enough arguments for '/'
#define	IERR_NOT_ENOUGH_ARGS_FOR_UNPLUS		36	// Not enough arguments for unary '+'
#define	IERR_NOT_ENOUGH_ARGS_FOR_UNMINUS	37	// Not enough arguments for unary '-'
#define	IERR_NOT_ENOUGH_ARGS_FOR_EQ			38	// Not enough arguments for '=='
#define	IERR_NOT_ENOUGH_ARGS_FOR_NEQ		39	// Not enough arguments for '!='
#define	IERR_NOT_ENOUGH_ARGS_FOR_G			40	// Not enough arguments for '>'
#define	IERR_NOT_ENOUGH_ARGS_FOR_L			41	// Not enough arguments for '<'
#define	IERR_NOT_ENOUGH_ARGS_FOR_GE			42	// Not enough arguments for '>='
#define	IERR_NOT_ENOUGH_ARGS_FOR_LE			43	// Not enough arguments for '<='
#define	IERR_BADSYMB_BEFORE_NUM				44	// Invalid character before number!
#define	IERR_MULTIPLE_UNARY_OPERATION		45	// Multiple unary operations (example --a) is NOT allowed!
#define	IERR_NOT_ENOUGH_ARGS_FOR_COMMA		46	// Not enough arguments for comma operator
#define	IERR_BADSYMB_BEFORE_COMMA			47	// Недопустимый символ перед оператором "запятая"
#define	IERR_INVALID_FUNCTION_CALL			48	// Invalid function call

#define	IERR_VAR_NOT_FOUND					99	// Переменная не найдена. Этот код ошибки НЕ МОЖЕТ возвращаться функцией Compile, но он зарезервирован для дальнейших действий.

#define CHARTYPE_SYMBOL			100	// Символ (часть имени переменной)
#define CHARTYPE_BINARY_ARITHMETICAL_OPERATION		101	// Арифметическая операция (бинарная)
#define CHARTYPE_BINARY_LOGICAL_OPERATION			102	// Логическая операция (бинарная)
#define CHARTYPE_OPAR			103	// Открывающая круглая скобка
#define CHARTYPE_CPAR			104	// Закрывающая круглая скобка
#define CHARTYPE_SPACE			105	// Пробел или символ табуляции
#define CHARTYPE_UNKNOWN		106	// Неопознанный символ (ошибка!)
#define CHARTYPE_DIGIT			107	// Цифра (часть имени переменной, НЕ МОЖЕТ ИДТИ В САМОМ НАЧАЛЕ ПЕРЕМЕННОЙ!!!)
#define CHARTYPE_INEQUALITY		108	// Символы '<', '>' 
#define CHARTYPE_EQUALITY		109	// Символ '=' 
#define CHARTYPE_UNARY_LOGICAL_OPERATION		110	// Логическая операция (унарная). Арифметические унарные операции будут получаться из бинарных.
#define CHARTYPE_DIGIT_PUNCTUATION				111	// Допустимые символы числа (точка, отделяющая целую часть от дробной и т. д.)
#define	CHARTYPE_COMMA_OPERATOR					112	// Оператор "запятая". Используется в функциях для отделения друг от друга паратетров

enum Lexem	// Тип лексемы
{
	OPar,	// Открывающая скобка
	CPar,	// Закрывающая скобка
	BinarySignum,	// Знак бинарной операции (логическое И, логическое ИЛИ и т. д.)	
	UnarySignum,	// Знак унарной операции (отрицание, унарный минус и т. д.)	
	Character,	// Символ (из имени переменной)
	Number,		// Число
	CommaOperator,	// Операор запятая (для функций)
	FunctionCall // Вызов функции
};

struct S_GomogFunctionInfo	// Информация о функции с одним или несколькими однотипными параметрами
{
	TCHAR* lpFunName;
	TCHAR chTypeOfResult;
	TCHAR chTypeOfParams;
	int iParamsCount;
};

class CMathTrans
{
protected:
	TCHAR* m_lpFormula;					// Строка символов, представляющая формулу
	int m_iFormulaLength;				// Число символов в этой строке	
	CStack<TCHAR>* m_pOperationStack;	// Стек операций
	CStack<TCHAR>* m_pTypesStack;		// Стек типов ('N' - числовой, 'B' - логический, 'A' - произвольный (возможность сделать автоматический type cast в любой тип))
	CStack<S_GomogFunctionInfo*>* m_pGomogFunctionStack;	// Стек функций с одним или несколькими однотипными параметрами
	CL1List<TCHAR*>* m_pStrVarsList;	// Список, элементы которого - строки с именами переменных данной формулы. Его пустота означает, что формула пуста либо еще не откомпилирована, либо при компиляции произошла ошибка.
	enum Lexem m_PrevLex;				// Тип предыдущей лексемы
	int m_iFormulaArgumentsCount;		// Число аргументов (переменных) в формуле. Нужно для контроля ее правильности.
	CL1List<S_GomogFunctionInfo*>* m_pStrGomogFuncsList;	// Список функций с одним или несколькими однотипными параметрами (описание прототипов)

public:
	// <"Большая четверка">
	CMathTrans();					// Инициализация
	virtual ~CMathTrans();			// Разрушение
	CMathTrans(CMathTrans& C);			// Конструктор копирования
	virtual CMathTrans& operator = (CMathTrans& C);	// Перегруженный оператор присваивания
	// </"Большая четверка">

	CMathTrans(const TCHAR* lpFormula);	// Основной конструктор
	bool AddFunctionDefinition(const TCHAR* lpFunName, const TCHAR chTypeOfResult, const TCHAR chTypeOfParams, int iParamsCount);	// Добавить функцию с iParamsCount параметрами одинакового типа chTypeOfParams, возвращающую результат типа chTypeOfResult
	int Compile(const TCHAR* pchTypeOfResult = NULL);	// Процедура компиляции формулы. Возвращает код ошибки компиляции или ноль. pchTypeOfResult - указатель на символ или ноль, в него будет записан тип результата формулы ('N', 'B' 'S', или 'A')
	bool VarsListIsEmpty();
	bool GetFirstVar(const TCHAR* lpBuffer, int iBufSize);
	bool GetNextVar(const TCHAR* lpBuffer, int iBufSize);

protected:
	int CharType(TCHAR c);
	int CharWork(TCHAR c, int& iSymbPos);
	void SkipVar(int& iSymbPos);
	bool SkipNum(int& iSymbPos);
	int MakeOperations(TCHAR c);
	bool CanRun(TCHAR c1, TCHAR c2);
	bool Right(TCHAR c);
	bool ArgumentsMatchedForBooleanBinary(TCHAR Arg1, TCHAR Arg2);
	bool ArgumentsMatchedForBooleanUnary(TCHAR Arg);
	bool ArgumentsMatchedForArithmeticalBinary(TCHAR Arg1, TCHAR Arg2);
	bool ArgumentsMatchedForArithmeticalUnary(TCHAR Arg);
	int Priopity(TCHAR c);
	void KillVarsFromList();
	void KillGomogFuncsFromList();
	void HandleError(int iErrCode);
};

#endif
