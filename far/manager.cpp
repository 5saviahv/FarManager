/*
manager.cpp

������������ ����� ����������� file panels, viewers, editors, dialogs

*/

/* Revision: 1.54 08.10.2001 $ */

/*
Modify:
  04.10.2001 OT
    ������ ������������ ������ � ��������� ������
  21.09.2001 SVS
    ! �������� ������
  18.09.2001 SVS
    ! ��������� "_D(" ������� �� "_OT(".
      2OT: �� �� ������? � �� ��� _ALGO ���� ������ ��� ����� ������� :-)
  31.08.2001 OT
    - �������� ExitAll() ��� far -e � ��������� �����
  23.08.2001 OT
    - ����������� ��������� ���� �� F10 �� ������� � �������������� �������
      � ���������. ����� ���� �������� ������� ExitAll()
  08.08.2001 OT
    - ����������� CtrlF10 � ���������/������
  28.07.2001 OT
    - ����������� ������������ Reversi
  27.07.2001 SVS
    - ��� ���������� ������� ��������� ��� ����, ��� ��� ����� ����
      ��������� (� ���� �� �� � ��� ������� ����� �����)
    ! KEY_CTRLALTSHIFTPRESS ��� �� �����������������
  26.07.2001 OT
    - ����������� far /e - AltF9
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  25.07.2001 SVS
    ! ������������� ������� ShowTime(1); � ActivateCommit() ��� ����, �����
      ��������� ���� (��� ������ ��� ����� ���� ���� ������ :-)
  25.07.2001 SVS
    ! �� ����� ���������� ������� �� ����� �������� ���������� ������ :-)
      �� ���� ��� ������� ���� IsProcessAssignMacroKey
  24.07.2001 OT
    - ����������� ��������� CAS ��� 2-� � ����� ��������� ��������
  24.07.2001 SVS
    ! ������� ���� NotUseCAS - ����� �� �������� ������ ��� ����������
      ��������/������� (far /e)
  23.07.2001 SVS
    ! ����������� ���� WaitInFastFind - ��� ����� ��� �� �� �����.
  19.07.2001 OT
    + ���������� ����� ����� � ������ ���� Unmodalize���
  18.07.2001 OT
    ! VFMenu
  11.07.2001 OT
    ! ������� CtrlAltShift � Manager
  07.07.2001 IS
    + ��� ������ ������� (F12), ���� ������� ������ 10, �� ���������� ���
      ������� ������ ����� ���������� ��������, �.�. �������� ����� �� 10,
      � 36 ������� ������.
  29.06.2001 OT
    - ������������ ��� UpdateCommit()
  26.06.2001 SKV
    + PluginCommit(); ����� ACTL_COMMIT
  23.06.2001 OT
    - ������� �������� "������� �������"
    - ����� �������� ��� far -r. FramePos ������ ���������������� �������� -1.
  14.06.2001 OT
    ! "����" ;-)
  06.06.2001 OT
    - ���������� ���� ����������� � ������� ������ � ��������: AltF7->Enter->F3->F6->F6->:((
    - ���������� �����-�� � ExecuteFrame()...
  04.06.2001 OT
     �������� ��� "�������������" ���������� ������
  30.05.2001 OT
    - ��� ���� memory leak ����� F6 � ���������/������. ���������� ������� UpdateCommit()
    - ���������� CloseAll() � ExitAll() � �������� NFZ.
    - ����������� ActivateCommit(). ��� ��������� ���������������
      �������� "�������������" ActivatedFrame.
    + AltF9 ������� ������ �� ������ � �������, �� � ... ����� :)
  28.05.2001 OT
    - ����������� "���� ����� F3 �������� ����������" (��������� DeleteCommit())
  26.05.2001 OT
    - ����������� ExucuteModal()
    + ����� ������ ExecuteComit(), ExecuteFrame(), IndexOfStack()
    + ����� ���� Frame *ExecutedFrame;
    ! ����������� ������� DeleteCommit(), UpdateCommit(), ��������� � ���������� ExecuteFrame
    ! ����������� ��������� RefreshCommit() � ������ ����������.
  25.05.2001 DJ
    - ��������� ���� ��� �������� Alt-F7.
  23.05.2001 OT
    - ����������� ���� - �������� Frame, �� ���������� � ������ FrameList
  22.05.2001 OT
    + ��������� RefreshedFrame
  22.05.2001 DJ
    ! � ExecuteModal() ������ ����� ������ ����� commit (���� ��������
      ��������� ��������, �������� ������ �����)
  21.05.2001 DJ
    ! ������ �������������; � ����� � ���������� ������ ���� �������
      �������� GetFrameTypesCount(); �� ���������� OnChangeFocus(0)
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  16.05.2001 DJ
    ! ����������� ExecuteModal()
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 OT
    - ������ � F4 -> ReloadAgain
  12.05.2001 DJ
    ! ����� ModalFrame.Show() � Manager::ExecuteModal()
  12.05.2001 DJ
    ! FrameManager ������� �� CtrlObject
    ! ���������� ExecuteModal() � ExecuteModalPtr() (� � ��� � �����, �����
      ����� ��� �������?)
    ! ReplaceCurrentFrame() �������� �� ����� ������������� ReplaceFrame()
      (� ������ ��)
  11.05.2001 OT
    ! ��������� Background
  10.05.2001 DJ
    + SwitchToPanels()
    * GetFrameTypesCount() �� ��������� �����, ������� �� ��������� �������
    + ModalStack
    - ������ ��������� ������ DestroyFrame() � ��� � ����
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  07.05.2001 DJ
    ! ��������� � ������� CloseAll() � ExitAll()
  06.05.2001 DJ
    ! �������� #include
    + ReplaceCurrentFrame()
  07.05.2001 ��
    - ��� � �������� ������� �������� ������ FramePos ��� ��������
      ������-������ �� ������ :)
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  04.05.2001 OT
    + ������� ������������� ���� �������� �� F11 (NWZ)
      ��������� PluginSet::CommandsMenu()
  29.04.2001 ��
    + ��������� NWZ �� ����������
  29.12.2000 IS
    + ����� ExitAll - ������ CloseAll, �� ��������� ����������� �����������
      ������ � ����, ���� ������������ ��������� ������������� ����.
      ���������� TRUE, ���� ��� ������� � ����� �������� �� ����.
  28.07.2000 tran 1.04
    + ��������� ��� ������ ������ ���� -
      ���������� ����� � ��������� ����������� "*"
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  28.06.2000 tran
    - NT Console resize
      add class member ActiveModal
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "manager.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "frame.hpp"
#include "vmenu.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "savescr.hpp"
#include "cmdline.hpp"
#include "ctrlobj.hpp"

Manager *FrameManager;

Manager::Manager()
{
  FrameList=NULL;
  FrameCount=FrameListSize=0;
  FramePos=-1;
  ModalStack=NULL;
  FrameList=(Frame **)realloc(FrameList,sizeof(*FrameList)*(FrameCount+1));

  ModalStack=NULL;
  ModalStackSize = ModalStackCount = 0;
  EndLoop = FALSE;
  RefreshedFrame=NULL;

  CurrentFrame  = NULL;
  InsertedFrame = NULL;
  DeletedFrame  = NULL;
  ActivatedFrame= NULL;
  DeactivatedFrame=NULL;
  ModalizedFrame=NULL;
  UnmodalizedFrame=NULL;
  ExecutedFrame=NULL;
  SemiModalBackFrame=NULL;
}

Manager::~Manager()
{
  if (FrameList)
    free(FrameList);
  if (ModalStack)
    free (ModalStack);
}


/* $ 29.12.2000 IS
  ������ CloseAll, �� ��������� ����������� ����������� ������ � ����,
  ���� ������������ ��������� ������������� ����.
  ���������� TRUE, ���� ��� ������� � ����� �������� �� ����.
*/
BOOL Manager::ExitAll()
{
  int i;
  for (i=FrameCount-1; i>=0; i--){
    Frame *iFrame=FrameList[i];
    if (!iFrame->GetCanLoseFocus(TRUE)){
      ActivateFrame(iFrame);
      Commit();
      int PrevFrameCount=FrameCount;
      iFrame->ProcessKey(KEY_ESC);
      Commit();
      if (PrevFrameCount==FrameCount){
        return FALSE;
      }
    }
  }
  return TRUE;
}
/* IS $ */

