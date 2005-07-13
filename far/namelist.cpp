/*
namelist.cpp

������ ���� ������, ������������ � viewer ��� ������� Gray+/Gray-

*/

/* Revision: 1.11 13.07.2005 $ */

/*
Modify:
  13.07.2005 SVS
    ! ������� ����� NamesList. ������ �� ��������� ����� �������.
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  19.11.2003 IS
    + ������ �� ������� (TList) �� ������ ����, ���������� KS, �����
      ���������� ��� ��������.
    ! ��������� const
    + ������ �� ������������ ������ � GetNextName/GetPrevName
    ! MoveData �������� �� �������, � �� ����������
  14.10.2003 SVS
    ! �������� � NamesList.
    ! NamesList::GetCurDir - ����� ���. �������� - ��������� ������.
    + NamesList::Init()
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  08.05.2002 SVS
    ! �������� �� NULL ����� free()
  06.12.2001 SVS
    ! PrepareDiskPath() - ����� ���.�������� - ������������ ������ ������
  26.11.2001 SVS
    ! ������� PrepareDiskPath() ��� �������������� ����.
  06.05.2001 DJ
    ! �������� #include
  20.02.2001 SVS
    ! ��������� - � ������ ����!
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "namelist.hpp"

NamesList::NamesList()
{
  Init();
}

NamesList::~NamesList()
{
}

void NamesList::AddName(const char *Name,const char *ShortName)
{
  xstrncpy(CurName.Value.Name,Name,sizeof(CurName.Value.Name)-1);
  xstrncpy(CurName.Value.ShortName,NullToEmpty(ShortName),sizeof(CurName.Value.ShortName)-1);
  Names.push_back(CurName);
}


bool NamesList::GetNextName(char *Name, const size_t NameSize,char *ShortName, const size_t ShortNameSize)
{
  if(Names.isEnd())
    return(false);
  xstrncpy(Name, Names.toNext()->Value.Name, NameSize-1);
  if(ShortName)
    xstrncpy(ShortName, Names.toNext()->Value.ShortName, ShortNameSize-1);
  return(true);
}


bool NamesList::GetPrevName(char *Name, const size_t NameSize,char *ShortName, const size_t ShortNameSize)
{
  if (Names.isBegin())
    return(false);
  xstrncpy(Name, Names.toPrev()->Value.Name, NameSize-1);
  if(ShortName)
    xstrncpy(ShortName, Names.toPrev()->Value.ShortName, ShortNameSize-1);
  return(true);
}


void NamesList::SetCurName(const char *Name)
{
  Names.storePosition();
  pCurName=Names.toBegin();
  while(pCurName)
  {
    if(!strcmp(Name,pCurName->Value.Name))
      return;
    pCurName=Names.toNext();
  }
  Names.restorePosition();
}


void NamesList::MoveData(NamesList &Dest)
{
  Dest.Names.swap(Names);
  Dest.CurName=CurName;
  strcpy(Dest.CurDir,CurDir);
  Init();
}


void NamesList::GetCurDir(char *Dir,int DestSize)
{
  if(*CurDir)
    xstrncpy(Dir,CurDir,DestSize);
  else
    *Dir=0;
}


void NamesList::SetCurDir(const char *Dir)
{
  PrepareDiskPath(xstrncpy(CurDir,Dir,sizeof(CurDir)),sizeof(CurDir)-1);
}

void NamesList::Init()
{
  Names.clear();
  CurName.Value.Name[0]=0;
  CurName.Value.ShortName[0]=0;
  *CurDir=0;
}
