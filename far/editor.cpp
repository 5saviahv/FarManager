#if !defined(EDITOR2)
/*
editor.cpp

��������

*/

/* Revision: 1.253 07.07.2004 $ */

/*
Modify:
  07.07.2004 SVS
    ! Macro II
  30.06.2004 SVS
    + ��������� ������� �� ������ Macro II (� "�������" ���� �� ��������, �.�. ���������� �������� MACRODRIVE2)
  16.06.2004 SVS
    - BugZ#1101 - ���������� ������ ���������
  15.06.2004 SVS
    - BugZ#1096 - shiftins � ���������� ����� - ������� ���������.
  29.05.2004 SVS
    - BugZ#1048 - ���������� ����� ����������� � ��������� �� CtrlShiftEnd
      ������ "CurLine->EditLine.ObjWidth=X2-X1;" ����� ���� ������� ��
      ������ "ProcessKey(KEY_END);"!!!
  29.05.2004 SVS
    - Bugz#794 - ������� ����� �������� ��� �������� ������
  17.05.2004 SVS
    - BugZ#1080 - ��������. CtrlIns/CtrlC ��� ��������� �� ��������
  11.05.2004 SVS
    ! � ����� � ��������� ������������ $IClip ������� ������ �
      UsedInternalClipboard ��� Ctrl-P � Ctrl-M
  02.03.2004 SVS
    ! ��������� Editor::SaveData()
  24.02.2004 SVS
    - Ctrl+End � ���������
  12.01.2004 IS
   - ���: ������� "������ �� ������", ����� ����������� ���� �������� �������
     � ������ ������ 128. �������: ��� ��������� � WordDiv ����������
     IsWordDiv, � �� strchr
  14.12.2003 SVS
    - BugZ#1002 - ESPT_SETWORDDIV �� ������ �� ������� �� ������
  04.11.2003 SKV
    ! shift-left shift-right ������ �� ��������� ������� ���������.
  26.10.2003 KM
    ! ��������� ������ GlobalSearchString ����� ������� ������ � 16 �������������,
      ����� ����� ������������ � � LastSearchStr �������� ���� ����� � ��� �������������
      ������������� � ������� ��������� ������.
  20.10.2003 SVS
    ! ��������������
        KEY_MACRO_EDITSELECTED -> KEY_MACRO_SELECTED
        KEY_MACRO_CHECKEOF     -> KEY_MACRO_EOF
    + ��������� KEY_MACRO_EMPTY � KEY_MACRO_BOF
  20.10.2003 SVS
    ! ��������� ������� ��� ������� ����
  13.10.2003 SVS
    ! ESPT_WORDDIV -> ESPT_SETWORDDIV (NotInternal)
  10.10.2003 SVS
    + EditorOptions.WordDiv
    + ESPT_WORDDIV
  09.10.2003 SVS
    ! ������ ����������� "��� ��������"
    - ������ � ������������� ����������� �������
  04.10.2003 SVS
    + KEY_MACRO_CHECKEOF - �������� ����� �����
  26.09.2003 SVS
    ! ��������� � ��������� �����������
    + ��������� ��������� Ctrl-Q � ��������� ������ - ������ '"'
  17.09.2003 SVS
    - ����������� "��������" � ������� ��� ������� KEY_MACROPLAINTEXT � KEY_MACRODATE.
      �� ���� ��� ����, ��� $Date ����� ���� ��� ����������
    - ���������� ������� (��������, Alt-Shift-Left=Alt-Left) ����� 1687
  12.09.2003 SVS
    ! ����������� ������� ����� ��� ������� Plain-text �� �������.
      ��� ����� ������������� ����� ��������
      CtrlObject->Macro.GetMacroPlainTextSize(), ����� ��������
      ������ ������.
  08.09.2003 SVS
    + ��������� KEY_MACROPLAINTEXT
  02.09.2003 SKV
    - ���� ��������� shift-up ��� ����������� cursor beyond end of line
  31.07.2003 SKV
    - ���� ��������� ������������� ������ ����� Shift-[A-Z0-9]
  25.07.2003 SVS
    ! �������� SetLastError � Editor::ReadFile � ������ �������.
  15.07.2003 SVS
    - Del/Bs � ����� ��������� ������� ������ �� �������, ������� �� ������������...
  05.06.2003 SVS
    - XLat, ���� ������ �� ����������, ��� ����� ������ ����������� ������.
  05.06.2003 SKV
    - ������ ��������� � ������ � ���������� � ������ !CursorBeyonEOL.
  06.05.2003 SVS
    ! ������ � ���������� �������� � ��������� ������� SetBookmark() � GotoBookmark()
  28.04.2003 SVS
    ! �������� ��������� SaveData + ������� ��������� ���������
  15.04.2003 SVS
    - ��������� �������� ��������� �������������� ������� (��������, F1 -
      ����� ������) �� ����� ������ ��������, ������������ ProcessEditorInput
      (��������, AutoWrap)
      ��������� ECTL_PROCESSINPUT � ECTL_READINPUT �� Editor � FileEditor
  15.04.2003 VVM
    ! ����� ������ �������� �� �������� � �������� �� ���������� �������� ������
  31.03.2003 SVS
    + _SYS_EE_REDRAW
  05.03.2003 SVS
    + Editor::ReadData, Editor::SaveData (���� ��������� � ����� ������ ������) - ��� DI_MEMOEDIT
    ! ���������� _SVS
  25.02.2003 SVS
    ! "free/malloc/realloc -> xf_*" - ���-�� � ������� ��� ���������.
  14.02.2003 SVS
    ! ����������� ����� ��� ECTL_
  05.02.2003 SKV
    - little selection fix (deselection with shift-down on lines with tabs).
  26.01.2003 IS
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  24.01.2003 KM
    ! �� ��������� ������ �������� �� ����� ������ ��
      ����� ������������ ������. � �� �� �������������
      �������� ���������� ��������� ���������.
  10.01.2003 SVS
    ! Ctrl-P/Crtl-M � ��������� ��� ���������� ������.
      ������� ������� ��������� ���� ��������, �������� �������� ��������
      (see ��������� UsedInternalClipboard � clipboard.cpp)
  17.12.2002 SVS
    ! ������� ������� ������ � EditorPosCache (see ����� FilePositionCache)
  11.12.2002 SVS
    - � EditorControl (ECTL_READINPUT) ������� ������� ������� ��� KEY_CONSOLE_BUFFER_RESIZE
  05.12.2002 SVS
    ! �������� ����� MkStrFTime(), �������� ������� ����
  12.11.2002 DJ
    - ����������� ��������� Ctrl-Shift-Left �� ������ ������
    ! ������ ECTL_GETSTRING ��� ��������� �� ������ ������ ���������� ���������
      ����������, � �� �����������������
  10.11.2002 SKV
    - BugZ#690, ���������...
  08.11.2002 SVS
    ! ��������� ������ �������� Editor �� ������� "����":
      * Editor::PluginData ����� � FileEditor::PluginData
      * Editor::SetPluginData() ����� � FileEditor::SetPluginData()
      * ������� ECTL_EDITORTOOEM, ECTL_OEMTOEDITOR � ECTL_GETBOOKMARKS
        ����������� �� ������ FileEditor (��� DI_MEMOEDIT ����� ������)
      * ECTL_GETINFO ������ �� 2 �����. � FileEditor ����������� ����
        EditorInfo.FileName.
      * KEY_CTRLF - ������ �������������� � FileEditor::ProcessKey()
      * �� Editor::~Editor() ������� ���� �������� ����, ��� � ������ ����
        � FileEditor::~FileEditor()
      * Editor::ReadFile() - Name ��� � ������ �������!!!
        ������ ����������� "��������������" - ������ ���� � ����� ������
  07.11.2002 SVS
    ! ������� _SVS() ��� ������� :-)
  04.11.2002 SKV
    - Undo �� ��������� ������
  28.10.2002
    - ��������� � ������.
  18.10.2002 SKV
    - UnmarkEmptyBlock, �� ���� ������ ��� �������.
  14.10.2002 SVS
    - BugZ#684 - ECTL_SETPOSITION � ������������� ��������
  14.10.2002 SKV
    ! ��������� ���������� ������
  09.10.2002 SKV
    - selection bugs
  26.09.2002 SKV
    - ��� ��� �������� �����
    - ��� ��� ���������
  21.09.2002 VVM
    - ������� ��� �������� �����. �������� ����.
  19.09.2002 SKV
    - BugZ#643. ���������, ����� ������ � ������.
  18.09.2002 SKV
    - ����� ���������. �� ��� ��� � !EdOpt.CursorBeyondEOL ������.
  17.09.2002 SKV
    - BugZ#538, ����������� ������ ���������.
  17.09.2002 SVS
    - BugZ#616 - ctrl-shift-alt � ���������
      "�� ������ ������ ������ ("replace"). ������� ctrl-shift-alt,
      �� ��������, �� ��� ��� ����������� ��� ���� - "search".
      �� ������ �������."
  17.09.2002 SKV
    - ���������
  04.09.2002 SVS
    ! ��������� EditList ������� �� editor.cpp � edit.hpp
    ! ����� Editor "�������" �������� ���������� ����� ��������������,
      ������ ��� ���������� FileEditor`�
    ! ������� ECTL_SAVEFILE �������������� � FileEditor::EditorControl
    ! ������� ECTL_QUIT �������������� � FileEditor::EditorControl
    ! ������� ECTL_SETTITLE �������������� � FileEditor::EditorControl
    ! ��� Editor::ShowStatus() �������� � FileEditor::ShowStatus()
  27.08.2002 SVS
    ! ������� EditorInfo.WindowPos
  19.08.2002 SVS
    + ECTL_DELETEBLOCK - ������� ���� � ���������. ������� ������ TRUE
      � ������ �������� �������� ����� � FALSE, ���� �������� ������������
      (������������ ����� Ctrl-L) ��� ��� ����������� �����.
  06.08.2002 IS
    - ���: ������� � ECTL_SETSTRING ��� ������������� StringLength.
      ��������� ������������ StringLength � ������ FALSE, ���� ��� ������
      ����.
  23.07.2002 SKV
    - ��� ���� ������� ����������� � ����������.
  12.07.2002 SVS
    ! ��������� "������" ��� "Editor Not File" - ��������� ��������� F1
      �� Editor � FileEditor
  10.07.2002 SKV
    - Shift-Up �� ������ �������.
  02.07.2002 SKV
    - Bugz#538 - � ECTL_SETPOSITION Pos ����������� �� ������ �� -1,
      �� � �� ������ ���������� � ������� �������� ����� ������������
      FEDITOR_CURPOSCHANGEDBYPLUGIN.
  25.06.2002 SVS
    ! ���������:  BitFlags::Skip -> BitFlags::Clear
    ! � ��������� (Editor) � ������� (Viewer) ������������ �� ���. ��������
      �������� � ������ ������������� (�.�. ������ ScrX/ScrY ���������
      ObjWidth/ObjHeight).
    ! ���� ��������/������ �� � FullScreen, �� �� ���������� ������
    ! ������ Editor ����� ������� ������ - ��� ���������� FileEditor
      ������ ECTL_SETKEYBAR �������� �� Editor � FileEditor
  24.06.2002 SKV
    - ��������� ������ PersistentBlock � ������������� ����� �� �����.
  24.06.2002 SKV
    - ������������ ���������.
  24.06.2002 SKV
    - ��� Shift-Down ���� ���������� FEDITOR_CURPOSCHANGEDBYPLUGIN.
  17.06.2002 SVS
    - BugZ#557 - FAR builtin editor crash on invalid attributes of LastPositions
  14.06.2002 IS
    ! ���� SetDeleteOnClose ��������� � editor.cpp
    ! �������� � SetDeleteOnClose ���� int:
        0 - �� ������� ������
        1 - ������� ���� � �������
        2 - ������� ������ ����
    + ��������� FEDITOR_DELETEONLYFILEONCLOSE
  10.06.2002 SVS
    - ����������� ��������� ��������� ��� $Date & Ctrl-F
  24.05.2002 SVS
    + ������������ Numpad-������
    ! "FAR_VerticalBlock" -> FAR_VerticalBlock
  24.05.2002 SKV
    - ������ ������ �� 0x0d, �������� ���������. (Bugz#524)
  22.05.2002 SVS
    ! CurrentEditor ���������������� � FileEditor
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
    ! ����� - ������ � ���� ��������������� ����������
  13.05.2002 VVM
    + EditorInfo.WindowPos - ����� ���� ���������. ����� �������������� � ACTL_*WINDOW*
  06.05.2002 KM
    - AltF8 AltF9... Oops!
  29.04.2002 SVS
    - �� ���� ������� ���� BugZ#488
  29.04.2002 SVS
    - BugZ#488 - Shift=enter
  18.04.2002 SKV
    ! ��� ������ ������� �� ���� ������������� ����� / � \
    ! �������������������� ������� � topline ��� alt-f9 � ����� �������� �����.
  04.04.2002 SVS
    ! ECTL_TURNOFFMARKINGBLOK -> ECTL_TURNOFFMARKINGBLOCK
  04.04.2002 IS
    + ECTL_TURNOFFMARKINGBLOK: ������ ���� ��������� "��������������
      ��������� �����"
  30.03.2002 IS
    - ������ ���� ������������ FEDITOR_WASCHANGED � Undo (��. ����������� � ����
      �������) � ������� ������ ���, ����� ��� �������� ��������� ���� � �����,
      ������� ��� ������� �� ���� �� F4 � ��� �������, � ����� ��������� ����
      �������� ��� ������ Alt-BackSpace.
  23.03.2002 IS
    + ESPT_LOCKMODE - ���������/�������� ��������� ����� �� �������
  23.03.2002 VVM
    + ��������� �� 1 ������� � ������� ����. (������� ������)
  19.03.2002 SVS
    + ���������� ������ MAC_EOL_fmt
  18.03.2002 IS
    - ������� ���������� �� ������� ��������� ��� ������� �������� �
      Shift-Down, Shift-End (BugZ#319)
  10.03.2002 IS
    ! UnmarkEmptyBlock ������ ��������� �������� � ������������� �������
      (������ �� �� ������ ������ � ������� ��������� ���� ��� �� ������ �����)
  07.03.2002 IS
    + UnmarkEmptyBlock(): ������� ���������, ���� ��� ������ (�������� ����
      �������� � ������)
    ! BugZ#328: Del �� ��������, ����� Shift-Left, Shift-Right, Del
      ([ ] ���������� �����, [x] Del ������� �����)
    ! BugZ#326: �� ���������� ������ ESPT_CHARTABLE (��������� ������ ������
      � ���� �� Shift-F8)
  06.03.2002 SVS
    - ���� � ���������� ��� ���������� ������.
  03.03.2002 SVS
    ! �� ������ ����� (���� �� 28.06.2000) "����� 1000 �������"
      ������ ���������� ������� � ����� "�����, ��������, ��������",
      ���������� ����� �������� - "��������" :-)
  22.02.2002 SVS
    ! ��������� � ����� � �������� FAR_INT64
  21.02.2002 SKV
    - ����� EOL'� ������ ��� ������� ����.
  19.02.2002 SVS
    - �� ������ Ins - ���� �� �������� ������� - ������ ������� �� ��������� :-(
  15.02.2002 SVS
    ! ����� ShowProcessList() ������� � �������
    ! ��� Ctrl-L ������� ���������� �� ����� ������, � ������ ��������� �����
    ! Ins - ���� ������ �������������� ���� �����
  06.02.2002 IS
    - ������� ��� ��������� CtrlShiftEnd, ������� �������� ����� 1224
  05.02.2002 SVS
    ! ��������������� ���� - ��� �������
  05.02.2002 SVS
    ! Editor::IsShiftKey() -> keyboard.cpp::IsShiftKey()
  04.02.2002 IS
    ! ������� �������� ������� ��������� � shift-(left|right) � shift-end
      ����� ��������� ���� 280.
  29.01.2002 SVS
    - ������������ �����, Alt-F8 Enter - ��������� ����� �� ���������
  28.01.2002 VVM
    + FreeAllocatedDaat()
  19.01.2002 IS
    ! ���������� �������� ��������� �� ctrl-shift-(left|right) �� �������
      ���� - � shift-(left|right), ����� ������ ���� 226.
  18.01.2002 SKV
    - CtrlShiftEnd bug fix
  15.01.2002 SVS
    ! ����� ������ ���� ���������� ���� int
    ! ������ ����� �� ���������� ������ Editor ����� "����"
    ! ProcessEditorInput ���� � FileEditor (� ������� ������� ��...)
  14.01.2002 SVS
    ! DOS_EOL_fmt[], UNIX_EOL_fmt (� global.?pp)
    - ��� �� ���������� ��� MSVC ����� 1168.
  12.01.2002 IS
    ! ������ "\r\n" � "\n" ���������� ����������� ���������: DOS_EOL_fmt �
      UNIX_EOL_fmt.
    ! �������� ��� ����� ������ �� ��������� ���� - DOS_EOL_fmt.
    ! ������� const
  11.01.2002 IS
    - �������� ��������� ����� 1160
  10.01.2002 SVS
    + ECTLToName - ��� �����
  07.01.2002 IS
    - ���������� �� ��, ��� �����, ��� ������������� ctrl-shift-(left|right),
      ���� ����� ���� ������� ���� �������� ��������, � �� ����� �����, � ���
      ���� ������� � ������� ����� ���������� �������
  28.12.2001 SVS
    ! ������ � ������ ��������� �������� (��� ��������� union)
  28.12.2001 VVM
    ! ��������� �� �������� ������ ����� ������� ����� ���������.
  25.12.2001 SVS
    + ��� ��������� �������� ������� ����� ����������� ��������� �������
      ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL).
      �� ����� ���... ������� ��������� ������ �� EEREDRAW_CHANGE
      (��� ���� ������� �������� ;-()
  24.12.2001 SVS
    - ���� ���������� � Editor::ProcessEditorInput - ������ ������
  14.12.2001 IS
    ! stricmp -> LocalStricmp
  14.12.2001 IS
    ! ��������� const � ������������ � ����������� ������ Edit
  10.12.2001 SVS
    - BugZ#164. ��� ������� ����� ����� (Ctrl-F) ��������� ������������
      ��� �����.
  03.12.2001 IS
    ! UndoData - ������ ���������. ������ ������ undo ����� ��������
      ����� ������.
  03.12.2001 VVM
    ! 1. ����� 2 ������, ������ ������� ������.
      2. ������ �� 2 ������ �������� ����� ����� ������, �� �� ������.
      3. Shift-Up; Shift-End
      4. ����� �������� ������ ����, �� ��������� � ������...
  09.11.2001 IS
    -  ��������� ����� - EditorF7Rules,
       ����� ������, �.�. �� ������������� ����������� � techinfo.
  05.11.2001
    SVS: ! ESPT_SETTABLE -> ESPT_CHARTABLE: ��� ��������� ESPT_* ����
           �������������, �� SET  �  ��  ��������  ����.
    IS:  ! ��� ��������� ����� ������� �� ESPT_SETTABLE ������� ��� ��� ����
           (������ �������� oem).
  01.11.2001 SVS
    ! ECTL_GETBOOKMARK, EditorBookMark -> ECTL_GETBOOKMARKS, EditorBookMarks
    ! � ������� IsBadWritePtr �������� �� �������� �������� ������������
  29.10.2001 IS
    ! SaveEditorPos � SaveEditorShortPos ��������� � EditorOptions
      ������ ��� �������� ��� ������ ����� ���������, � ������ ����� �����
      ���� �������� ��������.
  28.10.2001 SVS
    ! �������� � ������ ����������� ������� �� ������� ����� (�� �� ��� �
      � �������)
  24.10.2001 SVS
    ! ��������� ������� ����� ����� � ���� �������� �� ������� ���������, �.�.
      ��� ��������� ����� � ��������� ����� �������� ������.
    ! ��������� �������� ������������������ ������ �������� - DeleteBlock()
      ��� ����� ���������� ����
    - ����������� �������� � Ctrl-F - ��� ���������� �� ������������� �����
      ��� ���� �� ���������.
  21.10.2001 SVS
    + CALLBACK-������� ��� ���������� �� BugZ#85
  19.10.2001 OT
    - ����������� ������ HyperViewer
  16.10.2001 SKV
    - ��� ��� ��������� ������������ ������
      ����� ������������ EdOpt.CursorBeyondEOL ���� ����.
  15.10.2001 SVS
    + _KEYMACRO()
  11.10.2001 IS
    + ��������� DeleteOnClose
    ! ��������� const
  11.10.2001 VVM
    ! ������ ��� ����� ��������� CTRL+F (������ CTRL+SHIFT+ENTER)
  10.10.2001 SVS
    + EditorInfo.CurState
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  24.09.2001 SKV
    - ctrl-left fix
  18.08.2001 SVS
    ! �������� � ������� Paste - ��� ��������� $Date, � ������� ���� '%n',
      �������������� ������� ��� ��������� "�������" KEY_MACRODATE.
  14.09.2001 SVS
    ! ������� SysLog`��
  13.09.2001 SKV
    - Shift-End �� ��������� ������ � ����� � ��������� ������ ����.
  30.08.2001 IS
    - �������������� ��������� ��� ������������� ECTL_SETPOSITION: �����
      Overtype �� �������, ���� � ������ ��� � ���� �������.
  07.08.2001 IS
    + ��������� ESPT_SETTABLE - ����� ���������.
  25.07.2001 IS
    - ���: � ���� �� shift-f8 ���������� ������ � ��� ����������, �������
      ������������� ������������ ��� �������� �����, ���� ���� �������
      ��������� ���� ������. �������: ��� �������� ����� �������������
      ������� ����� ������� ��������, ���� UseDecodeTable==FALSE.
  24.07.2001 IS
    ! ������ �������� �� ' ' � '\t' �� ����� isspace
  10.07.2001 SVS
    + ��������� KEY_MACROXLAT
  27.06.2001 SVS
    - Stream Block ����� ������� � ���������� ��������� :-((
  25.06.2001 IS
    ! ��������� const
  25.06.2001 SVS
    ! ����� SEARCHSTRINGBUFSIZE
  22.06.2001 SVS
    + ��������� KEY_MACRODATE
  11.06.2001 SVS
    ! ����� ��������� � GetSearchReplaceString() - ����������� ������� �������
  10.06.2001 IS
    - ���: �����-�� ��� ����������� _���������_ ������ �������������� �� ���
      _������_.
  08.06.2001 IS
    - ���� � ��������� ������ ��-�� ������������� strcpy, � �� strncpy, �
      ���������� ���� ������ � ������.
  06.06.2001 SVS
    + EDITOR_UNDO_COUNT - "�� ����� � ��������"
    + ECTL_GETBOOKMARK - �������� ���� � ���������.
    ! ��������� ��������� � ������ InternalEditorBookMark.
  04.06.2001 IS
    - Editor::SaveFile - ������ (� ������ SVS) ������������� ���� - ����� ��
      ������� ��� �� ����, ��� ������������� �������� �����
  03.06.2001 OT
    - �� ���������� StatusLine ����� DrawLine � ���������
  31.05.2001 OT
    ! ����������� � SaveFile() ������ ���� - ������������� ��������� ���� SAVEFILE_XXX
  25.05.2001 IS
    - ��� ������� ��������� ����� �� ������ ������ ������ ������ ���������� �
      �������� ���������. ��� ��� ����, ����� :)
  24.05.2001 IS
    ! ����� ������ ����������� 592 (shift-home/end)
  19.05.2001 IS
    - ������� �������� ���������� ����������� ��������� (������� ������ ����
      ��������� � ������ ������ ��� �����������) � ������� ��� �������
      ����-���� �� ������ ������.
  16.05.2001 DJ
    ! proof-of-concept
  15.05.2001 OT
    ! NWZ -> NFZ
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
    ! Search ������ ���������� TRUE/FALSE
  07.05.2001 IS
    - ����: �� shift-f7 ������������ ������, ���� ������ ��� ���� �����
      � Paste �������� ������������� ��� � delete []
  06.05.2001 DJ
    ! �������� #include
  05.05.2001 IS
    ! shift-home/end - ��������� ��������� � ����, ����� ���� �� 592
  04.05.2001 OT
    + ������� ������������� ���� �������� �� F11 (NWZ)
      ��������� PluginSet::CommandsMenu()
  27.04.2001 SVS
    - Ctrl-Q: �� ����������� ������� �����������
    - Ctrl-Q: �� ���������� ������ � ����� ������
    - XLat: �� ���������� ������ � ����� ������
  27.04.2001 VVM
    + ��������� KEY_MSWHEEL_XXXX
  25.04.2001 SVS
    + KEY_MEDIT_ISSELECTED, � ����� �� ������� Editor::ProcessKey ����������
      TRUE - ���� ���� ���������� ���� ��� FALSE - ����� ����.
  25.04.2001 IS
    + ctrl-shift-enter - �������� � ������ ������ ��� �������������� �����
  20.04.2001 IS
    ! _������������_ ��������� ��������� ������ ��� ������ shift-home ���
      shift-end. ������� ����� �������� �� �������� ���������� �� ������
      ������������ ��������� �� ����� �����, � ����� �������������������, ��
      ��� ���-������ � ��������.
  11.04.2001 SVS
    + ��������� ��������� Ctrl-Q - ��� ������� ������� �� �������� ����
      (���������� ����� ���������)
  10.04.2001 SVS
    - ���� ���� RO � �� ���������� ��� ��������� - ��� ����� �������
      ��������� �����, ��� ��� ���� ������! � ��� ��� ���, ��� ������ ��
      ������� ���� ��� ������� ������ - ���������, ���� :-((((
    ! ����������� �� ���������.
    - ^P/^M - ����������� ��������: ������ ��� CurPos ������ ���� ">=",
      � �� "������".
    - ������ Pasting ��������� :-( - ��� Shift-Ins ��-�� ���� ��� �� �������
      ������� �����������.
  03.04.2001 IS
    + ��������� ESPT_AUTOINDENT, ESPT_CURSORBEYONDEOL, ESPT_CHARCODEBASE
      (��� � ECTL_SETPARAM)
  28.03.2001 SVS
    + �������������� �������� ��� SaveFile() - SaveAs
  28.03.2001 VVM
    ! ��� �������� �� ������ ���������� ������� �� ������ ������.
  26.03.2001 SVS
    + ��� ������ ������ �������� ������� �������� � ���, ����� �� �����
      ���������������� ����� "Editor" ��� Shift-F1 �� ������ ��������
  20.03.2001 SVS
    + ��� ������ ��������� � ������� ����� ���������� ��� ������ �
      ���������� ���������� ������ ��������������.
  15.03.2001 OT
    - ���� ������ � ��������� <Enter> � ����� �����, � ����� ������� UNDO...
  12.03.2001 SVS
    ! ��������� � ����� � ����������� � ������ int64
  27.02.2001 SVS
    + � ��������� ������ ���������� ��� ������� � ����������� �� ���� -
      Oct, Dec ��� Hex
  27.02.2001 IS
    + �������� ������ ������� ��������� �� ���������� �������� � SetTabSize
  26.02.2001 IS
    ! ����� ��������������� ���������� �������� ���������������� ��
      EditorOptions. ���� ���� ��� ����� �������, �� � ���-�� ��������� :)
    + SetCursorBeyondEOL
  21.02.2001 IS
    ! Opt.TabSize -> Opt.EdOpt.TabSize
      Opt.EditorPersistentBlocks -> Opt.EdOpt.PersistentBlocks
      Opt.EditorDelRemovesBlocks -> Opt.EdOpt.DelRemovesBlocks
      Opt.EditorExpandTabs -> Opt.EdOpt.ExpandTabs
      Opt.EditorAutoIndent -> Opt.EdOpt.AutoIndent
      Opt.EditorAutoDetectTable -> Opt.EdOpt.AutoDetectTable
      Opt.EditorCursorBeyondEOL -> Opt.EdOpt.CursorBeyondEOL
      Opt.EditorBSLikeDel -> Opt.EdOpt.BSLikeDel
  19.02.2001 IS
    - ����� ������������������� AttrStr
  16.02.2001 IS
    + ��������� ECTL_SETPARAM, ���� ������ ESPT_TABSIZE � ESPT_EXPANDTABS
  15.02.2001 IS
    ! Opt.EditorDelRemovesBlocks -> DelRemovesBlocks
      Opt.EditorPersistentBlocks -> PersistentBlocks
    + SetDelRemovesBlocks, SetPersistentBlocks
  15.02.2001 IS
    + ������� ������ ��������� ��� ���� Edit � ������� SetTabSize
    ! Opt.EditorExpandTabs -> ConvertTabs
    + SetConvertTabs
    ! SetTabSize ���������� ����� ����� SetConvertTabs
  14.02.2001 IS
    + ������ ��������� �������� � TabSize, �������������� �� ����� ��� ������
      GetTabSize, SetTabSize
  14.02.2000 SVS
    ! ������������ ������ ��� ���������� �����
  14.02.2001 VVM
    + ��� ���������� ����� AltU/AltI �������� ������� �������
  13.02.2001 IS
    ! ������� � ���������� ���������������� ��� �� ����������, � �� ��� ������
      ���������� ������. �.�., ����������� ���� ����� ���� ������ ;)
  12.02.2001 IS
    ! �������� ����������� ������ � ���� ������, � �� ��� ������ ����������
      �������
  08.02.2001 skv
    - EEREDRAW ��� Ctrl-P, Ctrl-M
  03.02.2001 skv
    - EEREDRAW_ALL ������ ������������� ����������������,
      ��� �������� � ����� ������ �� ������ ��� �����������
      cursor beyond end of line �������� ������.
  21.01.2001 SVS
    ! ������� ������/������ ������� �� Editor::Search � ���������
      ������� GetSearchReplaceString (���� stddlg.cpp)
  14.01.2001 tran
    - ����� ���, ����� ��� ������ ��������� �� �������
      ����������� �����
  07.01.2001 IS
    - �� ��������� �������� �������� �� ������ ��� �������� ������,
      ����������� ������������ ������ (�� ������ �����������)
  28.12.2000 VVM
    + ������ ����� ������� ������������ ���� ������.
  23.12.2000 OT
    - �������� ������� CtrlShiftLeft � CtrlAltLeft �� ������������
      ������� ������
  22.12.2000 SVS
    - ����� �� EE_READ ������� ECTL_SETKEYBAR �������� � ������� ����, �.�.
      ������ EditKeyBar ��� �� ����������.
  21.12.2000 SVS
    - � ���������� ����������� ���� ������ �������� ������� ���
      ������� EditorF7Rules
  16.12.2000 OT
    - CtrlY �� ��������� ������ � ���������� ������������ ������ �� ������
      ���������
  15.12.2000 SVS
    ! ��������� �� ������ ����, ��� ������� GetFileAttributes()
  10.12.2000 IS
    ! ������������ ��� Xlat ������ �� �����, �� ������� ����� ������, ��� ��
      �����, ��� ��������� ����� ������� ������� �� 1 ������
  08.12.2000 SLV
    - Ctrl-Del � ������ ������ ��� ���������� ����� �
      ���������� Opt.EditorDelRemovesBlocks.
  03.12.2000 IS
    + ���������� � ��������� ������ ������� RSH ��������������� �������� �����,
      ���� ��� �����������.
  03.12.2000 SVS
    + "���� ���� ����� ������� ReadOnly..." ����� System � Hidden - ��������
      ��������.
  29.11.2000 SVS
    + ���� ���� ����� ������� ReadOnly ��� System ��� Hidden,
      �� ����� ����� ���� - ����������� ����������.
    + Opt.EditorFileSizeLimit - ���������� ���������� ������ �����, �����
      �������� ����� ����� ������ � ���������������� �������� ���������
      ����� �� ��������������
  28.11.2000 SVS
    + Opt.EditorF7Rules - ������� �� ���� ������ � ���������
      "�, ��� �� ������ - ��� �������� ���� ���������" :-)
      ����� ��������� ����� ��������������.
  25.11.2000 IS
    + ���� ��� ���������, �� ���������� ������� �����. ����� ������������ ��
      ������ ����������� ������ ������������.
  04.11.2000 SVS
    + �������� �� �������������� ������� ��� XLat-�������������
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  23.10.2000 tran 1.40
    ! ��, ��������� � CurBeyondEOL
  16.10.2000 tran 1.39
    ! ������ ����� ���� � ������� �������, � ��������� - �� ��������� (FGWL#10)
  11.10.2000 SVS
    ! Bs ������� ���� ��� ��, ��� � Del
    - "�������������, ��������, ��� ������ - �������� ���� :-("
  01.10.2000 IS
    ! ���������� ����� ����� � ��������� ������
  24.09.2000 SVS
    + ������ �� ����������/�������������� ������� � ����� �� RCtrl+<N>
    + ������������� Xlat
  20.09.2000 SVS
    - Bugs � "�������" ��������� (�� �������) �� �� ������!
  20.09.2000 SVS
    ! � Replace ������� ��� ������ replace ������ ���� DIF_USELASTHISTORY
    ! ���� ��� ������ �������� All, �� ��� ��������� Shift-F7 �����
      ���������� ������ � ������������� ��������.
  13.09.2000 skv
    ! EE_REDRAW ���������� � �����������. 1 � 2 ��������.
  07.09.2000 skv
    + ECTL_PROCESSKEY
  07.09.2000 skv
    - �������� ������� replace ��� ������������� �������������
  30.08.2000 tran 1.21
    - bug � �����������, ��������� ������ 66
  15.08.2000 skv
    ! ����������� Replace.
  10.08.2000 skv
    ! ����������� ������ EE_REDRAW ������� ���������.
  07.08.2000 SVS
    + ECTL_SETKEYBAR - ������� ��������� Keybar Labels
  03.08.2000 KM 1.17
    ! � ������� Search ��������� ����������� ������ ����� ����.
  03.08.2000 SVS 1.16
    ! WordDiv -> Opt.WordDiv
  01.08.2000 tran 1.15
    + DIF_USELASTHISTORY � �������� ������,������ � ��������
  25.07.2000 tran 1.14
    - Bug 22 (�������)
      ����������� ��������� alt-left,alt-right
      �� ������� �������� �� ������� �����
  21.07.2000 tran 1.13
    - Bug 22
      ��� ������ ��� ����� �������.
      ���������� ������ ������� ��������.
      ��� ��� ����� ����������
      ���� ��� ����� ������
        int  GetLineCurPos(); - ������ ��������� ��������
        void BeginVBlockMarking(); - �������� ������������ ����
        void AdjustVBlock(int PrevX); - ������������ ������������ ����
                ��� ����������� ������� ��� �������� �����
                ������ ����� ���������
      ������� �������� ��������������� SysLog
      ���� ���-�� � ���� ����� ��������, � ������� ���������
      ���� ��� - ����� ��� �����...
  21.07.2000 tran
    ! ��� ������ ������� GoToPosition();
  18.07.2000 tran
    - Bug #22
      ������ � ������ ������, ����� alt-right, alt-pagedown,
      ��������� ���� ������� � 1 �������, ����� ��� alt-right
      ��������� ���������
  17.07.2000 tran
    - ��� � ������������ ��� [ ] Expand tabs to spaces
      � ����� ����� ����� ����� ����� ������� '\t'
      ����� � ����� ������ ����������� ������� (���� \t)
      � ������ ��������� �� ���������� ������ ��������
      ������ �� �� ��������� � �� ����� ������ ��������
      ��� ���������� �� ������
      ����� {} ���-���, �������� ������ ������� ������ �������,
      ����� �� �����... :)
  17.07.2000 OT
    + ���������� ����� ��� ���������� "�����" ���������
  14.07.2000 tran
    + ������� �� ��������
      ������ 50%, �������� ����� � ��������
      ������� GetRowCol ����� ������� ������
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 tran
    + ������ ������� �������� � ������ ������ �������.
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  07.07.2000 SVS
    + �������������� ���� WordDiv ��������� ������ � global.cpp �
      ������� �� ������� (����� ��� ��������������)
  07.07.2000 tran & SVS
    + in AltF8 - row,col support
  29.06.2000 IG
    + CtrlAltLeft, CtrlAltRight ��� ������������ ������
  28.06.2000 tran
    - trap ��� ������� ������������� ����� ����� 1000 �������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "editor.hpp"
#include "edit.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "macroopcode.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "poscache.hpp"
#include "chgprior.hpp"
#include "filestr.hpp"
#include "dialog.hpp"
#include "fileedit.hpp"
#include "savescr.hpp"
#include "scrbuf.hpp"
#include "farexcpt.hpp"

static struct CharTableSet InitTableSet;
static int InitUseDecodeTable=FALSE,InitTableNum=0,InitAnsiText=FALSE;

static int ReplaceMode,ReplaceAll;

static int EditorID=0;

// struct EditorUndoData
enum {UNDO_NONE=0,UNDO_EDIT,UNDO_INSSTR,UNDO_DELSTR};

Editor::Editor()
{
  _KEYMACRO(SysLog("Editor::Editor()"));
  _KEYMACRO(SysLog(1));
  /* $ 26.02.2001 IS
       �������������� ���������� ����� ����� ;)
  */
  memcpy(&EdOpt, &Opt.EdOpt, sizeof(EditorOptions));
  /* IS $ */

  /* $ 26.10.2003 KM
     ���� ���������� ���������� ����� ������ 16-������ �����, �����
     ������������� GlobalSearchString � ������, ��� ��� �������� ������ �
     16-������ �������������.
  */
  if (GlobalSearchHex)
  {
    int LenSearchStr=sizeof(LastSearchStr);
    Transform(LastSearchStr,LenSearchStr,GlobalSearchString,'S');
  }
  else
    strncpy((char *)LastSearchStr,GlobalSearchString,sizeof(LastSearchStr)-1);
  /* KM $ */

  LastSearchCase=GlobalSearchCase;
  /* $ 03.08.2000 KM
     ���������� ��� ������ "Whole words"
  */
  LastSearchWholeWords=GlobalSearchWholeWords;
  /* KM $ */
  LastSearchReverse=GlobalSearchReverse;
  memcpy(&TableSet,&InitTableSet,sizeof(TableSet));
  UseDecodeTable=InitUseDecodeTable;
  TableNum=InitTableNum;
  AnsiText=InitAnsiText;

  DisableOut=0;
  Pasting=0;
  NumLine=0;
  NumLastLine=1;
  LastChangeStrPos=0;
  BlockStart=NULL;
  BlockStartLine=0;
  TopList=EndList=TopScreen=CurLine=new struct EditList;
  TopList->EditLine.SetObjectColor(COL_EDITORTEXT,COL_EDITORSELECTEDTEXT);
  /* $ 14.02.2001 IS
       ��������� ������ ������ ���������
  */
  TopList->EditLine.SetTabSize(EdOpt.TabSize);
  /* IS $ */
  TopList->EditLine.SetConvertTabs(EdOpt.ExpandTabs);
  TopList->EditLine.SetEditorMode(TRUE);
  TopList->EditLine.SetWordDiv(EdOpt.WordDiv);
  TopList->Prev=NULL;
  TopList->Next=NULL;
  /* $ 12.01.2002 IS
     �� ��������� ����� ������ ��� ��� ����� ����� \r\n, ������� ������
     ������� �����, �������� ��� ����.
  */
  strcpy(GlobalEOL,DOS_EOL_fmt);
  /* IS $ */
  /* $ 03.12.2001 IS ������ ������ undo ������ ����� �������� */
  UndoData=static_cast<EditorUndoData*>(xf_malloc(Opt.EditorUndoSize*sizeof(EditorUndoData)));
  if(UndoData)
    memset(UndoData,0,Opt.EditorUndoSize*sizeof(EditorUndoData));
  /* IS $ */
  UndoDataPos=0;
  StartLine=StartChar=-1;
  BlockUndo=FALSE;
  VBlockStart=NULL;
  memset(&SavePos,0xff,sizeof(SavePos));
  MaxRightPos=0;
  UndoSavePos=0;
  Editor::EditorID=::EditorID++;
  Flags.Set(FEDITOR_OPENFAILED); // ��, ����. ���� �� ��� �� ������,
                                  // ��� ������ ������� ������� �������� ��������

  HostFileEditor=NULL;
}


