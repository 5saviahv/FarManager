/*
fileedit.cpp

�������������� ����� - ���������� ��� editor.cpp

*/

/* Revision: 1.106 27.05.2002 $ */

/*
Modify:
  27.05.2002 SVS
    ! � ��������� ������ � Init() ���� �� ������ ����������� ���� ��������.
  24.05.2002 SVS
    ! ��������� � FileEditor::EditorControl ��� �����
  22.05.2002 SVS
    + SetTitle()
    ! � Init �������� ������ ���������� - Title
    ! FEdit �� ������� ������������ � ��������� - �������� � ����� �������
      ������� �����
  18.05.2002 SVS
    - BugZ#515 - AltF5 �� �������� � ProcessEditorInput
  18.05.2002 SVS
    ! �����
  13.05.2002 VVM
    + ���������� ��������� ������� ����� ���������������� �� ����.
  11.05.2002 SVS
    - BugZ#468 - File saving after, cancelation.
  29.04.2002 SVS
    - BugZ#488 - Shift=enter
  22.03.2002 SVS
    - strcpy - Fuck!
  21.03.2002 SVS
    ! �������� ����� ���. � �������� ������� Save As
  20.03.2002 SVS
    ! GetCurrentDirectory -> FarGetCurDir
  19.03.2002 SVS
    - BugZ#371 - F2 -> ������ ���������.
    + Save As... "Mac format (CR)"
  19.03.2002 SVS
    - BugZ#373 - F4 Ctrl-O - ����� ������
  18.03.2002 SVS
    + SetLockEditor() - ���������� ���������� ������ ��������
  26.02.2002 VVM
    ! ��� ������ ����� (������ �����) ����� ������ ����� ��� "������� �������"
  09.02.2002 VVM
    + �������� ������, ���� ������ � ������� �������
  05.02.2002 SVS
    ! ��������������� ���� - ��� �������
  04.02.2002 SVS
    - �������� � ������� ���������
  28.01.2002 OT
    ��� ��������� �������� ����� �� �������� ����� (��������� ������ 1210)
  28.01.2002 VVM
    ! ���� �� ��������� ���� - ���������� ������.
  23.01.2002 SVS
    ! MEditSavedChangedNonFile2
  21.01.2002 SVS
    - Bug#255 - Alt-Shift-Ins - ������� � ������ ������
  16.01.2002 SVS
    - ���. ������ ��������� "return TRUE" � FileEditor::SetFileName()
  15.01.2002 SVS
    ! ������ ����� �� ���������� ������ Editor ����� "����"
    + FileEditor::EditorControl() - ������ ������ �� �������� ������ Editor
      �� ����� File
    + SetFileName() - ���������� ���������� � ��� �������������� �����
    ! ProcessEditorInput ���� � FileEditor (� ������� ������� ��...)
    + ReadFile() - ���������� ���� ���������� ��� �� Editor::ReadFile
    + SaveFile() - ���������� ���� ���������� ��� �� Editor::SaveFile
  14.01.2002 IS
    ! chdir -> FarChDir
  12.01.2002 IS
    ! ExitCode=0 -> ExitCode=XC_OPEN_ERROR
  10.01.2002 SVS
    - Bugz#213 - �� ���� ����������� ����
  28.12.2001 DJ
    ! ��������� Ctrl-F10 �������� � ������ �������
  28.12.2001 SVS
    - BugZ#213 �� ���� ����������� ����
  26.12.2001 SVS
    + ��������� FEOPMODE_*
  25.12.2001 SVS
    + ResizeConsole()
  17.12.2001 KM
    ! ���� !GetCanLoseFocus() ����� �� Alt-F11 ������ ������ ������.
  08.12.2001 OT
    Bugzilla #144 ������� � �����, F4 �� �����, Ctrl-F10.
  27.11.2001 DJ
    + Local � EditorConfig
  26.11.2001 VVM
    ! ������������ ������ ��� ����� ��� CTRL+F10
  14.11.2001 SVS
    ! Ctrl-F10 �� �������, � ������ �������������
  02.11.2001 IS
    - ������������� ���������� ������ �������� ���� ���������� �� �������
  29.10.2001 IS
    + ������� ��������� "��������� ������� �����" � "��������� ��������" �����
      ����� �������� �� alt-shift-f9.
  28.10.2001 SVS
    - �� �������� ����� ����� ������ �������� ���������
  19.10.2001 OT
    - ����������� ������ HyperViewer
  15.10.2001 SVS
    + _KEYMACRO()
  10.10.2001 IS
    + ��������� DeleteOnClose
  04.10.2001 OT
    - ��������� ��� � fileEditor, ����� �� ������ How to open this file? �������� Current �� �������� ���������, ��� �� ������ ������ ������ �����.
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  26.09.2001 SVS
    - ���� � Ctrl-F10, ����� ���������� ������ �����.
  08.09.2001 IS
    + �������������� �������� � ������� ������������: DisableHistory
  17.08.2001 KM
    + ��������� ������� SetSaveToSaveAs ��� ��������� ��������� �������
      �� ������� F2 � ����� ShiftF2 ��� ������, � ������ ��������������
      ���������� ����� �� ������.
    ! ������ ����������� � ������� Init ��� ������ SaveToSaveAs.
    - ������ � KeyBar ������� �� ������� F12 ��� CanLoseFocus=TRUE
  01.08.2001 tran
    - bug � Shift-F2, ������������ ���, esc. F2
  23.07.2001 SVS
    ! ������� ������� ������  MNewOpen � MReload
  22.07.2001 SVS
    + �������� ���� ��� ������ ��� "������"
    ! ��� ����� � ������ ��� "������" ���������.
  11.07.2001 OT
    ! ������� CtrlAltShift � Manager
  06.07.2001 IS
    - ��� �������� ����� � ���� ��� �� �������� �������� ������� EE_READ, ����
      �� �������� �����������.
  25.06.2001 IS
    ! ��������� const
  14.06.2001 OT
    ! "����" ;-)
  07.06.2001 IS
    - ��� (���������� �����): ����� ������� ������� �������, � ������ �����
      �������
  06.06.2001 IS
    - ������ ���� ����� ���������� �����
  05.06.2001 IS
    + �������� �������� ����, ��� �������� ��������������� �������
  27.05.2001 DJ
    ! ������������ ��������� ��� ����� ��������
  26.05.2001 OT
    - ����������� ������ ������� � NFZ
    - �������� �������� ��������� � ��������� ������
  23.05.2001 OT
    + ����� AllowReedit
    ! ����������� ���� ��-�� ������� ���� �������� �� ������ ReloadAgain
    - ����������� ������ ������� � NFZ
  19.05.2001 DJ
    ! ����� ����������� NFZ
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 OT
    - ������ � F4 -> ReloadAgain
  12.05.2001 DJ
    ! ��������� �� OnChangeFocus ���������� � Frame
  11.05.2001 OT
    ! ��������� Background
  10.05.2001 DJ
    + OnDestroy() (�� �������� ���������� �� view/edit history)
    + FileEditor::DisableHistory, DisableF6
    + Alt-F11 - �������� �������
    + ��� Ctrl-F10 ������������� �� ������
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  07.05.2001 DJ
    + �������� NameList (���� ������ ��� �������� ������� �� ����� ���
      ��������� ������� F6)
    - ������ �� ����������
  06.05.2001 DJ
    ! �������� #include
    + ��������� F6
  07.05.2001 ��
    - ��������� �� "������������" �������� ����� � �� Frame :)
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  04.05.2001 DJ
    - � �������� ��������� 623-�� ��� ������� ���� 616:
      "�� �������� KEY_MACRO* � ProcessEditorInput()"
      ������ ��� �������.
  29.04.2001 ��
    + ��������� NWZ �� ����������
  28.04.2001 VVM
    + KeyBar ���� ����� ������������ �������.
  19.04.2001 SVS
    ! ������ SaveAs ����������� ������� ��� ������� Ctrl-Enter
  10.04.2001 IS
    ! �� ������ SetCurDir ��� ctrl-f10, ���� ������ ���� ��� ���� �� ��������
      �������, ��� ����� ���������� ����, ��� ��������� � ���������
      ������� �� ������������.
  05.04.2001 SVS
    + �������� ����� ������ "FileSaveAs" ��� ������� SaveAs
  28.03.2001 SVS
    ! ��������� � SaveFile ����� �������� - SaveAs?
  22.03.2001 SVS
    - "���������" ������� ����� ���������� �������
  18.03.2001 IS
    ! ������� ������� �������� ��� �������� �� "������ ��� ������" �
      "��� ������", ��� ����� ���������� �� ��������, ����� ��������� ������
      "�� �������, ��� ������ ������������� r/o ����" ��� ��� �������� ������.
  01.03.2001 IS
    - ���: �� �����������, �������� �� ���� �� ����� ���� �� ctrl-f10
  27.02.2001 SVS
    + ������� �� ������ ���� ������.
  26.02.2001 IS
    + � ������� ��� � �� ��� ������� :(
      ������ �� ����� ���� ����������� ����������, ������������� � ��������� ��
      alt-shift-f9, ���������, ����� �������� �������� ��������� � �����
      "��������� ������� �����", "��������� ��������"
  21.02.2001 IS
    + ��� ��������� alt-shift-f9 �������� � ���������� �����������
  15.02.2001 IS
    + ������� "���������� �����" � "del ������� �����"
      ��� ����� �������� ��������� �� AltShiftF9
  15.02.2001 IS
    + ������� ������ ��������� � ����� "������� ������ ���������" ��� �����
      �������� ��������� �� AltShiftF9
  01.02.2001 IS
    ! ��������� �� F6 ����� � ��������� �������� ����� �����, � �� ���������
  03.01.2001 SVS
    ! ��� KEY_ALTSHIFTF9 ������ ������� Show()
  19.12.2000 SVS
    + Alt-Shift-F9 - ����� ������� �������� (� ������ IS)
  16.12.2000 tran 1.15
    ! Ctrl-F10 ������� �� ��������� ������
  15.12.2000 SVS
    - Shift-F4, ����� ����. ������ ��������� :-(
  03.12.2000 SVS
    + "���� ���� ����� ������� ReadOnly..." ����� System � Hidden - ��������
      ��������.
  29.11.2000 SVS
    + ���� ���� ����� ������� ReadOnly ��� System ��� Hidden,
      � �������� �� ������ ���������, �� ������� �������.
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  16.10.2000 SVS
    ! ������ 1.08 (#229)
  13.10.2000 tran 1.08
    ! ��� �������� ����������� �� IsFileModified ������ IsFileChanged()
  27.09.2000 SVS
    + ������ �����/����� � �������������� ������� PrintMan
    ! Ctrl-Alt-Shift - ���������, ���� ����.
  27.09.2000 SKV
    + ��� ����������� ���������������� ����� � Ctrl-O �������� FEdit.Hide()
  24.08.2000 SVS
    + ��������� ������� ������ ���������� �� ������� CtrlAltShift
  07.08.2000 SVS
    + ������� �������� ����������� �������������� ������
    + ������� ������������� KeyBar Labels - InitKeyBar()
  21.07.2000 SKV
    + ����� � ����������������� �� ������������� ����� �� CTRLF10
  29.06.2000 tran
    + �������� ���� �������������� ������
  28.06.2000 tran
    - (NT Console resize bug)
      adding FileEditor::SetScreenPosition
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "fileedit.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "dialog.hpp"
#include "fileview.hpp"
#include "ctrlobj.hpp"
#include "manager.hpp"
#include "namelist.hpp"
#include "history.hpp"
#include "cmdline.hpp"
#include "savescr.hpp"

FileEditor::FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
                       int StartLine,int StartChar,int DisableHistory,
                       char *PluginData,int ToSaveAs, int OpenModeExstFile)
{
  _KEYMACRO(SysLog("FileEditor::FileEditor(1)"));
  _KEYMACRO(SysLog(1));
  ScreenObject::SetPosition(0,0,ScrX,ScrY);
  FullScreen=TRUE;
  Init(Name,NULL,CreateNewFile,EnableSwitch,StartLine,StartChar,
       DisableHistory,PluginData,ToSaveAs,FALSE,OpenModeExstFile);
}


FileEditor::FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
            int StartLine,int StartChar,const char *Title,
            int X1,int Y1,int X2,int Y2,int DisableHistory, BOOL DeleteOnClose,
            int OpenModeExstFile)
{
  _KEYMACRO(SysLog("FileEditor::FileEditor(1)"));
  _KEYMACRO(SysLog(1));
  /* $ 02.11.2001 IS
       ������������� ���������� ������ �������� ���� ���������� �� �������
  */
  if(X1<0) X1=0;
  if(Y1<0) Y1=0;
  /* IS $ */
  ScreenObject::SetPosition(X1,Y1,X2,Y2);
  FullScreen=(X1==0 && Y1==0 && X2==ScrX && Y2==ScrY);
  Init(Name,Title,CreateNewFile,EnableSwitch,StartLine,StartChar,DisableHistory,"",
       FALSE,DeleteOnClose,OpenModeExstFile);
}

