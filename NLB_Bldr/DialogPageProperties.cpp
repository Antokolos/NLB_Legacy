// DialogPageProperties.cpp : implementation file
//

#include "stdafx.h"
#include "NLB_Bldr.h"
#include "NLB_BldrDoc.h"
#include "NLB_BldrView.h"
#include "DialogPageProperties.h"
#include "funcs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPageProperties dialog


CDialogPageProperties::CDialogPageProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPageProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPageProperties)
	m_strPageCaption = _T("");
	m_strPageText = _T("");
	m_strPageVar = _T("");
	m_strPercComplete = _T("");
	m_fUsePageCaption = FALSE;
	//}}AFX_DATA_INIT
	POINT pt;

	pParentView = pParent;	// Он не должен быть NULL!!! И он не будет NULL!!!
	m_lBrush.lbColor = RGB(0, 0, 0);
	m_lBrush.lbStyle = BS_SOLID;
	m_lBrush.lbHatch = 0;	// If lbStyle is BS_SOLID or BS_HOLLOW, lbHatch is ignored. 
	m_lPen.lopnColor = RGB(0, 0, 0);
	pt.x = 1;
	pt.y = 0;
	m_lPen.lopnWidth = pt;
	m_lPen.lopnStyle = PS_INSIDEFRAME;
	m_txtColor = RGB(0, 0, 0);
	m_fAbortMassOperationFlag = TRUE;
	m_iPagesProcessed = 0;
	m_iPagesTotal = 1;
	m_fGoPrevPage = FALSE;
	m_iIconPos = -1;

	m_strInitialVarName = _T("");	// На самом деле изменение произойдет на OnInitDialog
}


void CDialogPageProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPageProperties)
	DDX_Control(pDX, IDC_STATIC_PERCENT_COMPLETE, m_stcPercComplete);
	DDX_Control(pDX, IDC_PROGRESS_MASS_OPERATION, m_ProgressMassOperation);
	DDX_Control(pDX, IDC_BUTTON_PREV_PAGE, m_btnPrevPage);
	DDX_Control(pDX, IDC_BUTTON_NEXT_PAGE, m_btnNextPage);
	DDX_Control(pDX, IDC_BUTTON_ABORT_MASS_OPERATION, m_btnAbortMassOperation);
	DDX_Control(pDX, IDC_BUTTON_MOVE_UP, m_btnMoveLinkUp);
	DDX_Control(pDX, IDC_BUTTON_MOVE_DOWN, m_btnMoveLinkDown);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_btnDeleteLink);
	DDX_Control(pDX, IDC_BUTTON_EDIT, m_btnEditLink);
	DDX_Control(pDX, IDC_LIST_LINK_NUMBERS, m_lstLinkNumbers);
	DDX_Control(pDX, IDC_EDIT_PAGE_CAPTION, m_edtPageCaption);
	DDX_Control(pDX, IDC_EDIT_PAGE_VAR, m_edtPageVar);
	DDX_Control(pDX, IDC_BUTT_TXT_CLR, m_btnTxtClr);
	DDX_Control(pDX, IDC_BUTT_PAGE_CLR, m_btnPageClr);
	DDX_Control(pDX, IDC_BUTT_BRDR_CLR, m_btnBrdrClr);
	DDX_Text(pDX, IDC_EDIT_PAGE_CAPTION, m_strPageCaption);
	DDX_Text(pDX, IDC_EDIT_PAGE_TEXT, m_strPageText);
	DDX_Text(pDX, IDC_EDIT_PAGE_VAR, m_strPageVar);
	DDX_Text(pDX, IDC_STATIC_PERCENT_COMPLETE, m_strPercComplete);
	DDX_Check(pDX, IDC_CHECK_USE_PAGE_CAPTION, m_fUsePageCaption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPageProperties, CDialog)
	//{{AFX_MSG_MAP(CDialogPageProperties)
	ON_BN_CLICKED(IDC_BUTT_PAGE_CLR, OnButtPageClr)
	ON_BN_CLICKED(IDC_BUTT_BRDR_CLR, OnButtBrdrClr)
	ON_BN_CLICKED(IDC_BUTT_TXT_CLR, OnButtTxtClr)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LINK_NUMBERS, OnItemchangedListLinkNumbers)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LINK_NUMBERS, OnDblclkListLinkNumbers)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_LINK_NUMBERS, OnGetdispinfoListLinkNumbers)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, OnButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, OnButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_REVERT_ORDER, OnButtonRevertOrder)
	ON_BN_CLICKED(IDC_BUTTON_ABORT_MASS_OPERATION, OnButtonAbortMassOperation)
	ON_BN_CLICKED(IDC_BUTTON_NEXT_PAGE, OnButtonNextPage)
	ON_BN_CLICKED(IDC_BUTTON_PREV_PAGE, OnButtonPrevPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPageProperties message handlers

void CDialogPageProperties::OnButtPageClr() 
{
	CColorDialog dlg;
	LOGBRUSH lbrush;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = m_lBrush.lbColor;
	m_btnPageClr.GetLogBrush(&lbrush);

	if (dlg.DoModal() == IDOK)
	{
		m_lBrush.lbColor = dlg.GetColor();		
		lbrush.lbColor = dlg.GetColor();		
		m_btnPageClr.SetLogBrush(&lbrush);
		m_btnPageClr.Invalidate();
	}	
}

void CDialogPageProperties::OnButtBrdrClr() 
{
	CColorDialog dlg;
	LOGBRUSH lbrush;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = m_lPen.lopnColor;
	m_btnBrdrClr.GetLogBrush(&lbrush);

	if (dlg.DoModal() == IDOK)
	{
		m_lPen.lopnColor = dlg.GetColor();
		lbrush.lbColor = dlg.GetColor();		
		m_btnBrdrClr.SetLogBrush(&lbrush);
		m_btnBrdrClr.Invalidate();
	}		
}

BOOL CDialogPageProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	LOGBRUSH lbrush_brdr;
	LOGBRUSH lbrush_txt;
	int iLnkCount;
	POSITION pos;	
	int i;
	int iMaxLnkNum;
	int iSupLnkNum;
	POSITION MaxLnkPos;
	BOOL fContinueIterating;	
	LVITEM lvI;
	CLink* pLinkCur;
	S_PLink_LCounter_And_LStrings* plcas_cur;
	int iPercentCompleteValue;

	CDialog::OnInitDialog();
	
	HICON hIconNew = AfxGetApp()->LoadIcon(IDR_LINK);	// Уничтожать иконку, созданную LoadIcon, вроде не надо.
	CString lpszColumnHeading;	// Имя колонки ListView

	//Установим стили для ListView...
	m_lstLinkNumbers.SetExtendedStyle(LVS_EX_LABELTIP | 
									   LVS_EX_FULLROWSELECT | 
									   LVS_EX_GRIDLINES);
	
	//Делаем колонки ListView...
	lpszColumnHeading.LoadString(IDS_UITXT_LV_LINKNUMBERS_LINK_CAPTION);		
	m_lstLinkNumbers.InsertColumn(0,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVLINKNUMBERS_COL0_WIDTH,
		0);

	lpszColumnHeading.LoadString(IDS_UITXT_LV_LINKNUMBERS_LINK_CONSTRAINTS);
	m_lstLinkNumbers.InsertColumn(1,
		(LPCTSTR) lpszColumnHeading,LVCFMT_LEFT,
		ILVLINKNUMBERS_COL1_WIDTH,
		0);
	
	m_ImListLinkNumbers.Create(GetSystemMetrics(SM_CXSMICON), 
								GetSystemMetrics(SM_CYSMICON), 
								ILC_COLORDDB, 
								1, 
								1);
	m_iIconPos = m_ImListLinkNumbers.Add(hIconNew);
	m_pImListLinkNumbersOld = m_lstLinkNumbers.SetImageList(&m_ImListLinkNumbers, LVSIL_SMALL);	

	iLnkCount = m_ListLinksFrom.GetCount();	
	iSupLnkNum = I_MAX_INT;	
	
	// Initialize LVITEM members that are common to all
	// items. 
	lvI.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM | LVIF_STATE; 
	lvI.state = 0; 
	lvI.stateMask = 0; 
	lvI.pszText = LPSTR_TEXTCALLBACK; // sends an
	                                  // LVN_GETDISPINFO
                                      // message.
	lvI.iImage = m_iIconPos;

	do
	{		
		int iLnkNumCur;

		fContinueIterating = FALSE;
		pos = m_ListLinksFrom.GetHeadPosition();
		iMaxLnkNum = -1;
		MaxLnkPos = NULL;
		for (i = 0; i < iLnkCount; i++)
		{		
			plcas_cur = m_ListLinksFrom.GetAt(pos);
			pLinkCur = plcas_cur->pLink;
			iLnkNumCur = pLinkCur->Get_iLinkNumber();
			if ((iLnkNumCur > iMaxLnkNum) && (iLnkNumCur < iSupLnkNum))
			{
				iMaxLnkNum = iLnkNumCur;
				MaxLnkPos = pos;
				fContinueIterating = TRUE;
			}
			m_ListLinksFrom.GetNext(pos);
		}
		if (fContinueIterating)
		{
			iSupLnkNum = iMaxLnkNum;
			
			plcas_cur = m_ListLinksFrom.GetAt(MaxLnkPos); 
			pLinkCur = plcas_cur->pLink;			
			lvI.iItem = 0;	
			lvI.iSubItem = 0;
			lvI.lParam = (LPARAM) plcas_cur;
			m_lstLinkNumbers.InsertItem(&lvI);	
			// Несмотря на то, что у нас два SubItem, инициализировать надо только один,
			// т.к. LPSTR_TEXTCALLBACK действует на весь item
		}
	}
	while (fContinueIterating);

	m_btnEditLink.EnableWindow(FALSE);
	m_btnDeleteLink.EnableWindow(FALSE);
	m_btnMoveLinkUp.EnableWindow(FALSE);
	m_btnMoveLinkDown.EnableWindow(FALSE);

	m_fGoPrevPage = FALSE;	// Такая вот инициализация. Думаем, что будем двигаться вперед :-)
	if (m_fAbortMassOperationFlag) 
	{
		m_btnAbortMassOperation.ShowWindow(SW_HIDE);
		m_btnNextPage.ShowWindow(SW_HIDE);
		m_btnPrevPage.ShowWindow(SW_HIDE);
		m_ProgressMassOperation.ShowWindow(SW_HIDE);
		m_stcPercComplete.ShowWindow(SW_HIDE);
	}
	else
	{		
		CString strDialogCaptForMassOp;
		// Надо подготовить прогрессбар
		m_ProgressMassOperation.SetRange32(0, m_iPagesTotal);
		m_ProgressMassOperation.SetPos(m_iPagesProcessed);

		// И кнопки
		if (m_iPagesProcessed <= 1)
			m_btnPrevPage.EnableWindow(FALSE);
		else
			m_btnPrevPage.EnableWindow(TRUE);

		if (m_iPagesProcessed >= m_iPagesTotal)
			m_btnNextPage.EnableWindow(FALSE);
		else
			m_btnNextPage.EnableWindow(TRUE);

		iPercentCompleteValue = (int) (((double) m_iPagesProcessed / (double) m_iPagesTotal) * 100);				
		m_strPercComplete.Format(IDS_STRING_PERCENT_COMPLETE_FORMAT_STRING, iPercentCompleteValue);
		UpdateData(FALSE);
		strDialogCaptForMassOp.Format(IDS_STRING_PAGE_PROPERTIES_DIALOG_CAPT_FOR_MASS_OPERATION_BUF, m_iPagesProcessed, m_iPagesTotal);
		SetWindowText(strDialogCaptForMassOp);
	}
		
	m_btnPageClr.SetLogBrush(&m_lBrush);	
	lbrush_txt = lbrush_brdr = m_lBrush;
	lbrush_brdr.lbColor = m_lPen.lopnColor;
	m_btnBrdrClr.SetLogBrush(&lbrush_brdr);
	lbrush_txt.lbColor = m_txtColor;
	m_btnTxtClr.SetLogBrush(&lbrush_txt);

	m_strInitialVarName = m_strPageVar;

	m_edtPageCaption.SetSel(0, -1);	// Выделить весь текст в Edit
	m_edtPageCaption.SetFocus();
	return FALSE;

	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogPageProperties::OnButtTxtClr() 
{
	CColorDialog dlg;
	LOGBRUSH lbrush;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = m_txtColor;
	m_btnTxtClr.GetLogBrush(&lbrush);

	if (dlg.DoModal() == IDOK)
	{
		m_txtColor = dlg.GetColor();
		lbrush.lbColor = dlg.GetColor();		
		m_btnTxtClr.SetLogBrush(&lbrush);
		m_btnTxtClr.Invalidate();
	}		
}

void CDialogPageProperties::OnOK() 
{		
	BOOL fOk = true;	// Будем оптимистами!
	BOOL fVarChanged;
	CNLB_BldrDoc* pDoc = ((CNLB_BldrView*) pParentView)->GetDocument();
	ASSERT_VALID(pDoc);		
	
	UpdateData(TRUE);
	fVarChanged = (m_strInitialVarName != m_strPageVar);

	if (m_strPageVar.GetLength() != 0)	// Если новая переменная - пустая строка, то ничего проверять не надо!
	{		
		CString strErrorMessage;
		CString strErrorCaption;

		if (((pDoc->FindPageIndexByVar(m_strPageVar) != -1) ||
			(pDoc->FindBookVarPositionByName(m_strPageVar)) ||
			(pDoc->FindProbVarPositionByName(m_strPageVar)) ||
			(pDoc->FindStrVarPositionByName(m_strPageVar)) ||
			(pDoc->FindTSVarPositionByName(m_strPageVar))) && (fVarChanged))
		{
			fOk = false;	
			strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
			strErrorMessage.Format(IDS_ERROR_VAR_ALREADY_EXISTS, m_strPageVar);
			MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
			m_edtPageVar.SetSel(0, -1);	// Выделить весь текст в Edit
			m_edtPageVar.SetFocus();
		}

		if (!CheckVarCorrect(m_strPageVar))
		{
			fOk = false;	
			strErrorCaption.LoadString(IDS_ERROR_MB_CAPT);
			strErrorMessage.Format(IDS_ERROR_VAR_INCORRECT, m_strPageVar);
			MessageBox(strErrorMessage, strErrorCaption, MB_ICONEXCLAMATION);
			m_edtPageVar.SetSel(0, -1);	// Выделить весь текст в Edit
			m_edtPageVar.SetFocus();
		}
		
		if (fOk) 
		{
			m_strPageVar.TrimLeft((WCHAR) ' ');	// Убрать пробелы слева от строки
			m_strPageVar.TrimRight((WCHAR) ' ');	// Убрать пробелы справа от строки
			UpdateData(FALSE);	// Сохранить подрезанную строку
			CDialog::OnOK();
		}
	}
	else
		CDialog::OnOK();
}

void CDialogPageProperties::OnDestroy() 
{
	CDialog::OnDestroy();
	m_lstLinkNumbers.SetImageList(m_pImListLinkNumbersOld, LVSIL_SMALL);		
	m_ImListLinkNumbers.DeleteImageList();	
}

void CDialogPageProperties::OnItemchangedListLinkNumbers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos;
	int nItem;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ((pNMListView->uChanged) & LVIF_STATE)
	{
		if ((pNMListView->uNewState ) & LVIS_SELECTED)
		{			
			pos = m_lstLinkNumbers.GetFirstSelectedItemPosition();
			if (pos)
			{	
				m_btnEditLink.EnableWindow(TRUE);
				m_btnDeleteLink.EnableWindow(TRUE);				

				nItem = m_lstLinkNumbers.GetNextSelectedItem(pos);		

				if (nItem < m_lstLinkNumbers.GetItemCount() - 1)
					m_btnMoveLinkDown.EnableWindow(TRUE);
				else
					m_btnMoveLinkDown.EnableWindow(FALSE);

				if (nItem > 0)
					m_btnMoveLinkUp.EnableWindow(TRUE);
				else
					m_btnMoveLinkUp.EnableWindow(FALSE);
			}	
			
		}
		else
		{
			m_btnEditLink.EnableWindow(FALSE);
			m_btnDeleteLink.EnableWindow(FALSE);
			m_btnMoveLinkUp.EnableWindow(FALSE);
			m_btnMoveLinkDown.EnableWindow(FALSE);
		}
	}		
	
	*pResult = 0;
}

