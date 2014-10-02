#include "stdafx.h"
#include <process.h>	// exit();
#include "CMathTrans.h"

CMathTrans::CMathTrans()
{
	if ((m_lpFormula = new TCHAR[MAX_LOADSTRING]) == NULL) HandleError(1);
	m_lpFormula[0] = '\0';
	m_iFormulaLength = 0;
	if ((m_pOperationStack = new CStack<TCHAR>) == NULL) HandleError(1);
	if ((m_pTypesStack = new CStack<TCHAR>) == NULL) HandleError(1);
	if ((m_pGomogFunctionStack = new CStack<S_GomogFunctionInfo*>) == NULL) HandleError(1);	
	// m_pGomogFunctionStack хранит адреса описаний функций, содержащиеся в m_pStrGomogFuncsList
	if ((m_pStrVarsList = new CL1List<TCHAR*>) == NULL) HandleError(1);
	if ((m_pStrGomogFuncsList = new CL1List<S_GomogFunctionInfo*>) == NULL) HandleError(1);
	m_PrevLex = OPar;	// Один фиг при Compile изменится	
	m_iFormulaArgumentsCount = 0;	
}

CMathTrans::~CMathTrans()
{
	if (m_lpFormula) delete []m_lpFormula;
	if (m_pOperationStack) delete m_pOperationStack;
	if (m_pTypesStack) delete m_pTypesStack;
	if (m_pGomogFunctionStack) delete m_pGomogFunctionStack;
	if (m_pStrVarsList)
	{
		KillVarsFromList();
		delete m_pStrVarsList;
	}
	if (m_pStrGomogFuncsList)
	{
		KillGomogFuncsFromList();
		delete m_pStrGomogFuncsList;
	}
}

CMathTrans::CMathTrans(CMathTrans& C)
{
	int iStrLen;

	if (C.m_lpFormula) 
	{
		iStrLen = _tcslen(C.m_lpFormula);
		if ((m_lpFormula = new TCHAR[iStrLen]) == NULL) HandleError(1);
		_tcscpy(m_lpFormula, C.m_lpFormula);
		m_iFormulaLength = C.m_iFormulaLength;		
	}
	else
	{
		if ((m_lpFormula = new TCHAR[MAX_LOADSTRING]) == NULL) HandleError(1);
		m_lpFormula[0] = '\0';
		m_iFormulaLength = 0;
	}
	
	
	// C.m_pOperationStack никак не может оказаться нулевым, иначе все закончилось бы еще в конструкторе	
	if ((m_pOperationStack = new CStack<TCHAR>(*(C.m_pOperationStack))) == NULL) HandleError(1);
	
	// C.m_pTypesStack никак не может оказаться нулевым, иначе все закончилось бы еще в конструкторе	
	if ((m_pTypesStack = new CStack<TCHAR>(*(C.m_pTypesStack))) == NULL) HandleError(1);	

	if (C.m_pStrVarsList)
		if ((m_pStrVarsList = new CL1List<TCHAR*>(*(C.m_pStrVarsList))) == NULL) HandleError(1);
	else
		if ((m_pStrVarsList = new CL1List<TCHAR*>) == NULL) HandleError(1);

	if (C.m_pStrGomogFuncsList)
		if ((m_pStrGomogFuncsList = new CL1List<S_GomogFunctionInfo*>(*(C.m_pStrGomogFuncsList))) == NULL) HandleError(1);
	else
		if ((m_pStrGomogFuncsList = new CL1List<S_GomogFunctionInfo*>) == NULL) HandleError(1);

	// !!! ТАК НЕЛЬЗЯ! ПРОВЕРИТЬ УКАЗАТЕЛИ!!!
	// C.m_pGomogFunctionStack никак не может оказаться нулевым, иначе все закончилось бы еще в конструкторе	
	if ((m_pGomogFunctionStack = new CStack<S_GomogFunctionInfo*>(*(C.m_pGomogFunctionStack))) == NULL) HandleError(1);	

	m_PrevLex = C.m_PrevLex;		
	m_iFormulaArgumentsCount = C.m_iFormulaArgumentsCount;	

}

