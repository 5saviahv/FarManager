#ifndef __FARSTRUCT_HPP__
#define __FARSTRUCT_HPP__
/*
struct.hpp

�� ������ᨬ� �������� (����� ᮤ�ঠ� ⮫쪮 ����� ⨯�)

*/

/* Revision: 1.46 15.03.2001 $ */

/*
Modify:
  15.03.2001 SVS
   + Opt.Confirm.RemoveConnection - ���⢥ত���� ��� 㤠����� ��������� ��᪮�
  12.03.2001 SVS
   + Opt.DeleteSymbolWipe -> Opt.WipeSymbol
  12.03.2001 SVS
   + Opt.DeleteSymbolWipe ᨬ��� �������⥫� ��� "ZAP-����樨"
  27.02.2001 SVS
   + EditorOptions.CharCodeBase - ���� �뢮�� ���� ᨬ����
  26.02.2001 VVM
   + Opt.ExceptCallDebugger - ��뢠�� �������� ��� ���
   ! Opt.ExceptRules - ��⮢�� ��᪠
  21.02.2001 IS
   ! ����� ����஥� ।���� ��॥堫� � EditorOptions
  12.02.2001 SKV
   + ConsoleDetachKey. ������ ����񣨢���� ���᮫� �� ��
     ���������饣� ����� � ��� ����饭����.
  11.02.2001 SVS
   ! ��������� ������� DialogItem � DialogData (DIF_VAREDIT)
   ! HighlightData.Masks - ᤥ��� ��뫪�� (DIF_VAREDIT)
   ! GroupSortData.Masks - ᤥ��� ��뫪�� (DIF_VAREDIT)
   ! FilterDataRecord.Masks - ᤥ��� ��뫪�� (DIF_VAREDIT)
  11.02.2001 SVS
   ! ��������� � MenuItem.
  09.02.2001 IS
   + RightSelectedFirst, LeftSelectedFirst;
   + Confirmation.Esc
  30.01.2001 VVM
   + �����뢠�� �६� ����஢����,��⠢襥�� �६� � �।��� ᪮����.
      ������ �� ����஥� � ॥��� CopyTimeRule
  22.01.2001 SVS
   + Opt.CursorSize[2] - ������ ����� ���� :-)
  19.01.2001 SVS
   + Opt.MacroReuseRules - ����୮� �ᯮ�짮�����.
  09.01.2001 SVS
   + Opt.ShiftsKeyRules - �ࠢ��� �� ��� �롮� ��堭���� �࠭��樨
     Alt-�㪢� ��� ����⨭᪨� �㪮��� � ᨬ����� "`-=[]\;',./" �
     ����䨪��ࠬ� Alt-, Ctrl-, Alt-Shift-, Ctrl-Shift-, Ctrl-Alt-
  28.12.2000 SVS
   + Opt.HotkeyRules - �ࠢ��� �� ��� �롮� ��堭���� �⪥��
  21.12.2000 SVS
   ! ������� MacroRecord ��७�ᥭ� � macro.hpp
  08.12.2000 SVS
   ! ��������� � ������� DialogItem.
  29.11.2000 SVS
   + Opt.EditorReadOnlyLock - ����� 䠩� �� ����⨨ � ।����, �᫨
     �� ����� ��ਡ��� R|S|H
   + Opt.EditorFileSizeLimit - �������쭮 �����⨬� ࠧ��� 䠩��, ��᫥
     ���ண� �㤥� �뤠� ������ � 楫�ᮮ�ࠧ���� ������ ���������
     䠩�� �� ।���஢����
  28.11.2000 SVS
   + Opt.EditorF7Rules - �ࠢ��� �� ��� ���᪠ � ।����
  27.11.2000 SVS
   + Opt.ExceptRules - �ࠢ��� �� ��� �맮�� �᪫�祭��
  25.11.2000 IS
   + ����࠭��⥫� ᫮� �� ॥��� ��� �㭪樨 Xlat (WordDivForXlat)
  24.11.2000 SVS
   ! XLat.Rules - 3 �� 30 �ࠢ��, ����� 10
   + �ࠢ��� �� ��� ��⠭���� ��ਡ�⮢ �� ��⠫���
  04.11.2000 SVS
   ! XLat - �� �� �⭮���� � XLat - � ���� ��������.
   + XLat - ���������� ����ୠ⨢��� ������:
       XLatAltEditorKey, XLatAltCmdLineKey, XLatAltDialogKey;
  20.10.2000 SVS
   + Opt.PanelCtrlFRule
      Panel/CtrlFRule � ॥��� - ������ ��������� Ctrl-F
      �᫨ = 0, � �⠬����� 䠩� ��� ����, ���� - � ��⮬
      �⮡ࠦ���� �� ������
  17.10.2000 SVS
   ! WordDiv ����� ࠧ��� 256;
  10.10.2000 SVS
   + Opt.EditorBSLikeDel
  27.09.2000 SVS
   + Opt.HelpURLRules - =0 �⪫���� ����������� ����᪠ URL-�ਫ������
   + Opt.AllCtrlAltShiftRule - ��⮢� 䫠��, ������ ��������� Ctrl-Alt-Shift
     ��� ��⠭����� - �㭪�� ����祭�:
     0 - Panel
     1 - Edit
     2 - View
     3 - Help
     4 - Dialog
  24.09.2000 SVS
   + Opt.MaxPositionCache - ������⢮ ����権 � ��� ��࠭����
   + Opt.SaveViewerShortPos - ����������� ����権 �� ��� �� Ctrl-0..9
   + Opt.SaveEditorShortPos - ����������� ����権 � ।���� �� Ctrl-0..9
   + Opt.CmdHistoryRule ������ ��������� Esc ��� ��������� ��ப�.
   + ������ ��� �맮�� �㭪樨 Xlat:
     Opt.XLatEditorKey, Opt.XLatCmdLineKey, Opt.XLatDialogKey
  20.09.2000 SVS
   + Opt.SubstPluginPrefix - 1 = ����⠭�������� ��䨪� �������
     ��� Ctrl-[ � ��� �������
  19.09.2000 SVS
   + Opt.PanelCtrlAltShiftRule ������ ��������� Ctrl-Alt-Shift ��� �������.
  12.09.2000 SVS
   + ��������� ��६����� Options.ViewerWrap
   ! ViewerTypeWrap ��२�������� � ViewerIsWrap
       ���������� Wrap/WWrap/UnWrap �� 2 ��⠢����� -
       ����ﭨ� (Wrap/UnWrap) � ⨯ (Wrap/WWrap)
        ViewerIsWrap  =  UnWrap=0  | ��७��=1
        ViewerWrap    =  Wrap=0    | WordWarp=1
   + Opt.PanelRightClickRule ������ ��������� �ࠢ�� ������ ���
     (�� �� ������ Bug#17)
  11.09.2000 SVS
   + � Opt ��������� ��६����� DlgEULBsClear
     �᫨ = 1, � BS � �������� ��� UnChanged ��ப� 㤠��� ⠪��
     ��ப� ⠪��, ��� � Del
  09.09.2000 SVS 1.14
   + CHAR_INFO *VBuf; � ������ �������
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
  /* $ 09.02.2001 IS
     ��� CheckForEsc
  */
  int Esc;
  /* IS $ */
  int RemoveConnection;
};

