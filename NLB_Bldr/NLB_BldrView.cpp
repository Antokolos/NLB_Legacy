// NLB_BldrView.cpp : implementation of the CNLB_BldrView class
//

#include "stdafx.h"
#include <afxtempl.h>
#include "NLB_Bldr.h"

#include "NLB_BldrDoc.h"
#include "NLB_BldrView.h"
#include "DialogPageProperties.h"
#include "DialogLinkProperties.h"
#include "DialogFindTxt.h"
#include "DialogComperr.h"
#include "MathTrans\CMathTrans.h"
#include "consts.h"
#include "DialogBookVars.h"
#include "DialogStrVars.h"
#include "DialogTriggeredStrings.h"
#include "DialogProbability.h"
#include "funcs.h"
#include "DialogLeafs.h"
#include <afxadv.h>	// CSharedFile

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrView

IMPLEMENT_DYNCREATE(CNLB_BldrView, CView)

BEGIN_MESSAGE_MAP(CNLB_BldrView, CView)
	//{{AFX_MSG_MAP(CNLB_BldrView)
	ON_COMMAND(ID_EDIT_ADDBOOKPAGE, OnEditAddbookpage)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_EDIT_ADDLINK, OnEditAddlink)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_ADDSTARTPOINT, OnEditAddstartpoint)
	ON_COMMAND(ID_EDIT_EDITPAGE_DELETEPAGE, OnEditEditpageDeletepage)
	ON_COMMAND(ID_EDIT_EDITPAGE_EDITPROPERTIES, OnEditEditpageEditproperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITPAGE_DELETEPAGE, OnUpdateEditEditpageDeletepage)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITPAGE_EDITPROPERTIES, OnUpdateEditEditpageEditproperties)
	ON_COMMAND(ID_EDIT_EDITLINK_DELETELINK, OnEditEditlinkDeletelink)
	ON_COMMAND(ID_EDIT_EDITLINK_EDITPROPERTIES, OnEditEditlinkEditproperties)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITLINK_DELETELINK, OnUpdateEditEditlinkDeletelink)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EDITLINK_EDITPROPERTIES, OnUpdateEditEditlinkEditproperties)
	ON_COMMAND(ID_FILE_EXPORTTO_NLBENGINEV10, OnFileExportTo_NLB_Engine10)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SYSCOLORCHANGE()
	ON_MESSAGE(WM_DISPLAYCHANGE, OnDisplayChange)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_BUILD_COMPILE, OnBuildCompile)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_BOOKVARIABLES, OnEditBookvariables)
	ON_COMMAND(ID_EDIT_LEAFS, OnEditLeafs)
	ON_COMMAND(ID_EDIT_PROBABILITYVARIABLES, OnEditProbabilityvariables)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDSTARTPOINT, OnUpdateEditAddstartpoint)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LEAFS, OnUpdateEditLeafs)
	ON_UPDATE_COMMAND_UI(ID_BUILD_COMPILE, OnUpdateBuildCompile)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTO_NLBENGINEV10, OnUpdateFileExporttoNlbenginev10)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDLINK, OnUpdateEditAddlink)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_CHANGEALLPAGES, OnEditChangeallpages)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CHANGEALLPAGES, OnUpdateEditChangeallpages)
	ON_COMMAND(ID_EDIT_STRINGVARIABLES, OnEditStringvariables)
	ON_COMMAND(ID_EDIT_TRIGGEREDSTRINGS, OnEditTriggeredstrings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrView construction/destruction

CNLB_BldrView::CNLB_BldrView()
{
	// m_BmpOffscreen, m_DCOffscreen, 
	// m_BrushOffscreen, m_pBmpOld, 
	// m_PenOffscreen, m_pPenOld, m_pBrushOld - ������������� �� 
	// OnCreate, ���������� �� OnDestroy

	m_iClientWidth = m_iClientHeight = m_iVertRes = m_iHorzRes = 0;	
	m_pPageFrom = NULL;
	m_pPageTo = NULL;	
	m_fIsDraggingStarted = FALSE;
	m_fIsDraggingInProgress = FALSE;
	m_fIsResizingNow = FALSE;
	m_iOffsDragX = m_iOffsDragY = 0;
	m_pLastSelectedBookItem = NULL;
	itSelItemType = SelItem_None;
	m_fMouseDownNow = FALSE;
	tb_Add_Option = OptAdd_None;
	hCurAddPage = AfxGetApp()->LoadCursor(IDC_POINTER_ADD_PAGE);
	hCurAddLink = AfxGetApp()->LoadCursor(IDC_POINTER_ADD_LINK);	
	memset(&m_siVert, 0, sizeof(m_siVert));
	m_siVert.cbSize = sizeof(m_siVert);
	m_siVert.fMask = SIF_ALL;
	memset(&m_siHorz, 0, sizeof(m_siHorz));
	m_siHorz.cbSize = sizeof(m_siHorz);
	m_siHorz.fMask = SIF_ALL;
	m_ixOffset = m_iyOffset = 0;
	m_strMessage = _TEXT("");

	if ((m_pDlgComperr = new CDialogComperr(this)) == NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);	
	m_fIsDialogComperrActivated = FALSE;	
	if ((m_pDlgSearchRes = new CDialogSearchResults(this)) == NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);	
	m_fIsDialogSearchResActivated = FALSE;
	m_fptBeginDragSet = FALSE;
	m_fptEndDragSet = FALSE;
	m_fMLeaved = TRUE;	// ����� ��� ������ �� ������� �� ����� ������ ����� ����������� ��������� WM_MOUSELEAVE
}

CNLB_BldrView::~CNLB_BldrView()
{	
	if (hCurAddPage) DestroyCursor(hCurAddPage);
	if (hCurAddLink) DestroyCursor(hCurAddLink);	
	if (m_pDlgComperr) delete m_pDlgComperr;	
	if (m_pDlgSearchRes) delete m_pDlgSearchRes;
}

BOOL CNLB_BldrView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs	
	cs.style |= WS_HSCROLL | WS_VSCROLL;

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrView drawing

void CNLB_BldrView::OnDraw(CDC* pDC)
{				
	if (m_fIsDraggingInProgress || m_fIsResizingNow) 
	{
		m_DCOffscreen_ForDrag.BitBlt(0, 0, m_iClientWidth, m_iClientHeight, &m_DCOffscreen, 0, 0, SRCCOPY);				
		DrawDragged(&m_DCOffscreen_ForDrag);
		pDC->BitBlt(0, 0, m_iClientWidth, m_iClientHeight, &m_DCOffscreen_ForDrag, 0, 0, SRCCOPY);				
	}
	else	
		pDC->BitBlt(0, 0, m_iClientWidth, m_iClientHeight, &m_DCOffscreen, 0, 0, SRCCOPY);			
}

BOOL CNLB_BldrView::DrawDragged(CDC* pDC, BOOL fDraw /*= TRUE*/)
{
	POSITION pos;
	CLink* pCurLnk;
	CBookPage* pCurPage;
	int iLnksCount;
	int iPagesCount;
	DWORD dwStyle;
	int i;	
	RECT RctClient;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDC) 
	{			
		RctClient.left = 0;
		RctClient.top = 0;
		RctClient.right = m_iClientWidth;
		RctClient.bottom = m_iClientHeight;

		if (m_fIsDraggingInProgress)
		{
			pos = pDoc->m_lstPages.GetHeadPosition();			
			iPagesCount = pDoc->m_lstPages.GetCount();
			for (i=0; i<iPagesCount; i++)
			{		
				dwStyle = PARAM_DRAW;
				pCurPage = pDoc->m_lstPages.GetAt(pos);
				
				if (pCurPage->IsSelected())					
				{
					pCurPage->Get_iLeft() += m_iOffsDragX;
					pCurPage->Get_iTop() += m_iOffsDragY;					
					if (pCurPage->IsIntersect(RctClient))	// ������ ������ �����, ����� �������� �������� � ������� ������� ������
					{
						pCurPage->NeedToEraseWhenNotVisible() = TRUE;	// ���� ��������, �������� �� ����, ����� ������� �� ��������� ����������.
						// ���� �� �� ��������, �� ��� �������� ���� ���������, � � ��������� ��� �� ����� ���� ��������, ��� �������� ������.
						if (fDraw) 
						{					
							dwStyle |= PARAM_DRAW_FOCUS;
							if (pDoc->m_pStartPoint == pCurPage) dwStyle |= PARAM_IS_STARTPOINT;	
							pCurPage->Draw(pDC, dwStyle);
						}
						else
							pCurPage->Draw(pDC, PARAM_ERASE);				
					}						
				}				
			
				pDoc->m_lstPages.GetNext(pos);
			}		

			// ������ ���� � ����� ��������
			pos = pDoc->m_lstLnks.GetHeadPosition();			
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				dwStyle = PARAM_DRAW;
				pCurLnk = pDoc->m_lstLnks.GetAt(pos);

				// ������ ������ ������ ����, �������, � ���� ��, ������ �� ������.
				// ����, �� ���������� � ������� ���� ������, ����������.
				if ((pCurLnk->Get_pPageFrom()->IsSelected()) ||
					(pCurLnk->Get_pPageTo()->IsSelected()))					
				{		
					if (pCurLnk->IsIntersect(RctClient))
					{
						pCurLnk->NeedToEraseWhenNotVisible() = TRUE;	// ���� ��������, �������� �� ����, ����� ������� �� ��������� ����������.
						// ���� �� �� ��������, �� ��� �������� ���� ���������, � � ��������� ��� �� ����� ���� ��������, ��� �������� ������.
						if (fDraw) 
						{
							if (pCurLnk->IsSelected()) dwStyle |= PARAM_DRAW_FOCUS;	
							pCurLnk->Draw(pDC, dwStyle);
						}
						else
							pCurLnk->Draw(pDC, PARAM_ERASE);
					}					
				}

				pDoc->m_lstLnks.GetNext(pos);
			}			
		}
		else if (m_fIsResizingNow)
		{
			pCurPage = (CBookPage*) m_pLastSelectedBookItem;

			// ������ ���� � ����� ��������
			pos = pDoc->m_lstLnks.GetHeadPosition();			
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				dwStyle = PARAM_DRAW;
				pCurLnk = pDoc->m_lstLnks.GetAt(pos);

				// ������ ������ ������ ����, �������, � ���� ��, ������ �� ������.
				// ����, �� ���������� � ������� ���� ������, ����������.
				if ((pCurLnk->Get_pPageFrom() == pCurPage) ||
					(pCurLnk->Get_pPageTo() == pCurPage))
				{	
					if (pCurLnk->IsIntersect(RctClient))
					{
						pCurLnk->NeedToEraseWhenNotVisible() = TRUE;	// ���� ��������, �������� �� ����, ����� ������� �� ��������� ����������.
						// ���� �� �� ��������, �� ��� �������� ���� ���������, � � ��������� ��� �� ����� ���� ��������, ��� �������� ������.
						if (fDraw) 
						{
							if (pCurLnk->IsSelected()) dwStyle |= PARAM_DRAW_FOCUS;	
							pCurLnk->Draw(pDC, dwStyle);
						}
						else
							pCurLnk->Draw(pDC, PARAM_ERASE);
					}
				}

				pDoc->m_lstLnks.GetNext(pos);
			}
			
			if (pCurPage->IsIntersect(RctClient))	// ������ ������ �����, ����� �������� � ������� �������
			{
				pCurPage->NeedToEraseWhenNotVisible() = TRUE;	// ���� ��������, �������� �� ����, ����� ������� �� ��������� ����������.
				// ���� �� �� ��������, �� ��� �������� ���� ���������, � � ��������� ��� �� ����� ���� ��������, ��� �������� ������.
				if (fDraw) 
				{
					dwStyle = PARAM_DRAW | PARAM_DRAW_FOCUS;
					if (pDoc->m_pStartPoint == pCurPage) dwStyle |= PARAM_IS_STARTPOINT;	
					pCurPage->Draw(pDC, dwStyle);
				}
				else
					pCurPage->Draw(pDC, PARAM_ERASE);
			}
		}	
		else return FALSE;

		return TRUE;
	}
	else return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrView diagnostics

#ifdef _DEBUG
void CNLB_BldrView::AssertValid() const
{
	CView::AssertValid();
}

void CNLB_BldrView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNLB_BldrDoc* CNLB_BldrView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNLB_BldrDoc)));
	return (CNLB_BldrDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNLB_BldrView message handlers

void CNLB_BldrView::OnEditAddbookpage() 
{		
	tb_Add_Option = OptAdd_BookPage;	
}

BOOL CNLB_BldrView::InitializeTmeForLeave()
{
	TRACKMOUSEEVENT tme;

	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = GetSafeHwnd();
	tme.dwHoverTime = HOVER_DEFAULT;
	return _TrackMouseEvent(&tme);	
}

LRESULT CNLB_BldrView::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	if (m_fMouseDownNow)
	{
		CPoint ptZero;
		ptZero.x = ptZero.y = 0;
		OnLButtonUp(0, ptZero);		// ��� ����� ���� �������� ����� ������...
	}
	m_fMLeaved = TRUE;
	return 0;
}

void CNLB_BldrView::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CBookPage* pPageCur;
	CLink* pLinkCur;
	m_pLastSelectedBookItem = NULL;	// ����� ������������
	m_fIsDraggingStarted = FALSE;
	m_fIsDraggingInProgress = FALSE;
	itSelItemType = SelItem_None;
	int iPagesCount;
	int iLnksCount;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	
	
	if (m_fMLeaved) 
	{
		InitializeTmeForLeave();	// ����� ����� �� ����� ����������� �������� ����������� WM_MOUSELEAVE
		m_fMLeaved = FALSE;
	}

	m_fptBeginDragSet = FALSE;	// ����� ������������

	m_fMouseDownNow = TRUE;
	if (tb_Add_Option == OptAdd_BookPage)
	{
		CBookPage* pPage;
		CString str;
		str.LoadString(IDS_STR_DEFAULT_PAGE_CAPT);
		CBookPage page(str, point.y, point.x);
		if ((pPage = new CBookPage(page))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
		pDoc->m_lstPages.AddHead(pPage);
		pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
		PaintInMemory();
		
		tb_Add_Option = OptAdd_None;		
	}
	else if (tb_Add_Option == OptAdd_Link)
	{
		CBookPage* pPage = NULL;
		iPagesCount = pDoc->m_lstPages.GetCount();
		
		POSITION pos = pDoc->m_lstPages.GetHeadPosition();
		for (int i=0; i<iPagesCount; i++)
		{		
			pPageCur = pDoc->m_lstPages.GetAt(pos);
			if (pPageCur->IsPointInside(point))
			{
				pPage = pDoc->m_lstPages.GetAt(pos);							
				break;
			}
			pDoc->m_lstPages.GetNext(pos);
		}

		if (m_pPageFrom == NULL) m_pPageFrom=pPage;
		else	//m_pPageFrom != NULL
		{
			if (pPage)
			{
				if (pPage!=m_pPageFrom)	// ������ �������� � ����������� ���� �� ����� � ��� �� ��������!
				{
					CLink* pLinkToAdd;
					CString strDefaultLinkCapt;
					int iLinkNumber = 0;

					strDefaultLinkCapt.LoadString(IDS_STR_DEFAULT_LINK_CAPT);					
					m_pPageTo = pPage;

					// ���������� ���������� ����� ���� ������ - ��� ����� ��������� �� ������� ����� ���� ������������
					POSITION pos = pDoc->m_lstLnks.GetHeadPosition();
					iLnksCount = pDoc->m_lstLnks.GetCount();
					for (int i=0; i<iLnksCount; i++)
					{		
						pLinkCur = pDoc->m_lstLnks.GetAt(pos);
						
						if (pLinkCur->Get_pPageFrom() == m_pPageFrom)
						{
							if (pLinkCur->Get_iLinkNumber() >= iLinkNumber)
								iLinkNumber = pLinkCur->Get_iLinkNumber() + 1;
						}
						
						pDoc->m_lstLnks.GetNext(pos);
					}

					CLink lnk(m_pPageFrom, m_pPageTo, strDefaultLinkCapt, iLinkNumber);
					if (!pDoc->IsExistLink(lnk)) // ���� ����� ��� ���...
					{
						if ((pLinkToAdd = new CLink(lnk))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
						if (pLinkToAdd->Get_pPageFrom()->IsSelected() && 
							pLinkToAdd->Get_pPageTo()->IsSelected())
							pLinkToAdd->Select();	// ...�� ������� ��, ���� ��� ����...
						pDoc->m_lstLnks.AddHead(pLinkToAdd);	// ...� �������
						pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
						PaintInMemory();
					}
					else
					{
						m_strMessage.LoadString(IDS_ALREADY_EXIST);
						MessageBox(m_strMessage);
					}

					m_pPageFrom = m_pPageTo = NULL;
					tb_Add_Option = OptAdd_None;
				}
				else
				{
					m_strMessage.LoadString(IDS_LOOPS_CANNOT_EXIST);
					MessageBox(m_strMessage);
				}
			}
		}		
	}
	else	// ����� ����������� �������������� �������
	{
		// m_pLastSelectedBookItem = NULL; - ��� ������� ��� ���� ����!
		// ������� ������� �������� ������� ��� ��������
		POSITION pos = pDoc->m_lstPages.GetHeadPosition();
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (int i=0; i<iPagesCount; i++)
		{		
			pPageCur = pDoc->m_lstPages.GetAt(pos);
			if (pPageCur->IsPointInside(point))
			{
				m_pLastSelectedBookItem = pPageCur;
				m_fIsDraggingStarted = TRUE;
				m_iOffsDragX = 0;	// ������� ������ ������...
				m_iOffsDragY = 0;	// ������� ������ ������...
				itSelItemType = SelItem_BookPage;

				if (!pPageCur->IsSelected())	// ��� �� ���� �������, ������, ��� ���������� ���������, � � ��������� ��������� ���������
				{
					DeselectAll();
					pPageCur->Select();	// ������ ��� �������	
					pDoc->m_iSelectedPagesCount = 1;	// ������ �������� ������ ���� ��������!
				}

				ptStartDrag = point;				
				break;
			}
			pDoc->m_lstPages.GetNext(pos);
		}

		if (!m_pLastSelectedBookItem)
		{
			// ���� �� �����, ������� �������� ������� ��� ������
			POSITION pos = pDoc->m_lstLnks.GetHeadPosition();
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				pLinkCur = pDoc->m_lstLnks.GetAt(pos);
				if (pLinkCur->IsPointInside(point))
				{
					if (!(pLinkCur->Get_pPageFrom()->IsSelected() && 
							pLinkCur->Get_pPageTo()->IsSelected()))
							DeselectAll();	// ��� �������� ������, �� ������� � ���������, �� ���� ����� ��������� �� ���� ���������.					
					m_pLastSelectedBookItem = (CDrawable*) pLinkCur;
					pLinkCur->Select();
					itSelItemType = SelItem_Link;								
					break;
				}
				pDoc->m_lstLnks.GetNext(pos);
			}
		}		

		if (!m_pLastSelectedBookItem)
		{
			// ���� � ������ �� �����, �� ������, ���� �������� ������������� ���������
			m_ptBeginDragSelBox = point;	// ����� ����� ���������� ��������� �������������� ���������.
			m_fptBeginDragSet = TRUE;			
			DeselectAll();	// ��� ������ ����� ���������, ������, �� ������ ���� ������
		}

		PaintInMemory();
	}	

	CView::OnLButtonDown(nFlags, point);
}

void CNLB_BldrView::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	m_fMouseDownNow = FALSE;	
	if (m_fIsDraggingStarted || m_fIsDraggingInProgress) 
	{		
		DrawDragged(&m_DCOffscreen);	// ��� ��� ���� �� �������� ���������� ��������� ��������������� �������� ������. ���� ����������.
		// ������ ������ ���������� �� ����	
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;				
	}

	if (m_fIsResizingNow)
	{
		DrawDragged(&m_DCOffscreen);	// ��� ��� ���� �� �������� ���������� ��������� ��������, ������ ������� ������, ������. ���� ����������.
		m_fIsResizingNow = FALSE; 		
	}
	
	if (m_fptBeginDragSet && m_fptEndDragSet)
	{
		CBookPage* pPageCur;
		CLink* pCurLnk;
		CRect rct;
		POSITION pos;
		int iPagesCount;
		int iLnksCount;
		int i;	// ��� ����� for
		CNLB_BldrDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);	

		CountSelRctCoords(rct);		

		// ������� ���������� ��������
		pos = pDoc->m_lstPages.GetHeadPosition();	
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (i=0; i<iPagesCount; i++)
		{			
			pPageCur = pDoc->m_lstPages.GetAt(pos);
			if (pPageCur->IsIntersect(rct))
			{
				if (!(pPageCur->IsSelected())) pDoc->m_iSelectedPagesCount++;	// ���� ������������� �������� ����� ��������, �������� �������
				pPageCur->Select();
			}
			else
			{
				if (pPageCur->IsSelected()) pDoc->m_iSelectedPagesCount--;	// ���� ��������, � ������� ������� �������, ������������� ���� ��������, �������� �������.
				pPageCur->Deselect();
			}
			pDoc->m_lstPages.GetNext(pos);
		}	

		// ������� ���������� ����
		pos = pDoc->m_lstLnks.GetHeadPosition();		
		iLnksCount = pDoc->m_lstLnks.GetCount();
		for (i=0; i<iLnksCount; i++)
		{		
			pCurLnk = pDoc->m_lstLnks.GetAt(pos);
			if (pCurLnk->IsBothPagesIntersectRect(rct))
				pCurLnk->Select();
			else
				pCurLnk->Deselect();
			pDoc->m_lstLnks.GetNext(pos);
		}

		PaintInMemory();	// ����� ������� ������ ��������� - ��� �������� ����� ��������� ������
		m_fptBeginDragSet = m_fptEndDragSet = FALSE;
		PaintInMemory();	// ����� ���������� ��������� ��������� (���������� ��������)
	}
	m_fptBeginDragSet = m_fptEndDragSet = FALSE;	// ��� ���� ������� ���� � ��� ������, ����� ������� ���� �� ���������.
	UpdateSB();	// ������ �����, ���� �������� ����������, ��� �����������, ������, ����������...

	SetWndCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		// ����� ���� ������� ������ �� ����������� ���������, ��� ��� 
		// ����� ����� ��������� (�� ����� ���� ������, � �������� ������
		// ����� �������� ������)

	CView::OnLButtonUp(nFlags, point);
}

