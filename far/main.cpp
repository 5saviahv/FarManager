/*
main.cpp

������� main.

*/

/* Revision: 1.70 06.06.2003 $ */

/*
Modify:
  06.06.2003 SVS
    ! ��������� ��� �� ������ - ���� /p ������������
  19.05.2003 SVS
    ! DetectTTFFont ����� �� main.cpp � interf.cpp
  06.05.2003 SVS
    ! /aw ������, ������ ���� /8, ������� � ������ �� ���������, � � NT
      ���������� �������� ��� � 8-������ ������� (�� ��������� � NT
      ��� �������� as Unicode - ��� ��� Out, ��� � ��� Inp
    ! ���������� TTF �������� �����
  16.04.2003 SVS
    ! /co ����������� ��� /p
    ! ��������� ��� _beginthread() - ������� ������� ��������, �����
      ������������������ ������������ � ������� :-(
  31.03.2003 SVS
    ! �������� ��� �������� _beginthread() � �������� "�������� �����"
  17.03.2003 SVS
    ! �������� ����� ����� FFPOL_*
  04.02.2003 SVS
    + � �����, ������ � �������� ������ ���� ���� "/cr", ����������� ����
      �������� �����������. ��� TD32 ������ ��������� ������� �����������.
  18.01.2003 IS
    - �� ��������� �������������� ������� /p[<path>], ���� � ���� ����
      ����� ������������� ��������.
  10.01.2003 SVS
    - BugZ#765 - ����� ��������� ������ �������� ������������.
    ! ������ ���������� �������� (� ����������� ������������ ������) ����������
      � ����� ����. ��� ����� ����� ���� �������� �� main � MainProcess.
      ����� MainProcess ��������� ����������� �������� ��� ����������
      (���� ����� Opt.ExceptRules)
  23.12.2002 SVS
    ! OnlyEditorViewerUsed ���� ������ ��������� Options
  07.10.2002 SVS
    - BugZ#674 - far.exe c:\dir [c:\dir2] - �� ��������
  21.08.2002 SVS
    ! ��������� ��� WaitKey
  02.07.2002 SVS
    - /u USER ��������
  27.06.2002 SVS
    - ������� � ������ ��� ������ ���� - ������ �������� TTF-������
      � ���� ��������� ��� ��������
  21.06.2002 SVS
    + ���������� TTF-������
    ! /w  - ��������� (����� ����� ����������)
    + /aw - ��������� ��������������� TrueType ������ ��� �������.
    + � ��������� ������ ����� ������� �� ����� ���� ����� � ���������,
      ������ ��� �������. ������ ���� ��� �������� ������, ������ -
      ��� ���������.
  17.06.2002 SVS
    ! ����� /ttf �������� �� /w
    ! ��� "far /?" � ������ �� ���������
  04.06.2002 SVS
    - BugZ#547 - No dot in help description
  30.05.2002 SVS
    ! ����� ����� /ttf - ��� ������������� TTF-����� ��� ����������� ����
  21.05.2002 IS
    + ������� �������� �������� ������� �������� ���� � ������
      ������������� ������ ��� MainPluginsPath
  18.04.2002 SKV
    + ��������� floating point ��� �� VC++ ��� ���������.
    + ifdef ��� �� ��������������� ��� VC 7.0
  08.04.2002 SVS
    ! ��������� ������ MainPluginsPath
  26.03.2002 IS
    + ����� InitLCIDSort, ������ ����� ������������� ����������, � �� �
      LocalUpperInit
  28.01.2002 VVM
    + ���� �� ������ ������� .��� ���� - �� ����� ������� ��������� �������
      ����� ����� - �� ������ ������...
  22.01.2002 SVS
    + ����� /xd  "Enable exception handling" - ��, ����� � ��������� �
      ������������ ��������. � "����������" ���� ����� ����. ��� ���������
      ����� ���������� ��� � �������� _DEBUGEXC
  22.01.2002 SVS
    - BugZ#201 - Shift of command prompt after exiting FAR
    + OnliEditorViewerUsed,  =TRUE, ���� ����� ��� /e ��� /v
  22.01.2002 SVS
    - BugZ#263 - Opening non-existent file in viewer (beta 4 only)
  11.01.2002 IS
    - "������", ���� :-(
      �� � LocalUpperInit ��� ����� ���������, � � ��������� �������, �
      LocalUpperInit ������ ������ ������ ��, ��� �������� � �� ������ ������.
  10.01.2002 SVS
    - "�������", ����. :-(
      ���������� Opt.HotkeyRules ������ ���� ������ ������ LocalUpperInit
      ���... � ����� ��� � ���� ����� LocalUpperInit � ����������?
  09.01.2002 IS
    ! ������� LocalUpperInit � ����� ������ main, � ��������� ������ �����
      �����, ���� ���������� LocalStricmp.
  24.12.2001 VVM
    ! ��� �������� ��������� ����������� ������ ����� ������...
  08.11.2001 SVS
    ! ����� �� ����� ���������� ����� :-(((
  06.11.2001 SVS
    + � ������ ������ ���� ������� � [HKCU\Software\Far\System\Environment]
      ���� ���������, � � ���� ����� ������� Values, �� FAR ��� ������
      ��������� ���� Value-Data � ������������� ���������� ���������.
  18.10.2001 SVS
    ! "��������" ����
  05.10.2001 SVS
    ! Opt.ExceptRules � �������
  03.10.2001 SVS
    ! ���� ��� ���������, �� ��������� ���������� ����������, ����� - ������
      ��� ������ �����.
  26.09.2001 SVS
    + ������� 19 - ���� � "�������" ��������� 19-� ��� - ������������ /p
  16.09.2001 SVS
    - ����������� ���������� (+ ���� ����� ���.������ ��� ���� �����)
  05.09.2001 SVS
    ! SetHighlighting() ��������� � hilight.cpp
  08.08.2001 SVS
    + �������� ��� "far /e" �������� ������������ F6
  06.08.2001 SVS
    + ���� "/do" �������������� ������ ��� "����" :-)
      �� �������� � far /? ������ �����, ����� ��� ��������� � ��������
      DIRECT_RT
      (��� ����� ������� ��� ��� �� ����� :-)
    - ��� ������� ����� ������ ����� Flush() :-(
  27.07.2001 SVS
    + ���� "/co" ��������������.
  24.07.2001 SVS
    ! ������� ���� NotUseCAS - ����� �� �������� ������ ��� ����������
      ��������/������� (far /e) - ����� ����!
    + ������ ����� ����� ������ �� ����
  11.07.2001 SVS
    + ���������� �����: FARLANG � FARUSER
  10.07.2001 SVS
    ! �������� (��������) ���������� ��� �����������.
  07.07.2001 IS
    ! � 806 ����� � ������� ���� �������, ��������� � ���������� ��������� �
      ������ "..", � ������ ������ �� ���. ��������� �������� :-)
  06.07.2001 IS
    ! ��������� � SetHighlighting � ������������ � ����������� � ���������
      HighlightData
  02.07.2001 IS
    - ���: ������������� ������ � PluginsCache � ��� ������, ���� far.exe ���
      ������� � ��������� ��������� ����.
  29.06.2001 OT
    ! ��������� ��������� ... � ������-� :( �������� :)
  23.06.2001 OT
    ! ��������� ��� ������ far -v/-e. �� ����� ��� ������ ������-��������.
  29.05.2001 tran
    + DIRECT_RT
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  23.04.2001 SVS
    ! ���! ����� ��<��� �� %PATHEXT% - �� ��� ����������� � ��, ���
      ����� - ������ ��������.
  08.04.2001 SVS
    ! �������� ��������� PATHEXT - ��� ������ FAR`� ����������� � ������� ����.
  04.04.2001 SVS
    ! ������� ������������ ���� � SetHighlighting()
  03.04.2001 SVS
    ! CmdExt - ��������� ����������� � ������ %PATHEXT% � WinNT (*.cmd)
    ! ��������� ������������ ����������.
  02.04.2001 SVS
    ! ��������� "���������" ���������� ���������� � ��������� ��� �������.
    + *.vbs,*.js - ��������� ��� � ����������� ������.
  07.03.2001 IS
    - ���������� �� ������� ��� �������� ����� � ������� (�� ������ ����������
      �� ����).������� ��������� �����, ���� ��� �����, ������ ��� � ����
      ������ ��� ����� ������ ��� �������� ��������� � �������.
  05.03.2001 SVS
    + ������������������ /co ��� /?
  02.03.2001 SVS
    - � ���� ��������� ������ :-((((
  01.03.2001 SVS
    ! ������������� ������� SetHighlighting() - � ����������� ����� �����
      �������� ��� ��������� - ������ ���� ������.
    ! ��� ������ ��� ��������� ������ � ������� �� "Highlight" �
      "Colors\Highlight". ����� "Highlight" �����������.
  19.01.2001 SVS
    + FAR.EXE /?
    + �������� �� ���������� ����� (MListEval)
  30.12.2000 SVS
    + ����������������� ������� ������ � ���������� Encryped ����� �����
      ������ FAR
  28.12.2000 SVS
    + Opt.HotkeyRules - ������� �� ���� ������ ��������� �������
  21.12.2000 SVS
    ���� �� ����� LNG, �� ����� ������ ��������� ������� ����� �������.
  15.12.2000 SVS
    + � ������, ���� �� ����� ������ LNG-������ - ������ ������� ���������
      � �� ����������������.
  01.09.2000 tran
    + /co switch
  03.08.2000 SVS
    ! WordDiv -> Opt.WordDiv
  03.08.2000 SVS
    ! �� ���������� ������ ������ ������ ��� ���-������
    + ����� �������� "������ ����������� %FAR%\Plugins ������ ������� ��
      ���������� ����". ��� ���� ������������ ���� �������� �����.
      IMHO ������ ����� ��������� ������ � ��������� ����������!!!
  07.07.2000 IS
    - ���������� SetHighlighting ������� � fn.cpp
  07.07.2000 SVS
    + �������� �������������� ���� �� ������� (����� ��� ��������������)
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "chgprior.hpp"
#include "colors.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "fileedit.hpp"
#include "fileview.hpp"
#include "lockscrn.hpp"
#include "hilight.hpp"
#include "manager.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"
#include "language.hpp"
#include "farexcpt.hpp"

#ifdef DIRECT_RT
int DirectRT=0;
#endif

static void ConvertOldSettings();
/* $ 07.07.2000 IS
  ����� ��� ���������� � fn.cpp, ����� ���� �������� ��������...
*/
//static void SetHighlighting();
/* IS $ */
static void CopyGlobalSettings();

