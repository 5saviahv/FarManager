/*
cmdline.cpp

��������� ������

*/

/* Revision: 1.44 02.11.2001 $ */

/*
Modify:
  02.11.2001 SVS
    + GetSelection()
  10.10.2001 SVS
    ! ����� ����, ������������� �� "��������" ������� ������ ��������
      � ��������� ������ execute.cpp
  05.10.2001 SVS
    ! ������� ����������� (� ���������� �������������� ����)
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  26.09.2001 VVM
    ! ������������ ������, ���� ���� ���������. � ������� � ����������� �����.
  23.09.2001 VVM
    ! CmdExecute: RedrawDesktop ������ ������������ _��_ ������ UpdateIfChanged.
      ����� InfoPanel ���������������� ��� ��������� �������� � ������ background
  09.09.2001 IS
    + SetPersistentBlocks - ����������/�������� ���������� �����
  08.09.2001 VVM
    + ������������ Opt.DialogsEditBlock
  30.08.2001 VVM
    ! � ��������� ������ ����� ������ �����������.
  23.08.2001 OT
    - ����������� far -e file -> AltF9
  13.08.2001 SKV
    + GetSelString, Select
  07.08.2001 SVS
    + ������� ��������� ������� OS - CLS
  10.07.2001 SVS
    + ��������� KEY_MACROXLAT
  25.06.2001 SVS
    - ������� �������� � "If exist" �������������� toFullName, �� ����������
      ���� ����, ��� ��� ��� ����� ����� ������ ����.
  22.06.2001 SKV
    - Update ������� ����� ���������� �������.
  18.06.2001 SVS
    - �� ����� �������� "If exist" �� ���������� ������� �������.
  17.06.2001 IS
    ! ������ ExpandEnvironmentStrings ��������� ExpandEnvironmentStr, �.�. ���
      ��������� �������� � ���������, ���� ������� ���� 0x7F.
    + ������������ ������ ����� SetEnvironmentVariable �� OEM � ANSI
  07.06.2001 SVS
    + ��������� ��������� ���������� "REM" � "::"
  04.06.2001 OT
    - ����������� ��������� ������� ��� ������� Qinfo � ��� ������ "�������������" �������
  26.05.2001 OT
    - ����������� ������ ������� � NFZ
  17.05.2001 OT
    - ��������� ��� ��������� �������� ������� - ResizeConsole().
  15.05.2001 OT
    ! NWZ -> NFZ
  12.05.2001 DJ
    ! ����������� ��������� ������ ����� ��������� ������� �������� ������
      ���� ������ �������� ������� �������
  11.05.2001 OT
    ! ����� ������ ��� ��������� Background
  10.05.2001 DJ
    * ShowViewEditHistory()
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  25.04.2001 DJ
    * ��������� @ � IF EXIST
    * ��������� ������� ������ ����� ����� � IF EXIST
  11.04.2001 SVS
    ! ��� Alt-F11 � Alt-F12 - ������ ����� ���� ���������� ���� ������, � ��
      ����������� �������� ������ ��������� ������ (�� ������ ��� ����
      �������)
  02.04.2001 VVM
    + ��������� Opt.FlagPosixSemantics
  12.03.2001 SVS
    + Alt-Shift-Left, Alt-Shift-Right, Alt-Shift-Home � Alt-Shift-End ��������
      ���� � ��������� ������ ���������� �� ��������� �������.
  21.02.2001 IS
    ! Opt.EditorPersistentBlocks -> Opt.EdOpt.PersistentBlocks
  19.02.2001 IS
    - ���: �� ������������ ��������� � ��������� ������ �� enter � shift-enter
  14.01.2001 SVS
    + � ProcessOSCommands ��������� ���������
       "IF [NOT] EXIST filename command"
       "IF [NOT] DEFINED variable command"
  18.12.2000 SVS
    - �������� �� "Ctrl-D - ������ ������"!
    + ���������� ��������� ��� �������������� �� ��������� ��������
  13.12.2000 SVS
    ! ��� CmdLine - ���� ��� ���������, ����������� ��� ������ (XLat)
  04.11.2000 SVS
    + �������� �� �������������� ������� ��� XLat-�������������
  24.09.2000 SVS
    + ��������� ESC.
    + ����� ������� Xlat
  19.09.2000 SVS
    - ��� ������ �� History (�� Alt-F8) ������ �� ������� ����������!
  13.09.2000 tran 1.02
    + COL_COMMANDLINEPREFIX
  02.08.2000 tran 1.01
    - ������ ���� - ��� ������ �� CtrlF10, ���� ���� ��� ������ �� ��������
      �� Alt-F11, ��� ����� keybar � �������
      ��� ������ ������� CtrlObject->Cp()->Redraw()
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "cmdline.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "keys.hpp"
#include "lang.hpp"
#include "ctrlobj.hpp"
#include "manager.hpp"
#include "history.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "foldtree.hpp"
#include "fileview.hpp"
#include "fileedit.hpp"
#include "rdrwdsk.hpp"
#include "savescr.hpp"
#include "scrbuf.hpp"

CommandLine::CommandLine()
{
  *CurDir=0;
  CmdStr.SetEditBeyondEnd(FALSE);
  SetPersistentBlocks(Opt.DialogsEditBlock);
  LastCmdPartLength=-1;
  *LastCmdStr=0;
  BackgroundScreen=NULL;
}

/* $ 09.09.2001 IS ����������/�������� ���������� ����� */
void CommandLine::SetPersistentBlocks(int Mode)
{
  CmdStr.SetPersistentBlocks(Mode);
}
/* IS $ */

