/*
findfile.cpp

����� (Alt-F7)

*/

/* Revision: 1.167 23.01.2005 $ */

/*
Modify:
  23.01.2005 SVS
    ! ���������� ������ ����� (��. 01916.search.txt)
  11.12.2004 WARP
    - ��������� � ���������� ESC/F10 � ������.
  09.12.2004 WARP
    ! ������� �� TRY/EXCEPT � ������� � fexcept � ������ ������ ������ (� ������ ��������� ��� ���� ����������).
  09.12.2004 WARP
    ! ��������� ������� ������ ��� ������� ��������� ����,
      ����� ������ ������ ����� �������� ������.
    ! ������ findfile.cpp (��. 01867.Search2.txt).
  08.12.2004 WARP
    ! ���� ��� ������ #1. ��������� 01864.FindFile.txt
  01.12.2004 WARP
    - ��������� ����� � ������� ���� ������ ����� ������ ��������� (��� ��������).
    ! ������� ������ �������������� ����� �� ��������� ������ (������� DM_ENABLEREDRAW)
  19.11.2004 WARP
    ! "����������" ���������� UpdateRequired ��� VMenu,
      ����� ��������������� ���������� ����� � �����.
  09.11.2004 WARP
    - �������� ���������������� � ������ ����� ��������� ����� �� ����� ������
  25.10.2004 SVS
    ! � �������� ������: ������� F3 �� �������� �������, Enter (��� �������� ������ [ View ]) - ��������.
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  14.06.2004 KM
    ! ������� �������� ������ ��� ������������� �������, � �������
      ������ ���������� ��������� �������� FILE_ATTRIBUTE_DIRECTORY
  08.06.2004 SVS
    ! ������ GetDriveType ������ �������� FAR_GetDriveType().
    ! ������ "DriveType==DRIVE_CDROM" �������� IsDriveTypeCDROM()
  02.03.2004 SVS
    - BugZ#1040 - ������������ �������������� ���������
    + ��������� ����������� ���� - ����� ����� ������� � ��������� ��������.
  19.11.2003 IS
    ! ������ NamesList �� ������� �������� ������� ������
  26.10.2003 KM
    ! ���� ���������� 16-������ �����, �� ����� ����� ������ ������
      0 �����: "00 00 00"
  23.10.2003 SVS
    ! ������ ������� ����� � �����
  16.10.2003 SVS
    ! Opt.FileSearchMode � Opt.FindFolders �������� � ��������� ��������� struct FindFileOptions
    + FindFileOptions.CollectFiles - �������� NamesList ��� ���������� (����� ���� F3 � ������� ����������� ������)
    ! ���� �� ��������� � Sleep(10) ������ � ������ ����������� ������
  15.10.2003 KM
    - ���������� ������������ "�������" ������
  10.10.2003 SVS
    - ��-�� ��������� ������� � ����������
       ����:  else if (Param1>=19 || Param1<=24)
      �� ������� ����� ����, ���������� �� ������������ ������ ����� �� ������ ��������� � ������
      (DI_EDIT <-> DI_FIXEDIT)
  09.10.2003 SVS
    ! � ������� ����������� ������ ������ "FindFile" ������� ����� ������ ������ "FindFileResult"
  05.10.2003 KM
    ! ������ � ����������� ������ ��������� � ������ 64-������ ����������.
    + � ����������� ������ ��������� �������� ��������� ����� � ������.
    + ������� [ ] Use filter, ���������� ����������� ������������
      �������� ������ ��� ������ ������������� ������ ������ FileFilter.
    + ������ Filter, ����������� ����������� ������ ��������.
    ! ��� ���������� ������� �������� ��� ���� � ������� ��������� ������
      ��������� � ����������� �� ������������� � �������:
      �����������, �������� ��� ���������� �������.
    + ���� ������������ ������ ��������, �� �� ����� ������ �������� �� ����
  24.09.2003 KM
    - ��������� ������ �� ���������� ����.
  22.09.2003 KM
    + �������� ����� 16-������� ����.
  20.09.2003 KM
    + ������� � ������ ����������� ������ ������ ANSI �������,
    � �� ���-�� �������� ���������� � ��������� � ������� ���
    ����, � � ������ ���.
  18.09.2003 KM
    ! ����������� ��������� ���������� ������
  12.09.2003 SVS
    ! ������� �������� ����� ��� GetPathRootOne
  20.06.2003 SVS
    ! _beginthread(WriteDialogData... ��������� ����� ������� �����-���������
    ! �������:
      DM_SETFOCUS �� 6 ������� ������������ ����� ������� ������ DN_DRAWDIALOGDONE
      �.�. �� ����������� �� �������� ������, �.�. DM_SETFOCUS ����� ���� � main-����
    ! �������� ����� ClevMutex (��� ������ �� ������ � ���, ��� "����� ��� �� ����� �������� ReleaseMutex()")
  13.06.2003 SVS
    + � ������ ������ ������ ��������� ����� "������ � ������������� ������"
      ("Search in symbolic links"). ��������� ������ ����� � �������������
      ������ ������� � �������� �������������. �� ��������� (��� �������
      ����) �� �������� ����� �������� ����� Opt.ScanJunction.
  14.05.2003 SVS
    + _ALGO()
  21.04.2003 SVS
    ! �� "return FALSE;", �� "DefDlgProc"
  07.04.2003 VVM
    ! ���� �� ����� ������ ������� ��������� ���� ����� - ��������.
      �� ���� �������� ��������� �� �������� �������. ��� �� � ����...
  15.03.2003 VVM
    - ���� ������ ���������� � �����, �� � DI_COMBOBOX ���� ������� � �����.
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
  04.02.2003 VVM
    ! ���������������� ������ � �������� ��� �������.
  26.01.2003 IS
    ! FAR_DeleteFile ������ DeleteFile, FAR_RemoveDirectory ������
      RemoveDirectory, ������� � ������ �� ������������ ��� ��������
      �������������� ������ � ���������.
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  19.01.2003 KM
    - ������� � ������ �� ������ ������� YaCat � ����������
      ������ "������ � �������".
    - ������������ ������� �� ���� � ������ ������� YaCat.
  03.12.2002 SVS
    - BugZ#680 - ������������������� ����������� �������� ������
  28.10.2002 SVS
    ! ��� ������ PrepareTable*() �������� UseTableName=TRUE.
      ���� ����������� �� BugZ#689
  14.10.2002 VVM
    ! ����� ������������� ������ ����� ���������. (bug # 590)
  03.10.2002 SVS
    - ��������� � ������� ����� (PluginMode ��� ���������)
  01.10.2002 SVS
    - BugZ#665 - ����� ������ [Drive]
  01.10.2002 SVS
    - BugZ#664 - ������ "&" � ������ ��������� � ������� ������
  01.10.2002 VVM
    ! ���������� ����� �� ������ [Go To] ��� ������ ���������� � ������,
      � �� ��� ����������.
  30.09.2002 SVS
    ! �������� ��������� ��������� DN_CTLCOLORDLGLIST - ����� �������� ������
      9-� ������!
  12.09.2002 SVS
    - BugZ#593 - ������ ��� ������� ������ �� ����
  31.08.2002 KM
    ! ��� ������ � ������� ����� ������ "Search in all non-removable drives"
      � "Search in all, except removable and network drives" ��
      ����� ������ � ������� �����������.
  14.08.2002 VVM
    ! ������ ����������� ������������ � �������� �������� �� ���� ������.
      ������� ��� �� �����...
  28.06.2002 VVM
    + ��� ������ ������ ����� ��������� �� ������ [New search]
      ��� ������ ���-������ ������ - ������ ��� �� [Go To], ���� ������
      �������� �� �������.
  02.06.2002 KM
    - ��������� ������ �� ����� ������. ����������� ���������.
  27.05.2002 VVM
    - ������� ���� - ��� ������ �� ������ ����� �� ������� �������.
  25.05.2002 IS
    ! ������ �������� � ConvertDate ������ ������ �� ���������
  24.05.2002 SVS
    + ������������ Numpad-������
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
  16.05.2002 SVS
    ! ����� ������ GetPathRoot � ���������� ����� GetPathRootOne
  15.05.2002 SKV
    + ����������� ���� � ��������� ��������
  06.05.2002 SVS
    ! ����� ������� ������� ����������������� ������ ����� ������ InitDialog()
  25.04.2002 IS
    ! ��������� const
  13.04.2002 KM
    ! ��������� ���� �� �������. ���445 ��� ���������� ���
      � 1347 �����.
  09.04.2002 SVS
    - BugZ#445 - �������������� ������� ������ ��������� �����
  07.04.2002 KM
    - ����� ������ ��������� ������������ ��� � �����������
      ������������ "||||" ������ ������ "������ � ����� �����".
    - ������������� �������� ������� ������ ��� ������
      ������ �����.
    - �������� ������� �� ������� �� ������ ��������� ������
      �� ����� ������� ��������� ������ ��������� �� ���������
      ������.
  05.04.2002 SVS
    ! MAX_READ -> Opt.PluginMaxReadData
  04.04.2002 KM
    - ��� ����� ����� �� ���������� � ������� �� �����������
      ��������� �������� "������ � �������".
  02.04.2002 KM
    + �� �������� ���������� �������� ����� �� ��������� ������.
    + ����� �����: � ������� ������ ��� ������ � ����� �����
      ����������� ����� ������.
  26.03.2002 DJ
    ! ScanTree::GetNextName() ��������� ������ ������ ��� ����� �����
  24.03.2002 KM
    - ������� ������������ �� ��������� ������ ������ ������
      � �������. ��� ������ �� �����, � ������� �������� ��
      ��������, �� ��������� ������ �������� �����, � �������
      ��� ����� ���� �������, �� ������ �� ��������� �� �����,
      ��� �� ���� ���������.
    ! ����������� ��������� ������ � ��������� ���������� ���������
      ��������� � ������ � ��������� 1.65 � 3 ����. � ���� ���
      ��� � ���. � ������ ����, ���� ��� ������ �� ����� � ��
      �������� � �������� �� ��� �������� � ������ ����������
      ���������� ������:
      ===
      �======================== Find file =========================�
      �C:\Work\FAR_SRC\Far\new\Debug\Plugins\                      �
      �  p_1.bat                                 0  24.03.02 04:31 �
      �  p1                                 Folder  24.03.02 04:31 �
      �  ProcList                           Folder  17.01.02 21:35 �
      ===
      ��� ���������� ��������� ������ � ��� �������� ����.
    ! ���������� � �������� �� ������ ����� ������ � ������,
      ���� ��� ����� �������� � ���� ����, � �� ������ ���
      OPIF_REALNAMES, � ��������� ������ � ����������� ������
      ���������� ���������� ���������� ������ ��������������
      ���������� ����� �� ������.

  21.03.2002 VVM
    + �������� ����� ��������� �� ������ ��� ��������������� "\"
  15.01.2002 VVM
    ! ����������� ������������ ����� ������� � ������
  15.03.2002 KM
    - ��������� ���� ����������� ��� ����������, �� � ����������
      ���� � ����� �������� ����� ��� ���������.
  12.02.2002 VVM
    + ����������� ������� AbortMessage()
  04.03.2002 DJ
    ! ������� Esc � ������� ������������� ���������� ������ ����� _��_
    ���������
  01.03.2002 SVS
    ! ���� ������ ���� ������� �������� ���������� ����� - FarMkTempEx
  21.02.2002 VVM
    ! ������� strcpy �� strncpy. � �� ������ ��� ������ � �������
      ������� �����.
    ! ������ ����� ��� ESC.
  21.02.2002 VVM
    ! ��� ������� �� ��������� ������ ESC ���������� ������ NO
  19.02.2002 VVM
    ! ������ ������ � �������. ����� ������ ���������...
  11.02.2002 SVS
    ! ����������������� ����� FarListItem � 0 (�������� ��� ���������
      ������� ��������� FarListItem)
  30.01.2002 VVM
    ! ������� ���� - ���������� hPluginMutex �� ����� ������ � �������
      �������. ����� �������� ����� ��� ��������� ���������� ����� �
      ������ ������ �� ������ �����-�� �������.
  23.01.2002 VVM
    + GetPluginFile() - �������� ���� ��� ��������� � ������ �������.
      � ������� �� ����������� ������� - ��������� ����������� �����
      � ������ ��� ��� SetDirectory()
  18.01.2002 VVM
    ! ��� ������ ��������� ������ ������� ��������� �������.
  17.01.2002 VVM
    ! �������� �������� � ������ ����� ����� ListBox, � �� � ���������.
      ������� ����������� �������� ���������
    ! ��������� ������ � ������� � ������ ������� � ������ -
      ���� OPIF_FINDFOLDERS � ������� ������� ���� ������������
  16.01.2002 VVM
    ! � ������� AddMenuRecord �� ���������� �������� Path, �� ��� ������...
      ����������� ���� � 236
  15.01.2002 VVM
    ! ����������� ������ � ������������ �������
    + ���������� � ���������� ��������� ������ � ��������� ���������.
  28.12.2001 SVS
    ! ������ � ������ ��������� �������� (��� ��������� union)
  18.12.2001 KM
    - > IS "p.s. ������� �� 1127 �����: ���� ������ �����-������ (�������?)
      ������, �� ��� ��������� ������ (������� "����� �����" � �������
      ������) ������ ������ ����� ��������� ��� �������, �� ���������
      �� ������.  ��������, � ����� "FROM_LEFT_2ND_BUTTON_PRESSED", �����
      ������ "����� �����" ������� "...PRESSED".... ������, � ������
      ������ � ����� ������� �� "FROM_LEFT_2ND_BUTTON_PRESSED", �
      "...PRESSED" (���-�� �� ��� ������������ ������ ������ �������)"
  16.12.2001 KM
    - �������-�� ������� ��� � ����������� ������ ������� �� �����
      ������ � �������. ���� ��������� � ���� ������������ ������
      ��� ������ � ������������ ������ - IsPluginGetsFile, �� ��� ���
      ����� ��� ��������� �� Dialog API, �� ��� ���� ������ ���� ������.
    + ��������� ��������������� � "Searching in:". ���� ���� ����� � ������,
      �� ������ �������� "Searching "text_to_find" in:".
    + ������� �������� �� ������������� ��������� ����� � ��������� ���
      ������� F4 �� ����� ������, ��� ���� ������� ������ ��� � �������
      (��������� ������).
      ��������: ��� ������� �� F4 �� ����� ������ �� ����� ��� �������� �
      ��������� ��������� ������� ���������� ���������� ���-����� ������
      ������ ������ (����� ��� ������ ��������, ����� ������ ����� :).
  11.12.2001 VVM
    - bugz#162 (* ������ ������ ��� ������ � ������ � ������ ��� �� �����, FAR �����,
         ��� ����� ������������ �� � ��� �� �������� � ��� ��� ������ ���
         �������� � ������ ����������� ������. *)
  11.12.2001 VVM
    - �� ������ ������ ��� �������� �����-�������
  03.12.2001 DJ
    - ���������� ����� ���� ������ � ������������
  01.12.2001 KM
    - [Bug#145] �-��... ����� ���������������� �� ����� �� ������ :(
      ���������� Param1 � Param2 ��� ���� ������...
    + ��������� � ��������� ���� ������ ���������� � ����� ������:
      "=== Find files: mask*.ext ==="
      ��� ����� ��� �� ������� ������� ������� ��� ���...
  23.11.2001 KM
    - ����! ������ ���� � ���� ������� :-(
      ��������� �������� �������� � �����������.
  23.11.2001 VVM
    ! ������� ���������, ���-�� ��������� ������� �� ����������� ���������.
  22.11.2001 KM
    - ������ ������, ��� DN_BTNCLICK �������� �� ����� "�������������"
      ����������, ������ � �������� ������� ��-�������, ������� �������
      ������� ENTER �� ������� Find ��� Cancel �� � ���� �� ���������.
    + �� ����� ������ ������ Esc - ������� ����������� ������ �� �������
      �������� (���� ����� ��������) ��� ������ ����� ���������������
      (������ �� ��� � ������� �� ����).
  22.11.2001 VVM
    ! ������������������� FindFolders ��� ����� ������.
      �� ������ ���� �� ������ ���� ��������� �������
      ��������� ����������� �����.
  15.11.2001 IS
    - ���� �������� ������, ����� ���� "�����" ������, ����� ������ ����������
    + ������ ������: �� ����� �������� ����� �����, ���� ���� �����
  13.11.2001 VVM
    ! ��. ������� ����� �� ������, ���� �� ������...
  26.10.2001 KM
    + ��������� ����� "������ ��������", ����������� ������ �� ������ �����, �� � ��������.
    - ���� ������� ������������� �������� �� ������� �� F3.
  20.10.2001 KM
    ! ��������� �� ������.
    ! �����-�� ��� ������� ������ �� ��������� ������ ��� ������ ������� �� ����,
      �� ������� ����� ������ � ������. ������.
  19.10.2001 KM
    - ��������� ������ � ��� ������� �����, ��������� ��������� ������� � memset.
    ! ������������� DIF_SEPARATOR2 ������ MakeSeparator.
    ! ��������� ������ [ New search ] ������� ����� enum VVM ��� ��������������.
  19.10.2001 VVM
    + ������� GoTo �� ������� ���������� ���������.������� ��� �� ��, ��� � � [ Panel ]
    ! FindFileArcIndex ������ ������������ ��� ��������� ���������� ��������� ������.
      �� ����� ���� � ������.
  19.10.2001 VVM
    ! ������� ����� 2. � ������ ��������� � ��������� �� KM � SVS ;)))
    ! �������� ������, �������� ���������, ����������� �����. ����� �� ������...
  19.10.2001 KM
    ! � ������ VVM ������� ���������� � ������ ������� LIST_INDEX_NONE �� ������ �������.
    - ��������� ����� �� ��������� ������, �� ��� �� ������� � ������� "�������� ����������",
      �� �������� ������������� � ������������� �����, ���� ��� � OPIF_REALNAMES.
    + ��������� SortItems ��� ���������� PluginPanelItem ��� ������ �� ��������� ������.
  18.10.2001 KM
    - ��� ����� ��� ������� ������� �� ������ "View", ����� ������ �� ���� �������.
    ! ��������� ��������� ��������� ������ � ���� ��� ������ ������.
  15.10.2001 VVM
    ! ������ ����������� ����� ��� ����-����. ���� ��������� ����-����� ������
      ����������� ��-�������.
  15.10.2001 KM
    - ��� ������� ��� ������� ������ � �������� ���� Temporary panel
      � Network browser. �� ���������� ���� ���������� ��������� � ����
      ��������.
    - �� ���������� ������� �� ���� ��� ����� �� �� ����� � ������.
    - ����� ���������� KEY_NUMPAD5 � ������ ��������� ������.
  13.10.2001 VVM
    ! ��� ��� ������ � ����-������.
    + ����� ������� - �������� ������. ���-�� ������ ����������� ������
      ����� ������� �� [New search]
  12.10.2001 VVM
    ! ��������� ������� ��� ������ ������. �����, ���
      INVALID_HANDLE_VALEU <> 0 ;)
  12.10.2001 VVM
    ! ��������� �������� ������. ����������� �� ���������� ����������,
      ������� ����� �������������. ������ ������� ������ ������ �����
      ������ � ����� ��� ����� ������.
  09.10.2001 VVM
    ! ��������� ������ - ��������� ����������� :))))
  07.10.2001 SVS
    ! ������ ����� �������������� ��������? ���� �������������� �� -
      ������ ����!
  07.10.2001 SVS
    ! ��������� ����������� ������� �������� Dialog
  01.10.2001 VVM
    ! ��������� ���� ���������� �� NULL
  01.10.2001 VVM
    ! ����� Alt-F7 � ������� � ������� �� �������� ���� � ������� ��������
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  24.09.2001 OT
    ������� ��� �������� ����� (F3,F4) �� ������
  13.09.2001 KM
    - �� ��������������� �����, ��������� � ������, ���� ����� ���������
      �� � ����� ������, � �� ������������.
  23.08.2001 VVM
    + � ������� ������ ������ � ��������� ��������� �� 20 �������� - ��� ����� ���������.
    + ������� TruncPathStr() ��� ��������...
  17.08.2001 KM
    ! ��� �������������� ���������� ����� �� ������ ������� "���������"
      ���������� �� ������� "��������� �", ������� ������ ��� ����� �����.
    + ��������� ������ "View" � ������ ������ ��� ������ ����� ������� ������.
    - ���������� ������� �� ������� ����� + � - � ��������� ������, ������
      � ������ ��������� ������������ ������ �� �������� �����.
  15.08.2001 KM
    ! ���������� �������� ������. 5-� �����
    + ��������� ����������� ��� ������ � �������:
      1) ������������� ����� ��������� � ������ � �����������
         � ������ �� F3 � F4;
      2) ��������� ������ �������, � ������� ��������� ���������
         ����� �� ��������� ������.
      3) ����������� ����� � ������� �� ������ �������, � ������� ��
         ��������� ������, ���� ������ ����������� � ������ OPIF_REALNAMES.
  11.08.2001 KM
    ! ����� �� ������� ��������� ���������������� ����
      � ������ ��� ���������� � ���� ������ ���������
      ���������.
  10.08.2001 KM
    + ��������� �������� ������� ������ ��� ��������� �������� �������.
  08.08.2001 KM
    ! ���������� �������� ������. 3-� �����
    ! ������� ������� ��������� ��� �� ������ ������������ (� �� ������)
      �������� ����� ��� �������� ������� ������ �� ����� �����: ������
      � ����� ����� ��������� ������� ��������� ��������� ������ ������ ����.
    ! ��������� (���� ��� ������� ANSI) ����� ������ ������.
  07.08.2001 IS
    ! ���������� ��������� � FarCharTable
  07.08.2001 SVS
    ! ������ FindSaveScr - �����, � ���������. � ��� ���� ����� ����� �������.
    ! ���� ���� ����� ������ (��� ��� ����), �� �������� ����� ������,
      ���... ��������� � �����������. ��� ����� ���������� �����, ����� ����
      ����� ��������.
    ! ��������� �� ����� ������ ����� ��������� ���������� ������� �����
      KEY_CTRLALTSHIFTPRESS � KEY_ALTF9
      (2KM: ��� Alt-F9 - ����� ����� ������� DN_RESIZECONSOLE)
    ! �� ����� ������ ���������/������� ��������� ����� �������:
      Alt-F9 � F11 :-)
    - �� � ����������... ��������� "������" :-) ��������� �����������
      ����� ����� F3/F4:
  01.08.2001 KM
    ! ���������� �������� ������. 2-� �����
    ! � ������ OT ������������� ���������
      ������ �� Mutex.
    - ������� ������� ���������� �� ����������.
      ���������:
        1. ���� ����� F3 ��� F4 �� ����� ������
           ������ CAS, �� ��� ������������
           ��������/���������� ����� ����� ����
           �� �������.
        2. ����� F3 � F4 �� �����������������
           ����������� ��� ��������.
        3. �� ������ �������� ���������� � �����
           ������.
  31.07.2001 KM
    ! ���������� �������� ������. 1-� �����
      ���������:
        1. ����� F3 � F4 �� �����������������
           ����������� ��� ��������.
        2. ��� ������ �� ������� ������ ������
           ����������� farexcpt.0xc0000005.
        3. �� ������ ��������� ���������������� ����,
           ��-�� ���� �� �������� ���������� � �����
           ������.
  27.07.2001 SVS
    ! �������� (�� ������� IS)
  24.07.2001 IS
    ! ������ �������� �� ' ' � '\t' �� ����� isspace
    ! ������ �������� �� '\n' � '\r' �� ����� iseol
  02.07.2001 IS
    ! FileMaskForFindFile.Set(NULL) -> FileMaskForFindFile.Free()
    + ������ �������������� ���������� '*' � ����� ����� ��� ������������
      �������� (��� ���� �������� ���������� ������).
  01.07.2001
    + ����� ������������ ����� ���������� ��� ������������ ���������� ������.
  25.06.2001 IS
    ! ��������� const
  25.06.2001 SVS
    ! ����� SEARCHSTRINGBUFSIZE
  23.06.2001 OT
    - ������������� �����������, ����� VC �� "������������" :)
  18.06.2001 SVS
    - ���������� ����������� ����������� ����� :-)
  18.06.2001 SVS
    - "�����������" � ������ - �������� ������� �� ��������� (� 706-� �����
      ���� ������ ���������)
  10.06.2001 IS
    + ������� ������� ��� ������� ������� � ������� ���������� ������ � ������.
  09.06.2001 IS
    ! ��� �������� � ���������� �� ������ �������, ���� �� ��� � ��� ���������.
      ��� ����� ���������� ����, ��� ��������� � ��������� ������ ��
      ������������.
  05.06.2001 SVS
    + ���������� ������� (��� ���� ������� �� ����� "[View]" ���������� ���)
  04.06.2001 OT
     �������� ��� "�������������" ���������� ������
  03.06.2001 SVS
    ! ��������� � ����� � ���������� UserData � VMenu
  30.05.2001 OT
    ! ��������� �������� �� 100% ����� ���� ��������� ������, ������� � ������� :(... �� ������ ������
  26.05.2001 OT
    ! ������� AltF7 � NFZ
  25.05.2001 DJ
    - ������ ���������� ���� ��� disabled �������
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  16.05.2001 DJ
    ! proof-of-concept
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 DJ
    * ���������, � �� ������ Search in archives �� ���������� ������
    * ������ ���� �������� :-)
  12.05.2001 DJ
    * ������ �� ��������������� �� ������ ������� ����� ����������
  10.05.2001 DJ
    + ��������� F6 �� ������/���������, ��������� �� Find files
  06.05.2001 DJ
    ! �������� #include
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  30.03.2001 SVS
    ! GetLogicalDrives ������� �� FarGetLogicalDrives() � ����� � �������
      �������� �� ��������� �������� "�������".
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  27.02.2001 VVM
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  14.12.2000 OT
    -  ���: ����� �� Alt-F7 � ����� ������� ������ ������/������
  11.11.2000 SVS
    ! FarMkTemp() - ������� (��� ������ - �� ������, �� ��� �� ������� :-(((
  11.11.2000 SVS
    ! ���������� ����������� FarMkTemp()
  21.10.2000 SVS
    ! ������� ��� ������ � FFFE-������.
  10.09.2000 SVS
    - ��������� ������� ������ ����������� ������!
  07.08.2000 KM
    - ���� � ������ ��� ������������ ������, ���� ����� ���������
      ������� �� ���� ������ ���������� ����� ������� ����� ��� ����
      ��� ���������� ������, ������ ������� ������ �������.
  05.08.2000 KM
    - ����������� �������� ������ � Alt-F7, ���� ������������ �� �����
      ������ �������� ������� ������ ��� �����.
  03.08.2000 KM
    + ��������� ����������� ������ �� "����� ������"
  01.08.2000 tran 1.03
    + |DIF_USELASTHISTORY
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

#include "findfile.hpp"
#include "ClevMutex.hpp"
#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "editor.hpp"
#include "fileview.hpp"
#include "fileedit.hpp"
#include "filelist.hpp"
#include "cmdline.hpp"
#include "chgprior.hpp"
#include "namelist.hpp"
#include "scantree.hpp"
#include "savescr.hpp"
#include "manager.hpp"
#include "scrbuf.hpp"
#include "CFileMask.hpp"
#include "filefilter.hpp"
#include "farexcpt.hpp"

#define DLG_HEIGHT 24
#define DLG_WIDTH 74
#define CHAR_TABLE_SIZE 5

#define LIST_DELTA  64
static DWORD LIST_INDEX_NONE = (DWORD)-1;

// ������ ��������� ������. ������ �� ������ �������� � ����.
static LPFINDLIST  FindList;
static DWORD       FindListCapacity;
static DWORD       FindListCount;
// ������ �������. ���� ���� ������ � ������, �� FindList->ArcIndex ��������� ����.
static LPARCLIST   ArcList;
static DWORD       ArcListCapacity;
static DWORD       ArcListCount;

static CriticalSection ffCS; // ����� �� ��������� ������ ��������� ������, ���� �� �� ���� ������
static CriticalSection statusCS; // ����� �� ������ � FindMessage/FindFileCount/FindDirCount ���� �� ������ ������


static DWORD FindFileArcIndex;
// ������������ ��� �������� ������ �� ��������� ������.
// ������ �������� �������� � ������ � ���� ��� ��������.
static DWORD FindExitIndex;
enum {FIND_EXIT_NONE, FIND_EXIT_SEARCHAGAIN, FIND_EXIT_GOTO, FIND_EXIT_PANEL};
static int FindExitCode;
//static char FindFileArcName[NM];

static char FindMask[NM],FindStr[SEARCHSTRINGBUFSIZE];
/* $ 30.07.2000 KM
   ��������� ���������� WholeWords ��� ������ �� ������� ����������
*/
static int SearchMode,CmpCase,WholeWords,SearchInArchives,SearchInSymLink,SearchHex;
/* KM $ */
static int FindFoldersChanged;
static int SearchFromChanged;
static int DlgWidth,DlgHeight;
static volatile int StopSearch,PauseSearch,SearchDone,LastFoundNumber,FindFileCount,FindDirCount,WriteDataUsed,PrepareFilesListUsed;
static char FindMessage[200],LastDirName[2*NM];
static int FindMessageReady,FindCountReady,FindPositionChanged;
static char PluginSearchPath[2*NM];
static HANDLE hDlg;
static int RecurseLevel;
static int BreakMainThread;
static int PluginMode;

