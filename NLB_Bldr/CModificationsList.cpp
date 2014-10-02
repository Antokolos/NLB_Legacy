#include "StdAfx.h"
#include "NLB_Bldr.h"
#include "consts.h"
#include "CModificationsList.h"

CModificationsList::CModificationsList()
{}

CModificationsList::~CModificationsList()
{
	EmptyModificationsList();
}

CModificationsList::CModificationsList(const CModificationsList& C)
{
	CNLB_BldrApp* pApp = (CNLB_BldrApp*) AfxGetApp();
	if (!InitModificationsByList(C.m_lstBookVarModifications)) pApp->HandleError(IERR_OUT_OF_MEMORY);
}

CModificationsList& CModificationsList::operator = (const CModificationsList& C)
{
	if (this == &C) return *this;
	CNLB_BldrApp* pApp = (CNLB_BldrApp*) AfxGetApp();
	if (!InitModificationsByList(C.m_lstBookVarModifications)) pApp->HandleError(IERR_OUT_OF_MEMORY);
	return *this;
}

BOOL CModificationsList::InitModificationsByList(const CList<S_BookVarModification*, S_BookVarModification*&>& lst)
{
	POSITION pos;
	int iModCount;
	S_BookVarModification* pbvmNew;
	S_BookVarModification* pbvmCur;

	EmptyModificationsList();	// Сейчас мы заполним его новыми элементами
	pos = lst.GetHeadPosition();
	iModCount = lst.GetCount();
	for (int i = 0; i < iModCount; i++)
	{
		pbvmNew = new S_BookVarModification;
		if (!pbvmNew) return FALSE;
		pbvmCur = lst.GetNext(pos);	// Gets the list element identified by rPosition, then sets rPosition to the POSITION value of the next entry in the list
		if (pbvmCur)
		{
			*pbvmNew = *pbvmCur;
			m_lstBookVarModifications.AddTail(pbvmNew);
		}
		else
			return FALSE;		
	}

	return TRUE;
}

S_BookVarModification* CModificationsList::AddBookVarModification(const S_BookVarModification& bvm)
{
	S_BookVarModification* pbvmNew;

	pbvmNew = new S_BookVarModification;
	if (!pbvmNew) return NULL;
	*pbvmNew = bvm;
	m_lstBookVarModifications.AddTail(pbvmNew);
	return pbvmNew;
}

BOOL CModificationsList::RemoveBookVarModification(S_BookVarModification* pbvm)
{
	POSITION pos;
	pos = m_lstBookVarModifications.Find(pbvm, NULL);
	if (!pos) return FALSE;
	if (pbvm) delete pbvm;
	m_lstBookVarModifications.RemoveAt(pos);
	return TRUE;
}

VOID CModificationsList::EmptyModificationsList()
{
	POSITION pos;	
	POSITION pos_remov;	

	pos = m_lstBookVarModifications.GetHeadPosition();
	while (!m_lstBookVarModifications.IsEmpty())
	{			
		pos_remov = pos;
		m_lstBookVarModifications.GetNext(pos);
		delete (m_lstBookVarModifications.GetAt(pos_remov));
		m_lstBookVarModifications.RemoveAt(pos_remov);
	}
}