void CommandLine::DisplayObject()
{
  _OT(SysLog("[%p] CommandLine::DisplayObject()",this));
  char TruncDir[NM];
  GetPrompt(TruncDir);
  TruncPathStr(TruncDir,(X2-X1)/2);
  GotoXY(X1,Y1);
  SetColor(COL_COMMANDLINEPREFIX);
  Text(TruncDir);
  CmdStr.SetObjectColor(COL_COMMANDLINE,COL_COMMANDLINESELECTED);
  CmdStr.SetLeftPos(0);
  CmdStr.SetPosition(X1+strlen(TruncDir),Y1,X2,Y2);
  CmdStr.Show();
}


void CommandLine::SetCurPos(int Pos)
{
  CmdStr.SetLeftPos(0);
  CmdStr.SetCurPos(Pos);
  CmdStr.Redraw();
}


int CommandLine::ProcessKey(int Key)
{
  char Str[2048], *PStr;

  if (Key==KEY_CTRLEND && CmdStr.GetCurPos()==CmdStr.GetLength())
  {
    if (LastCmdPartLength==-1)
      strncpy(LastCmdStr,CmdStr.GetStringAddr(),sizeof(LastCmdStr)-1);
    strcpy(Str,LastCmdStr);
    int CurCmdPartLength=strlen(Str);
    CtrlObject->CmdHistory->GetSimilar(Str,LastCmdPartLength);
    if (LastCmdPartLength==-1)
    {
      LastCmdPartLength=CurCmdPartLength;
      strncpy(LastCmdStr,CmdStr.GetStringAddr(),sizeof(LastCmdStr)-1);
    }
    CmdStr.SetString(Str);
    Show();
    return(TRUE);
  }

  if(Key == KEY_UP)
  {
    if (CtrlObject->Cp()->LeftPanel->IsVisible() || CtrlObject->Cp()->RightPanel->IsVisible())
      return(FALSE);
    Key=KEY_CTRLE;
  }
  else if(Key == KEY_DOWN)
  {
    if (CtrlObject->Cp()->LeftPanel->IsVisible() || CtrlObject->Cp()->RightPanel->IsVisible())
      return(FALSE);
    Key=KEY_CTRLX;
  }

  switch(Key)
  {
    case KEY_CTRLE:
    case KEY_CTRLX:
      if(Key == KEY_CTRLE)
        CtrlObject->CmdHistory->GetPrev(Str);
      else
        CtrlObject->CmdHistory->GetNext(Str);
    case KEY_ESC:
      if(Key == KEY_ESC)
      {
        /* $ 24.09.2000 SVS
           ���� ������ ��������� �� "������������ ��� Esc", �� ������� �
           ������� �� ������ � ������ � ������ ���������.
        */
        if(Opt.CmdHistoryRule)
          CtrlObject->CmdHistory->SetFirst();
        PStr="";
      }
      else
        PStr=Str;
      SetString(PStr);
      return(TRUE);
    case KEY_F2:
      ProcessUserMenu(0);
      return(TRUE);
    case KEY_ALTF8:
      {
        int Type;
        /* $ 19.09.2000 SVS
           - ��� ������ �� History (�� Alt-F8) ������ �� ������� ����������!
        */
        switch(CtrlObject->CmdHistory->Select(MSG(MHistoryTitle),"History",Str,Type))
        {
          case 1:
            SetString(Str);
            ProcessKey(KEY_ENTER);
            //ExecString(Str,FALSE,FALSE);
            //CtrlObject->CmdHistory->AddToHistory(Str);
            break;
          case 2:
            SetString(Str);
            ProcessKey(KEY_SHIFTENTER);
            //ExecString(Str,FALSE,TRUE);
            //CtrlObject->CmdHistory->AddToHistory(Str);
            break;
          case 3:
            SetString(Str);
            break;
        }
        /* SVS $ */
      }
      return(TRUE);
    case KEY_SHIFTF9:
      SaveConfig(1);
      return(TRUE);
    case KEY_F10:
      FrameManager->ExitMainLoop(TRUE);
      return(TRUE);
    case KEY_ALTF10:
      {
        {
          FolderTree Tree(Str,MODALTREE_ACTIVE,4,2,ScrX-4,ScrY-4);
        }
        if (*Str)
        {
          Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
          ActivePanel->SetCurDir(Str,TRUE);
          ActivePanel->Show();
          if (ActivePanel->GetType()==TREE_PANEL)
            ActivePanel->ProcessKey(KEY_ENTER);
        }
      }
      return(TRUE);
    case KEY_F11:
      CtrlObject->Plugins.CommandsMenu(FALSE,FALSE,0);
      return(TRUE);
    case KEY_ALTF11:
      /* $ 10.05.2001 DJ
         ����� view/edit history ������� � ��������� ���������
      */
      ShowViewEditHistory();
      /* DJ $ */
      CtrlObject->Cp()->Redraw();
      return(TRUE);
    case KEY_ALTF12:
      {
        int Type,SelectType;
        if ((SelectType=CtrlObject->FolderHistory->Select(MSG(MFolderHistoryTitle),"HistoryFolders",Str,Type))==1 || SelectType==2)
        {
          if (SelectType==2)
            CtrlObject->FolderHistory->SetAddMode(FALSE,2,TRUE);
          CtrlObject->Cp()->ActivePanel->SetCurDir(Str,Type==0 ? TRUE:FALSE);
          CtrlObject->Cp()->ActivePanel->Redraw();
          CtrlObject->FolderHistory->SetAddMode(TRUE,2,TRUE);
        }
        else
          if (SelectType==3)
            SetString(Str);
      }
      return(TRUE);
    case KEY_ENTER:
    case KEY_SHIFTENTER:
      {
        Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
        /* $ 19.02.2001 IS
             - ��������� ��� ��� �� �����
        */
        CmdStr.Select(-1,0);
        CmdStr.Show();
        /* IS $ */
        CmdStr.GetString(Str,sizeof(Str));
        if (*Str==0)
          break;
        ActivePanel->SetCurPath();
        CtrlObject->CmdHistory->AddToHistory(Str);
        if (!ActivePanel->ProcessPluginEvent(FE_COMMAND,(void *)Str))
          CmdExecute(Str,FALSE,Key==KEY_SHIFTENTER,FALSE);
      }
      return(TRUE);

    /* �������������� ������� ��� ��������� � ��� ������.
       ��������!
       ��� ���������� ���� ���� ����� ������ ������ ����� "default"
    */
    case KEY_ALTSHIFTLEFT:
    case KEY_ALTSHIFTRIGHT:
    case KEY_ALTSHIFTEND:
    case KEY_ALTSHIFTHOME:
      Key&=~KEY_ALT;

    default:
      /* $ 24.09.2000 SVS
         ���� �������� ������� ������ ������� Xlat, ��
         ��������� ������� ��� ���������, ���� ��� != 0
      */
      /* $ 04.11.2000 SVS
         �������� �� �������������� �������
      */
      if((Opt.XLat.XLatCmdLineKey && Key == Opt.XLat.XLatCmdLineKey) ||
         (Opt.XLat.XLatAltCmdLineKey && Key == Opt.XLat.XLatAltCmdLineKey) ||
         Key == KEY_MACROXLAT)
      {
        /* 13.12.2000 SVS
           ! ��� CmdLine - ���� ��� ���������, ����������� ��� ������ (XLat)
        */
        CmdStr.Xlat(TRUE);
        /* SVS $ */
        return(TRUE);
      }
      /* SVS $ */
      /* SVS $ */

      /* $ 18.12.2000 SVS
         ���������� ��������� �� ��������� ��������
      */
      if (!Opt.EdOpt.PersistentBlocks)
      {
        static int UnmarkKeys[]={KEY_LEFT,KEY_CTRLS,KEY_RIGHT,KEY_CTRLD,
                   KEY_CTRLLEFT,KEY_CTRLRIGHT,KEY_CTRLHOME,KEY_CTRLEND,
                   KEY_HOME,KEY_END
                   };
        for (int I=0;I<sizeof(UnmarkKeys)/sizeof(UnmarkKeys[0]);I++)
          if (Key==UnmarkKeys[I])
          {
            CmdStr.Select(-1,0);
            break;
          }
      }
      /* SVS $ */

      /* $ 18.12.2000 SVS
         �������� �� "Ctrl-D - ������ ������"
      */
      if(Key == KEY_CTRLD)
        Key=KEY_RIGHT;
      /* SVS $ */
      if (!CmdStr.ProcessKey(Key))
        break;
      LastCmdPartLength=-1;
      return(TRUE);
  }
  return(FALSE);
}