static HANDLE hPluginMutex;

static int UseAllTables=FALSE,UseDecodeTable=FALSE,UseANSI=FALSE,UseUnicode=FALSE,TableNum=0,UseFilter=0;
static struct CharTableSet TableSet;

/* $ 01.07.2001 IS
   ������ "����� ������". ������ ��� ����� ������������ ��� �������� �����
   ����� �� ���������� � �������.
*/
static CFileMask FileMaskForFindFile;
/* IS $ */

/* $ 05.10.2003 KM
   ��������� �� ������ ������� ��������
*/
static FileFilter *Filter;
/* KM $*/

int _cdecl SortItems(const void *p1,const void *p2)
{
  PluginPanelItem *Item1=(PluginPanelItem *)p1;
  PluginPanelItem *Item2=(PluginPanelItem *)p2;
  char n1[NM*2],n2[NM*2];
  n1[0]=0;n2[0]=0;
  if (*Item1->FindData.cFileName)
    xstrncpy(n1,Item1->FindData.cFileName, sizeof(n1)-1);
  if (*Item2->FindData.cFileName)
    xstrncpy(n2,Item2->FindData.cFileName, sizeof(n1)-1);
  *(PointToName(n1))=0;
  *(PointToName(n2))=0;
  return LocalStricmp(n1,n2);
}