/* $ 07.05.2001 DJ
   � ����������� ������� EditNamesList, ���� �� ��� ������, � � SetNamesList()
   ������� EditNamesList � �������� ���� ��������
*/
/*
  ����� ������������ ���� ���:
    FileEditor::~FileEditor()
    Editor::~Editor()
    ...
*/
FileEditor::~FileEditor()
{
  _OT(SysLog("[%p] FileEditor::~FileEditor()",this));
  if(FEdit)
    delete FEdit;

  CurrentEditor=NULL;
  if (EditNamesList)
    delete EditNamesList;

  _KEYMACRO(SysLog(-1));
  _KEYMACRO(SysLog("FileEditor::~FileEditor()"));
}

void FileEditor::Init(const char *Name,const char *Title,int CreateNewFile,int EnableSwitch,
                      int StartLine,int StartChar,int DisableHistory,
                      char *PluginData,int ToSaveAs,BOOL DeleteOnClose,
                      int OpenModeExstFile)
{
  FEdit=new Editor;

  if(!FEdit)
  {
    ExitCode=XC_OPEN_ERROR;
    return;
  }

  CurrentEditor=this;
  SetTitle(Title);
  RedrawTitle = FALSE;
  /* $ 07.05.2001 DJ */
  EditNamesList = NULL;
  KeyBarVisible = TRUE;
  /* DJ $ */
  /* $ 10.05.2001 DJ */
  FileEditor::DisableHistory = DisableHistory;
  EnableF6 = EnableSwitch;
  /* DJ $ */
  /* $ 17.08.2001 KM
    ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
    ������ ��� ������� F2 ������� ����� ShiftF2.
  */
  SaveToSaveAs=ToSaveAs;
  /* KM $ */

  if (*Name==0)
  {
    ExitCode=XC_OPEN_ERROR;
    return;
  }

  FEdit->SetPluginData(PluginData);
  FEdit->SetHostFileEditor(this);
  _OT(SysLog("Editor;:Editor(), EnableSwitch=%i",EnableSwitch));
  SetCanLoseFocus(EnableSwitch);
  FarGetCurDir(sizeof(StartDir),StartDir);

  if(!SetFileName(Name))
  {
    ExitCode=XC_OPEN_ERROR;
    return;
  }

  /*$ 11.05.2001 OT */
  //int FramePos=FrameManager->FindFrameByFile(MODALTYPE_EDITOR,FullFileName);
  //if (FramePos!=-1)
  if (EnableSwitch)
  {
    //if (EnableSwitch)
    int FramePos=FrameManager->FindFrameByFile(MODALTYPE_EDITOR,FullFileName);
    if (FramePos!=-1)
    {
      int SwitchTo=FALSE;
      int MsgCode=0;
      if (!(*FrameManager)[FramePos]->GetCanLoseFocus(TRUE) ||
          Opt.Confirm.AllowReedit)
      {
        if(OpenModeExstFile == FEOPMODE_QUERY)
        {
          char MsgFullFileName[NM];
          strncpy(MsgFullFileName,FullFileName,sizeof(MsgFullFileName)-1);
          SetMessageHelp("EditorReload");
          MsgCode=Message(0,3,MSG(MEditTitle),
                TruncPathStr(MsgFullFileName,ScrX-16),
                MSG(MAskReload),
                MSG(MCurrent),MSG(MNewOpen),MSG(MReload));
        }
        else
        {
          MsgCode=(OpenModeExstFile==FEOPMODE_USEEXISTING)?0:
                        (OpenModeExstFile==FEOPMODE_NEWIFOPEN?1:-100);
        }
        switch(MsgCode)
        {
          case 0:         // Current
            SwitchTo=TRUE;
            FrameManager->DeleteFrame(this); //???
            break;
          case 1:         // NewOpen
            SwitchTo=FALSE;
            break;
          case 2:         // Reload
            FrameManager->DeleteFrame(FramePos);
            SetExitCode(-2);
            break;
          case -100:
            //FrameManager->DeleteFrame(this);  //???
            SetExitCode(XC_EXISTS);
            return;
          default:
            FrameManager->DeleteFrame(this);  //???
            SetExitCode(MsgCode == -100?XC_EXISTS:XC_QUIT);
            return;
        }
      }
      else
      {
        SwitchTo=TRUE;
      }
      if (SwitchTo)
      {
        FrameManager->ActivateFrame(FramePos);
        //FrameManager->PluginCommit();
        SetExitCode((OpenModeExstFile != FEOPMODE_QUERY)?XC_EXISTS:TRUE);
        return ;
      }
    }
  }
  /* 11.05.2001 OT $*/

  /* $ 29.11.2000 SVS
     ���� ���� ����� ������� ReadOnly ��� System ��� Hidden,
     � �������� �� ������ ���������, �� ������� �������.
  */
  /* $ 03.12.2000 SVS
     System ��� Hidden - �������� ��������
  */
  /* $ 15.12.2000 SVS
    - Shift-F4, ����� ����. ������ ��������� :-(
  */
  DWORD FAttr=GetFileAttributes(Name);
  /* $ 05.06.2001 IS
     + �������� �������� ����, ��� �������� ��������������� �������
  */
  if(FAttr!=-1 && FAttr&FILE_ATTRIBUTE_DIRECTORY)
  {
    Message(MSG_WARNING,1,MSG(MEditTitle),MSG(MEditCanNotEditDirectory),MSG(MOk));
    ExitCode=XC_OPEN_ERROR;
    return;
  }
  /* IS $ */
  if((Opt.EditorReadOnlyLock&2) &&
     FAttr != -1 &&
     (FAttr &
        (FILE_ATTRIBUTE_READONLY|
           /* Hidden=0x2 System=0x4 - ������������� �� 2-� ���������,
              ������� ��������� ����� 0110.0000 �
              �������� �� ���� ����� => 0000.0110 � ��������
              �� ����� ������ ��������  */
           ((Opt.EditorReadOnlyLock&0x60)>>4)
        )
     )
  )
  /* SVS $ */
  {
    if(Message(MSG_WARNING,2,MSG(MEditTitle),Name,MSG(MEditRSH),
                             MSG(MEditROOpen),MSG(MYes),MSG(MNo)))
    {
      //SetLastError(ERROR_ACCESS_DENIED);
      ExitCode=XC_OPEN_ERROR;
      return;
    }
  }
  /* SVS 03.12.2000 $ */
  /* SVS $ */

  FEdit->SetPosition(X1,Y1,X2,Y2-1);
  FEdit->SetStartPos(StartLine,StartChar);
  FEdit->SetDeleteOnClose(DeleteOnClose);
  int UserBreak;
  /* $ 06.07.2001 IS
     ��� �������� ����� � ���� ��� �� �������� �������� ������� EE_READ, ����
     �� �������� �����������.
  */
  IsNewFile=CreateNewFile;
  if (!ReadFile(FullFileName,UserBreak))
  {
    if(!CreateNewFile || UserBreak)
    {
      if (UserBreak!=1)
      {
        Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MEditTitle),MSG(MEditCannotOpen),FileName,MSG(MOk));
        ExitCode=XC_OPEN_ERROR;
      }
      else
      {
        ExitCode=XC_LOADING_INTERRUPTED;
      }
      FrameManager->DeleteFrame(this);
      CtrlObject->Cp()->Redraw();
      return;
    }
    CtrlObject->Plugins.CurEditor=this;//&FEdit;
    CtrlObject->Plugins.ProcessEditorEvent(EE_READ,NULL);
  }
  /* IS $ */
  ShowConsoleTitle();
  EditKeyBar.SetOwner(this);
  EditKeyBar.SetPosition(X1,Y2,X2,Y2);

  /* $ 07.08.2000 SVS
    ! ���, �������� KeyBar ������� � ��������� ������� */
  InitKeyBar();
  /* SVS $*/

  MacroMode=MACRO_EDITOR;
