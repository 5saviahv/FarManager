/*
global.cpp

���������� ����������

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
int RightAltPressed=0,RightCtrlPressed=0,RightShiftPressed=0;
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
BYTE  BoxSymbolsA[64];

int _localLastError=0;

const char *ReservedFilenameSymbols="<>|";

int KeepUserScreen;
char DirToSet[NM];

BOOL IsFn_FAR_CopyFileEx=FALSE;

int ViewerInitUseDecodeTable=TRUE,ViewerInitTableNum=0,ViewerInitAnsiText=TRUE;
int EditorInitUseDecodeTable=TRUE,EditorInitTableNum=0,EditorInitAnsiText=TRUE;

int Macro_DskShowPosType=0; // ��� ����� ������ �������� ���� ������ ������ (0 - ������� �� ��������, 1 - ����� (AltF1), 2 - ������ (AltF2))
