/*
usermenu.cpp

User menu � ����

*/

/* Revision: 1.33 11.07.2001 $ */

/*
Modify:
  11.07.2001 SVS
    ! ���� ����� "Register" � LNG-����� ���� (������ ������), �� �� �������
      � ��������� ���� ������� ������
    ! ���� ����� ������, �� ������ ������ '&' �� �������.
  10.07.2001 SVS
    - ���������� ����� � �������������
  02.07.2001 SVS
    - Bug (����� 688): F9 c m l - �� ���������� ���� "FarMenu.Ini"
  25.06.2001 SVS
    ! ������� �������� "if exist" ����� ������ :-))
  19.06.2001 SVS
    + ������ ������: ������������� "if exist"
  17.06.2001 IS
    - ���: ���������� �����-�� ����� ��, ��� ���������� �� ������, � ������ -
      �����������.
  07.06.2001 SVS
    ! ��������� ��������������� ��������� ���������� "rem" � "::"
  05.06.2001 KM
    ! ����������. UserMenu �� ��������� ���� VMENU_WRAPMODE.
  30.05.2001 OT
    - ������ � ���������� ������� ����� ����������� ��� ������ � ���������� ���� :))
  29.01.2001 VVM
    - ����� ������ ����������� ALT+F4, ���� � �� ������...
  28.05.2001 OT
    - ����������� ALTF4 � ����
    ! ���������� ���� ������ ���������� ���������, � ������������ � NFZ
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
  07.05.2001 SVS
    ! '��� ��������� � ������ -a. � ���� �� F2 ������ � ������� ������
      ������������ ".". �������� �� ">".'
  06.05.2001 DJ
    ! �������� #include
  01.05.2001 IS
    ! ������ ��� ���������� ������ �� ���� ������ � �� ���������� ��
      �����������.
  29.04.2001 ��
    + ��������� NWZ �� ����������
  25.04.2001 DJ
    * ����� ��������� EC_COMMAND_EXECUTED; �� ��������� ������, ����
      ���� ���� ������� ��� ������ �������
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  27.02.2001 VVM
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  17.01.2001 SVS
    - ������ ������� ���������� ��������� � ����� � ��������� ���������� ������
  09.01.2001 SVS
    - ��� �������������� (Key>=KEY_F1 && Key<=KEY_F12) � ������ �������
      ����� �������� ��������� (Key-KEY_F1) �� 8 ������.
  22.12.2000 IS
    ! ���� �� ����� ����� � ������ "����������", �� �� ������� �� �������
      �������������� ������, �.�. ������ �� �������, ���, ��������, �����.
      ��� ������ �� ���� ����� ��������������� esc ��� ������� "��������".
  11.11.2000 SVS
    ! FarMkTemp() - ������� (��� ������ - �� ������, �� ��� �� ������� :-(((
  11.11.2000 SVS
    ! ���������� ����������� FarMkTemp()
  14.10.2000 VVM
    + ����������� ����, ���� ����� �����, � ������="-"
  02.09.2000 tran
    - !@!, !#!@! bug
  28.07.2000 VVM
    + ��������� ���������� ��������� � �������� ����
    - ��������� ��� � �������� BkSpace
  24.07.2000 VVM
    + ��� ������ �������� ���� � ��������� ��������� ��� - FAR/Registry
  20.07.2000 tran 1.04
    - Bug#19
      ">" ������������ ������� ������������� �� ������������ �������
  17.07.2000 VVM
    + ��� ������ ������ �� ���� ���� �� ������������� ��������
    + SHIFT+F2 ����������� ������� ����/��������� � �����
  14.07.2000 VVM
    + ����� �������� ���� �� SHIFT+F2
    + ����� ���� �� ������������� �������� �� BkSpace
  28.06.2000 tran
    + ����� �� ���������������� ���� ������ ������ �����������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "global.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "cmdline.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "fileedit.hpp"
#include "plognmn.hpp"
#include "savefpos.hpp"
#include "ctrlobj.hpp"
#include "manager.hpp"

static int ProcessSingleMenu(char *MenuKey,int MenuPos);
static int DeleteMenuRecord(char *MenuKey,int DeletePos);
static int EditMenuRecord(char *MenuKey,int EditPos,int TotalRecords,int NewRec);
static int EditSubMenu(char *MenuKey,int EditPos,int TotalRecords,int NewRec);
static void MenuRegToFile(char *MenuKey,FILE *MenuFile);
static void MenuFileToReg(char *MenuKey,FILE *MenuFile);

static int MenuModified;
static char MenuRootKey[100],LocalMenuKey[100];

/* $ 14.07.2000 VVM
   + ������ ������ ���� (Menu mode) � ���� ������ �� ���� (Exit codes)
*/
enum {MM_LOCAL=0,           // ��������� ����
      MM_FAR=1,             // ���� �� �������� ����
      MM_MAIN=2};           // ������� ����

/* $ 25.04.2001 DJ
   ����� ��������� EC_COMMAND_SELECTED
*/
enum {EC_CLOSE_LEVEL      = -1,   // ����� �� ���� �� ���� ������� �����
      EC_CLOSE_MENU       = -2,   // ����� �� ���� �� SHIFT+F10
      EC_PARENT_MENU      = -3,   // �������� ���� ������������� ��������
      EC_MAIN_MENU        = -4,   // �������� ������� ����
      EC_COMMAND_SELECTED = -5};  // ������� ������� - ������� ���� �
                                  // �������� �����
