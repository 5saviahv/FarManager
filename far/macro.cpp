#if defined(DMACRO2)
#include "macro2.cpp"
#else
/*
macro.cpp

�������

*/

/* Revision: 1.83 17.08.2002 $ */

/*
Modify:
  17.08.2002 SVS
    ! ������ DI_EDIT � ������� ���������� ������������ �������� DI_COMBOBOX.
      ��� ���� ����� ����������� ������� ��������� �������, ������� ��������
      ���������� �� ��������� (Esc, Enter, F1).
      � ���������������� ������ ������� "CtrlDown" ����� ��� ����, �����
      ����� ���� �� ������� ������ �� ������ ������, �� � � ����������.
    ! AssignMacroKey() ���������� -1 ������ Esc, �.�. ��� � ����� ���������
      ���������� ����� ��������� (����� �� ��������� ������� Esc)
  02.06.2002 SVS
    ! ��������� const
  24.05.2002 SVS
    + ��������� KEY_ALTINS
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
  16.05.2002 SVS
    - "KSFLAGS_DISABLEOUTPUT �� ��������"
  05.05.2002 SVS
    - BugZ#496 - �� �������� ������� (��� �������)
  15.04.2002 SVS
    - ����� �������.
  12.04.2002 SVS
    ! ������ #if/#endif - ����� �� ������ MAK-����� (��������������� ����)
    ! SaveMacros - ���� ��������
    ! ����� ���������: Macros - ��� ������ ���� PROM. ��� ��������� ����� MacroRAM
  03.04.2002 SVS
    - BugZ#426 - �� �������� �������������� �� ��������������� �������
  20.03.2002 DJ
    ! ���������� ��������� [x] Selection exists ��� ��������
  03.03.2002 SVS
    - BugZ#262 - ��������� macro:post � usermenu �� ��������
  03.03.2002 SVS
    ! ���� ��� VC �������� ���� /Gr, �� ����� ���� ����� :-/
  21.02.2002 SVS
    - BugZ#308. Enter �������� � �������� - ������� LastRecord � ��������
  05.02.2002 SVS
    ! ��������������� ���� - ��� �������
  16.01.2002 SVS
    - ����� ��������� ������ (���������� � ���������)
  14.01.2002 SVS
    - ������ �� Alt-����� � ������� �� �������.
  10.01.2002 SVS
    ! ������� _KEYMACRO
  03.01.2001 IS
    ! ���������� "�������� ���������" � GetMacroSettings
  28.12.2001 SVS
    - FAR ����� ����� ����������� macro:post ....
      ������� � CheckCurMacroFlags()
  26.12.2001 SVS
    ! ��� ������ ������ ����������� ������ � �����:
      "[!] ��� ���������� �������� �� ������� ������������
       �����������������������, ���������� ������ � ��������
       ������."
      �.�. ����������� ������ �� ������ � ����� ������������,
      �� � � "Sequence". � ����� �������� ������ �� �����������
      � ������.
  21.12.2001 SVS
    + MOUSEKEY
  10.12.2001 SVS
    ! ������� SetFarTitle(NULL) ����� ���������� ���������� �������.
  23.11.2001 VVM
    ! ����������� ���� ��� ������ �������-������������ �����-������� ��
      ������� �������. ����� ������ �� _��������� �����������_ �����. �
      _�������_ �����. (��� ����� ���� ��� ��� ������)
  05.10.2001 SVS
    - IsProcessAssignMacroKey � ������� ���������� ������� �������������
      ����, ��-�� ���� �� �������� CAS F12 CtrlTab ����� ���������� �������
      � ����������� ����������.
  04.10.2001 SVS
    - ���/����� ����� ������� ���������� �������.
  26.09.2001 SVS
    + ������� 18 - ���� � "�������" ��������� 18-� ��� - �� ���� �����������
      ������ �������.
  20.09.2001 SVS
    - ������� ������ ��� ����������, ���� ��������� ������ � ������� (OT)
  14.09.2001 SVS
    - BugZ#9 - ���������
  12.09.2001 SVS
    - BugZ#9 - ����� ����, ������������� �� ��������� ��������� �� ���������
               ������� ��������� �... PluginsSet::OpenPlugin
  07.09.2001 SVS
    + CheckCurMacroFlags() - �������� ������ �������� _������������_ �������.
    ! ����� ��� �������� MFLAGS_* ��������� � farconst.hpp
  15.08.2001 SVS
    ! ��������� - ��� ����������� ���� (�� ������ macro2.?pp)
    ! ��������� $StopIf
  09.08.2001 SVS
    + IfCondition() - ������ TRUE/FALSE � ����������� �� �������
    + KEY_MACROSTOPIFNOT, KEY_MACROSTOPIF - �������� ����������� ����������
      ������ ������� (���� ������� ������) � ���� ����������� � ��� :-)
  31.07.2001 SVS
    ! ������� ������� ��� ������(), ��� �� ���� �����...
  25.07.2001 SVS
    ! �� ����� ���������� ������� ��������� ���� IsProcessAssignMacroKey
  25.07.2001 SVS
    - �������������� ������� ����������� �� ���� ���, � ������� ��� ��
      �������� LoadMacros();
  24.07.2001 SVS
    - �� �������� �������������� �������
  23.07.2001 SVS
    - ������� � � ������� ��������. �� ������� �� ��������, ����.
      ����� ��������� ������ ������� ������������� �������� � �������
      ��, �� ��� ������������.
  10.07.2001 SVS
    + KEY_MACROXLAT - � �������� "$XLat" ���������� �� ������� ������ XLat
  25.06.2001 IS
    ! ��������� const
  25.06.2001 SVS
    - ���� ��� ��������� ����� ��� $Date
  24.06.2001 SVS
    ! ����������� ��������� �������� ����� 771-��.
  22.06.2001 SVS
    ! ������������ "$Year", "$Month", "$Day", "$Hour", "$Min" � "$Sec" ������
      �� �������������� � �������� �� ���� ������������ "$Date"
  22.06.2001 SVS
    ! ��������� ��������� �������� ������������
  20.06.2001 SVS
    - ����������� ������ ������� PlayKeyMacro(MacroRecord*).
      ������ ��� ��� ������ ����������� � "� ����" :-)
    ! �������� ������� ��������� � ����� ����������� �� ����������:
      PlayKeyMacro -> PostTempKeyMacro
    ! TempMacroType ������ �� �������������, �.�. ��� Temp-�������� ��� �����
      ������ ����������� ������������������.
  23.05.2001 SVS
    - ������� ����������� ������� ����� ��������.
  23.05.2001 SVS
    ! "��������" ������� �������� � ������ ���������� �����
    ! ������� ��������� � ������� GetIndex() �� ���� �������������� ������
      �������� � ������� ������� ��������.
  22.05.2001 SVS
    - �������������� ������ �� ������� - ������� ������ ��� :-(
      ��-�� ����, ��� �� ���� �� ���� ��������� ������...
  22.05.2001 tran
    ! �� ��������� ������� �� CodeGuard
  16.05.2001 SVS
    + GetCurRecord() - ��� �����
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  25.04.2001 SVS
    + MFLAGS_SELECTION - ���� �������� ���������:
      ��� ������� - ������ ������ ����������� �������,
      ��� ��������� - ��� �����.
    ! ��� �������� ������ ��� ������ �������� ������� � ������� Check* -
      ������� ����� �������������� ���� :-(
    ! ����� ������ ��������� ������� - ������������ 3-� ����������� ��������,
      ��� ��������� "�����" ������ � ��������.
  05.04.2001 VVM
    + 3 �������������� ������� �������� - "Info", "QView", "Tree"
  21.03.2001 SVS
    + ��������� ������ ������: F1 & Enter
  11.03.2001 SVS
    ! �������� MFLAGS_RUNAFTERSTART �������� �� MFLAGS_RUNAFTERFARSTART
    + ������� MkTextSequence - ������������ ���������� ������������� Sequence
    + �������� �� "������� ���� �����"
    ! ������� ������ - ������� �� ���� ������ ����� :-)
    ! ��������� ����������� ����.
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  22.02.2001 SVS
    + MFLAGS_DISABLEMACRO - ���� ������ ����������!
    ! �����, ��� ������ '~' � ������ �������� ������� - ��� �������������
      ������
    + � �������� ��� ��������, �������������� ��������� ������� ���� ���
      ������� Sequence
  19.02.2001 SVS
    - ��������� ������� ���������� ������� (��� ���������� �������)
  30.01.2001 SVS
    - ����� ������� �������� �� ��� �������� �� ������� ����������
  21.01.2001 SVS
    - ������ � ���������� ������� - ���������� ������� �������� �������, �
      �� ��������� ��� ��������� ������ �������.
  19.01.2001 SVS
    + ���������������: MFLAGS_REUSEMACRO - ��������� ������������� ��������
      ��� ���� �����, ����� ����� �������� �����������...
      � ���� ����� ����� :-)
  18.01.2001 SVS
    ! ������� ����������� ����
    + ������� ������ ������������ � ����������� ������� ���� �������
    + ������� - �������� ��������������, ��� ����� ������� ����������
      ��� ������������� ������� ������� ������������.
  09.01.2001 SVS
    - ���� � ����������� ������� ���������� �� ����� ��������� ��������
      ��������� � �������.
  09.01.2001 SVS
    + ����� ������� Opt.ShiftsKeyRules (WaitInFastFind)
  04.01.2001 SVS
    ! ���������� ��������� ��������� �������������� ������� :-)
    ! ����� ������ ���������� �������
    ! �������������� ����� - ��� ����
    + ���������� "NoFolders" � "NoFiles"
  28.12.2000 SVS
    - ���� � ������������ ��� ������.
  26.12.2000 SVS
    + KeyMacroToText()
    ! ��������� �� END_FARKEY_BASE
    + ��������� ����-�����������.
  25.12.2000 SVS
    ! MFLAGS_ ��������� �� plugin.hpp
  23.12.2000 SVS
    ! MFLAGS_ ��������� � plugin.hpp
    + int KeyMacro::ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr)
    + int KeyMacro::PlayKeyMacro(struct MacroRecord *MRec)
    + int KeyMacro::PlayKeyMacro(char *KeyBuffer)
  22.12.2000 SVS
    - �������� - ����� �������� ����� 2 ����� :-(
  22.12.2000 SVS
    - ��� ����������� ��������� ��������� ���������� �������� :-(
      ����� 333 ����� ��������� �������� ������� �����!
  21.12.2000 SVS
    ! 3-� ��������� ��� ���� �������.
    + LoadMacros(), InitVars(), ReleaseTempBuffer()
    ! ReadMacros - ���������� TRUE ��� FALSE (��� ������� �� ��������� ������)
    + TempMacroType, TempMacro - ����� �������������� ��� ������
      MCMD_PLAYRECORD, MCMD_PLAYSTRING.
  21.12.2000 SVS
    - ������� �������� ���������� ���������� �� ����� ������
      FilePanels � PluginPanels
  21.12.2000 SVS
    + "������� ������ ������  (LockScr)
    ! ������� KeyToText ������� �� �������������
    ! ��������� MacroRecord "�����"
    + 2 ������ ������ �������� � �������:
       MFLAGS_PLUGINPANEL - �������� �� ������ ��������
       MFLAGS_FILEPANEL - �������� �� �������� ������
      �� ��������� ��� ��������.
  27.09.2000 SKV
    - Don't redraw editor after macro finished if it is hidden.
  10.09.2000 SVS
    ! �������� �������� � ��������� � ����� � �������������� ������ ������
    ! ������� ReadMacros ����� �������������� ���������
  10.08.2000 skv
    ! ��������� � GetKey ��� ��������� ��������� ����� � �������.
  25.07.2000 SVS
    ! ������� KeyToText ������� ���������������� - ����� � ������
      FSF
  23.07.2000 SVS
    + �������:
       Ctrl- Shift- Alt- CtrlShift- AltShift- CtrlAlt- Apps :-)
       KEY_LWIN (VK_LWIN), KEY_RWIN (VK_RWIN)
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "macro.hpp"
#include "keys.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "plugin.hpp"
#include "lockscrn.hpp"
#include "fileedit.hpp"
#include "dialog.hpp"
#include "ctrlobj.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "cmdline.hpp"
#include "manager.hpp"
#include "scrbuf.hpp"
#include "udlist.hpp"

enum MCONDITIONTYPE {
  MCODE_WINDOWEDMODE=MACRO_LAST+1, // ������� �����?
  MCODE_APANEL_ISEMPTY,
  MCODE_PPANEL_ISEMPTY,
  MCODE_APANEL_VISIBLE,
  MCODE_PPANEL_VISIBLE,
};


// ��� ������� ���������� �������
struct DlgParam{
  KeyMacro *Handle;
  DWORD Key;
  int Mode;
};

/* $TODO:
    ������ ����� ������� �����, � ���.
    ������� ������� ��������.........
*/
static struct TMacroKeywords {
  int Type;                    // ���: 0=Area, 1=Flags, 2=Condition
  char *Name;                  // ������������
  DWORD Value;                 // ��������
  DWORD Reserved;
} MKeywords[] ={
  {0,  "Other",              MACRO_OTHER,0},
  {0,  "Shell",              MACRO_SHELL,0},
  {0,  "Viewer",             MACRO_VIEWER,0},
  {0,  "Editor",             MACRO_EDITOR,0},
  {0,  "Dialog",             MACRO_DIALOG,0},
  {0,  "Search",             MACRO_SEARCH,0},
  {0,  "Disks",              MACRO_DISKS,0},
  {0,  "MainMenu",           MACRO_MAINMENU,0},
  {0,  "Menu",               MACRO_MENU,0},
  {0,  "Help",               MACRO_HELP,0},
  {0,  "Info",               MACRO_INFOPANEL,0},
  {0,  "QView",              MACRO_QVIEWPANEL,0},
  {0,  "Tree",               MACRO_TREEPANEL,0},

  {1,  "DisableOutput",      MFLAGS_DISABLEOUTPUT,0},
  {1,  "RunAfterFARStart",   MFLAGS_RUNAFTERFARSTART,0},
  {1,  "EmptyCommandLine",   MFLAGS_EMPTYCOMMANDLINE,0},
  {1,  "NotEmptyCommandLine",MFLAGS_NOTEMPTYCOMMANDLINE,0},
  {1,  "NoFilePanels",       MFLAGS_NOFILEPANELS,0},
  {1,  "NoPluginPanels",     MFLAGS_NOPLUGINPANELS,0},
  {1,  "NoFolders",          MFLAGS_NOFOLDERS,0},
  {1,  "NoFiles",            MFLAGS_NOFILES,0},
  {1,  "ReuseMacro",         MFLAGS_REUSEMACRO,0},
  {1,  "Selection",          MFLAGS_SELECTION,0},
  {1,  "NoSelection",        MFLAGS_NOSELECTION,0},

  {2,  "Windowed",           MCODE_WINDOWEDMODE,0},
  {2,  "APanel.IsEmpty",     MCODE_APANEL_ISEMPTY,0},
  {2,  "PPanel.IsEmpty",     MCODE_PPANEL_ISEMPTY,0},
  {2,  "APanel.Visible",     MCODE_APANEL_VISIBLE,0},
  {2,  "PPanel.Visible",     MCODE_PPANEL_VISIBLE,0},
};