void CNLB_BldrView::OnMouseMove(UINT nFlags, CPoint point) 
{
	HCURSOR cur = NULL;	
	static BOOL fIsResizingWX = FALSE;	
	static BOOL fIsResizingEX = FALSE;	
	static BOOL fIsResizingNY = FALSE;
	static BOOL fIsResizingSY = FALSE;		

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_fptEndDragSet) PaintInMemory();	// ����� ������� ������ ���������
	m_fptEndDragSet = FALSE;	// ����� ������������

	if (!m_fIsDraggingInProgress)
	{
		if (m_pLastSelectedBookItem && 
			(itSelItemType == SelItem_BookPage))	// ����� �������� �� �������!
		{
			CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;

			if (((pLastSelectedPage->Get_iTop() - I_RESIZE_BORDER_WIDTH <= point.y)&&
				(pLastSelectedPage->Get_iTop() + I_RESIZE_BORDER_WIDTH >= point.y))&&
				((pLastSelectedPage->Get_iLeft() <= point.x)&&
				(pLastSelectedPage->Get_iLeft()+((CBookPage*) m_pLastSelectedBookItem)->Get_iWidth() >= point.x)))
			{								
					fIsResizingNY = TRUE;										
					fIsResizingSY = FALSE;				
			}

			if (((pLastSelectedPage->Get_iTop() + pLastSelectedPage->Get_iHeight() - I_RESIZE_BORDER_WIDTH <= point.y)&&
				(pLastSelectedPage->Get_iTop() + pLastSelectedPage->Get_iHeight() + I_RESIZE_BORDER_WIDTH >= point.y))&&
				((pLastSelectedPage->Get_iLeft() <= point.x)&&
				(pLastSelectedPage->Get_iLeft()+pLastSelectedPage->Get_iWidth() >= point.x)))
			{						
					fIsResizingNY = FALSE;
					fIsResizingSY = TRUE;				
			}			
				
			if (((pLastSelectedPage->Get_iLeft() - I_RESIZE_BORDER_WIDTH <= point.x)&&
				(pLastSelectedPage->Get_iLeft() + I_RESIZE_BORDER_WIDTH >= point.x))&&
				((pLastSelectedPage->Get_iTop() <= point.y)&&
				(pLastSelectedPage->Get_iTop()+pLastSelectedPage->Get_iHeight() >= point.y)))
			{		
					fIsResizingWX = TRUE;					
					fIsResizingEX = FALSE;							
			}	
			
			if (((pLastSelectedPage->Get_iLeft() + pLastSelectedPage->Get_iWidth() - I_RESIZE_BORDER_WIDTH <= point.x)&&
				(pLastSelectedPage->Get_iLeft() + pLastSelectedPage->Get_iWidth() + I_RESIZE_BORDER_WIDTH >= point.x))&&
				((pLastSelectedPage->Get_iTop() <= point.y)&&
				(pLastSelectedPage->Get_iTop()+pLastSelectedPage->Get_iHeight() >= point.y)))
			{						
					fIsResizingWX = FALSE;					
					fIsResizingEX = TRUE;									
			}				

			if ((fIsResizingNY && fIsResizingWX) || (fIsResizingSY && fIsResizingEX))
				cur = AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE);
			else if ((fIsResizingNY && fIsResizingEX) || (fIsResizingSY && fIsResizingWX))
				cur = AfxGetApp()->LoadStandardCursor(IDC_SIZENESW);		
			else if (fIsResizingNY || fIsResizingSY)
				cur = AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
			else if (fIsResizingWX || fIsResizingEX)
				cur = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
		}	
		else
			fIsResizingWX = fIsResizingEX = fIsResizingNY = fIsResizingSY = FALSE;	// (*)

		// ��� ��� ������������, �� ���� ����� ������ �� ������, �� ��� �� ����� ������...
		// �� ���� � ���� ������ ��� ���� ���� ���������, ����� ��������� ���������� ������� ��� resizing
		if (!m_fMouseDownNow) 
			fIsResizingWX = fIsResizingEX = fIsResizingNY = fIsResizingSY = FALSE;
	}

	if (!cur)
	{		
		if ((m_fIsDraggingStarted || m_fIsDraggingInProgress)&&
			(!fIsResizingWX)&&
			(!fIsResizingEX)&&
			(!fIsResizingNY)&&
			(!fIsResizingSY))	// �� ����� �������!
			cur = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		else
		{
			if (tb_Add_Option == OptAdd_None)
				cur = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			else if (tb_Add_Option == OptAdd_BookPage)
				cur = hCurAddPage;
			else if (tb_Add_Option == OptAdd_Link)
				cur = hCurAddLink;
		} 
	}

	// ���������, �������, ������...
	if (cur) SetWndCursor(cur);	

	if (fIsResizingNY)	// (itSelItemType == SelItem_BookPage) - �������������� ���������, �.�. ��. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// ������� ����� �������� ��� ��������������, �. �. ��� ����� ������
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;		
		if (fNeedRepaint) PaintInMemory();	// ���� � ������� ��� resizing'� �� ����, �� ���� ���������� �������� � ������, ��� ��� ����� � ������ �� �������� �������� � ������ �������
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;
		int iResize;		
		if (point.y < pLastSelectedPage->Get_iTop() + pLastSelectedPage->Get_iHeight()) 
		{
			iResize = (pLastSelectedPage->Get_iTop() + pLastSelectedPage->Get_iHeight()) - point.y;
			pLastSelectedPage->Get_iTop() = point.y;
			pLastSelectedPage->Get_iHeight() = iResize;
		}
		pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
		Invalidate();
	}

	if (fIsResizingWX)	// (itSelItemType == SelItem_BookPage) - �������������� ���������, �.�. ��. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// ������� ����� �������� ��� ��������������, �. �. ��� ����� ������
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;		
		if (fNeedRepaint) PaintInMemory();	// ���� � ������� ��� resizing'� �� ����, �� ���� ���������� �������� � ������, ��� ��� ����� � ������ �� �������� �������� � ������ �������
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;
		int iResize;		
		if (point.x < pLastSelectedPage->Get_iLeft() + pLastSelectedPage->Get_iWidth()) 
		{
			iResize = (pLastSelectedPage->Get_iLeft() + pLastSelectedPage->Get_iWidth()) - point.x;
			pLastSelectedPage->Get_iLeft() = point.x;
			pLastSelectedPage->Get_iWidth() = iResize;
		}
		pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
		Invalidate();
	}

	if (fIsResizingSY)	// (itSelItemType == SelItem_BookPage) - �������������� ���������, �.�. ��. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// ������� ����� �������� ��� ��������������, �. �. ��� ����� ������
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;		
		if (fNeedRepaint) PaintInMemory();	// ���� � ������� ��� resizing'� �� ����, �� ���� ���������� �������� � ������, ��� ��� ����� � ������ �� �������� �������� � ������ �������
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;		
		if (point.y > pLastSelectedPage->Get_iTop()) pLastSelectedPage->Get_iHeight() = point.y - pLastSelectedPage->Get_iTop();
		pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
		Invalidate();
	}

	if (fIsResizingEX)	// (itSelItemType == SelItem_BookPage) - �������������� ���������, �.�. ��. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// ������� ����� �������� ��� ��������������, �. �. ��� ����� ������		
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;
		if (fNeedRepaint) PaintInMemory();	// ���� � ������� ��� resizing'� �� ����, �� ���� ���������� �������� � ������, ��� ��� ����� � ������ �� �������� �������� � ������ �������
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;		
		if (point.x > pLastSelectedPage->Get_iLeft()) pLastSelectedPage->Get_iWidth() = point.x - pLastSelectedPage->Get_iLeft();
		pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
		Invalidate();
	}

	if ((m_fIsDraggingStarted || m_fIsDraggingInProgress)&&
		(!fIsResizingWX)&&
		(!fIsResizingEX)&&
		(!fIsResizingNY)&&
		(!fIsResizingSY))	// �� ����� �������!
	{						
		m_iOffsDragX = point.x - ptStartDrag.x;
		m_iOffsDragY = point.y - ptStartDrag.y;
		if (!m_fIsDraggingInProgress) 
		{
			m_fIsDraggingInProgress = TRUE;
			PaintInMemory(FALSE);	// ������ �������������� => ��������� �� �������� ���������� ��������� ���, ����� ��������������� ��������.		
		}
		pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
		Invalidate();		
		ptStartDrag = point;	// ����� ����, ��� �������������� ���� ���������, ��������� ���...
	}		
	else
		m_fIsDraggingInProgress = FALSE;
	
	if (m_fptBeginDragSet)
	{								
		m_fptEndDragSet = TRUE;
		m_ptEndDragSelBox = point;	
		PaintInMemory();	// ����� ���������� ����� ���������
	}

	CView::OnMouseMove(nFlags, point);
}

VOID CNLB_BldrView::SetWndCursor(HCURSOR Cur)
{
	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG) Cur);	// ���������� ������ ������ � �������� ����������� ��� ������� ����...
	// SetClassLongPtr(GetSafeHwnd(), GCLP_HCURSOR, (LONG_PTR) Cur); -- this can be used for amd64 compatibility...
	SetCursor(Cur);	// ...� ���������� ��� �����. ���� ����� �� �������, ��������� ��������� ������ ����� �������� ������
}

void CNLB_BldrView::OnEditAddlink() 
{
	tb_Add_Option = OptAdd_Link;	
}

void CNLB_BldrView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CBookPage* pPageCur;	
	CLink* pLinkCur;
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	
	int iPagesCount;
	int iLnksCount;

	// �� ������� ������ ������ ���� ������ ����������� ����� ��������
	m_pLastSelectedBookItem = NULL;	// ����� ������������
	m_fIsDraggingStarted = FALSE;
	m_fIsDraggingInProgress = FALSE;
	m_pPageFrom = m_pPageTo = NULL;	// ���� ���� ������ ���������� ������, �� ������� �� ����...
	tb_Add_Option = OptAdd_None;
	itSelItemType = SelItem_None;	
	POSITION pos = pDoc->m_lstPages.GetHeadPosition();
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (int i=0; i<iPagesCount; i++)
	{		
		pPageCur = pDoc->m_lstPages.GetAt(pos);
		if (pPageCur->IsPointInside(point))
		{
			m_pLastSelectedBookItem = (CDrawable*) pPageCur;
			itSelItemType = SelItem_BookPage;

			if (!pPageCur->IsSelected())	// ��� �� ���� �������, ������, ��� ���������� ���������, � � ��������� ��������� ���������
			{
				DeselectAll();
				pPageCur->Select();	// ������ ��� �������	
				pDoc->m_iSelectedPagesCount = 1;	// ������ �������� ������ ���� ��������!
			}

			break;
		}
		pDoc->m_lstPages.GetNext(pos);

		if (!m_pLastSelectedBookItem)
		{
			// ���� �� �����, ������� �������� ������� ��� ������
			POSITION pos = pDoc->m_lstLnks.GetHeadPosition();
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				pLinkCur = pDoc->m_lstLnks.GetAt(pos);
				if (pLinkCur->IsPointInside(point))
				{
					if (!(pLinkCur->Get_pPageFrom()->IsSelected() && 
							pLinkCur->Get_pPageTo()->IsSelected()))
							DeselectAll();	// ��� �������� ������, �� ������� � ���������, �� ���� ����� ��������� �� ���� ���������.					
					m_pLastSelectedBookItem = (CDrawable*) pLinkCur;
					pLinkCur->Select();
					itSelItemType = SelItem_Link;								
					break;
				}				
				pDoc->m_lstLnks.GetNext(pos);
			}
		}
	}
	
	if (!m_pLastSelectedBookItem) DeselectAll();	// ��� ��������, ��� ������������ ������� �� ������� �����; ������ ��� �������

	PaintInMemory();

	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))	// ��������� ���������� ���� ��� ��������
	{
		CPoint ptClick;		
		CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
	    CMenu* edit_menu = menu_bar->GetSubMenu(I_EDIT_MENU_POS);    
	    ASSERT(edit_menu);		
		CMenu* edit_page_menu = edit_menu->GetSubMenu(I_EDIT_PAGE_MENU_POS);    
	    ASSERT(edit_page_menu);		
		
		ptClick = point;
		ClientToScreen(&ptClick);								
	    edit_page_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptClick.x, 
		  ptClick.y, this);
	}
	else if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_Link))	// ��������� ���������� ���� ��� ������
	{
		CPoint ptClick;		
		CMenu* menu_bar = AfxGetMainWnd()->GetMenu();
	    CMenu* edit_menu = menu_bar->GetSubMenu(I_EDIT_MENU_POS);    
	    ASSERT(edit_menu);		
		CMenu* edit_link_menu = edit_menu->GetSubMenu(I_EDIT_LINK_MENU_POS);    
	    ASSERT(edit_link_menu);		
		
		ptClick = point;
		ClientToScreen(&ptClick);								
	    edit_link_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptClick.x, 
		  ptClick.y, this);
	}

	CView::OnRButtonDown(nFlags, point);
}

void CNLB_BldrView::OnEditAddstartpoint() 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		
	BOOL fReallyNeedChangeStartPoint = true;

	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))
	{
		if (pDoc->m_pStartPoint != (CBookPage*) m_pLastSelectedBookItem)
		{
			if (pDoc->m_pStartPoint)
			{
				CString strCaption;
				m_strMessage.LoadString(IDS_CONFIRM_CHANGE_STARTPOINT);	
				strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
				if (MessageBox(m_strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDNO)
					fReallyNeedChangeStartPoint = false;
			}

			if (fReallyNeedChangeStartPoint)
			{
				pDoc->m_pStartPoint = (CBookPage*) m_pLastSelectedBookItem;
				pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
				PaintInMemory();
			}
		}
	}
	else if (itSelItemType == SelItem_None)
	{
		pDoc->m_pStartPoint = NULL;	// ������� ���...
		pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
		PaintInMemory();
	}
}


void CNLB_BldrView::OnEditEditpageDeletepage() 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_iSelectedPagesCount != 0)
	{
		CString strCaption;
		m_strMessage.LoadString(IDS_CONFIRM_DELSEL_PAGES);	
		strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
		if (MessageBox(m_strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			DeleteSelectedPages();
		}
	}
}

VOID CNLB_BldrView::DeleteSelectedPages()
{
	POSITION pos;
	int i;
	int iLnksCount;
	int iPagesCount;
	CBookPage* pCurPage;	
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// ������� ������� ��� ��������� � ���������� ���������� ������
	pos = pDoc->m_lstLnks.GetHeadPosition();
	i = 0;
	iLnksCount = pDoc->m_lstLnks.GetCount();
	while (i < iLnksCount)	// �������� �� ���� ���������, �����-�� �������, � �����-�� ������ ����������
	{		
		if ((pDoc->m_lstLnks.GetAt(pos)->Get_pPageFrom()->IsSelected()) ||
		   (pDoc->m_lstLnks.GetAt(pos)->Get_pPageTo()->IsSelected()))
		{
			POSITION posRemov = pos;			    		
			pDoc->m_lstLnks.GetNext(pos);
			pDoc->m_lstLnks.GetAt(posRemov)->Draw(&m_DCOffscreen, PARAM_ERASE);	// ������� �� ������ �����...
			// ��� �������������� ��������� �������� ������� ����� ������ �� ���������, ��� ��� ������� ��� ���� � ����� ������...
			delete (pDoc->m_lstLnks.GetAt(posRemov));
			pDoc->m_lstLnks.RemoveAt(posRemov);						
		}
		else pDoc->m_lstLnks.GetNext(pos);
		i++; 
	}

	// � ������ � �� ����!
	pos = pDoc->m_lstPages.GetHeadPosition();
	i = 0;
	iPagesCount = pDoc->m_lstPages.GetCount();
	while (i < iPagesCount)	// �������� �� ���� ���������, �����-�� �������, � �����-�� ������ ����������
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		if (pCurPage->IsSelected())
		{
			POSITION posRemov = pos;			    		
			pDoc->m_lstPages.GetNext(pos);
			pCurPage->Draw(&m_DCOffscreen, PARAM_ERASE);	// ������� �� ������ �����...
			// ��� �������������� ��������� �������� ������� ����� ������ �� ���������, ��� ��� ������� ��� ���� � ����� ������...
			if (pDoc->m_pStartPoint == pCurPage) pDoc->m_pStartPoint = NULL;
			delete pCurPage;
			pDoc->m_lstPages.RemoveAt(posRemov);								
		}
		else pDoc->m_lstPages.GetNext(pos);
		i++; 
	}
	
	
	pDoc->m_iSelectedPagesCount = 0;	// �.�. ��� ���������� ���� ������� (����, �������, ������ ���-�� ���� ��������)
	m_pLastSelectedBookItem = NULL;	// ������ ������ �� �������!
	// ��� � ���. �� ������ ���...
	
	pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
	PaintInMemory();			
	UpdateSB();	// ����� ����, ��������, ����� �������� ����������
}

void CNLB_BldrView::OnEditEditpageEditproperties() 
{
	int i, iLnksCount;
	CLink* pLinkCur;
	POSITION pos, pos_remov;
	CDialogPageProperties dlg(this);
	S_PLink_LCounter_And_LStrings* plcas;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))
	{
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;
		dlg.m_strPageCaption = pLastSelectedPage->Get_strPageCaption();
		dlg.m_strPageText = pLastSelectedPage->Get_strPageText();
		dlg.m_strPageVar = pLastSelectedPage->Get_strPageVar();
		dlg.m_fUsePageCaption = pLastSelectedPage->Get_UsePageCaption();
		dlg.m_txtColor = pLastSelectedPage->Get_cr_Txt_Color();
		pLastSelectedPage->GetLogBrush(&dlg.m_lBrush);
		pLastSelectedPage->GetLogPen(&dlg.m_lPen);				

		// dlg.m_ListLinksFrom ���� ����, ��� ��� ������ ������ ��� ������
		pos = pDoc->m_lstLnks.GetHeadPosition();
		i = 0;
		iLnksCount = pDoc->m_lstLnks.GetCount();
		while (i < iLnksCount)	
		{		
			pLinkCur = pDoc->m_lstLnks.GetAt(pos);
			if (pLinkCur->Get_pPageFrom() == pLastSelectedPage)
			{
				plcas = new S_PLink_LCounter_And_LStrings;
				plcas->pLink = pLinkCur;
				plcas->iOldLinkNumber = pLinkCur->Get_iLinkNumber();
				plcas->lpszLinkCaptionForLV = new TCHAR[MAX_LOADSTRING];
				plcas->lpszLinkConstraintsForLV = new TCHAR[MAX_LOADSTRING];
				dlg.m_ListLinksFrom.AddTail(plcas);			
			}

			pDoc->m_lstLnks.GetNext(pos);
			i++; 
		}

		if (dlg.DoModal() == IDOK)
		{
			pLastSelectedPage->Get_strPageCaption() = dlg.m_strPageCaption;
			pLastSelectedPage->Get_strPageText() = dlg.m_strPageText;
			pLastSelectedPage->Get_strPageVar() = dlg.m_strPageVar;
			pLastSelectedPage->Get_UsePageCaption() = dlg.m_fUsePageCaption;
			pLastSelectedPage->Get_cr_Txt_Color() = dlg.m_txtColor;
			pLastSelectedPage->SetLogBrush(&dlg.m_lBrush);
			pLastSelectedPage->SetLogPen(&dlg.m_lPen);
			pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
			PaintInMemory();
		}

		// � ����� ������ ������� ��������������� ������ �������
		pos = dlg.m_ListLinksFrom.GetHeadPosition();
		while (!(dlg.m_ListLinksFrom.IsEmpty()))	
		{					
			pos_remov = pos;
			plcas = dlg.m_ListLinksFrom.GetNext(pos);
			if (plcas)
			{				
				if (plcas->lpszLinkCaptionForLV) delete []plcas->lpszLinkCaptionForLV;
				if (plcas->lpszLinkConstraintsForLV) delete []plcas->lpszLinkConstraintsForLV;
				// plcas->pLink �� �������!!!
				delete plcas;
			}
			dlg.m_ListLinksFrom.RemoveAt(pos_remov);
		}
	}
}

void CNLB_BldrView::OnUpdateEditEditpageDeletepage(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		

	if (pDoc->m_iSelectedPagesCount != 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);	
}

void CNLB_BldrView::OnUpdateEditEditpageEditproperties(CCmdUI* pCmdUI) 
{
	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);		
}

void CNLB_BldrView::OnEditEditlinkDeletelink() 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		

	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_Link))
	{
		CString strCaption;
		m_strMessage.LoadString(IDS_CONFIRM_DELSEL_LINK);	
		strCaption.LoadString(IDS_CONFIRM_MB_CAPT);
		if (MessageBox(m_strMessage, strCaption, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			EditlinkDeletelink((CLink*) m_pLastSelectedBookItem);
		}
	}	
}

BOOL CNLB_BldrView::EditlinkDeletelink(CLink* pLinkToDelete)
{
	CLink* pLinkCur;
	POSITION pos;
	int iLnksCount;
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	pos = pDoc->m_lstLnks.GetHeadPosition();
	iLnksCount = pDoc->m_lstLnks.GetCount();
	for (int i=0; i<iLnksCount; i++)
	{		
		pLinkCur = pDoc->m_lstLnks.GetAt(pos);		
		if (pLinkToDelete == pLinkCur)
		{
			pLinkCur->Draw(&m_DCOffscreen, PARAM_ERASE);	// ������� �� ������ �����...
			// ��� �������������� ��������� �������� ������� ����� ������ �� ���������, ��� ��� ������� ��� ���� � ����� ������...
			delete (pDoc->m_lstLnks.GetAt(pos));
			pDoc->m_lstLnks.RemoveAt(pos);
			m_pLastSelectedBookItem = NULL;	// ������ ������ �� �������!
			pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
			PaintInMemory();
			return TRUE;			
		}

		pDoc->m_lstLnks.GetNext(pos);
	}

	return FALSE;
}

void CNLB_BldrView::OnEditEditlinkEditproperties() 
{	
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_Link))
	{
		CLink* pLastSelectedLink = (CLink*) m_pLastSelectedBookItem;
		EditlinkEditproperties(pLastSelectedLink);
	}	
}

BOOL CNLB_BldrView::EditlinkEditproperties(CLink* pLinkToEdit)
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pLinkToEdit)
	{
		CDialogLinkProperties dlg(this);	

		dlg.m_strLinkCaption = pLinkToEdit->Get_strLinkCaption();
		dlg.m_strLinkConstraints = pLinkToEdit->Get_strLinkConstraints();
		dlg.m_lstModificationsObj = pLinkToEdit->Get_BVModList();
		pLinkToEdit->GetLogPen(&(dlg.m_lPen));		
		if (dlg.DoModal() == IDOK)
		{
			pLinkToEdit->Get_strLinkCaption() = dlg.m_strLinkCaption;
			pLinkToEdit->Get_strLinkConstraints() = dlg.m_strLinkConstraints;
			pLinkToEdit->Get_BVModList() = dlg.m_lstModificationsObj;
			pLinkToEdit->SetLogPen(&(dlg.m_lPen));
			pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
			ClearInMemory();
			PaintInMemory();
		}

		return TRUE;
	}
	else
		return FALSE;
}

void CNLB_BldrView::OnUpdateEditEditlinkDeletelink(CCmdUI* pCmdUI) 
{
	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_Link))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);	
	
}

void CNLB_BldrView::OnUpdateEditEditlinkEditproperties(CCmdUI* pCmdUI) 
{
	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_Link))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);		
}

BOOL CNLB_BldrView::DeparametrizeText(CNLB_BldrDoc* pDoc,
									  CString& strText, 
									  CString& strFaultyVarName,
									  CString& strIns, int iLenIns,
									  CString& strPara, int iLenPara,
									  CString& strDash, int iLenDash,
									  CString& strApostrophe, int iLenApostrophe,
									  CString& strBackslash, int iLenBackslash,
									  CString& strStrVarParamSeps)
