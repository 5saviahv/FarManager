/*
viewer.cpp

Internal viewer

*/

/* Revision: 1.151 20.10.2003 $ */

/*
Modify:
  20.10.2003 SVS
    ! ��������������
        KEY_MACRO_EDITSELECTED -> KEY_MACRO_SELECTED
        KEY_MACRO_CHECKEOF     -> KEY_MACRO_EOF
    + ��������� KEY_MACRO_EMPTY � KEY_MACRO_BOF
  14.10.2003 SVS
    ! NamesList::GetCurDir - ����� ���. �������� - ��������� ������.
  15.10.2003 KM
    - �� �������� "�������" ������� ��� ������ "Search"
  03.10.2003 SVS
    + ��������� KEY_MACRO_EDITSELECTED � KEY_MACRO_CHECKEOF �� �������
  22.09.2003 KM
    + GlobalSearchHex - ���������� ����������, �������� ��������
      "Search for hex" ��� ������
    ! ����������� ������� ������ � ������ hex.
  11.07.2003 SVS
    - BugZ#898 - ����������� ����������� � ������� ������������� � HEX-������ �������...
  02.06.2003 VVM
    + ������ ��������� ��� ���������� ������� �� Ctrl-Down. Ctrl-Up �� ����
  29.05.2003 SVS
    - BugZ#912 - ������� FAR � ���������� ����� ����....
  21.05.2003 VVM
    ! ������� ��������� ��� ���������� ����.
  17.05.2003 VVM
    - � ������ Hex-��������� 80�25 ��������� ������� ��������� ������.
  16.05.2003 VVM
    - CTRL+U �� �������� ��� ����������� ���������� ������.
  14.05.2003 VVM
    + ViOpt.PersistentBlocks - ���������� ����� �� ������.
  13.05.2003 VVM
    - ����������� ����������� ��������� ����� ������ ��� ���������� ��������
      ���� � ������ ���������.
  12.05.2003 VVM
    + ��� ���������� ��������� ������ "������" ��������� ��� ����������� �� �����.
  07.05.2003 VVM
    - ��� 16-��� ��������� �� ������� ��������� ����.
  06.05.2003 SVS
    ! ������ mprintf("<>"); �������� BoxText(), ������� � ����������� ��
      Opt.UseUnicodeConsole ����� �������� 0xbb/0xab ��� >/<
  24.04.2003 VVM
    + ����� ������� ShowDown() ������������ ��� ������� �� "������� ����"
  15.04.2003 SVS
    - ����� � ��������� ������ ���� �� ������� ������ ������ ���������,
      ����� ���� ��������� ����� � ��������� ��������� �� ������ �����.
  15.04.2003 VVM
    ! ������ ������ �� 1/4 ������, � �� �� 1/3
    ! ����� ������ � ������ ����� �� ���������� ���������� �����.
  08.03.2003 IS
    + ������ ��������� ������� ����� ������ ��� ��������� ��� ����������
      unicode, �.�. ��� ������ ��� ��������� unicode<->������������ ���������
  25.02.2003 SVS
    ! "free/malloc/realloc -> xf_*" - ���-�� � ������� ��� ���������.
    -  "...��� ������� ��� ������ yes � no � ���������
        "Continue the search from the beginning of the document?"..."
  25.02.2003 SVS
    + SelectFlags, GetSelectedParam - ��� �� ���� ����������� ������������ ���������
  20.02.2003 IS
    ! ������� ���������� ����, �.�. ����� ��������� ��� "������
      ��������������" �� ����� - ���� �� ���� ������������ ���������,
      ������������ � 2 ���� � ������ �������, �� � ������� �����
      ������ �� ��������� ��������� �����.
    - ������ � SetFileSize - � ������ ������� ������ �� ���������� � 2
      ����, � ��� ��� ���, ��� ����� ���� ��������� FilePos (��� ��������
      � 2 ���� �����������, �������) � FileSize!
  19.02.2003 IS
    - ������ ��� ��������� VCTL_SETPOSITION: � ������ � �������� �����
      ���������� ViewerSetPosition.StartPos ��� ����������� � ��� ����
      ������ ����������� �������� (�.�. � GoTo StartPos ����������� � 2
      ����)
  03.02.2003 VVM
    ! ����� � ������ ����� ����� ����� �� ������ ���������
    + Alt-F7 - ���������� ����� � "��������" �����������
    ! ���� �� ����� ��������� ������ �� ����� - �� ����������
      ���������� ����� � ������/����� ����� (��������� STACK_OVERFLOW)
  29.01.2003 VVM
    + CtrlU �� ������ ���������� ���������, �� � ���������� ������ "������ ������"
      �� ������ ������.
  28.01.2003 VVM
    ! ������� ������� Home/End � CtrlHome/CtrlEnd
    ! SVS> �������� ���� ���� ����������������!
  27.01.2003 VVM
    ! ��������� �����, ���������� ������ �� ������������ ���������! :)
    + ��� ���������� ����� ����� ���������� "� �� �������� �� � ������ �����?"
    - �������� ���� � ���������� ��� �������� �����
    - �������� ��� � ���������� ������ ������� ">" ��� �������� �����.
  26.01.2003 IS
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  25.01.2003 VVM
    - ��� ��������� ����� LeftPos ������ ���������� = 0
    + ������� Home/End ������������� �� ������/����� ����� �� ������
  24.01.2003 KM
    ! �� ��������� ������ �������� �� ����� ������ ��
      ����� ������������ ������. � �� �� �������������
      �������� ���������� ��������� ���������.
  24.01.2003 VVM
    - ���������� ���� � ����� ������� ����� ���������� ����������� �����������
  23.01.2003 VVM
    + ��������� �� ������������ ����� ����������� �� �����.
    + ����� ��������� CTRL+U - �������� ���������.
    - � ������ hex-��������� ��� ������ �� RIGHT � ������ ��������� "������ �������"
      ������������ ����� ������ ��� hex-���������.
  07.01.2003 SVS
    - BugZ#761 - Searching window disappears
  28.12.2002 IS
    - ����, ����� �� �������� :-(
      ����� ����������� ��������� VCTL_QUIT ��-�������� �� �������, ����
      ��� ������� ���� ������ �������� ���������
  27.12.2002 IS
    - ��� ����������� �� 05.10.2002 ������� ��������� VCTL_QUIT ������.
      ��������� ���������� VCTL_QUIT ������ ��� ������, �������
      �� �������� ������� ���������� � �������� ���������
  23.12.2002 SVS
    - ���� � ��������� "������ ���������� �����" � QView
    ! �����, ��� � ��� __int64 ������ ��� ����� FARINT64 �� ��������� ����������
  21.12.2002 SVS
    - BugZ#742/2 - �������� � Qview:
       ����� ����� �� ������ � ���������� Qview 100% �� ��������� �����
       � ���� Qview ����� ������� "������ �������� �����"
      �������� ��� � ��� ����� - ��� �������� ����� ����� 0
    - �������������������� ����������
  17.12.2002 SVS
    ! Viewer64. ��� �������� �������� � ������� ��������� � __int64, ���
      ��������� ����������� �������� ������� ������ �����������
      ��������������� ������.
    ! ��������� ������� ��� HEX-������, �� ���� ���������� ������.
    ! ��������� ������ �������� - ��������� ���������� 13 ���� �������,
      ��� �������� ������������� ������� ~932 Gb
  05.10.2002 IS
    - ���: ��������������� ��������� ����� ��������� VCTL_QUIT, ���� ������
      �������� �������. �������: VCTL_QUIT � ���� ������ �� ��������������
      ������.
  01.10.2002 SVS
    - BugZ#664 - ������ "&" � ������ ��������� � ������� ������
  29.09.2002 IS
    - �� �������������� VCTL_QUIT
  27.08.2002 SVS
    - BugZ#603 - F3 - F6 ������� ��������
  15.08.2002 IS
    - ������������� ���: ��� �������� (�� HEX) ������, ���� ��������� ������
      �� �������� ������� ������, �������� ����� �� ���� ��� ������ - �����
      ��������� ��������� End (� ��������) �� ����� ������ ���������� ��
      ��������� Down.
  01.08.2002 tran
    ! � hex ������ ��� ������ unicode �� ����������� ����� ��� �������� �������
  26.07.2002 IS
    ! ��������������� Unicode �� ������ �������� �� �����
      "��������������� ������� ��������", �.�. Unicode �� ����
      _������� ��������_ ��� �������������.
  25.06.2002 SVS
    ! ���������:  BitFlags::Skip -> BitFlags::Clear
    ! � ��������� (Editor) � ������� (Viewer) ������������ �� ���. ��������
      �������� � ������ ������������� (�.�. ������ ScrX/ScrY ���������
      ObjWidth/ObjHeight).
    ! ���� ��������/������ �� � FullScreen, �� �� ���������� ������
  14.06.2002 IS
    + ��������� DeleteFolder (��. TempViewName)
  02.06.2002 KM
    - ��������� ������ �� ����� ������. ����������� ���������.
  28.05.2002 SVS
    - ������� ��������� �� ������� ��� �������, ����� ���������������� ������
      �� 24 ������ � ScrY
  27.05.2002 SVS
    ! ����� FileViewer -> Viewer �� ������ ������ (�.�. � Info � QView �����
      ������������ ���������)
  24.05.2002 SVS
    ! ��������� ������� � Numpad - ��� �������� ���������.
  24.05.2002 SVS
    + ������������ Numpad-������
  22.05.2002 SVS
    ! Viewer -> FileViewer
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
  11.05.2002 SVS
    - Bug: viewer find. �� ������� �������� �������� �����, ���������� ����
      ������, �� ������ ������ � ������ ������ �� ��������.
      ���������� ��-�� ����, ��� ����� ������� Dlg.Show... �� �������
      InitDialog()
  06.05.2002 SVS
    - BugZ#493 - �� �������� ����� Goto � viewer ��� ����� ������ � ���������
  28.04.2002 IS
    ! ����� "������ ����� �����" �������� ��� ��������� ����, �����
      ������� ����� "������ 16-������ ���"
  10.03.2002 tran
    ! Bugz#275 - CtrlS ������ �������� ���������
  01.03.2002 SVS
    ! ���� ������ ���� ������� �������� ���������� ����� - FarMkTempEx
  15.02.2002 SVS
    ! ����� ShowProcessList() ������� � �������
  22.01.2002 IS
    ! ��� ��������� ��������� � ����� ������ ��� ������� ���������� �� ��� ��
      ��� � ��� ������� �� ������ (������� ���������� �� bz#250)
  16.01.2002 SVS
    ! ��� ������ ������� FILE_SHARE_DELETE - ���� ����� ����� ����
  14.01.2002 IS
    ! chdir -> FarChDir
    ! ���� ����������� � ������ FILE_SHARE_DELETE
  28.12.2001 SVS
    ! ����� ������� ���������������� � ������������ ����� Hex � ���� ������.
  12.12.2001 DJ
    ! ��������� ������ SVS �� ����������� VC++
  12.12.2001 SVS
    - BugZ#147 - ������� ���� ��� ������ ������� ������.
    ! ��������� ������� ���������� ��������� ��� ������ (��� �� ������� �
      ������� - "������...")
  11.12.2001 SVS
    ! ����������� �� �������������� ���� ��� ������ - ������ ������ ��� �����
      MsgStr ��� ����� ������ ������, �.�., ���� ������ ������ ������ ������
      SEARCHSTRINGBUFSIZE-1, �� �������� ����� �� ������� �������, �.�. �
      MsgStr ����������� ������ \"%s\" (�.�. 2 ���. �������)!
      �������� �� ���� ������� � �������� ��� BugZ#147
  08.12.2001 OT
    - Bugzilla #144 ������� � �����, F4 �� �����, Ctrl-F10.
  27.11.2001 DJ
    - ��������� �� BoundsChecker
  12.10.2001 SKV
    - ���� �� status line'� ������������ ������ ���� �� ����
  11.10.2001 IS
    ! ������� ���� ������, ���� ��� �������� ������� � ����� ������.
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  20.09.01 IS
    - ���: ��� ��������� 0x0D �� ��������� ����� ������� ��� �������
  15.09.2001 tran
    * ������ ProcessViewerEvent ��������������
      �������� - ��� ���. (�� vetest.dll)
  06.09.2001 VVM
    ! ���� ��� ����������� � ��������� �������
  05.09.2001 VVM
    + ����������� ��������� � ���������
  20.08.2001 VVM
    ! ��������� ��������� � ������.
  26.07.2001 VVM
    + � ������ ������� ������ �� 1
  24.07.2001 IS
    ! ������ �������� �� ' ' � '\t' �� ����� isspace
    ! ������ �������� �� '\n' � '\r' �� ����� iseol
  10.07.2001 IS
    - ���: ��� ����������� ������ ��������� ������ ������� �� ��� ������
      (� ���������� � 732).
  25.06.2001 IS
    ! ��������� const
  25.06.2001 SVS
    ! ����� SEARCHSTRINGBUFSIZE
    + ������� ������ � ������ ������ :-)
  21.06.2001 SVS
    ! "���������" ���������� ������� ������� (�������, ����� ������ :-(
  08.06.2001
    - ���� � ��������� ������ ��-�� ������������� strcpy, � �� strncpy, �
      ���������� ���� ������ � ������.
  06.06.2001 SVS
    ! Mix/Max
  30.05.2001 tran
    * TempFileName unlink/rmdir -> DeleteFileWithFolder
  22.05.2001 DJ
    ! ������ ��������� ��������� D � Alt-F8
  16.05.2001 DJ
    ! proof-of-concept
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 DJ
    * ����� ������ ���������������� ����������, �������� �� _�����_ ������� ������
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  06.05.2001 DJ
    ! �������� #include
  04.05.2001 OT
    + ������� ������������� ���� �������� �� F11 (NWZ)
      ��������� PluginSet::CommandsMenu()
  28.04.2001 SVS
    - Opt.MsWheelDeltaEdit -> Opt.MsWheelDeltaView
  27.04.2001 DJ
    - ������ ��������� key bar ����� �������� �����
    * ��������� ���������� �������� � DrawScrollbar(), � ������������� ������
      � ����������� �� ��� ������� - � AdjustWidth()
    - ��������� ���������������� ����������, ����� ��������� ������ �������
  27.04.2001 VVM
    + ��������� KEY_MSWHEEL_XXXX
  29.03.2001 IS
    + ��������� ViOpt � Get/Set ��� �� ������������
  26.03.2001 SVS
    ! ������������� ScrollBar`� �� ��������� "�� ���������"
  26.03.2001 SVS
    + ��� ������ ������ �������� ������� �������� � ���, ����� �� �����
      ���������������� ����� "Viewer" ��� Shift-F1 �� ������ ��������
  22.03.2001 IS
    - ��� � ��������� �� alt-f8: ������� ���������� ������ �����, ����� �
      ������ �������� ������� ������ ��������� � ������ ����� (���� ��������
      �������� ���� �������� �� �������).
  13.02.2001 IS
    ! ��� ����������� ���� � ���������� � ��������� ������ (06.02.2001) �� ����
      ��, ��� ��������� ������ ���� ������ ��� ����� ������ ������, ������� ���
      �� �� ����� ��� ���������. ������ �������� ��� �����.
  27.02.2001 VVM
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
  20.02.2001 VVM
    + GetWrapType()/SetWrapType()
  06.02.2001 IS
   - ���� � ����������, ������� ��������, ����� ������ ����� ���������� �������
     � ��������� ������. ��. SelectPosOffSet � SelectText
  29.01.2001 IS
   - ��� - ��������� StructSize � ViewerInfo
  28.01.2001
   - ����� �������� ViewFile �� NULL � ProcessKey  ����������� �� �������
  23.01.2001 SVS
   + ���������� Info->LeftPos � VCTL_GETINFO.
  22.01.2001 IS
   !  ��������! ��������, ��� �� ������ ������ ������� ��������
      ��������� �� ��������, �� ��� ���� ������� � ������ �� ������.
      � ����������� SelectSize ���� _������_ � Process*
  21.01.2001 IS
   ! ��� ����������� � ���������� ������� ���� ��������:
      VCTL_SETPOS -> VCTL_SETPOSITION
      AnsiText -> AnsiMode
  19.01.2001 SVS
    ! ������� ����� ������� GoTo() - ��� �������������� ���������
    - ���������� ������� ����� ��� �������� (������� GoTo)
    + ������� ��������� SelectText() - ��� ��������������� �������
    + VCTL_SELECT
    ! ��������� � VCTL_SETPOS
    + �������:
       "����� ����������� ����� ��������� ����� Hex/����.
       �������� ��� ������� �����"
      ����� ��������� ������, ��� ���� ������� ��� ����������� :-)
      ������� ��� ���� ����� :-)
  11.01.2000 VVM
    ! ����� ���� ��������� �� ���, � �� ���������� �� +4
  21.12.2000 SVS
    ! �� ������� ������� HEX � ��������� �������, � ���������� (��� Unicode)
  16.12.2000 tran
    + ������ ����� �� ������ ���� (ProcessMouseEvent())
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  20.10.2000 tran
    + �������� ������� ������ � ������� (fffe)
  18.10.2000 SVS
    - ����: DownDownUp � ������-������ (FEFF)
  02.10.2000 SVS
    - ���� �� ����������.
      > ���� ������ � ����� ���� ���������, ����� ���������� �� ��������
      > ���� ������ ������� ������. ����� ������� ����� ������ �����.
  01.10.2000 IS
    ! ���������� ����� ����� � ��������� ������
  29.09.2000 SVS
    ! TableNum - 2
  27.09.2000 SVS
    + ViewerControl - (���� ������: �������� ������� �����������
      ���������� - ��� PrintMan)
    ! ���������� UseDecodeTable,TableNum,AnsiText,Unicode,Wrap, TypeWrap, Hex
      ������� � ���� ��������� ViewerMode.
  27.09.2000 SVS
    + ����� � ������������ Unicode ��� ��������� �� '+' & '-'
  24.09.2000 SVS
    + ������ �� ����������/�������������� ������� � ����� �� RCtrl+<N>
  19.09.2000 SVS
    ! FEFF-����� - ��������� ��������� ����������� � �������������.
  18.09.2000 SVS
    ! ��������� Warp � KeyBar
  14.09.2000 SVS
    + AutoDecode Unicode - �� �����, ������� ���������� � 0xFEFF
  13.09.2000 tran 1.23
    + ��� WWrap ���������� ������� � ������ ������
  12.09.2000 SVS
    ! ���������� Wrap/WWrap/UnWrap �� 2 ������������ -
      F2 ��������� (Wrap/UnWrap) � Shift-F2 ��� (Wrap/WWrap)
  10.09.2000 SVS
    ! ���������� ��������� ��� ������� �������
      ������������ ������ ����
  01.09.2000 SVS
    - ��������� ���� � �������� � ������� ������� ScrollBar`�
  31.08.2000 SVS
    + ������ FAR ������ ��� Wrap
    - ���� - ��� ������� � �� ScrollBar ������� ������������ ������� ������
  04.08.2000 KM
    ! !!!�������� ���������� ����!!!!
  01.08.2000 KM 1.16
    + �������� ����� �� "����� ������". �������� � ������
      � ������� �� Alt-F7.
  01.08.2000 tran 1.16
    + |DIF_USELASTHISTORY
  19.07.2000 tran 1.15
    + ��� ������� ��������� ������� ����������� �� 1
  18.07.2000 tran 1.14
    + ��������� �������� � ������� � ����������� �� ��������
      �������� ����� ��������� �� �����
      � ������������� Ctrl-S
  17.07.2000 tran
    + � �� ���������, 1.13 ������ :)
      ������ ������ �� alt-f8 ��������� ��
      nn% - ��������
      0xnn, nnh, nn$ - hex offset
      +/-nn - ������������� ��������
  15.07.2000 SVS
    ! Wrap ������ ������������ ���������, � �� �������
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  12.07.2000 tran
    ! OutStr are dynamic, new, delete,
      and sizeof(OutStr[i]) changed to MAX_VIEWLINEB
  12.07.2000 SVS
    + wrap ����� 3 ��������� :-) �� ������ ��� ������������������.
  11.07.2000 tran
    + wrap are now WORD-WRAP
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  10.07.2000 tran
    ! ���������� ����� ������ � 512 �� MAX_VIEWLINE (2048)
      ��������� �� ������ - 8, "512" �������� �� MAX_VIEWLINE,
      � "511" �� MAX_VIEWLINE-1
  04.07.2000 tran
    + ��������� warning � ������ OpenFile()
      ����� ��� QuickView
  03.07.2000 tran
    - ��� � �������� ������� ��������� ������ � hex
      (��������� ���� ������ ����� 2)
  30.06.2000 tran
    - ��� � ������� ����� � ����� �����,
      ���� �� ������������ ���� � temppanel (��������)
      � ����� ������ ���� ��� ����� ����, � ��������� ��� �� ����
  28.06.2000 IS (22.06.2000)
    + ���������� ������ ��� ����� �� ������
  28.06.2000 tran
    - ����� ������ ������ � hex viewer
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "viewer.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "lang.hpp"
#include "colors.hpp"
#include "keys.hpp"
#include "poscache.hpp"
#include "help.hpp"
#include "dialog.hpp"
#include "panel.hpp"
#include "filepanels.hpp"
#include "fileview.hpp"
#include "savefpos.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"

static void PR_ViewerSearchMsg(void);
static void ViewerSearchMsg(char *Name);

static struct CharTableSet InitTableSet;
static int InitUseDecodeTable=FALSE,InitTableNum=0,InitAnsiText=FALSE;

static int InitHex=FALSE,SearchHex=FALSE;
/* $ 27.09.2000 SVS
   ID ������� - �� �������� � Editor
*/
static int ViewerID=0;
/* SVS $*/

