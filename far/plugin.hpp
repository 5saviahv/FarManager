#ifndef __PLUGIN_HPP__
#define __PLUGIN_HPP__
/*
  PLUGIN.HPP

  Plugin API for FAR Manager 1.70

  Copyright (c) 1996-2000 Eugene Roshal
  Copyrigth (c) 2000-2001 [ FAR group ]
*/
/* Revision: 1.103 14.05.2001 $ */

/*
��������!
� ���� ����� ������ ��� ��������� ������ � � ���� �����!!!!

Modify:
  14.05.2001 SVS
    ! FDLG_SMALLDILAOG -> FDLG_SMALLDIALOG
  13.05.2001 SVS
    + DIF_LISTWRAPMODE, DIF_LISTHIGHLIGHT
    + DM_LISTADDSTR
  12.05.2001 DJ
    + VF_ENABLE_F6, EF_ENABLE_F6
  08.05.2001 SVS
    + FDLG_* - ����� ��� DialogEx
  07.05.2001 SVS
    + DM_LISTADD, DM_LISTDELETE, DM_LISTGET, DM_LISTSORT, DM_LISTGETCURPOS,
      DM_LISTSETCURPOS
    + DIF_LISTNOBOX - ����� ��� DI_LISTBOX �� ��������
    + struct FarListDelete
    + ������� DlgList_*, DlgItem_*, DlgEdit_*, Dlg_* - ���� windowsx.h ;-)
      ��� �������� - ������� � ��������� ��������� :-)))
  04.05.2001 SVS
    ! ������� �� ����� ���� �� DI_LISTBOX ;-) - ����� ���� FarDialogItem.ListPos
  24.04.2001 SVS
    + PanelInfo.Flags, ����� PANELINFOFLAGS.
  22.04.2001 SVS
    + EJECT_LOAD_MEDIA - �������� ������ � NT/2000
  12.04.2001 SVS
    + DM_ADDHISTORY - �������� ������ � �������
    + DIF_MANUALADDHISTORY - ��������� � ������� ������ "�������"
  03.04.2001 IS
    + ESPT_AUTOINDENT, ESPT_CURSORBEYONDEOL, ESPT_CHARCODEBASE
  26.03.2001 SVS
    + FHELP_USECONTENTS - ���� �� ������ ������� �����, �� ���������� "Contents"
  24.03.2001 tran
    + qsortex
  21.03.2001 VVM
    + ���� EF_CREATENEW ��� ��������� - ������� ����� ���� (������ SHIFT+F4)
  20.03.2001 tran 1.89
    + FarRecursiveSearch - �������� void *param
  19.03.2001 SVS
    ! DN_CLOSE=DM_CLOSE, DN_KEY=DM_KEY - ��� �����������. :-)
  16.02.2001 IS
    + ��������� �������� ������������ ������������ �� ������ ����������
      ������� PluginPanelItem - �� ������ ���� ������ 366. ���� ��� �� ���, ��
      ���� ���������� STRICT, �� �������������� ������ �����������, ����� -
      ����� ����� warning
  16.02.2001 IS
    + ������� ECTL_SETPARAM - �������� ����� ��������� ���������
    + EDITOR_SETPARAMETER_TYPES - ��� ���������
    + ��������� EditorSetParameter - ���������� � ���� ��������� � ���
      ���������
  13.02.2001 SVS
    ! � ����� � ��������� DIF_VAREDIT ��� DI_COMBOBOX �������� ���������
      FarListItem � �������� ���� LIF_PTRDATA
    ! �������� �������� ����� LIF_DISABLE
    ! �������� ����� ����� ��������� FarDialogItemData - ��� "���������"
      �� ������.
  11.02.2001 SVS
    ! FarDialogItem - ���������, �������� Ptr
    + DIF_VAREDIT - ����, ����������� �� ��, ��� ����� ��������������
      FarDialogItem.Ptr ������ FarDialogItem.Data
  11.02.2001 SVS
    ! ��������� � LISTITEMFLAGS - ����� ��������� � ������� �����
  28.01.2001 SVS
    ! SequenceKey.Sequence ��! "��������" VK_* - ������ KEY_*
    + FMSG_ALLINONE - � �������� Items ���������� ��������� ��
      ������, � ������� ����������� ����� - ������ '\n'
    + FMSG_MB_* - ������������� �������� ������ (� Items ����� �� ���������)
  25.01.2001 SVS
    ! ��� SequenceKey.Sequence ������� �� DWORD
    + SKEY_VK_KEYS - � SequenceKey.Sequence "��������" VK_* ������ KEY_*
  23.01.2001 SVS
    + SKEY_NOTMACROS - �� ������������ ��������� ������� � SequenceKey
    + ViewerInfo.LeftPos � ViewerInfo.Reserved3;
  21.01.2001 SVS
    + struct SequenceKey
    + ACTL_PROCESSSEQUENCEKEY
  21.01.2001 IS
    ! ��� ����������� � ���������� ������� ���� ��������:
      VCTL_SETPOS -> VCTL_SETPOSITION
      AnsiText -> AnsiMode
  19.01.2001 SVS
    ! ������������ � �������� VIEWER_CONTROL_COMMANDS
    + ��������� ��������� ��� Viewer API: ViewerSelect, ViewerSetPosition �
      ������������ VIEWER_SETPOS_FLAGS
  03.01.2001 SVS
    + DIF_HIDDEN - ������� �� �����
    + DM_SHOWITEM ��������/������ �������
  25.12.2000 SVS
    ! ACTL_KEYMACRO ������������ ������ 2 �������: MCMD_LOADALL, MCMD_SAVEALL
  23.12.2000 SVS
    + MCMD_PLAYSTRING - "���������" ������.
    + MACRO_* - ������� �������� ��������
    ! ActlKeyMacro - ��������� ����������� ��������� ��� ������� MCMD_PLAYSTRING
    + MFLAGS_ - ����� �������
  21.12.2000 SVS
    + ACTL_KEYMACRO
    + ��������� ActlKeyMacro (� ������������������ ������ :-)
    + MacroCommand: MCMD_LOADALL, MCMD_SAVEALL (�� ���� ���� �����������,
      ��������� ����� ������)
  21.12.2000 SVS
    + DM_GETTEXTPTR, DM_SETTEXTPTR
  18.12.2000 SVS
    + FHELP_NOSHOWERROR
  14.12.2000 SVS
    + ACTL_EJECTMEDIA & struct ActlEjectMedia & EJECT_NO_MESSAGE
  08.12.2000 SVS 1.70
    ! ������������ ����� ������� ��������� - 1.70 ;-) - ����������.
      ��� DM_SETTEXT, DM_GETTEXT � Param2 ���������� ���������
      FarDialogItemData.
  07.12.2000 SVS
    ! �������� ��������� FARMANAGERVERSION. ��������� ������� �
      DIFF.DOC\00300.FAR_VERSION.txt
  04.12.2000 SVS
    + DIF_3STATE - 3-� ��������� CheckBox
    + ACTL_GETCOLOR - �������� ������������ ����
    + ACTL_GETARRAYCOLOR - �������� ���� ������ ������
  04.11.2000 SVS
    + XLAT_SWITCHKEYBBEEP - ������ �������� ������ ��� ������������
      ����������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  26.10.2000 SVS
    ! DM_SETEDITPOS/DM_GETEDITPOS -> DM_SETCURSORPOS/DM_GETCURSORPOS
  25.10.2000 IS
    + ������� ��� ��������� � MkTemp � Template �� Prefix
  23.10.2000 SVS
    + DM_SETEDITPOS, DM_GETEDITPOS -
      ���������������� ������� � ������� ��������������.
  20.10.2000 SVS
    ! ProcessName: Flags ������ ���� DWORD, � �� int
  20.10.2000 SVS
    + DM_GETFOCUS - �������� ID �������� �������� ����� �����
  09.10.2000 IS
    + ����� ��� ProcessName (PN_*)
    + ��������� � FARSTANDARDFUNCTIONS �� ProcessName;
  27.09.2000 SVS
    + VCTL_QUIT      - ������� ������
    + VCTL_GETINFO   - ��������� ���������� � Viewer
    + VCTL_SETKEYBAR - ������� ��������� KeyBar Labels �� �������
  27.09.2000 skv
    + DeleteBuffer
  26.09.2000 SVS
    ! FARSTDKEYTOTEXT -> FARSTDKEYTOKEYNAME
  24.09.2000 SVS
    ! ������ ����� �� ������������ - ������ ������ � ���� ����� (Modify)!!!
    ! FarKeyToText -> FarKeyToName
    + FarNameToKey
  21.09.2000 SVS
    + OPEN_FILEPANEL ������ �� �������� ������.
    + ���� PluginInfo.SysID - ��������� ������������� �������
  20.09.2000 SVS
    ! ������ FolderPresent (����, ������ ����� ������� :-(
  19.09.2000 SVS
    + ������������ �� 2 �����
    + ������� FSF.FolderPresent - "���������� �� �������"
  18.09.2000 SVS
    + DIF_READONLY - ���� ��� ����� ��������������
      (����! ��� ����� ��������������).
  18.09.2000 SVS
    ! ������� DialogEx ����� 2 �������������� ��������� (Future)
    ! ��������� � struct PluginStartupInfo!!!!
    ! FarRecurseSearch -> FarRecursiveSearch
    ! FRS_RECURSE -> FRS_RECUR
  14.09.2000 SVS
    ! ������ � �������� XLAT_SWITCHKEYBLAYOUT.
    + FSF.MkTemp
    + ���� DIF_LISTNOAMPERSAND. �� ��������� ��� DI_LISTBOX
      ������������ ���� MENU_SHOWAMPERSAND. ���� ���� ��������� �����
      ����������
  13.09.2000 skv
    + EEREDRAW_XXXXX defines
  12.09.2000 SVS
    + ����� FHELP_* ��� ������� ShowHelp
    ! FSF.ShowHelp ���������� BOOL
  10.09.2000 SVS
    ! KeyToText ���������� BOOL, ���� ��� ����� �������.
  10.09.2000 SVS 1.46
    + typedef struct _CHAR_INFO    CHAR_INFO;
      �� ��� ������, ���� wincon.h �� ��� ��������.
  10.09.2000 tran 1.45
    + FSF/FarRecurseSearch
  10.09.2000 SVS 1.44
    ! �������-�� ������� ���������� ��� ��� QWERTY -> Xlat.
    + DIF_NOFOCUS - ������� �� �������� ������ ����� (�����������)
    + CHAR_INFO *VBuf; � ��������� �������
    + DIF_SELECTONENTRY - ��������� Edit ��� ��������� ������
  08.09.2000 VVM
    + FCTL_SETSORTMODE, FCTL_SETANOTHERSORTMODE
      FCTL_SETSORTORDER, FCTL_SETANOTHERSORTORDER
      ����� ���������� �� ������
  08.09.2000 SVS
    ! QWERTY -> Transliterate
    ! QWED_SWITCHKEYBLAYER -> EDTR_SWITCHKEYBLAYER
  08.09.2000 SVS
    + FARMANAGERVERSION
    ! FarStandardFunctions.Reserved* -> FarStandardFunctions.Reserved[10];
  07.09.2000 skv
    + ECTL_PROCESSKEY
  07.09.2000 VVM 1.39
    + PF_FULLCMDLINE ���� ��� �������� ������� ���� ������ ������ �
      ���������
  07.09.2000 SVS 1.38
    + FSF.bsearch
    + FSF.GetFileOwner
    + FSF.GetNumberOfLinks;
  05.09.2000 SVS 1.37
    + QWERTY - �������������� - StandardFunctions.EDQwerty
  01.09.2000 SVS
    + ����������� (� ������ MY)
      #ifndef _WINCON_
      typedef struct _INPUT_RECORD INPUT_RECORD;
      #endif
  31.08.2000 tran 1.35
    + FSF: int FarInputRecordToKey(INPUT_RECORD*r);
  31.08.2000 SVS
    ! ��������� FSF-�������
      FSF.RemoveLeadingSpaces =FSF.LTrim
      FSF.RemoveTrailingSpaces=FSF.RTrim
      FSF.RemoveExternalSpaces=FSF.Trim
    + DM_ENABLE
    + ���� DIF_DISABLE ����������� ������� ������� � ��������� Disable
    + ���� LIF_DISABLE ����������� ������� ������ � ��������� Disable
  30.08.2000 SVS
    ! ��� ������� ������� ���� FMI_GETFARMSGID
    + DM_MOVEDIALOG - ����������� ������.
  29.08.2000 SVS
    ! ��� � ������ ����� � unsigned char �� ���������� ��������� DialogItem,
      � ��-�� ����� uchar ������� DI_USERCONTROL �� ����� ���� > 255 :-((((((
  29.08.2000 SVS
    + ������ ����� ��������� "�����" �� FAR*.LNG, ��� �����
      ��������� � MsgId (� ������� GetMsg)�������� ���� FMI_GETFARMSGID
  28.08.2000 SVS
    + SFS-������� ��� Local*
    ! ��������� ��� FARSTDQSORT - ����� �������� __cdecl ��� ������� ���������
    ! �� FarStandardFunctions._atoi64, �� FarStandardFunctions.atoi64
    + FARSTDITOA64
  25.08.2000 SVS
    + DM_GETDLGRECT - �������� ���������� ����������� ����
    + DM_USER - �� ��� ���������� ������� :-)
  25.08.2000 SVS
    ! ������� �� FSF �������:
      memset, memcpy, memmove, memcmp,
      strchr, strrchr, strstr, strtok, strpbrk
    + ���� FIB_BUTTONS - � ������� InputBox ���� ����� - ����������
      ������ <Ok> & <Cancel>
  24.08.2000 SVS
    + ACTL_WAITKEY - ������� ������������ (��� �����) �������
    + ������� DI_USERCONTROL - ���������� ���������� ������.
  23.08.2000 SVS
    ! ��������� ��������� DMSG_* -> DM_ (�����) & DN_ (������)
    + DM_KEY        - �������/�������� �������(�)
    + DM_GETDLGDATA - ����� ������ �������.
    + DM_SETDLGDATA - ���������� ������ �������.
    + DM_SHOWDIALOG - ��������/�������� ������
    ! ��� Flags ��������� � ������ ���� -> DWORD.
      ��������������:
        * �������   FarMenuFn, FarMessageFn, FarShowHelp
        * ��������� FarListItem, FarDialogItem
  22.08.2000 SVS
    ! DMSG_PAINT -> DMSG_DRAWDIALOG
    ! DMSG_DRAWITEM -> DMSG_DRAWDLGITEM
    ! DMSG_CHANGELIST -> DMSG_LISTCHANGE
  21.08.2000 SVS 1.23
    ! DMSG_CHANGEITEM -> DMSG_EDITCHANGE
    + DMSG_BTNCLICK
  18.08.2000 tran
    + Flags in ShowHelp
  12.08.2000 KM 1.22
    + DIF_MASKEDIT - ����� ����, ����������� ���������������� �����
      �� ����� � ������� �����.
    ! � ��������� FarDialogItem ����� ����, ���������� � union, char *Mask
  17.08.2000 SVS
    ! struct FarListItems -> struct FarList, � �� ������ ��������� :-)
    + ��������� �������: DMSG_ENABLEREDRAW, DMSG_MOUSECLICK,
    + ���� ��� DI_BUTTON - DIF_BTNNOCLOSE - "������ �� ��� �������� �������"
  17.08.2000 SVS
    ! ��������� ������ ������ :-)
  09.08.2000 SVS
    + FIB_NOUSELASTHISTORY - ���� ��� ������������� ���� �������� ��
      ������� �������� ��������!!!
  09.08.2000 tran
    + #define CONSOLE_*
  04.08.2000 SVS
    + ECTL_SETKEYBAR - ������� ��������� KeyBar Labels � ���������
  04.08.2000 SVS
    + FarListItems.CountItems -> FarListItems.ItemsNumber
  03.08.2000 SVS
    + ������� �� AT: GetMinFarVersion
  03.08.2000 SVS
    + ACTL_GETSYSWORDDIV �������� ������ � ��������� ������������� ����
  02.08.2000 SVS
    + ���������� ��� KeyBarTitles:
        CtrlShiftTitles
        AltShiftTitles,
        CtrlAltTitles
    + ������� � OpenPluginInfo ��� ����, ����� ��������� FAR <= 1.65 � > 1.65
  01.08.2000 SVS
    ! ������� ����� ������ ����� ���� �������� ��� ���� ������
    ! �������������� ��������� � KeyToText - ������ ������
    + ���� DIF_USELASTHISTORY ��� ����� �����.
      ���� � ������ ����� ���� ������� �� ��������� �������� ����� ������
      �� �������
    ! ������ ��������� ��������� ������ � "��������" ������ ������
    + ����� ��� FarListItem.Flags
      LIF_SELECTED, LIF_CHECKED, LIF_SEPARATOR
    + ��������� ��� ��������� �������, �������� ����� ���� :-)
      DMSG_SETDLGITEM, DMSG_CHANGELIST
    ! �������� ������������ ���� ������� ����������� �� �������������
      FARDIALOGPROC -> FARWINDOWPROC
  28.07.2000 SVS
    + ������ ����� ������� DI_LISTBOX (��������������� �����)
    + ��������� ��� ��������� �������, �������� ����� ���� :-)
        DMSG_INITDIALOG, DMSG_ENTERIDLE, DMSG_HELP, DMSG_PAINT,
        DMSG_SETREDRAW, DMSG_DRAWITEM, DMSG_GETDLGITEM, DMSG_KILLFOCUS,
        DMSG_GOTFOCUS, DMSG_SETFOCUS, DMSG_GETTEXTLENGTH, DMSG_GETTEXT,
        DMSG_CTLCOLORDIALOG, DMSG_CTLCOLORDLGITEM, DMSG_CTLCOLORDLGLIST,
        DMSG_SETTEXTLENGTH, DMSG_SETTEXT, DMSG_CHANGEITEM, DMSG_HOTKEY,
        DMSG_CLOSE,
  25.07.2000 SVS
    ! ��������� ������������ � FarStandardFunctions
    + ���������� ������������ FulScreen <-> Windowed (ACTL_CONSOLEMODE)
    + FSF-������� KeyToText
    ! WINAPI ��� ��������� �������������� �������
    + �������-������ ����� �������� ������ InputBox
  23.07.2000 SVS
    + DialogEx, SendDlgMessage, DefDlgProc,
    ! WINAPI ��� ��������� �������������� �������
  18.07.2000 SVS
    + ������ ����� �������: DI_COMBOBOX � ���� DIF_DROPDOWNLIST
      (��� ���������������� DI_COMBOBOX - ���� �� �����������!)
  12.07.2000 IS
    + �����  ���������:
      EF_NONMODAL - �������� ������������ ���������
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  10.07.2000 IS
    ! ��������� ��������� � ������ ������ C (�� ������ SVS)
  07.07.2000 IS
    + ��������� �� ������� � FarStandardFunctions:
      atoi, _atoi64, itoa, RemoveLeadingSpaces, RemoveTrailingSpaces,
      RemoveExternalSpaces, TruncStr, TruncPathStr, QuoteSpaceOnly,
      PointToName, GetPathRoot, AddEndSlash
  06.07.2000 IS
    + ������� AdvControl (PluginStartupInfo)
    + ������� ACTL_GETFARVERSION ��� AdvControl
    + ��������� �� ��������� FarStandardFunctions � PluginStartupInfo - ���
      �������� ��������� �� �������� �������. ������ ������ �����������
      ����������� �� ����, ���� ����� ������������ � ����������.
    + ��������� �� ������� � FarStandardFunctions:
      Unquote, ExpandEnvironmentStr,
      sprintf, sscanf, qsort, memcpy, memmove, memcmp, strchr, strrchr, strstr,
      strtok, memset, strpbrk
  05.06.2000 SVS
    + DI_EDIT ����� ���� DIF_EDITEXPAND - ���������� ���������� �����
      � enum FarDialogItemFlags
  03.07.2000 IS
    + ������� ������ ������ � api
  28.06.2000 SVS
    + ��� MSVC ���� ��������� extern "C" ��� ����������
      �������������� ������� + ��������� �� Borland C++ 5.5
  26.06.2000 SVS
    ! ���������� Master Copy
*/

