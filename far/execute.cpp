/*
execute.cpp

"�����������" ��������.

*/

/* Revision: 1.102 27.04.2004 $ */

/*
Modify:
  27.04.2004 SVS
    - BugZ#568 - Confusing message on wrong path in CHDIR in case of forward-slash delimiter
      ����������� �� ���������� �� �������������� - ���� ������ ��� ��� CD
      �� ���������� ����� �������, �� ���� ������ ��� '/' �� '\' � ������
      ������� �� ����� �����������.
      CommandLine::ProcessOSCommands() ��� CD ���������� -1, ���� ��� ��������.
      ���� � ����� ������������.
      ���� ��� �������.
  29.03.2004 SVS
    ! CHCP: ������� �������� LocalUpperInit(), ����� InitLCIDSort(), � �� ��������
  15.03.2004 SVS
    - �� �������� "C:\Program Files\Far\Far.exe" /p"C:\Program Files\Far\plugins"
      � �������� �� ��� ���� (�� Shift-Enter).
      ���� �����������..., �.�. �������� ���������� BugZ#752
  09.03.2004 SVS
    - SD> 2) ���� far'a 102x37
      SD> �������� ������ � ����� ���� �������
      SD> if '%os%'=='Windows_NT' MODE CON: COLS=80 LINES=25
      VC-������ ���������
      BC-������ ����� �� ����� ������
      �������� ������� ���� � ������� CtrlObject->CmdLine->CorrectRealScreenCoord()
  01.03.2004 SVS
    ! ������� FAR_OemTo* � FAR_CharTo* ������ ����������� WinAPI-�������
      (����� �� ������� + ������� ������������ ������ 1 ����)
  09.02.2004 SVS
    - BugZ#993 - ���������� ��������� ����� ���� (���������)
    - BugZ#752 - �������� Shift-Enter ��� UNC �������� � ��������� � ��� (���������)
  15.01.2004 SVS
    - BugZ#993 - ���������� ��������� ����� ����
  08.01.2004 SVS
    + ����� ����� Opt.ExecuteShowErrorMessage � ������� ����� �� �����, � �� � ����������
  05.01.2004 SVS
    ! ��������� ����������� (��� "far.exe/?")
  05.01.2004 SVS
    - BugZ#1007 - �� ���������� ��������� ������ ����� Executor\Type=1
  11.11.2003 SVS
    - ������ � Execute() �� ������ Shift-Enter
  16.10.2003 SVS
    ! ���� � ����������� ���� "command" ��� ���� �������� �������
      ���� - �� �������� ShellExecuteEx().
      ��� �� �������� ���� SEE_MASK_FLAG_NO_UI, ����� ��� ������� ���������
      (���� ��� - ������!).
  09.10.2003 SVS
    ! SetFileApisToANSI() � SetFileApisToOEM() �������� �� SetFileApisTo() � ����������
      APIS2ANSI ��� APIS2OEM - ����� �� �������
  06.10.2003 SVS
    - BugZ#955 - Alt-F9 + Mode CON: Lines=12
      ���������� "��������" ������ � ������� �������... ����� ��������
      � ScrBuf.FillBuf(), ��� ScrY �������� �������� �����
  26.09.2003 VVM
    ! ��� ������ ����� ������� ���� �� ���������� PATH � ������ ����� � ��������� ������
  03.09.2003 SVS
    - bugz#933 - �������� ���� strcpy :-(
    + � CommandLine::ProcessOSCommands() �������� ������������������ �����
      ...������� ��� "SET /P variable=[promptString]" - ��� �� �� ����������...
  02.09.2003 SVS
    ! ��������� ���� �������� ������� CheckFolder()
  31.07.2003 VVM
    ! ������������ ����������� ���� ������������ �����.
  05.06.2003 SVS
    ! SetFarConsoleMode ����� �������� - ����� �� ������������ �����
  06.05.2003 SVS
    ! ��� ����� ������� �������� ��� �� ������������������ ��������� �������
    ! ������� ������ � ����� ����� � 4NT ��� ������ �������
  17.03.2003 SVS
    - BugZ#831 - �������� ��������� ���� ��� ������� �� ��������� ������
  06.03.2003 SVS
    - BugZ#678 - �������� .msi �� Shift-Enter
    ! ���������� _SVS
  25.02.2003 SVS
    ! ������ "������" ��������� ��� ����������� ����� �������, ��� ������
      "gzip foo.bar" ������� �������� ��������, ������ "c:\usr\bin\gzip.EXE"
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
  26.01.2003 IS
    ! FAR_CreateFile - ������� ��� CreateFile, ������� ������������ ������
      �� ������ CreateFile
  17.01.2003 VVM
    ! ���������
  17.12.2002 VVM
    - BugZ#678 - �������� .msi �� Shift-Enter (������ �����!)
  11.12.2002 VVM
    - Opps. ������ ����� �� �������������...
  11.12.2002 VVM
    - ��������� ��� � �������� ���������� �� ������� � ������� ������.
      ��� ��� ����� ���������� ShellExcuteEx()
  04.10.2002 VVM
    ! ��������� ��� ��� ������ ����� �� ������ ����������. ����� ������� ����������
      ������� ��� ����� �� ���� � ����.
  03.10.2002 VVM
    + Default action ����� ��������� ��������� ������� ����� �������.
    + ��� ������ � App paths ����� ������� � ���������� ���������.
  20.09.2002 SKV
    | �������� cd net:server
  03.09.2002 SVS
    - BugZ#606 - �� �������� ���������� ��������� � �����������
      �� ������ �������� �� ������� ":\" ��� "�����������" ������...
  21.08.2002 SVS
    - ����������� 1493 �����. ������� ����� � ������������ ������� ���������
      ��� "open", � ���� ��� ����, ��... ��� ������ ��������� ;-)
  17.08.2002 VVM
    + GetShellAction() - ���� ��� "Default action",  �� ������� ������,
      � ������� ����� ���� "Command"
  12.08.2002 SVS
    + Opt.ExecuteUseAppPath
  08.08.2002 VVM
    ! ������ ����� ������ ���� �� �������� ��������.
  17.07.2002 VVM
    - ���� ������� �� �������� ��������, �� �� ������ "���������" ����.
    ! ������� �� ExcludeCmds ����� ImageSubsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI
    + ���� ImageSubsystem == IMAGE_SUBSYSTEM_UNKNOWN, �� ������� ����������� �������.
    - ���� ������� ��� "\ Enter"
  15.07.2002 VVM
    + ��� �������-��� ������� ������ ������ ����� ShellExecuteEx(). �� ���� SVS
  06.07.2002 VVM
    + ���� �� ����������� ���������� %COMSPEC% - ������������ � �����.
  05.07.2002 SVS
    - ��� ���� ���������� GUI-������ � NE-����������.
      ������� ���� ���� (� ������ Andrzej Novosiolov <andrzej@se.kiev.ua>)
  19.02.2002 SVS
    - BugZ#558 - ShiftEnter �� ���� ������ �����
      �������� ��� �� �� "������ �����" � �������� ������ ����� ShellExecuteEx()
  18.06.2002 SVS
    ! � ����������� ������� "CD" (see CommandLine::ProcessOSCommands)
      ��������� �� CHKFLD_NOTACCESS (��� ��������� ������ ��� �� �����
      CheckFolder). ���� ��� ��� - ���� ������, ����� ������ FALSE �
      ��� ���� ��� ���������� � ������� ������� OS � ���, ���
      "�������� �� ���� ������, �����. ������!".
      ��� ����� ��������� �������� "����-���������", �.�. ���������� ��
      �����, ����� ��������� � FileList::ReadFileNames, �... ����� ��������! ;-)
      (��� ��������� ��� BugZ#513 - ��� cd �� ���������� �������
      ���� ������� ������ ���������� ������).
  14.06.2002 VVM
    + IsCommandPEExeGUI ����������. ������ ��� ���������� �� IsGUI �������,
      � �������� IMAGE_SUBSYSTEM_*
    + ���������� ����������. ������ � ������ ����� ������� "excel" � ���������� ������.
      �.�. �� ������� �������������� ����� � ������������ �� StdExecuteEx
  30.05.2002 SVS
    ! ��� CHCP �������� ����� InitRecodeOutTable().
  29.05.2002 SVS
    ! "�� ��������� � �����������" - ����� IsLocalPath() �� ������ ������.
  28.05.2002 SVS
    ! �������� �������  IsLocalPath()
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
  10.05.2002 SVS
    + ��������� CHCP
  17.04.2002 VVM
    ! ��������� �����������.
  16.04.2002 DJ
    ! ���������� ���������� ��������� cd, ���� ����� Shift-Enter
  02.04.2002 SVS
    - BugZ#421 - ������ � ����������� ����������
  30.03.2002 VVM
    ! ������� ������ ���������� ������ ���� ������� � ��������� ����.
  28.03.2002 SVS
    - CLS �� ���������� �������� �������
  26.03.2002 SVS
    - BugZ#393 - . Shift-Enter
  25.03.2002 VVM
    ! ��������� ������ ����������� :)
  22.03.2002 IS
    - ������������� ��������� ������� ������������, ������ ��� �� �����, ���
      ��� 9x �������� � SetConsoleTitle ������ ���� � ANSI-���������
  21.03.2002 SVS
    - BugZ#365 - ���� � ��������� ����� � ���������� �� ����+�����
  20.03.2002 SVS
    ! GetCurrentDirectory -> FarGetCurDir
  20.03.2002 IS
    + "if [not] exist" ������ ������ � ������� ������
    ! PrepareOSIfExist ������ ��������� � ���������� const
  18.03.2002 SVS
    ! ������ ��� ������ - ��� ��� ����� ������, ������������ 1
  28.02.2002 SVS
    - BugZ#318 - dot Shift-Enter
  26.02.2002 SVS
    ! "." � ".." �� Shift-Enter ������ AS IS, ��� �����������.
  19.02.2002 SVS
    ! � ����������� ����� ������ *ConsoleTitle, �.�. �������...
  18.02.2002 SVS
    - set ����=������
      �������� ������� ��� cmd.exe �������� ���������, � ��� ����� - ������.
      ����� ����� ����������� � �������� ���������.
  15.02.2002 DJ
    - ��� ��� ���������: ��������� ���� �� ���������� � �����, ��� �����
      ��������� ������ ����� ������� ��������� � ����������� ��������
      �� �������� ���������� ���������
  14.02.2002 SVS
    - BugZ#300 - Shift-Enter �� ����� ������ ���� ��������� ����
  14.02.2002 VVM
    ! UpdateIfChanged ��������� �� ������� Force, � �������� �� UIC_*
  07.02.2002 SKV
    - �� ���� ��� ������ ������� ������ � ���� � �.�.
  04.02.2002 SVS
    - BugZ#289 ����������� ������������ GUI/Console ��� *.exe � ��������
      �������
  30.01.2002 SVS
    ! ��������, � �� ����� �� �� ����� ������� �� Shift-Enter?
      ���� ���, �� ������ CreateProcess �������� ShellExe...
  28.01.2002 tran
    ! ������� ������� ����� �� ������.
  18.01.2002 VVM
    ! ��������� �� setdisk() -> FarChDir()
  16.01.2002 SVS
    - ��� �� ����� BugZ#238. ������� �������� ������ (�� 4096 ������ ��� FullPath)
  15.01.2002 SVS
    - �� ����������� "C:\Program Files\Far\Far.exe" "C:\Program Files\Far\Plugins"
  14.01.2002 IS
    ! chdir -> FarChDir
  24.12.2001 SVS
    - BugZ#193: �� �������� <, >, | (� 9�)
    + �������� �� �������� �� ��������� - ���� ������� ����� ����� "|" ��� ">"
      ��� "<" (�.�. ���� ������) - ��... � ����.
  21.12.2001 SVS
    - Bug: ����� ������� ����������� Java �� MS, jvc.exe ������ �������������
           � ���������������� ���������� Far'� ���������� �� ������� �����.
      ������, ���� �� �������� ����� ���� sh.exe (bash) �� ��������������
      ��������� ������.
  20.12.2001 SVS
    ! ��� Shift-Enter ����� ��������������� � ������.
  14.12.2001 IS
    ! stricmp -> LocalStricmp
  08.12.2001 SVS
    ! ��������� � ����� ����������� - ������ ��� ������� ���������� ��������
      �� �������.
  07.12.2001 SVS
    ! ��������� � ����� ����������� (�� ��� ����� ���� ;-))
    ! �� CommandLine::CmdExecute() ������� ������� ���������� � RedrawDesktop
    ! � ����� ����������� ������� ������� ������������ ������, �������,
      ���� ����������, �� �� �����������!
    ! DWORD* ��������� � DWORD&
    ! � Execute ������� (������ ��������) - const
  06.12.2001 SVS
    ! ����� � ������� ����������� � ������������ ������ � �����.
      ��������� �������� ������� � 01104.Mix.txt
  05.12.2001 SVS
    - ��� ����������� ���������� ����� "���������" ���������� ����� :-(
  04.12.2001 SVS
    - ����� �������� ��� ������ (�� ����������� ������� � ����������)
  04.12.2001 SVS
    ! ��������� ��������� ���������. �� ���� ���... ��� ������ DOC-������
      ��� ���� ����������. ����� �� ��������� - "����������" �� ��������
      ���������.
  03.12.2001 SVS
    ! ��������� ���... ���� �� �������� :-)
    ! ����� ��������� - ������ DETACHED_PROCESS � ���� ���������� ��������.
  02.12.2001 SVS
    ! �������� �����. :-(( ������ ��� �������, �� � ��������� �������,
      �� ����� �������� ������ �������, ���� ���� �� ������� ����������, �.�.
      ���� ������ "date" - ����������� ���������� ������� ���.����., ����
      ������ "date.exe", �� ����� �������� � ����������� ������ "date.exe"
      � ��������� ��� ������ ���� ��� � ������.
  30.11.2001 SVS
    ! ����� ������ ����� ���������� ��������� � ����������
  29.11.2001 SVS
    - ���� � �������� ��������� - ������ ����������� ���� ������� � OEM.
  28.11.2001 SVS
    - BugZ#129 �� ����������� ���������� � �������� � ��������
    ! ��������� ��������� � PrepareExecuteModule()
  22.11.2001 SVS
    - ��� ��������� ��� ���� ����� ���������� ����� ��� �� ������� �������:
      >"" Enter
      ��. ����� ����������� ��� �� �������.
    + � Execute() �������� �������� - SetUpDirs "����� ������������� ��������?"
      ��� ��� ��� ��� �� �����, ����� ����� "�������" ����� ���� ���
      ��������� ���������. ��� ����������� ������� �����.
  21.11.2001 SVS
    ! ����������� � ��������� "��������" ���� ���������, � ��� ��
      �������������� IsCommandExeGUI � PrepareExecuteModule (����������
      ����� �������). GetExistAppPaths ������� �� �������������.
  21.11.2001 VVM
    ! ��������� �������� ���������� ��� ������� ��������.
  20.11.2001 SVS
    - BugZ#111 - ��� cd ��: ������������� ����� ����� - ������� �� Upper.
  20.11.2001 SVS
    ! ��������� ���������.
  19.11.2001 SVS
    + GetExistAppPaths() - �������� ���� ���� ���� �� App Paths
    ! ������� IsCommandExeGUI() ������ ���������� ����������� ������ ����
      � ��������� �����
  15.11.2001 OT
    - ����������� ��������� cd c:\ �� �������� ��������� �������
  14.11.2001 SVS
    - ����������� ����������� BugZ#90 - ������ �� �����������
  12.11.2001 SVS
    - BugZ#90: ������ �������� �� ������
  12.11.2001 SVS
    ! ����� 1033 � 1041 �� ������ ������.
  08.11.2001 SVS
    - ��������� ������� (�������� � ���� ���� ���������) ������� (��� ��������)
  31.10.2001 VVM
    + ������� ���������� ������ ��������. ��������� ������� �� ����� "start.exe",
      � ����� CREATE_NEW_CONSOLE
  10.10.2001 SVS
    + ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "farqueue.hpp"
#include "filepanels.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "plugin.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"
#include "savescr.hpp"
#include "chgprior.hpp"
#include "global.hpp"
#include "cmdline.hpp"
#include "panel.hpp"
#include "fn.hpp"
#include "rdrwdsk.hpp"
#include "udlist.hpp"

static const char strSystemExecutor[]="System\\Executor";

// ��������� ����� �� �������� GetFileInfo, �������� ����������� ���������� �
// ���� PE-������
/* 14.06.2002 VVM
  + ���������� ��������� IMAGE_SUBSYSTEM_*
    ���� ������� �������� */