long WINAPI FindFiles::MainDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  Dialog* Dlg=(Dialog*)hDlg;
  char *FindText=MSG(MFindFileText),*FindHex=MSG(MFindFileHex),*FindCode=MSG(MFindFileCodePage);
  char DataStr[NM*2];

  switch(Msg)
  {
    case DN_INITDIALOG:
    {
      /* $ 21.09.2003 KM
         ������������ ��������� ������ ����� �������� ������
         � ����������� �� Dlg->Item[13].Selected
      */
      if (Dlg->Item[13].Selected)
      {
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,FALSE);
      }
      else
      {
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,FALSE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,TRUE);
      }

      Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,5,1);
      Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,6,1);
      /* KM $ */

      unsigned int W=Dlg->Item[7].X1-Dlg->Item[4].X1-5;
      if (strlen((Dlg->Item[13].Selected?FindHex:FindText))>W)
      {
        xstrncpy(DataStr,(Dlg->Item[13].Selected?FindHex:FindText),W-3);
        DataStr[W-4]=0;
        strcat(DataStr,"...");
      }
      else
        xstrncpy(DataStr,(Dlg->Item[13].Selected?FindHex:FindText),sizeof(DataStr)-1);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,4,(long)DataStr);

      W=Dlg->Item[0].X2-Dlg->Item[7].X1-3;
      if (strlen(FindCode)>W)
      {
        xstrncpy(DataStr,FindCode,W-3);
        DataStr[W-4]=0;
        strcat(DataStr,"...");
      }
      else
        xstrncpy(DataStr,FindCode,sizeof(DataStr)-1);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,7,(long)DataStr);

      /* ��������� ����������� ����� ���������� */
      UseAllTables=Opt.CharTable.AllTables;
      UseANSI=Opt.CharTable.AnsiTable;
      UseUnicode=Opt.CharTable.UnicodeTable;
      UseDecodeTable=((UseAllTables==0) && (UseUnicode==0) && (UseANSI==0) && (Opt.CharTable.TableNum>0));
      if (UseDecodeTable)
        TableNum=Opt.CharTable.TableNum-1;
      else
        TableNum=0;
      /* -------------------------------------- */

      if (UseAllTables)
        xstrncpy(TableSet.TableName,MSG(MFindFileAllTables),sizeof(TableSet.TableName)-1);
      else if (UseUnicode)
        xstrncpy(TableSet.TableName,"Unicode",sizeof(TableSet.TableName)-1);
      /* $ 20.09.2003 KM
         ������� ��������� ANSI �������
      */
      else if (UseANSI)
      {
        GetTable(&TableSet,TRUE,TableNum,UseUnicode);
        xstrncpy(TableSet.TableName,MSG(MGetTableWindowsText),sizeof(TableSet.TableName)-1);
      }
      /* KM $ */
      else if (!UseDecodeTable)
        xstrncpy(TableSet.TableName,MSG(MGetTableNormalText),sizeof(TableSet.TableName)-1);
      else
        PrepareTable(&TableSet,TableNum,TRUE);
      RemoveChar(TableSet.TableName,'&',TRUE);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,8,(long)TableSet.TableName);

      FindFoldersChanged = FALSE;
      SearchFromChanged=FALSE;

      if (Dlg->Item[21].Selected==1)
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,29,TRUE);
      else
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,29,FALSE);

      return TRUE;
    }
    case DN_LISTCHANGE:
    {
      if (Param1==8)
      {
        /* $ 20.09.2003 KM
           ������� ��������� ANSI �������
        */
        UseAllTables=(Param2==0);
        UseANSI=(Param2==3);
        UseUnicode=(Param2==4);
        UseDecodeTable=(Param2>=(CHAR_TABLE_SIZE+1));
        TableNum=Param2-(CHAR_TABLE_SIZE+1);
        if (UseAllTables)
          xstrncpy(TableSet.TableName,MSG(MFindFileAllTables),sizeof(TableSet.TableName)-1);
        else if (UseUnicode)
          xstrncpy(TableSet.TableName,"Unicode",sizeof(TableSet.TableName)-1);
        else if (UseANSI)
        {
          GetTable(&TableSet,TRUE,TableNum,UseUnicode);
          xstrncpy(TableSet.TableName,MSG(MGetTableWindowsText),sizeof(TableSet.TableName)-1);
        }
        else if (!UseDecodeTable)
          xstrncpy(TableSet.TableName,MSG(MGetTableNormalText),sizeof(TableSet.TableName)-1);
        else
          PrepareTable(&TableSet,TableNum,TRUE);
        /* KM $ */
      }
      return TRUE;
    }
    /* 22.11.2001 VVM
      ! ������������������� FindFolders ��� ����� ������.
        �� ������ ���� �� ������ ���� ��������� ������� */
    case DN_BTNCLICK:
    {
      Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
      char SearchFromRoot[128];
      struct FarDialogItemData ItemData;

      /* $ 23.11.2001 KM
         - ����! ������ ���� � ���� ������� :-(
           ��������� �������� �������� � �����������.
      */
      /* $ 22.11.2001 KM
         - ������ ������, ��� DN_BTNCLICK �������� �� ����� "�������������"
           ����������, ������ � �������� ������� ��-�������, ������� �������
           ������� ENTER �� ������� Find ��� Cancel �� � ���� �� ���������.
      */
      if (Param1==28 || Param1==31) // [ Find ] ��� [ Cancel ]
        return FALSE;
      else if (Param1==29) // [ Drive ]
      {
        IsRedrawFramesInProcess++;
        ActivePanel->ChangeDisk();
        // �� ��� �, ��� ����� ����� ������ ��������� ������
        // ����� ����� ��������.
        //FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
        FrameManager->ResizeAllFrame();
        IsRedrawFramesInProcess--;

        PrepareDriveNameStr(SearchFromRoot,sizeof(SearchFromRoot));
        ItemData.PtrLength=strlen(SearchFromRoot);
        ItemData.PtrData=SearchFromRoot;
        Dialog::SendDlgMessage(hDlg,DM_SETTEXT,21,(long)&ItemData);
        PluginMode=CtrlObject->Cp()->ActivePanel->GetMode()==PLUGIN_PANEL;
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,14,PluginMode?FALSE:TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,19,PluginMode?FALSE:TRUE);
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,20,PluginMode?FALSE:TRUE);
      }
      else if (Param1==30) // Filter
        Filter->Configure();
      else if (Param1>=19 && Param1<=24)
      {
        Dialog::SendDlgMessage(hDlg,DM_ENABLE,29,Param1==21?TRUE:FALSE);
        SearchFromChanged=TRUE;
      }
      else if (Param1==15) // [ ] Search for folders
        FindFoldersChanged = TRUE;
      else if (Param1==13) // [ ] Search for hexadecimal code
      {
        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

        /* $ 21.09.2003 KM
           ������������ ��������� ������ ����� �������� ������
           � ����������� �� �������������� �������� hex mode
        */
        int LenDataStr=sizeof(DataStr);
        if (Param2)
        {
          Transform((unsigned char *)DataStr,LenDataStr,Dlg->Item[5].Data,'X');
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,6,(long)DataStr);

          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,FALSE);
        }
        else
        {
          Transform((unsigned char *)DataStr,LenDataStr,Dlg->Item[6].Data,'S');
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,5,(long)DataStr);

          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,5,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,6,FALSE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,8,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,11,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,12,TRUE);
          Dialog::SendDlgMessage(hDlg,DM_ENABLE,15,TRUE);
        }
        /* KM $ */

        unsigned int W=Dlg->Item[7].X1-Dlg->Item[4].X1-5;
        if (strlen((Param2?FindHex:FindText))>W)
        {
          xstrncpy(DataStr,(Param2?FindHex:FindText),W-3);
          DataStr[W-4]=0;
          strcat(DataStr,"...");
        }
        else
          xstrncpy(DataStr,(Param2?FindHex:FindText),sizeof(DataStr)-1);
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,4,(long)DataStr);

        if (strlen(DataStr)>0)
        {
          int UnchangeFlag=Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,5,-1);
          Dialog::SendDlgMessage(hDlg,DM_EDITUNCHANGEDFLAG,6,UnchangeFlag);
        }

        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
      }
      return TRUE;
      /* KM $ */
      /* KM $ */
    }
    case DN_EDITCHANGE:
    {
      FarDialogItem &Item=*reinterpret_cast<FarDialogItem*>(Param2);

      if ((Param1==5) && (!FindFoldersChanged))
      // ������ "���������� �����"
      {
        BOOL Checked = (*Item.Data.Data)?FALSE:Opt.FindOpt.FindFolders;
        if (Checked)
          Dialog::SendDlgMessage(hDlg, DM_SETCHECK, 15, BSTATE_CHECKED);
        else
          Dialog::SendDlgMessage(hDlg, DM_SETCHECK, 15, BSTATE_UNCHECKED);
      }
      return TRUE;
    }
    /* VVM $ */
    /* $ 15.10.2003 KM
        ���������� "�������" �������
    */
    case DN_HOTKEY:
    {
      if (Param1==4)
      {
        if (Dlg->Item[13].Selected)
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,6,0);
        else
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,5,0);
        return FALSE;
      }
    }
    /* KM $ */
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}


static void ShowTruncateMessage(int IDMField,int MaxSize)
{
  char Buf1 [128];
  char Buf2 [128];

  *Buf1='\'';
  xstrncpy (Buf1+1, MSG(IDMField), sizeof(Buf1)-3);
  strcat(Buf1,"'");
  RemoveHighlights(Buf1);
  sprintf (Buf2, MSG(MEditInputSize2), MaxSize);
  Message(MSG_WARNING,1,MSG(MWarning),MSG(MEditInputSize1),Buf1,Buf2,MSG(MOk));
}