static char BorderLine[]={0x0B3,0x020,0x00};

Viewer::Viewer()
{
  _OT(SysLog("[%p] Viewer::Viewer()", this));
  /* $ 29.03.2001 IS
       "���������" ��������� ����������
  */

  memcpy(&ViOpt, &Opt.ViOpt, sizeof(ViewerOptions));
  /* IS $ */
  /* $ 12.07.2000 tran
     alloc memory for OutStr */
  for ( int i=0; i<=MAXSCRY; i++ )
  {
    OutStr[i]=new char[MAX_VIEWLINEB];
  }
  /* tran 12.07.2000 $ */
  strcpy((char *)LastSearchStr,GlobalSearchString);
  LastSearchCase=GlobalSearchCase;
  /* $ 01.08.2000 KM
     ���������� ��� ������ "Whole words"
  */
  LastSearchWholeWords=GlobalSearchWholeWords;
  /* KM $ */
  LastSearchReverse=GlobalSearchReverse;
  /* $ 22.09.2003 */
  LastSearchHex=GlobalSearchHex;
  /* KM $ */
  memcpy(&TableSet,&InitTableSet,sizeof(TableSet));
  VM.UseDecodeTable=InitUseDecodeTable;
  VM.TableNum=InitTableNum;
  VM.AnsiMode=InitAnsiText;

  if (VM.AnsiMode && VM.TableNum==0)
  {
    int UseUnicode=TRUE;
    GetTable(&TableSet,TRUE,VM.TableNum,UseUnicode);
    VM.TableNum=0;
    VM.UseDecodeTable=TRUE;
  }
  VM.Unicode=(VM.TableNum==1) && VM.UseDecodeTable;
  /* $ 31.08.2000 SVS
    �������� ��� �����
  */
  VM.Wrap=Opt.ViewerIsWrap;
  VM.TypeWrap=Opt.ViewerWrap;
  /* SVS $ */
  VM.Hex=InitHex;

  ViewFile=NULL;
  ViewKeyBar=NULL;
  *FileName=0;
  FilePos=0;
  LeftPos=0;
  SecondPos=0;
  FileSize=0;
  LastPage=0;
  SelectPos=SelectSize=0;
  LastSelPos=0;
  SetStatusMode(TRUE);
  HideCursor=TRUE;
  *TempViewName=0;
  DeleteFolder=TRUE;
  *Title=0;
  *PluginData=0;
  TableChangedByUser=FALSE;
  ReadStdin=FALSE;
  memset(&BMSavePos,0xff,sizeof(BMSavePos));
  memset(UndoData,0xff,sizeof(UndoData));
  LastKeyUndo=FALSE;
  InternalKey=FALSE;
  Viewer::ViewerID=::ViewerID++;
  CtrlObject->Plugins.CurViewer=this;
  OpenFailed=false;
  HostFileViewer=NULL;
  /* $ 06.02.2001 IS
     ��. SelectText
  */
  SelectPosOffSet=0;
  /* IS $ */
}


Viewer::~Viewer()
{
  KeepInitParameters();
  if (ViewFile)
  {
    fclose(ViewFile);
    if (Opt.SaveViewerPos)
    {
      char CacheName[NM*3];
      if (*PluginData)
        sprintf(CacheName,"%s%s",PluginData,PointToName(FileName));
      else
        strcpy(CacheName,FullFileName);
      unsigned int Table=0;
      if (TableChangedByUser)
      {
        Table=1;
        if (VM.AnsiMode)
          Table=2;
        else
          if (VM.Unicode)
            Table=3;
          else
            if (VM.UseDecodeTable)
              Table=VM.TableNum+3;
      }
      {
        struct /*TPosCache32*/ TPosCache64 PosCache={0};
        PosCache.Param[0]=FilePos;
        PosCache.Param[1]=LeftPos;
        PosCache.Param[2]=VM.Hex;
        //=PosCache.Param[3];
        PosCache.Param[4]=Table;
        if(Opt.SaveViewerShortPos)
        {
          PosCache.Position[0]=BMSavePos.SavePosAddr;
          PosCache.Position[1]=(__int64*)BMSavePos.SavePosLeft;
          //PosCache.Position[2]=;
          //PosCache.Position[3]=;
        }
        CtrlObject->ViewerPosCache->AddPosition(CacheName,&PosCache);
      }
    }
  }
  _tran(SysLog("[%p] Viewer::~Viewer, TempViewName=[%s]",this,TempViewName));
  /* $ 11.10.2001 IS
     ������� ���� ������, ���� ��� �������� ������� � ����� ������.
  */
  if (*TempViewName && !FrameManager->CountFramesWithName(TempViewName))
  /* IS $ */
  {
    /* $ 14.06.2002 IS
       ���� DeleteFolder �������, �� ������� ������ ����. ����� - ������� ���
       � �������.
    */
    if(DeleteFolder)
    {
      _tran(SysLog("call DeleteFileWithFolder for '%s'",TempViewName));
      DeleteFileWithFolder(TempViewName);
    }
    else
    {
      SetFileAttributes(TempViewName,0);
      remove(TempViewName);
    }
    /* IS $ */
  }
  /* $ 12.07.2000 tran
     free memory  */
  for ( int i=0; i<=MAXSCRY; i++ )
  {
    /* $ 13.07.2000 SVS
      ��� �� ������� new[], �� � ����� delete[]
    */
    delete[] OutStr[i];
    /* SVS $ */
  }
  /* tran 12.07.2000 $ */
  if (!OpenFailed)
  {
    CtrlObject->Plugins.CurViewer=this; //HostFileViewer;
    /* $ 15.09.2001 tran
       ���� ���������������� */
    CtrlObject->Plugins.ProcessViewerEvent(VE_CLOSE,&ViewerID);
    /* tran $ */
  }
}


void Viewer::KeepInitParameters()
{
  strcpy(GlobalSearchString,(char *)LastSearchStr);
  GlobalSearchCase=LastSearchCase;
  /* $ 01.08.2000 KM
     ���������� ��������� "Whole words" � ���������� GlobalSearchWholeWords
  */
  GlobalSearchWholeWords=LastSearchWholeWords;
  /* KM $ */
  GlobalSearchReverse=LastSearchReverse;
  /* $ 22.09.2003 KM */
  GlobalSearchHex=LastSearchHex;
  /* KM $ */
  memcpy(&InitTableSet,&TableSet,sizeof(InitTableSet));
  InitUseDecodeTable=VM.UseDecodeTable;
  InitTableNum=VM.TableNum;
  InitAnsiText=VM.AnsiMode;
  Opt.ViewerIsWrap=VM.Wrap;
  Opt.ViewerWrap=VM.TypeWrap;
  InitHex=VM.Hex;
}


int Viewer::OpenFile(const char *Name,int warning)
{
  FILE *NewViewFile=NULL;
  OpenFailed=false;
  strcpy(FileName,Name);
  SelectSize = 0; // ������� ���������

  if (CmdMode && strcmp(Name,"-")==0)
  {
    HANDLE OutHandle;
    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    {
      char TempName[NM];
      if (!FarMkTempEx(TempName))
      {
        OpenFailed=TRUE;
        return(FALSE);
      }
      OutHandle=FAR_CreateFile(TempName,GENERIC_READ|GENERIC_WRITE,
                FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,
                FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE,NULL);
      if (OutHandle==INVALID_HANDLE_VALUE)
      {
        OpenFailed=true;
        return(FALSE);
      }
      char ReadBuf[8192];
      DWORD ReadSize,WrittenSize;
      while (ReadFile(GetStdHandle(STD_INPUT_HANDLE),ReadBuf,sizeof(ReadBuf),&ReadSize,NULL))
        WriteFile(OutHandle,ReadBuf,ReadSize,&WrittenSize,NULL);
    }
    else
      OutHandle=GetStdHandle(STD_INPUT_HANDLE);
    int InpHandle=_open_osfhandle((long)OutHandle,O_BINARY);
    if (InpHandle!=-1)
      NewViewFile=fdopen(InpHandle,"rb");
    vseek(NewViewFile,0,SEEK_SET);
    ReadStdin=TRUE;
  }
  else
  {
    NewViewFile=NULL;

    DWORD Flags=0;
    DWORD ShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;
    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    {
      Flags|=FILE_FLAG_POSIX_SEMANTICS;
      ShareMode|=FILE_SHARE_DELETE;
    }

    HANDLE hView=FAR_CreateFile(Name,GENERIC_READ,
                            ShareMode,
                            NULL,OPEN_EXISTING,Flags,NULL);
    if (hView==INVALID_HANDLE_VALUE && Flags!=0)
      hView=FAR_CreateFile(Name,GENERIC_READ,
                       ShareMode,
                       NULL,OPEN_EXISTING,0,NULL);
    if (hView!=INVALID_HANDLE_VALUE)
    {
      int ViewHandle=_open_osfhandle((long)hView,O_BINARY);
      if (ViewHandle!=-1)
        NewViewFile=fdopen(ViewHandle,"rb");
    }
  }

  if (NewViewFile==NULL)
  {
    /* $ 04.07.2000 tran
       + 'warning' flag processing, in QuickView it is FALSE
         so don't show red message box */
    if (warning)
        Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MViewerTitle),
            MSG(MViewerCannotOpenFile),Name,MSG(MOk));
    /* tran 04.07.2000 $ */

    OpenFailed=true;
    return(FALSE);
  }
  if (ViewFile)
    fclose(ViewFile);
  TableChangedByUser=FALSE;
  ViewFile=NewViewFile;

