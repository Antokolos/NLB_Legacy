#ifndef _CSTRINGVAR_H
#define _CSTRINGVAR_H
#include <afxwin.h>		// MFC
#include <afxtempl.h>

class CStringVar : public CObject	// Строковая переменная
{	
	DECLARE_SERIAL(CStringVar)

protected:
	CString m_strStrVarName;	// Имя переменной
	CString m_strStrVarBody;	// Содержимое переменной
	BOOL m_fSaveInTheBookmarksFile;
	
public:
	CList<CString, CString&> m_lstStrVarParameters;
	CList<CString, CString&> m_lstStrVarDependencies;

public:
	//<Большая четверка>
	CStringVar();
	virtual ~CStringVar();
	CStringVar(CStringVar& C);  //Конструктор копирования
	CStringVar& operator = (CStringVar& C);   //перегруженный оператор присваивания
	//</Большая четверка>
	
	virtual void Serialize(CArchive& ar);
	CString& Get_strStrVarName() {return m_strStrVarName; }
	CString& Get_strStrVarBody() {return m_strStrVarBody; }	
	BOOL& Get_fSaveInTheBookmarksFile() {return m_fSaveInTheBookmarksFile; }
	static VOID ListAssign(CList<CString, CString&>& lstToAssignTo, CList<CString, CString&>& lstToAssign);
};

#endif