CMathTrans& CMathTrans::operator = (CMathTrans& C)
{
	if (this == &C) return *this;

	if (m_lpFormula) delete []m_lpFormula;
	if (m_pOperationStack) delete m_pOperationStack;
	if (m_pTypesStack) delete m_pTypesStack;
	if (m_pStrVarsList) delete m_pStrVarsList;
	if (m_pStrGomogFuncsList) delete m_pStrGomogFuncsList;

	if (C.m_lpFormula) 
	{
		int iStrLen = _tcslen(C.m_lpFormula);
		if ((m_lpFormula = new TCHAR[iStrLen]) == NULL) HandleError(1);
		_tcscpy(m_lpFormula, C.m_lpFormula);
		m_iFormulaLength = C.m_iFormulaLength;		
	}
	else
	{
		if ((m_lpFormula = new TCHAR[MAX_LOADSTRING]) == NULL) HandleError(1);
		m_lpFormula[0] = '\0';
		m_iFormulaLength = 0;
	}
	
	
	// C.m_pOperationStack никак не может оказаться нулевым, иначе все закончилось бы еще в конструкторе	
	if ((m_pOperationStack = new CStack<TCHAR>(*(C.m_pOperationStack))) == NULL) HandleError(1);
	
	// C.m_pTypesStack никак не может оказаться нулевым, иначе все закончилось бы еще в конструкторе	
	if ((m_pTypesStack = new CStack<TCHAR>(*(C.m_pTypesStack))) == NULL) HandleError(1);

	if (C.m_pStrVarsList)
		if ((m_pStrVarsList = new CL1List<TCHAR*>(*(C.m_pStrVarsList))) == NULL) HandleError(1);
	else
		if ((m_pStrVarsList = new CL1List<TCHAR*>) == NULL) HandleError(1);

	if (C.m_pStrGomogFuncsList)
		if ((m_pStrGomogFuncsList = new CL1List<S_GomogFunctionInfo*>(*(C.m_pStrGomogFuncsList))) == NULL) HandleError(1);
	else
		if ((m_pStrGomogFuncsList = new CL1List<S_GomogFunctionInfo*>) == NULL) HandleError(1);

	// !!! ТАК НЕЛЬЗЯ! ПРОВЕРИТЬ УКАЗАТЕЛИ!!!
	// C.m_pGomogFunctionStack никак не может оказаться нулевым, иначе все закончилось бы еще в конструкторе	
	if ((m_pGomogFunctionStack = new CStack<S_GomogFunctionInfo*>(*(C.m_pGomogFunctionStack))) == NULL) HandleError(1);	

	m_PrevLex = C.m_PrevLex;	
	m_iFormulaArgumentsCount = C.m_iFormulaArgumentsCount;	

	return *this;
}

CMathTrans::CMathTrans(const TCHAR* lpFormula)
{
	if (!lpFormula) 
	{
		if ((m_lpFormula = new TCHAR[MAX_LOADSTRING]) == NULL) HandleError(1);
		m_lpFormula[0] = '\0';
		m_iFormulaLength = 0;
	}
	else
	{
		int iFrmLen = _tcslen(lpFormula);
		if ((m_lpFormula = new TCHAR[iFrmLen+1]) == NULL) HandleError(1);	// iFrmLen+1 - чтобы влез завершающий нуль
		_tcscpy(m_lpFormula, lpFormula);
		m_iFormulaLength = iFrmLen;		
	}

	if ((m_pOperationStack = new CStack<TCHAR>) == NULL) HandleError(1);
	if ((m_pTypesStack = new CStack<TCHAR>) == NULL) HandleError(1);	
	if ((m_pGomogFunctionStack = new CStack<S_GomogFunctionInfo*>) == NULL) HandleError(1);	
	if ((m_pStrVarsList = new CL1List<TCHAR*>) == NULL) HandleError(1);
	if ((m_pStrGomogFuncsList = new CL1List<S_GomogFunctionInfo*>) == NULL) HandleError(1);

	m_PrevLex = OPar;	// Один фиг при Compile изменится	
	m_iFormulaArgumentsCount = 0;	
}

void CMathTrans::KillVarsFromList()
{
	// Надо удалить все строчки, которые уже есть, так как в результате 
	// компиляции все может измениться.Здесь НЕЛЬЗЯ использовать метод 
	// списка MakeEmpty, так как он удалит информационные кирпичи, но не 
	// освободит память из-под самих строчек
	TCHAR* pstrToDelNow;
	m_pStrVarsList->GoBegin();
	while (!m_pStrVarsList->IsEmpty())
	{
		m_pStrVarsList->Extract(pstrToDelNow);			
		if (pstrToDelNow) delete []pstrToDelNow;
	}
}

void CMathTrans::KillGomogFuncsFromList()
{
	// Аналог KillVarsFromList для списка функций с однотипными аргументами
	S_GomogFunctionInfo* pgfi_del;
	m_pStrGomogFuncsList->GoBegin();
	while (!m_pStrGomogFuncsList->IsEmpty())
	{
		m_pStrGomogFuncsList->Extract(pgfi_del);			
		if (pgfi_del) 
		{
			if (pgfi_del->lpFunName) delete []pgfi_del->lpFunName;
			delete pgfi_del;
		}
	}
}

bool CMathTrans::VarsListIsEmpty()
{
	return m_pStrVarsList->IsEmpty();
}