//  ConvertNameToFull(FileName,FullFileName, sizeof(FullFileName));
  if (ConvertNameToFull(FileName,FullFileName, sizeof(FullFileName)) >= sizeof(FullFileName)){
    OpenFailed=false;
    return FALSE;
  }

  HANDLE ViewFindHandle;
  ViewFindHandle=FindFirstFile(FileName,&ViewFindData);
  FindClose(ViewFindHandle);

  /* $ 19.09.2000 SVS
    AutoDecode Unicode
  */
  BOOL IsDecode=FALSE;

  /* $ 26.07.2002 IS
       ��������������� Unicode �� ������ �������� �� �����
       "��������������� ������� ��������", �.�. Unicode �� ����
       _������� ��������_ ��� �������������.
  */
  //if(ViOpt.AutoDetectTable)
  /* IS $ */
  {
    VM.Unicode=0;
    FirstWord=0;
    vseek(ViewFile,0,SEEK_SET);
    vread((char *)&FirstWord,sizeof(FirstWord),ViewFile);
    //if(ReadSize == sizeof(FirstWord) &&
    if(FirstWord == 0x0FEFF || FirstWord == 0x0FFFE)
    {
      VM.AnsiMode=VM.UseDecodeTable=0;
      VM.Unicode=1;
      TableChangedByUser=TRUE;
      IsDecode=TRUE;
    }
  }
  /* SVS $ */

  if (Opt.SaveViewerPos && !ReadStdin)
  {
    __int64 NewLeftPos,NewFilePos;
    int Table;
    char CacheName[NM*3];
    if (*PluginData)
      sprintf(CacheName,"%s%s",PluginData,PointToName(FileName));
    else
      strcpy(CacheName,FileName);

    {
      struct /*TPosCache32*/ TPosCache64 PosCache={0};
      if(Opt.SaveViewerShortPos)
      {
        PosCache.Position[0]=BMSavePos.SavePosAddr;
        PosCache.Position[1]=(__int64*)BMSavePos.SavePosLeft;
        //PosCache.Position[2]=;
        //PosCache.Position[3]=;
      }
      CtrlObject->ViewerPosCache->GetPosition(CacheName,&PosCache);
      NewFilePos=PosCache.Param[0];
      NewLeftPos=PosCache.Param[1];
      VM.Hex=(int)PosCache.Param[2];
      //=PosCache.Param[3];
      Table=(int)PosCache.Param[4];
    }

    if(!IsDecode)
    {
      TableChangedByUser=(Table!=0);
      switch(Table)
      {
        case 0:
          break;
        case 1:
          VM.AnsiMode=VM.UseDecodeTable=VM.Unicode=0;
          break;
        case 2:
          {
            VM.AnsiMode=TRUE;
            VM.UseDecodeTable=TRUE;
            VM.Unicode=0;
            VM.TableNum=0;
            int UseUnicode=TRUE;
            GetTable(&TableSet,TRUE,VM.TableNum,UseUnicode);
          }
          break;
        case 3:
          VM.AnsiMode=VM.UseDecodeTable=0;
          VM.Unicode=1;
          break;
        default:
          VM.AnsiMode=VM.Unicode=0;
          VM.UseDecodeTable=1;
          VM.TableNum=Table-3;
          PrepareTable(&TableSet,Table-5);
          break;
      }
    }
    LastSelPos=FilePos=NewFilePos;
    LeftPos=NewLeftPos;
  }
  else
    FilePos=0;
  SetFileSize();
  if (FilePos>FileSize)
    FilePos=0;
  SetCRSym();
  if (ViOpt.AutoDetectTable && !TableChangedByUser)
  {
    VM.UseDecodeTable=DetectTable(ViewFile,&TableSet,VM.TableNum);
    if (VM.TableNum>0)
      VM.TableNum++;
    if (VM.Unicode)
    {
      VM.Unicode=0;
      FilePos*=2;
      SetFileSize();
    }
    /* $ 27.04.2001 DJ
       ������ ��������� keybar ����� �������� �����;
       ���������� ������ - � ��������� �������
    */
    if (VM.AnsiMode)
      VM.AnsiMode=FALSE;
  }
  ChangeViewKeyBar();
  AdjustWidth();
  /* DJ $ */
  CtrlObject->Plugins.CurViewer=this; // HostFileViewer;
  /* $ 15.09.2001 tran
     ���� ���������������� */
  CtrlObject->Plugins.ProcessViewerEvent(VE_READ,NULL);
  /* tran $ */
  return(TRUE);
}


/* $ 27.04.2001 DJ
   ������� ���������� ������ � ����������� �� ������� ����������
*/

void Viewer::AdjustWidth()
{
  /* $ 19.07.2000 tran
    + ���������� ������� */
  Width=X2-X1+1;
  XX2=X2;

  if ( ViOpt.ShowScrollbar )
  {
     Width--;
     XX2--;
  }
  /* tran 19.07.2000 $ */
}

/* DJ $ */

void Viewer::SetCRSym()
{
  if(!ViewFile)
    return;

  char Buf[2048];
  int CRCount=0,LFCount=0;
  int ReadSize,I;
  vseek(ViewFile,0,SEEK_SET);
  ReadSize=vread(Buf,sizeof(Buf),ViewFile);
  for (I=0;I<ReadSize;I++)
    switch(Buf[I])
    {
      case 10:
        LFCount++;
        break;
      case 13:
        if (I+1>=ReadSize || Buf[I+1]!=10)
          CRCount++;
        break;
    }
  if (LFCount<CRCount)
    CRSym=13;
  else
    CRSym=10;
}


void Viewer::DisplayObject()
{
  int Y,I;
  __int64 SelPos,SelSize;
  __int64 SaveSelectSize=SelectSize;

  /* $ 27.04.2001 DJ
     ���������� ������ - � ��������� �������
  */
  AdjustWidth();
  /* DJ $ */

  /* $ 04.07.2000 tran
    + ����� ������ "Cannot open the file" ������� ������
      � ������ ������������� ��� �������� */
  if (ViewFile==NULL)
  {
    /* BugZ#742/2 - �������� � Qview
       ����� ����� �� ������ � ���������� Qview 100% ��
       ��������� ����� � ���� Qview ����� �������
       "������ �������� �����"

       �������� ��� � ��� ����� - ��� �������� ����� ����� 0
    */
    if(*FileName)
    {
      GotoXY(X1,Y1);
      SetColor(COL_WARNDIALOGTEXT);
      mprintf(MSG(MViewerCannotOpenFile));
    }
    return;
  }
  /* tran $ */

  CtrlObject->Plugins.CurViewer=this; //HostFileViewer;

  ViewY1=Y1+ShowStatusLine;

  if (HideCursor)
  {
    MoveCursor(79,ScrY);
    SetCursorType(0,10);
  }
  vseek(ViewFile,FilePos,SEEK_SET);
  if (SelectSize == 0)
    SelectPos=FilePos;

  if (VM.Hex)
    ShowHex();
  else
  {
    for (I=0,Y=ViewY1;Y<=Y2;Y++,I++)
    {
      StrFilePos[I]=vtell(ViewFile);
      if (Y==ViewY1+1 && !feof(ViewFile))
        SecondPos=vtell(ViewFile);
      ReadString(OutStr[I],-1,MAX_VIEWLINEB,&SelPos,&SelSize);
      SetColor(COL_VIEWERTEXT);
      GotoXY(X1,Y);
      int StrLength = strlen((char *)OutStr[I]);
      if (StrLength > LeftPos)
      {
        /* $ 18.10.2000 SVS
           -Bug: Down Down Up & ������ ������
        */
        if(VM.Unicode &&
             (FirstWord == 0x0FEFF || FirstWord == 0x0FFFE)
             && !I && !LeftPos && !StrFilePos[I])
          mprintf("%-*.*s",Width,Width,&OutStr[I][(int)LeftPos+1]);
        else
          mprintf("%-*.*s",Width,Width,&OutStr[I][(int)LeftPos]);
        /* SVS $*/
      }
      else
        mprintf("%*s",Width,"");

      if (SelSize && SelPos >= LeftPos)
      {
        int SelX1=X1+SelPos-LeftPos;
        /* $ 12.07.2000 SVS
           ! Wrap - ���� �����������
        */
        if (!VM.Wrap && SelPos > LeftPos &&
        /* SVS $ */
           SelX1+SaveSelectSize-1>XX2 && LeftPos<MAX_VIEWLINE
        /* $ 11.01.2000 VVM
           ����� ���� ��������� �� ���, � �� ���������� �� +4 */
           && (X1+SelPos+SaveSelectSize-XX2<MAX_VIEWLINE))
        /* VVM $ */
        {
          if (AdjustSelPosition)
          {
            LeftPos=X1+SelPos+SaveSelectSize-XX2;
            SelectSize=SaveSelectSize;
            AdjustSelPosition = FALSE;
            Show();
            return;
          }
        } /* if */
        else
        {
          SetColor(COL_VIEWERSELECTEDTEXT);
          GotoXY(SelX1,Y);
          if (SelSize>XX2-SelX1+1)
            SelSize=XX2-SelX1+1;
          /* $ 06.02.2001 IS
             ��. SelectText
          */
          if (SelSize>0)
            mprintf("%.*s",(int)SelSize,&OutStr[I][(int)(SelPos+SelectPosOffSet)]);
          /* IS $ */
        } /* else */
      }

      /* $ 18.07.2000 tran -
         �������� �����
      */
      if (StrLength > LeftPos + Width && ViOpt.ShowArrows)
      {
        GotoXY(XX2,Y);
        SetColor(COL_VIEWERARROWS);
        BoxText(Opt.UseUnicodeConsole?0xbb:'>');
      }
      if (LeftPos>0 && *OutStr[I]!=0  && ViOpt.ShowArrows)
      {
        GotoXY(X1,Y);
        SetColor(COL_VIEWERARROWS);
        BoxText(Opt.UseUnicodeConsole?0xab:'<');
      }
    } /* for */
  } // if (Hex)  - else


  /* $ 27.04.2001 DJ
     ��������� ���������� - � ��������� �������
  */
  DrawScrollbar();
  /* DJ $ */
  ShowStatus();
}


void Viewer::ShowHex()
{
  if(!ViewFile)
    return;
  char OutStr[MAX_VIEWLINE],TextStr[20];
  int SelPos,SelSize,EndFile;
  int Ch,Ch1,X,Y,TextPos;
  SelSize=0;

  int HexLeftPos=((LeftPos>80-ObjWidth) ? Max(80-ObjWidth,0):LeftPos);

  for (EndFile=0,Y=ViewY1;Y<=Y2;Y++)
  {
    SetColor(COL_VIEWERTEXT);
    GotoXY(X1,Y);
    if (EndFile)
    {
      mprintf("%*s",ObjWidth,"");
      continue;
    }

    if (Y==ViewY1+1 && !feof(ViewFile))
      SecondPos=vtell(ViewFile);
    sprintf(OutStr,"%010I64X: ",(__int64)ftell64(ViewFile));

    TextPos=0;

    if (VM.Unicode)
      for (X=0;X<8;X++)
      {
        if (SelectSize>0 && SelectPos==vtell(ViewFile))
        {
          SelPos=strlen(OutStr);
          SelSize=SelectSize;
          /* $ 22.01.2001 IS
              ��������! ��������, ��� �� ������ ������ ������� ��������
              ��������� �� ��������, �� ��� ���� ������� � ������ �� ������.
              � ����������� SelectSize ���� � Process*
          */
          //SelectSize=0;
          /* IS $ */
        }

        if ((Ch=getc(ViewFile))==EOF || (Ch1=getc(ViewFile))==EOF)
        {
          /* $ 28.06.2000 tran
             ������� ����� ������ ������, ���� �����
             ����� ������ 16 */
          EndFile=LastPage=1;
          if ( X==0 )
          {
             strcpy(OutStr,"");
             break;
          }
          strcat(OutStr,"     ");
          TextStr[TextPos++]=' ';
          /* tran $ */
        }
        else
        {
          sprintf(OutStr+strlen(OutStr),"%02X%02X ",Ch1,Ch);
          char TmpBuf[2],NewCh;

          /* $ 01.08.2002 tran
          �������� ������� ������ */
          if ( FirstWord == 0x0FFFE )
          {
              TmpBuf[0]=Ch1;
              TmpBuf[1]=Ch;
          }
          else
          {
              TmpBuf[0]=Ch;
              TmpBuf[1]=Ch1;
          }
          /* tran $ */
          WideCharToMultiByte(CP_OEMCP,0,(LPCWSTR)TmpBuf,1,&NewCh,1," ",NULL);
          if (NewCh==0)
            NewCh=' ';
          TextStr[TextPos++]=NewCh;
          LastPage=0;
        }
        if (X==3)
          strcat(OutStr,BorderLine);
      }
    else
      for (X=0;X<16;X++)
      {
        if (SelectSize>0 && SelectPos==vtell(ViewFile))
        {
          SelPos=strlen(OutStr);
          SelSize=SelectSize;
          /* $ 22.01.2001 IS
              ��������! ��������, ��� �� ������ ������ ������� ��������
              ��������� �� ��������, �� ��� ���� ������� � ������ �� ������.
              � ����������� SelectSize ���� � Process*
          */
          //SelectSize=0;
          /* IS $ */
        }
        if ((Ch=vgetc(ViewFile))==EOF)
        {
          /* $ 28.06.2000 tran
             ������� ����� ������ ������, ���� �����
             ����� ������ 16 */
          EndFile=LastPage=1;
          if ( X==0 )
          {
             strcpy(OutStr,"");
             break;
          }
          /* $ 03.07.2000 tran
             - ������ 5 �������� ��� ���� 3 */
          strcat(OutStr,"   ");
          /* tran $ */
          TextStr[TextPos++]=' ';
          /* tran $ */
        }
        else
        {
          sprintf(OutStr+strlen(OutStr),"%02X ",Ch);
          if (Ch==0)
            Ch=' ';
          TextStr[TextPos++]=Ch;
          LastPage=0;
        }
        if (X==7)
          strcat(OutStr,BorderLine);
      }
    TextStr[TextPos]=0;
    if (VM.UseDecodeTable && !VM.Unicode)
      DecodeString(TextStr,(unsigned char *)TableSet.DecodeTable);
    strcat(TextStr," ");
    strcat(OutStr," ");
    strcat(OutStr,TextStr);
    if (strlen(OutStr)>HexLeftPos)
      mprintf("%-*.*s",ObjWidth,ObjWidth,OutStr+(int)HexLeftPos);
    else
      mprintf("%*s",ObjWidth,"");
    if (SelSize && SelPos>=HexLeftPos)
    {
      SetColor(COL_VIEWERSELECTEDTEXT);
      GotoXY((int)((__int64)X1+SelPos-HexLeftPos),Y);
      mprintf("%.*s",(VM.Unicode ? 4:2),OutStr+(int)SelPos);
      SelSize=0;
    }
  }
}

void Viewer::ShowUp()
{
  if(!ViewFile)
    return;

  int Y,I;

  /* $ 27.04.2001 DJ
     ���������� ������ - � ��������� �������
  */
  AdjustWidth();
  /* DJ $ */

  if (HideCursor)
    SetCursorType(0,10);
  vseek(ViewFile,FilePos,SEEK_SET);
  if (SelectSize == 0)
    SelectPos=FilePos;

  for (I=Y2-ViewY1-1;I>=0;I--)
  {
    StrFilePos[I+1]=StrFilePos[I];
    strcpy(OutStr[I+1],OutStr[I]);
  }
  StrFilePos[0]=FilePos;
  SecondPos=StrFilePos[1];

  ReadString(OutStr[0],(int)(SecondPos-FilePos),MAX_VIEWLINEB,NULL,NULL);

  for (I=0,Y=ViewY1;Y<=Y2;Y++,I++)
  {
    SetColor(COL_VIEWERTEXT);
    GotoXY(X1,Y);
    int StrLength = strlen(OutStr[I]);
    if (StrLength > LeftPos)
    {
      /* $ 18.10.2000 SVS
         -Bug: Down Down Up & ������ ������
      */
      if(VM.Unicode && (FirstWord == 0x0FEFF || FirstWord == 0x0FFFE) && !I && !LeftPos && !StrFilePos[I])
        mprintf("%-*.*s",Width,Width,&OutStr[I][(int)LeftPos+1]);
      else
        mprintf("%-*.*s",Width,Width,&OutStr[I][(int)LeftPos]);
      /* SVS $ */
    }
    else
      mprintf("%*s",Width,"");

    if (SelectPos >= StrFilePos[I] + LeftPos && SelectPos <= StrFilePos[I] + StrLength)
    {
      __int64 SelPos, SelSize;
      char OutStrTmp[MAX_VIEWLINEB];
      __int64 SavePos = vtell(ViewFile);
      int SaveLastPage = LastPage;
      vseek(ViewFile,StrFilePos[I],SEEK_SET);
      ReadString(OutStrTmp,-1,MAX_VIEWLINEB,&SelPos,&SelSize);
      vseek(ViewFile,SavePos,SEEK_SET);
      LastPage = SaveLastPage;
      int SelX1=(int)((__int64)X1+SelPos - LeftPos);
      if (SelPos - LeftPos < Width)
      {
        SetColor(COL_VIEWERSELECTEDTEXT);
        GotoXY(SelX1,Y);
        if (SelSize>XX2-SelX1+1)
          SelSize=XX2-SelX1+1;
        if (SelSize>0)
          mprintf("%.*s",(int)SelSize,&OutStr[I][(int)(SelPos+SelectPosOffSet)]);
      }
    }

    if (StrLength > LeftPos + Width && ViOpt.ShowArrows)
    {
      GotoXY(XX2,Y);
      SetColor(COL_VIEWERARROWS);
      BoxText(Opt.UseUnicodeConsole?0xbb:'>');
    }
    if (LeftPos>0 && *OutStr[I]!=0 && ViOpt.ShowArrows)
    {
      GotoXY(X1,Y);
      SetColor(COL_VIEWERARROWS);
      BoxText(Opt.UseUnicodeConsole?0xab:'<');
    }
  }

  /* $ 27.04.2001 DJ
     ��������� ���������� - � ��������� �������
  */
  DrawScrollbar();
  /* DJ $ */
  ShowStatus();
}

