/*
cvtname.cpp

������� ��� �������������� ���� ������/�����.

*/

/* Revision: 1.08 14.06.2003 $ */

/*
Modify:
  14.06.2003 IS
    ! ConvertNameToReal - ��� ����������� ������ ���� � �� �������� �������������
      ��������, �.�. ��� ��� ����� ����������
  05.03.2003 SVS
    + ������� �����
    ! ��������� �� ������� � ������������� ������
  25.02.2003 SVS
    ! "free/malloc/realloc -> xf_*" - ���-�� � ������� ��� ���������.
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  07.01.2003 IS
    - ������ � ConvertNameToReal: �����-�� ������������ ���� "�����:" - ��
      ����� �������� �������� �� ����� "�����", ��� ����� � ���������������
      �����������
  21.06.2002 VVM
    ! ��� ������ ������ ����� UNC ���� � �� �������� � ������ ������� ��� � ������
  28.05.2002 SVS
    ! �������� �������  IsLocalPath()
    ! ����� ������� �������� � ������� (���-�� ����� �� ����� ����������)
  22.03.2002 SVS
    ! ��������� � �������� ���������������� ������
    ! ������� CharBufferToSmallWarn, RawConvertShortNameToLongName,
      ConvertNameToFull, ConvertNameToReal, ConvertNameToShort ��������
      �� mix.cpp � cvtname.cpp (��������� � ��������� ������ ;-))
*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "lang.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"

/* $ 01.11.2000 OT
  ����������� ������. ������ ������� ������ � ������������ �������
  �������� ������ ������ � ������ ����� ����������� ����� �����.
  ���� ������ ������ ���, �� ����������� �� ����������
*/
void CharBufferTooSmallWarn(int BufSize, int FileNameSize)
{
  char Buf2 [80];
  sprintf (Buf2,MSG(MBuffSizeTooSmall_2), FileNameSize, BufSize);
  Message(MSG_WARNING,1,MSG(MError),MSG(MBuffSizeTooSmall_1),Buf2,MSG(MOk));
}

/* $ 02.07.2001 IS
   ��������� �������� ����� �� ������ ���������� ���������. ��������, ���� �
   ���������.
   src     - ��������� �� �������� ���
   dest    - ���� �������� ������� ���
   maxsize - ������ dest. � dest ����� ����������� �� ������ (maxsize-1)
             ��������
   ������������ ����� ������������� �������� ��� 0. ���� ������ dest
   ������������, �� ������������ ��������� ������.
   ����������: ��������� ���������� src � dest
*/
DWORD RawConvertShortNameToLongName(const char *src, char *dest, DWORD maxsize)
{
  if(!src || !dest)
     return 0;

  if(!*src)
  {
     *dest=0;
     return 1;
  }

  DWORD SrcSize=strlen(src);

  if(SrcSize == 3 && src[1] == ':' && (src[2] == '\\' || src[2] == '/'))
  {
    SrcSize=Min((DWORD)SrcSize,(DWORD)maxsize);
    memmove(dest,src,SrcSize);
    dest[SrcSize]=0;
    *dest=toupper(*dest);
    return SrcSize;
  }


  DWORD DestSize=0, FinalSize=0, AddSize;
  BOOL Error=FALSE;

  char *Src, *Dest, *DestBuf=NULL,
       *SrcBuf=(char *)xf_malloc(SrcSize+1);

  while(SrcBuf)
  {
     strcpy(SrcBuf, src);
     Src=SrcBuf;

     WIN32_FIND_DATA wfd;
     HANDLE hFile;

     char *Slash, *Dots=strchr(Src, ':');

     if(Dots)
     {
       ++Dots;
       if('\\'==*Dots) ++Dots;
       char tmp=*Dots;
       *Dots=0;
       AddSize=strlen(Src);
       FinalSize=AddSize;
       DestBuf=(char *)xf_malloc(AddSize+64);
       if(DestBuf)
       {
         DestSize=AddSize+64;
         Dest=DestBuf;
       }
       else
       {
         Error=TRUE;
         FinalSize=0;
         break;
       }
       strcpy(Dest, Src);
       Dest+=AddSize;

       *Dots=tmp;
       Src=Dots; // +1 ??? ����� ???
     }

     /* $ 03.12.2001 DJ
        ���� ������ �� �������� - �� �������� ����� ������ ������
     */
     while(!Error && *Src)   /* DJ $ */
     {
       Slash=strchr(Src, '\\');
       if(Slash) *Slash=0;
       hFile=FindFirstFile(SrcBuf, &wfd);
       if(hFile!=INVALID_HANDLE_VALUE)
       {
         FindClose(hFile);
         AddSize=strlen(wfd.cFileName);
         FinalSize+=AddSize;
         if(FinalSize>=DestSize)
         {
           DestBuf=(char *)xf_realloc(DestBuf, FinalSize+64);
           if(DestBuf)
           {
             DestSize+=64;
             Dest=DestBuf+FinalSize-AddSize;
           }
           else
           {
             Error=TRUE;
             FinalSize=0;
             break;
           }
         }
         strcpy(Dest, wfd.cFileName);
         Dest+=AddSize;
         if(Slash)
         {
           *Dest=*Slash='\\';
           ++Dest;
           /* $ 03.12.2001 DJ
              ���� ����� ����� ������ ���� - ���� �������� '\0'
           */
           *Dest = '\0';
           /* DJ $ */
           ++FinalSize;
           ++Slash;
           Slash=strchr(Src=Slash, '\\');
         }
         else
           break;
       }
       else
       {
         Error=TRUE;
         break;
       }
     }
     break;
  }

  if(!Error)
  {
    if(FinalSize<maxsize)
       strcpy(dest, DestBuf);
    else
    {
       *dest=0;
       ++FinalSize;
    }
  }

  if(SrcBuf)  xf_free(SrcBuf);
  if(DestBuf) xf_free(DestBuf);

  return FinalSize;
}
/* IS $ */

