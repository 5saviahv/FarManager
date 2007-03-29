/*
FileMasksWithExclude.cpp

����� ��� ������ �� �������� ������� ������ (����������� ������� �����
����������).
*/

#include "headers.hpp"
#pragma hdrstop

#include "FileMasksWithExclude.hpp"
#include "fn.hpp"


FileMasksWithExclude::FileMasksWithExclude():BaseFileMask()
{
}

void FileMasksWithExclude::Free()
{
    Include.Free();
    Exclude.Free();
}

/*
 �������������� ������ �����. ��������� ������, ����������� �������.
 ���������� FALSE ��� ������� (��������, ���� ��
 ����� ����� �� ����� ����� 0)
*/

BOOL FileMasksWithExclude::Set(const wchar_t *masks, DWORD Flags)
{
  Free();
  if(NULL==masks || !*masks) return FALSE;

  size_t len=wcslen(masks)+2;
  BOOL rc=FALSE;
  wchar_t *MasksStr=(wchar_t *) xf_malloc(len*sizeof (wchar_t));
  if(MasksStr)
  {
     rc=TRUE;
     wcscpy(MasksStr, masks);
     wchar_t *pExclude=wcschr(MasksStr,EXCLUDEMASKSEPARATOR);
     if(pExclude)
     {
       *pExclude=0;
       ++pExclude;
       if(wcschr(pExclude, EXCLUDEMASKSEPARATOR)) rc=FALSE;
     }

     if(rc)
     {
        rc=Include.Set(*MasksStr?MasksStr:L"*",
                       (Flags&FMPF_ADDASTERISK)?FMPF_ADDASTERISK:0);
        if(rc) rc=Exclude.Set(pExclude, 0);
     }
  }

  if(!rc)
    Free();

  if(MasksStr)
    xf_free (MasksStr);

  return rc;
}

/* �������� ��� ����� �� ������� �����
   ���������� TRUE � ������ ������.
   ���� � ����� � FileName �� ������������ */
BOOL FileMasksWithExclude::Compare(const wchar_t *FileName)
{
   return (Include.Compare(FileName) && !Exclude.Compare(FileName));
}

BOOL FileMasksWithExclude::IsEmpty(void)
{
  return Include.IsEmpty() && Exclude.IsEmpty();
}
