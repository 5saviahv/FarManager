#ifndef __FARSTRUCT_HPP__
#define __FARSTRUCT_HPP__
/*
struct.hpp

��� ����������� ��������� (������� �������� ������ ������� ����)

*/

/* Revision: 1.112 18.12.2003 $ */

/*
Modify:
  18.12.2003 SVS
    + DialogsOptions.MouseButton - ���������� ���������� ������/����� ������ ���� ��� ������ �������� ���� �������
      ��� ���� - ������� ��������
    ! ������������ FSizeType � FDateType ��������� �� struct.hpp � farconst.hpp
    + Opt.HistoryCount, FoldersHistoryCount, ViewHistoryCount, DialogsHistoryCount.
    + Opt.CASRule
  29.10.2003 SVS
    + LoadPluginsOptions.SilentLoadPlugin - ����� ����� �������� ��������
  14.10.2003 SVS
    ! Opt.FileSearchMode � Opt.FindFolders �������� � ��������� ��������� struct FindFileOptions
    + FindFileOptions.CollectFiles - �������� NamesList ��� ���������� (����� ���� F3 � ������� ����������� ������)
  13.10.2003 SVS
    ! ��������������:
      Opt.KeyMacroRecord1  -> Opt.KeyMacroCtrlDot
      Opt.KeyMacroRecord2  -> Opt.KeyMacroCtrlShiftDot
  10.10.2003 SVS
    + EditorOptions.WordDiv
  05.10.2003 KM
    + struct FilterParams, Opt.OpFilter
  04.10.2003 SVS
    + Opt.KeyMacroRecord1 ��� KEY_CTRLDOT � Opt.KeyMacroRecord2  ��� KEY_CTRLSHIFTDOT
  17.09.2003 KM
    + Opt.CharTable
  25.08.2003 SVS
    ! Opt.QuotedName - DWORD, �.�. ������� �����
  25.08.2003 SVS
    + Opt.QuotedName - ��������� ����� ������/����� � �������
  23.07.2003 SVS
    + Opt.ScrSize.DeltaXY - ��������� ������� ��� "����������" �������
  06.06.2003 SVS
    ! ���, ��� ������� � ��������� �������� ���������� � ��������� LoadPluginsOptions
  19.05.2003 SVS
    - ������������ ����������� � struct DialogsOptions
    ! DialogsOptions.SelectedType -> DialogsOptions.EditLine
  14.05.2003 VVM
    + ViewerOptions.PersistentBlocks - ���������� ����� �� ������
  06.05.2003 SVS
    ! Opt.UseTTFFont �������� �� Opt.UseUnicodeConsole - ��� ������
  21.04.2003 SVS
    + Opt.DelThreadPriority
    + struct ScreenSizes, Opt.ScrSize - ��� ������� "Alt-Enter"
  05.03.2003 SVS
    + Opt.ScanJunction - ����������� ��� �� ��������.
  10.02.2003 SVS
    + Opt.ShowTimeoutDelFiles; // ������ � �������� �������� (� ms)
  13.01.2003 SVS
    + Opt.SortFolderExt
  23.12.2002 SVS
    ! OnlyEditorViewerUsed ���� ������ ��������� Options
  07.10.2002 SVS
    + Opt.SetupArgv - ���������� ��������� � ���������� ����
  12.08.2002 SVS
   + Opt.ExecuteUseAppPath
  30.05.2002 SVS
    + Opt.UseTTFFont
  25.05.2002 IS
    ! ��������� const
  24.05.2002 SVS
    + Opt.UseNumPad
  22.05.2002 SVS
    + Opt.CloseCDGate
  05.04.2002 SVS
    + Opt.PluginMaxReadData
  01.04.2002 SVS
    + struct NowellOptions
  18.03.2002 SVS
    ! �����, ������������� �� ������� �������� � ���������
      ��������� DialogsOptions
    + Opt.Dialogs.SelectedType
  12.03.2002 VVM
    + Opt.EscTwiceToInterrupt
      ���������� ��������� ��� ���������� ���������� ��������
      0 - ������ ESC ���������� ��������
      1 - ������ ESC ��������� ��������
  19.02.2002 SVS
    ! � �������� � �������� Opt.XLat - ������ ���� = ������ �������.
    ! Opt.XLat.Rules - 80 ���� (�� 40 ������)
  26.12.2001 SVS
    + Opt.CloseConsoleRule, Opt.Diz.ROUpdate
    ! Opt.CursorSize - ���� ��� 2 ������� ��� Overide-�����
  21.12.2001 SVS
    + Opt.RestoreCPAfterExecute
  17.12.2001 IS
    + Opt.PanelMiddleClickRule - ��������� ������� ������ ���� � �������
  07.12.2001 IS
    + Opt.MultiMakeDir - ����� �������� ���������� ��������� �� ���� �����
  03.12.2001 IS
    + Opt.EditorUndoSize - ������ ������ undo � ���������
  01.11.2001 SVS
    ! ������ Opt.CPAJHefuayor ;-(
  30.10.2001 SVS
    + Opt.CPAJHefuayor
  29.10.2001 IS
    ! SaveEditorPos � SaveEditorShortPos ��������� � EditorOptions
  26.10.2001 KM
    + Opt.FindFolders. ��������� ���� ���������� ������ ��������� � Alt-F7
  19.10.2001 SVS
    + struct PreRedrawParamStruct - ��� ����������� BugZ#85
  15.10.2001 SVS
    + Opt.DlgSelectFromHistory
  12.10.2001 SVS
    ! �� �������� (Opt.FolderSetAttr165!!!) - ��� � ��� ���� ��, ��� ����:
      Opt.SetAttrFolderRules!
  11.10.2001 SVS
    + Opt.FolderSetAttr165; // ��������� ��� ��������� ��� � 1.65
  07.10.2001 SVS
    + Opt.HelpTabSize - ������ ��������� �� ���������.
  26.09.2001 SVS
    + Opt.AutoUpdateLimit -  ���� ����� ���������� �������������
      �� ��������� ������.
  08.09.2001 VVM
    + ���������� ����� � ������� ����� - Opt.DialogsEditBlock;
      0 - ����� ������������
      1 - ����� ����������
  05.09.2001 SVS
    + ���������� HighlightDataColor
  03.08.2001 IS
    + ����� "��������� �����������������/�����������/�������� ������":
      Opt.MultiCopy
  24.07.2001 SVS
    + Opt.PgUpChangeDisk
    + Opt.Confirmation.HistoryClear
  04.07.2001 SVS
    + Opt.LCIDSort
  22.06.2001 SVS
    + Opt.DateFormat
  21.05.2001 OT
    + Opt.AltF9
    + Opt.Confirmation.AllowReedit
  14.05.2001 SVS
    + Opt.ShowCheckingFile - ��� ��������� ���������� � ���������...
  06.05.2001 DJ
    ! �������� #include
  04.05.2001 SVS
    ! ������� �� ����� ���� �� DI_LISTBOX ;-) - ����� ���� FarDialogItem.ListPos
  28.04.2001 VVM
    + Opt.SubstNameRule ������� �����:
      0 - ���� ����������, �� ���������� ������� ����� ��� GetSubstName()
      1 - ���� ����������, �� ���������� ��� ��������� ��� GetSubstName()
  26.04.2001 VVM
    + Opt.MsWheelDeltaView - ������ �������� ��� ��������� ������.
    + Opt.MsWheelDeltaEdit - ������ �������� ��� ��������� ���������.
    + Opt.MouseWheelDelta -> Opt.MsWheelDelta
  23.04.2001 SVS
    ! ����� ����� GroupSortData.OriginalMasks � HighlightData.OriginalMasks
      ����� ����� �� %PATHEXT% - �� ��� ����������� � ��, ��� ����� -
      ������ ��������.
  22.04.2001 SVS
    + Opt.QuotedSymbols - ����������� ��� QuoteSpace()
  16.04.2001 VVM
    + Opt.MouseWheelDelta - ������ �������� ��� ���������.
  02.04.2001 VVM
    + Opt.FlagPosixSemantics ����� ������ ��:
        ���������� ������ � ������� � ������ ���������
        ���������� LastPositions � ��������� � ������
  30.03.2001 SVS
   + Opt.Policies - �������� �������.
  29.03.2001 IS
   + struct ViewerOptions
  28.03.2001 VVM
   + Opt.RememberLogicalDrives = ���������� ���������� ����� � �� ����������
     ������ ���. ��� �������������� "����������" "�������" ������.
  20.03.2001 SVS
   + Opt.FullScreenHelp - ��� ���������� ���������� ������������
  16.03.2001 SVS
   + Opt.ChangeDriveDisconnetMode - ��� ����������� - ��� ������� ���������
     ����� (��� CheckBox`�)
  15.03.2001 SVS
   + Opt.Confirm.RemoveConnection - ������������� ��� �������� ��������� ������
  12.03.2001 SVS
   + Opt.DeleteSymbolWipe -> Opt.WipeSymbol
  12.03.2001 SVS
   + Opt.DeleteSymbolWipe ������ ����������� ��� "ZAP-��������"
  27.02.2001 SVS
   + EditorOptions.CharCodeBase - ���� ������ ���� �������
  26.02.2001 VVM
   + Opt.ExceptCallDebugger - �������� �������� ��� ���
   ! Opt.ExceptRules - ������� �����
  21.02.2001 IS
   ! ����� �������� ��������� ��������� � EditorOptions
  12.02.2001 SKV
   + ConsoleDetachKey. ������� ����������� ������� ���� ��
     �������������� �������� � ��� �����������.
  11.02.2001 SVS
   ! ��������� �������� DialogItem � DialogData (DIF_VAREDIT)
   ! HighlightData.Masks - ������ ������� (DIF_VAREDIT)
   ! GroupSortData.Masks - ������ ������� (DIF_VAREDIT)
   ! FilterDataRecord.Masks - ������ ������� (DIF_VAREDIT)
  11.02.2001 SVS
   ! ��������� � MenuItem.
  09.02.2001 IS
   + RightSelectedFirst, LeftSelectedFirst;
   + Confirmation.Esc
  30.01.2001 VVM
   + ���������� ����� �����������,���������� ����� � ������� ��������.
      ������� �� �������� � ������� CopyTimeRule
  22.01.2001 SVS
   + Opt.CursorSize[2] - ������ ������� ���� :-)
  19.01.2001 SVS
   + Opt.MacroReuseRules - ��������� �������������.
  09.01.2001 SVS
   + Opt.ShiftsKeyRules - ������� �� ���� ������ ��������� ����������
     Alt-����� ��� ����������� ������� � �������� "`-=[]\;',./" �
     �������������� Alt-, Ctrl-, Alt-Shift-, Ctrl-Shift-, Ctrl-Alt-
  28.12.2000 SVS
   + Opt.HotkeyRules - ������� �� ���� ������ ��������� �������
  21.12.2000 SVS
   ! ��������� MacroRecord ���������� � macro.hpp
  08.12.2000 SVS
   ! ��������� � ��������� DialogItem.
  29.11.2000 SVS
   + Opt.EditorReadOnlyLock - ������ ���� ��� �������� � ���������, ����
     �� ����� �������� R|S|H
   + Opt.EditorFileSizeLimit - ���������� ���������� ������ �����, �����
     �������� ����� ����� ������ � ���������������� �������� ���������
     ����� �� ��������������
  28.11.2000 SVS
   + Opt.EditorF7Rules - ������� �� ���� ������ � ���������
  27.11.2000 SVS
   + Opt.ExceptRules - ������� �� ���� ������ ����������
  25.11.2000 IS
   + �������������� ���� �� ������� ��� ������� Xlat (WordDivForXlat)
  24.11.2000 SVS
   ! XLat.Rules - 3 �� 30 ������, ������ 10
   + ������� �� ���� ��������� ��������� �� ��������
  04.11.2000 SVS
   ! XLat - ��� ��� ��������� � XLat - � ���� ���������.
   + XLat - ���������� �������������� ������:
       XLatAltEditorKey, XLatAltCmdLineKey, XLatAltDialogKey;
  20.10.2000 SVS
   + Opt.PanelCtrlFRule
      Panel/CtrlFRule � ������� - ������ ��������� Ctrl-F
      ���� = 0, �� ���������� ���� ��� ����, ����� - � ������
      ����������� �� ������
  17.10.2000 SVS
   ! WordDiv ����� ������ 256;
  10.10.2000 SVS
   + Opt.EditorBSLikeDel
  27.09.2000 SVS
   + Opt.HelpURLRules - =0 ��������� ����������� ������� URL-����������
   + Opt.AllCtrlAltShiftRule - ������� �����, ������ ��������� Ctrl-Alt-Shift
     ��� ���������� - ������� ��������:
     0 - Panel
     1 - Edit
     2 - View
     3 - Help
     4 - Dialog
  24.09.2000 SVS
   + Opt.MaxPositionCache - ���������� ������� � ���� ����������
   + Opt.SaveViewerShortPos - ����������� ������� �� ������� �� Ctrl-0..9
   + Opt.SaveEditorShortPos - ����������� ������� � ��������� �� Ctrl-0..9
   + Opt.CmdHistoryRule ������ ��������� Esc ��� ��������� ������.
   + ������� ��� ������ ������� Xlat:
     Opt.XLatEditorKey, Opt.XLatCmdLineKey, Opt.XLatDialogKey
  20.09.2000 SVS
   + Opt.SubstPluginPrefix - 1 = ��������������� ������� �������
     ��� Ctrl-[ � ��� ��������
  19.09.2000 SVS
   + Opt.PanelCtrlAltShiftRule ������ ��������� Ctrl-Alt-Shift ��� �������.
  12.09.2000 SVS
   + ��������� ���������� Options.ViewerWrap
   ! ViewerTypeWrap ������������� � ViewerIsWrap
       ���������� Wrap/WWrap/UnWrap �� 2 ������������ -
       ��������� (Wrap/UnWrap) � ��� (Wrap/WWrap)
        ViewerIsWrap  =  UnWrap=0  | �������=1
        ViewerWrap    =  Wrap=0    | WordWarp=1
   + Opt.PanelRightClickRule ������ ��������� ������ ������� ����
     (��� �� ������ Bug#17)
  11.09.2000 SVS
   + � Opt ��������� ���������� DlgEULBsClear
     ���� = 1, �� BS � �������� ��� UnChanged ������ ������� �����
     ������ �����, ��� � Del
  09.09.2000 SVS 1.14
   + CHAR_INFO *VBuf; � ��������� �������
  07.09.2000 tran 1.13
   + Config//Current File
  05.09.2000 SVS 1.12
   + ��������� CodeQWERTY, ����������� QWERTY-��������������
   ! � Opt �������� ���� ����������, �������� QWERTY-�������������
  01.09.2000 tran 1.11
   + Options.PluginsCacheOnly - ������� ������� ������ �� ����
  31.08.2000 SVS
   ! DialogItem.Flags, DialogData.Flags - ��� DWORD
   - �� ����������� ��� �����.
     ��������� ���������� Options.ViewerTypeWrap
  12.08.2000 KM 1.09
   ! � ���������� DialogItem � DialogData ����� ����, ����������
     � union, char *Mask.
  18.08.2000 SVS
   ! struct FarListItems -> struct FarList, � �� ������ ��������� :-)
  03.08.2000 SVS
   ! WordDiv -> Opt.WordDiv
  03.08.2000 SVS
   + ������� � Options: MainPluginDir - ������������ �������� ���� ���
     ������ ��������...
  01.08.2000 SVS
   ! ��������� � ���������� Dialog*
  26.07.2000 SVS
   + Opt.AutoComplete
  18.07.2000 tran 1.04
   + Opt.ViewerShowScrollBar, Opt.ViewerShowArrows
  15.07.2000 tran
   + �������� �������� ������ KeyBar � Viewer - Options::ShowKeyBarViewer
  15.07.2000 SVS
   + Opt.PersonalPluginsPath - ���� ��� ������ ������������ ��������
  29.06.2000 SVS
   + �������� ������� ������ Scroll Bar � ���� - Options::ShowMenuScrollbar
  25.06.2000 SVS
   ! ���������� Master Copy
   ! ��������� � �������� ���������������� ������
*/

