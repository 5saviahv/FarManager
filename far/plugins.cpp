/*
plugins.cpp

������ � ��������� (������ �������, ���-��� ������ � flplugin.cpp)

*/

/* Revision: 1.165 06.05.2006 $ */

/*
Modify:
  06.05.2006 SVS
    - � PluginsSet::ReadUserBackgound ����� ������������� ���.
    - Mantis#0000171: ����� Viewer � ������ VF_IMMEDIATERETURN �� GetFiles
  13.04.2006 SVS
    + � ������� PluginsSet::ProcessCommandLine ��� �������� - ��������� �� ������.
  23.01.2006 SVS
    + _ALGO
  24.07.2005 WARP
    ! see 02033.LockUnlock.txt
  24.04.2005 AY
    ! GCC
  04.02.2005 SVS
    - ���� � ���� ��������/������������ ������������� ������, �� ������ �������� ������ ��������� ������.
  31.01.2005 SVS
    - BugZ#1249 - �������������� ��������������� � ������� "Assign Hotkey"
  11.12.2004 WARP
    ! �������� � FE_CLOSE, ���������� �� ������ ������� ���
      ������� ������� ������ ������ ������, �������� ����� ������
      �� �����.
  28.10.2004 SVS
    - BugZ#825 - �� ���������� ������� �� ��������
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  02.08.2004 SVS
    - ���� ���������� ��������� ���-���� � Message, ��... ���� �� �������� ����.
  26.07.2004 SVS
    - strcpy()
  09.07.2004 SVS
    - far /p
      F11 F4 - ����
  29.05.2004 SVS
    - BugZ#1082 - ������ "&" � �������� ������
  19.05.2004 SVS
    ! ������ "SetFileAttributes(Name,0)" �������� "SetFileAttributes(Name,FILE_ATTRIBUTE_NORMAL)"
      ����� ���������� �� �����.
  01.03.2004 SVS
    + FSF.SetFileApisTo
  07.01.2004 SVS
    ! ��������� �����������.
  11.11.2003 SVS
    ! KeepUserScreen � DirToSet ���������� �� plugins.cpp � global.cpp
  29.10.2003 SVS
    + Opt.LoadPlug.SilentLoadPlugin - ����� ����� �������� ��������
  06.10.2003 SVS
    ! PluginsSet::ProcessEditorEvent() � PluginsSet::ProcessViewerEvent() ���������� �������� ���� int
  31.07.2003 SVS
    ! ������� CurPluginItem ������ ��� OpenPlugin. ���� ���������� ���...
  22.07.2003 SVS
    - �������� �����������
  15.07.2003 SVS
    + CurPluginItem - NULL ��� ��������� �� ������� ������
  17.06.2003 SVS
    - ���������� ������������� ���� ��� /p
  06.06.2003 SVS
    ! ���������� �������� ������ ����� ��� �������� ��������, �������� ����� UserDefinedList
  19.05.2003 SVS
    + ������� ��� �������� �������� �������� �� ERROR_PROC_NOT_FOUND
      (��� BugZ#884)
  14.05.2003 SVS
    + _ALGO()
  09.05.2003 SVS
    - BugZ#884 - ��������� ������������� ���-�� �� ������.
      ��������� "���� ����" ������ ERROR_BAD_EXE_FORMAT
  21.04.2003 SVS
    + IsPluginsLoaded(), PSIF_PLUGINSLOADDED
  31.03.2003 SVS
    + _ECTLLOG ��� PluginsSet::ProcessEditorEvent()
  17.03.2003 SVS
    ! �������� ����� ����� FFPOL_*
  25.02.2003 SVS
    - ������ ���� ����� ��������������� ��� � ���������� ��������� ���� "=A:"
      (��������� �����, ��...)
  05.02.2003 VVM
    - ����� �������� ������� ����������� ��� ����, ��� ���.
  04.02.2003 SVS
    - BugZ#784 - ������� ������� ��� �������� �������
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  10.01.2002 SVS
    - BugZ#767 - ����������� �������� �� ���������� ���
      �������, ���, ����� ������� _������ 1 ���_
  03.12.2002 SVS
    - BugZ#713 - �� �������� ��������� �� ������
  09.11.2002 SVS
    - F4 Esc � ���� ������������ � ������ ��������: ������ ���������� ����
      �� 1 ������. + ��������� ������� (AltF9), �.�. ����� ������������
      ���� �� � ��������� ��������� ���������� ���� �� ������ ������.
  18.09.2002 SVS
    ! ������ ������� ���������� ������� ����� ����� ����.
  17.09.2002 SVS
    - ���������� ��������
    ! ��� ������ ���� ����� "�����" �������� ������ ���������� �� �����
      ������. �������� ���������� ��� ��� ����� ������ ��������� ������
      ��� �� �����, ���������� �������.
  10.09.2002 SVS
    ! ������� ��������� ���������� ��� ������� - ���� ���� PSIF_DIALOG
      ���������� - ���������� �������� �������.
  03.09.2002 SVS
    - BugZ#611 - ������� ���������� � ���� ��������
  21.08.2002 IS
    + �������� PluginTextSize � GetDiskMenuItem, ����� �����, ������� �����
  25.06.2002 SVS
    ! ���������:  BitFlags::Skip -> BitFlags::Clear
  21.05.2002 IS
    + ������� �������� �������� ������� �������� ���� � ������
      ������������� ������ ��� PersonalPluginsPath
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
  14.05.2002 SKV
    + ���������� �� ������������� CurEditor
  08.05.2002 SVS
    ! �������� �� NULL ����� free()
  28.04.2002 IS
    ! ��������� const
  26.03.2002 DJ
    ! ScanTree::GetNextName() ��������� ������ ������ ��� ����� �����
  22.03.2002 SVS
    - strcpy - Fuck!
  20.03.2002 IS
    ! ProcessCommandLine ��������� const
  19.03.2002 OT
    - ����������� #96
  05.03.2002 DJ
    ! ����� ��-�� ������������ ������ � PluginsSet::ProcessCommandLine()
  21.02.2002 SVS
    ! �������� ������� �������� ������ � ������ ���� ������ �� ����
      ��������� ����������
    ! ����� BlockExtKey ����� � BlockExtKey.hpp
  21.02.2002 SVS
    - BugZ#317 - ��������� � ��������� ������ ����
      ��� ������ ����� - ����� BlockExtKey, ������� ��������� ���������
      ������� ��������, � �.�. � Alt-F9 - ��������� �� ��� ���, ����
      ������� �� �����������.
  19.02.2002 SVS
    ! ������ ������� ��������� �������... �� ���� �� �������. ������ ����
      ���� �������� (� ������������ ����� ��� �� ���� ;-)
  12.02.2002 SVS
    ! � ����� PluginHotkeys ������ ������ ���� � ��������. ��� ��� �� ������
      (��� � �������� ��� ��) �������� BugZ#302
  28.01.2002 SVS
    + ���, ������������� �� �������� � ���������������� ���������� ��������
      PluginStartupInfo � FarStandardFunctions ������� � ��������� �������:
      PluginsSet::CreatePluginStartupInfo()
  25.01.2002 SVS
    + PIWF_PRELOADED
  23.01.2002 SVS
    - BugZ#137 - ��������� ������� ���������� �������
    - ��������� ���� � UnloadPlugin (� �������� ���� :-))
    + RemoveTrailingSpaces � ProcessCommandLine() ����� ������� �������
  22.01.2002 SVS
    ! ��������� ������ �� ������� �������
    ! ��������� ���������������� �������� � UnloadPlugin
    - BugZ#52 exception handling for floating point incorrect
  15.01.2002 SVS
    ! ������ ����� �� ���������� ������ Editor ����� "����"
  14.01.2002 IS
    ! chdir -> FarChDir
  10.01.2002 SVS
    ! ������� _KEYMACRO
  25.12.2001 SVS
    ! "��������" ��������� 1.6� �� ������ ��������� �������� � ���
  14.12.2001 IS
    ! stricmp -> LocalStricmp
  07.12.2001 IS
    ! GetString -> FarInputBox
  05.12.2001 SVS
    ! ��������� ����������� �� ������ ����������
  01.11.2001 SVS
    ! ������ �� ����������� ��������� ������! ������ ����� ����, ���
      ���������� ����������.
  15.10.2001 SVS
    + _KEYMACRO()
  03.10.2001 SVS
    - ������ � PluginsSet::UnloadPlugin() - ����� ��� ��������� �������� ��?
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  26.09.2001 SVS
    + ������� 4 - ��������� ������� �������
    + ������� 20 - ������������ ���� � ������������ ��������
  24.09.2001 SVS
    + FSF.GetRepasePointInfo
    ! ������� ����������� (���������� ����). ������ �������� � �������
      ��������� ��������� - � ��� ������ � ������ ���������� ��������� ������
      �������� ������� ��������� :-)
  20.09.2001 SVS
    + ����� ���� Flags � ������ PluginsSet.
  16.09.2001 SVS
    ! ����������� ����������
  14.09.2001 SVS
    - BugZ#9 - ���������
  13.09.2001 SKV
    - �� ������� .dll ���� ��� �� ������������ �� ����� far plugin api �������.
  12.09.2001 SVS
    + FSF.ConvertNameToReal
  12.09.2001 SVS
    - BugZ#9 - ����� ����, ������������� �� ��������� ��������� �� ���������
          ������� ��������� �� KeyMacro::GetKey() � PluginsSet::OpenPlugin()
  09.09.2001 SVS
    ! � ������� PluginsSet::Configure() � PluginsSet::CommandsMenu()
      ����� ��� ���������� ������� ������� � ��������� "�����������"
      ������� GetMenuHotKey(). ��� ���� ����������� ������ ��������
      �� ��������� ����� ������� � ��������� ����������� ���.
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  24.07.2001 IS
    ! ������ �������� �� ' ' � '\t' �� ����� isspace
  23.07.2001 SVS
    - ���� ��� ���������� ���� ������ ������������ ��������!
      ��������� �������.
  19.07.2001 OT
    - F11->������ � ��������->CAS -> �����-�� ���������� ���� :)
  18.07.2001 OT
    VFMenu
  10.07.2001 SKV
    - ������ ���������� �������� �������
  26.06.2001 SVS
    ! __except -> EXCEPT
  25.06.2001 IS
    ! ��������� const
  07.06.2001 SVS
    + F4 - ���������� ������� � ������ ����.��������.
      ������ ����� ����������� ����, �.�. �� CommandsMenu � Configure
      �������� �������� ����� ���� � ������ �� � ���������, ���������������
      �������.
  03.06.2001 SVS
    + ConfigureCurrent() - ����� ������� ����������� �������
  03.06.2001 SVS
    ! ��������� � ����� � ���������� UserData � VMenu
  03.06.2001 OT
    - �� ���������� StatusLine ����� DrawLine � ���������
  30.05.2001 SVS
    + StandardFunctions.MkLink
  29.05.2001 IS
    - ��� ��������� "���������� ������� �������" �� ����������� �������
      �������� � ����.
  22.05.2001 DJ
    - GetMinFarVersion() �� ��������� � ���� ������ �� ���� ������;
      ����� ��������� � SetPluginStartupInfo()
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  16.05.2001 SVS
    ! ����� DumpPluginsInfo - � ����. ���� "�������" �������� ������
      ���������� � �������� :-)
  16.05.2001 SVS
    ! xfilter -> farexcpt.cpp
  14.05.2001 SVS
    + Opt.ShowCheckingFile - ��� ��������� ���������� � ���������...
  06.05.2001 DJ
    ! �������� #include
  04.05.2001 OT
    + ������� ������������� ���� �������� �� F11 (NWZ)
      ��������� PluginSet::CommandsMenu()
  29.04.2001 ��
    + ��������� NWZ �� ����������
  28.04.2001 SVS
    ! xfilter - ����� � ������ �������
    ! ��������� ��������� ����������
  08.06.2001 SVS
    ! ��������� �����������:
      StartupInfo � StandardFunctions ������� ������������ - �����������
      ���� ���, ����� ���������� � ��������� ��������� �...
      �.�. (���������) ���������� �������.
      �������������� ������ �������-��������� ������.
  04.04.2001 SVS
    ! ������� ��������� �������� ������ � CallPlugin()
  26.03.2001 SVS
    + �������������� �������� � CommandsMenu() - HistoryName
  24.03.2001 tran
    + qsortex
  20.03.2001 tran
    + ��� �������� ����� ����� OpenFilePlugin
      � ��������� ���� ������������ ��� �������
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  26.02.2001 VVM
    ! ��������� ��� ��������� NULL � OpenPlugin
    + ��������� ���������� ��� OpenPlugin(OPEN_FINDLIST)
  26.02.2001 VVM
    ! ��������� NULL ����� OpenPlugin
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  01.02.2001 SVS
    ! Shift-F9 ������� �� ������ ��������
  28.01.2001 SVS
    ! DumpExeptionInfo -> DumpExceptionInfo ;-)
  25.01.2001 SVS
    ! ������� ������������ ������� ���������� ��� ������� ����������� �����
      �������� OpenPluginInfo � PluginInfo
  23.01.2001 SVS
    + DumpExeptionInfo(xp); - ������ ���������� �� ���������� � ����.
  23.01.2001 skv
    + ������� EXCEPTION_BREAKPOINT � ������ ���������
    + Unknown Exception �� �� ���������.
  29.12.2000 IS
    ! ��� ��������� "���������� ������� �������" ��������� ���� � ��
      ������� ������ ��� ������� �� ESC
  19.12.2000 IS
    + Shift-F9 � ������ �������� �������� ��������� ���������/���������
      ���������, ���� ���� ������ ��� ������ �������������� ��
      ���������/��������� ���������. ��� ����������� ����� ��� ��������, ��
      � ������ ������.
  18.12.2000 SVS
    + Shift-F1 � ������ �������� �������� ���� �� ������� �������
  07.12.2000 SVS
    + �������� �� ������ ������, �� � ������ ����� � ������� �������� ������.
  27.11.2000 SVS
    ! �������� ������ Debug � ������� ���������� � ����������� �������
      ���������� ��������.
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  31.10.2000 SVS
    + ������� TestOpenPluginInfo - �������� �� �������� ����������
      �������� ������
    ! ��������� � ����������
  26.10.2000 SVS
    - ������ � "int Ret;" :-)
  23.10.2000 SVS
    + ������� TestPluginInfo - �������� �� �������� ���������� �������� ������
  19.10.2000 tran
    + /co & PF_PRELOAD = friendship forever
      ������ ����� :)
  17.10.2000 SVS
    + �����, � �������������� ������� ������� ������ try-__except
  16.10.2000 SVS
    + ��������� ���������� ��� ������ �������� ������� (���� ������ ��� ������
      ���� ������� - OpenPlugin � OpenFilePlugin).
  12.10.2000 tran
    + /co & PF_PRELOAD = friendship.
    + PluginsSet::DumpPluginsInfo(), call by AltF11 in plugins menu
  12.10.2000 IS
    + ��������� �� ProcessName � StandardFunctions
  27.09.2000 SVS
    + ��������� �� ������� Viewer
    + ProcessViewerEvent
    + CallPlugin
  27.09.2000 skv
    + DeleteBuffer
  24.09.2000 SVS
    + ������� FarNameToKey - ��������� ���� ������� �� �����
      ���� ��� �� ����� ��� ��� ������ - ������������ -1
  21.09.2000 SVS
    + ������ �  PluginItem.SysID - ��������� ������������� �������
  20.09.2000 SVS
    ! ������ FolderPresent (����, ������ ����� ������� :-(
  19.09.2000 SVS
    + ������� FolderPresent - "���������� �� �������"
  18.09.2000 SVS
    ! PluginsSet::SetPluginStartupInfo - �������� ��������� PluginStartupInfo
      ������.
    ! FarRecurseSearch -> FarRecursiveSearch
  14.09.2000 SVS
    + FSF.MkTemp
  10.09.2000 IS 1.21
    - ������ �������� Info.CommandPrefix �� NULL �������, �������������� ���
      ������ � ������������ �������, ������ - ���.
  10.09.2000 tran 1.21
    + FSF/FarRecurseSearch
  10.09.2000 SVS
    ! �������-�� ������� ���������� ��� ��� QWERTY -> Xlat.
  08.09.2000 SVS
    ! QWERTY -> Transliterate
  07.09.2000 SVS 1.20
    - MultiPrefix
      �� �����-�� ���������� �������� �� ���� ��� Flags ������������
      �������� ������ 0 (���� ���� ��� � ������� ����� 0x10) :-(
  07.09.2000 VVM 1.19
    + ��������� ��������� � �������, ����������� ����� ":"
    + ���� ���� PF_FULLCMDLINE - �������� � ���������
  07.09.2000 SVS
    + ������� GetFileOwner ���� �������� �������� :-)
    + ������� GetNumberOfLinks ���� �������� �������� :-)
    + �������� FarBsearch ��� �������� (������� bsearch)
  05.09.2000 SVS 1.17
    + QWERTY - �������������� - StandardFunctions.EDQwerty
  01.09.2000 tran 1.16
    + PluginsSet::LoadPluginsFromCache()
  31.08.2000 tran
    + FSF/FarInputRecordTokey
  31.08.2000 SVS
    ! ��������� FSF-�������
      FSF.RemoveLeadingSpaces =FSF.LTrim
      FSF.RemoveTrailingSpaces=FSF.RTrim
      FSF.RemoveExternalSpaces=FSF.Trim
  28.08.2000 SVS
    + ������� ��� Local*
    ! �� FarStandardFunctions._atoi64, �� FarStandardFunctions.atoi64
    + FARSTDITOA64
  25.08.2000 SVS
    ! ������� �� FSF �������:
      memset, memcpy, memmove, memcmp, strchr, strrchr, strstr, strtok, strpbrk
  03.08.2000 tran 1.12
    + GetMinFarVersion export
      ��� ����������� ����������-����������� ������ ����.
  03.08.2000 SVS
    + �����, ��� ����� ���� ������ �������� -P � ��������� ������...
  01.08.2000 SVS
    ! ���������� ��������������� ���� ��� ������ ������������ ��������
      ���������� ��������������� ����� �������
    + ��������� ���� ��� ���������� ���� ����� ������
  23.07.2000 SVS
    + �������
       - ���� �������� ������
       - FSF-������� KeyToName
       - FSF: ������ � ������� ������ CopyToClipboard, PasteFromClipboard
  23.07.2000 SVS
    + ������� ��� ����������� �������
       - ����������� ������� ������� FarDialogEx;
       - ����� ����������� SendDlgMessage;
       - ������� �� ��������� DefDlgProc;
  15.07.2000 SVS
    + ������� � ���� ������� ��������������� ���� ��� ������ ��������
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  13.07.2000 IS
    - �������� ���� ��� ����� � �������� (PluginsSet::ProcessEditorInput)
      ������� ��������� tran.
    - ��������� ���� � PluginsSet::SavePluginSettings, ���������� ���
      ������������ �������� ��� VC: ���������� I ���������� �� ���� ������
      (������������ ��� ����, ���� ���� $%#...)
      ������� ��������� tran.
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  07.07.2000 IS
    + �������������: atoi, _atoi64, itoa, RemoveLeadingSpaces,
      RemoveTrailingSpaces, RemoveExternalSpaces, TruncStr, TruncPathStr,
      QuoteSpaceOnly, PointToName, GetPathRoot, AddEndSlash
  06.07.2000 IS
    + ���������� ��������� ���� FarStandardFunctions (��. plugin.hpp)
      ������������� �� ������:
      StructSize, Unquote, ExpandEnvironmentStr, sprintf, sscanf, qsort,
      memcpy, memmove, memcmp, strchr, strrchr, strstr, strtok, memset, strpbrk
  05.07.2000 IS
    + ������� AdvControl
  01.07.2000 IS
    + ������� ������ ������ � api
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "plugins.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "plugin.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "global.hpp"
#include "scantree.hpp"
#include "chgprior.hpp"
#include "constitle.hpp"
#include "cmdline.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "rdrwdsk.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"
#include "udlist.hpp"
#include "farexcpt.hpp"
#include "fileedit.hpp"
#include "RefreshFrameManager.hpp"
#include "BlockExtKey.hpp"

#ifdef _MSC_VER
#pragma warning(disable:4509)
#endif


static void CheckScreenLock();

static const char FmtPluginsCache_PluginD[]="PluginsCache\\Plugin%d";
static const char FmtPluginsCache_PluginDExport[]="PluginsCache\\Plugin%d\\Exports";
static const char FmtDiskMenuStringD[]="DiskMenuString%d";
static const char FmtDiskMenuNumberD[]="DiskMenuNumber%d";
static const char FmtPluginMenuStringD[]="PluginMenuString%d";
static const char FmtPluginConfigStringD[]="PluginConfigString%d";

static const char NFMP_Preload[]="Preload";
static const char NFMP_SysID[]="SysID";

static const char NFMP_OpenPlugin[]="OpenPlugin";
static const char NFMP_OpenFilePlugin[]="OpenFilePlugin";
static const char NFMP_SetFindList[]="SetFindList";
static const char NFMP_ProcessEditorInput[]="ProcessEditorInput";
static const char NFMP_ProcessEditorEvent[]="ProcessEditorEvent";
static const char NFMP_ProcessViewerEvent[]="ProcessViewerEvent";
static const char NFMP_SetStartupInfo[]="SetStartupInfo";
static const char NFMP_ClosePlugin[]="ClosePlugin";
static const char NFMP_GetPluginInfo[]="GetPluginInfo";
static const char NFMP_GetOpenPluginInfo[]="GetOpenPluginInfo";
static const char NFMP_GetFindData[]="GetFindData";
static const char NFMP_FreeFindData[]="FreeFindData";
static const char NFMP_GetVirtualFindData[]="GetVirtualFindData";
static const char NFMP_FreeVirtualFindData[]="FreeVirtualFindData";
static const char NFMP_SetDirectory[]="SetDirectory";
static const char NFMP_GetFiles[]="GetFiles";
static const char NFMP_PutFiles[]="PutFiles";
static const char NFMP_DeleteFiles[]="DeleteFiles";
static const char NFMP_MakeDirectory[]="MakeDirectory";
static const char NFMP_ProcessHostFile[]="ProcessHostFile";
static const char NFMP_Configure[]="Configure";
static const char NFMP_ExitFAR[]="ExitFAR";
static const char NFMP_ProcessKey[]="ProcessKey";
static const char NFMP_ProcessEvent[]="ProcessEvent";
static const char NFMP_Compare[]="Compare";
static const char NFMP_GetMinFarVersion[]="GetMinFarVersion";

static const char RKN_PluginsCache[]="PluginsCache";

static int _cdecl PluginsSort(const void *el1,const void *el2);
static BOOL PrepareModulePath(const char *ModuleName);

PluginsSet::PluginsSet()
{
  PluginsData=NULL;
  PluginsCount=0;
  CurPluginItem=NULL;
  Reserved=0;
  CurEditor=NULL;
  CurViewer=NULL;
}


PluginsSet::~PluginsSet()
{
  CurPluginItem=NULL;
  struct PluginItem *PData=PluginsData;
  for (int I=0;I<PluginsCount;I++,PData++)
  {
    if (PData->WorkFlags.Check(PIWF_CACHED))
      continue;
    FreeLibrary(PData->hModule);
    PData->Lang.Close();
  }
  if(PluginsData) xf_free(PluginsData);
}

void PluginsSet::SendExit()
{
  struct PluginItem *PData=PluginsData;
  for (int I=0;I<PluginsCount;I++,PData++)
  {
    if (!PData->WorkFlags.Check(PIWF_CACHED) && PData->pExitFAR && !ProcessException)
    {
      //CurPluginItem=PData;
      PData->FuncFlags.Set(PICFF_EXITFAR);
      if(Opt.ExceptRules)
      {
        TRY{
          PData->pExitFAR();
        }
        EXCEPT(xfilter(EXCEPT_EXITFAR,GetExceptionInformation(),PData,1)){
          ;
        }
        ProcessException=FALSE;
      }
      else
        PData->pExitFAR();
      //CurPluginItem=NULL;
      PData->FuncFlags.Clear(PICFF_EXITFAR);
    }
  }
}


void PluginsSet::LoadPlugins()
{
  BlockExtKey blockExtKey;
  Flags.Clear(PSIF_PLUGINSLOADDED);

  if (Opt.LoadPlug.PluginsCacheOnly)  // $ 01.09.2000 tran  '/co' switch
  {
     LoadPluginsFromCache();
     return;
  }

  if(Opt.LoadPlug.MainPluginDir || *Opt.LoadPlug.CustomPluginsPath || (Opt.LoadPlug.PluginsPersonal && *Opt.LoadPlug.PersonalPluginsPath))
  {
    ScanTree ScTree(FALSE,TRUE);
    UserDefinedList PluginPathList;  // �������� ������ ���������

    int I;
    char PluginsDir[1024],FullName[1024];
    WIN32_FIND_DATA FindData;
    struct PluginItem *PData;

    PluginPathList.SetParameters(0,0,ULF_UNIQUE);
    // ������� ���������� ������
    if(Opt.LoadPlug.MainPluginDir) // ������ �������� � ������������?
    {
      sprintf(PluginsDir,"%s%s",FarPath,PluginsFolderName);
      PluginPathList.AddItem(PluginsDir);
      // ...� ������������ ����?
      if(Opt.LoadPlug.PluginsPersonal && *Opt.LoadPlug.PersonalPluginsPath && !(Opt.Policies.DisabledOptions&FFPOL_PERSONALPATH))
        PluginPathList.AddItem(Opt.LoadPlug.PersonalPluginsPath);
    }
    else if(*Opt.LoadPlug.CustomPluginsPath) // ������ "��������" ����?
      PluginPathList.AddItem(Opt.LoadPlug.CustomPluginsPath);

    const char *NamePtr;
    PluginPathList.Reset();
    // ������ ��������� �� ����� ����� ���������� ������
    while(NULL!=(NamePtr=PluginPathList.GetNext()))
    {
      // ��������� �������� ����
      ExpandEnvironmentStr(NamePtr,FullName,sizeof(FullName));
      Unquote(FullName); //??? ����� ��
      if(!PathMayBeAbsolute(FullName))
      {
        xstrncpy(PluginsDir,FarPath,sizeof(PluginsDir)-1);
        strncat(PluginsDir,FullName,sizeof(PluginsDir)-1);
        strcpy(FullName,PluginsDir);
      }
      // ������� �������� �������� ������� �������� ���� � ������ ������������� ������.
      ConvertNameToReal(FullName,FullName,sizeof(FullName));
      RawConvertShortNameToLongName(FullName,FullName,sizeof(FullName));
      xstrncpy(PluginsDir,FullName,sizeof(PluginsDir)-1);

      if(!PluginsDir[0]) // ���... � ����� �� ��� ������� ����� ����� ������������ ��������� ��������?
        continue;

      // ������ �� ����� ��������� ���� �� ������...
      ScTree.SetFindPath(PluginsDir,"*.*");

      // ...� ��������� �� ����
      while (ScTree.GetNextName(&FindData,FullName,sizeof (FullName)-1))
      {
        if (CmpName("*.dll",FindData.cFileName,FALSE) && (FindData.dwFileAttributes & FA_DIREC)==0)
        {
          struct PluginItem CurPlugin;
          char RegKey[100];
          memset(&CurPlugin,0,sizeof(CurPlugin));
          xstrncpy(CurPlugin.ModuleName,FullName,sizeof(CurPlugin.ModuleName)-1);
          int CachePos=GetCacheNumber(FullName,&FindData,0);
          int LoadCached;
          if(CachePos!=-1)
          {
            LoadCached=TRUE;
            /* $ 12.10.2000 tran
               Preload=1 ����� ��� ���������� ��������� -co */
            sprintf(RegKey,FmtPluginsCache_PluginD,CachePos);
            if ( GetRegKey(RegKey,NFMP_Preload,0)==1 )
            {
              LoadCached=FALSE;
              CachePos=-1;
            }
            /* tran $ */
          }
          else
            LoadCached=FALSE;

          if (LoadCached)
          {
            char RegKey[100];
            sprintf(RegKey,FmtPluginsCache_PluginDExport,CachePos);
            CurPlugin.SysID=GetRegKey(RegKey,NFMP_SysID,0);
            CurPlugin.pOpenPlugin=(PLUGINOPENPLUGIN)GetRegKey(RegKey,NFMP_OpenPlugin,0);
            CurPlugin.pOpenFilePlugin=(PLUGINOPENFILEPLUGIN)GetRegKey(RegKey,NFMP_OpenFilePlugin,0);
            CurPlugin.pSetFindList=(PLUGINSETFINDLIST)GetRegKey(RegKey,NFMP_SetFindList,0);
            CurPlugin.pProcessEditorInput=(PLUGINPROCESSEDITORINPUT)GetRegKey(RegKey,NFMP_ProcessEditorInput,0);
            CurPlugin.pProcessEditorEvent=(PLUGINPROCESSEDITOREVENT)GetRegKey(RegKey,NFMP_ProcessEditorEvent,0);
            CurPlugin.pProcessViewerEvent=(PLUGINPROCESSVIEWEREVENT)GetRegKey(RegKey,NFMP_ProcessViewerEvent,0);
            CurPlugin.CachePos=CachePos;
          }
          if (LoadCached || LoadPlugin(CurPlugin,-1,TRUE))
          {
            struct PluginItem *NewPluginsData=(struct PluginItem *)xf_realloc(PluginsData,sizeof(*PluginsData)*(PluginsCount+1));
            if (NewPluginsData==NULL)
              break;

            PluginsData=NewPluginsData;
            CurPlugin.WorkFlags.Change(PIWF_CACHED,LoadCached);
            CurPlugin.FindData=FindData;
            PluginsData[PluginsCount]=CurPlugin;
            PluginsCount++;
          }
        }
      } // end while
    }

    far_qsort(PluginsData,PluginsCount,sizeof(*PluginsData),PluginsSort);

    int NewPlugin=FALSE;

    for (PData=PluginsData,I=0;I<PluginsCount;I++,PData++)
    {
      if (!PData->WorkFlags.Check(PIWF_CACHED))
      {
        SetPluginStartupInfo(*PData,I);
        if (SavePluginSettings(*PData,PData->FindData))
          NewPlugin=TRUE;
      }
    }

    if (NewPlugin)
    {
      for (int I=0;;I++)
      {
        char RegKey[100],PluginName[NM];
        sprintf(RegKey,FmtPluginsCache_PluginD,I);
        GetRegKey(RegKey,"Name",PluginName,"",sizeof(PluginName));
        if (*PluginName==0)
          break;
        if (GetFileAttributes(PluginName)==0xFFFFFFFF)
        {
          DeleteKeyRecord(FmtPluginsCache_PluginD,I);
          I--;
        }
      }
    }
  }
  Flags.Set(PSIF_PLUGINSLOADDED);
}

