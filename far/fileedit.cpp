/*
fileedit.cpp

�������������� ����� - ���������� ��� editor.cpp

*/

/* Revision: 1.142 26.09.2003 $ */

/*
Modify:
  26.09.2003 SVS
    ! ��������� � ��������� �����������
    + ��������� ��������� Ctrl-Q � ��������� ������ - ������ '"'
  15.09.2003 SVS
    ! �������� target �� ������������ �������, ������������� � ReservedFilenameSymbols
    + ���� �� Shift-F2 ���� ������ ��� - �� ������� ������ � �������� ����� � ������.
  04.09.2003 SVS
    ! ������ ������ CompareFileTime() �������� ���� � ������������� ������:
      �������� FILETIME � __int64
  29.07.2003 SVS
    ! ��������� ������ ������ ��������.
    + ECTL_PROCESSKEY _�����_ ����������� � FileEditor::EditorControl()!
  25.07.2003 SVS
    ! �������� SetLastError � ������ �������, ����� ��������� ���������� ���������
      (������������ ����� ���� ������ - DWORD SysErrorCode)
    ! ���������� ����� "#if 0" - �� �������!!! ��� NT! :-)
  15.07.2003 SVS
    + ������� �����
  30.05.2003 SVS
    + ���� :-) Shift-F4 � ���������/������� ��������� ��������� ������ ��������/������
      ���� ����������� (����� �� ����������)
  19.05.2003 SVS
    - BugZ#889 - ������� ������� ������� �� ����� ������ �������.
      ����� �������! ����� ���� ������ ������� � �� �������, ������� �� �������� :-)
  21.04.2003 SVS
    + ������� �����
  14.04.2003 SVS
    - ��������� �������� ��������� �������������� ������� (��������, F1 -
      ����� ������) �� ����� ������ ��������, ������������ ProcessEditorInput
      (��������, AutoWrap)
      ��������� ECTL_PROCESSINPUT � ECTL_READINPUT �� Editor � FileEditor
  31.03.2003 SVS
    + ������� _ECTLLOG ��� ECTL_GETINFO
  05.03.2003 SVS
    ! ���������� _SVS
  18.02.2003 SVS
    ! ����������� ����� ��� ECTL_
  07.02.2003 SVS
    ! � FileEditor::ShowStatus() ��� ������ ��������� ������ ����� �����
      �� ���������� ������� Editor, "����" ����� FileEditor.
  26.01.2003 IS
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  26.12.2002 SVS
    - BugZ#754 - �������� �������� � �������� �2, �2
      �������� ���������� � Init
  23.12.2002 SVS
    + Wish - � LNG-������ ��������� ������� �������� ��� /e � /v
  21.12.2002 SVS
    - ��...  ��������� ������� �������������� �������� BugZ#660 :-)
      �������� ����� ���� UPDATE_DRAW_MESSAGE - "���������� �������
      ����������� ������ ��� ���������� ������ � ������"
  17.12.2002 SVS
    ! ������� ������� ������ � EditorPosCache (see ����� FilePositionCache)
  11.12.2002 SVS
    - BugZ#292 - CtrlF10 �� ����� ������ ���������.
    ! ��������� ���������� ������ �������� �� �����
  10.12.2002 SVS
    - BugZ#720 - far /v file + Ctrl-O
  10.11.2002 SKV
    ! ����� � ����� ����� ������ ���� ������ ��������, ���� �������� ������� � �������� � �.�.
  08.11.2002 SVS
    ! Editor::PluginData ����� � FileEditor::PluginData
    ! Editor::SetPluginData() ����� � FileEditor::SetPluginData()
    ! ��� �� ������� ������� ������� � ���������� ������� FileEditor::UpdateFileList()
    - "The file was changed by an external program"
    ! ��������� ������ �������� Editor �� ������� "����"
  07.11.2002 SVS
    - BugZ#660 - "Reading: %d files" � ��������� ���� ���������
      ������� ��� �� ������� ���������� � ������� ������ �� ���������.
      ���������� ������ �� ������ :-))
  01.10.2002 SVS
    - BugZ#664 - ������ "&" � ������ ��������� � ������� ������
  04.09.2002 SVS
    + "���� ��� ������� ������� ����������"
    ! ����� Editor "�������" �������� ���������� ����� ��������������,
      ������ ��� ���������� FileEditor`�
    ! ������� ECTL_SAVEFILE �������������� � FileEditor::EditorControl
    ! ������� ECTL_QUIT �������������� � FileEditor::EditorControl
    ! ������� ECTL_SETTITLE �������������� � FileEditor::EditorControl
    ! ��� Editor::ShowStatus() �������� � FileEditor::ShowStatus()
  21.08.2002 SVS
    ! ��������� ��� WaitKey
  13.07.2002 SVS
    - �� ������� �� �������� ������� F1.
      ������ ������� ���� ���������!
  12.07.2002 SVS
    ! ��������� "������" ��� "Editor Not File" - ��������� ��������� F1
      �� Editor � FileEditor
  25.06.2002 SVS
    ! ���������:  BitFlags::Skip -> BitFlags::Clear
    ! ������ Editor ����� ������� ������ - ��� ���������� FileEditor
      ������ ECTL_SETKEYBAR �������� �� Editor � FileEditor
  14.06.2002 IS
    ! DeleteOnClose ���� int.
  10.06.2002 SVS
    - BugZ#554 - bug, overwrite local memory
    ! ������� � ���� ����� Dialog (��� ������ ��������)
  04.06.2002 SVS
    - BugZ#545 - Ctrl-F10 ��� ��������� �����
    - BugZ#546 - Editor ����� ���
  30.05.2002 IS
    ! ��������� ����������� ��� �� - ������ ������ strlen
  29.05.2002 SVS
    ! "�� ��������� � �����������" - ����� IsLocalPath() �� ������ ������.
  28.05.2002 SVS
    ! �������� �������  IsLocalPath()
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
#include "poscache.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "dialog.hpp"
#include "fileview.hpp"
#include "help.hpp"
#include "ctrlobj.hpp"
#include "manager.hpp"
#include "namelist.hpp"
#include "history.hpp"
#include "cmdline.hpp"
#include "scrbuf.hpp"
#include "savescr.hpp"


FileEditor::FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
                       int StartLine,int StartChar,int DisableHistory,
                       char *PluginData,int ToSaveAs, int OpenModeExstFile)
{
  _ECTLLOG(CleverSysLog SL("FileEditor::FileEditor(1)"));
  _KEYMACRO(SysLog("FileEditor::FileEditor(1)"));
  _KEYMACRO(SysLog(1));
  ScreenObject::SetPosition(0,0,ScrX,ScrY);
  Flags.Set(FFILEEDIT_FULLSCREEN);
  Init(Name,NULL,CreateNewFile,EnableSwitch,StartLine,StartChar,
       DisableHistory,PluginData,ToSaveAs,FALSE,OpenModeExstFile);
}


FileEditor::FileEditor(const char *Name,int CreateNewFile,int EnableSwitch,
            int StartLine,int StartChar,const char *Title,
            int X1,int Y1,int X2,int Y2,int DisableHistory, int DeleteOnClose,
            int OpenModeExstFile)
{
  _ECTLLOG(CleverSysLog SL("FileEditor::FileEditor(2)"));
  _KEYMACRO(SysLog("FileEditor::FileEditor(2)"));
  _KEYMACRO(SysLog(1));
  /* $ 02.11.2001 IS
       ������������� ���������� ������ �������� ���� ���������� �� �������
  */
  if(X1 < 0)
    X1=0;
  if(X2 < 0 || X2 > ScrX)
    X2=ScrX;
  if(Y1 < 0)
    Y1=0;
  if(Y2 < 0 || Y2 > ScrY)
    Y2=ScrY;
  if(X1 >= X2)
  {
    X1=0;
    X2=ScrX;
  }
  if(Y1 >= Y2)
  {
    Y1=0;
    Y2=ScrY;
  }

  /* IS $ */
  ScreenObject::SetPosition(X1,Y1,X2,Y2);
  Flags.Change(FFILEEDIT_FULLSCREEN,(X1==0 && Y1==0 && X2==ScrX && Y2==ScrY));
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

  if (FEdit->EdOpt.SavePos && CtrlObject!=NULL)
  {
    int ScreenLinePos=FEdit->CalcDistance(FEdit->TopScreen,FEdit->CurLine,-1);
    int CurPos=FEdit->CurLine->EditLine.GetTabCurPos();
    int LeftPos=FEdit->CurLine->EditLine.GetLeftPos();
    char CacheName[NM*3];
    if (*PluginData)
      sprintf(CacheName,"%s%s",PluginData,PointToName(FullFileName));
    else
      strcpy(CacheName,FullFileName);

    unsigned int Table=0;
    if (FEdit->Flags.Check(FEDITOR_TABLECHANGEDBYUSER))
    {
      Table=1;
      if (FEdit->AnsiText)
        Table=2;
      else
        if (FEdit->UseDecodeTable)
          Table=FEdit->TableNum+2;
    }

    if (!FEdit->Flags.Check(FEDITOR_OPENFAILED)) // ����� ���� � ��� �������� :-(
    {
      struct TPosCache32 PosCache={0};
      PosCache.Param[0]=FEdit->NumLine;
      PosCache.Param[1]=ScreenLinePos;
      PosCache.Param[2]=CurPos;
      PosCache.Param[3]=LeftPos;
      PosCache.Param[4]=Table;
      if(Opt.EdOpt.SaveShortPos)
      {
        PosCache.Position[0]=FEdit->SavePos.Line;
        PosCache.Position[1]=FEdit->SavePos.Cursor;
        PosCache.Position[2]=FEdit->SavePos.ScreenLine;
        PosCache.Position[3]=FEdit->SavePos.LeftPos;
      }
      CtrlObject->EditorPosCache->AddPosition(CacheName,&PosCache);
    }
  }

  BitFlags FEditFlags=FEdit->Flags;
  int FEditEditorID=FEdit->EditorID;

  if(FEdit)
    delete FEdit;


  if (!FEditFlags.Check(FEDITOR_OPENFAILED))
  {
    FileEditor *save = CtrlObject->Plugins.CurEditor;
    CtrlObject->Plugins.CurEditor=this;
    CtrlObject->Plugins.ProcessEditorEvent(EE_CLOSE,&FEditEditorID);
    /* $ 11.10.2001 IS
       ������ ���� ������ � ���������, ���� ��� �������� � ����� � ����� ��
       ������ �� ������� � ������ �������.
    */
    /* $ 14.06.2001 IS
       ���� ���������� FEDITOR_DELETEONLYFILEONCLOSE � �������
       FEDITOR_DELETEONCLOSE, �� ������� ������ ����.
    */
    if (FEditFlags.Check(FEDITOR_DELETEONCLOSE|FEDITOR_DELETEONLYFILEONCLOSE) &&
       !FrameManager->CountFramesWithName(FullFileName))
    {
       if(FEditFlags.Check(FEDITOR_DELETEONCLOSE))
         DeleteFileWithFolder(FullFileName);
       else
       {
         SetFileAttributes(FullFileName,0);
         remove(FullFileName);
       }
    }
    /* IS 14.06.2002 $ */
    /* IS 11.10.2001 $ */
    CtrlObject->Plugins.CurEditor = save;
  }

  CurrentEditor=NULL;
  if (EditNamesList)
    delete EditNamesList;

  _KEYMACRO(SysLog(-1));
  _KEYMACRO(SysLog("FileEditor::~FileEditor()"));
}

