/*
macro.cpp

�������

*/

/* Revision: 1.119 31.05.2004 $ */

/*
Modify:
  31.05.2004 SVS
    ! ������� ����� MCODE_OP_SENDKEY - �������
  26.05.2004 SVS
    ! MkTextSequence() - ������ �������� - ������������� ����� �������
    ! � ReadMacros ������������ �������� ����� �������
    ! � SaveMacros ������ �� ����������, � ������� �������� ����� �������
    ! ������������ ������ ������� ������ � ������ � ������ ������! ��� ����� ���
      ������ ������ ������... ��� ����� $If � ��� � ��� � ���������� �� �������.
  11.05.2004 SVS
    + $IClip, IClip - ������ � ���������� ����������
  14.04.2004 SVS
    - BugZ#1052 - ����������� �������� ������ ����������� ��� �� ������ � MainMenu � Menu
  09.02.2004 SVS
    + MCODE_F_SWITCHKBD ($KbdSwitch) - ������ ����������� ��������� ����������
  25.12.2003 SVS
    - ������� ���� ��� ��������� ������������� ������� - �� ���������. Src
  20.12.2003 IS
    - ������� ��� ���������� ������ ��� ����������� ������� ����� ����������� ���������
      (Ctrl-., a Ctrl-., Ctrl-~, Ctrl-~ -> �������)
  15.12.2003 SVS
    ! ��������� MacroRecord ��������� � ������� (����� �� �������)
  14.11.2003 SVS
    + ��������� ��� Alt-F4 � ������� ��������� �������� - �.�. �������������� �����������
  28.10.2003 SVS
    ! ���� ���� Work.MacroWORK ���� ��� ���.
    ! ��� �� ������ �������� ������ � WORK-�������, ������ ��� ���� � ����.
      ��� ��, ���� � ����� ��������� ���� ���-�� - �� ������� ��� ������
      ��� ��, ���� ������ ����������, �� ������������� ������ (���� ����)
      ���������� ��������� (�� ����� ���������)
    + MCODE_C_?PANEL_ROOT
    ! Executing -> MacroState.Executing
    ! ��������� ��������� �������� �� ������������ $If
  20.10.2003 SVS
    ! ��������������
        KEY_MACRO_EDITSELECTED -> KEY_MACRO_SELECTED
        KEY_MACRO_CHECKEOF     -> KEY_MACRO_EOF
    + ��������� KEY_MACRO_EMPTY � KEY_MACRO_BOF
    ! �������� ��������� ���������, � ����������� �� ���������
      ���������� ������� - Bof, Empty, Eof, Selected
  15.10.2003 SVS
    + GetMacroKeyInfo - ���������� �� ��������� ������������.
    + ����������/�������������� ��������������.
  13.10.2003 SVS
    ! ��������������:
      Opt.KeyMacroRecord1  -> Opt.KeyMacroCtrlDot
      Opt.KeyMacroRecord2  -> Opt.KeyMacroCtrlShiftDot
    - ������� �������� ������ ������� ����� ��������� ������� ���������� - ����� ���������
  10.10.2003 SVS
    + ��������� �������� ������� ��������� � ���.������ ��� "Selected"
    - ������ ��� ������������ ������� - �������� $Else
  04.10.2003 SVS
    ! ���� ��������� - ��� �������� ��. 01715.Macro.txt
  26.09.2003 SVS
    ! ��������������
      GetMacroPlainText        -> GetPlainText
      GetMacroPlainTextSize    -> GetPlainTextSize
    ! ��������� � ��������� �����������
  22.09.2003 SVS
    + KeyMacro::KeyToBuffer() - ��������� ��� � �����
    ! KeyMacro::IfCondition() - ���������� int
  12.09.2003 SVS
    + ��������� ������� KeyMacro::GetMacroPlainTextSize() - ������ plain-text
  09.09.2003 SVS
    - ������������ ������ ��� ������� ������� � ������� KeyMacro::ParseMacroString
      ������� CurKeyText ������������
  08.09.2003 SVS
    ! ��������� $Date
    + ��������� KEY_MACRO_PLAINTEXT - ����� �������� $Text - ������� ������.
  25.08.2003 SVS
    ! �� SendKeysToPlugins, �� NoSendKeysToPlugins, ����� ������ �� ����������.
    - BugZ#903 - ������������� ����������� ��������� Title Far'a ��� ��������
  29.07.2003 SVS
    ! ��������� ������ ������ ��������.
    ! ������ InsidePlugin � NoInsidePlugin ������ SendKeysToPlugins - ���������� �������� ������� ������ �� �������������� �������.
    ! �� CtrlShift. - ���������� ������ _�� ���������_ ������� ��������, ��� ��
      ����������� ������ ���������
      �� Ctrl. - ���������� ������ _���������_ ������� ��������, ����� ���������.
    + ���� � �������� �������� MACROMODE_*
  15.07.2003 SVS
    + ����� ����� � ���������� ������� - "Enable when plugin active"
    + KeyMacro::CheckInsidePlugin() - "�� ������ �������?"
    + KeyMacro::DropProcess() - �������� ������� ����������� ������.
  21.05.2003 SVS
    - BugZ#900 - ACTL_POSTKEYSEQUENCE ��������� �� �������
  02.05.2003 SVS
    - BugZ#790 - �������������� ������� ����� ����� ��������� ��� ����������?
    - BugZ#873 - ACTL_POSTKEYSEQUENCE � ��������� ����
    + IsExecutingLastKey() - ������ �������� ��... "��� ��������� ������� ������������?"
  14.04.2003 SVS
    - ���������� ��������� post`��
  17.03.2003 SVS
    ! �������� ����� ����� FFPOL_*
    - � ������� ���������� ������ ��������� Ctrl-F5 � �������� ������������.
    + ������� KEY_NONE ��� "���������" ��������. �.�. ������ ���� ����� ��� � �
      "������" ��������, �.�. �� ��� ��������� �������, � ��� �� _�����_ ���������
      ("���������" � �������� ����, ��� ��� ����� ������)
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  13.12.2002 SVS
    - BugZ#699 - ����������� ���� ��� ������������� ACTL_POSTKEYSEQUENCE
      ������ ������ ��� �������� :-(
  10.12.2002 SVS
    - BugZ#699 - ����������� ���� ��� ������������� ACTL_POSTKEYSEQUENCE
      ����������� ������ - ��������� KSFLAGS_DISABLEOUTPUT
  03.12.2002 SVS
    - BugZ#699 - ����������� ���� ��� ������������� ACTL_POSTKEYSEQUENCE
  09.11.2002 SVS
    ! � ����� � ���������� "ReturnAltValue" ������ � �������� �������
      AltXXXXX - �.�. ��, ��� ���� ������� ��� Alt-Num. ��� ����, �����
      �� �������������� ��� ������� � ���������!
  23.08.2002 SVS
    - ����� ������� *Buffer* ������ ��������� ��� �� NULL, �.�.
      ������� ������� ������� ��������� �� ������� �����
  21.08.2002 SVS
    - �������� � ����������� ��� VC
  19.08.2002 SVS
    ! ��� ��������� � ������� ���������� ������� Executing ����� ��
      ������������ (��� ������������� ������ ��� ���������� �������).
      ������ ���������� - ���� Executing==TRUE, ���� � ������� ���� �����.
    ! ���� BufferSize == 1, �� ������ ��� ������������������ �� ����������,
      � ��� ��������� Buffer ������������ ��� ���������� ���� DWORD, ���
      ��������� ��������� ������������ ��������� ������� �� ��������� ������.
    + KeyMacro::KeyFromBuffer() - ����� �� ��� ��������� ������ ������� ��
      ������
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
    + KEY_MACRO_XLAT - � �������� "$XLat" ���������� �� ������� ������ XLat
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


enum MACRO_OP_CODE {
  // 1 ������� ���������
  MCODE_OP_JMP=0x80000000,       // Jump, ���������� �����
  MCODE_OP_EXIT=KEY_MACROSPEC_BASE,// ������������� ��������� ���������� �����������������������
  MCODE_OP_MACROMODE,            // ������� ����� ���������� ������ �� �����
  MCODE_OP_REP,                  // $rep - ������� ������ �����
  MCODE_OP_ENDREP,               // $endrep - ������� ����� �����
  MCODE_OP_IF,                   // $if ...
  MCODE_OP_IFTHEN,               // ... $then ...
  MCODE_OP_ELSE,                 // ... $else ...
  MCODE_OP_ENDIF,                // ... $endif

  // �������
  MCODE_F_XLAT,                  // �������� XLat: Param=0 ��� 1
  MCODE_F_SWITCHKBD,             // ����������� ��������� ����������
  MCODE_F_ICLIP,                 // ���������� ��� ������� ��������

  // ������� �������
  MCODE_C_DISABLEOUTPUT,         // ����� ��������?
  MCODE_C_WINDOWEDMODE,          // ������� �����?
  MCODE_C_SELECTED,              // ���������� ���� ����?
  MCODE_C_EOF,                   // ����� �����/��������� ��������?
  MCODE_C_BOF,                   // ������ �����/��������� ��������?
  MCODE_C_ISEMPTY,               // ���.������ �����?
  MCODE_C_ICLIP,                 // ���������� ��� ������� ��������

  MCODE_C_APANEL_ISEMPTY,        // �������� ������:  �����?
  MCODE_C_PPANEL_ISEMPTY,        // ��������� ������: �����?
  MCODE_C_APANEL_BOF,            // ������ ���������  ��������?
  MCODE_C_PPANEL_BOF,            // ������ ���������� ��������?
  MCODE_C_APANEL_EOF,            // ����� ���������  ��������?
  MCODE_C_PPANEL_EOF,            // ����� ���������� ��������?
  MCODE_C_APANEL_ROOT,           // ��� �������� ������� �������� ������?
  MCODE_C_PPANEL_ROOT,           // ��� �������� ������� ��������� ������?
  MCODE_C_APANEL_VISIBLE,        // �������� ������:  ������?
  MCODE_C_PPANEL_VISIBLE,        // ��������� ������: ������?
  MCODE_C_APANEL_PLUGIN,         // �������� ������:  ����������?
  MCODE_C_PPANEL_PLUGIN,         // ��������� ������: ����������?
  MCODE_C_APANEL_FOLDER,         // �������� ������:  ������� ������� �������?
  MCODE_C_PPANEL_FOLDER,         // ��������� ������: ������� ������� �������?
  MCODE_C_APANEL_SELECTED,       // �������� ������:  ���������� �������� ����?
  MCODE_C_PPANEL_SELECTED,       // ��������� ������: ���������� �������� ����?
  MCODE_C_APANEL_LEFT,           // �������� ������ �����?
  MCODE_C_PPANEL_LEFT,           // ��������� ������ �����?
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
  // ��� ������ ������ ���� � ������ �������!
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

  // ������
  {2,  "Bof",                MCODE_C_BOF,0},
  {2,  "Eof",                MCODE_C_EOF,0},
  {2,  "Empty",              MCODE_C_ISEMPTY,0},
  {2,  "DisableOutput",      MCODE_C_DISABLEOUTPUT,0},
  {2,  "Selected",           MCODE_C_SELECTED,0},
  {2,  "IClip",              MCODE_C_ICLIP,0},

  {2,  "APanel.Empty",       MCODE_C_APANEL_ISEMPTY,0},
  {2,  "PPanel.Empty",       MCODE_C_PPANEL_ISEMPTY,0},
  {2,  "APanel.Bof",         MCODE_C_APANEL_BOF,0},
  {2,  "PPanel.Bof",         MCODE_C_PPANEL_BOF,0},
  {2,  "APanel.Eof",         MCODE_C_APANEL_EOF,0},
  {2,  "PPanel.Eof",         MCODE_C_PPANEL_EOF,0},
  {2,  "APanel.Root",        MCODE_C_APANEL_ROOT,0},
  {2,  "PPanel.Root",        MCODE_C_PPANEL_ROOT,0},
  {2,  "APanel.Visible",     MCODE_C_APANEL_VISIBLE,0},
  {2,  "PPanel.Visible",     MCODE_C_PPANEL_VISIBLE,0},
  {2,  "APanel.Plugin",      MCODE_C_APANEL_PLUGIN,0},
  {2,  "PPanel.Plugin",      MCODE_C_PPANEL_PLUGIN,0},
  {2,  "APanel.Folder",      MCODE_C_APANEL_FOLDER,0},
  {2,  "PPanel.Folder",      MCODE_C_PPANEL_FOLDER,0},
  {2,  "APanel.Selected",    MCODE_C_APANEL_SELECTED,0},
  {2,  "PPanel.Selected",    MCODE_C_PPANEL_SELECTED,0},
  {2,  "APanel.Left",        MCODE_C_APANEL_LEFT,0},
  {2,  "PPanel.Left",        MCODE_C_PPANEL_LEFT,0},

  {2,  "Windowed",           MCODE_C_WINDOWEDMODE,0},
},

MKeywordsFlags[] ={
  // �����
  {1,  "DisableOutput",      MFLAGS_DISABLEOUTPUT,0},
  {1,  "RunAfterFARStart",   MFLAGS_RUNAFTERFARSTART,0},
  {1,  "EmptyCommandLine",   MFLAGS_EMPTYCOMMANDLINE,0},
  {1,  "NotEmptyCommandLine",MFLAGS_NOTEMPTYCOMMANDLINE,0},
  {1,  "EVSelection",        MFLAGS_EDITSELECTION,0},
  {1,  "NoEVSelection",      MFLAGS_EDITNOSELECTION,0},

  {1,  "NoFilePanels",       MFLAGS_NOFILEPANELS,0},
  {1,  "NoPluginPanels",     MFLAGS_NOPLUGINPANELS,0},
  {1,  "NoFolders",          MFLAGS_NOFOLDERS,0},
  {1,  "NoFiles",            MFLAGS_NOFILES,0},
  {1,  "Selection",          MFLAGS_SELECTION,0},
  {1,  "NoSelection",        MFLAGS_NOSELECTION,0},

  {1,  "NoFilePPanels",      MFLAGS_PNOFILEPANELS,0},
  {1,  "NoPluginPPanels",    MFLAGS_PNOPLUGINPANELS,0},
  {1,  "NoPFolders",         MFLAGS_PNOFOLDERS,0},
  {1,  "NoPFiles",           MFLAGS_PNOFILES,0},
  {1,  "PSelection",         MFLAGS_PSELECTION,0},
  {1,  "NoPSelection",       MFLAGS_PNOSELECTION,0},

  {1,  "ReuseMacro",         MFLAGS_REUSEMACRO,0},
  {1,  "NoSendKeysToPlugins",MFLAGS_NOSENDKEYSTOPLUGINS,0},
};


// ������������� ������� - ��� <-> ��� ������������
static struct TKeyCodeName{
  int Key;
  int Len;
  char *Name;
} KeyMacroCodes[]={
#if defined(MOUSEKEY)
   { KEY_MACROSELWORD,              8, "$SelWord" },
#endif
   { MCODE_F_SWITCHKBD,            10, "$KbdSwitch"},
   { MCODE_OP_ENDREP,               7, "$EndRep"  },
   { MCODE_OP_ENDIF,                6, "$EndIf"   },
   { MCODE_F_ICLIP,                 6, "$IClip"   },
   { MCODE_OP_MACROMODE,            6, "$MMode"   },
   { KEY_MACRO_DATE,                5, "$Date"    }, // $Date "%d-%a-%Y"
   { MCODE_OP_ELSE,                 5, "$Else"    },
   { MCODE_OP_EXIT,                 5, "$Exit"    },
   { KEY_MACRO_PLAINTEXT,           5, "$Text"    }, // $Text "Plain Text"
   { MCODE_OP_IFTHEN,               5, "$Then"    },
   { KEY_MACRO_XLAT,                5, "$XLat"    },
   { MCODE_OP_REP,                  4, "$Rep"     },
   { MCODE_OP_IF,                   3, "$If"      },
};

static char __code2symbol(BYTE b1, BYTE b2);
static const char* ParsePlainText(char *CurKeyText, const char *BufPtr);
static const char *__GetNextWord(const char *BufPtr,char *CurKeyText);


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
  MacroVersion=GetRegKey("KeyMacros","MacroVersion",0);
  CurPCStack=-1;
  Work.MacroWORKCount=0;
  Work.MacroWORK=NULL;
  LockScr=NULL;
  MacroLIB=NULL;
  RecBuffer=NULL;
  Mode=MACRO_SHELL;
  LoadMacros();
}

KeyMacro::~KeyMacro()
{
  _OT(SysLog("[%p] KeyMacro::~KeyMacro()", this));
  InitInternalVars();
}

void KeyMacro::InitInternalLIBVars()
{
  if(MacroLIB)
  {
    for (int I=0;I<MacroLIBCount;I++)
    {
      if(MacroLIB[I].BufferSize > 1 && MacroLIB[I].Buffer)
        xf_free(MacroLIB[I].Buffer);
      if(MacroLIB[I].Src)
        xf_free(MacroLIB[I].Src);
    }
    xf_free(MacroLIB);
  }
  if(RecBuffer) xf_free(RecBuffer);
  MacroLIBCount=0;
  MacroLIB=NULL;
}

// ������������� ���� ����������
void KeyMacro::InitInternalVars(BOOL InitedRAM)
{
  InitInternalLIBVars();

  if(LockScr)
  {
    delete LockScr;
    LockScr=NULL;
  }

  if(InitedRAM)
  {
    ReleaseWORKBuffer(TRUE);
    Work.Executing=MACROMODE_NOMACRO;
  }

  RecBuffer=NULL;
  RecBufferSize=0;

  Recording=MACROMODE_NOMACRO;
  InternalInput=FALSE;
}

// �������� ���������� ������, ���� �� ���������� �����������
// (����������� - ������ � PlayMacros �������� ������.
void KeyMacro::ReleaseWORKBuffer(BOOL All)
{
  if(Work.MacroWORK)
  {
    if(All || Work.MacroWORKCount <= 1)
    {
      for (int I=0;I<Work.MacroWORKCount;I++)
      {
        if(Work.MacroWORK[I].BufferSize > 1 && Work.MacroWORK[I].Buffer)
          xf_free(Work.MacroWORK[I].Buffer);
        if(Work.MacroWORK[I].Src)
          xf_free(Work.MacroWORK[I].Src);
      }
      xf_free(Work.MacroWORK);
      Work.MacroWORK=NULL;
      Work.MacroWORKCount=0;
    }
    else
    {
      if(Work.MacroWORK->BufferSize > 1 && Work.MacroWORK->Buffer)
        xf_free(Work.MacroWORK->Buffer);
      if(Work.MacroWORK->Src)
        xf_free(Work.MacroWORK->Src);
      Work.MacroWORKCount--;

      memmove(Work.MacroWORK,((BYTE*)Work.MacroWORK)+sizeof(struct MacroRecord),sizeof(struct MacroRecord)*Work.MacroWORKCount);
      xf_realloc(Work.MacroWORK,sizeof(struct MacroRecord)*Work.MacroWORKCount);
    }
  }
}

// �������� ���� �������� �� �������
int KeyMacro::LoadMacros(BOOL InitedRAM)
{
  int Ret=FALSE;
  InitInternalVars(InitedRAM);

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
//  Mode=MACRO_SHELL;
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
    if (Key==Opt.KeyMacroCtrlDot || Key==Opt.KeyMacroCtrlShiftDot) // ������� ����� ������?
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

      // ���������� ����� �� ���������.
      DWORD Flags=MFLAGS_DISABLEOUTPUT; // ???

      // ������� �������� �� ��������
      // ���� �������, �� �� ����� �������� ������ ���������.
      //if (MacroKey != (DWORD)-1 && (Key==KEY_CTRLSHIFTDOT || Recording==2) && RecBufferSize)
      if (MacroKey != (DWORD)-1 && Key==Opt.KeyMacroCtrlShiftDot && RecBufferSize)
      {
        if (!GetMacroSettings(MacroKey,Flags))
          MacroKey=(DWORD)-1;
      }
      WaitInMainLoop=WaitInMainLoop0;
      InternalInput=FALSE;

      if (MacroKey==(DWORD)-1)
      {
        if(RecBuffer)  xf_free(RecBuffer);
      }
      else
      {
        int Pos;
        for (Pos=0;Pos<MacroLIBCount;Pos++)
          if (MacroLIB[Pos].Key==MacroKey && (MacroLIB[Pos].Flags&MFLAGS_MODEMASK)==StartMode)
            break;
        if (Pos==MacroLIBCount)
        {
          MacroLIB=(struct MacroRecord *)xf_realloc(MacroLIB,sizeof(*MacroLIB)*(MacroLIBCount+1));
          if (MacroLIB==NULL)
          {
            MacroLIBCount=0;
            WaitInFastFind++;
            return(FALSE);
          }
          MacroLIBCount++;
        }
        else
        {
          if(MacroLIB[Pos].BufferSize > 1 && MacroLIB[Pos].Buffer)
            xf_free(MacroLIB[Pos].Buffer);
          if(MacroLIB[Pos].Src)
            xf_free(MacroLIB[Pos].Src);
        }
        MacroLIB[Pos].Key=MacroKey;
        if(RecBufferSize > 1)
          MacroLIB[Pos].Buffer=RecBuffer;
        else if(RecBuffer)
          MacroLIB[Pos].Buffer=reinterpret_cast<DWORD*>(*RecBuffer);
        MacroLIB[Pos].BufferSize=RecBufferSize;
        MacroLIB[Pos].Src=MkTextSequence(MacroLIB[Pos].Buffer,MacroLIB[Pos].BufferSize);
        MacroLIB[Pos].Flags=Flags|(StartMode&MFLAGS_MODEMASK)|MFLAGS_NEEDSAVEMACRO|(Recording==MACROMODE_RECORDING_COMMON?0:MFLAGS_NOSENDKEYSTOPLUGINS);
      }

      Recording=MACROMODE_NOMACRO;
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

      RecBuffer=(DWORD *)xf_realloc(RecBuffer,sizeof(*RecBuffer)*(RecBufferSize+1));
      if (RecBuffer==NULL)
        return(FALSE);

      if(ReturnAltValue) // "����������" ������ ;-)
        Key|=KEY_ALTDIGIT;

      RecBuffer[RecBufferSize++]=Key;
      return(FALSE);
    }
  }
  else if (Key==Opt.KeyMacroCtrlDot || Key==Opt.KeyMacroCtrlShiftDot) // ������ ������?
  {
    // ������� 18
    if(Opt.Policies.DisabledOptions&FFPOL_CREATEMACRO)
      return FALSE;
//    if(CtrlObject->Plugins.CheckFlags(PSIF_ENTERTOOPENPLUGIN))
//      return FALSE;

    if(LockScr) delete LockScr;
    LockScr=NULL;

    // ��� ��?
    StartMode=(Mode==MACRO_SHELL && !WaitInMainLoop)?MACRO_OTHER:Mode;
    // ��� ������ - � ������� ������� �������� ���...
    // � ����������� �� ����, ��� ������ ������ ������, ��������� ����� ����� (Ctrl-.
    // � ��������� ������� ����) ��� ����������� (Ctrl-Shift-. - ��� �������� ������ �������)
    Recording=(Key==Opt.KeyMacroCtrlDot) ? MACROMODE_RECORDING_COMMON:MACROMODE_RECORDING;

    if(RecBuffer)
      xf_free(RecBuffer);

    RecBuffer=NULL;
    RecBufferSize=0;
    ScrBuf.ResetShadow();
    ScrBuf.Flush();
    WaitInFastFind--;
    return(TRUE);
  }
  else
  {
    if (Work.Executing == MACROMODE_NOMACRO) // ��� ��� �� ����� ����������?
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
      int I=GetIndex(Key,(Mode==MACRO_SHELL && !WaitInMainLoop) ? MACRO_OTHER:Mode);
      if(I != -1 && !((CurFlags=MacroLIB[I].Flags)&MFLAGS_DISABLEMACRO) && CtrlObject)
      {
//_SVS(SysLog("KeyMacro: %d (I=%d Key=%s,%s)",__LINE__,I,_FARKEY_ToName(Key),_FARKEY_ToName(MacroLIB[I].Key)));
        if(!CheckAll(Mode,CurFlags))
          return FALSE;

        // ��������� ������� ���������� � MacroWORK
        //PostNewMacro(MacroLIB+I);
        // ��������� �����?
        if (CurFlags&MFLAGS_DISABLEOUTPUT)
        {
          if(LockScr) delete LockScr;
          LockScr=new LockScreen;
        }

        // ��������� ����� ����� (� ��������� ������� ����) ��� ����������� (��� �������� ������ �������)
        Work.ExecLIBPos=0;
        PostNewMacro(MacroLIB+I);
        Work.MacroPC=I;

        IsRedrawEditor=CtrlObject->Plugins.CheckFlags(PSIF_ENTERTOOPENPLUGIN)?FALSE:TRUE;

        _KEYMACRO(SysLog("**** Start Of Execute Macro ****"));
        _KEYMACRO(SysLog(1));
        return(TRUE);
      }
    }
    return(FALSE);
  }
}

char *KeyMacro::GetPlainText(char *Dest)
{
  if(!Work.MacroWORK)
    return NULL;

  struct MacroRecord *MR=Work.MacroWORK;

  int LenTextBuf=strlen((char*)&MR->Buffer[Work.ExecLIBPos]);
  Dest[0]=0;
  if(LenTextBuf && MR->Buffer[Work.ExecLIBPos])
  {
    strcpy(Dest,(char *)&MR->Buffer[Work.ExecLIBPos]);
    Work.ExecLIBPos+=(LenTextBuf+1)/4;
    if(((LenTextBuf+1)%sizeof(DWORD)) != 0)
      ++Work.ExecLIBPos;
    return Dest;
  }
  else
    Work.ExecLIBPos++;
  return NULL;
}

int KeyMacro::GetPlainTextSize()
{
  if(!Work.MacroWORK)
    return 0;
  struct MacroRecord *MR=Work.MacroWORK;
  return strlen((char*)&MR->Buffer[Work.ExecLIBPos]);
}

int KeyMacro::IfCondition(DWORD OpCode,DWORD Flags,DWORD CheckCode)
{
  int I;
  BOOL Cond=0;
  char FileName[NM*2];
  int FileAttr=-1;

  // ������ ������ ������� ��������
  for(I=0; I < sizeof(MKeywords)/sizeof(MKeywords[0]); ++I)
    if(MKeywords[I].Value == CheckCode)
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
      if(WaitInMainLoop) // ����� ���� ������ ��� ����� WaitInMainLoop, ���� ���� � ���������!!!
        Cond=CheckCode == FrameManager->GetCurrentFrame()->GetMacroMode()?1:0;
      else
        Cond=CheckCode == CtrlObject->Macro.GetMode()?1:0;
      break;
    }

    case 2:
    {
      Panel *PassivePanel=NULL;
      if(ActivePanel!=NULL)
        PassivePanel=CtrlObject->Cp()->GetAnotherPanel(ActivePanel);
      Frame* CurFrame=FrameManager->GetCurrentFrame();

      switch(CheckCode)
      {
        case MCODE_C_DISABLEOUTPUT: // DisableOutput?
          Cond=Flags&CheckCode?1:0;
          break;

        case MCODE_C_WINDOWEDMODE: // Windowed?
          Cond=FarAltEnter(-2)==0?1:0;
          break;

        case MCODE_C_ICLIP:
          Cond=UsedInternalClipboard;
          break;

        case MCODE_C_BOF:
        case MCODE_C_EOF:
        {
          if(CurFrame)
          {
            Cond=CurFrame->ProcessKey(CheckCode==MCODE_C_BOF?KEY_MACRO_BOF:KEY_MACRO_EOF)?1:0;
          }
          break;
        }

        case MCODE_C_APANEL_ROOT:  // APanel.Root
        case MCODE_C_PPANEL_ROOT:  // PPanel.Root
        {
          Panel *SelPanel=(CheckCode==MCODE_C_APANEL_ROOT)?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
            Cond=SelPanel->ProcessKey(KEY_MACRO_ROOTFOLDER)?1:0;
          break;
        }

        case MCODE_C_APANEL_BOF:
        case MCODE_C_PPANEL_BOF:
        case MCODE_C_APANEL_EOF:
        case MCODE_C_PPANEL_EOF:
        {
          Panel *SelPanel=(CheckCode==MCODE_C_APANEL_BOF || CheckCode==MCODE_C_APANEL_EOF)?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
            Cond=SelPanel->ProcessKey(CheckCode==MCODE_C_APANEL_BOF || CheckCode==MCODE_C_PPANEL_BOF?KEY_MACRO_BOF:KEY_MACRO_EOF)?1:0;
          break;
        }

        case MCODE_C_SELECTED:    // Selected?
        {
          int NeedType = Mode == MACRO_EDITOR?MODALTYPE_EDITOR:(Mode == MACRO_VIEWER?MODALTYPE_VIEWER:(Mode == MACRO_DIALOG?MODALTYPE_DIALOG:MACRO_SHELL));
          if (CurFrame && CurFrame->GetType()==NeedType)
          {
            int CurSelected;
            if(Mode==MACRO_SHELL && CtrlObject->CmdLine->IsVisible())
              CurSelected=CtrlObject->CmdLine->ProcessKey(KEY_MACRO_SELECTED);
            else
              CurSelected=CurFrame->ProcessKey(KEY_MACRO_SELECTED);
            Cond=CurSelected?1:0;
          }
          break;
        }

        case MCODE_C_ISEMPTY:   // Empty
          if(CurFrame->GetType() == MACRO_SHELL)
            Cond=CtrlObject->CmdLine->GetLength()==0;
          else
            Cond=CurFrame->ProcessKey(KEY_MACRO_EMPTY);
          break;

        case MCODE_C_APANEL_VISIBLE:  // APanel.Visible
        case MCODE_C_PPANEL_VISIBLE:  // PPanel.Visible
        {
          Panel *SelPanel=CheckCode==MCODE_C_APANEL_VISIBLE?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
            Cond=SelPanel->IsVisible();
          break;
        }

        case MCODE_C_APANEL_ISEMPTY: // APanel.Empty
        case MCODE_C_PPANEL_ISEMPTY: // PPanel.Empty
        {
          Panel *SelPanel=CheckCode==MCODE_C_APANEL_ISEMPTY?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
          {
            SelPanel->GetFileName(FileName,SelPanel->GetCurrentPos(),FileAttr);
            int GetFileCount=SelPanel->GetFileCount();
            Cond=GetFileCount == 0 ||
                 GetFileCount == 1 && TestParentFolderName(FileName)
                 ?1:0;
          }
          break;
        }

        case MCODE_C_APANEL_LEFT: // APanel.Left
        {
          Cond=ActivePanel == CtrlObject->Cp()->LeftPanel?1:0;
          break;
        }

        case MCODE_C_PPANEL_LEFT: // PPanel.Left
        {
          Cond=PassivePanel == CtrlObject->Cp()->LeftPanel?1:0;
          break;
        }

        case MCODE_C_APANEL_PLUGIN: // APanel.Plugin
        case MCODE_C_PPANEL_PLUGIN: // PPanel.Plugin
        {
          Panel *SelPanel=CheckCode==MCODE_C_APANEL_PLUGIN?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
            Cond=SelPanel->GetMode() == PLUGIN_PANEL;
          break;
        }

        case MCODE_C_APANEL_FOLDER: // APanel.Folder
        case MCODE_C_PPANEL_FOLDER: // PPanel.Folder
        {
          Panel *SelPanel=CheckCode==MCODE_C_APANEL_FOLDER?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
          {
            SelPanel->GetFileName(FileName,SelPanel->GetCurrentPos(),FileAttr);
            if(FileAttr != -1)
              Cond=(FileAttr&FA_DIREC)?1:0;
          }
          break;
        }

        case MCODE_C_APANEL_SELECTED: // APanel.Selected
        case MCODE_C_PPANEL_SELECTED: // PPanel.Selected
        {
          Panel *SelPanel=CheckCode==MCODE_C_APANEL_SELECTED?ActivePanel:PassivePanel;
          if(SelPanel!=NULL)
          {
            int SelCount=SelPanel->GetRealSelCount();
            Cond=SelCount >= 1; //??
          }
          break;
        }
        // *****************
      }
    }
  }

  // ������, ��������� ������� - ��� ��� �� ���
  if(OpCode == MCODE_OP_IF && Cond) //$If Flag|Area
    return 1;

  return 0;
}


// �������� ��������� ��� ������� �� �������
int KeyMacro::GetKey()
{
  struct MacroRecord *MR;
//_SVS(SysLog(">KeyMacro::GetKey() InternalInput=%d Executing=%d (%p)",InternalInput,Work.Executing,FrameManager->GetCurrentFrame()));
  if (InternalInput || !FrameManager->GetCurrentFrame())
    return(FALSE);

  if(Work.Executing == MACROMODE_NOMACRO)
  {
    if(!Work.MacroWORK)
    {
      if(CurPCStack >= 0)
      {
        PopState();
        return(FALSE);
      }
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
      if(CurPCStack < 0)
      {
        if(LockScr) delete LockScr;
        LockScr=NULL;
      }
      if(TitleModified) SetFarTitle(NULL);
      UsedInternalClipboard=0; //??
      return(FALSE);
    }
/*
    else if(Work.ExecLIBPos>=MR->BufferSize)
    {
      ReleaseWORKBuffer();
      Work.Executing=MACROMODE_NOMACRO;
      return(FALSE);
    }
    else
*/
    //if(Work.MacroWORK)
    {
      Work.Executing=Work.MacroWORK->Flags&MFLAGS_NOSENDKEYSTOPLUGINS?MACROMODE_EXECUTING:MACROMODE_EXECUTING_COMMON;
      Work.ExecLIBPos=0; //?????????????????????????????????
    }
    //else
    //  return FALSE;
  }

