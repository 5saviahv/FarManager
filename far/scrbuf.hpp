#ifndef __SCREENBUF_HPP__
#define __SCREENBUF_HPP__
/*
scrbuf.hpp

����������� ������ �� �����, ���� ����� ���� ����� ���� �����

*/

/* Revision: 1.06 04.06.2002 $ */

/*
Modify:
  04.06.2002 SVS
    + ScreenBuf::WriteA - � ������������
  18.05.2002 SVS
    ! ������� ��������� ���������� �� �����
  04.03.2002 DJ
    ! Appli -> Apply
  03.03.2002 SVS
    + AppliColor()
  23.07.2001 SKV
    + Scroll
  12.05.2001 DJ
    ! ���������� ScrBuf �������� ����
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "bitflags.hpp"

class ScreenBuf
{
  private:
    BitFlags SBFlags;

    CHAR_INFO *Buf;
    CHAR_INFO *Shadow;
    CHAR_INFO MacroChar;
    HANDLE hScreen;

    int BufX,BufY;
    int CurX,CurY;
    int CurVisible,CurSize;

    int LockCount;

  public:
    ScreenBuf();
    ~ScreenBuf();

  public:
    void AllocBuf(int X,int Y);
    void Lock();
    void Unlock();
    int  GetLockCount() {return(LockCount);};
    void SetLockCount(int Count) {LockCount=Count;};
    void SetHandle(HANDLE hScreen);
    void ResetShadow();
    void MoveCursor(int X,int Y);
    void GetCursorPos(int& X,int& Y);
    void SetCursorType(int Visible,int Size);
    void GetCursorType(int &Visible,int &Size);

  public:
    void FillBuf();
    void Read(int X1,int Y1,int X2,int Y2,CHAR_INFO *Text);
    void Write(int X,int Y,const CHAR_INFO *Text,int TextLength);
#if defined(USE_WFUNC)
    void WriteA(int X,int Y,const CHAR_INFO *Text,int TextLength);
#endif
    void RestoreMacroChar();

    void ApplyColorMask(int X1,int Y1,int X2,int Y2,WORD ColorMask);
    void ApplyColor(int X1,int Y1,int X2,int Y2,int ColorMask);
    void FillRect(int X1,int Y1,int X2,int Y2,int Ch,int Color);

    void Scroll(int);
    void Flush();
};

extern ScreenBuf ScrBuf;

#endif  // __SCREENBUF_HPP__
