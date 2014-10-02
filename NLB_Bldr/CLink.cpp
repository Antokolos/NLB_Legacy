#include "stdafx.h"
#include "CLink.h"
#include "consts.h"
#include "ClipFuncs\ClipFuncs.h"
#include <math.h>

CLink::CLink()
{
	// Именно так, все корректно! Эти указатели здесь НЕ создаются, они только ИСПОЛЬЗУЮТСЯ!
	m_pPageFrom = NULL;
	m_pPageTo = NULL;
	m_iLinkNumber = 0;
	m_strLinkCaption = _TEXT("");
	m_strLinkConstraints = _TEXT("");
	m_ptBeginOld.x = m_ptBeginOld.y = 0;	// Начало дуги (старое)
	m_ptEndOld.x = m_ptEndOld.y = 0;		// Конец дуги (старое)
	m_iRadiusFromOld = 0;	// "Радиус" начальной страницы (старый)
	m_iRadiusToOld = 0;	// "Радиус" конечной страницы (старый)
	m_bFirstTime = TRUE;	// Рисуем в первый раз?
	m_dwParamsOld = PARAM_NONE;	// Параметры предыдущей прорисовки

	m_lfRotatedFont.lfHeight = 0;	// Берет по умолчанию
	m_lfRotatedFont.lfWidth = 0;	// Берет по умолчанию
	m_lfRotatedFont.lfEscapement = m_lfRotatedFont.lfOrientation  = 0;
	m_lfRotatedFont.lfWeight = FW_MEDIUM;
	m_lfRotatedFont.lfItalic = FALSE;
	m_lfRotatedFont.lfUnderline = FALSE;
	m_lfRotatedFont.lfStrikeOut = FALSE;
	m_lfRotatedFont.lfCharSet = DEFAULT_CHARSET;
	m_lfRotatedFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lfRotatedFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lfRotatedFont.lfQuality = DEFAULT_QUALITY;
	m_lfRotatedFont.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
	CString strFontName = _T("Arial");
	_tcscpy(m_lfRotatedFont.lfFaceName, strFontName);	

	// Список инициализируется по умолчанию
	// Будет также вызван конструктор CDrawable по умолчанию...
}
	
CLink::~CLink()
{
	// Ничего не удаляем, т.к. см. зам. выше	
}

CLink::CLink(CBookPage* pPageFrom, CBookPage* pPageTo, const CString& strLinkCaption, int iLinkNumber)
{
	m_pPageFrom = pPageFrom;
	m_pPageTo = pPageTo;	
	m_iLinkNumber = iLinkNumber;
	m_strLinkConstraints = _TEXT("");

	m_strLinkCaption = strLinkCaption;
	m_ptBeginOld.x = m_ptBeginOld.y = 0;	// Начало дуги (старое)
	m_ptEndOld.x = m_ptEndOld.y = 0;		// Конец дуги (старое)
	m_iRadiusFromOld = 0;	// "Радиус" начальной страницы (старый)
	m_iRadiusToOld = 0;	// "Радиус" конечной страницы (старый)	
	m_dwParamsOld = PARAM_NONE;	// Параметры предыдущей прорисовки
	m_bFirstTime = TRUE;	// Рисуем в первый раз - Да! В результате старая позиция будет пересчитана...

	m_lfRotatedFont.lfHeight = 0;	// Берет по умолчанию
	m_lfRotatedFont.lfWidth = 0;	// Берет по умолчанию
	m_lfRotatedFont.lfEscapement = m_lfRotatedFont.lfOrientation  = 0;
	m_lfRotatedFont.lfWeight = FW_MEDIUM;
	m_lfRotatedFont.lfItalic = FALSE;
	m_lfRotatedFont.lfUnderline = FALSE;
	m_lfRotatedFont.lfStrikeOut = FALSE;
	m_lfRotatedFont.lfCharSet = DEFAULT_CHARSET;
	m_lfRotatedFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_lfRotatedFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_lfRotatedFont.lfQuality = DEFAULT_QUALITY;
	m_lfRotatedFont.lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
	CString strFontName = _T("Arial");
	_tcscpy(m_lfRotatedFont.lfFaceName, strFontName);	

	// Список инициализируется по умолчанию
	// Будет также вызван конструктор CDrawable по умолчанию...	
}

