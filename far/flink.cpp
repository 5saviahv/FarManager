/*
flink.cpp

���� ������ ������� �� ��������� Link`�� - Hard&Sym

*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "copy.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"

//#if defined(__BORLANDC__)
// current thread's ANSI code page
#define CP_THREAD_ACP             3

#if !defined(__GNUC__)

#define FILE_ANY_ACCESS                 0
#define FILE_SPECIAL_ACCESS    (FILE_ANY_ACCESS)

#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE      ( 16 * 1024 )
// Predefined reparse tags.
// These tags need to avoid conflicting with IO_REMOUNT defined in ntos\inc\io.h
#define IO_REPARSE_TAG_RESERVED_ZERO             (0)
#define IO_REPARSE_TAG_RESERVED_ONE              (1)
#ifndef IO_REPARSE_TAG_MOUNT_POINT
#define IO_REPARSE_TAG_MOUNT_POINT               (0xA0000003)
#endif
#ifndef IO_REPARSE_TAG_SYMLINK
#define IO_REPARSE_TAG_SYMLINK                   (0xA000000C)
#endif

// The value of the following constant needs to satisfy the following conditions:
//  (1) Be at least as large as the largest of the reserved tags.
//  (2) Be strictly smaller than all the tags in use.
#define IO_REPARSE_TAG_RESERVED_RANGE            IO_REPARSE_TAG_RESERVED_ONE
// The following constant represents the bits that are valid to use in
// reparse tags.
#define IO_REPARSE_TAG_VALID_VALUES     (0xE000FFFF)
// Macro to determine whether a reparse tag is a valid tag.
#define IsReparseTagValid(_tag) (                               \
                  !((_tag) & ~IO_REPARSE_TAG_VALID_VALUES) &&   \
                  ((_tag) > IO_REPARSE_TAG_RESERVED_RANGE)      \
                 )
#define FILE_FLAG_OPEN_REPARSE_POINT    0x00200000
#define FSCTL_SET_REPARSE_POINT         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 41, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) // REPARSE_DATA_BUFFER,
#define FSCTL_GET_REPARSE_POINT         CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 42, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define FSCTL_DELETE_REPARSE_POINT      CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 43, METHOD_BUFFERED, FILE_SPECIAL_ACCESS) // REPARSE_DATA_BUFFER,

#endif //!defined(__GNUC__)

//#ifndef _MSC_VER
#ifndef REPARSE_GUID_DATA_BUFFER_HEADER_SIZE
//   For non-Microsoft reparse point drivers, a fixed header must be
//used, with the remainder of the reparse point tag belonging to the
//driver itself. The format for this information is:
typedef struct _REPARSE_GUID_DATA_BUFFER {
  DWORD  ReparseTag;
  WORD   ReparseDataLength;
  WORD   Reserved;
/*
#define IO_REPARSE_TAG_IFSTEST_CONGRUENT        (0x00000009L)
#define IO_REPARSE_TAG_ARKIVIO                  (0x0000000CL)
#define IO_REPARSE_TAG_SOLUTIONSOFT             (0x2000000DL)
#define IO_REPARSE_TAG_COMMVAULT                (0x0000000EL)
*/
  GUID   ReparseGuid;
  struct {
      BYTE   DataBuffer[1];
  } GenericReparseBuffer;
} REPARSE_GUID_DATA_BUFFER, *PREPARSE_GUID_DATA_BUFFER;

#define REPARSE_GUID_DATA_BUFFER_HEADER_SIZE   FIELD_OFFSET(REPARSE_GUID_DATA_BUFFER, GenericReparseBuffer)
#define MAXIMUM_REPARSE_DATA_BUFFER_SIZE      ( 16 * 1024 )
// REPARSE_DATA_BUFFER
//#endif
#endif // REPARSE_GUID_DATA_BUFFER_HEADER_SIZE

/*
// http://www.osr.com/ntinsider/2003/reparse.htm
// The format for Microsoft-defined reparse points (from ntifs.h) is:

typedef struct _REPARSE_DATA_BUFFER {
    ULONG  ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
    union {
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR PathBuffer[1];
        } SymbolicLinkReparseBuffer;
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            WCHAR PathBuffer[1];
        } MountPointReparseBuffer;
        struct {
            UCHAR  DataBuffer[1];
        } GenericReparseBuffer;
    };
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

*/
struct TMN_REPARSE_DATA_BUFFER
{
  DWORD  ReparseTag;
  WORD   ReparseDataLength;
  WORD   Reserved;
  union {
    struct {
      WORD   SubstituteNameOffset;
      WORD   SubstituteNameLength;
      WORD   PrintNameOffset;
      WORD   PrintNameLength;
      ULONG  Flags;
      WCHAR PathBuffer[1];
    } SymbolicLinkReparseBuffer;
    struct {
      WORD   SubstituteNameOffset;
      WORD   SubstituteNameLength;
      WORD   PrintNameOffset;
      WORD   PrintNameLength;
      WCHAR PathBuffer[1];
    } MountPointReparseBuffer;
    struct {
      BYTE   DataBuffer[1];
    } GenericReparseBuffer;
  };
};

typedef BOOL (WINAPI *PDELETEVOLUMEMOUNTPOINT)(
          LPCTSTR lpszVolumeMountPoint);  // volume mount point path

