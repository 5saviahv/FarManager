/*
copy.cpp

����������� ������

*/

/* Revision: 1.135 08.06.2004 $ */

/*
Modify:
  08.06.2004 SVS
    ! ������ GetDriveType ������ �������� FAR_GetDriveType().
    ! ������ "DriveType==DRIVE_CDROM" �������� IsDriveTypeCDROM()
  19.05.2004 SVS
    ! ������ "SetFileAttributes(Name,0)" �������� "SetFileAttributes(Name,FILE_ATTRIBUTE_NORMAL)"
      ����� ���������� �� �����.
  21.04.2004 SVS
    - BugZ#1059 - �������� ���� �������������� ������ ���� ��������
  16.04.2004 SVS
    - BugZ#1049 - ��������� ��������� ����������� ����������� ����������������
    ! ��� ���� ������� 4 ����� (� ����� ����� �� ����������)
  01.03.2004 SVS
    ! ������� FAR_OemTo* � FAR_CharTo* ������ ����������� WinAPI-�������
      (����� �� ������� + ������� ������������ ������ 1 ����)
  22.10.2003 SVS
    - BugZ#976 - ����������� (F6) ����� �� ����� � ".."
  09.10.2003 SVS
    ! SetFileApisToANSI() � SetFileApisToOEM() �������� �� SetFileApisTo() � ����������
      APIS2ANSI ��� APIS2OEM - ����� �� �������
  22.09.2003 SVS
    + CheckUpdatePanel(); ���������� ���� FCOPY_UPDATEPPANEL
    + ���� ������� ��������� �������� - ������ ������� _�����_, � �.�.
      ���� ���� Move � ��������� �����, �� � � ��������� ������ ����� Copy ��� ������
      ����� �, ��������������, ���������� ����� ���������...
  15.09.2003 SVS
    ! �������� target �� ������������ �������, ������������� � ReservedFilenameSymbols
    ! �������� ����������� �������� ������� ��������� ������.
      ������ ����, �� �������� �������. ����� ����� ������ :-(
  12.09.2003 SVS
    - BugZ#947 - ������ � ��������������� �� Shift-F6...
      �������� "��������" ������� ��������� ������
  04.09.2003 SVS
    ! ������ ������ CompareFileTime() �������� ���� � ������������� ������:
      �������� FILETIME � __int64
  19.07.2003 IS
    + ���� ���� �������� �����������, �� ������� �� � �������, ���� �� ��������
      ������ ��� F5, Enter � �������, ����� ������������ ������� MultiCopy
  15.07.2003 VVM
    + ��� ������������� SystemCopyRoutine �������� ���-�� �������� �� ���� 5 ��� � �������.
  11.07.2003 SVS
    - ���� � ����� (multitarget) ����������� con ��� nul, �� ����� �����.
    + CheckNulOrCon() - �������� ����� ���� ����
    - BugZ#641 - rename directory
  15.06.2003 SVS
    - ����� "[ ] ���������� ���������� ������������� ������" �...
      ��������� ������ + ����� �� ������������. ������ � ShellCopy::CalcTotalSize()
  14.06.2003 SVS
    - ���������� ��������� ��� ��������������, ��� ����� �� ���������
      ������ �������� �������� ����, ��� ������� �� ��������.
    + ��������� ����� "[ ] ���������� ���������� ������������� ������"
    ! ��������������, �������� ������ ������ ������ (��������� � ����� @CopyRule)
    ! ������������� ����������� ��������� �������� � ������ �� ���� -
      ��� ����� � ����� ������� �� ������� (�� ���, ���� ����)
  04.06.2003 SVS
    - �� ������������ �������� ��� ����������� ��������.
  08.05.2003 VVM
    ! ��� Append ������ ��� ������� ����� :) ���������� �������� lpDistanceToMoveHigh
      � ������� SetFilePointer.
  30.04.2003 VVM
    + ������ ���� FILE_FLAG_SEQUENTIAL_SCAN.
      � ���� ����������� ���������� ��������� �� 20%
  30.04.2003 VVM
    + ��� ����������� ��������� ���� ���-�� ������ 4 ���� � �������
  25.04.2003 VVM
    ! ������� ������������ �����. ������� � ������� ������� - ������� � �����
  21.04.2003 SVS
    ! � ����������� DN_KEY ������� "break", � �� "return". ������ ��
      ���������, �� ���� ����������!
  16.04.2003 VVM
    ! ���������� �������� 4 ���� � �������, ������ ������.
  08.03.2003 SVS
    ! ��������� ��������� ������ ������� (�������� �� ���... ����� ������
      ������� ��������� - �������������� ��� � ������ �� �������� ������
      �������� �����������)
  05.03.2003 SVS
    + _LOGCOPYR() - ��������� ��� �������� �����������
    - BugZ#636 - ������������ ������ ������ ����� �����������
    - BugZ#694 - �� �������� ����������� �� ������-������� �� ����.
      (������� ������ �������������� ��� ��������� ������� ��������
      �������� �� dest-�������)
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
  26.01.2003 IS
    ! FAR_DeleteFile ������ DeleteFile, FAR_RemoveDirectory ������
      RemoveDirectory, ������� � ������ �� ������������ ��� ��������
      �������������� ������ � ���������.
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  16.01.2003 SVS
    - ������� (Ins) ��������� ���������, ALT-F6 Enter - ��������� � ����� �� �������.
  30.12.2002 VVM
    - ��� ����������� � ��������� ��������� ����� ������� ����������� �������� �������������.
      ��������� ������������� ������ ����� :)
  21.12.2002 IS
    ! ������� ����������� � nul �������� � �������, ����� ���� ����������:
       - ���������� � "nul\", "\\.\nul\" ��� "con\"
       - ����� "nul", "\\.\nul" ��� "con"
  17.12.2002 SVS
    - BugZ#728 - ������������ �� ����� ������������ ���������� ��� �����������
      (��������� �����������)
  16.12.2002 SVS
    - BugZ#728 - ������������ �� ����� ������������ ���������� ��� �����������
  13.12.2002 VVM
    - �� ��������� ������� ����������� � ��������� ����.
  11.12.2002 SVS
    - BugZ#702 - �� �������������� ��������� ������� ������� �� ����� �����������
    - BugZ#686 - �������� � ���������� ���� ��������� �����������
  11.12.2002 VVM
    - ��������� ��� � ����������� ��������� ��� ����������� ����� ������
    - ��������� ��� � ��������� �������/�������� ����������� ��� ������� "�����������" ������
  09.11.2002 SVS
    - ������ � ������������ ���� ��� XP. ������ *Apis*
  14.10.2002 SVS
    - � ����� ���� �������� ��� ����� � ��������, ���� F6
      � ������ (��� ������� ��������!) ������� ���� ������ ��
      ��������, ������ ������ ��� ���������� ������.
      ������������ � ������ ��� � ���� Enter.
      ����� ���������� �� ����� ������� ������� �� ������ ����,
      �� �� ������ �����... ������ � ������������� (������
      ����� - �����).
  19.09.2002 VVM
    ! �������� �������� ��� ��������� ������� �����������
  18.09.2002 VVM
    ! ������������� ��������� �������� ������� �����������.
  14.09.2002 VVM
    + �������� ����������� ���������� �� ���� 1 ���� � �������.
    + �� ��������� ����� ����������� 64�. ����. ������ �� ��������� 512 �.
  15.08.2002 IS
    ! DestList.Start -> DestList.Reset
    + DestList - ����������� ULF_UNIQUE, ��� ���������� ������
  12.07.2002 SVS
    - ��������� �� ����������� �� ��������� ������� NTFS-������
      (���� ��� ��������!)
      ���� ���� ����. ��������� � �������. ���������. ��� - ���� �� ����.
      �����: �������� ��������, �� ������ ��������� � ������ (��� ����!)
      ������ � ShellCopy::LinkRules()
  02.07.2002 SVS
    - BugZ#522 - Refreshing panels after renaming directory (changing its register)
  18.06.2002 SVS
    ! ������� IsFolderNotEmpty ������������� � CheckFolder
  18.06.2002 VVM
    ! ShellSetAttr: ���� ���� �� ������ �������� ���������� � ���� - ���������� ��������� ��������
      � ���� �� ����������� ���� ��� UNC-���� ������-�� ������������ GetVolumeInformation()
  15.06.2002 IS
    - ���: ����������� �� �������������, ���� �� ���� ������� �������������
  10.06.2002 SVS
    + DIF_EDITPATH (FIB_EDITPATH)
    - BugZ#556 - ��� �������� ������ �� NWFS
  30.05.2002 SVS
    - ������ ��� ������������ ����� ��� �������� �����
  28.05.2002 SVS
    - BugZ#534 - ����������� ������ � ������� ���
  25.05.2002 IS
    + CmpFullNames: ������ �������� � ��������� _��������_ �������, �
      ���������� ���� ������������� ��������, �����
      Src="D:\Program Files\filename"
      Dest="D:\PROGRA~1\filename"
      ("D:\PROGRA~1" - �������� ��� ��� "D:\Program Files")
      ���������, ��� ����� ���� ����������, � ������ ���������,
      ��� ��� ������ (������� �� �����, ��� � � ������, � �� ������ ������
      ���� ���� � ��� ��)
    ! CmpFullNames: "���������" ������� �� DeleteEndSlash
    ! CmpFullNames: ������� ��� ���������������� �� �������� ���� � ������
      ��������� �� ������� �����, ������ ��� ��� ����� ������ ������ ���
      ��������������, � ������� ���������� � ��� ����������� ����. ��� ���
      ������ �������������� �� � ���, � ��� ��, ��� � RenameToShortName.
      ������ ������� ������ 1, ��� ������ ���� src=path\filename,
      dest=path\filename (������ ���������� 2 - �.�. ������ �� ������).
    + MoveFileThroughTemp - ������� �������������� �����, ������� ���������
      ���� ��� ������, ����� ��������������� ������ � ���� (�� ������
      ������������� SVS ������� - ������� ������ MoveFile).
    ! ������ FAR ����� ��������������� � �����, � _��������_ � ��������
      ��� (��� ����� ������������ MoveFileThroughTemp).
    ! ��������� const � ������
    - ShellCopyConvertWildcards ������ ��, ��� ������ ���� ������
  24.05.2002 SVS
    ! ���...
  24.05.2002 SVS
    + BugZ#521 - ������� ������������� abcdefghi.txt � abcdef~1.txt
  18.05.2002 SVS
    ! � �������� ����������� (F6) � "nul" ��� "con" �������� ������ �� ������������.
  15.05.2002 SVS
    ! �������� ����� �� ������� "������������� � ������� ��������"
  06.05.2002 VVM
    - ��� ������������� ���������� ����������� ������������� ����� ����������� :)
  26.04.2002 SVS
    - BugZ#484 - Addons\Macros\Space.reg (��� ��������� �������)
  24.04.2002 VVM
    - �� ����������� ���� �� �������� ����������� �����.
      ������: ���� �� ������ ����������� ����� - ���������� ������ �������
              ������ �� ����� �����.
  18.04.2002 SVS
    - ������ ��� �������� �������� �� ������ ��� ����� (C:\, ��������)
      � ���� ������ ����������� �������� ������������ ����� �, ���� ������
      �� ����� - ����������� ���. ��� "Disk_%c"
  08.04.2002 SVS
    - BugZ#440 - ���������������� ������ RO-�������� ��� Append
  05.04.2002 SVS
    - BugZ#430 - �������� ����������
    - ��������� ���� � ���������� (���������� ������ ������� �����������
      ��� ������, ����� ���� RO-���� � ������� ������)
  01.04.2002 SVS
    - BugZ#376 - ��� ������� �������� �������� � �������� ������ ��������
      ����� �������� ��������� �� ������
  28.03.2002 SVS
    ! ���� �� ����� ������� ���������� �������, �� F6 ���������� Shift-F6,
      �.�. � ������ ����� ������������ ��� �����.
    ! ������� const
  28.01.2002 VVM
    ! ���� �� ����� ���������, ���� Dest �������� ������...
  26.03.2002 DJ
    ! ScanTree::GetNextName() ��������� ������ ������ ��� ����� �����
  23.03.2002 VVM
    ! ������ ����� �� ������ DJ. bug#390
  23.03.2002 IS
    + ��� ����������� ������ �������� ��� �������� ����� ��� ��� � �������,
      ���� ��� �������� �����������.
  22.03.2002 SVS
    - strcpy - Fuck!
  19.03.2002 VVM
    - ������ ������������� ����� �����������. ����� ���������� ���
      �������� ����� � ������ �����������.
  18.03.2002 SVS
    - � NT4 �� ����������� ������� ����� ��� ���������
      (������ ��� ������ � ���������)
    - ������ ����������� ���� � ���������� � ������ ������
      (�� ����������� ���������)
  02.03.2002 KM
    ! �������: ��� ������� ������ ����������� (����� ���� �� ����)
      ��� ��������� ���������� ������ (� ��� ����������� ����������
      �� ��������� �����) ����������� "Retry Skip Cancel", ��� ���
      ����� �� ����� ����� ����������� ���������� ����� ���������
      ���� Skip ���������, ������� ��������� Skip all.
  02.03.2002 SVS
    ! ����������� � "con" ���������� ����������� � "nul".
  11.02.2002 SVS
    ! ������ ����� "Copy access rights" ��� ����������� �� ���������� ������.
      ����... ��� ����� �������� ��� REALFILES
    + �������� SetErrorMode() ��� ����, ����� ������ ����� �� ����� �� ������
      ������� �������� (����, ����, ��� ����� �����, �� ����� :-))
  16.01.2002 SVS
    - �������� � FILE_ATTRIBUTE_ENCRYPTED
  14.01.2002 IS
    ! chdir -> FarChDir
  28.12.2001 SVS
    ! ������ � ������ ��������� �������� (��� ��������� union)
  28.12.2001 SVS
    ! ������� ��������� ����������������� ����� "[ ] Only never..."
  19.12.2001 VVM
    ! �� ��������� �������� ��� �������� ������
  11.12.2001 SVS
    - BugZ#171 - ������ � [x] Process multiple destinations
  06.12.2001 VVM
    + ����������� ��������� ������ ��� ����� FCOPY_COPYLASTTIME
  16.11.2001 SVS
    - �������� � ShellCopy::GetSecurity "sa->nLength=sizeof(sa);"
                                                            ^^ ��� ���������!
  01.11.2001 SVS
    - F5, F10, Esc - ��� ���, � ���: F5, ������ ����� �� ����� "F10 - Tree",
      Esc - ��� ����� ���� :-(
      ������� - ������ ������ ������ ��������� ������.
  29.10.2001 SVS
    ! �������� ������� �������� �� ������, �.�. �� ������ ��������
  24.10.2001 SVS
    + �������� �������������� �������� � FolderTree - "��� �� ������!"
  22.10.2001 SVS
    - �������� � ����������� ����� ��������� CALLBACK-������� (����� 1 ������
      �������� - �������� ����� ����������)
  21.10.2001 SVS
    + CALLBACK-������� ��� ���������� �� BugZ#85
  17.10.2001 SVS
    ! ��������� const
  16.10.2001 SVS
    + �������� ���������� ��������� �� ������ - ��� ������ �����:
      1) �� �������� ��������� ������� �����������
      2) ��� NT
      3) ���� ���������� �� NTFS
  15.10.2001 SVS
    - strncpy - ������ �������� :-)
      "���������� �� �������������� ������� ����� ��������-������������"
       (�� ����� �����������)
  13.10.2001 IS
    + ��� ����������������� ��������� ��������� � "������" ������� � ���
      ������������� ������ ����� ����� � �������.
    - ���: ��� ����������������� ��������� � "������" ������� �� ����������
      � �������, ���� �� �������� � ����� ����� �������-�����������.
    - ���: ����������� �������� Shift-F10, ���� ������ ����� ���������
      ���� �� �����.
    - ���: ����������� �������� Shift-F10 ��� ����������������� - �����������
      �������� �������, ������ ������������ ����� ������ ������� � ������.
  18.09.2001 SVS
    + ��������� "������" ��� ������ ����� ���� ����������.
  12.09.2001 SVS
    - BugZ#15: ����������� ����������� Junction �������� ������ �� ����.
      � ������� ShellCopy::CmpFullNames() ������ ConvertNameToFull()
      ���������� ����� ������� ConvertNameToReal(), ������� ����������
      ������ �������� ���� � ������ reparse point � Win2K.
  15.08.2001 SVS
    - ���� - Win2K. �������� ����������. ��������� �������� (�� ���������)
      - ����� ����� ������� ���� � ������ Link.
      ���� � ����� - ����� ������� SameDisk ��� ����� ���������.
  03.08.2001 IS
    + ����� ����� ��� ����������������� � ������� � ��������������� ��
      ���������.
    ! ������ � ���������� ������ CopyDlg[2].Data -> CopyDlgValue
    ! ����� "#ifndef COPY_NOMULTICOPY", �.�. ������ ��������������
      �������������� �� ����������� ������.
  22.07.2001 SVS
    ! ����������� �� ���������
  18.07.2001 VVM
    + ���� �� ������� ������������� ������� - ������� �� ���� ������������
      � �������� - "��� ������?"
  27.06.2001 SVS
    - �������� ������ ����������.
  19.06.2001 SVS
    ! ����� ����������� �������� ������� ��� ����������� target-���������
  07.06.2001 IS
    - ��� (� ��������� �����): ����� ������� ������� �������, � ������ �����
      �������
  06.06.2001 tran
    *  ������������� NULL ������ ���� �� ����� Return �� ������������...
       ������ �� ".."
  02.06.2001 IS
    ! #define COPY* -> enum
    + ��������� (�������) ��������� ������ ����� � ������ �������
    + �������� ������ ����� �� ������������
    ! ��������� ����������� ��-�� ����������������� �������� ��� ������ ����
      ��������
  01.06.2001 SVS
    ! � FAT&CDFS -> NTFS ����� ������ symlink! �� ��� �������, ���
      CD ���������� �� NTFS!!!
  30.05.2001 SVS
    ! ShellCopy::CreatePath �������� �� ������ � �������� �������
    ! ��������� �������������� ����������� ������
    ! ��������� ����� - ��� ��������� ��� ����������� ���������� ��������
    + ����������.
  26.05.2001 OT
    - ����������� ����������� NFZ-���� - �� �������� � �������� �����������, ��������...
  18.05.2001 DJ
    + #include "colors.hpp"
  17.05.2001 SKV
    - fix GetTimeText �� ������� �����.
  07.05.2001 SVS
    ! ��� Move �� _��� ��_ ����� ��������� Total
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  24.04.2001 VVM
    ! ��� �������� ������� ����������� ������������ ����� ������� (� ��������)
  08.04.2001 SVS
    ! �������� ������ ������ ��� NT - � ��� ������ � �����������.
    + ���������� ��� ��� �������� repase point. ������� � ��� ���� ��
      ���������� - ���� ������, �.�. ����� ������ ��� copy.cpp ��������������.
      ���� ��� ������ ��������!
  05.04.2001 IS
    + ���� ��� �����������/����������� ���������� ��������� ������ 1 � �����
      ��� ���� �������, �� ������ ������ ���, ����� �� ����� ��� '\\', �����
      �� ���� ��� ������� � ���� :(
  14.03.2001 SVS
    + �������������� ����� ���� ��� �������� SymLink ��� ���������.
  12.03.2001 SVS
    ! ��������� � ����� � ����������� � ������ int64
  06.03.2001 SVS
    ! ������� ����������� � "���������� ��������" + ������� ��� 'Gb'
  27.02.2001 VVM
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
  12.02.2001 VVM
    - ��� ���������� TotalCopyIndicator � ������ CPS ������ ������������ 1 ��� :)
  06.02.2001 SKV
    - char SelNameShort[NM];, � �� char SelNameShort[40];
  30.01.2001 VVM
    + ���������� ����� �����������,���������� ����� � ������� ��������.
      ������� �� �������� � ������� CopyTimeRule
  25.01.2001 IS
    - �� ����� ��������� ���������� ��� � ������� �����������/�����������
  17.01.2001 SVS
    ! �� ��� NT - ���������� ����� ��� ����������� �����.
  01.01.2001 VVM
    + ������ ������ ��� ����������� ������� �� �������.
  30.12.2000 SVS
    - ��� �����������/�������� ������ ��������� FILE_ATTRIBUTE_ENCRYPTED
      ��� ��������, ���� �� ����
    ! ����� "[ ] ������ �����/����������� �����" ������ �� "��������", �
      ������ �����������
  14.12.2000 SVS
    + Copy to "NUL"
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  23.10.2000 VVM
    + ������������ ����� �����������
  21.10.2000 SVS
    + ����������������� ����� ���������� Copy_Buffer_Size � �����������
      �������� = 64�
  11.10.2000 SVS
    ! ���� Dest-������ ����������, �� �� ���������� "[ ] Only never..."
  14.08.2000 SVS
    ! ������, ��������� �� 40 ��������... :-(
      � ����� ������������ (CopyDlg[2].Data) �� ������ ���������...
  09.08.2000 KM
    ! ��������� �������� �� ����� ����������� �������
      � ShellCopy::ShellCopy, ����� �������� � ���� ������
      ����������� ������ ������ ���������.
  04.08.2000 SVS
    + ����� "Only newer file(s)"
  01.08.2000 tran 1.05
    + DIF_USELASTHISTORY � ��������
  31.07.2000 SVS
    + ���������� ���������� ����� ��� ������ ����� � ������� Copy/Move!
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  03.07.2000 IS
    ! ���������� �������� ������� ��� �����������/��������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/


#include "headers.hpp"
#pragma hdrstop

#include "copy.hpp"
#include "global.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "colors.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "dialog.hpp"
#include "ctrlobj.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "filelist.hpp"
#include "foldtree.hpp"
#include "treelist.hpp"
#include "chgprior.hpp"
#include "scantree.hpp"
#include "savescr.hpp"
#include "manager.hpp"
#include "constitle.hpp"
#include "lockscrn.hpp"

/* �������� ��� ���������� ��������-����. */
#define COPY_TIMEOUT 200