CLink::CLink(const CLink& C) : CDrawable(C)
{
	// Именно так, все корректно! Эти указатели здесь НЕ создаются, они только ИСПОЛЬЗУЮТСЯ!
	m_pPageFrom = C.m_pPageFrom;
	m_pPageTo = C.m_pPageTo;
	m_iLinkNumber = C.m_iLinkNumber;
	m_strLinkCaption = C.m_strLinkCaption;
	m_strLinkConstraints = C.m_strLinkConstraints;	
	m_BVModList = C.m_BVModList;

	m_ptBeginOld = C.m_ptBeginOld;	
	m_ptEndOld = C.m_ptEndOld;
	m_iRadiusFromOld = C.m_iRadiusFromOld;
	m_iRadiusToOld = C.m_iRadiusToOld;
	m_dwParamsOld = C.m_dwParamsOld;	
	m_bFirstTime = C.m_bFirstTime;

	m_lfRotatedFont = C.m_lfRotatedFont;	
}

CLink& CLink::operator = (const CLink& C)
{
	if (this==&C) return *this;
	CDrawable::operator = (C);	// Присвоим их также и как CDrawable!
	// Именно так, все корректно! Эти указатели здесь НЕ создаются, они только ИСПОЛЬЗУЮТСЯ!
	m_pPageFrom = C.m_pPageFrom;
	m_pPageTo = C.m_pPageTo;
	m_iLinkNumber = C.m_iLinkNumber;
	m_strLinkCaption = C.m_strLinkCaption;
	m_strLinkConstraints = C.m_strLinkConstraints;	
	m_BVModList = C.m_BVModList;

	m_ptBeginOld = C.m_ptBeginOld;	
	m_ptEndOld = C.m_ptEndOld;
	m_iRadiusFromOld = C.m_iRadiusFromOld;
	m_iRadiusToOld = C.m_iRadiusToOld;
	m_dwParamsOld = C.m_dwParamsOld;
	m_bFirstTime = C.m_bFirstTime;

	m_lfRotatedFont = C.m_lfRotatedFont;	

	return *this;
}

BOOL CLink::operator == ( const CLink& C) const
{
	/*return ((m_pPageFrom == C.m_pPageFrom)&&
		(m_pPageTo == C.m_pPageTo)&&
		(m_strLinkCaption == C.m_strLinkCaption)&&
		(m_strLinkConstraints == C.m_strLinkConstraints)&&
		(m_iLinkNumber == C.m_iLinkNumber)&&
		(m_BVModList == C.m_BVModList));*/
	return ((m_pPageFrom == C.m_pPageFrom) && (m_pPageTo == C.m_pPageTo));	
	// Правильнее было бы так, как написано выше, однако, нам необходимо, чтобы 
	// от страницы к странице можно было бы провести только одну ссылку, а тогда
	// можно будет провести сколько угодно, поменяв им, допустим, Caption
	// m_iLinkNumber здесь также не учитываем!
}


BOOL CLink::IsBothPagesIntersectRect(const RECT& rct)
{
	return (m_pPageFrom->IsIntersect(rct) && m_pPageTo->IsIntersect(rct));	
	// Так и только так!
}

BOOL CLink::IsIntersect(const RECT& rct)
{
	POINT ptBegin;	// Начало дуги
	POINT ptEnd;	// Конец дуги	

	// Центр начальной страницы
	ptBegin.y = m_pPageFrom->Get_iTop() + m_pPageFrom->Get_iHeight()/2;
	ptBegin.x = m_pPageFrom->Get_iLeft() + m_pPageFrom->Get_iWidth()/2;
	// Центр конечной страницы
	ptEnd.y = m_pPageTo->Get_iTop() + m_pPageTo->Get_iHeight()/2;
	ptEnd.x = m_pPageTo->Get_iLeft() + m_pPageTo->Get_iWidth()/2;
	return IsLineIntersectClipArea(ptBegin, ptEnd, rct);
}