FindFiles::FindFiles()
{
  _ALGO(CleverSysLog clv("FindFiles::FindFiles()"));
  static char LastFindMask[NM]="*.*",LastFindStr[SEARCHSTRINGBUFSIZE];
  // ����������� ��������� � ����������� ����������
  static char SearchFromRoot[128];
  /* $ 30.07.2000 KM
     ��������� ���������� LastWholeWords ��� ������ �� ������� ����������
  */
  static int LastCmpCase=0,LastWholeWords=0,LastSearchInArchives=0,LastSearchInSymLink=-1,LastSearchHex=0;
  /* KM $ */
  int I;

  // �������� ������ �������
  Filter=new FileFilter;

  CmpCase=LastCmpCase;
  WholeWords=LastWholeWords;
  SearchInArchives=LastSearchInArchives;
  SearchHex=LastSearchHex;
  if(LastSearchInSymLink == -1)
    LastSearchInSymLink=Opt.ScanJunction;
  if (!RegVer)
    LastSearchInSymLink=0;
  SearchInSymLink=LastSearchInSymLink;
  SearchMode=Opt.FindOpt.FileSearchMode;
  xstrncpy(FindMask,LastFindMask,sizeof(FindMask)-1);
  xstrncpy(FindStr,LastFindStr,sizeof(FindStr)-1);
  BreakMainThread=0;

  xstrncpy(SearchFromRoot,MSG(MSearchFromRoot),sizeof(SearchFromRoot)-1);
  SearchFromRoot[sizeof(SearchFromRoot)-1]=0;


  FarList TableList;
  FarListItem *TableItem=(FarListItem *)xf_malloc(sizeof(FarListItem)*CHAR_TABLE_SIZE);
  TableList.Items=TableItem;
  TableList.ItemsNumber=CHAR_TABLE_SIZE;

  memset(TableItem,0,sizeof(FarListItem)*CHAR_TABLE_SIZE);
  xstrncpy(TableItem[0].Text,MSG(MFindFileAllTables),sizeof(TableItem[0].Text)-1);
  TableItem[1].Flags=LIF_SEPARATOR;
  xstrncpy(TableItem[2].Text,MSG(MGetTableNormalText),sizeof(TableItem[2].Text)-1);
  xstrncpy(TableItem[3].Text,MSG(MGetTableWindowsText),sizeof(TableItem[3].Text)-1);
  xstrncpy(TableItem[4].Text,"Unicode",sizeof(TableItem[4].Text)-1);

  for (I=0;;I++)
  {
    CharTableSet cts;
    int RetVal=FarCharTable(I,(char *)&cts,sizeof(cts));
    if (RetVal==-1)
      break;

    if (I==0)
    {
      TableItem=(FarListItem *)xf_realloc(TableItem,sizeof(FarListItem)*(CHAR_TABLE_SIZE+1));
      if (TableItem==NULL)
        return;
      memset(&TableItem[CHAR_TABLE_SIZE],0,sizeof(FarListItem));
      TableItem[CHAR_TABLE_SIZE].Flags=LIF_SEPARATOR;
      TableList.Items=TableItem;
      TableList.ItemsNumber++;
    }

    TableItem=(FarListItem *)xf_realloc(TableItem,sizeof(FarListItem)*(I+CHAR_TABLE_SIZE+2));
    if (TableItem==NULL)
      return;
    memset(&TableItem[I+CHAR_TABLE_SIZE+1],0,sizeof(FarListItem));
    xstrncpy(TableItem[I+CHAR_TABLE_SIZE+1].Text,cts.TableName,sizeof(TableItem[I+CHAR_TABLE_SIZE+1].Text)-1);
    RemoveChar(TableItem[I+CHAR_TABLE_SIZE+1].Text,'&',TRUE);
    TableList.Items=TableItem;
    TableList.ItemsNumber++;
  }

  FindList = NULL;
  ArcList = NULL;
  hPluginMutex=CreateMutex(NULL,FALSE,NULL);

  do
  {
    ClearAllLists();

    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
    PluginMode=ActivePanel->GetMode()==PLUGIN_PANEL && ActivePanel->IsVisible();

    PrepareDriveNameStr(SearchFromRoot,sizeof(SearchFromRoot));

    const char *MasksHistoryName="Masks",*TextHistoryName="SearchText";
    const char *HexMask="HH HH HH HH HH HH HH HH HH HH HH";

    /* $ 30.07.2000 KM
       �������� ����� checkbox "Whole words" � ������ ������
    */
    static struct DialogData FindAskDlgData[]=
    {
      /* 00 */DI_DOUBLEBOX,3,1,DLG_WIDTH,DLG_HEIGHT-2,0,0,0,0,(char *)MFindFileTitle,
      /* 01 */DI_TEXT,5,2,0,0,0,0,0,0,(char *)MFindFileMasks,
      /* 02 */DI_EDIT,5,3,72,3,1,(DWORD)MasksHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,0,"",
      /* 03 */DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR2,0,"",
      /* 04 */DI_TEXT,5,5,0,0,0,0,0,0,"",
      /* 05 */DI_EDIT,5,6,36,6,0,(DWORD)TextHistoryName,DIF_HISTORY,0,"",
      /* 06 */DI_FIXEDIT,5,6,36,6,0,(DWORD)HexMask,DIF_MASKEDIT,0,"",
      /* 07 */DI_TEXT,40,5,0,0,0,0,0,0,"",
      /* 08 */DI_COMBOBOX,40,6,72,16,0,0,DIF_DROPDOWNLIST|DIF_LISTNOAMPERSAND,0,"",
      /* 09 */DI_TEXT,3,7,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 10 */DI_VTEXT,38,4,0,0,0,0,DIF_BOXCOLOR,0,"\xD1\xB3\xB3\xC1",
      /* 11 */DI_CHECKBOX,5,8,0,0,0,0,0,0,(char *)MFindFileCase,
      /* 12 */DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MFindFileWholeWords,
      /* 13 */DI_CHECKBOX,5,10,0,0,0,0,0,0,(char *)MSearchForHex,
      /* 14 */DI_CHECKBOX,40,8,0,0,0,0,0,0,(char *)MFindArchives,
      /* 15 */DI_CHECKBOX,40,9,0,0,0,0,0,0,(char *)MFindFolders,
      /* 16 */DI_CHECKBOX,40,10,0,0,0,0,0,0,(char *)MFindSymLinks,
      /* 17 */DI_TEXT,3,11,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR2,0,"",
      /* 18 */DI_VTEXT,38,7,0,0,0,0,DIF_BOXCOLOR,0,"\xC5\xB3\xB3\xB3\xCF",
      /* 19 */DI_RADIOBUTTON,5,12,0,0,0,0,DIF_GROUP,0,(char *)MSearchAllDisks,
      /* 20 */DI_RADIOBUTTON,5,13,0,0,0,1,0,0,(char *)MSearchAllButNetwork,
      /* 21 */DI_RADIOBUTTON,5,14,0,0,0,1,0,0,"",
      /* 22 */DI_RADIOBUTTON,5,15,0,0,0,0,0,0,(char *)MSearchFromCurrent,
      /* 23 */DI_RADIOBUTTON,5,16,0,0,0,0,0,0,(char *)MSearchInCurrent,
      /* 24 */DI_RADIOBUTTON,5,17,0,0,0,0,0,0,(char *)MSearchInSelected,
      /* 25 */DI_TEXT,3,18,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 26 */DI_CHECKBOX,5,19,0,0,0,0,0,0,(char *)MFindUseFilter,
      /* 27 */DI_TEXT,3,20,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 28 */DI_BUTTON,0,21,0,0,0,0,DIF_CENTERGROUP,1,(char *)MFindFileFind,
      /* 29 */DI_BUTTON,0,21,0,0,0,0,DIF_CENTERGROUP,0,(char *)MFindFileDrive,
      /* 30 */DI_BUTTON,0,21,0,0,0,0,DIF_CENTERGROUP,0,(char *)MFindFileSetFilter,
      /* 31 */DI_BUTTON,0,21,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel,
    };
    /* KM $ */
    FindAskDlgData[21].Data=SearchFromRoot;

    MakeDialogItems(FindAskDlgData,FindAskDlg);


    if (!*FindStr)
      FindAskDlg[15].Selected=Opt.FindOpt.FindFolders;
    for(I=19; I <= 24; ++I)
      FindAskDlg[I].Selected=0;
    FindAskDlg[19+SearchMode].Selected=1;

    {
      if (PluginMode)
      {
        struct OpenPluginInfo Info;
        HANDLE hPlugin=ActivePanel->GetPluginHandle();
        CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
        /* $ 14.05.2001 DJ
           ���������, � �� ������
        */
        if ((Info.Flags & OPIF_REALNAMES)==0)
          FindAskDlg[14].Flags |= DIF_DISABLE;
        /* DJ $ */
        if (FindAskDlg[19].Selected || FindAskDlg[20].Selected)
        {
          FindAskDlg[19].Selected=FindAskDlg[20].Selected=0;
          FindAskDlg[21].Selected=1;
        }
        FindAskDlg[19].Flags=FindAskDlg[20].Flags|=DIF_DISABLE;
      }
    }

    if (!RegVer || PluginMode)
    {
      FindAskDlg[16].Selected=0;
      FindAskDlg[16].Flags|=DIF_DISABLE;
    }
    else
      FindAskDlg[16].Selected=SearchInSymLink;

    /* $ 14.05.2001 DJ
       �� �������� �������, ���� ������ ������ � �������
    */
    if (!(FindAskDlg[14].Flags & DIF_DISABLE))
      FindAskDlg[14].Selected=SearchInArchives;
    /* DJ $ */

    xstrncpy(FindAskDlg[2].Data,FindMask,sizeof(FindAskDlg[2].Data)-1);

    /* $ 26.10.2003 KM */
    if (SearchHex)
      xstrncpy(FindAskDlg[6].Data,FindStr,sizeof(FindAskDlg[6].Data)-1);
    else
      xstrncpy(FindAskDlg[5].Data,FindStr,sizeof(FindAskDlg[5].Data)-1);
    /* KM $ */

    FindAskDlg[11].Selected=CmpCase;
    FindAskDlg[12].Selected=WholeWords;
    FindAskDlg[13].Selected=SearchHex;

    // ������������ ������. KM
    FindAskDlg[26].Selected=UseFilter;

    while (1)
    {
      int ExitCode;
      {
        FindAskDlg[8].ListItems=&TableList;

        Dialog Dlg(FindAskDlg,sizeof(FindAskDlg)/sizeof(FindAskDlg[0]),MainDlgProc);

        Dlg.SetHelp("FindFile");
        Dlg.SetPosition(-1,-1,DLG_WIDTH+4,DLG_HEIGHT);
        Dlg.Process();
        ExitCode=Dlg.GetExitCode();
      }

      if (ExitCode!=28)
      {
        xf_free(TableItem);
        CloseHandle(hPluginMutex);
        return;
      }

      /* ����������� ������������� ���������� */
      Opt.CharTable.AllTables=UseAllTables;
      Opt.CharTable.AnsiTable=UseANSI;
      Opt.CharTable.UnicodeTable=UseUnicode;
      if (UseDecodeTable)
        Opt.CharTable.TableNum=TableNum+1;
      else
        Opt.CharTable.TableNum=0;
      /****************************************/

      /* $ 01.07.2001 IS
         �������� ����� �� ������������
      */
      if(!*FindAskDlg[2].Data)             // ���� ������ � ������� �����,
         strcpy(FindAskDlg[2].Data, "*"); // �� �������, ��� ����� ���� "*"

      if(FileMaskForFindFile.Set(FindAskDlg[2].Data, FMF_ADDASTERISK))
           break;
      /* IS $ */
    }

    CmpCase=FindAskDlg[11].Selected;
    /* $ 30.07.2000 KM
       ��������� ����������
    */
    WholeWords=FindAskDlg[12].Selected;
    /* KM $ */
    SearchHex=FindAskDlg[13].Selected;
    SearchInArchives=FindAskDlg[14].Selected;
    if (FindFoldersChanged)
      Opt.FindOpt.FindFolders=FindAskDlg[15].Selected;

    if (RegVer && !PluginMode)
      SearchInSymLink=FindAskDlg[16].Selected;

    // ��������� ������� ������������� �������. KM
    UseFilter=FindAskDlg[26].Selected;

    /* $ 14.12.2000 OT */

    if (strlen (FindAskDlg[2].Data) > sizeof(FindMask) )
      ShowTruncateMessage(MFindFileMasks,sizeof(FindMask)-1);

    xstrncpy(FindMask,*FindAskDlg[2].Data ? FindAskDlg[2].Data:"*",sizeof(FindMask)-1);

    if (strlen((SearchHex)?FindAskDlg[5].Data:FindAskDlg[6].Data) > sizeof(FindStr))
      ShowTruncateMessage(MFindFileText,sizeof(FindStr)-1);

    /* $ 26.10.2003 KM */
    if (SearchHex)
    {
      xstrncpy(FindStr,FindAskDlg[6].Data,sizeof(FindStr)-1);
      RemoveTrailingSpaces((char *)FindStr);
    }
    else
      xstrncpy(FindStr,FindAskDlg[5].Data,sizeof(FindStr)-1);
    /* KM $ */

    /* OT $ */

    if (*FindStr)
    {
      xstrncpy(GlobalSearchString,FindStr,sizeof(GlobalSearchString)-1);
      GlobalSearchCase=CmpCase;
      /* $ 30.07.2000 KM
         ��������� ����������
      */
      GlobalSearchWholeWords=WholeWords;
      /* KM $ */
      GlobalSearchHex=SearchHex;
    }
    if (FindAskDlg[19].Selected)
      SearchMode=SEARCH_ALL;
    if (FindAskDlg[20].Selected)
      SearchMode=SEARCH_ALL_BUTNETWORK;
    if (FindAskDlg[21].Selected)
      SearchMode=SEARCH_ROOT;
    if (FindAskDlg[22].Selected)
      SearchMode=SEARCH_FROM_CURRENT;
    if (FindAskDlg[23].Selected)
      SearchMode=SEARCH_CURRENT_ONLY;
    if (FindAskDlg[24].Selected)
        SearchMode=SEARCH_SELECTED;
    if (SearchFromChanged)
    {
      Opt.FindOpt.FileSearchMode=SearchMode;
    }
    LastCmpCase=CmpCase;
    /* $ 30.07.2000 KM
       ��������� ����������
    */
    LastWholeWords=WholeWords;
    /* KM $ */
    LastSearchHex=SearchHex;
    LastSearchInArchives=SearchInArchives;
    LastSearchInSymLink=SearchInSymLink;
    xstrncpy(LastFindMask,FindMask,sizeof(LastFindMask)-1);
    xstrncpy(LastFindStr,FindStr,sizeof(LastFindStr)-1);
    if (*FindStr)
      Editor::SetReplaceMode(FALSE);
  } while (FindFilesProcess());
  CloseHandle(hPluginMutex);
  xf_free(TableItem);
}


FindFiles::~FindFiles()
{
  /* $ 02.07.2001 IS
     ��������� ������.
  */
  FileMaskForFindFile.Free();
  /* IS $ */
  ClearAllLists();
  ScrBuf.ResetShadow();

  // ��������� ������ �������
  if(Filter)
    delete Filter;
}

int FindFiles::GetPluginFile(DWORD ArcIndex, struct PluginPanelItem *PanelItem,
                             char *DestPath, char *ResultName)
{
  _ALGO(CleverSysLog clv("FindFiles::GetPluginFile()"));
  HANDLE hPlugin = ArcList[ArcIndex].hPlugin;
  char SaveDir[NM];
  struct OpenPluginInfo Info;
  CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
  xstrncpy(SaveDir,Info.CurDir,sizeof(SaveDir)-1);
  AddEndSlash(SaveDir);

  CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_SILENT|OPM_FIND);
  SetPluginDirectory(ArcList[ArcIndex].RootPath,hPlugin);
  SetPluginDirectory(PanelItem->FindData.cFileName,hPlugin);

  PluginPanelItem NewItem = *PanelItem;
  char *FileName = PointToName(RemovePseudoBackSlash(NewItem.FindData.cFileName));
  if (FileName != NewItem.FindData.cFileName)
    xstrncpy(NewItem.FindData.cFileName, FileName, sizeof(NewItem.FindData.cFileName));
  int Result = CtrlObject->Plugins.GetFile(hPlugin,&NewItem,DestPath,ResultName,OPM_SILENT|OPM_FIND);

  CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_SILENT|OPM_FIND);
//  SetPluginDirectory(ArcList[ArcIndex].RootPath,hPlugin);
  SetPluginDirectory(SaveDir,hPlugin);
  return(Result);
}

long WINAPI FindFiles::FindDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  int Result;
  Dialog* Dlg=(Dialog*)hDlg;
  VMenu *ListBox=Dlg->Item[1].ListPtr;

  CriticalSectionLock Lock(Dlg->CS);

  switch(Msg)
  {
    case DN_DRAWDIALOGDONE:
    {
      Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
      // ���������� ����� �� ������ [Go To]
      if((FindDirCount || FindFileCount) && !FindPositionChanged)
      {
        FindPositionChanged=TRUE;
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,6/* [Go To] */,0);
      }
//      else
//        ScrBuf.Flush();
      return TRUE;
    }