// ���� �����������
enum{
  KEY_MACROSTOP=KEY_MACROSPEC_BASE,
  KEY_MACROMODE,
  KEY_MACROSTOPIFNOT,
  KEY_MACROSTOPIF,
};

// ������������� ������� - ��� <-> ��� ������������
static struct TKeyCodeName{
  int Key;
  int Len;
  char *Name;
} KeyMacroCodes[]={
//   { KEY_MACROSTOPIFNOT,          10, "$StopIfNot"}, // $StopIfNot �������
//   { KEY_MACROSTOPIF,              7, "$StopIf"},  // $StopIf �������
#if defined(MOUSEKEY)
   { KEY_MACROSELWORD,             8, "$SelWord" },
#endif
   { KEY_MACROMODE,                6, "$MMode" },
   { KEY_MACRODATE,                5, "$Date"  }, // $Date "%d-%a-%Y"
   { KEY_MACROSTOP,                5, "$Stop"  },
   { KEY_MACROXLAT,                5, "$XLat"  },
};

// ������� �������������� ���� ������������ � �����
BOOL WINAPI KeyMacroToText(int Key,char *KeyText0,int Size)
{
  if(!KeyText0)
     return FALSE;

  char KeyText[128];

  KeyText[0]='\0';
  for (int I=0;I<sizeof(KeyMacroCodes)/sizeof(KeyMacroCodes[0]);I++)
    if (Key==KeyMacroCodes[I].Key)
    {
      strcpy(KeyText,KeyMacroCodes[I].Name);
      break;
    }

  if(!KeyText[0])
  {
    *KeyText0='\0';
    return FALSE;
  }
  if(Size > 0)
    strncpy(KeyText0,KeyText,Size);
  else
    strcpy(KeyText0,KeyText);

  return TRUE;
}

KeyMacro::KeyMacro()
{
  _OT(SysLog("[%p] KeyMacro::KeyMacro()", this));
  MacroRAMCount=0;
  MacroRAM=NULL;
  LockScr=NULL;
  MacroPROM=NULL;
  RecBuffer=NULL;
  StartMacroPos=-2; // ������ 1 ���(!) ����� ���������
  LoadMacros();
}

KeyMacro::~KeyMacro()
{
  _OT(SysLog("[%p] KeyMacro::~KeyMacro()", this));
  InitVars();
}

// ������������� ���� ����������
void KeyMacro::InitVars()
{
  if(MacroPROM)
  {
    for (int I=0;I<MacroPROMCount;I++)
      if(MacroPROM[I].Buffer)
        free(MacroPROM[I].Buffer);
    free(MacroPROM);
  }
  if(RecBuffer) delete[] RecBuffer;

  if(LockScr)
  {
    delete LockScr;
    LockScr=NULL;
  }

  ReleaseTempBuffer(TRUE);

  MacroPROMCount=0;
  Recording=FALSE;
  Executing=FALSE;
  MacroPROM=NULL;
  RecBuffer=NULL;
  RecBufferSize=0;
  InternalInput=FALSE;
}

