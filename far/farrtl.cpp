#if defined(__BORLANDC__)
#ifdef ALLOC
/*
farrtl.cpp

��८�।������ �㭪権 ࠡ��� � �������: new/delete/malloc/realloc/free
*/

/* Revision: 1.03 11.07.2000 $ */

/*
Modify:
  03.07.2000 IS
    ! ����祭�� ᥣ� 䠩�� � �஥��
  04.07.2000 SVS
    ! �뤥����� � ����⢥ ᠬ������筮�� �����!
  05.07.2000 IS
    ! ������� ���� �஢�ப, ���� ������ ����ᠫ ��, �� �� �� ࠢ��
      ������, �᫨ ��८�।����� new/delete :-(((
  11.07.2000 SVS
    ! ����� ࠧ㬭�� (� ����ᮬ) ��।������ �����.
  12.07.2000 SVS
    ! �����祭�� MEM_DELTA �� 4095
    + ����祭�� �����஢ new/delete by IS
    + ����祭�� cmem � ����⢥ ᬮ�५�� �����.
*/

#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned size_t;

void *malloc(size_t size);
void *realloc(void *block, size_t size);
void free(void *block);

#ifdef __cplusplus
}
#endif

#include "headers.hpp"
#pragma hdrstop


#define MEM_DELTA	4095

static HANDLE FARHeapForNew=NULL;

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
      HeapFree(FARHeapForNew,0,block);
    else if (!block) // � ���� ࠧ ���� �⮫쪮, ᪮�쪮
                     // �����. ��-�� ���뫮 ��譥�� ��室� �� �����筮� �맮��
      p=HeapAlloc(FARHeapForNew,HEAP_ZERO_MEMORY, size);
    else
    {
      p=block;
      // Required size, alligned to MEM_DELTA
      size  = (size + MEM_DELTA) & (~MEM_DELTA);
      // Current allocated size
      size2 = HeapSize(FARHeapForNew,0,block);
      if (size  > size2 ||          // ����ᨫ� �����, 祬 ॠ�쭮 �뤥����
          size2 - size > MEM_DELTA) // ���� �᢮������ ���� ࠧ��஬ � MEM_DELTA ��� �����
        p=HeapReAlloc(FARHeapForNew,HEAP_ZERO_MEMORY,block,size);
      // else
      /*
          ��祣�. ������ � 娯� 㦥 �뤥���� � ���� ࠧ � �� �� �ண��� ���.
          � �ணࠬ�� ��⠥�, �� �ந��諨 ���������...
      */
    }
  }
  return p;
}

void free(void *block)
{
  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();
  if(FARHeapForNew) HeapFree(FARHeapForNew,0,block);
}

#ifdef CMEM_INCLUDE
#include "cmem.cpp"
#else //� _����_ - ࠡ�⠥�, �� ���஢��� �� ����...

#if 1
/*
    ! ����訫���!!! ��த, rtfm - �㫥�� forever :-)))
      ���砫 � ⠪� �⠭���� �� C++:
         ftp://ftp.ldz.lv/pub/doc/ansi_iso_iec_14882_1998.pdf (ࠧ��� 2860601)
      � ⠬ �� ��� �� ������... ����, ��८�।���� � new/delete ��� ����
      (�᫨ ���� ���, � ���� ᯮᮡ�� �� �����⢨�, �.�. �� ��� �᪫�祭��
      ࠡ�⠥�), � �ࠩ��� ��砥, �� � _����_ ����� �� ��堥���! ����
      ������ ���஡����, ᬮ��� � 㪠������ �⠭���� ��ࠣ�� 18.4
*/
void *operator new(size_t size)
 {
  extern new_handler _new_handler;
  void *p;
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
void operator delete(void *p) {if(p)free(p);}
void operator delete[](void *ptr) {::operator delete(ptr);}
#endif

#endif // CMEM_INCLUDE

#endif // ALLOC
#endif // defined(__BORLANDC__)

