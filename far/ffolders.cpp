/*
ffolders.cpp

Folder shortcuts

*/

/* Revision: 1.14 26.04.2005 $ */

/*
Modify:
  26.04.2005 SVS
    ! � GetShortcutFolder �������� �������� - ����� ����� �� ����� ��� ����.
    + ������� GetShortcutFolderSize() - ���������� ������ ��� ShortcutN
    ! ProcessShortcutRecord() ������ ���������� ��������.
  04.11.2004 SVS
    ! ������� *_EDITPATH
  07.10.2003 SVS
    - ��������� ��������� �� ������.
  29.04.2002 SVS
    - BugZ#482 Folder shortcuts (�����������)
  27.04.2002 SVS
    ! 8192 -> MAXSIZE_SHORTCUTDATA
    - BugZ#482 - Folder shortcuts
    ! ������� �����������
    ! ���������� ������ Ins � ���������� �������.
  28.10.2001 SVS
    + Folder shortcuts - ������� ������� �� �����
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  17.06.2001 IS
    ! ExpandEnvironmentStrings -> ExpandEnvironmentStr, �.�. ������ �����������
      �������� � ���������, ���� ������� ������ 0x7F
  16.06.2001 KM
    ! ���������� WRAPMODE � ����.
  21.05.2001 SVS
    ! ��������� MENU_ - � ����
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "keys.hpp"
#include "global.hpp"
#include "lang.hpp"
#include "vmenu.hpp"
#include "cmdline.hpp"
#include "ctrlobj.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "filelist.hpp"

static int ShowFolderShortcutMenu(int Pos);
static const char HelpFolderShortcuts[]="FolderShortcuts";

enum PSCR_CMD{
  PSCR_CMDGET,
  PSCR_CMDSET,
  PSCR_CMDDELALL,
  PSCR_CMDGETFILDERSIZE,
};

enum PSCR_RECTYPE{
  PSCR_RT_SHORTCUT,
  PSCR_RT_PLUGINMODULE,
  PSCR_RT_PLUGINFILE,
  PSCR_RT_PLUGINDATA,
};

static int ProcessShortcutRecord(int Command,int ValType,int RecNumber,char *Value,int SizeValue)
{
  static const char FolderShortcuts[]="FolderShortcuts";
  static const char *RecTypeName[]={
    "Shortcut%d",
    "PluginModule%d",
    "PluginFile%d",
    "PluginData%d",
  };
  char ValueName[32];

  sprintf(ValueName,RecTypeName[ValType],RecNumber);
  if(Command == PSCR_CMDGET)
    GetRegKey(FolderShortcuts,ValueName,Value,"",SizeValue);
  else if(Command == PSCR_CMDSET)
    SetRegKey(FolderShortcuts,ValueName,NullToEmpty(Value));
  else if(Command == PSCR_CMDDELALL)
  {
    for(int I=0; I < sizeof(RecTypeName)/sizeof(RecTypeName[0]); ++I)
    {
      sprintf(ValueName,RecTypeName[I],RecNumber);
      SetRegKey(FolderShortcuts,ValueName,"");
    }
  }
  else if(Command == PSCR_CMDGETFILDERSIZE)
    return GetRegKeySize(FolderShortcuts,ValueName);
  return 0;
}

int GetShortcutFolderSize(int Key)
{
  if (Key<KEY_RCTRL0 || Key>KEY_RCTRL9)
    return 0;
  Key-=KEY_RCTRL0;

  return ProcessShortcutRecord(PSCR_CMDGETFILDERSIZE,PSCR_RT_SHORTCUT,Key,NULL,0);
  //ExpandEnvironmentStr(Folder,DestFolder,NM);
}


int GetShortcutFolder(int Key,char *DestFolder,int DestSize,
                              char *PluginModule,
                              char *PluginFile,
                              char *PluginData)
{
  if (Key<KEY_RCTRL0 || Key>KEY_RCTRL9)
    return(FALSE);

  char *Folder=new char[DestSize];
  if(!Folder)
    return FALSE;
  Key-=KEY_RCTRL0;

  ProcessShortcutRecord(PSCR_CMDGET,PSCR_RT_SHORTCUT,Key,Folder,DestSize);
  ExpandEnvironmentStr(Folder,DestFolder,DestSize);
  delete[] Folder;
  if(PluginModule)
    ProcessShortcutRecord(PSCR_CMDGET,PSCR_RT_PLUGINMODULE,Key,PluginModule,NM);
  if(PluginFile)
    ProcessShortcutRecord(PSCR_CMDGET,PSCR_RT_PLUGINFILE,Key,PluginFile,NM);
  if(PluginData)
    ProcessShortcutRecord(PSCR_CMDGET,PSCR_RT_PLUGINDATA,Key,PluginData,MAXSIZE_SHORTCUTDATA);

  return(*DestFolder || (PluginModule && *PluginModule));
}


int SaveFolderShortcut(int Key,char *SrcFolder,
                               char *PluginModule,
                               char *PluginFile,
                               char *PluginData)
{
  if (Key<KEY_CTRLSHIFT0 || Key>KEY_CTRLSHIFT9)
    return(FALSE);

  Key-=KEY_CTRLSHIFT0;
  ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_SHORTCUT,Key,SrcFolder,0);
  ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_PLUGINMODULE,Key,PluginModule,0);
  ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_PLUGINFILE,Key,PluginFile,0);
  ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_PLUGINDATA,Key,PluginData,0);

  return(TRUE);
}


void ShowFolderShortcut()
{
  int Pos=0;
  while (Pos!=-1)
    Pos=ShowFolderShortcutMenu(Pos);
}


static int ShowFolderShortcutMenu(int Pos)
{
  int ExitCode=-1;
  {
    struct MenuItem ListItem;
    VMenu FolderList(MSG(MFolderShortcutsTitle),NULL,0,ScrY-4);
    /* $ 16.06.2001 KM
       ! ���������� WRAPMODE � ����.
    */
    FolderList.SetFlags(VMENU_WRAPMODE); // VMENU_SHOWAMPERSAND|
    /* KM $ */
    FolderList.SetHelp(HelpFolderShortcuts);
    FolderList.SetPosition(-1,-1,0,0);
    FolderList.SetBottomTitle(MSG(MFolderShortcutBottom));

    for (int I=0;I<10;I++)
    {
      char ValueName[100],FolderName[NM];
      memset(&ListItem,0,sizeof(ListItem));
      ProcessShortcutRecord(PSCR_CMDGET,PSCR_RT_SHORTCUT,I,FolderName,sizeof(FolderName));
      TruncStr(FolderName,60);
      if (*FolderName==0)
      {
        ProcessShortcutRecord(PSCR_CMDGET,PSCR_RT_PLUGINMODULE,I,FolderName,sizeof(FolderName));
        if(*FolderName == 0)
          strcpy(FolderName,MSG(MShortcutNone));
        else
          strcpy(FolderName,MSG(MShortcutPlugin));
      }
      sprintf(ListItem.Name,"%s+&%d   %s",MSG(MRightCtrl),I,FolderName);
      ListItem.SetSelect(I == Pos);
      FolderList.AddItem(&ListItem);
    }

    FolderList.Show();

    while (!FolderList.Done())
    {
      int SelPos=FolderList.GetSelectPos();
      DWORD Key=FolderList.ReadInput();
      switch(Key)
      {
        case KEY_DEL:
        case KEY_INS:
        {
          ProcessShortcutRecord(PSCR_CMDDELALL,0,SelPos,NULL,0);
          if(Key == KEY_INS)
          {
            char NewDir[NM];
            Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
            CtrlObject->CmdLine->GetCurDir(NewDir);
            ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_SHORTCUT,SelPos,NewDir,0);

            if(ActivePanel->GetMode() == PLUGIN_PANEL)
            {
              struct OpenPluginInfo Info;
              ActivePanel->GetOpenPluginInfo(&Info);
              ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_PLUGINMODULE,SelPos,(char *)CtrlObject->Plugins.PluginsData[((struct PluginHandle *)ActivePanel->GetPluginHandle())->PluginNumber].ModuleName,0);
              ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_PLUGINFILE,SelPos,(char *)Info.HostFile,0);
              ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_PLUGINDATA,SelPos,(char *)Info.ShortcutData,0);
            }
          }
          return(SelPos);
        }

        case KEY_F4:
        {
          /* ���� ������� ������ ���:
             �������������� �� F4 - ������� ��� ��� ���.�������� ����!
             TODO: ����� ������� � ������ � ��������� (�������� :-)
          */
          char NewDir[NM*2], OldNewDir[NM*2];
          ProcessShortcutRecord(PSCR_CMDGET,PSCR_RT_SHORTCUT,SelPos,NewDir,sizeof(NewDir));
          strcpy(OldNewDir,NewDir);

          if (GetString(MSG(MFolderShortcutsTitle),MSG(MEnterShortcut),NULL,
                        NewDir,NewDir,sizeof(NewDir),HelpFolderShortcuts,0/*|FIB_EDITPATH*/) &&
              strcmp(NewDir,OldNewDir) != 0)
          {
            Unquote(NewDir);
            if(!(NewDir[1] == ':' && NewDir[2] == '\\' && NewDir[3] == 0))
              DeleteEndSlash(NewDir);
            BOOL Saved=TRUE;
            ExpandEnvironmentStr(NewDir,OldNewDir,sizeof(OldNewDir)-1);
            if(GetFileAttributes(OldNewDir) == -1)
            {
              TruncPathStr(NewDir, ScrX-16);
              SetLastError(ERROR_PATH_NOT_FOUND);
              Saved=(Message(MSG_WARNING | MSG_ERRORTYPE, 2, MSG (MError), NewDir, MSG(MSaveThisShortcut), MSG(MYes), MSG(MNo)) == 0);
            }

            if(Saved)
            {
              ProcessShortcutRecord(PSCR_CMDDELALL,0,SelPos,NULL,0);
              ProcessShortcutRecord(PSCR_CMDSET,PSCR_RT_SHORTCUT,SelPos,NewDir,0);
              return(SelPos);
            }
          }
          break;
        }
        default:
          FolderList.ProcessInput();
          break;
      }
    }
    ExitCode=FolderList.Modal::GetExitCode();
    FolderList.Hide();
  }

  if (ExitCode>=0)
  {
    CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_RCTRL0+ExitCode);
  }

  return(-1);
}
