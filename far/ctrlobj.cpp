/*
ctrlobj.cpp

���������� ���������� ���������, ������� ��������� ���������� � ����

*/

/* Revision: 1.39 14.03.2002 $ */

/*
Modify:
  14.03.2002 SVS
    - ����, �������� ����������� � ����� ������� ��������� :-((
      ���� "�����-������" � "��������-���������"
  21.02.2002 SVS
    ! ������� ������ (���� ����) � ��������� ������ ����� ���������
      ��������, � ���� ����, ��� ������ ������, ��� ��� ��� � ���������
  31.01.2002 SVS
    - BugZ#194 - �� ����������� ������� � ������� ��������/���������� ���
      ������ �� FAR�
      �� �����������, ������� ���� ���������, � ����� ExitAll() ������.
      �����, ����.
  14.12.2001 SVS
    ! ������� �������� ����� ������� "�������"
  15,11,2001 SVS
    ! ��� ������� �� *History �������� ���.
    - ��� ������������ Interface\ShowMenuBar=1 ��� ��� ������ �����,
      �.�. � panel.cpp ���-�� (IS) ������� �����-�� ����� TopMenuBar->Show(),
      � ���-�� (��� � �� ���� ���)... � ����� Long History
  29.10.2001 IS
    ! SaveEditorPos ��������� � EditorOptions
  24.10.2001 SVS
    ! ����������� "������" ���
  26.09.2001 SVS
    - ����: ��� ������ ������� ��������� ������� �������
  25.07.2001 SVS
    ! Copyright ��������� � global.cpp.
  06.07.2001 OT
    - ��� � ������������� ��� ������ ��������� � ������������ �������� �������
  23.06.2001 OT
    - far -r
  30.05.2001 OT
    ! ������� ���������� �� ��������������� ����� ������
  16.05.2001 DJ
    ! proof-of-concept
  15.05.2001 OT
    ! NWZ -> NFZ
  12.05.2001 DJ
    ! FrameManager ������� �� CtrlObject
    ! ���������� ��������� �� CtrlObject �������� ����
  11.05.2001 OT
    ! ��������� Background
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
  28.04.2001 VVM
    + KeyBar ���� ����� ������������ �������.
  22.04.2001 SVS
    ! �������� ������� - ����� �������� ���� �������� ��������
  02.04.2001 VVM
    + ��������� Opt.FlagPosixSemantics
  28.02.2001 IS
    ! �.�. CmdLine ������ ���������, �� ���������� ������
      "CmdLine." �� "CmdLine->" � ���������� ��������/������ �� � ������������
      � ����������� CtrlObject.
  09.02.2001 IS
    + ����������� ��������� ����� "���������� ������"
  09.01.2001 SVS
    + ����� ������� Opt.ShiftsKeyRules (WaitInFastFind)
  29.12.2000 IS
    + ��������� ��� ������, ��������� �� ��� ���������� �����. ���� ���, ��
      �� ������� �� ����.
  15.12.2000 SVS
    ! ����� ShowCopyright - public static & �������� Flags.
      ���� Flags&1, �� ������������ printf ������ ���������� �������
  25.11.2000 SVS
    ! Copyright � 2 ������
  27.09.2000 SVS
    ! Ctrl-Alt-Shift - ���������, ���� ����.
  19.09.2000 IS
    ! ��������� ������� �� ctrl-l|q|t ������ �������� �������� ������
  19.09.2000 SVS
    + Opt.PanelCtrlAltShiftRule ������ ��������� Ctrl-Alt-Shift ��� �������.
  19.09.2000 SVS
    + ��������� ������� ������ ���������� � ������� �� CtrlAltShift
  07.09.2000 tran 1.05
    + Current File
  15.07.2000 tran
    + � � ��� �������� :) ����� ����� ����� Redraw
  13.07.2000 SVS
    ! ��������� ��������� �� ���������� ���� ;-)
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  29.06.2000 tran
    ! �������� � ��������� ���������� �� copyright.inc
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "ctrlobj.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "manager.hpp"
#include "cmdline.hpp"
#include "hilight.hpp"
#include "grpsort.hpp"
#include "poscache.hpp"
#include "history.hpp"
#include "treelist.hpp"
#include "filter.hpp"
#include "filepanels.hpp"

ControlObject *CtrlObject;

ControlObject::ControlObject()
{
  _OT(SysLog("[%p] ControlObject::ControlObject()", this));
  FPanels=0;
  CtrlObject=this;
  /* $ 06.05.2001 DJ
     ������� ����������� (��� ���������� dependencies)
  */
  HiFiles = new HighlightFiles;
  GrpSort = new GroupSort;
  ViewerPosCache = new FilePositionCache;
  EditorPosCache = new FilePositionCache;
  FrameManager = new Manager;
  /* DJ $ */
  ReadConfig();
  /* $ 28.02.2001 IS
       �������� ����������� ������ ����� ����, ��� ��������� ���������
  */
  CmdLine=new CommandLine;
  /* IS $ */

  CmdHistory=new History(HISTORYTYPE_CMD,"SavedHistory",&Opt.SaveHistory,FALSE,FALSE);
  FolderHistory=new History(HISTORYTYPE_FOLDER,"SavedFolderHistory",&Opt.SaveFoldersHistory,FALSE,TRUE);
  ViewHistory=new History(HISTORYTYPE_VIEW,"SavedViewHistory",&Opt.SaveViewHistory,TRUE,TRUE);

  FolderHistory->SetAddMode(TRUE,2,TRUE);
  ViewHistory->SetAddMode(TRUE,Opt.FlagPosixSemantics?1:2,TRUE);
  if (Opt.SaveHistory)
    CmdHistory->ReadHistory();
  if (Opt.SaveFoldersHistory)
    FolderHistory->ReadHistory();
  if (Opt.SaveViewHistory)
    ViewHistory->ReadHistory();
  RegVer=-1;
}


