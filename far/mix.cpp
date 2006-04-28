/*
mix.cpp

���� ������ ��������������� �������

*/

/* Revision: 1.180 28.04.2006 $ */

/*
Modify:
  28.04.2006 AY
    - ExpandEnvironmentStr - ������ � ����� OEM2ANSI � �������.
      ������ ��� ������� ������������ ���� �������� %var% � �� ������� ��������� �����.
  23.04.2006 AY
    - FarChDir - �� ������ ������������ ���������� ���� =DriveLetter:
  25.03.2006 AY
    - %var% �� �������������� ������ � ���������� ��� ������
  20.02.2006 SVS
    ! � ConvertNameToShort ����� �������� - ������ ��� Dest
  21.01.2006 AY
    ! � ������� ��� ������� ������� �� �������� �����.
  17.01.2006 SVS
    - Mantis#69 - ������� ��� ������������� ���������� ������������� ��� ������� �����
  05.12.2005 AY
    ! FarChDir - ������������� ������������� ���� ��� �������� �� ������������� �����
    + PrepareDiskPath �������� UNC ���� (\\computer\share)
  05.10.2005 SVS
    ! ������� Opt.NetSupportEncryption. � ������ ��������� ��������� � ������ - �� �����
  04.10.2005 SVS
    ! ��������� � CheckDisksProps()
  30.09.2005 SVS
    + CheckDisksProps() - ������� �������� 2-� �������� ������
  29.09.2005 SVS
    ! ��������� ��������� GetDirInfo() => ����
  09.09.2005 SVS
    ! ���������� ��������� ����� ���������� �� �������� ���� ������� �
      ��������� ������� CurPath2ComputerName()
  23.07.2005 SVS
    - ���� � CreatePath
  24.04.2005 AY
    ! GCC
  23.04.2005 KM
    ! ������� ������ � ��������� � GetDirInfo � ������ ������� ��������
  06.04.2005 AY
   ! PartCmdLine() - ������ ������ Remove*() ����� Unquote() ��� ��� ��� ��������
     ������ ��������������, �������� ���� "��� .exe" ����� ��������� ��� "��� ".
   ! PartCmdLine() - ������ ����� RemoveExternalSpaces() �� RemoveTrailingSpaces()
     ����� ���� ��� Unquote() ��� ���������. ������ ��������� ��� ����� " .exe",
     � ��� � ����� ������� ��� ���� �� �����.
   ! PartCmdLine() - ������ ������ ����� �������� � ����������� �� �����,
     �� ����������� ������ � Execute.
   - ����� � PartCmdLine() ������� ������� ���� / �������� exe �� ����������
     ��� ����� ��� ������� ��������, ����� ��� ��� ����.
  03.03.2005 SVS
    ! ������� �������� CreatePath()
    + Ctrl-[] ��������� ���������� �������� ���� � ������ ������
  11.11.2004 SVS
    + _MakePath1()
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  14.06.2004 SVS
    + UnExpandEnvString() � PathUnExpandEnvStr().
      ������� ������������, �.�. ���� ������ ����������, � � ������� - ����� �� ����������
  08.06.2004 SVS
    ! ������ GetDriveType ������ �������� FAR_GetDriveType().
    ! ������ "DriveType==DRIVE_CDROM" �������� IsDriveTypeCDROM()
  20.05.2004 SVS
    ! ������ UndoGlobalSaveScrPtr � RefreshFrameManeger ��������� � RefreshFrameManager.?pp
  17.05.2004 SVS
    - BugZ#1073 - "File not found" ��� ����� ����������.
  06.05.2004 SVS
    + PartCmdLine()
    + ProcessOSAliases() - ���� ��������. ������� ��� ����, ����� ����� ���� ������ � ����� ����� - �����.
  01.03.2004 SVS
    ! ������� FAR_OemTo* � FAR_CharTo* ������ ����������� WinAPI-�������
      (����� �� ������� + ������� ������������ ������ 1 ����)
  09.02.2004 SVS
    + SaveAllCurDir/RestoreAllCurDir - ����������/�������������� ���������� ����� ���� "=A:"
  27.10.2003 SVS
    - ������� ���� ��� ������� � ������ ������������� ����������������� �����.
  26.10.2003 KM
    - ����������� ������ ������������ ������ � Transform.
    ! ��������� ������� ���������� � ������ ����������������� � Transform.
  20.10.2003 SVS
    - FSF.FarRecursiveSearch, �� ����� ����� ���� ���� � �������� ����� ������
      ��� �������� ���.
      �������� ��� �� � �������������� ���
  09.10.2003 SVS
    ! SetFileApisToANSI() � SetFileApisToOEM() �������� �� SetFileApisTo() � ����������
      APIS2ANSI ��� APIS2OEM - ����� �� �������
  24.09.2003 KM
    - Transform() ����������� ��������������� �� hex � ������.
  23.09.2003 KM
    + Transform() - ����������� ������ � hex ������������� � �������.
  12.09.2003 SVS
    ! ��������� ������ CheckFolder, �.�. �����������
      GetPathRootOne(Path,FindPath);
      if(GetFileAttributes(FindPath)!=0xFFFFFFFF)
        return CHKFLD_EMPTY;
      �������������� ��� ��������� ��������, �� �������� � �.�. � ���
      �������������� �����.
    - BugZ#951 - ������ ��� ������ � �������(?) dns-������
  02.09.2003 SVS
    ! ��������� ��������� CheckFolder() - ����� ��������� :-)
  02.09.2003 SVS
    ! ���, ������ ���� ��������� :-(
      ������� ����� FolderContentReady - ���� ���� �� CheckFolder!!!
    ! � CheckFolder - �������� ���� "const"
  02.09.2003 SVS
    ! � ������� CheckShortcutFolder ��������� �������� Silent - ����� ��������� ���� :-)
    + ����� ������� FolderContentReady(const char *Dir) - ���������� TRUE, ����
      ������� �������� ����� "���������"
    - BugZ#743 - ������� �� ����������� ������������ ���� �� �������� �� ������
      ����� ������� SetCurrentDirectory() "��������" ������� �����
      �������� FolderContentReady()
  21.08.2003 SVS
    - BugZ#933 - ������� CD � ������� ����������
  15.06.2003 SVS
    ! ����� ������ GetDirInfo - ����� ��� ��� ����������� ��������!
      (�������� ��� ���� ��������)
  14.06.2003 SVS
    ! FRS_SCANJUNCTION -> FRS_SCANSYMLINK
  14.06.2003 SVS
    ! ��� FRS_SCANJUNCTION ������ ������� ����!
    ! ���������� ������� CheckUpdateAnotherPanel - ���� ������ � ���������:
      ��������_������         ���������_������
      D:\FAR\1                D:\FAR\170
      ���� ������� �� �������� '1', �� ��������� ���� ����������� :-(
      ������ ���������� ������ ��������, �.�. D:\FAR
  06.06.2003 SVS
    ! GetFileOwner ������ �� mix.cpp � fileowner.cpp
  01.06.2003 SVS
    ! � FarRecursiveSearch() ����� ����� �������� ������������ ScanTree - FRS_SCANJUNCTION.
  17.04.2003 SVS
    + IsLocalRootPath()
  26.01.2003 IS
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  06.01.03 SVS
    ! ����������� � ������ SaveScreen � GetDirInfo (� ������ ��� GlobalSaveScrPtr
      ������� Discard ������ ���������� - � GetInputRecord ��� ��������� ������
      WINDOW_BUFFER_SIZE_EVENT)
  21.12.2002 SVS
    - ���� � �����������, ��� ������ GetDirInfo
    ! ������� �������� DontRedrawFrame � ������� GetDirInfo -
      "�� ��������� ������!"
  17.12.2002 SVS
    - BugZ#736 - ������� ���� �� ������ ��� ���������� QView
      ����� ��� ����, ��� ������� ��� �� ��������!
    - BugZ#728 - ������������ �� ����� ������������ ���������� ��� �����������
      (��������� �����������)
  11.12.2002 SVS
    - ����� ������� � KEY_CONSOLE_BUFFER_RESIZE (������������ ��������� ������� �������)
    - � PR_DrawGetDirInfoMsg ���� ���� - ������ Param2 ������ Param1
    ! � ConvertDate() �������� ����� sprintf`�
  07.11.2002 IS
    + FarChDir: ������������� ������ ��� / �� \ (������� �������� bugz#568)
  13.10.2002 IS
    - ��� � Add_PATHEXT: ���������� ������ � ������ ����, ����� ����������
      �� strstr � GetCommaWord - �� ��� ������ ��������, � ���������,
      �� �������� ��������� �� ����� "%pathext%,*.lnk,*.pif,*.awk,*.pln",
      ���� %pathext% ��������� ".pl", �.�. ��� ��������� ������� � "*.pln"
  06.07.2002 VVM
    ! ���������� PathMayBeAbsolute
  18.06.2002 SVS
    ! ������� IsFolderNotEmpty ������������� � CheckFolder � ������ �����
      ������ ������, ��� ���������� ��������� FolderNotEmpty, � ������
      ���������:
       CHKFLD_NOTACCESS = -1  - ��� �������
       CHKFLD_EMPTY     =  0  - �����
       CHKFLD_NOTEMPTY  =  1  - �� �����
      ����� �������������� ������� ��� �� �������� - �����, ������� �����
     (��� ������ ������� "����������������") ScanTree ������� �� ������
     ������� Win32-������� Find*File
  30.05.2002 SVS
    + ShellUpdatePanels � CheckUpdateAnotherPanel �������� �� delete.cpp
      � ��������������� ������� � mix.cpp
  28.05.2002 SVS
    + IsLocalPath()
    ! ���������� PathMayBeAbsolute (�������� ����� ������� strlen)
  25.05.2002 IS
    ! ������ �������� � ConvertDate ������ ������ �� ���������
  22.05.2002 SVS
    + IsDiskInDrive()
  29.04.2002 SVS
    - BugZ#239 - Folder shortcuts ��� �������������� �����
  26.04.2002 SVS
    - BugZ#484 - Addons\Macros\Space.reg (��� ��������� �������)
  09.04.2002 SVS
    ! ��������� ��� DriveLocalToRemoteName � ����� ������ �� ������
      � ���� ������ ������.
  05.04.2002 SVS
    + CheckShortcutFolder() - ����� ���������������
  26.03.2002 DJ
    ! ScanTree::GetNextName() ��������� ������ ������ ��� ����� �����
  22.03.2002 SVS
    ! ������� ������� FarBsearch, FarSscanf, FarSprintf, FarQsortEx,
      FarQsort, FarAtoi64, FarAtoi, FarItoa64, FarItoa �� mix.cpp
      � farrtl.cpp
    ! ������� DeleteFileWithFolder(), DeleteDirTree() �� mix.cpp �
      delete.cpp ��� ����� �� �����.
    ! ������� CharBufferToSmallWarn, RawConvertShortNameToLongName,
      ConvertNameToFull, ConvertNameToReal, ConvertNameToShort ��������
      �� mix.cpp � cvtname.cpp (��������� � ��������� ������ ;-))
  21.03.2002 DJ
    + GetDirInfo() ������������ � ��������� �������
  20.03.2002 SVS
    + FarGetCurDir()
    ! GetCurrentDirectory -> FarGetCurDir
  20.03.2002 SVS
    - BugZ#111 - �������� � ������� �����.
  20.03.2002 DJ
    ! � GetDirInfo() ������ . ������� �������� ���� ��������, ������� ���������
  01.03.2002 SVS
    ! ���� ������ ���� ������� �������� ���������� ����� - FarMkTempEx
  22.02.2002 SVS
    + ������� ������� ToPercent64() � filelen64()
  15.02.2002 IS
    + ����� �������� ChangeDir � FarChDir, ���� FALSE, �� �� ������ �������
      ����, � ������ ������������� ���������� ���������. �� ��������� - TRUE.
  15.02.2002 VVM
    ! ���� ������ �� ���������� � �����, �� ExpandEnvironmetStr ������� ��.
  15.02.2002 VVM
    ! ��������� ���������, �����
  05.02.2002 SVS
    ! � DeleteFileWithFolder �������� ����� ���� const
  22.01.2002 IS
    ! ������� ������� ���������� ��� ��������� ����� ���� "�����:" � FarChDir.
  14.01.2002 SVS
    - BugZ#238 - ������� ����
  14.01.2002 IS
    + FarChDir - ��������� ������� ����� � �������� � ������������
      ��������������� ���������� ���������. � ������ ������ ������������
      �� ����.
  26.12.2001 SVS
    - ...����� ����� � ������ cd //host/share... ��������
      C:\\host\share
  06.12.2001 SVS
    ! PrepareDiskPath() - ����� ���.�������� - ������������ ������ ������
  03.12.2001 SVS
    - ��������� ���� � RawConvertShortNameToLongName() - ��� ���������,
      ������� ���� ������! ��������, ����� "D:\1", ������� ���������� "D:\"
  03.12.2001 DJ
    - RawConvertShortNameToLongName() ���������� ������, ���� �� ���
      ������� ����, ��������������� �� \
  02.12.2001 SVS
    ! ��������� � RawConvertShortNameToLongName() ��� ������� ������
      "C:\\", ����� ������ ������ ��������� � ��������� dest
    ! ������� ������� ������� PrepareDiskPath() (�������� �����������
      ���� �������)
  26.11.2001 SVS
    + PrepareDiskPath()
  15.11.2001 OT
    ����������� ��������� cd c:\ �� �������� ��������� �������
  02.11.2001 SVS
    ! ConvertNameToReal() ���������� GetReparsePointInfo() - �������� Dest
      ����� �� ���������.
  28.10.2001 SVS
    ! ConvertNameToShort() - ��� �� ������� �������� ����� � �.�. �� �����
      � DOS-��������, �� ����������� ��� � ������� �������.
  21.10.2001 SVS
    + CALLBACK-������� ��� ���������� �� BugZ#85
  10.10.2001 SVS
    ! ����� ����, ������������� �� "��������" ������� ������ ��������
      � ��������� ������ execute.cpp
  05.10.2001 IS
    - ���: �� ���������������� ������ ����� ������� ��������� ��������, �������
      ����� ������ � �� ��������������� ��� ����� ������ ����������.
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  26.09.2001 SVS
    ! ������� �������� ������ ���������� ������ � ConvertNameToReal()
  24.09.2001 SVS
    - ���� � ConvertNameToReal() (��� ��, �����, ���� :-((
  24.09.2001 SVS
    - ���� � ConvertNameToReal().
      �������� "����������" ������� - �������� ������������ �
      �����, � �� � ������ ������ - ��� ����� ������.
  19.09.2001 SVS
    - ����������� � ����������� RawConvertShortNameToLongName() �
      ConvertNameToReal() �������� ��������� :-(
  14.09.2001 SVS
    ! ��� ������������ � ConvertNameToReal() �������� ����� �������
      RawConvertShortNameToLongName().
      ���� ����� ������� ������� - ��������� ��� �����
      (�� � ��� ��������� ������������!!!)
  12.09.2001 SVS
    + ConvertNameToReal() - ����������� Src � ������ �������� ���� �
      ������ reparse point � Win2K; ���� OS ����, �� ���������� �������
      ConvertNameToFull()
  07.09.2001 VVM
    + ����� ��������� �� "��������" ���������� ���������� ���������.
  30.07.2001 IS
    !  ������������������ FarRecursiveSearch
       1. ��������� ������������ ����������.
       2. ������ ��������� ��������� �� ������� �� ����� ������
       3. ����� ����� ���� ������������ ���������� ���� (�� ��������,
          ������������� � ��.). ����� ���� ��������� ����� ������, �����������
          �������� ��� ������ � �������, ����� ��������� ����� ����������,
          ����� ��������� ����� � �������. ������, ��� ��� � ������ ���� :-)
  06.07.2001 IS
    + ����������� � �������������� �������� � Add_PATHEXT
  04.07.2001 SVS
    ! ����� ��������������� ���� ��� ���� ����� ������� � syslog.cpp
  02.07.2001 IS
    + RawConvertShortNameToLongName
  25.06.2001 IS
    ! ��������� const
  17.06.2001 IS
    - ��� � ExpandEnvironmentStr: �� ����������� ��, ���
      ExpandEnvironmentStrings ���������� �������� ���������� ��������� � ANSI
      ������ ���������� ��������������� �������������, �����
      ExpandEnvironmentStr ���������� ������ � OEM, � �� � ��������� ���������.
  14.06.2001 KM
    ! ��������� ������� ����. ����� ������ ������.
  08.06.2001 SVS
    + �������� ���� ����� ���������� ����-�� ��������, ��� �� ������� ������
      ������� �����������.
  30.05.2001 SVS
    ! ShellCopy::CreatePath �������� �� ������ � �������� ������� CreatePath()
  21.05.2001 OT
    ! ����������� ��������� ������� ������� � Execute()
  17.05.2001 SKV
    + ��� detach console ������ ���� ������ "��������" hotkey
    - ���� ��� detach console ������ "������" ctrl,alt,shift,
      �� ����������� �� ������.
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  01.05.2001 SVS
    ! mktemp - ������� ����� ��������� ������ � ������� �������� (strupr)
  28.04.2001 VVM
    + GetSubstName() ��������� ��� ��������
  24.04.2001 SVS
    - ���� � ��������� (� ������ DJ)
  13.04.2001 VVM
    + ���� CREATE_DEFAULT_ERROR_MODE ��� CreateProcess.
  08.06.2001 SVS
    ! � Add_PATHEXT() ���������� ���������������� GetCommaWord().
    ! ExpandPATHEXT() ������� �� �������������.
    ! � ExpandEnvironmentStr(), ConvertNameToFull()
      ��������� alloca() ������ malloc().
  06.06.2001 SVS
    + ExpandPATHEXT() - ���������� ���������� ����� � ������ %PATHEXT%
      ��� ���� �������� %PATHEXT% �������� � ���������� �������.
  03.04.2001 SVS
    + Add_PATHEXT()
  30.03.2001 SVS
    + FarGetLogicalDrives - �������� ������ GetLogicalDrives, � ������
      ������� ���������� ������
  24.03.2001 tran
    + qsortex
  20.03.2001 tran
    + FarRecursiveSearch - �������� void *param
  20.03.2001 SKV
    - ��� ���� ���� � �������� ������� ��� ������.
  16.03.2001 SVS
    + ������� DriveLocalToRemoteName() - �������� �� ����� ����� RemoteName
  15.03.2001 SKV
    - ���� � �������� ������� ��� detach far console.
  12.03.2001 SVS
    ! ��������� � ����� � ����������� � ������ int64
  02.03.2001 IS
    ! ��������� ExpandEnvironmentStr, ��� �� ����������� ��� ��� - ����� ����
      ���������� �����.
  21.02.2001 VVM
    ! GetFileOwner()::��� ��/2000 ���������� Needed ���������������
      ���������� �� ���������.
  14.02.2001 SKV
    ! ��������� ����� ��������� �������.
  12.02.2001 SKV
    + ��������� ���-������ ������� �� ����������������
       �������� � ��� �����������.
  23.01.2001 SVS
    ! ExpandEnvironmentStr ����� ����������� �������� ������...
    ! ��������� ����� ������� VK_F16 ��� ������������ ����!
  23.01.2001 SVS
    ! ������� ��������� ������ �������� ���������� ������ �
      ExpandEnvironmentStr()
  21.01.2001 SVS
    ! ������� GetString - ��������� � stddlg.cpp
  05.01.2001 SVS
    ! ������� GetSubstName - ��������� � flink.cpp
    ! ������� InsertCommas, PointToName, GetPathRoot, CmpName, ConvertWildcards,
      QuoteSpace, QuoteSpaceOnly, TruncStr, TruncPathStr, Remove???Spaces,
      HiStrlen, AddEndSlash, NullToEmpty, CenterStr, GetCommaWord,
      RemoveHighlights, IsCaseMixed, IsCaseLower, Unquote,
      ��������� � strmix.cpp
  03.01.2001 SVS
    ! ������� SetFarTitle, ScrollBar, ShowSeparator
      ��������� � interf.cpp
    ! ������� MkLink, GetNumberOfLinks
      ��������� � flink.cpp
  03.01.2001 SVS
    ! ConvertDate - ����������� �������� ������� (���� �� �� ���� ���������)
  22.12.2000 SVS
    ! ������� ��������� ����:
      KeyNameToKey -> keyboard.cpp
      InputRecordToKey -> keyboard.cpp
      KeyToText -> keyboard.cpp
      ���, ��� �������� SysLog -> syslog.cpp
      Eject -> eject.cpp
      Xlat  -> xlat.cpp
      *Clipboard* -> clipboard.cpp
  21.12.2000 SVS
    + KEY_DTDAY, KEY_DTMONTH, KEY_DTYEAR - ��������� ���������� � �������� :-)
  14.12.2000 SVS
    + �������� ��� ��� ���������� Eject ������� ��������� ���
      Win9x & WinNT/2K
  06.12.2000 IS
    ! ������ ������� AddEndSlash �������� � ������ ������ ������, �����
      ���������� ��������� ��� ������������� ��������� ����� �� �����, �������
      ����������� ����.
  24.11.2000 SVS
    ! XLat ������� ��������� ����������� :-))) ��� �� �� �������� �� �������!
  08.11.2000 SVS
    - ���� � ������� ConvertNameToFull.
  04.11.2000 SVS
    + XLAT_SWITCHKEYBBEEP � XLat �������������.
    ! ��������� �������� � FarBsearch, InputRecordToKey, FarQsort, FarSprintf,
      FarAtoi64, FarAtoi, FarItoa64, FarItoa, KeyNameToKey, KeyToText,
      FarSscanf, AddEndSlash, RemoveTrailingSpaces, RemoveLeadingSpaces,
      TruncStr, TruncPathStr, QuoteSpaceOnly
  03.11.2000 OT
    ! ����������� ����������� ������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  26.10.2000 SVS
    ! � MkTemp ������� ����� ������� ���������� � ANSI
  25.10.2000 SVS
    ! ��������� OpenLogStream
    ! � MkTemp ������� ����� ���� � �� ������, ��� ��� ��������� � ANSI
  25.10.2000 IS
    ! ������� mktemp �� ����� ��������������� ������� �������, �.�. ����������
      ������� �������� � ������ (�������� �� Multiarc'�)
  23.10.2000 SVS
    ! ����������� ������ SysLog :-)
      ���������� � ������� ���������� ����� AT ;-)
  20.10.2000 SVS
    ! ProcessName: Flags ������ ���� DWORD, � �� int
  20.10.2000 SVS
    + SysLog
  16.10.2000 tran
    + PasteFromClipboardEx(int max);
  09.10.2000 IS
    + ����� ������� ��� ��������� ����� �����: ProcessName, ConvertWildcards
  27.09.2000 skv
    + DeleteBuffer. ������� ��, ��� ������ PasteFromClipboard.
  24.09.2000 SVS
    + ������� KeyNameToKey - ��������� ���� ������� �� �����
      ���� ��� �� ����� ��� ��� ������ - ������������ -1
  20.09.2000 SVS
    ! ������ FolderPresent (����, ������ ����� ������� :-(
  20.09.2000 SVS
    ! ��������� � ������� Xlat()
  19.09.2000 SVS
    + ������� FolderPresent - "���������� �� �������"
  19.09.2000 SVS
    + IsFolderNotEmpty ������� "�������"
    ! ������� FarMkTemp - ���������!
  18.09.2000 skv
    + � IsCommandExeGUI �������� �� ������� .bat � .cmd � ���. ����������
  18.09.2000 SVS
    ! FarRecurseSearch -> FarRecursiveSearch
    ! ����������� �������� � ������� FarMkTemp :-)))
  14.09.2000 SVS
    + ������� FarMkTemp - ��������� ����� ���������� ����� � ������ �����.
  10.09.2000 SVS
    ! KeyToText ���������� BOOL
  10.09.2000 tran
    + FSF/FarRecurseSearch
  10.09.2000 SVS
    ! �������-�� ������� ���������� ��� ��� QWERTY -> Xlat.
  08.09.2000 SVS
    - QWERTY - �������� ������� :-)))
    ! QWERTY -> Transliterate
    ! QWED_SWITCHKEYBLAYER -> EDTR_SWITCHKEYBLAYER
    + KEY_CTRLSHIFTDEL, KEY_ALTSHIFTDEL
  07.09.2000 SVS
    + ������� GetFileOwner ���� �������� �������� :-)
    + ������� GetNumberOfLinks ���� �������� �������� :-)
    + �������� FarBsearch ��� �������� (������� bsearch)
  05.09.2000 SVS 1.19
    + QWERTY - ������� ������������� QWERTY<->������
  31.08.2000 tran 1.18
    + InputRecordToKey
  29.08.2000 SVS
    - ������� ������������ ������� FarSscanf
  28.08.2000 SVS
    ! ��������� ��� FarQsort
    ! �� FarAtoa64, �� FarAtoi64
    + FarItoa64
  25.08.2000 SVS
    ! ������� GetString ����� ��� �������������� ����� (FIB_BUTTONS) ����������
      ��������� � ������ <Ok> & <Cancel>
  24.08.2000 SVS
    + � ������� KeyToText ��������� ��������� ������
      KEY_CTRLALTSHIFTPRESS, KEY_CTRLALTSHIFTRELEASE
  09.08.2000 SVS
    + FIB_NOUSELASTHISTORY - ���� ��� ������������� ���� �������� ��
      ������� �������� ��������!!! (int function GetString())
  01.08.2000 SVS
    ! ������� ����� ������ GetString ����� ���� �������� ��� ���� ������
    ! �������������� ��������� � KeyToText - ������ ������
  31.07.2000 SVS
    ! ������� GetString ����� ��� ���� �������� - ��������� �� ���������� �����!
    ! ���� � GetString ������ History, �� ����������� ��� � DIF_USELASTHISTORY
  25.07.2000 SVS
    ! ������� KeyToText ������� ���������������� - ����� � ������ FSF
    ! �������, ���������� � ������ FSF ������ ����� WINAPI!!!
  23.07.2000 SVS
    ! ������� GetString ����� ����� WINAPI
  18.07.2000 tran 1.08
    ! ������� ���� ��������� � ScrollBar
  13.07.2000 IG
    - � VC, ������, ������ ������� ���: 0x4550 == 'PE', ����
      ������ �������� ��������� (������� IsCommandExeGUI)
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  07.07.2000 tran
    - trap under win2000, or console height > 210
      bug was in ScrollBar ! :)))
  07.07.2000 SVS
    + �������������� ������� ��������� �����: RemoveExternalSpaces
    ! ������� ��� 2-� �������:
        RemoveLeadingSpaces
        RemoveTrailingSpaces
      ���������� char*
  05.07.2000 SVS
    + ��������� ������� ExpandEnvironmentStr
  28.06.2000 IS
    ! ������� Unquote ����� �������������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "treelist.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "savefpos.hpp"
#include "chgprior.hpp"
#include "filepanels.hpp"
#include "filelist.hpp"
#include "panel.hpp"
#include "scantree.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"
#include "CFileMask.hpp"
#include "constitle.hpp"
#include "udlist.hpp"
#include "manager.hpp"
#include "lockscrn.hpp"
#include "lasterror.hpp"
#include "RefreshFrameManager.hpp"
#include "filefilter.hpp"

long filelen(FILE *FPtr)
{
  SaveFilePos SavePos(FPtr);
  fseek(FPtr,0,SEEK_END);
  return(ftell(FPtr));
}

__int64 filelen64(FILE *FPtr)
{
  SaveFilePos SavePos(FPtr);
  fseek64(FPtr,0,SEEK_END);
  return(ftell64(FPtr));
}


UserDefinedList *SaveAllCurDir(void)
{
  UserDefinedList *DirList=new UserDefinedList(0,0,0);
  if(!DirList)
    return NULL;

  char CurDir[NM*2], Drive[4]="=A:";
  for(int I='A'; I <= 'Z'; ++I)
  {
    Drive[1]=I;
    DirList->AddItem(Drive);
    char *Ptr=CurDir;
    if(!GetEnvironmentVariable(Drive,CurDir,sizeof(CurDir)))  // ���������
      Ptr="\x01";
    DirList->AddItem(Ptr);
  }
  return DirList;
}

void RestoreAllCurDir(UserDefinedList *DirList)
{
  if(!DirList)
    return;

  char Drive[NM*2];
  const char *NamePtr;
  while(NULL!=(NamePtr=DirList->GetNext()))
  {
    xstrncpy(Drive,NamePtr,sizeof(Drive)-1);
    if((NamePtr=DirList->GetNext()) != NULL)
      SetEnvironmentVariable(Drive,(*NamePtr == '\x1'?"":NamePtr));
  }
  delete DirList;
}


/* $ 14.01.2002 IS
   ��������� ������� ����� � �������� � ������������ ��������������� ����������
   ���������. � ������ ������ ������������ �� ����.
*/
/* $ 22.01.2002 IS
   + ���������� �������������� ���� ���� "�����:"
*/
/* $ 15.02.2002 IS
   + ����� �������� ChangeDir, ���� FALSE, �� �� ������ ������� ����, � ������
     ������������� ���������� ���������. �� ��������� - TRUE.
*/
/* $ 07.11.2002 IS
   + ������������� ������ ��� / �� \ (������� �������� bugz#568)
*/
BOOL FarChDir(const char *NewDir, BOOL ChangeDir)
{
  if(!NewDir || *NewDir == 0)
    return FALSE;

  BOOL rc=FALSE;

  char CurDir[NM*2], Drive[4]="=A:";
  if(isalpha(*NewDir) && NewDir[1]==':' && NewDir[2]==0)// ���� ������� ������
  {                                                     // ����� �����, �� ����
    Drive[1]=toupper(*NewDir);                          // ������� �� ����������
    if(GetEnvironmentVariable(Drive,CurDir,sizeof(CurDir)))  // ���������
      FAR_CharToOem(CurDir,CurDir);
    else
    {
      sprintf(CurDir,"%s\\",NewDir); // ��� ������� ������������ � ������ �����
      char *Chr=CurDir;
      while(*Chr)
      {
        if(*Chr=='/') *Chr='\\';
        ++Chr;
      }
    }
    *CurDir=toupper(*CurDir);
    if(ChangeDir)
    {
      if(CheckFolder(CurDir) > CHKFLD_NOTACCESS)
        rc=SetCurrentDirectory(CurDir);
    }
  }
  else
  {
    xstrncpy(CurDir,NewDir,sizeof(CurDir)-1);
    if(!strcmp(CurDir,"\\"))
      FarGetCurDir(sizeof(CurDir),CurDir); // ����� ����� ������
    char *Chr=CurDir;
    while(*Chr)
    {
      if(*Chr=='/') *Chr='\\';
      ++Chr;
    }
    if(ChangeDir)
    {
      if(CheckFolder(NewDir) > CHKFLD_NOTACCESS)
      {
        char *ptr;
        char FullDir[sizeof(CurDir)];
        GetFullPathName(NewDir,sizeof(FullDir),FullDir,&ptr);
        PrepareDiskPath(FullDir,sizeof(FullDir)-1);

        DWORD att1 = GetFileAttributes(NewDir);
        DWORD att2 = GetFileAttributes(FullDir);

        if (att1 != att2)
          rc=SetCurrentDirectory(NewDir);
        else
          rc=SetCurrentDirectory(FullDir);
      }
    }
  }

  if(rc || !ChangeDir)
  {
    if ((!ChangeDir || GetCurrentDirectory(sizeof(CurDir),CurDir)) &&
        isalpha(*CurDir) && CurDir[1]==':')
    {
      Drive[1]=toupper(*CurDir);
      FAR_OemToChar(CurDir,CurDir); // ��������� SetEnvironmentVariable ������ ���� ANSI
      SetEnvironmentVariable(Drive,CurDir);
  //    rc=0;
    }
  }
  return rc;
}
/* IS 07.11.2002 $ */
/* IS 15.02.2002 $ */
/* IS 22.01.2002 $ */
/* IS 14.01.2002 $ */

