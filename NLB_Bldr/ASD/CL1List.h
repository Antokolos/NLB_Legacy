#ifndef _CL1LIST_H
#define _CL1LIST_H
// Линейный односвязный список (базовый). 
// Реализация на базе списка указателей 
// с буферным элементом в начале списка.

#include <process.h>	// exit();
#include <iostream>

using namespace std;

template <class T>
struct L1Rec   //Физические элементы списка - 
			   //информационые кирпичи
{     
	T* pInfo;		  //Поле информации
	L1Rec* Next;	  //Ссылка на следующий элемент того же типа
};

template <class T>
class CL1List
{
protected:
	L1Rec<T>* m_pBuf;		// Указатель на начало списка (буферный элемент)	
	L1Rec<T>* m_pEnd;		// Указатель на конец списка 
	L1Rec<T>* m_pNow;		// Указатель на текущий элемент списка	
	
public:
	// <"Большая четверка">
	CL1List();					// Инициализация
	virtual ~CL1List();			// Разрушение
	CL1List(CL1List<T>& C);	// Конструктор копирования
	virtual CL1List<T>& operator = (CL1List<T>& C);	// Перегруженный оператор присваивания
	// <"Большая четверка">

	// Основные методы списка
	void GoBegin();	// Стать в начало списка	
	void GoEnd();	// Стать в конец списка
	bool IsBegin() const;	// Начало списка??? (1-да, 0-нет)
	bool IsEnd() const;	// Конец списка??? (1-да, 0-нет)
	bool Next();	// Перейти к следующему элементу списка	
	bool Extract(T& x);	        // Извлечь элемент списка за указателем списка
	void Insert(const T& x);	// Вставить элемент за указателем списка	
	bool IsEmpty() const;				// Список пуст??? (1-да, 0-нет)
	void MakeEmpty();			// Cделать список пустым
	bool SeeElem(T& x) const;				// Посмотреть значение элемента за указателем списка
	T* GetElemPtr() const;					// Получить указатель на элемент списка за указателем списка
	L1Rec<T>* GetCurrentPos() {return m_pNow; }
	void SetCurrentPos(const L1Rec<T>* pPos) {m_pNow = (L1Rec<T>*) pPos; }	// С этой функцией надо быть очень осторожным, так как, подав неверный указатель, можно разрушить всю структуру списка
	bool ChangeElem(const T& x);			// Изменить значение элемента за указателем списка
	bool DelElem();					// Удалить элемент за указателем списка	
	bool ReplaceElemFromList(CL1List<T>& C);		// Переместить элемент за указателем списка C за указатель текущего списка
		// Данный метод полезен, если элементы списка - громоздкие классы,
		// и хотелось бы избежать копирования, которое неминуемо будет
		// иметь место при использовании пары Extract - Insert
	bool ReplaceTailFromList(CL1List<T>& C);	// Похоже на ReplaceElemFromList, но перемещается весь остаток списка C
	
	// Вспомогательные методы списка
	bool Search(const T& toFind);	// Найти элемент toFind а списке	
	friend ostream& operator << (ostream& out, CL1List<T>& C);	// Вывод в поток

protected:
	void HandleError(int iErrCode);
};