// �������� ���������� ������, ���� �� ���������� �����������
// (����������� - ������ � PlayMacros �������� ������.
void KeyMacro::ReleaseTempBuffer(BOOL All)
{
  if(MacroRAM)
  {
    if(All || MacroRAMCount <= 1)
    {
      for (int I=0;I<MacroRAMCount;I++)
        if(MacroRAM[I].Buffer)
          free(MacroRAM[I].Buffer);
      free(MacroRAM);
      MacroRAM=NULL;
      MacroRAMCount=0;
    }
    else
    {
      if(MacroRAM->Buffer)
        free(MacroRAM->Buffer);
      MacroRAMCount--;
      memmove(MacroRAM,((BYTE*)MacroRAM)+sizeof(struct MacroRecord),sizeof(struct MacroRecord)*MacroRAMCount);
      realloc(MacroRAM,sizeof(struct MacroRecord)*MacroRAMCount);
    }
  }
}

// �������� ���� �������� �� �������
int KeyMacro::LoadMacros()
{
  int Ret=FALSE;
  InitVars();

  #define TEMP_BUFFER_SIZE 32768
  char *Buffer=new char[TEMP_BUFFER_SIZE];

  if(Buffer)
  {
    int I;
    for(I=MACRO_OTHER; I < MACRO_LAST; ++I)
      if(!ReadMacros(I,Buffer,TEMP_BUFFER_SIZE))
        break;
    delete[] Buffer;
    // �������� ��� �������� - ���� �� ��� ��� �����������, ��
    // ����� FALSE
    Ret=(I == MACRO_LAST)?TRUE:FALSE;
    if(Ret)
      KeyMacro::Sort();
  }
  Mode=MACRO_SHELL;
  return Ret;
}

// ������� �������������� �������� � ��� ������������
// ������ -1, ���� ��� �����������!
int WINAPI KeyNameMacroToKey(const char *Name)
{
  // ��������� �� ���� �������������
  for(int I=0; I < sizeof(KeyMacroCodes)/sizeof(KeyMacroCodes[0]); ++I)
    if(!memicmp(Name,KeyMacroCodes[I].Name,KeyMacroCodes[I].Len))
      return KeyMacroCodes[I].Key;
  return -1;
}

int KeyMacro::ProcessKey(int Key)
{
  if (InternalInput || Key==KEY_IDLE || Key==KEY_NONE)
    return(FALSE);

  if (Recording) // ���� ������?
  {
    if (Key==KEY_CTRLSHIFTDOT || Key==KEY_CTRLDOT) // ������� ����� ������?
    {
      DWORD MacroKey;
      int WaitInMainLoop0=WaitInMainLoop;
      InternalInput=TRUE;
      WaitInMainLoop=FALSE;
//_SVS(SysLog("StartMode=%d",StartMode));
//_SVS(SysLog(1));
      /* $ 23.11.2001 VVM
        ! �������� _�������_ �����, � �� _��������� �����������_ */
      FrameManager->GetCurrentFrame()->LockRefresh(); // ������� ���������� ������
      MacroKey=AssignMacroKey();
      FrameManager->ResetLastInputRecord();
      FrameManager->GetCurrentFrame()->UnlockRefresh(); // ������ ����� :-)
      /* VVM $ */
//_SVS(SysLog(-1));
//_SVS(SysLog("StartMode=%d",StartMode));

      DWORD Flags=MFLAGS_DISABLEOUTPUT;

      // ������� �������� �� ��������
      // ���� �������, �� �� ����� �������� ������ ���������.
      if (MacroKey != (DWORD)-1 && (Key==KEY_CTRLSHIFTDOT || Recording==2) && RecBufferSize)
      {
        if (!GetMacroSettings(MacroKey,Flags))
          MacroKey=(DWORD)-1;
      }
      WaitInMainLoop=WaitInMainLoop0;
      InternalInput=FALSE;

      if (MacroKey==(DWORD)-1)
        delete RecBuffer;
      else
      {
        int Pos;
        for (Pos=0;Pos<MacroPROMCount;Pos++)
          if (MacroPROM[Pos].Key==MacroKey && (MacroPROM[Pos].Flags&MFLAGS_MODEMASK)==StartMode)
            break;
        if (Pos==MacroPROMCount)
        {
          MacroPROM=(struct MacroRecord *)realloc(MacroPROM,sizeof(*MacroPROM)*(MacroPROMCount+1));
          if (MacroPROM==NULL)
          {
            MacroPROMCount=0;
            WaitInFastFind++;
            return(FALSE);
          }
          MacroPROMCount++;
        }
        else
          delete MacroPROM[Pos].Buffer;
        MacroPROM[Pos].Key=MacroKey;
        MacroPROM[Pos].Buffer=RecBuffer;
        MacroPROM[Pos].BufferSize=RecBufferSize;
        MacroPROM[Pos].Flags=Flags|(StartMode&MFLAGS_MODEMASK);
      }

      Recording=FALSE;
      RecBuffer=NULL;
      RecBufferSize=0;
      ScrBuf.RestoreMacroChar();
      WaitInFastFind++;
      KeyMacro::Sort();
      if (Opt.AutoSaveSetup)
        SaveMacros(FALSE); // �������� ������ ���������!
      return(TRUE);
    }
    else // ������� ������ ������������.
    {
      if (Key>=KEY_NONE && Key<=KEY_END_SKEY) // ����������� ������� ��������
        return(FALSE);
      RecBuffer=(DWORD *)realloc(RecBuffer,sizeof(*RecBuffer)*(RecBufferSize+1));
      if (RecBuffer==NULL)
        return(FALSE);
      RecBuffer[RecBufferSize++]=Key;
      return(FALSE);
    }
  }
  else if (Key==KEY_CTRLSHIFTDOT || Key==KEY_CTRLDOT) // ������ ������?
  {
    // ������� 18
    if((Opt.Policies.DisabledOptions >> 18) & 1)
      return FALSE;
//    if(CtrlObject->Plugins.CheckFlags(PSIF_ENTERTOOPENPLUGIN))
//      return FALSE;

    if(LockScr) delete LockScr;
    LockScr=NULL;

    // ��� ��?
    StartMode=(Mode==MACRO_SHELL && !WaitInMainLoop)?MACRO_OTHER:Mode;
    // ��� ������ - � ������� ������� �������� ���...
    Recording=(Key==KEY_CTRLSHIFTDOT) ? 2:1;
    delete RecBuffer;
    RecBuffer=NULL;
    RecBufferSize=0;
    ScrBuf.ResetShadow();
    ScrBuf.Flush();
    WaitInFastFind--;
    return(TRUE);
  }
  else
  {
    if (!Executing) // ��� ��� �� ����� ����������?
    {
      DWORD CurFlags;
//_SVS(SysLog(">Key=%s",_FARKEY_ToName(Key)));
      if((Key&0x00FFFFFF) > 0x01 && (Key&0x00FFFFFF) < 0xFF)
      {
//        Key=LocalKeyToKey(Key&0x000000FF)|(Key&(~0x000000FF));
        Key=LocalUpper(Key&0x000000FF)|(Key&(~0x000000FF));
        if((Key&0x00FFFFFF) > 0x7F)
          Key=LocalKeyToKey(Key&0x000000FF)|(Key&(~0x000000FF));
      }

//_SVS(SysLog("<Key=%s",_FARKEY_ToName(Key)));
      int I=GetIndex(Key,
                    (Mode==MACRO_SHELL && !WaitInMainLoop) ? MACRO_OTHER:Mode);
      if(I != -1 && !((CurFlags=MacroPROM[I].Flags)&MFLAGS_DISABLEMACRO) && CtrlObject)
      {
//_SVS(SysLog("KeyMacro: %d (I=%d Key=%s,%s)",__LINE__,I,_FARKEY_ToName(Key),_FARKEY_ToName(MacroPROM[I].Key)));
        if(!CheckAll(CurFlags))
          return FALSE;

        // ��������� ������� ���������� � MacroRAM
        //PostTempKeyMacro(MacroPROM+I);
        // ��������� �����?
        if (CurFlags&MFLAGS_DISABLEOUTPUT)
        {
          if(LockScr) delete LockScr;
          LockScr=new LockScreen;
        }

        Executing=TRUE;
        ExecMacroPos=I;
        ExecKeyPos=0;

        IsRedrawEditor=CtrlObject->Plugins.CheckFlags(PSIF_ENTERTOOPENPLUGIN)?FALSE:TRUE;

        if (StartMacroPos==-1) // ������� ������� ����������
          MacroPROM[I].Flags&=~MFLAGS_RUNAFTERFARSTART2;
        _KEYMACRO(SysLog("**** Start Of Execute Macro ****"));
        _KEYMACRO(SysLog(1));
        return(TRUE);
      }
    }
    return(FALSE);
  }
}

char *KeyMacro::GetMacroPlainText(char *Dest)
{
  struct MacroRecord *MR;

  MR=!MacroRAM?MacroPROM+ExecMacroPos:MacroRAM;

  int LenTextBuf=strlen((char*)&MR->Buffer[ExecKeyPos]);
  Dest[0]=0;
  if(LenTextBuf && MR->Buffer[ExecKeyPos])
  {
    strcpy(Dest,(char *)&MR->Buffer[ExecKeyPos]);
    ExecKeyPos+=LenTextBuf/4;
    if((LenTextBuf%sizeof(DWORD)) != 0)
      ++ExecKeyPos;
    return Dest;
  }
  else
    ExecKeyPos++;
  return NULL;
}