/* $ 20.03.2002 SVS
 ������� ������ ������� ��������� �������� ����.
 ��� ���������� ���� ��������� ����� ����� � uppercase
*/
DWORD FarGetCurDir(DWORD Length,char *Buffer)
{
  DWORD Result=GetCurrentDirectory(Length,Buffer);
  if(Result && isalpha(*Buffer) && Buffer[1]==':' && (Buffer[2]==0 || Buffer[2]== '\\'))
    *Buffer=toupper(*Buffer);
  return Result;
}
/* SVS 20.03.2002 $ */

DWORD NTTimeToDos(FILETIME *ft)
{
  WORD DosDate,DosTime;
  FILETIME ct;
  FileTimeToLocalFileTime(ft,&ct);
  FileTimeToDosDateTime(&ct,&DosDate,&DosTime);
  return(((DWORD)DosDate<<16)|DosTime);
}


void ConvertDate(const FILETIME &ft,char *DateText,char *TimeText,int TimeLength,
                 int Brief,int TextMonth,int FullYear,int DynInit)
{
  static int WDateFormat,WDateSeparator,WTimeSeparator;
  static int Init=FALSE;
  static SYSTEMTIME lt;
  int DateFormat,DateSeparator,TimeSeparator;
  if (!Init)
  {
    WDateFormat=GetDateFormat();
    WDateSeparator=GetDateSeparator();
    WTimeSeparator=GetTimeSeparator();
    GetLocalTime(&lt);
    Init=TRUE;
  }
  DateFormat=DynInit?GetDateFormat():WDateFormat;
  DateSeparator=DynInit?GetDateSeparator():WDateSeparator;
  TimeSeparator=DynInit?GetTimeSeparator():WTimeSeparator;

  int CurDateFormat=DateFormat;
  if (Brief && CurDateFormat==2)
    CurDateFormat=0;

  SYSTEMTIME st;
  FILETIME ct;

  if (ft.dwHighDateTime==0)
  {
    if (DateText!=NULL)
      *DateText=0;
    if (TimeText!=NULL)
      *TimeText=0;
    return;
  }

  FileTimeToLocalFileTime(&ft,&ct);
  FileTimeToSystemTime(&ct,&st);

  if (TimeText!=NULL)
  {
    const char *Letter="";
    if (TimeLength==6)
    {
      Letter=(st.wHour<12) ? "a":"p";
      if (st.wHour>12)
        st.wHour-=12;
      if (st.wHour==0)
        st.wHour=12;
    }
    if (TimeLength<7)
      sprintf(TimeText,"%02d%c%02d%s",st.wHour,TimeSeparator,st.wMinute,Letter);
    else
    {
      char FullTime[100];
      sprintf(FullTime,"%02d%c%02d%c%02d.%03d",st.wHour,TimeSeparator,
              st.wMinute,TimeSeparator,st.wSecond,st.wMilliseconds);
      sprintf(TimeText,"%.*s",TimeLength,FullTime);
    }
  }

  if (DateText!=NULL)
  {
    int Year=st.wYear;
    if (!FullYear)
      Year%=100;
    if (TextMonth)
    {
      char *Month=MSG(MMonthJan+st.wMonth-1);
      switch(CurDateFormat)
      {
        case 0:
          sprintf(DateText,"%3.3s %2d %02d",Month,st.wDay,Year);
          break;
        case 1:
          sprintf(DateText,"%2d %3.3s %02d",st.wDay,Month,Year);
          break;
        default:
          sprintf(DateText,"%02d %3.3s %2d",Year,Month,st.wDay);
          break;
      }
    }
    else
    {
      int p1,p2,p3=Year;
      switch(CurDateFormat)
      {
        case 0:
          p1=st.wMonth;
          p2=st.wDay;
          break;
        case 1:
          p1=st.wDay;
          p2=st.wMonth;
          break;
        default:
          p1=Year;
          p2=st.wMonth;
          p3=st.wDay;
          break;
      }
      sprintf(DateText,"%02d%c%02d%c%02d",p1,DateSeparator,p2,DateSeparator,p3);
    }
  }

  if (Brief)
  {
    DateText[TextMonth ? 6:5]=0;
    if (lt.wYear!=st.wYear)
      sprintf(TimeText,"%5d",st.wYear);
  }
}