void CDialogPageProperties::OnDblclkListLinkNumbers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnButtonEdit();	
	*pResult = 0;
}

void CDialogPageProperties::OnButtonDelete() 
{
	POSITION pos;
	int nItem;		
	CString strCaption;
	CString strMessage;
	CLink* plnkToDelete;
	S_PLink_LCounter_And_LStrings* plcas;

	strMessage.LoadString(IDS_CONFIRM_DELSEL_LINK);	
	strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
	if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{			
		pos = m_lstLinkNumbers.GetFirstSelectedItemPosition();
		if (pos)
		{	
			nItem = m_lstLinkNumbers.GetNextSelectedItem(pos);
			plcas = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(nItem);	
			plnkToDelete = plcas->pLink;
			RemoveLinkFromListLinksFrom(plnkToDelete);			
			m_lstLinkNumbers.DeleteItem(nItem);
			((CNLB_BldrView*) pParentView)->EditlinkDeletelink(plnkToDelete);
		}
	}		
}

BOOL CDialogPageProperties::RemoveLinkFromListLinksFrom(CLink* pLinkToDelete)
{
	POSITION pos;
	CLink* pLinkCur;
	int iLnkCount;
	S_PLink_LCounter_And_LStrings* plcas_cur;

	iLnkCount = m_ListLinksFrom.GetCount();
	pos = m_ListLinksFrom.GetHeadPosition();	
	for (int i = 0; i < iLnkCount; i++)
	{			
		plcas_cur = m_ListLinksFrom.GetAt(pos);		
		if (plcas_cur)
		{
			pLinkCur = plcas_cur->pLink;
			if (pLinkCur == pLinkToDelete)
			{
				if (plcas_cur->lpszLinkCaptionForLV) delete []plcas_cur->lpszLinkCaptionForLV;
				if (plcas_cur->lpszLinkConstraintsForLV) delete []plcas_cur->lpszLinkConstraintsForLV;
				delete plcas_cur;
				// pLink не удаляем 
				m_ListLinksFrom.RemoveAt(pos);
				return TRUE;
			}
		}
		m_ListLinksFrom.GetNext(pos);
	}

	return FALSE;
}