void CommandLine::SetCurDir(char *CurDir)
{
  strcpy(CommandLine::CurDir,CurDir);
}


void CommandLine::GetCurDir(char *CurDir)
{
  strcpy(CurDir,CommandLine::CurDir);
}

void CommandLine::GetString(char *Str,int MaxSize)
{
  CmdStr.GetString(Str,MaxSize);
}


void CommandLine::SetString(char *Str)
{
  LastCmdPartLength=-1;
  CmdStr.SetString(Str);
  CmdStr.SetLeftPos(0);
  CmdStr.Show();
}


void CommandLine::ExecString(char *Str,int AlwaysWaitFinish,int SeparateWindow,
                             int DirectRun)
{
  SetString(Str);
  CmdExecute(Str,AlwaysWaitFinish,SeparateWindow,DirectRun);
}


void CommandLine::InsertString(char *Str)
{
  LastCmdPartLength=-1;
  CmdStr.InsertString(Str);
  CmdStr.Show();
}


int CommandLine::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  return(CmdStr.ProcessMouse(MouseEvent));
}


void CommandLine::GetPrompt(char *DestStr)
{
  char FormatStr[512],ExpandedFormatStr[512];
  strcpy(FormatStr,Opt.UsePromptFormat ? Opt.PromptFormat:"$p$g");
  char *Format=FormatStr;
  if (Opt.UsePromptFormat)
  {
    ExpandEnvironmentStr(FormatStr,ExpandedFormatStr,sizeof(ExpandedFormatStr));
    Format=ExpandedFormatStr;
  }
  while (*Format)
  {
    if (*Format=='$')
    {
      Format++;
      switch(*Format)
      {
        case '$':
          *(DestStr++)='$';
          break;
        case 'p':
          strcpy(DestStr,CurDir);
          DestStr+=strlen(CurDir);
          break;
        case 'n':
          if (isalpha(CurDir[0]) && CurDir[1]==':' && CurDir[2]=='\\')
            *(DestStr++)=LocalUpper(*CurDir);
          else
            *(DestStr++)='?';
          break;
        case 'g':
          *(DestStr++)='>';
          break;
      }
      Format++;
    }
    else
      *(DestStr++)=*(Format++);
  }
  *DestStr=0;
}