/*
    case DN_HELP: // � ������ ������ �������� ����� ������, ���... ��������� � �����������
    {
      return !SearchDone?NULL:Param2;
    }
*/
    case DN_MOUSECLICK:
    {
      /* $ 07.04.2003 VVM
         ! ���� �� ����� ������ ������� ��������� ���� ����� - ��������.
         �� ���� �������� ��������� �� �������� �������. ��� �� � ����... */
      SMALL_RECT drect,rect;
      int Ret = FALSE;
      Dialog::SendDlgMessage(hDlg,DM_GETDLGRECT,0,(long)&drect);
      Dialog::SendDlgMessage(hDlg,DM_GETITEMPOSITION,1,(long)&rect);
      if (Param1==1 && ((MOUSE_EVENT_RECORD *)Param2)->dwMousePosition.X<drect.Left+rect.Right)
      {
        Ret = FindDlgProc(hDlg,DN_BTNCLICK,6,0); // emulates a [ Go to ] button pressing
      }
      /* VVM $ */
      return Ret;
    }

    case DN_KEY:
    {
      if (!StopSearch && ((Param2==KEY_ESC) || (Param2 == KEY_F10)) )
      {
        PauseSearch=TRUE;
        IsProcessAssignMacroKey++; // �������� ���� �������
                                   // �.�. � ���� ������� ������ ������ Alt-F9!
        int LocalRes=TRUE;
        if (Opt.Confirm.Esc)
          LocalRes=AbortMessage();
        IsProcessAssignMacroKey--;
        PauseSearch=FALSE;
        StopSearch=LocalRes;

        return TRUE;
      }

      if (!ListBox)
      {
        return TRUE;
      }

      if(Param2 == KEY_LEFT || Param2 == KEY_RIGHT)
        FindPositionChanged = TRUE;

      // �������� ����.������� ����� �����������.
      if(Param2 == KEY_CTRLALTSHIFTPRESS || Param2 == KEY_ALTF9)
      {
        IsProcessAssignMacroKey--;
        FrameManager->ProcessKey(Param2);
        IsProcessAssignMacroKey++;
        return TRUE;
      }

      if (Param1==9 && (Param2==KEY_RIGHT || Param2==KEY_TAB)) // [ Stop ] button
      {
        FindPositionChanged = TRUE;
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,5/* [ New search ] */,0);
        return TRUE;
      }
      else if (Param1==5 && (Param2==KEY_LEFT || Param2==KEY_SHIFTTAB)) // [ New search ] button
      {
        FindPositionChanged = TRUE;
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,9/* [ Stop ] */,0);
        return TRUE;
      }
      else if (Param2==KEY_UP || Param2==KEY_DOWN || Param2==KEY_PGUP ||
               Param2==KEY_PGDN || Param2==KEY_HOME || Param2==KEY_END ||
               Param2==KEY_MSWHEEL_UP || Param2==KEY_MSWHEEL_DOWN)
      {
        ListBox->ProcessKey(Param2);
        return TRUE;
      }
      else if (Param2==KEY_F3 || Param2==KEY_NUMPAD5 || Param2==KEY_SHIFTNUMPAD5 || Param2==KEY_F4 ||
               Param2==KEY_ENTER && Dialog::SendDlgMessage(hDlg,DM_GETFOCUS,0,0) == 7
              )
      {
        if (ListBox->GetItemCount()==0)
        {
          return TRUE;
        }

        if(Param2==KEY_ENTER && Dialog::SendDlgMessage(hDlg,DM_GETFOCUS,0,0) == 7)
          Param2=KEY_F3;

        ffCS.Enter ();

        DWORD ItemIndex = (DWORD)ListBox->GetUserData(NULL, 0);
        if (ItemIndex != LIST_INDEX_NONE)
        {
          int RemoveTemp=FALSE;
          int ClosePlugin=FALSE; // ������� ���� ���������, ���� �������.
          char SearchFileName[NM];
          char TempDir[NM];

          if (FindList[ItemIndex].FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
          {
            ffCS.Leave ();

            return TRUE;
          }
          char *FileName=FindList[ItemIndex].FindData.cFileName;
          // FindFileArcIndex ������ ����� ������������
          // �� ����� ���� ��� ������.
          if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
              (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
          {
            char *FindArcName = ArcList[FindList[ItemIndex].ArcIndex].ArcName;
            if (ArcList[FindList[ItemIndex].ArcIndex].hPlugin == INVALID_HANDLE_VALUE)
            {
              char *Buffer=new char[Opt.PluginMaxReadData];
              FILE *ProcessFile=fopen(FindArcName,"rb");
              if (ProcessFile==NULL)
              {
                delete[] Buffer;

                ffCS.Leave ();
                return TRUE;
              }
              int ReadSize=fread(Buffer,1,Opt.PluginMaxReadData,ProcessFile);
              fclose(ProcessFile);

              int SavePluginsOutput=DisablePluginsOutput;
              DisablePluginsOutput=TRUE;

              WaitForSingleObject(hPluginMutex,INFINITE);
              ArcList[FindList[ItemIndex].ArcIndex].hPlugin = CtrlObject->Plugins.OpenFilePlugin(FindArcName,(unsigned char *)Buffer,ReadSize);
              ReleaseMutex(hPluginMutex);

              DisablePluginsOutput=SavePluginsOutput;

              delete[] Buffer;

              if (ArcList[FindList[ItemIndex].ArcIndex].hPlugin == (HANDLE)-2 ||
                  ArcList[FindList[ItemIndex].ArcIndex].hPlugin == INVALID_HANDLE_VALUE)
              {
                ArcList[FindList[ItemIndex].ArcIndex].hPlugin = INVALID_HANDLE_VALUE;

                ffCS.Leave ();
                return TRUE;
              }
              ClosePlugin = TRUE;
            }

            PluginPanelItem FileItem;
            memset(&FileItem,0,sizeof(FileItem));
            FileItem.FindData=FindList[ItemIndex].FindData;
            FarMkTempEx(TempDir); // � �������� �� NULL???
            CreateDirectory(TempDir, NULL);

//            if (!CtrlObject->Plugins.GetFile(ArcList[FindList[ItemIndex].ArcIndex].hPlugin,&FileItem,TempDir,SearchFileName,OPM_SILENT|OPM_FIND))
            WaitForSingleObject(hPluginMutex,INFINITE);
            if (!GetPluginFile(FindList[ItemIndex].ArcIndex,&FileItem,TempDir,SearchFileName))
            {
              FAR_RemoveDirectory(TempDir);
              if (ClosePlugin)
              {
                CtrlObject->Plugins.ClosePlugin(ArcList[FindList[ItemIndex].ArcIndex].hPlugin);
                ArcList[FindList[ItemIndex].ArcIndex].hPlugin = INVALID_HANDLE_VALUE;
              }
              ReleaseMutex(hPluginMutex);

              ffCS.Leave ();
              return FALSE;
            }
            else
            {
              if (ClosePlugin)
              {
                CtrlObject->Plugins.ClosePlugin(ArcList[FindList[ItemIndex].ArcIndex].hPlugin);
                ArcList[FindList[ItemIndex].ArcIndex].hPlugin = INVALID_HANDLE_VALUE;
              }
              ReleaseMutex(hPluginMutex);
            }
            RemoveTemp=TRUE;
          }
          else
            xstrncpy(SearchFileName,FindList[ItemIndex].FindData.cFileName,sizeof(SearchFileName)-1);

          DWORD FileAttr;
          if ((FileAttr=GetFileAttributes(SearchFileName))!=(DWORD)-1)
          {
            char OldTitle[512];
            GetConsoleTitle(OldTitle,sizeof(OldTitle));

            if (Param2==KEY_F3 || Param2==KEY_NUMPAD5 || Param2==KEY_SHIFTNUMPAD5)
            {
              int ListSize=ListBox->GetItemCount();
              NamesList ViewList;
              // ������� ��� �����, ������� ����� �������� �����...
              if(Opt.FindOpt.CollectFiles)
              {
                DWORD Index;
                for (int I=0;I<ListSize;I++)
                {
                  Index = (DWORD)ListBox->GetUserData(NULL, 0, I);
                  LPFINDLIST PtrFindList=FindList+Index;
                  if ((Index != LIST_INDEX_NONE) &&
                      ((PtrFindList->ArcIndex == LIST_INDEX_NONE) ||
                       (ArcList[PtrFindList->ArcIndex].Flags & OPIF_REALNAMES)))
                  {
                    // �� ��������� ����� � ������� � OPIF_REALNAMES
                    if (*PtrFindList->FindData.cFileName && !(PtrFindList->FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
                      ViewList.AddName(PtrFindList->FindData.cFileName);
                  } /* if */
                } /* for */
                ViewList.SetCurName(FindList[ItemIndex].FindData.cFileName);
              }
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,FALSE,0);
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
              {
                FileViewer ShellViewer (SearchFileName,FALSE,FALSE,FALSE,-1,NULL,(FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE)?NULL:(Opt.FindOpt.CollectFiles?&ViewList:NULL));
                ShellViewer.SetDynamicallyBorn(FALSE);
                ShellViewer.SetEnableF6(TRUE);
                // FindFileArcIndex ������ ����� ������������
                // �� ����� ���� ��� ������.
                if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
                    (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
                  ShellViewer.SetSaveToSaveAs(TRUE);
                //!Mutex.Unlock();

                ffCS.Leave (); // ����� ����� �����������, ���� �� ��� ������ �����

                IsProcessVE_FindFile++;
                FrameManager->EnterModalEV();
                FrameManager->ExecuteModal();
                FrameManager->ExitModalEV();
                IsProcessVE_FindFile--;

                ffCS.Enter ();

                // ���������� ���������� �����
                FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
              }

              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
            }
            else
            {
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,FALSE,0);
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
              {
/* $ 14.08.2002 VVM
  ! ����-��� �������� �� ������ ������������� � �������� ��������.
    � ���������, ������� �� ��� �� �������� ������
                int FramePos=FrameManager->FindFrameByFile(MODALTYPE_EDITOR,SearchFileName);
                int SwitchTo=FALSE;
                if (FramePos!=-1)
                {
                  if (!(*FrameManager)[FramePos]->GetCanLoseFocus(TRUE) ||
                      Opt.Confirm.AllowReedit)
                  {
                    char MsgFullFileName[NM];
                    xstrncpy(MsgFullFileName,SearchFileName,sizeof(MsgFullFileName)-1);
                    int MsgCode=Message(0,2,MSG(MFindFileTitle),
                          TruncPathStr(MsgFullFileName,ScrX-16),
                          MSG(MAskReload),
                          MSG(MCurrent),MSG(MNewOpen));
                    if (MsgCode==0)
                    {
                      SwitchTo=TRUE;
                    }
                    else if (MsgCode==1)
                    {
                      SwitchTo=FALSE;
                    }
                    else
                    {
                      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
                      Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
                      return TRUE;
                    }
                  }
                  else
                  {
                    SwitchTo=TRUE;
                  }
                }
                if (SwitchTo)
                {
                  (*FrameManager)[FramePos]->SetCanLoseFocus(FALSE);
                  (*FrameManager)[FramePos]->SetDynamicallyBorn(FALSE);
                  FrameManager->ActivateFrame(FramePos);
                  IsProcessVE_FindFile++;
                  FrameManager->EnterModalEV();
                  FrameManager->ExecuteModal ();
                  FrameManager->ExitModalEV();
//                  FrameManager->ExecuteNonModal();
                  IsProcessVE_FindFile--;
                  // ���������� ���������� �����
                  FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
                }
                else
*/
                {
                  FileEditor ShellEditor (SearchFileName,FALSE,FALSE);
                  ShellEditor.SetDynamicallyBorn(FALSE);
                  ShellEditor.SetEnableF6 (TRUE);
                  // FindFileArcIndex ������ ����� ������������
                  // �� ����� ���� ��� ������.
                  if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
                      (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
                    ShellEditor.SetSaveToSaveAs(TRUE);

                  ffCS.Leave (); // ����� ����� �����������, ���� �� ��� ������ �����
                  IsProcessVE_FindFile++;
                  FrameManager->EnterModalEV();
                  FrameManager->ExecuteModal ();
                  FrameManager->ExitModalEV();
                  IsProcessVE_FindFile--;

                  ffCS.Enter ();
                  // ���������� ���������� �����
                  FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
                }
              }
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
            }
            SetConsoleTitle(OldTitle);
          }
          if (RemoveTemp)
            DeleteFileWithFolder(SearchFileName);
        }

        ffCS.Leave ();

        return TRUE;
      }
      Result = Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
      return Result;
    }
    case DN_BTNCLICK:
    {

      FindPositionChanged = TRUE;

      if (Param1==5) // [ New search ] button pressed
      {
        StopSearch=TRUE;
        FindExitCode=FIND_EXIT_SEARCHAGAIN;
        return FALSE;
      }
      else if (Param1==9) // [ Stop ] button pressed
      {
        if (StopSearch)
          return FALSE;
        StopSearch=TRUE;
        return TRUE;
      }
      else if (Param1==6) // [ Goto ] button pressed
      {
        if (!ListBox)
        {
          return FALSE;
        }

        // ������� ����� ������ ��� �� ����� ������ �� �������.
        // ������� ������ ��� [ Panel ]
        if (ListBox->GetItemCount()==0)
        {
          return (TRUE);
        }
        FindExitIndex = (DWORD)ListBox->GetUserData(NULL, 0);
        if (FindExitIndex != LIST_INDEX_NONE)
          FindExitCode = FIND_EXIT_GOTO;
        return (FALSE);
      }
      else if (Param1==7) // [ View ] button pressed
      {
        FindDlgProc(hDlg,DN_KEY,1,KEY_F3);
        return TRUE;
      }
      else if (Param1==8) // [ Panel ] button pressed
      {
        if (!ListBox)
        {
          return FALSE;
        }

        // �� ������ ����� �������� �� � �������, � �����.
        // ����� ��������� ������. ����� �������� ��������, ����� ��
        // ���� �� ������, ����� �� ������� � ������� ����� (� �����-��
        // ����!) � � ���������� ��� ���������.
        if (ListBox->GetItemCount()==0)
        {
          return (TRUE);
        }
        FindExitCode = FIND_EXIT_PANEL;
        FindExitIndex = (DWORD)ListBox->GetUserData(NULL, 0);
        return (FALSE);
      }
    }
    case DN_CTLCOLORDLGLIST:
    {
      if (Param2)
        ((struct FarListColors *)Param2)->Colors[VMenuColorDisabled]=FarColorToReal(COL_DIALOGLISTTEXT);
      return TRUE;
    }
    case DN_CLOSE:
    {
      StopSearch=TRUE;
      return TRUE;
    }
    /* 10.08.2001 KM
       ��������� �������� ������� ������ ��� ��������� �������� �������.
    */
    case DN_RESIZECONSOLE:
    {
      COORD coord=(*(COORD*)Param2);
      SMALL_RECT rect;
      int IncY=coord.Y-DlgHeight-3;
      int I;

      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
      for (I=0;I<10;I++)
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,I,FALSE);

      Dialog::SendDlgMessage(hDlg,DM_GETDLGRECT,0,(long)&rect);
      coord.X=rect.Right-rect.Left+1;
      DlgHeight+=IncY;
      coord.Y=DlgHeight;

      if (IncY>0)
        Dialog::SendDlgMessage(hDlg,DM_RESIZEDIALOG,0,(long)&coord);

      for (I=0;I<2;I++)
      {
        Dialog::SendDlgMessage(hDlg,DM_GETITEMPOSITION,I,(long)&rect);
        rect.Bottom+=(short)IncY;
        Dialog::SendDlgMessage(hDlg,DM_SETITEMPOSITION,I,(long)&rect);
      }

      for (I=2;I<10;I++)
      {
        Dialog::SendDlgMessage(hDlg,DM_GETITEMPOSITION,I,(long)&rect);
        if (I==2)
          rect.Left=-1;
        rect.Top+=(short)IncY;
        Dialog::SendDlgMessage(hDlg,DM_SETITEMPOSITION,I,(long)&rect);
      }

      if (!(IncY>0))
        Dialog::SendDlgMessage(hDlg,DM_RESIZEDIALOG,0,(long)&coord);

      for (I=0;I<10;I++)
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,I,TRUE);
      Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);

      return TRUE;
    }
    /* KM $ */
  }
  Result = Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
  return Result;
}

int FindFiles::FindFilesProcess()
{
  _ALGO(CleverSysLog clv("FindFiles::FindFilesProcess()"));
  // � ����������� ��������� ����� � ����������� ����������
  static char Title[2*NM]="";
  static char SearchStr[NM]="";

  /* $ 05.10.2003 KM
     ���� ������������ ������ ��������, �� �� ����� ������ �������� �� ����
  */
  if (*FindMask)
    if (UseFilter)
      sprintf(Title,"%s: %s (%s)",MSG(MFindFileTitle),FindMask,MSG(MFindUsingFilter));
    else
      sprintf(Title,"%s: %s",MSG(MFindFileTitle),FindMask);
  else
    if (UseFilter)
      sprintf(Title,"%s (%s)",MSG(MFindFileTitle),MSG(MFindUsingFilter));
    else
      sprintf(Title,"%s",MSG(MFindFileTitle));
  /* KM $ */
  if (*FindStr)
  {
    /* $ 26.10.2003 KM */
    /* $ 24.09.2003 KM */
    char Temp[NM],FStr[NM*2];

    xstrncpy(FStr,FindStr,sizeof(FStr)-1);
    sprintf(Temp," \"%s\"",TruncStrFromEnd(FStr,10));
    sprintf(SearchStr,MSG(MFindSearchingIn),Temp);
    /* KM $ */
    /* KM $ */
  }
  else
    sprintf(SearchStr,MSG(MFindSearchingIn),"");

  /* $ 15.03.2002 KM
     ��������� ���� ����������� ��� ����������.
  /*
  /* $ 03.12.2001 DJ
     ���������� ����� ���� ������ � ������������
  */
  static struct DialogData FindDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,DLG_WIDTH,DLG_HEIGHT-4,0,0,DIF_SHOWAMPERSAND,0,Title,
  /* 01 */DI_LISTBOX,4,2,73,DLG_HEIGHT-9,0,0,DIF_LISTNOBOX,0,(char*)0,
  /* 02 */DI_TEXT,-1,DLG_HEIGHT-8,0,DLG_HEIGHT-8,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 03 */DI_TEXT,5,DLG_HEIGHT-7,0,DLG_HEIGHT-7,0,0,DIF_SHOWAMPERSAND,0,SearchStr,
  /* 04 */DI_TEXT,3,DLG_HEIGHT-6,0,DLG_HEIGHT-6,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 05 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,1,0,DIF_CENTERGROUP,1,(char *)MFindNewSearch,
  /* 06 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindGoTo,
  /* 07 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindView,
  /* 08 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindPanel,
  /* 09 */DI_BUTTON,0,DLG_HEIGHT-5,0,DLG_HEIGHT-5,0,0,DIF_CENTERGROUP,0,(char *)MFindStop
  };
  /* DJ $ */
  /* KM $ */
  MakeDialogItems(FindDlgData,FindDlg);

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  DlgHeight=DLG_HEIGHT-2;
  DlgWidth=FindDlg[0].X2-FindDlg[0].X1-4;

  int IncY=ScrY>24 ? ScrY-24:0;
  FindDlg[0].Y2+=IncY;
  FindDlg[1].Y2+=IncY;
  FindDlg[2].Y1+=IncY;
  FindDlg[3].Y1+=IncY;
  FindDlg[4].Y1+=IncY;
  FindDlg[5].Y1+=IncY;
  FindDlg[6].Y1+=IncY;
  FindDlg[7].Y1+=IncY;
  FindDlg[8].Y1+=IncY;
  FindDlg[9].Y1+=IncY;

  DlgHeight+=IncY;

  if (PluginMode)
  {
    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
    HANDLE hPlugin=ActivePanel->GetPluginHandle();
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);

    FindFileArcIndex = AddArcListItem(Info.HostFile, hPlugin, Info.Flags, Info.CurDir);
    if (FindFileArcIndex == LIST_INDEX_NONE)
      return(FALSE);

    if ((Info.Flags & OPIF_REALNAMES)==0)
    {
      FindDlg[8].Type=DI_TEXT;
      *FindDlg[8].Data=0;
    }
  }

  Dialog Dlg=Dialog(FindDlg,sizeof(FindDlg)/sizeof(FindDlg[0]),FindDlgProc);
  hDlg=(HANDLE)&Dlg;
