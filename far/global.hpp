#ifndef __FARGLOBAL_HPP__
#define __FARGLOBAL_HPP__
/*
global.hpp

�������� ���������� ����������
�������� ���������.

*/

/* Revision: 1.51 06.05.2003 $ */

/*
Modify:
  06.05.2003 SVS
    + BoxSymbols[] - ������ �������� ��������������
  21.04.2003 SVS
    + RegistrationBugs - =TRUE, ���� ���� ������� �� �������.
    + PrevFarAltEnterMode - ��� ������������ "Alt-Enetr"
  25.02.2003 SVS
    ! �������� ������� CallNewDelete/CallMallocFree ��� �������
  04.02.2003 SVS
    + � �����, ������ � �������� ������ ���� ���� "/cr", ����������� ����
      �������� �����������. ��� TD32 ������ ��������� ������� �����������.
  10.01.2003 SVS
    + ���������� ���������� CriticalInternalError �������, ���
      ����� ������ ������������� ���, �.�. ���������� ����������� ����������
      ���� �������������
    + UsedInternalClipboard - ��������� �������� ��������� :-)
  06.01.2003 SVS
    + GlobalSaveScrPtr - ���������� ����������-���������, ��� ����, �����
      ��� ��������� �������� ������ ����� "�����" ����� ����������.
  23.12.2002 SVS
    ! OnlyEditorViewerUsed ���� ������ ��������� Options
  10.12.2002 SVS
    + StartSysLog - ��� (����� �����) ������������ ������� �����!
  04.11.2002 SVS
    ! ReturnAltValue ������ �� keyboard.cpp � global.cpp
  14.07.2002 SVS
    ! ����� ��������� PluginsFolderName, HelpFileMask, HelpFormatLinkModule
      ��������� �� farconst.hpp � global.?pp
  24.05.2002 SVS
    + InGrabber, FAR_VerticalBlock
  22.05.2002 SVS
    ! Editor -> FileEditor
  16.05.2002 SVS
    + ProcessShowClock - "�� � �������?"
  07.04.2002 KM
    + IsRedrawFramesInProcess
  05.04.2002 SVS
    + Prev?ButtonPressed
  01.04.2002 SVS
    ! ��� ��������� - FarTitleAddons
  30.03.2002 OT
    - ����� ����������� ���� �314 (���� 1250) ���������� ��������
      ���� �� ������.
  19.03.2002 SVS
    + MAC_EOL_fmt
  01.03.2002 SVS
    ! FarTmpXXXXXX - ������� �� �������������.
  21.02.2002 SVS
    + ProcessException - ������� �������� ��������� ����������
  22.01.2002 SVS
    + OnliEditorViewerUsed,  =TRUE, ���� ����� ��� /e ��� /v
  14.01.2002 SVS
    + DOS_EOL_fmt[], UNIX_EOL_fmt (�� editor.cpp)
  26.11.2001 SVS
    + MouseEventFlags, PreMouseEventFlags - ���� ������� ����
  23.10.2001 SVS
    + WidthNameForMessage - 38% ��� ������� �������� ����� � �������-���������
  21.10.2001 SVS
    ! PREREDRAWFUNC � PISDEBUGGERPRESENT ��������� � farconst.hpp
    + PrevScrX,PrevScrY - ���������� ������� ������� (��� ����������������
      ��������)
  19.10.2001 SVS
    + PreRedraw* - ��� ����������� BugZ#85
  03.10.2001 SVS
    ! � ��������� ���������� ���������, ��� IsDebuggerPresent() ���� ������
      � NT, ��� ���... ��� ����� �� �����������!
  07.08.2001 SVS
    + IsProcessVE_FindFile - ���� ������� "������/��������" �� �����
      ������ ������?
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

class FileEditor;
extern FileEditor *CurrentEditor;


extern CONSOLE_SCREEN_BUFFER_INFO InitScreenBufferInfo, CurScreenBufferInfo;
extern int ScrX,ScrY;
extern int PrevScrX,PrevScrY;
extern HANDLE hConOut,hConInp;

extern int AltPressed,CtrlPressed,ShiftPressed;
extern int LButtonPressed, PrevLButtonPressed;
extern int RButtonPressed, PrevRButtonPressed;
extern int MButtonPressed, PrevMButtonPressed;
extern int PrevMouseX,PrevMouseY,MouseX,MouseY;
extern int PreMouseEventFlags,MouseEventFlags;
extern int ReturnAltValue;

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

extern int CloseFAR, CloseFARMenu;

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
extern BOOL IsProcessVE_FindFile;
extern BOOL IsRedrawFramesInProcess;

extern PREREDRAWFUNC PreRedrawFunc;
extern struct PreRedrawParamStruct PreRedrawParam;

extern char *Copyright;

extern PISDEBUGGERPRESENT pIsDebuggerPresent;

extern int WidthNameForMessage;

extern const char DOS_EOL_fmt[], UNIX_EOL_fmt[], MAC_EOL_fmt[];

extern BOOL ProcessException;

extern BOOL ProcessShowClock;

extern const char *FarTitleAddons;

extern const char FAR_VerticalBlock[];

extern int InGrabber;    // �� ������ � �������?

extern const char *PluginsFolderName;
extern const char *HelpFileMask;
extern const char *HelpFormatLinkModule;

#if defined(SYSLOG)
extern BOOL StartSysLog;
extern long CallNewDelete;
extern long CallMallocFree;
#endif

class SaveScreen;
extern SaveScreen *GlobalSaveScrPtr;

extern int CriticalInternalError;

extern int UsedInternalClipboard;

#ifdef _DEBUGEXC
extern int CheckRegistration;
#endif

extern int RegistrationBugs;

#if defined(DETECT_ALT_ENTER)
extern int PrevFarAltEnterMode;
#endif

extern WCHAR BoxSymbols[];

#endif  // __FARGLOBAL_HPP__