bool CMathTrans::GetFirstVar(const TCHAR* lpBuffer, int iBufSize)
{
	TCHAR* lpFirstVar;
	int iStrLen;

	if ((!lpBuffer)||(iBufSize<=0)) return false;
	m_pStrVarsList->GoBegin();
	if (!(m_pStrVarsList->SeeElem(lpFirstVar))) return false;
	iStrLen = _tcslen(lpFirstVar);
	if (iStrLen > iBufSize) return false;
	_tcscpy((TCHAR*) lpBuffer, lpFirstVar);	// No return value is reserved to indicate an error. Т.е. нельзя проверять на NULL как признак ошибки
	return true;
}

bool CMathTrans::GetNextVar(const TCHAR* lpBuffer, int iBufSize)
{
	TCHAR* lpFirstVar;
	int iStrLen;

	if ((!lpBuffer)||(iBufSize<=0)) return false;
	m_pStrVarsList->Next();
	if (!(m_pStrVarsList->SeeElem(lpFirstVar))) return false;
	iStrLen = _tcslen(lpFirstVar);
	if (iStrLen > iBufSize) return false;
	_tcscpy((TCHAR*) lpBuffer, lpFirstVar);	// No return value is reserved to indicate an error. Т.е. нельзя проверять на NULL как признак ошибки
	return true;
}

int CMathTrans::Compile(const TCHAR* pchTypeOfResult /*= NULL*/)
// Процедура компиляции формулы. Возвращает код ошибки компиляции или ноль.
// pchTypeOfResult - указатель на символ или ноль, в него будет записан тип 
// результата формулы ('N', 'B' 'S' или 'A'). Если он ноль, то код результата бесславно пропадает.
{
	TCHAR c;
	int iResult;
	int i;	// Индекс символа в формуле

	if (m_pStrVarsList)	KillVarsFromList();	
	m_pOperationStack->MakeEmpty();
	m_pTypesStack->MakeEmpty();
	
	m_pOperationStack->Push(_T('('));
	m_PrevLex = OPar;	
	i = 0;
	while (i<m_iFormulaLength)	
	{
		if ((iResult = CharWork(m_lpFormula[i], i)) != IERR_ERROR_SUCCESS) 
		{
			KillVarsFromList();	// Компиляция неуспешна => освободим список переменных
			return iResult;
		}
	}

	// Сейчас будет фиктивная закрывающая скобка... Надо проверить, 
	// что перед ней допустимый символ. OPar может быть, 
	// если формула пуста, ведь данная скобка - фиктивная!
	if (!((m_PrevLex == OPar) || 
		(m_PrevLex == CPar) || 
		(m_PrevLex == Character) ||
		(m_PrevLex == Number))) 
	{
		KillVarsFromList();	// Компиляция неуспешна => освободим список переменных
		return IERR_BADSYMB_BEFORE_CPAR; 
	}

	// Все функции к этому моменту должны быть исполнены
	if (!m_pGomogFunctionStack->IsEmpty())
	{
		KillVarsFromList();	// Компиляция неуспешна => освободим список переменных
		return IERR_INVALID_FUNCTION_CALL;
	}

	c = _T(')');	

	if ((iResult = MakeOperations(c)) != IERR_ERROR_SUCCESS) 
	{
		KillVarsFromList();	// Компиляция неуспешна => освободим список переменных
		return iResult;
	}

	m_pOperationStack->Pop(c);

	if (c != _T('(')) 
	{
		KillVarsFromList();	// Компиляция неуспешна => освободим список переменных
		return IERR_SYNTAX_ERROR;
	}

	// m_PrevLex = CPar; - в общем-то, это правда, но уже не нужно...
	if (!m_pOperationStack->IsEmpty()) 
	{		
		KillVarsFromList();	// Компиляция неуспешна => освободим список переменных
		return IERR_SYNTAX_ERROR;
	}

	if (pchTypeOfResult)
		m_pTypesStack->Pop((TCHAR&) *pchTypeOfResult);	
	else
		m_pTypesStack->DelTop();

	if (!m_pTypesStack->IsEmpty()) 
	{
		KillVarsFromList();	// Компиляция неуспешна => освободим список переменных
		return IERR_SYNTAX_ERROR;
	}	

	// if (m_iFormulaArgumentsCount != 1)... - эту проверку решено убрать как ненужную	

	return IERR_ERROR_SUCCESS;	// Формула откомпилирована успешно!
}

