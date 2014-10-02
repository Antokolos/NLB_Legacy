#ifndef _CSTACK_H
#define _CSTACK_H
// Базовый стек на основе односвязного списка.

#include <process.h>	// exit();
#include <iostream>
#include "CL1List.h"

template <class T>
class CStack
{
protected:
	CL1List<T>* pst_List;	// Список, на основе которого делаем стек

public:
	// <"Большая четверка">
	CStack();					// Инициализация
	virtual ~CStack();			// Разрушение
	CStack(CStack& C);			// Конструктор копирования
	virtual CStack& operator = (CStack& C);	// Перегруженный оператор присваивания
	// </"Большая четверка">

	// Основные методы стека
	bool IsEmpty() const;		// Cтек пуст?
	void Push(const T& x);		// Добавить в стек
    bool Pop(T& x);				// Извлечь из стека
    void MakeEmpty();			// Cделать стек пустым
    bool DelTop();				// Удалить вершину стека
    bool SeeTop(T& x);			// Просмотреть вершину стека   
    bool ChangeTop(const T& x);	// Изменить вершину стека

	// Вспомогательные методы стека
	friend ostream& operator << (ostream& out, CStack<T>& C);	// Вывод в поток

protected:
	void HandleError(int iErrCode);
};

template <class T>
CStack<T>::CStack()
{if ((pst_List = new CL1List<T>) == NULL) HandleError(1); }

template <class T>
CStack<T>::~CStack()
{if (pst_List) delete pst_List; }

template <class T>
CStack<T>::CStack(CStack& C)
{if ((pst_List = new CL1List<T>(*(C.pst_List))) == NULL) HandleError(1); }

template <class T>
CStack<T>& CStack<T>::operator = (CStack<T>& C)
{
	if (this == &C) return *this;
	(*pst_List) = (*(C.pst_List));
	return *this;
}

// pst_List не может быть NULL, иначе ошибка выскочит еще в конструкторе!
// Потому в следующих ниже функциях не проверяем pst_List на NULL

template <class T>
bool CStack<T>::IsEmpty() const
{return pst_List->IsEmpty(); }	
	
template <class T>
void CStack<T>::Push(const T& x)
{pst_List->Insert(x); }
    
template <class T>
bool CStack<T>::Pop(T& x)
{return pst_List->Extract(x); }

template <class T>
void CStack<T>::MakeEmpty()
{pst_List->MakeEmpty(); }

template <class T>
bool CStack<T>::DelTop()
{return pst_List->DelElem(); }

template <class T>
bool CStack<T>::SeeTop(T& x)
{return pst_List->SeeElem(x); }

template <class T>
bool CStack<T>::ChangeTop(const T& x)
{return pst_List->ChangeElem(x); }

template <class T>
ostream& operator << (ostream& out, CStack<T>& C)
{
	out << (*(C.pst_List));
	return out;
}

template <class T>
void CStack<T>::HandleError(int iErrCode)
{
	cout << "Terrible error in CStack: ";
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