/*
rdrwdsk.cpp

class RedrawDesktop

*/

/* Revision: 1.07 30.05.2001 $ */

/*
Modify:
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

RedrawDesktop::RedrawDesktop()
{
  CtrlObject->CmdLine->ShowBackground();
  CtrlObject->CmdLine->Show();
  LeftVisible=CtrlObject->Cp()->LeftPanel->IsVisible();
  RightVisible=CtrlObject->Cp()->RightPanel->IsVisible();
}


RedrawDesktop::~RedrawDesktop()
{
  CtrlObject->CmdLine->SaveBackground();
  CtrlObject->CmdLine->Show();
  if (Opt.ShowKeyBar)
    CtrlObject->MainKeyBar->Show();
  if (Opt.ShowMenuBar)
    CtrlObject->TopMenuBar->Show();
  int RightType=CtrlObject->Cp()->RightPanel->GetType();
  if (RightVisible && RightType!=QVIEW_PANEL)
    CtrlObject->Cp()->RightPanel->Show();
  if (LeftVisible)
    CtrlObject->Cp()->LeftPanel->Show();
  if (RightVisible && RightType==QVIEW_PANEL)
    CtrlObject->Cp()->RightPanel->Show();
}