VOID CLink::Draw(CDC* pDC, DWORD dwParams /*= 0*/)
{
	COLORREF crOldColor;
	COLORREF crOldTxtColor;
	LOGBRUSH lbrush;	
	CPen* pOldPen;
	CBrush* pOldBrush;
	
	// Стираем в старой позиции (цветом фона)...
	// (Толщина всех дуг одинакова (так сделано мной специально) и это хорошо :-)
	if ((dwParams & PARAM_ERASE) == PARAM_ERASE)
	{
		pOldBrush = pDC->GetCurrentBrush();
		crOldTxtColor = pDC->SetTextColor(GetSysColor(COLOR_WINDOW));
		DrawLinkMainProc(pDC, pOldBrush, m_dwParamsOld, TRUE);	
		pDC->SetTextColor(crOldTxtColor);
	}

	if ((dwParams & PARAM_DRAW) == PARAM_DRAW)
	{
		CPen thePen;
		CBrush theBrush;

		thePen.CreatePenIndirect(&m_theLogPen);
		theBrush.CreateBrushIndirect(&m_theLogBrush);

		pOldPen = pDC->SelectObject(&thePen);
		pOldBrush = pDC->SelectObject(&theBrush);
		crOldTxtColor = pDC->SetTextColor(m_cr_Txt_Color);
		theBrush.GetLogBrush(&lbrush);		
		crOldColor = pDC->SetBkColor(lbrush.lbColor);
		
		// Рисуем в новой позиции...	
		DrawLinkMainProc(pDC, pOldBrush, dwParams, FALSE);	

		pDC->SetBkColor(crOldColor);
		pDC->SetTextColor(crOldTxtColor);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush); 

		// Попользовались и убили...
		thePen.DeleteObject();
		theBrush.DeleteObject();
	}
	m_dwParamsOld = dwParams;
}

