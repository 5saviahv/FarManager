#ifndef __FileMasksWithExclude_CPP
#define __FileMasksWithExclude_CPP

/*
FileMasksWithExclude.cpp

����� ��� ࠡ��� � ᫮��묨 ��᪠�� 䠩��� (���뢠���� ����稥 ��᮪
�᪫�祭��).
*/

/* Revision: 1.00 01.07.2001 $ */

/*
Modify:
  01.07.2001 IS
    + ����� � ���
*/

#include "headers.hpp"
#pragma hdrstop

#include "FileMasksWithExclude.hpp"

FileMasksWithExclude::FileMasksWithExclude():BaseFileMask()
{
}

void FileMasksWithExclude::Free()
{
    Include.Set(NULL, 0);
    Exclude.Set(NULL, 0);
}

/*
 ���樠������� ᯨ᮪ ��᮪. �ਭ����� ᯨ᮪, ࠧ�������� ����⮩.
 �����頥� FALSE �� ��㤠� (���ਬ��, ���� ��
 ����� ����� �� ��᮪ ࠢ�� 0)
*/

BOOL FileMasksWithExclude::Set(const char *masks, DWORD /*Flags*/)
{
  Free();
  if(NULL==masks || !*masks) return FALSE;

  int len=strlen(masks)+2, rc=FALSE;
  char *MasksStr=(char *) malloc(len);
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
        rc=Include.Set(*MasksStr?MasksStr:"*", 0);
        if(rc) rc=Exclude.Set(pExclude, 0);
     }
  }

  if(!rc)
    Free();

  if(MasksStr)
    free (MasksStr);

  return rc;
}

/* �ࠢ���� ��� 䠩�� � ᯨ᪮� ��᮪
   �����頥� TRUE � ��砥 �ᯥ�.
   ���� � 䠩�� � FileName �� ���������� */
BOOL FileMasksWithExclude::Compare(const char *FileName)
{
   return (Include.Compare(FileName) && !Exclude.Compare(FileName));
}

BOOL FileMasksWithExclude::IsEmpty(void)
{
  return Include.IsEmpty() && Exclude.IsEmpty();
}

#endif // __FileMasksWithExclude_CPP