// ��� ������ �� ��������� IMAGE_SUBSYTEM_UNKNOWN ��������
// "���� �� �������� �����������".
// ��� DOS-���������� ��������� ��� ���� �������� �����.
#define IMAGE_SUBSYSTEM_DOS_EXECUTABLE  255

static int IsCommandPEExeGUI(const char *FileName,DWORD& ImageSubsystem)
{
//  char NameFile[NM];
  HANDLE hFile;
  int Ret=FALSE;
  ImageSubsystem = IMAGE_SUBSYSTEM_UNKNOWN;

  if((hFile=FAR_CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL)) != INVALID_HANDLE_VALUE)
  {
    DWORD FileSizeLow, FileSizeHigh, ReadSize;
    IMAGE_DOS_HEADER dos_head;

    FileSizeLow=GetFileSize(hFile,&FileSizeHigh);

    if(ReadFile(hFile,&dos_head,sizeof(IMAGE_DOS_HEADER),&ReadSize,NULL) &&
       dos_head.e_magic == IMAGE_DOS_SIGNATURE)
    {
      Ret=TRUE;
      ImageSubsystem = IMAGE_SUBSYSTEM_DOS_EXECUTABLE;
      /*  ���� �������� ����� �� �������� 18h (OldEXE - MZ) >= 40h,
      �� �������� ����� � 3Ch �������� ��������� ��������� Windows. */
      /* 31.07.2003 VVM
        ! ������� ���� MSDN - ����� ������� �� ����� */
//      if (dos_head.e_lfarlc >= 0x40)
      /* VVM $ */
      {
        DWORD signature;
        #include <pshpack1.h>
        struct __HDR
        {
           DWORD signature;
           IMAGE_FILE_HEADER _head;
           IMAGE_OPTIONAL_HEADER opt_head;
           // IMAGE_SECTION_HEADER section_header[];  /* actual number in NumberOfSections */
        } header, *pheader;
        #include <poppack.h>

        if(SetFilePointer(hFile,dos_head.e_lfanew,NULL,FILE_BEGIN) != -1)
        {
          // ������ ��������� ���������
          if(ReadFile(hFile,&header,sizeof(struct __HDR),&ReadSize,NULL))
          {
            signature=header.signature;
            pheader=&header;

            if(signature == IMAGE_NT_SIGNATURE) // PE
               ImageSubsystem = header.opt_head.Subsystem;
//            {
//              IsPEGUI=1;
//              IsPEGUI|=(header.opt_head.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI)?2:0;
//            }
            else if((WORD)signature == IMAGE_OS2_SIGNATURE) // NE
            {
              /*
                 NE,  ���...  � ��� ���������� ��� ��� ������?

                 Andrzej Novosiolov <andrzej@se.kiev.ua>
                 AN> ��������������� �� ����� "Target operating system" NE-���������
                 AN> (1 ���� �� �������� 0x36). ���� ��� Windows (�������� 2, 4) - �������������
                 AN> GUI, ���� OS/2 � ������ �������� (��������� ��������) - ������������� �������.
              */
              BYTE ne_exetyp=((IMAGE_OS2_HEADER *)pheader)->ne_exetyp;
              if(ne_exetyp == 2 || ne_exetyp == 4)
                ImageSubsystem=IMAGE_SUBSYSTEM_WINDOWS_GUI;
            }
          }
          else
          {
            ; // ������ ����� � ������� ���������� ��������� ;-(
          }
        }
        else
        {
          ; // ������ �������� ���� ���� � �����, �.�. dos_head.e_lfanew
            // ������ ������� � �������������� ����� (�������� ��� ������
            // ���� DOS-����
        }
      }
/*      else
      {
        ; // ��� ������� EXE, �� �� �������� EXE
      }
*/
    }
    else
    {
      ; // ��� �� ����������� ���� - � ���� ���� ��������� MZ
        // ��������, NLM-������ ��� ������ ������ :-)
    }
    CloseHandle(hFile);
  }

  return Ret;
}
/* VVM $ */

