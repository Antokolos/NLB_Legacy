#include "StdAfx.h"
#include "CStringVar.h"
#include "liststuff.h"

IMPLEMENT_SERIAL(CStringVar, CObject, VERSIONABLE_SCHEMA | 1)

CStringVar::CStringVar()
{
	m_strStrVarName = _TEXT("");
	m_strStrVarBody = _TEXT("");	
	m_fSaveInTheBookmarksFile = FALSE;
}

CStringVar::~CStringVar() 
{	
}

CStringVar::CStringVar(CStringVar& C)
{
	m_strStrVarName = C.m_strStrVarName;
	m_strStrVarBody = C.m_strStrVarBody;	
	m_fSaveInTheBookmarksFile = C.m_fSaveInTheBookmarksFile;
	ListAssign(m_lstStrVarParameters, C.m_lstStrVarParameters);
	ListAssign(m_lstStrVarDependencies, C.m_lstStrVarDependencies);
}

CStringVar& CStringVar::operator = (CStringVar& C)
{
	if (this==&C) return *this;
	m_strStrVarName = C.m_strStrVarName;
	m_strStrVarBody = C.m_strStrVarBody;	
	m_fSaveInTheBookmarksFile = C.m_fSaveInTheBookmarksFile;
	ListAssign(m_lstStrVarParameters, C.m_lstStrVarParameters);
	ListAssign(m_lstStrVarDependencies, C.m_lstStrVarDependencies);
	return *this;
}

VOID CStringVar::ListAssign(CList<CString, CString&>& lstToAssignTo, CList<CString, CString&>& lstToAssign)
{
	POSITION pos;
	int i, iParamsCount;
	CString strTemp;

	while (!lstToAssignTo.IsEmpty()) lstToAssignTo.RemoveTail();
	pos = lstToAssign.GetHeadPosition();
	iParamsCount = lstToAssign.GetCount();
	i = 0;
	while (i < iParamsCount)
	{
		strTemp = lstToAssign.GetNext(pos);
		lstToAssignTo.AddTail(strTemp);
		i++;
	}
}

void CStringVar::Serialize(CArchive& ar)
{	
	int iParamCount;
	int iDependencyCount;
	int i;
	POSITION pos;
	CString strTmp;

	if (ar.IsStoring())
	{
		// storing code 				
		ar << m_strStrVarName;
		ar << m_strStrVarBody;	
		ar << m_fSaveInTheBookmarksFile;

		iParamCount = m_lstStrVarParameters.GetCount();
		ar << iParamCount;
		pos = m_lstStrVarParameters.GetHeadPosition();
		for (i = 0; i < iParamCount; i++)
		{
			strTmp = m_lstStrVarParameters.GetNext(pos);
			ar << strTmp;
		}

		iDependencyCount = m_lstStrVarDependencies.GetCount();
		ar << iDependencyCount;
		pos = m_lstStrVarDependencies.GetHeadPosition();
		for (i = 0; i < iDependencyCount; i++)
		{
			strTmp = m_lstStrVarDependencies.GetNext(pos);
			ar << strTmp;
		}		
	}
	else
	{
		iParamCount = m_lstStrVarParameters.GetCount();			
		for (i = 0; i < iParamCount; i++) m_lstStrVarParameters.RemoveTail();
		iDependencyCount = m_lstStrVarDependencies.GetCount();
		for (i = 0; i < iDependencyCount; i++) m_lstStrVarDependencies.RemoveTail();

		// loading code
		ar >> m_strStrVarName;
		ar >> m_strStrVarBody;		
		ar >> m_fSaveInTheBookmarksFile;

		ar >> iParamCount;		
		for (i = 0; i < iParamCount; i++)
		{
			ar >> strTmp;
			m_lstStrVarParameters.AddTail(strTmp);
		}
		
		ar >> iDependencyCount;		
		for (i = 0; i < iDependencyCount; i++)
		{
			ar >> strTmp;
			m_lstStrVarDependencies.AddTail(strTmp);
		}		
	}
}