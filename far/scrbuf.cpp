/*
scrbuf.cpp

����������� ������ �� �����, ���� ����� ���� ����� ���� �����

*/

/* Revision: 1.14 03.03.2002 $ */

/*
Modify:
  03.03.2002 SVS
    + AppliColor()
    ! ������� �����������.
  16.01.2002 SVS
    - ����� FillRect() �� ���������� "�����" ������.
  15.08.2001 SVS
    ! ������� Flush() � ��������� ������� (������ � �������� ������ ��� �����)
  25.07.2001 SVS
    ! ����������������
  24.07.2001 SVS
    ! ������� ����������� � �������� ���������
  23.07.2001 SKV
    + Scroll - �������������� �������
  08.06.2001 SVS
    ! ��������� ������ (� �.�. BC ��������� �� "#elif DIRECT_RT")
  06.06.2001 SVS
    ! W-������� ����� ���� ������ � ������ USE_WFUNC
  06.06.2001 SVS
    ! ��� NT ����������� W-������� ��� ������ � �������
  29.05.2001 tran
    + DIRECT_RT
  12.05.2001 DJ
    ! ���������� ScrBuf �������� ����
  06.05.2001 DJ
    ! �������� #include
  09.01.2001 SVS
    ! �� ������� ER - � SetCursorType �� ������� ������
      ������� ��������� �������
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "scrbuf.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "colors.hpp"
#include "ctrlobj.hpp"

//#if defined(SYSLOG_OT)
// #define DIRECT_SCREEN_OUT
//#endif

#ifdef DIRECT_RT
extern int DirectRT;
#endif

#if defined(USE_WFUNC)
extern WCHAR Oem2Unicode[];
#endif

ScreenBuf ScrBuf;

ScreenBuf::ScreenBuf()
{
  Buf=Shadow=NULL;
  BufX=BufY=0;
  Flushed=FlushedCurPos=FlushedCurType=TRUE;
  LockCount=0;
  hScreen=NULL;
  UseShadow=FALSE;
}


ScreenBuf::~ScreenBuf()
{
  /* $ 13.07.2000 SVS
     ��� �� ������� new[], �� � ����� delete[]
  */
  if(Buf)    delete[] Buf;
  if(Shadow) delete[] Shadow;
  /* SVS $ */
}


/* $ 13.07.2000 SVS
   ��� �� ������� new[], �� � ����� delete[]
*/
void ScreenBuf::AllocBuf(int X,int Y)
{
  if (X==BufX && Y==BufY)
    return;
  if(Buf) delete[] Buf;
  if(Shadow) delete[] Shadow;

  int Cnt=X*Y;
  Buf=new CHAR_INFO[Cnt];
  Shadow=new CHAR_INFO[Cnt];
#if !defined(ALLOC)
  /* � ��� ����� ����� ������������: ��� ���������������� ������� �������
     ������ - ������ ��� �������������������� ������ ;-)
     ������� ���� ����� �����������...
  */
  memset(Buf,0,Cnt*sizeof(CHAR_INFO));
  memset(Shadow,0,Cnt*sizeof(CHAR_INFO));
#endif
  BufX=X;
  BufY=Y;
}
/* SVS $ */

/* ���������� ������������ ������ ��������� �� �������.
*/
void ScreenBuf::FillBuf()
{
  COORD Size,Corner;
  SMALL_RECT Coord;
  Size.X=BufX;
  Size.Y=BufY;
  Corner.X=0;
  Corner.Y=0;
  Coord.Left=0;
  Coord.Top=0;
  Coord.Right=BufX-1;
  Coord.Bottom=BufY-1;
#if defined(USE_WFUNC)
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
    ReadConsoleOutputW(hScreen,Buf,Size,Corner,&Coord);
  else
    ReadConsoleOutputA(hScreen,Buf,Size,Corner,&Coord);
#else
    ReadConsoleOutput(hScreen,Buf,Size,Corner,&Coord);
#endif
  memcpy(Shadow,Buf,BufX*BufY*sizeof(CHAR_INFO));
  UseShadow=TRUE;

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hScreen,&csbi);
  CurX=csbi.dwCursorPosition.X;
  CurY=csbi.dwCursorPosition.Y;
}