#define FARMANAGERVERSION 0x012C0146UL

#if defined(__BORLANDC__)
  #pragma option -a2
#elif defined(__GNUC__) || (defined(__WATCOMC__) && (__WATCOMC__ < 1100)) || defined(__LCC__)
  #pragma pack(2)
  #if defined(__LCC__)
    #define _export __declspec(dllexport)
  #endif
#else
  #pragma pack(push,2)
  #if _MSC_VER
    #define _export
  #endif
#endif

#define NM 260

#ifndef _WINCON_
typedef struct _INPUT_RECORD INPUT_RECORD;
typedef struct _CHAR_INFO    CHAR_INFO;
#endif

struct PluginPanelItem
{
  WIN32_FIND_DATA FindData;
  DWORD           PackSizeHigh;
  DWORD           PackSize;
  DWORD           Flags;
  DWORD           NumberOfLinks;
  char           *Description;
  char           *Owner;
  char          **CustomColumnData;
  int             CustomColumnNumber;
  DWORD           UserData;
  DWORD           Reserved[3];
};

#if defined(__BORLANDC__)
#if sizeof(PluginPanelItem) != 366
#if defined(STRICT)
#error Incorrect alignment: sizeof(PluginPanelItem)!=366
#else
#pragma message Incorrect alignment: sizeof(PluginPanelItem)!=366
#endif
#endif
#endif