int GetDateFormat()
{
  char Info[100];
  GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IDATE,Info,sizeof(Info));
  return(atoi(Info));
}


int GetDateSeparator()
{
  char Info[100];
  GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SDATE,Info,sizeof(Info));
  return(*Info);
}


int GetTimeSeparator()
{
  char Info[100];
  GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_STIME,Info,sizeof(Info));
  return(*Info);
}


int ToPercent(unsigned long N1,unsigned long N2)
{
  if (N1 > 10000)
  {
    N1/=100;
    N2/=100;
  }
  if (N2==0)
    return(0);
  if (N2<N1)
    return(100);
  return((int)(N1*100/N2));
}

int ToPercent64(__int64 N1,__int64 N2)
{
  if (N1 > _i64(10000))
  {
    N1/=_i64(100);
    N2/=_i64(100);
  }
  if (N2==_i64(0))
    return(_i64(0));
  if (N2<N1)
    return(100);
  return((int)(N1*_i64(100)/N2));
}



/* $ 09.10.2000 IS
    + ����� ������� ��� ��������� ����� �����
*/
// ���������� ��� �����: �������� � ������, �������, ������������� �� �����
int WINAPI ProcessName(const char *param1, char *param2, DWORD flags)
{
  int skippath=flags&PN_SKIPPATH;

  if(flags&PN_CMPNAME)
    return CmpName(param1, param2, skippath);

  if(flags&PN_CMPNAMELIST)
  {
    int Found=FALSE;
    char FileMask[NM];
    const char *MaskPtr;
    MaskPtr=param1;

    while ((MaskPtr=GetCommaWord(MaskPtr,FileMask))!=NULL)
      if (CmpName(FileMask,param2,skippath))
      {
        Found=TRUE;
        break;
      }
    return Found;
  }

  if(flags&PN_GENERATENAME)
   return ConvertWildcards(param1, param2, flags & 0xFF);

  return FALSE;
}
/* IS $ */

