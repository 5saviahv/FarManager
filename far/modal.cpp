/*
modal.cpp

Parent class ��� ��������� ��������

*/

/* Revision: 1.14 06.08.2004 $ */

/*
Modify:
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  11.12.2002 SVS
    - ����� ������� � KEY_CONSOLE_BUFFER_RESIZE (������������ ��������� ������� �������)
  25.04.2002 IS
    ! ��������� const
  30.03.2002 OT
    - ����� ����������� ���� �314 (���� 1250) ���������� ��������
      ���� �� ������.
  22.03.2002 SVS
    - strcpy - Fuck!
  20.02.2002 OT
    - BugZ#314 - Shift-Enter �� ����� ������ ���� ��������� ����
  14.06.2001 OT
    ! "����" ;-)
  16.05.2001 DJ
    ! proof-of-concept
  15.05.2001 OT
    ! NWZ -> NFZ
  06.05.2001 DJ
    ! �������� #include
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  11.07.2000 tran
    - trap if no lang files found
  29.06.2000 tran
    - (NT Console resize bug)
      adding virtual method SetScreenPosition
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "modal.hpp"
#include "keys.hpp"
#include "fn.hpp"
#include "help.hpp"
#include "lockscrn.hpp"

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
  if(ReadKey == KEY_CONSOLE_BUFFER_RESIZE)
  {
    LockScreen LckScr;
    Hide();
    Show();
  }
  if (CloseFARMenu){
    SetExitCode(TRUE);
  }
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


void Modal::SetHelp(const char *Topic)
{
  xstrncpy(HelpTopic,Topic,sizeof(HelpTopic)-1);
}


void Modal::ShowHelp()
{
  if (*HelpTopic)
    Help Hlp (HelpTopic);
}
