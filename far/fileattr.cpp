/*
fileattr.cpp

������ � ���������� ������

*/

/* Revision: 1.08 30.05.2003 $ */

/*
Modify:
  30.05.2003 SVS
    - �� ����������� ������� Compressed ��� ��������� �������, �������� ReadOnly.
      ������... ��� ����� �������� � Encripted �� Compressed... �������� ReadOnly.
  26.01.2003 IS
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  12.06.2002 SVS
    ! �������, ������������ ��������, ��� �� ��� � ESetFileTime
      ����������: 0 - ������, 1 - ��, 2 - Skip
      ������� ��� ���������� ���� "Skip == Cancel"
  30.01.2002 SVS
    - � ESetFileTime ��������� RO, �� ����� �� ������� �������.
  28.12.2001 SVS
    ! ��������� ������ � ��������
  19.10.2001 SVS
    - ���� � ������������ ���������� ��� ������ � �������� ���������.
  14.05.2001 SVS
    ��� ����������� ����/������� RO ������� �������, � ���������� �������� :-(
  06.05.2001 DJ
    ! �������� #include
  30.12.2000 SVS
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"

typedef BOOL (WINAPI *PEncryptFileA)(LPCSTR lpFileName);
typedef BOOL (WINAPI *PDecryptFileA)(LPCSTR lpFileName, DWORD dwReserved);

static PEncryptFileA pEncryptFileA=NULL;
static PDecryptFileA pDecryptFileA=NULL;

static int SetFileEncryption(const char *Name,int State);
static int SetFileCompression(const char *Name,int State);


// ������� ������� �����������
int GetEncryptFunctions(void)
{
  const char *Names[]={
    "KERNEL32","ADVAPI32","EncryptFileA","DecryptFileA",
  };

  if(!pEncryptFileA)
  {
    // �������� ������ ��� Win2000! ���� �� 2000, �� �� ���� � ���������� ��� �����.
    pEncryptFileA = (PEncryptFileA)GetProcAddress(GetModuleHandle(Names[0]),Names[2]);
    if(!pEncryptFileA)
      pEncryptFileA = (PEncryptFileA)GetProcAddress(GetModuleHandle(Names[1]),Names[2]);
  }

  if(!pDecryptFileA)
  {
    pDecryptFileA = (PDecryptFileA)GetProcAddress(GetModuleHandle(Names[0]),Names[3]);
    if(!pDecryptFileA)
      pDecryptFileA = (PDecryptFileA)GetProcAddress(GetModuleHandle(Names[1]),Names[3]);
  }

  if(pDecryptFileA && pEncryptFileA)
    IsCryptFileASupport=TRUE;

  return IsCryptFileASupport;
}

// ���������� 0 - ������, 1 - ��, 2 - Skip
int ESetFileAttributes(const char *Name,int Attr)
{
//_SVS(SysLog("Attr=0x%08X",Attr));
  while (!SetFileAttributes(Name,Attr))
  {
    int Code=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
             MSG(MSetAttrCannotFor),(char *)Name,MSG(MHRetry),MSG(MHSkip),MSG(MHCancel));
    if (Code==1 || Code<0)
      return 2;
    if (Code==2)
      return 0;
  }
  return 1;
}


static int SetFileCompression(const char *Name,int State)
{
  HANDLE hFile=FAR_CreateFile(Name,FILE_READ_DATA|FILE_WRITE_DATA,
                 FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,
                 FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
  if (hFile==INVALID_HANDLE_VALUE)
    return(FALSE);
  USHORT NewState=State ? COMPRESSION_FORMAT_DEFAULT:COMPRESSION_FORMAT_NONE;
  UDWORD Result;
  int RetCode=DeviceIoControl(hFile,FSCTL_SET_COMPRESSION,&NewState,
                              sizeof(NewState),NULL,0,&Result,NULL);
  CloseHandle(hFile);
  return(RetCode);
}

/*
  ��� ������������ ����������� �������� FILE_ATTRIBUTE_COMPRESSED
  ���������� � �������� ��������� FileAttr �������� �������� 0
*/
// ���������� 0 - ������, 1 - ��, 2 - Skip
int ESetFileCompression(const char *Name,int State,int FileAttr)
{
  if (((FileAttr & FILE_ATTRIBUTE_COMPRESSED)!=0) == State)
    return 1;

  int Ret=1;
  if (FileAttr & (FA_RDONLY|FILE_ATTRIBUTE_SYSTEM))
    SetFileAttributes(Name,FileAttr & ~(FA_RDONLY|FILE_ATTRIBUTE_SYSTEM));

  // Drop Encryption
  if ((FileAttr & FILE_ATTRIBUTE_ENCRYPTED) && State)
    SetFileEncryption(Name,0);

  while (!SetFileCompression(Name,State))
  {
    if (GetLastError()==ERROR_INVALID_FUNCTION)
    {
      Ret=1;
      break;
    }
    int Code=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                MSG(MSetAttrCompressedCannotFor),(char *)Name,MSG(MHRetry),
                MSG(MHSkip),MSG(MHCancel));
    if (Code==1 || Code<0)
    {
      Ret=2;
      break;
    }
    if (Code==2)
    {
      Ret=0;
      break;
    }
  }
  // Set ReadOnly
  if (FileAttr & (FA_RDONLY|FILE_ATTRIBUTE_SYSTEM))
    SetFileAttributes(Name,FileAttr);
  return(Ret);
}