// �� ����� ����� (�� ��� ����������) �������� ������� ���������
// ������������� ��������� �������� �������-����������
// (����� �� ����� ����������)
char* GetShellAction(const char *FileName,DWORD& ImageSubsystem,DWORD& Error)
{
  char Value[512];
  const char *ExtPtr;
  char *RetPtr;
  LONG ValueSize;
  const char command_action[]="\\command";

  Error=0;
  ImageSubsystem = IMAGE_SUBSYSTEM_UNKNOWN;

  if ((ExtPtr=strrchr(FileName,'.'))==NULL)
    return(NULL);

  ValueSize=sizeof(Value);

  if (RegQueryValue(HKEY_CLASSES_ROOT,(LPCTSTR)ExtPtr,(LPTSTR)Value,&ValueSize)!=ERROR_SUCCESS)
    return(NULL);

  strcat(Value,"\\shell");

  HKEY hKey;
  if (RegOpenKey(HKEY_CLASSES_ROOT,Value,&hKey)!=ERROR_SUCCESS)
    return(NULL);

  static char Action[80];
  ValueSize=sizeof(Action);
  LONG RetQuery = RegQueryValueEx(hKey,"",NULL,NULL,(unsigned char *)Action,(LPDWORD)&ValueSize);

  strcat(Value,"\\");
  if (RetQuery == ERROR_SUCCESS)
  {
    RetPtr=(*Action==0 ? NULL:Action);
    /* $ 03.10.2002 VVM
      + ������ � ����� ������ ����� ���� ���������. */
    const char *ActionPtr;
    UserDefinedList ActionList(0,0,ULF_UNIQUE);

    LONG RetEnum = ERROR_SUCCESS;
    if (RetPtr != NULL && ActionList.Set(Action))
    {
      char NewValue[512];
      HKEY hOpenKey;
      ActionList.Reset();
      while (RetEnum == ERROR_SUCCESS && (ActionPtr = ActionList.GetNext()) != NULL)
      {
        strncpy(NewValue, Value, sizeof(NewValue) - 1);
        strcat(NewValue, ActionPtr);
        strcat(NewValue, command_action);
        if (RegOpenKey(HKEY_CLASSES_ROOT,NewValue,&hOpenKey)==ERROR_SUCCESS)
        {
          RegCloseKey(hOpenKey);
          strcat(Value, ActionPtr);
          RetPtr = strcpy(Action,ActionPtr);
          RetEnum = ERROR_NO_MORE_ITEMS;
        } /* if */
      } /* while */
    } /* if */
    else
      strcat(Value,Action);
    /* VVM $ */
    if(RetEnum != ERROR_NO_MORE_ITEMS) // ���� ������ �� �����, ��...
      RetPtr=NULL;
  }
  else
  {
    // This member defaults to "Open" if no verb is specified.
    // �.�. ���� �� ������� NULL, �� ��������������� ������� "Open"
      RetPtr=NULL;
//    strcat(Value,"\\open");
  }

  // ���� RetPtr==NULL - �� �� ����� default action.
  // ��������� - ���� �� ������ ���-������ � ����� ����������
  if (RetPtr==NULL)
  {
    LONG RetEnum = ERROR_SUCCESS;
    char NewValue[512];
    DWORD dwIndex = 0;
    DWORD dwKeySize = 0;
    FILETIME ftLastWriteTime;
    HKEY hOpenKey;

    // ������� �������� "open"...
    strcpy(Action,"open");
    strncpy(NewValue, Value, sizeof(NewValue) - 1);
    strcat(NewValue, Action);
    strcat(NewValue, command_action);
    if (RegOpenKey(HKEY_CLASSES_ROOT,NewValue,&hOpenKey)==ERROR_SUCCESS)
    {
      RegCloseKey(hOpenKey);
      strcat(Value, Action);
      RetPtr = Action;
      RetEnum = ERROR_NO_MORE_ITEMS;
    } /* if */

    // ... � ������ ��� ���������, ���� "open" ����
    while (RetEnum == ERROR_SUCCESS)
    {
      dwKeySize = sizeof(Action);
      RetEnum = RegEnumKeyEx(hKey, dwIndex++, Action, &dwKeySize, NULL, NULL, NULL, &ftLastWriteTime);
      if (RetEnum == ERROR_SUCCESS)
      {
        // �������� ������� "�������" � ����� �����
        strncpy(NewValue, Value, sizeof(NewValue) - 1);
        strcat(NewValue, Action);
        strcat(NewValue, command_action);
        if (RegOpenKey(HKEY_CLASSES_ROOT,NewValue,&hOpenKey)==ERROR_SUCCESS)
        {
          RegCloseKey(hOpenKey);
          strcat(Value, Action);
          RetPtr = Action;
          RetEnum = ERROR_NO_MORE_ITEMS;
        } /* if */
      } /* if */
    } /* while */
  } /* if */
  RegCloseKey(hKey);

  if (RetPtr != NULL)
  {
    strcat(Value,command_action);

    // � ������ �������� �������� ����������� �����
    if (RegOpenKey(HKEY_CLASSES_ROOT,Value,&hKey)==ERROR_SUCCESS)
    {
      char Command[2048];
      ValueSize=sizeof(Command);
      RetQuery=RegQueryValueEx(hKey,"",NULL,NULL,(unsigned char *)Command,(LPDWORD)&ValueSize);
      RegCloseKey(hKey);
      if(RetQuery == ERROR_SUCCESS)
      {
        char *Ptr;
        ExpandEnvironmentStr(Command,Command,sizeof(Command));
        // �������� ��� ������
        if (*Command=='\"')
        {
          FAR_OemToChar(Command+1,Command);
          if ((Ptr=strchr(Command,'\"'))!=NULL)
            *Ptr=0;
        }
        else
        {
          FAR_OemToChar(Command,Command);
          if ((Ptr=strpbrk(Command," \t/"))!=NULL)
            *Ptr=0;
        }
        IsCommandPEExeGUI(Command,ImageSubsystem);
      }
      else
      {
        Error=ERROR_NO_ASSOCIATION;
        RetPtr=NULL;
      }
    }
  }

  return RetPtr;
}