/* $ 01.09.2000 tran
   Load cache only plugins  - '/co' switch */
void PluginsSet::LoadPluginsFromCache()
{
  /*
    [HKEY_CURRENT_USER\Software\Far\PluginsCache\Plugin0]
    "Name"="C:\\PROGRAM FILES\\FAR\\Plugins\\ABOOK\\AddrBook.dll"
    "ID"="e400a14def00a37ea900"
    "DiskMenuString0"="Address Book"
    "PluginMenuString0"="Address Book"
    "PluginConfigString0"="Address Book"
    "PluginConfigString1"="Address: E-Mail"
    "PluginConfigString2"="Address: Birthday"
    "PluginConfigString3"="Address: Phone number"
    "PluginConfigString4"="Address: Fidonet"
    "CommandPrefix"=""
    "Flags"=dword:00000000

    [HKEY_CURRENT_USER\Software\Far\PluginsCache\Plugin0\Exports]
    "OpenPlugin"=dword:00000001
    "OpenFilePlugin"=dword:00000000
    "SetFindList"=dword:00000000
    "ProcessEditorInput"=dword:00000000
    "ProcessEditorEvent"=dword:00000000
  */
  int I;
  char PlgKey[512];
  char RegKey[512];
  struct PluginItem CurPlugin;

  for (I=0;;I++)
  {
    if (!EnumRegKey(RKN_PluginsCache,I,PlgKey,sizeof(PlgKey)))
      break;

    memset(&CurPlugin,0,sizeof(CurPlugin));
                        //  012345678901234567890
    strcpy(RegKey,PlgKey); // "PLuginsCache\PluginXX"
    GetRegKey(RegKey,"Name",CurPlugin.ModuleName,"",NM);
    /* $ 12.10.2000 tran
      -co ������ �������� PRELOAD ������� */
    if ( GetRegKey(RegKey,NFMP_Preload,0)==1 )
    {

      if (!LoadPlugin(CurPlugin,-1,TRUE))
        continue; // �������� �� �������
      CurPlugin.WorkFlags.Clear(PIWF_CACHED);
    }
    else
    {
    /* tran $ */
      strcat(RegKey,"\\");
      strcat(RegKey,"Exports");
      CurPlugin.SysID=GetRegKey(RegKey,NFMP_SysID,0);
      CurPlugin.pOpenPlugin=(PLUGINOPENPLUGIN)GetRegKey(RegKey,NFMP_OpenPlugin,0);
      CurPlugin.pOpenFilePlugin=(PLUGINOPENFILEPLUGIN)GetRegKey(RegKey,NFMP_OpenFilePlugin,0);
      CurPlugin.pSetFindList=(PLUGINSETFINDLIST)GetRegKey(RegKey,NFMP_SetFindList,0);
      CurPlugin.pProcessEditorInput=(PLUGINPROCESSEDITORINPUT)GetRegKey(RegKey,NFMP_ProcessEditorInput,0);
      CurPlugin.pProcessEditorEvent=(PLUGINPROCESSEDITOREVENT)GetRegKey(RegKey,NFMP_ProcessEditorEvent,0);
      CurPlugin.pProcessViewerEvent=(PLUGINPROCESSVIEWEREVENT)GetRegKey(RegKey,NFMP_ProcessViewerEvent,0);
      CurPlugin.CachePos=atoi(PlgKey+19);
      CurPlugin.WorkFlags.Set(PIWF_CACHED);
      // ��� ��� ��� ���� �� ���������, �������, ��� ��� �� ��������
      // CurPlugin.FindData=FindData;
    }
    struct PluginItem *NewPluginsData=(struct PluginItem *)xf_realloc(PluginsData,sizeof(*PluginsData)*(PluginsCount+1));
    if (NewPluginsData==NULL)
        break;
    PluginsData=NewPluginsData;
    PluginsData[PluginsCount]=CurPlugin;
    PluginsCount++;
  }
  far_qsort(PluginsData,PluginsCount,sizeof(*PluginsData),PluginsSort);
  /* $ 19.10.2000 tran
     ����� ������� SetStartupInfo :) */
  struct PluginItem *PData=PluginsData;
  for (I=0;I<PluginsCount;I++,PData++)
  {
    if (!PData->WorkFlags.Check(PIWF_CACHED))
    {
      SetPluginStartupInfo(*PData,I);
    }
  }
  /* tran $ */
  Flags.Set(PSIF_PLUGINSLOADDED);
}
/* tran $ */

