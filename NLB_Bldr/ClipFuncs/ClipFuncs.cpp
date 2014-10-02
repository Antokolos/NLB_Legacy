#include "stdafx.h"
#include "ClipFuncs.h"

BOOL IsLineIntersectClipArea(const POINT& ptBegin, 
							 const POINT& ptEnd, 
							 const RECT& rctClipArea)
/* �������� �����-���������� ��������� ������� �������� ������.
���������� TRUE, ���� ������� �������� � ������� ������, FALSE �����
��������� ������������ �������������� ��������� �������:

	  |      |
 1010 | 1000 | 1001
______|______|______
      |      | 
 0010 | 0000 | 0001
______|______|______
      |      |
 0110 | 0100 | 0101
      |      |

  ���� � ����� 0000 ������������ ����� ������� ������
 

*/
{
	_BYTE btCodeBegin = GetPtCode(ptBegin, rctClipArea);
	_BYTE btCodeEnd = GetPtCode(ptEnd, rctClipArea);
	_BYTE btTempResult;
	LONG a1, b1;	// ������������ ������ - ������ �����
	LONG yRight, yLeft;	// �������� ������ � ��������������� ������, ���������� �� ��������� ����������� - ������ �����
	LONG BottomBound, TopBound;	// �������, � �������� ����� ������������ �������� ������
	LONG lFactor;	// �����������, ������������� � yRight, yLeft

	if ((btCodeBegin & btCodeEnd) != 0x0) return FALSE;	// ������� ��������� ����� �� ����� � ������� ������.
	if ((btCodeBegin == 0x0) || (btCodeEnd == 0x0)) return TRUE;	// ������� ��������� ����� ����� � ������� ������. �����, �� �������, �� �����!
	btTempResult = btCodeBegin | btCodeEnd;
	if ((btTempResult == 0x03) ||	// 0011 == 0010 | 0001
		(btTempResult == 0x0C))		// 1100 == 1000 | 0100
		return TRUE;	// ��. ������� � ���������� ������������, ����� ������.
	/*
	� ���������� ������� ��� ������ �� ��� �� ����������. �������� ����������� ����������.
	��-������, �����������, ��� ����� ������� ����� ��� (x0, y0), (x1, y1). 
	����� � ������� ��������� ���������� ����� �������� ��������� ��� ������������� ������:
	y = ax + b (*) - ��������� ������, ���������� ��������� �������:
	a = (y1 - y0)/(x1 - x0),
	b = y0 - x0*a
	������ ����� ��������� �������� y � ������ 
	1) x = rctClipArea.left 
	2) x = rctClipArea.right
	�, ���� ��� ��� (������������) ����� ���� ��� ���� ������� ������, 
	������� ����� � ���, ��� ������ �� ���������� ������� ������ (������, ������� ����)
	��� ����, ����� �� ���������� ����� ���������� � ������������ ������,
	�������� ��� ����� (*) �� x1 - x0 (x1 - x0 != 0, ��� ��� ����� ������� ������� ������)
	�������:
	y1 = (x1 - x0)*y = a1*x + b1, ���
	a1 = y1 - y0,
	b1 = y0*(x1 - x0) - x0*a1.
	*/
	lFactor = ptEnd.x - ptBegin.x;	// �������, ��� lFactor != 0, ��� ��� ����� ������� ������� ������!!! �����! 
	a1 = ptEnd.y - ptBegin.y,
	b1 = ptBegin.y*lFactor - ptBegin.x*a1;

	// ������� �������� ������ � ������ ������, ����������� �� ������ �������� lFactor:
	// ������� ������ ���������...
	yLeft = a1*rctClipArea.left + b1;	
	yRight = a1*rctClipArea.right + b1;
	BottomBound = rctClipArea.bottom * lFactor;
	TopBound = rctClipArea.top * lFactor;
	if (lFactor < 0)	// ���� lFactor < 0, �� ���� ������� � ���������� ���������
	{
		yLeft = -yLeft;
		yRight = -yRight;
		BottomBound = -BottomBound;
		TopBound = -TopBound;
	}
	 		
	if ((yLeft > BottomBound) && (yRight > BottomBound)) return FALSE;	// ������ ����� ������� ���� ������� ��������� => �� ����� 
	if ((yLeft < TopBound) && (yRight < TopBound)) return FALSE;	// ������ ����� ������� ���� ������� ��������� => �� ����� 
	return TRUE;	// ���� �� ������� � ������ ������ ��� ��� ���������, �� �� ���-���� ���������� ������� ������
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