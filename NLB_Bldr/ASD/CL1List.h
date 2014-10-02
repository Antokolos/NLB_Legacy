#ifndef _CL1LIST_H
#define _CL1LIST_H
// �������� ����������� ������ (�������). 
// ���������� �� ���� ������ ���������� 
// � �������� ��������� � ������ ������.

#include <process.h>	// exit();
#include <iostream>

using namespace std;

template <class T>
struct L1Rec   //���������� �������� ������ - 
			   //������������� �������
{     
	T* pInfo;		  //���� ����������
	L1Rec* Next;	  //������ �� ��������� ������� ���� �� ����
};

template <class T>
class CL1List
{
protected:
	L1Rec<T>* m_pBuf;		// ��������� �� ������ ������ (�������� �������)	
	L1Rec<T>* m_pEnd;		// ��������� �� ����� ������ 
	L1Rec<T>* m_pNow;		// ��������� �� ������� ������� ������	
	
public:
	// <"������� ��������">
	CL1List();					// �������������
	virtual ~CL1List();			// ����������
	CL1List(CL1List<T>& C);	// ����������� �����������
	virtual CL1List<T>& operator = (CL1List<T>& C);	// ������������� �������� ������������
	// <"������� ��������">

	// �������� ������ ������
	void GoBegin();	// ����� � ������ ������	
	void GoEnd();	// ����� � ����� ������
	bool IsBegin() const;	// ������ ������??? (1-��, 0-���)
	bool IsEnd() const;	// ����� ������??? (1-��, 0-���)
	bool Next();	// ������� � ���������� �������� ������	
	bool Extract(T& x);	        // ������� ������� ������ �� ���������� ������
	void Insert(const T& x);	// �������� ������� �� ���������� ������	
	bool IsEmpty() const;				// ������ ����??? (1-��, 0-���)
	void MakeEmpty();			// C������ ������ ������
	bool SeeElem(T& x) const;				// ���������� �������� �������� �� ���������� ������
	T* GetElemPtr() const;					// �������� ��������� �� ������� ������ �� ���������� ������
	L1Rec<T>* GetCurrentPos() {return m_pNow; }
	void SetCurrentPos(const L1Rec<T>* pPos) {m_pNow = (L1Rec<T>*) pPos; }	// � ���� �������� ���� ���� ����� ����������, ��� ���, ����� �������� ���������, ����� ��������� ��� ��������� ������
	bool ChangeElem(const T& x);			// �������� �������� �������� �� ���������� ������
	bool DelElem();					// ������� ������� �� ���������� ������	
	bool ReplaceElemFromList(CL1List<T>& C);		// ����������� ������� �� ���������� ������ C �� ��������� �������� ������
		// ������ ����� �������, ���� �������� ������ - ���������� ������,
		// � �������� �� �������� �����������, ������� ��������� �����
		// ����� ����� ��� ������������� ���� Extract - Insert
	bool ReplaceTailFromList(CL1List<T>& C);	// ������ �� ReplaceElemFromList, �� ������������ ���� ������� ������ C
	
	// ��������������� ������ ������
	bool Search(const T& toFind);	// ����� ������� toFind � ������	
	friend ostream& operator << (ostream& out, CL1List<T>& C);	// ����� � �����

protected:
	void HandleError(int iErrCode);
};

