/*
fileview.cpp

��ᬮ�� 䠩�� - �����ன�� ��� viewer.cpp

*/

/* Revision: 1.15 19.12.2000 $ */

/*
Modify:
  19.12.2000 SVS
    + Alt-Shift-F9 - �맮� ������� ����஥� (� ����� IS)
    - [*] ���뫨 "���⮫����" ���� � LNG-䠩��� ��� ������� F9 :-)
      ���⮫���� -���⮫����, �� �� �����뢠��.
  16.12.2000 tran 1.14
    ! Ctrl-F10 ᬮ��� �� ���ᨢ��� ������
  03.11.2000 OT
    ! �������� �஢�ન �����頥���� ���祭��
  02.11.2000 OT
    ! �������� �஢�ન �� ����� ����, �⢥������� ��� ��� 䠩��.
  27.09.2000 SVS
    + ����� 䠩�� � �ᯮ�짮������ ������� PrintMan
    ! Ctrl-Alt-Shift - ॠ���㥬, �᫨ ����.
  15.09.2000 tran 1.09
    - FKL bug
  14.09.2000 SVS
    - Bug #NN1 - ������⪨  ���������� KeyBar (�. ���ᠭ�� � Patch#191)
  24.08.2000 SVS
    + ������塞 ॠ��� ������ �����㭤� �� ������� CtrlAltShift
  07.08.2000 SVS
    + ������� �������� ���७��� �㭪樮������ ������
  22.07.2000 tran 1.06
    + Ctrl-F10 ��室�� � ��⠭����� �� 䠩� �� ⥪�饩 ������
  21.07.2000 tran 1.05
      - ���䠪� �� CtrlO �� �몫�祭��� ������
  15.07.2000 tran
      + CtrlB �몫�砥�/����砥� keybar
  04.07.2000 tran
    + �� �����뢠�� ���ᠣ ���� �� ������������ ������ 䠩�
  29.06.2000 tran
    + �������� ��� �㭪樮������ ������
  28.06.2000 tran
    - NT Console resize
      adding SetScreenPosition
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

FileViewer::FileViewer(char *Name,int EnableSwitch,int DisableHistory,
                       int DisableEdit,long ViewStartPos,char *PluginData,
                       NamesList *ViewNamesList)
{
  FileViewer::DisableEdit=DisableEdit;
  SetPosition(0,0,ScrX,ScrY);
  FullScreen=TRUE;
  Init(Name,EnableSwitch,DisableHistory,ViewStartPos,PluginData,ViewNamesList);
}


FileViewer::FileViewer(char *Name,int EnableSwitch,char *Title,
                       int X1,int Y1,int X2,int Y2)
{
  DisableEdit=TRUE;
  SetPosition(X1,Y1,X2,Y2);
  FullScreen=(X1==0 && Y1==0 && X2==ScrX && Y2==ScrY);
  View.SetTitle(Title);
  Init(Name,EnableSwitch,TRUE,-1,"",NULL);
}


void FileViewer::Init(char *Name,int EnableSwitch,int DisableHistory,
                      long ViewStartPos,char *PluginData,
                      NamesList *ViewNamesList)
{
  ViewKeyBar.SetOwner(this);
  ViewKeyBar.SetPosition(X1,Y2,X2,Y2);
  View.SetPluginData(PluginData);
  /* $ 07.08.2000 SVS
  */
  View.SetHostFileViewer(this);
  /* SVS $ */

  SetEnableSwitch(EnableSwitch);

  /* $ 07.08.2000 SVS
    ! ���, ��ᠥ�� KeyBar �뭥ᥭ � �⤥���� �㭪�� */
  InitKeyBar();
  /* SVS $*/

  /* $ 04.07.2000 tran
     + add TRUE as 'warning' parameter */
  if (!View.OpenFile(Name,TRUE))
  /* tran 04.07.2000 $ */
  {
    ExitCode=FALSE;
    return;
  }

  if (ViewStartPos!=-1)
    View.SetFilePos(ViewStartPos);
  if (ViewNamesList)
    View.SetNamesList(ViewNamesList);
  ExitCode=TRUE;
  ViewKeyBar.Show();
  /* $ 15.07.2000 tran
     dirty trick :( */
  if ( Opt.ShowKeyBarViewer==0 )
    ViewKeyBar.Hide0();
  /* tran 15.07.2000 $ */


  sprintf(NewTitle,MSG(MInViewer),PointToName(Name));
  SetFarTitle(NewTitle);
  ShowConsoleTitle();
  F3KeyOnly=TRUE;
  Process();
  if (!DisableHistory && (CtrlObject->ActivePanel!=NULL || strcmp(Name,"-")!=0))
  {
    char FullFileName[NM];
//    ConvertNameToFull(Name,FullFileName, sizeof(FullFileName));
    if (ConvertNameToFull(Name,FullFileName, sizeof(FullFileName)) >= sizeof(FullFileName)){
      return ;
    }
    CtrlObject->ViewHistory->AddToHistory(FullFileName,MSG(MHistoryView),0);
  }
}


