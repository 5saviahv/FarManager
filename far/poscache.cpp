/*
poscache.cpp

��� ������� � ������ ��� viewer/editor

*/

/* Revision: 1.06 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  06.04.2001 VVM
    - ������������ ���������������� � �������� ������
  02.04.2001 VVM
    + ��������� Opt.FlagPosixSemantics � �������� ����� � ������� FindPosition()
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  24.09.2000 SVS
    + ������ �� ����������/�������������� ������� � ����� �� RCtrl+<N>
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "poscache.hpp"
#include "global.hpp"
#include "fn.hpp"

FilePositionCache::FilePositionCache()
{
  if(!Opt.MaxPositionCache)
  {
    GetRegKey("System","MaxPositionCache",Opt.MaxPositionCache,64);
    if(Opt.MaxPositionCache < 16 || Opt.MaxPositionCache > 128)
      Opt.MaxPositionCache=64;
  }
  Names=(char*)malloc(Opt.MaxPositionCache*3*NM);
  Positions=(unsigned int*)malloc(Opt.MaxPositionCache*5*sizeof(unsigned int));
  ShortPos=(long*)malloc(Opt.MaxPositionCache*40*sizeof(long));

  if(Names  && Positions && ShortPos)
  {
    memset(Names,0,Opt.MaxPositionCache*3*NM);
    memset(Positions,0,sizeof(unsigned int)*Opt.MaxPositionCache*5);
    memset(ShortPos,0xFF,sizeof(long)*Opt.MaxPositionCache*40);
    CurPos=0;
    IsMemory=1;
  }
  else
  {
    IsMemory=0;
  }

}

FilePositionCache::~FilePositionCache()
{
  if(IsMemory)
  {
    free(Names);
    free(Positions);
    free(ShortPos);
  }
}

void FilePositionCache::AddPosition(char *Name,unsigned int Position1,
     unsigned int Position2,unsigned int Position3,unsigned int Position4,
     unsigned int Position5,
     long *SPosLine,long *SPosLeftPos,
     long *SPosCursor,long *SPosScreenLine)
{
  if(!IsMemory)
    return;

  char FullName[3*NM];
  if (*Name=='<') {
    strcpy(FullName,Name);
  } else {
//    ConvertNameToFull(Name,FullName, sizeof(FullName));
    if (ConvertNameToFull(Name,FullName, sizeof(FullName)) >= sizeof(FullName)){
      return;
    }
  }
  /* $ 06.04.2001 VVM
    - ������������ ���������������� � �������� ������
      ����� ��������� �� ������, � �� ����� :) */
  int Pos = FindPosition(FullName);
  if (Pos >= 0)
    CurPos = Pos;
  strcpy(&Names[CurPos*3*NM],FullName);
  /* VVM $ */
  Positions[CurPos*5+0]=Position1;
  Positions[CurPos*5+1]=Position2;
  Positions[CurPos*5+2]=Position3;
  Positions[CurPos*5+3]=Position4;
  Positions[CurPos*5+4]=Position5;

  memset(&ShortPos[CurPos*40],0xFF,40*sizeof(long));
  if(SPosLine) memmove(&ShortPos[CurPos*40+0],SPosLine,10*sizeof(long));
  if(SPosLeftPos) memmove(&ShortPos[CurPos*40+10],SPosLeftPos,10*sizeof(long));
  if(SPosCursor) memmove(&ShortPos[CurPos*40+20],SPosCursor,10*sizeof(long));
  if(SPosScreenLine) memmove(&ShortPos[CurPos*40+30],SPosScreenLine,10*sizeof(long));

  if (++CurPos>=Opt.MaxPositionCache)
    CurPos=0;
}


