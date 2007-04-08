#ifndef __FILEPANELS_HPP__
#define __FILEPANELS_HPP__
/*
filepanels.hpp

�������� ������

*/

#include "frame.hpp"
#include "keybar.hpp"
#include "menubar.hpp"
#include "UnicodeString.hpp"

class Panel;
class CommandLine;

class FilePanels:public Frame
{
  private:
    virtual void DisplayObject();
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
    virtual int VMProcess(int OpCode,void *vParam=NULL,__int64 iParam=0);

    int SetAnhoterPanelFocus(void);
    int SwapPanels(void);
    int ChangePanelViewMode(Panel *Current,int Mode,BOOL RefreshFrame);

    virtual void SetScreenPosition();

    void Update();

    virtual int GetTypeAndName(string &strType, string &strName);
    virtual int GetType() { return MODALTYPE_PANELS; }
    virtual const wchar_t *GetTypeName(){return L"[FilePanels]";};

    virtual void OnChangeFocus(int focus);

    virtual void RedrawKeyBar(); // virtual
    virtual void ShowConsoleTitle();
    virtual void ResizeConsole();
/* $ ������� ��� ���� CtrlAltShift OT */
    virtual int FastHide();
    virtual void Refresh();
    /* $ 28.12.2001 DJ
       ������ ����� ��� ��������� Ctrl-F10 �� ������ � ���������
    */
    void GoToFile (const wchar_t *FileName);

    /* $ 16.01.2002 OT
       ���������������� ����������� ����� �� Frame
    */
    virtual int GetMacroMode();
    /* OT $ */
};

#endif // __FILEPANELS_HPP__
