#ifndef __FILEVIEWER_HPP__
#define __FILEVIEWER_HPP__
/*
fileview.hpp

�������� ����� - ���������� ��� viewer.cpp

*/

#include "frame.hpp"
#include "viewer.hpp"
#include "keybar.hpp"

class FileViewer:public Frame
{
  private:
    virtual void Show();
    virtual void DisplayObject();
    Viewer View;
    int RedrawTitle;
    KeyBar ViewKeyBar;
    char NewTitle[NM];
    int F3KeyOnly;
    int FullScreen;
    int DisableEdit;
    int DisableHistory;
    char Name[NM];
    typedef class Frame inherited;
    /* $ 17.08.2001 KM
      ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
      ������ ��� ������� F2 ������� ����� ShiftF2.
    */
    int SaveToSaveAs;
    /* KM $ */

  public:
    FileViewer(const char *Name,int EnableSwitch=FALSE,int DisableHistory=FALSE,
               int DisableEdit=FALSE,long ViewStartPos=-1,char *PluginData=NULL,
               NamesList *ViewNamesList=NULL,int ToSaveAs=FALSE);
    FileViewer(const char *Name,int EnableSwitch,int DisableHistory,
               const char *Title,int X1,int Y1,int X2,int Y2);
    virtual ~FileViewer();

  public:
    void Init(const char *Name,int EnableSwitch,int DisableHistory,
              long ViewStartPos,char *PluginData,NamesList *ViewNamesList,int ToSaveAs);
    /* $ 07.08.2000 SVS
       ������� ������������� KeyBar Labels
    */
    virtual void InitKeyBar(void);
    /* SVS $ */
    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    virtual __int64 VMProcess(int OpCode,void *vParam=NULL,__int64 iParam=0);
    virtual void ShowConsoleTitle();
    /* $ 14.06.2002 IS
       �������� DeleteFolder - ������� �� ������ ����, �� � �������, ���
       ���������� (���� ������� ����). �� ��������� - TRUE (��������
       ��������� SetTempViewName ����� ��, ��� � ������)
    */
    void SetTempViewName(const char *Name,BOOL DeleteFolder=TRUE);
    /* IS $ */
    virtual void OnDestroy();

    virtual int GetTypeAndName(char *Type,char *Name);
    virtual const char *GetTypeName(){return "[FileView]";}; ///
    virtual int GetType() { return MODALTYPE_VIEWER; }

    /* $ 12.05.2001 DJ */
    void SetEnableF6 (int AEnable) { DisableEdit = !AEnable; InitKeyBar(); }
    /* DJ $ */
/* $ ������� ��� ���� CtrlAltShift OT */
    virtual int FastHide();

    /* $ 17.08.2001 KM
      ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
      ������ ��� ������� F2 ������� ����� ShiftF2.
    */
    void SetSaveToSaveAs(int ToSaveAs) { SaveToSaveAs=ToSaveAs; InitKeyBar(); }
    /* KM $ */
    int  ViewerControl(int Command,void *Param);
    BOOL IsFullScreen(){return FullScreen;}
    virtual void GetTitle(char *Title,int LenTitle,int TruncSize=0);
    __int64 GetViewFileSize() const;
    __int64 GetViewFilePos() const;
};

#endif  // __FILEVIEWER_HPP__
