/*
filelist.cpp

�������� ������ - ��騥 �㭪樨

*/

/* Revision: 1.14 23.10.2000 $ */

/*
Modify:
  23.10.2000 SVS
    ! ��筥��� ��� Ctlr-F (� ����� tran!)
  20.10.2000 SVS
    ! ������� ��� Ctrl-F ��樮���쭮�!
  13.10.2000 tran 1.12
    + �� Ctrl-f ��� �⢥砥� �᫮��� �� ������
  27.09.2000 SVS
    + ����� ⥪�饣�/��࠭��� 䠩��/��
    ! FileList::CallPlugin() ��७�ᥭ � PluginsSet
  20.09.2000 SVS
    + �᫨ � ������� ���� ��䨪�, � Ctrl-[ � ��� � ���
      ����⠢�� ���� ��䨪�.
  18.09.2000 SVS
    + �� �맮�� ।���� �� Shift-F4 ����� 㯮�ॡ����
      ��६���� �।�.
  12.09.2000 SVS
    + ��樮���쭮� ��������� ��� �ࠢ�� ������ ��� �� ���⮩ ������
  11.09.2000 SVS
    - Bug #17: ������ ⠪��� - �᫨ ������� ��������� ����, �
      ����⢨� ��������� ������ ����� ������, ���� �⬥砥� 䠩�.
  09.08.2000 SVS
    ! ��� Ctrl-Z ���㦭� ���� �।��饥 ���祭��!
      �⠢�� ᮮ⢥�����騩 䫠�!
  02.08.2000 IG
    ! Wish.Mix #21 - �� ����⨨ '/' ��� '\' � QuickSerach ���室��
      �� ��४���
  01.08.2000 SVS
    ! ��������� �� �맮�� GetString
  15.07.2000 tran
    ! "...��뢠�� ����ᮢ�� ������� ��⮬� �� ��� viewer,
      editor ����� ��� ����୮ ����⠭�����..."
  13.07.2000 SVS
    ! ������� ���४樨 �� �ᯮ�짮����� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ���������� �������樨 ��� BC & VC
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

extern struct PanelViewSettings ViewSettingsArray[];

static int _cdecl SortList(const void *el1,const void *el2);
int _cdecl SortSearchList(const void *el1,const void *el2);

static int ListSortMode,ListSortOrder,ListSortGroups,ListSelectedFirst;
static int ListPanelMode,ListCaseSensitive;
static HANDLE hSortPlugin;


FileList::FileList()
{
  Type=FILE_PANEL;
  GetCurrentDirectory(sizeof(CurDir),CurDir);
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
  Columns=PreparePanelView(&ViewSettings);
  Height=0;
  PluginsStack=NULL;
  PluginsStackSize=0;
  ShiftSelection=-1;
  DisableOut=0;
  hListChange=NULL;
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
}


FileList::~FileList()
{
  CloseChangeNotification();
  for (int I=0;I<PrevDataStackSize;I++)
    DeleteListData(PrevDataStack[I].PrevListData,PrevDataStack[I].PrevFileCount);
  delete PrevDataStack;
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
  for (int I=0;I<FileCount;I++)
  {
    if (ListData[I].CustomColumnNumber>0 && ListData[I].CustomColumnData!=NULL)
    {
      for (int J=0;J<ListData[I].CustomColumnNumber;J++)
        delete ListData[I].CustomColumnData[J];
      delete ListData[I].CustomColumnData;
    }
    if (ListData[I].UserFlags & PPIF_USERDATA)
      delete (void *)ListData[I].UserData;
    if (ListData[I].DizText && ListData[I].DeleteDiz)
      delete ListData[I].DizText;
  }
  /* $ 13.07.2000 SVS
     �� �� �� ��뢠� ����� ����� �१ new :-)
  */
  free(ListData);
  /* SVS $ */
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
  ShowFileList(TRUE);
}


void FileList::Down(int Count)
{
  CurFile+=Count;
  ShowFileList(TRUE);
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

    if (KeepPosition)
      strcpy(CurName,ListData[CurFile].Name);

    hSortPlugin=(PanelMode==PLUGIN_PANEL) ? hPlugin:NULL;

    qsort((void *)ListData,FileCount,sizeof(*ListData),SortList);
    if (KeepPosition)
      GoToFile(CurName);
  }
}


int _cdecl SortList(const void *el1,const void *el2)
{
  int RetCode;
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

  int NameCmp;
  if (ListCaseSensitive)
    NameCmp=ListSortOrder*strcmp(Name1,Name2);
  else
    NameCmp=ListSortOrder*LCStricmp(Name1,Name2);
  if (NameCmp==0)
    NameCmp=SPtr1->Position>SPtr2->Position ? ListSortOrder:-ListSortOrder;
  switch(ListSortMode)
  {
    case BY_NAME:
      return(NameCmp);
    case BY_EXT:
      ChPtr1=strrchr(*Name1 ? Name1+1:Name1,'.');
      ChPtr2=strrchr(*Name2 ? Name2+1:Name2,'.');
      if (ChPtr1==NULL && ChPtr2==NULL)
        return(NameCmp);
      if (ChPtr1==NULL)
        return(-ListSortOrder);
      if (ChPtr2==NULL)
        return(ListSortOrder);
      if (*(ChPtr1+1)=='.')
        return(-ListSortOrder);
      if (*(ChPtr2+1)=='.')
        return(ListSortOrder);
      RetCode=ListSortOrder*LocalStricmp(ChPtr1+1,ChPtr2+1);
      return(RetCode!=0 ? RetCode:NameCmp);
    case BY_MTIME:
      RetCode=CompareFileTime(&SPtr1->WriteTime,&SPtr2->WriteTime);
      if (RetCode==0)
        return(NameCmp);
      return(-ListSortOrder*RetCode);
    case BY_CTIME:
      RetCode=CompareFileTime(&SPtr1->CreationTime,&SPtr2->CreationTime);
      if (RetCode==0)
        return(NameCmp);
      return(-ListSortOrder*RetCode);
    case BY_ATIME:
      RetCode=CompareFileTime(&SPtr1->AccessTime,&SPtr2->AccessTime);
      if (RetCode==0)
        return(NameCmp);
      return(-ListSortOrder*RetCode);
    case BY_SIZE:
      if (SPtr1->UnpSizeHigh==SPtr2->UnpSizeHigh)
      {
        if (SPtr1->UnpSize==SPtr2->UnpSize)
          return(NameCmp);
        return((SPtr1->UnpSize > SPtr2->UnpSize) ? -ListSortOrder : ListSortOrder);
      }
      return((SPtr1->UnpSizeHigh > SPtr2->UnpSizeHigh) ? -ListSortOrder : ListSortOrder);
    case BY_DIZ:
      if (SPtr1->DizText==NULL)
        if (SPtr2->DizText==NULL)
          return(NameCmp);
        else
          return(ListSortOrder);
      if (SPtr2->DizText==NULL)
        return(-ListSortOrder);
      RetCode=ListSortOrder*LCStricmp(SPtr1->DizText,SPtr2->DizText);
      return(RetCode!=0 ? RetCode:NameCmp);
    case BY_OWNER:
      RetCode=ListSortOrder*LocalStricmp(SPtr1->Owner,SPtr2->Owner);
      return(RetCode!=0 ? RetCode:NameCmp);
    case BY_COMPRESSEDSIZE:
      if (SPtr1->PackSizeHigh==SPtr2->PackSizeHigh)
      {
        if (SPtr1->PackSize==SPtr2->PackSize)
          return(NameCmp);
        return((SPtr1->PackSize > SPtr2->PackSize) ? -ListSortOrder : ListSortOrder);
      }
      return((SPtr1->PackSizeHigh > SPtr2->PackSizeHigh) ? -ListSortOrder : ListSortOrder);
    case BY_NUMLINKS:
      if (SPtr1->NumberOfLinks==SPtr2->NumberOfLinks)
        return(NameCmp);
      return((SPtr1->NumberOfLinks > SPtr2->NumberOfLinks) ? -ListSortOrder : ListSortOrder);
  }
  return(0);
}


int _cdecl SortSearchList(const void *el1,const void *el2)
{
  struct FileListItem *SPtr1,*SPtr2;
  SPtr1=(struct FileListItem *)el1;
  SPtr2=(struct FileListItem *)el2;
  return(strcmp(SPtr1->Name,SPtr2->Name));
}


void FileList::SetFocus()
{
  Panel::SetFocus();
  SetTitle();
}