VOID CLink::DrawLinkMainProc(CDC* pDC, 
							 CBrush* pBrushBackground, 
							 DWORD dwParams, 
							 BOOL fCountInOldCoords)
{
	/*
	Чтобы постигнуть премудрости работы этого алгоритма, надо иметь знания по 
	аналитической геометрии (ВЕКТОРА)
	*/

	POINT ptBegin;	// Начало дуги
	POINT ptEnd;	// Конец дуги	
	POINT ptEnd_Arrow1;	// Точка для рисования стрелки на конце дуги
	POINT ptEnd_Arrow2;	// Точка для рисования стрелки на конце дуги
	POINT ptEndArrowOnMainLine;
	POINT_DOUBLE lineVectorTo;	// Вектор сдвига ptEnd на радиус конечной страницы (так, чтобы ptEnd оказалась на границе)
	POINT_DOUBLE lineVectorFrom;	// Вектор сдвига ptBegin на радиус начальной страницы (так, чтобы ptBegin оказалась на границе)
	POINT_DOUBLE lineVector_1;	// Вектор дуги (единичной длины)
	POINT_DOUBLE Ort_To_lineVector_1;	// Перпендикуляр к lineVector (нужен для рисования стрелки на конце дуги)
	int iRadiusFrom;	// "Радиус" начальной страницы (расстояние от центра прямоугольника до точки пересечения дуги с границей)
	int iRadiusTo;	// "Радиус" конечной страницы (расстояние от центра прямоугольника до точки пересечения дуги с границей)			
	double lineVectorNorm;	// Длина дуги в евклидовой норме		

	if (fCountInOldCoords&&!m_bFirstTime)
	{
		ptBegin = m_ptBeginOld;
		ptEnd = m_ptEndOld;
		iRadiusFrom = m_iRadiusFromOld;
		iRadiusTo = m_iRadiusToOld;
	}
	else
	{
		// Центр начальной страницы
		ptBegin.y = m_ptBeginOld.y = m_pPageFrom->Get_iTop() + m_pPageFrom->Get_iHeight()/2;
		ptBegin.x = m_ptBeginOld.x = m_pPageFrom->Get_iLeft() + m_pPageFrom->Get_iWidth()/2;
		// Центр конечной страницы
		ptEnd.y = m_ptEndOld.y = m_pPageTo->Get_iTop() + m_pPageTo->Get_iHeight()/2;
		ptEnd.x = m_ptEndOld.x = m_pPageTo->Get_iLeft() + m_pPageTo->Get_iWidth()/2;
		iRadiusFrom = m_iRadiusFromOld = (int) m_pPageFrom->CountPageRadius(m_pPageTo);
		iRadiusTo = m_iRadiusToOld = (int) m_pPageTo->CountPageRadius(m_pPageFrom);		
		m_bFirstTime = FALSE;	// Выходим на нормальный режим работы...
	}

	// Ищем вектор, коллинеарный нашей дуге
	lineVectorTo.x = ptEnd.x - ptBegin.x;
	lineVectorTo.y = ptEnd.y - ptBegin.y;
	// Ищем его норму (длина дуги)
	lineVectorNorm = sqrt(lineVectorTo.x*lineVectorTo.x + lineVectorTo.y*lineVectorTo.y);

	// Если длина дуги <= суммы радиусов начальной и конечной страницы => страницы пересекаются, и потому дугу между ними не рисуем вообще...
	if (lineVectorNorm > (iRadiusFrom + iRadiusTo))
	{
		// Иначе нарисуем дугу!

		// Нормировка
		lineVectorTo.x /= lineVectorNorm;
		lineVectorTo.y /= lineVectorNorm;
		lineVector_1 = lineVectorTo;	// Запомним вектор единичной длины (lineVectorTo в дальнейшем изменится, а вектор единичной длины еще понадобится)

		// Теперь делаем длину векторов соответствующей радиусам начальной и конечной страниц
		lineVectorFrom.x = lineVector_1.x * iRadiusFrom;
		lineVectorFrom.y = lineVector_1.y * iRadiusFrom;
		lineVectorTo.x *= iRadiusTo;
		lineVectorTo.y *= iRadiusTo;	

		// Сдвигаем начало дуги 
		ptBegin.x += (int) lineVectorFrom.x;
		ptBegin.y += (int) lineVectorFrom.y;

		// Сдвигаем конец дуги 
		ptEnd.x -= (int) lineVectorTo.x;	
		ptEnd.y -= (int) lineVectorTo.y;				

		// Рисуем дугу
		pDC->MoveTo(ptBegin);
		pDC->LineTo(ptEnd);

		// Перпендикуляр к вектору (a, b) - это (-b, a)
		Ort_To_lineVector_1.x = -lineVector_1.y;
		Ort_To_lineVector_1.y = lineVector_1.x;		

		// Вычислим координаты точек стрелки (запишите в векторном виде и нарисуйте, чтобы понять)
		ptEnd_Arrow1.x = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.x - Ort_To_lineVector_1.x*W_ARROW_HEIGHT_COEF) + ptEnd.x);
		ptEnd_Arrow1.y = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.y - Ort_To_lineVector_1.y*W_ARROW_HEIGHT_COEF) + ptEnd.y);
		ptEnd_Arrow2.x = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.x + Ort_To_lineVector_1.x*W_ARROW_HEIGHT_COEF) + ptEnd.x);
		ptEnd_Arrow2.y = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.y + Ort_To_lineVector_1.y*W_ARROW_HEIGHT_COEF) + ptEnd.y);

		ptEndArrowOnMainLine.x = (ptEnd_Arrow1.x + ptEnd_Arrow2.x)/2;
		ptEndArrowOnMainLine.y = (ptEnd_Arrow1.y + ptEnd_Arrow2.y)/2;

		// Рисуем стрелку на конце дуги
		pDC->LineTo(ptEnd_Arrow1);
		pDC->MoveTo(ptEnd);
		pDC->LineTo(ptEnd_Arrow2);		

		if (pBrushBackground)	// Если этот параметр равен NULL, то фокус рисоваться не будет!
		{
			if ((dwParams & PARAM_DRAW_FOCUS) == PARAM_DRAW_FOCUS) // Дуга была выделена
			{
				CBrush* pOldBrush;
				pOldBrush = pDC->SelectObject(pBrushBackground);
				DrawFocusByPoint(pDC, ptEnd);
				DrawFocusByPoint(pDC, ptEnd_Arrow1);
				DrawFocusByPoint(pDC, ptEnd_Arrow2);
				pDC->SelectObject(pOldBrush);
			}
		}
			
		UINT uiOldTextAlign;
		int iOldBKMode;
		CFont RotatedFont;		
		CFont* pFntOld;
		DWORD dwTextExtent;
		int iCaptionStrlen = m_strLinkCaption.GetLength();
		INT iFit;
		SIZE sSize;
		CString strLinkCaptionFitted;
		LONG lixDiff = (ptEndArrowOnMainLine.x-ptBegin.x);
		LONG liyDiff = (ptEndArrowOnMainLine.y-ptBegin.y);
		double flFittedStrlen = 0.8*sqrt((double) lixDiff*lixDiff + liyDiff*liyDiff);
	
		m_lfRotatedFont.lfEscapement = m_lfRotatedFont.lfOrientation = (LONG) -(atan(lineVector_1.y / lineVector_1.x) * 360 / (2*_PI))*10;
		RotatedFont.CreateFontIndirect(&m_lfRotatedFont);
		iOldBKMode = pDC->SetBkMode(TRANSPARENT);
		pFntOld = (CFont*) pDC->SelectObject(&RotatedFont);		

		// Эта странная проверка нам нужна только для того, чтобы отличать
		// друг от друга две ссылки, идущие в разных направлениях к одним и тем же 
		// страницам
		if ((DWORD) m_pPageFrom > (DWORD) m_pPageTo)
			uiOldTextAlign = pDC->SetTextAlign(TA_BOTTOM | TA_CENTER | TA_NOUPDATECP);		
		else
			uiOldTextAlign = pDC->SetTextAlign(TA_TOP | TA_CENTER | TA_NOUPDATECP);		

		dwTextExtent = ::GetTextExtentExPoint(pDC->GetSafeHdc(), m_strLinkCaption, iCaptionStrlen, (int) flFittedStrlen, &iFit, NULL, &sSize);
		if (iFit > 3)
		{
			if (iFit < iCaptionStrlen)
			{
				strLinkCaptionFitted = m_strLinkCaption.Mid(0, iFit - 3);
				strLinkCaptionFitted += _T("...");
			}
			else
				strLinkCaptionFitted = m_strLinkCaption.Mid(0, iFit);
		}
		else
			strLinkCaptionFitted = _T("...");

		pDC->TextOut((ptBegin.x + ptEndArrowOnMainLine.x)/2, (ptBegin.y + ptEndArrowOnMainLine.y)/2, strLinkCaptionFitted);
		pDC->SetTextAlign(uiOldTextAlign);
		pDC->SelectObject(pFntOld);		
		pDC->SetBkMode(iOldBKMode);
	}	
}