int ConvertNameToFull(const char *Src,char *Dest, int DestSize)
{
  int Result = 0;
//  char *FullName = (char *) xf_malloc (DestSize);
//  char *AnsiName = (char *) xf_malloc (DestSize);
  char *FullName = (char *) alloca (DestSize);
  char *AnsiName = (char *) alloca (DestSize);
  *FullName = 0;
  *AnsiName = 0;

//  char FullName[NM],AnsiName[NM],
  char *NamePtr=PointToName(const_cast<char *>(Src));
  Result+=strlen(Src);

  if (NamePtr==Src && (NamePtr[0]!='.' || NamePtr[1]!=0))
  {
    Result+=FarGetCurDir(DestSize,FullName);
    Result+=AddEndSlash(FullName);
    if (Result < DestSize)
    {
      strncat(FullName,Src,Result);
      strncpy(Dest,FullName,Result);
      Dest [Result] = '\0';
    }
    else
    {
      CharBufferTooSmallWarn(DestSize,Result+1);
    }
    return Result;
  }

  if (PathMayBeAbsolute(Src)) //  (isalpha(Src[0]) && Src[1]==':' || Src[0]=='\\' && Src[1]=='\\') //????
  {
    if (*NamePtr &&
        (*NamePtr!='.' || NamePtr[1]!=0 && (NamePtr[1]!='.' || NamePtr[2]!=0)) &&
        (strstr(Src,"\\..\\")==NULL && strstr(Src,"\\.\\")==NULL)
       )
    {
      if (Dest!=Src)
        strcpy(Dest,Src);
      return Result;
    }
  }

  SetFileApisToANSI();
  OemToChar(Src,AnsiName);
  /* $ 08.11.2000 SVS
     ������ DestSize ������������� sizeof(FullName)...
  */
  if(GetFullPathName(AnsiName,DestSize,FullName,&NamePtr))
    CharToOem(FullName,Dest);
  else
    strcpy(Dest,Src);

  // ��� ����� ����� � ������ cd //host/share
  // � ������ ����� �� ���� c:\\host\share
  if(Src[0] == '/' && Src[1] == '/' && Dest[1] == ':' && Dest[3] == '\\')
    memmove(Dest,Dest+2,strlen(Dest+2)+1);

  /* SVS $*/
  SetFileApisToOEM();

  return Result;
}

