/*
iswind.cpp

�஢�ઠ fullscreen/windowed

*/

/* Revision: 1.02 20.09.2000 $ */

/*
Modify:
  20.09.2000 SVS
    ! hFarWnd ������쭠
  25.07.2000 SVS
    + �ணࠬ��� ��४��祭�� FulScreen <-> Windowed
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

void InitDetectWindowedMode()
{
  EnumWindows(IsWindowedEnumProc,(LPARAM)GetCurrentProcessId());

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
   �ணࠬ��� ��४��祭�� FulScreen <-> Windowed
   (� ����� "Vasily V. Moshninov" <vmoshninov@newmail.ru>)
   mode = -2 - ������� ⥪�饥 ���ﭨ�
          -1 - ��� �ਣ��
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
      //Windows9X ���뫠�� ᮮ�饭�� WM_COMMAND � ᯥ樠��� �����䨪��஬,
      //����� ���짮��⥫� �������� ALT+ENTER:
      #define ID_SWITCH_CONSOLEMODE 0xE00F
      SendMessage(hFarWnd,WM_COMMAND,ID_SWITCH_CONSOLEMODE,
           (mode == -1)?(IsWindowed()?1:0):(mode&1));
    }
  }
  DetectWindowedMode();
  return IsWindowed()?0:1;
}
/* SVS $*/