int _cdecl PluginsSort(const void *el1,const void *el2)
{
  struct PluginItem *Plugin1=(struct PluginItem *)el1;
  struct PluginItem *Plugin2=(struct PluginItem *)el2;
  return(LocalStricmp(PointToName(Plugin1->ModuleName),PointToName(Plugin2->ModuleName)));
}

static BOOL PrepareModulePath(const char *ModuleName)
{
  char ModulePath[NM];
  xstrncpy(ModulePath,ModuleName,sizeof(ModulePath)-1);
  *PointToName(ModulePath)=0;
  return FarChDir(ModulePath,TRUE);
}

int PluginsSet::LoadPlugin(struct PluginItem &CurPlugin,int ModuleNumber,int Init)
{
  if(CurPlugin.WorkFlags.Check(PIWF_DONTLOADAGAIN))
  {
    return (FALSE);
  }

  DWORD LstErr;
  HMODULE hModule=CurPlugin.hModule;
  if(!hModule)
  {
   /* $ 05.02.2003 VVM
     - ����� �������� ������� ����������� ��� ����, ��� ��� */
    char CurPath[1024], CurPlugDiskPath[1024], Drive[4];
    Drive[0]=0; // ������ 0, ��� ������� ����, ��� ������� ������� ������!
    FarGetCurDir(sizeof(CurPath)-1,CurPath);
    if(IsLocalPath(CurPlugin.ModuleName)) // ���� ������ ��������� ����, ��...
    {
      // ...������� ��������������� ���������� ���������
      strcpy(Drive,"= :");
      Drive[1]=CurPlugin.ModuleName[0];
      GetEnvironmentVariable(Drive,CurPlugDiskPath,sizeof(CurPlugDiskPath));
    }
    PrepareModulePath(CurPlugin.ModuleName);
    hModule=LoadLibraryEx(CurPlugin.ModuleName,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
    if(!hModule)
      LstErr=GetLastError();
    FarChDir(CurPath, TRUE);
    if(Drive[0]) // ������ �� (���������� ���������) �������
      SetEnvironmentVariable(Drive,CurPlugDiskPath);
    /* VVM $ */
  }

  /* "...� ������ ��������� �������� � DONT_RESOLVE_DLL_REFERENCES..."  */
  if (!hModule && !(LstErr == ERROR_BAD_EXE_FORMAT || LstErr == ERROR_PROC_NOT_FOUND))
  {
    if(!Opt.LoadPlug.SilentLoadPlugin)
    {
      char PlgName[NM];
      xstrncpy(PlgName,CurPlugin.ModuleName,sizeof(PlgName)-1);
      TruncPathStr(PlgName,ScrX-20);
      SetMessageHelp("ErrLoadPlugin");
      Message(MSG_WARNING,1,MSG(MError),MSG(MPlgLoadPluginError),PlgName,MSG(MOk));
    }
    CurPlugin.WorkFlags.Set(PIWF_DONTLOADAGAIN);
    return(FALSE);
  }

  CurPlugin.hModule=hModule;
  CurPlugin.WorkFlags.Clear(PIWF_CACHED);
  CurPlugin.pSetStartupInfo=(PLUGINSETSTARTUPINFO)GetProcAddress(hModule,NFMP_SetStartupInfo);
  CurPlugin.pOpenPlugin=(PLUGINOPENPLUGIN)GetProcAddress(hModule,NFMP_OpenPlugin);
  CurPlugin.pOpenFilePlugin=(PLUGINOPENFILEPLUGIN)GetProcAddress(hModule,NFMP_OpenFilePlugin);
  CurPlugin.pClosePlugin=(PLUGINCLOSEPLUGIN)GetProcAddress(hModule,NFMP_ClosePlugin);
  CurPlugin.pGetPluginInfo=(PLUGINGETPLUGININFO)GetProcAddress(hModule,NFMP_GetPluginInfo);
  CurPlugin.pGetOpenPluginInfo=(PLUGINGETOPENPLUGININFO)GetProcAddress(hModule,NFMP_GetOpenPluginInfo);
  CurPlugin.pGetFindData=(PLUGINGETFINDDATA)GetProcAddress(hModule,NFMP_GetFindData);
  CurPlugin.pFreeFindData=(PLUGINFREEFINDDATA)GetProcAddress(hModule,NFMP_FreeFindData);
  CurPlugin.pGetVirtualFindData=(PLUGINGETVIRTUALFINDDATA)GetProcAddress(hModule,NFMP_GetVirtualFindData);
  CurPlugin.pFreeVirtualFindData=(PLUGINFREEVIRTUALFINDDATA)GetProcAddress(hModule,NFMP_FreeVirtualFindData);
  CurPlugin.pSetDirectory=(PLUGINSETDIRECTORY)GetProcAddress(hModule,NFMP_SetDirectory);
  CurPlugin.pGetFiles=(PLUGINGETFILES)GetProcAddress(hModule,NFMP_GetFiles);
  CurPlugin.pPutFiles=(PLUGINPUTFILES)GetProcAddress(hModule,NFMP_PutFiles);
  CurPlugin.pDeleteFiles=(PLUGINDELETEFILES)GetProcAddress(hModule,NFMP_DeleteFiles);
  CurPlugin.pMakeDirectory=(PLUGINMAKEDIRECTORY)GetProcAddress(hModule,NFMP_MakeDirectory);
  CurPlugin.pProcessHostFile=(PLUGINPROCESSHOSTFILE)GetProcAddress(hModule,NFMP_ProcessHostFile);
  CurPlugin.pSetFindList=(PLUGINSETFINDLIST)GetProcAddress(hModule,NFMP_SetFindList);
  CurPlugin.pConfigure=(PLUGINCONFIGURE)GetProcAddress(hModule,NFMP_Configure);
  CurPlugin.pExitFAR=(PLUGINEXITFAR)GetProcAddress(hModule,NFMP_ExitFAR);
  CurPlugin.pProcessKey=(PLUGINPROCESSKEY)GetProcAddress(hModule,NFMP_ProcessKey);
  CurPlugin.pProcessEvent=(PLUGINPROCESSEVENT)GetProcAddress(hModule,NFMP_ProcessEvent);
  CurPlugin.pCompare=(PLUGINCOMPARE)GetProcAddress(hModule,NFMP_Compare);
  CurPlugin.pProcessEditorInput=(PLUGINPROCESSEDITORINPUT)GetProcAddress(hModule,NFMP_ProcessEditorInput);
  CurPlugin.pProcessEditorEvent=(PLUGINPROCESSEDITOREVENT)GetProcAddress(hModule,NFMP_ProcessEditorEvent);
  CurPlugin.pProcessViewerEvent=(PLUGINPROCESSVIEWEREVENT)GetProcAddress(hModule,NFMP_ProcessViewerEvent);
  CurPlugin.pMinFarVersion=(PLUGINMINFARVERSION)GetProcAddress(hModule,NFMP_GetMinFarVersion);
  CurPlugin.LinkedFrame=NULL;
  /*$ 13.09.2001 SKV
    ���� ������ �� ������������ �� �����
    �������, �� �� ����� ��� �������.
  */
  if(!(CurPlugin.pSetStartupInfo ||
    CurPlugin.pOpenPlugin ||
    CurPlugin.pOpenFilePlugin ||
    CurPlugin.pClosePlugin ||
    CurPlugin.pGetPluginInfo ||
    CurPlugin.pGetOpenPluginInfo ||
    CurPlugin.pGetFindData ||
    CurPlugin.pFreeFindData ||
    CurPlugin.pGetVirtualFindData ||
    CurPlugin.pFreeVirtualFindData ||
    CurPlugin.pSetDirectory ||
    CurPlugin.pGetFiles ||
    CurPlugin.pPutFiles ||
    CurPlugin.pDeleteFiles ||
    CurPlugin.pMakeDirectory ||
    CurPlugin.pProcessHostFile ||
    CurPlugin.pSetFindList ||
    CurPlugin.pConfigure ||
    CurPlugin.pExitFAR ||
    CurPlugin.pProcessKey ||
    CurPlugin.pProcessEvent ||
    CurPlugin.pCompare ||
    CurPlugin.pProcessEditorInput ||
    CurPlugin.pProcessEditorEvent ||
    CurPlugin.pProcessViewerEvent ||
    CurPlugin.pMinFarVersion))
  {
    FreeLibrary(hModule);
    CurPlugin.hModule=0;
    CurPlugin.WorkFlags.Set(PIWF_DONTLOADAGAIN);
    return FALSE;
  }
  /* SKV$*/
  CurPlugin.FuncFlags.Set(PICFF_LOADED);
  if (ModuleNumber!=-1 && Init)
  {
    /* $ 22.05.2001 DJ
       �������� ���. ������ ���������� � SetPluginStartupInfo()
    */
    return(SetPluginStartupInfo(CurPlugin,ModuleNumber));
    /* DJ $ */
  }
  return(TRUE);
}

/* $ 03.08.2000 tran
   ������� �������� ����������� ������ */
/* $ 07.12.2000 SVS
   �������� �� ������ ������, �� � ������ �����
*/
int  PluginsSet::CheckMinVersion(struct PluginItem &CurPlugin)
{
  DWORD FVer;

  if ( CurPlugin.pMinFarVersion==0 || ProcessException) // ������ �� ������������, ��� ��� �������, ��� �� ��� <1.65
    return (TRUE);

  //CurPluginItem=&CurPlugin; //??
  CurPlugin.FuncFlags.Set(PICFF_MINFARVERSION);
  if(Opt.ExceptRules)
  {
    TRY {
      FVer=(DWORD)CurPlugin.pMinFarVersion();
    }
    EXCEPT ( xfilter(EXCEPT_MINFARVERSION,
                     GetExceptionInformation(),&CurPlugin,0) )
    {
      CurPlugin.FuncFlags.Clear(PICFF_MINFARVERSION);
      UnloadPlugin(CurPlugin,EXCEPT_MINFARVERSION); // ���� �� �������, ��������� ���
      ProcessException=FALSE;
      return (FALSE);
    }
  }
  else
    FVer=(DWORD)CurPlugin.pMinFarVersion();
  CurPlugin.FuncFlags.Clear(PICFF_MINFARVERSION);
  //CurPluginItem=NULL;

  if (LOWORD(FVer) >  LOWORD(FAR_VERSION) ||
     (LOWORD(FVer) == LOWORD(FAR_VERSION) &&
      HIWORD(FVer) >  HIWORD(FAR_VERSION)))
  {
    // ������ - ������ ������� ������� ���
    ShowMessageAboutIllegalPluginVersion(CurPlugin.ModuleName,FVer);
    return (FALSE);
  }
  return (TRUE); // ���������, ���� ������
}

// �������� �������
// ������ ��� ������ ��� ����������
void PluginsSet::UnloadPlugin(struct PluginItem &CurPlugin,DWORD Exception)
{
//_SVS(SysLog("UnloadPlugin(%s)",CurPlugin.ModuleName));
//    if(FrameManager->GetBottomFrame() != FrameManager->GetCurrentFrame())
//      FrameManager->DeleteFrame();

  CurPluginItem=NULL;
  Frame *frame;
  if((frame=FrameManager->GetBottomFrame()) != NULL)
    frame->Unlock();

  if(Flags.Check(PSIF_DIALOG)) // BugZ#52 exception handling for floating point incorrect
  {
    Flags.Clear(PSIF_DIALOG);
    FrameManager->DeleteFrame();
  }

  BOOL Ret=FreeLibrary(CurPlugin.hModule);
  CurPlugin.FuncFlags.Clear(PICFF_LOADED); //??

  // ��� ��������� �����������!!! :-(
  char ModuleName[NM];
  xstrncpy(ModuleName,CurPlugin.ModuleName,sizeof(ModuleName)-1);

  BOOL NeedUpdatePanels=CurPlugin.FuncFlags.Check(PICFF_PANELPLUGIN);

  memset(&CurPlugin,0,sizeof(CurPlugin));
  xstrncpy(CurPlugin.ModuleName,ModuleName,sizeof(CurPlugin.ModuleName)-1);
  CurPlugin.WorkFlags.Set(PIWF_DONTLOADAGAIN);

  // BugZ#137 - ��������� ������� ���������� �������
  // ����� ����� ��������� �� "�����������" �������,
  if(NeedUpdatePanels)
  {
    CtrlObject->Cp()->ActivePanel->SetCurDir(".",TRUE);

//    FrameManager->RefreshFrame();

    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;

    ActivePanel->Update(UPDATE_KEEP_SELECTION);
    ActivePanel->Redraw();

    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(ActivePanel);
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    AnotherPanel->Redraw();
  }
}

void PluginsSet::ShowMessageAboutIllegalPluginVersion(char* plg,int required)
{
    char msg[2][512];
    char PlgName[NM];
    xstrncpy(PlgName,plg,sizeof(PlgName)-1);
    TruncPathStr(PlgName,ScrX-20);
    sprintf(msg[0],MSG(MPlgRequired),
           HIBYTE(LOWORD(required)),LOBYTE(LOWORD(required)),HIWORD(required));
    sprintf(msg[1],MSG(MPlgRequired2),
           HIBYTE(LOWORD(FAR_VERSION)),LOBYTE(LOWORD(FAR_VERSION)),HIWORD(FAR_VERSION));
    Message(MSG_WARNING,1,MSG(MError),MSG(MPlgBadVers),PlgName,msg[0],msg[1],MSG(MOk));
}
/* tran 03.08.2000 $ */
/* SVS $ */

void PluginsSet::CreatePluginStartupInfo(struct PluginStartupInfo *PSI,
                                         struct FarStandardFunctions *FSF,
                                         const char *ModuleName,
                                         int ModuleNumber)
{
  static struct PluginStartupInfo StartupInfo={0};
  static struct FarStandardFunctions StandardFunctions={0};
  /* $ 06.07.2000 IS
    ���������� ��������� ���� FarStandardFunctions (��. plugin.hpp)
    ������������� �� ������:
       StructSize, Unquote, ExpandEnvironmentStr,
       sprintf, sscanf, qsort, memcpy, memmove, memcmp, strchr,
       strrchr, strstr, strtok, memset, strpbrk
   $ 07.07.2000 IS
     ������ ������������... �������������: atoi, _atoi64, itoa,
     RemoveLeadingSpaces, RemoveTrailingSpaces, RemoveExternalSpaces,
     TruncStr, TruncPathStr, QuoteSpaceOnly, PointToName, GetPathRoot,
     AddEndSlash
  */
  // ��������� ��������� StandardFunctions ���� ���!!!
  if(!StandardFunctions.StructSize)
  {
    StandardFunctions.StructSize=sizeof(StandardFunctions);
    StandardFunctions.sprintf=FarSprintf;
#ifndef FAR_MSVCRT
    StandardFunctions.sscanf=FarSscanf;
#else
    StandardFunctions.sscanf=sscanf;
#endif
    StandardFunctions.qsort=FarQsort;
    /* $ 24.03.2001 tran
      + qsortex */
    StandardFunctions.qsortex=FarQsortEx;
    StandardFunctions.atoi=FarAtoi;
    StandardFunctions.atoi64=FarAtoi64;
    StandardFunctions.itoa=FarItoa;
    StandardFunctions.itoa64=FarItoa64;

    //StandardFunctions.far_qsort=FarQsort;
    // ??? ������ ������?
    /* tran $ */

    StandardFunctions.bsearch=FarBsearch;

    StandardFunctions.SetFileApisTo=SetFileApisTo;
    /* $ 28.08.2000 SVS
       + ������� ������ �...
    */
    StandardFunctions.LIsLower   =LocalIslower;
    StandardFunctions.LIsUpper   =LocalIsupper;
    StandardFunctions.LIsAlpha   =LocalIsalpha;
    StandardFunctions.LIsAlphanum=LocalIsalphanum;
    StandardFunctions.LUpper     =LocalUpper;
    StandardFunctions.LUpperBuf  =LocalUpperBuf;
    StandardFunctions.LLowerBuf  =LocalLowerBuf;
    StandardFunctions.LLower     =LocalLower;
    StandardFunctions.LStrupr    =LocalStrupr;
    StandardFunctions.LStrlwr    =LocalStrlwr;
    StandardFunctions.LStricmp   =LStricmp;
    StandardFunctions.LStrnicmp  =LStrnicmp;
    /* SVS $ */

    StandardFunctions.Unquote=Unquote;
    StandardFunctions.ExpandEnvironmentStr=ExpandEnvironmentStr;
    StandardFunctions.LTrim=RemoveLeadingSpaces;
    StandardFunctions.RTrim=RemoveTrailingSpaces;
    StandardFunctions.Trim=RemoveExternalSpaces;
    StandardFunctions.TruncStr=TruncStr;
    StandardFunctions.TruncPathStr=TruncPathStr;
    StandardFunctions.QuoteSpaceOnly=QuoteSpaceOnly;
    StandardFunctions.PointToName=PointToName;
    StandardFunctions.GetPathRoot=GetPathRoot;
    StandardFunctions.AddEndSlash=AddEndSlash;
    /* IS $ */
    /* $ 25.07.2000 SVS
       ��� ������� ���������� ������ :-)
    */
    StandardFunctions.CopyToClipboard=CopyToClipboard;
    StandardFunctions.PasteFromClipboard=PasteFromClipboard;
    StandardFunctions.FarKeyToName=KeyToText;
    /* SVS $ */
    /* $ 24.09.2000 SVS
     + ������� FarNameToKey - ��������� ���� ������� �� �����
       ���� ��� �� ����� ��� ��� ������ - ������������ -1
    */
    StandardFunctions.FarNameToKey=KeyNameToKey;
    /* SVS $ */
    /* $ 31.08.2000 tran
       + InputRecordToKey*/
    StandardFunctions.FarInputRecordToKey=InputRecordToKey;
    /* tran 31.08.2000 $ */
    /* $ 05.09.2000 SVS 1.17
       + QWERTY - ��������������
    */
    StandardFunctions.XLat=Xlat;
    /* SVS $ */
    /* $ 07.09.2000 SVS 1.17
       + ������� GetFileOwner ���� �������� �������� :-)
       + ������� GetNumberOfLinks ���� �������� �������� :-)
    */
    StandardFunctions.GetFileOwner=GetFileOwner;
    StandardFunctions.GetNumberOfLinks=GetNumberOfLinks;
    /* SVS $ */
    /* $ 10.09.2000 tran
      + ������������� */
    StandardFunctions.FarRecursiveSearch=FarRecursiveSearch;
    /* tran 08.09.2000 $ */
    /* $ 14.09.2000 SVS
      ������� ��������� ���������� ����� � ������ �����.
    */
    StandardFunctions.MkTemp=FarMkTemp;
    /* SVS $ */
    /*$ 27.09.2000 skv
      + Delete buffer allocated in PasteFromClipboard
    */
    StandardFunctions.DeleteBuffer=DeleteBuffer;
    /* skv$*/
    /* $ 12.10.2000 IS
      + ProcessName - ���������� ��� �����: �������� � ������, �������,
        ������������� �� �����
    */
    StandardFunctions.ProcessName=ProcessName;
    /* IS $ */
    StandardFunctions.MkLink=FarMkLink;
    StandardFunctions.ConvertNameToReal=ConvertNameToReal;
    StandardFunctions.GetReparsePointInfo=FarGetReparsePointInfo;
  }

  if(!StartupInfo.StructSize)
  {
    StartupInfo.StructSize=sizeof(StartupInfo);
    StartupInfo.Menu=FarMenuFn;
    StartupInfo.Dialog=FarDialogFn;
    StartupInfo.GetMsg=FarGetMsgFn;
    StartupInfo.Message=FarMessageFn;
    StartupInfo.Control=FarControl;
    StartupInfo.SaveScreen=FarSaveScreen;
    StartupInfo.RestoreScreen=FarRestoreScreen;
    StartupInfo.GetDirList=FarGetDirList;
    StartupInfo.GetPluginDirList=FarGetPluginDirList;
    StartupInfo.FreeDirList=FarFreeDirList;
    StartupInfo.Viewer=FarViewer;
    StartupInfo.Editor=FarEditor;
    StartupInfo.CmpName=FarCmpName;
    StartupInfo.CharTable=FarCharTable;
    StartupInfo.Text=FarText;
    StartupInfo.EditorControl=FarEditorControl;
    StartupInfo.ViewerControl=FarViewerControl;
    /* 01.07.2000 IS
       ������� ������ ������
    */
    StartupInfo.ShowHelp=FarShowHelp;
    /* IS $ */
    /* 05.07.2000 IS
       �������, ������� ����� ����������� � � ���������, � � �������, �...
    */
    StartupInfo.AdvControl=FarAdvControl;
    /* IS $ */
    /* $ 23.07.2000 SVS
       ������� ��� ����������� �������
         - ����������� ������� �������
         - ����� �����������
         - ������� �� ���������
    */
    StartupInfo.DialogEx=FarDialogEx;
    StartupInfo.SendDlgMessage=FarSendDlgMessage;
    StartupInfo.DefDlgProc=FarDefDlgProc;
    /* $ 25.07.2000 SVS
       �������-����������� ������ ����� ������
    */
    StartupInfo.InputBox=FarInputBox;
    /* SVS $ */
  }

  memcpy(PSI,&StartupInfo,sizeof(StartupInfo));
  memcpy(FSF,&StandardFunctions,sizeof(StandardFunctions));
  PSI->ModuleNumber=ModuleNumber;
  PSI->FSF=FSF;
  xstrncpy(PSI->ModuleName,ModuleName,sizeof(PSI->ModuleName)-1);
  PSI->RootKey=NULL;
}

/* $ 22.05.2001 DJ
   �������� ���. ������ ���������� � SetPluginStartupInfo()
*/
int PluginsSet::SetPluginStartupInfo(struct PluginItem &CurPlugin,int ModuleNumber)
{
  _ALGO(CleverSysLog clv("PluginsSet::SetPluginStartupInfo()"));
  _ALGO(SysLog("ModuleNumber=%d",ModuleNumber));
  //PrepareModulePath(CurPlugin.ModuleName);

  /* $ 03.08.2000 tran
     �������� �� ����������� ������ ���� */
  if (!CheckMinVersion(CurPlugin))
  {
    UnloadPlugin(CurPlugin,-1); // ���� �� �������, ��������� ���
    return (FALSE);
  }

  if (CurPlugin.pSetStartupInfo!=NULL && !ProcessException)
  {
    // ��� ���� ��������� ����� ����������� ��������, ��� �� ��������
    // ����-���� ���� �� ������ ������.
    struct PluginStartupInfo LocalStartupInfo;
    struct FarStandardFunctions LocalStandardFunctions;

    CreatePluginStartupInfo(&LocalStartupInfo,&LocalStandardFunctions,CurPlugin.ModuleName,ModuleNumber);

    // ������������ ������ � �������-��������� ����
    xstrncpy(CurPlugin.RootKey,Opt.RegRoot,sizeof(CurPlugin.RootKey)-1);
    strncat(CurPlugin.RootKey,"\\Plugins",sizeof(CurPlugin.RootKey)-1);
    LocalStartupInfo.RootKey=CurPlugin.RootKey;

    //CurPluginItem=&CurPlugin;
    CurPlugin.FuncFlags.Set(PICFF_SETSTARTUPINFO);
    if(Opt.ExceptRules)
    {
      TRY {
        CurPlugin.pSetStartupInfo(&LocalStartupInfo);
      }
      EXCEPT(xfilter(EXCEPT_SETSTARTUPINFO,GetExceptionInformation(),&CurPlugin,0)){
        CurPlugin.FuncFlags.Clear(PICFF_SETSTARTUPINFO);
        UnloadPlugin(CurPlugin,EXCEPT_SETSTARTUPINFO); // ���� �� �������, ��������� ���
        ProcessException=FALSE;
        return FALSE;
      }
    }
    else
      CurPlugin.pSetStartupInfo(&LocalStartupInfo);
    //CurPluginItem=NULL;
    CurPlugin.FuncFlags.Clear(PICFF_SETSTARTUPINFO);
  }
  return TRUE;
}
/* DJ $ */


int PluginsSet::PreparePlugin(int PluginNumber)
{
  struct PluginItem *PData=PluginsData+PluginNumber;
  if (!PData->WorkFlags.Check(PIWF_CACHED))
    return(TRUE);
  return(LoadPlugin(*PData,PluginNumber,TRUE));
}


int PluginsSet::GetCacheNumber(char *FullName,WIN32_FIND_DATA *FindData,int CachePos)
{
  for (int I=-1;;I++)
  {
    if (I==-1 && CachePos==0)
      continue;
    int Pos=(I==-1) ? CachePos:I;

    char RegKey[100],PluginName[NM],PluginID[100],CurPluginID[100];
    sprintf(RegKey,FmtPluginsCache_PluginD,Pos);
    GetRegKey(RegKey,"Name",PluginName,"",sizeof(PluginName));
    if (*PluginName==0)
      break;
    if (LocalStricmp(PluginName,FullName)!=0)
      continue;
    if (FindData!=NULL)
    {
      GetRegKey(RegKey,"ID",PluginID,"",sizeof(PluginID));
      sprintf(CurPluginID,"%x%x%x",FindData->nFileSizeLow,
              FindData->ftCreationTime.dwLowDateTime,
              FindData->ftLastWriteTime.dwLowDateTime);
      if (strcmp(PluginID,CurPluginID)!=0)
        continue;
    }
    return(Pos);
  }
  return(-1);
}


int PluginsSet::SavePluginSettings(struct PluginItem &CurPlugin,
                                    WIN32_FIND_DATA &FindData)
{
  if(!(CurPlugin.pGetPluginInfo     ||
      CurPlugin.pOpenPlugin         ||
      CurPlugin.pOpenFilePlugin     ||
      CurPlugin.pSetFindList        ||
      CurPlugin.pProcessEditorInput ||
      CurPlugin.pProcessEditorEvent ||
      CurPlugin.pProcessViewerEvent
  // ���� ��������� �� �������, ��-�� ������� ������ ����� ����� ���� � ����
  ))
   return FALSE;

  struct PluginInfo Info;
  memset(&Info,0,sizeof(Info));

  if(CurPlugin.pGetPluginInfo && !ProcessException)
  {
    //CurPluginItem=&CurPlugin;
    CurPlugin.FuncFlags.Set(PICFF_GETPLUGININFO);
    if(Opt.ExceptRules)
    {
      TRY {
        CurPlugin.pGetPluginInfo(&Info);
      }
      EXCEPT(xfilter(EXCEPT_GETPLUGININFO,GetExceptionInformation(),&CurPlugin,0)){
        CurPlugin.FuncFlags.Clear(PICFF_GETPLUGININFO);
        UnloadPlugin(CurPlugin,EXCEPT_GETPLUGININFO); // ���� �� �������, ��������� ���
        ProcessException=FALSE;
        return FALSE;
      }
      if(!TestPluginInfo(CurPlugin,&Info))
      {
        CurPlugin.FuncFlags.Clear(PICFF_GETPLUGININFO);
        return FALSE;
      }
    }
    else
      CurPlugin.pGetPluginInfo(&Info);
    //CurPluginItem=NULL;
    CurPlugin.FuncFlags.Clear(PICFF_GETPLUGININFO);
  }

  CurPlugin.SysID=Info.SysID;
  /* $ 12.10.2000 tran
     ��� PF_PRELOAD � ��� ����� ��������, ��� ������ �� ����������
     ��� ����� �������� -co */
//  if (Info.Flags & PF_PRELOAD)
//    return(FALSE);
  /* tran $ */

  /* $ 13.07.2000 IS
    ��������� ����, ���������� ��� ������������ �������� ��� VC:
    ���������� I ���������� �� ���� ������ (������������ ��� ����,
    ���� ���� $%#...)
    (��������� tran)
  */
  int I,I0;
  for (I0=0;;I0++)
  {
    char RegKey[100],PluginName[NM],CurPluginID[100];
    sprintf(RegKey,FmtPluginsCache_PluginD,I0);
    GetRegKey(RegKey,"Name",PluginName,"",sizeof(PluginName));
    if (*PluginName==0 || LocalStricmp(PluginName,CurPlugin.ModuleName)==0)
    {
      DeleteKeyTree(RegKey);

      SetRegKey(RegKey,"Name",CurPlugin.ModuleName);
      sprintf(CurPluginID,"%x%x%x",FindData.nFileSizeLow,
              FindData.ftCreationTime.dwLowDateTime,
              FindData.ftLastWriteTime.dwLowDateTime);
      SetRegKey(RegKey,"ID",CurPluginID);
      /* $ 12.10.2000 tran
         ���� ������ PRELOAD, � ��� ������� �� ���� */
      if (Info.Flags & PF_PRELOAD)
      {
        SetRegKey(RegKey,NFMP_Preload,1);
        CurPlugin.WorkFlags.Set(PIWF_PRELOADED);
        break;
      }
      else
      {
        SetRegKey(RegKey,NFMP_Preload,(DWORD)0);
        CurPlugin.WorkFlags.Clear(PIWF_PRELOADED);
      }
      /* tran $ */

      for (I=0;I<Info.DiskMenuStringsNumber;I++)
      {
        char Value[100];
        sprintf(Value,FmtDiskMenuStringD,I);
        SetRegKey(RegKey,Value,Info.DiskMenuStrings[I]);
        if (Info.DiskMenuNumbers)
        {
          sprintf(Value,FmtDiskMenuNumberD,I);
          SetRegKey(RegKey,Value,Info.DiskMenuNumbers[I]);
        }
      }
      for (I=0;I<Info.PluginMenuStringsNumber;I++)
      {
        char Value[100];
        sprintf(Value,FmtPluginMenuStringD,I);
        SetRegKey(RegKey,Value,Info.PluginMenuStrings[I]);
      }
      for (I=0;I<Info.PluginConfigStringsNumber;I++)
      {
        char Value[100];
        sprintf(Value,FmtPluginConfigStringD,I);
        SetRegKey(RegKey,Value,Info.PluginConfigStrings[I]);
      }
      SetRegKey(RegKey,"CommandPrefix",NullToEmpty(Info.CommandPrefix));
      SetRegKey(RegKey,"Flags",Info.Flags);
      sprintf(RegKey,FmtPluginsCache_PluginDExport,I0);
      SetRegKey(RegKey,NFMP_SysID,CurPlugin.SysID);
      SetRegKey(RegKey,NFMP_OpenPlugin,CurPlugin.pOpenPlugin!=NULL);
      SetRegKey(RegKey,NFMP_OpenFilePlugin,CurPlugin.pOpenFilePlugin!=NULL);
      SetRegKey(RegKey,NFMP_SetFindList,CurPlugin.pSetFindList!=NULL);
      SetRegKey(RegKey,NFMP_ProcessEditorInput,CurPlugin.pProcessEditorInput!=NULL);
      SetRegKey(RegKey,NFMP_ProcessEditorEvent,CurPlugin.pProcessEditorEvent!=NULL);
      SetRegKey(RegKey,NFMP_ProcessViewerEvent,CurPlugin.pProcessViewerEvent!=NULL);
      break;
    }
  }
  /* IS $ */
  return(TRUE);
}


HANDLE PluginsSet::OpenPlugin(int PluginNumber,int OpenFrom,int Item)
{
  _ALGO(CleverSysLog clv("PluginsSet::OpenPlugin()"));
  _ALGO(SysLog("PluginNumber=%d, OpenFrom=%d, Item=%d",PluginNumber,OpenFrom,Item));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  CheckScreenLock();
  char CurDir[NM];
  CtrlObject->CmdLine->GetCurDir(CurDir);
  FarChDir(CurDir);
  *DirToSet=0;
  HANDLE hInternal=0;
  if (PluginNumber<PluginsCount)
  {
    struct PluginItem *PData=PluginsData+PluginNumber;
    if (PData->pOpenPlugin && PreparePlugin(PluginNumber) && !ProcessException)
    {
      BOOL IsUnload=FALSE;
      {
        _KEYMACRO(CleverSysLog clv("PluginsSet::OpenPlugin()"));
        _KEYMACRO(SysLog("**** Enter to Plugin **** (%s)",PData->ModuleName));
        /* $ 16.10.2000 SVS
           + ��������� ���������� ��� ������ �������� �������.
        */
        CurPluginItem=PData;
        PData->FuncFlags.Set(PICFF_OPENPLUGIN);
        CtrlObject->Macro.SetRedrawEditor(FALSE);
        Flags.Set(PSIF_ENTERTOOPENPLUGIN);
        if(Opt.ExceptRules)
        {
          _ECTLLOG(CleverSysLog SL(NFMP_OpenPlugin));
          TRY {
             _SVS(SysLog("OPENPLUGIN >>> '%s'",(char *)Item));
             hInternal=PData->pOpenPlugin(OpenFrom,Item);
             _SVS(SysLog("OPENPLUGIN <<< '%s'",(char *)Item));
             /* $ 26.02.2201 VVM
                 ! ��������� ������, ���� ������� NULL */
             if (!hInternal)
               RaiseException(STATUS_INVALIDFUNCTIONRESULT, 0, 0, 0);
             /* VVM $ */
             if(CtrlObject->Plugins.Flags.Check(PSIF_DIALOG))
               RaiseException(EXCEPT_FARDIALOG, 0, 0, 0);
          }
          EXCEPT(xfilter(EXCEPT_OPENPLUGIN,GetExceptionInformation(),PData,1)){
            IsUnload=TRUE;
          }
        }
        else
        {
          hInternal=PData->pOpenPlugin(OpenFrom,Item);
          // ��� ����������� ��������� �� ��������� ������.
        }
        PData->FuncFlags.Clear(PICFF_OPENPLUGIN);
        CurPluginItem=NULL;
        Flags.Clear(PSIF_ENTERTOOPENPLUGIN);
        CtrlObject->Macro.SetRedrawEditor(TRUE);
        _KEYMACRO(SysLog("**** Leave from Plugin **** (%s)",PData->ModuleName));
      }
      /* SVS $ */
      /*$ 10.08.2000 skv
        If we are in editor mode, and CurEditor defined,
        we need to call this events.
        EE_REDRAW 2 - to notify that text changed.
        EE_REDRAW 0 - to notify that whole screen updated
        ->Show() to actually update screen.

        This duplication take place since ShowEditor method
        will NOT send this event while screen is locked.
      */
      if(OpenFrom == OPEN_EDITOR &&
         !CtrlObject->Macro.IsExecuting() &&
         CtrlObject->Plugins.CurEditor &&
         CtrlObject->Plugins.CurEditor->IsVisible() && !IsUnload)
      {
//_SVS(SysLog("**** Enter to EE_REDRAW (return from Plugin)"));
        CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE);
        CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL);
        CtrlObject->Plugins.CurEditor->Show();
      }
      /* skv$*/
      if(IsUnload)
      {
        UnloadPlugin(*PData,EXCEPT_OPENPLUGIN);
        ProcessException=FALSE;
        return(INVALID_HANDLE_VALUE);
      }

      if (hInternal!=INVALID_HANDLE_VALUE)
      {
        PluginHandle *hPlugin=new PluginHandle;
        hPlugin->InternalHandle=hInternal;
        hPlugin->PluginNumber=PluginNumber;
        return((HANDLE)hPlugin);
      }
      else
        if (*DirToSet)
        {
          CtrlObject->Cp()->ActivePanel->SetCurDir(DirToSet,TRUE);
          CtrlObject->Cp()->ActivePanel->Redraw();
        }
    }
  }
  return(INVALID_HANDLE_VALUE);
}