/* $ 20.10.2000 SVS
   ����� ������� Encripted
*/

static int SetFileEncryption(const char *Name,int State)
{
  class ApisToANSI{
    public:
     ApisToANSI() {SetFileApisToANSI();}
    ~ApisToANSI() {SetFileApisToOEM();}
  };

  char AnsiName[NM];

  ApisToANSI Apis;
  OemToChar(Name,AnsiName);

  // ������ � ����������� ���������� ��������� ������ API...
  if(State)
     return pEncryptFileA ? (*pEncryptFileA)(AnsiName) : FALSE;
  else
     return pDecryptFileA ? (*pDecryptFileA)(AnsiName, 0) : FALSE;
}
/*
  ��� ������������ ����������� �������� FILE_ATTRIBUTE_ENCRYPTED
  ���������� � �������� ��������� FileAttr �������� �������� 0
*/
// ���������� 0 - ������, 1 - ��, 2 - Skip
int ESetFileEncryption(const char *Name,int State,int FileAttr)
{
  if (((FileAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0) == State)
    return 1;

  if(!IsCryptFileASupport)
    return 1;

  int Ret=1;

  // Drop Compress
  // ���� ����� �� �����, �.�. ������� ����������� ���� �����
  // ��������� ������ �����.
  //if ((FileAttr & FILE_ATTRIBUTE_COMPRESSED) && State)
  //  SetFileCompression(Name,0);

  // Drop ReadOnly
  if (FileAttr & (FA_RDONLY|FILE_ATTRIBUTE_SYSTEM))
    SetFileAttributes(Name,FileAttr & ~(FA_RDONLY|FILE_ATTRIBUTE_SYSTEM));

  while (!SetFileEncryption(Name,State))
  {
    if (GetLastError()==ERROR_INVALID_FUNCTION)
      break;
    int Code=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                MSG(MSetAttrEncryptedCannotFor),(char *)Name,MSG(MHRetry),
                MSG(MHSkip),MSG(MHCancel));
    if (Code==1 || Code<0)
    {
      Ret=2;
      break;
    }
    if (Code==2)
    {
      Ret=0;
      break;
    }
  }

  // Set ReadOnly
  if (FileAttr & (FA_RDONLY|FILE_ATTRIBUTE_SYSTEM))
    SetFileAttributes(Name,FileAttr);

  return(Ret);
}


// ���������� 0 - ������, 1 - ��, 2 - Skip
int ESetFileTime(const char *Name,FILETIME *LastWriteTime,FILETIME *CreationTime,
                  FILETIME *LastAccessTime,int FileAttr)
{
  if (LastWriteTime==NULL && CreationTime==NULL && LastAccessTime==NULL ||
      (FileAttr & FA_DIREC) && WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT)
    return 1;

  while (1)
  {
    if (FileAttr & FA_RDONLY)
      SetFileAttributes(Name,FileAttr & ~FA_RDONLY);
    HANDLE hFile=FAR_CreateFile(Name,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
                 NULL,OPEN_EXISTING,
                 (FileAttr & FA_DIREC) ? FILE_FLAG_BACKUP_SEMANTICS:0,NULL);
    int SetTime;
    if (hFile==INVALID_HANDLE_VALUE)
      SetTime=FALSE;
    else
    {
      SetTime=SetFileTime(hFile,CreationTime,LastAccessTime,LastWriteTime);
      CloseHandle(hFile);
    }
    if (SetTime)
      break;
    int Code=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                MSG(MSetAttrTimeCannotFor),(char *)Name,MSG(MHRetry),
                MSG(MHSkip),MSG(MHCancel));
    if (Code<0)
    {
      break;
    }
    if(Code == 1)
    {
      if (FileAttr & FA_RDONLY)
        SetFileAttributes(Name,FileAttr);
      return 2;
    }
    if(Code == 2)
    {
      if (FileAttr & FA_RDONLY)
        SetFileAttributes(Name,FileAttr);
      return 0;
    }
  }
  /* $ 14.05.2001 SVS
     ���, RO �������� (��. ����), � ���������� ���� �����?
  */
  SetFileAttributes(Name,FileAttr);
  /* SVS $ */
  return 1;
}
