/*
iswind.cpp

�������� fullscreen/windowed

*/

/* Revision: 1.04 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  19.01.2001 VVM
    + ���� �� ����� ��� �� pid, �� ���� �� ����������� ��������� ����
      ����� ������, ���� ��� ������� �� ��� ���-� ��� ������ ����������
      ���������
  20.09.2000 SVS
    ! hFarWnd ���������
  25.07.2000 SVS
    + ���������� ������������ FulScreen <-> Windowed
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"

static BOOL CALLBACK IsWindowedEnumProc(HWND hwnd,LPARAM lParam);

HWND hFarWnd;
static BOOL WindowedMode=FALSE;
static HICON hOldLargeIcon,hOldSmallIcon;

typedef BOOL (WINAPI *PROCSETCONSOLEDISPLAYMODEELLWND)(HANDLE,DWORD,LPDWORD);
typedef BOOL (WINAPI *PROCGETCONSOLEDISPLAYMODE)(LPDWORD);
static PROCSETCONSOLEDISPLAYMODEELLWND SetConsoleDisplayMode=NULL;
static PROCGETCONSOLEDISPLAYMODE GetConsoleDisplayMode=NULL;

void DetectWindowedMode()
{
  if (hFarWnd)
    WindowedMode=!IsIconic(hFarWnd);
}

/* $ 19.01.2001 VVM
    + ���� �� ����� ��� �� pid, �� ���� �� ����������� ��������� ���� */
void FindFarWndByTitle()
{
  char OldTitle[256];
  char NewTitle[256];

  if (GetConsoleTitle(OldTitle, sizeof(OldTitle))) {
    sprintf(NewTitle,"%d - %s",clock(),OldTitle);
    SetConsoleTitle(NewTitle);
    hFarWnd = FindWindow(NULL,NewTitle);
    SetConsoleTitle(OldTitle);
  } /* if */
} /* void FindFarWndByTitle */
/* VVM $ */

void InitDetectWindowedMode()
{
  EnumWindows(IsWindowedEnumProc,(LPARAM)GetCurrentProcessId());

  /* $ 19.01.2001 VVM
      + ���� �� ����� ��� �� pid, �� ���� �� ����������� ��������� ���� */
  if (!hFarWnd)
    FindFarWndByTitle();
  /* VVM $ */

  if (hFarWnd && Opt.SmallIcon)
  {
    char FarName[NM];
    GetModuleFileName(NULL,FarName,sizeof(FarName));
    HICON hSmallIcon=NULL,hLargeIcon=NULL;
    ExtractIconEx(FarName,0,&hLargeIcon,&hSmallIcon,1);

    if (hLargeIcon!=NULL)
      hOldLargeIcon=(HICON)SendMessage(hFarWnd,WM_SETICON,1,(LPARAM)hLargeIcon);
    if (hSmallIcon!=NULL)
      hOldSmallIcon=(HICON)SendMessage(hFarWnd,WM_SETICON,0,(LPARAM)hSmallIcon);
  }

  DetectWindowedMode();
}


int IsWindowed()
{
  return(WindowedMode);
}


BOOL CALLBACK IsWindowedEnumProc(HWND hwnd,LPARAM FARpid)
{
  DWORD pid;
  GetWindowThreadProcessId(hwnd,&pid);
  if (pid==FARpid)
  {
    hFarWnd=hwnd;
    return(FALSE);
  }
  return(TRUE);
}


void RestoreIcons()
{
  if (hFarWnd && Opt.SmallIcon)
  {
    if (hOldLargeIcon!=NULL)
    {
      SendMessage(hFarWnd,WM_SETICON,1,(LPARAM)hOldLargeIcon);
      SendMessage(hFarWnd,WM_SETICON,0,(LPARAM)(hOldSmallIcon!=NULL ? hOldSmallIcon:hOldLargeIcon));
    }
  }
}

/* $ 25.07.2000 SVS
   ���������� ������������ FulScreen <-> Windowed
   (� ������ "Vasily V. Moshninov" <vmoshninov@newmail.ru>)
   mode = -2 - �������� ������� ���������
          -1 - ��� ������
           0 - Windowed
           1 - FulScreen
   Return
           0 - Windowed
           1 - FulScreen
*/
int FarAltEnter(int mode)
{
  if(mode != -2)
  {
    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    {
      DWORD dwOldMode;
      if(!SetConsoleDisplayMode)
      {
        HMODULE hKernel32 = GetModuleHandle("kernel32");
        SetConsoleDisplayMode = (PROCSETCONSOLEDISPLAYMODEELLWND)GetProcAddress(hKernel32,"SetConsoleDisplayMode");
        GetConsoleDisplayMode = (PROCGETCONSOLEDISPLAYMODE)GetProcAddress(hKernel32,"GetConsoleDisplayMode");
      }
      SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE),
           (mode == -1)?(IsWindowed()?1:0):(mode&1),&dwOldMode);
    }
    else if (hFarWnd) // win9x
    {
      //Windows9X �������� ��������� WM_COMMAND �� ����������� ���������������,
      //����� ������������ �������� ALT+ENTER:
      #define ID_SWITCH_CONSOLEMODE 0xE00F
      SendMessage(hFarWnd,WM_COMMAND,ID_SWITCH_CONSOLEMODE,
           (mode == -1)?(IsWindowed()?1:0):(mode&1));
    }
  }
  DetectWindowedMode();
  return IsWindowed()?0:1;
}
/* SVS $*/