#define PPIF_PROCESSDESCR 0x80000000
#define PPIF_SELECTED     0x40000000
#define PPIF_USERDATA     0x20000000

enum {
  FMENU_SHOWAMPERSAND=1,
  FMENU_WRAPMODE=2,
  FMENU_AUTOHIGHLIGHT=4,
  FMENU_REVERSEAUTOHIGHLIGHT=8
};

enum {
 PN_CMPNAME=0,
 PN_CMPNAMELIST=0x1000UL,
 PN_GENERATENAME=0x2000UL,
 PN_SKIPPATH=0x100000UL
};

typedef int (WINAPI *FARAPIMENU)(
  int                 PluginNumber,
  int                 X,
  int                 Y,
  int                 MaxHeight,
  DWORD               Flags,
  char               *Title,
  char               *Bottom,
  char               *HelpTopic,
  int                *BreakKeys,
  int                *BreakCode,
  struct FarMenuItem *Item,
  int                 ItemsNumber
);

typedef long (WINAPI *FARWINDOWPROC)(
  HANDLE hDlg,
  int    Msg,
  int    Param1,
  long   Param2
);

typedef long (WINAPI *FARAPISENDDLGMESSAGE)(
  HANDLE hDlg,
  int    Msg,
  int    Param1,
  long   Param2
);

