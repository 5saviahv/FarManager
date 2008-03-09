#define __NEW_H
#include "headers.hpp"
#pragma hdrstop

// maxlen - ������������ ����� ��������, ������� ����� ���������
//          dest ��� ����� ��������������� ����, �.�. � �����
//          ������ ��� "sizeof-1"

char * __cdecl xstrncat(char * dest,const char * src, size_t maxlen)
{
	char * start=dest;
	while(*dest)
	{
		dest++;
		maxlen--;
	}
	while(maxlen--)
		if(!(*dest++=*src++))
			return start;
	*dest=0;
	return start;
}