struct MenuItem
{
  DWORD Flags;
  char Name[128];
  unsigned char Selected;
  unsigned char Checked;
  unsigned char Separator;
  unsigned char Disabled;
  char  UserData[sizeof(WIN32_FIND_DATA)+NM+10];
  int   UserDataSize;
  char *PtrData;
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
struct CodeXLAT{
  DWORD Flags;       // �������⥫�� 䫠��
  /* $ 05.09.2000 SVS
     � Opt ��������� ������, ��뢠�騥 �㭪�� Xlat
  */
  int XLatEditorKey;
  int XLatCmdLineKey;
  int XLatDialogKey;
  /* SVS $*/
  /* $ 04.11.2000 SVS
     � Opt ��������� ����ୠ⨢�� ������, ��뢠�騥 �㭪�� Xlat
  */
  int XLatAltEditorKey;
  int XLatAltCmdLineKey;
  int XLatAltDialogKey;
  /* SVS $*/
  /* $ 25.11.2000 IS
     ����࠭��⥫� ᫮� �� ॥��� ��� �㭪樨 Xlat
  */
  char WordDivForXlat[256];
  /* IS $ */
  BYTE Table[2][81]; // [0] non-english �㪢�, [1] english �㪢�
  BYTE Rules[3][60]; // 3 �� 30 �ࠢ��:
                    //  [0] "�᫨ �।��騩 ᨬ��� ��⨭᪨�"
                    //  [1] "�᫨ �।��騩 ᨬ��� ����⨭᪨� ᨬ���"
                    //  [2] "�᫨ �।��騩 ᨬ��� �� ���/lat"
};
/* SVS $*/

/* $ 21.02.2001 IS
     ����� �������: ����ன�� ।����
*/
struct EditorOptions
{
  int TabSize;
  int ExpandTabs;
  int PersistentBlocks;
  int DelRemovesBlocks;
  int AutoIndent;
  int AutoDetectTable;
  int CursorBeyondEOL;
  int BSLikeDel;
  int CharCodeBase;
};
/* IS $ */

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

