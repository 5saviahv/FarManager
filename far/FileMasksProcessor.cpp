/*
FileMasksProcessor.cpp

����� ��� ������ � �������� ������� ������ (�� ����������� ������� �����
����������).
*/

/* Revision: 1.03 15.08.2002 $ */

/*
Modify:
  15.08.2002 IS
    ! Masks.Start -> Masks.Reset
    + ����� ������: ����� �������������� ��������� � ������� ���������
  11.08.2001 IS
    + ���������� UDL_FLAGS:
  10.07.2001 SVS
    ! � ���� ��� CPP-������ if/endif
  01.07.2001 IS
    + ������� � �����
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
 �������������� ������ �����. ��������� ������, ����������� �������.
 ���������� FALSE ��� ������� (��������, ���� ��
 ����� ����� �� ����� ����� 0)
*/

BOOL FileMasksProcessor::Set(const char *masks, DWORD Flags)
{
  // ������������ ����� �������� �� ������ �������, �� � ����� � �������!
  DWORD flags=ULF_PACKASTERISKS|ULF_PROCESSBRACKETS|ULF_SORT|ULF_UNIQUE;
  if(Flags&FMPF_ADDASTERISK) flags|=ULF_ADDASTERISK;
  Masks.SetParameters(',',';',flags);
  return Masks.Set(masks);
}

BOOL FileMasksProcessor::IsEmpty(void)
{
  Masks.Reset();
  return Masks.IsEmpty();
}

/* �������� ��� ����� �� ������� �����
   ���������� TRUE � ������ ������.
   ���� � ����� � FileName �� ������������ */
BOOL FileMasksProcessor::Compare(const char *FileName)
{
  Masks.Reset();
  while(NULL!=(MaskPtr=Masks.GetNext()))
  {
    if (CmpName(MaskPtr,FileName, FALSE))
    // SkipPath=FALSE, �.�. � CFileMask ���������� PointToName
       return TRUE;
  }
  return FALSE;
}