/* $ 10.05.2001 DJ
   ����� history �� Alt-F11 ������� � ��������� �������
*/

void CommandLine::ShowViewEditHistory()
{
  char Str[1024],ItemTitle[256];
  int Type,SelectType;
  if ((SelectType=CtrlObject->ViewHistory->Select(MSG(MViewHistoryTitle),"HistoryViews",Str,Type,ItemTitle))==1 || SelectType==2)
  {
    if (SelectType!=2)
      CtrlObject->ViewHistory->AddToHistory(Str,ItemTitle,Type);
    CtrlObject->ViewHistory->SetAddMode(FALSE,Opt.FlagPosixSemantics?1:2,TRUE);

    switch(Type)
    {
    case 0:
      new FileViewer(Str,TRUE);
      break;
    case 1:
      new FileEditor(Str,FALSE,TRUE);
      break;
    case 2:
    case 3:
      {
        if (*Str!='@')
          ExecString(Str,Type-2);
        else
        {
          SaveScreen SaveScr;
          CtrlObject->Cp()->LeftPanel->CloseFile();
          CtrlObject->Cp()->RightPanel->CloseFile();
          Execute(Str+1,Type-2);
        }
        break;
      }
    }
    CtrlObject->ViewHistory->SetAddMode(TRUE,Opt.FlagPosixSemantics?1:2,TRUE);
  }
  else
    if (SelectType==3)
      SetString(Str);
}

/* DJ $ */
int CommandLine::GetCurPos()
{
  return(CmdStr.GetCurPos());
}


void CommandLine::SaveBackground(int X1,int Y1,int X2,int Y2)
{
  if (BackgroundScreen) {
    delete BackgroundScreen;
  }
  BackgroundScreen=new SaveScreen(X1,Y1,X2,Y2);
}

void CommandLine::SaveBackground()
{
  if (BackgroundScreen) {
//    BackgroundScreen->Discard();
    BackgroundScreen->SaveArea();
  }
}
void CommandLine::ShowBackground()
{
  if (BackgroundScreen){
    BackgroundScreen->RestoreArea();
  }
}

void CommandLine::ResizeConsole()
{
  BackgroundScreen->Resize(ScrX+1,ScrY+1,2);
//  this->DisplayObject();
}

/*$ 13.08.2001 SKV */
void CommandLine::GetSelString(char* Buffer,int MaxLength)
{
  CmdStr.GetSelString(Buffer,MaxLength);
}

void CommandLine::Select(int Start,int End)
{
  CmdStr.Select(Start,End);
}
/* SKV$*/

void CommandLine::GetSelection(int &Start,int &End)
{
  CmdStr.GetSelection(Start,End);
}
