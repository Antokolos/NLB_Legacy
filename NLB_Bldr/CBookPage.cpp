#include "stdafx.h"
#include "resource.h"
#include <afxwin.h>
#include "CBookPage.h"
#include "consts.h"
#include <math.h>

CBookPage::CBookPage()
{
	m_strPageText = _TEXT("");
	m_strPageVar = _TEXT("");
	m_strPageCaption.LoadString(IDS_STR_DEFAULT_PAGE_CAPT);
	m_fUsePageCaption = FALSE;
	m_iTopOld = m_iTop = 0;
	m_iLeftOld = m_iLeft = 0;
	m_iWidthOld = m_iWidth = I_DEFAULT_PAGE_WIDTH;		
	m_iHeightOld = m_iHeight = I_DEFAULT_PAGE_HEIGHT;		
	m_bWasFocusedLastTime = FALSE;
	// Будет также вызван конструктор CDrawable по умолчанию...
}

CBookPage::~CBookPage()
{		
	// Будет также вызван деструктор CDrawable...
}

CBookPage::CBookPage(CString& strPageCaption, int iTop, int iLeft)
{
	// Эти параметры берем все равно по умолчанию
	m_strPageText = _TEXT("");
	m_strPageVar = _TEXT("");		
	m_iWidthOld = m_iWidth = I_DEFAULT_PAGE_WIDTH;		
	m_iHeightOld = m_iHeight = I_DEFAULT_PAGE_HEIGHT;				
	m_bWasFocusedLastTime = FALSE;
	m_fUsePageCaption = FALSE;
	// Будет также вызван конструктор CDrawable по умолчанию...

	m_strPageCaption = strPageCaption;	
	m_iTopOld = m_iTop = iTop;
	m_iLeftOld = m_iLeft = iLeft;
}

CBookPage::CBookPage(const CBookPage& C) : CDrawable(C)
{	
	m_strPageText = C.m_strPageText;
	m_strPageCaption = C.m_strPageCaption;
	m_strPageVar = C.m_strPageVar;
	m_fUsePageCaption = C.m_fUsePageCaption;
	m_iTop = C.m_iTop;
	m_iLeft = C.m_iLeft;
	m_iWidth = C.m_iWidth;		
	m_iHeight = C.m_iHeight;		
	m_iTopOld = C.m_iTopOld;
	m_iLeftOld = C.m_iLeftOld;
	m_iWidthOld = C.m_iWidthOld;
	m_iHeightOld = C.m_iHeightOld;
	m_bWasFocusedLastTime = C.m_bWasFocusedLastTime;
}

CBookPage& CBookPage::operator = (const CBookPage& C)
{	
	if (this==&C) return *this;
	CDrawable::operator = (C);	// Присвоим их также и как CDrawable!
	m_strPageText = C.m_strPageText;
	m_strPageCaption = C.m_strPageCaption;	
	m_strPageVar = C.m_strPageVar;
	m_fUsePageCaption = C.m_fUsePageCaption;
	m_iTop = C.m_iTop;
	m_iLeft = C.m_iLeft;
	m_iWidth = C.m_iWidth;		
	m_iHeight = C.m_iHeight;	
	m_iTopOld = C.m_iTopOld;
	m_iLeftOld = C.m_iLeftOld;
	m_iWidthOld = C.m_iWidthOld;
	m_iHeightOld = C.m_iHeightOld;
	m_bWasFocusedLastTime = C.m_bWasFocusedLastTime;
	return *this;
}

