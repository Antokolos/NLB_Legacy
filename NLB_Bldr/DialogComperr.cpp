// DialogComperr.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "DialogComperr.h"
#include "DialogBookVarProperties.h"
#include "NLB_BldrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogComperr dialog


CDialogComperr::CDialogComperr(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogComperr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogComperr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_iIconPosLink = -1;
	m_iIconPosBookVar = -1;
}


void CDialogComperr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogComperr)
	DDX_Control(pDX, IDC_LIST_COMPERR, m_lstCompErr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogComperr, CDialog)
	//{{AFX_MSG_MAP(CDialogComperr)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_COMPERR, OnDblclkListComperr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogComperr message handlers

void CDialogComperr::OnCancel() 
{
	// CDialog::OnCancel(); - закрыть диалог по Esc теперь нельзя!
}

void CDialogComperr::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	DestroyWindow();	// Use the CWnd::DestroyWindow function to destroy a dialog box created by the Create function.
	((CNLB_BldrView*) m_pParent)->m_fIsDialogComperrActivated = FALSE;	// Т.к. теперь диалог закрыт!
}

BOOL CDialogComperr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	HICON hIconNewLink = AfxGetApp()->LoadIcon(IDR_LINK);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	HICON hIconNewBookVar = AfxGetApp()->LoadIcon(IDR_BOOKVAR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstCompErr.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_LNK_NAME);		
	m_lstCompErr.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVERR_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_ERROR);		
	m_lstCompErr.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVERR_COL1_WIDTH,
		0);
	
	m_ImListComperr.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								2, 
								1);
	m_iIconPosLink = m_ImListComperr.Add(hIconNewLink);
	m_iIconPosBookVar = m_ImListComperr.Add(hIconNewBookVar);
	m_pImListComperrOld = m_lstCompErr.SetImageList(&m_ImListComperr, LVSIL_SMALL);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogComperr::OnDestroy() 
{
	S_CompoundLVItemData* pclid;
	
	CDialog::OnDestroy();	
	m_lstCompErr.SetImageList(m_pImListComperrOld, LVSIL_SMALL);		
	m_ImListComperr.DeleteImageList();
	
	int ciItems = m_lstCompErr.GetItemCount();
	for (int i=0; i<ciItems; i++)
	{		
		pclid = (S_CompoundLVItemData*) m_lstCompErr.GetItemData(i);				
		if (pclid) delete pclid;
	}
}

void CDialogComperr::OnDblclkListComperr(NMHDR* pNMHDR, LRESULT* pResult) 
{
	S_CompoundLVItemData* pclid;
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	CNLB_BldrView* pView = (CNLB_BldrView*) m_pParent;

	if (pView && lpnmitem)
	{		
		pclid = (S_CompoundLVItemData*) m_lstCompErr.GetItemData(lpnmitem->iItem);
		if (pclid)
		{
			if (pclid->cliType == CLI_Link)
			{
				pView->DeselectAll();
				pView->itSelItemType = SelItem_Link;
				pView->m_pLastSelectedBookItem = (CLink*) pclid->lpCLIObject;				
				pView->m_fIsDraggingStarted = FALSE;	// Чтобы не было проблем
				pView->m_fIsDraggingInProgress = FALSE;	// Чтобы не было проблем
				((CLink*) pclid->lpCLIObject)->Select();
				pView->ScrollToLink((CLink*) pclid->lpCLIObject);
				pView->EditlinkEditproperties();
			}
			else if (pclid->cliType == CLI_BookVar)
			{
				CDialogBookVarProperties dlg;
				CBookVar* pBookVar = (CBookVar*) pclid->lpCLIObject;

				dlg.m_strBookVarName = pBookVar->Get_strBookVarName();
				dlg.m_strBookVarBody  = pBookVar->Get_strBookVarBody();
				dlg.m_fIsVarGlobal = pBookVar->Get_fIsVarGlobal();
				if (dlg.DoModal() == IDOK)
				{
					pBookVar->Get_strBookVarName() = dlg.m_strBookVarName;
					pBookVar->Get_strBookVarBody() = dlg.m_strBookVarBody;							
					pBookVar->Get_fIsVarGlobal() = dlg.m_fIsVarGlobal;
				}
			}
		}		
	}
	
	*pResult = 0;
}