int GetFileTypeByName(const char *Name)
{
  HANDLE hFile=FAR_CreateFile(Name,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
                          NULL,OPEN_EXISTING,0,NULL);
  if (hFile==INVALID_HANDLE_VALUE)
    return(FILE_TYPE_UNKNOWN);
  int Type=GetFileType(hFile);
  CloseHandle(hFile);
  return(Type);
}


static void DrawGetDirInfoMsg(char *Title,const char *Name)
{
  Message(0,0,Title,MSG(MScanningFolder),Name);
  PreRedrawParam.Param1=Title;
  PreRedrawParam.Param2=Name;
}

static void PR_DrawGetDirInfoMsg(void)
{
  DrawGetDirInfoMsg((char *)PreRedrawParam.Param1,(char *)PreRedrawParam.Param2);
}

int GetDirInfo(char *Title,
               const char *DirName,
               unsigned long &DirCount,
               unsigned long &FileCount,
               int64 &FileSize,
               int64 &CompressedFileSize,
               int64 &RealSize,
               unsigned long &ClusterSize,
               clock_t MsgWaitTime,
               DWORD Flags)
               //int EnhBreak,
               //BOOL DontRedrawFrame,
               //int ScanSymLink,
               //int UseFilter)
{
  char FullDirName[NM],DriveRoot[NM];
  char FullName[NM],CurDirName[NM],LastDirName[NM];
  if (ConvertNameToFull(DirName,FullDirName, sizeof(FullDirName)) >= sizeof(FullDirName))
    return -1;

  SaveScreen SaveScr;
  UndoGlobalSaveScrPtr UndSaveScr(&SaveScr);

  ScanTree ScTree(FALSE,TRUE,
                  (Flags&GETDIRINFO_SCANSYMLINKDEF?-1:(Flags&GETDIRINFO_SCANSYMLINK)),
                  Flags&GETDIRINFO_USEDALTFOLDERNAME);
  WIN32_FIND_DATA FindData;
  int MsgOut=0;
  clock_t StartTime=clock();

  SetCursorType(FALSE,0);
  GetPathRoot(FullDirName,DriveRoot);

  /* $ 20.03.2002 DJ
     ��� . - ������� ��� ������������� ��������
  */
  const char *ShowDirName = DirName;
  if (DirName[0] == '.' && DirName[1] == 0)
  {
    char *p = strrchr (FullDirName, '\\');
    if (p)
      ShowDirName = p + 1;
  }
  /* DJ */

  ConsoleTitle OldTitle;
  RefreshFrameManager frref(ScrX,ScrY,MsgWaitTime,Flags&GETDIRINFO_DONTREDRAWFRAME);

  PREREDRAWFUNC OldPreRedrawFunc=PreRedrawFunc;

  if ((ClusterSize=GetClusterSize(DriveRoot))==0)
  {
    DWORD SectorsPerCluster=0,BytesPerSector=0,FreeClusters=0,Clusters=0;

    if (GetDiskFreeSpace(DriveRoot,&SectorsPerCluster,&BytesPerSector,&FreeClusters,&Clusters))
      ClusterSize=SectorsPerCluster*BytesPerSector;
  }

  // ��������� ��������� ��� ���������
  *LastDirName=0;
  *CurDirName=0;

  // �������� ������ �������
  FileFilter Filter;

  DirCount=FileCount=0;
  FileSize=CompressedFileSize=RealSize=0;
  ScTree.SetFindPath(DirName,"*.*");

  while (ScTree.GetNextName(&FindData,FullName, sizeof (FullName)-1))
  {
    if (!CtrlObject->Macro.IsExecuting())
    {
      INPUT_RECORD rec;
      switch(PeekInputRecord(&rec))
      {
        case 0:
        case KEY_IDLE:
          break;
        case KEY_NONE:
        case KEY_ALT:
        case KEY_CTRL:
        case KEY_SHIFT:
        case KEY_RALT:
        case KEY_RCTRL:
          GetInputRecord(&rec);
          break;
        case KEY_ESC:
        case KEY_BREAK:
          GetInputRecord(&rec);
          SetPreRedrawFunc(OldPreRedrawFunc);
          return(0);
        default:
          if (Flags&GETDIRINFO_ENHBREAK)
          {
            SetPreRedrawFunc(OldPreRedrawFunc);
            return(-1);
          }
          GetInputRecord(&rec);
          break;
      }
    }

    if (!MsgOut && MsgWaitTime!=0xffffffff && clock()-StartTime > MsgWaitTime)
    {
      OldTitle.Set("%s %s",MSG(MScanningFolder), ShowDirName); // ������� ��������� �������
      SetCursorType(FALSE,0);
      SetPreRedrawFunc(PR_DrawGetDirInfoMsg);
      DrawGetDirInfoMsg(Title,ShowDirName);
      MsgOut=1;
    }

    if (FindData.dwFileAttributes & FA_DIREC)
    {
      // ������� ��������� ���������� ������ ���� �� ������� ������,
      // � ��������� ������ ��� ����� ������ � �������� ���������� ������
      if (!(Flags&GETDIRINFO_USEFILTER))
        DirCount++;
    }
    else
    {
      /* $ 17.04.2005 KM
         �������� ��������� ����� � ������� ������
      */
      if ((Flags&GETDIRINFO_USEFILTER))
      {
        if (!Filter.FileInFilter(&FindData))
          continue;
      }

      // ���������� ������� ��������� ��� ���������� ������� ������ �����,
      // ����� � ����� �������� ������ ����, ��������������� ��������
      // �������.
      if ((Flags&GETDIRINFO_USEFILTER))
      {
        char *p1=strrchr(FullName,'\\');
        if (p1)
          xstrncpy(CurDirName,FullName,p1-FullName);
        else
          xstrncpy(CurDirName,FullName,sizeof(CurDirName)-1);

        if (LocalStricmp(CurDirName,LastDirName)!=0)
        {
          DirCount++;
          xstrncpy(LastDirName,CurDirName,sizeof(LastDirName)-1);
        }
      }
      /* KM $ */

      FileCount++;
      int64 CurSize(FindData.nFileSizeHigh,FindData.nFileSizeLow);
      FileSize+=CurSize;
      if (FindData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
      {
        DWORD CompressedSize,CompressedSizeHigh;
        CompressedSize=GetCompressedFileSize(FullName,&CompressedSizeHigh);
        if (CompressedSize!=0xFFFFFFFF || GetLastError()==NO_ERROR)
          CurSize.Set(CompressedSizeHigh,CompressedSize);
      }
      CompressedFileSize+=CurSize;
      if (ClusterSize>0)
      {
        RealSize+=CurSize;
        int Slack=(CurSize%ClusterSize).PLow();
        if (Slack>0)
          RealSize+=ClusterSize-Slack;
      }
    }
  }

  SetPreRedrawFunc(OldPreRedrawFunc);
  return(1);
}


int GetPluginDirInfo(HANDLE hPlugin,char *DirName,unsigned long &DirCount,
               unsigned long &FileCount,int64 &FileSize,
               int64 &CompressedFileSize)
{
  struct PluginPanelItem *PanelItem=NULL;
  int ItemsNumber,ExitCode;
  DirCount=FileCount=0;
  FileSize=CompressedFileSize=0;
  if ((ExitCode=FarGetPluginDirList(((struct PluginHandle *)hPlugin)->PluginNumber,
      ((struct PluginHandle *)hPlugin)->InternalHandle,DirName,
      &PanelItem,&ItemsNumber))==TRUE)
  {
    for (int I=0;I<ItemsNumber;I++)
    {
      if (PanelItem[I].FindData.dwFileAttributes & FA_DIREC)
        DirCount++;
      else
      {
        FileCount++;
        int64 CurSize(PanelItem[I].FindData.nFileSizeHigh,PanelItem[I].FindData.nFileSizeLow);
        FileSize+=CurSize;
        if (PanelItem[I].PackSize==0 && PanelItem[I].PackSizeHigh==0)
          CompressedFileSize+=CurSize;
        else
        {
          int64 AddSize(PanelItem[I].PackSizeHigh,PanelItem[I].PackSize);
          CompressedFileSize+=AddSize;
        }
      }
    }
  }
  if (PanelItem!=NULL)
    FarFreeDirList(PanelItem);
  return(ExitCode);
}

/*
  ������� CheckFolder ���������� ���� ��������� ������������ ��������:

    CHKFLD_NOTFOUND   (2) - ��� ������
    CHKFLD_NOTEMPTY   (1) - �� �����
    CHKFLD_EMPTY      (0) - �����
    CHKFLD_NOTACCESS (-1) - ��� �������
    CHKFLD_ERROR     (-2) - ������ (��������� - ������ ��� ��������� ������ ��� ��������� ������������� �������)
*/
int CheckFolder(const char *Path)
{
  if(!(Path || *Path)) // �������� �� ��������
    return CHKFLD_ERROR;

  int LenFindPath=Max((int)strlen(Path),2048)+8;
  char *FindPath=(char *)alloca(LenFindPath); // ����� alloca - ����� _�����_ ������� �� ��� ��� ���.
  if(!FindPath)
    return CHKFLD_ERROR;

  HANDLE FindHandle;
  WIN32_FIND_DATA fdata;
  int Done=FALSE;

  // ��������� ����� ��� ������.
  strcpy(FindPath,Path);
  AddEndSlash(FindPath);
  strcat(FindPath,"*.*");

  // ������ �������� - ��-���� ������� �����?
  if((FindHandle=FindFirstFile(FindPath,&fdata)) == INVALID_HANDLE_VALUE)
  {
    GuardLastError lstError;
    if(lstError.Get() == ERROR_FILE_NOT_FOUND)
      return CHKFLD_EMPTY;

    // ����������... �� ����, ��� ���� �� ������, �.�. �� ������ ����� � ����� ���� ���� "."
    // ������� ��������� �� Root
    GetPathRootOne(Path,FindPath);
    if(!strcmp(Path,FindPath))
    {
      // �������� ��������� ������������� ������ - ��� ���� BugZ#743 ��� ������ ������ �����.
      if(GetFileAttributes(FindPath)!=0xFFFFFFFF)
      {
        if(lstError.Get() == ERROR_ACCESS_DENIED)
          return CHKFLD_NOTACCESS;
        return CHKFLD_EMPTY;
      }
    }
    strcpy(FindPath,Path);
    if(CheckShortcutFolder(FindPath,LenFindPath,FALSE,TRUE))
    {
      if(strcmp(Path,FindPath))
        return CHKFLD_NOTFOUND;
#if 0
      DWORD Attr=GetFileAttributes(Path);
      if(Attr != 0xFFFFFFFF && (Attr&FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT)
      {
        ConvertNameToFull(Path,FindPath,LenFindPath);
        GetPathRoot(FindPath,FindPath);
        // �������� ��������� ������������� ������ - ��� ���� BugZ#743 ��� ������ ������ �����.
        if(GetFileAttributes(FindPath)!=0xFFFFFFFF)
          return CHKFLD_EMPTY;
      }
#endif
    }

    return CHKFLD_NOTACCESS;
  }

  // ��. ���-�� ����. ��������� �������� �� ������ "����� �������?"
  while(!Done)
  {
    if (fdata.cFileName[0] == '.' && (fdata.cFileName[1] == 0 || fdata.cFileName[1] == '.' && fdata.cFileName[2] == 0))
      ; // ���������� "." � ".."
    else
    {
      // ���-�� ����, �������� �� "." � ".." - ������� �� ����
      FindClose(FindHandle);
      return CHKFLD_NOTEMPTY;
    }
    Done=!FindNextFile(FindHandle,&fdata);
  }

  // ���������� ������� ����
  FindClose(FindHandle);
  return CHKFLD_EMPTY;
}

char* FarMSG(int MsgID)
{
  return(Lang.GetMsg(MsgID));
}


BOOL GetDiskSize(char *Root,int64 *TotalSize,int64 *TotalFree,int64 *UserFree)
{
typedef BOOL (WINAPI *GETDISKFREESPACEEX)(
    LPCTSTR lpDirectoryName,
    PULARGE_INTEGER lpFreeBytesAvailableToCaller,
    PULARGE_INTEGER lpTotalNumberOfBytes,
    PULARGE_INTEGER lpTotalNumberOfFreeBytes
   );
  static GETDISKFREESPACEEX pGetDiskFreeSpaceEx=NULL;
  static int LoadAttempt=FALSE;
  int ExitCode=0;

  ULARGE_INTEGER uiTotalSize,uiTotalFree,uiUserFree;
  uiUserFree.u.LowPart=uiUserFree.u.HighPart=0;
  uiTotalSize.u.LowPart=uiTotalSize.u.HighPart=0;
  uiTotalFree.u.LowPart=uiTotalFree.u.HighPart=0;

  if (!LoadAttempt && pGetDiskFreeSpaceEx==NULL)
  {
    HMODULE hKernel=GetModuleHandle("kernel32.dll");
    if (hKernel!=NULL)
      pGetDiskFreeSpaceEx=(GETDISKFREESPACEEX)GetProcAddress(hKernel,"GetDiskFreeSpaceExA");
    LoadAttempt=TRUE;
  }
  if (pGetDiskFreeSpaceEx!=NULL)
  {
    ExitCode=pGetDiskFreeSpaceEx(Root,&uiUserFree,&uiTotalSize,&uiTotalFree);
    if (uiUserFree.u.HighPart>uiTotalFree.u.HighPart)
      uiUserFree.u=uiTotalFree.u;
  }

  if (pGetDiskFreeSpaceEx==NULL || ExitCode==0 ||
      uiTotalSize.u.HighPart==0 && uiTotalSize.u.LowPart==0)
  {
    DWORD SectorsPerCluster,BytesPerSector,FreeClusters,Clusters;
    ExitCode=GetDiskFreeSpace(Root,&SectorsPerCluster,&BytesPerSector,
                              &FreeClusters,&Clusters);
    uiTotalSize.u.LowPart=SectorsPerCluster*BytesPerSector*Clusters;
    uiTotalSize.u.HighPart=0;
    uiTotalFree.u.LowPart=SectorsPerCluster*BytesPerSector*FreeClusters;
    uiTotalFree.u.HighPart=0;
    uiUserFree.u=uiTotalFree.u;
  }
  TotalSize->Set(uiTotalSize.u.HighPart,uiTotalSize.u.LowPart);
  TotalFree->Set(uiTotalFree.u.HighPart,uiTotalFree.u.LowPart);
  UserFree->Set(uiUserFree.u.HighPart,uiUserFree.u.LowPart);
  return(ExitCode);
}

int GetClusterSize(char *Root)
{
  struct ExtGetDskFreSpc
  {
    WORD ExtFree_Size;
    WORD ExtFree_Level;
    DWORD ExtFree_SectorsPerCluster;
    DWORD ExtFree_BytesPerSector;
    DWORD ExtFree_AvailableClusters;
    DWORD ExtFree_TotalClusters;
    DWORD ExtFree_AvailablePhysSectors;
    DWORD ExtFree_TotalPhysSectors;
    DWORD ExtFree_AvailableAllocationUnits;
    DWORD ExtFree_TotalAllocationUnits;
    DWORD ExtFree_Rsvd[2];
  } DiskInfo;

  struct _DIOC_REGISTERS
  {
    DWORD reg_EBX;
    DWORD reg_EDX;
    DWORD reg_ECX;
    DWORD reg_EAX;
    DWORD reg_EDI;
    DWORD reg_ESI;
    DWORD reg_Flags;
  } reg;

  BOOL fResult;
  DWORD cb;

  if (WinVer.dwPlatformId!=VER_PLATFORM_WIN32_WINDOWS ||
      WinVer.dwBuildNumber<0x04000457)
    return(0);

  HANDLE hDevice = FAR_CreateFile("\\\\.\\vwin32", 0, 0, NULL, 0,
                              FILE_FLAG_DELETE_ON_CLOSE, NULL);

  if (hDevice==INVALID_HANDLE_VALUE)
    return(0);

  DiskInfo.ExtFree_Level=0;

  reg.reg_EAX = 0x7303;
  reg.reg_EDX = (DWORD)Root;
  reg.reg_EDI = (DWORD)&DiskInfo;
  reg.reg_ECX = sizeof(DiskInfo);
  reg.reg_Flags = 0x0001;

  fResult=DeviceIoControl(hDevice,6,&reg,sizeof(reg),&reg,sizeof(reg),&cb,0);

  CloseHandle(hDevice);
  if (!fResult || (reg.reg_Flags & 0x0001))
    return(0);
  return(DiskInfo.ExtFree_SectorsPerCluster*DiskInfo.ExtFree_BytesPerSector);
}



/* $ 02.03.2001 IS
   ���������� ���������� �����
   �������� � �������� ��������������� ������ ������� ������
     ExpandEnvironmentStrings.
   src � dest ����� ���� �����������
*/
DWORD WINAPI ExpandEnvironmentStr(const char *src, char *dest, size_t size)
{
  DWORD ret=0;
  if(size && src && dest)
  {
    char *tmpDest=(char *)alloca(size),
          *tmpSrc=(char *)alloca(strlen(src)+1);
    if(tmpDest && tmpSrc)
    {
      FAR_OemToChar(src, tmpSrc);
      DWORD Len = ExpandEnvironmentStrings(tmpSrc,tmpDest,size);
      if (Len && Len <= size)
      {
        const char *ptrsrc=src;
        char *ptrdest=tmpDest;

        //AY: ������������ ������ %var% �������� ���� �� ������ ������ OEM2ANSI
        //    � ����� �������������.
        //    �������� �� size �� ����� ��� ��� �� ��� ��� ���
        //    �������� ExpandEnvironmentStrings().
        while (1)
        {
          while (*ptrsrc && *ptrsrc!='%')
            *(ptrdest++) = *(ptrsrc++);

          if (*ptrsrc == '%')
          {
            const char *ptrnext=strchr(ptrsrc+1,'%');
            if (ptrnext)
            {
              Len = ptrnext-ptrsrc+1;
              FAR_OemToCharBuff(ptrsrc,tmpSrc,Len)
              tmpSrc[Len]=0;
              Len = ExpandEnvironmentStrings(tmpSrc,ptrdest,size-(ptrdest-tmpDest));

              //AY: ��� �� ��� �������� %var%?
              if (Len && strcmp(tmpSrc,ptrdest))
              {
                FAR_CharToOem(ptrdest, ptrdest);
                ptrdest += strlen(ptrdest);
                ptrsrc = ptrnext+1;
              }
              else
                *(ptrdest++) = *(ptrsrc++);
            }
            else
              *(ptrdest++) = *(ptrsrc++);
          }

          if (!*ptrsrc)
          {
            *ptrdest = 0;
            break;
          }
        }
        xstrncpy(dest, tmpDest, size-1);
      }
      else
      {
        xstrncpy(dest, src, size-1);
      }
      ret=strlen(dest);
    }
  }
  return ret;
}

#if 0
/*
In: "C:\WINNT\SYSTEM32\FOO.TXT", "%SystemRoot%"
Out: "%SystemRoot%\SYSTEM32\FOO.TXT"
*/
BOOL UnExpandEnvString(const char *Path, const char *EnvVar, char* Dest, int DestSize)
{
  int I;
  char Temp[NM*2];
  Temp[0] = 0;

  ExpandEnvironmentStr(EnvVar, Temp, sizeof(Temp));
  I = strlen(Temp);

  if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, Temp, I, Path, I) == 2)
  {
    if (strlen(Path)-I+strlen(EnvVar) < DestSize)
    {
      xstrncpy(Dest, EnvVar, DestSize-1);
      strncat(Dest, Path + I, DestSize-1);
      return TRUE;
    }
  }
  return FALSE;
}