void CDialogPageProperties::OnButtonEdit() 
{	
	POSITION pos;
	int nItem;			
	CLink* plnkToEdit;
	S_PLink_LCounter_And_LStrings* plcas;
	
	pos = m_lstLinkNumbers.GetFirstSelectedItemPosition();
	if (pos)
	{	
		nItem = m_lstLinkNumbers.GetNextSelectedItem(pos);
		plcas = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(nItem);
		plnkToEdit = plcas->pLink;
		((CNLB_BldrView*) pParentView)->EditlinkEditproperties(plnkToEdit);
	}	
	m_lstLinkNumbers.Invalidate();	// Обновим...
}

void CDialogPageProperties::OnGetdispinfoListLinkNumbers(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CLink* pLinkCur;
	CString strLinkStrBuffer;
	S_PLink_LCounter_And_LStrings* plcas;
	int iLnkNumCur;		

	plcas = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(pDispInfo->item.iItem);
	pLinkCur = plcas->pLink;
	iLnkNumCur = pLinkCur->Get_iLinkNumber();
	strLinkStrBuffer.LoadString(IDS_STRING_LINK_CAPTION_BUFFER_FOR_LV_LINKNUMBERS);
	_stprintf(plcas->lpszLinkCaptionForLV, strLinkStrBuffer, iLnkNumCur, pLinkCur->Get_strLinkCaption());	
	strLinkStrBuffer.LoadString(IDS_STRING_LINK_CONSTRAINTS_BUFFER_FOR_LV_LINKNUMBERS);
	_stprintf(plcas->lpszLinkConstraintsForLV, strLinkStrBuffer, pLinkCur->Get_strLinkConstraints());	
	if (pDispInfo->item.iSubItem == 0)
	{
		pDispInfo->item.pszText = plcas->lpszLinkCaptionForLV;
	}
	else if (pDispInfo->item.iSubItem == 1)
	{
		pDispInfo->item.pszText = plcas->lpszLinkConstraintsForLV;
	}
	
	*pResult = 0;
}

