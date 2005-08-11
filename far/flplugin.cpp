/*
flplugin.cpp

�������� ������ - ������ � ���������

*/

/* Revision: 1.53 11.08.2005 $ */

/*
Modify:
  11.08.2005 WARP
    ! see 02039.Mix.txt
  04.08.2005 SVS
    - ������ � �����. ����� ���������� ��� ��� ��������!
  28.07.2005 SVS
    - �� �������������� ������ ���������� ��� ������ �� ���������� �������
  25.07.2005 SVS
    - �����, ��� ������ Info.StartPanelMode as '0'+����� ������.
  22.07.2005 SVS
    - ����� ���� ��������� �������� PluginsStackSize �� ������ ClosePlugin()
    + ���� ������������ ��� PluginsStackItem.PrevViewSettings
  13.07.2005 SVS
    - ���� �� ������ (��. 02023.Mix.txt)
    ! ��� �������� ������ �� ����� ������ ������ ����������
  29.06.2005 SVS
    - BugZ#1253 - ������������ ��������� PanelMode.FullScreen
  21.04.2005 SVS
    ! ��� ������ FileList::ViewSettingsToText ����� ���������, ��� ��� ������,
      ��� ��� ��������� ��������� �������� � NM
  16.11.2004 WARP
    - FCTL_GET[ANOTHER]PANELSHORTINFO �������� ������� "������������������" �
      ����� ������ ������, ���������� ���������� ������� FCTL_GET[ANOTHER]PANELINFO.
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  24.05.2004 SVS
    - BugZ#1085 - ���� �������� ���������� �� ������
  13.11.2003 SVS
    + _ALGO()
  06.10.2003 SVS
    - BugZ#964 - ������ ��������� �� Access Denied
      �� ������������ �������� "�����"
    ! FileList::CreatePluginItemList() ����� ���.�������� - "��������� '..'?"
      �� ��������� - "���������".
      � FileList::PluginGetPanelInfo() ���� �������� = FALSE ("�� ���������")
  26.09.2003 SVS
    - BugZ#886 - FAR ������� ��������� �� ����� ���� ������ �� ����.
  04.09.2003 SVS
    ! ������ ������ CompareFileTime() �������� ���� � ������������� ������:
      �������� FILETIME � __int64
  02.09.2003 SVS
    - BugZ#937 - ���������� ������ ��������� �� Access Denied
    ! � FileList::OpenPluginForFile() ����� �������� - �������� ��������
      (��� ����, ����� ����� ��������� DIR)
  30.07.2003 SVS
    - BugZ#856 - ����� �������� ���������� ����� ����� � ������ �� �������
  08.07.2003 SVS
    - ���� ������� ������� 1 � ��� ����� ��������� � ���� ������� "..",
      �� ����� ���� � PI.SelectedItems[0] - ����� �����!
  14.05.2003 SVS
    + _ALGO()
  05.03.2003 SVS
    ! ���������� _SVS
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
    ! � FileList::PluginPutFilesToNew() ������ ���������� �������
      �������� ���������.
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  02.07.2002 SVS
    + _PluginsStackItem_Dump() - ���� ����� ��������
  25.06.2002 SVS
    ! ��� �������� ������� ���������� ����� (FileListToPluginItem) ��� ��
      ��������� ���� Owner (���� ��� ������� ���������!)
  08.05.2002 SVS
    ! �������� ������� 1248 (��� ���������� �������� Ctrl-7 Enter � ����� Ctrl-2 � �������)
  12.04.2002 SVS
    - BugZ#452 - Ctrl+N �� ���������
  12.04.2002 IS
    ! PluginPutFilesToAnother ������ int - ���������� ��, ��� ����������
      PutFiles:
      -1 - �������� ������������
       0 - �������
       1 - �����
       2 - �����, ������ ������������� ���������� �� ���� � ������ ���
           ������������� �� �����
    + PluginPutFilesToNew ��������� ��� �������� PluginPutFilesToAnother
  11.04.2002 SVS
    ! ���.�������� � PluginGetPanelInfo - �������� ������ ���� ��� �� ������
  10.04.2002 SVS
    - BugZ#353 - ������� �� ���� Shift-F3 �� �������� �� ���������� ���������� �������
  05.04.2002 SVS
    ! ������ ����� 0x20000 ������� Opt.PluginMaxReadData
  22.03.2002 SVS
    - strcpy - Fuck!
  20.03.2002 SVS
    ! GetCurrentDirectory -> FarGetCurDir
  01.03.2002 SVS
    ! ���� ������ ���� ������� �������� ���������� ����� - FarMkTempEx
  19.02.2002 SVS
    ! ����������� ������ ������� ����� ������������ ������� �� �����.
  14.01.2002 IS
    ! chdir -> FarChDir
  25.12.2001 SVS
    ! ������� ����������� (���� VC ��� ����� ��� ������, ��
      �������� ����� ������)
  12.12.2001 SVS
    - Bug: ����� ClosePlugin ���������� SortOrder �� �����-�� ���������
      �������� ���������� = -1 (���� ��������� 0 ��� 1)...
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  24.09.2001 SVS
    ! ������� ����������� (���������� ����)
  17.08.2001 VVM
    + ��������� PluginPanelItem.CRC32
  06.07.2001 IS
    + �������� ������ ��������� � PluginSetSelection � � PluginClearSelection
  14.06.2001 SVS
    - ��������� �� ������: "������� ������������� some.foo.rar � ������� some\"
  17.05.2001 SVS
    ! ������� ����������� ����� ���������� (����� doxygen ����� �� ������� :-)
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  26.04.2001 DJ
    - � ProcessHostFile() �� ����������� OPM_TOPLEVEL
  04.01.2001 SVS
    ! TranslateKeyToVK() -> keyboard.cpp
  11.11.2000 SVS
    ! FarMkTemp() - ������� (��� ������ - �� ������, �� ��� �� ������� :-(((
  11.11.2000 SVS
    ! ���������� ����������� FarMkTemp()
  08.09.2000 SVS
    + ������� � FileList::TranslateKeyToVK ��� ����������
      KEY_SHIFTDEL, KEY_ALTSHIFTDEL, KEY_CTRLSHIFTDEL
  23.07.2000 SVS
    + ������� (FileList::TranslateKeyToVK):
       Ctrl- Shift- Alt- CtrlShift- AltShift- CtrlAlt- Apps :-)
       KEY_LWIN (VK_LWIN), KEY_RWIN (VK_RWIN)
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "lang.hpp"
#include "filelist.hpp"
#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "filepanels.hpp"
#include "history.hpp"
#include "ctrlobj.hpp"
/*
   � ����� ������ ������ �� �������� - ������ ����������!
*/