typedef long (WINAPI *FARAPIDEFDLGPROC)(
  HANDLE hDlg,
  int    Msg,
  int    Param1,
  long   Param2
);

typedef int (WINAPI *FARAPIDIALOG)(
  int                   PluginNumber,
  int                   X1,
  int                   Y1,
  int                   X2,
  int                   Y2,
  char                 *HelpTopic,
  struct FarDialogItem *Item,
  int                   ItemsNumber
);

typedef int (WINAPI *FARAPIDIALOGEX)(
  int                   PluginNumber,
  int                   X1,
  int                   Y1,
  int                   X2,
  int                   Y2,
  char                 *HelpTopic,
  struct FarDialogItem *Item,
  int                   ItemsNumber,
  DWORD                 Reserved,
  DWORD                 Flags,
  FARWINDOWPROC         DlgProc,
  long                  Param
);

enum {
  FDLG_WARNING             =0x00000001,
  FDLG_SMALLDIALOG         =0x00000002,
};

enum {
  FMSG_WARNING             =0x00000001,
  FMSG_ERRORTYPE           =0x00000002,
  FMSG_KEEPBACKGROUND      =0x00000004,
  FMSG_DOWN                =0x00000008,
  FMSG_LEFTALIGN           =0x00000010,

  FMSG_ALLINONE            =0x00000020,

  FMSG_MB_OK               =0x00010000,
  FMSG_MB_OKCANCEL         =0x00020000,
  FMSG_MB_ABORTRETRYIGNORE =0x00030000,
  FMSG_MB_YESNO            =0x00040000,
  FMSG_MB_YESNOCANCEL      =0x00050000,
  FMSG_MB_RETRYCANCEL      =0x00060000,
};

typedef int (WINAPI *FARAPIMESSAGE)(
  int PluginNumber,
  DWORD Flags,
  char *HelpTopic,
  char **Items,
  int ItemsNumber,
  int ButtonsNumber
);


typedef char* (WINAPI *FARAPIGETMSG)(
  int PluginNumber,
  int MsgId
);

enum DialogItemTypes {
  DI_TEXT,
  DI_VTEXT,
  DI_SINGLEBOX,
  DI_DOUBLEBOX,
  DI_EDIT,
  DI_PSWEDIT,
  DI_FIXEDIT,
  DI_BUTTON,
  DI_CHECKBOX,
  DI_RADIOBUTTON,
  DI_COMBOBOX,
  DI_LISTBOX,

  DI_USERCONTROL=255,
};

enum FarDialogItemFlags {
  DIF_COLORMASK       =0x000000ffUL,
  DIF_SETCOLOR        =0x00000100UL,
  DIF_BOXCOLOR        =0x00000200UL,
  DIF_GROUP           =0x00000400UL,
  DIF_LEFTTEXT        =0x00000800UL,
  DIF_MOVESELECT      =0x00001000UL,
  DIF_SHOWAMPERSAND   =0x00002000UL,
  DIF_CENTERGROUP     =0x00004000UL,
  DIF_NOBRACKETS      =0x00008000UL,
  DIF_MANUALADDHISTORY=0x00008000UL,
  DIF_SEPARATOR       =0x00010000UL,
  DIF_VAREDIT         =0x00010000UL,
  DIF_EDITOR          =0x00020000UL,
  DIF_LISTNOAMPERSAND =0x00020000UL,
  DIF_LISTNOBOX       =0x00040000UL,
  DIF_HISTORY         =0x00040000UL,
  DIF_BTNNOCLOSE      =0x00040000UL,
  DIF_EDITEXPAND      =0x00080000UL,
  DIF_DROPDOWNLIST    =0x00100000UL,
  DIF_USELASTHISTORY  =0x00200000UL,
  DIF_MASKEDIT        =0x00400000UL,
  DIF_SELECTONENTRY   =0x00800000UL,
  DIF_3STATE          =0x00800000UL,
  DIF_LISTWRAPMODE    =0x01000000UL,
  DIF_LISTHIGHLIGHT   =0x02000000UL,
  DIF_HIDDEN          =0x10000000UL,
  DIF_READONLY        =0x20000000UL,
  DIF_NOFOCUS         =0x40000000UL,
  DIF_DISABLE         =0x80000000UL,
};

enum FarMessagesProc{
  DM_FIRST=0,
  DM_CLOSE,
  DM_ENABLE,
  DM_ENABLEREDRAW,
  DM_GETDLGDATA,
  DM_GETDLGITEM,
  DM_GETDLGRECT,
  DM_GETTEXT,
  DM_GETTEXTLENGTH,
  DM_KEY,
  DM_MOVEDIALOG,
  DM_SETDLGDATA,
  DM_SETDLGITEM,
  DM_SETFOCUS,
  DM_SETREDRAW,
  DM_SETTEXT,
  DM_SETTEXTLENGTH,
  DM_SHOWDIALOG,
  DM_GETFOCUS,
  DM_GETCURSORPOS,
  DM_SETCURSORPOS,
  DM_GETTEXTPTR,
  DM_SETTEXTPTR,
  DM_SHOWITEM,
  DM_ADDHISTORY,

  DM_LISTSORT,
  DM_LISTGET,
  DM_LISTGETCURPOS,
  DM_LISTSETCURPOS,
  DM_LISTDELETE,
  DM_LISTADD,
  DM_LISTADDSTR,

  DN_FIRST=0x1000,
  DN_BTNCLICK,
  DN_CTLCOLORDIALOG,
  DN_CTLCOLORDLGITEM,
  DN_CTLCOLORDLGLIST,
  DN_DRAWDIALOG,
  DN_DRAWDLGITEM,
  DN_EDITCHANGE,
  DN_ENTERIDLE,
  DN_GOTFOCUS,
  DN_HELP,
  DN_HOTKEY,
  DN_INITDIALOG,
  DN_KILLFOCUS,
  DN_LISTCHANGE,
  DN_MOUSECLICK,


  DN_CLOSE=DM_CLOSE,
  DN_KEY=DM_KEY,

  DM_USER=0x4000,
};

enum LISTITEMFLAGS{
  LIF_SELECTED = 0x00010000UL,
  LIF_CHECKED  = 0x00020000UL,
  LIF_SEPARATOR= 0x00040000UL,
  LIF_DISABLE  = 0x00080000UL,
  LIF_PTRDATA  = 0x00100000UL,
};

struct FarListItem
{
  DWORD Flags;
  union {
    char Text[124];
    struct {
      DWORD PtrFlags;
      int   PtrLength;
      char *PtrData;
      char  PtrTail[1];
    } Ptr;
  };
};

struct FarList
{
  int ItemsNumber;
  struct FarListItem *Items;
};

struct FarListDelete
{
  int StartIndex;
  int Count;
};

struct FarDialogItem
{
  int Type;
  int X1,Y1,X2,Y2;
  int Focus;
  union {
    int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
    CHAR_INFO *VBuf;
  };
  DWORD Flags;
  int DefaultButton;
  union {
    char Data[512];
    int  ListPos;
    struct {
      DWORD PtrFlags;
      int   PtrLength;
      char *PtrData;
      char  PtrTail[1];
    } Ptr;
  };
};

struct FarDialogItemData
{
  int   PtrLength;
  char *PtrData;
};

#define Dlg_GetDlgData(Info,hDlg)              Info.SendDlgMessage(hDlg,DM_GETDLGDATA,0,0)
#define Dlg_SetDlgData(Info,hDlg,Data)         Info.SendDlgMessage(hDlg,DM_SETDLGDATA,0,(long)Data)

