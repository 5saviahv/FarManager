/*
plist.cpp

������ ��������� (Ctrl-W)

*/

/* Revision: 1.12 21.03.2003 $ */

/*
Modify:
  21.03.2003 SVS
    - Ctrl-W Del Ctrl-W � �.�....
  17.03.2003 SVS
    + ������� 21 - "������ ������ ������"
  23.12.2002 SVS
    - BugZ#744 - [wish] ������ Ctrl-R � ���� ������ �����
  17.12.2002 SVS
    + Ctrl-R ��������� �������� ������ �����.
  24.02.2002 SVS
    ! ����������� �������� � ������ MY.
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  18.07.2001 OT
    VFMenu
  03.06.2001 SVS
    ! ��������� � ����� � ���������� UserData � VMenu
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  06.05.2001 DJ
    ! �������� #include
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  31.01.2001 IS
    ! ������ ��� ���� ����� ���������� - ������� ����������� ������� ����
      ������� ������ ��� ���� ���������� ����� ��������� � �����������
      �������������� � ���������� ��� � ������� ������.
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "fn.hpp"
#include "keys.hpp"
#include "lang.hpp"
#include "vmenu.hpp"
#include "BlockExtKey.hpp"

static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);
static BOOL KillProcess(DWORD dwPID);

void ShowProcessList()
{
  VMenu ProcList(MSG(MProcessListTitle),NULL,0,ScrY-4);
  /* $ 31.01.2001 IS
     ! ������ ��� ���� ����� ���������� ;)
  */
  ProcList.SetFlags(VMENU_WRAPMODE);
  /* IS $ */
  ProcList.SetHelp("TaskList");
  ProcList.SetPosition(-1,-1,0,0);
  if (!EnumWindows(EnumWindowsProc,(LPARAM)&ProcList))
    return;
  ProcList.AssignHighlights(FALSE);
  ProcList.Show();

  while (!ProcList.Done())
  {
    int Key=ProcList.ReadInput();
    switch(Key)
    {
      case KEY_CTRLR:
      {
        ProcList.Hide();
        ProcList.DeleteItems();
        ProcList.SetPosition(-1,-1,0,0);
        if (!EnumWindows(EnumWindowsProc,(LPARAM)&ProcList))
        {
          ProcList.Modal::SetExitCode(-1);
          break;
        }
        ProcList.Show();
        break;
      }

      case KEY_DEL:
        {
          BlockExtKey blockExtKey;

          // ������� 21
          if(Opt.Policies.DisabledOptions&FFPOL_KILLTASK)
            break;

          HWND ProcWnd=(HWND)ProcList.GetUserData(NULL,0);
          if (ProcWnd!=NULL)
          {
            char WinTitle[512];
            GetWindowText(ProcWnd,WinTitle,sizeof(WinTitle));
            CharToOem(WinTitle,WinTitle);
            DWORD ProcID;
            GetWindowThreadProcessId(ProcWnd,&ProcID);
            if (Message(MSG_WARNING,2,MSG(MKillProcessTitle),MSG(MAskKillProcess),
                        WinTitle,MSG(MKillProcessWarning),MSG(MKillProcessKill),MSG(MCancel))==0)
              if (KillProcess(ProcID))
              {
                Sleep(500);
                ProcList.Hide();
                ShowProcessList();
                return;
              }
              else
                Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MKillProcessTitle),MSG(MCannotKillProcess),MSG(MOk));
          }
        }
        break;
      default:
        ProcList.ProcessInput();
        break;
    }
  }
  if (ProcList.Modal::GetExitCode()>=0)
  {
    HWND ProcWnd=(HWND)ProcList.GetUserData(NULL,0);
    if (ProcWnd!=NULL)
    {
      //SetForegroundWindow(ProcWnd);
      #ifndef SPI_GETFOREGROUNDLOCKTIMEOUT
      #define SPI_GETFOREGROUNDLOCKTIMEOUT        0x2000
      #define SPI_SETFOREGROUNDLOCKTIMEOUT        0x2001
      #endif

      // Allow SetForegroundWindow on Win98+.
      DWORD dwMs;
      // Remember the current value.
      BOOL bSPI = SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &dwMs, 0);
      if(bSPI) // Reset foreground lock timeout
        bSPI = SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, 0, 0);
      SetForegroundWindow(ProcWnd);
      if(bSPI) // Restore old value
        SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID)dwMs, 0);

      WINDOWPLACEMENT wp;
      wp.length=sizeof(wp);
      if (!GetWindowPlacement(ProcWnd,&wp) || wp.showCmd!=SW_SHOWMAXIMIZED)
        ShowWindowAsync(ProcWnd,SW_RESTORE);
    }
  }
}


BOOL KillProcess(DWORD dwPID)
{
  // ������� 21
  if(Opt.Policies.DisabledOptions&FFPOL_KILLTASK)
   return FALSE;

  HANDLE hProcess;
  BOOL bRet;
  hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,dwPID);
  if (hProcess!=NULL)
  {
    bRet=TerminateProcess(hProcess,0xFFFFFFFF);
    if (bRet)
      WaitForSingleObject(hProcess,5000);
    CloseHandle(hProcess);
  }
  else
    bRet=FALSE;
  return(bRet);
}


BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
  VMenu *ProcList=(VMenu *)lParam;
  if (IsWindowVisible(hwnd) ||
      IsIconic(hwnd) && (GetWindowLong(hwnd,GWL_STYLE) & WS_DISABLED)==0)
  {
    char Title[256];
    GetWindowText(hwnd,Title,sizeof(Title));
    if (*Title)
    {
      struct MenuItem ListItem;
      memset(&ListItem,0,sizeof(ListItem));
      TruncStr(Title,sizeof(ListItem.Name)-1);
      sprintf(ListItem.Name,"%-25s",Title);
      CharToOem(ListItem.Name,ListItem.Name);
      ProcList->SetUserData((void*)hwnd,sizeof(hwnd),ProcList->AddItem(&ListItem));
    }
  }
  return(TRUE);
}
