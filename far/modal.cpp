/*
modal.cpp

Parent class ��� �������� ��ꥪ⮢

*/

/* Revision: 1.04 05.05.2001 $ */

/*
Modify:
  05.05.2001 DJ
    + ������� NWZ
  29.04.2001 ��
    + ����७�� NWZ �� ����类��
  11.07.2000 tran
    - trap if no lang files found
  29.06.2000 tran
    - (NT Console resize bug)
      adding virtual method SetScreenPosition
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

Modal::Modal()
{
  ExitCode=-1;
  WriteKey=-1;
  EndLoop=0;
  *HelpTopic=0;
}


void Modal::Process()
{
  Show();

  while (!Done())
  {
    ReadInput();
    ProcessInput();
  }
  GetDialogObjectsData();
}


int Modal::ReadInput()
{
  if (WriteKey>=0)
  {
    ReadKey=WriteKey;
    WriteKey=-1;
  }
  else
    ReadKey=GetInputRecord(&ReadRec);
  return(ReadKey);
}


void Modal::WriteInput(int Key)
{
  WriteKey=Key;
}


void Modal::ProcessInput()
{
  if (ReadRec.EventType==MOUSE_EVENT)
    ProcessMouse(&ReadRec.Event.MouseEvent);
  else
    ProcessKey(ReadKey);
}


int Modal::Done()
{
  return(EndLoop);
}


void Modal::ClearDone()
{
  EndLoop=0;
}


int Modal::GetExitCode()
{
  return(ExitCode);
}


void Modal::SetExitCode(int Code)
{
  ExitCode=Code;
  EndLoop=TRUE;
}


void Modal::SetHelp(char *Topic)
{
  strcpy(HelpTopic,Topic);
}


void Modal::ShowHelp()
{
  if (*HelpTopic)
    Help Hlp(HelpTopic);
}

/* $ 29.06.2000 tran
   adding virtual method for processing NT Console resize
*/

void Modal::SetScreenPosition()
{
}
/* tran $ */
