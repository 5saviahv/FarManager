#ifndef __INFOLIST_HPP__
#define __INFOLIST_HPP__
/*
infolist.hpp

�������������� ������

*/

/* Revision: 1.04 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  30.04.2001 DJ
    + UpdateKeyBar()
    ! ������ CloseDizFile() ������������ ����������� CloseFile()
  05.04.2001 VVM
    + ������������ �������� � ����� MACRO_INFOPANEL
  03.04.2001 VVM
    + ������������ Viewer ��� ��������� ��������.
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "panel.hpp"

class Viewer;

class InfoList:public Panel
{
  private:
    Viewer *DizView;
    int  PrevMacroMode;
    int  OldWrapMode;
    int  OldWrapType;
    char DizFileName[NM];

  private:
    void DisplayObject();
    void ShowDirDescription();
    void ShowPluginDescription();
    void PrintText(char *Str);
    void PrintText(int MsgID);
    void PrintInfo(char *Str);
    void PrintInfo(int MsgID);
    int  OpenDizFile(char *DizFile);
    void SetMacroMode(int Restore = FALSE);
    /* $ 30.04.2001 DJ */
    void DynamicUpdateKeyBar();
    /* DJ $ */

  public:
    InfoList();
    ~InfoList();

  public:
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void Update(int Mode) {Redraw();};
    virtual void SetFocus();
    virtual void KillFocus();
    /* $ 30.04.2001 DJ */
    virtual BOOL UpdateKeyBar();
    virtual void CloseFile();
    /* DJ $ */
};

#endif	// __INFOLIST_HPP__
