/*
FileMasksProcessor.cpp

����� ��� ࠡ��� � ����묨 ��᪠�� 䠩��� (�� ���뢠���� ����稥 ��᮪
�᪫�祭��).
*/

/* Revision: 1.01 10.07.2001 $ */

/*
Modify:
  10.07.2001 SVS
    ! � ��� ��� CPP-䠩��� if/endif
  01.07.2001 IS
    + ����� � ���
*/
#include "headers.hpp"
#pragma hdrstop

#include "FileMasksProcessor.hpp"
#include "fn.hpp"

FileMasksProcessor::FileMasksProcessor():BaseFileMask()
{
}

void FileMasksProcessor::Free()
{
    Masks.Free();
}

/*
 ���樠������� ᯨ᮪ ��᮪. �ਭ����� ᯨ᮪, ࠧ�������� ����⮩.
 �����頥� FALSE �� ��㤠� (���ਬ��, ���� ��
 ����� ����� �� ��᮪ ࠢ�� 0)
*/

BOOL FileMasksProcessor::Set(const char *masks, DWORD Flags)
{
  // ࠧ����⥫�� ��᮪ ���� �� ⮫쪮 ������, �� � �窠 � ����⮩!
  Masks.SetSeparators(',',';',TRUE, (Flags&FMPF_ADDASTERISK)?TRUE:FALSE);
  return Masks.Set(masks);
}

BOOL FileMasksProcessor::IsEmpty(void)
{
  Masks.Start();
  return Masks.IsEmpty();
}

/* �ࠢ���� ��� 䠩�� � ᯨ᪮� ��᮪
   �����頥� TRUE � ��砥 �ᯥ�.
   ���� � 䠩�� � FileName �� ���������� */
BOOL FileMasksProcessor::Compare(const char *FileName)
{
  Masks.Start();
  while(NULL!=(MaskPtr=Masks.GetNext()))
  {
    if (CmpName(MaskPtr,FileName, FALSE))
    // SkipPath=FALSE, �.�. � CFileMask ��뢠���� PointToName
       return TRUE;
  }
  return FALSE;
}