#define DlgItem_GetFocus(Info,hDlg)            Info.SendDlgMessage(hDlg,DM_GETFOCUS,0,0)
#define DlgItem_SetFocus(Info,hDlg,ID)         Info.SendDlgMessage(hDlg,DM_SETFOCUS,ID,0)
#define DlgItem_Enable(Info,hDlg,ID)           Info.SendDlgMessage(hDlg,DM_ENABLE,ID,TRUE)
#define DlgItem_Disable(Info,hDlg,ID)          Info.SendDlgMessage(hDlg,DM_ENABLE,ID,FALSE)
#define DlgItem_IsEnable(Info,hDlg,ID)         Info.SendDlgMessage(hDlg,DM_ENABLE,ID,-1)
#define DlgItem_SetText(Info,hDlg,ID,Str)      Info.SendDlgMessage(hDlg,DM_SETTEXTPTR,ID,(long)Str)

#define DlgEdit_AddHistory(Info,hDlg,ID,Str)   Info.SendDlgMessage(hDlg,DM_ADDHISTORY,ID,(long)Str)

#define DlgList_AddString(Info,hDlg,ID,Str)    Info.SendDlgMessage(hDlg,DM_LISTADDSTR,ID,(long)Str)
#define DlgList_GetCurPos(Info,hDlg,ID)        Info.SendDlgMessage(hDlg,DM_LISTGETCURPOS,ID,0)
#define DlgList_SetCurPos(Info,hDlg,ID,NewPos) Info.SendDlgMessage(hDlg,DM_LISTSETCURPOS,ID,NewPos)
#define DlgList_ClearList(Info,hDlg,ID)        Info.SendDlgMessage(hDlg,DM_LISTDELETE,ID,0)
#define DlgList_DeleteItem(Info,hDlg,ID,Index) {struct FarListDelete FLDItem={Index,1}; Info.SendDlgMessage(hDlg,DM_LISTDELETE,ID,(long)&FLDItem);}
#define DlgList_SortUp(Info,hDlg,ID)           Info.SendDlgMessage(hDlg,DM_LISTSORT,ID,0)
#define DlgList_SortDown(Info,hDlg,ID)         Info.SendDlgMessage(hDlg,DM_LISTSORT,ID,1)


struct FarMenuItem
{
  char Text[128];
  int Selected;
  int Checked;
  int Separator;
};


enum {FCTL_CLOSEPLUGIN,FCTL_GETPANELINFO,FCTL_GETANOTHERPANELINFO,
      FCTL_UPDATEPANEL,FCTL_UPDATEANOTHERPANEL,
      FCTL_REDRAWPANEL,FCTL_REDRAWANOTHERPANEL,
      FCTL_SETANOTHERPANELDIR,FCTL_GETCMDLINE,FCTL_SETCMDLINE,
      FCTL_SETSELECTION,FCTL_SETANOTHERSELECTION,
      FCTL_SETVIEWMODE,FCTL_SETANOTHERVIEWMODE,FCTL_INSERTCMDLINE,
      FCTL_SETUSERSCREEN,FCTL_SETPANELDIR,FCTL_SETCMDLINEPOS,
      FCTL_GETCMDLINEPOS,
      FCTL_SETSORTMODE,FCTL_SETANOTHERSORTMODE,
      FCTL_SETSORTORDER,FCTL_SETANOTHERSORTORDER
};

enum {PTYPE_FILEPANEL,PTYPE_TREEPANEL,PTYPE_QVIEWPANEL,PTYPE_INFOPANEL};

enum PANELINFOFLAGS {
  PFLAGS_SHOWHIDDEN           =0x00000001,
  PFLAGS_HIGHLIGHT            =0x00000002,
  PFLAGS_AUTOCHANGEFOLDER     =0x00000004,
  PFLAGS_SELECTFOLDERS        =0x00000008,
  PFLAGS_ALLOWREVERSESORT     =0x00000010,
  PFLAGS_REVERSESORTORDER     =0x00000020,
  PFLAGS_USESORTGROUPS        =0x00000040,
  PFLAGS_SELECTEDFIRST        =0x00000080,
};

struct PanelInfo
{
  int PanelType;
  int Plugin;
  RECT PanelRect;
  struct PluginPanelItem *PanelItems;
  int ItemsNumber;
  struct PluginPanelItem *SelectedItems;
  int SelectedItemsNumber;
  int CurrentItem;
  int TopPanelItem;
  int Visible;
  int Focus;
  int ViewMode;
  char ColumnTypes[80];
  char ColumnWidths[80];
  char CurDir[NM];
  int ShortNames;
  int SortMode;
  DWORD Flags;
  DWORD Reserved;
};


struct PanelRedrawInfo
{
  int CurrentItem;
  int TopPanelItem;
};


typedef int (WINAPI *FARAPICONTROL)(
  HANDLE hPlugin,
  int Command,
  void *Param
);

typedef HANDLE (WINAPI *FARAPISAVESCREEN)(int X1,int Y1,int X2,int Y2);

typedef void (WINAPI *FARAPIRESTORESCREEN)(HANDLE hScreen);

typedef int (WINAPI *FARAPIGETDIRLIST)(
  char *Dir,
  struct PluginPanelItem **pPanelItem,
  int *pItemsNumber
);

typedef int (WINAPI *FARAPIGETPLUGINDIRLIST)(
  int PluginNumber,
  HANDLE hPlugin,
  char *Dir,
  struct PluginPanelItem **pPanelItem,
  int *pItemsNumber
);

typedef void (WINAPI *FARAPIFREEDIRLIST)(struct PluginPanelItem *PanelItem);

enum VIEWER_FLAGS {
  VF_NONMODAL      =1,
  VF_DELETEONCLOSE =2,
  VF_ENABLE_F6     =4,
};

enum EDITOR_FLAGS {
  EF_NONMODAL      =1,
  EF_CREATENEW     =2,
  EF_ENABLE_F6     =4,
};

typedef int (WINAPI *FARAPIVIEWER)(
  char *FileName,
  char *Title,
  int X1,
  int Y1,
  int X2,
  int Y2,
  DWORD Flags
);

typedef int (WINAPI *FARAPIEDITOR)(
  char *FileName,
  char *Title,
  int X1,
  int Y1,
  int X2,
  int Y2,
  DWORD Flags,
  int StartLine,
  int StartChar
);

typedef int (WINAPI *FARAPICMPNAME)(
  char *Pattern,
  char *String,
  int SkipPath
);


#define FCT_DETECT 0x40000000

struct CharTableSet
{
  char DecodeTable[256];
  char EncodeTable[256];
  char UpperTable[256];
  char LowerTable[256];
  char TableName[128];
};

typedef int (WINAPI *FARAPICHARTABLE)(
  int Command,
  char *Buffer,
  int BufferSize
);

typedef void (WINAPI *FARAPITEXT)(
  int X,
  int Y,
  int Color,
  char *Str
);


typedef int (WINAPI *FARAPIEDITORCONTROL)(
  int Command,
  void *Param
);

enum FarHelpFlags{
  FHELP_NOSHOWERROR =0x80000000,
  FHELP_SELFHELP    =0x00000000,
  FHELP_FARHELP     =0x00000001,
  FHELP_CUSTOMFILE  =0x00000002,
  FHELP_CUSTOMPATH  =0x00000004,
  FHELP_USECONTENTS =0x40000000,
};

typedef BOOL (WINAPI *FARAPISHOWHELP)(char *ModuleName,char *Topic,DWORD Flags);

enum {
  SKEY_NOTMACROS  = 0x00000001,
};

struct SequenceKey{
  DWORD Flags;
  int Count;
  DWORD *Sequence;
};