int FileList::ProcessKey(int Key)
{
  struct FileListItem *CurPtr;
  int N;
  int CmdLength=CtrlObject->CmdLine.GetLength();

  if (IsVisible())
  {
    if (PanelMode==PLUGIN_PANEL && !InternalProcessKey)
      if ((Key!=KEY_ENTER && Key!=KEY_SHIFTENTER) || CmdLength==0)
      {
        int VirtKey,ControlState;
        if (TranslateKeyToVK(Key,VirtKey,ControlState))
        {
          int ProcessCode=CtrlObject->Plugins.ProcessKey(hPlugin,VirtKey,ControlState);
          ProcessPluginCommand();
          if (ProcessCode)
            return(TRUE);
        }
      }
  }
  else
    if (Key!=KEY_SHIFTF4)
      return(FALSE);

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
    char ShortcutFolder[NM],PluginModule[NM],PluginFile[NM],PluginData[8192];
    if (PanelMode==PLUGIN_PANEL)
    {
      int PluginNumber=((struct PluginHandle *)hPlugin)->PluginNumber;
      strcpy(PluginModule,CtrlObject->Plugins.PluginsData[PluginNumber].ModuleName);
      struct OpenPluginInfo Info;
      CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
      strcpy(PluginFile,NullToEmpty(Info.HostFile));
      strcpy(ShortcutFolder,NullToEmpty(Info.CurDir));
      strncpy(PluginData,NullToEmpty(Info.ShortcutData),sizeof(PluginData)-1);
      PluginData[sizeof(PluginData)-1]=0;
    }
    else
    {
      *PluginModule=*PluginFile=*PluginData=0;
      strcpy(ShortcutFolder,CurDir);
    }
    if (SaveFolderShortcut(Key,ShortcutFolder,PluginModule,PluginFile,PluginData))
      return(TRUE);
    if (GetShortcutFolder(Key,ShortcutFolder,PluginModule,PluginFile,PluginData))
    {
      if (ProcessPluginEvent(FE_CLOSE,NULL))
        return(TRUE);
      if (*PluginModule)
      {
        if (*PluginFile)
        {
          OpenFilePlugin(PluginFile,FALSE);
          if (*ShortcutFolder)
            SetCurDir(ShortcutFolder,FALSE);
          Show();
        }
        else
        {
          for (int I=0;I<CtrlObject->Plugins.PluginsCount;I++)
          {
            if (LocalStricmp(CtrlObject->Plugins.PluginsData[I].ModuleName,PluginModule)==0)
            {
              if (CtrlObject->Plugins.PluginsData[I].pOpenPlugin)
              {
                HANDLE hNewPlugin=CtrlObject->Plugins.OpenPlugin(I,OPEN_SHORTCUT,(int)PluginData);
                if (hNewPlugin!=INVALID_HANDLE_VALUE)
                {
                  int CurFocus=GetFocus();
                  Panel *NewPanel=CtrlObject->ChangePanel(this,FILE_PANEL,TRUE,TRUE);
                  NewPanel->SetPluginMode(hNewPlugin,"");
                  if (*ShortcutFolder)
                    CtrlObject->Plugins.SetDirectory(hNewPlugin,ShortcutFolder,0);
                  NewPanel->Update(0);
                  if (CurFocus || !CtrlObject->GetAnotherPanel(NewPanel)->IsVisible())
                    NewPanel->SetFocus();
                  NewPanel->Show();
                }
              }
              break;
            }
          }
        }
        return(TRUE);
      }
      SetCurDir(ShortcutFolder,TRUE);
      Show();
      return(TRUE);
    }
  }

  switch(Key)
  {
    case KEY_F1:
      if (PanelMode==PLUGIN_PANEL && Help::PluginPanelHelp(hPlugin))
        return(TRUE);
      return(FALSE);
    case KEY_SHIFTSUBTRACT:
      SaveSelection();
      ClearSelection();
      Redraw();
      return(TRUE);
    case KEY_SHIFTADD:
      SaveSelection();
      {
        for (int I=0;I<FileCount;I++)
          if ((ListData[I].FileAttr & FA_DIREC)==0 || Opt.SelectFolders)
            Select(&ListData[I],1);
      }
      if (SelectedFirst)
        SortFileList(TRUE);
      Redraw();
      return(TRUE);
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
    case KEY_CTRLINS:
      if (CmdLength>0)
        return(FALSE);
    case KEY_CTRLSHIFTINS:
      CopyNames();
      return(TRUE);
    case KEY_ALTLEFT:
      LeftPos--;
      Redraw();
      return(TRUE);
    case KEY_ALTRIGHT:
      LeftPos++;
      Redraw();
      return(TRUE);
    case KEY_CTRLENTER:
    case KEY_CTRLJ:
    case KEY_CTRLF:
      if (FileCount>0 && SetCurPath())
      {
        char FileName[NM],temp[NM];
        int CurrentPath=FALSE;
        CurPtr=ListData+CurFile;
        strcpy(FileName,ShowShortNames && *CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name);
        if (strcmp(FileName,"..")==0)
        {
          strcpy(FileName,".");
          Key=KEY_CTRLF;
          CurrentPath=TRUE;
        }
        if (Key==KEY_CTRLF)
        {
          struct OpenPluginInfo Info;
          if (PanelMode==PLUGIN_PANEL)
            CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
          if (PanelMode!=PLUGIN_PANEL || (Info.Flags & OPIF_REALNAMES))
          {
            ConvertNameToFull(FileName,FileName);
            if (ShowShortNames)
              ConvertNameToShort(FileName,FileName);
            /* $ 20.10.2000 SVS
               ������� ��� Ctrl-F ��樮���쭮�!*/
            if(Opt.PanelCtrlFRule)
            {
              /* $ 13.10.2000 tran
                �� Ctrl-f ��� ������ �⢥��� �᫮��� �� ������ */
              if (ViewSettings.FolderUpperCase)
              {
                if ( CurPtr->FileAttr & FA_DIREC )
                  LocalStrupr(FileName);
                else
                {
                    strcpy(temp,FileName);
                    *strrchr(temp,'\\')=0;
                    LocalStrupr(temp);
                    strncpy(FileName,temp,strlen(temp));
                }
              }
              if (ViewSettings.FileUpperToLowerCase)
                if (!(CurPtr->FileAttr & FA_DIREC) && strrchr(FileName,'\\') && !IsCaseMixed(strrchr(FileName,'\\')))
                   LocalStrlwr(strrchr(FileName,'\\'));
              if ( ViewSettings.FileLowerCase && strrchr(FileName,'\\') && !(CurPtr->FileAttr & FA_DIREC))
                LocalStrlwr(strrchr(FileName,'\\'));
              /* tran $ */
            }
            /* SVS $ */
          }
          else
          {
            char FullName[NM];
            strcpy(FullName,NullToEmpty(Info.CurDir));
            /* $ 13.10.2000 tran
              �� Ctrl-f ��� ������ �⢥��� �᫮��� �� ������ */
            /* $ 20.10.2000 SVS
               ������� ��� Ctrl-F ��樮���쭮�!*/
            if (Opt.PanelCtrlFRule && ViewSettings.FolderUpperCase)
              LocalStrupr(FullName);
            /* SVS $ */
            /* tran $ */
            for (int I=0;FullName[I]!=0;I++)
              if (FullName[I]=='/')
                FullName[I]='\\';
            if (*FullName)
              AddEndSlash(FullName);
            /* $ 20.10.2000 SVS
               ������� ��� Ctrl-F ��樮���쭮�!*/
            if(Opt.PanelCtrlFRule)
            {
              /* $ 13.10.2000 tran
                �� Ctrl-f ��� ������ �⢥��� �᫮��� �� ������ */
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
        QuoteSpace(FileName);
        strcat(FileName," ");
        CtrlObject->CmdLine.InsertString(FileName);
      }
      return(TRUE);
    case KEY_CTRLBRACKET:
    case KEY_CTRLBACKBRACKET:
    case KEY_CTRLSHIFTBRACKET:
    case KEY_CTRLSHIFTBACKBRACKET:
      {
        Panel *SrcPanel;
        switch(Key)
        {
          case KEY_CTRLBRACKET:
            SrcPanel=CtrlObject->LeftPanel;
            break;
          case KEY_CTRLBACKBRACKET:
            SrcPanel=CtrlObject->RightPanel;
            break;
          case KEY_CTRLSHIFTBRACKET:
            SrcPanel=CtrlObject->ActivePanel;
            break;
          case KEY_CTRLSHIFTBACKBRACKET:
            SrcPanel=CtrlObject->GetAnotherPanel(CtrlObject->ActivePanel);
            break;
        }
        if (SrcPanel->GetType()!=FILE_PANEL)
          return(FALSE);

        FileList *SrcFilePanel=(FileList *)SrcPanel;
        char PanelDir[NM];
        if (SrcPanel->GetMode()!=PLUGIN_PANEL)
        {
          SrcPanel->GetCurDir(PanelDir);
          if (SrcPanel->GetShowShortNamesMode())
            ConvertNameToShort(PanelDir,PanelDir);
          AddEndSlash(PanelDir);
        }
        else
        {
          /* $ 20.09.2000 SVS
             + �᫨ � ������� ���� ��䨪�, � Ctrl-[ � ��� � ���
             ����⠢�� ���� ��䨪�.
          */
          PanelDir[0]=0;
          if(Opt.SubstPluginPrefix)
          {
            struct PluginInfo PInfo;
            CtrlObject->Plugins.GetPluginInfo(((struct PluginHandle *)SrcFilePanel->hPlugin)->PluginNumber,&PInfo);
            if(PInfo.CommandPrefix && *PInfo.CommandPrefix)
            {
              strcpy(PanelDir,PInfo.CommandPrefix);
              char *Ptr=strchr(PanelDir,':');
              if(Ptr) *++Ptr=0; else strcat(PanelDir,":");
            }
          }
          struct OpenPluginInfo Info;
          CtrlObject->Plugins.GetOpenPluginInfo(SrcFilePanel->hPlugin,&Info);
          strcat(PanelDir,NullToEmpty(Info.CurDir));
          /* SVS $ */
        }
        QuoteSpace(PanelDir);
        CtrlObject->CmdLine.InsertString(PanelDir);
      }
      return(TRUE);
    case KEY_CTRLA:
      if (PanelMode!=PLUGIN_PANEL ||
          CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FAROTHER))
        if (FileCount>0 && SetCurPath())
          ShellSetFileAttributes(this);
      return(TRUE);
    case KEY_CTRLG:
      if (PanelMode!=PLUGIN_PANEL ||
          CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FAROTHER))
        if (FileCount>0 && SetCurPath())
        {
          ApplyCommand();
          Update(UPDATE_KEEP_SELECTION);
          Redraw();
          Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
          AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
          AnotherPanel->Redraw();
        }
      return(TRUE);
    case KEY_CTRLZ:
      if (FileCount>0 && PanelMode==NORMAL_PANEL && SetCurPath())
        DescribeFiles();
      return(TRUE);
    case KEY_CTRLH:
      {
        Opt.ShowHidden=!Opt.ShowHidden;
        Update(UPDATE_KEEP_SELECTION);
        Redraw();
        Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
        AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
        AnotherPanel->Redraw();
      }
      return(TRUE);
    case KEY_CTRLM:
      RestoreSelection();
      return(TRUE);
    case KEY_CTRLR:
      Update(UPDATE_KEEP_SELECTION);
      Redraw();
      {
        Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
        if (AnotherPanel->GetType()!=FILE_PANEL)
        {
          AnotherPanel->SetCurDir(CurDir,FALSE);
          AnotherPanel->Redraw();
        }
      }
      break;
    case KEY_CTRLN:
      ShowShortNames=!ShowShortNames;
      Redraw();
      return(TRUE);
    case KEY_ENTER:
    case KEY_SHIFTENTER:
      if (CmdLength>0 || FileCount==0)
        break;
      ProcessEnter(1,Key==KEY_SHIFTENTER);
      return(TRUE);
    case KEY_CTRLBACKSLASH:
      ChangeDir("\\");
      if (PanelMode==PLUGIN_PANEL && *PluginsStack[PluginsStackSize-1].HostFile)
        ChangeDir("..");
      Show();
      return(TRUE);
    case KEY_SHIFTF1:
      if (FileCount>0 && PanelMode!=PLUGIN_PANEL && SetCurPath())
        PluginPutFilesToNew();
      return(TRUE);
    case KEY_SHIFTF2:
      if (FileCount>0 && SetCurPath())
      {
        if (PanelMode==PLUGIN_PANEL)
        {
          struct OpenPluginInfo Info;
          CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
          if (Info.HostFile!=NULL && *Info.HostFile!=0)
            ProcessKey(KEY_F5);
          return(TRUE);
        }
        PluginHostGetFiles();
      }
      return(TRUE);
    case KEY_SHIFTF3:
      ProcessHostFile();
      return(TRUE);
    case KEY_F3:
    case KEY_NUMPAD5:
    case KEY_ALTF3:
    case KEY_CTRLSHIFTF3:
    case KEY_F4:
    case KEY_ALTF4:
    case KEY_SHIFTF4:
    case KEY_CTRLSHIFTF4:
      if (Key==KEY_NUMPAD5)
        Key=KEY_F3;
      if ((Key==KEY_SHIFTF4 || FileCount>0) && SetCurPath())
      {
        int Edit=(Key==KEY_F4 || Key==KEY_ALTF4 || Key==KEY_SHIFTF4 || Key==KEY_CTRLSHIFTF4);
        int UploadFile=TRUE;
        char FileName[NM],ShortFileName[NM],PluginData[NM*2];

        int PluginMode=PanelMode==PLUGIN_PANEL &&
            !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILE);

        if (PluginMode)
        {
          struct OpenPluginInfo Info;
          CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
          sprintf(PluginData,"<%s:%s>",NullToEmpty(Info.HostFile),NullToEmpty(Info.CurDir));
        }
        else
          *PluginData=0;

        if (Key==KEY_SHIFTF4)
        {
          static char LastFileName[NM];
          /* $ 18.09.2000 SVS
             + �� �맮�� ।���� �� Shift-F4 ����� 㯮�ॡ����
               ��६���� �।�.
          */
          if (!GetString(MSG(MEditTitle),
                         MSG(MFileToEdit),
                         "NewEdit",
                         LastFileName,
                         LastFileName,
                         sizeof(LastFileName),
                         NULL,
                         FIB_EXPANDENV))
            return(FALSE);
          /* SVS $ */
          strcpy(FileName,LastFileName);
          Unquote(FileName);
          RemoveTrailingSpaces(FileName);
          ConvertNameToShort(FileName,ShortFileName);
          if (*FileName && (FileName[1]==':' ||
              FileName[0]=='\\' && FileName[1]=='\\'))
            PluginMode=FALSE;
        }
        else
        {
          CurPtr=ListData+CurFile;

          if (CurPtr->FileAttr & FA_DIREC)
          {
            if (Edit)
              return(TRUE);
            CountDirSize();
            return(TRUE);
          }

          strcpy(FileName,CurPtr->Name);
          strcpy(ShortFileName,*CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name);
        }
        char TempDir[NM],TempName[NM];

        int UploadFailed=FALSE;

        if (PluginMode)
        {
          strcpy(TempDir,Opt.TempPath);
          strcat(TempDir,"FarTmpXXXXXX");
          if (mktemp(TempDir)==NULL)
            return(TRUE);
          CreateDirectory(TempDir,NULL);
          sprintf(TempName,"%s\\%s",TempDir,PointToName(FileName));
          int NewFile=FALSE;
          if (Key==KEY_SHIFTF4)
          {
            int Pos=FindFile(FileName);
            if (Pos!=-1)
              CurPtr=ListData+Pos;
            else
            {
              NewFile=TRUE;
              strcpy(FileName,TempName);
            }
          }
          if (!NewFile)
          {
            struct PluginPanelItem PanelItem;
            FileListToPluginItem(CurPtr,&PanelItem);
            if (!CtrlObject->Plugins.GetFile(hPlugin,&PanelItem,TempDir,FileName,OPM_SILENT|(Edit ? OPM_EDIT:OPM_VIEW)))
            {
              RemoveDirectory(TempDir);
              return(TRUE);
            }
          }
          ConvertNameToShort(FileName,ShortFileName);
        }

        if (*FileName)
          if (Edit)
          {
            int EnableExternal=((Key==KEY_F4 || Key==KEY_SHIFTF4) && Opt.UseExternalEditor ||
                Key==KEY_ALTF4 && !Opt.UseExternalEditor) && *Opt.ExternalEditor;
            if (Key==KEY_ALTF4 || Key==KEY_CTRLSHIFTF4 || !ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_EDIT,PluginMode))
              if (EnableExternal)
                ProcessExternal(Opt.ExternalEditor,FileName,ShortFileName,0);
              else
                if (PluginMode)
                {
                  FileEditor ShellEditor(FileName,Key==KEY_SHIFTF4,FALSE,-1,-1,TRUE,PluginData);
                  UploadFile=ShellEditor.IsFileChanged();
                }
                else
                {
                  FileEditor *ShellEditor=new FileEditor(FileName,Key==KEY_SHIFTF4,TRUE);
                  CtrlObject->ModalManager.AddModal(ShellEditor);
                }
            if (PluginMode && UploadFile)
            {
              struct PluginPanelItem PanelItem;
              char SaveDir[NM];
              GetCurrentDirectory(sizeof(SaveDir),SaveDir);
              if (GetFileAttributes(TempName)==0xffffffff)
              {
                char FindName[NM];
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
                int PutCode=CtrlObject->Plugins.PutFiles(hPlugin,&PanelItem,1,FALSE,0);
                if (PutCode==1)
                  SetPluginModified();
                if (PutCode==0)
                  UploadFailed=TRUE;
              }
              chdir(SaveDir);
            }
          }
          else
          {
            int EnableExternal=(Key==KEY_F3 && Opt.UseExternalViewer ||
                Key==KEY_ALTF3 && !Opt.UseExternalViewer) && *Opt.ExternalViewer;
            if (Key==KEY_ALTF3 || Key==KEY_CTRLSHIFTF3 || !ProcessLocalFileTypes(FileName,ShortFileName,FILETYPE_VIEW,PluginMode))
              if (EnableExternal)
                ProcessExternal(Opt.ExternalViewer,FileName,ShortFileName,PluginMode);
              else
              {
                NamesList ViewList;
                if (!PluginMode)
                {
                  for (int I=0;I<FileCount;I++)
                    if ((ListData[I].FileAttr & FA_DIREC)==0)
                      ViewList.AddName(ListData[I].Name);
                  ViewList.SetCurDir(CurDir);
                  ViewList.SetCurName(FileName);
                }
                FileViewer *ShellViewer=new FileViewer(FileName,TRUE,PluginMode,PluginMode,-1,PluginData,&ViewList);
                if (PluginMode)
                  ShellViewer->SetTempViewName(FileName);
                CtrlObject->ModalManager.AddModal(ShellViewer);
              }
          }
        if (PluginMode)
          if (UploadFailed)
            Message(MSG_WARNING,1,MSG(MError),MSG(MCannotSaveFile),
                    MSG(MTextSavedToTemp),TempName,MSG(MOk));
          else
            DeleteFileWithFolder(TempName);
        if (Edit || IsColumnDisplayed(ADATE_COLUMN))
        {
          if (!PluginMode || UploadFile)
          {
            Update(UPDATE_KEEP_SELECTION);
            Redraw();
            Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
            if (AnotherPanel->GetMode()==NORMAL_PANEL)
            {
              AnotherPanel->Update(UPDATE_KEEP_SELECTION);
              AnotherPanel->Redraw();
            }
          }
          else
            SetTitle();
        }
        else
          if (PanelMode==NORMAL_PANEL)
            AccessTimeUpdateRequired=TRUE;
      }
      /* $ 15.07.2000 tran
         � ��� �� ��뢠�� ����ᮢ�� �������
         ��⮬� �� ��� viewer, editor ����� ��� ����୮ ����⠭�����
         */
      CtrlObject->Redraw();
      /* tran 15.07.2000 $ */
      return(TRUE);
    case KEY_F5:
    case KEY_F6:
    case KEY_ALTF6:
    case KEY_DRAGCOPY:
    case KEY_DRAGMOVE:
      if (FileCount>0 && SetCurPath())
        ProcessCopyKeys(Key);
      return(TRUE);

    /* $ 27.09.2000 SVS
       ����� ⥪�饣�/��࠭��� 䠩��/��
    */
    case KEY_ALTF5:
      if(CtrlObject->Plugins.FindPlugin(SYSID_PRINTMANAGER) != -1)
         CtrlObject->Plugins.CallPlugin(SYSID_PRINTMANAGER,OPEN_FILEPANEL,0); // printman
      else if (FileCount>0 && SetCurPath())
        PrintFiles(this);
      return(TRUE);
    /* SVS $ */

    case KEY_SHIFTF5:
    case KEY_SHIFTF6:
      if (FileCount>0 && SetCurPath())
      {
        int OldFileCount=FileCount,OldCurFile=CurFile;
        int OldSelection=ListData[CurFile].Selected;
        int ToPlugin=0;
        ReturnCurrentFile=TRUE;
        if (PanelMode!=PLUGIN_PANEL)
          ShellCopy ShCopy(this,Key==KEY_SHIFTF6,FALSE,TRUE,TRUE,ToPlugin,NULL);
        else
          ProcessCopyKeys(Key==KEY_SHIFTF5 ? KEY_F5:KEY_F6);
        ReturnCurrentFile=FALSE;
        if (Key!=KEY_SHIFTF5 && FileCount==OldFileCount &&
            CurFile==OldCurFile && OldSelection!=ListData[CurFile].Selected)
        {
          Select(&ListData[CurFile],OldSelection);
          Redraw();
        }
      }
      return(TRUE);
    case KEY_F7:
      if (SetCurPath())
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
          Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
          AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
          AnotherPanel->Redraw();
        }
        else
          ShellMakeDir(this);
      return(TRUE);
    case KEY_F8:
    case KEY_SHIFTDEL:
    case KEY_SHIFTF8:
    case KEY_ALTDEL:
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
    case KEY_HOME:
      Up(0x7fffff);
      return(TRUE);
    case KEY_END:
      Down(0x7fffff);
      return(TRUE);
    case KEY_UP:
      Up(1);
      return(TRUE);
    case KEY_DOWN:
      Down(1);
      return(TRUE);
    case KEY_PGUP:
      N=Columns*Height-1;
      CurTopFile-=N;
      CurFile-=N;
      ShowFileList(TRUE);
      return(TRUE);
    case KEY_PGDN:
      N=Columns*Height-1;
      CurTopFile+=N;
      CurFile+=N;
      ShowFileList(TRUE);
      return(TRUE);
    case KEY_LEFT:
      if (Columns>1 || CmdLength==0)
      {
        if (CurTopFile>=Height && CurFile-CurTopFile<Height)
          CurTopFile-=Height;
        Up(Height);
        return(TRUE);
      }
      return(FALSE);
    case KEY_RIGHT:
      if (Columns>1 || CmdLength==0)
      {
        if (CurFile+Height<FileCount && CurFile-CurTopFile>=(Columns-1)*(Height))
          CurTopFile+=Height;
        Down(Height);
        return(TRUE);
      }
      return(FALSE);
    case KEY_SHIFTHOME:
      InternalProcessKey++;
      DisableOut++;
      while (CurFile>0)
        ProcessKey(KEY_SHIFTUP);
      ProcessKey(KEY_SHIFTUP);
      InternalProcessKey--;
      DisableOut--;
      ShowFileList(TRUE);
      return(TRUE);
    case KEY_SHIFTEND:
      InternalProcessKey++;
      DisableOut++;
      while (CurFile<FileCount-1)
        ProcessKey(KEY_SHIFTDOWN);
      ProcessKey(KEY_SHIFTDOWN);
      InternalProcessKey--;
      DisableOut--;
      ShowFileList(TRUE);
      return(TRUE);
    case KEY_SHIFTLEFT:
    case KEY_SHIFTRIGHT:
      if (Columns>1)
      {
        int N=Height;
        InternalProcessKey++;
        DisableOut++;
        while (N--)
          ProcessKey(Key==KEY_SHIFTLEFT ? KEY_SHIFTUP:KEY_SHIFTDOWN);
        Select(ListData+CurFile,ShiftSelection);
        if (SelectedFirst)
          SortFileList(TRUE);
        InternalProcessKey--;
        DisableOut--;
        ShowFileList(TRUE);
        return(TRUE);
      }
      return(FALSE);
    case KEY_SHIFTPGUP:
    case KEY_SHIFTPGDN:
      N=Columns*Height-1;
      InternalProcessKey++;
      DisableOut++;
      while (N--)
        ProcessKey(Key==KEY_SHIFTPGUP ? KEY_SHIFTUP:KEY_SHIFTDOWN);
      InternalProcessKey--;
      DisableOut--;
      ShowFileList(TRUE);
      return(TRUE);
    case KEY_SHIFTUP:
    case KEY_SHIFTDOWN:
      if (FileCount==0)
        return(TRUE);
      CurPtr=ListData+CurFile;
      if (ShiftSelection==-1)
        ShiftSelection=!CurPtr->Selected;
      Select(CurPtr,ShiftSelection);
      if (Key==KEY_SHIFTUP)
        Up(1);
      else
        Down(1);
      if (SelectedFirst)
        SortFileList(TRUE);
      ShowFileList(TRUE);
      return(TRUE);
    case KEY_INS:
      if (FileCount==0)
        return(TRUE);
      CurPtr=ListData+CurFile;
      Select(CurPtr,!CurPtr->Selected);
      Down(1);
      if (SelectedFirst)
        SortFileList(TRUE);
      ShowFileList(TRUE);
      return(TRUE);
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
    case KEY_CTRLPGUP:
      ChangeDir("..");
      Show();
      return(TRUE);
    case KEY_CTRLPGDN:
      ProcessEnter(0,0);
      return(TRUE);
    default:
      if (Key>=KEY_ALT_BASE && Key<=KEY_ALT_BASE+255 ||
          Key>=KEY_ALTSHIFT_BASE && Key<=KEY_ALTSHIFT_BASE+255)
        FastFind(Key);
      else
        break;
      return(TRUE);
  }
  return(FALSE);
}