void Viewer::ShowDown()
{
  if(!ViewFile)
    return;

  int Y,I;

  /* $ 27.04.2001 DJ
     ���������� ������ - � ��������� �������
  */
  AdjustWidth();
  /* DJ $ */

  if (HideCursor)
    SetCursorType(0,10);

  for (I=0; I<Y2-ViewY1;I++)
  {
    StrFilePos[I]=StrFilePos[I+1];
    strcpy(OutStr[I],OutStr[I+1]);
  }
  FilePos = StrFilePos[0];
  SecondPos = StrFilePos[1];
  if (SelectSize == 0)
    SelectPos=FilePos;

  vseek(ViewFile,StrFilePos[Y2-ViewY1],SEEK_SET);
  ReadString(OutStr[Y2-ViewY1],-1,MAX_VIEWLINEB,NULL,NULL);
  StrFilePos[Y2-ViewY1] = vtell(ViewFile);
  ReadString(OutStr[Y2-ViewY1],-1,MAX_VIEWLINEB,NULL,NULL);

  for (I=0,Y=ViewY1;Y<=Y2;Y++,I++)
  {
    SetColor(COL_VIEWERTEXT);
    GotoXY(X1,Y);
    int StrLength = strlen(OutStr[I]);
    if (StrLength > LeftPos)
    {
      /* $ 18.10.2000 SVS
         -Bug: Down Down Up & ������ ������
      */
      if(VM.Unicode && (FirstWord == 0x0FEFF || FirstWord == 0x0FFFE) && !I && !LeftPos && !StrFilePos[I])
        mprintf("%-*.*s",Width,Width,&OutStr[I][(int)LeftPos+1]);
      else
        mprintf("%-*.*s",Width,Width,&OutStr[I][(int)LeftPos]);
      /* SVS $ */
    }
    else
      mprintf("%*s",Width,"");

    if (SelectPos >= StrFilePos[I] + LeftPos && SelectPos <= StrFilePos[I] + StrLength)
    {
      __int64 SelPos, SelSize;
      char OutStrTmp[MAX_VIEWLINEB];
      __int64 SavePos = vtell(ViewFile);
      int SaveLastPage = LastPage;
      vseek(ViewFile,StrFilePos[I],SEEK_SET);
      ReadString(OutStrTmp,-1,MAX_VIEWLINEB,&SelPos,&SelSize);
      vseek(ViewFile,SavePos,SEEK_SET);
      LastPage = SaveLastPage;
      int SelX1=(int)((__int64)X1+SelPos - LeftPos);
      if (SelPos - LeftPos < Width)
      {
        SetColor(COL_VIEWERSELECTEDTEXT);
        GotoXY(SelX1,Y);
        if (SelSize>XX2-SelX1+1)
          SelSize=XX2-SelX1+1;
        if (SelSize>0)
          mprintf("%.*s",(int)SelSize,&OutStr[I][(int)(SelPos+SelectPosOffSet)]);
      }
    }

    if (StrLength > LeftPos + Width && ViOpt.ShowArrows)
    {
      GotoXY(XX2,Y);
      SetColor(COL_VIEWERARROWS);
      BoxText(Opt.UseUnicodeConsole?0xbb:'>');

    }
    if (LeftPos>0 && *OutStr[I]!=0 && ViOpt.ShowArrows)
    {
      GotoXY(X1,Y);
      SetColor(COL_VIEWERARROWS);
      BoxText(Opt.UseUnicodeConsole?0xab:'<');
    }
  }

  /* $ 27.04.2001 DJ
     ��������� ���������� - � ��������� �������
  */
  DrawScrollbar();
  /* DJ $ */
  ShowStatus();
}

/* $ 27.04.2001 DJ
   ��������� ���������� - � ��������� �������
*/

void Viewer::DrawScrollbar()
{
  /* $ 18.07.2000 tran
     ��������� ��������� */
  if ( ViOpt.ShowScrollbar )
  {
    SetColor(COL_VIEWERSCROLLBAR);
    /* $ 27.04.2001 DJ
       ���� status line ���������, ������ ��������� �� ����� ����
    */
    ScrollBar(X2,ViewY1,Y2-ViewY1+1,(LastPage != 0? (!FilePos?0:100):ToPercent64(FilePos,FileSize)),100);
    /* DJ $ */
  }
  /* tran 18.07.2000 $ */
}

/* DJ $ */


void Viewer::ShowStatus()
{
  char Status[4096],Name[4096];
  if (!ShowStatusLine)
    return;
  /* $ 22.06.2000 IS
    ���������� ������ ��� ����� �� ������
    Was: strcpy(Name,*Title ? Title:FileName);
  */
  if(*Title)
    strcpy(Name,Title);
  else
  {
    /* $ 30.06.2000 tran
       - double path when show file from temp panel */
    if (!(FileName[1]==':' && FileName[2]=='\\'))
    {
      ViewNamesList.GetCurDir(Name,sizeof(Name)-1);
      if(*Name)
        AddEndSlash(Name);
      strcat(Name,FileName);
    }
    else
      strcpy(Name,FileName);
    /* tran $ */
  }
  /* IS $  */
  int NameLength=ScrX-43; //???41
  if (Opt.ViewerEditorClock && HostFileViewer!=NULL && HostFileViewer->IsFullScreen())
    NameLength-=6;
  if (NameLength<20)
    NameLength=20;
  /* $ 01.10.2000 IS
     ! ���������� ����� ����� � ��������� ������
  */
  TruncPathStr(Name,NameLength);
  /* IS $ */
  char *TableName;
  char TmpTableName[32];
  if (VM.Unicode)
    TableName="Unicode";
  else if (VM.UseDecodeTable)
  {
    strncpy(TmpTableName,TableSet.TableName,sizeof(TmpTableName));
    TableName=RemoveChar(TmpTableName,'&',TRUE);
  }
  else if (VM.AnsiMode)
    TableName="Win";
  else
    TableName="DOS";

  const char *StatusFormat="%-*s %10.10s %13I64u %7.7s %-4I64d %s%3d%%";
  sprintf(Status,StatusFormat,
          NameLength,Name,TableName,
          FileSize,MSG(MViewerStatusCol),LeftPos,
          Opt.ViewerEditorClock ? "":" ",
          (LastPage ? 100:ToPercent64(FilePos,FileSize)));
  SetColor(COL_VIEWERSTATUS);
  GotoXY(X1,Y1);
  /* $ 31.08.2000 SVS
     ���� - ��� ������� ������� ������������ ������� ������
  */
  mprintf("%-*.*s",Width+(ViOpt.ShowScrollbar?1:0),
                   Width+(ViOpt.ShowScrollbar?1:0),Status);
  /* SVS $ */
  if (Opt.ViewerEditorClock && HostFileViewer!=NULL && HostFileViewer->IsFullScreen())
    ShowTime(FALSE);
}


void Viewer::SetStatusMode(int Mode)
{
  ShowStatusLine=Mode;
  ViewY1=Y1+ShowStatusLine;
}


void Viewer::ReadString(char *Str,int MaxSize,int StrSize,__int64 *SelPos1,__int64 *SelSize1)
{
  int Ch, Ch2;
  __int64 SelPos=0, SelSize=0, OutPtr;

  /* $ 27.04.2001 DJ
     ���������� ������ - � ��������� �������
  */
  AdjustWidth();
  /* DJ $ */

  OutPtr=0;
  SelSize=0;
  if (VM.Hex)
  {
    OutPtr=vread(Str,VM.Unicode ? 8:16,ViewFile);
    Str[VM.Unicode ? 8:16]=0;
  }
  else
  {
    bool CRSkipped=false;
    while (1)
    {
      if (OutPtr>=StrSize-16)
        break;
      /* $ 12.07.2000 SVS
        ! Wrap - ���������������
      */
      if (VM.Wrap && OutPtr>XX2-X1)
      {
        /* $ 11.07.2000 tran
           + warp are now WORD-WRAP */
        __int64 SavePos=vtell(ViewFile);
        if ((Ch=vgetc(ViewFile))!=CRSym && (Ch!=13 || vgetc(ViewFile)!=CRSym))
        {
          vseek(ViewFile,SavePos,SEEK_SET);
          if (VM.TypeWrap && RegVer) // ������ ��� ������������������
          {
            if ( !IsSpace(Ch) && !IsSpace(Str[(int)OutPtr]))
            {
               __int64 SavePtr=OutPtr;
               /* $ 18.07.2000 tran
                  ������� � �������� wordwrap ������������ , ; > ) */
               while (OutPtr)
               {
                  Ch2=Str[(int)OutPtr];
                  if(IsSpace(Ch2) || Ch2==',' || Ch2==';' || Ch2=='>'|| Ch2==')')
                    break;
                  OutPtr--;
               }

               Ch2=Str[(int)OutPtr];
               if (Ch2==',' || Ch2==';' || Ch2==')' || Ch2=='>')
                   OutPtr++;
               else
                   while (IsSpace(Str[(int)OutPtr]) && OutPtr<=SavePtr)
                      OutPtr++;

               if (OutPtr)
               {
                  vseek(ViewFile,OutPtr-SavePtr,SEEK_CUR);
                  //
               }
               else
                  OutPtr=SavePtr;
            }
            /* tran 11.07.2000 $ */
            /* $ 13.09.2000 tran
               remove space at WWrap */
            __int64 savepos=vtell(ViewFile);
            while (IsSpace(Ch))
                Ch=vgetc(ViewFile);
            if ( vtell(ViewFile)!=savepos)
                vseek(ViewFile,-1,SEEK_CUR);
            /* tran 13.09.2000 $ */
          }// wwrap
          /* SVS $ */
        }
        break;
      }

      if (SelectSize > 0 && SelectPos==vtell(ViewFile))
      {
        SelPos=OutPtr;
        SelSize=SelectSize;
        /* $ 22.01.2001 IS
            ��������! ��������, ��� �� ������ ������ ������� ��������
            ��������� �� ��������, �� ��� ���� ������� � ������ �� ������.
            � ����������� SelectSize ���� � Process*
        */
        //SelectSize=0;
        /* IS $ */
      }

      if (MaxSize-- == 0)
        break;
      if ((Ch=vgetc(ViewFile))==EOF)
        break;
      if (Ch==CRSym)
        break;
      if (CRSkipped)
      {
        CRSkipped=false;
        Str[(int)OutPtr++]=13;
      }

      if (Ch=='\t')
      {
        do
        {
          Str[(int)OutPtr++]=' ';
        } while ((OutPtr % ViOpt.TabSize)!=0);
        /* $ 12.07.2000 SVS
          Wrap - 3-x ����������� � ���� ���� ����������� :-)
        */
        /* $ 22.01.2002 IS
           �� ������� � ��� ������� ������� �� �� ������
        */
        if ((VM.Wrap && (!VM.TypeWrap || (VM.TypeWrap && RegVer)))
        /* IS $ */
        /* SVS $ */
            && OutPtr>XX2-X1)
          Str[XX2-X1+1]=0;
        continue;
      }
      /* $ 20.09.01 IS
         ���: �� ��������� ����� ������� ��� �������
      */
      if (Ch==13)
      {
        CRSkipped=true;
        if(OutPtr>=XX2-X1)
        {
          __int64 SavePos=vtell(ViewFile);
          int nextCh=vgetc(ViewFile);
          if(nextCh!=CRSym && nextCh!=EOF) CRSkipped=false;
          vseek(ViewFile,SavePos,SEEK_SET);
        }
        if(CRSkipped)
           continue;
      }
      /* IS $ */
      if (Ch==0 || Ch==10)
        Ch=' ';
      Str[(int)OutPtr++]=Ch;
    }
  }
  Str[(int)OutPtr]=0;

  if (VM.UseDecodeTable && !VM.Unicode)
    DecodeString(Str,(unsigned char *)TableSet.DecodeTable);
  LastPage=feof(ViewFile);
  if(SelPos1) *SelPos1=SelPos;
  if(SelSize1) *SelSize1=SelSize;
}


