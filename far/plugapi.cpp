/*
plugapi.cpp

API, ��������� �������� (�������, ����, ...)

*/

/* Revision: 1.167 25.12.2003 $ */

/*
Modify:
  25.12.2003 SVS
    ! �������������� ������������� ��������� MRec
  15.12.2003 SVS
    ! ������������� MacroRecord � ACTL_POSTKEYSEQUENCE
  25.11.2003 SVS
    ! ������������� �������� ��������� ��� ����.
  11.11.2003 SVS
    + FMENU_CHANGECONSOLETITLE - �������� ��������� ������� ��� �����
    + ��� �������� �� �������� � FarControl()
  17.10.2003 SVS
    + ����� ��� ����� � ������� ��������� �������� - FDIS_BSDELETEUNCHANGEDTEXT
  04.10.2003 SVS
    ! ��������� ��������� ActlKeyMacro: �������� ���� Param.PlainText -
      ��������� �� ������, ���������� �����������������������.
  08.09.2003 SVS
    + ����� ������� ��� ACTL_KEYMACRO: MCMD_POSTMACROSTRING - ��������� ������
      � ���� plain-text.
  21.08.2003 SVS
    - ������� � ���������� ������� �����, ���������� �� '..' � ���� F3.
      �������� ��������. ������������ �� ���������� - ����� ���� � �����������
  15.06.2003 SVS
    + ACTL_GETDIALOGSETTINGS
    ! FIS_PERSISTENTBLOCKSINEDITCONTROLS -> FDIS_PERSISTENTBLOCKSINEDITCONTROLS
    ! FIS_HISTORYINDIALOGEDITCONTROLS    -> FDIS_HISTORYINDIALOGEDITCONTROLS
    ! FIS_AUTOCOMPLETEININPUTLINES       -> FDIS_AUTOCOMPLETEININPUTLINES
  14.06.2003 SVS
    + FSS_SCANSYMLINK
  30.05.2003 SVS
    + ACTL_GETPLUGINMAXREADDATA
  06.05.2003 SVS
    ! Opt.UseTTFFont �������� �� Opt.UseUnicodeConsole - ��� ������
    - ������ � ���������:
      BugZ#790 - �������������� ������� ����� ����� ��������� ��� ����������?
      BugZ#873 - ACTL_POSTKEYSEQUENCE � ��������� ����
  17.03.2003 SVS
    + ACTL_GETPOLICIES + FFPOL_* - ���� ��� ����������� ������
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
  04.02.2003 SVS
    - BugZ#788 - ����� ��������� ������ ����.
      �������� ���������� ������ ����� (�� ���������� ��������) � ���������
      �� ��� ������������� ��������.
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  26.12.2002 SVS
    ! �� FarViewer � FarEditor �������� ��������� �������� � ������������ ���� ��������
  11.12.2002 SVS
    - ����� ������� � KEY_CONSOLE_BUFFER_RESIZE (������������ ��������� ������� �������)
  04.11.2002 SVS
    ! � FarMessageFn() �������� PluginNumber ����� ���� ����� -1
      ��� ���� ���� ����������!
    - ��� ACTL_GETFARHWND ��������� �� ���� (��� ���������� �������) == NULL
      � ���� ������ ����������� �� �����
  27.10.2002 DJ
    ! ����������� FARColor � FarSetColors (��� ������������ � ���������� �,
      ����� ��, ����� ���� �������, ��� � ����)
  23.10.2002 SVS
    - ������ ��������������  � FarCharTable() - ����� ������� ������ ������� ;-/
  22.10.2002 SVS
    ! ���������� FarCharTable() - ������� "����������" ����, �.�. ������� ���
      ���������� � ��������� ����������, � ����� "������" ��������� �������
    ! ��� �� ������� CharTableSet.RFCCharset, �� �������������� - ����� �����
      �� ������ ��� ��� ������� ;-)
  30.09.2002 SVS
    ! ������� �� ������� ����������� ������� ��� ��������� ������
    ! ����������������� ID � ��������� �������!
  23.09.2002 SVS
    + ACTL_SETARRAYCOLOR, FARCOLORFLAGS, FARColor ���� ��� ����������� ������
      (����, ����, �������� �� ���!)
  20.09.2002 SVS
    + ����� FDLG_NODRAWSHADOW � FDLG_NODRAWPANEL
  10.09.2002 SVS
    ! ������� ��������� ���������� ��� ������� - ���� ���� PSIF_DIALOG
      ���������� - ���������� �������� �������.
  21.08.2002 SVS
    ! ACTL_WAITKEY ������ ���������� ��� ������� �������
  25.06.2002 SVS
    ! ���������:  BitFlags::Skip -> BitFlags::Clear
  21.06.2002 SVS
    + ACTL_GETWCHARMODE ��� FAR_USE_INTERNALS
      "������� ��� ������ � ���� � ������� W-������� ��� ���?"
  14.06.2002 IS
    + ��������� xF_DELETEONLYFILEONCLOSE - ���� ���� ����� ����� ������
      ��������� �� ��������� � xF_DELETEONCLOSE
  27.05.2002 SVS
    - ����, �������� � BugZ#514 (��� ���������� ������� - � ����� ����� ��������,
      � ����� ������� - MultiArc...  :-(
  27.05.2002 SVS
    - BugZ#514 - Multiple selected items in menu...
    + ��������� ��� ?F_IMMEDIATERETURN (�� ������� BugZ#530 - ����
      EF_IMMEDIATERETURN �� �������� �� SetStartupInfo)
    + �������� ���� �������� �������� ���������, ���� Editor !=0 � ��� �������� != XC_OPEN_ERROR?
      �� ���������� ����������, ����� ������ EEC_OPEN_ERROR
  24.05.2002 SVS
    + ������� � FarControl() ����� ��� _FCTLLOG
    ! �� FarEditorControl ������� ��� ��� ����� - ��������� � fileedit.cpp
  22.05.2002 SKV
    + ?F_IMMEDIATERETURN
  18.05.2002 SVS
    ! ������� ������, ��-�� �������� �� ���� ����������� ������� �����
      ����������� SetOwnsItems �� ��� ����, ���� �� �������� �����������
      �������
  14.05.2002 SKV
    + �������� �������� ������������ viewer/editor �� ����������,
      � ��� �� ������������ �� ������ ������.
  10.05.2002 SVS
    - BugZ#502 - ����� Info.Control ��� �� ��������� far /e
      ��������� ������������� Control ��� CmdMode=1 (��� /e � /v)
    + FCTL_CHECKPANELSEXIST - �������� � ��������� ������ (������ ��������?)
  30.04.2002 SVS
    - ����� Message :-(
  15.04.2002 SVS
    - Message(FMSG_ALLINONE,"\n\nFoobar") �������� � �������
  11.04.2002 SVS
    + FCTL_GET[ANOTHER]PANELSHORTINFO
    - Message(FMSG_ALLINONE,"\nFoobar") �������� � ������� (������ '\n'
      �������� �������� ����� ��������� ����������)
  04.04.2002 SVS
    - ���� � ���� ������� ������ ������ ���������, �� �� �������
      selected, �� �������� �� ������ ����.
  30.03.2002 OT
    - ����� ����������� ���� �314 (���� 1250) ���������� ��������
      ���� �� ������.
  26.03.2002 DJ
    ! �������� GetMsg() ��� ManagerIsDown()
    ! ScanTree::GetNextName() ��������� ������ ������ ��� ����� �����
  25.03.2002 SVS
    ! ������ ������� �������� ������� ���, ���� ManagerIsDown() == FALSE
  22.03.2002 SVS
    - strcpy - Fuck!
  18.03.2002 SVS
    ! ���������, � ����� � ��������� Opt.Dialogs
  17.03.2002 IS
    - FarCharTable: �� ����������� ���������� �������� TableName (BugZ#331)
  01.03.2002 SVS
    ! ���� ������ ���� ������� �������� ���������� ����� - FarMkTempEx
  28.02.2002 SVS
    - ��� ACTL_CONSOLEMODE+CONSOLE_GET_MODE
      Param=NULL - ���... ������� (void*)0, ��� ���...
      � ���� �� "*(int*)Param" - ��� ������ �����.
    ! ACTL_*WINDOW* - ������� �������� �� FrameManager != NULL
    - ACTL_GETWINDOWINFO - ������ ��������� ���������� �������� WindowInfo.Pos
  19.02.2002 SVS
    ! ������� �������� �� PluginNumber � Menu, Message & Dialog
  13.02.2002 SVS
    + MIF_USETEXTPTR - ��� ����� TextPtr
    ! ������ ���������
  11.02.2002 SVS
    + ������� � ���� - ����������� - ������� BugZ#299
    ! ��������� ����� ����� ���� ������ ��� �������� � Ex
  05.02.2002 SVS
    ! ��������������� ���� ��� �������
  30.01.2002 DJ
    + ACTL_GETDESCSETTINGS
  22.01.2002 SVS
    ! ������� ������� Opt.ExceptRules � ��������, ��� ������ � ����� ����,
      ��� ������� ���� � �������! ;-)
    ! ������� ��������� � ������.
  10.01.2002 SVS
    ! ������� SYSLOG_KEYMACRO
    ! ��������� ������ ����� ����������� ������
  26.12.2001 SVS
    + EF_USEEXISTING, EF_BREAKIFOPEN - ��������� ��� �������� ���������
      ��������, � �������� ��������� ��� ��?
  24.12.2001 SVS
    - ���� � ����������� (��� ������ � ����� ����� ��������� �����, �� �����
      ������ �� �����������, ��� �������� HLF-����� - ����������)
    ! ��� LIF_SEPARATOR �������� ����� ����.
  22.12.2001 VVM
    + ACTL_GETWINDOWINFO: ���� Pos == -1 �� ����� ������� �����
  07.12.2001 IS
    + FarInputBox - ������� ������ GetString ��� �������� - � �������
      �����������������. ������� ��� ����, ����� �� ����������� ��� GetString.
  05.12.2001 SVS
    ! �������� ��������� ��������� ���������� �� ���� ������.
  30.11.2001 DJ
    - �� ��������, ���� GetPluginDirList() ���������� �� �� ���������� ������
    - � GetPluginDirList() ����������� ������������ PPIF_USERDATA ��� ���������
  28.11.2001 SVS
    + FIS_SHOWCOPYINGTIMEINFO
    ! ������� ����������� ��� ACTL_GET*SETTINGS
  24.11.2001 IS
    + ��������� ACTL_GETSYSTEMSETTINGS, ACTL_GETPANELSETTINGS,
      ACTL_GETINTERFACESETTINGS, ACTL_GETCONFIRMATIONS,
  02.11.2001 SVS
    ! ������� �������� ��� (� ���-��� ������� :-))
    ! FCTL_GETCMDLINESELECTION -> FCTL_GETCMDLINESELECTEDTEXT
    + FCTL_GETCMDLINESELECTION - �������� ������� ���������!
  28.10.2001 SVS
    ! ������ ������� ����������� �������� SaveScreen.
  26.10.2001 SVS
    - ������� ����������������� ������ MsgItems � FarMessageFn()
  22.10.2001 SVS
    - ��������� (��������������) ��������� Message ��� ��������
  22.10.2001 SVS
    ! ������� ������ +16 ��������� ADDSPACEFORPSTRFORMESSAGE
  21.10.2001 SVS
    + CALLBACK-������� ��� ���������� �� BugZ#85
    ! �������������� Message ��� �������� (����� ����������� �� 13 �����)
  11.10.2001 IS
    + ��������� EF_DELETEONCLOSE
  08.10.2001 OT
    ! ������ ������������ ������ � ��������� ������
  07.10.2001 SVS
    - ���� � ��� ����������� ��������!
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  22.09.2001 OT
    ����� Viewer � Editor �� ���� ������� ���������� ����-�� � background window
  21.09.2001 SVS
    - ����. ��� ������ ���, ���� ������ �������� Message ��� Dialog, ��
      ���� �������, �.�. Frame ��� �������� ����. � ����� ������� ��������
      �� NULL ��� Lock/Unlock
  20.09.2001 SVS
    ! ��������� ������� � ����������, ��������� �� �������� "��������
      ������������ ������� ������"
      � ��� �� ���� ������ ���� �� �������???
  16.09.2001 SVS
    ! ����������� ����������
  15.09.2001 tran
    + ACTL_GETFARHWND
  09.09.2001 IS
    + ��������� VF_DISABLEHISTORY/EF_DISABLEHISTORY - ������ ����� ���������
      ���������� ����� ����� � �������.
  08.09.2001 IS
    ! ������ ��� ����� ����������� � ������� ���������/��������������
      ��� ������������� ������� ��� Editor/Viewer
  13.08.2001 SKV
    + FCTL_GETCMDLINESELECTION,FCTL_SETCMDLINESELECTION
  13.08.2001 SVS
    - ����� � ������� ��� ���������� �������� PluginNumber � �������
      GetPluginDirList() ��� ������, ���� hPlugin=INVALID_HANDLE_VALUE
  07.08.2001 IS
    + ���� � FarCharTable: ��� ������� ���������� �������� �� ������������
      ���������� ������� ��������� CharTableSet ����������� ������� ��� OEM.
    ! FarCharTable: ������ �������� ������ �� const, �.�. �� �������� � �������.
  05.08.2001 SVS
    - ���� � ������� ����� � �������� �� ��������
    ! ������� �� �������� �������� (���������) ��������
  01.08.2001 SVS
    + �������� ������� ���� � ������ ������� (��� ���� ������)!
  31.07.2001 IS
    + ��������� � ����� � FarDialogEx, FarMenuFn, FarMessageFn:
      1. ����� ���� F*_CUSTOMNAME
      2. ������� ������������� ��� ������ � *HelpTopic=='#'
      3. ���������, ��� ������ ���, ���� ��� ���� ������.
  31.07.2001 IS
    - ���: ���� ������� ����������� � ������ ������� ��
      ctrl-key, alt-key ��� shift-key, ���� ���� ��� ����������
      ����� �� ���� ������� � BreakKeys, � ������� ���� �����
      ��������� ������� �� ������ key.
    ! ��������� const (FarGetMsgFn)
  31.07.2001 SVS
    + ��������� ������ �� �������: ~Text~@#Path#Topic@
  27.07.2001 SVS
    + ����� ��� �������� ���������� � ACTL_POSTKEYSEQUENCE
    + ����� ����������� ������� ��� ������ GetPluginDirList
      ��������� �� �������� ������, �.�. hPlugin=INVALID_HANDLE_VALUE.
      �� �������� � Control
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  19.07.2001 OT
    ������ ��� � ������������
  18.07.2001 OT
    VFMenu
  16.07.2001 SVS
    + ��������� FarMenuItemEx � FarMenu
  26.06.2001 SVS
    ! __except -> EXCEPT
  26.06.2001 SKV
    + ACTL_COMMIT
  25.06.2001 IS
   ! ��������� const
  21.06.2001 SVS
    ! ACTL_POSTSEQUENCEKEY -> ACTL_POSTKEYSEQUENCE - (� ����� ������ eng)
    ! SequenceKey           -> KeySequence
    ! � ACTL_GETWINDOWINFO ������� �������� Param �� NULL
  20.06.2001 SVS
    ! ACTL_PROCESSSEQUENCEKEY -> ACTL_POSTSEQUENCEKEY, � �.�. ��������
      ���� ������� ������ ������������ �� Macro API
  19.06.2001 IS
    - ���: �� �������� ��������������� ��������� ����� 268.
  06.06.2001 SVS
    - �� ����� ���������� ������� � ���� �������������� BreakKeys.
  05.06.2001 tran
    + ACTL_GETWINDOWCOUNT,ACTL_GETWINDOWINFO,ACTL_SETCURRENTWINDOW
  04.06.2001 SVS
    ! ����� �� ������ Checked ������� � ����. ����������.
  03.06.2001 SVS
    ! ��������� � ����� � ���������� UserData � VMenu
  31.05.2001 OT
    - ExitCode � ������������ � ����������� ������������� plugin.hlp
  27.05.2001 OT
    - ����������� ������� ������������� FileView � FileEdit � FarViewer() � FarEditor()
  26.05.2001 OT
    - ����������� ������ ������� � NFZ
  21.05.2001 DJ
    + FDLG_NONMODAL
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  16.05.2001 DJ
    ! proof-of-concept
  16.05.2001 SVS
    + #include "farexcpt.hpp"
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 SVS
    ! FDLG_SMALLDILAOG -> FDLG_SMALLDIALOG
  12.05.2001 DJ
    + EF_ENABLE_F6, VF_ENABLE_F6
  12.05.2001 DJ
    ! ����� SaveScr ��� ������ ���������/������ �� �������
  10.05.2001 SVS
    + � FarDialogEx() ��������� ��������� ������ FDLG_WARNING � FDLG_SMALLDILAOG.
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  01.05.2001 SVS
    - ������ PluginNumber � FarDialogEx() ������� ItemsNumber :-(((
  28.04.2001 SVS
    + ��������� ���������� � FarDialogEx() - ����������� ����� ����� ���
      ������ � ��������, ��� �����.
  11.09.2001 SVS
    - FarMessageFn(): ���� ���������� ���� FMSG_ERRORTYPE, ItemsNumber ������ 1,
      ���� ������ ��������� ���������� ���������.
    - FarMessageFn(): ������� ��������� ���� #530 - ����������.
  09.04.2001 SVS
    ! ��������� �� ��������� ���������
  08.04.2001 SVS
    ! ��������� � FarShowHelp
  28.03.2001 SVS
    ! ACTL_GETFARVERSION ���������� ����� ������, � �� TRUE - ��� ����������
    ! ACTL_KEYMACRO, ACTL_EJECTMEDIA, ACTL_WAITKEY, ACTL_CONSOLEMODE -
      �������� Param �� NULL.
    ! FarGetMsgFn - �������� �� "����������" CtrlObject (�������� �� NULL)
  26.03.2001 SVS
    + ��������� ��������� ����� FHELP_USECONTENTS
  22.03.2001 tran 1.42
    ! ������ ��� � FarMessageFn/FMSG_ALLINONE
  21.03.2001 VVM
    + ��������� ����� EF_CREATENEW
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  11.02.2001 SVS
    ! �������� ������������� ��� � FarDialogEx
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  28.01.2001 SVS
    ! ��������� ������� ������� FarMessageFn()
  23.01.2001 SVS
    ! �������� ���������� � FarDialogEx()
  21.01.2001 SVS
    + ACTL_PROCESSSEQUENCEKEY
  24.12.2000 SVS
    ! ��������� "MCMD_PLAYSTRING"
    ! ��������� MCMD_LOADALL � MCMD_SAVEALL - �� �������� �� ����� ������
      ��� "��������������� �������
  23.12.2000 SVS
    + MCMD_PLAYSTRING - "���������" ������ (������ � ��� ����, ��� � �������)
  21.12.2000 SVS
    + ACTL_KEYMACRO - ������� �������� KeyMacro API
  18.12.2000 SVS
    ! ��������� � FarShowHelp
  14.12.2000 SVS
    + ACTL_EJECTMEDIA
  13.12.2000 SVS
    ! FarDialogItem.Data - ����������� strcpy �������� �� memmove
      (�������� ������ ������������)
  04.12.2000 SVS
    + ACTL_GETCOLOR - �������� ������������ ����
    + ACTL_GETARRAYCOLOR - �������� ���� ������ ������
  17.11.2000 SVS
    ! "������� ������ �������" - ���� � ������� ShowHelp ��������� :-(
  11.11.2000 SVS
    ! FarMkTemp() - ������� (��� ������ - �� ������, �� ��� �� ������� :-(((
  11.11.2000 SVS
    ! ���������� ����������� FarMkTemp()
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  05.10.2000 SVS
   - ���� � ������� ����� (FHELP_CUSTOMFILE)
  27.09.2000 SVS
   + FarViewerControl
  18.09.2000 SVS
    ! ������� FarDialogEx ����� 2 �������������� ��������� (Future)
  12.09.2000 SVS
    + ���������� ������ FHELP_* ��� ������ ������.
  08.09.2000 VVM
    + ��������� ������
      FCTL_SETSORTMODE, FCTL_SETANOTHERSORTMODE
      FCTL_SETSORTORDER, FCTL_SETANOTHERSORTORDER
  30.08.2000 SVS
    ! ��� ������� ������� ���� FMI_GETFARMSGID
  29.08.2000 SVS
    + ��� ������� �������� ����� �������, ���������� ���� ������. ������
      ��� ��� ����, ����� ��������� ������������� � Dialog API �� DN_HELP
  29.08.2000 SVS
    ! ���� PluginStartupInfo.GetMsg(?,N|FMI_GETFARMSGID), �� �������������, ���
      ����� ������������ "������" �� ������ far*.lng
  24.08.2000 SVS
    + ACTL_WAITKEY - ������� ������������ (��� �����) �������
  23.08.2000 SVS
    ! ��� Flags ��������� � ������ ���� -> DWORD.
      ��������������:
        * �������   FarMenuFn, FarMessageFn, FarShowHelp
        * ��������� FarListItem, FarDialogItem
  22.08.2000 SVS
    ! ��������� �������� ������ �� FarText.
  18.08.2000 tran 1.12
    + Flags parameter in FarShowHelp
  09.08.2000 tran 1.11
    ! ACTL_GETSYSWORDDIV ��� Param==NULL ������ ���������� ����� ������
  03.08.2000 SVS
    + ACTL_GETSYSWORDDIV �������� ������ � ��������� ������������� ����
  01.08.2000 SVS
    ! FARDIALOGPROC -> FARWINDOWPROC
  28.07.2000 SVS
    ! � ����� � ���������� SendDlgMessage � ������ Dialog
      ������ ��������� ���������!
  25.07.2000 SVS
    + ���������� ������������ FulScreen <-> Windowed (ACTL_CONSOLEMODE)
  23.07.2000 SVS
    + ������� FarDefDlgProc ��������� ������� �� ���������
    + ������� FarSendDlgMessage - ������� ��������� �������
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  12.07.2000 IS
    + �������� ������ ��������� � FarEditor (������ ��� ��������������) �
      �������� _������������_ ���������, ���� ���� ��������������� ����.
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  05.07.2000 IS
    + ������� FarAdvControl
    + ������� ACTL_GETFARVERSION (� FarAdvControl)
  03.07.2000 IS
    + ������� ������ ������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "struct.hpp"
#include "keys.hpp"
#include "lang.hpp"
#include "help.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "cmdline.hpp"
#include "scantree.hpp"
#include "rdrwdsk.hpp"
#include "fileview.hpp"
#include "fileedit.hpp"
#include "plugins.hpp"
#include "savescr.hpp"
#include "manager.hpp"
#include "ctrlobj.hpp"
#include "frame.hpp"
#include "scrbuf.hpp"
#include "farexcpt.hpp"
#include "lockscrn.hpp"


void ScanPluginDir();


/* $ 07.12.2001 IS
   ������� ������ GetString ��� �������� - � ������� �����������������.
   ������� ��� ����, ����� �� ����������� ��� GetString.
*/
int WINAPI FarInputBox(const char *Title,const char *Prompt,
                       const char *HistoryName,const char *SrcText,
                       char *DestText,int DestLength,
                       const char *HelpTopic,DWORD Flags)
{
  if (FrameManager->ManagerIsDown())
    return FALSE;
   return GetString(Title,Prompt,HistoryName,SrcText,DestText,DestLength,
     HelpTopic,Flags&~FIB_CHECKBOX,NULL,NULL);
}
/* IS $ */

