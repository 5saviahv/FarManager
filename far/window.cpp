/*
frame.cpp

Parent class ��� ���������� ��ꥪ⮢

*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

Frame::Frame()
{
  EnableSwitch=FALSE;
  ExitCode=-1;
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
