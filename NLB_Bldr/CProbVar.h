#ifndef _CPROBVAR_H
#define _CPROBVAR_H
#include <afxwin.h>		// MFC

class CProbVar : public CObject	// Вероятностная переменная (в соответствии с классической схемой)
{	
	DECLARE_SERIAL(CProbVar)

protected:
	CString m_strProbVarName;	// Имя переменной
	int		m_iDiceK;	// Число кубиков
	int		m_iDiceY;	// Размер одного кубика (число граней)

public:
	//<Большая четверка>
	CProbVar();
	virtual ~CProbVar();
	CProbVar(const CProbVar& C);  //Конструктор копирования
	CProbVar& operator = (const CProbVar& C);   //перегруженный оператор присваивания
	//</Большая четверка>
	
	virtual void Serialize(CArchive& ar);
	CString& Get_strProbVarName() {return m_strProbVarName; }
	int Get_iDiceK() {return m_iDiceK; }
	BOOL Set_iDiceK(int _iDiceK);
	int Get_iDiceY() {return m_iDiceY; }
	BOOL Set_iDiceY(int _iDiceY);	
};

#endif