HANDLE PluginsSet::OpenFilePlugin(char *Name,const unsigned char *Data,int DataSize)
{
  _ALGO(CleverSysLog clv("PluginsSet::OpenFilePlugin()"));
  _ALGO(SysLog("Name='%s', Data=%p, DataSize=%d",(Name?Name:"(NULL)"),Data,DataSize));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  /* $ 20.03.2001 tran */
  ConsoleTitle ct(Opt.ShowCheckingFile?MSG(MCheckingFileInPlugin):NULL);
  /* tran $ */

  struct PluginItem *PData=PluginsData;
  for (int I=0;I<PluginsCount;I++,PData++)
    if (PData->pOpenFilePlugin && PreparePlugin(I) && !ProcessException)
    {
      char FullName[NM],*NamePtr=NULL;
      if (Name!=NULL)
      {
        ConvertNameToFull(Name,FullName, sizeof(FullName));
        NamePtr=FullName;
      }
      /* $ 16.10.2000 SVS
         + ��������� ���������� ��� ������ �������� �������.
      */
      HANDLE hInternal;
      /* $ 20.03.2001 tran */
      if(Opt.ShowCheckingFile)
        ct.Set("%s - [%s]...",MSG(MCheckingFileInPlugin),strrchr(PluginsData[I].ModuleName,'\\')+1);
      /* tran $ */
      PData->FuncFlags.Set(PICFF_OPENFILEPLUGIN);
      if(Opt.ExceptRules)
      {
        TRY
        {
           _ALGO(SysLog("call [%d]OpenFilePlugin(), ModuleName='%s'",I,PluginsData[I].ModuleName));
           hInternal=PData->pOpenFilePlugin(NamePtr,Data,DataSize);
           if (!hInternal)
             RaiseException(STATUS_INVALIDFUNCTIONRESULT, 0, 0, 0);
           //????????????????????????????????????????????????????
        }
        EXCEPT(xfilter(EXCEPT_OPENFILEPLUGIN,GetExceptionInformation(),PData,1)){
          UnloadPlugin(*PData,EXCEPT_OPENFILEPLUGIN);
          hInternal=INVALID_HANDLE_VALUE;
          ProcessException=FALSE;
        }
      }
      else
      {
        hInternal=PData->pOpenFilePlugin(NamePtr,Data,DataSize);
        // ��� ����������� ��������� �� ��������� ������.
      }
      PData->FuncFlags.Clear(PICFF_OPENFILEPLUGIN);

      if (hInternal==(HANDLE)-2)
        return((HANDLE)-2);
      if (hInternal!=INVALID_HANDLE_VALUE)
      {
        PluginHandle *hPlugin=new PluginHandle;
        hPlugin->InternalHandle=hInternal;
        hPlugin->PluginNumber=I;
        return((HANDLE)hPlugin);
      }
    }
  return(INVALID_HANDLE_VALUE);
}