void FileList::PushPlugin(HANDLE hPlugin,char *HostFile)
{
  _ALGO(CleverSysLog("FileList::PushPlugin()"));
  _ALGO(SysLog("hPlugin=%p, HostFile='%s'",hPlugin,HostFile?HostFile:"(NULL)"));
  _ALGO(PanelViewSettings_Dump("Prev",ViewSettings));
  DeleteAllDataToDelete();

  struct PluginsStackItem stItem;
  stItem.hPlugin=hPlugin;
  strcpy(stItem.HostFile,NullToEmpty(HostFile)); //??NULL??
  stItem.Modified=FALSE;
  stItem.PrevViewMode=ViewMode;
  stItem.PrevSortMode=SortMode;
  stItem.PrevSortOrder=SortOrder;
  stItem.PrevNumericSort=NumericSort;
  memmove(&stItem.PrevViewSettings,&ViewSettings,sizeof(struct PanelViewSettings));

  PluginsStack=(struct PluginsStackItem *)xf_realloc(PluginsStack,(PluginsStackSize+1)*sizeof(*PluginsStack));
  memmove(PluginsStack+PluginsStackSize,&stItem,sizeof(struct PluginsStackItem));
  PluginsStackSize++;
  _ALGO(PluginsStackItem_Dump("FileList::PushPlugin",PluginsStack,PluginsStackSize));
}

int FileList::PopPlugin(int EnableRestoreViewMode)
{
  struct OpenPluginInfo Info;
  Info.StructSize=0;

  DeleteAllDataToDelete();

  if (PluginsStackSize==0)
  {
    PanelMode=NORMAL_PANEL;
    return(FALSE);
  }

  PluginsStackSize--;

  // ��������� ������� ������.
  CtrlObject->Plugins.ClosePlugin(hPlugin);

  struct PluginsStackItem *PStack=PluginsStack+PluginsStackSize; // ��������� �� ������, � �������� ������

  if (PluginsStackSize>0)
  {
    hPlugin=PluginsStack[PluginsStackSize-1].hPlugin;

    if (EnableRestoreViewMode)
    {
      SetViewMode (PStack->PrevViewMode);

      SortMode=PStack->PrevSortMode;
      NumericSort=PStack->PrevNumericSort;
      SortOrder=PStack->PrevSortOrder;
    }

    if (PStack->Modified)
    {
      struct PluginPanelItem PanelItem;

      char SaveDir[NM];

      FarGetCurDir(sizeof(SaveDir),SaveDir);

      if (FileNameToPluginItem(PStack->HostFile,&PanelItem))
        CtrlObject->Plugins.PutFiles(hPlugin,&PanelItem,1,FALSE,0);
      else
      {
        memset(&PanelItem,0,sizeof(PanelItem));
        xstrncpy(PanelItem.FindData.cFileName,PointToName(PStack->HostFile),sizeof(PanelItem.FindData.cFileName)-1);
        CtrlObject->Plugins.DeleteFiles(hPlugin,&PanelItem,1,0);
      }

      FarChDir(SaveDir);
    }

    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);

    if ((Info.Flags & OPIF_REALNAMES)==0)
      DeleteFileWithFolder(PStack->HostFile); // �������� ����� �� ����������� �������

  }
  else
  {
    PanelMode=NORMAL_PANEL;

    if(EnableRestoreViewMode)
    {
      SetViewMode (PStack->PrevViewMode);

      SortMode=PStack->PrevSortMode;
      NumericSort=PStack->PrevNumericSort;
      SortOrder=PStack->PrevSortOrder;
    }
  }

  PluginsStack=(struct PluginsStackItem *)xf_realloc(PluginsStack,PluginsStackSize*sizeof(*PluginsStack));

  if (EnableRestoreViewMode)
    CtrlObject->Cp()->RedrawKeyBar();

  return(TRUE);
}


int FileList::FileNameToPluginItem(char *Name,PluginPanelItem *pi)
{
  char TempDir[NM],*ChPtr;
  xstrncpy(TempDir,Name,sizeof(TempDir)-1);
  if ((ChPtr=strrchr(TempDir,'\\'))==NULL)
    return(FALSE);
  *ChPtr=0;
  FarChDir(TempDir);
  memset(pi,0,sizeof(*pi));
  HANDLE FindHandle;
  FindHandle=FindFirstFile(Name,&pi->FindData);
  if (FindHandle==INVALID_HANDLE_VALUE)
    return(FALSE);
  FindClose(FindHandle);
  return(TRUE);
}


