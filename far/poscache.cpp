/*
poscache.cpp

��� ����権 � 䠩��� ��� viewer/editor

*/

/* Revision: 1.02 02.11.2000 $ */

/*
Modify:
  02.11.2000 OT
    ! �������� �஢�ન �� ����� ����, �⢥������� ��� ��� 䠩��.
  24.09.2000 SVS
    + ����� �� ��࠭����/����⠭������� ����権 � 䠩�� �� RCtrl+<N>
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

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
  if (*Name=='<')
    strcpy(FullName,Name);
  else
    ConvertNameToFull(Name,FullName, sizeof(FullName));
  strcpy(&Names[CurPos*3*NM],FullName);
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
  if (*Name=='<')
    strcpy(FullName,Name);
  else
    ConvertNameToFull(Name,FullName, sizeof(FullName));
  Position1=Position2=Position3=Position4=Position5=0;
  for (int I=1;I<=Opt.MaxPositionCache;I++)
  {
    int Pos=CurPos-I;
    if (Pos<0)
      Pos+=Opt.MaxPositionCache;
    if (LocalStricmp(&Names[Pos*3*NM],FullName)==0)
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
      break;
    }
  }
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
      // �᫨ �� ������������ ����樨 �� RCtrl+<N>, � � �� �����뢠�� ��
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