HANDLE PluginsSet::OpenFindListPlugin(const PluginPanelItem *PanelItem,int ItemsNumber)
{
  _ALGO(CleverSysLog clv("PluginsSet::OpenFindListPlugin()"));
  _ALGO(SysLog("PanelItem=%p, ItemsNumber=%d",PanelItem,ItemsNumber));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  struct PluginItem *PData=PluginsData;
  for (int I=0;I<PluginsCount;I++,PData++)
    if (PData->pOpenPlugin && PData->pSetFindList && PreparePlugin(I) && !ProcessException)
    {
      /* $ 26.02.2001 VVM
          + ��������� ���������� ��� OpenPlugin(OPEN_FINDLIST) */
      HANDLE hInternal;
      PData->FuncFlags.Set(PICFF_OPENPLUGIN);
      if(Opt.ExceptRules)
      {
        TRY {
           hInternal = PData->pOpenPlugin(OPEN_FINDLIST,0);
           /* $ 26.02.2201 VVM
               ! ��������� ������, ���� ������� NULL */
           if (!hInternal)
             RaiseException(STATUS_INVALIDFUNCTIONRESULT, 0, 0, 0);
           //????????????????????????????????????????????????????
           /* VVM $ */
        }
        EXCEPT(xfilter(EXCEPT_OPENPLUGIN,GetExceptionInformation(),PData,1)){
          UnloadPlugin(*PData,EXCEPT_OPENPLUGIN);
          hInternal=INVALID_HANDLE_VALUE;
          ProcessException=FALSE;
        }
        /* VVM $ */
      }
      else
      {
        hInternal = PData->pOpenPlugin(OPEN_FINDLIST,0);
        // ��� ����������� ��������� �� ��������� ������.
      }
      PData->FuncFlags.Clear(PICFF_OPENPLUGIN);

      if (hInternal!=INVALID_HANDLE_VALUE)
      {
        //EXCEPTION_POINTERS *xp;
        BOOL Ret;

        PData->FuncFlags.Set(PICFF_SETFINDLIST);
        if(Opt.ExceptRules)
        {
          TRY {
            Ret=PData->pSetFindList(hInternal,PanelItem,ItemsNumber);
          }
          EXCEPT(xfilter(EXCEPT_SETFINDLIST,GetExceptionInformation(),PData,1)){
             UnloadPlugin(*PData,EXCEPT_SETFINDLIST);
             Ret=FALSE;
             ProcessException=FALSE; //???
          }
        }
        else
          Ret=PData->pSetFindList(hInternal,PanelItem,ItemsNumber);
        PData->FuncFlags.Clear(PICFF_SETFINDLIST);

        if (!Ret)
          continue;

        PluginHandle *hPlugin=new PluginHandle;
        hPlugin->InternalHandle=hInternal;
        hPlugin->PluginNumber=I;
        return((HANDLE)hPlugin);
      }
    }
  return(INVALID_HANDLE_VALUE);
}


void PluginsSet::ClosePlugin(HANDLE hPlugin)
{
  _ALGO(CleverSysLog clv("PluginsSet::ClosePlugin()"));
  _ALGO(SysLog("hPlugin=0x%08X",hPlugin));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pClosePlugin && !ProcessException)
  {
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_CLOSEPLUGIN);
    if(Opt.ExceptRules)
    {
      TRY {
        PData->pClosePlugin(ph->InternalHandle);
      }
      EXCEPT(xfilter(EXCEPT_CLOSEPLUGIN,GetExceptionInformation(),PData,1))
      {
        UnloadPlugin(*PData,EXCEPT_CLOSEPLUGIN);
        ProcessException=FALSE;
      }
    }
    else
      PData->pClosePlugin(ph->InternalHandle);
    PData->FuncFlags.Clear(PICFF_CLOSEPLUGIN);
    //CurPluginItem=NULL;
  }
  delete ph;
}


int PluginsSet::ProcessEditorInput(INPUT_RECORD *Rec)
{
  _ALGO(CleverSysLog clv("PluginsSet::ProcessEditorInput()"));
  _ALGO(SysLog("Rec=%s",_INPUT_RECORD_Dump(Rec)));
  //EXCEPTION_POINTERS *xp;
  struct PluginItem *PData=PluginsData;
  _KEYMACRO(CleverSysLog SL("PluginsSet::ProcessEditorInput()"));
  for (int I=0;I<PluginsCount;I++,PData++)
  {
    if (PData->pProcessEditorInput && PreparePlugin(I) && !ProcessException)
    {
      /* $ 13.07.2000 IS
         ������ ���� ��� ����� � �������� (��������� tran)
      */
      if (PData->pProcessEditorInput)
      {
        int Ret;
        _KEYMACRO(SysLog("CALL pProcessEditorInput(): '%s'",PData->ModuleName));

        PData->FuncFlags.Set(PICFF_PROCESSEDITORINPUT);
        if(Opt.ExceptRules)
        {
          TRY {
            Ret=PData->pProcessEditorInput(Rec);
          }
          EXCEPT(xfilter(EXCEPT_PROCESSEDITORINPUT,GetExceptionInformation(),PData,1))
          {
            UnloadPlugin(*PData,EXCEPT_PROCESSEDITORINPUT); //????!!!!
            Ret=FALSE;
            ProcessException=FALSE; //??
          }
        }
        else
          Ret=PData->pProcessEditorInput(Rec);
        PData->FuncFlags.Clear(PICFF_PROCESSEDITORINPUT);

        _KEYMACRO(SysLog("Ret=%d",Ret));
        if(Ret)
          return(TRUE);
        /* IS $ */
      }
    }
  }
  return(FALSE);
}


int PluginsSet::ProcessEditorEvent(int Event,void *Param)
{
  _ALGO(CleverSysLog clv("PluginsSet::ProcessEditorEvent()"));
  _ALGO(SysLog("Params: Event=%s, Param=%s",_EE_ToName(Event),_EEREDRAW_ToName((int)Param)));
  _ECTLLOG(CleverSysLog Clev("PluginsSet::ProcessEditorEvent()"));
  int Ret=0;
  //EXCEPTION_POINTERS *xp;
  if(CtrlObject->Plugins.CurEditor)
  {
    struct PluginItem *PData=PluginsData;
    for (int I=0;I<PluginsCount;I++,PData++)
    {
      if (PData->pProcessEditorEvent && PreparePlugin(I) && !ProcessException)
      {
        //_ECTLLOG(CleverSysLog Clev2("Call ProcessEditorEvent()"));
        _ECTLLOG(SysLog("Plugin: '%s'",PData->ModuleName));
        _ECTLLOG(SysLog("Params: Event=%s, Param=%s",_EE_ToName(Event),_EEREDRAW_ToName((int)Param)));
        PData->FuncFlags.Set(PICFF_PROCESSEDITOREVENT);
        if(Opt.ExceptRules)
        {
          TRY {
            Ret=PData->pProcessEditorEvent(Event,Param);
          }
          EXCEPT(xfilter(EXCEPT_PROCESSEDITOREVENT,GetExceptionInformation(),PData,1))
          {
            UnloadPlugin(*PData,EXCEPT_PROCESSEDITOREVENT);
            ProcessException=FALSE;
          }
        }
        else
          Ret=PData->pProcessEditorEvent(Event,Param);
        _ECTLLOG(SysLog("Return=%d",Ret));
        PData->FuncFlags.Clear(PICFF_PROCESSEDITOREVENT);
      }
    }
  }
  return Ret;
}


/* $ 27.09.2000 SVS
   ������� �� �������
*/
int PluginsSet::ProcessViewerEvent(int Event,void *Param)
{
  _ALGO(CleverSysLog clv("PluginsSet::ProcessViewerEvent()"));
  //EXCEPTION_POINTERS *xp;
  struct PluginItem *PData=PluginsData;
  int Ret=0;
  for (int I=0;I<PluginsCount;I++,PData++)
  {
    if (PData->pProcessViewerEvent && PreparePlugin(I) && !ProcessException)
    {
      PData->FuncFlags.Set(PICFF_PROCESSVIEWEREVENT);
      if(Opt.ExceptRules)
      {
        TRY {
          Ret=PData->pProcessViewerEvent(Event,Param);
        }
        EXCEPT(xfilter(EXCEPT_PROCESSVIEWEREVENT,GetExceptionInformation(),PData,1))
        {
          UnloadPlugin(*PData,EXCEPT_PROCESSVIEWEREVENT);
          ProcessException=FALSE;
        }
      }
      else
        Ret=PData->pProcessViewerEvent(Event,Param);
      PData->FuncFlags.Clear(PICFF_PROCESSVIEWEREVENT);
    }
  }
  return Ret;
}
/* SVS $ */

int PluginsSet::GetFindData(HANDLE hPlugin,PluginPanelItem **pPanelData,int *pItemsNumber,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::GetFindData()"));
  _ALGO(SysLog("hPlugin=%p, pPanelData=%p, pItemsNumber=%p, OpMode=%d",hPlugin,pPanelData,pItemsNumber,OpMode));
  //EXCEPTION_POINTERS *xp;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  *pItemsNumber=0;
  if (PData->pGetFindData && !ProcessException)
  {
    int Ret;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_GETFINDDATA);
    if(Opt.ExceptRules)
    {
      TRY {
        Ret=PData->pGetFindData(ph->InternalHandle,pPanelData,pItemsNumber,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_GETFINDDATA,GetExceptionInformation(),PData,1))
      {
        UnloadPlugin(*PData,EXCEPT_GETFINDDATA); //????!!!!
        Ret=FALSE;
        ProcessException=FALSE; //??
      }
    }
    else
      Ret=PData->pGetFindData(ph->InternalHandle,pPanelData,pItemsNumber,OpMode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_GETFINDDATA);
    _ALGO(SysLog("Ret=%d",Ret));
    return(Ret);
  }
  return(FALSE);
}


void PluginsSet::FreeFindData(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber)
{
  _ALGO(CleverSysLog clv("PluginsSet::FreeFindData()"));
  _ALGO(SysLog("hPlugin=%p, PanelItem=%p, ItemsNumber=%d",hPlugin,PanelItem,ItemsNumber));
  //EXCEPTION_POINTERS *xp;
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pFreeFindData && !ProcessException)
  {
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_FREEFINDDATA);
    if(Opt.ExceptRules)
    {
      TRY {
        PData->pFreeFindData(ph->InternalHandle,PanelItem,ItemsNumber);
      }
      EXCEPT(xfilter(EXCEPT_FREEFINDDATA,GetExceptionInformation(),PData,1))
      {
        UnloadPlugin(*PData,EXCEPT_FREEFINDDATA); //????!!!!
        ProcessException=FALSE;
      }
    }
    else
      PData->pFreeFindData(ph->InternalHandle,PanelItem,ItemsNumber);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_FREEFINDDATA);
  }
}


int PluginsSet::GetVirtualFindData(HANDLE hPlugin,PluginPanelItem **pPanelData,int *pItemsNumber,const char *Path)
{
  _ALGO(CleverSysLog clv("PluginsSet::GetVirtualFindData()"));
  //_ALGO(SysLog("hPlugin=%p, PanelItem=%p, ItemsNumber=%d",hPlugin,PanelItem,ItemsNumber));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  *pItemsNumber=0;
  if (PData->pGetVirtualFindData && !ProcessException)
  {
    int Ret;
    PData->FuncFlags.Set(PICFF_GETVIRTUALFINDDATA);
    //CurPluginItem=PData;
    if(Opt.ExceptRules)
    {
      TRY{
        Ret=PData->pGetVirtualFindData(ph->InternalHandle,pPanelData,pItemsNumber,Path);
      }
      EXCEPT(xfilter(EXCEPT_GETVIRTUALFINDDATA,GetExceptionInformation(),PData,1))
      {
        UnloadPlugin(*PData,EXCEPT_GETVIRTUALFINDDATA); //????!!!!
        Ret=FALSE;
        ProcessException=FALSE;
      }
    }
    else
      Ret=PData->pGetVirtualFindData(ph->InternalHandle,pPanelData,pItemsNumber,Path);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_GETVIRTUALFINDDATA);
    return(Ret);
  }
  return(FALSE);
}


