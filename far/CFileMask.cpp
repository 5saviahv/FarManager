/*
CFileMask.cpp

�������� ����� ��� ������ � ������� ������. ������������ ����� ������ ���.

*/

#include "headers.hpp"
#pragma hdrstop

#include "CFileMask.hpp"
#include "FileMasksProcessor.hpp"
#include "FileMasksWithExclude.hpp"
#include "fn.hpp"
#include "lang.hpp"

const int EXCLUDEMASKSEPARATOR=0x7C; // '|'

CFileMask::CFileMask()
{
	FileMask=NULL;
}

void CFileMask::Free()
{
	if (FileMask)
		delete FileMask;

	FileMask=NULL;
}

/*
 �������������� ������ �����. ��������� ������, ����������� ������� ��� ������
 � �������. ����������� ��������� ����� ����������, ������� �� �� ��������
 �������� '|' ���������� FALSE ��� ������� (��������, ���� �� ����� ����� ��
 ����� ����� 0).
*/

BOOL CFileMask::Set(const char *Masks, DWORD Flags)
{
	Free();
	BOOL rc=FALSE;
	int Silent=Flags & FMF_SILENT;
	DWORD flags=0;

	if (Flags & FMF_ADDASTERISK) flags|=FMPF_ADDASTERISK;

	if (Masks && *Masks)
	{
		if (strchr(Masks, EXCLUDEMASKSEPARATOR))
		{
			if (!(Flags&FMF_FORBIDEXCLUDE))
				FileMask=new FileMasksWithExclude;
		}
		else
			FileMask=new FileMasksProcessor;

		if (FileMask)
			rc=FileMask->Set(Masks, flags);

		if (!rc)
			Free();
	}

	if (!Silent && !rc)
		Message(MSG_DOWN|MSG_WARNING,1,MSG(MWarning),MSG(MIncorrectMask), MSG(MOk));

	return rc;
}

// ���������� TRUE, ���� ������ ����� ������
BOOL CFileMask::IsEmpty(void)
{
	return FileMask?FileMask->IsEmpty():TRUE;
}

/* �������� ��� ����� �� ������� �����
   ���������� TRUE � ������ ������.
   ���� � ����� ����� ������������.
*/
BOOL CFileMask::Compare(const char *FileName)
{
	return FileMask?FileMask->Compare(PointToName((char*)FileName)):FALSE;
}