VOID CBookPage::Draw(CDC* pDC, DWORD dwParams /*= 0*/)
{
	RECT rct;
	COLORREF crOldColor;
	COLORREF crOldTxtColor;	
	CPen* pOldPen;
	CBrush* pOldBrush;
	CString strToPrint;
	CString strFormattedVarName;
	CString strFormattedVarTemplate;		
		
	if ((dwParams & PARAM_ERASE) == PARAM_ERASE)
	{
		// Если хоть что-то в положении страницы изменилось, а именно:	
		if ((m_iLeftOld != m_iLeft)||	// Изменилась координата по x левого верхнего угла
			(m_iTopOld != m_iTop)||		// Изменилась координата по y левого верхнего угла
			(m_iWidthOld != m_iWidth)||		// Ширина
			(m_iHeightOld != m_iHeight)||	// Высота
			(m_bWasFocusedLastTime != ((dwParams & PARAM_DRAW_FOCUS) == PARAM_DRAW_FOCUS)))	// Поставили или убрали фокус...
		{
			// То тогда надо бы залить страницу в старой позиции цветом фона, дабы стереть!
			// Кстати, R2_XORPEN применить не получилось из-за того, что надписи (DrawText) подчиняться
			// закону XOR не хотят :-( Но и так вышло неплохо :-)
			if (m_bWasFocusedLastTime)
				pDC->PatBlt(m_iLeftOld - I_FOCUS_RECT_OFFSET, 
					m_iTopOld - I_FOCUS_RECT_OFFSET, 
					m_iWidthOld + 2*I_FOCUS_RECT_OFFSET,
					m_iHeightOld + 2*I_FOCUS_RECT_OFFSET,
					PATCOPY);
			else			
				pDC->PatBlt(m_iLeftOld, 
					m_iTopOld, 
					m_iWidthOld,
					m_iHeightOld, 
					PATCOPY);	
		}
	}

	// Вычислим новые координаты
	rct.left = m_iLeft;
	rct.top = m_iTop;
	rct.bottom = m_iTop + m_iHeight;
	rct.right = m_iLeft + m_iWidth;	

	if (dwParams != PARAM_ERASE)	// Если целью вызова не было только затереть страницу на старом месте
	{
		CPen thePen;
		CBrush theBrush;		
		
		thePen.CreatePenIndirect(&m_theLogPen);
		theBrush.CreateBrushIndirect(&m_theLogBrush);

		if ((dwParams & PARAM_DRAW_FOCUS) == PARAM_DRAW_FOCUS)
		{
			POINT ptWhere;
			pOldPen = pDC->SelectObject(&thePen);

			ptWhere.x = rct.left;
			ptWhere.y = rct.top;
			DrawFocusByPoint(pDC, ptWhere);
			ptWhere.x = rct.left;
			ptWhere.y = rct.bottom;
			DrawFocusByPoint(pDC, ptWhere);
			ptWhere.x = rct.right;
			ptWhere.y = rct.top;
			DrawFocusByPoint(pDC, ptWhere);
			ptWhere.x = rct.right;
			ptWhere.y = rct.bottom;
			DrawFocusByPoint(pDC, ptWhere);
			ptWhere.x = (rct.left + rct.right)/2;
			ptWhere.y = rct.bottom;
			DrawFocusByPoint(pDC, ptWhere);
			ptWhere.x = (rct.left + rct.right)/2;
			ptWhere.y = rct.top;
			DrawFocusByPoint(pDC, ptWhere);
			ptWhere.x = rct.left;
			ptWhere.y = (rct.top + rct.bottom)/2;
			DrawFocusByPoint(pDC, ptWhere);
			ptWhere.x = rct.right;
			ptWhere.y = (rct.top + rct.bottom)/2;
			DrawFocusByPoint(pDC, ptWhere);
			
			pDC->SelectObject(pOldPen);
		}

		if ((dwParams & PARAM_DRAW) == PARAM_DRAW)
		{		
			// А теперь рисуем собственно страницу
			pOldPen = pDC->SelectObject(&thePen);
			pOldBrush = pDC->SelectObject(&theBrush);	
			crOldTxtColor = pDC->SetTextColor(m_cr_Txt_Color);
			crOldColor = pDC->SetBkColor(m_theLogBrush.lbColor);
			pDC->Rectangle(&rct);	// Страница есть прямоугольник...

			// Изменим прямоугольник так, чтобы строчка круто влезла в него...
			if (m_theLogPen.lopnWidth.x!=0)
			{
				rct.top += m_theLogPen.lopnWidth.x;
				rct.bottom -= m_theLogPen.lopnWidth.x;
				rct.left += m_theLogPen.lopnWidth.x;
				rct.right -= m_theLogPen.lopnWidth.x;
			}	

			strToPrint = m_strPageCaption;	
			if (m_strPageVar != _TEXT(""))
			{
				strFormattedVarTemplate.LoadString(IDS_STR_PAGE_VAR);
				strFormattedVarName.Format(strFormattedVarTemplate, m_strPageVar);	
				strToPrint += strFormattedVarName;
			}

			pDC->DrawText(strToPrint, &rct, DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			// Если страница - Startpoint => отметим это
			if ((dwParams & PARAM_IS_STARTPOINT) == PARAM_IS_STARTPOINT) 
			{		
				CString strTxt;				
				strTxt.LoadString(IDS_STARTPOINT);
				pDC->DrawText(strTxt, &rct, DT_NOCLIP | DT_SINGLELINE | DT_LEFT);
			}
			pDC->SetBkColor(crOldColor);
			pDC->SetTextColor(crOldTxtColor);
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);	
		}

		// Попользовались и убили...
		thePen.DeleteObject();
		theBrush.DeleteObject();
	}

	m_iTopOld = m_iTop;
	m_iLeftOld = m_iLeft;
	m_iWidthOld = m_iWidth;
	m_iHeightOld = m_iHeight;	
	m_bWasFocusedLastTime = ((dwParams & PARAM_DRAW_FOCUS) == PARAM_DRAW_FOCUS);	
}