BOOL KeyMacro::IfCondition(DWORD Key,DWORD Flags,DWORD Code)
{
  int I;
  BOOL Cond=TRUE;
  char FileName[NM*2];
  int FileAttr=-1;

  // ������ ������ ������� ��������
  for(I=0; I < sizeof(MKeywords)/sizeof(MKeywords[0]); ++I)
    if(MKeywords[I].Value == Code)
      break;
  if(I == sizeof(MKeywords)/sizeof(MKeywords[0]))
    return Cond; // ����� TRUE �����������, ����� ���������� ����������
                 // �������, ��� ��� �� ���������.

  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;

  // ������ ������� ����������� ��������
  switch(MKeywords[I].Type)
  {
    case 0: // �������� �� �������
    {
      Cond=Code == FrameManager->GetCurrentFrame()->GetMacroMode()?TRUE:FALSE;
      break;
    }

    case 1: // �������� �� �����
    {
      switch(Code)
      {
        case MFLAGS_DISABLEOUTPUT: // $StopIf[Not] DisableOutput
          Cond=Flags&Code?TRUE:FALSE;
          break;

        case MFLAGS_RUNAFTERFARSTART: // $StopIf[Not] RunAfterFARStart
          Cond=Flags&MFLAGS_RUNAFTERFARSTART2?TRUE:FALSE;
          break;

        case MFLAGS_NOTEMPTYCOMMANDLINE: // $StopIf[Not] NotEmptyCommandLine
          Cond=CtrlObject->CmdLine->GetLength()!=0;
          break;
        case MFLAGS_EMPTYCOMMANDLINE: // $StopIf[Not] EmptyCommandLine
          Cond=CtrlObject->CmdLine->GetLength()==0;
          break;
        case MFLAGS_NOFILEPANELS: // $StopIf[Not] NoFilePanels
          if(ActivePanel!=NULL)
            Cond=ActivePanel->GetMode() != NORMAL_PANEL;
          break;
        case MFLAGS_NOPLUGINPANELS: // $StopIf[Not] NoPluginPanels
          if(ActivePanel!=NULL)
            Cond=ActivePanel->GetMode() != PLUGIN_PANEL;
          break;
        case MFLAGS_NOFOLDERS: // $StopIf[Not] NoFolders
        case MFLAGS_NOFILES: // $StopIf[Not] NoFiles
          if(ActivePanel!=NULL)
          {
            ActivePanel->GetFileName(FileName,ActivePanel->GetCurrentPos(),FileAttr);
            if(FileAttr != -1)
            {
              if(Code == MFLAGS_NOFOLDERS)
                Cond=!(FileAttr&FA_DIREC)?TRUE:FALSE;
              else
                Cond=(FileAttr&FA_DIREC)?TRUE:FALSE;
//_SVS(SysLog("FileName='%s', FileAttr=0x%08X, Cond=%d",FileName,FileAttr,Cond));
            }
            else
              Cond=TRUE; //???
          }
          break;
        case MFLAGS_SELECTION: // $StopIf[Not] Selection
        case MFLAGS_NOSELECTION: // $StopIf[Not] NoSelection
        {
          int Mode=FrameManager->GetCurrentFrame()->GetMacroMode();
          if(Mode == MACRO_EDITOR) //???
          {
            Frame* CurFrame=FrameManager->GetCurrentFrame();
            if (CurFrame && CurFrame->GetType()==MODALTYPE_EDITOR)
            {
              int CurSelected=CurFrame->ProcessKey(KEY_MEDIT_ISSELECTED);
              if(Code == MFLAGS_SELECTION)
                Cond=CurSelected?TRUE:FALSE;
              else
                Cond=CurSelected?FALSE:TRUE;
            }
          }
          else if(ActivePanel!=NULL)
          {
            int SelCount=ActivePanel->GetRealSelCount();
            if(Code == MFLAGS_SELECTION)
              Cond=SelCount >= 1;
            else
              Cond=SelCount < 1;
          }
          break;
        }
      }
      break;
    }

    case 2:
    {
      Panel *PassivePanel=NULL;
      int GetFileCount=0;
      if(ActivePanel!=NULL)
        PassivePanel=CtrlObject->Cp()->GetAnotherPanel(ActivePanel);
//_SVS(SysLog("Code=2=%d",Code));
      switch(Code)
      {
        case MCODE_WINDOWEDMODE: // Windowed?
          // FarAltEnter(-2) -> 0 - Windowed, 1 - FulScreen
          Cond=FarAltEnter(-2)==0?TRUE:FALSE;
          break;

        case MCODE_APANEL_ISEMPTY:
        case MCODE_PPANEL_ISEMPTY:
        {
          Panel *SelPanel=Code==MCODE_APANEL_ISEMPTY?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
          {
            SelPanel->GetFileName(FileName,SelPanel->GetCurrentPos(),FileAttr);
            GetFileCount=SelPanel->GetFileCount();
            Cond=GetFileCount == 0 ||
                 GetFileCount == 1 && !strcmp(FileName,"..")
                 ?TRUE:FALSE;
          }
//          _SVS(SysLog("SelPanel->GetFileCount()=%d",SelPanel->GetFileCount()));
          break;
        }

        case MCODE_APANEL_VISIBLE:
        case MCODE_PPANEL_VISIBLE:
        {
          Panel *SelPanel=Code==MCODE_APANEL_VISIBLE?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
            Cond=SelPanel->IsVisible();
          break;
        }

      }
    }
  }

  // ������, ��������� ������� - ��� ��� �� ���
  if(Key == KEY_MACROSTOPIF && Cond) // $StopIf Flag|Area
    return TRUE;

  if(Key == KEY_MACROSTOPIFNOT && !Cond) //$StopIfNot Flag|Area
    return TRUE;

  return FALSE;
}

// �������� ��������� ��� ������� �� �������
int KeyMacro::GetKey()
{
  struct MacroRecord *MR;
//_SVS(SysLog(">KeyMacro::GetKey() InternalInput=%d Executing=%d (%p)",InternalInput,Executing,FrameManager->GetCurrentFrame()));
  if (InternalInput || !Executing || !FrameManager->GetCurrentFrame())
    return(FALSE);

initial:
  MR=!MacroRAM?MacroPROM+ExecMacroPos:MacroRAM;
//_SVS(SysLog("KeyMacro::GetKey() initial: ExecKeyPos=%d (%d) %p",ExecKeyPos,MR->BufferSize,MacroRAM));

  // ��������! �������� �����!
  if(!ExecKeyPos && !LockScr && (MR->Flags&MFLAGS_DISABLEOUTPUT))
    LockScr=new LockScreen;

begin:
  if (ExecKeyPos>=MR->BufferSize || MR->Buffer==NULL)
  {
done:
    /*$ 10.08.2000 skv
      If we are in editor mode, and CurEditor defined,
      we need to call this events.
      EE_REDRAW 2 - to notify that text changed.
      EE_REDRAW 0 - to notify that whole screen updated
      ->Show() to actually update screen.

      This duplication take place since ShowEditor method
      will NOT send this event while screen is locked.
    */
    if(Mode==MACRO_EDITOR &&
       IsRedrawEditor &&
       CtrlObject->Plugins.CurEditor &&
       CtrlObject->Plugins.CurEditor->IsVisible() &&
       LockScr)
    {
      CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE);
      CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL);
      CtrlObject->Plugins.CurEditor->Show();
    }
    /* skv$*/
    if(LockScr) delete LockScr;
    LockScr=NULL;
    Executing=FALSE;
    ReleaseTempBuffer();
    // �������� - "� ���� �� � ��������� ����� ��� �������"?
    if(MacroRAMCount > 0)
    {
      // �������, �������� ��������� �� �����
      Executing=TRUE;
      ExecKeyPos=0;
    }
    SetFarTitle(NULL); // �������� ������ ��������� �� ���������� �������
    //FrameManager->RefreshFrame();
    //FrameManager->PluginCommit();
    _KEYMACRO(SysLog(-1));
    _KEYMACRO(SysLog("**** End Of Execute Macro ****"));
    return(FALSE);
  }

  DWORD Key=MR->Buffer[ExecKeyPos++];
  switch(Key)
  {
    case KEY_MACROSTOPIFNOT: // $StopIfNot Flag|Area
    case KEY_MACROSTOPIF:    // $StopIf Flag|Area
    {
      if(IfCondition(Key,MR->Flags,MR->Buffer[ExecKeyPos++]))
        goto done;
      goto begin;
    }

    case KEY_MACROSTOP:
      goto done;

    case KEY_ALTINS:
    {
      if(RunGraber())
        return KEY_NONE;
      break;
    }

    case KEY_MACROMODE:
      if (ExecKeyPos<MR->BufferSize)
      {
        Key=MR->Buffer[ExecKeyPos++];
        if(Key == '1')
        {
          DWORD Flags=MR->Flags;
          if(Flags&MFLAGS_DISABLEOUTPUT) // ���� ��� - ������
          {
            if(LockScr) delete LockScr;
            LockScr=NULL;
          }

          SwitchFlags(MR->Flags,MFLAGS_DISABLEOUTPUT);

          if(MR->Flags&MFLAGS_DISABLEOUTPUT) // ���� ���� - �������
          {
            if(LockScr) delete LockScr;
            LockScr=new LockScreen;
          }
        }
        goto begin;
      }
  }
