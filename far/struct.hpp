#ifndef __FARSTRUCT_HPP__
#define __FARSTRUCT_HPP__
/*
struct.hpp

��� ����������� ��������� (������� �������� ������ ������� ����)

*/

#include "farconst.hpp"

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
	int NumericSort;
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
	int RemoveSUBST;
	int RemoveHotPlug;
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
struct CodeXLAT
{
	DWORD Flags;       // �������������� �����
	char WordDivForXlat[256];
	HKL Layouts[10];
	int CurrentLayout;
	// ������ ���� - ������ �������
	BYTE Table[2][81]; // [0] non-english �����, [1] english �����
	BYTE Rules[3][81]; // 3 �� 40 ������:
	//  [0] "���� ���������� ������ ���������"
	//  [1] "���� ���������� ������ ����������� ������"
	//  [2] "���� ���������� ������ �� ���/lat"
};

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
	int AnsiTableAsDefault;
	int CursorBeyondEOL;
	int BSLikeDel;
	int CharCodeBase;
	int SavePos;
	int SaveShortPos;
	int F7Rules; // $ 28.11.2000 SVS - ������� �� ���� ������ � ���������
	int AllowEmptySpaceAfterEof; // $ 21.06.2005 SKV - ��������� ���������� ������ ������������ ����� ��������� ������ �������������� �����.
	char WordDiv[256];

	int ReadOnlyLock; // $ 29.11.2000 SVS - ������ ���� ��� �������� � ���������, ���� �� ����� �������� R|S|H
	int UndoSize; // $ 03.12.2001 IS - ������ ������ undo � ���������
	int UseExternalEditor;
	/* $ 29.11.2000 SVS
	 + Opt.EditorFileSizeLimit - ���������� ���������� ������ �����, �����
	   �������� ����� ����� ������ � ���������������� �������� ���������
	   ����� �� ��������������
	*/
	DWORD FileSizeLimitLo;
	DWORD FileSizeLimitHi;
	/* SVS $ */
	int ShowKeyBar;
	int ShowTitleBar;
	int ShowScrollBar;
};
/* IS $ */

/* $ 29.03.2001 IS
     ��� ������� ������� "���������" ��������� ��� ��������� ���������
*/
struct ViewerOptions
{
	int TabSize;
	int AutoDetectTable;
	int ShowScrollbar;     // $ 18.07.2000 tran ���� �������� ��� viewer
	int ShowArrows;
	int PersistentBlocks; // $ 14.05.2002 VVM ���������� ����� �� ������
	int ViewerIsWrap; // (Wrap|WordWarp)=1 | UnWrap=0
	int ViewerWrap; // Wrap=0|WordWarp=1
	int SaveViewerPos;
	int SaveViewerShortPos;
	int UseExternalViewer;
	int ShowKeyBar; // $ 15.07.2000 tran + ShowKeyBar
	int AnsiTableAsDefault;
	int ShowTitleBar;
};
/* IS $ */

// "�������"
struct PoliciesOptions
{
	int DisabledOptions;  // ������������� ���� ������������
	int ShowHiddenDrives; // ���������� ������� ���������� �����
};

struct DialogsOptions
{
	int   EditBlock;          // ���������� ����� � ������� �����
	int   EditHistory;        // ��������� � �������?
	int   AutoComplete;       // ��������� ��������������?
	int   EULBsClear;         // = 1 - BS � �������� ��� UnChanged ������ ������� ����� ������ �����, ��� � Del
	int   SelectFromHistory;  // = 0 then (ctrl-down � ������ � �������� ������ �������������� �� ����� ������� ������)
	DWORD EditLine;           // ����� ���������� � ������ ����� (������ ��� ����... ��������� ��������� ����������)
	int   MouseButton;        // ���������� ���������� ������/����� ������ ���� ��� ������ �������� ���� �������
	int   DelRemovesBlocks;
	int   CBoxMaxHeight;      // ������������ ������ ������������ ������ (�� ���������=8)
};

struct NowellOptions
{
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

struct ScreenSizes
{
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

struct LoadPluginsOptions
{
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

struct FindFileOptions
{
	int FindFolders;
	int FindSymLinks;
	int CollectFiles;
	int FileSearchMode;
	int UseFilter;
	char SearchInFirstSize[NM];
};

struct TreeOptions
{
	int LocalDisk;         // ������� ���� ��������� ����� ��� ��������� ������
	int NetDisk;           // ������� ���� ��������� ����� ��� ������� ������
	int NetPath;           // ������� ���� ��������� ����� ��� ������� �����
	int RemovableDisk;     // ������� ���� ��������� ����� ��� ������� ������
	int MinTreeCount;      // ����������� ���������� ����� ��� ���������� ������ � �����.
	int AutoChangeFolder;  // ��������� ����� ��� ����������� �� ������
	DWORD TreeFileAttr;      // �������� �������� ��� ������-�������
};

struct CopyMoveOptions
{
	int UseSystemCopy;         // ������������ ��������� ������� �����������
	int CopyOpened;            // ���������� �������� �� ������ �����
	int CopyShowTotal;         // �������� ����� ��������� �����������
	int MultiCopy;             // "��������� �����������������/�����������/�������� ������"
	DWORD CopySecurityOptions; // ��� �������� Move - ��� ������ � ������ "Copy access rights"
	int CopyTimeRule;          // $ 30.01.2001 VVM  ���������� ����� �����������,���������� ����� � ������� ��������
};

struct Options
{
	int Clock;
	int Mouse;
	int ShowKeyBar;
	int ScreenSaver;
	int ScreenSaverTime;
	int UsePromptFormat;
	char PromptFormat[80];
	int AltGr;
	int UseVk_oem_x;
	int InactivityExit;
	int InactivityExitTime;
	int ShowHidden;
	int Highlight;
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
	int DeleteToRecycleBin;         // ������� � �������?
	int DeleteToRecycleBinKillLink; // ����� ��������� ����� � ������� ������� ��������� ��������.
	int WipeSymbol; // ������ ����������� ��� "ZAP-��������"