BOOL CLink::IsPointInside(CPoint& point)
{
	POINT ptBegin;	// Начало дуги
	POINT ptEnd;	// Конец дуги	
	double A;	// Коэффициент прямой
	double B;	// Коэффициент прямой
	double C;	// Коэффициент прямой
	double d;	// Расстояние от точки до дуги	
	long int lixDiff;
	long int liyDiff;
	
	// Центр начальной страницы
	ptBegin.y = m_pPageFrom->Get_iTop() + m_pPageFrom->Get_iHeight()/2;
	ptBegin.x = m_pPageFrom->Get_iLeft() + m_pPageFrom->Get_iWidth()/2;
	// Центр конечной страницы
	ptEnd.y = m_pPageTo->Get_iTop() + m_pPageTo->Get_iHeight()/2;
	ptEnd.x = m_pPageTo->Get_iLeft() + m_pPageTo->Get_iWidth()/2;

	// Изменим начало так, чтобы отслеживать точки с середины дуги (дуга может 
	// иметь два конца (т.е. две дуги, налегающие друг на друга), так что
	// надо иметь возможность различать, где кто)
	ptBegin.y = (ptBegin.y + ptEnd.y)/2;
	ptBegin.x = (ptBegin.x + ptEnd.x)/2;

	// Грубый предварительный отсев (по прямоугольнику, диагональю которого является дуга)
	// Просто проверить, попала ли точка в прямоугольник, нельзя, т.к.
	// прямоугольник может быть вырожденным в линию, и тогда мы не сможем ничего
	// пометить. Поэтому проверяем только те стороны данного прямоугольника,
	// разница координат между которыми наиболее велика
	// Правда, может быть еще случай вырождения в точку, но его не рассматриваем
	lixDiff = ptEnd.x - ptBegin.x;
	if (lixDiff < 0) lixDiff = -lixDiff;
	liyDiff = ptEnd.y - ptBegin.y;
	if (liyDiff < 0) liyDiff = -liyDiff;
	if (lixDiff > liyDiff)	// Проверяем по x
	{
		if (((point.x > ptBegin.x)&&(point.x > ptEnd.x)) ||	// Правее => не попали
			((point.x < ptBegin.x)&&(point.x < ptEnd.x))) 	// Левее => не попали
			return FALSE;
	}
	else	// Проверяем по y
	{
		if (((point.y > ptBegin.y)&&(point.y > ptEnd.y)) ||	// Выше => не попали
			((point.y < ptBegin.y)&&(point.y < ptEnd.y))) // Ниже => не попали
			return FALSE;
	}		

	// Если же мы все же внутри...
	/*
	Немного теории из анал. геометрии:
	Предположим, F(x, y) = A*x + B*y + C - прямая
	M(x1, y1) - точка на плоскости.
	Тогда расстояние от M до данной прямой вычисляется по формуле:

         abs(F(x1, y1))
    d = -----------------
	     sqrt(A*A + B*B)

    где:
    abs(x) - модуль x,
	sqrt(x) - квадратный корень из x
	*/

	// Ищем вектор, коллинеарный нашей дуге и выражаем A и B
	A = -(ptEnd.y - ptBegin.y);
	B = ptEnd.x - ptBegin.x;		

	// С находим, допустим, из того условия, что координаты точки 
	// ptBegin должны занулять F, ибо она лежит на прямой	
	C = -A*ptBegin.x - B*ptBegin.y;	// C = -A*x - B*y

	d = fabs(A*point.x + B*point.y + C)/sqrt(A*A + B*B);	// См. формулу выше

    if (d <= ARC_SELECTION_WIDTH) 
		return TRUE;	// Попали!
	else 
		return FALSE;	// Не попали!
}

