/*
global.cpp

���������� ����������

*/

/* Revision: 1.21 25.07.2001 $ */

/*
Modify:
  25.07.2001 SVS
    ! Copyright ��������� �� ctrlobj.cpp.
  24.07.2001 SVS
    + NotUseCAS: ���� �� ������ ������ Ctrl-Alt-Shift
  27.06.2001 SVS
    + LanguageLoaded
  25.06.2001 SVS
    ! ����� SEARCHSTRINGBUFSIZE
  21.05.2001 OT
    + ���������� CONSOLE_SCREEN_BUFFER_INFO InitScreenBufferInfo, CurScreenBufferInfo
      ����� ��� ����� ������������� ��������� AltF9
  12.05.2001 DJ
    ! ��� ��������� #include: ����� #include "ctrlobj.hpp", � ��������� ��
      CtrlObject �������� � ctrlobj.hpp; ��� ������� ���������
  06.05.2001 DJ
    ! �������� #include
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

#include "global.hpp"
#include "farqueue.hpp"

/* $ 07.12.2000 SVS
   + ������ ������� �� ����� farversion.inc
*/
#include "farversion.inc"
/* SVS $ */

/* $ 29.06.2000 tran
  ����� char *CopyRight �� inc ����� */
#include "copyright.inc"
/* tran $ */

FarQueue<DWORD> *KeyQueue=NULL;

CONSOLE_SCREEN_BUFFER_INFO InitScreenBufferInfo, CurScreenBufferInfo;
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

char GlobalSearchString[SEARCHSTRINGBUFSIZE];
int GlobalSearchCase;
/* $ 29.07.2000 KM
   ���������� ����������, �������� �������� "Whole words" ��� ������
*/
int GlobalSearchWholeWords;
/* KM $*/
int GlobalSearchReverse;

int ScreenSaverActive;

char LastFarTitle[512];

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

BOOL LanguageLoaded=FALSE;

// ���� �� ������ ������ Ctrl-Alt-Shift
BOOL NotUseCAS=FALSE;
