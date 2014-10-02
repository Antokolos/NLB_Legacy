#include "stdafx.h"
#include "CLink.h"
#include "consts.h"
#include "ClipFuncs\ClipFuncs.h"
#include <math.h>

CLink::CLink()
{
	// ������ ���, ��� ���������! ��� ��������� ����� �� ���������, ��� ������ ������������!
	m_pPageFrom = NULL;
	m_pPageTo = NULL;
	m_iLinkNumber = 0;
	m_strLinkCaption = _TEXT("");
	m_strLinkConstraints = _TEXT("");
	m_ptBeginOld.x = m_ptBeginOld.y = 0;	// ������ ���� (������)
	m_ptEndOld.x = m_ptEndOld.y = 0;		// ����� ���� (������)
	m_iRadiusFromOld = 0;	// "������" ��������� �������� (������)
	m_iRadiusToOld = 0;	// "������" �������� �������� (������)
	m_bFirstTime = TRUE;	// ������ � ������ ���?
	m_dwParamsOld = PARAM_NONE;	// ��������� ���������� ����������

	m_lfRotatedFont.lfHeight = 0;	// ����� �� ���������
	m_lfRotatedFont.lfWidth = 0;	// ����� �� ���������
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

	// ������ ���������������� �� ���������
	// ����� ����� ������ ����������� CDrawable �� ���������...
}
	
CLink::~CLink()
{
	// ������ �� �������, �.�. ��. ���. ����	
}

CLink::CLink(CBookPage* pPageFrom, CBookPage* pPageTo, const CString& strLinkCaption, int iLinkNumber)
{
	m_pPageFrom = pPageFrom;
	m_pPageTo = pPageTo;	
	m_iLinkNumber = iLinkNumber;
	m_strLinkConstraints = _TEXT("");

	m_strLinkCaption = strLinkCaption;
	m_ptBeginOld.x = m_ptBeginOld.y = 0;	// ������ ���� (������)
	m_ptEndOld.x = m_ptEndOld.y = 0;		// ����� ���� (������)
	m_iRadiusFromOld = 0;	// "������" ��������� �������� (������)
	m_iRadiusToOld = 0;	// "������" �������� �������� (������)	
	m_dwParamsOld = PARAM_NONE;	// ��������� ���������� ����������
	m_bFirstTime = TRUE;	// ������ � ������ ��� - ��! � ���������� ������ ������� ����� �����������...

	m_lfRotatedFont.lfHeight = 0;	// ����� �� ���������
	m_lfRotatedFont.lfWidth = 0;	// ����� �� ���������
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

	// ������ ���������������� �� ���������
	// ����� ����� ������ ����������� CDrawable �� ���������...	
}

CLink::CLink(const CLink& C) : CDrawable(C)
{
	// ������ ���, ��� ���������! ��� ��������� ����� �� ���������, ��� ������ ������������!
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
	CDrawable::operator = (C);	// �������� �� ����� � ��� CDrawable!
	// ������ ���, ��� ���������! ��� ��������� ����� �� ���������, ��� ������ ������������!
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
	// ���������� ���� �� ���, ��� �������� ����, ������, ��� ����������, ����� 
	// �� �������� � �������� ����� ���� �� �������� ������ ���� ������, � �����
	// ����� ����� �������� ������� ������, ������� ��, ��������, Caption
	// m_iLinkNumber ����� ����� �� ���������!
}


BOOL CLink::IsBothPagesIntersectRect(const RECT& rct)
{
	return (m_pPageFrom->IsIntersect(rct) && m_pPageTo->IsIntersect(rct));	
	// ��� � ������ ���!
}