void FileList::Select(struct FileListItem *SelPtr,int Selection)
{
  if (strcmp(SelPtr->Name,"..")!=0 && SelPtr->Selected!=Selection)
    if ((SelPtr->Selected=Selection)!=0)
    {
      SelFileCount++;
      SelFileSize+=int64(SelPtr->UnpSizeHigh,SelPtr->UnpSize);
    }
    else
    {
       SelFileCount--;
       SelFileSize-=int64(SelPtr->UnpSizeHigh,SelPtr->UnpSize);;
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
    if (PanelMode==PLUGIN_PANEL || strchr(CurPtr->Name,'?')==NULL ||
        *CurPtr->ShortName==0)
      ChangeDir(CurPtr->Name);
    else
      ChangeDir(CurPtr->ShortName);
    Show();
  }
  else
  {
    int PluginMode=PanelMode==PLUGIN_PANEL &&
        !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILE);
    if (PluginMode)
    {
      char TempDir[NM];
      strcpy(TempDir,Opt.TempPath);
      strcat(TempDir,"FarTmpXXXXXX");
      if (mktemp(TempDir)==NULL)
        return;
      CreateDirectory(TempDir,NULL);
      struct PluginPanelItem PanelItem;
      FileListToPluginItem(CurPtr,&PanelItem);
      if (!CtrlObject->Plugins.GetFile(hPlugin,&PanelItem,TempDir,FileName,OPM_SILENT|OPM_VIEW))
      {
        RemoveDirectory(TempDir);
        return;
      }
      ConvertNameToShort(FileName,ShortFileName);
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
      BatType=stricmp(ExtPtr,".bat")==0 || stricmp(ExtPtr,".cmd")==0;
    }
    if (EnableExec && (ExeType || BatType))
    {
      QuoteSpace(FileName);
      if (!PluginMode)
        CtrlObject->CmdHistory->AddToHistory(FileName);
      int DirectRun=(CurDir[0]=='\\' && CurDir[1]=='\\' && ExeType);
      CtrlObject->CmdLine.ExecString(FileName,PluginMode,SeparateWindow,DirectRun);
      if (PluginMode)
        DeleteFileWithFolder(FileName);
    }
    else
      if (SetCurPath())
      {
        HANDLE hOpen=NULL;
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
  if (ClosePlugin && PanelMode==PLUGIN_PANEL)
  {
    while (1)
    {
      if (ProcessPluginEvent(FE_CLOSE,NULL))
        return;
      if (!PopPlugin(TRUE))
        break;
    }
    CtrlObject->RedrawKeyBar();
  }
  ChangeDir(NewDir);
}


void FileList::ChangeDir(char *NewDir)
{
  Panel *AnotherPanel;
  char FindDir[NM],SetDir[NM];

  strcpy(SetDir,NewDir);

  if (strcmp(SetDir,"..")!=0 && strcmp(SetDir,"\\")!=0)
    UpperFolderTopFile=CurTopFile;

  if (SelFileCount>0)
    ClearSelection();

  int PluginClosed=FALSE,GoToPanelFile=FALSE;
  if (PanelMode==PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);

    CtrlObject->FolderHistory->AddToHistory(NullToEmpty(Info.CurDir),Info.Format,1);

    int UpperFolder=(strcmp(SetDir,"..")==0);
    if (UpperFolder && *NullToEmpty(Info.CurDir)==0)
    {
      if (ProcessPluginEvent(FE_CLOSE,NULL))
        return;
      PluginClosed=TRUE;
      strcpy(FindDir,NullToEmpty(Info.HostFile));
      if (*FindDir==0 && (Info.Flags & OPIF_REALNAMES) && CurFile<FileCount)
      {
        strcpy(FindDir,ListData[CurFile].Name);
        GoToPanelFile=TRUE;
      }
      PopPlugin(TRUE);
      Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
      if (AnotherPanel->GetType()==INFO_PANEL)
        AnotherPanel->Redraw();
    }
    else
    {
      strcpy(FindDir,NullToEmpty(Info.CurDir));
      CtrlObject->Plugins.SetDirectory(hPlugin,SetDir,0);
    }
    ProcessPluginCommand();
    Update(0);
    if (PluginClosed && PrevDataStackSize>0)
    {
      PrevDataStackSize--;
      if (PrevDataStack[PrevDataStackSize].PrevFileCount>0)
      {
        MoveSelection(ListData,FileCount,PrevDataStack[PrevDataStackSize].PrevListData,PrevDataStack[PrevDataStackSize].PrevFileCount);
        if (!GoToPanelFile)
          strcpy(FindDir,PrevDataStack[PrevDataStackSize].PrevName);
        DeleteListData(PrevDataStack[PrevDataStackSize].PrevListData,PrevDataStack[PrevDataStackSize].PrevFileCount);
        if (ListSelectedFirst)
          SortFileList(FALSE);
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
    else
      CurFile=CurTopFile=0;
    return;
  }
  else
  {
    char FullNewDir[NM];
    ConvertNameToFull(SetDir,FullNewDir);
    if (LocalStricmp(FullNewDir,CurDir)!=0)
      CtrlObject->FolderHistory->AddToHistory(CurDir,NULL,0);

    /* $ 21.09.2000 SVS
       �⫮��� ������ ".." � "\\host\share"
    */
    if(!strcmp(SetDir,".."))
    {
      if(CurDir[0] == '\\' && CurDir[1] == '\\')
      {
        char *PtrS1=strchr(CurDir+2,'\\');
        if(PtrS1 && !strchr(PtrS1+1,'\\'))
        {
          //*PtrS1=0;
          if(CtrlObject->Plugins.CallPlugin(0x5774654E,OPEN_FILEPANEL,CurDir)) // NetWork Plugin :-)
            return;
        }
      }
    }
    /* SVS $ */
  }

  strcpy(FindDir,PointToName(CurDir));

  if (SetDir[0]==0 || SetDir[1]!=':' || SetDir[2]!='\\')
    chdir(CurDir);

  chdir(SetDir);
  if (isalpha(SetDir[0]) && SetDir[1]==':')
  {
    int CurDisk=toupper(SetDir[0])-'A';
    setdisk(CurDisk);
  }
  GetCurrentDirectory(sizeof(CurDir),CurDir);

  Update(0);

  if (strcmp(SetDir,"..")==0)
  {
    GoToFile(FindDir);
    CurTopFile=UpperFolderTopFile;
    UpperFolderTopFile=0;
    CorrectPosition();
  }
  else
    CurFile=CurTopFile=0;

  if (GetFocus())
  {
    CtrlObject->CmdLine.SetCurDir(CurDir);
    CtrlObject->CmdLine.Show();
  }
  AnotherPanel=CtrlObject->GetAnotherPanel(this);
  if (AnotherPanel->GetType()!=FILE_PANEL)
  {
    AnotherPanel->SetCurDir(CurDir,FALSE);
    AnotherPanel->Redraw();
  }
  if (PanelMode==PLUGIN_PANEL)
    CtrlObject->RedrawKeyBar();
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
        int ProcessCode=CtrlObject->Plugins.ProcessKey(hPlugin,VK_RETURN,ShiftPressed ? PKF_SHIFT:0);
        ProcessPluginCommand();
        if (ProcessCode)
          return(TRUE);
      }
      ProcessEnter(1,ShiftPressed!=0);
      return(TRUE);
    }
    else
      /* $ 11.09.2000 SVS
         Bug #17: �뤥�塞 �� �᫮���, �� ������� ��������� ����.
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
  + ��樮���쭮� ��������� ��� �ࠢ�� ������ ��� �� ���⮩ ������
*/
void FileList::MoveToMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  int CurColumn=0,ColumnsWidth,I;
  int PanelX=MouseEvent->dwMousePosition.X-X1-1;
  for (ColumnsWidth=I=0;I<ViewSettings.ColumnCount;I++)
  {
    if ((ViewSettings.ColumnType[I] & 0xff)==NAME_COLUMN)
      CurColumn++;
    ColumnsWidth+=ViewSettings.ColumnWidth[I];
    if (ColumnsWidth>=PanelX)
      break;
    ColumnsWidth++;
  }
  if (CurColumn==0)
    CurColumn=1;
  int OldCurFile=CurFile;
  CurFile=CurTopFile+MouseEvent->dwMousePosition.Y-Y1-1-Opt.ShowColumnTitles;
  if (CurColumn>1)
    CurFile+=(CurColumn-1)*Height;
  CorrectPosition();
  /* $ 11.09.2000 SVS
     Bug #17: �஢�ਬ �� ��������� ������ �������.
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
  PrepareViewSettings(ViewMode,NULL);
  int NewOwner=IsColumnDisplayed(OWNER_COLUMN);
  int NewPacked=IsColumnDisplayed(PACKED_COLUMN);
  int NewNumLink=IsColumnDisplayed(NUMLINK_COLUMN);
  int NewDiz=IsColumnDisplayed(DIZ_COLUMN);
  int NewAccessTime=IsColumnDisplayed(ADATE_COLUMN);
  int NewCaseSensitiveSort=ViewSettings.CaseSensitiveSort;
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
    if (OldCaseSensitiveSort!=NewCaseSensitiveSort)
      ResortRequired=TRUE;

  if (!OldDiz && NewDiz)
    ReadDiz();

  if (ViewSettings.FullScreen && !CurFullScreen)
  {
    Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
    int AnotherVisible=AnotherPanel->IsVisible();
    Hide();
    AnotherPanel->Hide();
    if (Y2>0)
      SetPosition(0,Y1,ScrX,Y2);
    FileList::ViewMode=ViewMode;
    if (AnotherVisible)
      AnotherPanel->Show();
    Show();
  }
  else
    if (!ViewSettings.FullScreen && CurFullScreen)
    {
      Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
      int AnotherVisible=AnotherPanel->IsVisible();
      Hide();
      AnotherPanel->Hide();
      if (Y2>0)
        if (this==CtrlObject->LeftPanel)
          SetPosition(0,Y1,ScrX/2-Opt.WidthDecrement,Y2);
        else
          SetPosition(ScrX/2+1-Opt.WidthDecrement,Y1,ScrX,Y2);
      FileList::ViewMode=ViewMode;
      if (AnotherVisible)
        AnotherPanel->Show();
      Show();
    }
    else
    {
      FileList::ViewMode=ViewMode;
      Show();
    }
  if (PanelMode==PLUGIN_PANEL)
  {
    char ColumnTypes[80],ColumnWidths[80];
    ViewSettingsToText(ViewSettings.ColumnType,ViewSettings.ColumnWidth,
        ViewSettings.ColumnCount,ColumnTypes,ColumnWidths);
    ProcessPluginEvent(FE_CHANGEVIEWMODE,ColumnTypes);
  }
  if (ResortRequired)
  {
    SortFileList(TRUE);
    ShowFileList(TRUE);
    Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
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
  Show();
}


int FileList::GoToFile(char *Name)
{
  long Pos=FindFile(Name);
  if (Pos!=-1)
  {
    CurFile=Pos;
    CorrectPosition();
    return(TRUE);
  }
  return(FALSE);
}


int FileList::FindFile(char *Name)
{
  long I;
  for (I=0;I<FileCount;I++)
    if (strcmp(Name,ListData[I].Name)==0)
      return(I);
  for (I=0;I<FileCount;I++)
    if (LocalStricmp(Name,ListData[I].Name)==0)
      return(I);
  return(-1);
}


int FileList::IsSelected(char *Name)
{
  long Pos=FindFile(Name);
  return(Pos!=-1 && (ListData[Pos].Selected || SelFileCount==0 && Pos==CurFile));
}


int FileList::FindPartName(char *Name,int Next)
{
  char Mask[NM];
  int I;

  /* $ 02.08.2000 IG
     Wish.Mix #21 - �� ����⨨ '/' ��� '\' � QuickSerach ���室�� �� ��४���
  */
  int DirFind = 0;
  int Length = strlen(Name);

  strcpy(Mask,Name);
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
  for (I=(Next) ? CurFile+1:CurFile;I<FileCount;I++)
  {
    CmpNameSearchMode=(I==CurFile);
    if (CmpName(Mask,ListData[I].Name,TRUE))
      if (strcmp(ListData[I].Name,"..")!=0)
        if (!DirFind || (ListData[I].FileAttr & FA_DIREC))
        {
          CmpNameSearchMode=FALSE;
          CurFile=I;
          CurTopFile=CurFile-(Y2-Y1)/2;
          ShowFileList(TRUE);
          return(TRUE);
        }
  }
  CmpNameSearchMode=FALSE;
  for (I=0;I<CurFile;I++)
    if (CmpName(Mask,ListData[I].Name,TRUE))
      if (strcmp(ListData[I].Name,"..")!=0)
        if (!DirFind || (ListData[I].FileAttr & FA_DIREC))
        {
          CurFile=I;
          CurTopFile=CurFile-(Y2-Y1)/2;
          ShowFileList(TRUE);
          return(TRUE);
        }
  /* IG $ */
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


int FileList::GetSelName(char *Name,int &FileAttr,char *ShortName)
{
  if (Name==NULL)
  {
    GetSelPosition=0;
    LastSelPosition=-1;
    return(TRUE);
  }

  if (SelFileCount==0 || ReturnCurrentFile)
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
      return(TRUE);
    }
    else
      return(FALSE);

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


long FileList::GetLastSelectedSize(int64 *Size)
{
  if (LastSelPosition>=0 && LastSelPosition<FileCount)
  {
    if (Size!=NULL)
      Size->Set(ListData[LastSelPosition].UnpSizeHigh,ListData[LastSelPosition].UnpSize);
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


void FileList::SelectFiles(int Mode)
{
  const char *HistoryName="Masks";
  static struct DialogData SelectDlgData[]=
  {
    DI_DOUBLEBOX,3,1,41,3,0,0,0,0,"",
    DI_EDIT,5,2,39,2,1,(DWORD)HistoryName,DIF_HISTORY,1,""
  };
  MakeDialogItems(SelectDlgData,SelectDlg);

  struct FileListItem *CurPtr;
  static char PrevMask[NM]="*.*";
  char Mask[NM];
  int Selection,I;

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
  bool MultipleMasks=false;
  if (Mode==SELECT_ADDEXT || Mode==SELECT_REMOVEEXT)
  {
    strcpy(Mask,"*.");
    char *DotPtr=strrchr(CurName,'.');
    if (DotPtr!=NULL)
      strcat(Mask,DotPtr+1);
    Mode=(Mode==SELECT_ADDEXT) ? SELECT_ADD:SELECT_REMOVE;
  }
  else
    if (Mode==SELECT_ADDNAME || Mode==SELECT_REMOVENAME)
    {
      strcpy(Mask,CurName);
      char *DotPtr=strrchr(Mask,'.');
      if (DotPtr!=NULL)
        strcpy(DotPtr,".*");
      else
        strcat(Mask,".*");
      Mode=(Mode==SELECT_ADDNAME) ? SELECT_ADD:SELECT_REMOVE;
    }
    else
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
          Dlg.SetPosition(-1,-1,45,5);
          Dlg.Process();
          if (Dlg.GetExitCode()!=1)
            return;
        }
        strncpy(Mask,SelectDlg[1].Data,sizeof(Mask)-1);
        Mask[sizeof(Mask)-1]=0;
        Unquote(Mask);
        strcpy(PrevMask,Mask);
        MultipleMasks=true;
      }
  SaveSelection();
  for (I=0;I<FileCount;I++)
  {
    int Match=FALSE;
    CurPtr=&ListData[I];
    if (Mode==SELECT_INVERT || Mode==SELECT_INVERTALL)
      Match=TRUE;
    else
      if (MultipleMasks)
      {
        char ArgName[NM],*NamePtr=Mask;
        while ((NamePtr=GetCommaWord(NamePtr,ArgName))!=NULL)
          if (CmpName(ArgName,(ShowShortNames && *CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name)))
          {
            Match=TRUE;
            break;
          }
      }
      else
        Match=CmpName(Mask,(ShowShortNames && *CurPtr->ShortName ? CurPtr->ShortName:CurPtr->Name));

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
      if ((CurPtr->FileAttr & FA_DIREC)==0 || Selection==0 ||
          Opt.SelectFolders || RawSelection || Mode==SELECT_INVERTALL)
        Select(CurPtr,Selection);
    }
  }
  if (SelectedFirst)
    SortFileList(TRUE);
  ShowFileList(TRUE);
}


