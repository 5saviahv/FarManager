#ifndef __INFOLIST_HPP__
#define __INFOLIST_HPP__
/*
infolist.hpp

���ଠ樮���� ������

*/

/* Revision: 1.02 05.04.2001 $ */

/*
Modify:
  05.04.2001 VVM
    + ��४��祭�� ����ᮢ � ०�� MACRO_INFOPANEL
  03.04.2001 VVM
    + �ᯮ������ Viewer ��� ��ᬮ�� ���ᠭ��.
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

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
    void CloseDizFile();
    int  OpenDizFile(char *DizFile);
    void SetMacroMode(int Restore = FALSE);

  public:
    InfoList();
    ~InfoList();

  public:
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void Update(int Mode) {Redraw();};
    virtual void SetFocus();
    virtual void KillFocus();
};

#endif	// __INFOLIST_HPP__
