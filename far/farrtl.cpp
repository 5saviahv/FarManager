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

#define MEM_DELTA	1023

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

#if 0 //���� ��� �⮣� ��ࠡ�⠥�, ��� ������ �� �� �������� ��稭��
void *operator new(size_t sz)
 {
  extern new_handler _new_handler;
  void *p;
  sz=sz?sz:1;
  while((p=malloc(sz))==NULL)
   {
    if(_new_handler!=NULL)_new_handler();
    else break;
   }
  return p;
 }
void operator delete(void *v)
 {
  if(v)free(v);
 }
void *operator new[](size_t sz)
 {
  extern new_handler _new_handler;
  void *p;
  sz=sz?sz:1;
  while((p=malloc(sz))==NULL)
   {
    if(_new_handler!=NULL)_new_handler();
    else break;
   }
  return p;
 }
void operator delete[](void *v)
 {
  if(v)free(v);
 }
#endif