void FilePositionCache::GetPosition(char *Name,unsigned int &Position1,
     unsigned int &Position2,unsigned int &Position3,unsigned int &Position4,
     unsigned int &Position5,
     long *SPosLine,long *SPosLeftPos,
     long *SPosCursor,long *SPosScreenLine)
{
  if(!IsMemory)
    return;

  char FullName[3*NM];
  if (*Name=='<'){
    strcpy(FullName,Name);
  } else {
//    ConvertNameToFull(Name,FullName, sizeof(FullName));
    if (ConvertNameToFull(Name,FullName, sizeof(FullName)) >= sizeof(FullName)){
      return ;
    }
  }
  Position1=Position2=Position3=Position4=Position5=0;
  int Pos = FindPosition(FullName);
  if (Pos >= 0)
  {
    Position1=Positions[Pos*5+0];
    Position2=Positions[Pos*5+1];
    Position3=Positions[Pos*5+2];
    Position4=Positions[Pos*5+3];
    Position5=Positions[Pos*5+4];
    if(SPosLine) memmove(SPosLine,&ShortPos[Pos*40+0],10*sizeof(long));
    if(SPosLeftPos) memmove(SPosLeftPos,&ShortPos[Pos*40+10],10*sizeof(long));
    if(SPosCursor) memmove(SPosCursor,&ShortPos[Pos*40+20],10*sizeof(long));
    if(SPosScreenLine) memmove(SPosScreenLine,&ShortPos[Pos*40+30],10*sizeof(long));
  }
}

int FilePositionCache::FindPosition(char *FullName)
{
  for (int I=1;I<=Opt.MaxPositionCache;I++)
  {
    int Pos=CurPos-I;
    if (Pos<0)
      Pos+=Opt.MaxPositionCache;
    int CmpRes;
    if (Opt.FlagPosixSemantics)
      CmpRes = strcmp(&Names[Pos*3*NM],FullName);
    else
      CmpRes = LStricmp(&Names[Pos*3*NM],FullName);
    if (CmpRes == 0)
      return(Pos);
  }
  return(-1);
}

void FilePositionCache::Read(char *Key)
{
  if(!IsMemory)
    return;

  for (int I=0;I < Opt.MaxPositionCache;I++)
  {
    char SubKey[100],DataStr[512];
    sprintf(SubKey,"Item%d",I);
    GetRegKey(Key,SubKey,DataStr,"",sizeof(DataStr));

    char ArgData[2*NM],*DataPtr=DataStr;
    for (int J=0;(DataPtr=GetCommaWord(DataPtr,ArgData))!=NULL;J++)
    {
      if (*ArgData=='$')
        strcpy(Names+I*3*NM,ArgData+1);
      else
        switch(J)
        {
          case 0:
            Positions[I*5+0]=atoi(ArgData);
            break;
          case 1:
            Positions[I*5+1]=atoi(ArgData);
            break;
          case 2:
            Positions[I*5+2]=atoi(ArgData);
            break;
          case 3:
            Positions[I*5+3]=atoi(ArgData);
            break;
          case 4:
            Positions[I*5+4]=atoi(ArgData);
            break;
        }
    }
    sprintf(SubKey,"Short%d",I);
    memset(DataStr,0xff,40*sizeof(long));
    GetRegKey(Key,SubKey,(LPBYTE)&ShortPos[I*40+0],(LPBYTE)DataStr,40*sizeof(long));
  }
}


void FilePositionCache::Save(char *Key)
{
  int J, I;
  if(!IsMemory)
    return;
  for (I=0;I < Opt.MaxPositionCache;I++)
  {
    int Pos=CurPos+I;
    if (Pos>=Opt.MaxPositionCache)
      Pos-=Opt.MaxPositionCache;
    char SubKey[100],DataStr[512];
    sprintf(SubKey,"Item%d",I);
    sprintf(DataStr,"%d,%d,%d,%d,%d,$%s",Positions[Pos*5+0],Positions[Pos*5+1],
            Positions[Pos*5+2],Positions[Pos*5+3],Positions[Pos*5+4],&Names[Pos*3*NM]);
    SetRegKey(Key,SubKey,DataStr);
    if((Opt.SaveViewerShortPos && Opt.SaveViewerPos) ||
       (Opt.SaveEditorShortPos && Opt.SaveEditorPos))
    {
      // ���� �� ������������ ������� �� RCtrl+<N>, �� � �� ���������� ��
      for(J=0; J < 40*sizeof(long); ++J)
        if(ShortPos[Pos*40+J] != 0xFFFFFFFFU)
          break;

      if(J < 40*sizeof(long))
      {
        sprintf(SubKey,"Short%d",I);
        SetRegKey(Key,SubKey,(LPBYTE)&ShortPos[Pos*40+0],40*sizeof(long));
      }
    }
  }
}
