/*
savescr.cpp

��������� � ���������������� ����� ������� � �������

*/

/* Revision: 1.02 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "savescr.hpp"
#include "fn.hpp"
#include "global.hpp"

SaveScreen::SaveScreen()
{
  RealScreen=FALSE;
  SaveArea(0,0,ScrX,ScrY);
}


SaveScreen::SaveScreen(int RealScreen)
{
  SaveScreen::RealScreen=RealScreen;
  SaveArea(0,0,ScrX,ScrY);
}


SaveScreen::SaveScreen(int X1,int Y1,int X2,int Y2,int RealScreen)
{
  SaveScreen::RealScreen=RealScreen;
  SaveArea(X1,Y1,X2,Y2);
}


SaveScreen::~SaveScreen()
{
  if (!ScreenBuf)
    return;
  RestoreArea();
  /* $ 13.07.2000 SVS
     ��� �� ������� new[], �� � ����� delete[]
  */
  delete[] ScreenBuf;
  /* SVS $ */
}


void SaveScreen::Discard()
{
  if (!ScreenBuf)
    return;
  /* $ 13.07.2000 SVS
     ��� �� ������� new[], �� � ����� delete[]
  */
  delete[] ScreenBuf;
  /* SVS $ */
  ScreenBuf=NULL;
}


void SaveScreen::RestoreArea(int RestoreCursor)
{
  if (!ScreenBuf)
    return;
  if (RealScreen)
    PutRealText(X1,Y1,X2,Y2,ScreenBuf);
  else
    PutText(X1,Y1,X2,Y2,ScreenBuf);
  if (RestoreCursor)
    if (RealScreen)
    {
      SetRealCursorType(CurVisible,CurSize);
      MoveRealCursor(CurPosX,CurPosY);
    }
    else
    {
      SetCursorType(CurVisible,CurSize);
      MoveCursor(CurPosX,CurPosY);
    }
}


void SaveScreen::SaveArea(int X1,int Y1,int X2,int Y2)
{
  ScreenBuf=new char[(X2-X1+1)*(Y2-Y1+1)*4+10];
  if (!ScreenBuf)
    return;
  SaveScreen::X1=X1;
  SaveScreen::Y1=Y1;
  SaveScreen::X2=X2;
  SaveScreen::Y2=Y2;
  if (RealScreen)
  {
    GetRealText(X1,Y1,X2,Y2,ScreenBuf);
    GetRealCursorPos(CurPosX,CurPosY);
    GetRealCursorType(CurVisible,CurSize);
  }
  else
  {
    GetText(X1,Y1,X2,Y2,ScreenBuf);
    GetCursorPos(CurPosX,CurPosY);
    GetCursorType(CurVisible,CurSize);
  }
}


void SaveScreen::AppendArea(SaveScreen *NewArea)
{
  CHAR_INFO *Buf=(CHAR_INFO *)ScreenBuf,*NewBuf=(CHAR_INFO *)NewArea->ScreenBuf;
  if (Buf==NULL || NewBuf==NULL)
    return;
  for (int X=X1;X<=X2;X++)
    if (X>=NewArea->X1 && X<=NewArea->X2)
      for (int Y=Y1;Y<=Y2;Y++)
        if (Y>=NewArea->Y1 && Y<=NewArea->Y2)
          Buf[X-X1+(X2-X1+1)*(Y-Y1)]=NewBuf[X-NewArea->X1+(NewArea->X2-NewArea->X1+1)*(Y-NewArea->Y1)];
}