enum {COPY_BUFFER_SIZE  = 0x10000};

/* $ 30.01.2001 VVM
   + ��������� ��� ������ ������
   + ������� ��������� */
enum {
  COPY_RULE_NUL    = 0x0001,
  COPY_RULE_FILES  = 0x0002,
};

static int ShowCopyTime;
static clock_t CopyStartTime;
static clock_t CopyTime;
static clock_t LastShowTime;
/* VVM $ */
static int OrigScrX,OrigScrY;

static DWORD WINAPI CopyProgressRoutine(LARGE_INTEGER TotalFileSize,
       LARGE_INTEGER TotalBytesTransferred,LARGE_INTEGER StreamSize,
       LARGE_INTEGER StreamBytesTransferred,DWORD dwStreamNumber,
       DWORD dwCallbackReason,HANDLE hSourceFile,HANDLE hDestinationFile,
       LPVOID lpData);

static int BarX,BarY,BarLength;

static int64 TotalCopySize, TotalCopiedSize; // ����� ��������� �����������
static int64 CurCopiedSize;                  // ������� ��������� �����������
static int64 TotalSkippedSize;               // ����� ������ ����������� ������
static int64 TotalCopiedSizeEx;
static int   CountTarget;                    // ����� �����.

static bool ShowTotalCopySize;
static int StaticMove;
static char TotalCopySizeText[32];
static ConsoleTitle *StaticCopyTitle=NULL;
static BOOL NT5, NT;

struct CopyDlgParam {
  ShellCopy *thisClass;
  int AltF10;
  int FileAttr;
  int SelCount;
  int FolderPresent;
  int FilesPresent;
  int OnlyNewerFiles;
  int CopySecurity;
  char FSysNTFS;
  char PluginFormat[32]; // � ����� ����� ����������.
  DWORD FileSystemFlagsSrc;
  int IsDTSrcFixed;
  int IsDTDstFixed;
};


ShellCopy::ShellCopy(Panel *SrcPanel,        // �������� ������ (��������)
                     int Move,               // =1 - �������� Move
                     int Link,               // =1 - Sym/Hard Link
                     int CurrentOnly,        // =1 - ������ ������� ����, ��� ��������
                     int Ask,                // =1 - �������� ������?
                     int &ToPlugin,          // =?
                     char *PluginDestPath)   // =?
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::ShellCopy()"));
  /* $ 15.08.2002 IS
     �������� ����� � ������ �����
  */
  DestList.SetParameters(0,0,ULF_UNIQUE);
  /* IS $ */
  struct CopyDlgParam CDP;
  /* $ 03.08.2001 IS
       CopyDlgValue - � ���� ���������� ������ �������� ������� �� �������,
       ������ ��� ���������� �������� ����������, � CopyDlg[2].Data �� �������.
  */
  char CopyStr[100],SelName[NM],CopyDlgValue[NM];
  char DestDir[NM],InitDestDir[NM],SrcDir[NM];
  /* IS $ */
  char SelNameShort[NM];
  int DestPlugin;
  int AddSlash=FALSE;

  // ***********************************************************************
  // *** ��������������� ��������
  // ***********************************************************************
  // ����� ��������� ������ OS

  NT=WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT;
  NT5=NT && WinVer.dwMajorVersion >= 5;

  /* $ 06.06.2001 tran
     ������������� NULL ������ ���� �� ����� Return �� ������������... */
  CopyBuffer=NULL;

  if(Link && !NT) // �������� ������ ������ ��� NT
  {
    Message(MSG_DOWN|MSG_WARNING,1,MSG(MWarning),
              MSG(MCopyNotSupportLink1),
              MSG(MCopyNotSupportLink2),
              MSG(MOk));
    _LOGCOPYR(SysLog("return -> %d NotSupportLink!!!",__LINE__));
    return;
  }

  memset(&CDP,0,sizeof(CDP));
  CDP.IsDTSrcFixed=-1;
  CDP.IsDTDstFixed=-1;

  if ((CDP.SelCount=SrcPanel->GetSelCount())==0)
  {
    _LOGCOPYR(SysLog("return -> %d SelCount == 0",__LINE__));
    return;
  }

  *SelName=*RenamedName=*CopiedName=0;

  if (CDP.SelCount==1)
  {
    SrcPanel->GetSelName(NULL,CDP.FileAttr);
    SrcPanel->GetSelName(SelName,CDP.FileAttr);
    if (TestParentFolderName(SelName))
    {
      _LOGCOPYR(SysLog("return -> %d TestParentFolderName('%s') != 0",__LINE__,SelName));
      return;
    }
  }

  /* $ 26.05.2001 OT ��������� ����������� ������� �� ����� ����������� */
  _tran(SysLog("call (*FrameManager)[0]->LockRefresh()"));
  (*FrameManager)[0]->LockRefresh();
  /* OT $ */

  // ������ ������ ������� �� �������
  GetRegKey("System", "CopyBufferSize", CopyBufferSize, 0);
  if (CopyBufferSize == 0)
    CopyBufferSize = COPY_BUFFER_SIZE; //����. ������ 64�
  if (CopyBufferSize < COPY_BUFFER_SIZE)
    CopyBufferSize = COPY_BUFFER_SIZE;

  CDP.thisClass=this;
  CDP.AltF10=0;
  CDP.FolderPresent=0;
  CDP.FilesPresent=0;

  ShellCopy::Flags=0;
  ShellCopy::Flags|=Move?FCOPY_MOVE:0;
  ShellCopy::Flags|=Link?FCOPY_LINK:0;
  ShellCopy::Flags|=CurrentOnly?FCOPY_CURRENTONLY:0;

  ShowTotalCopySize=Opt.CopyShowTotal != 0;

  *TotalCopySizeText=0;
  SelectedFolderNameLength=0;
  DestPlugin=ToPlugin;
  ToPlugin=FALSE;
  *SrcDriveRoot=0;
  *SrcFSName=0;
  SrcDriveType=0;
  StaticMove=Move;

  ShellCopy::SrcPanel=SrcPanel;
  DestPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  DestPanelMode=DestPlugin ? DestPanel->GetMode():NORMAL_PANEL;
  SrcPanelMode=SrcPanel->GetMode();

  // ***********************************************************************
  // *** Prepare Dialog Controls
  // ***********************************************************************
  const char *HistoryName="Copy";
  /* $ 03.08.2001 IS ������� ����� �����: ����������������� */
  static struct DialogData CopyDlgData[]={
  /* 00 */  DI_DOUBLEBOX,3,1,72,11,0,0,0,0,(char *)MCopyDlgTitle,
  /* 01 */  DI_TEXT,5,2,0,2,0,0,0,0,(char *)MCMLTargetTO,
  /* 02 */  DI_EDIT,5,3,70,3,1,(DWORD)HistoryName,DIF_HISTORY|DIF_EDITEXPAND|DIF_USELASTHISTORY|DIF_EDITPATH,0,"",
  /* 03 */  DI_TEXT,3,4,0,4,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */  DI_CHECKBOX,5,5,0,5,0,0,0,0,(char *)MCopySecurity,
  /* 05 */  DI_CHECKBOX,5,6,0,6,0,0,0,0,(char *)MCopyOnlyNewerFiles,
  /* 06 */  DI_CHECKBOX,5,7,0,7,0,0,0,0,(char *)MCopySymLinkContents,
  /* 07 */  DI_CHECKBOX,5,8,0,7,0,0,0,0,(char *)MCopyMultiActions,
  /* 08 */  DI_TEXT,3,9,0,9,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 09 */  DI_BUTTON,0,10,0,10,0,0,DIF_CENTERGROUP,1,(char *)MCopyDlgCopy,
  /* 10 */  DI_BUTTON,0,10,0,10,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MCopyDlgTree,
  /* 11 */  DI_BUTTON,0,10,0,10,0,0,DIF_CENTERGROUP,0,(char *)MCopyDlgCancel,
  /* 12 */  DI_TEXT,5,2,0,2,0,0,DIF_SHOWAMPERSAND,0,"",
  };
  MakeDialogItems(CopyDlgData,CopyDlg);

  CopyDlg[7].Selected=Opt.MultiCopy;
  /* IS $ */

  if(Link)
  {
    CopyDlg[6].Selected=0;
    CopyDlg[6].Flags|=DIF_DISABLE;
  }

  if (CDP.SelCount==1)
  { // SelName & FileAttr ��� ��������� (��. � ����� ������ �������)

    // ���� ������� � �� ����, �� ������������, ��� ����� ������� �������
    if(Link && NT5 && (CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && DestPanelMode == NORMAL_PANEL)
    {
      CDP.OnlyNewerFiles=CopyDlg[5].Selected=1;
      CDP.FolderPresent=TRUE;
    }
    else
      CDP.OnlyNewerFiles=CopyDlg[5].Selected=0;

    if (SrcPanel->GetType()==TREE_PANEL)
    {
      char NewDir[NM],*ChPtr;
      strcpy(NewDir,SelName);
      if ((ChPtr=strrchr(NewDir,'\\'))!=0)
      {
        *ChPtr=0;
        if (ChPtr==NewDir || *(ChPtr-1)==':')
          strcat(NewDir,"\\");
        FarChDir(NewDir);
      }
    }
    // ������� ��� �� �������� �������.
    sprintf(CopyStr,
            MSG(Move?MMoveFile:(Link?MLinkFile:MCopyFile)),
            TruncPathStr(strcpy(SelNameShort,SelName),33));
  }
  else // �������� ���������!
  {
    int NOper=MCopyFiles;
         if (Move) NOper=MMoveFiles;
    else if (Link) NOper=MLinkFiles;

    // ��������� ����� - ��� ���������
    char StrItems[32];
    itoa(CDP.SelCount,StrItems,10);
    int LenItems=strlen(StrItems);
    int NItems=MCMLItemsA;
    if((LenItems >= 2 && StrItems[LenItems-2] == '1') ||
        StrItems[LenItems-1] >= '5' ||
        StrItems[LenItems-1] == '0')
      NItems=MCMLItemsS;
    else if(StrItems[LenItems-1] == '1')
      NItems=MCMLItems0;
    sprintf(CopyStr,MSG(NOper),CDP.SelCount,MSG(NItems));
  }
  sprintf(CopyDlg[12].Data,"%.65s",CopyStr);

  // ��������� ���������
  strcpy(CopyDlg[5].Data,MSG(Link?MCopySymLink:MCopyOnlyNewerFiles));
  strcpy(CopyDlg[0].Data,MSG(Move?MMoveDlgTitle :(Link?MLinkDlgTitle:MCopyDlgTitle)));
  strcpy(CopyDlg[9].Data,MSG(Move?MCopyDlgRename:(Link?MCopyDlgLink:MCopyDlgCopy)));

  if (Link)
  {
    // ���������� ����� ��� �������, ���� OS < NT2000.
    if(!NT5 || ((CurrentOnly || CDP.SelCount==1) && !(CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY)))
    {
      CopyDlg[5].Flags|=DIF_DISABLE;
      CDP.OnlyNewerFiles=CopyDlg[5].Selected=0;
    }
    // ���������� ����� ��� ����������� �����.
    CopyDlg[4].Selected=1;
    CopyDlg[4].Flags|=DIF_DISABLE;
  }
  else if(DestPanelMode == PLUGIN_PANEL)
  {
    // ���� ��������������� ������ - ������, �� �������� OnlyNewer
    CDP.CopySecurity=CDP.OnlyNewerFiles=0;
    CopyDlg[4].Selected=CopyDlg[5].Selected=0;
    CopyDlg[5].Flags|=DIF_DISABLE;
    CopyDlg[4].Flags|=DIF_DISABLE;
  }

  DestPanel->GetCurDir(DestDir);
  SrcPanel->GetCurDir(SrcDir);

  if (CurrentOnly)
  /* $ 23.03.2002 IS
     ��� ����������� ������ �������� ��� �������� ����� ��� ��� � �������,
     ���� ��� �������� �����������.
  */
  {
    strcpy(CopyDlg[2].Data,SelName);
    if(strpbrk(CopyDlg[2].Data,",;"))
    {
      Unquote(CopyDlg[2].Data);     // ������ ��� ������ �������
      InsertQuote(CopyDlg[2].Data); // ������� � �������, �.�. ����� ���� �����������
    }
  }
  /* IS $ */
  else
    switch(DestPanelMode)
    {
      case NORMAL_PANEL:
        if ((*DestDir==0 || !DestPanel->IsVisible() || !stricmp(SrcDir,DestDir)) && CDP.SelCount==1)
          strcpy(CopyDlg[2].Data,SelName);
        else
        {
          AddEndSlash(strcpy(CopyDlg[2].Data,DestDir));
        }
        CDP.PluginFormat[0]=0;
        /* $ 19.07.2003 IS
           ���� ���� �������� �����������, �� ������� �� � �������, ���� �� ��������
           ������ ��� F5, Enter � �������, ����� ������������ ������� MultiCopy
        */
        if(strpbrk(CopyDlg[2].Data,",;"))
        {
          Unquote(CopyDlg[2].Data);     // ������ ��� ������ �������
          InsertQuote(CopyDlg[2].Data); // ������� � �������, �.�. ����� ���� �����������
        }
        /* IS $ */
        break;
      case PLUGIN_PANEL:
        {
          struct OpenPluginInfo Info;
          DestPanel->GetOpenPluginInfo(&Info);
          /* $ 14.08.2000 SVS
             ������, ��������� �� 40 ��������... :-(
             � ����� ������������ (CopyDlg[2].Data) �� ������ ���������...
             "%.40s:" -> "%s:"
          */
          sprintf(CopyDlg[2].Data,"%s:",NullToEmpty(Info.Format));
          /* SVS $ */
          while (strlen(CopyDlg[2].Data)<2)
            strcat(CopyDlg[2].Data,":");
          strupr(strncpy(CDP.PluginFormat,CopyDlg[2].Data,sizeof(CDP.PluginFormat)-1));
        }
        break;
    }
  strcpy(InitDestDir,CopyDlg[2].Data);

  SrcPanel->GetSelName(NULL,CDP.FileAttr);
  while(SrcPanel->GetSelName(SelName,CDP.FileAttr))
  {
    if(CDP.FileAttr & FILE_ATTRIBUTE_DIRECTORY)
    {
      CDP.FolderPresent=TRUE;
      AddSlash=TRUE;
//      break;
    }
    else
      CDP.FilesPresent=TRUE;
  }

  if(Link) // ������ �� ������ ������ (���������������!)
  {
    int Selected5=CopyDlg[5].Selected;
    if(CDP.SelCount > 1 && !CDP.FilesPresent && CDP.FolderPresent)
      Selected5=1;
    if(!LinkRules(&CopyDlg[9].Flags,
                  &CopyDlg[5].Flags,
                  &Selected5,
                  SrcDir,
                  CopyDlg[2].Data,
                  &CDP))
    {
      _LOGCOPYR(SysLog("return -> %d LinkRules() == 0",__LINE__));
      return;
    }
    CopyDlg[5].Selected=Selected5;
  }

  // ����������� ������� " to"
  CopyDlg[1].X1=CopyDlg[1].X2=CopyDlg[12].X1+strlen(RemoveTrailingSpaces(CopyDlg[12].Data));

  /* $ 15.06.2002 IS
     ��������� ����������� ������ - � ���� ������ ������ �� ������������,
     �� ���������� ��� ����� ����������������. ���� ���������� ���������
     ���������� ������ �����, �� ������� ������.
  */
  if(!Ask)
  {
    strcpy(CopyDlgValue,CopyDlg[2].Data);
    Unquote(CopyDlgValue);
    InsertQuote(CopyDlgValue);
    if(!DestList.Set(CopyDlgValue))
      Ask=TRUE;
  }
  /* IS $ */
  // ***********************************************************************
  // *** ����� � ��������� �������
  // ***********************************************************************
  if (Ask)
  {
    Dialog Dlg(CopyDlg,sizeof(CopyDlg)/sizeof(CopyDlg[0]),CopyDlgProc,(long)&CDP);
    Dlg.SetHelp(Link?"HardSymLink":"CopyFiles");
    Dlg.SetPosition(-1,-1,76,13);

//    Dlg.Show();
    /* $ 02.06.2001 IS
       + �������� ������ ����� � �������� �������, ���� �� �������� ������
    */
    int DlgExitCode;
    for(;;)
    {
      Dlg.ClearDone();
      Dlg.Process();
      DlgExitCode=Dlg.GetExitCode();
      if(DlgExitCode == 9)
      {
        /* $ 03.08.2001 IS
           �������� ������� �� ������� � �������� �� ������ � ����������� ��
           ��������� ����� �����������������
        */
        strcpy(CopyDlgValue,CopyDlg[2].Data);
        Opt.MultiCopy=CopyDlg[7].Selected;
        if(!Opt.MultiCopy || !strpbrk(CopyDlg[2].Data,",;")) // ��������� multi*
        {
           // ������ �������, ������ �������
           RemoveTrailingSpaces(CopyDlg[2].Data);
           Unquote(CopyDlg[2].Data);
           RemoveTrailingSpaces(CopyDlgValue);
           Unquote(CopyDlgValue);

           // ������� �������, ����� "������" ������ ��������������� ���
           // ����������� �� ������� ������������ � ����
           InsertQuote(CopyDlgValue);
        }
        /* IS $ */
        if(DestList.Set(CopyDlgValue) && !strpbrk(CopyDlgValue,ReservedFilenameSymbols))
          break;
        else
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MWarning),
                  MSG(MCopyIncorrectTargetList), MSG(MOk));
      }
      else
        break;
    }
    /* IS $ */
    if(DlgExitCode == 11 || DlgExitCode < 0 || (CopyDlg[9].Flags&DIF_DISABLE))
    {
      if (DestPlugin)
        ToPlugin=-1;
      _LOGCOPYR(SysLog("return -> %d",__LINE__));
      return;
    }
  }
  // ***********************************************************************
  // *** ������ ���������� ������ ����� �������
  // ***********************************************************************
  ShellCopy::Flags|=CopyDlg[4].Selected?FCOPY_COPYSECURITY:0;
  ShellCopy::Flags|=CopyDlg[5].Selected?FCOPY_ONLYNEWERFILES:0;
  ShellCopy::Flags|=CopyDlg[6].Selected?FCOPY_COPYSYMLINKCONTENTS:0;

  if (DestPlugin && !strcmp(CopyDlg[2].Data,InitDestDir))
  {
    ToPlugin=TRUE;
    _LOGCOPYR(SysLog("return -> %d",__LINE__));
    return;
  }

  int WorkMove=Move;

  _LOGCOPYR(SysLog("CopyDlg[2].Data='%s'",CopyDlg[2].Data));
  if(CheckNulOrCon(CopyDlg[2].Data))
    ShellCopy::Flags|=FCOPY_COPYTONUL;
  if(ShellCopy::Flags&FCOPY_COPYTONUL)
  {
    ShellCopy::Flags&=~FCOPY_MOVE;
    StaticMove=WorkMove=0;
  }

  if(CDP.SelCount==1 || (ShellCopy::Flags&FCOPY_COPYTONUL))
    AddSlash=FALSE; //???

  if (DestPlugin==2)
  {
    if (*PluginDestPath!=NULL)
      strcpy(PluginDestPath,CopyDlg[2].Data);
    _LOGCOPYR(SysLog("return -> %d",__LINE__));
    return;
  }

  _tran(SysLog("[%p] ShellCopy::ShellCopy() 5",this));
  if (Opt.Diz.UpdateMode==DIZ_UPDATE_IF_DISPLAYED && SrcPanel->IsDizDisplayed() ||
      Opt.Diz.UpdateMode==DIZ_UPDATE_ALWAYS)
  {
    CtrlObject->Cp()->LeftPanel->ReadDiz();
    CtrlObject->Cp()->RightPanel->ReadDiz();
  }

  CopyBuffer=new char[CopyBufferSize];
  DestPanel->CloseFile();
  *DestDizPath=0;
  SrcPanel->SaveSelection();

  for (int I=0;CopyDlgValue[I]!=0;I++)
    if (CopyDlgValue[I]=='/')
      CopyDlgValue[I]='\\';

  // ����� �� ���������� ����� �����������?
  ShowCopyTime = Opt.CopyTimeRule & ((ShellCopy::Flags&FCOPY_COPYTONUL)?COPY_RULE_NUL:COPY_RULE_FILES);

  // ***********************************************************************
  // **** ����� ��� ���������������� �������� ���������, ����� ����������
  // **** � �������� Copy/Move/Link
  // ***********************************************************************

  int NeedDizUpdate=FALSE;
  int NeedUpdateAPanel=FALSE;

  // ����! ������������� �������� ����������.
  // � ����������� ���� ���� ����� ������������ � ShellCopy::CheckUpdatePanel()
  ShellCopy::Flags|=FCOPY_UPDATEPPANEL;

  /*
     ���� ������� � �������� ����������� �����, �������� ';',
     �� ����� ������� CopyDlgValue �� ������� MultiCopy �
     �������� CopyFileTree ������ ���������� ���.
  */
  /* $ 02.06.2001 IS
     + ��������� ��������� ������ ����� � ������ �������
  */
  {
    ShellCopy::Flags&=~FCOPY_MOVE;
    if(DestList.Set(CopyDlgValue)) // ���� ������ ������� "���������������"
    {
        const char *NamePtr;
        char NameTmp[NM];

        // ������������������ ���������� � ����� ������ (BugZ#171)
//        CopyBufSize = COPY_BUFFER_SIZE; // �������� � 1�
        CopyBufSize = CopyBufferSize;
        ReadOnlyDelMode=ReadOnlyOvrMode=OvrMode=SkipMode=-1;

        // ��������� ���������� �����.
        DestList.Reset();
        CountTarget=0;
        while(DestList.GetNext())
          CountTarget++;

        DestList.Reset();
        TotalFiles=0;
        TotalCopySize=TotalCopiedSize=TotalSkippedSize=0;
        // �������� ����� ������
        if (ShowCopyTime)
          CopyStartTime = clock();

        CopyTitle = new ConsoleTitle(NULL);
        StaticCopyTitle=CopyTitle;

        if(CountTarget > 1)
          StaticMove=WorkMove=0;

        while(NULL!=(NamePtr=DestList.GetNext()))
        {
          CurCopiedSize=0;
          CopyTitle->Set((ShellCopy::Flags&FCOPY_MOVE) ? MSG(MCopyMovingTitle):MSG(MCopyCopyingTitle));

          strcpy(NameTmp, NamePtr);
          _LOGCOPYR(SysLog("NamePtr='%s', Move=%d",NamePtr,WorkMove));

          if(!strcmp(NameTmp,"..") && IsLocalRootPath(SrcDir))
          {
            if(Message(MSG_WARNING,2,MSG(MError),MSG((!Move?MCannotCopyToTwoDot:MCannotMoveToTwoDot)),MSG(MCannotCopyMoveToTwoDot),MSG(MCopySkip),MSG(MCopyCancel)) == 0)
              continue;
            break;
          }

          if(CheckNulOrCon(NameTmp))
            ShellCopy::Flags|=FCOPY_COPYTONUL;
          else
            ShellCopy::Flags&=~FCOPY_COPYTONUL;

          if(ShellCopy::Flags&FCOPY_COPYTONUL)
          {
            ShellCopy::Flags&=~FCOPY_MOVE;
            StaticMove=WorkMove=0;
          }
//          else
//            StaticMove=WorkMove=Move;

          if(DestList.IsEmpty()) // ����� ������ ������� ��������� � ��������� Move.
          {
            StaticMove=WorkMove=Move;
            ShellCopy::Flags|=WorkMove?FCOPY_MOVE:0; // ������ ��� ��������� ��������
            ShellCopy::Flags|=FCOPY_COPYLASTTIME;
          }

          // ���� ���������� ��������� ������ 1 � ����� ��� ���� �������, �� ������
          // ������ ���, ����� �� ����� ��� '\\'
          // ������� ��� �� ������, � ������ ����� NameTmp �� �������� ������.
          if (AddSlash && strpbrk(NameTmp,"*?")==NULL)
            AddEndSlash(NameTmp);

          // ��� ����������� ������ ������� ������ ������� "������"
          if (CDP.SelCount==1 && WorkMove && strpbrk(NameTmp,":\\")==NULL)
            ShowTotalCopySize=FALSE;

          if(WorkMove) // ��� ����������� "�����" ��� �� ����������� ��� "���� �� �����"
          {
            if(IsSameDisk(SrcDir,NameTmp))
              ShowTotalCopySize=FALSE;
            if(CDP.SelCount==1 && CDP.FolderPresent && CheckUpdateAnotherPanel(SrcPanel,SelName))
            {
              NeedUpdateAPanel=TRUE;
            }
          }

          // ������� ���� ��� ����
          *DestDizPath=0;
          ShellCopy::Flags&=~FCOPY_DIZREAD;

          // �������� ���������
          SrcPanel->SaveSelection();

          *DestDriveRoot=0;
          *DestFSName=0;

          int OldCopySymlinkContents=ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS;
          // ���������� - ���� ������ �����������
          SetPreRedrawFunc(ShellCopy::PR_ShellCopyMsg);
          int I=CopyFileTree(NameTmp);
          SetPreRedrawFunc(NULL);

          if(OldCopySymlinkContents)
            ShellCopy::Flags|=FCOPY_COPYSYMLINKCONTENTS;
          else
            ShellCopy::Flags&=~FCOPY_COPYSYMLINKCONTENTS;

          if(I == COPY_CANCEL)
          {
            NeedDizUpdate=TRUE;
            break;
          }

          // ���� "���� ����� � ������������" - ����������� ���������
          if(!DestList.IsEmpty())
            SrcPanel->RestoreSelection();

          // ����������� � �����.
          if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && *DestDizPath)
          {
            char DestDizName[NM*2];
            DestDiz.GetDizName(DestDizName);
            DWORD Attr=GetFileAttributes(DestDizName);
            int DestReadOnly=(Attr!=0xffffffff && (Attr & FA_RDONLY));
            if(DestList.IsEmpty()) // ��������� ������ �� ����� ��������� Op.
              if (WorkMove && !DestReadOnly)
                SrcPanel->FlushDiz();
            DestDiz.Flush(DestDizPath);
          }
        }
        StaticCopyTitle=NULL;
        delete CopyTitle;
    }
    _LOGCOPYR(else SysLog("Error: DestList.Set(CopyDlgValue) return FALSE"));
  }
  /* IS $ */

  // ***********************************************************************
  // *** �������������� ������ ��������
  // *** ���������������/�����/��������
  // ***********************************************************************

  if(NeedDizUpdate) // ��� ����������������� ����� ���� �����, �� ��� ���
  {                 // ����� ����� ��������� ����!
    if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && *DestDizPath)
    {
      char DestDizName[NM+32];
      DestDiz.GetDizName(DestDizName);
      DWORD Attr=GetFileAttributes(DestDizName);
      int DestReadOnly=(Attr!=0xffffffff && (Attr & FA_RDONLY));
      if (Move && !DestReadOnly)
        SrcPanel->FlushDiz();
      DestDiz.Flush(DestDizPath);
    }
  }