//  pDlg->SetDynamicallyBorn();
  Dlg.SetHelp("FindFileResult");
  Dlg.SetPosition(-1,-1,DLG_WIDTH+4,DLG_HEIGHT-2+IncY);
  // ���� �� �������� ������, � �� ������������� ��������� �����������
  // ������ ��� ������ � ������ �������� �� �����������
  Dlg.InitDialog();
  Dlg.Show();

  LastFoundNumber=0;
  SearchDone=FALSE;
  StopSearch=FALSE;
  PauseSearch=FALSE;
  WriteDataUsed=FALSE;
  PrepareFilesListUsed=0;
  FindFileCount=FindDirCount=0;
  FindExitIndex = LIST_INDEX_NONE;
  FindExitCode = FIND_EXIT_NONE;
  *FindMessage=*LastDirName=FindMessageReady=FindCountReady=FindPositionChanged=0;

  if (PluginMode)
  {
    if (_beginthread(PreparePluginList,0,NULL)==(unsigned long)-1)
      return FALSE;
  }
  else
  {
    if (_beginthread(PrepareFilesList,0,NULL)==(unsigned long)-1)
      return FALSE;
  }

  // ����� ��� ������ � ������� ���������� � ���� ������
  if (_beginthread(WriteDialogData,0,NULL)==(unsigned long)-1)
    return FALSE;

  IsProcessAssignMacroKey++; // �������� ��� ����. �������
  Dlg.Process();
  IsProcessAssignMacroKey--;

  while(WriteDataUsed || PrepareFilesListUsed > 0)
    Sleep(10);

  ::hDlg=NULL;

  switch (FindExitCode)
  {
    case FIND_EXIT_SEARCHAGAIN:
    {
      return TRUE;
    }
    case FIND_EXIT_PANEL:
    // ���������� ���������� �� ��������� ������
    {
      int ListSize = FindListCount;
      PluginPanelItem *PanelItems=new PluginPanelItem[ListSize];
      if (PanelItems==NULL)
        ListSize=0;
      int ItemsNumber=0;
      for (int i=0;i<ListSize;i++)
      {
        if (strlen(FindList[i].FindData.cFileName)>0 && FindList[i].Used)
        // ��������� ������, ���� ��� ������
        {
          // ��� �������� � ������������ ������� ������� ��� ����� �� ��� ������.
          // ������ ���� ������ ������ �����.
          int IsArchive = ((FindList[i].ArcIndex != LIST_INDEX_NONE) &&
                          !(ArcList[FindList[i].ArcIndex].Flags&OPIF_REALNAMES));
          // ��������� ������ ����� ��� ����� �������
          /* $ 24.09.2003 KM
             ���� ������� ����� ������ hex-�����, ����� ����� � ����� �� ��������
          */
          /* $ 13.11.2001 VVM
            ! ��. ������� �����, ���� �� ������... */
          if (IsArchive || Opt.FindOpt.FindFolders ||
              !(FindList[i].FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) &&
              !SearchHex)
          /* VVM $ */
          /* KM $ */
          {
            if (IsArchive)
              xstrncpy(FindList[i].FindData.cFileName, ArcList[FindList[i].ArcIndex].ArcName,
                      sizeof(FindList[i].FindData.cFileName)-1);
            PluginPanelItem *pi=&PanelItems[ItemsNumber++];
            memset(pi,0,sizeof(*pi));
            pi->FindData=FindList[i].FindData;
            if (IsArchive)
              pi->FindData.dwFileAttributes = 0;
            /* $ 21.03.2002 VVM
              + �������� ����� ��������� ��� ��������������� "\" */
            if (pi->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
              int Length = strlen(pi->FindData.cFileName);
              if ((Length) && (pi->FindData.cFileName[Length-1]=='\\'))
                pi->FindData.cFileName[Length-1] = 0;
            }
            /* VVM $ */
          }
        } /* if */
      } /* for */

      HANDLE hNewPlugin=CtrlObject->Plugins.OpenFindListPlugin(PanelItems,ItemsNumber);
      if (hNewPlugin!=INVALID_HANDLE_VALUE)
      {
        Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
        Panel *NewPanel=CtrlObject->Cp()->ChangePanel(ActivePanel,FILE_PANEL,TRUE,TRUE);
        NewPanel->SetPluginMode(hNewPlugin,"");
        NewPanel->SetVisible(TRUE);
        NewPanel->Update(0);
//        if (FindExitIndex != LIST_INDEX_NONE)
//          NewPanel->GoToFile(FindList[FindExitIndex].FindData.cFileName);
        NewPanel->Show();
        NewPanel->SetFocus();
      }
      /* $ 13.07.2000 SVS
         ������������ new[]
      */
      delete[] PanelItems;
      break;
    } /* case FIND_EXIT_PANEL */
    case FIND_EXIT_GOTO:
    {
      char *FileName=FindList[FindExitIndex].FindData.cFileName;
      Panel *FindPanel=CtrlObject->Cp()->ActivePanel;

      if ((FindList[FindExitIndex].ArcIndex != LIST_INDEX_NONE) &&
          (!(ArcList[FindList[FindExitIndex].ArcIndex].Flags & OPIF_REALNAMES)))
      {
        HANDLE hPlugin = ArcList[FindList[FindExitIndex].ArcIndex].hPlugin;
        if (hPlugin == INVALID_HANDLE_VALUE)
        {
          char ArcName[NM],ArcPath[NM];
          xstrncpy(ArcName,ArcList[FindList[FindExitIndex].ArcIndex].ArcName,sizeof(ArcName)-1);
          if (FindPanel->GetType()!=FILE_PANEL)
            FindPanel=CtrlObject->Cp()->ChangePanel(FindPanel,FILE_PANEL,TRUE,TRUE);
          xstrncpy(ArcPath,ArcName,sizeof(ArcPath)-1);
          *PointToName(ArcPath)=0;
          FindPanel->SetCurDir(ArcPath,TRUE);
          hPlugin=((FileList *)FindPanel)->OpenFilePlugin(ArcName,FALSE);
          if (hPlugin==(HANDLE)-2)
            hPlugin = INVALID_HANDLE_VALUE;
        } /* if */
        if (hPlugin != INVALID_HANDLE_VALUE)
        {
          struct OpenPluginInfo Info;
          CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);

          /* $ 19.01.2003 KM
             ��������� �������� � ������ ������� �������.
          */
          if (SearchMode==SEARCH_ROOT ||
              SearchMode==SEARCH_ALL ||
              SearchMode==SEARCH_ALL_BUTNETWORK)
            CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_FIND);
          SetPluginDirectory(FileName,hPlugin,TRUE);
          /* KM $ */
        }
      } /* if */
      else
      {
        char SetName[NM];
        int Length;
        if ((Length=strlen(FileName))==0)
          break;
        if (Length>1 && FileName[Length-1]=='\\' && FileName[Length-2]!=':')
          FileName[Length-1]=0;
        if (GetFileAttributes(FileName)==(DWORD)-1)
          break;
        {
          char *NamePtr;
          NamePtr=PointToName(FileName);
          xstrncpy(SetName,NamePtr,sizeof(SetName)-1);
          *NamePtr=0;
          Length=strlen(FileName);
          if (Length>1 && FileName[Length-1]=='\\' && FileName[Length-2]!=':')
            FileName[Length-1]=0;
        }
        if (*FileName==0)
          break;
        if (FindPanel->GetType()!=FILE_PANEL &&
            CtrlObject->Cp()->GetAnotherPanel(FindPanel)->GetType()==FILE_PANEL)
          FindPanel=CtrlObject->Cp()->GetAnotherPanel(FindPanel);
        if ((FindPanel->GetType()!=FILE_PANEL) || (FindPanel->GetMode()!=NORMAL_PANEL))
        // ������ ������ �� ������� ��������...
        {
          FindPanel=CtrlObject->Cp()->ChangePanel(FindPanel,FILE_PANEL,TRUE,TRUE);
          FindPanel->SetVisible(TRUE);
          FindPanel->Update(0);
        }
        /* $ 09.06.2001 IS
           ! �� ������ �������, ���� �� ��� � ��� ���������. ��� �����
             ���������� ����, ��� ��������� � ��������� ������ �� ������������.
        */
        {
          char DirTmp[NM];
          FindPanel->GetCurDir(DirTmp);
          Length=strlen(DirTmp);
          if (Length>1 && DirTmp[Length-1]=='\\' && DirTmp[Length-2]!=':')
            DirTmp[Length-1]=0;
          if(0!=LocalStricmp(FileName, DirTmp))
            FindPanel->SetCurDir(FileName,TRUE);
        }
        /* IS $ */
        if (*SetName)
          FindPanel->GoToFile(SetName);
        FindPanel->Show();
        FindPanel->SetFocus();
      }
      break;
    } /* case FIND_EXIT_GOTO */
  } /* switch */

  return FALSE;
}


void FindFiles::SetPluginDirectory(char *DirName,HANDLE hPlugin,int UpdatePanel)
{
  _ALGO(CleverSysLog clv("FindFiles::SetPluginDirectory()"));
  _ALGO(SysLog("DirName='%s', hPlugin=0x%08X, UpdatePanel=%d",(DirName?DirName:"NULL"),hPlugin,UpdatePanel));
  char Name[NM],*StartName,*EndName;
  int IsPluginDir;

  /* $ 19.01.2003 KM
     ����������� ��������� �� 4 ����. ���� � DirName ����
     ������ '\x1' ������ ��� ���� �� �������. ����� �������
     ����� ���������� ���������� ���� �, ��������������,
     ������� ���������� �������.
  */
  if (strlen(DirName)>0)
    IsPluginDir=strchr(DirName,'\x1')!=NULL;
  else
    IsPluginDir=FALSE;

  xstrncpy(Name,DirName,sizeof(Name)-1);
  StartName=Name;
  while(IsPluginDir?(EndName=strchr(StartName,'\x1'))!=NULL:(EndName=strchr(StartName,'\\'))!=NULL)
  /* KM $ */
  {
    *EndName=0;
    // RereadPlugin
    {
      int FileCount=0;
      PluginPanelItem *PanelData=NULL;
      if (CtrlObject->Plugins.GetFindData(hPlugin,&PanelData,&FileCount,OPM_FIND))
        CtrlObject->Plugins.FreeFindData(hPlugin,PanelData,FileCount);
    }
    CtrlObject->Plugins.SetDirectory(hPlugin,StartName,OPM_FIND);
    StartName=EndName+1;
  }
  /* $ 19.01.2003 KM
     �������� ������ ��� �������������.
  */
  if (UpdatePanel)
  {
    CtrlObject->Cp()->ActivePanel->Update(UPDATE_KEEP_SELECTION);
    if (!CtrlObject->Cp()->ActivePanel->GoToFile(StartName))
      CtrlObject->Cp()->ActivePanel->GoToFile(DirName);
    CtrlObject->Cp()->ActivePanel->Show();
  }
  /* KM $ */
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif
void _cdecl FindFiles::PrepareFilesList(void *Param)
{
  WIN32_FIND_DATA FindData;
  char FullName[NM],Root[NM*2];

  TRY {

    PrepareFilesListUsed++;
    DWORD DiskMask=FarGetLogicalDrives();
    CtrlObject->CmdLine->GetCurDir(Root);

    for (int CurrentDisk=0;DiskMask!=0;CurrentDisk++,DiskMask>>=1)
    {
      if (SearchMode==SEARCH_ALL ||
          SearchMode==SEARCH_ALL_BUTNETWORK)
      {
        if ((DiskMask & 1)==0)
          continue;
        sprintf(Root,"%c:\\",'A'+CurrentDisk);
        int DriveType=FAR_GetDriveType(Root);
        if (DriveType==DRIVE_REMOVABLE || IsDriveTypeCDROM(DriveType) ||
           (DriveType==DRIVE_REMOTE && SearchMode==SEARCH_ALL_BUTNETWORK))
          if (DiskMask==1)
            break;
          else
            continue;
      }
      else if (SearchMode==SEARCH_ROOT)
        GetPathRootOne(Root,Root);

      {
        ScanTree ScTree(FALSE,SearchMode!=SEARCH_CURRENT_ONLY,SearchInSymLink);

        char SelName[NM];
        int FileAttr;
        if (SearchMode==SEARCH_SELECTED)
          CtrlObject->Cp()->ActivePanel->GetSelName(NULL,FileAttr);

        while (1)
        {
          char CurRoot[2*NM];
          if (SearchMode==SEARCH_SELECTED)
          {
            if (!CtrlObject->Cp()->ActivePanel->GetSelName(SelName,FileAttr))
              break;
            if ((FileAttr & FILE_ATTRIBUTE_DIRECTORY)==0 || TestParentFolderName(SelName) ||
                strcmp(SelName,".")==0)
              continue;
            xstrncpy(CurRoot,Root,sizeof(CurRoot)-1);
            AddEndSlash(CurRoot);
            strcat(CurRoot,SelName);
          }
          else
            xstrncpy(CurRoot,Root,sizeof(CurRoot)-1);

          ScTree.SetFindPath(CurRoot,"*.*");

          statusCS.Enter ();

          xstrncpy(FindMessage,CurRoot,sizeof(FindMessage)-1);
          FindMessage[sizeof(FindMessage)-1]=0;
          FindMessageReady=TRUE;

          statusCS.Leave ();

          while (!StopSearch && ScTree.GetNextName(&FindData,FullName, sizeof (FullName)-1))
          {
            while (PauseSearch)
              Sleep(10);

            /* $ 30.09.2003 KM
              ����������� ����� �� ���������� � ����������� ������
            */
            int IsFile;
            if (UseFilter)
              IsFile=Filter->FileInFilter(&FindData);
            else
              IsFile=TRUE;

            if (IsFile)
            {
              /* $ 14.06.2004 KM
                ��������� �������� ��� ��������� ���������
              */
              if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
              {
                statusCS.Enter();

                xstrncpy(FindMessage,FullName,sizeof(FindMessage)-1);
                FindMessage[sizeof(FindMessage)-1]=0;
                FindMessageReady=TRUE;

                statusCS.Leave();
              }
              /* KM $ */


              if (IsFileIncluded(NULL,FullName,FindData.dwFileAttributes))
                AddMenuRecord(FullName,&FindData);

              if (SearchInArchives)
                ArchiveSearch(FullName);
            }
            /* KM $ */
          }
          if (SearchMode!=SEARCH_SELECTED)
            break;
        }
        if (SearchMode!=SEARCH_ALL && SearchMode!=SEARCH_ALL_BUTNETWORK)
          break;
      }
    }

    statusCS.Enter();
    sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);
    SetFarTitle (FindMessage);
    statusCS.Leave();

    while (!StopSearch && FindMessageReady)
      Sleep(10);
  //  sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);

    statusCS.Enter ();

    SearchDone=TRUE;
    FindMessageReady=TRUE;

    statusCS.Leave ();

    PrepareFilesListUsed--;
  }
  EXCEPT (xfilter((int)INVALID_HANDLE_VALUE,GetExceptionInformation(),NULL,1))
  {
    TerminateProcess( GetCurrentProcess(), 1);
  }
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


void FindFiles::ArchiveSearch(char *ArcName)
{
  _ALGO(CleverSysLog clv("FindFiles::ArchiveSearch()"));
  _ALGO(SysLog("ArcName='%s'",(ArcName?ArcName:"NULL")));
  char *Buffer=new char[Opt.PluginMaxReadData];
  FILE *ProcessFile=fopen(ArcName,"rb");
  if (ProcessFile==NULL)
  {
    /* $ 13.07.2000 SVS
       ������������ new[]
    */
    delete[] Buffer;
    /* SVS $ */
    return;
  }
  int ReadSize=fread(Buffer,1,Opt.PluginMaxReadData,ProcessFile);
  fclose(ProcessFile);

  int SavePluginsOutput=DisablePluginsOutput;
  DisablePluginsOutput=TRUE;
  HANDLE hArc=CtrlObject->Plugins.OpenFilePlugin(ArcName,(unsigned char *)Buffer,ReadSize);
  /* $ 01.10.2001 VVM */
  DisablePluginsOutput=SavePluginsOutput;
  /* VVM $ */


  /* $ 13.07.2000 SVS
     ������������ new[]
  */
  delete[] Buffer;
  /* SVS $ */

  if (hArc==(HANDLE)-2)
  {
    BreakMainThread=TRUE;
    return;
  }
  if (hArc==INVALID_HANDLE_VALUE)
    return;

  int SaveSearchMode=SearchMode;
  DWORD SaveArcIndex = FindFileArcIndex;
  {
    SearchMode=SEARCH_FROM_CURRENT;
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hArc,&Info);
    FindFileArcIndex = AddArcListItem(ArcName, hArc, Info.Flags, Info.CurDir);
    /* $ 11.12.2001 VVM
      - �������� ������� ����� ������� � ������.
        � ���� ������ �� ����� - �� ������ ��� ����� */
    {
      char SaveDirName[NM],SaveSearchPath[2*NM];
      int SaveListCount = FindListCount;
      /* $ 19.01.2003 KM
         �������� ���� ������ � �������, ��� ����� ����������.
      */
      xstrncpy(SaveSearchPath,PluginSearchPath,2*NM);
      /* KM $ */
      xstrncpy(SaveDirName, LastDirName, NM);
      *LastDirName = 0;
      PreparePluginList((void *)1);
      xstrncpy(PluginSearchPath,SaveSearchPath,2*NM);
      WaitForSingleObject(hPluginMutex,INFINITE);
      CtrlObject->Plugins.ClosePlugin(ArcList[FindFileArcIndex].hPlugin);
      ArcList[FindFileArcIndex].hPlugin = INVALID_HANDLE_VALUE;
      ReleaseMutex(hPluginMutex);
      if (SaveListCount == FindListCount)
        xstrncpy(LastDirName, SaveDirName, NM);
    }
    /* VVM $ */
  }
  FindFileArcIndex = SaveArcIndex;
  SearchMode=SaveSearchMode;
}

