#ifndef __FARGLOBAL_HPP__
#define __FARGLOBAL_HPP__
/*
global.hpp

�������� ���������� ����������
�������� ���������.

*/

/* Revision: 1.24 25.07.2001 $ */

/*
Modify:
  25.07.2001 SVS
    + IsProcessAssignMacroKey - ���� ������� ���������� ������� � �������?
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
    + hConOut,hConInp
  07.12.2000 SVS
    + �������� ������ FAR_VERSION - ��� ���������.
  11.11.2000 SVS
    ! ���������: "FarTmpXXXXXX" �������� �� ���������� FarTmpXXXXXX
  20.09.2000 SVS
    ! hFarWnd ���������
  23.08.2000 SVS
    + MButtonPressed - ��� ������� ������� ����.
  03.08.2000 KM 1.03
    + ��������� ���������� int GlobalSearchWholeWords.
  03.08.2000 SVS
    ! WordDiv -> Opt.WordDiv
  03.08.2000 SVS
    + ���� ��� ������ �������� ��������
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  07.07.2000 SVS
    + �������������� ���� �� ������� (����� ��� ��������������)
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "farconst.hpp"
#include "struct.hpp"

#ifdef __FARQUEUE_HPP__
extern FarQueue<DWORD> *KeyQueue;
#endif

#if defined(__BORLANDC__)
 #ifdef __TIME_H
 extern clock_t StartIdleTime;
 extern clock_t StartExecTime;
 #endif
#else
 extern clock_t StartIdleTime;
 extern clock_t StartExecTime;
#endif

#if defined(_INC_WINDOWS) || defined(_WINDOWS_)
extern DWORD InitialConsoleMode;
extern OSVERSIONINFO WinVer;
#endif

extern struct Options Opt;

class Editor;
extern Editor *CurrentEditor;


extern CONSOLE_SCREEN_BUFFER_INFO InitScreenBufferInfo, CurScreenBufferInfo;
extern int ScrX,ScrY;
extern HANDLE hConOut,hConInp;

extern int AltPressed,CtrlPressed,ShiftPressed;
extern int LButtonPressed,RButtonPressed,PrevMouseX,PrevMouseY,MouseX,MouseY;
/* $ 23.08.2000 SVS
    + MButtonPressed - ��� ������� ������� ����.
*/
extern int MButtonPressed;
/* SVS $ */

extern int WaitInMainLoop;
extern int WaitInFastFind;

extern char FarPath[NM];
/* $ 03.08.2000 SVS
    + ���� ��� ������ �������� ��������
*/
extern char MainPluginsPath[NM];
/* SVS $ */

extern char GlobalSearchString[SEARCHSTRINGBUFSIZE];
extern int GlobalSearchCase;
/* $ 29.07.2000 KM
   ���������� ����������, �������� �������� "Whole words" ��� ������
*/
extern int GlobalSearchWholeWords;
/* KM $*/
extern int GlobalSearchReverse;

extern int ScreenSaverActive;

extern char LastFarTitle[512];

extern int CloseFAR;

extern int RegVer;
extern char RegName[256];

extern int CmpNameSearchMode;
extern int DisablePluginsOutput;
extern int CmdMode;

extern unsigned char DefaultPalette[];
extern unsigned char Palette[];
extern unsigned char BlackPalette[];
extern int SizeArrayPalette;

/* $ 20.09.2000 SVS
  ! hFarWnd ���������
*/
extern HWND hFarWnd;
/* SVS $ */

/* $ 09.11.2000 SVS
  + ��� ����, �����...
*/
extern const char FarTmpXXXXXX[];
/* SVS $ */

/* $ 07.12.2000 SVS
  + �������� ������ FAR_VERSION - ��� ���������.
*/
extern const DWORD FAR_VERSION;
/* SVS $ */

extern BOOL IsCryptFileASupport;


extern char RegColorsHighlight[];

extern BOOL LanguageLoaded;

extern BOOL NotUseCAS;
extern BOOL IsProcessAssignMacroKey;

extern char *Copyright;

#endif	// __FARGLOBAL_HPP__