int CMathTrans::CharType(TCHAR c)
{	
	if (_istspace((_TXCHAR) c))
		return CHARTYPE_SPACE;	// Пробел

	if ((c >= _T('a')) && (c <= _T('z')) ||
	   (c >= _T('A')) && (c <= _T('Z')) ||
	   (c == _T('_')))
		return CHARTYPE_SYMBOL;

	if ((c >= _T('0')) && (c <= _T('9')))
		return CHARTYPE_DIGIT;

	if ((c == _T('&')) ||
	   (c == _T('|')) ||
	   (c == _T('^')))
		return CHARTYPE_BINARY_LOGICAL_OPERATION;

	if ((c == _T('*')) ||
	   (c == _T('/')) ||
	   (c == _T('+')) ||
	   (c == _T('-')))
		return CHARTYPE_BINARY_ARITHMETICAL_OPERATION;

	if (c == _T('!'))
		return CHARTYPE_UNARY_LOGICAL_OPERATION;

	if ((c == _T('<')) ||
	   (c == _T('>')))
		return CHARTYPE_INEQUALITY;

	if (c == _T('='))
		return CHARTYPE_EQUALITY;	

	if (c == _T('('))
		return CHARTYPE_OPAR;

	if (c == _T(')'))
		return CHARTYPE_CPAR;

	if (c == _T('.'))
		return CHARTYPE_DIGIT_PUNCTUATION;

	if (c == _T(','))
		return CHARTYPE_COMMA_OPERATOR;
	
	return CHARTYPE_UNKNOWN;
}

void CMathTrans::SkipVar(int& iSymbPos)
{
	int iCharType = CharType(m_lpFormula[iSymbPos]);
	while ((iCharType == CHARTYPE_SYMBOL) || (iCharType == CHARTYPE_DIGIT)) 
	{
		iSymbPos++; 
		iCharType = CharType(m_lpFormula[iSymbPos]);
	}
}

bool CMathTrans::SkipNum(int& iSymbPos)
{
	bool fFracBegins = false;
	int iCharType = CharType(m_lpFormula[iSymbPos]);
	
	while ((iCharType == CHARTYPE_DIGIT_PUNCTUATION) || 
		   (iCharType == CHARTYPE_DIGIT)) 
	{
		if (iCharType == CHARTYPE_DIGIT_PUNCTUATION) 
		{
			if (fFracBegins) return false;
			fFracBegins = true;
		}		
		
		iSymbPos++; 
		iCharType = CharType(m_lpFormula[iSymbPos]);
	}

	return true;
}

bool CMathTrans::AddFunctionDefinition(const TCHAR* lpFunName, const TCHAR chTypeOfResult, const TCHAR chTypeOfParams, int iParamsCount)
{
	S_GomogFunctionInfo* psgfi;
	int iFNameLen;

	if (lpFunName)
	{
		psgfi = new S_GomogFunctionInfo;
		if (!psgfi) HandleError(1);
		iFNameLen = _tcslen(lpFunName);
		psgfi->lpFunName = new TCHAR[iFNameLen + 1]; 	
		if (!(psgfi->lpFunName)) HandleError(1);		
		_tcscpy(psgfi->lpFunName, lpFunName);
		psgfi->chTypeOfParams = chTypeOfParams;
		psgfi->chTypeOfResult = chTypeOfResult;
		psgfi->iParamsCount = iParamsCount;
		m_pStrGomogFuncsList->Insert(psgfi);
		return true;
	}
	else
		return false;
}

