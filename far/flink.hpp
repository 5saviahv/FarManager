#ifndef __FLINK_HPP__
#define __FLINK_HPP__

/*
flink.hpp

���������� 䠩� ��� ࠡ��� � Hard & SymLink

*/

/* Revision: 1.05 28.04.2001 $ */

/*
Modify:
  28.04.2001 VVM
    + GetSubstName ����砥� ⨯ ���⥫�
  25.04.2001 SVS
    + CreateVolumeMountPoint() - ����஢���� ��᪠ �� 䠩����� ��⥬�
  06.04.2001 SVS
    + CanCreateHardLinks() - �஢�ઠ �� �訢����.
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
BOOL  WINAPI CanCreateHardLinks(char *TargetFile,char *HardLinkName);
int   WINAPI GetNumberOfLinks(char *Name);
int   WINAPI CreateVolumeMountPoint(LPCTSTR SrcVolume,LPCTSTR LinkFolder);
BOOL  WINAPI CreateJunctionPoint(LPCTSTR szMountDir, LPCTSTR szDestDir);
BOOL  WINAPI DeleteJunctionPoint(LPCTSTR szMountDir);
DWORD WINAPI GetJunctionPointInfo(LPCTSTR szMountDir,
              LPTSTR  szDestBuff,
              DWORD   dwBuffSize);

BOOL GetSubstName(int DriveType,char *LocalName,char *SubstName,int SubstSize);
int DelSubstDrive(char *DosDeviceName);
void  WINAPI GetPathRoot(char *Path,char *Root);

#endif // __FLINK_HPP__
