#ifndef _CCOLORSELECTBUTTON_H
#define _CCOLORSELECTBUTTON_H
#include <afxwin.h>	// MFC

class CColorSelectButton : public CButton
{
protected:
	CBrush m_BrushBtnBckg;	// ����� ��� ���� ������
public:
	//<������� ��������>
	CColorSelectButton();
	virtual ~CColorSelectButton();
	CColorSelectButton(const CColorSelectButton& C);  //����������� �����������
	CColorSelectButton& operator = (const CColorSelectButton& C);   //������������� �������� ������������
	//</������� ��������>

	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );		
	BOOL GetLogBrush(LOGBRUSH* plBrush);	
	BOOL SetLogBrush(LOGBRUSH* plBrush);	
};

#endif