/*& OT */
  if (EnableSwitch)
  {
    FrameManager->InsertFrame(this);
    //FrameManager->PluginCommit(); // ����! ����� ������ ������ �� ��������
  }
  else
  {
    FrameManager->ExecuteFrame(this);
  }
/* OT &*/

}

/* $ 07.08.2000 SVS
  ������� ������������� KeyBar Labels
*/
void FileEditor::InitKeyBar(void)
{
  /* $ 29.06.2000 tran
     ������� �������� ���� �������������� ������ */
  /* $ 10.05.2001 DJ
     ������� �� EnableF6 ������ CanLoseFocus
  */
  char *FEditKeys[]={MSG(MEditF1),MSG(MEditF2),MSG(MEditF3),MSG(MEditF4),MSG(MEditF5),MSG(MEditF6),MSG(MEditF7),MSG(MEditF8),MSG(MEditF9),MSG(MEditF10),MSG(MEditF11),MSG(MEditF12)};
  if(SaveToSaveAs)
    FEditKeys[2-1]=MSG(MEditShiftF2);
  if(!EnableF6)
    FEditKeys[6-1]="";
  if(!GetCanLoseFocus())
    FEditKeys[12-1]="";
  /* DJ $ */
  char *FEditShiftKeys[]={MSG(MEditShiftF1),MSG(MEditShiftF2),MSG(MEditShiftF3),MSG(MEditShiftF4),MSG(MEditShiftF5),MSG(MEditShiftF6),MSG(MEditShiftF7),MSG(MEditShiftF8),MSG(MEditShiftF9),MSG(MEditShiftF10),MSG(MEditShiftF11),MSG(MEditShiftF12)};
  /* $ 17.12.2001 KM
     ! ���� !GetCanLoseFocus() ����� �� Alt-F11 ������ ������ ������.
  */
  char *FEditAltKeys[]={MSG(MEditAltF1),MSG(MEditAltF2),MSG(MEditAltF3),MSG(MEditAltF4),MSG(MEditAltF5),MSG(MEditAltF6),MSG(MEditAltF7),MSG(MEditAltF8),MSG(MEditAltF9),MSG(MEditAltF10),MSG(MEditAltF11),MSG(MEditAltF12)};
  if(!GetCanLoseFocus())
    FEditAltKeys[11-1]="";
  /* KM $ */
  char *FEditCtrlKeys[]={MSG(MEditCtrlF1),MSG(MEditCtrlF2),MSG(MEditCtrlF3),MSG(MEditCtrlF4),MSG(MEditCtrlF5),MSG(MEditCtrlF6),MSG(MEditCtrlF7),MSG(MEditCtrlF8),MSG(MEditCtrlF9),MSG(MEditCtrlF10),MSG(MEditCtrlF11),MSG(MEditCtrlF12)};

  if(CtrlObject->Plugins.FindPlugin(SYSID_PRINTMANAGER) == -1)
    FEditAltKeys[5-1]="";
  /* tran $ */
  /* $ 07.08.2000 SVS
     ������� �������� ����������� �������������� ������ */
  char *FEditAltShiftKeys[]={MSG(MEditAltShiftF1),MSG(MEditAltShiftF2),MSG(MEditAltShiftF3),MSG(MEditAltShiftF4),MSG(MEditAltShiftF5),MSG(MEditAltShiftF6),MSG(MEditAltShiftF7),MSG(MEditAltShiftF8),MSG(MEditAltShiftF9),MSG(MEditAltShiftF10),MSG(MEditAltShiftF11),MSG(MEditAltShiftF12)};
  char *FEditCtrlShiftKeys[]={MSG(MEditCtrlShiftF1),MSG(MEditCtrlShiftF2),MSG(MEditCtrlShiftF3),MSG(MEditCtrlShiftF4),MSG(MEditCtrlShiftF5),MSG(MEditCtrlShiftF6),MSG(MEditCtrlShiftF7),MSG(MEditCtrlShiftF8),MSG(MEditCtrlShiftF9),MSG(MEditCtrlShiftF10),MSG(MEditCtrlShiftF11),MSG(MEditCtrlShiftF12)};
  char *FEditCtrlAltKeys[]={MSG(MEditCtrlAltF1),MSG(MEditCtrlAltF2),MSG(MEditCtrlAltF3),MSG(MEditCtrlAltF4),MSG(MEditCtrlAltF5),MSG(MEditCtrlAltF6),MSG(MEditCtrlAltF7),MSG(MEditCtrlAltF8),MSG(MEditCtrlAltF9),MSG(MEditCtrlAltF10),MSG(MEditCtrlAltF11),MSG(MEditCtrlAltF12)};
  /* SVS $*/

  EditKeyBar.Set(FEditKeys,sizeof(FEditKeys)/sizeof(FEditKeys[0]));
  EditKeyBar.SetShift(FEditShiftKeys,sizeof(FEditShiftKeys)/sizeof(FEditShiftKeys[0]));
  EditKeyBar.SetAlt(FEditAltKeys,sizeof(FEditAltKeys)/sizeof(FEditAltKeys[0]));
  EditKeyBar.SetCtrl(FEditCtrlKeys,sizeof(FEditCtrlKeys)/sizeof(FEditCtrlKeys[0]));

  /* $ 07.08.2000 SVS
     ������� �������� ����������� �������������� ������ */
  EditKeyBar.SetCtrlAlt(FEditCtrlAltKeys,sizeof(FEditCtrlAltKeys)/sizeof(FEditCtrlAltKeys[0]));
  EditKeyBar.SetCtrlShift(FEditCtrlShiftKeys,sizeof(FEditCtrlShiftKeys)/sizeof(FEditCtrlShiftKeys[0]));
  EditKeyBar.SetAltShift(FEditAltShiftKeys,sizeof(FEditAltShiftKeys)/sizeof(FEditAltShiftKeys[0]));
  /* SVS $ */

  EditKeyBar.Show();
  SetKeyBar(&EditKeyBar);
  FEdit->SetEditKeyBar(&EditKeyBar);
}
/* SVS $ */