int CMathTrans::CharWork(TCHAR c, int& iSymbPos)
// Обработать символ формулы
{
	int iRes = CharType(c);
	int iCharWorkResult = IERR_ERROR_SUCCESS;	// Будем оптимистами

	switch (iRes)
	{
	case CHARTYPE_SPACE:
		{
			iSymbPos++;	// Пробелы просто игнорируем...
		}
		break;

	case CHARTYPE_DIGIT_PUNCTUATION: return IERR_SYNTAX_ERROR;	// Такое запрещено (.1415 вместо 0.1415)

	case CHARTYPE_DIGIT: 
		{
			if (!((m_PrevLex == OPar) || 
				(m_PrevLex == BinarySignum) ||
				(m_PrevLex == UnarySignum) ||
				(m_PrevLex == CommaOperator))) return IERR_BADSYMB_BEFORE_NUM;

			if (SkipNum(iSymbPos))
			{
				m_iFormulaArgumentsCount++;	// Одним числом в формуле стало больше...
				m_pTypesStack->Push(_T('N'));	
				m_PrevLex = Number;
			}
			else
				return IERR_DIGIT_STARTS_VAR;	// Это не число, а ошибка!
		}
		break;

	case CHARTYPE_COMMA_OPERATOR:
		{
			// Оператор "запятая" используется в функциях для отделения параметров
			if (!((m_PrevLex == CPar) || 
				(m_PrevLex == Character) ||
				(m_PrevLex == Number))) return IERR_BADSYMB_BEFORE_COMMA; 
			iSymbPos++;

			iCharWorkResult = MakeOperations(c); // Обработать отложенные операции
			m_pOperationStack->Push(c);			
			m_PrevLex = CommaOperator;
		}
		break;

	case CHARTYPE_SYMBOL:
		{			
			int iBeginVarPos;
			int iEndVarPos;
			int iVarLen;
			TCHAR* lpVar;
			S_GomogFunctionInfo* pgfi_cur;	
			BOOL fIsFunction;

			if (!((m_PrevLex == OPar) || 
				(m_PrevLex == BinarySignum) ||
				(m_PrevLex == UnarySignum) ||
				(m_PrevLex == CommaOperator))) return IERR_BADSYMB_BEFORE_VAR;
			iBeginVarPos = iSymbPos;
			SkipVar(iSymbPos);
			iEndVarPos = iSymbPos;
			iVarLen = iEndVarPos - iBeginVarPos + 1;	// +1 в конце - для завершающего нуля
			if (iVarLen > MAX_VAR_LENGTH) return IERR_VAR_TOO_LONG;
			if ((lpVar = new TCHAR[iVarLen]) == NULL) HandleError(1);
			for (int i = 0; i < iVarLen-1; i++)
			{
				lpVar[i] = m_lpFormula[i+iBeginVarPos];
			}
			lpVar[iVarLen-1] = _T('\0');	// Завершающий нуль

			fIsFunction = FALSE;
			m_pStrGomogFuncsList->GoBegin();
			while (!m_pStrGomogFuncsList->IsEnd())
			{
				m_pStrGomogFuncsList->SeeElem(pgfi_cur);			
				m_pStrGomogFuncsList->Next();
				if (pgfi_cur) 
				{
					if (_tcscmp(pgfi_cur->lpFunName, lpVar) == 0)
					{
						fIsFunction = TRUE;
						delete lpVar;
						lpVar = NULL;
						break;
					}					
				}
			}

			if (fIsFunction)
			{
				iCharWorkResult = MakeOperations(_T('f')); // Обработать отложенные операции
				m_pOperationStack->Push(_T('f'));	
				m_pGomogFunctionStack->Push(pgfi_cur);	
				m_PrevLex = FunctionCall;
			}
			else
			{
				m_pStrVarsList->Insert(lpVar);
				m_iFormulaArgumentsCount++;	// Одной переменной в формуле стало больше...
				m_pTypesStack->Push(_T('A'));	// Переменная может быть преобразована к любому типу
				m_PrevLex = Character;			
			}			
		}
		break;

	case CHARTYPE_INEQUALITY:	
		{	
			int iNextCharType;

			if (!((m_PrevLex == CPar) || 
				(m_PrevLex == Character) || 
				(m_PrevLex == Number))) 
				return IERR_BADSYMB_BEFORE_OPERATION;
		
			iSymbPos++;			
			iNextCharType = CharType(m_lpFormula[iSymbPos]);
			
			switch (iNextCharType)
			{
			case CHARTYPE_EQUALITY:
				{
					if (c == _T('<')) c = _T('L');	// 'L' означает '<='
					else if (c == _T('>')) c = _T('G');	// 'G' означает '>='
					iSymbPos++;	// Пропустить '='
				}
				break;
			case CHARTYPE_DIGIT:
			case CHARTYPE_SYMBOL:
				break;	// Ничего не делаем, просто констатируем факт, что этот символ является допустимым
			case CHARTYPE_BINARY_ARITHMETICAL_OPERATION:
				{
					if (!((m_lpFormula[iSymbPos] == _T('+')) ||
						(m_lpFormula[iSymbPos] == _T('-')))) 
						return IERR_UNKNOWN_OPERATION;	
				}
				break;
			case CHARTYPE_OPAR:
			case CHARTYPE_SPACE:				
				break;	// Ничего не делаем, просто констатируем факт, что этот символ является допустимым

			default:
				return IERR_UNKNOWN_OPERATION;			
			}

			iCharWorkResult = MakeOperations(c); // Обработать отложенные операции
			m_pOperationStack->Push(c);			
			m_PrevLex = BinarySignum;
		}
		break;
	
	case CHARTYPE_EQUALITY:
		{	
			int iNextCharType;

			if (!((m_PrevLex == CPar) || 
				(m_PrevLex == Character) || 
				(m_PrevLex == Number))) 
				return IERR_BADSYMB_BEFORE_OPERATION;
		
			iSymbPos++;			
			iNextCharType = CharType(m_lpFormula[iSymbPos]);
			
			if (iNextCharType == CHARTYPE_EQUALITY)	// Допускается только символ '==', '=' не допускается!
			{				
				iSymbPos++;		// Пропустить '='
			}
			else
				return IERR_UNKNOWN_OPERATION;			

			iCharWorkResult = MakeOperations(c); // Обработать отложенные операции
			m_pOperationStack->Push(c);			
			m_PrevLex = BinarySignum;
		}
		break;

	case CHARTYPE_BINARY_LOGICAL_OPERATION:
		{			
			if (!((m_PrevLex == CPar) || 
				(m_PrevLex == Character) || 
				(m_PrevLex == Number))) 
				return IERR_BADSYMB_BEFORE_OPERATION;
		
			iSymbPos++;
			if (c == _T('&'))
			{
			  if (m_lpFormula[iSymbPos] == _T('&')) 
				  iSymbPos++;
			  else
				  return IERR_UNKNOWN_OPERATION;
			} 
			else if (c == _T('|'))
			{
			  if (m_lpFormula[iSymbPos] == _T('|')) 
				  iSymbPos++;
			  else
				  return IERR_UNKNOWN_OPERATION;
			}

			iCharWorkResult = MakeOperations(c); // Обработать отложенные операции
			m_pOperationStack->Push(c);			
			m_PrevLex = BinarySignum;
		}
		break;

	case CHARTYPE_BINARY_ARITHMETICAL_OPERATION:
		{		
			BOOL fIsUnaryOp;

			fIsUnaryOp = ((m_PrevLex == OPar) || 
						  (m_PrevLex == BinarySignum) ||
						  (m_PrevLex == UnarySignum));	// В первом приближении...
				
			if (fIsUnaryOp)
			{
				if (c == _T('+')) c = _T('p');	// Унарный плюс
				else if (c == _T('-')) c = _T('m');	// Унарный минус
				else fIsUnaryOp = FALSE;	// Этот символ не может быть унарной операцией!
			}

			// Множественные унарные операции, к примеру, ---а, не допускаются!
			if (fIsUnaryOp && (m_PrevLex == UnarySignum)) return IERR_MULTIPLE_UNARY_OPERATION;

			if (!((m_PrevLex == CPar) || 
				(m_PrevLex == Character) || 
				(m_PrevLex == Number) ||
				fIsUnaryOp)) return IERR_BADSYMB_BEFORE_OPERATION;			
		
			iSymbPos++;			

			iCharWorkResult = MakeOperations(c); // Обработать отложенные операции
			m_pOperationStack->Push(c);			
			(fIsUnaryOp) ? m_PrevLex = UnarySignum : m_PrevLex = BinarySignum;
		}
		break;

	case CHARTYPE_UNARY_LOGICAL_OPERATION:
		{		
			bool fBinaryOperation;

			iSymbPos++;
			fBinaryOperation = ((m_PrevLex == CPar) || 
					(m_PrevLex == Character) || 
					(m_PrevLex == Number) ||
					(m_PrevLex == CommaOperator));	// В первом приближении

			if (fBinaryOperation)
			{				
				if (c == _T('!'))
				{
				  if (m_lpFormula[iSymbPos] == _T('=')) 
				  {
					  c = _T('#');	// Это логическая операция 'Не равно'
					  iSymbPos++;
				  }
				  else
					  fBinaryOperation = FALSE;	// Это не может быть бинарной операцией
				} 
			}

			// Множественные унарные операции, к примеру, !!!а, не допускаются!
			if (!fBinaryOperation && (m_PrevLex == UnarySignum)) return IERR_MULTIPLE_UNARY_OPERATION;

			if (!((m_PrevLex == OPar) || 
				 (m_PrevLex == UnarySignum) ||
				 (m_PrevLex == BinarySignum) ||
				 fBinaryOperation)) 
				return IERR_BADSYMB_BEFORE_OPERATION;
         						
			iCharWorkResult = MakeOperations(c); // Обработать отложенные операции
			m_pOperationStack->Push(c);			
			(fBinaryOperation) ? m_PrevLex = BinarySignum : m_PrevLex = UnarySignum;
		}
		break;	

	case CHARTYPE_OPAR:
		{			
			TCHAR chLastOp;			

			if (!((m_PrevLex == OPar) || 
				(m_PrevLex == BinarySignum) ||
				(m_PrevLex == UnarySignum) ||
				(m_PrevLex == CommaOperator) ||
				(m_PrevLex == FunctionCall))) return IERR_BADSYMB_BEFORE_OPAR; 	
			
			if (m_PrevLex == FunctionCall)
			{
				// Если это действительно функция, засунем ее перед скобкой
				m_pOperationStack->Pop(chLastOp);	// 'f'

				m_pOperationStack->Push(c);
				m_pOperationStack->Push(chLastOp);
			}
			else
				m_pOperationStack->Push(c);				
			
			iSymbPos++;         
			m_PrevLex = OPar;
		}
		break;

	case CHARTYPE_CPAR:
		{
			TCHAR Ch0;
			TCHAR chTop;			

			m_pOperationStack->SeeTop(chTop);

			if (!((m_PrevLex == CPar) || 
				(m_PrevLex == Character) ||
				(m_PrevLex == Number) ||
				((m_PrevLex == OPar) && (chTop == _T('f')))))	// Вызов функции без параметров, т.е. f()
				return IERR_BADSYMB_BEFORE_CPAR; 
			iSymbPos++;                  
			iCharWorkResult = MakeOperations(c);
         	m_pOperationStack->Pop(Ch0);
			if (Ch0 != _T('(')) return IERR_PARENTHESES_NOT_BALANCED;			
			m_PrevLex = CPar;
		}
		break;

	case CHARTYPE_UNKNOWN:
		{
			return IERR_UNKNOWN_SYMBOL;
		}
		break;
	}

	return iCharWorkResult;	
}