/* $ 12.09.2000 SVS
  + ���������� ������ ��� ������ ������.
*/
/* $ 18.08.2000 tran
   + Flags parameter */
/* $ 03.07.2000 IS
  ������� ������ ������
*/
BOOL WINAPI FarShowHelp(const char *ModuleName,
                        const char *HelpTopic,
                        DWORD Flags)
{
  if (FrameManager->ManagerIsDown())
    return FALSE;
  if (!HelpTopic)
    HelpTopic="Contents";

  DWORD OFlags=Flags;
  Flags&=~(FHELP_NOSHOWERROR|FHELP_USECONTENTS);
  char Path[2*NM],Topic[512];
  char *Mask=NULL;

  // ��������� � ������ ������ ���� �� ������������ � � ��� ������,
  // ���� ����� FHELP_FARHELP...
  if((Flags&FHELP_FARHELP) || *HelpTopic==':')
    strncpy(Topic,HelpTopic+((*HelpTopic == ':')?1:0),sizeof(Topic)-1);
  else
  {
    if(ModuleName)
    {
      // FHELP_SELFHELP=0 - ���������� ������ ���-� ��� Info.ModuleName
      //                   � �������� ����� �� ����� ���������� �������
      /* $ 17.11.2000 SVS
         � �������� FHELP_SELFHELP ����� ����? ��������� - 0
         � ����� ����� ��������� ����, ��� ������� �� �������� :-(
      */
      if(Flags == FHELP_SELFHELP || (Flags&(FHELP_CUSTOMFILE|FHELP_CUSTOMPATH)))
      {
        strncpy(Path,ModuleName,sizeof(Path)-1);
        if(Flags == FHELP_SELFHELP || (Flags&(FHELP_CUSTOMFILE)))
        {
          Mask=PointToName(Path);
          if(Flags&FHELP_CUSTOMFILE)
          {
            memmove(Mask+1,Mask,strlen(Mask)+1);
            *Mask++=0;
          }
          else
          {
            *Mask=0;
            Mask=NULL;
          }
        }
      }
      else
        return FALSE;
      /* SVS $*/

      sprintf(Topic,HelpFormatLink,Path,HelpTopic);
    }
    else
      return FALSE;
  }
  {
    Help Hlp (Topic,Mask,OFlags);
    if(Hlp.GetError())
      return FALSE;
  }
  return TRUE;
}
/* IS $ */
/* tran 18.08.2000 $ */
/* SVS 12.09.2000 $ */