// strFaultyVarName - ��� ����������, �� ������� ������������ ������ (���� ������� ����� �����)
{
	int isLen = strText.GetLength();
	int i = 0;	// ������� �������, �.�. ������ ������ � ����� ���������
	while (i < isLen-1)	
	{
		if ((strText[i]==_T('\r'))&&(strText[i+1]==_T('\n')))	// ���������� ������� ������
		{
			strText.Delete(i, 2);
			strText.Insert(i, strIns);			
			isLen+=(iLenIns-2);	// ����� ������ ����������� �� iLenIns ��������, �� 2 ���� �������...
			i+=iLenIns;	// ������� � �������, ���������� �� ����������� �������...

			if (i < isLen-1) // ���� ����� ������ ���������� (������ ����� ��������� �� �������)
			{
				if ((strText[i]==_T(' '))||(strText[i]==_T('\t')))	// ���� ��������� ������ �������������� ������ ������ ���������...
				{
					strText.Insert(i, strPara);	// ...�� � ������� ��� ��� ���������!
					isLen+=iLenPara;
					i+=iLenPara;
				}
			}
		}
		else if ((strText[i]==_T('-')) && (strText[i+1]==_T('-')))  // ������� �� ���� &mdash;
		{
			strText.Delete(i, 2);
			strText.Insert(i, strDash);			
			isLen+=(iLenDash-2);
			i+=iLenDash;
		}
		else if (((strText[i]==_T('-')) && _istspace((_TXCHAR) strText[i+1])) ||
				((strText[i]==_T('-')) && (strText[i+1]==_T('\r'))))
				// ������� �� ���� &mdash; (������� ��-�������)
		{
			strText.Delete(i, 1);
			strText.Insert(i, strDash);			
			isLen+=(iLenDash-1);
			i+=iLenDash;
		}
		else if (strText[i]==_T('\''))  // �������� - ��������� ������ Antokolos NLB Engine. ������� ��� �� ���������� ����������.
		{
			strText.Delete(i, 1);
			strText.Insert(i, strApostrophe);			
			isLen+=(iLenApostrophe-1);
			i+=iLenApostrophe;
		}
		else if (strText[i]==_T('\\'))  // ������� - ��������� ������ ����� C. ��� ���� �������� ���������, ����� �� ��������� �����������
		{
			strText.Delete(i, 1);
			strText.Insert(i, strBackslash);			
			isLen+=(iLenBackslash-1);
			i+=iLenBackslash;
		}
		else if ((strText[i]==_T('%')) && (strText[i+1]==_T('%')))	// ������� - ��������� ������. � ��� ������� ������ ���������� ����� �������� ����� ���������� ������������ �� ��������� ��������. � ���������� %% �������� ������ ������ ��������
		{
			strText.Delete(i, 2);
			strText.Insert(i, _T('%'));			
			isLen--;
			i++;
		}			
		else if ((strText[i]==_T('%')) && (strText[i+1]!=_T('%')))	// ��� ������ ����� ����������
		{				
			int j = i + 1;	
			CString strVarName;
			int iStrVarLen;

			POSITION bvPos;
			CBookVar* pbv;

			POSITION svPos;
			CStringVar* psv;

			POSITION tsvPos;

			CString strVarValue;
			int iLenVarValue;
			BOOL fVarReplaceOK = FALSE;

			while (j < isLen)	
			{
				if (strText[j]==_T('%')) break;

				j++;
			}

			strVarName.Empty();
			if (j < isLen)	// ������� (j > i + 1) ����� ��������� �������������, �.�. ����� ��� %%, �.�. ����� ������� � ���������� ����� if
			{
				strVarName = strText.Mid(i+1, j - i - 1);
				bvPos = pDoc->FindBookVarPositionByName(strVarName);
				if (bvPos)	// �.�. ����� ���������� ������������� ����
				{
					pbv = pDoc->m_lstBookVars.GetAt(bvPos);
					if (pbv)
					{
						if (pbv->Get_fIsVarGlobal())	// ���������� ����� ��������� ������ ���������� ����������
						{
							iStrVarLen = strVarName.GetLength();
							strText.Delete(i, iStrVarLen + 2);	// �� ������� ������� � ������� �������� � ������ � �����
							strVarValue.Format(IDS_STRING_STR_VARVALUE_BUFFER, strVarName);
							strText.Insert(i, strVarValue);
							iLenVarValue = strVarValue.GetLength();
							isLen+=(iLenVarValue - (iStrVarLen + 2));
							i+=iLenVarValue;
							fVarReplaceOK = TRUE;
						}
					}
				}
				else
				{	
					int iCurParamCount;						
					int iCurDependencyCount;

					svPos = pDoc->FindStrVarPositionByName(strVarName);
					if (svPos)
					{
						psv = pDoc->m_lstStrVars.GetAt(svPos);							
						if (psv)
						{				
							iCurParamCount = psv->m_lstStrVarParameters.GetCount();
							iCurDependencyCount = psv->m_lstStrVarDependencies.GetCount();

							if ((iCurParamCount == 0) && (iCurDependencyCount == 0))
							{
								iStrVarLen = strVarName.GetLength();
								strText.Delete(i, iStrVarLen + 2);	// �� ������� ������� � ������� �������� � ������ � �����
								strVarValue.Format(IDS_STRING_STR_VARVALUE_BUFFER, strVarName);
								strText.Insert(i, strVarValue);
								iLenVarValue = strVarValue.GetLength();
								isLen+=(iLenVarValue - (iStrVarLen + 2));
								i+=iLenVarValue;									
								fVarReplaceOK = TRUE;
							}
							else
								fVarReplaceOK = FALSE;
						}							
					}
					else
					{
						tsvPos = pDoc->FindTSVarPositionByName(strVarName);
						if (tsvPos)
						{
							iStrVarLen = strVarName.GetLength();
							strText.Delete(i, iStrVarLen + 2);	// �� ������� ������� � ������� �������� � ������ � �����
							strVarValue.Format(IDS_STRING_TRIGGERED_STRING_VARVALUE_BUFFER, strVarName);
							strText.Insert(i, strVarValue);
							iLenVarValue = strVarValue.GetLength();
							isLen+=(iLenVarValue - (iStrVarLen + 2));
							i+=iLenVarValue;									
							fVarReplaceOK = TRUE;
						}
						else
						{
							TCHAR* pstrCurSVarName;						
							TCHAR* pstrTempVarName;	// ��� ����� ���������� _tcstok()											
							CMathTrans TransStrVarFuncFormula(strVarName);
							BOOL fVarsFound = TRUE;

							pstrTempVarName = new TCHAR[strVarName.GetLength() + 1];
							if (!pstrTempVarName) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
							
							if (pstrTempVarName)
							{
								_tcscpy(pstrTempVarName, strVarName);								
								
								pstrCurSVarName = _tcstok(pstrTempVarName, strStrVarParamSeps);

								while (pstrCurSVarName != NULL)
								{
									svPos = pDoc->FindStrVarPositionByName(pstrCurSVarName);
									if (svPos)	// �.�. ����� ���������� ������������� ����
									{
										psv = pDoc->m_lstStrVars.GetAt(svPos);
										iCurParamCount = psv->m_lstStrVarParameters.GetCount();
										iCurDependencyCount = psv->m_lstStrVarDependencies.GetCount();
										if (iCurParamCount > 0)
										{
											TransStrVarFuncFormula.AddFunctionDefinition(pstrCurSVarName, _T('S'), _T('S'), iCurParamCount);
										}
										else if (iCurDependencyCount > 0)
										{
											// ������� ��� ����������, �.�. f()
											TransStrVarFuncFormula.AddFunctionDefinition(pstrCurSVarName, _T('S'), _T('S'), 0);
										}
									}
									else
										fVarsFound = FALSE;

									pstrCurSVarName = _tcstok(NULL, strStrVarParamSeps);
								}

								if (fVarsFound)
								{
									if (TransStrVarFuncFormula.Compile() == IERR_ERROR_SUCCESS)
										fVarReplaceOK = TRUE;
									else
										fVarReplaceOK = FALSE;
								}
								else
									fVarReplaceOK = FALSE;

								if (fVarReplaceOK)
								{
									iStrVarLen = strVarName.GetLength();
									strText.Delete(i, iStrVarLen + 2);	// �� ������� ������� � ������� �������� � ������ � �����
									strVarValue.Format(IDS_STRING_STR_VARVALUE_BUFFER, strVarName);
									strText.Insert(i, strVarValue);
									iLenVarValue = strVarValue.GetLength();
									isLen+=(iLenVarValue - (iStrVarLen + 2));
									i+=iLenVarValue;									
								}

								delete []pstrTempVarName;
							}
						}
					}
				}
			}

			// ���� ��������� �����-�� ������, �� ������ ��������� �����, � ������������ ������� �� ���� ��������������
			if (!fVarReplaceOK)
			{
				strText.Delete(i, j - i + 1);
				isLen -= (j - i + 1);

				strFaultyVarName = strVarName;
				return I_DEPARAM_ERR_IN_VAR_VALUE;				
			}
		}
		else if ((strText[i]==_T('$')) && (strText[i+1]==_T('$')))	// ������ - ��������� ������. � ��� ������� � ��������� ���������� ���� �����, � ������� ����� ������ ����� �������� ��������������� ��������� ����������. � ���������� $$ �������� ������ ������ �������
		{
			strText.Delete(i, 2);
			strText.Insert(i, _T('$'));			
			isLen--;
			i++;
		}
		else if ((strText[i]==_T('$')) && (strText[i+1]!=_T('$')))	// ��� ������ ����� ����������, ������� ����� ��������. 
		{				
			int j = i + 1;
			CString strVarName;
			int iStrVarLen;
			
			POSITION svPos;
			CStringVar* psv;

			CString strVarInput;
			int iLenVarInput;
			BOOL fVarReplaceOK = FALSE;

			while (j < isLen)	
			{
				if (strText[j]==_T('$')) break;

				j++;
			}

			strVarName.Empty();
			if (j < isLen)	// ������� (j > i + 1) ����� ��������� �������������, �.�. ����� ��� $$, �.�. ����� ������� � ���������� ����� if
			{
				strVarName = strText.Mid(i+1, j - i - 1);
									
				svPos = pDoc->FindStrVarPositionByName(strVarName);
				if (svPos)	// �.�. ����� ���������� ������������� ����
				{
					psv = pDoc->m_lstStrVars.GetAt(svPos);
					if (psv)
					{								
						if (psv->Get_fSaveInTheBookmarksFile())	// ��������� ���� ����� ������ ��� ��� ����������, ������� ����� �����������
						{
							iStrVarLen = strVarName.GetLength();
							strText.Delete(i, iStrVarLen + 2);	// �� ������� ������� � ������� �������� � ������ � �����
							strVarInput.Format(IDS_STRING_STR_VARINPUT_BUFFER, strVarName, strVarName);
							strText.Insert(i, strVarInput);
							iLenVarInput = strVarInput.GetLength();
							isLen+=(iLenVarInput - (iStrVarLen + 2));
							i+=iLenVarInput;
							fVarReplaceOK = TRUE;
						}
					}
				}					
			}

			// ���� ��������� �����-�� ������, �� ������ ��������� �����, � ������������ ������� �� ���� ��������������
			if (!fVarReplaceOK)
			{
				strText.Delete(i, j - i + 1);
				isLen -= (j - i + 1);

				strFaultyVarName = strVarName;
				return I_DEPARAM_ERR_IN_VAR_INPUT;
			}
		}
		else
			i++;
	}

	return I_DEPARAM_ERR_SUCCESS;
}

