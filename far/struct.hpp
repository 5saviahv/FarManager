#ifndef __FARSTRUCT_HPP__
#define __FARSTRUCT_HPP__
/*
struct.hpp

�� ������ᨬ� �������� (����� ᮤ�ঠ� ⮫쪮 ����� ⨯�)

*/

/* Revision: 1.13 07.09.2000 $ */

/*
Modify:
  07.09.2000 tran 1.13
   + Config//Current File
  05.09.2000 SVS 1.12
    + ������� CodeQWERTY, ����뢠��� QWERTY-��४���஢騪
    ! � Opt �������� ���� ��६����, ��ᠥ��� QWERTY-��४���஢��
  01.09.2000 tran 1.11
    + Options.PluginsCacheOnly - ��㧨�� ������� ⮫쪮 �� ���
  31.08.2000 SVS
    ! DialogItem.Flags, DialogData.Flags - ⨯ DWORD
    - �� ��࠭���� ⨯ �࠯�.
      ��������� ��६����� Options.ViewerTypeWrap
  12.08.2000 KM 1.09
    ! � �������� DialogItem � DialogData ����� ����, ����祭���
      � union, char *Mask.
  18.08.2000 SVS
    ! struct FarListItems -> struct FarList, � � ᮢᥬ ����⠫�� :-)
  03.08.2000 SVS
    ! WordDiv -> Opt.WordDiv
  03.08.2000 SVS
    + ������� � Options: MainPluginDir - �ᯮ�짮���� �᭮���� ���� ���
      ���᪠ ��������...
  01.08.2000 SVS
    ! ��������� � �������� Dialog*
  26.07.2000 SVS
    + Opt.AutoComplete
  18.07.2000 tran 1.04
    + Opt.ViewerShowScrollBar, Opt.ViewerShowArrows
  15.07.2000 tran
    + �������� ���ਡ�� ������ KeyBar � Viewer - Options::ShowKeyBarViewer
  15.07.2000 SVS
    + Opt.PersonalPluginsPath - ���� ��� ���᪠ ���ᮭ����� ��������
  29.06.2000 SVS
    + �������� ��ਡ�� ������ Scroll Bar � ���� - Options::ShowMenuScrollbar
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

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
};

struct MenuItem
{
  char Name[128];
  unsigned char Selected;
  unsigned char Checked;
  unsigned char Separator;
  char UserData[sizeof(WIN32_FIND_DATA)+NM+10];
  int UserDataSize;
};


struct MenuData
{
  char *Name;
  unsigned char Selected;
  unsigned char Checked;
  unsigned char Separator;
};


struct HMenuData
{
  char *Name;
  int Selected;
  struct MenuData *SubMenu;
  int SubMenuSize;
  char *SubMenuHelp;
};


struct TreeItem
{
  char Name[NM];
  int Last[NM/2];
  int Depth;
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
  int UpdateMode;
  int SetHidden;
  int StartPos;
};

/* $ 05.09.2000 SVS
   ������� CodeQWERTY, ����뢠��� QWERTY-��४���஢騪
*/
struct CodeQWERTY{
  DWORD Flags;       // �������⥫�� 䫠��
  BYTE Table[2][81]; // [0] non-english �㪢�, [1] english �㪢�
  BYTE Rules[3][20]; // 3 �� 10 �ࠢ��:
                    //  [0] "�᫨ �।��騩 ᨬ��� ��⨭᪨�"
                    //  [1] "�᫨ �।��騩 ᨬ��� ����⨭᪨� ᨬ���"
                    //  [2] "�᫨ �।��騩 ᨬ��� �� ���/lat"
};
/* SVS $*/

struct Options
{
  /* $ 03.08.2000 SVS
     TRUE - �ᯮ�짮���� �⠭����� ���� � �᭮��� ��������
  */
  int MainPluginDir;
  /* SVS $*/
  int Clock;
  int ViewerEditorClock;
  int Mouse;
  int ShowKeyBar;
  int ScreenSaver;
  int ScreenSaverTime;
  int DialogsEditHistory;
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

  int SelectFolders;
  int ReverseSort;
  int ClearReadOnly;
  int DeleteToRecycleBin;
  int UseSystemCopy;
  int CopyOpened;
  int CopyShowTotal;
  int CreateUppercaseFolders;
  int UseRegisteredTypes;
  int UseExternalViewer;
  char ExternalViewer[NM];
  int UseExternalEditor;
  char ExternalEditor[NM];
  int SaveViewerPos;
  int EditorExpandTabs;
  int TabSize;
  int EditorPersistentBlocks;
  int EditorDelRemovesBlocks;
  int EditorAutoIndent;
  int EditorAutoDetectTable;
  int EditorCursorBeyondEOL;
  /* $ 03.08.2000 SVS
     ����࠭��⥫� ᫮� �� ॥���
  */
  char WordDiv[80];
  /* SVS $ */
  int ViewerAutoDetectTable;
  int ViewTabSize;
  int SaveEditorPos;
  int SaveHistory;
  int SaveFoldersHistory;
  int SaveViewHistory;
  int AutoSaveSetup;
  int ChangeDriveMode;
  int FileSearchMode;
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
    �������� ��ਡ�� ������ Scroll Bar � ����.
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
  char Language[80];
  char HelpLanguage[80];
  int SmallIcon;
  char RegRoot[NM];
  /* $ 15.07.2000 SVS
    + ���� ��� ���᪠ ���ᮭ����� ��������, ����让 ࠧ��� ��-�� ⮣�,
      �� ����� ����� ����� �⥢�� ����...
  */
  char PersonalPluginsPath[1024];
  /* SVS $*/
  /* $ 18.07.2000 tran
    + ��� ����஥� ��� viewer*/
  int ViewerShowScrollbar;
  int ViewerShowArrows;
  /* tran 18.07.2000 $ */
  /* $ 26.07.2000 SVS
     ����襭�� ��� �㭪樨 ��⮧����襭�� � ��ப�� ����� � ��������
     ������ History
  */
  int AutoComplete;
  /* SVS $*/
  /* $ 31.08.2000 SVS
     ��������� ��६����� Options.ViewerTypeWrap
  */
  int ViewerTypeWrap;
  /* SVS $*/
  /* $ 01.09.2000 tran
     seting by '/co' switch, not saved in registry. */
  int PluginsCacheOnly;
  /* tran $ */
  /* $ 05.09.2000 SVS
     � Opt �������� ���� ��६����, ��ᠥ��� QWERTY-��४���஢��
  */
  struct CodeQWERTY QWERTY;
  /* SVS $*/
};


