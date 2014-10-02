#ifndef _FUNCS_H
#define _FUNCS_H
#include "NLB_BldrDoc.h"
#include "NLB_Bldr.h"

int CompileBooleanFormula(CNLB_BldrApp* pNLB_Bldr,
						  CNLB_BldrDoc* pDoc,
						  const CString& strFormula,
						  CString* pstrCompilerErrorMessage,
						  CString* pstrCompilerErrorCaption);
BOOL CompileBooleanFormulaAndDisplayResults(const CString& strFormula);
BOOL CheckVarCorrect(const CString& strVarName);
VOID SortBookVars(CList<CBookVar*, CBookVar*&>& lstBookVarsForCurPage);

enum E_ILType {E_IL_OK, E_IL_COMPERR, E_IL_ILEXISTS, E_IL_APP_ERROR};
E_ILType CheckLinkContainInfiniteLoop(CString& strFormula, CList<CBookVar*, CBookVar*&>& lstBookVarsForCurPage, BOOL fModifyStringIfSuccessful = FALSE);

#endif