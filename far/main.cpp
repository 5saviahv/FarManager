/*
far.cpp

������� main.

*/

/* Revision: 1.19 29.04.2001 $ */

/*
Modify:
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

/* $ 30.06.2000 IS
   ����������� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

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
//" /co\n"
//"      Forces FAR to load plugins from the cache only.\n"
);
}


int _cdecl main(int Argc, char *Argv[])
{
  char EditName[NM],ViewName[NM],DestName[NM];
  int StartLine=-1,StartChar=-1,RegOpt=FALSE;
  *EditName=*ViewName=*DestName=0;
  CmdMode=FALSE;

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
        case 'U':
          if (I+1<Argc)
          {
            strcat(Opt.RegRoot,"\\Users\\");
            strcat(Opt.RegRoot,Argv[I+1]);
            CopyGlobalSettings();
            I++;
          }
          break;
        case 'P':
        {
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
  *PointToName(FarPath)=0;
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
      Panel *DummyPanel=new Panel;
      CmdMode=TRUE;
      CtrlObj.Cp()->LeftPanel=CtrlObj.Cp()->RightPanel=CtrlObj.Cp()->ActivePanel=DummyPanel;
      CtrlObj.Plugins.LoadPlugins();
      if (*EditName)
        FileEditor ShellEditor(EditName,TRUE,FALSE,StartLine,StartChar);
      if (*ViewName)
        FileViewer ShellViewer(ViewName,FALSE);
      delete DummyPanel;
      CtrlObj.Cp()->LeftPanel=CtrlObj.Cp()->RightPanel=CtrlObj.Cp()->ActivePanel=NULL;
    }
    else
    {
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
      CtrlObj.ModalManager.EnterMainLoop();
    }
  }
  SetConsoleTitle(OldTitle);
  CloseConsole();
  RestoreIcons();
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


void SetHighlighting()
{
  if (CheckRegKey(RegColorsHighlight))
    return;

  int I;
  char RegKey[80], *Ptr;
  // ����� �������� %PATHEXT%, � HighlightFiles::GetHiColor() ��� �����������
  // �������.
  char CmdExt[512]="*.exe,*.com,*.bat,%PATHEXT%";
  static char *Masks[]={
    "*.*",
    CmdExt,
    "*.rar,*.r[0-9][0-9],*.arj,*.a[0-9][0-9],*.zip,*.lha,*.lzh,*.lsz,*.ain,*.ha,*.cab,*.uc2,*.j,*.uue,*.ice,*.arc,*.xxe,*.tar,*.tgz,*.gz,*.z,*.ace,*.pak,*.bz2,*.zoo,*.sit",
    "*.bak,*.tmp",
  };
  struct HighlightData  StdHighlightData[]=
  { /*
     Mask                        NormalColor       SelectedCursorColor
               IncludeAttributes       SelectedColor     MarkChar
                       ExcludeAttributes     CursorColor             */
    {Masks[0], NULL,     0x0002, 0x0000,   0x13, 0x00, 0x38, 0x00, 0x00},
    {Masks[0], NULL,     0x0004, 0x0000,   0x13, 0x00, 0x38, 0x00, 0x00},
    {Masks[0], NULL,     0x0010, 0x0000,   0x1F, 0x00, 0x3F, 0x00, 0x00},
    {Masks[1], Masks[1], 0x0000, 0x0000,   0x1A, 0x00, 0x3A, 0x00, 0x00},
    {Masks[2], NULL,     0x0000, 0x0000,   0x1D, 0x00, 0x3D, 0x00, 0x00},
    {Masks[3], NULL,     0x0000, 0x0000,   0x16, 0x00, 0x36, 0x00, 0x00},
  };

  // ��� NT ��������� CMD
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
    strcat(CmdExt,",*.cmd");

//  Add_PATHEXT(CmdExt);

  Ptr=MkRegKeyHighlightName(RegKey);
  for(I=0; I < sizeof(StdHighlightData)/sizeof(StdHighlightData[0]); ++I)
  {
    itoa(I,Ptr,10);
    SetRegKey(RegKey,"Mask",StdHighlightData[I].Masks);
    if(StdHighlightData[I].IncludeAttr)
      SetRegKey(RegKey,"IncludeAttributes",StdHighlightData[I].IncludeAttr);
    if(StdHighlightData[I].ExcludeAttr)
      SetRegKey(RegKey,"ExcludeAttributes",StdHighlightData[I].ExcludeAttr);
    if(StdHighlightData[I].Color)
      SetRegKey(RegKey,"NormalColor",StdHighlightData[I].Color);
    if(StdHighlightData[I].SelColor)
      SetRegKey(RegKey,"SelectedColor",StdHighlightData[I].SelColor);
    if(StdHighlightData[I].CursorColor)
      SetRegKey(RegKey,"CursorColor",StdHighlightData[I].CursorColor);
    if(StdHighlightData[I].CursorSelColor)
      SetRegKey(RegKey,"SelectedCursorColor",StdHighlightData[I].CursorSelColor);
    if(StdHighlightData[I].MarkChar)
      SetRegKey(RegKey,"MarkChar",StdHighlightData[I].MarkChar);
  }
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
