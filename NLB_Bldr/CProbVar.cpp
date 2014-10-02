#include "StdAfx.h"
#include "CProbVar.h"

IMPLEMENT_SERIAL(CProbVar, CObject, VERSIONABLE_SCHEMA | 1)

CProbVar::CProbVar()
{
	m_strProbVarName = _TEXT("");
	m_iDiceK = 1;
	m_iDiceY = 1;	
}

CProbVar::~CProbVar() 
{	
}

CProbVar::CProbVar(const CProbVar& C)
{
	m_strProbVarName = C.m_strProbVarName;
	m_iDiceK = C.m_iDiceK;
	m_iDiceY = C.m_iDiceY;
}

CProbVar& CProbVar::operator = (const CProbVar& C)
{
	if (this==&C) return *this;
	m_strProbVarName = C.m_strProbVarName;
	m_iDiceK = C.m_iDiceK;
	m_iDiceY = C.m_iDiceY;
	return *this;
}

BOOL CProbVar::Set_iDiceK(int _iDiceK)
{
	if (_iDiceK >= 1)
	{
		m_iDiceK = _iDiceK;
		return TRUE;
	}
	else
		return FALSE;
}
	
BOOL CProbVar::Set_iDiceY(int _iDiceY)
{
	if (_iDiceY >= 1)
	{
		m_iDiceY = _iDiceY;
		return TRUE;
	}
	else
		return FALSE;
}

void CProbVar::Serialize(CArchive& ar)
{	
	if (ar.IsStoring())
	{
		// storing code 				
		ar << m_strProbVarName;
		ar << m_iDiceK;		
		ar << m_iDiceY;		
	}
	else
	{
		// loading code
		ar >> m_strProbVarName;
		ar >> m_iDiceK;		
		ar >> m_iDiceY;	
	}
}
