/*
FileMasksProcessor.cpp

����� ��� ࠡ��� � ����묨 ��᪠�� 䠩��� (�� ���뢠���� ����稥 ��᮪
�᪫�祭��).
*/

/* Revision: 1.02 11.08.2001 $ */

/*
Modify:
  11.08.2001 IS
    + �ᯮ��㥬 UDL_FLAGS:
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
  DWORD flags=ULF_PACKASTERISKS|ULF_PROCESSBRACKETS;
  if(Flags&FMPF_ADDASTERISK) flags|=ULF_ADDASTERISK;
  Masks.SetParameters(',',';',flags);
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
