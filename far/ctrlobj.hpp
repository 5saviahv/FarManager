#ifndef __CONTROLOBJECT_HPP__
#define __CONTROLOBJECT_HPP__
/*
ctrlobj.hpp

���������� ���������� ���������, ������� ��������� ���������� � ����

*/

/* Revision: 1.06 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  28.02.2001 IS
    ! CmdLine ������ ���������
  15.12.2000 SVS
    ! ����� ShowCopyright - public static & �������� Flags.
  15.07.2000 tran
    + new method Redraw()
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "macro.hpp"
#include "plugins.hpp"

class CommandLine;
class History;
class KeyBar;
class MenuBar;
class HighlightFiles;
class GroupSort;
class FilePositionCache;
class FilePanels;
class Manager;

class ControlObject
{
  private:
    FilePanels *FPanels;

  public:
    ControlObject();
    ~ControlObject();

  public:
    void Init();
//    int ProcessKey(int Key);
//    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
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
    FilePanels *Cp(); // {return FPanels;};

    Manager *FrameManager;
    CommandLine *CmdLine;
    History *CmdHistory,*FolderHistory,*ViewHistory;
    KeyBar *MainKeyBar;
    MenuBar *TopMenuBar;
    HighlightFiles *HiFiles;
    GroupSort *GrpSort;
    FilePositionCache *ViewerPosCache,*EditorPosCache;
    KeyMacro Macro;
    PluginsSet Plugins;

    static void ShowCopyright(DWORD Flags=0);
};


#endif // __CONTROLOBJECT_HPP__