BOOL CLink::IsIntersect(const RECT& rct)
{
	POINT ptBegin;	// ������ ����
	POINT ptEnd;	// ����� ����	

	// ����� ��������� ��������
	ptBegin.y = m_pPageFrom->Get_iTop() + m_pPageFrom->Get_iHeight()/2;
	ptBegin.x = m_pPageFrom->Get_iLeft() + m_pPageFrom->Get_iWidth()/2;
	// ����� �������� ��������
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
	
	// ������� � ������ ������� (������ ����)...
	// (������� ���� ��� ��������� (��� ������� ���� ����������) � ��� ������ :-)
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
		
		// ������ � ����� �������...	
		DrawLinkMainProc(pDC, pOldBrush, dwParams, FALSE);	

		pDC->SetBkColor(crOldColor);
		pDC->SetTextColor(crOldTxtColor);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush); 

		// �������������� � �����...
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
	����� ���������� ����������� ������ ����� ���������, ���� ����� ������ �� 
	������������� ��������� (�������)
	*/

	POINT ptBegin;	// ������ ����
	POINT ptEnd;	// ����� ����	
	POINT ptEnd_Arrow1;	// ����� ��� ��������� ������� �� ����� ����
	POINT ptEnd_Arrow2;	// ����� ��� ��������� ������� �� ����� ����
	POINT ptEndArrowOnMainLine;
	POINT_DOUBLE lineVectorTo;	// ������ ������ ptEnd �� ������ �������� �������� (���, ����� ptEnd ��������� �� �������)
	POINT_DOUBLE lineVectorFrom;	// ������ ������ ptBegin �� ������ ��������� �������� (���, ����� ptBegin ��������� �� �������)
	POINT_DOUBLE lineVector_1;	// ������ ���� (��������� �����)
	POINT_DOUBLE Ort_To_lineVector_1;	// ������������� � lineVector (����� ��� ��������� ������� �� ����� ����)
	int iRadiusFrom;	// "������" ��������� �������� (���������� �� ������ �������������� �� ����� ����������� ���� � ��������)
	int iRadiusTo;	// "������" �������� �������� (���������� �� ������ �������������� �� ����� ����������� ���� � ��������)			
	double lineVectorNorm;	// ����� ���� � ���������� �����		

	if (fCountInOldCoords&&!m_bFirstTime)
	{
		ptBegin = m_ptBeginOld;
		ptEnd = m_ptEndOld;
		iRadiusFrom = m_iRadiusFromOld;
		iRadiusTo = m_iRadiusToOld;
	}
	else
	{
		// ����� ��������� ��������
		ptBegin.y = m_ptBeginOld.y = m_pPageFrom->Get_iTop() + m_pPageFrom->Get_iHeight()/2;
		ptBegin.x = m_ptBeginOld.x = m_pPageFrom->Get_iLeft() + m_pPageFrom->Get_iWidth()/2;
		// ����� �������� ��������
		ptEnd.y = m_ptEndOld.y = m_pPageTo->Get_iTop() + m_pPageTo->Get_iHeight()/2;
		ptEnd.x = m_ptEndOld.x = m_pPageTo->Get_iLeft() + m_pPageTo->Get_iWidth()/2;
		iRadiusFrom = m_iRadiusFromOld = (int) m_pPageFrom->CountPageRadius(m_pPageTo);
		iRadiusTo = m_iRadiusToOld = (int) m_pPageTo->CountPageRadius(m_pPageFrom);		
		m_bFirstTime = FALSE;	// ������� �� ���������� ����� ������...
	}

	// ���� ������, ������������ ����� ����
	lineVectorTo.x = ptEnd.x - ptBegin.x;
	lineVectorTo.y = ptEnd.y - ptBegin.y;
	// ���� ��� ����� (����� ����)
	lineVectorNorm = sqrt(lineVectorTo.x*lineVectorTo.x + lineVectorTo.y*lineVectorTo.y);

	// ���� ����� ���� <= ����� �������� ��������� � �������� �������� => �������� ������������, � ������ ���� ����� ���� �� ������ ������...
	if (lineVectorNorm > (iRadiusFrom + iRadiusTo))
	{
		// ����� �������� ����!

		// ����������
		lineVectorTo.x /= lineVectorNorm;
		lineVectorTo.y /= lineVectorNorm;
		lineVector_1 = lineVectorTo;	// �������� ������ ��������� ����� (lineVectorTo � ���������� ���������, � ������ ��������� ����� ��� �����������)

		// ������ ������ ����� �������� ��������������� �������� ��������� � �������� �������
		lineVectorFrom.x = lineVector_1.x * iRadiusFrom;
		lineVectorFrom.y = lineVector_1.y * iRadiusFrom;
		lineVectorTo.x *= iRadiusTo;
		lineVectorTo.y *= iRadiusTo;	

		// �������� ������ ���� 
		ptBegin.x += (int) lineVectorFrom.x;
		ptBegin.y += (int) lineVectorFrom.y;

		// �������� ����� ���� 
		ptEnd.x -= (int) lineVectorTo.x;	
		ptEnd.y -= (int) lineVectorTo.y;				

		// ������ ����
		pDC->MoveTo(ptBegin);
		pDC->LineTo(ptEnd);

		// ������������� � ������� (a, b) - ��� (-b, a)
		Ort_To_lineVector_1.x = -lineVector_1.y;
		Ort_To_lineVector_1.y = lineVector_1.x;		

		// �������� ���������� ����� ������� (�������� � ��������� ���� � ���������, ����� ������)
		ptEnd_Arrow1.x = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.x - Ort_To_lineVector_1.x*W_ARROW_HEIGHT_COEF) + ptEnd.x);
		ptEnd_Arrow1.y = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.y - Ort_To_lineVector_1.y*W_ARROW_HEIGHT_COEF) + ptEnd.y);
		ptEnd_Arrow2.x = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.x + Ort_To_lineVector_1.x*W_ARROW_HEIGHT_COEF) + ptEnd.x);
		ptEnd_Arrow2.y = (int) (W_ARROW_WIDTH_COEF*(-lineVector_1.y + Ort_To_lineVector_1.y*W_ARROW_HEIGHT_COEF) + ptEnd.y);

		ptEndArrowOnMainLine.x = (ptEnd_Arrow1.x + ptEnd_Arrow2.x)/2;
		ptEndArrowOnMainLine.y = (ptEnd_Arrow1.y + ptEnd_Arrow2.y)/2;

		// ������ ������� �� ����� ����
		pDC->LineTo(ptEnd_Arrow1);
		pDC->MoveTo(ptEnd);
		pDC->LineTo(ptEnd_Arrow2);		

		if (pBrushBackground)	// ���� ���� �������� ����� NULL, �� ����� ���������� �� �����!
		{
			if ((dwParams & PARAM_DRAW_FOCUS) == PARAM_DRAW_FOCUS) // ���� ���� ��������
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

		// ��� �������� �������� ��� ����� ������ ��� ����, ����� ��������
		// ���� �� ����� ��� ������, ������ � ������ ������������ � ����� � ��� �� 
		// ���������
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
	POINT ptBegin;	// ������ ����
	POINT ptEnd;	// ����� ����	
	double A;	// ����������� ������
	double B;	// ����������� ������
	double C;	// ����������� ������
	double d;	// ���������� �� ����� �� ����	
	long int lixDiff;
	long int liyDiff;
	
	// ����� ��������� ��������
	ptBegin.y = m_pPageFrom->Get_iTop() + m_pPageFrom->Get_iHeight()/2;
	ptBegin.x = m_pPageFrom->Get_iLeft() + m_pPageFrom->Get_iWidth()/2;
	// ����� �������� ��������
	ptEnd.y = m_pPageTo->Get_iTop() + m_pPageTo->Get_iHeight()/2;
	ptEnd.x = m_pPageTo->Get_iLeft() + m_pPageTo->Get_iWidth()/2;

	// ������� ������ ���, ����� ����������� ����� � �������� ���� (���� ����� 
	// ����� ��� ����� (�.�. ��� ����, ���������� ���� �� �����), ��� ���
	// ���� ����� ����������� ���������, ��� ���)
	ptBegin.y = (ptBegin.y + ptEnd.y)/2;
	ptBegin.x = (ptBegin.x + ptEnd.x)/2;

	// ������ ��������������� ����� (�� ��������������, ���������� �������� �������� ����)
	// ������ ���������, ������ �� ����� � �������������, ������, �.�.
	// ������������� ����� ���� ����������� � �����, � ����� �� �� ������ ������
	// ��������. ������� ��������� ������ �� ������� ������� ��������������,
	// ������� ��������� ����� �������� �������� ������
	// ������, ����� ���� ��� ������ ���������� � �����, �� ��� �� �������������
	lixDiff = ptEnd.x - ptBegin.x;
	if (lixDiff < 0) lixDiff = -lixDiff;
	liyDiff = ptEnd.y - ptBegin.y;
	if (liyDiff < 0) liyDiff = -liyDiff;
	if (lixDiff > liyDiff)	// ��������� �� x
	{
		if (((point.x > ptBegin.x)&&(point.x > ptEnd.x)) ||	// ������ => �� ������
			((point.x < ptBegin.x)&&(point.x < ptEnd.x))) 	// ����� => �� ������
			return FALSE;
	}
	else	// ��������� �� y
	{
		if (((point.y > ptBegin.y)&&(point.y > ptEnd.y)) ||	// ���� => �� ������
			((point.y < ptBegin.y)&&(point.y < ptEnd.y))) // ���� => �� ������
			return FALSE;
	}		

	// ���� �� �� ��� �� ������...
	/*
	������� ������ �� ����. ���������:
	�����������, F(x, y) = A*x + B*y + C - ������
	M(x1, y1) - ����� �� ���������.
	����� ���������� �� M �� ������ ������ ����������� �� �������:

         abs(F(x1, y1))
    d = -----------------
	     sqrt(A*A + B*B)

    ���:
    abs(x) - ������ x,
	sqrt(x) - ���������� ������ �� x
	*/

	// ���� ������, ������������ ����� ���� � �������� A � B
	A = -(ptEnd.y - ptBegin.y);
	B = ptEnd.x - ptBegin.x;		

	// � �������, ��������, �� ���� �������, ��� ���������� ����� 
	// ptBegin ������ �������� F, ��� ��� ����� �� ������	
	C = -A*ptBegin.x - B*ptBegin.y;	// C = -A*x - B*y

	d = fabs(A*point.x + B*point.y + C)/sqrt(A*A + B*B);	// ��. ������� ����

    if (d <= ARC_SELECTION_WIDTH) 
		return TRUE;	// ������!
	else 
		return FALSE;	// �� ������!
}