/* $ 05.07.2000 IS
  �������, ������� ����� ����������� � � ���������, � � �������, �...
*/
#ifndef _MSC_VER
#pragma warn -par
#endif
int WINAPI FarAdvControl(int ModuleNumber, int Command, void *Param)
{
  struct Opt2Flags{
    int *Opt;
    DWORD Flags;
  };
  int I;

  switch(Command)
  {
    case ACTL_GETFARVERSION:
    case ACTL_GETSYSWORDDIV:
    case ACTL_GETCOLOR:
    case ACTL_GETARRAYCOLOR:
    case ACTL_GETFARHWND:
    case ACTL_GETSYSTEMSETTINGS:
    case ACTL_GETPANELSETTINGS:
    case ACTL_GETINTERFACESETTINGS:
    case ACTL_GETCONFIRMATIONS:
    case ACTL_GETDESCSETTINGS:
    case ACTL_GETPOLICIES:
    case ACTL_GETPLUGINMAXREADDATA:
      break;
    default:
     if (FrameManager && FrameManager->ManagerIsDown())
       return 0;
  }

  switch(Command)
  {
    case ACTL_GETFARVERSION:
    {
      if(Param)
        *(DWORD*)Param=FAR_VERSION;
      return FAR_VERSION;
    }

    /* $ 25.07.2000 SVS
       + ���������� ������������ FulScreen <-> Windowed (ACTL_CONSOLEMODE)
       mode = -2 - �������� ������� ���������
              -1 - ��� ������
               0 - Windowed
               1 - FulScreen
       Return
               0 - Windowed
               1 - FulScreen
    */
    case ACTL_CONSOLEMODE:
    {
      return FarAltEnter((int)Param);
    }
    /* SVS $ */

    case ACTL_GETPLUGINMAXREADDATA:
    {
      return Opt.PluginMaxReadData;
    }

    case ACTL_GETWCHARMODE:
    {
      return Opt.UseUnicodeConsole;
    }

    /* $ 03.08.2000 SVS
       ��������� ������ � ������������� ����
       ���������� ������ ���������� ������ ��� '\0'
       ������������ ������ ��������� ������ = 80 � �������������� '\0'
       ������ ���������� �� �� �������, � �� Opt.
    */
    case ACTL_GETSYSWORDDIV:
    {
      int LenWordDiv=strlen(Opt.WordDiv);
      /* $ 09.08.2000 tran
       + if param==NULL, plugin ����� ������ ������ ����� ������  */
      if (Param && !IsBadWritePtr(Param,LenWordDiv+1))
        strcpy((char *)Param,Opt.WordDiv);
      /* tran 09.08.2000 $ */
      return LenWordDiv;
    }
    /* SVS $ */

    /* $ 24.08.2000 SVS
       ������� ������������ (��� �����) �������
       (int)Param - ���������� ��� �������, ������� �������, ��� -1
       ���� ��� ����� ����� ������� �����.
       ���������� 0;
    */
    case ACTL_WAITKEY:
    {
      return WaitKey(Param?(DWORD)Param:(DWORD)-1);
    }
    /* SVS $ */

    /* $ 04.12.2000 SVS
      ACTL_GETCOLOR - �������� ������������ ���� �� ������, �������������
       � farcolor.hpp
      (int)Param - ������.
      Return - �������� ����� ��� -1 ���� ������ �������.
    */
    case ACTL_GETCOLOR:
    {
      if((int)Param < SizeArrayPalette && (int)Param >= 0)
        return (int)((unsigned int)Palette[(int)Param]);
      return -1;
    }
    /* SVS $ */

    /* $ 04.12.2000 SVS
      ACTL_GETARRAYCOLOR - �������� ���� ������ ������
      Param - ��������� �� ������ ��� NULL - ����� �������� ������ ������
      Return - ������ �������.
    */
    case ACTL_GETARRAYCOLOR:
    {
      if(Param && !IsBadWritePtr(Param,SizeArrayPalette))
        memmove(Param,Palette,SizeArrayPalette);
      return SizeArrayPalette;
    }
    /* SVS $ */

    /*
      Param=struct FARColor{
        DWORD Flags;
        int StartIndex;
        int ColorItem;
        LPBYTE Colors;
      };
    */
    case ACTL_SETARRAYCOLOR:
    {
      if(Param && !IsBadReadPtr(Param,sizeof(struct FarSetColors)))
      {
        struct FarSetColors *Pal=(struct FarSetColors*)Param;
        if(Pal->Colors &&
           Pal->StartIndex >= 0 &&
           Pal->StartIndex+Pal->ColorCount <= SizeArrayPalette &&
           !IsBadReadPtr(Pal->Colors,Pal->ColorCount)
          )
        {
          memmove(Palette+Pal->StartIndex,Pal->Colors,Pal->ColorCount);
          if(Pal->Flags&FCLR_REDRAW)
          {
            ScrBuf.Lock(); // �������� ������ ����������
            FrameManager->ResizeAllFrame();
            FrameManager->PluginCommit(); // ��������.
            ScrBuf.Unlock(); // ��������� ����������
          }
          return TRUE;
        }
      }
      return FALSE;
    }

    /* $ 14.12.2000 SVS
      ACTL_EJECTMEDIA - ������� ���� �� �������� ����������
      Param - ��������� �� ��������� ActlEjectMedia
      Return - TRUE - �������� ����������, FALSE - ������.
    */
    case ACTL_EJECTMEDIA:
    {
      return Param?EjectVolume((char)((ActlEjectMedia*)Param)->Letter,
                               ((ActlEjectMedia*)Param)->Flags):FALSE;
    }
    /* $ 21.12.2000 SVS
       Macro API
    */
    case ACTL_KEYMACRO:
    {
      if(CtrlObject && Param) // ��� ������� �� ���� ������.
      {
        KeyMacro& Macro=CtrlObject->Macro; //??
        struct ActlKeyMacro *KeyMacro=(struct ActlKeyMacro*)Param;
        switch(KeyMacro->Command)
        {
          case MCMD_LOADALL: // �� ������� � ������ ��� � ���������� �����������
          {
            if(Macro.IsRecording())
              return FALSE;
            return Macro.LoadMacros(!Macro.IsExecuting());
          }

          case MCMD_SAVEALL: // �� ������ ���� � �������
          {
            if(Macro.IsRecording() || Macro.IsExecuting())
              return FALSE;
            Macro.SaveMacros();
            return TRUE;
          }

          case MCMD_POSTMACROSTRING:
          {
            return Macro.PostNewMacro(KeyMacro->Param.PlainText.SequenceText,KeyMacro->Param.PlainText.Flags);
          }
#if 0
          case MCMD_COMPILEMACRO:
          {
            struct MacroRecord CurMacro={0};
            int Ret=Macro.ParseMacroString(&CurMacro,KeyMacro->Param.PlainText.SequenceText);
            if(Ret)
            {
              //KeyMacro->Params.Compile.Flags=CurMacro.Flags;
              KeyMacro->Param.Compile.Sequence=CurMacro.Buffer;
              KeyMacro->Param.Compile.Count=CurMacro.BufferSize;
            }
            return Ret;
          }
#endif
        }
      }
      return FALSE;
    }

    case ACTL_POSTKEYSEQUENCE:
    {
      if(CtrlObject && Param && ((struct KeySequence*)Param)->Count > 0)
      {
        struct MacroRecord MRec;
        memset(&MRec,0,sizeof(struct MacroRecord));
        MRec.Flags=(((struct KeySequence*)Param)->Flags)<<8;
        MRec.BufferSize=((struct KeySequence*)Param)->Count;
        if(MRec.BufferSize == 1)
          MRec.Buffer=(DWORD *)((struct KeySequence*)Param)->Sequence[0];
        else
          MRec.Buffer=((struct KeySequence*)Param)->Sequence;
        return CtrlObject->Macro.PostNewMacro(&MRec,TRUE);
#if 0
        // ���� ����� - ��� ���������� �������������
        {
          //CtrlObject->Macro.PostNewMacro(&MRec);
          for(int I=0; I < MRec.BufferSize; ++I)
          {
            int Key=MRec.Buffer[I];
            if(CtrlObject->Macro.ProcessKey(Key))
            {
              while((Key=CtrlObject->Macro.GetKey()) != 0)
              {
                FrameManager->ProcessKey(Key);
              }
            }
            else
              FrameManager->ProcessKey(Key);
            FrameManager->PluginCommit();
          }
          return TRUE;
        }
#endif
      }
      return FALSE;
    }

    /* $ 05.06.2001 tran
       ����� ACTL_ ��� ������ � �������� */
    case ACTL_GETWINDOWINFO:
    {
      if(FrameManager && Param && !IsBadWritePtr(Param,sizeof(WindowInfo)))
      {
        WindowInfo *wi=(WindowInfo*)Param;
        Frame *f;
        /* $ 22.12.2001 VVM
          + ���� Pos == -1 �� ����� ������� ����� */
        if (wi->Pos == -1)
          f=FrameManager->GetCurrentFrame();
        else
          f=FrameManager->operator[](wi->Pos);
        /* VVM $ */
        if ( f==NULL )
          return FALSE;
        f->GetTypeAndName(wi->TypeName,wi->Name);
        wi->Pos=FrameManager->IndexOf(f);
        wi->Type=f->GetType();
        wi->Modified=f->IsFileModified();
        wi->Current=f==FrameManager->GetCurrentFrame();
        return TRUE;
      }
      return FALSE;
    }

    case ACTL_GETWINDOWCOUNT:
    {
      return FrameManager?FrameManager->GetFrameCount():0;
    }

    case ACTL_SETCURRENTWINDOW:
    {
      /* $ 15.05.2002 SKV
        �������� ������������ �������,
        ���� ��������� � ��������� ���������/������.
      */
      if (FrameManager && !FrameManager->InModalEV() &&
          FrameManager->operator[]((int)Param)!=NULL )
      {
        FrameManager->ActivateFrame((int)Param);
        return TRUE;
      }
      return FALSE;
    }
    /* tran 05.06.2001 $ */
    /*$ 26.06.2001 SKV
      ��� ����������� ������ � ACTL_SETCURRENTWINDOW
      (� ����� ��� ��� ���� � �������)
    */
    case ACTL_COMMIT:
    {
      return FrameManager?FrameManager->PluginCommit():FALSE;
    }
    /* SKV$*/
    /* $ 15.09.2001 tran
       ���������� �������� */
    case ACTL_GETFARHWND:
    {
      if(!hFarWnd)
        InitDetectWindowedMode();
      return (int)hFarWnd;
    }
    /* tran $ */

    case ACTL_GETDIALOGSETTINGS:
    {
      DWORD Options=0;
      static struct Opt2Flags ODlg[]={
        {&Opt.Dialogs.EditHistory,FDIS_HISTORYINDIALOGEDITCONTROLS},
        {&Opt.Dialogs.EditBlock,FDIS_PERSISTENTBLOCKSINEDITCONTROLS},
        {&Opt.Dialogs.AutoComplete,FDIS_AUTOCOMPLETEININPUTLINES},
        {&Opt.Dialogs.EULBsClear,FDIS_BSDELETEUNCHANGEDTEXT},
      };
      for(I=0; I < sizeof(ODlg)/sizeof(ODlg[0]); ++I)
        if(*ODlg[I].Opt)
          Options|=ODlg[I].Flags;
      return Options;
    }
    /* $ 24.11.2001 IS
       ��������� � ����������� ����������, ������, ����������, �������������
    */
    case ACTL_GETSYSTEMSETTINGS:
    {
      DWORD Options=0;
      static struct Opt2Flags OSys[]={
        {&Opt.ClearReadOnly,FSS_CLEARROATTRIBUTE},
        {&Opt.DeleteToRecycleBin,FSS_DELETETORECYCLEBIN},
        {&Opt.UseSystemCopy,FSS_USESYSTEMCOPYROUTINE},
        {&Opt.CopyOpened,FSS_COPYFILESOPENEDFORWRITING},
        {&Opt.ScanJunction,FSS_SCANSYMLINK},
        {&Opt.CreateUppercaseFolders,FSS_CREATEFOLDERSINUPPERCASE},
        {&Opt.SaveHistory,FSS_SAVECOMMANDSHISTORY},
        {&Opt.SaveFoldersHistory,FSS_SAVEFOLDERSHISTORY},
        {&Opt.SaveViewHistory,FSS_SAVEVIEWANDEDITHISTORY},
        {&Opt.UseRegisteredTypes,FSS_USEWINDOWSREGISTEREDTYPES},
        {&Opt.AutoSaveSetup,FSS_AUTOSAVESETUP},
      };
      for(I=0; I < sizeof(OSys)/sizeof(OSys[0]); ++I)
        if(*OSys[I].Opt)
          Options|=OSys[I].Flags;
      return Options;
    }

    case ACTL_GETPANELSETTINGS:
    {
      DWORD Options=0;
      static struct Opt2Flags OSys[]={
        {&Opt.ShowHidden,FPS_SHOWHIDDENANDSYSTEMFILES},
        {&Opt.Highlight,FPS_HIGHLIGHTFILES},
        {&Opt.AutoChangeFolder,FPS_AUTOCHANGEFOLDER},
        {&Opt.SelectFolders,FPS_SELECTFOLDERS},
        {&Opt.ReverseSort,FPS_ALLOWREVERSESORTMODES},
        {&Opt.ShowColumnTitles,FPS_SHOWCOLUMNTITLES},
        {&Opt.ShowPanelStatus,FPS_SHOWSTATUSLINE},
        {&Opt.ShowPanelTotals,FPS_SHOWFILESTOTALINFORMATION},
        {&Opt.ShowPanelFree,FPS_SHOWFREESIZE},
        {&Opt.ShowPanelScrollbar,FPS_SHOWSCROLLBAR},
        {&Opt.ShowScreensNumber,FPS_SHOWBACKGROUNDSCREENSNUMBER},
        {&Opt.ShowSortMode,FPS_SHOWSORTMODELETTER},
      };
      for(I=0; I < sizeof(OSys)/sizeof(OSys[0]); ++I)
        if(*OSys[I].Opt)
          Options|=OSys[I].Flags;
      return Options;
    }

    case ACTL_GETINTERFACESETTINGS:
    {
      DWORD Options=0;
      static struct Opt2Flags OSys[]={
        {&Opt.Clock,FIS_CLOCKINPANELS},
        {&Opt.ViewerEditorClock,FIS_CLOCKINVIEWERANDEDITOR},
        {&Opt.Mouse,FIS_MOUSE},
        {&Opt.ShowKeyBar,FIS_SHOWKEYBAR},
        {&Opt.ShowMenuBar,FIS_ALWAYSSHOWMENUBAR},
        {&Opt.AltGr,FIS_USERIGHTALTASALTGR},
        {&Opt.CopyShowTotal,FIS_SHOWTOTALCOPYPROGRESSINDICATOR},
        {&Opt.CopyTimeRule,FIS_SHOWCOPYINGTIMEINFO},
        {&Opt.PgUpChangeDisk,FIS_USECTRLPGUPTOCHANGEDRIVE},
      };
      for(I=0; I < sizeof(OSys)/sizeof(OSys[0]); ++I)
        if(*OSys[I].Opt)
          Options|=OSys[I].Flags;
      return Options;
    }

    case ACTL_GETCONFIRMATIONS:
    {
      DWORD Options=0;
      static struct Opt2Flags OSys[]={
        {&Opt.Confirm.Copy,FCS_COPYOVERWRITE},
        {&Opt.Confirm.Move,FCS_MOVEOVERWRITE},
        {&Opt.Confirm.Drag,FCS_DRAGANDDROP},
        {&Opt.Confirm.Delete,FCS_DELETE},
        {&Opt.Confirm.DeleteFolder,FCS_DELETENONEMPTYFOLDERS},
        {&Opt.Confirm.Esc,FCS_INTERRUPTOPERATION},
        {&Opt.Confirm.RemoveConnection,FCS_DISCONNECTNETWORKDRIVE},
        {&Opt.Confirm.AllowReedit,FCS_RELOADEDITEDFILE},
        {&Opt.Confirm.HistoryClear,FCS_CLEARHISTORYLIST},
        {&Opt.Confirm.Exit,FCS_EXIT},
      };
      for(I=0; I < sizeof(OSys)/sizeof(OSys[0]); ++I)
        if(*OSys[I].Opt)
          Options|=OSys[I].Flags;
      return Options;
    }
    /* IS $ */

    /* $ 30.01.2002 DJ
       ACTL_GETDESCSETTINGS
    */
    case ACTL_GETDESCSETTINGS:
    {
      // ����� ���� - � �������� �� ��������������
      DWORD Options=0;
      if (Opt.Diz.UpdateMode == DIZ_UPDATE_IF_DISPLAYED)
        Options |= FDS_UPDATEIFDISPLAYED;
      else if (Opt.Diz.UpdateMode == DIZ_UPDATE_ALWAYS)
        Options |= FDS_UPDATEALWAYS;
      if (Opt.Diz.SetHidden)
        Options |= FDS_SETHIDDEN;
      if (Opt.Diz.ROUpdate)
        Options |= FDS_UPDATEREADONLY;
      return Options;
    }
    /* DJ $ */

    case ACTL_GETPOLICIES:
    {
      return Opt.Policies.DisabledOptions|(Opt.Policies.ShowHiddenDrives?FFPOL_SHOWHIDDENDRIVES:0);
    }

  }
  return FALSE;
}
#ifndef _MSC_VER
#pragma warn +par
#endif
/* IS $ */

