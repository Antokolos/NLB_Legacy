#include "StdAfx.h"
#include "Resource.h"
#include "funcs.h"
#include "NLB_BldrDoc.h"
#include "NLB_Bldr.h"
#include "NLB_BldrView.h"
#include "MainFrm.h"
#include "MathTrans\CMathTrans.h"

int CompileBooleanFormula(CNLB_BldrApp* pNLB_Bldr,
						  CNLB_BldrDoc* pDoc,
						  const CString& strFormula,
						  CString* pstrCompilerErrorMessage,
						  CString* pstrCompilerErrorCaption)
{
	int iCompileResult;

	CMathTrans trans(strFormula);
	switch (iCompileResult = trans.Compile())
	{
	case IERR_ERROR_SUCCESS:
		// Нет ошибки компиляции, все OK!
		// Теперь осталось только проверить, что все переменные из этой формулы есть в нашей книге.			
		{			
			int iVarPageIndex;

			try
			{
				CString strBuf((WCHAR) ' ', MAX_VAR_LENGTH);	// MAX_VAR_LENGTH пробелов. MAX_VAR_LENGTH - константа из CLogicTrans.h. Идентификатор переменной не может иметь большую длину.			
				
				if (trans.GetFirstVar(strBuf, MAX_VAR_LENGTH))
				{				
					iVarPageIndex = pDoc->FindPageIndexByVar(strBuf);
					if (iVarPageIndex == -1)
					{
						// Итак, это не страничная переменная. Но не надо отчаиваться, быть может, она содержится в списке обычных переменных
						if (!(pDoc->FindBookVarPositionByName(strBuf)))
						{
							// Итак, это не обычная переменная. Но не надо отчаиваться, быть может, она содержится в списке вероятностных переменных
							if (!(pDoc->FindProbVarPositionByName(strBuf)))
							{
								iCompileResult = IERR_VAR_NOT_FOUND;
								if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
								if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->Format(STRERR_VAR_NOT_FOUND, strBuf);
							}
						}						
					}

					while (trans.GetNextVar(strBuf, MAX_VAR_LENGTH))
					{
						iVarPageIndex = pDoc->FindPageIndexByVar(strBuf);
						if (iVarPageIndex == -1)
						{
							// Итак, это не страничная переменная. Но не надо отчаиваться, быть может, она содержится в списке обычных переменных
							if (!(pDoc->FindBookVarPositionByName(strBuf)))
							{
								// Итак, это не обычная переменная. Но не надо отчаиваться, быть может, она содержится в списке вероятностных переменных
								if (!(pDoc->FindProbVarPositionByName(strBuf)))
								{
									iCompileResult = IERR_VAR_NOT_FOUND;
									if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
									if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->Format(STRERR_VAR_NOT_FOUND, strBuf);
								}
							}
						}
					}
				}
			}
			catch (CMemoryException e)
			{
				pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
			}

		}
		break;
	case IERR_SYNTAX_ERROR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_SYNTAX_ERROR);
		break;
	case IERR_BADSYMB_BEFORE_VAR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_BADSYMB_BEFORE_VAR);
		break;
	case IERR_BADSYMB_BEFORE_OPERATION:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_BADSYMB_BEFORE_OPERATION);
		break;
	case IERR_UNKNOWN_OPERATION:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_UNKNOWN_OPERATION);
		break;
	case IERR_BADSYMB_BEFORE_OPAR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_BADSYMB_BEFORE_OPAR);		
		break;
	case IERR_BADSYMB_BEFORE_CPAR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_BADSYMB_BEFORE_CPAR);	
		break;
	case IERR_PARENTHESES_NOT_BALANCED:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_PARENTHESES_NOT_BALANCED);	
		break;
	case IERR_UNKNOWN_SYMBOL:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_UNKNOWN_SYMBOL);	
		break;
	case IERR_UNKNOWN_OPERATION_SIGN:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_UNKNOWN_OPERATION_SIGN);	
		break;
	case IERR_NOT_MATCH_ARGS_FOR_AND:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_AND);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_OR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_OR);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_NOT:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_NOT);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_XOR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_XOR);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_PLUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_PLUS);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_MINUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_MINUS);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_MUL:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_MUL);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_DIV:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_DIV);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_UNPLUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_UNPLUS);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_UNMINUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_UNMINUS);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_EQ:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_EQ);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_NEQ:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_NEQ);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_G:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_G);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_L:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_L);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_GE:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_GE);
		break;
	case IERR_NOT_MATCH_ARGS_FOR_LE:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_MATCH_ARGS_FOR_LE);
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_AND:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_AND);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_OR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_OR);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_NOT:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_NOT);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_XOR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_XOR);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_PLUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_PLUS);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_MINUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_MINUS);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_MUL:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_MUL);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_DIV:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_DIV);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_UNPLUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_UNPLUS);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_UNMINUS:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_UNMINUS);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_EQ:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_EQ);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_NEQ:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_NEQ);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_G:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_G);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_L:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_L);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_GE:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_GE);	
		break;
	case IERR_NOT_ENOUGH_ARGS_FOR_LE:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_NOT_ENOUGH_ARGS_FOR_LE);	
		break;
	case IERR_BADSYMB_BEFORE_NUM:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_BADSYMB_BEFORE_NUM);	
		break;
	case IERR_MULTIPLE_UNARY_OPERATION:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_MULTIPLE_UNARY_OPERATION);	
		break;
	case IERR_DIGIT_STARTS_VAR:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_DIGIT_STARTS_VAR);	
		break;
	case IERR_VAR_TOO_LONG:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->Format(STRERR_VAR_TOO_LONG, MAX_VAR_LENGTH - 1);	
		break;
	default:
		if (pstrCompilerErrorCaption) pstrCompilerErrorCaption->Format(IDS_COMPERR_CAPT, iCompileResult);
		if (pstrCompilerErrorMessage) pstrCompilerErrorMessage->LoadString(STRERR_UNKNOWN);	
		break;
	}
		
	return iCompileResult;
}

