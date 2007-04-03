/*
filelist.cpp

�������� ������ - ����� �������

*/

#include "headers.hpp"
#pragma hdrstop

#include "filelist.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "keys.hpp"
#include "macroopcode.hpp"
#include "lang.hpp"
#include "ctrlobj.hpp"
#include "filefilter.hpp"
#include "dialog.hpp"
#include "vmenu.hpp"
#include "cmdline.hpp"
#include "manager.hpp"
#include "filepanels.hpp"
#include "help.hpp"
#include "fileedit.hpp"
#include "namelist.hpp"
#include "fileview.hpp"
#include "copy.hpp"
#include "history.hpp"
#include "qview.hpp"
#include "rdrwdsk.hpp"
#include "plognmn.hpp"
#include "scrbuf.hpp"
#include "CFileMask.hpp"

extern struct PanelViewSettings ViewSettingsArray[];

static int _cdecl SortList(const void *el1,const void *el2);
int _cdecl SortSearchList(const void *el1,const void *el2);

static int ListSortMode,ListSortOrder,ListSortGroups,ListSelectedFirst;
static int ListPanelMode,ListCaseSensitive,ListNumericSort;
static HANDLE hSortPlugin;

enum SELECT_MODES {SELECT_INVERT,SELECT_INVERTALL,SELECT_ADD,SELECT_REMOVE,
    SELECT_ADDEXT,SELECT_REMOVEEXT,SELECT_ADDNAME,SELECT_REMOVENAME};

FileList::FileList()
{
  _OT(SysLog("[%p] FileList::FileList()", this));
  /* $ 09.11.2001 IS
       ����������������� ���� "������"
  */
  {
    char *data=MSG(MPanelBracketsForLongName);
    if(strlen(data)>1)
    {
      *openBracket=data[0];
      *closeBracket=data[1];
    }
    else
    {
      *openBracket='{';
      *closeBracket='}';
    }
    openBracket[1]=closeBracket[1]=0;
  }
  /* IS $ */
  Type=FILE_PANEL;
  FarGetCurDir(sizeof(CurDir),CurDir);
  hPlugin=INVALID_HANDLE_VALUE;
  Filter=NULL;
  ListData=NULL;
  FileCount=0;
  UpperFolderTopFile=0;
  CurTopFile=CurFile=0;
  LastCurFile=-1;
  ShowShortNames=0;
  MouseSelection=0;
  SortMode=BY_NAME;
  SortOrder=1;
  SortGroups=0;
  SelectedFirst=0;
  ViewMode=VIEW_3;
  ViewSettings=ViewSettingsArray[ViewMode];
  NumericSort=0;
  Columns=PreparePanelView(&ViewSettings);
  Height=0;
  PluginsStack=NULL;
  PluginsStackSize=0;
  ShiftSelection=-1;
  hListChange=INVALID_HANDLE_VALUE;
  SelFileCount=0;
  SelFileSize=0;
  TotalFileCount=0;
  TotalFileSize=0;
  FreeDiskSize=0;
  ReturnCurrentFile=FALSE;
  LastUpdateTime=0;
  PluginCommand=-1;
  DataToDeleteCount=0;
  PrevDataStack=NULL;
  PrevDataStackSize=0;
  LeftPos=0;
  UpdateRequired=FALSE;
  AccessTimeUpdateRequired=FALSE;
  *PluginDizName=0;
  DizRead=FALSE;
  InternalProcessKey=FALSE;
  GetSelPosition = 0;

  Is_FS_NTFS=FALSE;
}


FileList::~FileList()
{
  _OT(SysLog("[%p] FileList::~FileList()", this));
  CloseChangeNotification();
  struct PrevDataItem *CurPrevDataStack=PrevDataStack;
  for (int I=0;I < PrevDataStackSize; I++, CurPrevDataStack++)
    DeleteListData(CurPrevDataStack->PrevListData,CurPrevDataStack->PrevFileCount);
  /* $ 29.11.2001 DJ
     �������� ����� realloc - ����������� ���� ����� free
  */
  if(PrevDataStack) xf_free (PrevDataStack);
  /* DJ $ */
  DeleteListData(ListData,FileCount);
  if (PanelMode==PLUGIN_PANEL)
    while (PopPlugin(FALSE))
      ;
  DeleteAllDataToDelete();
  delete Filter;
}


void FileList::DeleteListData(struct FileListItem *(&ListData),long &FileCount)
{
  if (ListData==NULL)
    return;

  struct FileListItem *CurPtr=ListData;
  for (int I=0;I<FileCount;I++,CurPtr++)
  {
    if (CurPtr->CustomColumnNumber>0 && CurPtr->CustomColumnData!=NULL)
    {
      for (int J=0; J < CurPtr->CustomColumnNumber; J++)
        delete[] CurPtr->CustomColumnData[J];
      delete[] CurPtr->CustomColumnData;
    }
    /* $ 18.01.2003 VVM
      - �������� ����� malloc() � ����������� ����� ����� free() */
    if (CurPtr->UserFlags & PPIF_USERDATA)
      xf_free((void *)CurPtr->UserData);
    /* VVM $ */
    if (CurPtr->DizText && CurPtr->DeleteDiz)
      delete[] CurPtr->DizText;
  }
  xf_free(ListData);
  ListData=NULL;
  FileCount=0;
}


void FileList::DeleteAllDataToDelete()
{
  while (DataToDeleteCount>0)
  {
    DataToDeleteCount--;
    DeletePluginItemList(DataToDelete[DataToDeleteCount],DataSizeToDelete[DataToDeleteCount]);
  }
}


void FileList::Up(int Count)
{
  CurFile-=Count;
  if(CurFile < 0)
    CurFile=0;
  ShowFileList(TRUE);
}


void FileList::Down(int Count)
{
  CurFile+=Count;
  if(CurFile >= FileCount)
    CurFile=FileCount-1;
  ShowFileList(TRUE);
}

void FileList::Scroll(int Count)
{
  CurTopFile+=Count;
  if (Count<0)
    Up(-Count);
  else
    Down(Count);
}

void FileList::CorrectPosition()
{
  if (FileCount==0)
  {
    CurFile=CurTopFile=0;
    return;
  }
  if (CurTopFile+Columns*Height>FileCount)
    CurTopFile=FileCount-Columns*Height;
  if (CurFile<0)
    CurFile=0;
  if (CurFile > FileCount-1)
    CurFile=FileCount-1;
  if (CurTopFile<0)
    CurTopFile=0;
  if (CurTopFile > FileCount-1)
    CurTopFile=FileCount-1;
  if (CurFile<CurTopFile)
    CurTopFile=CurFile;
  if (CurFile>CurTopFile+Columns*Height-1)
    CurTopFile=CurFile-Columns*Height+1;
}


void FileList::SortFileList(int KeepPosition)
{
  if (FileCount>1)
  {
    char CurName[NM];

    if (SortMode==BY_DIZ)
      ReadDiz();

    ListSortMode=SortMode;
    ListSortOrder=SortOrder;
    ListSortGroups=SortGroups;
    ListSelectedFirst=SelectedFirst;
    ListPanelMode=PanelMode;
    ListCaseSensitive=ViewSettingsArray[ViewMode].CaseSensitiveSort;
    //ListCaseSensitive=ViewSettings.CaseSensitiveSort;
    ListNumericSort=NumericSort;


    if (KeepPosition)
      strcpy(CurName,ListData[CurFile].Name);

    hSortPlugin=(PanelMode==PLUGIN_PANEL) ? hPlugin:NULL;

    far_qsort((void *)ListData,FileCount,sizeof(*ListData),SortList);
    if (KeepPosition)
      GoToFile(CurName);
  }
}

#if defined(__BORLANDC__)
#pragma intrinsic strcmp
#endif

int _cdecl SortList(const void *el1,const void *el2)
{
  int RetCode;
  __int64 RetCode64;
  char *ChPtr1,*ChPtr2;
  struct FileListItem *SPtr1,*SPtr2;
  SPtr1=(struct FileListItem *)el1;
  SPtr2=(struct FileListItem *)el2;

  char *Name1=PointToName(SPtr1->Name);
  char *Name2=PointToName(SPtr2->Name);

  if (Name1[0]=='.' && Name1[1]=='.' && Name1[2]==0)
    return(-1);
  if (Name2[0]=='.' && Name2[1]=='.' && Name2[2]==0)
    return(1);

  if (ListSortMode==UNSORTED)
  {
    if (ListSelectedFirst && SPtr1->Selected!=SPtr2->Selected)
      return(SPtr1->Selected>SPtr2->Selected ? -1:1);
    return((SPtr1->Position>SPtr2->Position) ? ListSortOrder:-ListSortOrder);
  }

  if ((SPtr1->FileAttr & FA_DIREC) < (SPtr2->FileAttr & FA_DIREC))
    return(1);
  if ((SPtr1->FileAttr & FA_DIREC) > (SPtr2->FileAttr & FA_DIREC))
    return(-1);
  if (ListSelectedFirst && SPtr1->Selected!=SPtr2->Selected)
    return(SPtr1->Selected>SPtr2->Selected ? -1:1);
  if (ListSortGroups && (ListSortMode==BY_NAME || ListSortMode==BY_EXT) &&
      SPtr1->SortGroup!=SPtr2->SortGroup)
    return(SPtr1->SortGroup<SPtr2->SortGroup ? -1:1);

  if (hSortPlugin!=NULL)
  {
    DWORD SaveFlags1,SaveFlags2;
    SaveFlags1=SPtr1->UserFlags;
    SaveFlags2=SPtr2->UserFlags;
    SPtr1->UserFlags=SPtr2->UserFlags=0;
    PluginPanelItem pi1,pi2;
    FileList::FileListToPluginItem(SPtr1,&pi1);
    FileList::FileListToPluginItem(SPtr2,&pi2);
    SPtr1->UserFlags=SaveFlags1;
    SPtr2->UserFlags=SaveFlags2;
    int RetCode=CtrlObject->Plugins.Compare(hSortPlugin,&pi1,&pi2,ListSortMode+(SM_UNSORTED-UNSORTED));
    if (RetCode==-3)
      hSortPlugin=NULL;
    else
      if (RetCode!=-2)
        return(ListSortOrder*RetCode);
  }

  // �� ��������� �������� � ������ "�� ����������" (�����������!)
  if(!(ListSortMode == BY_EXT && !Opt.SortFolderExt && (SPtr1->FileAttr & FA_DIREC)))
  {
    switch(ListSortMode)
    {
      case BY_NAME:
        break;

      case BY_EXT:
        ChPtr1=strrchr(*Name1 ? Name1+1:Name1,'.');
        ChPtr2=strrchr(*Name2 ? Name2+1:Name2,'.');
        if (ChPtr1==NULL && ChPtr2==NULL)
          break;
        if (ChPtr1==NULL)
          return(-ListSortOrder);
        if (ChPtr2==NULL)
          return(ListSortOrder);
        if (*(ChPtr1+1)=='.')
          return(-ListSortOrder);
        if (*(ChPtr2+1)=='.')
          return(ListSortOrder);
        RetCode=ListSortOrder*LocalStricmp(ChPtr1+1,ChPtr2+1);
        if(RetCode)
          return RetCode;
        break;

      case BY_MTIME:
        if((RetCode64=*(__int64*)&SPtr1->WriteTime - *(__int64*)&SPtr2->WriteTime) == 0)
          break;
        return -ListSortOrder*(RetCode64<0?-1:1);

      case BY_CTIME:
        if((RetCode64=*(__int64*)&SPtr1->CreationTime - *(__int64*)&SPtr2->CreationTime) == 0)
          break;
        return -ListSortOrder*(RetCode64<0?-1:1);

      case BY_ATIME:
        if((RetCode64=*(__int64*)&SPtr1->AccessTime - *(__int64*)&SPtr2->AccessTime) == 0)
          break;
        return -ListSortOrder*(RetCode64<0?-1:1);

      case BY_SIZE:
        if (SPtr1->UnpSizeHigh==SPtr2->UnpSizeHigh)
        {
          if (SPtr1->UnpSize==SPtr2->UnpSize)
            break;
          return((SPtr1->UnpSize > SPtr2->UnpSize) ? -ListSortOrder : ListSortOrder);
        }
        return((SPtr1->UnpSizeHigh > SPtr2->UnpSizeHigh) ? -ListSortOrder : ListSortOrder);

      case BY_DIZ:
        if (SPtr1->DizText==NULL)
          if (SPtr2->DizText==NULL)
            break;
          else
            return(ListSortOrder);
        if (SPtr2->DizText==NULL)
          return(-ListSortOrder);
        RetCode=ListSortOrder*LCStricmp(SPtr1->DizText,SPtr2->DizText);
        if(RetCode)
          return RetCode;
        break;

      case BY_OWNER:
        RetCode=ListSortOrder*LocalStricmp(SPtr1->Owner,SPtr2->Owner);
        if(RetCode)
          return RetCode;
        break;

      case BY_COMPRESSEDSIZE:
        if (SPtr1->PackSizeHigh==SPtr2->PackSizeHigh)
        {
          if (SPtr1->PackSize==SPtr2->PackSize)
            break;
          return((SPtr1->PackSize > SPtr2->PackSize) ? -ListSortOrder : ListSortOrder);
        }
        return((SPtr1->PackSizeHigh > SPtr2->PackSizeHigh) ? -ListSortOrder : ListSortOrder);

      case BY_NUMLINKS:
        if (SPtr1->NumberOfLinks==SPtr2->NumberOfLinks)
          break;
        return((SPtr1->NumberOfLinks > SPtr2->NumberOfLinks) ? -ListSortOrder : ListSortOrder);
    }
  }

  int NameCmp;
  //int NameCmp=ListSortOrder*(ListCaseSensitive?strcmp(Name1,Name2):LCStricmp(Name1,Name2));
  if(!ListNumericSort)
    NameCmp=ListCaseSensitive?strcmp(Name1,Name2):LCStricmp(Name1,Name2);
  else
    NameCmp=ListCaseSensitive?NumStrcmp(Name1,Name2):LCNumStricmp(Name1,Name2);
  NameCmp*=ListSortOrder;
  if (NameCmp==0)
    NameCmp=SPtr1->Position>SPtr2->Position ? ListSortOrder:-ListSortOrder;

  return(NameCmp);
}


int _cdecl SortSearchList(const void *el1,const void *el2)
{
  struct FileListItem *SPtr1,*SPtr2;
  SPtr1=(struct FileListItem *)el1;
  SPtr2=(struct FileListItem *)el2;
  return strcmp(SPtr1->Name,SPtr2->Name);
//  return NumStrcmp(SPtr1->Name,SPtr2->Name);
}
#if defined(__BORLANDC__)
#pragma intrinsic -strcmp
#endif

void FileList::SetFocus()
{
  Panel::SetFocus();
  /* $ 07.04.2002 KM
    ! ������ ��������� ������� ���� ������ �����, �����
      �� ��� ������� ����������� ���� �������. � ������
      ������ ��� �������� ����� ������ � ������� ��������
      ��������� ���������.
  */
  if (!IsRedrawFramesInProcess)
    SetTitle();
  /* KM $ */
}

int FileList::SendKeyToPlugin(DWORD Key,BOOL Pred)
{
  _ALGO(CleverSysLog clv("FileList::SendKeyToPlugin()"));
  _ALGO(SysLog("Key=%u (0x%08X) Pred=%d",Key,Key,Pred));
  if (PanelMode==PLUGIN_PANEL &&
      (CtrlObject->Macro.IsRecording() == MACROMODE_RECORDING_COMMON || CtrlObject->Macro.IsExecuting() == MACROMODE_EXECUTING_COMMON || CtrlObject->Macro.GetCurRecord(NULL,NULL) == MACROMODE_NOMACRO)
     )
  {
    int VirtKey,ControlState;
    if (TranslateKeyToVK(Key,VirtKey,ControlState))
    {
      _ALGO(SysLog("call Plugins.ProcessKey() {"));
      int ProcessCode=CtrlObject->Plugins.ProcessKey(hPlugin,VirtKey|(Pred?PKF_PREPROCESS:0),ControlState);
      _ALGO(SysLog("} ProcessCode=%d",ProcessCode));
      ProcessPluginCommand();
      if (ProcessCode)
        return(TRUE);
    }
  }
  return FALSE;
}

