/*
fileattr.cpp

����� � ��ਡ�⠬� 䠩���

*/

/* Revision: 1.00 30.12.2000 $ */

/*
Modify:
  30.12.2000 SVS
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

#include "internalheaders.hpp"

typedef BOOL (WINAPI *PEncryptFileA)(LPCSTR lpFileName);
typedef BOOL (WINAPI *PDecryptFileA)(LPCSTR lpFileName, DWORD dwReserved);


static PEncryptFileA pEncryptFileA=NULL;
static PDecryptFileA pDecryptFileA=NULL;

static int SetFileEncryption(const char *Name,int State);
static int SetFileCompression(const char *Name,int State);


// ����稬 �㭪樨 �ਯ⮢����
int GetEncryptFunctions(void)
{
  const char *Names[]={
    "KERNEL32","ADVAPI32","EncryptFileA","DecryptFileA",
  };

  if(!pEncryptFileA)
  {
    // ࠡ�⠥� ⮫쪮 ��� Win2000! �᫨ �� 2000, � �� ���� � �����뢠�� ��� ����.
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

int ESetFileAttributes(const char *Name,int Attr)
{
  while (!SetFileAttributes(Name,Attr))
  {
    int Code=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
             MSG(MSetAttrCannotFor),(char *)Name,MSG(MRetry),MSG(MSkip),MSG(MCancel));
    if (Code==1 || Code<0)
      break;
    if (Code==2)
      return(FALSE);
  }
  return(TRUE);
}


static int SetFileCompression(const char *Name,int State)
{
  HANDLE hFile=CreateFile(Name,FILE_READ_DATA|FILE_WRITE_DATA,
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
  ��� ����᫮����� ���⠢����� ��ਡ�� FILE_ATTRIBUTE_COMPRESSED
  ����室��� � ����⢥ ��ࠬ��� FileAttr ��।��� ���祭�� 0
*/
int ESetFileCompression(const char *Name,int State,int FileAttr)
{
  if (((FileAttr & FILE_ATTRIBUTE_COMPRESSED)!=0) == State)
    return(TRUE);

  // Drop Encryption
  if ((FileAttr & FILE_ATTRIBUTE_ENCRYPTED) && State)
    SetFileEncryption(Name,0);

  int Ret=TRUE;
  if (FileAttr & (FA_RDONLY|FILE_ATTRIBUTE_SYSTEM))
    SetFileAttributes(Name,FileAttr & ~(FA_RDONLY|FILE_ATTRIBUTE_SYSTEM));
  while (!SetFileCompression(Name,State))
  {
    if (GetLastError()==ERROR_INVALID_FUNCTION)
      break;
    int Code=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                MSG(MSetAttrCompressedCannotFor),(char *)Name,MSG(MRetry),
                MSG(MSkip),MSG(MCancel));
    if (Code==1 || Code<0)
      break;
    if (Code==2)
    {
      Ret=FALSE;
      break;
    }
  }
  // Set ReadOnly
  if (FileAttr & (FA_RDONLY|FILE_ATTRIBUTE_SYSTEM))
    SetFileAttributes(Name,FileAttr);
  return(Ret);
}

/* $ 20.10.2000 SVS
   ���� ��ਡ�� Encripted
*/
static int SetFileEncryption(const char *Name,int State)
{
  // ������ � �஢������ �ᯥ譮��� ����祭�� ���� API...
  if(State)
     return pEncryptFileA ? (*pEncryptFileA)(Name) : FALSE;
  else
     return pDecryptFileA ? (*pDecryptFileA)(Name, 0) : FALSE;
}

/*
  ��� ����᫮����� ���⠢����� ��ਡ�� FILE_ATTRIBUTE_ENCRYPTED
  ����室��� � ����⢥ ��ࠬ��� FileAttr ��।��� ���祭�� 0
*/
int ESetFileEncryption(const char *Name,int State,int FileAttr)
{
  if (((FileAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0) == State)
    return(TRUE);

  if(!IsCryptFileASupport)
    return(TRUE);

  int Ret=TRUE;

  // Drop Compress
  // ��� ��᮪ �� �㦥�, �.�. �㭪�� �ਯ⮢���� ᠬ� 㬥��
  // ࠧ������ ᦠ�� 䠩��.
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
                MSG(MSetAttrEncryptedCannotFor),(char *)Name,MSG(MRetry),
                MSG(MSkip),MSG(MCancel));
    if (Code==1 || Code<0)
      break;
    if (Code==2)
    {
      Ret=FALSE;
      break;
    }
  }

  // Set ReadOnly
  if (FileAttr & (FA_RDONLY|FILE_ATTRIBUTE_SYSTEM))
    SetFileAttributes(Name,FileAttr);

  return(Ret);
}


int ESetFileTime(const char *Name,FILETIME *LastWriteTime,FILETIME *CreationTime,
                  FILETIME *LastAccessTime,int FileAttr)
{
  if (LastWriteTime==NULL && CreationTime==NULL && LastAccessTime==NULL ||
      (FileAttr & FA_DIREC) && WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT)
    return(TRUE);
  while (1)
  {
    if (FileAttr & FA_RDONLY)
      SetFileAttributes(Name,FileAttr & ~FA_RDONLY);
    HANDLE hFile=CreateFile(Name,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,
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
                MSG(MSetAttrTimeCannotFor),(char *)Name,MSG(MRetry),
                MSG(MSkip),MSG(MCancel));
    if (Code==1 || Code<0)
      break;
    if (Code==2)
      return(FALSE);
  }
  return(TRUE);
}