typedef BOOL (WINAPI *PGETVOLUMENAMEFORVOLUMEMOUNTPOINT)(
          LPCTSTR lpszVolumeMountPoint, // volume mount point or directory
          LPTSTR lpszVolumeName,        // volume name buffer
          DWORD cchBufferLength);       // size of volume name buffer

typedef BOOL (WINAPI *PSETVOLUMEMOUNTPOINT)(
          LPCTSTR lpszVolumeMountPoint, // mount point
          LPCTSTR lpszVolumeName);        // volume to be mounted

static PGETVOLUMENAMEFORVOLUMEMOUNTPOINT pGetVolumeNameForVolumeMountPoint=NULL;
static PDELETEVOLUMEMOUNTPOINT pDeleteVolumeMountPoint=NULL;
static PSETVOLUMEMOUNTPOINT pSetVolumeMountPoint=NULL;


/*
 SrcVolume
   Pointer to a string that indicates the volume mount point where the
   volume is to be mounted. This may be a root directory (X:\) or a
   directory on a volume (X:\mnt\). The string must end with a trailing
   backslash ('\').
*/
int WINAPI CreateVolumeMountPoint(LPCTSTR SrcVolume,LPCTSTR LinkFolder)
{
   char Buf[1024];            // temporary buffer for volume name

   if(!pGetVolumeNameForVolumeMountPoint)
      pGetVolumeNameForVolumeMountPoint=(PGETVOLUMENAMEFORVOLUMEMOUNTPOINT)GetProcAddress(GetModuleHandle("KERNEL32"),"GetVolumeNameForVolumeMountPointA");
   if(!pSetVolumeMountPoint)
      pSetVolumeMountPoint=(PSETVOLUMEMOUNTPOINT)GetProcAddress(GetModuleHandle("KERNEL32"),"SetVolumeMountPointA");
   if(!pGetVolumeNameForVolumeMountPoint || !pSetVolumeMountPoint)
     return(3);

   // We should do some error checking on the inputs. Make sure
   // there are colons and backslashes in the right places, etc.
   if(pGetVolumeNameForVolumeMountPoint(
                SrcVolume, // input volume mount point or directory
                      Buf, // output volume name buffer
               sizeof(Buf) // size of volume name buffer
           ) != TRUE)
   {
      // printf( "Retrieving volume name for %s failed.\n", SrcVolume);
      return (1);
   }
   if(!pSetVolumeMountPoint(LinkFolder, // mount point
                               Buf)) // volume to be mounted
   {
     //printf ("Attempt to mount %s at %s failed.\n", SrcVolume, LinkFolder);
     return (2);
   }
   return (0);
}

BOOL WINAPI CreateJunctionPoint(LPCTSTR SrcFolder,LPCTSTR LinkFolder)
{
  if (!LinkFolder || !SrcFolder || !*LinkFolder || !*SrcFolder)
    return FALSE;

//_SVS(SysLog("SrcFolder ='%s'",SrcFolder));
//_SVS(SysLog("LinkFolder='%s'",LinkFolder));

  char szDestDir[1024];
  if (SrcFolder[0] == '\\' && SrcFolder[1] == '?')
    xstrncpy(szDestDir, SrcFolder,sizeof(szDestDir)-1);
  else
  {
    LPTSTR pFilePart;
    char szFullDir[1024];

    strcpy(szDestDir, "\\??\\");
    if (!GetFullPathName(SrcFolder, sizeof(szFullDir), szFullDir, &pFilePart) ||
      GetFileAttributes(szFullDir) == -1)
    {
      SetLastError(ERROR_PATH_NOT_FOUND);
      return FALSE;
    }

    char *PtrFullDir=szFullDir;
    // �������� �� subst
    if(IsLocalPath(szFullDir))
    {
      char LocalName[8], SubstName[NM];
      sprintf(LocalName,"%c:",*szFullDir);
      if(GetSubstName(DRIVE_NOT_INIT,LocalName,SubstName,sizeof(SubstName)))
      {
        strcat(szDestDir, SubstName);
        AddEndSlash(szDestDir);
        PtrFullDir=szFullDir+3;
      }
    }
    else if(IsLocalPrefixPath(szFullDir) || IsLocalVolumePath(szFullDir))
      PtrFullDir=szFullDir+4;
    strcat(szDestDir, PtrFullDir);
  }

  char szBuff[MAXIMUM_REPARSE_DATA_BUFFER_SIZE] = { 0 };
  TMN_REPARSE_DATA_BUFFER& rdb = *(TMN_REPARSE_DATA_BUFFER*)szBuff;

  int cchDest = (int)strlen(szDestDir) + 1;
  if (cchDest > 512) {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    return FALSE;
  }

  FAR_OemToChar(szDestDir,szDestDir); // !!!
  wchar_t wszDestMountPoint[512];
  if (!MultiByteToWideChar(CP_THREAD_ACP,           // code page
                           MB_PRECOMPOSED,          // character-type options
                           szDestDir,               // string to map
                           cchDest,                 // number of bytes in string
                           wszDestMountPoint,       // wide-character buffer
                           cchDest))                // size of buffer

  {
    return FALSE;
  }

  //_SVS(SysLog("szDestDir ='%s'",szDestDir));
  int nDestMountPointBytes    = (int)wcslen(wszDestMountPoint) * 2;
  rdb.ReparseTag              = IO_REPARSE_TAG_MOUNT_POINT;
  rdb.ReparseDataLength       = nDestMountPointBytes + 12;
  rdb.Reserved                = 0;
  rdb.MountPointReparseBuffer.SubstituteNameOffset = 0;
  rdb.MountPointReparseBuffer.SubstituteNameLength = nDestMountPointBytes;
  rdb.MountPointReparseBuffer.PrintNameOffset      = nDestMountPointBytes + 2;
  rdb.MountPointReparseBuffer.PrintNameLength      = 0;
  wcscpy(rdb.MountPointReparseBuffer.PathBuffer, wszDestMountPoint);
  //_SVS(SysLogDump("rdb",0,szBuff,MAXIMUM_REPARSE_DATA_BUFFER_SIZE/3,0));


  HANDLE hDir=FAR_CreateFile(LinkFolder,GENERIC_WRITE|GENERIC_READ,0,0,OPEN_EXISTING,
          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,0);

  if (hDir == INVALID_HANDLE_VALUE)
  {
    SetLastError(ERROR_PATH_NOT_FOUND);
    return FALSE;
  }
  DWORD dwBytes;
#define TMN_REPARSE_DATA_BUFFER_HEADER_SIZE \
      FIELD_OFFSET(TMN_REPARSE_DATA_BUFFER, MountPointReparseBuffer)
  if(!DeviceIoControl(hDir,
            FSCTL_SET_REPARSE_POINT,
            (LPVOID)&rdb,
            rdb.ReparseDataLength + TMN_REPARSE_DATA_BUFFER_HEADER_SIZE,
            NULL,
            0,
            &dwBytes,
            0))
  {
    DWORD LastErr=GetLastError();
    CloseHandle(hDir);
    FAR_DeleteFile(LinkFolder); // � ����� �� �������, ����� ������� �������, �� ������� �� ������� ???
    SetLastError(LastErr);
    return 0;
  }
  CloseHandle(hDir);
  return TRUE;
}

