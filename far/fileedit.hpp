#ifndef __FILEEDITOR_HPP__
#define __FILEEDITOR_HPP__
/*
fileedit.hpp

�������������� ����� - ���������� ��� editor.cpp

*/

/* Revision: 1.25 10.01.2002 $ */

/*
Modify:
  10.01.2002 SVS
    + FEOPMODE_NEWIFOPEN
    ! FirstSave � ProcessQuitKey() - ��� ��������.
  26.12.2001 SVS
    + ��������� FEOPMODE_*
  25.12.2001 SVS
    + ResizeConsole()
  08.12.2001 OT
    - Bugzilla #144 ������� � �����, F4 �� �����, Ctrl-F10.
  02.11.2001 SVS
    ! ������������ �������� � GetTypeName() - ����������� const
  10.10.2001 IS
    + DeleteOnClose
  08.09.2001 IS
    + �������������� �������� � ������� ������������: DisableHistory
  17.08.2001 KM
    + ��������� ������� SetSaveToSaveAs ��� ��������� ��������� �������
      �� ������� F2 � ����� ShiftF2 ��� ������, � ������ ��������������
      ���������� ����� �� ������.
    ! ������ ����������� � ������� Init ��� ������ SaveToSaveAs.
  11.07.2001 OT
    ������� CtrlAltShift � Manager
  25.06.2001 IS
   ! ��������� const
  14.06.2001 OT
    ! "����" ;-)
  31.05.2001 OT
    ! ��������� ��� ExitCode ���������� � XC_QUIT � ���������� � farconst.cpp
  27.05.2001 DJ
    + ��������� ��� ExitCode
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 OT
    - ������ � F4 -> ReloadAgain
  12.05.2001 DJ
    ! ��������� �� OnChangeFocus ���������� � Frame
  10.05.2001 DJ
    + OnDestroy(), DisableHistory, DisableF6
  07.05.2001 DJ
    + �������� NameList (���� ������ ��� �������� ������� �� ����� ���
      ��������� ������� F6)
  06.05.2001 DJ
    ! �������� #include
  07.05.2001 ��
    - ��������� �� "������������" ExitCode ����� � �� Frame :)
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
#include "editor.hpp"
#include "keybar.hpp"

class NamesList;

enum FEOPMODEEXISTFILE{
  FEOPMODE_QUERY        =0,
  FEOPMODE_NEWIFOPEN    =1,
  FEOPMODE_USEEXISTING  =2,
  FEOPMODE_BREAKIFOPEN  =3,
};

class FileEditor:public Frame
{
  private:
    typedef class Frame inherited;

    Editor FEdit;
    KeyBar EditKeyBar;

    /* $ 07.05.2001 DJ */
    NamesList *EditNamesList;
    /* DJ $ */
    char FileName[NM];
    char FullFileName[NM];
    char StartDir[NM];
    char NewTitle[NM];
    int FullScreen;
    /* $ 10.05.2001 DJ */
    int DisableHistory;
    int EnableF6;
    /* DJ $ */
    /* $ 17.08.2001 KM
      ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
      ������ ��� ������� F2 ������� ����� ShiftF2.
    */
    int SaveToSaveAs;
    /* KM $ */

  public:
    FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
               int StartLine=-1,int StartChar=-1,int DisableHistory=FALSE,
               char *PluginData=NULL,int ToSaveAs=FALSE,
               int OpenModeExstFile=FEOPMODE_QUERY);
    FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
               int StartLine,int StartChar,const char *Title,
               int X1,int Y1,int X2,int Y2, int DisableHistory,
               BOOL DeleteOnClose=FALSE,
               int OpenModeExstFile=FEOPMODE_QUERY);

  private:
    void Show();
    void DisplayObject();
    int ProcessQuitKey(int FirstSave,BOOL NeedQuestion=TRUE);

  public:
    /* $ 07.05.2001 DJ */
    virtual ~FileEditor();
    /* DJ $ */
    void Init(const char *Name,int CreateNewFile,int EnableSwitch,
              int StartLine,int StartChar,int DisableHistory,char *PluginData,
              int ToSaveAs, BOOL DeleteOnClose,int OpenModeExstFile);
    /* $ 07.08.2000 SVS
       ������� ������������� KeyBar Labels
    */
    void InitKeyBar(void);
    /* SVS $ */
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void ShowConsoleTitle();
    int IsFileChanged() {return(FEdit.IsFileChanged());};
    virtual int IsFileModified() {return(FEdit.IsFileModified());};
    /* $ 28.06.2000 tran
       NT Console resize - resize editor */
    void SetScreenPosition();
    /* tran $ */

    virtual int GetTypeAndName(char *Type,char *Name);
    virtual const char *GetTypeName(){return "[FileEdit]";}; ///
    virtual int GetType() { return MODALTYPE_EDITOR; }

    /* $ 10.05.2001 DJ */
    virtual void OnDestroy();
    /* DJ $ */

    /* $ 07.05.2001 DJ */
    void SetNamesList (NamesList *Names);
    /* DJ $ */
    /* $ 10.05.2001 DJ */
    void SetEnableF6 (int AEnableF6) { EnableF6 = AEnableF6; InitKeyBar(); }
    /* DJ $ */
    int GetCanLoseFocus(int DynamicMode=FALSE);
    /* $ ������� ��� ���� CtrlAltShift OT */
    int FastHide();

    /* $ 17.08.2001 KM
      ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
      ������ ��� ������� F2 ������� ����� ShiftF2.
    */
    void SetSaveToSaveAs(int ToSaveAs) { SaveToSaveAs=ToSaveAs; InitKeyBar(); }
    /* KM $ */

    /* $ 08.12.2001 OT
      ���������� ������� ����, �������� �� ���� ���������
      ������������ ��� �������� ������� ���������� � ������� �� CtrlF10*/
    BOOL isTemporary();
    void ResizeConsole();
};

#endif  // __FILEEDITOR_HPP__
