/*
farrtl.cpp

��������������� ������� ������ � �������: new/delete/malloc/realloc/free
*/

/* Revision: 1.10 22.02.2002 $ */

/*
Modify:
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

#if defined(__BORLANDC__)
#ifdef ALLOC

#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned size_t;

void *malloc(size_t size);
void *realloc(void *block, size_t size);
void *calloc(size_t nitems, size_t size);
void free(void *block);

#ifdef __cplusplus
}
#endif

#define MEM_DELTA	4095

static HANDLE FARHeapForNew=NULL;

void *calloc(size_t nitems, size_t size)
{
  return realloc(NULL,size*nitems);
}


void *malloc(size_t size)
{
  return realloc(NULL,size);
}


void *realloc(void *block, size_t size)
{
  void *p=NULL;
  int size2;

  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();

  if(FARHeapForNew)
  {
    if(!size)
    {
      if(block)
        HeapFree(FARHeapForNew,0,block);
    }
    else if (!block) // � ������ ��� ���� �������, �������
    {
                     // ������. ���-�� ������ ������� ������� ��� ��������� ������
      p=HeapAlloc(FARHeapForNew,HEAP_ZERO_MEMORY, size);
    }
    else
    {
      p=block;
      // Required size, alligned to MEM_DELTA
      size  = (size + MEM_DELTA) & (~MEM_DELTA);
      // Current allocated size
      size2 = HeapSize(FARHeapForNew,0,block);
      if (size  > size2 ||          // ��������� ������, ��� ������� ��������
          size2 - size > MEM_DELTA) // ���� ���������� ���� �������� � MEM_DELTA ��� ������
      {
        p=HeapReAlloc(FARHeapForNew,HEAP_ZERO_MEMORY,block,size);
      }
      // else
      /*
          ������. ������ � ���� ��� �������� � ������� ��� � �� �� ������� ���.
          � ��������� �������, ��� ��������� ���������...
      */
    }
  }
  return p;
}

void free(void *block)
{
 if(block!=NULL)
 {
  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();
  if(FARHeapForNew) HeapFree(FARHeapForNew,0,block);
 }
}

#ifdef CMEM_INCLUDE
#include "cmem.cpp"
#else //� _����_ - ��������, �� ����������� ��� ����...

#if 1
/*
    ! ����������!!! �����, rtfm - ������ forever :-)))
      ������ � ���� �������� �� C++:
         ftp://ftp.ldz.lv/pub/doc/ansi_iso_iec_14882_1998.pdf (������ 2860601)
      � ��� ��� ������ �� ������... ������, ������������� � new/delete ��� ����
      (���� ���� ������, �� ���� �������� �� ����������, �.�. �� ��� ����������
      ��������), � ������� ������, ��� � _����_ ������ �� ���������! ����
      ��������� �����������, �������� � ��������� ��������� �������� 18.4
*/
void *operator new(size_t size)
 {
  extern new_handler _new_handler;
  void *p=NULL;
  size=size?size:1;
  while((p=malloc(size))==NULL)
   {
    if(_new_handler!=NULL)_new_handler();
    else break;
   }
  return p;
 }
void *operator new[](size_t size) {return ::operator new(size);}
void *operator new(size_t size, void *p) {return p;}
void operator delete(void *p) {free(p);}
void operator delete[](void *ptr) {::operator delete(ptr);}
#endif

#endif // CMEM_INCLUDE

#endif // ALLOC
#endif // defined(__BORLANDC__)



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

_CRTIMP __int64 __cdecl _lseeki64(int, __int64, int);

__int64 ftell64(FILE *fp)
{
  return _lseeki64(fileno(fp), 0i64, SEEK_CUR );
}

int fseek64 (FILE *fp, __int64 offset, int whence)
{
  return (int)_lseeki64(fileno(fp),offset,whence);
}

#endif
/* SVS $*/