Editor::~Editor()
{
  //_SVS(SysLog("[%p] Editor::~Editor()",this));
  FreeAllocatedData();

  KeepInitParameters();

  _KEYMACRO(SysLog(-1));
  _KEYMACRO(SysLog("Editor::~Editor()"));
}

void Editor::FreeAllocatedData()
{
  //_SVS(CleverSysLog Clev("Editor::FreeAllocatedData()"));
  //_SVS(DWORD I=0;SysLog("TopList=%p, EndList=%p _heapchk() = %d",TopList, EndList,_heapchk()));
  //_SVS(TRY)
  {
    while (EndList!=NULL)
    {
      struct EditList *Prev=EndList->Prev;
      delete EndList;
      EndList=Prev;
     //_SVS(I++);
    }
  }
  //_SVS(EXCEPT(EXCEPTION_EXECUTE_HANDLER){SysLog("I=%d EndList=%p{%p,%p} _heapchk() = %d",I,EndList,EndList->Next,EndList->Prev,_heapchk());});

  //_SVS(SysLog("I=%d) _heapchk() = %d",I,_heapchk()));

  /* $ 03.12.2001 IS
     UndoData - ���������
  */
  if(UndoData)
  {
    for (int I=0;I<Opt.EditorUndoSize;++I)
      if (UndoData[I].Type!=UNDO_NONE && UndoData[I].Str!=NULL)
        delete UndoData[I].Str;
    xf_free(UndoData);
    UndoData=NULL;
  }
  /* IS $ */
}

void Editor::KeepInitParameters()
{
  /* $ 26.10.2003 KM
     ! �������������� GlobalSearchString � ������ ����������� 16-������� ������,
       � ����� ���� �� ������ �� ������ ��� ���� ������� ������ �� ����������
       ����� ������ � ���������.
  */
  // ���������� ���������� ����� ������ 16-������ ������?
  if (GlobalSearchHex)
  {
    // ��! ����� ��������, ���������� �� LastSearchStr � ��������� ������������� GlobalSearchString...
    char SearchStr[2*NM];
    int LenSearchStr=sizeof(SearchStr);
    Transform((unsigned char *)SearchStr,LenSearchStr,(char *)GlobalSearchString,'S');

    // LastSearchStr ���������� �� ���������� ������������� GlobalSearchString
    if (memcmp(LastSearchStr,SearchStr,LenSearchStr)!=0)
    {
      // ��! ����������, ������ ������������� ����� �� ���������, �������
      // ������������� ��� �������� � 16-������ �������������.
      int LenSearchStr=sizeof(GlobalSearchString);
      Transform((unsigned char *)GlobalSearchString,LenSearchStr,(char *)LastSearchStr,'X');
    }
  }
  else
    strcpy(GlobalSearchString,(char *)LastSearchStr);
  /* KM $ */

  GlobalSearchCase=LastSearchCase;
  /* $ 03.08.2000 KM
    ����� ���������� ��� ������ "Whole words"
  */
  GlobalSearchWholeWords=LastSearchWholeWords;
  /* KM $ */
  GlobalSearchReverse=LastSearchReverse;
  memcpy(&InitTableSet,&TableSet,sizeof(InitTableSet));
  InitUseDecodeTable=UseDecodeTable;
  InitTableNum=TableNum;
  InitAnsiText=AnsiText;
}


int Editor::ReadFile(const char *Name,int &UserBreak)
{
  FILE *EditFile;
  struct EditList *PrevPtr;
  int Count=0,LastLineCR=0,MessageShown=FALSE;

  UserBreak=0;
  Flags.Clear(FEDITOR_OPENFAILED);

/* Name ��� � ������ �������!!!
  if (ConvertNameToFull(Name,FileName, sizeof(FileName)) >= sizeof(FileName))
  {
    Flags.Set(FEDITOR_OPENFAILED);
    return FALSE;
  }
*/

  DWORD FileFlags=FILE_FLAG_SEQUENTIAL_SCAN;
  if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    FileFlags|=FILE_FLAG_POSIX_SEMANTICS;

  HANDLE hEdit=FAR_CreateFile(Name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FileFlags,NULL);

  if (hEdit==INVALID_HANDLE_VALUE && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    hEdit=FAR_CreateFile(Name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

  if (hEdit==INVALID_HANDLE_VALUE)
  {
    int LastError=GetLastError();
    SetLastError(LastError);
    if (LastError!=ERROR_FILE_NOT_FOUND && LastError!=ERROR_PATH_NOT_FOUND)
    {
      UserBreak=-1;
      Flags.Set(FEDITOR_OPENFAILED);
    }
    return(FALSE);
  }

  int EditHandle=_open_osfhandle((long)hEdit,O_BINARY);
  if (EditHandle==-1)
    return(FALSE);
  if ((EditFile=fdopen(EditHandle,"rb"))==NULL)
    return(FALSE);
  if (GetFileType(hEdit)!=FILE_TYPE_DISK)
  {
    fclose(EditFile);
    SetLastError(ERROR_INVALID_NAME);
    UserBreak=-1;
    Flags.Set(FEDITOR_OPENFAILED);
    return(FALSE);
  }

  /* $ 29.11.2000 SVS
   + �������� �� ���������� ���������� ������ �����, �����
     �������� ����� ����� ������ � ���������������� �������� ���������
     ����� �� ��������������
  */
  if(Opt.EditorFileSizeLimitLo || Opt.EditorFileSizeLimitHi)
  {
    int64 RealSizeFile;
    /* $ 07.01.2001 IS
        - ��� ����� �� ��������� �������� �������� �� ������
    */
    SetLastError(NO_ERROR);
    /* IS $ */
    RealSizeFile.PLow()=GetFileSize(hEdit,(DWORD*)&RealSizeFile.PHigh());
    if (GetLastError() == NO_ERROR)
    {
      int64 NeedSizeFile(Opt.EditorFileSizeLimitHi,Opt.EditorFileSizeLimitLo);
      if(RealSizeFile > NeedSizeFile)
      {
        char TempBuf[2][128];
        char TempBuf2[2][64];
        // ������ = 8 - ��� �����... � Kb � ����...
        FileSizeToStr(TempBuf2[0],RealSizeFile.PHigh(),RealSizeFile.PLow(),8);
        FileSizeToStr(TempBuf2[1],NeedSizeFile.PHigh(),NeedSizeFile.PLow(),8);
        sprintf(TempBuf[0],MSG(MEditFileLong),RemoveExternalSpaces(TempBuf2[0]));
        sprintf(TempBuf[1],MSG(MEditFileLong2),RemoveExternalSpaces(TempBuf2[1]));
        if(Message(MSG_WARNING,2,MSG(MEditTitle),
                    Name,
                    TempBuf[0],
                    TempBuf[1],
                    MSG(MEditROOpen),
                    MSG(MYes),MSG(MNo)))
        {
          fclose(EditFile);
          SetLastError(ERROR_OPEN_FAILED);
          UserBreak=1;
          Flags.Set(FEDITOR_OPENFAILED);
          return(FALSE);
        }
      }
    }
  }
  /* SVS $ */
  /* $ 29.11.2000 SVS
     ���� ���� ����� ������� ReadOnly ��� System ��� Hidden,
     �� ����� ����� ���� - ����������� ����������.
  */
  /* $ 03.12.2000 SVS
     System ��� Hidden - �������� ��������
  */
  /* $ 15.12.2000 SVS
     �������� ������� ��������� ��, ��� ������� GetFileAttributes() :-)
  */
  {
    /* $ 12.02.2001 IS
         �������� ��������
    */
    DWORD FileAttributes=HostFileEditor?HostFileEditor->GetFileAttributes(Name):(DWORD)-1;
    if((Opt.EditorReadOnlyLock&1) &&
       FileAttributes != -1 &&
       (FileAttributes &
          (FILE_ATTRIBUTE_READONLY|
             /* Hidden=0x2 System=0x4 - ������������� �� 2-� ���������,
                ������� ��������� ����� 0110.0000 �
                �������� �� ���� ����� => 0000.0110 � ��������
                �� ����� ������ ��������  */
             ((Opt.EditorReadOnlyLock&0x60)>>4)
          )
       )
     )
    /* IS $ */
      Flags.Swap(FEDITOR_LOCKMODE);
  }
  /* SVS 15.12.2000 $ */
  /* SVS 03.12.2000 $ */
  /* SVS $ */

  {
    ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
    GetFileString GetStr(EditFile);
    //SaveScreen SaveScr;
    NumLastLine=0;
    /* $ 12.01.2002 IS
       ������� ������� ����, ��������� ������� ����� ������ �� ������ ���
       ����������, � ���� ������ ������� ��.
    */
    *GlobalEOL=0;
    /* IS $ */
    char *Str;
    int StrLength,GetCode;

    clock_t StartTime=clock();

    if (EdOpt.AutoDetectTable)
    {
      UseDecodeTable=DetectTable(EditFile,&TableSet,TableNum);
      AnsiText=FALSE;
    }

    while ((GetCode=GetStr.GetString(&Str,StrLength))!=0)
    {
      if (GetCode==-1)
      {
        fclose(EditFile);
        SetPreRedrawFunc(NULL);
        return(FALSE);
      }
      LastLineCR=0;

      if ((++Count & 0xfff)==0 && clock()-StartTime>500)
      {
        if (CheckForEsc())
        {
          UserBreak=1;
          fclose(EditFile);
          SetPreRedrawFunc(NULL);
          return(FALSE);
        }
        if (!MessageShown)
        {
          SetCursorType(FALSE,0);
          SetPreRedrawFunc(Editor::PR_EditorShowMsg);
          EditorShowMsg(MSG(MEditTitle),MSG(MEditReading),Name);
          MessageShown=TRUE;
        }
      }

      char *CurEOL;
      if (!LastLineCR && ((CurEOL=(char *)memchr(Str,'\r',StrLength))!=NULL ||
          (CurEOL=(char *)memchr(Str,'\n',StrLength))!=NULL))
      {
        strncpy(GlobalEOL,CurEOL,sizeof(GlobalEOL)-1);
        GlobalEOL[sizeof(GlobalEOL)-1]=0;
        LastLineCR=1;
      }

      if (NumLastLine!=0)
      {
        EndList->Next=new struct EditList;
        if (EndList->Next==NULL)
        {
          fclose(EditFile);
          SetPreRedrawFunc(NULL);
          return(FALSE);
        }
        PrevPtr=EndList;
        EndList=EndList->Next;
        EndList->Prev=PrevPtr;
        EndList->Next=NULL;
      }

      /* $ 14.02.2001 IS
           ��������� ������ ������ ���������
      */
      EndList->EditLine.SetTabSize(EdOpt.TabSize);
      /* IS $ */
      /* $ 24.06.2002 SKV
        � PersistentBlocks ��� ����� �������������?
      */
      EndList->EditLine.SetPersistentBlocks(EdOpt.PersistentBlocks);
      /* SKV $ */
      EndList->EditLine.SetConvertTabs(EdOpt.ExpandTabs);
      EndList->EditLine.SetBinaryString(Str,StrLength);
      EndList->EditLine.SetCurPos(0);
      EndList->EditLine.SetObjectColor(COL_EDITORTEXT,COL_EDITORSELECTEDTEXT);
      EndList->EditLine.SetEditorMode(TRUE);
      EndList->EditLine.SetWordDiv(EdOpt.WordDiv);
      NumLastLine++;
    }
    SetPreRedrawFunc(NULL);
    if (LastLineCR)
      if ((EndList->Next=new struct EditList)!=NULL)
      {
        PrevPtr=EndList;
        EndList=EndList->Next;
        EndList->Prev=PrevPtr;
        EndList->Next=NULL;
        /* $ 14.02.2001 IS
           ��������� ������ ������ ���������
        */
        EndList->EditLine.SetTabSize(EdOpt.TabSize);
        /* IS $ */
        /* $ 24.06.2002 SKV
          � PersistentBlocks ��� ����� �������������?
        */
        EndList->EditLine.SetPersistentBlocks(EdOpt.PersistentBlocks);
        /* SKV $ */
        EndList->EditLine.SetConvertTabs(EdOpt.ExpandTabs);
        EndList->EditLine.SetString("");
        EndList->EditLine.SetCurPos(0);
        EndList->EditLine.SetObjectColor(COL_EDITORTEXT,COL_EDITORSELECTEDTEXT);
        EndList->EditLine.SetEditorMode(TRUE);
        EndList->EditLine.SetWordDiv(EdOpt.WordDiv);
        NumLastLine++;
      }
  }
  if (NumLine>0)
    NumLastLine--;
  if (NumLastLine==0)
    NumLastLine=1;
  fclose(EditFile);
  if (StartLine==-2)
  {
    struct EditList *CurPtr=TopList;
    long TotalSize=0;
    while (CurPtr!=NULL && CurPtr->Next!=NULL)
    {
      const char *SaveStr,*EndSeq;
      int Length;
      CurPtr->EditLine.GetBinaryString(SaveStr,&EndSeq,Length);
      TotalSize+=Length+strlen(EndSeq);
      if (TotalSize>StartChar)
        break;
      CurPtr=CurPtr->Next;
      NumLine++;
    }
    TopScreen=CurLine=CurPtr;
    if (EdOpt.SavePos && CtrlObject!=NULL)
    {
      /* $ 14.01.2001 tran
         LeftPos ���� ���� ����������������... */
      unsigned int Line,ScreenLine,LinePos,LeftPos=0;
      char CacheName[NM*3];
      if (HostFileEditor && *HostFileEditor->GetPluginData())
        sprintf(CacheName,"%s%s",HostFileEditor->GetPluginData(),PointToName(Name));
      else
      {
        strcpy(CacheName,Name);
        for(int i=0;CacheName[i];i++)
        {
          if(CacheName[i]=='/')CacheName[i]='\\';
        }
      }
      unsigned int Table;
      {
        struct TPosCache32 PosCache={0};
        if(Opt.EdOpt.SaveShortPos)
        {
          PosCache.Position[0]=SavePos.Line;
          PosCache.Position[1]=SavePos.Cursor;
          PosCache.Position[2]=SavePos.ScreenLine;
          PosCache.Position[3]=SavePos.LeftPos;
        }
        CtrlObject->EditorPosCache->GetPosition(CacheName,&PosCache);
        Line=PosCache.Param[0];
        ScreenLine=PosCache.Param[1];
        LinePos=PosCache.Param[2];
        LeftPos=PosCache.Param[3];
        Table=PosCache.Param[4];
      }
      //_D(SysLog("after Get cache, LeftPos=%i",LeftPos));
      if((int)Line < 0) Line=0;
      if((int)ScreenLine < 0) ScreenLine=0;
      if((int)LinePos < 0) LinePos=0;
      if((int)LeftPos < 0) LeftPos=0;
      if((int)Table < 0) Table=0;
      Flags.Change(FEDITOR_TABLECHANGEDBYUSER,(Table!=0));
      switch(Table)
      {
        case 0:
          break;
        case 1:
          AnsiText=UseDecodeTable=0;
          break;
        case 2:
          {
            AnsiText=TRUE;
            UseDecodeTable=TRUE;
            TableNum=0;
            int UseUnicode=FALSE;
            GetTable(&TableSet,TRUE,TableNum,UseUnicode);
          }
          break;
        default:
          AnsiText=0;
          UseDecodeTable=1;
          TableNum=Table-2;
          PrepareTable(&TableSet,Table-3);
          break;
      }
      if (NumLine==Line-ScreenLine)
      {
        DisableOut++;
        for (int I=0;I<ScreenLine;I++)
          ProcessKey(KEY_DOWN);
        CurLine->EditLine.SetTabCurPos(LinePos);
        DisableOut--;
      }
      //_D(SysLog("Setleftpos to %i",LeftPos));
      CurLine->EditLine.SetLeftPos(LeftPos);
    }
  }
  else
    if (StartLine!=-1 || EdOpt.SavePos && CtrlObject!=NULL)
    {
      /* $ 14.01.2001 tran
         LeftPos ���� ���� ����������������... */
      unsigned int Line,ScreenLine,LinePos,LeftPos=0;
      if (StartLine!=-1)
      {
        Line=StartLine-1;
        ScreenLine=ObjHeight/2; //ScrY
        if (ScreenLine>Line)
          ScreenLine=Line;
        LinePos=(StartChar>0) ? StartChar-1:0;
      }
      else
      {
        char CacheName[NM*3];
        if (HostFileEditor && *HostFileEditor->GetPluginData())
          sprintf(CacheName,"%s%s",HostFileEditor->GetPluginData(),PointToName(Name));
        else
        {
          strcpy(CacheName,Name);
          for(int i=0;CacheName[i];i++)
          {
            if(CacheName[i]=='/')CacheName[i]='\\';
          }
        }
        unsigned int Table;
        {
          struct TPosCache32 PosCache={0};
          if(Opt.EdOpt.SaveShortPos)
          {
            PosCache.Position[0]=SavePos.Line;
            PosCache.Position[1]=SavePos.Cursor;
            PosCache.Position[2]=SavePos.ScreenLine;
            PosCache.Position[3]=SavePos.LeftPos;
          }
          CtrlObject->EditorPosCache->GetPosition(CacheName,&PosCache);
          Line=PosCache.Param[0];
          ScreenLine=PosCache.Param[1];
          LinePos=PosCache.Param[2];
          LeftPos=PosCache.Param[3];
          Table=PosCache.Param[4];
        }
        //_D(SysLog("after Get cache 2, LeftPos=%i",LeftPos));
        if((int)Line < 0) Line=0;
        if((int)ScreenLine < 0) ScreenLine=0;
        if((int)LinePos < 0) LinePos=0;
        if((int)LeftPos < 0) LeftPos=0;
        if((int)Table < 0) Table=0;
        Flags.Change(FEDITOR_TABLECHANGEDBYUSER,(Table!=0));
        switch(Table)
        {
          case 0:
            break;
          case 1:
            AnsiText=UseDecodeTable=0;
            break;
          case 2:
            {
              AnsiText=TRUE;
              UseDecodeTable=TRUE;
              TableNum=0;
              int UseUnicode=FALSE;
              GetTable(&TableSet,TRUE,TableNum,UseUnicode);
            }
            break;
          default:
            AnsiText=0;
            UseDecodeTable=1;
            TableNum=Table-2;
            PrepareTable(&TableSet,Table-3);
            break;
        }
      }
      if (ScreenLine>ObjHeight)//ScrY
        ScreenLine=ObjHeight;//ScrY;
      if (Line>=ScreenLine)
      {
        DisableOut++;
        GoToLine(Line-ScreenLine);
        TopScreen=CurLine;
        for (int I=0;I<ScreenLine;I++)
          ProcessKey(KEY_DOWN);
        CurLine->EditLine.SetTabCurPos(LinePos);
        //_D(SysLog("Setleftpos 2 to %i",LeftPos));
        CurLine->EditLine.SetLeftPos(LeftPos);
        DisableOut--;
      }
    }
  if (UseDecodeTable)
    for (struct EditList *CurPtr=TopList;CurPtr!=NULL;CurPtr=CurPtr->Next)
      CurPtr->EditLine.SetTables(&TableSet);
  /* $ 25.07.2001 IS
       ������������� ������� ����� ������� ��������, �.�. ������� ������
       �������� �� ������������ (UseDecodeTable==FALSE)
  */
  else
    TableNum=0;
  /* IS $ */

  CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
//_D(SysLog("%08d EE_READ",__LINE__));
  CtrlObject->Plugins.ProcessEditorEvent(EE_READ,NULL);
  //_SVS(SysLog("Editor::ReadFile _heapchk() = %d",_heapchk()));
  return(TRUE);
}

// �������������� �� ������ � ������
int Editor::ReadData(LPCSTR SrcBuf,int SizeSrcBuf)
{
#if defined(PROJECT_DI_MEMOEDIT)
  struct EditList *PrevPtr;
  int Count=0,LastLineCR=0;

  UserBreak=0;

  {
    GetFileString GetStr(EditFile);
    NumLastLine=0;
    *GlobalEOL=0;
    char *Str;
    int StrLength,GetCode;

    if (EdOpt.AutoDetectTable)
    {
      UseDecodeTable=DetectTable(EditFile,&TableSet,TableNum);
      AnsiText=FALSE;
    }

    while ((GetCode=GetStr.GetString(&Str,StrLength))!=0)
    {
      if (GetCode==-1)
      {
        return(FALSE);
      }
      LastLineCR=0;

      char *CurEOL;
      if (!LastLineCR && ((CurEOL=(char *)memchr(Str,'\r',StrLength))!=NULL ||
          (CurEOL=(char *)memchr(Str,'\n',StrLength))!=NULL))
      {
        strncpy(GlobalEOL,CurEOL,sizeof(GlobalEOL)-1);
        GlobalEOL[sizeof(GlobalEOL)-1]=0;
        LastLineCR=1;
      }

      if (NumLastLine!=0)
      {
        EndList->Next=new struct EditList;
        if (EndList->Next==NULL)
        {
          return(FALSE);
        }
        PrevPtr=EndList;
        EndList=EndList->Next;
        EndList->Prev=PrevPtr;
        EndList->Next=NULL;
      }

      EndList->EditLine.SetTabSize(EdOpt.TabSize);
      EndList->EditLine.SetPersistentBlocks(EdOpt.PersistentBlocks);
      EndList->EditLine.SetConvertTabs(EdOpt.ExpandTabs);
      EndList->EditLine.SetBinaryString(Str,StrLength);
      EndList->EditLine.SetCurPos(0);
      EndList->EditLine.SetObjectColor(COL_EDITORTEXT,COL_EDITORSELECTEDTEXT);
      EndList->EditLine.SetEditorMode(TRUE);
      EndList->EditLine.SetWordDiv(EdOpt.WordDiv);

      NumLastLine++;
    }

    if (LastLineCR && ((EndList->Next=new struct EditList)!=NULL))
    {
      PrevPtr=EndList;
      EndList=EndList->Next;
      EndList->Prev=PrevPtr;
      EndList->Next=NULL;
      EndList->EditLine.SetTabSize(EdOpt.TabSize);
      EndList->EditLine.SetPersistentBlocks(EdOpt.PersistentBlocks);
      EndList->EditLine.SetConvertTabs(EdOpt.ExpandTabs);
      EndList->EditLine.SetString("");
      EndList->EditLine.SetCurPos(0);
      EndList->EditLine.SetObjectColor(COL_EDITORTEXT,COL_EDITORSELECTEDTEXT);
      EndList->EditLine.SetEditorMode(TRUE);
      EndList->EditLine.SetWordDiv(EdOpt.WordDiv);
      NumLastLine++;
    }
  }

  if (NumLine>0)
    NumLastLine--;

  if (NumLastLine==0)
    NumLastLine=1;

  if (UseDecodeTable)
    for (struct EditList *CurPtr=TopList;CurPtr!=NULL;CurPtr=CurPtr->Next)
      CurPtr->EditLine.SetTables(&TableSet);
  else
    TableNum=0;

#endif
  return(TRUE);
}

/*
  Editor::SaveData - �������������� �� ������ � �����

    DestBuf     - ���� ��������� (���������� �����������!)
    SizeDestBuf - ������ ����������
    TextFormat  - ��� �������� �����
*/
int Editor::SaveData(char **DestBuf,int& SizeDestBuf,int TextFormat)
{
#if defined(PROJECT_DI_MEMOEDIT)
  char *PDest=NULL;
  SizeDestBuf=0; // ����� ������ = 0

  // ���������� EOL
  switch(TextFormat)
  {
    case 1:
      strcpy(GlobalEOL,DOS_EOL_fmt);
      break;
    case 2:
      strcpy(GlobalEOL,UNIX_EOL_fmt);
      break;
    case 3:
      strcpy(GlobalEOL,MAC_EOL_fmt);
      break;
  }

  int StrLength=0;
  const char *SaveStr, *EndSeq;
  int Length;

  // ��������� ���������� ����� � ����� ������ ������ (����� �� ������� realloc)
  struct EditList *CurPtr=TopList;

  DWORD AllLength=0;
  while (CurPtr!=NULL)
  {
    CurPtr->EditLine.GetBinaryString(SaveStr,&EndSeq,Length);
    // ���������� �������� �����
    if (*EndSeq==0 && CurPtr->Next!=NULL)
      EndSeq=*GlobalEOL ? GlobalEOL:DOS_EOL_fmt;

    if (TextFormat!=0 && *EndSeq!=0)
    {
      if (TextFormat==1)
        EndSeq=DOS_EOL_fmt;
      else if (TextFormat==2)
        EndSeq=UNIX_EOL_fmt;
      else
        EndSeq=MAC_EOL_fmt;

      CurPtr->EditLine.SetEOL(EndSeq);
    }
    AllLength+=Length+strlen(EndSeq)+16;
  }

  char *MemEditStr=(char *)malloc(sizeof(char) * AllLength);

  if(MemEditStr)
  {
    *MemEditStr=0;
    PDest=MemEditStr;
    // �������� �� ������ �����
    CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->EditLine.GetBinaryString(SaveStr,&EndSeq,Length);

      strcpy(PDest,SaveStr);
      strcat(PDest,EndSeq);
      PDest+=strlen(PDest);

      CurPtr=CurPtr->Next;
    }
    SizeDestBuf=strlen(MemEditStr);
    DestBuf=&MemEditStr;
    return TRUE;
  }
  else
    return FALSE;
#else
  return TRUE;
#endif
}


void Editor::DisplayObject()
{
  if (!DisableOut)
  {
    ShowEditor(FALSE);
  }
}


void Editor::ShowEditor(int CurLineOnly)
{
  if (DisableOut)
    return;

  struct EditList *CurPtr;
  int LeftPos,CurPos,Y;

//_SVS(SysLog("Enter to ShowEditor, CurLineOnly=%i",CurLineOnly));
  /*$ 10.08.2000 skv
    To make sure that CurEditor is set to required value.
  */
  CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
  /* skv$*/

  /* 17.04.2002 skv
    ��� � ������ �� ����� ��� Alt-F9 � ����� �������� �����.
    ���� �� ������ ���� ��������� �����, � ���� ����� ������,
    �����������������.
  */

  while(CalcDistance(TopScreen,NULL,Y2-Y1-1)<Y2-Y1-1)
  {
    if(TopScreen->Prev)
    {
      TopScreen=TopScreen->Prev;
    }
    else
    {
      break;
    }
  }
  /*
    ���� ������ ����� �������� "�� �������",
    �������� ����� ��� ������, � ��
    ������ ������� � �����.
  */

  while (CalcDistance(TopScreen,CurLine,-1)>=Y2-Y1)
  {
    TopScreen=TopScreen->Next;
    //DisableOut=TRUE;
    //ProcessKey(KEY_UP);
    //DisableOut=FALSE;
  }


  /* skv $ */

  CurPos=CurLine->EditLine.GetTabCurPos();
  if (!EdOpt.CursorBeyondEOL)
  {
    MaxRightPos=CurPos;
    int RealCurPos=CurLine->EditLine.GetCurPos();
    int Length=CurLine->EditLine.GetLength();

    if (RealCurPos>Length)
    {
      CurLine->EditLine.SetCurPos(Length);
      CurLine->EditLine.SetLeftPos(0);
      //_D(SysLog("call CurLine->EditLine.FastShow()"));
      CurLine->EditLine.FastShow();
      CurPos=CurLine->EditLine.GetTabCurPos();
    }
  }

  if (!Pasting)
  {
    /*$ 10.08.2000 skv
      Don't send EE_REDRAW while macro is being executed.
      Send EE_REDRAW with param=2 if text was just modified.

    */
    _SYS_EE_REDRAW(CleverSysLog Clev("Editor::ShowEditor()"));
    if(!ScrBuf.GetLockCount())
    {
      /*$ 13.09.2000 skv
        EE_REDRAW 1 and 2 replaced.
      */
      if(Flags.Check(FEDITOR_JUSTMODIFIED))
      {
        Flags.Clear(FEDITOR_JUSTMODIFIED);
        _SYS_EE_REDRAW(SysLog("Call ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE)"));
        CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE);
      }
      else
      {
        _SYS_EE_REDRAW(SysLog("Call ProcessEditorEvent(EE_REDRAW,%s)",(CurLineOnly?"EEREDRAW_LINE":"EEREDRAW_ALL")));
        CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,CurLineOnly?EEREDRAW_LINE:EEREDRAW_ALL);
      }
      /* skv$*/
    }
    _SYS_EE_REDRAW(else SysLog("ScrBuf Locked !!!"));
    /* skv$*/
  }

  if (!CurLineOnly)
  {
    LeftPos=CurLine->EditLine.GetLeftPos();

    for (CurPtr=TopScreen,Y=Y1+1;Y<=Y2;Y++)
      if (CurPtr!=NULL)
      {
        CurPtr->EditLine.SetEditBeyondEnd(TRUE);
        CurPtr->EditLine.SetPosition(X1,Y,X2,Y);
        CurPtr->EditLine.SetTables(UseDecodeTable ? &TableSet:NULL);
        //_D(SysLog("Setleftpos 3 to %i",LeftPos));
        CurPtr->EditLine.SetLeftPos(LeftPos);
        CurPtr->EditLine.SetTabCurPos(CurPos);
        CurPtr->EditLine.FastShow();
        CurPtr->EditLine.SetEditBeyondEnd(EdOpt.CursorBeyondEOL);
        CurPtr=CurPtr->Next;
      }
      else
      {
        //GotoXY(X1,Y);
        //SetColor(COL_EDITORTEXT);
        //mprintf("%*s",ObjWidth,"");
        SetScreen(X1,Y,X2,Y,' ',COL_EDITORTEXT); //??
      }
  }

  CurLine->EditLine.SetOvertypeMode(Flags.Check(FEDITOR_OVERTYPE));
  CurLine->EditLine.Show();

  if (VBlockStart!=NULL && VBlockSizeX>0 && VBlockSizeY>0)
  {
    int CurScreenLine=NumLine-CalcDistance(TopScreen,CurLine,-1);
    LeftPos=CurLine->EditLine.GetLeftPos();
    for (CurPtr=TopScreen,Y=Y1+1;Y<=Y2;Y++)
      if (CurPtr!=NULL)
      {
        if (CurScreenLine>=VBlockY && CurScreenLine<VBlockY+VBlockSizeY)
        {
          int BlockX1=VBlockX-LeftPos;
          int BlockX2=VBlockX+VBlockSizeX-1-LeftPos;
          if (BlockX1<X1)
            BlockX1=X1;
          if (BlockX2>X2)
            BlockX2=X2;
          if (BlockX1<=X2 && BlockX2>=X1)
            ChangeBlockColor(BlockX1,Y,BlockX2,Y,COL_EDITORSELECTEDTEXT);
        }
        CurPtr=CurPtr->Next;
        CurScreenLine++;
      }
    }

  if(HostFileEditor) HostFileEditor->ShowStatus();