static void show_help(void)
{
printf(
"The following switches may be used in the command line:\n\n"
" /?   This help.\n"
" /a   Disable display of characters with codes 0 - 31 and 255.\n"
" /ag  Disable display of pseudographics characters.\n");
#if defined(USE_WFUNC)
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
    printf(
" /8   Forces FAR to work in ANSI (non-Unicode) console.\n"
    );
  }
#endif
printf(
" /e[<line>[:<pos>]] <filename>\n"
"      Edit the specified file.\n"
" /i   Set small (16x16) icon for FAR console window.\n"
" /p[<path>]\n"
"      Search for \"common\" plugins in the directory, specified by <path>.\n"
" /co  Forces FAR to load plugins from the cache only.\n"
" /u <username>\n"
"      Allows to have separate settings for different users.\n"
" /v <filename>\n"
"      View the specified file. If <filename> is -, data is read from the stdin.\n"
" /x   Disable exception handling.\n"
#if defined(_DEBUGEXC)
" /xd  Enable exception handling.\n"
" /cr  Disable check registration.\n"
#endif
#ifdef DIRECT_RT
" /do  Direct output.\n"
#endif
);
#if 0
#if defined(USE_WFUNC)
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
    printf(
" /w   Specify this if you are using a TrueType font for the console.\n"
    );
  }