int FileList::ProcessKey(int Key)
{
  struct FileListItem *CurPtr=NULL;
  int N, NeedRealName=FALSE;
  int CmdLength=CtrlObject->CmdLine->GetLength();

  switch(Key)
  {
    case MCODE_C_ROOTFOLDER:
    {
      if (PanelMode==PLUGIN_PANEL)
      {
        struct OpenPluginInfo Info;
        CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
        return *NullToEmpty(Info.CurDir)==0;
      }
      else
      {
        if(!IsLocalRootPath(CurDir))
        {
          char DriveRoot[NM];
          GetPathRoot(CurDir,DriveRoot);
          return !stricmp(CurDir,DriveRoot);
        }
        return TRUE;
      }
    }
    case MCODE_C_EOF:
      return CurFile == FileCount-1;
    case MCODE_C_BOF:
      return CurFile==0;
    case MCODE_C_SELECTED:
      return GetRealSelCount()>1;
    case MCODE_V_ITEMCOUNT:
      return FileCount;
    case MCODE_V_CURPOS:
      return CurFile+1;
  }

  if (IsVisible())
  {
    if(!InternalProcessKey)
      if ((Key!=KEY_ENTER && Key!=KEY_SHIFTENTER) || CmdLength==0)
        if(SendKeyToPlugin(Key))
          return TRUE;
  }
  else
  {
    // �� �������, ������� �������� ��� ���������� �������:
    switch(Key)
    {
      case KEY_CTRLF:
      case KEY_CTRLALTF:
      case KEY_CTRLENTER:
      case KEY_CTRLBRACKET:
      case KEY_CTRLBACKBRACKET:
      case KEY_CTRLSHIFTBRACKET:
      case KEY_CTRLSHIFTBACKBRACKET:
      case KEY_CTRL|KEY_COLON:
      case KEY_CTRL|KEY_ALT|KEY_COLON:
      case KEY_CTRLALTBRACKET:
      case KEY_CTRLALTBACKBRACKET:
      case KEY_ALTSHIFTBRACKET:
      case KEY_ALTSHIFTBACKBRACKET:
        break;

      case KEY_CTRLG:
      case KEY_SHIFTF4:
      case KEY_F7:
      case KEY_CTRLH:
      case KEY_ALTSHIFTF9:
      case KEY_CTRLN:
        break;

      // ��� �������, ����, ���� Ctrl-F ��������, �� � ��� ������ :-)
/*
      case KEY_CTRLINS:
      case KEY_CTRLSHIFTINS:
      case KEY_CTRLALTINS:
      case KEY_ALTSHIFTINS:
        break;
*/
      default:
        return(FALSE);
    }
  }

  if (!ShiftPressed && ShiftSelection!=-1)
  {
    if (SelectedFirst)
    {
      SortFileList(TRUE);
      ShowFileList(TRUE);
    }
    ShiftSelection=-1;
  }

  if (!InternalProcessKey && (Key>=KEY_RCTRL0 && Key<=KEY_RCTRL9 ||
      Key>=KEY_CTRLSHIFT0 && Key<=KEY_CTRLSHIFT9))
  {
    char *ShortcutFolder=NULL;
    char PluginModule[NM],PluginFile[NM],PluginData[MAXSIZE_SHORTCUTDATA];
    if (PanelMode==PLUGIN_PANEL)
    {
      int PluginNumber=((struct PluginHandle *)hPlugin)->PluginNumber;
      strcpy(PluginModule,CtrlObject->Plugins.PluginsData[PluginNumber].ModuleName);
      struct OpenPluginInfo Info;
      CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
      strcpy(PluginFile,NullToEmpty(Info.HostFile));
      ShortcutFolder=new char[strlen(NullToEmpty(Info.CurDir))+NM];
      strcpy(ShortcutFolder,NullToEmpty(Info.CurDir));
      xstrncpy(PluginData,NullToEmpty(Info.ShortcutData),sizeof(PluginData)-1);
      PluginData[sizeof(PluginData)-1]=0;
    }
    else
    {
      *PluginModule=*PluginFile=*PluginData=0;
      ShortcutFolder=new char[strlen(CurDir)+NM];
      strcpy(ShortcutFolder,CurDir);
    }
    if (SaveFolderShortcut(Key,ShortcutFolder,PluginModule,PluginFile,PluginData))
    {
      if(ShortcutFolder) delete[] ShortcutFolder;
      return(TRUE);
    }

    if(ShortcutFolder)
      delete[] ShortcutFolder;

    int SizeFolderNameShortcut=GetShortcutFolderSize(Key);
    ShortcutFolder=new char[SizeFolderNameShortcut+NM];
    if(ShortcutFolder)
    {
      if (GetShortcutFolder(Key,ShortcutFolder,SizeFolderNameShortcut+NM,PluginModule,PluginFile,PluginData))
      {
        if(*PluginModule)
        {
          if(*PluginFile)
          {
            switch(CheckShortcutFolder(PluginFile,0,TRUE))
            {
              case 0:
  //              return FALSE;
              case -1:
                delete[] ShortcutFolder;
                return TRUE;
            }
            /* ������������ ������� BugZ#50 */
            char RealDir[2048], *Ptr;
            Ptr=strrchr(strcpy(RealDir,PluginFile),'\\');
            if(Ptr)
            {
              *++Ptr=0;
              SetCurDir(RealDir,TRUE);
              GoToFile(PointToName(PluginFile));
              // ������ ����.��������.
              if(PrevDataStackSize>0)
              {
                for(--PrevDataStackSize;PrevDataStackSize > 0;PrevDataStackSize--)
                  DeleteListData(PrevDataStack[PrevDataStackSize].PrevListData,PrevDataStack[PrevDataStackSize].PrevFileCount);
              }
            }
            /**/
            OpenFilePlugin(PluginFile,FALSE);
            if (*ShortcutFolder)
              SetCurDir(ShortcutFolder,FALSE);
            Show();
          }
          else
          {
            switch(CheckShortcutFolder(NULL,0,TRUE))
            {
              case 0:
  //              return FALSE;
              case -1:
                delete[] ShortcutFolder;
                return TRUE;
            }
            for (int I=0;I<CtrlObject->Plugins.PluginsCount;I++)
            {
              if (LocalStricmp(CtrlObject->Plugins.PluginsData[I].ModuleName,PluginModule)==0)
              {
                if (CtrlObject->Plugins.PluginsData[I].pOpenPlugin)
                {
                  HANDLE hNewPlugin=CtrlObject->Plugins.OpenPlugin(I,OPEN_SHORTCUT,(INT_PTR)PluginData);
                  if (hNewPlugin!=INVALID_HANDLE_VALUE)
                  {
                    int CurFocus=GetFocus();
                    Panel *NewPanel=CtrlObject->Cp()->ChangePanel(this,FILE_PANEL,TRUE,TRUE);
                    NewPanel->SetPluginMode(hNewPlugin,"");
                    if (*ShortcutFolder)
                      CtrlObject->Plugins.SetDirectory(hNewPlugin,ShortcutFolder,0);
                    NewPanel->Update(0);
                    if (CurFocus || !CtrlObject->Cp()->GetAnotherPanel(NewPanel)->IsVisible())
                      NewPanel->SetFocus();
                    NewPanel->Show();
                  }
                }
                break;
              }
            }
            /*
            if(I == CtrlObject->Plugins.PluginsCount)
            {
              char Target[NM*2];
              xstrncpy(Target, PluginModule, sizeof(Target)-1);
              TruncPathStr(Target, ScrX-16);
              Message (MSG_WARNING | MSG_ERRORTYPE, 1, MSG(MError), Target, MSG (MNeedNearPath), MSG(MOk))
            }
            */
          }
          delete[] ShortcutFolder;
          return(TRUE);
        }
        switch(CheckShortcutFolder(ShortcutFolder,SizeFolderNameShortcut,FALSE))
        {
          case 0:
  //          return FALSE;
          case -1:
            delete[] ShortcutFolder;
            return TRUE;
        }
        SetCurDir(ShortcutFolder,TRUE);
        Show();
        delete[] ShortcutFolder;
        return(TRUE);
      }
      delete[] ShortcutFolder;
    }
  }

  /* $ 27.08.2002 SVS
      [*] � ������ � ����� �������� Shift-Left/Right ���������� �������
          Shift-PgUp/PgDn.
  */
  if(Columns==1 && CmdLength==0)
  {
    if(Key == KEY_SHIFTLEFT || Key == KEY_SHIFTNUMPAD4)
      Key=KEY_SHIFTPGUP;
    else if(Key == KEY_SHIFTRIGHT || Key == KEY_SHIFTNUMPAD6)
      Key=KEY_SHIFTPGDN;
  }
  /* SVS$ */

  switch(Key)
  {
    case KEY_F1:
    {
      _ALGO(CleverSysLog clv("F1"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      if (PanelMode==PLUGIN_PANEL && PluginPanelHelp(hPlugin))
        return(TRUE);
      return(FALSE);
    }

    case KEY_ALTSHIFTF9:
    {
      _ALGO(CleverSysLog clv("Alt-Shift-F9"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      if (PanelMode==PLUGIN_PANEL)
        CtrlObject->Plugins.ConfigureCurrent(((struct PluginHandle *)hPlugin)->PluginNumber,0);
      else
        CtrlObject->Plugins.Configure();
      return TRUE;
    }

    case KEY_SHIFTSUBTRACT:
    {
      SaveSelection();
      ClearSelection();
      Redraw();
      return(TRUE);
    }

    case KEY_SHIFTADD:
    {
      SaveSelection();
      {
        struct FileListItem *CurPtr=ListData;
        for (int I=0; I < FileCount; I++, CurPtr++)
          if ((CurPtr->FileAttr & FA_DIREC)==0 || Opt.SelectFolders)
            Select(CurPtr,1);
      }
      if (SelectedFirst)
        SortFileList(TRUE);
      Redraw();
      return(TRUE);
    }

    case KEY_ADD:
      SelectFiles(SELECT_ADD);
      return(TRUE);

    case KEY_SUBTRACT:
      SelectFiles(SELECT_REMOVE);
      return(TRUE);

    case KEY_CTRLADD:
      SelectFiles(SELECT_ADDEXT);
      return(TRUE);

    case KEY_CTRLSUBTRACT:
      SelectFiles(SELECT_REMOVEEXT);
      return(TRUE);

    case KEY_ALTADD:
      SelectFiles(SELECT_ADDNAME);
      return(TRUE);

    case KEY_ALTSUBTRACT:
      SelectFiles(SELECT_REMOVENAME);
      return(TRUE);

    case KEY_MULTIPLY:
      SelectFiles(SELECT_INVERT);
      return(TRUE);

    case KEY_CTRLMULTIPLY:
      SelectFiles(SELECT_INVERTALL);
      return(TRUE);

    case KEY_ALTLEFT:     // ��������� ������� ���� � ��������
    case KEY_ALTHOME:     // ��������� ������� ���� � �������� - � ������
      LeftPos=(Key == KEY_ALTHOME)?0:LeftPos-1;
      Redraw();
      return(TRUE);

    case KEY_ALTRIGHT:    // ��������� ������� ���� � ��������
    case KEY_ALTEND:     // ��������� ������� ���� � �������� - � �����
      LeftPos=(Key == KEY_ALTEND)?0x7fff:LeftPos+1;
      Redraw();
      return(TRUE);

    case KEY_CTRLINS:      case KEY_CTRLNUMPAD0:
      if (CmdLength>0)
        return(FALSE);
    case KEY_CTRLSHIFTINS: case KEY_CTRLSHIFTNUMPAD0:  // ���������� �����
    case KEY_CTRLALTINS:   case KEY_CTRLALTNUMPAD0:    // ���������� UNC-�����
    case KEY_ALTSHIFTINS:                              // ���������� ������ �����

      //if (FileCount>0 && SetCurPath()) // ?????
      SetCurPath ();

      CopyNames(Key == KEY_CTRLALTINS || Key == KEY_ALTSHIFTINS || Key == KEY_CTRLALTNUMPAD0,
                (Key&(KEY_CTRL|KEY_ALT))==(KEY_CTRL|KEY_ALT));
      return(TRUE);

    /* $ 14.02.2001 VVM
      + Ctrl: ��������� ��� ����� � ��������� ������.
      + CtrlAlt: ��������� UNC-��� ����� � ��������� ������ */
    case KEY_CTRL|KEY_COLON:
    case KEY_CTRL|KEY_ALT|KEY_COLON:
    {
      int NewKey = KEY_CTRLF;
      if (Key & KEY_ALT)
        NewKey|=KEY_ALT;

      Panel *SrcPanel = CtrlObject->Cp()->GetAnotherPanel(CtrlObject->Cp()->ActivePanel);
      int OldState = SrcPanel->IsVisible();
      SrcPanel->SetVisible(1);
      SrcPanel->ProcessKey(NewKey);
      SrcPanel->SetVisible(OldState);

      SetCurPath();
      return(TRUE);
    }
    /* VVM $ */

    case KEY_CTRLENTER:
    case KEY_CTRLSHIFTENTER:
    case KEY_CTRLJ:
    case KEY_CTRLF:
    case KEY_CTRLALTF:  // 29.01.2001 VVM + �� CTRL+ALT+F � ��������� ������ ������������ UNC-��� �������� �����.
    {
      if (FileCount>0 && SetCurPath())
      {
        char FileName[2048];
        if(Key==KEY_CTRLSHIFTENTER)
          _MakePath1(Key,FileName,sizeof(FileName)-1, " ");
        else
        {
          int CurrentPath=FALSE;
          CurPtr=ListData+CurFile;
          strcpy(FileName,ShowShortNames && *CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name);
          if (TestParentFolderName(FileName))
          {
            if (PanelMode==PLUGIN_PANEL)
              *FileName=0;
            else
              FileName[1]=0; // "."

            if(Key!=KEY_CTRLALTF)
              Key=KEY_CTRLF;
            CurrentPath=TRUE;
          }
          if (Key==KEY_CTRLF || Key==KEY_CTRLALTF)
          {
            bool realName=PanelMode!=PLUGIN_PANEL;
            struct OpenPluginInfo Info={0};
            if (PanelMode==PLUGIN_PANEL)
            {
              CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
            }
            if (PanelMode!=PLUGIN_PANEL || (Info.Flags & OPIF_REALNAMES))
            {
              if(!CreateFullPathName(CurPtr->Name,CurPtr->ShortName,CurPtr->FileAttr,
                                 FileName,sizeof(FileName)-1,Key==KEY_CTRLALTF))
                return FALSE;
            }
            else
            {
              char FullName[NM];
              strcpy(FullName,NullToEmpty(Info.CurDir));
              /* $ 13.10.2000 tran
                �� Ctrl-f ��� ������ �������� �������� �� ������ */
              /* $ 20.10.2000 SVS
                 ������� ���� Ctrl-F ������������!*/
              if (Opt.PanelCtrlFRule && ViewSettings.FolderUpperCase)
                LocalStrupr(FullName);
              /* SVS $ */
              /* tran $ */
              if (*FullName)
                AddEndSlash(FullName,0); //'\\');
              /* $ 20.10.2000 SVS
                 ������� ���� Ctrl-F ������������!*/
              if(Opt.PanelCtrlFRule)
              {
                /* $ 13.10.2000 tran
                  �� Ctrl-f ��� ������ �������� �������� �� ������ */
                if ( ViewSettings.FileLowerCase && !(CurPtr->FileAttr & FA_DIREC))
                  LocalStrlwr(FileName);
                if (ViewSettings.FileUpperToLowerCase)
                  if (!(CurPtr->FileAttr & FA_DIREC) && !IsCaseMixed(FileName))
                     LocalStrlwr(FileName);
                /* tran $ */
              }
              /* SVS $*/
              strcat(FullName,FileName);
              strcpy(FileName,FullName);
            }
          }
          if (CurrentPath)
            AddEndSlash(FileName);

          // ������� ������ �������!
          if(PanelMode==PLUGIN_PANEL && Opt.SubstPluginPrefix && !(Key == KEY_CTRLENTER || Key == KEY_CTRLJ))
          {
            char Prefix[NM*2];
            /* $ 19.11.2001 IS ����������� �� �������� :) */
            if(*AddPluginPrefix((FileList *)CtrlObject->Cp()->ActivePanel,Prefix))
            {
              strcat(Prefix,FileName);
              xstrncpy(FileName,Prefix,sizeof(FileName)-1);
            }
            /* IS $ */
          }
          if(Opt.QuotedName&QUOTEDNAME_INSERT)
            QuoteSpace(FileName);
          strcat(FileName," ");
        }
        CtrlObject->CmdLine->InsertString(FileName);
      }
      return(TRUE);
    }

    case KEY_CTRLALTBRACKET:       // �������� ������� (UNC) ���� �� ����� ������
    case KEY_CTRLALTBACKBRACKET:   // �������� ������� (UNC) ���� �� ������ ������
    case KEY_ALTSHIFTBRACKET:      // �������� ������� (UNC) ���� �� �������� ������
    case KEY_ALTSHIFTBACKBRACKET:  // �������� ������� (UNC) ���� �� ��������� ������
    case KEY_CTRLBRACKET:          // �������� ���� �� ����� ������
    case KEY_CTRLBACKBRACKET:      // �������� ���� �� ������ ������
    case KEY_CTRLSHIFTBRACKET:     // �������� ���� �� �������� ������
    case KEY_CTRLSHIFTBACKBRACKET: // �������� ���� �� ��������� ������
    {
      char PanelDir[2048];
      if(_MakePath1(Key,PanelDir,sizeof(PanelDir)-1, ""))
        CtrlObject->CmdLine->InsertString(PanelDir);
      return(TRUE);
    }

    case KEY_CTRLA:
    {
      _ALGO(CleverSysLog clv("Ctrl-A"));
      if (FileCount>0 && SetCurPath())
      {
        ShellSetFileAttributes(this);
        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLG:
    {
      _ALGO(CleverSysLog clv("Ctrl-G"));
      if (PanelMode!=PLUGIN_PANEL ||
          CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FAROTHER))
        if (FileCount>0 && SetCurPath())
        {
          ApplyCommand();
          Update(UPDATE_KEEP_SELECTION);
          Redraw();
          Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
          AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
          AnotherPanel->Redraw();
        }
      return(TRUE);
    }

    case KEY_CTRLZ:
      if (FileCount>0 && PanelMode==NORMAL_PANEL && SetCurPath())
        DescribeFiles();
      return(TRUE);

    case KEY_CTRLH:
    {
      Opt.ShowHidden=!Opt.ShowHidden;
      Update(UPDATE_KEEP_SELECTION);
      Redraw();
      Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
      AnotherPanel->Update(UPDATE_KEEP_SELECTION);//|UPDATE_SECONDARY);
      AnotherPanel->Redraw();
      return(TRUE);
    }

    case KEY_CTRLM:
    {
      RestoreSelection();
      return(TRUE);
    }

    case KEY_CTRLR:
    {
      Update(UPDATE_KEEP_SELECTION);
      Redraw();
      {
        Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
        if (AnotherPanel->GetType()!=FILE_PANEL)
        {
          AnotherPanel->SetCurDir(CurDir,FALSE);
          AnotherPanel->Redraw();
        }
      }
      break;
    }

    case KEY_CTRLN:
    {
      ShowShortNames=!ShowShortNames;
      Redraw();
      return(TRUE);
    }

    case KEY_ENTER:
    case KEY_SHIFTENTER:
    {
      _ALGO(CleverSysLog clv("Enter/Shift-Enter"));
      _ALGO(SysLog("%s, FileCount=%d Key=%s",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount,_FARKEY_ToName(Key)));
      if (CmdLength>0 || FileCount==0)
      {
        // ��� ������� ������� ���� � �� ������ ������... �������� ��� ������
        if(MButtonPressed && CmdLength>0)
        {
          CtrlObject->CmdLine->ProcessKey(Key);
          return(TRUE);
        }
        break;
      }
      ProcessEnter(1,Key==KEY_SHIFTENTER);
      return(TRUE);
    }

    case KEY_CTRLBACKSLASH:
    {
      _ALGO(CleverSysLog clv("Ctrl-\\"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      BOOL NeedChangeDir=TRUE;
      if (PanelMode==PLUGIN_PANEL)// && *PluginsStack[PluginsStackSize-1].HostFile)
      {
        int CheckFullScreen=IsFullScreen();
        struct OpenPluginInfo Info;
        CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
        if (!Info.CurDir || *Info.CurDir == 0)
        {
          ChangeDir("..");
          NeedChangeDir=FALSE;
          if(CheckFullScreen)
            CtrlObject->Cp()->GetAnotherPanel(this)->Show();
        }
      }
      if(NeedChangeDir)
        ChangeDir("\\");
      Show();
      return(TRUE);
    }

    case KEY_SHIFTF1:
    {
      _ALGO(CleverSysLog clv("Shift-F1"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      if (FileCount>0 && PanelMode!=PLUGIN_PANEL && SetCurPath())
        PluginPutFilesToNew();
      return(TRUE);
    }

    case KEY_SHIFTF2:
    {
      _ALGO(CleverSysLog clv("Shift-F2"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      if (FileCount>0 && SetCurPath())
      {
        if (PanelMode==PLUGIN_PANEL)
        {
          struct OpenPluginInfo Info;
          CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
          if (Info.HostFile!=NULL && *Info.HostFile!=0)
            ProcessKey(KEY_F5);
          else
            if ( (Info.Flags & OPIF_REALNAMES) == OPIF_REALNAMES )
              PluginHostGetFiles();

          return(TRUE);
        }
        PluginHostGetFiles();
      }
      return(TRUE);
    }

    case KEY_SHIFTF3:
    {
      _ALGO(CleverSysLog clv("Shift-F3"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      ProcessHostFile();
      return(TRUE);
    }

    case KEY_F3:
    case KEY_NUMPAD5:      case KEY_SHIFTNUMPAD5:
    case KEY_ALTF3:
    case KEY_CTRLSHIFTF3:
    case KEY_F4:
    case KEY_ALTF4:
    case KEY_SHIFTF4:
    case KEY_CTRLSHIFTF4:
    {
      _ALGO(CleverSysLog clv("Edit/View"));
      _ALGO(SysLog("%s, FileCount=%d Key=%s",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount,_FARKEY_ToName(Key)));

      struct OpenPluginInfo Info;
      BOOL RefreshedPanel=TRUE;

      if(PanelMode==PLUGIN_PANEL)
        CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
      else
        memset(&Info,0,sizeof(struct OpenPluginInfo));


      if (Key == KEY_NUMPAD5 || Key == KEY_SHIFTNUMPAD5)
        Key=KEY_F3;
      if ((Key==KEY_SHIFTF4 || FileCount>0) && SetCurPath())
      {
        int Edit=(Key==KEY_F4 || Key==KEY_ALTF4 || Key==KEY_SHIFTF4 || Key==KEY_CTRLSHIFTF4);
        BOOL Modaling=FALSE; ///
        int UploadFile=TRUE;
        char FileName[NM*2],ShortFileName[NM],PluginData[NM*2];

        int PluginMode=PanelMode==PLUGIN_PANEL &&
            !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILE);

        /* $ 24.11.2001 IS
           ���� ������ ������� �������� �������� �����, �� �������, ��� ���
           ������� �������� ������
        */
        if (PluginMode)
        {
          if(Info.Flags & OPIF_REALNAMES)
            PluginMode=FALSE;
          else
            sprintf(PluginData,"<%s:%s>",NullToEmpty(Info.HostFile),NullToEmpty(Info.CurDir));
        }

        if(!PluginMode)
          *PluginData=0;
        /* IS $ */

        if (Key==KEY_SHIFTF4)
        {
            static char LastFileName[NM*2]="";

          do{

            *FileName=0;
            if (!GetString(MSG(MEditTitle),
                           MSG(MFileToEdit),
                           "NewEdit",
                           LastFileName,
                           LastFileName,
                           sizeof(LastFileName),
                           "Editor",
                           FIB_BUTTONS|FIB_EXPANDENV/*|FIB_EDITPATH*/|FIB_ENABLEEMPTY))
              return(FALSE);

            if(*LastFileName)
            {
              xstrncpy(FileName,LastFileName,sizeof(FileName)-1);

              RemoveTrailingSpaces(FileName);
              Unquote(FileName);
              ConvertNameToShort(FileName,ShortFileName,sizeof(ShortFileName)-1);

              if (PathMayBeAbsolute(FileName))
              {
                PluginMode=FALSE;
              }

              {
                // �������� ���� � �����
                char *Ptr=strrchr(FileName,'\\');
                if(Ptr && Ptr != FileName)
                {
                  *Ptr=0;
                  DWORD CheckFAttr=GetFileAttributes(FileName);
                  if(CheckFAttr == (DWORD)-1)
                  {
                    SetMessageHelp("WarnEditorPath");
                    if (Message(MSG_WARNING,2,MSG(MWarning),
                                MSG(MEditNewPath1),
                                MSG(MEditNewPath2),
                                MSG(MEditNewPath3),
                                MSG(MHYes),MSG(MHNo))!=0)

                      return(FALSE);
                  }
                  *Ptr='\\';
                }
              }
            }
            else if(PluginMode) // ������ ��� ����� � ������ ������� �� �����������!
            {
              SetMessageHelp("WarnEditorPluginName");
              if (Message(MSG_WARNING,2,MSG(MWarning),
                          MSG(MEditNewPlugin1),
                          MSG(MEditNewPath3),MSG(MCancel))!=0)

                return(FALSE);
            }
            else
              strcpy(FileName,MSG(MNewFileName));

          } while(!*FileName);

        }
        else
        {
          CurPtr=ListData+CurFile;

          if (CurPtr->FileAttr & FA_DIREC)
          {
            if (Edit)
              return ProcessKey(KEY_CTRLA);

            CountDirSize(Info.Flags);
            return(TRUE);
          }

          strcpy(FileName,CurPtr->Name);
          strcpy(ShortFileName,*CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name);
        }

        char TempDir[NM],TempName[NM*3];

        int UploadFailed=FALSE, NewFile=FALSE;

        if (PluginMode)
        {
          if(!FarMkTempEx(TempDir))
            return(TRUE);
          CreateDirectory(TempDir,NULL);
          sprintf(TempName,"%s\\%s",TempDir,PointToName(FileName));
          if (Key==KEY_SHIFTF4)
          {
            int Pos=FindFile(FileName);
            if (Pos!=-1)
              CurPtr=ListData+Pos;
            else
            {
              NewFile=TRUE;
              xstrncpy(FileName,TempName,sizeof(FileName)-1);
            }
          }
          if (!NewFile)
          {
            struct PluginPanelItem PanelItem;
            FileListToPluginItem(CurPtr,&PanelItem);
            if (!CtrlObject->Plugins.GetFile(hPlugin,&PanelItem,TempDir,FileName,OPM_SILENT|(Edit ? OPM_EDIT:OPM_VIEW)))
            {
              FAR_RemoveDirectory(TempDir);
              return(TRUE);
            }
          }
          ConvertNameToShort(FileName,ShortFileName,sizeof(ShortFileName)-1);
        }

        /* $ 08.04.2002 IS
           ����, ��������� � ���, ��� ����� ������� ����, ������� ��������� ��
           ������. ���� ���� ������� �� ���������� ������, �� DeleteViewedFile
           ������ ��� ����� false, �.�. ���������� ����� ��� ��� ������.
        */
        bool DeleteViewedFile=PluginMode && !Edit;
        /* IS $ */
        if (*FileName)
          if (Edit)
          {
            int EnableExternal=((Key==KEY_F4 || Key==KEY_SHIFTF4) && Opt.EdOpt.UseExternalEditor ||
                Key==KEY_ALTF4 && !Opt.EdOpt.UseExternalEditor) && *Opt.ExternalEditor;
            /* $ 02.08.2001 IS ���������� ���������� ��� alt-f4 */
            BOOL Processed=FALSE;
            if(Key==KEY_ALTF4 &&
               ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_ALTEDIT,
               PluginMode))
               Processed=TRUE;
            else if(Key==KEY_F4 &&
               ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_EDIT,
               PluginMode))
               Processed=TRUE;

            if (!Processed || Key==KEY_CTRLSHIFTF4)
            {
            /* IS $ */
              if (EnableExternal)
                /* $ 24.11.200 IS
                   �������� ���������� �������, ���� �� �� ������ �������
                */
                ProcessExternal(Opt.ExternalEditor,FileName,ShortFileName,PluginMode);
                /* IS $ */
              else if (PluginMode)
              {
                RefreshedPanel=FrameManager->GetCurrentFrame()->GetType()==MODALTYPE_EDITOR?FALSE:TRUE;
                FileEditor ShellEditor (FileName,(Key==KEY_SHIFTF4?FFILEEDIT_CANNEWFILE:0)|FFILEEDIT_DISABLEHISTORY,-1,-1,PluginData);
                ShellEditor.SetDynamicallyBorn(false);
                FrameManager->EnterModalEV();
                FrameManager->ExecuteModal();//OT
                FrameManager->ExitModalEV();
                /* $ 24.11.2001 IS
                     ���� �� ������� ����� ����, �� �� �����, ��������� ��
                     ��� ���, ��� ����� ������� ��� �� ������ �������.
                */
                UploadFile=ShellEditor.IsFileChanged() || NewFile;
                /* IS $ */
                Modaling=TRUE;///
              }
              else
              {
                NamesList EditList;
                if (!PluginMode)
                {
                  for (int I=0;I<FileCount;I++)
                    if ((ListData[I].FileAttr & FA_DIREC)==0)
                      EditList.AddName(ListData[I].Name,ListData[I].ShortName);
                  EditList.SetCurDir(CurDir);
                  EditList.SetCurName(FileName);
                }
                FileEditor *ShellEditor=new FileEditor((strcmp(FileName,MSG(MNewFileName)) != 0 &&
                                                        GetFileAttributes(FileName) == (DWORD)-1 &&
                                                        GetFileAttributes(ShortFileName) != (DWORD)-1)?ShortFileName:FileName,
                                                        (Key==KEY_SHIFTF4?FFILEEDIT_CANNEWFILE:0)|FFILEEDIT_ENABLEF6);
                ShellEditor->SetNamesList (&EditList);
                FrameManager->ExecuteModal();//OT
              }
            }

            if (PluginMode && UploadFile)
            {
              struct PluginPanelItem PanelItem;
              char SaveDir[NM];

              FarGetCurDir(sizeof(SaveDir),SaveDir);

              if (GetFileAttributes(TempName)==0xffffffff)
              {
                char FindName[NM*2];
                strcpy(FindName,TempName);
                strcpy(PointToName(FindName),"*");
                HANDLE FindHandle;
                WIN32_FIND_DATA FindData;

                bool Done=((FindHandle=FindFirstFile(FindName,&FindData))==INVALID_HANDLE_VALUE);
                while (!Done)
                {
                  if ((FindData.dwFileAttributes & FA_DIREC)==0)
                  {
                    strcpy(PointToName(TempName),FindData.cFileName);
                    break;
                  }
                  Done=!FindNextFile(FindHandle,&FindData);
                }
                FindClose(FindHandle);
              }

              if (FileNameToPluginItem(TempName,&PanelItem))
              {
                int PutCode=CtrlObject->Plugins.PutFiles(hPlugin,&PanelItem,1,FALSE,OPM_EDIT);
                if (PutCode==1 || PutCode==2)
                  SetPluginModified();
                if (PutCode==0)
                  UploadFailed=TRUE;
              }

              FarChDir(SaveDir);
            }
          }
          else
          {
            int EnableExternal=(Key==KEY_F3 && Opt.ViOpt.UseExternalViewer ||
                                Key==KEY_ALTF3 && !Opt.ViOpt.UseExternalViewer) &&
                                *Opt.ExternalViewer;
            /* $ 02.08.2001 IS ���������� ���������� ��� alt-f3 */
            BOOL Processed=FALSE;
            if(Key==KEY_ALTF3 &&
               ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_ALTVIEW,PluginMode))
               Processed=TRUE;
            else if(Key==KEY_F3 &&
               ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_VIEW,PluginMode))
               Processed=TRUE;

            if (!Processed || Key==KEY_CTRLSHIFTF3)
            /* IS $ */
              if (EnableExternal)
                ProcessExternal(Opt.ExternalViewer,FileName,ShortFileName,PluginMode);
              else
              {
                NamesList ViewList;
                if (!PluginMode)
                {
                  for (int I=0;I<FileCount;I++)
                    if ((ListData[I].FileAttr & FA_DIREC)==0)
                      ViewList.AddName(ListData[I].Name,ListData[I].ShortName);
                  ViewList.SetCurDir(CurDir);
                  ViewList.SetCurName(FileName);
                }
                //FileViewer *ShellViewer=new FileViewer(FileName,TRUE,PluginMode,PluginMode,-1,PluginData,&ViewList);
                //FileViewer *ShellViewer=new FileViewer(GetShowShortNamesMode()?ShortFileName:FileName,TRUE,PluginMode,PluginMode,-1,PluginData,&ViewList);
                FileViewer *ShellViewer=new FileViewer((GetFileAttributes(FileName) == (DWORD)-1 &&
                                                        GetFileAttributes(ShortFileName) != (DWORD)-1)?ShortFileName:FileName,
                                                        TRUE,PluginMode,PluginMode,-1,PluginData,&ViewList);

                /* $ 08.04.2002 IS
                   ������� DeleteViewedFile, �.�. ���������� ����� ��� ���
                   ������
                */
                if (PluginMode)
                {
                  ShellViewer->SetTempViewName(FileName);
                  DeleteViewedFile=false;
                }
                /* IS $ */
                Modaling=FALSE;
              }
          }
        /* $ 08.04.2002 IS
             ��� �����, ������� ���������� �� ���������� ������, ������ ��
             �������������, �.�. ����� �� ���� ����������� ���
        */
        if (PluginMode)
        {
          if (UploadFailed)
            Message(MSG_WARNING,1,MSG(MError),MSG(MCannotSaveFile),
                    MSG(MTextSavedToTemp),FileName,MSG(MOk));
          else if(Edit || DeleteViewedFile)
            // ������� ���� ������ ��� ������ ������� ��� � ��������� ��� ��
            // ������� ������, �.�. ���������� ����� ������� ���� ���
            DeleteFileWithFolder(FileName);
        }
        /* IS $ */
        if (Modaling && (Edit || IsColumnDisplayed(ADATE_COLUMN)) && RefreshedPanel)
        {
          //if (!PluginMode || UploadFile)
          {
            Update(UPDATE_KEEP_SELECTION);
            Redraw();
            Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
            if (AnotherPanel->GetMode()==NORMAL_PANEL)
            {
              AnotherPanel->Update(UPDATE_KEEP_SELECTION);
              AnotherPanel->Redraw();
            }
          }
//          else
//            SetTitle();
        }
        else
          if (PanelMode==NORMAL_PANEL)
            AccessTimeUpdateRequired=TRUE;
      }
      /* $ 15.07.2000 tran
         � ��� �� �������� ����������� �������
         ������ ��� ���� viewer, editor ����� ��� ������� ������������
         */
//      CtrlObject->Cp()->Redraw();
      /* tran 15.07.2000 $ */
      return(TRUE);
    }

    case KEY_F5:
    case KEY_F6:
    case KEY_ALTF6:
    case KEY_DRAGCOPY:
    case KEY_DRAGMOVE:
    {
      _ALGO(CleverSysLog clv("F5/F6/Alt-F6/DragCopy/DragMove"));
      _ALGO(SysLog("%s, FileCount=%d Key=%s",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount,_FARKEY_ToName(Key)));
      if (FileCount>0 && SetCurPath())
        ProcessCopyKeys(Key);
      return(TRUE);
    }

    case KEY_ALTF5:  // ������ ��������/��������� �����/��
    {
      _ALGO(CleverSysLog clv("Alt-F5"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      // $ 11.03.2001 VVM - ������ ����� pman ������ �� �������� �������.
      if ((PanelMode!=PLUGIN_PANEL) && (Opt.UsePrintManager && CtrlObject->Plugins.FindPlugin(SYSID_PRINTMANAGER) != -1))
         CtrlObject->Plugins.CallPlugin(SYSID_PRINTMANAGER,OPEN_FILEPANEL,0); // printman
      else
        if (FileCount>0 && SetCurPath())
          PrintFiles(this);
      return(TRUE);
    }

    case KEY_SHIFTF5:
    case KEY_SHIFTF6:
    {
      _ALGO(CleverSysLog clv("Shift-F5/Shift-F6"));
      _ALGO(SysLog("%s, FileCount=%d Key=%s",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount,_FARKEY_ToName(Key)));
      if (FileCount>0 && SetCurPath())
      {
        int OldFileCount=FileCount,OldCurFile=CurFile;
        int OldSelection=ListData[CurFile].Selected;
        int ToPlugin=0;
        int RealName=PanelMode!=PLUGIN_PANEL;
        ReturnCurrentFile=TRUE;

        if (PanelMode==PLUGIN_PANEL)
        {
          struct OpenPluginInfo Info;
          CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
          RealName=Info.Flags&OPIF_REALNAMES;
        }

        if (RealName)
        {
          ShellCopy ShCopy(this,Key==KEY_SHIFTF6,FALSE,TRUE,TRUE,ToPlugin,NULL);
        }
        else
        {
          ProcessCopyKeys(Key==KEY_SHIFTF5 ? KEY_F5:KEY_F6);
        }
        ReturnCurrentFile=FALSE;
        if (Key!=KEY_SHIFTF5 && FileCount==OldFileCount &&
            CurFile==OldCurFile && OldSelection!=ListData[CurFile].Selected)
        {
          Select(&ListData[CurFile],OldSelection);
          Redraw();
        }
      }
      return(TRUE);
    }

    case KEY_F7:
    {
      _ALGO(CleverSysLog clv("F7"));
      _ALGO(SysLog("%s, FileCount=%d",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount));
      if (SetCurPath())
      {
        if (PanelMode==PLUGIN_PANEL && !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARMAKEDIRECTORY))
        {
          char DirName[NM];
          *DirName=0;
          int MakeCode=CtrlObject->Plugins.MakeDirectory(hPlugin,DirName,0);
          if (!MakeCode)
            Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),MSG(MCannotCreateFolder),DirName,MSG(MOk));
          Update(UPDATE_KEEP_SELECTION);
          if (MakeCode==1)
            GoToFile(PointToName(DirName));
          Redraw();
          Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
          /* $ 07.09.2001 VVM
            ! �������� �������� ������ � ���������� �� ����� ������� */
//          AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
//          AnotherPanel->Redraw();

          if (AnotherPanel->GetType()!=FILE_PANEL)
          {
            AnotherPanel->SetCurDir(CurDir,FALSE);
            AnotherPanel->Redraw();
          }
          /* VVM */
        }
        else
          ShellMakeDir(this);
      }
      return(TRUE);
    }

    case KEY_F8:
    case KEY_SHIFTDEL:
    case KEY_SHIFTF8:
    case KEY_ALTDEL:
    {
      _ALGO(CleverSysLog clv("F8/Shift-F8/Shift-Del/Alt-Del"));
      _ALGO(SysLog("%s, FileCount=%d, Key=%s",(PanelMode==PLUGIN_PANEL?"PluginPanel":"FilePanel"),FileCount,_FARKEY_ToName(Key)));
      if (FileCount>0 && SetCurPath())
      {
        if (Key==KEY_SHIFTF8)
          ReturnCurrentFile=TRUE;
        if (PanelMode==PLUGIN_PANEL &&
            !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARDELETEFILES))
          PluginDelete();
        else
        {
          int SaveOpt=Opt.DeleteToRecycleBin;
          if (Key==KEY_SHIFTDEL)
            Opt.DeleteToRecycleBin=0;
          ShellDelete(this,Key==KEY_ALTDEL);
          Opt.DeleteToRecycleBin=SaveOpt;
        }
        if (Key==KEY_SHIFTF8)
          ReturnCurrentFile=FALSE;
      }
      return(TRUE);
    }

    /* $ 26.07.2001 VVM
       + � ������ ������� ������ �� 1 */
    /* $ 26.04.2001 VVM
       + ��������� ������ ����� */
    case KEY_MSWHEEL_UP:
    case (KEY_MSWHEEL_UP | KEY_ALT):
      Scroll(Key & KEY_ALT?-1:-Opt.MsWheelDelta);
      return(TRUE);

    case KEY_MSWHEEL_DOWN:
    case (KEY_MSWHEEL_DOWN | KEY_ALT):
      Scroll(Key & KEY_ALT?1:Opt.MsWheelDelta);
      return(TRUE);
    /* VVM $ */
    /* VVM $ */

    case KEY_HOME:         case KEY_NUMPAD7:
      Up(0x7fffff);
      return(TRUE);

    case KEY_END:          case KEY_NUMPAD1:
      Down(0x7fffff);
      return(TRUE);

    case KEY_UP:           case KEY_NUMPAD8:
      Up(1);
      return(TRUE);

    case KEY_DOWN:         case KEY_NUMPAD2:
      Down(1);
      return(TRUE);

    case KEY_PGUP:         case KEY_NUMPAD9:
      N=Columns*Height-1;
      CurTopFile-=N;
      Up(N);
      return(TRUE);

    case KEY_PGDN:         case KEY_NUMPAD3:
      N=Columns*Height-1;
      CurTopFile+=N;
      Down(N);
      return(TRUE);

    case KEY_LEFT:         case KEY_NUMPAD4:
      if ( (Columns==1 && Opt.ShellRightLeftArrowsRule == 1) || Columns>1 || CmdLength==0 )
      {
        if (CurTopFile>=Height && CurFile-CurTopFile<Height)
          CurTopFile-=Height;
        Up(Height);
        return(TRUE);
      }
      return(FALSE);

    case KEY_RIGHT:        case KEY_NUMPAD6:
      if ( (Columns==1 && Opt.ShellRightLeftArrowsRule == 1) || Columns>1 || CmdLength==0 )
      {
        if (CurFile+Height<FileCount && CurFile-CurTopFile>=(Columns-1)*(Height))
          CurTopFile+=Height;
        Down(Height);
        return(TRUE);
      }
      return(FALSE);
    /* $ 25.04.2001 DJ
       ����������� Shift-������� ��� Selected files first: ������ ����������
       ���� ���
    */
    case KEY_SHIFTHOME:    case KEY_SHIFTNUMPAD7:
    {
      InternalProcessKey++;
      Lock ();
      while (CurFile>0)
        ProcessKey(KEY_SHIFTUP);
      ProcessKey(KEY_SHIFTUP);
      InternalProcessKey--;
      Unlock ();
      if (SelectedFirst)
        SortFileList(TRUE);
      ShowFileList(TRUE);
      return(TRUE);
    }

    case KEY_SHIFTEND:     case KEY_SHIFTNUMPAD1:
    {
      InternalProcessKey++;
      Lock ();
      while (CurFile<FileCount-1)
        ProcessKey(KEY_SHIFTDOWN);
      ProcessKey(KEY_SHIFTDOWN);
      InternalProcessKey--;
      Unlock ();
      if (SelectedFirst)
        SortFileList(TRUE);
      ShowFileList(TRUE);
      return(TRUE);
    }

    case KEY_SHIFTPGUP:    case KEY_SHIFTNUMPAD9:
    case KEY_SHIFTPGDN:    case KEY_SHIFTNUMPAD3:
    {
      N=Columns*Height-1;
      InternalProcessKey++;
      Lock ();
      while (N--)
        ProcessKey(Key==KEY_SHIFTPGUP||Key==KEY_SHIFTNUMPAD9? KEY_SHIFTUP:KEY_SHIFTDOWN);
      InternalProcessKey--;
      Unlock ();
      if (SelectedFirst)
        SortFileList(TRUE);
      ShowFileList(TRUE);
      return(TRUE);
    }

    case KEY_SHIFTLEFT:    case KEY_SHIFTNUMPAD4:
    case KEY_SHIFTRIGHT:   case KEY_SHIFTNUMPAD6:
    {
      if (FileCount==0)
        return(TRUE);
      if (Columns>1)
      {
        int N=Height;
        InternalProcessKey++;
        Lock ();
        while (N--)
          ProcessKey(Key==KEY_SHIFTLEFT || Key==KEY_SHIFTNUMPAD4? KEY_SHIFTUP:KEY_SHIFTDOWN);
        Select(ListData+CurFile,ShiftSelection);
        if (SelectedFirst)
          SortFileList(TRUE);
        InternalProcessKey--;
        Unlock ();
        if (SelectedFirst)
          SortFileList(TRUE);
        ShowFileList(TRUE);
        return(TRUE);
      }
      return(FALSE);
    }

    case KEY_SHIFTUP:      case KEY_SHIFTNUMPAD8:
    case KEY_SHIFTDOWN:    case KEY_SHIFTNUMPAD2:
    {
      if (FileCount==0)
        return(TRUE);
      CurPtr=ListData+CurFile;
      if (ShiftSelection==-1)
      {
        // .. is never selected
        if (CurFile < FileCount-1 && TestParentFolderName(CurPtr->Name))
          ShiftSelection = !ListData [CurFile+1].Selected;
        else
          ShiftSelection=!CurPtr->Selected;
      }
      Select(CurPtr,ShiftSelection);
      if (Key==KEY_SHIFTUP || Key == KEY_SHIFTNUMPAD8)
        Up(1);
      else
        Down(1);
      if (SelectedFirst && !InternalProcessKey)
        SortFileList(TRUE);
      ShowFileList(TRUE);
      return(TRUE);
    }
    /* DJ $ */

    case KEY_INS:          case KEY_NUMPAD0:
    {
      if (FileCount==0)
        return(TRUE);
      CurPtr=ListData+CurFile;
      Select(CurPtr,!CurPtr->Selected);
      Down(1);
      if (SelectedFirst)
        SortFileList(TRUE);
      ShowFileList(TRUE);
      return(TRUE);
    }

    case KEY_CTRLF3:
      SetSortMode(BY_NAME);
      return(TRUE);

    case KEY_CTRLF4:
      SetSortMode(BY_EXT);
      return(TRUE);

    case KEY_CTRLF5:
      SetSortMode(BY_MTIME);
      return(TRUE);

    case KEY_CTRLF6:
      SetSortMode(BY_SIZE);
      return(TRUE);

    case KEY_CTRLF7:
      SetSortMode(UNSORTED);
      return(TRUE);

    case KEY_CTRLF8:
      SetSortMode(BY_CTIME);
      return(TRUE);

    case KEY_CTRLF9:
      SetSortMode(BY_ATIME);
      return(TRUE);

    case KEY_CTRLF10:
      SetSortMode(BY_DIZ);
      return(TRUE);

    case KEY_CTRLF11:
      SetSortMode(BY_OWNER);
      return(TRUE);

    case KEY_CTRLF12:
      SelectSortMode();
      return(TRUE);

    case KEY_SHIFTF11:
      SortGroups=!SortGroups;
      if (SortGroups)
        ReadSortGroups();
      SortFileList(TRUE);
      Show();
      return(TRUE);

    case KEY_SHIFTF12:
      SelectedFirst=!SelectedFirst;
      SortFileList(TRUE);
      Show();
      return(TRUE);

    case KEY_CTRLPGUP:     case KEY_CTRLNUMPAD9:
      /* $ 09.04.2001 SVS
         �� ��������������, ���� ChangeDir ������� ������
      */
    /* $ 16.08.2001 OT
     �������������� ������ ! ( ����� if, ����������� ChangeDir(".."))
    */
      /* $ 25.12.2001 DJ
         � ���� �� ����� ��������������, ���� ChangeDir() ������ ����������
         ������? ���������, �� this, � �������� ������. ������ ��� this
         ��� ���������!
      */
      ChangeDir("..");
      /* OT $ */
      /* $ 24.04.2001 IS
           ����������������� ������ ����� ������.
      */
      {
        Panel *NewActivePanel = CtrlObject->Cp()->ActivePanel;
        NewActivePanel->SetViewMode(NewActivePanel->GetViewMode());
        NewActivePanel->Show();
      }
      /* DJ $ */
      /* IS $ */
      /* SVS $ */
      return(TRUE);

    case KEY_CTRLPGDN:     case KEY_CTRLNUMPAD3:
      ProcessEnter(0,0);
      return(TRUE);

    default:
      if((Key>=KEY_ALT_BASE+0x01 && Key<=KEY_ALT_BASE+255 ||
          Key>=KEY_ALTSHIFT_BASE+0x01 && Key<=KEY_ALTSHIFT_BASE+255) &&
         Key != KEY_ALTBS && Key != (KEY_ALTBS|KEY_SHIFT)
        )
      {
        //_SVS(SysLog(">FastFind: Key=%s",_FARKEY_ToName(Key)));
        // ������������ ��� ����� ������ �������, �.�. WaitInFastFind
        // � ��� ����� ��� ����� ����.
        static const char Code[]=")!@#$%^&*(";
        if(Key >= KEY_ALTSHIFT0 && Key <= KEY_ALTSHIFT9)
          Key=(DWORD)Code[Key-KEY_ALTSHIFT0];
        else if((Key&(~(KEY_ALT+KEY_SHIFT))) == '/')
          Key='?';
        else if(Key == KEY_ALTSHIFT+'-')
          Key='_';
        else if(Key == KEY_ALTSHIFT+'=')
          Key='+';
        //_SVS(SysLog("<FastFind: Key=%s",_FARKEY_ToName(Key)));
        FastFind(Key);
      }
      else
        break;
      return(TRUE);
  }
  return(FALSE);
}


void FileList::Select(struct FileListItem *SelPtr,int Selection)
{
  if (!TestParentFolderName(SelPtr->Name) && SelPtr->Selected!=Selection)
    if ((SelPtr->Selected=Selection)!=0)
    {
      SelFileCount++;
      SelFileSize+=MKUINT64(SelPtr->UnpSizeHigh,SelPtr->UnpSize);
    }
    else
    {
       SelFileCount--;
       SelFileSize-=MKUINT64(SelPtr->UnpSizeHigh,SelPtr->UnpSize);
    }
}


void FileList::ProcessEnter(int EnableExec,int SeparateWindow)
{
  struct FileListItem *CurPtr;
  char FileName[NM],ShortFileName[NM],*ExtPtr;
  if (CurFile>=FileCount)
    return;

  CurPtr=ListData+CurFile;
  strcpy(FileName,CurPtr->Name);
  strcpy(ShortFileName,*CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name);
  if (CurPtr->FileAttr & FA_DIREC)
  {
    BOOL IsRealName=FALSE;
    if(PanelMode==PLUGIN_PANEL)
    {
      struct OpenPluginInfo Info;
      CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
      IsRealName=Info.Flags&OPIF_REALNAMES;
    }

    // Shift-Enter �� �������� �������� ���������
    if((PanelMode!=PLUGIN_PANEL || IsRealName) && SeparateWindow)
    {
      char FullPath[4096];
      if(!PathMayBeAbsolute(CurPtr->Name))
      {
        AddEndSlash(strcpy(FullPath,CurDir));
        /* 23.08.2001 VVM
          ! SHIFT+ENTER �� ".." ����������� ��� �������� ��������, � �� ������������� */
        if (!TestParentFolderName(CurPtr->Name))
          strcat(FullPath,CurPtr->Name);
        /* VVM $ */
      }
      else
      {
        strcpy(FullPath,CurPtr->Name);
      }
      QuoteSpace(FullPath);
      Execute(FullPath,FALSE,SeparateWindow?2:0,TRUE,CurPtr->FileAttr&FA_DIREC);
    }
    else
    {
      /* $ 09.04.2001 SVS
         �� ��������������, ���� ChangeDir ������� ������
      */
      BOOL res=FALSE;
      int CheckFullScreen=IsFullScreen();
      if (PanelMode==PLUGIN_PANEL || strchr(CurPtr->Name,'?')==NULL ||
          *CurPtr->ShortName==0)
      {
        res=ChangeDir(CurPtr->Name);
      }
      else
        res=ChangeDir(CurPtr->ShortName);
//      if(res)
      if(CheckFullScreen)
      {
        CtrlObject->Cp()->GetAnotherPanel(this)->Show();
      }
      CtrlObject->Cp()->ActivePanel->Show();
      /* SVS $ */
    }
  }
  else
  {
    int PluginMode=PanelMode==PLUGIN_PANEL &&
        !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILE);
    if (PluginMode)
    {
      char TempDir[NM];
      if(!FarMkTempEx(TempDir))
        return;
      CreateDirectory(TempDir,NULL);
      struct PluginPanelItem PanelItem;
      FileListToPluginItem(CurPtr,&PanelItem);
      if (!CtrlObject->Plugins.GetFile(hPlugin,&PanelItem,TempDir,FileName,OPM_SILENT|OPM_VIEW))
      {
        FAR_RemoveDirectory(TempDir);
        return;
      }
      ConvertNameToShort(FileName,ShortFileName,sizeof(ShortFileName)-1);
    }
    if (EnableExec && SetCurPath() && !SeparateWindow &&
        ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_EXEC,PluginMode))
    {
      if (PluginMode)
        DeleteFileWithFolder(FileName);
      return;
    }

    ExtPtr=strrchr((char *)FileName,'.');
    int ExeType=FALSE,BatType=FALSE;
    if (ExtPtr!=NULL)
    {
      ExeType=stricmp(ExtPtr,".exe")==0 || stricmp(ExtPtr,".com")==0;
      BatType=IsBatchExtType(ExtPtr);
    }
    if (EnableExec && (ExeType || BatType))
    {
      QuoteSpace(FileName);
      if (!(Opt.ExcludeCmdHistory&EXCLUDECMDHISTORY_NOTPANEL) && !PluginMode) //AN
        CtrlObject->CmdHistory->AddToHistory(FileName);

      int DirectRun=(CurDir[0]=='\\' && CurDir[1]=='\\' && ExeType);
      CtrlObject->CmdLine->ExecString(FileName,PluginMode,SeparateWindow,DirectRun);
      if (PluginMode)
        DeleteFileWithFolder(FileName);
    }
    else
      if (SetCurPath())
      {
        HANDLE hOpen=NULL;
        /* $ 02.08.2001 IS ���������� ���������� ��� ctrl-pgdn */

        if(!EnableExec &&     // �� ��������� � �� � ��������� ����,
           !SeparateWindow && // ������������� ��� Ctrl-PgDn
           ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_ALTEXEC,
           PluginMode)
          )
        {
          if (PluginMode)
            DeleteFileWithFolder(FileName);
          return;
        }
        /* IS $ */
        if (SeparateWindow || (hOpen=OpenFilePlugin(FileName,TRUE))==INVALID_HANDLE_VALUE ||
            hOpen==(HANDLE)-2)
        {
          if (EnableExec && hOpen!=(HANDLE)-2)
            if (SeparateWindow || Opt.UseRegisteredTypes)
              ProcessGlobalFileTypes(FileName,PluginMode);
          if (PluginMode)
            DeleteFileWithFolder(FileName);
        }
        return;
      }
  }
}


void FileList::SetCurDir(char *NewDir,int ClosePlugin)
{
  _ALGO(CleverSysLog clv("FileList::SetCurDir"));
  _ALGO(SysLog("(NewDir=\"%s\", ClosePlugin=%d)",NewDir,ClosePlugin));
  if (ClosePlugin && PanelMode==PLUGIN_PANEL)
  {
    while (1)
    {
      if (ProcessPluginEvent(FE_CLOSE,NULL))
        return;
      if (!PopPlugin(TRUE))
        break;
    }
    CtrlObject->Cp()->RedrawKeyBar();
  }
  /* $ 20.07.2001 VVM
    ! ��������� �� �������� ������ */
  if ((NewDir) && (*NewDir))
  {
    ChangeDir(NewDir);
  }
  /* VVM $ */
}


BOOL FileList::ChangeDir(char *NewDir,BOOL IsUpdated)
{
  Panel *AnotherPanel;
  char FindDir[4096],SetDir[4096];

  strcpy(SetDir,NewDir);
  PrepareDiskPath(SetDir,sizeof(SetDir)-1);

  if (!TestParentFolderName(SetDir) && strcmp(SetDir,"\\")!=0)
    UpperFolderTopFile=CurTopFile;

  if (SelFileCount>0)
    ClearSelection();

  int PluginClosed=FALSE,GoToPanelFile=FALSE;
  if (PanelMode==PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);

    /* $ 16.01.2002 VVM
      + ���� � ������� ��� OPIF_REALNAMES, �� ������� ����� �� ������� � ������ */
    CtrlObject->FolderHistory->AddToHistory(NullToEmpty(Info.CurDir),Info.Format,1,
                               (Info.Flags & OPIF_REALNAMES)?0:(Opt.SavePluginFoldersHistory?0:1));
    /* VVM $ */

    /* $ 25.04.01 DJ
       ��� ������� SetDirectory �� ���������� ���������
    */
    BOOL SetDirectorySuccess = TRUE;
    /* DJ $ */
    int UpperFolder=TestParentFolderName(SetDir);
    if (UpperFolder && *NullToEmpty(Info.CurDir)==0)
    {
      if (ProcessPluginEvent(FE_CLOSE,NULL))
        return(TRUE);
      PluginClosed=TRUE;
      strcpy(FindDir,NullToEmpty(Info.HostFile));
      if (*FindDir==0 && (Info.Flags & OPIF_REALNAMES) && CurFile<FileCount)
      {
        strcpy(FindDir,ListData[CurFile].Name);
        GoToPanelFile=TRUE;
      }
      PopPlugin(TRUE);
      Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
      if (AnotherPanel->GetType()==INFO_PANEL)
        AnotherPanel->Redraw();
    }
    else
    {
      strcpy(FindDir,NullToEmpty(Info.CurDir));
      /* $ 25.04.01 DJ
         ��� ������� SetDirectory �� ���������� ���������
      */
      SetDirectorySuccess=CtrlObject->Plugins.SetDirectory(hPlugin,SetDir,0);
    }
    ProcessPluginCommand();
    if (SetDirectorySuccess)
      Update(0);
    else
      Update(UPDATE_KEEP_SELECTION);
    /* DJ $ */
    if (PluginClosed && PrevDataStackSize>0)
    {
      PrevDataStackSize--;
      if (PrevDataStack[PrevDataStackSize].PrevFileCount>0)
      {
        MoveSelection(ListData,FileCount,PrevDataStack[PrevDataStackSize].PrevListData,PrevDataStack[PrevDataStackSize].PrevFileCount);
        UpperFolderTopFile = PrevDataStack[PrevDataStackSize].PrevTopFile;
        if (!GoToPanelFile)
          strcpy(FindDir,PrevDataStack[PrevDataStackSize].PrevName);
        DeleteListData(PrevDataStack[PrevDataStackSize].PrevListData,PrevDataStack[PrevDataStackSize].PrevFileCount);
        if (ListSelectedFirst)
          SortFileList(FALSE);
        else if (FileCount>0)
          SortFileList(TRUE);
      }
    }

    if (UpperFolder)
    {
      long Pos=FindFile(PointToName(FindDir));
      if (Pos!=-1)
        CurFile=Pos;
      else
        GoToFile(FindDir);
      CurTopFile=UpperFolderTopFile;
      UpperFolderTopFile=0;
      CorrectPosition();
    }
    /* $ 26.04.2001 DJ
       ������� ��� ������� ��������� ��� ������� SetDirectory
    */
    else if (SetDirectorySuccess)
      CurFile=CurTopFile=0;
    /* DJ $ */
    return(TRUE);
  }
  else
  {
    char FullNewDir[NM];
    //_SVS(char FullCurDir[NM]);
//    ConvertNameToFull(SetDir,FullNewDir, sizeof(FullNewDir));
    if (ConvertNameToFull(SetDir,FullNewDir, sizeof(FullNewDir)) >= sizeof(FullNewDir)){
      return (TRUE);
    }
    //_SVS(ConvertNameToFull(CurDir,FullCurDir, sizeof(FullCurDir)));
    //_SVS(SysLog("\nFullNewDir=%s\nFullCurDir=%s",FullNewDir,FullCurDir));

    if (LocalStricmp(FullNewDir,CurDir)!=0)
      CtrlObject->FolderHistory->AddToHistory(CurDir,NULL,0);

    /* $ 21.09.2000 SVS
       ������� ������ ".." � "\\host\share"
    */
    if(TestParentFolderName(SetDir))
    {
      /* $ 21.08.2001 KM
        - ����������� ����� � ������� ���� ������ ������ �� UNC �����
          ��� ������ �� ������������ �������� �������:
          \\Server\Disk\Dir1\Dir2
          ��� ������ �� Dir2 ���������� ���� ������.
      */
      char RootDir[NM],TempDir[NM];
      xstrncpy(TempDir,CurDir,sizeof(TempDir)-1);
      TempDir[NM-1]=0;
      AddEndSlash(TempDir);
      GetPathRoot(TempDir,RootDir);

      if((CurDir[0] == '\\' && CurDir[1] == '\\' && strcmp(TempDir,RootDir)==0) ||
         (CurDir[1] == ':'  && CurDir[2] == '\\' && CurDir[3]==0))
      {
      /* KM $ */
        /* $ 08.05.2001 SVS
           ��� ���������� ������ ���� ������� ���� ������ ������
           ����� ���� ����� �������� ����� ������-���� �������.
           ��������, ��� �������� SysID
           CtrlObject->Plugins.CallPlugin(PLG_MYCOMP_SYSID,OPEN_FILEPANEL,CurDir);
           ������� ����� ���������� ������ ���� "���� ����" :-)
        */
        /* $ 10.06.2001 KM
           - ������� GetDriveType ������� ���� � "\" �� �����
             ��� ����������� ����������� ���� ������.
        */
        char DirName[NM];
        xstrncpy(DirName,CurDir,sizeof(DirName)-1);
        AddEndSlash(DirName);
        if(Opt.PgUpChangeDisk &&
          (FAR_GetDriveType(DirName) != DRIVE_REMOTE ||
           CtrlObject->Plugins.FindPlugin(SYSID_NETWORK) == -1))
        {
          CtrlObject->Cp()->ActivePanel->ChangeDisk();
          return TRUE;
        }
        /* KM $ */
        /* SVS $ */
        /* $ 26.03.2001 SVS
           ������� ����������� ������ Network-������� �� ����� ����������
           ������.
        */
        char NewCurDir[NM*2];
        strcpy(NewCurDir,CurDir);
        if(NewCurDir[1] == ':')
        {
          char Letter=*NewCurDir;
          DriveLocalToRemoteName(DRIVE_REMOTE,Letter,NewCurDir);
        }
        if(*NewCurDir) // �������� - ����� �� ������� ���������� RemoteName
        {
          char *PtrS1=strchr(NewCurDir+2,'\\');
          if(PtrS1 && !strchr(PtrS1+1,'\\'))
          {
  // _D(SysLog("1) SetDir=%s  NewCurDir=%s",SetDir,NewCurDir));
            if(CtrlObject->Plugins.CallPlugin(SYSID_NETWORK,OPEN_FILEPANEL,NewCurDir)) // NetWork Plugin :-)
            {
  //_D(SysLog("2) SetDir=%s  NewCurDir=%s",SetDir,NewCurDir));
              return(FALSE);
            }
          }
        }
        /* SVS $ */
      }
    }
    /* SVS $ */
  }

  strcpy(FindDir,PointToName(CurDir));

  if (SetDir[0]==0 || SetDir[1]!=':' || SetDir[2]!='\\')
    FarChDir(CurDir);

  /* $ 26.04.2001 DJ
     ���������, ������� �� ������� �������, � ��������� � KEEP_SELECTION,
     ���� �� �������
  */
  int UpdateFlags = 0;

  // ...����� ����� � ������ cd //host/share
  if(WinVer.dwPlatformId != VER_PLATFORM_WIN32_NT &&
    SetDir[0] == '/' && SetDir[1] == '/')
  {
    char *Ptr=SetDir;
    while(*Ptr)
    {
      if(*Ptr == '/')
        *Ptr='\\';
      ++Ptr;
    }
  }

  if(PanelMode!=PLUGIN_PANEL && !strcmp(SetDir,"\\"))
  {
#if 1    // ���� ��������� 0, �� ��� ����� ��������� � ������ ���� �����, ������� ��������� �� �������� �������
    GetPathRootOne(CurDir,SetDir);
#else
    GetPathRoot(CurDir,SetDir);
    if(!strncmp(SetDir,"\\\\?\\Volume{",11)) // ������, ����� ��� ����������� �� NTFS � �������� ��������, �� ����� �� �����.
      GetPathRootOne(CurDir,SetDir);
#endif
  }

  if (!FarChDir(SetDir))
  {
    /* $ 03.11.2001 IS
         ������ ��� ���������� ��������
    */
    char Target[NM];
    xstrncpy(Target, SetDir, sizeof(Target)-1);
    TruncPathStr(Target, ScrX-16);
    Message (MSG_WARNING | MSG_ERRORTYPE, 1, MSG (MError), Target, MSG (MOk));
    /* IS $ */
    UpdateFlags = UPDATE_KEEP_SELECTION;
  }
  /* $ 28.04.2001 IS
       ������������� "�� ������ ������".
       � �� ����, ������ ���� ���������� ������ � ����, �� ���� ����, ������ ��
       ��� ������-���� ������ ���������. �������� ����� ������� RTFM. ���� ���
       ��������: chdir, setdisk, SetCurrentDirectory � ���������� ���������

  */
  /*else {
    if (isalpha(SetDir[0]) && SetDir[1]==':')
    {
      int CurDisk=toupper(SetDir[0])-'A';
      setdisk(CurDisk);
    }
  }*/
  /* IS $ */
  FarGetCurDir(sizeof(CurDir),CurDir);

  if(!IsUpdated)
    return(TRUE);

  Update(UpdateFlags);

  if (TestParentFolderName(SetDir))
  {
    GoToFile(FindDir);
    CurTopFile=UpperFolderTopFile;
    UpperFolderTopFile=0;
    CorrectPosition();
  }
  else if (UpdateFlags != UPDATE_KEEP_SELECTION)
    CurFile=CurTopFile=0;
  /* DJ $ */

  if (GetFocus())
  {
    CtrlObject->CmdLine->SetCurDir(CurDir);
    CtrlObject->CmdLine->Show();
  }
  AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  if (AnotherPanel->GetType()!=FILE_PANEL)
  {
    AnotherPanel->SetCurDir(CurDir,FALSE);
    AnotherPanel->Redraw();
  }
  if (PanelMode==PLUGIN_PANEL)
    CtrlObject->Cp()->RedrawKeyBar();
  return(TRUE);
}


int FileList::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  struct FileListItem *CurPtr;
  int RetCode;

  if (IsVisible() && Opt.ShowColumnTitles && MouseEvent->dwEventFlags==0 &&
      MouseEvent->dwMousePosition.Y==Y1+1 &&
      MouseEvent->dwMousePosition.X>X1 && MouseEvent->dwMousePosition.X<X1+3)
  {
    if (MouseEvent->dwButtonState)
      if (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
        ChangeDisk();
      else
        SelectSortMode();
    return(TRUE);
  }

  if (IsVisible() && Opt.ShowPanelScrollbar && MouseX==X2 &&
      (MouseEvent->dwButtonState & 1) && !IsDragging())
  {
    int ScrollY=Y1+1+Opt.ShowColumnTitles;
    if (MouseY==ScrollY)
    {
      while (IsMouseButtonPressed())
        ProcessKey(KEY_UP);
      SetFocus();
      return(TRUE);
    }
    if (MouseY==ScrollY+Height-1)
    {
      while (IsMouseButtonPressed())
        ProcessKey(KEY_DOWN);
      SetFocus();
      return(TRUE);
    }
    if (MouseY>ScrollY && MouseY<ScrollY+Height-1 && Height>2)
    {
      CurFile=(FileCount-1)*(MouseY-ScrollY)/(Height-2);
      ShowFileList(TRUE);
      SetFocus();
      return(TRUE);
    }
  }

  /* $ 21.08.2001 VVM
    + ������� ������� ������� ������ �� ����� */
  /* $ 17.12.2001 IS
    ! ��������� �� ������� - �����������
  */
  if ((MouseEvent->dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
      && (MouseEvent->dwEventFlags != MOUSE_MOVED)
      && Opt.PanelMiddleClickRule)
  /* IS $ */
  {
    int Key = KEY_ENTER;
    if (MouseEvent->dwControlKeyState & SHIFT_PRESSED)
      Key |= KEY_SHIFT;
    if (MouseEvent->dwControlKeyState & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED))
      Key |= KEY_CTRL;
    if (MouseEvent->dwControlKeyState & (LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED))
      Key |= KEY_ALT;
    ProcessKey(Key);
    return(TRUE);
  }
  /* VVM $ */

  if (Panel::PanelProcessMouse(MouseEvent,RetCode))
    return(RetCode);

  if (MouseEvent->dwMousePosition.Y>Y1+Opt.ShowColumnTitles &&
      MouseEvent->dwMousePosition.Y<Y2-2*Opt.ShowPanelStatus)
  {
    SetFocus();
    if (FileCount==0)
      return(TRUE);
    MoveToMouse(MouseEvent);
    CurPtr=ListData+CurFile;

    if ((MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) &&
        MouseEvent->dwEventFlags==DOUBLE_CLICK)
    {
      if (PanelMode==PLUGIN_PANEL)
      {
        FlushInputBuffer(); // !!!
        int ProcessCode=CtrlObject->Plugins.ProcessKey(hPlugin,VK_RETURN,ShiftPressed ? PKF_SHIFT:0);
        ProcessPluginCommand();
        if (ProcessCode)
          return(TRUE);
      }
      /*$ 21.02.2001 SKV
        ���� ������ DOUBLE_CLICK ��� ���������������� ���
        �������� �����, �� ������ �� ����������������.
        ���������� ���.
        �� ���� ��� ���������� DOUBLE_CLICK, �����
        ������� �����������...
        �� �� �� �������� Fast=TRUE...
        ����� �� ������ ���� ��.
      */
      ShowFileList(TRUE);
      /* SKV$*/
      FlushInputBuffer();
      ProcessEnter(1,ShiftPressed!=0);
      return(TRUE);
    }
    else
    {
      /* $ 11.09.2000 SVS
         Bug #17: �������� ��� �������, ��� ������� ��������� �����.
      */
      if ((MouseEvent->dwButtonState & RIGHTMOST_BUTTON_PRESSED) && !IsEmpty)
      {
        if (MouseEvent->dwEventFlags==0)
          MouseSelection=!CurPtr->Selected;
        Select(CurPtr,MouseSelection);
        if (SelectedFirst)
          SortFileList(TRUE);
      }
      /* SVS $ */
    }
    ShowFileList(TRUE);
    return(TRUE);
  }
  if (MouseEvent->dwMousePosition.Y<=Y1+1)
  {
    SetFocus();
    if (FileCount==0)
      return(TRUE);
    while (IsMouseButtonPressed() && MouseY<=Y1+1)
    {
      Up(1);
      if (RButtonPressed)
      {
        CurPtr=ListData+CurFile;
        Select(CurPtr,MouseSelection);
      }
    }
    if (SelectedFirst)
      SortFileList(TRUE);
    return(TRUE);
  }
  if (MouseEvent->dwMousePosition.Y>=Y2-2)
  {
    SetFocus();
    if (FileCount==0)
      return(TRUE);
    while (IsMouseButtonPressed() && MouseY>=Y2-2)
    {
      Down(1);
      if (RButtonPressed)
      {
        CurPtr=ListData+CurFile;
        Select(CurPtr,MouseSelection);
      }
    }
    if (SelectedFirst)
      SortFileList(TRUE);
    return(TRUE);
  }
  return(FALSE);
}


/* $ 12.09.2000 SVS
  + ������������ ��������� ��� ������ ������� ���� �� ������ ������
*/
void FileList::MoveToMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  int CurColumn=1,ColumnsWidth,I;
  int PanelX=MouseEvent->dwMousePosition.X-X1-1;

  int Level = 0;

  for (ColumnsWidth=I=0;I<ViewSettings.ColumnCount;I++)
  {
    if ( Level == ColumnsInGlobal )
    {
      CurColumn++;
      Level = 0;
    }
    ColumnsWidth+=ViewSettings.ColumnWidth[I];
    if (ColumnsWidth>=PanelX)
      break;
    ColumnsWidth++;
    Level++;
  }
//  if (CurColumn==0)
//    CurColumn=1;
  int OldCurFile=CurFile;
  CurFile=CurTopFile+MouseEvent->dwMousePosition.Y-Y1-1-Opt.ShowColumnTitles;
  if (CurColumn>1)
    CurFile+=(CurColumn-1)*Height;
  CorrectPosition();
  /* $ 11.09.2000 SVS
     Bug #17: �������� �� ��������� ������ �������.
  */
  if(Opt.PanelRightClickRule == 1)
    IsEmpty=((CurColumn-1)*Height > FileCount);
  else if(Opt.PanelRightClickRule == 2 &&
          (MouseEvent->dwButtonState & RIGHTMOST_BUTTON_PRESSED) &&
          ((CurColumn-1)*Height > FileCount))
  {
    CurFile=OldCurFile;
    IsEmpty=TRUE;
  }
  else
    IsEmpty=FALSE;
  /* SVS $ */
}
/* SVS $ */

void FileList::SetViewMode(int ViewMode)
{
  int CurFullScreen=IsFullScreen();
  int OldOwner=IsColumnDisplayed(OWNER_COLUMN);
  int OldPacked=IsColumnDisplayed(PACKED_COLUMN);
  int OldNumLink=IsColumnDisplayed(NUMLINK_COLUMN);
  int OldDiz=IsColumnDisplayed(DIZ_COLUMN);
  int OldCaseSensitiveSort=ViewSettings.CaseSensitiveSort;
  int OldNumericSort=NumericSort;
  PrepareViewSettings(ViewMode,NULL);
  int NewOwner=IsColumnDisplayed(OWNER_COLUMN);
  int NewPacked=IsColumnDisplayed(PACKED_COLUMN);
  int NewNumLink=IsColumnDisplayed(NUMLINK_COLUMN);
  int NewDiz=IsColumnDisplayed(DIZ_COLUMN);
  int NewAccessTime=IsColumnDisplayed(ADATE_COLUMN);
  int NewCaseSensitiveSort=ViewSettings.CaseSensitiveSort;
  int NewNumericSort=NumericSort;
  int ResortRequired=FALSE;

  char DriveRoot[NM];
  DWORD FileSystemFlags;
  GetPathRoot(CurDir,DriveRoot);
  if (NewPacked && GetVolumeInformation(DriveRoot,NULL,0,NULL,NULL,&FileSystemFlags,NULL,0))
    if ((FileSystemFlags & FS_FILE_COMPRESSION)==0)
      NewPacked=FALSE;

  if (FileCount>0 && PanelMode!=PLUGIN_PANEL &&
      (!OldOwner && NewOwner || !OldPacked && NewPacked ||
       !OldNumLink && NewNumLink ||
       AccessTimeUpdateRequired && NewAccessTime))
    Update(UPDATE_KEEP_SELECTION);
  else
    if (OldCaseSensitiveSort!=NewCaseSensitiveSort || OldNumericSort!=NewNumericSort) //????
      ResortRequired=TRUE;

  if (!OldDiz && NewDiz)
    ReadDiz();

  if (ViewSettings.FullScreen && !CurFullScreen)
  {
    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
    int AnotherVisible=AnotherPanel->IsVisible();
//    Hide();
//    AnotherPanel->Hide();
    if (Y2>0)
      SetPosition(0,Y1,ScrX,Y2);
    FileList::ViewMode=ViewMode;
//    if (AnotherVisible)
//      AnotherPanel->Show();
//    Show();
  }
  else
    if (!ViewSettings.FullScreen && CurFullScreen)
    {
      Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
      int AnotherVisible=AnotherPanel->IsVisible();
      int CurrentVisible=IsVisible();
//      Hide();
//      AnotherPanel->Hide();
      if (Y2>0)
        if (this==CtrlObject->Cp()->LeftPanel)
          SetPosition(0,Y1,ScrX/2-Opt.WidthDecrement,Y2);
        else
          SetPosition(ScrX/2+1-Opt.WidthDecrement,Y1,ScrX,Y2);
      FileList::ViewMode=ViewMode;
//      if (AnotherVisible)
//        AnotherPanel->Show();
//      if (CurrentVisible)
//        Show();
    }
    else
    {
      FileList::ViewMode=ViewMode;
      FrameManager->RefreshFrame();
    }

  if (PanelMode==PLUGIN_PANEL)
  {
    char ColumnTypes[NM];
//    SetScreenPosition();
    ViewSettingsToText(ViewSettings.ColumnType,ViewSettings.ColumnWidth,
        ViewSettings.ColumnCount,ColumnTypes);
    ProcessPluginEvent(FE_CHANGEVIEWMODE,ColumnTypes);
  }

  if (ResortRequired)
  {
    SortFileList(TRUE);
    ShowFileList(TRUE);
    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
    if (AnotherPanel->GetType()==TREE_PANEL)
      AnotherPanel->Redraw();
  }
}


void FileList::SetSortMode(int SortMode)
{
  if (SortMode==FileList::SortMode && Opt.ReverseSort)
    SortOrder=-SortOrder;
  else
    SortOrder=1;
  FileList::SortMode=SortMode;
  if (FileCount>0)
    SortFileList(TRUE);
  FrameManager->RefreshFrame();
}


int FileList::GoToFile(const char *Name,BOOL OnlyPartName)
{
  long Pos=FindFile(Name,OnlyPartName);
  if (Pos!=-1)
  {
    CurFile=Pos;
    CorrectPosition();
    return(TRUE);
  }
  return(FALSE);
}


int FileList::FindFile(const char *Name,BOOL OnlyPartName)
{
  long I;
  struct FileListItem *CurPtr;

  for (CurPtr=ListData, I=0; I < FileCount; I++, CurPtr++)
  {
    char *CurPtrName=CurPtr->Name;
    if(OnlyPartName)
      CurPtrName=PointToName(CurPtr->Name);

    if (strcmp(Name,CurPtrName)==0)
      return I;
    if (LocalStricmp(Name,CurPtrName)==0)
      return I;
  }
  return -1;
}

int FileList::FindFirst(const char *Name)
{
  return FindNext(0,Name);
}

int FileList::FindNext(int StartPos, const char *Name)
{
  int I;
  struct FileListItem *CurPtr;

  if((DWORD)StartPos < (DWORD)FileCount)
    for( CurPtr=ListData+StartPos, I=StartPos; I < FileCount; I++, CurPtr++ )
    {
      if (CmpName(Name,CurPtr->Name,TRUE))
        if (!TestParentFolderName(CurPtr->Name))
          return I;
    }
  return -1;
}


int FileList::IsSelected(char *Name)
{
  long Pos=FindFile(Name);
  return(Pos!=-1 && (ListData[Pos].Selected || SelFileCount==0 && Pos==CurFile));
}

// $ 02.08.2000 IG  Wish.Mix #21 - ��� ������� '/' ��� '\' � QuickSerach ��������� �� ����������
int FileList::FindPartName(char *Name,int Next,int Direct)
{
  char Mask[NM*2];
  int I;
  struct FileListItem *CurPtr;

  int DirFind = 0;
  int Length = Min((int)strlen(Name),(int)(sizeof(Mask)-1));

  xstrncpy(Mask,Name,sizeof(Mask)-1);
  if ( Length > 0 && (Name[Length-1] == '/' || Name[Length-1] == '\\') )
  {
    DirFind = 1;
    Mask[Length-1] = '*';
  }
  else
  {
    Mask[Length] = '*';
    Mask[Length+1] = 0;
  }
  for (I=CurFile+(Next?Direct:0), CurPtr=ListData+I; I >= 0 && I < FileCount; I+=Direct, (Direct==1?CurPtr++:CurPtr--))
  {
    CmpNameSearchMode=(I==CurFile);
    if (CmpName(Mask,CurPtr->Name,TRUE))
      if (!TestParentFolderName(CurPtr->Name))
        if (!DirFind || (CurPtr->FileAttr & FA_DIREC))
        {
          CmpNameSearchMode=FALSE;
          CurFile=I;
          CurTopFile=CurFile-(Y2-Y1)/2;
          ShowFileList(TRUE);
          return(TRUE);
        }
  }
  CmpNameSearchMode=FALSE;

  for(
      CurPtr=ListData+(Direct > 0?0:FileCount-1), I=(Direct > 0)?0:FileCount-1;
      (Direct > 0) ? I < CurFile:I > CurFile;
      I+=Direct, (Direct > 0 ? CurPtr++ : CurPtr--)
     )
  {
    if (CmpName(Mask,CurPtr->Name,TRUE))
      if (!TestParentFolderName(CurPtr->Name))
        if (!DirFind || (CurPtr->FileAttr & FA_DIREC))
        {
          CurFile=I;
          CurTopFile=CurFile-(Y2-Y1)/2;
          ShowFileList(TRUE);
          return(TRUE);
        }
  }
  return(FALSE);
}

int FileList::GetSelCount()
{
  if (FileCount==0)
    return(0);
  if (SelFileCount==0 || ReturnCurrentFile)
    return(1);
  return(SelFileCount);
}

int FileList::GetRealSelCount()
{
  if (FileCount==0)
    return(0);
  return(SelFileCount);
}


int FileList::GetSelName(char *Name,int &FileAttr,char *ShortName,WIN32_FIND_DATA *fd)
{
  if (Name==NULL)
  {
    GetSelPosition=0;
    LastSelPosition=-1;
    return(TRUE);
  }

  if (SelFileCount==0 || ReturnCurrentFile)
  {
    if (GetSelPosition==0 && CurFile<FileCount)
    {
      GetSelPosition=1;
      strcpy(Name,ListData[CurFile].Name);
      if (ShortName!=NULL)
      {
        strcpy(ShortName,ListData[CurFile].ShortName);
        if (*ShortName==0)
          strcpy(ShortName,Name);
      }
      FileAttr=ListData[CurFile].FileAttr;
      LastSelPosition=CurFile;

      if (fd)
      {
        fd->dwFileAttributes=ListData[CurFile].FileAttr;
        fd->ftCreationTime=ListData[CurFile].CreationTime;
        fd->ftLastAccessTime=ListData[CurFile].AccessTime;
        fd->ftLastWriteTime=ListData[CurFile].WriteTime;
        fd->nFileSizeHigh=ListData[CurFile].UnpSizeHigh;
        fd->nFileSizeLow=ListData[CurFile].UnpSize;
        fd->dwReserved0=ListData[CurFile].PackSizeHigh;
        fd->dwReserved1=ListData[CurFile].PackSize;
        xstrncpy(fd->cFileName,ListData[CurFile].Name,MAX_PATH-1);
        xstrncpy(fd->cAlternateFileName,ListData[CurFile].ShortName,sizeof(fd->cAlternateFileName)-1);
      }

      return(TRUE);
    }
    else
      return(FALSE);
  }

  while (GetSelPosition<FileCount)
    if (ListData[GetSelPosition++].Selected)
    {
      strcpy(Name,ListData[GetSelPosition-1].Name);
      if (ShortName!=NULL)
      {
        strcpy(ShortName,ListData[GetSelPosition-1].ShortName);
        if (*ShortName==0)
          strcpy(ShortName,Name);
      }
      FileAttr=ListData[GetSelPosition-1].FileAttr;
      LastSelPosition=GetSelPosition-1;

      if (fd)
      {
        fd->dwFileAttributes=ListData[GetSelPosition-1].FileAttr;
        fd->ftCreationTime=ListData[GetSelPosition-1].CreationTime;
        fd->ftLastAccessTime=ListData[GetSelPosition-1].AccessTime;
        fd->ftLastWriteTime=ListData[GetSelPosition-1].WriteTime;
        fd->nFileSizeHigh=ListData[GetSelPosition-1].UnpSizeHigh;
        fd->nFileSizeLow=ListData[GetSelPosition-1].UnpSize;
        fd->dwReserved0=ListData[GetSelPosition-1].PackSizeHigh;
        fd->dwReserved1=ListData[GetSelPosition-1].PackSize;
        xstrncpy(fd->cFileName,ListData[GetSelPosition-1].Name,MAX_PATH-1);
        xstrncpy(fd->cAlternateFileName,ListData[GetSelPosition-1].ShortName,sizeof(fd->cAlternateFileName)-1);
      }

      return(TRUE);
    }
  return(FALSE);
}


void FileList::ClearLastGetSelection()
{
  if (LastSelPosition>=0 && LastSelPosition<FileCount)
    Select(ListData+LastSelPosition,0);
}


void FileList::UngetSelName()
{
  GetSelPosition=LastSelPosition;
}


long FileList::GetLastSelectedSize(__int64 *Size)
{
  if (LastSelPosition>=0 && LastSelPosition<FileCount)
  {
    if (Size!=NULL)
      *Size=MKUINT64(ListData[LastSelPosition].UnpSizeHigh,ListData[LastSelPosition].UnpSize);
    return(ListData[LastSelPosition].UnpSize);
  }
  return(-1);
}


int FileList::GetLastSelectedItem(struct FileListItem *LastItem)
{
  if (LastSelPosition>=0 && LastSelPosition<FileCount)
  {
    *LastItem=ListData[LastSelPosition];
    return(TRUE);
  }
  return(FALSE);
}


int FileList::GetCurName(char *Name,char *ShortName)
{
  if (FileCount==0)
  {
    *Name=*ShortName=0;
    return(FALSE);
  }
  strcpy(Name,ListData[CurFile].Name);
  strcpy(ShortName,ListData[CurFile].ShortName);
  if (*ShortName==0)
    strcpy(ShortName,Name);
  return(TRUE);
}

int FileList::GetCurBaseName(char *Name,char *ShortName)
{
  *Name=*ShortName=0;
  if (FileCount==0)
    return(FALSE);
  if(PanelMode==PLUGIN_PANEL && PluginsStack) // ��� ��������
  {
    // ����� ����� ������ (��� ���������)
    strcpy(Name,PointToName(NullToEmpty(PluginsStack->HostFile)));
  }
  else if(PanelMode==NORMAL_PANEL)
  {
    strcpy(Name,ListData[CurFile].Name);
    strcpy(ShortName,ListData[CurFile].ShortName);
  }

  if (*ShortName==0)
    strcpy(ShortName,Name);
  return(TRUE);
}

void FileList::SelectFiles(int Mode)
{
  CFileMask FileMask; // ����� ��� ������ � �������
  const char *HistoryName="Masks";
  static struct DialogData SelectDlgData[]=
  {
    DI_DOUBLEBOX,3,1,51,5,0,0,0,0,"",
    DI_EDIT,5,2,49,2,1,(DWORD_PTR)HistoryName,DIF_HISTORY,0,"",
    DI_TEXT,0,3,0,3,0,0,DIF_SEPARATOR,0,"",
    DI_BUTTON,0,4,0,4,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,4,0,4,0,0,DIF_CENTERGROUP,0,(char *)MSelectFilter,
    DI_BUTTON,0,4,0,4,0,0,DIF_CENTERGROUP,0,(char *)MCancel,
  };
  MakeDialogItems(SelectDlgData,SelectDlg);

  FileFilter Filter(this,FFT_SELECT);
  bool bUseFilter = false;

  struct FileListItem *CurPtr;
  static char PrevMask[NM]="*.*";
  /* $ 20.05.2002 IS
     ��� ��������� �����, ���� �������� � ������ ����� �� ������,
     ����� ������ ���������� ������ � ����� ��� ����������� ����� � ������,
     ����� �������� ����� ������������� ���������� ������ - ��� ���������,
     ��������� CmpName.
  */
  char Mask[NM]="*.*", RawMask[NM];
  int Selection=0,I;
  bool WrapBrackets=false; // ������� � ���, ��� ����� ����� ��.������ � ������

  if (CurFile>=FileCount)
    return;

  int RawSelection=FALSE;
  if (PanelMode==PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
    RawSelection=(Info.Flags & OPIF_RAWSELECTION);
  }

  CurPtr=&ListData[CurFile];
  char *CurName=(ShowShortNames && *CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name);

  if (Mode==SELECT_ADDEXT || Mode==SELECT_REMOVEEXT)
  {
    char *DotPtr=strrchr(CurName,'.');
    if (DotPtr!=NULL)
    {
      // ����� ��� ������, ��� ���������� ����� ��������� �������-�����������
      sprintf(RawMask, "\"*.%s\"", DotPtr+1);
      WrapBrackets=true;
    }
    else
      strcpy(Mask,"*.");
    Mode=(Mode==SELECT_ADDEXT) ? SELECT_ADD:SELECT_REMOVE;
  }
  else
  {
    if (Mode==SELECT_ADDNAME || Mode==SELECT_REMOVENAME)
    {
      // ����� ��� ������, ��� ��� ����� ��������� �������-�����������
      sprintf(RawMask,"\"%s", CurName);
      char *DotPtr=strrchr(RawMask,'.');
      if (DotPtr!=NULL)
        strcpy(DotPtr,".*\"");
      else
        strcat(RawMask,".*\"");
      WrapBrackets=true;
      Mode=(Mode==SELECT_ADDNAME) ? SELECT_ADD:SELECT_REMOVE;
    }
    else
    {
      if (Mode==SELECT_ADD || Mode==SELECT_REMOVE)
      {
        strcpy(SelectDlg[1].Data,PrevMask);
        if (Mode==SELECT_ADD)
          strcpy(SelectDlg[0].Data,MSG(MSelectTitle));
        else
          strcpy(SelectDlg[0].Data,MSG(MUnselectTitle));
        {
          Dialog Dlg(SelectDlg,sizeof(SelectDlg)/sizeof(SelectDlg[0]));
          Dlg.SetHelp("SelectFiles");
          Dlg.SetPosition(-1,-1,55,7);
          for(;;)
          {
            Dlg.ClearDone();
            Dlg.Process();
            if (Dlg.GetExitCode()==4 && Filter.FilterEdit())
            {
              bUseFilter = true;
              break;
            }
            if (Dlg.GetExitCode()!=3)
              return;
            xstrncpy(Mask,SelectDlg[1].Data,sizeof(Mask)-1);
            Mask[sizeof(Mask)-1]=0;
            if(FileMask.Set(Mask, 0)) // �������� �������� ������������� ����� �� ������
            {
              // Unquote(Mask); �� �����! �.�. ��� �������� � FileMask.Set()
              strcpy(PrevMask,Mask);
              break;
            }
          }
        }
      }
    }
  }
  SaveSelection();

  if(!bUseFilter && WrapBrackets) // ������� ��.������ � ������, ����� ��������
  {                               // ��������������� �����
     const char *src=RawMask;
     const int maxlen=sizeof(Mask)-1;
     int dest=0;
     for(;*src && dest<maxlen;++src)
     {
       if(*src==']' || *src=='[')
       {
         Mask[dest++]='[';
         Mask[dest++]=*src;
         Mask[dest++]=']';
       }
       else
         Mask[dest++]=*src;
     }
     Mask[dest]=0;
  }

  if(bUseFilter || FileMask.Set(Mask, FMF_SILENT)) // ������������ ����� ������ � ��������
  {                                                // ������ � ����������� �� ������ ����������
    for (CurPtr=ListData, I=0; I < FileCount; I++, CurPtr++)
    {
      int Match=FALSE;
      if (Mode==SELECT_INVERT || Mode==SELECT_INVERTALL)
        Match=TRUE;
      else
      {
        if (bUseFilter)
          Match=Filter.FileInFilter(CurPtr);
        else
          Match=FileMask.Compare((ShowShortNames && *CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name));
      }

      if (Match)
      {
        switch(Mode)
        {
          case SELECT_ADD:
            Selection=1;
            break;
          case SELECT_REMOVE:
            Selection=0;
            break;
          case SELECT_INVERT:
          case SELECT_INVERTALL:
            Selection=!CurPtr->Selected;
            break;
        }
        if (bUseFilter || (CurPtr->FileAttr & FA_DIREC)==0 || Opt.SelectFolders ||
            Selection==0 || RawSelection || Mode==SELECT_INVERTALL)
          Select(CurPtr,Selection);
      }
    }
  }
  if (SelectedFirst)
    SortFileList(TRUE);
  ShowFileList(TRUE);
}

void FileList::UpdateViewPanel()
{
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  if (FileCount>0 && AnotherPanel->IsVisible() &&
      AnotherPanel->GetType()==QVIEW_PANEL && SetCurPath())
  {
    QuickView *ViewPanel=(QuickView *)AnotherPanel;
    struct FileListItem *CurPtr=ListData+CurFile;
    if (PanelMode!=PLUGIN_PANEL ||
        CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILE))
    {
      if (TestParentFolderName(CurPtr->Name))
        ViewPanel->ShowFile(CurDir,FALSE,NULL);
      else
        ViewPanel->ShowFile((GetFileAttributes(CurPtr->Name) == (DWORD)-1 && GetFileAttributes(CurPtr->ShortName) != (DWORD)-1)?CurPtr->ShortName:CurPtr->Name,FALSE,NULL);
    }
    else
      if ((CurPtr->FileAttr & FA_DIREC)==0)
      {
        char TempDir[NM],FileName[NM];
        strcpy(FileName,CurPtr->Name);
        if(!FarMkTempEx(TempDir))
          return;
        CreateDirectory(TempDir,NULL);
        struct PluginPanelItem PanelItem;
        FileListToPluginItem(CurPtr,&PanelItem);
        if (!CtrlObject->Plugins.GetFile(hPlugin,&PanelItem,TempDir,FileName,OPM_SILENT|OPM_VIEW|OPM_QUICKVIEW))
        {
          ViewPanel->ShowFile(NULL,FALSE,NULL);
          FAR_RemoveDirectory(TempDir);
          return;
        }
        ViewPanel->ShowFile(FileName,TRUE,NULL);
      }
      else
        if (!TestParentFolderName(CurPtr->Name))
          ViewPanel->ShowFile(CurPtr->Name,FALSE,hPlugin);
        else
          ViewPanel->ShowFile(NULL,FALSE,NULL);

    SetTitle();
  }
}


