#include "StdAfx.h"
#include "CBookVar.h"

IMPLEMENT_SERIAL(CBookVar, CObject, VERSIONABLE_SCHEMA | 1)

CBookVar::CBookVar()
{
	m_strBookVarName = _TEXT("");
	m_strBookVarBody = _TEXT("");
	m_fIsVarGlobal = FALSE;
}

CBookVar::~CBookVar() 
{	
}

CBookVar::CBookVar(const CBookVar& C)
{
	m_strBookVarName = C.m_strBookVarName;
	m_strBookVarBody = C.m_strBookVarBody;
	m_fIsVarGlobal = C.m_fIsVarGlobal;
}

CBookVar& CBookVar::operator = (const CBookVar& C)
{
	if (this==&C) return *this;
	m_strBookVarName = C.m_strBookVarName;
	m_strBookVarBody = C.m_strBookVarBody;
	m_fIsVarGlobal = C.m_fIsVarGlobal;
	return *this;
}

void CBookVar::Serialize(CArchive& ar)
{	
	if (ar.IsStoring())
	{
		// storing code 				
		ar << m_strBookVarName;
		ar << m_strBookVarBody;		
		ar << m_fIsVarGlobal;
	}
	else
	{
		// loading code
		ar >> m_strBookVarName;
		ar >> m_strBookVarBody;	
		ar >> m_fIsVarGlobal;
	}
}