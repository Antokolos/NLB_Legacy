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
	// m_PenOffscreen, m_pPenOld, m_pBrushOld - инициализация на 
	// OnCreate, разрушение на OnDestroy

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
	m_fMLeaved = TRUE;	// Чтобы при первом же нажатии на левую кнопку иметь возможность отследить WM_MOUSELEAVE
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
					if (pCurPage->IsIntersect(RctClient))	// Рисуем только тогда, когда страница попадает в видимую область экрана
					{
						pCurPage->NeedToEraseWhenNotVisible() = TRUE;	// Если попадают, поставим им флаг, чтобы стереть на следующей прорисовке.
						// Если же не попадают, то при стирании флаг обнулится, и в следующий раз не будет даже стирания, что ускоряет работу.
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

			// Рисуем дуги в новых позициях
			pos = pDoc->m_lstLnks.GetHeadPosition();			
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				dwStyle = PARAM_DRAW;
				pCurLnk = pDoc->m_lstLnks.GetAt(pos);

				// Рисуем только нужные дуги, которые, к тому же, видимы на экране.
				// Дуги, не попадающие в видимую зону экрана, игнорируем.
				if ((pCurLnk->Get_pPageFrom()->IsSelected()) ||
					(pCurLnk->Get_pPageTo()->IsSelected()))					
				{		
					if (pCurLnk->IsIntersect(RctClient))
					{
						pCurLnk->NeedToEraseWhenNotVisible() = TRUE;	// Если попадают, поставим им флаг, чтобы стереть на следующей прорисовке.
						// Если же не попадают, то при стирании флаг обнулится, и в следующий раз не будет даже стирания, что ускоряет работу.
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

			// Рисуем дуги в новых позициях
			pos = pDoc->m_lstLnks.GetHeadPosition();			
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				dwStyle = PARAM_DRAW;
				pCurLnk = pDoc->m_lstLnks.GetAt(pos);

				// Рисуем только нужные дуги, которые, к тому же, видимы на экране.
				// Дуги, не попадающие в видимую зону экрана, игнорируем.
				if ((pCurLnk->Get_pPageFrom() == pCurPage) ||
					(pCurLnk->Get_pPageTo() == pCurPage))
				{	
					if (pCurLnk->IsIntersect(RctClient))
					{
						pCurLnk->NeedToEraseWhenNotVisible() = TRUE;	// Если попадают, поставим им флаг, чтобы стереть на следующей прорисовке.
						// Если же не попадают, то при стирании флаг обнулится, и в следующий раз не будет даже стирания, что ускоряет работу.
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
			
			if (pCurPage->IsIntersect(RctClient))	// Рисуем только тогда, когда попадает в видимую область
			{
				pCurPage->NeedToEraseWhenNotVisible() = TRUE;	// Если попадают, поставим им флаг, чтобы стереть на следующей прорисовке.
				// Если же не попадают, то при стирании флаг обнулится, и в следующий раз не будет даже стирания, что ускоряет работу.
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
		OnLButtonUp(0, ptZero);		// Как будто была отпущена левая кнопка...
	}
	m_fMLeaved = TRUE;
	return 0;
}

void CNLB_BldrView::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CBookPage* pPageCur;
	CLink* pLinkCur;
	m_pLastSelectedBookItem = NULL;	// Будем пессимистами
	m_fIsDraggingStarted = FALSE;
	m_fIsDraggingInProgress = FALSE;
	itSelItemType = SelItem_None;
	int iPagesCount;
	int iLnksCount;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	
	
	if (m_fMLeaved) 
	{
		InitializeTmeForLeave();	// Чтобы сразу же иметь возможность получать нотификацию WM_MOUSELEAVE
		m_fMLeaved = FALSE;
	}

	m_fptBeginDragSet = FALSE;	// Будем пессимистами

	m_fMouseDownNow = TRUE;
	if (tb_Add_Option == OptAdd_BookPage)
	{
		CBookPage* pPage;
		CString str;
		str.LoadString(IDS_STR_DEFAULT_PAGE_CAPT);
		CBookPage page(str, point.y, point.x);
		if ((pPage = new CBookPage(page))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
		pDoc->m_lstPages.AddHead(pPage);
		pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
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
				if (pPage!=m_pPageFrom)	// Нельзя начинать и заканчивать дугу на одной и той же странице!
				{
					CLink* pLinkToAdd;
					CString strDefaultLinkCapt;
					int iLinkNumber = 0;

					strDefaultLinkCapt.LoadString(IDS_STR_DEFAULT_LINK_CAPT);					
					m_pPageTo = pPage;

					// Определяем порядковый номер этой ссылки - она будет последней по порядку среди всех существующих
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
					if (!pDoc->IsExistLink(lnk)) // Если такой еще нет...
					{
						if ((pLinkToAdd = new CLink(lnk))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
						if (pLinkToAdd->Get_pPageFrom()->IsSelected() && 
							pLinkToAdd->Get_pPageTo()->IsSelected())
							pLinkToAdd->Select();	// ...то пометим ее, если это надо...
						pDoc->m_lstLnks.AddHead(pLinkToAdd);	// ...и добавим
						pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
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
	else	// Будет произведено перетаскивание объекта
	{
		// m_pLastSelectedBookItem = NULL; - эта команда уже была выше!
		// Сначала пробуем пометить элемент как страницу
		POSITION pos = pDoc->m_lstPages.GetHeadPosition();
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (int i=0; i<iPagesCount; i++)
		{		
			pPageCur = pDoc->m_lstPages.GetAt(pos);
			if (pPageCur->IsPointInside(point))
			{
				m_pLastSelectedBookItem = pPageCur;
				m_fIsDraggingStarted = TRUE;
				m_iOffsDragX = 0;	// Стрижка только начата...
				m_iOffsDragY = 0;	// Стрижка только начата...
				itSelItemType = SelItem_BookPage;

				if (!pPageCur->IsSelected())	// Она не была выбрана, значит, она становится выбранной, а с остальных выделение снимается
				{
					DeselectAll();
					pPageCur->Select();	// Теперь она выбрана	
					pDoc->m_iSelectedPagesCount = 1;	// Теперь помечена только одна страница!
				}

				ptStartDrag = point;				
				break;
			}
			pDoc->m_lstPages.GetNext(pos);
		}

		if (!m_pLastSelectedBookItem)
		{
			// Если не вышло, пробуем пометить элемент как ссылку
			POSITION pos = pDoc->m_lstLnks.GetHeadPosition();
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				pLinkCur = pDoc->m_lstLnks.GetAt(pos);
				if (pLinkCur->IsPointInside(point))
				{
					if (!(pLinkCur->Get_pPageFrom()->IsSelected() && 
							pLinkCur->Get_pPageTo()->IsSelected()))
							DeselectAll();	// Раз выделена ссылка, не лежащая в выделении, то надо снять выделение со всех элементов.					
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
			// Если и сейчас не вышло, то значит, надо рисовать прямоугольник выделения
			m_ptBeginDragSelBox = point;	// Здесь может начинаться рисование прямоугольника выделения.
			m_fptBeginDragSet = TRUE;			
			DeselectAll();	// Раз начато новое выделение, значит, на старое надо забить
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
		DrawDragged(&m_DCOffscreen);	// Так как пока на основном заэкранном контексте перетаскиваемая страница стерта. Надо отобразить.
		// Ничего больше перемещать не надо	
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;				
	}

	if (m_fIsResizingNow)
	{
		DrawDragged(&m_DCOffscreen);	// Так как пока на основном заэкранном контексте страница, размер которой меняют, стерта. Надо отобразить.
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
		int i;	// Для цикла for
		CNLB_BldrDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);	

		CountSelRctCoords(rct);		

		// Пометим выделенные страницы
		pos = pDoc->m_lstPages.GetHeadPosition();	
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (i=0; i<iPagesCount; i++)
		{			
			pPageCur = pDoc->m_lstPages.GetAt(pos);
			if (pPageCur->IsIntersect(rct))
			{
				if (!(pPageCur->IsSelected())) pDoc->m_iSelectedPagesCount++;	// Если действительно пометили новую страницу, увеличим счетчик
				pPageCur->Select();
			}
			else
			{
				if (pPageCur->IsSelected()) pDoc->m_iSelectedPagesCount--;	// Если страница, с которой снимаем пометку, действительно была помечена, уменьшим счетчик.
				pPageCur->Deselect();
			}
			pDoc->m_lstPages.GetNext(pos);
		}	

		// Пометим выделенные дуги
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

		PaintInMemory();	// Чтобы стереть старое выделение - эта операция будет выполнена БЫСТРО
		m_fptBeginDragSet = m_fptEndDragSet = FALSE;
		PaintInMemory();	// Чтобы отобразить результат выделения (помеченные страницы)
	}
	m_fptBeginDragSet = m_fptEndDragSet = FALSE;	// Это надо сделать даже в том случае, когда условие выше не выполнено.
	UpdateSB();	// Скорее всего, надо обновить скроллбары, ибо изображение, видимо, изменилось...

	SetWndCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		// Также надо сменить курсор на стандартную стрелочку, так как 
		// иначе будет некрасиво (он может быть другим, а сменится только
		// после движения мышкой)

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

	if (m_fptEndDragSet) PaintInMemory();	// Чтобы стереть старое выделение
	m_fptEndDragSet = FALSE;	// Будем пессимистами

	if (!m_fIsDraggingInProgress)
	{
		if (m_pLastSelectedBookItem && 
			(itSelItemType == SelItem_BookPage))	// Можно изменять ее размеры!
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

		// Все это замечательно, но если левая кнопка не нажата, то оно не имеет смысла...
		// Но даже в этом случае код выше надо исполнить, чтобы правильно отобразить курсоры для resizing
		if (!m_fMouseDownNow) 
			fIsResizingWX = fIsResizingEX = fIsResizingNY = fIsResizingSY = FALSE;
	}

	if (!cur)
	{		
		if ((m_fIsDraggingStarted || m_fIsDraggingInProgress)&&
			(!fIsResizingWX)&&
			(!fIsResizingEX)&&
			(!fIsResizingNY)&&
			(!fIsResizingSY))	// Ее можно таскать!
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

	// Установим, наконец, курсор...
	if (cur) SetWndCursor(cur);	

	if (fIsResizingNY)	// (itSelItemType == SelItem_BookPage) - гарантированно выполнено, т.к. см. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// Отменим выбор страницы для перетаскивания, т. к. это будет мешать
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;		
		if (fNeedRepaint) PaintInMemory();	// Если в прошлый раз resizing'а не было, то надо отобразить страницы в памяти, так как иначе в начале не затрутся страницы в старой позиции
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;
		int iResize;		
		if (point.y < pLastSelectedPage->Get_iTop() + pLastSelectedPage->Get_iHeight()) 
		{
			iResize = (pLastSelectedPage->Get_iTop() + pLastSelectedPage->Get_iHeight()) - point.y;
			pLastSelectedPage->Get_iTop() = point.y;
			pLastSelectedPage->Get_iHeight() = iResize;
		}
		pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
		Invalidate();
	}

	if (fIsResizingWX)	// (itSelItemType == SelItem_BookPage) - гарантированно выполнено, т.к. см. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// Отменим выбор страницы для перетаскивания, т. к. это будет мешать
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;		
		if (fNeedRepaint) PaintInMemory();	// Если в прошлый раз resizing'а не было, то надо отобразить страницы в памяти, так как иначе в начале не затрутся страницы в старой позиции
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;
		int iResize;		
		if (point.x < pLastSelectedPage->Get_iLeft() + pLastSelectedPage->Get_iWidth()) 
		{
			iResize = (pLastSelectedPage->Get_iLeft() + pLastSelectedPage->Get_iWidth()) - point.x;
			pLastSelectedPage->Get_iLeft() = point.x;
			pLastSelectedPage->Get_iWidth() = iResize;
		}
		pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
		Invalidate();
	}

	if (fIsResizingSY)	// (itSelItemType == SelItem_BookPage) - гарантированно выполнено, т.к. см. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// Отменим выбор страницы для перетаскивания, т. к. это будет мешать
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;		
		if (fNeedRepaint) PaintInMemory();	// Если в прошлый раз resizing'а не было, то надо отобразить страницы в памяти, так как иначе в начале не затрутся страницы в старой позиции
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;		
		if (point.y > pLastSelectedPage->Get_iTop()) pLastSelectedPage->Get_iHeight() = point.y - pLastSelectedPage->Get_iTop();
		pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
		Invalidate();
	}

	if (fIsResizingEX)	// (itSelItemType == SelItem_BookPage) - гарантированно выполнено, т.к. см. (*)
	{
		BOOL fNeedRepaint = !m_fIsResizingNow;		
		m_fIsResizingNow = TRUE;
		// Отменим выбор страницы для перетаскивания, т. к. это будет мешать		
		m_fIsDraggingStarted = FALSE;
		m_fIsDraggingInProgress = FALSE;
		if (fNeedRepaint) PaintInMemory();	// Если в прошлый раз resizing'а не было, то надо отобразить страницы в памяти, так как иначе в начале не затрутся страницы в старой позиции
		CBookPage* pLastSelectedPage = (CBookPage*) m_pLastSelectedBookItem;		
		if (point.x > pLastSelectedPage->Get_iLeft()) pLastSelectedPage->Get_iWidth() = point.x - pLastSelectedPage->Get_iLeft();
		pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
		Invalidate();
	}

	if ((m_fIsDraggingStarted || m_fIsDraggingInProgress)&&
		(!fIsResizingWX)&&
		(!fIsResizingEX)&&
		(!fIsResizingNY)&&
		(!fIsResizingSY))	// Ее можно таскать!
	{						
		m_iOffsDragX = point.x - ptStartDrag.x;
		m_iOffsDragY = point.y - ptStartDrag.y;
		if (!m_fIsDraggingInProgress) 
		{
			m_fIsDraggingInProgress = TRUE;
			PaintInMemory(FALSE);	// Начато перетаскивание => отобразим на основном заэкранном контексте все, кроме перетаскиваемой страницы.		
		}
		pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
		Invalidate();		
		ptStartDrag = point;	// После того, как перетаскивание было совершено, продолжим его...
	}		
	else
		m_fIsDraggingInProgress = FALSE;
	
	if (m_fptBeginDragSet)
	{								
		m_fptEndDragSet = TRUE;
		m_ptEndDragSelBox = point;	
		PaintInMemory();	// Чтобы нарисовать новое выделение
	}

	CView::OnMouseMove(nFlags, point);
}

VOID CNLB_BldrView::SetWndCursor(HCURSOR Cur)
{
	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG) Cur);	// Установили данный курсор в качестве постоянного для данного окна...
	// SetClassLongPtr(GetSafeHwnd(), GCLP_HCURSOR, (LONG_PTR) Cur); -- this can be used for amd64 compatibility...
	SetCursor(Cur);	// ...и установили его сразу. Если этого не сделать, изменения проявятся только после движения мышкой
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

	// По правому щелчку должен быть заново осуществлен выбор страницы
	m_pLastSelectedBookItem = NULL;	// Будем пессимистами
	m_fIsDraggingStarted = FALSE;
	m_fIsDraggingInProgress = FALSE;
	m_pPageFrom = m_pPageTo = NULL;	// Если было начато построение ссылки, то забудем об этом...
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

			if (!pPageCur->IsSelected())	// Она не была выбрана, значит, она становится выбранной, а с остальных выделение снимается
			{
				DeselectAll();
				pPageCur->Select();	// Теперь она выбрана	
				pDoc->m_iSelectedPagesCount = 1;	// Теперь помечена только одна страница!
			}

			break;
		}
		pDoc->m_lstPages.GetNext(pos);

		if (!m_pLastSelectedBookItem)
		{
			// Если не вышло, пробуем пометить элемент как ссылку
			POSITION pos = pDoc->m_lstLnks.GetHeadPosition();
			iLnksCount = pDoc->m_lstLnks.GetCount();
			for (int i=0; i<iLnksCount; i++)
			{		
				pLinkCur = pDoc->m_lstLnks.GetAt(pos);
				if (pLinkCur->IsPointInside(point))
				{
					if (!(pLinkCur->Get_pPageFrom()->IsSelected() && 
							pLinkCur->Get_pPageTo()->IsSelected()))
							DeselectAll();	// Раз выделена ссылка, не лежащая в выделении, то надо снять выделение со всех элементов.					
					m_pLastSelectedBookItem = (CDrawable*) pLinkCur;
					pLinkCur->Select();
					itSelItemType = SelItem_Link;								
					break;
				}				
				pDoc->m_lstLnks.GetNext(pos);
			}
		}
	}
	
	if (!m_pLastSelectedBookItem) DeselectAll();	// Это означает, что пользователь щелкнул по пустому месту; снимем все пометки

	PaintInMemory();

	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))	// Отобразим выпадающее меню для страницы
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
		(itSelItemType == SelItem_Link))	// Отобразим выпадающее меню для ссылки
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
				pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
				PaintInMemory();
			}
		}
	}
	else if (itSelItemType == SelItem_None)
	{
		pDoc->m_pStartPoint = NULL;	// Сбросим его...
		pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
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

	// Сначала удаляем все связанные с удаляемыми страницами ссылки
	pos = pDoc->m_lstLnks.GetHeadPosition();
	i = 0;
	iLnksCount = pDoc->m_lstLnks.GetCount();
	while (i < iLnksCount)	// Проходим по ВСЕМ элементам, какие-то удаляем, а какие-то просто пропускаем
	{		
		if ((pDoc->m_lstLnks.GetAt(pos)->Get_pPageFrom()->IsSelected()) ||
		   (pDoc->m_lstLnks.GetAt(pos)->Get_pPageTo()->IsSelected()))
		{
			POSITION posRemov = pos;			    		
			pDoc->m_lstLnks.GetNext(pos);
			pDoc->m_lstLnks.GetAt(posRemov)->Draw(&m_DCOffscreen, PARAM_ERASE);	// Стереть на старом месте...
			// Тут принадлежность рисуемого элемента видимой части экрана не проверяем, так как стереть его надо в любом случае...
			delete (pDoc->m_lstLnks.GetAt(posRemov));
			pDoc->m_lstLnks.RemoveAt(posRemov);						
		}
		else pDoc->m_lstLnks.GetNext(pos);
		i++; 
	}

	// А теперь и их сами!
	pos = pDoc->m_lstPages.GetHeadPosition();
	i = 0;
	iPagesCount = pDoc->m_lstPages.GetCount();
	while (i < iPagesCount)	// Проходим по ВСЕМ элементам, какие-то удаляем, а какие-то просто пропускаем
	{				
		pCurPage = pDoc->m_lstPages.GetAt(pos);
		if (pCurPage->IsSelected())
		{
			POSITION posRemov = pos;			    		
			pDoc->m_lstPages.GetNext(pos);
			pCurPage->Draw(&m_DCOffscreen, PARAM_ERASE);	// Стереть на старом месте...
			// Тут принадлежность рисуемого элемента видимой части экрана не проверяем, так как стереть его надо в любом случае...
			if (pDoc->m_pStartPoint == pCurPage) pDoc->m_pStartPoint = NULL;
			delete pCurPage;
			pDoc->m_lstPages.RemoveAt(posRemov);								
		}
		else pDoc->m_lstPages.GetNext(pos);
		i++; 
	}
	
	
	pDoc->m_iSelectedPagesCount = 0;	// Т.к. все помеченное было удалено (если, конечно, вообзе что-то было помечено)
	m_pLastSelectedBookItem = NULL;	// Теперь ничего не выбрано!
	// Вот и все. Ее больше нет...
	
	pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
	PaintInMemory();			
	UpdateSB();	// Кроме того, возможно, нужно обновить скроллбары
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

		// dlg.m_ListLinksFrom пока пуст, так как объект только что создан
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
			pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
			PaintInMemory();
		}

		// В конце концов очистим вспомогательный список диалога
		pos = dlg.m_ListLinksFrom.GetHeadPosition();
		while (!(dlg.m_ListLinksFrom.IsEmpty()))	
		{					
			pos_remov = pos;
			plcas = dlg.m_ListLinksFrom.GetNext(pos);
			if (plcas)
			{				
				if (plcas->lpszLinkCaptionForLV) delete []plcas->lpszLinkCaptionForLV;
				if (plcas->lpszLinkConstraintsForLV) delete []plcas->lpszLinkConstraintsForLV;
				// plcas->pLink не удаляем!!!
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
			pLinkCur->Draw(&m_DCOffscreen, PARAM_ERASE);	// Стереть на старом месте...
			// Тут принадлежность рисуемого элемента видимой части экрана не проверяем, так как стереть его надо в любом случае...
			delete (pDoc->m_lstLnks.GetAt(pos));
			pDoc->m_lstLnks.RemoveAt(pos);
			m_pLastSelectedBookItem = NULL;	// Теперь ничего не выбрано!
			pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
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
			pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
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
// strFaultyVarName - имя переменной, на которой обнаружилась ошибка (если таковая имеет место)
{
	int isLen = strText.GetLength();
	int i = 0;	// Обнулим счетчик, т.к. начали работу с новой страницей
	while (i < isLen-1)	
	{
		if ((strText[i]==_T('\r'))&&(strText[i+1]==_T('\n')))	// Обработать перевод строки
		{
			strText.Delete(i, 2);
			strText.Insert(i, strIns);			
			isLen+=(iLenIns-2);	// Длина строки увеличилась на iLenIns символов, но 2 было удалено...
			i+=iLenIns;	// Перейти к символу, следующему за вставленной строкой...

			if (i < isLen-1) // Если такой символ существует (вполне могли выскочить за пределы)
			{
				if ((strText[i]==_T(' '))||(strText[i]==_T('\t')))	// Если следующий символ идентифицирует начало нового параграфа...
				{
					strText.Insert(i, strPara);	// ...то и вставим код для параграфа!
					isLen+=iLenPara;
					i+=iLenPara;
				}
			}
		}
		else if ((strText[i]==_T('-')) && (strText[i+1]==_T('-')))  // Сделать из него &mdash;
		{
			strText.Delete(i, 2);
			strText.Insert(i, strDash);			
			isLen+=(iLenDash-2);
			i+=iLenDash;
		}
		else if (((strText[i]==_T('-')) && _istspace((_TXCHAR) strText[i+1])) ||
				((strText[i]==_T('-')) && (strText[i+1]==_T('\r'))))
				// Сделать из него &mdash; (немного по-другому)
		{
			strText.Delete(i, 1);
			strText.Insert(i, strDash);			
			isLen+=(iLenDash-1);
			i+=iLenDash;
		}
		else if (strText[i]==_T('\''))  // Апостроф - служебный символ Antokolos NLB Engine. Заменим его на приемлемую комбинацию.
		{
			strText.Delete(i, 1);
			strText.Insert(i, strApostrophe);			
			isLen+=(iLenApostrophe-1);
			i+=iLenApostrophe;
		}
		else if (strText[i]==_T('\\'))  // Бэкслеш - служебный символ строк C. Его надо заменить удвоенным, чтобы он правильно отображался
		{
			strText.Delete(i, 1);
			strText.Insert(i, strBackslash);			
			isLen+=(iLenBackslash-1);
			i+=iLenBackslash;
		}
		else if ((strText[i]==_T('%')) && (strText[i+1]==_T('%')))	// Процент - служебный символ. С его помощью вместо набранного после процента имени переменной показывается ее численное значение. А композиция %% означает просто символ процента
		{
			strText.Delete(i, 2);
			strText.Insert(i, _T('%'));			
			isLen--;
			i++;
		}			
		else if ((strText[i]==_T('%')) && (strText[i+1]!=_T('%')))	// Щас должна пойти переменная
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
			if (j < isLen)	// Условие (j > i + 1) будет выполнено автоматически, т.к. иначе это %%, т.е. будет отсеяно в предыдущей ветке if
			{
				strVarName = strText.Mid(i+1, j - i - 1);
				bvPos = pDoc->FindBookVarPositionByName(strVarName);
				if (bvPos)	// Т.е. такая переменная действительно есть
				{
					pbv = pDoc->m_lstBookVars.GetAt(bvPos);
					if (pbv)
					{
						if (pbv->Get_fIsVarGlobal())	// Отображать можно состояние только глобальных переменных
						{
							iStrVarLen = strVarName.GetLength();
							strText.Delete(i, iStrVarLen + 2);	// Не забудем удалить и символы процента в начале и конце
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
								strText.Delete(i, iStrVarLen + 2);	// Не забудем удалить и символы процента в начале и конце
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
							strText.Delete(i, iStrVarLen + 2);	// Не забудем удалить и символы процента в начале и конце
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
							TCHAR* pstrTempVarName;	// Она будет изменяться _tcstok()											
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
									if (svPos)	// Т.е. такая переменная действительно есть
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
											// Функция без параметров, т.е. f()
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
									strText.Delete(i, iStrVarLen + 2);	// Не забудем удалить и символы процента в начале и конце
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

			// Если произошла какая-то ошибка, то удалим ошибочный вызов, а пользователю покажем об этом предупреждение
			if (!fVarReplaceOK)
			{
				strText.Delete(i, j - i + 1);
				isLen -= (j - i + 1);

				strFaultyVarName = strVarName;
				return I_DEPARAM_ERR_IN_VAR_VALUE;				
			}
		}
		else if ((strText[i]==_T('$')) && (strText[i+1]==_T('$')))	// Доллар - служебный символ. С его помощью в страничку внедряется поле ввода, в которое можно ввести новое значение соответствующей строковой переменной. А композиция $$ означает просто символ доллара
		{
			strText.Delete(i, 2);
			strText.Insert(i, _T('$'));			
			isLen--;
			i++;
		}
		else if ((strText[i]==_T('$')) && (strText[i+1]!=_T('$')))	// Щас должна пойти переменная, которую будем изменять. 
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
			if (j < isLen)	// Условие (j > i + 1) будет выполнено автоматически, т.к. иначе это $$, т.е. будет отсеяно в предыдущей ветке if
			{
				strVarName = strText.Mid(i+1, j - i - 1);
									
				svPos = pDoc->FindStrVarPositionByName(strVarName);
				if (svPos)	// Т.е. такая переменная действительно есть
				{
					psv = pDoc->m_lstStrVars.GetAt(svPos);
					if (psv)
					{								
						if (psv->Get_fSaveInTheBookmarksFile())	// Разрешать ввод можно только для тех переменных, которые будут сохраняться
						{
							iStrVarLen = strVarName.GetLength();
							strText.Delete(i, iStrVarLen + 2);	// Не забудем удалить и символы процента в начале и конце
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

			// Если произошла какая-то ошибка, то удалим ошибочный вызов, а пользователю покажем об этом предупреждение
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
	CString strCurPage = _TEXT("");		// Результат преобразования текущей страницы

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
	POSITION posPages;	// Позиция в списке страниц
	POSITION posLnks;	// Позиция в списке ссылок
	CBookPage* pCurPage;
	CBookPage* pPageFrom;
	CLink* pCurLnk;	
	int iStartPointPos;
	CString strText;	
	CString strConstraint;
	CString strVarsToAppend;
	CList<CLink*, CLink*&> lstLinksForCurPage;	// Список используемых для данной страницы ссылок, отсортированных по возрастанию m_iLinkNumber
	int iLnksCountForCurPage;
	POSITION posLnksForCurPage;
	CLink* pCurLnkForCurPage;
	CList<CBookVar*, CBookVar*&> lstBookVarsForCurPage;		// Список используемых на текущей странице Book Variables.
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
	CString strDeclareVarBuf;	// Она нужна для уверенности, что Find вернет то, что надо... См. ниже.
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

	// Списки для составления списка переменных для текущей Triggered String	
	CList<CBookPage*, CBookPage*&> lstPageVarsForCurTS;	
	POSITION posTSPage;
	CList<CProbVar*, CProbVar*&> lstProbVarsForCurTS;	
	POSITION posTSProb;
	CList<CBookVar*, CBookVar*&> lstWorkerListOfVarsForCurTS;	
	POSITION posTSWork;
	POSITION posTSBook;	// Текущая позиция BookVar	
	POSITION posRemovForTSWork;
	CBookPage* pPageVarCur_ForTS;
	CProbVar* pProbVarCur_ForTS;
	CBookVar* pBookVarCur_ForTS;
	int ibvIdx, isvIdx;
		
	strResult.Empty();	// Сделать ее пустой...

	if (pDoc->m_pStartPoint)
	{
		iStartPointPos = pDoc->FindPageIndexByPointer(pDoc->m_pStartPoint);
	}
	else
		return FALSE;	// Без startpoint'а книга не выйдет

	fNeedStartCompileOnError = TRUE;

	// Запишем информацию о глобальных переменных
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
			 strResult += strGlobalVarDefinition;	// Записали информацию о глобальной переменной
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
			// Если предупреждения не показывать, то неверный вызов будет просто тихо удален... Может, так и вернее, но это почва для ошибок.
			CString strMessage;
			CString strCapt;
			
			strCapt.LoadString(IDS_ERROR_MB_CAPT);
			strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_VALUE_BUFFER, strFaultyVarName);
			if (MessageBox(strMessage, strCapt, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{				
				// Вставить редактирование 
			}

			fNeedStartCompileOnError = FALSE;
			return FALSE;
		 }
		 else if (iDeparTextResult == I_DEPARAM_ERR_IN_VAR_INPUT)
		 {			
			// Если предупреждения не показывать, то неверный вызов будет просто тихо удален... Может, так и вернее, но это почва для ошибок.
			CString strMessage;
			CString strCapt;
			
			strCapt.LoadString(IDS_ERROR_MB_CAPT);
			strMessage.Format(IDS_STRING_CANNOT_FIND_VAR_TO_DISPLAY_INPUT_BUFFER, strFaultyVarName);
			if (MessageBox(strMessage, strCapt, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			{
				// Вставить редактирование 					
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
				strResult += strGlobalVarDefinition;	// Записали информацию о глобальной строковой переменной		 
			}
			else
			{
				strGlobalVarDefinition.Format(IDS_STRING_DEPENDENT_STRVAR_WITHOUT_PARAMETERS_DEFINITION_BLOCK, 
					pStrVarCur->Get_strStrVarName(), 
					strToInsertInStrVarDefinition);	
				strDependentVarFunctionsImplementation += strGlobalVarDefinition;	// Записали информацию о глобальной строковой переменной		 
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
			// Последний параметр пойдет без запятой
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
			// Позже это мы добавим в конец, это необходимо для того, чтобы все переменные, которые будут использоваться в функции, были бы уже определены
		 }		 
	}

	// Запишем все функции для Triggered String
	posTSVarsMain = pDoc->m_lstTriggeredStrings.GetHeadPosition();
	iTSVarsMainCount = pDoc->m_lstTriggeredStrings.GetCount();	
	for (int itsvIdx = 0; itsvIdx < iTSVarsMainCount; itsvIdx++)
	{		 
		 pTSVarCur = pDoc->m_lstTriggeredStrings.GetNext(posTSVarsMain);

		 // <Этот код взят из составления списка переменных для страницы книги>
		 strVarsToAppend.Empty();
		 lstBookVarsForCurPage.RemoveAll();	// Очистим список			 
		 // </Этот код взят из составления списка переменных для страницы книги>
		 lstPageVarsForCurTS.RemoveAll();	// Очистим список	
		 lstProbVarsForCurTS.RemoveAll();	// Очистим список	
		 lstWorkerListOfVarsForCurTS.RemoveAll();	// Очистим список			 

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

		 // Определим все необходимые переменные
		 PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
		 while (PosInLstBookVars)
		 {		
		 	pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);			
			lstWorkerListOfVarsForCurTS.AddTail(pBookVarCur);
		 }

		 lstBookVarsForCurPage.RemoveAll();	// Очистим список, он будет заполнен заново
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
				 CString strCurVar(_T(' '), MAX_VAR_LENGTH);	// MAX_VAR_LENGTH пробелов. MAX_VAR_LENGTH - константа из CLogicTrans.h. Идентификатор переменной не может иметь большую длину.										 
				
				 if (aTrans.GetFirstVar(strCurVar, MAX_VAR_LENGTH))
				 {						 					 
					 posTSProb = pDoc->FindProbVarPositionByName(strCurVar);

					 // Для ускорения поиска
					 if (posTSProb) 
						 posTSBook = NULL;
					 else
						 posTSBook = pDoc->FindBookVarPositionByName(strCurVar);

					 // Для ускорения поиска
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

						 // Для ускорения поиска
						 if (posTSProb) 
							 posTSBook = NULL;
						 else
							 posTSBook = pDoc->FindBookVarPositionByName(strCurVar);

						 // Для ускорения поиска
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

		 // <Этот код взят из составления списка переменных для страницы книги>
		 SortBookVars(lstBookVarsForCurPage);	// Расставим переменные в списке в нужном порядке
		 // Итак, переменные в списке расположены в нужном порядке. Теперь сформируем строки с определениями переменных.
		 PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
		 while (PosInLstBookVars)
		 {		
		 	pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);
			if (!pBookVarCur->Get_fIsVarGlobal())	// Глобальные переменные переопределять не надо!
			{
				strPageVar.Format(strBookVarBufForTS, 
						pBookVarCur->Get_strBookVarName(), 
						pBookVarCur->Get_strBookVarBody());
				strVarsToAppend += strPageVar;
			}
		 }
		 // </Этот код взят из составления списка переменных для страницы книги>
		 
		 strTSVarDefinition.Format(IDS_STRING_TRIGGERED_STRING_DEFINITION_BLOCK, 
						pTSVarCur->Get_strTSName(), 
						strVarsToAppend,
						strToInsertInTSVarDefinition);	
		 strDependentVarFunctionsImplementation += strTSVarDefinition;
	}

	lstBookVarsForCurPage.RemoveAll();	// Очистим список	
	lstPageVarsForCurTS.RemoveAll();	// Очистим список	
	lstProbVarsForCurTS.RemoveAll();	// Очистим список	
	lstWorkerListOfVarsForCurTS.RemoveAll();	// Очистим список		

	strNumPages.Format(strNumPagesBuf, iPagesCount);	// Внесли информацию о числе страниц в книге
	strStartpoint.Format(strStartpointBuf, iStartPointPos);
	strResult += strNumPages;	// Записали информацию о числе страниц в книге
	strResult += strStartpoint;	// Записали информацию о стартовой странице	

	strResult += strFuncBegins;	// Началась функция GoTo
	posPages = pDoc->m_lstPages.GetHeadPosition();
	for (int iPage=0; iPage<iPagesCount; iPage++)	// Идем по страницам...
	{
		pCurPage = pDoc->m_lstPages.GetAt(posPages);
		strCurPage = pCurPage->Get_strPageText();	// присвоить ей значение текста очередной страницы
		
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
			// Если предупреждения не показывать, то неверный вызов будет просто тихо удален... Может, так и вернее, но это почва для ошибок.
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
			// Если предупреждения не показывать, то неверный вызов будет просто тихо удален... Может, так и вернее, но это почва для ошибок.
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
		lstBookVarsForCurPage.RemoveAll();	// Очистим список		
		lstLinksForCurPage.RemoveAll();		// Очистим список		
		for (int j=0; j<iLnksCount; j++)
		{
			pCurLnk = pDoc->m_lstLnks.GetAt(posLnks);
			pPageFrom = pCurLnk->Get_pPageFrom();

			if (pPageFrom == pCurPage)	// Если эта ссылка - от текущей страницы
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

			// Сначала обрежем все пробелы слева и справа. Может быть, после этого строка даже окажется пустой
			strConstraint.TrimLeft();
			strConstraint.TrimRight();
			int iConstraintLength = strConstraint.GetLength();

			if (iConstraintLength!=0)
			{
				CString strTmpConstraint = strConstraint;
				int iVarPageIndex;
				
				if (CheckLinkContainInfiniteLoop(strTmpConstraint, lstBookVarsForCurPage, TRUE) != E_IL_OK) return FALSE;

				// После выполнения предыдущей функции формула strTmpConstraint уже не содержит Book Variables, однако, содержит все необходимые страничные переменные
				pTrans = new CMathTrans(strTmpConstraint);					
				if (pTrans)
				{
					if (pTrans->Compile() != IERR_ERROR_SUCCESS) return FALSE;
					// Теперь осталось только проверить, что все переменные из этой формулы есть в нашей книге. 
					// Если есть, то вставим эти переменные в конвертированный текст bookbody.js						

					try
					{
						CString strCurVar(_T(' '), MAX_VAR_LENGTH);	// MAX_VAR_LENGTH пробелов. MAX_VAR_LENGTH - константа из CLogicTrans.h. Идентификатор переменной не может иметь большую длину.			
						
						if (pTrans->GetFirstVar(strCurVar, MAX_VAR_LENGTH))
						{												
							// strDeclareVar здесь добавляем для уверенности, что функция найдет именно имя ПЕРЕМЕННОЙ, которая уже есть, а не что-нибудь еще (к примеру, пользователь назовет переменную ArrPageVisitState)...
							strDeclareVar.Format(strDeclareVarBuf, strCurVar);
							if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// Т.е такой еще нет...
							{
								iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
								if (iVarPageIndex == -1) 
								{
									// Возможно, это не страничная, а вероятностная переменная
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
										return FALSE;	// Такая переменная не была найдена, значит, пользователь неверно ввел имя переменной
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
								// strDeclareVar здесь добавляем для уверенности, что функция найдет именно имя ПЕРЕМЕННОЙ, которая уже есть, а не что-нибудь еще (к примеру, пользователь назовет переменную ArrPageVisitState)...
								strDeclareVar.Format(strDeclareVarBuf, strCurVar);
								if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// Т.е такой еще нет...
								{
									iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
									if (iVarPageIndex == -1) 
									{
										// Возможно, это не страничная, а вероятностная переменная
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
											return FALSE;	// Такая переменная не была найдена, значит, пользователь неверно ввел имя переменной
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
						j,	// Индекс текущей ссылки
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
						j,	// Индекс текущей ссылки
						pDoc->FindPageIndexByPointer(pCurLnkForCurPage->Get_pPageTo()), 
						strText);
				}
			}
			
			isLen = strCurPage.GetLength();
			strCurPage.Insert(isLen, strOption);
			lstLinksForCurPage.GetNext(posLnksForCurPage);
		}


		SortBookVars(lstBookVarsForCurPage);	// Расставим переменные в списке в нужном порядке

		// Итак, переменные в списке расположены в нужном порядке. Теперь сформируем строки с определениями переменных.
		PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
		while (PosInLstBookVars)
		{		
			pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);
			if (!pBookVarCur->Get_fIsVarGlobal())	// Глобальные переменные переопределять не надо!
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

		strResult += strCurPage;	// Приплюсуем...
		pDoc->m_lstPages.GetNext(posPages);	// И пойдем дальше!
	}
	strResult += strFuncEnds;	// Кончилась функция GoTo

	strResult += strDependentVarFunctionsImplementation;
	// Сразу после GoTo запишем давно хранимую информацию о функциях зависимых строковых переменных - см. комментарий в начале

	// Началась функция ModifyVars()...
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
			lstBookVarsForCurPage.RemoveAll();	// Очистим список
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

				// После выполнения предыдущей функции формула strTmpConstraint уже не содержит Book Variables
				CMathTrans aTrans(strTmpConstraint);										
				if (aTrans.Compile() != IERR_ERROR_SUCCESS) return FALSE;

				// Решено разрешить использование страничных и вероятностных переменных в модификациях
				// if (!aTrans.VarsListIsEmpty()) return FALSE;	// Ссылки на страничные переменные запрещены!!! Полученная в результате формула должна содержать только численные константы!!!				

				try
				{
					CString strCurVar(_T(' '), MAX_VAR_LENGTH);	// MAX_VAR_LENGTH пробелов. MAX_VAR_LENGTH - константа из CLogicTrans.h. Идентификатор переменной не может иметь большую длину.			
					
					if (aTrans.GetFirstVar(strCurVar, MAX_VAR_LENGTH))
					{												
						// strDeclareVar здесь добавляем для уверенности, что функция найдет именно имя ПЕРЕМЕННОЙ, которая уже есть, а не что-нибудь еще (к примеру, пользователь назовет переменную ArrPageVisitState)...
						strDeclareVar.Format(strDeclareVarBuf, strCurVar);
						if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// Т.е такой еще нет...
						{
							iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
							if (iVarPageIndex == -1) 
							{
								// Возможно, это не страничная, а вероятностная переменная
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
									return FALSE;	// Такая переменная не была найдена, значит, пользователь неверно ввел имя переменной
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
							// strDeclareVar здесь добавляем для уверенности, что функция найдет именно имя ПЕРЕМЕННОЙ, которая уже есть, а не что-нибудь еще (к примеру, пользователь назовет переменную ArrPageVisitState)...
							strDeclareVar.Format(strDeclareVarBuf, strCurVar);
							if (strVarsToAppend.Find(strDeclareVar,0) == -1)	// Т.е такой еще нет...
							{
								iVarPageIndex = pDoc->FindPageIndexByVar(strCurVar);
								if (iVarPageIndex == -1) 
								{
									// Возможно, это не страничная, а вероятностная переменная
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
										return FALSE;	// Такая переменная не была найдена, значит, пользователь неверно ввел имя переменной
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
			SortBookVars(lstBookVarsForCurPage);	// Расставим переменные в списке в нужном порядке

			// Итак, переменные в списке расположены в нужном порядке. Теперь сформируем строки с определениями переменных.
			PosInLstBookVars = lstBookVarsForCurPage.GetHeadPosition();
			while (PosInLstBookVars)
			{		
				pBookVarCur = lstBookVarsForCurPage.GetNext(PosInLstBookVars);
				if (!pBookVarCur->Get_fIsVarGlobal())	// Глобальные переменные переопределять не надо!
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
	strResult += strModifyVarsFuncEnds;	// Кончилась функция ModifyVars()

	// Началась функция SaveGlobalVariables()...
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
			 strResult += strCurSaveStatement;	// Записали информацию о глобальной переменной
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
			 strResult += strCurSaveStatement;	// Записали информацию о глобальной переменной
		 }
	}	
	strResult += strSaveVarsFuncEnds;	// Кончилась функция SaveGlobalVariables()..

	// Началась функция LoadGlobalVariables()...
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
			 strResult += strCurLoadStatement;	// Записали информацию о глобальной переменной
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
			 strResult += strCurLoadStatement;	// Записали информацию о глобальной переменной
		 }
	}	
	strResult += strLoadVarsFuncEnds; // Кончилась функция LoadGlobalVariables().

	return TRUE;	// Кажись, все OK
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
	CString strTitle;	// Строка, которая будет заключена в <title> ... </title>
	BOOL fNeedToStartCompileOnError = TRUE;

	//Фильтр расширений имени файлов
	CString strFilter_NLB_Engine10;
	strFilter_NLB_Engine10.LoadString(IDS_FILTER_NLB_ENGINE_10);	

	//Инициализация имени выбираемого файла
	CString strFName_NLB_Engine10;
	strFName_NLB_Engine10.LoadString(IDS_FNAME_NLB_ENGINE_10);

	//Инициализация расширения по умолчанию выбираемого файла
	CString strExt_NLB_Engine10;
	strExt_NLB_Engine10.LoadString(IDS_EXT_NLB_ENGINE_10);

	CFileDialog dlg(FALSE, // Save As dialog box
		strExt_NLB_Engine10, // Расширение по умолчанию
		strFName_NLB_Engine10, // Имя файла по умолчанию
		OFN_HIDEREADONLY | 
			OFN_OVERWRITEPROMPT | 			
			OFN_PATHMUSTEXIST,	// Флаги
		strFilter_NLB_Engine10, // Фильтр
		this	// Для этого окна...
		);		

	if (dlg.DoModal() == IDOK)
	{
		CFileException ex;
		CFile FileToSave;		
		CString strCaption;
		TCHAR strErrorDescrptn[MAX_LOADSTRING];		
		HRSRC h_NLBE_10;
		HGLOBAL hGl_NLBE_10;
		char* pstr_NLBE_10;	// В ресурсах файла данный шаблон создан в формате ANSI (однобайтные символы)
		TCHAR* sz_NLBE_10_Buf;
		int i_NLBE_10_Buf_Size;
		
		if (!FileToSave.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite, &ex))
		{
			// Операция провалилась, о чем надо сказать пользователю
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
				// Операция провалилась, о чем надо сказать пользователю
				m_strMessage.LoadString(IDS_ERR_CANNOT_RETRIEVE_NLBE_10_RESOURCE);
				strCaption.LoadString(IDS_ERROR_MB_CAPT);
				MessageBox(m_strMessage, strCaption, MB_ICONERROR);

				FileToSave.Close();	// Закрыть так неосмотрительно, с такой надеждой открытый файл...			
				return;	// И вернуть ошибку!
			}

			i_NLBE_10_Buf_Size = SizeofResource(NULL, h_NLBE_10);	// Возращает размер в байтах, а так как там использованы однобайтные символы, то, значит, длину строки
			if (i_NLBE_10_Buf_Size > 0)
			{				
				sz_NLBE_10_Buf = new TCHAR[i_NLBE_10_Buf_Size + 1];	// +1 - для завершающего нуля
				if (sz_NLBE_10_Buf)
				{
					// В этом месте можно при большом желании поизвращаться с макросом _UNICODE. Но зачем, если код ниже, вроде бы, должен работать в любом случае?
					// (***) -- пометим все же это место... Возможно, тут могут быть проблемы...
					for (int i = 0; i < i_NLBE_10_Buf_Size; i++)
					{
						sz_NLBE_10_Buf[i] = ((TCHAR) (pstr_NLBE_10[i]));
					}										

					sz_NLBE_10_Buf[i_NLBE_10_Buf_Size] = _T('\0');	// Завершающий нуль необходим в любом случае
				}
			}
			else
				sz_NLBE_10_Buf = NULL;

			if (!sz_NLBE_10_Buf)
			{			
				// Операция провалилась, о чем надо сказать пользователю
				m_strMessage.LoadString(IDS_ERR_CANNOT_CREATE_NLBE_10_BUF);
				strCaption.LoadString(IDS_ERROR_MB_CAPT);
				MessageBox(m_strMessage, strCaption, MB_ICONERROR);

				FileToSave.Close();	// Закрыть так неосмотрительно, с такой надеждой открытый файл...			
				return;	// И вернуть ошибку!
			}

			BeginWaitCursor();
			fConvertSuccess = Convert_To_NLB_Engine_v10(str_NLB_Conv_Result, fNeedToStartCompileOnError);
			EndWaitCursor();
			if (!fConvertSuccess)
			{
				// Ошибка конвертации!
				if (sz_NLBE_10_Buf) delete []sz_NLBE_10_Buf;
				FileToSave.Close();	// Закрыть так неосмотрительно, с такой надеждой открытый файл...
				if (fNeedToStartCompileOnError) OnBuildCompile();	// Если произошла ошибка, вызовем компиляцию и разберемся...
				return;	// И вернуть ошибку!
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
			// Если не вышло, загрузим заголовок по умолчанию
			if (strTitle.GetLength() == 0) strTitle.LoadString(IDS_STRING_NLB_DEFAULT_TITLE);

			strResult.Format(sz_NLBE_10_Buf, strTitle, str_NLB_Conv_Result);
			try
			{
				lpBuf = (LPVOID) strResult.GetBuffer(strResult.GetLength());
			}
			catch (CMemoryException* pex)			
			{			
				// Операция провалилась, о чем надо сказать пользователю
				m_strMessage.LoadString(IDS_ERR_OUT_OF_MEMORY);				
				pex->GetErrorMessage(strErrorDescrptn, MAX_LOADSTRING);
				m_strMessage += strErrorDescrptn;
				strCaption.LoadString(IDS_ERROR_MB_CAPT);

				MessageBox(m_strMessage, strCaption, MB_ICONERROR);

				if (sz_NLBE_10_Buf) delete []sz_NLBE_10_Buf;
				FileToSave.Close();	// Закрыть так неосмотрительно, с такой надеждой открытый файл...				
				return;	// И вернуть ошибку!
			}

			try
			{
				BeginWaitCursor();
				FileToSave.Write(lpBuf, 
					strResult.GetLength()*sizeof(TCHAR)	// Т.к. GetLength возвращает число СИМВОЛОВ, а не байт (проверено!)
					);
				EndWaitCursor();
			}
			catch (CFileException* pex)
			{
				EndWaitCursor();	// Кончаем, записать не получилось
				// Операция провалилась, о чем надо сказать пользователю
				m_strMessage.LoadString(IDS_ERR_FILE_CORRUPT);
				pex->GetErrorMessage(strErrorDescrptn, MAX_LOADSTRING);
				m_strMessage += strErrorDescrptn;
				strCaption.LoadString(IDS_ERROR_MB_CAPT);

				MessageBox(m_strMessage, strCaption, MB_ICONERROR);
				// return; не вызываем, так как должен быть исполнен код (*)
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
	CDC* pdc;	// Контекст данного окна	

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//Подготовка контекста памяти
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
	
	//m_DCOffscreen.SelectObject(m_pBmpOld); - не нужно, в MFC такое даже будет ошибкой!!!
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
	//return CView::OnEraseBkgnd(pDC); - нетушки
	return TRUE;	// Типа все стерли :-)
}

VOID CNLB_BldrView::CountSelRctCoords(RECT& rct)
{
	// Предполагается, что m_ptBeginDragSelBox и m_ptEndDragSelBox корректны
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
	int i;	// Для цикла for
	RECT RctClient;	// Окно вывода (клиентская область окна)

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_fptBeginDragSet && m_fptEndDragSet)
	{
		CRect rct;
		CountSelRctCoords(rct);
		m_DCOffscreen.DrawFocusRect(&rct);		// Пока только рисуем. А вот на LBUTTONUP еще и выделение сделаем.
	}
	else
	{	
		RctClient.left = 0;
		RctClient.top = 0;
		RctClient.right = m_iClientWidth;
		RctClient.bottom = m_iClientHeight;		

		// Стираем страницы в старых позициях, если они попадают в видимую область экрана
		pos = pDoc->m_lstPages.GetHeadPosition();	
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (i=0; i<iPagesCount; i++)
		{			
			pPageCur = pDoc->m_lstPages.GetAt(pos);
			if (pPageCur->IsIntersect(RctClient) || // Если попадает в видимую область
				pPageCur->NeedToEraseWhenNotVisible())	// Или же, может, даже не попадает, но стереть в старой позиции все же надо
			{
				pPageCur->NeedToEraseWhenNotVisible() = FALSE;	// Чтобы в другой раз, если страница не видна, уже не стирать ее на экране, а вообще никак не реагировать.
				dwStyle = PARAM_ERASE;		
				if (pDoc->m_pStartPoint == pPageCur)
					dwStyle |= PARAM_IS_STARTPOINT;						
				
				pPageCur->Draw(&m_DCOffscreen, dwStyle);			
			}
			pDoc->m_lstPages.GetNext(pos);
		}	

		// Стираем дуги в старых позициях, если они попадают в видимую область экрана
		pos = pDoc->m_lstLnks.GetHeadPosition();
		dwStyle = PARAM_ERASE;
		iLnksCount = pDoc->m_lstLnks.GetCount();
		for (i=0; i<iLnksCount; i++)
		{		
			pCurLnk = pDoc->m_lstLnks.GetAt(pos);
			if (pCurLnk->IsIntersect(RctClient) || // Если попадает в видимую область
				pCurLnk->NeedToEraseWhenNotVisible())	// Или же, может, даже не попадает, но стереть в старой позиции все же надо
			{
				pCurLnk->NeedToEraseWhenNotVisible() = FALSE;	// Чтобы в другой раз, если страница не видна, уже не стирать ее на экране, а вообще никак не реагировать.
				pCurLnk->Draw(&m_DCOffscreen, dwStyle);
			}
			pDoc->m_lstLnks.GetNext(pos);
		}

		// Рисуем страницы в новых позициях, если они попадают в видимую область экрана
		// Если попадают, поставим им флаг, чтобы стереть на следующей прорисовке и больше не трогать.
		pos = pDoc->m_lstPages.GetHeadPosition();
		iPagesCount = pDoc->m_lstPages.GetCount();
		for (i=0; i<iPagesCount; i++)
		{			
			BOOL fNeedToDraw;

			pPageCur = pDoc->m_lstPages.GetAt(pos);

			if (pPageCur->IsIntersect(RctClient))
			{				
				pPageCur->NeedToEraseWhenNotVisible() = TRUE;	// Если попадают, поставим им флаг, чтобы стереть на следующей прорисовке.
				// Если же не попадают, то при стирании флаг обнулится, и в следующий раз не будет даже стирания, что ускоряет работу.
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

		// Рисуем дуги в новых позициях, если они попадают в видимую область экрана
		pos = pDoc->m_lstLnks.GetHeadPosition();			
		iLnksCount = pDoc->m_lstLnks.GetCount();
		for (i=0; i<iLnksCount; i++)
		{		
			BOOL fNeedToDraw;
			
			pCurLnk = pDoc->m_lstLnks.GetAt(pos);

			if (pCurLnk->IsIntersect(RctClient))
			{
				pCurLnk->NeedToEraseWhenNotVisible() = TRUE;	// Если попадают, поставим им флаг, чтобы стереть на следующей прорисовке.
				// Если же не попадают, то при стирании флаг обнулится, и в следующий раз не будет даже стирания, что ускоряет работу.
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

		// Кроме того, нарисуем элемент, на котором сейчас фокус, еще раз поверх всех!
		// Иначе получается некрасиво. 	

		if (m_pLastSelectedBookItem &&	// Если что-то выбрано...
				!m_fIsDraggingInProgress &&		// ...и ничего не перетаскивается...
				!m_fIsResizingNow &&		// ...и ничто не изменяет размеры
				m_pLastSelectedBookItem->IsIntersect(RctClient)) // А самое главное - этот элемент виден
		{
			dwStyle = PARAM_DRAW | PARAM_DRAW_FOCUS;	
			if (pDoc->m_pStartPoint == m_pLastSelectedBookItem)
				dwStyle |= PARAM_IS_STARTPOINT;		
			m_pLastSelectedBookItem->Draw(&m_DCOffscreen, dwStyle);
		}	
		// NeedToEraseWhenNotVisible ему ставить не надо, так как если это было необходимо,
		// это уже было сделано в одном из циклов выше
	}

	if (fPaintOnScreenImmediately) Invalidate();
}

void CNLB_BldrView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// Потрем фон
	m_DCOffscreen.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	m_pPageFrom = m_pPageTo = NULL;
	itSelItemType = SelItem_None;
	tb_Add_Option = OptAdd_None;
	m_pLastSelectedBookItem = NULL;	// Теперь ничего не выбрано!	
	// Страницу больше перемещать не надо	
	m_fIsDraggingStarted = FALSE;
	m_fIsDraggingInProgress = FALSE;
	m_fIsResizingNow = FALSE;	// Изменять размер тоже не надо
	m_ixOffset = m_iyOffset = 0;
	UpdateSB();	// Надо разобраться и со скроллингом...
	PaintInMemory(TRUE);	// Если содержимое документа изменилось, прежде всего следует перерисовать контекст памяти...
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

	iResult += I_ADDITIONAL_SPACE_FOR_X;	// Чуть подальше края страницы, чтобы можно было помещать новые страницы
	 	
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
	
	iResult -= I_ADDITIONAL_SPACE_FOR_X;	// Чуть подальше края страницы, чтобы можно было помещать новые страницы

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
	
	iResult += I_ADDITIONAL_SPACE_FOR_Y;	// Чуть подальше края страницы, чтобы можно было помещать новые страницы

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
	
	iResult -= I_ADDITIONAL_SPACE_FOR_Y;	// Чуть подальше края страницы, чтобы можно было помещать новые страницы

	if (iResult > 0) iResult = 0;
	iResult += m_iyOffset;
	return iResult;
}

VOID CNLB_BldrView::UpdateSB()
// Обновить скроллбары
{	
	m_siVert.nMax = GetMaximumPageYCoord();
	m_siVert.nMin = GetMinimumPageYCoord();
	m_siVert.nPage = m_iClientHeight;
	m_siVert.nPos = m_iyOffset;			
	if ((m_siVert.nMax <= m_iClientHeight + m_iyOffset - 1 + I_ADDITIONAL_SPACE_FOR_Y) && 
		(m_siVert.nMin >= m_iyOffset - I_ADDITIONAL_SPACE_FOR_Y))
		ShowScrollBar(SB_VERT, FALSE);	// Если он не нужен, спрячем его
	else
	{
		// Если он нужен, доделаем необходимое и покажем его		
		SetScrollInfo(SB_VERT, &m_siVert, TRUE);
		ShowScrollBar(SB_VERT, TRUE);	
	}
		
	m_siHorz.nMax = GetMaximumPageXCoord();
	m_siHorz.nMin = GetMinimumPageXCoord();
	m_siHorz.nPage = m_iClientWidth;
	m_siHorz.nPos = m_ixOffset;	
	if ((m_siHorz.nMax <= m_iClientWidth + m_ixOffset - 1 + I_ADDITIONAL_SPACE_FOR_X) && 
		(m_siHorz.nMin >= m_ixOffset - I_ADDITIONAL_SPACE_FOR_X))
		ShowScrollBar(SB_HORZ, FALSE);	// Если он не нужен, спрячем его
	else
	{
		// Если он нужен, доделаем необходимое и покажем его		
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
			m_siHorz.fMask = uiOldMask;	// Так как предыдущая функция маску МЕНЯЕТ!
			m_ixOffset = m_siHorz.nTrackPos;	// Так надежнее, чем через nPos. В моем случае nPos ГЛЮЧИЛ!!!
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
			m_siVert.fMask = uiOldMask;		// Так как предыдущая функция маску МЕНЯЕТ!
			m_iyOffset = m_siVert.nTrackPos;	// Так надежнее, чем через nPos. В моем случае nPos ГЛЮЧИЛ!!!
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

	// Убьем старые кисть и перо...		
	m_BrushOffscreen.DeleteObject();	
	m_PenOffscreen.DeleteObject();
	
	m_BrushOffscreen_ForDrag.DeleteObject();	
	m_PenOffscreen_ForDrag.DeleteObject();

	// ...cоздадим новые кисть и перо...
	m_BrushOffscreen.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));

	m_BrushOffscreen_ForDrag.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PenOffscreen_ForDrag.CreatePen(PS_INSIDEFRAME, I_PEN_WIDTH, GetSysColor(COLOR_WINDOW));

	// ...и выберем их в контексте...
	m_DCOffscreen.SelectObject(&m_BrushOffscreen);
	m_DCOffscreen.SelectObject(&m_PenOffscreen);

	m_DCOffscreen_ForDrag.SelectObject(&m_BrushOffscreen_ForDrag);
	m_DCOffscreen_ForDrag.SelectObject(&m_PenOffscreen_ForDrag);

	// Обновим цвет фона во всем контексте...
	m_DCOffscreen.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);
	m_DCOffscreen_ForDrag.PatBlt(0,0,m_iHorzRes,m_iVertRes,PATCOPY);

	// И отобразим документ!
	PaintInMemory();
}

VOID CNLB_BldrView::ClearInMemory()
{
	m_DCOffscreen.PatBlt(0,0,m_iClientWidth,m_iClientHeight,PATCOPY);
	m_DCOffscreen_ForDrag.PatBlt(0,0,m_iClientWidth,m_iClientHeight,PATCOPY);
}

LRESULT CNLB_BldrView::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	CDC* pdc;	// Контекст данного окна	

	// Убьем все старое...	
	m_BmpOffscreen.DeleteObject();	
	m_BrushOffscreen.DeleteObject();	
	m_PenOffscreen.DeleteObject();
	m_DCOffscreen.DeleteDC();
	
	m_BmpOffscreen_ForDrag.DeleteObject();	
	m_BrushOffscreen_ForDrag.DeleteObject();	
	m_PenOffscreen_ForDrag.DeleteObject();
	m_DCOffscreen_ForDrag.DeleteDC();

	// И сделаем все заново!
	//Подготовка контекста памяти
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

	// И отобразим документ!
	PaintInMemory();
	return 0;
}

void CNLB_BldrView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// Вообще-то, перед этим будет обработан еще и обработчик одиночного клика, т.к. я не поставил таймер... Но это мне и нужно!
	if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_BookPage))	// Приступим к редактированию страницы...
	{
		OnEditEditpageEditproperties();
	}
	else if (m_pLastSelectedBookItem &&
		(itSelItemType == SelItem_Link))	// Приступим к редактированию ссылки...
	{
		OnEditEditlinkEditproperties();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CNLB_BldrView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_DELETE:	// Нужно стереть элемент, на котором стоит фокус
		{
			CNLB_BldrDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);

			if (pDoc->m_iSelectedPagesCount != 0)	// Что-то выделено в страницах => приступим к удалению страниц...
			{
				OnEditEditpageDeletepage();
			}
			else if (m_pLastSelectedBookItem &&
				(itSelItemType == SelItem_Link))	// Приступим к удалению ссылки...
			{
				OnEditEditlinkDeletelink();
			}
		}
		break;
	case VK_SPACE:
	case VK_RETURN:	// Нужно редактировать элемент, на котором стоит фокус	
		{
			if (m_pLastSelectedBookItem &&
				(itSelItemType == SelItem_BookPage))	// Приступим к редактированию страницы...
			{
				OnEditEditpageEditproperties();
			}
			else if (m_pLastSelectedBookItem &&
				(itSelItemType == SelItem_Link))	// Приступим к редактированию ссылки...
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
	BOOL fOk = true;	// Будем оптимистами
	CLink* pCurLnk;
	CBookVar* pCurBookVar;
	CNLB_BldrApp* pNLB_Bldr = (CNLB_BldrApp*) AfxGetApp();

	if (pNLB_Bldr)
	{
		CNLB_BldrDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		if (pDoc->m_pStartPoint)
		{
			if (m_fIsDialogComperrActivated) m_pDlgComperr->OnOkDialogComperr();	// Если диалог ошибок был открыт до вызова Compile, сначала закроем его...	
			// ...и тут же создадим заново!
			m_pDlgComperr->Create(IDD_DIALOG_COMPERR, this);	// m_pDlgComperr != NULL
			// Может, он и не понадобится, но будем пессимистами. Если он не нужен, в конце этой функции он будет тихо и незаметно убит.

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
				
				m_pDlgComperr->DestroyWindow();	// Диалог со списком ошибок не понадобился!		
				iPagesCount = pDoc->m_lstPages.GetCount();
				iLnksCount = pDoc->m_lstLnks.GetCount();
				strSuccessText.Format(IDS_COMPILE_SUCCESSFUL, iPagesCount, iLnksCount);
				MessageBox(strSuccessText);		
			}
			else
			{
				m_pDlgComperr->ShowWindow(SW_SHOW);	// Показать ошибки... (m_pDlg != NULL)
				m_fIsDialogComperrActivated = TRUE;	// Теперь он действительно открыт!
			}
		}
		else	// Startpoint книги отсутствует!!!
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
	PaintInMemory();	// Надо еще отобразить все это, пока просто изменились некоторые числа
}

VOID CNLB_BldrView::SelectAll()
{
	BeginWaitCursor();
	POSITION posnew;	// Позиция для установки пометок
	int i;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	// Снимаем пометку со всех прочих страниц и дуг				
	posnew = pDoc->m_lstPages.GetHeadPosition();	
	int iPagesCount = pDoc->m_lstPages.GetCount();
	for (i=0; i<iPagesCount; i++)
	{						
		pDoc->m_lstPages.GetAt(posnew)->Select();	// Изменение m_iSelectedPagesCount будет в конце, сейчас его не трогаем!
		pDoc->m_lstPages.GetNext(posnew);
	}	
	
	posnew = pDoc->m_lstLnks.GetHeadPosition();		
	int iLnksCount = pDoc->m_lstLnks.GetCount();
	for (i=0; i<iLnksCount; i++)
	{		
		pDoc->m_lstLnks.GetAt(posnew)->Select();
		pDoc->m_lstLnks.GetNext(posnew);
	}
	pDoc->m_iSelectedPagesCount = iPagesCount;	// Все страницы помечены!
	EndWaitCursor();
}	

VOID CNLB_BldrView::DeselectAll()
{
	BeginWaitCursor();
	POSITION posnew;	// Позиция для снятия пометок
	int i;

	CNLB_BldrDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);	

	// Снимаем пометку со всех прочих страниц и дуг				
	posnew = pDoc->m_lstPages.GetHeadPosition();	
	int iPagesCount = pDoc->m_lstPages.GetCount();
	for (i=0; i<iPagesCount; i++)
	{						
		pDoc->m_lstPages.GetAt(posnew)->Deselect();	// Обнуление m_iSelectedPagesCount будет в конце, сейчас его не трогаем!
		pDoc->m_lstPages.GetNext(posnew);
	}	
	
	posnew = pDoc->m_lstLnks.GetHeadPosition();		
	int iLnksCount = pDoc->m_lstLnks.GetCount();
	for (i=0; i<iLnksCount; i++)
	{		
		pDoc->m_lstLnks.GetAt(posnew)->Deselect();
		pDoc->m_lstLnks.GetNext(posnew);
	}
	pDoc->m_iSelectedPagesCount = 0;	// Все пометки сняты
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
			// Запишем ссылки (номера страниц в списке)
			ar << pDoc->FindPageIndexByPointer(pCurLnk->Get_pPageFrom());
			ar << pDoc->FindPageIndexByPointer(pCurLnk->Get_pPageTo());
			// Указатели при вставке будут перелинкованы, все OK			
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
			ar << pDoc->FindPageIndexByPointer(pCurPage);	// Указатели при вставке будут перелинкованы, все OK
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

	DeselectAll();	// Так как будет новый Selection - то, что сейчас будет вставлено
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
		Может быть такая проблема. Сделали страницу, задали ей страничную
		переменную, скопировали и вставили в ту же книгу. Получится 
		дублирование переменной, что недопустимо. Потому вставляемое значение
		переменной проверяется, и если такая уже есть, то она корректируется.
		*/

		ar >> iBufferForPtrs;
		if (iBufferForPtrs == -1) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_CLIPBOARD_ERROR);
		pPagePtrRead = (CBookPage*) iBufferForPtrs;
		PageRead.Serialize(ar);
		PageRead.Get_iTop() -= iTopMin;
		PageRead.Get_iLeft() -= iLeftMin;
		strInitialPageVar = PageRead.Get_strPageVar();
		dwCntr = 0;
		while (pDoc->FindPageIndexByVar(PageRead.Get_strPageVar()) != -1)	// Если такая переменная в книге уже есть...
		{
			// Тогда изменим переменную
			PageRead.Get_strPageVar().Format(IDS_STRING_PAGEVAR_BUFFER_FOR_PASTE, 
				strInitialPageVar, 
				dwCntr++);
			if (dwCntr >= DW_MAX_DWORD)	// Весьма маловероятное событие :-)
			{
				// В этом случае просто положим переменную пустой, вот и все!
				PageRead.Get_strPageVar() = _T("");
				break;
			}
		}

		if ((pPageToInsert = new CBookPage(PageRead))==NULL) ((CNLB_BldrApp*) AfxGetApp())->HandleError(IERR_OUT_OF_MEMORY);
		// Перелинкуем указатели в списке ссылок, так как там записаны старые значения		
		pDoc->RelinkPointers(lstLnksRead, pPagePtrRead, pPageToInsert);					
		pDoc->m_lstPages.AddHead(pPageToInsert);		
	}		

    ar.Close();
    mf.Detach();
    CloseClipboard();

	pDoc->m_iSelectedPagesCount = iSavedPagesCount;	// После вставки стало именно так.
	pDoc->m_lstLnks.AddHead(&lstLnksRead);	// Теперь все ссылки перелинкованы и их можно вставить в основной список

	// Очистим вспомогательный список...
	pos = lstLnksRead.GetHeadPosition();
	while (!lstLnksRead.IsEmpty())
	{
		pos_remov = pos;
		lstLnksRead.GetNext(pos);
		lstLnksRead.RemoveAt(pos_remov);
	}
	// Очистим вспомогательные списки документа, так как они были использованы в RelinkPointers
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

	pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...	
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
	OnEditCopy();	// Скопировать страницы в клипборд...
	DeleteSelectedPages();	// ...и удалить их из списков
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
	BOOL fFound = FALSE;	// Будем пессимистами				
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
				if (m_fIsDialogSearchResActivated) m_pDlgSearchRes->OnOkDialogSearchResults();	// Если диалог был открыт до вызова, сначала закроем его...	
				// ...и тут же создадим заново!
				m_pDlgSearchRes->Create(IDD_DIALOG_SEARCH_RESULTS, this);	// m_pDlgSearchRes != NULL
				// Может, он и не понадобится, но будем пессимистами. Если он не нужен, в конце этой функции он будет тихо и незаметно убит.
				
				BeginWaitCursor();

				if (!dlg.m_fCaseSensitive)	// Иначе strToFindUppercase нам вообще не понадобится
				{
					strToFindUppercase = dlg.m_strToFind;
					strToFindUppercase.MakeUpper();
				}

				if (dlg.m_fWholeWords)	// Иначе iStrLenStrToFind нам вообще не понадобится
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

							// Т.е. если попали сюда, то fPageFound == TRUE.Но надо проверить, что это целое слово (если надо) 
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

										iPosFindFromStr[i]++;	// На всякий случай сразу приготовимся к новому поиску
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

								if (fPageFound) break;	// Это действительно целое слово!
								// А иначе будет новый поиск, уже со сдвинутой позицией в строке!
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

							// Т.е. если попали сюда, то fLinkFound == TRUE.Но надо проверить, что это целое слово (если надо) 
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

										iPosFindFromStr[i]++;	// На всякий случай сразу приготовимся к новому поиску
									}
									else
									{
										fMatchLeft[i] = FALSE;
										fMatchRight[i] = FALSE;
									}									
								}		
								
								fLinkFound = ((fMatchLeft[0] && fMatchRight[0]) ||
									(fMatchLeft[1] && fMatchRight[1]));

								if (fLinkFound) break;	// Это действительно целое слово!
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

							// Т.е. если попали сюда, то fBookVarFound == TRUE.Но надо проверить, что это целое слово (если надо) 
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

										iPosFindFromStr[i]++;	// На всякий случай сразу приготовимся к новому поиску
									}
									else
									{
										fMatchLeft[i] = FALSE;
										fMatchRight[i] = FALSE;
									}
								}		
								
								fBookVarFound = ((fMatchLeft[0] && fMatchRight[0]) ||
									(fMatchLeft[1] && fMatchRight[1]));

								if (fBookVarFound) break;	// Это действительно целое слово!
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

							// Т.е. если попали сюда, то fProbVarFound == TRUE.Но надо проверить, что это целое слово (если надо) 
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

									iPosFindFromStr[i]++;	// На всякий случай сразу приготовимся к новому поиску
								}
								else
								{
									fMatchLeft[0] = FALSE;
									fMatchRight[0] = FALSE;
								}								
								
								fProbVarFound = (fMatchLeft[0] && fMatchRight[0]);

								if (fProbVarFound) break;	// Это действительно целое слово!
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
					m_pDlgSearchRes->ShowWindow(SW_SHOW);	// Показать результаты поиска... (m_pDlgSearchRes != NULL)
					m_fIsDialogSearchResActivated = TRUE;	// Теперь он действительно открыт!
				}
				else
				{			
					CString strTextNotFound;
					strTextNotFound.LoadString(IDS_TEXT_NOT_FOUND);
					m_pDlgSearchRes->DestroyWindow();	// Диалог не понадобился!											
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
		if (pDoc->m_lstPages.GetCount() < 2)	// Для того, чтобы провести ссылку, нужно как минимум две страницы
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
	wParam = SB_THUMBPOSITION;	// Вообще-то в верхнее слово надо ложить число, на которое сместить позицию скроллинга, но т.к. мы используем SetScrollPos(), то на это можно забить
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
	
	// return CView::OnMouseWheel(nFlags, zDelta, pt); - может быть конфликт, если спецпрога для мыши с колесиком сама решит вызывать функции скроллинга. Не дадим ей этого!
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
	dlg.m_fAbortMassOperationFlag = FALSE;	// Это приводит к появлению кнопки Abort mass operation
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

		// dlg.m_ListLinksFrom пока пуст, так как объект только что создан
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
			pDoc->SetModifiedFlag(TRUE);	// Документ был изменен...
			PaintInMemory();
		}
		else if (iDoModalRes == IDCANCEL)
		{
			if (dlg.m_fAbortMassOperationFlag) fNeedBreak = TRUE;
		}

		// В конце концов очистим вспомогательный список диалога
		pos = dlg.m_ListLinksFrom.GetHeadPosition();
		while (!(dlg.m_ListLinksFrom.IsEmpty()))	
		{					
			pos_remov = pos;
			plcas = dlg.m_ListLinksFrom.GetNext(pos);
			if (plcas)
			{				
				if (plcas->lpszLinkCaptionForLV) delete []plcas->lpszLinkCaptionForLV;
				if (plcas->lpszLinkConstraintsForLV) delete []plcas->lpszLinkConstraintsForLV;
				// plcas->pLink не удаляем!!!
				delete plcas;
			}
			dlg.m_ListLinksFrom.RemoveAt(pos_remov);
		}

		if (fNeedBreak) break;	// Завершим цикл обработки досрочно

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