/* $ 01.07.2001 IS
   ���������� FileMaskForFindFile ������ GetCommaWord
*/
int FindFiles::IsFileIncluded(PluginPanelItem *FileItem,char *FullName,DWORD FileAttr)
{
  int FileFound=FileMaskForFindFile.Compare(FullName);
  HANDLE hPlugin=INVALID_HANDLE_VALUE;
  if (FindFileArcIndex != LIST_INDEX_NONE)
    hPlugin = ArcList[FindFileArcIndex].hPlugin;
  while(FileFound)
  {
    /* $ 24.09.2003 KM
       ���� ������� ����� ������ hex-�����, ����� ����� � ����� �� ��������
    */
    /* $ 17.01.2002 VVM
      ! ��������� ������ � ������� � ������ ������� � ������ -
        ���� � ������� ������� ���� ������������ */
    if ((FileAttr & FILE_ATTRIBUTE_DIRECTORY) && ((Opt.FindOpt.FindFolders==0) || SearchHex))
//        ((hPlugin == INVALID_HANDLE_VALUE) ||
//        (ArcList[FindFileArcIndex].Flags & OPIF_FINDFOLDERS)==0))
      return FALSE;
    /* VVM $ */
    /* KM $ */

    if (*FindStr && FileFound)
    {
      FileFound=FALSE;
      if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
        break;
      char SearchFileName[NM];
      int RemoveTemp=FALSE;
      if ((hPlugin != INVALID_HANDLE_VALUE) && (ArcList[FindFileArcIndex].Flags & OPIF_REALNAMES)==0)
      {
        char TempDir[NM];
        FarMkTempEx(TempDir); // � �������� �� NULL???
        CreateDirectory(TempDir,NULL);
        WaitForSingleObject(hPluginMutex,INFINITE);
        if (!CtrlObject->Plugins.GetFile(hPlugin,FileItem,TempDir,SearchFileName,OPM_SILENT|OPM_FIND))
        {
          ReleaseMutex(hPluginMutex);
          FAR_RemoveDirectory(TempDir);
          break;
        }
        else
          ReleaseMutex(hPluginMutex);
        RemoveTemp=TRUE;
      }
      else
        xstrncpy(SearchFileName,FullName,sizeof(SearchFileName)-1);
      if (LookForString(SearchFileName))
        FileFound=TRUE;
      if (RemoveTemp)
        DeleteFileWithFolder(SearchFileName);
    }
    break;
  }
  return(FileFound);
}
/* IS $ */