void FileList::CompareDir()
{
  FileList *Another=(FileList *)CtrlObject->Cp()->GetAnotherPanel(this);
  int I,J;
  if (Another->GetType()!=FILE_PANEL || !Another->IsVisible())
  {
    Message(MSG_WARNING,1,MSG(MCompareTitle),MSG(MCompareFilePanelsRequired1),
            MSG(MCompareFilePanelsRequired2),MSG(MOk));
    return;
  }

  ScrBuf.Flush();

  // ��������� ������� ��������� � ����� �������
  ClearSelection();
  Another->ClearSelection();

  struct FileListItem *CurPtr, *AnotherCurPtr;
  char TempName1[NM*2],*PtrTempName1,TempName2[NM*2],*PtrTempName2;
  BOOL OpifRealnames1=FALSE, OpifRealnames2=FALSE;

  // �������� ���, ����� ��������� �� �������� ������
  for (CurPtr=ListData, I=0; I < FileCount; I++, CurPtr++)
    if((CurPtr->FileAttr & FA_DIREC)==0)
      Select(CurPtr,TRUE);

  // �������� ���, ����� ��������� �� ��������� ������
  for (AnotherCurPtr=Another->ListData,J=0; J < Another->FileCount; J++, AnotherCurPtr++)
    if((AnotherCurPtr->FileAttr & FA_DIREC)==0)
      Another->Select(AnotherCurPtr,TRUE);

  int CompareFatTime=FALSE;
  if (PanelMode==PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
    if (Info.Flags & OPIF_COMPAREFATTIME)
      CompareFatTime=TRUE;
    OpifRealnames1=Info.Flags & OPIF_REALNAMES;
  }
  if (Another->PanelMode==PLUGIN_PANEL && !CompareFatTime)
  {
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(Another->hPlugin,&Info);
    if (Info.Flags & OPIF_COMPAREFATTIME)
      CompareFatTime=TRUE;
    OpifRealnames2=Info.Flags & OPIF_REALNAMES;
  }

  if (PanelMode==NORMAL_PANEL && Another->PanelMode==NORMAL_PANEL)
  {
    char FileSystemName1[NM],FileSystemName2[NM];
    GetPathRoot(CurDir,TempName1);
    GetPathRoot(Another->CurDir,TempName2);
    if (GetVolumeInformation(TempName1,NULL,0,NULL,NULL,NULL,FileSystemName1,sizeof(FileSystemName1)) &&
        GetVolumeInformation(TempName2,NULL,0,NULL,NULL,NULL,FileSystemName2,sizeof(FileSystemName2)))
      if (LocalStricmp(FileSystemName1,FileSystemName2)!=0)
        CompareFatTime=TRUE;
  }

  // ������ ������ ���� �� ������ ���������
  // ������ ������� �������� ������...
  for (CurPtr=ListData, I=0; I < FileCount; I++, CurPtr++)
  {
    // ...���������� � ��������� ��������� ������...
    for (AnotherCurPtr=Another->ListData,J=0; J < Another->FileCount; J++, AnotherCurPtr++)
    {
      int Cmp=0;
#if 0
      PtrTempName1=CurPtr->Name;
      PtrTempName2=AnotherCurPtr->Name;

      int fp1=strpbrk(CurPtr->Name,":\\/")!=NULL;
      int fp2=strpbrk(AnotherCurPtr->Name,":\\/")!=NULL;

      if(fp1 && !fp2 && strcmp(PtrTempName2,".."))
      {
        strcpy(TempName2,Another->CurDir);
        AddEndSlash(TempName2);
        strncat(TempName2,AnotherCurPtr->Name,sizeof(TempName2)-1);
        PtrTempName2=TempName2;
      }
      else if(!fp1 && fp2 && strcmp(PtrTempName1,".."))
      {
        strcpy(TempName1,CurDir);
        AddEndSlash(TempName1);
        strncat(TempName1,CurPtr->Name,sizeof(TempName1)-1);
        PtrTempName1=TempName1;
      }

      if(OpifRealnames1 || OpifRealnames2)
      {
        PtrTempName1=PointToName(CurPtr->Name);
        PtrTempName2=PointToName(AnotherCurPtr->Name);
      }
#else
      PtrTempName1=PointToName(CurPtr->Name);
      PtrTempName2=PointToName(AnotherCurPtr->Name);
#endif

      if (LocalStricmp(PtrTempName1,PtrTempName2)==0)
      //if (LocalStricmp(CurPtr->Name,AnotherCurPtr->Name)==0)
      {
        if (CompareFatTime)
        {
          WORD DosDate,DosTime,AnotherDosDate,AnotherDosTime;
          FileTimeToDosDateTime(&CurPtr->WriteTime,&DosDate,&DosTime);
          FileTimeToDosDateTime(&AnotherCurPtr->WriteTime,&AnotherDosDate,&AnotherDosTime);
          DWORD FullDosTime,AnotherFullDosTime;
          FullDosTime=((DWORD)DosDate<<16)+DosTime;
          AnotherFullDosTime=((DWORD)AnotherDosDate<<16)+AnotherDosTime;
          int D=FullDosTime-AnotherFullDosTime;
          if (D>=-1 && D<=1)
            Cmp=0;
          else
            Cmp=(FullDosTime<AnotherFullDosTime) ? -1:1;
        }
        else
        {
          __int64 RetCompare=*(__int64*)&CurPtr->WriteTime - *(__int64*)&AnotherCurPtr->WriteTime;
          Cmp=!RetCompare?0:(RetCompare > 0?1:-1);
        }

        if (Cmp==0 && (CurPtr->UnpSize!=AnotherCurPtr->UnpSize ||
                       CurPtr->UnpSizeHigh!=AnotherCurPtr->UnpSizeHigh))
          continue;

        if (Cmp < 1 && CurPtr->Selected)
          Select(CurPtr,0);

        if (Cmp > -1 && AnotherCurPtr->Selected)
          Another->Select(AnotherCurPtr,0);

        if (Another->PanelMode!=PLUGIN_PANEL)
          break;
      }
    }
  }

  if (SelectedFirst)
    SortFileList(TRUE);

  // ���������� ����� ������ (Mantis#225)
  if (Another->SelectedFirst)
    Another->SortFileList(TRUE);

  Redraw();
  Another->Redraw();
  if (SelFileCount==0 && Another->SelFileCount==0)
    Message(0,1,MSG(MCompareTitle),MSG(MCompareSameFolders1),MSG(MCompareSameFolders2),MSG(MOk));
}

