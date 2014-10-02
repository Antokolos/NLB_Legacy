#ifndef _CBOOKPAGE_H
#define _CBOOKPAGE_H
#include "stdafx.h"
#include <afxwin.h>
#include "CDrawable.h"
#include "consts.h"

class CBookPage : public CDrawable
{
protected:
	// Атрибуты класса, как элемента книги
	CString m_strPageText;
	CString m_strPageCaption;	
	CString m_strPageVar;	// Имя булевской переменной, которая будет обозначать в книге то, что страница уже была посещена.
	BOOL m_fUsePageCaption;

	// Атрибуты класса, как графического изображения
	int m_iTop;
	int m_iLeft;
	int m_iWidth;		
	int m_iHeight;		

	// Старая позиция (В ФАЙЛЕ НЕ СОХРАНЯЕМ, ПРИ ПРОВЕРКЕ НА РАВЕНСТВО НЕ УЧИТЫВАЕМ!)
	int m_iTopOld;
	int m_iLeftOld;
	int m_iWidthOld;		
	int m_iHeightOld;		
	BOOL m_bWasFocusedLastTime;	// Был ли нарисован прямоугольник фокуса?

public:
	//<Большая четверка>
	CBookPage();
	virtual ~CBookPage(); 
	CBookPage(const CBookPage& C);  //Конструктор копирования
	CBookPage& operator = (const CBookPage& C);   //перегруженный оператор присваивания
	//</Большая четверка>

	BOOL operator == ( const CBookPage& C) const;
	CBookPage(CString& strPageCaption, int iTop, int iLeft);	
	CString& Get_strPageText() {return m_strPageText; }
	CString& Get_strPageCaption() {return m_strPageCaption; }
	CString& Get_strPageVar() {return m_strPageVar; }	
	int& Get_iTop() {return m_iTop; }
	int& Get_iLeft() {return m_iLeft; }
	int& Get_iWidth() {return m_iWidth; }	
	int& Get_iHeight() {return m_iHeight; }		
	BOOL& Get_UsePageCaption() {return m_fUsePageCaption; }
	virtual VOID Draw(CDC* pDC, DWORD dwParams = 0);
	virtual BOOL IsPointInside(CPoint& point);
	double CountPageRadius(CBookPage* pPageRelativeTo);	// "Радиус" страницы относительно другой страницы (длина отрезка прямой от центра до границы, соединяющей центры этих двух страниц)
	virtual void Serialize(CArchive& ar);
	virtual BOOL IsIntersect(const RECT& rct);	// Пересекается ли изображение с данным прямоугольником
};

#endif