#ifndef __FILEPANELS_HPP__
#define __FILEPANELS_HPP__
/*
filepanels.hpp

�������� ������

*/

/* Revision: 1.11 28.12.2001 $ */

/*
Modify:
  28.12.2001 DJ
    + ������ ����� GoToFile()
  02.11.2001 SVS
    ! ������������ �������� � GetTypeName() - ����������� const
  18.07.2001 OT
    ! VFMenu
  11.07.2001 OT
    ! ������� CtrlAltShift � Manager
  14.06.2001 OT
    ! "����" ;-)
  21.05.2001 OT
    + ������� �� ��������� �������� ������� virtual void ResizeConsole();
  15.05.2001 OT
    ! NWZ -> NFZ
  11.05.2001 OT
    ! ��������� Background
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  01.01.2001 tran
      created
*/

#include "frame.hpp"
#include "keybar.hpp"
#include "menubar.hpp"

class Panel;
class CommandLine;

class FilePanels:public Frame
{
  private:
    void DisplayObject();
    typedef class Frame inherited;

  public:
    Panel *LastLeftFilePanel,
          *LastRightFilePanel;
    Panel *LeftPanel,
          *RightPanel,
          *ActivePanel;

    KeyBar      MainKeyBar;
    MenuBar     TopMenuBar;

    int LastLeftType,
        LastRightType;
    int LeftStateBeforeHide,
        RightStateBeforeHide;

  public:
    FilePanels();
    ~FilePanels();

  public:
    void Init();

    Panel* CreatePanel(int Type);
    void   DeletePanel(Panel *Deleted);
    Panel* GetAnotherPanel(Panel *Current);
    Panel* ChangePanelToFilled(Panel *Current,int NewType);
    Panel* ChangePanel(Panel *Current,int NewType,int CreateNew,int Force);
    void   SetPanelPositions(int LeftFullScreen,int RightFullScreen);
//    void   SetPanelPositions();

    void   SetupKeyBar();

    virtual void Show();

    void Redraw();

    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);


    void SetScreenPosition();

    void Update();

    virtual int GetTypeAndName(char *Type,char *Name);
    virtual int GetType() { return MODALTYPE_PANELS; }
    virtual const char *GetTypeName(){return "[FilePanels]";};

    virtual void OnChangeFocus(int focus);

    void RedrawKeyBar(); // virtual
    virtual void ShowConsoleTitle();
    void ResizeConsole();
/* $ ������� ��� ���� CtrlAltShift OT */
    int FastHide();
    void Refresh();
    /* $ 28.12.2001 DJ
       ������ ����� ��� ��������� Ctrl-F10 �� ������ � ���������
    */
    void GoToFile (char *FileName);
    /* DJ $ */
};

#endif // __FILEPANELS_HPP__