void FileList::CopyNames(int FillPathName,int UNC)
{
  struct OpenPluginInfo Info;
  char *CopyData=NULL;
  long DataSize=0;
  char SelName[NM], SelShortName[NM], QuotedName[4096];
  int FileAttr;

  if (PanelMode==PLUGIN_PANEL)
  {
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
  }

  GetSelName(NULL,FileAttr);
  while (GetSelName(SelName,FileAttr,SelShortName))
  {
    if (DataSize>0)
    {
      strcat(CopyData+DataSize,"\r\n");
      DataSize+=2;
    }
    strcpy(QuotedName,ShowShortNames && *SelShortName ? SelShortName:SelName);
    if(FillPathName)
    {

      if (PanelMode!=PLUGIN_PANEL || (Info.Flags & OPIF_REALNAMES))
      {
        /* $ 14.02.2002 IS
           ".." � ������� �������� ���������� ��� ��� �������� ��������
        */
        if(TestParentFolderName(QuotedName) && TestParentFolderName(SelShortName))
        {
          QuotedName[1]=SelShortName[1]=0;
        }
        /* IS $ */
        if(!CreateFullPathName(QuotedName,SelShortName,FileAttr,QuotedName,sizeof(QuotedName)-1,UNC))
        {
          xf_free(CopyData);
          CopyData=NULL;
          break;
        }
      }
      else
      {
        char FullName[NM];
        strcpy(FullName,NullToEmpty(Info.CurDir));
        if (Opt.PanelCtrlFRule && ViewSettings.FolderUpperCase)
          LocalStrupr(FullName);

        if (*FullName)
          AddEndSlash(FullName);

        if(Opt.PanelCtrlFRule)
        {
          // ��� ������ �������� �������� �� ������
          if (ViewSettings.FileLowerCase && !(FileAttr & FA_DIREC))
            LocalStrlwr(QuotedName);
          if (ViewSettings.FileUpperToLowerCase)
            if (!(FileAttr & FA_DIREC) && !IsCaseMixed(QuotedName))
               LocalStrlwr(QuotedName);
        }
        strcat(FullName,QuotedName);
        strcpy(QuotedName,FullName);
        // ������� ������ �������!
        if(PanelMode==PLUGIN_PANEL && Opt.SubstPluginPrefix)
        {
          char Prefix[NM*2];
          /* $ 19.11.2001 IS ����������� �� �������� :) */
          if(*AddPluginPrefix((FileList *)CtrlObject->Cp()->ActivePanel,Prefix))
          {
            strcat(Prefix,QuotedName);
            xstrncpy(QuotedName,Prefix,sizeof(QuotedName)-1);
          }
          /* IS $ */
        }
      }
    }
    if(Opt.QuotedName&QUOTEDNAME_CLIPBOARD)
      QuoteSpace(QuotedName);
    int Length=(int)strlen(QuotedName);
    char *NewPtr=(char *)xf_realloc(CopyData,DataSize+Length+3);
    if (NewPtr==NULL)
    {
      xf_free(CopyData);
      CopyData=NULL;
      break;
    }
    CopyData=NewPtr;
    CopyData[DataSize]=0;
    strcat(CopyData+DataSize,QuotedName);
    DataSize+=Length;
  }

  CopyToClipboard(CopyData);
  xf_free(CopyData);
}