int WINAPI FarMenuFn(int PluginNumber,int X,int Y,int MaxHeight,
           DWORD Flags,const char *Title,const char *Bottom,
           const char *HelpTopic, const int *BreakKeys,int *BreakCode,
           const struct FarMenuItem *Item, int ItemsNumber)
{
  int I;

  if (FrameManager->ManagerIsDown())
    return -1;

  if (DisablePluginsOutput)
    return(-1);

  if((DWORD)PluginNumber >= (DWORD)CtrlObject->Plugins.PluginsCount)
    return(-1); // � ���������.

  int ExitCode;
  {
    VMenu FarMenu(Title,NULL,0,MaxHeight);
    FarMenu.SetPosition(X,Y,0,0);
    if (BreakCode!=NULL)
      *BreakCode=-1;

    {
      char Topic[512];
      if(Help::MkTopic(PluginNumber,HelpTopic,Topic))
        FarMenu.SetHelp(Topic);
    }

    if (Bottom!=NULL)
      FarMenu.SetBottomTitle(Bottom);

    // ����� ����� ����
    DWORD MenuFlags=0;
    if (Flags & FMENU_SHOWAMPERSAND)
      MenuFlags|=VMENU_SHOWAMPERSAND;
    if (Flags & FMENU_WRAPMODE)
      MenuFlags|=VMENU_WRAPMODE;
    if (Flags & FMENU_CHANGECONSOLETITLE)
      MenuFlags|=VMENU_CHANGECONSOLETITLE;
    FarMenu.SetFlags(MenuFlags);

    struct MenuItem CurItem;
    memset(&CurItem,0,sizeof(CurItem));
    int Selected=0;

    if(Flags&FMENU_USEEXT)
    {
      struct FarMenuItemEx *ItemEx=(struct FarMenuItemEx*)Item;
      for (I=0; I < ItemsNumber; I++, ++ItemEx)
      {
        CurItem.Flags=ItemEx->Flags;
        CurItem.NamePtr=NULL; // �� ��� 4 ����� � 0 :-)

        // ��������� MultiSelected, �.�. � ��� ������ ������ � ����� �� ������������, ��������� ������ ������
        DWORD SelCurItem=CurItem.Flags&LIF_SELECTED;
        CurItem.Flags&=~LIF_SELECTED;
        if(!Selected && !(CurItem.Flags&LIF_SEPARATOR) && SelCurItem)
        {
          CurItem.Flags|=SelCurItem;
          Selected++;
        }

        if(CurItem.Flags&MIF_USETEXTPTR)
          CurItem.NamePtr=(char*)ItemEx->Text.TextPtr;
        else
          strncpy(CurItem.Name,ItemEx->Text.Text,sizeof(CurItem.Name)-1);
        /*
        strncpy(CurItem.Name,
            ((ItemEx->Flags&MIF_USETEXTPTR) && ItemEx->Text.TextPtr)?ItemEx->Text.TextPtr:ItemEx->Text.Text,
            sizeof(CurItem.Name)-1);
        */
        CurItem.AccelKey=(CurItem.Flags&LIF_SEPARATOR)?0:ItemEx->AccelKey;
        FarMenu.AddItem(&CurItem);
      }
    }
    else
    {
      for (I=0;I<ItemsNumber;I++)
      {
        CurItem.Flags=Item[I].Checked?(LIF_CHECKED|(Item[I].Checked&0xFFFF)):0;
        CurItem.Flags|=Item[I].Selected?LIF_SELECTED:0;
        CurItem.Flags|=Item[I].Separator?LIF_SEPARATOR:0;
        if(Item[I].Separator)
          CurItem.Name[0]=0;
        else
          strncpy(CurItem.Name,Item[I].Text,sizeof(CurItem.Name)-1);

        DWORD SelCurItem=CurItem.Flags&LIF_SELECTED;
        CurItem.Flags&=~LIF_SELECTED;
        if(!Selected && !(CurItem.Flags&LIF_SEPARATOR) && SelCurItem)
        {
          CurItem.Flags|=SelCurItem;
          Selected++;
        }
        FarMenu.AddItem(&CurItem);
      }
    }

    if(!Selected)
      FarMenu.SetSelectPos(0,1);

    // ����� ����, � ������� ���������
    if (Flags & FMENU_AUTOHIGHLIGHT)
      FarMenu.AssignHighlights(FALSE);
    if (Flags & FMENU_REVERSEAUTOHIGHLIGHT)
      FarMenu.AssignHighlights(TRUE);
    FarMenu.SetTitle(Title);
    FarMenu.Show();
    while (!FarMenu.Done() && !CloseFARMenu)
    {
      INPUT_RECORD ReadRec;
      int ReadKey=GetInputRecord(&ReadRec);
      if(ReadKey==KEY_CONSOLE_BUFFER_RESIZE)
      {
        LockScreen LckScr;
        FarMenu.Hide();
        FarMenu.Show();
      }
      else if (ReadRec.EventType==MOUSE_EVENT)
        FarMenu.ProcessMouse(&ReadRec.Event.MouseEvent);
      else if (ReadKey!=KEY_NONE)
      {
        if (BreakKeys!=NULL)
        {
          for (int I=0;BreakKeys[I]!=0;I++)
          {
            if(CtrlObject->Macro.IsExecuting())
            {
              int VirtKey,ControlState;
              TranslateKeyToVK(ReadKey,VirtKey,ControlState,&ReadRec);
            }
            if (ReadRec.Event.KeyEvent.wVirtualKeyCode==(BreakKeys[I] & 0xffff))
            {
              DWORD Flags=BreakKeys[I]>>16;
              /* $ 31.07.2001 IS
                 - ���: ���� ������� ����������� � ������ ������� ��
                   ctrl-key, alt-key ��� shift-key, ���� ���� ��� ����������
                   ����� �� ���� ������� � BreakKeys, � ������� ���� �����
                   ��������� ������� �� ������ key. �������: ��������� ����
                   ����� �� �������, �.�. ���������� ��� ������ �����.
              */
              DWORD RealFlags=ReadRec.Event.KeyEvent.dwControlKeyState &
                    (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED|
                    LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED|SHIFT_PRESSED);

              int Accept;
              if(RealFlags) // ������ shift, ctrl ��� alt
              {
                 Accept=FALSE; // �.�. ���� ������ �� ��������
                 if(Flags) // ������ ���� �������� � ������ ctrl|alt|shift
                 {
                   if ((Flags & PKF_CONTROL) &&
                       (RealFlags & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED)))
                     Accept=TRUE;
                   if ((Flags & PKF_ALT) &&
                       (RealFlags & (LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED)))
                     Accept=TRUE;
                   if ((Flags & PKF_SHIFT) && (RealFlags & SHIFT_PRESSED))
                     Accept=TRUE;
                 }
              }
              else
                 Accept=!Flags;  // TRUE ������, ���� ��� �� ����� ���������
                                 // ������ � ctrl|alt|shift
              /* IS $ */
              if (Accept)
              {
                if (BreakCode!=NULL)
                  *BreakCode=I;
                FarMenu.Hide();
//                  CheckScreenLock();
                return(FarMenu.GetSelectPos());
              }
            }
          }
        }
        FarMenu.ProcessKey(ReadKey);
      }
    }
    ExitCode=FarMenu.Modal::GetExitCode();
  }