#include "farconst.hpp"

struct FilterParams
{
  struct
  {
    DWORD Used;
    char Mask[NM*2];
  } FMask;
  struct
  {
    DWORD Used;
    FDateType DateType;
    FILETIME DateAfter;
    FILETIME DateBefore;
  } FDate;
  struct
  {
    DWORD Used;
    FSizeType SizeType;
    __int64 SizeAbove;
    __int64 SizeBelow;
  } FSize;
  struct
  {
    DWORD Used;
    DWORD AttrSet;
    DWORD AttrClear;
  } FAttr;
};

struct PanelOptions
{
  int Type;
  int Visible;
  int Focus;
  int ViewMode;
  int SortMode;
  int SortOrder;
  int SortGroups;
  int ShowShortNames;
};

struct Confirmation
{
  int Copy;
  int Move;
  int Drag;
  int Delete;
  int DeleteFolder;
  int Exit;
  /* $ 09.02.2001 IS
     ��� CheckForEsc
  */
  int Esc;
  /* IS $ */
  /* $ 12.03.2002 VVM
    + Opt.EscTwiceToInterrupt
      ���������� ��������� ��� ���������� ���������� ��������
      0 - ������ ESC ���������� ��������
      1 - ������ ESC ��������� �������� */
  int EscTwiceToInterrupt;
  /* VVM $ */
  int RemoveConnection;
  /* $ 23.05.2001
    +  Opt.Confirmation.AllowReedit - ����, ������� �������� ��������� ��������
      ����� �� �������������� ����, ������ ���� ��� �������������. �� ��������� - 1
      0 - ���� ��� �������� ���� �� ��� �������, �� ���������� ������� � ��������� ���������
          ��� �������������� ��������. ���� ���� ��� �������, �� �������� ������, � � ������
          ���� ������� ������� Reload, �� ����������� ����� ����� �����, ��� ���� ���������
          ��������� ��������.
      1 - ��� ��� ���� ������. �������� ������ � ���������� ������� ���� ��� � ��������� �����
          ���� ����������� ����� ������ ���������.
      */
  int AllowReedit;
  int HistoryClear;
};