BOOL WINAPI DeleteJunctionPoint(LPCTSTR szDir)
{
  HANDLE hDir=FAR_CreateFile(szDir,
          GENERIC_READ | GENERIC_WRITE,
          0,
          0,
          OPEN_EXISTING,
          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,
          0);
  if (hDir == INVALID_HANDLE_VALUE)
  {
    SetLastError(ERROR_PATH_NOT_FOUND);
    return FALSE;
  }

  REPARSE_GUID_DATA_BUFFER rgdb = { 0 };
  rgdb.ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
  DWORD dwBytes;
  const BOOL bOK =
    DeviceIoControl(hDir,
            FSCTL_DELETE_REPARSE_POINT,
            &rgdb,
            REPARSE_GUID_DATA_BUFFER_HEADER_SIZE,
            NULL,
            0,
            &dwBytes,
            0);
  CloseHandle(hDir);
  return bOK != 0;
}

DWORD WINAPI GetJunctionPointInfo(LPCTSTR szMountDir,
              LPTSTR  szDestBuff,
              DWORD   dwBuffSize)
{
  const DWORD FileAttr = GetFileAttributes(szMountDir);
  /* $ 14.06.2003 IS
     ��� ����������� ������ �������� ���������� ���������� � �����
     �� �������������� ���������
  */
  if (FileAttr == INVALID_FILE_ATTRIBUTES || !(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT)
      || !IsLocalDrive(szMountDir))
  /* IS $ */
  {
    SetLastError(ERROR_PATH_NOT_FOUND);
    return 0;
  }

  HANDLE hDir=FAR_CreateFile(szMountDir,GENERIC_READ|0,0,0,OPEN_EXISTING,
          FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT,0);

  if (hDir == INVALID_HANDLE_VALUE)
  {
    SetLastError(ERROR_PATH_NOT_FOUND);
    return 0;
  }

  char szBuff[MAXIMUM_REPARSE_DATA_BUFFER_SIZE];
  TMN_REPARSE_DATA_BUFFER& rdb = *(TMN_REPARSE_DATA_BUFFER*)szBuff;

  DWORD dwBytesReturned;
  if (!DeviceIoControl(hDir,
            FSCTL_GET_REPARSE_POINT,
            NULL,
            0,
            (LPVOID)&rdb,
            MAXIMUM_REPARSE_DATA_BUFFER_SIZE,
            &dwBytesReturned,
            0) ||
    !IsReparseTagValid(rdb.ReparseTag))
  {
    CloseHandle(hDir);
    return 0;
  }

  CloseHandle(hDir);

  WORD SubstituteNameLength;
  WCHAR *PathBuffer;

  if (rdb.ReparseTag == IO_REPARSE_TAG_SYMLINK)
  {
    SubstituteNameLength = rdb.SymbolicLinkReparseBuffer.SubstituteNameLength/sizeof(WCHAR);
    PathBuffer = &rdb.SymbolicLinkReparseBuffer.PathBuffer[rdb.SymbolicLinkReparseBuffer.SubstituteNameOffset/sizeof(WCHAR)];
  }
  else
  {
    SubstituteNameLength = rdb.MountPointReparseBuffer.SubstituteNameLength/sizeof(WCHAR);
    PathBuffer = &rdb.MountPointReparseBuffer.PathBuffer[rdb.MountPointReparseBuffer.SubstituteNameOffset/sizeof(WCHAR)];
  }

  if (dwBuffSize < SubstituteNameLength + sizeof(TCHAR))
  {
    return SubstituteNameLength + sizeof(TCHAR);
  }

  if (!WideCharToMultiByte(CP_THREAD_ACP,
              0,
              PathBuffer,
              SubstituteNameLength,
              szDestBuff,
              dwBuffSize,
              "",
              FALSE))
  {
    //printf("WideCharToMultiByte failed (%d)\n", GetLastError());
    return 0;
  }
  szDestBuff[SubstituteNameLength] = 0;
  FAR_CharToOemBuff(szDestBuff,szDestBuff,dwBuffSize); // !!!
  return SubstituteNameLength;
}