void PluginsSet::FreeVirtualFindData(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber)
{
  _ALGO(CleverSysLog clv("PluginsSet::FreeVirtualFindData()"));
  _ALGO(SysLog("hPlugin=%p, PanelItem=%p, ItemsNumber=%d",hPlugin,PanelItem,ItemsNumber));
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pFreeVirtualFindData && !ProcessException)
  {
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_FREEVIRTUALFINDDATA);
    if(Opt.ExceptRules)
    {
      TRY {
        PData->pFreeVirtualFindData(ph->InternalHandle,PanelItem,ItemsNumber);
      }
      EXCEPT(xfilter(EXCEPT_FREEVIRTUALFINDDATA,GetExceptionInformation(),PData,1))
      {
        UnloadPlugin(*PData,EXCEPT_FREEVIRTUALFINDDATA); //????!!!!
        ProcessException=FALSE;
      }
    }
    else
      PData->pFreeVirtualFindData(ph->InternalHandle,PanelItem,ItemsNumber);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_FREEVIRTUALFINDDATA);
  }
}


int PluginsSet::SetDirectory(HANDLE hPlugin,const char *Dir,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::SetDirectory()"));
  _ALGO(SysLog("hPlugin=%p, Dir='%s', OpMode=%d",hPlugin,(Dir?Dir:"(NULL)"),OpMode));
  //EXCEPTION_POINTERS *xp;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pSetDirectory && !ProcessException)
  {
    int Ret;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_SETDIRECTORY);
    if(Opt.ExceptRules)
    {
      TRY{
        Ret=PData->pSetDirectory(ph->InternalHandle,Dir,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_SETDIRECTORY,GetExceptionInformation(),PData,1))
      {
        UnloadPlugin(*PData,EXCEPT_SETDIRECTORY); //????!!!!
        Ret=FALSE;
        ProcessException=FALSE;
      }
    }
    else
      Ret=PData->pSetDirectory(ph->InternalHandle,Dir,OpMode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_SETDIRECTORY);
    return(Ret);
  }
  return(FALSE);
}


int PluginsSet::GetFile(HANDLE hPlugin,struct PluginPanelItem *PanelItem,
                        char *DestPath,char *ResultName,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::GetFile()"));
  _ALGO(SysLog("hPlugin=%p, PanelItem=%p, DestPath='%s', ResultName='%s', OpMode=%d",hPlugin,PanelItem,(DestPath?DestPath:"(NULL)"),(ResultName?ResultName:"(NULL)"),OpMode));
  //EXCEPTION_POINTERS *xp;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  SaveScreen *SaveScr=NULL;
  int Found=FALSE;
  KeepUserScreen=FALSE;
  if (PData->pGetFiles && !ProcessException)
  {
    if ((OpMode & OPM_FIND)==0)
      SaveScr=new SaveScreen;
    UndoGlobalSaveScrPtr UndSaveScr(SaveScr);

    int GetCode;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_GETFILES);
    if(Opt.ExceptRules)
    {
      TRY{
        GetCode=PData->pGetFiles(ph->InternalHandle,PanelItem,1,0,DestPath,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_GETFILES,GetExceptionInformation(),PData,1))
      {
        PData->FuncFlags.Clear(PICFF_GETFILES);
        UnloadPlugin(*PData,EXCEPT_GETFILES); //????!!!!
        ProcessException=FALSE;
        // ??????????
        ReadUserBackgound(SaveScr);
        delete SaveScr;
        // ??????????
        return(Found);
      }
    }
    else
      GetCode=PData->pGetFiles(ph->InternalHandle,PanelItem,1,0,DestPath,OpMode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_GETFILES);

    char FindPath[NM];
    xstrncpy(FindPath,DestPath,sizeof(FindPath)-2);
    AddEndSlash(FindPath);
    strncat(FindPath,"*.*",sizeof(FindPath)-1);
    HANDLE FindHandle;
    WIN32_FIND_DATA fdata;
    if ((FindHandle=FindFirstFile(FindPath,&fdata))!=INVALID_HANDLE_VALUE)
    {
      int Done=0;
      while (!Done)
      {
        if ((fdata.dwFileAttributes & FA_DIREC)==0)
          break;
        Done=!FindNextFile(FindHandle,&fdata);
      }
      FindClose(FindHandle);
      if (!Done)
      {
        strcpy(ResultName,DestPath);
        AddEndSlash(ResultName);
        strcat(ResultName,fdata.cFileName);
        if (GetCode!=1)
        {
          SetFileAttributes(ResultName,FILE_ATTRIBUTE_NORMAL);
          remove(ResultName);
        }
        else
          Found=TRUE;
      }
    }
    ReadUserBackgound(SaveScr);

    if(SaveScr)
      delete SaveScr;
  }

  return(Found);
}


int PluginsSet::DeleteFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,
                            int ItemsNumber,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::DeleteFiles()"));
  _ALGO(SysLog("hPlugin=%p, PanelItem=%p, ItemsNumber=%d, OpMode=%d",hPlugin,PanelItem,ItemsNumber,OpMode));
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pDeleteFiles && !ProcessException)
  {
    ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
    SaveScreen SaveScr;
    KeepUserScreen=FALSE;
    int Code;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_DELETEFILES);
    if(Opt.ExceptRules)
    {
      TRY{
        Code=PData->pDeleteFiles(ph->InternalHandle,PanelItem,ItemsNumber,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_DELETEFILES,GetExceptionInformation(),PData,1) )
      {
        UnloadPlugin(*PData,EXCEPT_DELETEFILES); //????!!!!
        Code=FALSE;
        ProcessException=FALSE;
      }
    }
    else
      Code=PData->pDeleteFiles(ph->InternalHandle,PanelItem,ItemsNumber,OpMode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_DELETEFILES);
    ReadUserBackgound(&SaveScr);
    return(Code);
  }
  return(FALSE);
}


int PluginsSet::MakeDirectory(HANDLE hPlugin,char *Name,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::MakeDirectory()"));
  _ALGO(SysLog("hPlugin=%p, Name='%s', OpMode=%d",hPlugin,(Name?Name:"(NULL)"),OpMode));
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pMakeDirectory && !ProcessException)
  {
    ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
    SaveScreen SaveScr;
    KeepUserScreen=FALSE;
    int Code;

    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_MAKEDIRECTORY);
    if(Opt.ExceptRules)
    {
      TRY{
        Code=PData->pMakeDirectory(ph->InternalHandle,Name,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_MAKEDIRECTORY,GetExceptionInformation(),PData,1) )
      {
        UnloadPlugin(*PData,EXCEPT_MAKEDIRECTORY); //????!!!!
        Code=-1;
        ProcessException=FALSE;
      }
    }
    else
      Code=PData->pMakeDirectory(ph->InternalHandle,Name,OpMode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_MAKEDIRECTORY);

    ReadUserBackgound(&SaveScr);
    return(Code);
  }
  return(-1);
}


int PluginsSet::ProcessHostFile(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::ProcessHostFile()"));
  _ALGO(SysLog("hPlugin=%p, PanelItem=%p, ItemsNumber=%d, OpMode=%d",hPlugin,PanelItem,ItemsNumber,OpMode));
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pProcessHostFile && !ProcessException)
  {
    ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
    SaveScreen SaveScr;
    KeepUserScreen=FALSE;
    int Code;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_PROCESSHOSTFILE);
    if(Opt.ExceptRules)
    {
      TRY{
        Code=PData->pProcessHostFile(ph->InternalHandle,PanelItem,ItemsNumber,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_PROCESSHOSTFILE,GetExceptionInformation(),PData,1)){
        UnloadPlugin(*PData,EXCEPT_PROCESSHOSTFILE); //????!!!!
        Code=FALSE;
        ProcessException=FALSE;
      }
    }
    else
      Code=PData->pProcessHostFile(ph->InternalHandle,PanelItem,ItemsNumber,OpMode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_PROCESSHOSTFILE);
    ReadUserBackgound(&SaveScr);
    return(Code);
  }
  return(FALSE);
}


int PluginsSet::GetFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,
                int ItemsNumber,int Move,char *DestPath,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::GetFiles()"));
  //_ALGO(SysLog("hPlugin=%p, PanelItem=%p, ItemsNumber=%d, OpMode=%d",hPlugin,PanelItem,ItemsNumber,OpMode));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  int ExitCode=FALSE;
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pGetFiles && !ProcessException)
  {
    SaveScreen SaveScr;
    KeepUserScreen=FALSE;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_GETFILES);
    if(Opt.ExceptRules)
    {
      TRY{
        ExitCode=PData->pGetFiles(ph->InternalHandle,PanelItem,ItemsNumber,Move,DestPath,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_GETFILES,GetExceptionInformation(),PData,1)){
        UnloadPlugin(*PData,EXCEPT_GETFILES); //????!!!!
        ExitCode=0;
        ProcessException=FALSE;
      }
    }
    else
      ExitCode=PData->pGetFiles(ph->InternalHandle,PanelItem,ItemsNumber,Move,DestPath,OpMode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_GETFILES);
  }
  return(ExitCode);
}


int PluginsSet::PutFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int Move,int OpMode)
{
  _ALGO(CleverSysLog clv("PluginsSet::PutFiles()"));
  _ALGO(SysLog("hPlugin=%p, PanelItem=%p, ItemsNumber=%d, Move=%d, OpMode=%d",hPlugin,PanelItem,ItemsNumber,Move,OpMode));
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pPutFiles && !ProcessException)
  {
    ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
    SaveScreen SaveScr;
    KeepUserScreen=FALSE;
    int Code;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_PUTFILES);
    if(Opt.ExceptRules)
    {
      TRY{
        Code=PData->pPutFiles(ph->InternalHandle,PanelItem,ItemsNumber,Move,OpMode);
      }
      EXCEPT(xfilter(EXCEPT_PUTFILES,GetExceptionInformation(),PData,1) )
      {
        UnloadPlugin(*PData,EXCEPT_PUTFILES); //????!!!!
        Code=0;
        ProcessException=FALSE;
      }
    }
    else
      Code=PData->pPutFiles(ph->InternalHandle,PanelItem,ItemsNumber,Move,OpMode);
    _ALGO(SysLog("return Code=%d",Code));
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_PUTFILES);
    ReadUserBackgound(&SaveScr);
    return(Code);
  }
  return(FALSE);
}


int PluginsSet::GetPluginInfo(int PluginNumber,struct PluginInfo *Info)
{
  _ALGO(CleverSysLog clv("PluginsSet::GetPluginInfo()"));
  _ALGO(SysLog("PluginNumber=%p, Info=%p",PluginNumber,Info));
  if (PluginNumber>=PluginsCount || !Info)
    return(FALSE);
  memset(Info,0,sizeof(*Info));
  struct PluginItem *PData=PluginsData+PluginNumber;
  if (PData->pGetPluginInfo && !ProcessException)
  {
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_GETPLUGININFO);
    if(Opt.ExceptRules)
    {
      TRY{
        PData->pGetPluginInfo(Info);
      }
      EXCEPT(xfilter(EXCEPT_GETPLUGININFO,GetExceptionInformation(),PData,1)){
        PData->FuncFlags.Clear(PICFF_GETPLUGININFO);
        UnloadPlugin(*PData,EXCEPT_GETPLUGININFO); //????!!!!
        ProcessException=FALSE;
        return FALSE;
      }
      if(!TestPluginInfo(*PData,Info)) //???
      {
        PData->FuncFlags.Clear(PICFF_GETPLUGININFO);
        return FALSE;
      }
    }
    else
      PData->pGetPluginInfo(Info);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_GETPLUGININFO);
  }
  return(TRUE);
}


void PluginsSet::GetOpenPluginInfo(HANDLE hPlugin,struct OpenPluginInfo *Info)
{
  _ALGO(CleverSysLog clv("PluginsSet::GetOpenPluginInfo()"));
  _ALGO(SysLog("hPlugin=%p, Info=%p",hPlugin,Info));
  if (!Info)
    return;
  memset(Info,0,sizeof(*Info));
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pGetOpenPluginInfo && !ProcessException)
  {
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_GETOPENPLUGININFO);
    if(Opt.ExceptRules)
    {
      TRY{
        PData->pGetOpenPluginInfo(ph->InternalHandle,Info);
      }
      EXCEPT(xfilter(EXCEPT_GETOPENPLUGININFO,GetExceptionInformation(),PData,1)){
        PData->FuncFlags.Clear(PICFF_GETOPENPLUGININFO);
        UnloadPlugin(*PData,EXCEPT_GETOPENPLUGININFO); //????!!!!
        ProcessException=FALSE;
        return;
      }
      if(!TestOpenPluginInfo(*PData,Info)) // ??
      {
        PData->FuncFlags.Clear(PICFF_GETOPENPLUGININFO);
        return;
      }
    }
    else
      PData->pGetOpenPluginInfo(ph->InternalHandle,Info);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_GETOPENPLUGININFO);
    _ALGO(GetOpenPluginInfo_Dump("",Info,NULL));
  }
  if (Info->CurDir==NULL)
    Info->CurDir="";
}


int PluginsSet::ProcessKey(HANDLE hPlugin,int Key,unsigned int ControlState)
{
  _ALGO(CleverSysLog clv("PluginsSet::ProcessKey()"));
  _ALGO(SysLog("hPlugin=%p, Key=%u (0x%08X) ControlState=%u (0x%08X) ",hPlugin,Key,Key,ControlState,ControlState));
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pProcessKey && !ProcessException)
  {
    int Ret;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_PROCESSKEY);
    if(Opt.ExceptRules)
    {
      TRY{
        Ret=PData->pProcessKey(ph->InternalHandle,Key,ControlState);
      }
      EXCEPT(xfilter(EXCEPT_PROCESSKEY,GetExceptionInformation(),PData,1)){
        UnloadPlugin(*PData,EXCEPT_PROCESSKEY); //????!!!!
        Ret=FALSE;
        ProcessException=FALSE;
      }
    }
    else
      Ret=PData->pProcessKey(ph->InternalHandle,Key,ControlState);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_PROCESSKEY);
    return(Ret);
  }
  return(FALSE);
}


int PluginsSet::ProcessEvent(HANDLE hPlugin,int Event,void *Param)
{
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pProcessEvent && !ProcessException)
  {
    int Ret;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_PROCESSEVENT);
    if(Opt.ExceptRules)
    {
      TRY{
        Ret=PData->pProcessEvent(ph->InternalHandle,Event,Param);
      }
      EXCEPT(xfilter(EXCEPT_PROCESSEVENT,GetExceptionInformation(),PData,1)){
        UnloadPlugin(*PData,EXCEPT_PROCESSEVENT); //????!!!!
        Ret=FALSE;
        ProcessException=FALSE;
      }
    }
    else
      Ret=PData->pProcessEvent(ph->InternalHandle,Event,Param);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_PROCESSEVENT);
    return(Ret);
  }
  return(FALSE);
}


int PluginsSet::Compare(HANDLE hPlugin,const struct PluginPanelItem *Item1,
                        const struct PluginPanelItem *Item2,unsigned int Mode)
{
  struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
  struct PluginItem *PData=PluginsData+ph->PluginNumber;
  if (PData->pCompare && !ProcessException)
  {
    int Ret;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_COMPARE);
    if(Opt.ExceptRules)
    {
      TRY{
        Ret=PData->pCompare(ph->InternalHandle,Item1,Item2,Mode);
      }
      EXCEPT(xfilter(EXCEPT_COMPARE,GetExceptionInformation(),PData,1)) {
        UnloadPlugin(*PData,EXCEPT_COMPARE); //????!!!!
        Ret=-3;
        ProcessException=FALSE;
      }
    }
    else
      Ret=PData->pCompare(ph->InternalHandle,Item1,Item2,Mode);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_COMPARE);
    return(Ret);
  }
  return(-3);
}

void PluginsSet::ConfigureCurrent(int PNum,int INum)
{
  if (PreparePlugin(PNum) && PluginsData[PNum].pConfigure!=NULL && !ProcessException)
  {
    int Ret;
    struct PluginItem *PData=PluginsData+PNum;
    //CurPluginItem=PData;
    PData->FuncFlags.Set(PICFF_CONFIGURE);
    if(Opt.ExceptRules)
    {
      TRY{
        Ret=PData->pConfigure(INum);
      }
      EXCEPT(xfilter(EXCEPT_CONFIGURE,GetExceptionInformation(),PData,1))
      {
        PData->FuncFlags.Clear(PICFF_CONFIGURE);
        UnloadPlugin(*PData,EXCEPT_CONFIGURE); //????!!!!
        ProcessException=FALSE;
        return;
      }
    }
    else
      Ret=PData->pConfigure(INum);
    //CurPluginItem=NULL;
    PData->FuncFlags.Clear(PICFF_CONFIGURE);

    if (Ret)
    {
      int PMode[2];
      PMode[0]=CtrlObject->Cp()->LeftPanel->GetMode();
      PMode[1]=CtrlObject->Cp()->RightPanel->GetMode();
      for(int I=0; I < sizeof(PMode)/sizeof(PMode[0]); ++I)
      {
        if(PMode[I] == PLUGIN_PANEL)
        {
          Panel *pPanel=(I==0?CtrlObject->Cp()->LeftPanel:CtrlObject->Cp()->RightPanel);
          pPanel->Update(UPDATE_KEEP_SELECTION);
          pPanel->SetViewMode(pPanel->GetViewMode());
          pPanel->Redraw();
        }
      }
    }
    SavePluginSettings(*PData,PData->FindData);
  }
}

