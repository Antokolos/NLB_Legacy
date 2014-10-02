#include "stdafx.h"
#include "CColorSelectButton.h"
#include "consts.h"

CColorSelectButton::CColorSelectButton()
{
	m_BrushBtnBckg.CreateSolidBrush(cr_BRUSH_DEFAULT);		
}

CColorSelectButton::~CColorSelectButton()
{
	m_BrushBtnBckg.DeleteObject();
}

CColorSelectButton::CColorSelectButton(const CColorSelectButton& C)
{	
	LOGBRUSH lbrush;

	// Возьмем информацию...
	((CBrush&) (C.m_BrushBtnBckg)).GetLogBrush(&lbrush);
	// ...уничтожим старые объекты...	
	m_BrushBtnBckg.DeleteObject();
	// ...и создадим новые!!!	
	m_BrushBtnBckg.CreateBrushIndirect(&lbrush);
}

CColorSelectButton& CColorSelectButton::operator = (const CColorSelectButton& C)
{
	LOGBRUSH lbrush;

	if (this==&C) return *this;
	// Возьмем информацию...
	((CBrush&) (C.m_BrushBtnBckg)).GetLogBrush(&lbrush);
	// ...уничтожим старые объекты...	
	m_BrushBtnBckg.DeleteObject();
	// ...и создадим новые!!!	
	m_BrushBtnBckg.CreateBrushIndirect(&lbrush);
	return *this;
}

BOOL CColorSelectButton::GetLogBrush(LOGBRUSH* plBrush)
{	
	if (plBrush)
	{
		m_BrushBtnBckg.GetLogBrush(plBrush);
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CColorSelectButton::SetLogBrush(LOGBRUSH* plBrush)
{
	if (plBrush)
	{		
		m_BrushBtnBckg.DeleteObject();		
		m_BrushBtnBckg.CreateBrushIndirect(plBrush);
		return TRUE;
	}
	else
		return FALSE;
}

void CColorSelectButton::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	UINT uStyle = DFCS_BUTTONPUSH;	
	LOGBRUSH lbrush;
	DWORD dwFontColor;
	RECT rct = lpDrawItemStruct->rcItem;

	// This code only works with buttons.
	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);

	// If drawing selected, add the pushed style to DrawFrameControl.
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		uStyle |= DFCS_PUSHED;

	HBRUSH hBrushOld = (HBRUSH) ::SelectObject(lpDrawItemStruct->hDC, m_BrushBtnBckg);
	// Draw the button frame.
	::DrawFrameControl(lpDrawItemStruct->hDC, &rct, 
		DFC_BUTTON, uStyle);

	// Проверок на последующую операцию не делаю, так что задавайте НОРМАЛЬНЫЕ 
	// размеры кнопки, чтобы результирующий прямоугольник получился таким, каким надо
	// (т.е. чтобы top не оказался ниже bottom et.c.)
	Rectangle(lpDrawItemStruct->hDC,
		rct.left + I_BTN_BORDER, 
		rct.top + I_BTN_BORDER,
		rct.right - I_BTN_BORDER,
		rct.bottom - I_BTN_BORDER);
	::SelectObject(lpDrawItemStruct->hDC, hBrushOld);

	// Get the button's text.
	CString strText;
	GetWindowText(strText);

	// Draw the button text using the text color.
	COLORREF crOldBkColor = ::GetBkColor(lpDrawItemStruct->hDC);
	m_BrushBtnBckg.GetLogBrush(&lbrush);	
	::SetBkColor(lpDrawItemStruct->hDC, lbrush.lbColor);	
	dwFontColor = (DWORD) lbrush.lbColor;	

	// Инвертируем цвет и обнулим первые два бита (см. формат COLORREF)
	// Это надо для того, чтобы надпись на кнопке контрастировала с ее фоном
	dwFontColor = ~dwFontColor & 0x00FFFFFF;	

	COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, dwFontColor);
	::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
		&rct, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	::SetTextColor(lpDrawItemStruct->hDC, crOldColor);
	::SetBkColor(lpDrawItemStruct->hDC, crOldBkColor);
}