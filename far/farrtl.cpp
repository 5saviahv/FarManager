/*
farrtl.cpp

��८�।������ �㭪権 ࠡ��� � �������: new/delete/malloc/realloc/free
*/

/* Revision: 1.01 04.07.2000 $ */

/*
Modify:
  03.07.2000 IS
    ! ����祭�� ᥣ� 䠩�� � �஥��
  04.07.2000 SVS
    ! �뤥����� � ����⢥ ᠬ������筮�� �����!
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
  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();
  return HeapAlloc(FARHeapForNew?FARHeapForNew:GetProcessHeap(),HEAP_ZERO_MEMORY,size);
}

void *realloc(void *block, size_t size)
{
  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();
  if (block) return HeapReAlloc(FARHeapForNew?FARHeapForNew:GetProcessHeap(),HEAP_ZERO_MEMORY,block,size);
  else return HeapAlloc(FARHeapForNew?FARHeapForNew:GetProcessHeap(),HEAP_ZERO_MEMORY, size);
}

void free(void *block)
{
  if(!FARHeapForNew) FARHeapForNew=GetProcessHeap();
  HeapFree(FARHeapForNew?FARHeapForNew:GetProcessHeap(),0,block);
}

void *operator new(size_t sz) {return malloc(sz);}
void operator delete(void *v) {free(v);}
void *operator new[](size_t sz) {return malloc(sz);}
void operator delete[](void *v) {free(v);}
