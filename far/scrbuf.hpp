#ifndef __SCREENBUF_HPP__
#define __SCREENBUF_HPP__
/*
scrbuf.hpp

����������� ������ �� �����, ���� ����� ���� ����� ���� �����

*/

/* Revision: 1.01 12.05.2001 $ */

/*
Modify:
  12.05.2001 DJ
    ! ���������� ScrBuf �������� ����
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

class ScreenBuf
{
  private:
    CHAR_INFO *Buf;
    CHAR_INFO *Shadow;
    CHAR_INFO MacroChar;
    HANDLE hScreen;
    int BufX,BufY;
    int Flushed;
    int LockCount;
    int UseShadow;
    int CurX,CurY,CurVisible,CurSize;
    int FlushedCurPos,FlushedCurType;
  public:
    ScreenBuf();
    ~ScreenBuf();
    void AllocBuf(int X,int Y);
    void FillBuf();
    void Write(int X,int Y,CHAR_INFO *Text,int TextLength);
    void Read(int X1,int Y1,int X2,int Y2,CHAR_INFO *Text);
    void Flush();
    void Lock();
    void Unlock();
    int GetLockCount() {return(LockCount);};
    void SetLockCount(int Count) {LockCount=Count;};
    void SetHandle(HANDLE hScreen);
    void ResetShadow();
    void MoveCursor(int X,int Y);
    void GetCursorPos(int& X,int& Y);
    void SetCursorType(int Visible,int Size);
    void GetCursorType(int &Visible,int &Size);
    void RestoreMacroChar();
};

extern ScreenBuf ScrBuf;

#endif	// __SCREENBUF_HPP__
