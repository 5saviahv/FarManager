/*
global.cpp

���������� ����������

*/

/* Revision: 1.58 22.09.2003 $ */

/*
Modify:
  22.09.2003 KM
    + GlobalSearchHex - ���������� ����������, �������� ��������
      "Search for hex" ��� ������
  15.09.2003 SVS
    + ReservedFilenameSymbols - ������������ ������� � ����� �����/��������
  26.08.2003 SVS
    + TitleModified
  06.06.2003 SVS
    ! MainPluginsPath �������� � Opt.LoadPlug.
  01.06.2003 SVS
    + _localLastError
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
  05.12.2002 SVS
    - ������� ������������������� PrevScrX � PrevScrY
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
    ! _���_ �����������������. ��������� "�������������"!
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
  15.01.2002 SVS
    ! ������� ������ ������� ��� ���� ��������� ������ FarTmpXXXXXX
  14.01.2002 SVS
    + DOS_EOL_fmt[], UNIX_EOL_fmt (�� editor.cpp)
  26.11.2001 SVS
    + MouseEventFlags, PreMouseEventFlags - ���� ������� ����
  23.10.2001 SVS
    + WidthNameForMessage - 38% ��� ������� �������� ����� � �������-���������
  21.10.2001 SVS
    + PrevScrX,PrevScrY - ���������� ������� ������� (��� ����������������
      ��������)
  19.10.2001 SVS
    + PreRedraw* - ��� ����������� BugZ#85
  03.10.2001 SVS
    ! � ��������� ���������� ���������, ��� IsDebuggerPresent() ���� ������
      � NT, ��� ���... ��� ����� �� �����������!
  18.09.2001 SVS
    ! "FarTmpXXXXXX" -> "FARTMPXXXXXX".
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

/* $ 29.06.2000 tran
  ����� char *CopyRight �� inc ����� */
#include "copyright.inc"
/* tran $ */

/* $ 07.12.2000 SVS
   + ������ ������� �� ����� farversion.inc
*/
#include "farversion.inc"
/* SVS $ */

OSVERSIONINFO WinVer={0};

struct Options Opt={0};

// ������� ���������� (Win2K) ���������? (��� SetAttr!)
BOOL IsCryptFileASupport=FALSE;

// �������� ���� ��������?
BOOL LanguageLoaded=FALSE;

// ���� �� ������ ������ Ctrl-Alt-Shift
BOOL NotUseCAS=FALSE;

// ���� ������� ���������� ������� � �������?
BOOL IsProcessAssignMacroKey=FALSE;

// ���� ������� "������/��������" �� ����� ������ ������?
BOOL IsProcessVE_FindFile=FALSE;

// ��� ������� ����������� ���� �������
BOOL IsRedrawFramesInProcess=FALSE;

// ���� ������� �������� ������ � �������?
int WaitInFastFind=FALSE;

// �� �������� � �������� �����?
int WaitInMainLoop=FALSE;

// "��������������" ������� ����� ������
FarQueue<DWORD> *KeyQueue=NULL;
int AltPressed=0,CtrlPressed=0,ShiftPressed=0;
int LButtonPressed=0,RButtonPressed=0,MButtonPressed=0;
int PrevLButtonPressed=0, PrevRButtonPressed=0, PrevMButtonPressed=0;
int PrevMouseX=0,PrevMouseY=0,MouseX=0,MouseY=0;
int PreMouseEventFlags=0,MouseEventFlags=0;

// ������ ��� ��� ���� Alt-������?
int ReturnAltValue=0;


CONSOLE_SCREEN_BUFFER_INFO InitScreenBufferInfo={0};
CONSOLE_SCREEN_BUFFER_INFO CurScreenBufferInfo={0};
int ScrX=0,ScrY=0;
int PrevScrX=-1,PrevScrY=-1;
HANDLE hConOut=NULL,hConInp=NULL;

clock_t StartIdleTime=0;

DWORD InitialConsoleMode=0;

clock_t StartExecTime=0;

char FarPath[NM];

char LastFarTitle[512];
int  TitleModified=FALSE;
char RegColorsHighlight[]="Colors\\Highlight";


char GlobalSearchString[SEARCHSTRINGBUFSIZE];
int GlobalSearchCase=FALSE;
/* $ 29.07.2000 KM
   ���������� ����������, �������� �������� "Whole words" ��� ������
*/
int GlobalSearchWholeWords=FALSE;
/* KM $*/
/* $ 22.09.2003 KM
   ���������� ����������, �������� �������� "Search for hex" ��� ������
*/
int GlobalSearchHex=FALSE;
/* KM $ */
int GlobalSearchReverse=FALSE;

int ScreenSaverActive=FALSE;

FileEditor *CurrentEditor=NULL;
int CloseFAR=FALSE,CloseFARMenu=FALSE;

// ��� �����������
int  RegVer;
char RegName[256];

int CmpNameSearchMode=FALSE;
int DisablePluginsOutput=FALSE;
int CmdMode=FALSE;

PISDEBUGGERPRESENT pIsDebuggerPresent=NULL;

PREREDRAWFUNC PreRedrawFunc=NULL;
struct PreRedrawParamStruct PreRedrawParam={0};

int WidthNameForMessage=0;

const char DOS_EOL_fmt[]  ="\r\n",
           UNIX_EOL_fmt[] ="\n",
           MAC_EOL_fmt[]  ="\r";

BOOL ProcessException=FALSE;
BOOL ProcessShowClock=FALSE;

const char *FarTitleAddons=" - Far";

const char FAR_VerticalBlock[]= "FAR_VerticalBlock";

int InGrabber=FALSE;

const char *PluginsFolderName="Plugins";
const char *HelpFileMask="*.hlf";
const char *HelpFormatLinkModule="<%s>%s";

#if defined(SYSLOG)
BOOL StartSysLog=0;
long CallNewDelete=0;
long CallMallocFree=0;
#endif

class SaveScreen;
SaveScreen *GlobalSaveScrPtr=NULL;

int CriticalInternalError=FALSE;

int UsedInternalClipboard=0;

#ifdef _DEBUGEXC
int CheckRegistration=TRUE;
#endif

int RegistrationBugs=FALSE;

#if defined(DETECT_ALT_ENTER)
int PrevFarAltEnterMode=-1;
#endif

WCHAR BoxSymbols[64];

int _localLastError=0;

const char *ReservedFilenameSymbols="<>|";