#endif
#endif
}

static int MainProcess(char *EditName,char *ViewName,char *DestName1,char *DestName2,int StartLine,int StartChar,int RegOpt)
{
  {
    ChangePriority ChPriority(WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS ? THREAD_PRIORITY_ABOVE_NORMAL:THREAD_PRIORITY_NORMAL);
    ControlObject CtrlObj;
    if (*EditName || *ViewName)
    {
      NotUseCAS=TRUE;
      Opt.OnlyEditorViewerUsed=1;
      Panel *DummyPanel=new Panel;
      CmdMode=TRUE;
      _tran(SysLog("create dummy panels"));
      CtrlObj.CreateFilePanels();
      CtrlObj.Cp()->LeftPanel=CtrlObj.Cp()->RightPanel=CtrlObj.Cp()->ActivePanel=DummyPanel;
      CtrlObj.Plugins.LoadPlugins();
      if (*EditName)
      {
        FileEditor *ShellEditor=new FileEditor(EditName,TRUE,TRUE,StartLine,StartChar);
        _tran(SysLog("make shelleditor %p",ShellEditor));
        if (!ShellEditor->GetExitCode()){ // ????????????
          FrameManager->ExitMainLoop(0);
        }
      }
      if (*ViewName)
      {
        FileViewer *ShellViewer=new FileViewer(ViewName,FALSE);
        if (!ShellViewer->GetExitCode()){
          FrameManager->ExitMainLoop(0);
        }
        _tran(SysLog("make shellviewer, %p",ShellViewer));
      }
      FrameManager->EnterMainLoop();
      CtrlObj.Cp()->LeftPanel=CtrlObj.Cp()->RightPanel=CtrlObj.Cp()->ActivePanel=NULL;
      delete DummyPanel;
      _tran(SysLog("editor/viewer closed, delete dummy panels"));
    }
    else
    {
      char Path[NM];
      Opt.OnlyEditorViewerUsed=0;
      NotUseCAS=FALSE;
#ifdef _DEBUGEXC
      if(CheckRegistration)
      {
#endif
        if (RegOpt)
          Register();
        static struct RegInfo Reg;
        if(_beginthread(CheckReg,0x10000,&Reg) == -1)
        {
          RegistrationBugs=TRUE;
          CheckReg(&Reg);
        }
        else
          RegistrationBugs=FALSE;
        while (!Reg.Done)
          Sleep(10);
#ifdef _DEBUGEXC
      }
#endif

      Opt.SetupArgv=0;

      // ������������� ���, ��� ControlObject::Init() ������� ������
      // ���� Opt.*
      if(*DestName1) // ������� ������
      {
        Opt.SetupArgv++;
        strcpy(Path,DestName1);
        *PointToName(Path)=0;
        DeleteEndSlash(Path); // ���� �������� ����� �� ������ - �������� �������� ������ - ����������� ".."
        if(Path[1]==':' && !Path[2])
          AddEndSlash(Path);

        // �� ������, ������� ����� ����� - ������� (������ �� �������� � ����� ������ ;-)
        if(Opt.LeftPanel.Focus)
        {
          Opt.LeftPanel.Type=FILE_PANEL;  // ������ ���� ������
          Opt.LeftPanel.Visible=TRUE;     // � ������� ��
          strcpy(Opt.LeftFolder,Path);
        }
        else
        {
          Opt.RightPanel.Type=FILE_PANEL;
          Opt.RightPanel.Visible=TRUE;
          strcpy(Opt.RightFolder,Path);
        }

        if(*DestName2) // ��������� ������
        {
          Opt.SetupArgv++;
          strcpy(Path,DestName2);
          *PointToName(Path)=0;
          DeleteEndSlash(Path);
          if(Path[1]==':' && !Path[2])
            AddEndSlash(Path);

          // � ����� � �������� �������� - ������������ ��������� ������
          if(Opt.LeftPanel.Focus)
          {
            Opt.RightPanel.Type=FILE_PANEL; // ������ ���� ������
            Opt.RightPanel.Visible=TRUE;    // � ������� ��
            strcpy(Opt.RightFolder,Path);
          }
          else
          {
            Opt.LeftPanel.Type=FILE_PANEL;
            Opt.LeftPanel.Visible=TRUE;
            strcpy(Opt.LeftFolder,Path);
          }
        }
      }

      // ������ ��� ������ - ������� ������!
      CtrlObj.Init();

      // � ������ "����������" � ������� ��� ����-���� (���� ��������� ;-)
      if(*DestName1) // ������� ������
      {
        LockScreen LockScr;
        Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
        Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(ActivePanel);

        strcpy(Path,PointToName(DestName1));
        if (*Path)
        {
          if (ActivePanel->GoToFile(Path))
            ActivePanel->ProcessKey(KEY_CTRLPGDN);
        }

        if(*DestName2) // ��������� ������
        {
          strcpy(Path,PointToName(DestName2));
          if (*Path)
          {
            if (AnotherPanel->GoToFile(Path))
              AnotherPanel->ProcessKey(KEY_CTRLPGDN);
          }
        }

        // !!! �������� !!!
        // ������� �������� ��������� ������, � ����� ��������!
        AnotherPanel->Redraw();
        ActivePanel->Redraw();
      }

      FrameManager->EnterMainLoop();
    }

    // ������� �� �����!
    SetScreen(0,0,ScrX,ScrY,' ',F_LIGHTGRAY|B_BLACK);
    ScrBuf.ResetShadow();
    ScrBuf.Flush();
    MoveRealCursor(0,0);
  }

  CloseConsole();
  RestoreIcons();
  return(0);
}