BOOL PathUnExpandEnvStr(const char *Path, char* Dest, int DestSize)
{
  const char *StdEnv[]={
     "%TEMP%",               // C:\Documents and Settings\Administrator\Local Settings\Temp
     "%APPDATA%",            // C:\Documents and Settings\Administrator\Application Data
     "%USERPROFILE%",        // C:\Documents and Settings\Administrator
     "%ALLUSERSPROFILE%",    // C:\Documents and Settings\All Users
     "%CommonProgramFiles%", // C:\Program Files\Common Files
     "%ProgramFiles%",       // C:\Program Files
     "%SystemRoot%",         // C:\WINNT
  };

  for(int I=0; I < sizeof(StdEnv)/sizeof(StdEnv[0]); ++I)
  {
    if(UnExpandEnvironmentString(Path, StdEnv[I], Dest, DestSize))
      return TRUE;
  }
  xstrncpy(Dest, Path, DestSize-1);
  return FALSE;

}
#endif

/* $ 10.09.2000 tran
   FSF/FarRecurseSearch */
/* $ 30.07.2001 IS
     1. ��������� ������������ ����������.
     2. ������ ��������� ��������� �� ������� �� ����� ������
     3. ����� ����� ���� ������������ ���������� ���� (�� ��������,
        ������������� � ��.). ����� ���� ��������� ����� ������, �����������
        �������� ��� ������ � �������, ����� ��������� ����� ����������,
        ����� ��������� ����� � �������. ������, ��� ��� � ������ ���� :-)
*/
void WINAPI FarRecursiveSearch(const char *InitDir,const char *Mask,FRSUSERFUNC Func,DWORD Flags,void *Param)
{
  if(Func && InitDir && *InitDir && Mask && *Mask)
  {
    CFileMask FMask;
    if(!FMask.Set(Mask, FMF_SILENT)) return;

    Flags=Flags&0x000000FF; // ������ ������� ����!
    ScanTree ScTree(Flags & FRS_RETUPDIR,Flags & FRS_RECUR, Flags & FRS_SCANSYMLINK);
    WIN32_FIND_DATA FindData;
    char FullName[NM];

    ScTree.SetFindPath(InitDir,"*");
    while (ScTree.GetNextName(&FindData,FullName, sizeof (FullName)-1))
    {
      if ((FMask.Compare(FindData.cFileName) || FMask.Compare(FindData.cAlternateFileName)) &&
          Func(&FindData,FullName,Param) == 0)
          break;
    }
  }
}
/* IS $ */
/* tran 10.09.2000 $ */