/* DJ $ */

static int MenuMode;
/* VVM $ */

static char SubMenuSymbol[]={0x020,0x010,0x000};

#define LocalMenuFileName "FarMenu.Ini"

void ProcessUserMenu(int EditMenu)
{
/* $ 14.07.2000 VVM
   + ����� ��� �������� � ������ ���������. ������ � � ������� :)
*/
  FILE *MenuFile;
//  char MenuFileName[NM];
//  char MenuKey[512];
//  char CurDir[NM];
  char MenuFilePath[NM];    // ���� � �������� �������� � ������ LocalMenuFileName
  char *ChPtr;
  int  ExitCode = 0;
  int RunFirst  = 1;
/* VVM $ */

  CtrlObject->CmdLine->GetCurDir(MenuFilePath);
/* $ 14.07.2000 VVM
  ! ������ ���� ������ �� ���� - ����� ������.
*/
//  if (chdir(MenuFilePath)==-1)
//    chdir(FarPath);

//  FILE *MenuFile;

//  sprintf(LocalMenuName,"LocalMenu%u",clock());
  sprintf(LocalMenuKey,"UserMenu\\LocalMenu%u",clock());
  MenuMode=MM_LOCAL;

  DeleteKeyTree(LocalMenuKey);
//  char MenuFileName[NM];

//  strcpy(MenuFileName,LocalMenuFileName);

//  MainMenuTitle=MainMenu=TRUE;
/* VVM $ */

  MenuModified=FALSE;

  if (EditMenu)
  {
    int EditChoice=Message(0,3,MSG(MUserMenuTitle),MSG(MChooseMenuType),
                   MSG(MChooseMenuMain),MSG(MChooseMenuLocal),MSG(MCancel));
    if (EditChoice<0 || EditChoice==2)
      return;
    if (EditChoice==0)
    {
      MenuMode=MM_FAR;
      strcpy(MenuFilePath, FarPath);
    } /* if */
  }
/* $ 14.07.2000 VVM
   + ����� ��������� ��������� �������� ������� ProcessUserMenu. �������� ����.
*/
/* $ 25.04.2001 DJ
   ��������� EC_COMMAND_EXECUTED
*/
  while((ExitCode != EC_CLOSE_LEVEL) && (ExitCode != EC_CLOSE_MENU) &&
      (ExitCode != EC_COMMAND_SELECTED))
/* DJ $ */
  {

    if (MenuMode!=MM_MAIN)
    {
      // �������� ������� ���� �� ��������� �����
      if ((chdir(MenuFilePath)==0) &&
         ((MenuFile=fopen(LocalMenuFileName,"rb"))!=NULL))
      {
        MenuFileToReg(LocalMenuKey, MenuFile);
        fclose(MenuFile);
      } /* if */
      else
      // ���� �� ��������. ������� ������.
        if (MenuMode==MM_FAR)
          MenuMode=MM_MAIN;
        else
          if (!EditMenu)
          {
/* $ 14.07.2000 VVM
    + ��� ������ ������ �� ���� ���� �� ������������� ��������
*/
/* $ 28.07.2000 VVM
    + ������ ���� ��� ������� ������
*/
            if (!RunFirst)
            {
              ChPtr=strrchr(MenuFilePath, '\\');
              if (ChPtr!=NULL)
              {
                *(ChPtr--)=0;
                if (*ChPtr!=':')
                  continue;
              } /* if */
            } /* if */
            RunFirst=0;
/* VVM $ */
            strcpy(MenuFilePath, FarPath);
            MenuMode=MM_FAR;
            continue;
          } /* if */
    } /* if */

    strcpy(MenuRootKey,(MenuMode==MM_MAIN) ? "UserMenu\\MainMenu":LocalMenuKey);
    ExitCode=ProcessSingleMenu(MenuRootKey, 0);

    // ��������� ����� �� ������ �����
    if ((MenuMode!=MM_MAIN) && (MenuModified))
    {
      chdir(MenuFilePath);
      int FileAttr=GetFileAttributes(LocalMenuFileName);
      if (FileAttr!=-1)
      {
        if (FileAttr & FA_RDONLY)
        {
          int AskOverwrite;
          AskOverwrite=Message(MSG_WARNING,2,MSG(MUserMenuTitle),LocalMenuFileName,
                       MSG(MEditRO),MSG(MEditOvr),MSG(MYes),MSG(MNo));
          if (AskOverwrite==0)
            SetFileAttributes(LocalMenuFileName,FileAttr & ~FA_RDONLY);
        }
        if (FileAttr & (FA_HIDDEN|FA_SYSTEM))
          SetFileAttributes(LocalMenuFileName,0);
      }
      if ((MenuFile=fopen(LocalMenuFileName,"wb"))!=NULL)
      {
        MenuRegToFile(LocalMenuKey,MenuFile);
        long Length=filelen(MenuFile);
        fclose(MenuFile);
        if (Length==0)
          remove(LocalMenuFileName);
      }
    }
    if (MenuMode!=MM_MAIN)
      DeleteKeyTree(LocalMenuKey);

    switch(ExitCode)
    {
      case EC_PARENT_MENU:
      {
        if (MenuMode==MM_LOCAL)
        {
          ChPtr=strrchr(MenuFilePath, '\\');
          if (ChPtr!=NULL)
          {
            *(ChPtr--)=0;
            if (*ChPtr!=':')
              continue;
          } /* if */
          strcpy(MenuFilePath, FarPath);
          MenuMode=MM_FAR;
        } /* if */
        else
          MenuMode=MM_MAIN;
        break;
      }
      case EC_MAIN_MENU:
      {
/* $ 14.07.2000 VVM
    + SHIFT+F2 ����������� ������� ����/��������� � �����
*/
        switch(MenuMode)
        {
          case MM_LOCAL:
          {
            strcpy(MenuFilePath, FarPath);
            MenuMode=MM_FAR;
            break;
          }
          case MM_FAR:
          {
            MenuMode=MM_MAIN;
            break;
          }

          default: // MM_MAIN
          {
            CtrlObject->CmdLine->GetCurDir(MenuFilePath);
            MenuMode=MM_LOCAL;
          }
        } /* switch */
//      if (MenuMode==MM_LOCAL)
//      {
//        strcpy(MenuFilePath, FarPath);
//        MenuMode=MM_FAR;
//      }
//      else
//        MenuMode=MM_MAIN;
/* VVM $ */
        break;
      } /* case */
    } /* switch */
  } /* while */

  CtrlObject->CmdLine->GetCurDir(MenuFilePath);
  chdir(MenuFilePath);
  /* $ 25.04.2001 DJ
     �� �������������� ������, ���� ������������ ������ �� ������
     � ����
  */
  if (ExitCode == EC_COMMAND_SELECTED || MenuModified)
  {
    CtrlObject->Cp()->ActivePanel->Update(UPDATE_KEEP_SELECTION);
    CtrlObject->Cp()->ActivePanel->Redraw();
  }
  /* DJ $ */

/*
  if ((!EditMenu || !MainMenu) && (MenuFile=fopen(MenuFileName,"rb"))!=NULL)
  {
    MenuFileToReg(LocalMenuKey,MenuFile);
    fclose(MenuFile);
    MainMenuTitle=MainMenu=FALSE;
  }
  else
    if (!EditMenu || MainMenu)
    {
      sprintf(MenuFileName,"%s%s",FarPath,LocalMenuFileName);
      if ((MenuFile=fopen(MenuFileName,"rb"))!=NULL)
      {
        MenuFileToReg(LocalMenuKey,MenuFile);
        fclose(MenuFile);
        MainMenu=FALSE;
      }
    }

  char MenuKey[512];
  strcpy(MenuKey,MainMenu ? "MainMenu":LocalMenuName);
  sprintf(MenuRootKey,"UserMenu\\%s",MenuKey);
  ProcessSingleMenu(MenuKey,0);

  chdir(CurDir);

  if (!MainMenu && MenuModified)
  {
    int FileAttr=GetFileAttributes(MenuFileName);
    if (FileAttr!=-1)
    {
      if (FileAttr & FA_RDONLY)
      {
        int AskOverwrite;
        AskOverwrite=Message(MSG_WARNING,2,MSG(MUserMenuTitle),MenuFileName,
                     MSG(MEditRO),MSG(MEditOvr),MSG(MYes),MSG(MNo));
        if (AskOverwrite==0)
          SetFileAttributes(MenuFileName,FileAttr & ~FA_RDONLY);
      }
      if (FileAttr & (FA_HIDDEN|FA_SYSTEM))
        SetFileAttributes(MenuFileName,0);
    }
    if ((MenuFile=fopen(MenuFileName,"wb"))!=NULL)
    {
      MenuRegToFile(LocalMenuKey,MenuFile);
      long Length=filelen(MenuFile);
      fclose(MenuFile);
      if (Length==0)
        remove(LocalMenuFileName);
      CtrlObject->Cp()->ActivePanel->Update(UPDATE_KEEP_SELECTION);
      CtrlObject->Cp()->ActivePanel->Redraw();
    }
  }
  if (!MainMenu)
    DeleteKeyTree(LocalMenuKey);
*/
}