//_SVS(SysLog("Exit from ShowEditor"));
}


/*$ 10.08.2000 skv
  Wrapper for Modified.
  Set JustModified every call to 1
  to track any text state change.
  Even if state==0, this can be
  last UNDO.
*/
void Editor::TextChanged(int State)
{
  Flags.Change(FEDITOR_MODIFIED,State);
  Flags.Set(FEDITOR_JUSTMODIFIED);
}
/* skv$*/

int Editor::ProcessKey(int Key)
{
  if (Key==KEY_IDLE)
  {
    if (Opt.ViewerEditorClock && HostFileEditor!=NULL && HostFileEditor->IsFullScreen())
      ShowTime(FALSE);
    return(TRUE);
  }

  if (Key==KEY_NONE)
    return(TRUE);

  _KEYMACRO(CleverSysLog SL("Editor::ProcessKey()"));
  _KEYMACRO(SysLog("Key=%s",_FARKEY_ToName(Key)));

  int CurPos,CurVisPos,I;
  CurPos=CurLine->EditLine.GetCurPos();
  CurVisPos=GetLineCurPos();

  int isk=IsShiftKey(Key);
  _SVS(SysLog("[%d] isk=%d",__LINE__,isk));
  //if ((!isk || CtrlObject->Macro.IsExecuting()) && !isk && !Pasting)
//  if (!isk && !Pasting && !(Key >= KEY_MACRO_BASE && Key <= KEY_MACRO_ENDBASE))
  if (!isk && !Pasting && (Key < KEY_MACRO_BASE || Key > KEY_MACRO_ENDBASE))
  {
    _SVS(SysLog("[%d] BlockStart=(%d,%d)",__LINE__,BlockStart,VBlockStart));
    if (BlockStart!=NULL || VBlockStart!=NULL)
    {
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
    }
    if ((BlockStart!=NULL || VBlockStart!=NULL) && !EdOpt.PersistentBlocks)
//    if (BlockStart!=NULL || VBlockStart!=NULL && !EdOpt.PersistentBlocks)
    {
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      if (!EdOpt.PersistentBlocks)
      {
        static int UnmarkKeys[]={
           KEY_LEFT,      KEY_NUMPAD4,
           KEY_RIGHT,     KEY_NUMPAD6,
           KEY_HOME,      KEY_NUMPAD7,
           KEY_END,       KEY_NUMPAD1,
           KEY_UP,        KEY_NUMPAD8,
           KEY_DOWN,      KEY_NUMPAD2,
           KEY_PGUP,      KEY_NUMPAD9,
           KEY_PGDN,      KEY_NUMPAD3,
           KEY_CTRLHOME,  KEY_CTRLNUMPAD7,
           KEY_CTRLPGUP,  KEY_CTRLNUMPAD9,
           KEY_CTRLEND,   KEY_CTRLNUMPAD1,
           KEY_CTRLPGDN,  KEY_CTRLNUMPAD3,
           KEY_CTRLLEFT,  KEY_CTRLNUMPAD4,
           KEY_CTRLRIGHT, KEY_CTRLNUMPAD7,
           KEY_CTRLUP,    KEY_CTRLNUMPAD8,
           KEY_CTRLDOWN,  KEY_CTRLNUMPAD2,
           KEY_CTRLN,
           KEY_CTRLE,
           KEY_CTRLS
        };
        for (int I=0;I<sizeof(UnmarkKeys)/sizeof(UnmarkKeys[0]);I++)
          if (Key==UnmarkKeys[I])
          {
            UnmarkBlock();
            break;
          }
      }
      else
      {
        int StartSel,EndSel;
//        struct EditList *BStart=!BlockStart?VBlockStart:BlockStart;
//        BStart->EditLine.GetRealSelection(StartSel,EndSel);
        BlockStart->EditLine.GetRealSelection(StartSel,EndSel);
        _SVS(SysLog("[%d] PersistentBlocks! StartSel=%d, EndSel=%d",__LINE__,StartSel,EndSel));
        if (StartSel==-1 || StartSel==EndSel)
          UnmarkBlock();
      }
    }
  }

  switch(Key)
  {
    case MCODE_C_EMPTY:
      return !CurLine->Next && !CurLine->Prev; //??
    case MCODE_C_EOF:
      return !CurLine->Next && CurPos>=CurLine->EditLine.GetLength();
    case MCODE_C_BOF:
      return !CurLine->Prev && CurPos==0;
    case MCODE_C_SELECTED:
      return BlockStart || VBlockStart?TRUE:FALSE;
  }

  if (Key==KEY_ALTD)
    Key=KEY_CTRLK;

  // ������ � ����������
  if (Key>=KEY_CTRL0 && Key<=KEY_CTRL9)
    return GotoBookmark(Key-KEY_CTRL0);
  if (Key>=KEY_CTRLSHIFT0 && Key<=KEY_CTRLSHIFT9)
    Key=Key-KEY_CTRLSHIFT0+KEY_RCTRL0;
  if (Key>=KEY_RCTRL0 && Key<=KEY_RCTRL9)
    return SetBookmark(Key-KEY_RCTRL0);

  int SelStart,SelEnd;
  int SelFirst=FALSE;
  int SelAtBeginning=FALSE;

  /* $ 05.11.2003 SKV

  */
  EditorBlockGuard _bg(*this,&Editor::UnmarkEmptyBlock);
  /* SKV $ */

  switch(Key)
  {
    case KEY_SHIFTLEFT:    case KEY_SHIFTRIGHT:
    case KEY_SHIFTUP:      case KEY_SHIFTDOWN:
    case KEY_SHIFTHOME:    case KEY_SHIFTEND:
    case KEY_SHIFTNUMPAD4: case KEY_SHIFTNUMPAD6:
    case KEY_SHIFTNUMPAD8: case KEY_SHIFTNUMPAD2:
    case KEY_SHIFTNUMPAD7: case KEY_SHIFTNUMPAD1:
    case KEY_CTRLSHIFTLEFT:  case KEY_CTRLSHIFTNUMPAD4:   /* 12.11.2002 DJ */
    {
      _KEYMACRO(CleverSysLog SL("Editor::ProcessKey(KEY_SHIFT*)"));
      _SVS(SysLog("[%d] SelStart=%d, SelEnd=%d",__LINE__,SelStart,SelEnd));
      UnmarkEmptyBlock(); // ������ ���������, ���� ��� ������ ����� 0
      _bg.needCheckUnmark=true;
      CurLine->EditLine.GetRealSelection(SelStart,SelEnd);
      if(Flags.Check(FEDITOR_CURPOSCHANGEDBYPLUGIN))
      {
        if(SelStart!=-1 && (CurPos<SelStart || // ���� ������ �� ���������
           (SelEnd!=-1 && (CurPos>SelEnd ||    // ... ����� ���������
            (CurPos>SelStart && CurPos<SelEnd)))) &&
           CurPos<CurLine->EditLine.GetLength()) // ... ������ ��������
          Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
        Flags.Clear(FEDITOR_CURPOSCHANGEDBYPLUGIN);
      }

      _SVS(SysLog("[%d] SelStart=%d, SelEnd=%d",__LINE__,SelStart,SelEnd));
      if (!Flags.Check(FEDITOR_MARKINGBLOCK))
      /* IS $ */
      {
        UnmarkBlock();
        Flags.Set(FEDITOR_MARKINGBLOCK);
        BlockStart=CurLine;
        BlockStartLine=NumLine;
        SelFirst=TRUE;
        SelStart=SelEnd=CurPos;
      }
      else
      {
        SelAtBeginning=CurLine==BlockStart && CurPos==SelStart;
        if(SelStart==-1)
        {
          SelStart=SelEnd=CurPos;
        }
      }
      _SVS(SysLog("[%d] SelStart=%d, SelEnd=%d",__LINE__,SelStart,SelEnd));
    }
  }

  switch(Key)
  {
    case KEY_CTRLSHIFTPGUP:   case KEY_CTRLSHIFTNUMPAD9:
    case KEY_CTRLSHIFTHOME:   case KEY_CTRLSHIFTNUMPAD7:
    {
      DisableOut++;
      Pasting++;
      while (CurLine!=TopList)
      {
        Edit::DisableEditOut(TRUE);
        ProcessKey(KEY_SHIFTPGUP);
      }
      ProcessKey(KEY_SHIFTHOME);
      Pasting--;
      DisableOut--;
      Edit::DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_CTRLSHIFTPGDN:   case KEY_CTRLSHIFTNUMPAD3:
    case KEY_CTRLSHIFTEND:    case KEY_CTRLSHIFTNUMPAD1:
    {
      DisableOut++;
      Pasting++;
      while (CurLine!=EndList)
      {
        Edit::DisableEditOut(TRUE);
        ProcessKey(KEY_SHIFTPGDN);
      }
      /* $ 06.02.2002 IS
         ������������� ������� ���� ����, ��� ������� �������� ��������.
         ��� ����:
           ��� ���������� "ProcessKey(KEY_SHIFTPGDN)" (��. ���� ����)
           ������� ������� (� ���� ������ - �������) ����� �������
           ECTL_SETPOSITION, � ���������� ���� ������������ ����
           FEDITOR_CURPOSCHANGEDBYPLUGIN. � ��� ��������� KEY_SHIFTEND
           ��������� � �������� ������ ���������� � ����, ��� ������ �� ���
           ���������� ���������� KEY_SHIFTPGDN.
      */
      Flags.Clear(FEDITOR_CURPOSCHANGEDBYPLUGIN);
      /* IS $ */
      ProcessKey(KEY_SHIFTEND);
      Pasting--;
      DisableOut--;
      Edit::DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_SHIFTPGUP:       case KEY_SHIFTNUMPAD9:
    {
      Pasting++;
      DisableOut++;
      Edit::DisableEditOut(TRUE);
      for (I=Y1+1;I<Y2;I++)
      {
        ProcessKey(KEY_SHIFTUP);
        if(!EdOpt.CursorBeyondEOL)
        {
          if(CurLine->EditLine.GetCurPos()>CurLine->EditLine.GetLength())
          {
            CurLine->EditLine.SetCurPos(CurLine->EditLine.GetLength());
          }
        }
      }
      Pasting--;
      DisableOut--;
      Edit::DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_SHIFTPGDN:       case KEY_SHIFTNUMPAD3:
    {
      Pasting++;
      DisableOut++;
      Edit::DisableEditOut(TRUE);
      for (I=Y1+1;I<Y2;I++)
      {
        ProcessKey(KEY_SHIFTDOWN);
        if(!EdOpt.CursorBeyondEOL)
        {
          if(CurLine->EditLine.GetCurPos()>CurLine->EditLine.GetLength())
          {
            CurLine->EditLine.SetCurPos(CurLine->EditLine.GetLength());
          }
        }
      }
      Pasting--;
      DisableOut--;
      Edit::DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_SHIFTHOME:       case KEY_SHIFTNUMPAD7:
    {
      Pasting++;
      DisableOut++;
      Edit::DisableEditOut(TRUE);
      if(SelAtBeginning)
      {
        CurLine->EditLine.Select(0,SelEnd);
      }else
      {
        if(SelStart==0)
        {
          CurLine->EditLine.Select(-1,0);
        }else
        {
          CurLine->EditLine.Select(0,SelStart);
        }
      }
      ProcessKey(KEY_HOME);
      Pasting--;
      DisableOut--;
      Edit::DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_SHIFTEND:    case KEY_SHIFTNUMPAD1:
    {
      {
        int LeftPos=CurLine->EditLine.GetLeftPos();
        Pasting++;
        DisableOut++;
        Edit::DisableEditOut(TRUE);
        int CurLength=CurLine->EditLine.GetLength();

        if(!SelAtBeginning || SelFirst)
        {
          CurLine->EditLine.Select(SelStart,CurLength);
        }else
        {
          if(SelEnd!=-1)
            CurLine->EditLine.Select(SelEnd,CurLength);
          else
            CurLine->EditLine.Select(CurLength,-1);
        }

        /* $ 17.01.2002 SKV
          ��� ��� �� ��� FastShow LeftPos �� ���������� � ����� ������.
        */
        CurLine->EditLine.ObjWidth=X2-X1;
        /* SKV$*/

        ProcessKey(KEY_END);

        Pasting--;
        DisableOut--;
        Edit::DisableEditOut(FALSE);

        /* $ 13.9.2001 SKV
          ������ LeftPos ���������� ������ � FastShow :-\
        */
        if(EdOpt.PersistentBlocks)
          Show();
        else
        {
          CurLine->EditLine.FastShow();
          ShowEditor(LeftPos==CurLine->EditLine.GetLeftPos());
        }
        /* SKV$*/
      }
      return(TRUE);
    }

    case KEY_SHIFTLEFT:  case KEY_SHIFTNUMPAD4:
    {
      _SVS(CleverSysLog SL("case KEY_SHIFTLEFT"));
      if (CurPos==0 && CurLine->Prev==NULL)return TRUE;
      if (CurPos==0) //������ � ������ ������
      {
        if(SelAtBeginning) //������ � ������ �����
        {
          BlockStart=CurLine->Prev;
          CurLine->Prev->EditLine.Select(CurLine->Prev->EditLine.GetLength(),-1);
        }
        else // ������ � ����� �����
        {
          CurLine->EditLine.Select(-1,0);
          CurLine->Prev->EditLine.GetRealSelection(SelStart,SelEnd);
          CurLine->Prev->EditLine.Select(SelStart,CurLine->Prev->EditLine.GetLength());
        }
      }
      else
      {
        if(SelAtBeginning || SelFirst)
        {
          CurLine->EditLine.Select(SelStart-1,SelEnd);
        }
        else
        {
          CurLine->EditLine.Select(SelStart,SelEnd-1);
        }
      }
      int LeftPos=CurLine->EditLine.GetLeftPos();
      EditList *OldCur=CurLine;
      Pasting++;
      ProcessKey(KEY_LEFT);
      Pasting--;

      ShowEditor(OldCur==CurLine && LeftPos==CurLine->EditLine.GetLeftPos());
      return(TRUE);
    }

    case KEY_SHIFTRIGHT:  case KEY_SHIFTNUMPAD6:
    {
      _SVS(CleverSysLog SL("case KEY_SHIFTRIGHT"));
      if(CurLine->Next==NULL && CurPos==CurLine->EditLine.GetLength() && !EdOpt.CursorBeyondEOL)
      {
        return TRUE;
      }

      if(SelAtBeginning)
      {
        CurLine->EditLine.Select(SelStart+1,SelEnd);
      }
      else
      {
        CurLine->EditLine.Select(SelStart,SelEnd+1);
      }
      EditList *OldCur=CurLine;
      int OldLeft=CurLine->EditLine.GetLeftPos();
      Pasting++;
      ProcessKey(KEY_RIGHT);
      Pasting--;
      if(OldCur!=CurLine)
      {
        if(SelAtBeginning)
        {
          OldCur->EditLine.Select(-1,0);
          BlockStart=CurLine;
          BlockStartLine=NumLine;
        }
        else
        {
          OldCur->EditLine.Select(SelStart,-1);
        }
      }
      ShowEditor(OldCur==CurLine && OldLeft==CurLine->EditLine.GetLeftPos());
      return(TRUE);
    }

    case KEY_CTRLSHIFTLEFT:  case KEY_CTRLSHIFTNUMPAD4:
    {
      _SVS(CleverSysLog SL("case KEY_CTRLSHIFTLEFT"));
      _SVS(SysLog("[%d] Pasting=%d, SelEnd=%d",__LINE__,Pasting,DisableOut));
      {
        int SkipSpace=TRUE;
        Pasting++;
        /* $ 23.12.2000 OT */
        DisableOut++;
        Edit::DisableEditOut(TRUE);
        /* OT $ */

        int CurPos;
        while (1)
        {
          const char *Str;
          int Length;
          CurLine->EditLine.GetBinaryString(Str,NULL,Length);
          /* $ 12.11.2002 DJ
             ��������� ���������� ������ Ctrl-Shift-Left �� ������ ������
          */
          CurPos=CurLine->EditLine.GetCurPos();
          if (CurPos>Length)
          {
            int SelStartPos = CurPos;
            CurLine->EditLine.ProcessKey(KEY_END);
            CurPos=CurLine->EditLine.GetCurPos();
            if (CurLine->EditLine.SelStart >= 0)
            {
              if (!SelAtBeginning)
                CurLine->EditLine.Select(CurLine->EditLine.SelStart, CurPos);
              else
                CurLine->EditLine.Select(CurPos, CurLine->EditLine.SelEnd);
            }
            else
              CurLine->EditLine.Select(CurPos, SelStartPos);
          }
          /* DJ $ */
          if (CurPos==0)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos-1]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos-1]))
          /* IS $ */
          /* SVS $ */
            if (SkipSpace)
            {
              ProcessKey(KEY_SHIFTLEFT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_SHIFTLEFT);
        }
        Pasting--;
        /* $ 23.12.2000 OT */
        DisableOut--;
        Edit::DisableEditOut(FALSE);
        Show();
        /* OT $ */
      }
      return(TRUE);
    }

    case KEY_CTRLSHIFTRIGHT:  case KEY_CTRLSHIFTNUMPAD6:
    {
      _SVS(CleverSysLog SL("case KEY_CTRLSHIFTRIGHT"));
      _SVS(SysLog("[%d] Pasting=%d, SelEnd=%d",__LINE__,Pasting,DisableOut));
      {
        int SkipSpace=TRUE;
        Pasting++;
        DisableOut++;
        Edit::DisableEditOut(TRUE);

        int CurPos;
        while (1)
        {
          const char *Str;
          int Length;
          CurLine->EditLine.GetBinaryString(Str,NULL,Length);
          CurPos=CurLine->EditLine.GetCurPos();
          if (CurPos>=Length)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos]))
          /* IS $ */
          /* SVS $ */
            if (SkipSpace)
            {
              ProcessKey(KEY_SHIFTRIGHT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_SHIFTRIGHT);
        }
        Pasting--;
        DisableOut--;
        Edit::DisableEditOut(FALSE);
        Show();
      }
      return(TRUE);
    }

    case KEY_SHIFTDOWN:  case KEY_SHIFTNUMPAD2:
    {
      if (CurLine->Next==NULL)return TRUE;
      CurPos=CurLine->EditLine.RealPosToTab(CurPos);
      if(SelAtBeginning)//������� ���������
      {
        if(SelEnd==-1)
        {
          CurLine->EditLine.Select(-1,0);
          BlockStart=CurLine->Next;
          BlockStartLine=NumLine+1;
        }else
        {
          CurLine->EditLine.Select(SelEnd,-1);
        }
        CurLine->Next->EditLine.GetRealSelection(SelStart,SelEnd);
        if(SelStart!=-1)SelStart=CurLine->Next->EditLine.RealPosToTab(SelStart);
        if(SelEnd!=-1)SelEnd=CurLine->Next->EditLine.RealPosToTab(SelEnd);
        if(SelStart==-1)
        {
          SelStart=0;
          SelEnd=CurPos;
        }else
        {
          if(SelEnd!=-1 && SelEnd<CurPos)
          {
            SelStart=SelEnd;
            SelEnd=CurPos;
          }else
          {
            SelStart=CurPos;
          }
        }
        if(SelStart!=-1)SelStart=CurLine->Next->EditLine.TabPosToReal(SelStart);
        if(SelEnd!=-1)SelEnd=CurLine->Next->EditLine.TabPosToReal(SelEnd);
        /*if(!EdOpt.CursorBeyondEOL && SelEnd>CurLine->Next->EditLine.GetLength())
        {
          SelEnd=CurLine->Next->EditLine.GetLength();
        }
        if(!EdOpt.CursorBeyondEOL && SelStart>CurLine->Next->EditLine.GetLength())
        {
          SelStart=CurLine->Next->EditLine.GetLength();
        }*/
      }else //��������� ���������
      {
        CurLine->EditLine.Select(SelStart,-1);
        SelStart=0;
        SelEnd=CurPos;
        if(SelStart!=-1)SelStart=CurLine->Next->EditLine.TabPosToReal(SelStart);
        if(SelEnd!=-1)SelEnd=CurLine->Next->EditLine.TabPosToReal(SelEnd);
      }
      if(!EdOpt.CursorBeyondEOL && SelEnd>CurLine->Next->EditLine.GetLength())
      {
        SelEnd=CurLine->Next->EditLine.GetLength();
      }
      if(!EdOpt.CursorBeyondEOL && SelStart>CurLine->Next->EditLine.GetLength())
      {
        SelStart=CurLine->Next->EditLine.GetLength();
      }
      CurLine->Next->EditLine.Select(SelStart,SelEnd);
      Down();
      Show();
      return(TRUE);
    }

    case KEY_SHIFTUP: case KEY_SHIFTNUMPAD8:
    {
      if (CurLine->Prev==NULL)return NULL;
      if(SelAtBeginning || SelFirst) // ��������� ���������
      {
        CurLine->EditLine.Select(0,SelEnd);
        SelStart=CurLine->EditLine.RealPosToTab(CurPos);
        if(!EdOpt.CursorBeyondEOL &&
            CurLine->Prev->EditLine.TabPosToReal(SelStart)>CurLine->Prev->EditLine.GetLength())
        {
          SelStart=CurLine->Prev->EditLine.RealPosToTab(CurLine->Prev->EditLine.GetLength());
        }
        SelStart=CurLine->Prev->EditLine.TabPosToReal(SelStart);
        CurLine->Prev->EditLine.Select(SelStart,-1);
        BlockStart=CurLine->Prev;
        BlockStartLine=NumLine-1;
      }else // ������� ���������
      {
        CurPos=CurLine->EditLine.RealPosToTab(CurPos);
        if(SelStart==0)
        {
          CurLine->EditLine.Select(-1,0);
        }else
        {
          CurLine->EditLine.Select(0,SelStart);
        }
        CurLine->Prev->EditLine.GetRealSelection(SelStart,SelEnd);
        if(SelStart!=-1)SelStart=CurLine->Prev->EditLine.RealPosToTab(SelStart);
        if(SelStart!=-1)SelEnd=CurLine->Prev->EditLine.RealPosToTab(SelEnd);
        if(SelStart==-1)
        {
          BlockStart=CurLine->Prev;
          BlockStartLine=NumLine-1;
          SelStart=CurLine->Prev->EditLine.TabPosToReal(CurPos);
          SelEnd=-1;
        }else
        {
          if(CurPos<SelStart)
          {
            SelEnd=SelStart;
            SelStart=CurPos;
          }else
          {
            SelEnd=CurPos;
          }
          SelStart=CurLine->Prev->EditLine.TabPosToReal(SelStart);
          SelEnd=CurLine->Prev->EditLine.TabPosToReal(SelEnd);
          if(!EdOpt.CursorBeyondEOL && SelEnd>CurLine->Prev->EditLine.GetLength())
          {
            SelEnd=CurLine->Prev->EditLine.GetLength();
          }
          if(!EdOpt.CursorBeyondEOL && SelStart>CurLine->Prev->EditLine.GetLength())
          {
            SelStart=CurLine->Prev->EditLine.GetLength();
          }
        }
        CurLine->Prev->EditLine.Select(SelStart,SelEnd);
      }
      Up();
      Show();
      return(TRUE);
    }

    case KEY_CTRLADD:
    {
      Copy(TRUE);
      return(TRUE);
    }

    case KEY_CTRLA:
    {
      UnmarkBlock();
      SelectAll();
      return(TRUE);
    }

    case KEY_CTRLU:
    {
      UnmarkBlock();
      return(TRUE);
    }

    case KEY_CTRLC:
    case KEY_CTRLINS:    case KEY_CTRLNUMPAD0:
    {
      if (/*!EdOpt.PersistentBlocks && */BlockStart==NULL && VBlockStart==NULL)
      {
        BlockStart=CurLine;
        BlockStartLine=NumLine;
        CurLine->EditLine.AddSelect(0,-1);
        Show();
      }
      Copy(FALSE);
      return(TRUE);
    }

    case KEY_CTRLP:
    case KEY_CTRLM:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
        return TRUE;
      if (BlockStart!=NULL || VBlockStart!=NULL)
      {
        int SelStart,SelEnd;
        CurLine->EditLine.GetSelection(SelStart,SelEnd);

        Pasting++;
        int OldUsedInternalClipboard=UsedInternalClipboard;
        UsedInternalClipboard=1;
        ProcessKey(Key==KEY_CTRLP ? KEY_CTRLINS:KEY_SHIFTDEL);

        /* $ 10.04.2001 SVS
          ^P/^M - ����������� ��������: ������ ��� CurPos ������ ���� ">=",
           � �� "������".
        */
        if (Key==KEY_CTRLM && SelStart!=-1 && SelEnd!=-1)
          if (CurPos>=SelEnd)
            CurLine->EditLine.SetCurPos(CurPos-(SelEnd-SelStart));
          else
            CurLine->EditLine.SetCurPos(CurPos);
        /* SVS $ */
        ProcessKey(KEY_SHIFTINS);
        Pasting--;
        FAR_EmptyClipboard();
        UsedInternalClipboard=OldUsedInternalClipboard;

        /*$ 08.02.2001 SKV
          �� �������� � pasting'��, ������� redraw �������� �� ����.
          ������� ���.
        */
        Show();
        /* SKV$*/
      }
      return(TRUE);
    }

    case KEY_CTRLX:
    case KEY_SHIFTDEL:
    {
      Copy(FALSE);
    }
    case KEY_CTRLD:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
        return TRUE;
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      DeleteBlock();
      Show();
      return(TRUE);
    }

    case KEY_CTRLV:
    case KEY_SHIFTINS: case KEY_SHIFTNUMPAD0:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
        return TRUE;
      /* $ 10.04.2001 SVS
         ������ Pasting ��������� :-(
      */
      Pasting++;
      if (!EdOpt.PersistentBlocks && VBlockStart==NULL)
        DeleteBlock();

      Paste();
      // MarkingBlock=(VBlockStart==NULL);
      Flags.Change(FEDITOR_MARKINGBLOCK,(VBlockStart==NULL));
      Flags.Clear(FEDITOR_MARKINGVBLOCK);
      if (!EdOpt.PersistentBlocks)
        UnmarkBlock();
      Pasting--;
      Show();
      return(TRUE);
      /* SVS $ */
    }

    case KEY_LEFT: case KEY_NUMPAD4:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      if (CurPos==0 && CurLine->Prev!=NULL)
      {
        Up();
        Show();
        CurLine->EditLine.ProcessKey(KEY_END);
        Show();
      }
      else
      {
        int LeftPos=CurLine->EditLine.GetLeftPos();
        CurLine->EditLine.ProcessKey(KEY_LEFT);
        ShowEditor(LeftPos==CurLine->EditLine.GetLeftPos());
      }
      return(TRUE);
    }

    case KEY_INS: case KEY_NUMPAD0:
    {
      Flags.Swap(FEDITOR_OVERTYPE);
      Show();
      return(TRUE);
    }

    case KEY_DEL:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        // Del � ����� ��������� ������� ������ �� �������, ������� �� ������������...
        if(!CurLine->Next && CurPos>=CurLine->EditLine.GetLength() && BlockStart==NULL && VBlockStart==NULL)
          return TRUE;
        /* $ 07.03.2002 IS
           ������ ���������, ���� ���� ��� ����� ������
        */
        if(!Pasting)
          UnmarkEmptyBlock();
        /* IS $ */
        if (!Pasting && EdOpt.DelRemovesBlocks && (BlockStart!=NULL || VBlockStart!=NULL))
          DeleteBlock();
        else
        {
          AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
                      CurLine->EditLine.GetCurPos(),UNDO_EDIT);
          if (CurPos>=CurLine->EditLine.GetLength())
          {
            if (CurLine->Next==NULL)
              CurLine->EditLine.SetEOL("");
            else
            {
              int SelStart,SelEnd,NextSelStart,NextSelEnd;
              int Length=CurLine->EditLine.GetLength();
              CurLine->EditLine.GetSelection(SelStart,SelEnd);
              CurLine->Next->EditLine.GetSelection(NextSelStart,NextSelEnd);

              const char *Str;
              int NextLength;
              CurLine->Next->EditLine.GetBinaryString(Str,NULL,NextLength);
              CurLine->EditLine.InsertBinaryString(Str,NextLength);
              CurLine->EditLine.SetCurPos(CurPos);

              BlockUndo++;
              DeleteString(CurLine->Next,TRUE,NumLine+1);
              BlockUndo--;
              if (NextLength==0)
                CurLine->EditLine.SetEOL("");

              if (NextSelStart!=-1)
                if (SelStart==-1)
                {
                  CurLine->EditLine.Select(Length+NextSelStart,NextSelEnd==-1 ? -1:Length+NextSelEnd);
                  BlockStart=CurLine;
                  BlockStartLine=NumLine;
                }
                else
                  CurLine->EditLine.Select(SelStart,NextSelEnd==-1 ? -1:Length+NextSelEnd);

            }
          }
          else
            CurLine->EditLine.ProcessKey(KEY_DEL);
          /*$ 10.08.2000 skv
            Modified->TextChanged
          */
          TextChanged(1);
          /* skv $*/
        }
        Show();
      }
      return(TRUE);
    }

    case KEY_BS:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        // Bs � ����� ������ ������� ������ �� �������, ������ �� ����� ����������
        if(!CurLine->Prev && !CurPos && BlockStart==NULL && VBlockStart==NULL)
          return TRUE;
        /*$ 10.08.2000 skv
          Modified->TextChanged
        */
        TextChanged(1);
        /* skv $*/
        /* $ 11.10.2000 SVS
           Bs ������� ���� ��� ��, ��� � Del
        */
        int IsDelBlock=FALSE;
        if(EdOpt.BSLikeDel)
        {
          if (!Pasting && EdOpt.DelRemovesBlocks && (BlockStart!=NULL || VBlockStart!=NULL))
            IsDelBlock=TRUE;
        }
        else
        {
          if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
            IsDelBlock=TRUE;
        }
        if (IsDelBlock)
        /* SVS $ */
          DeleteBlock();
        else
          if (CurPos==0 && CurLine->Prev!=NULL)
          {
            Pasting++;
            Up();
            CurLine->EditLine.ProcessKey(KEY_CTRLEND);
            ProcessKey(KEY_DEL);
            Pasting--;
          }
          else
          {
            AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
                        CurLine->EditLine.GetCurPos(),UNDO_EDIT);
            CurLine->EditLine.ProcessKey(KEY_BS);
          }

        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLBS:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        /*$ 10.08.2000 skv
          Modified->TextChanged
        */
        TextChanged(1);
        /* skv $*/
        if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
          DeleteBlock();
        else
          if (CurPos==0 && CurLine->Prev!=NULL)
            ProcessKey(KEY_BS);
          else
          {
            AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
                        CurLine->EditLine.GetCurPos(),UNDO_EDIT);
            CurLine->EditLine.ProcessKey(KEY_CTRLBS);
          }
        Show();
      }
      return(TRUE);
    }

    case KEY_UP: case KEY_NUMPAD8:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int PrevMaxPos=MaxRightPos;
        struct EditList *LastTopScreen=TopScreen;
        Up();
        if (TopScreen==LastTopScreen)
          ShowEditor(TRUE);
        else
          Show();
        if (PrevMaxPos>CurLine->EditLine.GetTabCurPos())
        {
          CurLine->EditLine.SetTabCurPos(PrevMaxPos);
          CurLine->EditLine.FastShow();
          CurLine->EditLine.SetTabCurPos(PrevMaxPos);
          Show();
        }
      }
      return(TRUE);
    }

    case KEY_DOWN: case KEY_NUMPAD2:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int PrevMaxPos=MaxRightPos;
        struct EditList *LastTopScreen=TopScreen;
        Down();
        if (TopScreen==LastTopScreen)
          ShowEditor(TRUE);
        else
          Show();
        if (PrevMaxPos>CurLine->EditLine.GetTabCurPos())
        {
          CurLine->EditLine.SetTabCurPos(PrevMaxPos);
          CurLine->EditLine.FastShow();
          CurLine->EditLine.SetTabCurPos(PrevMaxPos);
          Show();
        }
      }
      return(TRUE);
    }

    /* $ 27.04.2001 VVM
      + ��������� ������ ����� */
    case KEY_MSWHEEL_UP:
    case (KEY_MSWHEEL_UP | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaEdit;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_CTRLUP);
      return(TRUE);
    }

    case KEY_MSWHEEL_DOWN:
    case (KEY_MSWHEEL_DOWN | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaEdit;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_CTRLDOWN);
      return(TRUE);
    }
    /* VVM $ */

    case KEY_CTRLUP:  case KEY_CTRLNUMPAD8:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      ScrollUp();
      Show();
      return(TRUE);
    }

    case KEY_CTRLDOWN: case KEY_CTRLNUMPAD2:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      ScrollDown();
      Show();
      return(TRUE);
    }

    case KEY_PGUP:     case KEY_NUMPAD9:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      for (I=Y1+1;I<Y2;I++)
        ScrollUp();
      Show();
      return(TRUE);
    }

    case KEY_PGDN:    case KEY_NUMPAD3:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      for (I=Y1+1;I<Y2;I++)
        ScrollDown();
      Show();
      return(TRUE);
    }

    case KEY_CTRLHOME:  case KEY_CTRLNUMPAD7:
    case KEY_CTRLPGUP:  case KEY_CTRLNUMPAD9:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int StartPos=CurLine->EditLine.GetTabCurPos();
        NumLine=0;
        TopScreen=CurLine=TopList;
        if (Key==KEY_CTRLHOME)
          CurLine->EditLine.SetCurPos(0);
        else
          CurLine->EditLine.SetTabCurPos(StartPos);
        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLEND:   case KEY_CTRLNUMPAD1:
    case KEY_CTRLPGDN:  case KEY_CTRLNUMPAD3:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int StartPos=CurLine->EditLine.GetTabCurPos();
        NumLine=NumLastLine-1;
        CurLine=EndList;
        for (TopScreen=CurLine,I=Y1+1;I<Y2 && TopScreen->Prev!=NULL;I++)
        {
          TopScreen->EditLine.SetPosition(X1,I,X2,I);
          TopScreen=TopScreen->Prev;
        }
        CurLine->EditLine.SetLeftPos(0);
        if (Key==KEY_CTRLEND)
        {
          CurLine->EditLine.SetCurPos(CurLine->EditLine.GetLength());
          CurLine->EditLine.FastShow();
        }
        else
          CurLine->EditLine.SetTabCurPos(StartPos);
        Show();
      }
      return(TRUE);
    }

    case KEY_ENTER:
    {
      if (Pasting || !ShiftPressed || CtrlObject->Macro.IsExecuting())
      {
        if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
          DeleteBlock();
        Flags.Set(FEDITOR_NEWUNDO);
        InsertString();
        CurLine->EditLine.FastShow();
        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLN:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      while (CurLine!=TopScreen)
      {
        CurLine=CurLine->Prev;
        NumLine--;
      }
      CurLine->EditLine.SetCurPos(CurPos);
      Show();
      return(TRUE);
    }

    case KEY_CTRLE:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        struct EditList *CurPtr=TopScreen;
        int CurLineFound=FALSE;
        for (I=Y1+1;I<Y2;I++)
        {
          if (CurPtr->Next==NULL)
            break;
          if (CurPtr==CurLine)
            CurLineFound=TRUE;
          if (CurLineFound)
            NumLine++;
          CurPtr=CurPtr->Next;
        }
        CurLine=CurPtr;
        CurLine->EditLine.SetCurPos(CurPos);
        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLL:
    {
      Flags.Swap(FEDITOR_LOCKMODE);
      if(HostFileEditor) HostFileEditor->ShowStatus();
      return(TRUE);
    }

    case KEY_CTRLY:
    {
      DeleteString(CurLine,FALSE,NumLine);
      Show();
      return(TRUE);
    }

    case KEY_F7:
    {
      int ReplaceMode0=ReplaceMode;
      int ReplaceAll0=ReplaceAll;
      ReplaceMode=ReplaceAll=FALSE;
      if(!Search(FALSE))
      {
        ReplaceMode=ReplaceMode0;
        ReplaceAll=ReplaceAll0;
      }
      return(TRUE);
    }

    case KEY_CTRLF7:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        int ReplaceMode0=ReplaceMode;
        int ReplaceAll0=ReplaceAll;
        ReplaceMode=TRUE;
        ReplaceAll=FALSE;
        if(!Search(FALSE))
        {
          ReplaceMode=ReplaceMode0;
          ReplaceAll=ReplaceAll0;
        }
      }
      return(TRUE);
    }

    case KEY_SHIFTF7:
    {
      /* $ 20.09.2000 SVS
         ��� All ����� ������� Shift-F7 ������� ����� ��������...
      */
      //ReplaceAll=FALSE;
      /* SVS $*/
      /* $ 07.05.2001 IS
         ������� � ����� "Shift-F7 ���������� _�����_"
      */
      //ReplaceMode=FALSE;
      /* IS */
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      Search(TRUE);
      return(TRUE);
    }

    case KEY_F8:
    {
      Flags.Set(FEDITOR_TABLECHANGEDBYUSER);
      if ((AnsiText=!AnsiText)!=0)
      {
        int UseUnicode=FALSE;
        GetTable(&TableSet,TRUE,TableNum,UseUnicode);
      }
      TableNum=0;
      UseDecodeTable=AnsiText;
      SetStringsTable();
      if (HostFileEditor) HostFileEditor->ChangeEditKeyBar();
      Show();
      return(TRUE);
    }

    case KEY_SHIFTF8:
    {
      {
        int UseUnicode=FALSE;
        int GetTableCode=GetTable(&TableSet,FALSE,TableNum,UseUnicode);
        if (GetTableCode!=-1)
        {
          Flags.Set(FEDITOR_TABLECHANGEDBYUSER);
          UseDecodeTable=GetTableCode;
          AnsiText=FALSE;
          SetStringsTable();
          if (HostFileEditor) HostFileEditor->ChangeEditKeyBar();
          Show();
        }
      }
      return(TRUE);
    }

    case KEY_F11:
    {
/*
      CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
      if (CtrlObject->Plugins.CommandsMenu(MODALTYPE_EDITOR,0,"Editor"))
        *PluginTitle=0;
      Show();
*/
      return(TRUE);
    }

    case KEY_ALTBS:
    case KEY_CTRLZ:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        /*$ 10.08.2000 skv
          Without this group undo, like undo of 'delete block' operation
          will be animated.
        */
        DisableOut++;
        Undo();
        DisableOut--;
        /* skv$*/
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTF8:
    {
      {
        /* $ 05.07.2000 tran
           + ����������� ���������� �� ������ �� ������, �� � �� ������� */
        /* $ 21.07.2000 tran
           ��� ������ ������� */
        GoToPosition();
        /* tran 21.07.2000 $ */
        /* tran 05.07.2000 $ */
        // <GOTO_UNMARK:1>
        if (!EdOpt.PersistentBlocks)
          UnmarkBlock();
        // </GOTO_UNMARK>
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTU:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        BlockLeft();
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTI:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        BlockRight();
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTSHIFTLEFT:  case KEY_ALTSHIFTNUMPAD4:
    case KEY_ALTLEFT:
    {
      if (CurPos==0)
        return(TRUE);
      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */
      Pasting++;
      {
        int Delta=CurLine->EditLine.GetTabCurPos()-CurLine->EditLine.RealPosToTab(CurPos-1);
        if (CurLine->EditLine.GetTabCurPos()>VBlockX)
          VBlockSizeX-=Delta;
        else
        {
          VBlockX-=Delta;
          VBlockSizeX+=Delta;
        }
        /* $ 25.07.2000 tran
           ������� ���� 22 - ��������� ��� �������� �� ������� ����� */
        if ( VBlockSizeX<0 )
        {
            VBlockSizeX=-VBlockSizeX;
            VBlockX-=VBlockSizeX;
        }
        /* tran 25.07.2000 $ */
        ProcessKey(KEY_LEFT);
      }
      Pasting--;
      Show();
      //_D(SysLog("VBlockX=%i, VBlockSizeX=%i, GetLineCurPos=%i",VBlockX,VBlockSizeX,GetLineCurPos()));
      //_D(SysLog("~~~~~~~~~~~~~~~~ KEY_ALTLEFT END, VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      return(TRUE);
    }

    case KEY_ALTSHIFTRIGHT:  case KEY_ALTSHIFTNUMPAD6:
    case KEY_ALTRIGHT:
    {
      /* $ 23.10.2000 tran
         ������ GetTabCurPos ���� �������� GetCurPos -
         ���������� �������� ������� � �������� ������
         � ���� ��������� ������� �������� � �������� ������*/
      if (!EdOpt.CursorBeyondEOL && CurLine->EditLine.GetCurPos()>=CurLine->EditLine.GetLength())
        return(TRUE);
      /* tran 23.10.2000 $ */

      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */

      /* $ 21.07.2000 tran
         bug22 - �����������
      */
      //_D(SysLog("---------------- KEY_ALTRIGHT, getLineCurPos=%i",GetLineCurPos()));
      Pasting++;
      {
        int Delta;
        /* $ 18.07.2000 tran
             ������ � ������ ������, ����� alt-right, alt-pagedown,
             ��������� ���� ������� � 1 �������, ����� ��� alt-right
             ��������� ���������
        */
        int VisPos=CurLine->EditLine.RealPosToTab(CurPos),
            NextVisPos=CurLine->EditLine.RealPosToTab(CurPos+1);
        //_D(SysLog("CurPos=%i, VisPos=%i, NextVisPos=%i",
        //    CurPos,VisPos, NextVisPos); //,CurLine->EditLine.GetTabCurPos()));

        Delta=NextVisPos-VisPos;
         //_D(SysLog("Delta=%i",Delta));
        /* tran $ */

        if (CurLine->EditLine.GetTabCurPos()>=VBlockX+VBlockSizeX)
          VBlockSizeX+=Delta;
        else
        {
          VBlockX+=Delta;
          VBlockSizeX-=Delta;
        }
        /* $ 25.07.2000 tran
           ������� ���� 22 - ��������� ��� �������� �� ������� ����� */
        if ( VBlockSizeX<0 )
        {
            VBlockSizeX=-VBlockSizeX;
            VBlockX-=VBlockSizeX;
        }
        /* tran 25.07.2000 $ */
        ProcessKey(KEY_RIGHT);
        //_D(SysLog("VBlockX=%i, VBlockSizeX=%i, GetLineCurPos=%i",VBlockX,VBlockSizeX,GetLineCurPos()));
      }
      Pasting--;
      Show();
      //_D(SysLog("~~~~~~~~~~~~~~~~ KEY_ALTRIGHT END, VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      /* tran 21.07.2000 $ */

      return(TRUE);
    }

  /* $ 29.06.2000 IG
      + CtrlAltLeft, CtrlAltRight ��� ������������ ������
  */
    case KEY_CTRLALTLEFT: case KEY_CTRLALTNUMPAD4:
    {
      {
        int SkipSpace=TRUE;
        Pasting++;
        /* $ 23.12.2000 OT */
        DisableOut++;
        Edit::DisableEditOut(TRUE);
        /* OT $ */
        while (1)
        {
          const char *Str;
          int Length;
          CurLine->EditLine.GetBinaryString(Str,NULL,Length);
          int CurPos=CurLine->EditLine.GetCurPos();
          if (CurPos>Length)
          {
            CurLine->EditLine.ProcessKey(KEY_END);
            CurPos=CurLine->EditLine.GetCurPos();
          }
          if (CurPos==0)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos-1]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos-1]))
          /* IS $ */
          /* SVS $ */
            if (SkipSpace)
            {
              ProcessKey(KEY_ALTSHIFTLEFT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_ALTSHIFTLEFT);
        }
        Pasting--;
        /* $ 23.12.2000 OT */
        DisableOut--;
        Edit::DisableEditOut(FALSE);
        Show();
        /* OT $ */
      }
      return(TRUE);
    }

    case KEY_CTRLALTRIGHT: case KEY_CTRLALTNUMPAD6:
    {
      {
        int SkipSpace=TRUE;
        Pasting++;
        DisableOut++;
        Edit::DisableEditOut(TRUE);
        while (1)
        {
          const char *Str;
          int Length;
          CurLine->EditLine.GetBinaryString(Str,NULL,Length);
          int CurPos=CurLine->EditLine.GetCurPos();
          if (CurPos>=Length)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos]))
          /* IS $ */
          /* SVS $*/
            if (SkipSpace)
            {
              ProcessKey(KEY_ALTSHIFTRIGHT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_ALTSHIFTRIGHT);
        }
        Pasting--;
        DisableOut--;
        Edit::DisableEditOut(FALSE);
        Show();
      }
      return(TRUE);
    }
    /* IG $ */

    case KEY_ALTSHIFTUP:    case KEY_ALTSHIFTNUMPAD8:
    case KEY_ALTUP:
    {
      if (CurLine->Prev==NULL)
        return(TRUE);
      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */

      if (!EdOpt.CursorBeyondEOL && VBlockX>=CurLine->Prev->EditLine.GetLength())
        return(TRUE);
      Pasting++;
      if (NumLine>VBlockY)
        VBlockSizeY--;
      else
      {
        VBlockY--;
        VBlockSizeY++;
        VBlockStart=VBlockStart->Prev;
        BlockStartLine--;
      }
      ProcessKey(KEY_UP);
      /* $ 21.07.2000 tran
         �������� ������� �������� ����� */
      AdjustVBlock(CurVisPos);
      /* tran 21.07.2000 $ */
      Pasting--;
      Show();
      //_D(SysLog("~~~~~~~~ ALT_PGUP, VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      return(TRUE);
    }

    case KEY_ALTSHIFTDOWN:  case KEY_ALTSHIFTNUMPAD2:
    case KEY_ALTDOWN:
    {
      if (CurLine->Next==NULL)
        return(TRUE);
      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */
      if (!EdOpt.CursorBeyondEOL && VBlockX>=CurLine->Next->EditLine.GetLength())
        return(TRUE);
      Pasting++;
      if (NumLine>=VBlockY+VBlockSizeY-1)
        VBlockSizeY++;
      else
      {
        VBlockY++;
        VBlockSizeY--;
        VBlockStart=VBlockStart->Next;
        BlockStartLine++;
      }
      ProcessKey(KEY_DOWN);
      /* $ 21.07.2000 tran
         �������� ������� �������� ����� */
      AdjustVBlock(CurVisPos);
      /* tran 21.07.2000 $ */
      Pasting--;
      Show();
      //_D(SysLog("~~~~ Key_AltDOWN: VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      return(TRUE);
    }

    case KEY_ALTSHIFTHOME: case KEY_ALTSHIFTNUMPAD7:
    case KEY_ALTHOME:
    {
      Pasting++;
      DisableOut++;
      while (CurLine->EditLine.GetCurPos()>0)
        ProcessKey(KEY_ALTSHIFTLEFT);
      DisableOut--;
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_ALTSHIFTEND: case KEY_ALTSHIFTNUMPAD1:
    case KEY_ALTEND:
    {
      Pasting++;
      DisableOut++;
      if (CurLine->EditLine.GetCurPos()<CurLine->EditLine.GetLength())
        while (CurLine->EditLine.GetCurPos()<CurLine->EditLine.GetLength())
          ProcessKey(KEY_ALTSHIFTRIGHT);
      if (CurLine->EditLine.GetCurPos()>CurLine->EditLine.GetLength())
        while (CurLine->EditLine.GetCurPos()>CurLine->EditLine.GetLength())
          ProcessKey(KEY_ALTSHIFTLEFT);
      DisableOut--;
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_ALTSHIFTPGUP: case KEY_ALTSHIFTNUMPAD9:
    case KEY_ALTPGUP:
    {
      Pasting++;
      DisableOut++;
      for (I=Y1+1;I<Y2;I++)
        ProcessKey(KEY_ALTSHIFTUP);
      DisableOut--;
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_ALTSHIFTPGDN: case KEY_ALTSHIFTNUMPAD3:
    case KEY_ALTPGDN:
    {
      Pasting++;
      DisableOut++;
      for (I=Y1+1;I<Y2;I++)
        ProcessKey(KEY_ALTSHIFTDOWN);
      DisableOut--;
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_CTRLALTPGUP: case KEY_CTRLALTNUMPAD9:
    case KEY_CTRLALTHOME: case KEY_CTRLALTNUMPAD7:
    {
      DisableOut++;
      Pasting++;
      while (CurLine!=TopList)
      {
        Edit::DisableEditOut(TRUE);
        ProcessKey(KEY_ALTUP);
      }
      Pasting--;
      DisableOut--;
      Edit::DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_CTRLALTPGDN:  case KEY_CTRLALTNUMPAD3:
    case KEY_CTRLALTEND:   case KEY_CTRLALTNUMPAD1:
    {
      DisableOut++;
      Pasting++;
      while (CurLine!=EndList)
      {
        Edit::DisableEditOut(TRUE);
        ProcessKey(KEY_ALTDOWN);
      }
      Pasting--;
      DisableOut--;
      Edit::DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_CTRLALTBRACKET:       // �������� ������� (UNC) ���� �� ����� ������
    case KEY_CTRLALTBACKBRACKET:   // �������� ������� (UNC) ���� �� ������ ������
    case KEY_ALTSHIFTBRACKET:      // �������� ������� (UNC) ���� �� �������� ������
    case KEY_ALTSHIFTBACKBRACKET:  // �������� ������� (UNC) ���� �� ��������� ������
    case KEY_CTRLBRACKET:          // �������� ���� �� ����� ������
    case KEY_CTRLBACKBRACKET:      // �������� ���� �� ������ ������
    case KEY_CTRLSHIFTBRACKET:     // �������� ���� �� �������� ������
    case KEY_CTRLSHIFTBACKBRACKET: // �������� ���� �� ��������� ������

    case KEY_CTRLSHIFTENTER:
    case KEY_SHIFTENTER:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        Pasting++;
        TextChanged(1);
        if (!EdOpt.PersistentBlocks && BlockStart!=NULL)
        {
          Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
          DeleteBlock();
        }
        AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
                        CurLine->EditLine.GetCurPos(),UNDO_EDIT);
        CurLine->EditLine.ProcessKey(Key);
        Pasting--;
        Show();
      }
      return(TRUE);
    }

    /* $ 11.04.2001 SVS
       ��������� ��������� Ctrl-Q
    */
    case KEY_CTRLQ:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        Flags.Set(FEDITOR_PROCESSCTRLQ);
        if(HostFileEditor) HostFileEditor->ShowStatus();
        Pasting++;
        TextChanged(1);
        if (!EdOpt.PersistentBlocks && BlockStart!=NULL)
        {
          Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
          DeleteBlock();
        }
        AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
                        CurLine->EditLine.GetCurPos(),UNDO_EDIT);
        CurLine->EditLine.ProcessCtrlQ();
        Flags.Clear(FEDITOR_PROCESSCTRLQ);
        Pasting--;
        Show();
      }
      return(TRUE);
    }
    /* SVS $ */

    case MCODE_OP_DATE:
    case MCODE_OP_PLAINTEXT:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        const char *Fmt = eStackAsString();
        int SizeMacroText = 16+(*Fmt ? 0 : strlen(Opt.DateFormat));
        if(Key == MCODE_OP_PLAINTEXT)
          SizeMacroText=strlen(Fmt)+1;
        SizeMacroText*=4+1;
        char *TStr=(char*)alloca(SizeMacroText);
        if(!TStr)
          return FALSE;
        if(Key == MCODE_OP_PLAINTEXT)
          strcpy(TStr,Fmt);
        if(Key == MCODE_OP_PLAINTEXT || MkStrFTime(TStr,SizeMacroText,Fmt))
        {
          char *Ptr=TStr;
          while(*Ptr) // ������� 0x0A �� 0x0D �� �������� Paset ;-)
          {
            if(*Ptr == 10)
              *Ptr=13;
            ++Ptr;
          }

          Pasting++;
          //_SVS(SysLogDump(Fmt,0,TStr,strlen(TStr),NULL));
          TextChanged(1);
          BOOL IsBlock=VBlockStart || BlockStart;
          if (!EdOpt.PersistentBlocks && IsBlock)
          {
            Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
            DeleteBlock();
          }
          //AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
          //              CurLine->EditLine.GetCurPos(),UNDO_EDIT);
          Paste(TStr);
          //if (!EdOpt.PersistentBlocks && IsBlock)
          UnmarkBlock();
          Pasting--;
          Show();
        }
      }
      return(TRUE);
    }

    default:
    {
      {
        if ((Key==KEY_CTRLDEL || Key==KEY_CTRLT) && CurPos>=CurLine->EditLine.GetLength())
        {
         /*$ 08.12.2000 skv
           - CTRL-DEL � ������ ������ ��� ���������� ����� �
             ���������� EditorDelRemovesBlocks
         */
          int save=EdOpt.DelRemovesBlocks;
          EdOpt.DelRemovesBlocks=0;
          int ret=ProcessKey(KEY_DEL);
          EdOpt.DelRemovesBlocks=save;
          return ret;
          /* skv$*/
        }

        if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
          if (Key>=32 && Key<256 || Key==KEY_ADD || Key==KEY_SUBTRACT ||
              Key==KEY_MULTIPLY || Key==KEY_DIVIDE || Key==KEY_TAB)
          {
            DeleteBlock();
            /* $ 19.09.2002 SKV
              ������ ����.
              ����� ���� ��� ������� ��������� ��������
              ����� � ������ ����� �� ��������� � ���������
              ���������� ���� ����� �������.
            */
            Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
            /* SKV $ */
            Show();
          }

        int SkipCheckUndo=(Key==KEY_RIGHT     || Key==KEY_NUMPAD6     ||
                           Key==KEY_CTRLLEFT  || Key==KEY_CTRLNUMPAD4 ||
                           Key==KEY_CTRLRIGHT || Key==KEY_CTRLNUMPAD6 ||
                           Key==KEY_HOME      || Key==KEY_NUMPAD7     ||
                           Key==KEY_END       || Key==KEY_NUMPAD1     ||
                           Key==KEY_CTRLS);

        if (Flags.Check(FEDITOR_LOCKMODE) && !SkipCheckUndo)
          return(TRUE);

        if ((Key==KEY_CTRLLEFT || Key==KEY_CTRLNUMPAD4) && CurLine->EditLine.GetCurPos()==0)
        {
          Pasting++;
          ProcessKey(KEY_LEFT);
          Pasting--;
          /* $ 24.9.2001 SKV
            fix ���� � ctrl-left � ������ ������
            � ����� � ��������������� �������� �����.
          */
          ShowEditor(FALSE);
          //CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
          //_D(SysLog("%08d EE_REDRAW",__LINE__));
          //CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL);
          /* SKV$*/
          return(TRUE);
        }

        if ((!EdOpt.CursorBeyondEOL && Key==KEY_RIGHT || Key==KEY_NUMPAD6 || Key==KEY_CTRLRIGHT || Key==KEY_CTRLNUMPAD6) &&
            CurLine->EditLine.GetCurPos()>=CurLine->EditLine.GetLength() &&
            CurLine->Next!=NULL)
        {
          Pasting++;
          ProcessKey(KEY_HOME);
          ProcessKey(KEY_DOWN);
          Pasting--;
          CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
          //_D(SysLog("%08d EE_REDRAW",__LINE__));
          _SYS_EE_REDRAW(SysLog("Editor::ProcessKey[%d](!EdOpt.CursorBeyondEOL): EE_REDRAW(EEREDRAW_ALL)",__LINE__));
          CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL);
          /*$ 03.02.2001 SKV
            � �� EEREDRAW_ALL �� ������, � �� ����� ����
            ������ ������� ����� ����������������.
          */
          ShowEditor(0);
          /* SKV$*/
          return(TRUE);
        }

        const char *Str;
        char *CmpStr;
        int Length,CurPos;

        CurLine->EditLine.GetBinaryString(Str,NULL,Length);
        CurPos=CurLine->EditLine.GetCurPos();

        if (Key<256 && CurPos>0 && Length==0)
        {
          struct EditList *PrevLine=CurLine->Prev;
          while (PrevLine!=NULL && PrevLine->EditLine.GetLength()==0)
            PrevLine=PrevLine->Prev;
          if (PrevLine!=NULL)
          {
            int TabPos=CurLine->EditLine.GetTabCurPos();
            CurLine->EditLine.SetCurPos(0);
            const char *PrevStr;
            int PrevLength;
            PrevLine->EditLine.GetBinaryString(PrevStr,NULL,PrevLength);
            for (int I=0;I<PrevLength && IsSpace(PrevStr[I]);I++)
            {
              int NewTabPos=CurLine->EditLine.GetTabCurPos();
              if (NewTabPos==TabPos)
                break;
              if (NewTabPos>TabPos)
              {
                CurLine->EditLine.ProcessKey(KEY_BS);
                while (CurLine->EditLine.GetTabCurPos()<TabPos)
                  CurLine->EditLine.ProcessKey(' ');
                break;
              }
              if (NewTabPos<TabPos)
                CurLine->EditLine.ProcessKey(PrevStr[I]);
            }
            CurLine->EditLine.SetTabCurPos(TabPos);
          }
        }

        if (!SkipCheckUndo)
        {
          CurLine->EditLine.GetBinaryString(Str,NULL,Length);
          CurPos=CurLine->EditLine.GetCurPos();
          CmpStr=new char[Length+1];
          memcpy(CmpStr,Str,Length);
          CmpStr[Length]=0;
        }

        int LeftPos=CurLine->EditLine.GetLeftPos();

        /* $ 24.09.2000 SVS
           ����� ������� Xlat
        */
        /* $ 04.11.2000 SVS
           �������� �� �������������� �������
        */
        /* $ 25.11.2000 IS
           ������ Xlat �������� ���� ��� ���������� ���������
        */
        if((Opt.XLat.XLatEditorKey && Key == Opt.XLat.XLatEditorKey ||
            Opt.XLat.XLatAltEditorKey && Key == Opt.XLat.XLatAltEditorKey) ||
            Key == MCODE_OP_XLAT)
        /* IS  $ */
        {
          Xlat();
          Show();
          return TRUE;
        }
        /* SVS $ */
        /* SVS $ */

        // <comment> - ��� ��������� ��� ���������� ������ ������ ������ ��� Ctrl-K
        int PreSelStart,PreSelEnd;
        CurLine->EditLine.GetSelection(PreSelStart,PreSelEnd);
        // </comment>

        if (CurLine->EditLine.ProcessKey(Key))
        {
          int SelStart,SelEnd;
          /* $ 17.09.2002 SKV
            ���� ��������� � �������� �����,
            � ������ ������, � �������� tab, ������� ����������
            �� �������, ��������� ������. ��� ����.
          */
          if(Key==KEY_TAB && CurLine->EditLine.GetConvertTabs() &&
             BlockStart!=NULL && BlockStart!=CurLine)
          {
            CurLine->EditLine.GetSelection(SelStart,SelEnd);
            CurLine->EditLine.Select(0,SelEnd);
          }
          /* SKV $ */
          if (!SkipCheckUndo)
          {
            const char *NewCmpStr;
            int NewLength;
            CurLine->EditLine.GetBinaryString(NewCmpStr,NULL,NewLength);
            if (NewLength!=Length || memcmp(CmpStr,NewCmpStr,Length)!=0)
            {
              AddUndoData(CmpStr,NumLine,CurPos,UNDO_EDIT);
              /*$ 10.08.2000 skv
                Modified->TextChanged
              */
              TextChanged(1);
              /* skv $*/
            }
            delete[] CmpStr;
          }
          // <Bug 794>
          // ���������� ������ ������ � ��������� ������ � ������
          if(Key == KEY_CTRLK && EdOpt.PersistentBlocks)
          {
             if(CurLine==BlockStart)
             {
               if(CurPos)
               {
                 CurLine->EditLine.GetSelection(SelStart,SelEnd);
                 // 1. ���� �� ������ ������ (CurPos ��� ����� � ������, ��� SelStart)
                 if(SelEnd == -1 && PreSelStart > CurPos || SelEnd > CurPos)
                   SelStart=SelEnd=-1; // � ���� ������ ������� ���������
                 // 2. CurPos ������ �����
                 else if(SelEnd == -1 && PreSelEnd > CurPos && SelStart < CurPos)
                   SelEnd=PreSelEnd;   // � ���� ������ ������� ����
                 // 3. ���� ������� ����� �� CurPos ��� ��������� ����� ����� (��. ����)
                 if(SelEnd >= CurPos || SelStart==-1)
                   CurLine->EditLine.Select(SelStart,CurPos);
               }
               else
               {
                 CurLine->EditLine.Select(-1,-1);
                 BlockStart=BlockStart->Next;
               }
             }
             else // ����� ������ !!! ���� ���������� ���� ���������� ������� (�� �������), �� ���� ��������... ����� ��������...
             {
               // ������ ��� ��������� ������ (� ��������� �� ���)
               struct EditList *CurPtrBlock=BlockStart,*CurPtrBlock2=BlockStart;
               while (CurPtrBlock!=NULL)
               {
                 CurPtrBlock->EditLine.GetRealSelection(SelStart,SelEnd);
                 if (SelStart==-1)
                   break;
                 CurPtrBlock2=CurPtrBlock;
                 CurPtrBlock=CurPtrBlock->Next;
               }

               if(CurLine==CurPtrBlock2)
               {
                 if(CurPos)
                 {
                   CurLine->EditLine.GetSelection(SelStart,SelEnd);
                   CurLine->EditLine.Select(SelStart,CurPos);
                 }
                 else
                 {
                   CurLine->EditLine.Select(-1,-1);
                   CurPtrBlock2=CurPtrBlock2->Next;
                 }
               }

             }
          }
          // </Bug 794>

          ShowEditor(LeftPos==CurLine->EditLine.GetLeftPos());
          return(TRUE);
        }
        else
          if (!SkipCheckUndo)
            delete[] CmpStr;
        if (VBlockStart!=NULL)
          Show();
      }
      return(FALSE);
    }
  }
}