void FileEditor::SetNamesList (NamesList *Names)
{
  if (EditNamesList == NULL)
    EditNamesList = new NamesList;
  Names->MoveData (EditNamesList);
}

/* DJ $ */

void FileEditor::Show()
{
  if (FullScreen)
  {
    EditKeyBar.SetPosition(0,ScrY,ScrX,ScrY);
    EditKeyBar.Redraw();
    ScreenObject::SetPosition(0,0,ScrX,ScrY-1);
    FEdit->SetPosition(0,0,ScrX,ScrY-1);
  }
  ScreenObject::Show();
}


void FileEditor::DisplayObject()
{
  FEdit->Show();
}


int FileEditor::ProcessKey(int Key)
{
  DWORD FNAttr;
  char *Ptr, Chr;

  if (RedrawTitle && ((Key & 0x00ffffff) < KEY_END_FKEY))
    ShowConsoleTitle();

  // BugZ#488 - Shift=enter
  if(ShiftPressed && Key == KEY_ENTER && !CtrlObject->Macro.IsExecuting())
  {
    Key=KEY_SHIFTENTER;
  }

  switch(Key)
  {
    /* $ 24.08.2000 SVS
       + ��������� ������� ������ ���������� �� ������� CtrlAltShift
    */
    case KEY_CTRLO:
    {
      /*$ 27.09.2000 skv
        To prevent redraw in macro with Ctrl-O
      */
      FEdit->Hide();
      /* skv$*/
      FrameManager->ShowBackground();
      SetCursorType(FALSE,0);
      WaitKey(-1);
      Show();
      return(TRUE);
    }
/* $ KEY_CTRLALTSHIFTPRESS ������� � manager OT */

    case KEY_F2:
    case KEY_SHIFTF2:
    {
      BOOL Done=FALSE;
      char OldCurDir[4096];
      FarGetCurDir(sizeof(OldCurDir),OldCurDir);

      while(!Done) // ������ �� �����
      {
        // �������� ���� � �����, ����� ��� ��� ������...
        Ptr=strrchr(FullFileName,'\\');
        if(Ptr)
        {
          Chr=*Ptr;
          *Ptr=0;
          // � �����?
          if (!((strlen(FullFileName)==2) && isalpha(FullFileName[0]) && (FullFileName[1]==':')))
          {
            // � ������? ������� ����������?
            if((FNAttr=GetFileAttributes(FullFileName)) == -1 ||
                              !(FNAttr&FILE_ATTRIBUTE_DIRECTORY)
                //|| LocalStricmp(OldCurDir,FullFileName)  // <- ��� ������ ������.
              )
              SaveToSaveAs=TRUE;
          }
          *Ptr=Chr;
        }

        if(Key == KEY_F2 &&
           (FNAttr=GetFileAttributes(FullFileName)) != -1 &&
           !(FNAttr&FILE_ATTRIBUTE_DIRECTORY))
            SaveToSaveAs=FALSE;

        static int TextFormat=0;
        int NameChanged=FALSE;
        if (Key==KEY_SHIFTF2 || SaveToSaveAs)
        {
          const char *HistoryName="NewEdit";
          static struct DialogData EditDlgData[]=
          {
            /* 0 */ DI_DOUBLEBOX,3,1,72,12,0,0,0,0,(char *)MEditTitle,
            /* 1 */ DI_TEXT,5,2,0,0,0,0,0,0,(char *)MEditSaveAs,
            /* 2 */ DI_EDIT,5,3,70,3,1,(DWORD)HistoryName,DIF_HISTORY,0,"",
            /* 3 */ DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
            /* 4 */ DI_TEXT,5,5,0,0,0,0,0,0,(char *)MEditSaveAsFormatTitle,
            /* 5 */ DI_RADIOBUTTON,5,6,0,0,0,0,DIF_GROUP,0,(char *)MEditSaveOriginal,
            /* 6 */ DI_RADIOBUTTON,5,7,0,0,0,0,0,0,(char *)MEditSaveDOS,
            /* 7 */ DI_RADIOBUTTON,5,8,0,0,0,0,0,0,(char *)MEditSaveUnix,
            /* 8 */ DI_RADIOBUTTON,5,9,0,0,0,0,0,0,(char *)MEditSaveMac,
            /* 9 */ DI_TEXT,3,10,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
            /*10 */ DI_BUTTON,0,11,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
            /*11 */ DI_BUTTON,0,11,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel,
          };
          MakeDialogItems(EditDlgData,EditDlg);
          strncpy(EditDlg[2].Data,(SaveToSaveAs?FullFileName:FileName),sizeof(EditDlg[2].Data)-1);
          EditDlg[5].Selected=EditDlg[6].Selected=EditDlg[7].Selected=EditDlg[8].Selected=0;
          EditDlg[5+TextFormat].Selected=TRUE;
          Dialog Dlg(EditDlg,sizeof(EditDlg)/sizeof(EditDlg[0]));
          Dlg.SetPosition(-1,-1,76,14);
          Dlg.SetHelp("FileSaveAs");
          Dlg.Process();
          if (Dlg.GetExitCode()!=10 || *EditDlg[2].Data==0)
            return(FALSE);
          /* $ 07.06.2001 IS
             - ���: ����� ������� ������� �������, � ������ ����� �������
          */
          RemoveTrailingSpaces(EditDlg[2].Data);
          Unquote(EditDlg[2].Data);
          /* IS $ */

          NameChanged=LocalStricmp(EditDlg[2].Data,(SaveToSaveAs?FullFileName:FileName))!=0;
          /* $ 01.08.2001 tran
             ���� ����� ��������� ������ � ������ FileName
             ����������� EditDlg[2].Data */
          if(!NameChanged)
            FarChDir(StartDir); // ������? � ����� ��???

          FNAttr=GetFileAttributes(EditDlg[2].Data);
          if (NameChanged && FNAttr != -1)
          {
            if (Message(MSG_WARNING,2,MSG(MEditTitle),EditDlg[2].Data,MSG(MEditExists),
                         MSG(MEditOvr),MSG(MYes),MSG(MNo))!=0)
            {
              FarChDir(OldCurDir);
              return(TRUE);
            }
          }
          /* tran $ */

          if(!SetFileName(EditDlg[2].Data))
          {
            if(!NameChanged)
              FarChDir(OldCurDir);
            return FALSE;
          }

          if (EditDlg[5].Selected)
            TextFormat=0;
          if (EditDlg[6].Selected)
            TextFormat=1;
          if (EditDlg[7].Selected)
            TextFormat=2;
          if (EditDlg[8].Selected)
            TextFormat=3;
          if(!NameChanged)
            FarChDir(OldCurDir);
        }
        ShowConsoleTitle();
        FarChDir(StartDir); //???

        if(SaveFile(FullFileName,0,Key==KEY_SHIFTF2 ? TextFormat:0,Key==KEY_SHIFTF2)==0)
        {
          if (Message(MSG_WARNING|MSG_ERRORTYPE,2,MSG(MEditTitle),MSG(MEditCannotSave),
                      FileName,MSG(MRetry),MSG(MCancel))!=0)
          {
            Done=TRUE;
            break;
          }
        }
        else
          Done=TRUE;
      }
      return(TRUE);
    }

    case KEY_F6:
    {
      /* $ 10.05.2001 DJ
         ���������� EnableF6
      */
      if (EnableF6)
      {
        int FirstSave=1, NeedQuestion=1;
        // �������� �� "� ����� ��� ����� ������� ���?"
        // �������� ����� ��� � �� �����!
        // ����, ��� �� ���� ��������, ��
        if(FEdit->IsFileChanged() &&  // � ������� ������ ���� ���������?
           GetFileAttributes(FullFileName) == -1) // � ���� ��� ����������?
        {
          switch(Message(MSG_WARNING,2,MSG(MEditTitle),
                         MSG(MEditSavedChangedNonFile),
                         MSG(MEditSavedChangedNonFile2),
                         MSG(MEditSave),MSG(MCancel)))
          {
            case 0:
              if(ProcessKey(KEY_F2))
              {
                FirstSave=0;
                break;
              }
            default:
              return FALSE;
          }
        }

        if(!FirstSave || FEdit->IsFileChanged() || GetFileAttributes(FullFileName)!=-1)
        {
          long FilePos=FEdit->GetCurPos();
          /* $ 01.02.2001 IS
             ! ��������� ����� � ��������� �������� ����� �����, � �� ���������
          */
          if (ProcessQuitKey(FirstSave,NeedQuestion))
          {
            /* $ 11.10.200 IS
               �� ����� ������� ����, ���� ���� �������� ��������, �� ��� ����
               ������������ ������������ �� �����
            */
            FEdit->SetDeleteOnClose(FALSE);
            /* IS $ */
            /* $ 06.05.2001 DJ
               ��������� F6 ��� NWZ
            */
            /* $ 07.05.2001 DJ
               ��������� NamesList
            */
            FileViewer *Viewer = new FileViewer (FullFileName, GetCanLoseFocus(), FALSE,
               FALSE, FilePos, NULL, EditNamesList, SaveToSaveAs);
            /* DJ $ */
  //OT          FrameManager->InsertFrame (Viewer);
            /* DJ $ */
          }
          /* IS $ */
          ShowTime(2);
        }
        return(TRUE);
      }
      break; // ������� F6 ��������, ���� ���� ������ �� ������������
      /* DJ $ */
    }
    /*$ 21.07.2000 SKV
        + ����� � ����������������� �� ������������� ����� �� CTRLF10
    */
    case KEY_CTRLF10:
    {
      {
        if (isTemporary())
        {
          return(TRUE);
        }
        /* 26.11.2001 VVM
          ! ������������ ������ ��� ����� */
        /* $ 28.12.2001 DJ
           ������� ��� � ����� �������
        */
        if(GetFileAttributes(FullFileName) == -1) // � ��� ���� �� ��� �� �����?
        {
          return FALSE;
        }

        {
          SaveScreen Sc;
          CtrlObject->Cp()->GoToFile (FullFileName);
          RedrawTitle = TRUE;
        }
        /* DJ $ */
        /* VVM $ */
      }
      return (TRUE);
    }
    /* SKV $*/

    case KEY_SHIFTF10:
      if(!ProcessKey(KEY_F2)) // ����� ���� ����, ��� ����� ���������� �� ����������
        return FALSE;
    case KEY_ESC:
    case KEY_F10:
    {
      int FirstSave=1, NeedQuestion=1;
      if(Key != KEY_SHIFTF10)    // KEY_SHIFTF10 �� ���������!
      {
        if(FEdit->IsFileChanged() &&  // � ������� ������ ���� ���������?
           GetFileAttributes(FullFileName) == -1 && !IsNewFile) // � ��� ���� �� ��� �� �����?
        {
          switch(Message(MSG_WARNING,3,MSG(MEditTitle),
                         MSG(MEditSavedChangedNonFile),
                         MSG(MEditSavedChangedNonFile2),
                         MSG(MEditSave),MSG(MEditNotSave),MSG(MEditContinue)))
          {
            case 0:
              if(!ProcessKey(KEY_F2))  // ������� ������� ���������
                NeedQuestion=0;
              FirstSave=0;
              break;
            case 1:
              NeedQuestion=0;
              FirstSave=0;
              break;
            case 2:
            default:
              return FALSE;
          }
        }
      }
      ProcessQuitKey(FirstSave,NeedQuestion);
      return(TRUE);
    }

    /* $ 27.09.2000 SVS
       ������ �����/����� � �������������� ������� PrintMan
    */
    case KEY_ALTF5:
    {
      if(CtrlObject->Plugins.FindPlugin(SYSID_PRINTMANAGER) != -1)
      {
        CtrlObject->Plugins.CallPlugin(SYSID_PRINTMANAGER,OPEN_EDITOR,0); // printman
        return TRUE;
      }
      break; // ������� Alt-F5 �� ����������� ��������, ���� �� ���������� PrintMan
    }
    /* SVS $*/

    /* $ 19.12.2000 SVS
       ����� ������� �������� (� ������ IS)
    */
    case KEY_ALTSHIFTF9:
    {
      /* $ 26.02.2001 IS
           ������ � ��������� ������ EditorOptions
      */
      struct EditorOptions EdOpt;

      EdOpt.TabSize=FEdit->GetTabSize();
      EdOpt.ExpandTabs=FEdit->GetConvertTabs();
      EdOpt.PersistentBlocks=FEdit->GetPersistentBlocks();
      EdOpt.DelRemovesBlocks=FEdit->GetDelRemovesBlocks();
      EdOpt.AutoIndent=FEdit->GetAutoIndent();
      EdOpt.AutoDetectTable=FEdit->GetAutoDetectTable();
      EdOpt.CursorBeyondEOL=FEdit->GetCursorBeyondEOL();
      EdOpt.CharCodeBase=FEdit->GetCharCodeBase();
      FEdit->GetSavePosMode(EdOpt.SavePos, EdOpt.SaveShortPos);
      //EdOpt.BSLikeDel=FEdit->GetBSLikeDel();

      /* $ 27.11.2001 DJ
         Local � EditorConfig
      */
      EditorConfig(EdOpt,1);
      /* DJ $ */
      EditKeyBar.Show(); //???? ����� ��????

      FEdit->SetTabSize(EdOpt.TabSize);
      FEdit->SetConvertTabs(EdOpt.ExpandTabs);
      FEdit->SetPersistentBlocks(EdOpt.PersistentBlocks);
      FEdit->SetDelRemovesBlocks(EdOpt.DelRemovesBlocks);
      FEdit->SetAutoIndent(EdOpt.AutoIndent);
      FEdit->SetAutoDetectTable(EdOpt.AutoDetectTable);
      FEdit->SetCursorBeyondEOL(EdOpt.CursorBeyondEOL);
      FEdit->SetCharCodeBase(EdOpt.CharCodeBase);
      FEdit->SetSavePosMode(EdOpt.SavePos, EdOpt.SaveShortPos);
      //FEdit->SetBSLikeDel(EdOpt.BSLikeDel);
      /* IS $ */
      FEdit->Show();
      return TRUE;
    }
    /* SVS $ */

    /* $ 10.05.2001 DJ
       Alt-F11 - �������� view/edit history
    */
    case KEY_ALTF11:
    {
      if (GetCanLoseFocus())
      {
        CtrlObject->CmdLine->ShowViewEditHistory();
        return TRUE;
      }
      break; // ������� Alt-F11 �� ����������� ��������, ���� �������� ���������
    }
    /* DJ $ */
  }

  // ��� ��������� �������������� ������� ������ �����
  /* $ 28.04.2001 DJ
     �� �������� KEY_MACRO* ������� - ��������� ReadRec � ���� ������
     ����� �� ������������� �������������� �������, ��������� ������������
     �����
  */
  if(Key&KEY_MACROSPEC_BASE) // ��������� MACRO
     return(FEdit->ProcessKey(Key));
  /* DJ $ */
  _KEYMACRO(CleverSysLog SL("FileEditor::ProcessKey()"));
  _KEYMACRO(SysLog("Key=%s Macro.IsExecuting()=%d",_FARKEY_ToName(Key),CtrlObject->Macro.IsExecuting()));
  if (CtrlObject->Macro.IsExecuting() ||
    !ProcessEditorInput(FrameManager->GetLastInputRecord()))
  {
    /* $ 22.03.2001 SVS
       ��� ������� �� ��������� :-)
    */
    if (FullScreen && !CtrlObject->Macro.IsExecuting())
      EditKeyBar.Show();
    /* SVS $ */
    if (!EditKeyBar.ProcessKey(Key))
      return(FEdit->ProcessKey(Key));
  }
  return(TRUE);
}