/*
  ����������� Src � ������ �������� ���� � ������ reparse point � Win2K
  ���� OS ����, �� ���������� ������� ConvertNameToFull()
*/
int WINAPI ConvertNameToReal(const char *Src,char *Dest, int DestSize)
{
  _SVS(CleverSysLog Clev("ConvertNameToReal()"));
  _SVS(SysLog("Params: Src='%s'",Src));
  char TempDest[2048];
  BOOL IsAddEndSlash=FALSE; // =TRUE, ���� ���� ��������� ��������������
                            // � ����� �� ��� ����... ������.

  // ������� ������� ������ ���� �� ������� ������� ��������
  int Ret=ConvertNameToFull(Src,TempDest,sizeof(TempDest));
  //RawConvertShortNameToLongName(TempDest,TempDest,sizeof(TempDest));
  _SVS(SysLog("ConvertNameToFull('%s') -> '%s'",Src,TempDest));

  /* $ 14.06.2003 IS
     ��� ����������� ������ ���� � �� �������� ������������� ��������
  */
  // ��������� �������� Win2K, �.�. � ������ ���� ������ ���� ���������
  // �������, ����������� ������ �������� ��� �����.
  // ����� ������ �� ������ ��� ����������� ������, �.�. ��� ��� ���������� ������
  // ���������� ���������� ��� ������, �� ������� ��������� ������� (�.�. ����������
  // "������������ �������")
  if (IsLocalDrive(TempDest) &&
      WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5)
  /* IS $ */
  {
    _SVS(CleverSysLog Clev("VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5"));
    DWORD FileAttr;
    char *Ptr, Chr;

    Ptr=TempDest+strlen(TempDest);

    _SVS(SysLog("%d FileAttr=0x%08X",__LINE__,GetFileAttributes(TempDest)));
    // ������� ����������������� �� �������� - ���������� ���� ��
    // ����� �������� ������ ���� ������� ����� �������� ������!
    if((FileAttr=GetFileAttributes(TempDest)) != -1 && (FileAttr&FILE_ATTRIBUTE_DIRECTORY))
    {
      if(Ptr[-1] != '\\')
      {
        AddEndSlash(TempDest);
        IsAddEndSlash=TRUE;
        ++Ptr;
      }
    }

    /* $ 21.06.2002 VVM
      ! ����� UNC ���� */
    char *CtrlChar = TempDest;
    if (strlen(TempDest) > 2 && TempDest[0]=='\\' && TempDest[1]=='\\')
      CtrlChar= strchr(TempDest+2, '\\');
    /* VVM $ */
    // ������� ���� ������� ����� �� �����
    while(CtrlChar)
    {
      while(Ptr > TempDest && *Ptr != '\\')
        --Ptr;
      /* $ 07.01.2003 IS
         - ������: �����-�� ������������ ���� "�����:" - �� �����
           �������� �������� �� ����� "�����", ��� ����� �
           ��������������� �����������
      */
      // ���� ��� UNC, �� �������� �� ����� �������, �� ������...
      if(*Ptr != '\\' || Ptr == CtrlChar
        // ���� ����� �� "�����:", �� ���� �����������
        || *(Ptr-1)==':')
        break;
      /* IS $ */

      Chr=*Ptr;
      *Ptr=0;
      FileAttr=GetFileAttributes(TempDest);
      _SVS(SysLog("%d FileAttr=0x%08X ('%s')",__LINE__,FileAttr,TempDest));

      // �! ��� ��� ������ - ���� �� "���������" ���� - �������
      if(FileAttr != (DWORD)-1 && (FileAttr&FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT)
      {
        char TempDest2[1024];

//        if(CheckParseJunction(TempDest,sizeof(TempDest)))
        {
          _SVS(SysLog("%d Parse Junction",__LINE__));
          // ������� ���� ��������
          if(GetJunctionPointInfo(TempDest,TempDest2,sizeof(TempDest2)))
          {
            // ��� ������ �������������� ����� (�� �������� �����)...
            if(!strncmp(TempDest2+4,"Volume{",7))
            {
              char JuncRoot[NM];
              JuncRoot[0]=JuncRoot[1]=0;
              // ������� ���� ����� �����, ����...
              GetPathRootOne(TempDest2+4,JuncRoot);
              // ...�� � ����� ������ ����� ���������.
              strcpy(TempDest2+4,JuncRoot);
            }
            // ��������� ����������� � ������, ���� ������� ������� "\\?\"
            // �� ��� "Volume{" ������ ������ ����� ����������!
            memmove(TempDest2,TempDest2+4,strlen(TempDest2+4)+1);
            *Ptr=Chr; // ����������� ������
            DeleteEndSlash(TempDest2);
            strcat(TempDest2,Ptr);
            strcpy(TempDest,TempDest2);
            Ret=strlen(TempDest);
            // ���. �������� ���� � ��� � �������...
            break;
          }
        }
      }
      *Ptr=Chr;
      --Ptr;
    }
  }
  if(IsAddEndSlash) // ���� �� ������� - ������.
    TempDest[strlen(TempDest)-1]=0;

  if(Dest && DestSize)
    strncpy(Dest,TempDest,DestSize-1);
  _SVS(SysLog("return Dest='%s'",Dest));
  return Ret;
}

void ConvertNameToShort(const char *Src,char *Dest)
{
  char ShortName[NM],AnsiName[NM];
  SetFileApisToANSI();
  OemToChar(Src,AnsiName);
  if (GetShortPathName(AnsiName,ShortName,sizeof(ShortName)))
    CharToOem(ShortName,Dest);
  else
    strcpy(Dest,Src);
  SetFileApisToOEM();
  LocalUpperBuf(Dest,strlen(Dest));
}