//  CheckScreenLock();
  return(ExitCode);
}

/* $ 23.07.2000 SVS
   ������� ��� ������������ �������
*/
// ������� FarDefDlgProc ��������� ������� �� ���������
long WINAPI FarDefDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  if(hDlg)  // ��������� ������ ����� ��� hDlg=0
    return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
  return 0;
}

/* $ 28.07.2000 SVS
    ! � ����� � ���������� SendDlgMessage � ������ Dialog
      ������ ��������� ���������!
*/
// ������� ��������� �������
long WINAPI FarSendDlgMessage(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  if(hDlg) // ��������� ������ ����� ��� hDlg=0
    return Dialog::SendDlgMessage(hDlg,Msg,Param1,Param2);
  return 0;
}
/* SVS $ */

int WINAPI FarDialogFn(int PluginNumber,int X1,int Y1,int X2,int Y2,
           const char *HelpTopic,struct FarDialogItem *Item,int ItemsNumber)
{
  return FarDialogEx(PluginNumber,X1,Y1,X2,Y2,HelpTopic,Item,ItemsNumber,0,0,NULL,0);
}

/* $   13.12.2000 SVS
   ! FarDialogItem.Data - ����������� strcpy �������� �� memmove
   (�������� ������ ������������)
*/
#ifndef _MSC_VER
#pragma warn -par
#endif
/* ���� ������ ������� - ��������� ���� Flags - ������� ����, ���
   �� ����� ��� �� � �������
*/
static int Except_FarDialogEx(struct DialogItem *InternalItem)
{
  if(CtrlObject)
    CtrlObject->Plugins.Flags.Set(PSIF_DIALOG);

  // ���������
  delete[] InternalItem;

  Frame *frame;
  if((frame=FrameManager->GetBottomFrame()) != NULL)
  {
    //while(!frame->Refreshable()) // � ����� ��� ���� �����???
      frame->UnlockRefresh(); // ������ ����� :-)
  }
//  CheckScreenLock();
  FrameManager->RefreshFrame(); //??

  return EXCEPTION_CONTINUE_SEARCH; // ��������� ���������� ������� ����������!
}


int WINAPI FarDialogEx(int PluginNumber,int X1,int Y1,int X2,int Y2,
           const char *HelpTopic,struct FarDialogItem *Item,int ItemsNumber,
           DWORD Reserved, DWORD Flags,
           FARWINDOWPROC DlgProc,long Param)

{
  if (FrameManager->ManagerIsDown())
    return -1;

  if (DisablePluginsOutput ||
      ItemsNumber <= 0 ||
      !Item ||
      IsBadReadPtr(Item,sizeof(struct FarDialogItem)*ItemsNumber))
    return(-1);

  if((DWORD)PluginNumber >= (DWORD)CtrlObject->Plugins.PluginsCount)
    return(-1); // � ���������.

  // ����! ������ ��������� ������������� X2 � Y2
  if(X2 < 0 || Y2 < 0)
  {
    return -1;
  }

  struct DialogItem *InternalItem=new DialogItem[ItemsNumber];

  if(!InternalItem)
    return -1;

  int ExitCode=-1;

  //struct PluginItem *CurPlugin=&CtrlObject->Plugins.PluginsData[PluginNumber];

  memset(InternalItem,0,sizeof(DialogItem)*ItemsNumber);

  Dialog::ConvertItem(CVTITEM_FROMPLUGIN,Item,InternalItem,ItemsNumber);

  Frame *frame;
  if((frame=FrameManager->GetBottomFrame()) != NULL)
    frame->LockRefresh(); // ������� ���������� ������

  {
    Dialog FarDialog(InternalItem,ItemsNumber,DlgProc,Param);
    FarDialog.SetPosition(X1,Y1,X2,Y2);

    if(Flags & FDLG_WARNING)
      FarDialog.SetDialogMode(DMODE_WARNINGSTYLE);
    if(Flags & FDLG_SMALLDIALOG)
      FarDialog.SetDialogMode(DMODE_SMALLDIALOG);
    if(Flags & FDLG_NODRAWSHADOW)
      FarDialog.SetDialogMode(DMODE_NODRAWSHADOW);
    if(Flags & FDLG_NODRAWPANEL)
      FarDialog.SetDialogMode(DMODE_NODRAWPANEL);
    if(Flags & FDLG_NONMODAL)
      FarDialog.SetCanLoseFocus(TRUE);
    //FarDialog.SetOwnsItems(TRUE);
    FarDialog.SetHelp(HelpTopic);

    /* IS $ */
    /* $ 29.08.2000 SVS
       �������� ����� ������� - ������ � �������� ��� ������������ HelpTopic
    */
    FarDialog.SetPluginNumber(PluginNumber);
    /* SVS $ */

    int I;
    if(Opt.ExceptRules)
    {
      CtrlObject->Plugins.Flags.Clear(PSIF_DIALOG);
      TRY
      {
        FarDialog.Process();
        Dialog::ConvertItem(CVTITEM_TOPLUGIN,Item,InternalItem,ItemsNumber);
        for(I=0; I < ItemsNumber; ++I)
          InternalItem[I].ID=I;
        ExitCode=FarDialog.GetExitCode();
      }
      EXCEPT (Except_FarDialogEx(InternalItem))
      {
        ;
      }
    }
    else
    {
      FarDialog.Process();
      Dialog::ConvertItem(CVTITEM_TOPLUGIN,Item,InternalItem,ItemsNumber);
      for(I=0; I < ItemsNumber; ++I)
        InternalItem[I].ID=I;
      ExitCode=FarDialog.GetExitCode();
    }
  }

  delete[] InternalItem;

  /* $ 15.05.2002 SKV
    ������ ����������� ����� ����� ��, ��� ��������.
  */
  if(frame != NULL)
    frame->UnlockRefresh(); // ������ ����� :-)
 /* SKV $ */
//  CheckScreenLock();
  FrameManager->RefreshFrame(); //??
  return(ExitCode);
}
#ifndef _MSC_VER
#pragma warn +par
#endif
/* SVS 13.12.2000 $ */
/* SVS $ */

const char* WINAPI FarGetMsgFn(int PluginNumber,int MsgId)
{
  return(CtrlObject?CtrlObject->Plugins.FarGetMsg(PluginNumber,MsgId):"");
}

char* PluginsSet::FarGetMsg(int PluginNumber,int MsgId)
{
  if (PluginNumber<PluginsCount)
  {
    struct PluginItem *CurPlugin=&PluginsData[PluginNumber];
    char Path[NM];
    strncpy(Path,CurPlugin->ModuleName,sizeof(Path)-1);
    *PointToName(Path)=0;
    if (CurPlugin->Lang.Init(Path))
      return(CurPlugin->Lang.GetMsg(MsgId));
  }
  return("");
}

/* $ 28.01.2001 SVS
   ! ��������� ������� ������� FarMessageFn()
*/