void CLink::Serialize(CArchive& ar)
{
	CDrawable::Serialize(ar);
	if (ar.IsStoring())
	{
		// storing code 		
		// m_pPageFrom & m_pPageTo �� ���������, �������� �������������� ������ �� ��������	
		// ����� �� �������� ����������� ����������/�������������� ������ �����������
		ar << m_iLinkNumber;
		ar << m_strLinkCaption << m_strLinkConstraints;		
	}
	else
	{
		// loading code		
		m_pPageFrom = m_pPageTo = NULL;	// �������� �������������� ������ �� ��������
		// ����� �� �������� ����������� ����������/�������������� ������ �����������

		ar >> m_iLinkNumber;
		ar >> m_strLinkCaption >> m_strLinkConstraints;		

		m_ptBeginOld.x = m_ptBeginOld.y = 0;	// ������ ���� (������)
		m_ptEndOld.x = m_ptEndOld.y = 0;		// ����� ���� (������)
		m_iRadiusFromOld = 0;	// "������" ��������� �������� (������)
		m_iRadiusToOld = 0;	// "������" �������� �������� (������)	
		m_dwParamsOld = PARAM_NONE;	// ��������� ���������� ����������
		m_bFirstTime = TRUE;	// ������ � ������ ��� - ��! � ���������� ������ ������� ����� �����������...
	}
}