int Editor::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  struct EditList *NewPtr;
  int NewDist,Dist;
/* $ 28.12.2000 VVM
  + ������ ������ ������� ������������ ���� ������ */
  if ((MouseEvent->dwButtonState & 3)!=0)
  {
    Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
    if ((!EdOpt.PersistentBlocks) && (BlockStart!=NULL || VBlockStart!=NULL))
    {
      UnmarkBlock();
      Show();
    } /* if */
  } /* if */
  if (CurLine->EditLine.ProcessMouse(MouseEvent))
  {
    if(HostFileEditor) HostFileEditor->ShowStatus();
    if (VBlockStart!=NULL)
      Show();
    else
    {
      CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
      _SYS_EE_REDRAW(SysLog("Editor::ProcessMouse[%08d] ProcessEditorEvent(EE_REDRAW,EEREDRAW_LINE)",__LINE__));
      CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_LINE);
    }
    return(TRUE);
  }
  if ((MouseEvent->dwButtonState & 3)==0)
    return(FALSE);
/* VVM $ */
  if (MouseEvent->dwMousePosition.Y==Y1)
  {
    while (IsMouseButtonPressed() && MouseY==Y1)
      ProcessKey(KEY_UP);
    return(TRUE);
  }
  if (MouseEvent->dwMousePosition.Y==Y2+1)
  {
    while (IsMouseButtonPressed() && MouseY==Y2+1)
      ProcessKey(KEY_DOWN);
    return(TRUE);
  }
  if (MouseEvent->dwMousePosition.X<X1 || MouseEvent->dwMousePosition.X>X2 ||
      MouseEvent->dwMousePosition.Y<=Y1 || MouseEvent->dwMousePosition.Y>Y2)
    return(FALSE);
  NewDist=MouseEvent->dwMousePosition.Y-Y1-1;
  NewPtr=TopScreen;
  while (NewDist-- && NewPtr->Next)
    NewPtr=NewPtr->Next;

  Dist=CalcDistance(TopScreen,NewPtr,-1)-CalcDistance(TopScreen,CurLine,-1);

  if (Dist>0)
    while (Dist--)
      Down();
  else
    while (Dist++)
      Up();
  CurLine->EditLine.ProcessMouse(MouseEvent);
  Show();
  return(TRUE);
}


