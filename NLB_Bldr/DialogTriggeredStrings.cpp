// DialogTriggeredStrings.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "DialogTriggeredStrings.h"
#include "DialogTriggeredStringProperties.h"
#include "CTriggeredString.h"
#include "consts.h"
#include "NLB_BldrDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStrings dialog


CDialogTriggeredStrings::CDialogTriggeredStrings(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTriggeredStrings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTriggeredStrings)
	//}}AFX_DATA_INIT
	m_iIconPos = -1;
}


void CDialogTriggeredStrings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTriggeredStrings)
	DDX_Control(pDX, IDC_BUTTON_DELETE_TRIGGERED_STRING, m_btnDeleteTriggeredString);
	DDX_Control(pDX, IDC_BUTTON_EDIT_TRIGGERED_STRING, m_btnEditTriggeredString);
	DDX_Control(pDX, IDC_LIST_TRIGGERED_STRINGS, m_lstTriggeredStrings);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTriggeredStrings, CDialog)
	//{{AFX_MSG_MAP(CDialogTriggeredStrings)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ADD_TRIGGERED_STRINGS, OnButtonAddTriggeredStrings)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TRIGGERED_STRING, OnButtonDeleteTriggeredString)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_TRIGGERED_STRING, OnButtonEditTriggeredString)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRIGGERED_STRINGS, OnItemchangedListTriggeredStrings)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TRIGGERED_STRINGS, OnDblclkListTriggeredStrings)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_TRIGGERED_STRINGS, OnKeydownListTriggeredStrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTriggeredStrings message handlers

BOOL CDialogTriggeredStrings::OnInitDialog() 
{
	BeginWaitCursor();
	POSITION pos;
	CTriggeredString* pTriggeredStringCur;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	CNLB_BldrDoc* pDoc;
	int iTriggeredStringsCount;
	CString strTriggeredStringBodyToSetInLV;	

	CDialog::OnInitDialog();	

	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_TRIGSTR);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstTriggeredStrings.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_TRIGGERED_STRING_NAME);		
	m_lstTriggeredStrings.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVTRIGSTR_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_TRIGGERED_STRING_INFO);		
	m_lstTriggeredStrings.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVTRIGSTR_COL1_WIDTH,
		0);
	
	m_ImListTriggeredStrings.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListTriggeredStrings.Add(hIconNew);
	m_pImListTriggeredStringsOld = m_lstTriggeredStrings.SetImageList(&m_ImListTriggeredStrings, LVSIL_SMALL);	

	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{			
			iTriggeredStringsCount = pDoc->m_lstTriggeredStrings.GetCount();
			pos = pDoc->m_lstTriggeredStrings.GetHeadPosition();			
			for (int i = 0; i < iTriggeredStringsCount; i++)
			{
				pTriggeredStringCur = pDoc->m_lstTriggeredStrings.GetAt(pos);
				m_lstTriggeredStrings.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
						i,
						pTriggeredStringCur->Get_strTSName(),
						0,
						0,
						m_iIconPos,
						(LPARAM) pTriggeredStringCur);		

				GenerateTSBodyToSetInLV(pTriggeredStringCur, strTriggeredStringBodyToSetInLV);
				m_lstTriggeredStrings.SetItemText(i, 1, strTriggeredStringBodyToSetInLV);
				pDoc->m_lstTriggeredStrings.GetNext(pos);
			}			
		}
	}

	m_btnEditTriggeredString.EnableWindow(FALSE);
	m_btnDeleteTriggeredString.EnableWindow(FALSE);
	
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogTriggeredStrings::OnDestroy() 
{
	CDialog::OnDestroy();
	m_lstTriggeredStrings.SetImageList(m_pImListTriggeredStringsOld, LVSIL_SMALL);		
	m_ImListTriggeredStrings.DeleteImageList();	
}

void CDialogTriggeredStrings::OnButtonAddTriggeredStrings() 
{
	CTriggeredString* pTriggeredStringNew;
	CDialogTriggeredStringProperties dlg;	
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
	int ciItems;				
	CString strTriggeredStringBodyToSetInLV;	
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{	
			if ((pTriggeredStringNew = new CTriggeredString) == NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
			dlg.pTSCur = pTriggeredStringNew;
			dlg.m_strTSVarName = _T("");	// Новая Triggered String
			if (dlg.DoModal() == IDOK)
			{				
				pTriggeredStringNew->Get_strTSName() = dlg.m_strTSVarName;				
				//CTriggeredString::ListAssign(pTriggeredStringNew->m_lstStrVarParameters, dlg.m_lstStrVarParameters);				

				pDoc->m_lstTriggeredStrings.AddHead(pTriggeredStringNew);
				ciItems = m_lstTriggeredStrings.GetItemCount();	
				m_lstTriggeredStrings.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
					ciItems,
					pTriggeredStringNew->Get_strTSName(),
					0,
					0,
					m_iIconPos,
					(LPARAM) pTriggeredStringNew);	

				GenerateTSBodyToSetInLV(pTriggeredStringNew, strTriggeredStringBodyToSetInLV);				
				m_lstTriggeredStrings.SetItemText(ciItems, 1, strTriggeredStringBodyToSetInLV);	
			}
			else
			{
				if (pTriggeredStringNew) delete pTriggeredStringNew;
			}
		}
	}	
}

