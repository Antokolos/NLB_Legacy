#ifndef _CLINK_H
#define _CLINK_H
#include "stdafx.h"
#include "CDrawable.h"
#include "CBookPage.h"
#include "CModificationsList.h"

class CLink : public CDrawable
{
protected:
	CBookPage* m_pPageFrom;
	CBookPage* m_pPageTo;
	int	m_iLinkNumber;	// Порядковый номер ссылки (при предоставлении выбора, т.е. ссылки с меньшим порядковым номером находятся в списке раньше)
	CString m_strLinkCaption;
	CString m_strLinkConstraints;	
	CModificationsList m_BVModList;
	
	// Не сохраняемые в файле вспомогательные переменные
	LOGFONT m_lfRotatedFont;	// Шрифт для изображения повернутой на некоторый угол строки, которая будет изображена вдоль стрелки ссылки

	// Старая позиция (В ФАЙЛЕ НЕ СОХРАНЯЕМ, ПРИ ПРОВЕРКЕ НА РАВЕНСТВО НЕ УЧИТЫВАЕМ!)
	POINT m_ptBeginOld;	// Начало дуги (старое)
	POINT m_ptEndOld;		// Конец дуги (старое)
	int m_iRadiusFromOld;	// "Радиус" начальной страницы (старый)
	int m_iRadiusToOld;	// "Радиус" конечной страницы (старый)	
	DWORD m_dwParamsOld;	// Параметры предыдущей прорисовки
	BOOL m_bFirstTime;	// В первый раз?

public:
	// В помошь сериализации...
	CList<POSITION, POSITION> m_lstModifiedBookVarChanged;	// Не используйте этот список ни для чего другого, кроме сериализации
	CList<POSITION, POSITION> m_lstModificationBodyChanged;	// Не используйте этот список ни для чего другого, кроме сериализации									 

public:
	//<Большая четверка>
	CLink();
	virtual ~CLink();
	CLink(const CLink& C);  //Конструктор копирования
	CLink& operator = (const CLink& C);   //перегруженный оператор присваивания
	//</Большая четверка>

	CLink(CBookPage* pPageFrom, CBookPage* pPageTo, const CString& strLinkCaption, int iLinkNumber);
	BOOL operator == ( const CLink& C) const;
	virtual VOID Draw(CDC* pDC, DWORD dwParams = 0);
	VOID DrawLinkMainProc(CDC* pDC, 
		CBrush* pBrushBackground, 
		DWORD dwParams, 
		BOOL fCountInOldCoords);
	CModificationsList& Get_BVModList() {return m_BVModList; }
	CBookPage*& Get_pPageFrom() {return m_pPageFrom; }
	CBookPage*& Get_pPageTo() {return m_pPageTo; }
	CString& Get_strLinkCaption() {return m_strLinkCaption; }
	CString& Get_strLinkConstraints() {return m_strLinkConstraints; }
	int& Get_iLinkNumber() {return m_iLinkNumber; }
	virtual BOOL IsPointInside(CPoint& point);	
	virtual BOOL IsIntersect(const RECT& rct);	// Пересекается ли изображение с данным прямоугольником?
	virtual BOOL IsBothPagesIntersectRect(const RECT& rct);	// Пересекаются ли начальная и коенчная страница ссылки с данным прямоугольником?
	virtual void Serialize(CArchive& ar);
};

// Структура для списка порядка ссылок в диалоге редактирования страницы
struct S_PLink_LCounter_And_LStrings
{
	CLink* pLink;		// Это единственный указатель в этой структуре, который не надо удалять, т.к. он только используется. Строки же надо будет убить.
	int iOldLinkNumber;	// Если в диалоге нажата "отмена", то у всех неудаленных ссылок надо вновь изменить значение LinkNumber на это
	TCHAR* lpszLinkCaptionForLV;		// Буфер для обработки TEXT_CALLBACK
	TCHAR* lpszLinkConstraintsForLV;	// Буфер для обработки TEXT_CALLBACK
};

#endif