int Editor::CalcDistance(struct EditList *From,struct EditList *To,int MaxDist)
{
  int Distance=0;
  while (From!=To && From->Next!=NULL && (MaxDist==-1 || MaxDist-- > 0))
  {
    Distance++;
    From=From->Next;
  }
  return(Distance);
}



void Editor::DeleteString(struct EditList *DelPtr,int DeleteLast,int UndoLine)
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;
  /* $ 16.12.2000 OT
     CtrlY �� ��������� ������ � ���������� ������������ ������ �� ������ ��������� */
  if (VBlockStart!=NULL && NumLine<VBlockY+VBlockSizeY)
    if (NumLine<VBlockY)
    {
      if (VBlockY>0)
      {
        VBlockY--;
        BlockStartLine--;
      }
    }
    else
      if (--VBlockSizeY<=0)
        VBlockStart=NULL;
  /* OT $ */

  /*$ 10.08.2000 skv
    Modified->TextChanged
  */
  TextChanged(1);
  /* skv $*/
  if (DelPtr->Next==NULL && (!DeleteLast || DelPtr->Prev==NULL))
  {
    AddUndoData(DelPtr->EditLine.GetStringAddr(),UndoLine,
                DelPtr->EditLine.GetCurPos(),UNDO_EDIT);
    DelPtr->EditLine.SetString("");
    return;
  }

  for (int I=0;I<sizeof(SavePos.Line)/sizeof(SavePos.Line[0]);I++)
    if (SavePos.Line[I]!=0xffffffff && UndoLine<SavePos.Line[I])
      SavePos.Line[I]--;

  NumLastLine--;

  if (CurLine==DelPtr)
  {
    int LeftPos,CurPos;
    CurPos=DelPtr->EditLine.GetTabCurPos();
    LeftPos=DelPtr->EditLine.GetLeftPos();
    if (DelPtr->Next!=NULL)
      CurLine=DelPtr->Next;
    else
    {
      CurLine=DelPtr->Prev;
      /* $ 04.11.2002 SKV
        ����� ��� ���� ��� ���������, ����� ������� ������ ���� ��������.
      */
      NumLine--;
      /* SKV $ */
    }
    CurLine->EditLine.SetLeftPos(LeftPos);
    CurLine->EditLine.SetTabCurPos(CurPos);
  }

  if (DelPtr->Prev)
  {
    DelPtr->Prev->Next=DelPtr->Next;
    if (DelPtr==EndList)
      EndList=EndList->Prev;
  }
  if (DelPtr->Next!=NULL)
    DelPtr->Next->Prev=DelPtr->Prev;
  if (DelPtr==TopScreen)
    if (TopScreen->Next!=NULL)
      TopScreen=TopScreen->Next;
    else
      TopScreen=TopScreen->Prev;
  if (DelPtr==TopList)
    TopList=TopList->Next;
  if (DelPtr==BlockStart)
    BlockStart=BlockStart->Next;
  if (DelPtr==VBlockStart)
    VBlockStart=VBlockStart->Next;
  if (UndoLine!=-1)
    AddUndoData(DelPtr->EditLine.GetStringAddr(),UndoLine,0,UNDO_DELSTR);
  delete DelPtr;
}


void Editor::InsertString()
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;
  /*$ 10.08.2000 skv
    There is only one return - if new will fail.
    In this case things are realy bad.
    Move TextChanged to the end of functions
    AFTER all modifications are made.
  */
//  TextChanged(1);
  /* skv $*/
  struct EditList *NewString;
  struct EditList *SrcIndent=NULL;
  int SelStart,SelEnd;
  int CurPos;
  /* $ 17.07.2000 tran
     + ����� ���������� */
  int NewLineEmpty=TRUE;
  /* tran 17.07.2000 $ */

  if ((NewString=new struct EditList)==NULL)
    return;

  NewString->EditLine.SetObjectColor(COL_EDITORTEXT,COL_EDITORSELECTEDTEXT);
  /* $ 14.02.2001 IS
       ��������� ������ ������ ���������
  */
  NewString->EditLine.SetTabSize(EdOpt.TabSize);
  /* IS $ */
  /* $ 24.06.2002 SKV
    � PersistentBlocks ��� ����� �������������?
  */
  NewString->EditLine.SetPersistentBlocks(EdOpt.PersistentBlocks);
  /* SKV $ */
  NewString->EditLine.SetConvertTabs(EdOpt.ExpandTabs);
  NewString->EditLine.SetTables(UseDecodeTable ? &TableSet:NULL);
  NewString->EditLine.SetEditBeyondEnd(EdOpt.CursorBeyondEOL);
  NewString->EditLine.SetEditorMode(TRUE);
  NewString->EditLine.SetWordDiv(EdOpt.WordDiv);
  NewString->Prev=CurLine;
  NewString->Next=CurLine->Next;
  if (CurLine->Next)
    CurLine->Next->Prev=NewString;
  CurLine->Next=NewString;
  int Length;
  char *CurLineStr;
  const char *EndSeq;
  CurLine->EditLine.GetBinaryString(CurLineStr,&EndSeq,Length);

  /* $ 13.01.2002 IS
     ���� �� ��� ��������� ��� ����� ������, �� ������� ��� ����� ������
     � ��� ����� DOS_EOL_fmt � ��������� ��� ����.
  */
  if (!*EndSeq)
      CurLine->EditLine.SetEOL(*GlobalEOL?GlobalEOL:DOS_EOL_fmt);
  /* IS $ */

  CurPos=CurLine->EditLine.GetCurPos();
  CurLine->EditLine.GetSelection(SelStart,SelEnd);

  for (int I=0;I<sizeof(SavePos.Line)/sizeof(SavePos.Line[0]);I++)
    if (SavePos.Line[I]!=0xffffffff &&
        (NumLine<SavePos.Line[I] || NumLine==SavePos.Line[I] && CurPos==0))
      SavePos.Line[I]++;

  int IndentPos=0;

  if (EdOpt.AutoIndent && !Pasting)
  {
    struct EditList *PrevLine=CurLine;
    while (PrevLine!=NULL)
    {
      const char *Str;
      int Length,Found=FALSE;
      PrevLine->EditLine.GetBinaryString(Str,NULL,Length);
      for (int I=0;I<Length;I++)
        /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
        if (!IsSpace(Str[I]))
        /* IS $ */
        {
          PrevLine->EditLine.SetCurPos(I);
          IndentPos=PrevLine->EditLine.GetTabCurPos();
          SrcIndent=PrevLine;
          Found=TRUE;
          break;
        }
      if (Found)
        break;
      PrevLine=PrevLine->Prev;
    }
  }

  int SpaceOnly=TRUE;

  if (CurPos<Length)
  {


    /* $ 30.08.2000 tran
       ��������������� ���, ��� ������.
    */
    /* $ 17.07.2000 tran
       - �������������� ���, ��� �� ������*/
    if (IndentPos>0)
      for (int I=0;I<CurPos;I++)
        /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
        if (!IsSpace(CurLineStr[I]))
        /* IS $ */
        {
          SpaceOnly=FALSE;
          break;
        }
    /* tran 30.08.2000 $ */

    NewString->EditLine.SetBinaryString(&CurLineStr[CurPos],Length-CurPos);
    /* $ 17.07.2000 tran
       ��� �� ��������� ����� ������, ���� �� �� ��� ��� ������ ����� ��������
    */
    for ( int i0=0; i0<Length-CurPos; i0++ )
    {
        /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
        if (!IsSpace(CurLineStr[i0+CurPos]))
        /* IS $ */
        {
            NewLineEmpty=FALSE;
            break;
        }
    }
    /* tran 17.07.2000 $ */

    AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
                CurLine->EditLine.GetCurPos(),UNDO_EDIT);
    BlockUndo++;
    AddUndoData(NULL,NumLine+1,0,UNDO_INSSTR);
    BlockUndo--;
    CurLineStr[CurPos]=0;
    int StrSize=CurPos;
    /* $ 17.07.2000 tran
       � ��� � ������� �������� �������� �� ���� ����� ���������� */
    if (EdOpt.AutoIndent && NewLineEmpty)
    {
      RemoveTrailingSpaces(CurLineStr);
      StrSize=strlen(CurLineStr);
    }
    /* tran 17.07.2000 $ */

    /*$ SKV 21.02.2002
      � ���� ����� �������� EOL ������.
      ���� ��� ������� �� ����.
    */
    CurLine->EditLine.SetBinaryString(CurLineStr,StrSize);
    CurLine->EditLine.SetEOL(EndSeq);
    /* skv $*/
  }
  else
  {
    NewString->EditLine.SetString("");
    AddUndoData(NULL,NumLine+1,0,UNDO_INSSTR);
  }

  if (VBlockStart!=NULL && NumLine<VBlockY+VBlockSizeY)
    if (NumLine<VBlockY)
    {
      VBlockY++;
      BlockStartLine++;
    }
    else
      VBlockSizeY++;

  if (SelStart!=-1 && (SelEnd==-1 || CurPos<SelEnd))
  {
    if (CurPos>=SelStart)
    {
      CurLine->EditLine.Select(SelStart,-1);
      NewString->EditLine.Select(0,SelEnd==-1 ? -1:SelEnd-CurPos);
    }
    else
    {
      CurLine->EditLine.Select(-1,0);
      NewString->EditLine.Select(SelStart-CurPos,SelEnd==-1 ? -1:SelEnd-CurPos);
      BlockStart=NewString;
      BlockStartLine++;
    }
  }
  else
    if (BlockStart!=NULL && NumLine<BlockStartLine)
      BlockStartLine++;

  NewString->EditLine.SetEOL(EndSeq);

  CurLine->EditLine.SetCurPos(0);
  if (CurLine==EndList)
    EndList=NewString;
  NumLastLine++;
  Down();

  if (IndentPos>0)
  {
    int OrgIndentPos=IndentPos;
    ShowEditor(FALSE);

    CurLine->EditLine.GetBinaryString(CurLineStr,NULL,Length);

    if (SpaceOnly)
    {
      int Decrement=0;
      for (int I=0;I<IndentPos && I<Length;I++)
      {
        /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
        if (!IsSpace(CurLineStr[I]))
        /* IS $ */
          break;
        if (CurLineStr[I]==' ')
          Decrement++;
        else
        {
          int TabPos=CurLine->EditLine.RealPosToTab(I);
          Decrement+=EdOpt.TabSize - (TabPos % EdOpt.TabSize);
        }
      }
      IndentPos-=Decrement;
    }

    if (IndentPos>0)
    {
      if (CurLine->EditLine.GetLength()!=0 || !EdOpt.CursorBeyondEOL)
      {
        CurLine->EditLine.ProcessKey(KEY_HOME);

        int SaveOvertypeMode=CurLine->EditLine.GetOvertypeMode();
        CurLine->EditLine.SetOvertypeMode(FALSE);

        const char *PrevStr;
        int PrevLength;

        if (SrcIndent)
        {
          SrcIndent->EditLine.GetBinaryString(PrevStr,NULL,PrevLength);
        }

        for (int I=0;CurLine->EditLine.GetTabCurPos()<IndentPos;I++)
        {
          if (SrcIndent!=NULL && I<PrevLength && IsSpace(PrevStr[I]))
          {
            CurLine->EditLine.ProcessKey(PrevStr[I]);
          }
          else
          {
            CurLine->EditLine.ProcessKey(KEY_SPACE);
          }
        }
        while (CurLine->EditLine.GetTabCurPos()>IndentPos)
          CurLine->EditLine.ProcessKey(KEY_BS);

        CurLine->EditLine.SetOvertypeMode(SaveOvertypeMode);
      }
      CurLine->EditLine.SetTabCurPos(IndentPos);
    }

    CurLine->EditLine.GetBinaryString(CurLineStr,NULL,Length);
    CurPos=CurLine->EditLine.GetCurPos();
    if (SpaceOnly)
    {
      int NewPos=0;
      for (int I=0;I<Length;I++)
      {
        NewPos=I;
        /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
        if (!IsSpace(CurLineStr[I]))
        /* IS $ */
          break;
      }
      if (NewPos>OrgIndentPos)
        NewPos=OrgIndentPos;
      if (NewPos>CurPos)
        CurLine->EditLine.SetCurPos(NewPos);
    }
  }
  /*$ 10.08.2000 skv
    Modified->TextChanged
  */
  TextChanged(1);
  /* skv$*/

}



void Editor::Down()
{
  struct EditList *CurPtr;
  int LeftPos,CurPos,Y;
  if (CurLine->Next==NULL)
    return;
  for (Y=0,CurPtr=TopScreen;CurPtr!=CurLine;CurPtr=CurPtr->Next)
    Y++;
  if (Y>=Y2-Y1-1)
    TopScreen=TopScreen->Next;
  CurPos=CurLine->EditLine.GetTabCurPos();
  LeftPos=CurLine->EditLine.GetLeftPos();
  CurLine=CurLine->Next;
  NumLine++;
  CurLine->EditLine.SetLeftPos(LeftPos);
  CurLine->EditLine.SetTabCurPos(CurPos);
}


void Editor::ScrollDown()
{
  int LeftPos,CurPos;
  if (CurLine->Next==NULL || TopScreen->Next==NULL)
    return;
  if (CalcDistance(TopScreen,EndList,Y2-Y1)<Y2-Y1)
  {
    Down();
    return;
  }
  TopScreen=TopScreen->Next;
  CurPos=CurLine->EditLine.GetTabCurPos();
  LeftPos=CurLine->EditLine.GetLeftPos();
  CurLine=CurLine->Next;
  NumLine++;
  CurLine->EditLine.SetLeftPos(LeftPos);
  CurLine->EditLine.SetTabCurPos(CurPos);
}