template <class T>
CL1List<T>::CL1List()
{	
	if ((m_pBuf = new L1Rec<T>) == NULL) HandleError(1);
	m_pBuf->pInfo = NULL;	// Т.к. это буфер, в нем нет информации
	m_pBuf->Next = NULL;	// Т.к. пока это конец списка!		
	m_pEnd = m_pNow = m_pBuf;	
}
/*----------------------------------------------------------------------------------------*/
template <class T>
CL1List<T>::~CL1List()
{
	MakeEmpty();

	// Список опустошен, но остался буферный элемент. Надо его прикончить.
	if (m_pBuf) delete m_pBuf;	
}
/*----------------------------------------------------------------------------------------*/
template <class T>
CL1List<T>::CL1List(CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pCur;				// Только что созданный элемент заполняемого списка.
	L1Rec<T>* pPrev;			// Хранит предыдущий элемент заполняемого списка. Нужно для установки связей.
	L1Rec<T>* pOldPtrPosInC;	// Чтобы после работы вернуть указатель списка в списке C в прежнюю позицию
/*=</Locals>==============================================================================*/
	
	if ((m_pBuf = new L1Rec<T>) == NULL) HandleError(1);
	m_pBuf->pInfo = NULL;	// Т.к. это буфер, в нем нет информации
	// m_pBufBegin->Next - это будет присвоено позже, в цикле

	pPrev = m_pBuf;
	pOldPtrPosInC = C.m_pNow;	
	C.GoBegin();
	while (!C.IsEnd())
	{
		if ((pCur = new L1Rec<T>) == NULL) HandleError(1);
		if ((pCur->pInfo = new T) == NULL) HandleError(1);		
		C.SeeElem(*(pCur->pInfo));
		if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// В новом списке указатель будет стоять в той же позиции
		pPrev->Next = pCur;
		pPrev = pCur;		
		C.Next();
	}
	if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// В новом списке указатель будет стоять в той же позиции
	m_pEnd = pPrev;
	pPrev->Next = NULL;			// Это конец списка!
	C.m_pNow = pOldPtrPosInC;	// Вернем указатель списка C в прежнюю позицию	
}
/*----------------------------------------------------------------------------------------*/
template <class T>
CL1List<T>& CL1List<T>::operator = (CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pCur;				// Только что созданный элемент заполняемого списка.
	L1Rec<T>* pPrev;			// Хранит предыдущий элемент заполняемого списка. Нужно для установки связей.
	L1Rec<T>* pOldPtrPosInC;	// Чтобы после работы вернуть указатель списка в списке C в прежнюю позицию
/*=</Locals>==============================================================================*/

	if (this == &C) return *this;
	MakeEmpty();

	// А вот буферный элемент можно не создавать, так как он и так уже есть.
	pPrev = m_pBuf;
	pOldPtrPosInC = C.m_pNow;	
	C.GoBegin();
	while (!C.IsEnd())
	{
		if ((pCur = new L1Rec<T>) == NULL) HandleError(1);
		if ((pCur->pInfo = new T) == NULL) HandleError(1);		
		C.SeeElem(*(pCur->pInfo));
		if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// В новом списке указатель будет стоять в той же позиции
		pPrev->Next = pCur;
		pPrev = pCur;		
		C.Next();
	}
	if (pOldPtrPosInC == C.m_pNow) m_pNow = pPrev;	// В новом списке указатель будет стоять в той же позиции
	m_pEnd = pPrev;
	pPrev->Next = NULL;			// Это конец списка!
	C.m_pNow = pOldPtrPosInC;	// Вернем указатель списка C в прежнюю позицию	
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
	L1Rec<T>* pExtrRec;	// Указатель на извлекаемую запись списка	
/*=</Locals>==============================================================================*/

	pExtrRec = m_pNow->Next;

	if (pExtrRec)	// Т.е. еще не конец списка	
	{
		x = (*(pExtrRec->pInfo));		// Скопировали содержимое...
		m_pNow->Next = pExtrRec->Next;			// Поменяли ссылку списка...
		if (pExtrRec->Next == NULL)	// Извлекаемая запись - последняя
		{
			// Если так, то последней станет ее предшественница
			m_pEnd = m_pNow;
		}
		delete (pExtrRec->pInfo);				// Убили содержимое извлекаемой записи...
		delete pExtrRec;						// Убили извлекаемую запись!
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
	L1Rec<T>* pCur;				// Только что созданный элемент списка.
/*=</Locals>==============================================================================*/

	if ((pCur = new L1Rec<T>) == NULL) HandleError(1);
	if ((pCur->pInfo = new T) == NULL) HandleError(1);
	(*(pCur->pInfo)) = x;
	pCur->Next = m_pNow->Next;
	if (m_pNow->Next == NULL)	// Текущий элемент был последним
	{
		// Тогда последним станет вставляемый элемент
		m_pEnd = pCur;
	}
	m_pNow->Next = pCur;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::IsEmpty() const
{return ((m_pNow->pInfo == NULL) && (m_pNow->Next == NULL)); }	// Т.е. это одновременно и начало и конец, а значит, список пуст.
/*----------------------------------------------------------------------------------------*/
template <class T>
void CL1List<T>::MakeEmpty()
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pExtrRec;	// Указатель на извлекаемую запись списка	
/*=</Locals>==============================================================================*/

	m_pNow = m_pBuf;	// Т.е. указатель списка - в начало!
	while (m_pNow->Next)
	{
		pExtrRec = m_pNow->Next;
		m_pNow->Next = pExtrRec->Next;			// Поменяли ссылку списка...		
		delete (pExtrRec->pInfo);				// Убили содержимое извлекаемой записи...
		delete pExtrRec;						// Убили извлекаемую запись!
	}
	m_pEnd = m_pNow;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::SeeElem(T& x) const
{
	if (m_pNow->Next)	// Т.е. еще не конец списка	
	{
		x = (*(m_pNow->Next->pInfo));		// Скопировали содержимое...		
		return true;
	}
	else
		return false;
}
/*----------------------------------------------------------------------------------------*/
template <class T>
bool CL1List<T>::ChangeElem(const T& x)
{
	if (m_pNow->Next)	// Т.е. еще не конец списка	
	{
		(*(m_pNow->Next->pInfo)) = x;		// Поменяли содержимое...		
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
	L1Rec<T>* pExtrRec;	// Указатель на удаляемую запись списка	
/*=</Locals>==============================================================================*/

	pExtrRec = m_pNow->Next;

	if (pExtrRec)	// Т.е. еще не конец списка	
	{
		m_pNow->Next = pExtrRec->Next;			// Поменяли ссылку списка...
		if (pExtrRec->Next == NULL)	// Удаляемая запись - последняя
		{
			// Если так, то последней станет ее предшественница
			m_pEnd = m_pNow;
		}
		delete (pExtrRec->pInfo);				// Убили содержимое удаляемой записи...
		delete pExtrRec;						// Убили удаляемую запись!
		return true;
	}
	else
		return false;
}
/*----------------------------------------------------------------------------------------*/	
template <class T>	
bool CL1List<T>::Search(const T& toFind) 
// По окончанию работы указатель стоит перед найденным элементом (или в конце списка)
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
	L1Rec<T>* pOldPtrPosInC;	// Чтобы после работы вернуть указатель списка в списке C в прежнюю позицию
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
	C.m_pNow = pOldPtrPosInC;	// Вернем указатель списка C в прежнюю позицию	
	return out;
}
/*----------------------------------------------------------------------------------------*/
template <class T>	
T* CL1List<T>::GetElemPtr() const
{
	if (m_pNow->Next)	// Т.е. еще не конец списка	
		return m_pNow->Next->pInfo;
	else
		return NULL;	
}
/*----------------------------------------------------------------------------------------*/
template <class T>	
bool CL1List<T>::ReplaceElemFromList(CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pForReplace;	// Перемещаемый элемент
/*=</Locals>==============================================================================*/

	if (C.m_pNow->Next == NULL) return false;	// Указатель списка C в конце => вставлять нечего...
	pForReplace = C.m_pNow->Next;
	C.m_pNow->Next = pForReplace->Next;		// Теперь в списке C этого элемента уже нет!
	if (C.m_pNow->Next == NULL) C.m_pEnd = C.m_pNow;	// Быть может, в процессе выполнения этой операции текущая запись стала последней...

	// Вставляем элемент в наш список
	pForReplace->Next = m_pNow->Next;
	m_pNow->Next = pForReplace;	// Теперь он добавлен в наш список
	if (pForReplace->Next == NULL) m_pEnd = pForReplace;	// Быть может, в процессе выполнения этой операции текущая запись стала последней...
	return true;
}
/*----------------------------------------------------------------------------------------*/
template <class T>	
bool CL1List<T>::ReplaceTailFromList(CL1List<T>& C)
{
/*=<Locals>===============================================================================*/
	L1Rec<T>* pForReplaceFirst;	// Начало перемещаемого "хвоста"
	L1Rec<T>* pForReplaceLast;	// Конец перемещаемого "хвоста"
/*=</Locals>==============================================================================*/

	if (C.m_pNow->Next == NULL) return false;	// Указатель списка C в конце => вставлять нечего...
	pForReplaceFirst = C.m_pNow->Next;
	pForReplaceLast = C.m_pEnd;
	C.m_pEnd = C.m_pNow;	// Совершенно очевидно, что только так и может быть
	C.m_pNow->Next = NULL;	// Обрезали хвост списку...	

	// Вставляем хвост в наш список (в текущую позицию!!!)
	pForReplaceLast->Next = m_pNow->Next;
	m_pNow->Next = pForReplaceFirst;	// Теперь хвост добавлен в наш список
	if (pForReplaceLast->Next == NULL) m_pEnd = pForReplaceLast;	// Быть может, в процессе выполнения этой операции текущая запись стала последней...
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