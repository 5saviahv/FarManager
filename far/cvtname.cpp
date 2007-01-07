/*
cvtname.cpp

������� ��� �������������� ���� ������/�����.

*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "lang.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"


int ConvertNameToFullW (
        const wchar_t *lpwszSrc,
        string &strDest
        )
{
	string strSrc = lpwszSrc; //����������� � ������ ���������� �� ������ dest == src

	lpwszSrc = strSrc;

    int Result = wcslen (lpwszSrc);

    const wchar_t *lpwszName = PointToNameW (lpwszSrc);

    if ( (lpwszName == lpwszSrc) &&
         (lpwszName[0] != L'.' || lpwszName[1] != 0) )
    {
        FarGetCurDirW (strDest);
        AddEndSlashW (strDest);

        strDest += lpwszSrc;

        return strDest.GetLength ();
    }

    if ( PathMayBeAbsoluteW (lpwszSrc) )
    {
        if ( *lpwszName &&
            (*lpwszName != L'.' || lpwszName[1] != 0 && (lpwszName[1] != L'.' || lpwszName[2] != 0) ) &&
            (wcsstr (lpwszSrc, L"\\..\\") == NULL && wcsstr (lpwszSrc, L"\\.\\") == NULL) )
        {
            strDest = lpwszSrc;

            return strDest.GetLength ();
        }
    }

    int nLength = GetFullPathNameW (lpwszSrc, 0, NULL, NULL)+1;

    wchar_t *lpwszDest = strDest.GetBuffer (nLength);
    GetFullPathNameW (lpwszSrc, nLength, lpwszDest, NULL);


    // ��� ����� ����� � ������ cd //host/share
    // � ������ ����� �� ���� c:\\host\share

    if ( lpwszSrc[0] == L'/' &&
         lpwszSrc[1] == L'/' &&
         lpwszDest[1] == L':' &&
         lpwszDest[3] == L'\\' )
         memmove (lpwszDest, lpwszDest+2, (wcslen (lpwszDest+2)+1)*sizeof (wchar_t));

    strDest.ReleaseBuffer (nLength);

    return strDest.GetLength ();
}

/*
  ����������� Src � ������ �������� ���� � ������ reparse point � Win2K
  ���� OS ����, �� ���������� ������� ConvertNameToFull()
*/
int WINAPI ConvertNameToRealW (const wchar_t *Src, string &strDest)
{
  string strTempDest;
  BOOL IsAddEndSlash=FALSE; // =TRUE, ���� ���� ��������� ��������������
                            // � ����� �� ��� ����... ������.

  // ������� ������� ������ ���� �� ������� ������� ��������
  int Ret=ConvertNameToFullW(Src, strTempDest);
  //RawConvertShortNameToLongName(TempDest,TempDest,sizeof(TempDest));
  _SVS(SysLog(L"ConvertNameToFull('%S') -> '%S'",Src,(const wchar_t*)strTempDest));

  wchar_t *TempDest;
  /* $ 14.06.2003 IS
     ��� ����������� ������ ���� � �� �������� ������������� ��������
  */
  // ��������� �������� Win2K, �.�. � ������ ���� ������ ���� ���������
  // �������, ����������� ������ �������� ��� �����.
  // ����� ������ �� ������ ��� ����������� ������, �.�. ��� ��� ���������� ������
  // ���������� ���������� ��� ������, �� ������� ��������� ������� (�.�. ����������
  // "������������ �������")
  if (IsLocalDriveW(strTempDest) &&
      WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5)
  {
    DWORD FileAttr;

    if((FileAttr=GetFileAttributesW(strTempDest)) != -1 && (FileAttr&FILE_ATTRIBUTE_DIRECTORY))
    {
      AddEndSlashW (strTempDest);
      IsAddEndSlash=TRUE;
    }

    TempDest = strTempDest.GetBuffer (2048); //BUGBUGBUG!!!!
    wchar_t *Ptr, Chr;

    Ptr = TempDest+wcslen(TempDest);

    const wchar_t *CtrlChar = TempDest;

    if (wcslen(TempDest) > 2 && TempDest[0]==L'\\' && TempDest[1]==L'\\')
      CtrlChar= wcschr(TempDest+2, L'\\');

    // ������� ���� ������� ����� �� �����
    while(CtrlChar)
    {
      while(Ptr > TempDest && *Ptr != L'\\')
        --Ptr;
      /* $ 07.01.2003 IS
         - ������: �����-�� ������������ ���� "�����:" - �� �����
           �������� �������� �� ����� "�����", ��� ����� �
           ��������������� �����������
      */
      // ���� ��� UNC, �� �������� �� ����� �������, �� ������...
      if(*Ptr != L'\\' || Ptr == CtrlChar
        // ���� ����� �� "�����:", �� ���� �����������
        || *(Ptr-1)==L':')
        break;
      /* IS $ */

      Chr=*Ptr;
      *Ptr=0;
      FileAttr=GetFileAttributesW(TempDest);
      // �! ��� ��� ������ - ���� �� "���������" ���� - �������
      if(FileAttr != (DWORD)-1 && (FileAttr&FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT)
      {
        string strTempDest2;

//        if(CheckParseJunction(TempDest,sizeof(TempDest)))
        {
          // ������� ���� ��������
          if(GetJunctionPointInfoW(TempDest, strTempDest2))
          {
            strTempDest.LShift (4); //???
            // ��� ������ �������������� ����� (�� �������� �����)...
            if(!wcsncmp(strTempDest2,L"Volume{",7))
            {
              string strJuncRoot;
              // ������� ���� ����� �����, ����...
              GetPathRootOneW(strTempDest2, strJuncRoot);
              // ...�� � ����� ������ ����� ���������.
              strTempDest2 = strJuncRoot;
            }

            *Ptr=Chr; // ����������� ������
            DeleteEndSlashW(strTempDest2);
            strTempDest2 = Ptr;
            wcscpy(TempDest,strTempDest2); //BUGBUG
            Ret=wcslen(TempDest);
            // ���. �������� ���� � ��� � �������...
            break;
          }
        }
      }
      *Ptr=Chr;
      --Ptr;
    }

    strTempDest.ReleaseBuffer ();
  }

  TempDest = strTempDest.GetBuffer ();

  if(IsAddEndSlash) // ���� �� ������� - ������.
    TempDest[wcslen(TempDest)-1]=0;

  strTempDest.ReleaseBuffer ();

  strDest = strTempDest;

  return Ret;
}


void ConvertNameToShortW(const wchar_t *Src, string &strDest)
{
	string strCopy = NullToEmptyW(Src);

	int nSize = GetShortPathNameW (strCopy, NULL, 0);

	if ( nSize )
	{
		wchar_t *lpwszDest = strDest.GetBuffer (nSize);

		GetShortPathNameW (strCopy, lpwszDest, nSize);

		strDest.ReleaseBuffer ();
	}
	else
		strDest = strCopy;

	strDest.Upper ();
}

void ConvertNameToLongW(const wchar_t *Src, string &strDest)
{
	string strCopy = NullToEmptyW(Src);

	int nSize = GetLongPathNameW (strCopy, NULL, 0);

	if ( nSize )
	{
		wchar_t *lpwszDest = strDest.GetBuffer (nSize);

		GetLongPathNameW (strCopy, lpwszDest, nSize);

		strDest.ReleaseBuffer ();
	}
	else
		strDest = strCopy;
}
