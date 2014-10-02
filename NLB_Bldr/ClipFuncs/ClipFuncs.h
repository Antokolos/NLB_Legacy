#ifndef _CLIPFUNCS_H
#define _CLIPFUNCS_H
#include <windows.h>	// POINT, RECT

typedef unsigned char _BYTE;
const _BYTE MASK_1000 = 0x08;	// 1000 в двоичном представлении
const _BYTE MASK_0100 = 0x04;	// 0100	в двоичном представлении
const _BYTE MASK_0010 = 0x02;	// 0010 в двоичном представлении
const _BYTE MASK_0001 = 0x01;	// 0001 в двоичном представлении


BOOL IsLineIntersectClipArea(const POINT& ptBegin, 
							 const POINT& ptEnd, 
							 const RECT& rctClipArea);
_BYTE GetPtCode(const POINT& pt, const RECT& rctClipArea);

#endif