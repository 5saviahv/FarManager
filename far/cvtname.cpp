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

/* $ 01.11.2000 OT
  ����������� ������. ������ ������� ������ � ������������ �������
  �������� ������ ������ � ������ ����� ����������� ����� �����.
  ���� ������ ������ ���, �� ����������� �� ����������
*/
void CharBufferTooSmallWarn(int BufSize, int FileNameSize)
{
  if(FileNameSize > MAX_PATH)
      Message(MSG_WARNING,1,MSG(MError),MSG(MFileNameExceedSystem),MSG(MOk));
  else {
    char Buf2 [80];
    sprintf (Buf2,MSG(MBuffSizeTooSmall_2), FileNameSize, BufSize);
    Message(MSG_WARNING,1,MSG(MError),MSG(MBuffSizeTooSmall_1),Buf2,MSG(MOk));
  }
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

  char BuffSrc[2048];
  char *NamePtr;

  *BuffSrc=0;
  if(!GetFullPathName(src,sizeof(BuffSrc)-1,BuffSrc,&NamePtr))
  {
    xstrncpy(BuffSrc,src,sizeof(BuffSrc)-1);
  }

  DWORD SrcSize=(DWORD)strlen(BuffSrc);

  if(SrcSize == 3 && BuffSrc[1] == ':' && (BuffSrc[2] == '\\' || BuffSrc[2] == '/'))
  {
    SrcSize=Min((DWORD)SrcSize,(DWORD)maxsize);
    memmove(dest,BuffSrc,SrcSize);
    dest[SrcSize]=0;
    *dest=toupper(*dest);
    return SrcSize;
  }


  DWORD DestSize=0, FinalSize=0, AddSize;
  BOOL Error=FALSE;

  char *Src, *Dest=0, *DestBuf=NULL,
       *SrcBuf=(char *)xf_malloc(Max((DWORD)sizeof(BuffSrc),(DWORD)SrcSize)+1);

  while(SrcBuf)
  {
     strcpy(SrcBuf, BuffSrc);
     Src=SrcBuf;

     WIN32_FIND_DATA wfd;

     char *Slash, *Dots=strchr(Src, ':');

     if(Dots)
     {
       ++Dots;
       if('\\'==*Dots) ++Dots;
       char tmp=*Dots;
       *Dots=0;
       AddSize=(DWORD)strlen(Src);
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
     else if (Src[0]=='\\' && Src[1]=='\\')
     {
       Dots=Src+2;
       while(*Dots && '\\'!=*Dots) ++Dots;
       if('\\'==*Dots)
         ++Dots;
       else
       {
          SrcSize=Min((DWORD)SrcSize,(DWORD)maxsize);
          memmove(dest,BuffSrc,SrcSize);
          dest[SrcSize]=0;
          if(SrcBuf) xf_free(SrcBuf);
          return SrcSize;
       }
       while(*Dots && '\\'!=*Dots) ++Dots;
       if('\\'==*Dots) ++Dots;
       char tmp=*Dots;
       *Dots=0;
       AddSize=(DWORD)strlen(Src);
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
       Src=Dots;
     }

     /* $ 03.12.2001 DJ
        ���� ������ �� �������� - �� �������� ����� ������ ������
     */
     while(!Error && *Src)   /* DJ $ */
     {
       Slash=strchr(Src, '\\');
       if(Slash) *Slash=0;
       if(GetFileWin32FindData(SrcBuf, &wfd,false))
       {
         AddSize=(DWORD)strlen(wfd.cFileName);
         FinalSize+=AddSize;
         if(FinalSize>=DestSize-1)
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
  Result+=(int)strlen(Src);

  if (NamePtr==Src && (NamePtr[0]!='.' || NamePtr[1]!=0))
  {
    Result+=FarGetCurDir(DestSize,FullName);
    Result+=AddEndSlash(FullName);
    if (Result < DestSize)
    {
      strncat(FullName,Src,Result);
      xstrncpy(Dest,FullName,Result);
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

  SetFileApisTo(APIS2ANSI);
  FAR_OemToChar(Src,AnsiName);
  /* $ 08.11.2000 SVS
     ������ DestSize ������������� sizeof(FullName)...
  */
  if(GetFullPathName(AnsiName,DestSize,FullName,&NamePtr))
    FAR_CharToOem(FullName,Dest);
  else
    strcpy(Dest,Src);

  // ��� ����� ����� � ������ cd //host/share
  // � ������ ����� �� ���� c:\\host\share
  if(Src[0] == '/' && Src[1] == '/' && Dest[1] == ':' && Dest[3] == '\\')
    memmove(Dest,Dest+2,strlen(Dest+2)+1);

  /* SVS $*/
  SetFileApisTo(APIS2OEM);

  return Result;
}

/*
  ����������� Src � ������ �������� ���� � ������ reparse point � Win2K
  ���� OS ����, �� ���������� ������� ConvertNameToFull()
*/
int WINAPI ConvertNameToReal(const char *Src,char *Dest, int DestSize, bool Internal)
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
  {
    _SVS(CleverSysLog Clev("VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion >= 5"));
    DWORD FileAttr;
    char *Ptr, Chr;

    Ptr=TempDest+strlen(TempDest);

    _SVS(SysLog("%d FileAttr=0x%08X",__LINE__,GetFileAttributes(TempDest)));
    // ������� ����������������� �� �������� - ���������� ���� ��
    // ����� �������� ������ ���� ������� ����� �������� ������!
    
    //���� �� ������� - �� ����� �������, �.�. ��� ����� ���� ����-�������
    if((FileAttr=GetFileAttributes(TempDest)) != INVALID_FILE_ATTRIBUTES)
    {
      if(Ptr[-1] != '\\')
      {
        AddEndSlash(TempDest);
        IsAddEndSlash=TRUE;
        ++Ptr;
      }
    }

    char *CtrlChar = TempDest;
    if (strlen(TempDest) > 2 && TempDest[0]=='\\' && TempDest[1]=='\\')
      CtrlChar= strchr(TempDest+2, '\\');

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
          if(GetReparsePointInfo(TempDest,TempDest2,sizeof(TempDest2)))
          {
            int offset = 0;
            if (!strncmp(TempDest2,"\\??\\",4))
              offset = 4;
            // ��� ������ �������������� ����� (�� �������� �����)...
            if(!strnicmp(TempDest2+offset,"Volume{",7))
            {
              char JuncRoot[NM*2];
              JuncRoot[0]=JuncRoot[1]=0;
              // ������� ���� ����� �����, ����...
              GetPathRootOne(TempDest2+offset,JuncRoot);
              // ...�� � ����� ������ ����� ���������.
              // (�������� - ���� ����� �� �������� - ����� ����� ������������)
              strcpy(TempDest2+offset,(JuncRoot[1]==':'||!Internal)?JuncRoot:TempDest);
            }
            // ��������� ����������� � ������, ���� ������� ������� "\??\"
            // �� ��� "Volume{" ������ ������ ����� ����������!
            memmove(TempDest2,TempDest2+offset,strlen(TempDest2+offset)+1);

            DeleteEndSlash(TempDest2);
            // ����� ���� ��������
            size_t tempLength = strlen(TempDest2);
            // �������� ����� ����� � ������ ������ ����
            size_t leftLength = strlen(TempDest);
            size_t rightLength = strlen(Ptr + 1); // �������� ����� ���� ������� �� ���������� ������� ����� �������

            *Ptr=Chr; // ����������� ������

            // ���� ���� �������� ������ ����� ����� ����, ����������� �����
            if (leftLength < tempLength)
            {
              ;//TempDest = strTempDest.GetBuffer((int)(strTempDest.GetLength() + tempLength - leftLength));
            }
            // ��� ��� �� ����������� ����������� � ����� ������ ���� �������� ��������� ��
            // ������� ������� ������� � ����
            Ptr = TempDest + tempLength - 1;
            // ���������� ������ ����� ���� �� ������ �����, ������ ���� ����� ���� ���������� ��
            // ������� �� ���� ��������
            if (leftLength != tempLength)
            {
              // ���������� ����� �������� ��� �����, ��������� '/', �������� '/' (���� �� ����) � '\0'
              memmove(TempDest + tempLength, TempDest + leftLength, rightLength + (IsAddEndSlash ? 3 : 2));
            }
            // �������� ���� � �������� ������� ����
            memcpy(TempDest, TempDest2, tempLength);
            // ��������� ������ �� ������ ���������� ����������� �� ����
            CtrlChar = TempDest;
            if (strlen(TempDest) > 2 && TempDest[0] == '\\' && TempDest[1] == '\\')
              CtrlChar = strchr(TempDest + 2, '\\');
            // ������������� ����� ������������ ������
            Ret = (int)strlen(TempDest);
            // ��������� � ���������� ����
            continue;

          }
        }
      }
      *Ptr=Chr;
      --Ptr;
    }
  }

  // ���� �� ������� - ������.
  if(IsAddEndSlash && DeleteEndSlash(TempDest))
    --Ret;

  if(Dest && DestSize)
    xstrncpy(Dest,TempDest,DestSize-1);
  _SVS(SysLog("return Dest='%s'",Dest));
  return Ret;
}

int WINAPI OldConvertNameToReal(const char *Src,char *Dest, int DestSize)
{
	return ConvertNameToReal(Src,Dest,DestSize,false);
}

void ConvertNameToShort(const char *Src,char *Dest,int DestSize)
{
  char *AnsiName=(char*)alloca(strlen(Src)+8);
  if(!AnsiName)
  {
    xstrncpy(Dest,Src,DestSize);
    return;
  }
  SetFileApisTo(APIS2ANSI);
  FAR_OemToChar(Src,AnsiName);
  char ShortName[NM];
  if (GetShortPathName(AnsiName,ShortName,sizeof(ShortName)))
    FAR_CharToOemBuff(ShortName,Dest,DestSize);
  else
    xstrncpy(Dest,Src,DestSize);
  SetFileApisTo(APIS2OEM);
  LocalUpperBuf(Dest,(int)strlen(Dest));
}