BOOL CNLB_BldrView::Convert_To_NLB_Engine_v10(CString& strResult, BOOL& fNeedStartCompileOnError)
{		
	CString strCurPage = _TEXT("");		// ��������� �������������� ������� ��������

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		
	CString strPageVar;
	CString strPageVarBuf;
	strPageVarBuf.LoadString(IDS_STRING_PAGE_VAR);
	CString strPageVarBufForTS;
	strPageVarBufForTS.LoadString(IDS_STRING_PAGE_VAR_FOR_TS);
	CString strPageVarBufForModifyVars;
	strPageVarBufForModifyVars.LoadString(IDS_STRING_PAGE_VAR_FOR_MODIFY_VARS);
	CString strProbVar;
	CString strProbVarBuf;
	strProbVarBuf.LoadString(IDS_STRING_PROB_VAR);
	CString strProbVarBufForTS;
	strProbVarBufForTS.LoadString(IDS_STRING_PROB_VAR_FOR_TS);
	CString strBookVarBuf;
	strBookVarBuf.LoadString(IDS_STRING_BOOK_VAR);
	CString strBookVarBufForTS;
	strBookVarBufForTS.LoadString(IDS_STRING_BOOK_VAR_FOR_TS);
	CString strCaseBegins;
	CString strCaseBeginsBuf;
	strCaseBeginsBuf.LoadString(IDS_STRING_CASE_BEGINS);
	CString strFuncBegins;
	strFuncBegins.LoadString(IDS_STRING_FUNC_BEGINS);
	CString strFuncEnds;
	strFuncEnds.LoadString(IDS_STRING_FUNC_ENDS);
	CString strNumPages;
	CString strNumPagesBuf;
	strNumPagesBuf.LoadString(IDS_STRING_NUM_PAGES);
	CString strStartpoint;
	CString strStartpointBuf;
	strStartpointBuf.LoadString(IDS_STRING_STARTPOINT);
	CString strIns;
	strIns.LoadString(IDS_STRING_INS);
	CString strPara;		
	strPara.LoadString(IDS_STRING_PARA);
	CString strDash;
	strDash.LoadString(IDS_STRING_DASH);
	CString strApostrophe;
	strApostrophe.LoadString(IDS_APOSTROPHE_REPLACEMENT);
	CString strBackslash;
	strBackslash.LoadString(IDS_BACKSLASH_REPLACEMENT);
	CString strInsBegin;		
	CString strInsBeginBuf;		
	strInsBeginBuf.LoadString(IDS_STRING_INS_BEGIN);
	CString strInsBeginWOHdrBuf;		
	strInsBeginWOHdrBuf.LoadString(IDS_STRING_INS_BEGIN_WO_HEADER);
	CString strInsEnd;	
	strInsEnd.LoadString(IDS_STRING_INS_END);
	CString strVeryEnd;		
	CString strOption;	
	CString strOptionBuf;	
	strOptionBuf.LoadString(IDS_STRING_OPTION);
	CString strOptionWithConstraintBuf;	
	strOptionWithConstraintBuf.LoadString(IDS_STRING_OPTION_WITH_CONSTRAINT);
	CString strVarInBracketsBuf;	
	strVarInBracketsBuf.LoadString(IDS_STRING_VAR_IN_BRACKETS);
	CString strVarInBracketsConcatBuf;	
	strVarInBracketsConcatBuf.LoadString(IDS_STRING_VAR_IN_BRACKETS_CONCAT);
	int iLenDash = strDash.GetLength();
	int iLenPara = strPara.GetLength();
	int iLenIns = strIns.GetLength();
	int iLenApostrophe = strApostrophe.GetLength();
	int iLenBackslash = strBackslash.GetLength();
	int isLen;
	int i;
	int iPagesCount = pDoc->m_lstPages.GetCount();
	int iLnksCount = pDoc->m_lstLnks.GetCount();
	POSITION posPages;	// ������� � ������ �������
	POSITION posLnks;	// ������� � ������ ������
	CBookPage* pCurPage;
	CBookPage* pPageFrom;
	CLink* pCurLnk;	
	int iStartPointPos;
	CString strText;	
	CString strConstraint;
	CString strVarsToAppend;
	CList<CLink*, CLink*&> lstLinksForCurPage;	// ������ ������������ ��� ������ �������� ������, ��������������� �� ����������� m_iLinkNumber
	int iLnksCountForCurPage;
	POSITION posLnksForCurPage;
	CLink* pCurLnkForCurPage;
	CList<CBookVar*, CBookVar*&> lstBookVarsForCurPage;		// ������ ������������ �� ������� �������� Book Variables.
	POSITION PosInLstBookVars;
	POSITION ProbVarPos;
	CBookVar* pBookVarCur;
	CMathTrans* pTrans;
	POSITION posBookVarsMain;
	int iBookVarsMainCount;
	CStringVar* pStrVarCur;
	POSITION posStrVarsMain;
	int iStrVarsMainCount;
	CString strToInsertInStrVarDefinition;	
	int iDeparTextResult;
	CString strFaultyVarName;
	
	CString strDeclareVar;
	CString strDeclareVarBuf;	// ��� ����� ��� �����������, ��� Find ������ ��, ��� ����... ��. ����.
	strDeclareVarBuf.LoadString(IDS_STRING_DECLARE_VAR_BUF);	
	CString strGlobalVarDefinition;		
	CString strStrVarParamSeps;
	strStrVarParamSeps.LoadString(IDS_STR_VAR_PARAM_SEPS);
	CString strStrVarParamsList;
	CString strStrVarParamCur;
	POSITION PosInStrVarParamsList;
	CString strTemp;
	CString strDependentVarFunctionsImplementation;

	CString strTSVarDefinition;
	CTriggeredString* pTSVarCur;
	CString strToInsertInTSVarDefinition;
	CStringVar* pSVDefaultValueCur;
	POSITION posTSVarsMain;
	int iTSVarsMainCount;

	POSITION posTSCP;
	int iCurTSCPCount;
	S_TS_ConditionPair* tscp;
	CString strToInsertInTSVarDefinitionCur;

	// ������ ��� ����������� ������ ���������� ��� ������� Triggered String	
	CList<CBookPage*, CBookPage*&> lstPageVarsForCurTS;	
	POSITION posTSPage;
	CList<CProbVar*, CProbVar*&> lstProbVarsForCurTS;	
	POSITION posTSProb;
	CList<CBookVar*, CBookVar*&> lstWorkerListOfVarsForCurTS;	
	POSITION posTSWork;
	POSITION posTSBook;	// ������� ������� BookVar	
	POSITION posRemovForTSWork;
	CBookPage* pPageVarCur_ForTS;
	CProbVar* pProbVarCur_ForTS;
	CBookVar* pBookVarCur_ForTS;
	int ibvIdx, isvIdx;
		
	strResult.Empty();	// ������� �� ������...

	if (pDoc->m_pStartPoint)
	{
		iStartPointPos = pDoc->FindPageIndexByPointer(pDoc->m_pStartPoint);
	}
	else
		return FALSE;	// ��� startpoint'� ����� �� ������

	fNeedStartCompileOnError = TRUE;

	// ������� ���������� � ���������� ����������
	posBookVarsMain = pDoc->m_lstBookVars.GetHeadPosition();
	iBookVarsMainCount = pDoc->m_lstBookVars.GetCount();
	for (ibvIdx = 0; ibvIdx < iBookVarsMainCount; ibvIdx++)
	{
		 pBookVarCur = pDoc->m_lstBookVars.GetNext(posBookVarsMain);
		 if (pBookVarCur->Get_fIsVarGlobal())
		 {
			 strGlobalVarDefinition.Format(IDS_STRING_GLOBALVAR_DEFINITION_BLOCK, 
				 pBookVarCur->Get_strBookVarName(), 
				 pBookVarCur->Get_strBookVarBody());
			 strResult += strGlobalVarDefinition;	// �������� ���������� � ���������� ����������
		 }
	}
		
	posStrVarsMain = pDoc->m_lstStrVars.GetHeadPosition();
	iStrVarsMainCount = pDoc->m_lstStrVars.GetCount();
	strDependentVarFunctionsImplementation.Empty();
	for (isvIdx = 0; isvIdx < iStrVarsMainCount; isvIdx++)
	{
		 int iStrVarParamsCountCur;		 

		 pStrVarCur = pDoc->m_lstStrVars.GetNext(posStrVarsMain);		 
		 strToInsertInStrVarDefinition = pStrVarCur->Get_strStrVarBody();		 
		 iDeparTextResult = DeparametrizeText(pDoc,
			strToInsertInStrVarDefinition, 
			strFaultyVarName,
			strIns, iLenIns,
			strPara, iLenPara,			
			strDash, iLenDash,
			strApostrophe, iLenApostrophe,
			strBackslash, iLenBackslash,
			strStrVarParamSeps);
		
		 if (iDeparTextResult == I_DEPARAM_ERR_IN_VAR_VALUE)
		 {
			// ���� �������������� �� ����������, �� �������� ����� ����� ������ ���� ������... �����, ��� � ������, �� ��� ����� ��� ������.
			CString strMessage;
			CString strCapt;
			
			strCapt.LoadString(IDS_ERROR_MB_CAPT);
			strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_VALUE_BUFFER, strFaultyVarName);
			if (MessageBox(strMessage, strCapt, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{				
				// �������� �������������� 
			}

			fNeedStartCompileOnError = FALSE;
			return FALSE;
		 }
		 else if (iDeparTextResult == I_DEPARAM_ERR_IN_VAR_INPUT)
		 {			
			// ���� �������������� �� ����������, �� �������� ����� ����� ������ ���� ������... �����, ��� � ������, �� ��� ����� ��� ������.
			CString strMessage;
			CString strCapt;
			
			strCapt.LoadString(IDS_ERROR_MB_CAPT);
			strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_INPUT_BUFFER, strFaultyVarName);
			if (MessageBox(strMessage, strCapt, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{
				// �������� �������������� 					
			}

			fNeedStartCompileOnError = FALSE;
			return FALSE;
		 }

		 iStrVarParamsCountCur = pStrVarCur->m_lstStrVarParameters.GetCount();
		 if (iStrVarParamsCountCur == 0)
		 {			 
			if (pStrVarCur->m_lstStrVarDependencies.GetCount() == 0)
			{
				strGlobalVarDefinition.Format(IDS_STRING_GLOBAL_STRVAR_DEFINITION_BLOCK, 
					pStrVarCur->Get_strStrVarName(), 
					strToInsertInStrVarDefinition);	
				strResult += strGlobalVarDefinition;	// �������� ���������� � ���������� ��������� ����������		 
			}
			else
			{
				strGlobalVarDefinition.Format(IDS_STRING_DEPENDENT_STRVAR_WITHOUT_PARAMETERS_DEFINITION_BLOCK, 
					pStrVarCur->Get_strStrVarName(), 
					strToInsertInStrVarDefinition);	
				strDependentVarFunctionsImplementation += strGlobalVarDefinition;	// �������� ���������� � ���������� ��������� ����������		 
			}
		 }
		 else
		 {
			strStrVarParamsList.Empty();
			PosInStrVarParamsList = pStrVarCur->m_lstStrVarParameters.GetHeadPosition();
			for (i = 0; i < iStrVarParamsCountCur - 1; i++)
			{
				strTemp = pStrVarCur->m_lstStrVarParameters.GetNext(PosInStrVarParamsList);
				strStrVarParamCur.Format(IDS_STR_VAR_PARAM_CUR_WITH_COMMA_BUF, strTemp);
				strStrVarParamsList += strStrVarParamCur;
			}
			// ��������� �������� ������ ��� �������
			strTemp = pStrVarCur->m_lstStrVarParameters.GetNext(PosInStrVarParamsList);
			strStrVarParamsList += strTemp;

			strTemp = pStrVarCur->Get_strStrVarName();
			strGlobalVarDefinition.Format(IDS_STRING_DEPENDENT_STRVAR_DEFINITION_BLOCK, 
				strTemp, 
				strStrVarParamsList,
				strTemp, 
				strToInsertInStrVarDefinition,
				strTemp, strTemp, strTemp, strTemp, strTemp, strTemp);

			strDependentVarFunctionsImplementation += strGlobalVarDefinition;
			// ����� ��� �� ������� � �����, ��� ���������� ��� ����, ����� ��� ����������, ������� ����� �������������� � �������, ���� �� ��� ����������
		 }		 
	}

	// ������� ��� ������� ��� Triggered String
	posTSVarsMain = pDoc->m_lstTriggeredStrings.GetHeadPosition();
	iTSVarsMainCount = pDoc->m_lstTriggeredStrings.GetCount();	
	for (int itsvIdx = 0; itsvIdx < iTSVarsMainCount; itsvIdx++)
	{		 
		 pTSVarCur = pDoc->m_lstTriggeredStrings.GetNext(posTSVarsMain);

		 // <���� ��� ���� �� ����������� ������ ���������� ��� �������� �����>
		 strVarsToAppend.Empty();
		 lstBookVarsForCurPage.RemoveAll();	// ������� ������			 
		 // </���� ��� ���� �� ����������� ������ ���������� ��� �������� �����>
		 lstPageVarsForCurTS.RemoveAll();	// ������� ������	
		 lstProbVarsForCurTS.RemoveAll();	// ������� ������	
		 lstWorkerListOfVarsForCurTS.RemoveAll();	// ������� ������			 

		 strToInsertInTSVarDefinition.Empty();
		 posTSCP = pTSVarCur->GetConditionPairsHeadPosition();
		 iCurTSCPCount = pTSVarCur->GetConditionPairsCount();

		 tscp = pTSVarCur->GetNextConditionPair(posTSCP);
		 if (tscp->pStrVar->m_lstStrVarDependencies.GetCount() == 0)
			 strToInsertInTSVarDefinitionCur.Format(IDS_STRING_TO_INSERT_IN_TS_VAR_DEFINITION_FORMAT_BLOCK1, tscp->pBVCondition->Get_strBookVarName(), tscp->pStrVar->Get_strStrVarName());
		 else
			 strToInsertInTSVarDefinitionCur.Format(IDS_STRING_TO_INSERT_IN_TS_VAR_DEFINITION_FORMAT_BLOCK5, tscp->pBVCondition->Get_strBookVarName(), tscp->pStrVar->Get_strStrVarName());
		 lstBookVarsForCurPage.AddTail(tscp->pBVCondition);
		 strToInsertInTSVarDefinition += strToInsertInTSVarDefinitionCur;
		 for (int itscpIdx = 1; itscpIdx < iCurTSCPCount; itscpIdx++)
		 {
			 tscp = pTSVarCur->GetNextConditionPair(posTSCP);
			 if (tscp->pStrVar->m_lstStrVarDependencies.GetCount() == 0)
				 strToInsertInTSVarDefinitionCur.Format(IDS_STRING_TO_INSERT_IN_TS_VAR_DEFINITION_FORMAT_BLOCK2, tscp->pBVCondition->Get_strBookVarName(), tscp->pStrVar->Get_strStrVarName());
			 else
				 strToInsertInTSVarDefinitionCur.Format(IDS_STRING_TO_INSERT_IN_TS_VAR_DEFINITION_FORMAT_BLOCK6, tscp->pBVCondition->Get_strBookVarName(), tscp->pStrVar->Get_strStrVarName());
			 lstBookVarsForCurPage.AddTail(tscp->pBVCondition);
			 strToInsertInTSVarDefinition += strToInsertInTSVarDefinitionCur;
		 }		 
		 pSVDefaultValueCur = pTSVarCur->GetDefaultValue();
		 if (pSVDefaultValueCur)
		 {
			 if (pSVDefaultValueCur->m_lstStrVarDependencies.GetCount() == 0)
				 strToInsertInTSVarDefinitionCur.Format(IDS_STRING_TO_INSERT_IN_TS_VAR_DEFINITION_FORMAT_BLOCK3, pSVDefaultValueCur->Get_strStrVarName());
			 else
				 strToInsertInTSVarDefinitionCur.Format(IDS_STRING_TO_INSERT_IN_TS_VAR_DEFINITION_FORMAT_BLOCK7, pSVDefaultValueCur->Get_strStrVarName());
		 }
		 else
			 strToInsertInTSVarDefinitionCur.LoadString(IDS_STRING_TO_INSERT_IN_TS_VAR_DEFINITION_FORMAT_BLOCK4);
		 strToInsertInTSVarDefinition += strToInsertInTSVarDefinitionCur;

		 // ��������� ��� ����������� ����������
		 PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
		 while (PosInLstBookVars)
		 {		
		 	pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);			
			lstWorkerListOfVarsForCurTS.AddTail(pBookVarCur);
		 }

		 lstBookVarsForCurPage.RemoveAll();	// ������� ������, �� ����� �������� ������
		 posTSWork = lstWorkerListOfVarsForCurTS.GetHeadPosition();
		 while (posTSWork)
		 {
			 pBookVarCur = lstWorkerListOfVarsForCurTS.GetAt(posTSWork);
			 
			 if (!lstBookVarsForCurPage.Find(pBookVarCur)) lstBookVarsForCurPage.AddTail(pBookVarCur);
			 posRemovForTSWork = posTSWork;
			 lstWorkerListOfVarsForCurTS.GetNext(posTSWork);				 
			 lstWorkerListOfVarsForCurTS.RemoveAt(posRemovForTSWork);

			 CMathTrans aTrans(pBookVarCur->Get_strBookVarBody());
			 if (aTrans.Compile() != IERR_ERROR_SUCCESS) return FALSE;
			 try
			 {
				 CString strCurVar(_T(' '), MAX_VAR_LENGTH);	// MAX_VAR_LENGTH ��������. MAX_VAR_LENGTH - ��������� �� CLogicTrans.h. ������������� ���������� �� ����� ����� ������� �����.										 
				
				 if (aTrans.GetFirstVar(strCurVar, MAX_VAR_LENGTH))
				 {						 					 
					 posTSProb = pDoc->FindProbVarPositionByName(strCurVar);

					 // ��� ��������� ������
					 if (posTSProb) 
						 posTSBook = NULL;
					 else
						 posTSBook = pDoc->FindBookVarPositionByName(strCurVar);

					 // ��� ��������� ������
					 if (posTSProb || posTSBook)
						 posTSPage = NULL;
					 else
						 posTSPage = pDoc->FindPagePositionByName(strCurVar);

					 if (posTSPage) 
					 {
						 pPageVarCur_ForTS = pDoc->m_lstPages.GetAt(posTSPage);
						 if (!lstPageVarsForCurTS.Find(pPageVarCur_ForTS)) lstPageVarsForCurTS.AddTail(pPageVarCur_ForTS);
					 }
					 else if (posTSProb)
					 {
						 pProbVarCur_ForTS = pDoc->m_lstProbVars.GetAt(posTSProb);
						 if (!lstProbVarsForCurTS.Find(pProbVarCur_ForTS)) lstProbVarsForCurTS.AddTail(pProbVarCur_ForTS);
					 }
					 else if (posTSBook)
					 {
						pBookVarCur_ForTS = pDoc->m_lstBookVars.GetAt(posTSBook);
						if (!lstBookVarsForCurPage.Find(pBookVarCur_ForTS)) lstWorkerListOfVarsForCurTS.AddTail(pBookVarCur_ForTS);
					 }
					 else
						 ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_UNKNOWN_ERROR_WHILE_PROCESSING_TRIGGERED_STRING_VAR);

					 while (aTrans.GetNextVar(strCurVar, MAX_VAR_LENGTH))
					 {					
						 posTSProb = pDoc->FindProbVarPositionByName(strCurVar);

						 // ��� ��������� ������
						 if (posTSProb) 
							 posTSBook = NULL;
						 else
							 posTSBook = pDoc->FindBookVarPositionByName(strCurVar);

						 // ��� ��������� ������
						 if (posTSProb || posTSBook)
							 posTSPage = NULL;
						 else
							 posTSPage = pDoc->FindPagePositionByName(strCurVar);

						 if (posTSPage) 
						 {
							 pPageVarCur_ForTS = pDoc->m_lstPages.GetAt(posTSPage);
							 if (!lstPageVarsForCurTS.Find(pPageVarCur_ForTS)) lstPageVarsForCurTS.AddTail(pPageVarCur_ForTS);
						 }
						 else if (posTSProb)
						 {
							 pProbVarCur_ForTS = pDoc->m_lstProbVars.GetAt(posTSProb);
							 if (!lstProbVarsForCurTS.Find(pProbVarCur_ForTS)) lstProbVarsForCurTS.AddTail(pProbVarCur_ForTS);
						 }
						 else if (posTSBook)
						 {
							pBookVarCur_ForTS = pDoc->m_lstBookVars.GetAt(posTSBook);
							if (!lstBookVarsForCurPage.Find(pBookVarCur_ForTS)) lstWorkerListOfVarsForCurTS.AddTail(pBookVarCur_ForTS);
						 }
						 else
							 ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_UNKNOWN_ERROR_WHILE_PROCESSING_TRIGGERED_STRING_VAR);
					 }
				 } 
			 }
			 catch (CMemoryException e)
			 {
				 ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
			 }			 
		 }

		 int iVarPageIndex;	
		 posTSPage = lstPageVarsForCurTS.GetHeadPosition();
		 while (posTSPage)
		 {
			 pPageVarCur_ForTS = lstPageVarsForCurTS.GetNext(posTSPage);
			 iVarPageIndex = pDoc->FindPageIndexByPointer(pPageVarCur_ForTS);
			 strPageVar.Format(strPageVarBufForTS, 
				 pPageVarCur_ForTS->Get_strPageVar(), 
				 iVarPageIndex, 
				 iVarPageIndex);
			 strVarsToAppend += strPageVar;
		 }

		 posTSProb = lstProbVarsForCurTS.GetHeadPosition();
		 while (posTSProb)
		 {
			 pProbVarCur_ForTS = lstProbVarsForCurTS.GetNext(posTSProb);
			 strProbVar.Format(strProbVarBufForTS, 
				 pProbVarCur_ForTS->Get_strProbVarName(), 
				 pProbVarCur_ForTS->Get_iDiceK(),
				 pProbVarCur_ForTS->Get_iDiceY());
			 strVarsToAppend += strProbVar;
		 }

		 // <���� ��� ���� �� ����������� ������ ���������� ��� �������� �����>
		 SortBookVars(lstBookVarsForCurPage);	// ��������� ���������� � ������ � ������ �������
		 // ����, ���������� � ������ ����������� � ������ �������. ������ ���������� ������ � ������������� ����������.
		 PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
		 while (PosInLstBookVars)
		 {		
		 	pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);
			if (!pBookVarCur->Get_fIsVarGlobal())	// ���������� ���������� �������������� �� ����!
			{
				strPageVar.Format(strBookVarBufForTS, 
						pBookVarCur->Get_strBookVarName(), 
						pBookVarCur->Get_strBookVarBody());
				strVarsToAppend += strPageVar;
			}
		 }
		 // </���� ��� ���� �� ����������� ������ ���������� ��� �������� �����>
		 
		 strTSVarDefinition.Format(IDS_STRING_TRIGGERED_STRING_DEFINITION_BLOCK, 
						pTSVarCur->Get_strTSName(), 
						strVarsToAppend,
						strToInsertInTSVarDefinition);	
		 strDependentVarFunctionsImplementation += strTSVarDefinition;
	}

	lstBookVarsForCurPage.RemoveAll();	// ������� ������	
	lstPageVarsForCurTS.RemoveAll();	// ������� ������	
	lstProbVarsForCurTS.RemoveAll();	// ������� ������	
	lstWorkerListOfVarsForCurTS.RemoveAll();	// ������� ������		

	strNumPages.Format(strNumPagesBuf, iPagesCount);	// ������ ���������� � ����� ������� � �����
	strStartpoint.Format(strStartpointBuf, iStartPointPos);
	strResult += strNumPages;	// �������� ���������� � ����� ������� � �����
	strResult += strStartpoint;	// �������� ���������� � ��������� ��������	

	strResult += strFuncBegins;	// �������� ������� GoTo
	posPages = pDoc->m_lstPages.GetHeadPosition();
	for (int iPage=0; iPage<iPagesCount; iPage++)	// ���� �� ���������...
	{
		pCurPage = pDoc->m_lstPages.GetAt(posPages);
		strCurPage = pCurPage->Get_strPageText();	// ��������� �� �������� ������ ��������� ��������
		
		iDeparTextResult = DeparametrizeText(pDoc,
			strCurPage, 
			strFaultyVarName,
			strIns, iLenIns,
			strPara, iLenPara,			
			strDash, iLenDash,
			strApostrophe, iLenApostrophe,
			strBackslash, iLenBackslash,
			strStrVarParamSeps);
		
		if (iDeparTextResult == I_DEPARAM_ERR_IN_VAR_VALUE)
		{
			// ���� �������������� �� ����������, �� �������� ����� ����� ������ ���� ������... �����, ��� � ������, �� ��� ����� ��� ������.
			CString strMessage;
			CString strCapt;
			
			strCapt.LoadString(IDS_ERROR_MB_CAPT);
			strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_VALUE_BUFFER, strFaultyVarName);
			if (MessageBox(strMessage, strCapt, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{
				m_pLastSelectedBookItem = pCurPage;
				itSelItemType = SelItem_BookPage;
				ScrollToPage(pCurPage);
				OnEditEditpageEditproperties();						
			}

			fNeedStartCompileOnError = FALSE;
			return FALSE;
		}
		else if (iDeparTextResult == I_DEPARAM_ERR_IN_VAR_INPUT)
		{			
			// ���� �������������� �� ����������, �� �������� ����� ����� ������ ���� ������... �����, ��� � ������, �� ��� ����� ��� ������.
			CString strMessage;
			CString strCapt;
			
			strCapt.LoadString(IDS_ERROR_MB_CAPT);
			strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_INPUT_BUFFER, strFaultyVarName);
			if (MessageBox(strMessage, strCapt, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{
				m_pLastSelectedBookItem = pCurPage;
				itSelItemType = SelItem_BookPage;
				ScrollToPage(pCurPage);
				OnEditEditpageEditproperties();						
			}

			fNeedStartCompileOnError = FALSE;
			return FALSE;
		}
		
		if (pCurPage->Get_UsePageCaption())
			strInsBegin.Format(strInsBeginBuf, pCurPage->Get_strPageCaption());
		else
			strInsBegin = strInsBeginWOHdrBuf;
		strCurPage.Insert(0, strInsBegin);		
		isLen = strCurPage.GetLength();
		strCurPage.Insert(isLen, strInsEnd);
		
		posLnks = pDoc->m_lstLnks.GetHeadPosition();
		strVarsToAppend.Empty();
		lstBookVarsForCurPage.RemoveAll();	// ������� ������		
		lstLinksForCurPage.RemoveAll();		// ������� ������		
		for (int j=0; j<iLnksCount; j++)
		{
			pCurLnk = pDoc->m_lstLnks.GetAt(posLnks);
			pPageFrom = pCurLnk->Get_pPageFrom();

			if (pPageFrom == pCurPage)	// ���� ��� ������ - �� ������� ��������
			{
				BOOL fNotInserted = TRUE;

				posLnksForCurPage = lstLinksForCurPage.GetHeadPosition();
				iLnksCountForCurPage = lstLinksForCurPage.GetCount();
				for (int k=0; k<iLnksCountForCurPage; k++)
				{
					pCurLnkForCurPage = lstLinksForCurPage.GetAt(posLnksForCurPage);
					if (pCurLnkForCurPage->Get_iLinkNumber() < pCurLnk->Get_iLinkNumber())
						lstLinksForCurPage.GetNext(posLnksForCurPage);
					else
					{
						lstLinksForCurPage.InsertBefore(posLnksForCurPage, pCurLnk);
						fNotInserted = FALSE;
						break;
					}
				}				

				if (fNotInserted)
					lstLinksForCurPage.AddTail(pCurLnk);
			}
						
			pDoc->m_lstLnks.GetNext(posLnks);
		}			
		
		posLnksForCurPage = lstLinksForCurPage.GetHeadPosition();
		iLnksCountForCurPage = lstLinksForCurPage.GetCount();
		for (j=0; j<iLnksCountForCurPage; j++)
		{
			pCurLnkForCurPage = lstLinksForCurPage.GetAt(posLnksForCurPage);
			strText = pCurLnkForCurPage->Get_strLinkCaption();
			strConstraint = pCurLnkForCurPage->Get_strLinkConstraints();

			// ������� ������� ��� ������� ����� � ������. ����� ����, ����� ����� ������ ���� �������� ������
			strConstraint.TrimLeft();
			strConstraint.TrimRight();
			int iConstraintLength = strConstraint.GetLength();

			if (iConstraintLength!=0)
			{
				CString strTmpConstraint = strConstraint;
				int iVarPageIndex;
				
				if (CheckLinkContainInfiniteLoop(strTmpConstraint, lstBookVarsForCurPage, TRUE) != E_IL_OK) return FALSE;

				// ����� ���������� ���������� ������� ������� strTmpConstraint ��� �� �������� Book Variables, ������, �������� ��� ����������� ���������� ����������
				pTrans = new CMathTrans(strTmpConstraint);					
				if (pTrans)
				{
					if (pTrans->Compile() != IERR_ERROR_SUCCESS) return FALSE;
					// ������ �������� ������ ���������, ��� ��� ���������� �� ���� ������� ���� � ����� �����. 
					// ���� ����, �� ������� ��� ���������� � ���������������� ����� bookbody.js						

					try
					{
						CString strCurVar(_T(' '), MAX_VAR_LENGTH);	// MAX_VAR_LENGTH ��������. MAX_VAR_LENGTH - ��������� �� CLogicTrans.h. ������������� ���������� �� ����� ����� ������� �����.			
						
						if (pTrans->GetFirstVar(strCurVar, MAX_VAR_LENGTH))
						{												
							// strDeclareVar ����� ��������� ��� �����������, ��� ������� ������ ������ ��� ����������, ������� ��� ����, � �� ���-������ ��� (� �������, ������������ ������� ���������� ArrPageVisitState)...
							strDeclareVar.Format(strDeclareVarBuf, strCurVar);
							if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// �.� ����� ��� ���...
							{
								iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
								if (iVarPageIndex == -1) 
								{
									// ��������, ��� �� ����������, � ������������� ����������
									ProbVarPos = pDoc->FindProbVarPositionByName(strCurVar);
									if (ProbVarPos)
									{
										strProbVar.Format(strProbVarBuf, 
											strCurVar, 
											pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceK(),
											pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceY());
										strVarsToAppend.Insert(0, strProbVar);									
									}
									else
										return FALSE;	// ����� ���������� �� ���� �������, ������, ������������ ������� ���� ��� ����������
								}									
								else
								{
									strPageVar.Format(strPageVarBuf, 
										strCurVar, 
										iVarPageIndex, 
										iVarPageIndex);
									strVarsToAppend.Insert(0, strPageVar);									
								}
							}

							while (pTrans->GetNextVar(strCurVar, MAX_VAR_LENGTH))
							{
								// strDeclareVar ����� ��������� ��� �����������, ��� ������� ������ ������ ��� ����������, ������� ��� ����, � �� ���-������ ��� (� �������, ������������ ������� ���������� ArrPageVisitState)...
								strDeclareVar.Format(strDeclareVarBuf, strCurVar);
								if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// �.� ����� ��� ���...
								{
									iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
									if (iVarPageIndex == -1) 
									{
										// ��������, ��� �� ����������, � ������������� ����������
										ProbVarPos = pDoc->FindProbVarPositionByName(strCurVar);
										if (ProbVarPos)
										{
											strProbVar.Format(strProbVarBuf, 
												strCurVar, 
												pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceK(),
												pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceY());
											strVarsToAppend.Insert(0, strProbVar);									
										}
										else
											return FALSE;	// ����� ���������� �� ���� �������, ������, ������������ ������� ���� ��� ����������
									}									
									else
									{
										strPageVar.Format(strPageVarBuf, 
											strCurVar, 
											iVarPageIndex, 
											iVarPageIndex);
										strVarsToAppend.Insert(0, strPageVar);									
									}
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
									
				if (pCurLnkForCurPage->Get_BVModList().GetModificationsCount() == 0)
				{
					strOption.Format(strOptionWithConstraintBuf, strConstraint, 
						pDoc->FindPageIndexByPointer(pCurLnkForCurPage->Get_pPageTo()), 
						strText);
				}
				else
				{
					strOption.Format(IDS_STRING_OPTION_WITH_CONSTRAINT_AND_MODIFICATION, 
						strConstraint, 
						j,	// ������ ������� ������
						pDoc->FindPageIndexByPointer(pCurLnkForCurPage->Get_pPageTo()), 
						strText);
				}
			}
			else
			{
				if (pCurLnkForCurPage->Get_BVModList().GetModificationsCount() == 0)
				{
					strOption.Format(strOptionBuf, 
						pDoc->FindPageIndexByPointer(pCurLnkForCurPage->Get_pPageTo()), 
						strText);
				}
				else
				{
					strOption.Format(IDS_STRING_OPTION_WITH_MODIFICATION, 							
						j,	// ������ ������� ������
						pDoc->FindPageIndexByPointer(pCurLnkForCurPage->Get_pPageTo()), 
						strText);
				}
			}
			
			isLen = strCurPage.GetLength();
			strCurPage.Insert(isLen, strOption);
			lstLinksForCurPage.GetNext(posLnksForCurPage);
		}


		SortBookVars(lstBookVarsForCurPage);	// ��������� ���������� � ������ � ������ �������

		// ����, ���������� � ������ ����������� � ������ �������. ������ ���������� ������ � ������������� ����������.
		PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
		while (PosInLstBookVars)
		{		
			pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);
			if (!pBookVarCur->Get_fIsVarGlobal())	// ���������� ���������� �������������� �� ����!
			{
				strPageVar.Format(strBookVarBuf, 
						pBookVarCur->Get_strBookVarName(), 
						pBookVarCur->Get_strBookVarBody());
				strVarsToAppend += strPageVar;
			}
		}
								
		strCurPage.Insert(0, strVarsToAppend);
		strCaseBegins.Format(strCaseBeginsBuf, iPage, iPage);
		strCurPage.Insert(0, strCaseBegins);
		
		isLen = strCurPage.GetLength();
		strVeryEnd.LoadString(IDS_VERY_END);
		strCurPage.Insert(isLen, strVeryEnd);

		strResult += strCurPage;	// ����������...
		pDoc->m_lstPages.GetNext(posPages);	// � ������ ������!
	}
	strResult += strFuncEnds;	// ��������� ������� GoTo

	strResult += strDependentVarFunctionsImplementation;
	// ����� ����� GoTo ������� ����� �������� ���������� � �������� ��������� ��������� ���������� - ��. ����������� � ������

	// �������� ������� ModifyVars()...
	CString strCurLnkCase;
	CString strCurLnkCaseBegins;
	CString strCurLnkCaseEnds;
	strCurLnkCaseEnds.LoadString(IDS_STRING_CUR_LNK_CASE_ENDS);
	CString strModifyVarsFuncBegins;
	strModifyVarsFuncBegins.LoadString(IDS_STRING_MODIFYVAR_FUNC_BEGINS);
	strResult += strModifyVarsFuncBegins;
	CString strModifyVarsFuncEnds;
	CString strCurBVMod;
	POSITION posInModificationsList;
	int iModificationsCount;
	S_BookVarModification* pBVModCur;
	strModifyVarsFuncEnds.LoadString(IDS_STRING_MODIFYVAR_FUNC_ENDS);
	posLnks = pDoc->m_lstLnks.GetHeadPosition();
	for (int j=0; j<iLnksCount; j++)
	{
		pCurLnk = pDoc->m_lstLnks.GetNext(posLnks);
		iModificationsCount = pCurLnk->Get_BVModList().GetModificationsCount();
		if (iModificationsCount != 0)
		{
			strCurLnkCaseBegins.Format(IDS_STRING_CUR_LNK_CASE_BEGINS, j);
			strVarsToAppend.Empty();
			strCurLnkCase.Empty();
			lstBookVarsForCurPage.RemoveAll();	// ������� ������
			posInModificationsList = pCurLnk->Get_BVModList().GetModificationsHeadPosition();
			for (int iModIdx = 0; iModIdx < iModificationsCount; iModIdx++)
			{
				int iVarPageIndex;

				pBVModCur = pCurLnk->Get_BVModList().GetNextModification(posInModificationsList);

				strCurBVMod.Format(IDS_STRING_CURRENT_BOOKVAR_MODIFICATION, 
					pBVModCur->pModifiedBookVar->Get_strBookVarName(), 
					pBVModCur->pModificationBody->Get_strBookVarName());
				strCurLnkCase += strCurBVMod;

				CString strTmpConstraint = pBVModCur->pModificationBody->Get_strBookVarName();													
				if (CheckLinkContainInfiniteLoop(strTmpConstraint, lstBookVarsForCurPage, TRUE) != E_IL_OK) return FALSE;

				// ����� ���������� ���������� ������� ������� strTmpConstraint ��� �� �������� Book Variables
				CMathTrans aTrans(strTmpConstraint);										
				if (aTrans.Compile() != IERR_ERROR_SUCCESS) return FALSE;

				// ������ ��������� ������������� ���������� � ������������� ���������� � ������������
				// if (!aTrans.VarsListIsEmpty()) return FALSE;	// ������ �� ���������� ���������� ���������!!! ���������� � ���������� ������� ������ ��������� ������ ��������� ���������!!!				

				try
				{
					CString strCurVar(_T(' '), MAX_VAR_LENGTH);	// MAX_VAR_LENGTH ��������. MAX_VAR_LENGTH - ��������� �� CLogicTrans.h. ������������� ���������� �� ����� ����� ������� �����.			
					
					if (aTrans.GetFirstVar(strCurVar, MAX_VAR_LENGTH))
					{												
						// strDeclareVar ����� ��������� ��� �����������, ��� ������� ������ ������ ��� ����������, ������� ��� ����, � �� ���-������ ��� (� �������, ������������ ������� ���������� ArrPageVisitState)...
						strDeclareVar.Format(strDeclareVarBuf, strCurVar);
						if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// �.� ����� ��� ���...
						{
							iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
							if (iVarPageIndex == -1) 
							{
								// ��������, ��� �� ����������, � ������������� ����������
								ProbVarPos = pDoc->FindProbVarPositionByName(strCurVar);
								if (ProbVarPos)
								{
									strProbVar.Format(strProbVarBuf, 
										strCurVar, 
										pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceK(),
										pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceY());
									strVarsToAppend.Insert(0, strProbVar);									
								}
								else
									return FALSE;	// ����� ���������� �� ���� �������, ������, ������������ ������� ���� ��� ����������
							}									
							else
							{
								strPageVar.Format(strPageVarBufForModifyVars, 
									strCurVar, 
									iVarPageIndex, 
									iVarPageIndex);
								strVarsToAppend.Insert(0, strPageVar);									
							}
						}

						while (aTrans.GetNextVar(strCurVar, MAX_VAR_LENGTH))
						{
							// strDeclareVar ����� ��������� ��� �����������, ��� ������� ������ ������ ��� ����������, ������� ��� ����, � �� ���-������ ��� (� �������, ������������ ������� ���������� ArrPageVisitState)...
							strDeclareVar.Format(strDeclareVarBuf, strCurVar);
							if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// �.� ����� ��� ���...
							{
								iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
								if (iVarPageIndex == -1) 
								{
									// ��������, ��� �� ����������, � ������������� ����������
									ProbVarPos = pDoc->FindProbVarPositionByName(strCurVar);
									if (ProbVarPos)
									{
										strProbVar.Format(strProbVarBuf, 
											strCurVar, 
											pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceK(),
											pDoc->m_lstProbVars.GetAt(ProbVarPos)->Get_iDiceY());
										strVarsToAppend.Insert(0, strProbVar);									
									}
									else
										return FALSE;	// ����� ���������� �� ���� �������, ������, ������������ ������� ���� ��� ����������
								}									
								else
								{
									strPageVar.Format(strPageVarBufForModifyVars, 
										strCurVar, 
										iVarPageIndex, 
										iVarPageIndex);
									strVarsToAppend.Insert(0, strPageVar);									
								}
							}
						}
					}
				}
				catch (CMemoryException e)
				{
					((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
				}
			}
			SortBookVars(lstBookVarsForCurPage);	// ��������� ���������� � ������ � ������ �������

			// ����, ���������� � ������ ����������� � ������ �������. ������ ���������� ������ � ������������� ����������.
			PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
			while (PosInLstBookVars)
			{		
				pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);
				if (!pBookVarCur->Get_fIsVarGlobal())	// ���������� ���������� �������������� �� ����!
				{
					strPageVar.Format(strBookVarBuf, 
							pBookVarCur->Get_strBookVarName(), 
							pBookVarCur->Get_strBookVarBody());
					strVarsToAppend += strPageVar;
				}
			}
			strCurLnkCase.Insert(0, strVarsToAppend);
			strCurLnkCase.Insert(0, strCurLnkCaseBegins);			
			strCurLnkCase += strCurLnkCaseEnds;
			strResult += strCurLnkCase;
		}		
	}
	strResult += strModifyVarsFuncEnds;	// ��������� ������� ModifyVars()

	// �������� ������� SaveGlobalVariables()...
	CString strSaveVarsFuncBegins;
	strSaveVarsFuncBegins.LoadString(IDS_STRING_SAVEVARS_FUNC_BEGINS);
	CString strSaveVarsFuncEnds;
	strSaveVarsFuncEnds.LoadString(IDS_STRING_SAVEVARS_FUNC_ENDS);
	CString strCurSaveStatement;
	strResult += strSaveVarsFuncBegins;
	posBookVarsMain = pDoc->m_lstBookVars.GetHeadPosition();
	iBookVarsMainCount = pDoc->m_lstBookVars.GetCount();	
	for (ibvIdx = 0; ibvIdx < iBookVarsMainCount; ibvIdx++)
	{
		 pBookVarCur = pDoc->m_lstBookVars.GetNext(posBookVarsMain);
		 if (pBookVarCur->Get_fIsVarGlobal())
		 {
			 strCurSaveStatement.Format(IDS_BOOKVAR_SAVE_STATEMENT, pBookVarCur->Get_strBookVarName());
			 strResult += strCurSaveStatement;	// �������� ���������� � ���������� ����������
		 }
	}		

	posStrVarsMain = pDoc->m_lstStrVars.GetHeadPosition();
	iStrVarsMainCount = pDoc->m_lstStrVars.GetCount();	
	for (isvIdx = 0; isvIdx < iStrVarsMainCount; isvIdx++)
	{
		 pStrVarCur = pDoc->m_lstStrVars.GetNext(posStrVarsMain);
		 
		 if (pStrVarCur->Get_fSaveInTheBookmarksFile())
		 {
			 strCurSaveStatement.Format(IDS_STRVAR_SAVE_STATEMENT, pStrVarCur->Get_strStrVarName());
			 strResult += strCurSaveStatement;	// �������� ���������� � ���������� ����������
		 }
	}	
	strResult += strSaveVarsFuncEnds;	// ��������� ������� SaveGlobalVariables()..

	// �������� ������� LoadGlobalVariables()...
	CString strLoadVarsFuncBegins;
	strLoadVarsFuncBegins.LoadString(IDS_STRING_LOADVARS_FUNC_BEGINS);
	CString strLoadVarsFuncEnds;
	strLoadVarsFuncEnds.LoadString(IDS_STRING_LOADVARS_FUNC_ENDS);
	CString strCurLoadStatement;
	posBookVarsMain = pDoc->m_lstBookVars.GetHeadPosition();
	iBookVarsMainCount = pDoc->m_lstBookVars.GetCount();
	strResult += strLoadVarsFuncBegins;
	for (ibvIdx = 0; ibvIdx < iBookVarsMainCount; ibvIdx++)
	{
		 pBookVarCur = pDoc->m_lstBookVars.GetNext(posBookVarsMain);
		 if (pBookVarCur->Get_fIsVarGlobal())
		 {
			 strCurLoadStatement.Format(IDS_BOOKVAR_LOAD_STATEMENT, pBookVarCur->Get_strBookVarName());
			 strResult += strCurLoadStatement;	// �������� ���������� � ���������� ����������
		 }
	}		
	
	posStrVarsMain = pDoc->m_lstStrVars.GetHeadPosition();
	iStrVarsMainCount = pDoc->m_lstStrVars.GetCount();	
	for (isvIdx = 0; isvIdx < iStrVarsMainCount; isvIdx++)
	{
		 pStrVarCur = pDoc->m_lstStrVars.GetNext(posStrVarsMain);
		 
		 if (pStrVarCur->Get_fSaveInTheBookmarksFile())
		 {
			 strCurLoadStatement.Format(IDS_STRVAR_LOAD_STATEMENT, pStrVarCur->Get_strStrVarName());
			 strResult += strCurLoadStatement;	// �������� ���������� � ���������� ����������
		 }
	}	
	strResult += strLoadVarsFuncEnds; // ��������� ������� LoadGlobalVariables().

	return TRUE;	// ������, ��� OK
}

void CNLB_BldrView::OnFileExportTo_NLB_Engine10() 
{	
	SaveStringToFile_NLB_Engine10();	
}

VOID CNLB_BldrView::SaveStringToFile_NLB_Engine10()
{		
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);		
	BOOL fConvertSuccess;
	CString strResult;
	CString str_NLB_Conv_Result;
	CString strTitle;	// ������, ������� ����� ��������� � <title> ... </title>
	BOOL fNeedToStartCompileOnError = TRUE;

	//������ ���������� ����� ������
	CString strFilter_NLB_Engine10;
	strFilter_NLB_Engine10.LoadString(IDS_FILTER_NLB_ENGINE_10);	

	//������������� ����� ����������� �����
	CString strFName_NLB_Engine10;
	strFName_NLB_Engine10.LoadString(IDS_FNAME_NLB_ENGINE_10);

	//������������� ���������� �� ��������� ����������� �����
	CString strExt_NLB_Engine10;
	strExt_NLB_Engine10.LoadString(IDS_EXT_NLB_ENGINE_10);

	CFileDialog dlg(FALSE, // Save As dialog box
		strExt_NLB_Engine10, // ���������� �� ���������
		strFName_NLB_Engine10, // ��� ����� �� ���������
		OFN_HIDEREADONLY | 
			OFN_OVERWRITEPROMPT | 			
			OFN_PATHMUSTEXIST,	// �����
		strFilter_NLB_Engine10, // ������
		this	// ��� ����� ����...
		);		

	if (dlg.DoModal() == IDOK)
	{
		CFileException ex;
		CFile FileToSave;		
		CString strCaption;
		TCHAR strErrorDescrptn[MAX_LOADSTRING];		
		HRSRC h_NLBE_10;
		HGLOBAL hGl_NLBE_10;
		char* pstr_NLBE_10;	// � �������� ����� ������ ������ ������ � ������� ANSI (����������� �������)
		TCHAR* sz_NLBE_10_Buf;
		int i_NLBE_10_Buf_Size;
		
		if (!FileToSave.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite, &ex))
		{
			// �������� �����������, � ��� ���� ������� ������������
			m_strMessage.LoadString(IDS_ERR_FILE_CORRUPT);
			ex.GetErrorMessage(strErrorDescrptn, MAX_LOADSTRING);
			m_strMessage += strErrorDescrptn;
			strCaption.LoadString(IDS_ERROR_MB_CAPT);

			MessageBox(m_strMessage, strCaption, MB_ICONERROR);
			return;
		}
		else
		{			
			pstr_NLBE_10 = NULL;
			h_NLBE_10 = FindResource(NULL, LPCTSTR(NLBE_10), _T("TEXT"));
			if (h_NLBE_10)
			{
				hGl_NLBE_10 = LoadResource(NULL, h_NLBE_10);
				if (hGl_NLBE_10)
				{
					pstr_NLBE_10 = (char*) LockResource(hGl_NLBE_10);
				}
			}

			if (!pstr_NLBE_10)
			{			
				// �������� �����������, � ��� ���� ������� ������������
				m_strMessage.LoadString(IDS_ERR_CANNOT_RETRIEVE_NLBE_10_RESOURCE);
				strCaption.LoadString(IDS_ERROR_MB_CAPT);
				MessageBox(m_strMessage, strCaption, MB_ICONERROR);

				FileToSave.Close();	// ������� ��� ���������������, � ����� �������� �������� ����...			
				return;	// � ������� ������!
			}

			i_NLBE_10_Buf_Size = SizeofResource(NULL, h_NLBE_10);	// ��������� ������ � ������, � ��� ��� ��� ������������ ����������� �������, ��, ������, ����� ������
			if (i_NLBE_10_Buf_Size > 0)
			{				
				sz_NLBE_10_Buf = new TCHAR[i_NLBE_10_Buf_Size + 1];	// +1 - ��� ������������ ����
				if (sz_NLBE_10_Buf)
				{
					// � ���� ����� ����� ��� ������� ������� ������������� � �������� _UNICODE. �� �����, ���� ��� ����, ����� ��, ������ �������� � ����� ������?
					// (***) -- ������� ��� �� ��� �����... ��������, ��� ����� ���� ��������...
					for (int i = 0; i < i_NLBE_10_Buf_Size; i++)
					{
						sz_NLBE_10_Buf[i] = ((TCHAR) (pstr_NLBE_10[i]));
					}										

					sz_NLBE_10_Buf[i_NLBE_10_Buf_Size] = _T('\0');	// ����������� ���� ��������� � ����� ������
				}
			}
			else
				sz_NLBE_10_Buf = NULL;

			if (!sz_NLBE_10_Buf)
			{			
				// �������� �����������, � ��� ���� ������� ������������
				m_strMessage.LoadString(IDS_ERR_CANNOT_CREATE_NLBE_10_BUF);
				strCaption.LoadString(IDS_ERROR_MB_CAPT);
				MessageBox(m_strMessage, strCaption, MB_ICONERROR);

				FileToSave.Close();	// ������� ��� ���������������, � ����� �������� �������� ����...			
				return;	// � ������� ������!
			}

			BeginWaitCursor();
			fConvertSuccess = Convert_To_NLB_Engine_v10(str_NLB_Conv_Result, fNeedToStartCompileOnError);
			EndWaitCursor();
			if (!fConvertSuccess)
			{
				// ������ �����������!
				if (sz_NLBE_10_Buf) delete []sz_NLBE_10_Buf;
				FileToSave.Close();	// ������� ��� ���������������, � ����� �������� �������� ����...
				if (fNeedToStartCompileOnError) OnBuildCompile();	// ���� ��������� ������, ������� ���������� � ����������...
				return;	// � ������� ������!
			}

			LPVOID lpBuf;

			strTitle.Empty();
			if (pDoc)
			{
				if (pDoc->m_pStartPoint) 
				{
					strTitle = ((CBookPage*) pDoc->m_pStartPoint)->Get_strPageCaption();				
					strTitle.TrimLeft();
					strTitle.TrimRight();
				}
			}			
			// ���� �� �����, �������� ��������� �� ���������
			if (strTitle.GetLength() == 0) strTitle.LoadString(IDS_STRING_NLB_DEFAULT_TITLE);

			strResult.Format(sz_NLBE_10_Buf, strTitle, str_NLB_Conv_Result);
			try
			{
				lpBuf = (LPVOID) strResult.GetBuffer(strResult.GetLength());
			}
			catch (CMemoryException* pex)			
			{			
				// �������� �����������, � ��� ���� ������� ������������
				m_strMessage.LoadString(IDS_ERR_OUT_OF_MEMORY);				
				pex->GetErrorMessage(strErrorDescrptn, MAX_LOADSTRING);
				m_strMessage += strErrorDescrptn;
				strCaption.LoadString(IDS_ERROR_MB_CAPT);

				MessageBox(m_strMessage, strCaption, MB_ICONERROR);

				if (sz_NLBE_10_Buf) delete []sz_NLBE_10_Buf;
				FileToSave.Close();	// ������� ��� ���������������, � ����� �������� �������� ����...				
				return;	// � ������� ������!
			}

			try
			{
				BeginWaitCursor();
				FileToSave.Write(lpBuf, 
					strResult.GetLength()*sizeof(TCHAR)	// �.�. GetLength ���������� ����� ��������, � �� ���� (���������!)
					);
				EndWaitCursor();
			}
			catch (CFileException* pex)
			{
				EndWaitCursor();	// �������, �������� �� ����������
				// �������� �����������, � ��� ���� ������� ������������
				m_strMessage.LoadString(IDS_ERR_FILE_CORRUPT);
				pex->GetErrorMessage(strErrorDescrptn, MAX_LOADSTRING);
				m_strMessage += strErrorDescrptn;
				strCaption.LoadString(IDS_ERROR_MB_CAPT);

				MessageBox(m_strMessage, strCaption, MB_ICONERROR);
				// return; �� ��������, ��� ��� ������ ���� �������� ��� (*)
			}

			// (*)
			if (sz_NLBE_10_Buf) delete []sz_NLBE_10_Buf;
			strResult.ReleaseBuffer();
			FileToSave.Close();	
		}		
	}	
}

int CNLB_BldrView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CDC* pdc;	// �������� ������� ����	

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//���������� ��������� ������
	m_BrushOffscreen.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));
	m_BrushOffscreen_ForDrag.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen_ForDrag.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));
	pdc = GetDC();
	m_iVertRes = pdc->GetDeviceCaps(VERTRES);
	m_iHorzRes = pdc->GetDeviceCaps(HORZRES);	
	m_DCOffscreen.CreateCompatibleDC(pdc);
	m_DCOffscreen_ForDrag.CreateCompatibleDC(pdc);
	m_BmpOffscreen.CreateCompatibleBitmap(pdc,m_iHorzRes,m_iVertRes);
	m_BmpOffscreen_ForDrag.CreateCompatibleBitmap(pdc,m_iHorzRes,m_iVertRes);
	m_DCOffscreen.SelectObject(&m_BmpOffscreen);
	m_DCOffscreen_ForDrag.SelectObject(&m_BmpOffscreen_ForDrag);
	m_DCOffscreen.SelectObject(&m_BrushOffscreen);
	m_DCOffscreen_ForDrag.SelectObject(&m_BrushOffscreen_ForDrag);
	m_DCOffscreen.SelectObject(&m_PenOffscreen);
	m_DCOffscreen_ForDrag.SelectObject(&m_PenOffscreen_ForDrag);
	m_DCOffscreen.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	m_DCOffscreen_ForDrag.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	ReleaseDC(pdc);		

	return 0;
}

