/*
options.cpp

��������� �������������� ���� (����� hmenu.cpp � ����������� �����������)

*/

/* Revision: 1.06 29.04.2001 $ */

/*
Modify:
  29.04.2001 ��
    + ��������� NWZ �� ����������
  30.03.2001 SVS
    ! � OptionsDisabled() ����������� Opt.Policies.DisabledOptions, �������
      �������������� � ������� ����������� � config.cpp
  29.03.2001 IS
    + ViewerConfig ���������� � Opt.ViOpt
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  21.02.2001 IS
    + EditorConfig ���������� � Opt.EdOpt
  05.09.2000 tran
    + OptionsEnabled - reg:Policies/DisabledOptions
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   ����������� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

int OptionsDisabled(int i);

void ShellOptions(int LastCommand,MOUSE_EVENT_RECORD *MouseEvent)
{
  struct MenuData LeftMenu[]=
  {
    (char *)MMenuBriefView,1,0,0,
    (char *)MMenuMediumView,0,0,0,
    (char *)MMenuFullView,0,0,0,
    (char *)MMenuWideView,0,0,0,
    (char *)MMenuDetailedView,0,0,0,
    (char *)MMenuDizView,0,0,0,
    (char *)MMenuLongDizView,0,0,0,
    (char *)MMenuOwnersView,0,0,0,
    (char *)MMenuLinksView,0,0,0,
    (char *)MMenuAlternativeView,0,0,0,
    "",0,0,1,
    (char *)MMenuInfoPanel,0,0,0,
    (char *)MMenuTreePanel,0,0,0,
    (char *)MMenuQuickView,0,0,0,
    "",0,0,1,
    (char *)MMenuSortModes,0,0,0,
    (char *)MMenuLongNames,0,0,0,
    (char *)MMenuTogglePanel,0,0,0,
    (char *)MMenuReread,0,0,0,
    (char *)MMenuChangeDrive,0,0,0
  };

  struct MenuData FilesMenu[]=
  {
    (char *)MMenuView,1,0,0,
    (char *)MMenuEdit,0,0,0,
    (char *)MMenuCopy,0,0,0,
    (char *)MMenuMove,0,0,0,
    (char *)MMenuCreateFolder,0,0,0,
    (char *)MMenuDelete,0,0,0,
    "",0,0,1,
    (char *)MMenuAdd,0,0,0,
    (char *)MMenuExtract,0,0,0,
    (char *)MMenuArchiveCommands,0,0,0,
    "",0,0,1,
    (char *)MMenuAttributes,0,0,0,
    (char *)MMenuApplyCommand,0,0,0,
    (char *)MMenuDescribe,0,0,0,
    "",0,0,1,
    (char *)MMenuSelectGroup,0,0,0,
    (char *)MMenuUnselectGroup,0,0,0,
    (char *)MMenuInvertSelection,0,0,0,
    (char *)MMenuRestoreSelection,0,0,0
  };


  struct MenuData CmdMenu[]=
  {
    (char *)MMenuFindFile,1,0,0,
    (char *)MMenuHistory,0,0,0,
    (char *)MMenuVideoMode,0,0,0,
    (char *)MMenuFindFolder,0,0,0,
    (char *)MMenuViewHistory,0,0,0,
    (char *)MMenuFoldersHistory,0,0,0,
    "",0,0,1,
    (char *)MMenuSwapPanels,0,0,0,
    (char *)MMenuTogglePanels,0,0,0,
    (char *)MMenuCompareFolders,0,0,0,
    "",0,0,1,
    (char *)MMenuUserMenu,0,0,0,
    (char *)MMenuFileAssociations,0,0,0,
    (char *)MMenuFolderShortcuts,0,0,0,
    (char *)MMenuEditSortGroups,0,0,0,
    (char *)MMenuFilter,0,0,0,
    "",0,0,1,
    (char *)MMenuPluginCommands,0,0,0,
    (char *)MMenuWindowsList,0,0,0,
    (char *)MMenuProcessList,0,0,0
  };


  struct MenuData OptionsMenu[]=
  {
    (char *)MMenuSystemSettings,1,0,0,
    (char *)MMenuPanelSettings,0,0,0,
    (char *)MMenuInterface,0,0,0,
    (char *)MMenuLanguages,0,0,0,
    (char *)MMenuPluginsConfig,0,0,0,
    "",0,0,1,
    (char *)MMenuConfirmation,0,0,0,
    (char *)MMenuFilePanelModes,0,0,0,
    (char *)MMenuFileDescriptions,0,0,0,
    (char *)MMenuFolderInfoFiles,0,0,0,
    "",0,0,1,
    (char *)MMenuViewer,0,0,0,
    (char *)MMenuEditor,0,0,0,
    "",0,0,1,
    (char *)MMenuColors,0,0,0,
    (char *)MMenuFilesHighlighting,0,0,0,
    "",0,0,1,
    (char *)MMenuSaveSetup,0,0,0
  };


  struct MenuData RightMenu[]=
  {
    (char *)MMenuBriefView,1,0,0,
    (char *)MMenuMediumView,0,0,0,
    (char *)MMenuFullView,0,0,0,
    (char *)MMenuWideView,0,0,0,
    (char *)MMenuDetailedView,0,0,0,
    (char *)MMenuDizView,0,0,0,
    (char *)MMenuLongDizView,0,0,0,
    (char *)MMenuOwnersView,0,0,0,
    (char *)MMenuLinksView,0,0,0,
    (char *)MMenuAlternativeView,0,0,0,
    "",0,0,1,
    (char *)MMenuInfoPanel,0,0,0,
    (char *)MMenuTreePanel,0,0,0,
    (char *)MMenuQuickView,0,0,0,
    "",0,0,1,
    (char *)MMenuSortModes,0,0,0,
    (char *)MMenuLongNames,0,0,0,
    (char *)MMenuTogglePanelRight,0,0,0,
    (char *)MMenuReread,0,0,0,
    (char *)MMenuChangeDriveRight,0,0,0
  };


  struct HMenuData MainMenu[]=
  {
    MSG(MMenuLeftTitle),1,LeftMenu,sizeof(LeftMenu)/sizeof(LeftMenu[0]),"LeftRightMenu",
    MSG(MMenuFilesTitle),0,FilesMenu,sizeof(FilesMenu)/sizeof(FilesMenu[0]),"FilesMenu",
    MSG(MMenuCommandsTitle),0,CmdMenu,sizeof(CmdMenu)/sizeof(CmdMenu[0]),"CmdMenu",
    MSG(MMenuOptionsTitle),0,OptionsMenu,sizeof(OptionsMenu)/sizeof(OptionsMenu[0]),"OptMenu",
    MSG(MMenuRightTitle),0,RightMenu,sizeof(RightMenu)/sizeof(RightMenu[0]),"LeftRightMenu"
  };

  static int LastHItem=0,LastVItem=0;
  int HItem,VItem;

  switch(CtrlObject->Cp()->LeftPanel->GetType())
  {
    case FILE_PANEL:
      {
        int MenuLine=CtrlObject->Cp()->LeftPanel->GetViewMode()-VIEW_0;
        if (MenuLine<10)
          if (MenuLine==0)
            LeftMenu[9].Checked=1;
          else
            LeftMenu[MenuLine-1].Checked=1;
      }
      break;
    case INFO_PANEL:
      LeftMenu[11].Checked=1;
      break;
    case TREE_PANEL:
      LeftMenu[12].Checked=1;
      break;
    case QVIEW_PANEL:
      LeftMenu[13].Checked=1;
      break;
  }

  LeftMenu[16].Checked=!CtrlObject->Cp()->LeftPanel->GetShowShortNamesMode();

  switch(CtrlObject->Cp()->RightPanel->GetType())
  {
    case FILE_PANEL:
      {
        int MenuLine=CtrlObject->Cp()->RightPanel->GetViewMode()-VIEW_0;
        if (MenuLine<10)
          if (MenuLine==0)
            RightMenu[9].Checked=1;
          else
            RightMenu[MenuLine-1].Checked=1;
      }
      break;
    case INFO_PANEL:
      RightMenu[11].Checked=1;
      break;
    case TREE_PANEL:
      RightMenu[12].Checked=1;
      break;
    case QVIEW_PANEL:
      RightMenu[13].Checked=1;
      break;
  }


  RightMenu[16].Checked=!CtrlObject->Cp()->RightPanel->GetShowShortNamesMode();

  {
    HMenu HOptMenu(MainMenu,sizeof(MainMenu)/sizeof(MainMenu[0]));
    HOptMenu.SetHelp("Menus");
    HOptMenu.SetPosition(0,0,ScrX,0);
    if (LastCommand)
    {
      struct MenuData *VMenuTable[]={LeftMenu,FilesMenu,CmdMenu,OptionsMenu,RightMenu};
      MainMenu[0].Selected=0;
      MainMenu[LastHItem].Selected=1;
      VMenuTable[LastHItem][0].Selected=0;
      VMenuTable[LastHItem][LastVItem].Selected=1;
      HOptMenu.Show();
      ChangeMacroMode MacroMode(MACRO_MAINMENU);
      HOptMenu.ProcessKey(KEY_DOWN);
    }
    else
      if (CtrlObject->Cp()->ActivePanel==CtrlObject->Cp()->RightPanel &&
          CtrlObject->Cp()->ActivePanel->IsVisible())
      {
        MainMenu[0].Selected=0;
        MainMenu[4].Selected=1;
      }
    if (MouseEvent!=NULL)
    {
      HOptMenu.Show();
      HOptMenu.ProcessMouse(MouseEvent);
    }
    ChangeMacroMode MacroMode(MACRO_MAINMENU);
    HOptMenu.Process();
    HOptMenu.GetExitCode(HItem,VItem);
  }

  switch(HItem)
  {
    case 0:
      if (VItem>=0 && VItem<=9)
      {
        CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->LeftPanel,FILE_PANEL);
        int NewViewMode=VItem==9 ? VIEW_0:VIEW_1+VItem;
        CtrlObject->Cp()->LeftPanel->SetViewMode(NewViewMode);
      }
      else
        switch(VItem)
        {
          case 11:
            CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->LeftPanel,INFO_PANEL);
            break;
          case 12:
            CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->LeftPanel,TREE_PANEL);
            break;
          case 13:
            CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->LeftPanel,QVIEW_PANEL);
            break;
          case 15:
            CtrlObject->Cp()->LeftPanel->ProcessKey(KEY_CTRLF12);
            break;
          case 16:
            CtrlObject->Cp()->LeftPanel->ProcessKey(KEY_CTRLN);
            break;
          case 17:
            CtrlObject->ProcessKey(KEY_CTRLF1);
            break;
          case 18:
            CtrlObject->Cp()->LeftPanel->ProcessKey(KEY_CTRLR);
            break;
          case 19:
            CtrlObject->Cp()->LeftPanel->ChangeDisk();
            break;
        }
      break;
    case 1:
      switch(VItem)
      {
        case 0:
          CtrlObject->ProcessKey(KEY_F3);
          break;
        case 1:
          CtrlObject->ProcessKey(KEY_F4);
          break;
        case 2:
          CtrlObject->ProcessKey(KEY_F5);
          break;
        case 3:
          CtrlObject->ProcessKey(KEY_F6);
          break;
        case 4:
          CtrlObject->ProcessKey(KEY_F7);
          break;
        case 5:
          CtrlObject->ProcessKey(KEY_F8);
          break;
        case 7:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_SHIFTF1);
          break;
        case 8:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_SHIFTF2);
          break;
        case 9:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_SHIFTF3);
          break;
        case 11:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_CTRLA);
          break;
        case 12:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_CTRLG);
          break;
        case 13:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_CTRLZ);
          break;
        case 15:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_ADD);
          break;
        case 16:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_SUBTRACT);
          break;
        case 17:
          CtrlObject->Cp()->ActivePanel->ProcessKey(KEY_MULTIPLY);
          break;
        case 18:
          CtrlObject->Cp()->ActivePanel->RestoreSelection();
          break;
      }
      break;
    case 2:
      switch(VItem)
      {
        case 0:
          CtrlObject->ProcessKey(KEY_ALTF7);
          break;
        case 1:
          CtrlObject->ProcessKey(KEY_ALTF8);
          break;
        case 2:
          CtrlObject->ProcessKey(KEY_ALTF9);
          break;
        case 3:
          CtrlObject->ProcessKey(KEY_ALTF10);
          break;
        case 4:
          CtrlObject->ProcessKey(KEY_ALTF11);
          break;
        case 5:
          CtrlObject->ProcessKey(KEY_ALTF12);
          break;
        case 7:
          CtrlObject->ProcessKey(KEY_CTRLU);
          break;
        case 8:
          CtrlObject->ProcessKey(KEY_CTRLO);
          break;
        case 9:
          CtrlObject->Cp()->ActivePanel->CompareDir();
          break;
        case 11:
          ProcessUserMenu(1);
          break;
        case 12:
          EditFileTypes(0);
          break;
        case 13:
          ShowFolderShortcut();
          break;
        case 14:
          CtrlObject->GrpSort.EditGroups();
          break;
        case 15:
          CtrlObject->Cp()->ActivePanel->EditFilter();
          break;
        case 17:
          CtrlObject->ProcessKey(KEY_F11);
          break;
        case 18:
          CtrlObject->ProcessKey(KEY_F12);
          break;
        case 19:
          ShowProcessList();
          break;
      }
      break;
    case 3:
      /* $ 05.09.2000 tran
        + ��������� ���������� ���������
      */
      if ( !OptionsDisabled(VItem) )
        break;
      /* tran 05.09.2000 $ */
      switch(VItem)
      {
        case 0:
          SystemSettings();
          break;
        case 1:
          PanelSettings();
          break;
        case 2:
          InterfaceSettings();
          break;
        case 3:
          {
            VMenu *LangMenu,*HelpMenu;
            if (Language::Select(FALSE,&LangMenu))
            {
              Lang.Close();
              if (!Lang.Init(FarPath,MListEval))
              {
                Message(MSG_WARNING,1,"Error","Cannot load language data","Ok");
                exit(0);
              }
              Language::Select(TRUE,&HelpMenu);
              delete HelpMenu;
              LangMenu->Hide();
              CtrlObject->Plugins.ReloadLanguage();
              CtrlObject->Cp()->RedrawKeyBar();
              CtrlObject->Cp()->SetScreenPositions();
            }
            delete LangMenu;
          }
          break;
        case 4:
          CtrlObject->Plugins.Configure();
          break;
        case 6:
          SetConfirmations();
          break;
        case 7:
          FileList::SetFilePanelModes();
          break;
        case 8:
          SetDizConfig();
          break;
        case 9:
          SetFolderInfoFiles();
          break;
        case 11:
          ViewerConfig(Opt.ViOpt);
          break;
        case 12:
          EditorConfig(Opt.EdOpt);
          break;
        case 14:
          SetColors();
          break;
        case 15:
          CtrlObject->HiFiles.HiEdit(0);
          break;
        case 17:
          SaveConfig(1);
          break;
      }
      break;
    case 4:
      if (VItem>=0 && VItem<=9)
      {
        CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->RightPanel,FILE_PANEL);
        int NewViewMode=VItem==9 ? VIEW_0:VIEW_1+VItem;
        CtrlObject->Cp()->RightPanel->SetViewMode(NewViewMode);
      }
      else
        switch(VItem)
        {
          case 11:
            CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->RightPanel,INFO_PANEL);
            break;
          case 12:
            CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->RightPanel,TREE_PANEL);
            break;
          case 13:
            CtrlObject->Cp()->ChangePanelToFilled(CtrlObject->Cp()->RightPanel,QVIEW_PANEL);
            break;
          case 15:
            CtrlObject->Cp()->RightPanel->ProcessKey(KEY_CTRLF12);
            break;
          case 16:
            CtrlObject->Cp()->RightPanel->ProcessKey(KEY_CTRLN);
            break;
          case 17:
            CtrlObject->ProcessKey(KEY_CTRLF2);
            break;
          case 18:
            CtrlObject->Cp()->RightPanel->ProcessKey(KEY_CTRLR);
            break;
          case 19:
            CtrlObject->Cp()->RightPanel->ChangeDisk();
            break;
        }
      break;
  }
  CtrlObject->CmdLine->Show();
  if (HItem!=-1 && VItem!=-1)
  {
    LastHItem=HItem;
    LastVItem=VItem;
  }
}

/* $ 05.09.2000 tran
   ������� �������� ������������� ������������ */
int OptionsDisabled(int I)
{
  if (Opt.Policies.DisabledOptions)
    if ((Opt.Policies.DisabledOptions >> I) & 1)
       return FALSE;
  return TRUE;
}
/* tran 05.09.2000 $ */