int WINAPI FarMessageFn(int PluginNumber,DWORD Flags,const char *HelpTopic,
                        const char * const *Items,int ItemsNumber,
                        int ButtonsNumber)
{
  if (FrameManager->ManagerIsDown())
    return -1;

  if (DisablePluginsOutput)
    return(-1);

  if ((!(Flags&(FMSG_ALLINONE|FMSG_ERRORTYPE)) && ItemsNumber<2) || !Items)
    return(-1);

  if(PluginNumber != -1 && (DWORD)PluginNumber >= (DWORD)CtrlObject->Plugins.PluginsCount)
    return(-1); // � ���������.

  char *SingleItems=NULL;
  char *Msg;
  int I;

  // ������ ���������� ����� ��� FMSG_ALLINONE
  if(Flags&FMSG_ALLINONE)
  {
    ItemsNumber=0;
    I=strlen((char *)Items)+2;
    if((SingleItems=(char *)xf_malloc(I)) == NULL)
      return -1;

    Msg=strcpy(SingleItems,(char *)Items);
    while ((Msg = strchr(Msg, '\n')) != NULL)
    {
//      *Msg='\0';

      if(*++Msg == '\0')
        break;
      ++ItemsNumber;
    }
    ItemsNumber++; //??
  }

  const char **MsgItems=(const char **)xf_malloc(sizeof(char*)*(ItemsNumber+ADDSPACEFORPSTRFORMESSAGE));
  if(!MsgItems)
  {
    xf_free(SingleItems);
    return(-1);
  }

  memset(MsgItems,0,sizeof(char*)*(ItemsNumber+ADDSPACEFORPSTRFORMESSAGE));

  if(Flags&FMSG_ALLINONE)
  {
    I=0;
    Msg=SingleItems;

    // ������ ���������� ����� � �������� �� ������
    char *MsgTemp;
    while ((MsgTemp = strchr(Msg, '\n')) != NULL)
    {
      *MsgTemp='\0';
      MsgItems[I]=Msg;
      Msg+=strlen(Msg)+1;

      if(*Msg == '\0')
        break;
      ++I;
    }
    if(*Msg)
    {
      MsgItems[I]=Msg;
    }
  }
  else
  {
    for (I=0; I < ItemsNumber; I++)
      MsgItems[I]=Items[I];
  }

  // ����������� �� ������
  if(ItemsNumber > ScrY-2)
  {
    ItemsNumber=ScrY-2-(Flags&0x000F0000?1:0);
  }

  /* $ 22.03.2001 tran
     ItemsNumber++ -> ++ItemsNumber
     �������� ��������� ������� */
  switch(Flags&0x000F0000)
  {
    case FMSG_MB_OK:
      ButtonsNumber=1;
      MsgItems[ItemsNumber++]=MSG(MOk);
      break;
    case FMSG_MB_OKCANCEL:
      ButtonsNumber=2;
      MsgItems[ItemsNumber++]=MSG(MOk);
      MsgItems[ItemsNumber++]=MSG(MCancel);
      break;
    case FMSG_MB_ABORTRETRYIGNORE:
      ButtonsNumber=3;
      MsgItems[ItemsNumber++]=MSG(MAbort);
      MsgItems[ItemsNumber++]=MSG(MRetry);
      MsgItems[ItemsNumber++]=MSG(MIgnore);
      break;
    case FMSG_MB_YESNO:
      ButtonsNumber=2;
      MsgItems[ItemsNumber++]=MSG(MYes);
      MsgItems[ItemsNumber++]=MSG(MNo);
      break;
    case FMSG_MB_YESNOCANCEL:
      ButtonsNumber=3;
      MsgItems[ItemsNumber++]=MSG(MYes);
      MsgItems[ItemsNumber++]=MSG(MNo);
      MsgItems[ItemsNumber++]=MSG(MCancel);
      break;
    case FMSG_MB_RETRYCANCEL:
      ButtonsNumber=2;
      MsgItems[ItemsNumber++]=MSG(MRetry);
      MsgItems[ItemsNumber++]=MSG(MCancel);
      break;
  }
  /* tran $ */

  // ���������� �����
  if(PluginNumber != -1)
  {
    char Topic[512];
    if(Help::MkTopic(PluginNumber,HelpTopic,Topic))
      SetMessageHelp(Topic);
  }

  // ���������������... �����
  Frame *frame;
  if((frame=FrameManager->GetBottomFrame()) != NULL)
    frame->LockRefresh(); // ������� ���������� ������
  int MsgCode=Message(Flags,ButtonsNumber,MsgItems[0],MsgItems+1,ItemsNumber-1,PluginNumber);
  /* $ 15.05.2002 SKV
    ������ ����������� ���� ����� ��, ��� ��������.
  */
  if(frame != NULL)
    frame->UnlockRefresh(); // ������ ����� :-)
  /* SKV $ */
  //CheckScreenLock();

  if(SingleItems)
    xf_free(SingleItems);
  xf_free(MsgItems);
  return(MsgCode);
}

int WINAPI FarControl(HANDLE hPlugin,int Command,void *Param)
{
  _FCTLLOG(CleverSysLog("Control"));
  _FCTLLOG(SysLog("(hPlugin=0x%08X, Command=%s, Param=[%d/0x%08X])",hPlugin,_FCTL_ToName(Command),(int)Param,Param));
  _ALGO(CleverSysLog clv("FarControl"));
  _ALGO(SysLog("(hPlugin=0x%08X, Command=%s, Param=[%d/0x%08X])",hPlugin,_FCTL_ToName(Command),(int)Param,Param));

  if(Command == FCTL_CHECKPANELSEXIST)
    return CmdMode == FALSE?TRUE:FALSE;

  if (CmdMode || !CtrlObject || !FrameManager || FrameManager->ManagerIsDown())
    return 0;

  switch(Command)
  {
    case FCTL_CLOSEPLUGIN:
      strncpy(DirToSet,NullToEmpty((char *)Param),sizeof(DirToSet)-1);

    case FCTL_GETPANELINFO:
    case FCTL_GETANOTHERPANELINFO:
    case FCTL_GETPANELSHORTINFO:
    case FCTL_GETANOTHERPANELSHORTINFO:
    case FCTL_UPDATEPANEL:
    case FCTL_UPDATEANOTHERPANEL:
    case FCTL_REDRAWPANEL:
    case FCTL_REDRAWANOTHERPANEL:
    case FCTL_SETPANELDIR:
    case FCTL_SETANOTHERPANELDIR:
    case FCTL_SETSELECTION:
    case FCTL_SETANOTHERSELECTION:
    case FCTL_SETVIEWMODE:
    case FCTL_SETANOTHERVIEWMODE:
    case FCTL_SETSORTMODE:                 //  VVM 08.09.2000  + ����� ���������� �� �������
    case FCTL_SETANOTHERSORTMODE:
    case FCTL_SETSORTORDER:
    case FCTL_SETANOTHERSORTORDER:
    {
      if(!CtrlObject->Cp())
        return FALSE;

      if (hPlugin==INVALID_HANDLE_VALUE)
      {
        if(CtrlObject->Cp()->ActivePanel)
        {
          CtrlObject->Cp()->ActivePanel->SetPluginCommand(Command,Param);
          return TRUE;
        }
        return FALSE; //??
      }

      HANDLE hInternal;
      Panel *LeftPanel=CtrlObject->Cp()->LeftPanel;
      Panel *RightPanel=CtrlObject->Cp()->RightPanel;
      int Processed=FALSE;
      struct PluginHandle *PlHandle;

      if (LeftPanel && LeftPanel->GetMode()==PLUGIN_PANEL)
      {
        PlHandle=(struct PluginHandle *)LeftPanel->GetPluginHandle();
        if(PlHandle && !IsBadReadPtr(PlHandle,sizeof(struct PluginHandle)))
        {
          hInternal=PlHandle->InternalHandle;
          if (hPlugin==hInternal)
          {
            LeftPanel->SetPluginCommand(Command,Param);
            Processed=TRUE;
          }
        }
      }

      if (RightPanel && RightPanel->GetMode()==PLUGIN_PANEL)
      {
        PlHandle=(struct PluginHandle *)RightPanel->GetPluginHandle();
        if(PlHandle && !IsBadReadPtr(PlHandle,sizeof(struct PluginHandle)))
        {
          hInternal=PlHandle->InternalHandle;
          if (hPlugin==hInternal)
          {
            RightPanel->SetPluginCommand(Command,Param);
            Processed=TRUE;
          }
        }
      }

      return(Processed);
    }

    case FCTL_SETUSERSCREEN:
    {
      if (!CtrlObject->Cp() || !CtrlObject->Cp()->LeftPanel || !CtrlObject->Cp()->RightPanel)
        return(FALSE);

      KeepUserScreen++;
      CtrlObject->Cp()->LeftPanel->ProcessingPluginCommand++;
      CtrlObject->Cp()->RightPanel->ProcessingPluginCommand++;
      ScrBuf.FillBuf();
      SaveScreen SaveScr;
      {
        RedrawDesktop Redraw;
        CtrlObject->CmdLine->Hide();
        SaveScr.RestoreArea(FALSE);
      }
      KeepUserScreen--;
      CtrlObject->Cp()->LeftPanel->ProcessingPluginCommand--;
      CtrlObject->Cp()->RightPanel->ProcessingPluginCommand--;
      return(TRUE);
    }

    case FCTL_GETCMDLINE:
    case FCTL_GETCMDLINESELECTEDTEXT:
    {
      if(!IsBadWritePtr(Param,sizeof(char) * 1024))
      {
        if (Command==FCTL_GETCMDLINE)
          CtrlObject->CmdLine->GetString((char *)Param,1024);
        else
          CtrlObject->CmdLine->GetSelString((char *)Param,1024);
        return TRUE;
      }
      return FALSE;
    }

    case FCTL_SETCMDLINE:
    case FCTL_INSERTCMDLINE:
    {
      if (Command==FCTL_SETCMDLINE)
        CtrlObject->CmdLine->SetString(NullToEmpty((char *)Param));
      else
        CtrlObject->CmdLine->InsertString(NullToEmpty((char *)Param));
      CtrlObject->CmdLine->Redraw();
      return(TRUE);
    }

    case FCTL_SETCMDLINEPOS:
    {
      if(!IsBadReadPtr(Param,sizeof(int)))
      {
        CtrlObject->CmdLine->SetCurPos(*(int *)Param);
        CtrlObject->CmdLine->Redraw();
        return TRUE;
      }
      return FALSE;
    }

    case FCTL_GETCMDLINEPOS:
    {
      if(!IsBadWritePtr(Param,sizeof(int)))
      {
        *(int *)Param=CtrlObject->CmdLine->GetCurPos();
        return TRUE;
      }
      return FALSE;
    }

    case FCTL_GETCMDLINESELECTION:
    {
      CmdLineSelect *sel=(CmdLineSelect*)Param;
      if(sel && !IsBadWritePtr(sel,sizeof(struct CmdLineSelect)))
      {
        CtrlObject->CmdLine->GetSelection(sel->SelStart,sel->SelEnd);
        return TRUE;
      }
      return FALSE;
    }

    case FCTL_SETCMDLINESELECTION:
    {
      CmdLineSelect *sel=(CmdLineSelect*)Param;
      if(sel && !IsBadReadPtr(sel,sizeof(struct CmdLineSelect)))
      {
        CtrlObject->CmdLine->Select(sel->SelStart,sel->SelEnd);
        CtrlObject->CmdLine->Redraw();
        return TRUE;
      }
      return FALSE;
    }

  }
  return(FALSE);
}


HANDLE WINAPI FarSaveScreen(int X1,int Y1,int X2,int Y2)
{
  if (FrameManager->ManagerIsDown())
    return NULL;

  if (X2==-1)
    X2=ScrX;
  if (Y2==-1)
    Y2=ScrY;
  return((HANDLE)(new SaveScreen(X1,Y1,X2,Y2,FALSE)));
}


void WINAPI FarRestoreScreen(HANDLE hScreen)
{
  if (FrameManager->ManagerIsDown())
    return;
  if (hScreen==NULL)
    ScrBuf.FillBuf();
  if (hScreen)
    delete (SaveScreen *)hScreen;
}


static void PR_FarGetDirListMsg(void)
{
  Message(MSG_DOWN,0,"",MSG(MPreparingList));
}

