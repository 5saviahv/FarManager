/*
rdrwdsk.cpp

class RedrawDesktop

*/

/* Revision: 1.08 12.11.2001 $ */

/*
Modify:
  12.11.2001 SVS
    ! ������� ��������� � ����������� �������
  30.05.2001 OT
    - �� ��������������� ��������� ������ ��� ����������/�������������� ������
  21.05.2001 OT
    ! ��� "��������� ������ �������"
  11.05.2001 OT
    ! ��������� Background
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "rdrwdsk.hpp"
#include "global.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "cmdline.hpp"
#include "ctrlobj.hpp"

RedrawDesktop::RedrawDesktop(BOOL IsHidden)
{
  CtrlObject->CmdLine->ShowBackground();
  CtrlObject->CmdLine->Show();
  LeftVisible=CtrlObject->Cp()->LeftPanel->IsVisible();
  RightVisible=CtrlObject->Cp()->RightPanel->IsVisible();
  KeyBarVisible=Opt.ShowKeyBar;//CtrlObject->MainKeyBar->IsVisible();
  TopMenuBarVisible=Opt.ShowMenuBar;//CtrlObject->TopMenuBar->IsVisible();
  if(IsHidden)
  {
    CtrlObject->Cp()->LeftPanel->CloseFile();
    CtrlObject->Cp()->RightPanel->CloseFile();
    CtrlObject->MainKeyBar->Hide();
    CtrlObject->TopMenuBar->Hide();
  }
}


RedrawDesktop::~RedrawDesktop()
{
  CtrlObject->CmdLine->SaveBackground();
  CtrlObject->CmdLine->Show();
  if (KeyBarVisible)
    CtrlObject->MainKeyBar->Show();
  if (TopMenuBarVisible)
    CtrlObject->TopMenuBar->Show();
  int RightType=CtrlObject->Cp()->RightPanel->GetType();
  if (RightVisible && RightType!=QVIEW_PANEL)
    CtrlObject->Cp()->RightPanel->Show();
  if (LeftVisible)
    CtrlObject->Cp()->LeftPanel->Show();
  if (RightVisible && RightType==QVIEW_PANEL)
    CtrlObject->Cp()->RightPanel->Show();
}
