/*
constitle.cpp

��������� �������

*/

/* Revision: 1.05 01.03.2004 $ */

/*
Modify:
  01.03.2004 SVS
    ! ������� FAR_OemTo* � FAR_CharTo* ������ ����������� WinAPI-�������
      (����� �� ������� + ������� ������������ ������ 1 ����)
  10.05.2002 SVS
    - ����� ���������� � ���������. ������� ���������������
  01.04.2002 SVS
    ! ��� ��������� - ������� SetFarTitle ��� ����������� �������������� �����
      �������
  14.05.2001 SVS
    + ������� �����������
  06.05.2001 DJ
    + �������� #include
  20.03.2001 tran
    ! created
*/

#include "headers.hpp"
#pragma hdrstop

#include "constitle.hpp"
#include "global.hpp"
#include "fn.hpp"

ConsoleTitle::ConsoleTitle(char *title)
{
  GetConsoleTitle(OldTitle,512);
  if (WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT)
    FAR_CharToOem(OldTitle,OldTitle);
//  _SVS(SysLog(1,"ConsoleTitle> '%s'",OldTitle));
  if(title)
    SetFarTitle(title);
}

ConsoleTitle::~ConsoleTitle()
{
  char *Ptr=OldTitle+strlen(OldTitle)-strlen(FarTitleAddons);
  if(!stricmp(Ptr,FarTitleAddons))
    *Ptr=0;
  SetFarTitle(OldTitle);
//  _SVS(SysLog(-1,"~ConsoleTitle '%s'",OldTitle));
}

void ConsoleTitle::Set(char *fmt,...)
{
  char msg[2048];

  va_list argptr;
  va_start( argptr, fmt );

  vsprintf( msg, fmt, argptr );
  va_end(argptr);
  SetFarTitle(msg);
}
