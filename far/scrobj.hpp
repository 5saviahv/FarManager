#ifndef __SCREENOBJECT_HPP__
#define __SCREENOBJECT_HPP__
/*
scrobj.hpp

Parent class ��� ���� screen objects

*/

/* Revision: 1.09 25.02.2003 $ */

/*
Modify:
  25.02.2003 SVS
    + FSCROBJ_ISREDRAWING - ���� ������� Show?
  18.05.2002 SVS
    ! ������� ��������� ���������� �� �����
  19.11.2001 OT
    - ����������� ��������� ������ ������������� �������. 115 � 116 ����
  11.07.2001 OT
    ! ������� ShadowSaveScr �� ��������� ������� � protected
  23.06.2001
    ! ����� ���� ��� ��������� Type, ������� ����� �� ������������...
  14.06.2001
    + ����� ����� SetScreenPosition() - ��� ����������. ����� �������������� ���������,
      ������� ��������� ��������� ���� �������, �� ������, � ��������� �������,
      ��������� �� ��������� ������ ��������.
  21.05.2001 OT
    + ������� �� ��������� ������� �������
  06.05.2001 DJ
    ! �������� #include
  15.07.2000 tran
    + add new dirty method - Hide0(), jys set Visible to False
      used in FileViewer, for keybar hiding
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/
#include "bitflags.hpp"

class SaveScreen;

// ����� ����� ������ ������� ���� (�.�. ����� 0xFF) ��������� - ����������� �������
enum {
  FSCROBJ_VISIBLE              = 0x00000001,
  FSCROBJ_ENABLERESTORESCREEN  = 0x00000002,
  FSCROBJ_SETPOSITIONDONE      = 0x00000004,
  FSCROBJ_ISREDRAWING          = 0x00000008,   // ���� ������� Show?
};

class ScreenObject
{
  protected:
    BitFlags Flags;
    SaveScreen *ShadowSaveScr;
    int X1,Y1,X2,Y2;
    int ObjWidth,ObjHeight;

  public:
    SaveScreen *SaveScr;

  private:
    virtual void DisplayObject() {};

  public:
    ScreenObject();
    virtual ~ScreenObject();

  public:
    virtual int ProcessKey(int Key) { return(0); };
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent) { return(0); };
    virtual void Hide();
    virtual void Hide0();   // 15.07.2000 tran - dirty hack :(
    virtual void Show();
    virtual void ShowConsoleTitle() {};
    virtual void SetPosition(int X1,int Y1,int X2,int Y2);
    virtual void SetScreenPosition();
    virtual void ResizeConsole(){};

    void SavePrevScreen();
    void Redraw();
    void GetPosition(int& X1,int& Y1,int& X2,int& Y2);
    int  IsVisible() { return Flags.Check(FSCROBJ_VISIBLE); };
    void SetVisible(int Visible) {Flags.Change(FSCROBJ_VISIBLE,Visible);};
    void SetRestoreScreenMode(int Mode) {Flags.Change(FSCROBJ_ENABLERESTORESCREEN,Mode);};
    void Shadow();
};

#endif  // __SCREENOBJECT_HPP__