void CDialogPageProperties::OnButtonMoveUp() 
{
	POSITION pos;
	int nItem;			
	CLink* plnkToMove;
	CLink* plnkToSwap;
	int iTemp;
	S_PLink_LCounter_And_LStrings* plcasToMove;
	S_PLink_LCounter_And_LStrings* plcasToSwap;
	
	pos = m_lstLinkNumbers.GetFirstSelectedItemPosition();
	if (pos)
	{	
		nItem = m_lstLinkNumbers.GetNextSelectedItem(pos);
		plcasToMove = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(nItem);			
		plnkToMove = plcasToMove->pLink;
		if (nItem > 0)
		{
			plcasToSwap = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(nItem - 1);
			plnkToSwap = plcasToSwap->pLink;
			iTemp = plnkToSwap->Get_iLinkNumber();
			plnkToSwap->Get_iLinkNumber() = plnkToMove->Get_iLinkNumber();
			plnkToMove->Get_iLinkNumber() = iTemp;
			m_lstLinkNumbers.SetItemData(nItem - 1, (DWORD) plcasToMove);
			m_lstLinkNumbers.SetItemData(nItem, (DWORD) plcasToSwap);
			m_lstLinkNumbers.SetItemState(nItem - 1, LVIS_SELECTED, LVIS_SELECTED);
			m_lstLinkNumbers.SetItemState(nItem, ~LVIS_SELECTED, LVIS_SELECTED);
		}
	}	
	m_lstLinkNumbers.Invalidate();	// Обновим...	
}

