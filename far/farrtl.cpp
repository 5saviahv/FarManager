/*
farrtl.cpp

��८�।������ �㭪権 ࠡ��� � �������: new/delete/malloc/realloc/free
*/

/* Revision: 1.02 05.07.2000 $ */

/*
Modify:
  03.07.2000 IS
    ! ����祭�� ᥣ� 䠩�� � �஥��
  04.07.2000 SVS
    ! �뤥����� � ����⢥ ᠬ������筮�� �����!
  05.07.2000 IS
    ! ������� ���� �஢�ப, ���� ������ ����ᠫ ��, �� �� �� ࠢ��
      ������, �᫨ ��८�।����� new/delete :-(((
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

static HANDLE FARHeapForNew=NULL;

void *malloc(size_t size)
{
  void *p;
  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();
  if(FARHeapForNew) p=HeapAlloc(FARHeapForNew,HEAP_ZERO_MEMORY,size);
//  if(p==NULL)MessageBox(NULL,"NULL","p",0);
  return p;
}

void *realloc(void *block, size_t size)
{
  void *p;
  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();
  if(FARHeapForNew)
   {
    if (block) p=HeapReAlloc(FARHeapForNew,HEAP_ZERO_MEMORY,block,size);
    else p=HeapAlloc(FARHeapForNew,HEAP_ZERO_MEMORY, size);
   }
//  if(p==NULL)MessageBox(NULL,"NULL","p",0);
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