int _cdecl main(int Argc, char *Argv[])
{
  _OT(SysLog("[[[[[[[[New Session of FAR]]]]]]]]]"));
  char EditName[NM],ViewName[NM];
  char DestName[2][NM];
  int StartLine=-1,StartChar=-1,RegOpt=FALSE;
  int CntDestName=0; // ���������� ����������-���� ���������

  *EditName=*ViewName=DestName[0][0]=DestName[1][0]=0;
  CmdMode=FALSE;

  WinVer.dwOSVersionInfoSize=sizeof(WinVer);
  GetVersionEx(&WinVer);

  /*$ 18.04.2002 SKV
    ���������� floating point ��� �� �������������������� vc-��� fprtl.
  */
#ifdef _MSC_VER
  float x=1.1f;
  char buf[15];
  sprintf(buf,"%f",x);
#endif

  // ���� ��� ���������, �� ��������� ���������� ����������,
  //  ����� - ������ ��� ������ �����.
#if defined(_DEBUGEXC)
  Opt.ExceptRules=-1;
#else
  if(!pIsDebuggerPresent)
    pIsDebuggerPresent=(PISDEBUGGERPRESENT)GetProcAddress(GetModuleHandle("KERNEL32"),"IsDebuggerPresent");
  Opt.ExceptRules=(pIsDebuggerPresent && pIsDebuggerPresent()?0:-1);
#endif

#if defined(USE_WFUNC)
  Opt.UseUnicodeConsole=(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT /* && WinVer.dwMajorVersion >= 5 */)?TRUE:FALSE;
#endif

//  Opt.ExceptRules=-1;
//_SVS(SysLog("Opt.ExceptRules=%d",Opt.ExceptRules));

  /* $ 30.12.2000 SVS
     ����������������� ������� ������ � ���������� Encryped ����� �����
     ������ FAR
  */
  GetEncryptFunctions();
  /* SVS $ */

  strcpy(Opt.RegRoot,"Software\\Far");
  /* $ 03.08.2000 SVS
     �� ��������� - ����� ������� �� ��������� ��������
  */
  Opt.LoadPlug.MainPluginDir=TRUE;
  /* SVS $ */
  Opt.LoadPlug.PluginsPersonal=TRUE;
  /* $ 01.09.2000 tran
     /co - cache only, */
  Opt.LoadPlug.PluginsCacheOnly=FALSE;
  /* tran $ */
  /* $ 09.01.2002 IS ������ ����� ����� ����� ������������ Local* */
  LocalUpperInit();
  /* IS $ */

  for (int I=1;I<Argc;I++)
  {
    if ((Argv[I][0]=='/' || Argv[I][0]=='-') && Argv[I][1])
    {
      switch(toupper(Argv[I][1]))
      {
        case 'A':
          switch (toupper(Argv[I][2]))
          {
            case 0:
              Opt.CleanAscii=TRUE;
              break;
            case 'G':
              Opt.NoGraphics=TRUE;
              break;
          }
          break;
#if defined(USE_WFUNC)
        case '8':
          Opt.UseUnicodeConsole=FALSE;
          break;
#endif
        case 'E':
          if (isdigit(Argv[I][2]))
          {
            StartLine=atoi(&Argv[I][2]);
            char *ChPtr=strchr(&Argv[I][2],':');
            if (ChPtr!=NULL)
              StartChar=atoi(ChPtr+1);
          }
          if (I+1<Argc)
          {
            CharToOem(Argv[I+1],EditName);
            I++;
          }
          break;
        case 'V':
          if (I+1<Argc)
          {
            CharToOem(Argv[I+1],ViewName);
            I++;
          }
          break;
        case 'R':
          RegOpt=TRUE;
          break;
        case 'I':
          Opt.SmallIcon=TRUE;
          break;
        case 'X':
          Opt.ExceptRules=0;
#if defined(_DEBUGEXC)
          if ( toupper(Argv[I][2])=='D' )
            Opt.ExceptRules=1;
#endif
          break;
        case 'U':
          if (I+1<Argc)
          {
            strcat(Opt.RegRoot,"\\Users\\");
            strcat(Opt.RegRoot,Argv[I+1]);
            SetEnvironmentVariable("FARUSER",Argv[I+1]);
            CopyGlobalSettings();
            I++;
          }
          break;
        case 'P':
        {
          // ������� 19
          if(Opt.Policies.DisabledOptions&FFPOL_USEPSWITCH)
             break;
          Opt.LoadPlug.PluginsPersonal=FALSE;
          Opt.LoadPlug.MainPluginDir=FALSE;
          /* $ 03.08.2000 SVS
            + ����� �������� "������ ����������� %FAR%\Plugins ������ ������� ��
              ���������� ����". ��� ���� ������������ ���� �������� �����.
              IMHO ������ ����� ��������� ������ � ��������� ����������!!!
              /P[<����>]
              ������, <����> ����� ��������� Env-����������
          */
          if (Argv[I][2])
          {
            strncpy(Opt.LoadPlug.CustomPluginsPath,&Argv[I][2],sizeof(Opt.LoadPlug.CustomPluginsPath));
            /* 18.01.2003 IS
               - �� ��������� �������������� ������� /p[<path>], ���� � ����
                 ���� ����� ������������� ��������.
            */
            CharToOem(Opt.LoadPlug.CustomPluginsPath,Opt.LoadPlug.CustomPluginsPath);
            /* IS $ */
          }
          else
          {
            // ���� ������ -P ��� <����>, ��, �������, ��� ��������
            //  ������� �� ��������� ��������!!!
            Opt.LoadPlug.CustomPluginsPath[0]=0;
          }
          break;
          /* SVS $*/
        }
        /* $ 01.09.2000 tran
           /co switch support */
        case 'C':
            if (toupper(Argv[I][2])=='O')
            {
              Opt.LoadPlug.PluginsCacheOnly=TRUE;
              Opt.LoadPlug.PluginsPersonal=FALSE;
            }
#ifdef _DEBUGEXC
            else if (toupper(Argv[I][2])=='R')
              CheckRegistration=FALSE;
#endif
            break;
        /* tran $ */
        /* $ 19.01.2001 SVS
           FAR.EXE /?
        */
        case '?':
        case 'H':
          ControlObject::ShowCopyright(1);
          show_help();
          exit(0);
        /* SVS $ */
#ifdef DIRECT_RT
        case 'D':
          if ( toupper(Argv[I][2])=='O' )
            DirectRT=1;
          break;
#endif
      }
    }
    else // ������� ���������. �� ����� ���� max ��� �����.
    {
      if(CntDestName < 2)
      {
        CharToOem(Argv[I],DestName[CntDestName++]);
      }
    }
  }

  /* $ 26.03.2002 IS
     ��������� ����������.
     ������ ���� ����� CopyGlobalSettings � ����� InitKeysArray!
  */
  InitLCIDSort();
  /* IS $ */
  /* $ 11.01.2002 IS
     ������������� ������� ������. ������ ���� ����� CopyGlobalSettings!
  */
  InitKeysArray();
  /* IS $ */

  WaitForInputIdle(GetCurrentProcess(),0);
  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));