void Manager::CloseAll()
{
  int i;
  Frame *iFrame;
  for (i=ModalStackCount-1;i>=0;i--){
    iFrame=ModalStack[i];
    DeleteFrame(iFrame);
    DeleteCommit();
    DeletedFrame=NULL;
  }
  for (i=FrameCount-1;i>=0;i--){
    iFrame=(*this)[i];
    DeleteFrame(iFrame);
    DeleteCommit();
    DeletedFrame=NULL;
  }
  /* $ 13.07.2000 SVS
     ����� ���� "delete ModalList;", �� ����������������� ������� ������
     ���� ����� realloc...
  */
  free(FrameList);
  /* SVS $ */
  FrameList=NULL;
  FrameCount=FramePos=0;
}

BOOL Manager::IsAnyFrameModified(int Activate)
{
  for (int I=0;I<FrameCount;I++)
    if (FrameList[I]->IsFileModified())
    {
      if (Activate)
      {
        ActivateFrame(I);
        Commit();
      }
      return(TRUE);
    }

  return(FALSE);
}

void Manager::InsertFrame(Frame *Inserted, int Index)
{
  _OT(SysLog("InsertFrame(), Inserted=%p, Index=%i",Inserted, Index));
  if (Index==-1)
    Index=FramePos;
  InsertedFrame=Inserted;
}

