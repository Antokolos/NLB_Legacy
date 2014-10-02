// DialogLeafs.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "DialogLeafs.h"
#include "CBookPage.h"
#include "CLink.h"
#include "MainFrm.h"
#include "NLB_BldrDoc.h"
#include "NLB_BldrView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogLeafs dialog


CDialogLeafs::CDialogLeafs(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLeafs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogLeafs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iIconPos = -1;
}


void CDialogLeafs::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLeafs)
	DDX_Control(pDX, IDC_BUTTON_GOTO, m_btnGoTo);
	DDX_Control(pDX, IDC_LIST_LEAFS, m_lstLeafs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLeafs, CDialog)
	//{{AFX_MSG_MAP(CDialogLeafs)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LEAFS, OnItemchangedListLeafs)
	ON_BN_CLICKED(IDC_BUTTON_GOTO, OnButtonGoto)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LEAFS, OnDblclkListLeafs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogLeafs message handlers

BOOL CDialogLeafs::OnInitDialog() 
{
	BeginWaitCursor();
	POSITION posPage;
	POSITION posLnk;
	CBookPage* pBookPageCur;
	CLink* pLinkCur;
	BOOL fIsLeaf;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;
	int iPagesCount;
	int iLnksCount;

	CDialog::OnInitDialog();
	
	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_LEAF);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstLeafs.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_LEAF_NAME);		
	m_lstLeafs.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVLEAFS_COL0_WIDTH,
		0);	
	
	m_ImListLeafs.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListLeafs.Add(hIconNew);
	m_pImListLeafsOld = m_lstLeafs.SetImageList(&m_ImListLeafs, LVSIL_SMALL);	

	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{			
			iPagesCount = pDoc->m_lstPages.GetCount();
			iLnksCount = pDoc->m_lstLnks.GetCount();
			posPage = pDoc->m_lstPages.GetHeadPosition();			
			for (int i = 0; i < iPagesCount; i++)
			{
				fIsLeaf = TRUE;	// Будем оптимистами
				pBookPageCur = pDoc->m_lstPages.GetNext(posPage);
				
				posLnk = pDoc->m_lstLnks.GetHeadPosition();			
				for (int j = 0; j < iLnksCount; j++)
				{					
					pLinkCur = pDoc->m_lstLnks.GetNext(posLnk);
					if (pLinkCur->Get_pPageFrom() == pBookPageCur)
					{
						fIsLeaf = FALSE;
						break;
					}					
				}

				if (fIsLeaf)
				{
					m_lstLeafs.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
								i,
								pBookPageCur->Get_strPageCaption(),
								0,
								0,
								m_iIconPos,
								(LPARAM) pBookPageCur);						
				}
			}	
			
			m_btnGoTo.EnableWindow(FALSE);		
		}
	}
	
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogLeafs::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_lstLeafs.SetImageList(m_pImListLeafsOld, LVSIL_SMALL);		
	m_ImListLeafs.DeleteImageList();		
}

void CDialogLeafs::OnItemchangedListLeafs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
			m_btnGoTo.EnableWindow(TRUE);					
		else		
			m_btnGoTo.EnableWindow(FALSE);		
	}
	
	*pResult = 0;
}

void CDialogLeafs::OnButtonGoto() 
{	
	CBookPage* pPageToGo;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrView* pView;
	CNLB_BldrDoc* pDoc;
	POSITION SelPos;
	int nItem;
		
	if (pMainFrame)
	{
		pView = (CNLB_BldrView*) pMainFrame->GetActiveView();
		if (pView)
		{
			pDoc = pView->GetDocument();
			ASSERT_VALID(pDoc);
			if (pDoc)
			{
				SelPos = m_lstLeafs.GetFirstSelectedItemPosition();
				if (SelPos)
				{	
					nItem = m_lstLeafs.GetNextSelectedItem(SelPos);
					pPageToGo = (CBookPage*) m_lstLeafs.GetItemData(nItem);
					if (pPageToGo) 
					{
						pView->DeselectAll();
						pView->itSelItemType = SelItem_BookPage;
						pView->m_pLastSelectedBookItem = pPageToGo;				
						pView->m_fIsDraggingStarted = FALSE;	// Чтобы не было проблем
						pView->m_fIsDraggingInProgress = FALSE;	// Чтобы не было проблем						
						pPageToGo->Select();
						pDoc->m_iSelectedPagesCount = 1;	// Теперь помечена только одна страница!
						pView->ScrollToPage(pPageToGo);				
					}
				}
			}
		}
	}
}

void CDialogLeafs::OnDblclkListLeafs(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonGoto();	
	*pResult = 0;
}