	struct CopyMoveOptions CMOpt;

	/* IS $ */
	/* $ 07.12.2001 IS ����� �������� ���������� ��������� �� ���� ����� */
	int MultiMakeDir;
	/* IS $ */
	int CreateUppercaseFolders;
	int UseRegisteredTypes;

	int ViewerEditorClock;
	int OnlyEditorViewerUsed; // =1, ���� ����� ��� /e ��� /v
	int SaveViewHistory;
	int ViewHistoryCount;

	char ExternalEditor[NM];
	struct EditorOptions EdOpt;
	char ExternalViewer[NM];
	struct ViewerOptions ViOpt;


	char WordDiv[256]; // $ 03.08.2000 SVS �������������� ���� �� �������
	char QuotedSymbols[32];
	DWORD QuotedName;
	int AutoSaveSetup;
	int SetupArgv; // ���������� ��������� � ���������� ����
	int ChangeDriveMode;
	int ChangeDriveDisconnetMode;

	int SaveHistory;
	int HistoryCount;
	int SaveFoldersHistory;
	int SavePluginFoldersHistory;
	int FoldersHistoryCount;
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
	int ShowMenuScrollbar; // $ 29.06.2000 SVS �������� ������� ������ Scroll Bar � ����.
	int ShowScreensNumber;
	int ShowSortMode;
	int ShowMenuBar;
	int FolderDeepScan;
	int FormatNumberSeparators;
	int CleanAscii;
	int NoGraphics;
	char FolderInfoFiles[1024];

	struct Confirmation Confirm;
	struct DizOptions Diz;

	int ShellRightLeftArrowsRule;
	struct PanelOptions LeftPanel;
	struct PanelOptions RightPanel;

	DWORD  AutoUpdateLimit; // ���� ����� ���������� ������������� �� ��������� ������.
	int AutoUpdateRemoteDrive;

	char Language[LANGUAGENAME_SIZE];
	int SmallIcon;
	char RegRoot[NM];
	/* $ 12.09.2000 SVS
	 + Opt.PanelRightClickRule ������ ��������� ������ ������� ����
	   (��� �� ������ Bug#17)
	*/
	int PanelRightClickRule;
	/* SVS $*/
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
	int CASRule; // 18.12.2003 - ������� ��������� ����� � ������ CAS (������� #1).
	/* $ 24.09.2000 SVS
	 + Opt.CmdHistoryRule ������ ��������� Esc ��� ��������� ������:
	    =1 - �� �������� ��������� � History, ���� ����� Ctrl-E/Ctrl/-X
	         ������ ESC (��������� - ��� VC).
	    =0 - ��������� ��� � ���� - �������� ��������� � History
	*/
	int CmdHistoryRule;
	/* SVS $*/
	DWORD ExcludeCmdHistory;
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
	DWORD DisableMacro; // ��������� /m ��� /ma ��� /m....

	int IgnoreErrorBadPathName;

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
	/*$ 08.02.2001 SKV
	  ���������� ������ ��� ������ Far'������ �������
	  �� ����������� ���������������� �������� � ��� �����������.
	*/
	int ConsoleDetachKey;
	/* SKV$*/

	int UsePrintManager;

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
	int MsWheelDeltaHelp;
	// �������������� ���������
	int MsHWheelDelta;
	int MsHWheelDeltaView;
	int MsHWheelDeltaEdit;
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
	int ExecuteShowErrorMessage;
	int ExecuteUseAppPath;
	int ExecuteFullTitle;
	char ExecuteBatchType[256];

	DWORD PluginMaxReadData;
	int UseNumPad;
	int UseUnicodeConsole;
	int ScanJunction;

	DWORD ShowTimeoutDelFiles; // ������ � �������� �������� (� ms)
	DWORD ShowTimeoutDACLFiles;
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
	struct TreeOptions Tree;
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

enum enumHighlightDataColor
{
	HIGHLIGHTCOLOR_NORMAL = 0,
	HIGHLIGHTCOLOR_SELECTED,
	HIGHLIGHTCOLOR_UNDERCURSOR,
	HIGHLIGHTCOLOR_SELECTEDUNDERCURSOR,

	HIGHLIGHTCOLORTYPE_FILE = 0,
	HIGHLIGHTCOLORTYPE_MARKCHAR = 1,
};

struct HighlightDataColor
{
	WORD Color[2][4];  // [0=file, 1=mark][0=normal,1=selected,2=undercursor,3=selectedundercursor]; if HIBYTE == 0xFF then transparent
	WORD MarkChar;
};

#endif // __FARSTRUCT_HPP__
