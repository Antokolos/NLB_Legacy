#ifndef _TYPES_H
#define _TYPES_H
#include "CBookVar.h"
#include "CStringVar.h"

enum OptAdd	// ��������� �� toolbar'� �����
{
	OptAdd_None,	// ������ �� �������
	OptAdd_BookPage,	// �������: �������� ��������
	OptAdd_Link			// �������: �������� ������
};

enum SelItemType	// ��� ���������� � ������ ������ �������� �����
{
	SelItem_None,	// ������ �� �������
	SelItem_BookPage,	// ������� �������� �����
	SelItem_Link		// ������� ������
};

struct POINT_DOUBLE
{
	double x;
	double y;
};

enum E_CompoundLVItemType {CLI_Page, CLI_Link, CLI_BookVar, CLI_ProbVar};

struct S_CompoundLVItemData
{
	E_CompoundLVItemType cliType;	// ��� ������� - ������ ��� ����������
	LPVOID lpCLIObject;				// ��������� �� ������
};

struct S_BookVarModification
{
	CBookVar* pModifiedBookVar;
	CBookVar* pModificationBody;
};

struct S_TS_ConditionPair
{
	CBookVar* pBVCondition;	// �������, ��� �������...
	CStringVar* pStrVar;	// ...������ TriggeredString ��������� � ���������� pStrVar
};

#endif