#if 1
  SrcPanel->Update(UPDATE_KEEP_SELECTION);
  int LenRenamedName=0;
  if (CDP.SelCount==1 && *RenamedName)
  {
    LenRenamedName=strlen(RenamedName);
    SrcPanel->GoToFile(RenamedName);
  }
#if 1
  if(NeedUpdateAPanel && CDP.FileAttr != -1 && (CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY) && DestPanelMode != PLUGIN_PANEL)
  {
    SrcPanel->GetCurDir(SrcDir);
    DestPanel->SetCurDir(SrcDir,FALSE);
  }
#else
  if(CDP.FileAttr != -1 && (CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY) && DestPanelMode != PLUGIN_PANEL)
  {
    // ���� SrcDir ���������� � DestDir...
    DestPanel->GetCurDir(DestDir);
    SrcPanel->GetCurDir(SrcDir);

    // ���������� ������ ���������� ��������� ������.
//    int LenSrcDir=strlen(SrcDir);
//    int LenDstDir=strlen(DestDir);
//    if(CDP.SelCount == 1 && !LocalStrnicmp(DestDir,RenamedName,Min(LenDstDir,LenRenamedName)) ||
//      (LenDstDir > LenSrcDir && !LocalStrnicmp(DestDir,SrcDir,Min(LenDstDir,LenSrcDir)) && !IsLocalRootPath(SrcDir)))
    if(CheckUpdateAnotherPanel(SrcPanel,SrcDir))
    {
      // ...�� ������.. ;-) �������� ��� �� �������, ��� �������� ��������
      // ������.
      DestPanel->SetCurDir(DestDir,FALSE);
    }
  }
#endif
  // �������� "��������" ������� ��������� ������
  if(ShellCopy::Flags&FCOPY_UPDATEPPANEL)
  {
    DestPanel->SortFileList(TRUE);
    DestPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
  }

  if(SrcPanelMode == PLUGIN_PANEL)
    SrcPanel->SetPluginModified();

  CtrlObject->Cp()->Redraw();
#else

  SrcPanel->Update(UPDATE_KEEP_SELECTION);
  if (CDP.SelCount==1 && *RenamedName)
    SrcPanel->GoToFile(RenamedName);

  SrcPanel->Redraw();

  DestPanel->SortFileList(TRUE);
  DestPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
  DestPanel->Redraw();
#endif
}

long WINAPI ShellCopy::CopyDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
#define DM_CALLTREE (DM_USER+1)
  struct CopyDlgParam *DlgParam;
  DlgParam=(struct CopyDlgParam *)Dialog::SendDlgMessage(hDlg,DM_GETDLGDATA,0,0);

  switch(Msg)
  {
    case DN_BTNCLICK:
    {
      if(Param1 == 10)
      {
        Dialog::SendDlgMessage(hDlg,DM_CALLTREE,0,0);
        return FALSE;
      }
      else if(Param1 == 9)
      {
        Dialog::SendDlgMessage(hDlg,DM_CLOSE,9,0);
      }
      else if(Param1 == 5 && ((DlgParam->thisClass->Flags)&FCOPY_LINK))
      {
        // ����������� ��� ����� �������� ������������ � ������ ����� :-))
        struct FarDialogItem DItem2;
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,2,(long)&DItem2);
        Dialog::SendDlgMessage(hDlg,DN_EDITCHANGE,2,(long)&DItem2);
      }
      break;
    }

    case DM_KEY: // �� ������ ������!
    {
      if(Param2 == KEY_ALTF10 || Param2 == KEY_F10 || Param2 == KEY_SHIFTF10)
      {
        DlgParam->AltF10=Param2 == KEY_ALTF10?1:(Param2 == KEY_SHIFTF10?2:0);
        Dialog::SendDlgMessage(hDlg,DM_CALLTREE,DlgParam->AltF10,0);
        return TRUE;
      }
      break;
    }

    case DN_EDITCHANGE:
      if(Param1 == 2)
      {
        char SrcDir[NM];
        struct FarDialogItem DItem4,DItem5,DItem9;
        DlgParam->thisClass->SrcPanel->GetCurDir(SrcDir);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,4,(long)&DItem4);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,5,(long)&DItem5);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,9,(long)&DItem9);

        // ��� �������� �����?
        if((DlgParam->thisClass->Flags)&FCOPY_LINK)
        {
          DlgParam->thisClass->LinkRules(&DItem9.Flags,
                    &DItem5.Flags,
                    &DItem5.Param.Selected,
                    SrcDir,
                    ((struct FarDialogItem *)Param2)->Data.Data,DlgParam);
        }
        else // ������� Copy/Move
        {
          char Buf[1024];
          struct FarDialogItem *DItem2=(struct FarDialogItem *)Param2;
          strupr(strncpy(Buf,DItem2->Data.Data,sizeof(Buf)-1));
          if(*DlgParam->PluginFormat && strstr(Buf,DlgParam->PluginFormat))
          {
            DItem4.Flags|=DIF_DISABLE;
            DItem5.Flags|=DIF_DISABLE;
            DlgParam->OnlyNewerFiles=DItem5.Param.Selected;
            DlgParam->CopySecurity=DItem4.Param.Selected;
            DItem4.Param.Selected=0;
            DItem5.Param.Selected=0;
          }
          else
          {
            DItem4.Flags&=~DIF_DISABLE;
            DItem5.Flags&=~DIF_DISABLE;
            DItem5.Param.Selected=DlgParam->OnlyNewerFiles;
            DItem4.Param.Selected=DlgParam->CopySecurity;
          }
        }

        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,4,(long)&DItem4);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,5,(long)&DItem5);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,9,(long)&DItem9);
      }
      break;

    case DM_CALLTREE:
    {
      /* $ 13.10.2001 IS
         + ��� ����������������� ��������� ��������� � "������" ������� � ���
           ������������� ������ ����� ����� � �������.
         - ���: ��� ����������������� ��������� � "������" ������� ��
           ���������� � �������, ���� �� �������� � �����
           ����� �������-�����������.
         - ���: ����������� �������� Shift-F10, ���� ������ ����� ���������
           ���� �� �����.
         - ���: ����������� �������� Shift-F10 ��� ����������������� -
           ����������� �������� �������, ������ ������������ ����� ������ �������
           � ������.
      */
      BOOL MultiCopy=Dialog::SendDlgMessage(hDlg,DM_GETCHECK,7,0)==BSTATE_CHECKED;
      char NewFolder[NM], OldFolder[NM];
      *NewFolder = 0;
      Dialog::SendDlgMessage(hDlg,DM_GETTEXTPTR,2,(long)OldFolder);
      if(DlgParam->AltF10 == 2)
      {
        strcpy(NewFolder, OldFolder);
        if(MultiCopy)
        {
          UserDefinedList DestList(0,0,ULF_UNIQUE);
          if(DestList.Set(OldFolder))
          {
            DestList.Reset();
            const char *NamePtr=DestList.GetNext();
            if(NamePtr)
              strncpy(NewFolder, NamePtr, sizeof(NewFolder)-1);
          }
        }
        if(*NewFolder == 0)
          DlgParam->AltF10=-1;
        else // ������� ������ ����
          DeleteEndSlash(NewFolder);
      }

      if(DlgParam->AltF10 != -1)
      {
        {
          FolderTree Tree(NewFolder,
               (DlgParam->AltF10==1?MODALTREE_PASSIVE:
                  (DlgParam->AltF10==2?MODALTREE_FREE:
                     MODALTREE_ACTIVE)),
               25,2,ScrX-7,ScrY-5,FALSE);
        }
        if (*NewFolder)
        {
          AddEndSlash(NewFolder);
          if(MultiCopy) // �����������������
          {
            // ������� �������, ���� ��� �������� �������� �������-�����������
            if(strpbrk(NewFolder,";,"))
              InsertQuote(NewFolder);

            int len=strlen(OldFolder), newlen=strlen(NewFolder), addSep=0;
            addSep=len>0;
            if(len+newlen+addSep<sizeof(OldFolder))// ������������ ����� ������
            {
              if(addSep)
                OldFolder[len++]=';'; // ������� ����������� � ��������� ������
              strcpy(OldFolder+len, NewFolder);
            }
            strcpy(NewFolder, OldFolder);
          }
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,2,(long)NewFolder);
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,2,0);
        }
      }
      DlgParam->AltF10=0;
      return TRUE;
      /* IS $ */
    }
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

BOOL ShellCopy::LinkRules(DWORD *Flags9,DWORD* Flags5,int* Selected5,
                         char *SrcDir,char *DstDir,struct CopyDlgParam *CDP)
{
  char Root[1024];
  *Flags9|=DIF_DISABLE; // �������� �����!
  *Flags5|=DIF_DISABLE;

  if(DstDir && DstDir[0] == '\\' && DstDir[1] == '\\')
  {
    *Selected5=0;
    return TRUE;
  }
//// // _SVS(SysLog("\n---"));
  // �������� ������ ���� � ��������� � ���������
  if(CDP->IsDTSrcFixed == -1)
  {
    char FSysNameSrc[NM];
    strncpy(Root,SrcDir,sizeof(Root));
    Unquote(Root);
    ConvertNameToFull(Root,Root, sizeof(Root));
    GetPathRoot(Root,Root);
//// // _SVS(SysLog("SrcDir=%s",SrcDir));
//// // _SVS(SysLog("Root=%s",Root));
    CDP->IsDTSrcFixed=FAR_GetDriveType(Root);
    CDP->IsDTSrcFixed=CDP->IsDTSrcFixed == DRIVE_FIXED ||
                      IsDriveTypeCDROM(CDP->IsDTSrcFixed) ||
                      (NT5 && WinVer.dwMinorVersion>0?DRIVE_REMOVABLE:0);
    GetVolumeInformation(Root,NULL,0,NULL,NULL,&CDP->FileSystemFlagsSrc,FSysNameSrc,sizeof(FSysNameSrc));
    CDP->FSysNTFS=!stricmp(FSysNameSrc,"NTFS")?TRUE:FALSE;
//// // _SVS(SysLog("FSysNameSrc=%s",FSysNameSrc));
  }

/*
� �������� �� ������ - ����� ������������� [ ] Symbolic link.
� ������ �� �������  - ���������� ������� �������
*/
  // 1. ���� �������� ��������� �� �� ���������� �����
  if(CDP->IsDTSrcFixed || CDP->FSysNTFS)
  {
    char FSysNameDst[NM];
    DWORD FileSystemFlagsDst;

    strncpy(Root,DstDir,sizeof(Root));
    Unquote(Root);

    ConvertNameToFull(Root,Root, sizeof(Root));
    GetPathRoot(Root,Root);
    if(GetFileAttributes(Root) == -1)
      return TRUE;

    //GetVolumeInformation(Root,NULL,0,NULL,NULL,&FileSystemFlagsDst,FSysNameDst,sizeof(FSysNameDst));
    // 3. ���� �������� ��������� �� �� ���������� �����
    CDP->IsDTDstFixed=FAR_GetDriveType(Root);
    CDP->IsDTDstFixed=CDP->IsDTDstFixed == DRIVE_FIXED || IsDriveTypeCDROM(CDP->IsDTSrcFixed);
    *FSysNameDst=0;
    GetVolumeInformation(Root,NULL,0,NULL,NULL,&FileSystemFlagsDst,FSysNameDst,sizeof(FSysNameDst));
    int SameDisk=IsSameDisk(SrcDir,DstDir);
    int IsHardLink=(!CDP->FolderPresent && CDP->FilesPresent && SameDisk && (CDP->IsDTDstFixed || !stricmp(FSysNameDst,"NTFS")));
    // 4. ���� �������� ��������� �� ���������� �����, �������� �� NTFS
    if(!IsHardLink && (CDP->IsDTDstFixed || !stricmp(FSysNameDst,"NTFS")) || IsHardLink)
    {
      if(CDP->SelCount == 1)
      {
        if(CDP->FolderPresent) // Folder?
        {
          // . ���� �������� ��������� �� ���������� ����� NTFS, �� �� �������������� repase point
          if(NT5 &&
//             (CDP->FileSystemFlagsSrc&FILE_SUPPORTS_REPARSE_POINTS) &&
             (FileSystemFlagsDst&FILE_SUPPORTS_REPARSE_POINTS) &&
//    ! ������������� ����������� ��������� �������� � ������ �� ���� -
//      ��� ����� � ����� ������� �� ������� (�� ���, ���� ����)
             CDP->IsDTDstFixed && CDP->IsDTSrcFixed)
          {
            *Flags5 &=~ DIF_DISABLE;
            // ��� �������� �� ��������, ����� �� ������ ���� �� ������� �� ������
            // ����� �... ����� ����� � ��������.
            if(*Selected5 || (!*Selected5 && SameDisk))
               *Flags9 &=~ DIF_DISABLE;

            if(!CDP->IsDTDstFixed && SameDisk)
            {
              *Selected5=0;
              *Flags5 |= DIF_DISABLE;
              *Flags9 &=~ DIF_DISABLE;
            }
          }
          else if(NT /* && !NT5 */ && SameDisk)
          {
            *Selected5=0;
            *Flags9 &=~ DIF_DISABLE;
          }
          else
          {
            *Selected5=0;
//            *Flags9 &=~ DIF_DISABLE;
          }
        }
        else if(SameDisk)// && CDP->FSysNTFS) // ��� ����!
        {
          *Selected5=0;
          *Flags9 &=~ DIF_DISABLE;
        }
      }
      else
      {
        if(CDP->FolderPresent)
        {
          if(NT5 && (FileSystemFlagsDst&FILE_SUPPORTS_REPARSE_POINTS))
          {
            *Flags5 &=~ DIF_DISABLE;
            if(!CDP->FilesPresent)
            {
              *Flags9 &=~ DIF_DISABLE;
            }

            if(!CDP->IsDTDstFixed && SameDisk)
            {
              *Selected5=0;
              *Flags5 |= DIF_DISABLE;
              *Flags9 &=~ DIF_DISABLE;
            }
          }
          else if(NT && !NT5 && SameDisk)
          {
            *Selected5=0;
            *Flags9 &=~ DIF_DISABLE;
          }

          if(CDP->FilesPresent && SameDisk)// && CDP->FSysNTFS)
          {
//            *Selected5=0;
            *Flags9 &=~ DIF_DISABLE;
          }
        }
        else if(SameDisk)// && CDP->FSysNTFS) // ��� ����!
        {
          *Selected5=0;
          *Flags9 &=~ DIF_DISABLE;
        }
      }
    }
  }
  else
    return FALSE;
  return TRUE;
}


ShellCopy::~ShellCopy()
{
  /* $ 13.07.2000 SVS
       ��� �� ������� new[], �� � ������� � delete[] ����... */
  _tran(SysLog("[%p] ShellCopy::~ShellCopy(), CopyBufer=%p",this,CopyBuffer));
  if ( CopyBuffer )
      delete[] CopyBuffer;
  /* SVS $ */
  /* $ 26.05.2001 OT
     ��������� ����������� ������� */
  _tran(SysLog("call (*FrameManager)[0]->UnlockRefresh()"));
  (*FrameManager)[0]->UnlockRefresh();
  /* OT $ */
}