/* $ 07.08.2000 SVS
  �㭪�� ���樠����樨 KeyBar Labels
*/
void FileViewer::InitKeyBar(void)
{
  /* $ 29.06.2000 tran
     ������� �������� ��� �㭪樮������ ������ */
  char *FViewKeys[]={MSG(MViewF1),MSG(MViewF2),MSG(MViewF3),MSG(MViewF4),MSG(MViewF5),DisableEdit ? "":MSG(MViewF6),MSG(MViewF7),MSG(MViewF8),MSG(MViewF9),MSG(MViewF10),MSG(MViewF11),MSG(MViewF12)};
  char *FViewShiftKeys[]={MSG(MViewShiftF1),MSG(MViewShiftF2),MSG(MViewShiftF3),MSG(MViewShiftF4),MSG(MViewShiftF5),MSG(MViewShiftF6),MSG(MViewShiftF7),MSG(MViewShiftF8),MSG(MViewShiftF9),MSG(MViewShiftF10),MSG(MViewShiftF11),MSG(MViewShiftF12)};
  char *FViewAltKeys[]={MSG(MViewAltF1),MSG(MViewAltF2),MSG(MViewAltF3),MSG(MViewAltF4),MSG(MViewAltF5),MSG(MViewAltF6),MSG(MViewAltF7),MSG(MViewAltF8),MSG(MViewAltF9),MSG(MViewAltF10),MSG(MViewAltF11),MSG(MViewAltF12)};
  char *FViewCtrlKeys[]={MSG(MViewCtrlF1),MSG(MViewCtrlF2),MSG(MViewCtrlF3),MSG(MViewCtrlF4),MSG(MViewCtrlF5),MSG(MViewCtrlF6),MSG(MViewCtrlF7),MSG(MViewCtrlF8),MSG(MViewCtrlF9),MSG(MViewCtrlF10),MSG(MViewCtrlF11),MSG(MViewCtrlF12)};

  if(CtrlObject->Plugins.FindPlugin(SYSID_PRINTMANAGER) == -1)
    FViewAltKeys[5-1]="";

  /* $ 07.08.2000 SVS
     ������� �������� ���७��� �㭪樮������ ������ */
  char *FViewAltShiftKeys[]={MSG(MViewAltShiftF1),MSG(MViewAltShiftF2),MSG(MViewAltShiftF3),MSG(MViewAltShiftF4),MSG(MViewAltShiftF5),MSG(MViewAltShiftF6),MSG(MViewAltShiftF7),MSG(MViewAltShiftF8),MSG(MViewAltShiftF9),MSG(MViewAltShiftF10),MSG(MViewAltShiftF11),MSG(MViewAltShiftF12)};
  char *FViewCtrlShiftKeys[]={MSG(MViewCtrlShiftF1),MSG(MViewCtrlShiftF2),MSG(MViewCtrlShiftF3),MSG(MViewCtrlShiftF4),MSG(MViewCtrlShiftF5),MSG(MViewCtrlShiftF6),MSG(MViewCtrlShiftF7),MSG(MViewCtrlShiftF8),MSG(MViewCtrlShiftF9),MSG(MViewCtrlShiftF10),MSG(MViewCtrlShiftF11),MSG(MViewCtrlShiftF12)};
  char *FViewCtrlAltKeys[]={MSG(MViewCtrlAltF1),MSG(MViewCtrlAltF2),MSG(MViewCtrlAltF3),MSG(MViewCtrlAltF4),MSG(MViewCtrlAltF5),MSG(MViewCtrlAltF6),MSG(MViewCtrlAltF7),MSG(MViewCtrlAltF8),MSG(MViewCtrlAltF9),MSG(MViewCtrlAltF10),MSG(MViewCtrlAltF11),MSG(MViewCtrlAltF12)};
  /* SVS $*/

  ViewKeyBar.Set(FViewKeys,sizeof(FViewKeys)/sizeof(FViewKeys[0]));
  ViewKeyBar.SetShift(FViewShiftKeys,sizeof(FViewShiftKeys)/sizeof(FViewShiftKeys[0]));
  ViewKeyBar.SetAlt(FViewAltKeys,sizeof(FViewAltKeys)/sizeof(FViewAltKeys[0]));
  ViewKeyBar.SetCtrl(FViewCtrlKeys,sizeof(FViewCtrlKeys)/sizeof(FViewCtrlKeys[0]));
  /* $ 07.08.2000 SVS
     ������� �������� ���७��� �㭪樮������ ������ */
  ViewKeyBar.SetCtrlAlt(FViewCtrlAltKeys,sizeof(FViewCtrlAltKeys)/sizeof(FViewCtrlAltKeys[0]));
  ViewKeyBar.SetCtrlShift(FViewCtrlShiftKeys,sizeof(FViewCtrlShiftKeys)/sizeof(FViewCtrlShiftKeys[0]));
  ViewKeyBar.SetAltShift(FViewAltShiftKeys,sizeof(FViewAltShiftKeys)/sizeof(FViewAltShiftKeys[0]));
  /* SVS $ */
  /* tran $ */

  SetKeyBar(&ViewKeyBar);
  /* $ 15.07.2000 tran
     ShowKeyBarViewer support*/
  View.SetPosition(X1,Y1,X2,Y2-(Opt.ShowKeyBarViewer?1:0));
  /* tran 15.07.2000 $ */

  View.SetViewKeyBar(&ViewKeyBar);
}
/* SVS $ */

