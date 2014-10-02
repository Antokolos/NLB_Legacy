#ifndef _CPROBVAR_H
#define _CPROBVAR_H
#include <afxwin.h>		// MFC

class CProbVar : public CObject	// ������������� ���������� (� ������������ � ������������ ������)
{	
	DECLARE_SERIAL(CProbVar)

protected:
	CString m_strProbVarName;	// ��� ����������
	int		m_iDiceK;	// ����� �������
	int		m_iDiceY;	// ������ ������ ������ (����� ������)

public:
	//<������� ��������>
	CProbVar();
	virtual ~CProbVar();
	CProbVar(const CProbVar& C);  //����������� �����������
	CProbVar& operator = (const CProbVar& C);   //������������� �������� ������������
	//</������� ��������>
	
	virtual void Serialize(CArchive& ar);
	CString& Get_strProbVarName() {return m_strProbVarName; }
	int Get_iDiceK() {return m_iDiceK; }
	BOOL Set_iDiceK(int _iDiceK);
	int Get_iDiceY() {return m_iDiceY; }
	BOOL Set_iDiceY(int _iDiceY);	
};

#endif