struct RegInfo
{
  char RegName[256];
  char RegCode[256];
  int Done;
};


struct DizOptions
{
  char ListNames[NM];
  int ROUpdate;
  int UpdateMode;
  int SetHidden;
  int StartPos;
};

/* $ 05.09.2000 SVS
   ��������� CodeQWERTY, ����������� QWERTY-��������������
*/
struct CodeXLAT{
  DWORD Flags;       // �������������� �����
  /* $ 05.09.2000 SVS
     � Opt ��������� �������, ���������� ������� Xlat
  */
  int XLatEditorKey;
  int XLatCmdLineKey;
  int XLatDialogKey;
  /* SVS $*/
  /* $ 04.11.2000 SVS
     � Opt ��������� �������������� �������, ���������� ������� Xlat
  */
  int XLatAltEditorKey;
  int XLatAltCmdLineKey;
  int XLatAltDialogKey;
  /* SVS $*/
  /* $ 25.11.2000 IS
     �������������� ���� �� ������� ��� ������� Xlat
  */
  char WordDivForXlat[256];
  /* IS $ */
  // ������ ���� - ������ �������
  BYTE Table[2][81]; // [0] non-english �����, [1] english �����
  BYTE Rules[3][81]; // 3 �� 40 ������:
                    //  [0] "���� ���������� ������ ���������"
                    //  [1] "���� ���������� ������ ����������� ������"
                    //  [2] "���� ���������� ������ �� ���/lat"
};
/* SVS $*/