void FileList::UpdateViewPanel()
{
  Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
  if (FileCount>0 && AnotherPanel->IsVisible() &&
      AnotherPanel->GetType()==QVIEW_PANEL && SetCurPath())
  {
    QuickView *ViewPanel=(QuickView *)AnotherPanel;
    struct FileListItem *CurPtr=ListData+CurFile;
    if (PanelMode!=PLUGIN_PANEL ||
        CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILE))
    {
      if (strcmp(CurPtr->Name,"..")==0)
        ViewPanel->ShowFile(CurDir,FALSE,NULL);
      else
        ViewPanel->ShowFile(CurPtr->Name,FALSE,NULL);
    }
    else
      if ((CurPtr->FileAttr & FA_DIREC)==0)
      {
        char TempDir[NM],FileName[NM];
        strcpy(FileName,CurPtr->Name);
        strcpy(TempDir,Opt.TempPath);
        strcat(TempDir,"FarTmpXXXXXX");
        if (mktemp(TempDir)==NULL)
          return;
        CreateDirectory(TempDir,NULL);
        struct PluginPanelItem PanelItem;
        FileListToPluginItem(CurPtr,&PanelItem);
        if (!CtrlObject->Plugins.GetFile(hPlugin,&PanelItem,TempDir,FileName,OPM_SILENT|OPM_VIEW))
        {
          RemoveDirectory(TempDir);
          return;
        }
        ViewPanel->ShowFile(FileName,TRUE,NULL);
      }
      else
        if (strcmp(CurPtr->Name,"..")!=0)
          ViewPanel->ShowFile(CurPtr->Name,FALSE,hPlugin);
        else
          ViewPanel->ShowFile(NULL,FALSE,NULL);

    SetTitle();
  }
}