void Editor::Up()
{
  int LeftPos,CurPos;
  if (CurLine->Prev==NULL)
    return;

  if (CurLine==TopScreen)
    TopScreen=TopScreen->Prev;

  CurPos=CurLine->EditLine.GetTabCurPos();
  LeftPos=CurLine->EditLine.GetLeftPos();
  CurLine=CurLine->Prev;
  NumLine--;
  CurLine->EditLine.SetLeftPos(LeftPos);
  CurLine->EditLine.SetTabCurPos(CurPos);
}


void Editor::ScrollUp()
{
  int LeftPos,CurPos;
  if (CurLine->Prev==NULL)
    return;
  if (TopScreen->Prev==NULL)
  {
    Up();
    return;
  }

  TopScreen=TopScreen->Prev;
  CurPos=CurLine->EditLine.GetTabCurPos();
  LeftPos=CurLine->EditLine.GetLeftPos();
  CurLine=CurLine->Prev;
  NumLine--;
  CurLine->EditLine.SetLeftPos(LeftPos);
  CurLine->EditLine.SetTabCurPos(CurPos);
}

/* $ 21.01.2001 SVS
   ������� ������/������ ������� �� Editor::Search
   � ��������� ������� GetSearchReplaceString
   (���� stddlg.cpp)
*/
BOOL Editor::Search(int Next)
{
  struct EditList *CurPtr,*TmpPtr;
  unsigned char SearchStr[SEARCHSTRINGBUFSIZE],ReplaceStr[SEARCHSTRINGBUFSIZE];
  static char LastReplaceStr[SEARCHSTRINGBUFSIZE];
  static int LastSuccessfulReplaceMode=0;
  char MsgStr[512];
  const char *TextHistoryName="SearchText",*ReplaceHistoryName="ReplaceText";
  /* $ 03.08.2000 KM
     ����� ����������
  */
  int CurPos,Count,Case,WholeWords,ReverseSearch,Match,NewNumLine,UserBreak;
  /* KM $ */
  if (Next && *LastSearchStr==0)
    return TRUE;

  strncpy((char *)SearchStr,(char *)LastSearchStr,sizeof(SearchStr)-1);
  strncpy((char *)ReplaceStr,(char *)LastReplaceStr,sizeof(ReplaceStr)-1);
  Case=LastSearchCase;
  WholeWords=LastSearchWholeWords;
  ReverseSearch=LastSearchReverse;

  if (!Next)
    if(!GetSearchReplaceString(ReplaceMode,SearchStr,sizeof(SearchStr),
                   ReplaceStr,sizeof(ReplaceStr),
                   TextHistoryName,ReplaceHistoryName,
                   &Case,&WholeWords,&ReverseSearch))
      return FALSE;

  strncpy((char *)LastSearchStr,(char *)SearchStr,sizeof(LastSearchStr)-1);
  strncpy((char *)LastReplaceStr,(char *)ReplaceStr,sizeof(LastReplaceStr)-1);
  LastSearchCase=Case;
  LastSearchWholeWords=WholeWords;
  LastSearchReverse=ReverseSearch;

  if (*SearchStr==0)
    return TRUE;

  LastSuccessfulReplaceMode=ReplaceMode;

  if (!EdOpt.PersistentBlocks)
    UnmarkBlock();

  {
    //SaveScreen SaveScr;

    int SearchLength=strlen((char *)SearchStr);

    sprintf(MsgStr,"\"%s\"",SearchStr);
    SetCursorType(FALSE,0);
    //SetPreRedrawFunc(Editor::PR_EditorShowMsg);
    EditorShowMsg(MSG(MEditSearchTitle),MSG(MEditSearchingFor),MsgStr);

    Count=0;
    Match=0;
    UserBreak=0;
    CurPos=CurLine->EditLine.GetCurPos();
    /* $ 16.10.2000 tran
       CurPos ������������� ��� ��������� ������ */
    /* $ 28.11.2000 SVS
       "�, ��� �� ������ - ��� �������� ���� ���������" :-)
       ����� ��������� ����� ��������������
    */
    /* $ 21.12.2000 SVS
       - � ���������� ����������� ���� ������ �������� ������� ���
         ������� EditorF7Rules
    */
    /* $ 10.06.2001 IS
       - ���: �����-�� ��� ����������� _���������_ ������ �������������� �� ���
         _������_.
    */
    /* $ 09.11.2001 IS
         ��������� �����, ����.
         ����� ������, �.�. �� ������������� �����������
    */
    if( !ReverseSearch && ( Next || (Opt.EditorF7Rules && !ReplaceMode) ) )
        CurPos++;
    /* IS $ */
    /* IS $ */
    /* SVS $ */
    /* SVS $ */
    /* tran $ */

    NewNumLine=NumLine;
    CurPtr=CurLine;

    while (CurPtr!=NULL)
    {
      if ((++Count & 0xfff)==0 && CheckForEsc())
      {
        UserBreak=TRUE;
        break;
      }
      /* $ 03.08.2000 KM
         ���������� ������ ��������� � ������� ������
      */
      if (CurPtr->EditLine.Search((char *)SearchStr,CurPos,Case,WholeWords,ReverseSearch))
      /* KM $ */
      {
        int Skip=FALSE;
        /* $ 24.01.2003 KM
           ! �� ��������� ������ �������� �� ����� ������ ��
             ����� ������������ ������.
        */
        /* $ 15.04.2003 VVM
           �������� �� �������� � �������� �� ���������� �������� ������ */
        int FromTop=(ScrY-2)/4;
        if (FromTop<0 || FromTop>=((ScrY-5)/2-2))
          FromTop=0;
        /* VVM $ */

        TmpPtr=CurLine=CurPtr;
        for (int i=0;i<FromTop;i++)
        {
          if (TmpPtr->Prev)
            TmpPtr=TmpPtr->Prev;
          else
            break;
        }
        TopScreen=TmpPtr;
        /* KM $ */

        NumLine=NewNumLine;

        int LeftPos=CurPtr->EditLine.GetLeftPos();
        int TabCurPos=CurPtr->EditLine.GetTabCurPos();
        if (ObjWidth>8 && TabCurPos-LeftPos+SearchLength>ObjWidth-8)
          CurPtr->EditLine.SetLeftPos(TabCurPos+SearchLength-ObjWidth+8);

        if (ReplaceMode)
        {
          int MsgCode=0;
          if (!ReplaceAll)
          {
            Show();
            int CurX,CurY;
            GetCursorPos(CurX,CurY);
            GotoXY(CurX,CurY);
            SetColor(COL_EDITORSELECTEDTEXT);
            const char *Str=CurPtr->EditLine.GetStringAddr()+CurPtr->EditLine.GetCurPos();
            char *TmpStr=new char[SearchLength+1];
            strncpy(TmpStr,Str,SearchLength);
            TmpStr[SearchLength]=0;
            if (UseDecodeTable)
              DecodeString(TmpStr,(unsigned char *)TableSet.DecodeTable);
            Text(TmpStr);
            delete[] TmpStr;

            char QSearchStr[SEARCHSTRINGBUFSIZE+4],QReplaceStr[SEARCHSTRINGBUFSIZE+4];
            sprintf(QSearchStr,"\"%s\"",LastSearchStr);
            sprintf(QReplaceStr,"\"%s\"",LastReplaceStr);
            MsgCode=Message(0,4,MSG(MEditReplaceTitle),MSG(MEditAskReplace),
              QSearchStr,MSG(MEditAskReplaceWith),QReplaceStr,
              MSG(MEditReplace),MSG(MEditReplaceAll),MSG(MEditSkip),MSG(MEditCancel));
            if (MsgCode==1)
              ReplaceAll=TRUE;
            if (MsgCode==2)
              Skip=TRUE;
            if (MsgCode<0 || MsgCode==3)
            {
              UserBreak=TRUE;
              break;
            }
          }
          if (MsgCode==0 || MsgCode==1)
          {
            Pasting++;
            /*$ 15.08.2000 skv
              If Replace string doesn't contain control symbols (tab and return),
              processed with fast method, otherwise use improved old one.
            */
            if(strchr((char*)ReplaceStr,'\t') || strchr((char*)ReplaceStr,13))
            {
              int SaveOvertypeMode=Flags.Check(FEDITOR_OVERTYPE);
              Flags.Set(FEDITOR_OVERTYPE);
              CurLine->EditLine.SetOvertypeMode(TRUE);
              //int CurPos=CurLine->EditLine.GetCurPos();
              int I;
              for (I=0;SearchStr[I]!=0 && ReplaceStr[I]!=0;I++)
              {
                int Ch=ReplaceStr[I];
                if (Ch==KEY_TAB)
                {
                  Flags.Clear(FEDITOR_OVERTYPE);
                  CurLine->EditLine.SetOvertypeMode(FALSE);
                  ProcessKey(KEY_DEL);
                  ProcessKey(KEY_TAB);
                  Flags.Set(FEDITOR_OVERTYPE);
                  CurLine->EditLine.SetOvertypeMode(TRUE);
                  continue;
                }
                /* $ 24.05.2002 SKV
                  ���� ��������� �� Enter, �� overtype �� �����.
                  ����� ������� ������� ��, ��� ��������.
                */
                if(Ch==0x0d)
                {
                  ProcessKey(KEY_DEL);
                }
                /* SKV $ */
                if (Ch!=KEY_BS && Ch!=KEY_DEL)
                  ProcessKey(Ch);
              }
              if(SearchStr[I]==0)
              {
                Flags.Clear(FEDITOR_OVERTYPE);
                CurLine->EditLine.SetOvertypeMode(FALSE);
                for (;ReplaceStr[I]!=0;I++)
                {
                  int Ch=ReplaceStr[I];
                  if (Ch!=KEY_BS && Ch!=KEY_DEL)
                    ProcessKey(Ch);
                }
              }else
              {
                for (;SearchStr[I]!=0;I++)
                {
                  ProcessKey(KEY_DEL);
                }
              }
              int Cnt=0;
              char *Tmp=(char*)ReplaceStr;
              while((Tmp=strchr(Tmp,13)) != NULL)
              {
                Cnt++;
                Tmp++;
              }
              if(Cnt>0)
              {
                CurPtr=CurLine;
                NewNumLine+=Cnt;
              }
              Flags.Change(FEDITOR_OVERTYPE,SaveOvertypeMode);
            }
            else
            {
              /* Fast method */
              const char *Str,*Eol;
              int StrLen,NewStrLen;
              int SStrLen=strlen((char*)SearchStr),
                  RStrLen=strlen((char*)ReplaceStr);
              CurLine->EditLine.GetBinaryString(Str,&Eol,StrLen);
              int EolLen=strlen((char*)Eol);
              NewStrLen=StrLen;
              NewStrLen-=SStrLen;
              NewStrLen+=RStrLen;
              NewStrLen+=EolLen;
              char *NewStr=new char[NewStrLen+1];
              int CurPos=CurLine->EditLine.GetCurPos();
              memcpy(NewStr,Str,CurPos);
              memcpy(NewStr+CurPos,ReplaceStr,RStrLen);
              /*$ 07.09.2000 skv
                If table set need to encode string.
              */
              if(UseDecodeTable)
              {
                EncodeString(NewStr+CurPos,(unsigned char*)TableSet.EncodeTable,RStrLen);
              }
              /* skv$*/
              memcpy(NewStr+CurPos+RStrLen,Str+CurPos+SStrLen,StrLen-CurPos-SStrLen);
              memcpy(NewStr+NewStrLen-EolLen,Eol,EolLen);
              AddUndoData(CurLine->EditLine.GetStringAddr(),NumLine,
                          CurLine->EditLine.GetCurPos(),UNDO_EDIT);
              CurLine->EditLine.SetBinaryString(NewStr,NewStrLen);
              CurLine->EditLine.SetCurPos(CurPos+RStrLen);
              delete [] NewStr;

              TextChanged(1);
            }
            /* skv$*/

            if (ReverseSearch)
              CurLine->EditLine.SetCurPos(CurPos);
            Pasting--;
          }
        }
        Match=1;
        if (!ReplaceMode)
          break;
        CurPos=CurLine->EditLine.GetCurPos();
        if (Skip)
          if (!ReverseSearch)
            CurPos++;
      }
      else
        if (ReverseSearch)
        {
          CurPtr=CurPtr->Prev;
          if (CurPtr==NULL)
            break;
          CurPos=CurPtr->EditLine.GetLength();
          NewNumLine--;
        }
        else
        {
          CurPos=0;
          CurPtr=CurPtr->Next;
          NewNumLine++;
        }
    }
    //SetPreRedrawFunc(NULL);
  }
  Show();
  if (!Match && !UserBreak)
    Message(MSG_DOWN|MSG_WARNING,1,MSG(MEditSearchTitle),MSG(MEditNotFound),
            MsgStr,MSG(MOk));
   return TRUE;
}
/* SVS $ */

void Editor::Paste(char *Src)
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;

  char *ClipText=Src;
  BOOL IsDeleteClipText=FALSE;

  if(!ClipText)
  {
    if ((ClipText=PasteFormatFromClipboard(FAR_VerticalBlock))!=NULL)
    {
      VPaste(ClipText);
      return;
    }
    if ((ClipText=PasteFromClipboard())==NULL)
      return;
    IsDeleteClipText=TRUE;
  }

  if (*ClipText)
  {
    Flags.Set(FEDITOR_NEWUNDO);
    if (UseDecodeTable)
      EncodeString(ClipText,(unsigned char *)TableSet.EncodeTable);
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/
    int SaveOvertype=Flags.Check(FEDITOR_OVERTYPE);
    UnmarkBlock();
    Pasting++;
    DisableOut++;
    Edit::DisableEditOut(TRUE);
    if (Flags.Check(FEDITOR_OVERTYPE))
    {
      Flags.Clear(FEDITOR_OVERTYPE);
      CurLine->EditLine.SetOvertypeMode(FALSE);
    }
    BlockStart=CurLine;
    BlockStartLine=NumLine;
    /* $ 19.05.2001 IS
       ������� �������� ���������� ����������� ��������� (������� ������ ����
       ��������� � ������ ������ ��� �����������) � �������.
    */
    int StartPos=CurLine->EditLine.GetCurPos(),
        oldAutoIndent=EdOpt.AutoIndent;

    for (int I=0;ClipText[I]!=0;)
      if (ClipText[I]!=10)
        if (ClipText[I]==13)
        {
          CurLine->EditLine.Select(StartPos,-1);
          StartPos=0;
          EdOpt.AutoIndent=FALSE;
          ProcessKey(KEY_ENTER);
          BlockUndo=TRUE;
          I++;
        }
        else
        {
          if(EdOpt.AutoIndent)       // ������ ������ ������� ���, �����
          {                          // �������� ����������
            /* $ 25.05.2001 IS
                 ��������� ���������� ������� � ���� � ���������,
                 �������� �� oem
            */
            ProcessKey(UseDecodeTable?TableSet.DecodeTable[(unsigned)ClipText[I]]:ClipText[I]);
            /* IS $ */
            I++;
            StartPos=CurLine->EditLine.GetCurPos();
            if(StartPos) StartPos--;
          }

          int Pos=I;
          while (ClipText[Pos]!=0 && ClipText[Pos]!=10 && ClipText[Pos]!=13)
            Pos++;
          if (Pos>I)
          {
            const char *Str;
            int Length,CurPos;
            CurLine->EditLine.GetBinaryString(Str,NULL,Length);
            CurPos=CurLine->EditLine.GetCurPos();
            AddUndoData(Str,NumLine,CurPos,UNDO_EDIT);
            BlockUndo=TRUE;
            CurLine->EditLine.InsertBinaryString(&ClipText[I],Pos-I);
          }
          I=Pos;
        }
      else
        I++;

    EdOpt.AutoIndent=oldAutoIndent;

    CurLine->EditLine.Select(StartPos,CurLine->EditLine.GetCurPos());
    /* IS $ */

    if (SaveOvertype)
    {
      Flags.Set(FEDITOR_OVERTYPE);
      CurLine->EditLine.SetOvertypeMode(TRUE);
    }

    Edit::DisableEditOut(FALSE);
    Pasting--;
    DisableOut--;
  }
  /* $ 07.05.2001 IS �������� �� � PasteFromClipboard ��� new [] */
  if(IsDeleteClipText)
    delete [] ClipText;
  /* IS $ */
  BlockUndo=FALSE;
}


void Editor::Copy(int Append)
{
  if (VBlockStart!=NULL)
  {
    VCopy(Append);
    return;
  }

  struct EditList *CurPtr=BlockStart;
  char *CopyData=NULL;
  long DataSize=0,PrevSize=0;

  if (Append)
  {
    CopyData=PasteFromClipboard();
    if (CopyData!=NULL)
      PrevSize=DataSize=strlen(CopyData);
  }

  while (CurPtr!=NULL)
  {
    int StartSel,EndSel;
    int Length=CurPtr->EditLine.GetLength()+1;
    CurPtr->EditLine.GetSelection(StartSel,EndSel);
    if (StartSel==-1)
      break;
    char *NewPtr=(char *)xf_realloc(CopyData,DataSize+Length+2);
    if (NewPtr==NULL)
    {
      delete CopyData;
      CopyData=NULL;
      break;
    }
    CopyData=NewPtr;
    CurPtr->EditLine.GetSelString(CopyData+DataSize,Length);
    DataSize+=strlen(CopyData+DataSize);
    if (EndSel==-1)
    {
      strcpy(CopyData+DataSize,DOS_EOL_fmt);
      DataSize+=2;
    }
    CurPtr=CurPtr->Next;
  }

  if (CopyData!=NULL)
  {
    if (UseDecodeTable)
      DecodeString(CopyData+PrevSize,(unsigned char *)TableSet.DecodeTable);
    CopyToClipboard(CopyData);
    delete CopyData;
  }
}


void Editor::DeleteBlock()
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;

  if (VBlockStart!=NULL)
  {
    DeleteVBlock();
    return;
  }

  struct EditList *CurPtr=BlockStart;

  int UndoNext=FALSE;

  while (CurPtr!=NULL)
  {
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/
    int StartSel,EndSel;
    /* $ 17.09.2002 SKV
      ������ �� Real ��� � ������ ��������� �� ������ ������.
    */
    CurPtr->EditLine.GetRealSelection(StartSel,EndSel);
    if(EndSel!=-1 && EndSel>CurPtr->EditLine.GetLength())
      EndSel=-1;
    /* SKV $ */
    if (StartSel==-1)
      break;
    if (StartSel==0 && EndSel==-1)
    {
      struct EditList *NextLine=CurPtr->Next;
      BlockUndo=UndoNext;
      DeleteString(CurPtr,FALSE,BlockStartLine);
      UndoNext=TRUE;
      if (BlockStartLine<NumLine)
        NumLine--;
      if (NextLine!=NULL)
      {
        CurPtr=NextLine;
        continue;
      }
      else
        break;
    }
    int Length=CurPtr->EditLine.GetLength();
    if (StartSel!=0 || EndSel!=0)
    {
      BlockUndo=UndoNext;
      AddUndoData(CurPtr->EditLine.GetStringAddr(),BlockStartLine,
                  CurPtr->EditLine.GetCurPos(),UNDO_EDIT);
      UndoNext=TRUE;
    }
    /* $ 17.09.2002 SKV
      ����� ��� ��������� �� ������ ������.
      InsertBinaryString ������� trailing space'��
    */
    if(StartSel>Length)
    {
      Length=StartSel;
      CurPtr->EditLine.SetCurPos(Length);
      CurPtr->EditLine.InsertBinaryString("",0);
    }
    /* SKV $ */
    const char *CurStr,*EndSeq;
    CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);
    // ������ ����� realloc, ������� ��� malloc.
    char *TmpStr=(char*)xf_malloc(Length+3);
    memcpy(TmpStr,CurStr,Length);
    TmpStr[Length]=0;
    int DeleteNext=FALSE;
    if (EndSel==-1)
    {
      EndSel=Length;
      if (CurPtr->Next!=NULL)
        DeleteNext=TRUE;
    }
    memmove(TmpStr+StartSel,TmpStr+EndSel,strlen(TmpStr+EndSel)+1);
    int CurPos=StartSel;
/*    if (CurPos>=StartSel)
    {
      CurPos-=(EndSel-StartSel);
      if (CurPos<StartSel)
        CurPos=StartSel;
    }
*/
    Length-=EndSel-StartSel;
    if (DeleteNext)
    {
      const char *NextStr,*EndSeq;
      int NextLength,NextStartSel,NextEndSel;
      CurPtr->Next->EditLine.GetSelection(NextStartSel,NextEndSel);
      if (NextStartSel==-1)
        NextEndSel=0;
      if (NextEndSel==-1)
        EndSel=-1;
      else
      {
        CurPtr->Next->EditLine.GetBinaryString(NextStr,&EndSeq,NextLength);
        NextLength-=NextEndSel;
        TmpStr=(char *)xf_realloc(TmpStr,Length+NextLength+3);
        memcpy(TmpStr+Length,NextStr+NextEndSel,NextLength);
        Length+=NextLength;
      }
      if (CurLine==CurPtr->Next)
      {
        CurLine=CurPtr;
        NumLine--;
      }

      BlockUndo=UndoNext;
      if (CurLine==CurPtr && CurPtr->Next!=NULL && CurPtr->Next==TopScreen)
      {
        CurLine=CurPtr->Next;
        NumLine++;
      }
      DeleteString(CurPtr->Next,FALSE,BlockStartLine+1);
      UndoNext=TRUE;
      if (BlockStartLine+1<NumLine)
        NumLine--;
    }
    int EndLength=strlen(EndSeq);
    memcpy(TmpStr+Length,EndSeq,EndLength);
    Length+=EndLength;
    TmpStr[Length]=0;
    CurPtr->EditLine.SetBinaryString(TmpStr,Length);
    /* $ 17.09.2002 SKV
      �������� ����� malloc
    */
    xf_free(TmpStr);
    /* SKV $ */
    CurPtr->EditLine.SetCurPos(CurPos);
    if (DeleteNext && EndSel==-1)
    {
      CurPtr->EditLine.Select(CurPtr->EditLine.GetLength(),-1);
    }
    else
    {
      CurPtr->EditLine.Select(-1,0);
      CurPtr=CurPtr->Next;
      BlockStartLine++;
    }
  }
  BlockStart=NULL;
  BlockUndo=FALSE;
}


void Editor::UnmarkBlock()
{
  if (BlockStart==NULL && VBlockStart==NULL)
    return;
  VBlockStart=NULL;
  _SVS(SysLog("[%d] Editor::UnmarkBlock()",__LINE__));
  Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
  while (BlockStart!=NULL)
  {
    int StartSel,EndSel;
    BlockStart->EditLine.GetSelection(StartSel,EndSel);
    if (StartSel==-1)
    {
      /* $ 24.06.2002 SKV
        ���� � ������� ������ ��� ���������,
        ��� ��� �� ������ ��� �� � �����.
        ��� ����� ���� ������ ������ :)
      */
      if(BlockStart->Next)
      {
        BlockStart->Next->EditLine.GetSelection(StartSel,EndSel);
        if(StartSel==-1)
        {
          break;
        }
      }else break;
      /* SKV $ */
    }
    BlockStart->EditLine.Select(-1,0);
    BlockStart=BlockStart->Next;
  }
  BlockStart=NULL;
  Show();
}

/* $ 10.03.2002 IS
   ! ������ ��������� �������� � ������������� �������
*/
/* $ 07.03.2002 IS
   ������� ���������, ���� ��� ������ (�������� ���� �������� � ������)
*/
void Editor::UnmarkEmptyBlock()
{
  _SVS(SysLog("[%d] Editor::UnmarkEmptyBlock()",__LINE__));
  if(BlockStart || VBlockStart)  // ������������ ���������
  {
    int Lines=0,StartSel,EndSel;
    struct EditList *Block=BlockStart;
    if(VBlockStart)
    {
      if(VBlockSizeX)
        Lines=VBlockSizeY;
    }
    else while(Block) // ��������� �� ���� ���������� �������
    {
      Block->EditLine.GetRealSelection(StartSel,EndSel);
      if (StartSel==-1)
        break;
      if(StartSel!=EndSel)// �������� �������-�� ��������
      {
        ++Lines;           // �������� ������� �������� �����
        break;
      }
      Block=Block->Next;
    }
    if(!Lines)             // ���� �������� ���� �������� � ������, ��
      UnmarkBlock();       // ���������� �������� ������ � ������ ���������
  }
}
/* IS 07.03.2002 $ */
/* IS 10.03.2002 $ */

/* $ 07.07.2000 tran & SVS
   + ��������� ����������� ���������� �� �������
     �� ������� [!][ROW][,COL]
     �������� ��� �������� ��� ������������� �������� � void �� int
     �� �������� ������� ���������� � �����
     '!' - ������ ������������� �������� (���� �� ����������� ;-)
*/
/* $ 21.07.2000 tran
   GotoLine ����� ���� � �� ������� �������� */
void Editor::GoToLine(int Line)
{
  int NewLine;

  NewLine=Line;

  int LastNumLine=NumLine;
  int CurScrLine=CalcDistance(TopScreen,CurLine,-1);
  for (NumLine=0,CurLine=TopList;
         NumLine<NewLine && CurLine->Next!=NULL;
         NumLine++)
    CurLine=CurLine->Next;
  CurScrLine+=NumLine-LastNumLine;

  if (CurScrLine<0 || CurScrLine>=Y2-Y1)
    TopScreen=CurLine;

// <GOTO_UNMARK:2>
//  if (!EdOpt.PersistentBlocks)
//     UnmarkBlock();
// </GOTO_UNMARK>

  Show();
  return ;
}
/* tran 21.07.2000 $ */

/* $ 07.07.2000 tran & SVS
   + ��������� ����������� ���������� �� �������
     �� ������� [!][ROW][,COL]
     �������� ��� �������� ��� ������������� �������� � void �� int
     �� �������� ������� ���������� � �����
     '!' - ������ ������������� �������� (���� �� ����������� ;-)
*/
/* $ 21.07.2000 tran
   ������ �� GotoLine ����������� ���� */
void Editor::GoToPosition()
{
  int NewLine, NewCol;
  int LeftPos=CurLine->EditLine.GetTabCurPos()+1;
  int CurPos;
  CurPos=CurLine->EditLine.GetCurPos();

  const char *LineHistoryName="LineNumber";
  static struct DialogData GoToDlgData[]=
  {
    DI_DOUBLEBOX,3,1,21,3,0,0,0,0,(char *)MEditGoToLine,
    DI_EDIT,5,2,19,2,1,(DWORD)LineHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,1,"",
  };
  MakeDialogItems(GoToDlgData,GoToDlg);
  /* $ 01.08.2000 tran
    PrevLine ������ �� ����� - USELASTHISTORY ����� */
  //  static char PrevLine[40]={0};

  //  strcpy(GoToDlg[1].Data,PrevLine);
  Dialog Dlg(GoToDlg,sizeof(GoToDlg)/sizeof(GoToDlg[0]));
  Dlg.SetPosition(-1,-1,25,5);
  Dlg.SetHelp("EditorGotoPos");
  Dlg.Process();

  /* $ 06.05.2002 KM
      ������� ShadowSaveScr ��� �������������� ���������
      �����������.
  */
  Dialog::SendDlgMessage((HANDLE)&Dlg,DM_KILLSAVESCREEN,0,0);
  /* KM $ */

    // tran: was if (Dlg.GetExitCode()!=1 || !isdigit(*GoToDlg[1].Data))
  if (Dlg.GetExitCode()!=1 )
      return ;
  // �������� ����� ��������� �������� � ������� ������ ������ FAR`�
  //  strncpy(PrevLine,GoToDlg[1].Data,sizeof(PrevLine));

  GetRowCol(GoToDlg[1].Data,&NewLine,&NewCol);

  //_D(SysLog("GoToPosition: NewLine=%i, NewCol=%i",NewLine,NewCol));
  GoToLine(NewLine);

  if ( NewCol == -1)
  {
    CurLine->EditLine.SetTabCurPos(CurPos);
    CurLine->EditLine.SetLeftPos(LeftPos);
  }
  else
    CurLine->EditLine.SetTabCurPos(NewCol);

// <GOTO_UNMARK:3>
//  if (!EdOpt.PersistentBlocks)
//     UnmarkBlock();
// </GOTO_UNMARK>

  Show();
  return ;
}
/* tran 07.07.2000 $ */
/* tran 21.07.2000 $ */


/* $ 07.07.2000 tran & SVS
   function for AltF8 user answer parsing
   ����������:
      TRUE  - ���������� ��������
      FALSE - �������������
*/
/* $ 21.07.2000 tran
   ������ ������ �� ����������
   ������ ���� ���������� ���������������
   � ��������� ����� ���������� */
void Editor::GetRowCol(char *argv,int *row,int *col)
{
  int x=0xffff,y,l;
  char *argvx=0;
  int LeftPos=CurLine->EditLine.GetTabCurPos() + 1;

  // ��� �� �� �������� "�����" ������ - ������ ��, ��� �� ����� ;-)
  // "�������" ��� ������� �������.
  RemoveExternalSpaces(argv);

  // �������� ������ ��������� ������ �����������
  // � ������� ������
  l=strcspn(argv,",:;. ");
  // ���� ����������� ����, �� l=strlen(argv)

  if(l < strlen(argv)) // ��������: "row,col" ��� ",col"?
  {
    argv[l]='\0'; // ������ ����������� ��������� "����� ������" :-)
    argvx=argv+l+1;
    x=atoi(argvx);
  }
  y=atoi(argv);
  /* $ 14.07.2000 tran
    + ������� �� �������� */
  if ( strchr(argv,'%')!=0 )
    y=NumLastLine * y / 100;
  /* tran $ */

  /* $ 21.07.2000 tran
     ��������� ��������������� */
  if ( argv[0]=='-' || argv[0]=='+' )
    y=NumLine+y+1;
  if ( argvx )
  {
    if ( argvx[0]=='-' || argvx[0]=='+' )
    {
        x=LeftPos+x;
    }
  }

  /* tran 21.07.2000 $ */

  // ������ ������� ��������� �����
  *row=y;
  if ( x!=0xffff )
    *col=x;
  else
    /* $ 28.03.2001 VVM
      ! �����-�� ���������� 1. � ���������� ������... */
    *col=LeftPos;
    /* VVM $ */


  (*row)--;
  if (*row< 0)   // ���� ����� ",Col"
     *row=NumLine;  //   �� ��������� �� ������� ������ � �������
  (*col)--;
  if (*col< -1)
     *col=-1;
  return ;
}
/* tran 07.07.2000 $ */