void FileEditor::Init(const char *Name,const char *Title,int CreateNewFile,int EnableSwitch,
                      int StartLine,int StartChar,int DisableHistory,
                      char *PluginData,int ToSaveAs,int DeleteOnClose,
                      int OpenModeExstFile)
{
  _ECTLLOG(CleverSysLog SL("FileEditor::Init()"));
  _ECTLLOG(SysLog("(Name=%s, Title=%s)",Name,Title));
  SysErrorCode=0;

  FEdit=new Editor;

  if(!FEdit)
  {
    ExitCode=XC_OPEN_ERROR;
    return;
  }

  /* $ 19.02.2001 IS
       � �� ����, ��� ��� ������ ����� GetFileAttributes �� ����������...
  */
  *AttrStr=0;
  /* IS $ */
  CurrentEditor=this;
  FileAttributes=-1;
  *PluginTitle=0;
  SetTitle(Title);
  /* $ 07.05.2001 DJ */
  EditNamesList = NULL;
  KeyBarVisible = TRUE;
  /* DJ $ */
  /* $ 10.05.2001 DJ */
  Flags.Change(FFILEEDIT_DISABLEHISTORY,DisableHistory);
  Flags.Change(FFILEEDIT_ENABLEF6,EnableSwitch);
  /* DJ $ */
  /* $ 17.08.2001 KM
    ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
    ������ ��� ������� F2 ������� ����� ShiftF2.
  */
  Flags.Change(FFILEEDIT_SAVETOSAVEAS,ToSaveAs);
  /* KM $ */

  if (*Name==0)
  {
    ExitCode=XC_OPEN_ERROR;
    return;
  }

  SetPluginData(PluginData);
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
  DWORD FAttr=::GetFileAttributes(Name);
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
    _ECTLLOG(SysLog("Message: %s",MSG(MEditROOpen)));
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
  SetDeleteOnClose(DeleteOnClose);
  int UserBreak;
  /* $ 06.07.2001 IS
     ��� �������� ����� � ���� ��� �� �������� �������� ������� EE_READ, ����
     �� �������� �����������.
  */
  if(FAttr == -1)
    Flags.Set(FFILEEDIT_NEW);

  Flags.Change(FFILEEDIT_ISNEWFILE,CreateNewFile);

  if (!ReadFile(FullFileName,UserBreak))
  {
    if(!CreateNewFile || UserBreak)
    {
      if (UserBreak!=1)
      {
        SetLastError(SysErrorCode);
        Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MEditTitle),MSG(MEditCannotOpen),FileName,MSG(MOk));
        ExitCode=XC_OPEN_ERROR;
      }
      else
      {
        ExitCode=XC_LOADING_INTERRUPTED;
      }
      //FrameManager->DeleteFrame(this); // BugZ#546 - Editor ����� ���!
      CtrlObject->Cp()->Redraw();
      return;
    }
    CtrlObject->Plugins.CurEditor=this;//&FEdit;
    _ECTLLOG(SysLog("call ProcessEditorEvent(EE_READ,NULL) {"));
    CtrlObject->Plugins.ProcessEditorEvent(EE_READ,NULL);
    _ECTLLOG(SysLog("} return From ProcessEditorEvent(EE_READ,NULL)"));
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
// $ 29.06.2000 tran - ������� �������� ���� �������������� ������
void FileEditor::InitKeyBar(void)
{
  int IKeyLabel[2][7][13]=
  {
    // ������� ��������
    {
      /* (empty)   */ {KBL_MAIN,MEditF1,MEditF2,MEditF3,MEditF4,MEditF5,MEditF6,MEditF7,MEditF8,MEditF9,MEditF10,MEditF11,MEditF12},
      /* Shift     */ {KBL_SHIFT,MEditShiftF1,MEditShiftF2,MEditShiftF3,MEditShiftF4,MEditShiftF5,MEditShiftF6,MEditShiftF7,MEditShiftF8,MEditShiftF9,MEditShiftF10,MEditShiftF11,MEditShiftF12},
      /* Alt       */ {KBL_ALT,MEditAltF1,MEditAltF2,MEditAltF3,MEditAltF4,MEditAltF5,MEditAltF6,MEditAltF7,MEditAltF8,MEditAltF9,MEditAltF10,MEditAltF11,MEditAltF12},
      /* Ctrl      */ {KBL_CTRL,MEditCtrlF1,MEditCtrlF2,MEditCtrlF3,MEditCtrlF4,MEditCtrlF5,MEditCtrlF6,MEditCtrlF7,MEditCtrlF8,MEditCtrlF9,MEditCtrlF10,MEditCtrlF11,MEditCtrlF12},
      /* AltShift  */ {KBL_ALTSHIFT,MEditAltShiftF1,MEditAltShiftF2,MEditAltShiftF3,MEditAltShiftF4,MEditAltShiftF5,MEditAltShiftF6,MEditAltShiftF7,MEditAltShiftF8,MEditAltShiftF9,MEditAltShiftF10,MEditAltShiftF11,MEditAltShiftF12},
      /* CtrlShift */ {KBL_CTRLSHIFT,MEditCtrlShiftF1,MEditCtrlShiftF2,MEditCtrlShiftF3,MEditCtrlShiftF4,MEditCtrlShiftF5,MEditCtrlShiftF6,MEditCtrlShiftF7,MEditCtrlShiftF8,MEditCtrlShiftF9,MEditCtrlShiftF10,MEditCtrlShiftF11,MEditCtrlShiftF12},
      /* CtrlAlt   */ {KBL_CTRLALT,MEditCtrlAltF1,MEditCtrlAltF2,MEditCtrlAltF3,MEditCtrlAltF4,MEditCtrlAltF5,MEditCtrlAltF6,MEditCtrlAltF7,MEditCtrlAltF8,MEditCtrlAltF9,MEditCtrlAltF10,MEditCtrlAltF11,MEditCtrlAltF12},
    },
    // ��������� ��������
    {
      /* (empty)   */ {KBL_MAIN,MSingleEditF1,MSingleEditF2,MSingleEditF3,MSingleEditF4,MSingleEditF5,MSingleEditF6,MSingleEditF7,MSingleEditF8,MSingleEditF9,MSingleEditF10,MSingleEditF11,MSingleEditF12},
      /* Shift     */ {KBL_SHIFT,MSingleEditShiftF1,MSingleEditShiftF2,MSingleEditShiftF3,MSingleEditShiftF4,MSingleEditShiftF5,MSingleEditShiftF6,MSingleEditShiftF7,MSingleEditShiftF8,MSingleEditShiftF9,MSingleEditShiftF10,MSingleEditShiftF11,MSingleEditShiftF12},
      /* Alt       */ {KBL_ALT,MSingleEditAltF1,MSingleEditAltF2,MSingleEditAltF3,MSingleEditAltF4,MSingleEditAltF5,MSingleEditAltF6,MSingleEditAltF7,MSingleEditAltF8,MSingleEditAltF9,MSingleEditAltF10,MSingleEditAltF11,MSingleEditAltF12},
      /* Ctrl      */ {KBL_CTRL,MSingleEditCtrlF1,MSingleEditCtrlF2,MSingleEditCtrlF3,MSingleEditCtrlF4,MSingleEditCtrlF5,MSingleEditCtrlF6,MSingleEditCtrlF7,MSingleEditCtrlF8,MSingleEditCtrlF9,MSingleEditCtrlF10,MSingleEditCtrlF11,MSingleEditCtrlF12},
      /* AltShift  */ {KBL_ALTSHIFT,MSingleEditAltShiftF1,MSingleEditAltShiftF2,MSingleEditAltShiftF3,MSingleEditAltShiftF4,MSingleEditAltShiftF5,MSingleEditAltShiftF6,MSingleEditAltShiftF7,MSingleEditAltShiftF8,MSingleEditAltShiftF9,MSingleEditAltShiftF10,MSingleEditAltShiftF11,MSingleEditAltShiftF12},
      /* CtrlShift */ {KBL_CTRLSHIFT,MSingleEditCtrlShiftF1,MSingleEditCtrlShiftF2,MSingleEditCtrlShiftF3,MSingleEditCtrlShiftF4,MSingleEditCtrlShiftF5,MSingleEditCtrlShiftF6,MSingleEditCtrlShiftF7,MSingleEditCtrlShiftF8,MSingleEditCtrlShiftF9,MSingleEditCtrlShiftF10,MSingleEditCtrlShiftF11,MSingleEditCtrlShiftF12},
      /* CtrlAlt   */ {KBL_CTRLALT,MSingleEditCtrlAltF1,MSingleEditCtrlAltF2,MSingleEditCtrlAltF3,MSingleEditCtrlAltF4,MSingleEditCtrlAltF5,MSingleEditCtrlAltF6,MSingleEditCtrlAltF7,MSingleEditCtrlAltF8,MSingleEditCtrlAltF9,MSingleEditCtrlAltF10,MSingleEditCtrlAltF11,MSingleEditCtrlAltF12},
    }
  };
  char *FEditKeys[12];
  int I,J;

  for(I=0; I < 7; ++I)
  {
    for(J=1; J <= 12; ++J)
    {
      FEditKeys[J-1]=MSG(IKeyLabel[Opt.OnlyEditorViewerUsed][I][J]);
    }
    switch(IKeyLabel[Opt.OnlyEditorViewerUsed][I][0])
    {
      case KBL_MAIN:
        if(Flags.Check(FFILEEDIT_SAVETOSAVEAS))
          FEditKeys[2-1]=MSG(MEditShiftF2);
        // $ 10.05.2001 DJ - ������� �� EnableF6 ������ CanLoseFocus
        if(!Flags.Check(FFILEEDIT_ENABLEF6))
          FEditKeys[6-1]="";
        if(!GetCanLoseFocus())
          FEditKeys[12-1]="";
        break;
      case KBL_ALT:
        // $ 17.12.2001 KM  - ���� !GetCanLoseFocus() ����� �� Alt-F11 ������ ������ ������.
        if(!GetCanLoseFocus())
          FEditKeys[11-1]="";
        if(CtrlObject->Plugins.FindPlugin(SYSID_PRINTMANAGER) == -1)
          FEditKeys[5-1]="";
        break;
    }
    EditKeyBar.SetGroup(IKeyLabel[Opt.OnlyEditorViewerUsed][I][0],FEditKeys,sizeof(FEditKeys)/sizeof(FEditKeys[0]));
  }

  EditKeyBar.Show();
  SetKeyBar(&EditKeyBar);
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
  if (Flags.Check(FFILEEDIT_FULLSCREEN))
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
  if (!FEdit->DisableOut)
  {
    if(FEdit->Flags.Check(FEDITOR_ISRESIZEDCONSOLE))
    {
      FEdit->Flags.Clear(FEDITOR_ISRESIZEDCONSOLE);
      CtrlObject->Plugins.CurEditor=this;
      CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE);//EEREDRAW_ALL);
    }
    FEdit->Show();
  }
}