void FileList::CompareDir()
{
  FileList *Another=(FileList *)CtrlObject->GetAnotherPanel(this);
  int I,J;
  if (Another->GetType()!=FILE_PANEL || !Another->IsVisible())
  {
    Message(MSG_WARNING,1,MSG(MCompareTitle),MSG(MCompareFilePanelsRequired1),
            MSG(MCompareFilePanelsRequired2),MSG(MOk));
    return;
  }

  ScrBuf.Flush();
  ClearSelection();
  Another->ClearSelection();
  for (I=0;I<FileCount;I++)
    Select(&ListData[I],(ListData[I].FileAttr & FA_DIREC)==0);
  for (J=0;J<Another->FileCount;J++)
    Another->Select(&Another->ListData[J],(Another->ListData[J].FileAttr & FA_DIREC)==0);

  int CompareFatTime=FALSE;
  if (PanelMode==PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
    if (Info.Flags & OPIF_COMPAREFATTIME)
      CompareFatTime=TRUE;
  }
  if (Another->PanelMode==PLUGIN_PANEL && !CompareFatTime)
  {
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(Another->hPlugin,&Info);
    if (Info.Flags & OPIF_COMPAREFATTIME)
      CompareFatTime=TRUE;
  }

  if (PanelMode==NORMAL_PANEL && Another->PanelMode==NORMAL_PANEL)
  {
    char RootDir1[NM],RootDir2[NM];
    char FileSystemName1[NM],FileSystemName2[NM];
    GetPathRoot(CurDir,RootDir1);
    GetPathRoot(Another->CurDir,RootDir2);
    if (GetVolumeInformation(RootDir1,NULL,0,NULL,NULL,NULL,FileSystemName1,sizeof(FileSystemName1)) &&
        GetVolumeInformation(RootDir2,NULL,0,NULL,NULL,NULL,FileSystemName2,sizeof(FileSystemName2)))
      if (stricmp(FileSystemName1,FileSystemName2)!=0)
        CompareFatTime=TRUE;
  }

  for (I=0;I<FileCount;I++)
    for (J=0;J<Another->FileCount;J++)
      if (LocalStricmp(PointToName(ListData[I].Name),PointToName(Another->ListData[J].Name))==0)
      {
        int Cmp;
        if (CompareFatTime)
        {
          WORD DosDate,DosTime,AnotherDosDate,AnotherDosTime;
          FileTimeToDosDateTime(&ListData[I].WriteTime,&DosDate,&DosTime);
          FileTimeToDosDateTime(&Another->ListData[J].WriteTime,&AnotherDosDate,&AnotherDosTime);
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
          Cmp=CompareFileTime(&ListData[I].WriteTime,&Another->ListData[J].WriteTime);
        if (Cmp==0 && (ListData[I].UnpSize!=Another->ListData[J].UnpSize ||
            ListData[I].UnpSizeHigh!=Another->ListData[J].UnpSizeHigh))
          continue;
        if (Cmp<1)
          Select(&ListData[I],0);
        if (Cmp>-1)
          Another->Select(&Another->ListData[J],0);
        break;
      }
  if (SelectedFirst)
    SortFileList(TRUE);
  Redraw();
  Another->Redraw();
  if (SelFileCount==0 && Another->SelFileCount==0)
    Message(0,1,MSG(MCompareTitle),MSG(MCompareSameFolders1),MSG(MCompareSameFolders2),MSG(MOk));
}


void FileList::CopyNames()
{
  char *CopyData=NULL;
  long DataSize=0;
  char SelName[NM],SelShortName[NM];
  int FileAttr;
  GetSelName(NULL,FileAttr);
  while (GetSelName(SelName,FileAttr,SelShortName))
  {
    if (DataSize>0)
    {
      strcat(CopyData+DataSize,"\r\n");
      DataSize+=2;
    }
    char QuotedName[NM];
    strcpy(QuotedName,ShowShortNames && *SelShortName ? SelShortName:SelName);
    QuoteSpace(QuotedName);
    int Length=strlen(QuotedName);
    char *NewPtr=(char *)realloc(CopyData,DataSize+Length+3);
    if (NewPtr==NULL)
    {
      delete CopyData;
      CopyData=NULL;
      break;
    }
    CopyData=NewPtr;
    CopyData[DataSize]=0;
    strcat(CopyData+DataSize,QuotedName);
    DataSize+=Length;
  }

  CopyToClipboard(CopyData);
  delete CopyData;
}


void FileList::SetTitle()
{
  if (GetFocus() || CtrlObject->GetAnotherPanel(this)->GetType()!=FILE_PANEL)
  {
    char TitleDir[NM];
    if (PanelMode==PLUGIN_PANEL)
    {
      struct OpenPluginInfo Info;
      char Title[512];
      CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
      strcpy(Title,NullToEmpty(Info.PanelTitle));
      RemoveLeadingSpaces(Title);
      RemoveTrailingSpaces(Title);
      sprintf(TitleDir,"{%s}",Title);
    }
    else
      sprintf(TitleDir,"{%s}",CurDir);
    strcpy(LastFarTitle,TitleDir);
    SetFarTitle(TitleDir);
  }
}


void FileList::ClearSelection()
{
  for (int I=0;I<FileCount;I++)
    Select(&ListData[I],0);
  if (SelectedFirst)
    SortFileList(TRUE);
}


void FileList::SaveSelection()
{
  for (int I=0;I<FileCount;I++)
    ListData[I].PrevSelected=ListData[I].Selected;
}


void FileList::RestoreSelection()
{
  for (int I=0;I<FileCount;I++)
  {
    int NewSelection=ListData[I].PrevSelected;
    ListData[I].PrevSelected=ListData[I].Selected;
    Select(&ListData[I],NewSelection);
  }
  if (SelectedFirst)
    SortFileList(TRUE);
  Redraw();
}


int FileList::GetFileName(char *Name,int Pos,int &FileAttr)
{
  if (Pos>=FileCount)
    return(FALSE);
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
    Filter=new PanelFilter(this);
  Filter->FilterEdit();
}


void FileList::SelectSortMode()
{
  struct MenuData SortMenu[]=
  {
    (char *)MMenuSortByName,1,0,0,
    (char *)MMenuSortByExt,0,0,0,
    (char *)MMenuSortByModification,0,0,0,
    (char *)MMenuSortBySize,0,0,0,
    (char *)MMenuUnsorted,0,0,0,
    (char *)MMenuSortByCreation,0,0,0,
    (char *)MMenuSortByAccess,0,0,0,
    (char *)MMenuSortByDiz,0,0,0,
    (char *)MMenuSortByOwner,0,0,0,
    (char *)MMenuSortByCompressedSize,0,0,0,
    (char *)MMenuSortByNumLinks,0,0,0,
    "",0,0,1,
    (char *)MMenuSortUseGroups,0,0,0,
    (char *)MMenuSortSelectedFirst,0,0,0
  };

  SortMenu[12].Checked=GetSortGroups();
  SortMenu[13].Checked=SelectedFirst;

  static int SortModes[]={BY_NAME,BY_EXT,BY_MTIME,BY_SIZE,UNSORTED,
    BY_CTIME,BY_ATIME,BY_DIZ,BY_OWNER,BY_COMPRESSEDSIZE,BY_NUMLINKS};

  for (int I=0;I<sizeof(SortModes)/sizeof(SortModes[0]);I++)
    if (SortMode==SortModes[I])
    {
      SortMenu[I].Checked=SortOrder==1 ? '+':'-';
      break;
    }

  int SortCode;
  {
    VMenu SortModeMenu(MSG(MMenuSortTitle),SortMenu,sizeof(SortMenu)/sizeof(SortMenu[0]),0);
    SortModeMenu.SetPosition(X1+4,-1,0,0);
    SortModeMenu.Process();
    if ((SortCode=SortModeMenu.GetExitCode())<0)
      return;
  }
  if (SortCode<sizeof(SortModes)/sizeof(SortModes[0]))
    SetSortMode(SortModes[SortCode]);
  else
    switch(SortCode)
    {
      case 12:
        ProcessKey(KEY_SHIFTF11);
        break;
      case 13:
        ProcessKey(KEY_SHIFTF12);
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
       ��� Ctrl-Z ���㦭� ���� �।��饥 ���祭��!
    */
    if (!GetString(MSG(MDescribeFiles),TruncMsg,"DizText",
                   PrevText!=NULL ? PrevText:"",DizText,sizeof(DizText),
                   "FileDiz",FIB_ENABLEEMPTY|FIB_NOUSELASTHISTORY))
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
  }
  if (DizCount>0)
  {
    FlushDiz();
    Update(UPDATE_KEEP_SELECTION);
    Redraw();
    Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
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
  if (!GetString(MSG(MAskApplyCommandTitle),MSG(MAskApplyCommand),"ApplyCmd",PrevCommand,Command,sizeof(Command),"ApplyCmd"))
    return;
  strcpy(PrevCommand,Command);
  char SelName[NM],SelShortName[NM];
  int FileAttr;

  RedrawDesktop Redraw;
  SaveSelection();
  GetSelName(NULL,FileAttr);
  while (GetSelName(SelName,FileAttr,SelShortName) && !CheckForEsc())
  {
    char ConvertedCommand[512];
    strcpy(ConvertedCommand,Command);
    {
      int PreserveLFN=SubstFileName(ConvertedCommand,SelName,SelShortName,NULL,NULL);
      PreserveLongName PreserveName(SelShortName,PreserveLFN);
      Execute(ConvertedCommand,FALSE,FALSE);
      ClearLastGetSelection();
    }
  }
}


void FileList::CountDirSize()
{
  unsigned long DirCount,DirFileCount,ClusterSize;;
  int64 FileSize,CompressedFileSize,RealFileSize;
  unsigned long SelDirCount=0;
  for (int I=0;I<FileCount;I++)
  {
    struct FileListItem *CurPtr=ListData+I;
    if (CurPtr->Selected && (CurPtr->FileAttr & FA_DIREC))
    {
      SelDirCount++;
      if (PanelMode==PLUGIN_PANEL &&
          GetPluginDirInfo(hPlugin,CurPtr->Name,DirCount,DirFileCount,
                           FileSize,CompressedFileSize) ||
          PanelMode!=PLUGIN_PANEL &&
          GetDirInfo(MSG(MDirInfoViewTitle),CurPtr->Name,DirCount,
                     DirFileCount,FileSize,CompressedFileSize,RealFileSize,
                     ClusterSize,0,FALSE)==1)
      {
        SelFileSize-=int64(CurPtr->UnpSizeHigh,CurPtr->UnpSize);
        SelFileSize+=FileSize;
        CurPtr->UnpSize=FileSize.LowPart;
        CurPtr->UnpSizeHigh=FileSize.HighPart;
        CurPtr->PackSize=CompressedFileSize.LowPart;
        CurPtr->PackSizeHigh=CompressedFileSize.HighPart;
        CurPtr->ShowFolderSize=1;
      }
      else
        break;
    }
  }
  struct FileListItem *CurPtr=ListData+CurFile;
  if (SelDirCount==0)
    if (PanelMode==PLUGIN_PANEL &&
        GetPluginDirInfo(hPlugin,CurPtr->Name,DirCount,DirFileCount,FileSize,
                         CompressedFileSize) ||
        PanelMode!=PLUGIN_PANEL &&
        GetDirInfo(MSG(MDirInfoViewTitle),strcmp(CurPtr->Name,"..")==0 ? ".":CurPtr->Name,DirCount,DirFileCount,
                   FileSize,CompressedFileSize,RealFileSize,ClusterSize,0,FALSE)==1)
    {
      CurPtr->UnpSize=FileSize.LowPart;
      CurPtr->UnpSizeHigh=FileSize.HighPart;
      CurPtr->PackSize=CompressedFileSize.LowPart;
      CurPtr->PackSizeHigh=CompressedFileSize.HighPart;
      CurPtr->ShowFolderSize=1;
    }
  SortFileList(TRUE);
  ShowFileList(TRUE);
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
      PrevDataStack=(struct PrevDataItem *)realloc(PrevDataStack,(PrevDataStackSize+1)*sizeof(*PrevDataStack));
      PrevDataStack[PrevDataStackSize].PrevListData=ListData;
      PrevDataStack[PrevDataStackSize].PrevFileCount=FileCount;
      strcpy(PrevDataStack[PrevDataStackSize].PrevName,FileName);
      PrevDataStackSize++;
      ListData=NULL;
      FileCount=0;
    }
    SetPluginMode(hNewPlugin,FileName);
    PanelMode=PLUGIN_PANEL;
    UpperFolderTopFile=CurTopFile;
    CurFile=0;
    Update(0);
    Redraw();
    Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
    if (AnotherPanel->GetType()==INFO_PANEL)
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
    Panel *AnotherPanel=CtrlObject->GetAnotherPanel(this);
    if (AnotherPanel->GetType()==FILE_PANEL)
    {
      FileList *AnotherFilePanel=(FileList *)AnotherPanel;
      if (AnotherFilePanel->FileCount>0 &&
          (AnotherFilePanel->ListData[AnotherFilePanel->CurFile].FileAttr & FA_DIREC) &&
          strcmp(AnotherFilePanel->ListData[AnotherFilePanel->CurFile].Name,"..")!=0)
      {
        AnotherDir=TRUE;
        if (Drag)
        {
          AnotherPanel->ProcessKey(KEY_ENTER);
          SetCurPath();
        }
      }
    }
    if (PanelMode==PLUGIN_PANEL &&
        !CtrlObject->Plugins.UseFarCommand(hPlugin,PLUGIN_FARGETFILES))
    {
      if (Key!=KEY_ALTF6)
      {
        char PluginDestPath[NM];
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
              AnotherPanel->GetCurDir(DestPath);
            PluginGetFiles(DestPath,Move);
          }
      }
    }
    else
    {
      int ToPlugin=AnotherPanel->GetMode()==PLUGIN_PANEL &&
                    AnotherPanel->IsVisible() && Key!=KEY_ALTF6 &&
                    !CtrlObject->Plugins.UseFarCommand(AnotherPanel->GetPluginHandle(),PLUGIN_FARPUTFILES);

      ShellCopy ShCopy(this,Move,Key==KEY_ALTF6,FALSE,Ask,ToPlugin,NULL);

      if (ToPlugin==1)
        PluginPutFilesToAnother(Move,AnotherPanel);
    }
    if (AnotherDir && Drag)
      AnotherPanel->ProcessKey(KEY_ENTER);
  }
}
