/*
findfile.cpp

����� (Alt-F7)

*/

#include "headers.hpp"
#pragma hdrstop

#include "findfile.hpp"
#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "editor.hpp"
#include "fileview.hpp"
#include "fileedit.hpp"
#include "filelist.hpp"
#include "cmdline.hpp"
#include "chgprior.hpp"
#include "namelist.hpp"
#include "scantree.hpp"
#include "savescr.hpp"
#include "manager.hpp"
#include "scrbuf.hpp"
#include "CFileMask.hpp"
#include "filefilter.hpp"
#include "farexcpt.hpp"

#define DLG_HEIGHT 23
#define DLG_WIDTH 74
#define PARTIAL_DLG_STR_LEN 30
#define CHAR_TABLE_SIZE 5

#define LIST_DELTA  64
static DWORD LIST_INDEX_NONE = (DWORD)-1;

// ������ ��������� ������. ������ �� ������ �������� � ����.
static LPFINDLIST  FindList;
static DWORD       FindListCapacity;
static DWORD       FindListCount;
// ������ �������. ���� ���� ������ � ������, �� FindList->ArcIndex ��������� ����.
static LPARCLIST   ArcList;
static DWORD       ArcListCapacity;
static DWORD       ArcListCount;

static CriticalSection ffCS; // ����� �� ��������� ������ ��������� ������, ���� �� �� ���� ������
static CriticalSection statusCS; // ����� �� ������ � FindMessage/FindFileCount/FindDirCount ���� �� ������ ������


static DWORD FindFileArcIndex;
// ������������ ��� �������� ������ �� ��������� ������.
// ������ �������� �������� � ������ � ���� ��� ��������.
static DWORD FindExitIndex;
enum {FIND_EXIT_NONE, FIND_EXIT_SEARCHAGAIN, FIND_EXIT_GOTO, FIND_EXIT_PANEL};
static int FindExitCode;
//static char FindFileArcName[NM];

static char FindMask[NM],FindStr[SEARCHSTRINGBUFSIZE];
/* $ 30.07.2000 KM
   ��������� ���������� WholeWords ��� ������ �� ������� ����������
*/
static int SearchMode,CmpCase,WholeWords,SearchInArchives,SearchInSymLink,SearchHex;
/* KM $ */
static int FindFoldersChanged;
static int SearchFromChanged;
static int DlgWidth,DlgHeight;
static volatile int StopSearch,PauseSearch,SearchDone,LastFoundNumber,FindFileCount,FindDirCount,WriteDataUsed,PrepareFilesListUsed;
static char FindMessage[200],LastDirName[2*NM];
static int FindMessageReady,FindCountReady,FindPositionChanged;
static char PluginSearchPath[2*NM];
static HANDLE hDlg;
static int RecurseLevel;
static int BreakMainThread;
static int PluginMode;

static HANDLE hPluginMutex;

static int UseAllTables=FALSE,UseDecodeTable=FALSE,UseANSI=FALSE,UseUnicode=FALSE,TableNum=0,UseFilter=0;
static int SearchInFirstIndex=0,EnableSearchInFirst=FALSE;
static __int64 SearchInFirst=_i64(0);
static struct CharTableSet TableSet;

/* $ 01.07.2001 IS
   ������ "����� ������". ������ ��� ����� ������������ ��� �������� �����
   ����� �� ���������� � �������.
*/
static CFileMask FileMaskForFindFile;
/* IS $ */

/* $ 05.10.2003 KM
   ��������� �� ������ ������� ��������
*/
static FileFilter *Filter;
/* KM $*/

int _cdecl SortItems(const void *p1,const void *p2)
{
  PluginPanelItem *Item1=(PluginPanelItem *)p1;
  PluginPanelItem *Item2=(PluginPanelItem *)p2;
  char n1[NM*2],n2[NM*2];
  n1[0]=0;n2[0]=0;
  if (*Item1->FindData.cFileName)
    xstrncpy(n1,Item1->FindData.cFileName, sizeof(n1)-1);
  if (*Item2->FindData.cFileName)
    xstrncpy(n2,Item2->FindData.cFileName, sizeof(n1)-1);
  *(PointToName(n1))=0;
  *(PointToName(n2))=0;
  return LocalStricmp(n1,n2);
}

LONG_PTR WINAPI FindFiles::MainDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2)
{
  Dialog* Dlg=(Dialog*)hDlg;
  char *FindText=MSG(MFindFileText),*FindHex=MSG(MFindFileHex),*FindCode=MSG(MFindFileCodePage);
  char DataStr[NM*2];

  switch(Msg)
  {
    case DN_INITDIALOG:
    {
      /* $ 21.09.2003 KM
         ������������ ��������� ������ ����� �������� ������
         � ����������� �� Dlg->Item[13].Selected
      */
      if (Dlg->Item[13].Selected) // [ ] Search for hexadecimal code
      {
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,FALSE);
      }
      else
      {
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,TRUE);
      }

      Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,5,1);
      Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,6,1);
      /* KM $ */

      unsigned int W=Dlg->Item[7].X1-Dlg->Item[4].X1-5;
      if (strlen((Dlg->Item[13].Selected?FindHex:FindText))>W)
      {
        xstrncpy(DataStr,(Dlg->Item[13].Selected?FindHex:FindText),W-3);
        DataStr[W-4]=0;
        strcat(DataStr,"...");
      }
      else
        xstrncpy(DataStr,(Dlg->Item[13].Selected?FindHex:FindText),sizeof(DataStr)-1);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,4,(LONG_PTR)DataStr);

      W=Dlg->Item[0].X2-Dlg->Item[7].X1-3;
      if (strlen(FindCode)>W)
      {
        xstrncpy(DataStr,FindCode,W-3);
        DataStr[W-4]=0;
        strcat(DataStr,"...");
      }
      else
        xstrncpy(DataStr,FindCode,sizeof(DataStr)-1);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,7,(LONG_PTR)DataStr);

      /* ��������� ����������� ����� ���������� */
      UseAllTables=Opt.CharTable.AllTables;
      UseANSI=Opt.CharTable.AnsiTable;
      UseUnicode=Opt.CharTable.UnicodeTable;
      UseDecodeTable=((UseAllTables==0) && (UseUnicode==0) && (UseANSI==0) && (Opt.CharTable.TableNum>0));
      if (UseDecodeTable)
        TableNum=Opt.CharTable.TableNum-1;
      else
        TableNum=0;
      /* -------------------------------------- */

      if (UseAllTables)
        xstrncpy(TableSet.TableName,MSG(MFindFileAllTables),sizeof(TableSet.TableName)-1);
      else if (UseUnicode)
        xstrncpy(TableSet.TableName,"Unicode",sizeof(TableSet.TableName)-1);
      /* $ 20.09.2003 KM
         ������� ��������� ANSI �������
      */
      else if (UseANSI)
      {
        GetTable(&TableSet,TRUE,TableNum,UseUnicode);
        xstrncpy(TableSet.TableName,MSG(MGetTableWindowsText),sizeof(TableSet.TableName)-1);
      }
      /* KM $ */
      else if (!UseDecodeTable)
        xstrncpy(TableSet.TableName,MSG(MGetTableNormalText),sizeof(TableSet.TableName)-1);
      else
        PrepareTable(&TableSet,TableNum,TRUE);
      RemoveChar(TableSet.TableName,'&',TRUE);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,8,(LONG_PTR)TableSet.TableName);

      FindFoldersChanged = FALSE;
      SearchFromChanged=FALSE;

      if (Dlg->Item[23].Selected==1)
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,32,TRUE);
      else
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,32,FALSE);

      return TRUE;
    }
    case DN_LISTCHANGE:
    {
      if (Param1==8)
      {
        /* $ 20.09.2003 KM
           ������� ��������� ANSI �������
        */
        UseAllTables=(Param2==0);
        UseANSI=(Param2==3);
        UseUnicode=(Param2==4);
        UseDecodeTable=(Param2>=(CHAR_TABLE_SIZE+1));
        TableNum=(int)Param2-(CHAR_TABLE_SIZE+1);
        if (UseAllTables)
          xstrncpy(TableSet.TableName,MSG(MFindFileAllTables),sizeof(TableSet.TableName)-1);
        else if (UseUnicode)
          xstrncpy(TableSet.TableName,"Unicode",sizeof(TableSet.TableName)-1);
        else if (UseANSI)
        {
          GetTable(&TableSet,TRUE,TableNum,UseUnicode);
          xstrncpy(TableSet.TableName,MSG(MGetTableWindowsText),sizeof(TableSet.TableName)-1);
        }
        else if (!UseDecodeTable)
          xstrncpy(TableSet.TableName,MSG(MGetTableNormalText),sizeof(TableSet.TableName)-1);
        else
          PrepareTable(&TableSet,TableNum,TRUE);
        /* KM $ */
      }
      return TRUE;
    }
    /* 22.11.2001 VVM
      ! ������������������� FindFolders ��� ����� ������.
        �� ������ ���� �� ������ ���� ��������� ������� */
    case DN_BTNCLICK:
    {
      Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
      char SearchFromRoot[128];
      struct FarDialogItemData ItemData;

      /* $ 23.11.2001 KM
         - ����! ������ ���� � ���� ������� :-(
           ��������� �������� �������� � �����������.
      */
      /* $ 22.11.2001 KM
         - ������ ������, ��� DN_BTNCLICK �������� �� ����� "�������������"
           ����������, ������ � �������� ������� ��-�������, ������� �������
           ������� ENTER �� ������� Find ��� Cancel �� � ���� �� ���������.
      */
      if (Param1==31 || Param1==35) // [ Find ] ��� [ Cancel ]
        return FALSE;
      else if (Param1==32) // [ Drive ]
      {
        IsRedrawFramesInProcess++;
        ActivePanel->ChangeDisk();
        // �� ��� �, ��� ����� ����� ������ ��������� ������
        // ����� ����� ��������.
        //FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
        FrameManager->ResizeAllFrame();
        IsRedrawFramesInProcess--;

        PrepareDriveNameStr(SearchFromRoot,Min((int)sizeof(SearchFromRoot),(int)PARTIAL_DLG_STR_LEN));
        ItemData.PtrLength=(int)strlen(SearchFromRoot);
        ItemData.PtrData=SearchFromRoot;
        Dialog::SendDlgMessage(hDlg,DM_SETTEXT,23,(LONG_PTR)&ItemData);
        PluginMode=CtrlObject->Cp()->ActivePanel->GetMode()==PLUGIN_PANEL;
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,PluginMode?FALSE:TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,20,PluginMode?FALSE:TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,21,PluginMode?FALSE:TRUE);
      }
      else if (Param1==33) // Filter
        Filter->FilterEdit();
      else if (Param1==34) // Advanced
        AdvancedDialog();
      else if (Param1>=20 && Param1<=26)
      {
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,32,Param1==23?TRUE:FALSE);
        SearchFromChanged=TRUE;
      }
      else if (Param1==15) // [ ] Search for folders
        FindFoldersChanged = TRUE;
      else if (Param1==13) // [ ] Search for hexadecimal code
      {
        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

        /* $ 21.09.2003 KM
           ������������ ��������� ������ ����� �������� ������
           � ����������� �� �������������� �������� hex mode
        */
        int LenDataStr=sizeof(DataStr);
        if (Param2)
        {
          Transform((unsigned char *)DataStr,LenDataStr,Dlg->Item[5].Data,'X');
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,6,(LONG_PTR)DataStr);

          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,FALSE);
        }
        else
        {
          Transform((unsigned char *)DataStr,LenDataStr,Dlg->Item[6].Data,'S');
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,5,(LONG_PTR)DataStr);

          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,TRUE);
        }
        /* KM $ */

        unsigned int W=Dlg->Item[7].X1-Dlg->Item[4].X1-5;
        if (strlen((Param2?FindHex:FindText))>W)
        {
          xstrncpy(DataStr,(Param2?FindHex:FindText),W-3);
          DataStr[W-4]=0;
          strcat(DataStr,"...");
        }
        else
          xstrncpy(DataStr,(Param2?FindHex:FindText),sizeof(DataStr)-1);
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,4,(LONG_PTR)DataStr);

        if (strlen(DataStr)>0)
        {
          int UnchangeFlag=(int)Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,5,-1);
          Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,6,UnchangeFlag);
        }

        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
      }
      else if (Param1==29) // [ ] Advanced options
        EnableSearchInFirst=(int)Param2;
      return TRUE;
      /* KM $ */
      /* KM $ */
    }
    case DN_EDITCHANGE:
    {
      FarDialogItem &Item=*reinterpret_cast<FarDialogItem*>(Param2);

      if (Param1==5)
      {
        if(!FindFoldersChanged)
        // ������ "���������� �����"
        {
          xstrncpy(DataStr,Item.Data.Data,sizeof(DataStr)-1);

          BOOL Checked = (*DataStr)?FALSE:Opt.FindOpt.FindFolders;
          if (Checked)
            Dialog::SendDlgMessage(hDlg, DM_SETCHECK, 15, BSTATE_CHECKED);
          else
            Dialog::SendDlgMessage(hDlg, DM_SETCHECK, 15, BSTATE_UNCHECKED);
        }
      }
      return TRUE;
    }
    /* VVM $ */
    /* $ 15.10.2003 KM
        ���������� "�������" �������
    */
    case DN_HOTKEY:
    {
      if (Param1==4)
      {
        if (Dlg->Item[13].Selected)
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,6,0);
        else
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,5,0);
        return FALSE;
      }
    }
    /* KM $ */
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}


static void ShowTruncateMessage(int IDMField,int MaxSize)
{
  char Buf1 [128];
  char Buf2 [128];

  *Buf1='\'';
  xstrncpy (Buf1+1, MSG(IDMField), sizeof(Buf1)-3);
  strcat(Buf1,"'");
  RemoveHighlights(Buf1);
  sprintf (Buf2, MSG(MEditInputSize2), MaxSize);
  Message(MSG_WARNING,1,MSG(MWarning),MSG(MEditInputSize1),Buf1,Buf2,MSG(MOk));
}