int FileEditor::ProcessKey(int Key)
{
  DWORD FNAttr;
  char *Ptr, Chr;

  _KEYMACRO(CleverSysLog SL("FileEditor::ProcessKey()"));
  _KEYMACRO(SysLog("Key=%s Macro.IsExecuting()=%d",_FARKEY_ToName(Key),CtrlObject->Macro.IsExecuting()));

  if (Flags.Check(FFILEEDIT_REDRAWTITLE) && ((Key & 0x00ffffff) < KEY_END_FKEY))
    ShowConsoleTitle();

  // BugZ#488 - Shift=enter
  if(ShiftPressed && Key == KEY_ENTER && CtrlObject->Macro.IsExecuting() == MACROMODE_NOMACRO)
  {
    Key=KEY_SHIFTENTER;
  }

  // ��� ��������� �������������� ������� ������ �����
  /* $ 28.04.2001 DJ
     �� �������� KEY_MACRO* ������� - ��������� ReadRec � ���� ������
     ����� �� ������������� �������������� �������, ��������� ������������
     �����
  */
  if(Key >= KEY_MACRO_BASE && Key <= KEY_MACRO_ENDBASE || (Key&MCODE_OP_SENDKEY)) // ��������� MACRO
     return(FEdit->ProcessKey(Key));
  /* DJ $ */

  switch(Key)
  {
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

    case KEY_F6:
    {
      /* $ 10.05.2001 DJ
         ���������� EnableF6
      */
      if (Flags.Check(FFILEEDIT_ENABLEF6))
      {
        int FirstSave=1, NeedQuestion=1;
        // �������� �� "� ����� ��� ����� ������� ���?"
        // �������� ����� ��� � �� �����!
        // ����, ��� �� ���� ��������, ��
        if(FEdit->IsFileChanged() &&  // � ������� ������ ���� ���������?
           ::GetFileAttributes(FullFileName) == -1) // � ���� ��� ����������?
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

        if(!FirstSave || FEdit->IsFileChanged() || ::GetFileAttributes(FullFileName)!=-1)
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
            SetDeleteOnClose(0);
            /* IS $ */
            /* $ 06.05.2001 DJ
               ��������� F6 ��� NWZ
            */
            /* $ 07.05.2001 DJ
               ��������� NamesList
            */
            FileViewer *Viewer = new FileViewer (FullFileName, GetCanLoseFocus(), FALSE,
               FALSE, FilePos, NULL, EditNamesList, Flags.Check(FFILEEDIT_SAVETOSAVEAS));
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

#if 1
  BOOL ProcessedNext=TRUE;

  _SVS(if(Key=='n' || Key=='m'))
    _SVS(SysLog("%d Key='%c'",__LINE__,Key));

  if(CtrlObject->Macro.IsRecording() == MACROMODE_RECORDING_COMMON || CtrlObject->Macro.IsExecuting() == MACROMODE_EXECUTING_COMMON || CtrlObject->Macro.GetCurRecord(NULL,NULL) == MACROMODE_NOMACRO)
  {
    _SVS(if(CtrlObject->Macro.IsRecording() == MACROMODE_RECORDING_COMMON || CtrlObject->Macro.IsExecuting() == MACROMODE_EXECUTING_COMMON))
      _SVS(SysLog("%d !!!! CtrlObject->Macro.GetCurRecord(NULL,NULL) != MACROMODE_NOMACRO !!!!",__LINE__));
    ProcessedNext=!ProcessEditorInput(FrameManager->GetLastInputRecord());
  }

  if (ProcessedNext)
#else
  if (//CtrlObject->Macro.IsExecuting() || CtrlObject->Macro.IsRecording() || // ����� �������!
    !ProcessEditorInput(FrameManager->GetLastInputRecord()))
#endif
  {
    _KEYMACRO(SysLog("if (ProcessedNext) => __LINE__=%d",__LINE__));
    switch(Key)
    {
      case KEY_F1:
      {
        Help Hlp ("Editor");
        return(TRUE);
      }

      /* $ 25.04.2001 IS
           ctrl+f - �������� � ������ ������ ��� �������������� �����
      */
      case KEY_CTRLF:
      {
        if (!FEdit->Flags.Check(FEDITOR_LOCKMODE))
        {
          FEdit->Pasting++;
          FEdit->TextChanged(1);
          BOOL IsBlock=FEdit->VBlockStart || FEdit->BlockStart;
          if (!FEdit->EdOpt.PersistentBlocks && IsBlock)
          {
            FEdit->Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
            FEdit->DeleteBlock();
          }
          //AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
          //                CurLine->EditLine.GetCurPos(),UNDO_EDIT);
          char FileName0[NM];
          strncpy(FileName0,FullFileName,sizeof(FileName0)-1);
          FEdit->Paste(FileName0);
          //if (!EdOpt.PersistentBlocks)
          FEdit->UnmarkBlock();
          FEdit->Pasting--;
          FEdit->Show(); //???
        }
        return (TRUE);
      }
      /* IS $ */
      /* $ 24.08.2000 SVS
         + ��������� ������� ������ ���������� �� ������� CtrlAltShift
      */
      case KEY_CTRLO:
      {
        if(!Opt.OnlyEditorViewerUsed)
        {
          /*$ 27.09.2000 skv
            To prevent redraw in macro with Ctrl-O
          */
          FEdit->Hide();
          /* skv$*/
          FrameManager->ShowBackground();
          SetCursorType(FALSE,0);
          WaitKey();
          Show();
        }
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
            if (!(isalpha(FullFileName[0]) && (FullFileName[1]==':') && !FullFileName[2]))
            {
              // � ������? ������� ����������?
              if((FNAttr=::GetFileAttributes(FullFileName)) == -1 ||
                                !(FNAttr&FILE_ATTRIBUTE_DIRECTORY)
                  //|| LocalStricmp(OldCurDir,FullFileName)  // <- ��� ������ ������.
                )
                Flags.Set(FFILEEDIT_SAVETOSAVEAS);
            }
            *Ptr=Chr;
          }

          if(Key == KEY_F2 &&
             (FNAttr=::GetFileAttributes(FullFileName)) != -1 &&
             !(FNAttr&FILE_ATTRIBUTE_DIRECTORY))
              Flags.Clear(FFILEEDIT_SAVETOSAVEAS);

          static int TextFormat=0;
          int NameChanged=FALSE;
          if (Key==KEY_SHIFTF2 || Flags.Check(FFILEEDIT_SAVETOSAVEAS))
          {
            const char *HistoryName="NewEdit";
            static struct DialogData EditDlgData[]=
            {
              /* 0 */ DI_DOUBLEBOX,3,1,72,12,0,0,0,0,(char *)MEditTitle,
              /* 1 */ DI_TEXT,5,2,0,0,0,0,0,0,(char *)MEditSaveAs,
              /* 2 */ DI_EDIT,5,3,70,3,1,(DWORD)HistoryName,DIF_HISTORY|DIF_EDITPATH,0,"",
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
            strncpy(EditDlg[2].Data,(Flags.Check(FFILEEDIT_SAVETOSAVEAS)?FullFileName:FileName),sizeof(EditDlg[2].Data)-1);
            EditDlg[5].Selected=EditDlg[6].Selected=EditDlg[7].Selected=EditDlg[8].Selected=0;
            EditDlg[5+TextFormat].Selected=TRUE;

            {
              Dialog Dlg(EditDlg,sizeof(EditDlg)/sizeof(EditDlg[0]));
              Dlg.SetPosition(-1,-1,76,14);
              Dlg.SetHelp("FileSaveAs");
              Dlg.Process();
              if (Dlg.GetExitCode()!=10 || *EditDlg[2].Data==0)
                return(FALSE);
            }
            /* $ 07.06.2001 IS
               - ���: ����� ������� ������� �������, � ������ ����� �������
            */
            RemoveTrailingSpaces(EditDlg[2].Data);
            Unquote(EditDlg[2].Data);
            /* IS $ */

            NameChanged=LocalStricmp(EditDlg[2].Data,(Flags.Check(FFILEEDIT_SAVETOSAVEAS)?FullFileName:FileName))!=0;
            /* $ 01.08.2001 tran
               ���� ����� ��������� ������ � ������ FileName
               ����������� EditDlg[2].Data */
            if(!NameChanged)
              FarChDir(StartDir); // ������? � ����� ��???

            FNAttr=::GetFileAttributes(EditDlg[2].Data);
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
              SetLastError(ERROR_INVALID_NAME);
              Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MEditTitle),EditDlg[2].Data,MSG(MOk));
              if(!NameChanged)
                FarChDir(OldCurDir);
              continue;
              //return FALSE;
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

          if(SaveFile(FullFileName,0,Key==KEY_SHIFTF2 ? TextFormat:0,Key==KEY_SHIFTF2) == SAVEFILE_ERROR)
          {
            SetLastError(SysErrorCode);
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

#if 1
      /* $ 30.05.2003 SVS
         ���� :-) Shift-F4 � ���������/������� ��������� ��������� ������ ��������/������
         ���� �����������
      */
      case KEY_SHIFTF4:
      {
        if(!Opt.OnlyEditorViewerUsed)
          CtrlObject->Cp()->ActivePanel->ProcessKey(Key);
        return TRUE;
      }
      /* $ SVS */
#endif

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

          char FullFileNameTemp[NM*2];
          strcpy(FullFileNameTemp,FullFileName);
          /* 26.11.2001 VVM
            ! ������������ ������ ��� ����� */
          /* $ 28.12.2001 DJ
             ������� ��� � ����� �������
          */
          if(::GetFileAttributes(FullFileName) == -1) // � ��� ���� �� ��� �� �����?
          {
            if(!CheckShortcutFolder(FullFileNameTemp,sizeof(FullFileNameTemp)-1,FALSE))
              return FALSE;
            strcat(FullFileNameTemp,"\\."); // ��� ���������� ������ :-)
          }

          if(Flags.Check(FFILEEDIT_NEW))
          {
            UpdateFileList();
            Flags.Clear(FFILEEDIT_NEW);
          }

          {
            SaveScreen Sc;
            CtrlObject->Cp()->GoToFile (FullFileNameTemp);
            Flags.Set(FFILEEDIT_REDRAWTITLE);
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
             ::GetFileAttributes(FullFileName) == -1 && !Flags.Check(FFILEEDIT_ISNEWFILE)) // � ��� ���� �� ��� �� �����?
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

      default:
      {
        _KEYMACRO(SysLog("default: __LINE__=%d",__LINE__));
        /* $ 22.03.2001 SVS
           ��� ������� �� ��������� :-)
        */
        if (Flags.Check(FFILEEDIT_FULLSCREEN) && CtrlObject->Macro.IsExecuting() == MACROMODE_NOMACRO)
          EditKeyBar.Show();
        /* SVS $ */
        if (!EditKeyBar.ProcessKey(Key))
          return(FEdit->ProcessKey(Key));
      }
    }
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
      /* $ 09.02.2002 VVM
        + �������� ������, ���� ������ � ������� ������� */
      if (NeedQuestion)
      {
        UpdateFileList();
      }
      /* VVM $ */

      FrameManager->DeleteFrame();
      SetExitCode (XC_QUIT);
      break;
    }
    SetLastError(SysErrorCode);
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
  int Ret=FEdit->ReadFile(Name,UserBreak);
  SysErrorCode=GetLastError();
  GetLastInfo(Name,&FileInfo);
  return Ret;
}

BOOL FileEditor::GetLastInfo(const char *Name,WIN32_FIND_DATA *FInfo)
{
  if(FInfo)
  {
    HANDLE FindHandle;
    if ((FindHandle=FindFirstFile(Name,FInfo))!=INVALID_HANDLE_VALUE)
    {
      FindClose(FindHandle);
      return TRUE;
    }
    else
      memset(FInfo,0,sizeof(WIN32_FIND_DATA));
  }
  return FALSE;
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
  if ((FileAttributes=::GetFileAttributes(Name))!=-1)
  {
    // �������� ������� �����������...
    WIN32_FIND_DATA FInfo;
    if(GetLastInfo(Name,&FInfo) && *FileInfo.cFileName)
    {
      __int64 RetCompare=*(__int64*)&FileInfo.ftLastWriteTime - *(__int64*)&FInfo.ftLastWriteTime;
      if(RetCompare || !(FInfo.nFileSizeHigh == FileInfo.nFileSizeHigh && FInfo.nFileSizeLow  == FInfo.nFileSizeLow))
      {
        SetMessageHelp("WarnEditorSavedEx");
        switch (Message(MSG_WARNING,3,MSG(MEditTitle),MSG(MEditAskSaveExt),
                MSG(MEditSave),MSG(MEditBtnSaveAs),MSG(MEditContinue)))
        {
          case -1:
          case -2:
          case 2:  // Continue Edit
            return SAVEFILE_CANCEL;
          case 1:  // Save as
            if(ProcessKey(KEY_SHIFTF2))
              return SAVEFILE_SUCCESS;
            else
              return SAVEFILE_CANCEL;
          case 0:  // Save
            break;
        }
      }
    }

    NewFile=FALSE;
    if (FileAttributes & FA_RDONLY)
    {
      int AskOverwrite=Message(MSG_WARNING,2,MSG(MEditTitle),Name,MSG(MEditRO),
                           MSG(MEditOvr),MSG(MYes),MSG(MNo));
      if (AskOverwrite!=0)
        return SAVEFILE_CANCEL;

      SetFileAttributes(Name,FileAttributes & ~FA_RDONLY); // ����� ��������
    }

    if (FileAttributes & (FA_HIDDEN|FA_SYSTEM))
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
      if(::GetFileAttributes(CreatedPath) == -1)
      {
        // � ��������� �������.
        // ��� ��
        CreatePath(CreatedPath);
        FAttr=::GetFileAttributes(CreatedPath);
      }
      *Ptr=Chr;
      if(FAttr == -1)
        return SAVEFILE_ERROR;
    }
  }

  //int Ret=FEdit->SaveFile(Name,Ask,TextFormat,SaveAs);
  // ************************************
  /* $ 12.02.2001 IS
       ������� ��������� FileAttr �� FileAttributes
  */
  /* $ 04.06.2001 IS
       ������ (� ������ SVS) ������������� ���� - ����� �� ������� ��� �� ����,
       ��� ������������� �������� �����
  */
  int RetCode=SAVEFILE_SUCCESS;

  if (TextFormat!=0)
    FEdit->Flags.Set(FEDITOR_WASCHANGED);

  switch(TextFormat)
  {
    case 1:
      strcpy(FEdit->GlobalEOL,DOS_EOL_fmt);
      break;
    case 2:
      strcpy(FEdit->GlobalEOL,UNIX_EOL_fmt);
      break;
    case 3:
      strcpy(FEdit->GlobalEOL,MAC_EOL_fmt);
      break;
  }

  if(::GetFileAttributes(Name) == -1)
    Flags.Set(FFILEEDIT_NEW);

  {
    FILE *EditFile;
    //SaveScreen SaveScr;
    /* $ 11.10.2001 IS
       ���� ���� ����������� ���������� � ����� �����������, �� �� ������� ����
    */
    FEdit->Flags.Clear(FEDITOR_DELETEONCLOSE|FEDITOR_DELETEONLYFILEONCLOSE);
    /* IS $ */
    CtrlObject->Plugins.CurEditor=this;
//_D(SysLog("%08d EE_SAVE",__LINE__));
    CtrlObject->Plugins.ProcessEditorEvent(EE_SAVE,NULL);

    DWORD FileFlags=FILE_ATTRIBUTE_ARCHIVE|FILE_FLAG_SEQUENTIAL_SCAN;
    if (FileAttributes!=-1 && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
      FileFlags|=FILE_FLAG_POSIX_SEMANTICS;

    HANDLE hEdit=FAR_CreateFile(Name,GENERIC_WRITE,FILE_SHARE_READ,NULL,
                 FileAttributes!=-1 ? TRUNCATE_EXISTING:CREATE_ALWAYS,FileFlags,NULL);
    if (hEdit==INVALID_HANDLE_VALUE && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT && FileAttributes!=-1)
    {
      //_SVS(SysLogLastError();SysLog("Name='%s',FileAttributes=%d",Name,FileAttributes));
      hEdit=FAR_CreateFile(Name,GENERIC_WRITE,FILE_SHARE_READ,NULL,TRUNCATE_EXISTING,
                       FILE_ATTRIBUTE_ARCHIVE|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    }
    if (hEdit==INVALID_HANDLE_VALUE)
    {
      //_SVS(SysLogLastError();SysLog("Name='%s',FileAttributes=%d",Name,FileAttributes));
      RetCode=SAVEFILE_ERROR;
      SysErrorCode=GetLastError();
      goto end;
    }
    int EditHandle=_open_osfhandle((long)hEdit,O_BINARY);
    if (EditHandle==-1)
    {
      RetCode=SAVEFILE_ERROR;
      SysErrorCode=GetLastError();
      goto end;
    }
    if ((EditFile=fdopen(EditHandle,"wb"))==NULL)
    {
      RetCode=SAVEFILE_ERROR;
      SysErrorCode=GetLastError();
      goto end;
    }

    FEdit->UndoSavePos=FEdit->UndoDataPos;
    FEdit->Flags.Clear(FEDITOR_UNDOOVERFLOW);

//    ConvertNameToFull(Name,FileName, sizeof(FileName));
/*
    if (ConvertNameToFull(Name,FEdit->FileName, sizeof(FEdit->FileName)) >= sizeof(FEdit->FileName))
    {
      FEdit->Flags.Set(FEDITOR_OPENFAILED);
      RetCode=SAVEFILE_ERROR;
      goto end;
    }
*/
    SetCursorType(FALSE,0);
    SetPreRedrawFunc(Editor::PR_EditorShowMsg);
    Editor::EditorShowMsg(MSG(MEditTitle),MSG(MEditSaving),Name);

    struct EditList *CurPtr=FEdit->TopList;

    while (CurPtr!=NULL)
    {
      const char *SaveStr, *EndSeq;
      int Length;
      CurPtr->EditLine.GetBinaryString(SaveStr,&EndSeq,Length);
      if (*EndSeq==0 && CurPtr->Next!=NULL)
        EndSeq=*FEdit->GlobalEOL ? FEdit->GlobalEOL:DOS_EOL_fmt;
      if (TextFormat!=0 && *EndSeq!=0)
      {
        if (TextFormat==1)
          EndSeq=DOS_EOL_fmt;
        else if (TextFormat==2)
          EndSeq=UNIX_EOL_fmt;
        else
          EndSeq=MAC_EOL_fmt;
        CurPtr->EditLine.SetEOL(EndSeq);
      }
      int EndLength=strlen(EndSeq);
      if (fwrite(SaveStr,1,Length,EditFile)!=Length ||
          fwrite(EndSeq,1,EndLength,EditFile)!=EndLength)
      {
        fclose(EditFile);
        remove(Name);
        RetCode=SAVEFILE_ERROR;
        goto end;
      }
      CurPtr=CurPtr->Next;
    }
    if (fflush(EditFile)==EOF)
    {
      fclose(EditFile);
      remove(Name);
      RetCode=SAVEFILE_ERROR;
      goto end;
    }
    SetEndOfFile(hEdit);
    fclose(EditFile);
  }

end:
  SetPreRedrawFunc(NULL);

  if (FileAttributes!=-1)
    SetFileAttributes(Name,FileAttributes|FA_ARCH);
  GetLastInfo(FullFileName,&FileInfo);

  if (FEdit->Flags.Check(FEDITOR_MODIFIED) || NewFile)
    FEdit->Flags.Set(FEDITOR_WASCHANGED);

  /* ���� ����� ���������������� � ��� ������, ���� ����� �����, ���
     ��� ���� ���� ��� ������� � �� ��� ���������� ��� ����� ������...
     ...�� "�����" ������ ���� �����.
  */
//  if(SaveAs)
//    Flags.Clear(FEDITOR_LOCKMODE);


  /*$ 10.08.2000 skv
    Modified->TextChanged
  */
  /* 28.12.2001 VVM
    ! ��������� �� �������� ������ */
  if (RetCode==SAVEFILE_SUCCESS)
    FEdit->TextChanged(0);
  /* VVM $ */
  /* skv$*/
  Show();
  /* IS $ */
  /* IS $ */

  // ************************************
  Flags.Clear(FFILEEDIT_ISNEWFILE);

  return RetCode;
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
  Flags.Clear(FFILEEDIT_REDRAWTITLE);
}


/* $ 28.06.2000 tran
 (NT Console resize)
 resize editor */
void FileEditor::SetScreenPosition()
{
  if (Flags.Check(FFILEEDIT_FULLSCREEN))
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
  if (!Flags.Check(FFILEEDIT_DISABLEHISTORY))
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
    FEdit->Flags.Clear(FEDITOR_LOCKMODE);
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
  _KEYMACRO(if(Rec->EventType == KEY_EVENT)          SysLog("KEY_EVENT:          %cVKey=%s",(Rec->Event.KeyEvent.bKeyDown?0x19:0x18),_VK_KEY_ToName(Rec->Event.KeyEvent.wVirtualKeyCode)));
  _KEYMACRO(if(Rec->EventType == FARMACRO_KEY_EVENT) SysLog("FARMACRO_KEY_EVENT: %cVKey=%s",(Rec->Event.KeyEvent.bKeyDown?0x19:0x18),_VK_KEY_ToName(Rec->Event.KeyEvent.wVirtualKeyCode)));

  CtrlObject->Plugins.CurEditor=this;
  RetCode=CtrlObject->Plugins.ProcessEditorInput(Rec);

  _KEYMACRO(SysLog("RetCode=%d",RetCode));
  return RetCode;
}

void FileEditor::SetPluginTitle(const char *PluginTitle)
{
  strcpy(FileEditor::PluginTitle,NullToEmpty(PluginTitle));
}

BOOL FileEditor::SetFileName(const char *NewFileName)
{
  if (strpbrk(NewFileName,ReservedFilenameSymbols) ||
      ConvertNameToFull(NewFileName,FullFileName, sizeof(FullFileName)) >= sizeof(FullFileName))
  {
    return FALSE;
  }
  /* $ 10.11.2002 SKV
    ���� �������� �������, �������� �������...
  */
  for(char* fn=FullFileName;*fn;fn++)
  {
    if(*fn=='/')*fn='\\';
  }
  /* SKV $ */
  strncpy(FileName,NewFileName,sizeof(FileName)-1);
  return TRUE;
}

void FileEditor::SetTitle(const char *Title)
{
  if (Title==NULL)
    *FileEditor::Title=0;
  else
  /* $ 08.06.2001
     - ���: �� ���������� ������ Title, ��� ��������� � ����� ������ �
       � ������� ����.
  */
    strncpy(FileEditor::Title, Title, sizeof(FileEditor::Title)-1);
  /* IS $ */
}

void FileEditor::ChangeEditKeyBar()
{
  if (FEdit->AnsiText)
    EditKeyBar.Change(MSG(Opt.OnlyEditorViewerUsed?MSingleEditF8DOS:MEditF8DOS),7);
  else
    EditKeyBar.Change(MSG(Opt.OnlyEditorViewerUsed?MSingleEditF8:MEditF8),7);

  EditKeyBar.Redraw();
}

void FileEditor::ShowStatus()
{
  if (FEdit->DisableOut)
    return;
  SetColor(COL_EDITORSTATUS);
  GotoXY(X1,Y1); //??
  char TruncFileName[NM],StatusStr[NM],LineStr[50];
  /* $ 08.06.2001 IS
     - ���: ��������� ����, ������ ���, ��������, ������ Title ������,
       ��� ������ TruncFileName
  */
  strncpy(TruncFileName,*PluginTitle ? PluginTitle:(*Title ? Title:FullFileName),sizeof(TruncFileName)-1);
  /* IS $ */
  int NameLength=Opt.ViewerEditorClock && Flags.Check(FFILEEDIT_FULLSCREEN) ? 19:25;
  /* $ 11.07.2000 tran
     + expand filename if console more when 80 column */
  if (X2>80)
     NameLength+=(X2-80);
  /* tran 11.07.2000 $ */

  if (*PluginTitle || *Title)
    /* $ 20.09.2000 SVS
      - Bugs � "�������" ��������� (�� �������) �� �� ������!
    */
    /* $ 01.10.2000 IS
      ! ���������� ����� ����� � ��������� ������
    */
    TruncPathStr(TruncFileName,(ObjWidth<NameLength?ObjWidth:NameLength));
    /* IS $ */
    /* SVS $ */
  else
    /* $ 01.10.2000 IS
      ! ���������� ����� ����� � ��������� ������
    */
    TruncPathStr(TruncFileName,NameLength);
    /* IS $ */
  /* $ 14.02.2000 SVS
     ������������ ������ ��� ���������� �����
  */
  // ��������������� ������.
  sprintf(LineStr,"%d/%d",FEdit->NumLastLine,FEdit->NumLastLine);
  int SizeLineStr=strlen(LineStr);
  if(SizeLineStr > 12)
    NameLength-=(SizeLineStr-12);
  else
    SizeLineStr=12;

  sprintf(LineStr,"%d/%d",FEdit->NumLine+1,FEdit->NumLastLine);
  /* $ 13.02.2001 IS
    ! ���������� ��� ������� AttrStr, ������� ������������ �
      GetFileAttributes
  */
  char *TableName;
  char TmpTableName[32];
  if(FEdit->UseDecodeTable)
  {
    strncpy(TmpTableName,FEdit->TableSet.TableName,sizeof(TmpTableName));
    TableName=RemoveChar(TmpTableName,'&',TRUE);
  }
  else
    TableName=FEdit->AnsiText ? "Win":"DOS";

  sprintf(StatusStr,"%-*s %c%c%c%10.10s %7s %*.*s %5s %-4d %3s",
          NameLength,TruncFileName,(FEdit->Flags.Check(FEDITOR_MODIFIED) ? '*':' '),
          (FEdit->Flags.Check(FEDITOR_LOCKMODE) ? '-':' '),
          (FEdit->Flags.Check(FEDITOR_PROCESSCTRLQ) ? '"':' '),
          TableName,
          MSG(MEditStatusLine),SizeLineStr,SizeLineStr,LineStr,
          MSG(MEditStatusCol),FEdit->CurLine->EditLine.GetTabCurPos()+1,AttrStr);
  /* IS $ */
  /* SVS $ */
  int StatusWidth=ObjWidth - (Opt.ViewerEditorClock && Flags.Check(FFILEEDIT_FULLSCREEN)?5:0);
  if (StatusWidth<0)
    StatusWidth=0;
  mprintf("%-*.*s",StatusWidth,StatusWidth,StatusStr);

  {
    const char *Str;
    int Length;
    FEdit->CurLine->EditLine.GetBinaryString(Str,NULL,Length);
    int CurPos=FEdit->CurLine->EditLine.GetCurPos();
    if (CurPos<Length)
    {
      GotoXY(X2-(Opt.ViewerEditorClock && Flags.Check(FFILEEDIT_FULLSCREEN) ? 9:2),Y1);
      SetColor(COL_EDITORSTATUS);
      /* $ 27.02.2001 SVS
      ���������� � ����������� �� ���� */
      static char *FmtCharCode[3]={"%03o","%3d","%02Xh"};
      mprintf(FmtCharCode[FEdit->EdOpt.CharCodeBase%3],(unsigned char)Str[CurPos]);
      /* SVS $ */
    }
  }
  if (Opt.ViewerEditorClock && Flags.Check(FFILEEDIT_FULLSCREEN))
    ShowTime(FALSE);
}

/* $ 13.02.2001
     ������ �������� ����� � ������ ���������� ������� ������ ��������� ���
     �������.
*/
DWORD FileEditor::GetFileAttributes(LPCTSTR Name)
{
  FileAttributes=::GetFileAttributes(Name);
  int ind=0;
  if(0xFFFFFFFF!=FileAttributes)
  {
     if(FileAttributes&FILE_ATTRIBUTE_READONLY) AttrStr[ind++]='R';
     if(FileAttributes&FILE_ATTRIBUTE_SYSTEM) AttrStr[ind++]='S';
     if(FileAttributes&FILE_ATTRIBUTE_HIDDEN) AttrStr[ind++]='H';
  }
  AttrStr[ind]=0;
  return FileAttributes;
}
/* IS $ */

/* Return TRUE - ������ �������
*/
BOOL FileEditor::UpdateFileList()
{
  Panel *ActivePanel = CtrlObject->Cp()->ActivePanel;
  char *FileName = PointToName((char *)FullFileName);
  char FilePath[NM], PanelPath[NM];
  strncpy(FilePath, FullFileName, FileName - FullFileName);
  ActivePanel->GetCurDir(PanelPath);
  AddEndSlash(PanelPath);
  AddEndSlash(FilePath);
  if (!strcmp(PanelPath, FilePath))
  {
    ActivePanel->Update(UPDATE_KEEP_SELECTION|UPDATE_DRAW_MESSAGE);
    return TRUE;
  }
  return FALSE;
}

void FileEditor::SetPluginData(char *PluginData)
{
  strcpy(FileEditor::PluginData,NullToEmpty(PluginData));
}

/* $ 14.06.2002 IS
   DeleteOnClose ���� int:
     0 - �� ������� ������
     1 - ������� ���� � �������
     2 - ������� ������ ����
*/
void FileEditor::SetDeleteOnClose(int NewMode)
{
  FEdit->Flags.Clear(FEDITOR_DELETEONCLOSE|FEDITOR_DELETEONLYFILEONCLOSE);
  if(NewMode==1)
    FEdit->Flags.Set(FEDITOR_DELETEONCLOSE);
  else if(NewMode==2)
    FEdit->Flags.Set(FEDITOR_DELETEONLYFILEONCLOSE);
}
/* IS $ */

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
  switch(Command)
  {
    case ECTL_GETINFO:
    {
      FEdit->EditorControl(Command,Param);
      struct EditorInfo *Info=(struct EditorInfo *)Param;
      Info->FileName=FullFileName;
      _ECTLLOG(SysLog("struct EditorInfo{"));
      _ECTLLOG(SysLog("  EditorID       = %d",Info->EditorID));
      _ECTLLOG(SysLog("  FileName       = '%s'",Info->FileName));
      _ECTLLOG(SysLog("  WindowSizeX    = %d",Info->WindowSizeX));
      _ECTLLOG(SysLog("  WindowSizeY    = %d",Info->WindowSizeY));
      _ECTLLOG(SysLog("  TotalLines     = %d",Info->TotalLines));
      _ECTLLOG(SysLog("  CurLine        = %d",Info->CurLine));
      _ECTLLOG(SysLog("  CurPos         = %d",Info->CurPos));
      _ECTLLOG(SysLog("  CurTabPos;     = %d",Info->CurTabPos));
      _ECTLLOG(SysLog("  TopScreenLine  = %d",Info->TopScreenLine));
      _ECTLLOG(SysLog("  LeftPos        = %d",Info->LeftPos));
      _ECTLLOG(SysLog("  Overtype       = %d",Info->Overtype));
      _ECTLLOG(SysLog("  BlockType      = %s (%d)",(Info->BlockType==BTYPE_NONE?"BTYPE_NONE":((Info->BlockType==BTYPE_STREAM?"BTYPE_STREAM":((Info->BlockType==BTYPE_COLUMN?"BTYPE_COLUMN":"BTYPE_?????"))))),Info->BlockType));
      _ECTLLOG(SysLog("  BlockStartLine = %d",Info->BlockStartLine));
      _ECTLLOG(SysLog("  AnsiMode       = %d",Info->AnsiMode));
      _ECTLLOG(SysLog("  TableNum       = %d",Info->TableNum));
      _ECTLLOG(SysLog("  Options        = 0x%08X",Info->Options));
      _ECTLLOG(SysLog("  TabSize        = %d",Info->TabSize));
      _ECTLLOG(SysLog("  BookMarkCount  = %d",Info->BookMarkCount));
      _ECTLLOG(SysLog("  CurState       = 0x%08X",Info->CurState));
      _ECTLLOG(SysLog("}"));
      return TRUE;
    }

    case ECTL_GETBOOKMARKS:
    {
      if(!FEdit->Flags.Check(FEDITOR_OPENFAILED) && Param)
      {
        struct EditorBookMarks *ebm=(struct EditorBookMarks *)Param;
        if(ebm->Line && !IsBadWritePtr(ebm->Line,BOOKMARK_COUNT*sizeof(long)))
          memcpy(ebm->Line,FEdit->SavePos.Line,BOOKMARK_COUNT*sizeof(long));
        if(ebm->Cursor && !IsBadWritePtr(ebm->Cursor,BOOKMARK_COUNT*sizeof(long)))
          memcpy(ebm->Cursor,FEdit->SavePos.Cursor,BOOKMARK_COUNT*sizeof(long));
        if(ebm->ScreenLine && !IsBadWritePtr(ebm->ScreenLine,BOOKMARK_COUNT*sizeof(long)))
          memcpy(ebm->ScreenLine,FEdit->SavePos.ScreenLine,BOOKMARK_COUNT*sizeof(long));
        if(ebm->LeftPos && !IsBadWritePtr(ebm->LeftPos,BOOKMARK_COUNT*sizeof(long)))
          memcpy(ebm->LeftPos,FEdit->SavePos.LeftPos,BOOKMARK_COUNT*sizeof(long));
        return TRUE;
      }
      return FALSE;
    }

    case ECTL_SETTITLE:
    {
      // $ 08.06.2001 IS - ���: �� ���������� ������ PluginTitle
      _ECTLLOG(SysLog("Title='%s'",Param));
      strncpy(PluginTitle,NullToEmpty((char *)Param),sizeof(PluginTitle)-1);
      ShowStatus();
      ScrBuf.Flush();
      return TRUE;
    }

    case ECTL_EDITORTOOEM:
    {
      if(!Param)
        return FALSE;
      struct EditorConvertText *ect=(struct EditorConvertText *)Param;
      _ECTLLOG(SysLog("struct EditorConvertText{"));
      _ECTLLOG(SysLog("  Text       ='%s'",ect->Text));
      _ECTLLOG(SysLog("  TextLength =%d",ect->TextLength));
      _ECTLLOG(SysLog("}"));
      if (FEdit->UseDecodeTable && ect->Text)
      {
        DecodeString(ect->Text,(unsigned char *)FEdit->TableSet.DecodeTable,ect->TextLength);
        _ECTLLOG(SysLog("DecodeString -> ect->Text='%s'",ect->Text));
      }
      return TRUE;
    }

    case ECTL_OEMTOEDITOR:
    {
      if(!Param)
        return FALSE;

      struct EditorConvertText *ect=(struct EditorConvertText *)Param;
      _ECTLLOG(SysLog("struct EditorConvertText{"));
      _ECTLLOG(SysLog("  Text       ='%s'",ect->Text));
      _ECTLLOG(SysLog("  TextLength =%d",ect->TextLength));
      _ECTLLOG(SysLog("}"));
      if (FEdit->UseDecodeTable && ect->Text)
      {
        EncodeString(ect->Text,(unsigned char *)FEdit->TableSet.EncodeTable,ect->TextLength);
        _ECTLLOG(SysLog("EncodeString -> ect->Text='%s'",ect->Text));
      }
      return TRUE;
    }

    case ECTL_REDRAW:
    {
      FileEditor::DisplayObject();
      ScrBuf.Flush();
      return(TRUE);
    }

    /* $ 07.08.2000 SVS
       ������� ��������� Keybar Labels
         Param = NULL - ������������, ����. ��������
         Param = -1   - �������� ������ (������������)
         Param = KeyBarTitles
    */
    // ������ ����������� � FileEditor::EditorControl()
    case ECTL_SETKEYBAR:
    {
      struct KeyBarTitles *Kbt=(struct KeyBarTitles*)Param;
      if(!Kbt)
      {        // ������������ ���� ��������!
        InitKeyBar();
      }
      else
      {
        if((long)Param != (long)-1) // �� ������ ������������?
        {
          for(int I=0; I < 12; ++I)
          {
            if(Kbt->Titles[I])
              EditKeyBar.Change(KBL_MAIN,Kbt->Titles[I],I);
            if(Kbt->CtrlTitles[I])
              EditKeyBar.Change(KBL_CTRL,Kbt->CtrlTitles[I],I);
            if(Kbt->AltTitles[I])
              EditKeyBar.Change(KBL_ALT,Kbt->AltTitles[I],I);
            if(Kbt->ShiftTitles[I])
              EditKeyBar.Change(KBL_SHIFT,Kbt->ShiftTitles[I],I);
            if(Kbt->CtrlShiftTitles[I])
              EditKeyBar.Change(KBL_CTRLSHIFT,Kbt->CtrlShiftTitles[I],I);
            if(Kbt->AltShiftTitles[I])
              EditKeyBar.Change(KBL_ALTSHIFT,Kbt->AltShiftTitles[I],I);
            if(Kbt->CtrlAltTitles[I])
              EditKeyBar.Change(KBL_CTRLALT,Kbt->CtrlAltTitles[I],I);
          }
        }
        EditKeyBar.Show();
      }
      return TRUE;
    }
    /* SVS $ */

    case ECTL_SAVEFILE:
    {
      EditorSaveFile *esf=(EditorSaveFile *)Param;
      char *Name=FullFileName;
      int EOL=0;
      if (esf!=NULL)
      {
        _ECTLLOG(char *LinDump=(esf->FileEOL?(char *)_SysLog_LinearDump(esf->FileEOL,strlen(esf->FileEOL)):NULL));
        _ECTLLOG(SysLog("struct EditorSaveFile{"));
        _ECTLLOG(SysLog("  FileName   ='%s'",esf->FileName));
        _ECTLLOG(SysLog("  FileEOL    ='%s'",(esf->FileEOL?LinDump:"(null)")));
        _ECTLLOG(SysLog("}"));
        _ECTLLOG(if(LinDump)xf_free(LinDump));

        if (*esf->FileName)
          Name=esf->FileName;
        if (esf->FileEOL!=NULL)
        {
          if (strcmp(esf->FileEOL,DOS_EOL_fmt)==0)
            EOL=1;
          if (strcmp(esf->FileEOL,UNIX_EOL_fmt)==0)
            EOL=2;
          if (strcmp(esf->FileEOL,MAC_EOL_fmt)==0)
            EOL=3;
        }
        _ECTLLOG(SysLog("EOL=%d",EOL));
      }
      return SaveFile(Name,FALSE,EOL,!LocalStricmp(Name,FullFileName));
    }

    case ECTL_QUIT:
    {
      FrameManager->DeleteFrame(this);
      SetExitCode(SAVEFILE_ERROR); // ���-�� ���� ������� ������� �������� ...???
      return(TRUE);
    }

    case ECTL_READINPUT:
    {
      _KEYMACRO(CleverSysLog SL("FileEditor::EditorControl(ECTL_READINPUT)"));

      if(CtrlObject->Macro.IsRecording() == MACROMODE_RECORDING || CtrlObject->Macro.IsExecuting() == MACROMODE_EXECUTING)
      {
        _KEYMACRO(SysLog("%d if(CtrlObject->Macro.IsRecording() == MACROMODE_RECORDING || CtrlObject->Macro.IsExecuting() == MACROMODE_EXECUTING)",__LINE__));
//        return FALSE;
      }

      if(!Param)
      {
        _ECTLLOG(SysLog("Param = NULL"));
        return FALSE;
      }
      else
      {
        INPUT_RECORD *rec=(INPUT_RECORD *)Param;
        DWORD Key=GetInputRecord(rec);
        //if(Key==KEY_CONSOLE_BUFFER_RESIZE) //????
        //  Show();                          //????
#if defined(SYSLOG_KEYMACRO)
        if(rec->EventType == KEY_EVENT)
        {
          SysLog("ECTL_READINPUT={%s,{%d,%d,Vk=0x%04X,0x%08X}}",
                             (rec->EventType == FARMACRO_KEY_EVENT?"FARMACRO_KEY_EVENT":"KEY_EVENT"),
                             rec->Event.KeyEvent.bKeyDown,
                             rec->Event.KeyEvent.wRepeatCount,
                             rec->Event.KeyEvent.wVirtualKeyCode,
                             rec->Event.KeyEvent.dwControlKeyState);
        }
#endif
      }
      return(TRUE);
    }

    case ECTL_PROCESSINPUT:
    {
      _KEYMACRO(CleverSysLog SL("FileEditor::EditorControl(ECTL_PROCESSINPUT)"));

      if(!Param)
      {
        _ECTLLOG(SysLog("Param = NULL"));
        return FALSE;
      }
      else
      {
        INPUT_RECORD *rec=(INPUT_RECORD *)Param;
        if (ProcessEditorInput(rec))
        {
          _ECTLLOG(SysLog("ProcessEditorInput(rec) => return 1 !!!"));
          return(TRUE);
        }
        if (rec->EventType==MOUSE_EVENT)
          ProcessMouse(&rec->Event.MouseEvent);
        else
        {
#if defined(SYSLOG_KEYMACRO)
          if(rec->EventType == KEY_EVENT || rec->EventType == FARMACRO_KEY_EVENT)
          {
            SysLog("ECTL_PROCESSINPUT={%s,{%d,%d,Vk=0x%04X,0x%08X}}",
                             (rec->EventType == FARMACRO_KEY_EVENT?"FARMACRO_KEY_EVENT":"KEY_EVENT"),
                             rec->Event.KeyEvent.bKeyDown,
                             rec->Event.KeyEvent.wRepeatCount,
                             rec->Event.KeyEvent.wVirtualKeyCode,
                             rec->Event.KeyEvent.dwControlKeyState);
          }
#endif
          int Key=CalcKeyCode(rec,FALSE);
          _KEYMACRO(SysLog("Key=CalcKeyCode() = 0x%08X",Key));
          ProcessKey(Key);
        }
      }
      return(TRUE);
    }

    case ECTL_PROCESSKEY:
    {
      _KEYMACRO(CleverSysLog SL("FileEditor::EditorControl(ECTL_PROCESSKEY)"));
      _ECTLLOG(SysLog("Key = %s",_FARKEY_ToName((DWORD)Param)));
      ProcessKey((int)Param);
      return TRUE;
    }

  }

  return FEdit->EditorControl(Command,Param);
}
