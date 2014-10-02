// DialogStrVarProperties.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "DialogStrVarProperties.h"
#include "MainFrm.h"
#include "NLB_BldrDoc.h"
#include "funcs.h"
#include "MathTrans\CMathTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogStrVarProperties dialog


CDialogStrVarProperties::CDialogStrVarProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogStrVarProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStrVarProperties)
	m_strStrVarBody = _T("");
	m_strStrVarName = _T("");
	m_fSaveInTheBookmarksFile = FALSE;
	//}}AFX_DATA_INIT
}


void CDialogStrVarProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStrVarProperties)
	DDX_Control(pDX, IDC_LIST_STRVAR_DEPENDENCIES, m_lstbxStrVarDependencies);
	DDX_Control(pDX, IDC_CHECK_SAVE_IN_THE_BOOKMARKS_FILE, m_chkSaveInTheBookmarksFile);
	DDX_Control(pDX, IDC_LIST_STRVAR_VARS, m_lstbxStrVarParams);
	DDX_Control(pDX, IDC_BUTTON_MOVE_UP, m_btnMoveUp);
	DDX_Control(pDX, IDC_BUTTON_MOVE_DOWN, m_btnMoveDown);
	DDX_Control(pDX, IDC_EDIT_STRVAR_NAME, m_edtStrVarName);
	DDX_Text(pDX, IDC_EDIT_STRVAR_BODY, m_strStrVarBody);
	DDX_Text(pDX, IDC_EDIT_STRVAR_NAME, m_strStrVarName);
	DDX_Check(pDX, IDC_CHECK_SAVE_IN_THE_BOOKMARKS_FILE, m_fSaveInTheBookmarksFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogStrVarProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogStrVarProperties)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_LIST, OnButtonRefreshList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStrVarProperties message handlers

void CDialogStrVarProperties::OnOK() 
{
	CString strErrorMessage;
	CString strErrorCaption;
	BOOL fOk = true;	// Будем оптимистами!
	BOOL fVarChanged;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;
	
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);		
		UpdateData(TRUE);
		fVarChanged = (m_strInitialVarName != m_strStrVarName);

		if (((pDoc->FindPageIndexByVar(m_strStrVarName) != -1) ||
			(pDoc->FindBookVarPositionByName(m_strStrVarName)) ||
			(pDoc->FindProbVarPositionByName(m_strStrVarName)) ||
			(pDoc->FindStrVarPositionByName(m_strStrVarName)) ||
			(pDoc->FindTSVarPositionByName(m_strStrVarName))) && (fVarChanged))
		{
			fOk = false;	
			strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
			strErrorMessage.Format(IDS_ERROR_VAR_ALREADY_EXISTS, m_strStrVarName);
			MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
			m_edtStrVarName.SetSel(0, -1);	// Выделить весь текст в Edit
			m_edtStrVarName.SetFocus();
		}

		if (!CheckVarCorrect(m_strStrVarName))
		{
			fOk = false;	
			strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
			strErrorMessage.Format(IDS_ERROR_VAR_INCORRECT, m_strStrVarName);
			MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
			m_edtStrVarName.SetSel(0, -1);	// Выделить весь текст в Edit
			m_edtStrVarName.SetFocus();
		}		

		if (fOk)
		{
			if (RefreshVarsList())
			{
				CDialog::OnOK();
				pDoc->SetModifiedFlag(TRUE);
			}
		}
	}	
}

