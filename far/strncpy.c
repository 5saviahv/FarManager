#include <string.h>
#include <memory.h>

// dest � src �� ������ ������������
// maxlen - ������������ ����� ��������, ������� ����� �����������
//          � dest ��� ����� ��������������� ����, �.�. � �����
//          ������ ��� "sizeof-1"
char * __cdecl strncpy (char * dest,const char * src,size_t maxlen)
{
  char *tmpsrc = dest;
  while (maxlen && 0 != (*dest++ = *src++))
    --maxlen;
  *dest = 0;
  return tmpsrc;
}