  /* $ 09.02.2001 IS
     ���ﭨ� ०��� "����祭��� ���।"
  */
  int RightSelectedFirst, LeftSelectedFirst;
  /* IS $ */
  int SelectFolders;
  int ReverseSort;
  int ClearReadOnly;
  int DeleteToRecycleBin;
  int WipeSymbol; // ᨬ��� �������⥫� ��� "ZAP-����樨"
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
  int SaveViewerShortPos;
  /* $ 21.02.2001 IS
       ��६���� ��� ।���� ��॥堫� � ᮮ⢥�������� ��������
  */
  struct EditorOptions EdOpt;
  /* IS $ */
  /* $ 29.11.2000 SVS
   + Opt.EditorReadOnlyLock - ����� 䠩� �� ����⨨ � ।����, �᫨
     �� ����� ��ਡ��� R|S|H
  */
  int EditorReadOnlyLock;
  /* SVS $ */
  /* $ 29.11.2000 SVS
   + Opt.EditorFileSizeLimit - �������쭮 �����⨬� ࠧ��� 䠩��, ��᫥
     ���ண� �㤥� �뤠� ������ � 楫�ᮮ�ࠧ���� ������ ���������
     䠩�� �� ।���஢����
  */
  DWORD EditorFileSizeLimitLo;
  DWORD EditorFileSizeLimitHi;
  /* SVS $ */
  /* $ 03.08.2000 SVS
     ����࠭��⥫� ᫮� �� ॥���
  */
  char WordDiv[256];
  /* SVS $ */
  int ViewerAutoDetectTable;
  int ViewTabSize;
  int SaveEditorPos;
  int SaveEditorShortPos;
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
  /* $ 12.09.2000 SVS
     ��������� ��६����� Options.ViewerWrap
     ViewerTypeWrap ��२�������� � ViewerIsWrap
  */
  int ViewerIsWrap; // (Wrap|WordWarp)=1 | UnWrap=0
  int ViewerWrap; // Wrap=0|WordWarp=1
  /* SVS 12.09.2000 $*/
  /* SVS $*/
  /* $ 01.09.2000 tran
     seting by '/co' switch, not saved in registry. */
  int PluginsCacheOnly;
  /* tran $ */
  /* $ 11.09.2000 SVS
     � Opt ��������� ��६����� DlgEULBsClear
     �᫨ = 1, � BS � �������� ��� UnChanged ��ப� 㤠��� ⠪��
     ��ப� ⠪��, ��� � Del
  */
  int DlgEULBsClear;
  /* SVS $*/
  /* $ 12.09.2000 SVS
   + Opt.PanelRightClickRule ������ ��������� �ࠢ�� ������ ���
     (�� �� ������ Bug#17)
  */
  int PanelRightClickRule;
  /* SVS $*/
  /* $ 19.09.2000 SVS
   + Opt.PanelCtrlAltShiftRule ������ ��������� Ctrl-Alt-Shift ��� �������.
  */
  int PanelCtrlAltShiftRule;
  /* SVS $*/
  /* $ 20.10.2000 SVS
    Panel/CtrlFRule � ॥��� - ������ ��������� Ctrl-F
    �᫨ = 0, � �⠬����� 䠩� ��� ����, ���� - � ��⮬
    �⮡ࠦ���� �� ������
  */
  int PanelCtrlFRule;
  /* SVS $*/
  /* $ 27.09.2000 SVS
   + Opt.AllCtrlAltShiftRule - ��⮢� 䫠��, ������ ��������� Ctrl-Alt-Shift
     ��� ��⠭����� - �㭪�� ����祭�:
     0 - Panel
     1 - Edit
     2 - View
     3 - Help
     4 - Dialog
  */
  int AllCtrlAltShiftRule;
  /* SVS $*/
  /* $ 24.09.2000 SVS
   + Opt.CmdHistoryRule ������ ��������� Esc ��� ��������� ��ப�:
      =1 - �� �������� ��������� � History, �᫨ ��᫥ Ctrl-E/Ctrl/-X
           ������ ESC (��������� - ��� VC).
      =0 - ��������� ��� � �뫮 - �������� ��������� � History
  */
  int CmdHistoryRule;
  /* SVS $*/
  /* $ 20.09.2000 SVS
   + Opt.SubstPluginPrefix - 1 = ����⠭�������� ��䨪� �������
     ��� Ctrl-[ � ��� �������
  */
  int SubstPluginPrefix;
  /* SVS $*/
  /* $ 24.09.2000 SVS
   + Opt.MaxPositionCache - ������⢮ ����権 � ��� ��࠭����
  */
  int MaxPositionCache;
  /* SVS $*/
  /* $ 27.09.2000 SVS
   + Opt.HelpURLRules - =0 �⪫���� ����������� ����᪠ URL-�ਫ������
  */
  int HelpURLRules;
  /* SVS $*/
  /* $ 22.11.2000 SVS
   + �ࠢ��� �� ��� ��⠭���� ��ਡ�⮢ �� ��⠫���*/
  int SetAttrFolderRules;
  /* SVS $ */
  /* $ 27.11.2000 SVS
   + Opt.ExceptRules - �ࠢ��� �� ��� �맮�� �᪫�祭�� */
  int ExceptRules;
  /* $ 26.02.2001 VVM
   + Opt.ExceptCallDebugger - ��뢠�� �������� �� �᪫�祭�� */
  int ExceptCallDebugger;
  /* VVM $ */
  /* SVS $ */
  /* $ 28.11.2000 SVS
   + Opt.EditorF7Rules - �ࠢ��� �� ��� ���᪠ � ।���� */
  int EditorF7Rules;
  /* SVS $ */
  /* $ 28.12.2000 SVS
   + Opt.HotkeyRules - �ࠢ��� �� ��� �롮� ��堭���� �⪥�� */
  int HotkeyRules;
  /* SVS $ */
  /* $ 09.01.2001 SVS
   + Opt.ShiftsKeyRules - �ࠢ��� �� ��� �롮� ��堭���� �࠭��樨
     Alt-�㪢� ��� ����⨭᪨� �㪮��� � ᨬ����� "`-=[]\;',./" �
     ����䨪��ࠬ� Alt-, Ctrl-, Alt-Shift-, Ctrl-Shift-, Ctrl-Alt- */
  int ShiftsKeyRules;
  /* SVS $ */
  /* $ 19.01.2001 SVS
   + Opt.MacroReuseRules - �ࠢ��� �� ��� ����୮ �ᯮ�짮����� �����������
     ������ */
  int MacroReuseRules;
  /* SVS $ */
  /* $ 22.01.2001 SVS
   + Opt.CursorSize - ������ ����� ���� :-)
     ������ */
  int CursorSize[2];
  /* SVS $ */
  /* $ 05.09.2000 SVS
     � Opt �������� ���� ��६����, ��ᠥ��� QWERTY-��४���஢��
  */
  struct CodeXLAT XLat;
  /* SVS $*/
  /* $ 30.01.2001 VVM
    + �����뢠�� �६� ����஢����,��⠢襥�� �६� � �।��� ᪮����.
      ������ �� ����஥� � ॥��� CopyTimeRule */
  int CopyTimeRule;
  /* VVM $ */
  /*$ 08.02.2001 SKV
    ��������� ������ ��� ���� Far'��᪮� ���᮫�
    �� ���⥫쭮�� �����ࠪ⨢���� ����� � ��� ����饭����.
  */
  int ConsoleDetachKey;
  /* SKV$*/
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
/* $ 08.12.2000 SVS
   Data "��ꥤ����" � ����� ������ன
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
    CHAR_INFO *VBuf;
  };
  DWORD Flags;
  unsigned char DefaultButton;
  union {
    char Data[512];
    struct {
      DWORD PtrFlags;
      int   PtrLength;
      void *PtrData;
      char  PtrTail[1];
    } Ptr;
  };
  void *ObjPtr;
};
/* SVS $ */

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
    CHAR_INFO *VBuf;
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

struct PluginHandle
{
  HANDLE InternalHandle;
  int PluginNumber;
};


// for class GroupSort
struct GroupSortData
{
  char *Masks;
  int Group;
};


// for class PanelFilter
struct FilterDataRecord
{
  char Title[128];
  char *Masks;
  int LeftPanelInclude;
  int LeftPanelExclude;
  int RightPanelInclude;
  int RightPanelExclude;
};


// for class HighlightFiles
struct HighlightData
{
  char *Masks;
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
