#ifndef __INFOLIST_HPP__
#define __INFOLIST_HPP__
/*
infolist.hpp

�������������� ������

*/

#include "panel.hpp"
#include "viewer.hpp"
//class Viewer;

/* $ 12.10.2001 SKV
  �������� Viewer ��� �� ����������� ������������� ������
  ������� DizView � �������� �� ������� ��� �� ����� ������.
*/
class DizViewer: public Viewer
{
public:
  int InRecursion;
  DizViewer():InRecursion(0){}
  virtual ~DizViewer() {};
  virtual int ProcessKey(int Key)
  {
    InRecursion=1;
    int res=Viewer::ProcessKey(Key);
    InRecursion=0;
    return res;
  }
  virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
  {
    InRecursion=1;
    int res=Viewer::ProcessMouse(MouseEvent);
    InRecursion=0;
    return res;
  }
};

class InfoList:public Panel
{
  private:
    DizViewer *DizView;
    int  PrevMacroMode;
    int  DizPresent;
    int  OldWrapMode;
    int  OldWrapType;
    char DizFileName[NM];

  private:
    virtual void DisplayObject();
    void ShowDirDescription();
    void ShowPluginDescription();
    void PrintText(const char *Str);
    void PrintText(int MsgID);
    void PrintInfo(const char *Str);
    void PrintInfo(int MsgID);
    int  OpenDizFile(char *DizFile);
    void SetMacroMode(int Restore = FALSE);
    void DynamicUpdateKeyBar();

  public:
    InfoList();
    virtual ~InfoList();

  public:
    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    virtual __int64 VMProcess(int OpCode,void *vParam=NULL,__int64 iParam=0);
    virtual void Update(int Mode);
    virtual void SetFocus();
    virtual const char *GetTitle(char *Title,int LenTitle,int TruncSize=0);
    virtual void KillFocus();
    virtual BOOL UpdateKeyBar();
    virtual void CloseFile();
    /* $ 02.01.2002 IS
       �������� ��� ���������������� diz-�����
    */
    virtual int GetCurName(char *Name,char *ShortName);
};

#endif  // __INFOLIST_HPP__