template <class T>
CL1List<T>::CL1List()
{	
	if ((m_pBuf = new L1Rec<T>) == NULL) HandleError(1);
	m_pBuf->pInfo = NULL;	// �.�. ��� �����, � ��� ��� ����������
	m_pBuf->Next = NULL;	// �.�. ���� ��� ����� ������!		
	m_pEnd = m_pNow = m_pBuf;	
}
/*----------------------------------------------------------------------------------------*/
template <class T>
CL1List<T>::~CL1List()
{
	MakeEmpty();

	// ������ ���������, �� ������� �������� �������. ���� ��� ����������.
	if (m_pBuf) delete m_pBuf;	
}
/*----------------------------------------------------------------------------------------*/
template <class T>
CL1List<T>::CL1List(CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pCur;				// ������ ��� ��������� ������� ������������ ������.
	L1Rec<T>* pPrev;			// ������ ���������� ������� ������������ ������. ����� ��� ��������� ������.
	L1Rec<T>* pOldPtrPosInC;	// ����� ����� ������ ������� ��������� ������ � ������ C � ������� �������
/*=</Locals>==============================================================================*/
	
	if ((m_pBuf = new L1Rec<T>) == NULL) HandleError(1);
	m_pBuf->pInfo = NULL;	// �.�. ��� �����, � ��� ��� ����������
	// m_pBufBegin->Next - ��� ����� ��������� �����, � �����

	pPrev = m_pBuf;
	pOldPtrPosInC = C.m_pNow;	
	C.GoBegin();
	while (!C.IsEnd())
	{
		if ((pCur = new L1Rec<T>) == NULL) HandleError(1);
		if ((pCur->pInfo = new T) == NULL) HandleError(1);		
		C.SeeElem(*(pCur->pInfo));
		if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// � ����� ������ ��������� ����� ������ � ��� �� �������
		pPrev->Next = pCur;
		pPrev = pCur;		
		C.Next();
	}
	if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// � ����� ������ ��������� ����� ������ � ��� �� �������
	m_pEnd = pPrev;
	pPrev->Next = NULL;			// ��� ����� ������!
	C.m_pNow = pOldPtrPosInC;	// ������ ��������� ������ C � ������� �������	
}
/*----------------------------------------------------------------------------------------*/
template <class T>
CL1List<T>& CL1List<T>::operator = (CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pCur;				// ������ ��� ��������� ������� ������������ ������.
	L1Rec<T>* pPrev;			// ������ ���������� ������� ������������ ������. ����� ��� ��������� ������.
	L1Rec<T>* pOldPtrPosInC;	// ����� ����� ������ ������� ��������� ������ � ������ C � ������� �������
/*=</Locals>==============================================================================*/

	if (this == &C) return *this;
	MakeEmpty();

	// � ��� �������� ������� ����� �� ���������, ��� ��� �� � ��� ��� ����.
	pPrev = m_pBuf;
	pOldPtrPosInC = C.m_pNow;	
	C.GoBegin();
	while (!C.IsEnd())
	{
		if ((pCur = new L1Rec<T>) == NULL) HandleError(1);
		if ((pCur->pInfo = new T) == NULL) HandleError(1);		
		C.SeeElem(*(pCur->pInfo));
		if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// � ����� ������ ��������� ����� ������ � ��� �� �������
		pPrev->Next = pCur;
		pPrev = pCur;		
		C.Next();
	}
	if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// � ����� ������ ��������� ����� ������ � ��� �� �������
	m_pEnd = pPrev;
	pPrev->Next = NULL;			// ��� ����� ������!
	C.m_pNow = pOldPtrPosInC;	// ������ ��������� ������ C � ������� �������	
	return *this;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
void CL1List<T>::GoBegin()
{m_pNow = m_pBuf; }
/*----------------------------------------------------------------------------------------*/
template <class T>
void CL1List<T>::GoEnd()
{m_pNow = m_pEnd; }
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::IsBegin() const
{return (m_pNow->pInfo == NULL); }
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::IsEnd() const
{return (m_pNow->Next == NULL); }
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::Next()
{
	if (m_pNow->Next != NULL) 
	{
		m_pNow = m_pNow->Next;
		return true;
	}
	else
		return false;
}
/*----------------------------------------------------------------------------------------*/		
template <class T>
bool CL1List<T>::Extract(T& x)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pExtrRec;	// ��������� �� ����������� ������ ������	
/*=</Locals>==============================================================================*/

	pExtrRec = m_pNow->Next;

	if (pExtrRec)	// �.�. ��� �� ����� ������	
	{
		x = (*(pExtrRec->pInfo));		// ����������� ����������...
		m_pNow->Next = pExtrRec->Next;			// �������� ������ ������...
		if (pExtrRec->Next == NULL)	// ����������� ������ - ���������
		{
			// ���� ���, �� ��������� ������ �� ���������������
			m_pEnd = m_pNow;
		}
		delete (pExtrRec->pInfo);				// ����� ���������� ����������� ������...
		delete pExtrRec;						// ����� ����������� ������!
		return true;
	}
	else
		return false;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
void CL1List<T>::Insert(const T& x)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pCur;				// ������ ��� ��������� ������� ������.
/*=</Locals>==============================================================================*/

	if ((pCur = new L1Rec<T>) == NULL) HandleError(1);
	if ((pCur->pInfo = new T) == NULL) HandleError(1);
	(*(pCur->pInfo)) = x;
	pCur->Next = m_pNow->Next;
	if (m_pNow->Next == NULL)	// ������� ������� ��� ���������
	{
		// ����� ��������� ������ ����������� �������
		m_pEnd = pCur;
	}
	m_pNow->Next = pCur;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::IsEmpty() const
{return ((m_pNow->pInfo == NULL) && (m_pNow->Next == NULL)); }	// �.�. ��� ������������ � ������ � �����, � ������, ������ ����.
/*----------------------------------------------------------------------------------------*/
template <class T>
void CL1List<T>::MakeEmpty()
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pExtrRec;	// ��������� �� ����������� ������ ������	
/*=</Locals>==============================================================================*/

	m_pNow = m_pBuf;	// �.�. ��������� ������ - � ������!
	while (m_pNow->Next)
	{
		pExtrRec = m_pNow->Next;
		m_pNow->Next = pExtrRec->Next;			// �������� ������ ������...		
		delete (pExtrRec->pInfo);				// ����� ���������� ����������� ������...
		delete pExtrRec;						// ����� ����������� ������!
	}
	m_pEnd = m_pNow;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::SeeElem(T& x) const
{
	if (m_pNow->Next)	// �.�. ��� �� ����� ������	
	{
		x = (*(m_pNow->Next->pInfo));		// ����������� ����������...		
		return true;
	}
	else
		return false;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::ChangeElem(const T& x)
{
	if (m_pNow->Next)	// �.�. ��� �� ����� ������	
	{
		(*(m_pNow->Next->pInfo)) = x;		// �������� ����������...		
		return true;
	}
	else
		return false;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::DelElem()
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pExtrRec;	// ��������� �� ��������� ������ ������	
/*=</Locals>==============================================================================*/

	pExtrRec = m_pNow->Next;

	if (pExtrRec)	// �.�. ��� �� ����� ������	
	{
		m_pNow->Next = pExtrRec->Next;			// �������� ������ ������...
		if (pExtrRec->Next == NULL)	// ��������� ������ - ���������
		{
			// ���� ���, �� ��������� ������ �� ���������������
			m_pEnd = m_pNow;
		}
		delete (pExtrRec->pInfo);				// ����� ���������� ��������� ������...
		delete pExtrRec;						// ����� ��������� ������!
		return true;
	}
	else
		return false;
}
/*----------------------------------------------------------------------------------------*/	
template <class T>	
bool CL1List<T>::Search(const T& toFind) 
// �� ��������� ������ ��������� ����� ����� ��������� ��������� (��� � ����� ������)
{
/*=<Locals>===============================================================================*/
	T Elem;
/*=</Locals>==============================================================================*/

	GoBegin();
	while (!IsEnd())
	{
		SeeElem(Elem);	
		if (toFind == Elem) return true;
		Next();
	}

	return false;
}
/*----------------------------------------------------------------------------------------*/	
template <class T>	
ostream& operator << (ostream& out, CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pOldPtrPosInC;	// ����� ����� ������ ������� ��������� ������ � ������ C � ������� �������
	T Elem;
/*=</Locals>==============================================================================*/

	pOldPtrPosInC = C.m_pNow;	
	C.GoBegin();
	while (!C.IsEnd())
	{
		if (pOldPtrPosInC == C.m_pNow) out << '|';
		C.SeeElem(Elem);
		out << Elem << " - ";		
		C.Next();
	}
	if (pOldPtrPosInC == C.m_pNow) out << '|';
	C.m_pNow = pOldPtrPosInC;	// ������ ��������� ������ C � ������� �������	
	return out;
}
/*----------------------------------------------------------------------------------------*/
template <class T>	
T* CL1List<T>::GetElemPtr() const
{
	if (m_pNow->Next)	// �.�. ��� �� ����� ������	
		return m_pNow->Next->pInfo;
	else
		return NULL;	
}
/*----------------------------------------------------------------------------------------*/
template <class T>	
bool CL1List<T>::ReplaceElemFromList(CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pForReplace;	// ������������ �������
/*=</Locals>==============================================================================*/

	if (C.m_pNow->Next == NULL) return false;	// ��������� ������ C � ����� => ��������� ������...
	pForReplace = C.m_pNow->Next;
	C.m_pNow->Next = pForReplace->Next;		// ������ � ������ C ����� �������� ��� ���!
	if (C.m_pNow->Next == NULL) C.m_pEnd = C.m_pNow;	// ���� �����, � �������� ���������� ���� �������� ������� ������ ����� ���������...

	// ��������� ������� � ��� ������
	pForReplace->Next = m_pNow->Next;
	m_pNow->Next = pForReplace;	// ������ �� �������� � ��� ������
	if (pForReplace->Next == NULL) m_pEnd = pForReplace;	// ���� �����, � �������� ���������� ���� �������� ������� ������ ����� ���������...
	return true;
}
/*----------------------------------------------------------------------------------------*/
template <class T>	
bool CL1List<T>::ReplaceTailFromList(CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pForReplaceFirst;	// ������ ������������� "������"
	L1Rec<T>* pForReplaceLast;	// ����� ������������� "������"
/*=</Locals>==============================================================================*/

	if (C.m_pNow->Next == NULL) return false;	// ��������� ������ C � ����� => ��������� ������...
	pForReplaceFirst = C.m_pNow->Next;
	pForReplaceLast = C.m_pEnd;
	C.m_pEnd = C.m_pNow;	// ���������� ��������, ��� ������ ��� � ����� ����
	C.m_pNow->Next = NULL;	// �������� ����� ������...	

	// ��������� ����� � ��� ������ (� ������� �������!!!)
	pForReplaceLast->Next = m_pNow->Next;
	m_pNow->Next = pForReplaceFirst;	// ������ ����� �������� � ��� ������
	if (pForReplaceLast->Next == NULL) m_pEnd = pForReplaceLast;	// ���� �����, � �������� ���������� ���� �������� ������� ������ ����� ���������...
	return true;
}
/*----------------------------------------------------------------------------------------*/
template <class T>	
void CL1List<T>::HandleError(int iErrCode)
{
	cout << "Terrible error in CL1List: ";
	switch (iErrCode)
	{
		case 1:
			cout << "Out of memory!" << endl;
			exit(1);
			break;

		default: 
			cout << "Unknown." << endl;
			exit(1);
			break;
	}
}

#endif