void FileViewer::Process()
{
  ChangeMacroMode MacroMode(MACRO_VIEWER);
  /* $ 15.09.2000 tran
     FKL bug */
  if ( Opt.ShowKeyBarViewer )
      ViewKeyBar.Show();
  else
      ViewKeyBar.Hide0(); // 0 mean - Don't purge saved screen
  /* tran 15.09.2000 $ */
  Modal::Process();
}


void FileViewer::Show()
{
  if (FullScreen)
  {
    /* $ 15.07.2000 tran
       + keybar hide/show support */
    if ( Opt.ShowKeyBarViewer )
    {
        ViewKeyBar.SetPosition(0,ScrY,ScrX,ScrY);
        ViewKeyBar.Redraw();
    }
    SetPosition(0,0,ScrX,ScrY-(Opt.ShowKeyBarViewer?1:0));
    View.SetPosition(0,0,ScrX,ScrY-(Opt.ShowKeyBarViewer?1:0));
    /* tran 15.07.2000 $ */
  }
  ScreenObject::Show();
}


void FileViewer::DisplayObject()
{
  View.Show();
}


int FileViewer::ProcessKey(int Key)
{
  if (Key!=KEY_F3 && Key!=KEY_NUMPAD5)
    F3KeyOnly=FALSE;
  switch(Key)
  {
    /* $ 22.07.2000 tran
       + ��室 �� ctrl-f10 � ��⠭����� ����� �� 䠩� */
    case KEY_CTRLF10:
      {
        if(GetEnableSwitch())
        {
          char DirTmp[NM],ADir[NM],PDir[NM],*NameTmp,FileName[NM];
          View.GetFileName(FileName);
          ProcessKey(KEY_F10);
          if(strchr(FileName,'\\') || strchr(FileName,'/'))
          {
            strncpy(DirTmp,FileName,NM);
            NameTmp=PointToName(DirTmp);
            if(NameTmp>DirTmp)NameTmp[-1]=0;
            CtrlObject->GetAnotherPanel(CtrlObject->ActivePanel)->GetCurDir(PDir);
            CtrlObject->ActivePanel->GetCurDir(ADir);
            // �᫨ �㦭� ���� ���� �� ���ᨢ��� ������
            if ( LocalStricmp(ADir,DirTmp)!=0  && LocalStricmp(PDir,DirTmp)==0)
            {
                CtrlObject->ProcessKey(KEY_TAB);
            }
            CtrlObject->ActivePanel->SetCurDir(DirTmp,TRUE);
            CtrlObject->ActivePanel->GoToFile(NameTmp);
          }
          /*else
          {
            CtrlObject->ActivePanel->SetCurDir(StartDir,TRUE);
            CtrlObject->ActivePanel->GoToFile(FileName);
          } */
        }
        return (TRUE);
      }
    /* tran 22.07.2000 $ */
    /* $ 15.07.2000 tran
       + CtrlB switch KeyBar*/
    case KEY_CTRLB:
      Opt.ShowKeyBarViewer=!Opt.ShowKeyBarViewer;
      if ( Opt.ShowKeyBarViewer )
        ViewKeyBar.Show();
      else
        ViewKeyBar.Hide0(); // 0 mean - Don't purge saved screen
      Show();
      return (TRUE);
    /* tran 15.07.2000 $ */
    /* $ 24.08.2000 SVS
       + ������塞 ॠ��� ������ �����㭤� �� ������� CtrlAltShift
    */
    case KEY_CTRLALTSHIFTPRESS:
      if(!(Opt.AllCtrlAltShiftRule & CASR_VIEWER))
        return TRUE;
    case KEY_CTRLO:
      Hide();
      if (CtrlObject->LeftPanel!=CtrlObject->RightPanel)
        CtrlObject->ModalManager.ShowBackground();
      else
      {
        ViewKeyBar.Hide();
        if(Opt.AllCtrlAltShiftRule & CASR_VIEWER)
          WaitKey(Key==KEY_CTRLALTSHIFTPRESS?KEY_CTRLALTSHIFTRELEASE:-1);
      }
      /* $ 21.07.2000 tran
         - ���䠪� �� Ctrl-O*/
      if ( Opt.ShowKeyBarViewer )
        ViewKeyBar.Show();
      else
        ViewKeyBar.Hide0(); // 0 mean - Don't purge saved screen
      /* tran 21.07.2000 $ */
      Show();
      return(TRUE);
    /* SVS $ */
    case KEY_CTRLTAB:
    case KEY_CTRLSHIFTTAB:
    case KEY_F12:
      if (GetEnableSwitch())
      {
        View.KeepInitParameters();
        if (Key==KEY_CTRLSHIFTTAB)
          SetExitCode(3);
        else
          SetExitCode(Key==KEY_CTRLTAB ? 1:2);
      }
      return(TRUE);
    case KEY_F3:
    case KEY_NUMPAD5:
      if (F3KeyOnly)
        return(TRUE);
    case KEY_ESC:
    case KEY_F10:
      SetExitCode(0);
      return(TRUE);
    case KEY_F6:
      if (!DisableEdit)
      {
        SetExitCode(0);
        char ViewFileName[NM];
        View.GetFileName(ViewFileName);
        long FilePos=View.GetFilePos();
        CtrlObject->ModalManager.SetNextWindow(FALSE,ViewFileName,FilePos);
        ShowTime(2);
      }
      return(TRUE);

    /* $ 27.09.2000 SVS
       + ����� 䠩�� � �ᯮ�짮������ ������� PrintMan
    */
    case KEY_ALTF5:
    {
      if(CtrlObject->Plugins.FindPlugin(SYSID_PRINTMANAGER) != -1)
        CtrlObject->Plugins.CallPlugin(SYSID_PRINTMANAGER,OPEN_VIEWER,0); // printman
      return TRUE;
    }
    /* SVS $*/

    /* $ 19.12.2000 SVS
       �맮� ������� ����஥� (� ����� IS)
    */
    case KEY_ALTSHIFTF9:
      ViewerConfig();
      if ( Opt.ShowKeyBarViewer )
        ViewKeyBar.Show();
      return TRUE;
    /* SVS $ */

    default:
      return(View.ProcessKey(Key));
  }
}


int FileViewer::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  F3KeyOnly=FALSE;
  if (!View.ProcessMouse(MouseEvent))
    if (!ViewKeyBar.ProcessMouse(MouseEvent))
      return(FALSE);
  return(TRUE);
}


int FileViewer::GetTypeAndName(char *Type,char *Name)
{
  strcpy(Type,MSG(MScreensView));
  View.GetFileName(Name);
  return(MODALTYPE_VIEWER);
}


void FileViewer::ShowConsoleTitle()
{
  View.ShowConsoleTitle();
}


void FileViewer::SetTempViewName(char *Name)
{
  View.SetTempViewName(Name);
}


int FileViewer::GetExitCode()
{
  return(ExitCode);
}

/* $ 28.06.2000 tran
 (NT Console resize)
 resize viewer */
void FileViewer::SetScreenPosition()
{
  if (FullScreen)
  {
    SetPosition(0,0,ScrX,ScrY);
    Show();
  }
}
/* tran $ */
