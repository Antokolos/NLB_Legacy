#ifndef _CTRIGGEREDSTRING_H
#define _CTRIGGEREDSTRING_H
#include <afxtempl.h>
#include "types.h"

class CTriggeredString : public CObject	// Строка, меняющая свое значение в зависимости от параметра (Triggered String)
{
	DECLARE_SERIAL(CTriggeredString)

protected:
	CString m_strTSName;
	CStringVar* m_pSVDefaultValue;
	CList<S_TS_ConditionPair*, S_TS_ConditionPair*&> m_lstConditionPairs;

public:
	// В помошь сериализации...
	CList<POSITION, POSITION> m_lstConditionBookVarChanged;	// Не используйте этот список ни для чего другого, кроме сериализации
	CList<POSITION, POSITION> m_lstValueStrVarChanged;	// Не используйте этот список ни для чего другого, кроме сериализации									 

public:
	//<Большая четверка>
	CTriggeredString();
	virtual ~CTriggeredString();
	CTriggeredString(const CTriggeredString& C);  //Конструктор копирования
	CTriggeredString& operator = (const CTriggeredString& C);   //перегруженный оператор присваивания
	//</Большая четверка>

	virtual void Serialize(CArchive& ar);
	CString& Get_strTSName() {return m_strTSName; }
	VOID EmptyTSList();
	BOOL InitTSByList(const CList<S_TS_ConditionPair*, S_TS_ConditionPair*&>& lst);
	S_TS_ConditionPair* AddConditionPair(const S_TS_ConditionPair& scp);
	BOOL RemoveConditionPair(S_TS_ConditionPair* pscp);
	int GetConditionPairsCount() {return m_lstConditionPairs.GetCount(); }
	POSITION GetConditionPairsHeadPosition() {return m_lstConditionPairs.GetHeadPosition(); }
	S_TS_ConditionPair* GetNextConditionPair(POSITION& pos) {return m_lstConditionPairs.GetNext(pos); }
	S_TS_ConditionPair* GetConditionPairAt(POSITION pos) {return m_lstConditionPairs.GetAt(pos); }
	CStringVar* GetDefaultValue() {return m_pSVDefaultValue; }
	VOID SetDefaultValue(CStringVar* psv) {m_pSVDefaultValue = psv; }
};

#endif