COPY_CODES ShellCopy::CopyFileTree(char *Dest)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::CopyFileTree()"));
  _LOGCOPYR(SysLog("Params: Dest='%s'",Dest));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  //SaveScreen SaveScr;
  DWORD DestAttr;
  _tran(SysLog("[%p] ShellCopy::CopyFileTree()",this));

  char SelName[NM],SelShortName[NM];
  int Length,FileAttr;

  if ((Length=strlen(Dest))==0 || strcmp(Dest,".")==0)
  {
    _LOGCOPYR(SysLog("return COPY_FAILURE -> strlen('%s')=%d",Dest,Length));
    return COPY_FAILURE; //????
  }

  SetCursorType(FALSE,0);

  ShellCopy::Flags&=~(FCOPY_STREAMSKIP|FCOPY_STREAMALL);

  if(TotalCopySize == 0)
  {
    *TotalCopySizeText=0;
    /* $ 19.12.2001 VVM
      ! �� ��������� �������� ��� �������� ������ */
    if (ShowTotalCopySize && !(ShellCopy::Flags&FCOPY_LINK) && !CalcTotalSize())
    {
      _LOGCOPYR(SysLog("return COPY_FAILURE -> if (ShowTotalCopySize && !(ShellCopy::Flags&FCOPY_LINK) && !CalcTotalSize())"));
      return COPY_FAILURE;
    }
  }
  else
    CurCopiedSize=0;

  ShellCopyMsg("","",MSG_LEFTALIGN);

  CopyTime = 0;
  LastShowTime = 0;

  // �������� ��������� ��������� � ����� ����������
  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    //if (Length > 1 && Dest[Length-1]=='\\' && Dest[Length-2]!=':') //??????????
    {
      char NewPath[NM*3];
      strcpy(NewPath,Dest);
      char *Ptr=strrchr(NewPath,'\\');
      if(!Ptr)
        Ptr=strrchr(NewPath,'/');

      if(Ptr)
      {
        *Ptr=0;
        if (Opt.CreateUppercaseFolders && !IsCaseMixed(NewPath))
          LocalStrupr(NewPath);

        DWORD Attr=GetFileAttributes(NewPath);
        if (Attr==0xFFFFFFFF)
        {
          if (CreateDirectory(NewPath,NULL))
            TreeList::AddTreeName(NewPath);
          else
            CreatePath(NewPath);
        }
        else if ((Attr & FILE_ATTRIBUTE_DIRECTORY)==0)
        {
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),MSG(MCopyCannotCreateFolder),NewPath,MSG(MOk));
          _LOGCOPYR(SysLog("return COPY_FAILURE -> %d (CannotCreateFolder)",__LINE__));
          return COPY_FAILURE;
        }
      }
    }
    DestAttr=GetFileAttributes(Dest);
  }


  // �������� ������� "��� �� ����"
  int SameDisk=FALSE;
  if (ShellCopy::Flags&FCOPY_MOVE)
  {
    char SrcDir[NM];
    SrcPanel->GetCurDir(SrcDir);
    SameDisk=IsSameDisk(SrcDir,Dest);
  }

  // �������� ���� ����������� ����� ������.
  SetPreRedrawFunc(ShellCopy::PR_ShellCopyMsg);
  SrcPanel->GetSelName(NULL,FileAttr);
  {
  _LOGCOPYR(CleverSysLog Clev("Run process copy"));
  while (SrcPanel->GetSelName(SelName,FileAttr,SelShortName))
  {
    _LOGCOPYR(SysLog("SelName='%s', FileAttr=0x%08X, SelShortName='%s'",SelName,FileAttr,SelShortName));
    if (!(ShellCopy::Flags&FCOPY_COPYTONUL))
    {
      char FullDest[NM];
      strcpy(FullDest,Dest);
      if(strpbrk(Dest,"*?")!=NULL)
        ShellCopyConvertWildcards(SelName,FullDest);
      DestAttr=GetFileAttributes(FullDest);
      // ������� ������ � ����� ����������
      if (*DestDriveRoot==0)
      {
        GetPathRoot(FullDest,DestDriveRoot);
        DestDriveType=FAR_GetDriveType(strchr(FullDest,'\\')!=NULL ? DestDriveRoot:NULL);
        if(GetFileAttributes(DestDriveRoot) != -1)
          if(!GetVolumeInformation(DestDriveRoot,NULL,0,NULL,NULL,&DestFSFlags,DestFSName,sizeof(DestFSName)))
            *DestFSName=0;
        _LOGCOPYR(SysLog("DestDriveRoot='%s', DestDriveType=%d, DestFSFlags=0x%08X, DestFSName='%s'",DestDriveRoot,DestDriveType,DestFSFlags,DestFSName));
      }
    }

    char DestPath[NM];
    strcpy(DestPath,Dest);
    HANDLE FindHandle;
    WIN32_FIND_DATA SrcData;
    int CopyCode=COPY_SUCCESS,KeepPathPos;

    ShellCopy::Flags&=~FCOPY_OVERWRITENEXT;

    if (*SrcDriveRoot==0 || LocalStrnicmp(SelName,SrcDriveRoot,strlen(SrcDriveRoot))!=0)
    {
      GetPathRoot(SelName,SrcDriveRoot);
      SrcDriveType=FAR_GetDriveType(strchr(SelName,'\\')!=NULL ? SrcDriveRoot:NULL);
      if(GetFileAttributes(SrcDriveRoot) != -1)
        if(!GetVolumeInformation(SrcDriveRoot,NULL,0,NULL,NULL,&SrcFSFlags,SrcFSName,sizeof(SrcFSName)))
          *SrcFSName=0;
      _LOGCOPYR(SysLog("SrcDriveRoot='%s', SrcDriveType=%d, SrcFSFlags=0x%08X, SrcFSName='%s'",SrcDriveRoot,SrcDriveType,SrcFSFlags,SrcFSName));
    }

    if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
      SelectedFolderNameLength=strlen(SelName);
    else
      SelectedFolderNameLength=0;

    // "�������" � ������ ���� ������� - �������� ������ �������, ���������� �� �����
    if(DestDriveType == DRIVE_REMOTE || SrcDriveType == DRIVE_REMOTE)
      ShellCopy::Flags|=FCOPY_COPYSYMLINKCONTENTS;

    KeepPathPos=PointToName(SelName)-SelName;
    _LOGCOPYR(SysLog("%d KeepPathPos=%d",__LINE__,KeepPathPos));
    if(!stricmp(SrcDriveRoot,SelName) && (ShellCopy::Flags&FCOPY_CREATESYMLINK)) // �� ������� ��������� �� "��� ������ �����?"
    {
      _LOGCOPYR(SysLog("%d if(!stricmp(SrcDriveRoot,SelName) &&...",__LINE__));
      SrcData.dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;
      //// // _SVS(SysLog("Dest='%s'",Dest));
    }
    else
    {
      // �������� �� �������� ;-)
      if ((FindHandle=FindFirstFile(SelName,&SrcData))==INVALID_HANDLE_VALUE)
      {
        CopyTime+= (clock() - CopyStartTime);
        strcat(DestPath,SelName);
        ShellCopy::ShellCopyMsg(SelName,DestPath,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);
        if (Message(MSG_DOWN|MSG_WARNING,2,MSG(MError),MSG(MCopyCannotFind),
                SelName,MSG(MSkip),MSG(MCancel))==1)
        {
          _LOGCOPYR(SysLog("return COPY_FAILURE -> MCopyCannotFind"));
          return COPY_FAILURE;
        }
        CopyStartTime = clock();
        /* $ 23.03.2002 VVM
          ! ������ ���, �.�. ��������� SrcData ���������� */
  //      int64 SubSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
  //      TotalCopySize-=SubSize;
        /* VVM $ */
        _LOGCOPYR(SysLog("%d continue;",__LINE__));
        continue;
      }
      FindClose(FindHandle);
    }

    // ���� ��� ������� � ����� ������� �������...
    if((SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
       (ShellCopy::Flags&FCOPY_CREATESYMLINK)
      )
    {
      /*
      ���� �����, ���� ����� �� ������ ������ �� ������!
      char SrcRealName[NM*2];
      ConvertNameToReal(SelName,SrcRealName,sizeof(SrcRealName));
      switch(MkSymLink(SrcRealName,Dest,ShellCopy::Flags))
      */
      switch(MkSymLink(SelName,Dest,ShellCopy::Flags))
      {
        case 2:
          break;
        case 1:
            // ������� (Ins) ��������� ���������, ALT-F6 Enter - ��������� � ����� �� �������.
            if ((!(ShellCopy::Flags&FCOPY_CURRENTONLY)) && (ShellCopy::Flags&FCOPY_COPYLASTTIME))
              SrcPanel->ClearLastGetSelection();
            _LOGCOPYR(SysLog("%d continue;",__LINE__));
            continue;
        case 0:
          _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
          return COPY_FAILURE;
      }
    }

    //KeepPathPos=PointToName(SelName)-SelName;

    // �����?
    if ((ShellCopy::Flags&FCOPY_MOVE))
    {
      // ����, � ��� �� ���� "��� �� ����"?
      if (KeepPathPos && PointToName(Dest)==Dest)
      {
        strcpy(DestPath,SelName);
        strcpy(DestPath+KeepPathPos,Dest);
        SameDisk=TRUE;
      }

      if (!SameDisk || (SrcData.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) && (ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS))
        CopyCode=COPY_FAILURE;
      else
      {
        CopyCode=ShellCopyOneFile(SelName,SrcData,DestPath,KeepPathPos,1);
        if (CopyCode==COPY_SUCCESS_MOVE)
        {
          if (*DestDizPath)
          {
            if (*RenamedName)
            {
              DestDiz.DeleteDiz(SelName,SelShortName);
              SrcPanel->CopyDiz(SelName,SelShortName,RenamedName,RenamedName,&DestDiz);
            }
            else
            {
              if (*CopiedName==0)
                strcpy(CopiedName,SelName);
              SrcPanel->CopyDiz(SelName,SelShortName,CopiedName,CopiedName,&DestDiz);
              SrcPanel->DeleteDiz(SelName,SelShortName);
            }
          }
          continue;
        }

        if (CopyCode==COPY_CANCEL)
        {
         _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
          return COPY_CANCEL;
        }

        if (CopyCode==COPY_NEXT)
        {
          int64 CurSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
          TotalCopiedSize = TotalCopiedSize - CurCopiedSize + CurSize;
          TotalSkippedSize = TotalSkippedSize + CurSize - CurCopiedSize;
//          TotalCopySize-=SubSize;
          continue;
        }

        if (!(ShellCopy::Flags&FCOPY_MOVE) || CopyCode==COPY_FAILURE)
          ShellCopy::Flags|=FCOPY_OVERWRITENEXT;
      }
    }

    if (!(ShellCopy::Flags&FCOPY_MOVE) || CopyCode==COPY_FAILURE)
    {
      _LOGCOPYR(SysLog("%d Call ShellCopyOneFile('%s',%p,'%s',%d,0)",__LINE__,NullToEmpty(SelName),&SrcData,NullToEmpty(Dest),KeepPathPos));
      CopyCode=ShellCopyOneFile(SelName,SrcData,Dest,KeepPathPos,0);
      _LOGCOPYR(SysLog("return %d",CopyCode));
      ShellCopy::Flags&=~FCOPY_OVERWRITENEXT;

      if (CopyCode==COPY_CANCEL)
      {
        _LOGCOPYR(SysLog("%d CopyCode==COPY_CANCEL",__LINE__));
        return COPY_CANCEL;
      }

      if (CopyCode!=COPY_SUCCESS)
      {
        int64 CurSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
        TotalCopiedSize = TotalCopiedSize - CurCopiedSize + CurSize;
        if (CopyCode == COPY_NEXT)
          TotalSkippedSize = TotalSkippedSize + CurSize - CurCopiedSize;
        continue;
      }
    }

    if (CopyCode==COPY_SUCCESS && !(ShellCopy::Flags&FCOPY_COPYTONUL) && *DestDizPath)
    {
      if (*CopiedName==0)
        strcpy(CopiedName,SelName);
      SrcPanel->CopyDiz(SelName,SelShortName,CopiedName,CopiedName,&DestDiz);
    }
#if 0
    // ���� [ ] Copy contents of symbolic links
    if((SrcData.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) && !(ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS))
    {
      //������� �������
      switch(MkSymLink(SelName,Dest,FCOPY_LINK/*|FCOPY_NOSHOWMSGLINK*/))
      {
        case 2:
          break;
        case 1:
            // ������� (Ins) ��������� ���������, ALT-F6 Enter - ��������� � ����� �� �������.
            if ((!(ShellCopy::Flags&FCOPY_CURRENTONLY)) && (ShellCopy::Flags&FCOPY_COPYLASTTIME))
              SrcPanel->ClearLastGetSelection();
            _LOGCOPYR(SysLog("%d continue;",__LINE__));
            continue;
        case 0:
          _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
          return COPY_FAILURE;
      }
      continue;
    }
#endif

    // ���� ������� - �������� ���������� ����������...
    if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      _LOGCOPYR(CleverSysLog Clev("if(SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)"));

      int SubCopyCode;
      char SubName[NM],FullName[NM];
      ScanTree ScTree(TRUE,TRUE,ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS);

      strcpy(SubName,SelName);
      strcat(SubName,"\\");
      if (DestAttr==(DWORD)-1)
        KeepPathPos=strlen(SubName);

      int NeedRename=!((SrcData.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) && (ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS) && (ShellCopy::Flags&FCOPY_MOVE));

      ScTree.SetFindPath(SubName,"*.*",FSCANTREE_FILESFIRST);
      while (ScTree.GetNextName(&SrcData,FullName, sizeof (FullName)-1))
      {
        _LOGCOPYR(SysLog("FullName='%s', SameDisk=%d",FullName,SameDisk));

        int AttemptToMove=FALSE;
        if ((ShellCopy::Flags&FCOPY_MOVE) && SameDisk && (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
        {
          AttemptToMove=TRUE;

          switch(ShellCopyOneFile(FullName,SrcData,Dest,KeepPathPos,NeedRename)) // 1
          {
            case COPY_CANCEL:
              _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
              return COPY_CANCEL;

            case COPY_NEXT:
            {
              int64 CurSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
              TotalCopiedSize = TotalCopiedSize - CurCopiedSize + CurSize;
              TotalSkippedSize = TotalSkippedSize + CurSize - CurCopiedSize;
              continue;
            }

            case COPY_SUCCESS_MOVE:
              continue;

            case COPY_SUCCESS:
              if(!NeedRename) // ������� ��� ����������� ����������� ������� � ������ "���������� ���������� ���..."
              {
                int64 CurSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
                TotalCopiedSize = TotalCopiedSize - CurCopiedSize + CurSize;
                TotalSkippedSize = TotalSkippedSize + CurSize - CurCopiedSize;
                continue;     // ...  �.�. �� ��� �� ������, � �����������, �� ���, �� ���� �������� �������� � ���� ������
              }
          }
        }

        int SaveOvrMode=OvrMode;

        if (AttemptToMove)
          OvrMode=1;

        _LOGCOPYR(SysLog("%d Call ShellCopyOneFile('%s',%p,'%s',%d,0)",__LINE__,FullName,&SrcData,Dest,KeepPathPos));
        SubCopyCode=ShellCopyOneFile(FullName,SrcData,Dest,KeepPathPos,0);
        _LOGCOPYR(SysLog("SubCopyCode=%d",SubCopyCode));

        if (AttemptToMove)
          OvrMode=SaveOvrMode;

        if (SubCopyCode==COPY_CANCEL)
        {
          _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
          return COPY_CANCEL;
        }

        if (SubCopyCode==COPY_NEXT)
        {
          int64 CurSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
          TotalCopiedSize = TotalCopiedSize - CurCopiedSize + CurSize;
          TotalSkippedSize = TotalSkippedSize + CurSize - CurCopiedSize;
        }

        if (SubCopyCode==COPY_SUCCESS)
        {
          if(ShellCopy::Flags&FCOPY_MOVE)
          {
            if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
              _LOGCOPYR(SysLog("************* %d (%s) ******************",__LINE__,FullName));
              if (ScTree.IsDirSearchDone() ||
                  ((SrcData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) && !(ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS)))
              {
                if (SrcData.dwFileAttributes & FA_RDONLY)
                  SetFileAttributes(FullName,FILE_ATTRIBUTE_NORMAL);
                _LOGCOPYR(SysLog("************* %d (%s) Pred FAR_RemoveDirectory ******************",__LINE__,FullName));
                if (FAR_RemoveDirectory(FullName))
                  TreeList::DelTreeName(FullName);
                _LOGCOPYR(else SysLog("************* %d (%s) ******************",__LINE__,FullName));
              }
            }
            // ����� ����� �������� �� FSCANTREE_INSIDEJUNCTION, �����
            // ��� ������� ����� �������� �����, ��� ������ �����������!
            else if(!ScTree.InsideJunction())
            {
              if (DeleteAfterMove(FullName,SrcData.dwFileAttributes)==COPY_CANCEL)
              {
                _LOGCOPYR(SysLog("return COPY_CANCEL -> %d DeleteAfterMove('%s',0x%08X) == COPY_CANCEL",__LINE__,FullName,SrcData.dwFileAttributes));
                return COPY_CANCEL;
              }
            }
          }
        }
      }

      if ((ShellCopy::Flags&FCOPY_MOVE) && CopyCode==COPY_SUCCESS)
      {
        if (FileAttr & FA_RDONLY)
          SetFileAttributes(SelName,FILE_ATTRIBUTE_NORMAL);

        if (FAR_RemoveDirectory(SelName))
        {
          TreeList::DelTreeName(SelName);

          if (*DestDizPath)
            SrcPanel->DeleteDiz(SelName,SelShortName);
        }
      }
    }
    else if ((ShellCopy::Flags&FCOPY_MOVE) && CopyCode==COPY_SUCCESS)
    {
      int DeleteCode;
      if ((DeleteCode=DeleteAfterMove(SelName,FileAttr))==COPY_CANCEL)
      {
        _LOGCOPYR(SysLog("return COPY_CANCEL -> %d DeleteAfterMove('%s',0x%08X) == COPY_CANCEL",__LINE__,SelName,FileAttr));
        return COPY_CANCEL;
      }

      if (DeleteCode==COPY_SUCCESS && *DestDizPath)
        SrcPanel->DeleteDiz(SelName,SelShortName);
    }

    if ((!(ShellCopy::Flags&FCOPY_CURRENTONLY)) && (ShellCopy::Flags&FCOPY_COPYLASTTIME))
    {
      SrcPanel->ClearLastGetSelection();
    }
  }
  }

  _tran(SysLog("[%p] ShellCopy::CopyFileTree() end",this));
  return COPY_SUCCESS; //COPY_SUCCESS_MOVE???
}

