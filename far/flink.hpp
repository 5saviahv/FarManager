#ifndef __FLINK_HPP__
#define __FLINK_HPP__

/*
flink.hpp

������������ ���� ��� ������ � Hard & SymLink

*/

/* Revision: 1.11 16.10.2001 $ */

/*
Modify:
  16.10.2001 SVS
    + EnumNTFSStreams() - �������� ���������� � �������
    ! ������� const-�������������
  01.10.2001 SVS
    ! FarGetRepasePointInfo -> FarGetRepa_R_sePointInfo
  24.09.2001 SVS
    + FarGetRepasePointInfo - �������� ������ GetJunctionPointInfo() ���
      ��������.
  25.06.2001 IS
   ! ��������� const
  30.05.2001 SVS
    + FarMkLink()
  29.05.2001 SVS
    + GetPathRootOne()
  28.04.2001 VVM
    + GetSubstName �������� ��� ��������
  25.04.2001 SVS
    + CreateVolumeMountPoint() - ������������ ����� �� �������� �������
  06.04.2001 SVS
    + CanCreateHardLinks() - �������� �� ��������.
  13.03.2001 SVS
    ! GetPathRoot ��������� � fn.hpp :-)
  05.01.2001 SVS
    + ������� DelSubstDrive - �������� Subst ��������
    + ������� GetSubstName ��������� �� fh.hpp
  04.01.2001 SVS
    + ������.
    + �������� MkLink, GetNumberOfLinks ��������� �� fn.hpp
*/

int   WINAPI MkLink(char *Src,char *Dest);
int   WINAPI FarMkLink(char *Src,char *Dest,DWORD Flags);
BOOL  WINAPI CanCreateHardLinks(const char *TargetFile,const char *HardLinkName);
int   WINAPI GetNumberOfLinks(const char *Name);
int   WINAPI CreateVolumeMountPoint(LPCTSTR SrcVolume,LPCTSTR LinkFolder);
BOOL  WINAPI CreateJunctionPoint(LPCTSTR szMountDir, LPCTSTR szDestDir);
BOOL  WINAPI DeleteJunctionPoint(LPCTSTR szMountDir);
DWORD WINAPI GetJunctionPointInfo(LPCTSTR szMountDir,
              LPTSTR  szDestBuff,
              DWORD   dwBuffSize);
int   WINAPI FarGetReparsePointInfo(const char *Src,char *Dest,int DestSize);

BOOL GetSubstName(int DriveType,char *LocalName,char *SubstName,int SubstSize);
int DelSubstDrive(char *DosDeviceName);
void  WINAPI GetPathRoot(const char *Path,char *Root);
void GetPathRootOne(const char *Path,char *Root);

// ������������� ��� EnumNTFSStreams
// � ��������� sid ���� cStreamName �� ���������, �.�. ������� ��� ������
//    ���������� � ��������� StreamName
typedef BOOL (WINAPI *ENUMFILESTREAMS)(int Idx,const WCHAR *StreamName,const WIN32_STREAM_ID *sid);
int WINAPI EnumNTFSStreams(const char *FileName,ENUMFILESTREAMS fpEnum,__int64 *SizeStreams);

#endif // __FLINK_HPP__
