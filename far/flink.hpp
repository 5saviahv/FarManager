#ifndef __FLINK_HPP__
#define __FLINK_HPP__

/*
flink.hpp

���������� 䠩� ��� ࠡ��� � Hard & SymLink

*/

/* Revision: 1.02 13.03.2001 $ */

/*
Modify:
  13.03.2001 SVS
    ! GetPathRoot ��॥堫� � fn.hpp :-)
  05.01.2001 SVS
    + �㭪�� DelSubstDrive - 㤠����� Subst �ࠩ���
    + �㭪�� GetSubstName ��॥堫� �� fh.hpp
  04.01.2001 SVS
    + ������.
    + ���ᠭ�� MkLink, GetNumberOfLinks ��॥堫� �� fn.hpp

*/

int   WINAPI MkLink(char *Src,char *Dest);
int   WINAPI GetNumberOfLinks(char *Name);
BOOL  WINAPI CreateJunctionPoint(LPCTSTR szMountDir, LPCTSTR szDestDir);
BOOL  WINAPI DeleteJunctionPoint(LPCTSTR szMountDir);
DWORD WINAPI GetJunctionPointInfo(LPCTSTR szMountDir,
              LPTSTR  szDestBuff,
              DWORD   dwBuffSize);

BOOL GetSubstName(char *LocalName,char *SubstName,int SubstSize);
int DelSubstDrive(char *DosDeviceName);
void  WINAPI GetPathRoot(char *Path,char *Root);

#endif // __FLINK_HPP__
