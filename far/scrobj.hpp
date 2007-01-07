#ifndef __SCREENOBJECT_HPP__
#define __SCREENOBJECT_HPP__
/*
scrobj.hpp

Parent class ��� ���� screen objects

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

    int nLockCount;
    ScreenObject *pOwner;

  public:
    SaveScreen *SaveScr;
    static ScreenObject *CaptureMouseObject;

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

    virtual int  VMProcess(int OpCode,void *vParam=NULL,__int64 iParam=0){return 0;};

    void Lock ();
    void Unlock ();
    bool Locked ();

    void SetOwner (ScreenObject *pOwner);
    ScreenObject* GetOwner ();

    void SavePrevScreen();
    void Redraw();
    void GetPosition(int& X1,int& Y1,int& X2,int& Y2);
    int  IsVisible() { return Flags.Check(FSCROBJ_VISIBLE); };
    void SetVisible(int Visible) {Flags.Change(FSCROBJ_VISIBLE,Visible);};
    void SetRestoreScreenMode(int Mode) {Flags.Change(FSCROBJ_ENABLERESTORESCREEN,Mode);};
    void Shadow();

    static void SetCapture(ScreenObject *Obj);
};

#endif  // __SCREENOBJECT_HPP__
