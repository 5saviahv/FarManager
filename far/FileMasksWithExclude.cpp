/*
FileMasksWithExclude.cpp

����� ��� ������ �� �������� ������� ������ (����������� ������� �����
����������).
*/

/* Revision: 1.03 21.01.2003 $ */

/*
Modify:
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  10.07.2001 SVS
    ! � ���� ��� CPP-������ if/endif
  02.07.2001 IS
    ! ������������� ���, ��� �������� FileMasksProcessor.Free()
  01.07.2001 IS
    + ������� � �����
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

BOOL FileMasksWithExclude::Set(const char *masks, DWORD Flags)
{
  Free();
  if(NULL==masks || !*masks) return FALSE;

  int len=strlen(masks)+2, rc=FALSE;
  char *MasksStr=(char *) xf_malloc(len);
  if(MasksStr)
  {
     rc=TRUE;
     strcpy(MasksStr, masks);
     char *pExclude=strchr(MasksStr,EXCLUDEMASKSEPARATOR);
     if(pExclude)
     {
       *pExclude=0;
       ++pExclude;
       if(strchr(pExclude, EXCLUDEMASKSEPARATOR)) rc=FALSE;
     }

     if(rc)
     {
        rc=Include.Set(*MasksStr?MasksStr:"*",
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
BOOL FileMasksWithExclude::Compare(const char *FileName)
{
   return (Include.Compare(FileName) && !Exclude.Compare(FileName));
}

BOOL FileMasksWithExclude::IsEmpty(void)
{
  return Include.IsEmpty() && Exclude.IsEmpty();
}
