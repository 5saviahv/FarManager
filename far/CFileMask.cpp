/*
CFileMask.cpp

�᭮���� ����� ��� ࠡ��� � ��᪠�� 䠩���. �ᯮ�짮���� �㦭� ������ ���.

*/

/* Revision: 1.02 10.07.2001 $ */

/*
Modify:
  10.07.2001 SVS
    ! � ��� ��� CPP-䠩��� if/endif
  02.07.2001 IS
    + ��ࠡ�⪠ FMF_ADDASTERISK
  01.07.2001 IS
    + ����� � ���
*/

#include "headers.hpp"
#pragma hdrstop

#include "CFileMask.hpp"
#include "FileMasksProcessor.hpp"
#include "FileMasksWithExclude.hpp"
#include "fn.hpp"
#include "lang.hpp"

const EXCLUDEMASKSEPARATOR=0x7C; // '|'

CFileMask::CFileMask()
{
    FileMask=NULL;
}

void CFileMask::Free()
{
    if(FileMask)
       delete FileMask;
    FileMask=NULL;
}

/*
 ���樠������� ᯨ᮪ ��᮪. �ਭ����� ᯨ᮪, ࠧ�������� ����⮩ ��� �窮�
 � ����⮩. ����蠥��� 㪠�뢠�� ��᪨ �᪫�祭��, �⤥��� �� �� �᭮����
 ᨬ����� '|' �����頥� FALSE �� ��㤠� (���ਬ��, ���� �� ����� ����� ��
 ��᮪ ࠢ�� 0).
*/

BOOL CFileMask::Set(const char *Masks, DWORD Flags)
{
  Free();
  BOOL rc=FALSE;
  int Silent=Flags & FMF_SILENT;
  DWORD flags=0;
  if(Flags & FMF_ADDASTERISK) flags|=FMPF_ADDASTERISK;
  if (Masks && *Masks)
  {
    if(strchr(Masks, EXCLUDEMASKSEPARATOR))
    {
      if(!(Flags&FMF_FORBIDEXCLUDE))
        FileMask=new FileMasksWithExclude;
    }
    else
      FileMask=new FileMasksProcessor;

    if(FileMask)
       rc=FileMask->Set(Masks, flags);

    if(!rc)
      Free();
  }

  if(!Silent && !rc)
    Message(MSG_DOWN|MSG_WARNING,1,MSG(MWarning),MSG(MIncorrectMask), MSG(MOk));

  return rc;
}

// �����頥� TRUE, �᫨ ᯨ᮪ ��᮪ ���⮩
BOOL CFileMask::IsEmpty(void)
{
  return FileMask?FileMask->IsEmpty():TRUE;
}

/* �ࠢ���� ��� 䠩�� � ᯨ᪮� ��᮪
   �����頥� TRUE � ��砥 �ᯥ�.
   ���� � ����� 䠩�� ����������.
*/
BOOL CFileMask::Compare(const char *FileName)
{
  return FileMask?FileMask->Compare(PointToName((char*)FileName)):FALSE;
}
