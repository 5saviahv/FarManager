/*
farrtl.cpp

��������������� ������� ������ � �������: new/delete/malloc/realloc/free
*/

/* Revision: 1.16 20.02.2003 $ */

/*
Modify:
  20.02.2003 SVS
    ! xf_* ����� __cdecl, �.�. ������� � strdup.c, del.cpp � new.cpp
  26.01.2003 IS
    + FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  21.03.2002 SVS
    ! ������� ������� FarBsearch, FarSscanf, FarSprintf, FarQsortEx,
      FarQsort, FarAtoi64, FarAtoi, FarItoa64, FarItoa �� mix.cpp
      � farrtl.cpp
  27.02.2002 SVS
    ! ����� ���� ������� ����� � cmem.cpp
  26.02.2002 SVS
    - ���� � RTL VC (������ ��� ��������� � V64 ��� MSVC!)
  22.02.2002 SVS
    ! ��������� fseek64 � ftell64 (� �.�. ������ ����������� WINAPI)
  10.07.2001 SVS
    - ������ � ���� ����� �������� "fn.hpp" :-(
  31.10.2000 SVS
    ! ��������� ����� � ��� ���������� _nfile ������� (BC5.5.1 ��������)
  29.08.2000 SVS
    ! ��������� ��� ������� ��������� seek ��� __int64
  14.08.2000 SVS
    + ������� ��������� seek ��� __int64
  19.07.2000 SVS
    + ��������� ������� getdisk
      ��-�� �������� � ���������� ������� getdisk � BC & VC
      �� ������� AltFx ���� ������ ����� UNC ����
  13.07.2000 SVS (� ������ VVM)
    ! VVM> ��� � ������� - ���� block = NULL, �� 95/98 ����������
      VVM> ������, � �� ������������...
      VVM> if(!size)
      VVM>   HeapFree(FARHeapForNew,0,block);
  12.07.2000 IS
    ! � new ������� "void *p" �� "void *p=NULL" (SVS ����� ��� �������...)
    ! �������� �� NULL "���������" �� delete � free
  12.07.2000 SVS
    ! ���������� MEM_DELTA �� 4095
    + ��������� ���������� new/delete by IS
    + ��������� cmem � �������� ��������� ������.
  11.07.2000 SVS
    ! ����� �������� (� �������) ������������� ������.
  05.07.2000 IS
    ! ������� ���� ��������, ����� ������ ������� ���, �� ��� ��� �����
      �������, ���� �������������� new/delete :-(((
  04.07.2000 SVS
    ! ��������� � �������� ���������������� ������!
  03.07.2000 IS
    ! ��������� ���� ����� � ������
*/

#include "headers.hpp"
#pragma hdrstop
#include "fn.hpp"
#include "farconst.hpp"

/* $ 19.07.2000 SVS
  - ��-�� �������� � ���������� ������� getdisk � BC & VC
    �� ������� AltFx ���� ������ ����� UNC ����
    ���� ������� ��������� � farrtl.cpp
*/
int _cdecl getdisk(void)
{
  unsigned drive;
  char    buf[MAX_PATH];

  /* Use GetCurrentDirectory to get the current directory path, then
   * parse the drive name.
   */
  GetCurrentDirectory(sizeof(buf), buf);    /* ignore errors */
  drive = buf[0] >= 'a' ? buf[0] - 'a' + 1 : buf[0] - 'A' + 1;
  return (int)drive - 1;
}
/* SVS $*/

/* $ 14.08.2000 SVS
    + ������� ��������� seek ��� __int64
*/
#if defined(__BORLANDC__)
//#include <windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif

int  __IOerror    (int  __doserror);   /* returns -1 */
int  __NTerror     (void);              /* returns -1 */

#ifdef __cplusplus
};
#endif

#define RETURN(code)    {rc=(code); goto exit;}

//extern unsigned  _nfile;

/* Array of open file flags.
 */
extern unsigned int _openfd[];