void CNLB_BldrView::OnDestroy() 
{
	CView::OnDestroy();
	
	//m_DCOffscreen.SelectObject(m_pBmpOld); - �� �����, � MFC ����� ���� ����� �������!!!
	m_BmpOffscreen.DeleteObject();	
	m_BrushOffscreen.DeleteObject();	
	m_PenOffscreen.DeleteObject();
	m_DCOffscreen.DeleteDC();
	
	m_BmpOffscreen_ForDrag.DeleteObject();	
	m_BrushOffscreen_ForDrag.DeleteObject();	
	m_PenOffscreen_ForDrag.DeleteObject();
	m_DCOffscreen_ForDrag.DeleteDC();	
}

void CNLB_BldrView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	m_iClientWidth = cx;
	m_iClientHeight = cy;	
	
	UpdateSB();
}

BOOL CNLB_BldrView::OnEraseBkgnd(CDC* pDC) 
{	
	//return CView::OnEraseBkgnd(pDC); - �������
	return TRUE;	// ���� ��� ������ :-)
}

VOID CNLB_BldrView::CountSelRctCoords(RECT& rct)
{
	// ��������������, ��� m_ptBeginDragSelBox � m_ptEndDragSelBox ���������
	BOOL FlagX = (m_ptBeginDragSelBox.x < m_ptEndDragSelBox.x);
	BOOL FlagY = (m_ptBeginDragSelBox.y < m_ptEndDragSelBox.y);
	
	rct.left = FlagX ? m_ptBeginDragSelBox.x : m_ptEndDragSelBox.x;
	rct.top = FlagY ? m_ptBeginDragSelBox.y : m_ptEndDragSelBox.y;
	rct.right = FlagX ? m_ptEndDragSelBox.x : m_ptBeginDragSelBox.x;
	rct.bottom = FlagY ? m_ptEndDragSelBox.y : m_ptBeginDragSelBox.y;
}