char *FileList::CreateFullPathName(char *Name, char *ShortName,DWORD FileAttr,
                                   char *Dest,int SizeDest,int UNC,int ShortNameAsIs)
{
  char Temp[4906], FileName[4906];
  char *NamePtr, Chr=0;
  /* $ 02.04.2001 IS
   ��������� ���:
   -----
   1) � Temporary panel Ctrl+F �� .. ������: C:\dr_dr_dr\ (���
      "C:\dr_dr_dr\" ���� �� ������������ � Temporary panel)

   2) � Temporary panel -> Ctrl+N (�������� �������� �����) -> Ctrl+F ��
      ����� ����� ��������: C:\dr_dr_dr\FILENAME.EXT (��� "C:\dr_dr_dr\"
      �� ���� �� �����, � ��.1)
       -----
   ����� 1 ����������� �����, ����� 2 ������������ ����.
   ������� �����������:
   1. ���� ��� �������� '\\', �� ��� �������� ����
   2. ���� �������� ��� �������� ����, �� ������� ��� �����
      �������� ����.
   3. ���� ��� �������� ����, �� �������� ConvertNameToFull ��
      �����.
  */
  xstrncpy(FileName,Dest,sizeof(FileName)-1);
  char *ShortNameLastSlash=strrchr(ShortName, '\\'),
       *NameLastSlash=strrchr(Name, '\\');
  if (NULL==ShortNameLastSlash && NULL==NameLastSlash)
  {
    if(ConvertNameToFull(FileName,FileName, sizeof(FileName)) >= sizeof(FileName))
    {
      return NULL;
    }
  }
  else if(ShowShortNames)
  {
    strcpy(Temp, Name);
    if(NameLastSlash)
      Temp[1+NameLastSlash-Name]=0;

    if((NamePtr=strrchr(FileName, '\\')) != NULL)
      NamePtr++;
    else
      NamePtr=FileName;

    strcat(Temp, NameLastSlash?NameLastSlash+1:Name);
    strcpy(FileName, Temp);
  }
  /* IS $ */
  if (ShowShortNames && ShortNameAsIs)
    ConvertNameToShort(FileName,FileName,sizeof(FileName)-1);

  /* $ 29.01.2001 VVM
    + �� CTRL+ALT+F � ��������� ������ ������������ UNC-��� �������� �����. */
  if (UNC)
  {
    // ��������� �� ��� �������� �������
    char FileSystemName[NM];
    GetPathRoot(FileName,Temp);
    if(!GetVolumeInformation(Temp,NULL,0,NULL,NULL,NULL,FileSystemName,sizeof(FileSystemName)))
      *FileSystemName=0;

    DWORD uniSize = sizeof(Temp);
    // ��������� WNetGetUniversalName ��� ���� ������, ������ �� ��� Novell`�
    if (stricmp(FileSystemName,"NWFS") != 0 &&
        WNetGetUniversalName(FileName, UNIVERSAL_NAME_INFO_LEVEL,&Temp, &uniSize) == NOERROR)
    {
      UNIVERSAL_NAME_INFO *lpuni = (UNIVERSAL_NAME_INFO *)&Temp;
      xstrncpy(FileName, lpuni->lpUniversalName, sizeof(FileName)-1);
    }
    else if(FileName[1] == ':')
    {
      // BugZ#449 - �������� ������ CtrlAltF � ��������� Novell DS
      // �����, ���� �� ���������� �������� UniversalName � ���� ���
      // ��������� ���� - �������� ��� ��� ���� ������ ������
      if(*DriveLocalToRemoteName(DRIVE_UNKNOWN,*FileName,Temp) != 0)
      {
        if((NamePtr=strchr(FileName, '/')) == NULL)
          NamePtr=strchr(FileName, '\\');
        if(NamePtr != NULL)
        {
          AddEndSlash(Temp);
          strcat(Temp,++NamePtr);
        }
        xstrncpy(FileName, Temp, sizeof(FileName)-1);
      }
    }

    ConvertNameToReal(FileName,FileName, sizeof(FileName));
  } /* if */
  /* VVM $ */
  // $ 20.10.2000 SVS ������� ���� Ctrl-F ������������!
  if(Opt.PanelCtrlFRule)
  {
    /* $ 13.10.2000 tran
      �� Ctrl-f ��� ������ �������� �������� �� ������ */
    if (ViewSettings.FolderUpperCase)
    {
      if ( FileAttr & FA_DIREC )
        LocalStrupr(FileName);
      else
      {
          if((NamePtr=strrchr(FileName,'\\')) != NULL)
          {
            Chr=*NamePtr;
            *NamePtr=0;
          }
          LocalStrupr(FileName);
          if(NamePtr)
            *NamePtr=Chr;
      }
    }
    if (ViewSettings.FileUpperToLowerCase)
      if (!(FileAttr & FA_DIREC) && strrchr(FileName,'\\') && !IsCaseMixed(strrchr(FileName,'\\')))
         LocalStrlwr(strrchr(FileName,'\\'));
    if ( ViewSettings.FileLowerCase && strrchr(FileName,'\\') && !(FileAttr & FA_DIREC))
      LocalStrlwr(strrchr(FileName,'\\'));
  }

  return xstrncpy(Dest,FileName,SizeDest);
}