#if _MSC_VER>=1300
  _set_new_handler(0);
#else
  set_new_handler(0);
#endif

  /* $ 08.01.2003 SVS
     BugZ#765 - ����� ��������� ������ �������� ������������.
  */
  if(Opt.LoadPlug.MainPluginDir)
    Opt.LoadPlug.PluginsCacheOnly=FALSE;

  if(Opt.LoadPlug.PluginsCacheOnly)
  {
    *Opt.LoadPlug.CustomPluginsPath=0;
    Opt.LoadPlug.MainPluginDir=FALSE;
    Opt.LoadPlug.PluginsPersonal=FALSE;
  }

  SetFileApisToOEM();
  GetModuleFileName(NULL,FarPath,sizeof(FarPath));
#if defined(USE_WFUNC)
//  if(Opt.CleanAscii || Opt.NoGraphics)
//    Opt.UseUnicodeConsole=FALSE;
#endif
  /* $ 02.07.2001 IS
     ����� ��, ��� GetModuleFileName ������ ���������� �������� ���, �������
     ��� ����� �� �����.
  */
  *(PointToName(FarPath)-1)=0;
  {
     char tmpFarPath[sizeof(FarPath)];
     DWORD s=RawConvertShortNameToLongName(FarPath, tmpFarPath,
                                           sizeof(tmpFarPath));
     if(s && s<sizeof(tmpFarPath))
        strcpy(FarPath, tmpFarPath);
  }
  AddEndSlash(FarPath);
  /* IS $ */
  /* $ 03.08.2000 SVS
     ���� �� ������ �������� -P
  */
