/*
frame.cpp

Parent class ��� ����������� ��������

*/

/* Revision: 1.06 12.05.2001 $ */

/*
Modify:
  12.05.2001 DJ
    + IsTopFrame()
    + ��������� �� OnChangeFocus ������� ��������� ����������
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  07.05.2001 DJ
    ! �������� ��������������
  06.05.2001 DJ
    ! �������� #include
  07.05.2001 OT
    - � ������������ ������������������ �� ��� �����. ��-�� ����� ��� �������� :(
  05.05.2001 DJ
    + created
*/

#include "headers.hpp"
#pragma hdrstop

#include "frame.hpp"
#include "fn.hpp"
#include "keybar.hpp"
#include "manager.hpp"

Frame::Frame()
{
  _OT(SysLog("[%p] Frame::Frame()", this));
  CanLoseFocus=FALSE;
  ExitCode=-1;
  KeyBarVisible=MacroMode=0;
  ModalKeyBar=NULL;
}

Frame::~Frame()
{
  _OT(SysLog("[%p] Frame::~Frame()", this));
}

void Frame::SetKeyBar(KeyBar *ModalKeyBar)
{
  Frame::ModalKeyBar=ModalKeyBar;
}

void Frame::UpdateKeyBar()
{
    _D(SysLog("Frame::UpdateKeyBar(), ModalKeyBar=0x%p",ModalKeyBar));
    if ( ModalKeyBar!=NULL && KeyBarVisible )
        ModalKeyBar->RedrawIfChanged();
}

/* $ 12.05.2001 DJ */
int Frame::IsTopFrame()
{
  return FrameManager->GetCurrentFrame() == this;
}

void Frame::OnChangeFocus (int focus)
{
  if (focus)
    Show();
}

/* DJ $ */