/* $ 14.07.2000 VVM
   + ������ TRUE/FALSE ���������� ���� EC_*
*/
/* VVM $ */
int ProcessSingleMenu(char *MenuKey,int MenuPos)
{
  while (1)
  {
    int RetCode;
    struct MenuItem UserMenuItem;
    memset(&UserMenuItem,0,sizeof(UserMenuItem));
    int NumLine,ExitCode,FuncPos[12];

    for (int I=0;I<sizeof(FuncPos)/sizeof(FuncPos[0]);I++)
      FuncPos[I]=-1;

    char Name[NM],ShortName[NM];
    CtrlObject->Cp()->ActivePanel->GetCurName(Name,ShortName);

    {
      /* $ 24.07.2000 VVM
       + ��� ������ �������� ���� � ��������� ��������� ��� - FAR/Registry
      */
      char MenuTitle[128];
      switch (MenuMode)
      {
      case MM_LOCAL:
        strcpy(MenuTitle,MSG(MLocalMenuTitle));
        break;
      case MM_FAR:
        sprintf(MenuTitle,"%s (%s)",MSG(MMainMenuTitle),MSG(MMainMenuFAR));
        break;
      default:
        {
          char *Ptr=MSG(MMainMenuREG);
          if(*Ptr)
            sprintf(MenuTitle,"%s (%s)",MSG(MMainMenuTitle),Ptr);
          else
            sprintf(MenuTitle,"%s",MSG(MMainMenuTitle));
        }
      } /* switch */
      VMenu UserMenu(MenuTitle,NULL,0,ScrY-4);
      /* VVM $ */

      /* $ 05.06.2001 KM
         ! ����������. UserMenu �� ��������� ���� VMENU_WRAPMODE
      */
      UserMenu.SetFlags(VMENU_WRAPMODE);
      /* KM $ */
      UserMenu.SetHelp("UserMenu");
      UserMenu.SetPosition(-1,-1,0,0);

      NumLine=0;

      /* $ 20.07.2000 tran
         + ������ ������ ��� ���������� ������������ ����� ������ */
      int MaxLen=20;

      while (1)
      {
        int MenuTextLen;
        char ItemKey[512],HotKey[10],Label[512],MenuText[512];
        sprintf(ItemKey,"%s\\Item%d",MenuKey,NumLine);
        if (!GetRegKey(ItemKey,"HotKey",HotKey,"",sizeof(HotKey)))
          break;
        if (!GetRegKey(ItemKey,"Label",Label,"",sizeof(Label)))
          break;
        SubstFileName(Label,Name,ShortName,NULL,NULL,TRUE);
        /* $ 28.07.2000 VVM
           + ��������� ���������� ���������
        */
        ExpandEnvironmentStr(Label, Label, sizeof(Label));
        /* VVM $ */

        int FuncNum=0;
        if (strlen(HotKey)>1)
        {
          FuncNum=atoi(&HotKey[1]);
          if (FuncNum<1 || FuncNum>12)
            FuncNum=1;
          sprintf(HotKey,"F%d",FuncNum);
        }
        /* $ 14.10.2000 VVM
           + ����������� ����, ���� ����� �����, � ������="-"
        */
        if ((strlen(Label)==0) && (strcmp(HotKey,"-")==0))
        {
         // Nothing to do
        }
        else
        {
          sprintf(MenuText,"%s%-3.3s %-20.*s",FuncNum>0 ? "":"&",HotKey,ScrX-12,Label);
          MenuTextLen=strlen(MenuText)-(FuncNum>0?1:0);
          MaxLen=(MaxLen<MenuTextLen ? MenuTextLen : MaxLen);
        } /* else */
        /* VVM $ */
        NumLine++;
      }

      // ��������� ������������ �����
      if(MaxLen > ScrX-14)
        MaxLen = ScrX-14;

      NumLine=0;

      while (1)
      {
        char ItemKey[512],HotKey[10],Label[512],MenuText[512];
        sprintf(ItemKey,"%s\\Item%d",MenuKey,NumLine);
        if (!GetRegKey(ItemKey,"HotKey",HotKey,"",sizeof(HotKey)))
          break;
        if (!GetRegKey(ItemKey,"Label",Label,"",sizeof(Label)))
          break;
        SubstFileName(Label,Name,ShortName,NULL,NULL,TRUE);
        /* $ 28.07.2000 VVM
           + ��������� ���������� ���������
        */
        ExpandEnvironmentStr(Label, Label, sizeof(Label));
        /* VVM $ */

        int SubMenu;
        GetRegKey(ItemKey,"Submenu",SubMenu,0);

        int FuncNum=0;
        if (strlen(HotKey)>1)
        {
          FuncNum=atoi(&HotKey[1]);
          if (FuncNum<1 || FuncNum>12)
            FuncNum=1;
          sprintf(HotKey,"F%d",FuncNum);
        }
        /* $ 14.10.2000 VVM
           + ����������� ����, ���� ����� �����, � ������="-"
        */
        if ((strlen(Label)==0) && (strcmp(HotKey,"-")==0))
        {
          UserMenuItem.Flags|=LIF_SEPARATOR;
          UserMenuItem.Flags&=~LIF_SELECTED;
          if (NumLine==MenuPos)
            MenuPos++;
        }
        else
        {
        /* $ 20.07.2000 tran
           %-20.*s ������� �� %-*.*s � ������������ MaxLen ��� ������������ ����� */
          sprintf(MenuText,"%s%-3.3s %-*.*s",FuncNum>0 ? "":"&",HotKey,MaxLen,MaxLen,Label);
        /* tran 20.07.2000 $ */
          if (SubMenu)
          {
            /* $ 07.05.2001 SVS
               FAR 1.70.3 b591. ��������� � ������ -a. � ���� �� F2 ������ �
               ������� ������ ������������ ".". �������� �� ">".
            */
            if(Opt.CleanAscii)
              SubMenuSymbol[1]='>';
            /* SVS $ */
            strcat(MenuText,SubMenuSymbol);
          }
          strncpy(UserMenuItem.Name,MenuText,sizeof(UserMenuItem.Name));
          UserMenuItem.SetSelect(NumLine==MenuPos);
          UserMenuItem.Flags&=~LIF_SEPARATOR;
        }
        /* VVM $ */
        int ItemPos=UserMenu.AddItem(&UserMenuItem);
        if (FuncNum>0)
          FuncPos[FuncNum-1]=ItemPos;
        NumLine++;
      }

      *UserMenuItem.Name=0;
      UserMenuItem.Flags&=~LIF_SEPARATOR;
      UserMenuItem.SetSelect(NumLine==MenuPos);
      UserMenu.AddItem(&UserMenuItem);

      {
        MenuModified=TRUE;
        while (!UserMenu.Done())
        {
          if (MenuModified==TRUE){
            UserMenu.Show();
            MenuModified=FALSE;
          }
          int SelectPos=UserMenu.GetSelectPos();
          int Key=UserMenu.ReadInput();
          if (Key>=KEY_F1 && Key<=KEY_F12)
          {
            int FuncItemPos;
            if ((FuncItemPos=FuncPos[Key-KEY_F1])!=-1)
            {
              UserMenu.SetExitCode(FuncItemPos);
              continue;
            }
          }
          switch(Key)
          {
            case KEY_DEL:
              if (SelectPos<NumLine)
                if (DeleteMenuRecord(MenuKey,SelectPos))
                {
                  UserMenu.Hide();
                  RetCode=ProcessSingleMenu(MenuKey,SelectPos);
                  MenuModified=TRUE;
                  return(RetCode);
                }
              break;
            case KEY_INS:
              if (EditMenuRecord(MenuKey,SelectPos,NumLine,1))
              {
                UserMenu.Hide();
                RetCode=ProcessSingleMenu(MenuKey,SelectPos);
                MenuModified=TRUE;
                return(RetCode);
              }
              break;
            case KEY_F4:
            case KEY_SHIFTF4:
              if (SelectPos<NumLine)
                if (EditMenuRecord(MenuKey,SelectPos,NumLine,0))
                {
                  UserMenu.Hide();
                  RetCode=ProcessSingleMenu(MenuKey,SelectPos);
                  MenuModified=TRUE;
                  return(RetCode);
                }
              break;
            case KEY_ALTF4:
              if (RegVer)
              {
                (*FrameManager)[0]->UnlockRefresh();
                FILE *MenuFile;
                char MenuFileName[NM];
                strcpy(MenuFileName,Opt.TempPath);
                strcat(MenuFileName,FarTmpXXXXXX);
                if (mktemp(MenuFileName)==NULL || (MenuFile=fopen(MenuFileName,"wb"))==NULL)
                //if (FarMkTemp(MenuFileName,"Far")==NULL || (MenuFile=fopen(MenuFileName,"wb"))==NULL)
                  break;
                MenuRegToFile(MenuRootKey,MenuFile);
                MenuModified=TRUE;
                fclose(MenuFile);
                {
                  char OldTitle[512];
                  GetConsoleTitle(OldTitle,sizeof(OldTitle));
                  FileEditor ShellEditor(MenuFileName,FALSE,FALSE,-1,-1,TRUE,NULL);
                  ShellEditor.SetDynamicallyBorn(false);
                  SetConsoleTitle(OldTitle);
                  FrameManager->ExecuteModal();
                  if (!ShellEditor.IsFileChanged() || (MenuFile=fopen(MenuFileName,"rb"))==NULL)
                  {
                    remove(MenuFileName);
//                    break;
                    return(0);
                  }
                }
                DeleteKeyTree(MenuRootKey);
                MenuFileToReg(MenuRootKey,MenuFile);
                fclose(MenuFile);
                remove(MenuFileName);
                UserMenu.Hide();
/* $ 14.07.2000 VVM
   ! ������� ����
*/
                return(0);
/* VVM $ */
              }
              else
                Message(MSG_WARNING,1,MSG(MWarning),MSG(MRegOnly),MSG(MOk));
              break;
            /* $ 28.06.2000 tran
               ����� �� ����������������� ���� �� ShiftF10 �� ������ ������
               ����������� ������ ������ ExitCode -1, � ���������� FALSE -
               �� FALSE ��� � ������ ������ ������ */
            case KEY_SHIFTF10:
//              UserMenu.SetExitCode(-1);
              return(EC_CLOSE_MENU);
             /* tran $ */
/* $ 14.07.2000 VVM
   + �������� ������� ����
*/
            case KEY_SHIFTF2:
                return(EC_MAIN_MENU);
/* $ 17.07.2000 VVM
   + �������� ���� �� ������������� �������� ������ � MM_LOCAL ������
*/
            case KEY_BS:
              if (MenuMode!=MM_MAIN)
                return(EC_PARENT_MENU);
/* VVM $ */
            default:
              UserMenu.ProcessInput();
              break;
          }
        }
      }
      ExitCode=UserMenu.GetExitCode();
    }

    if (ExitCode<0 || ExitCode>=NumLine)
/* $ 14.07.2000 VVM
   ! ����� �� ���� �������
*/
      return(EC_CLOSE_LEVEL);
/* VVM $ */

    char CurrentKey[512];
    int SubMenu;
    sprintf(CurrentKey,"%s\\Item%d",MenuKey,ExitCode);
    GetRegKey(CurrentKey,"Submenu",SubMenu,0);

    if (SubMenu)
    {
      char SubMenuKey[512];
      sprintf(SubMenuKey,"%s\\Item%d",MenuKey,ExitCode);
/* $ 14.07.2000 VVM
   ! ���� ������� �������, �� ��������. ����� �������� ���������� ����
*/
      MenuPos=ProcessSingleMenu(SubMenuKey,0);
      if (MenuPos!=EC_CLOSE_LEVEL)
        return(MenuPos);
/* VVM $ */
      MenuPos=ExitCode;
      continue;
    }

    /* $ 01.05.2001 IS
         �������� �� ������ ������
    */
    //int LeftVisible,RightVisible,PanelsHidden=0;
    /* IS $ */
    int CurLine=0;

    char CmdLineDir[NM];
    CtrlObject->CmdLine->GetCurDir(CmdLineDir);

    while (1)
    {
      char LineName[50],Command[4096];
      sprintf(LineName,"Command%d",CurLine);
      if (!GetRegKey(CurrentKey,LineName,Command,"",sizeof(Command)))
        break;

      char ListName[NM*2]="",ShortListName[NM*2]="";
      if (memicmp(Command,"REM ",4) && memicmp(Command,"::",2))
      {
        /*
          �������� ��������� ���������� ��������, ��������:
          if exist !#!\!^!.! far:edit < diff -c -p !#!\!^!.! !\!.!
          �.�. ������� "���������" ����� "if exist !#!\!^!.!", �� � ����
          ����������, �� ������ ������.
          ��� ��� ������,
          if exist ..\a.bat D:\FAR\170\DIFF.MY\mkdiff.bat !?&����� �����?!
          ��� ����������� ������, �.�. ������� ���� ������ ����, � ����
          ��������� ���� "if exist ..\a.bat", � �� ����� ������ ������...
        */
        //if(ExtractIfExistCommand(Command))
        {
          int PreserveLFN=SubstFileName(Command,Name,ShortName,ListName,ShortListName,FALSE,CmdLineDir);
          PreserveLongName PreserveName(ShortName,PreserveLFN);
          /* $ 01.05.2001 IS
             �������� �� ������ ������
          */
          /*if (!PanelsHidden)
          {
            LeftVisible=CtrlObject->Cp()->LeftPanel->IsVisible();
            RightVisible=CtrlObject->Cp()->RightPanel->IsVisible();
            CtrlObject->Cp()->LeftPanel->Hide();
            CtrlObject->Cp()->RightPanel->Hide();
            CtrlObject->Cp()->LeftPanel->SetUpdateMode(FALSE);
            CtrlObject->Cp()->RightPanel->SetUpdateMode(FALSE);
            PanelsHidden=TRUE;
          }*/
          /* IS $ */
//          ;
//_SVS(SysLog("!%s!",Command));
          if(ExtractIfExistCommand(Command))
          {
            if (*Command)
              CtrlObject->CmdLine->ExecString(Command,FALSE);
          }
        }
      }
      if (*ListName)
          remove(ListName);
      if (ListName[NM])
          remove(ListName+NM);
      if (*ShortListName)
          remove(ShortListName);
      if (ShortListName[NM])
          remove(ShortListName+NM);
      CurLine++;
    }
    /* $ 01.05.2001 IS
         �������� �� ������ ������
    */
    /*if (PanelsHidden)
    {
      CtrlObject->Cp()->LeftPanel->SetUpdateMode(TRUE);
      CtrlObject->Cp()->RightPanel->SetUpdateMode(TRUE);
      CtrlObject->Cp()->LeftPanel->Update(UPDATE_KEEP_SELECTION);
      CtrlObject->Cp()->RightPanel->Update(UPDATE_KEEP_SELECTION);
      if (RightVisible)
        CtrlObject->Cp()->RightPanel->Show();
      if (LeftVisible)
        CtrlObject->Cp()->LeftPanel->Show();
    }*/
    /* IS $ */
/* $ 14.07.2000 VVM
   ! ������� ����
*/
/* $ 25.04.2001 DJ
   ��������, ��� ���� ��������� ������� (����� ������������ ������)
*/
    return(EC_COMMAND_SELECTED);
/* DJ $ */
/* VVM $ */
  }
}