void FileList::FileListToPluginItem(struct FileListItem *fi,struct PluginPanelItem *pi)
{
  strcpy(pi->FindData.cFileName,fi->Name);
  xstrncpy(pi->FindData.cAlternateFileName,fi->ShortName,sizeof(pi->FindData.cAlternateFileName)-1);
  pi->FindData.nFileSizeHigh=fi->UnpSizeHigh;
  pi->FindData.nFileSizeLow=fi->UnpSize;
  pi->FindData.dwReserved0=pi->FindData.dwReserved1=0;
  pi->PackSizeHigh=fi->PackSizeHigh;
  pi->PackSize=fi->PackSize;
  pi->FindData.dwFileAttributes=fi->FileAttr;
  pi->FindData.ftLastWriteTime=fi->WriteTime;
  pi->FindData.ftCreationTime=fi->CreationTime;
  pi->FindData.ftLastAccessTime=fi->AccessTime;
  pi->NumberOfLinks=fi->NumberOfLinks;
  pi->Flags=fi->UserFlags;
  if (fi->Selected)
    pi->Flags|=PPIF_SELECTED;
  pi->CustomColumnData=fi->CustomColumnData;
  pi->CustomColumnNumber=fi->CustomColumnNumber;
  pi->Description=fi->DizText;
  if (fi->UserData && (fi->UserFlags & PPIF_USERDATA))
  {
    DWORD Size=*(DWORD *)fi->UserData;
    /* $ 13.07.2000 SVS
       ������� new �� malloc
    */
    pi->UserData=(DWORD)xf_malloc(Size);
    /* SVS $ */
    memcpy((void *)pi->UserData,(void *)fi->UserData,Size);
  }
  else
    pi->UserData=fi->UserData;
  pi->CRC32=fi->CRC32;
  pi->Reserved[0]=pi->Reserved[1]=0;
  pi->Owner=fi->Owner[0]?fi->Owner:NULL;
}


void FileList::PluginToFileListItem(struct PluginPanelItem *pi,struct FileListItem *fi)
{
  xstrncpy(fi->Name,pi->FindData.cFileName,sizeof(fi->Name)-1);
  xstrncpy(fi->ShortName,pi->FindData.cAlternateFileName,sizeof(fi->ShortName)-1);
  xstrncpy(fi->Owner,NullToEmpty(pi->Owner),sizeof(fi->Owner)-1);
  if (pi->Description)
  {
    fi->DizText=new char[strlen(pi->Description)+1];
    strcpy(fi->DizText,pi->Description);
    fi->DeleteDiz=TRUE;
  }
  else
    fi->DizText=NULL;
  fi->UnpSizeHigh=pi->FindData.nFileSizeHigh;
  fi->UnpSize=pi->FindData.nFileSizeLow;
  fi->PackSizeHigh=pi->PackSizeHigh;
  fi->PackSize=pi->PackSize;
  fi->FileAttr=pi->FindData.dwFileAttributes;
  fi->WriteTime=pi->FindData.ftLastWriteTime;
  fi->CreationTime=pi->FindData.ftCreationTime;
  fi->AccessTime=pi->FindData.ftLastAccessTime;
  fi->NumberOfLinks=pi->NumberOfLinks;
  fi->UserFlags=pi->Flags;

  if (pi->UserData && (pi->Flags & PPIF_USERDATA))
  {
    DWORD Size=*(DWORD *)pi->UserData;
    /* $ 13.07.2000 SVS
       ������� new �� malloc
    */
    fi->UserData=(DWORD)xf_malloc(Size);
    /* SVS $ */
    memcpy((void *)fi->UserData,(void *)pi->UserData,Size);
  }
  else
    fi->UserData=pi->UserData;
  if (pi->CustomColumnNumber>0)
  {
    fi->CustomColumnData=new LPSTR[pi->CustomColumnNumber];
    for (int I=0;I<pi->CustomColumnNumber;I++)
      if (pi->CustomColumnData!=NULL && pi->CustomColumnData[I]!=NULL)
      {
        fi->CustomColumnData[I]=new char[strlen(pi->CustomColumnData[I])+1];
        strcpy(fi->CustomColumnData[I],pi->CustomColumnData[I]);
      }
      else
        fi->CustomColumnData[I]="";
  }
  fi->CustomColumnNumber=pi->CustomColumnNumber;
  fi->CRC32=pi->CRC32;
}


HANDLE FileList::OpenPluginForFile(char *FileName,DWORD FileAttr)
{
  _ALGO(CleverSysLog clv("FileList::OpenPluginForFile()"));
  _ALGO(SysLog("FileName='%s'",(FileName?FileName:"(NULL)")));

  if(!FileName || !*FileName || (FileAttr&FA_DIREC))
    return(INVALID_HANDLE_VALUE);

  SetCurPath();

  HANDLE hFile=INVALID_HANDLE_VALUE;
  if(WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    hFile=FAR_CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE ,NULL,
                         OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN|FILE_FLAG_POSIX_SEMANTICS,
                         NULL);
  if(hFile==INVALID_HANDLE_VALUE)
    hFile=FAR_CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE ,NULL,
                         OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (hFile==INVALID_HANDLE_VALUE)
  {
    //Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MEditTitle),MSG(MCannotOpenFile),FileName,MSG(MOk));
    Message(MSG_WARNING|MSG_ERRORTYPE,1,"",MSG(MOpenPluginCannotOpenFile),FileName,MSG(MOk));
    return(INVALID_HANDLE_VALUE);
  }

  char *Buffer=new char[Opt.PluginMaxReadData];
  if(Buffer)
  {
    DWORD BytesRead;
    _ALGO(SysLog("Read %d byte(s)",Opt.PluginMaxReadData));
    if(ReadFile(hFile,Buffer,Opt.PluginMaxReadData,&BytesRead,NULL))
    {
      CloseHandle(hFile);
      _ALGO(SysLogDump("First 128 bytes",0,(LPBYTE)Buffer,128,NULL));

      _ALGO(SysLog("close AnotherPanel file"));
      CtrlObject->Cp()->GetAnotherPanel(this)->CloseFile();

      _ALGO(SysLog("call Plugins.OpenFilePlugin {"));
      HANDLE hNewPlugin=CtrlObject->Plugins.OpenFilePlugin(FileName,(unsigned char *)Buffer,BytesRead);
      _ALGO(SysLog("}"));

      delete[] Buffer;

      return(hNewPlugin);
    }
    else
    {
      delete[] Buffer;
      _ALGO(SysLogLastError());
    }
  }

  CloseHandle(hFile);
  return(INVALID_HANDLE_VALUE);
}