BOOL CBookPage::operator == ( const CBookPage& C) const 
{
	return ((m_strPageText == C.m_strPageText)&&
	(m_strPageCaption == C.m_strPageCaption)&&
	(m_strPageVar == C.m_strPageVar)&&
	(m_fUsePageCaption == C.m_fUsePageCaption)&&
	(m_iTop == C.m_iTop)&&
	(m_iLeft == C.m_iLeft)&&
	(m_iWidth == C.m_iWidth)&&
	(m_iHeight == C.m_iHeight));	
}

BOOL CBookPage::IsPointInside(CPoint& point)
{
	return ((m_iTop <= point.y)&&
			(m_iTop + m_iHeight >= point.y)&&
			(m_iLeft <= point.x)&&
			(m_iLeft + m_iWidth >= point.x));
}

BOOL CBookPage::IsIntersect(const RECT& rct)
{	
	if (((rct.top < m_iTop) && (rct.bottom < m_iTop)) ||
		((rct.top > m_iTop + m_iHeight) && (rct.bottom > m_iTop + m_iHeight)) ||
		((rct.left < m_iLeft) && (rct.right < m_iLeft)) ||
		((rct.left > m_iLeft + m_iWidth) && (rct.right > m_iLeft + m_iWidth)))
		return FALSE;
	else
		return TRUE;
}

double CBookPage::CountPageRadius(CBookPage* pPageRelativeTo)
{
	// При вычислении используем теорему Пифагора и подобие треугольников
	// Чтобы не возиться с ситуациями расположения, вычислим оба возможных радиуса и выберем наименьший.
	double flRadiusHorz;	// "Радиус по горизонтали"
	double flRadiusVert;	// "Радиус по вертикали"
	double flHorzKatetA;	// Для вычисления flRadiusHorz
	double flHorzKatetB;	// Для вычисления flRadiusHorz, вычисляется из подобия трегольников
	double flLambdaHorz;	// Коэффициент подобия для горизонтальных прямоугольников
	double flVertKatetA; 	// Для вычисления flRadiusVert
	double flVertKatetB;	// Для вычисления flRadiusVert, вычисляется из подобия трегольников
	double flLambdaVert;	// Коэффициент подобия для вертикальных прямоугольников
	POINT ptThis;	// Центр этой страницы
	POINT ptTo;	// Центр pPageRelativeTo

	if (!pPageRelativeTo) return -1;	// Ошибка!

	flHorzKatetA = (double) m_iWidth/2.0;
	flVertKatetA = (double) m_iHeight/2.0;
	// Центр этой страницы
	ptThis.y = Get_iTop() + Get_iHeight()/2;
	ptThis.x = Get_iLeft() + Get_iWidth()/2;
	// Центр pPageRelativeTo
	ptTo.y = pPageRelativeTo->Get_iTop() + pPageRelativeTo->Get_iHeight()/2;
	ptTo.x = pPageRelativeTo->Get_iLeft() + pPageRelativeTo->Get_iWidth()/2;

	flLambdaHorz = fabs((double) (ptTo.x - ptThis.x))/flHorzKatetA;
	flLambdaVert = fabs((double) (ptTo.y - ptThis.y))/flVertKatetA;

	flHorzKatetB = fabs((double) (ptTo.y - ptThis.y))/flLambdaHorz;
	flVertKatetB = fabs((double) (ptTo.x - ptThis.x))/flLambdaVert;

	flRadiusHorz = sqrt(flHorzKatetA*flHorzKatetA + flHorzKatetB*flHorzKatetB);	// Т. Пифагора
	flRadiusVert = sqrt(flVertKatetA*flVertKatetA + flVertKatetB*flVertKatetB);	// Т. Пифагора

	// Возвращаем наименьший из них
	if (flRadiusHorz < flRadiusVert)
		return flRadiusHorz;
	else
		return flRadiusVert;
}

void CBookPage::Serialize(CArchive& ar)
{
	CDrawable::Serialize(ar);
	if (ar.IsStoring())
	{
		// storing code 					
		ar << m_strPageText 
			<< m_strPageCaption 
			<< m_strPageVar 
			<< m_fUsePageCaption
			<< m_iTop
			<< m_iLeft
			<< m_iWidth
			<< m_iHeight;
	}
	else
	{
		// loading code		
		ar >> m_strPageText 
			>> m_strPageCaption 
			>> m_strPageVar 
			>> m_fUsePageCaption
			>> m_iTop
			>> m_iLeft
			>> m_iWidth
			>> m_iHeight;

		m_iTopOld = m_iTop;
		m_iLeftOld = m_iLeft;
		m_iWidthOld = m_iWidth;		
		m_iHeightOld = m_iHeight;		
		m_bWasFocusedLastTime = FALSE;
	}
}