int DeleteMenuRecord(char *MenuKey,int DeletePos)
{
  char RecText[200],ItemName[200],RegKey[512];
  sprintf(RegKey,"%s\\Item%d",MenuKey,DeletePos);
  GetRegKey(RegKey,"Label",RecText,"",sizeof(RecText));
  sprintf(ItemName,"\"%s\"",RecText);
  if (Message(MSG_WARNING,2,MSG(MUserMenuTitle),MSG(MAskDeleteMenuItem),
              ItemName,MSG(MDelete),MSG(MCancel))!=0)
    return(FALSE);
  MenuModified=TRUE;
  sprintf(RegKey,"%s\\Item%%d",MenuKey);
  DeleteKeyRecord(RegKey,DeletePos);
  return(TRUE);
}


int EditMenuRecord(char *MenuKey,int EditPos,int TotalRecords,int NewRec)
{
  static struct DialogData EditDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,72,20,0,0,0,0,(char *)MEditMenuTitle,
  /* 01 */DI_TEXT,5,2,0,0,0,0,0,0,(char *)MEditMenuHotKey,
  /* 02 */DI_FIXEDIT,5,3,7,3,1,0,0,0,"",
  /* 03 */DI_TEXT,5,4,0,0,0,0,0,0,(char *)MEditMenuLabel,
  /* 04 */DI_EDIT,5,5,70,3,0,0,0,0,"",
  /* 05 */DI_TEXT,3,6,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 06 */DI_TEXT,5,7,0,0,0,0,0,0,(char *)MEditMenuCommands,
  /* 07 */DI_EDIT,5,8,70,3,0,0,DIF_EDITOR,0,"",
  /* 08 */DI_EDIT,5,9,70,3,0,0,DIF_EDITOR,0,"",
  /* 09 */DI_EDIT,5,10,70,3,0,0,DIF_EDITOR,0,"",
  /* 10 */DI_EDIT,5,11,70,3,0,0,DIF_EDITOR,0,"",
  /* 11 */DI_EDIT,5,12,70,3,0,0,DIF_EDITOR,0,"",
  /* 12 */DI_EDIT,5,13,70,3,0,0,DIF_EDITOR,0,"",
  /* 13 */DI_EDIT,5,14,70,3,0,0,DIF_EDITOR,0,"",
  /* 14 */DI_EDIT,5,15,70,3,0,0,DIF_EDITOR,0,"",
  /* 15 */DI_EDIT,5,16,70,3,0,0,DIF_EDITOR,0,"",
  /* 16 */DI_EDIT,5,17,70,3,0,0,DIF_EDITOR,0,"",
  /* 17 */DI_TEXT,3,18,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 18 */DI_BUTTON,0,19,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
  /* 19 */DI_BUTTON,0,19,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel

  };
  MakeDialogItems(EditDlgData,EditDlg);

  char ItemKey[512];
  sprintf(ItemKey,"%s\\Item%d",MenuKey,EditPos);

  MenuModified=TRUE;

  if (NewRec)
  {
    switch (Message(0,2,MSG(MUserMenuTitle),MSG(MAskInsertMenuOrCommand),
                    MSG(MMenuInsertCommand),MSG(MMenuInsertMenu)))
    {
      case -1:
      case -2:
        return(FALSE);
      case 1:
        return(EditSubMenu(MenuKey,EditPos,TotalRecords,TRUE));
    }
  }
  else
  {
    int SubMenu;
    GetRegKey(ItemKey,"Submenu",SubMenu,0);
    if (SubMenu)
      return(EditSubMenu(MenuKey,EditPos,TotalRecords,FALSE));
    GetRegKey(ItemKey,"HotKey",EditDlg[2].Data,"",sizeof(EditDlg[2].Data));
    GetRegKey(ItemKey,"Label",EditDlg[4].Data,"",sizeof(EditDlg[4].Data));
    int CommandNumber=0;
    while (CommandNumber<10)
    {
      char CommandName[20],Command[4096];
      sprintf(CommandName,"Command%d",CommandNumber);
      if (!GetRegKey(ItemKey,CommandName,Command,"",sizeof(Command)))
        break;
      strncpy(EditDlg[7+CommandNumber].Data,Command,sizeof(EditDlg[0].Data));
      CommandNumber++;
    }
  }

  {
    Dialog Dlg(EditDlg,sizeof(EditDlg)/sizeof(EditDlg[0]));
    Dlg.SetHelp("UserMenu");
    Dlg.SetPosition(-1,-1,76,22);
    /* $ 22.12.2000 IS
       ! ���� �� ����� ����� � ������ "����������", �� �� ������� �� �������
         �������������� ������, �.�. ������ �� �������, ���, ��������, �����.
         ��� ������ �� ���� ����� ��������������� esc ��� ������� "��������".
    */
    while(1)
    {
      Dlg.Process();
      if(18==Dlg.GetExitCode())
      {
       if(!strcmp(EditDlg[2].Data,"-") || strlen(EditDlg[4].Data)) break;
       Dlg.ClearDone();
      }
      else return FALSE;
    }
    /* IS $ */
  }

  if (NewRec)
  {
    char KeyMask[512];
    sprintf(KeyMask,"%s\\Item%%d",MenuKey);
    InsertKeyRecord(KeyMask,EditPos,TotalRecords);
  }

  SetRegKey(ItemKey,"HotKey",EditDlg[2].Data);
  SetRegKey(ItemKey,"Label",
    (*EditDlg[2].Data?RemoveChar(EditDlg[4].Data,'&',TRUE):EditDlg[4].Data));
  SetRegKey(ItemKey,"Submenu",(DWORD)0);

  int CommandNumber=0,I;
  for (I=0;I<10;I++)
    if (*EditDlg[I+7].Data)
      CommandNumber=I+1;
  for (I=0;I<10;I++)
  {
    char CommandName[20];
    sprintf(CommandName,"Command%d",I);
    if (I>=CommandNumber)
      DeleteRegValue(ItemKey,CommandName);
    else
      SetRegKey(ItemKey,CommandName,EditDlg[I+7].Data);
  }
  return(TRUE);
}