FindFiles::FindFiles()
{
  _ALGO(CleverSysLog clv("FindFiles::FindFiles()"));
  static char LastFindMask[NM]="*.*",LastFindStr[SEARCHSTRINGBUFSIZE];
  // ����������� ��������� � ����������� ����������
  static char SearchFromRoot[128];
  /* $ 30.07.2000 KM
     ��������� ���������� LastWholeWords ��� ������ �� ������� ����������
  */
  static int LastCmpCase=0,LastWholeWords=0,LastSearchInArchives=0,LastSearchInSymLink=-1,LastSearchHex=0;
  /* KM $ */
  int I;

  // �������� ������ �������
  Filter=new FileFilter(CtrlObject->Cp()->ActivePanel,FFT_FINDFILE);

  CmpCase=LastCmpCase;
  WholeWords=LastWholeWords;
  SearchInArchives=LastSearchInArchives;
  SearchHex=LastSearchHex;
  if(LastSearchInSymLink == -1)
    LastSearchInSymLink=Opt.ScanJunction;
  if (!RegVer)
    LastSearchInSymLink=0;
  SearchInSymLink=LastSearchInSymLink;
  SearchMode=Opt.FindOpt.FileSearchMode;
  UseFilter=Opt.FindOpt.UseFilter;

  // ������ ���� �������, � ������� ����������� ����������� ������ � �����
  SearchInFirstIndex=Opt.FindOpt.SearchInFirst;

  // ��-��������� �������������� ��������� �� ������������
  EnableSearchInFirst=FALSE;

  xstrncpy(FindMask,LastFindMask,sizeof(FindMask)-1);
  xstrncpy(FindStr,LastFindStr,sizeof(FindStr)-1);
  BreakMainThread=0;

  xstrncpy(SearchFromRoot,MSG(MSearchFromRootFolder),sizeof(SearchFromRoot)-1);
  SearchFromRoot[sizeof(SearchFromRoot)-1]=0;

  FarList TableList;
  FarListItem *TableItem=(FarListItem *)xf_malloc(sizeof(FarListItem)*CHAR_TABLE_SIZE);
  TableList.Items=TableItem;
  TableList.ItemsNumber=CHAR_TABLE_SIZE;

  memset(TableItem,0,sizeof(FarListItem)*CHAR_TABLE_SIZE);
  xstrncpy(TableItem[0].Text,MSG(MFindFileAllTables),sizeof(TableItem[0].Text)-1);
  TableItem[1].Flags=LIF_SEPARATOR;
  xstrncpy(TableItem[2].Text,MSG(MGetTableNormalText),sizeof(TableItem[2].Text)-1);
  xstrncpy(TableItem[3].Text,MSG(MGetTableWindowsText),sizeof(TableItem[3].Text)-1);
  xstrncpy(TableItem[4].Text,"Unicode",sizeof(TableItem[4].Text)-1);

  for (I=0;;I++)
  {
    CharTableSet cts;
    int RetVal=FarCharTable(I,(char *)&cts,sizeof(cts));
    if (RetVal==-1)
      break;

    if (I==0)
    {
      TableItem=(FarListItem *)xf_realloc(TableItem,sizeof(FarListItem)*(CHAR_TABLE_SIZE+1));
      if (TableItem==NULL)
        return;
      memset(&TableItem[CHAR_TABLE_SIZE],0,sizeof(FarListItem));
      TableItem[CHAR_TABLE_SIZE].Flags=LIF_SEPARATOR;
      TableList.Items=TableItem;
      TableList.ItemsNumber++;
    }

    TableItem=(FarListItem *)xf_realloc(TableItem,sizeof(FarListItem)*(I+CHAR_TABLE_SIZE+2));
    if (TableItem==NULL)
      return;
    memset(&TableItem[I+CHAR_TABLE_SIZE+1],0,sizeof(FarListItem));
    xstrncpy(TableItem[I+CHAR_TABLE_SIZE+1].Text,cts.TableName,sizeof(TableItem[I+CHAR_TABLE_SIZE+1].Text)-1);
    RemoveChar(TableItem[I+CHAR_TABLE_SIZE+1].Text,'&',TRUE);
    TableList.Items=TableItem;
    TableList.ItemsNumber++;
  }

  FindList = NULL;
  ArcList = NULL;
  hPluginMutex=CreateMutex(NULL,FALSE,NULL);

  do
  {
    ClearAllLists();

    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
    PluginMode=ActivePanel->GetMode()==PLUGIN_PANEL && ActivePanel->IsVisible();

    PrepareDriveNameStr(SearchFromRoot,Min((int)sizeof(SearchFromRoot),(int)PARTIAL_DLG_STR_LEN));

    const char *MasksHistoryName="Masks",*TextHistoryName="SearchText";
    const char *HexMask="HH HH HH HH HH HH HH HH HH HH HH"; // HH HH HH HH HH HH HH HH HH HH HH HH";

/*
    000000000011111111112222222222333333333344444444445555555555666666666677777777
    012345678901234567890123456789012345678901234567890123456789012345678901234567
00  """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
01  "  +----------------------------- Find file ------------------------------+  "
02  "  � A file mask or several file masks:                                   �  "
03  "  � *.*#################################################################.�  "
04  "  �----------------------------------+-----------------------------------�  "
05  "  � Containing text:                 � Using character table:            �  "
06  "  � ################################.� Windows text#####################.�  "
07  "  �----------------------------------+-----------------------------------�  "
08  "  � [ ] Case sensitive               � [ ] Search in archives            �  "
09  "  � [ ] Whole words                  � [x] Search for folders            �  "
10  "  � [ ] Search for hex               � [x] Search in symbolic links      �  "
11  "  �----------------------------------+-----------------------------------�  "
12  "  � Select place to search                                               �  "
13  "  � ( ) In all non-removable drives    (.) From the current folder       �  "
14  "  � ( ) In all local drives            ( ) The current folder only       �  "
15  "  � ( ) In PATH folders                ( ) Selected folders              �  "
    "  � ( ) From the root of C:                                              �  "
16  "  �----------------------------------------------------------------------�  "
17  "  � [ ] Use filter                     [ ] Advanced options              �  "
18  "  �----------------------------------------------------------------------�  "
19  "  �      [ Find ]  [ Drive ]  [ Filter ]  [ Advanced ]  [ Cancel ]       �  "
20  "  +----------------------------------------------------------------------+  "
21  "                                                                            "
22  """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
*/

    /* $ 30.07.2000 KM
       �������� ����� checkbox "Whole words" � ������ ������
    */
    static struct DialogData FindAskDlgData[]=
    {
      /* 00 */DI_DOUBLEBOX,3,1,DLG_WIDTH,DLG_HEIGHT-2,0,0,0,0,(char *)MFindFileTitle,
      /* 01 */DI_TEXT,5,2,0,2,0,0,0,0,(char *)MFindFileMasks,
      /* 02 */DI_EDIT,5,3,72,3,1,(DWORD_PTR)MasksHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,0,"",
      /* 03 */DI_TEXT,3,4,0,4,0,0,DIF_BOXCOLOR|DIF_SEPARATOR2,0,"",
      /* 04 */DI_TEXT,5,5,0,5,0,0,0,0,"",
      /* 05 */DI_EDIT,5,6,36,6,0,(DWORD_PTR)TextHistoryName,DIF_HISTORY,0,"",
      /* 06 */DI_FIXEDIT,5,6,36,6,0,(DWORD_PTR)HexMask,DIF_MASKEDIT,0,"",
      /* 07 */DI_TEXT,40,5,0,5,0,0,0,0,"",
      /* 08 */DI_COMBOBOX,40,6,72,18,0,0,DIF_DROPDOWNLIST|DIF_LISTNOAMPERSAND,0,"",
      /* 09 */DI_TEXT,3,7,0,7,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 10 */DI_VTEXT,38,4,0,4,0,0,DIF_BOXCOLOR,0,"\xD1\xB3\xB3\xC1",
      /* 11 */DI_CHECKBOX,5,8,0,8,0,0,0,0,(char *)MFindFileCase,
      /* 12 */DI_CHECKBOX,5,9,0,9,0,0,0,0,(char *)MFindFileWholeWords,
      /* 13 */DI_CHECKBOX,5,10,0,10,0,0,0,0,(char *)MSearchForHex,
      /* 14 */DI_CHECKBOX,40,8,0,8,0,0,0,0,(char *)MFindArchives,
      /* 15 */DI_CHECKBOX,40,9,0,9,0,0,0,0,(char *)MFindFolders,
      /* 16 */DI_CHECKBOX,40,10,0,10,0,0,0,0,(char *)MFindSymLinks,
      /* 17 */DI_TEXT,3,11,0,11,0,0,DIF_BOXCOLOR|DIF_SEPARATOR2,0,"",
      /* 18 */DI_VTEXT,38,7,0,7,0,0,DIF_BOXCOLOR,0,"\xC5\xB3\xB3\xB3\xCF",
      /* 19 */DI_TEXT,5,12,0,12,0,0,0,0,(char *)MSearchWhere,
      /* 20 */DI_RADIOBUTTON,5,13,0,13,0,0,DIF_GROUP,0,(char *)MSearchAllDisks,
      /* 21 */DI_RADIOBUTTON,5,14,0,14,0,1,0,0,(char *)MSearchAllButNetwork,
      /* 22 */DI_RADIOBUTTON,5,15,0,15,0,1,0,0,(char *)MSearchInPATH,
      /* 23 */DI_RADIOBUTTON,5,16,0,16,0,1,0,0,"",
      /* 24 */DI_RADIOBUTTON,40,13,0,13,0,0,0,0,(char *)MSearchFromCurrent,
      /* 25 */DI_RADIOBUTTON,40,14,0,14,0,0,0,0,(char *)MSearchInCurrent,
      /* 26 */DI_RADIOBUTTON,40,15,0,15,0,0,0,0,(char *)MSearchInSelected,
      /* 27 */DI_TEXT,3,17,0,17,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 28 */DI_CHECKBOX,5,18,0,18,0,0,0,0,(char *)MFindUseFilter,
      /* 29 */DI_CHECKBOX,40,18,0,18,0,0,0,0,(char *)MFindAdvancedOptions,
      /* 30 */DI_TEXT,3,19,0,19,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 31 */DI_BUTTON,0,20,0,20,0,0,DIF_CENTERGROUP,1,(char *)MFindFileFind,
      /* 32 */DI_BUTTON,0,20,0,20,0,0,DIF_CENTERGROUP,0,(char *)MFindFileDrive,
      /* 33 */DI_BUTTON,0,20,0,20,0,0,DIF_CENTERGROUP,0,(char *)MFindFileSetFilter,
      /* 34 */DI_BUTTON,0,20,0,20,0,0,DIF_CENTERGROUP,0,(char *)MFindFileAdvanced,
      /* 35 */DI_BUTTON,0,20,0,20,0,0,DIF_CENTERGROUP,0,(char *)MCancel,
    };
    /* KM $ */
    FindAskDlgData[23].Data=SearchFromRoot;

    MakeDialogItems(FindAskDlgData,FindAskDlg);


    if (!*FindStr)
      FindAskDlg[15].Selected=Opt.FindOpt.FindFolders;
    for(I=20; I <= 26; ++I)
      FindAskDlg[I].Selected=0;
    FindAskDlg[20+SearchMode].Selected=1;

    {
      if (PluginMode)
      {
        struct OpenPluginInfo Info;
        HANDLE hPlugin=ActivePanel->GetPluginHandle();
        CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
        /* $ 14.05.2001 DJ
           ���������, � �� ������
        */
        if ((Info.Flags & OPIF_REALNAMES)==0)
          FindAskDlg[14].Flags |= DIF_DISABLE;
        /* DJ $ */
        if (FindAskDlg[20].Selected || FindAskDlg[21].Selected)
        {
          FindAskDlg[20].Selected=FindAskDlg[21].Selected=0;
          FindAskDlg[23].Selected=1;
        }
        FindAskDlg[20].Flags=FindAskDlg[21].Flags|=DIF_DISABLE;
      }
    }

    if (!RegVer || PluginMode)
    {
      FindAskDlg[16].Selected=0;
      FindAskDlg[16].Flags|=DIF_DISABLE;
    }
    else
      FindAskDlg[16].Selected=SearchInSymLink;

    /* $ 14.05.2001 DJ
       �� �������� �������, ���� ������ ������ � �������
    */
    if (!(FindAskDlg[14].Flags & DIF_DISABLE))
      FindAskDlg[14].Selected=SearchInArchives;
    /* DJ $ */

    xstrncpy(FindAskDlg[2].Data,FindMask,sizeof(FindAskDlg[2].Data)-1);

    /* $ 26.10.2003 KM */
    if (SearchHex)
      xstrncpy(FindAskDlg[6].Data,FindStr,sizeof(FindAskDlg[6].Data)-1);
    else
      xstrncpy(FindAskDlg[5].Data,FindStr,sizeof(FindAskDlg[5].Data)-1);
    /* KM $ */

    FindAskDlg[11].Selected=CmpCase;
    FindAskDlg[12].Selected=WholeWords;
    FindAskDlg[13].Selected=SearchHex;

    // ������������ ������. KM
    FindAskDlg[28].Selected=UseFilter;

    // �������� �������������� ����� ������. KM
    FindAskDlg[29].Selected=EnableSearchInFirst;

    while (1)
    {
      int ExitCode;
      {
        FindAskDlg[8].ListItems=&TableList;

        Dialog Dlg(FindAskDlg,sizeof(FindAskDlg)/sizeof(FindAskDlg[0]),MainDlgProc);

        Dlg.SetHelp("FindFile");
        Dlg.SetPosition(-1,-1,DLG_WIDTH+4,DLG_HEIGHT);
        Dlg.Process();
        ExitCode=Dlg.GetExitCode();
      }

      if (ExitCode!=31)
      {
        xf_free(TableItem);
        CloseHandle(hPluginMutex);
        return;
      }

      /* ����������� ������������� ���������� */
      Opt.CharTable.AllTables=UseAllTables;
      Opt.CharTable.AnsiTable=UseANSI;
      Opt.CharTable.UnicodeTable=UseUnicode;
      if (UseDecodeTable)
        Opt.CharTable.TableNum=TableNum+1;
      else
        Opt.CharTable.TableNum=0;
      /****************************************/

      /* $ 01.07.2001 IS
         �������� ����� �� ������������
      */
      if(!*FindAskDlg[2].Data)             // ���� ������ � ������� �����,
         strcpy(FindAskDlg[2].Data, "*"); // �� �������, ��� ����� ���� "*"

      if(FileMaskForFindFile.Set(FindAskDlg[2].Data, FMF_ADDASTERISK))
           break;
      /* IS $ */
    }

    CmpCase=FindAskDlg[11].Selected;
    /* $ 30.07.2000 KM
       ��������� ����������
    */
    WholeWords=FindAskDlg[12].Selected;
    /* KM $ */
    SearchHex=FindAskDlg[13].Selected;
    SearchInArchives=FindAskDlg[14].Selected;
    if (FindFoldersChanged)
      Opt.FindOpt.FindFolders=FindAskDlg[15].Selected;

    if (RegVer && !PluginMode)
      SearchInSymLink=FindAskDlg[16].Selected;

    // ��������� ������� ������������� �������. KM
    Opt.FindOpt.UseFilter=UseFilter=FindAskDlg[28].Selected;

    /* $ 14.12.2000 OT */

    if (strlen (FindAskDlg[2].Data) > sizeof(FindMask) )
      ShowTruncateMessage(MFindFileMasks,sizeof(FindMask)-1);

    xstrncpy(FindMask,*FindAskDlg[2].Data ? FindAskDlg[2].Data:"*",sizeof(FindMask)-1);

    if (strlen((SearchHex)?FindAskDlg[5].Data:FindAskDlg[6].Data) > sizeof(FindStr))
      ShowTruncateMessage(MFindFileText,sizeof(FindStr)-1);

    /* $ 26.10.2003 KM */
    if (SearchHex)
    {
      xstrncpy(FindStr,FindAskDlg[6].Data,sizeof(FindStr)-1);
      RemoveTrailingSpaces((char *)FindStr);
    }
    else
      xstrncpy(FindStr,FindAskDlg[5].Data,sizeof(FindStr)-1);
    /* KM $ */

    /* OT $ */

    if (*FindStr)
    {
      xstrncpy(GlobalSearchString,FindStr,sizeof(GlobalSearchString)-1);
      GlobalSearchCase=CmpCase;
      /* $ 30.07.2000 KM
         ��������� ����������
      */
      GlobalSearchWholeWords=WholeWords;
      /* KM $ */
      GlobalSearchHex=SearchHex;
    }
    if (FindAskDlg[20].Selected)
      SearchMode=SEARCH_ALL;
    if (FindAskDlg[21].Selected)
      SearchMode=SEARCH_ALL_BUTNETWORK;
    if (FindAskDlg[22].Selected)
      SearchMode=SEARCH_INPATH;
    if (FindAskDlg[23].Selected)
      SearchMode=SEARCH_ROOT;
    if (FindAskDlg[24].Selected)
      SearchMode=SEARCH_FROM_CURRENT;
    if (FindAskDlg[25].Selected)
      SearchMode=SEARCH_CURRENT_ONLY;
    if (FindAskDlg[26].Selected)
        SearchMode=SEARCH_SELECTED;
    if (SearchFromChanged)
    {
      Opt.FindOpt.FileSearchMode=SearchMode;
    }
    LastCmpCase=CmpCase;
    /* $ 30.07.2000 KM
       ��������� ����������
    */
    LastWholeWords=WholeWords;
    /* KM $ */
    LastSearchHex=SearchHex;
    LastSearchInArchives=SearchInArchives;
    LastSearchInSymLink=SearchInSymLink;
    xstrncpy(LastFindMask,FindMask,sizeof(LastFindMask)-1);
    xstrncpy(LastFindStr,FindStr,sizeof(LastFindStr)-1);
    if (*FindStr)
      Editor::SetReplaceMode(FALSE);
  } while (FindFilesProcess());
  CloseHandle(hPluginMutex);
  xf_free(TableItem);
}


