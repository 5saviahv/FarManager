#ifndef __INFOLIST_HPP__
#define __INFOLIST_HPP__
/*
infolist.hpp

���ଠ樮���� ������

*/

/* Revision: 1.03 30.04.2001 $ */

/*
Modify:
  30.04.2001 DJ
    + UpdateKeyBar()
    ! ����� CloseDizFile() �ᯮ������ ����㠫�� CloseFile()
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