void FileList::CreatePluginItemList(struct PluginPanelItem *(&ItemList),int &ItemNumber,BOOL AddTwoDot)
{
  if (!ListData)
    return;

  long SaveSelPosition=GetSelPosition;
  long OldLastSelPosition=LastSelPosition;

  char SelName[NM];
  int FileAttr;
  ItemNumber=0;
  ItemList=new PluginPanelItem[SelFileCount+1];
  if (ItemList!=NULL)
  {
    memset(ItemList,0,sizeof(struct PluginPanelItem) * (SelFileCount+1));
    GetSelName(NULL,FileAttr);
    while (GetSelName(SelName,FileAttr))
      if (((FileAttr & FA_DIREC)==0 || !TestParentFolderName(SelName))
          && LastSelPosition>=0 && LastSelPosition<FileCount)
      {
        FileListToPluginItem(ListData+LastSelPosition,ItemList+ItemNumber);
        ItemNumber++;
      }

    if(AddTwoDot && !ItemNumber && (FileAttr & FA_DIREC)) // ��� ��� ".."
    {
      strcpy(ItemList->FindData.cFileName,ListData->Name);
      ItemList->FindData.dwFileAttributes=ListData->FileAttr;
      ItemNumber++;
    }
  }

  LastSelPosition=OldLastSelPosition;
  GetSelPosition=SaveSelPosition;
}


void FileList::DeletePluginItemList(struct PluginPanelItem *(&ItemList),int &ItemNumber)
{
  struct PluginPanelItem *PItemList=ItemList;
  if(PItemList)
  {
    for (int I=0;I<ItemNumber;I++,PItemList++)
      if ((PItemList->Flags & PPIF_USERDATA) && PItemList->UserData)
        xf_free((void *)PItemList->UserData);
    delete[] ItemList;
  }
}


void FileList::PluginDelete()
{
  _ALGO(CleverSysLog clv("FileList::PluginDelete()"));
  struct PluginPanelItem *ItemList;
  int ItemNumber;
  SaveSelection();
  CreatePluginItemList(ItemList,ItemNumber);
  if (ItemList!=NULL && ItemNumber>0)
  {
    if (CtrlObject->Plugins.DeleteFiles(hPlugin,ItemList,ItemNumber,0))
    {
      SetPluginModified();
      PutDizToPlugin(this,ItemList,ItemNumber,TRUE,FALSE,NULL,&Diz);
    }
    DeletePluginItemList(ItemList,ItemNumber);
    Update(UPDATE_KEEP_SELECTION);
    Redraw();
    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    AnotherPanel->Redraw();
  }
}


void FileList::PutDizToPlugin(FileList *DestPanel,struct PluginPanelItem *ItemList,
                              int ItemNumber,int Delete,int Move,DizList *SrcDiz,
                              DizList *DestDiz)
{
  _ALGO(CleverSysLog clv("FileList::PutDizToPlugin()"));
  struct OpenPluginInfo Info;
  CtrlObject->Plugins.GetOpenPluginInfo(DestPanel->hPlugin,&Info);
  if (*DestPanel->PluginDizName==0 && Info.DescrFilesNumber>0)
    strcpy(DestPanel->PluginDizName,Info.DescrFiles[0]);
  if ((Opt.Diz.UpdateMode==DIZ_UPDATE_IF_DISPLAYED && IsDizDisplayed() ||
      Opt.Diz.UpdateMode==DIZ_UPDATE_ALWAYS) && *DestPanel->PluginDizName &&
      (Info.HostFile==NULL || *Info.HostFile==0 || DestPanel->GetModalMode() ||
      GetFileAttributes(Info.HostFile)!=0xFFFFFFFF))
  {
    CtrlObject->Cp()->LeftPanel->ReadDiz();
    CtrlObject->Cp()->RightPanel->ReadDiz();

    if (DestPanel->GetModalMode())
      DestPanel->ReadDiz();

    int DizPresent=FALSE;
    for (int I=0;I<ItemNumber;I++)
      if (ItemList[I].Flags & PPIF_PROCESSDESCR)
      {
        char *Name=ItemList[I].FindData.cFileName;
        char *ShortName=ItemList[I].FindData.cAlternateFileName;
        int Code;
        if (Delete)
          Code=DestDiz->DeleteDiz(Name,ShortName);
        else
        {
          Code=SrcDiz->CopyDiz(Name,ShortName,Name,ShortName,DestDiz);
          if (Code && Move)
            SrcDiz->DeleteDiz(Name,ShortName);
        }
        if (Code)
          DizPresent=TRUE;
      }
    if (DizPresent)
    {
      char TempDir[NM],DizName[NM];
      if (FarMkTempEx(TempDir) && CreateDirectory(TempDir,NULL))
      {
        char SaveDir[NM];
        FarGetCurDir(sizeof(SaveDir),SaveDir);
        sprintf(DizName,"%s\\%s",TempDir,DestPanel->PluginDizName);
        DestDiz->Flush("",DizName);
        if (Move)
          SrcDiz->Flush("",NULL);
        struct PluginPanelItem PanelItem;
        if (FileNameToPluginItem(DizName,&PanelItem))
          CtrlObject->Plugins.PutFiles(DestPanel->hPlugin,&PanelItem,1,FALSE,OPM_SILENT|OPM_DESCR);
        else
          if (Delete)
          {
            PluginPanelItem pi;
            memset(&pi,0,sizeof(pi));
            strcpy(pi.FindData.cFileName,DestPanel->PluginDizName);
            CtrlObject->Plugins.DeleteFiles(DestPanel->hPlugin,&pi,1,OPM_SILENT);
          }
        FarChDir(SaveDir);
        DeleteFileWithFolder(DizName);
      }
    }
  }
}