//_SVS(SysLog("%s.%s.Key=%s ExecMacroPos=%d ExecKeyPos=%d", GetSubKey(Mode),GetSubKey(MacroPROM[ExecMacroPos].Flags&MFLAGS_MODEMASK),_FARKEY_ToName(Key),ExecMacroPos,ExecKeyPos));
  return(Key);
}

// ��������� - ����� �� ��� �������?
int KeyMacro::PeekKey()
{
  struct MacroRecord *MR=!MacroRAM?MacroPROM+ExecMacroPos:MacroRAM;
  if (InternalInput || !Executing || ExecKeyPos >= MR->BufferSize)
    return(0);
  int Key=MR->Buffer[ExecKeyPos];
  return(Key);
}

DWORD KeyMacro::SwitchFlags(DWORD& Flags,DWORD Value)
{
  if(Flags&Value) Flags&=~Value;
  else Flags|=Value;
  return Flags;
}


char *KeyMacro::MkRegKeyName(int IdxMacro,char *RegKeyName)
{
  char KeyText[50];
  ::KeyToText(MacroPROM[IdxMacro].Key,KeyText);
  sprintf(RegKeyName,"KeyMacros\\%s\\%s%s",
     GetSubKey(MacroPROM[IdxMacro].Flags&MFLAGS_MODEMASK),
     (MacroPROM[IdxMacro].Flags&MFLAGS_DISABLEMACRO?"~":""),
     KeyText);
  return RegKeyName;
}

// ����� ������ ���� ������� ����� ������� ������!!!
char *KeyMacro::MkTextSequence(DWORD *Buffer,int BufferSize)
{
  int I,J;
  char MacroKeyText[50], *TextBuffer;

  // �������� �������� ������� �����
  if((TextBuffer=(char*)malloc(BufferSize*64+16)) == NULL)
    return NULL;

  TextBuffer[0]=0;

  for (J=0; J < BufferSize; J++)
  {
    if(KeyToText(Buffer[J],MacroKeyText))
    {
      if(J)
        strcat(TextBuffer," ");
      strcat(TextBuffer,MacroKeyText);

      switch(Buffer[J])
      {
        /* $Date
           0: KEY_MACRODATE
           1: ������ ���������� �� 4 �����
              ���� ������ �����, �� ��������� �� KEY_MACRODATE DWORD = 0
        */
        case KEY_MACRODATE:
        {
          ++J;
          int LenTextBuf=strlen((char*)&Buffer[J]);
          if(LenTextBuf)
          {
            strcat(TextBuffer," \"");
            strcat(TextBuffer,(char*)&Buffer[J]);
            strcat(TextBuffer,"\"");
          }
          // ����� ��� ������ ������ 4
          J+=LenTextBuf/sizeof(DWORD);
          // ������� ����� ��������� ��� ��������� �������� �����
          break;
        }

        case KEY_MACROSTOPIFNOT:
        case KEY_MACROSTOPIF:
        {
          ++J;
          for(I=0; I < sizeof(MKeywords)/sizeof(MKeywords[0]); ++I)
            if(MKeywords[I].Value == Buffer[J])
            {
              strcat(TextBuffer," ");
              strcat(TextBuffer,MKeywords[I].Name);
              break;
            }
          break;
        }
        // *****
      }
    }
  }
  return TextBuffer;
}

// ���������� ���� ��������
void KeyMacro::SaveMacros(BOOL AllSaved)
{
  char *TextBuffer;
  char RegKeyName[150];
  for (int I=0;I<MacroPROMCount;I++)
  {
    MkRegKeyName(I,RegKeyName);

    if (MacroPROM[I].BufferSize==0)
    {
      DeleteRegKey(RegKeyName);
      continue;
    }

    if((TextBuffer=MkTextSequence(MacroPROM[I].Buffer,MacroPROM[I].BufferSize)) == NULL)
      continue;

    SetRegKey(RegKeyName,"Sequence",TextBuffer);
    //_SVS(SysLog("%3d) %s|Sequence='%s'",I,RegKeyName,TextBuffer));
    if(TextBuffer)
      free(TextBuffer);

    // ����������� ����...
    for(int J=0; J < sizeof(MKeywords)/sizeof(MKeywords[0]); ++J)
    {
      if(MKeywords[J].Type == 1)
      {
        if (MacroPROM[I].Flags & MKeywords[J].Value)
          SetRegKey(RegKeyName,MKeywords[J].Name,1);
        else
          DeleteRegValue(RegKeyName,MKeywords[J].Name);
      }
    }
  }
}


/* $ 10.09.2000 SVS
  ! �������� �������� � ��������� � ����� � �������������� ������ ������
  ! ������� ReadMacros ����� �������������� ���������
*/
int KeyMacro::ReadMacros(int ReadMode,char *Buffer,int BufferSize)
{
  int I, J;

  char UpKeyName[100];
  sprintf(UpKeyName,"KeyMacros\\%s",GetSubKey(ReadMode));

  for (I=0;;I++)
  {
    char RegKeyName[150],KeyText[50];
    DWORD MFlags=0;

    if (!EnumRegKey(UpKeyName,I,RegKeyName,sizeof(RegKeyName)))
      break;
    char *KeyNamePtr=strrchr(RegKeyName,'\\');
    if (KeyNamePtr!=NULL)
    {
      strcpy(KeyText,KeyNamePtr+1);
      // ������! ��� �������� �������, ������������ �� ������ ~ - ���
      // ������������� ������!!!
      if(*KeyText == '~' && KeyText[1])
      {
        char *Ptr=KeyText+1;
        while(*Ptr && *Ptr == '~')// && IsSpace(KeyText[1]))
          ++Ptr;
        memmove(KeyText,Ptr,strlen(Ptr)+1);
        MFlags|=MFLAGS_DISABLEMACRO;
      }
    }
    else
      *KeyText=0;
    int KeyCode=KeyNameToKey(KeyText);
    if (KeyCode==-1)
      continue;

    struct MacroRecord CurMacro;
    CurMacro.Key=KeyCode;
    CurMacro.Buffer=NULL;
    CurMacro.BufferSize=0;
    CurMacro.Flags=MFlags|(ReadMode&MFLAGS_MODEMASK);
    GetRegKey(RegKeyName,"Sequence",Buffer,"",BufferSize);

    for(J=0; J < sizeof(MKeywords)/sizeof(MKeywords[0]); ++J)
      if(MKeywords[J].Type == 1)
        CurMacro.Flags|=GetRegKey(RegKeyName,MKeywords[J].Name,0)?MKeywords[J].Value:0;

    if(!ParseMacroString(&CurMacro,Buffer))
      continue;

    struct MacroRecord *NewMacros=(struct MacroRecord *)realloc(MacroPROM,sizeof(*MacroPROM)*(MacroPROMCount+1));
    if (NewMacros==NULL)
    {
      return FALSE;
    }
    MacroPROM=NewMacros;
    memcpy(MacroPROM+MacroPROMCount,&CurMacro,sizeof(CurMacro));
    MacroPROMCount++;
  }
  return TRUE;
}
/* SVS $ */

// �������, ����������� ������� ��� ������ ����
// ���� �� ��������� �������������� � �������������� ���������
// �������� ��������, �� ������ ����!
void KeyMacro::RunStartMacro()
{
  if (StartMacroPos==-1)
    return;

  if(StartMacroPos==-2)
    StartMacroPos=IndexMode[MACRO_SHELL][0];

  DWORD CurFlags;
  while (StartMacroPos<MacroPROMCount)
  {
    int CurPos=StartMacroPos++;

    if (((CurFlags=MacroPROM[CurPos].Flags)&MFLAGS_MODEMASK)==MACRO_SHELL &&
        MacroPROM[CurPos].BufferSize>0 &&
        // ��������� �� ������������� �������
        !(CurFlags&MFLAGS_DISABLEMACRO) &&
        (CurFlags&MFLAGS_RUNAFTERFARSTART) && CtrlObject)
    {
      if(!CheckAll(CurFlags))
        return;

      if (CurFlags&MFLAGS_DISABLEOUTPUT)
      {
        if(LockScr) delete LockScr;
        LockScr=new LockScreen;
      }
      Executing=TRUE;
      ExecMacroPos=CurPos;
      ExecKeyPos=0;
      // �������� ������� ����, ��� ������ ��������� ��� ����������.
      MacroPROM[CurPos].Flags|=MFLAGS_RUNAFTERFARSTART2;
      return;
    }
  }
  StartMacroPos=-1;
}