VOID CDialogTriggeredStrings::GenerateTSBodyToSetInLV(CTriggeredString* pTS, CString& strResult)
{
	CString strCurText;
	CString strSeparator;
	POSITION posCP;
	S_TS_ConditionPair* ptscp;

	strSeparator.LoadString(IDS_TRIGGERED_STRING_BODY_SEPARATOR);
	posCP = pTS->GetConditionPairsHeadPosition();
	strResult.Empty();
	while (posCP)
	{
		ptscp = pTS->GetNextConditionPair(posCP);
		
		if (ptscp->pStrVar->m_lstStrVarDependencies.GetCount() == 0)
			strCurText.Format(IDS_TRIGGERED_STRING_BODYPART_BUFFER, ptscp->pBVCondition->Get_strBookVarName(), ptscp->pStrVar->Get_strStrVarName());
		else
			strCurText.Format(IDS_TRIGGERED_STRING_BODYPART_BUFFER_W_DEPENDENT_STRVAR, ptscp->pBVCondition->Get_strBookVarName(), ptscp->pStrVar->Get_strStrVarName());

		strResult += strCurText;
		if (posCP) strResult += strSeparator;
	}
	if (pTS->GetDefaultValue())
	{
		strResult += strSeparator;
		if (pTS->GetDefaultValue()->m_lstStrVarDependencies.GetCount() == 0)
			strCurText.Format(IDS_TRIGGERED_STRING_BODYPART_DEFAULT_BUFFER, pTS->GetDefaultValue()->Get_strStrVarName());
		else
			strCurText.Format(IDS_TRIGGERED_STRING_BODYPART_DEFAULT_BUFFER_W_DEPENDENT_STRVAR, pTS->GetDefaultValue()->Get_strStrVarName());
		strResult += strCurText;
	}
}

void CDialogTriggeredStrings::OnButtonDeleteTriggeredString() 
{
	CTriggeredString* pTSToDelete;	
	CTriggeredString* pTSVarCur;	
	CNLB_BldrDoc* pDoc;
	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();	
	POSITION pos;
	int nItem;
	int iTSVarCount;										
						
	if (pMainFrame)
	{
		pDoc = (CNLB_BldrDoc*) pMainFrame->GetActiveDocument();
		ASSERT_VALID(pDoc);
		if (pDoc)
		{
			CString strCaption;
			CString strMessage;
			strMessage.LoadString(IDS_CONFIRM_DEL_TRIGGERED_STRING);	
			strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
			if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
			{				
				pos = m_lstTriggeredStrings.GetFirstSelectedItemPosition();
				if (pos)
				{	
					nItem = m_lstTriggeredStrings.GetNextSelectedItem(pos);
					pTSToDelete = (CTriggeredString*) m_lstTriggeredStrings.GetItemData(nItem);				
					iTSVarCount = pDoc->m_lstTriggeredStrings.GetCount();
					pos = pDoc->m_lstTriggeredStrings.GetHeadPosition();			
					for (int i = 0; i < iTSVarCount; i++)
					{
						pTSVarCur = pDoc->m_lstTriggeredStrings.GetAt(pos);
						if (pTSVarCur == pTSToDelete)
						{
							delete pTSToDelete;
							pDoc->m_lstTriggeredStrings.RemoveAt(pos);
							m_lstTriggeredStrings.DeleteItem(nItem);
							break;
						}
						pDoc->m_lstTriggeredStrings.GetNext(pos);
					}
				}	
			}
		}
	}
}

void CDialogTriggeredStrings::OnButtonEditTriggeredString() 
{
	CDialogTriggeredStringProperties dlg;
	CTriggeredString* pTSToEdit;
	CTriggeredString* pTSToEditCpy;
	POSITION SelPos;
	int nItem;
	CString strTriggeredStringBodyToSetInLV;

	SelPos = m_lstTriggeredStrings.GetFirstSelectedItemPosition();
	if (SelPos)
	{	
		nItem = m_lstTriggeredStrings.GetNextSelectedItem(SelPos);
		pTSToEdit = (CTriggeredString*) m_lstTriggeredStrings.GetItemData(nItem);
		if (pTSToEdit)
		{
			pTSToEditCpy = new CTriggeredString(*pTSToEdit);
			if (pTSToEditCpy)	// Будет изменяться не сама строчка, а ее копия, это позволит разобраться со списком, который не надо изменять при нажатии Cancel
			{
				dlg.pTSCur = pTSToEditCpy;
				dlg.m_strTSVarName = pTSToEditCpy->Get_strTSName();					
				if (dlg.DoModal() == IDOK)
				{
					pTSToEditCpy->Get_strTSName() = dlg.m_strTSVarName;
					*pTSToEdit = *pTSToEditCpy;					
					m_lstTriggeredStrings.SetItemText(nItem, 0, pTSToEdit->Get_strTSName());
					
					GenerateTSBodyToSetInLV(pTSToEdit, strTriggeredStringBodyToSetInLV);
					m_lstTriggeredStrings.SetItemText(nItem, 1, strTriggeredStringBodyToSetInLV);	
				}

				delete pTSToEditCpy;
			}
		}
	}
}

void CDialogTriggeredStrings::OnItemchangedListTriggeredStrings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
		{
			m_btnEditTriggeredString.EnableWindow(TRUE);
			m_btnDeleteTriggeredString.EnableWindow(TRUE);
		}
		else
		{
			m_btnEditTriggeredString.EnableWindow(FALSE);
			m_btnDeleteTriggeredString.EnableWindow(FALSE);
		}
	}
		
	*pResult = 0;
}

void CDialogTriggeredStrings::OnDblclkListTriggeredStrings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEditTriggeredString();
	
	*pResult = 0;
}

void CDialogTriggeredStrings::OnKeydownListTriggeredStrings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

	if (pLVKeyDow->wVKey == VK_SPACE)
		OnButtonEditTriggeredString();
	else if (pLVKeyDow->wVKey == VK_DELETE)
		OnButtonDeleteTriggeredString();
	
	*pResult = 0;
}
