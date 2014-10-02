#ifndef _CBOOKVAR_H
#define _CBOOKVAR_H
#include <afxwin.h>		// MFC

class CBookVar : public CObject	// ���������� - ��������� ������ ������� �� ���������� ���������� � ������ ����������
{	
	DECLARE_SERIAL(CBookVar)

protected:
	CString m_strBookVarName;	// ��� ����������
	CString m_strBookVarBody;	// ���������� ����������
	BOOL m_fIsVarGlobal;		// ���������� ����������?

public:
	//<������� ��������>
	CBookVar();
	virtual ~CBookVar();
	CBookVar(const CBookVar& C);  //����������� �����������
	CBookVar& operator = (const CBookVar& C);   //������������� �������� ������������
	//</������� ��������>
	
	virtual void Serialize(CArchive& ar);
	CString& Get_strBookVarName() {return m_strBookVarName; }
	CString& Get_strBookVarBody() {return m_strBookVarBody; }
	BOOL& Get_fIsVarGlobal() {return m_fIsVarGlobal; }
};

#endif