/* $ 21.02.2001 IS
     ����� ���������: ��������� ���������
*/
struct EditorOptions
{
  int TabSize;
  int ExpandTabs;
  int PersistentBlocks;
  int DelRemovesBlocks;
  int AutoIndent;
  int AutoDetectTable;
  int AnsiTableForNewFile;
  int CursorBeyondEOL;
  int BSLikeDel;
  int CharCodeBase;
  int SavePos;
  int SaveShortPos;
  char WordDiv[256];
};
/* IS $ */

/* $ 29.03.2001 IS
     ��� ������� ������� "���������" ��������� ��� ��������� ���������
*/
struct ViewerOptions
{
  int TabSize;
  int AutoDetectTable;
  /* $ 18.07.2000 tran
    + ���� �������� ��� viewer*/
  int ShowScrollbar;
  int ShowArrows;
  /* tran 18.07.2000 $ */
  /* $ 14.05.2002 VVM
    + ���������� ����� �� ������ */
  int PersistentBlocks;
  /* VVM */
};
/* IS $ */

// "�������"
struct PoliciesOptions {
  int DisabledOptions;  // ������������� ���� ������������
  int ShowHiddenDrives; // ���������� ������� ���������� �����
};

struct DialogsOptions{
  int   EditBlock;          // ���������� ����� � ������� �����
  int   EditHistory;        // ��������� � �������?
  int   AutoComplete;       // ��������� ��������������?
  int   EULBsClear;         // = 1 - BS � �������� ��� UnChanged ������ ������� ����� ������ �����, ��� � Del
  int   SelectFromHistory;  // = 0 then (ctrl-down � ������ � �������� ������ �������������� �� ����� ������� ������)
  DWORD EditLine;           // ����� ���������� � ������ ����� (������ ��� ����... ��������� ��������� ����������)
  int   MouseButton;        // ���������� ���������� ������/����� ������ ���� ��� ������ �������� ���� �������
};

