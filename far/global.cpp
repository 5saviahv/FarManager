/*
global.cpp

�������� ��६����

*/

/* Revision: 1.04 03.08.2000 $ */

/*
Modify:
  03.08.2000 KM 1.04
    + ��������� ������쭠� ��६����� int GlobalSearchWholeWords.
  03.08.2000 SVS
    ! WordDiv -> Opt.WordDiv
  03.08.2000 SVS
    + ���� ��� ���᪠ �᭮���� ��������
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
/* $ 03.08.2000 SVS
    + ���� ��� ���᪠ �᭮���� ��������
*/
char MainPluginsPath[NM];
/* SVS $ */

char GlobalSearchString[512];
int GlobalSearchCase;
/* $ 29.07.2000 KM
   ������쭠� ��६�����, �࠭��� ���祭�� "Whole words" ��� ���᪠
*/
int GlobalSearchWholeWords;
/* KM $*/
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