FindFiles::~FindFiles()
{
  /* $ 02.07.2001 IS
     ��������� ������.
  */
  FileMaskForFindFile.Free();
  /* IS $ */
  ClearAllLists();
  ScrBuf.ResetShadow();

  // ��������� ������ �������
  if(Filter)
    delete Filter;
}

LONG_PTR WINAPI FindFiles::AdvancedDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2)
{
  switch(Msg)
  {
    case DN_INITDIALOG:
    {
      Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,2,1);
    }
    case DN_LISTCHANGE:
    {
      /* $ 11.12.2005 KM
        �������� ������ ���� �������
      */
      if (Param1==3)
      {
        SearchInFirstIndex=(int)Param2;
      }
      return TRUE;
    }
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

void FindFiles::AdvancedDialog()
{
  /* $ 10.04.2005 KM
     ������ ������ ���������, ��������� ������� ���������� �����
     � ������ ������ (����������, ���������� ��� ����������) ������
  */
  FarList SizeList;
  FarListItem *SizeItem=(FarListItem *)xf_malloc(sizeof(FarListItem)*4);
  SizeList.Items=SizeItem;
  SizeList.ItemsNumber=4;

  memset(SizeItem,0,sizeof(FarListItem)*4);
  xstrncpy(SizeItem[0].Text,MSG(MFindFileSearchInBytes),sizeof(SizeItem[0].Text)-1);
  xstrncpy(SizeItem[1].Text,MSG(MFindFileSearchInKBytes),sizeof(SizeItem[2].Text)-1);
  xstrncpy(SizeItem[2].Text,MSG(MFindFileSearchInMBytes),sizeof(SizeItem[3].Text)-1);
  xstrncpy(SizeItem[3].Text,MSG(MFindFileSearchInGBytes),sizeof(SizeItem[4].Text)-1);
  /* KM $ */

  const char *DigitMask="99999999999999999999";

  static struct DialogData AdvancedDlgData[]=
  {
    /* 00 */DI_DOUBLEBOX,3,1,38,6,0,0,0,0,(char *)MFindFileAdvancedTitle,
    /* 01 */DI_TEXT,5,2,0,2,0,0,0,0,(char *)MFindFileSearchFirst,
    /* 02 */DI_FIXEDIT,5,3,24,3,0,(DWORD_PTR)DigitMask,DIF_MASKEDIT,0,"",
    /* 03 */DI_COMBOBOX,26,3,36,13,0,0,DIF_DROPDOWNLIST|DIF_LISTNOAMPERSAND,0,"",
    /* 04 */DI_TEXT,3,4,0,4,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    /* 05 */DI_BUTTON,0,5,0,5,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    /* 06 */DI_BUTTON,0,5,0,5,0,0,DIF_CENTERGROUP,0,(char *)MCancel,
  };

  MakeDialogItems(AdvancedDlgData,AdvancedDlg);

  // ��������� ������, ������� ����� ����������� ��� ����������� ������
  // ������ � ������ ������ (����������, ���������� ��� ����������) � ������.
  xstrncpy(AdvancedDlg[2].Data,Opt.FindOpt.SearchInFirstSize,sizeof(AdvancedDlg[2].Data)-1);

  // ��������� ������� ���������, � ������� ����� ��������� ������ ����������� ������
  // ������ � ������ ������ (����������, ���������� ��� ����������) � ������.
  xstrncpy(AdvancedDlg[3].Data,SizeItem[Opt.FindOpt.SearchInFirst].Text,sizeof(AdvancedDlg[3].Data)-1);

  AdvancedDlg[3].ListItems=&SizeList;

  Dialog Dlg(AdvancedDlg,sizeof(AdvancedDlg)/sizeof(AdvancedDlg[0]),AdvancedDlgProc);

  Dlg.SetHelp("FindFileAdvanced");
  Dlg.SetPosition(-1,-1,38+4,6+2);
  Dlg.Process();
  int ExitCode=Dlg.GetExitCode();

  if (ExitCode==5) // OK
  {
    /* $ 11.04.2005 KM
      �������� ���������, ����������� � ������ � ������
      ������ (��,�� ��� ��) ����� ������.
    */
    Opt.FindOpt.SearchInFirst=SearchInFirstIndex;
    xstrncpy(Opt.FindOpt.SearchInFirstSize,AdvancedDlg[2].Data,sizeof(Opt.FindOpt.SearchInFirstSize)-1);

    // ������� ������������ ������ ������ �� ����� ��� ������
    SearchInFirst=GetSearchInFirst(AdvancedDlg[2].Data);
    /* KM $ */
  }

  xf_free(SizeItem);
}

int FindFiles::GetPluginFile(DWORD ArcIndex, struct PluginPanelItem *PanelItem,
                             char *DestPath, char *ResultName)
{
  _ALGO(CleverSysLog clv("FindFiles::GetPluginFile()"));
  HANDLE hPlugin = ArcList[ArcIndex].hPlugin;
  char SaveDir[NM];
  struct OpenPluginInfo Info;
  CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
  xstrncpy(SaveDir,Info.CurDir,sizeof(SaveDir)-1);
  AddEndSlash(SaveDir);

  CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_SILENT|OPM_FIND);
  SetPluginDirectory(ArcList[ArcIndex].RootPath,hPlugin);
  SetPluginDirectory(PanelItem->FindData.cFileName,hPlugin);

  int nItemsNumber;
  struct PluginPanelItem *pItems;
  int nResult = 0;

  char *lpFileNameToFind = xf_strdup (PanelItem->FindData.cFileName);
  char *lpFileNameToFindShort = xf_strdup (PanelItem->FindData.cAlternateFileName);

  lpFileNameToFind = PointToName(RemovePseudoBackSlash(lpFileNameToFind));
  lpFileNameToFindShort = PointToName(RemovePseudoBackSlash(lpFileNameToFindShort));

  if ( CtrlObject->Plugins.GetFindData (
      hPlugin,
      &pItems,
      &nItemsNumber,
      OPM_FIND
      ) )
  {
    for (int i = 0; i < nItemsNumber; i++)
    {
      struct PluginPanelItem *pItem = &pItems[i];

      char *lpFileName = PointToName(RemovePseudoBackSlash(pItem->FindData.cFileName));

      if ( lpFileName != pItem->FindData.cFileName )
        xstrncpy(pItem->FindData.cFileName, lpFileName, sizeof (pItem->FindData.cFileName));

      lpFileName = PointToName(RemovePseudoBackSlash(pItem->FindData.cAlternateFileName));

      if ( lpFileName != pItem->FindData.cAlternateFileName )
        xstrncpy(pItem->FindData.cAlternateFileName, lpFileName, sizeof (pItem->FindData.cAlternateFileName));

      if ( !strcmp (lpFileNameToFind, pItem->FindData.cFileName) &&
           !strcmp (lpFileNameToFindShort, pItem->FindData.cAlternateFileName) )
      {
        nResult = CtrlObject->Plugins.GetFile (
                            hPlugin,
                            pItem,
                            DestPath,
                            ResultName,
                            OPM_SILENT|OPM_FIND
                            );

        break;
      }
    }

    CtrlObject->Plugins.FreeFindData (hPlugin, pItems, nItemsNumber);
  }

  CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_SILENT|OPM_FIND);
//  SetPluginDirectory(ArcList[ArcIndex].RootPath,hPlugin);
  SetPluginDirectory(SaveDir,hPlugin);

  return nResult;
}