void Manager::DeleteFrame(Frame *Deleted)
{
  _OT(SysLog("DeleteFrame(), Deleted=%p",Deleted));
  for (int i=0;i<FrameCount;i++){
    Frame *iFrame=FrameList[i];
    if(iFrame->RemoveModal(Deleted)){
      return;
    }
  }
  if (!Deleted){
    DeletedFrame=CurrentFrame;
  } else {
    DeletedFrame=Deleted;
  }
}

void Manager::DeleteFrame(int Index)
{
  _OT(SysLog("DeleteFrame(), Index=%i",Index));
  DeleteFrame(this->operator[](Index));
}


void Manager::ModalizeFrame (Frame *Modalized, int Mode)
{
  _OT(SysLog("ModalizeFrame(), Modalized=%p",Modalized));
  ModalizedFrame=Modalized;
  ModalizeCommit();
}

void Manager::UnmodalizeFrame (Frame *Unmodalized)
{
  UnmodalizedFrame=Unmodalized;
  UnmodalizeCommit();
}

void Manager::ExecuteNonModal ()
{
  _OT(SysLog("ExecuteNonModal(), ExecutedFrame=%p, InsertedFrame=%p, DeletedFrame=%p",ExecutedFrame, InsertedFrame, DeletedFrame));
  Frame *NonModal=InsertedFrame?InsertedFrame:(ExecutedFrame?ExecutedFrame:ActivatedFrame);
  if (!NonModal) {
    return;
  }
  Frame *SaveFrame=SemiModalBackFrame;
  SemiModalBackFrame=CurrentFrame;
//  int SavePos = FramePos;
  int NonModalIndex=IndexOf(NonModal);
  if (-1==NonModalIndex){
    InsertedFrame=NonModal;
    ExecutedFrame=NULL;
    InsertCommit();
    InsertedFrame=NULL;
  } else {
    ActivateFrame(NonModalIndex);
  }

  ExecuteModal(NonModal);
  SemiModalBackFrame=SaveFrame;
}

void Manager::ExecuteModal (Frame *Executed)
{
  _OT(SysLog("ExecuteModal(), Executed=%p, ExecutedFrame=%p",Executed,ExecutedFrame));
  if (!Executed && !ExecutedFrame){
    return;
  }
  if (Executed){
    if (ExecutedFrame) {
      _OT(SysLog("������� � ����� ����� ��������� � ��������� ������ ��� ������. Executed=%p, ExecitedFrame=%p",Executed, ExecutedFrame));
      return;// NULL; //?? ����������, ����� �������� ��������� ���������� � ���� ������
    } else {
      ExecutedFrame=Executed;
    }
  }

  int ModalStartLevel=ModalStackCount;
  while (1){
    Commit();
    if (ModalStackCount<=ModalStartLevel){
      break;
    }
    ProcessMainLoop();
  }
  return;// GetModalExitCode();
}

int Manager::GetModalExitCode()
{
  return ModalExitCode;
}

