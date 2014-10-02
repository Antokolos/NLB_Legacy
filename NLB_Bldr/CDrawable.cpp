#include "stdafx.h"
#include "CDrawable.h"
#include "consts.h"

IMPLEMENT_SERIAL(CDrawable, CObject, VERSIONABLE_SCHEMA | 1)

CDrawable::CDrawable()
{
	m_theLogPen.lopnStyle = PS_INSIDEFRAME;
	m_theLogPen.lopnWidth.x = I_PEN_WIDTH;
	m_theLogPen.lopnColor = cr_PEN_DEFAULT;
	m_theLogBrush.lbColor = cr_BRUSH_DEFAULT;	
	m_theLogBrush.lbStyle = BS_SOLID;
	m_theLogBrush.lbHatch = 0;	// Если стиль BS_SOLID, то это игнорируется	
	m_cr_Txt_Color = cr_TXT_DEFAULT;
	m_fInSelectionArea = FALSE;
	m_fNeedToEraseWhenNotVisible = FALSE;
}

CDrawable::~CDrawable() 
{	
}

CDrawable::CDrawable(const CDrawable& C)
{
	m_theLogPen = C.m_theLogPen;
	m_theLogBrush = C.m_theLogBrush;
	m_cr_Txt_Color = C.m_cr_Txt_Color;	
	m_fInSelectionArea = C.m_fInSelectionArea;
	m_fNeedToEraseWhenNotVisible = C.m_fNeedToEraseWhenNotVisible;
}

CDrawable& CDrawable::operator = (const CDrawable& C)
{
	if (this==&C) return *this;
	m_theLogPen = C.m_theLogPen;
	m_theLogBrush = C.m_theLogBrush;
	m_cr_Txt_Color = C.m_cr_Txt_Color;	
	m_fInSelectionArea = C.m_fInSelectionArea;
	m_fNeedToEraseWhenNotVisible = C.m_fNeedToEraseWhenNotVisible;
	return *this;
}

BOOL CDrawable::GetLogBrush(LOGBRUSH* plBrush)
{	
	if (plBrush)
	{
		(*plBrush) = m_theLogBrush;
		return TRUE;		
	}
	else
		return FALSE;
}

BOOL CDrawable::GetLogPen(LOGPEN* plPen)
{	
	if (plPen) 
	{
		(*plPen) = m_theLogPen;
		return TRUE;		
	}
	else
		return FALSE;
}

BOOL CDrawable::SetLogBrush(LOGBRUSH* plBrush)
{
	if (plBrush)
	{		
		m_theLogBrush = (*plBrush);
		return TRUE;		
	}
	else
		return FALSE;
}

BOOL CDrawable::SetLogPen(LOGPEN* plPen)
{
	if (plPen) 
	{
		m_theLogPen = (*plPen);
		return TRUE;
	}
	else
		return FALSE;
}

VOID CDrawable::DrawFocusByPoint(CDC* pDC, POINT& ptWhere)
{
	pDC->Rectangle(ptWhere.x - I_FOCUS_RECT_OFFSET, 
				   ptWhere.y - I_FOCUS_RECT_OFFSET, 
				   ptWhere.x + I_FOCUS_RECT_OFFSET, 
				   ptWhere.y + I_FOCUS_RECT_OFFSET);
}

void CDrawable::Serialize(CArchive& ar)
{
	// m_fNeedToEraseWhenNotVisible не сохраняем и не загружаем, это чисто вспомогательная фишка, которая в нужный момент сама будет меняться в PaintInMemory
	if (ar.IsStoring())
	{
		// storing code 				
		ar << m_cr_Txt_Color;
		ar << m_fInSelectionArea;
		ar << m_theLogPen.lopnColor
			<< m_theLogPen.lopnStyle 
			<< m_theLogPen.lopnWidth
			<< m_theLogBrush.lbColor
			<< m_theLogBrush.lbHatch
			<< m_theLogBrush.lbStyle;		
	}
	else
	{
		// loading code
		ar >> m_cr_Txt_Color;
		ar >> m_fInSelectionArea;
		ar >> m_theLogPen.lopnColor
			>> m_theLogPen.lopnStyle 
			>> m_theLogPen.lopnWidth
			>> m_theLogBrush.lbColor
			>> m_theLogBrush.lbHatch
			>> m_theLogBrush.lbStyle;	
	}
}