int FileEditor::ProcessQuitKey(int FirstSave,BOOL NeedQuestion)
{
  char OldCurDir[4096];
  FarGetCurDir(sizeof(OldCurDir),OldCurDir);
  while (1)
  {
    FarChDir(StartDir); // ������? � ����� ��???
    int SaveCode=SAVEFILE_SUCCESS;
    if(NeedQuestion)
    {
      SaveCode=SaveFile(FullFileName,FirstSave,0,FALSE);
    }
    if (SaveCode==SAVEFILE_CANCEL)
      break;
    if (SaveCode==SAVEFILE_SUCCESS)
    {
      FrameManager->DeleteFrame();
      SetExitCode (XC_QUIT);
      break;
    }
    if (Message(MSG_WARNING|MSG_ERRORTYPE,2,MSG(MEditTitle),MSG(MEditCannotSave),
                FileName,MSG(MRetry),MSG(MCancel))!=0)
      break;
    FirstSave=0;
  }
  FarChDir(OldCurDir);
  return GetExitCode() == XC_QUIT;
}


// ���� ������ ���������� ��� �� Editor::ReadFile()
int FileEditor::ReadFile(const char *Name,int &UserBreak)
{
  return FEdit->ReadFile(Name,UserBreak);
}

// ���� ������ ���������� ��� �� Editor::SaveFile()
int FileEditor::SaveFile(const char *Name,int Ask,int TextFormat,int SaveAs)
{
  /* $ 11.10.2000 SVS
     �������������, ��������, ��� ������ - �������� ���� :-(
  */
  if (FEdit->Flags.Check(FEDITOR_LOCKMODE) && !FEdit->Flags.Check(FEDITOR_MODIFIED) && !SaveAs)
    return SAVEFILE_SUCCESS;
  /* SVS $ */

  if (Ask)
  {
    if(!FEdit->Flags.Check(FEDITOR_MODIFIED))
      return SAVEFILE_SUCCESS;

    if (Ask)
    {
      switch (Message(MSG_WARNING,3,MSG(MEditTitle),MSG(MEditAskSave),
              MSG(MEditSave),MSG(MEditNotSave),MSG(MEditContinue)))
      {
        case -1:
        case -2:
        case 2:  // Continue Edit
          return SAVEFILE_CANCEL;
        case 0:  // Save
          break;
        case 1:  // Not Save
          FEdit->TextChanged(0); // 10.08.2000 skv: TextChanged() support;
          return SAVEFILE_SUCCESS;
      }
    }
  }

  int NewFile=TRUE;
  if ((FEdit->FileAttributes=GetFileAttributes(Name))!=-1)
  {
    NewFile=FALSE;
    if (FEdit->FileAttributes & FA_RDONLY)
    {
      int AskOverwrite=Message(MSG_WARNING,2,MSG(MEditTitle),Name,MSG(MEditRO),
                           MSG(MEditOvr),MSG(MYes),MSG(MNo));
      if (AskOverwrite!=0)
        return SAVEFILE_CANCEL;

      SetFileAttributes(Name,FEdit->FileAttributes & ~FA_RDONLY); // ����� ��������
    }

    if (FEdit->FileAttributes & (FA_HIDDEN|FA_SYSTEM))
      SetFileAttributes(Name,0);
  }
  else
  {
    // �������� ���� � �����, ����� ��� ��� ������...
    char CreatedPath[4096];
    char *Ptr=strrchr(strncpy(CreatedPath,Name,sizeof(CreatedPath)-1),'\\'), Chr;
    if(Ptr)
    {
      Chr=*Ptr;
      *Ptr=0;
      DWORD FAttr=0;
      if(GetFileAttributes(CreatedPath) == -1)
      {
        // � ��������� �������.
        // ��� ��
        CreatePath(CreatedPath);
        FAttr=GetFileAttributes(CreatedPath);
      }
      *Ptr=Chr;
      if(FAttr == -1)
        return SAVEFILE_ERROR;
    }
  }

  int Ret=FEdit->SaveFile(Name,Ask,TextFormat,SaveAs);
  IsNewFile=0;

  /* $ 09.02.2002 VVM
    + �������� ������, ���� ������ � ������� ������� */
  if (Ret==SAVEFILE_SUCCESS)
  {
    Panel *ActivePanel = CtrlObject->Cp()->ActivePanel;
    char *FileName = PointToName((char *)Name);
    char FilePath[NM], PanelPath[NM];
    strncpy(FilePath, Name, FileName - Name);
    ActivePanel->GetCurDir(PanelPath);
    AddEndSlash(PanelPath);
    AddEndSlash(FilePath);
    if (!strcmp(PanelPath, FilePath))
      ActivePanel->Update(UPDATE_KEEP_SELECTION);
  }
  /* VVM $ */
  return Ret;
}

