#ifndef __FILEEDITOR_HPP__
#define __FILEEDITOR_HPP__
/*
fileedit.hpp

�������������� ����� - ���������� ��� editor.cpp

*/

/* Revision: 1.38 22.04.2004 $ */

/*
Modify:
  22.04.2004 SVS
    ! ��������� SAVEFILE_* ������ �� editor.hpp � fileedit.hpp
  04.11.2003 SKV
    + FileAttributesModified - ������� ����, ��� ���� ��������������� ��������.
  09.10.2003 SVS
    + FFILEEDIT_SAVEWQUESTIONS - ��������� ��� ��������
  25.07.2003 SVS
    ! ����� ���� ������ - DWORD SysErrorCode.
  11.12.2002 SVS
    ! ��������� ���������� ������ �������� �� �����
  08.11.2002 SVS
    ! Editor::PluginData ����� � FileEditor::PluginData
    ! Editor::SetPluginData() ����� � FileEditor::SetPluginData()
    + GetPluginData()
    + UpdateFileList()
    ! ��������� ������ �������� Editor �� ������� "����"
  04.09.2002 SVS
    + GetLastInfo() & FileInfo - ���������� � �����
  25.06.2002 SVS
    + IsFullScreen()
    ! ������ Editor ����� ������� ������ - ��� ���������� FileEditor
  14.06.2002 IS
    ! DeleteOnClose ���� int:
      0 - �� ������� ������
      1 - ������� ���� � �������
      2 - ������� ������ ����
  22.05.2002 SVS
    + SetTitle()
    ! � Init �������� ������ ���������� - Title
    ! FEdit �� ������� ������������ � ��������� - �������� � ����� �������
      ������� �����
  13.05.2002 VVM
    + ���������� ��������� ������� ����� ���������������� �� ����.
  18.03.2002 SVS
    + SetLockEditor() - ���������� ���������� ������ ��������
  15.01.2002 SVS
    - ����� Show() �� ����� ���� � �������, �.�. � ScreenObject �� �
      ������-������
    ! �������� �������� ������ ������ ����� "����", �� ������� � ��� �����,
      �� �� ��� (������ �������� ���� �������������� �� ������ � ������ ��
      Editor � FileEditor)
    + SetFileName() - ���������� ���������� � ��� �������������� �����
    + ReadFile() - ���������� ���� ���������� ��� �� Editor::ReadFile
    + SaveFile() - ���������� ���� ���������� ��� �� Editor::SaveFile
    ! ProcessEditorInput ���� � FileEditor (� ������� ������� ��...)
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

/* $ 27.05.2001 DJ
   ���� �������� Editor::SaveFile()
*/
enum {
    SAVEFILE_ERROR   = 0,         // �������� ���������, �� ����������
    SAVEFILE_SUCCESS = 1,         // ���� ������� ���������, ���� ��������� ���� �� ����
    SAVEFILE_CANCEL  = 2          // ���������� ��������, �������� �� ���������
};
/* DJ $ */

enum FEOPMODEEXISTFILE{
  FEOPMODE_QUERY        =0,
  FEOPMODE_NEWIFOPEN    =1,
  FEOPMODE_USEEXISTING  =2,
  FEOPMODE_BREAKIFOPEN  =3,
};

enum FFILEEDIT_FLAGS{
  FFILEEDIT_NEW            = 0x00010000,  // ���� ���� ����������! ����� ��� ��� ������ �������! ���� ������ � ��� ���.
  FFILEEDIT_REDRAWTITLE    = 0x00020000,  // ����� ��������� ���������?
  FFILEEDIT_FULLSCREEN     = 0x00040000,  // ������������� �����?
  FFILEEDIT_DISABLEHISTORY = 0x00080000,  // ��������� ������ � �������?
  FFILEEDIT_ENABLEF6       = 0x00100000,  // ������������� �� ������ �����?
  FFILEEDIT_SAVETOSAVEAS   = 0x00200000,  // $ 17.08.2001 KM  ��������� ��� ������ �� AltF7.
                                          //   ��� �������������� ���������� ����� �� ������ ���
                                          //   ������� F2 ������� ����� ShiftF2.
  FFILEEDIT_ISNEWFILE      = 0x00400000,
  FFILEEDIT_SAVEWQUESTIONS = 0x00800000,  // ��������� ��� ��������
};


class FileEditor:public Frame
{
  private:
    typedef class Frame inherited;

    Editor *FEdit;
    KeyBar EditKeyBar;

    /* $ 07.05.2001 DJ */
    NamesList *EditNamesList;
    /* DJ $ */
    char FileName[NM];
    char FullFileName[NM];
    char StartDir[NM];
    char NewTitle[NM];

    char Title[512];
    char PluginTitle[512];
    char PluginData[NM*2];

