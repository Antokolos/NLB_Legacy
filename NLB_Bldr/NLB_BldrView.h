// NLB_BldrView.h : interface of the CNLB_BldrView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NLB_BLDRVIEW_H__B7981A3B_1A6C_484E_8455_F11AE2AC21E0__INCLUDED_)
#define AFX_NLB_BLDRVIEW_H__B7981A3B_1A6C_484E_8455_F11AE2AC21E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "consts.h"
#include "DialogComperr.h"
#include "DialogSearchResults.h"
#include "CBookPage.h"
#include "NLB_BldrDoc.h"

class CNLB_BldrView : public CView
{
protected: // create from serialization only
	CNLB_BldrView();
	DECLARE_DYNCREATE(CNLB_BldrView)

// Attributes
public:	
	CString m_strMessage;	// Строчка, которая будет вылазить во всяких там MessageBox
	HCURSOR hCurAddPage;
	HCURSOR hCurAddLink;
	CBookPage* m_pPageFrom;	// Страница, от которой проводим текущую ссылку
	CBookPage* m_pPageTo;	// Страница, к которой проводим текущую ссылку
	OptAdd tb_Add_Option;
	CNLB_BldrDoc* GetDocument();		
	BOOL m_fMouseDownNow;
	BOOL m_fIsDraggingStarted;	// Верно ли, что начато перетаскивание?
	BOOL m_fIsDraggingInProgress;	// Верно ли, что СЕЙЧАС перетаскиваются объекты?	
	BOOL m_fIsResizingNow;	// Верно ли, что сейчас изменяется размер какой-то страницы?
	CDrawable* m_pLastSelectedBookItem;	// Страница, которая была последней выделена (или NULL)
	SelItemType itSelItemType;
	int m_iOffsDragX;	// смещение по x
	int m_iOffsDragY;	// смещение по y
	CPoint ptStartDrag;	// Точка, в которой начато или продолжено перетасуивание

	int m_iClientWidth;
	int m_iClientHeight;
	int m_iVertRes;	// Вертикальное разрешение экрана
	int m_iHorzRes;	// Горизонтальное разрешение экрана
	//работа с заэкранным контекстом
	CBitmap m_BmpOffscreen;
	CDC m_DCOffscreen;
	CBrush m_BrushOffscreen;	
	CPen m_PenOffscreen;	
	//...плюс заэкранный контекст для отрисовки перетаскиваемого...
	CBitmap m_BmpOffscreen_ForDrag;	
	CDC m_DCOffscreen_ForDrag;
	CBrush m_BrushOffscreen_ForDrag;	
	CPen m_PenOffscreen_ForDrag;	

	// Скроллинг
	SCROLLINFO m_siVert;
	SCROLLINFO m_siHorz;
	int m_ixOffset;
	int m_iyOffset;

protected:
	CDialogComperr* m_pDlgComperr;
	CDialogSearchResults* m_pDlgSearchRes;
	CPoint m_ptBeginDragSelBox;	// Точка, в которой будет начало прямоугольника выделения (см. LButtonDown)
	BOOL m_fptBeginDragSet;		// Эта точка была поставлена!
	CPoint m_ptEndDragSelBox;		// Точка, в которой будет конец прямоугольника выделения
	BOOL m_fptEndDragSet;		// Эта точка была поставлена!	
	BOOL m_fMLeaved;

public:
	BOOL m_fIsDialogComperrActivated;	// Открыт ли на данный момент диалог с ошибками компиляции формулы?
	BOOL m_fIsDialogSearchResActivated;	// Открыт ли на данный момент диалог с результатами поиска?

// Operations
public:	
	VOID PaintInMemory(BOOL fPaintOnScreenImmediately = TRUE);
	VOID ClearInMemory();	// Очистить все заэкранные контексты с использованием PatBlt по текущим размерам окна. Медленная, не надо часто пользовать
	VOID ScrollToPage(CBookPage* pPage);
	VOID ScrollToLink(CLink* pLink);
	void EditlinkEditproperties();
	void EditpageEditproperties();
	VOID SelectAll();
	VOID DeselectAll();	
	BOOL EditlinkDeletelink(CLink* pLinkToDelete);
	BOOL EditlinkEditproperties(CLink* pLinkToEdit);

protected:
	int GetMaximumPageXCoord();
	int GetMinimumPageXCoord();
	int GetMaximumPageYCoord();
	int GetMinimumPageYCoord();
	VOID UpdateSB();
	VOID ShiftAllPagesHorz(int iShift);
	VOID ShiftAllPagesVert(int iShift);
	BOOL Convert_To_NLB_Engine_v10(CString& strResult, BOOL& fNeedStartCompileOnError);
	VOID SaveStringToFile_NLB_Engine10();		
	BOOL DrawDragged(CDC* pDC, BOOL fDraw = TRUE);
	BOOL InitializeTmeForLeave();	
	VOID DeleteSelectedPages();
	VOID CountSelRctCoords(RECT& rct);
	VOID SetWndCursor(HCURSOR Cur);
	int DeparametrizeText(CNLB_BldrDoc* pDoc,
							CString& strText, 
							CString& strFaultyVarName,
							CString& strIns, int iLenIns,
							CString& strPara, int iLenPara,
							CString& strDash, int iLenDash,
							CString& strApostrophe, int iLenApostrophe,
							CString& strBackslash, int iLenBackslash,
							CString& strStrVarParamSeps);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLB_BldrView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNLB_BldrView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNLB_BldrView)
	afx_msg void OnEditAddbookpage();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEditAddlink();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditAddstartpoint();
	afx_msg void OnEditEditpageDeletepage();
	afx_msg void OnEditEditpageEditproperties();
	afx_msg void OnUpdateEditEditpageDeletepage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditEditpageEditproperties(CCmdUI* pCmdUI);
	afx_msg void OnEditEditlinkDeletelink();
	afx_msg void OnEditEditlinkEditproperties();
	afx_msg void OnUpdateEditEditlinkDeletelink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditEditlinkEditproperties(CCmdUI* pCmdUI);
	afx_msg void OnFileExportTo_NLB_Engine10();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnDisplayChange(WPARAM, LPARAM);
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBuildCompile();
	afx_msg void OnEditSelectall();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditBookvariables();
	afx_msg void OnEditLeafs();
	afx_msg void OnEditProbabilityvariables();
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditAddstartpoint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditLeafs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildCompile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExporttoNlbenginev10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditAddlink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditChangeallpages();
	afx_msg void OnUpdateEditChangeallpages(CCmdUI* pCmdUI);
	afx_msg void OnEditStringvariables();
	afx_msg void OnEditTriggeredstrings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NLB_BldrView.cpp
inline CNLB_BldrDoc* CNLB_BldrView::GetDocument()
   { return (CNLB_BldrDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLB_BLDRVIEW_H__B7981A3B_1A6C_484E_8455_F11AE2AC21E0__INCLUDED_)