void CLink::Serialize(CArchive& ar)
{
	CDrawable::Serialize(ar);
	if (ar.IsStoring())
	{
		// storing code 		
		// m_pPageFrom & m_pPageTo не сохраняем, возложим восстановление ссылок на документ	
		// Также на документ возлагается сохранение/восстановление списка модификаций
		ar << m_iLinkNumber;
		ar << m_strLinkCaption << m_strLinkConstraints;		
	}
	else
	{
		// loading code		
		m_pPageFrom = m_pPageTo = NULL;	// возложим восстановление ссылок на документ
		// Также на документ возлагается сохранение/восстановление списка модификаций

		ar >> m_iLinkNumber;
		ar >> m_strLinkCaption >> m_strLinkConstraints;		

		m_ptBeginOld.x = m_ptBeginOld.y = 0;	// Начало дуги (старое)
		m_ptEndOld.x = m_ptEndOld.y = 0;		// Конец дуги (старое)
		m_iRadiusFromOld = 0;	// "Радиус" начальной страницы (старый)
		m_iRadiusToOld = 0;	// "Радиус" конечной страницы (старый)	
		m_dwParamsOld = PARAM_NONE;	// Параметры предыдущей прорисовки
		m_bFirstTime = TRUE;	// Рисуем в первый раз - Да! В результате старая позиция будет пересчитана...
	}
}
