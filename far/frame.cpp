/*
frame.cpp

Parent class ��� ����������� ��������

*/

/* Revision: 1.03 07.05.2001 $ */

/*
Modify:
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

Frame::Frame()
{
  CanLoseFocus=FALSE;
  ExitCode=-1;
  KeyBarVisible=MacroMode=0;
  ModalKeyBar=NULL;
}

void Frame::SetKeyBar(KeyBar *ModalKeyBar)
{
  Frame::ModalKeyBar=ModalKeyBar;
}

void Frame::UpdateKeyBar()
{
    SysLog("Frame::UpdateKeyBar(), ModalKeyBar=0x%p",ModalKeyBar);
    if ( ModalKeyBar!=NULL && KeyBarVisible )
        ModalKeyBar->RedrawIfChanged();
}
