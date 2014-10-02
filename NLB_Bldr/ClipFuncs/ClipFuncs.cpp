#include "stdafx.h"
#include "ClipFuncs.h"

BOOL IsLineIntersectClipArea(const POINT& ptBegin, 
							 const POINT& ptEnd, 
							 const RECT& rctClipArea)
/* Алгоритм Коэна-Сазерленда отсечения отрезка областью вывода.
Возвращает TRUE, если отрезок попадает в область вывода, FALSE иначе
Кодировка пространства осуществляется следующим образом:

	  |      |
 1010 | 1000 | 1001
______|______|______
      |      | 
 0010 | 0000 | 0001
______|______|______
      |      |
 0110 | 0100 | 0101
      |      |

  Блок с кодом 0000 представляет собой область вывода
 

*/
{
	_BYTE btCodeBegin = GetPtCode(ptBegin, rctClipArea);
	_BYTE btCodeEnd = GetPtCode(ptEnd, rctClipArea);
	_BYTE btTempResult;
	LONG a1, b1;	// Коэффициенты прямой - смотри далее
	LONG yRight, yLeft;	// Значения прямой в соответствующих точках, умноженное на некоторый коэффициент - смотри далее
	LONG BottomBound, TopBound;	// Границы, с которыми будет сравниваться значение прямой
	LONG lFactor;	// Коэффициент, упоминавшийся в yRight, yLeft

	if ((btCodeBegin & btCodeEnd) != 0x0) return FALSE;	// Отрезок абсолютно точно НЕ ЛЕЖИТ в области вывода.
	if ((btCodeBegin == 0x0) || (btCodeEnd == 0x0)) return TRUE;	// Отрезок абсолютно точно ЛЕЖИТ в области вывода. Может, не целикос, но лежит!
	btTempResult = btCodeBegin | btCodeEnd;
	if ((btTempResult == 0x03) ||	// 0011 == 0010 | 0001
		(btTempResult == 0x0C))		// 1100 == 1000 | 0100
		return TRUE;	// См. рисунок с кодировкой пространства, чтобы понять.
	/*
	В оставшихся случаях так просто мы уже не отделаемся. Придется производить вычисления.
	Во-первых, предположим, что концы отрезка имеют вид (x0, y0), (x1, y1). 
	Тогда с помощью несложных вычислений можно получить выражения для коэффициентов прямой:
	y = ax + b (*) - уравнение прямой, содержащей указанный отрезок:
	a = (y1 - y0)/(x1 - x0),
	b = y0 - x0*a
	Теперь можно посчитать значение y в точках 
	1) x = rctClipArea.left 
	2) x = rctClipArea.right
	и, если они оба (одновременно) лежат выше или ниже области вывода, 
	сделать вывод о том, что прямая не пересекает область вывода (значит, отрезок тоже)
	Для того, чтобы не привлекать здесь вычисления с вещественной точкой,
	домножим обе части (*) на x1 - x0 (x1 - x0 != 0, так как иначе функция вылетит раньше)
	Получим:
	y1 = (x1 - x0)*y = a1*x + b1, где
	a1 = y1 - y0,
	b1 = y0*(x1 - x0) - x0*a1.
	*/
	lFactor = ptEnd.x - ptBegin.x;	// Заметим, что lFactor != 0, так как иначе функция вылетит раньше!!! Важно! 
	a1 = ptEnd.y - ptBegin.y,
	b1 = ptBegin.y*lFactor - ptBegin.x*a1;

	// Считаем значение прямой в данных точках, домноженное на модуль величины lFactor:
	// Сначала просто вычисляем...
	yLeft = a1*rctClipArea.left + b1;	
	yRight = a1*rctClipArea.right + b1;
	BottomBound = rctClipArea.bottom * lFactor;
	TopBound = rctClipArea.top * lFactor;
	if (lFactor < 0)	// Если lFactor < 0, то надо перейти к абсолютным величинам
	{
		yLeft = -yLeft;
		yRight = -yRight;
		BottomBound = -BottomBound;
		TopBound = -TopBound;
	}
	 		
	if ((yLeft > BottomBound) && (yRight > BottomBound)) return FALSE;	// Прямая лежит целиком ниже области отсечения => не видна 
	if ((yLeft < TopBound) && (yRight < TopBound)) return FALSE;	// Прямая лежит целиком выше области отсечения => не видна 
	return TRUE;	// Если уж отрезок с честью прошел все эти испытания, то он все-таки пересекает область вывода
}

_BYTE GetPtCode(const POINT& pt, const RECT& rctClipArea)
{
	_BYTE btResult = 0x0;
	if (pt.y < rctClipArea.top)		btResult |= MASK_1000;
	if (pt.y > rctClipArea.bottom)  btResult |= MASK_0100;
	if (pt.x < rctClipArea.left)	btResult |= MASK_0010;
	if (pt.x > rctClipArea.right)	btResult |= MASK_0001;
	return btResult;
}