/* $ 03.12.2001 IS
   UndoData - ������ ���������
*/
void Editor::AddUndoData(const char *Str,int StrNum,int StrPos,int Type)
{
  int PrevUndoDataPos;
  if (Flags.Check(FEDITOR_DISABLEUNDO) || !UndoData)
    return;
  if (StrNum==-1)
    StrNum=NumLine;
  if ((PrevUndoDataPos=UndoDataPos-1)<0)
    PrevUndoDataPos=Opt.EditorUndoSize-1;
  if (!Flags.Check(FEDITOR_NEWUNDO) && Type==UNDO_EDIT &&
      UndoData[PrevUndoDataPos].Type==UNDO_EDIT &&
      StrNum==UndoData[PrevUndoDataPos].StrNum &&
      (abs(StrPos-UndoData[PrevUndoDataPos].StrPos)<=1 ||
      abs(StrPos-LastChangeStrPos)<=1))
  {
    LastChangeStrPos=StrPos;
    return;
  }
  Flags.Clear(FEDITOR_NEWUNDO);
  if (UndoData[UndoDataPos].Type!=UNDO_NONE && UndoData[UndoDataPos].Str!=NULL)
    delete[] UndoData[UndoDataPos].Str;
  UndoData[UndoDataPos].Type=Type;
  UndoData[UndoDataPos].UndoNext=BlockUndo;
  UndoData[UndoDataPos].StrPos=StrPos;
  UndoData[UndoDataPos].StrNum=StrNum;
  if (Str!=NULL)
  {
    UndoData[UndoDataPos].Str=new char[strlen(Str)+1];
    if (UndoData[UndoDataPos].Str!=NULL)
      strcpy(UndoData[UndoDataPos].Str,Str);
  }
  else
    UndoData[UndoDataPos].Str=NULL;
  if (++UndoDataPos==Opt.EditorUndoSize)
    UndoDataPos=0;
  if (UndoDataPos==UndoSavePos)
    Flags.Set(FEDITOR_UNDOOVERFLOW);
}
/* IS $ */

/* $ 03.12.2001 IS
   UndoData - ������ ���������
*/
void Editor::Undo()
{
  if(!UndoData)
    return;
  int NewPos=UndoDataPos-1;
  if (NewPos<0)
    NewPos=Opt.EditorUndoSize-1;
  if (UndoData[NewPos].Type==UNDO_NONE)
    return;
  UnmarkBlock();
  UndoDataPos=NewPos;
  /*$ 10.08.2000 skv
    Modified->TextChanged
  */
  TextChanged(1);
  /* skv $*/
  /* $ 30.03.2002 IS
     ������ ���� ������������ FEDITOR_WASCHANGED, �.�. ��� �����, ��� � �����,
     ������ ���������� ����� ��� ��������� ����� ��������������� _�� �����_, �
     �� ��������� � ����� �������� FEDITOR_MODIFIED. ��������� � �����
     ������������� � ������ �����, �� �� ��������������� ���������, ������ ���
     ��� ��������������� � "TextChanged(1)" - ��. ����.
  */
  Flags.Set(/*FEDITOR_WASCHANGED|*/FEDITOR_DISABLEUNDO);
  /* IS $ */
  GoToLine(UndoData[UndoDataPos].StrNum);
  switch(UndoData[UndoDataPos].Type)
  {
    case UNDO_INSSTR:
      DeleteString(CurLine,TRUE,NumLine>0 ? NumLine-1:NumLine);
      break;
    case UNDO_DELSTR:
      Pasting++;
      if (NumLine<UndoData[UndoDataPos].StrNum)
      {
        ProcessKey(KEY_END);
        ProcessKey(KEY_ENTER);
      }
      else
      {
        ProcessKey(KEY_HOME);
        ProcessKey(KEY_ENTER);
        ProcessKey(KEY_UP);
      }
      Pasting--;
      if (UndoData[UndoDataPos].Str!=NULL)
        CurLine->EditLine.SetString(UndoData[UndoDataPos].Str);
      break;
    case UNDO_EDIT:
      if (UndoData[UndoDataPos].Str!=NULL)
        CurLine->EditLine.SetString(UndoData[UndoDataPos].Str);
      CurLine->EditLine.SetCurPos(UndoData[UndoDataPos].StrPos);
      break;
  }
  if (UndoData[UndoDataPos].Str!=NULL)
    delete[] UndoData[UndoDataPos].Str;
  UndoData[UndoDataPos].Type=UNDO_NONE;
  if (UndoData[UndoDataPos].UndoNext)
    Undo();
  /*$ 10.08.2000 skv
    ! Modified->TextChanged
  */
  if (!Flags.Check(FEDITOR_UNDOOVERFLOW) && UndoDataPos==UndoSavePos)
    TextChanged(0);
  /* skv $*/
  Flags.Clear(FEDITOR_DISABLEUNDO);
}
/* IS $ */

void Editor::SelectAll()
{
  struct EditList *CurPtr;
  BlockStart=TopList;
  BlockStartLine=0;
  for (CurPtr=TopList;CurPtr!=NULL;CurPtr=CurPtr->Next)
    if (CurPtr->Next!=NULL)
      CurPtr->EditLine.Select(0,-1);
    else
      CurPtr->EditLine.Select(0,CurPtr->EditLine.GetLength());
  Show();
}


void Editor::SetStartPos(int LineNum,int CharNum)
{
  StartLine=LineNum==0 ? 1:LineNum;
  StartChar=CharNum==0 ? 1:CharNum;
}


int Editor::IsFileChanged()
{
  return(Flags.Check(FEDITOR_MODIFIED|FEDITOR_WASCHANGED));
}


int Editor::IsFileModified()
{
  return(Flags.Check(FEDITOR_MODIFIED));
}

// ������������ � FileEditor
long Editor::GetCurPos()
{
  struct EditList *CurPtr=TopList;
  long TotalSize=0;
  while (CurPtr!=TopScreen)
  {
    const char *SaveStr,*EndSeq;
    int Length;
    CurPtr->EditLine.GetBinaryString(SaveStr,&EndSeq,Length);
    TotalSize+=Length+strlen(EndSeq);
    CurPtr=CurPtr->Next;
  }
  return(TotalSize);
}


void Editor::SetStringsTable()
{
  struct EditList *CurPtr=TopList;
  while (CurPtr!=NULL)
  {
    CurPtr->EditLine.SetTables(UseDecodeTable ? &TableSet:NULL);
    CurPtr=CurPtr->Next;
  }
}


void Editor::BlockLeft()
{
  if (VBlockStart!=NULL)
  {
    VBlockShift(TRUE);
    return;
  }
  struct EditList *CurPtr=BlockStart;
  int LineNum=BlockStartLine;
/* $ 14.02.2001 VVM
    + ��� ���������� ����� AltU/AltI �������� ������� ������� */
  int MoveLine = 0;
  if (CurPtr==NULL)
  {
    MoveLine = 1;
    CurPtr = CurLine;
    LineNum = NumLine;
  }
/* VVM $ */
  while (CurPtr!=NULL)
  {
    int StartSel,EndSel;
    CurPtr->EditLine.GetSelection(StartSel,EndSel);
    /* $ 14.02.2001 VVM
      + ����� ��� - ������� ��� ������������ */
    if (MoveLine) {
      StartSel = 0; EndSel = -1;
    }
    /* VVM $ */
    if (StartSel==-1)
      break;

    int Length=CurPtr->EditLine.GetLength();
    char *TmpStr=new char[Length+EdOpt.TabSize+5];

    const char *CurStr,*EndSeq;
    CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);

    Length--;
    if (*CurStr==' ')
      memcpy(TmpStr,CurStr+1,Length);
    else
      if (*CurStr=='\t')
      {
        memset(TmpStr,' ',EdOpt.TabSize-1);
        memcpy(TmpStr+EdOpt.TabSize-1,CurStr+1,Length);
        Length+=EdOpt.TabSize-1;
      }

    /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
    if ((EndSel==-1 || EndSel>StartSel) && IsSpace(*CurStr))
    /* IS $ */
    {
      int EndLength=strlen(EndSeq);
      memcpy(TmpStr+Length,EndSeq,EndLength);
      Length+=EndLength;
      TmpStr[Length]=0;
      AddUndoData(CurStr,LineNum,0,UNDO_EDIT);
      BlockUndo=TRUE;
      int CurPos=CurPtr->EditLine.GetCurPos();
      CurPtr->EditLine.SetBinaryString(TmpStr,Length);
      CurPtr->EditLine.SetCurPos(CurPos>0 ? CurPos-1:CurPos);
      /* $ 14.02.2001 VVM
        + �������� ������ ���� ������� ���� */
      if (!MoveLine)
      /* VVM $ */
        CurPtr->EditLine.Select(StartSel>0 ? StartSel-1:StartSel,EndSel>0 ? EndSel-1:EndSel);
      /*$ 10.08.2000 skv
        Modified->TextChanged
      */
      TextChanged(1);
      /* skv $*/
    }

    delete[] TmpStr;
    CurPtr=CurPtr->Next;
    LineNum++;
    MoveLine = 0;
  }
  BlockUndo=FALSE;
}


void Editor::BlockRight()
{
  if (VBlockStart!=NULL)
  {
    VBlockShift(FALSE);
    return;
  }
  struct EditList *CurPtr=BlockStart;
  int LineNum=BlockStartLine;
/* $ 14.02.2001 VVM
    + ��� ���������� ����� AltU/AltI �������� ������� ������� */
  int MoveLine = 0;
  if (CurPtr==NULL)
  {
    MoveLine = 1;
    CurPtr = CurLine;
    LineNum = NumLine;
  }
/* VVM $ */
  while (CurPtr!=NULL)
  {
    int StartSel,EndSel;
    CurPtr->EditLine.GetSelection(StartSel,EndSel);
    /* $ 14.02.2001 VVM
      + ����� ��� - ������� ��� ������������ */
    if (MoveLine) {
      StartSel = 0; EndSel = -1;
    }
    /* VVM $ */
    if (StartSel==-1)
      break;

    int Length=CurPtr->EditLine.GetLength();
    char *TmpStr=new char[Length+5];

    const char *CurStr,*EndSeq;
    CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);
    *TmpStr=' ';
    memcpy(TmpStr+1,CurStr,Length);
    Length++;

    if (EndSel==-1 || EndSel>StartSel)
    {
      int EndLength=strlen(EndSeq);
      memcpy(TmpStr+Length,EndSeq,EndLength);
      TmpStr[Length+EndLength]=0;
      AddUndoData(CurStr,LineNum,0,UNDO_EDIT);
      BlockUndo=TRUE;
      int CurPos=CurPtr->EditLine.GetCurPos();
      if (Length>1)
        CurPtr->EditLine.SetBinaryString(TmpStr,Length+EndLength);
      CurPtr->EditLine.SetCurPos(CurPos+1);
      /* $ 14.02.2001 VVM
        + �������� ������ ���� ������� ���� */
      if (!MoveLine)
      /* VVM $ */
        CurPtr->EditLine.Select(StartSel>0 ? StartSel+1:StartSel,EndSel>0 ? EndSel+1:EndSel);
      /*$ 10.08.2000 skv
        Modified->TextChanged
      */
      TextChanged(1);
      /* skv $*/
    }

    delete[] TmpStr;
    CurPtr=CurPtr->Next;
    LineNum++;
    MoveLine = 0;
  }
  BlockUndo=FALSE;
}


void Editor::DeleteVBlock()
{
  if (Flags.Check(FEDITOR_LOCKMODE) || VBlockSizeX<=0 || VBlockSizeY<=0)
    return;

  int UndoNext=FALSE;

  if (!EdOpt.PersistentBlocks)
  {
    struct EditList *CurPtr=CurLine;
    struct EditList *NewTopScreen=TopScreen;
    while (CurPtr!=NULL)
    {
      if (CurPtr==VBlockStart)
      {
        TopScreen=NewTopScreen;
        CurLine=CurPtr;
        CurPtr->EditLine.SetTabCurPos(VBlockX);
        break;
      }
      NumLine--;
      if (NewTopScreen==CurPtr && CurPtr->Prev!=NULL)
        NewTopScreen=CurPtr->Prev;
      CurPtr=CurPtr->Prev;
    }
  }

  struct EditList *CurPtr=VBlockStart;

  for (int Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->Next)
  {
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/

    int TBlockX=CurPtr->EditLine.TabPosToReal(VBlockX);
    int TBlockSizeX=CurPtr->EditLine.TabPosToReal(VBlockX+VBlockSizeX)-
                    CurPtr->EditLine.TabPosToReal(VBlockX);

    const char *CurStr,*EndSeq;
    int Length;
    CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);
    if (TBlockX>=Length)
      continue;

    BlockUndo=UndoNext;
    AddUndoData(CurPtr->EditLine.GetStringAddr(),BlockStartLine+Line,
                CurPtr->EditLine.GetCurPos(),UNDO_EDIT);
    UndoNext=TRUE;

    char *TmpStr=new char[Length+3];
    int CurLength=TBlockX;
    memcpy(TmpStr,CurStr,TBlockX);
    if (Length>TBlockX+TBlockSizeX)
    {
      int CopySize=Length-(TBlockX+TBlockSizeX);
      memcpy(TmpStr+CurLength,CurStr+TBlockX+TBlockSizeX,CopySize);
      CurLength+=CopySize;
    }
    int EndLength=strlen(EndSeq);
    memcpy(TmpStr+CurLength,EndSeq,EndLength);
    CurLength+=EndLength;
    TmpStr[CurLength]=0;

    int CurPos=CurPtr->EditLine.GetCurPos();
    CurPtr->EditLine.SetBinaryString(TmpStr,CurLength);
    if (CurPos>TBlockX)
    {
      CurPos-=TBlockSizeX;
      if (CurPos<TBlockX)
        CurPos=TBlockX;
    }
    CurPtr->EditLine.SetCurPos(CurPos);
    delete[] TmpStr;
  }

  VBlockStart=NULL;
  BlockUndo=FALSE;
}

void Editor::VCopy(int Append)
{
  struct EditList *CurPtr=VBlockStart;
  char *CopyData=NULL;
  long DataSize=0,PrevSize=0;

  if (Append)
  {
    CopyData=PasteFormatFromClipboard(FAR_VerticalBlock);
    if (CopyData!=NULL)
      PrevSize=DataSize=strlen(CopyData);
    else
    {
      CopyData=PasteFromClipboard();
      if (CopyData!=NULL)
        PrevSize=DataSize=strlen(CopyData);
    }
  }

  for (int Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->Next)
  {
    int TBlockX=CurPtr->EditLine.TabPosToReal(VBlockX);
    int TBlockSizeX=CurPtr->EditLine.TabPosToReal(VBlockX+VBlockSizeX)-
                    CurPtr->EditLine.TabPosToReal(VBlockX);
    const char *CurStr,*EndSeq;
    int Length;
    CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);

    int AllocSize=Max(DataSize+Length+3,DataSize+TBlockSizeX+3);
    char *NewPtr=(char *)xf_realloc(CopyData,AllocSize);
    if (NewPtr==NULL)
    {
      delete CopyData;
      CopyData=NULL;
      break;
    }
    CopyData=NewPtr;

    if (Length>TBlockX)
    {
      int CopySize=Length-TBlockX;
      if (CopySize>TBlockSizeX)
        CopySize=TBlockSizeX;
      memcpy(CopyData+DataSize,CurStr+TBlockX,CopySize);
      if (CopySize<TBlockSizeX)
        memset(CopyData+DataSize+CopySize,' ',TBlockSizeX-CopySize);
    }
    else
      memset(CopyData+DataSize,' ',TBlockSizeX);

    DataSize+=TBlockSizeX;


    strcpy(CopyData+DataSize,DOS_EOL_fmt);
    DataSize+=2;
  }

  if (CopyData!=NULL)
  {
    if (UseDecodeTable)
      DecodeString(CopyData+PrevSize,(unsigned char *)TableSet.DecodeTable);
    CopyToClipboard(CopyData);
    CopyFormatToClipboard(FAR_VerticalBlock,CopyData);
    delete CopyData;
  }
}

void Editor::VPaste(char *ClipText)
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;

  if (*ClipText)
  {
    Flags.Set(FEDITOR_NEWUNDO);
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/
    int SaveOvertype=Flags.Check(FEDITOR_OVERTYPE);
    UnmarkBlock();
    Pasting++;
    DisableOut++;
    Edit::DisableEditOut(TRUE);
    if (Flags.Check(FEDITOR_OVERTYPE))
    {
      Flags.Clear(FEDITOR_OVERTYPE);
      CurLine->EditLine.SetOvertypeMode(FALSE);
    }

    VBlockStart=CurLine;
    BlockStartLine=NumLine;

    int StartPos=CurLine->EditLine.GetTabCurPos();

    VBlockX=StartPos;
    VBlockSizeX=0;
    VBlockY=NumLine;
    VBlockSizeY=0;

    struct EditList *SavedTopScreen=TopScreen;


    for (int I=0;ClipText[I]!=0;I++)
      if (ClipText[I]!=13 && ClipText[I+1]!=10)
        ProcessKey(ClipText[I]);
      else
      {
        BlockUndo=TRUE;
        int CurWidth=CurLine->EditLine.GetTabCurPos()-StartPos;
        if (CurWidth>VBlockSizeX)
          VBlockSizeX=CurWidth;
        VBlockSizeY++;
        if (CurLine->Next==NULL)
        {
          if (ClipText[I+2]!=0)
          {
            ProcessKey(KEY_END);
            ProcessKey(KEY_ENTER);
            /* $ 19.05.2001 IS
               �� ��������� ������� �����, ����� ��� �� ���� �� ������, �
               ������ - ��� ���������� ����������� ������ ��������� �� �����,
               ��� ���� ��������� � � ������ �����.
            */
            if(!EdOpt.AutoIndent)
              for (int I=0;I<StartPos;I++)
                ProcessKey(' ');
            /* IS $ */
          }
        }
        else
        {
          ProcessKey(KEY_DOWN);
          CurLine->EditLine.SetTabCurPos(StartPos);
          CurLine->EditLine.SetOvertypeMode(FALSE);
        }
        I++;
        continue;
      }

    int CurWidth=CurLine->EditLine.GetTabCurPos()-StartPos;
    if (CurWidth>VBlockSizeX)
      VBlockSizeX=CurWidth;
    if (VBlockSizeY==0)
      VBlockSizeY++;

    if (SaveOvertype)
    {
      Flags.Set(FEDITOR_OVERTYPE);
      CurLine->EditLine.SetOvertypeMode(TRUE);
    }

    TopScreen=SavedTopScreen;
    CurLine=VBlockStart;
    NumLine=BlockStartLine;
    CurLine->EditLine.SetTabCurPos(StartPos);

    Edit::DisableEditOut(FALSE);
    Pasting--;
    DisableOut--;
  }
  delete ClipText;
  BlockUndo=FALSE;
}


void Editor::VBlockShift(int Left)
{
  if (Flags.Check(FEDITOR_LOCKMODE) || Left && VBlockX==0 || VBlockSizeX<=0 || VBlockSizeY<=0)
    return;

  struct EditList *CurPtr=VBlockStart;

  int UndoNext=FALSE;

  for (int Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->Next)
  {
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/

    int TBlockX=CurPtr->EditLine.TabPosToReal(VBlockX);
    int TBlockSizeX=CurPtr->EditLine.TabPosToReal(VBlockX+VBlockSizeX)-
                    CurPtr->EditLine.TabPosToReal(VBlockX);

    const char *CurStr,*EndSeq;
    int Length;
    CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);
    if (TBlockX>Length)
      continue;
    if (Left && CurStr[TBlockX-1]=='\t' ||
        !Left && TBlockX+TBlockSizeX<Length && CurStr[TBlockX+TBlockSizeX]=='\t')
    {
      CurPtr->EditLine.ReplaceTabs();
      CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);
      TBlockX=CurPtr->EditLine.TabPosToReal(VBlockX);
      TBlockSizeX=CurPtr->EditLine.TabPosToReal(VBlockX+VBlockSizeX)-
                  CurPtr->EditLine.TabPosToReal(VBlockX);
    }


    BlockUndo=UndoNext;
    AddUndoData(CurPtr->EditLine.GetStringAddr(),BlockStartLine+Line,
                CurPtr->EditLine.GetCurPos(),UNDO_EDIT);
    UndoNext=TRUE;

    int StrLength=Max(Length,TBlockX+TBlockSizeX+!Left);
    char *TmpStr=new char[StrLength+3];
    memset(TmpStr,' ',StrLength);
    memcpy(TmpStr,CurStr,Length);

    if (Left)
    {
      int Ch=TmpStr[TBlockX-1];
      for (int I=TBlockX;I<TBlockX+TBlockSizeX;I++)
        TmpStr[I-1]=TmpStr[I];
      TmpStr[TBlockX+TBlockSizeX-1]=Ch;
    }
    else
    {
      int Ch=TmpStr[TBlockX+TBlockSizeX];
      for (int I=TBlockX+TBlockSizeX-1;I>=TBlockX;I--)
        TmpStr[I+1]=TmpStr[I];
      TmpStr[TBlockX]=Ch;
    }

    while (StrLength>0 && TmpStr[StrLength-1]==' ')
      StrLength--;
    int EndLength=strlen(EndSeq);
    memcpy(TmpStr+StrLength,EndSeq,EndLength);
    StrLength+=EndLength;
    TmpStr[StrLength]=0;

    CurPtr->EditLine.SetBinaryString(TmpStr,StrLength);
    delete[] TmpStr;
  }
  VBlockX+=Left ? -1:1;
  CurLine->EditLine.SetTabCurPos(Left ? VBlockX:VBlockX+VBlockSizeX);
}


