#ifndef _CMATHTRANS_H
#define _CMATHTRANS_H
// ����c����� �������������� � ���������� ���������. ������� ������������ �� �������� C++, �.�.
// && - ���������� �
// || - ���������� ��� 
// ! - ���������� ��
// ^ - ���������� ����������� ��� 
// ��������� �������� ���!!!
// �������������� �������� -- ��� �����������

#include <tchar.h>	// ��������� UNICODE
#include "..\ASD\CStack.h"	// ����
#include "..\ASD\CL1List.h"	// ������

#define MAX_LOADSTRING 255
#define MAX_VAR_LENGTH 32	// ����������� ���������� �������� ����� ���������� (������� ����������� ����). 
							// �� ��������� ANSI C ������������ ����� �������������� - 31 ������

#define	IERR_ERROR_SUCCESS					0	// ��� ������
#define	IERR_SYNTAX_ERROR					1	// �������������� ������
#define	IERR_BADSYMB_BEFORE_VAR				2	// ������������ ������ ����� ��������������� ����������
#define	IERR_BADSYMB_BEFORE_OPERATION       3	// ������������ ������ ����� ��������������� ��������
#define	IERR_UNKNOWN_OPERATION				4	// ����������� ��������
#define	IERR_BADSYMB_BEFORE_OPAR			5	// ������������ ������ ����� ����������� �������
#define	IERR_BADSYMB_BEFORE_CPAR			6	// ������������ ������ ����� ����������� ������� ��� � ����� �������
#define	IERR_PARENTHESES_NOT_BALANCED		7	// ������� ������ ������
#define	IERR_UNKNOWN_SYMBOL					8	// ����������� ������ � �������
#define	IERR_UNKNOWN_OPERATION_SIGN			9	// ����������� ���� �������� ���� ������� ������ ������
#define	IERR_DIGIT_STARTS_VAR				10	// ����� � ������ ����� ����������
#define	IERR_VAR_TOO_LONG					11	// ����� ����� ���������� ��������� ����������� ���������� �������� MAX_VAR_LENGTH

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
#define	IERR_NOT_ENOUGH_ARGS_FOR_AND		28	// �� ������� ���������� ��� '&&'
#define	IERR_NOT_ENOUGH_ARGS_FOR_OR		    29	// �� ������� ���������� ��� '||'
#define	IERR_NOT_ENOUGH_ARGS_FOR_NOT		30	// �� ������� ���������� ��� '!'
#define	IERR_NOT_ENOUGH_ARGS_FOR_XOR		31	// �� ������� ���������� ��� '^'
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
#define	IERR_BADSYMB_BEFORE_COMMA			47	// ������������ ������ ����� ���������� "�������"
#define	IERR_INVALID_FUNCTION_CALL			48	// Invalid function call

#define	IERR_VAR_NOT_FOUND					99	// ���������� �� �������. ���� ��� ������ �� ����� ������������ �������� Compile, �� �� �������������� ��� ���������� ��������.

#define CHARTYPE_SYMBOL			100	// ������ (����� ����� ����������)
#define CHARTYPE_BINARY_ARITHMETICAL_OPERATION		101	// �������������� �������� (��������)
#define CHARTYPE_BINARY_LOGICAL_OPERATION			102	// ���������� �������� (��������)
#define CHARTYPE_OPAR			103	// ����������� ������� ������
#define CHARTYPE_CPAR			104	// ����������� ������� ������
#define CHARTYPE_SPACE			105	// ������ ��� ������ ���������
#define CHARTYPE_UNKNOWN		106	// ������������ ������ (������!)
#define CHARTYPE_DIGIT			107	// ����� (����� ����� ����������, �� ����� ���� � ����� ������ ����������!!!)
#define CHARTYPE_INEQUALITY		108	// ������� '<', '>' 
#define CHARTYPE_EQUALITY		109	// ������ '=' 
#define CHARTYPE_UNARY_LOGICAL_OPERATION		110	// ���������� �������� (�������). �������������� ������� �������� ����� ���������� �� ��������.
#define CHARTYPE_DIGIT_PUNCTUATION				111	// ���������� ������� ����� (�����, ���������� ����� ����� �� ������� � �. �.)
#define	CHARTYPE_COMMA_OPERATOR					112	// �������� "�������". ������������ � �������� ��� ��������� ���� �� ����� ����������

enum Lexem	// ��� �������
{
	OPar,	// ����������� ������
	CPar,	// ����������� ������
	BinarySignum,	// ���� �������� �������� (���������� �, ���������� ��� � �. �.)	
	UnarySignum,	// ���� ������� �������� (���������, ������� ����� � �. �.)	
	Character,	// ������ (�� ����� ����������)
	Number,		// �����
	CommaOperator,	// ������� ������� (��� �������)
	FunctionCall // ����� �������
};

struct S_GomogFunctionInfo	// ���������� � ������� � ����� ��� ����������� ����������� �����������
{
	TCHAR* lpFunName;
	TCHAR chTypeOfResult;
	TCHAR chTypeOfParams;
	int iParamsCount;
};

class CMathTrans
{
protected:
	TCHAR* m_lpFormula;					// ������ ��������, �������������� �������
	int m_iFormulaLength;				// ����� �������� � ���� ������	
	CStack<TCHAR>* m_pOperationStack;	// ���� ��������
	CStack<TCHAR>* m_pTypesStack;		// ���� ����� ('N' - ��������, 'B' - ����������, 'A' - ������������ (����������� ������� �������������� type cast � ����� ���))
	CStack<S_GomogFunctionInfo*>* m_pGomogFunctionStack;	// ���� ������� � ����� ��� ����������� ����������� �����������
	CL1List<TCHAR*>* m_pStrVarsList;	// ������, �������� �������� - ������ � ������� ���������� ������ �������. ��� ������� ��������, ��� ������� ����� ���� ��� �� ���������������, ���� ��� ���������� ��������� ������.
	enum Lexem m_PrevLex;				// ��� ���������� �������
	int m_iFormulaArgumentsCount;		// ����� ���������� (����������) � �������. ����� ��� �������� �� ������������.
	CL1List<S_GomogFunctionInfo*>* m_pStrGomogFuncsList;	// ������ ������� � ����� ��� ����������� ����������� ����������� (�������� ����������)

public:
	// <"������� ��������">
	CMathTrans();					// �������������
	virtual ~CMathTrans();			// ����������
	CMathTrans(CMathTrans& C);			// ����������� �����������
	virtual CMathTrans& operator = (CMathTrans& C);	// ������������� �������� ������������
	// </"������� ��������">

	CMathTrans(const TCHAR* lpFormula);	// �������� �����������
	bool AddFunctionDefinition(const TCHAR* lpFunName, const TCHAR chTypeOfResult, const TCHAR chTypeOfParams, int iParamsCount);	// �������� ������� � iParamsCount ����������� ����������� ���� chTypeOfParams, ������������ ��������� ���� chTypeOfResult
	int Compile(const TCHAR* pchTypeOfResult = NULL);	// ��������� ���������� �������. ���������� ��� ������ ���������� ��� ����. pchTypeOfResult - ��������� �� ������ ��� ����, � ���� ����� ������� ��� ���������� ������� ('N', 'B' 'S', ��� 'A')
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