/* �������� Text � ����������� �����
*/
void ScreenBuf::Write(int X,int Y,CHAR_INFO *Text,int TextLength)
{
  if (Y>=BufY || TextLength==0)
    return;
  if(X+TextLength >= BufX)
    TextLength=BufX-X; //??
  memcpy(Buf+Y*BufX+X,Text,sizeof(CHAR_INFO)*TextLength);
  Flushed=FALSE;

#ifdef DIRECT_SCREEN_OUT
  Flush();
#elif defined(DIRECT_RT)
  if ( DirectRT  )
    Flush();
#endif
}

/* ������ ���� �� ������������ ������.
*/
void ScreenBuf::Read(int X1,int Y1,int X2,int Y2,CHAR_INFO *Text)
{
  int Width=X2-X1+1;
  int Height=Y2-Y1+1;
  int I, Idx;

  for (Idx=I=0; I < Height; I++, Idx+=Width)
    memcpy(Text+Idx,Buf+(Y1+I)*BufX+X1,sizeof(CHAR_INFO)*Width);

  if (X1==0 && Y1==0 &&
      CtrlObject!=NULL &&
      CtrlObject->Macro.IsRecording() &&
      GetVidChar(MacroChar) != 'R')
    Text[0]=MacroChar;
}

/* �������� �������� �������� ��������� � ������������ � ������
   (� �������� ����������� ��� "��������" ����)
*/
void ScreenBuf::AppliColorMask(int X1,int Y1,int X2,int Y2,WORD ColorMask)
{
int Width=X2-X1+1;
  int Height=Y2-Y1+1;
  int I, J, K;

  for (I=0;I < Height; I++)
  {
    K=(Y1+I)*BufX+X1;
    for (J=0; J < Width; J++)
      Buf[K+J].Attributes&=~ColorMask;
  }

#ifdef DIRECT_SCREEN_OUT
  Flush();
#elif defined(DIRECT_RT)
  if ( DirectRT  )
    Flush();
#endif
}

/* ���������������� ��������� �������� ���������
*/
void ScreenBuf::AppliColor(int X1,int Y1,int X2,int Y2,int Color)
{
  int Width=X2-X1+1;
  int Height=Y2-Y1+1;
  int I, J, K;

  for (I=0;I < Height; I++)
  {
    K=(Y1+I)*BufX+X1;
    for (J=0; J < Width; J++)
      Buf[K+J].Attributes=Color;
  }

#ifdef DIRECT_SCREEN_OUT
  Flush();
#elif defined(DIRECT_RT)
  if ( DirectRT  )
    Flush();
#endif
}

/* ��������� ������������� �������� Ch � ������ Color
*/
void ScreenBuf::FillRect(int X1,int Y1,int X2,int Y2,int Ch,int Color)
{
  int Width=X2-X1+1;
  int Height=Y2-Y1+1;
  int Pos, I, J;

  CHAR_INFO CI,*PtrBuf;
  CI.Attributes=Color;
  SetVidChar(CI,Ch);

  for (I=0; I < Height; I++)
  {
    for (PtrBuf=Buf+(Y1+I)*BufX+X1, J=0; J < Width; J++, ++PtrBuf)
      *PtrBuf=CI;
  }

  Flushed=FALSE;

#ifdef DIRECT_SCREEN_OUT
  Flush();
#elif defined(DIRECT_RT)
  if ( DirectRT  )
    Flush();
#endif
}

