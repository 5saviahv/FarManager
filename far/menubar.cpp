/*
menubar.cpp

����� ��ਧ��⠫쭮�� ���� �� ����祭��� "Always show menu bar"

*/

/* Revision: 1.01 05.01.2001 $ */

/*
Modify:
  05.01.2001 SVS
    ! ����� "⮫��" ����㤮� - �⠭����� ���ୠ� ���������
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 05.01.2001 SVS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

void MenuBar::DisplayObject()
{
  char Msg[100],FullMsg[500];
  sprintf(Msg,"    %s    %s    %s    %s    %s  ",MSG(MMenuLeftTitle),
          MSG(MMenuFilesTitle),MSG(MMenuCommandsTitle),
          MSG(MMenuOptionsTitle),MSG(MMenuRightTitle));
  RemoveHighlights(Msg);
  int Length=X2-X1+1;
  sprintf(FullMsg,"%-*.*s",Length,Length,Msg);
  GotoXY(X1,Y1);
  SetColor(COL_HMENUTEXT);
  Text(FullMsg);
}