// for class History
struct HistoryRecord
{
  char Name[512];
  char Title[32];
  int Type;
};


// for class Grabber
struct GrabberArea
{
  int X1,Y1,X2,Y2;
  int CurX,CurY;
};


// for class Editor
struct EditorUndoData
{
  int Type;
  int UndoNext;
  int StrPos;
  int StrNum;
  char *Str;
};


/* $ 01.08.2000 SVS
  � ������� DialogI* ��������:
  union {
    unsigned int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
  } Addons;

*/
// for class Dialog
/*
����뢠�� ���� ����� ������� - ����७�� �।�⠢�����.
��� �������� �� FarDialogItem (�� �᪫�祭��� ObjPtr)
*/
/* $ 12.08.2000 KM
   �������⥫쭮� ����, ᮤ�ঠ饥 ���� �����
*/
struct DialogItem
{
  unsigned char Type;
  unsigned char X1,Y1,X2,Y2;
  unsigned char Focus;
  union {
    unsigned int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
  };
  DWORD Flags;
  unsigned char DefaultButton;
  char Data[512];
  void *ObjPtr;
};


/*
����뢠�� ���� ����� ������� - ��� ᮪�饭�� ��ꥬ��
������� �������祭� ������� InitDialogItem (�. "Far PlugRinG
Russian Help Encyclopedia of Developer")
*/
struct DialogData
{
  unsigned char Type;
  unsigned char X1,Y1,X2,Y2;
  unsigned char Focus;
  union {
    unsigned int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
  };
  DWORD Flags;
  unsigned char DefaultButton;
  char *Data;
};
/* SVS $*/
/* KM $*/

// for class FileList
struct FileListItem
{
  char Selected;
  char PrevSelected;
  char ShowFolderSize;
  char ShortNamePresent;
  unsigned char Color,SelColor,CursorColor,CursorSelColor;
  unsigned char MarkChar;
  DWORD UnpSizeHigh;
  DWORD UnpSize;
  DWORD PackSizeHigh;
  DWORD PackSize;
  DWORD NumberOfLinks;
  DWORD UserFlags;
  DWORD UserData;

  FILETIME WriteTime;
  FILETIME CreationTime;
  FILETIME AccessTime;

  DWORD FileAttr;
  int Position;
  int SortGroup;
  char *DizText;
  char DeleteDiz;
  char Owner[40];
  char Name[NM];
  char ShortName[80];
  char **CustomColumnData;
  int CustomColumnNumber;
};


// for class FileList
struct PluginsStackItem
{
  HANDLE hPlugin;
  char HostFile[NM];
  int Modified;
  int PrevViewMode;
  int PrevSortMode;
  int PrevSortOrder;
};


// for class FileList
struct PrevDataItem
{
  struct FileListItem *PrevListData;
  long PrevFileCount;
  char PrevName[NM];
};


// for class KeyMacro
struct MacroRecord
{
  int Key;
  int *Buffer;
  int BufferSize;
  int Mode;
  int DisableOutput;
  int EmptyCommandLine;
  int NotEmptyCommandLine;
  int RunAfterStart;
};


struct PluginHandle
{
  HANDLE InternalHandle;
  int PluginNumber;
};


// for class GroupSort
struct GroupSortData
{
  char Masks[256];
  int Group;
};


// for class PanelFilter
struct FilterDataRecord
{
  char Title[128];
  char Masks[256];
  int LeftPanelInclude;
  int LeftPanelExclude;
  int RightPanelInclude;
  int RightPanelExclude;
};


// for class HighlightFiles
struct HighlightData
{
  char Masks[256];
  unsigned int IncludeAttr;
  unsigned int ExcludeAttr;
  unsigned int Color,SelColor,CursorColor,CursorSelColor,MarkChar;
};


// for class DizList
struct DizRecord
{
  char *DizText;
  int Deleted;
};

// for class Edit
struct ColorItem
{
  int StartPos;
  int EndPos;
  int Color;
};


//for class Panel
struct PanelViewSettings
{
  unsigned int ColumnType[20];
  int ColumnWidth[20];
  int ColumnCount;
  unsigned int StatusColumnType[20];
  int StatusColumnWidth[20];
  int StatusColumnCount;
  int FullScreen;
  int AlignExtensions;
  int FolderUpperCase;
  int FileLowerCase;
  int FileUpperToLowerCase;
  int CaseSensitiveSort;
};

#endif // __FARSTRUCT_HPP__
