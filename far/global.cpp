/*
global.cpp

�������� ��६����

*/

/* Revision: 1.01 07.07.2000 $ */

/*
Modify:
  07.07.2000 SVS
    + ����࠭��⥫� ᫮� �� ॥��� (��騩 ��� ।���஢����)
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

Language Lang;

ControlObject *CtrlObject;

int ScrX,ScrY;

int AltPressed,CtrlPressed,ShiftPressed;
int LButtonPressed,RButtonPressed,PrevMouseX,PrevMouseY,MouseX,MouseY;
clock_t StartIdleTime;

DWORD InitialConsoleMode;

int WaitInMainLoop;
clock_t StartExecTime;

struct Options Opt;

OSVERSIONINFO WinVer;

char FarPath[NM];

char GlobalSearchString[512];
int GlobalSearchCase;
int GlobalSearchReverse;

int ScreenSaverActive;

char LastFarTitle[512];

ScreenBuf ScrBuf;

Editor *CurrentEditor;
int CloseFAR;

int RegVer;
char RegName[256];

int CmpNameSearchMode;
int DisablePluginsOutput;
int CmdMode;

/* $ 07.07.2000 SVS
   ����࠭��⥫� ᫮� �� ॥���
*/
char WordDiv[80];
/* SVS $ */
