#ifndef __FILEVIEWER_HPP__
#define __FILEVIEWER_HPP__
/*
fileview.hpp

�������� ����� - ���������� ��� viewer.cpp

*/

/* Revision: 1.09 05.06.2001 $ */

/*
Modify:
  05.06.2001 tran
    + ����� FileView - �������� OnChangeFocus
  15.05.2001 OT
    ! NWZ -> NFZ
  12.05.2001 DJ
    ! ��������� �� OnChangeFocus ���������� � Frame
    ! ����� ������������� ExitCode
    + SetEnableF6()
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  07.08.2000 SVS
    + ������� ������������� KeyBar Labels - InitKeyBar()
  28.06.2000 tran
    - NT Console resize bug
      adding SetScreenPosition method
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "frame.hpp"
#include "viewer.hpp"
#include "keybar.hpp"

class FileViewer:public Frame
{
  private:
    void Show();
    void DisplayObject();
    Viewer View;
    KeyBar ViewKeyBar;
    char NewTitle[NM];
    int F3KeyOnly;
    int FullScreen;
    int DisableEdit;
    int DisableHistory;
    char Name[NM];
    typedef class Frame inherited;

  public:
    FileViewer(char *Name,int EnableSwitch=FALSE,int DisableHistory=FALSE,
               int DisableEdit=FALSE,long ViewStartPos=-1,char *PluginData=NULL,
               NamesList *ViewNamesList=NULL);
    FileViewer(char *Name,int EnableSwitch,char *Title,
               int X1,int Y1,int X2,int Y2);
    void Init(char *Name,int EnableSwitch,int DisableHistory,
              long ViewStartPos,char *PluginData,NamesList *ViewNamesList);
    /* $ 07.08.2000 SVS
       ������� ������������� KeyBar Labels
    */
    void InitKeyBar(void);
    /* SVS $ */
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    int GetTypeAndName(char *Type,char *Name);
    void ShowConsoleTitle();
    void SetTempViewName(char *Name);
    /* $ 28.06.2000 tran
       NT Console resize - resize viewer */
    virtual void SetScreenPosition();
    /* tran $ */
    virtual void OnDestroy();
    virtual char *GetTypeName(){return "[FileView]";}; ///
    virtual int GetType() { return MODALTYPE_VIEWER; }

    /* $ 12.05.2001 DJ */
    void SetEnableF6 (int AEnable) { DisableEdit = !AEnable; }
    /* DJ $ */

    virtual void OnChangeFocus(int f);
};

#endif  // __FILEVIEWER_HPP__