COPY_CODES ShellCopy::ShellCopyOneFile(const char *Src,
                                       const WIN32_FIND_DATA &SrcData,
                                       const char *Dest, int KeepPathPos,
                                       int Rename)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::ShellCopyOneFile()"));
  _LOGCOPYR(SysLog("Params: Src='%s', Dest='%s', KeepPathPos=%d, Rename=%d",Src, Dest,KeepPathPos,Rename));
  char DestPath[2*NM];
  DWORD DestAttr;
  HANDLE FindHandle=INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA DestData;
  /* $ 25.05.2002 IS
     + RenameToShortName - ��������� SameName � ���������� ������ ���� �����,
       ����� ������ ����������������� � ��� �� _��������_ ���.
  */
  int SameName=0,
      RenameToShortName=0,
      Append=0;
  /* IS $ */

  *RenamedName=*CopiedName=0;

  CurCopiedSize = 0; // �������� ������� ��������

  if (CheckForEscSilent())
  {
    CopyTime+= (clock() - CopyStartTime);
    int AbortOp = ConfirmAbortOp();
    CopyStartTime = clock();
    if (AbortOp)
    {
      _LOGCOPYR(SysLog("return COPY_CANCEL -> %d Pressed ESC",__LINE__));
      return(COPY_CANCEL);
    }
  }

  strcpy(DestPath,Dest);

  SetPreRedrawFunc(ShellCopy::PR_ShellCopyMsg);
  ShellCopyConvertWildcards(Src,DestPath);

  char *NamePtr=PointToName(DestPath);

  DestAttr=-1;

  if (DestPath[0]=='\\' && DestPath[1]=='\\')
  {
    char Root[NM];
    GetPathRoot(DestPath,Root);
    int RootLength=strlen(Root);

    if (RootLength>0 && Root[RootLength-1]=='\\')
      Root[RootLength-1]=0;

    if (strcmp(DestPath,Root)==0)
      DestAttr=FILE_ATTRIBUTE_DIRECTORY;
  }

  _LOGCOPYR(SysLog("NamePtr='%s', TestParentFolderName()=>%d",NamePtr,TestParentFolderName(NamePtr)));
  if (*NamePtr==0 || TestParentFolderName(NamePtr))
    DestAttr=FILE_ATTRIBUTE_DIRECTORY;

  *DestData.cFileName=0;

  if (DestAttr==(DWORD)-1 && (FindHandle=FindFirstFile(DestPath,&DestData))!=INVALID_HANDLE_VALUE)
  {
    FindClose(FindHandle);
    DestAttr=DestData.dwFileAttributes;
  }
  _LOGCOPYR(SysLog("%d DestAttr=0x%08X",__LINE__,DestAttr));

  if (DestAttr!=(DWORD)-1 && (DestAttr & FILE_ATTRIBUTE_DIRECTORY))
  {
    int CmpCode;
    if ((CmpCode=CmpFullNames(Src,DestPath))!=0)
    {
      _LOGCOPYR(SysLog("%d CmpCode=%d, CmpFullNames('%s','%s')",__LINE__,CmpCode,Src,DestPath));
      SameName=1;
      /* $ 25.05.2002 IS
         �������� �� ��������, ����� ����������������� _�������_ � ���� ��
         _��������_ ���
      */
      if(CmpCode!=2 && Rename)
      {
         if(!strcmp(PointToName(Src),PointToName(DestPath)))
           CmpCode=2; // ������: ����� ��� ��������� �������
         else
           RenameToShortName = (!LocalStricmp(DestData.cFileName,
             SrcData.cFileName) &&
             0!=LocalStricmp(DestData.cAlternateFileName,SrcData.cFileName));
      }
      /* IS $ */
      if (CmpCode==2 || !Rename)
      {
        CopyTime+= (clock() - CopyStartTime);
        SetMessageHelp("ErrCopyItSelf");
        Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),MSG(MCannotCopyFolderToItself1),
                Src,MSG(MCannotCopyFolderToItself2),MSG(MOk));
        CopyStartTime = clock();
        _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
        return(COPY_CANCEL);
      }
    }
    _LOGCOPYR(else SysLog("%d CmpFullNames() == 0",__LINE__));

    if (!SameName)
    {
      int Length=strlen(DestPath);

      if (DestPath[Length-1]!='\\' && DestPath[Length-1]!=':')
        strcat(DestPath,"\\");

      const char *PathPtr=Src+KeepPathPos;

      if (*PathPtr && KeepPathPos==0 && PathPtr[1]==':')
        PathPtr+=2;

      if (*PathPtr=='\\')
        PathPtr++;

      strcat(DestPath,PathPtr);

      if ((FindHandle=FindFirstFile(DestPath,&DestData))==INVALID_HANDLE_VALUE)
        DestAttr=-1;
      else
      {
        FindClose(FindHandle);
        DestAttr=DestData.dwFileAttributes;
      }
      _LOGCOPYR(SysLog("%d DestPath='%s', DestAttr=0x%08X",__LINE__,DestPath,DestAttr));
    }
  }

  /* $ 25.05.2002 IS
     ������ � �������� ������� nul==con, ������ ����, ����� �������� � �������
     �������� �� con, ������ ������ �������� �� FCOPY_COPYTONUL
  */
  if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && stricmp(DestPath,"prn")!=0)
  /* IS $ */
    SetDestDizPath(DestPath);

  ShellCopyMsg(Src,DestPath,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);

  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    // �������� ���������� ��������� �� ������
    switch(CheckStreams(Src,DestPath))
    {
      case COPY_NEXT:
        _LOGCOPYR(SysLog("return COPY_NEXT -> %d CheckStreams('%s','%s')",__LINE__,Src,DestPath));
        return COPY_NEXT;
      case COPY_CANCEL:
        _LOGCOPYR(SysLog("return COPY_CANCEL -> %d CheckStreams('%s','%s')",__LINE__,Src,DestPath));
        return COPY_CANCEL;
    }

    if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      _LOGCOPYR(CleverSysLog Clev("if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)"));
      if (!Rename)
        strcpy(CopiedName,PointToName(DestPath));

      /* $ 25.05.2002 IS
         �� ��������� ����� ��������, ����� RenameToShortName.
      */
      if (DestAttr!=(DWORD)-1 && !RenameToShortName)
      /* IS $ */
      {
        if ((DestAttr & FILE_ATTRIBUTE_DIRECTORY) && !SameName)
        {
          DWORD SetAttr=SrcData.dwFileAttributes;
          if (IsDriveTypeCDROM(SrcDriveType) && Opt.ClearReadOnly && (SetAttr & FA_RDONLY))
            SetAttr&=~FA_RDONLY;

          _LOGCOPYR(SysLog("%d SetAttr=0x%08X, DestAttr=0x%08X",__LINE__,SetAttr,DestAttr));

          if (SetAttr!=DestAttr)
            ShellSetAttr(DestPath,SetAttr);

          char SrcFullName[NM];
          if (ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
          {
            _LOGCOPYR(SysLog("return COPY_NEXT -> %d ConvertNameToFull()-but",__LINE__));
            return(COPY_NEXT);
          }
          _LOGCOPYR(SysLog("%d SrcFullName='%s', DestPath='%s', return ??;",__LINE__,SrcFullName,DestPath));
          return(strcmp(DestPath,SrcFullName)==0 ? COPY_NEXT:COPY_SUCCESS);
        }

        int Type=GetFileTypeByName(DestPath);
        if (Type==FILE_TYPE_CHAR || Type==FILE_TYPE_PIPE)
        {
          _LOGCOPYR(SysLog("return %d -> %d if (Type==FILE_TYPE_CHAR || Type==FILE_TYPE_PIPE)",(Rename ? COPY_NEXT:COPY_SUCCESS),__LINE__));
          return(Rename ? COPY_NEXT:COPY_SUCCESS);
        }
      }

      if (Rename)
      {
        char SrcFullName[NM],DestFullName[NM];

        if (ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
        {
          _LOGCOPYR(SysLog("return COPY_NEXT -> %d ConvertNameToFull()-but",__LINE__));
          return(COPY_NEXT);
        }

        /* $ 18.07.2001 VVM
          + �������� �������������, ���� �� ������� */
        while (1)
        {
          /* $ 25.05.2002 IS
             ��������� ��������� RenameToShortName ��� ��������.
          */
          BOOL SuccessMove=RenameToShortName?MoveFileThroughTemp(Src,DestPath):MoveFile(Src,DestPath);
          if (SuccessMove)
          /* IS $ */
          {
            if (PointToName(DestPath)==DestPath)
              strcpy(RenamedName,DestPath);
            else
              strcpy(CopiedName,PointToName(DestPath));
//          ConvertNameToFull(DestPath,DestFullName, sizeof(DestFullName));

            if (ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
            {
              _LOGCOPYR(SysLog("return COPY_NEXT -> %d ConvertNameToFull()-but",__LINE__));
              return(COPY_NEXT);
            }

            TreeList::RenTreeName(SrcFullName,DestFullName);
            _LOGCOPYR(SysLog("return %d -> %d",(SameName ? COPY_NEXT:COPY_SUCCESS_MOVE),__LINE__));
            return(SameName ? COPY_NEXT:COPY_SUCCESS_MOVE);
          }
          else // $ 18.07.2001 VVM
          {    //  ��������, ��� ������, ���� �� ������ ������������� �������
            /* $ 24.04.2002 VVM
              + ����� ��� ��� ���������� - ���������� ������� ������ */
            int LastError = GetLastError();
            int CopySecurity = ShellCopy::Flags&FCOPY_COPYSECURITY;
            SECURITY_ATTRIBUTES sa;
            if ((CopySecurity) && !GetSecurity(Src,sa))
              CopySecurity = FALSE;
            if (CreateDirectory(DestPath,CopySecurity?&sa:NULL))
            {
              if (PointToName(DestPath)==DestPath)
                strcpy(RenamedName,DestPath);
              else
                strcpy(CopiedName,PointToName(DestPath));
//              if (ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
//                return(COPY_NEXT);
//              TreeList::RenTreeName(SrcFullName,DestFullName);
#if 0
              // ��� ���������, �������� ���� �������� - �������� �������
              if(SrcData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
              {
                char SrcRealName[NM*2];
                ConvertNameToReal(Src,SrcRealName,sizeof(SrcRealName));
                switch(MkSymLink(SrcRealName,DestPath,FCOPY_LINK))
                {
                  case 2:
                    _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
                    return COPY_CANCEL;
                  case 1: break;
                  case 0:
                    _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
                    return COPY_FAILURE;
                }
              }
#endif
              TreeList::AddTreeName(DestPath);
              _LOGCOPYR(SysLog("return COPY_SUCCESS -> %d",__LINE__));
              return(COPY_SUCCESS);
            }
            else
            {
              CopyTime+= (clock() - CopyStartTime);
              SetLastError(LastError);
              int MsgCode = Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                                    MSG(MCopyCannotRenameFolder),Src,MSG(MCopyRetry),
                                    MSG(MCopyIgnore),MSG(MCopyCancel));
              CopyStartTime = clock();
              switch (MsgCode)
              {
                case 0:  continue;
                case 1:
                  _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
                  return (COPY_FAILURE);
                default:
                  _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
                  return (COPY_CANCEL);
              } /* switch */
            } /* else */
            /* VVM $ */
          } /* else */
        } /* while */
        /* VVM $ */
      }

      SECURITY_ATTRIBUTES sa;
      if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(Src,sa))
      {
        _LOGCOPYR(SysLog("return COPY_CANCEL -> %d GetSecurity()==0",__LINE__));
        return COPY_CANCEL;
      }

      while (!CreateDirectory(DestPath,(ShellCopy::Flags&FCOPY_COPYSECURITY) ? &sa:NULL))
      {
        int MsgCode;
        CopyTime+= (clock() - CopyStartTime);
        MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                        MSG(MCopyCannotCreateFolder),DestPath,MSG(MCopyRetry),
                        MSG(MCopySkip),MSG(MCopyCancel));
        CopyStartTime = clock();

        if (MsgCode!=0)
        {
          _LOGCOPYR(SysLog("return %d -> %d",((MsgCode==-2 || MsgCode==2) ? COPY_CANCEL:COPY_NEXT),__LINE__));
          return((MsgCode==-2 || MsgCode==2) ? COPY_CANCEL:COPY_NEXT);
        }
      }

      DWORD SetAttr=SrcData.dwFileAttributes;

      if (IsDriveTypeCDROM(SrcDriveType) && Opt.ClearReadOnly && (SetAttr & FA_RDONLY))
        SetAttr&=~FA_RDONLY;

      if(!(ShellCopy::Flags&FCOPY_SKIPSETATTRFLD) && (SetAttr&(~FILE_ATTRIBUTE_DIRECTORY)) != 0 /*SetAttr != SrcData.dwFileAttributes && SetAttr != FILE_ATTRIBUTE_DIRECTORY && */) // ������ 0x10 �����!
      {
        // �� ����� ���������� ����������, ���� ������� � �������
        // � ������������ FILE_ATTRIBUTE_ENCRYPTED (� �� ��� ����� ��������� ����� CreateDirectory)
        // �.�. ���������� ������ ���.
        if(GetFileAttributes(DestPath)&FILE_ATTRIBUTE_ENCRYPTED)
          SetAttr&=~FILE_ATTRIBUTE_COMPRESSED;

        if(SetAttr&FILE_ATTRIBUTE_COMPRESSED)
        {
          int MsgCode;
          while(1)
          {
            CopyTime+= (clock() - CopyStartTime);
            MsgCode=ESetFileCompression(DestPath,1,0);
            CopyStartTime = clock();
            if(MsgCode)
            {
              if(MsgCode == 2)
                ShellCopy::Flags|=FCOPY_SKIPSETATTRFLD;
              break;
            }
            if(MsgCode != 1)
              return (MsgCode==2) ? COPY_NEXT:COPY_CANCEL;
          }
        }
#if 0
        else if(SetAttr&FILE_ATTRIBUTE_ENCRYPTED)
        {
          int MsgCode;
          while(1)
          {
            CopyTime+= (clock() - CopyStartTime);
            MsgCode=ESetFileEncryption(DestPath,1,0);
            CopyStartTime = clock();
            if(MsgCode)
            {
              if(MsgCode == 2)
                ShellCopy::Flags|=FCOPY_SKIPSETATTRFLD;
              break;
            }
            if(MsgCode != 1)
              return (MsgCode==2) ? COPY_NEXT:COPY_CANCEL;
          }
        }
#endif

        while(!ShellSetAttr(DestPath,SetAttr))
        {
          int MsgCode;
          CopyTime+= (clock() - CopyStartTime);
          MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,4,MSG(MError),
                          MSG(MCopyCannotChangeFolderAttr),DestPath,
                          MSG(MCopyRetry),MSG(MCopySkip),MSG(MCopySkipAll),MSG(MCopyCancel));
          CopyStartTime = clock();

          if (MsgCode!=0)
          {
            if(MsgCode==1)
              break;
            if(MsgCode==2)
            {
              ShellCopy::Flags|=FCOPY_SKIPSETATTRFLD;
              break;
            }
            FAR_RemoveDirectory(DestPath);
            _LOGCOPYR(SysLog("return COPY_CANCEL -> %d ShellSetAttr('%s', 0x%08X)==0",__LINE__,DestPath,SetAttr));
            return((MsgCode==-2 || MsgCode==3) ? COPY_CANCEL:COPY_NEXT);
          }
        }
      }
#if 1
      // ��� ���������, �������� ���� �������� - �������� �������
      // ���� [ ] Copy contents of symbolic links
      if(SrcData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT && !(ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS))
      {
        char SrcRealName[NM*2];
        ConvertNameToFull(Src,SrcRealName,sizeof(SrcRealName));
        switch(MkSymLink(SrcRealName,DestPath,FCOPY_LINK))
        {
          case 2:
            _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
            return COPY_CANCEL;
          case 1:
            break;
          case 0:
            _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
            return COPY_FAILURE;
        }
      }
#endif
      TreeList::AddTreeName(DestPath);
      _LOGCOPYR(SysLog("return COPY_SUCCESS -> %d",__LINE__));
      return COPY_SUCCESS;
    }

    if (DestAttr!=(DWORD)-1 && (DestAttr & FILE_ATTRIBUTE_DIRECTORY)==0)
    {
      _LOGCOPYR(SysLog("%d enter to !FILE_ATTRIBUTE_DIRECTORY",__LINE__));
      /* $ 25.05.2002 IS
         + �� ��������� RenameToShortName ������ ���
         + ����� �� ������ ������ ������, ��������� ������� ���������� �������
           ������, � �.�. � nFileSizeHigh
      */
      if(!RenameToShortName)
      {
        if (SrcData.nFileSizeHigh==DestData.nFileSizeHigh &&
            SrcData.nFileSizeLow==DestData.nFileSizeLow)
      /* IS $ */
        {
          int CmpCode;
          if ((CmpCode=CmpFullNames(Src,DestPath))!=0)
          {
            SameName=1;
            /* $ 25.05.2002 IS
               �������� �� ��������, ����� ����������������� _����_ � ���� ��
               _��������_ ���
            */
            if(CmpCode!=2 && Rename)
            {
               if(!strcmp(PointToName(Src),PointToName(DestPath)))
                 CmpCode=2; // ������: ����� ��� ��������� �������
               else
               {
                 RenameToShortName = (!LocalStricmp(DestData.cFileName,
                   SrcData.cFileName) &&
                   0!=LocalStricmp(DestData.cAlternateFileName,SrcData.cFileName));
                 _LOGCOPYR(SysLog("%d RenameToShortName=%d (LocalStricmp('%s','%s')=%d)",__LINE__,RenameToShortName,DestData.cAlternateFileName,SrcData.cFileName,LocalStricmp(DestData.cAlternateFileName,SrcData.cFileName)));
               }
            }
            /* IS $ */
            if (CmpCode==2 || !Rename)
            {
              CopyTime+= (clock() - CopyStartTime);
              Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),MSG(MCannotCopyFileToItself1),
                      Src,MSG(MCannotCopyFileToItself2),MSG(MOk));
              CopyStartTime = clock();
              _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
              return(COPY_CANCEL);
            }
          }
        }

        int RetCode, AskCode;
        CopyTime+= (clock() - CopyStartTime);
        AskCode = AskOverwrite(SrcData,DestPath,DestAttr,SameName,Rename,((ShellCopy::Flags&FCOPY_LINK)?0:1),Append,RetCode);
        CopyStartTime = clock();

        if (!AskCode)
        {
          _LOGCOPYR(SysLog("return RetCode=%d -> %d if (!AskCode)",RetCode,__LINE__));
          return((COPY_CODES)RetCode);
        }
      }
    }
  }
  else
  {
    if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      _LOGCOPYR(SysLog("return return COPY_SUCCESS -> %d if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)",__LINE__));
      return COPY_SUCCESS;
    }
/*
    {
      char SrcFullName[NM];
      if (ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
      {
        return(COPY_NEXT);
      }
    }
*/
  }

  int NWFS_Attr=(Opt.Nowell.MoveRO && !strcmp(DestFSName,"NWFS"))?TRUE:FALSE;
  {

  _LOGCOPYR(CleverSysLog Clev("while (1)"));
  while (1)
  {
    int CopyCode;
//    int64 SaveCopiedSize=CurCopiedSize;
    int64 SaveTotalSize=TotalCopiedSize;
    if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && Rename)
    {
      int MoveCode=FALSE,AskDelete;

      if ((WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT || !strcmp(DestFSName,"NWFS")) && !Append &&
          DestAttr!=(DWORD)-1 && !SameName &&
          !RenameToShortName) // !!!
      {
        remove(DestPath);
      }

      if (!Append)
      {
        char SrcFullName[NM*2];
        ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName));

        if (NWFS_Attr)
          SetFileAttributes(SrcFullName,SrcData.dwFileAttributes&(~FA_RDONLY));

        /* $ 25.05.2002 IS
           ��������� ��������� RenameToShortName ��� ��������.
        */
        if(RenameToShortName)
          MoveCode=MoveFileThroughTemp(SrcFullName, DestPath);
        else
        {
          if (WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT || !strcmp(DestFSName,"NWFS"))
            MoveCode=MoveFile(SrcFullName,DestPath);
          else
            MoveCode=MoveFileEx(SrcFullName,DestPath,SameName ? MOVEFILE_COPY_ALLOWED:MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
        }

        if (!MoveCode)
        {
          int MoveLastError=GetLastError();
          if (NWFS_Attr)
            SetFileAttributes(SrcFullName,SrcData.dwFileAttributes);

          if(MoveLastError==ERROR_NOT_SAME_DEVICE)
          {
            _LOGCOPYR(SysLog("return COPY_FAILURE -> %d if(MoveLastError==ERROR_NOT_SAME_DEVICE)",__LINE__));
            return COPY_FAILURE;
          }

          SetLastError(MoveLastError);
        }

        if (NWFS_Attr)
          SetFileAttributes(DestPath,SrcData.dwFileAttributes);

        if (ShowTotalCopySize && MoveCode)
        {
          int64 AddSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
          TotalCopiedSize+=AddSize;
          ShowBar(TotalCopiedSize,TotalCopySize,true);
          ShowTitle(FALSE);
        }
        AskDelete=0;
      }
      else
      {
        _LOGCOPYR(SysLog("%d call ShellCopyFile()",__LINE__));
        CopyCode=ShellCopyFile(Src,SrcData,DestPath,(DWORD)-1,Append);
        _LOGCOPYR(SysLog("%d CopyCode=%d",__LINE__,CopyCode));

        switch(CopyCode)
        {
          case COPY_SUCCESS:
            MoveCode=TRUE;
            break;
          case COPY_FAILUREREAD:
          case COPY_FAILURE:
            MoveCode=FALSE;
            break;
          case COPY_CANCEL:
            _LOGCOPYR(SysLog("return COPY_CANCEL -> %d ",__LINE__));
            return COPY_CANCEL;
          case COPY_NEXT:
            _LOGCOPYR(SysLog("return COPY_NEXT -> %d ",__LINE__));
            return COPY_NEXT;
        }
        AskDelete=1;
      }

      if (MoveCode)
      {
        if (DestAttr==(DWORD)-1 || (DestAttr & FILE_ATTRIBUTE_DIRECTORY)==0)
        {
          if (PointToName(DestPath)==DestPath)
            strcpy(RenamedName,DestPath);
          else
            strcpy(CopiedName,PointToName(DestPath));
        }

        if (IsDriveTypeCDROM(SrcDriveType) && Opt.ClearReadOnly &&
            (SrcData.dwFileAttributes & FA_RDONLY))
          ShellSetAttr(DestPath,SrcData.dwFileAttributes & ~FA_RDONLY);

        TotalFiles++;
        if (AskDelete && DeleteAfterMove(Src,SrcData.dwFileAttributes)==COPY_CANCEL)
        {
          _LOGCOPYR(SysLog("return COPY_CANCEL -> %d DeleteAfterMove()==COPY_CANCEL",__LINE__));
          return COPY_CANCEL;
        }

        _LOGCOPYR(SysLog("return COPY_SUCCESS_MOVE -> %d",__LINE__));
        return(COPY_SUCCESS_MOVE);
      }
    }
    else
    {
      _LOGCOPYR(SysLog("%d call ShellCopyFile('%s',%p,'%s',0x%08X,%d)",__LINE__,Src,&SrcData,DestPath,DestAttr,Append));
      CopyCode=ShellCopyFile(Src,SrcData,DestPath,DestAttr,Append);
      _LOGCOPYR(SysLog("%d CopyCode=%d",__LINE__,CopyCode));

      if (CopyCode==COPY_SUCCESS)
      {
        strcpy(CopiedName,PointToName(DestPath));
        if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
        {
          if (IsDriveTypeCDROM(SrcDriveType) && Opt.ClearReadOnly &&
              (SrcData.dwFileAttributes & FA_RDONLY))
            ShellSetAttr(DestPath,SrcData.dwFileAttributes & ~FA_RDONLY);

          if (DestAttr!=(DWORD)-1 && LocalStricmp(CopiedName,DestData.cFileName)==0 &&
              strcmp(CopiedName,DestData.cFileName)!=0)
            MoveFile(DestPath,DestPath);
        }

        TotalFiles++;
        if(DestAttr!=-1 && Append)
          SetFileAttributes(DestPath,DestAttr);

        _LOGCOPYR(SysLog("return COPY_SUCCESS -> %d",__LINE__));
        return COPY_SUCCESS;
      }
      else if (CopyCode==COPY_CANCEL || CopyCode==COPY_NEXT)
      {
        if(DestAttr!=-1 && Append)
          SetFileAttributes(DestPath,DestAttr);
        _LOGCOPYR(SysLog("return CopyCode [%d] -> %d",CopyCode,__LINE__));
        return((COPY_CODES)CopyCode);
      }

      if(DestAttr!=-1 && Append)
        SetFileAttributes(DestPath,DestAttr);
    }
    //????
    if(CopyCode == COPY_FAILUREREAD)
    {
      _LOGCOPYR(SysLog("return COPY_FAILURE -> %d if(CopyCode == COPY_FAILUREREAD)",__LINE__));
      return COPY_FAILURE;
    }
    //????

    char Msg1[2*NM],Msg2[2*NM];
    int MsgMCannot=(ShellCopy::Flags&FCOPY_LINK) ? MCannotLink: (ShellCopy::Flags&FCOPY_MOVE) ? MCannotMove: MCannotCopy;
    InsertQuote(TruncPathStr(strcpy(Msg1,Src),64));
    InsertQuote(TruncPathStr(strcpy(Msg2,DestPath),64));

    {
      int MsgCode;
      /* $ 02.03.2002 KM
        ! ������� ���������� ������.
          ���������� "Skip all".
      */
      if (SkipMode!=-1)
        MsgCode=SkipMode;
      else
      {
        CopyTime+= (clock() - CopyStartTime);
        MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,4,MSG(MError),
                        MSG(MsgMCannot),
                        Msg1,
                        MSG(MCannotCopyTo),
                        Msg2,
                        MSG(MCopyRetry),MSG(MCopySkip),
                        MSG(MCopySkipAll),MSG(MCopyCancel));
        CopyStartTime = clock();
      }

      switch(MsgCode)
      {
        case -1:
        case  1:
          _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
          return COPY_NEXT;
        case  2:
          SkipMode=1;
          _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
          return COPY_NEXT;
        case -2:
        case  3:
          _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
          return COPY_CANCEL;
      }
      /* KM $ */
    }