enum {
  ACTL_GETFARVERSION,
  ACTL_CONSOLEMODE,
  ACTL_GETSYSWORDDIV,
  ACTL_WAITKEY,
  ACTL_GETCOLOR,
  ACTL_GETARRAYCOLOR,
  ACTL_EJECTMEDIA,
  ACTL_KEYMACRO,
  ACTL_PROCESSSEQUENCEKEY,
};

#define CONSOLE_GET_MODE       (-2)
#define CONSOLE_TRIGGER        (-1)
#define CONSOLE_SET_WINDOWED   (0)
#define CONSOLE_SET_FULLSCREEN (1)
#define CONSOLE_WINDOWED       (0)
#define CONSOLE_FULLSCREEN     (1)

#define EJECT_NO_MESSAGE       0x00000001
#define EJECT_LOAD_MEDIA       0x00000002

struct ActlEjectMedia {
  DWORD Letter;
  DWORD Flags;
};


enum MacroCommand{
  MCMD_LOADALL, MCMD_SAVEALL
};

struct ActlKeyMacro{
  int Command;
  DWORD Reserved[3];
};


typedef int (WINAPI *FARAPIADVCONTROL)(
  int ModuleNumber,
  int Command,
  void *Param
);


enum VIEWER_CONTROL_COMMANDS {
  VCTL_GETINFO,
  VCTL_QUIT,
  VCTL_REDRAW,
  VCTL_SETKEYBAR,
  VCTL_SETPOSITION,
  VCTL_SELECT,
};

enum VIEWER_OPTIONS {
  VOPT_SAVEFILEPOSITION=1,
  VOPT_AUTODETECTTABLE=2,
};

struct ViewerSelect
{
   long  BlockStartPos;
   long  Reserved1;
   int   BlockLen;
};

enum VIEWER_SETPOS_FLAGS {
  VSP_NOREDRAW    = 0x0001,
  VSP_PERCENT     = 0x0002,
  VSP_RELATIVE    = 0x0004,
  VSP_NORETNEWPOS = 0x0008,
};

struct ViewerSetPosition
{
  DWORD Flags;
  long  StartPos;
  long  Reserved1;
  int   LeftPos;
};

struct ViewerMode{
  int UseDecodeTable;
  int TableNum;
  int AnsiMode;
  int Unicode;
  int Wrap;
  int TypeWrap;
  int Hex;
  DWORD Reserved[4];
};

struct ViewerInfo
{
  int    StructSize;
  int    ViewerID;
  char  *FileName;
  DWORD  FileSize;
  DWORD  Reserved1;
  DWORD  FilePos;
  DWORD  Reserved2;
  int    WindowSizeX;
  int    WindowSizeY;
  DWORD  Options;
  int    TabSize;
  struct ViewerMode CurMode;
  int    LeftPos;
  DWORD  Reserved3;
};

typedef int (WINAPI *FARAPIVIEWERCONTROL)(
  int Command,
  void *Param
);

enum EDITOR_EVENTS {
  EE_READ,EE_SAVE,EE_REDRAW,EE_CLOSE
};

#define EEREDRAW_ALL    (void*)0
#define EEREDRAW_CHANGE (void*)1
#define EEREDRAW_LINE   (void*)2

enum EDITOR_CONTROL_COMMANDS {
  ECTL_GETSTRING,ECTL_SETSTRING,ECTL_INSERTSTRING,ECTL_DELETESTRING,
  ECTL_DELETECHAR,ECTL_INSERTTEXT,ECTL_GETINFO,ECTL_SETPOSITION,
  ECTL_SELECT,ECTL_REDRAW,ECTL_EDITORTOOEM,ECTL_OEMTOEDITOR,
  ECTL_TABTOREAL,ECTL_REALTOTAB,ECTL_EXPANDTABS,ECTL_SETTITLE,
  ECTL_READINPUT,ECTL_PROCESSINPUT,ECTL_ADDCOLOR,ECTL_GETCOLOR,
  ECTL_SAVEFILE,ECTL_QUIT,ECTL_SETKEYBAR,ECTL_PROCESSKEY,ECTL_SETPARAM,
};

enum EDITOR_SETPARAMETER_TYPES {
  ESPT_TABSIZE, ESPT_EXPANDTABS, ESPT_AUTOINDENT, ESPT_CURSORBEYONDEOL,
  ESPT_CHARCODEBASE,
};

struct EditorSetParameter
{
  int Type;
  union {
    int iParam;
    char *cParam;
    DWORD Reserved1;
  };
  DWORD Flags;
  DWORD Reserved2;
};

struct EditorGetString
{
  int StringNumber;
  char *StringText;
  char *StringEOL;
  int StringLength;
  int SelStart;
  int SelEnd;
};


struct EditorSetString
{
  int StringNumber;
  char *StringText;
  char *StringEOL;
  int StringLength;
};


enum EDITOR_OPTIONS {
  EOPT_EXPANDTABS=1,EOPT_PERSISTENTBLOCKS=2,EOPT_DELREMOVESBLOCKS=4,
  EOPT_AUTOINDENT=8,EOPT_SAVEFILEPOSITION=16,EOPT_AUTODETECTTABLE=32,
  EOPT_CURSORBEYONDEOL=64
};


enum EDITOR_BLOCK_TYPES {
  BTYPE_NONE,BTYPE_STREAM,BTYPE_COLUMN
};


struct EditorInfo
{
  int EditorID;
  char *FileName;
  int WindowSizeX;
  int WindowSizeY;
  int TotalLines;
  int CurLine;
  int CurPos;
  int CurTabPos;
  int TopScreenLine;
  int LeftPos;
  int Overtype;
  int BlockType;
  int BlockStartLine;
  int AnsiMode;
  int TableNum;
  DWORD Options;
  int TabSize;
  DWORD Reserved[8];
};


struct EditorSetPosition
{
  int CurLine;
  int CurPos;
  int CurTabPos;
  int TopScreenLine;
  int LeftPos;
  int Overtype;
};


struct EditorSelect
{
  int BlockType;
  int BlockStartLine;
  int BlockStartPos;
  int BlockWidth;
  int BlockHeight;
};


struct EditorConvertText
{
  char *Text;
  int TextLength;
};


struct EditorConvertPos
{
  int StringNumber;
  int SrcPos;
  int DestPos;
};


struct EditorColor
{
  int StringNumber;
  int ColorItem;
  int StartPos;
  int EndPos;
  int Color;
};

struct EditorSaveFile
{
  char FileName[NM];
  char *FileEOL;
};

typedef void (WINAPI *FARSTDUNQUOTE)(char *Str);
typedef DWORD (WINAPI *FARSTDEXPANDENVIRONMENTSTR)(char *src,char *dst,size_t size);

enum INPUTBOXFLAGS{
  FIB_ENABLEEMPTY      = 0x0001,
  FIB_PASSWORD         = 0x0002,
  FIB_EXPANDENV        = 0x0004,
  FIB_NOUSELASTHISTORY = 0x0008,
  FIB_BUTTONS          = 0x0010,
};

typedef int (WINAPI *FARAPIINPUTBOX)(
  char *Title,
  char *SubTitle,
  char *HistoryName,
  char *SrcText,
  char *DestText,
  int   DestLength,
  char *HelpTopic,
  DWORD Flags
);