LONG_PTR WINAPI FindFiles::FindDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2)
{
  Dialog* Dlg=(Dialog*)hDlg;
  VMenu *ListBox=Dlg->Item[1].ListPtr;

  CriticalSectionLock Lock(Dlg->CS);

  switch(Msg)
  {
    case DN_DRAWDIALOGDONE:
    {
      Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
      // ���������� ����� �� ������ [Go To]
      if((FindDirCount || FindFileCount) && !FindPositionChanged)
      {
        FindPositionChanged=TRUE;
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,6/* [Go To] */,0);
      }
//      else
//        ScrBuf.Flush();
      return TRUE;
    }
/*
    case DN_HELP: // � ������ ������ �������� ����� ������, ���... ��������� � �����������
    {
      return !SearchDone?NULL:Param2;
    }
*/
    case DN_MOUSECLICK:
    {
      /* $ 07.04.2003 VVM
         ! ���� �� ����� ������ ������� ��������� ���� ����� - ��������.
         �� ���� �������� ��������� �� �������� �������. ��� �� � ����... */
      SMALL_RECT drect,rect;
      int Ret = FALSE;
      Dialog::SendDlgMessage(hDlg,DM_GETDLGRECT,0,(LONG_PTR)&drect);
      Dialog::SendDlgMessage(hDlg,DM_GETITEMPOSITION,1,(LONG_PTR)&rect);
      if (Param1==1 && ((MOUSE_EVENT_RECORD *)Param2)->dwMousePosition.X<drect.Left+rect.Right)
      {
        Ret = (int)FindDlgProc(hDlg,DN_BTNCLICK,6,0); // emulates a [ Go to ] button pressing
      }
      /* VVM $ */
      return Ret;
    }

    case DN_KEY:
    {
      if (!StopSearch && ((Param2==KEY_ESC) || (Param2 == KEY_F10)) )
      {
        PauseSearch=TRUE;
        IsProcessAssignMacroKey++; // �������� ���� �������
                                   // �.�. � ���� ������� ������ ������ Alt-F9!
        int LocalRes=TRUE;
        if (Opt.Confirm.Esc)
          LocalRes=AbortMessage();
        IsProcessAssignMacroKey--;
        PauseSearch=FALSE;
        StopSearch=LocalRes;

        return TRUE;
      }

      if (!ListBox)
      {
        return TRUE;
      }

      if(Param2 == KEY_LEFT || Param2 == KEY_RIGHT)
        FindPositionChanged = TRUE;

      // �������� ����.������� ����� �����������.
      if(Param2 == KEY_CTRLALTSHIFTPRESS || Param2 == KEY_ALTF9)
      {
        IsProcessAssignMacroKey--;
        FrameManager->ProcessKey((DWORD)Param2);
        IsProcessAssignMacroKey++;
        return TRUE;
      }

      if (Param1==9 && (Param2==KEY_RIGHT || Param2==KEY_TAB)) // [ Stop ] button
      {
        FindPositionChanged = TRUE;
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,5/* [ New search ] */,0);
        return TRUE;
      }
      else if (Param1==5 && (Param2==KEY_LEFT || Param2==KEY_SHIFTTAB)) // [ New search ] button
      {
        FindPositionChanged = TRUE;
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,9/* [ Stop ] */,0);
        return TRUE;
      }
      else if (Param2==KEY_UP || Param2==KEY_DOWN || Param2==KEY_PGUP ||
               Param2==KEY_PGDN || Param2==KEY_HOME || Param2==KEY_END ||
               Param2==KEY_MSWHEEL_LEFT || Param2==KEY_MSWHEEL_RIGHT ||
               Param2==KEY_MSWHEEL_UP || Param2==KEY_MSWHEEL_DOWN)
      {
        ListBox->ProcessKey((int)Param2);
        return TRUE;
      }
      else if (Param2==KEY_F3 || Param2==KEY_NUMPAD5 || Param2==KEY_SHIFTNUMPAD5 || Param2==KEY_F4 ||
               (Param2==KEY_ENTER||Param2==KEY_NUMENTER) && Dialog::SendDlgMessage(hDlg,DM_GETFOCUS,0,0) == 7
              )
      {
        if (ListBox->GetItemCount()==0)
        {
          return TRUE;
        }

        if((Param2==KEY_ENTER||Param2==KEY_NUMENTER) && Dialog::SendDlgMessage(hDlg,DM_GETFOCUS,0,0) == 7)
          Param2=KEY_F3;

        ffCS.Enter ();

        DWORD ItemIndex = (DWORD)(DWORD_PTR)ListBox->GetUserData(NULL, 0);
        if (ItemIndex != LIST_INDEX_NONE)
        {
          int RemoveTemp=FALSE;
          int ClosePlugin=FALSE; // ������� ���� ���������, ���� �������.
          char SearchFileName[NM];
          char TempDir[NM];

          if (FindList[ItemIndex].FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
          {
            ffCS.Leave ();

            return TRUE;
          }
          char *FileName=FindList[ItemIndex].FindData.cFileName;
          // FindFileArcIndex ������ ����� ������������
          // �� ����� ���� ��� ������.
          if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
              (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
          {
            char *FindArcName = ArcList[FindList[ItemIndex].ArcIndex].ArcName;
            if (ArcList[FindList[ItemIndex].ArcIndex].hPlugin == INVALID_HANDLE_VALUE)
            {
              char *Buffer=new char[Opt.PluginMaxReadData];
              if(Buffer)
              {
                FILE *ProcessFile=fopen(FindArcName,"rb");
                if (ProcessFile==NULL)
                {
                  delete[] Buffer;

                  ffCS.Leave ();
                  return TRUE;
                }
                int ReadSize=(int)fread(Buffer,1,Opt.PluginMaxReadData,ProcessFile);
                fclose(ProcessFile);

                int SavePluginsOutput=DisablePluginsOutput;
                DisablePluginsOutput=TRUE;

                WaitForSingleObject(hPluginMutex,INFINITE);
                ArcList[FindList[ItemIndex].ArcIndex].hPlugin = CtrlObject->Plugins.OpenFilePlugin(FindArcName,(unsigned char *)Buffer,ReadSize);
                ReleaseMutex(hPluginMutex);

                DisablePluginsOutput=SavePluginsOutput;

                delete[] Buffer;

                if (ArcList[FindList[ItemIndex].ArcIndex].hPlugin == (HANDLE)-2 ||
                    ArcList[FindList[ItemIndex].ArcIndex].hPlugin == INVALID_HANDLE_VALUE)
                {
                  ArcList[FindList[ItemIndex].ArcIndex].hPlugin = INVALID_HANDLE_VALUE;

                  ffCS.Leave ();
                  return TRUE;
                }
                ClosePlugin = TRUE;
              }
            }

            PluginPanelItem FileItem;
            memset(&FileItem,0,sizeof(FileItem));
            FileItem.FindData=FindList[ItemIndex].FindData;
            FarMkTempEx(TempDir); // � �������� �� NULL???
            CreateDirectory(TempDir, NULL);

//            if (!CtrlObject->Plugins.GetFile(ArcList[FindList[ItemIndex].ArcIndex].hPlugin,&FileItem,TempDir,SearchFileName,OPM_SILENT|OPM_FIND))
            WaitForSingleObject(hPluginMutex,INFINITE);
            if (!GetPluginFile(FindList[ItemIndex].ArcIndex,&FileItem,TempDir,SearchFileName))
            {
              FAR_RemoveDirectory(TempDir);
              if (ClosePlugin)
              {
                CtrlObject->Plugins.ClosePlugin(ArcList[FindList[ItemIndex].ArcIndex].hPlugin);
                ArcList[FindList[ItemIndex].ArcIndex].hPlugin = INVALID_HANDLE_VALUE;
              }
              ReleaseMutex(hPluginMutex);

              ffCS.Leave ();
              return FALSE;
            }
            else
            {
              if (ClosePlugin)
              {
                CtrlObject->Plugins.ClosePlugin(ArcList[FindList[ItemIndex].ArcIndex].hPlugin);
                ArcList[FindList[ItemIndex].ArcIndex].hPlugin = INVALID_HANDLE_VALUE;
              }
              ReleaseMutex(hPluginMutex);
            }
            RemoveTemp=TRUE;
          }
          else
          {
            xstrncpy(SearchFileName,FindList[ItemIndex].FindData.cFileName,sizeof(SearchFileName)-1);
            if(GetFileAttributes(SearchFileName) == (DWORD)-1 && GetFileAttributes(FindList[ItemIndex].FindData.cAlternateFileName) != (DWORD)-1)
              xstrncpy(SearchFileName,FindList[ItemIndex].FindData.cAlternateFileName,sizeof(SearchFileName)-1);
          }

          DWORD FileAttr;
          if ((FileAttr=GetFileAttributes(SearchFileName))!=(DWORD)-1)
          {
            char OldTitle[512];
            GetConsoleTitle(OldTitle,sizeof(OldTitle));

            if (Param2==KEY_F3 || Param2==KEY_NUMPAD5 || Param2==KEY_SHIFTNUMPAD5)
            {
              int ListSize=ListBox->GetItemCount();
              NamesList ViewList;
              // ������� ��� �����, ������� ����� �������� �����...
              if(Opt.FindOpt.CollectFiles)
              {
                DWORD Index;
                for (int I=0;I<ListSize;I++)
                {
                  Index = (DWORD)(DWORD_PTR)ListBox->GetUserData(NULL, 0, I);
                  LPFINDLIST PtrFindList=FindList+Index;
                  if ((Index != LIST_INDEX_NONE) &&
                      ((PtrFindList->ArcIndex == LIST_INDEX_NONE) ||
                       (ArcList[PtrFindList->ArcIndex].Flags & OPIF_REALNAMES)))
                  {
                    // �� ��������� ����� � ������� � OPIF_REALNAMES
                    if (*PtrFindList->FindData.cFileName && !(PtrFindList->FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
                      ViewList.AddName(PtrFindList->FindData.cFileName,PtrFindList->FindData.cAlternateFileName);
                  } /* if */
                } /* for */
                ViewList.SetCurName(FindList[ItemIndex].FindData.cFileName);
              }
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,FALSE,0);
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
              {
                FileViewer ShellViewer (SearchFileName,FALSE,FALSE,FALSE,-1,NULL,(FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE)?NULL:(Opt.FindOpt.CollectFiles?&ViewList:NULL));
                ShellViewer.SetDynamicallyBorn(FALSE);
                ShellViewer.SetEnableF6(TRUE);
                // FindFileArcIndex ������ ����� ������������
                // �� ����� ���� ��� ������.
                if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
                    (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
                  ShellViewer.SetSaveToSaveAs(TRUE);
                //!Mutex.Unlock();

                ffCS.Leave (); // ����� ����� �����������, ���� �� ��� ������ �����

                IsProcessVE_FindFile++;
                FrameManager->EnterModalEV();
                FrameManager->ExecuteModal();
                FrameManager->ExitModalEV();
                IsProcessVE_FindFile--;

                ffCS.Enter ();

                // ���������� ���������� �����
                FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
              }

              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
            }
            else
            {
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,FALSE,0);
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
              {
/* $ 14.08.2002 VVM
  ! ����-��� �������� �� ������ ������������� � �������� ��������.
    � ���������, ������� �� ��� �� �������� ������
                int FramePos=FrameManager->FindFrameByFile(MODALTYPE_EDITOR,SearchFileName);
                int SwitchTo=FALSE;
                if (FramePos!=-1)
                {
                  if (!(*FrameManager)[FramePos]->GetCanLoseFocus(TRUE) ||
                      Opt.Confirm.AllowReedit)
                  {
                    char MsgFullFileName[NM];
                    xstrncpy(MsgFullFileName,SearchFileName,sizeof(MsgFullFileName)-1);
                    int MsgCode=Message(0,2,MSG(MFindFileTitle),
                          TruncPathStr(MsgFullFileName,ScrX-16),
                          MSG(MAskReload),
                          MSG(MCurrent),MSG(MNewOpen));
                    if (MsgCode==0)
                    {
                      SwitchTo=TRUE;
                    }
                    else if (MsgCode==1)
                    {
                      SwitchTo=FALSE;
                    }
                    else
                    {
                      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
                      Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
                      return TRUE;
                    }
                  }
                  else
                  {
                    SwitchTo=TRUE;
                  }
                }
                if (SwitchTo)
                {
                  (*FrameManager)[FramePos]->SetCanLoseFocus(FALSE);
                  (*FrameManager)[FramePos]->SetDynamicallyBorn(FALSE);
                  FrameManager->ActivateFrame(FramePos);
                  IsProcessVE_FindFile++;
                  FrameManager->EnterModalEV();
                  FrameManager->ExecuteModal ();
                  FrameManager->ExitModalEV();
//                  FrameManager->ExecuteNonModal();
                  IsProcessVE_FindFile--;
                  // ���������� ���������� �����
                  FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
                }
                else
*/
                {
                  FileEditor ShellEditor (SearchFileName,0);
                  ShellEditor.SetDynamicallyBorn(FALSE);
                  ShellEditor.SetEnableF6 (TRUE);
                  // FindFileArcIndex ������ ����� ������������
                  // �� ����� ���� ��� ������.
                  if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
                      (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
                    ShellEditor.SetSaveToSaveAs(TRUE);

                  ffCS.Leave (); // ����� ����� �����������, ���� �� ��� ������ �����
                  IsProcessVE_FindFile++;
                  FrameManager->EnterModalEV();
                  FrameManager->ExecuteModal ();
                  FrameManager->ExitModalEV();
                  IsProcessVE_FindFile--;

                  ffCS.Enter ();
                  // ���������� ���������� �����
                  FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
                }
              }
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
            }
            SetConsoleTitle(OldTitle);
          }
          if (RemoveTemp)
            DeleteFileWithFolder(SearchFileName);
        }

        ffCS.Leave ();

        return TRUE;
      }
      return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
    }
    case DN_BTNCLICK:
    {

      FindPositionChanged = TRUE;

      if (Param1==5) // [ New search ] button pressed
      {
        StopSearch=TRUE;
        FindExitCode=FIND_EXIT_SEARCHAGAIN;
        return FALSE;
      }
      else if (Param1==9) // [ Stop ] button pressed
      {
        if (StopSearch)
          return FALSE;
        StopSearch=TRUE;
        return TRUE;
      }
      else if (Param1==6) // [ Goto ] button pressed
      {
        if (!ListBox)
        {
          return FALSE;
        }

        // ������� ����� ������ ��� �� ����� ������ �� �������.
        // ������� ������ ��� [ Panel ]
        if (ListBox->GetItemCount()==0)
        {
          return (TRUE);
        }
        FindExitIndex = (DWORD)(DWORD_PTR)ListBox->GetUserData(NULL, 0);
        if (FindExitIndex != LIST_INDEX_NONE)
          FindExitCode = FIND_EXIT_GOTO;
        return (FALSE);
      }
      else if (Param1==7) // [ View ] button pressed
      {
        FindDlgProc(hDlg,DN_KEY,1,KEY_F3);
        return TRUE;
      }
      else if (Param1==8) // [ Panel ] button pressed
      {
        if (!ListBox)
        {
          return FALSE;
        }

        // �� ������ ����� �������� �� � �������, � �����.
        // ����� ��������� ������. ����� �������� ��������, ����� ��
        // ���� �� ������, ����� �� ������� � ������� ����� (� �����-��
        // ����!) � � ���������� ��� ���������.
        if (ListBox->GetItemCount()==0)
        {
          return (TRUE);
        }
        FindExitCode = FIND_EXIT_PANEL;
        FindExitIndex = (DWORD)(DWORD_PTR)ListBox->GetUserData(NULL, 0);
        return (FALSE);
      }
    }
    case DN_CTLCOLORDLGLIST:
    {
      if (Param2)
        ((struct FarListColors *)Param2)->Colors[VMenuColorDisabled]=FarColorToReal(COL_DIALOGLISTTEXT);
      return TRUE;
    }
    case DN_CLOSE:
    {
      StopSearch=TRUE;
      return TRUE;
    }
    /* 10.08.2001 KM
       ��������� �������� ������� ������ ��� ��������� �������� �������.
    */
    case DN_RESIZECONSOLE:
    {
      COORD coord=(*(COORD*)Param2);
      SMALL_RECT rect;
      int IncY=coord.Y-DlgHeight-3;
      int I;

      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
      for (I=0;I<10;I++)
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,I,FALSE);

      Dialog::SendDlgMessage(hDlg,DM_GETDLGRECT,0,(LONG_PTR)&rect);
      coord.X=rect.Right-rect.Left+1;
      DlgHeight+=IncY;
      coord.Y=DlgHeight;

      if (IncY>0)
        Dialog::SendDlgMessage(hDlg,DM_RESIZEDIALOG,0,(LONG_PTR)&coord);

      for (I=0;I<2;I++)
      {
        Dialog::SendDlgMessage(hDlg,DM_GETITEMPOSITION,I,(LONG_PTR)&rect);
        rect.Bottom+=(short)IncY;
        Dialog::SendDlgMessage(hDlg,DM_SETITEMPOSITION,I,(LONG_PTR)&rect);
      }

      for (I=2;I<10;I++)
      {
        Dialog::SendDlgMessage(hDlg,DM_GETITEMPOSITION,I,(LONG_PTR)&rect);
        if (I==2)
          rect.Left=-1;
        rect.Top+=(short)IncY;
        Dialog::SendDlgMessage(hDlg,DM_SETITEMPOSITION,I,(LONG_PTR)&rect);
      }

      if (!(IncY>0))
        Dialog::SendDlgMessage(hDlg,DM_RESIZEDIALOG,0,(LONG_PTR)&coord);

      for (I=0;I<10;I++)
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,I,TRUE);
      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);

      return TRUE;
    }
    /* KM $ */
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