struct NowellOptions{
  int MoveRO;               // ����� ��������� Move ������� R/S/H ��������, ����� �������� - ���������� �������
};

// ��������� ���������� ������ character table
struct FindCharTable
{
  int AllTables;
  int AnsiTable;
  int UnicodeTable;
  int TableNum;
};

struct ScreenSizes{
  COORD DeltaXY;            // �� ������� ���. �������� ������� ��� ������������ ������
#if defined(DETECT_ALT_ENTER)
  /*
    Opt.WScreenSize - Windowed/Full Screen Size
       COORD[0].X - Windowed Width  mode 1
       COORD[0].Y - Windowed Height mode 1
       COORD[1].X - Windowed Width  mode 2
       COORD[1].Y - Windowed Height mode 2

       COORD[2].X - FullScreen Width  mode 1
       COORD[2].Y - FullScreen Height mode 1
       COORD[3].X - FullScreen Width  mode 2
       COORD[3].Y - FullScreen Height mode 2
  */
  int WScreenSizeSet;
  COORD WScreenSize[4];
#endif
};

struct LoadPluginsOptions{
//  DWORD TypeLoadPlugins;       // see TYPELOADPLUGINSOPTIONS
  /* $ 03.08.2000 SVS
     TRUE - ������������ ����������� ���� � �������� ��������
  */
  int MainPluginDir;
  /* SVS $*/
  /* $ 01.09.2000 tran
     seting by '/co' switch, not saved in registry. */
  int PluginsCacheOnly;
  /* tran $ */
  int PluginsPersonal;