/*!
  \return ���������� NULL ���� ����� "�����" ��� ���� ����� ������� �����.
  ������� �������, ���� ����������� ����� �� ���������.
  ���� �� ����� ���������, �� ����� ������� ������ ����������
  ��������� �� ���������� �����.
*/
Frame *Manager::FrameMenu()
{
  int ExitCode;
  {
    struct MenuItem ModalMenuItem;
    memset(&ModalMenuItem,0,sizeof(ModalMenuItem));
    VMenu ModalMenu(MSG(MScreensTitle),NULL,0,ScrY-4);
    ModalMenu.SetHelp("ScrSwitch");
    ModalMenu.SetFlags(VMENU_WRAPMODE);
    ModalMenu.SetPosition(-1,-1,0,0);

    for (int I=0;I<FrameCount;I++)
    {
      char Type[200],Name[NM],NumText[100];
      FrameList[I]->GetTypeAndName(Type,Name);
      /* $ 07.07.2001 IS
         ���� ������� ������ 10, �� ���������� ��� ������� ������ �����
         ���������� ��������, �.�. �������� ����� �� 10, � 36 ������� ������.
      */
      if (I<10)
        sprintf(NumText,"&%d. ",I);
      else if (I<36)
        sprintf(NumText,"&%c. ",I+55); // 55='A'-10
      else
        strcpy(NumText,"&   ");
      /* IS $ */
      /* $ 28.07.2000 tran
         ���� ������� �� ������ ������ */
      TruncPathStr(Name,ScrX-24);
      /*  ����������� "*" ���� ���� ������� */
      sprintf(ModalMenuItem.Name,"%s%-10.10s %c %s",NumText,Type,(FrameList[I]->IsFileModified()?'*':' '),Name);
      /* tran 28.07.2000 $ */
      ModalMenuItem.SetSelect(I==FramePos);
      ModalMenu.AddItem(&ModalMenuItem);
    }
    ModalMenu.Process();
    ExitCode=ModalMenu.Modal::GetExitCode();
  }
  if (ExitCode>=0)
  {
    ActivateFrame (ExitCode);
  }
  return (ActivatedFrame==CurrentFrame?NULL:CurrentFrame);
}


int Manager::GetFrameCountByType(int Type)
{
  int ret=0;
  for (int I=0;I<FrameCount;I++)
  {
    /* $ 10.05.2001 DJ
       �� ��������� �����, ������� ���������� �������
    */
    if (FrameList[I] == DeletedFrame || FrameList [I]->GetExitCode() == XC_QUIT)
      continue;
    /* DJ $ */
    if (FrameList[I]->GetType()==Type)
      ret++;
  }
  return ret;
}

void Manager::SetFramePos(int NewPos)
{
  _OT(SysLog("Manager::SetFramePos(), NewPos=%i",NewPos));
  FramePos=NewPos;
}

/*$ 11.05.2001 OT ������ ����� ������ ���� �� ������ �� ������� �����, �� � �������� - ����, �������� ��� */
int  Manager::FindFrameByFile(int ModalType,char *FileName,char *Dir)
{
  char bufFileName[NM];
  char *FullFileName=FileName;
  if (Dir){
    strcpy(bufFileName,Dir);
    AddEndSlash(bufFileName);
    strcat(bufFileName,FileName);
    FullFileName=bufFileName;
  }
  for (int I=0;I<FrameCount;I++)
  {
    char Type[200],Name[NM];
    if (FrameList[I]->GetTypeAndName(Type,Name)==ModalType)
      if (LocalStricmp(Name,FullFileName)==0)
        return(I);
  }
  return(-1);
}
/* 11.05.2001 OT $*/

void Manager::ShowBackground()
{
  if (!RegVer)
  {
    Message(MSG_WARNING,1,MSG(MWarning),MSG(MRegOnly),MSG(MOk));
    return;
  }
  CtrlObject->CmdLine->ShowBackground();
}


void Manager::ActivateFrame(Frame *Activated)
{
  _OT(SysLog("ActivateFrame(), Activated=%i",Activated));
  if (!ActivatedFrame){
    ActivatedFrame=Activated;
  }
}

void Manager::ActivateFrame(int Index)
{
  _OT(SysLog("ActivateFrame(), Index=%i",Index));
  ActivateFrame((*this)[Index]);
}