void FileList::PluginGetFiles(char *DestPath,int Move)
{
  _ALGO(CleverSysLog clv("FileList::PluginGetFiles()"));
  struct PluginPanelItem *ItemList, *PList;
  int ItemNumber;
  SaveSelection();
  CreatePluginItemList(ItemList,ItemNumber);
  if (ItemList!=NULL && ItemNumber>0)
  {
    int GetCode=CtrlObject->Plugins.GetFiles(hPlugin,ItemList,ItemNumber,Move,DestPath,0);
    if (Opt.Diz.UpdateMode==DIZ_UPDATE_IF_DISPLAYED && IsDizDisplayed() ||
        Opt.Diz.UpdateMode==DIZ_UPDATE_ALWAYS)
    {
      DizList DestDiz;
      int DizFound=FALSE;
      PList=ItemList;
      for (int I=0;I<ItemNumber;I++,PList++)
        if (PList->Flags & PPIF_PROCESSDESCR)
        {
          if (!DizFound)
          {
            CtrlObject->Cp()->LeftPanel->ReadDiz();
            CtrlObject->Cp()->RightPanel->ReadDiz();
            DestDiz.Read(DestPath);
            DizFound=TRUE;
          }
          char *Name=PList->FindData.cFileName;
          char *ShortName=PList->FindData.cAlternateFileName;
          CopyDiz(Name,ShortName,Name,Name,&DestDiz);
        }
      DestDiz.Flush(DestPath);
    }
    if (GetCode==1)
    {
      if (!ReturnCurrentFile)
        ClearSelection();
      if (Move)
      {
        SetPluginModified();
        PutDizToPlugin(this,ItemList,ItemNumber,TRUE,FALSE,NULL,&Diz);
      }
    }
    else
      if (!ReturnCurrentFile)
        PluginClearSelection(ItemList,ItemNumber);
    DeletePluginItemList(ItemList,ItemNumber);
    Update(UPDATE_KEEP_SELECTION);
    Redraw();
    Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    AnotherPanel->Redraw();
  }
}


void FileList::PluginToPluginFiles(int Move)
{
  _ALGO(CleverSysLog clv("FileList::PluginToPluginFiles()"));
  struct PluginPanelItem *ItemList;
  int ItemNumber;
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  char TempDir[NM];
  if (AnotherPanel->GetMode()!=PLUGIN_PANEL)
    return;
  FileList *AnotherFilePanel=(FileList *)AnotherPanel;

  if (!FarMkTempEx(TempDir))
    return;
  SaveSelection();
  CreateDirectory(TempDir,NULL);
  CreatePluginItemList(ItemList,ItemNumber);
  if (ItemList!=NULL && ItemNumber>0)
  {
    int PutCode=CtrlObject->Plugins.GetFiles(hPlugin,ItemList,ItemNumber,FALSE,TempDir,OPM_SILENT);
    if (PutCode==1 || PutCode==2)
    {
      char SaveDir[NM];
      FarGetCurDir(sizeof(SaveDir),SaveDir);
      FarChDir(TempDir);
      PutCode=CtrlObject->Plugins.PutFiles(AnotherFilePanel->hPlugin,ItemList,ItemNumber,FALSE,0);
      if (PutCode==1 || PutCode==2)
      {
        if (!ReturnCurrentFile)
          ClearSelection();
        AnotherPanel->SetPluginModified();
        PutDizToPlugin(AnotherFilePanel,ItemList,ItemNumber,FALSE,FALSE,&Diz,&AnotherFilePanel->Diz);
        if (Move)
          if (CtrlObject->Plugins.DeleteFiles(hPlugin,ItemList,ItemNumber,OPM_SILENT))
          {
            SetPluginModified();
            PutDizToPlugin(this,ItemList,ItemNumber,TRUE,FALSE,NULL,&Diz);
          }
      }
      else
        if (!ReturnCurrentFile)
          PluginClearSelection(ItemList,ItemNumber);
      FarChDir(SaveDir);
    }
    DeleteDirTree(TempDir);
    DeletePluginItemList(ItemList,ItemNumber);
    Update(UPDATE_KEEP_SELECTION);
    Redraw();
    if (PanelMode==PLUGIN_PANEL)
      AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    else
      AnotherPanel->Update(UPDATE_KEEP_SELECTION);
    AnotherPanel->Redraw();
  }
}