int FileEditor::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  if (!EditKeyBar.ProcessMouse(MouseEvent))
    if (!ProcessEditorInput(FrameManager->GetLastInputRecord()))
      if (!FEdit->ProcessMouse(MouseEvent))
        return(FALSE);
  return(TRUE);
}


int FileEditor::GetTypeAndName(char *Type,char *Name)
{
  if ( Type ) strcpy(Type,MSG(MScreensEdit));
  if ( Name ) strcpy(Name,FullFileName);
  return(MODALTYPE_EDITOR);
}


void FileEditor::ShowConsoleTitle()
{
  char Title[NM+20];
  sprintf(Title,MSG(MInEditor),PointToName(FileName));
  SetFarTitle(Title);
  RedrawTitle = FALSE;
}


/* $ 28.06.2000 tran
 (NT Console resize)
 resize editor */
void FileEditor::SetScreenPosition()
{
  if (FullScreen)
  {
    SetPosition(0,0,ScrX,ScrY);
  }
}
/* tran $ */

/* $ 10.05.2001 DJ
   ���������� � view/edit history
*/

void FileEditor::OnDestroy()
{
  _OT(SysLog("[%p] FileEditor::OnDestroy()",this));
  if (!DisableHistory)
    CtrlObject->ViewHistory->AddToHistory(FullFileName,MSG(MHistoryEdit),
                  (FEdit->Flags.Check(FEDITOR_LOCKMODE)?4:1));
  /* $ 19.10.2001 OT
  */
  if (CtrlObject->Plugins.CurEditor==this)//&this->FEdit)
  {
    CtrlObject->Plugins.CurEditor=NULL;
  }
}