int EditSubMenu(char *MenuKey,int EditPos,int TotalRecords,int NewRec)
{
  static struct DialogData EditDlgData[]=
  {
    DI_DOUBLEBOX,3,1,72,8,0,0,0,0,(char *)MEditSubmenuTitle,
    DI_TEXT,5,2,0,0,0,0,0,0,(char *)MEditSubmenuHotKey,
    DI_FIXEDIT,5,3,7,3,1,0,0,0,"",
    DI_TEXT,5,4,0,0,0,0,0,0,(char *)MEditSubmenuLabel,
    DI_EDIT,5,5,70,3,0,0,0,0,"",
    DI_TEXT,3,6,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    DI_BUTTON,0,7,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
    DI_BUTTON,0,7,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(EditDlgData,EditDlg);

  char ItemKey[512];
  sprintf(ItemKey,"%s\\Item%d",MenuKey,EditPos);
  if (NewRec)
  {
    *EditDlg[2].Data=0;
    *EditDlg[4].Data=0;
  }
  else
  {
    GetRegKey(ItemKey,"HotKey",EditDlg[2].Data,"",sizeof(EditDlg[2].Data));
    GetRegKey(ItemKey,"Label",EditDlg[4].Data,"",sizeof(EditDlg[4].Data));
  }
  {
    Dialog Dlg(EditDlg,sizeof(EditDlg)/sizeof(EditDlg[0]));
    Dlg.SetHelp("UserMenu");
    Dlg.SetPosition(-1,-1,76,10);
    Dlg.Process();
    if (Dlg.GetExitCode()!=6 || *EditDlg[4].Data==0)
      return(FALSE);
  }
  if (NewRec)
  {
    char KeyMask[512];
    sprintf(KeyMask,"%s\\Item%%d",MenuKey);
    InsertKeyRecord(KeyMask,EditPos,TotalRecords);
  }

  SetRegKey(ItemKey,"HotKey",EditDlg[2].Data);
  SetRegKey(ItemKey,"Label",EditDlg[4].Data);
  SetRegKey(ItemKey,"Submenu",(DWORD)1);
  return(TRUE);
}


void MenuRegToFile(char *MenuKey,FILE *MenuFile)
{
  for (int I=0;;I++)
  {
    char ItemKey[512],HotKey[10],Label[512];
    int SubMenu;
    sprintf(ItemKey,"%s\\Item%d",MenuKey,I);
    if (!GetRegKey(ItemKey,"Label",Label,"",sizeof(Label)))
      break;
    GetRegKey(ItemKey,"Label",Label,"",sizeof(Label));
    GetRegKey(ItemKey,"HotKey",HotKey,"",sizeof(HotKey));
    GetRegKey(ItemKey,"Submenu",SubMenu,0);
    fprintf(MenuFile,"%s:  %s\r\n",HotKey,Label);
    if (SubMenu)
    {
      fprintf(MenuFile,"{\r\n");
      MenuRegToFile(ItemKey,MenuFile);
      fprintf(MenuFile,"}\r\n");
    }
    else
      for (int J=0;;J++)
      {
        char LineName[50],Command[4096];
        sprintf(LineName,"Command%d",J);
        if (!GetRegKey(ItemKey,LineName,Command,"",sizeof(Command)))
          break;
        fprintf(MenuFile,"    %s\r\n",Command);
      }
  }
}


void MenuFileToReg(char *MenuKey,FILE *MenuFile)
{
  char MenuStr[512];
  int KeyNumber=-1,CommandNumber;
  while (fgets(MenuStr,sizeof(MenuStr),MenuFile)!=NULL)
  {
    char ItemKey[512];
    sprintf(ItemKey,"%s\\Item%d",MenuKey,KeyNumber);
    RemoveTrailingSpaces(MenuStr);
    if (*MenuStr==0)
      continue;
    if (*MenuStr=='{' && KeyNumber>=0)
    {
      MenuFileToReg(ItemKey,MenuFile);
      continue;
    }
    if (*MenuStr=='}')
      break;
    if (!isspace(*MenuStr))
    {
      char HotKey[10],Label[512],*ChPtr;
      int SubMenu;
      if ((ChPtr=strchr(MenuStr,':'))==NULL)
        continue;
      sprintf(ItemKey,"%s\\Item%d",MenuKey,++KeyNumber);
      *ChPtr=0;
      strcpy(HotKey,MenuStr);
      strcpy(Label,ChPtr+1);
      RemoveLeadingSpaces(Label);
      SaveFilePos SavePos(MenuFile);
      SubMenu=(fgets(MenuStr,sizeof(MenuStr),MenuFile)!=NULL && *MenuStr=='{');
      UseSameRegKey();
      SetRegKey(ItemKey,"HotKey",HotKey);
      SetRegKey(ItemKey,"Label",Label);
      SetRegKey(ItemKey,"Submenu",SubMenu);
      CloseSameRegKey();
      CommandNumber=0;
    }
    else
      if (KeyNumber>=0)
      {
        char LineName[50];
        sprintf(LineName,"Command%d",CommandNumber++);
        RemoveLeadingSpaces(MenuStr);
        SetRegKey(ItemKey,LineName,MenuStr);
      }
  }
}