bool CMathTrans::ArgumentsMatchedForBooleanBinary(TCHAR Arg1, TCHAR Arg2)
{
	return (((Arg1 == _T('B')) || (Arg1 == _T('A'))) &&
			((Arg2 == _T('B')) || (Arg2 == _T('A'))));
}

bool CMathTrans::ArgumentsMatchedForBooleanUnary(TCHAR Arg)
{
	return ((Arg == _T('B')) || (Arg == _T('A')));
}

bool CMathTrans::ArgumentsMatchedForArithmeticalBinary(TCHAR Arg1, TCHAR Arg2)
{
	return (((Arg1 == _T('N')) || (Arg1 == _T('A'))) &&
			((Arg2 == _T('N')) || (Arg2 == _T('A'))));
}

bool CMathTrans::ArgumentsMatchedForArithmeticalUnary(TCHAR Arg)
{
	return ((Arg == _T('N')) || (Arg == _T('A')));
}

int CMathTrans::MakeOperations(TCHAR c)
/************************************************************************
 * Дано: (CharType(c)='Знак' or                                         *
 *        CharType(c)='Правая скобка') and                              *
 *        not(m_pOperationStack->IsEmpty()) ¦ там, как минимум, "("     *
 * Получить: Операции из стека операций, которые можно выполнить        *
 * до "c", выполнены...                                                 *
 ************************************************************************/
{
	TCHAR OpStEl;
	TCHAR Arg1;
	TCHAR Arg2;
	S_GomogFunctionInfo* pgfi_cur = NULL;
	int i;	// Для цикла for
	
	m_pOperationStack->Pop(OpStEl);
	while (CanRun(OpStEl,c))
	{
		switch (OpStEl)
		{
		case _T('+'):	// Плюс бинарный
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_PLUS;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_PLUS;		
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_PLUS;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_PLUS;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('N'));	// Тип этой операции - числовой
			}
			break;
		case _T('-'):	// Минус бинарный
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_MINUS;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_MINUS;		
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_MINUS;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_MINUS;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('N'));	// Тип этой операции - числовой
			}
			break;
		case _T('*'):	// Умножение
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_MUL;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_MUL;		
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_MUL;		
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_MUL;		
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('N'));	// Тип этой операции - числовой
			}
			break;
		case _T('/'):	// Деление
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_DIV;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_DIV;		
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_DIV;			
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_DIV;	
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('N'));	// Тип этой операции - числовой
			}
			break;
		case _T('p'):	// Плюс унарный
			{				
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_UNPLUS;	
				if (!ArgumentsMatchedForArithmeticalUnary(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_UNPLUS;	
				m_iFormulaArgumentsCount--;	// Операция унарная => забираем один аргумент
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_UNPLUS;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('N'));	// Тип этой операции - числовой
			}
			break;
		case _T('m'):	// Минус унарный
			{				
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_UNMINUS;	
				if (!ArgumentsMatchedForArithmeticalUnary(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_UNMINUS;	
				m_iFormulaArgumentsCount--;	// Операция унарная => забираем один аргумент
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_UNMINUS;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('N'));	// Тип этой операции - числовой
			}
			break;
		case _T('='):	// ==
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_EQ;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_EQ;	
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_EQ;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_EQ;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('#'):	// !=
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_NEQ;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_NEQ;	
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_NEQ;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_NEQ;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('<'):	// Меньше
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_L;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_L;	
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_L;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_L;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('>'):	// Больше
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_G;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_G;	
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_G;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_G;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('L'):	// <=
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_LE;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_LE;	
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_LE;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_LE;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('G'):	// >=
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_GE;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_GE;	
				if (!ArgumentsMatchedForArithmeticalBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_GE;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_GE;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('&'):	// && - Логическое И
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_AND;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_AND;		
				if (!ArgumentsMatchedForBooleanBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_AND;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_AND;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('|'):	// || - Логическое ИЛМ 
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_OR;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_OR;	
				if (!ArgumentsMatchedForBooleanBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_OR;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_OR;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('!'):	// ! - Логическое НЕ
			{				
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_NOT;	
				if (!ArgumentsMatchedForBooleanUnary(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_NOT;	
				m_iFormulaArgumentsCount--;	// Операция унарная => забираем один аргумент
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_NOT;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T('^'):	// ^ - Логическое исключающее ИЛМ 
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_MATCH_ARGS_FOR_XOR;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_MATCH_ARGS_FOR_XOR;	
				if (!ArgumentsMatchedForBooleanBinary(Arg1, Arg2)) return IERR_NOT_MATCH_ARGS_FOR_XOR;	
				m_iFormulaArgumentsCount -= 2;	// Операция бинарная => забираем два аргумента
				if (m_iFormulaArgumentsCount<0) return IERR_NOT_ENOUGH_ARGS_FOR_XOR;
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(_T('B'));	// Тип этой операции - логический
			}
			break;
		case _T(','):	// , - оператор запятая, используется для отделения параметров функций
			{
				if (!m_pTypesStack->Pop(Arg2)) return IERR_NOT_ENOUGH_ARGS_FOR_COMMA;	
				if (!m_pTypesStack->Pop(Arg1)) return IERR_NOT_ENOUGH_ARGS_FOR_COMMA;	
				m_pTypesStack->Push(Arg1);
				m_pTypesStack->Push(Arg2);
				// Просто проверяем, есть ли соответствующие два аргумента любого типа. Запятую считаем бинарной операцией.
				// Оба аргумента операции потом снова ложим в стек.
			}
			break;
		case _T('f'):	// Вызов функции
			{
				int iFunArgCount;
				TCHAR chTypeOfParams;

				if (!m_pGomogFunctionStack->Pop(pgfi_cur)) return IERR_INVALID_FUNCTION_CALL;

				iFunArgCount = pgfi_cur->iParamsCount;
				chTypeOfParams = pgfi_cur->chTypeOfParams;
				for (i = 0; i < iFunArgCount; i++)
				{
					if (!m_pTypesStack->Pop(Arg1)) return IERR_INVALID_FUNCTION_CALL;
					if ((Arg1 != chTypeOfParams) && (Arg1 != _T('A'))) 
						return IERR_INVALID_FUNCTION_CALL;
				}
				
				m_iFormulaArgumentsCount++;	// В результате совершения данной операции появился один новый аргумент (результат).
				m_pTypesStack->Push(pgfi_cur->chTypeOfResult);	// Тип этой операции - какая-то :-)
			}
			break;
		default:
			return IERR_UNKNOWN_OPERATION_SIGN;
		}
		m_pOperationStack->Pop(OpStEl);
	}
	m_pOperationStack->Push(OpStEl);

	return IERR_ERROR_SUCCESS;	// Усе хорошо!
}

bool CMathTrans::CanRun(TCHAR c1, TCHAR c2)
// Дано: (вид символа <c1> = знак) или (вид символа <c1> = левая скобка) и
//		 (вид символа <c2> = знак) или (вид символа <c1> = правая скобка)
{
	if (c1==_T('(')) return false;
	if (c2==_T(')')) return true;

	if (!(Right(c1))) 
		return (Priopity(c1)>=Priopity(c2));	// Левая ассоциативность
	else 
		return (Priopity(c1)>Priopity(c2));		// Правая ассоциативность
}

bool CMathTrans::Right(TCHAR c)
{
	switch (c)
	{
	case _T('|'): 
	case _T('&'): 
	case _T('^'): 
	case _T('='): // Равно
	case _T('#'): // Не равно
	case _T('G'): // >=
	case _T('L'): // <=
	case _T('<'): 
	case _T('>'): 
	case _T('+'): 
	case _T('-'): 
	case _T('*'): 
	case _T('/'): 	
		return false;
	case _T('!'): 
	case _T('p'): // Унарный плюс
	case _T('m'): // Унарный минус
	case _T(','): 
	case _T('f'): // Определение функции
		return true;
	default:
		return false;
	}
}

int CMathTrans::Priopity(TCHAR c)
// Дано: вид символа <c> = знак
// Найти: вес символа
{
	switch (c)
	{
	case _T('^'): 
		return 1;
	case _T('|'): 
		return 2;
	case _T('&'): 
		return 3;
	case _T('='): 
	case _T('#'): 
	case _T('<'): 
	case _T('>'):
		return 4;
	case _T('+'): 
	case _T('-'): 	
		return 5;
	case _T('*'): 
	case _T('/'): 	
		return 6;
	case _T('p'): 
	case _T('m'): 
	case _T('!'): 
		return 7;
	case _T(','): 
		return 8;
	case _T('f'): 
		return 9999;	// Функции имеют самый высокий приоритет

	default:
		return 0;
	}
}

void CMathTrans::HandleError(int iErrCode)
{
	cout << "Terrible error in CMathTrans: ";
	switch (iErrCode)
	{
		case 1:
			cout << "Out of memory!" << endl;
			exit(1);
			break;

		default: 
			cout << "Unknown." << endl;
			exit(1);
			break;
	}
}
