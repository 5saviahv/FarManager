#ifndef __FRAME_HPP__
#define __FRAME_HPP__

/*
frame.hpp

����������� ���� (������� ����� ��� FilePanels, FileEditor, FileViewer)

*/

/* Revision: 1.09 30.05.2001 */

/*
Modify:
  30.05.2001 OT
    - ��������� ������������� UnlockRefresh()
  26.05.2001 OT
    + ����� ������� - DynamicallyBorn - ����������, ���������� ��� ����������� ��� ������ ������
    + SetDynamicallyBorn() � GetDynamicallyBorn()
    + ����������� ���������� ����������� ������: LockRefreshCount, LockRefresh(),UnlockRefresh(),Refreshable()
  18.05.2001 DJ
    ! ������� SetExitCode() ������ �����������
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 OT
    ! ����� GetCanLoseFocus() ���� ����������� � ������ �������� �� ���������
  12.05.2001 DJ
    ! ��������� �� OnChangeFocus ������� ��������� ����������
  12.05.2001 DJ
    + IsTopFrame(), GetMacroMode()
  07.05.2001 DJ
    ! �������� ��������������
  06.05.2001 DJ
    ! �������� #include
  05.05.2001 DJ
    created
*/

#include "scrobj.hpp"

class KeyBar;

enum { MODALTYPE_VIRTUAL,
  MODALTYPE_PANELS,
  MODALTYPE_VIEWER,
  MODALTYPE_EDITOR,
  MODALTYPE_DIALOG,
  MODALTYPE_VMENU,
  MODALTYPE_HELP,
  MODALTYPE_USER
};

class Frame: public ScreenObject
{
  private:
    Frame **ModalStack;
    int  ModalStackCount, ModalStackSize;


  protected:
    int  DynamicallyBorn;
    int  CanLoseFocus;
    int  ExitCode;
    int  KeyBarVisible;
    KeyBar *FrameKeyBar;
    int MacroMode;
    int LockRefreshCount;

  public:
    Frame();
    virtual ~Frame();

//    int ProcessKey(int Key);
//    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);

    virtual int GetCanLoseFocus(int DynamicMode=FALSE) { return(CanLoseFocus); };
    void SetCanLoseFocus(int Mode) { CanLoseFocus=Mode; };
    int  GetExitCode() { return ExitCode; };
    virtual void SetExitCode(int Code) { ExitCode=Code; };

    virtual BOOL IsFileModified() {return(FALSE);};
    virtual char *GetTypeName() {return "Modal";};
    virtual int GetTypeAndName(char *Type,char *Name) {return(MODALTYPE_VIRTUAL);};
    virtual int GetType() { return MODALTYPE_VIRTUAL; }

    virtual void OnDestroy() {};  // ���������� ����� ������������ ����
    virtual void OnCreate() {};   // ���������� ����� ��������� ����
    virtual void OnChangeFocus(int focus); // ���������� ��� ����� ������

    void SetKeyBar(KeyBar *FrameKeyBar);
    void UpdateKeyBar();
    virtual void RedrawKeyBar() { Frame::UpdateKeyBar(); };

    /* $ 12.05.2001 DJ */
    int IsTopFrame();
    int GetMacroMode() { return MacroMode; }
    /* DJ $ */
    void Push(Frame* Modalized);
    bool Pop();
    Frame *operator[](int Index);
    int operator[](Frame *ModalFarame);
    int ModalCount() {return ModalStackCount;}
    void DestroyAllModal();
    void SetDynamicallyBorn(int Born) {DynamicallyBorn=Born;}
    int GetDynamicallyBorn(){return DynamicallyBorn;};
    void LockRefresh() {LockRefreshCount++;}
    void UnlockRefresh() {LockRefreshCount=(LockRefreshCount>0)?LockRefreshCount-1:0;}
    int Refreshable() {return !LockRefreshCount;}
};

#endif // __FRAME_HPP__