//  if(Opt.LoadPlug.MainPluginDir)
//    sprintf(Opt.LoadPlug.CustomPluginsPath,"%s%s",FarPath,PluginsFolderName);
  /* SVS $*/
  InitDetectWindowedMode();
  InitConsole();
  GetRegKey("Language","Main",Opt.Language,"English",sizeof(Opt.Language));
  if (!Lang.Init(FarPath,MListEval))
  {
    /* $ 15.12.2000 SVS
       � ������, ���� �� ����� ������ LNG-������ - ������ ������� ���������
       � �� ����������������.
    */
    //Message(MSG_WARNING,1,"Error","Cannot load language data","Ok");
    ControlObject::ShowCopyright(1);
    fprintf(stderr,"\nError: Cannot load language data\n\nPress any key...");
    FlushConsoleInputBuffer(hConInp);
    WaitKey(); // � ����� �� ������� �������??? �����
    exit(0);
    /* SVS $ */
  }
  SetEnvironmentVariable("FARLANG",Opt.Language);
  ConvertOldSettings();
  SetHighlighting();
  /* $ 07.03.2001 IS
     - ���������� �� ������� ��� �������� ����� � ������� (�� ������ ����������
       �� ����).������� ��������� �����, ���� ��� �����, ������ ��� � ����
       ������ ��� ����� ������ ��� �������� ��������� � �������.
  */
  {
    DeleteEmptyKey(HKEY_CLASSES_ROOT,"Directory\\shellex\\CopyHookHandlers");
  }
  /* IS $ */


  int Result;
  if(Opt.ExceptRules)
  {
    TRY{
      Result=MainProcess(EditName,ViewName,DestName[0],DestName[1],StartLine,StartChar,RegOpt);
    }
    EXCEPT(xfilter((int)INVALID_HANDLE_VALUE,GetExceptionInformation(),NULL,1)){
       TerminateProcess( GetCurrentProcess(), 1);
    }
  }
  else
    Result=MainProcess(EditName,ViewName,DestName[0],DestName[1],StartLine,StartChar,RegOpt);

  SetConsoleTitle(OldTitle);
  _OT(SysLog("[[[[[Exit of FAR]]]]]]]]]"));
  return Result;
}