void FileList::PluginHostGetFiles()
{
  _ALGO(CleverSysLog clv("FileList::PluginHostGetFiles()"));
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  char DestPath[NM],SelName[NM],*ExtPtr;
  int FileAttr;

  SaveSelection();

  GetSelName(NULL,FileAttr);
  if (!GetSelName(SelName,FileAttr))
    return;

  AnotherPanel->GetCurDir(DestPath);
  if ((!AnotherPanel->IsVisible() || AnotherPanel->GetType()!=FILE_PANEL) &&
      SelFileCount==0 || *DestPath==0)
  {
    strcpy(DestPath,PointToName(SelName));
    // SVS: � ����� ����� ����� ����� ����� � ������?
    if ((ExtPtr=strrchr(DestPath,'.'))!=NULL)
      *ExtPtr=0;
  }

  int OpMode=OPM_TOPLEVEL,ExitLoop=FALSE;
  GetSelName(NULL,FileAttr);
  while (!ExitLoop && GetSelName(SelName,FileAttr))
  {
    HANDLE hCurPlugin;
    _ALGO(SysLog("call OpenPluginForFile('%s')",NullToEmpty(SelName)));
    if ((hCurPlugin=OpenPluginForFile(SelName,FileAttr))!=INVALID_HANDLE_VALUE &&
        hCurPlugin!=(HANDLE)-2)
    {
      struct PluginPanelItem *ItemList;
      int ItemNumber;
      _ALGO(SysLog("call Plugins.GetFindData()"));
      if (CtrlObject->Plugins.GetFindData(hCurPlugin,&ItemList,&ItemNumber,0))
      {
        _ALGO(SysLog("call Plugins.GetFiles()"));
        ExitLoop=CtrlObject->Plugins.GetFiles(hCurPlugin,ItemList,ItemNumber,FALSE,DestPath,OpMode)!=1;
        if (!ExitLoop)
        {
          _ALGO(SysLog("call ClearLastGetSelection()"));
          ClearLastGetSelection();
        }
        _ALGO(SysLog("call Plugins.FreeFindData()"));
        CtrlObject->Plugins.FreeFindData(hCurPlugin,ItemList,ItemNumber);
        OpMode|=OPM_SILENT;
      }
      _ALGO(SysLog("call Plugins.ClosePlugin"));
      CtrlObject->Plugins.ClosePlugin(hCurPlugin);
    }
  }
  Update(UPDATE_KEEP_SELECTION);
  Redraw();
  AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
  AnotherPanel->Redraw();
}


void FileList::PluginPutFilesToNew()
{
  _ALGO(CleverSysLog clv("FileList::PluginPutFilesToNew()"));
  //_ALGO(SysLog("FileName='%s'",(FileName?FileName:"(NULL)")));
  _ALGO(SysLog("call Plugins.OpenFilePlugin(NULL,NULL,0)"));
  HANDLE hNewPlugin=CtrlObject->Plugins.OpenFilePlugin(NULL,NULL,0);
  if (hNewPlugin!=INVALID_HANDLE_VALUE && hNewPlugin!=(HANDLE)-2)
  {
    _ALGO(SysLog("Create: FileList TmpPanel, FileCount=%d",FileCount));
    FileList TmpPanel;
    TmpPanel.SetPluginMode(hNewPlugin,"");
    TmpPanel.SetModalMode(TRUE);
    int PrevFileCount=FileCount;
    /* $ 12.04.2002 IS
       ���� PluginPutFilesToAnother ������� �����, �������� �� 2, �� �����
       ����������� ���������� ������ �� ��������� ����.
    */
    int rc=PluginPutFilesToAnother(FALSE,&TmpPanel);
    if (rc!=2 && FileCount==PrevFileCount+1)
    /* IS $ */
    {
      int LastPos=0;
      /* �����, ��� ����������� ���������� ��������������� �����
         ���������������� ���������� �� ���� � ������������ �����
         �������� �����. ������, ���� �����-�� ������� �������� ������
         � ������� �������� ����� � ����� �������� ������� �������,
         �� ����������� ���������������� �� ����������!
      */
      struct FileListItem *PtrListData=ListData+1, *PtrLastPos=ListData;
      for (int I=1; I < FileCount; I++,PtrListData++)
      {
        if ((*(__int64*)&PtrListData->CreationTime - *(__int64*)&PtrLastPos->CreationTime) > 0)
        {
          PtrLastPos=ListData+(LastPos=I);
        }
      }
      CurFile=LastPos;
      Redraw();
    }
  }
}


/* $ 12.04.2002 IS
     PluginPutFilesToAnother ������ int - ���������� ��, ��� ����������
     PutFiles:
     -1 - �������� ������������
      0 - �������
      1 - �����
      2 - �����, ������ ������������� ���������� �� ���� � ������ ���
          ������������� �� ����� (��. PluginPutFilesToNew)
*/
int FileList::PluginPutFilesToAnother(int Move,Panel *AnotherPanel)
{
  if (AnotherPanel->GetMode()!=PLUGIN_PANEL)
    return 0;
  FileList *AnotherFilePanel=(FileList *)AnotherPanel;
  struct PluginPanelItem *ItemList;
  int ItemNumber,PutCode=0;
  SaveSelection();
  CreatePluginItemList(ItemList,ItemNumber);
  if (ItemList!=NULL && ItemNumber>0)
  {
    SetCurPath();
    _ALGO(SysLog("call Plugins.PutFiles"));
    PutCode=CtrlObject->Plugins.PutFiles(AnotherFilePanel->hPlugin,ItemList,ItemNumber,Move,0);
    if (PutCode==1 || PutCode==2)
    {
      if (!ReturnCurrentFile)
      {
        _ALGO(SysLog("call ClearSelection()"));
        ClearSelection();
      }
      _ALGO(SysLog("call PutDizToPlugin"));
      PutDizToPlugin(AnotherFilePanel,ItemList,ItemNumber,FALSE,Move,&Diz,&AnotherFilePanel->Diz);
      AnotherPanel->SetPluginModified();
    }
    else
      if (!ReturnCurrentFile)
        PluginClearSelection(ItemList,ItemNumber);
    _ALGO(SysLog("call DeletePluginItemList"));
    DeletePluginItemList(ItemList,ItemNumber);
    Update(UPDATE_KEEP_SELECTION);
    Redraw();
    if (AnotherPanel==CtrlObject->Cp()->GetAnotherPanel(this))
    {
      AnotherPanel->Update(UPDATE_KEEP_SELECTION);
      AnotherPanel->Redraw();
    }
  }
  return PutCode;
}


