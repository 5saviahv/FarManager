#ifndef __FARGLOBAL_HPP__
#define __FARGLOBAL_HPP__
/*
global.hpp

���ᠭ�� ��������� ��६�����
������� ��᫥����.

*/

/* Revision: 1.06 23.08.2000 $ */

/*
Modify:
  23.08.2000 SVS
    + MButtonPressed - ��� �।��� ������ ���.
  03.08.2000 KM 1.03
    + ��������� ��६����� int GlobalSearchWholeWords.
  03.08.2000 SVS
    ! WordDiv -> Opt.WordDiv
  03.08.2000 SVS
    + ���� ��� ���᪠ �᭮���� ��������
  11.07.2000 SVS
    ! ��������� ��� ���������� �������樨 ��� BC & VC
  07.07.2000 SVS
    + ����࠭��⥫� ᫮� �� ॥��� (��騩 ��� ।���஢����)
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#ifdef __LANGUAGE_HPP__
extern Language Lang;
#endif

#ifdef __CONTROLOBJECT_HPP__
extern ControlObject *CtrlObject;
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

#ifdef __FARSTRUCT_HPP__
extern struct Options Opt;
#endif

#ifdef __SCREENBUF_HPP__
extern ScreenBuf ScrBuf;
#endif

#ifdef __EDITOR_HPP__
extern Editor *CurrentEditor;
#endif

extern int ScrX,ScrY;

extern int AltPressed,CtrlPressed,ShiftPressed;
extern int LButtonPressed,RButtonPressed,PrevMouseX,PrevMouseY,MouseX,MouseY;
/* $ 23.08.2000 SVS
    + MButtonPressed - ��� �।��� ������ ���.
*/
extern int MButtonPressed;
/* SVS $ */

extern int WaitInMainLoop;

extern char FarPath[NM];
/* $ 03.08.2000 SVS
    + ���� ��� ���᪠ �᭮���� ��������
*/
extern char MainPluginsPath[NM];
/* SVS $ */

extern char GlobalSearchString[512];
extern int GlobalSearchCase;
/* $ 29.07.2000 KM
   ������쭠� ��६�����, �࠭��� ���祭�� "Whole words" ��� ���᪠
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

#endif	// __FARGLOBAL_HPP__