int FindFiles::FindFilesProcess()
{
  _ALGO(CleverSysLog clv("FindFiles::FindFilesProcess()"));
  // � ����������� ��������� ����� � ����������� ����������
  static char Title[2*NM]="";
  static char SearchStr[NM]="";

  /* $ 05.10.2003 KM
     ���� ������������ ������ ��������, �� �� ����� ������ �������� �� ����
  */
  if (*FindMask)
    if (UseFilter)
      sprintf(Title,"%s: %s (%s)",MSG(MFindFileTitle),FindMask,MSG(MFindUsingFilter));
    else
      sprintf(Title,"%s: %s",MSG(MFindFileTitle),FindMask);
  else
    if (UseFilter)
      sprintf(Title,"%s (%s)",MSG(MFindFileTitle),MSG(MFindUsingFilter));
    else
      sprintf(Title,"%s",MSG(MFindFileTitle));
  /* KM $ */
  if (*FindStr)
  {
    /* $ 26.10.2003 KM */
    /* $ 24.09.2003 KM */
    char Temp[NM],FStr[NM*2];

    xstrncpy(FStr,FindStr,sizeof(FStr)-1);
    sprintf(Temp," \"%s\"",TruncStrFromEnd(FStr,10));
    sprintf(SearchStr,MSG(MFindSearchingIn),Temp);
    /* KM $ */
    /* KM $ */
  }
  else
    sprintf(SearchStr,MSG(MFindSearchingIn),"");

  /* $ 15.03.2002 KM
     ��������� ���� ����������� ��� ����������.
  /*
  /* $ 03.12.2001 DJ
     ���������� ����� ���� ������ � ������������
  */
  static struct DialogData FindDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,DLG_WIDTH,DLG_HEIGHT-4,0,0,DIF_SHOWAMPERSAND,0,Title,
  /* 01 */DI_LISTBOX,4,2,73,DLG_HEIGHT-9,0,0,DIF_LISTNOBOX,0,(char*)0,
  /* 02 */DI_TEXT,-1,DLG_HEIGHT-8,0,DLG_HEIGHT-8,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 03 */DI_TEXT,5,DLG_HEIGHT-7,0,DLG_HEIGHT-7,0,0,DIF_SHOWAMPERSAND,0,SearchStr,
  /* 04 */DI_TEXT,3,DLG_HEIGHT-6,0,DLG_HEIGHT-6,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 05 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,1,0,DIF_CENTERGROUP,1,(char *)MFindNewSearch,
  /* 06 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindGoTo,
  /* 07 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindView,
  /* 08 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindPanel,
  /* 09 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindStop
  };
  /* DJ $ */
  /* KM $ */
  MakeDialogItems(FindDlgData,FindDlg);

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  DlgHeight=DLG_HEIGHT-2;
  DlgWidth=FindDlg[0].X2-FindDlg[0].X1-4;

  int IncY=ScrY>24 ? ScrY-24:0;
  FindDlg[0].Y2+=IncY;
  FindDlg[1].Y2+=IncY;
  FindDlg[2].Y1+=IncY;
  FindDlg[3].Y1+=IncY;
  FindDlg[4].Y1+=IncY;
  FindDlg[5].Y1+=IncY;
  FindDlg[6].Y1+=IncY;
  FindDlg[7].Y1+=IncY;
  FindDlg[8].Y1+=IncY;
  FindDlg[9].Y1+=IncY;

  DlgHeight+=IncY;

  if (PluginMode)
  {
    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
    HANDLE hPlugin=ActivePanel->GetPluginHandle();
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);

    FindFileArcIndex = AddArcListItem(Info.HostFile, hPlugin, Info.Flags, Info.CurDir);
    if (FindFileArcIndex == LIST_INDEX_NONE)
      return(FALSE);

    if ((Info.Flags & OPIF_REALNAMES)==0)
    {
      FindDlg[8].Type=DI_TEXT;
      *FindDlg[8].Data=0;
    }
  }

  Dialog Dlg=Dialog(FindDlg,sizeof(FindDlg)/sizeof(FindDlg[0]),FindDlgProc);
  hDlg=(HANDLE)&Dlg;
//  pDlg->SetDynamicallyBorn();
  Dlg.SetHelp("FindFileResult");
  Dlg.SetPosition(-1,-1,DLG_WIDTH+4,DLG_HEIGHT-2+IncY);
  // ���� �� �������� ������, � �� ������������� ��������� �����������
  // ������ ��� ������ � ������ �������� �� �����������
  Dlg.InitDialog();
  Dlg.Show();

  LastFoundNumber=0;
  SearchDone=FALSE;
  StopSearch=FALSE;
  PauseSearch=FALSE;
  WriteDataUsed=FALSE;
  PrepareFilesListUsed=0;
  FindFileCount=FindDirCount=0;
  FindExitIndex = LIST_INDEX_NONE;
  FindExitCode = FIND_EXIT_NONE;
  *FindMessage=*LastDirName=FindMessageReady=FindCountReady=FindPositionChanged=0;

  if (PluginMode)
  {
    if (_beginthread(PreparePluginList,0,NULL)==(unsigned long)-1)
      return FALSE;
  }
  else
  {
    if (_beginthread(PrepareFilesList,0,NULL)==(unsigned long)-1)
      return FALSE;
  }

  // ����� ��� ������ � ������� ���������� � ���� ������
  if (_beginthread(WriteDialogData,0,NULL)==(unsigned long)-1)
    return FALSE;

  IsProcessAssignMacroKey++; // �������� ��� ����. �������
  Dlg.Process();
  IsProcessAssignMacroKey--;

  while(WriteDataUsed || PrepareFilesListUsed > 0)
    Sleep(10);

  ::hDlg=NULL;

  switch (FindExitCode)
  {
    case FIND_EXIT_SEARCHAGAIN:
    {
      return TRUE;
    }
    case FIND_EXIT_PANEL:
    // ���������� ���������� �� ��������� ������
    {
      int ListSize = FindListCount;
      PluginPanelItem *PanelItems=new PluginPanelItem[ListSize];
      if (PanelItems==NULL)
        ListSize=0;
      int ItemsNumber=0;
      for (int i=0;i<ListSize;i++)
      {
        if (strlen(FindList[i].FindData.cFileName)>0 && FindList[i].Used)
        // ��������� ������, ���� ��� ������
        {
          // ��� �������� � ������������ ������� ������� ��� ����� �� ��� ������.
          // ������ ���� ������ ������ �����.
          int IsArchive = ((FindList[i].ArcIndex != LIST_INDEX_NONE) &&
                          !(ArcList[FindList[i].ArcIndex].Flags&OPIF_REALNAMES));
          // ��������� ������ ����� ��� ����� ������� ��� ����� ����� �������
          if (IsArchive || (Opt.FindOpt.FindFolders && !SearchHex) ||
              !(FindList[i].FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
          {
            if (IsArchive)
              xstrncpy(FindList[i].FindData.cFileName, ArcList[FindList[i].ArcIndex].ArcName,
                      sizeof(FindList[i].FindData.cFileName)-1);
            PluginPanelItem *pi=&PanelItems[ItemsNumber++];
            memset(pi,0,sizeof(*pi));
            pi->FindData=FindList[i].FindData;
            if (IsArchive)
              pi->FindData.dwFileAttributes = 0;
            /* $ 21.03.2002 VVM
              + �������� ����� ��������� ��� ��������������� "\" */
            if (pi->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
              int Length = (int)strlen(pi->FindData.cFileName);
              if ((Length) && (pi->FindData.cFileName[Length-1]=='\\'))
                pi->FindData.cFileName[Length-1] = 0;
            }
            /* VVM $ */
          }
        } /* if */
      } /* for */

      HANDLE hNewPlugin=CtrlObject->Plugins.OpenFindListPlugin(PanelItems,ItemsNumber);
      if (hNewPlugin!=INVALID_HANDLE_VALUE)
      {
        Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
        Panel *NewPanel=CtrlObject->Cp()->ChangePanel(ActivePanel,FILE_PANEL,TRUE,TRUE);
        NewPanel->SetPluginMode(hNewPlugin,"",true);
        NewPanel->SetVisible(TRUE);
        NewPanel->Update(0);
//        if (FindExitIndex != LIST_INDEX_NONE)
//          NewPanel->GoToFile(FindList[FindExitIndex].FindData.cFileName);
        NewPanel->Show();
      }
      /* $ 13.07.2000 SVS
         ������������ new[]
      */
      delete[] PanelItems;
      break;
    } /* case FIND_EXIT_PANEL */
    case FIND_EXIT_GOTO:
    {
      char *FileName=FindList[FindExitIndex].FindData.cFileName;
      Panel *FindPanel=CtrlObject->Cp()->ActivePanel;

      if ((FindList[FindExitIndex].ArcIndex != LIST_INDEX_NONE) &&
          (!(ArcList[FindList[FindExitIndex].ArcIndex].Flags & OPIF_REALNAMES)))
      {
        HANDLE hPlugin = ArcList[FindList[FindExitIndex].ArcIndex].hPlugin;
        if (hPlugin == INVALID_HANDLE_VALUE)
        {
          char ArcName[NM],ArcPath[NM];
          xstrncpy(ArcName,ArcList[FindList[FindExitIndex].ArcIndex].ArcName,sizeof(ArcName)-1);
          if (FindPanel->GetType()!=FILE_PANEL)
            FindPanel=CtrlObject->Cp()->ChangePanel(FindPanel,FILE_PANEL,TRUE,TRUE);
          xstrncpy(ArcPath,ArcName,sizeof(ArcPath)-1);
          *PointToName(ArcPath)=0;
          FindPanel->SetCurDir(ArcPath,TRUE);
          hPlugin=((FileList *)FindPanel)->OpenFilePlugin(ArcName,FALSE);
          if (hPlugin==(HANDLE)-2)
            hPlugin = INVALID_HANDLE_VALUE;
        } /* if */
        if (hPlugin != INVALID_HANDLE_VALUE)
        {
          struct OpenPluginInfo Info;
          CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);

          /* $ 19.01.2003 KM
             ��������� �������� � ������ ������� �������.
          */
          if (SearchMode==SEARCH_ROOT ||
              SearchMode==SEARCH_ALL ||
              SearchMode==SEARCH_ALL_BUTNETWORK ||
              SearchMode==SEARCH_INPATH)
            CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_FIND);
          SetPluginDirectory(FileName,hPlugin,TRUE);
          /* KM $ */
        }
      } /* if */
      else
      {
        char SetName[NM];
        int Length;
        if ((Length=(int)strlen(FileName))==0)
          break;
        if (Length>1 && FileName[Length-1]=='\\' && FileName[Length-2]!=':')
          FileName[Length-1]=0;

        #if !defined(INVALID_FILE_ATTRIBUTES)
        #define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
        #endif
        if ( (GetFileAttributes(FileName)==INVALID_FILE_ATTRIBUTES) && (GetLastError () != ERROR_ACCESS_DENIED))
          break;
        {
          char *NamePtr;
          NamePtr=PointToName(FileName);
          xstrncpy(SetName,NamePtr,sizeof(SetName)-1);
          *NamePtr=0;
          Length=(int)strlen(FileName);
          if (Length>1 && FileName[Length-1]=='\\' && FileName[Length-2]!=':')
            FileName[Length-1]=0;
        }
        if (*FileName==0)
          break;
        if (FindPanel->GetType()!=FILE_PANEL &&
            CtrlObject->Cp()->GetAnotherPanel(FindPanel)->GetType()==FILE_PANEL)
          FindPanel=CtrlObject->Cp()->GetAnotherPanel(FindPanel);
        if ((FindPanel->GetType()!=FILE_PANEL) || (FindPanel->GetMode()!=NORMAL_PANEL))
        // ������ ������ �� ������� ��������...
        {
          FindPanel=CtrlObject->Cp()->ChangePanel(FindPanel,FILE_PANEL,TRUE,TRUE);
          FindPanel->SetVisible(TRUE);
          FindPanel->Update(0);
        }
        /* $ 09.06.2001 IS
           ! �� ������ �������, ���� �� ��� � ��� ���������. ��� �����
             ���������� ����, ��� ��������� � ��������� ������ �� ������������.
        */
        {
          char DirTmp[NM];
          FindPanel->GetCurDir(DirTmp);
          Length=(int)strlen(DirTmp);
          if (Length>1 && DirTmp[Length-1]=='\\' && DirTmp[Length-2]!=':')
            DirTmp[Length-1]=0;
          if(0!=LocalStricmp(FileName, DirTmp))
            FindPanel->SetCurDir(FileName,TRUE);
        }
        /* IS $ */
        if (*SetName)
          FindPanel->GoToFile(SetName);
        FindPanel->Show();
        FindPanel->SetFocus();
      }
      break;
    } /* case FIND_EXIT_GOTO */
  } /* switch */

  return FALSE;
}