BOOL CDialogStrVarProperties::OnInitDialog() 
{	
	CDialog::OnInitDialog();
	
	m_strInitialVarName	= m_strStrVarName;	

	m_edtStrVarName.SetSel(0, -1);	// Выделить весь текст в Edit
	m_edtStrVarName.SetFocus();
	
	RemoveAbsentVarsAndFillListBoxParameters();
	RemoveAbsentVarsAndFillListBoxDependencies();

	if ((m_lstStrVarParameters.GetCount() == 0) &&
		(m_lstStrVarDependencies.GetCount() == 0))
		m_chkSaveInTheBookmarksFile.EnableWindow(TRUE);
	else
	{
		m_chkSaveInTheBookmarksFile.SetCheck(BST_UNCHECKED);
		m_chkSaveInTheBookmarksFile.EnableWindow(FALSE);
	}

	return FALSE;

	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CDialogStrVarProperties::RemoveAbsentVarsAndFillListBoxParameters()
{
	POSITION pos, pos_old;
	int i, iParamsCount;	
	CString strTmp, strTmpToFind;

	iParamsCount = m_lstbxStrVarParams.GetCount();
	for (i = 0; i < iParamsCount; i++)
		m_lstbxStrVarParams.DeleteString(0);

	pos = pos_old = m_lstStrVarParameters.GetHeadPosition();
	iParamsCount = m_lstStrVarParameters.GetCount();
	i = 0;
	while (i < iParamsCount)
	{
		strTmp = m_lstStrVarParameters.GetNext(pos);		 
		strTmpToFind = _T('#') + strTmp + _T('#');
		if (m_strStrVarBody.Find(strTmpToFind) != -1)	// Есть такой параметр в теле переменной
		{
			m_lstbxStrVarParams.AddString(strTmp);			
		}
		else
			m_lstStrVarParameters.RemoveAt(pos_old);

		pos_old = pos;
		i++;		
	}
}

VOID CDialogStrVarProperties::RemoveAbsentVarsAndFillListBoxDependencies()
{
	POSITION pos, pos_old;
	int i, iParamsCount;	
	CString strTmpToFind;

	iParamsCount = m_lstbxStrVarDependencies.GetCount();
	for (i = 0; i < iParamsCount; i++)
		m_lstbxStrVarDependencies.DeleteString(0);

	pos = pos_old = m_lstStrVarDependencies.GetHeadPosition();
	iParamsCount = m_lstStrVarDependencies.GetCount();
	i = 0;
	while (i < iParamsCount)
	{
		strTmpToFind = m_lstStrVarDependencies.GetNext(pos);		
		if (m_strStrVarBody.Find(strTmpToFind) != -1)	// Есть такой параметр в теле переменной
		{
			m_lstbxStrVarDependencies.AddString(strTmpToFind);			
		}
		else
			m_lstStrVarDependencies.RemoveAt(pos_old);

		pos_old = pos;
		i++;		
	}
}

void CDialogStrVarProperties::OnButtonRefreshList() 
{
	RefreshVarsList();
}

BOOL CDialogStrVarProperties::RefreshVarsList()
// Функция возвращает FALSE, если не удалось найти в списке переменных какую-либо
// переменную из текста текущей. События pMainFrame == NULL и pDoc == NULL слишком маловероятны,
// потому на них функция не реагирует. 
{
	int iLastParamPos;
	int iLastParamEndPos;
	int iLenVarBody;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;
	CString strCurSVarParam;	
	CString strCurSVarParamWithControlSequence;
	CString strMessage;
	CString strCaption;
	POSITION svPos;	
	CStringVar* pStrVarCur;
	POSITION bvPos;	
	CBookVar* pBookVarCur;
	BOOL fOkSV;
	BOOL fOkBV;
	CString strStrVarParamSeps;
	
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);	
			
		if (pDoc)
		{			
			strStrVarParamSeps.LoadString(IDS_STR_VAR_PARAM_SEPS);
			UpdateData(TRUE);		
			// Ищем параметры типа #
			iLastParamPos = 0;	// Не -1, чтобы мы могли попасть в цикл
			iLenVarBody = m_strStrVarBody.GetLength();
			while (true)
			{
				iLastParamPos = m_strStrVarBody.Find(_T('#'), iLastParamPos);
				if ((iLastParamPos != -1) && (iLastParamPos < iLenVarBody - 1))
				{
					if (m_strStrVarBody[iLastParamPos + 1] != _T('#'))	// ## - символ #
					{
						iLastParamEndPos = m_strStrVarBody.Find(_T('#'), iLastParamPos + 1);
						if (iLastParamEndPos != -1)
						{
							strCurSVarParam = m_strStrVarBody.Mid(iLastParamPos + 1, iLastParamEndPos - iLastParamPos - 1);
							strCurSVarParam.TrimLeft();
							strCurSVarParam.TrimRight();
							
							if (strCurSVarParam.GetLength() != 0)
							{
								if (strCurSVarParam != m_strStrVarName)
								{			
									if (CheckVarCorrect(strCurSVarParam))
									{
										if (!m_lstStrVarParameters.Find(strCurSVarParam))
										{
											m_lstStrVarParameters.AddTail(strCurSVarParam);									
										}
									}
									else
									{
										strCaption.LoadString(IDS_ERROR_MB_CAPT);
										strMessage.Format(IDS_ERROR_VAR_INCORRECT, strCurSVarParam);
										MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
										return FALSE;
									}
									
									iLastParamPos = iLastParamEndPos + 1;
								}
								else
								{								
									strMessage.LoadString(IDS_STRERR_SELFREFERENCE_FOUND);
									strCaption.LoadString(IDS_ERROR_MB_CAPT);

									MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
									return FALSE;
								}
							}
							else
							{								
								strMessage.LoadString(IDS_STRERR_EMPTY_STRVAR_PARAM);
								strCaption.LoadString(IDS_ERROR_MB_CAPT);

								MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
								return FALSE;
							}
						}
						else
						{							
							strMessage.LoadString(IDS_STRERR_ERROR_IN_STRVAR_PARAM_SYNTAX);
							strCaption.LoadString(IDS_ERROR_MB_CAPT);

							MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
							return FALSE;
						}						
					}
					else
					{
						iLastParamPos += 2;
						if (iLastParamPos >= iLenVarBody) break;
					}
				}
				else
				{			
					if (iLastParamPos == iLenVarBody - 1)
					{
						strMessage.LoadString(IDS_STRERR_ERROR_IN_STRVAR_PARAM_SYNTAX);
						strCaption.LoadString(IDS_ERROR_MB_CAPT);

						MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
						return FALSE;
					}
						
					break;
				}
			}

			// Ищем параметры типа $
			iLastParamPos = 0;	// Не -1, чтобы мы могли попасть в цикл
			iLenVarBody = m_strStrVarBody.GetLength();
			while (true)
			{
				iLastParamPos = m_strStrVarBody.Find(_T('$'), iLastParamPos);
				if ((iLastParamPos != -1) && (iLastParamPos < iLenVarBody - 1))
				{
					if (m_strStrVarBody[iLastParamPos + 1] != _T('$'))	// $$ - символ $
					{
						iLastParamEndPos = m_strStrVarBody.Find(_T('$'), iLastParamPos + 1);
						if (iLastParamEndPos != -1)
						{					
							strCurSVarParamWithControlSequence = m_strStrVarBody.Mid(iLastParamPos, iLastParamEndPos - iLastParamPos + 1);
							strCurSVarParam = m_strStrVarBody.Mid(iLastParamPos + 1, iLastParamEndPos - iLastParamPos - 1);
							strCurSVarParam.TrimLeft();
							strCurSVarParam.TrimRight();
							
							if (strCurSVarParam.GetLength() != 0)
							{
								svPos = pDoc->FindStrVarPositionByName(strCurSVarParam);
								fOkSV = TRUE;
								if (svPos)
								{
									pStrVarCur = pDoc->m_lstStrVars.GetAt(svPos);
									if ((pStrVarCur->m_lstStrVarParameters.GetCount() == 0) && 
										(pStrVarCur->m_lstStrVarDependencies.GetCount() == 0) &&
										(pStrVarCur->Get_fSaveInTheBookmarksFile()))
									{
										if (strCurSVarParam != m_strStrVarName)
										{														
											if (!m_lstStrVarDependencies.Find(strCurSVarParamWithControlSequence))
											{
												m_lstStrVarDependencies.AddTail(strCurSVarParamWithControlSequence);									
											}											
											
											iLastParamPos = iLastParamEndPos + 1;
										}
										else
										{								
											strMessage.LoadString(IDS_STRERR_SELFREFERENCE_FOUND);
											strCaption.LoadString(IDS_ERROR_MB_CAPT);

											MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
											return FALSE;
										}
									}
									else
										fOkSV = FALSE;
								}
								else
									fOkSV = FALSE;
								
								if (!fOkSV)
								{																	
									strCaption.LoadString(IDS_ERROR_MB_CAPT);
									strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_INPUT_BUFFER_WITHOUT_EDIT_GREETING, strCurSVarParam);
									MessageBox(strMessage, strCaption, MB_ICONEXCLAMATION | MB_OK);
									return FALSE;
								}
							}
							else
							{								
								strMessage.LoadString(IDS_STRERR_EMPTY_STRVAR_PARAM);
								strCaption.LoadString(IDS_ERROR_MB_CAPT);

								MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
								return FALSE;
							}
						}
						else
						{							
							strMessage.LoadString(IDS_STRERR_ERROR_IN_STRVAR_PARAM_SYNTAX2);
							strCaption.LoadString(IDS_ERROR_MB_CAPT);

							MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
							return FALSE;
						}						
					}
					else
					{
						iLastParamPos += 2;
						if (iLastParamPos >= iLenVarBody) break;
					}
				}
				else
				{			
					if (iLastParamPos == iLenVarBody - 1)
					{
						strMessage.LoadString(IDS_STRERR_ERROR_IN_STRVAR_PARAM_SYNTAX2);
						strCaption.LoadString(IDS_ERROR_MB_CAPT);

						MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
						return FALSE;
					}
						
					break;
				}
			}

			// Ищем параметры типа %
			iLastParamPos = 0;	// Не -1, чтобы мы могли попасть в цикл
			iLenVarBody = m_strStrVarBody.GetLength();
			while (true)
			{
				iLastParamPos = m_strStrVarBody.Find(_T('%'), iLastParamPos);
				if ((iLastParamPos != -1) && (iLastParamPos < iLenVarBody - 1))
				{
					if (m_strStrVarBody[iLastParamPos + 1] != _T('%'))	// %% - символ %
					{
						iLastParamEndPos = m_strStrVarBody.Find(_T('%'), iLastParamPos + 1);
						if (iLastParamEndPos != -1)
						{					
							strCurSVarParamWithControlSequence = m_strStrVarBody.Mid(iLastParamPos, iLastParamEndPos - iLastParamPos + 1);
							strCurSVarParam = m_strStrVarBody.Mid(iLastParamPos + 1, iLastParamEndPos - iLastParamPos - 1);
							strCurSVarParam.TrimLeft();
							strCurSVarParam.TrimRight();
							
							if (strCurSVarParam.GetLength() != 0)
							{								
								bvPos = pDoc->FindBookVarPositionByName(strCurSVarParam);										
								fOkSV = FALSE;
								fOkBV = TRUE;								
								if (bvPos)
								{
									pBookVarCur = pDoc->m_lstBookVars.GetAt(bvPos);
									if (!(pBookVarCur->Get_fIsVarGlobal())) fOkBV = FALSE;
								}
								else
								{
									fOkSV = TRUE;
									fOkBV = FALSE;
									svPos = pDoc->FindStrVarPositionByName(strCurSVarParam);									
									if (svPos)
									{
										pStrVarCur = pDoc->m_lstStrVars.GetAt(svPos);
										if (pStrVarCur->m_lstStrVarParameters.GetCount() != 0) fOkSV = FALSE;										
									}
									else
									{
										TCHAR* pstrCurSVarName;
										int iCurParamCount;						
										TCHAR* pstrTempVarName;	// Она будет изменяться _tcstok()											
										CMathTrans TransStrVarFuncFormula(strCurSVarParam);
										BOOL fVarsFound = TRUE;
										CStringVar* psv;

										pstrTempVarName = new TCHAR[strCurSVarParam.GetLength() + 1];
										if (!pstrTempVarName) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
										
										if (pstrTempVarName)
										{
											_tcscpy(pstrTempVarName, strCurSVarParam);								
											
											pstrCurSVarName = _tcstok(pstrTempVarName, strStrVarParamSeps);

											while (pstrCurSVarName != NULL)
											{
												svPos = pDoc->FindStrVarPositionByName(pstrCurSVarName);
												if (svPos)	// Т.е. такая переменная действительно есть
												{
													psv = pDoc->m_lstStrVars.GetAt(svPos);
													iCurParamCount = psv->m_lstStrVarParameters.GetCount();
													if (iCurParamCount > 0)
													{
														TransStrVarFuncFormula.AddFunctionDefinition(pstrCurSVarName, _T('S'), _T('S'), iCurParamCount);
													}
												}
												else
													fVarsFound = FALSE;

												pstrCurSVarName = _tcstok(NULL, strStrVarParamSeps);
											}

											if (fVarsFound)
											{
												if (TransStrVarFuncFormula.Compile() == IERR_ERROR_SUCCESS)
													fOkSV = TRUE;
												else
													fOkSV = FALSE;
											}
											else
												fOkSV = FALSE;

											delete []pstrTempVarName;
										}
									}
								}
								
								if (fOkSV || fOkBV)
								{
									if (strCurSVarParam != m_strStrVarName)
									{													
										if (!m_lstStrVarDependencies.Find(strCurSVarParamWithControlSequence))
										{
											m_lstStrVarDependencies.AddTail(strCurSVarParamWithControlSequence);									
										}										
										
										iLastParamPos = iLastParamEndPos + 1;
									}
									else
									{								
										strMessage.LoadString(IDS_STRERR_SELFREFERENCE_FOUND);
										strCaption.LoadString(IDS_ERROR_MB_CAPT);

										MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
										return FALSE;
									}
								}
								else
								{
									// (!fOkSV) && (!fOkBV)
																									
									strCaption.LoadString(IDS_ERROR_MB_CAPT);
									strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_VALUE_BUFFER_WITHOUT_EDIT_GREETING_FOR_DIALOG_STRVAR_PROPERTIES, strCurSVarParam);
									MessageBox(strMessage, strCaption, MB_ICONEXCLAMATION | MB_OK);										

									return FALSE;
								}
							}
							else
							{								
								strMessage.LoadString(IDS_STRERR_EMPTY_STRVAR_PARAM);
								strCaption.LoadString(IDS_ERROR_MB_CAPT);

								MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
								return FALSE;
							}
						}
						else
						{							
							strMessage.LoadString(IDS_STRERR_ERROR_IN_STRVAR_PARAM_SYNTAX3);
							strCaption.LoadString(IDS_ERROR_MB_CAPT);

							MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
							return FALSE;
						}						
					}
					else
					{
						iLastParamPos += 2;
						if (iLastParamPos >= iLenVarBody) break;
					}
				}
				else
				{			
					if (iLastParamPos == iLenVarBody - 1)
					{
						strMessage.LoadString(IDS_STRERR_ERROR_IN_STRVAR_PARAM_SYNTAX3);
						strCaption.LoadString(IDS_ERROR_MB_CAPT);

						MessageBox(strMessage, strCaption, MB_OK | MB_ICONEXCLAMATION);
						return FALSE;
					}
						
					break;
				}
			}
		}
	}
		
	RemoveAbsentVarsAndFillListBoxParameters();
	RemoveAbsentVarsAndFillListBoxDependencies();

	if ((m_lstStrVarParameters.GetCount() == 0) &&
		(m_lstStrVarDependencies.GetCount() == 0))
		m_chkSaveInTheBookmarksFile.EnableWindow(TRUE);
	else
	{
		m_chkSaveInTheBookmarksFile.SetCheck(BST_UNCHECKED);
		m_chkSaveInTheBookmarksFile.EnableWindow(FALSE);
	}

	return TRUE;
}
