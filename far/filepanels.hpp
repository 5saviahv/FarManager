#ifndef __FILEPANELS_HPP__
#define __FILEPANELS_HPP__
/*
filepanels.hpp

�������� ������

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
    virtual ~FilePanels();

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

    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);

    int SetAnhoterPanelFocus(void);
    int SwapPanels(void);
    int ChangePanelViewMode(Panel *Current,int Mode,BOOL RefreshFrame);

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
    void GoToFile (const char *FileName);
    /* DJ $ */

    /* $ 16.01.2002 OT
       ���������������� ����������� ����� �� Frame
    */
  int GetMacroMode();
    /* OT $ */
};

#endif // __FILEPANELS_HPP__