/* $ 14.09.2000 SVS
 + ������� FarMkTemp - ��������� ����� ���������� ����� � ������ �����.
    Dest - �������� ���������� (������ ���� ���������� �������, �������� NM
    Template - ������ �� �������� ������� mktemp, �������� "FarTmpXXXXXX"
   ������ ���� NULL, ���� ��������� �� Dest.
*/
/* $ 18.09.2000 SVS
  �� �� ������� ������� :-)))
*/
/* $ 25.10.2000 IS
 ! ������� mktemp �� ����� ��������������� ������� �������, �.�. ����������
   ������� �������� � ������ (�������� �� Multiarc'�)
   �������� Prefix - ������, ����������� �� ������ ������� ����� ����������
   �����. ������������ ������ ������ 3 ������� �� ���� ������.
*/
char* WINAPI FarMkTemp(char *Dest, const char *Prefix)
{
  return FarMkTempEx(Dest,Prefix,TRUE);
}

/*
             v - �����
   prefXXX X X XXX
       \ / ^   ^^^\ PID + TID
        |  \------/
        |
        +---------- [0A-Z]
*/
char* FarMkTempEx(char *Dest, const char *Prefix, BOOL WithPath)
{
  if(Dest)
  {
    if(!(Prefix && *Prefix))
      Prefix="FTMP";

    char TempName[NM];
    TempName[0]=0;
    if(WithPath)
      strcpy(TempName,Opt.TempPath);
    strcat(TempName,"0000XXXXXXXX");
    memcpy(TempName+strlen(TempName)-12,Prefix,Min((int)strlen(Prefix),4));
    if (farmktemp(TempName)!=NULL)
    {
      strcpy(Dest,strupr(TempName));
      return Dest;
    }
  }
  return NULL;
}
/* IS $ */
/* SVS 18.09.2000 $ */
/* SVS $ */

/*$ 27.09.2000 skv
  + �������� ������ ����������� ����� new char[n];
    ������� ��� �������� ������������� PasteFromClipboard
*/
void WINAPI DeleteBuffer(char *Buffer)
{
  if(Buffer)delete [] Buffer;
}
/* skv$*/

// �������� �� ����� ����� RemoteName
char* DriveLocalToRemoteName(int DriveType,char Letter,char *Dest)
{
  int NetPathShown=FALSE, IsOK=FALSE;
  char LocalName[8]=" :\0\0\0",RemoteName[NM];
  DWORD RemoteNameSize=sizeof(RemoteName);

  *LocalName=Letter;
  *Dest=0;

  if(DriveType == DRIVE_UNKNOWN)
  {
    LocalName[2]='\\';
    DriveType = FAR_GetDriveType(LocalName);
    LocalName[2]=0;
  }

  if (DriveType==DRIVE_REMOTE)
  {
    SetFileApisTo(APIS2ANSI);
    if (WNetGetConnection(LocalName,RemoteName,&RemoteNameSize)==NO_ERROR)
    {
      NetPathShown=TRUE;
      IsOK=TRUE;
    }
    SetFileApisTo(APIS2OEM);
  }
  if (!NetPathShown)
    if (GetSubstName(DriveType,LocalName,RemoteName,sizeof(RemoteName)))
      IsOK=TRUE;

  if(IsOK)
  {
    FAR_CharToOem(RemoteName,RemoteName);
    strcpy(Dest,RemoteName);
  }
  return Dest;
}