VOID CNLB_BldrView::PaintInMemory(BOOL fPaintOnScreenImmediately /*= TRUE*/)
{
	CBookPage* pPageCur;
	CLink* pCurLnk;
	POSITION pos;
	DWORD dwStyle;
	int iPagesCount;
	int iLnksCount;
	int i;	// ��� ����� for
	RECT RctClient;	// ���� ������ (���������� ������� ����)

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_fptBeginDragSet && m_fptEndDragSet)
	{
		CRect rct;
		CountSelRctCoords(rct);
		m_DCOffscreen.DrawFocusRect(&rct);		// ���� ������ ������. � ��� �� LBUTTONUP ��� � ��������� �������.
	}
	else
	{	
		RctClient.left = 0;
		RctClient.top = 0;
		RctClient.right = m_iClientWidth;
		RctClient.bottom = m_iClientHeight;		

		// ������� �������� � ������ ��������, ���� ��� �������� � ������� ������� ������
		pos = pDoc->m_lstPages.GetHeadPosition();	
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (i=0; i<iPagesCount; i++)
		{			
			pPageCur = pDoc->m_lstPages.GetAt(pos);
			if (pPageCur->IsIntersect(RctClient) || // ���� �������� � ������� �������
				pPageCur->NeedToEraseWhenNotVisible())	// ��� ��, �����, ���� �� ��������, �� ������� � ������ ������� ��� �� ����
			{
				pPageCur->NeedToEraseWhenNotVisible() = FALSE;	// ����� � ������ ���, ���� �������� �� �����, ��� �� ������� �� �� ������, � ������ ����� �� �����������.
				dwStyle = PARAM_ERASE;		
				if (pDoc->m_pStartPoint == pPageCur)
					dwStyle |= PARAM_IS_STARTPOINT;						
				
				pPageCur->Draw(&m_DCOffscreen, dwStyle);			
			}
			pDoc->m_lstPages.GetNext(pos);
		}	

		// ������� ���� � ������ ��������, ���� ��� �������� � ������� ������� ������
		pos = pDoc->m_lstLnks.GetHeadPosition();
		dwStyle = PARAM_ERASE;
		iLnksCount = pDoc->m_lstLnks.GetCount();
		for (i=0; i<iLnksCount; i++)
		{		
			pCurLnk = pDoc->m_lstLnks.GetAt(pos);
			if (pCurLnk->IsIntersect(RctClient) || // ���� �������� � ������� �������
				pCurLnk->NeedToEraseWhenNotVisible())	// ��� ��, �����, ���� �� ��������, �� ������� � ������ ������� ��� �� ����
			{
				pCurLnk->NeedToEraseWhenNotVisible() = FALSE;	// ����� � ������ ���, ���� �������� �� �����, ��� �� ������� �� �� ������, � ������ ����� �� �����������.
				pCurLnk->Draw(&m_DCOffscreen, dwStyle);
			}
			pDoc->m_lstLnks.GetNext(pos);
		}

		// ������ �������� � ����� ��������, ���� ��� �������� � ������� ������� ������
		// ���� ��������, �������� �� ����, ����� ������� �� ��������� ���������� � ������ �� �������.
		pos = pDoc->m_lstPages.GetHeadPosition();
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (i=0; i<iPagesCount; i++)
		{			
			BOOL fNeedToDraw;

			pPageCur = pDoc->m_lstPages.GetAt(pos);

			if (pPageCur->IsIntersect(RctClient))
			{				
				pPageCur->NeedToEraseWhenNotVisible() = TRUE;	// ���� ��������, �������� �� ����, ����� ������� �� ��������� ����������.
				// ���� �� �� ��������, �� ��� �������� ���� ���������, � � ��������� ��� �� ����� ���� ��������, ��� �������� ������.
				dwStyle = PARAM_DRAW;
				if (m_fIsDraggingInProgress)
					fNeedToDraw = !pPageCur->IsSelected();
				else if (m_fIsResizingNow)
					fNeedToDraw = (pPageCur != (CBookPage*) m_pLastSelectedBookItem);			
				else
					fNeedToDraw = TRUE;

				if (fNeedToDraw)
				{
					if (pDoc->m_pStartPoint == pPageCur)
						dwStyle |= PARAM_IS_STARTPOINT;		
					
					if (pPageCur->IsSelected()) dwStyle |= PARAM_DRAW_FOCUS;			
					
					pPageCur->Draw(&m_DCOffscreen, dwStyle);			
				}
			}			

			pDoc->m_lstPages.GetNext(pos);
		}

		// ������ ���� � ����� ��������, ���� ��� �������� � ������� ������� ������
		pos = pDoc->m_lstLnks.GetHeadPosition();			
		iLnksCount = pDoc->m_lstLnks.GetCount();
		for (i=0; i<iLnksCount; i++)
		{		
			BOOL fNeedToDraw;
			
			pCurLnk = pDoc->m_lstLnks.GetAt(pos);

			if (pCurLnk->IsIntersect(RctClient))
			{
				pCurLnk->NeedToEraseWhenNotVisible() = TRUE;	// ���� ��������, �������� �� ����, ����� ������� �� ��������� ����������.
				// ���� �� �� ��������, �� ��� �������� ���� ���������, � � ��������� ��� �� ����� ���� ��������, ��� �������� ������.
				dwStyle = PARAM_DRAW;
				if (m_fIsDraggingInProgress)
					fNeedToDraw = !pCurLnk->Get_pPageFrom()->IsSelected() && !pCurLnk->Get_pPageTo()->IsSelected() ;
				else if (m_fIsResizingNow)
					fNeedToDraw = ((pCurLnk->Get_pPageFrom() != (CBookPage*) m_pLastSelectedBookItem) &&
								   (pCurLnk->Get_pPageTo() != (CBookPage*) m_pLastSelectedBookItem));
				else 
					fNeedToDraw = TRUE;

				if (fNeedToDraw)		
				{	
					if (pCurLnk->IsSelected()) dwStyle |= PARAM_DRAW_FOCUS;			
					pCurLnk->Draw(&m_DCOffscreen, dwStyle);
				}
			}

			pDoc->m_lstLnks.GetNext(pos);
		}

		// ����� ����, �������� �������, �� ������� ������ �����, ��� ��� ������ ����!
		// ����� ���������� ���������. 	

		if (m_pLastSelectedBookItem &&	// ���� ���-�� �������...
				!m_fIsDraggingInProgress &&		// ...� ������ �� ���������������...
				!m_fIsResizingNow &&		// ...� ����� �� �������� �������
				m_pLastSelectedBookItem->IsIntersect(RctClient)) // � ����� ������� - ���� ������� �����
		{
			dwStyle = PARAM_DRAW | PARAM_DRAW_FOCUS;	
			if (pDoc->m_pStartPoint == m_pLastSelectedBookItem)
				dwStyle |= PARAM_IS_STARTPOINT;		
			m_pLastSelectedBookItem->Draw(&m_DCOffscreen, dwStyle);
		}	
		// NeedToEraseWhenNotVisible ��� ������� �� ����, ��� ��� ���� ��� ���� ����������,
		// ��� ��� ���� ������� � ����� �� ������ ����
	}

	if (fPaintOnScreenImmediately) Invalidate();
}

void CNLB_BldrView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// ������ ���
	m_DCOffscreen.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	m_pPageFrom = m_pPageTo = NULL;
	itSelItemType = SelItem_None;
	tb_Add_Option = OptAdd_None;
	m_pLastSelectedBookItem = NULL;	// ������ ������ �� �������!	
	// �������� ������ ���������� �� ����	
	m_fIsDraggingStarted = FALSE;
	m_fIsDraggingInProgress = FALSE;
	m_fIsResizingNow = FALSE;	// �������� ������ ���� �� ����
	m_ixOffset = m_iyOffset = 0;
	UpdateSB();	// ���� ����������� � �� �����������...
	PaintInMemory(TRUE);	// ���� ���������� ��������� ����������, ������ ����� ������� ������������ �������� ������...
}

int CNLB_BldrView::GetMaximumPageXCoord()
{
	POSITION pos;
	int iPagesCount;
	CBookPage* pCurPage;
	int iResult;
	int iCurCoord;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	iResult = iCurCoord = 0;
	pos = pDoc->m_lstPages.GetHeadPosition();
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (int i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		iCurCoord = pCurPage->Get_iLeft() + pCurPage->Get_iWidth();		
		if ((iCurCoord > iResult)||(i==0)) iResult = iCurCoord;
		pDoc->m_lstPages.GetNext(pos);
	}	

	iResult += I_ADDITIONAL_SPACE_FOR_X;	// ���� �������� ���� ��������, ����� ����� ���� �������� ����� ��������
	 	
	if (m_iClientWidth > iResult) iResult = m_iClientWidth;
	iResult += m_ixOffset - 1;
	return iResult;
}

int CNLB_BldrView::GetMinimumPageXCoord()
{
	POSITION pos;
	int iPagesCount;
	CBookPage* pCurPage;
	int iResult;
	int iCurCoord;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	iResult = iCurCoord = 0;
	pos = pDoc->m_lstPages.GetHeadPosition();
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (int i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		iCurCoord = pCurPage->Get_iLeft();
		if ((iCurCoord < iResult)||(i==0)) iResult = iCurCoord;
		pDoc->m_lstPages.GetNext(pos);
	}	
	
	iResult -= I_ADDITIONAL_SPACE_FOR_X;	// ���� �������� ���� ��������, ����� ����� ���� �������� ����� ��������

	if (iResult > 0) iResult = 0;
	iResult += m_ixOffset;
	return iResult;
}

int CNLB_BldrView::GetMaximumPageYCoord()
{
	POSITION pos;
	int iPagesCount;
	CBookPage* pCurPage;
	int iResult;
	int iCurCoord;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	iResult = iCurCoord = 0;
	pos = pDoc->m_lstPages.GetHeadPosition();
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (int i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		iCurCoord = pCurPage->Get_iTop() + pCurPage->Get_iHeight();
		if ((iCurCoord > iResult)||(i==0)) iResult = iCurCoord;
		pDoc->m_lstPages.GetNext(pos);
	}	
	
	iResult += I_ADDITIONAL_SPACE_FOR_Y;	// ���� �������� ���� ��������, ����� ����� ���� �������� ����� ��������

	if (m_iClientHeight > iResult) iResult = m_iClientHeight;
	iResult += m_iyOffset - 1;
	return iResult;
}

int CNLB_BldrView::GetMinimumPageYCoord()
{
	POSITION pos;
	int iPagesCount;
	CBookPage* pCurPage;
	int iResult;
	int iCurCoord;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	iResult = iCurCoord = 0;
	pos = pDoc->m_lstPages.GetHeadPosition();
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (int i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		iCurCoord = pCurPage->Get_iTop();
		if ((iCurCoord < iResult)||(i==0)) iResult = iCurCoord;
		pDoc->m_lstPages.GetNext(pos);
	}	
	
	iResult -= I_ADDITIONAL_SPACE_FOR_Y;	// ���� �������� ���� ��������, ����� ����� ���� �������� ����� ��������

	if (iResult > 0) iResult = 0;
	iResult += m_iyOffset;
	return iResult;
}

VOID CNLB_BldrView::UpdateSB()
// �������� ����������
{	
	m_siVert.nMax = GetMaximumPageYCoord();
	m_siVert.nMin = GetMinimumPageYCoord();
	m_siVert.nPage = m_iClientHeight;
	m_siVert.nPos = m_iyOffset;			
	if ((m_siVert.nMax <= m_iClientHeight + m_iyOffset - 1 + I_ADDITIONAL_SPACE_FOR_Y) && 
		(m_siVert.nMin >= m_iyOffset - I_ADDITIONAL_SPACE_FOR_Y))
		ShowScrollBar(SB_VERT, FALSE);	// ���� �� �� �����, ������� ���
	else
	{
		// ���� �� �����, �������� ����������� � ������� ���		
		SetScrollInfo(SB_VERT, &m_siVert, TRUE);
		ShowScrollBar(SB_VERT, TRUE);	
	}
		
	m_siHorz.nMax = GetMaximumPageXCoord();
	m_siHorz.nMin = GetMinimumPageXCoord();
	m_siHorz.nPage = m_iClientWidth;
	m_siHorz.nPos = m_ixOffset;	
	if ((m_siHorz.nMax <= m_iClientWidth + m_ixOffset - 1 + I_ADDITIONAL_SPACE_FOR_X) && 
		(m_siHorz.nMin >= m_ixOffset - I_ADDITIONAL_SPACE_FOR_X))
		ShowScrollBar(SB_HORZ, FALSE);	// ���� �� �� �����, ������� ���
	else
	{
		// ���� �� �����, �������� ����������� � ������� ���		
		SetScrollInfo(SB_HORZ, &m_siHorz, TRUE);
		ShowScrollBar(SB_HORZ, TRUE);	
	}
}

VOID CNLB_BldrView::ShiftAllPagesHorz(int iShift)
{
	POSITION pos;
	int iPagesCount;
	CBookPage* pCurPage;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pos = pDoc->m_lstPages.GetHeadPosition();
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (int i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		pCurPage->Get_iLeft() += iShift;
		pDoc->m_lstPages.GetNext(pos);
	}	
}

VOID CNLB_BldrView::ShiftAllPagesVert(int iShift)
{
	POSITION pos;
	int iPagesCount;
	CBookPage* pCurPage;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	pos = pDoc->m_lstPages.GetHeadPosition();
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (int i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		pCurPage->Get_iTop() += iShift;
		pDoc->m_lstPages.GetNext(pos);
	}	
}

void CNLB_BldrView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode)
	{
	case SB_LEFT: 
		{			
			if (m_ixOffset > m_siHorz.nMin)
			{
				ShiftAllPagesHorz(m_ixOffset - m_siHorz.nMin);
				m_ixOffset = m_siHorz.nMin;
			}
		} 
		break;
	case SB_RIGHT: 
		{
			if (m_ixOffset <= m_siHorz.nMax - m_iClientWidth)
			{
				ShiftAllPagesHorz(m_ixOffset + m_iClientWidth - m_siHorz.nMax - 1);
				m_ixOffset = m_siHorz.nMax - m_iClientWidth + 1;
			}
		} 
		break;
	case SB_LINELEFT: 
		{
			if (m_ixOffset > m_siHorz.nMin)
			{
				if (m_ixOffset - I_ONE_LINE_SCROLL_VALUE > m_siHorz.nMin) 
				{
					ShiftAllPagesHorz(I_ONE_LINE_SCROLL_VALUE);
					m_ixOffset-=I_ONE_LINE_SCROLL_VALUE;
				}
				else 
				{
					ShiftAllPagesHorz(m_ixOffset - m_siHorz.nMin);
					m_ixOffset = m_siHorz.nMin;
				}
			}			
		} 
		break;
	case SB_LINERIGHT:
		{
			if (m_ixOffset <= m_siHorz.nMax - m_iClientWidth)
			{
				if (m_ixOffset + m_iClientWidth + I_ONE_LINE_SCROLL_VALUE < m_siHorz.nMax) 
				{
					ShiftAllPagesHorz(-I_ONE_LINE_SCROLL_VALUE);
					m_ixOffset+=I_ONE_LINE_SCROLL_VALUE;
				}
				else
				{	
					ShiftAllPagesHorz(m_ixOffset + m_iClientWidth - m_siHorz.nMax - 1);
					m_ixOffset = m_siHorz.nMax - m_iClientWidth + 1;
				}
			}			
		} 
		break;
	case SB_PAGELEFT: 
		{
			if (m_ixOffset > m_siHorz.nMin)
			{
				if (m_ixOffset - m_iClientWidth > m_siHorz.nMin) 
				{
					ShiftAllPagesHorz(m_iClientWidth);
					m_ixOffset -= m_iClientWidth;
				}
				else
				{
					ShiftAllPagesHorz(m_ixOffset - m_siHorz.nMin);
					m_ixOffset = m_siHorz.nMin;
				}
			}
		} 
		break;
	case SB_PAGERIGHT:
		{
			if (m_ixOffset <= m_siHorz.nMax - m_iClientWidth)
			{
				if (m_ixOffset + m_iClientWidth + m_iClientWidth < m_siHorz.nMax) 
				{
					ShiftAllPagesHorz(-m_iClientWidth);
					m_ixOffset += m_iClientWidth;
				}
				else
				{
					ShiftAllPagesHorz(m_ixOffset + m_iClientWidth - m_siHorz.nMax - 1);
					m_ixOffset = m_siHorz.nMax - m_iClientWidth + 1;
				}
			}
		} 
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		{
			int ixOffsetOld = m_ixOffset;
			int iDiff;
			UINT uiOldMask = m_siHorz.fMask;
			GetScrollInfo(SB_HORZ, &m_siHorz, SIF_TRACKPOS);
			m_siHorz.fMask = uiOldMask;	// ��� ��� ���������� ������� ����� ������!
			m_ixOffset = m_siHorz.nTrackPos;	// ��� ��������, ��� ����� nPos. � ���� ������ nPos ������!!!
			iDiff = ixOffsetOld - m_ixOffset;			
			ShiftAllPagesHorz(iDiff);			
		}
        break;
    default:
        break;

	}

	m_siHorz.nPos = m_ixOffset;			 
	SetScrollInfo(SB_HORZ, &m_siHorz, TRUE);
	PaintInMemory();
	
	CView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CNLB_BldrView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode)
	{
	case SB_TOP: 
		{			
			if (m_iyOffset > m_siVert.nMin)
			{
				ShiftAllPagesVert(m_iyOffset - m_siVert.nMin);
				m_iyOffset = m_siVert.nMin;
			}
		} 
		break;
	case SB_BOTTOM: 
		{
			if (m_iyOffset <= m_siVert.nMax - m_iClientHeight)
			{
				ShiftAllPagesVert(m_iyOffset + m_iClientHeight - m_siVert.nMax - 1);
				m_iyOffset = m_siVert.nMax - m_iClientHeight + 1;
			}
		} 
		break;
	case SB_LINEUP: 
		{
			if (m_iyOffset > m_siVert.nMin)
			{
				if (m_iyOffset - I_ONE_LINE_SCROLL_VALUE > m_siVert.nMin) 
				{
					ShiftAllPagesVert(I_ONE_LINE_SCROLL_VALUE);
					m_iyOffset-=I_ONE_LINE_SCROLL_VALUE;
				}
				else 
				{
					ShiftAllPagesVert(m_iyOffset - m_siVert.nMin);
					m_iyOffset = m_siVert.nMin;
				}
			}			
		} 
		break;
	case SB_LINEDOWN:
		{
			if (m_iyOffset <= m_siVert.nMax - m_iClientHeight)
			{
				if (m_iyOffset + m_iClientHeight + I_ONE_LINE_SCROLL_VALUE < m_siVert.nMax) 
				{
					ShiftAllPagesVert(-I_ONE_LINE_SCROLL_VALUE);
					m_iyOffset+=I_ONE_LINE_SCROLL_VALUE;
				}
				else
				{	
					ShiftAllPagesVert(m_iyOffset + m_iClientHeight - m_siVert.nMax - 1);
					m_iyOffset = m_siVert.nMax - m_iClientHeight + 1;
				}
			}			
		} 
		break;
	case SB_PAGEUP: 
		{
			if (m_iyOffset > m_siVert.nMin)
			{
				if (m_iyOffset - m_iClientHeight > m_siVert.nMin) 
				{
					ShiftAllPagesVert(m_iClientHeight);
					m_iyOffset -= m_iClientHeight;
				}
				else
				{
					ShiftAllPagesVert(m_iyOffset - m_siVert.nMin);
					m_iyOffset = m_siVert.nMin;
				}
			}
		} 
		break;
	case SB_PAGEDOWN:
		{
			if (m_iyOffset <= m_siVert.nMax - m_iClientHeight)
			{
				if (m_iyOffset + m_iClientHeight + m_iClientHeight < m_siVert.nMax) 
				{
					ShiftAllPagesVert(-m_iClientHeight);
					m_iyOffset += m_iClientHeight;
				}
				else
				{
					ShiftAllPagesVert(m_iyOffset + m_iClientHeight - m_siVert.nMax - 1);
					m_iyOffset = m_siVert.nMax - m_iClientHeight + 1;
				}
			}
		} 
		break;

	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		{
			int iyOffsetOld = m_iyOffset;
			int iDiff;			
			UINT uiOldMask = m_siVert.fMask;
			GetScrollInfo(SB_VERT, &m_siVert, SIF_TRACKPOS);
			m_siVert.fMask = uiOldMask;		// ��� ��� ���������� ������� ����� ������!
			m_iyOffset = m_siVert.nTrackPos;	// ��� ��������, ��� ����� nPos. � ���� ������ nPos ������!!!
			iDiff = iyOffsetOld - m_iyOffset;			
			ShiftAllPagesVert(iDiff);			
		}
        break;
    default:
        break;

	}

	
	m_siVert.nPos = m_iyOffset;		
	SetScrollInfo(SB_VERT, &m_siVert, TRUE);
	PaintInMemory();
	
	CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CNLB_BldrView::OnSysColorChange()
{	
	CView::OnSysColorChange();

	// ����� ������ ����� � ����...		
	m_BrushOffscreen.DeleteObject();	
	m_PenOffscreen.DeleteObject();
	
	m_BrushOffscreen_ForDrag.DeleteObject();	
	m_PenOffscreen_ForDrag.DeleteObject();

	// ...c������� ����� ����� � ����...
	m_BrushOffscreen.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));

	m_BrushOffscreen_ForDrag.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen_ForDrag.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));

	// ...� ������� �� � ���������...
	m_DCOffscreen.SelectObject(&m_BrushOffscreen);
	m_DCOffscreen.SelectObject(&m_PenOffscreen);

	m_DCOffscreen_ForDrag.SelectObject(&m_BrushOffscreen_ForDrag);
	m_DCOffscreen_ForDrag.SelectObject(&m_PenOffscreen_ForDrag);

	// ������� ���� ���� �� ���� ���������...
	m_DCOffscreen.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	m_DCOffscreen_ForDrag.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);

	// � ��������� ��������!
	PaintInMemory();
}

VOID CNLB_BldrView::ClearInMemory()
{
	m_DCOffscreen.PatBlt(0,0,m_iClientWidth,m_iClientHeight,PATCOPY);
	m_DCOffscreen_ForDrag.PatBlt(0,0,m_iClientWidth,m_iClientHeight,PATCOPY);
}

LRESULT CNLB_BldrView::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	CDC* pdc;	// �������� ������� ����	

	// ����� ��� ������...	
	m_BmpOffscreen.DeleteObject();	
	m_BrushOffscreen.DeleteObject();	
	m_PenOffscreen.DeleteObject();
	m_DCOffscreen.DeleteDC();
	
	m_BmpOffscreen_ForDrag.DeleteObject();	
	m_BrushOffscreen_ForDrag.DeleteObject();	
	m_PenOffscreen_ForDrag.DeleteObject();
	m_DCOffscreen_ForDrag.DeleteDC();

	// � ������� ��� ������!
	//���������� ��������� ������
	m_BrushOffscreen.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));
	m_BrushOffscreen_ForDrag.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen_ForDrag.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));
	pdc = GetDC();
	m_iVertRes = pdc->GetDeviceCaps(VERTRES);
	m_iHorzRes = pdc->GetDeviceCaps(HORZRES);	
	m_DCOffscreen.CreateCompatibleDC(pdc);
	m_DCOffscreen_ForDrag.CreateCompatibleDC(pdc);
	m_BmpOffscreen.CreateCompatibleBitmap(pdc,m_iHorzRes,m_iVertRes);
	m_BmpOffscreen_ForDrag.CreateCompatibleBitmap(pdc,m_iHorzRes,m_iVertRes);
	m_DCOffscreen.SelectObject(&m_BmpOffscreen);
	m_DCOffscreen_ForDrag.SelectObject(&m_BmpOffscreen_ForDrag);
	m_DCOffscreen.SelectObject(&m_BrushOffscreen);
	m_DCOffscreen_ForDrag.SelectObject(&m_BrushOffscreen_ForDrag);
	m_DCOffscreen.SelectObject(&m_PenOffscreen);
	m_DCOffscreen_ForDrag.SelectObject(&m_PenOffscreen_ForDrag);
	m_DCOffscreen.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	m_DCOffscreen_ForDrag.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	ReleaseDC(pdc);

	// � ��������� ��������!
	PaintInMemory();
	return 0;
}

