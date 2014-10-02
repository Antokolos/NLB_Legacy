// DialogSearchResults.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "DialogSearchResults.h"
#include "DialogProbVarProperties.h"
#include "DialogBookVarProperties.h"
#include "CBookPage.h"
#include "CLink.h"
#include "CBookVar.h"
#include "CProbVar.h"
#include "NLB_BldrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSearchResults dialog


CDialogSearchResults::CDialogSearchResults(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSearchResults::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSearchResults)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_iIconPosPage = -1;
	m_iIconPosLink = -1;
	m_iIconPosBookVar = -1;
	m_iIconPosProbVar = -1;
}


void CDialogSearchResults::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSearchResults)
	DDX_Control(pDX, IDC_LIST_RESULTS, m_lstResults);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSearchResults, CDialog)
	//{{AFX_MSG_MAP(CDialogSearchResults)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RESULTS, OnDblclkListResults)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSearchResults message handlers

void CDialogSearchResults::OnCancel() 
{
	// CDialog::OnCancel(); - закрыть диалог по Esc теперь нельзя!
}

void CDialogSearchResults::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
	DestroyWindow();	// Use the CWnd::DestroyWindow function to destroy a dialog box created by the Create function.
	((CNLB_BldrView*) m_pParent)->m_fIsDialogSearchResActivated = FALSE;	// Т.к. теперь диалог закрыт!
}

BOOL CDialogSearchResults::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HICON hIconNewPage = AfxGetApp()->LoadIcon(IDR_PAGE);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	HICON hIconNewLink = AfxGetApp()->LoadIcon(IDR_LINK);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	HICON hIconNewBookVar = AfxGetApp()->LoadIcon(IDR_BOOKVAR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	HICON hIconNewProbVar = AfxGetApp()->LoadIcon(IDR_PROBVAR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstResults.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_LNK_NAME);		
	m_lstResults.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVSRES_COL0_WIDTH,
		0);
	
	m_ImListSRes.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								4, 
								1);
	m_iIconPosPage = m_ImListSRes.Add(hIconNewPage);
	m_iIconPosLink = m_ImListSRes.Add(hIconNewLink);
	m_iIconPosBookVar = m_ImListSRes.Add(hIconNewBookVar);
	m_iIconPosProbVar = m_ImListSRes.Add(hIconNewProbVar);
	m_pImListSResOld = m_lstResults.SetImageList(&m_ImListSRes, LVSIL_SMALL);		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSearchResults::OnDestroy() 
{
	S_CompoundLVItemData* pclid;
	
	CDialog::OnDestroy();	
	m_lstResults.SetImageList(m_pImListSResOld, LVSIL_SMALL);		
	m_ImListSRes.DeleteImageList();
	
	int ciItems = m_lstResults.GetItemCount();
	for (int i=0; i<ciItems; i++)
	{		
		pclid = (S_CompoundLVItemData*) m_lstResults.GetItemData(i);				
		if (pclid) delete pclid;
	}
}

void CDialogSearchResults::OnDblclkListResults(NMHDR* pNMHDR, LRESULT* pResult) 
{
	S_CompoundLVItemData* pclid;
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	CNLB_BldrView* pView = (CNLB_BldrView*) m_pParent;
	CNLB_BldrDoc* pDoc;

	if (pView && lpnmitem)
	{		
		pDoc = pView->GetDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			pclid = (S_CompoundLVItemData*) m_lstResults.GetItemData(lpnmitem->iItem);
			if (pclid)
			{
				if (pclid->cliType == CLI_Page)
				{
					pView->DeselectAll();
					pView->itSelItemType = SelItem_BookPage;
					pView->m_pLastSelectedBookItem = (CBookPage*) pclid->lpCLIObject;				
					pView->m_fIsDraggingStarted = FALSE;	// Чтобы не было проблем
					pView->m_fIsDraggingInProgress = FALSE;	// Чтобы не было проблем				
					((CBookPage*) pclid->lpCLIObject)->Select();
					pDoc->m_iSelectedPagesCount = 1;	// Теперь помечена только одна страница!
					pView->ScrollToPage((CBookPage*) pclid->lpCLIObject);
					pView->EditpageEditproperties();
				}
				else if (pclid->cliType == CLI_Link)
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
				else if (pclid->cliType == CLI_ProbVar)
				{
					CDialogProbVarProperties dlg;
					CProbVar* pProbVar = (CProbVar*) pclid->lpCLIObject;
				
					dlg.m_strProbVarName = pProbVar->Get_strProbVarName();
					dlg.m_iDiceK = pProbVar->Get_iDiceK();
					dlg.m_iDiceY = pProbVar->Get_iDiceY();
					if (dlg.DoModal() == IDOK)
					{
						pProbVar->Get_strProbVarName() = dlg.m_strProbVarName;
						pProbVar->Set_iDiceK(dlg.m_iDiceK);
						pProbVar->Set_iDiceY(dlg.m_iDiceY);							
					}
				}
			}		
		}
	}
	
	*pResult = 0;
}