void FindFiles::SetPluginDirectory(char *DirName,HANDLE hPlugin,int UpdatePanel)
{
  _ALGO(CleverSysLog clv("FindFiles::SetPluginDirectory()"));
  _ALGO(SysLog("DirName='%s', hPlugin=0x%08X, UpdatePanel=%d",(DirName?DirName:"NULL"),hPlugin,UpdatePanel));
  char Name[NM],*StartName,*EndName;
  int IsPluginDir;

  /* $ 19.01.2003 KM
     ����������� ��������� �� 4 ����. ���� � DirName ����
     ������ '\x1' ������ ��� ���� �� �������. ����� �������
     ����� ���������� ���������� ���� �, ��������������,
     ������� ���������� �������.
  */
  if (strlen(DirName)>0)
    IsPluginDir=strchr(DirName,'\x1')!=NULL;
  else
    IsPluginDir=FALSE;

  xstrncpy(Name,DirName,sizeof(Name)-1);
  StartName=Name;
  while(IsPluginDir?(EndName=strchr(StartName,'\x1'))!=NULL:(EndName=strchr(StartName,'\\'))!=NULL)
  /* KM $ */
  {
    *EndName=0;
    // RereadPlugin
    {
      int FileCount=0;
      PluginPanelItem *PanelData=NULL;
      if (CtrlObject->Plugins.GetFindData(hPlugin,&PanelData,&FileCount,OPM_FIND))
        CtrlObject->Plugins.FreeFindData(hPlugin,PanelData,FileCount);
    }
    CtrlObject->Plugins.SetDirectory(hPlugin,StartName,OPM_FIND);
    StartName=EndName+1;
  }
  /* $ 19.01.2003 KM
     �������� ������ ��� �������������.
  */
  if (UpdatePanel)
  {
    CtrlObject->Cp()->ActivePanel->Update(UPDATE_KEEP_SELECTION);
    if (!CtrlObject->Cp()->ActivePanel->GoToFile(StartName))
      CtrlObject->Cp()->ActivePanel->GoToFile(DirName);
    CtrlObject->Cp()->ActivePanel->Show();
  }
  /* KM $ */
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif

void _cdecl FindFiles::DoScanTree(char* Root, WIN32_FIND_DATA& FindData, char* FullName, size_t cbFullName)
{
        ScanTree ScTree(FALSE,!(SearchMode==SEARCH_CURRENT_ONLY||SearchMode==SEARCH_INPATH),SearchInSymLink);

        char SelName[NM];
        int FileAttr;
        if (SearchMode==SEARCH_SELECTED)
          CtrlObject->Cp()->ActivePanel->GetSelName(NULL,FileAttr);

        while (1)
        {
          char CurRoot[2*NM];
          if (SearchMode==SEARCH_SELECTED)
          {
            if (!CtrlObject->Cp()->ActivePanel->GetSelName(SelName,FileAttr))
              break;
            if ((FileAttr & FILE_ATTRIBUTE_DIRECTORY)==0 || TestParentFolderName(SelName) ||
                strcmp(SelName,".")==0)
              continue;
            xstrncpy(CurRoot,Root,sizeof(CurRoot)-1);
            AddEndSlash(CurRoot);
            strcat(CurRoot,SelName);
          }
          else
            xstrncpy(CurRoot,Root,sizeof(CurRoot)-1);

          ScTree.SetFindPath(CurRoot,"*.*");

          statusCS.Enter ();

          xstrncpy(FindMessage,CurRoot,sizeof(FindMessage)-1);
          FindMessage[sizeof(FindMessage)-1]=0;
          FindMessageReady=TRUE;

          statusCS.Leave ();

          while (!StopSearch && ScTree.GetNextName(&FindData,FullName, cbFullName-1))
          {
            while (PauseSearch)
              Sleep(10);

            /* $ 30.09.2003 KM
              ����������� ����� �� ���������� � ����������� ������
            */
            if (UseFilter)
            {
              // ���� ������� ������ "� ��������" � ��� ��� �������� ��������
              // �������, �� ����������  �� ������ ��� �������� �� � �� ���
              // ���� (�� ������). ������ ���� �������� ��� ��������� ��
              // ������������� - �� ����� � ��� �������� ����
              bool isDir = (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
              if (Filter->FileInFilter(&FindData, isDir) == isDir)
              {
                if(isDir)
                  ScTree.SkipDir();
                continue;
              }
            }

            /* $ 14.06.2004 KM
              ��������� �������� ��� ��������� ���������
            */
            if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
              statusCS.Enter();

              xstrncpy(FindMessage,FullName,sizeof(FindMessage)-1);
              FindMessage[sizeof(FindMessage)-1]=0;
              FindMessageReady=TRUE;

              statusCS.Leave();
            }
            /* KM $ */

            if (IsFileIncluded(NULL,FullName,FindData.dwFileAttributes))
              AddMenuRecord(FullName,&FindData);

            if (SearchInArchives)
              ArchiveSearch(FullName);
            /* KM $ */
          }
          if (SearchMode!=SEARCH_SELECTED)
            break;
        }
      }

void _cdecl FindFiles::PrepareFilesList(void *Param)
{
  WIN32_FIND_DATA FindData;
  char FullName[NM],Root[NM*2];
  char *PathEnv=NULL, *Ptr;

  TRY {

    PrepareFilesListUsed++;
    DWORD DiskMask=FarGetLogicalDrives();
    CtrlObject->CmdLine->GetCurDir(Root);

    if(SearchMode==SEARCH_INPATH)
    {
      DWORD SizeStr=GetEnvironmentVariable("PATH",NULL,0);
      if((PathEnv=(char *)alloca(SizeStr+2)) != NULL)
      {
        GetEnvironmentVariable("PATH",PathEnv,SizeStr+1);
        PathEnv[strlen(PathEnv)]=0;
        Ptr=PathEnv;
        while(*Ptr)
        {
          if(*Ptr==';')
            *Ptr=0;
          ++Ptr;
        }
      }
      Ptr=PathEnv;
    }

    for (int CurrentDisk=0;;CurrentDisk++,DiskMask>>=1)
    {
      if (SearchMode==SEARCH_ALL ||
          SearchMode==SEARCH_ALL_BUTNETWORK)
      {
        if(DiskMask==0)
          break;

        if ((DiskMask & 1)==0)
          continue;
        sprintf(Root,"%c:\\",'A'+CurrentDisk);
        int DriveType=FAR_GetDriveType(Root);
        if (DriveType==DRIVE_REMOVABLE || IsDriveTypeCDROM(DriveType) ||
           (DriveType==DRIVE_REMOTE && SearchMode==SEARCH_ALL_BUTNETWORK))
          if (DiskMask==1)
            break;
          else
            continue;
      }
      else if (SearchMode==SEARCH_ROOT)
        GetPathRootOne(Root,Root);
      else if(SearchMode==SEARCH_INPATH)
      {
        if(!*Ptr)
          break;
        xstrncpy(Root,Ptr,sizeof(Root)-1);
        Ptr+=strlen(Ptr)+1;
      }

      DoScanTree(Root, FindData, FullName, sizeof(FullName));

      if (SearchMode!=SEARCH_ALL && SearchMode!=SEARCH_ALL_BUTNETWORK && SearchMode!=SEARCH_INPATH)
        break;
    } // END: for (...

    while (!StopSearch && FindMessageReady)
      Sleep(10);
  //  sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);

    statusCS.Enter ();

    sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);
    SetFarTitle (FindMessage);

    SearchDone=TRUE;
    FindMessageReady=TRUE;

    statusCS.Leave ();

    PrepareFilesListUsed--;
  }
  EXCEPT (xfilter((int)(INT_PTR)INVALID_HANDLE_VALUE,GetExceptionInformation(),NULL,1))
  {
    TerminateProcess( GetCurrentProcess(), 1);
  }
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


void FindFiles::ArchiveSearch(char *ArcName)
{
  _ALGO(CleverSysLog clv("FindFiles::ArchiveSearch()"));
  _ALGO(SysLog("ArcName='%s'",(ArcName?ArcName:"NULL")));
  char *Buffer=new char[Opt.PluginMaxReadData];
  if ( !Buffer )
  {
    _ALGO(SysLog("ERROR: alloc buffer (size=%u)",Opt.PluginMaxReadData));
    return;
  }

  FILE *ProcessFile=fopen(ArcName,"rb");
  if (ProcessFile==NULL)
  {
    delete[] Buffer;
    _ALGO(SysLog("ERROR: open file '%s'",(ArcName?ArcName:"NULL")));
    return;
  }
  int ReadSize=(int)fread(Buffer,1,Opt.PluginMaxReadData,ProcessFile);
  fclose(ProcessFile);

  int SavePluginsOutput=DisablePluginsOutput;
  DisablePluginsOutput=TRUE;
  HANDLE hArc=CtrlObject->Plugins.OpenFilePlugin(ArcName,(unsigned char *)Buffer,ReadSize);
  DisablePluginsOutput=SavePluginsOutput;

  delete[] Buffer;

  if (hArc==(HANDLE)-2)
  {
    BreakMainThread=TRUE;
    _ALGO(SysLog("return: hArc==(HANDLE)-2"));
    return;
  }
  if (hArc==INVALID_HANDLE_VALUE)
  {
    _ALGO(SysLog("return: hArc==INVALID_HANDLE_VALUE"));
    return;
  }

  int SaveSearchMode=SearchMode;
  DWORD SaveArcIndex = FindFileArcIndex;
  {
    SearchMode=SEARCH_FROM_CURRENT;
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hArc,&Info);
    FindFileArcIndex = AddArcListItem(ArcName, hArc, Info.Flags, Info.CurDir);
    /* $ 11.12.2001 VVM
      - �������� ������� ����� ������� � ������.
        � ���� ������ �� ����� - �� ������ ��� ����� */
    {
      char SaveDirName[NM],SaveSearchPath[2*NM];
      int SaveListCount = FindListCount;
      /* $ 19.01.2003 KM
         �������� ���� ������ � �������, ��� ����� ����������.
      */
      xstrncpy(SaveSearchPath,PluginSearchPath,2*NM);
      /* KM $ */
      xstrncpy(SaveDirName, LastDirName, NM);
      *LastDirName = 0;
      PreparePluginList((void *)1);
      xstrncpy(PluginSearchPath,SaveSearchPath,2*NM);
      WaitForSingleObject(hPluginMutex,INFINITE);
      CtrlObject->Plugins.ClosePlugin(ArcList[FindFileArcIndex].hPlugin);
      ArcList[FindFileArcIndex].hPlugin = INVALID_HANDLE_VALUE;
      ReleaseMutex(hPluginMutex);
      if (SaveListCount == FindListCount)
        xstrncpy(LastDirName, SaveDirName, NM);
    }
    /* VVM $ */
  }
  FindFileArcIndex = SaveArcIndex;
  SearchMode=SaveSearchMode;
  _ALGO(SysLog("return: ok"));
}

/* $ 01.07.2001 IS
   ���������� FileMaskForFindFile ������ GetCommaWord
*/
int FindFiles::IsFileIncluded(PluginPanelItem *FileItem,char *FullName,DWORD FileAttr)
{
  CriticalSectionLock Lock (ffCS);

  int FileFound=FileMaskForFindFile.Compare(FullName);
  HANDLE hPlugin=INVALID_HANDLE_VALUE;
  if (FindFileArcIndex != LIST_INDEX_NONE)
    hPlugin = ArcList[FindFileArcIndex].hPlugin;
  while(FileFound)
  {
    /* $ 24.09.2003 KM
       ���� ������� ����� ������ hex-�����, ����� ����� � ����� �� ��������
    */
    /* $ 17.01.2002 VVM
      ! ��������� ������ � ������� � ������ ������� � ������ -
        ���� � ������� ������� ���� ������������ */
    if ((FileAttr & FILE_ATTRIBUTE_DIRECTORY) && ((Opt.FindOpt.FindFolders==0) || SearchHex))
//        ((hPlugin == INVALID_HANDLE_VALUE) ||
//        (ArcList[FindFileArcIndex].Flags & OPIF_FINDFOLDERS)==0))
      return FALSE;
    /* VVM $ */
    /* KM $ */

    if (*FindStr && FileFound)
    {
      FileFound=FALSE;
      if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
        break;
      char SearchFileName[NM];
      int RemoveTemp=FALSE;
      if ((hPlugin != INVALID_HANDLE_VALUE) && (ArcList[FindFileArcIndex].Flags & OPIF_REALNAMES)==0)
      {
        char TempDir[NM];
        FarMkTempEx(TempDir); // � �������� �� NULL???
        CreateDirectory(TempDir,NULL);
        WaitForSingleObject(hPluginMutex,INFINITE);
        if (!CtrlObject->Plugins.GetFile(hPlugin,FileItem,TempDir,SearchFileName,OPM_SILENT|OPM_FIND))
        {
          ReleaseMutex(hPluginMutex);
          FAR_RemoveDirectory(TempDir);
          break;
        }
        else
          ReleaseMutex(hPluginMutex);
        RemoveTemp=TRUE;
      }
      else
        xstrncpy(SearchFileName,FullName,sizeof(SearchFileName)-1);
      if (LookForString(SearchFileName))
        FileFound=TRUE;
      if (RemoveTemp)
        DeleteFileWithFolder(SearchFileName);
    }
    break;
  }
  return(FileFound);
}
/* IS $ */