/* $ 29.05.2001 IS
   ! ��� ��������� "���������� ������� �������" ��������� ���� � ��
     ������� ������ ��� ������� �� ESC
*/
void PluginsSet::Configure(int StartPos)
{
  // ������� 4 - ��������� ������� �������
  if(Opt.Policies.DisabledOptions&FFPOL_MAINMENUPLUGINS)
    return;

  {
    VMenu PluginList(MSG(MPluginConfigTitle),NULL,0,ScrY-4);
    PluginList.SetFlags(VMENU_WRAPMODE);
    PluginList.SetHelp("PluginsConfig");

    for(;;)
    {
      DWORD Data;
      int I, J;
      BOOL NeedUpdateItems=TRUE;
      BOOL Done=FALSE;
      int MenuItemNumber=0;


      char FirstHotKey[512];
      int HotKeysPresent=EnumRegKey("PluginHotkeys",0,FirstHotKey,sizeof(FirstHotKey));


      if(NeedUpdateItems)
      {
        PluginList.ClearDone();
        PluginList.DeleteItems();
        PluginList.SetPosition(-1,-1,0,0);
        MenuItemNumber=0;

        LoadIfCacheAbsent();
        char HotRegKey[512],HotKey[100];

        *HotKey=0;

        for (I=0;I<PluginsCount;I++)
        {
          if (PluginsData[I].WorkFlags.Check(PIWF_CACHED))
          {
            char RegKey[100],Value[100];
            int RegNumber=GetCacheNumber(PluginsData[I].ModuleName,NULL,PluginsData[I].CachePos);
            if (RegNumber==-1)
              continue;
            else
              for (J=0;;J++)
              {
                *HotKey=0;
                if (GetHotKeyRegKey(I,J,HotRegKey))
                  GetRegKey(HotRegKey,"ConfHotkey",HotKey,"",sizeof(HotKey));
                struct MenuItem ListItem;
                memset(&ListItem,0,sizeof(ListItem));
                sprintf(RegKey,FmtPluginsCache_PluginD,RegNumber);
                sprintf(Value,FmtPluginConfigStringD,J);
                char Name[sizeof(ListItem.Name)];
                GetRegKey(RegKey,Value,Name,"",sizeof(Name));
                if (*Name==0)
                  break;
                if (!HotKeysPresent)
                  strcpy(ListItem.Name,Name);
                else
                  if (*HotKey)
                    sprintf(ListItem.Name,"&%c%s  %s",*HotKey,(*HotKey=='&'?"&":""),Name);
                  else
                    sprintf(ListItem.Name,"   %s",Name);
                //ListItem.SetSelect(MenuItemNumber++ == StartPos);
                MenuItemNumber++;
                Data=MAKELONG(I,J);
                PluginList.SetUserData((void*)Data,sizeof(Data),PluginList.AddItem(&ListItem));
              }
          }
          else
          {
            struct PluginInfo Info;
            if (!GetPluginInfo(I,&Info))
              continue;
            for (J=0;J<Info.PluginConfigStringsNumber;J++)
            {
              *HotKey=0;
              if (GetHotKeyRegKey(I,J,HotRegKey))
                GetRegKey(HotRegKey,"ConfHotkey",HotKey,"",sizeof(HotKey));
              struct MenuItem ListItem;
              memset(&ListItem,0,sizeof(ListItem));
              char Name[sizeof(ListItem.Name)];
              xstrncpy(Name,NullToEmpty(Info.PluginConfigStrings[J]),sizeof(Name)-1);
              if (!HotKeysPresent)
                strcpy(ListItem.Name,Name);
              else
                if (*HotKey)
                  sprintf(ListItem.Name,"&%c%s  %s",*HotKey,(*HotKey=='&'?"&":""),Name);
                else
                  sprintf(ListItem.Name,"   %s",Name);
              //ListItem.SetSelect(MenuItemNumber++ == StartPos);
              MenuItemNumber++;
              Data=MAKELONG(I,J);
              PluginList.SetUserData((void*)Data,sizeof(Data),PluginList.AddItem(&ListItem));
            }
          }
        }
        PluginList.AssignHighlights(FALSE);
        PluginList.SetBottomTitle(MSG(MPluginHotKeyBottom));

        PluginList.ClearDone();
        PluginList.SortItems(0,3);
        PluginList.SetSelectPos(StartPos,1);
        NeedUpdateItems=FALSE;
      }

      /* $ 18.12.2000 SVS
         Shift-F1 � ������ �������� �������� ���� �� ������� �������
      */
      PluginList.Show();
      while (!PluginList.Done())
      {
        int SelPos=PluginList.GetSelectPos();
        Data=(DWORD)PluginList.GetUserData(NULL,0,SelPos);
        char RegKey[512];
        switch(PluginList.ReadInput())
        {
          case KEY_SHIFTF1:
            char PluginModuleName[NM*2];
            strcpy(PluginModuleName,PluginsData[LOWORD(Data)].ModuleName);
            if(!FarShowHelp(PluginModuleName,"Config",FHELP_SELFHELP|FHELP_NOSHOWERROR) &&
               !FarShowHelp(PluginModuleName,"Configure",FHELP_SELFHELP|FHELP_NOSHOWERROR))
            {
              //strcpy(PluginModuleName,PluginsData[Data[0]].ModuleName);
              FarShowHelp(PluginModuleName,NULL,FHELP_SELFHELP|FHELP_NOSHOWERROR);
            }
            break;
          case KEY_F4:
            if (PluginList.GetItemCount() > 0 && SelPos<MenuItemNumber && GetHotKeyRegKey(LOWORD(Data),HIWORD(Data),RegKey))
            {
              BlockExtKey blockExtKey;
              struct PluginInfo Info;
              char Name00[NM];

              int nOffset = HotKeysPresent?3:0;

              xstrncpy(Name00,PluginList.GetItemPtr()->Name+nOffset,sizeof(Name00)-1);
              RemoveExternalSpaces(Name00);
              if(GetMenuHotKey(NULL,1,
                        (char *)MPluginHotKeyTitle,
                        (char *)MPluginHotKey,
                        Name00,
                        NULL,RegKey,"ConfHotkey"))
              {
                PluginList.Hide();
                NeedUpdateItems=TRUE;
                StartPos=SelPos;
                PluginList.SetExitCode(SelPos);
                /* ������� ��� :-( */
//              PluginList.Hide();
                PluginList.Show();
                break;
              }
            }
            break;
          default:
            PluginList.ProcessInput();
            break;
        }
      }

      if(!NeedUpdateItems)
      {
        StartPos=PluginList.Modal::GetExitCode();
        PluginList.Hide();
        if (StartPos<0)
          break;
        Data=(DWORD)PluginList.GetUserData(NULL,0,StartPos);
        // ��������� �������, ��� ��� �����������!
        //  ���� ��� ��������, ��� ����� �� ��� ��� ����� �����������,
        //  ���� �� ��� �� �������?
        ConfigureCurrent(LOWORD(Data),HIWORD(Data));
      }
    }
  }
}
/* IS $ */

///int PluginsSet::CommandsMenu(int Editor,int Viewer,int StartPos,char *HistoryName)
int PluginsSet::CommandsMenu(int ModalType,int StartPos,const char *HistoryName)
{
  int MenuItemNumber=0;
/* $ 04.05.2001 OT */
  int Editor,Viewer;

  Editor = ModalType==MODALTYPE_EDITOR;
  Viewer = ModalType==MODALTYPE_VIEWER;
  /* OT $ */

  DWORD Data;
  {
    VMenu PluginList(MSG(MPluginCommandsMenuTitle),NULL,0,ScrY-4);
    PluginList.SetFlags(VMENU_WRAPMODE);
    PluginList.SetHelp("Plugins");
    BOOL NeedUpdateItems=TRUE;
    BOOL Done=FALSE;


    while(!Done)
    {

      char FirstHotKey[512];
      int HotKeysPresent=EnumRegKey("PluginHotkeys",0,FirstHotKey,sizeof(FirstHotKey));

      if(NeedUpdateItems)
      {
        PluginList.ClearDone();
        PluginList.DeleteItems();
        PluginList.SetPosition(-1,-1,0,0);

        LoadIfCacheAbsent();

        for (int I=0;I<PluginsCount;I++)
        {
          char HotRegKey[512],HotKey[100];
          *HotKey=0;
          if (PluginsData[I].WorkFlags.Check(PIWF_CACHED))
          {
            char RegKey[100],Value[100];
            int RegNumber=GetCacheNumber(PluginsData[I].ModuleName,NULL,PluginsData[I].CachePos);
            if (RegNumber==-1)
              continue;
            else
            {
              sprintf(RegKey,FmtPluginsCache_PluginD,RegNumber);
              int IFlags=GetRegKey(RegKey,"Flags",0);
              /* todo: ��� ���� �� �������� �� Editor/Viewer
              � ������� ������ ������ �� ModalType */
              if (Editor && (IFlags & PF_EDITOR)==0 ||
                Viewer && (IFlags & PF_VIEWER)==0 ||
                !Editor && !Viewer && (IFlags & PF_DISABLEPANELS))
                continue;
              for (int J=0;;J++)
              {
                *HotKey=0;
                if (GetHotKeyRegKey(I,J,HotRegKey))
                  GetRegKey(HotRegKey,"Hotkey",HotKey,"",sizeof(HotKey));
                struct MenuItem ListItem;
                memset(&ListItem,0,sizeof(ListItem));
                sprintf(Value,FmtPluginMenuStringD,J);
                char Name[sizeof(ListItem.Name)];
                GetRegKey(RegKey,Value,Name,"",sizeof(Name));
                if (*Name==0)
                  break;
                if (!HotKeysPresent)
                  sprintf(ListItem.Name,"   %s",Name);//strcpy(ListItem.Name,Name);
                else
                  if (*HotKey)
                    sprintf(ListItem.Name,"&%c%s  %s",*HotKey,(*HotKey=='&'?"&":""),Name);
                  else
                    sprintf(ListItem.Name,"   %s",Name);
                  //ListItem.SetSelect(MenuItemNumber++ == StartPos);
                  MenuItemNumber++;
                  Data=MAKELONG(I,J);
                  PluginList.SetUserData((void*)Data,sizeof(Data),PluginList.AddItem(&ListItem));
              }
            }
          }
          else
          {
            struct PluginInfo Info;
            if (!GetPluginInfo(I,&Info))
              continue;
            if (Editor && (Info.Flags & PF_EDITOR)==0 ||
              Viewer && (Info.Flags & PF_VIEWER)==0 ||
              !Editor && !Viewer && (Info.Flags & PF_DISABLEPANELS))
              continue;
            for (int J=0;J<Info.PluginMenuStringsNumber;J++)
            {
              *HotKey=0;
              if (GetHotKeyRegKey(I,J,HotRegKey))
                GetRegKey(HotRegKey,"Hotkey",HotKey,"",sizeof(HotKey));
              struct MenuItem ListItem;
              memset(&ListItem,0,sizeof(ListItem));
              char Name[sizeof(ListItem.Name)];
              xstrncpy(Name,NullToEmpty(Info.PluginMenuStrings[J]),sizeof(Name)-1);
              if (!HotKeysPresent)
                sprintf(ListItem.Name,"   %s",Name);//strcpy(ListItem.Name,Name);
              else
                if (*HotKey)
                  sprintf(ListItem.Name,"&%c%s  %s",*HotKey,(*HotKey=='&'?"&":""),Name);
                else
                  sprintf(ListItem.Name,"   %s",Name);
                //ListItem.SetSelect(MenuItemNumber++ == StartPos);
                MenuItemNumber++;
                Data=MAKELONG(I,J);
                PluginList.SetUserData((void*)Data,sizeof(Data),PluginList.AddItem(&ListItem));
            }
          }
        }

        PluginList.AssignHighlights(FALSE);
        PluginList.SetBottomTitle(MSG(MPluginHotKeyBottom));

        PluginList.SortItems(0,3);
        PluginList.SetSelectPos(StartPos,1);
        NeedUpdateItems=FALSE;
      }

      PluginList.Show();

      while (!PluginList.Done())
      {
        int SelPos=PluginList.GetSelectPos();
        char RegKey[512];

        Data=(DWORD)PluginList.GetUserData(NULL,0,SelPos);
        switch(PluginList.ReadInput())
        {
        /* $ 18.12.2000 SVS
        Shift-F1 � ������ �������� �������� ���� �� ������� �������
          */
        case KEY_SHIFTF1:
          {
            // �������� ������ �����, ������� �������� � CommandsMenu()
            FarShowHelp(PluginsData[LOWORD(Data)].ModuleName,HistoryName,FHELP_SELFHELP|FHELP_NOSHOWERROR|FHELP_USECONTENTS);
            break;
          }
          /* SVS $ */
        case KEY_ALTF11:
          WriteEvent(FLOG_PLUGINSINFO);
          break;
        case KEY_F4:
          if (PluginList.GetItemCount() > 0 && SelPos<MenuItemNumber && GetHotKeyRegKey(LOWORD(Data),HIWORD(Data),RegKey))
          {
            BlockExtKey blockExtKey;
            char Name00[NM];

            int nOffset = HotKeysPresent?3:0;

            xstrncpy(Name00,PluginList.GetItemPtr()->Name+nOffset,sizeof(Name00)-1);
            RemoveExternalSpaces(Name00);
            if(GetMenuHotKey(NULL,1,(char *)MPluginHotKeyTitle,(char *)MPluginHotKey,Name00,NULL,RegKey,"Hotkey"))
            {
              PluginList.Hide();
              NeedUpdateItems=TRUE;
              StartPos=SelPos;
              PluginList.SetExitCode(SelPos);
              /* ������� ��� :-( */
//            PluginList.Hide();
              PluginList.Show();
              break;
            }
          }
          break;
        default:
          PluginList.ProcessInput();
          break;
        }
      }

      if(!NeedUpdateItems && PluginList.Done())
        break;
    }

    int ExitCode=PluginList.Modal::GetExitCode();

    PluginList.Hide();
    if (ExitCode<0)
      return(FALSE);
    ScrBuf.Flush();
    Data=(DWORD)PluginList.GetUserData(NULL,0,ExitCode);
  }
  if (PreparePlugin(LOWORD(Data)) && PluginsData[LOWORD(Data)].pOpenPlugin!=NULL && !ProcessException)
  {
    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
//    if (ActivePanel->ProcessPluginEvent(FE_CLOSE,NULL))
//      return(FALSE);
    int OpenCode=OPEN_PLUGINSMENU;
    if (Editor)
      OpenCode=OPEN_EDITOR;
    if (Viewer)
      OpenCode=OPEN_VIEWER;
    HANDLE hPlugin=OpenPlugin(LOWORD(Data),OpenCode,HIWORD(Data));
    if (hPlugin!=INVALID_HANDLE_VALUE && !Editor && !Viewer)
    {
      if ( ActivePanel->ProcessPluginEvent(FE_CLOSE,NULL) )
      {
        ClosePlugin (hPlugin);
        return(FALSE);
      }


      Panel *NewPanel=CtrlObject->Cp()->ChangePanel(ActivePanel,FILE_PANEL,TRUE,TRUE);
      NewPanel->SetPluginMode(hPlugin,"");
      NewPanel->Update(0);
      NewPanel->Show();
      NewPanel->SetFocus();
    }
    /* $ 14.05.2002 SKV
       �������� ��������, ����� � ���� ����� CurEditor==NULL
    */
    if (Editor && CurEditor){
      CurEditor->SetPluginTitle(NULL);
    }
    /* SKV $ */
  }
  return(TRUE);
}