/* $ 14.06.2003 IS
   ������� ��������� - �������� �� ���� ���������,
   ���������� 0 ��� ����������� ������.
   ���� ��� ����� �����, �� ���� ������� �����������.
   ������� �������, ������ �� ���� ������� ��� �������������.
*/
int IsLocalDrive(const char *Path)
{
  // ���������� �������� ������ ����
  char RootDir[MAX_PATH]="A:\\";
  DWORD DriveType = 0;

  if(IsLocalPath(Path))
  {
    RootDir[0] = Path[0];
    DriveType = FAR_GetDriveType(RootDir);
  }
  else
  {
    if(ConvertNameToFull(Path, RootDir, sizeof(RootDir)) >= sizeof(RootDir))
      return 0; // �������� ������� ������� ����, ����� �������, ��� ���� �����������!
    if(IsLocalPath(RootDir))
    {
      RootDir[3] = 0;
      DriveType = FAR_GetDriveType(RootDir);
    }
    else if(IsLocalPrefixPath(RootDir))
    {
      RootDir[7] = 0;
      DriveType = FAR_GetDriveType(RootDir);
    }
    else if(IsLocalVolumePath(RootDir))
    {
      RootDir[49] = 0;
      DriveType = FAR_GetDriveType(RootDir);
    }
  }

  return (DriveType == DRIVE_REMOVABLE || DriveType == DRIVE_FIXED ||
    IsDriveTypeCDROM(DriveType) || DriveType == DRIVE_RAMDISK);
}
/* IS $ */

/* $ 07.09.2000 SVS
   ������� GetNumberOfLinks ���� �������� �������� :-)
*/
int WINAPI GetNumberOfLinks(const char *Name)
{
  HANDLE hFile=FAR_CreateFile(Name,0,FILE_SHARE_READ|FILE_SHARE_WRITE,
                          NULL,OPEN_EXISTING,0,NULL);
  if (hFile==INVALID_HANDLE_VALUE)
    return(1);
  BY_HANDLE_FILE_INFORMATION bhfi;
  int GetCode=GetFileInformationByHandle(hFile,&bhfi);
  CloseHandle(hFile);
  return(GetCode ? bhfi.nNumberOfLinks:0);
}
/* SVS $*/


#if defined(__BORLANDC__)
#pragma option -a4
#endif
int WINAPI MkLink(const char *Src,const char *Dest)
{
  char FileSource[NM],FileDest[NM];

//  ConvertNameToFull(Src,FileSource, sizeof(FileSource));
  if (ConvertNameToFull(Src,FileSource, sizeof(FileSource)) >= sizeof(FileSource)){
    return FALSE;
  }
//  ConvertNameToFull(Dest,FileDest, sizeof(FileDest));
  if (ConvertNameToFull(Dest,FileDest, sizeof(FileDest)) >= sizeof(FileDest)){
    return FALSE;
  }

  BOOL bSuccess=FALSE;

  // ���� ����� ��� Win2K
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT &&
     WinVer.dwMajorVersion >= 5
     //&& FAR_GetDriveType(FileSource) == DRIVE_FIXED
    )
  {
    typedef BOOL (WINAPI *PCREATEHARDLINK)(
       LPCTSTR lpFileName,                         // new file name
       LPCTSTR lpExistingFileName,                 // extant file name
       LPSECURITY_ATTRIBUTES lpSecurityAttributes  // SD
    );
    static PCREATEHARDLINK PCreateHardLinkA=NULL;
    if(!PCreateHardLinkA)
      PCreateHardLinkA=(PCREATEHARDLINK)GetProcAddress(GetModuleHandle("KERNEL32"),"CreateHardLinkA");
    if(PCreateHardLinkA)
    {
      bSuccess=PCreateHardLinkA(FileDest, FileSource, NULL) != 0;
    }
  }

  if(bSuccess)
    return bSuccess;

  // ��� ��� ���� �������� � NT4/2000
  struct CORRECTED_WIN32_STREAM_ID
  {
    DWORD          dwStreamId ;
    DWORD          dwStreamAttributes ;
    LARGE_INTEGER  Size ;
    DWORD          dwStreamNameSize ;
    WCHAR          cStreamName[ ANYSIZE_ARRAY ] ;
  } StreamId;

  WCHAR FileLink[NM];

  HANDLE hFileSource;

  DWORD dwBytesWritten;
  LPVOID lpContext;
  DWORD cbPathLen;
  DWORD StreamSize;

  MultiByteToWideChar(CP_OEMCP,0,FileDest,-1,FileLink,sizeof(FileLink)/sizeof(FileLink[0]));

  hFileSource = FAR_CreateFile(FileSource,FILE_WRITE_ATTRIBUTES,
                FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);

  if(hFileSource == INVALID_HANDLE_VALUE)
    return(FALSE);

  lpContext = NULL;
  cbPathLen = (lstrlenW(FileLink) + 1) * sizeof(WCHAR);

  StreamId.dwStreamId = BACKUP_LINK;
  StreamId.dwStreamAttributes = 0;
  StreamId.dwStreamNameSize = 0;
  StreamId.Size.u.HighPart = 0;
  StreamId.Size.u.LowPart = cbPathLen;

  StreamSize=sizeof(StreamId)-sizeof(WCHAR **)+StreamId.dwStreamNameSize;

  bSuccess = BackupWrite(hFileSource,(LPBYTE)&StreamId,StreamSize,
             &dwBytesWritten,FALSE,FALSE,&lpContext);

  int LastError=0;

  if (bSuccess)
  {
    bSuccess = BackupWrite(hFileSource,(LPBYTE)FileLink,cbPathLen,
                &dwBytesWritten,FALSE,FALSE,&lpContext);
    if (!bSuccess)
      LastError=GetLastError();

    BackupWrite(hFileSource,NULL,0,&dwBytesWritten,TRUE,FALSE,&lpContext);
  }
  else
    LastError=GetLastError();

  CloseHandle(hFileSource);

  if (LastError)
    SetLastError(LastError);

  return(bSuccess);
}