int FileEditor::GetCanLoseFocus(int DynamicMode)
{
  if (DynamicMode)
  {
    if (FEdit->IsFileModified())
    {
      return FALSE;
    }
  }
  else
  {
    return CanLoseFocus;
  }
  return TRUE;
}

void FileEditor::SetLockEditor(BOOL LockMode)
{
  if(LockMode)
    FEdit->Flags.Set(FEDITOR_LOCKMODE);
  else
    FEdit->Flags.Skip(FEDITOR_LOCKMODE);
}

int FileEditor::FastHide()
{
  return Opt.AllCtrlAltShiftRule & CASR_EDITOR;
}

BOOL FileEditor::isTemporary()
{
  return (!GetDynamicallyBorn());
}

void FileEditor::ResizeConsole()
{
  FEdit->PrepareResizedConsole();
}

int FileEditor::ProcessEditorInput(INPUT_RECORD *Rec)
{
  int RetCode;
  _KEYMACRO(CleverSysLog SL("FileEditor::ProcessEditorInput()"));
  _KEYMACRO(if(Rec->EventType == KEY_EVENT)SysLog("%cVKey=%s",(Rec->Event.KeyEvent.bKeyDown?0x19:0x18),_VK_KEY_ToName(Rec->Event.KeyEvent.wVirtualKeyCode)));

  CtrlObject->Plugins.CurEditor=this;
  RetCode=CtrlObject->Plugins.ProcessEditorInput(Rec);

  _KEYMACRO(SysLog("RetCode=%d",RetCode));
  return RetCode;
}