void CDialogPageProperties::OnButtonMoveDown() 
{
	POSITION pos;
	int nItem;			
	CLink* plnkToMove;
	CLink* plnkToSwap;
	int iTemp;
	S_PLink_LCounter_And_LStrings* plcasToMove;
	S_PLink_LCounter_And_LStrings* plcasToSwap;
	
	pos = m_lstLinkNumbers.GetFirstSelectedItemPosition();
	if (pos)
	{	
		nItem = m_lstLinkNumbers.GetNextSelectedItem(pos);
		plcasToMove = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(nItem);			
		plnkToMove = plcasToMove->pLink;
		if (nItem < m_lstLinkNumbers.GetItemCount() - 1)
		{
			plcasToSwap = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(nItem + 1);
			plnkToSwap = plcasToSwap->pLink;
			iTemp = plnkToSwap->Get_iLinkNumber();
			plnkToSwap->Get_iLinkNumber() = plnkToMove->Get_iLinkNumber();
			plnkToMove->Get_iLinkNumber() = iTemp;
			m_lstLinkNumbers.SetItemData(nItem + 1, (DWORD) plcasToMove);
			m_lstLinkNumbers.SetItemData(nItem, (DWORD) plcasToSwap);
			m_lstLinkNumbers.SetItemState(nItem + 1, LVIS_SELECTED, LVIS_SELECTED);
			m_lstLinkNumbers.SetItemState(nItem, ~LVIS_SELECTED, LVIS_SELECTED);
		}
	}	
	m_lstLinkNumbers.Invalidate();	// Обновим...	
}

void CDialogPageProperties::OnCancel() 
{
	S_PLink_LCounter_And_LStrings* plcas_cur;
	CLink* pLinkCur;
	POSITION pos;
	int i, iLnkCount;

	// TODO: Add extra cleanup here
	// Здесь надо вернуть на место старые значения для m_iLinkNumber

	pos = m_ListLinksFrom.GetHeadPosition();		
	iLnkCount = m_ListLinksFrom.GetCount();
	for (i = 0; i < iLnkCount; i++)
	{		
		plcas_cur = m_ListLinksFrom.GetNext(pos);
		pLinkCur = plcas_cur->pLink;
		pLinkCur->Get_iLinkNumber() = plcas_cur->iOldLinkNumber;		
	}
	
	CDialog::OnCancel();
}

void CDialogPageProperties::OnButtonRevertOrder() 
{	
	CLink* plnkToMove;
	CLink* plnkToSwap;
	int iTemp;
	S_PLink_LCounter_And_LStrings* plcasToMove;
	S_PLink_LCounter_And_LStrings* plcasToSwap;
	int iItemCount = m_lstLinkNumbers.GetItemCount();
	
	for (int i = 0; i < iItemCount/2; i++)
	{		
		plcasToMove = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(i);			
		plnkToMove = plcasToMove->pLink;		
		plcasToSwap = (S_PLink_LCounter_And_LStrings*) m_lstLinkNumbers.GetItemData(iItemCount - 1 - i);
		plnkToSwap = plcasToSwap->pLink;
		iTemp = plnkToSwap->Get_iLinkNumber();
		plnkToSwap->Get_iLinkNumber() = plnkToMove->Get_iLinkNumber();
		plnkToMove->Get_iLinkNumber() = iTemp;
		m_lstLinkNumbers.SetItemData(iItemCount - 1 - i, (DWORD) plcasToMove);
		m_lstLinkNumbers.SetItemData(i, (DWORD) plcasToSwap);					
	}
	m_lstLinkNumbers.Invalidate();	// Обновим...		
}

void CDialogPageProperties::OnButtonAbortMassOperation() 
{
	if (!m_fAbortMassOperationFlag) 
	{
		CString strCaption;
		CString strMessage;	

		strMessage.LoadString(IDS_CONFIRM_ABORT_MASS_OPERATION);	
		strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
		if (MessageBox(strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			m_fAbortMassOperationFlag = TRUE;
			OnCancel();
		}
	}
}

void CDialogPageProperties::OnButtonNextPage() 
{
	m_fGoPrevPage = FALSE;	// Впрочем, это уже было сделано на InitDialog, но ладно...
	OnOK();	
}

void CDialogPageProperties::OnButtonPrevPage() 
{
	m_fGoPrevPage = TRUE;
	OnOK();	
}