void FileList::SetTitle()
{
  if (GetFocus() || CtrlObject->Cp()->GetAnotherPanel(this)->GetType()!=FILE_PANEL)
  {
    char TitleDir[NM*2];
    if (PanelMode==PLUGIN_PANEL)
    {
      struct OpenPluginInfo Info;
      /* $ 21.03.2002 DJ
         �� ����� ������� ����
      */
      char Title[240];
      CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
      xstrncpy(Title,NullToEmpty(Info.PanelTitle), sizeof (Title)-1);
      /* DJ $ */
      RemoveLeadingSpaces(Title);
      RemoveTrailingSpaces(Title);
      sprintf(TitleDir,"{%.*s}",sizeof(TitleDir)-3,Title);
    }
    else
      sprintf(TitleDir,"{%.*s}",sizeof(TitleDir)-3,CurDir);
    xstrncpy(LastFarTitle,TitleDir,sizeof(LastFarTitle)-1);
    SetFarTitle(TitleDir);
  }
}


void FileList::ClearSelection()
{
  struct FileListItem *CurPtr=ListData;
  for (int I=0; I < FileCount; I++, CurPtr++)
    Select(CurPtr,0);

  if (SelectedFirst)
    SortFileList(TRUE);
}


void FileList::SaveSelection()
{
  struct FileListItem *CurPtr=ListData;
  for (int I=0; I < FileCount; I++, CurPtr++)
    CurPtr->PrevSelected=CurPtr->Selected;
}