void Manager::DeactivateFrame (Frame *Deactivated,int Direction)
{
  _OT(SysLog("DeactivateFrame(), Deactivated=%p",Deactivated));
  if (Direction) {
    FramePos+=Direction;
    if (Direction>0){
      if (FramePos>=FrameCount){
        FramePos=0;
      }
    } else {
      if (FramePos<0) {
        FramePos=FrameCount-1;
      }
    }
    ActivateFrame(FramePos);
  } else {
    // Direction==0
    // Direct access from menu or (in future) from plugin
  }
  DeactivatedFrame=Deactivated;
}

void Manager::RefreshFrame(Frame *Refreshed)
{
  _OT(SysLog("RefreshFrame(), Refreshed=%p",Refreshed));
  if (ActivatedFrame)
    return;
  if (Refreshed){
    RefreshedFrame=Refreshed;
  } else {
    RefreshedFrame=CurrentFrame;
  }
}

void Manager::RefreshFrame(int Index)
{
  RefreshFrame((*this)[Index]);
}

void Manager::ExecuteFrame(Frame *Executed)
{
  _OT(SysLog("ExecuteFrame(), Executed=%p",Executed));
  ExecutedFrame=Executed;
}


/* $ 10.05.2001 DJ
   ������������� �� ������ (����� � ������� 0)
*/

void Manager::SwitchToPanels()
{
  ActivateFrame (0);
}

/* DJ $ */


int Manager::HaveAnyFrame()
{
    if ( FrameCount || InsertedFrame || DeletedFrame || ActivatedFrame || RefreshedFrame ||
         ModalizedFrame || DeactivatedFrame || ExecutedFrame || CurrentFrame)
        return 1;
    return 0;
}

void Manager::EnterMainLoop()
{
  WaitInFastFind=0;
  while (1)
  {
    Commit();
    if (EndLoop || !HaveAnyFrame()) {
      break;
    }
    ProcessMainLoop();
  }
}

void Manager::ProcessMainLoop()
{

  WaitInMainLoop=IsPanelsActive();

  //WaitInFastFind++;
  int Key=GetInputRecord(&LastInputRecord);
  //WaitInFastFind--;
  WaitInMainLoop=FALSE;
  if (EndLoop)
    return;
  if (LastInputRecord.EventType==MOUSE_EVENT)
    ProcessMouse(&LastInputRecord.Event.MouseEvent);
  else
    ProcessKey(Key);
}

void Manager::ExitMainLoop(int Ask)
{
  if (!Ask || !Opt.Confirm.Exit || Message(0,2,MSG(MQuit),MSG(MAskQuit),MSG(MYes),MSG(MNo))==0)
   /* $ 29.12.2000 IS
      + ���������, ��������� �� ��� ���������� �����. ���� ���, �� �� �������
        �� ����.
   */
   if(ExitAll())
   /* IS $ */
    if (!CtrlObject->Cp()->LeftPanel->ProcessPluginEvent(FE_CLOSE,NULL) && !CtrlObject->Cp()->RightPanel->ProcessPluginEvent(FE_CLOSE,NULL))
      EndLoop=TRUE;
}

