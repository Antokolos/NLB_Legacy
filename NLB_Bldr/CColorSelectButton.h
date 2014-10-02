#ifndef _CCOLORSELECTBUTTON_H
#define _CCOLORSELECTBUTTON_H
#include <afxwin.h>	// MFC

class CColorSelectButton : public CButton
{
protected:
	CBrush m_BrushBtnBckg;	// Кисть для фона кнопки
public:
	//<Большая четверка>
	CColorSelectButton();
	virtual ~CColorSelectButton();
	CColorSelectButton(const CColorSelectButton& C);  //Конструктор копирования
	CColorSelectButton& operator = (const CColorSelectButton& C);   //перегруженный оператор присваивания
	//</Большая четверка>

	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );		
	BOOL GetLogBrush(LOGBRUSH* plBrush);	
	BOOL SetLogBrush(LOGBRUSH* plBrush);	
};

#endif