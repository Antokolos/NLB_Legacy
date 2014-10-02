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
	int	m_iLinkNumber;	// ���������� ����� ������ (��� �������������� ������, �.�. ������ � ������� ���������� ������� ��������� � ������ ������)
	CString m_strLinkCaption;
	CString m_strLinkConstraints;	
	CModificationsList m_BVModList;
	
	// �� ����������� � ����� ��������������� ����������
	LOGFONT m_lfRotatedFont;	// ����� ��� ����������� ���������� �� ��������� ���� ������, ������� ����� ���������� ����� ������� ������

	// ������ ������� (� ����� �� ���������, ��� �������� �� ��������� �� ���������!)
	POINT m_ptBeginOld;	// ������ ���� (������)
	POINT m_ptEndOld;		// ����� ���� (������)
	int m_iRadiusFromOld;	// "������" ��������� �������� (������)
	int m_iRadiusToOld;	// "������" �������� �������� (������)	
	DWORD m_dwParamsOld;	// ��������� ���������� ����������
	BOOL m_bFirstTime;	// � ������ ���?

public:
	// � ������ ������������...
	CList<POSITION, POSITION> m_lstModifiedBookVarChanged;	// �� ����������� ���� ������ �� ��� ���� �������, ����� ������������
	CList<POSITION, POSITION> m_lstModificationBodyChanged;	// �� ����������� ���� ������ �� ��� ���� �������, ����� ������������									 

public:
	//<������� ��������>
	CLink();
	virtual ~CLink();
	CLink(const CLink& C);  //����������� �����������
	CLink& operator = (const CLink& C);   //������������� �������� ������������
	//</������� ��������>

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
	virtual BOOL IsIntersect(const RECT& rct);	// ������������ �� ����������� � ������ ���������������?
	virtual BOOL IsBothPagesIntersectRect(const RECT& rct);	// ������������ �� ��������� � �������� �������� ������ � ������ ���������������?
	virtual void Serialize(CArchive& ar);
};

// ��������� ��� ������ ������� ������ � ������� �������������� ��������
struct S_PLink_LCounter_And_LStrings
{
	CLink* pLink;		// ��� ������������ ��������� � ���� ���������, ������� �� ���� �������, �.�. �� ������ ������������. ������ �� ���� ����� �����.
	int iOldLinkNumber;	// ���� � ������� ������ "������", �� � ���� ����������� ������ ���� ����� �������� �������� LinkNumber �� ���
	TCHAR* lpszLinkCaptionForLV;		// ����� ��� ��������� TEXT_CALLBACK
	TCHAR* lpszLinkConstraintsForLV;	// ����� ��� ��������� TEXT_CALLBACK
};

#endif