// <C&C++>
typedef int     (WINAPIV *FARSTDSPRINTF)(char *buffer,const char *format,...);
typedef int     (WINAPIV *FARSTDSSCANF)(const char *s, const char *format,...);
// </C&C++>
typedef void    (WINAPI *FARSTDQSORT)(void *base, size_t nelem, size_t width, int (__cdecl *fcmp)(const void *, const void *));
typedef void    (WINAPI *FARSTDQSORTEX)(void *base, size_t nelem, size_t width, int (__cdecl *fcmp)(void *, void *,void *userparam),void *userparam);
typedef void   *(WINAPI *FARSTDBSEARCH)(const void *key, const void *base, size_t nelem, size_t width, int (__cdecl *fcmp)(const void *, const void *));
typedef int     (WINAPI *FARSTDGETFILEOWNER)(char *Computer,char *Name,char *Owner);
typedef int     (WINAPI *FARSTDGETNUMBEROFLINKS)(char *Name);
typedef int     (WINAPI *FARSTDATOI)(const char *s);
typedef __int64 (WINAPI *FARSTDATOI64)(const char *s);
typedef char   *(WINAPI *FARSTDITOA64)(__int64 value, char *string, int radix);
typedef char   *(WINAPI *FARSTDITOA)(int value, char *string, int radix);
typedef char   *(WINAPI *FARSTDLTRIM)(char *Str);
typedef char   *(WINAPI *FARSTDRTRIM)(char *Str);
typedef char   *(WINAPI *FARSTDTRIM)(char *Str);
typedef char   *(WINAPI *FARSTDTRUNCSTR)(char *Str,int MaxLength);
typedef char   *(WINAPI *FARSTDTRUNCPATHSTR)(char *Str,int MaxLength);
typedef char   *(WINAPI *FARSTDQUOTESPACEONLY)(char *Str);
typedef char   *(WINAPI *FARSTDPOINTTONAME)(char *Path);
typedef void    (WINAPI *FARSTDGETPATHROOT)(char *Path,char *Root);
typedef int     (WINAPI *FARSTDADDENDSLASH)(char *Path);
typedef int     (WINAPI *FARSTDCOPYTOCLIPBOARD)(char *Data);
typedef char   *(WINAPI *FARSTDPASTEFROMCLIPBOARD)(void);
typedef int     (WINAPI *FARSTDINPUTRECORDTOKEY)(INPUT_RECORD *r);
typedef int     (WINAPI *FARSTDLOCALISLOWER)(int Ch);
typedef int     (WINAPI *FARSTDLOCALISUPPER)(int Ch);
typedef int     (WINAPI *FARSTDLOCALISALPHA)(int Ch);
typedef int     (WINAPI *FARSTDLOCALISALPHANUM)(int Ch);
typedef int     (WINAPI *FARSTDLOCALUPPER)(int LowerChar);
typedef void    (WINAPI *FARSTDLOCALUPPERBUF)(char *Buf,int Length);
typedef void    (WINAPI *FARSTDLOCALLOWERBUF)(char *Buf,int Length);
typedef int     (WINAPI *FARSTDLOCALLOWER)(int UpperChar);
typedef void    (WINAPI *FARSTDLOCALSTRUPR)(char *s1);
typedef void    (WINAPI *FARSTDLOCALSTRLWR)(char *s1);
typedef int     (WINAPI *FARSTDLOCALSTRICMP)(char *s1,char *s2);
typedef int     (WINAPI *FARSTDLOCALSTRNICMP)(char *s1,char *s2,int n);
typedef int     (WINAPI *FARSTDPROCESSNAME)(char *param1, char *param2, DWORD flags);

enum XLATMODE{
  XLAT_SWITCHKEYBLAYOUT = 0x0000001UL,
  XLAT_SWITCHKEYBBEEP   = 0x0000002UL,
};

typedef char*   (WINAPI *FARSTDXLAT)(char *Line,int StartPos,int EndPos,struct CharTableSet *TableSet,DWORD Flags);
typedef BOOL    (WINAPI *FARSTDKEYTOKEYNAME)(int Key,char *KeyText,int Size);
typedef int     (WINAPI *FARSTDKEYNAMETOKEY)(char *Name);
typedef int     (WINAPI *FRSUSERFUNC)(WIN32_FIND_DATA *FData,char *FullName,void *Param);
typedef void    (WINAPI *FARSTDRECURSIVESEARCH)(char *InitDir,char *Mask,FRSUSERFUNC Func,DWORD Flags,void *Param);
typedef char*   (WINAPI *FARSTDMKTEMP)(char *Dest, char *Prefix);
typedef void    (WINAPI *FARSTDDELETEBUFFER)(char *Buffer);

enum FRSMODE{
  FRS_RETUPDIR = 0x0001,
  FRS_RECUR    = 0x0002
};

typedef struct FarStandardFunctions
{
  int StructSize;

  FARSTDATOI                 atoi;
  FARSTDATOI64               atoi64;
  FARSTDITOA                 itoa;
  FARSTDITOA64               itoa64;
  // <C&C++>
  FARSTDSPRINTF              sprintf;
  FARSTDSSCANF               sscanf;
  // </C&C++>
  FARSTDQSORT                qsort;
  FARSTDBSEARCH              bsearch;
  FARSTDQSORTEX              qsortex;

  DWORD                      Reserved[9];

  FARSTDLOCALISLOWER         LIsLower;
  FARSTDLOCALISUPPER         LIsUpper;
  FARSTDLOCALISALPHA         LIsAlpha;
  FARSTDLOCALISALPHANUM      LIsAlphanum;
  FARSTDLOCALUPPER           LUpper;
  FARSTDLOCALLOWER           LLower;
  FARSTDLOCALUPPERBUF        LUpperBuf;
  FARSTDLOCALLOWERBUF        LLowerBuf;
  FARSTDLOCALSTRUPR          LStrupr;
  FARSTDLOCALSTRLWR          LStrlwr;
  FARSTDLOCALSTRICMP         LStricmp;
  FARSTDLOCALSTRNICMP        LStrnicmp;

  FARSTDUNQUOTE              Unquote;
  FARSTDEXPANDENVIRONMENTSTR ExpandEnvironmentStr;
  FARSTDLTRIM                LTrim;
  FARSTDRTRIM                RTrim;
  FARSTDTRIM                 Trim;
  FARSTDTRUNCSTR             TruncStr;
  FARSTDTRUNCPATHSTR         TruncPathStr;
  FARSTDQUOTESPACEONLY       QuoteSpaceOnly;
  FARSTDPOINTTONAME          PointToName;
  FARSTDGETPATHROOT          GetPathRoot;
  FARSTDADDENDSLASH          AddEndSlash;
  FARSTDCOPYTOCLIPBOARD      CopyToClipboard;
  FARSTDPASTEFROMCLIPBOARD   PasteFromClipboard;
  FARSTDKEYTOKEYNAME         FarKeyToName;
  FARSTDKEYNAMETOKEY         FarNameToKey;
  FARSTDINPUTRECORDTOKEY     FarInputRecordToKey;
  FARSTDXLAT                 XLat;
  FARSTDGETFILEOWNER         GetFileOwner;
  FARSTDGETNUMBEROFLINKS     GetNumberOfLinks;
  FARSTDRECURSIVESEARCH      FarRecursiveSearch;
  FARSTDMKTEMP               MkTemp;
  FARSTDDELETEBUFFER         DeleteBuffer;
  FARSTDPROCESSNAME          ProcessName;
} FARSTANDARDFUNCTIONS;

struct PluginStartupInfo
{
  int StructSize;
  char ModuleName[NM];
  int ModuleNumber;
  char *RootKey;
  FARAPIMENU             Menu;
  FARAPIDIALOG           Dialog;
  FARAPIMESSAGE          Message;
  FARAPIGETMSG           GetMsg;
  FARAPICONTROL          Control;
  FARAPISAVESCREEN       SaveScreen;
  FARAPIRESTORESCREEN    RestoreScreen;
  FARAPIGETDIRLIST       GetDirList;
  FARAPIGETPLUGINDIRLIST GetPluginDirList;
  FARAPIFREEDIRLIST      FreeDirList;
  FARAPIVIEWER           Viewer;
  FARAPIEDITOR           Editor;
  FARAPICMPNAME          CmpName;
  FARAPICHARTABLE        CharTable;
  FARAPITEXT             Text;
  FARAPIEDITORCONTROL    EditorControl;

