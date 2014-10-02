#ifndef _CDRAWABLE_H
#define _CDRAWABLE_H
#include <afxwin.h>		// MFC

class CDrawable : public CObject
{	
	DECLARE_SERIAL(CDrawable)

protected:
	LOGPEN m_theLogPen;	// LOGPEN дл€ пера, которым будем рисовать	
	LOGBRUSH m_theLogBrush;	// LOGBRUSH дл€ кисти, которой будем закрашивать
	// ѕару слов по поводу этого.
	// »значально здесь были CPen и CBrush и все было отлично. ќднако, когда 
	// число объектов этого типа стало достаточно велико, по€вилс€ глюк.
	// ¬идимо, Windows GDI не поддерживает большое число одновременно открытых дескрипторов
	// кистей и перьев. ѕоэтому в новосозданных объектах было какое-то дерьмо.
	// ѕосле вы€влени€ бага было большое желание создать массив всех используемых
	// книгой CPen и CBrush и те страницы, которые используют одни и те же кисть и перо,
	// переназначить на один и тот же элемент этого массива. ќднако, это долго, муторно,
	// содержит богатую почву дл€ дальнейших глюков и т.д. “аким образом, решено хранить
	// здесь только LOGPEN и LOGBRUSH, а сами кисть и перо создавать заново при каждой
	// перерисовке и удал€ть в ее конце.  онечно, это несколько снизит скорость,
	// особенно при наличии большого числа элементов книги на экране и частой перерисовке.
	// ¬озможно, надо будет сделать это по-другому.
	
	COLORREF m_cr_Txt_Color;	
	BOOL m_fInSelectionArea;	// ƒанный элемент изображени€ выделен
	BOOL m_fNeedToEraseWhenNotVisible;		// ¬спомогательное поле, нужно дл€ правильной отрисовки

public:
	//<Ѕольша€ четверка>
	CDrawable();
	virtual ~CDrawable();
	CDrawable(const CDrawable& C);  // онструктор копировани€
	CDrawable& operator = (const CDrawable& C);   //перегруженный оператор присваивани€
	//</Ѕольша€ четверка>

	virtual VOID Draw(CDC* pDC, DWORD dwParams = 0) {return; };
	virtual BOOL IsPointInside(CPoint& point) {return TRUE; };
	virtual BOOL IsIntersect(const RECT& rct) {return TRUE; };	// ѕересекаетс€ ли изображение с данным пр€моугольником
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