int  Manager::ProcessKey(int Key)
{
  int ret=FALSE;
  _OT(char kn[32]);
  _OT(KeyToText(Key,kn));
  //    _D(SysLog(1,"Manager::ProcessKey(), key=%i, '%s'",Key,kn));

  if ( CurrentFrame)
  {
    //      _D(SysLog("Manager::ProcessKey(), to CurrentFrame 0x%p, '%s'",CurrentFrame, CurrentFrame->GetTypeName()));
    int i;

    /*** ���� ����������������� ������ ! ***/
    /***   ������� ������ �����������    ***/
    switch(Key)
    {
      case KEY_CONSOLE_BUFFER_RESIZE:
        _OT(SysLog("[%p] Manager::ProcessKey(KEY_CONSOLE_BUFFER_RESIZE)",this));
        for (i=0;i<FrameCount;i++){
          FrameList[i]->ResizeConsole();
        }
        for (i=0;i<ModalStackCount;i++){
          ModalStack[i]->ResizeConsole();
        }
        ImmediateHide();
        FrameManager->RefreshFrame();
        return TRUE;
    }

    /*** � ��� ����� - ��� ���������! ***/
    if(!IsProcessAssignMacroKey || IsProcessVE_FindFile)
       // � ����� ������ ���� ����-�� ������� ��� ������� ���
    {
      /* ** ��� ������� ��������� ��� ������ �������/���������
            �� ����� ������ ����� �� ������ ������ ** */
      switch(Key)
      {
        case KEY_F11:
          PluginsMenu();
          FrameManager->RefreshFrame();
          _OT(SysLog(-1));
          return TRUE;
        case KEY_ALTF9:
          _OT(SysLog("Manager::ProcessKey, KEY_ALTF9 pressed..."));
          SetVideoMode(FarAltEnter(-2));
          return TRUE;
      }

      // � ����� ��, ��� ����� ���� ��������� ����� :-)
      if(!IsProcessVE_FindFile)
      {
        switch(Key)
        {
          case KEY_CTRLALTSHIFTPRESS:
            if(!NotUseCAS)
            {
              if (CurrentFrame->FastHide()){
                ImmediateHide();
                WaitKey(KEY_CTRLALTSHIFTRELEASE);
                FrameManager->RefreshFrame();
              }
            }
            return TRUE;
          case KEY_F12:
            if (CurrentFrame->GetCanLoseFocus())
              DeactivateFrame(FrameMenu(),0);
            _OT(SysLog(-1));
            return TRUE;
          case KEY_CTRLTAB:
          case KEY_CTRLSHIFTTAB:
            if (CurrentFrame->GetCanLoseFocus()){
              DeactivateFrame(CurrentFrame,Key==KEY_CTRLTAB?1:-1);
            }
            _OT(SysLog(-1));
            return TRUE;
        }
      }
    }
    CurrentFrame->UpdateKeyBar();
    CurrentFrame->ProcessKey(Key);
  }
  _OT(SysLog(-1));
  return ret;
}

int  Manager::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
    int ret=FALSE;
//    _D(SysLog(1,"Manager::ProcessMouse()"));
    if ( CurrentFrame)
        ret=CurrentFrame->ProcessMouse(MouseEvent);
//    _D(SysLog("Manager::ProcessMouse() ret=%i",ret));
    _OT(SysLog(-1));
    return ret;
}

void Manager::PluginsMenu()
{
  _OT(SysLog(1));
  int curType = CurrentFrame->GetType();
  if (curType == MODALTYPE_PANELS || curType == MODALTYPE_EDITOR || curType == MODALTYPE_VIEWER)
    CtrlObject->Plugins.CommandsMenu(curType,0,0);
  _OT(SysLog(-1));
}

BOOL Manager::IsPanelsActive()
{
  if (FramePos>=0) {
    return CurrentFrame?CurrentFrame->GetType() == MODALTYPE_PANELS:FALSE;
  } else {
    return FALSE;
  }
}

Frame *Manager::operator[](int Index)
{
  if (Index<0||Index>=FrameCount ||FrameList==0){
    return NULL;
  }
  return FrameList[Index];
}

int Manager::IndexOfStack(Frame *Frame)
{
  int Result=-1;
  for (int i=0;i<ModalStackCount;i++)
  {
    if (Frame==ModalStack[i])
    {
      Result=i;
      break;
    }
  }
  return Result;
}

int Manager::IndexOf(Frame *Frame)
{
  int Result=-1;
  for (int i=0;i<FrameCount;i++)
  {
    if (Frame==FrameList[i])
    {
      Result=i;
      break;
    }
  }
  return Result;
}

BOOL Manager::Commit()
{
  _OT(SysLog(1));
  int Result = false;
  if (DeletedFrame && (InsertedFrame||ExecutedFrame)){
    UpdateCommit();
    DeletedFrame = NULL;
    InsertedFrame = NULL;
    ExecutedFrame=NULL;
    Result=true;
  } else if (ExecutedFrame) {
    ExecuteCommit();
    ExecutedFrame=NULL;
    Result=true;
  } else if (DeletedFrame){
    DeleteCommit();
    DeletedFrame = NULL;
    Result=true;
  } else if (InsertedFrame){
    InsertCommit();
    InsertedFrame = NULL;
    Result=true;
  } else if(DeactivatedFrame){
    DeactivateCommit();
    DeactivatedFrame=NULL;
    Result=true;
  } else if(ActivatedFrame){
    ActivateCommit();
    ActivatedFrame=NULL;
    Result=true;
  } else if (RefreshedFrame){
    RefreshCommit();
    RefreshedFrame=NULL;
    Result=true;
  } else if (ModalizedFrame){
    ModalizeCommit();
//    ModalizedFrame=NULL;
    Result=true;
  } else if (UnmodalizedFrame){
    UnmodalizeCommit();
//    UnmodalizedFrame=NULL;
    Result=true;
  }
  if (Result){
    Result=Commit();
  }
  _OT(SysLog(-1));
  return Result;
}