/*
 ������ PrepareExecuteModule �������� ����� ����������� ������ (� �.�. � ��
 %PATHEXT%). � ������ ������ �������� � Command ������, ������������� ��
 ����������� ������ �� ��������� ��������, �������� ��������� � Dest �
 �������� ��������� ��������� PE �� �������� (����� ��������� �������
 � ��������� ���� � �� ����� ����������).
 � ������ ������� Dest �� �����������!
 Return: TRUE/FALSE - �����/�� �����
*/
/* $ 14.06.2002 VVM
 ������� � ������� ���������� ��� �������������. ������ �� ������.
 � ��������� ������ �� ����, �.�. ��� ��������� �� ������� ������
*/
int WINAPI PrepareExecuteModule(const char *Command,char *Dest,int DestSize,DWORD& ImageSubsystem)
{
  int Ret, I;
  char FileName[4096],FullName[4096], *Ptr;
  // int IsQuoted=FALSE;
  // int IsExistExt=FALSE;

  // ����� ������� �����! ������� �������,  � ����� ��������� �����.
  static char StdExecuteExt[NM]=".BAT;.CMD;.EXE;.COM;";
  static const char RegPath[]="SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\";
  static int PreparePrepareExt=FALSE;

  if(!PreparePrepareExt) // ���������������������� �����
  {
    // ���� ���������� %PATHEXT% ��������...
    if((I=GetEnvironmentVariable("PATHEXT",FullName,sizeof(FullName)-1)) != 0)
    {
      FullName[I]=0;
      // ������� ������� �� PATHEXT
      static char const * const StdExecuteExt0[4]={".BAT;",".CMD;",".EXE;",".COM;"};
      for(I=0; I < sizeof(StdExecuteExt0)/sizeof(StdExecuteExt0[0]); ++I)
        ReplaceStrings(FullName,StdExecuteExt0[I],"",-1);
    }

    Ptr=strcat(StdExecuteExt,strcat(FullName,";")); //!!!
    StdExecuteExt[strlen(StdExecuteExt)]=0;
    while(*Ptr)
    {
      if(*Ptr == ';')
        *Ptr=0;
      ++Ptr;
    }
    PreparePrepareExt=TRUE;
  }

  /* ����� "����������" �� �������, ������� ������ ����������� ��������,
     ��������, ��������� ���������� ������� ���.����������.
  */
  static char ExcludeCmds[4096]={0};
  static int PrepareExcludeCmds=FALSE;
  if(GetRegKey(strSystemExecutor,"Type",0))
  {
    if (!PrepareExcludeCmds)
    {
      GetRegKey(strSystemExecutor,"ExcludeCmds",(char*)ExcludeCmds,"",0);
      Ptr=strcat(ExcludeCmds,";"); //!!!
      ExcludeCmds[strlen(ExcludeCmds)]=0;
      while(*Ptr)
      {
        if(*Ptr == ';')
          *Ptr=0;
        ++Ptr;
      }
      PrepareExcludeCmds=TRUE;
    }
  }
  else
  {
    *ExcludeCmds=0;
    PrepareExcludeCmds=FALSE;
  }

  ImageSubsystem = IMAGE_SUBSYSTEM_UNKNOWN; // GUIType ������ ������� ���������������� � FALSE
  Ret=FALSE;

  /* $ 14.06.2002 VVM
     ��� ������ ������ ���������� ��� �������. ����� ������ ��� ������ ����/����
  // �������� ��� ������
  if (*Command=='\"')
  {
    FAR_OemToChar(Command+1,FullName);
    if ((Ptr=strchr(FullName,'\"'))!=NULL)
      *Ptr=0;
    IsQuoted=TRUE;
  }
  else
  {
    FAR_OemToChar(Command,FullName);
    if ((Ptr=strpbrk(FullName," \t/|><"))!=NULL)
      *Ptr=0;
  } VVM $ */

  if(!*Command) // ��� ��, ���� ��... �������� �������� :-(
    return 0;

  FAR_OemToChar(Command,FullName);

  /* $ 07.09.2001 VVM ���������� ���������� ��������� */
  ExpandEnvironmentStrings(FullName,FileName,sizeof(FullName));

  // ������� ������ - ������� ����������
  {
    char *Ptr=ExcludeCmds;
    while(*Ptr)
    {
      if(!LocalStricmp(FileName,Ptr))
      {
        ImageSubsystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
        return TRUE;
      }
      Ptr+=strlen(Ptr)+1;
    }
  }

  // IsExistExt - ���� ����� ���� (����������), �� ����� �������������� ��
  // �����.
  // IsExistExt=strrchr(FullName,'.')!=NULL;

  SetFileApisTo(APIS2ANSI);

  {
    char *FilePart;
    char *PtrFName=strrchr(PointToName(strcpy(FullName,FileName)),'.');
    char *WorkPtrFName;
    if(!PtrFName)
      WorkPtrFName=FullName+strlen(FullName);

    char *PtrExt=StdExecuteExt;
    while(*PtrExt) // ������ ������ - � ������� ��������
    {
      if(!PtrFName)
        strcpy(WorkPtrFName,PtrExt);
      if(GetFileAttributes(FullName) != -1)
      {
        // GetFullPathName - ��� �����, �.�. ���� ������ � date.exe
        // � ������� ��������, �� ������ ������ ������� �� ��������
        // cmd.exe �� ����� �� �������� ������� ���������� date
        GetFullPathName(FullName,sizeof(FullName),FullName,&FilePart);

        Ret=TRUE;
        break;
      }
      PtrExt+=strlen(PtrExt)+1;
    }

    if(!Ret) // ������ ������ - �� �������� SearchPath
    {
      /* $ 26.09.2003 VVM
        ! ������� ������ �� ���������� PATH, � �� ����� ����� */
      char PathEnv[4096];
      if (GetEnvironmentVariable("PATH",PathEnv,sizeof(PathEnv)-1) != 0)
      {
        PtrExt=StdExecuteExt;
        while(*PtrExt)
        {
          if(!PtrFName)
            strcpy(WorkPtrFName,PtrExt);
          if(SearchPath(PathEnv,FullName,PtrExt,sizeof(FullName),FullName,&FilePart))
          {
            Ret=TRUE;
            break;
          }
          PtrExt+=strlen(PtrExt)+1;
        }
      }

      if (!Ret)
      {
        PtrExt=StdExecuteExt;
        while(*PtrExt)
        {
          if(!PtrFName)
            strcpy(WorkPtrFName,PtrExt);
          if(SearchPath(NULL,FullName,PtrExt,sizeof(FullName),FullName,&FilePart))
          {
            Ret=TRUE;
            break;
          }
          PtrExt+=strlen(PtrExt)+1;
        }
      }
      /* VVM $ */

      if (!Ret && Opt.ExecuteUseAppPath) // ������ ������ - ����� � ������ � "App Paths"
      {
        // � ������ Command ������� ����������� ������ �� ������ ����, �������
        // ������� �� SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths
        // ������� ������� � HKCU, ����� - � HKLM
        HKEY hKey;
        HKEY RootFindKey[2]={HKEY_CURRENT_USER,HKEY_LOCAL_MACHINE};

        for(I=0; I < sizeof(RootFindKey)/sizeof(RootFindKey[0]); ++I)
        {
          sprintf(FullName,"%s%s",RegPath,FileName);
          if (RegOpenKeyEx(RootFindKey[I], FullName, 0,KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
          {
            DWORD Type, DataSize=sizeof(FullName);
            RegQueryValueEx(hKey,"", 0, &Type, (LPBYTE)FullName,&DataSize);
            RegCloseKey(hKey);
            /* $ 03.10.2001 VVM ���������� ���������� ��������� */
            strcpy(FileName, FullName);
            Unquote(FileName);
            ExpandEnvironmentStrings(FileName,FullName,sizeof(FullName));
            Ret=TRUE;
            break;
          }
        }

        if (!Ret && Opt.ExecuteUseAppPath)
        /* $ 14.06.2002 VVM
           �� ����� - ��������� � ����������� */
        {
          PtrExt=StdExecuteExt;
          while(*PtrExt && !Ret)
          {
            for(I=0; I < sizeof(RootFindKey)/sizeof(RootFindKey[0]); ++I)
            {
              sprintf(FullName,"%s%s%s",RegPath,FileName,PtrExt);
              if (RegOpenKeyEx(RootFindKey[I], FullName, 0,KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
              {
                DWORD Type, DataSize=sizeof(FullName);
                RegQueryValueEx(hKey,"", 0, &Type, (LPBYTE)FullName,&DataSize);
                RegCloseKey(hKey);
                /* $ 03.10.2001 VVM ���������� ���������� ��������� */
                strcpy(FileName, FullName);
                Unquote(FileName);
                ExpandEnvironmentStrings(FileName,FullName,sizeof(FullName));
                Ret=TRUE;
                break;
              }
            } /* for */
            PtrExt+=strlen(PtrExt)+1;
          }
        } /* if */
      } /* if */
    }
  }

  if(Ret) // ��������� "�������" ������
  {
    // char TempStr[4096];
    // ������� ��������...
    IsCommandPEExeGUI(FullName,ImageSubsystem);
    /* $ 14.06.2002 VVM
       �� ���� ������� - ����� ��� ������� - ��� � �������...
    QuoteSpaceOnly(FullName);
    QuoteSpaceOnly(FileName);
      VVM $ */

    // ��� ������, ����� ����������� ������:
    /* $ 14.06.2002 VVM
       ������ - ���������� ������ � ����� �����...
    if(strpbrk(FullName,"()"))
      IsExistExt=FALSE;
      VVM $ */

    // strncpy(TempStr,Command,sizeof(TempStr)-1);
    FAR_CharToOem(FullName,FullName);
    // FAR_CharToOem(FileName,FileName);
    // ReplaceStrings(TempStr,FileName,FullName);
    if(!DestSize)
      DestSize=strlen(FullName);
    // if(Dest && IsExistExt)
    if (Dest)
      strncpy(Dest,FullName,DestSize);
  }

  SetFileApisTo(APIS2OEM);
  return(Ret);
}

/* $ 14.06.2002 VVM
   �������� ��� �������, �.�. �� ����� �� �������� */
#ifdef ADD_GUI_CHECK
DWORD IsCommandExeGUI(const char *Command)
{
  char FileName[4096],FullName[4096],*EndName,*FilePart;

  if (*Command=='\"')
  {
    FAR_OemToChar(Command+1,FullName);
    if ((EndName=strchr(FullName,'\"'))!=NULL)
      *EndName=0;
  }
  else
  {
    FAR_OemToChar(Command,FullName);
    if ((EndName=strpbrk(FullName," \t/"))!=NULL)
      *EndName=0;
  }
  int GUIType=0;

  /* $ 07.09.2001 VVM
    + ���������� ���������� ��������� */
  ExpandEnvironmentStrings(FullName,FileName,sizeof(FileName));
  /* VVM $ */

  SetFileApisTo(APIS2ANSI);
  /*$ 18.09.2000 skv
    + to allow execution of c.bat in current directory,
      if gui program c.exe exists somewhere in PATH,
      in FAR's console and not in separate window.
      for(;;) is just to prevent multiple nested ifs.
  */
  for(;;)
  {
    sprintf(FullName,"%s.bat",FileName);
    if(GetFileAttributes(FullName)!=-1)break;
    sprintf(FullName,"%s.cmd",FileName);
    if(GetFileAttributes(FullName)!=-1)break;
  /* skv$*/

    if (SearchPath(NULL,FileName,".exe",sizeof(FullName),FullName,&FilePart))
    {
      SHFILEINFO sfi;
      DWORD ExeType=SHGetFileInfo(FullName,0,&sfi,sizeof(sfi),SHGFI_EXETYPE);
      GUIType=HIWORD(ExeType)>=0x0300 && HIWORD(ExeType)<=0x1000 &&
              /* $ 13.07.2000 IG
                 � VC, ������, ������ ������� ���: 0x4550 == 'PE', ����
                 ������ �������� ���������.
              */
              HIBYTE(ExeType)=='E' && (LOBYTE(ExeType)=='N' || LOBYTE(ExeType)=='P');
              /* IG $ */
    }
/*$ 18.09.2000 skv
    little trick.
*/
    break;
  }
  /* skv$*/
  SetFileApisTo(APIS2OEM);
  return(GUIType);
}
#endif
/* VVM $ */

/* �������-��������� ������� ���������
   ���������� -1 � ������ ������ ���...
*/
int Execute(const char *CmdStr,          // ���.������ ��� ����������
            int AlwaysWaitFinish,  // ����� ���������� ��������?
            int SeparateWindow,    // ��������� � ��������� ����? =2 ��� ������ ShellExecuteEx()
            int DirectRun,         // ��������� ��������? (��� CMD)
            int SetUpDirs)         // ����� ������������� ��������?
{
  int NT = WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT;
  char NewCmdStr[4096];
  char NewCmdPar[4096];
  char CommandName[NM];
  NewCmdPar[0] = 0;

  // ������������ �� ��������
  ////_tran(SysLog("Execute: CmdStr [%s]",CmdStr);)

  strcpy(NewCmdStr,CmdStr);
  RemoveExternalSpaces(NewCmdStr);

  // �������� ���������� ������� ��� ���������� � ���������.
  // ��� ���� ������ ��������� ������� �������� ��������������� �������
  // �������� � ������ �������. �.�. ���� � �������� - �� ����.
  char *CmdPtr = NewCmdStr;
  char *ParPtr = NULL;
  int QuoteFound = FALSE;
  int PipeFound = FALSE;

  while (*CmdPtr)
  {
    if (*CmdPtr == '"')
      QuoteFound = !QuoteFound;
    if (!QuoteFound)
    {
      if (*CmdPtr == '>' || *CmdPtr == '<' ||
          *CmdPtr == '|' || *CmdPtr == ' ' ||
          *CmdPtr == '/' ||      // ������� "far.exe/?"
          (NT && *CmdPtr == '&') // ��� ��/2� ���������� ����������� ������
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

  if (ParPtr)
  // �� ����� ��������� � �������� ��� �� ������
  {
    if (*ParPtr=='/')
    {
      *NewCmdPar=0x20;
      strcpy(NewCmdPar+1, ParPtr);
    }
    else
      strcpy(NewCmdPar, ParPtr);
    *ParPtr = 0;
  }
  Unquote(NewCmdStr);
  RemoveExternalSpaces(NewCmdStr);

  //_tran(SysLog("Execute: newCmdStr [%s]",NewCmdStr);)

  // ��������, � �� ����� �� �� ����� ������� �� Shift-Enter?
  //_SVS(SysLog("SeparateWindow=%d",SeparateWindow));
  DWORD Attr=GetFileAttributes(NewCmdStr);
  if(SeparateWindow) //???
  {
    if(!*NewCmdPar && Attr != -1 && (Attr&FILE_ATTRIBUTE_DIRECTORY) || (NewCmdStr[1]==':' && (NewCmdStr[2] == '\\' && !NewCmdStr[3] || !NewCmdStr[2])))
    {
      ConvertNameToFull(NewCmdStr,NewCmdStr,sizeof(NewCmdStr));
      SeparateWindow=2;
    }
  }

  // ������ �� ���������
  if(!*NewCmdStr || (strlen(NewCmdStr)==1 && strpbrk(NewCmdStr,"<>|:")!=NULL))
  {
    // � ����� ������ ��������� CMD ��� ���������?
    // ���� "��", �� ���� ����� ����� ���� ���������.
    return -1;
  }
  // �������� CP-������� ����� ������� �����
  UINT ConsoleCP=GetConsoleCP();
  UINT ConsoleOutputCP=GetConsoleOutputCP();

  CONSOLE_SCREEN_BUFFER_INFO sbi;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  int Visible,Size;
  int PrevLockCount;
  char ExecLine[4096];
  char OldTitle[512];
  DWORD ImageSubsystem = IMAGE_SUBSYSTEM_UNKNOWN;
  int ExitCode=1;
  DWORD _LastErrCode=0;

  int ExecutorType = GetRegKey(strSystemExecutor,"Type",0);
  // ������� ������ - �.�. �������, �� ����� ���������� �������!
  if(*NewCmdStr && !((*NewCmdStr == '\\'|| *NewCmdStr == '/') && !NewCmdStr[1]))
  {
    ExitCode=PrepareExecuteModule(NewCmdStr,NewCmdStr,sizeof(NewCmdStr)-1,ImageSubsystem);
    Attr=GetFileAttributes(NewCmdStr);
  }
  // ��� �������-��� ������ ������� ������ ����� ShellExecuteEx()
//  if (ImageSubsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI && !AlwaysWaitFinish)
  if (ImageSubsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI)
    SeparateWindow=2;
  if (!ExecutorType || SeparateWindow==2)
    ExitCode = 1;
  QuoteSpace(NewCmdStr);
  QuoteFound = NewCmdStr[0] == '"';
  CmdPtr = NewCmdStr;

  CommandName[0]=0;
  GetEnvironmentVariable("COMSPEC",CommandName,sizeof(CommandName));
  if ((CommandName[0] == 0) && (SetUpDirs || (!ExecutorType && SeparateWindow != 2)))
  {
    Message(MSG_WARNING, 1, MSG(MWarning), MSG(MComspecNotFound), MSG(MErrorCancelled), MSG(MOk));
    return -1;
  }

  /* $ 13.04.2001 VVM
    + ���� CREATE_DEFAULT_ERROR_MODE. ���-�� ��������� ��� ������ */
  DWORD CreateFlags = CREATE_DEFAULT_ERROR_MODE;
  /* VVM $ */

  GetCursorType(Visible,Size);
  //SetCursorType(0,-1);
  SetInitialCursorType();

  PrevLockCount=ScrBuf.GetLockCount();
  ScrBuf.SetLockCount(0);
  ScrBuf.Flush();

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  GetConsoleTitle(OldTitle,sizeof(OldTitle));
  memset(&si,0,sizeof(si));
  si.cb=sizeof(si);

  if (SetUpDirs)
  {
    Panel *PassivePanel=CtrlObject->Cp()->GetAnotherPanel(CtrlObject->Cp()->ActivePanel);
    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS && PassivePanel->GetType()==FILE_PANEL)
      for (int I=0;CmdStr[I]!=0;I++)
        if (isalpha(CmdStr[I]) && CmdStr[I+1]==':' && CmdStr[I+2]!='\\')
        {
          char SavePath[NM],PanelPath[NM],SetPathCmd[NM];
          FarGetCurDir(sizeof(SavePath),SavePath);
          PassivePanel->GetCurDir(PanelPath);
          sprintf(SetPathCmd,"%s /C chdir %s",CommandName,QuoteSpace(PanelPath));
          CreateProcess(NULL,SetPathCmd,NULL,NULL,FALSE,CreateFlags,NULL,NULL,&si,&pi);
          CloseHandle(pi.hThread);
          CloseHandle(pi.hProcess);
          FarChDir(SavePath);
        }
  }

//  strcpy(NewCmdStr,CmdStr);
//  if (ParPtr)
//    *ParPtr = 0;
  //while (IsSpace(*CmdPtr))
  //  CmdPtr++;

  if (ExitCode)
  {
    if (DirectRun && !SeparateWindow)
      strcpy(ExecLine,CmdPtr);
    else
    {
      if(ExecutorType)
      {
        if(ImageSubsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI && !AlwaysWaitFinish)
          strcpy(ExecLine,NewCmdStr);
        else
        {
          char TemplExecute[512];
          char TemplExecuteStart[512];
          char TemplExecuteWait[512];
          // <TODO: ����� ���� �� ������� ����������>
          GetRegKey(strSystemExecutor,"Normal",TemplExecute,"%COMSPEC% /c",sizeof(TemplExecute));
          GetRegKey(strSystemExecutor,"Start",TemplExecuteStart,"%COMSPEC% /c start",sizeof(TemplExecuteStart));
          GetRegKey(strSystemExecutor,"Wait",TemplExecuteWait,"%COMSPEC% /c start /wait",sizeof(TemplExecuteWait));

          char *Fmt=TemplExecute;
          if (SeparateWindow || ImageSubsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI && (NT || AlwaysWaitFinish))
          {
            Fmt=TemplExecuteStart;
            if (AlwaysWaitFinish)
              Fmt=TemplExecuteWait;
          }
          char *CmdEnd=NewCmdStr+strlen(NewCmdStr)-1;
          if (NT && *NewCmdStr == '\"' && *CmdEnd == '\"' &&
             strchr(NewCmdStr+1, '\"') != CmdEnd && SeparateWindow!=2)
          {
            char *Ptr=NewCmdStr;
            int NumSq=0;
            while(*Ptr)
            {
              if(*Ptr == '\"')
                NumSq++;
              ++Ptr;
            }
            InsertQuote(NewCmdStr);
            if(NumSq > 2)
              InsertQuote(NewCmdStr);
          }

//          if(SeparateWindow)
//            ReplaceStrings(NewCmdPar,"\"","\"\"",-1);

          strncpy(ExecLine,Fmt,sizeof(ExecLine)-1);
          strcat(ExecLine,(Fmt != TemplExecute && NT && *CmdPtr=='\"'?" \"\" ":" "));
          strcat(ExecLine, NewCmdStr);
          strcat(ExecLine, NewCmdPar);
          ExpandEnvironmentStr(ExecLine,ExecLine,sizeof(ExecLine));
          // </TODO>
          //_tran(SysLog("Execute: ExecLine [%s]",ExecLine);)
        }
      }
      else if (SeparateWindow != 2)
      {
//        int Pipe=strpbrk(CmdPtr,"<>|")!=NULL;
        sprintf(ExecLine,"%s /C",CommandName);
        //_tran(SysLog("1. execline='%s'",ExecLine);)

        int QuoteAll = FALSE;
// WINDOWS_GUI ������ ��������� ����� ShellExecute
//        if ((PipeFound && (SeparateWindow || ImageSubsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI && (NT || AlwaysWaitFinish))) ||
//            (QuoteFound && NT && ImageSubsystem != IMAGE_SUBSYSTEM_WINDOWS_GUI))
        if ((PipeFound && SeparateWindow) || (QuoteFound && NT))
          QuoteAll = TRUE;

        if (SeparateWindow)
        {
          strcat(ExecLine," start");
          if (AlwaysWaitFinish)
            strcat(ExecLine," /wait");
          if(PipeFound || QuoteAll)
           sprintf(ExecLine+strlen(ExecLine)," %s /C",CommandName);
          else if (NT && *CmdPtr=='\"')
            strcat(ExecLine," \"\"");
//          ReplaceStrings(NewCmdPar,"\"","\"\"",-1);
        }

        strcat(ExecLine," ");
        //_tran(SysLog("2. execline=[%s]",ExecLine);)
        //_tran(SysLog("3. cmdptr=[%s]",CmdPtr);)

        if (QuoteAll) strcat(ExecLine, "\"");
        strcat(ExecLine, CmdPtr);
        strcat(ExecLine, NewCmdPar);
        if (QuoteAll) strcat(ExecLine, "\"");
        //_tran(SysLog("Execute: ExecLine2 [%s]",ExecLine);)
      }
    }
_SVS(SysLog("ExecLine='%s'",ExecLine));
    // ���� ������ ����� ShellExecuteEx(), �� ������ ������� ���������
    /* $ 22.03.2002 IS
         ��� 9x �������� � SetConsoleTitle ������ ���� ANSI
    */
    if(SeparateWindow != 2)
    {
      if (WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT)
      {
        char FarTitle[2*NM];
        int size=Min((DWORD)strlen(CmdStr),(DWORD)sizeof(FarTitle)-1);
        FAR_OemToCharBuff(CmdStr,FarTitle,size);
        FarTitle[size]=0;
        SetConsoleTitle(FarTitle);
      }
      else
        SetConsoleTitle(CmdStr);//SetFarTitle(CmdPtr);
    }
    /* IS $ */
//_SVS(SysLog("CmdPtr  ='%s'",CmdPtr));
    FlushInputBuffer();

    /*$ 15.03.2001 SKV
      ���� ��������� ��������� ������� �� ������ � �.�.
    */
    GetConsoleScreenBufferInfo(hConOut,&sbi);
    /* SKV$*/

    ChangeConsoleMode(InitialConsoleMode);

    if (SeparateWindow && (ImageSubsystem == IMAGE_SUBSYSTEM_UNKNOWN))
        CreateFlags|=DETACHED_PROCESS;

    if (SeparateWindow==2)
    {
      char AnsiCmdStr[4096];
      char AnsiCmdPar[4096];
      SHELLEXECUTEINFO si;
      FAR_OemToChar(CmdPtr, AnsiCmdStr);
      FAR_OemToChar(NewCmdPar, AnsiCmdPar);

      if (PointToName(AnsiCmdStr)==AnsiCmdStr && strcmp(AnsiCmdStr,".") && !TestParentFolderName(AnsiCmdStr))
      {
        char FullName[4096];
        sprintf(FullName,".\\%s",AnsiCmdStr);
        strcpy(AnsiCmdStr,FullName);
      }
      Unquote(AnsiCmdStr); // �.�. ����� ��� �������?
      // ???
      if(Attr != -1 && (Attr&FILE_ATTRIBUTE_DIRECTORY) && !(!strcmp(AnsiCmdStr,".") || TestParentFolderName(AnsiCmdStr))
         && !(AnsiCmdStr[0] == '\\' && AnsiCmdStr[1] == '\\') // Bug 752 - �������� Shift-Enter ��� UNC �������� � ��������� � ���
      )
        strcat(AnsiCmdStr,".");
      // ???

      memset(&si,0,sizeof(si));
      si.lpVerb=(Attr&FILE_ATTRIBUTE_DIRECTORY)?NULL:GetShellAction((char *)AnsiCmdStr,ImageSubsystem,_LastErrCode);
      if(!_LastErrCode)
      {
        si.cbSize=sizeof(si);
        si.fMask=SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_DDEWAIT|SEE_MASK_FLAG_NO_UI;
        si.lpFile=AnsiCmdStr;
        si.nShow=SW_SHOWNORMAL;
        if (AnsiCmdPar[0])
          si.lpParameters = AnsiCmdPar;
        SetFileApisTo(APIS2ANSI);
        ExitCode=ShellExecuteEx(&si);
        if(!ExitCode)
          _LastErrCode=GetLastError();
        SetFileApisTo(APIS2OEM);
        pi.hProcess=si.hProcess;
      }
      else
        ExitCode=0;
    }
    else
    {
      SetRealColor(F_LIGHTGRAY|B_BLACK); // ������� ������ � ����� ����� � 4NT ��� ������ �������
      ExitCode=CreateProcess(NULL,ExecLine,NULL,NULL,0,CreateFlags,NULL,NULL,&si,&pi);
      if(!ExitCode)
        _LastErrCode=GetLastError();
    }

    StartExecTime=clock();
  }

  if (ExitCode)
  {
    if (!SeparateWindow && ImageSubsystem != IMAGE_SUBSYSTEM_WINDOWS_GUI || AlwaysWaitFinish)
    {
      /*$ 12.02.2001 SKV
        ����� ����� ;)
        ��������� ��������� ������� �� ���������������� ��������.
        ������� ������� � System/ConsoleDetachKey
      */
      if(Opt.ConsoleDetachKey>0)
      {
        HANDLE h[2];
        HANDLE hConOut=GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE hConInp=GetStdHandle(STD_INPUT_HANDLE);
        h[0]=pi.hProcess;
        h[1]=hConInp;
        INPUT_RECORD ir[256];
        DWORD rd;

        int vkey=0,ctrl=0;
        TranslateKeyToVK(Opt.ConsoleDetachKey,vkey,ctrl,NULL);
        int alt=ctrl&PKF_ALT;
        int shift=ctrl&PKF_SHIFT;
        ctrl=ctrl&PKF_CONTROL;

        while(WaitForMultipleObjects(2,h,FALSE,INFINITE)!=WAIT_OBJECT_0)
        {
          if(PeekConsoleInput(h[1],ir,256,&rd) && rd)
          {
            int stop=0;
            for(int i=0;i<rd;i++)
            {
              PINPUT_RECORD pir=&ir[i];
              if(pir->EventType==KEY_EVENT)
              {
                if(vkey==pir->Event.KeyEvent.wVirtualKeyCode &&
                  (alt?(pir->Event.KeyEvent.dwControlKeyState&LEFT_ALT_PRESSED || pir->Event.KeyEvent.dwControlKeyState&RIGHT_ALT_PRESSED):!(pir->Event.KeyEvent.dwControlKeyState&LEFT_ALT_PRESSED || pir->Event.KeyEvent.dwControlKeyState&RIGHT_ALT_PRESSED)) &&
                  (ctrl?(pir->Event.KeyEvent.dwControlKeyState&LEFT_CTRL_PRESSED || pir->Event.KeyEvent.dwControlKeyState&RIGHT_CTRL_PRESSED):!(pir->Event.KeyEvent.dwControlKeyState&LEFT_CTRL_PRESSED || pir->Event.KeyEvent.dwControlKeyState&RIGHT_CTRL_PRESSED)) &&
                  (shift?(pir->Event.KeyEvent.dwControlKeyState&SHIFT_PRESSED):!(pir->Event.KeyEvent.dwControlKeyState&SHIFT_PRESSED))
                  )
                {

                  HICON hSmallIcon=NULL,hLargeIcon=NULL;
                  if(hFarWnd)
                  {
                    hSmallIcon=CopyIcon((HICON)SendMessage(hFarWnd,WM_SETICON,0,(LPARAM)0));
                    hLargeIcon=CopyIcon((HICON)SendMessage(hFarWnd,WM_SETICON,1,(LPARAM)0));
                  }
                  ReadConsoleInput(hConInp,ir,256,&rd);
                  /*$ 07.02.2002 SKV
                    �� ����� ������� CloseConsole, ������, ��� ��� ��������
                    ConsoleMode �� ���, ��� ��� �� ������� Far'�,
                    ���� ���������� ���������� ����� � �� �������.
                  */
                  //CloseConsole();
                  CloseHandle(hConInp);
                  CloseHandle(hConOut);
                  delete KeyQueue;
                  KeyQueue=NULL;
                  /* SKV $*/

                  FreeConsole();
                  AllocConsole();

                  /*$ 17.05.2001 SKV
                    ���� ���� ����� HOTKEY, �� ������ ������ ��� ������.
                  */
                  if(hFarWnd)
                  {
                    SendMessage(hFarWnd,WM_SETHOTKEY,0,(LPARAM)0);
                  }
                  /* SKV $*/


                  /*$ 20.03.2001 SKV
                    ��� ����� ��� ������ :-\
                  */
                  SetConsoleScreenBufferSize(hConOut,sbi.dwSize);
                  SetConsoleWindowInfo(hConOut,TRUE,&sbi.srWindow);
                  SetConsoleScreenBufferSize(hConOut,sbi.dwSize);

                  /* SKV$*/

                  Sleep(100);
                  InitConsole(0);

                  hFarWnd=0;
                  InitDetectWindowedMode();

                  if (hFarWnd)
                  {
                    if(Opt.SmallIcon)
                    {
                      char FarName[NM];
                      GetModuleFileName(NULL,FarName,sizeof(FarName));
                      ExtractIconEx(FarName,0,&hLargeIcon,&hSmallIcon,1);
                    }
                    if (hLargeIcon!=NULL)
                      SendMessage(hFarWnd,WM_SETICON,1,(LPARAM)hLargeIcon);
                    if (hSmallIcon!=NULL)
                      SendMessage(hFarWnd,WM_SETICON,0,(LPARAM)hSmallIcon);
                  }

                  stop=1;
                  break;
                }
              }
            }
            if(stop)break;
          }
          Sleep(100);
        }
      }
      else
      {
        WaitForSingleObject(pi.hProcess,INFINITE);
      }
      /* SKV$*/
    }
    else if(ExecutorType && !SeparateWindow)//!(GUIType&2))// && AlwaysWaitFinish)
    {
      // �������� 800 ��, ����� ������... ����...
      // ��� �������� ��������� ��� - ��� ����� ������ (���� ���������!)
      WaitForSingleObject(pi.hProcess,INFINITE);//INFINITE);
    }

    GetExitCodeProcess(pi.hProcess,(LPDWORD)&ExitCode);
    if (SeparateWindow!=2)
      CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    {
      CONSOLE_SCREEN_BUFFER_INFO csbi;
      GetVideoMode(csbi);
    }
    ScrBuf.FillBuf();
    ScrBuf.SetLockCount(PrevLockCount);
  }
  else
  {
    //if (SeparateWindow!=2)
    {
      char OutStr[2048];

      SetLastError(_LastErrCode);
      //Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),MSG(MCannotExecute),
      //        SeparateWindow==2 ? CmdPtr:ExecLine,MSG(MOk));
      //        ^^^^^^^^^^^^^^^^^ �����? ��� ������� �� �������� - ��. ����
      if(Opt.ExecuteShowErrorMessage)
      {
        SetMessageHelp("ErrCannotExecute");
        // BugZ#993 - ���������� ��������� ����� ����
        strncpy(OutStr,CmdPtr,sizeof(OutStr)-1);
        Unquote(OutStr);
        TruncPathStr(OutStr,ScrX-15);
        Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),MSG(MCannotExecute),OutStr,MSG(MOk));
      }
      else
      {
        sprintf(OutStr,MSG(MExecuteErrorMessage),CmdPtr);
        char *PtrStr=FarFormatText(OutStr,ScrX,OutStr,sizeof(OutStr),"\n",0);
        printf(PtrStr);
        ScrBuf.FillBuf();
      }

    }
    ExitCode=-1;
    //ScrBuf.FillBuf();
    //ScrBuf.SetLockCount(PrevLockCount);
  }
  SetFarConsoleMode(TRUE);
  /* $ 05.10.2001 IS
     - ��������
     + �������������� ��������� �������, �.�. SetCursorType ������ �� �������
       ���������� ����� �����������, ������� � ������ ������ ������� �����.
  */
  SetCursorType(Visible,Size);
  SetRealCursorType(Visible,Size);
  /* IS $ */
  if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS &&
      WinVer.dwBuildNumber<=0x4000457)
    WriteInput(VK_F16,SKEY_VK_KEYS);
  /* ���� ���� �������� ������� �������, ��������
     mode con lines=50 cols=100
     �� ��� �� ���� �� ��������� ������� �������.
     ��� ����� ���� ���� ��������� ������ ��� :-)
  */
  GenerateWINDOW_BUFFER_SIZE_EVENT(-1,-1);

  SetConsoleTitle(OldTitle);

  if(Opt.RestoreCPAfterExecute)
  {
    // ����������� CP-������� ����� ���������� �����
    SetConsoleCP(ConsoleCP);
    SetConsoleOutputCP(ConsoleOutputCP);
  }
  return(ExitCode);
}

int CommandLine::CmdExecute(char *CmdLine,int AlwaysWaitFinish,
                            int SeparateWindow,int DirectRun)
{
  LastCmdPartLength=-1;
  if (!SeparateWindow && CtrlObject->Plugins.ProcessCommandLine(CmdLine))
  {
    /* $ 12.05.2001 DJ
       �������� ������ ���� �������� ������� �������
    */
    if (CtrlObject->Cp()->IsTopFrame())
    {
      CmdStr.SetString("");
      GotoXY(X1,Y1);
      mprintf("%*s",X2-X1+1,"");
      Show();
      ScrBuf.Flush();
    }
    return(-1);
  }
  int Code;
  /* 21.11.2001 VVM
    ! � ��������� ��� �������� � ����������� ����.
      ����� �� ������ ������� ����� :) */
  {
    CONSOLE_SCREEN_BUFFER_INFO sbi0,sbi1;
    GetConsoleScreenBufferInfo(hConOut,&sbi0);
    {
      RedrawDesktop Redraw(TRUE);

      ScrollScreen(1);
      MoveCursor(X1,Y1);
      if (CurDir[0] && CurDir[1]==':')
        FarChDir(CurDir);
      CmdStr.SetString("");
      if ((Code=ProcessOSCommands(CmdLine,SeparateWindow)) == TRUE)
        Code=-1;
      else
      {
        char TempStr[2048];
        strncpy(TempStr,CmdLine,sizeof(TempStr)-1);
        if(Code == -1)
          ReplaceStrings(TempStr,"/","\\",-1);
        Code=Execute(TempStr,AlwaysWaitFinish,SeparateWindow,DirectRun);
      }

      GetConsoleScreenBufferInfo(hConOut,&sbi1);
      if(!(sbi0.dwSize.X == sbi1.dwSize.X && sbi0.dwSize.Y == sbi1.dwSize.Y))
        CtrlObject->CmdLine->CorrectRealScreenCoord();

      //if(Code != -1)
      {
        int CurX,CurY;
        GetCursorPos(CurX,CurY);
        if (CurY>=Y1-1)
          ScrollScreen(Min(CurY-Y1+2,2/*Opt.ShowKeyBar ? 2:1*/));
      }
    }
    CtrlObject->Cp()->LeftPanel->UpdateIfChanged(UIC_UPDATE_FORCE);
    CtrlObject->Cp()->RightPanel->UpdateIfChanged(UIC_UPDATE_FORCE);
    CtrlObject->Cp()->Redraw();
  }
  /* VVM $ */
  ScrBuf.Flush();
  return(Code);
}


/* 20.03.2002 IS
   "if [not] exist" ������ ������ � ������� ������
   PrepareOSIfExist ������ ��������� � ���������� const
*/
/* $ 14.01.2001 SVS
   + � ProcessOSCommands ��������� ���������
     "IF [NOT] EXIST filename command"
     "IF [NOT] DEFINED variable command"

   ��� ������� ������������� ��� ��������� ���������� IF`�
   CmdLine - ������ ������ ����
     if exist file if exist file2 command
   Return - ��������� �� "command"
            ������ ������ - ������� �� ���������
            NULL - �� ������� "IF" ��� ������ � �����������, ��������
                   �� exist, � exist ��� ����������� �������.

   DEFINED - ������� EXIST, �� ��������� � ����������� �����

   �������� ������ (CmdLine) �� ��������������!!! - �� ��� ���� ��������� const
                                                    IS 20.03.2002 :-)
*/
const char* WINAPI PrepareOSIfExist(const char *CmdLine)
{
  if(!CmdLine || !*CmdLine)
    return NULL;

  char Cmd[1024];
  const char *PtrCmd=CmdLine, *CmdStart;
  int Not=FALSE;
  int Exist=0; // ������� ������� ����������� "IF [NOT] EXIST filename command"
               // > 0 - ���� ����� �����������

  /* $ 25.04.2001 DJ
     ��������� @ � IF EXIST
  */
  if (*PtrCmd == '@')
  {
    // ����� @ ������������; �� ������� � ���������� ����� �������
    // ExtractIfExistCommand � filetype.cpp
    PtrCmd++;
    while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd;
  }
  /* DJ $ */
  while(1)
  {
    if (!PtrCmd || !*PtrCmd || memicmp(PtrCmd,"IF ",3))
      break;

    PtrCmd+=3; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;

    if (memicmp(PtrCmd,"NOT ",4)==0)
    {
      Not=TRUE;
      PtrCmd+=4; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;
    }

    if (*PtrCmd && !memicmp(PtrCmd,"EXIST ",6))
    {
      PtrCmd+=6; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;
      CmdStart=PtrCmd;

      /* $ 25.04.01 DJ
         ��������� ������� ������ ����� ����� � IF EXIST
      */
      BOOL InQuotes=FALSE;
      while (*PtrCmd)
      {
        if (*PtrCmd == '\"')
          InQuotes = !InQuotes;
        else if (*PtrCmd == ' ' && !InQuotes)
          break;
        PtrCmd++;
      }

      if(PtrCmd && *PtrCmd && *PtrCmd == ' ')
      {
        char ExpandedStr[8192];
        memmove(Cmd,CmdStart,PtrCmd-CmdStart+1);
        Cmd[PtrCmd-CmdStart]=0;
        Unquote(Cmd);
//_SVS(SysLog("Cmd='%s'",Cmd));
        if (ExpandEnvironmentStr(Cmd,ExpandedStr,sizeof(ExpandedStr))!=0)
        {
          char FullPath[8192]="";
          if(!(Cmd[1] == ':' || (Cmd[0] == '\\' && Cmd[1]=='\\') || ExpandedStr[1] == ':' || (ExpandedStr[0] == '\\' && ExpandedStr[1]=='\\')))
          {
            if(CtrlObject)
              CtrlObject->CmdLine->GetCurDir(FullPath);
            else
              FarGetCurDir(sizeof(FullPath),FullPath);
            AddEndSlash(FullPath);
          }
          strcat(FullPath,ExpandedStr);
          DWORD FileAttr=-1;
          if(strpbrk(ExpandedStr,"*?")) // ��� �����?
          {
            WIN32_FIND_DATA wfd;
            HANDLE hFile=FindFirstFile(FullPath, &wfd);
            if(hFile!=INVALID_HANDLE_VALUE)
            {
              FileAttr=wfd.dwFileAttributes;
              FindClose(hFile);
            }
          }
          else
          {
            ConvertNameToFull(FullPath, FullPath, sizeof(FullPath));
            FileAttr=GetFileAttributes(FullPath);
          }
//_SVS(SysLog("%08X FullPath=%s",FileAttr,FullPath));
          if(FileAttr != (DWORD)-1 && !Not || FileAttr == (DWORD)-1 && Not)
          {
            while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd;
            Exist++;
          }
          else
            return "";
        }
      }
      /* DJ $ */
    }
    // "IF [NOT] DEFINED variable command"
    else if (*PtrCmd && !memicmp(PtrCmd,"DEFINED ",8))
    {
      PtrCmd+=8; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;
      CmdStart=PtrCmd;
      if(*PtrCmd == '"')
        PtrCmd=strchr(PtrCmd+1,'"');

      if(PtrCmd && *PtrCmd)
      {
        PtrCmd=strchr(PtrCmd,' ');
        if(PtrCmd && *PtrCmd && *PtrCmd == ' ')
        {
          char ExpandedStr[8192];
          memmove(Cmd,CmdStart,PtrCmd-CmdStart+1);
          Cmd[PtrCmd-CmdStart]=0;
          DWORD ERet=GetEnvironmentVariable(Cmd,ExpandedStr,sizeof(ExpandedStr));
//_SVS(SysLog(Cmd));
          if(ERet && !Not || !ERet && Not)
          {
            while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd;
            Exist++;
          }
          else
            return "";
        }
      }
    }
  }
  return Exist?PtrCmd:NULL;
}
/* SVS $ */
/* IS $ */

int CommandLine::ProcessOSCommands(char *CmdLine,int SeparateWindow)
{
  Panel *SetPanel;
  int Length;

  SetPanel=CtrlObject->Cp()->ActivePanel;

  if (SetPanel->GetType()!=FILE_PANEL && CtrlObject->Cp()->GetAnotherPanel(SetPanel)->GetType()==FILE_PANEL)
    SetPanel=CtrlObject->Cp()->GetAnotherPanel(SetPanel);

  RemoveTrailingSpaces(CmdLine);

  if (!SeparateWindow && isalpha(CmdLine[0]) && CmdLine[1]==':' && CmdLine[2]==0)
  {
    char NewDir[10];
    sprintf(NewDir,"%c:",toupper(CmdLine[0]));
    FarChDir(CmdLine);
    if (getdisk()!=NewDir[0]-'A')
    {
      strcat(NewDir,"\\");
      FarChDir(NewDir);
    }
    SetPanel->ChangeDirToCurrent();
    return(TRUE);
  }

  // SET [����������=[������]]
  if (strnicmp(CmdLine,"SET ",4)==0)
  {
    char Cmd[1024];
#if 0
    // ������� ��� "SET /P variable=[promptString]"
    int Offset=4, NeedInput=FALSE;
    char *ParamP=strchr(CmdLine,'/');
    if (ParamP && (ParamP[1] == 'P' || ParamP[1] == 'p') && ParamP[2] == ' ')
    {
      Offset=ParamP-CmdLine+3;
      NeedInput=TRUE;
    }

    strncpy(Cmd,CmdLine+Offset,sizeof(Cmd)-1);

    char *Value=strchr(Cmd,'=');
    if (Value==NULL)
      return(FALSE);

    *Value=0;
    if(NeedInput)
    {
      Offset=Value-Cmd+1;
      if(!::GetString("",Value+1,"PromptSetEnv","",Value+1,sizeof(Cmd)-Offset-1,NULL,FIB_ENABLEEMPTY))
        return TRUE;
    }
#else
    strncpy(Cmd,CmdLine+4,sizeof(Cmd)-1);
    char *Value=strchr(Cmd,'=');
    if (Value==NULL)
      return(FALSE);

    *Value=0;
#endif
    FAR_OemToChar(Cmd, Cmd);

    if (Value[1]==0)
      SetEnvironmentVariable(Cmd,NULL);
    else
    {
      char ExpandedStr[8192];
      /* $ 17.06.2001 IS
         ! ��������� ExpandEnvironmentStr, �.�. ��� ��������� �������� �
           �������� �������.
         + ������������ ������ ����� SetEnvironmentVariable �� OEM � ANSI
      */
      if (ExpandEnvironmentStr(Value+1,ExpandedStr,sizeof(ExpandedStr))!=0)
      {
        // ���������� ��������� ������ ���� � ANSI???
        FAR_OemToChar(ExpandedStr, ExpandedStr);
        SetEnvironmentVariable(Cmd,ExpandedStr);
      }
      /* IS $ */
    }
    return(TRUE);
  }

  if (!memicmp(CmdLine,"REM ",4) || !memicmp(CmdLine,"::",2))
  {
    return TRUE;
  }

  if (!memicmp(CmdLine,"CLS",3))
  {
    if(CmdLine[3])
      return FALSE;

    ClearScreen(F_LIGHTGRAY|B_BLACK);
    return TRUE;
  }

  /*
  Displays or sets the active code page number.
  CHCP [nnn]
    nnn   Specifies a code page number (Dec or Hex).
  Type CHCP without a parameter to display the active code page number.
  */
  if (!memicmp(CmdLine,"CHCP",4))
  {
    if(CmdLine[4] == 0 || !(CmdLine[4] == ' ' || CmdLine[4] == '\t'))
      return(FALSE);

    char *Ptr=RemoveExternalSpaces(CmdLine+5), Chr;

    if(!isdigit(*Ptr))
      return FALSE;

    while((Chr=*Ptr) != 0)
    {
      if(!isdigit(Chr))
        break;
      ++Ptr;
    }
    UINT cp=(UINT)strtol(CmdLine+5,&Ptr,10);
    BOOL r1=SetConsoleCP(cp);
    BOOL r2=SetConsoleOutputCP(cp);
    if(r1 && r2) // ���� ��� ���, �� ���  �...
    {
      InitRecodeOutTable(cp);
      LocalUpperInit();
      InitLCIDSort();
      InitKeysArray();
      CtrlObject->Cp()->Redraw();
      ScrBuf.Flush();
      return TRUE;
    }
    else  // ��� ������ ������� chcp ���� ������ ;-)
     return FALSE;
  }

  /* $ 14.01.2001 SVS
     + � ProcessOSCommands ��������� ���������
       "IF [NOT] EXIST filename command"
       "IF [NOT] DEFINED variable command"
  */
  if (memicmp(CmdLine,"IF ",3)==0)
  {
    const char *PtrCmd=PrepareOSIfExist(CmdLine);
    // ����� PtrCmd - ��� ������� �������, ��� IF
    if(PtrCmd && *PtrCmd && CtrlObject->Plugins.ProcessCommandLine(PtrCmd))
    {
      CmdStr.SetString("");
      GotoXY(X1,Y1);
      mprintf("%*s",X2-X1+1,"");
      Show();
      return TRUE;
    }
    return FALSE;
  }
  /* SVS $ */

  /* $ 16.04.2002 DJ
     ���������� ���������, ���� ����� Shift-Enter
  */
  if (!SeparateWindow &&  /* DJ $ */
      (strnicmp(CmdLine,"CD",Length=2)==0 || strnicmp(CmdLine,"CHDIR",Length=5)==0) &&
      (IsSpace(CmdLine[Length]) || CmdLine[Length]=='\\' || TestParentFolderName(CmdLine+Length)))
  {
    int ChDir=(Length==5);

    while (IsSpace(CmdLine[Length]))
      Length++;

    if (CmdLine[Length]=='\"')
      Length++;

    char ExpandedDir[8192];
    strncpy(ExpandedDir,&CmdLine[Length],sizeof(ExpandedDir)-1);

    // ������������� ����� ����� �� "���������"
    if(ExpandedDir[1] == ':' && isalpha(ExpandedDir[0]))
      ExpandedDir[0]=toupper(ExpandedDir[0]);

    /* $ 15.11.2001 OT
      ������� ��������� ���� �� ����� "�������" ����������.
      ���� �� ���, �� ����� �������� ������, ��� ��� ���������� ���������
    */
    DWORD DirAtt=GetFileAttributes(ExpandedDir);
    if (DirAtt!=0xffffffff && (DirAtt & FILE_ATTRIBUTE_DIRECTORY) && PathMayBeAbsolute(ExpandedDir))
    {
      SetPanel->SetCurDir(ExpandedDir,TRUE);
      return TRUE;
    }
    /* OT $ */

    /* $ 20.09.2002 SKV
      ��� ��������� ����������� ��������� ����� ������� ���:
      cd net:server � cd ftp://server/dir
      ��� ��� ��� �� �� ������� �������� �
      cd s&r:, cd make: � �.�., ������� � �����
      �������� �� ����� �������� ���������.
    */
    /*
    if (CtrlObject->Plugins.ProcessCommandLine(ExpandedDir))
    {
      CmdStr.SetString("");
      GotoXY(X1,Y1);
      mprintf("%*s",X2-X1+1,"");
      Show();
      return(TRUE);
    }
    */
    /* SKV $ */

    char *ChPtr=strrchr(ExpandedDir,'\"');
    if (ChPtr!=NULL)
      *ChPtr=0;

    if (SetPanel->GetType()==FILE_PANEL && SetPanel->GetMode()==PLUGIN_PANEL)
    {
      SetPanel->SetCurDir(ExpandedDir,ChDir);
      return(TRUE);
    }

    if (ExpandEnvironmentStr(ExpandedDir,ExpandedDir,sizeof(ExpandedDir))!=0)
    {
      if(CheckFolder(ExpandedDir) <= CHKFLD_NOTACCESS)
        return -1;

      if (!FarChDir(ExpandedDir))
        return(FALSE);
    }

    SetPanel->ChangeDirToCurrent();
    if (!SetPanel->IsVisible())
      SetPanel->SetTitle();

    return(TRUE);
  }
  return(FALSE);
}
