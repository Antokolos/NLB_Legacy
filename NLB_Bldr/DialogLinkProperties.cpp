// DialogLinkProperties.cpp : implementation file
//

#include "stdafx.h"
#include "DialogLinkProperties.h"
#include "DialogModification.h"
#include "MainFrm.h"
#include "funcs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogLinkProperties dialog


CDialogLinkProperties::CDialogLinkProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLinkProperties::IDD, pParent)
{	
	//{{AFX_DATA_INIT(CDialogLinkProperties)
	m_strLinkCaption = _T("");
	m_strLinkConstraints = _T("");
	//}}AFX_DATA_INIT
	m_iIconPos = -1;
}


void CDialogLinkProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLinkProperties)
	DDX_Control(pDX, IDC_LIST_MODIFICATIONS, m_lstModifications);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_EDIT_LINK_CAPTION, m_edtLnkCaption);
	DDX_Control(pDX, IDC_BUTTON_SET_LINK_COLOR, m_btnSetLinkColor);
	DDX_Text(pDX, IDC_EDIT_LINK_CAPTION, m_strLinkCaption);
	DDX_Text(pDX, IDC_EDIT_LINK_CONSTRAINTS, m_strLinkConstraints);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLinkProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogLinkProperties)
	ON_BN_CLICKED(IDC_BUTTON_SET_LINK_COLOR, OnButtonSetLinkColor)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MODIFICATIONS, OnItemchangedListModifications)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MODIFICATIONS, OnDblclkListModifications)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogLinkProperties message handlers

void CDialogLinkProperties::OnButtonSetLinkColor() 
{
	CColorDialog dlg;
	LOGBRUSH lbrush;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = m_lPen.lopnColor;
	m_btnSetLinkColor.GetLogBrush(&lbrush);

	if (dlg.DoModal() == IDOK)
	{
		m_lPen.lopnColor = dlg.GetColor();
		lbrush.lbColor = dlg.GetColor();		
		m_btnSetLinkColor.SetLogBrush(&lbrush);
		m_btnSetLinkColor.Invalidate();
	}			
}

BOOL CDialogLinkProperties::OnInitDialog() 
{
	int iModCount;
	POSITION pos;
	S_BookVarModification* pModCur;

	CDialog::OnInitDialog();
	
	LOGBRUSH lbrush;
	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_BOOKVAR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstModifications.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_MODIFICATIONS_VARNAME);		
	m_lstModifications.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVMODIFICATIONS_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_MODIFICATIONS_VARMODIFICATION);		
	m_lstModifications.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVMODIFICATIONS_COL1_WIDTH,
		0);
	
	m_ImListModifications.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListModifications.Add(hIconNew);
	m_pImListModificationsOld = m_lstModifications.SetImageList(&m_ImListModifications, LVSIL_SMALL);	

	iModCount = m_lstModificationsObj.GetModificationsCount();
	pos = m_lstModificationsObj.GetModificationsHeadPosition();
	for (int i = 0; i < iModCount; i++)
	{
		pModCur = m_lstModificationsObj.GetNextModification(pos);
		m_lstModifications.InsertItem(i, pModCur->pModifiedBookVar->Get_strBookVarName(), m_iIconPos);
		m_lstModifications.SetItemText(i, 1, pModCur->pModificationBody->Get_strBookVarName());
		m_lstModifications.SetItemData(i, (DWORD) pModCur);
	}	

	m_btnEdit.EnableWindow(FALSE);
	m_btnDelete.EnableWindow(FALSE);
			
	lbrush.lbStyle = BS_SOLID;
	lbrush.lbHatch = 0;	// If lbStyle is BS_SOLID or BS_HOLLOW, lbHatch is ignored
	lbrush.lbColor = m_lPen.lopnColor;
	m_btnSetLinkColor.SetLogBrush(&lbrush);	
	m_edtLnkCaption.SetSel(0, -1);	// Выделить весь текст в Edit
	m_edtLnkCaption.SetFocus();
	return FALSE;

	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogLinkProperties::OnOK() 
{	
	CString strErrorCaption;
	CString strErrorMessage;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;	
	
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);		
		UpdateData(TRUE);

		if (m_strLinkCaption.GetLength() != 0)	// Имя ссылки не может быть пустым!!!
		{
			CList<CBookVar*, CBookVar*&> lstBookVarsForCurPage;

			if (CompileBooleanFormulaAndDisplayResults(m_strLinkConstraints))
			{
				E_ILType ilResult = CheckLinkContainInfiniteLoop(m_strLinkConstraints, lstBookVarsForCurPage);
				if ((ilResult == E_IL_OK) ||	// Это имеет место, если формула верна и бесконечных циклов нет
					(ilResult == E_IL_COMPERR))	// Это имеет место, если формула неверна, но пользователь все равно выбрал сохранить ее
				{
					pDoc->SetModifiedFlag(TRUE);
					CDialog::OnOK();
				}
				else if (ilResult == E_IL_ILEXISTS)
				{
					strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
					strErrorMessage.LoadString(IDS_FORMULA_CONTAINS_INFINITE_LOOP);
					if (MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
					{
						pDoc->SetModifiedFlag(TRUE);
						CDialog::OnOK();
					}
				}
				else
				{
					strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
					strErrorMessage.LoadString(IDS_ERROR_CALLING_CHECK_LINK_CONTAINS_IL);
					MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);							
				}
			}
		}
		else
		{		
			CString strCompilerErrorMessage;
			CString strCompilerErrorCaption;	

			// Правда, в этом случае это не совсем ошибки компиляции, но какая разница...
			strCompilerErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
			strCompilerErrorMessage.LoadString(IDS_ERROR_LINK_CAPT_CANNOT_BE_EMPTY);
			MessageBox(strCompilerErrorMessage, strCompilerErrorCaption, MB_ICONEXCLAMATION);
		}
	}
}