//    CurCopiedSize=SaveCopiedSize;
    TotalCopiedSize=SaveTotalSize;
    int RetCode, AskCode;
    CopyTime+= (clock() - CopyStartTime);
    AskCode = AskOverwrite(SrcData,DestPath,DestAttr,SameName,Rename,((ShellCopy::Flags&FCOPY_LINK)?0:1),Append,RetCode);
    CopyStartTime = clock();

    if (!AskCode)
    {
      _LOGCOPYR(SysLog("return RetCode=%d -> %d if (!AskCode)",RetCode,__LINE__));
      return((COPY_CODES)RetCode);
    }
  }
  }
}

// �������� ���������� ��������� �� ������
COPY_CODES ShellCopy::CheckStreams(const char *Src,const char *DestPath)
{
#if 0
  int AscStreams=(ShellCopy::Flags&FCOPY_STREAMSKIP)?2:((ShellCopy::Flags&FCOPY_STREAMALL)?0:1);
  if(!Opt.UseSystemCopy && NT && AscStreams)
  {
    int CountStreams=EnumNTFSStreams(Src,NULL,NULL);
    if(CountStreams > 1 ||
       (CountStreams >= 1 && (GetFileAttributes(Src)&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
    {
      if(AscStreams == 2)
      {
        return(COPY_NEXT);
      }

      SetMessageHelp("WarnCopyStream");
      //char SrcFullName[NM];
      //ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName));
      //TruncPathStr(SrcFullName,ScrX-16);
      int MsgCode=Message(MSG_DOWN|MSG_WARNING,5,MSG(MWarning),
              MSG(MCopyStream1),
              MSG(CanCreateHardLinks(DestPath,NULL)?MCopyStream2:MCopyStream3),
              MSG(MCopyStream4),"\1",//SrcFullName,"\1",
              MSG(MCopyResume),MSG(MCopyOverwriteAll),MSG(MCopySkipOvr),MSG(MCopySkipAllOvr),MSG(MCopyCancelOvr));
      switch(MsgCode)
      {
        case 0: break;
        case 1: ShellCopy::Flags|=FCOPY_STREAMALL; break;
        case 2: return(COPY_NEXT);
        case 3: ShellCopy::Flags|=FCOPY_STREAMSKIP; return(COPY_NEXT);
        default:
          return COPY_CANCEL;
      }
    }
  }
#endif
  return COPY_SUCCESS;
}

void ShellCopy::PR_ShellCopyMsg(void)
{
  // // _LOGCOPYR(CleverSysLog clv("PR_ShellCopyMsg"));
  // // _LOGCOPYR(SysLog("2='%s'/0x%08X  3='%s'/0x%08X  Flags=0x%08X",(char*)PreRedrawParam.Param2,PreRedrawParam.Param2,(char*)PreRedrawParam.Param3,PreRedrawParam.Param3,PreRedrawParam.Flags));
  LastShowTime = 0;
  ((ShellCopy*)PreRedrawParam.Param1)->ShellCopyMsg((char*)PreRedrawParam.Param2,(char*)PreRedrawParam.Param3,PreRedrawParam.Flags&(~MSG_KEEPBACKGROUND));
}

void ShellCopy::ShellCopyMsg(const char *Src,const char *Dest,int Flags)
{
  // // _LOGCOPYR(CleverSysLog clv("ShellCopyMsg"));
  char FilesStr[100],BarStr[100],SrcName[NM],DestName[NM];

  //// // _LOGCOPYR(SysLog("[%p] ShellCopy::ShellCopyMsg('%s','%s',%x)",this,Src,Dest,Flags));
  #define BAR_SIZE  40
  static char Bar[BAR_SIZE+2]={0};
  if(!Bar[0])
    memset(Bar,0x0C4,BAR_SIZE);

  strcpy(BarStr,Bar);

  if (ShowTotalCopySize)
  {
    char TotalMsg[100];
    if (*TotalCopySizeText)
      sprintf(TotalMsg," %s: %s ",MSG(MCopyDlgTotal),TotalCopySizeText);
    else
      sprintf(TotalMsg," %s ",MSG(MCopyDlgTotal));
    int TotalLength=strlen(TotalMsg);
    memcpy(BarStr+(strlen(BarStr)-TotalLength+1)/2,TotalMsg,TotalLength);
    *FilesStr=0;
  }
  else
  {
    sprintf(FilesStr,MSG(MCopyProcessed),TotalFiles);
    /* $ 30.01.2001 VVM
        + ��������� ����� ������.
          �������� ��� ������� ����� ��� ����������� ShowTotalIndicator  */
    if ((Src!=NULL) && (ShowCopyTime))
    {
      CopyStartTime = clock();
      CopyTime = 0;
      LastShowTime = 0;
    }
    /* VVM $ */
  }

  if (Src!=NULL)
  {
    sprintf(SrcName,"%-40s",Src);
    TruncPathStr(SrcName,40);
  }
  sprintf(DestName,"%-40s",Dest);
  TruncPathStr(DestName,40);

  SetMessageHelp("CopyFiles");
  if (Src==NULL)
    Message(Flags,0,(ShellCopy::Flags&FCOPY_MOVE) ? MSG(MMoveDlgTitle):
                       MSG(MCopyDlgTitle),
                       "",MSG(MCopyScanning),
                       DestName,"","",BarStr,"");
  else
/* $ 30.01.2001 VVM
    + ���������� ����� �����������,���������� ����� � ������� ��������. */
    if ((ShellCopy::Flags&FCOPY_MOVE))
    {
      if (ShowCopyTime)
        Message(Flags,0,MSG(MMoveDlgTitle),MSG(MCopyMoving),SrcName,MSG(MCopyTo),DestName,"",BarStr,FilesStr,Bar,"");
      else
        Message(Flags,0,MSG(MMoveDlgTitle),MSG(MCopyMoving),SrcName,MSG(MCopyTo),DestName,"",BarStr,FilesStr);
    }
    else
    {
      if (ShowCopyTime)
        Message(Flags,0,MSG(MCopyDlgTitle),MSG(MCopyCopying),SrcName,MSG(MCopyTo),DestName,"",BarStr,FilesStr,Bar,"");
      else
        Message(Flags,0,MSG(MCopyDlgTitle),MSG(MCopyCopying),SrcName,MSG(MCopyTo),DestName,"",BarStr,FilesStr);
    }
/* VVM $ */
  int MessageX1,MessageY1,MessageX2,MessageY2;
  GetMessagePosition(MessageX1,MessageY1,MessageX2,MessageY2);
  BarX=MessageX1+5;
  BarY=MessageY1+6;
  BarLength=MessageX2-MessageX1-9;

  if (Src!=NULL)
  {
    // // _LOGCOPYR(SysLog(" ******************  ShowTotalCopySize=%d",ShowTotalCopySize));
    ShowBar(0,0,false);
    if (ShowTotalCopySize)
    {
      ShowBar(TotalCopiedSize,TotalCopySize,true);
      ShowTitle(FALSE);
    }
  }
  PreRedrawParam.Flags=Flags;
  PreRedrawParam.Param1=this;
  PreRedrawParam.Param2=Src;
  PreRedrawParam.Param3=Dest;
  // // _LOGCOPYR(SysLog("@@ShellCopyMsg 2='%s'/0x%08X  3='%s'/0x%08X  Flags=0x%08X",(char*)PreRedrawParam.Param2,PreRedrawParam.Param2,(char*)PreRedrawParam.Param3,PreRedrawParam.Param3,PreRedrawParam.Flags));
}


int ShellCopy::ShellCopyConvertWildcards(const char *SrcName,char *Dest)
{
  char WildName[2*NM],*CurWildPtr,*DestNamePtr,*SrcNamePtr;
  /* $ 25.05.2002 IS
     ��������� SrcName �� ���������� �����, �.�. ��� ���� ���������� ��������
  */
  char Src[2*NM];
  strncpy(Src,SrcName,sizeof(Src)-1);
  /* IS $ */
  char PartBeforeName[NM],PartAfterFolderName[NM];
  DestNamePtr=PointToName(Dest);
  strcpy(WildName,DestNamePtr);
  if (strchr(WildName,'*')==NULL && strchr(WildName,'?')==NULL)
    return(FALSE);

  if (SelectedFolderNameLength!=0)
  {
    strcpy(PartAfterFolderName,Src+SelectedFolderNameLength);
    Src[SelectedFolderNameLength]=0;
  }

  SrcNamePtr=PointToName(Src);

  int BeforeNameLength=DestNamePtr==Dest ? SrcNamePtr-Src:0;
  strncpy(PartBeforeName,Src,BeforeNameLength);
  PartBeforeName[BeforeNameLength]=0;

  char *SrcNameDot=strrchr(SrcNamePtr,'.');
  CurWildPtr=WildName;
  while (*CurWildPtr)
    switch(*CurWildPtr)
    {
      case '?':
        CurWildPtr++;
        if (*SrcNamePtr)
          *(DestNamePtr++)=*(SrcNamePtr++);
        break;
      case '*':
        CurWildPtr++;
        while (*SrcNamePtr)
        {
          if (*CurWildPtr=='.' && SrcNameDot!=NULL && strchr(CurWildPtr+1,'.')==NULL)
          {
            if (SrcNamePtr==SrcNameDot)
              break;
          }
          else
            if (*SrcNamePtr==*CurWildPtr)
              break;
          *(DestNamePtr++)=*(SrcNamePtr++);
        }
        break;
      case '.':
        CurWildPtr++;
        *(DestNamePtr++)='.';
        if (strpbrk(CurWildPtr,"*?")!=NULL)
          while (*SrcNamePtr)
            if (*(SrcNamePtr++)=='.')
              break;
        break;
      default:
        *(DestNamePtr++)=*(CurWildPtr++);
        if (*SrcNamePtr && *SrcNamePtr!='.')
          SrcNamePtr++;
        break;
    }

  *DestNamePtr=0;
  if (DestNamePtr!=Dest && *(DestNamePtr-1)=='.')
    *(DestNamePtr-1)=0;
  if (*PartBeforeName)
  {
    strcat(PartBeforeName,Dest);
    strcpy(Dest,PartBeforeName);
  }
  if (SelectedFolderNameLength!=0)
    strcat(Src,PartAfterFolderName);
  return(TRUE);
}

int ShellCopy::DeleteAfterMove(const char *Name,int Attr)
{
  if (Attr & FA_RDONLY)
  {
    int MsgCode;
    CopyTime+= (clock() - CopyStartTime);
    if (ReadOnlyDelMode!=-1)
      MsgCode=ReadOnlyDelMode;
    else
      MsgCode=Message(MSG_DOWN|MSG_WARNING,5,MSG(MWarning),
              MSG(MCopyFileRO),Name,MSG(MCopyAskDelete),
              MSG(MCopyDeleteRO),MSG(MCopyDeleteAllRO),
              MSG(MCopySkipRO),MSG(MCopySkipAllRO),MSG(MCopyCancelRO));
    CopyStartTime = clock();
    switch(MsgCode)
    {
      case 1:
        ReadOnlyDelMode=1;
        break;
      case 2:
        return(COPY_NEXT);
      case 3:
        ReadOnlyDelMode=3;
        return(COPY_NEXT);
      case -1:
      case -2:
      case 4:
        return(COPY_CANCEL);
    }
    SetFileAttributes(Name,FILE_ATTRIBUTE_NORMAL);
  }
  while (remove(Name)!=0)
  {
    int MsgCode;
    CopyTime+= (clock() - CopyStartTime);
    MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                    MSG(MCannotDeleteFile),Name,MSG(MDeleteRetry),
                    MSG(MDeleteSkip),MSG(MDeleteCancel));
    CopyStartTime = clock();
    if (MsgCode==1 || MsgCode==-1)
      break;
    if (MsgCode==2 || MsgCode==-2)
      return(COPY_CANCEL);
  }
  return(COPY_SUCCESS);
}


int ShellCopy::ShellCopyFile(const char *SrcName,const WIN32_FIND_DATA &SrcData,
                             const char *DestName,DWORD DestAttr,int Append)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::ShellCopyFile()"));
  _LOGCOPYR(SysLog("Params: SrcName='%s', DestName='%s', DestAttr=0x%08X",SrcName, DestName,DestAttr));
  OrigScrX=ScrX;
  OrigScrY=ScrY;

  SetPreRedrawFunc(ShellCopy::PR_ShellCopyMsg);
  if ((ShellCopy::Flags&FCOPY_LINK))
  {
    FAR_DeleteFile(DestName);
    return(MkLink(SrcName,DestName) ? COPY_SUCCESS:COPY_FAILURE);
  }

  if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && Opt.UseSystemCopy && !Append)
  {
    if (!Opt.CopyOpened)
    {
      HANDLE SrcHandle=FAR_CreateFile(SrcName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
      if (SrcHandle==INVALID_HANDLE_VALUE)
      {
        _LOGCOPYR(SysLog("return COPY_FAILURE -> %d if (SrcHandle==INVALID_HANDLE_VALUE)",__LINE__));
        return COPY_FAILURE;
      }
      CloseHandle(SrcHandle);
    }
    _LOGCOPYR(SysLog("call ShellSystemCopy('%s','%s',%p)",SrcName,DestName,SrcData));
    return(ShellSystemCopy(SrcName,DestName,SrcData));
  }

  SECURITY_ATTRIBUTES sa;
  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(SrcName,sa))
  {
    _LOGCOPYR(SysLog("return COPY_CANCEL -> %d GetSecurity() == 0",__LINE__));
    return COPY_CANCEL;
  }
  int OpenMode=FILE_SHARE_READ;
  if (Opt.CopyOpened)
    OpenMode|=FILE_SHARE_WRITE;
  HANDLE SrcHandle=FAR_CreateFile(SrcName,GENERIC_READ,OpenMode,NULL,OPEN_EXISTING,0,NULL);
  if (SrcHandle==INVALID_HANDLE_VALUE)
  {
    _LOGCOPYR(SysLog("return COPY_FAILURE -> %d if (SrcHandle==INVALID_HANDLE_VALUE)",__LINE__));
    return COPY_FAILURE;
  }

  HANDLE DestHandle;
  DWORD AppendPos=0;
  LONG  AppendPosHigh=0;

  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    if (DestAttr!=(DWORD)-1 && !Append)
      remove(DestName);
    DestHandle=FAR_CreateFile(DestName,GENERIC_WRITE,FILE_SHARE_READ,
                          (ShellCopy::Flags&FCOPY_COPYSECURITY) ? &sa:NULL,
                          Append ? OPEN_EXISTING:CREATE_ALWAYS,
                          SrcData.dwFileAttributes,NULL);
    if (DestHandle==INVALID_HANDLE_VALUE)
    {
      DWORD LastError=GetLastError();
      CloseHandle(SrcHandle);
      SetLastError(LastError);
      _LOGCOPYR(SysLog("return COPY_FAILURE -> %d CreateFile=-1, LastError=%d (0x%08X)",__LINE__,LastError,LastError));
      return COPY_FAILURE;
    }

    if (Append && (AppendPos=SetFilePointer(DestHandle,0,&AppendPosHigh,FILE_END))==0xFFFFFFFF)
    {
      DWORD LastError=GetLastError();
      CloseHandle(SrcHandle);
      CloseHandle(DestHandle);
      SetLastError(LastError);
      _LOGCOPYR(SysLog("return COPY_FAILURE -> %d SetFilePointer() == -1, LastError=%d (0x%08X)",__LINE__,LastError,LastError));
      return COPY_FAILURE;
    }
  }

//  int64 WrittenSize(0,0);
  int   AbortOp = FALSE;
  UINT  OldErrMode=SetErrorMode(SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);
  int64 FileSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);

  while (1)
  {
    BOOL IsChangeConsole=OrigScrX != ScrX || OrigScrY != ScrY;
    if (CheckForEscSilent())
    {
      CopyTime+= (clock() - CopyStartTime);
      AbortOp = ConfirmAbortOp();
      IsChangeConsole=TRUE; // !!! ������ ���; ��� ����, ����� ��������� �����
      CopyStartTime = clock();
    }
    if(IsChangeConsole)
    {
      ShellCopy::PR_ShellCopyMsg();
      OrigScrX=ScrX;
      OrigScrY=ScrY;
    }

    if (AbortOp)
    {
      CloseHandle(SrcHandle);
      if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
      {
        if (Append)
        {
          SetFilePointer(DestHandle,AppendPos,&AppendPosHigh,FILE_BEGIN);
          SetEndOfFile(DestHandle);
        }
        CloseHandle(DestHandle);
        if (!Append)
        {
          SetFileAttributes(DestName,FILE_ATTRIBUTE_NORMAL);
          FAR_DeleteFile(DestName);
        }
      }
      _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
      SetErrorMode(OldErrMode);
      return COPY_CANCEL;
    }
    DWORD BytesRead,BytesWritten;

    /* $ 23.10.2000 VVM
       + ������������ ����� ����������� */

    /* $ 25.04.2003 VVM
       - ������� ���� ��� ����� */
//    if (CopyBufSize < CopyBufferSize)
//      StartTime=clock();
    while (!ReadFile(SrcHandle,CopyBuffer,CopyBufSize,&BytesRead,NULL))
    {
      CopyTime+= (clock() - CopyStartTime);
      int MsgCode = Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                            MSG(MCopyReadError),SrcName,
                            MSG(MRetry),MSG(MCancel));
      ShellCopy::PR_ShellCopyMsg();
      CopyStartTime = clock();
      if (MsgCode==0)
        continue;
      DWORD LastError=GetLastError();
      CloseHandle(SrcHandle);
      if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
      {
        if (Append)
        {
          SetFilePointer(DestHandle,AppendPos,&AppendPosHigh,FILE_BEGIN);
          SetEndOfFile(DestHandle);
        }
        CloseHandle(DestHandle);
        if (!Append)
        {
          SetFileAttributes(DestName,FILE_ATTRIBUTE_NORMAL);
          FAR_DeleteFile(DestName);
        }
      }
      ShowBar(0,0,false);
      ShowTitle(FALSE);
      SetErrorMode(OldErrMode);
      SetLastError(LastError);
      // return COPY_FAILUREREAD;
      _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
      CurCopiedSize = 0; // �������� ������� ��������
      return COPY_FAILURE;
    }
    if (BytesRead==0)
      break;

    if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
    {
      while (!WriteFile(DestHandle,CopyBuffer,BytesRead,&BytesWritten,NULL))
      {
        DWORD LastError=GetLastError();
        int Split=FALSE,SplitCancelled=FALSE,SplitSkipped=FALSE;
        if ((LastError==ERROR_DISK_FULL || LastError==ERROR_HANDLE_DISK_FULL) &&
            DestName[0]!=0 && DestName[1]==':')
        {
          char DriveRoot[NM];
          GetPathRoot(DestName,DriveRoot);

          DWORD SectorsPerCluster,BytesPerSector,FreeClusters,Clusters;
          if (GetDiskFreeSpace(DriveRoot,&SectorsPerCluster,&BytesPerSector,
                               &FreeClusters,&Clusters))
          {
            DWORD FreeSize=SectorsPerCluster*BytesPerSector*FreeClusters;
            if (FreeSize<BytesRead &&
                WriteFile(DestHandle,CopyBuffer,FreeSize,&BytesWritten,NULL) &&
                SetFilePointer(SrcHandle,FreeSize-BytesRead,NULL,FILE_CURRENT)!=0xFFFFFFFF)
            {
              CloseHandle(DestHandle);
              SetMessageHelp("CopyFiles");
              CopyTime+= (clock() - CopyStartTime);
              int MsgCode=Message(MSG_DOWN|MSG_WARNING,4,MSG(MError),
                                  MSG(MErrorInsufficientDiskSpace),DestName,
                                  MSG(MSplit),MSG(MSkip),MSG(MRetry),MSG(MCancel));
              ShellCopy::PR_ShellCopyMsg();
              CopyStartTime = clock();
              if (MsgCode==2)
              {
                CloseHandle(SrcHandle);
                if (!Append)
                {
                  SetFileAttributes(DestName,FILE_ATTRIBUTE_NORMAL);
                  FAR_DeleteFile(DestName);
                }
                _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
                SetErrorMode(OldErrMode);
                return COPY_FAILURE;
              }
              if (MsgCode==0)
              {
                Split=TRUE;
                while (1)
                {
                  if (GetDiskFreeSpace(DriveRoot,&SectorsPerCluster,&BytesPerSector,&FreeClusters,&Clusters))
                    if (SectorsPerCluster*BytesPerSector*FreeClusters==0)
                    {
                      CopyTime+= (clock() - CopyStartTime);
                      int MsgCode = Message(MSG_DOWN|MSG_WARNING,2,MSG(MWarning),
                                            MSG(MCopyErrorDiskFull),DestName,
                                            MSG(MRetry),MSG(MCancel));
                      ShellCopy::PR_ShellCopyMsg();
                      CopyStartTime = clock();
                      if (MsgCode!=0)
                      {
                        Split=FALSE;
                        SplitCancelled=TRUE;
                      }
                      else
                        continue;
                    }
                  break;
                }
              }
              if (MsgCode==1)
                SplitSkipped=TRUE;
              if (MsgCode==-1 || MsgCode==3)
                SplitCancelled=TRUE;
            }
          }
        }
        if (Split)
        {
          int RetCode, AskCode;
          CopyTime+= (clock() - CopyStartTime);
          AskCode = AskOverwrite(SrcData,DestName,0xFFFFFFFF,FALSE,((ShellCopy::Flags&FCOPY_MOVE)?TRUE:FALSE),((ShellCopy::Flags&FCOPY_LINK)?0:1),Append,RetCode);
          CopyStartTime = clock();
          if (!AskCode)
          {
            CloseHandle(SrcHandle);
            SetErrorMode(OldErrMode);
            _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
            return(COPY_CANCEL);
          }
          char DestDir[NM],*ChPtr;
          strcpy(DestDir,DestName);
          if ((ChPtr=strrchr(DestDir,'\\'))!=NULL)
          {
            *ChPtr=0;
            CreatePath(DestDir);
          }
          DestHandle=FAR_CreateFile(DestName,GENERIC_WRITE,FILE_SHARE_READ,NULL,
                                Append ? OPEN_EXISTING:CREATE_ALWAYS,
                                SrcData.dwFileAttributes,NULL);

          if (DestHandle==INVALID_HANDLE_VALUE ||
              Append && SetFilePointer(DestHandle,0,NULL,FILE_END)==0xFFFFFFFF)
          {
            DWORD LastError=GetLastError();
            CloseHandle(SrcHandle);
            CloseHandle(DestHandle);
            SetErrorMode(OldErrMode);
            SetLastError(LastError);
            _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
            return COPY_FAILURE;
          }
        }
        else
        {
          CopyTime+= (clock() - CopyStartTime);
          if (!SplitCancelled && !SplitSkipped &&
              Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
              MSG(MCopyWriteError),DestName,MSG(MRetry),MSG(MCancel))==0)
          {
            CopyStartTime = clock();
            continue;
          }
          else
            CopyStartTime = clock();
          CloseHandle(SrcHandle);
          if (Append)
          {
            SetFilePointer(DestHandle,AppendPos,&AppendPosHigh,FILE_BEGIN);
            SetEndOfFile(DestHandle);
          }
          CloseHandle(DestHandle);
          if (!Append)
          {
            SetFileAttributes(DestName,FILE_ATTRIBUTE_NORMAL);
            FAR_DeleteFile(DestName);
          }
          ShowBar(0,0,false);
          ShowTitle(FALSE);
          SetErrorMode(OldErrMode);
          SetLastError(LastError);
          if (SplitSkipped)
          {
            _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
            return COPY_NEXT;
          }
          _LOGCOPYR(SysLog("return (%d ? COPY_CANCEL:COPY_FAILURE) -> %d",SplitCancelled,__LINE__));
          return(SplitCancelled ? COPY_CANCEL:COPY_FAILURE);
        }
        break;
      }
    }
    else
    {
      BytesWritten=BytesRead; // �� ������� ���������� ���������� ���������� ����
    }