  FARSTANDARDFUNCTIONS  *FSF;

  FARAPISHOWHELP         ShowHelp;
  FARAPIADVCONTROL       AdvControl;
  FARAPIINPUTBOX         InputBox;
  FARAPIDIALOGEX         DialogEx;
  FARAPISENDDLGMESSAGE   SendDlgMessage;
  FARAPIDEFDLGPROC       DefDlgProc;
  DWORD                  Reserved;
  FARAPIVIEWERCONTROL    ViewerControl;
};


enum PLUGIN_FLAGS {
  PF_PRELOAD        = 0x0001,
  PF_DISABLEPANELS  = 0x0002,
  PF_EDITOR         = 0x0004,
  PF_VIEWER         = 0x0008,
  PF_FULLCMDLINE    = 0x0010,
};


struct PluginInfo
{
  int StructSize;
  DWORD Flags;
  char **DiskMenuStrings;
  int *DiskMenuNumbers;
  int DiskMenuStringsNumber;
  char **PluginMenuStrings;
  int PluginMenuStringsNumber;
  char **PluginConfigStrings;
  int PluginConfigStringsNumber;
  char *CommandPrefix;
  DWORD SysID;
};


struct InfoPanelLine
{
  char Text[80];
  char Data[80];
  int Separator;
};


struct PanelMode
{
  char *ColumnTypes;
  char *ColumnWidths;
  char **ColumnTitles;
  int FullScreen;
  int DetailedStatus;
  int AlignExtensions;
  int CaseConversion;
  char *StatusColumnTypes;
  char *StatusColumnWidths;
  DWORD Reserved[2];
};


enum OPENPLUGININFO_FLAGS {
  OPIF_USEFILTER               = 0x0001,
  OPIF_USESORTGROUPS           = 0x0002,
  OPIF_USEHIGHLIGHTING         = 0x0004,
  OPIF_ADDDOTS                 = 0x0008,
  OPIF_RAWSELECTION            = 0x0010,
  OPIF_REALNAMES               = 0x0020,
  OPIF_SHOWNAMESONLY           = 0x0040,
  OPIF_SHOWRIGHTALIGNNAMES     = 0x0080,
  OPIF_SHOWPRESERVECASE        = 0x0100,
  OPIF_FINDFOLDERS             = 0x0200,
  OPIF_COMPAREFATTIME          = 0x0400,
  OPIF_EXTERNALGET             = 0x0800,
  OPIF_EXTERNALPUT             = 0x1000,
  OPIF_EXTERNALDELETE          = 0x2000,
  OPIF_EXTERNALMKDIR           = 0x4000,
  OPIF_USEATTRHIGHLIGHTING     = 0x8000
};


enum OPENPLUGININFO_SORTMODES {
  SM_DEFAULT,SM_UNSORTED,SM_NAME,SM_EXT,SM_MTIME,SM_CTIME,
  SM_ATIME,SM_SIZE,SM_DESCR,SM_OWNER,SM_COMPRESSEDSIZE,SM_NUMLINKS
};


struct KeyBarTitles
{
  char *Titles[12];
  char *CtrlTitles[12];
  char *AltTitles[12];
  char *ShiftTitles[12];

  char *CtrlShiftTitles[12];
  char *AltShiftTitles[12];
  char *CtrlAltTitles[12];
};


struct OpenPluginInfo
{
  int                   StructSize;
  DWORD                 Flags;
  char                 *HostFile;
  char                 *CurDir;
  char                 *Format;
  char                 *PanelTitle;
  struct InfoPanelLine *InfoLines;
  int                   InfoLinesNumber;
  char                **DescrFiles;
  int                   DescrFilesNumber;
  struct PanelMode     *PanelModesArray;
  int                   PanelModesNumber;
  int                   StartPanelMode;
  int                   StartSortMode;
  int                   StartSortOrder;
  struct KeyBarTitles  *KeyBar;
  char                 *ShortcutData;
  long                  Reserverd;
};

enum {
  OPEN_DISKMENU,
  OPEN_PLUGINSMENU,
  OPEN_FINDLIST,
  OPEN_SHORTCUT,
  OPEN_COMMANDLINE,
  OPEN_EDITOR,
  OPEN_VIEWER,
  OPEN_FILEPANEL,
};

enum {PKF_CONTROL=1,PKF_ALT=2,PKF_SHIFT=4};

enum FAR_EVENTS {
  FE_CHANGEVIEWMODE,
  FE_REDRAW,
  FE_IDLE,
  FE_CLOSE,
  FE_BREAK,
  FE_COMMAND
};

enum OPERATION_MODES {
  OPM_SILENT=1,
  OPM_FIND=2,
  OPM_VIEW=4,
  OPM_EDIT=8,
  OPM_TOPLEVEL=16,
  OPM_DESCR=32
};

#if defined(__BORLANDC__) || defined(_MSC_VER)
#ifdef __cplusplus
extern "C"{
#endif
// Exported Functions

void   WINAPI _export ClosePlugin(HANDLE hPlugin);
int    WINAPI _export Compare(HANDLE hPlugin,struct PluginPanelItem *Item1,struct PluginPanelItem *Item2,unsigned int Mode);
int    WINAPI _export Configure(int ItemNumber);
int    WINAPI _export DeleteFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
void   WINAPI _export ExitFAR(void);
void   WINAPI _export FreeFindData(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber);
void   WINAPI _export FreeVirtualFindData(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber);
int    WINAPI _export GetFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int Move,char *DestPath,int OpMode);
int    WINAPI _export GetFindData(HANDLE hPlugin,struct PluginPanelItem **pPanelItem,int *pItemsNumber,int OpMode);
int    WINAPI _export GetMinFarVersion(void);
void   WINAPI _export GetOpenPluginInfo(HANDLE hPlugin,struct OpenPluginInfo *Info);
void   WINAPI _export GetPluginInfo(struct PluginInfo *Info);
int    WINAPI _export GetVirtualFindData(HANDLE hPlugin,struct PluginPanelItem **pPanelItem,int *pItemsNumber,char *Path);
int    WINAPI _export MakeDirectory(HANDLE hPlugin,char *Name,int OpMode);
HANDLE WINAPI _export OpenFilePlugin(char *Name,const unsigned char *Data,int DataSize);
HANDLE WINAPI _export OpenPlugin(int OpenFrom,int Item);
int    WINAPI _export ProcessEvent(HANDLE hPlugin,int Event,void *Param);
int    WINAPI _export ProcessHostFile(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
int    WINAPI _export ProcessKey(HANDLE hPlugin,int Key,unsigned int ControlState);
int    WINAPI _export PutFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int Move,int OpMode);
int    WINAPI _export SetDirectory(HANDLE hPlugin,char *Dir,int OpMode);
int    WINAPI _export SetFindList(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber);
void   WINAPI _export SetStartupInfo(struct PluginStartupInfo *Info);
int    WINAPI _export ProcessEditorInput(INPUT_RECORD *Rec);
int    WINAPI _export ProcessEditorEvent(int Event,void *Param);

#ifdef __cplusplus
};
#endif
#endif

#if defined(__BORLANDC__)
  #pragma option -a.
#elif defined(__GNUC__) || (defined(__WATCOMC__) && (__WATCOMC__ < 1100)) || defined(__LCC__)
  #pragma pack()
#else
  #pragma pack(pop)
#endif

#endif /* __PLUGIN_HPP__ */