/* $ 28.01.2001
   - ����� �������� ViewFile �� NULL ����������� �� �������
*/
int Viewer::ProcessKey(int Key)
{
  int I;
  char ReadStr[4096];

  /* $ 22.01.2001 IS
       ���������� �����-�� ����������� -> ������ ���������
  */
  if (!ViOpt.PersistentBlocks &&
      Key!=KEY_IDLE && Key!=KEY_NONE && !(Key==KEY_CTRLINS||Key==KEY_CTRLNUMPAD0) && Key!=KEY_CTRLC)
    SelectSize=0;
  /* IS $ */

  if (!InternalKey && !LastKeyUndo && (FilePos!=UndoData[0].UndoAddr || LeftPos!=UndoData[0].UndoLeft))
  {
    for (int I=sizeof(UndoData)/sizeof(UndoData[0])-1;I>0;I--)
    {
      UndoData[I].UndoAddr=UndoData[I-1].UndoAddr;
      UndoData[I].UndoLeft=UndoData[I-1].UndoLeft;
    }
    UndoData[0].UndoAddr=FilePos;
    UndoData[0].UndoLeft=LeftPos;
  }

  if (Key!=KEY_ALTBS && Key!=KEY_CTRLZ && Key!=KEY_NONE && Key!=KEY_IDLE)
    LastKeyUndo=FALSE;

  if (Key>=KEY_CTRL0 && Key<=KEY_CTRL9)
  {
    int Pos=Key-KEY_CTRL0;
    if (BMSavePos.SavePosAddr[Pos]!=-1)
    {
      FilePos=BMSavePos.SavePosAddr[Pos];
      LeftPos=BMSavePos.SavePosLeft[Pos];
//      LastSelPos=FilePos;
      Show();
    }
    return(TRUE);
  }
  if (Key>=KEY_CTRLSHIFT0 && Key<=KEY_CTRLSHIFT9)
    Key=Key-KEY_CTRLSHIFT0+KEY_RCTRL0;
  if (Key>=KEY_RCTRL0 && Key<=KEY_RCTRL9)
  {
    int Pos=Key-KEY_RCTRL0;
    BMSavePos.SavePosAddr[Pos]=FilePos;
    BMSavePos.SavePosLeft[Pos]=LeftPos;
    return(TRUE);
  }


  switch(Key)
  {
    case KEY_F1:
    {
      {
        Help Hlp ("Viewer");
      }
      return(TRUE);
    }

    case KEY_CTRLU:
    {
//      if (SelectSize)
      {
        SelectSize = 0;
        Show();
      }
      return(TRUE);
    }

    /* $ 05.09.2001 VVM
      + ����������� ��������� � ��������� */
    case KEY_CTRLC:
    case KEY_CTRLINS:  case KEY_CTRLNUMPAD0:
    {
      if (SelectSize && ViewFile)
      {
        char *SelData;
        int DataSize = (int)SelectSize+(VM.Unicode?2:1);
        __int64 CurFilePos=vtell(ViewFile);

        if ((SelData=(char*)xf_malloc(DataSize)) != NULL)
        {
          memset(SelData, 0, DataSize);
          vseek(ViewFile,SelectPos,SEEK_SET);
          vread(SelData, (int)SelectSize, ViewFile);
          if (VM.UseDecodeTable && !VM.Unicode)
            DecodeString(SelData, (unsigned char *)TableSet.DecodeTable);
          if (VM.Unicode)
            WideCharToMultiByte(CP_OEMCP,0,(LPCWSTR)(SelData),(int)SelectSize,SelData,(int)SelectSize," ",NULL);
          CopyToClipboard(SelData);
          xf_free(SelData);
          vseek(ViewFile,CurFilePos,SEEK_SET);
        } /* if */
      } /* if */
      return(TRUE);
    }
    /* VVM $ */

    /* $ 18.07.2000 tran
       ��������/��������� �������� */
    case KEY_CTRLS:
    {
        ViOpt.ShowScrollbar=!ViOpt.ShowScrollbar;
        /* $ 10.03.2002 tran
           Bugz#275 - CtrlS ������ �������� ��������� */
        Opt.ViOpt.ShowScrollbar=ViOpt.ShowScrollbar;
        /* tran $ */
        Show();
        return (TRUE);
    }
    /* tran 18.07.2000 $ */

    case KEY_IDLE:
    {
      {
        if(ViewFile)
        {
          char Root[NM];
          GetPathRoot(FullFileName,Root);
          int DriveType=GetDriveType(Root);
          if (DriveType!=DRIVE_REMOVABLE && DriveType!=DRIVE_CDROM)
          {
            HANDLE ViewFindHandle;
            WIN32_FIND_DATA NewViewFindData;
            ViewFindHandle=FindFirstFile(FullFileName,&NewViewFindData);
            if (ViewFindHandle==INVALID_HANDLE_VALUE)
              return(TRUE);
            FindClose(ViewFindHandle);
            fflush(ViewFile);
            vseek(ViewFile,0,SEEK_END);
            __int64 CurFileSize=vtell(ViewFile);
            if (ViewFindData.ftLastWriteTime.dwLowDateTime!=NewViewFindData.ftLastWriteTime.dwLowDateTime ||
                ViewFindData.ftLastWriteTime.dwHighDateTime!=NewViewFindData.ftLastWriteTime.dwHighDateTime ||
                CurFileSize!=FileSize)
            {
              ViewFindData=NewViewFindData;
              FileSize=CurFileSize;
              if (FilePos>FileSize)
                ProcessKey(KEY_CTRLEND);
              else
              {
                __int64 PrevLastPage=LastPage;
                Show();
                if (PrevLastPage && !LastPage)
                {
                  ProcessKey(KEY_CTRLEND);
                  LastPage=TRUE;
                }
              }
            }
          }
        }
        if (Opt.ViewerEditorClock && HostFileViewer!=NULL && HostFileViewer->IsFullScreen())
          ShowTime(FALSE);
      }
      return(TRUE);
    }

    case KEY_ALTBS:
    case KEY_CTRLZ:
    {
      for (int I=1;I<sizeof(UndoData)/sizeof(UndoData[0]);I++)
      {
        UndoData[I-1].UndoAddr=UndoData[I].UndoAddr;
        UndoData[I-1].UndoLeft=UndoData[I].UndoLeft;
      }
      if (UndoData[0].UndoAddr!=-1)
      {
        FilePos=UndoData[0].UndoAddr;
        LeftPos=UndoData[0].UndoLeft;
        UndoData[sizeof(UndoData)/sizeof(UndoData[0])-1].UndoAddr=-1;
        UndoData[sizeof(UndoData)/sizeof(UndoData[0])-1].UndoLeft=-1;

        Show();
//        LastSelPos=FilePos;
      }
      return(TRUE);
    }

    case KEY_ADD:
    case KEY_SUBTRACT:
    {
      if (*TempViewName==0)
      {
        char Name[NM];
        bool NextFileFound;

        if (Key==KEY_ADD)
          NextFileFound=ViewNamesList.GetNextName(Name);
        else
          NextFileFound=ViewNamesList.GetPrevName(Name);

        if (NextFileFound)
        {
          if (Opt.SaveViewerPos)
          {
            char CacheName[NM*3];
            if (*PluginData)
              sprintf(CacheName,"%s%s",PluginData,PointToName(FileName));
            else
              strcpy(CacheName,FileName);
            unsigned int Table=0;
            if (TableChangedByUser)
            {
              Table=1;
              if (VM.AnsiMode)
                Table=2;
              else
                if (VM.Unicode)
                  Table=3;
                else
                  if (VM.UseDecodeTable)
                    Table=VM.TableNum+3;
            }
            {
              struct /*TPosCache32*/ TPosCache64 PosCache={0};
              PosCache.Param[0]=FilePos;
              PosCache.Param[1]=LeftPos;
              PosCache.Param[2]=VM.Hex;
              //=PosCache.Param[3];
              PosCache.Param[4]=Table;
              if(Opt.SaveViewerShortPos)
              {
                PosCache.Position[0]=BMSavePos.SavePosAddr;
                PosCache.Position[1]=(__int64*)BMSavePos.SavePosLeft;
                //PosCache.Position[2]=;
                //PosCache.Position[3]=;
              }
              CtrlObject->ViewerPosCache->AddPosition(CacheName,&PosCache);
            }
          }
          if (PointToName(Name)==Name)
          {
            char ViewDir[NM];
            ViewNamesList.GetCurDir(ViewDir,sizeof(ViewDir)-1);
            if(*ViewDir)
              FarChDir(ViewDir);
          }
          /* $ 04.07.2000 tran
             + ��������� 'warning' � OpenFile � ������ ����� �� TRUE
          */
          if (OpenFile(Name,TRUE))
          /* tran $ */
          {
            LeftPos=0;
            SecondPos=0;
//            LastSelPos=FilePos;
            Show();
            ShowConsoleTitle();
          }
        }
      }
      return(TRUE);
    }

    case KEY_SHIFTF2:
    {
      if(RegVer)
      {
        VM.TypeWrap=!VM.TypeWrap;
        if(!VM.Wrap)
        {
          VM.Wrap=!VM.Wrap;
          LeftPos = 0;
        }
        ChangeViewKeyBar();
        Show();
        Opt.ViewerWrap=VM.TypeWrap;
//        LastSelPos=FilePos;
      }
      return TRUE;
    }

    case KEY_F2:
    {
      /* $ 12.07.200 SVS
        ! Wrap ����� 3 ��������� �...
      */
      VM.Wrap=!VM.Wrap;
      ChangeViewKeyBar();
      if (VM.Wrap)
        LeftPos = 0;
      Show();
      /* $ 31.08.2000 SVS
        ��������� ��� �����
      */
      Opt.ViewerIsWrap=VM.Wrap;
      /* SVS $ */
//      LastSelPos=FilePos;
      return(TRUE);
    }

    case KEY_F4:
    {
      VM.Hex=!VM.Hex;
      ChangeViewKeyBar();
      Show();
//      LastSelPos=FilePos;
      return(TRUE);
    }

    case KEY_F7:
    {
      Search(0,0);
      return(TRUE);
    }

    case KEY_SHIFTF7:
    case KEY_SPACE:
    {
      Search(1,0);
      return(TRUE);
    }

    case KEY_ALTF7:
    {
      SearchFlags.Set(REVERSE_SEARCH);
      Search(1,0);
      SearchFlags.Clear(REVERSE_SEARCH);
      return(TRUE);
    }

    case KEY_F8:
    {
      if ((VM.AnsiMode=!VM.AnsiMode)!=0)
      {
        int UseUnicode=TRUE;
        GetTable(&TableSet,TRUE,VM.TableNum,UseUnicode);
      }
      if (VM.Unicode)
      {
        FilePos*=2;
        VM.Unicode=FALSE;
        SetFileSize();
      }
      VM.TableNum=0;
      VM.UseDecodeTable=VM.AnsiMode;
      ChangeViewKeyBar();
      Show();
//      LastSelPos=FilePos;
      TableChangedByUser=TRUE;
      return(TRUE);
    }

    case KEY_SHIFTF8:
    {
      {
        int UseUnicode=TRUE;
        int GetTableCode=GetTable(&TableSet,FALSE,VM.TableNum,UseUnicode);
        if (GetTableCode!=-1)
        {
          /* $ 08.03.2003 IS
               ������ ��������� ������� ����� ������,
               �.�. ��� ������ ��� ���������
               unicode<->������������ ���������
          */
          bool oldIsUnicode=VM.Unicode;
          if (VM.Unicode && !UseUnicode)
            FilePos*=2;
          if (!VM.Unicode && UseUnicode)
            FilePos=(FilePos+(FilePos&1))/2;
          VM.UseDecodeTable=GetTableCode;
          VM.Unicode=UseUnicode;
          SetFileSize();
          VM.AnsiMode=FALSE;
          ChangeViewKeyBar();
          Show();
//          LastSelPos=FilePos;
          TableChangedByUser=TRUE;
          // IS: ���������� ������� ����� ������ ������,
          // IS: ���� �������� ��� ��������� ������
          if((oldIsUnicode && !VM.Unicode) || (!oldIsUnicode && VM.Unicode))
            SetCRSym();
          /* IS $ */
        }
      }
      return(TRUE);
    }

    case KEY_ALTF8:
    {
      if(ViewFile)
        GoTo();
      return(TRUE);
    }

    case KEY_F11:
    {
      CtrlObject->Plugins.CommandsMenu(MODALTYPE_VIEWER,0,"Viewer");
      Show();
      return(TRUE);
    }

    /* $ 27.06.2001 VVM
      + � ������ ������� �� 1 */
    /* $ 27.04.2001 VVM
      + ��������� KEY_MSWHEEL_XXXX */
    case KEY_MSWHEEL_UP:
    case (KEY_MSWHEEL_UP | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaView;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_UP);
      return(TRUE);
    }

    case KEY_MSWHEEL_DOWN:
    case (KEY_MSWHEEL_DOWN | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaView;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_DOWN);
      return(TRUE);
    }
    /* VVM $ */
    /* VVM $ */

    case KEY_UP: case KEY_NUMPAD8: case KEY_SHIFTNUMPAD8:
    {
      if (FilePos>0 && ViewFile)
      {
        Up();
        if (VM.Hex)
        {
          FilePos&=~(VM.Unicode ? 0x7:0xf);
          Show();
        }
        else
          ShowUp();
      }
//      LastSelPos=FilePos;
      return(TRUE);
    }

    case KEY_DOWN: case KEY_NUMPAD2:  case KEY_SHIFTNUMPAD2:
    {
      if (!LastPage && ViewFile)
      {
        if (VM.Hex)
        {
          FilePos=SecondPos;
          Show();
        }
        else
          ShowDown();
      }
//      LastSelPos=FilePos;
      return(TRUE);
    }

    case KEY_PGUP: case KEY_NUMPAD9: case KEY_SHIFTNUMPAD9: case KEY_CTRLUP:
    {
      if(ViewFile)
      {
        for (I=ViewY1;I<Y2;I++)
          Up();
        Show();
//        LastSelPos=FilePos;
      }
      return(TRUE);
    }

    case KEY_PGDN: case KEY_NUMPAD3:  case KEY_SHIFTNUMPAD3: case KEY_CTRLDOWN:
    {
      if (LastPage || ViewFile==NULL)
        return(TRUE);
      vseek(ViewFile,FilePos,SEEK_SET);
      for (I=ViewY1;I<Y2;I++)
      {
        ReadString(ReadStr,-1,sizeof(ReadStr),NULL,NULL);
        if (LastPage)
          return(TRUE);
      }
      FilePos=vtell(ViewFile);
      for (I=ViewY1;I<=Y2;I++)
        ReadString(ReadStr,-1,sizeof(ReadStr),NULL,NULL);
      /* $ 02.06.2003 VVM
        + ������ ��������� ������� �� Ctrl-Down */
      /* $ 21.05.2003 VVM
        + �� PgDn ������� ������ �� ����� ��������,
          ���� ���� �������� ����� ���� �������.
          ������ ������ ������ */
      if (LastPage && Key == KEY_CTRLDOWN)
      {
        InternalKey++;
        ProcessKey(KEY_CTRLPGDN);
        InternalKey--;
        return(TRUE);
      }
      /* VVM $ */
      /* VVM $ */
      Show();
//      LastSelPos=FilePos;
      return(TRUE);
    }

    case KEY_LEFT: case KEY_NUMPAD4: case KEY_SHIFTNUMPAD4:
    {
      if (LeftPos>0 && ViewFile)
      {
        if (VM.Hex && LeftPos>80-Width)
          LeftPos=Max(80-Width,1);
        LeftPos--;
        Show();
      }
//      LastSelPos=FilePos;
      return(TRUE);
    }

    case KEY_RIGHT: case KEY_NUMPAD6: case KEY_SHIFTNUMPAD6:
    {
      if (LeftPos<MAX_VIEWLINE && ViewFile && !VM.Hex)
      {
        LeftPos++;
        Show();
      }
//      LastSelPos=FilePos;
      return(TRUE);
    }

    case KEY_CTRLLEFT: case KEY_CTRLNUMPAD4:
    {
      if(ViewFile)
      {
        if(VM.Hex)
        {
          FilePos--;
          if (FilePos<0)
            FilePos=0;
        }
        else
        {
          LeftPos-=20;
          if (LeftPos<0)
            LeftPos=0;
        }
        Show();
//        LastSelPos=FilePos;
      }
      return(TRUE);
    }

    case KEY_CTRLRIGHT: case KEY_CTRLNUMPAD6:
    {
      if(ViewFile)
      {
        if(VM.Hex)
        {
          FilePos++;
          if (FilePos >= FileSize)
            FilePos=FileSize-1; //??
        }
        else
        {
          LeftPos+=20;
          if (LeftPos>MAX_VIEWLINE)
            LeftPos=MAX_VIEWLINE;
        }
        Show();
//        LastSelPos=FilePos;
      }
      return(TRUE);
    }

    case KEY_CTRLSHIFTLEFT:    case KEY_CTRLSHIFTNUMPAD4:
      // ������� �� ������ �����
      if (ViewFile)
      {
        LeftPos = 0;
        Show();
      }
      return(TRUE);
    case KEY_CTRLSHIFTRIGHT:     case KEY_CTRLSHIFTNUMPAD6:
    {
        // ������� �� ����� �����
        if (ViewFile)
        {
          int I, Y, Len, MaxLen = 0;
          for (I=0,Y=ViewY1;Y<=Y2;Y++,I++)
          {
             Len = strlen(OutStr[I]);
             if (Len > MaxLen)
               MaxLen = Len;
          } /* for */
          if (MaxLen > Width)
            LeftPos = MaxLen - Width;
          else
            LeftPos = 0;
          Show();
        } /* if */
        return(TRUE);
    }

    case KEY_CTRLHOME:    case KEY_CTRLNUMPAD7:
    case KEY_HOME:        case KEY_NUMPAD7:   case KEY_SHIFTNUMPAD7:
      // ������� �� ������ �����
      if(ViewFile)
        LeftPos=0;
    case KEY_CTRLPGUP:    case KEY_CTRLNUMPAD9:
      if(ViewFile)
      {
        FilePos=0;
        Show();
//        LastSelPos=FilePos;
      }
      return(TRUE);

    case KEY_CTRLEND:     case KEY_CTRLNUMPAD1:
    case KEY_END:         case KEY_NUMPAD1: case KEY_SHIFTNUMPAD1:
      // ������� �� ����� �����
      if(ViewFile)
        LeftPos=0;
    case KEY_CTRLPGDN:    case KEY_CTRLNUMPAD3:
      if(ViewFile)
      {
        /* $ 15.08.2002 IS
           ��� �������� ������, ���� ��������� ������ �� �������� �������
           ������, �������� ����� �� ���� ��� ������ - ����� ���������
           ��������� End (� ��������) �� ����� ������ ���������� �� ���������
           Down.
        */
        unsigned int max_counter=Y2-ViewY1;
        if(VM.Hex)
          vseek(ViewFile,0,SEEK_END);
        else
        {
          vseek(ViewFile,-1,SEEK_END);
          int LastSym=vgetc(ViewFile);
          if(LastSym!=EOF && LastSym!=CRSym)
            ++max_counter;
        }
        FilePos=vtell(ViewFile);
/*
        {
          char Buf[100];
          sprintf(Buf,"%I64X",FilePos);
          Message(0,1,"End",Buf,"Ok");
        }
*/
        for (I=0;I<max_counter;I++)
          Up();
        /* IS 15.08.2002 $ */
/*
        {
          char Buf[100];
          sprintf(Buf,"%I64X, %d",FilePos, I);
          Message(0,1,"Up",Buf,"Ok");
        }
*/
        if (VM.Hex)
          FilePos&=~(VM.Unicode ? 0x7:0xf);
/*
        if (VM.Hex)
        {
          char Buf[100];
          sprintf(Buf,"%I64X",FilePos);
          Message(0,1,"VM.Hex",Buf,"Ok");
        }
*/
        Show();
//        LastSelPos=FilePos;
      }
      return(TRUE);

    case KEY_MACRO_EMPTY:
      return FileSize==0;
    case KEY_MACRO_SELECTED:
      return SelectSize==0?FALSE:TRUE;
    case KEY_MACRO_EOF:
      return LastPage || ViewFile==NULL;
    case KEY_MACRO_BOF:
      return !FilePos || ViewFile==NULL;

    default:
      if (Key>=' ' && Key<=255)
      {
        Search(0,Key);
        return(TRUE);
      }
  }
  return(FALSE);
}
/* IS $ */