void FindFiles::AddMenuRecord(char *FullName, WIN32_FIND_DATA *FindData)
{
  char MenuText[NM],FileText[NM],SizeText[30];
  char Attr[30],Date[30],DateStr[30],TimeStr[30];
  struct MenuItem ListItem;

  Dialog* Dlg=(Dialog*)hDlg;
  if(!Dlg)
  {
    return;
  }

  VMenu *ListBox=Dlg->Item[1].ListPtr;

  if (!ListBox)
  {
    return;
  }

  memset(&ListItem,0,sizeof(ListItem));

  if (FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    sprintf(SizeText,"%13s",MSG(MFindFileFolder));
  else
  {
    // ������� ����������� ������� � ������ 64-�������� ������� �����
    __int64 fsize=(unsigned __int64)FindData->nFileSizeLow|((unsigned __int64)FindData->nFileSizeHigh<<32);
    _ui64toa(fsize,SizeText,10);
  }
  char *DisplayName=FindData->cFileName;
  /* $ 24.03.2002 KM
     � �������� ������������� �������� ��������� � ����� �� ���
     ��� ����������� ��� ����������� � ������, �������� ����,
     �.�. ��������� ������� ���������� ��� ������ � ������ ����,
     � ������� ��������� ������.
  */
  if (FindFileArcIndex != LIST_INDEX_NONE)
    DisplayName=PointToName(DisplayName);
  /* KM $ */

  sprintf(FileText," %-26.26s %13.13s",DisplayName,SizeText);

  /* $ 05.10.2003 KM
     ��� ������������� ������� �� ���� ����� ���������� ��� ���
     ����, ������� ����� � �������.
  */
  if (UseFilter && Filter->GetParams().FDate.Used)
  {
    switch(Filter->GetParams().FDate.DateType)
    {
      case FDATE_MODIFIED:
        // ���������� ���� ���������� ���������
        ConvertDate(FindData->ftLastWriteTime,DateStr,TimeStr,5);
        break;
      case FDATE_CREATED:
        // ���������� ���� ��������
        ConvertDate(FindData->ftCreationTime,DateStr,TimeStr,5);
        break;
      case FDATE_OPENED:
        // ���������� ���� ���������� �������
        ConvertDate(FindData->ftLastAccessTime,DateStr,TimeStr,5);
        break;
    }
  }
  else
    // ���������� ���� ���������� ���������
    ConvertDate(FindData->ftLastWriteTime,DateStr,TimeStr,5);
  /* KM $ */
  sprintf(Date,"  %s  %s",DateStr,TimeStr);
  strcat(FileText,Date);

  /* $ 05.10.2003 KM
     ��������� � ������ ������ �������� ��������� ������
  */
  char AttrStr[8];
  DWORD FileAttr=FindData->dwFileAttributes;
  AttrStr[0]=(FileAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':((FileAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':' ');
  AttrStr[1]=(FileAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':' ';
  AttrStr[2]=(FileAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':' ';
  AttrStr[3]=(FileAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':' ';
  AttrStr[4]=(FileAttr & FILE_ATTRIBUTE_READONLY) ? 'R':' ';
  AttrStr[5]=0;

  sprintf(Attr," %s",AttrStr);
  strcat(FileText,Attr);
  /* KM $ */
  sprintf(MenuText," %-*.*s",DlgWidth-3,DlgWidth-3,FileText);

  char PathName[2*NM];
  xstrncpy(PathName,FullName,sizeof(PathName)-1);
  PathName[sizeof(PathName)-1]=0;

  RemovePseudoBackSlash(PathName);

  *PointToName(PathName)=0;
  if (*PathName==0)
    strcpy(PathName,".\\");

  AddEndSlash(PathName);

  if (LocalStricmp(PathName,LastDirName)!=0)
  {
    if (*LastDirName)
    {
      /* $ 12.05.2001 DJ
         ������ �� ��������������� �� ������ ������� ����� ����������
      */
      ListItem.Flags|=LIF_DISABLE;
      // � ������ VVM ������� ���������� � ������ ������� LIST_INDEX_NONE �� ������ �������
      ListBox->SetUserData((void*)LIST_INDEX_NONE,sizeof(LIST_INDEX_NONE),ListBox->AddItem(&ListItem));
      ListItem.Flags&=~LIF_DISABLE;
      /* DJ $ */
    }
    xstrncpy(LastDirName,PathName,sizeof(LastDirName)-1);
    if ((FindFileArcIndex != LIST_INDEX_NONE) &&
        (!(ArcList[FindFileArcIndex].Flags & OPIF_REALNAMES)) &&
        (ArcList[FindFileArcIndex].ArcName) &&
        (*ArcList[FindFileArcIndex].ArcName))
    {
      char ArcPathName[NM*2];
      sprintf(ArcPathName,"%s:%s",ArcList[FindFileArcIndex].ArcName,*PathName=='.' ? "\\":PathName);
      xstrncpy(PathName,ArcPathName,sizeof(PathName)-1);
    }
    xstrncpy(SizeText,MSG(MFindFileFolder),sizeof(SizeText)-1);
    sprintf(FileText,"%-50.50s  <%6.6s>",TruncPathStr(PathName,50),SizeText);
    sprintf(ListItem.Name,"%-*.*s",DlgWidth-2,DlgWidth-2,FileText);

    ffCS.Enter ();

    DWORD ItemIndex = AddFindListItem(FindData);
    if (ItemIndex != LIST_INDEX_NONE)
    {
      // ������� ������ � FindData. ��� ��� �� �����
      memset(&FindList[ItemIndex].FindData,0,sizeof(FindList[ItemIndex].FindData));
      // ���������� LastDirName, �.�. PathName ��� ����� ���� ��������
      xstrncpy(FindList[ItemIndex].FindData.cFileName, LastDirName,
              sizeof(FindList[ItemIndex].FindData.cFileName)-1);
      /* $ 07.04.2002 KM
        - ������ ������� ����� ���������� ����, � ���������
          ������ �� �������� ������� �� ������� �� ������.
          Used=0 - ��� �� �������� �� ��������� ������.
      */
      FindList[ItemIndex].Used=0;
      /* KM $ */
      // �������� ������� � ��������, ���-�� �� �� ��� ������ :)
      FindList[ItemIndex].FindData.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
      if (FindFileArcIndex != LIST_INDEX_NONE)
        FindList[ItemIndex].ArcIndex = FindFileArcIndex;

      ListBox->SetUserData((void*)ItemIndex,sizeof(ItemIndex),
                           ListBox->AddItem(&ListItem));
    }

    ffCS.Leave ();
  }

  /* $ 24.03.2002 KM
     ������������� ��������� � ������ ����������
     �����. ��� ���� � 1.65 � � 3 ����, �� ���
     ������ ��������� ������ � ��� ���-�� �������,
     ������ ��������� �� �����.
  */

  ffCS.Enter ();

  DWORD ItemIndex = AddFindListItem(FindData);
  if (ItemIndex != LIST_INDEX_NONE)
  {
    xstrncpy(FindList[ItemIndex].FindData.cFileName, FullName,
            sizeof(FindList[ItemIndex].FindData.cFileName)-1);
    /* $ 07.04.2002 KM
       Used=1 - ��� �������� �� ��������� ������.
    */
    FindList[ItemIndex].Used=1;
    /* KM $ */
    if (FindFileArcIndex != LIST_INDEX_NONE)
      FindList[ItemIndex].ArcIndex = FindFileArcIndex;
  }
  xstrncpy(ListItem.Name,MenuText,sizeof(ListItem.Name)-1);

  ffCS.Leave ();
  /* $ 17.01.2002 VVM
    ! �������� ����� �� � ���������, � � ������. ���� �� �������� ��������� */
//  ListItem.SetSelect(!FindFileCount);

  int ListPos = ListBox->AddItem(&ListItem);
  ListBox->SetUserData((void*)ItemIndex,sizeof(ItemIndex), ListPos);
  // ������� ��� �������� - � ������.
  if (!FindFileCount && !FindDirCount)
    ListBox->SetSelectPos(ListPos, -1);
  /* VVM $ */

  statusCS.Enter();

  if (FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    FindDirCount++;
  else
    FindFileCount++;

  statusCS.Leave();
  /* KM $ */

  LastFoundNumber++;
  FindCountReady=TRUE;

}


int FindFiles::LookForString(char *Name)
{
  FILE *SrcFile;
  char Buf[32768],SaveBuf[32768],CmpStr[sizeof(FindStr)];
  int Length,ReadSize;
  if ((Length=strlen(FindStr))==0)
    return(TRUE);
  HANDLE FileHandle=FAR_CreateFile(Name,GENERIC_READ|GENERIC_WRITE,
         FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
  if (FileHandle==INVALID_HANDLE_VALUE)
    FileHandle=FAR_CreateFile(Name,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
                          NULL,OPEN_EXISTING,0,NULL);
  if (FileHandle==INVALID_HANDLE_VALUE)
    return(FALSE);
  int Handle=_open_osfhandle((long)FileHandle,O_BINARY);
  if (Handle==-1)
    return(FALSE);
  if ((SrcFile=fdopen(Handle,"rb"))==NULL)
    return(FALSE);

  FILETIME LastAccess;
  int TimeRead=GetFileTime(FileHandle,NULL,&LastAccess,NULL);

  /* $ 26.10.2003 KM */
  if (SearchHex)
  {
    int LenCmpStr=sizeof(CmpStr);

    Transform((unsigned char *)CmpStr,LenCmpStr,(char *)FindStr,'S');
    Length=LenCmpStr;
  }
  else
    xstrncpy(CmpStr,FindStr,sizeof(CmpStr)-1);
  /* KM $ */

  if (!CmpCase && !SearchHex)
    LocalStrupr(CmpStr);
  /* $ 30.07.2000 KM
     ���������� ����������
  */
  int FirstIteration=TRUE;
  /* KM $ */
  int ReverseBOM=FALSE;
  int IsFirst=FALSE;
  while (!StopSearch && (ReadSize=fread(Buf,1,sizeof(Buf),SrcFile))>0)
  {
    int DecodeTableNum=0;
    int UnicodeSearch=UseUnicode;
    int RealReadSize=ReadSize;

    /* $ 22.09.2003 KM
       ����� �� hex-�����
    */
    if (!SearchHex)
    {
      if (UseAllTables || UseUnicode)
      {
        memcpy(SaveBuf,Buf,ReadSize);

        /* $ 21.10.2000 SVS
           ������� ����������, � ��� ��� �������, �� ���� � FFFE-������
        */
        if(!IsFirst)
        {
          IsFirst=TRUE;
          if(*(WORD*)Buf == 0xFFFE) // The text contains the Unicode
             ReverseBOM=TRUE;       // byte-reversed byte-order mark
                                    // (Reverse BOM) 0xFFFE as its first character.
        }

        if(ReverseBOM)
        {
          BYTE Chr;
          for(int I=0; I < ReadSize; I+=2)
          {
            Chr=SaveBuf[I];
            SaveBuf[I]=SaveBuf[I+1];
            SaveBuf[I+1]=Chr;
          }
        }
        /* SVS $ */
      }
    }
    /* KM $ */

    while (1)
    {
      /* $ 22.09.2003 KM
         ����� �� hex-�����
      */
      if (!SearchHex)
      {
        if (DecodeTableNum>0 && !UnicodeSearch)
          memcpy(Buf,SaveBuf,ReadSize);
        if (UnicodeSearch)
        {
          WideCharToMultiByte(CP_OEMCP,0,(LPCWSTR)SaveBuf,ReadSize/2,Buf,ReadSize,NULL,NULL);
          ReadSize/=2;
        }
        else
          /* $ 20.09.2003 KM
             ������� ��������� ANSI �������
          */
          if (UseDecodeTable || UseANSI || DecodeTableNum>0)
            for (int I=0;I<ReadSize;I++)
              Buf[I]=TableSet.DecodeTable[Buf[I]];
        /* KM $ */
        if (!CmpCase)
          LocalUpperBuf(Buf,ReadSize);
      }
      /* KM $ */

      int CheckSize=ReadSize-Length+1;
      /* $ 30.07.2000 KM
         ��������� "Whole words" � ������
      */
      /* $ 26.05.2002 KM
          ���������� ������ � ������ �� ����� ������.
      */
      for (int I=0;I<CheckSize;I++)
      {
        int cmpResult;
        int locResultLeft=FALSE;
        int locResultRight=FALSE;

        /* $ 22.09.2003 KM
           ����� �� hex-�����
        */
        if (WholeWords && !SearchHex)
        {
          if (!FirstIteration)
          {
            if (IsSpace(Buf[I-1]) || IsEol(Buf[I-1]) ||
               (strchr(Opt.WordDiv,Buf[I-1])!=NULL))
              locResultLeft=TRUE;
          }
          else
          {
            FirstIteration=FALSE;
            locResultLeft=TRUE;
          }

          if (RealReadSize!=sizeof(Buf) && I+Length>=RealReadSize)
            locResultRight=TRUE;
          else
            if (I+Length<RealReadSize &&
               (IsSpace(Buf[I+Length]) || IsEol(Buf[I+Length]) ||
               (strchr(Opt.WordDiv,Buf[I+Length])!=NULL)))
              locResultRight=TRUE;
        }
        /* KM $ */
        else
        {
          locResultLeft=TRUE;
          locResultRight=TRUE;
        }

        cmpResult=locResultLeft && locResultRight && CmpStr[0]==Buf[I] &&
          (Length==1 || CmpStr[1]==Buf[I+1] &&
          (Length==2 || memcmp(CmpStr+2,&Buf[I+2],Length-2)==0));

        if (cmpResult)
        {
          if (TimeRead)
            SetFileTime(FileHandle,NULL,&LastAccess,NULL);
          fclose(SrcFile);
          return(TRUE);
        }
      }
      /* KM $ */
      /* KM $ */
      /* $ 22.09.2003 KM
         ����� �� hex-�����
      */
      if (UseAllTables && !SearchHex)
      {
        if (PrepareTable(&TableSet,DecodeTableNum++,TRUE))
        {
          xstrncpy(CmpStr,FindStr,sizeof(CmpStr)-1);
          if (!CmpCase)
            LocalStrupr(CmpStr);
        }
        else
          if (!UnicodeSearch)
            UnicodeSearch=true;
          else
            break;
      }
      else
        break;
      /* KM $ */
    }

    if (RealReadSize==sizeof(Buf))
    {
      /* $ 22.09.2003 KM
         ����� �� hex-�����
      */
      /* $ 30.07.2000 KM
         ��������� offset ��� ������ ������ ����� � ������ WordDiv
      */
      int NewPos;
      if (UnicodeSearch && !SearchHex)
        NewPos=ftell(SrcFile)-2*(Length+1);
      else
        NewPos=ftell(SrcFile)-(Length+1);
      fseek(SrcFile,Max(NewPos,0),SEEK_SET);
      /* KM $ */
      /* KM $ */
    }
  }
  if (TimeRead)
    SetFileTime(FileHandle,NULL,&LastAccess,NULL);
  fclose(SrcFile);
  return(FALSE);
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif
void _cdecl FindFiles::PreparePluginList(void *Param)
{
  char SaveDir[NM];

  TRY {

    Sleep(200);
    *PluginSearchPath=0;
    Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
    /* $ 15.10.2001 VVM */
    HANDLE hPlugin=ArcList[FindFileArcIndex].hPlugin;
    struct OpenPluginInfo Info;
    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
    xstrncpy(SaveDir,Info.CurDir,sizeof(SaveDir)-1);
    WaitForSingleObject(hPluginMutex,INFINITE);
    if (SearchMode==SEARCH_ROOT ||
        SearchMode==SEARCH_ALL ||
        SearchMode==SEARCH_ALL_BUTNETWORK)
      CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_FIND);
    ReleaseMutex(hPluginMutex);
    RecurseLevel=0;
    ScanPluginTree(hPlugin,ArcList[FindFileArcIndex].Flags);
    /* VVM $ */
    WaitForSingleObject(hPluginMutex,INFINITE);
    if (SearchMode==SEARCH_ROOT ||
        SearchMode==SEARCH_ALL ||
        SearchMode==SEARCH_ALL_BUTNETWORK)
      CtrlObject->Plugins.SetDirectory(hPlugin,SaveDir,OPM_FIND);
    ReleaseMutex(hPluginMutex);
    while (!StopSearch && FindMessageReady)
      Sleep(10);
    if (Param==NULL)
    {
      statusCS.Enter();

      sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);
      FindMessageReady=TRUE;
      SearchDone=TRUE;

      statusCS.Leave();
    }
  }
  EXCEPT (xfilter((int)INVALID_HANDLE_VALUE,GetExceptionInformation(),NULL,1))
  {
    TerminateProcess( GetCurrentProcess(), 1);
  }
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif

void FindFiles::ScanPluginTree(HANDLE hPlugin, DWORD Flags)
{
  PluginPanelItem *PanelData=NULL;
  int ItemCount=0;

  WaitForSingleObject(hPluginMutex,INFINITE);
  if (StopSearch || !CtrlObject->Plugins.GetFindData(hPlugin,&PanelData,&ItemCount,OPM_FIND))
  {
    ReleaseMutex(hPluginMutex);
    return;
  }
  else
    ReleaseMutex(hPluginMutex);
  RecurseLevel++;

  /* $ 19.01.2003 KM
     ������� ���� ����������, ���-�� ���������� ����������
     �� ������ ��, ������� � ������.
  */
  /* $ 24.03.2002 KM
     ���������� � �������� �� ������ ����� ������ � ������,
     ���� ��� ����� �������� � ���� ����, � �� ������ ���
     OPIF_REALNAMES, � ��������� ������ � ����������� ������
     ���������� ���������� ���������� ������ ��������������
     ���������� ����� �� ������.
  */
//  if (PanelData && strlen(PointToName(PanelData->FindData.cFileName))>0)
//    far_qsort((void *)PanelData,ItemCount,sizeof(*PanelData),SortItems);
  /* KM $ */
  /* KM $ */

  if (SearchMode!=SEARCH_SELECTED || RecurseLevel!=1)
  {
    for (int I=0;I<ItemCount && !StopSearch;I++)
    {
      while (PauseSearch)
        Sleep(10);

      PluginPanelItem *CurPanelItem=PanelData+I;
      char *CurName=CurPanelItem->FindData.cFileName;
      char FullName[2*NM];
      if (strcmp(CurName,".")==0 || TestParentFolderName(CurName))
        continue;
//      char AddPath[2*NM];
      if (Flags & OPIF_REALNAMES)
      {
        xstrncpy(FullName,CurName,sizeof(FullName)-1);
//        strcpy(AddPath,CurName);
//        *PointToName(AddPath)=0;
      }
      else
      {
        sprintf(FullName,"%s%s",PluginSearchPath,CurName);
//        strcpy(AddPath,PluginSearchPath);
      }

      /* $ 30.09.2003 KM
        ����������� ����� �� ���������� � ����������� ������
      */
      int IsFile;
      if (UseFilter)
        IsFile=Filter->FileInFilter(&CurPanelItem->FindData);
      else
        IsFile=TRUE;

      if (IsFile)
      {
        /* $ 14.06.2004 KM
          ��������� �������� ��� ��������� ���������
        */
        if (CurPanelItem->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          statusCS.Enter();

          xstrncpy(FindMessage,FullName,sizeof(FindMessage)-1);
          FindMessage[sizeof(FindMessage)-1]=0;
          RemovePseudoBackSlash(FindMessage);
          FindMessageReady=TRUE;

          statusCS.Leave();

        }
        /* KM $ */

        if (IsFileIncluded(CurPanelItem,CurName,CurPanelItem->FindData.dwFileAttributes))
          AddMenuRecord(FullName,&CurPanelItem->FindData);

        if (SearchInArchives && (hPlugin != INVALID_HANDLE_VALUE) && (Flags & OPIF_REALNAMES))
          ArchiveSearch(FullName);
      }
      /* KM $ */
    }
  }
  if (SearchMode!=SEARCH_CURRENT_ONLY)
  {
    for (int I=0;I<ItemCount && !StopSearch;I++)
    {
      PluginPanelItem *CurPanelItem=PanelData+I;
      char *CurName=CurPanelItem->FindData.cFileName;
      if ((CurPanelItem->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
          strcmp(CurName,".")!=0 && !TestParentFolderName(CurName) &&
          (SearchMode!=SEARCH_SELECTED || RecurseLevel!=1 ||
          CtrlObject->Cp()->ActivePanel->IsSelected(CurName)))
      {
        WaitForSingleObject(hPluginMutex,INFINITE);
        if (strchr(CurName,'\x1')==NULL && CtrlObject->Plugins.SetDirectory(hPlugin,CurName,OPM_FIND))
        {
          ReleaseMutex(hPluginMutex);
          /* $ 19.01.2003 KM
             ���� ����� � ��� �������� �� ������������
             PluginSearchPath, �� ������ ������������ ������
             ����� � ���� � ����� ����������, ��� �� ���� ���.
          */
          int SearchPathLen=strlen(PluginSearchPath);
          int CurNameLen=strlen(CurName);
          if (SearchPathLen+CurNameLen<NM-2)
          {
            strcat(PluginSearchPath,CurName);
            strcat(PluginSearchPath,"\x1");
            ScanPluginTree(hPlugin, Flags);
            char *Ptr=strrchr(PluginSearchPath,'\x1');
            if (Ptr!=NULL)
              *Ptr=0;
          }
          /* KM $ */
          char *NamePtr=strrchr(PluginSearchPath,'\x1');
          if (NamePtr!=NULL)
            *(NamePtr+1)=0;
          else
            *PluginSearchPath=0;
          WaitForSingleObject(hPluginMutex,INFINITE);
          if (!CtrlObject->Plugins.SetDirectory(hPlugin,"..",OPM_FIND))
            StopSearch=TRUE;
          ReleaseMutex(hPluginMutex);
          if (StopSearch) break;
        }
        else
          ReleaseMutex(hPluginMutex);
      }
    }
  }
  CtrlObject->Plugins.FreeFindData(hPlugin,PanelData,ItemCount);
  RecurseLevel--;
}

void _cdecl FindFiles::WriteDialogData(void *Param)
{
  FarDialogItemData ItemData;
  char DataStr[NM];

  WriteDataUsed=TRUE;

  while( true )
  {
    Dialog* Dlg=(Dialog*)hDlg;

    if( !Dlg )
      break;

    VMenu *ListBox=Dlg->Item[1].ListPtr;

    if (ListBox && !PauseSearch && !ScreenSaverActive)
    {
      if (BreakMainThread)
        StopSearch=TRUE;

      if (FindCountReady)
      {
        statusCS.Enter ();

        sprintf(DataStr," %s: %d ",MSG(MFindFound),FindFileCount+FindDirCount);
        ItemData.PtrData=DataStr;
        ItemData.PtrLength=strlen(DataStr);

        Dialog::SendDlgMessage(hDlg,DM_SETTEXT,2,(long)&ItemData);

        FindCountReady=FALSE;

        statusCS.Leave ();
      }

      if (FindMessageReady)
      {
        statusCS.Enter ();

        char SearchStr[NM];

        if (*FindStr)
        {
          char Temp[NM],FStr[NM*2];

          xstrncpy(FStr,FindStr,sizeof(FStr)-1);
          sprintf(Temp," \"%s\"",TruncStrFromEnd(FStr,10));
          sprintf(SearchStr,MSG(MFindSearchingIn),Temp);
        }
        else
          sprintf(SearchStr,MSG(MFindSearchingIn),"");

        int Wid1=strlen(SearchStr);
        int Wid2=DlgWidth-strlen(SearchStr)-1;

        if (SearchDone)
        {
          Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

          xstrncpy(DataStr,MSG(MFindCancel),sizeof(DataStr)-1);
          ItemData.PtrData=DataStr;
          ItemData.PtrLength=strlen(DataStr);
          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,9,(long)&ItemData);

          sprintf(DataStr,"%-*.*s",DlgWidth,DlgWidth,FindMessage);
          ItemData.PtrData=DataStr;
          ItemData.PtrLength=strlen(DataStr);
          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,3,(long)&ItemData);

          ItemData.PtrData="";
          ItemData.PtrLength=strlen(DataStr);
          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,2,(long)&ItemData);

          Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
          SetFarTitle(FindMessage);
          StopSearch=TRUE;
        }
        else
        {
          sprintf(DataStr,"%-*.*s %-*.*s",Wid1,Wid1,SearchStr,Wid2,Wid2,TruncPathStr(FindMessage,Wid2));
          ItemData.PtrData=DataStr;
          ItemData.PtrLength=strlen(DataStr);
          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,3,(long)&ItemData);
        }

        FindMessageReady=FALSE;

        statusCS.Leave ();
      }

      if (LastFoundNumber && ListBox)
      {
        LastFoundNumber=0;

        if ( ListBox->UpdateRequired () )
          Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,1,1);
      }
    }

    if (StopSearch && SearchDone && !FindMessageReady && !FindCountReady && !LastFoundNumber)
        break;

    Sleep(20);
  }

  WriteDataUsed=FALSE;
}

BOOL FindFiles::FindListGrow()
{
  DWORD Delta = (FindListCapacity < 256)?LIST_DELTA:FindListCapacity/2;
  LPFINDLIST NewList = (LPFINDLIST)xf_realloc(FindList, (FindListCapacity + Delta) * sizeof(FINDLIST));
  if (NewList)
  {
    FindList = NewList;
    FindListCapacity+= Delta;
    return(TRUE);
  }
  return(FALSE);
}

BOOL FindFiles::ArcListGrow()
{
  DWORD Delta = (ArcListCapacity < 256)?LIST_DELTA:ArcListCapacity/2;
  LPARCLIST NewList = (LPARCLIST)xf_realloc(ArcList, (ArcListCapacity + Delta) * sizeof(ARCLIST));
  if (NewList)
  {
    ArcList = NewList;
    ArcListCapacity+= Delta;
    return(TRUE);
  }
  return(FALSE);
}

DWORD FindFiles::AddFindListItem(WIN32_FIND_DATA *FindData)
{
  if ((FindListCount == FindListCapacity) &&
      (!FindListGrow()))
    return(LIST_INDEX_NONE);
  FindList[FindListCount].FindData = *FindData;
  FindList[FindListCount].ArcIndex = LIST_INDEX_NONE;
  return(FindListCount++);
}

DWORD FindFiles::AddArcListItem(const char *ArcName, HANDLE hPlugin,
                                DWORD dwFlags, const char *RootPath)
{
  if ((ArcListCount == ArcListCapacity) &&
      (!ArcListGrow()))
    return(LIST_INDEX_NONE);
  xstrncpy(ArcList[ArcListCount].ArcName, NullToEmpty(ArcName),
          sizeof(ArcList[ArcListCount].ArcName)-1);
  ArcList[ArcListCount].hPlugin = hPlugin;
  ArcList[ArcListCount].Flags = dwFlags;
  xstrncpy(ArcList[ArcListCount].RootPath, NullToEmpty(RootPath),
          sizeof(ArcList[ArcListCount].RootPath)-1);
  AddEndSlash(ArcList[ArcListCount].RootPath);
  return(ArcListCount++);
}

void FindFiles::ClearAllLists()
{
  CriticalSectionLock Lock(ffCS);

  FindFileArcIndex = LIST_INDEX_NONE;

  if (FindList)
    xf_free(FindList);
  FindList = NULL;
  FindListCapacity = FindListCount = 0;

  if (ArcList)
    xf_free(ArcList);
  ArcList = NULL;
  ArcListCapacity = ArcListCount = 0;
}

char *FindFiles::PrepareDriveNameStr(char *SearchFromRoot,size_t sz)
{
  char CurDir[NM*2],DriveName[64];

  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  PluginMode=ActivePanel->GetMode()==PLUGIN_PANEL && ActivePanel->IsVisible();

  CtrlObject->CmdLine->GetCurDir(CurDir);
  GetPathRootOne(CurDir,CurDir);
  if (CurDir[strlen(CurDir)-1]=='\\')
    CurDir[strlen(CurDir)-1]=0;

  if (*CurDir==0 || PluginMode)
  {
    xstrncpy(SearchFromRoot,MSG(MSearchFromRoot),sz-1);
  }
  else
  {
    sprintf(DriveName,MSG(MSearchFromRootOfDrive),CurDir);
    sprintf(SearchFromRoot,"%s %s",MSG(MSearchFromRoot),DriveName);
  }
  SearchFromRoot[sz-1]=0;

  if (strlen(SearchFromRoot)>DLG_WIDTH-10)
    TruncStrFromEnd(SearchFromRoot,DLG_WIDTH-10);

  return SearchFromRoot;
}

char *FindFiles::RemovePseudoBackSlash(char *FileName)
{
  for (int i=0;FileName[i]!=0;i++)
  {
    if (FileName[i]=='\x1')
      FileName[i]='\\';
  }
  return FileName;
}