/* Array of open file handles (not used on OS/2).
 */
extern long _handles[];

static int Displacement (FILE *fp)
{
  register    level;
  int         disp;
  register    unsigned char *P;

  if (fp->level < 0)
    disp = level = fp->bsize + fp->level + 1;
  else
    disp = level = fp->level;

  if (fp->flags & _F_BIN)
    return disp;

  P = fp->curp;

  if (fp->level < 0)          /* If writing */
  {
    while (level--)
     if ('\n' == *--P)
       disp++;
  }
  else                        /* Else reading */
  {
    while (level--)
     if ('\n' == *P++)
       disp++;
  }

  return  disp;
}


static __int64 __lseek64(int fd, __int64 offset, int kind)
{
  FAR_INT64 Number;

  DWORD  method;

  if ((unsigned)fd >= _nfile)
    return (__int64)__IOerror(ERROR_INVALID_HANDLE);

  /* Translate the POSIX seek type to the NT method.
   */
  switch (kind)
  {
    case SEEK_SET:
      method = FILE_BEGIN;
      break;
    case SEEK_CUR:
      method = FILE_CURRENT;
      break;
    case SEEK_END:
      method = FILE_END;
      break;
    default:
      return ((__int64)__IOerror(ERROR_INVALID_FUNCTION));
  }

  _openfd[fd] &= ~_O_EOF;      /* forget about ^Z      */

  Number.Part.HighPart=offset>>32;

  Number.Part.LowPart = SetFilePointer((HANDLE)_handles[fd], (DWORD)offset, &Number.Part.HighPart, method);
  if (Number.Part.LowPart == -1 && GetLastError() != NO_ERROR)
  {
    __NTerror();        /* set errno */
    Number.i64=-1;
  }

  return Number.i64;
}

int fseek64 (FILE *fp, __int64 offset, int whence)
{
  if (fflush (fp))
    return (EOF);

  if (SEEK_CUR == whence && fp->level > 0)
    offset -= Displacement (fp);

  fp->flags &= ~(_F_OUT | _F_IN | _F_EOF);
  fp->level = 0;
  fp->curp = fp->buffer;

  return (__lseek64 (fp->fd, offset, whence) == -1L) ? EOF : 0;
}

__int64 ftell64(FILE *fp)
{
  __int64  oldpos, rc;

  if ((rc = __lseek64( fp->fd, 0, SEEK_CUR )) != -1)
  {
    if (fp->level < 0)  /* if writing */
    {
      if (_openfd[fp->fd] & O_APPEND)
      {
        /* In append mode, find out how big the file is,
         * and add the number of buffered bytes to that.
         */
        oldpos = rc;
        if ((rc = __lseek64( fp->fd, 0, SEEK_END )) == -1)
          return rc;
        if (__lseek64( fp->fd, oldpos, SEEK_SET ) == -1)
          return -1;
      }
      rc += Displacement(fp);
    }
    else
      rc -= Displacement(fp);
  }
  return rc;
}

#else

extern "C"{
_CRTIMP __int64 __cdecl _ftelli64 (FILE *str);
_CRTIMP int __cdecl _fseeki64 (FILE *stream, __int64 offset, int whence);
};

__int64 ftell64(FILE *fp)
{
  return _ftelli64(fp);
}

int fseek64 (FILE *fp, __int64 offset, int whence)
{
  return _fseeki64(fp,offset,whence);
}

#endif
/* SVS $*/

/* $ 25.07.2000 SVS
   �������� ������ ������� ����������� �������, ����������� � WINAPI
*/
char *WINAPI FarItoa(int value, char *string, int radix)
{
  if(string)
    return itoa(value,string,radix);
  return NULL;
}
/* $ 28.08.2000 SVS
  + FarItoa64
*/
char *WINAPI FarItoa64(__int64 value, char *string, int radix)
{
  if(string)
    return _i64toa(value, string, radix);
  return NULL;
}
/* SVS $ */
int WINAPI FarAtoi(const char *s)
{
  if(s)
    return atoi(s);
  return 0;
}
__int64 WINAPI FarAtoi64(const char *s)
{
  if(s)
    return _atoi64(s);
  return 0i64;
}

