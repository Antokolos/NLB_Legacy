#include "StdAfx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "CTriggeredString.h"

IMPLEMENT_SERIAL(CTriggeredString, CObject, VERSIONABLE_SCHEMA | 1)

CTriggeredString::CTriggeredString()
{
	m_strTSName.Empty();
	m_pSVDefaultValue = NULL;
}

CTriggeredString::~CTriggeredString()
{
	EmptyTSList();
}

CTriggeredString::CTriggeredString(const CTriggeredString& C)
{
	CNLB_BldrApp* pApp = (CNLB_BldrApp*) AfxGetApp();
	m_strTSName = C.m_strTSName;
	m_pSVDefaultValue = C.m_pSVDefaultValue;
	if (!InitTSByList(C.m_lstConditionPairs)) pApp->HandleError(IERR_OUT_OF_MEMORY);
}

CTriggeredString& CTriggeredString::operator = (const CTriggeredString& C)
{
	if (this == &C) return *this;
	CNLB_BldrApp* pApp = (CNLB_BldrApp*) AfxGetApp();
	m_strTSName = C.m_strTSName;
	m_pSVDefaultValue = C.m_pSVDefaultValue;
	if (!InitTSByList(C.m_lstConditionPairs)) pApp->HandleError(IERR_OUT_OF_MEMORY);
	return *this;
}

BOOL CTriggeredString::InitTSByList(const CList<S_TS_ConditionPair*, S_TS_ConditionPair*&>& lst)
{
	POSITION pos;
	int iSCPCount;
	S_TS_ConditionPair* pscpNew;
	S_TS_ConditionPair* pscpCur;

	EmptyTSList();	// —ейчас мы заполним его новыми элементами
	pos = lst.GetHeadPosition();
	iSCPCount = lst.GetCount();
	for (int i = 0; i < iSCPCount; i++)
	{
		pscpNew = new S_TS_ConditionPair;
		if (!pscpNew) return FALSE;
		pscpCur = lst.GetNext(pos);	// Gets the list element identified by rPosition, then sets rPosition to the POSITION value of the next entry in the list
		if (pscpCur)
		{
			*pscpNew = *pscpCur;
			m_lstConditionPairs.AddTail(pscpNew);
		}
		else
			return FALSE;		
	}

	return TRUE;
}

S_TS_ConditionPair* CTriggeredString::AddConditionPair(const S_TS_ConditionPair& scp)
{
	S_TS_ConditionPair* pscpNew;

	pscpNew = new S_TS_ConditionPair;
	if (!pscpNew) return NULL;
	*pscpNew = scp;
	m_lstConditionPairs.AddTail(pscpNew);
	return pscpNew;
}

BOOL CTriggeredString::RemoveConditionPair(S_TS_ConditionPair* pscp)
{
	POSITION pos;
	pos = m_lstConditionPairs.Find(pscp, NULL);
	if (!pos) return FALSE;
	if (pscp) delete pscp;
	m_lstConditionPairs.RemoveAt(pos);
	return TRUE;
}

VOID CTriggeredString::EmptyTSList()
{
	POSITION pos;	
	POSITION pos_remov;	

	pos = m_lstConditionPairs.GetHeadPosition();
	while (!m_lstConditionPairs.IsEmpty())
	{			
		pos_remov = pos;
		m_lstConditionPairs.GetNext(pos);
		delete (m_lstConditionPairs.GetAt(pos_remov));
		m_lstConditionPairs.RemoveAt(pos_remov);
	}
}

void CTriggeredString::Serialize(CArchive& ar)
{	
	if (ar.IsStoring())
	{
		// storing code 		
		// ¬озложим сохранение/восстановление списка Condition Pairs на документ		
		// “акже на документ возлагаетс€ сохранение/восстановление m_pSVDefaultValue
		ar << m_strTSName;
	}
	else
	{
		// loading code		
		m_pSVDefaultValue = NULL;	// —охранение/восстановление m_pSVDefaultValue возлагаетс€ на документ
		// “акже на документ возлагаетс€ сохранение/восстановление списка Condition Pairs

		ar >> m_strTSName;
	}
}