initial:
  if((MR=Work.MacroWORK) == NULL)
    return FALSE;
//_SVS(SysLog("KeyMacro::GetKey() initial: Work.ExecLIBPos=%d (%d) %p",Work.ExecLIBPos,MR->BufferSize,Work.MacroWORK));

  // ��������! �������� �����!
  if(!Work.ExecLIBPos && !LockScr && (MR->Flags&MFLAGS_DISABLEOUTPUT))
    LockScr=new LockScreen;

begin:
  if (Work.ExecLIBPos>=MR->BufferSize || MR->Buffer==NULL)
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
    if(CurPCStack < 0)
    {
      if(LockScr) delete LockScr;
      LockScr=NULL;
    }
    UsedInternalClipboard=0; //??
    Work.Executing=MACROMODE_NOMACRO;
    ReleaseWORKBuffer();
    // �������� - "� ���� �� � ��������� ����� ��� �������"?
    if(Work.MacroWORKCount > 0)
    {
      // �������, �������� ��������� �� �����
      Work.ExecLIBPos=0;
    }
    if(TitleModified) SetFarTitle(NULL); // �������� ������ ��������� �� ���������� �������
    //FrameManager->RefreshFrame();
    //FrameManager->PluginCommit();
    _KEYMACRO(SysLog(-1));
    _KEYMACRO(SysLog("**** End Of Execute Macro ****"));
    if(CurPCStack >= 0)
    {
      PopState();
      goto initial;
    }
    return(FALSE);
  }

  if(Work.ExecLIBPos==0)
    Work.Executing=Work.MacroWORK->Flags&MFLAGS_NOSENDKEYSTOPLUGINS?MACROMODE_EXECUTING:MACROMODE_EXECUTING_COMMON;

  DWORD Key=GetOpCode(MR,Work.ExecLIBPos++);
  _SVS(char KeyText[50]);
  _SVS(::KeyToText(Key,KeyText));
  _SVS(SysLog("%s",KeyText));

  if(Key&KEY_ALTDIGIT) // "����������" ������ ;-)
  {
    Key&=~KEY_ALTDIGIT;
    ReturnAltValue=1;
  }

  if(Key&MCODE_OP_JMP)
  {
    Work.ExecLIBPos=Key&(~MCODE_OP_JMP); // ������ ����������� �������...
    goto begin;
  }

  switch(Key)
  {
    /* $IClip
       0: MCODE_F_ICLIP
    */
    case MCODE_F_ICLIP:
    {
      UsedInternalClipboard=UsedInternalClipboard==0?1:0;
      goto begin;
    }

    case MCODE_F_SWITCHKBD:
    {
      if(hFarWnd)
      {
        PostMessage(hFarWnd,WM_INPUTLANGCHANGEREQUEST, 1, HKL_NEXT);
        //if(Flags & XLAT_SWITCHKEYBBEEP)
        //  MessageBeep(0);
      }
      goto begin;
    }

    /* $Rep Count Seq $EndRep

               0: MCODE_OP_REP
               1: CounterOrig
               2: MCODE_OP_JMP | idxEndRep
        idxSeq+0: seq
                  ...
    */
    case MCODE_OP_REP:
    {
      // ������� ������������ �������� ��������...
      int Counter=GetOpCode(MR,Work.ExecLIBPos);
      // ������� ������ �������� �� idxEndRep
      int idxEndRep=GetOpCode(MR,Work.ExecLIBPos+1)&(~MCODE_OP_JMP);
      // � ������� ��� � ������� �����
      SetOpCode(MR,idxEndRep+1,Counter);
      Work.ExecLIBPos=idxEndRep; // ������ ����������� ������� �������� idxEndRep, ��� ������ �� $RepEnd
                            // � �����, ����� ����� ����� ������� "goto begin;", �� �� �������� ���� ��� -
      goto begin;           // ��������� MCODE_OP_JMP
    }

    /* $Rep Count Seq $EndRep
                  ...
                  seq
                  ...
     idxEndRep+0: MCODE_OP_ENDREP
     idxEndRep+1: Counter
     idxEndRep+2: MCODE_OP_JMP | idxSeq

    */
    case MCODE_OP_ENDREP:
    {
      int Counter=(int)GetOpCode(MR,Work.ExecLIBPos); // Work.ExecLIBPos ��������� �� �������
      Counter--;                                     // ��������...
      SetOpCode(MR,Work.ExecLIBPos,Counter);            // ...� ������� ������� �������, ��� ��������� ��������
      Work.ExecLIBPos++;                                  // Work.ExecLIBPos ��������� �� ����� (������) idxSeq
      if(Counter < 0)                                // ���� ����������� ����?
        Work.ExecLIBPos++;                                // ��������� ������� �� $RepEnd (��������� "MCODE_OP_JMP | idxSeq")
      goto begin;
    }

    /* $If [!]Condition $Then Seq_then [$Else Seq_else] $EndIf
               0: MCODE_OP_IF
               1: Condition
               2: Not                  ���� ���������� '!', �� ����� 1, ����� 0
               3: MCODE_OP_JMP | idxElse
               4: MCODE_OP_IFTHEN
               5: Seq_then
                  ...
     [
               N: MCODE_OP_JMP | idxEndIf
         idxElse: MCODE_OP_ELSE                ���� ����� ����� � �� ����,
       idxElse+2: Seq_else                     �.�. �� MCODE_OP_ELSE ����� �����
                  ...                          ��������� MCODE_OP_ENDIF
     ]
        idxEndIf: MCODE_OP_ENDIF
    */
    case MCODE_OP_IF:
    {
      int Condition=IfCondition(Key,MR->Flags,GetOpCode(MR,Work.ExecLIBPos++)); // �������
      int Not=GetOpCode(MR,Work.ExecLIBPos++);    // ������� NOT
      if(Condition && Not || !Condition && !Not) // �������� Condition
        ; // �� ��������� ���� ��������� "MCODE_OP_JMP | idxElse"
      else
        Work.ExecLIBPos++;          // ������������� "MCODE_OP_JMP | idxElse" �... ���������� �� MCODE_OP_IFTHEN
      goto begin;
    }

    case MCODE_OP_IFTHEN:
      goto begin;

    case MCODE_OP_ELSE:
      goto begin;

    case MCODE_OP_ENDIF:
      goto begin;

    case MCODE_OP_EXIT:
      goto done;

    case KEY_ALTINS:
    {
      if(RunGraber())
        return KEY_NONE;
      break;
    }

    /* $MMode 1
       0: MCODE_OP_MACROMODE
       1: '1'
    */
    case MCODE_OP_MACROMODE:
      if (Work.ExecLIBPos<MR->BufferSize)
      {
        Key=GetOpCode(MR,Work.ExecLIBPos++);
        if(Key == '1') // �������� ����� ����������� ("DisableOutput").
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
      break;

    default:
      ;
  }

  if(MR==Work.MacroWORK && Work.ExecLIBPos>=MR->BufferSize)
  {
    ReleaseWORKBuffer();
    Work.Executing=MACROMODE_NOMACRO;
    if(TitleModified)
      SetFarTitle(NULL);
  }

  return(Key);
}