  char CustomPluginsPath[NM];  // ���� ��� ������ ��������, ��������� � /p
  /* $ 15.07.2000 SVS
    + ���� ��� ������ ������������ ��������, ������� ������ ��-�� ����,
      ��� ����� ����� ������ ������� ����...
  */
  char PersonalPluginsPath[1024];
  /* SVS $*/
  int SilentLoadPlugin; // ��� �������� ������� � ������...
};

struct FindFileOptions{
  int FindFolders;
  int CollectFiles;
  int FileSearchMode;
};

struct Options
{
  int Clock;
  int ViewerEditorClock;
  int Mouse;
  int ShowKeyBar;
  int ScreenSaver;
  int ScreenSaverTime;
  int UsePromptFormat;
  char PromptFormat[80];
  int AltGr;
  int InactivityExit;
  int InactivityExitTime;
  int ShowHidden;
  int Highlight;
  int AutoChangeFolder;
  char LeftFolder[NM];
  char RightFolder[NM];
  /* $ 07.09.2000 tran
     + Config//Current File */
  char LeftCurFile[NM];
  char RightCurFile[NM];
  /* tran 07.09.2000 $ */

  /* $ 09.02.2001 IS
     ��������� ������ "���������� ������"
  */
  int RightSelectedFirst;
  int LeftSelectedFirst;
  /* IS $ */
  int SelectFolders;
  int ReverseSort;
  int ClearReadOnly;
  int SortFolderExt;
  int DeleteToRecycleBin;
  int WipeSymbol; // ������ ����������� ��� "ZAP-��������"
  int UseSystemCopy;
  int CopyOpened;
  int CopyShowTotal;
  /* $ 03.08.2001 IS
       ����� "��������� �����������������/�����������/�������� ������"
  */
  int MultiCopy;
  /* IS $ */
  /* $ 07.12.2001 IS ����� �������� ���������� ��������� �� ���� ����� */
  int MultiMakeDir;
  /* IS $ */
  /* $ 03.12.2001 IS
       ������ ������ undo � ���������
  */
  int EditorUndoSize;
  /* IS $ */
  int CreateUppercaseFolders;
  int UseRegisteredTypes;
  int UseExternalViewer;
  char ExternalViewer[NM];
  int UseExternalEditor;
  char ExternalEditor[NM];
  int SaveViewerPos;
  int SaveViewerShortPos;
  /* $ 21.02.2001 IS
       ���������� ��� ��������� ��������� � ��������������� ���������
  */
  struct EditorOptions EdOpt;
  /* IS $ */
  /* $ 29.03.2001 IS
       ��������� ���������� ��� ������ ��������� � ��������������� ���������
  */
  struct ViewerOptions ViOpt;
  /* IS $ */
  /* $ 29.11.2000 SVS
   + Opt.EditorReadOnlyLock - ������ ���� ��� �������� � ���������, ����
     �� ����� �������� R|S|H
  */
  int EditorReadOnlyLock;
  /* SVS $ */
  /* $ 29.11.2000 SVS
   + Opt.EditorFileSizeLimit - ���������� ���������� ������ �����, �����
     �������� ����� ����� ������ � ���������������� �������� ���������
     ����� �� ��������������
  */
  DWORD EditorFileSizeLimitLo;
  DWORD EditorFileSizeLimitHi;
  /* SVS $ */
  /* $ 03.08.2000 SVS
     �������������� ���� �� �������
  */
  char WordDiv[256];
  /* SVS $ */
  char QuotedSymbols[32];
  DWORD QuotedName;
  int SaveHistory;
  int SaveFoldersHistory;
  int SaveViewHistory;
  int AutoSaveSetup;
  int SetupArgv; // ���������� ��������� � ���������� ����
  int ChangeDriveMode;
  int ChangeDriveDisconnetMode;

  int HistoryCount;
  int FoldersHistoryCount;
  int ViewHistoryCount;
  int DialogsHistoryCount;

  struct FindFileOptions FindOpt;