void FileList::RestoreSelection()
{
  struct FileListItem *CurPtr=ListData;
  for (int I=0; I < FileCount; I++, CurPtr++)
  {
    int NewSelection=CurPtr->PrevSelected;
    CurPtr->PrevSelected=CurPtr->Selected;
    Select(CurPtr,NewSelection);
  }
  if (SelectedFirst)
    SortFileList(TRUE);
  Redraw();
}


int FileList::GetFileName(char *Name,int Pos,int &FileAttr)
{
  if (Pos>=FileCount)
    return(FALSE);
  if(Name)
    strcpy(Name,ListData[Pos].Name);
  FileAttr=ListData[Pos].FileAttr;
  return(TRUE);
}


int FileList::GetCurrentPos()
{
  return(CurFile);
}


void FileList::EditFilter()
{
  if (Filter==NULL)
    Filter=new FileFilter(this,FFT_PANEL);
  Filter->FilterEdit();
}


void FileList::SelectSortMode()
{
  struct MenuData SortMenu[]=
  {
   /* 00 */(char *)MMenuSortByName,LIF_SELECTED,KEY_CTRLF3,
   /* 01 */(char *)MMenuSortByExt,0,KEY_CTRLF4,
   /* 02 */(char *)MMenuSortByModification,0,KEY_CTRLF5,
   /* 03 */(char *)MMenuSortBySize,0,KEY_CTRLF6,
   /* 04 */(char *)MMenuUnsorted,0,KEY_CTRLF7,
   /* 05 */(char *)MMenuSortByCreation,0,KEY_CTRLF8,
   /* 06 */(char *)MMenuSortByAccess,0,KEY_CTRLF9,
   /* 07 */(char *)MMenuSortByDiz,0,KEY_CTRLF10,
   /* 08 */(char *)MMenuSortByOwner,0,KEY_CTRLF11,
   /* 09 */(char *)MMenuSortByCompressedSize,0,0,
   /* 10 */(char *)MMenuSortByNumLinks,0,0,
   /* 11 */"",LIF_SEPARATOR,0,
   /* 12 */(char *)MMenuSortUseNumeric,0,0,
   /* 13 */(char *)MMenuSortUseGroups,0,KEY_SHIFTF11,
   /* 14 */(char *)MMenuSortSelectedFirst,0,KEY_SHIFTF12,
  };

  static int SortModes[]={BY_NAME,   BY_EXT,    BY_MTIME,
                          BY_SIZE,   UNSORTED,  BY_CTIME,
                          BY_ATIME,  BY_DIZ,    BY_OWNER,
                          BY_COMPRESSEDSIZE,BY_NUMLINKS};

  for (int I=0;I<sizeof(SortModes)/sizeof(SortModes[0]);I++)
    if (SortMode==SortModes[I])
    {
      SortMenu[I].SetCheck(SortOrder==1 ? '+':'-');
      break;
    }

  int SG=GetSortGroups();
  SortMenu[12].SetCheck(NumericSort);
  SortMenu[13].SetCheck(SG);
  SortMenu[14].SetCheck(SelectedFirst);

  int SortCode;
  {
    VMenu SortModeMenu(MSG(MMenuSortTitle),SortMenu,sizeof(SortMenu)/sizeof(SortMenu[0]),0);
    SortModeMenu.SetHelp("PanelCmdSort");
    /* $ 16.06.2001 KM
       ! ���������� WRAPMODE � ����.
    */
    SortModeMenu.SetPosition(X1+4,-1,0,0);
    /* KM $ */
    SortModeMenu.SetFlags(VMENU_WRAPMODE);
    SortModeMenu.Process();
    if ((SortCode=SortModeMenu.Modal::GetExitCode())<0)
      return;
  }
  if (SortCode<sizeof(SortModes)/sizeof(SortModes[0]))
    SetSortMode(SortModes[SortCode]);
  else
    switch(SortCode)
    {
      case 13:
        ProcessKey(KEY_SHIFTF11);
        break;
      case 14:
        ProcessKey(KEY_SHIFTF12);
        break;
      case 12:
        NumericSort=NumericSort?0:1;
        Update(UPDATE_KEEP_SELECTION);
        Redraw();
        Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
        AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
        AnotherPanel->Redraw();
        break;
    }
}


void FileList::DeleteDiz(char *Name,char *ShortName)
{
  if (PanelMode==NORMAL_PANEL)
    Diz.DeleteDiz(Name,ShortName);
}


void FileList::FlushDiz()
{
  if (PanelMode==NORMAL_PANEL)
    Diz.Flush(CurDir);

}


void FileList::GetDizName(char *DizName)
{
  if (PanelMode==NORMAL_PANEL)
    Diz.GetDizName(DizName);
}


void FileList::CopyDiz(char *Name,char *ShortName,char *DestName,
                       char *DestShortName,DizList *DestDiz)
{
  Diz.CopyDiz(Name,ShortName,DestName,DestShortName,DestDiz);
}


void FileList::DescribeFiles()
{
  char SelName[NM],SelShortName[NM];
  int FileAttr,DizCount=0;

  ReadDiz();

  SaveSelection();
  GetSelName(NULL,FileAttr);
  while (GetSelName(SelName,FileAttr,SelShortName))
  {
    char DizText[1024],Msg[300],TruncMsg[100],QuotedName[NM],*PrevText;
    PrevText=Diz.GetDizTextAddr(SelName,SelShortName,GetLastSelectedSize(NULL));
    strcpy(QuotedName,SelName);
    QuoteSpaceOnly(QuotedName);
    sprintf(Msg,MSG(MEnterDescription),QuotedName);
    sprintf(TruncMsg,"%.65s",Msg);
    /* $ 09.08.2000 SVS
       ��� Ctrl-Z ������� ����� ���������� ��������!
    */
    if (!GetString(MSG(MDescribeFiles),TruncMsg,"DizText",
                   PrevText!=NULL ? PrevText:"",DizText,sizeof(DizText),
                   "FileDiz",FIB_ENABLEEMPTY|(!DizCount?FIB_NOUSELASTHISTORY:0)|FIB_BUTTONS))
      break;
    /* SVS $*/
    DizCount++;
    if (*DizText==0)
      Diz.DeleteDiz(SelName,SelShortName);
    else
    {
      char DizLine[NM+1030];
      sprintf(DizLine,"%-*s %s",Opt.Diz.StartPos>1 ? Opt.Diz.StartPos-2:0,QuotedName,DizText);
      Diz.AddDiz(SelName,SelShortName,DizLine);
    }
    ClearLastGetSelection();
    // BugZ#442 - Deselection is late when making file descriptions
    FlushDiz();
    // BugZ#863 - ��� �������������� ������ ������������ ��� �� ����������� �� ����
    Update(UPDATE_KEEP_SELECTION);
    Redraw();
  }
  if (DizCount>0)
  {
    FlushDiz();
    Update(UPDATE_KEEP_SELECTION);
    Redraw();
    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    AnotherPanel->Redraw();
  }
}