//    if ((CopyBufSize < CopyBufferSize) && (BytesWritten==CopyBufSize))
//   {
//      StopTime=clock();
//      if ((StopTime - StartTime) < 250)
//      {
//        CopyBufSize*=2;
//        if (CopyBufSize > CopyBufferSize)
//          CopyBufSize=CopyBufferSize;
//      }
//    } /* if */
    /* VVM $ */
    /* VVM $ */

    CurCopiedSize+=BytesWritten;
    if (ShowTotalCopySize)
      TotalCopiedSize+=BytesWritten;

    /* $ 14.09.2002 VVM
      + ���������� �������� �� ���� 5 ��� � ������� */
    if ((CurCopiedSize == FileSize) || (clock() - LastShowTime > COPY_TIMEOUT))
    {
      ShowBar(CurCopiedSize,FileSize,false);
      if (ShowTotalCopySize)
      {
        ShowBar(TotalCopiedSize,TotalCopySize,true);
        ShowTitle(FALSE);
      }
    }
    /* VVM $ */
  } /* while */
  SetErrorMode(OldErrMode);

  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    SetFileTime(DestHandle,NULL,NULL,&SrcData.ftLastWriteTime);
    CloseHandle(SrcHandle);
    CloseHandle(DestHandle);

    // ����� ������� Compressed???
    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS &&
        (SrcData.dwFileAttributes & (FA_HIDDEN|FA_SYSTEM|FA_RDONLY)))
      ShellSetAttr(DestName,SrcData.dwFileAttributes);
  }
  else
    CloseHandle(SrcHandle);

  _LOGCOPYR(SysLog("return COPY_SUCCESS -> %d",__LINE__));
  return COPY_SUCCESS;
}

/* $ 30.01.2001 VVM
    + ������� ������ � ����� */
static void GetTimeText(int Time, char *TimeText)
{
  int Sec = Time;
  int Min = Sec/60;
  Sec-=(Min * 60);
  /*$ 17.05.2001 SKV
    ��. � ���� 24 ������??? :)
    int Hour = Min/24;
    Min-=(Hour*24);
  */
  int Hour = Min/60;
  Min-=(Hour*60);
  /* SKV$*/
  sprintf(TimeText,"%02d:%02d:%02d",Hour,Min,Sec);
}
/* VVM $ */

/* $ 30.04.2003 VVM
  + ������� ���������� TRUE, ���� ���-�� ����������, ����� FALSE */
int ShellCopy::ShowBar(int64 WrittenSize,int64 TotalSize,bool TotalBar)
{
  // // _LOGCOPYR(CleverSysLog clv("ShellCopy::ShowBar"));
  // // _LOGCOPYR(SysLog("WrittenSize=%Ld ,TotalSize=%Ld, TotalBar=%d",WrittenSize,TotalSize,TotalBar));
  if (!ShowTotalCopySize || TotalBar)
    LastShowTime = clock();
/* $ 30.01.2001 VVM
    + ��������� ������� */
  int64 OldWrittenSize = WrittenSize;
  int64 OldTotalSize = TotalSize;
/* VVM $ */
  WrittenSize=WrittenSize>>8;
  TotalSize=TotalSize>>8;

  int Length;
  if (WrittenSize.PLow()>TotalSize.PLow())
    WrittenSize.PLow()=TotalSize.PLow();
  if (TotalSize.PLow()==0)
    Length=BarLength;
  else
    if (TotalSize.PLow()<1000000)
      Length=WrittenSize.PLow()*BarLength/TotalSize.PLow();
    else
      Length=(WrittenSize.PLow()/100)*BarLength/(TotalSize.PLow()/100);
  char ProgressBar[100];
  memset(ProgressBar,0x0B0,BarLength);
  ProgressBar[BarLength]=0;
  if (TotalSize.PLow()!=0)
    memset(ProgressBar,0x0DB,Length);
  SetColor(COL_DIALOGTEXT);
  GotoXY(BarX,BarY+(TotalBar ? 2:0));
  Text(ProgressBar);
/* $ 30.01.2001 VVM
    + ���������� ����� �����������,���������� ����� � ������� ��������. */
  // // _LOGCOPYR(SysLog("!!!!!!!!!!!!!! ShowCopyTime=%d ,ShowTotalCopySize=%d, TotalBar=%d",ShowCopyTime,ShowTotalCopySize,TotalBar));
  if (ShowCopyTime && (!ShowTotalCopySize || TotalBar))
  {
//    CopyTime+= (clock() - CopyStartTime);
//    CopyStartTime = clock();
    int WorkTime = (CopyTime + (clock() - CopyStartTime))/1000;
    int64 SizeLeft = OldTotalSize - OldWrittenSize;
    if (SizeLeft < 0)
      SizeLeft = 0;

    int TimeLeft;
    char TimeStr[100];
    char c[2];
    c[1]=0;

    if (OldTotalSize == 0 || WorkTime == 0)
      sprintf(TimeStr,MSG(MCopyTimeInfo), " ", " ", 0, " ");
    else
    {
      if (TotalBar)
        OldWrittenSize = OldWrittenSize - TotalSkippedSize;
      int CPS = (OldWrittenSize/WorkTime).PLow();
      TimeLeft = (CPS)?(SizeLeft/CPS).PLow():0;
      c[0]=' ';
      if (CPS > 99999) {
        c[0]='K';
        CPS = CPS/1024;
      }
      if (CPS > 99999) {
        c[0]='M';
        CPS = CPS/1024;
      }
      /* $ 06.03.2001 SVS
         � � ���� � ����� ���� :-)
      */
      if (CPS > 99999) {
        c[0]='G';
        CPS = CPS/1024;
      }
      /* SVS $ */
      char WorkTimeStr[12];
      char TimeLeftStr[12];
      GetTimeText(WorkTime, WorkTimeStr);
      GetTimeText(TimeLeft, TimeLeftStr);
      sprintf(TimeStr,MSG(MCopyTimeInfo), WorkTimeStr, TimeLeftStr, CPS, c);
    }
    GotoXY(BarX,BarY+4);
    Text(TimeStr);
  }
  return (TRUE);
/* VVM $ */
}


void ShellCopy::SetDestDizPath(const char *DestPath)
{
  if (!(ShellCopy::Flags&FCOPY_DIZREAD))
  {
    strcpy(DestDizPath,DestPath);
    char *ChPtr=PointToName(DestDizPath);
    *(ChPtr--)=0;
    if (*DestDizPath==0)
      strcpy(DestDizPath,".");
    if (ChPtr>DestDizPath && *ChPtr!=':' && *(ChPtr-1)!=':')
      *ChPtr=0;
    if (Opt.Diz.UpdateMode==DIZ_UPDATE_IF_DISPLAYED && !SrcPanel->IsDizDisplayed() ||
        Opt.Diz.UpdateMode==DIZ_NOT_UPDATE)
      *DestDizPath=0;
    if (*DestDizPath)
      DestDiz.Read(DestDizPath);
    ShellCopy::Flags|=FCOPY_DIZREAD;
  }
}