/* "��������" ����������� ����� �� �������
*/
void ScreenBuf::Flush()
{
  if (LockCount>0)
    return;
  if (CtrlObject!=NULL && CtrlObject->Macro.IsRecording())
  {
    if (GetVidChar(Buf[0])!='R')
      MacroChar=Buf[0];
    SetVidChar(Buf[0],'R');
    Buf[0].Attributes=FarColorToReal(COL_WARNDIALOGTEXT);
  }
  if (!FlushedCurType && !CurVisible)
  {
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize=CurSize;
    cci.bVisible=CurVisible;
    SetConsoleCursorInfo(hScreen,&cci);
    FlushedCurType=TRUE;
  }
  if (!Flushed)
  {
    Flushed=TRUE;
    if (WaitInMainLoop && Opt.Clock)
      ShowTime(FALSE);
    int WriteX1=BufX-1,WriteY1=BufY-1,WriteX2=0,WriteY2=0;
    int NoChanges=TRUE;
    if (UseShadow)
    {
      CHAR_INFO *PtrBuf=Buf, *PtrShadow=Shadow;

      for (int I=0; I < BufY; I++)
      {
        for (int J=0; J < BufX; J++, ++PtrBuf, ++PtrShadow)
        {
          if (GetVidChar(*PtrBuf) != GetVidChar(*PtrShadow) ||
              PtrBuf->Attributes  != PtrShadow->Attributes)
          {
            if (WriteX1>J)
              WriteX1=J;
            if (WriteX2<J)
              WriteX2=J;
            if (WriteY1>I)
              WriteY1=I;
            if (WriteY2<I)
              WriteY2=I;
            NoChanges=FALSE;
          }
        }
      }
    }
    else
    {
      NoChanges=FALSE;
      WriteX1=0;
      WriteY1=0;
      WriteX2=BufX-1;
      WriteY2=BufY-1;
    }
    if (!NoChanges)
    {
      COORD Size,Corner;
      SMALL_RECT Coord;
      Size.X=BufX;
      Size.Y=BufY;
      Corner.X=WriteX1;
      Corner.Y=WriteY1;
      Coord.Left=WriteX1;
      Coord.Top=WriteY1;
      Coord.Right=WriteX2;
      Coord.Bottom=WriteY2;
#if defined(USE_WFUNC)
      if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
      {
        // ����� �� ����� ������ ������������� oem->unicode???
        WriteConsoleOutputW(hScreen,Buf,Size,Corner,&Coord);
      }
      else
        WriteConsoleOutputA(hScreen,Buf,Size,Corner,&Coord);
#else
      WriteConsoleOutput(hScreen,Buf,Size,Corner,&Coord);
#endif
      memcpy(Shadow,Buf,BufX*BufY*sizeof(CHAR_INFO));
    }
  }
  if (!FlushedCurPos)
  {
    COORD C;
    C.X=CurX;
    C.Y=CurY;
    SetConsoleCursorPosition(hScreen,C);
    FlushedCurPos=TRUE;
  }
  if (!FlushedCurType && CurVisible)
  {
    CONSOLE_CURSOR_INFO cci;
    cci.dwSize=CurSize;
    cci.bVisible=CurVisible;
    SetConsoleCursorInfo(hScreen,&cci);
    FlushedCurType=TRUE;
  }
  UseShadow=TRUE;
  Flushed=TRUE;
}


void ScreenBuf::Lock()
{
  LockCount++;
}


void ScreenBuf::Unlock()
{
  if (LockCount>0)
    LockCount--;
}


void ScreenBuf::SetHandle(HANDLE hScreen)
{
  ScreenBuf::hScreen=hScreen;
}


void ScreenBuf::ResetShadow()
{
  Flushed=FlushedCurType=FlushedCurPos=UseShadow=FALSE;
}


void ScreenBuf::MoveCursor(int X,int Y)
{
  CurX=X;
  CurY=Y;
  FlushedCurPos=FALSE;
}


void ScreenBuf::GetCursorPos(int& X,int& Y)
{
  X=CurX;
  Y=CurY;
}


void ScreenBuf::SetCursorType(int Visible,int Size)
{
  /* $ 09.01.2001 SVS
     �� ������� ER - � SetCursorType �� ������� ������
     ������� ��������� �������
  */
  if (CurVisible!=Visible || CurSize!=Size)
  {
    CurVisible=Visible;
    CurSize=Size;
    FlushedCurType=FALSE;
  }
  /* SVS $ */
}

void ScreenBuf::GetCursorType(int &Visible,int &Size)
{
  Visible=CurVisible;
  Size=CurSize;
}


void ScreenBuf::RestoreMacroChar()
{
  Write(0,0,&MacroChar,1);
  ResetShadow();
}

/*$ 23.07.2001 SKV
  ���������������� ������ �� ���� ������ �����.
*/
void ScreenBuf::Scroll(int Num)
{
  if(Num > 0 && Num < BufY)
    memcpy(Buf,Buf+Num*BufX,(BufY-Num)*BufX*sizeof(CHAR_INFO));
#ifdef DIRECT_SCREEN_OUT
  Flush();
#elif defined(DIRECT_RT)
  if ( DirectRT  )
    Flush();
#endif
}
/* SKV$*/
