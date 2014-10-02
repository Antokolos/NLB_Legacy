#ifndef _TYPES_H
#define _TYPES_H
#include "CBookVar.h"
#include "CStringVar.h"

enum OptAdd	// Выбранная на toolbar'е опция
{
	OptAdd_None,	// Ничего не выбрано
	OptAdd_BookPage,	// Выбрано: добавить страницу
	OptAdd_Link			// Выбрано: добавить ссылку
};

enum SelItemType	// Тип выбранного в данный момент элемента книги
{
	SelItem_None,	// Ничего не выбрано
	SelItem_BookPage,	// Выбрана страница книги
	SelItem_Link		// Выбрана ссылка
};

struct POINT_DOUBLE
{
	double x;
	double y;
};

enum E_CompoundLVItemType {CLI_Page, CLI_Link, CLI_BookVar, CLI_ProbVar};

struct S_CompoundLVItemData
{
	E_CompoundLVItemType cliType;	// Тип объекта - ссылка или переменная
	LPVOID lpCLIObject;				// Указатель на объект
};

struct S_BookVarModification
{
	CBookVar* pModifiedBookVar;
	CBookVar* pModificationBody;
};

struct S_TS_ConditionPair
{
	CBookVar* pBVCondition;	// Условие, при котором...
	CStringVar* pStrVar;	// ...данная TriggeredString совпадает с содержимым pStrVar
};

#endif