/*
  ������� EnumNTFSStreams ��������� �������� ���������� �������,
  ����������� � ����� FileName.
  ���������:
   FileName - ��� �����
   fpEnum   - �������, ���������� 2 ��������� - ����� ������ (Idx) � ���
              ������ � Unicode. ������������ �����������, ���� �������
              ������ FALSE. ���� �������� ����� ���� ����� NULL.
              ���������� ������� CALLBACK-�������������:

              BOOL WINAPI EnumFileStreams(int Idx,WCHAR *StreamName,const WIN32_STREAM_ID *sid)
              {
                char Buf[260];
                UnicodeToANSI(StreamName,Buf,sizeof(Buf));
                printf("%2d) '%s' StreamId=%d",Idx,Buf,sid->dwStreamId);
                switch(sid->dwStreamId)
                {
                  case BACKUP_DATA: printf(" (Standard data)\n");break;
                  case BACKUP_EA_DATA: printf(" (Extended attribute data)\n");break;
                  case BACKUP_SECURITY_DATA: printf(" (Windows NT security descriptor data)\n");break;
                  case BACKUP_ALTERNATE_DATA: printf(" (Alternative data streams)\n");break;
                  case BACKUP_LINK: printf(" (Hard link information)\n");break;
                }
                return TRUE;
              }
   SizeStreams - ��������� �� ���������� ���� __int64, � ������� ����� �������
                 ����� ������ ���� ������� (��������� ��� ������!).
                 �������� ��������������.

  Return: ���������� ������� � �����.
          -1 - ������ �������� �����
           0 - FileName - ������� � � ��� ��� ��� �������, �� ���������
               �������� �� ����� ������� (����� ����� ���� - {Data})
           1 - ���� ����� �������� ����� ������...
          >1 - ... � ��������� ��������������.
*/
int WINAPI EnumNTFSStreams(const char *FileName,ENUMFILESTREAMS fpEnum,__int64 *SizeStreams)
{
  int StreamsCount=-1;

  HANDLE hFile = FAR_CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                     OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
  if (hFile != INVALID_HANDLE_VALUE)
  {
    // Prepare for execution
    LPVOID lpContext = NULL;
    WIN32_STREAM_ID sid;
    DWORD dwRead;
    WCHAR wszStreamName[MAX_PATH];

    StreamsCount=0;

    // Enumerate the streams...
    BOOL bContinue = TRUE;
    while (bContinue)
    {
      // Calculate the size of the variable length
      // WIN32_STREAM_ID structure
      memset(&sid,0,sizeof(WIN32_STREAM_ID));
      memset(wszStreamName,0,MAX_PATH);
      DWORD dwStreamHeaderSize = (DWORD)((LPBYTE)&sid.cStreamName -
           (LPBYTE)&sid+ sid.dwStreamNameSize);

      bContinue = BackupRead(hFile, (LPBYTE) &sid,
        dwStreamHeaderSize, &dwRead, FALSE, FALSE,
        &lpContext);

      if (!dwRead)
        break;
      if (dwRead != dwStreamHeaderSize)
        break;
      // At this point, we've read the header of the i.th
      // stream. What follows is the name of the stream and
      // next its content.

      // Read the stream name
      BackupRead(hFile,(LPBYTE)wszStreamName,sid.dwStreamNameSize,&dwRead,FALSE,FALSE,&lpContext);
      if (dwRead != sid.dwStreamNameSize)
        break;

      // A stream name is stored enclosed in a pair of colons
      // plus a $DATA default trailer. If the stream name is
      // VersionInfo it's stored (and retrieved) as:
      // :VersionInfo:$DATA
      if (wcslen(wszStreamName))
      {
        WCHAR Wsz[MAX_PATH], *pwsz=Wsz;
        lstrcpyW(pwsz, wszStreamName + sizeof(CHAR));
        LPWSTR wp = wcsstr(pwsz, L":");
        pwsz[wp-pwsz] = 0;
        lstrcpyW(wszStreamName, pwsz);
      }

      StreamsCount++;
      if(SizeStreams)
        *SizeStreams+=sid.Size.QuadPart;
      if(fpEnum && !fpEnum(StreamsCount,wszStreamName,&sid))
        break;

      // Skip the stream body
      DWORD dw1, dw2;
      if(!BackupSeek(hFile, sid.Size.u.LowPart, sid.Size.u.HighPart,&dw1, &dw2, &lpContext))
         break;
    }

    CloseHandle(hFile);
  }
  return StreamsCount;
}
#if defined(__BORLANDC__)
#pragma option -a.
#endif