/*
  FarGetLogicalDrives
  �������� ������ GetLogicalDrives, � ������ ������� ���������� ������
  HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
  NoDrives:DWORD
    ��������� 26 ��� ���������� ����� ������ �� A �� Z (������ ������ ������).
    ���� ����� ��� ������������� 0 � ����� ��� �������� 1.
    ���� A ����������� ������ ��������� ������ ��� �������� �������������.
    ��������, �������� 00000000000000000000010101(0x7h)
    �������� ����� A, C, � E
*/
DWORD WINAPI FarGetLogicalDrives(void)
{
  static DWORD LogicalDrivesMask = 0;
  DWORD NoDrives=0;
  if ((!Opt.RememberLogicalDrives) || (LogicalDrivesMask==0))
    LogicalDrivesMask=GetLogicalDrives();

  if(!Opt.Policies.ShowHiddenDrives)
  {
    HKEY hKey;
    if (RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",0,KEY_QUERY_VALUE,&hKey)==ERROR_SUCCESS && hKey)
    {
      int ExitCode;
      DWORD Type,Size=sizeof(NoDrives);
      ExitCode=RegQueryValueEx(hKey,"NoDrives",0,&Type,(BYTE *)&NoDrives,&Size);
      RegCloseKey(hKey);
      if(ExitCode != ERROR_SUCCESS)
        NoDrives=0;
    }
  }
  return LogicalDrivesMask&(~NoDrives);
}

/* $ 13.10.2002 IS
   ���������� ������ � ������ ����, ����� ���������� �� strstr �
   GetCommaWord - �� ��� ������ ��������, � ���������, �� �������� ���������
   �� ����� "%pathext%,*.lnk,*.pif,*.awk,*.pln", ���� %pathext% ���������
   ".pl", �.�. ��� ��������� ������� � "*.pln"
*/

// �������������� �������� ������� PATHEXT � ��������� :-)
// ������� ���������� ������ ����������, ��� ���� ��������� ��, ��� ����
// � %PATHEXT%
// IS: ��������� �� ���������� ��������� ����� � ���, ��� ������� � Dest
// IS: �� ��������, �.�. ����� ���� �������� ��� ���������� �����
char *Add_PATHEXT(char *Dest)
{
  char Buf[1024];
  int curpos=strlen(Dest)-1, l;
  UserDefinedList MaskList(0,0,ULF_UNIQUE);
  if(GetEnvironmentVariable("PATHEXT",Buf,sizeof(Buf)) && MaskList.Set(Buf))
  {
    /* $ 13.10.2002 IS �������� �� '|' (����� ����������) */
    if(*Dest && Dest[curpos]!=',' && Dest[curpos]!='|')
    {
      Dest[curpos+1]=',';
      Dest[curpos+2]=0;
      ++curpos;
    }
    /* IS $ */
    ++curpos;
    const char *Ptr;
    MaskList.Reset();
    while(NULL!=(Ptr=MaskList.GetNext()))
    {
      Dest[curpos]='*';
      ++curpos;
      l=strlen(Ptr);
      memcpy(Dest+curpos,Ptr,l);
      curpos+=l;
      Dest[curpos]=',';
      ++curpos;
      Dest[curpos]=0;
    }
    --curpos;
  }
  // ������ ������� - � ����!
  /* $ 13.10.2002 IS ����������� �� �������� */
  if(Dest[curpos] == ',')
    Dest[curpos]=0;
  /* IS $ */
  return Dest;
}
/* IS 13.10.2002 $ */

void CreatePath (char *Path)
{
  char *ChPtr = Path;
  char *DirPart = Path;

  BOOL bEnd = FALSE;

  while ( TRUE )
  {
    if ( (*ChPtr == 0) || (*ChPtr == '\\') )
    {
      if ( *ChPtr == 0 )
        bEnd = TRUE;

      *ChPtr = 0;

      if ( Opt.CreateUppercaseFolders && !IsCaseMixed(DirPart) && GetFileAttributes(Path) == (DWORD)-1)
        LocalStrupr(DirPart);

      if ( CreateDirectory(Path, NULL) )
        TreeList::AddTreeName(Path);

      if ( bEnd )
        break;

      *ChPtr = '\\';
      DirPart = ChPtr+1;
    }

    ChPtr++;
  }
}

void SetPreRedrawFunc(PREREDRAWFUNC Func)
{
  if((PreRedrawFunc=Func) == NULL)
    memset(&PreRedrawParam,0,sizeof(PreRedrawParam));
}

int PathMayBeAbsolute(const char *Path)
{
  return (Path &&
           (
//             (isalpha(*Path) && Path[1]==':' && Path[2]) ||
//           ���������� ���� �� ����������� ����� ":" �������� ��� ���-��.
//           ������ ������ � "�:"
             (isalpha(*Path) && Path[1]==':') ||
             (Path[0]=='\\'  && Path[1]=='\\') ||
             (Path[0]=='/'   && Path[1]=='/')
           )
         );
}

BOOL IsLocalPath(const char *Path)
{
  return (Path && isalpha(*Path) && Path[1]==':' && Path[2]);
}

BOOL IsLocalRootPath(const char *Path)
{
  return (Path && isalpha(*Path) && Path[1]==':' && Path[2] == '\\' && !Path[3]);
}

// ������������� �������������� ������ ����.
// CheckFullPath ������������ � FCTL_SET[ANOTHER]PANELDIR
char* PrepareDiskPath(char *Path,int MaxSize,BOOL CheckFullPath)
{
  if(Path)
  {
    if((isalpha(Path[0]) && Path[1]==':') || (Path[0]=='\\' && Path[1]=='\\'))
    {
      if(CheckFullPath)
      {
        char NPath[1024];
        *NPath=0;
        RawConvertShortNameToLongName(Path,NPath,sizeof(NPath));
        if(*NPath)
          xstrncpy(Path,NPath,MaxSize);
      }
      /* $ 03.12.2001 DJ
         RawConvertShortNameToLongName() �� ����������� ������ ����� Path
         => ������ else
      */
      if (Path[0]=='\\' && Path[1]=='\\')
      {
        char *ptr=&Path[2];
        while (*ptr && *ptr!='\\')
          *(ptr++)=toupper(*ptr);
      }
      else
        Path[0]=toupper(Path[0]);
      /* DJ $ */
    }
  }
  return Path;
}

/*
   �������� ���� ��� ����-����� �� �������������
   ���� ���� �������� ���� (IsHostFile=FALSE), �� �����
   ����������� ������� ����� ��������� ����. ��������� �������
   ������������ � ���������� TestPath.

   Return: 0 - ����.
           1 - ���!,
          -1 - ����� ��� ���, �� ProcessPluginEvent ������ TRUE
   TestPath ����� ���� ������, ����� ������ �������� ProcessPluginEvent()

*/
int CheckShortcutFolder(char *TestPath,int LengthPath,int IsHostFile, BOOL Silent)
{
  if(TestPath && *TestPath && GetFileAttributes(TestPath) == -1)
  {
    char Target[NM];
    int FoundPath=0;

    xstrncpy(Target, TestPath, sizeof(Target)-1);
    TruncPathStr(Target, ScrX-16);

    if(IsHostFile)
    {
      SetLastError(ERROR_FILE_NOT_FOUND);
      if(!Silent)
        Message (MSG_WARNING | MSG_ERRORTYPE, 1, MSG (MError), Target, MSG (MOk));
    }
    else // ������� �����!
    {
      SetLastError(ERROR_PATH_NOT_FOUND);
      if(Silent || Message (MSG_WARNING | MSG_ERRORTYPE, 2, MSG (MError), Target, MSG (MNeedNearPath), MSG(MHYes),MSG(MHNo)) == 0)
      {
        char *Ptr;
        char TestPathTemp[1024];
        xstrncpy(TestPathTemp,TestPath,sizeof(TestPathTemp)-1);
        while((Ptr=strrchr(TestPathTemp,'\\')) != NULL)
        {
          *Ptr=0;
          if(GetFileAttributes(TestPathTemp) != -1)
          {
            int ChkFld=CheckFolder(TestPathTemp);
            if(ChkFld > CHKFLD_NOTACCESS && ChkFld < CHKFLD_NOTFOUND)
            {
              if(!(TestPath[0] == '\\' && TestPath[1] == '\\' && TestPathTemp[1] == 0))
              {
                xstrncpy(TestPath,TestPathTemp,LengthPath);
                if(strlen(TestPath) == 2) // ��� ������ "C:", ����� ������� � ������� ������� ����� C:
                  AddEndSlash(TestPath);
                FoundPath=1;
              }
              break;
            }
          }
        }
      }
    }
    if(!FoundPath)
      return 0;
  }
  if(CtrlObject->Cp()->ActivePanel->ProcessPluginEvent(FE_CLOSE,NULL))
    return -1;
  return 1;
}

BOOL IsDiskInDrive(const char *Root)
{
  char   VolName[256];
  char   Drive[NM];
  DWORD  MaxComSize;
  DWORD  Flags;
  char   FS[256];

  strcpy(Drive,Root);
  AddEndSlash(Drive);
  UINT ErrMode = SetErrorMode ( SEM_FAILCRITICALERRORS );
  //���� �� ������� SetErrorMode - �������� ����������� ������ "Drive Not Ready"
  BOOL Res = GetVolumeInformation (Drive, VolName, sizeof(VolName), NULL, &MaxComSize, &Flags, FS, sizeof(FS));
  SetErrorMode(ErrMode);
  return Res;
}

void ShellUpdatePanels(Panel *SrcPanel,BOOL NeedSetUpADir)
{
  if(!SrcPanel)
    SrcPanel=CtrlObject->Cp()->ActivePanel;
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  int AnotherType=AnotherPanel->GetType();
  if (AnotherType!=QVIEW_PANEL)
  {
    if(NeedSetUpADir)
    {
      char CurDir[2048];
      SrcPanel->GetCurDir(CurDir);
      AnotherPanel->SetCurDir(CurDir,TRUE);
      AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    }
    else
    {
      if(AnotherPanel->NeedUpdatePanel(SrcPanel))
        AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
      else
      {
        // ������� ����� ���������� ������. ���� ��� ���� ����������� - ��������� ����.
        if (AnotherType==FILE_PANEL)
          ((FileList *)AnotherPanel)->ResetLastUpdateTime();
        AnotherPanel->UpdateIfChanged(UIC_UPDATE_NORMAL);
      }
    }
  }
  SrcPanel->Update(UPDATE_KEEP_SELECTION);
//  SrcPanel->Redraw();
  if (AnotherType==QVIEW_PANEL)
  {
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
//    AnotherPanel->Redraw();
  }
  CtrlObject->Cp()->Redraw();
}

int CheckUpdateAnotherPanel(Panel *SrcPanel,const char *SelName)
{
  if(!SrcPanel)
    SrcPanel=CtrlObject->Cp()->ActivePanel;
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  AnotherPanel->CloseFile();
  if(AnotherPanel->GetMode() == NORMAL_PANEL)
  {
    char AnotherCurDir[2048];
    char FullName[2058];

    AnotherPanel->GetCurDir(AnotherCurDir);
    AddEndSlash(AnotherCurDir);

    if (ConvertNameToFull(SelName,FullName, sizeof(FullName)) >= sizeof(FullName))
      return -1;
    AddEndSlash(FullName);

    if(strstr(AnotherCurDir,FullName))
    {
      ((FileList*)AnotherPanel)->CloseChangeNotification();
      return TRUE;
    }
  }
  return FALSE;
}

/* $ 26.10.2003 KM
   ����������� � ��������� ���������� ������
*/
/* $ 21.09.2003 KM
   ������������� ������ �� ��������� ����.
*/
void Transform(unsigned char *Buffer,int &BufLen,const char *ConvStr,char TransformType)
{
  int I,J,L,N;
  char *stop,HexNum[3];

  switch(TransformType)
  {
    case 'X': // Convert common string to hexadecimal string representation
    {
      *(char *)Buffer=0;
      L=strlen(ConvStr);
      N=min((BufLen-1)/2,L);
      for (I=0,J=0;I<N;I++,J+=2)
      {
        // "%02X" - ��� ��������� ������� �� ������ ���� ��������
        sprintf((char *)Buffer+J,"%02X",ConvStr[I]);
        BufLen=J+1;
      }

      RemoveTrailingSpaces((char *)Buffer);
      break;
    }
    case 'S': // Convert hexadecimal string representation to common string
    {
      *(char *)Buffer=0;

      L=strlen(ConvStr);
      char *NewStr=new char[L+1];
      if (NewStr==NULL)
        return;

      // ���������� ��������� ������
      memset(NewStr,0,L+1);

      // ��������� hex-������: ������� ������� ����� �������.
      for (I=0,J=0;ConvStr[I];++I)
      {
        if (ConvStr[I]==' ')
          continue;
        NewStr[J]=ConvStr[I];
        ++J;
      }

      L=strlen(NewStr);
      N=min(BufLen-1,L);
      for (I=0,J=0;I<N;I+=2,J++)
      {
        // "HH" - ��� �������� ������� �� ������ ���� ���������
        xstrncpy(HexNum,&NewStr[I],2);
        HexNum[2]=0;
        unsigned long value=strtoul(HexNum,&stop,16);
        Buffer[J]=value;
        BufLen=J+1;
      }
      Buffer[J]=0;

      delete []NewStr;
      break;
    }
    default:
      break;
  }
}
/* KM $ */
/* KM $ */

/*
 ���������� PipeFound
*/
int PartCmdLine(const char *CmdStr,char *NewCmdStr,int SizeNewCmdStr,char *NewCmdPar,int SizeNewCmdPar)
{
  int PipeFound = FALSE;
  int QuoteFound = FALSE;

  ExpandEnvironmentStr(CmdStr, NewCmdStr, SizeNewCmdStr);
  RemoveExternalSpaces(NewCmdStr);

  char *CmdPtr = NewCmdStr;
  char *ParPtr = NULL;

  // �������� ���������� ������� ��� ���������� � ���������.
  // ��� ���� ������ ��������� ������� �������� ��������������� �������
  // �������� � ������ �������. �.�. ���� � �������� - �� ����.

  while (*CmdPtr)
  {
    if (*CmdPtr == '"')
      QuoteFound = !QuoteFound;
    if (!QuoteFound)
    {
      if (*CmdPtr == '>' || *CmdPtr == '<' ||
          *CmdPtr == '|' || *CmdPtr == ' ' ||
          *CmdPtr == '/' ||      // ������� "far.exe/?"
          (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT && *CmdPtr == '&') // ��� ��/2� ���������� ����������� ������
         )
      {
        if (!ParPtr)
          ParPtr = CmdPtr;
        if (*CmdPtr != ' ' && *CmdPtr != '/')
          PipeFound = TRUE;
      }
    }

    if (ParPtr && PipeFound)
    // ��� ������ ������ �� ���� ��������
      break;
    CmdPtr++;
  }

  if (ParPtr) // �� ����� ��������� � �������� ��� �� ������
  {
    /* $ 05.04.2005 AY: ��� ����� ��� ������� ��������, ����� ��� ��� ���� */
    //if (*ParPtr=='/')
    //{
      //*NewCmdPar=0x20;
      //xstrncpy(NewCmdPar+1, ParPtr, SizeNewCmdPar-2);
    //}
    //else
    /* AY $ */

    if (*ParPtr == 0x20) //AY: ������ ������ ����� �������� � ����������� �� �����,
      *(ParPtr++)=0;     //    �� ����������� ������ � Execute.

    xstrncpy(NewCmdPar, ParPtr, SizeNewCmdPar-1);
    *ParPtr = 0;
  }

  Unquote(NewCmdStr);
  //RemoveExternalSpaces(NewCmdStr);
  //AY: ������ ��������� ��� ����� " .exe"
  //    � ��� � ����� ������� ��� ���� �� �����.

  //RemoveTrailingSpaces(NewCmdStr);
  //AY: ��� �������� ������ ��������������, �������� ���� "��� .exe" ����� ��������� ��� "��� ".

  return PipeFound;
}


BOOL ProcessOSAliases(char *Str,int SizeStr)
{
#if 0
  if(WinVer.dwPlatformId != VER_PLATFORM_WIN32_NT)
    return FALSE;

  typedef DWORD (WINAPI *PGETCONSOLEALIAS)(
    LPTSTR lpSource,
    LPTSTR lpTargetBuffer,
    DWORD TargetBufferLength,
    LPTSTR lpExeName
  );

  static PGETCONSOLEALIAS GetConsoleAlias=NULL;
  if(!GetConsoleAlias)
  {
    GetConsoleAlias = (PGETCONSOLEALIAS)GetProcAddress(GetModuleHandle("kernel32"),"GetConsoleAliasA");
    if(!GetConsoleAlias)
      return FALSE;
  }

  char NewCmdStr[4096];
  char NewCmdPar[2048];

  PartCmdLine(Str,NewCmdStr,sizeof(NewCmdStr),NewCmdPar,sizeof(NewCmdPar));

  char ModuleName[NM];
  GetModuleFileName(NULL,ModuleName,sizeof(ModuleName));
//  if(GetConsoleAlias(NewCmdStr,NewCmdStr,sizeof(NewCmdStr),ModuleName) > 0)
  int b=GetConsoleAlias(NewCmdStr,NewCmdStr,sizeof(NewCmdStr),"cmd.exe");
  if(b > 0)
  {
    strncat(NewCmdStr,NewCmdPar,sizeof(NewCmdStr)-1);
    xstrncpy(Str,NewCmdStr,SizeStr-1);
    return TRUE;
  }
#endif
  return FALSE;
}

/*
  ��������, ��� ��� [A|P]Panel.UNCPath
  _MakePath1(CheckCode == MCODE_V_APANEL_UNCPATH?KEY_ALTSHIFTBRACKET:KEY_ALTSHIFTBACKBRACKET,FileName,sizeof(FileName)-1,"")
*/
int _MakePath1(DWORD Key,char *PathName,int PathNameSize, const char *Param2)
{
  int RetCode=FALSE;
  int NeedRealName=FALSE;
  *PathName=0;
  switch(Key)
  {
    case KEY_CTRLALTBRACKET:       // �������� ������� (UNC) ���� �� ����� ������
    case KEY_CTRLALTBACKBRACKET:   // �������� ������� (UNC) ���� �� ������ ������
    case KEY_ALTSHIFTBRACKET:      // �������� ������� (UNC) ���� �� �������� ������
    case KEY_ALTSHIFTBACKBRACKET:  // �������� ������� (UNC) ���� �� ��������� ������
      NeedRealName=TRUE;
    case KEY_CTRLBRACKET:          // �������� ���� �� ����� ������
    case KEY_CTRLBACKBRACKET:      // �������� ���� �� ������ ������
    case KEY_CTRLSHIFTBRACKET:     // �������� ���� �� �������� ������
    case KEY_CTRLSHIFTBACKBRACKET: // �������� ���� �� ��������� ������

    case KEY_CTRLSHIFTENTER:       // ������� ���� � ����.������
    case KEY_SHIFTENTER:           // ������� ���� � �����.������
    {
      Panel *SrcPanel=NULL;
      FilePanels *Cp=CtrlObject->Cp();
      switch(Key)
      {
        case KEY_CTRLALTBRACKET:
        case KEY_CTRLBRACKET:
          SrcPanel=Cp->LeftPanel;
          break;
        case KEY_CTRLALTBACKBRACKET:
        case KEY_CTRLBACKBRACKET:
          SrcPanel=Cp->RightPanel;
          break;
        case KEY_SHIFTENTER:
        case KEY_ALTSHIFTBRACKET:
        case KEY_CTRLSHIFTBRACKET:
          SrcPanel=Cp->ActivePanel;
          break;
        case KEY_CTRLSHIFTENTER:
        case KEY_ALTSHIFTBACKBRACKET:
        case KEY_CTRLSHIFTBACKBRACKET:
          SrcPanel=Cp->GetAnotherPanel(Cp->ActivePanel);
          break;
      }

      if (SrcPanel!=NULL)
      {
        if(Key == KEY_SHIFTENTER || Key == KEY_CTRLSHIFTENTER)
        {
          char ShortFileName[NM];
          SrcPanel->GetCurName(PathName,ShortFileName);
          if(SrcPanel->GetShowShortNamesMode()) // ����� ���������� ���� :-)
            strcpy(PathName,ShortFileName);
        }
        else
        {
          /* TODO: ����� ����� ������, ��� � TreeList ���� ���� ���� :-) */
          if (!(SrcPanel->GetType()==FILE_PANEL || SrcPanel->GetType()==TREE_PANEL))
            return(FALSE);

          SrcPanel->GetCurDir(PathName);
          if (SrcPanel->GetMode()!=PLUGIN_PANEL)
          {
            FileList *SrcFilePanel=(FileList *)SrcPanel;
            SrcFilePanel->GetCurDir(PathName);
            if(NeedRealName)
              SrcFilePanel->CreateFullPathName(PathName,PathName,FA_DIREC,PathName,PathNameSize,TRUE);
            if (SrcFilePanel->GetShowShortNamesMode())
              ConvertNameToShort(PathName,PathName,PathNameSize-1);
          }
          else
          {
            FileList *SrcFilePanel=(FileList *)SrcPanel;
            struct OpenPluginInfo Info;
            CtrlObject->Plugins.GetOpenPluginInfo(SrcFilePanel->GetPluginHandle(),&Info);
            FileList::AddPluginPrefix(SrcFilePanel,PathName);
            strncat(PathName,NullToEmpty(Info.CurDir),PathNameSize);

          }
          AddEndSlash(PathName);
        }

        if(Opt.QuotedName&QUOTEDNAME_INSERT)
          QuoteSpace(PathName);
        strncat(PathName,NullToEmpty(Param2),PathNameSize);

        RetCode=TRUE;
      }
    }
    break;
  }
  return RetCode;
}

const char *CurPath2ComputerName(const char *CurDir, char *ComputerName,int SizeName)
{
  char NetDir[NM*3];

  *ComputerName=0;
  *NetDir=0;

  if (CurDir[0]=='\\' && CurDir[1]=='\\')
    strcpy(NetDir,CurDir);
  else
  {
    char LocalName[3];
    DWORD RemoteNameSize=sizeof(NetDir);
    xstrncpy(LocalName,CurDir,2);
    LocalName[sizeof(LocalName)-1]=0;

    SetFileApisTo(APIS2ANSI);
    if (WNetGetConnection(LocalName,NetDir,&RemoteNameSize)==NO_ERROR)
      FAR_CharToOem(NetDir,NetDir);
    SetFileApisTo(APIS2OEM);
  }

  if (NetDir[0]=='\\' && NetDir[1]=='\\')
  {
    strncpy(ComputerName,NetDir+2,SizeName);
    char *EndSlash=strchr(ComputerName,'\\');
    if (EndSlash==NULL)
      *ComputerName=0;
    else
      *EndSlash=0;
  }

  return ComputerName;
}

int CheckDisksProps(const char *SrcPath,const char *DestPath,int CheckedType)
{
  char SrcRoot[NM],DestRoot[NM];
  int SrcDriveType, DestDriveType;

  DWORD SrcVolumeNumber=0, DestVolumeNumber=0;
  char SrcVolumeName[NM], DestVolumeName[NM];
  char  SrcFileSystemName[NM], DestFileSystemName[NM];
  DWORD SrcFileSystemFlags, DestFileSystemFlags;
  DWORD SrcMaximumComponentLength, DestMaximumComponentLength;


  GetPathRoot(SrcPath,SrcRoot);
  GetPathRoot(DestPath,DestRoot);

  SrcDriveType=FAR_GetDriveType(SrcRoot,NULL,TRUE);
  DestDriveType=FAR_GetDriveType(DestRoot,NULL,TRUE);

  if (!GetVolumeInformation(SrcRoot,SrcVolumeName,sizeof(SrcVolumeName),&SrcVolumeNumber,&SrcMaximumComponentLength,&SrcFileSystemFlags,SrcFileSystemName,sizeof(SrcFileSystemName)))
    return(FALSE);

  if (!GetVolumeInformation(DestRoot,DestVolumeName,sizeof(DestVolumeName),&DestVolumeNumber,&DestMaximumComponentLength,&DestFileSystemFlags,DestFileSystemName,sizeof(DestFileSystemName)))
    return(FALSE);

  if(CheckedType == CHECKEDPROPS_ISSAMEDISK)
  {
    if (strpbrk(DestPath,"\\:")==NULL)
      return TRUE;

    if ((SrcRoot[0]=='\\' && SrcRoot[1]=='\\' || DestRoot[0]=='\\' && DestRoot[1]=='\\') &&
        LocalStricmp(SrcRoot,DestRoot)!=0)
      return FALSE;

    if (*SrcPath==0 || *DestPath==0 || (SrcPath[1]!=':' && DestPath[1]!=':')) //????
       return TRUE;

    if (toupper(DestRoot[0])==toupper(SrcRoot[0]))
      return TRUE;

    int64 SrcTotalSize,SrcTotalFree,SrcUserFree;
    int64 DestTotalSize,DestTotalFree,DestUserFree;

    if (!GetDiskSize(SrcRoot,&SrcTotalSize,&SrcTotalFree,&SrcUserFree))
      return FALSE;
    if (!GetDiskSize(DestRoot,&DestTotalSize,&DestTotalFree,&DestUserFree))
      return FALSE;

    if (!(SrcVolumeNumber!=0 &&
        SrcVolumeNumber==DestVolumeNumber &&
        strcmp(SrcVolumeName,DestVolumeName)==0 &&
        SrcTotalSize==DestTotalSize))
      return FALSE;
  }

  else if(CheckedType == CHECKEDPROPS_ISDST_ENCRYPTION)
  {
    if(!(DestFileSystemFlags&FILE_SUPPORTS_ENCRYPTION))
      return FALSE;
    if(!(DestDriveType==DRIVE_REMOVABLE || DestDriveType==DRIVE_FIXED || DestDriveType==DRIVE_REMOTE))
      return FALSE;
  }

  return TRUE;
}
