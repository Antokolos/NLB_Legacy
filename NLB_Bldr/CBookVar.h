#ifndef _CBOOKVAR_H
#define _CBOOKVAR_H
#include <afxwin.h>		// MFC

class CBookVar : public CObject	// Переменная - некоторая булева функция от страничных переменных и просто переменных
{	
	DECLARE_SERIAL(CBookVar)

protected:
	CString m_strBookVarName;	// Имя переменной
	CString m_strBookVarBody;	// Содержимое переменной
	BOOL m_fIsVarGlobal;		// Переменная глобальная?

public:
	//<Большая четверка>
	CBookVar();
	virtual ~CBookVar();
	CBookVar(const CBookVar& C);  //Конструктор копирования
	CBookVar& operator = (const CBookVar& C);   //перегруженный оператор присваивания
	//</Большая четверка>
	
	virtual void Serialize(CArchive& ar);
	CString& Get_strBookVarName() {return m_strBookVarName; }
	CString& Get_strBookVarBody() {return m_strBookVarBody; }
	BOOL& Get_fIsVarGlobal() {return m_fIsVarGlobal; }
};

#endif