void WINAPI FarQsort(void *base, size_t nelem, size_t width,
                     int (__cdecl *fcmp)(const void *, const void *))
{
  if(base && fcmp)
    qsort(base,nelem,width,fcmp);
}

/* $ 24.03.2001 tran
   ����� �����...*/
void WINAPI FarQsortEx(void *base, size_t nelem, size_t width,
                     int (__cdecl *fcmp)(const void *, const void *,void *user),void *user)
{
  if(base && fcmp)
    qsortex((char*)base,nelem,width,fcmp,user);
}
/* tran $ */

int WINAPIV FarSprintf(char *buffer,const char *format,...)
{
  int ret=0;
  if(buffer && format)
  {
    va_list argptr;
    va_start(argptr,format);
    ret=vsprintf(buffer,format,argptr);
    va_end(argptr);
  }
  return ret;
}

/* $ 29.08.2000 SVS
   - ������� ������������ ������� FarSscanf
   ������� - �.�. � VC ���� vsscanf, �� �������� ������������� (����� ��
   ���������� VC sscanf � "����������" �� ����
*/
#if defined(_MSC_VER)
extern "C" {
int __cdecl _input (FILE *stream,const unsigned char *format,va_list arglist);
};
#endif

int WINAPIV FarSscanf(const char *buffer, const char *format,...)
{
  if(!buffer || !format)
    return 0;
#if defined(_MSC_VER)
  // ������ ����� ������������� sscanf :-)
  va_list arglist;
  FILE str;
  FILE *infile = &str;
  int retval;

  va_start(arglist, format);

  infile->_flag = _IOREAD|_IOSTRG|_IOMYBUF;
  infile->_ptr = infile->_base = (char *) buffer;
  infile->_cnt = strlen(buffer);

  retval = (_input(infile,(const unsigned char *)format,arglist));

  return(retval);
#else
  va_list argptr;
  va_start(argptr,format);
  int ret=vsscanf(buffer,format,argptr);
  va_end(argptr);
  return ret;
#endif
}
/* 29.08.2000 SVS $ */
/* SVS $ */


/* $ 07.09.2000 SVS
   �������� FarBsearch ��� �������� (������� bsearch)
*/
void *WINAPI FarBsearch(const void *key, const void *base, size_t nelem, size_t width, int (__cdecl *fcmp)(const void *, const void *))
{
  if(key && fcmp && base)
    return bsearch(key,base,nelem,width,fcmp);
  return NULL;
}
/* SVS $ */

void __cdecl  xf_free(void *__block)
{
  free(__block);
}

void *__cdecl xf_malloc(size_t __size)
{
  void *Ptr=malloc(__size);
  return Ptr;
}

void *__cdecl xf_realloc(void *__block, size_t __size)
{
  void *Ptr=realloc(__block,__size);
  return Ptr;
}

/* $ 26.01.2003 IS
     + FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
       �� ������ CreateFile
*/
// ������� ����, ��� �������� ���������� CreateFile
HANDLE WINAPI FAR_CreateFile(
    LPCTSTR lpFileName,     // pointer to name of the file
    DWORD dwDesiredAccess,  // access (read-write) mode
    DWORD dwShareMode,      // share mode
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // pointer to security attributes
    DWORD dwCreationDistribution, // how to create
    DWORD dwFlagsAndAttributes,   // file attributes
    HANDLE hTemplateFile          // handle to file with attributes to copy
   )
{
  HANDLE hFile=CreateFile(lpFileName,dwDesiredAccess,dwShareMode,
    lpSecurityAttributes, dwCreationDistribution,dwFlagsAndAttributes,
    hTemplateFile);
  return hFile;
}
/* IS $ */