int ShellCopy::AskOverwrite(const WIN32_FIND_DATA &SrcData,
               const char *DestName, DWORD DestAttr,
               int SameName,int Rename,int AskAppend,
               int &Append,int &RetCode)
{
  HANDLE FindHandle;
  WIN32_FIND_DATA DestData;
  int DestDataFilled=FALSE;

  int MsgCode;

  Append=FALSE;

  if((ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    RetCode=COPY_NEXT;
    return TRUE;
  }

  if (DestAttr==0xFFFFFFFF)
    if ((DestAttr=GetFileAttributes(DestName))==0xFFFFFFFF)
      return(TRUE);

  if (DestAttr & FILE_ATTRIBUTE_DIRECTORY)
    return(TRUE);

  char TruncDestName[NM];
  strcpy(TruncDestName,DestName);
  TruncPathStr(TruncDestName,ScrX-16);

  if (OvrMode!=-1)
    MsgCode=OvrMode;
  else
  {
    int Type;
    if (!Opt.Confirm.Copy && !Rename || !Opt.Confirm.Move && Rename ||
        SameName || (Type=GetFileTypeByName(DestName))==FILE_TYPE_CHAR ||
        Type==FILE_TYPE_PIPE || (ShellCopy::Flags&FCOPY_OVERWRITENEXT))
      MsgCode=1;
    else
    {
      memset(&DestData,0,sizeof(DestData));
      if ((FindHandle=FindFirstFile(DestName,&DestData))!=INVALID_HANDLE_VALUE)
        FindClose(FindHandle);
      DestDataFilled=TRUE;
      /* $ 04.08.2000 SVS
         ����� "Only newer file(s)"
      */
      if((ShellCopy::Flags&FCOPY_ONLYNEWERFILES))
      {
        // ������� �����
        __int64 RetCompare=*(__int64*)&DestData.ftLastWriteTime - *(__int64*)&SrcData.ftLastWriteTime;
        if(RetCompare < 0)
          MsgCode=0;
        else
          MsgCode=2;
      }
      else
      {
        char SrcFileStr[512],DestFileStr[512];
        int64 SrcSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
        char SrcSizeText[20];
        SrcSize.itoa(SrcSizeText);
        int64 DestSize(DestData.nFileSizeHigh,DestData.nFileSizeLow);
        char DestSizeText[20];
        DestSize.itoa(DestSizeText);

        char DateText[20],TimeText[20];
        ConvertDate(SrcData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(SrcFileStr,"%-17s %11.11s %s %s",MSG(MCopySource),SrcSizeText,DateText,TimeText);
        ConvertDate(DestData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(DestFileStr,"%-17s %11.11s %s %s",MSG(MCopyDest),DestSizeText,DateText,TimeText);

        SetMessageHelp("CopyFiles");
        MsgCode=Message(MSG_DOWN|MSG_WARNING,AskAppend ? 6:5,MSG(MWarning),
                MSG(MCopyFileExist),TruncDestName,"\x1",SrcFileStr,DestFileStr,
                "\x1",MSG(MCopyOverwrite),MSG(MCopyOverwriteAll),
                MSG(MCopySkipOvr),MSG(MCopySkipAllOvr),
                AskAppend ? (AskAppend==1 ? MSG(MCopyAppend):MSG(MCopyResume)):MSG(MCopyCancelOvr),
                AskAppend ? MSG(MCopyCancelOvr):NULL);
        if (!AskAppend && MsgCode==4)
          MsgCode=5;
      }
      /* SVS $*/
    }
  }

  switch(MsgCode)
  {
    case 1:
      OvrMode=1;
      break;
    case 2:
      RetCode=COPY_NEXT;
      return(FALSE);
    case 3:
      OvrMode=3;
      RetCode=COPY_NEXT;
      return(FALSE);
    case 4:
      Append=TRUE;
      break;
    case -1:
    case -2:
    case 5:
      RetCode=COPY_CANCEL;
      return(FALSE);
  }
  if ((DestAttr & FA_RDONLY) && !(ShellCopy::Flags&FCOPY_OVERWRITENEXT))
  {
    int MsgCode;
    if (SameName)
      MsgCode=0;
    else
      if (ReadOnlyOvrMode!=-1)
        MsgCode=ReadOnlyOvrMode;
      else
      {
        if (!DestDataFilled)
        {
          memset(&DestData,0,sizeof(DestData));
          if ((FindHandle=FindFirstFile(DestName,&DestData))!=INVALID_HANDLE_VALUE)
            FindClose(FindHandle);
        }
        char SrcFileStr[512],DestFileStr[512],DateText[20],TimeText[20];

        int64 SrcSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
        char SrcSizeText[20];
        SrcSize.itoa(SrcSizeText);
        int64 DestSize(DestData.nFileSizeHigh,DestData.nFileSizeLow);
        char DestSizeText[20];
        DestSize.itoa(DestSizeText);

        ConvertDate(SrcData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(SrcFileStr,"%-17s %11.11s %s %s",MSG(MCopySource),SrcSizeText,DateText,TimeText);
        ConvertDate(DestData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(DestFileStr,"%-17s %11.11s %s %s",MSG(MCopyDest),DestSizeText,DateText,TimeText);

        SetMessageHelp("CopyFiles");
        MsgCode=Message(MSG_DOWN|MSG_WARNING,AskAppend ? 6:5,MSG(MWarning),
                MSG(MCopyFileRO),TruncDestName,"\x1",SrcFileStr,DestFileStr,
                "\x1",MSG(MCopyOverwrite),MSG(MCopyOverwriteAll),
                MSG(MCopySkipOvr),MSG(MCopySkipAllOvr),
                AskAppend ? MSG(MCopyAppend):MSG(MCopyCancelOvr),
                AskAppend ? MSG(MCopyCancelOvr):NULL);
        if (!AskAppend && MsgCode==4)
          MsgCode=5;
      }
    switch(MsgCode)
    {
      case 1:
        ReadOnlyOvrMode=1;
        break;
      case 2:
        RetCode=COPY_NEXT;
        return(FALSE);
      case 3:
        ReadOnlyOvrMode=3;
        RetCode=COPY_NEXT;
        return(FALSE);
      case 4:
        ReadOnlyOvrMode=1;
        Append=TRUE;
        break;
      case -1:
      case -2:
      case 5:
        RetCode=COPY_CANCEL;
        return(FALSE);
    }
  }
  if (!SameName && (DestAttr & (FA_RDONLY|FA_HIDDEN|FA_SYSTEM)))
    SetFileAttributes(DestName,FILE_ATTRIBUTE_NORMAL);
  return(TRUE);
}


int ShellCopy::GetSecurity(const char *FileName,SECURITY_ATTRIBUTES &sa)
{
  char AnsiName[NM];
  SECURITY_INFORMATION si=DACL_SECURITY_INFORMATION;
  SECURITY_DESCRIPTOR *sd=(SECURITY_DESCRIPTOR *)sddata;
  DWORD Needed;
  SetFileApisTo(APIS2ANSI);
  FAR_OemToChar(FileName,AnsiName);
  BOOL RetSec=GetFileSecurity(AnsiName,si,sd,sizeof(sddata),&Needed);
  SetFileApisTo(APIS2OEM);
  if (!RetSec)
  {
    sd=NULL;
    int LastError=GetLastError();
    if (LastError!=ERROR_SUCCESS && LastError!=ERROR_FILE_NOT_FOUND &&
        LastError!=ERROR_CALL_NOT_IMPLEMENTED &&
        Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                MSG(MCannotGetSecurity),FileName,MSG(MOk),MSG(MCancel))==1)
      return(FALSE);
  }
  sa.nLength=sizeof(SECURITY_ATTRIBUTES);
  sa.lpSecurityDescriptor=sd;
  sa.bInheritHandle=FALSE;
  return(TRUE);
}


int ShellCopy::SetSecurity(const char *FileName,const SECURITY_ATTRIBUTES &sa)
{
  char AnsiName[NM];
  SECURITY_INFORMATION si=DACL_SECURITY_INFORMATION;
  SetFileApisTo(APIS2ANSI);
  FAR_OemToChar(FileName,AnsiName);
  BOOL RetSec=SetFileSecurity(AnsiName,si,sa.lpSecurityDescriptor);
  SetFileApisTo(APIS2OEM);
  if (!RetSec)
  {
    int LastError=GetLastError();
    if (LastError!=ERROR_SUCCESS && LastError!=ERROR_FILE_NOT_FOUND &&
        LastError!=ERROR_CALL_NOT_IMPLEMENTED &&
        Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                MSG(MCannotSetSecurity),FileName,MSG(MOk),MSG(MCancel))==1)
      return(FALSE);
  }
  return(TRUE);
}


typedef BOOL (WINAPI *COPYFILEEX)(LPCTSTR lpExistingFileName,
            LPCTSTR lpNewFileName,void *lpProgressRoutine,
            LPVOID lpData,LPBOOL pbCancel,DWORD dwCopyFlags);

int ShellCopy::ShellSystemCopy(const char *SrcName,const char *DestName,const WIN32_FIND_DATA &SrcData)
{
  static COPYFILEEX pCopyFileEx=NULL;
  static int LoadAttempt=FALSE;
  int Res;

  if (!LoadAttempt && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
  {
    HMODULE hKernel=GetModuleHandle("KERNEL32.DLL");
    if (hKernel)
      pCopyFileEx=(COPYFILEEX)GetProcAddress(hKernel,"CopyFileExA");
    LoadAttempt=TRUE;
  }


  SECURITY_ATTRIBUTES sa;
  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(SrcName,sa))
    return(COPY_CANCEL);

  //// // _LOGCOPYR(SysLog("[%p] ShellCopy::ShellSystemCopy('%s','%s',..)",this,SrcName,DestName));
  ShellCopyMsg(SrcName,DestName,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);

  if (pCopyFileEx)
  {
    BOOL Cancel=0;
    TotalCopiedSizeEx=TotalCopiedSize;
    if (!pCopyFileEx(SrcName,DestName,CopyProgressRoutine,NULL,&Cancel,0))
      return GetLastError()==ERROR_REQUEST_ABORTED ? COPY_CANCEL:COPY_FAILURE;
  }
  else
  {
    if (ShowTotalCopySize)
    {
      int64 AddSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
      TotalCopiedSize += AddSize;
      CurCopiedSize = AddSize;
      ShowBar(TotalCopiedSize,TotalCopySize,true);
      ShowTitle(FALSE);
    }
    // ����� ��... ����� ���� ������ ���, ��� �� ����� �� �������� ���� CopyFileExA
    if (!CopyFile(SrcName,DestName,FALSE))
      return COPY_FAILURE;
  }

  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !SetSecurity(DestName,sa))
    return(COPY_CANCEL);
  return(COPY_SUCCESS);
}


#define PROGRESS_CONTINUE  0
#define PROGRESS_CANCEL    1
#if defined(__BORLANDC__)
#pragma warn -par
#endif

DWORD WINAPI CopyProgressRoutine(LARGE_INTEGER TotalFileSize,
      LARGE_INTEGER TotalBytesTransferred,LARGE_INTEGER StreamSize,
      LARGE_INTEGER StreamBytesTransferred,DWORD dwStreamNumber,
      DWORD dwCallbackReason,HANDLE hSourceFile,HANDLE hDestinationFile,
      LPVOID lpData)
{
  // // _LOGCOPYR(CleverSysLog clv("CopyProgressRoutine"));
  // // _LOGCOPYR(SysLog("dwStreamNumber=%d",dwStreamNumber));

  int64 TransferredSize(TotalBytesTransferred.u.HighPart,TotalBytesTransferred.u.LowPart);
  int64 TotalSize(TotalFileSize.u.HighPart,TotalFileSize.u.LowPart);

  int AbortOp = FALSE;
  BOOL IsChangeConsole=OrigScrX != ScrX || OrigScrY != ScrY;
  if (CheckForEscSilent())
  {
    CopyTime+= (clock() - CopyStartTime);
    // // _LOGCOPYR(SysLog("2='%s'/0x%08X  3='%s'/0x%08X  Flags=0x%08X",(char*)PreRedrawParam.Param2,PreRedrawParam.Param2,(char*)PreRedrawParam.Param3,PreRedrawParam.Param3,PreRedrawParam.Flags));
    AbortOp = ConfirmAbortOp();
    IsChangeConsole=TRUE; // !!! ������ ���; ��� ����, ����� ��������� �����
    CopyStartTime = clock();
  }

  if(IsChangeConsole)
  {
    // // _LOGCOPYR(SysLog("IsChangeConsole 1"));
    ShellCopy::PR_ShellCopyMsg();
    OrigScrX=ScrX;
    OrigScrY=ScrY;
  }

  CurCopiedSize = TransferredSize;

  if ((CurCopiedSize == TotalSize) || (clock() - LastShowTime > COPY_TIMEOUT))
  {
    ShellCopy::ShowBar(TransferredSize,TotalSize,FALSE);
    if (ShowTotalCopySize && dwStreamNumber==1)
    {
      TotalCopiedSize=TotalCopiedSizeEx+CurCopiedSize;
      ShellCopy::ShowBar(TotalCopiedSize,TotalCopySize,true);
      ShellCopy::ShowTitle(FALSE);
    }
  }
  return(AbortOp ? PROGRESS_CANCEL:PROGRESS_CONTINUE);
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


int ShellCopy::IsSameDisk(const char *SrcPath,const char *DestPath)
{
  char SrcRoot[NM],DestRoot[NM];
  GetPathRoot(SrcPath,SrcRoot);
  GetPathRoot(DestPath,DestRoot);
  if (strpbrk(DestPath,"\\:")==NULL)
    return(TRUE);
  if ((SrcRoot[0]=='\\' && SrcRoot[1]=='\\' || DestRoot[0]=='\\' && DestRoot[1]=='\\') &&
      LocalStricmp(SrcRoot,DestRoot)!=0)
    return(FALSE);
  if (*SrcPath==0 || *DestPath==0 || SrcPath[1]!=':' || DestPath[1]!=':') //????
    return(TRUE);
  if (toupper(DestRoot[0])==toupper(SrcRoot[0]))
    return(TRUE);
  DWORD SrcVolumeNumber=0,DestVolumeNumber=0;
  char SrcVolumeName[NM],DestVolumeName[NM];
  int64 SrcTotalSize,SrcTotalFree,SrcUserFree;
  int64 DestTotalSize,DestTotalFree,DestUserFree;

  if (!GetDiskSize(SrcRoot,&SrcTotalSize,&SrcTotalFree,&SrcUserFree))
    return(FALSE);
  if (!GetDiskSize(DestRoot,&DestTotalSize,&DestTotalFree,&DestUserFree))
    return(FALSE);
  if (!GetVolumeInformation(SrcRoot,SrcVolumeName,sizeof(SrcVolumeName),&SrcVolumeNumber,NULL,NULL,NULL,0))
    return(FALSE);
  if (!GetVolumeInformation(DestRoot,DestVolumeName,sizeof(DestVolumeName),&DestVolumeNumber,NULL,NULL,NULL,0))
    return(FALSE);
  if (SrcVolumeNumber!=0 && SrcVolumeNumber==DestVolumeNumber &&
      strcmp(SrcVolumeName,DestVolumeName)==0 &&
      SrcTotalSize==DestTotalSize)
    return(TRUE);
  return(FALSE);
}


bool ShellCopy::CalcTotalSize()
{
  char SelName[NM],SelShortName[NM];
  int FileAttr;

  TotalCopySize=CurCopiedSize=0;

  ShellCopyMsg(NULL,"",MSG_LEFTALIGN);

  SrcPanel->GetSelName(NULL,FileAttr);
  while (SrcPanel->GetSelName(SelName,FileAttr,SelShortName))
  {
    if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
    {
      {
        unsigned long DirCount,FileCount,ClusterSize;
        int64 FileSize,CompressedSize,RealFileSize;
        ShellCopyMsg(NULL,SelName,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);
        if (!GetDirInfo("",SelName,DirCount,FileCount,FileSize,CompressedSize,
                        RealFileSize,ClusterSize,0xffffffff,FALSE,FALSE,
                        ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS))
        {
          ShowTotalCopySize=false;
          return(false);
        }
        TotalCopySize+=FileSize;
      }
    }
    else
    {
      int64 FileSize;
      if (SrcPanel->GetLastSelectedSize(&FileSize)!=-1)
        TotalCopySize+=FileSize;
    }
  }
  // TODO: ��� ��� ��������, ����� "����� = ����� ������ * ���������� �����"
  TotalCopySize=TotalCopySize*(__int64)CountTarget;

  InsertCommas(TotalCopySize,TotalCopySizeText);
  return(true);
}


void ShellCopy::ShowTitle(int FirstTime)
{
  if (ShowTotalCopySize && !FirstTime)
  {
    int64 CopySize=TotalCopiedSize>>8,TotalSize=TotalCopySize>>8;
    StaticCopyTitle->Set("{%d%%} %s",ToPercent(CopySize.PLow(),TotalSize.PLow()),StaticMove ? MSG(MCopyMovingTitle):MSG(MCopyCopyingTitle));
  }
}


/* $ 25.05.2002 IS
 + ������ �������� � ��������� _��������_ �������, � ���������� ����
   ������������� ��������, �����
   Src="D:\Program Files\filename"
   Dest="D:\PROGRA~1\filename"
   ("D:\PROGRA~1" - �������� ��� ��� "D:\Program Files")
   ���������, ��� ����� ���� ����������, � ������ ���������,
   ��� ��� ������ (������� �� �����, ��� � � ������, � �� ������ ������
   ���� ���� � ��� ��)
 ! ����������� - "���������" ������� �� DeleteEndSlash
 ! ������� ��� ���������������� �� �������� ���� � ������
   ��������� �� ������� �����, ������ ��� ��� ����� ������ ������ ���
   ��������������, � ������� ���������� � ��� ����������� ����. ��� ���
   ������ �������������� �� � ���, � ��� ��, ��� � RenameToShortName.
   ������ ������� ������ 1, ��� ������ ���� src=path\filename,
   dest=path\filename (������ ���������� 2 - �.�. ������ �� ������).
*/
int ShellCopy::CmpFullNames(const char *Src,const char *Dest)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::CmpFullNames()"));
  _LOGCOPYR(SysLog("Params: Src='%s', Dest='%s'",Src, Dest));
  char SrcFullName[1024],DestFullName[1024];
  int I;

  // ������� ������ ���� � ������ ������������� ������
  if (ConvertNameToReal(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
    return 2;
  if (ConvertNameToReal(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
    return 2;

  // ������ ����� �� ����
  for (I=strlen(SrcFullName)-1;I>0 && SrcFullName[I]=='.';I--)
    SrcFullName[I]=0;
  DeleteEndSlash(SrcFullName);
  _LOGCOPYR(SysLog("SrcFullName='%s'",SrcFullName));
  for (I=strlen(DestFullName)-1;I>0 && DestFullName[I]=='.';I--)
    DestFullName[I]=0;
  DeleteEndSlash(DestFullName);
  _LOGCOPYR(SysLog("DestFullName='%s'",DestFullName));

/*
  if (LocalStricmp(SrcFullName,DestFullName)!=0)
    return(0);

  return(strcmp(PointToName(SrcFullName),PointToName(DestFullName))==0 ? 2:1);
*/
  // ��������� �� �������� ����
  if(IsLocalPath(SrcFullName))
  {
    I=RawConvertShortNameToLongName(SrcFullName,SrcFullName,sizeof(SrcFullName));
    _LOGCOPYR(SysLog("RawConvertShortNameToLongName() -> SrcFullName='%s'",SrcFullName));
    if(!I || I>=sizeof(SrcFullName))
      return 2;
  }
  if(IsLocalPath(DestFullName))
  {
    I=RawConvertShortNameToLongName(DestFullName,DestFullName,sizeof(DestFullName));
    _LOGCOPYR(SysLog("RawConvertShortNameToLongName() -> DestFullName='%s'",DestFullName));
    if(!I || I>=sizeof(DestFullName))
      return 2;
  }

  _LOGCOPYR(SysLog("return LocalStricmp(SrcFullName,DestFullName)='%d'",LocalStricmp(SrcFullName,DestFullName)));
  return LocalStricmp(SrcFullName,DestFullName)==0;
}
/* IS $ */

// ����� ��� �������� SymLink ��� ���������.
int ShellCopy::MkSymLink(const char *SelName,const char *Dest,DWORD Flags)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::MkSymLink()"));
  _LOGCOPYR(SysLog("Params: SelName='%s', Dest='%s', Flags=0x%08X",SelName,Dest,Flags));
  if (Flags&(FCOPY_LINK|FCOPY_VOLMOUNT))
  {
    char SrcFullName[NM], DestFullName[NM], SelOnlyName[NM];
    char MsgBuf[NM],MsgBuf2[NM];

    // ������� ���
    strncpy(SelOnlyName,SelName,sizeof(SelOnlyName)-1);
    if(SelOnlyName[strlen(SelOnlyName)-1] == '\\')
      SelOnlyName[strlen(SelOnlyName)-1]=0;
    char *PtrSelName=strrchr(SelOnlyName,'\\');
    if(!PtrSelName)
      PtrSelName=SelOnlyName;
    else
      ++PtrSelName;

    if(SelName[1] == ':' && (SelName[2] == 0 || SelName[2] == '\\' && SelName[3] == 0)) // C: ��� C:/
    {
//      if(Flags&FCOPY_VOLMOUNT)
      {
        strcpy(SrcFullName,SelName);
        AddEndSlash(SrcFullName);
      }
/*
    ��� ����� - �� ����� ����� ���������!
    �.�. ���� � �������� SelName �������� "C:", �� � ���� ����� ����������
    ��������� ���� ����� - � symlink`� �� volmount
*/
      Flags&=~FCOPY_LINK;
      Flags|=FCOPY_VOLMOUNT;
      _LOGCOPYR(SysLog("Flags=0x%08X (transfer SymLink to VolMount)",Flags));
    }
    else
      if (ConvertNameToFull(SelName,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
        return 0;
    _LOGCOPYR(SysLog("Src: ConvertNameToFull('%s','%s')",SelName,SrcFullName));

    if (ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
      return 0;
    _LOGCOPYR(SysLog("Dst: ConvertNameToFull('%s','%s')",Dest,DestFullName));

//    char *EndDestFullName=DestFullName+strlen(DestFullName);
    if(DestFullName[strlen(DestFullName)-1] == '\\')
    {
      if(!(Flags&FCOPY_VOLMOUNT))
      {
        // AddEndSlash(DestFullName);
        strcat(DestFullName,PtrSelName);
      }
      else
      {
        // ���� ������ �� ����� - ����������� ���. ��� "Disk_%c"
        sprintf(DestFullName+strlen(DestFullName),"Disk_%c",*SelName);
      }
    }

    if(Flags&FCOPY_VOLMOUNT)
    {
      AddEndSlash(SrcFullName);
      AddEndSlash(DestFullName);
    }

    int JSAttr=GetFileAttributes(DestFullName);
    _LOGCOPYR(SysLog("%d DestFullName='%s' JSAttr=0x%08X",__LINE__,DestFullName,JSAttr));
    if(JSAttr != -1 && (JSAttr&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY) // ���������� �����?
    {
      if(CheckFolder(DestFullName) == CHKFLD_NOTEMPTY) // � ������?
      {
        _LOGCOPYR(SysLog("CheckFolder('%s') == CHKFLD_NOTEMPTY",DestFullName));
        // �� ������, �� ��� ��, ����� ������� ������� dest\srcname
        AddEndSlash(DestFullName);
        if(Flags&FCOPY_VOLMOUNT)
        {
          char TmpName[NM];
          sprintf(TmpName,MSG(MCopyMountName),*SelName);
          strcat(DestFullName,TmpName);
          AddEndSlash(DestFullName);
        }
        else
          strcat(DestFullName,PtrSelName);

        int JSAttr=GetFileAttributes(DestFullName);
         _LOGCOPYR(SysLog("%d DestFullName='%s' JSAttr=0x%08X",__LINE__,DestFullName,JSAttr));
        if(JSAttr != -1) // � ����� ���� ����???
        {
          _LOGCOPYR(SysLog("Ops! Folder '%s' Exist!",DestFullName));
          if(CheckFolder(DestFullName) == CHKFLD_NOTEMPTY) // � ������?
          {
            _LOGCOPYR(SysLog("%d CheckFolder('%s') == CHKFLD_NOTEMPTY",__LINE__,DestFullName));
            if(!(Flags&FCOPY_NOSHOWMSGLINK))
            {
              if(Flags&FCOPY_VOLMOUNT)
              {
                sprintf(MsgBuf,MSG(MCopyMountVolFailed), SelName);
                sprintf(MsgBuf2,MSG(MCopyMountVolFailed2), DestFullName);
                Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
                   MsgBuf,
                   MsgBuf2,
                   MSG(MCopyFolderNotEmpty),
                   MSG(MOk));
              }
              else
                Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
                      MSG(MCopyCannotCreateJunction),DestFullName,
                      MSG(MCopyFolderNotEmpty),MSG(MOk));
            }
            _LOGCOPYR(SysLog("return 0 -> %d Unequivocally into mortuary",__LINE__));
            return 0; // ���������� � ����
          }
        }
        else // �������.
        {
           _LOGCOPYR(SysLog("%d CreateDirectory('%s')",__LINE__,DestFullName));
          if (CreateDirectory(DestFullName,NULL))
            TreeList::AddTreeName(DestFullName);
          else
            CreatePath(DestFullName);
        }
        if(GetFileAttributes(DestFullName) == -1) // ���, ��� ����� ���� �����.
        {
          if(!(Flags&FCOPY_NOSHOWMSGLINK))
          {
            Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),
                      MSG(MCopyCannotCreateFolder),
                      DestFullName,MSG(MOk));
          }
          _LOGCOPYR(SysLog("return 0 -> %d So, all very much even is bad: GetFileAttributes('%s') == -1",__LINE__,DestFullName));
          return 0;
        }
      }
    }
    else
    {
      _LOGCOPYR(SysLog("Folder '%s' not exist. Create",DestFullName));
      if (CreateDirectory(DestFullName,NULL))
        TreeList::AddTreeName(DestFullName);
      else
        CreatePath(DestFullName);

      if(GetFileAttributes(DestFullName) == -1) // ���. ��� ����� ���� �����.
      {
        if(!(Flags&FCOPY_NOSHOWMSGLINK))
        {
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
                   MSG(MCopyCannotCreateFolder),DestFullName,MSG(MOk));
        }
        _LOGCOPYR(SysLog("return 0 -> %d So, all very much even is bad: GetFileAttributes('%s') == -1",__LINE__,DestFullName));
        return 0;
      }
    }

    _LOGCOPYR(SysLog("('%s','%s')",SrcFullName,DestFullName));
    if(Flags&FCOPY_LINK)
    {
      if(CreateJunctionPoint(SrcFullName,DestFullName))
      {
        _LOGCOPYR(SysLog("Ok: CreateJunctionPoint('%s','%s')",SrcFullName,DestFullName));
        return 1;
      }
      else
      {
        if(!(Flags&FCOPY_NOSHOWMSGLINK))
        {
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
                 MSG(MCopyCannotCreateJunction),DestFullName,MSG(MOk));
        }
        _LOGCOPYR(SysLog("Fail: CreateJunctionPoint('%s','%s')",SrcFullName,DestFullName));
        return 0;
      }
    }
    else if(Flags&FCOPY_VOLMOUNT)
    {
      int ResultVol=CreateVolumeMountPoint(SrcFullName,DestFullName);
      if(!ResultVol)
      {
        _LOGCOPYR(SysLog("Ok: CreateVolumeMountPoint('%s','%s')",SrcFullName,DestFullName));
        return 1;
      }
      else
      {
        if(!(Flags&FCOPY_NOSHOWMSGLINK))
        {
          switch(ResultVol)
          {
            case 1:
              sprintf(MsgBuf,MSG(MCopyRetrVolFailed), SelName);
              break;
            case 2:
              sprintf(MsgBuf,MSG(MCopyMountVolFailed), SelName);
              sprintf(MsgBuf2,MSG(MCopyMountVolFailed2), DestFullName);
              break;
            case 3:
              strcpy(MsgBuf,MSG(MCopyCannotSupportVolMount));
              break;
          }

          if(ResultVol == 2)
            Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
              MsgBuf,
              MsgBuf2,
              MSG(MOk));
          else
            Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
              MSG(MCopyCannotCreateVolMount),
              MsgBuf,
              MSG(MOk));
        }
        _LOGCOPYR(SysLog("Fail: CreateVolumeMountPoint('%s','%s')",SrcFullName,DestFullName));
        return 0;
      }
    }
  }
  _LOGCOPYR(SysLog("return 2 -> %d",__LINE__));
  return 2;
}

/*
  �������� ������ SetFileAttributes() ���
  ����������� ����������� ���������
*/
int ShellCopy::ShellSetAttr(const char *Dest,DWORD Attr)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::ShellSetAttr()"));
  _LOGCOPYR(SysLog("Params: Dest='%s', Attr=0x%08X",Dest,Attr));
  char Root[1024];
  char FSysNameDst[NM];
  DWORD FileSystemFlagsDst;

  ConvertNameToFull(Dest,Root, sizeof(Root));
  GetPathRoot(Root,Root);
  if(GetFileAttributes(Root) == -1) // �������, ����� ������� ����, �� ��� � �������
  { // ... � ���� ������ �������� AS IS
    ConvertNameToFull(Dest,Root, sizeof(Root));
    GetPathRootOne(Root,Root);
    if(GetFileAttributes(Root) == -1)
    {
      _LOGCOPYR(SysLog("return 0 -> %d GetFileAttributes('%s') == -1",__LINE__,Root));
      return FALSE;
    }
  }

  /* 18.06.2002 VVM
    ! ���� ���� �� ������ �������� ���������� � ���� - ���������� ��������� ��������
      � ���� �� ����������� ���� ��� UNC-���� ������-�� ������������ GetVolumeInformation() */
  _LOGCOPYR(SysLog("%d 0x%08X Dest='%s' Root='%s'",__LINE__,Attr,Dest,Root));
  int GetInfoSuccess = GetVolumeInformation(Root,NULL,0,NULL,NULL,&FileSystemFlagsDst,FSysNameDst,sizeof(FSysNameDst));
  if (GetInfoSuccess)
  {
     _LOGCOPYR(SysLog("GetVolumeInformation -> FS='%s' (Flags=0x%08X) %c%c",FSysNameDst,FileSystemFlagsDst,(FileSystemFlagsDst&FS_FILE_COMPRESSION?'C':'.'),(FileSystemFlagsDst&FILE_SUPPORTS_ENCRYPTION?'E':'.')));
     if(!(FileSystemFlagsDst&FS_FILE_COMPRESSION))
       Attr&=~FILE_ATTRIBUTE_COMPRESSED;
     if(!(FileSystemFlagsDst&FILE_SUPPORTS_ENCRYPTION))
       Attr&=~FILE_ATTRIBUTE_ENCRYPTED;
  }
  if (!SetFileAttributes(Dest,Attr))
  {
    _LOGCOPYR(SysLog("return 0 -> %d SetFileAttributes('%s',0x%08X) == 0",__LINE__,Dest,Attr));
    return FALSE;
  }
    // ��� �����������/�������� ���������� FILE_ATTRIBUTE_ENCRYPTED
    // ��� ��������, ���� �� ����
  if (GetInfoSuccess && (FileSystemFlagsDst&FILE_SUPPORTS_ENCRYPTION) &&
     (Attr&(FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_DIRECTORY)) == (FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_DIRECTORY))
    if (!ESetFileEncryption(Dest,1,0))
    {
      _LOGCOPYR(SysLog("return 0 -> %d ESetFileEncryption('%s',1,0) == 0",__LINE__,Dest));
      return FALSE;
    }
  _LOGCOPYR(SysLog("return 1 -> %d",__LINE__));
  return TRUE;
  /* VVM $ */
}

BOOL ShellCopy::MoveFileThroughTemp(const char *Src, const char *Dest)
{
  char Temp[NM];
  BOOL rc = FALSE;
  if(FarMkTempEx(Temp, NULL, FALSE))
  {
    if(MoveFile(Src, Temp))
      rc = MoveFile(Temp, Dest);
  }
  return rc;
}

BOOL ShellCopy::CheckNulOrCon(const char *Src)
{
  // ���������� ������� ����������� � NUL
  /* $ 21.12.2002 IS
     ������� ����������� � nul �������� � �������, ����� ���� ����������:
     - ���������� � "nul\", "\\.\nul\" ��� "con\"
     - ����� "nul", "\\.\nul" ��� "con"
  */
  if(!LocalStricmp (Src,"nul")             ||
     !LocalStrnicmp(Src,"nul\\", 4)        ||
     !LocalStrnicmp(Src,"\\\\.\\nul", 7)   ||
     !LocalStrnicmp(Src,"\\\\.\\nul\\", 8) ||
     !LocalStricmp (Src,"con")             ||
     !LocalStrnicmp(Src,"con\\", 4)
    )
    return TRUE;
  return FALSE;
}

void ShellCopy::CheckUpdatePanel() // ���������� ���� FCOPY_UPDATEPPANEL
{
}