void Manager::DeactivateCommit()
{
  _OT(SysLog("DeactivateCommit(), DeactivatedFrame=%p",DeactivatedFrame));
  if (!DeactivatedFrame){
    return;
  }
  if (!ActivatedFrame){
    _OT("WARNING!!!!!!!!");
  }
  if (DeactivatedFrame){
    DeactivatedFrame->OnChangeFocus(0);
  }

  int modalIndex=IndexOfStack(DeactivatedFrame);
  if (-1 != modalIndex && modalIndex== this-> ModalStackCount-1){
    if (SemiModalBackFrame==ActivatedFrame){
      ModalStackCount--;
    } else {
      ModalStack[ModalStackCount-1]=ActivatedFrame;
    }
  }
}


void Manager::ActivateCommit()
{
  _OT(SysLog("ActivateCommit(), ActivatedFrame=%p",ActivatedFrame));
  if (CurrentFrame==ActivatedFrame){
    RefreshedFrame=ActivatedFrame;
    return;
  }
  int FrameIndex=IndexOf(ActivatedFrame);
  if (-1!=FrameIndex){
    FramePos=FrameIndex;
  }
  RefreshedFrame=CurrentFrame=ActivatedFrame;
}

void Manager::UpdateCommit()
{
  _OT(SysLog("UpdateCommit(), DeletedFrame=%p, InsertedFrame=%p, ExecutedFrame=%p",DeletedFrame,InsertedFrame, ExecutedFrame));
  if (ExecutedFrame){
    DeleteCommit();
    ExecuteCommit();
    return;
  }
  int FrameIndex=IndexOf(DeletedFrame);
  if (-1!=FrameIndex){
    ActivateFrame(FrameList[FrameIndex] = InsertedFrame);
    ActivatedFrame->FrameToBack=CurrentFrame;
    DeleteCommit();
  } else {
    _OT(SysLog("UpdateCommit(). ������ �� ������ ��������� �����"));
  }
}

//! ������� DeletedFrame ��� ���� ��������!
//! ��������� ��������� ��������, (������ �� ����� �������������)
//! �� ������ � ��� ������, ���� �������� ����� ��� �� �������� �������.
void Manager::DeleteCommit()
{
  _OT(SysLog("DeleteCommit(), DeletedFrame=%p",DeletedFrame));
  if (!DeletedFrame){
    return;
  }

  BOOL ifDoubI=ifDoubleInstance();
  if (ModalStackCount){
    ModalStackCount--;
    if (ModalStackCount){
      ActivateFrame(ModalStack[ModalStackCount-1]);
    }
  }

  for (int i=0;i<FrameCount;i++){
    if (FrameList[i]->FrameToBack==DeletedFrame) {
      FrameList[i]->FrameToBack=CtrlObject->Cp();
    }
  }

  int FrameIndex=IndexOf(DeletedFrame);
  if (-1!=FrameIndex) {
    DeletedFrame->DestroyAllModal();
    for (int j=FrameIndex; j<FrameCount-1; j++ ){
      FrameList[j]=FrameList[j+1];
    }
    FrameCount--;
    if ( FramePos >= FrameCount ) {
      FramePos=0;
    }
    if (DeletedFrame->FrameToBack==CtrlObject->Cp()){
      ActivateFrame(FrameList[FramePos]);
    } else {
      ActivateFrame(DeletedFrame->FrameToBack);
    }
  }

  //
  if (ifDoubI && ActivatedFrame!=SemiModalBackFrame){
//    ModalStackCount++;
    ModalStack[ModalStackCount++]=ActivatedFrame;
  }

  DeletedFrame->OnDestroy();
  if (DeletedFrame->GetDynamicallyBorn())
  {
    _tran(SysLog("delete DeletedFrame %p, CurrentFrame=%p",DeletedFrame,CurrentFrame));
    if ( CurrentFrame==DeletedFrame )
      CurrentFrame=0;
    delete DeletedFrame;
  }
  // ���������� �� ��, ��� � ActevateFrame �� ����� ��������� ���
  // �����������  ActivatedFrame
  ActivateFrame(FramePos);
}