  char TempPath[NM];
  int HeightDecrement;
  int WidthDecrement;
  char PassiveFolder[NM];
  int ShowColumnTitles;
  int ShowPanelStatus;
  int ShowPanelTotals;
  int ShowPanelFree;
  int ShowPanelScrollbar;
  /* $ 29.06.2000 SVS
    �������� ������� ������ Scroll Bar � ����.
  */
  int ShowMenuScrollbar;
  /* SVS $*/
  int ShowScreensNumber;
  int ShowSortMode;
  int ShowMenuBar;
  /* $ 15.07.2000 tran
    + ShowKeyBarViewer*/
  int ShowKeyBarViewer;
  /* tran 15.07.2000 $ */
  int CleanAscii;
  int NoGraphics;
  char FolderInfoFiles[1024];
  struct Confirmation Confirm;
  struct DizOptions Diz;
  struct PanelOptions LeftPanel;
  struct PanelOptions RightPanel;
  DWORD  AutoUpdateLimit; // ���� ����� ���������� ������������� �� ��������� ������.
  char Language[80];
  int SmallIcon;
  char RegRoot[NM];
  /* $ 31.08.2000 SVS
     ��������� ���������� Options.ViewerTypeWrap
  */
  /* $ 12.09.2000 SVS
     ��������� ���������� Options.ViewerWrap
     ViewerTypeWrap ������������� � ViewerIsWrap
  */
  int ViewerIsWrap; // (Wrap|WordWarp)=1 | UnWrap=0
  int ViewerWrap; // Wrap=0|WordWarp=1
  /* SVS 12.09.2000 $*/
  /* SVS $*/
  /* $ 12.09.2000 SVS
   + Opt.PanelRightClickRule ������ ��������� ������ ������� ����
     (��� �� ������ Bug#17)
  */
  int PanelRightClickRule;
  /* SVS $*/
  /* $ 17.12.2001 IS ��������� ������� ������ ���� � ������� */
  int PanelMiddleClickRule;
  /* IS $ */
  /* $ 19.09.2000 SVS
   + Opt.PanelCtrlAltShiftRule ������ ��������� Ctrl-Alt-Shift ��� �������.
  */
  int PanelCtrlAltShiftRule;
  /* SVS $*/
  /* $ 20.10.2000 SVS
    Panel/CtrlFRule � ������� - ������ ��������� Ctrl-F
    ���� = 0, �� ���������� ���� ��� ����, ����� - � ������
    ����������� �� ������
  */
  int PanelCtrlFRule;
  /* SVS $*/
  /* $ 27.09.2000 SVS
   + Opt.AllCtrlAltShiftRule - ������� �����, ������ ��������� Ctrl-Alt-Shift
     ��� ���������� - ������� ��������:
     0 - Panel
     1 - Edit
     2 - View
     3 - Help
     4 - Dialog
  */
  int AllCtrlAltShiftRule;
  /* SVS $*/
  /* $ 24.09.2000 SVS
   + Opt.CmdHistoryRule ������ ��������� Esc ��� ��������� ������:
      =1 - �� �������� ��������� � History, ���� ����� Ctrl-E/Ctrl/-X
           ������ ESC (��������� - ��� VC).
      =0 - ��������� ��� � ���� - �������� ��������� � History
  */
  int CmdHistoryRule;
  /* SVS $*/
  /* $ 20.09.2000 SVS
   + Opt.SubstPluginPrefix - 1 = ��������������� ������� �������
     ��� Ctrl-[ � ��� ��������
  */
  int SubstPluginPrefix;
  /* SVS $*/
  /* $ 24.09.2000 SVS
   + Opt.MaxPositionCache - ���������� ������� � ���� ����������
  */
  int MaxPositionCache;
  /* SVS $*/
  /* $ 22.11.2000 SVS
   + ������� �� ���� ��������� ��������� �� ��������*/
  int SetAttrFolderRules;
  /* SVS $ */
  /* $ 27.11.2000 SVS
   + Opt.ExceptRules - ������� �� ���� ������ ���������� */
  int ExceptRules;
  /* $ 26.02.2001 VVM
   + Opt.ExceptCallDebugger - �������� �������� ��� ���������� */
  int ExceptCallDebugger;
  /* VVM $ */
  /* SVS $ */
  /* $ 28.11.2000 SVS
   + Opt.EditorF7Rules - ������� �� ���� ������ � ��������� */
  int EditorF7Rules;
  /* SVS $ */
  /* $ 28.12.2000 SVS
   + Opt.HotkeyRules - ������� �� ���� ������ ��������� ������� */
  int HotkeyRules;
  /* SVS $ */
  /* $ 09.01.2001 SVS
   + Opt.ShiftsKeyRules - ������� �� ���� ������ ��������� ����������
     Alt-����� ��� ����������� ������� � �������� "`-=[]\;',./" �
     �������������� Alt-, Ctrl-, Alt-Shift-, Ctrl-Shift-, Ctrl-Alt- */
  int ShiftsKeyRules;
  /* SVS $ */
  /* $ 19.01.2001 SVS
   + Opt.MacroReuseRules - ������� �� ���� �������� ������������� �����������
     ������ */
  int MacroReuseRules;
  /* SVS $ */
  DWORD KeyMacroCtrlDot; // ��� KEY_CTRLDOT
  DWORD KeyMacroCtrlShiftDot; // ��� KEY_CTRLSHIFTDOT
  /* $ 22.01.2001 SVS
   + Opt.CursorSize - ������ ������� ���� :-)
     ������ */
  int CursorSize[4];
  /* SVS $ */
  /* $ 05.09.2000 SVS
     � Opt �������� ���� ����������, �������� QWERTY-�������������
  */
  struct CodeXLAT XLat;
  /* SVS $*/
  /* $ 30.01.2001 VVM
    + ���������� ����� �����������,���������� ����� � ������� ��������.
      ������� �� �������� � ������� CopyTimeRule */
  int CopyTimeRule;
  /* VVM $ */
  /*$ 08.02.2001 SKV
    ���������� ������ ��� ������ Far'������ �������
    �� ����������� ���������������� �������� � ��� �����������.
  */
  int ConsoleDetachKey;
  /* SKV$*/