BOOL CompileBooleanFormulaAndDisplayResults(const CString& strFormula)
{
	CString strCompilerErrorMessage;
	CString strCompilerErrorCaption;	
	int iCompileResult;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrApp* pNLB_Bldr = (CNLB_BldrApp*) AfxGetApp();

	if (pMainFrame && pNLB_Bldr)
	{
		CNLB_BldrDoc* pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);		
		iCompileResult = CompileBooleanFormula(pNLB_Bldr,
			pDoc, 
			strFormula,
			&strCompilerErrorMessage,
			&strCompilerErrorCaption);
		
		if (iCompileResult != IERR_ERROR_SUCCESS) 
		{
			CString strCompilerErrorQuest;

			strCompilerErrorQuest.LoadString(IDS_COMPERR_QUEST);
			strCompilerErrorMessage += strCompilerErrorQuest;
			if (pMainFrame->MessageBox(strCompilerErrorMessage, 
				strCompilerErrorCaption, 
				MB_ICONEXCLAMATION | MB_YESNO) == IDYES) 
				return TRUE;
			else
				return FALSE;
		}
		else
			return TRUE;
	}
	else
		return FALSE;
}

BOOL CheckVarCorrect(const CString& strVarName)
{
	CString strPageVar = strVarName;
	strPageVar.TrimLeft((WCHAR) ' ');	// Убрать пробелы слева от строки
	strPageVar.TrimRight((WCHAR) ' ');	// Убрать пробелы справа от строки
	int iLen = _tcslen(strPageVar);
	if (iLen <= 0) return FALSE;

	if (!(((strPageVar[0] >= 'a') && (strPageVar[0] <= 'z')) ||
		((strPageVar[0] >= 'A') && (strPageVar[0] <= 'Z')) ||
		(strPageVar[0] == '_'))) return FALSE; // Потому что ни с какого другого символа переменная начинаться не может
	for (int i = 1; i < iLen; i++)
	{
		if (!(((strPageVar[i] >= 'a') && (strPageVar[i] <= 'z')) ||
		((strPageVar[i] >= 'A') && (strPageVar[i] <= 'Z')) ||
		((strPageVar[i] >= '0') && (strPageVar[i] <= '9')) ||
		(strPageVar[i] == '_'))) return FALSE; // Потому что никакого другого символа переменная содержать не может
	}
	return TRUE;
}