void ControlObject::Init()
{
  TreeList::ClearCache(0);
  PanelFilter::InitFilter();

  SetColor(F_LIGHTGRAY|B_BLACK);
  GotoXY(0,ScrY-3);
  while (RegVer==-1)
    Sleep(0);
  ShowCopyright();
  GotoXY(0,ScrY-2);

  char TruncRegName[512];
  strcpy(TruncRegName,RegName);
  char *CountPtr=strstr(TruncRegName," - (");
  if (CountPtr!=NULL && isdigit(CountPtr[4]) && strchr(CountPtr+5,'/')!=NULL &&
      strchr(CountPtr+6,')')!=NULL)
    *CountPtr=0;
  if (RegVer)
    mprintf("%s: %s",MSG(MRegistered),TruncRegName);
  else
    Text(MShareware);

  CmdLine->SaveBackground(0,0,ScrX,ScrY);

  FPanels=new FilePanels();
  this->MainKeyBar=&(FPanels->MainKeyBar);
  this->TopMenuBar=&(FPanels->TopMenuBar);
  FPanels->Init();
  FPanels->SetScreenPosition();

  _beginthread(CheckVersion,0x10000,NULL);

  Cp()->LeftPanel->Update(0);
  Cp()->RightPanel->Update(0);
  /* $ 07.09.2000 tran
    + Config//Current File */
  if (Opt.AutoSaveSetup)
  {
      Cp()->LeftPanel->GoToFile(Opt.LeftCurFile);
      Cp()->RightPanel->GoToFile(Opt.RightCurFile);
  }
  /* tran 07.09.2000 $ */

  CmdLine->Show();
  if(Opt.ShowKeyBar)
    this->MainKeyBar->Show();

  Plugins.LoadPlugins();
  FrameManager->InsertFrame(FPanels);

  char StartCurDir[NM];
  Cp()->ActivePanel->GetCurDir(StartCurDir);
  chdir(StartCurDir);
  Cp()->ActivePanel->SetFocus();

//  _SVS(SysLog("ActivePanel->GetCurDir='%s'",StartCurDir));
//  _SVS(char PPP[NM];Cp()->GetAnotherPanel(Cp()->ActivePanel)->GetCurDir(PPP);SysLog("AnotherPanel->GetCurDir='%s'",PPP));
}

void ControlObject::CreateFilePanels()
{
  FPanels=new FilePanels();
}

ControlObject::~ControlObject()
{
  _OT(SysLog("[%p] ControlObject::~ControlObject()", this));
  if (Cp()&&Cp()->ActivePanel!=NULL)
  {
    if (Opt.AutoSaveSetup)
      SaveConfig(0);
    if (Cp()->ActivePanel->GetMode()!=PLUGIN_PANEL)
    {
      char CurDir[NM];
      Cp()->ActivePanel->GetCurDir(CurDir);
      FolderHistory->AddToHistory(CurDir,NULL,0);
    }
  }

  FrameManager->CloseAll();
  FPanels=NULL;

  Plugins.SendExit();
  PanelFilter::CloseFilter();
  delete CmdHistory;
  delete FolderHistory;
  delete ViewHistory;
  delete CmdLine;
  /* $ 06.05.2001 DJ
     ������� ��, ��� ������� �����������
  */
  delete HiFiles;
  delete GrpSort;

  if (Opt.SaveViewerPos)
    ViewerPosCache->Save("Viewer\\LastPositions");
  delete ViewerPosCache;

  if (Opt.EdOpt.SavePos)
    EditorPosCache->Save("Editor\\LastPositions");

  delete EditorPosCache;
  delete FrameManager;
  /* DJ $ */
  Lang.Close();
  CtrlObject=NULL;
}


/* $ 25.11.2000 SVS
   Copyright � 2 ������
*/
/* $ 15.12.2000 SVS
 ����� ShowCopyright - public static & �������� Flags.
*/
void ControlObject::ShowCopyright(DWORD Flags)
{
  char Str[256];
  char *Line2=NULL;
  strcpy(Str,Copyright);
  char Xor=17;
  for (int I=0;Str[I];I++)
  {
    Str[I]=(Str[I]&0x7f)^Xor;
    Xor^=Str[I];
    if(Str[I] == '\n')
    {
      Line2=&Str[I+1];
      Str[I]='\0';
    }
  }
  if(Flags&1)
  {
    fprintf(stderr,"%s\n%s\n",Str,Line2);
  }
  else
  {
#ifdef BETA
    mprintf("Beta version %d.%02d.%d",BETA/1000,(BETA%1000)/10,BETA%10);
#else
    ScrollScreen(2+Line2?1:0);
    if(Line2)
    {
      GotoXY(0,ScrY-4);
      Text(Str);
      GotoXY(0,ScrY-3);
      Text(Line2);
    }
    else
      Text(Str);
#endif
  }
}
/* SVS $ */
/* SVS $ */


FilePanels* ControlObject::Cp()
{
  return FPanels;
}