int WINAPI FarGetDirList(const char *Dir,struct PluginPanelItem **pPanelItem,
                  int *pItemsNumber)
{
  if (FrameManager->ManagerIsDown())
    return 0;

  PluginPanelItem *ItemsList=NULL;
  int ItemsNumber=0;
  SaveScreen SaveScr;
  clock_t StartTime=clock();
  int MsgOut=0;

  *pItemsNumber=0;
  *pPanelItem=NULL;

  ScanTree ScTree(FALSE);
  WIN32_FIND_DATA FindData;
  char FullName[NM],DirName[NM];
//  ConvertNameToFull(Dir,DirName, sizeof(DirName));
  if (ConvertNameToFull(Dir,DirName, sizeof(DirName)) >= sizeof(DirName)){
    return FALSE;
  }
  ScTree.SetFindPath(DirName,"*.*");
  *PointToName(DirName)=0;
  int DirLength=strlen(DirName);
  while (ScTree.GetNextName(&FindData,FullName, sizeof (FullName)-1))
  {
    if ((ItemsNumber & 31)==0)
    {
      if (CheckForEsc())
      {
        if(ItemsList)
          xf_free(ItemsList);
        SetPreRedrawFunc(NULL);
        return(FALSE);
      }
      if (!MsgOut && clock()-StartTime > 500)
      {
        SetCursorType(FALSE,0);
        SetPreRedrawFunc(PR_FarGetDirListMsg);
        PR_FarGetDirListMsg();
        MsgOut=1;
      }
      ItemsList=(PluginPanelItem *)xf_realloc(ItemsList,sizeof(*ItemsList)*(ItemsNumber+32+1));
      if (ItemsList==NULL)
      {
        *pItemsNumber=0;
        SetPreRedrawFunc(NULL);
        return(FALSE);
      }
    }
    memset(&ItemsList[ItemsNumber],0,sizeof(*ItemsList));
    ItemsList[ItemsNumber].FindData=FindData;
    strcpy(ItemsList[ItemsNumber].FindData.cFileName,FullName+DirLength);
    ItemsNumber++;
  }
  SetPreRedrawFunc(NULL);
  *pPanelItem=ItemsList;
  *pItemsNumber=ItemsNumber;
  return(TRUE);
}


static struct PluginPanelItem *PluginDirList;
static int DirListItemsNumber;
static char PluginSearchPath[NM*16];
static int StopSearch;
static HANDLE hDirListPlugin;
static int PluginSearchMsgOut;
static struct
{
  PluginPanelItem *Addr;
  int ItemsNumber;
} DirListNumbers[16];

static void FarGetPluginDirListMsg(char *Name,DWORD Flags)
{
  Message(Flags,0,"",MSG(MPreparingList),Name);
  PreRedrawParam.Flags=Flags;
  PreRedrawParam.Param1=Name;
}

static void PR_FarGetPluginDirListMsg(void)
{
  FarGetPluginDirListMsg((char *)PreRedrawParam.Param1,PreRedrawParam.Flags&(~MSG_KEEPBACKGROUND));
}

int WINAPI FarGetPluginDirList(int PluginNumber,HANDLE hPlugin,
                  const char *Dir,struct PluginPanelItem **pPanelItem,
                  int *pItemsNumber)
{
  if (FrameManager->ManagerIsDown())
    return FALSE;

  {
    if (strcmp(Dir,".")==0 || TestParentFolderName(Dir))
      return(FALSE);

    static struct PluginHandle DirListPlugin;

    // � �� ����� �� ������ ���������� �� ������� ������?
    if (hPlugin==INVALID_HANDLE_VALUE)
    {
      /* $ 30.11.2001 DJ
         � ���������� �� ��� ������?
      */
      DWORD Handle = (DWORD) CtrlObject->Cp()->ActivePanel->GetPluginHandle();
      if (!Handle || Handle == 0xffffffff)
        return FALSE;

      DirListPlugin=*((struct PluginHandle *)Handle);
      /* DJ $ */
    }
    else
    {
      DirListPlugin.PluginNumber=PluginNumber;
      DirListPlugin.InternalHandle=hPlugin;
    }

    SaveScreen SaveScr;

    SetPreRedrawFunc(NULL);
    {
      char DirName[512];
      strncpy(DirName,Dir,sizeof(DirName)-1);
      TruncStr(DirName,30);
      CenterStr(DirName,DirName,30);
      SetCursorType(FALSE,0);

      SetPreRedrawFunc(PR_FarGetPluginDirListMsg);
      FarGetPluginDirListMsg(DirName,0);
      PluginSearchMsgOut=FALSE;

      hDirListPlugin=(HANDLE)&DirListPlugin;
      StopSearch=FALSE;
      *pItemsNumber=DirListItemsNumber=0;
      *pPanelItem=PluginDirList=NULL;
      struct OpenPluginInfo Info;
      CtrlObject->Plugins.GetOpenPluginInfo(hDirListPlugin,&Info);
      char PrevDir[NM];
      strcpy(PrevDir,Info.CurDir);
      if (CtrlObject->Plugins.SetDirectory(hDirListPlugin,Dir,OPM_FIND))
      {
        strcpy(PluginSearchPath,Dir);
        strcat(PluginSearchPath,"\x1");
        ScanPluginDir();
        *pPanelItem=PluginDirList;
        *pItemsNumber=DirListItemsNumber;
        CtrlObject->Plugins.SetDirectory(hDirListPlugin,"..",OPM_FIND);
        PluginPanelItem *PanelData=NULL;
        int ItemCount=0;
        if (CtrlObject->Plugins.GetFindData(hDirListPlugin,&PanelData,&ItemCount,OPM_FIND))
          CtrlObject->Plugins.FreeFindData(hDirListPlugin,PanelData,ItemCount);
        struct OpenPluginInfo NewInfo;
        CtrlObject->Plugins.GetOpenPluginInfo(hDirListPlugin,&NewInfo);
        if (LocalStricmp(PrevDir,NewInfo.CurDir)!=0)
          CtrlObject->Plugins.SetDirectory(hDirListPlugin,PrevDir,OPM_FIND);
      }
    }
    SetPreRedrawFunc(NULL);
  }

  if (!StopSearch)
    for (int I=0;I<sizeof(DirListNumbers)/sizeof(DirListNumbers[0]);I++)
      if (DirListNumbers[I].Addr==NULL)
      {
        DirListNumbers[I].Addr=*pPanelItem;
        DirListNumbers[I].ItemsNumber=*pItemsNumber;
        break;
      }
  return(!StopSearch);
}

/* $ 30.11.2001 DJ
   ������� � ������� ����� ��� ��� ����������� ������ � ScanPluginDir()
*/

static void CopyPluginDirItem (PluginPanelItem *CurPanelItem)
{
  char FullName[2*NM+1];
  sprintf(FullName,"%.*s%.*s",NM,PluginSearchPath,NM,CurPanelItem->FindData.cFileName);
  for (int I=0;FullName[I]!=0;I++)
    if (FullName[I]=='\x1')
      FullName[I]='\\';
  PluginPanelItem *DestItem=PluginDirList+DirListItemsNumber;
  *DestItem=*CurPanelItem;
  if (CurPanelItem->UserData && (CurPanelItem->Flags & PPIF_USERDATA))
  {
    DWORD Size=*(DWORD *)CurPanelItem->UserData;
    /* $ 13.07.2000 SVS
       ������ new ����� ������������ malloc
    */
    DestItem->UserData=(DWORD)xf_malloc(Size);
    /* SVS $*/
    memcpy((void *)DestItem->UserData,(void *)CurPanelItem->UserData,Size);
  }

  strncpy(DestItem->FindData.cFileName,FullName,sizeof(DestItem->FindData.cFileName)-1);
  DirListItemsNumber++;
}

/* DJ $ */


void ScanPluginDir()
{
  int I;
  PluginPanelItem *PanelData=NULL;
  int ItemCount=0;
  int AbortOp=FALSE;

  char DirName[NM];
  strncpy(DirName,PluginSearchPath,sizeof(DirName)-1);
  DirName[sizeof(DirName)-1]=0;
  for (I=0;DirName[I]!=0;I++)
    if (DirName[I]=='\x1')
      DirName[I]=DirName[I+1]==0 ? 0:'\\';
  TruncStr(DirName,30);
  CenterStr(DirName,DirName,30);

  if (CheckForEscSilent())
  {
    if (Opt.Confirm.Esc) // ����� ���������� ������?
      AbortOp=TRUE;

    if (ConfirmAbortOp())
      StopSearch=TRUE;
  }

  FarGetPluginDirListMsg(DirName,AbortOp?0:MSG_KEEPBACKGROUND);

  if (StopSearch || !CtrlObject->Plugins.GetFindData(hDirListPlugin,&PanelData,&ItemCount,OPM_FIND))
    return;
  struct PluginPanelItem *NewList=(struct PluginPanelItem *)xf_realloc(PluginDirList,1+sizeof(*PluginDirList)*(DirListItemsNumber+ItemCount));
  if (NewList==NULL)
  {
    StopSearch=TRUE;
    return;
  }
  PluginDirList=NewList;
  for (I=0;I<ItemCount && !StopSearch;I++)
  {
    PluginPanelItem *CurPanelItem=PanelData+I;
    /* $ 30.11.2001 DJ
       ������� ����������� ������ � �������
    */
    if ((CurPanelItem->FindData.dwFileAttributes & FA_DIREC)==0)
      CopyPluginDirItem (CurPanelItem);
    /* DJ $ */
  }
  for (I=0;I<ItemCount && !StopSearch;I++)
  {
    PluginPanelItem *CurPanelItem=PanelData+I;
    if ((CurPanelItem->FindData.dwFileAttributes & FA_DIREC) &&
        strcmp(CurPanelItem->FindData.cFileName,".")!=0 &&
        !TestParentFolderName(CurPanelItem->FindData.cFileName))

    {
      struct PluginPanelItem *NewList=(struct PluginPanelItem *)xf_realloc(PluginDirList,sizeof(*PluginDirList)*(DirListItemsNumber+1));
      if (NewList==NULL)
      {
        StopSearch=TRUE;
        return;
      }
      PluginDirList=NewList;
      /* $ 30.11.2001 DJ
         ���������� ����� ������� ��� ����������� FindData (�� ��������
         ���������� PPIF_USERDATA)
      */
      CopyPluginDirItem (CurPanelItem);
      /* DJ $ */
      if (CtrlObject->Plugins.SetDirectory(hDirListPlugin,CurPanelItem->FindData.cFileName,OPM_FIND))
      {
        strcat(PluginSearchPath,CurPanelItem->FindData.cFileName);
        strcat(PluginSearchPath,"\x1");
        if (strlen(PluginSearchPath)<sizeof(PluginSearchPath)-NM)
          ScanPluginDir();
        *strrchr(PluginSearchPath,'\x1')=0;
        char *NamePtr=strrchr(PluginSearchPath,'\x1');
        if (NamePtr!=NULL)
          *(NamePtr+1)=0;
        else
          *PluginSearchPath=0;
        if (!CtrlObject->Plugins.SetDirectory(hDirListPlugin,"..",OPM_FIND))
        {
          StopSearch=TRUE;
          break;
        }
      }
    }
  }
  CtrlObject->Plugins.FreeFindData(hDirListPlugin,PanelData,ItemCount);
}