void FileList::GetPluginInfo(struct PluginInfo *Info)
{
  _ALGO(CleverSysLog clv("FileList::GetPluginInfo()"));
  memset(Info,0,sizeof(*Info));
  if (PanelMode==PLUGIN_PANEL)
  {
    struct PluginHandle *ph=(struct PluginHandle *)hPlugin;
    CtrlObject->Plugins.GetPluginInfo(ph->PluginNumber,Info);
  }
}


void FileList::GetOpenPluginInfo(struct OpenPluginInfo *Info)
{
  _ALGO(CleverSysLog clv("FileList::GetOpenPluginInfo()"));
  //_ALGO(SysLog("FileName='%s'",(FileName?FileName:"(NULL)")));
  memset(Info,0,sizeof(*Info));
  if (PanelMode==PLUGIN_PANEL)
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,Info);
}


/*
   ������� ��� ������ ������� "�������� �������" (Shift-F3)
*/
void FileList::ProcessHostFile()
{
  _ALGO(CleverSysLog clv("FileList::ProcessHostFile()"));
  //_ALGO(SysLog("FileName='%s'",(FileName?FileName:"(NULL)")));
  if (FileCount>0 && SetCurPath())
  {
    int Done=FALSE;

    SaveSelection();

    if (PanelMode==PLUGIN_PANEL && *PluginsStack[PluginsStackSize-1].HostFile)
    {
      struct PluginPanelItem *ItemList;
      int ItemNumber;
      _ALGO(SysLog("call CreatePluginItemList"));
      CreatePluginItemList(ItemList,ItemNumber);
      _ALGO(SysLog("call Plugins.ProcessHostFile"));
      Done=CtrlObject->Plugins.ProcessHostFile(hPlugin,ItemList,ItemNumber,0);
      if (Done)
        SetPluginModified();
      else
      {
        if (!ReturnCurrentFile)
          PluginClearSelection(ItemList,ItemNumber);
        Redraw();
      }
      _ALGO(SysLog("call DeletePluginItemList"));
      DeletePluginItemList(ItemList,ItemNumber);
      if (Done)
        ClearSelection();
    }
    else
    {
      int SCount=GetRealSelCount();
      if(SCount > 0)
      {
        struct FileListItem *CurPtr=ListData;
        for(int I=0; I < FileCount; ++I, CurPtr++)
        {
          if (CurPtr->Selected)
          {
            Done=ProcessOneHostFile(I);
            if(Done == 1)
              Select(CurPtr,0);
            else if(Done == -1)
              continue;
            else       // ���� ��� ������, ��... ����� ���� ESC �� ������ ������
              break;   //
          }
        }

        if (SelectedFirst)
          SortFileList(TRUE);
      }
      else
      {
        if((Done=ProcessOneHostFile(CurFile)) == 1)
         ClearSelection();
      }
    }

    if (Done)
    {
      Update(UPDATE_KEEP_SELECTION);
      Redraw();
      Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
      AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
      AnotherPanel->Redraw();
    }
  }
}

/*
  ��������� ������ ����-�����.
  Return:
    -1 - ���� ���� ������� �������� �� ���������
     0 - ������ ������ FALSE
     1 - ������ ������ TRUE
*/
int FileList::ProcessOneHostFile(int Idx)
{
  _ALGO(CleverSysLog clv("FileList::ProcessOneHostFile()"));
  int Done=-1;

  _ALGO(SysLog("call OpenPluginForFile([Idx=%d] '%s')",Idx,ListData[Idx].Name));
  HANDLE hNewPlugin=OpenPluginForFile(ListData[Idx].Name,ListData[Idx].FileAttr);

  if (hNewPlugin!=INVALID_HANDLE_VALUE && hNewPlugin!=(HANDLE)-2)
  {
    struct PluginPanelItem *ItemList;
    int ItemNumber;
    _ALGO(SysLog("call Plugins.GetFindData"));
    if (CtrlObject->Plugins.GetFindData(hNewPlugin,&ItemList,&ItemNumber,OPM_TOPLEVEL))
    {
      /* $ 26.04.2001 DJ
         � ProcessHostFile �� ����������� OPM_TOPLEVEL
      */
      _ALGO(SysLog("call Plugins.ProcessHostFile"));
      Done=CtrlObject->Plugins.ProcessHostFile(hNewPlugin,ItemList,ItemNumber,OPM_TOPLEVEL);
      /* DJ $ */
      _ALGO(SysLog("call Plugins.FreeFindData"));
      CtrlObject->Plugins.FreeFindData(hNewPlugin,ItemList,ItemNumber);
    }
    _ALGO(SysLog("call Plugins.ClosePlugin"));
    CtrlObject->Plugins.ClosePlugin(hNewPlugin);
  }
  return Done;
}