int PluginsSet::GetHotKeyRegKey(int PluginNumber,int ItemNumber,char *RegKey)
{
#if 1
// ������ �������
/*
FarPath
C:\Program Files\Far\

ModuleName                                             PluginName
---------------------------------------------------------------------------------------
C:\Program Files\Far\Plugins\MultiArc\MULTIARC.DLL  -> Plugins\MultiArc\MULTIARC.DLL
C:\MultiArc\MULTIARC.DLL                            -> DLL
---------------------------------------------------------------------------------------
*/
  unsigned int FarPathLength=strlen(FarPath);
  *RegKey=0;
  if (FarPathLength < strlen(PluginsData[PluginNumber].ModuleName))
  {
    char PluginName[NM], *Ptr;
//    strcpy(PluginName,PluginsData[PluginNumber].ModuleName+FarPathLength);
    strcpy(PluginName,PluginsData[PluginNumber].ModuleName+
           (strncmp(PluginsData[PluginNumber].ModuleName,FarPath,FarPathLength)?0:FarPathLength));
    for (Ptr=PluginName;*Ptr;++Ptr)
      if (*Ptr=='\\')
        *Ptr='/';
    if (ItemNumber>0)
      sprintf(Ptr,"%%%d",ItemNumber);
    sprintf(RegKey,"PluginHotkeys\\%s",PluginName);
    return(TRUE);
  }
  return(FALSE);
#else
// ����� �������, � ������� ������
  *RegKey=0;
  char PluginName[NM], *Ptr;
  strcpy(PluginName,PluginsData[PluginNumber].ModuleName);
  for (Ptr=PluginName;*Ptr;++Ptr)
  {
    if (*Ptr=='\\')
      *Ptr='/';
  }
  if (ItemNumber>0)
    sprintf(Ptr,"%%%d",ItemNumber);

  sprintf(RegKey,"PluginHotkeys\\%s",PluginName);

#if 1
  // ����������� - ����� ������
  unsigned int FarPathLength=strlen(FarPath);
  if (FarPathLength < strlen(PluginsData[PluginNumber].ModuleName))
  {
    char OldRevKey[NM];
    sprintf(OldRevKey,"PluginHotkeys\\%s",PluginName+FarPathLength);
    if(CheckRegKey(OldRevKey))
    {
      char ConfHotkey[32];
      char Hotkey[32];
      GetRegKey(OldRevKey,"ConfHotkey",ConfHotkey,"",sizeof(ConfHotkey));
      GetRegKey(OldRevKey,"Hotkey",Hotkey,"",sizeof(Hotkey));
      if(*ConfHotkey)
        SetRegKey(RegKey,"ConfHotkey",ConfHotkey);
      if(*Hotkey)
        SetRegKey(RegKey,"Hotkey",Hotkey);
      DeleteRegKey(OldRevKey); //??? � �����?
    }
  }
#endif
  return(TRUE);
#endif
}

/* $ 21.08.2002 IS
   + �������� PluginTextSize, ����� �����, ������� �����
*/
int PluginsSet::GetDiskMenuItem(int PluginNumber,int PluginItem,
                int &ItemPresent,int &PluginTextNumber,char *PluginText,
                DWORD PluginTextSize)
{
  if (PluginNumber>=PluginsCount)
    return(FALSE);

  LoadIfCacheAbsent();

  struct PluginItem *PData=PluginsData+PluginNumber;
  if (PData->WorkFlags.Check(PIWF_CACHED))
  {
    char RegKey[100],Value[100];
    int RegNumber=GetCacheNumber(PData->ModuleName,NULL,PData->CachePos);
    if (RegNumber==-1)
      ItemPresent=0;
    else
    {
      sprintf(RegKey,FmtPluginsCache_PluginD,RegNumber);
      sprintf(Value,FmtDiskMenuStringD,PluginItem);
      GetRegKey(RegKey,Value,PluginText,"",PluginTextSize);
      sprintf(Value,FmtDiskMenuNumberD,PluginItem);
      GetRegKey(RegKey,Value,PluginTextNumber,0);
      ItemPresent=*PluginText!=0;
    }
    return(TRUE);
  }
  struct PluginInfo Info;
  if (!GetPluginInfo(PluginNumber,&Info) || Info.DiskMenuStringsNumber <= PluginItem)
    ItemPresent=FALSE;
  else
  {
    if (Info.DiskMenuNumbers)
      PluginTextNumber=Info.DiskMenuNumbers[PluginItem];
    else
      PluginTextNumber=0;
    xstrncpy(PluginText,Info.DiskMenuStrings[PluginItem],PluginTextSize-1);
    ItemPresent=TRUE;
  }
  return(TRUE);
}
/* IS $ */

int PluginsSet::UseFarCommand(HANDLE hPlugin,int CommandType)
{
  struct OpenPluginInfo Info;
  GetOpenPluginInfo(hPlugin,&Info);
  if ((Info.Flags & OPIF_REALNAMES)==0)
    return(FALSE);

  struct PluginItem *PData=PluginsData+((struct PluginHandle *)hPlugin)->PluginNumber;
  switch(CommandType)
  {
    case PLUGIN_FARGETFILE:
    case PLUGIN_FARGETFILES:
      return(PData->pGetFiles==NULL || (Info.Flags & OPIF_EXTERNALGET));
    case PLUGIN_FARPUTFILES:
      return(PData->pPutFiles==NULL || (Info.Flags & OPIF_EXTERNALPUT));
    case PLUGIN_FARDELETEFILES:
      return(PData->pDeleteFiles==NULL || (Info.Flags & OPIF_EXTERNALDELETE));
    case PLUGIN_FARMAKEDIRECTORY:
      return(PData->pMakeDirectory==NULL || (Info.Flags & OPIF_EXTERNALMKDIR));
  }
  return(TRUE);
}


void PluginsSet::ReloadLanguage()
{
  struct PluginItem *PData=PluginsData;
  for (int I=0;I<PluginsCount;I++,PData++)
    PData->Lang.Close();
  DiscardCache();
}


void PluginsSet::DiscardCache()
{
  for (int I=0;I<PluginsCount;I++)
    PreparePlugin(I);
  DeleteKeyTree(RKN_PluginsCache);
}


void PluginsSet::LoadIfCacheAbsent()
{
  if (!CheckRegKey(RKN_PluginsCache))
    for (int I=0;I<PluginsCount;I++)
      PreparePlugin(I);
}


int PluginsSet::ProcessCommandLine(const char *CommandParam,Panel *Target)
{
  int PrefixLength=0;
  char *Command=(char *)alloca(strlen(CommandParam)+1);
  if(!Command)
    return FALSE;
  strcpy(Command,CommandParam);
  UnquoteExternal(Command);
  RemoveLeadingSpaces(Command);

  while (1)
  {
    int Ch=Command[PrefixLength];
    if (Ch==0 || IsSpace(Ch) || Ch=='/' || PrefixLength>64)
      return(FALSE);
    if (Ch==':' && PrefixLength>0)
      break;
    PrefixLength++;
  }

  LoadIfCacheAbsent();

  char *Prefix=(char *)alloca(PrefixLength+1);
  if(!Prefix)
    return FALSE;

  xstrncpy(Prefix,Command,PrefixLength);
  Prefix[PrefixLength]=0;

  int PluginPos=-1;
  /* $ 07.09.2000 VVM 1.18
     + ��������� ��������� � �������, ����������� ����� ":"
  */
  DWORD PluginFlags = 0;
  char PluginPrefix[512]="";

  struct PluginItem *PData=PluginsData;
  for (int I=0;I<PluginsCount;I++,PData++)
  {
    if (PData->WorkFlags.Check(PIWF_CACHED))
    {
      int RegNumber=GetCacheNumber(PData->ModuleName,NULL,PData->CachePos);
      if (RegNumber!=-1)
      {
        char RegKey[100];
        sprintf(RegKey,FmtPluginsCache_PluginD,RegNumber);
        GetRegKey(RegKey,"CommandPrefix",PluginPrefix,"",sizeof(PluginPrefix));
        /* $ 07.09.2000 SVS
             �� �����-�� ���������� �������� �� ���� ��� Flags ������������
             �������� ������ 0 (���� ���� ��� � ������� ����� 0x10) :-(
        */
        PluginFlags=GetRegKey(RegKey,"Flags",0);
        /* SVS $ */
      } /* if */
      else
        continue;
    } /* if */
    else
    {
      struct PluginInfo Info;
      if (GetPluginInfo(I,&Info))
      {
        /* $ 10.09.2000 IS
             ���, ���, ��� �� ������... ������ �������� Info.CommandPrefix ��
             NULL �������, �������������� ��� ������ � ������������ �������,
             ������ - ���.
        */
        xstrncpy(PluginPrefix,NullToEmpty(Info.CommandPrefix),sizeof(PluginPrefix)-1);
        /* IS $ */
        PluginFlags = Info.Flags;
      } /* if */
      else
        continue;
    } /* else */
    if (PluginPrefix[0]==0)
      continue;
    char *PrStart = PluginPrefix;
    /*$ 10.07.2001 SKV
      ������� ������ ��������� ���������,
      � �� ������...
    */
    int PrefixLength=strlen(Prefix);
    while(1)
    {
      char *PrEnd = strchr(PrStart, ':');
      int Len=PrEnd==NULL ? strlen(PrStart):(PrEnd-PrStart);
      if(Len<PrefixLength)Len=PrefixLength;
      if (LocalStrnicmp(Prefix, PrStart, Len)==0)
      /* SKV$*/
      {
        PluginPos=I;
        break;
      } /* if */
      if (PrEnd == NULL)
        break;
      PrStart = ++PrEnd;
    } /* while */
    if (PluginPos >= 0)
      break;
  } /* for */
  /* VVM $ */

  if (PluginPos==-1)
    return(FALSE);
  if (!PreparePlugin(PluginPos) || PluginsData[PluginPos].pOpenPlugin==NULL)
    return(FALSE);
  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  Panel *CurPanel=(Target)?Target:ActivePanel;
  if (CurPanel->ProcessPluginEvent(FE_CLOSE,NULL))
    return(FALSE);
  CtrlObject->CmdLine->SetString("");

  /* $ 05.03.2002 DJ
     ������� ����� ��������, � �� ������� ���������
  */
  char PluginCommand[4096];
  /* $ 07.09.2000 VVM 1.18
    + ���� ���� PF_FULLCMDLINE - �������� � ���������
  */
  xstrncpy(PluginCommand,Command+(PluginFlags & PF_FULLCMDLINE ? 0:PrefixLength+1), sizeof (PluginCommand)-1);
  /* VVM $ */
  /* DJ $ */
  RemoveTrailingSpaces(PluginCommand);
  HANDLE hPlugin=OpenPlugin(PluginPos,OPEN_COMMANDLINE,(int)PluginCommand);
  if (hPlugin!=INVALID_HANDLE_VALUE)
  {
    Panel *NewPanel=CtrlObject->Cp()->ChangePanel(CurPanel,FILE_PANEL,TRUE,TRUE);
    NewPanel->SetPluginMode(hPlugin,"");
    NewPanel->Update(0);
    NewPanel->Show();
    if(!Target || Target == ActivePanel)
      NewPanel->SetFocus();
  }
  return(TRUE);
}


void PluginsSet::ReadUserBackgound(SaveScreen *SaveScr)
{
  FilePanels *FPanel=CtrlObject->Cp();
  FPanel->LeftPanel->ProcessingPluginCommand++;
  FPanel->RightPanel->ProcessingPluginCommand++;
  if (KeepUserScreen)
  {
    if(SaveScr)
      SaveScr->Discard();
    RedrawDesktop Redraw;
  }
  FPanel->LeftPanel->ProcessingPluginCommand--;
  FPanel->RightPanel->ProcessingPluginCommand--;
}


void CheckScreenLock()
{
  if (ScrBuf.GetLockCount()>0 && !CtrlObject->Macro.PeekKey())
  {
    ScrBuf.SetLockCount(0);
    ScrBuf.Flush();
  }
}

/* $ 27.09.2000 SVS
  ������� CallPlugin - ����� ������ �� ID � ���������
  � ���������� ���������!
*/
int PluginsSet::CallPlugin(DWORD SysID,int OpenFrom, void *Data)
{
  int I;
  if((I=FindPlugin(SysID)) != -1)
  {
    if (PluginsData[I].pOpenPlugin && !ProcessException)
    {
      HANDLE hNewPlugin=OpenPlugin(I,OpenFrom,(int)Data);

      if (hNewPlugin!=INVALID_HANDLE_VALUE &&
         (OpenFrom == OPEN_PLUGINSMENU || OpenFrom == OPEN_FILEPANEL))
      {
        int CurFocus=CtrlObject->Cp()->ActivePanel->GetFocus();
        Panel *NewPanel=CtrlObject->Cp()->ChangePanel(CtrlObject->Cp()->ActivePanel,FILE_PANEL,TRUE,TRUE);
        NewPanel->SetPluginMode(hNewPlugin,"");
        if (Data && *(char *)Data)
          SetDirectory(hNewPlugin,(char *)Data,0);
        /* $ 04.04.2001 SVS
           ��� ���������������! ������� ��������� �������� ������ � CallPlugin()
           ���� ���-�� �� ��� - �����������������!!!
        */
//        NewPanel->Update(0);
        if (CurFocus || !CtrlObject->Cp()->GetAnotherPanel(NewPanel)->IsVisible())
          NewPanel->SetFocus();
//        NewPanel->Show();
        /* SVS $ */
      }
      return TRUE;
    }
  }
  return FALSE;
}

int PluginsSet::FindPlugin(DWORD SysID)
{
  if(SysID != 0 && SysID != 0xFFFFFFFFUl) // �� ����������� 0 � -1
  {
    struct PluginItem *PData=PluginsData;
    for (int I=0;I<PluginsCount;I++,PData++)
      if (PData->SysID == SysID)
        return I;
  }
  return -1;
}
/* SVS $ */

/* $ 23.10.2000 SVS
   ������� TestPluginInfo - �������� �� �������� ���������� �������� ������
*/
#if defined(__BORLANDC__)
#pragma warn -par
#endif
BOOL PluginsSet::TestPluginInfo(struct PluginItem& Item,struct PluginInfo *Info)
{
  if(!Opt.ExceptRules)
    return TRUE;

  char Buf[1];
  int I=FALSE;
  //EXCEPTION_POINTERS *xp;
  TRY {
    if(Info->DiskMenuStringsNumber > 0 && !Info->DiskMenuStrings)
      RaiseException( STATUS_STRUCTWRONGFILLED, 0, 0, 0);
    else for (I=0; I<Info->DiskMenuStringsNumber; I++)
      memcpy(Buf,Info->DiskMenuStrings[I],1);

    if(Info->PluginMenuStringsNumber > 0 && !Info->PluginMenuStrings)
      RaiseException( STATUS_STRUCTWRONGFILLED+1, 0, 0, 0);
    else for (I=0; I<Info->PluginMenuStringsNumber; I++)
     memcpy(Buf,Info->PluginMenuStrings[I],1);

    if(Info->PluginConfigStringsNumber > 0 && !Info->PluginConfigStrings)
      RaiseException( STATUS_STRUCTWRONGFILLED+2, 0, 0, 0);
    else for (I=0; I<Info->PluginConfigStringsNumber; I++)
      memcpy(Buf,Info->PluginConfigStrings[I],1);

    if (Info->CommandPrefix)
      memcpy(Buf,Info->CommandPrefix,1);

    I=TRUE;
  }
  EXCEPT(xfilter(EXCEPT_GETPLUGININFO_DATA,GetExceptionInformation(),&Item,1))
  {
     UnloadPlugin(Item,EXCEPT_GETPLUGININFO_DATA); // ���� �� �������, ��������� ���
     I=FALSE;
//     ProcessException=FALSE;
  }
  return I;
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif
/* SVS $ */

/* $ 31.10.2000 SVS
   ������� TestOpenPluginInfo - �������� �� �������� ���������� �������� ������
*/
#if defined(__BORLANDC__)
#pragma warn -par
#endif
BOOL PluginsSet::TestOpenPluginInfo(struct PluginItem& Item,struct OpenPluginInfo *Info)
{
  if(!Opt.ExceptRules)
    return TRUE;

  char Buf[1];
  int I=FALSE;
  //EXCEPTION_POINTERS *xp;
  TRY {
    if(Info->HostFile) memcpy(Buf,Info->HostFile,1);
    if(Info->CurDir) memcpy(Buf,Info->CurDir,1);
    if(Info->Format) memcpy(Buf,Info->Format,1);
    if(Info->PanelTitle) memcpy(Buf,Info->PanelTitle,1);

    if(Info->InfoLinesNumber > 0 && !Info->InfoLines)
      RaiseException( STATUS_STRUCTWRONGFILLED, 0, 0, 0);
    else for (I=0; I<Info->InfoLinesNumber; I++)
      memcpy(Buf,&Info->InfoLines[I],1);

    if(Info->DescrFilesNumber > 0 && !Info->DescrFiles)
      RaiseException( STATUS_STRUCTWRONGFILLED+1, 0, 0, 0);
    else for (I=0; I<Info->DescrFilesNumber; I++)
      memcpy(Buf,Info->DescrFiles[I],1);

    if(Info->PanelModesNumber > 0 && !Info->PanelModesArray)
      RaiseException( STATUS_STRUCTWRONGFILLED+2, 0, 0, 0);
    for (I=0; I<Info->PanelModesNumber; I++)
      memcpy(Buf,&Info->PanelModesArray[I],1);

    if(Info->KeyBar) memcpy(Buf,Info->KeyBar,1);
    if(Info->ShortcutData) memcpy(Buf,Info->ShortcutData,1);
    I=TRUE;
  }
  EXCEPT(xfilter(EXCEPT_GETOPENPLUGININFO_DATA,GetExceptionInformation(),&Item,1))
  {
     UnloadPlugin(Item,EXCEPT_GETOPENPLUGININFO_DATA); // ���� �� �������, ��������� ���
     I=FALSE;
//     ProcessException=FALSE;
  }
  return I;
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif
/* SVS $ */