void Manager::InsertCommit()
{
  _OT(SysLog("InsertCommit(), InsertedFrame=%p",InsertedFrame));
  if (InsertedFrame){
    if (FrameListSize <= FrameCount)
    {
      FrameList=(Frame **)realloc(FrameList,sizeof(*FrameList)*(FrameCount+1));
      FrameListSize++;
    }
    InsertedFrame->FrameToBack=CurrentFrame;
    FrameList[FrameCount]=InsertedFrame;
    if (!ActivatedFrame){
      ActivatedFrame=InsertedFrame;
    }
    FrameCount++;
  }
}

void Manager::RefreshCommit()
{
  _OT(SysLog("RefreshCommit(), RefreshedFrame=%p,Refreshable()=%i",RefreshedFrame,RefreshedFrame->Refreshable()));
  if (!RefreshedFrame)
    return;
  if (RefreshedFrame->Refreshable()){
    RefreshedFrame->ShowConsoleTitle();
    RefreshedFrame->Refresh();
    CtrlObject->Macro.SetMode(RefreshedFrame->GetMacroMode());
  }
  if (Opt.ViewerEditorClock &&
      (RefreshedFrame->GetType() == MODALTYPE_EDITOR ||
      RefreshedFrame->GetType() == MODALTYPE_VIEWER)
      || WaitInMainLoop && Opt.Clock)
    ShowTime(1);
}

void Manager::ExecuteCommit()
{
  _OT(SysLog("ExecuteCommit(), ExecutedFrame=%p",ExecutedFrame));
  if (!ExecutedFrame) {
    return;
  }
  if (ModalStackCount == ModalStackSize){
    ModalStack = (Frame **) realloc (ModalStack, ++ModalStackSize * sizeof (Frame *));
  }
  ModalStack [ModalStackCount++] = ExecutedFrame;
  ActivatedFrame=ExecutedFrame;
}

/*$ 26.06.2001 SKV
  ��� ������ �� �������� ����������� ACTL_COMMIT
*/
BOOL Manager::PluginCommit()
{
  return Commit();
}
/* SKV$*/

/* $ ������� ��� ���� CtrlAltShift OT */
void Manager::ImmediateHide()
{
  if (FramePos<0){
    return;
  }
  // ������� ���������, ���� �� � ������������ ������ SaveScreen
  if (CurrentFrame->HasSaveScreen()) {
    CurrentFrame->Hide();
    return;
  }
  if (ModalStackCount>0){
    int UnlockCount=0;

    while (!(*this)[FramePos]->Refreshable()){
      (*this)[FramePos]->UnlockRefresh();
      UnlockCount++;
    }
    RefreshFrame((*this)[FramePos]);

    Commit();
    for (int i=0;i<UnlockCount;i++){
      (*this)[FramePos]->LockRefresh();
    }

    if (ModalStackCount>1){
      for (int i=0;i<ModalStackCount-1;i++){
        if (!(ModalStack[i]->FastHide() & CASR_HELP)){
          RefreshFrame(ModalStack[i]);
          Commit();
        } else {
          break;
        }
      }
    }

  } else {
    CtrlObject->CmdLine->ShowBackground();
  }
}

void Manager::ModalizeCommit()
{
  CurrentFrame->Push(ModalizedFrame);
  ModalizedFrame=NULL;
}

void Manager::UnmodalizeCommit()
{
  int i;
  Frame *iFrame;
  for (i=0;i<FrameCount;i++){
    iFrame=FrameList[i];
    if(iFrame->RemoveModal(UnmodalizedFrame)){
      break;
    }
  }
  for (i=0;i<ModalStackCount;i++){
    iFrame=ModalStack[i];
    if(iFrame->RemoveModal(UnmodalizedFrame)){
      break;
    }
  }
  UnmodalizedFrame=NULL;
}
/* OT $*/

BOOL Manager::ifDoubleInstance()
{
  if (0>=ModalStackCount){
    return FALSE;
  }
  Frame *TopOnStack=ModalStack[ModalStackCount-1];
  if (-1==IndexOf(TopOnStack)){
    return FALSE;
  }
  return TRUE;
}