void ConvertOldSettings()
{
  // ��������� ������ :-) ������ �� �����...
  if(!CheckRegKey(RegColorsHighlight))
    if(CheckRegKey("Highlight"))
    {
      char NameSrc[NM],NameDst[NM];
      strcpy(NameSrc,Opt.RegRoot);
      strcat(NameSrc,"\\Highlight");
      strcpy(NameDst,Opt.RegRoot);
      strcat(NameDst,"\\");
      strcat(NameDst,RegColorsHighlight);
      CopyKeyTree(NameSrc,NameDst,"\0");
    }
  DeleteKeyTree("Highlight");
}

/* $ 03.08.2000 SVS
  ! �� ���������� ������ ������ ������ ��� ���-������
*/
void CopyGlobalSettings()
{
  if (CheckRegKey("")) // ��� ������������ - ������������
    return;
  // ������ ������ ���� - ��������� ������!
  SetRegRootKey(HKEY_LOCAL_MACHINE);
  CopyKeyTree("Software\\Far",Opt.RegRoot,"Software\\Far\\Users\0");
  SetRegRootKey(HKEY_CURRENT_USER);
  CopyKeyTree("Software\\Far",Opt.RegRoot,"Software\\Far\\Users\0Software\\Far\\PluginsCache\0");
  //  "��������" ���� �� �������!!!
  SetRegRootKey(HKEY_LOCAL_MACHINE);
  GetRegKey("System","TemplatePluginsPath",Opt.LoadPlug.PersonalPluginsPath,"",sizeof(Opt.LoadPlug.PersonalPluginsPath));
  // ������!!!
  DeleteRegKey("System");
  // ������� ����� ��������!
  SetRegRootKey(HKEY_CURRENT_USER);
  SetRegKey("System","PersonalPluginsPath",Opt.LoadPlug.PersonalPluginsPath);
}
/* SVS $ */