void FindFiles::AddMenuRecord(char *FullName, WIN32_FIND_DATA *FindData)
{
  char MenuText[NM],FileText[NM],SizeText[30];
  char Attr[30],Date[30],DateStr[30],TimeStr[30];
  struct MenuItem ListItem;

  Dialog* Dlg=(Dialog*)hDlg;
  if(!Dlg)
  {
    return;
  }

  VMenu *ListBox=Dlg->Item[1].ListPtr;

  if (!ListBox)
  {
    return;
  }

  memset(&ListItem,0,sizeof(ListItem));

  if (FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    sprintf(SizeText,"%13s",MSG(MFindFileFolder));
  else
  {
    // ������� ����������� ������� � ������ 64-�������� ������� �����
    __int64 fsize=(unsigned __int64)FindData->nFileSizeLow|((unsigned __int64)FindData->nFileSizeHigh<<32);
    _ui64toa(fsize,SizeText,10);
  }
  char *DisplayName=FindData->cFileName;
  /* $ 24.03.2002 KM
     � �������� ������������� �������� ��������� � ����� �� ���
     ��� ����������� ��� ����������� � ������, �������� ����,
     �.�. ��������� ������� ���������� ��� ������ � ������ ����,
     � ������� ��������� ������.
  */
  if (FindFileArcIndex != LIST_INDEX_NONE)
    DisplayName=PointToName(DisplayName);
  /* KM $ */

  sprintf(FileText," %-26.26s %13.13s",DisplayName,SizeText);

  /* $ 05.10.2003 KM
     ��� ������������� ������� �� ���� ����� ���������� ��� ���
     ����, ������� ����� � �������.

     $ 24.01.2007 AY
     ��� ��� ������ ���� ������ ��� ���� ������ �� ���� ������ �
     ���� �� ���� ��� ������� �������.

  if (UseFilter && Filter->GetParams().FDate.Used)
  {
    switch(Filter->GetParams().FDate.DateType)
    {
      case FDATE_MODIFIED:
        // ���������� ���� ���������� ���������
        ConvertDate(FindData->ftLastWriteTime,DateStr,TimeStr,5);
        break;
      case FDATE_CREATED:
        // ���������� ���� ��������
        ConvertDate(FindData->ftCreationTime,DateStr,TimeStr,5);
        break;
      case FDATE_OPENED:
        // ���������� ���� ���������� �������
        ConvertDate(FindData->ftLastAccessTime,DateStr,TimeStr,5);
        break;
    }
  }
  else
  */
    // ���������� ���� ���������� ���������
    ConvertDate(FindData->ftLastWriteTime,DateStr,TimeStr,5);
  /* KM $ */
  sprintf(Date,"  %s  %s",DateStr,TimeStr);
  strcat(FileText,Date);

  /* $ 05.10.2003 KM
     ��������� � ������ ������ �������� ��������� ������
  */
  char AttrStr[16];
  DWORD FileAttr=FindData->dwFileAttributes;
  AttrStr[0]=(FileAttr & FILE_ATTRIBUTE_READONLY) ? 'R':' ';
  AttrStr[1]=(FileAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':' ';
  AttrStr[2]=(FileAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':' ';
  AttrStr[3]=(FileAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':' ';
  AttrStr[4]=(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT) ? 'L' : ((FileAttr & FILE_ATTRIBUTE_SPARSE_FILE) ? '$':' ');
  AttrStr[5]=(FileAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':((FileAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':' ');
  AttrStr[6]=(FileAttr & FILE_ATTRIBUTE_TEMPORARY) ? 'T':' ';
  AttrStr[7]=(FileAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) ? 'I':' ';
  AttrStr[8]=0;

  sprintf(Attr," %s",AttrStr);
  strcat(FileText,Attr);
  /* KM $ */
  sprintf(MenuText," %-*.*s",DlgWidth-3,DlgWidth-3,FileText);

  char PathName[2*NM];
  xstrncpy(PathName,FullName,sizeof(PathName)-1);
  PathName[sizeof(PathName)-1]=0;

  RemovePseudoBackSlash(PathName);

  *PointToName(PathName)=0;
  if (*PathName==0)
    strcpy(PathName,".\\");

  AddEndSlash(PathName);

  if (LocalStricmp(PathName,LastDirName)!=0)
  {
    if (*LastDirName)
    {
      /* $ 12.05.2001 DJ
         ������ �� ��������������� �� ������ ������� ����� ����������
      */
      ListItem.Flags|=LIF_DISABLE;
      // � ������ VVM ������� ���������� � ������ ������� LIST_INDEX_NONE �� ������ �������
      ListBox->SetUserData((void*)(DWORD_PTR)LIST_INDEX_NONE,sizeof(LIST_INDEX_NONE),ListBox->AddItem(&ListItem));
      ListItem.Flags&=~LIF_DISABLE;
      /* DJ $ */
    }
    ffCS.Enter ();

    xstrncpy(LastDirName,PathName,sizeof(LastDirName)-1);
    if ((FindFileArcIndex != LIST_INDEX_NONE) &&
        (!(ArcList[FindFileArcIndex].Flags & OPIF_REALNAMES)) &&
        (ArcList[FindFileArcIndex].ArcName) &&
        (*ArcList[FindFileArcIndex].ArcName))
    {
      char ArcPathName[NM*2];
      sprintf(ArcPathName,"%s:%s",ArcList[FindFileArcIndex].ArcName,*PathName=='.' ? "\\":PathName);
      xstrncpy(PathName,ArcPathName,sizeof(PathName)-1);
    }
    xstrncpy(SizeText,MSG(MFindFileFolder),sizeof(SizeText)-1);
    sprintf(FileText,"%-50.50s  <%6.6s>",TruncPathStr(PathName,50),SizeText);
    sprintf(ListItem.Name,"%-*.*s",DlgWidth-2,DlgWidth-2,FileText);

    DWORD ItemIndex = AddFindListItem(FindData);
    if (ItemIndex != LIST_INDEX_NONE)
    {
      // ������� ������ � FindData. ��� ��� �� �����
      memset(&FindList[ItemIndex].FindData,0,sizeof(FindList[ItemIndex].FindData));
      // ���������� LastDirName, �.�. PathName ��� ����� ���� ��������
      xstrncpy(FindList[ItemIndex].FindData.cFileName, LastDirName,
              sizeof(FindList[ItemIndex].FindData.cFileName)-1);
      /* $ 07.04.2002 KM
        - ������ ������� ����� ���������� ����, � ���������
          ������ �� �������� ������� �� ������� �� ������.
          Used=0 - ��� �� �������� �� ��������� ������.
      */
      FindList[ItemIndex].Used=0;
      /* KM $ */
      // �������� ������� � ��������, ���-�� �� �� ��� ������ :)
      FindList[ItemIndex].FindData.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
      if (FindFileArcIndex != LIST_INDEX_NONE)
        FindList[ItemIndex].ArcIndex = FindFileArcIndex;

      ListBox->SetUserData((void*)(DWORD_PTR)ItemIndex,sizeof(ItemIndex),
                           ListBox->AddItem(&ListItem));
    }

    ffCS.Leave ();
  }

  /* $ 24.03.2002 KM
     ������������� ��������� � ������ ����������
     �����. ��� ���� � 1.65 � � 3 ����, �� ���
     ������ ��������� ������ � ��� ���-�� �������,
     ������ ��������� �� �����.
  */

  ffCS.Enter ();

  DWORD ItemIndex = AddFindListItem(FindData);
  if (ItemIndex != LIST_INDEX_NONE)
  {
    xstrncpy(FindList[ItemIndex].FindData.cFileName, FullName,
            sizeof(FindList[ItemIndex].FindData.cFileName)-1);
    /* $ 07.04.2002 KM
       Used=1 - ��� �������� �� ��������� ������.
    */
    FindList[ItemIndex].Used=1;
    /* KM $ */
    if (FindFileArcIndex != LIST_INDEX_NONE)
      FindList[ItemIndex].ArcIndex = FindFileArcIndex;
  }
  xstrncpy(ListItem.Name,MenuText,sizeof(ListItem.Name)-1);

  ffCS.Leave ();
  /* $ 17.01.2002 VVM
    ! �������� ����� �� � ���������, � � ������. ���� �� �������� ��������� */
//  ListItem.SetSelect(!FindFileCount);

  int ListPos = ListBox->AddItem(&ListItem);
  ListBox->SetUserData((void*)(DWORD_PTR)ItemIndex,sizeof(ItemIndex), ListPos);
  // ������� ��� �������� - � ������.
  if (!FindFileCount && !FindDirCount)
    ListBox->SetSelectPos(ListPos, -1);
  /* VVM $ */

  statusCS.Enter();

  if (FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    FindDirCount++;
  else
    FindFileCount++;

  statusCS.Leave();
  /* KM $ */

  LastFoundNumber++;
  FindCountReady=TRUE;

}


int FindFiles::LookForString(char *Name)
{
  int Length;
  if ((Length=(int)strlen(FindStr))==0)
    return(TRUE);

  HANDLE FileHandle=FAR_CreateFile (Name, FILE_READ_ATTRIBUTES|FILE_READ_DATA|FILE_WRITE_ATTRIBUTES,
                                    FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                                    FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  FILETIME LastAccess;
  int TimeRead=0;
  if (FileHandle==INVALID_HANDLE_VALUE)
  {
    FileHandle=FAR_CreateFile (Name, FILE_READ_DATA, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
                               OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
  }
  else
    TimeRead=GetFileTime (FileHandle, NULL, &LastAccess, NULL);

  if (FileHandle==INVALID_HANDLE_VALUE) return (FALSE);

  char Buf[32768],SaveBuf[32768],CmpStr[sizeof(FindStr)];
  int ReadSize,SaveReadSize;

  if (SearchHex)
  {
    int LenCmpStr=sizeof(CmpStr);

    Transform((unsigned char *)CmpStr,LenCmpStr,(char *)FindStr,'S');
    Length=LenCmpStr;
  }
  else
    xstrncpy(CmpStr,FindStr,sizeof(CmpStr)-1);

  if (!CmpCase && !SearchHex)
    LocalStrupr(CmpStr);

  int FirstIteration=TRUE;
  int ReverseBOM=FALSE;
  int IsFirst=FALSE;

  // ��� ������� �� �����. ������������ ��� ���������
  // � ������������ ��������, � ������� ������������ �����
  __int64 AlreadyRead=_i64(0);

  while ( !StopSearch && ReadFile (FileHandle, Buf, sizeof(Buf)/sizeof(Buf[0]), ((LPDWORD)&ReadSize), NULL) )
  {
    if (ReadSize==0) break;
    /* $ 12.04.2005 KM
       ���� ������������ ����������� �� ������ �� ������ ������ �� �����,
       �������� �� ������� �� �� ��� ������� ����������� ����������� �������
    */
    if (EnableSearchInFirst && SearchInFirst)
    {
      if (AlreadyRead+ReadSize>SearchInFirst)
      {
        ReadSize=static_cast<int>(SearchInFirst-AlreadyRead);
      }

      if (ReadSize<=0) break;

      AlreadyRead+=ReadSize;
    }

    /* $ 11.09.2005 KM
       - Bug #1377
    */
    int DecodeTableNum=0;
    int ANSISearch=UseANSI;
    int UnicodeSearch=UseUnicode;
    int RealReadSize=ReadSize;

    // ������� � ������ ���������� ���������� ���������: OEM, ANSI � Unicode
    int UseInnerTables=true;

    /* $ 22.09.2003 KM
       ����� �� hex-�����
    */
    if (!SearchHex)
    {
      if (UseAllTables || UseUnicode)
      {
        // ��� ����� ��� �� ���� ���������� ��� � �������,
        // �������� ��������� ������ ������
        SaveReadSize=ReadSize;

        // � ����� �������� ��������� �����.
        memcpy(SaveBuf,Buf,ReadSize);

        /* $ 21.10.2000 SVS
           ������� ����������, � ��� ��� �������, �� ���� � FFFE-������
        */
        if(!IsFirst)
        {
          IsFirst=TRUE;
          if(*(WORD*)Buf == 0xFFFE) // The text contains the Unicode
             ReverseBOM=TRUE;       // byte-reversed byte-order mark
                                    // (Reverse BOM) 0xFFFE as its first character.
        }
      }
    }

    while (1)
    {
      /* $ 22.09.2003 KM
         ����� �� hex-�����
      */
      if (!SearchHex)
      {
        if (UnicodeSearch)
        {
          char BOMBuf[sizeof(SaveBuf)];
          char *BufPtr=SaveBuf;

          if(ReverseBOM)
          {
            BufPtr=BOMBuf;

            for(int I=0; I < SaveReadSize; I+=2)
            {
              BOMBuf[I]=SaveBuf[I+1];
              BOMBuf[I+1]=SaveBuf[I];
            }
          }

          WideCharToMultiByte(CP_OEMCP,0,(LPCWSTR)BufPtr,SaveReadSize/2,Buf,sizeof(Buf),NULL,NULL);
          ReadSize=SaveReadSize/2;
        }
        else
        {
          // ���� ����� ��� �� ���� ����������, ����������� ����������� �����
          // � ��� ������ ��� ������ � ������������ ��������� ������ � ������ ���������
          if (UseAllTables)
          {
            ReadSize=SaveReadSize;
            memcpy(Buf,SaveBuf,ReadSize);
          }

          /* $ 20.09.2003 KM
             ������� ��������� ANSI �������
          */
          if (UseDecodeTable || DecodeTableNum>0 || ANSISearch)
          {
            // ��� ��������� �� ������ � ANSI ���������, ���������� ������� �������������
            if (ANSISearch)
              GetTable(&TableSet,TRUE,TableNum,UseUnicode);

            for (int I=0;I<ReadSize;I++)
              Buf[I]=TableSet.DecodeTable[Buf[I]];
          }
        }
        if (!CmpCase)
          LocalUpperBuf(Buf,ReadSize);
      }

      int CheckSize=ReadSize-Length+1;
      /* $ 30.07.2000 KM
         ��������� "Whole words" � ������
      */
      /* $ 26.05.2002 KM
          ���������� ������ � ������ �� ����� ������.
      */
      for (int I=0;I<CheckSize;I++)
      {
        int cmpResult;
        int locResultLeft=FALSE;
        int locResultRight=FALSE;

        /* $ 22.09.2003 KM
           ����� �� hex-�����
        */
        if (WholeWords && !SearchHex)
        {
          if (!FirstIteration)
          {
            if (IsSpace(Buf[I-1]) || IsEol(Buf[I-1]) ||
               (strchr(Opt.WordDiv,Buf[I-1])!=NULL))
              locResultLeft=TRUE;
          }
          else
          {
            FirstIteration=FALSE;
            locResultLeft=TRUE;
          }

          if (RealReadSize!=sizeof(Buf) && I+Length>=RealReadSize)
            locResultRight=TRUE;
          else
            if (I+Length<RealReadSize &&
               (IsSpace(Buf[I+Length]) || IsEol(Buf[I+Length]) ||
               (strchr(Opt.WordDiv,Buf[I+Length])!=NULL)))
              locResultRight=TRUE;
        }
        else
        {
          locResultLeft=TRUE;
          locResultRight=TRUE;
        }

        cmpResult=locResultLeft && locResultRight && CmpStr[0]==Buf[I] &&
          (Length==1 || CmpStr[1]==Buf[I+1] &&
          (Length==2 || memcmp(CmpStr+2,&Buf[I+2],Length-2)==0));

        if (cmpResult)
        {
          if (TimeRead)
            SetFileTime(FileHandle,NULL,&LastAccess,NULL);
          CloseHandle (FileHandle);
          return(TRUE);
        }
      }

      /* $ 22.09.2003 KM
         ����� �� hex-�����
      */
      if (UseAllTables && !SearchHex)
      {
        if (!ANSISearch && !UnicodeSearch && UseInnerTables)
        {
          // ��� �� ����� � OEM ���������, ������ ������ � ANSI.
          ANSISearch=true;
        }
        else
        {
          if (!UnicodeSearch && UseInnerTables)
          {
            // �� ����� � ANSI ���������, ������ ������ � Unicode.
            UnicodeSearch=true;
            ANSISearch=false;
          }
          else
          {
            // ���������� ������� ��� ������, ������ �������� ������ � ����������������
            UseInnerTables=false;

            UnicodeSearch=false;
            ANSISearch=false;

            // �� ����� �� � OEM, �� � ANSI, �� � Unicode, ������ ����� ������
            // � ������������� ���������������� ����������.
            if (PrepareTable(&TableSet,DecodeTableNum,TRUE))
            {
              DecodeTableNum++;
              xstrncpy(CmpStr,FindStr,sizeof(CmpStr)-1);
              if (!CmpCase)
                LocalStrupr(CmpStr);
            }
            else
              break;
          }
        }
      }
      else
        break;
    }

    if (RealReadSize==sizeof(Buf)/sizeof(Buf[0]))
    {
      /* $ 22.09.2003 KM
         ����� �� hex-�����
      */
      /* $ 30.07.2000 KM
         ��������� offset ��� ������ ������ ����� � ������ WordDiv
      */
      //��� ������ �� ���� �������� �� �� ���� ��� ����� ���������� ����� � � �������
      //����� �� �������� FileSize/sizeof(Buf)*(Length+1) ���� ����� �������
      //�� ���� ��� �� ������ �� ��� ������ �� ���� �������� � ������� �� �����
      //��������� ���� ���������� ������.
      int offset=(Length/*+1*/);
      if ((UseAllTables || UnicodeSearch) && !SearchHex) offset*=2;

      if ( INVALID_SET_FILE_POINTER != SetFilePointer (FileHandle, -offset, NULL, FILE_CURRENT) )
      {
        if ((EnableSearchInFirst && SearchInFirst) ) AlreadyRead-=offset;
      }
    }
  }

  if (TimeRead)
    SetFileTime(FileHandle,NULL,&LastAccess,NULL);
  CloseHandle (FileHandle);
  return(FALSE);
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif
void _cdecl FindFiles::PreparePluginList(void *Param)
{
  char SaveDir[NM];

  TRY {

    Sleep(200);
    *PluginSearchPath=0;
    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
    /* $ 15.10.2001 VVM */
    HANDLE hPlugin=ArcList[FindFileArcIndex].hPlugin;
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
    xstrncpy(SaveDir,Info.CurDir,sizeof(SaveDir)-1);
    WaitForSingleObject(hPluginMutex,INFINITE);
    if (SearchMode==SEARCH_ROOT ||
        SearchMode==SEARCH_ALL ||
        SearchMode==SEARCH_ALL_BUTNETWORK ||
        SearchMode==SEARCH_INPATH)
      CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_FIND);
    ReleaseMutex(hPluginMutex);
    RecurseLevel=0;
    ScanPluginTree(hPlugin,ArcList[FindFileArcIndex].Flags);
    /* VVM $ */
    WaitForSingleObject(hPluginMutex,INFINITE);
    if (SearchMode==SEARCH_ROOT ||
        SearchMode==SEARCH_ALL ||
        SearchMode==SEARCH_ALL_BUTNETWORK ||
        SearchMode==SEARCH_INPATH)
      CtrlObject->Plugins.SetDirectory(hPlugin,SaveDir,OPM_FIND);
    ReleaseMutex(hPluginMutex);
    while (!StopSearch && FindMessageReady)
      Sleep(10);
    if (Param==NULL)
    {
      statusCS.Enter();

      sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);
      FindMessageReady=TRUE;
      SearchDone=TRUE;

      statusCS.Leave();
    }
  }
  EXCEPT (xfilter((int)(INT_PTR)INVALID_HANDLE_VALUE,GetExceptionInformation(),NULL,1))
  {
    TerminateProcess( GetCurrentProcess(), 1);
  }
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif

void FindFiles::ScanPluginTree(HANDLE hPlugin, DWORD Flags)
{
  PluginPanelItem *PanelData=NULL;
  int ItemCount=0;

  WaitForSingleObject(hPluginMutex,INFINITE);
  if (StopSearch || !CtrlObject->Plugins.GetFindData(hPlugin,&PanelData,&ItemCount,OPM_FIND))
  {
    ReleaseMutex(hPluginMutex);
    return;
  }
  else
    ReleaseMutex(hPluginMutex);
  RecurseLevel++;

  /* $ 19.01.2003 KM
     ������� ���� ����������, ���-�� ���������� ����������
     �� ������ ��, ������� � ������.
  */
  /* $ 24.03.2002 KM
     ���������� � �������� �� ������ ����� ������ � ������,
     ���� ��� ����� �������� � ���� ����, � �� ������ ���
     OPIF_REALNAMES, � ��������� ������ � ����������� ������
     ���������� ���������� ���������� ������ ��������������
     ���������� ����� �� ������.
  */
//  if (PanelData && strlen(PointToName(PanelData->FindData.cFileName))>0)
//    far_qsort((void *)PanelData,ItemCount,sizeof(*PanelData),SortItems);
  /* KM $ */
  /* KM $ */

  if (SearchMode!=SEARCH_SELECTED || RecurseLevel!=1)
  {
    for (int I=0;I<ItemCount && !StopSearch;I++)
    {
      while (PauseSearch)
        Sleep(10);

      PluginPanelItem *CurPanelItem=PanelData+I;
      char *CurName=CurPanelItem->FindData.cFileName;
      char FullName[2*NM];
      if (strcmp(CurName,".")==0 || TestParentFolderName(CurName))
        continue;
//      char AddPath[2*NM];
      if (Flags & OPIF_REALNAMES)
      {
        xstrncpy(FullName,CurName,sizeof(FullName)-1);
//        strcpy(AddPath,CurName);
//        *PointToName(AddPath)=0;
      }
      else
      {
        sprintf(FullName,"%s%s",PluginSearchPath,CurName);
//        strcpy(AddPath,PluginSearchPath);
      }

      /* $ 30.09.2003 KM
        ����������� ����� �� ���������� � ����������� ������
      */
      if (!UseFilter || Filter->FileInFilter(&CurPanelItem->FindData))
      {
        /* $ 14.06.2004 KM
          ��������� �������� ��� ��������� ���������
        */
        if (CurPanelItem->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          statusCS.Enter();

          xstrncpy(FindMessage,FullName,sizeof(FindMessage)-1);
          FindMessage[sizeof(FindMessage)-1]=0;
          RemovePseudoBackSlash(FindMessage);
          FindMessageReady=TRUE;

          statusCS.Leave();

        }
        /* KM $ */

        if (IsFileIncluded(CurPanelItem,CurName,CurPanelItem->FindData.dwFileAttributes))
          AddMenuRecord(FullName,&CurPanelItem->FindData);

        if (SearchInArchives && (hPlugin != INVALID_HANDLE_VALUE) && (Flags & OPIF_REALNAMES))
          ArchiveSearch(FullName);
      }
      /* KM $ */
    }
  }
  if (SearchMode!=SEARCH_CURRENT_ONLY)
  {
    for (int I=0;I<ItemCount && !StopSearch;I++)
    {
      PluginPanelItem *CurPanelItem=PanelData+I;
      char *CurName=CurPanelItem->FindData.cFileName;
      // ���� ������� ������ "� ��������" � ��� ��� �������� �������� �������,
      // �� ����������  �� ������ ��� �������� �� � �� ��� ���� (�� ������)
      if ((CurPanelItem->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
          strcmp(CurName,".")!=0 && !TestParentFolderName(CurName) &&
          (!UseFilter || !Filter->FileInFilter(&CurPanelItem->FindData, true)) &&
          (SearchMode!=SEARCH_SELECTED || RecurseLevel!=1 ||
          CtrlObject->Cp()->ActivePanel->IsSelected(CurName)))
      {
        WaitForSingleObject(hPluginMutex,INFINITE);
        if (strchr(CurName,'\x1')==NULL && CtrlObject->Plugins.SetDirectory(hPlugin,CurName,OPM_FIND))
        {
          ReleaseMutex(hPluginMutex);
          /* $ 19.01.2003 KM
             ���� ����� � ��� �������� �� ������������
             PluginSearchPath, �� ������ ������������ ������
             ����� � ���� � ����� ����������, ��� �� ���� ���.
          */
          int SearchPathLen=(int)strlen(PluginSearchPath);
          int CurNameLen=(int)strlen(CurName);
          if (SearchPathLen+CurNameLen<NM-2)
          {
            strcat(PluginSearchPath,CurName);
            strcat(PluginSearchPath,"\x1");
            ScanPluginTree(hPlugin, Flags);
            char *Ptr=strrchr(PluginSearchPath,'\x1');
            if (Ptr!=NULL)
              *Ptr=0;
          }
          /* KM $ */
          char *NamePtr=strrchr(PluginSearchPath,'\x1');
          if (NamePtr!=NULL)
            *(NamePtr+1)=0;
          else
            *PluginSearchPath=0;
          WaitForSingleObject(hPluginMutex,INFINITE);
          if (!CtrlObject->Plugins.SetDirectory(hPlugin,"..",OPM_FIND))
            StopSearch=TRUE;
          ReleaseMutex(hPluginMutex);
          if (StopSearch) break;
        }
        else
          ReleaseMutex(hPluginMutex);
      }
    }
  }
  CtrlObject->Plugins.FreeFindData(hPlugin,PanelData,ItemCount);
  RecurseLevel--;
}

void _cdecl FindFiles::WriteDialogData(void *Param)
{
  FarDialogItemData ItemData;
  char DataStr[NM];

  WriteDataUsed=TRUE;

  while( true )
  {
    Dialog* Dlg=(Dialog*)hDlg;

    if( !Dlg )
      break;

    VMenu *ListBox=Dlg->Item[1].ListPtr;

    if (ListBox && !PauseSearch && !ScreenSaverActive)
    {
      if (BreakMainThread)
        StopSearch=TRUE;

      if (FindCountReady)
      {
        statusCS.Enter ();

        sprintf(DataStr," %s: %d ",MSG(MFindFound),FindFileCount+FindDirCount);
        ItemData.PtrData=DataStr;
        ItemData.PtrLength=(int)strlen(DataStr);

        statusCS.Leave ();

        Dialog::SendDlgMessage(hDlg,DM_SETTEXT,2,(LONG_PTR)&ItemData);

        FindCountReady=FALSE;
      }

      if (FindMessageReady)
      {
        char SearchStr[NM];

        if (*FindStr)
        {
          char Temp[NM],FStr[NM*2];

          xstrncpy(FStr,FindStr,sizeof(FStr)-1);
          sprintf(Temp," \"%s\"",TruncStrFromEnd(FStr,10));
          sprintf(SearchStr,MSG(MFindSearchingIn),Temp);
        }
        else
          sprintf(SearchStr,MSG(MFindSearchingIn),"");

        int Wid1=(int)strlen(SearchStr);
        int Wid2=DlgWidth-(int)strlen(SearchStr)-1;

        if (SearchDone)
        {
          Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

          xstrncpy(DataStr,MSG(MFindCancel),sizeof(DataStr)-1);
          ItemData.PtrData=DataStr;
          ItemData.PtrLength=(int)strlen(DataStr);
          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,9,(LONG_PTR)&ItemData);

          statusCS.Enter ();

          sprintf(DataStr,"%-*.*s",DlgWidth,DlgWidth,FindMessage);
          ItemData.PtrData=DataStr;
          ItemData.PtrLength=(int)strlen(DataStr);

          statusCS.Leave ();

          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,3,(LONG_PTR)&ItemData);

          ItemData.PtrData="";
          ItemData.PtrLength=(int)strlen(DataStr);
          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,2,(LONG_PTR)&ItemData);

          Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
          SetFarTitle(FindMessage);
          StopSearch=TRUE;
        }
        else
        {
          statusCS.Enter ();

          sprintf(DataStr,"%-*.*s %-*.*s",Wid1,Wid1,SearchStr,Wid2,Wid2,TruncPathStr(FindMessage,Wid2));
          ItemData.PtrData=DataStr;
          ItemData.PtrLength=(int)strlen(DataStr);

          statusCS.Leave ();

          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,3,(LONG_PTR)&ItemData);
        }

        FindMessageReady=FALSE;
      }

      if (LastFoundNumber && ListBox)
      {
        LastFoundNumber=0;

        if ( ListBox->UpdateRequired () )
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,1,1);
      }
    }

    if (StopSearch && SearchDone && !FindMessageReady && !FindCountReady && !LastFoundNumber)
        break;

    Sleep(20);
  }

  WriteDataUsed=FALSE;
}

BOOL FindFiles::FindListGrow()
{
  DWORD Delta = (FindListCapacity < 256)?LIST_DELTA:FindListCapacity/2;
  LPFINDLIST NewList = (LPFINDLIST)xf_realloc(FindList, (FindListCapacity + Delta) * sizeof(FINDLIST));
  if (NewList)
  {
    FindList = NewList;
    FindListCapacity+= Delta;
    return(TRUE);
  }
  return(FALSE);
}

BOOL FindFiles::ArcListGrow()
{
  DWORD Delta = (ArcListCapacity < 256)?LIST_DELTA:ArcListCapacity/2;
  LPARCLIST NewList = (LPARCLIST)xf_realloc(ArcList, (ArcListCapacity + Delta) * sizeof(ARCLIST));
  if (NewList)
  {
    ArcList = NewList;
    ArcListCapacity+= Delta;
    return(TRUE);
  }
  return(FALSE);
}

DWORD FindFiles::AddFindListItem(WIN32_FIND_DATA *FindData)
{
  if ((FindListCount == FindListCapacity) &&
      (!FindListGrow()))
    return(LIST_INDEX_NONE);
  FindList[FindListCount].FindData = *FindData;
  FindList[FindListCount].ArcIndex = LIST_INDEX_NONE;
  return(FindListCount++);
}

DWORD FindFiles::AddArcListItem(const char *ArcName, HANDLE hPlugin,
                                DWORD dwFlags, const char *RootPath)
{
  if ((ArcListCount == ArcListCapacity) &&
      (!ArcListGrow()))
    return(LIST_INDEX_NONE);
  xstrncpy(ArcList[ArcListCount].ArcName, NullToEmpty(ArcName),
          sizeof(ArcList[ArcListCount].ArcName)-1);
  ArcList[ArcListCount].hPlugin = hPlugin;
  ArcList[ArcListCount].Flags = dwFlags;
  xstrncpy(ArcList[ArcListCount].RootPath, NullToEmpty(RootPath),
          sizeof(ArcList[ArcListCount].RootPath)-1);
  AddEndSlash(ArcList[ArcListCount].RootPath);
  return(ArcListCount++);
}

void FindFiles::ClearAllLists()
{
  CriticalSectionLock Lock(ffCS);

  FindFileArcIndex = LIST_INDEX_NONE;

  if (FindList)
    xf_free(FindList);
  FindList = NULL;
  FindListCapacity = FindListCount = 0;

  if (ArcList)
    xf_free(ArcList);
  ArcList = NULL;
  ArcListCapacity = ArcListCount = 0;
}

char *FindFiles::PrepareDriveNameStr(char *SearchFromRoot,size_t sz)
{
  char CurDir[NM*2],MsgStr[NM],MsgStr1[NM];
  int MsgLen,DrvLen,MsgLenDiff;

  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  PluginMode=ActivePanel->GetMode()==PLUGIN_PANEL && ActivePanel->IsVisible();

  CtrlObject->CmdLine->GetCurDir(CurDir);
  GetPathRootOne(CurDir,CurDir);
  if (CurDir[strlen(CurDir)-1]=='\\')
    CurDir[strlen(CurDir)-1]=0;

  if (*CurDir==0 || PluginMode)
  {
    xstrncpy(SearchFromRoot,MSG(MSearchFromRootFolder),sz-1);
    xstrncpy(MsgStr1,SearchFromRoot,sizeof(MsgStr1)-1);

    RemoveHighlights(MsgStr1);
    MsgLen=(int)strlen(MsgStr1);

    // ������� � ����� ����� � '&' � ���. ����� ��� �����������
    // ����� ������ ����� ������, ��� ����� '&'
    MsgLenDiff=(int)strlen(SearchFromRoot)-MsgLen;
  }
  else
  {
    xstrncpy(MsgStr,MSG(MSearchFromRootOfDrive),sizeof(MsgStr)-1);
    xstrncpy(MsgStr1,MsgStr,sizeof(MsgStr1)-1);

    RemoveHighlights(MsgStr1);
    MsgLen=(int)strlen(MsgStr1);

    // ������� � ����� ����� � '&' � ���. ����� ��� �����������
    // ����� ������ ����� ������, ��� ����� '&'
    MsgLenDiff=(int)strlen(MsgStr)-MsgLen;

    DrvLen=(int)sz-MsgLen-1-MsgLenDiff; // -1 - ��� ������ ����� ������� � ������, -MsgLenDiff - ��� ���� ������� '&'
    if (DrvLen<7)
    {
      DrvLen=7; // ������� ����������� ������ ����� ����� 7 ��������
                // (���� ������ TruncPathStr � UNC, ����� ���� ���-�� ���� �����)
      MsgLen=(int)sz-DrvLen-1-MsgLenDiff;
    }

    sprintf(SearchFromRoot,"%s %s",TruncStrFromEnd(MsgStr,MsgLen+MsgLenDiff),TruncPathStr(CurDir,DrvLen));
  }
  SearchFromRoot[sz-1+MsgLenDiff]=0;

  return SearchFromRoot;
}

char *FindFiles::RemovePseudoBackSlash(char *FileName)
{
  for (int i=0;FileName[i]!=0;i++)
  {
    if (FileName[i]=='\x1')
      FileName[i]='\\';
  }
  return FileName;
}


/* $ 12.05.2004 KM
   ������� ������, � ������� ����� ������������� �����,
   �������� ��������� �������� ���������
*/
__int64 __fastcall FindFiles::GetSearchInFirst(char *DigitStr)
{
    __int64 LocalSize=_i64(0);

  if (*DigitStr)
  {
    switch(SearchInFirstIndex)
    {
      case FSIZE_INBYTES:
        LocalSize=_atoi64(DigitStr);
        break;
      case FSIZE_INKBYTES:
        // ������ ����� � ����������, �������� ��� � �����.
        LocalSize=_atoi64(DigitStr)*_i64(1024);
        break;
      case FSIZE_INMBYTES:
        // ������ ����� � ����������, �������� ��� � �����.
        LocalSize=_atoi64(DigitStr)*_i64(1024)*_i64(1024);
        break;
      case FSIZE_INGBYTES:
        // ������ ����� � ����������, �������� ��� � �����.
        LocalSize=_atoi64(DigitStr)*_i64(1024)*_i64(1024)*_i64(1024);
        break;
      default:
        break;
    }
  }
  else
  {
    LocalSize=_i64(0);
  }

  return LocalSize;
}
/* KM $ */