void CNLB_BldrView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// ������-��, ����� ���� ����� ��������� ��� � ���������� ���������� �����, �.�. � �� �������� ������... �� ��� ��� � �����!
	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))	// ��������� � �������������� ��������...
	{
		OnEditEditpageEditproperties();
	}
	else if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_Link))	// ��������� � �������������� ������...
	{
		OnEditEditlinkEditproperties();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CNLB_BldrView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_DELETE:	// ����� ������� �������, �� ������� ����� �����
		{
			CNLB_BldrDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);

			if (pDoc->m_iSelectedPagesCount != 0)	// ���-�� �������� � ��������� => ��������� � �������� �������...
			{
				OnEditEditpageDeletepage();
			}
			else if (m_pLastSelectedBookItem &&
				(itSelItemType == SelItem_Link))	// ��������� � �������� ������...
			{
				OnEditEditlinkDeletelink();
			}
		}
		break;
	case VK_SPACE:
	case VK_RETURN:	// ����� ������������� �������, �� ������� ����� �����	
		{
			if (m_pLastSelectedBookItem &&
				(itSelItemType == SelItem_BookPage))	// ��������� � �������������� ��������...
			{
				OnEditEditpageEditproperties();
			}
			else if (m_pLastSelectedBookItem &&
				(itSelItemType == SelItem_Link))	// ��������� � �������������� ������...
			{
				OnEditEditlinkEditproperties();
			}
		}
		break;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNLB_BldrView::OnBuildCompile() 
{
	BeginWaitCursor();	
	POSITION pos;
	int i;
	int iPagesCount;
	int iLnksCount;
	int iBookVarsCount;
	int iCompileResult;
	CString strCompilerErrorMessage;
	BOOL fOk = true;	// ����� �����������
	CLink* pCurLnk;
	CBookVar* pCurBookVar;
	CNLB_BldrApp* pNLB_Bldr = (CNLB_BldrApp*) AfxGetApp();

	if (pNLB_Bldr)
	{
		CNLB_BldrDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		if (pDoc->m_pStartPoint)
		{
			if (m_fIsDialogComperrActivated) m_pDlgComperr->OnOkDialogComperr();	// ���� ������ ������ ��� ������ �� ������ Compile, ������� ������� ���...	
			// ...� ��� �� �������� ������!
			m_pDlgComperr->Create(IDD_DIALOG_COMPERR, this);	// m_pDlgComperr != NULL
			// �����, �� � �� �����������, �� ����� ������������. ���� �� �� �����, � ����� ���� ������� �� ����� ���� � ��������� ����.

			pos = pDoc->m_lstLnks.GetHeadPosition();	
			
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (i = 0; i < iLnksCount; i++)
			{					
				pCurLnk = pDoc->m_lstLnks.GetAt(pos);
				
				iCompileResult = CompileBooleanFormula(pNLB_Bldr,
					pDoc,
					pCurLnk->Get_strLinkConstraints(),
					&strCompilerErrorMessage,
					NULL);

				if (iCompileResult != IERR_ERROR_SUCCESS) 
				{
					int ciItems;					
					fOk = FALSE;
					S_CompoundLVItemData* pclid;
							
					if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
					pclid->cliType = CLI_Link;
					pclid->lpCLIObject = (LPVOID) pCurLnk;

					ciItems=m_pDlgComperr->m_lstCompErr.GetItemCount();	
					m_pDlgComperr->m_lstCompErr.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
						ciItems,
						pCurLnk->Get_strLinkCaption(),
						0,
						0,
						m_pDlgComperr->m_iIconPosLink,
						(LPARAM) pclid);		
					m_pDlgComperr->m_lstCompErr.SetItemText(ciItems, 1, strCompilerErrorMessage);
				}
				else
				{
					CList<CBookVar*, CBookVar*&> lstBookVarsForCurPage;

					E_ILType ilResult = CheckLinkContainInfiniteLoop(pCurLnk->Get_strLinkConstraints(), lstBookVarsForCurPage);

					if (ilResult != E_IL_OK)
					{
						int ciItems;					
						fOk = FALSE;					
						S_CompoundLVItemData* pclid;
							
						if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
						pclid->cliType = CLI_Link;
						pclid->lpCLIObject = (LPVOID) pCurLnk;
								
						ciItems=m_pDlgComperr->m_lstCompErr.GetItemCount();	
						m_pDlgComperr->m_lstCompErr.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
							ciItems,
							pCurLnk->Get_strLinkCaption(),
							0,
							0,
							m_pDlgComperr->m_iIconPosLink,
							(LPARAM) pclid);		
						if (ilResult == E_IL_ILEXISTS)
							strCompilerErrorMessage.LoadString(IDS_ERR_IL_SHORT_MESSAGE);
						else if (ilResult == E_IL_APP_ERROR)
							strCompilerErrorMessage.LoadString(IDS_ERROR_CALLING_CHECK_LINK_CONTAINS_IL);

						m_pDlgComperr->m_lstCompErr.SetItemText(ciItems, 1, strCompilerErrorMessage);
					}
				}			

				pDoc->m_lstLnks.GetNext(pos);
			}

			pos = pDoc->m_lstBookVars.GetHeadPosition();	
			
			iBookVarsCount = pDoc->m_lstBookVars.GetCount();
			for (i = 0; i < iBookVarsCount; i++)
			{					
				pCurBookVar = pDoc->m_lstBookVars.GetAt(pos);
				
				iCompileResult = CompileBooleanFormula(pNLB_Bldr,
					pDoc,
					pCurBookVar->Get_strBookVarBody(),
					&strCompilerErrorMessage,
					NULL);

				if (iCompileResult != IERR_ERROR_SUCCESS) 
				{
					int ciItems;					
					fOk = FALSE;							
					S_CompoundLVItemData* pclid;
							
					if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
					pclid->cliType = CLI_BookVar;
					pclid->lpCLIObject = (LPVOID) pCurBookVar;
							
					ciItems=m_pDlgComperr->m_lstCompErr.GetItemCount();	
					m_pDlgComperr->m_lstCompErr.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
						ciItems,
						pCurBookVar->Get_strBookVarName(),
						0,
						0,
						m_pDlgComperr->m_iIconPosBookVar,
						(LPARAM) pclid);		
					m_pDlgComperr->m_lstCompErr.SetItemText(ciItems, 1, strCompilerErrorMessage);
				}
				else
				{
					CList<CBookVar*, CBookVar*&> lstBookVarsForCurPage;
					E_ILType ilResult = CheckLinkContainInfiniteLoop(pCurBookVar->Get_strBookVarBody(), lstBookVarsForCurPage);

					if (ilResult != E_IL_OK)
					{
						int ciItems;					
						fOk = FALSE;						
						S_CompoundLVItemData* pclid;
							
						if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
						pclid->cliType = CLI_BookVar;
						pclid->lpCLIObject = (LPVOID) pCurBookVar;
								
						ciItems=m_pDlgComperr->m_lstCompErr.GetItemCount();	
						m_pDlgComperr->m_lstCompErr.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
							ciItems,
							pCurBookVar->Get_strBookVarName(),
							0,
							0,
							m_pDlgComperr->m_iIconPosBookVar,
							(LPARAM) pclid);		
						if (ilResult == E_IL_ILEXISTS)
							strCompilerErrorMessage.LoadString(IDS_ERR_IL_SHORT_MESSAGE);
						else if (ilResult == E_IL_APP_ERROR)
							strCompilerErrorMessage.LoadString(IDS_ERROR_CALLING_CHECK_LINK_CONTAINS_IL);

						m_pDlgComperr->m_lstCompErr.SetItemText(ciItems, 1, strCompilerErrorMessage);
					}
				}			

				pDoc->m_lstBookVars.GetNext(pos);
			}

			if (fOk)
			{		
				CString strSuccessText;
				
				m_pDlgComperr->DestroyWindow();	// ������ �� ������� ������ �� �����������!		
				iPagesCount = pDoc->m_lstPages.GetCount();
				iLnksCount = pDoc->m_lstLnks.GetCount();
				strSuccessText.Format(IDS_COMPILE_SUCCESSFUL, iPagesCount, iLnksCount);
				MessageBox(strSuccessText);		
			}
			else
			{
				m_pDlgComperr->ShowWindow(SW_SHOW);	// �������� ������... (m_pDlg != NULL)
				m_fIsDialogComperrActivated = TRUE;	// ������ �� ������������� ������!
			}
		}
		else	// Startpoint ����� �����������!!!
		{
			CString strCaption;
			m_strMessage.LoadString(IDS_STARTPOINT_NEEDED);	
			strCaption.LoadString(IDS_ERROR_MB_CAPT);
			MessageBox(m_strMessage, strCaption, MB_ICONEXCLAMATION);
		}
	}

	EndWaitCursor();
}

void CNLB_BldrView::EditlinkEditproperties()
{OnEditEditlinkEditproperties(); }

void CNLB_BldrView::EditpageEditproperties()
{{OnEditEditpageEditproperties(); }}

void CNLB_BldrView::OnEditSelectall() 
{	
	SelectAll();	
	PaintInMemory();	// ���� ��� ���������� ��� ���, ���� ������ ���������� ��������� �����
}

VOID CNLB_BldrView::SelectAll()
{
	BeginWaitCursor();
	POSITION posnew;	// ������� ��� ��������� �������
	int i;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	// ������� ������� �� ���� ������ ������� � ���				
	posnew = pDoc->m_lstPages.GetHeadPosition();	
	int iPagesCount = pDoc->m_lstPages.GetCount();
	for (i=0; i<iPagesCount; i++)
	{						
		pDoc->m_lstPages.GetAt(posnew)->Select();	// ��������� m_iSelectedPagesCount ����� � �����, ������ ��� �� �������!
		pDoc->m_lstPages.GetNext(posnew);
	}	
	
	posnew = pDoc->m_lstLnks.GetHeadPosition();		
	int iLnksCount = pDoc->m_lstLnks.GetCount();
	for (i=0; i<iLnksCount; i++)
	{		
		pDoc->m_lstLnks.GetAt(posnew)->Select();
		pDoc->m_lstLnks.GetNext(posnew);
	}
	pDoc->m_iSelectedPagesCount = iPagesCount;	// ��� �������� ��������!
	EndWaitCursor();
}	

VOID CNLB_BldrView::DeselectAll()
{
	BeginWaitCursor();
	POSITION posnew;	// ������� ��� ������ �������
	int i;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	// ������� ������� �� ���� ������ ������� � ���				
	posnew = pDoc->m_lstPages.GetHeadPosition();	
	int iPagesCount = pDoc->m_lstPages.GetCount();
	for (i=0; i<iPagesCount; i++)
	{						
		pDoc->m_lstPages.GetAt(posnew)->Deselect();	// ��������� m_iSelectedPagesCount ����� � �����, ������ ��� �� �������!
		pDoc->m_lstPages.GetNext(posnew);
	}	
	
	posnew = pDoc->m_lstLnks.GetHeadPosition();		
	int iLnksCount = pDoc->m_lstLnks.GetCount();
	for (i=0; i<iLnksCount; i++)
	{		
		pDoc->m_lstLnks.GetAt(posnew)->Deselect();
		pDoc->m_lstLnks.GetNext(posnew);
	}
	pDoc->m_iSelectedPagesCount = 0;	// ��� ������� �����
	EndWaitCursor();
}


void CNLB_BldrView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	
    pCmdUI->Enable(pDoc->m_iSelectedPagesCount > 0 ? TRUE : FALSE);	
}

void CNLB_BldrView::OnEditCopy() 
{	
	BeginWaitCursor();
	int iPagesCount;
	int iLnksCount;
	int iSavedLnksCount;
	int iTopMin;
	int iLeftMin;
	BOOL fFirstFound = TRUE;
	CBookPage* pCurPage;
	CLink* pCurLnk;
	POSITION pos;
	int i;
	CNLB_BldrDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);	
    ASSERT(pDoc->m_iSelectedPagesCount > 0);    
    // Create a memory-file-based archive.
    CSharedFile mf (GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
    CArchive ar(&mf, CArchive::store);  

	pos = pDoc->m_lstPages.GetHeadPosition();			
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		if (pCurPage->IsSelected()) 
		{
			if (fFirstFound)
			{
				iTopMin = pCurPage->Get_iTop();
				iLeftMin = pCurPage->Get_iLeft();
				fFirstFound = FALSE;
			}
			else
			{
				if (pCurPage->Get_iTop() < iTopMin) iTopMin = pCurPage->Get_iTop();
				if (pCurPage->Get_iLeft() < iLeftMin) iLeftMin = pCurPage->Get_iLeft();
			}			
		}
		pDoc->m_lstPages.GetNext(pos);
	}		
	ar << iTopMin;
	ar << iLeftMin;
	ar << pDoc->m_iSelectedPagesCount;
	iSavedLnksCount = 0;
	pos = pDoc->m_lstLnks.GetHeadPosition();			
	iLnksCount = pDoc->m_lstLnks.GetCount();
	for (i=0; i<iLnksCount; i++)
	{		
		pCurLnk = pDoc->m_lstLnks.GetAt(pos);
		if (pCurLnk->IsSelected()) iSavedLnksCount++;
		pDoc->m_lstLnks.GetNext(pos);
	}
	ar << iSavedLnksCount;

	pos = pDoc->m_lstLnks.GetHeadPosition();			
	iLnksCount = pDoc->m_lstLnks.GetCount();
	for (i=0; i<iLnksCount; i++)
	{		
		pCurLnk = pDoc->m_lstLnks.GetAt(pos);
		if (pCurLnk->IsSelected()) 
		{
			// ������� ������ (������ ������� � ������)
			ar << pDoc->FindPageIndexByPointer(pCurLnk->Get_pPageFrom());
			ar << pDoc->FindPageIndexByPointer(pCurLnk->Get_pPageTo());
			// ��������� ��� ������� ����� �������������, ��� OK			
			pCurLnk->Serialize(ar);
		}
		pDoc->m_lstLnks.GetNext(pos);
	}	
	pos = pDoc->m_lstPages.GetHeadPosition();			
	iPagesCount = pDoc->m_lstPages.GetCount();
	for (i=0; i<iPagesCount; i++)
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		if (pCurPage->IsSelected()) 
		{
			ar << pDoc->FindPageIndexByPointer(pCurPage);	// ��������� ��� ������� ����� �������������, ��� OK
			pCurPage->Serialize(ar);
		}
		pDoc->m_lstPages.GetNext(pos);
	}		

    ar.Close(); // Flush and close.
    HGLOBAL hMem = mf.Detach();
    if (!hMem) return;    
    // Send the Clipboard the data.
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(((CNLB_BldrApp*) AfxGetApp())->m_uiNLBClipFormat, hMem);
    CloseClipboard();	
	EndWaitCursor();
}

void CNLB_BldrView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// See if there is a NLB available.
    OpenClipboard();
    UINT uiFmt = 0;
    while (uiFmt = EnumClipboardFormats(uiFmt)) {
        if (uiFmt == ((CNLB_BldrApp*) AfxGetApp())->m_uiNLBClipFormat) {
            CloseClipboard();
            pCmdUI->Enable(TRUE);
            return;
        }
    }
    pCmdUI->Enable(FALSE);
    CloseClipboard();
}

void CNLB_BldrView::OnEditPaste() 
{
	BeginWaitCursor();
	CBookPage* pPageToInsert;
	CBookPage* pPagePtrRead;
	CBookPage PageRead;
	CLink* pLinkToInsert;
	CBookPage* pLnkPageFromPtr;
	CBookPage* pLnkPageToPtr;
	CLink LnkRead;
	int iSavedPagesCount;
	int iSavedLnksCount;
	int iTopMin;
	int iLeftMin;	
	int i;
	CList<CLink*, CLink*&> lstLnksRead;
	POSITION pos;
	POSITION pos_remov;
	int iBufferForPtrs;
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	DeselectAll();	// ��� ��� ����� ����� Selection - ��, ��� ������ ����� ���������
	OpenClipboard();
    HGLOBAL hMem = ::GetClipboardData(((CNLB_BldrApp*) AfxGetApp())->m_uiNLBClipFormat);
    if (!hMem) {
        CloseClipboard();
        return;
    }
    // Create a mem file.
    CSharedFile mf;
    mf.SetHandle(hMem);
    // Create the archive and get the data.
    CArchive ar(&mf, CArchive::load);  
    
	ar >> iTopMin;
	ar >> iLeftMin;
	ar >> iSavedPagesCount;	
	ar >> iSavedLnksCount;
	for (i=0; i<iSavedLnksCount; i++)
	{			
		ar >> iBufferForPtrs;
		if (iBufferForPtrs == -1) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_CLIPBOARD_ERROR);
		pLnkPageFromPtr = (CBookPage*) iBufferForPtrs;
		ar >> iBufferForPtrs;
		if (iBufferForPtrs == -1) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_CLIPBOARD_ERROR);
		pLnkPageToPtr = (CBookPage*) iBufferForPtrs;		
		LnkRead.Serialize(ar);
		LnkRead.Get_pPageFrom() = pLnkPageFromPtr;
		LnkRead.Get_pPageTo() = pLnkPageToPtr;
		if ((pLinkToInsert = new CLink(LnkRead))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
		lstLnksRead.AddHead(pLinkToInsert);		
	}	
	for (i=0; i<iSavedPagesCount; i++)
	{	
		CString strInitialPageVar;
		DWORD dwCntr;

		/*
		����� ���� ����� ��������. ������� ��������, ������ �� ����������
		����������, ����������� � �������� � �� �� �����. ��������� 
		������������ ����������, ��� �����������. ������ ����������� ��������
		���������� �����������, � ���� ����� ��� ����, �� ��� ��������������.
		*/

		ar >> iBufferForPtrs;
		if (iBufferForPtrs == -1) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_CLIPBOARD_ERROR);
		pPagePtrRead = (CBookPage*) iBufferForPtrs;
		PageRead.Serialize(ar);
		PageRead.Get_iTop() -= iTopMin;
		PageRead.Get_iLeft() -= iLeftMin;
		strInitialPageVar = PageRead.Get_strPageVar();
		dwCntr = 0;
		while (pDoc->FindPageIndexByVar(PageRead.Get_strPageVar()) != -1)	// ���� ����� ���������� � ����� ��� ����...
		{
			// ����� ������� ����������
			PageRead.Get_strPageVar().Format(IDS_STRING_PAGEVAR_BUFFER_FOR_PASTE, 
				strInitialPageVar, 
				dwCntr++);
			if (dwCntr >= DW_MAX_DWORD)	// ������ ������������� ������� :-)
			{
				// � ���� ������ ������ ������� ���������� ������, ��� � ���!
				PageRead.Get_strPageVar() = _T("");
				break;
			}
		}

		if ((pPageToInsert = new CBookPage(PageRead))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
		// ����������� ��������� � ������ ������, ��� ��� ��� �������� ������ ��������		
		pDoc->RelinkPointers(lstLnksRead, pPagePtrRead, pPageToInsert);					
		pDoc->m_lstPages.AddHead(pPageToInsert);		
	}		

    ar.Close();
    mf.Detach();
    CloseClipboard();

	pDoc->m_iSelectedPagesCount = iSavedPagesCount;	// ����� ������� ����� ������ ���.
	pDoc->m_lstLnks.AddHead(&lstLnksRead);	// ������ ��� ������ ������������� � �� ����� �������� � �������� ������

	// ������� ��������������� ������...
	pos = lstLnksRead.GetHeadPosition();
	while (!lstLnksRead.IsEmpty())
	{
		pos_remov = pos;
		lstLnksRead.GetNext(pos);
		lstLnksRead.RemoveAt(pos_remov);
	}
	// ������� ��������������� ������ ���������, ��� ��� ��� ���� ������������ � RelinkPointers
	pos = pDoc->m_lstPosFromChanged.GetHeadPosition();
	while (!pDoc->m_lstPosFromChanged.IsEmpty())
	{
		pos_remov = pos;
		pDoc->m_lstPosFromChanged.GetNext(pos);
		pDoc->m_lstPosFromChanged.RemoveAt(pos_remov);
	}
	pos = pDoc->m_lstPosToChanged.GetHeadPosition();
	while (!pDoc->m_lstPosToChanged.IsEmpty())
	{
		pos_remov = pos;
		pDoc->m_lstPosToChanged.GetNext(pos);
		pDoc->m_lstPosToChanged.RemoveAt(pos_remov);
	}	

	pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...	
    pDoc->UpdateAllViews(NULL);	
	EndWaitCursor();
}

void CNLB_BldrView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	
    pCmdUI->Enable(pDoc->m_iSelectedPagesCount > 0 ? TRUE : FALSE);	
}

void CNLB_BldrView::OnEditCut() 
{	
	OnEditCopy();	// ����������� �������� � ��������...
	DeleteSelectedPages();	// ...� ������� �� �� �������
}

void CNLB_BldrView::OnEditBookvariables() 
{
	CDialogBookVars dlg;
	dlg.DoModal();
}

void CNLB_BldrView::OnEditLeafs() 
{
	CDialogLeafs dlg;
	dlg.DoModal();	
}

VOID CNLB_BldrView::ScrollToPage(CBookPage* pPage)
{
	if (pPage)
	{
		m_ixOffset = pPage->Get_iLeft();
		m_iyOffset = pPage->Get_iTop();
		SetScrollInfo(SB_VERT, &m_siVert, TRUE);
		SetScrollInfo(SB_HORZ, &m_siHorz, TRUE);
		ShiftAllPagesVert(-m_iyOffset + m_iClientHeight/2 - pPage->Get_iHeight()/2);
		ShiftAllPagesHorz(-m_ixOffset + m_iClientWidth/2 - pPage->Get_iWidth()/2);
		UpdateSB();
		PaintInMemory(TRUE);
	}
}

VOID CNLB_BldrView::ScrollToLink(CLink* pLink)
{
	if (pLink)
	{
		m_ixOffset = pLink->Get_pPageTo()->Get_iLeft();		
		m_iyOffset = pLink->Get_pPageTo()->Get_iTop();
		SetScrollInfo(SB_VERT, &m_siVert, TRUE);
		SetScrollInfo(SB_HORZ, &m_siHorz, TRUE);		 
		ShiftAllPagesVert(-m_iyOffset + m_iClientHeight/2 - pLink->Get_pPageTo()->Get_iHeight()/2);
		ShiftAllPagesHorz(-m_ixOffset + m_iClientWidth/2 - pLink->Get_pPageTo()->Get_iHeight()/2);		
		UpdateSB();
		PaintInMemory(TRUE);
	}
}

void CNLB_BldrView::OnEditProbabilityvariables() 
{
	CDialogProbability dlg;
	dlg.DoModal();
}