E_ILType CheckLinkContainInfiniteLoop(CString& strFormula, CList<CBookVar*, CBookVar*&>& lstBookVarsForCurPage, BOOL fModifyStringIfSuccessful /*= FALSE*/)
{
	CString strToAdd;
	CString strCurBookVarBody;
	const CString FORMAT = "%f";
	CString strVarInBracketsBuf;	
	strVarInBracketsBuf.LoadString(IDS_STRING_VAR_IN_BRACKETS);
	CString strVarInBracketsConcatBuf;	
	strVarInBracketsConcatBuf.LoadString(IDS_STRING_VAR_IN_BRACKETS_CONCAT);	
	POSITION PosInLstBookVars;
	POSITION BookVarPos;
	CString strTmpConstraint = strFormula;
	CMathTrans* pTrans;
	int iTotalBookVarsCount;
	int iExistingBookVarCounter;
	BOOL fBookVarFound;
	BOOL fIsFormulaContainsBookVars = TRUE;	// Формула cодержит Book Vars				
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrApp* pNLB_Bldr = (CNLB_BldrApp*) AfxGetApp();

	// Для определения того, является ли переменная просто числом
	float flTempResult;	
	BOOL fWasNoVarsYet = FALSE;	// До сих пор в формуле не было ни одной переменной

	/*
	Для определения того, является ли строка просто числом, используется функция 
	_stscanf (аналог sscanf). Заметим, что такой вызов:
	_stscanf("-11.4sa", "%f", &flTempResult);
	является абсолютно верным, и flTempResult == -11.4, однако, вызовы типа
	_stscanf("as77sa", "%f", &flTempResult);
	и
	_stscanf("assa77", "%f", &flTempResult);
	провалятся.
	В первом случае, вообще-то, не совсем корректно считать переменную числом,
	на самом деле это ошибка. Но об этом будет заявлено на этапе компиляции 
	соответствующей формулы, так что здесь можно не париться и считать,
	что переменная действительно имеет значение числа. Главное, что этот
	подход позволит выявить истинные числа, которые переживут компиляцию.	
	*/

	if (pMainFrame && pNLB_Bldr)
	{			
		CNLB_BldrDoc* pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			// Совершаем цикл, пока не доберемся (в результате замен) до формулы,
			// содержащей только Page Vars
			// В процессе выполнения этого цикла strTmpConstraint будет постепенно 
			// превращаться в монстра, составленного из тел всех переменных 
			// из текущей формулы, соединенных операцией конъюнкции.
			// Короче, см лучше код...

			/* 
			Вот. Есть, правда, еще проблема: этот цикл может оказаться
			бесконечным, если пользователь допустит ошибку и задаст 
			формулу, которая циклится, например:
			var v1 = v3;
			var v2 = v1;
			var v3 = v2;
			Но и тут есть способ. Надо сделать так: подсчитывать число 
			итераций, в течение которого не было добавлено ни одной 
			новой ссылки в список lstBookVarsForCurPage, и если этот счетчик 
			превысит общее число Book Variables во всей книге, то это означает,
			что имеет место описанная выше ситуация.
			*/
			iTotalBookVarsCount = pDoc->m_lstBookVars.GetCount();
			iExistingBookVarCounter = 0;
			while (fIsFormulaContainsBookVars)
			{
				if (iExistingBookVarCounter > iTotalBookVarsCount) return E_IL_ILEXISTS;	// Есть бесконечный цикл!
				
				fIsFormulaContainsBookVars = FALSE;	// А вдруг уже не содержит???
				pTrans = new CMathTrans(strTmpConstraint);							
				if (pTrans)
				{			
					if (pTrans->Compile() != IERR_ERROR_SUCCESS) return E_IL_COMPERR;	// Нет бесконечного цикла, так как даже компиляция провалилась!
					// Теперь осталось только проверить, что все переменные из этой формулы есть в нашей книге. 							

					try
					{
						CString strCurVar((WCHAR) ' ', MAX_VAR_LENGTH);	// MAX_VAR_LENGTH пробелов. MAX_VAR_LENGTH - константа из CLogicTrans.h. Идентификатор переменной не может иметь большую длину.			
						
						if (pTrans->GetFirstVar(strCurVar, MAX_VAR_LENGTH))
						{																																								
							BookVarPos = pDoc->FindBookVarPositionByName(strCurVar);
							if (BookVarPos)
							{													
								fIsFormulaContainsBookVars = TRUE;	
								strCurBookVarBody = pDoc->m_lstBookVars.GetAt(BookVarPos)->Get_strBookVarBody();
								
								if (_stscanf(strCurBookVarBody, FORMAT, &flTempResult) == 1)	// Строка представляет собой число - подменим его
								{
									fWasNoVarsYet = TRUE;	// Просто ничего не добавляем в формулу
									strTmpConstraint.Empty();
								}
								else	
									strTmpConstraint.Format(strVarInBracketsBuf, strCurBookVarBody);
								
								fBookVarFound = FALSE;
								PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
								while (PosInLstBookVars)
								{
									if (lstBookVarsForCurPage.GetNext(PosInLstBookVars)->Get_strBookVarName() == strCurVar) 
									{
										iExistingBookVarCounter++;	// Такая уже есть в списке... Увеличиваем злобный счетчик...
										fBookVarFound = TRUE;
										break;
									}
								}

								if (!fBookVarFound)	// Т.е такой еще нет
								{	
									iExistingBookVarCounter = 0;	// Так как вставляем новую ссылку
									lstBookVarsForCurPage.AddHead(pDoc->m_lstBookVars.GetAt(BookVarPos));	// Тогда добавим...
								}
							}
							else						
								strTmpConstraint.Format(strVarInBracketsBuf, strCurVar);									

							while (pTrans->GetNextVar(strCurVar, MAX_VAR_LENGTH))
							{
								BookVarPos = pDoc->FindBookVarPositionByName(strCurVar);
								if (BookVarPos)
								{													
									fIsFormulaContainsBookVars = TRUE;	
									strCurBookVarBody = pDoc->m_lstBookVars.GetAt(BookVarPos)->Get_strBookVarBody();

									if (_stscanf(strCurBookVarBody, FORMAT, &flTempResult) != 1)
									{
										if (fWasNoVarsYet)
										{
											strToAdd.Format(strVarInBracketsBuf, strCurBookVarBody);										
											fWasNoVarsYet = FALSE;
										}
										else
											strToAdd.Format(strVarInBracketsConcatBuf, strCurBookVarBody);										
									}
									
									strTmpConstraint += strToAdd;

									fBookVarFound = FALSE;
									PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
									while (PosInLstBookVars)
									{
										if (lstBookVarsForCurPage.GetNext(PosInLstBookVars)->Get_strBookVarName() == strCurVar) 
										{
											iExistingBookVarCounter++;	// Такая уже есть в списке... Увеличиваем злобный счетчик...
											fBookVarFound = TRUE;
											break;
										}
									}

									if (!fBookVarFound)	// Т.е такой еще нет
									{										
										iExistingBookVarCounter = 0;	// Так как вставляем новую ссылку
										lstBookVarsForCurPage.AddHead(pDoc->m_lstBookVars.GetAt(BookVarPos));	// Тогда добавим...
									}
								}
								else
								{
									strToAdd.Format(strVarInBracketsConcatBuf, strCurVar);
									strTmpConstraint += strToAdd;
								}
							}
						}
					}
					catch (CMemoryException e)
					{
						((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
					}
					delete pTrans;
				}
				else
					((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);					
			}
		}

		if (fModifyStringIfSuccessful) strFormula = strTmpConstraint;
		return E_IL_OK;	// Нет бесконечного цикла!
	}
	else
		return E_IL_APP_ERROR;	// Произошла ошибка!
}

VOID SortBookVars(CList<CBookVar*, CBookVar*&>& lstBookVarsForCurPage)
{
	// Расставим переменные в списке в нужном порядке, т.е. так, чтобы не было такого:
	// var fVar1 = fVar2; -- ОШИБКА, так как fVar2 используется еще до объявления!!!
	// var fVar2 = ...;	
	CBookVar* pBookVarCur;
	CBookVar* pBookVarAtWrongPos;
	CMathTrans* pTrans;
	POSITION PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
	while (PosInLstBookVars)
	{
		POSITION PosTmp;
		POSITION PosDel;

		PosDel = NULL;
		pBookVarCur = lstBookVarsForCurPage.GetAt(PosInLstBookVars);
		PosTmp = PosInLstBookVars;			
		while (PosTmp)
		{
			pBookVarAtWrongPos = lstBookVarsForCurPage.GetAt(PosTmp);
			pTrans = new CMathTrans(pBookVarAtWrongPos->Get_strBookVarBody());					
			if (pTrans)
			{
				pTrans->Compile();	// Ошибки компиляции к этому моменту уже не будет...

				try
				{
					CString strCurVar((WCHAR) ' ', MAX_VAR_LENGTH);	// MAX_VAR_LENGTH пробелов. MAX_VAR_LENGTH - константа из CLogicTrans.h. Идентификатор переменной не может иметь большую длину.			
					
					if (pTrans->GetFirstVar(strCurVar, MAX_VAR_LENGTH))
					{																																																								
						if (pBookVarCur->Get_strBookVarName() == strCurVar)
						{
							PosDel = PosTmp;
							lstBookVarsForCurPage.GetPrev(PosTmp);
							lstBookVarsForCurPage.RemoveAt(PosDel);
							lstBookVarsForCurPage.AddTail(pBookVarAtWrongPos);
						}
						else
						{
							while (pTrans->GetNextVar(strCurVar, MAX_VAR_LENGTH))
							{
								if (pBookVarCur->Get_strBookVarName() == strCurVar)
								{
									PosDel = PosTmp;
									lstBookVarsForCurPage.GetPrev(PosTmp);
									lstBookVarsForCurPage.RemoveAt(PosDel);
									lstBookVarsForCurPage.AddTail(pBookVarAtWrongPos);
									break;
								}
							}

							if (!PosDel) lstBookVarsForCurPage.GetPrev(PosTmp);	// Если ничего не переместили, просто перейдем к следующему элементу...
						}
					}
					else 
						lstBookVarsForCurPage.GetPrev(PosTmp);	// Текущая переменная вообще не содержит никаких других (просто число, к примеру). Перейдем к следующему (т.е. к предыдущему в списке) элементу
				}
				catch (CMemoryException e)
				{
					((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
				}

				delete pTrans;
			}
			else
				((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
		}
		
		lstBookVarsForCurPage.GetNext(PosInLstBookVars);	// Переходим к следующему элементу			
	}
}