int Viewer::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  if ((MouseEvent->dwButtonState & 3)==0)
    return(FALSE);

  /* $ 18.07.2000 tran
     ������ ��� ���������� ����*/
  int MsX=MouseEvent->dwMousePosition.X;
  int MsY=MouseEvent->dwMousePosition.Y;
  /* tran 18.07.2000 $ */

  /* $ 22.01.2001 IS
       ���������� �����-�� ����������� -> ������ ���������
  */
//  SelectSize=0;
  /* IS $ */

  /* $ 18.07.2000 tran
     ��������� �������� */
  /* $ 10.09.2000 SVS
     ! ���������� ��������� ��� ������� �������
       ������������ ������ ����
  */
  /* $ 02.10.2000 SVS
    > ���� ������ � ����� ���� ���������, ����� ���������� �� ��������
    > ���� ������ ������� ������. ����� ������� ����� ������ �����.
  */
  if ( ViOpt.ShowScrollbar && MsX==X2)
  {
    /* $ 01.09.2000 SVS
       ��������� ���� � �������� � ������� ������� ScrollBar`�
    */
    if (MsY == Y1+1)
      while (IsMouseButtonPressed())
        ProcessKey(KEY_UP);
    /* SVS $*/
    else if (MsY==Y2)
    {
      while (IsMouseButtonPressed())
      {
//        _SVS(SysLog("Viewer/ KEY_DOWN= %i, %i",FilePos,FileSize));
        ProcessKey(KEY_DOWN);
      }
    }
    else if(MsY == Y1+2)
      ProcessKey(KEY_CTRLHOME);
    else if(MsY == Y2-1)
      ProcessKey(KEY_CTRLEND);
    else
    {
      INPUT_RECORD rec;
      while (IsMouseButtonPressed())
      {
        /* $ 14.05.2001 DJ
           ����� ������ ����������������; ���������� ������ �� ������� ������
        */
        FilePos=(FileSize-1)/(Y2-ViewY1-1)*(MsY-ViewY1);
        /* DJ $ */
        int Perc;
        if(FilePos > FileSize)
        {
          FilePos=FileSize;
          Perc=100;
        }
        else if(FilePos < 0)
        {
          FilePos=0;
          Perc=0;
        }
        else
          Perc=ToPercent64(FilePos,FileSize);
//_SVS(SysLog("Viewer/ ToPercent()=%i, %I64d, %I64d, Mouse=[%d:%d]",Perc,FilePos,FileSize,MsX,MsY));
        if(Perc == 100)
          ProcessKey(KEY_CTRLEND);
        else if(!Perc)
          ProcessKey(KEY_CTRLHOME);
        else
        {
          /* $ 27.04.2001 DJ
             �� ���� ������ ����������
          */
          AdjustFilePos();
          Show();
          /* DJ $ */
        }
        GetInputRecord(&rec);
        MsX=rec.Event.MouseEvent.dwMousePosition.X;
        MsY=rec.Event.MouseEvent.dwMousePosition.Y;
      }
    }
    return (TRUE);
  }
  /* SVS 02.10.2000 $ */
  /* SVS $*/
  /* tran 18.07.2000 $ */

  /* $ 16.12.2000 tran
     ������ ����� �� ������ ���� */
  /* $ 12.10.2001 SKV
    ���, � ������ ���� �� ����, statusline...
  */
  if ( MsY==Y1 && ShowStatusLine) // Status line
  /* SKV$*/
  {
    int XTable, XPos, NameLength;
    NameLength=ObjWidth-40;
    if (Opt.ViewerEditorClock && HostFileViewer!=NULL && HostFileViewer->IsFullScreen())
      NameLength-=6;
    if (NameLength<20)
      NameLength=20;
    XTable=NameLength+1;
    XPos=NameLength+1+10+1+10+1;

    while(IsMouseButtonPressed());

    MsX=MouseX;
    MsY=MouseY;

    if (MsY!=Y1)
      return(TRUE);

    //_D(SysLog("MsX=%i, XTable=%i, XPos=%i",MsX,XTable,XPos));
    if ( MsX>=XTable && MsX<=XTable+10 )
    {
        ProcessKey(KEY_SHIFTF8);
        return (TRUE);
    }
    if ( MsX>=XPos && MsX<=XPos+7+1+4+1+3 )
    {
        ProcessKey(KEY_ALTF8);
        return (TRUE);
    }
  }
  /* tran $ */
  if (MsX<X1 || MsX>X2 || MsY<ViewY1 || MsY>Y2)
    return(FALSE);

  if (MsX<X1+7)
    while (IsMouseButtonPressed() && MouseX<X1+7)
      ProcessKey(KEY_LEFT);
  else
    if (MsX>X2-7)
      while (IsMouseButtonPressed() && MouseX>X2-7)
        ProcessKey(KEY_RIGHT);
    else
      if (MsY<ViewY1+(Y2-ViewY1)/2)
        while (IsMouseButtonPressed() && MouseY<ViewY1+(Y2-ViewY1)/2)
          ProcessKey(KEY_UP);
      else
        while (IsMouseButtonPressed() && MouseY>=ViewY1+(Y2-ViewY1)/2)
          ProcessKey(KEY_DOWN);
  return(TRUE);
}

void Viewer::Up()
{
  if(!ViewFile)
    return;

  char Buf[MAX_VIEWLINE];
  int BufSize,StrPos,Skipped,I,J;

  if(FilePos > (__int64)sizeof(Buf))
    BufSize=sizeof(Buf);
  else
    BufSize=(int)FilePos;

  if (BufSize==0)
    return;
  LastPage=0;
  if (VM.Hex)
  {
    int UpSize=VM.Unicode ? 8:16;
    if (FilePos<(__int64)UpSize)
      FilePos=0;
    else
      FilePos-=UpSize;
    return;
  }
  vseek(ViewFile,FilePos-(__int64)BufSize,SEEK_SET);
  vread(Buf,BufSize,ViewFile);
  Skipped=0;
  if (Buf[BufSize-1]==CRSym)
  {
    BufSize--;
    Skipped++;
  }
  if (BufSize>0 && CRSym==10 && Buf[BufSize-1]==13)
  {
    BufSize--;
    Skipped++;
  }
  for (I=BufSize-1;I>=-1;I--)
  {
    /* $ 29.11.2001 DJ
       �� ���������� �� ������� ������� (� ���� ���� ����� ���� �������� ������� �������...)
    */
    if (I==-1 || Buf[I]==CRSym)   /* DJ $ */
      if (!VM.Wrap)
      {
        FilePos-=BufSize-(I+1)+Skipped;
        return;
      }
      else
      {
        if (!Skipped && I==-1)
          break;

        for (StrPos=0,J=I+1;J<=BufSize;J++)
        {
          if (StrPos==0 || StrPos >= Width)
          {
            if (J==BufSize)
            {
              if (Skipped==0)
                FilePos--;
              else
                FilePos-=Skipped;
              return;
            }
            if (CalcStrSize(&Buf[J],BufSize-J) <= Width)
            {
              FilePos-=BufSize-J+Skipped;
              return;
            }
            else
              StrPos=0;
          }
          if (J<BufSize)
            if (Buf[J]=='\t')
              StrPos+=ViOpt.TabSize-(StrPos % ViOpt.TabSize);
            else
              if (Buf[J]!=13)
                StrPos++;
        }
      }
  }
  for (I=Min(Width,BufSize);I>0;I-=5)
    if (CalcStrSize(&Buf[BufSize-I],I) <= Width)
    {
      FilePos-=I+Skipped;
      break;
    }
}


int Viewer::CalcStrSize(char *Str,int Length)
{
  int Size,I;
  for (Size=0,I=0;I<Length;I++)
    switch(Str[I])
    {
      case '\t':
        Size+=ViOpt.TabSize-(Size % ViOpt.TabSize);
        break;
      case 10:
      case 13:
        break;
      default:
        Size++;
        break;
    }
  return(Size);
}


void Viewer::SetViewKeyBar(KeyBar *ViewKeyBar)
{
  Viewer::ViewKeyBar=ViewKeyBar;
  ChangeViewKeyBar();
}


void Viewer::ChangeViewKeyBar()
{
  if (ViewKeyBar)
  {
    /* $ 12.07.2000 SVS
       Wrap ����� 3 �������
    */
    /* $ 15.07.2000 SVS
       Wrap ������ ������������ ���������, � �� �������
    */
    ViewKeyBar->Change(
       MSG(
       (!VM.Wrap)?((!VM.TypeWrap)?MViewF2:MViewShiftF2)
       :MViewF2Unwrap),1);
    ViewKeyBar->Change(KBL_SHIFT,MSG((VM.TypeWrap)?MViewF2:MViewShiftF2),1);
    /* SVS $ */
    /* SVS $ */

    if (VM.Hex)
      ViewKeyBar->Change(MSG(MViewF4Text),3);
    else
      ViewKeyBar->Change(MSG(MViewF4),3);

    if (VM.AnsiMode)
      ViewKeyBar->Change(MSG(MViewF8DOS),7);
    else
      ViewKeyBar->Change(MSG(MViewF8),7);

    ViewKeyBar->Redraw();
  }
  struct ViewerMode vm;
  memmove(&vm,&VM,sizeof(struct ViewerMode));
  CtrlObject->Plugins.CurViewer=this; //HostFileViewer;
//  CtrlObject->Plugins.ProcessViewerEvent(VE_MODE,&vm);
}

long WINAPI ViewerSearchDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  /* 23.09.2003 KM */
  Dialog* Dlg=(Dialog*)hDlg;
  char DataStr[NM*2];
  struct FarDialogItem Item;

  switch(Msg)
  {
    case DN_INITDIALOG:
    {
      /* $ 22.09.2003 KM
         ������������ ��������� ������ ����� �������� ������
         � ����������� �� Dlg->Item[6].Selected
      */
      Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,6,(long)&Item);

      if (Item.Param.Selected)
      {
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,2,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,3,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,7,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,FALSE);
      }
      else
      {
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,2,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,3,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,7,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,TRUE);
      }

      Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,2,1);
      Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,3,1);
      /* KM $ */

      return TRUE;
    }
    case DN_EDITCHANGE:
    {
      FarDialogItem &Item=*reinterpret_cast<FarDialogItem*>(Param2);

      if (Param1==2) // Containing text
      {
        Transform(DataStr,Item.Data.Data,sizeof(DataStr),'X');
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,3,(long)DataStr);
      }

      if (Param1==3) // Containing hexadecimal code
      {
        Transform(DataStr,Item.Data.Data,sizeof(DataStr),'S');
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,2,(long)DataStr);
      }
      return TRUE;
    }
    case DN_BTNCLICK:
    {
      if(Param1 == 5 || Param1 == 6)
      {
	      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

        /* $ 22.09.2003 KM
           ������������ ��������� ������ ����� �������� ������
           � ����������� �� �������������� ������ hex search
        */
        if (Param1 == 6 && Param2)
        {
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,2,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,3,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,7,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,FALSE);
        }
        else
        {
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,2,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,3,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,7,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,TRUE);
        }
        /* KM $ */

        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,2,(long)&Item);
        Transform(DataStr,Item.Data.Data,sizeof(DataStr),'X');
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,3,(long)DataStr);

        if (strlen(DataStr)>0)
        {
          int UnchangeFlag=Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,2,-1);
          Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,3,UnchangeFlag);
        }

	      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
        return TRUE;
      }
    }
    /* $ 15.10.2003 KM
        ���������� "�������" �������
    */
    case DN_HOTKEY:
    {
      if (Param1==1)
      {
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,6,(long)&Item);

        if (Item.Param.Selected)
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,3,0);
        else
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,2,0);
        return FALSE;
      }
    }
    /* KM $ */
  }
  /* KM $ */
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

static void PR_ViewerSearchMsg(void)
{
  ViewerSearchMsg((char*)PreRedrawParam.Param1);
}

void ViewerSearchMsg(char *MsgStr)
{
  /* $ 23.09.2003 KM */
  Message(0,0,MSG(MViewSearchTitle),(SearchHex?MSG(MViewSearchingHex):MSG(MViewSearchingFor)),MsgStr);
  /* KM $ */
  PreRedrawParam.Param1=MsgStr;
}

