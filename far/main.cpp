/*
main.cpp

������� main.

*/

/* Revision: 1.37 05.10.2001 $ */

/*
Modify:
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
" /?   This help\n"
" /a   Disable display of characters with codes 0 - 31 and 255.\n"
" /ag  Disable display of pseudographics characters.\n"
//" /co  Forces FAR to load plugins from the cache only.\n"
" /e[<line>[:<pos>]] <filename>\n"
"      Edit the specified file.\n"
" /i   Set small (16x16) icon for FAR console window.\n"
" /p[<path>]\n"
"      Search for \"common\" plugins in the directory, specified by <path>.\n"
" /u <username>\n"
"      Allows to have separate settings for different users.\n"
" /v <filename>\n"
"      View the specified file. If <filename> is -, data is read from the stdin.\n"
" /co\n"
"      Forces FAR to load plugins from the cache only.\n"
" /x\n"
"      Disable exception handling.\n"
#ifdef DIRECT_RT
" /do\n"
"      Direct output.\n"
#endif
);
}


int _cdecl main(int Argc, char *Argv[])
{
  _OT(SysLog("[[[[[[[[New Session of FAR]]]]]]]]]"));
  char EditName[NM],ViewName[NM],DestName[NM];
  int StartLine=-1,StartChar=-1,RegOpt=FALSE;
  *EditName=*ViewName=*DestName=0;
  CmdMode=FALSE;

  // ���� ��� ���������, �� ��������� ���������� ����������,
  //  ����� - ������ ��� ������ �����.
#if defined(_DEBUGEXC)
  Opt.ExceptRules=-1;
#else
  if(!pIsDebuggerPresent)
    pIsDebuggerPresent=(PISDEBUGGERPRESENT)GetProcAddress(GetModuleHandle("KERNEL32"),"IsDebuggerPresent");
  Opt.ExceptRules=(pIsDebuggerPresent && pIsDebuggerPresent()?0:-1);
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
  Opt.MainPluginDir=TRUE;
  /* SVS $ */
  /* $ 01.09.2000 tran
     /co - cache only, */
  Opt.PluginsCacheOnly=FALSE;
  /* tran $ */
  for (int I=1;I<Argc;I++)
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
           if((Opt.Policies.DisabledOptions >> 19) & 1)
              break;
          /* $ 03.08.2000 SVS
            + ����� �������� "������ ����������� %FAR%\Plugins ������ ������� ��
              ���������� ����". ��� ���� ������������ ���� �������� �����.
              IMHO ������ ����� ��������� ������ � ��������� ����������!!!
              /P[<����>]
              ������, <����> ����� ��������� Env-����������
          */
          if (Argv[I][2])
          {
            ExpandEnvironmentStrings(&Argv[I][2],MainPluginsPath,sizeof(MainPluginsPath));
          }
          else
          {
            // ���� ������ -P ��� <����>, ��, �������, ��� ��������
            //  ������� �� ��������� ��������!!!
            MainPluginsPath[0]=0;
          }
          Opt.MainPluginDir=FALSE;
          break;
          /* SVS $*/
        }
        /* $ 01.09.2000 tran
           /co switch support */
        case 'C':
            if (toupper(Argv[I][2])=='O')
            {
                Opt.PluginsCacheOnly=TRUE;
            }
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
    else
      CharToOem(Argv[I],DestName);

  WaitForInputIdle(GetCurrentProcess(),0);
  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));

  set_new_handler(0);

  SetFileApisToOEM();
  WinVer.dwOSVersionInfoSize=sizeof(WinVer);
  GetVersionEx(&WinVer);
  /* $ 28.12.2000 SVS
   + Opt.HotkeyRules - ������� �� ���� ������ ��������� ������� */
  GetRegKey("Interface","HotkeyRules",Opt.HotkeyRules,1);
  /* SVS $*/
  LocalUpperInit();
  GetModuleFileName(NULL,FarPath,sizeof(FarPath));
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
  if(Opt.MainPluginDir)
    sprintf(MainPluginsPath,"%s%s",FarPath,PluginsFolderName);
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
    WaitKey(-1); // � ����� �� ������� �������??? �����
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
  {
    ChangePriority ChPriority(WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS ? THREAD_PRIORITY_ABOVE_NORMAL:THREAD_PRIORITY_NORMAL);
    ControlObject CtrlObj;
    if (*EditName || *ViewName)
    {
      NotUseCAS=TRUE;
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
      }
      if (*ViewName)
      {
        FileViewer *ShellViewer=new FileViewer(ViewName,FALSE);
        _tran(SysLog("make shellviewer, %p",ShellViewer));
      }
      FrameManager->EnterMainLoop();
      CtrlObj.Cp()->LeftPanel=CtrlObj.Cp()->RightPanel=CtrlObj.Cp()->ActivePanel=NULL;
      delete DummyPanel;
      _tran(SysLog("editor/viewer closed, delete dummy panels"));
    }
    else
    {
      NotUseCAS=FALSE;
      if (RegOpt)
        Register();
      static struct RegInfo Reg;
      _beginthread(CheckReg,0x10000,&Reg);
      while (!Reg.Done)
        Sleep(0);
      CtrlObj.Init();
      if (*DestName)
      {
        LockScreen LockScr;
        char Path[NM];
        strcpy(Path,DestName);
        *PointToName(Path)=0;
        Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
        if (*Path)
          ActivePanel->SetCurDir(Path,TRUE);
        strcpy(Path,PointToName(DestName));
        if (*Path)
        {
          if (ActivePanel->GoToFile(Path))
            ActivePanel->ProcessKey(KEY_CTRLPGDN);
        }
        CtrlObject->Cp()->LeftPanel->Redraw();
        CtrlObject->Cp()->RightPanel->Redraw();
      }
      FrameManager->EnterMainLoop();
    }
    // ������� �� �����!
    SetScreen(0,0,ScrX,ScrY,' ',F_LIGHTGRAY|B_BLACK);
    ScrBuf.ResetShadow();
    ScrBuf.Flush();
  }

  SetConsoleTitle(OldTitle);
  CloseConsole();
  RestoreIcons();
  _OT(SysLog("[[[[[Exit of FAR]]]]]]]]]"));
  return(0);
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
  GetRegKey("System","TemplatePluginsPath",Opt.PersonalPluginsPath,"",sizeof(Opt.PersonalPluginsPath));
  // ������!!!
  DeleteRegKey("System");
  // ������� ����� ��������!
  SetRegRootKey(HKEY_CURRENT_USER);
  SetRegKey("System","PersonalPluginsPath",Opt.PersonalPluginsPath);
}
/* SVS $ */