void CDialogLinkProperties::OnDestroy() 
{
	CDialog::OnDestroy();	
	m_lstModifications.SetImageList(m_pImListModificationsOld, LVSIL_SMALL);		
	m_ImListModifications.DeleteImageList();		
}

void CDialogLinkProperties::OnItemchangedListModifications(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
		{
			m_btnEdit.EnableWindow(TRUE);
			m_btnDelete.EnableWindow(TRUE);
		}
		else
		{
			m_btnEdit.EnableWindow(FALSE);
			m_btnDelete.EnableWindow(FALSE);
		}
	}		

	*pResult = 0;
}

void CDialogLinkProperties::OnButtonAdd() 
{
	CDialogModification dlg;
	S_BookVarModification* pbvmNew;
	int iLVItemCount;
	CNLB_BldrApp* pApp = NULL;
	// Сначала надо проверить - может мы и не можем пока создавать модификаций, т.к. еще не завели глобальных переменных
	BOOL fGlobalVarExists = FALSE;
	BOOL fNonGlobalVarExists = FALSE;

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CNLB_BldrDoc* pDoc;
	POSITION pos;
	int iBookVarsCount;
	CBookVar* pBookVarCur;
	
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			pos = pDoc->m_lstBookVars.GetHeadPosition();
			iBookVarsCount = pDoc->m_lstBookVars.GetCount();
			for (int i=0; i<iBookVarsCount; i++)
			{			
				pBookVarCur = pDoc->m_lstBookVars.GetAt(pos);				
				if (pBookVarCur->Get_fIsVarGlobal())
				{
					fGlobalVarExists = TRUE;
					if (fNonGlobalVarExists) break;
				}
				else
				{
					fNonGlobalVarExists = TRUE;
					if (fGlobalVarExists) break;
				}
				
				pDoc->m_lstBookVars.GetNext(pos);
			}
		}
	}

	if (fGlobalVarExists && fNonGlobalVarExists)
	{
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_bvm.pModifiedBookVar && dlg.m_bvm.pModificationBody)
			{
				// Вставим модификацию в список...
				pbvmNew = m_lstModificationsObj.AddBookVarModification(dlg.m_bvm);
				if (!pbvmNew) 
				{
					pApp->HandleError(IERR_OUT_OF_MEMORY);
					pApp = (CNLB_BldrApp*) AfxGetApp();
				}

				// ...и в ListView!
				iLVItemCount = m_lstModifications.GetItemCount();
				m_lstModifications.InsertItem(iLVItemCount, dlg.m_bvm.pModifiedBookVar->Get_strBookVarName(), m_iIconPos);
				m_lstModifications.SetItemText(iLVItemCount, 1, dlg.m_bvm.pModificationBody->Get_strBookVarName());
				m_lstModifications.SetItemData(iLVItemCount, (DWORD) pbvmNew);
			}
		}
	}
	else
	{
		CString strCaption;
		CString strMessage;	

		strMessage.LoadString(IDS_CANNOT_ADD_VARMODIFICATION);	
		strCaption.LoadString(IDS_NOTE_MB_CAPT);
		MessageBox(strMessage, strCaption, MB_ICONEXCLAMATION | MB_OK);
	}
}

void CDialogLinkProperties::OnButtonEdit() 
{
	CDialogModification dlg;	
	POSITION SelPos;
	int nItem;
	CNLB_BldrApp* pApp = NULL;
	S_BookVarModification* pbvmToEdit;	
		
	SelPos = m_lstModifications.GetFirstSelectedItemPosition();
	if (SelPos)
	{	
		nItem = m_lstModifications.GetNextSelectedItem(SelPos);
		pbvmToEdit = (S_BookVarModification*) m_lstModifications.GetItemData(nItem);
		if (pbvmToEdit)
		{
			dlg.m_bvm = (*pbvmToEdit);
			if (dlg.DoModal() == IDOK)
			{
				if (dlg.m_bvm.pModifiedBookVar && dlg.m_bvm.pModificationBody)
				{
					// Изменим модификацию в списке...
					*pbvmToEdit = dlg.m_bvm;					

					// ...и в ListView!
					m_lstModifications.SetItemText(nItem, 0, dlg.m_bvm.pModifiedBookVar->Get_strBookVarName());	
					m_lstModifications.SetItemText(nItem, 1, dlg.m_bvm.pModificationBody->Get_strBookVarName());						
				}
			}
		}
	}	
}

void CDialogLinkProperties::OnDblclkListModifications(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEdit();	
	*pResult = 0;
}

void CDialogLinkProperties::OnButtonDelete() 
{		
	POSITION pos;
	int nItem;		
	CString strCaption;
	CString strMessage;
	S_BookVarModification* pbvmToDelete;

	strMessage.LoadString(IDS_CONFIRM_DEL_VARMODIFICATION);	
	strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
	if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{				
		pos = m_lstModifications.GetFirstSelectedItemPosition();
		if (pos)
		{	
			nItem = m_lstModifications.GetNextSelectedItem(pos);
			pbvmToDelete = (S_BookVarModification*) m_lstModifications.GetItemData(nItem);				
			m_lstModificationsObj.RemoveBookVarModification(pbvmToDelete);
			m_lstModifications.DeleteItem(nItem);
		}	
	}		
}