/* $ 05.01.2001 SVS
   ������� DelSubstDrive - �������� Subst ��������
   Return: -1 - ��� ���� �� SUBST-�������, ���� OS �� ��.
            0 - ��� ������� �� ���
            1 - ������ ��� ��������.
*/
int DelSubstDrive(char *DosDeviceName)
{
  char NtDeviceName[512]="\\??\\";
  int Pos=(WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT?4:0);
  if(GetSubstName(DRIVE_NOT_INIT, DosDeviceName,
                  NtDeviceName+Pos, sizeof(NtDeviceName)-Pos))
  {
    return !DefineDosDevice(DDD_RAW_TARGET_PATH|
                       DDD_REMOVE_DEFINITION|
                       DDD_EXACT_MATCH_ON_REMOVE,
                       DosDeviceName, NtDeviceName)?1:0;
  }
  return(-1);
}
/* SVS $ */

BOOL GetSubstName(int DriveType,char *LocalName,char *SubstName,int SubstSize)
{
  /* $28.04.2001 VVM
    + ��������� � ����������� �� Opt.SubstNameRule
      ������� �����:
      0 - ���� ����������, �� ���������� ������� �����
      1 - ���� ����������, �� ���������� ��� ��������� */
  if (DriveType!=DRIVE_NOT_INIT)
  {
    int DriveRemovable = (DriveType==DRIVE_REMOVABLE) || (DriveType==DRIVE_CDROM);
    if ((!(Opt.SubstNameRule & 1)) && (DriveRemovable))
      return(FALSE);
    if ((!(Opt.SubstNameRule & 2)) && (!DriveRemovable))
      return(FALSE);
  }
  /* VVM $ */
  char Name[NM*2]="";
  LocalName=CharUpper((LPTSTR)LocalName);
  if ((LocalName[0]>='A') && ((LocalName[0]<='Z')))
  {
    // ��� �����������, ����� � WIN98 �������� �� �����!!!!
/*$ 01.02.2001 skv
  ����� ���������� � ��� ��++ ������.
*/
#if defined(_MSC_VER) || defined(__GNUC__)
    int SizeName=WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT?sizeof(Name):_MAX_PATH;
#else
    int SizeName=WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT?sizeof(Name):MAXPATH;
#endif
    /* skv$*/

    if (QueryDosDevice(LocalName,Name,SizeName) >= 3)
    {
      /* Subst drive format API differences:
       *   WinNT: \??\qualified_path (e.g. \??\C:\WinNT)
       *   Win98: qualified_path (e.g. C:\ or C:\Win98) */
      if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
      {
        if (!strncmp(Name,"\\??\\",4))
        {
          if(SubstName)
            xstrncpy(SubstName,Name+4,SubstSize-1);
          return TRUE;
        }
      }
      else
      {
        if(Name[1] == ':' && Name[2] == '\\')
        {
          if(SubstName)
          {
            xstrncpy(SubstName,Name,SubstSize-1);
            FAR_OemToChar(SubstName,SubstName); // Mantis#224
          }
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

// �������� ����� ������� :-)
void GetPathRootOne(const char *Path,char *Root)
{
  char TempRoot[2048],*ChPtr;
  xstrncpy(TempRoot,Path,NM-1);

  if (WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5)
  {
    if(!pGetVolumeNameForVolumeMountPoint)
      // �������� ������ ��� Win2000!
      pGetVolumeNameForVolumeMountPoint=(PGETVOLUMENAMEFORVOLUMEMOUNTPOINT)GetProcAddress(GetModuleHandle("KERNEL32"),"GetVolumeNameForVolumeMountPointA");

    // ��������� mounted volume
    if(pGetVolumeNameForVolumeMountPoint && !strncmp(Path,"Volume{",7))
    {
      char Drive[] = "A:\\"; // \\?\Volume{...
      int I;
      for (I = 'A'; I <= 'Z';  I++ )
      {
        Drive[0] = (char)I;
        if(pGetVolumeNameForVolumeMountPoint(
                  Drive, // input volume mount point or directory
                  TempRoot, // output volume name buffer
                  sizeof(TempRoot)) &&       // size of volume name buffer
           !LocalStricmp(TempRoot+4,Path))
        {
           strcpy(Root,Drive);
           return;
        }
      }
      // Ops. ���� �� �� ����� �������
      strcpy(Root,"\\\\?\\");
      strcat(Root,Path);
      return;
    }
  }

  if (*TempRoot==0)
    strcpy(TempRoot,"\\");
  else
  {
    // ..2 <> ...\2
    if(!PathMayBeAbsolute(TempRoot))
    {
      char Temp[2048];
      FarGetCurDir(sizeof(Temp)-2,Temp);
      AddEndSlash(Temp);
      strcat(Temp,TempRoot); //+(*TempRoot=='\\' || *TempRoot == '/'?1:0)); //??
      xstrncpy(TempRoot,Temp,sizeof(TempRoot)-1);
    }

    if (TempRoot[0]=='\\' && TempRoot[1]=='\\')
    {
      if ((ChPtr=strchr(TempRoot+2,'\\'))!=NULL)
        if ((ChPtr=strchr(ChPtr+1,'\\'))!=NULL)
          *(ChPtr+1)=0;
        else
          strcat(TempRoot,"\\");
    }
    else
    {
      if ((ChPtr=strchr(TempRoot,'\\'))!=NULL)
        *(ChPtr+1)=0;
      else
        if ((ChPtr=strchr(TempRoot,':'))!=NULL)
          strcpy(ChPtr+1,"\\");
    }
  }
  xstrncpy(Root,TempRoot,NM-1);
}

// ������ ������ ��!!!
static void _GetPathRoot(const char *Path,char *Root,int Reenter)
{
  _LOGCOPYR(CleverSysLog Clev("GetPathRoot()"));
  _LOGCOPYR(SysLog("Params: Path='%s'",Path));
  char TempRoot[1024], *TmpPtr;
  char NewPath[2048];
  /* $ 06.06.2002 VVM
    ! ����� UNC ���� */
  int IsUNC = FALSE;
  int PathLen = (int)strlen(Path);
  ConvertNameToFull(Path,NewPath,sizeof(NewPath));
  // �������� ��� �� UNC
  if (PathLen > 2 && Path[0] == '\\' && Path[1] == '\\')
  {
    if (WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT &&
        PathLen > 3 && Path[2] == '?' && Path[3] == '\\' &&
        //"\\?\Volume{GUID}" �� �������
        strnicmp(&Path[4],"Volume{",7))
    { // �������� �� ������� UNC ��� ��� NT
      xstrncpy(NewPath, &Path[4], sizeof(NewPath)-1);
      if (PathLen > 8 && strncmp(NewPath, "UNC\\", 4)==0)
      {
        IsUNC = TRUE;
        xstrncpy(NewPath, "\\",  sizeof(NewPath)-1);
        xstrncat(NewPath, &Path[7], sizeof(NewPath)-1);
      }
    }
    else
      IsUNC = TRUE;
  }

  _LOGCOPYR(SysLog("%d NewPath='%s', IsUNC=%d",__LINE__,NewPath,IsUNC));
  if (WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5)
  {
    _LOGCOPYR(CleverSysLog Clev("VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5"));
    DWORD FileAttr;
    char JuncName[NM];

    xstrncpy(TempRoot,NewPath,sizeof(TempRoot)-1);
    TmpPtr=TempRoot;

    char *CtlChar = NULL; // ��������� �� ������ ��������� ���� � UNC. ��� ����� �������
    if (IsUNC)
      CtlChar = strchr(TmpPtr+2,'\\');
    if (!IsUNC || CtlChar)
    {
      char *Ptr=strrchr(TmpPtr,'\\');
      while(Ptr >= CtlChar && strlen(TempRoot) > 2)
      {
        FileAttr=GetFileAttributes(TempRoot);
        _LOGCOPYR(SysLog("GetFileAttributes('%s')=0x%08X",TempRoot,FileAttr));
        if(FileAttr != (DWORD)-1 && (FileAttr&FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT)
        {
          if(GetJunctionPointInfo(TempRoot,JuncName,sizeof(JuncName)))
          {
            int offset = 0;
            if (!strncmp(JuncName,"\\??\\",4))
            {
              offset = 4;
            }
            else if (*JuncName == '.')
            {
              //AY: ��� ��� ��������� �� ������ ������ (�� ������ �������� ��� ����)
              //    � �� ������ ����� ������ ��� � ����� �������� ����� ��������� �������������
              //    ����. ��� ������ ���� ��� �� ������� GetFullPathName.
              char TempJunc[2048];
              if (Ptr)
              {
                *Ptr = 0;
                if (*(Ptr+1) == 0)
                {
                  Ptr=strrchr(TmpPtr,'\\');
                  if (Ptr) *Ptr = 0;
                }
              }
              xstrncpy(TempJunc,TempRoot,sizeof(TempJunc)-1);
              strcat(TempJunc,JuncName);
              xstrncpy(JuncName,TempJunc,sizeof(JuncName)-1);
            }
            if(!Reenter && !IsLocalVolumePath(JuncName))
              _GetPathRoot(JuncName+offset,Root,TRUE);
            else
              GetPathRootOne(JuncName+offset,Root);
#if 0
            _LOGCOPYR(SysLog("afret  GetPathRootOne() Root='%s', JuncName='%s'",Root,JuncName));
              //CheckParseJunction('\\vr-srv002\userhome$\vskirdin\wwwroot')=2
              //return -> 952 Root='F:\', JuncName='\??\F:\wwwroot'
            if(TempRoot[0] == '\\' && TempRoot[1] == '\\' && IsLocalPath(Root))
            {
              char *Ptr=strchr(TempRoot+2,'\\');
              if(Ptr)
              {
                JuncName[5]='$';
                strcpy(Ptr+1,JuncName+4);
                strcpy(Root,TempRoot);
              }
            }
#endif
            _LOGCOPYR(SysLog("return -> %d Root='%s', JuncName='%s'",__LINE__,Root,JuncName));
            return;
          }
        } /* if */
        if(Ptr) *Ptr=0; else break;
        Ptr=strrchr(TmpPtr,'\\');
      } /* while */
    } /* if */
  } /* if */
  _LOGCOPYR(SysLog("%d NewPath='%s'",__LINE__,NewPath));
  GetPathRootOne(NewPath,Root);
  _LOGCOPYR(SysLog("return -> %d Root='%s'",__LINE__,Root));
  // ���... � ���� ����� ����� ���� \\?\ � ��� � ���
  //GetPathRootOne(Path+((strlen(Path) > 4 && Path[0]=='\\' && Path[2]=='?' && Path[3]=='\\')?4:0),Root);
  /* VVM $ */
}

void WINAPI GetPathRoot(const char *Path,char *Root)
{
  _GetPathRoot(Path,Root,0);
}

int WINAPI FarGetReparsePointInfo(const char *Src,char *Dest,int DestSize)
{
  _LOGCOPYR(CleverSysLog Clev("FarGetReparsePointInfo()"));
  _LOGCOPYR(SysLog("Params: Src='%s'",Src));
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5 && Src && *Src)
  {
      char Src2[2048];
      xstrncpy(Src2,Src,sizeof(Src2)-1);
      /* $ 27.09.2001 IS
         ! �������� ������� ������, ������� �����.
         - ������������ ������ ���������, � �� ������ ������.
         - ��������� �� ������ ������ ��� xstrncpy
      */
      int TempSize=Max((int)(strlen(Src2)+1),DestSize);
      char *TempDest=(char *)alloca(TempSize);
      strcpy(TempDest,Src2);
      AddEndSlash(TempDest);
      DWORD Size=GetJunctionPointInfo(TempDest,TempDest,TempSize);
      // Src2='\\vr-srv002\userhome$\vskirdin\wwwroot', TempDest='\??\F:\wwwroot'
      _LOGCOPYR(SysLog("return -> %d Src2='%s', TempDest='%s'",__LINE__,Src2,TempDest));
#if 0
      if(Src2[0] == '\\' && Src2[1] == '\\' && IsLocalPath(TempDest+4))
      {
        char *Ptr=strchr(Src2+2,'\\');
        if(Ptr)
        {
          TempDest[5]='$';
          strcpy(Ptr+1,TempDest+4);
          strcpy(TempDest,"\\??\\");
          strcat(TempDest,Src2);
        }
      }
#endif
      if(Size && Dest)
        xstrncpy(Dest,TempDest,DestSize-1);
      /* IS $ */
//      _LOGCOPYR(SysLog("return -> %d Dest='%s'",__LINE__,Dest));
      return Size;
  }
  return 0;
}


// Verify that both files are on the same NTFS disk
BOOL WINAPI CanCreateHardLinks(const char *TargetFile,const char *HardLinkName)
{
  if(!TargetFile)
    return FALSE;

  char Root[2][512],FSysName[NM];
  GetPathRoot(TargetFile,Root[0]);
  if(HardLinkName)
    GetPathRoot(HardLinkName,Root[1]);
  else
    strcpy(Root[1],Root[0]);

   // same drive?
  if(!strcmp(Root[0],Root[1]))
  {
    // NTFS drive?
    DWORD FileSystemFlags;
    if(GetVolumeInformation(Root[0],NULL,0,NULL,NULL,&FileSystemFlags,FSysName,sizeof(FSysName)))
    {
      if(!strcmp(FSysName,"NTFS"))
        return TRUE;
    }
  }
  return FALSE;
}

int WINAPI FarMkLink(const char *Src,const char *Dest,DWORD Flags)
{
  int RetCode=0;

  if(Src && *Src && Dest && *Dest)
  {
//    int Delete=Flags&FLINK_DELETE;
    int Op=Flags&0xFFFF;

    switch(Op)
    {
      case FLINK_HARDLINK:
//        if(Delete)
//          RetCode=FAR_DeleteFile(Src);
//        else
          if(CanCreateHardLinks(Src,Dest))
            RetCode=MkLink(Src,Dest);
        break;

      case FLINK_SYMLINK:
      case FLINK_VOLMOUNT:
//        if(Delete)
//          RetCode=FAR_RemoveDirectory(Src);
//        else
          RetCode=ShellCopy::MkSymLink(Src,Dest,
             (Op==FLINK_VOLMOUNT?FCOPY_VOLMOUNT:FCOPY_LINK)|
             (Flags&FLINK_SHOWERRMSG?0:FCOPY_NOSHOWMSGLINK));
    }
  }

  if(RetCode && !(Flags&FLINK_DONOTUPDATEPANEL))
    ShellUpdatePanels(NULL,FALSE);

  return RetCode;
}