int Editor::EditorControl(int Command,void *Param)
{
  int I;
  _ECTLLOG(CleverSysLog SL("Editor::EditorControl()"));
  _ECTLLOG(SysLog("Command=%s Param=[%d/0x%08X]",_ECTL_ToName(Command),Param,Param));
  switch(Command)
  {
    case ECTL_GETSTRING:
    {
      if(!Param)
        return FALSE;
      else
      {
        struct EditorGetString *GetString=(struct EditorGetString *)Param;
        struct EditList *CurPtr=GetStringByNumber(GetString->StringNumber);
        if (!CurPtr)
        {
          _ECTLLOG(SysLog("struct EditorGetString => GetStringByNumber(%d) return NULL",GetString->StringNumber));
          return(FALSE);
        }
        //CurPtr->EditLine.GetBinaryString(GetString->StringText,
        //                      &const_cast<const char*>(GetString->StringEOL),
        //                      GetString->StringLength);
        CurPtr->EditLine.GetBinaryString(GetString->StringText,
                                const_cast<const char **>(&GetString->StringEOL),
                                GetString->StringLength);
        GetString->SelStart=-1;
        GetString->SelEnd=0;
        int DestLine=GetString->StringNumber;
        if (DestLine==-1)
          DestLine=NumLine;
        if (BlockStart!=NULL)
        {
          /* $ 12.11.2002 DJ
             ������ ��������� ����������
          */
          CurPtr->EditLine.GetRealSelection(GetString->SelStart,GetString->SelEnd);
          /* DJ $ */
        }
        else if (VBlockStart!=NULL && DestLine>=VBlockY && DestLine<VBlockY+VBlockSizeY)
        {
          GetString->SelStart=CurPtr->EditLine.TabPosToReal(VBlockX);
          GetString->SelEnd=GetString->SelStart+
                            CurPtr->EditLine.TabPosToReal(VBlockX+VBlockSizeX)-
                            CurPtr->EditLine.TabPosToReal(VBlockX);
        }
        _ECTLLOG(char *LinDump=(GetString->StringEOL?(char *)_SysLog_LinearDump(GetString->StringEOL,strlen(GetString->StringEOL)):NULL));
        _ECTLLOG(SysLog("struct EditorGetString{"));
        _ECTLLOG(SysLog("  StringNumber    =%d",GetString->StringNumber));
        _ECTLLOG(SysLog("  StringText      ='%s'",GetString->StringText));
        _ECTLLOG(SysLog("  StringEOL       ='%s'",GetString->StringEOL?LinDump:"(null)"));
        _ECTLLOG(SysLog("  StringLength    =%d",GetString->StringLength));
        _ECTLLOG(SysLog("  SelStart        =%d",GetString->SelStart));
        _ECTLLOG(SysLog("  SelEnd          =%d",GetString->SelEnd));
        _ECTLLOG(SysLog("}"));
        _ECTLLOG(if(LinDump)xf_free(LinDump));
      }
      return(TRUE);
    }

    case ECTL_INSERTSTRING:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      else
      {
        int Indent=Param!=NULL && *(int *)Param!=FALSE;
        if (!Indent)
          Pasting++;
        Flags.Set(FEDITOR_NEWUNDO);
        InsertString();
        Show();
        if (!Indent)
          Pasting--;
      }
      return(TRUE);
    }

    case ECTL_INSERTTEXT:
    {
      if(!Param)
        return FALSE;

      _ECTLLOG(SysLog("(char *)Param='%s'",(char *)Param));
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      else
      {
        char *Str=(char *)Param;
        Pasting++;
        DisableOut++;
        Edit::DisableEditOut(TRUE);
        while (*Str)
          ProcessKey(*(Str++));
        Edit::DisableEditOut(FALSE);
        DisableOut--;
        Pasting--;
      }
      return(TRUE);
    }

    case ECTL_SETSTRING:
    {
      if(!Param)
        return FALSE;

      struct EditorSetString *SetString=(struct EditorSetString *)Param;
      _ECTLLOG(SysLog("struct EditorSetString{"));
      _ECTLLOG(SysLog("  StringNumber    =%d",SetString->StringNumber));
      _ECTLLOG(SysLog("  StringText      ='%s'",SetString->StringText));
      _ECTLLOG(SysLog("  StringEOL       ='%s'",SetString->StringEOL?_SysLog_LinearDump((LPBYTE)SetString->StringEOL,strlen(SetString->StringEOL)):"(null)"));
      _ECTLLOG(SysLog("  StringLength    =%d",SetString->StringLength));
      _ECTLLOG(SysLog("}"));

      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      else
      {
        /* $ 06.08.2002 IS
           ��������� ������������ StringLength � ������ FALSE, ���� ��� ������
           ����.
        */
        int Length=SetString->StringLength;
        if(Length < 0)
        {
          _ECTLLOG(SysLog("SetString->StringLength < 0"));
          return(FALSE);
        }

        struct EditList *CurPtr=GetStringByNumber(SetString->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",SetString->StringNumber));
          return(FALSE);
        }

        const char *EOL=SetString->StringEOL==NULL ? GlobalEOL:SetString->StringEOL;
        /* IS 06.08.2002 IS $ */
        int LengthEOL=strlen(EOL);
        char *NewStr=(char*)xf_malloc(Length+LengthEOL+1);
        if (NewStr==NULL)
        {
          _ECTLLOG(SysLog("xf_malloc(%d) return NULL",Length+LengthEOL+1));
          return(FALSE);
        }

        int DestLine=SetString->StringNumber;
        if (DestLine==-1)
          DestLine=NumLine;

        memcpy(NewStr,SetString->StringText,Length);
        memcpy(NewStr+Length,EOL,LengthEOL);
        AddUndoData(CurPtr->EditLine.GetStringAddr(),DestLine,
                    CurPtr->EditLine.GetCurPos(),UNDO_EDIT);

        int CurPos=CurPtr->EditLine.GetCurPos();
        CurPtr->EditLine.SetBinaryString(NewStr,Length+LengthEOL);
        CurPtr->EditLine.SetCurPos(CurPos);
        TextChanged(1);    // 10.08.2000 skv - Modified->TextChanged
        xf_free(NewStr);
      }
      return(TRUE);
    }

    case ECTL_DELETESTRING:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      DeleteString(CurLine,FALSE,NumLine);
      return(TRUE);
    }

    case ECTL_DELETECHAR:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      Pasting++;
      ProcessKey(KEY_DEL);
      Pasting--;
      return(TRUE);
    }

    case ECTL_GETINFO:
    {
      struct EditorInfo *Info=(struct EditorInfo *)Param;
      if(Info && !IsBadWritePtr(Info,sizeof(struct EditorInfo)))
      {
        memset(Info,0,sizeof(*Info));
        Info->EditorID=Editor::EditorID;
        Info->FileName="";
        Info->WindowSizeX=ObjWidth;
        Info->WindowSizeY=Y2-Y1;
        Info->TotalLines=NumLastLine;
        Info->CurLine=NumLine;
        Info->CurPos=CurLine->EditLine.GetCurPos();
        Info->CurTabPos=CurLine->EditLine.GetTabCurPos();
        Info->TopScreenLine=NumLine-CalcDistance(TopScreen,CurLine,-1);
        Info->LeftPos=CurLine->EditLine.GetLeftPos();
        Info->Overtype=Flags.Check(FEDITOR_OVERTYPE);
        Info->BlockType=BTYPE_NONE;
        if (BlockStart!=NULL)
          Info->BlockType=BTYPE_STREAM;
        if (VBlockStart!=NULL)
          Info->BlockType=BTYPE_COLUMN;
        Info->BlockStartLine=Info->BlockType==BTYPE_NONE ? 0:BlockStartLine;
        Info->AnsiMode=AnsiText;
        Info->TableNum=UseDecodeTable ? TableNum-1:-1;
        //Info->Options=0;
        if (EdOpt.ExpandTabs)
          Info->Options|=EOPT_EXPANDTABS;
        if (EdOpt.PersistentBlocks)
          Info->Options|=EOPT_PERSISTENTBLOCKS;
        if (EdOpt.DelRemovesBlocks)
          Info->Options|=EOPT_DELREMOVESBLOCKS;
        if (EdOpt.AutoIndent)
          Info->Options|=EOPT_AUTOINDENT;
        if (EdOpt.SavePos)
          Info->Options|=EOPT_SAVEFILEPOSITION;
        if (EdOpt.AutoDetectTable)
          Info->Options|=EOPT_AUTODETECTTABLE;
        if (EdOpt.CursorBeyondEOL)
          Info->Options|=EOPT_CURSORBEYONDEOL;
        Info->TabSize=EdOpt.TabSize;
        Info->BookMarkCount=BOOKMARK_COUNT;
        Info->CurState=Flags.Check(FEDITOR_LOCKMODE)?ECSTATE_LOCKED:0;
        Info->CurState|=!Flags.Check(FEDITOR_MODIFIED)?ECSTATE_SAVED:0;
        Info->CurState|=Flags.Check(FEDITOR_MODIFIED|FEDITOR_WASCHANGED)?ECSTATE_MODIFIED:0;
        return TRUE;
      }
      _ECTLLOG(SysLog("Error: Param == NULL or IsBadWritePtr(Param,sizeof(struct EditorInfo))"));
      return FALSE;
    }

    case ECTL_SETPOSITION:
    {
      // "������� ���� �����..."
      if(Param && !IsBadReadPtr(Param,sizeof(struct EditorSetPosition)))
      {
        // ...� ��� ������ ���������� � ���, ��� ���������
        struct EditorSetPosition *Pos=(struct EditorSetPosition *)Param;
        _ECTLLOG(SysLog("struct EditorSetPosition{"));
        _ECTLLOG(SysLog("  CurLine       = %d",Pos->CurLine));
        _ECTLLOG(SysLog("  CurPos        = %d",Pos->CurPos));
        _ECTLLOG(SysLog("  CurTabPos     = %d",Pos->CurTabPos));
        _ECTLLOG(SysLog("  TopScreenLine = %d",Pos->TopScreenLine));
        _ECTLLOG(SysLog("  LeftPos       = %d",Pos->LeftPos));
        _ECTLLOG(SysLog("  Overtype      = %d",Pos->Overtype));
        _ECTLLOG(SysLog("}"));

        DisableOut++;

        int CurPos=CurLine->EditLine.GetCurPos();

        // �������� ���� �� ��������� ��� (���� ����)
        if ((Pos->CurLine >= 0 || Pos->CurPos >= 0)&&
            (Pos->CurLine!=NumLine || Pos->CurPos!=CurPos))
          Flags.Set(FEDITOR_CURPOSCHANGEDBYPLUGIN);

        if (Pos->CurLine >= 0) // �������� ������
        {
          if (Pos->CurLine==NumLine-1)
            Up();
          else
            if (Pos->CurLine==NumLine+1)
              Down();
            else
              GoToLine(Pos->CurLine);
        }

        if (Pos->TopScreenLine >= 0 && Pos->TopScreenLine<=NumLine)
        {
          TopScreen=CurLine;
          for (int I=NumLine;I>0 && NumLine-I<Y2-Y1+1 && I!=Pos->TopScreenLine;I--)
            TopScreen=TopScreen->Prev;
        }

        if (Pos->CurPos >= 0)
          CurLine->EditLine.SetCurPos(Pos->CurPos);

        if (Pos->CurTabPos >= 0)
          CurLine->EditLine.SetTabCurPos(Pos->CurTabPos);

        if (Pos->LeftPos >= 0)
          CurLine->EditLine.SetLeftPos(Pos->LeftPos);

        /* $ 30.08.2001 IS
           ��������� ������ ����� ���������� �����, � ��������� ������ ��������
           �����, �.�. ��������������� ������, ��� ����� �������, � ����� ����
           ��������������, � ���������� ���� �������� �������������� ���������.
        */
        if (Pos->Overtype >= 0)
        {
          Flags.Change(FEDITOR_OVERTYPE,Pos->Overtype);
          CurLine->EditLine.SetOvertypeMode(Flags.Check(FEDITOR_OVERTYPE));
        }
        /* IS $ */

        DisableOut--;
        return TRUE;
      }
      _ECTLLOG(SysLog("Error: Param == NULL or IsBadReadPtr(Param,sizeof(struct EditorSetPosition))"));
      return FALSE;
    }

    case ECTL_SELECT:
    {
      if(!Param)
        return FALSE;
      else
      {
        struct EditorSelect *Sel=(struct EditorSelect *)Param;
        _ECTLLOG(SysLog("struct EditorSelect{"));
        _ECTLLOG(SysLog("  BlockType     =%s (%d)",(Sel->BlockType==BTYPE_NONE?"BTYPE_NONE":(Sel->BlockType==BTYPE_STREAM?"":(Sel->BlockType==BTYPE_COLUMN?"BTYPE_COLUMN":"BTYPE_?????"))),Sel->BlockType));
        _ECTLLOG(SysLog("  BlockStartLine=%d",Sel->BlockStartLine));
        _ECTLLOG(SysLog("  BlockStartPos =%d",Sel->BlockStartPos));
        _ECTLLOG(SysLog("  BlockWidth    =%d",Sel->BlockWidth));
        _ECTLLOG(SysLog("  BlockHeight   =%d",Sel->BlockHeight));
        _ECTLLOG(SysLog("}"));

        UnmarkBlock();
        if (Sel->BlockType==BTYPE_NONE)
          return(TRUE);

        struct EditList *CurPtr=GetStringByNumber(Sel->BlockStartLine);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",Sel->BlockStartLine));
          return(FALSE);
        }

        if (Sel->BlockType==BTYPE_STREAM)
        {
          BlockStart=CurPtr;
          BlockStartLine=Sel->BlockStartLine;
          for (I=0;I<Sel->BlockHeight;I++)
          {
            int SelStart=(I==0) ? Sel->BlockStartPos:0;
            int SelEnd=(I<Sel->BlockHeight-1) ? -1:Sel->BlockStartPos+Sel->BlockWidth;
            CurPtr->EditLine.Select(SelStart,SelEnd);
            CurPtr=CurPtr->Next;
            if (CurPtr==NULL)
              return(FALSE);
          }
        }
        if (Sel->BlockType==BTYPE_COLUMN)
        {
          VBlockStart=CurPtr;
          BlockStartLine=Sel->BlockStartLine;

          if (Sel->BlockWidth==-1)
            return(FALSE);

          VBlockX=Sel->BlockStartPos;
          VBlockY=Sel->BlockStartLine;
          VBlockSizeX=Sel->BlockWidth;
          VBlockSizeY=Sel->BlockHeight;
        }
      }
      return(TRUE);
    }

    case ECTL_REDRAW:
    {
      Show();
      ScrBuf.Flush();
      return(TRUE);
    }

    case ECTL_TABTOREAL:
    {
      if(!Param)
        return FALSE;
      else
      {
        struct EditorConvertPos *ecp=(struct EditorConvertPos *)Param;
        struct EditList *CurPtr=GetStringByNumber(ecp->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",ecp->StringNumber));
          return(FALSE);
        }
        ecp->DestPos=CurPtr->EditLine.TabPosToReal(ecp->SrcPos);
        _ECTLLOG(SysLog("struct EditorConvertPos{"));
        _ECTLLOG(SysLog("  StringNumber =%d",ecp->StringNumber));
        _ECTLLOG(SysLog("  SrcPos       =%d",ecp->SrcPos));
        _ECTLLOG(SysLog("  DestPos      =%d",ecp->DestPos));
        _ECTLLOG(SysLog("}"));
      }
      return(TRUE);
    }

    case ECTL_REALTOTAB:
    {
      if(!Param)
        return FALSE;
      else
      {
        struct EditorConvertPos *ecp=(struct EditorConvertPos *)Param;
        struct EditList *CurPtr=GetStringByNumber(ecp->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",ecp->StringNumber));
          return(FALSE);
        }
        ecp->DestPos=CurPtr->EditLine.RealPosToTab(ecp->SrcPos);
        _ECTLLOG(SysLog("struct EditorConvertPos{"));
        _ECTLLOG(SysLog("  StringNumber =%d",ecp->StringNumber));
        _ECTLLOG(SysLog("  SrcPos       =%d",ecp->SrcPos));
        _ECTLLOG(SysLog("  DestPos      =%d",ecp->DestPos));
        _ECTLLOG(SysLog("}"));
      }
      return(TRUE);
    }

    case ECTL_EXPANDTABS:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return FALSE;
      }
      else
      {
        int StringNumber=*(int *)Param;
        struct EditList *CurPtr=GetStringByNumber(StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",StringNumber));
          return FALSE;
        }
        AddUndoData(CurPtr->EditLine.GetStringAddr(),StringNumber,
                    CurPtr->EditLine.GetCurPos(),UNDO_EDIT);
        CurPtr->EditLine.ReplaceTabs();
      }
      return TRUE;
    }

    // ������ ����������� � FileEditor::EditorControl()
    // � ������� - ����� ������
    case ECTL_ADDCOLOR:
    {
      if(!Param)
        return FALSE;
      else
      {
        struct EditorColor *col=(struct EditorColor *)Param;
        _ECTLLOG(SysLog("struct EditorColor{"));
        _ECTLLOG(SysLog("  StringNumber=%d",col->StringNumber));
        _ECTLLOG(SysLog("  ColorItem   =%d (0x%08X)",col->ColorItem,col->ColorItem));
        _ECTLLOG(SysLog("  StartPos    =%d",col->StartPos));
        _ECTLLOG(SysLog("  EndPos      =%d",col->EndPos));
        _ECTLLOG(SysLog("  Color       =%d (0x%08X)",col->Color,col->Color));
        _ECTLLOG(SysLog("}"));

        struct ColorItem newcol;
        newcol.StartPos=col->StartPos;
        newcol.EndPos=col->EndPos;
        newcol.Color=col->Color;
        struct EditList *CurPtr=GetStringByNumber(col->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",col->StringNumber));
          return FALSE;
        }
        if (col->Color==0)
          return(CurPtr->EditLine.DeleteColor(newcol.StartPos));
        CurPtr->EditLine.AddColor(&newcol);
      }
      return TRUE;
    }

    // ������ ����������� � FileEditor::EditorControl()
    // � ������� - ����� ������
    case ECTL_GETCOLOR:
    {
      if(!Param)
        return FALSE;
      else
      {
        struct EditorColor *col=(struct EditorColor *)Param;
        struct EditList *CurPtr=GetStringByNumber(col->StringNumber);
        if (!CurPtr || IsBadWritePtr(col,sizeof(struct EditorColor)))
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL or IsBadWritePtr(col,sizeof(struct EditorColor)",col->StringNumber));
          return FALSE;
        }
        struct ColorItem curcol;
        if (!CurPtr->EditLine.GetColor(&curcol,col->ColorItem))
        {
          _ECTLLOG(SysLog("GetColor() return NULL"));
          return FALSE;
        }
        col->StartPos=curcol.StartPos;
        col->EndPos=curcol.EndPos;
        col->Color=curcol.Color;
        _ECTLLOG(SysLog("struct EditorColor{"));
        _ECTLLOG(SysLog("  StringNumber=%d",col->StringNumber));
        _ECTLLOG(SysLog("  ColorItem   =%d (0x%08X)",col->ColorItem,col->ColorItem));
        _ECTLLOG(SysLog("  StartPos    =%d",col->StartPos));
        _ECTLLOG(SysLog("  EndPos      =%d",col->EndPos));
        _ECTLLOG(SysLog("  Color       =%d (0x%08X)",col->Color,col->Color));
        _ECTLLOG(SysLog("}"));
      }
      return TRUE;
    }

    /*$ 07.09.2000 skv
      New ECTL parameter
    */
    // ������ ����������� � FileEditor::EditorControl()
    case ECTL_PROCESSKEY:
    {
      _ECTLLOG(SysLog("Key = %s",_FARKEY_ToName((DWORD)Param)));
      ProcessKey((int)Param);
      return TRUE;
    }
    /* skv$*/
    /* $ 16.02.2001 IS
         ��������� ��������� ���������� �������� ���������. Param ��������� ��
         ��������� EditorSetParameter
    */
    case ECTL_SETPARAM:
    {
      struct EditorSetParameter *espar=(struct EditorSetParameter *)Param;
      if(espar)
      {
        int rc=TRUE;
        _ECTLLOG(SysLog("struct EditorSetParameter{"));
        _ECTLLOG(SysLog("  Type        =%s",_ESPT_ToName(espar->Type)));
        switch(espar->Type)
        {
          case ESPT_GETWORDDIV:
            _ECTLLOG(SysLog("  cParam      =(%p)",espar->Param.cParam));
            if(!IsBadWritePtr(espar->Param.cParam,sizeof(EdOpt.WordDiv)))
              strncpy(espar->Param.cParam,EdOpt.WordDiv,sizeof(EdOpt.WordDiv)-1);
            else
              rc=FALSE;
            break;
          case ESPT_SETWORDDIV:
            _ECTLLOG(SysLog("  cParam      =[%s]",espar->Param.cParam));
            SetWordDiv((!espar->Param.cParam || !*espar->Param.cParam)?Opt.WordDiv:espar->Param.cParam);
            break;
          case ESPT_TABSIZE:
            _ECTLLOG(SysLog("  iParam      =%d",espar->Param.iParam));
            SetTabSize(espar->Param.iParam);
            break;
          case ESPT_EXPANDTABS:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetConvertTabs(espar->Param.iParam);
            break;
          case ESPT_AUTOINDENT:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetAutoIndent(espar->Param.iParam);
            break;
          case ESPT_CURSORBEYONDEOL:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetCursorBeyondEOL(espar->Param.iParam);
            break;
          case ESPT_CHARCODEBASE:
            _ECTLLOG(SysLog("  iParam      =%s",(espar->Param.iParam==0?"0 (Oct)":(espar->Param.iParam==1?"1 (Dec)":(espar->Param.iParam==2?"2 (Hex)":"?????")))));
            SetCharCodeBase(espar->Param.iParam);
            break;
          /* $ 07.08.2001 IS ������� ��������� �� ������� */
          case ESPT_CHARTABLE:
          {
            _ECTLLOG(if(espar->Param.iParam <= 3)SysLog("  iParam      =%s",(espar->Param.iParam==1?"1 (OEM)":(espar->Param.iParam==2?"2 (ANSI)":"3 (table '0')"))));
            _ECTLLOG(else SysLog("  iParam      =%d",espar->Param.iParam));
            int UseUnicode=FALSE;
            /*  $ 04.11.2001 IS
                ��� ��������� ����� ������� ������� ��� ��� ����
                (������ �������� oem)
            */
            int oldAnsiText(AnsiText), oldUseDecodeTable(UseDecodeTable),
                oldTableNum(TableNum), oldChangedByUser(Flags.Check(FEDITOR_TABLECHANGEDBYUSER));

            AnsiText=espar->Param.iParam==2,
            UseDecodeTable=espar->Param.iParam>1,
            TableNum=UseDecodeTable?espar->Param.iParam-3:-1;
            Flags.Set(FEDITOR_TABLECHANGEDBYUSER);

            if(AnsiText)
               rc=GetTable(&TableSet,TRUE,TableNum,UseUnicode);
            else if(UseDecodeTable)
               rc=PrepareTable(&TableSet, TableNum);

            /* $ 07.03.2002 IS
               ��� ����, ����� �� Shift-F8 ��������� ���������� ����� ��������
               ������� ������ �� 1, �.�. ��� ��������� ���� �� � 0, � � 1.
            */
            if(rc)
               ++TableNum;
            else
            /* IS $ */
            {
              Flags.Change(FEDITOR_TABLECHANGEDBYUSER,oldChangedByUser);
              TableNum=oldTableNum;
              UseDecodeTable=oldUseDecodeTable;
              AnsiText=oldAnsiText;
            }
            /* IS $ */

            SetStringsTable();
            if (HostFileEditor) HostFileEditor->ChangeEditKeyBar();
            Show();
          }
          /* IS $ */
          /* $ 29.10.2001 IS ��������� ��������� "��������� ������� �����" */
          case ESPT_SAVEFILEPOSITION:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetSavePosMode(espar->Param.iParam, -1);
            break;
          /* IS $ */
          /* $ 23.03.2002 IS ���������/�������� ��������� ����� */
          case ESPT_LOCKMODE:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            Flags.Change(FEDITOR_LOCKMODE, espar->Param.iParam);
            break;
          /* IS $ */
          default:
            _ECTLLOG(SysLog("}"));
            return FALSE;
        }
        _ECTLLOG(SysLog("}"));
        return rc;
      }
      return  FALSE;
    }
    /* IS $ */
    /* $ 04.04.2002 IS
       ������ ���� ��������� "�������������� ��������� �����"
    */
    case ECTL_TURNOFFMARKINGBLOCK:
    {
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      return TRUE;
    }
    /* IS $ */

    case ECTL_DELETEBLOCK:
    {
      if (Flags.Check(FEDITOR_LOCKMODE) || !(VBlockStart || BlockStart))
      {
        _ECTLLOG(if(Flags.Check(FEDITOR_LOCKMODE))SysLog("FEDITOR_LOCKMODE!"));
        _ECTLLOG(if(!(VBlockStart || BlockStart))SysLog("Not selected block!"));
        return FALSE;
      }

      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      DeleteBlock();
      Show();
      return TRUE;
    }
  }
  return FALSE;
}

int Editor::SetBookmark(DWORD Pos)
{
  if(Pos < BOOKMARK_COUNT)
  {
    SavePos.Line[Pos]=NumLine;
    SavePos.Cursor[Pos]=CurLine->EditLine.GetCurPos();
    SavePos.LeftPos[Pos]=CurLine->EditLine.GetLeftPos();
    SavePos.ScreenLine[Pos]=CalcDistance(TopScreen,CurLine,-1);
    return TRUE;
  }
  return FALSE;
}

int Editor::GotoBookmark(DWORD Pos)
{
  if(Pos < BOOKMARK_COUNT)
  {
    if (SavePos.Line[Pos]!=0xffffffff)
    {
      GoToLine(SavePos.Line[Pos]);
      CurLine->EditLine.SetCurPos(SavePos.Cursor[Pos]);
      CurLine->EditLine.SetLeftPos(SavePos.LeftPos[Pos]);
      TopScreen=CurLine;
      for (int I=0;I<SavePos.ScreenLine[Pos] && TopScreen->Prev!=NULL;I++)
        TopScreen=TopScreen->Prev;
      if (!EdOpt.PersistentBlocks)
        UnmarkBlock();
      Show();
    }
    return TRUE;
  }
  return FALSE;
}

struct EditList * Editor::GetStringByNumber(int DestLine)
{
  if (DestLine==NumLine || DestLine<0)
    return(CurLine);
  if (DestLine>NumLastLine)
    return(NULL);

  if (DestLine>NumLine)
  {
    struct EditList *CurPtr=CurLine;
    for (int Line=NumLine;Line<DestLine;Line++)
    {
      CurPtr=CurPtr->Next;
      if (CurPtr==NULL)
        return(NULL);
    }
    return(CurPtr);
  }

  if (DestLine<NumLine && DestLine>NumLine/2)
  {
    struct EditList *CurPtr=CurLine;
    for (int Line=NumLine;Line>DestLine;Line--)
    {
      CurPtr=CurPtr->Prev;
      if (CurPtr==NULL)
        return(NULL);
    }
    return(CurPtr);
  }

  {
    struct EditList *CurPtr=TopList;
    for (int Line=0;Line<DestLine;Line++)
    {
      CurPtr=CurPtr->Next;
      if (CurPtr==NULL)
        return(NULL);
    }
    return(CurPtr);
  }
}

void Editor::SetReplaceMode(int Mode)
{
  ::ReplaceMode=Mode;
}

int Editor::GetLineCurPos()
{
  return CurLine->EditLine.GetTabCurPos();
}

void Editor::BeginVBlockMarking()
{
    UnmarkBlock();
    VBlockStart=CurLine;
    VBlockX=CurLine->EditLine.GetTabCurPos();
    VBlockSizeX=0;
    VBlockY=NumLine;
    VBlockSizeY=1;
    Flags.Set(FEDITOR_MARKINGVBLOCK);
    BlockStartLine=NumLine;
    //_D(SysLog("BeginVBlockMarking, set vblock to  VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
}

void Editor::AdjustVBlock(int PrevX)
{
    int x=GetLineCurPos();
    int c2;

    //_D(SysLog("AdjustVBlock, x=%i,   vblock is VBlockY=%i:%i, VBlockX=%i:%i, PrevX=%i",x,VBlockY,VBlockSizeY,VBlockX,VBlockSizeX,PrevX));
    if ( x==VBlockX+VBlockSizeX)  // ������ �� ���������, ������� ��������� ���
        return;
    if ( x>VBlockX )  // ������ ������ ������ �����
    {
        VBlockSizeX=x-VBlockX;
        //_D(SysLog("x>VBlockX");
    }
    else if ( x<VBlockX ) // ������ ������ �� ������ �����
    {
        c2=VBlockX;
        if ( PrevX>VBlockX )    // ���������� ������, � ������ �����
        {
            VBlockX=x;
            VBlockSizeX=c2-x;   // ������ ����
        }
        else      // ���������� ����� � ������ ��� ������ �����
        {
            VBlockX=x;
            VBlockSizeX+=c2-x;  // ��������� ����
        }
        //_D(SysLog("x<VBlockX"));
    }
    else if (x==VBlockX && x!=PrevX)
    {
        VBlockSizeX=0;  // ������ � 0, ������ �������� ���� �� ���������
        //_D(SysLog("x==VBlockX && x!=PrevX"));
    }
    // ����������
    //   ������ x>VBLockX+VBlockSizeX �� ����� ����
    //   ������ ��� ������ ������� ����� �� ���������, �� �� ������

    //_D(SysLog("AdjustVBlock, changed vblock  VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
}


/* $ 24.09.2000 SVS
  ������������� Xlat
*/
void Editor::Xlat()
{
  struct EditList *CurPtr;
  int Line;
  BOOL DoXlat=FALSE;

  if (VBlockStart!=NULL)
  {
    CurPtr=VBlockStart;

    for (Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->Next)
    {
      int TBlockX=CurPtr->EditLine.TabPosToReal(VBlockX);
      int TBlockSizeX=CurPtr->EditLine.TabPosToReal(VBlockX+VBlockSizeX)-
                      CurPtr->EditLine.TabPosToReal(VBlockX);
      const char *CurStr,*EndSeq;
      int Length;
      CurPtr->EditLine.GetBinaryString(CurStr,&EndSeq,Length);
      int CopySize=Length-TBlockX;
      if (CopySize>TBlockSizeX)
         CopySize=TBlockSizeX;
      AddUndoData(CurPtr->EditLine.GetStringAddr(),BlockStartLine+Line,0,UNDO_EDIT);
      BlockUndo=TRUE;
      ::Xlat(CurPtr->EditLine.Str,TBlockX,TBlockX+CopySize,CurPtr->EditLine.TableSet,Opt.XLat.Flags);
    }
    DoXlat=TRUE;
  }
  else
  {
    Line=0;
    CurPtr=BlockStart;
    /* $ 25.11.2000 IS
         ���� ��� ���������, �� ���������� ������� �����. ����� ������������ ��
         ������ ����������� ������ ������������.
    */
    if(CurPtr!=NULL)
    {
      while (CurPtr!=NULL)
      {
        int StartSel,EndSel;
        CurPtr->EditLine.GetSelection(StartSel,EndSel);
        if (StartSel==-1)
          break;
        if(EndSel == -1)
          EndSel=strlen(CurPtr->EditLine.Str);
        AddUndoData(CurPtr->EditLine.GetStringAddr(),BlockStartLine+Line,0,UNDO_EDIT);
        ::Xlat(CurPtr->EditLine.Str,StartSel,EndSel,CurPtr->EditLine.TableSet,Opt.XLat.Flags);
        BlockUndo=TRUE;
        Line++;
        CurPtr=CurPtr->Next;
      }
      DoXlat=TRUE;
    }
    else
    {
      char *Str=CurLine->EditLine.Str;
      int start=CurLine->EditLine.GetCurPos(), end, StrSize=strlen(Str);
      /* $ 10.12.2000 IS
         ������������ ������ �� �����, �� ������� ����� ������, ��� �� �����,
         ��� ��������� ����� ������� ������� �� 1 ������
      */
      DoXlat=TRUE;

      /* $ 12.01.2004 IS
         ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
         ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
      */
      if(IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[start]))
      {
         if(start) start--;
         DoXlat=(!IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[start]));
      }

      if(DoXlat)
      {
        while(start>=0 && !IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[start]))
          start--;
        start++;
        end=start+1;
        while(end<StrSize && !IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[end]))
          end++;
        AddUndoData(Str,NumLine,start,UNDO_EDIT);
        ::Xlat(Str,start,end,CurLine->EditLine.TableSet,Opt.XLat.Flags);
      }
      /* 12.01.2004 IS */
     /* IS $ */
    }
    /* IS $ */
  }
  BlockUndo=FALSE;
  if(DoXlat)
    TextChanged(1);
}
/* SVS $ */

/* $ 15.02.2001 IS
     ����������� � ���������� �� ������ ����� ������������ �����.
     ����� ���� ���������� �� ������� ���������, �� ��� ��, imho,
     ������ �� ��������.
*/
//������� ������ ���������
void Editor::SetTabSize(int NewSize)
{
  if (NewSize<1 || NewSize>512)
    NewSize=8;
  if(NewSize!=EdOpt.TabSize) /* ������ ������ ��������� ������ � ��� ������, ���� ��
                          �� ����� ���� ��������� */
  {
    EdOpt.TabSize=NewSize;
    struct EditList *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->EditLine.SetTabSize(NewSize);
      CurPtr=CurPtr->Next;
    }
  }
}

// ������� ����� ������� ������ ���������
// �������� ����������, ������, �.�. ������� �� ��������� ������� �� ���������
void Editor::SetConvertTabs(int NewMode)
{
  if(NewMode!=EdOpt.ExpandTabs) /* ������ ����� ������ � ��� ������, ���� ��
                              �� ����� ���� ��������� */
  {
    EdOpt.ExpandTabs=NewMode;
    struct EditList *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->EditLine.SetConvertTabs(NewMode);
      CurPtr->EditLine.ReplaceTabs();
      CurPtr=CurPtr->Next;
    }
  }
}
/* IS $ */

/* $ 15.02.2001 IS
     + ������ ������������ :) ������� ��������� EdOpt.DelRemovesBlocks �
       EdOpt.PersistentBlocks
*/
void Editor::SetDelRemovesBlocks(int NewMode)
{
  if(NewMode!=EdOpt.DelRemovesBlocks)
  {
    EdOpt.DelRemovesBlocks=NewMode;
    struct EditList *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->EditLine.SetDelRemovesBlocks(NewMode);
      CurPtr=CurPtr->Next;
    }
  }
}

void Editor::SetPersistentBlocks(int NewMode)
{
  if(NewMode!=EdOpt.PersistentBlocks)
  {
    EdOpt.PersistentBlocks=NewMode;
    struct EditList *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->EditLine.SetPersistentBlocks(NewMode);
      CurPtr=CurPtr->Next;
    }
  }
}
/* IS $ */

/* $ 26.02.2001 IS
     "������ �� ��������� ������"
*/
void Editor::SetCursorBeyondEOL(int NewMode)
{
  if(NewMode!=EdOpt.CursorBeyondEOL)
  {
    EdOpt.CursorBeyondEOL=NewMode;
    struct EditList *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->EditLine.SetEditBeyondEnd(NewMode);
      CurPtr=CurPtr->Next;
    }
  }
  /* $ 16.10.2001 SKV
    ���� ������������� ���� ���� ���� �����,
    �� ��-�� ���� ����� ��������� ������� �����
    ��� ��������� ������������ ������.
  */
  if(EdOpt.CursorBeyondEOL)
  {
    MaxRightPos=0;
  }
  /* SKV$*/
}
/* IS $ */

/* $ 29.10.200 IS
     ������ � ����������� "��������� ������� �����" �
     "��������� ��������" ����� ����� �������� �� alt-shift-f9.
*/
void Editor::GetSavePosMode(int &SavePos, int &SaveShortPos)
{
   SavePos=EdOpt.SavePos;
   SaveShortPos=EdOpt.SaveShortPos;
}

// ����������� � �������� �������� ��������� "-1" ��� ���������,
// ������� �� ����� ������
void Editor::SetSavePosMode(int SavePos, int SaveShortPos)
{
   if(SavePos!=-1)
      EdOpt.SavePos=SavePos;
   if(SaveShortPos!=-1)
      EdOpt.SaveShortPos=SaveShortPos;
}
/* IS $ */

void Editor::EditorShowMsg(const char *Title,const char *Msg, const char* Name)
{
  Message(0,0,Title,Msg,Name);
  PreRedrawParam.Param1=(void *)Title;
  PreRedrawParam.Param2=(void *)Msg;
  PreRedrawParam.Param3=(void *)Name;
}

void Editor::PR_EditorShowMsg(void)
{
  Editor::EditorShowMsg((char*)PreRedrawParam.Param1,(char*)PreRedrawParam.Param2,(char*)PreRedrawParam.Param3);
}

#endif //!defined(EDITOR2)