  char HelpLanguage[80];
  int FullScreenHelp;
  int HelpTabSize;
  /* $ 27.09.2000 SVS
   + Opt.HelpURLRules - =0 ��������� ����������� ������� URL-����������
  */
  int HelpURLRules;

  /* SVS $*/
  /* $ 28.03.2001 VVM
    + RememberLogicalDrives = ���������� ���������� ����� � �� ����������
      ������ ���. ��� �������������� "����������" "�������" ������. */
  int RememberLogicalDrives;
  /* VVM $ */
  /* $ 02.04.2001 VVM
    + Opt.FlagPosixSemantics ����� ������ ��:
        ���������� ������ � ������� � ������ ���������
        ���������� LastPositions � ��������� � ������ */
  int FlagPosixSemantics;
  /* VVM $ */
  /* $ 16.04.2001 VVM
    + Opt.MouseWheelDelta - ������ �������� ��� ���������. */
  int MsWheelDelta;
  /* VVM $ */
  int MsWheelDeltaView;
  int MsWheelDeltaEdit;
  /* $ 28.04.2001 VVM
    + Opt.SubstNameRule ������� �����:
      0 - ���� ����������, �� ���������� ������� ����� ��� GetSubstName()
      1 - ���� ����������, �� ���������� ��� ��������� ��� GetSubstName() */
  int SubstNameRule;
  /* VVM $ */

  /* $ 23.05.2001 AltF9
    + Opt.AltF9 ���� ��������� ������� ��������  ������ ���������� Alt-F9
         (��������� ������� ������) � ������� ������. �� ��������� - 1.
      0 - ������������ ��������, ����������� � FAR ������ 1.70 beta 3 �
         ����, �.�. ������������ 25/50 �����.
      1 - ������������ ������������������� �������� - ���� FAR Manager
         ����� ������������� � ����������� �� ����������� ��������� ������
         ����������� ���� � �������.*/
  int AltF9;
  /* OT $ */
  int PgUpChangeDisk;
  int ShowCheckingFile;
  int CloseConsoleRule;
  int CloseCDGate;       // ���������������� CD

  DWORD LCIDSort;
  int RestoreCPAfterExecute;
  int ExecuteUseAppPath;
  DWORD PluginMaxReadData;
  int UseNumPad;
  int CASRule;
  int UseUnicodeConsole;
  int ScanJunction;

  int OnlyEditorViewerUsed; // =1, ���� ����� ��� /e ��� /v

  DWORD ShowTimeoutDelFiles; // ������ � �������� �������� (� ms)
  int DelThreadPriority; // ��������� �������� ��������, �� ��������� = THREAD_PRIORITY_NORMAL

  //int CPAJHefuayor; // ����������� �� "Close Plugin And Jump:
                  // Highly experimental feature, use at your own risk"

  char DateFormat[80]; // ��� $Date
  struct LoadPluginsOptions LoadPlug;

  struct DialogsOptions Dialogs;
  struct PoliciesOptions Policies;
  struct NowellOptions Nowell;
  struct ScreenSizes ScrSize;
  /* $ 17.09.2003 KM
       ��������� ��� ����������� ���������� ������ �������� � ������
  */
  struct FindCharTable CharTable;
  /* KM $ */
  /* $ 28.09.2003 KM
     OpFilter - �������� ��� ����������� ������� ��������� �������
  */
  struct FilterParams OpFilter;
  /* KM $ */
};


struct PluginHandle
{
  HANDLE InternalHandle;
  int PluginNumber;
};

// for class Edit
struct ColorItem
{
  int StartPos;
  int EndPos;
  int Color;
};

/* ���������!!! ���������� ������� ������ :-))
   ��������� ������ ���� 16 ����!
*/
struct HighlightDataColor
{
  BYTE Color;
  BYTE SelColor;
  BYTE CursorColor;
  BYTE CursorSelColor;
  BYTE MarkChar;
  BYTE Reserved[11];
};

struct PreRedrawParamStruct
{
  DWORD Flags;
  void *Param1;
  const void *Param2;
  const void *Param3;
  void *Param4;
};

#endif // __FARSTRUCT_HPP__