void CNLB_BldrView::OnEditFind() 
{	
	CNLB_BldrApp* pNLB_Bldr = (CNLB_BldrApp*) AfxGetApp();
	int ciItems;					
	BOOL fFound = FALSE;	// ����� ������������				
	POSITION pos;
	CString strToFindUppercase;
	int iStrLenStrToFind;

	int iPosFindFromStr[3];		
	BOOL fMatchLeft[3];
	BOOL fMatchRight[3];	
	CString* pStrWhere[3];	

	if (pNLB_Bldr)
	{		
		CDialogFindTxt dlg;

		if (dlg.DoModal() == IDOK)
		{
			CNLB_BldrDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			if (pDoc)
			{
				if (m_fIsDialogSearchResActivated) m_pDlgSearchRes->OnOkDialogSearchResults();	// ���� ������ ��� ������ �� ������, ������� ������� ���...	
				// ...� ��� �� �������� ������!
				m_pDlgSearchRes->Create(IDD_DIALOG_SEARCH_RESULTS, this);	// m_pDlgSearchRes != NULL
				// �����, �� � �� �����������, �� ����� ������������. ���� �� �� �����, � ����� ���� ������� �� ����� ���� � ��������� ����.
				
				BeginWaitCursor();

				if (!dlg.m_fCaseSensitive)	// ����� strToFindUppercase ��� ������ �� �����������
				{
					strToFindUppercase = dlg.m_strToFind;
					strToFindUppercase.MakeUpper();
				}

				if (dlg.m_fWholeWords)	// ����� iStrLenStrToFind ��� ������ �� �����������
				{
					iStrLenStrToFind = dlg.m_strToFind.GetLength();
				}

				if (dlg.m_fFindInPages)
				{
					CBookPage* pCurPage;	
					int iPagesCount;
					BOOL fPageFound;
					CString tmpStr1;
					CString tmpStr2;
					CString tmpStr3;					
					
					if (!dlg.m_fCaseSensitive)
					{
						pStrWhere[0] = &tmpStr1;
						pStrWhere[1] = &tmpStr2;
						pStrWhere[2] = &tmpStr3;
					}

					iPagesCount = pDoc->m_lstPages.GetCount();	
					pos = pDoc->m_lstPages.GetHeadPosition();
					for (int i = 0; i < iPagesCount; i++)
					{
						pCurPage = pDoc->m_lstPages.GetNext(pos);

						iPosFindFromStr[0] = 0;	
						iPosFindFromStr[1] = 0;
						iPosFindFromStr[2] = 0;
						if (dlg.m_fCaseSensitive)
						{			
							pStrWhere[0] = &(pCurPage->Get_strPageCaption());
							pStrWhere[1] = &(pCurPage->Get_strPageText());
							pStrWhere[2] = &(pCurPage->Get_strPageVar());
						}
						else
						{
							tmpStr1 = pCurPage->Get_strPageCaption();
							tmpStr2 = pCurPage->Get_strPageText();
							tmpStr3 = pCurPage->Get_strPageVar();

							tmpStr1.MakeUpper();
							tmpStr2.MakeUpper();
							tmpStr3.MakeUpper();
						}
						do
						{
							if (dlg.m_fCaseSensitive)
							{									
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = pStrWhere[0]->Find(dlg.m_strToFind, iPosFindFromStr[0]);
								if (iPosFindFromStr[1] != -1) iPosFindFromStr[1] = pStrWhere[1]->Find(dlg.m_strToFind, iPosFindFromStr[1]);
								if (iPosFindFromStr[2] != -1) iPosFindFromStr[2] = pStrWhere[2]->Find(dlg.m_strToFind, iPosFindFromStr[2]);													
							}
							else
							{								
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = tmpStr1.Find(strToFindUppercase, iPosFindFromStr[0]);
								if (iPosFindFromStr[1] != -1) iPosFindFromStr[1] = tmpStr2.Find(strToFindUppercase, iPosFindFromStr[1]);
								if (iPosFindFromStr[2] != -1) iPosFindFromStr[2] = tmpStr3.Find(strToFindUppercase, iPosFindFromStr[2]);							
							}

							fPageFound = ((iPosFindFromStr[0] != -1) ||
									(iPosFindFromStr[1] != -1) ||
									(iPosFindFromStr[2] != -1));

							if (!fPageFound) break;

							// �.�. ���� ������ ����, �� fPageFound == TRUE.�� ���� ���������, ��� ��� ����� ����� (���� ����) 
							if (dlg.m_fWholeWords)
							{
								for (int i = 0; i < 3; i++)
								{
									if (iPosFindFromStr[i] != -1)
									{										
										if (iPosFindFromStr[i] == 0) 
											fMatchLeft[i] = TRUE;
										else
											fMatchLeft[i] = _istspace(pStrWhere[i]->GetAt(iPosFindFromStr[i] - 1));
									
										if (iPosFindFromStr[i] + iStrLenStrToFind == pStrWhere[i]->GetLength())
											fMatchRight[i] = TRUE;
										else
											fMatchRight[i] = _istspace(pStrWhere[i]->GetAt(iPosFindFromStr[i] + iStrLenStrToFind));

										iPosFindFromStr[i]++;	// �� ������ ������ ����� ������������ � ������ ������
									}
									else
									{
										fMatchLeft[i] = FALSE;
										fMatchRight[i] = FALSE;
									}
								}		
								
								fPageFound = ((fMatchLeft[0] && fMatchRight[0]) ||
									(fMatchLeft[1] && fMatchRight[1]) ||
									(fMatchLeft[2] && fMatchRight[2]));

								if (fPageFound) break;	// ��� ������������� ����� �����!
								// � ����� ����� ����� �����, ��� �� ��������� �������� � ������!
							}
						} 
						while (dlg.m_fWholeWords);

						if (fPageFound)
						{
								fFound = TRUE;
								S_CompoundLVItemData* pclid;
										
								if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
								pclid->cliType = CLI_Page;
								pclid->lpCLIObject = (LPVOID) pCurPage;

								ciItems=m_pDlgSearchRes->m_lstResults.GetItemCount();	
								m_pDlgSearchRes->m_lstResults.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
									ciItems,
									pCurPage->Get_strPageCaption(),
									0,
									0,
									m_pDlgSearchRes->m_iIconPosPage,
									(LPARAM) pclid);
						}
					}
				}

				if (dlg.m_fFindInLnks)
				{
					CLink* pCurLnk;
					int iLnksCount;
					BOOL fLinkFound;
					CString tmpStr1;
					CString tmpStr2;					

					if (!dlg.m_fCaseSensitive)
					{
						pStrWhere[0] = &tmpStr1;
						pStrWhere[1] = &tmpStr2;					
					}

					iLnksCount = pDoc->m_lstLnks.GetCount();	
					pos = pDoc->m_lstLnks.GetHeadPosition();
					for (int i = 0; i < iLnksCount; i++)
					{
						pCurLnk = pDoc->m_lstLnks.GetNext(pos);							
							
						iPosFindFromStr[0] = 0;	
						iPosFindFromStr[1] = 0;						
						if (dlg.m_fCaseSensitive)
						{			
							pStrWhere[0] = &(pCurLnk->Get_strLinkCaption());
							pStrWhere[1] = &(pCurLnk->Get_strLinkConstraints());								
						}
						else
						{
							tmpStr1 = pCurLnk->Get_strLinkCaption();
							tmpStr2 = pCurLnk->Get_strLinkConstraints();

							tmpStr1.MakeUpper();
							tmpStr2.MakeUpper();								
						}
						do
						{
							if (dlg.m_fCaseSensitive)
							{									
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = pStrWhere[0]->Find(dlg.m_strToFind, iPosFindFromStr[0]);
								if (iPosFindFromStr[1] != -1) iPosFindFromStr[1] = pStrWhere[1]->Find(dlg.m_strToFind, iPosFindFromStr[1]);												
							}
							else
							{																
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = tmpStr1.Find(strToFindUppercase, iPosFindFromStr[0]);
								if (iPosFindFromStr[1] != -1) iPosFindFromStr[1] = tmpStr2.Find(strToFindUppercase, iPosFindFromStr[1]);								
							}

							fLinkFound = ((iPosFindFromStr[0] != -1) || (iPosFindFromStr[1] != -1));

							if (!fLinkFound) break;

							// �.�. ���� ������ ����, �� fLinkFound == TRUE.�� ���� ���������, ��� ��� ����� ����� (���� ����) 
							if (dlg.m_fWholeWords)
							{
								for (int i = 0; i < 2; i++)
								{
									if (iPosFindFromStr[i] != -1)
									{										
										if (iPosFindFromStr[i] == 0) 
											fMatchLeft[i] = TRUE;
										else
											fMatchLeft[i] = _istspace(pStrWhere[i]->GetAt(iPosFindFromStr[i] - 1));
									
										if (iPosFindFromStr[i] + iStrLenStrToFind == pStrWhere[i]->GetLength())
											fMatchRight[i] = TRUE;
										else
											fMatchRight[i] = _istspace(pStrWhere[i]->GetAt(iPosFindFromStr[i] + iStrLenStrToFind));

										iPosFindFromStr[i]++;	// �� ������ ������ ����� ������������ � ������ ������
									}
									else
									{
										fMatchLeft[i] = FALSE;
										fMatchRight[i] = FALSE;
									}									
								}		
								
								fLinkFound = ((fMatchLeft[0] && fMatchRight[0]) ||
									(fMatchLeft[1] && fMatchRight[1]));

								if (fLinkFound) break;	// ��� ������������� ����� �����!
							}
						} 
						while (dlg.m_fWholeWords);

						if (fLinkFound)
						{
								fFound = TRUE;
								S_CompoundLVItemData* pclid;
										
								if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
								pclid->cliType = CLI_Link;
								pclid->lpCLIObject = (LPVOID) pCurLnk;

								ciItems=m_pDlgSearchRes->m_lstResults.GetItemCount();	
								m_pDlgSearchRes->m_lstResults.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
									ciItems,
									pCurLnk->Get_strLinkCaption(),
									0,
									0,
									m_pDlgSearchRes->m_iIconPosLink,
									(LPARAM) pclid);
						}
					}
				}

				if (dlg.m_fFindInBookVars)
				{
					CBookVar* pCurBookVar;
					int iBookVarsCount;
					BOOL fBookVarFound;
					CString tmpStr1;
					CString tmpStr2;

					if (!dlg.m_fCaseSensitive)
					{
						pStrWhere[0] = &tmpStr1;
						pStrWhere[1] = &tmpStr2;					
					}

					iBookVarsCount = pDoc->m_lstBookVars.GetCount();	
					pos = pDoc->m_lstBookVars.GetHeadPosition();
					for (int i = 0; i < iBookVarsCount; i++)
					{
						pCurBookVar = pDoc->m_lstBookVars.GetNext(pos);						

						iPosFindFromStr[0] = 0;	
						iPosFindFromStr[1] = 0;						
						if (dlg.m_fCaseSensitive)
						{			
							pStrWhere[0] = &(pCurBookVar->Get_strBookVarName());
							pStrWhere[1] = &(pCurBookVar->Get_strBookVarBody());								
						}
						else
						{
							tmpStr1 = pCurBookVar->Get_strBookVarName();
							tmpStr2 = pCurBookVar->Get_strBookVarBody();

							tmpStr1.MakeUpper();
							tmpStr2.MakeUpper();								
						}
						do
						{
							if (dlg.m_fCaseSensitive)
							{																	
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = pStrWhere[0]->Find(dlg.m_strToFind, iPosFindFromStr[0]);
								if (iPosFindFromStr[1] != -1) iPosFindFromStr[1] = pStrWhere[1]->Find(dlg.m_strToFind, iPosFindFromStr[1]);									
							}
							else
							{																
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = tmpStr1.Find(strToFindUppercase, iPosFindFromStr[0]);
								if (iPosFindFromStr[1] != -1) iPosFindFromStr[1] = tmpStr2.Find(strToFindUppercase, iPosFindFromStr[1]);																
							}

							fBookVarFound = ((iPosFindFromStr[0] != -1) || (iPosFindFromStr[1] != -1));

							if (!fBookVarFound) break;

							// �.�. ���� ������ ����, �� fBookVarFound == TRUE.�� ���� ���������, ��� ��� ����� ����� (���� ����) 
							if (dlg.m_fWholeWords)
							{
								for (int i = 0; i < 2; i++)
								{
									if (iPosFindFromStr[i] != -1)
									{										
										if (iPosFindFromStr[i] == 0) 
											fMatchLeft[i] = TRUE;
										else
											fMatchLeft[i] = _istspace(pStrWhere[i]->GetAt(iPosFindFromStr[i] - 1));
									
										if (iPosFindFromStr[i] + iStrLenStrToFind == pStrWhere[i]->GetLength())
											fMatchRight[i] = TRUE;
										else
											fMatchRight[i] = _istspace(pStrWhere[i]->GetAt(iPosFindFromStr[i] + iStrLenStrToFind));

										iPosFindFromStr[i]++;	// �� ������ ������ ����� ������������ � ������ ������
									}
									else
									{
										fMatchLeft[i] = FALSE;
										fMatchRight[i] = FALSE;
									}
								}		
								
								fBookVarFound = ((fMatchLeft[0] && fMatchRight[0]) ||
									(fMatchLeft[1] && fMatchRight[1]));

								if (fBookVarFound) break;	// ��� ������������� ����� �����!
							}
						} 
						while (dlg.m_fWholeWords);

						if (fBookVarFound)
						{
								fFound = TRUE;
								S_CompoundLVItemData* pclid;
										
								if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
								pclid->cliType = CLI_BookVar;
								pclid->lpCLIObject = (LPVOID) pCurBookVar;

								ciItems=m_pDlgSearchRes->m_lstResults.GetItemCount();	
								m_pDlgSearchRes->m_lstResults.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
									ciItems,
									pCurBookVar->Get_strBookVarName(),
									0,
									0,
									m_pDlgSearchRes->m_iIconPosBookVar,
									(LPARAM) pclid);
						}
					}
				}

				if (dlg.m_fFindInProbVars)
				{
					CProbVar* pCurProbVar;
					int iProbVarsCount;
					BOOL fProbVarFound;
					CString tmpStr1;					

					if (!dlg.m_fCaseSensitive)
					{
						pStrWhere[0] = &tmpStr1;					
					}

					iProbVarsCount = pDoc->m_lstProbVars.GetCount();	
					pos = pDoc->m_lstProbVars.GetHeadPosition();
					for (int i = 0; i < iProbVarsCount; i++)
					{
						pCurProbVar = pDoc->m_lstProbVars.GetNext(pos);

						iPosFindFromStr[0] = 0;							
						if (dlg.m_fCaseSensitive)
						{			
							pStrWhere[0] = &(pCurProbVar->Get_strProbVarName());								
						}
						else
						{
							tmpStr1 = pCurProbVar->Get_strProbVarName();								
							tmpStr1.MakeUpper();								
						}
						do
						{
							if (dlg.m_fCaseSensitive)
							{									
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = pStrWhere[0]->Find(dlg.m_strToFind, iPosFindFromStr[0]);								
							}
							else
							{																
								if (iPosFindFromStr[0] != -1) iPosFindFromStr[0] = tmpStr1.Find(strToFindUppercase, iPosFindFromStr[0]);
							}

							fProbVarFound = (iPosFindFromStr[0] != -1);

							if (!fProbVarFound) break;

							// �.�. ���� ������ ����, �� fProbVarFound == TRUE.�� ���� ���������, ��� ��� ����� ����� (���� ����) 
							if (dlg.m_fWholeWords)
							{
								
								if (iPosFindFromStr[0] != -1)
								{										
									if (iPosFindFromStr[0] == 0) 
										fMatchLeft[0] = TRUE;
									else
										fMatchLeft[0] = _istspace(pStrWhere[i]->GetAt(iPosFindFromStr[0] - 1));
								
									if (iPosFindFromStr[0] + iStrLenStrToFind == pStrWhere[0]->GetLength())
										fMatchRight[0] = TRUE;
									else
										fMatchRight[0] = _istspace(pStrWhere[0]->GetAt(iPosFindFromStr[0] + iStrLenStrToFind));

									iPosFindFromStr[i]++;	// �� ������ ������ ����� ������������ � ������ ������
								}
								else
								{
									fMatchLeft[0] = FALSE;
									fMatchRight[0] = FALSE;
								}								
								
								fProbVarFound = (fMatchLeft[0] && fMatchRight[0]);

								if (fProbVarFound) break;	// ��� ������������� ����� �����!
							}
						} 
						while (dlg.m_fWholeWords);						

						if (fProbVarFound)
						{
								fFound = TRUE;
								S_CompoundLVItemData* pclid;
										
								if ((pclid = new S_CompoundLVItemData) == NULL) pNLB_Bldr->HandleError(IERR_OUT_OF_MEMORY);
								pclid->cliType = CLI_ProbVar;
								pclid->lpCLIObject = (LPVOID) pCurProbVar;

								ciItems=m_pDlgSearchRes->m_lstResults.GetItemCount();	
								m_pDlgSearchRes->m_lstResults.InsertItem(LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT,
									ciItems,
									pCurProbVar->Get_strProbVarName(),
									0,
									0,
									m_pDlgSearchRes->m_iIconPosProbVar,
									(LPARAM) pclid);
						}
					}
				}
				EndWaitCursor();

				if (fFound)
				{
					m_pDlgSearchRes->ShowWindow(SW_SHOW);	// �������� ���������� ������... (m_pDlgSearchRes != NULL)
					m_fIsDialogSearchResActivated = TRUE;	// ������ �� ������������� ������!
				}
				else
				{			
					CString strTextNotFound;
					strTextNotFound.LoadString(IDS_TEXT_NOT_FOUND);
					m_pDlgSearchRes->DestroyWindow();	// ������ �� �����������!											
					MessageBox(strTextNotFound);		
				}
			}
		}
	}	
}

void CNLB_BldrView::OnUpdateEditAddstartpoint(CCmdUI* pCmdUI) 
{
	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);	
}

void CNLB_BldrView::OnUpdateEditLeafs(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if (pDoc->m_lstPages.IsEmpty())
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);	
}

void CNLB_BldrView::OnUpdateBuildCompile(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if (pDoc->m_lstPages.IsEmpty())
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);		
}

void CNLB_BldrView::OnUpdateFileExporttoNlbenginev10(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if (pDoc->m_lstPages.IsEmpty())
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);		
}

void CNLB_BldrView::OnUpdateEditAddlink(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if (pDoc->m_lstPages.GetCount() < 2)	// ��� ����, ����� �������� ������, ����� ��� ������� ��� ��������
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);		
}

void CNLB_BldrView::OnUpdateEditSelectall(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if (pDoc->m_lstPages.IsEmpty())
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);		
}

void CNLB_BldrView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if ((pDoc->m_lstPages.IsEmpty()) && 
			(pDoc->m_lstLnks.IsEmpty()) &&
			(pDoc->m_lstBookVars.IsEmpty()) &&
			(pDoc->m_lstProbVars.IsEmpty()))
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);	
}

void CNLB_BldrView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if ((pDoc->m_lstPages.IsEmpty()) && 
			(pDoc->m_lstLnks.IsEmpty()) &&
			(pDoc->m_lstBookVars.IsEmpty()) &&
			(pDoc->m_lstProbVars.IsEmpty()) &&
			(pDoc->m_lstStrVars.IsEmpty()))
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);		
}

void CNLB_BldrView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc)
	{
		if ((pDoc->m_lstPages.IsEmpty()) && 
			(pDoc->m_lstLnks.IsEmpty()) &&
			(pDoc->m_lstBookVars.IsEmpty()) &&
			(pDoc->m_lstProbVars.IsEmpty()))
			pCmdUI->Enable(FALSE);	
		else
			pCmdUI->Enable(TRUE);	
	}
	else
		pCmdUI->Enable(FALSE);		
}

BOOL CNLB_BldrView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{	
	WPARAM wParam;	
	wParam = SB_THUMBPOSITION;	// ������-�� � ������� ����� ���� ������ �����, �� ������� �������� ������� ����������, �� �.�. �� ���������� SetScrollPos(), �� �� ��� ����� ������
	if (nFlags & MK_CONTROL)
	{
		m_siHorz.nPos -= zDelta;		
		SetScrollInfo(SB_HORZ, &m_siHorz, TRUE);			
		SendMessage(WM_HSCROLL, wParam, 0);
	}
	else
	{
		m_siVert.nPos -= zDelta;		
		SetScrollInfo(SB_VERT, &m_siVert, TRUE);		
		SendMessage(WM_VSCROLL, wParam, 0);
	}
	
	// return CView::OnMouseWheel(nFlags, zDelta, pt); - ����� ���� ��������, ���� ��������� ��� ���� � ��������� ���� ����� �������� ������� ����������. �� ����� �� �����!
	return TRUE;	// Nonzero if mouse wheel scrolling is enabled; otherwise 0.
}

void CNLB_BldrView::OnEditChangeallpages() 
{
	int i, j, iLnksCount, iPagesCount;
	POSITION pos, pos_pages, pos_remov;
	CBookPage* pCurPage;
	CLink* pLinkCur;
	CDialogPageProperties dlg(this);
	S_PLink_LCounter_And_LStrings* plcas;
	int iDoModalRes;
	BOOL fNeedBreak = FALSE;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	iPagesCount = pDoc->m_lstPages.GetCount();	
	pos_pages = pDoc->m_lstPages.GetHeadPosition();
	dlg.m_fAbortMassOperationFlag = FALSE;	// ��� �������� � ��������� ������ Abort mass operation
	dlg.m_iPagesTotal = iPagesCount;
	j = 0;
	while (j < iPagesCount)
	{
		pCurPage = pDoc->m_lstPages.GetAt(pos_pages);
		ScrollToPage(pCurPage);
		
		dlg.m_iPagesProcessed = j + 1;
		dlg.m_strPageCaption = pCurPage->Get_strPageCaption();
		dlg.m_strPageText = pCurPage->Get_strPageText();
		dlg.m_strPageVar = pCurPage->Get_strPageVar();
		dlg.m_fUsePageCaption = pCurPage->Get_UsePageCaption();
		dlg.m_txtColor = pCurPage->Get_cr_Txt_Color();
		pCurPage->GetLogBrush(&dlg.m_lBrush);
		pCurPage->GetLogPen(&dlg.m_lPen);				

		// dlg.m_ListLinksFrom ���� ����, ��� ��� ������ ������ ��� ������
		pos = pDoc->m_lstLnks.GetHeadPosition();
		i = 0;
		iLnksCount = pDoc->m_lstLnks.GetCount();
		while (i < iLnksCount)	
		{		
			pLinkCur = pDoc->m_lstLnks.GetAt(pos);
			if (pLinkCur->Get_pPageFrom() == pCurPage)
			{
				plcas = new S_PLink_LCounter_And_LStrings;
				plcas->pLink = pLinkCur;
				plcas->iOldLinkNumber = pLinkCur->Get_iLinkNumber();
				plcas->lpszLinkCaptionForLV = new TCHAR[MAX_LOADSTRING];
				plcas->lpszLinkConstraintsForLV = new TCHAR[MAX_LOADSTRING];
				dlg.m_ListLinksFrom.AddTail(plcas);			
			}

			pDoc->m_lstLnks.GetNext(pos);
			i++; 
		}

		iDoModalRes = dlg.DoModal();
		if (iDoModalRes == IDOK)
		{
			pCurPage->Get_strPageCaption() = dlg.m_strPageCaption;
			pCurPage->Get_strPageText() = dlg.m_strPageText;
			pCurPage->Get_strPageVar() = dlg.m_strPageVar;
			pCurPage->Get_UsePageCaption() = dlg.m_fUsePageCaption;
			pCurPage->Get_cr_Txt_Color() = dlg.m_txtColor;
			pCurPage->SetLogBrush(&dlg.m_lBrush);
			pCurPage->SetLogPen(&dlg.m_lPen);
			pDoc->SetModifiedFlag(TRUE);	// �������� ��� �������...
			PaintInMemory();
		}
		else if (iDoModalRes == IDCANCEL)
		{
			if (dlg.m_fAbortMassOperationFlag) fNeedBreak = TRUE;
		}

		// � ����� ������ ������� ��������������� ������ �������
		pos = dlg.m_ListLinksFrom.GetHeadPosition();
		while (!(dlg.m_ListLinksFrom.IsEmpty()))	
		{					
			pos_remov = pos;
			plcas = dlg.m_ListLinksFrom.GetNext(pos);
			if (plcas)
			{				
				if (plcas->lpszLinkCaptionForLV) delete []plcas->lpszLinkCaptionForLV;
				if (plcas->lpszLinkConstraintsForLV) delete []plcas->lpszLinkConstraintsForLV;
				// plcas->pLink �� �������!!!
				delete plcas;
			}
			dlg.m_ListLinksFrom.RemoveAt(pos_remov);
		}

		if (fNeedBreak) break;	// �������� ���� ��������� ��������

		if (dlg.m_fGoPrevPage)
		{
			if (j > 0)
			{
				pDoc->m_lstPages.GetPrev(pos_pages);
				j--;
			}
		}
		else
		{
			pDoc->m_lstPages.GetNext(pos_pages);
			j++;
		}
	}	
}

void CNLB_BldrView::OnUpdateEditChangeallpages(CCmdUI* pCmdUI) 
{
	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int iPagesCount = pDoc->m_lstPages.GetCount();
	if (iPagesCount > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CNLB_BldrView::OnEditStringvariables() 
{
	CDialogStrVars dlg;
	dlg.DoModal();	
}

void CNLB_BldrView::OnEditTriggeredstrings() 
{
	CDialogTriggeredStrings dlg;
	dlg.DoModal();	
}