void WINAPI FarFreeDirList(const struct PluginPanelItem *PanelItem)
{
  if (PanelItem==NULL)
    return;
  int ItemsNumber=0;
  int I;
  for (I=0;I<sizeof(DirListNumbers)/sizeof(DirListNumbers[0]);I++)
    if (DirListNumbers[I].Addr==PanelItem)
    {
      DirListNumbers[I].Addr=NULL;
      ItemsNumber=DirListNumbers[I].ItemsNumber;
      break;
    }

  for (I=0;I<ItemsNumber;I++)
  {
    const PluginPanelItem *CurPanelItem=PanelItem+I;
    if (CurPanelItem->UserData && (CurPanelItem->Flags & PPIF_USERDATA))
      /* $ 13.07.2000 SVS
        ��� ������� ������������ malloc
      */
      xf_free((void *)CurPanelItem->UserData);
      /* SVS $*/
  }
  /* $ 13.07.2000 SVS
    ��� ������� ������������ realloc
  */
  // ��� ��� �� ������??????????
  //xf_free(static_cast<void*>(const_cast<PluginPanelItem *>(PanelItem)));
  xf_free((void*)PanelItem);
  /* SVS $*/
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif
int WINAPI FarViewer(const char *FileName,const char *Title,
                     int X1,int Y1,int X2, int Y2,DWORD Flags)
{
  if (FrameManager->ManagerIsDown())
    return FALSE;
  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));
  /* $ 09.09.2001 IS */
  int DisableHistory=(Flags & VF_DISABLEHISTORY)?TRUE:FALSE;
  /* IS $ */
  /* $ 15.05.2002 SKV
    �������� ����� ������������ ��������� ������ �� ����������.
  */
  if( FrameManager->InModalEV())
  {
    Flags&=~VF_NONMODAL;
  }
  /* SKV $ */

  if (Flags & VF_NONMODAL)
  {
    /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
    FileViewer *Viewer=new FileViewer(FileName,TRUE,DisableHistory,Title,X1,Y1,X2,Y2);
    /* IS $ */
    if(!Viewer)
      return FALSE;
    /* $ 14.06.2002 IS
       ��������� VF_DELETEONLYFILEONCLOSE - ���� ���� ����� ����� ������
       ��������� �� ��������� � VF_DELETEONCLOSE
    */
    if (Flags & (VF_DELETEONCLOSE|VF_DELETEONLYFILEONCLOSE))
      Viewer->SetTempViewName(FileName,(Flags&VF_DELETEONCLOSE)?TRUE:FALSE);
    /* IS $ */
    /* $ 12.05.2001 DJ */
    Viewer->SetEnableF6 ((Flags & VF_ENABLE_F6) != 0);
    /* DJ $ */

    /* $ 21.05.2002 SKV
      ��������� ���� ���� ������ ����
      �� ��� ������ ����.
    */
    if(!(Flags&VF_IMMEDIATERETURN))
    {
      FrameManager->ExecuteNonModal();
    }
    else
      FrameManager->PluginCommit();
    /* SKV $ */
  }
  else
  {
    /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
    FileViewer Viewer (FileName,FALSE,DisableHistory,Title,X1,Y1,X2,Y2);
    /* IS $ */
    /* $ 28.05.2001 �� ��������� �����, ������� ����� ����� ������� ��������� ���� */
    Viewer.SetDynamicallyBorn(false);
    FrameManager->EnterModalEV();
    FrameManager->ExecuteModal();
    FrameManager->ExitModalEV();
    /* $ 14.06.2002 IS
       ��������� VF_DELETEONLYFILEONCLOSE - ���� ���� ����� ����� ������
       ��������� �� ��������� � VF_DELETEONCLOSE
    */
    if (Flags & (VF_DELETEONCLOSE|VF_DELETEONLYFILEONCLOSE))
      Viewer.SetTempViewName(FileName,(Flags&VF_DELETEONCLOSE)?TRUE:FALSE);
    /* IS $ */
    /* $ 12.05.2001 DJ */
    Viewer.SetEnableF6 ((Flags & VF_ENABLE_F6) != 0);
    /* DJ $ */
    SetConsoleTitle(OldTitle);
    if (!Viewer.GetExitCode()){
      return FALSE;
    }
  }
  return(TRUE);
}


int WINAPI FarEditor(const char *FileName,const char *Title,
                     int X1,int Y1,int X2,
                     int Y2,DWORD Flags,int StartLine,int StartChar)
{
  if (FrameManager->ManagerIsDown())
    return EEC_OPEN_ERROR;
  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));
  /* $ 12.07.2000 IS
   �������� ������ ��������� (������ ��� ��������������) � ��������
   ������������ ���������, ���� ���� ��������������� ����
  */
  /* $ 21.03.2001 VVM
    + ��������� ����� EF_CREATENEW */
  int CreateNew = (Flags & EF_CREATENEW)?TRUE:FALSE;
  /* VVM $ */
  /* $ 09.09.2001 IS */
  int DisableHistory=(Flags & EF_DISABLEHISTORY)?TRUE:FALSE;
  /* $ 14.06.2002 IS
     ��������� EF_DELETEONLYFILEONCLOSE - ���� ���� ����� ����� ������
     ��������� �� ��������� � EF_DELETEONCLOSE
  */
  int DeleteOnClose = 0;
  if(Flags & EF_DELETEONCLOSE)
    DeleteOnClose = 1;
  else if(Flags & EF_DELETEONLYFILEONCLOSE)
    DeleteOnClose = 2;
  /* IS 14.06.2002 $ */
  /* IS 09.09.2001 $ */
  int OpMode=FEOPMODE_QUERY;
#if 0
  if((Flags&(EF_USEEXISTING|EF_BREAKIFOPEN)) != (EF_USEEXISTING|EF_BREAKIFOPEN))
    OpMode=(Flags&EF_USEEXISTING)?FEOPMODE_USEEXISTING:FEOPMODE_BREAKIFOPEN;
#endif

  /*$ 15.05.2002 SKV
    �������� ����� ������������ ���������, ���� ��������� � ���������
    ��������� ��� ������.
  */
  if (FrameManager->InModalEV())
  {
    Flags&=~EF_NONMODAL;
  }
  /* SKV $ */

  int ExitCode=EEC_OPEN_ERROR;
  if (Flags & EF_NONMODAL)
  {
    /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
    FileEditor *Editor=new FileEditor(FileName,CreateNew,TRUE,
                                      StartLine,StartChar,Title,
                                      X1,Y1,X2,Y2,DisableHistory,
                                      DeleteOnClose,OpMode);
    /* IS $ */
    // ��������� - �������� ���� �������� (������ ��������� XC_OPEN_ERROR - ��. ��� FileEditor::Init())
    if (Editor && Editor->GetExitCode() == XC_OPEN_ERROR)
    {
      delete Editor;
      Editor=NULL;
    }

    if (Editor)
    {
      /* $ 12.05.2001 DJ */
      Editor->SetEnableF6 ((Flags & EF_ENABLE_F6) != 0);
      /* DJ $ */
      /* $ 21.05.2002 SKV
        ��������� ���� ����, ������ ����
        �� ��� ������ ����.
      */
      if(!(Flags&EF_IMMEDIATERETURN))
      {
        FrameManager->ExecuteNonModal();
      }
      else
        FrameManager->PluginCommit();
      /* SKV $ */
      ExitCode=XC_MODIFIED;
    }
  }
  else
  {
    /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
    FileEditor Editor(FileName,CreateNew,FALSE,
                      StartLine,StartChar,Title,
                      X1,Y1,X2,Y2,DisableHistory,
                      DeleteOnClose,OpMode);
    /* IS $ */

    // �������� ������������ (������ ������ ����� ����)
    if(Editor.GetExitCode() != XC_OPEN_ERROR)
    {
      Editor.SetDynamicallyBorn(false);
      /* $ 12.05.2001 DJ */
      Editor.SetEnableF6 ((Flags & EF_ENABLE_F6) != 0);
      /* DJ $ */
      SetConsoleTitle(OldTitle);
      /* $ 15.05.2002 SKV
        ����������� ���� � ����� �/�� ���������� ���������.
      */
      FrameManager->EnterModalEV();
      FrameManager->ExecuteModal();
      FrameManager->ExitModalEV();
      /* SKV $ */
      ExitCode = Editor.GetExitCode();
      if (ExitCode && ExitCode != XC_LOADING_INTERRUPTED)
      {
#if 0
         if(OpMode==FEOPMODE_BREAKIFOPEN && ExitCode==XC_QUIT)
           ExitCode = XC_OPEN_ERROR;
         else
#endif
           ExitCode = Editor.IsFileChanged()?XC_MODIFIED:XC_NOT_MODIFIED;
      }
    }
  }
  return ExitCode;
  /* IS $ */
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


int WINAPI FarCmpName(const char *pattern,const char *string,int skippath)
{
  return(CmpName(pattern,string,skippath));
}


int WINAPI FarCharTable(int Command,char *Buffer,int BufferSize)
{
  if (FrameManager->ManagerIsDown())
    return -1;

  struct CharTableSet TableSet;

  if (Command==FCT_DETECT)
  {
    char DataFileName[NM];
    FILE *DataFile;
    /* $ 19.06.2001
       - ���: �� �������� ���������������.
         ��, ����, ����� �� �� return -1 ������������� � 268??
    */
    if (!FarMkTempEx(DataFileName) || (DataFile=fopen(DataFileName,"w+b"))==NULL)
      return(-1);
    /* IS $ */
    fwrite(Buffer,1,BufferSize,DataFile);
    fseek(DataFile,0,SEEK_SET);
    int TableNum;
    int DetectCode=DetectTable(DataFile,&TableSet,TableNum);
    fclose(DataFile);
    remove(DataFileName);
    return(DetectCode ? TableNum-1:-1);
  }

  if (BufferSize > sizeof(struct CharTableSet))
    return(-1);

  /* $ 07.08.2001 IS
       ��� ������� �������� ��������� ������� ��� OEM
  */
  memcpy(&TableSet,Buffer,Min((int)sizeof(CharTableSet),BufferSize));
  /* $ 17.03.2002 IS �� ����������� ���������� �������� TableName */
  if (!PrepareTable(&TableSet,Command,TRUE))
  /* IS $ */
  {
    for(unsigned int i=0;i<256;++i)
    {
      TableSet.EncodeTable[i]=TableSet.DecodeTable[i]=i;
      TableSet.UpperTable[i]=LocalUpper(i);
      TableSet.LowerTable[i]=LocalLower(i);
    }
    strncpy(TableSet.TableName,MSG(MGetTableNormalText),sizeof(TableSet.TableName));
    // *TableSet.RFCCharset=0; // ���� ���!
    Command=-1;
  }
  memcpy(Buffer,&TableSet,BufferSize);
  /* IS $ */
  return(Command);
}


void WINAPI FarText(int X,int Y,int Color,const char *Str)
{
  if (FrameManager->ManagerIsDown())
    return;
  if (Str==NULL)
  {
    int PrevLockCount=ScrBuf.GetLockCount();
    ScrBuf.SetLockCount(0);
    ScrBuf.Flush();
    ScrBuf.SetLockCount(PrevLockCount);
  }
  else
  {
    /* $ 22.08.2000 SVS
       ��������� �������� ������ �� FarText.
    */
    Text(X,Y,Color,Str);
    /* SVS $ */
  }
}


int WINAPI FarEditorControl(int Command,void *Param)
{
  if (CtrlObject->Plugins.CurEditor==NULL || FrameManager->ManagerIsDown())
    return(0);
  return(CtrlObject->Plugins.CurEditor->EditorControl(Command,Param));
}

/* $ 27.09.2000 SVS
  ���������� ��������
*/
int WINAPI FarViewerControl(int Command,void *Param)
{
  if (CtrlObject->Plugins.CurViewer==NULL || FrameManager->ManagerIsDown())
    return(0);
  return(CtrlObject->Plugins.CurViewer->ViewerControl(Command,Param));
}
/* SVS $ */