void FileList::SetPluginMode(HANDLE hPlugin,char *PluginFile)
{
  if (PanelMode!=PLUGIN_PANEL)
    CtrlObject->FolderHistory->AddToHistory(CurDir,NULL,0);

  PushPlugin(hPlugin,PluginFile);

  FileList::hPlugin=hPlugin;
  PanelMode=PLUGIN_PANEL;
  struct OpenPluginInfo Info;
  CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
  if (Info.StartPanelMode)
    SetViewMode(VIEW_0+Info.StartPanelMode-'0');
  CtrlObject->Cp()->RedrawKeyBar();
  if (Info.StartSortMode)
  {
    SortMode=Info.StartSortMode-(SM_UNSORTED-UNSORTED);
    SortOrder=Info.StartSortOrder ? -1:1;
  }
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  if (AnotherPanel->GetType()!=FILE_PANEL)
  {
    AnotherPanel->Update(UPDATE_KEEP_SELECTION);
    AnotherPanel->Redraw();
  }
}


void FileList::PluginGetPanelInfo(struct PanelInfo *Info,int FullInfo)
{
//  DeleteAllDataToDelete();
  Info->PanelItems=NULL;
  Info->SelectedItems=NULL;

  if(FullInfo)
  {
    DeleteAllDataToDelete();

    Info->ItemsNumber=0;
    Info->PanelItems=new PluginPanelItem[FileCount+1];
    if (Info->PanelItems!=NULL)
    {
      struct FileListItem *CurPtr=ListData;
      for (int I=0; I < FileCount; I++, CurPtr++)
      {
        FileListToPluginItem(CurPtr,Info->PanelItems+Info->ItemsNumber);
        Info->ItemsNumber++;
      }
    }
    DataToDelete[DataToDeleteCount]=Info->PanelItems;
    DataSizeToDelete[DataToDeleteCount++]=Info->ItemsNumber;

    CreatePluginItemList(Info->SelectedItems,Info->SelectedItemsNumber,FALSE);

    DataToDelete[DataToDeleteCount]=Info->SelectedItems;
    DataSizeToDelete[DataToDeleteCount++]=Info->SelectedItemsNumber;
  }
  else
  {
    Info->ItemsNumber=FileCount;
    Info->SelectedItemsNumber=GetSelCount();
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
    /* ��� �� ���� ����� �� ������! */
    DataToDelete[DataToDeleteCount]=NULL;
    DataSizeToDelete[DataToDeleteCount++]=0;
    DataToDelete[DataToDeleteCount]=NULL;
    DataSizeToDelete[DataToDeleteCount++]=0;
    /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
  }

  Info->CurrentItem=CurFile;
  Info->TopPanelItem=CurTopFile;

  char ColumnTypes[NM],ColumnWidths[NM];
  ViewSettingsToText(ViewSettings.ColumnType,ViewSettings.ColumnWidth,
                     ViewSettings.ColumnCount,ColumnTypes,ColumnWidths);
  xstrncpy(Info->ColumnTypes,ColumnTypes,sizeof(Info->ColumnTypes)-1);
  xstrncpy(Info->ColumnWidths,ColumnWidths,sizeof(Info->ColumnWidths)-1);
  Info->ShortNames=ShowShortNames;
}


void FileList::PluginSetSelection(struct PanelInfo *Info)
{
  /* $ 06.07.2001 IS �������� ������ ��������� */
  SaveSelection();
  /* IS $ */
  struct FileListItem *CurPtr=ListData;
  for (int I=0; I < FileCount && I < Info->ItemsNumber; I++, CurPtr++)
  {
    int Selection=(Info->PanelItems[I].Flags & PPIF_SELECTED)!=0;
    Select(CurPtr,Selection);
  }
  if (SelectedFirst)
    SortFileList(TRUE);
}


void FileList::ProcessPluginCommand()
{
  _ALGO(CleverSysLog clv("FileList::ProcessPluginCommand"));
  _ALGO(SysLog("PanelMode=%s",(PanelMode==PLUGIN_PANEL?"PLUGIN_PANEL":"NORMAL_PANEL")));
  int Command=PluginCommand;
  PluginCommand=-1;
  if (PanelMode==PLUGIN_PANEL)
    switch(Command)
    {
      case FCTL_CLOSEPLUGIN:
        _ALGO(SysLog("Command=FCTL_CLOSEPLUGIN"));
        SetCurDir((char *)PluginParam,TRUE);
        if(!PluginParam || !*(char *)PluginParam)
          Update(UPDATE_KEEP_SELECTION);
        Redraw();
        break;
    }
}

void FileList::SetPluginModified()
{
  if (PluginsStackSize>0)
    PluginsStack[PluginsStackSize-1].Modified=TRUE;
}


HANDLE FileList::GetPluginHandle()
{
  return(hPlugin);
}


int FileList::ProcessPluginEvent(int Event,void *Param)
{
  if (PanelMode==PLUGIN_PANEL)
    return(CtrlObject->Plugins.ProcessEvent(hPlugin,Event,Param));
  return(FALSE);
}


void FileList::PluginClearSelection(struct PluginPanelItem *ItemList,int ItemNumber)
{
  /* $ 06.07.2001 IS �������� ������ ��������� */
  SaveSelection();
  /* IS $ */
  int FileNumber=0,PluginNumber=0;
  while (PluginNumber<ItemNumber)
  {
    struct PluginPanelItem *CurPluginPtr=ItemList+PluginNumber;
    if ((CurPluginPtr->Flags & PPIF_SELECTED)==0)
    {
      while (LocalStricmp(CurPluginPtr->FindData.cFileName,ListData[FileNumber].Name)!=0)
        if (++FileNumber>=FileCount)
          return;
      Select(&ListData[FileNumber++],0);
    }
    PluginNumber++;
  }
}