/* $ 27.01.2003 VVM
   + �������� Next ����� ��������� ��������:
   0 - ����� �����
   1 - ���������� ����� �� ��������� �������
   2 - ���������� ����� � ������ �����
*/
void Viewer::Search(int Next,int FirstChar)
{
  const char *TextHistoryName="SearchText";
  const char *HexMask="HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH ";
  /* $ 01.08.2000 KM
     �������� ����� checkbox ��� ������ "Whole words"
  */
  static struct DialogData SearchDlgData[]={
  /* 00 */ DI_DOUBLEBOX,3,1,72,10,0,0,0,0,(char *)MViewSearchTitle,
  /* 01 */ DI_TEXT,5,2,0,0,0,0,0,0,(char *)MViewSearchFor,
  /* 02 */ DI_EDIT,5,3,70,3,1,(DWORD)TextHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,0,"",
  /* 03 */ DI_FIXEDIT,5,3,70,3,0,(DWORD)HexMask,DIF_MASKEDIT,0,"",
  /* 04 */ DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 05 */ DI_RADIOBUTTON,5,5,0,0,0,1,DIF_GROUP,0,(char *)MViewSearchForText,
  /* 06 */ DI_RADIOBUTTON,5,6,0,0,0,0,0,0,(char *)MViewSearchForHex,
  /* 07 */ DI_CHECKBOX,40,5,0,0,0,0,0,0,(char *)MViewSearchCase,
  /* 08 */ DI_CHECKBOX,40,6,0,0,0,0,0,0,(char *)MViewSearchWholeWords,
  /* 09 */ DI_CHECKBOX,40,7,0,0,0,0,0,0,(char *)MViewSearchReverse,
  /* 10 */ DI_TEXT,3,8,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 11 */ DI_BUTTON,0,9,0,0,0,0,DIF_CENTERGROUP,1,(char *)MViewSearchSearch,
  /* 12 */ DI_BUTTON,0,9,0,0,0,0,DIF_CENTERGROUP,0,(char *)MViewSearchCancel
  };
  /* KM $ */
  MakeDialogItems(SearchDlgData,SearchDlg);

  unsigned char SearchStr[SEARCHSTRINGBUFSIZE];
  char MsgStr[SEARCHSTRINGBUFSIZE+16];
  __int64 MatchPos;
  /* $ 01.08.2000 KM
     ��������� ����� ���������� WholeWords
  */
  int SearchLength,Case,WholeWords,ReverseSearch,Match;
  /* KM $ */

  if (ViewFile==NULL || Next && *LastSearchStr==0)
    return;

  /* 23.09.2003 KM */
  if (*LastSearchStr)
    strncpy((char *)SearchStr,(char *)LastSearchStr,sizeof(SearchStr)-1);
  else
    *SearchStr=0;

  strncpy(SearchDlg[2].Data,(char *)SearchStr,sizeof(SearchDlg[2].Data)-1);
  SearchDlg[5].Selected=!LastSearchHex;
  SearchDlg[6].Selected=LastSearchHex;
  SearchDlg[7].Selected=LastSearchCase;
  /* KM $ */
  /* $ 01.08.2000 KM
     ������������� checkbox'� "Whole words"
  */
  SearchDlg[8].Selected=LastSearchWholeWords;
  /* KM $ */
  SearchDlg[9].Selected=LastSearchReverse;

  if (SearchFlags.Check(REVERSE_SEARCH))
    SearchDlg[9].Selected = !SearchDlg[9].Selected;

  if (VM.Unicode)
  {
    SearchDlg[5].Selected=TRUE;
    SearchDlg[6].Flags|=DIF_DISABLE;
    SearchDlg[6].Selected=FALSE;
  }

  if(SearchDlg[6].Selected)
  {
    SearchDlg[7].Flags|=DIF_DISABLE;
  }

  /* $ 22.09.2003 KM */
  char HexStr[NM*2];
  Transform(HexStr,(char *)SearchStr,sizeof(HexStr),'X');
  strncpy(SearchDlg[3].Data,HexStr,sizeof(SearchDlg[3].Data)-1);
  /* KM $ */

  if (!Next)
  {
    SearchFlags.Flags = 0;
    Dialog Dlg(SearchDlg,sizeof(SearchDlg)/sizeof(SearchDlg[0]),ViewerSearchDlgProc);
    Dlg.SetPosition(-1,-1,76,12);
    Dlg.SetHelp("ViewerSearch");
    if (FirstChar)
    {
      Dlg.InitDialog();
      Dlg.Show();
      Dlg.ProcessKey(FirstChar);
    }
    Dlg.Process();
    if (Dlg.GetExitCode()!=11)
      return;
  }
  strncpy((char *)SearchStr,SearchDlg[2].Data,sizeof(SearchStr)-1);
  SearchHex=SearchDlg[6].Selected;
  Case=SearchDlg[7].Selected;
  /* $ 01.08.2000 KM
     ���������� ��������� checkbox'� "Whole words"
  */
  WholeWords=SearchDlg[8].Selected;
  /* KM $ */
  ReverseSearch=SearchDlg[9].Selected;

  strncpy((char *)LastSearchStr,(char *)SearchStr,sizeof(LastSearchStr)-1);
  LastSearchHex=SearchHex;
  LastSearchCase=Case;
  /* $ 01.08.2000 KM
     ���������� ���������� ��������� WholeWords
  */
  LastSearchWholeWords=WholeWords;
  /* KM $ */
  if (!SearchFlags.Check(REVERSE_SEARCH))
    LastSearchReverse=ReverseSearch;

  if ((SearchLength=strlen((char *)SearchStr))==0)
    return;

  {
    //SaveScreen SaveScr;
    SetCursorType(FALSE,0);
    if (SearchHex)
    {
      strncpy(MsgStr,(char *)SearchDlg[3].Data,sizeof(MsgStr)-1);
      RemoveTrailingSpaces(MsgStr);
    }
    else
      strncpy(MsgStr,(char *)SearchStr,sizeof(MsgStr)-1);

    if(strlen(MsgStr)+16 >= X2)
      TruncStrFromEnd(MsgStr, ObjWidth-17);
    InsertQuote(MsgStr);

    SetPreRedrawFunc(PR_ViewerSearchMsg);
    ViewerSearchMsg(MsgStr);

    if (!Case && !SearchHex)
      for (int I=0;I<SearchLength;I++)
        SearchStr[I]=LocalUpper(SearchStr[I]);

    SelectSize = 0;
    if (Next)
    {
      if (Next == 2)
      {
        SearchFlags.Set(SEARCH_MODE2);
        LastSelPos = ReverseSearch?FileSize:0;
      }
      else
        LastSelPos = SelectPos + (ReverseSearch?-1:1);
    }
    else
    {
      LastSelPos = FilePos;
      if (LastSelPos == 0 || LastSelPos == FileSize)
        SearchFlags.Set(SEARCH_MODE2);
    }

    vseek(ViewFile,LastSelPos,SEEK_SET);
    Match=0;
    if (SearchLength>0 && (!ReverseSearch || LastSelPos>0))
    {
      char Buf[8192];
      /* $ 01.08.2000 KM
         ������ ��� CurPos � unsigned long �� long
         ��-�� ����, ��� ������ ��� �������� ��� ���������
         �� -1, � CurPos �� ��� ����� ������������� � ������
         ��������� �������� ���������
      */
      __int64 CurPos=LastSelPos;
      /* KM $ */
      int BufSize=sizeof(Buf);
      if (ReverseSearch)
      {
        /* $ 01.08.2000 KM
           ������� ���������� CurPos � ������ Whole words
        */
        if (WholeWords)
          CurPos-=sizeof(Buf)-SearchLength+1;
        else
          CurPos-=sizeof(Buf)-SearchLength;
        /* KM $ */
        if (CurPos<0)
          BufSize+=(int)CurPos;
      }
      int ReadSize;
      while (!Match)
      {
        /* $ 01.08.2000 KM
           �������� ������ if (ReverseSearch && CurPos<0) �� if (CurPos<0),
           ��� ��� ��� ������� ������ � LastSelPos=0xFFFFFFFF, �����
           ������������ ��� � �� ���������.
        */
        if (CurPos<0)
          CurPos=0;
        /* KM $ */

        vseek(ViewFile,CurPos,SEEK_SET);
        if ((ReadSize=vread(Buf,BufSize,ViewFile))<=0)
          break;

        if(CheckForEscSilent())
        {
          if (ConfirmAbortOp())
          {
            SetPreRedrawFunc(NULL);
            return;
          }
          ViewerSearchMsg(MsgStr);
        }

        if (VM.UseDecodeTable && !SearchHex && !VM.Unicode)
          for (int I=0;I<ReadSize;I++)
            Buf[I]=TableSet.DecodeTable[Buf[I]];

        /* $ 01.08.2000 KM
           ������� ����� �������� �� Case sensitive � Hex
           � ���� ���, ����� Buf ���������� � �������� ��������
        */
        if (!Case && !SearchHex)
          LocalUpperBuf(Buf,ReadSize);
        /* KM $ */

        /* $ 01.08.2000 KM
           ����� ����� ������ ����� ���������� ��������� ������
           � Buf � ������� ��������, ���� ����� �� �����������������
           ��� �� ������ Hex-������ � � ����� � ���� ����������� ��� ������
        */
        int MaxSize=ReadSize-SearchLength+1;
        int Increment=ReverseSearch ? -1:+1;
        for (int I=ReverseSearch ? MaxSize-1:0;I<MaxSize && I>=0;I+=Increment)
        {
          /* $ 01.08.2000 KM
             ��������� ������ "Whole words"
          */
          /* $ 26.05.2002 KM
              ���������� ������ � ������ �� ����� ������.
          */
          int locResultLeft=FALSE;
          int locResultRight=FALSE;

          if (WholeWords)
          {
            if (I!=0)
            {
              if (IsSpace(Buf[I-1]) || IsEol(Buf[I-1]) ||
                 (strchr(Opt.WordDiv,Buf[I-1])!=NULL))
                locResultLeft=TRUE;
            }
            else
            {
              locResultLeft=TRUE;
            }

            if (ReadSize!=BufSize && I+SearchLength>=ReadSize)
              locResultRight=TRUE;
            else
              if (I+SearchLength<ReadSize &&
                 (IsSpace(Buf[I+SearchLength]) || IsEol(Buf[I+SearchLength]) ||
                 (strchr(Opt.WordDiv,Buf[I+SearchLength])!=NULL)))
                locResultRight=TRUE;
          }
          else
          {
            locResultLeft=TRUE;
            locResultRight=TRUE;
          }

          Match=locResultLeft && locResultRight && SearchStr[0]==Buf[I] &&
            (SearchLength==1 || SearchStr[1]==Buf[I+1] &&
            (SearchLength==2 || memcmp(SearchStr+2,&Buf[I+2],SearchLength-2)==0));

          if (Match)
          {
            MatchPos=CurPos+I;
            break;
          }
          /* KM $ */
          /* KM $ */
        }
        /* KM $ */

        if ((ReverseSearch && CurPos <= 0) || (!ReverseSearch && ReadSize < BufSize))
          break;
        if (ReverseSearch)
        {
          /* $ 01.08.2000 KM
             ������� ���������� CurPos � ������ Whole words
          */
          if (WholeWords)
            CurPos-=sizeof(Buf)-SearchLength+1;
          else
            CurPos-=sizeof(Buf)-SearchLength;
        }
        else
        {
          if (WholeWords)
            CurPos+=sizeof(Buf)-SearchLength+1;
          else
            CurPos+=sizeof(Buf)-SearchLength;
        }
        /* KM $ */
      }
    }
  }

  SetPreRedrawFunc(NULL);

  if (Match)
  {
    /* $ 24.01.2003 KM
       ! �� ��������� ������ �������� �� ����� ������ ��
         ����� ������������ ������.
    */
    SelectText(MatchPos,SearchLength,ReverseSearch?0x2:0);

    // ������� ��������� �� ���������� ����� ������ �� �����.
    int FromTop=(ScrY-(Opt.ShowKeyBarViewer?2:1))/4;
    if (FromTop<0 || FromTop>ScrY)
      FromTop=0;

    for (int i=0;i<FromTop;i++)
      Up();

    AdjustSelPosition = TRUE;
    Show();
    AdjustSelPosition = FALSE;
    /* KM $ */
  }
  else
  {
    /* $ 27.01.2003 VVM
       + ����� ��������� ������ ������� � �������� ������ � ������/����� */
    if (SearchFlags.Check(SEARCH_MODE2))
      Message(MSG_DOWN|MSG_WARNING,1,MSG(MViewSearchTitle),
        (SearchHex?MSG(MViewSearchCannotFindHex):MSG(MViewSearchCannotFind)),MsgStr,MSG(MOk));
    else
    {
      if (Message(MSG_DOWN|MSG_WARNING,2,MSG(MViewSearchTitle),
            (SearchHex?MSG(MViewSearchCannotFindHex):MSG(MViewSearchCannotFind)),MsgStr,
            (ReverseSearch?MSG(MViewSearchFromEnd):MSG(MViewSearchFromBegin)),
             MSG(MHYes),MSG(MHNo)) == 0)
        Search(2,0);
    }
    /* VVM $ */
  }
}


/*void Viewer::ConvertToHex(char *SearchStr,int &SearchLength)
{
  char OutStr[512],*SrcPtr;
  int OutPos=0,N=0;
  SrcPtr=SearchStr;
  while (*SrcPtr)
  {
    while (IsSpace(*SrcPtr))
      SrcPtr++;
    if (SrcPtr[0])
      if (SrcPtr[1]==0 || IsSpace(SrcPtr[1]))
      {
        N=HexToNum(SrcPtr[0]);
        SrcPtr++;
      }
      else
      {
        N=16*HexToNum(SrcPtr[0])+HexToNum(SrcPtr[1]);
        SrcPtr+=2;
      }
    if (N>=0)
      OutStr[OutPos++]=N;
    else
      break;
  }
  memcpy(SearchStr,OutStr,OutPos);
  SearchLength=OutPos;
}*/


int Viewer::HexToNum(int Hex)
{
  Hex=toupper(Hex);
  if (Hex>='0' && Hex<='9')
    return(Hex-'0');
  if (Hex>='A' && Hex<='F')
    return(Hex-'A'+10);
  return(-1000);
}


int Viewer::GetWrapMode()
{
  return(VM.Wrap);
}


void Viewer::SetWrapMode(int Wrap)
{
  Viewer::VM.Wrap=Wrap;
}


void Viewer::EnableHideCursor(int HideCursor)
{
  Viewer::HideCursor=HideCursor;
}


int Viewer::GetWrapType()
{
  return(VM.TypeWrap);
}


void Viewer::SetWrapType(int TypeWrap)
{
  Viewer::VM.TypeWrap=TypeWrap;
}


void Viewer::GetFileName(char *Name)
{
  strcpy(Name,FullFileName);
}


void Viewer::ShowConsoleTitle()
{
  char Title[NM+20];
  sprintf(Title,MSG(MInViewer),PointToName(FileName));
  SetFarTitle(Title);
}


void Viewer::SetTempViewName(const char *Name, BOOL DeleteFolder)
{
//  ConvertNameToFull(Name,TempViewName, sizeof(TempViewName));
  _tran(SysLog("[%p] Viewer::SetTempViewName() [%s]",this,Name));

  if(Name && *Name)
  {
    if (ConvertNameToFull(Name,TempViewName, sizeof(TempViewName)) >= sizeof(TempViewName))
      return;
  }
  else
  {
    *TempViewName=0;
    DeleteFolder=FALSE;
  }
  Viewer::DeleteFolder=DeleteFolder;
}


void Viewer::SetTitle(const char *Title)
{
  if (Title==NULL)
    *Viewer::Title=0;
  else
  /* $ 10.07.2001 IS
     - ���: �� ���������� ������ Title, ��� ��������� � ����� ������ �
       � ������� ����.
  */
    strncpy(Viewer::Title,Title,sizeof(Viewer::Title)-1);
  /* IS $ */
}


__int64 Viewer::GetFilePos()
{
  return(FilePos);
}


/* $ 18.07.2000 tran
   * changes 'long' to 'unsigned long' */
void Viewer::SetFilePos(__int64 Pos)
{
  FilePos=Pos;
};


void Viewer::SetPluginData(char *PluginData)
{
  strcpy(Viewer::PluginData,NullToEmpty(PluginData));
}


void Viewer::SetNamesList(NamesList *List)
{
  if (List!=NULL)
    List->MoveData(&ViewNamesList);
}


int Viewer::vread(char *Buf,int Size,FILE *SrcFile)
{
  if(!SrcFile)
    return -1;
  if (VM.Unicode)
  {
    char TmpBuf[16384+10];
    int i;
    char t;
    int ReadSize=fread(TmpBuf,1,Size*2,SrcFile);
    TmpBuf[ReadSize]=0;
    /* $ 20.10.2000 tran
       �������� ������� ������ */
    TmpBuf[ReadSize+1]=0;
    if ( FirstWord == 0x0FFFE )
    {
        for ( i=0; i<ReadSize; i+=2 )
        {
            t=TmpBuf[i];
            TmpBuf[i]=TmpBuf[i+1];
            TmpBuf[i+1]=t;
        }
    }
    /* tran $ */
    ReadSize+=(ReadSize & 1);
    WideCharToMultiByte(CP_OEMCP,0,(LPCWSTR)TmpBuf,ReadSize/2,Buf,Size," ",NULL);
    return(ReadSize/2);
  }
  else
    return(fread(Buf,1,Size,SrcFile));
}


int Viewer::vseek(FILE *SrcFile,__int64 Offset,int Whence)
{
  if(!SrcFile)
    return -1;
  if (VM.Unicode)
    return(fseek64(SrcFile,Offset*2,Whence));
  else
    return(fseek64(SrcFile,Offset,Whence));
}


__int64 Viewer::vtell(FILE *SrcFile)
{
  if(!SrcFile)
    return -1;
  __int64 Pos=ftell64(SrcFile);
  if (VM.Unicode)
    Pos=(Pos+(Pos&1))/2;
  return(Pos);
}


int Viewer::vgetc(FILE *SrcFile)
{
  if(!SrcFile)
    return -1;
  if (VM.Unicode)
  {
    char TmpBuf[1];
    if (vread(TmpBuf,1,SrcFile)==0)
      return(EOF);
    return(TmpBuf[0]);
  }
  else
    return(getc(SrcFile));
}


#define RB_PRC 3
#define RB_HEX 4
#define RB_DEC 5