    WIN32_FIND_DATA FileInfo;
    /* $ 13.02.2001 IS
         ���� �������� ����� ���������, ����� �� ��������� �� ����� ���
    */
    char AttrStr[4];
    /* IS $ */
    /* $ 12.02.2001 IS
         ���� �������� �������� ����� ��� ��������, ���������� ���-������...
    */
    DWORD FileAttributes;
    /* IS $ */
    /* $ 04.11.2003 SKV
      ���� �� ��������������� ��������� ��� save
    */
    BOOL  FileAttributesModified;
    /* SKV $ */
    DWORD SysErrorCode;

  public:
    FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
               int StartLine=-1,int StartChar=-1,int DisableHistory=FALSE,
               char *PluginData=NULL,int ToSaveAs=FALSE,
               int OpenModeExstFile=FEOPMODE_QUERY);
    /* $ 14.06.2002 IS
       DeleteOnClose ���� int:
         0 - �� ������� ������
         1 - ������� ���� � �������
         2 - ������� ������ ����
    */
    FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
               int StartLine,int StartChar,const char *Title,
               int X1,int Y1,int X2,int Y2, int DisableHistory,
               int DeleteOnClose=0,
               int OpenModeExstFile=FEOPMODE_QUERY);
    /* IS $ */
    /* $ 07.05.2001 DJ */
    virtual ~FileEditor();
    /* DJ $ */

  private:
    void DisplayObject();
    int  ProcessQuitKey(int FirstSave,BOOL NeedQuestion=TRUE);
    BOOL GetLastInfo(const char *Name,WIN32_FIND_DATA *FInfo);
    BOOL UpdateFileList();
    /* $ 10.10.2001 IS ��������� DeleteOnClose */
    /* $ 14.06.2002 IS
        DeleteOnClose ���� int:
          0 - �� ������� ������
          1 - ������� ���� � �������
          2 - ������� ������ ����
    */
    void SetDeleteOnClose(int NewMode);
    /* IS 14.06.2002 */
    /* IS 10.10.2001 */

  public:
    /* $ 14.06.2002 IS
       DeleteOnClose ���� int:
         0 - �� ������� ������
         1 - ������� ���� � �������
         2 - ������� ������ ����
    */
    void Init(const char *Name,const char *Title,int CreateNewFile,int EnableSwitch,
              int StartLine,int StartChar,int DisableHistory,char *PluginData,
              int ToSaveAs, int DeleteOnClose,int OpenModeExstFile);
    /* IS $ */

    void InitKeyBar(void);                            // $ 07.08.2000 SVS - ������� ������������� KeyBar Labels
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void ShowConsoleTitle();
    int IsFileChanged() {return(FEdit->IsFileChanged());};
    virtual int IsFileModified() {return(FEdit->IsFileModified());};

    void SetScreenPosition();                         // $ 28.06.2000 tran - NT Console resize - resize editor

    virtual int GetTypeAndName(char *Type,char *Name);
    virtual const char *GetTypeName(){return "[FileEdit]";};
    virtual int GetType() { return MODALTYPE_EDITOR; }


    virtual void OnDestroy();                         // $ 10.05.2001 DJ
    void SetNamesList (NamesList *Names);             // $ 07.05.2001 DJ

    int GetCanLoseFocus(int DynamicMode=FALSE);

    int FastHide();                                   // $ OT - ������� ��� ���� CtrlAltShift

    void SetEnableF6 (int AEnableF6) { Flags.Change(FFILEEDIT_ENABLEF6,AEnableF6); InitKeyBar(); }  // $ 10.05.2001 DJ
    /* $ 17.08.2001 KM
      ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
      ������ ��� ������� F2 ������� ����� ShiftF2.
    */
    void SetSaveToSaveAs(int ToSaveAs) { Flags.Change(FFILEEDIT_SAVETOSAVEAS,ToSaveAs); InitKeyBar(); }
    /* KM $ */

    /* $ 08.12.2001 OT
      ���������� ������� ����, �������� �� ���� ���������
      ������������ ��� �������� ������� ���������� � ������� �� CtrlF10*/
    BOOL isTemporary();
    void ResizeConsole();
    void Show();

    int ReadFile(const char *Name,int &UserBreak);
    int SaveFile(const char *Name,int Ask,int TextFormat,int SaveAs);
    int EditorControl(int Command,void *Param);
    void SetPluginTitle(const char *PluginTitle);
    void SetTitle(const char *Title);
    BOOL SetFileName(const char *NewFileName);
    int ProcessEditorInput(INPUT_RECORD *Rec);
    void SetLockEditor(BOOL LockMode);
    BOOL IsFullScreen(){return Flags.Check(FFILEEDIT_FULLSCREEN);}
    void ChangeEditKeyBar();
    void ShowStatus();

    DWORD GetFileAttributes(LPCTSTR);                 // $ 13.02.2001 IS - ������� ������ ����������� ������� �� win32 api

    void SetPluginData(char *PluginData);
    char *GetPluginData(void){return PluginData;};
};

#endif  // __FILEEDITOR_HPP__