// ���������� ����������� ���� ���������� �������
long WINAPI KeyMacro::AssignMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  char KeyText[50];
  static int LastKey;
  static struct DlgParam *KMParam=NULL;
  int Index, I;

  if(Msg == DN_INITDIALOG)
  {
    KMParam=(struct DlgParam *)Param2;
    LastKey=0;

    // <�������, ������� �� ������� � ������� ����������>
    static const char * const PreDefKeyName[]={
      "CtrlDown", "Enter", "Esc", "F1"
    };
    for(I=0; I < sizeof(PreDefKeyName)/sizeof(PreDefKeyName[0]); ++I)
      Dialog::SendDlgMessage(hDlg,DM_LISTADDSTR,2,(long)PreDefKeyName[I]);
/*
    int KeySize=GetRegKeySize("KeyMacros","DlgKeys");
    char *KeyStr;
    if(KeySize &&
       (KeyStr=(char*)malloc(KeySize+1)) != NULL &&
       GetRegKey("KeyMacros","DlgKeys",KeyStr,"",KeySize)
      )
    {
      UserDefinedList KeybList;
      if(KeybList.Set(KeyStr))
      {
        KeybList.Start();
        const char *OneKey;
        *KeyText=0;
        while(NULL!=(OneKey=KeybList.GetNext()))
        {
          strncpy(KeyText, OneKey, sizeof(KeyText)-1);
          Dialog::SendDlgMessage(hDlg,DM_LISTADDSTR,2,(long)KeyText);
        }
      }
      free(KeyStr);
    }
*/
    Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,2,(long)"");
    // </�������, ������� �� ������� � ������� ����������>

  }
  else if(Param1 == 2 && Msg == DN_EDITCHANGE)
  {
    LastKey=0;
    Param2=KeyNameToKey(((struct FarDialogItem*)Param2)->Data.Data);
    if(Param2 != -1)
      goto M1;
  }
  else if(Msg == DN_KEY && (Param2&KEY_END_SKEY) < KEY_END_FKEY)
  {
//_SVS(SysLog("Macro: Key=%s",_FARKEY_ToName(Param2)));
    // <��������� ������ ������: F1 & Enter>
    // Esc & (Enter � ���������� Enter) - �� ������������
    if(Param2 == KEY_ESC ||
       Param2 == KEY_ENTER && LastKey == KEY_ENTER ||
       Param2 == KEY_CTRLDOWN ||
       Param2 == KEY_F1
      )
    {
      return FALSE;
    }
/*
    // F1 - ������ ������ - ����� ���� 2 ����
    // ������ ��� ����� ������� ����,
    // � ������ ��� - ������ ��� ��� ����������
    if(Param2 == KEY_F1 && LastKey!=KEY_F1)
    {
      LastKey=KEY_F1;
      return FALSE;
    }
*/
    // ���� ���-�� ��� ������ � Enter`�� ������������
    if(Param2 == KEY_ENTER && LastKey && LastKey != KEY_ENTER)
      return FALSE;
    // </��������� ������ ������: F1 & Enter>
M1:
    KeyMacro *MacroDlg=KMParam->Handle;

    if((Param2&0x00FFFFFF) > 0x7F && (Param2&0x00FFFFFF) < 0xFF)
      Param2=LocalKeyToKey(Param2&0x000000FF)|(Param2&(~0x000000FF));

    KMParam->Key=(DWORD)Param2;
    KeyToText(Param2,KeyText);

    // ���� ��� ���� ����� ������...
    if((Index=MacroDlg->GetIndex(Param2,KMParam->Mode)) != -1)
    {
      struct MacroRecord *Mac=MacroDlg->MacroPROM+Index;
      DWORD DisFlags=Mac->Flags&MFLAGS_DISABLEMACRO;
      char Buf[256];
      char BufKey[64];
      char RegKeyName[150];
      char *TextBuffer;

      MacroDlg->MkRegKeyName(Index,RegKeyName);
      // ����� �� ������.
      if((TextBuffer=MacroDlg->MkTextSequence(Mac->Buffer,Mac->BufferSize)) != NULL)
      {
        int F=0;
        I=strlen(TextBuffer);
        if(I > 45) { I=45; F++; }
        sprintf(Buf,"\"%*.*s%s\"",I,I,TextBuffer,(F?"...":""));
        strcpy(BufKey,Buf);
        free(TextBuffer);
      }
      else
        BufKey[0]=0;

      sprintf(Buf,
        MSG(!MacroDlg->RecBufferSize?
           (DisFlags?MMacroDeleteAssign:MMacroDeleteKey):
           MMacroReDefinedKey),
        KeyText);

      // �������� "� �� ��������� �� ��?"
      if(!DisFlags &&
         Mac->Buffer && MacroDlg->RecBuffer &&
         Mac->BufferSize == MacroDlg->RecBufferSize &&
         !memcmp(Mac->Buffer,MacroDlg->RecBuffer,MacroDlg->RecBufferSize*sizeof(DWORD)))
        I=0;
      else
        I=Message(MSG_WARNING,2,MSG(MWarning),
            Buf,
            BufKey,
            MSG(!MacroDlg->RecBufferSize?
                  MMacroDeleteKey2:
                  (DisFlags?MMacroDisDisabledKey:MMacroReDefinedKey2)),
            MSG(DisFlags && MacroDlg->RecBufferSize?MMacroDisOverwrite:MYes),
            MSG(DisFlags && MacroDlg->RecBufferSize?MMacroDisAnotherKey:MNo));

      if(!I)
      {
        if(DisFlags)
        {
          if (Opt.AutoSaveSetup) // ������� �� ������� ������ � ������
          {                      // ����� ������� ��������
            // ������ ������ ������ �� �������
            DeleteRegKey(RegKeyName);
          }
          // �����������
          Mac->Flags&=~MFLAGS_DISABLEMACRO;
        }
        // � ����� ������ - ������������
        Dialog::SendDlgMessage(hDlg,DM_CLOSE,1,0);
        return TRUE;
      }
      // ����� - ����� �� �������� "���", �� � �� ��� � ���� ���
      //  � ������ ������� ���� �����.
      KeyText[0]=0;
    }
    Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,2,(long)KeyText);