// ��������� - ����� �� ��� �������?
int KeyMacro::PeekKey()
{
  if (InternalInput || !Work.MacroWORK)
    return(0);

  struct MacroRecord *MR=Work.MacroWORK;
  if(Work.Executing == MACROMODE_NOMACRO && !Work.MacroWORK || Work.ExecLIBPos >= MR->BufferSize)
    return(FALSE);

  DWORD OpCode=GetOpCode(MR,Work.ExecLIBPos);
  return OpCode;
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
  ::KeyToText(MacroLIB[IdxMacro].Key,KeyText);
  sprintf(RegKeyName,"KeyMacros\\%s\\%s%s",
       GetSubKey(MacroLIB[IdxMacro].Flags&MFLAGS_MODEMASK),  (MacroLIB[IdxMacro].Flags&MFLAGS_DISABLEMACRO?"~":""),  KeyText);
  return RegKeyName;
}

/*
  ����� ������ ���� ������� ����� ������� ������!!!
  ������� ���������� ������ ������� ������������������, �.�.... �������
  � ��������� ������ ���������� Src
*/
char *KeyMacro::MkTextSequence(DWORD *Buffer,int BufferSize,const char *Src)
{
  int I,J, Key;
  char MacroKeyText[50], *TextBuffer;

  // �������� �������� ������� �����
  if((TextBuffer=(char*)xf_malloc(BufferSize*64+16)) == NULL)
    return NULL;

  TextBuffer[0]=0;

  if(BufferSize == 1)
  {
    if((((DWORD)Buffer)&KEY_MACRO_ENDBASE) >= KEY_MACRO_BASE && (((DWORD)Buffer)&KEY_MACRO_ENDBASE) <= KEY_MACRO_ENDBASE)
    {
      xf_free(TextBuffer);
      return Src?strdup(Src):NULL;
    }

    if(KeyToText((DWORD)Buffer,MacroKeyText))
      strcpy(TextBuffer,MacroKeyText);

    return TextBuffer;
  }

  for (J=0; J < BufferSize; J++)
  {
    Key=Buffer[J];

    if((Key&KEY_MACRO_ENDBASE) >= KEY_MACRO_BASE && (Key&KEY_MACRO_ENDBASE) <= KEY_MACRO_ENDBASE || (Key&MCODE_OP_JMP))
    {
      xf_free(TextBuffer);
      return Src?strdup(Src):NULL;
    }

    if(Key&MCODE_OP_JMP)
    {
//      J++;
      continue;
    }

    if(KeyToText(Key,MacroKeyText))
    {
      if(J)
        strcat(TextBuffer," ");
      strcat(TextBuffer,MacroKeyText);

#if 0
      switch(Key)
      {
        /* $If Condition $Then Seq_then [$Else Seq_else] $EndIf
                   0: MCODE_OP_IF
                   1: Condition
                   2: Not                  ���� ���������� '!', �� ����� 1, ����� 0
                   3: MCODE_OP_JMP | idxElse
                   4: MCODE_OP_IFTHEN
                   5: Seq_then
                        ...
         [
                   N: MCODE_OP_JMP | idxEndIf
             idxElse: MCODE_OP_ELSE                ���� ����� ����� � �� ����,
           idxElse+2: Seq_else                     �.�. �� MCODE_OP_ELSE ����� �����
                      ...                          ��������� MCODE_OP_ENDIF
         ]
            idxEndIf: MCODE_OP_ENDIF
        */
        case MCODE_OP_IF:
        {
          J++;
          strcat(TextBuffer," ");
          for(I=0; I < sizeof(MKeywords)/sizeof(MKeywords[0]); ++I)
            if(MKeywords[I].Value == Buffer[J])
              break;
          J++;
          if(Buffer[J])
            strcat(TextBuffer,"!");
          strcat(TextBuffer,(I == sizeof(MKeywords)/sizeof(MKeywords[0]))?"???":MKeywords[I].Name);
          J++;
          break;
        }

        case MCODE_OP_ELSE:
        case MCODE_OP_IFTHEN:
        case MCODE_OP_ENDIF:
        {                              // 28.10.2003 L&K  �� ��� �� ����������� �����? :-)
          break;
        }

        /* $Rep Count Seq $EndRep

                   0: MCODE_OP_REP
                   1: CounterOrig
                   2: MCODE_OP_JMP | idxEndRep
            idxSeq+0: seq
                      ...
        */
        case MCODE_OP_REP:
        {
          strcat(TextBuffer," ");
          strcat(TextBuffer,itoa(Buffer[J+1],MacroKeyText,10)); // ������� �������� ��������
          J+=2;
          break;
        }
        /* $Rep Count Seq $EndRep
                      ...
                      seq
                      ...
         idxEndRep+0: MCODE_OP_ENDREP
         idxEndRep+1: Counter
         idxEndRep+2: MCODE_OP_JMP | idxSeq

        */
        case MCODE_OP_ENDREP:
        {
          J+=2;
          break;
        }

        /* $Text
           0: KEY_MACRO_PLAINTEXT
           1: ������ ���������� �� 4 �����
              ���� ������ �����, �� ��������� �� KEY_MACRO_DATE DWORD = 0
        */
        case KEY_MACRO_PLAINTEXT:
        /* $Date
           0: KEY_MACRO_DATE
           1: ������ ���������� �� 4 �����
              ���� ������ �����, �� ��������� �� KEY_MACRO_DATE DWORD = 0
        */
        case KEY_MACRO_DATE:
        {
          ++J;
          int LenTextBuf=strlen((char*)&Buffer[J]);
          if(LenTextBuf)
          {
            //strcat(TextBuffer,((Key == KEY_MACRO_DATE)?" \"":" "));
            strcat(TextBuffer," \"");
            char *TextBufferPtr=TextBuffer+strlen(TextBuffer);
            strcat(TextBuffer,(char*)&Buffer[J]);
            ReplaceStrings(TextBufferPtr,"\\","\\\\",-1);
            ReplaceStrings(TextBufferPtr,"\"","\\\"",-1);
            ReplaceStrings(TextBufferPtr,"\n","\\n",-1);
            ReplaceStrings(TextBufferPtr,"\t","\\t",-1);
            char *TextBufferPtr2=TextBufferPtr, TempBuf1[2],TempBuf2[16];
            TempBuf1[1]=0;
            TempBuf2[0]='\\';
            TempBuf2[1]='x';
            while(*TextBufferPtr2)
            {
              if(*TextBufferPtr2 < ' ')
              {
                TempBuf1[0]=*TextBufferPtr2;
                itoa(*TextBufferPtr2,TempBuf2+2,16);
                ReplaceStrings(TextBufferPtr2,TempBuf1,TempBuf2,-1);
              }
              ++TextBufferPtr2;
            }
            strcat(TextBuffer,"\"");
          }
          // ����� ��� ������ ������ 4
          J+=LenTextBuf/sizeof(DWORD);
          // ������� ����� ��������� ��� ��������� �������� �����
          break;
        }

      }
#endif
    }
  }
  return TextBuffer;
}

