/*
farwinapi.cpp

������� ������ ��������� WinAPI �������

*/

/* Revision: 1.02 01.03.2004 $ */

/*
Modify:
  01.03.2004 SVS
    + ������� FAR_OemTo* � FAR_CharTo* ������ WinAPI
    + FAR_ANSI - ������ �� ������ (�� ������ ������ �������������� ����������)
      �� "������" ������� �� ������, ���� �����...
  09.10.2003 SVS
    + SetFileApisTo() � ���������� APIS2ANSI ��� APIS2OEM ������ SetFileApisToANSI() � SetFileApisToOEM()
  01.06.2003 SVS
    ! ��������� � �������� ���������������� ������
    ! FAR_DeleteFile � FAR_RemoveDirectory ��������� �� delete.cpp � farwinapi.cpp
    ! FAR_CreateFile �������� �� farrtl.cpp � farwinapi.cpp
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "fn.hpp"

/* $ 26.01.2003 IS
    + FAR_DeleteFile ������ DeleteFile, FAR_RemoveDirectory ������
      RemoveDirectory, ������� � ������ �� ������������ ��� ��������
      �������������� ������ � ���������.
*/
// ������� ����, ��� �������� ���������� DeleteFile
BOOL WINAPI FAR_DeleteFile(const char *FileName)
{
  // IS: ������� ��������� ������� ����������� ��������, �����
  // IS: �� ������������ ������ ������������
  BOOL rc=DeleteFile(FileName);
  if(!rc) // IS: ��� ��� ������ ������������ � ������...
  {
    SetLastError((_localLastError = GetLastError()));
    if(CheckErrorForProcessed(_localLastError))
    {
      char FullName[NM*2]="\\\\?\\";
      // IS: +4 - ����� �� �������� ���� "\\?\"
      if(ConvertNameToFull(FileName, FullName+4, sizeof(FullName)-4) < sizeof(FullName)-4)
      {
        // IS: ��������, � ����� ��� ���� ���� ������ ������� � ����
        if( (FullName[4]=='/' && FullName[5]=='/') ||
            (FullName[4]=='\\' && FullName[5]=='\\') )
          rc=DeleteFile(FullName+4);
        // IS: ������ �������� � ���� ���, ������� ���������� ����
        else
          rc=DeleteFile(FullName);
      }
    }
  }
  return rc;
}

// ������� �������, ��� �������� ���������� RemoveDirectory
BOOL WINAPI FAR_RemoveDirectory(const char *DirName)
{
  // IS: ������� ��������� ������� ����������� ��������, �����
  // IS: �� ������������ ������ ������������
  BOOL rc=RemoveDirectory(DirName);
  if(!rc) // IS: ��� ��� ������ ������������ � ������...
  {
    SetLastError((_localLastError = GetLastError()));
    if(CheckErrorForProcessed(_localLastError))
    {
      char FullName[NM+16]="\\\\?\\";
      // IS: +4 - ����� �� �������� ���� "\\?\"
      if(ConvertNameToFull(DirName, FullName+4, sizeof(FullName)-4) < sizeof(FullName)-4)
      {
        // IS: ��������, � ����� ��� ���� ���� ������ ������� � ����
        if( (FullName[4]=='/' && FullName[5]=='/') ||
            (FullName[4]=='\\' && FullName[5]=='\\') )
          rc=RemoveDirectory(FullName+4);
        // IS: ������ �������� � ���� ���, ������� ���������� ����
        else
          rc=RemoveDirectory(FullName);
      }
    }
  }
  return rc;
}
/* IS $ */

/* $ 26.01.2003 IS
     + FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
       �� ������ CreateFile
*/
// ������� ����, ��� �������� ���������� CreateFile
HANDLE WINAPI FAR_CreateFile(
    LPCTSTR lpFileName,     // pointer to name of the file
    DWORD dwDesiredAccess,  // access (read-write) mode
    DWORD dwShareMode,      // share mode
    LPSECURITY_ATTRIBUTES lpSecurityAttributes, // pointer to security attributes
    DWORD dwCreationDistribution, // how to create
    DWORD dwFlagsAndAttributes,   // file attributes
    HANDLE hTemplateFile          // handle to file with attributes to copy
   )
{
  HANDLE hFile=CreateFile(lpFileName,dwDesiredAccess,dwShareMode,
    lpSecurityAttributes, dwCreationDistribution,dwFlagsAndAttributes,
    hTemplateFile);
  return hFile;
}
/* IS $ */

void WINAPI SetFileApisTo(int Type)
{
  switch(Type)
  {
    case APIS2OEM:
      SetFileApisToOEM();
      break;
    case APIS2ANSI:
      SetFileApisToANSI();
      break;
  }
}

BOOL WINAPI FAR_OemToCharBuff(LPCSTR lpszSrc,LPTSTR lpszDst,DWORD cchDstLength)
{
#if defined(FAR_ANSI)
  if(Opt.FarAnsi)
  {
    if(lpszDst != lpszSrc)
      memmove(lpszDst,lpszSrc,cchDstLength);
    return TRUE;
  }
#endif
  return OemToCharBuff(lpszSrc,lpszDst,cchDstLength);
}

BOOL WINAPI FAR_CharToOemBuff(LPCSTR lpszSrc,LPTSTR lpszDst,DWORD cchDstLength)
{
#if defined(FAR_ANSI)
  if(Opt.FarAnsi)
  {
    if(lpszDst != lpszSrc)
      memmove(lpszDst,lpszSrc,cchDstLength);
    return TRUE;
  }
#endif
  return CharToOemBuff(lpszSrc,lpszDst,cchDstLength);
}


BOOL WINAPI FAR_OemToChar(LPCSTR lpszSrc,LPTSTR lpszDst)
{
#if defined(FAR_ANSI)
  if(Opt.FarAnsi)
  {
    if(lpszDst != lpszSrc)
      memmove(lpszDst,lpszSrc,strlen(lpszSrc)+1);
    return TRUE;
  }
#endif
  return OemToChar(lpszSrc,lpszDst);
}

BOOL WINAPI FAR_CharToOem(LPCSTR lpszSrc,LPTSTR lpszDst)
{
#if defined(FAR_ANSI)
  if(Opt.FarAnsi)
  {
    if(lpszDst != lpszSrc)
      memmove(lpszDst,lpszSrc,strlen(lpszSrc)+1);
    return TRUE;
  }
#endif
  return CharToOem(lpszSrc,lpszDst);
}