void FileEditor::SetPluginTitle(char *PluginTitle)
{
  FEdit->SetPluginTitle(PluginTitle);
}

BOOL FileEditor::SetFileName(const char *NewFileName)
{
  if (ConvertNameToFull(NewFileName,FullFileName, sizeof(FullFileName)) >= sizeof(FullFileName))
  {
    return FALSE;
  }
  strncpy(FileName,NewFileName,sizeof(FileName)-1);
  return TRUE;
}

void FileEditor::SetTitle(const char *Title)
{
  FEdit->SetTitle(NullToEmpty(Title));
}

int FileEditor::EditorControl(int Command,void *Param)
{
#if defined(SYSLOG_KEYMACRO)
  _KEYMACRO(CleverSysLog SL("FileEditor::EditorControl()"));
  if(Command == ECTL_READINPUT || Command == ECTL_PROCESSINPUT)
  {
    _KEYMACRO(SysLog("(Command=%s, Param=[%d/0x%08X]) Macro.IsExecuting()=%d",_ECTL_ToName(Command),(int)Param,Param,CtrlObject->Macro.IsExecuting()));
  }
#else
  _ECTLLOG(CleverSysLog SL("FileEditor::EditorControl()"));
  _ECTLLOG(SysLog("(Command=%s, Param=[%d/0x%08X])",_ECTL_ToName(Command),(int)Param,Param));
#endif
  return FEdit->EditorControl(Command,Param);
}