void FileList::SetReturnCurrentFile(int Mode)
{
  ReturnCurrentFile=Mode;
}


void FileList::ApplyCommand()
{
  static char PrevCommand[512];
  char Command[512];

  if (!GetString(MSG(MAskApplyCommandTitle),MSG(MAskApplyCommand),"ApplyCmd",PrevCommand,Command,sizeof(Command),"ApplyCmd",FIB_BUTTONS))
    return;

  strcpy(PrevCommand,Command);
  char SelName[NM],SelShortName[NM];
  int FileAttr;
  int RdrwDskt=CtrlObject->MainKeyBar->IsVisible();

  RedrawDesktop Redraw(TRUE);
  SaveSelection();

  GetSelName(NULL,FileAttr);
  while (GetSelName(SelName,FileAttr,SelShortName) && !CheckForEsc())
  {
    char ConvertedCommand[512];
    char ListName[NM*2],ShortListName[NM*2];
    strcpy(ConvertedCommand,Command);

    {
      int PreserveLFN=SubstFileName(ConvertedCommand,sizeof (ConvertedCommand),SelName,SelShortName,ListName,ShortListName);
      PreserveLongName PreserveName(SelShortName,PreserveLFN);
      Execute(ConvertedCommand,FALSE,FALSE);
      ClearLastGetSelection();
    }
  }
  /*$ 23.07.2001 SKV
    ��� �� �� �������� ��������� ������ ������.
  */
  if(RdrwDskt)
  {
    ScrBuf.Scroll(1);
    ScrBuf.Flush();
  }
  /* SKV$*/
}


void FileList::CountDirSize(DWORD PluginFlags)
{
  unsigned long DirCount,DirFileCount,ClusterSize;;
  unsigned __int64 FileSize,CompressedFileSize,RealFileSize;
  unsigned long SelDirCount=0;
  struct FileListItem *CurPtr;
  int I;

  /* $ 09.11.2000 OT
    F3 �� ".." � ��������
  */
  if ( PanelMode==PLUGIN_PANEL && !CurFile && TestParentFolderName(ListData->Name))
  {
    struct FileListItem *DoubleDotDir = NULL;
    if (SelFileCount)
    {
      DoubleDotDir = ListData;
      for (CurPtr=ListData, I=0; I < FileCount; I++, CurPtr++)
      {
        if (CurPtr->Selected && (CurPtr->FileAttr & FA_DIREC))
        {
          DoubleDotDir = NULL;
          break;
        }
      }
    }
    else
    {
      DoubleDotDir = ListData;
    }

    if (DoubleDotDir)
    {
      DoubleDotDir->ShowFolderSize=1;
      unsigned __int64 UnpSize=0, PackSize=0;

      for (I=1, CurPtr=ListData+I; I < FileCount; I++, CurPtr++)
      {
        if (CurPtr->FileAttr & FA_DIREC)
        {
          if (GetPluginDirInfo(hPlugin,CurPtr->Name,DirCount,DirFileCount,FileSize,CompressedFileSize))
          {
            UnpSize  += FileSize;
            PackSize += CompressedFileSize;
          }
        }
        else
        {
          UnpSize  += MKUINT64(CurPtr->UnpSizeHigh,CurPtr->UnpSize);
          PackSize += MKUINT64(CurPtr->PackSizeHigh,CurPtr->PackSize);
        }
      }

      DoubleDotDir->UnpSize     = (DWORD)(UnpSize&_i64(0xFFFFFFFF));
      DoubleDotDir->UnpSizeHigh = (DWORD)(UnpSize>>32);
      DoubleDotDir->PackSize    = (DWORD)(PackSize&_i64(0xFFFFFFFF));
      DoubleDotDir->PackSizeHigh= (DWORD)(PackSize>>32);
    }
  }
  /* OT $*/

  for (CurPtr=ListData, I=0; I < FileCount; I++, CurPtr++)
  {
    if (CurPtr->Selected && (CurPtr->FileAttr & FA_DIREC))
    {
      SelDirCount++;
      if (PanelMode==PLUGIN_PANEL && !(PluginFlags & OPIF_REALNAMES) &&
          GetPluginDirInfo(hPlugin,CurPtr->Name,DirCount,DirFileCount,FileSize,CompressedFileSize)
        ||
          (PanelMode!=PLUGIN_PANEL || (PluginFlags & OPIF_REALNAMES)) &&
          GetDirInfo(MSG(MDirInfoViewTitle),
                     (Opt.FolderDeepScan && GetFileAttributes(CurPtr->Name)==(DWORD)-1 && *CurPtr->ShortName?CurPtr->ShortName:CurPtr->Name),
                     DirCount,DirFileCount,FileSize,
                     CompressedFileSize,RealFileSize, ClusterSize,0,
                     Filter,
                     GETDIRINFO_DONTREDRAWFRAME|GETDIRINFO_USEDALTFOLDERNAME|GETDIRINFO_SCANSYMLINKDEF)==1)
      {
        SelFileSize-=MKUINT64(CurPtr->UnpSizeHigh,CurPtr->UnpSize);
        SelFileSize+=FileSize;

        CurPtr->UnpSize=(DWORD)(FileSize&_i64(0xFFFFFFFF));
        CurPtr->UnpSizeHigh=(DWORD)(FileSize>>32);
        CurPtr->PackSize=(DWORD)(CompressedFileSize&_i64(0xFFFFFFFF));
        CurPtr->PackSizeHigh=(DWORD)(CompressedFileSize>>32);

        CurPtr->ShowFolderSize=1;
      }
      else
        break;
    }
  }

  CurPtr=ListData+CurFile;

  if (SelDirCount==0)
  {
    if (PanelMode==PLUGIN_PANEL && !(PluginFlags & OPIF_REALNAMES) &&
        GetPluginDirInfo(hPlugin,CurPtr->Name,DirCount,DirFileCount,FileSize,CompressedFileSize)
      ||
        (PanelMode!=PLUGIN_PANEL || (PluginFlags & OPIF_REALNAMES)) &&
        GetDirInfo(MSG(MDirInfoViewTitle),
                   TestParentFolderName(CurPtr->Name) ? ".":
                     (Opt.FolderDeepScan && GetFileAttributes(CurPtr->Name)==(DWORD)-1 && *CurPtr->ShortName?CurPtr->ShortName:CurPtr->Name),
                   DirCount,
                   DirFileCount,FileSize,CompressedFileSize,RealFileSize,ClusterSize,0,
                   Filter,
                   GETDIRINFO_DONTREDRAWFRAME|GETDIRINFO_USEDALTFOLDERNAME|GETDIRINFO_SCANSYMLINKDEF)==1)
    {
      CurPtr->UnpSize=(DWORD)(FileSize&_i64(0xFFFFFFFF));
      CurPtr->UnpSizeHigh=(DWORD)(FileSize>>32);
      CurPtr->PackSize=(DWORD)(CompressedFileSize&_i64(0xFFFFFFFF));
      CurPtr->PackSizeHigh=(DWORD)(CompressedFileSize>>32);
      CurPtr->ShowFolderSize=1;
    }
  }

  SortFileList(TRUE);
  ShowFileList(TRUE);
  CtrlObject->Cp()->Redraw();
  CreateChangeNotification(TRUE);
}


int FileList::GetPrevViewMode()
{
  if (PanelMode==PLUGIN_PANEL && PluginsStackSize>0)
    return(PluginsStack[0].PrevViewMode);
  else
    return(ViewMode);
}


int FileList::GetPrevSortMode()
{
  if (PanelMode==PLUGIN_PANEL && PluginsStackSize>0)
    return(PluginsStack[0].PrevSortMode);
  else
    return(SortMode);
}


int FileList::GetPrevSortOrder()
{
  if (PanelMode==PLUGIN_PANEL && PluginsStackSize>0)
    return(PluginsStack[0].PrevSortOrder);
  else
    return(SortOrder);
}

int FileList::GetPrevNumericSort()
{
  if (PanelMode==PLUGIN_PANEL && PluginsStackSize>0)
    return(PluginsStack[0].PrevNumericSort);
  else
    return(NumericSort);
}


HANDLE FileList::OpenFilePlugin(char *FileName,int PushPrev)
{
  if (!PushPrev && PanelMode==PLUGIN_PANEL)
    while (1)
    {
      if (ProcessPluginEvent(FE_CLOSE,NULL))
        return((HANDLE)-2);
      if (!PopPlugin(TRUE))
        break;
    }
  HANDLE hNewPlugin=OpenPluginForFile(FileName);
  if (hNewPlugin!=INVALID_HANDLE_VALUE && hNewPlugin!=(HANDLE)-2)
  {
    if (PushPrev)
    {
      PrevDataStack=(struct PrevDataItem *)xf_realloc(PrevDataStack,(PrevDataStackSize+1)*sizeof(*PrevDataStack));
      PrevDataStack[PrevDataStackSize].PrevListData=ListData;
      PrevDataStack[PrevDataStackSize].PrevFileCount=FileCount;
      PrevDataStack[PrevDataStackSize].PrevTopFile = CurTopFile;
      strncpy(PrevDataStack[PrevDataStackSize].PrevName,FileName,sizeof(PrevDataStack[PrevDataStackSize].PrevName));
      PrevDataStackSize++;
      ListData=NULL;
      FileCount=0;
    }

    BOOL WasFullscreen = IsFullScreen();

    SetPluginMode(hNewPlugin,FileName);
    PanelMode=PLUGIN_PANEL;
    UpperFolderTopFile=CurTopFile;
    CurFile=0;
    Update(0);
    Redraw();
    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
    if ( (AnotherPanel->GetType()==INFO_PANEL) || WasFullscreen )
      AnotherPanel->Redraw();
  }
  return(hNewPlugin);
}


void FileList::ProcessCopyKeys(int Key)
{
  if (FileCount>0)
  {
    int Drag=Key==KEY_DRAGCOPY || Key==KEY_DRAGMOVE;
    int Ask=!Drag || Opt.Confirm.Drag;
    int Move=(Key==KEY_F6 || Key==KEY_DRAGMOVE);
    int AnotherDir=FALSE;
    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
    if (AnotherPanel->GetType()==FILE_PANEL)
    {
      FileList *AnotherFilePanel=(FileList *)AnotherPanel;
      if (AnotherFilePanel->FileCount>0 &&
          (AnotherFilePanel->ListData[AnotherFilePanel->CurFile].FileAttr & FA_DIREC) &&
          !TestParentFolderName(AnotherFilePanel->ListData[AnotherFilePanel->CurFile].Name))
      {
        AnotherDir=TRUE;
        if (Drag)
        {
          AnotherPanel->ProcessKey(KEY_ENTER);
          SetCurPath();
        }
      }
    }
    if (PanelMode==PLUGIN_PANEL && !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILES))
    {
      if (Key!=KEY_ALTF6)
      {
        char PluginDestPath[NM*2];
        int ToPlugin=FALSE;
        *PluginDestPath=0;
        if (AnotherPanel->GetMode()==PLUGIN_PANEL && AnotherPanel->IsVisible() &&
            !CtrlObject->Plugins.UseFarCommand(AnotherPanel->GetPluginHandle(),PLUGIN_FARPUTFILES))
        {
          ToPlugin=2;
          ShellCopy ShCopy(this,Move,FALSE,FALSE,Ask,ToPlugin,PluginDestPath);
        }
        if (ToPlugin!=-1)
          if (ToPlugin)
            PluginToPluginFiles(Move);
          else
          {
            char DestPath[NM];
            if (*PluginDestPath)
              strcpy(DestPath,PluginDestPath);
            else
            {
              AnotherPanel->GetCurDir(DestPath);
              if(!AnotherPanel->IsVisible())
              {
                struct OpenPluginInfo Info;
                CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
                if (Info.HostFile!=NULL && *Info.HostFile!=0)
                {
                  char *ExtPtr;
                  xstrncpy(DestPath,PointToName(Info.HostFile),sizeof(DestPath));
                  if ((ExtPtr=strrchr(DestPath,'.'))!=NULL)
                    *ExtPtr=0;
                }
              }
            }

            PluginGetFiles(DestPath,Move);
          }
      }
    }
    else
    {
      int ToPlugin=AnotherPanel->GetMode()==PLUGIN_PANEL &&
                    AnotherPanel->IsVisible() && Key!=KEY_ALTF6 &&
                    !CtrlObject->Plugins.UseFarCommand(AnotherPanel->GetPluginHandle(),PLUGIN_FARPUTFILES);

      if(Key != KEY_ALTF6 ||
        (Key == KEY_ALTF6 && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT))
      {
         ShellCopy ShCopy(this,Move,Key==KEY_ALTF6,FALSE,Ask,ToPlugin,NULL);
      }

      if (ToPlugin==1)
        PluginPutFilesToAnother(Move,AnotherPanel);
    }
    if (AnotherDir && Drag)
      AnotherPanel->ProcessKey(KEY_ENTER);
  }
}

/* $ 09.02.2001 IS
   ����������/�������� ����� "���������� ������"
*/
void FileList::SetSelectedFirstMode(int Mode)
{
  SelectedFirst=Mode;
  SortFileList(TRUE);
}
/* IS $ */

void FileList::ChangeSortOrder(int NewOrder)
{
  Panel::ChangeSortOrder(NewOrder);
  SortFileList(TRUE);
  Show();
}

BOOL FileList::UpdateKeyBar()
{
  KeyBar *KB=CtrlObject->MainKeyBar;

  KB->ReadRegGroup("Shell",Opt.Language);

  KB->SetAllGroup (KBL_MAIN, MF1, 12);
  KB->SetAllGroup (KBL_SHIFT, MShiftF1, 12);
  KB->SetAllGroup (KBL_ALT, MAltF1, 12);
  KB->SetAllGroup (KBL_CTRL, MCtrlF1, 12);
  KB->SetAllGroup (KBL_CTRLSHIFT, MCtrlShiftF1, 12);
  KB->SetAllGroup (KBL_CTRLALT, MCtrlAltF1, 12);
  KB->SetAllGroup (KBL_ALTSHIFT, MAltShiftF1, 12);

  if(WinVer.dwPlatformId != VER_PLATFORM_WIN32_NT)
    KB->Change(KBL_ALT,"",6-1);

  KB->SetAllRegGroup();

  if (GetMode() == PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    GetOpenPluginInfo(&Info);
    if (Info.KeyBar)
    {
      KB->Set((const char **)Info.KeyBar->Titles,12);
      KB->SetShift((const char **)Info.KeyBar->ShiftTitles,12);
      KB->SetAlt((const char **)Info.KeyBar->AltTitles,12);
      KB->SetCtrl((const char **)Info.KeyBar->CtrlTitles,12);
      if(Info.StructSize >= sizeof(struct OpenPluginInfo))
      {
        KB->SetCtrlShift((const char **)Info.KeyBar->CtrlShiftTitles,12);
        KB->SetAltShift((const char **)Info.KeyBar->AltShiftTitles,12);
        KB->SetCtrlAlt((const char **)Info.KeyBar->CtrlAltTitles,12);
      }
    }
  }

  return TRUE;
}


int FileList::PluginPanelHelp(HANDLE hPlugin)
{
  char Path[NM],FileName[NM],StartTopic[256],*Slash;
  int PluginNumber=((struct PluginHandle *)hPlugin)->PluginNumber;

  strcpy(Path,CtrlObject->Plugins.PluginsData[PluginNumber].ModuleName);
  if ((Slash=strrchr(Path,'\\'))!=NULL)
    *Slash=0;
  FILE *HelpFile=Language::OpenLangFile(Path,HelpFileMask,Opt.HelpLanguage,FileName);
  if (HelpFile==NULL)
    return(FALSE);
  fclose(HelpFile);
  sprintf(StartTopic,HelpFormatLink,Path,"Contents");
  Help PanelHelp(StartTopic);
  return(TRUE);
}

/* $ 19.11.2001 IS
     ��� �������� ������� � ��������� ������� �������� �������� �� ���������
*/
char* FileList::AddPluginPrefix(FileList *SrcPanel,char *Prefix)
{
  if(!Prefix)return "";
  Prefix[0]=0;
  if(Opt.SubstPluginPrefix && SrcPanel->GetMode()==PLUGIN_PANEL)
  {
    OpenPluginInfo Info;
    PluginHandle *plugin=static_cast<PluginHandle*>(SrcPanel->hPlugin);
    CtrlObject->Plugins.GetOpenPluginInfo(plugin,&Info);
    if(!(Info.Flags & OPIF_REALNAMES))
    {
      PluginInfo PInfo;
      CtrlObject->Plugins.GetPluginInfo(plugin->PluginNumber,&PInfo);
      if(PInfo.CommandPrefix && *PInfo.CommandPrefix)
      {
        strcpy(Prefix,PInfo.CommandPrefix);
        char *Ptr=strchr(Prefix,':');
        if(Ptr) *++Ptr=0; else strcat(Prefix,":");
      }
    }
  }
  return Prefix;
}
/* IS $ */

void FileList::IfGoHome(char Drive)
{
  char TmpCurDir [NM];

  // ������� ��������� ������!!!
  /*
     ������? - ������ - ���� �������� ������� (��� ���������
     �������) - �������� ���� � �����������!
  */
  Panel *Another=CtrlObject->Cp()->GetAnotherPanel (this);
  if (Another->GetMode() != PLUGIN_PANEL)
  {
    Another->GetCurDir (TmpCurDir);
    if (TmpCurDir[0] == Drive && TmpCurDir[1] == ':')
    {
      // ��������� � ������ ����� � far.exe
      if (GetModuleFileName (NULL, TmpCurDir, sizeof (TmpCurDir)-1))
      {
        TmpCurDir [3] = '\0';
        Another->SetCurDir (TmpCurDir, FALSE);
      }
    }
  }

  if (GetMode() != PLUGIN_PANEL)
  {
    GetCurDir (TmpCurDir);
    if (TmpCurDir [0] == Drive && TmpCurDir [1] == ':')
    {
      // ��������� � ������ ����� � far.exe
      if (GetModuleFileName (NULL, TmpCurDir, sizeof (TmpCurDir)-1))
      {
        TmpCurDir [3] = '\0';
        SetCurDir (TmpCurDir, FALSE);
      }
    }
  }
}

BOOL FileList::GetItem(int Index,void *Dest)
{
  if(Index == -1 || Index == -2)
    Index=GetCurrentPos();
  if((DWORD)Index >= (DWORD)FileCount)
    return FALSE;
  memcpy(Dest,ListData+Index,sizeof(struct FileListItem));
  return TRUE;
}
