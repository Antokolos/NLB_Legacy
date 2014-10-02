#ifndef _CSTRINGVAR_H
#define _CSTRINGVAR_H
#include <afxwin.h>		// MFC
#include <afxtempl.h>

class CStringVar : public CObject	// ��������� ����������
{	
	DECLARE_SERIAL(CStringVar)

protected:
	CString m_strStrVarName;	// ��� ����������
	CString m_strStrVarBody;	// ���������� ����������
	BOOL m_fSaveInTheBookmarksFile;
	
public:
	CList<CString, CString&> m_lstStrVarParameters;
	CList<CString, CString&> m_lstStrVarDependencies;

public:
	//<������� ��������>
	CStringVar();
	virtual ~CStringVar();
	CStringVar(CStringVar& C);  //����������� �����������
	CStringVar& operator = (CStringVar& C);   //������������� �������� ������������
	//</������� ��������>
	
	virtual void Serialize(CArchive& ar);
	CString& Get_strStrVarName() {return m_strStrVarName; }
	CString& Get_strStrVarBody() {return m_strStrVarBody; }	
	BOOL& Get_fSaveInTheBookmarksFile() {return m_fSaveInTheBookmarksFile; }
	static VOID ListAssign(CList<CString, CString&>& lstToAssignTo, CList<CString, CString&>& lstToAssign);
};

#endif