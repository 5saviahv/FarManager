#define __NEW_H
#include "headers.hpp"
#pragma hdrstop

// dest � src �� ������ ������������
// maxlen - ������������ ����� ��������, ������� ����� �����������
//          � dest ��� ����� ��������������� ����, �.�. � �����
//          ������ ��� "sizeof-1"
char * __cdecl xstrncpy (char * dest,const char * src,size_t maxlen)
{
  char *tmpsrc = dest;
  while (maxlen && 0 != (*dest++ = *src++))
    --maxlen;
  *dest = 0;
  return tmpsrc;
}
