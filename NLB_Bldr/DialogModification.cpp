// DialogModification.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "DialogModification.h"
#include "MainFrm.h"
#include "NLB_BldrDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogModification dialog


CDialogModification::CDialogModification(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogModification::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogModification)
	//}}AFX_DATA_INIT
}


void CDialogModification::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogModification)
	DDX_Control(pDX, IDC_COMBO_VARNAME, m_cbbVarName);
	DDX_Control(pDX, IDC_COMBO_MODIFICATION, m_cbbModification);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogModification, CDialog)
	//{{AFX_MSG_MAP(CDialogModification)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogModification message handlers

BOOL CDialogModification::OnInitDialog() 
{
	int iVarNameSel = 0;
	int iModificationSel = 0;
	int iVarNameCount = 0;
	int iModificationCount = 0;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	CNLB_BldrDoc* pDoc;
	POSITION pos;
	int iBookVarsCount;
	CBookVar* pBookVarCur;
	int iItemPos;

	CDialog::OnInitDialog();
	
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
					// iItemPos = m_cbbVarName.AddString(pBookVarCur->Get_strBookVarName());
					// Вместо нее используем InsertString, так как
					// Unlike the AddString method, InsertString does not cause a list with the LBS_SORT style to be sorted.
					// Или же надо быть уверенным, что в списке не стоит галка автосортировки
					// -1 означает "добавить в конец"
					iItemPos = m_cbbVarName.InsertString(-1, pBookVarCur->Get_strBookVarName());
					if (iItemPos >= 0) 
					{
						m_cbbVarName.SetItemDataPtr(iItemPos, pBookVarCur);
						if (pBookVarCur == m_bvm.pModifiedBookVar) iVarNameSel = iVarNameCount;
						iVarNameCount++;						
					}
				}
				else
				{
					// iItemPos = m_cbbModification.AddString(pBookVarCur->Get_strBookVarName());
					iItemPos = m_cbbModification.InsertString(-1, pBookVarCur->Get_strBookVarName());
					if (iItemPos >= 0) 
					{
						m_cbbModification.SetItemDataPtr(iItemPos, pBookVarCur);
						if (pBookVarCur == m_bvm.pModificationBody) iModificationSel = iModificationCount;
						iModificationCount++;
					}
				}
				
				pDoc->m_lstBookVars.GetNext(pos);
			}
		}
	}
	
	m_cbbModification.SetCurSel(iModificationSel);
	m_cbbVarName.SetCurSel(iVarNameSel);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogModification::OnOK() 
{
	int iVarNameSel = m_cbbVarName.GetCurSel();
	int iModificationSel = m_cbbModification.GetCurSel();

	if ((iVarNameSel >= 0) && (iModificationSel >= 0))
	{
		m_bvm.pModifiedBookVar = (CBookVar*) m_cbbVarName.GetItemData(iVarNameSel);
		m_bvm.pModificationBody = (CBookVar*) m_cbbModification.GetItemData(iModificationSel);
	}
	else
	{
		m_bvm.pModifiedBookVar = m_bvm.pModificationBody = NULL;
	}
	
	CDialog::OnOK();
}

void CDialogModification::OnCancel() 
{
	m_bvm.pModifiedBookVar = m_bvm.pModificationBody = NULL;
	
	CDialog::OnCancel();
}
