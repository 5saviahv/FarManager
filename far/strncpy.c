#include <string.h>
#include <memory.h>

// dest � src ����� ���ᥪ�����
char * __cdecl strncpy (char * dest,const char * src,size_t maxlen)
{
  size_t len;

  len = strlen(src);
  len = (len > maxlen)?maxlen:len;
  memmove(dest,src,len);
  // ����� �� try/except ���⠢���...
  dest[len]=0;
  return (dest);
}
