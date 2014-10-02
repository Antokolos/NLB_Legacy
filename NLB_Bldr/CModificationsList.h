#ifndef _CMODIFICATIONSLIST_H
#define _CMODIFICATIONSLIST_H
#include <afxtempl.h>
#include "types.h"

class CModificationsList
{
protected:
	CList<S_BookVarModification*, S_BookVarModification*&> m_lstBookVarModifications;

public:
	//<Большая четверка>
	CModificationsList();
	virtual ~CModificationsList();
	CModificationsList(const CModificationsList& C);  //Конструктор копирования
	CModificationsList& operator = (const CModificationsList& C);   //перегруженный оператор присваивания
	//</Большая четверка>

	VOID EmptyModificationsList();
	BOOL InitModificationsByList(const CList<S_BookVarModification*, S_BookVarModification*&>& lst);
	S_BookVarModification* AddBookVarModification(const S_BookVarModification& bvm);
	BOOL RemoveBookVarModification(S_BookVarModification* pbvm);
	int GetModificationsCount() {return m_lstBookVarModifications.GetCount(); }
	POSITION GetModificationsHeadPosition() {return m_lstBookVarModifications.GetHeadPosition(); }
	S_BookVarModification* GetNextModification(POSITION& pos) {return m_lstBookVarModifications.GetNext(pos); }
	S_BookVarModification* GetModificationAt(POSITION pos) {return m_lstBookVarModifications.GetAt(pos); }
};

#endif