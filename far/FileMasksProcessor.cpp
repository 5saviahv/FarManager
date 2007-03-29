/*
FileMasksProcessor.cpp

����� ��� ������ � �������� ������� ������ (�� ����������� ������� �����
����������).
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

BOOL FileMasksProcessor::Set(const wchar_t *masks, DWORD Flags)
{
  // ������������ ����� �������� �� ������ �������, �� � ����� � �������!
  DWORD flags=ULF_PACKASTERISKS|ULF_PROCESSBRACKETS|ULF_SORT|ULF_UNIQUE;
  if(Flags&FMPF_ADDASTERISK) flags|=ULF_ADDASTERISK;
  Masks.SetParameters(L',',L';',flags);
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
BOOL FileMasksProcessor::Compare(const wchar_t *FileName)
{
  Masks.Reset();
  while(NULL!=(MaskPtr=Masks.GetNext()))
  {
    if (CmpNameW(MaskPtr,FileName, FALSE))
    // SkipPath=FALSE, �.�. � CFileMask ���������� PointToName
       return TRUE;
  }
  return FALSE;
}
