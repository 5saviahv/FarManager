/*
filestr.cpp

����� GetFileString

*/

/* Revision: 1.01 13.07.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
  13.07.2000 SVS
    ! ������� ���४樨 �� �ᯮ�짮����� new/delete/realloc
*/

#include "headers.hpp"
#pragma hdrstop

#ifndef __GETFILESTRING_HPP__
#include "filestr.hpp"
#endif

GetFileString::GetFileString(FILE *SrcFile)
{
  /* $ 13.07.2000 SVS
     �.�. � ��᫥��⢨� ������ �����।������ �१ realloc, �
     ��������� Str=new char[1024]; �� �ਬ�����...
  */
  Str=(char*)malloc(1024);
  /* SVS $ */
  StrLength=1024;
  GetFileString::SrcFile=SrcFile;
  ReadPos=ReadSize=0;
}


GetFileString::~GetFileString()
{
  /* $ 13.07.2000 SVS
     �ᯮ��㥬 free
  */
  free(Str);
  /* SVS $ */
}


int GetFileString::GetString(char **DestStr,int &Length)
{
  Length=0;
  int CurLength=0,ExitCode=1;
  while (1)
  {
    if (ReadPos>=ReadSize)
    {
      if ((ReadSize=fread(ReadBuf,1,sizeof(ReadBuf),SrcFile))==0)
      {
        if (CurLength==0)
          ExitCode=0;
        break;
      }
      ReadPos=0;
    }
    int Ch=ReadBuf[ReadPos];
    if (Ch!='\n' && CurLength>0 && Str[CurLength-1]=='\r')
      break;
    ReadPos++;
    if (CurLength>=StrLength-1)
    {
      StrLength+=1024;
      char *NewStr=(char *)realloc(Str,StrLength);
      if (NewStr==NULL)
        return(-1);
      Str=NewStr;
    }
    Str[CurLength++]=Ch;
    if (Ch=='\n')
      break;
  }
  Str[CurLength]=0;
  *DestStr=Str;
  Length=CurLength;
  return(ExitCode);
}

