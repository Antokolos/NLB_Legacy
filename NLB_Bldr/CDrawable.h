#ifndef _CDRAWABLE_H
#define _CDRAWABLE_H
#include <afxwin.h>		// MFC

class CDrawable : public CObject
{	
	DECLARE_SERIAL(CDrawable)

protected:
	LOGPEN m_theLogPen;	// LOGPEN ��� ����, ������� ����� ��������	
	LOGBRUSH m_theLogBrush;	// LOGBRUSH ��� �����, ������� ����� �����������
	// ���� ���� �� ������ �����.
	// ���������� ����� ���� CPen � CBrush � ��� ���� �������. ������, ����� 
	// ����� �������� ����� ���� ����� ���������� ������, �������� ����.
	// ������, Windows GDI �� ������������ ������� ����� ������������ �������� ������������
	// ������ � ������. ������� � ������������� �������� ���� �����-�� ������.
	// ����� ��������� ���� ���� ������� ������� ������� ������ ���� ������������
	// ������ CPen � CBrush � �� ��������, ������� ���������� ���� � �� �� ����� � ����,
	// ������������� �� ���� � ��� �� ������� ����� �������. ������, ��� �����, �������,
	// �������� ������� ����� ��� ���������� ������ � �.�. ����� �������, ������ �������
	// ����� ������ LOGPEN � LOGBRUSH, � ���� ����� � ���� ��������� ������ ��� ������
	// ����������� � ������� � �� �����. �������, ��� ��������� ������ ��������,
	// �������� ��� ������� �������� ����� ��������� ����� �� ������ � ������ �����������.
	// ��������, ���� ����� ������� ��� ��-�������.
	
	COLORREF m_cr_Txt_Color;	
	BOOL m_fInSelectionArea;	// ������ ������� ����������� �������
	BOOL m_fNeedToEraseWhenNotVisible;		// ��������������� ����, ����� ��� ���������� ���������

public:
	//<������� ��������>
	CDrawable();
	virtual ~CDrawable();
	CDrawable(const CDrawable& C);  //����������� �����������
	CDrawable& operator = (const CDrawable& C);   //������������� �������� ������������
	//</������� ��������>

	virtual VOID Draw(CDC* pDC, DWORD dwParams = 0) {return; };
	virtual BOOL IsPointInside(CPoint& point) {return TRUE; };
	virtual BOOL IsIntersect(const RECT& rct) {return TRUE; };	// ������������ �� ����������� � ������ ���������������
	virtual void Serialize(CArchive& ar);
	COLORREF& Get_cr_Txt_Color() {return m_cr_Txt_Color; }
	BOOL GetLogBrush(LOGBRUSH* plBrush);
	BOOL GetLogPen(LOGPEN* plPen);
	BOOL SetLogBrush(LOGBRUSH* plBrush);
	BOOL SetLogPen(LOGPEN* plPen);	
	VOID Select() {m_fInSelectionArea = TRUE; }
	VOID Deselect() {m_fInSelectionArea = FALSE; }
	BOOL IsSelected() {return m_fInSelectionArea; }
	BOOL& NeedToEraseWhenNotVisible() {return m_fNeedToEraseWhenNotVisible; }

protected:
	VOID DrawFocusByPoint(CDC* pDC, POINT& ptWhere);
};

#endif