//   ! ������� ����� ������� GoTo() - ��� �������������� ���������
//   - ���������� ������� ����� ��� �������� (������� GoTo)
void Viewer::GoTo(int ShowDlg,__int64 Offset, DWORD Flags)
{
  /* $ 17.07.2000 tran
     + new variable*/
  __int64 Relative=0;
  /* tran 17.07.2000 $ */
  const char *LineHistoryName="ViewerOffset";
  static struct DialogData GoToDlgData[]=
  {
    /* 0 */ DI_DOUBLEBOX,3,1,31,7,0,0,0,0,(char *)MViewerGoTo,
    /* 1 */ DI_EDIT,5,2,29,2,1,(DWORD)LineHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,1,"",
    /* 2 */ DI_TEXT,3,3,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    /* 3 */ DI_RADIOBUTTON,5,4,0,0,0,0,DIF_GROUP,0,(char *)MGoToPercent,
    /* 4 */ DI_RADIOBUTTON,5,5,0,0,0,0,0,0,(char *)MGoToHex,
    /* 5 */ DI_RADIOBUTTON,5,6,0,0,0,0,0,0,(char *)MGoToDecimal,
  };
  MakeDialogItems(GoToDlgData,GoToDlg);
  /* $ 01.08.2000 tran
     � DIF_USELASTHISTORY ��� �� �����*/
  //  static char PrevLine[20];
  static int PrevMode=0;

  // strcpy(GoToDlg[1].Data,PrevLine);
  GoToDlg[3].Selected=GoToDlg[4].Selected=GoToDlg[5].Selected=0;
  if ( VM.Hex )
    PrevMode=1;
  GoToDlg[PrevMode+3].Selected=TRUE;

  {
    if(ShowDlg)
    {
      Dialog Dlg(GoToDlg,sizeof(GoToDlg)/sizeof(GoToDlg[0]));
      Dlg.SetHelp("ViewerGotoPos");
      Dlg.SetPosition(-1,-1,35,9);
      Dlg.Process();
      /* $ 17.07.2000 tran
         - remove isdigit check()
           ������, ��� ��� ���
           ���� ������ ffff ��� hex offset, �� ��� ��� ����� ������ �� ��� */
      /* $ 22.03.2001 IS
         - ������� ���������� ������ �����, ����� � ������ �������� �������
           ������ ��������� � ������ �����.
      */
      if (Dlg.GetExitCode()<=0 ) //|| !isdigit(*GoToDlg[1].Data))
        return;
      /* IS $ */
      // strncpy(PrevLine,GoToDlg[1].Data,sizeof(PrevLine));
      /* $ 17.07.2000 tran
         ��� ��� ���������� ���� ���� ptr, ������� � ���������� */
      char *ptr=GoToDlg[1].Data;
      if ( ptr[0]=='+' || ptr[0]=='-' )     // ���� ����� ���������������
      {
          if (ptr[0]=='+')
              Relative=1;
          else
              Relative=-1;
          memmove(ptr,ptr+1,strlen(ptr)); // ���� �� ������� ��� strlen(ptr)-1,
                                          // �� �� ���������� :)
      }
      if ( strchr(ptr,'%') )   // �� ����� ���������
      {
          GoToDlg[RB_HEX].Selected=GoToDlg[RB_DEC].Selected=0;
          GoToDlg[RB_PRC].Selected=1;
      }
      else if ( strnicmp(ptr,"0x",2)==0 || ptr[0]=='$' || strchr(ptr,'h') ||strchr(ptr,'H') ) // �� ����� - hex ��� ����!
      {
          GoToDlg[RB_PRC].Selected=GoToDlg[RB_DEC].Selected=0;
          GoToDlg[RB_HEX].Selected=1;
          if ( strnicmp(ptr,"0x",2)==0)
              memmove(ptr,ptr+2,strlen(ptr)-1); // � ��� ���� -1, � �� -2  // ������� ������
          else if (ptr[0]=='$')
              memmove(ptr,ptr+1,strlen(ptr));
          //Relative=0; // ��� hex �������� ������� ������������� ��������?
      }
      /* $ 19.07.2000 tran
         ��� ����� NNNd - ���������� ����� */
      /* $ 22.05.2001 DJ
         ������-��, ���� ����� ����������������� ����� - D...
         �������� ��������������
      /*
      else if (strchr(ptr,'d') || strchr(ptr,'D'))
      {
          GoToDlg[RB_HEX].Selected=GoToDlg[RB_PRC].Selected=0;
          GoToDlg[RB_DEC].Selected=1;
      }
      */
      /* DJ $ */
      /* tran 19.07.2000 $ */
      if (GoToDlg[RB_PRC].Selected)
      {
        //int cPercent=ToPercent64(FilePos,FileSize);
        PrevMode=0;
        int Percent=atoi(GoToDlg[1].Data);
        //if ( Relative  && (cPercent+Percent*Relative<0) || (cPercent+Percent*Relative>100)) // �� ������� - ����
        //  return;
        if (Percent>100)
          return;
        //if ( Percent<0 )
        //  Percent=0;
        Offset=FileSize/100*Percent;
        if (VM.Unicode)
          Offset*=2;
        while (ToPercent64(Offset,FileSize)<Percent)
          Offset++;
      }
      if (GoToDlg[RB_HEX].Selected)
      {
        PrevMode=1;
        sscanf(GoToDlg[1].Data,"%I64x",&Offset);
      }
      if (GoToDlg[RB_DEC].Selected)
      {
        PrevMode=2;
        sscanf(GoToDlg[1].Data,"%I64d",&Offset);
      }
    }// ShowDlg
    else
    {
      Relative=(Flags&VSP_RELATIVE)*(Offset<0?-1:1);
      if(Flags&VSP_PERCENT)
      {
        __int64 Percent=Offset;
        if (Percent>100)
          return;
        //if ( Percent<0 )
        //  Percent=0;
        Offset=FileSize/100*Percent;
        if (VM.Unicode)
          Offset*=2;
        while (ToPercent64(Offset,FileSize)<Percent)
          Offset++;
      }
    }

    if ( Relative )
    {
        if ( Relative==-1 && Offset>FilePos ) // ������ ����, if (FilePos<0) �� ������� - FilePos � ��� unsigned long
            FilePos=0;
        else
            FilePos=VM.Unicode? FilePos+Offset*Relative/2 : FilePos+Offset*Relative;
    }
    else
        FilePos=VM.Unicode ? Offset/2:Offset;
    if ( FilePos>FileSize )   // � ���� ��� �����?
        FilePos=FileSize;     // ��� ��� ����� ������ ����
    /* tran 17.07.2000 $ */
  }
  // ���������
  /* $ 27.04.2001 DJ
     ��������� �������� � ��������� �������
  */
  AdjustFilePos();
  /* DJ $ */
//  LastSelPos=FilePos;
  if(!(Flags&VSP_NOREDRAW))
    Show();
}


/* $ 27.04.2001 DJ
   ������������� ������� �������� � ��������� �������
*/

void Viewer::AdjustFilePos()
{
  if (!VM.Hex)
  {
    char Buf[4096];
    __int64 StartLinePos=-1,GotoLinePos=FilePos-(__int64)sizeof(Buf);
    if (GotoLinePos<0)
      GotoLinePos=0;
    vseek(ViewFile,GotoLinePos,SEEK_SET);
    int ReadSize=(int)Min((__int64)sizeof(Buf),(__int64)(FilePos-GotoLinePos));
    ReadSize=vread(Buf,ReadSize,ViewFile);
    for (int I=ReadSize-1;I>=0;I--)
      if (Buf[I]==CRSym)
      {
        StartLinePos=GotoLinePos+I;
        break;
      }
    vseek(ViewFile,FilePos+1,SEEK_SET);
    if (VM.Hex)
      FilePos&=~(VM.Unicode ? 0x7:0xf);
    else
    {
      if (FilePos!=StartLinePos)
        Up();
    }
  }
}
/* DJ $ */

void Viewer::SetFileSize()
{
  if(!ViewFile)
    return;

  SaveFilePos SavePos(ViewFile);
  vseek(ViewFile,0,SEEK_END);
  FileSize=ftell64(ViewFile);
  /* $ 20.02.2003 IS
     ����� ���������� FilePos � FileSize, FilePos ��� ��������� ������
     ����������� � ��� ����, ������� FileSize ���� ���� ���������
  */
  if (VM.Unicode)
    FileSize=(FileSize+(FileSize&1))/2;
  /* IS $ */
}


void Viewer::GetSelectedParam(__int64& Pos,int& Length, DWORD& Flags)
{
  Pos=SelectPos;
  Length=SelectSize;
  Flags=SelectFlags;
}

/* $ 19.01.2001 SVS
   ��������� - � �������� ��������������� �������.
   Flags=0x01 - ���������� (������ Show())
         0x02 - "�������� �����" ?
*/
void Viewer::SelectText(__int64 MatchPos,int SearchLength, DWORD Flags)
{
  if(!ViewFile)
    return;

  char Buf[1024];
  __int64 StartLinePos=-1,SearchLinePos=MatchPos-sizeof(Buf);
  if (SearchLinePos<0)
    SearchLinePos=0;
  vseek(ViewFile,SearchLinePos,SEEK_SET);
  int ReadSize=(int)Min((__int64)sizeof(Buf),(__int64)(MatchPos-SearchLinePos));
  ReadSize=vread(Buf,ReadSize,ViewFile);
  for (int I=ReadSize-1;I>=0;I--)
    if (Buf[I]==CRSym)
    {
      StartLinePos=SearchLinePos+I;
      break;
    }
//MessageBeep(0);
  vseek(ViewFile,MatchPos+1,SEEK_SET);
  SelectPos=FilePos=MatchPos;
  SelectSize=SearchLength;
  SelectFlags=Flags;
//  LastSelPos=SelectPos+((Flags&0x2) ? -1:1);
  /* $ 13.03.2001 IS
     ���� ��������� ����������� � ����� ������ ������ ���������� ����� � ����
     ����� � ������ fffe ��� feff, �� ��� ����� ����������� ���������, ���
     ������� ����� ��������� �� ������� (��-�� ����, ��� ������ ������ ��
     ������������)
  */
  SelectPosOffSet=VM.Unicode && (FirstWord==0x0FFFE || FirstWord==0x0FEFF)
           && (MatchPos+SelectSize<=ObjWidth && MatchPos<strlen(OutStr[0]));
  SelectPos-=SelectPosOffSet;
  /* IS $ */
  if (VM.Hex)
    FilePos&=~(VM.Unicode ? 0x7:0xf);
  else
  {
    if (SelectPos!=StartLinePos)
      Up();
    __int64 Length=SelectPos-StartLinePos-1;
    if (VM.Wrap)
      Length%=Width+1; //??
    if (Length<=Width)
        LeftPos=0;
    if (Length-LeftPos>Width || Length<LeftPos)
    {
      LeftPos=Length;
      if (LeftPos>(MAX_VIEWLINE-1) || LeftPos<0)
        LeftPos=0;
      else
        if (LeftPos>10)
          LeftPos-=10;
    }
  }
  if(Flags&1)
  {
    AdjustSelPosition = TRUE;
    Show();
    AdjustSelPosition = FALSE;
  }
}
/* SVS $ */


/* $ 27.09.2000 SVS
   "����" �������� Viewer API :-)
*/
int Viewer::ViewerControl(int Command,void *Param)
{
  int I;
  switch(Command)
  {
    case VCTL_GETINFO:
    {
      struct ViewerInfo *Info=(struct ViewerInfo *)Param;
      if(Info)
      {
        /* $ 29.01.2001 IS
          - ��� - ��������� StructSize
        */
        memset(&Info->ViewerID,0,Info->StructSize-sizeof(Info->StructSize));
        /* IS */
        Info->ViewerID=Viewer::ViewerID;
        Info->FileName=FullFileName;
        Info->WindowSizeX=ObjWidth;
        Info->WindowSizeY=Y2-Y1;
        Info->FilePos.i64=FilePos;
        Info->FileSize.i64=FileSize;
        memmove(&Info->CurMode,&VM,sizeof(struct ViewerMode));
        Info->CurMode.TableNum=VM.UseDecodeTable ? VM.TableNum-2:-1;
        Info->Options=0;
        if (Opt.SaveViewerPos)         Info->Options|=VOPT_SAVEFILEPOSITION;
        if (ViOpt.AutoDetectTable)     Info->Options|=VOPT_AUTODETECTTABLE;
        Info->TabSize=ViOpt.TabSize;

        // ���� ������ �������
        if(Info->StructSize >= sizeof(struct ViewerInfo))
        {
          Info->LeftPos=(int)LeftPos;  //???
        }
        return(TRUE);
      }
      break;
    }
    /*
       Param = struct ViewerSetPosition
               ���� �� ����� �������� ����� ��������
               � �������� ��������� � ����������
    */
    case VCTL_SETPOSITION:
    {
      if(Param)
      {
        struct ViewerSetPosition vsp=*(struct ViewerSetPosition*)Param;
        bool isReShow=vsp.StartPos.i64 != FilePos;
        if((LeftPos=vsp.LeftPos) < 0)
          LeftPos=0;
        /* $ 20.01.2003 IS
             ���� ��������� - ������, �� ��������� �������, ������������ �
             2 ����. ������� �������� StartPos � 2 ����, �.�. �������
             GoTo ��������� �������� � _������_.
        */
        GoTo(FALSE, vsp.StartPos.i64*(VM.Unicode?2:1), vsp.Flags);
        /* IS $ */
        if (isReShow && !(vsp.Flags&VSP_NOREDRAW))
          ScrBuf.Flush();
        if(!(vsp.Flags&VSP_NORETNEWPOS))
        {
          ((struct ViewerSetPosition*)Param)->StartPos.i64=FilePos;
          ((struct ViewerSetPosition*)Param)->LeftPos=(int)LeftPos; //???
        }
        return(TRUE);
      }
      break;
    }

    // Param=ViewerSelect
    case VCTL_SELECT:
    {
      if(Param)
      {
        __int64 SPos=((ViewerSelect*)Param)->BlockStartPos.i64;
        int SSize=((ViewerSelect*)Param)->BlockLen;
        if(SPos < FileSize)
        {
          if(SPos+SSize > FileSize)
          {
            SSize=(int)(FileSize-SPos);
          }
          SelectText(SPos,SSize,0x1);
          ScrBuf.Flush();
          return(TRUE);
        }
      }
      break;
    }

    /* ������� ��������� Keybar Labels
         Param = NULL - ������������, ����. ��������
         Param = -1   - �������� ������ (������������)
         Param = KeyBarTitles
    */
    case VCTL_SETKEYBAR:
    {
      struct KeyBarTitles *Kbt=(struct KeyBarTitles*)Param;
      if(!Kbt)
      {        // ������������ ���� ��������!
        if (HostFileViewer!=NULL)
          HostFileViewer->InitKeyBar();
      }
      else
      {
        if((long)Param != (long)-1) // �� ������ ������������?
        {
          for(I=0; I < 12; ++I)
          {
            if(Kbt->Titles[I])
              ViewKeyBar->Change(KBL_MAIN,Kbt->Titles[I],I);
            if(Kbt->CtrlTitles[I])
              ViewKeyBar->Change(KBL_CTRL,Kbt->CtrlTitles[I],I);
            if(Kbt->AltTitles[I])
              ViewKeyBar->Change(KBL_ALT,Kbt->AltTitles[I],I);
            if(Kbt->ShiftTitles[I])
              ViewKeyBar->Change(KBL_SHIFT,Kbt->ShiftTitles[I],I);
            if(Kbt->CtrlShiftTitles[I])
              ViewKeyBar->Change(KBL_CTRLSHIFT,Kbt->CtrlShiftTitles[I],I);
            if(Kbt->AltShiftTitles[I])
              ViewKeyBar->Change(KBL_ALTSHIFT,Kbt->AltShiftTitles[I],I);
            if(Kbt->CtrlAltTitles[I])
              ViewKeyBar->Change(KBL_CTRLALT,Kbt->CtrlAltTitles[I],I);
          }
        }
        ViewKeyBar->Show();
        ScrBuf.Flush(); //?????
      }
      return(TRUE);
    }

    // Param=0
    case VCTL_REDRAW:
    {
      Show();
      ScrBuf.Flush();
      return(TRUE);
    }

    // Param=0
    case VCTL_QUIT:
    {
      /* $ 28.12.2002 IS
         ��������� ���������� VCTL_QUIT ������ ��� ������, �������
         �� �������� ������� ���������� � �������� ��������� (�.�.
         ���������� ������� �� ������ �� �����)
      */
      if(!FrameManager->IsPanelsActive())
      {
        /* $ 29.09.2002 IS
           ��� ����� �� ���������� �����, � ������� ������ ���
        */
        FrameManager->DeleteFrame(HostFileViewer);
        /* IS $ */
        if (HostFileViewer!=NULL)
          HostFileViewer->SetExitCode(0);
        return(TRUE);
      }
      /* IS 28.12.2002 $ */
    }
  }
  return(FALSE);
}
/* SVS $ */

BOOL Viewer::isTemporary() const
{
  return (*TempViewName);
}
