/*
global.cpp

���������� ����������

*/

/* Revision: 1.14 26.04.2001 $ */

/*
Modify:
  26.04.2001 VVM
    - ������� ����� MouseWheeled
  24.04.2001 SVS
    + MouseWheeled - ������� ����, ��� ��������� ������
  01.03.2001 SVS
    + RegColorsHighlight - ��� ����������
  24.01.2001 SVS
    + KeyQueue - ���������� ������� ����������
  09.01.2001 SVS
    + WaitInFastFind - ��������� �� ��������� ������� ��� ������� ShiftsKeyRules
  30.12.2000 SVS
    + IsCryptFileASupport
  22.12.2000 SVS
    + hConOut,hConInp ������ ��������� �� interf.cpp
  07.12.2000 SVS
    + ������ ������� �� ����� farversion.inc
  11.11.2000 SVS
    ! "FarTmpXXXXXX" �������� �� ���������� FarTmpXXXXXX
  23.08.2000 SVS
    + MButtonPressed - ��� ������� ������� ����.
  03.08.2000 KM 1.04
    + ��������� ���������� ���������� int GlobalSearchWholeWords.
  03.08.2000 SVS
    ! WordDiv -> Opt.WordDiv
  03.08.2000 SVS
    + ���� ��� ������ �������� ��������
  07.07.2000 SVS
    + �������������� ���� �� ������� (����� ��� ��������������)
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   ����������� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

/* $ 07.12.2000 SVS
   + ������ ������� �� ����� farversion.inc
*/
#include "farversion.inc"
/* SVS $ */

Language Lang;

ControlObject *CtrlObject=NULL;
FarQueue<DWORD> *KeyQueue=NULL;

int ScrX,ScrY;
HANDLE hConOut,hConInp;

int AltPressed,CtrlPressed,ShiftPressed;
int LButtonPressed,RButtonPressed,PrevMouseX,PrevMouseY,MouseX,MouseY;
/* $ 23.08.2000 SVS
    + MButtonPressed - ��� ������� ������� ����.
*/
int MButtonPressed;
/* SVS $ */

clock_t StartIdleTime;

DWORD InitialConsoleMode;

int WaitInMainLoop=FALSE;
int WaitInFastFind=0;


clock_t StartExecTime;

struct Options Opt;

OSVERSIONINFO WinVer;

char FarPath[NM];
/* $ 03.08.2000 SVS
    + ���� ��� ������ �������� ��������
*/
char MainPluginsPath[NM];
/* SVS $ */

char GlobalSearchString[512];
int GlobalSearchCase;
/* $ 29.07.2000 KM
   ���������� ����������, �������� �������� "Whole words" ��� ������
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

const char FarTmpXXXXXX[]="FarTmpXXXXXX";

BOOL IsCryptFileASupport=FALSE;

char RegColorsHighlight[]="Colors\\Highlight";