//    if(Param2 == KEY_F1 && LastKey == KEY_F1)
//      LastKey=-1;
//    else
      LastKey=Param2;
    return TRUE;
  }
  else if(Msg == DN_CTLCOLORDLGITEM) // ������� Unchanged
  {
    Param2&=0xFF00FFFFU;      // Unchanged � ��� ����� � ������� ����� �������� �����
    Param2|=(Param2&0xFF)<<16;
    return Param2;
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

DWORD KeyMacro::AssignMacroKey()
{
/*
  +------ Define macro ------+
  | Press the desired key    |
  | ________________________ |
  +--------------------------+
*/
  static struct DialogData MacroAssignDlgData[]={
  /* 00 */ DI_DOUBLEBOX,3,1,30,4,0,0,0,0,(char *)MDefineMacroTitle,
  /* 01 */ DI_TEXT,-1,2,0,0,0,0,DIF_BOXCOLOR|DIF_READONLY,0,(char *)MDefineMacro,
  /* 02 */ DI_COMBOBOX,5,3,28,3,1,0,0,1,"",
  };
  MakeDialogItems(MacroAssignDlgData,MacroAssignDlg);
  struct DlgParam Param={this,0,StartMode};
//_SVS(SysLog("StartMode=%d",StartMode));

  IsProcessAssignMacroKey++;
  Dialog Dlg(MacroAssignDlg,sizeof(MacroAssignDlg)/sizeof(MacroAssignDlg[0]),AssignMacroDlgProc,(long)&Param);
  Dlg.SetPosition(-1,-1,34,6);
  Dlg.SetHelp("KeyMacro");
  Dlg.Process();
  IsProcessAssignMacroKey--;
  /* $ 30.01.2001 SVS
     ����� ������� �������� �� ��� �������� �� ������� ����������
  */
  if(Dlg.GetExitCode() == -1)
    return (DWORD)-1;
  /* SVS $ */
  return Param.Key;
}

static int Set3State(DWORD Flags,DWORD Chk1,DWORD Chk2)
{
  DWORD Chk12=Chk1|Chk2, FlagsChk12=Flags&Chk12;
  if(FlagsChk12 == Chk12 || FlagsChk12 == 0)
    return (2);
  else
    return (Flags&Chk1?1:0);
}

/* $ 03.01.2001 IS
   ! ���������� "�������� ���������"
*/
int KeyMacro::GetMacroSettings(int Key,DWORD &Flags)
{

  static struct DialogData MacroSettingsDlgData[]={
  /* 00 */ DI_DOUBLEBOX,3,1,62,11,0,0,0,0,"",
  /* 01 */ DI_CHECKBOX,5,2,0,0,1,0,0,0,(char *)MMacroSettingsEnableOutput,
  /* 02 */ DI_CHECKBOX,5,3,0,0,0,0,0,0,(char *)MMacroSettingsRunAfterStart,
  /* 03 */ DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */ DI_CHECKBOX,5,5,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsCommandLine,
  /* 05 */ DI_CHECKBOX,5,6,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsPluginPanel,
  /* 06 */ DI_CHECKBOX,5,7,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsFolders,
  /* 07 */ DI_CHECKBOX,5,8,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsSelectionPresent,
  /* 08 */ DI_TEXT,3,9,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 09 */ DI_BUTTON,0,10,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
  /* 10 */ DI_BUTTON,0,10,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(MacroSettingsDlgData,MacroSettingsDlg);

  char KeyText[66];
  KeyToText(Key,KeyText);
  sprintf(MacroSettingsDlg[0].Data,MSG(MMacroSettingsTitle),KeyText);
//  if(!(Key&0x7F000000))
//    MacroSettingsDlg[3].Flags|=DIF_DISABLE;

  MacroSettingsDlg[1].Selected=Flags&MFLAGS_DISABLEOUTPUT?0:1;
  MacroSettingsDlg[2].Selected=Flags&MFLAGS_RUNAFTERFARSTART?1:0;

  MacroSettingsDlg[4].Selected=Set3State(Flags,MFLAGS_EMPTYCOMMANDLINE,MFLAGS_NOTEMPTYCOMMANDLINE);
  MacroSettingsDlg[5].Selected=Set3State(Flags,MFLAGS_NOFILEPANELS,MFLAGS_NOPLUGINPANELS);
  MacroSettingsDlg[6].Selected=Set3State(Flags,MFLAGS_NOFILES,MFLAGS_NOFOLDERS);
  MacroSettingsDlg[7].Selected=Set3State(Flags,MFLAGS_SELECTION,MFLAGS_NOSELECTION);

  Dialog Dlg(MacroSettingsDlg,sizeof(MacroSettingsDlg)/sizeof(MacroSettingsDlg[0]));
  Dlg.SetPosition(-1,-1,66,13);
  Dlg.SetHelp("KeyMacroSetting");
  FrameManager->GetBottomFrame()->LockRefresh(); // ������� ���������� ������
  Dlg.Process();
  FrameManager->GetBottomFrame()->UnlockRefresh(); // ������ ����� :-)
  if (Dlg.GetExitCode()!=9)
    return(FALSE);

  Flags=MacroSettingsDlg[1].Selected?0:MFLAGS_DISABLEOUTPUT;
  Flags|=MacroSettingsDlg[2].Selected?MFLAGS_RUNAFTERFARSTART:0;
  Flags|=MacroSettingsDlg[4].Selected==2?0:
          (MacroSettingsDlg[4].Selected==0?MFLAGS_NOTEMPTYCOMMANDLINE:MFLAGS_EMPTYCOMMANDLINE);
  Flags|=MacroSettingsDlg[5].Selected==2?0:
          (MacroSettingsDlg[5].Selected==0?MFLAGS_NOPLUGINPANELS:MFLAGS_NOFILEPANELS);
  Flags|=MacroSettingsDlg[6].Selected==2?0:
          (MacroSettingsDlg[6].Selected==0?MFLAGS_NOFOLDERS:MFLAGS_NOFILES);
  Flags|=MacroSettingsDlg[7].Selected==2?0:
          (MacroSettingsDlg[7].Selected==0?MFLAGS_NOSELECTION:MFLAGS_SELECTION);

  return(TRUE);
}
/* IS $ */

int KeyMacro::PostTempKeyMacro(char *KeyBuffer)
{
  struct MacroRecord NewMacroRAM2={0};

  // ������� ������� �� ������
  if(!ParseMacroString(&NewMacroRAM2,KeyBuffer))
  {
    free(NewMacroRAM2.Buffer);
    return FALSE;
  }

  // ������ ��������� �������� ������� ������ ������
  struct MacroRecord *NewMacroRAM;
  if((NewMacroRAM=(struct MacroRecord *)realloc(MacroRAM,sizeof(MacroRecord)*(MacroRAMCount+1))) == NULL)
    return FALSE;

  // ������ ������� � ���� "�������" ����� ������
  MacroRAM=NewMacroRAM;
  NewMacroRAM=MacroRAM+MacroRAMCount;
  memcpy(NewMacroRAM,&NewMacroRAM2,sizeof(struct MacroRecord));
  MacroRAMCount++;

  Executing=TRUE;
  ExecKeyPos=0;
  return TRUE;
}

int KeyMacro::PostTempKeyMacro(struct MacroRecord *MRec)
{
  if(!MRec)
    return FALSE;

  struct MacroRecord NewMacroRAM2={0};
  memcpy(&NewMacroRAM2,MRec,sizeof(struct MacroRecord));
  if((NewMacroRAM2.Buffer=(DWORD*)malloc(MRec->BufferSize*sizeof(DWORD))) == NULL)
  {
    return FALSE;
  }

  // ������ ��������� �������� ������� ������ ������
  struct MacroRecord *NewMacroRAM;
  if((NewMacroRAM=(struct MacroRecord *)realloc(MacroRAM,sizeof(MacroRecord)*(MacroRAMCount+1))) == NULL)
  {
    free(NewMacroRAM2.Buffer);
    return FALSE;
  }

  // ������ ������� � ���� "�������" ����� ������
  memcpy(NewMacroRAM2.Buffer,MRec->Buffer,sizeof(DWORD)*MRec->BufferSize);

  MacroRAM=NewMacroRAM;
  NewMacroRAM=MacroRAM+MacroRAMCount;
  memcpy(NewMacroRAM,&NewMacroRAM2,sizeof(struct MacroRecord));
  MacroRAMCount++;

  Executing=TRUE;
  ExecKeyPos=0;
  return TRUE;
}

// ������ ��������� ������������ � ���� ������
int KeyMacro::ParseMacroString(struct MacroRecord *CurMacro,const char *BufPtr)
{
  int J,I;
  BOOL IsKeyWord2;
  DWORD KeyCode=KEY_NONE;
  DWORD PrevKeyKode=KEY_NONE; // ��� $StopIf
  int Size;
  char CurKeyText[NM*2];

  if(!CurMacro || !BufPtr || !*BufPtr)
    return FALSE;

  IsKeyWord2=FALSE;
  // ����� ��������� ������������, �������� ������ ������������������,
  // ������� ��������� � Buffer
  while (1)
  {
    // ���������� ������� ���������� �������
    while (IsSpace(*BufPtr))
      BufPtr++;
    if (*BufPtr==0)
      break;

    const char *CurBufPtr=BufPtr;

    // ���� ����� ���������� �������� �������
    while (*BufPtr && !IsSpace(*BufPtr))
      BufPtr++;
    int Length=BufPtr-CurBufPtr;

    memcpy(CurKeyText,CurBufPtr,Length);
    CurKeyText[Length]=0;

    // � CurKeyText - �������� �������. ��������� �������� �� ���...
    PrevKeyKode=KeyCode;
    KeyCode=KEY_NONE;
    if(!IsKeyWord2)
    {
      if((KeyCode=KeyNameToKey(CurKeyText)) == -1)
      {
        if (CurMacro->Buffer!=NULL)
        {
          free(CurMacro->Buffer);
          CurMacro->Buffer=NULL;
        }
        CurMacro->BufferSize=0;
        return FALSE;
      }
    }
    else
      for(I=0;I < sizeof(MKeywords)/sizeof(MKeywords[0]); ++I)
        if(!stricmp(MKeywords[I].Name,CurKeyText))
        {
          KeyCode=MKeywords[I].Value;
          break;
        }
    if(KeyCode == KEY_NONE)
    {
      if(PrevKeyKode == KEY_MACROSTOPIFNOT)
      {
        KeyCode=1; //??
      }
      else if(PrevKeyKode == KEY_MACROSTOPIF)
      {
        KeyCode=0; //??
      }
    }
    IsKeyWord2=FALSE;

    Size=1;
    /* $Date
       0: KEY_MACRODATE
       1: ������, ����������� �� 4 �����
          ���� ������ �����, �� ��������� �� KEY_MACRODATE DWORD = 0
    */
    switch(KeyCode)
    {
      case KEY_MACRODATE:
      {
        const char *BufPtr2=BufPtr;
        memset(CurKeyText,0,sizeof(CurKeyText));
        // ���� ������ �������
        while (*BufPtr && *BufPtr != '"')
          BufPtr++;
        if(*BufPtr)
        {
            ++BufPtr;
          // ���� �������� �������
          char *PtrCurKeyText=CurKeyText;
          while (*BufPtr)
          {
            if(*BufPtr == '\\' && BufPtr[1] == '"')
            {
              *PtrCurKeyText++='\\';
              *PtrCurKeyText++='"';
              BufPtr+=2;
            }
            else if(*BufPtr == '"')
            {
              *PtrCurKeyText=0;
              BufPtr++;
              break;
            }
            else
              *PtrCurKeyText++=*BufPtr++;
          }
          if(*BufPtr)
            BufPtr++;
        }
        else
          BufPtr=BufPtr2;
        Length=strlen(CurKeyText)+1;
        // ������ ������ ���� ��������� �� 4
        Size+=Length/sizeof(DWORD);
        if(Length==1 || (Length%sizeof(DWORD)) != 0) // ���������� �� sizeof(DWORD) ������.
          Size++;
        break;
      }

      case KEY_MACROSTOPIFNOT:
      case KEY_MACROSTOPIF:
        IsKeyWord2=TRUE;
        break;
    }

    // ��� ������, ������� ���� ��� � ����� ������������������.
    if (KeyCode!=-1)
    {
      CurMacro->Buffer=(DWORD *)realloc(CurMacro->Buffer,sizeof(*CurMacro->Buffer)*(CurMacro->BufferSize+Size));
      if (CurMacro->Buffer==NULL)
      {
        return FALSE;
      }
      CurMacro->Buffer[CurMacro->BufferSize]=KeyCode;

      if(KeyCode == KEY_MACRODATE)
      {
        memcpy(&CurMacro->Buffer[CurMacro->BufferSize+1],CurKeyText,(Size-1)*sizeof(DWORD));
      }

      CurMacro->BufferSize+=Size;
    }
  }
  return TRUE;
}


// ������� ��������� ������� ������� ������� � �������
// Ret=-1 - �� ������ �������.
// ���� CheckMode=-1 - ������ ������ � ����� ������, �.�. ������ ����������
int KeyMacro::GetIndex(int Key, int ChechMode)
{
  if(MacroPROM)
  {
    int Pos,Len;
    struct MacroRecord *MPtr;
    if(ChechMode == -1)
    {
      Len=MacroPROMCount;
      MPtr=MacroPROM;
    }
    else
    {
      Len=IndexMode[ChechMode][1];
      if(!Len)
       return -1;
      MPtr=MacroPROM+IndexMode[ChechMode][0];
//_SVS(SysLog("ChechMode=%d (%d,%d)",ChechMode,IndexMode[ChechMode][0],IndexMode[ChechMode][1]));
    }
    for(Pos=0; Pos < Len; ++Pos, ++MPtr)
    {
      if (LocalUpper(MPtr->Key)==LocalUpper(Key) && MPtr->BufferSize > 0)
      {
//        && (ChechMode == -1 || (MPtr->Flags&MFLAGS_MODEMASK) == ChechMode))
//_SVS(SysLog("GetIndex: Pos=%d MPtr->Key=0x%08X", Pos,MPtr->Key));
        if(!(MPtr->Flags&MFLAGS_DISABLEMACRO))
          return Pos+((ChechMode >= 0)?IndexMode[ChechMode][0]:0);
      }
    }
  }
  return -1;
}

// ��������� �������, ����������� ��������� ��������
// Ret= 0 - �� ������ �������.
// ���� CheckMode=-1 - ������ ������ � ����� ������, �.�. ������ ����������
int KeyMacro::GetRecordSize(int Key, int CheckMode)
{
  int Pos=GetIndex(Key,CheckMode);
  if(Pos == -1)
    return 0;
  return sizeof(struct MacroRecord)+MacroPROM[Pos].BufferSize;
}

/* $ 21.12.2000 SVS
   ����������� ���.
*/
// �������� �������� ���� �� ����
char* KeyMacro::GetSubKey(int Mode)
{
  return (char *)((Mode >= MACRO_OTHER && Mode < MACRO_LAST)?MKeywords[Mode].Name:"");
}

// �������� ��� ���� �� �����
int KeyMacro::GetSubKey(char *Mode)
{
  int I;
  for(I=MACRO_OTHER; I < MACRO_LAST; ++I)
    if(!stricmp(MKeywords[I].Name,Mode))
      return I;
  return -1;
}

/* $ 20.03.2002 DJ
   ����������� �������� ����� � ��� ��������
*/

BOOL KeyMacro::CheckEditSelected(DWORD CurFlags)
{
  if(Mode==MACRO_EDITOR || Mode==MACRO_DIALOG)
  {
    int NeedType = (Mode == MACRO_EDITOR) ? MODALTYPE_EDITOR : MODALTYPE_DIALOG;
    Frame* CurFrame=FrameManager->GetCurrentFrame();
    if (CurFrame && CurFrame->GetType()==NeedType)
    {
      int CurSelected=CurFrame->ProcessKey(KEY_MEDIT_ISSELECTED);
      if((CurFlags&MFLAGS_SELECTION) && !CurSelected ||
         (CurFlags&MFLAGS_NOSELECTION) && CurSelected)
          return FALSE;
    }
  }
  return TRUE;
}

/* DJ $ */

BOOL KeyMacro::CheckPanel(int PanelMode,DWORD CurFlags)
{
  if(PanelMode == PLUGIN_PANEL && (CurFlags&MFLAGS_NOPLUGINPANELS) ||
     PanelMode == NORMAL_PANEL && (CurFlags&MFLAGS_NOFILEPANELS))
    return FALSE;
  return TRUE;
}

BOOL KeyMacro::CheckCmdLine(int CmdLength,DWORD CurFlags)
{
 if ((CurFlags&MFLAGS_EMPTYCOMMANDLINE) && CmdLength!=0 ||
     (CurFlags&MFLAGS_NOTEMPTYCOMMANDLINE) && CmdLength==0)
      return FALSE;
  return TRUE;
}

BOOL KeyMacro::CheckFileFolder(Panel *ActivePanel,DWORD CurFlags)
{
  char FileName[NM*2];
  int FileAttr=-1;
  ActivePanel->GetFileName(FileName,ActivePanel->GetCurrentPos(),FileAttr);
  if(FileAttr != -1)
  {
    if((FileAttr&FA_DIREC) && (CurFlags&MFLAGS_NOFOLDERS) ||
      !(FileAttr&FA_DIREC) && (CurFlags&MFLAGS_NOFILES))
      return FALSE;
  }
  return TRUE;
}

BOOL KeyMacro::CheckAll(DWORD CurFlags)
{
/* $TODO:
     ����� ������ Check*() ����������� ������� IfCondition()
     ��� ���������� �������������� ����.
*/
  // �������� �� �����/�� ����� � ���.������ (� � ���������? :-)
  if(!CheckCmdLine(CtrlObject->CmdLine->GetLength(),CurFlags))
    return FALSE;

  // �������� ������ � ���� �����
  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  if(ActivePanel!=NULL)// && (CurFlags&MFLAGS_MODEMASK)==MACRO_SHELL)
  {
    if(!CheckPanel(ActivePanel->GetMode(),CurFlags))
      return FALSE;

    if(!CheckFileFolder(ActivePanel,CurFlags))
      return FALSE;

    int SelCount=ActivePanel->GetRealSelCount();
	/* $ 20.03.2002 DJ
	   ��� �������� - �� ������ �������� ���� �� ����
	*/
    if(Mode!=MACRO_EDITOR && Mode != MACRO_DIALOG) // ??? ������ �� ���� �������� !!!
    {
      if((CurFlags&MFLAGS_SELECTION) && SelCount < 1 ||
         (CurFlags&MFLAGS_NOSELECTION) && SelCount >= 1)
        return FALSE;
    }
	/* DJ $ */
  }

  if(!CheckEditSelected(CurFlags))
    return FALSE;

  return TRUE;
}

/*
  Return: FALSE - ���� ����������� MFLAGS_* �� ���������� ���
                  ��� �� ����� ���������� �������!
          TRUE  - ����� ����(�) ����������(�)
*/
BOOL KeyMacro::CheckCurMacroFlags(DWORD Flags)
{
  if(Executing)
  {
    struct MacroRecord *MR=!MacroRAM?MacroPROM+ExecMacroPos:MacroRAM;
    return (MR->Flags&Flags)?TRUE:FALSE;
  }
  return(FALSE);

}

/*
  Return: 0 - �� � ������ �����, 1 - Recording, 2 - Executing
*/
int KeyMacro::GetCurRecord(struct MacroRecord* RBuf,int *KeyPos)
{
  if(KeyPos && RBuf)
  {
    *KeyPos=Executing?ExecKeyPos:0;
    memset(RBuf,0,sizeof(struct MacroRecord));
    if(!Recording)
    {
      if(Executing)
      {
        memcpy(RBuf,MacroPROM+ExecMacroPos,sizeof(struct MacroRecord));
        return 2;
      }
      memset(RBuf,0,sizeof(struct MacroRecord));
      return 0;
    }
    RBuf->BufferSize=RecBufferSize;
    RBuf->Buffer=RecBuffer;
    return 1;
  }
  return Recording?1:(Executing?2:0);
}

static int __cdecl SortMacros(const struct MacroRecord *el1,
                           const struct MacroRecord *el2)
{
  int Mode1, Mode2;
  if((Mode1=(el1->Flags&MFLAGS_MODEMASK)) == (Mode2=(el2->Flags&MFLAGS_MODEMASK)))
    return 0;
  if(Mode1 < Mode2)
    return -1;
  return 1;
}

// ���������� ��������� ������
void KeyMacro::Sort(void)
{
  typedef int (__cdecl *qsort_fn)(const void*,const void*);
  // ���������
  qsort(MacroPROM,
        MacroPROMCount,
        sizeof(struct MacroRecord),
        (qsort_fn)SortMacros);
  // ������������� ������ �����
  struct MacroRecord *MPtr;
  int I,J;
  int CurMode=MACRO_OTHER;
  memset(IndexMode,0,sizeof(IndexMode));
  for(MPtr=MacroPROM,I=0; I < MacroPROMCount; ++I,++MPtr)
  {
    J=MPtr->Flags&MFLAGS_MODEMASK;
    if(CurMode != J)
    {
      IndexMode[J][0]=I;
      CurMode=J;
    }
    IndexMode[J][1]++;
  }

//_SVS(for(I=0; I < sizeof(IndexMode)/sizeof(IndexMode[0]); ++I)SysLog("IndexMode[%02d.%s]=%d,%d",I,GetSubKey(I),IndexMode[I][0],IndexMode[I][1]));
}
#endif // defined(DMACRO2)
