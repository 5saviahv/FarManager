#ifndef __CONTROLOBJECT_HPP__
#define __CONTROLOBJECT_HPP__
/*
ctrlobj.hpp

��ࠢ����� ��⠫�묨 ��ꥪ⠬�, ࠧ��� ᮮ�饭�� ���������� � ���

*/

/* Revision: 1.04 29.02.2001 $ */

/*
Modify:
  29.04.2001 ��
    + ����७�� NWZ �� ����类��
  28.02.2001 IS
    ! CmdLine ⥯��� 㪠��⥫�
  15.12.2000 SVS
    ! ��⮤ ShowCopyright - public static & ��ࠬ��� Flags.
  15.07.2000 tran
    + new method Redraw()
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class ControlObject:public BaseInput
{
  private:
    int EndLoop;
//    int LastLeftType,LastRightType;
//    int LeftStateBeforeHide,RightStateBeforeHide,HideState;
//    Panel *LastLeftFilePanel,*LastRightFilePanel;

  private:
//    Panel* ControlObject::CreatePanel(int Type);
//    void DeletePanel(Panel *Deleted);

  public:
    ControlObject();
    ~ControlObject();

  public:
    void Init();
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void EnterMainLoop();
    void ExitMainLoop(int Ask);
//    Panel* GetAnotherPanel(Panel *Current);
//    Panel* ChangePanelToFilled(Panel *Current,int NewType);
//    Panel* ChangePanel(Panel *Current,int NewType,int CreateNew,int Force);
//    void SetPanelPositions(int LeftFullScreen,int RightFullScreen);
//    void SetScreenPositions();
//    void RedrawKeyBar();
    /* $ 15.07.2000 tran
       here is :) */
//    void Redraw();
    /* tran 15.07.2000 $ */

  public:
//    Panel *LeftPanel,*RightPanel,*ActivePanel;
    FilePanels *FPanels;
    FilePanels *Cp(); // {return FPanels;};

    Manager ModalManager;
    CommandLine *CmdLine;
    History *CmdHistory,*FolderHistory,*ViewHistory;
    KeyBar *MainKeyBar;
    MenuBar *TopMenuBar;
    HighlightFiles HiFiles;
    GroupSort GrpSort;
    FilePositionCache ViewerPosCache,EditorPosCache;
    KeyMacro Macro;
    PluginsSet Plugins;

    static void ShowCopyright(DWORD Flags=0);
};


#endif // __CONTROLOBJECT_HPP__