// ���������� ���� ��������
void KeyMacro::SaveMacros(BOOL AllSaved)
{
  char *TextBuffer;
  char RegKeyName[150];
  for (int I=0;I<MacroLIBCount;I++)
  {
    if(!AllSaved  && !(MacroLIB[I].Flags&MFLAGS_NEEDSAVEMACRO))
      continue;

    MkRegKeyName(I,RegKeyName);

    if (MacroLIB[I].BufferSize==0 || !MacroLIB[I].Src)
    {
      DeleteRegKey(RegKeyName);
      continue;
    }
#if 0
    if((TextBuffer=MkTextSequence(MacroLIB[I].Buffer,MacroLIB[I].BufferSize,MacroLIB[I].Src)) == NULL)
      continue;

    SetRegKey(RegKeyName,"Sequence",TextBuffer);

    //_SVS(SysLog("%3d) %s|Sequence='%s'",I,RegKeyName,TextBuffer));
    if(TextBuffer)
      xf_free(TextBuffer);
#endif
    SetRegKey(RegKeyName,"Sequence",MacroLIB[I].Src);

    // ����������� ����...
    for(int J=0; J < sizeof(MKeywordsFlags)/sizeof(MKeywordsFlags[0]); ++J)
    {
      if (MacroLIB[I].Flags & MKeywordsFlags[J].Value)
        SetRegKey(RegKeyName,MKeywordsFlags[J].Name,1);
      else
        DeleteRegValue(RegKeyName,MKeywordsFlags[J].Name);
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
  struct MacroRecord CurMacro;
  /* $ 20.12.2003 IS
       ������������� �������, ����� ������ ��� xf_free(Src)
  */
  memset(&CurMacro,0,sizeof(CurMacro));
  /* IS $ */

  char UpKeyName[100];
  char RegKeyName[150],KeyText[50];

  sprintf(UpKeyName,"KeyMacros\\%s",GetSubKey(ReadMode));

  for (I=0;;I++)
  {
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

    GetRegKey(RegKeyName,"Sequence",Buffer,"",BufferSize);
    RemoveExternalSpaces(Buffer);
    if(!strlen(Buffer))
      continue;

    CurMacro.Key=KeyCode;
    CurMacro.Buffer=NULL;
    CurMacro.Src=NULL;
    CurMacro.BufferSize=0;
    CurMacro.Flags=MFlags|(ReadMode&MFLAGS_MODEMASK);

    for(J=0; J < sizeof(MKeywordsFlags)/sizeof(MKeywordsFlags[0]); ++J)
      CurMacro.Flags|=GetRegKey(RegKeyName,MKeywordsFlags[J].Name,0)?MKeywordsFlags[J].Value:0;

    if(ReadMode == MACRO_EDITOR || ReadMode == MACRO_DIALOG || ReadMode == MACRO_VIEWER)
    {
      if(CurMacro.Flags&MFLAGS_SELECTION)
      {
        CurMacro.Flags&=~MFLAGS_SELECTION;
        CurMacro.Flags|=MFLAGS_EDITSELECTION;
      }
      if(CurMacro.Flags&MFLAGS_NOSELECTION)
      {
        CurMacro.Flags&=~MFLAGS_NOSELECTION;
        CurMacro.Flags|=MFLAGS_EDITNOSELECTION;
      }
    }

    if(!ParseMacroString(&CurMacro,Buffer))
      continue;

    struct MacroRecord *NewMacros=(struct MacroRecord *)xf_realloc(MacroLIB,sizeof(*MacroLIB)*(MacroLIBCount+1));
    if (NewMacros==NULL)
    {
      return FALSE;
    }
    MacroLIB=NewMacros;
    CurMacro.Src=strdup(Buffer);
    memcpy(MacroLIB+MacroLIBCount,&CurMacro,sizeof(CurMacro));
    MacroLIBCount++;
  }
  return TRUE;
}
/* SVS $ */

// �������, ����������� ������� ��� ������ ����
// ���� �� ��������� �������������� � �������������� ���������
// �������� ��������, �� ������ ����!
void KeyMacro::RunStartMacro()
{
  if (!(CtrlObject->Cp() && CtrlObject->Cp()->ActivePanel && !CmdMode && CtrlObject->Plugins.IsPluginsLoaded()))
    return;

  static int IsRunStartMacro=FALSE;
  if(IsRunStartMacro)
    return;
  IsRunStartMacro=TRUE;

  if(!IndexMode[MACRO_SHELL][1])
    return;

  struct MacroRecord *MR=MacroLIB+IndexMode[MACRO_SHELL][0];
  for(int I=0; I < IndexMode[MACRO_SHELL][1]; ++I)
  {
    DWORD CurFlags;
    if (((CurFlags=MR[I].Flags)&MFLAGS_MODEMASK)==MACRO_SHELL &&
        MR[I].BufferSize>0 &&
        // ��������� �� ������������� �������
        !(CurFlags&MFLAGS_DISABLEMACRO) &&
        (CurFlags&MFLAGS_RUNAFTERFARSTART) && CtrlObject)
    {
      if(CheckAll(MACRO_SHELL,CurFlags))
        PostNewMacro(MR+I);
    }
  }
  Work.ExecLIBPos=0;
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
      "CtrlDown", "Enter", "Esc", "F1", "CtrlF5"
    };
    for(I=0; I < sizeof(PreDefKeyName)/sizeof(PreDefKeyName[0]); ++I)
      Dialog::SendDlgMessage(hDlg,DM_LISTADDSTR,2,(long)PreDefKeyName[I]);
/*
    int KeySize=GetRegKeySize("KeyMacros","DlgKeys");
    char *KeyStr;
    if(KeySize &&
       (KeyStr=(char*)xf_malloc(KeySize+1)) != NULL &&
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
      xf_free(KeyStr);
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
//    if((Param2&0x00FFFFFF) >= 'A' && (Param2&0x00FFFFFF) <= 'Z' && ShiftPressed)
//      Param2|=KEY_SHIFT;

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
      struct MacroRecord *Mac=MacroDlg->MacroLIB+Index;
      DWORD DisFlags=Mac->Flags&MFLAGS_DISABLEMACRO;
      char Buf[256];
      char BufKey[64];
      char RegKeyName[150];
      char *TextBuffer;

      MacroDlg->MkRegKeyName(Index,RegKeyName);
      // ����� �� ������.
#if 0
      if((TextBuffer=MacroDlg->MkTextSequence(Mac->Buffer,Mac->BufferSize)) != NULL)
      {
        int F=0;
        I=strlen(TextBuffer);
        if(I > 45) { I=45; F++; }
        sprintf(Buf,"\"%*.*s%s\"",I,I,TextBuffer,(F?"...":""));
        strcpy(BufKey,Buf);
        xf_free(TextBuffer);
      }
      else
        BufKey[0]=0;
#else
      if(Mac->Src != NULL)
      {
        int F=0;
        I=strlen(Mac->Src);
        if(I > 45) { I=45; F++; }
        sprintf(Buf,"\"%*.*s%s\"",I,I,Mac->Src,(F?"...":""));
        strcpy(BufKey,Buf);
      }
      else
        BufKey[0]=0;
#endif
      sprintf(Buf,
        MSG(!MacroDlg->RecBufferSize?
           (DisFlags?MMacroDeleteAssign:MMacroDeleteKey):
           MMacroReDefinedKey),
        KeyText);

      // �������� "� �� ��������� �� ��?"
      if(!DisFlags &&
         Mac->Buffer && MacroDlg->RecBuffer &&
         Mac->BufferSize == MacroDlg->RecBufferSize &&
         (
           Mac->BufferSize >  1 && !memcmp(Mac->Buffer,MacroDlg->RecBuffer,MacroDlg->RecBufferSize*sizeof(DWORD)) ||
           Mac->BufferSize == 1 && (DWORD)Mac->Buffer == (DWORD)MacroDlg->RecBuffer
         )
        )
        I=0;
      else
        I=Message(MSG_WARNING,2,MSG(MWarning),
            Buf,
            BufKey,
            MSG(!MacroDlg->RecBufferSize?MMacroDeleteKey2:
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


long WINAPI KeyMacro::ParamMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  static struct DlgParam *KMParam=NULL;

  if(Msg == DN_INITDIALOG)
  {
    KMParam=(struct DlgParam *)Param2;
  }
  else if(Msg == DN_BTNCLICK && (Param1 == 4 || Param1 == 8))
  {
    for(int I=1; I <= 3; ++I)
      Dialog::SendDlgMessage(hDlg,DM_ENABLE,Param1+I,Param2);
  }
#if 0
  else if(Msg==DN_KEY && Param2==KEY_ALTF4)
  {
    if (RegVer)
    {
      KeyMacro *MacroDlg=KMParam->Handle;
      (*FrameManager)[0]->UnlockRefresh();
      FILE *MacroFile;
      char MacroFileName[NM];
      if (!FarMkTempEx(MacroFileName) || (MacroFile=fopen(MacroFileName,"wb"))==NULL)
        return TRUE;

      char *TextBuffer;
      DWORD Buf[1];
      Buf[0]=MacroDlg->RecBuffer[0];
      if((TextBuffer=MacroDlg->MkTextSequence((MacroDlg->RecBufferSize==1?Buf:MacroDlg->RecBuffer),MacroDlg->RecBufferSize)) != NULL)
      {
        fwrite(TextBuffer,strlen(TextBuffer),1,MacroFile);
        fclose(MacroFile);
        xf_free(TextBuffer);
        {
          //ConsoleTitle *OldTitle=new ConsoleTitle;
          FileEditor ShellEditor(MacroFileName,FALSE,FALSE,-1,-1,TRUE,NULL);
          //delete OldTitle;
          ShellEditor.SetDynamicallyBorn(false);
          FrameManager->EnterModalEV();
          FrameManager->ExecuteModal();
          FrameManager->ExitModalEV();
          if (!ShellEditor.IsFileChanged() || (MacroFile=fopen(MacroFileName,"rb"))==NULL)
            ;
          else
          {
            struct MacroRecord NewMacroWORK2={0};
            long FileSize=filelen(MacroFile);
            TextBuffer=(char*)malloc(FileSize);
            if(TextBuffer)
            {
              fread(TextBuffer,FileSize,1,MacroFile);
              if(!MacroDlg->ParseMacroString(&NewMacroWORK2,TextBuffer))
              {
                if(NewMacroWORK2.BufferSize > 1)
                  xf_free(NewMacroWORK2.Buffer);
              }
              else
              {
                MacroDlg->RecBuffer=NewMacroWORK2.Buffer;
                MacroDlg->RecBufferSize=NewMacroWORK2.BufferSize;
              }
            }
            fclose(MacroFile);
          }
        }
        FrameManager->ResizeAllFrame();
        FrameManager->PluginCommit();
      }
      else
        fclose(MacroFile);
      remove(MacroFileName);
    }
    else
      Message(MSG_WARNING,1,MSG(MWarning),MSG(MRegOnly),MSG(MOk));
    return TRUE;
  }
#endif
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

/* $ 03.01.2001 IS
   ! ���������� "�������� ���������"
*/
int KeyMacro::GetMacroSettings(int Key,DWORD &Flags)
{
/*
           1         2         3         4         5         6
    3456789012345678901234567890123456789012345678901234567890123456789
  1 �=========== ��������� ������������ ��� 'CtrlP' ==================�
  2 � [ ] ��������� �� ����� ���������� ����� �� �����                �
  3 � [ ] ��������� ����� ������� FAR                                 �
  4 �-----------------------------------------------------------------�
  5 � [ ] �������� ������             [ ] ��������� ������            �
  6 �   [?] �� ������ �������           [?] �� ������ �������         �
  7 �   [?] ��������� ��� �����         [?] ��������� ��� �����       �
  8 �   [?] �������� �����              [?] �������� �����            �
  9 �-----------------------------------------------------------------�
 10   [?] ������ ��������� ������
 11 � [?] ������� ����                                                �
 12 �-----------------------------------------------------------------�
 13 �               [ ���������� ]  [ �������� ]                      �
 14 L=================================================================+

*/
  static struct DialogData MacroSettingsDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,69,14,0,0,0,0,"",
  /* 01 */DI_CHECKBOX,5,2,0,0,1,0,0,0,(char *)MMacroSettingsEnableOutput,
  /* 02 */DI_CHECKBOX,5,3,0,0,0,0,0,0,(char *)MMacroSettingsRunAfterStart,
  /* 03 */DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */DI_CHECKBOX,5,5,0,0,0,0,0,0,(char *)MMacroSettingsActivePanel,
  /* 05 */DI_CHECKBOX,7,6,0,0,0,2,DIF_3STATE|DIF_DISABLE,0,(char *)MMacroSettingsPluginPanel,
  /* 06 */DI_CHECKBOX,7,7,0,0,0,2,DIF_3STATE|DIF_DISABLE,0,(char *)MMacroSettingsFolders,
  /* 07 */DI_CHECKBOX,7,8,0,0,0,2,DIF_3STATE|DIF_DISABLE,0,(char *)MMacroSettingsSelectionPresent,
  /* 08 */DI_CHECKBOX,37,5,0,0,0,0,0,0,(char *)MMacroSettingsPassivePanel,
  /* 09 */DI_CHECKBOX,39,6,0,0,0,2,DIF_3STATE|DIF_DISABLE,0,(char *)MMacroSettingsPluginPanel,
  /* 10 */DI_CHECKBOX,39,7,0,0,0,2,DIF_3STATE|DIF_DISABLE,0,(char *)MMacroSettingsFolders,
  /* 11 */DI_CHECKBOX,39,8,0,0,0,2,DIF_3STATE|DIF_DISABLE,0,(char *)MMacroSettingsSelectionPresent,
  /* 12 */DI_TEXT,3,9,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 13 */DI_CHECKBOX,5,10,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsCommandLine,
  /* 14 */DI_CHECKBOX,5,11,0,0,0,2,DIF_3STATE,0,(char *)MMacroSettingsSelectionBlockPresent,
  /* 15 */DI_TEXT,3,12,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 16 */DI_BUTTON,0,13,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
  /* 17 */DI_BUTTON,0,13,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(MacroSettingsDlgData,MacroSettingsDlg);

  char KeyText[66];
  KeyToText(Key,KeyText);
  sprintf(MacroSettingsDlg[0].Data,MSG(MMacroSettingsTitle),KeyText);
//  if(!(Key&0x7F000000))
//    MacroSettingsDlg[3].Flags|=DIF_DISABLE;

  MacroSettingsDlg[1].Selected=Flags&MFLAGS_DISABLEOUTPUT?0:1;
  MacroSettingsDlg[2].Selected=Flags&MFLAGS_RUNAFTERFARSTART?1:0;

  MacroSettingsDlg[5].Selected=Set3State(Flags,MFLAGS_NOFILEPANELS,MFLAGS_NOPLUGINPANELS);
  MacroSettingsDlg[6].Selected=Set3State(Flags,MFLAGS_NOFILES,MFLAGS_NOFOLDERS);
  MacroSettingsDlg[7].Selected=Set3State(Flags,MFLAGS_SELECTION,MFLAGS_NOSELECTION);

  MacroSettingsDlg[9].Selected=Set3State(Flags,MFLAGS_PNOFILEPANELS,MFLAGS_PNOPLUGINPANELS);
  MacroSettingsDlg[10].Selected=Set3State(Flags,MFLAGS_PNOFILES,MFLAGS_PNOFOLDERS);
  MacroSettingsDlg[11].Selected=Set3State(Flags,MFLAGS_PSELECTION,MFLAGS_PNOSELECTION);

  MacroSettingsDlg[13].Selected=Set3State(Flags,MFLAGS_EMPTYCOMMANDLINE,MFLAGS_NOTEMPTYCOMMANDLINE);
  MacroSettingsDlg[14].Selected=Set3State(Flags,MFLAGS_EDITSELECTION,MFLAGS_EDITNOSELECTION);

  struct DlgParam Param={this,0,0};
  Dialog Dlg(MacroSettingsDlg,sizeof(MacroSettingsDlg)/sizeof(MacroSettingsDlg[0]),ParamMacroDlgProc,(long)&Param);
  Dlg.SetPosition(-1,-1,73,16);
  Dlg.SetHelp("KeyMacroSetting");
  FrameManager->GetBottomFrame()->LockRefresh(); // ������� ���������� ������
  Dlg.Process();
  FrameManager->GetBottomFrame()->UnlockRefresh(); // ������ ����� :-)
  if (Dlg.GetExitCode()!=16)
    return(FALSE);

  Flags=MacroSettingsDlg[1].Selected?0:MFLAGS_DISABLEOUTPUT;
  Flags|=MacroSettingsDlg[2].Selected?MFLAGS_RUNAFTERFARSTART:0;
  if(MacroSettingsDlg[4].Selected)
  {
    Flags|=MacroSettingsDlg[5].Selected==2?0:
            (MacroSettingsDlg[5].Selected==0?MFLAGS_NOPLUGINPANELS:MFLAGS_NOFILEPANELS);
    Flags|=MacroSettingsDlg[6].Selected==2?0:
            (MacroSettingsDlg[6].Selected==0?MFLAGS_NOFOLDERS:MFLAGS_NOFILES);
    Flags|=MacroSettingsDlg[7].Selected==2?0:
            (MacroSettingsDlg[7].Selected==0?MFLAGS_NOSELECTION:MFLAGS_SELECTION);
  }
  if(MacroSettingsDlg[8].Selected)
  {
    Flags|=MacroSettingsDlg[9].Selected==2?0:
            (MacroSettingsDlg[9].Selected==0?MFLAGS_PNOPLUGINPANELS:MFLAGS_PNOFILEPANELS);
    Flags|=MacroSettingsDlg[10].Selected==2?0:
            (MacroSettingsDlg[10].Selected==0?MFLAGS_PNOFOLDERS:MFLAGS_PNOFILES);
    Flags|=MacroSettingsDlg[11].Selected==2?0:
            (MacroSettingsDlg[11].Selected==0?MFLAGS_PNOSELECTION:MFLAGS_PSELECTION);
  }
  Flags|=MacroSettingsDlg[13].Selected==2?0:
          (MacroSettingsDlg[13].Selected==0?MFLAGS_NOTEMPTYCOMMANDLINE:MFLAGS_EMPTYCOMMANDLINE);
  Flags|=MacroSettingsDlg[14].Selected==2?0:
          (MacroSettingsDlg[14].Selected==0?MFLAGS_EDITNOSELECTION:MFLAGS_EDITSELECTION);

  return(TRUE);
}
/* IS $ */

int KeyMacro::PostNewMacro(char *PlainText,DWORD Flags)
{
  struct MacroRecord NewMacroWORK2;
  memset(&NewMacroWORK2,0,sizeof(struct MacroRecord));

  // ������� ������� �� ������
  if(!ParseMacroString(&NewMacroWORK2,PlainText))
  {
    if(NewMacroWORK2.BufferSize > 1)
      xf_free(NewMacroWORK2.Buffer);
    return FALSE;
  }
  NewMacroWORK2.Flags=Flags;

  // ������ ��������� �������� ������� ������ ������
  struct MacroRecord *NewMacroWORK;
  if((NewMacroWORK=(struct MacroRecord *)xf_realloc(Work.MacroWORK,sizeof(MacroRecord)*(Work.MacroWORKCount+1))) == NULL)
  {
    if(NewMacroWORK2.BufferSize > 1)
      xf_free(NewMacroWORK2.Buffer);
    return FALSE;
  }

  // ������ ������� � ���� "�������" ����� ������
  Work.MacroWORK=NewMacroWORK;
  NewMacroWORK=Work.MacroWORK+Work.MacroWORKCount;
  memcpy(NewMacroWORK,&NewMacroWORK2,sizeof(struct MacroRecord));
  Work.MacroWORKCount++;

//  Work.Executing=Work.MacroWORK->Flags&MFLAGS_NOSENDKEYSTOPLUGINS?MACROMODE_EXECUTING:MACROMODE_EXECUTING_COMMON;
  if(Work.ExecLIBPos == Work.MacroWORK->BufferSize)
    Work.ExecLIBPos=0;
  return TRUE;
}

int KeyMacro::PostNewMacro(struct MacroRecord *MRec,BOOL NeedAddSendFlag)
{
  if(!MRec)
    return FALSE;

  struct MacroRecord NewMacroWORK2;
  memcpy(&NewMacroWORK2,MRec,sizeof(struct MacroRecord));
  NewMacroWORK2.Src=NULL;

//  if(MRec->BufferSize > 1)
  {
    if((NewMacroWORK2.Buffer=(DWORD*)xf_malloc((MRec->BufferSize+1)*sizeof(DWORD))) == NULL)
    {
      return FALSE;
    }
  }

  // ������ ��������� �������� ������� ������ ������
  struct MacroRecord *NewMacroWORK;
  if((NewMacroWORK=(struct MacroRecord *)xf_realloc(Work.MacroWORK,sizeof(MacroRecord)*(Work.MacroWORKCount+1))) == NULL)
  {
//    if(MRec->BufferSize > 1)
      xf_free(NewMacroWORK2.Buffer);
    return FALSE;
  }

  // ������ ������� � ���� "�������" ����� ������
  if((MRec->BufferSize+1) > 2)
    memcpy(NewMacroWORK2.Buffer,MRec->Buffer,sizeof(DWORD)*MRec->BufferSize);
  else if(MRec->Buffer)
    NewMacroWORK2.Buffer[0]=(DWORD)MRec->Buffer;
  NewMacroWORK2.Buffer[NewMacroWORK2.BufferSize++]=KEY_NONE; // ���.�������/��������

  Work.MacroWORK=NewMacroWORK;
  NewMacroWORK=Work.MacroWORK+Work.MacroWORKCount;
  memcpy(NewMacroWORK,&NewMacroWORK2,sizeof(struct MacroRecord));
  Work.MacroWORKCount++;

//  Work.Executing=Work.MacroWORK->Flags&MFLAGS_NOSENDKEYSTOPLUGINS?MACROMODE_EXECUTING:MACROMODE_EXECUTING_COMMON;
  if(Work.ExecLIBPos == Work.MacroWORK->BufferSize)
    Work.ExecLIBPos=0;
  return TRUE;
}

// ������ ��������� ������������ � ���� ������
int KeyMacro::ParseMacroString(struct MacroRecord *CurMacro,const char *BufPtr)
{
  if(!CurMacro || !BufPtr || !*BufPtr)
    return FALSE;

  int SizeCurKeyText=strlen(BufPtr)*2; // ������ ����� �������
  char *CurKeyText=(char *)xf_malloc(SizeCurKeyText); // ������ ����� �������

  if(!CurKeyText)
    return FALSE;

  static int Rep[17];
  static int If[17];

  int J,I;
  int RepLevel=-1;
  int IfLevel=-1;
  /* 28.10.2003 L&K ��� �������� �� ������� ��� ������ $Then */
  int ThenLevel=-1;
  DWORD *CurMacro_Buffer=NULL;
  DWORD KeyCode;
  int Size, Length;

  memset(If,0xFF,sizeof(If));

  // ����� ��������� ������������, �������� ������ ������������������,
  // ������� ��������� � Buffer
  while (1)
  {
    if((BufPtr=__GetNextWord(BufPtr,CurKeyText)) == NULL)
      break;

    // � CurKeyText - �������� �������. ��������� �������� �� ���...
    if((KeyCode=KeyNameToKey(CurKeyText)) == -1)
    {
      if (CurMacro_Buffer!=NULL)
      {
        xf_free(CurMacro_Buffer);
        CurMacro->Buffer=NULL;
      }
      xf_free(CurKeyText);
      CurMacro->BufferSize=0;
      return FALSE;
    }

    Size=1;
    switch(KeyCode)
    {
      /* $Text
         0: KEY_MACRO_PLAINTEXT
         1: ������ ���������� �� 4 �����
      */
      case KEY_MACRO_PLAINTEXT:
      /* $Date
         0: KEY_MACRO_DATE
         1: ������, ����������� �� 4 �����
            ���� ������ �����, �� ��������� �� KEY_MACRO_DATE DWORD = 0
      */
      case KEY_MACRO_DATE:
      {
        const char *BufPtr2=BufPtr;
        memset(CurKeyText,0,SizeCurKeyText);
        // ���� ������ �������
        while (*BufPtr)
        {
          if(*BufPtr == '\\' && BufPtr[1] == '"')
            BufPtr++;
          else if(*BufPtr == '"')
          {
            //BufPtr++;
            break;
          }
          BufPtr++;
        }
        if(*BufPtr)
          BufPtr=ParsePlainText(CurKeyText,BufPtr);
        else
          BufPtr=BufPtr2;
        Length=strlen(CurKeyText)+1;
        // ������ ������ ���� ��������� �� 4
        Size+=Length/sizeof(DWORD);
        if(Length==1 || (Length%sizeof(DWORD)) != 0) // ���������� �� sizeof(DWORD) ������.
          Size++;
        break;
      }

      // $Rep Count Seq $EndRep
      case MCODE_OP_REP:
      {
//        if(!strstr(BufPtr,"$EndRep"))
//          goto clean_done;
        if((BufPtr=__GetNextWord(BufPtr,CurKeyText)) == NULL) // ������� ��������� "�����", ��������������, ��� ��� CounterOrig
          break;
        Size+=2;  // �����������, ������ ����� �� 2 ������
        break;
      }

      case MCODE_OP_ENDREP:
      {
        Size+=2; // �.�. ��������������, ��� �� $EndRep ������� ������� � ������ ������ seq
        break;
      }

      /* $If Condition $Then Seq_then [$Else Seq_else] $EndIf
                 0: MCODE_OP_IF
                 1: Condition
                 2: Not                  ���� ���������� '!', �� ����� 1, ����� 0
                 3: MCODE_OP_JMP | idxElse
                 4: MCODE_OP_IFTHEN
                 5: Seq_then
                      ...
       [
                 N: MCODE_OP_JMP | idxEndIf
           idxElse: MCODE_OP_ELSE                ���� ����� ����� � �� ����,
         idxElse+2: Seq_else                     �.�. �� MCODE_OP_ELSE ����� �����
                    ...                          ��������� MCODE_OP_ENDIF
       ]
          idxEndIf: MCODE_OP_ENDIF
      */
      case MCODE_OP_IF:
      {
//        if(!strstr(BufPtr,"$Then"))
//          goto clean_done;
//        if(!strstr(BufPtr,"$EndIf"))
//          goto clean_done;
        if((BufPtr=__GetNextWord(BufPtr,CurKeyText)) == NULL) // ������� ��������� "�����" - Condition
          break;
        Size+=3;  // �����������, ������ ����� �� 3 ������
        break;
      }

      case MCODE_OP_IFTHEN:
      case MCODE_OP_ENDIF:
        break;

      case MCODE_OP_ELSE:
      {
//        if(!strstr(BufPtr,"$EndIf"))
//          goto clean_done;
        // ����� ����� �������� (MCODE_OP_JMP|idxEndIf) ����� MCODE_OP_ELSE
        Size++;
        break;
      }
    }

    if(!BufPtr)
      break;

    // ��� ������, ������� ���� ��� � ����� ������������������.
    if (KeyCode!=-1)
    {
      CurMacro_Buffer=(DWORD *)xf_realloc(CurMacro_Buffer,sizeof(*CurMacro_Buffer)*(CurMacro->BufferSize+Size));
      if (CurMacro_Buffer==NULL)
      {
        CurMacro->Buffer=NULL;
        CurMacro->BufferSize=0;
        xf_free(CurKeyText);
        return FALSE;
      }

      switch(KeyCode)
      {
        case MCODE_OP_REP:
        case MCODE_OP_ENDREP:
        case MCODE_OP_IF:
        case MCODE_OP_ENDIF:
        case MCODE_OP_MACROMODE:
        case MCODE_OP_EXIT:
          CurMacro_Buffer[CurMacro->BufferSize]=KeyCode;
          break;
        case MCODE_OP_IFTHEN:
          /* 28.10.2003 L&K �������� �� ������� $Then */
          if(ThenLevel+1 != IfLevel)
            goto clean_done;    // ���������� � ���������� $If � $Then
          // ��� �����, $Then'�� �� ���� ������, ��� $If'��
          ThenLevel++;                  // ������������...
          CurMacro_Buffer[CurMacro->BufferSize]=KeyCode;
          break;
        case MCODE_OP_ELSE:
        {
          /* $If Condition $Then Seq_then [$Else Seq_else] $EndIf
                     0: MCODE_OP_IF
                     1: Condition
                     2: Not                  ���� ���������� '!', �� ����� 1, ����� 0
                     3: MCODE_OP_JMP | idxElse
                     4: MCODE_OP_IFTHEN
                     5: Seq_then
                          ...
           [
                     N: MCODE_OP_JMP | idxEndIf       <- CurMacro->BufferSize
               idxElse: MCODE_OP_ELSE                 <- CurMacro->BufferSize+1
             idxElse+2: Seq_else                      <- CurMacro->BufferSize+2
                        ...
           ]
              idxEndIf: MCODE_OP_ENDIF
          */
          /* 28.10.2003 L&K �������� �� ������� $Then */
          if(ThenLevel != IfLevel)
            goto clean_done;    // ������

          CurMacro_Buffer[If[IfLevel]]=MCODE_OP_JMP | (CurMacro->BufferSize+1);  // 3: MCODE_OP_JMP | idxElse
          If[IfLevel]=CurMacro->BufferSize;                                    // ��������� ������� ��� ���������� "N: MCODE_OP_JMP | idxEndIf"
          CurMacro_Buffer[CurMacro->BufferSize+1]=MCODE_OP_ELSE;
          break;
        }
        default:
          CurMacro_Buffer[CurMacro->BufferSize]=KeyCode;
      } // end switch(KeyCode)


      switch(KeyCode)
      {
        case KEY_MACRO_DATE:
        case KEY_MACRO_PLAINTEXT:
          // $Date � $Text - ����� ���� ASCIIZ-������
          memcpy(&CurMacro_Buffer[CurMacro->BufferSize+1],CurKeyText,(Size-1)*sizeof(DWORD));
          break;

        case MCODE_OP_REP:
        {
          if(++RepLevel >= sizeof(Rep))  // ������� ����� ��������!
            goto clean_done;

          CurMacro_Buffer[CurMacro->BufferSize+1]=atoi(CurKeyText); // ���� �������� ������������ �������
          Rep[RepLevel]=CurMacro->BufferSize+2;                     //           2: MCODE_OP_JMP | idxEndRep
          break;
        }

        case MCODE_OP_ENDREP:
        {
          /* $Rep Count Seq $EndRep

               0: MCODE_OP_REP
               1: CounterOrig
               2: MCODE_OP_JMP | idxEndRep
        idxSeq+0: seq
                  ...
     idxEndRep+0: MCODE_OP_ENDREP
     idxEndRep+1: Counter
     idxEndRep+2: MCODE_OP_JMP | idxSeq

          */
          if(RepLevel < 0) // ��� �� �������, ������� ��������� :-(
            goto clean_done;

          CurMacro_Buffer[CurMacro->BufferSize+1]=0; // Counter - ����������� � ������ ���������� MCODE_OP_REP
          CurMacro_Buffer[CurMacro->BufferSize+2]=MCODE_OP_JMP | (Rep[RepLevel]+1); // idxEndRep+2: MCODE_OP_JMP | idxSeq
          CurMacro_Buffer[Rep[RepLevel]]=MCODE_OP_JMP | CurMacro->BufferSize;       //           2: MCODE_OP_JMP | idxEndRep
          RepLevel--;
          break;
        }

        case MCODE_OP_IF:
        {
          /* $If Condition $Then Seq_then [$Else Seq_else] $EndIf
                     0: MCODE_OP_IF
                     1: Condition
                     2: Not                  ���� ���������� '!', �� ����� 1, ����� 0
                     3: MCODE_OP_JMP | idxElse
                     4: MCODE_OP_IFTHEN
                     5: Seq_then
                          ...
           [
                     N: MCODE_OP_JMP | idxEndIf
               idxElse: MCODE_OP_ELSE                ���� ����� ����� � �� ����,
             idxElse+2: Seq_else                     �.�. �� MCODE_OP_ELSE ����� �����
                        ...                          ��������� MCODE_OP_ENDIF
           ]
              idxEndIf: MCODE_OP_ENDIF
          */
          IfLevel++;
          KeyCode=-1;

          //CurKeyText==Condition
          for(I=0;I < sizeof(MKeywords)/sizeof(MKeywords[0]); ++I)
            if(!stricmp(MKeywords[I].Name,CurKeyText+(*CurKeyText=='!'?1:0)))
            {
              KeyCode=MKeywords[I].Value;
              break;
            }
                                                                            // 0: MCODE_OP_IF
          CurMacro_Buffer[CurMacro->BufferSize+1]=KeyCode;                  // 1: Condition
          CurMacro_Buffer[CurMacro->BufferSize+2]=(*CurKeyText=='!'?1:0);   // 2: Not: ���� ���������� '!', �� ����� 1, ����� 0
          CurMacro_Buffer[CurMacro->BufferSize+3]=-1;                       // 3: MCODE_OP_JMP | idxElse
                                                                            //      ����������� ��� ��������� MCODE_OP_ELSE
          If[IfLevel]=CurMacro->BufferSize+3;                               // ���������� ������� ��� (MCODE_OP_JMP|idxElse)
          break;
        }

        case MCODE_OP_ENDIF:
        {
          /* 28.10.2003 L&K �������� �� ������� $Then */
          if(ThenLevel != IfLevel)
            goto clean_done;    // ������ (��� ������ :)

          // ���������� ������� ��� N: (MCODE_OP_JMP|idxEndIf) ��� (MCODE_OP_JMP|idxElse), ���� '$else' �� ����
          if(If[IfLevel] != -1)
            CurMacro_Buffer[If[IfLevel]]=MCODE_OP_JMP | CurMacro->BufferSize;
          If[IfLevel]=-1;
          --IfLevel;
          /* 28.10.2003 L&K �������� �� ������� $Then */
          --ThenLevel;          // $If ��������, ���������� ������� $Then'��
          break;
        }
      } // end switch(KeyCode)

      CurMacro->BufferSize+=Size;
    }
  }
  if(CurMacro->BufferSize > 1)
  {
    CurMacro->Buffer=CurMacro_Buffer;
  }
  else if(CurMacro_Buffer)
  {
    CurMacro->Buffer=reinterpret_cast<DWORD*>(*CurMacro_Buffer);
    xf_free(CurMacro_Buffer);
  }

  xf_free(CurKeyText);
  return TRUE;

clean_done:
  if (CurMacro_Buffer)
  {
    xf_free(CurMacro_Buffer);
    CurMacro_Buffer=NULL;
  }
  if (CurMacro->Buffer)
  {
    xf_free(CurMacro->Buffer);
    CurMacro->Buffer=NULL;
  }
  CurMacro->BufferSize=0;
  xf_free(CurKeyText);
  return FALSE;
}


// ������� ��������� ������� ������� ������� � �������
// Ret=-1 - �� ������ �������.
// ���� CheckMode=-1 - ������ ������ � ����� ������, �.�. ������ ����������
int KeyMacro::GetIndex(int Key, int ChechMode)
{
  if(MacroLIB)
  {
    int Pos,Len;
    struct MacroRecord *MPtr;
    if(ChechMode == -1)
    {
      Len=MacroLIBCount;
      MPtr=MacroLIB;
    }
    else
    {
      Len=IndexMode[ChechMode][1];
      if(!Len)
       return -1;
      MPtr=MacroLIB+IndexMode[ChechMode][0];
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
  return sizeof(struct MacroRecord)+MacroLIB[Pos].BufferSize;
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

int KeyMacro::GetMacroKeyInfo(int Mode,int Pos,char *KeyName,char *Description,int DescriptionSize)
{
  if(Mode >= MACRO_OTHER && Mode < MACRO_LAST)
  {
    char UpKeyName[100];
    char RegKeyName[150],KeyText[50];
    sprintf(UpKeyName,"KeyMacros\\%s",GetSubKey(Mode));

    if (!EnumRegKey(UpKeyName,Pos,RegKeyName,sizeof(RegKeyName)))
      return -1;

    char *KeyNamePtr=strrchr(RegKeyName,'\\');
    if (KeyNamePtr!=NULL)
      strcpy(KeyName,KeyNamePtr+1);
    GetRegKey(RegKeyName,"Description",Description,"",DescriptionSize);
    return Pos+1;
  }
  return -1;
}

/* $ 20.03.2002 DJ
   ����������� �������� ����� � ��� ��������
*/

BOOL KeyMacro::CheckEditSelected(DWORD CurFlags)
{
  if(Mode==MACRO_EDITOR || Mode==MACRO_DIALOG || Mode==MACRO_VIEWER || (Mode==MACRO_SHELL&&CtrlObject->CmdLine->IsVisible()))
  {
    int NeedType = Mode == MACRO_EDITOR?MODALTYPE_EDITOR:(Mode == MACRO_VIEWER?MODALTYPE_VIEWER:(Mode == MACRO_DIALOG?MODALTYPE_DIALOG:MACRO_SHELL));
    Frame* CurFrame=FrameManager->GetCurrentFrame();
    if (CurFrame && CurFrame->GetType()==NeedType)
    {
      int CurSelected;
      if(Mode==MACRO_SHELL && CtrlObject->CmdLine->IsVisible())
        CurSelected=CtrlObject->CmdLine->ProcessKey(KEY_MACRO_SELECTED);
      else
        CurSelected=CurFrame->ProcessKey(KEY_MACRO_SELECTED);

      if((CurFlags&MFLAGS_EDITSELECTION) && !CurSelected ||
         (CurFlags&MFLAGS_EDITNOSELECTION) && CurSelected)
          return FALSE;
    }
  }
  return TRUE;
}

/* DJ $ */

BOOL KeyMacro::CheckInsidePlugin(DWORD CurFlags)
{
  if(CtrlObject && CtrlObject->Plugins.CurPluginItem && (CurFlags&MFLAGS_NOSENDKEYSTOPLUGINS)) // ?????
  //if(CtrlObject && CtrlObject->Plugins.CurEditor && (CurFlags&MFLAGS_NOSENDKEYSTOPLUGINS))
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

BOOL KeyMacro::CheckPanel(int PanelMode,DWORD CurFlags,BOOL IsPassivePanel)
{
  if(IsPassivePanel)
  {
    if(PanelMode == PLUGIN_PANEL && (CurFlags&MFLAGS_PNOPLUGINPANELS) ||
       PanelMode == NORMAL_PANEL && (CurFlags&MFLAGS_PNOFILEPANELS))
      return FALSE;
  }
  else
  {
    if(PanelMode == PLUGIN_PANEL && (CurFlags&MFLAGS_NOPLUGINPANELS) ||
       PanelMode == NORMAL_PANEL && (CurFlags&MFLAGS_NOFILEPANELS))
      return FALSE;
  }
  return TRUE;
}

BOOL KeyMacro::CheckFileFolder(Panel *CheckPanel,DWORD CurFlags, BOOL IsPassivePanel)
{
  char FileName[NM*2];
  int FileAttr=-1;
  CheckPanel->GetFileName(FileName,CheckPanel->GetCurrentPos(),FileAttr);
  if(FileAttr != -1)
  {
    if(IsPassivePanel)
    {
      if((FileAttr&FA_DIREC) && (CurFlags&MFLAGS_PNOFOLDERS) || !(FileAttr&FA_DIREC) && (CurFlags&MFLAGS_PNOFILES))
        return FALSE;
    }
    else
    {
      if((FileAttr&FA_DIREC) && (CurFlags&MFLAGS_NOFOLDERS) || !(FileAttr&FA_DIREC) && (CurFlags&MFLAGS_NOFILES))
        return FALSE;
    }
  }
  return TRUE;
}

BOOL KeyMacro::CheckAll(int CheckMode,DWORD CurFlags)
{
/* $TODO:
     ����� ������ Check*() ����������� ������� IfCondition()
     ��� ���������� �������������� ����.
*/
  if(!CheckInsidePlugin(CurFlags))
    return FALSE;

  // �������� �� �����/�� ����� � ���.������ (� � ���������? :-)
  if(CurFlags&(MFLAGS_EMPTYCOMMANDLINE|MFLAGS_NOTEMPTYCOMMANDLINE))
    if(!CheckCmdLine(CtrlObject->CmdLine->GetLength(),CurFlags))
      return FALSE;

  // �������� ������ � ���� �����
  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  Panel *PassivePanel=CtrlObject->Cp()->GetAnotherPanel(CtrlObject->Cp()->ActivePanel);
  if(ActivePanel && PassivePanel)// && (CurFlags&MFLAGS_MODEMASK)==MACRO_SHELL)
  {

    if(CurFlags&(MFLAGS_NOPLUGINPANELS|MFLAGS_NOFILEPANELS))
      if(!CheckPanel(ActivePanel->GetMode(),CurFlags,FALSE))
        return FALSE;

    if(CurFlags&(MFLAGS_PNOPLUGINPANELS|MFLAGS_PNOFILEPANELS))
      if(!CheckPanel(PassivePanel->GetMode(),CurFlags,TRUE))
        return FALSE;

    if(CurFlags&(MFLAGS_NOFOLDERS|MFLAGS_NOFILES))
      if(!CheckFileFolder(ActivePanel,CurFlags,FALSE))
        return FALSE;

    if(CurFlags&(MFLAGS_PNOFOLDERS|MFLAGS_PNOFILES))
      if(!CheckFileFolder(PassivePanel,CurFlags,TRUE))
        return FALSE;

    /* $ 20.03.2002 DJ
       ��� �������� - �� ������ �������� ���� �� ����
    */
    if(CurFlags&(MFLAGS_SELECTION|MFLAGS_NOSELECTION|MFLAGS_PSELECTION|MFLAGS_PNOSELECTION))
      if(Mode!=MACRO_EDITOR && Mode != MACRO_DIALOG && Mode!=MACRO_VIEWER)
      {
        int SelCount=ActivePanel->GetRealSelCount();
        if((CurFlags&MFLAGS_SELECTION) && SelCount < 1 ||
           (CurFlags&MFLAGS_NOSELECTION) && SelCount >= 1)
          return FALSE;

        SelCount=PassivePanel->GetRealSelCount();
        if((CurFlags&MFLAGS_PSELECTION) && SelCount < 1 ||
           (CurFlags&MFLAGS_PNOSELECTION) && SelCount >= 1)
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
  if(Work.Executing && Work.MacroWORK)
  {
    return (Work.MacroWORK->Flags&Flags)?TRUE:FALSE;
  }
  return(FALSE);

}

/*
  Return: 0 - �� � ������ �����, 1 - Executing, 2 - Executing common, 3 - Recording, 4 - Recording common
  See farconst.hpp::MacroRecordAndExecuteType
*/
int KeyMacro::GetCurRecord(struct MacroRecord* RBuf,int *KeyPos)
{
  if(KeyPos && RBuf)
  {
    *KeyPos=Work.Executing?Work.ExecLIBPos:0;
    memset(RBuf,0,sizeof(struct MacroRecord));
    if(Recording == MACROMODE_NOMACRO)
    {
      if(Work.Executing)
      {
        memcpy(RBuf,MacroLIB+Work.MacroPC,sizeof(struct MacroRecord)); //????
        return Work.Executing;
      }
      memset(RBuf,0,sizeof(struct MacroRecord));
      return MACROMODE_NOMACRO;
    }
    RBuf->BufferSize=RecBufferSize;
    RBuf->Buffer=RecBuffer;
    return Recording==MACROMODE_RECORDING?MACROMODE_RECORDING:MACROMODE_RECORDING_COMMON;
  }
  return Recording?(Recording==MACROMODE_RECORDING?MACROMODE_RECORDING:MACROMODE_RECORDING_COMMON):(Work.Executing?Work.Executing:MACROMODE_NOMACRO);
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
  qsort(MacroLIB,
        MacroLIBCount,
        sizeof(struct MacroRecord),
        (qsort_fn)SortMacros);
  // ������������� ������ �����
  struct MacroRecord *MPtr;
  int I,J;
  int CurMode=MACRO_OTHER;
  memset(IndexMode,0,sizeof(IndexMode));
  for(MPtr=MacroLIB,I=0; I < MacroLIBCount; ++I,++MPtr)
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

DWORD KeyMacro::GetOpCode(struct MacroRecord *MR,int PC)
{
  DWORD OpCode=(MR->BufferSize > 1)?MR->Buffer[PC]:(DWORD)MR->Buffer;
  return OpCode;
}

// ������ OpCode � �����. ���������� ���������� ��������
DWORD KeyMacro::SetOpCode(struct MacroRecord *MR,int PC,DWORD OpCode)
{
  DWORD OldOpCode;
  if(MR->BufferSize > 1)
  {
    OldOpCode=MR->Buffer[PC];
    MR->Buffer[PC]=OpCode;
  }
  else
  {
    OldOpCode=(DWORD)MR->Buffer;
    MR->Buffer=(DWORD*)OpCode;
  }
  return OldOpCode;
}

// ��� ��� ����� ��� ���:
// BugZ#873 - ACTL_POSTKEYSEQUENCE � ��������� ����
int KeyMacro::IsExecutingLastKey()
{
  if(Work.Executing && Work.MacroWORK)
  {
    return (Work.ExecLIBPos == Work.MacroWORK->BufferSize-1);
  }
  return FALSE;
}

void KeyMacro::DropProcess()
{
  if(Work.Executing)
  {
    if(LockScr) delete LockScr;
    LockScr=NULL;
    UsedInternalClipboard=0; //??
    Work.Executing=MACROMODE_NOMACRO;
    ReleaseWORKBuffer();
  }
}

static char __code2symbol(BYTE b1, BYTE b2)
{
  if(b1 >= '0' && b1 <= '9') b1-='0';
  else { b1&=~0x20; b1=b1-'A'+10; }
  if(b2 >= '0' && b2 <= '9') b2-='0';
  else { b2&=~0x20; b2=b2-'A'+10; }
  return ((b1<<4)&0x00F0)|(b2&0x000F);
}

static const char* ParsePlainText(char *CurKeyText, const char *BufPtr)
{
  char *PtrCurKeyText=CurKeyText;
  ++BufPtr;
  while (*BufPtr)
  {
    if(*BufPtr == '\\')
    {
      switch(BufPtr[1])
      {
        case '\\':
          *PtrCurKeyText++='\\';
          break;
        case '"':
          *PtrCurKeyText++='"';
          break;
        case 'n':
          *PtrCurKeyText++='\n';
          break;
        case 't':
          *PtrCurKeyText++='\t';
          break;
        case 'x':
          BufPtr+=2;
          if(isxdigit(BufPtr[0]) && isxdigit(BufPtr[1]))
            *PtrCurKeyText++=__code2symbol((BYTE)BufPtr[0],(BYTE)BufPtr[1]);
          break;
        default:
          *PtrCurKeyText++=BufPtr[1];
          break;
      }
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
  return BufPtr;
}

static const char *__GetNextWord(const char *BufPtr,char *CurKeyText)
{
   // ���������� ������� ���������� �������
   while (IsSpace(*BufPtr) || IsEol(*BufPtr))
     BufPtr++;
   if (*BufPtr==0)
     return NULL;

   const char *CurBufPtr=BufPtr;

   // ���� ����� ���������� �������� �������
   while (*BufPtr && !(IsSpace(*BufPtr) || IsEol(*BufPtr)))
     BufPtr++;
   int Length=BufPtr-CurBufPtr;

   memcpy(CurKeyText,CurBufPtr,Length);
   CurKeyText[Length]=0;
   return BufPtr;
}

int KeyMacro::PushState()
{
  if(CurPCStack+1 >= STACKLEVEL)
    return FALSE;
  ++CurPCStack;
  memcpy(PCStack+CurPCStack,&Work,sizeof(struct MacroState));
  memset(&Work,0,sizeof(struct MacroState));
  return TRUE;
}

int KeyMacro::PopState()
{
  if(CurPCStack < 0)
    return FALSE;
  memcpy(&Work,PCStack+CurPCStack,sizeof(struct MacroState));
  CurPCStack--;
  return TRUE;
}
