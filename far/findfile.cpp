/*
findfile.cpp

����� (Alt-F7)

*/

/* Revision: 1.97 04.03.2002 $ */

/*
Modify:
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

#define DLG_HEIGHT 21
#define MAX_READ 0x20000

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
static int SearchMode,CmpCase,WholeWords,UseAllTables,SearchInArchives;
/* KM $ */
static int FindFoldersChanged;
static int DlgWidth,DlgHeight;
static volatile int StopSearch,PauseSearch,SearchDone,LastFoundNumber,FindFileCount,FindDirCount,WriteDataUsed;
static char FindMessage[200],LastDirName[2*NM];
static int FindMessageReady,FindCountReady;
static char PluginSearchPath[2*NM];
static HANDLE hDlg;
static int RecurseLevel;
static int BreakMainThread;
static int PluginMode;

static HANDLE hDialogMutex, hPluginMutex;

static int UseDecodeTable=FALSE,UseUnicode=FALSE,TableNum=0;
static struct CharTableSet TableSet;

/* $ 01.07.2001 IS
   ������ "����� ������". ������ ��� ����� ������������ ��� �������� �����
   ����� �� ���������� � �������.
*/
static CFileMask FileMaskForFindFile;
/* IS $ */

int _cdecl SortItems(const void *p1,const void *p2)
{
  PluginPanelItem *Item1=(PluginPanelItem *)p1;
  PluginPanelItem *Item2=(PluginPanelItem *)p2;
  char n1[NM*2],n2[NM*2];
  n1[0]=0;n2[0]=0;
  if (*Item1->FindData.cFileName)
    strncpy(n1,Item1->FindData.cFileName, sizeof(n1)-1);
  if (*Item2->FindData.cFileName)
    strncpy(n2,Item2->FindData.cFileName, sizeof(n1)-1);
  *(PointToName(n1))=0;
  *(PointToName(n2))=0;
  return LocalStricmp(n1,n2);
}

long WINAPI FindFiles::MainDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  Dialog* Dlg=(Dialog*)hDlg;
  char *FindText=MSG(MFindFileText),*FindCode=MSG(MFindFileCodePage);
  char DataStr[NM];

  switch(Msg)
  {
    case DN_INITDIALOG:
    {
      unsigned int W=Dlg->Item[6].X1-Dlg->Item[4].X1-5;
      if (strlen(FindText)>W)
      {
        strncpy(DataStr,FindText,W-3);
        DataStr[W-4]=0;
        strcat(DataStr,"...");
      }
      else
        strncpy(DataStr,FindText,sizeof(DataStr)-1);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,4,(long)DataStr);

      W=Dlg->Item[0].X2-Dlg->Item[6].X1-3;
      if (strlen(FindCode)>W)
      {
        strncpy(DataStr,FindCode,W-3);
        DataStr[W-4]=0;
        strcat(DataStr,"...");
      }
      else
        strncpy(DataStr,FindCode,sizeof(DataStr)-1);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,6,(long)DataStr);

      if (UseAllTables)
        strncpy(TableSet.TableName,MSG(MFindFileAllTables),sizeof(TableSet.TableName)-1);
      else if (UseUnicode)
        strncpy(TableSet.TableName,"Unicode",sizeof(TableSet.TableName)-1);
      else if (!UseDecodeTable)
        strncpy(TableSet.TableName,MSG(MGetTableNormalText),sizeof(TableSet.TableName)-1);
      else
        PrepareTable(&TableSet,TableNum);
      Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,7,(long)TableSet.TableName);

      FindFoldersChanged = FALSE;

      return TRUE;
    }
    case DN_LISTCHANGE:
    {
      if (Param1==7)
      {
        UseAllTables=(Param2==0);
        UseUnicode=(Param2==3);
        UseDecodeTable=(Param2>=5);
        if (!UseAllTables)
        {
          strncpy(TableSet.TableName,MSG(MGetTableNormalText),sizeof(TableSet.TableName)-1);
          if (Param2>=5)
          {
            PrepareTable(&TableSet,Param2-5);
            TableNum=Param2-5;
          }
        }
      }
      return TRUE;
    }
    /* 22.11.2001 VVM
      ! ������������������� FindFolders ��� ����� ������.
        �� ������ ���� �� ������ ���� ��������� ������� */
    case DN_BTNCLICK:
    {
      /* $ 23.11.2001 KM
         - ����! ������ ���� � ���� ������� :-(
           ��������� �������� �������� � �����������.
      */
      /* $ 22.11.2001 KM
         - ������ ������, ��� DN_BTNCLICK �������� �� ����� "�������������"
           ����������, ������ � �������� ������� ��-�������, ������� �������
           ������� ENTER �� ������� Find ��� Cancel �� � ���� �� ���������.
      */
      if (Param1==22 || Param1==23) // [ Find ] ��� [ Cancel ]
        return FALSE;
      else if (Param1==13)
        FindFoldersChanged = TRUE;
      return TRUE;
      /* KM $ */
      /* KM $ */
    }
    case DN_EDITCHANGE:
    {
      if ((Param1==5) && (!FindFoldersChanged))
      // ������ "���������� �����"
      {
        FarDialogItem &Item=*reinterpret_cast<FarDialogItem*>(Param2);
        BOOL Checked = (*Item.Data.Data)?FALSE:Opt.FindFolders;
        if (Checked)
          Dialog::SendDlgMessage(hDlg, DM_SETCHECK, 13, BSTATE_CHECKED);
        else
          Dialog::SendDlgMessage(hDlg, DM_SETCHECK, 13, BSTATE_UNCHECKED);
      }
      return TRUE;
    }
    /* VVM $ */
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}


FindFiles::FindFiles()
{
  static char LastFindMask[NM]="*.*",LastFindStr[SEARCHSTRINGBUFSIZE];
  /* $ 30.07.2000 KM
     ��������� ���������� LastWholeWords ��� ������ �� ������� ����������
  */
  static int LastCmpCase=0,LastWholeWords=0,LastUseAllTables=0,LastSearchInArchives=0;
  /* KM $ */
  CmpCase=LastCmpCase;
  WholeWords=LastWholeWords;
  UseAllTables=LastUseAllTables;
  SearchInArchives=LastSearchInArchives;
  SearchMode=Opt.FileSearchMode;
  strncpy(FindMask,LastFindMask,sizeof(FindMask)-1);
  strncpy(FindStr,LastFindStr,sizeof(FindStr)-1);
  BreakMainThread=0;
  FarList TableList;
  FarListItem *TableItem=(FarListItem *)malloc(sizeof(FarListItem)*4);
  TableList.Items=TableItem;
  TableList.ItemsNumber=4;

  memset(TableItem,0,sizeof(FarListItem)*4);
  strncpy(TableItem[0].Text,MSG(MFindFileAllTables),sizeof(TableItem[0].Text)-1);
  TableItem[1].Flags=LIF_SEPARATOR;
  strncpy(TableItem[2].Text,MSG(MGetTableNormalText),sizeof(TableItem[2].Text)-1);
  strncpy(TableItem[3].Text,"Unicode",sizeof(TableItem[3].Text)-1);

  for (int I=0;;I++)
  {
    CharTableSet cts;
    int RetVal=FarCharTable(I,(char *)&cts,sizeof(cts));
    if (RetVal==-1)
      break;

    if (I==0)
    {
      TableItem=(FarListItem *)realloc(TableItem,sizeof(FarListItem)*5);
      if (TableItem==NULL)
        return;
      memset(&TableItem[4],0,sizeof(FarListItem));
      TableItem[4].Flags=LIF_SEPARATOR;
      TableList.Items=TableItem;
      TableList.ItemsNumber++;
    }

    TableItem=(FarListItem *)realloc(TableItem,sizeof(FarListItem)*(I+6));
    if (TableItem==NULL)
      return;
    memset(&TableItem[I+5],0,sizeof(FarListItem));
    strncpy(TableItem[I+5].Text,cts.TableName,sizeof(TableItem[I+5].Text)-1);
    TableList.Items=TableItem;
    TableList.ItemsNumber++;
  }

  FindList = NULL;
  ArcList = NULL;
  hPluginMutex=CreateMutex(NULL,FALSE,NULL);

  do
  {
    ClearAllLists();
    const char *MasksHistoryName="Masks",*TextHistoryName="SearchText";
    /* $ 30.07.2000 KM
       �������� ����� checkbox "Whole words" � ������ ������
    */
    static struct DialogData FindAskDlgData[]=
    {
      /* 00 */DI_DOUBLEBOX,3,1,72,18,0,0,0,0,(char *)MFindFileTitle,
      /* 01 */DI_TEXT,5,2,0,0,0,0,0,0,(char *)MFindFileMasks,
      /* 02 */DI_EDIT,5,3,70,16,1,(DWORD)MasksHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,0,"",
      /* 03 */DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR2,0,"",
      /* 04 */DI_TEXT,5,5,0,0,0,0,0,0,"",
      /* 05 */DI_EDIT,5,6,36,16,0,(DWORD)TextHistoryName,DIF_HISTORY,0,"",
      /* 06 */DI_TEXT,40,5,0,0,0,0,0,0,"",
      /* 07 */DI_COMBOBOX,40,6,70,10,0,(DWORD)&TableList,DIF_DROPDOWNLIST,0,"",
      /* 08 */DI_TEXT,3,7,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 09 */DI_VTEXT,38,4,0,0,0,0,DIF_BOXCOLOR,0,"\xD1\xB3\xB3\xC1",
      /* 10 */DI_CHECKBOX,5,8,0,0,0,0,0,0,(char *)MFindFileCase,
      /* 11 */DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MFindFileWholeWords,
      /* 12 */DI_CHECKBOX,40,8,0,0,0,0,0,0,(char *)MFindArchives,
      /* 13 */DI_CHECKBOX,40,9,0,0,0,0,0,0,(char *)MFindFolders,
      /* 14 */DI_TEXT,3,10,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR2,0,"",
      /* 15 */DI_VTEXT,38,7,0,0,0,0,DIF_BOXCOLOR,0,"\xC5\xB3\xB3\xCF",
      /* 16 */DI_RADIOBUTTON,5,11,0,0,0,0,DIF_GROUP,0,(char *)MSearchAllDisks,
      /* 17 */DI_RADIOBUTTON,5,12,0,0,0,1,0,0,(char *)MSearchFromRoot,
      /* 18 */DI_RADIOBUTTON,5,13,0,0,0,0,0,0,(char *)MSearchFromCurrent,
      /* 19 */DI_RADIOBUTTON,5,14,0,0,0,0,0,0,(char *)MSearchInCurrent,
      /* 20 */DI_RADIOBUTTON,5,15,0,0,0,0,0,0,(char *)MSearchInSelected,
      /* 21 */DI_TEXT,3,16,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
      /* 22 */DI_BUTTON,0,17,0,0,0,0,DIF_CENTERGROUP,1,(char *)MFindFileFind,
      /* 23 */DI_BUTTON,0,17,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
    };
    /* KM $ */
    MakeDialogItems(FindAskDlgData,FindAskDlg);

    {
      Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
      PluginMode=ActivePanel->GetMode()==PLUGIN_PANEL && ActivePanel->IsVisible();

      if (PluginMode)
      {
        struct OpenPluginInfo Info;
        HANDLE hPlugin=ActivePanel->GetPluginHandle();
        CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
        /* $ 14.05.2001 DJ
           ���������, � �� ������
        */
        if ((Info.Flags & OPIF_REALNAMES)==0)
          FindAskDlg[12].Flags |= DIF_DISABLE;
        /* DJ $ */
      }
    }

    strncpy(FindAskDlg[2].Data,FindMask,sizeof(FindAskDlg[2].Data)-1);
    strncpy(FindAskDlg[5].Data,FindStr,sizeof(FindAskDlg[5].Data)-1);
    FindAskDlg[10].Selected=CmpCase;
    FindAskDlg[11].Selected=WholeWords;

    /* $ 14.05.2001 DJ
       �� �������� �������, ���� ������ ������ � �������
    */
    if (!(FindAskDlg[12].Flags & DIF_DISABLE))
      FindAskDlg[12].Selected=SearchInArchives;
    /* DJ $ */
    if (!*FindStr)
      FindAskDlg[13].Selected=Opt.FindFolders;
    FindAskDlg[16].Selected=FindAskDlg[17].Selected=0;
    FindAskDlg[18].Selected=FindAskDlg[19].Selected=0;
    FindAskDlg[20].Selected=0;
    FindAskDlg[16+SearchMode].Selected=1;

    while (1)
    {
      int ExitCode;
      {
        Dialog Dlg(FindAskDlg,sizeof(FindAskDlg)/sizeof(FindAskDlg[0]),MainDlgProc);

        Dlg.SetHelp("FindFile");
        Dlg.SetPosition(-1,-1,76,20);
        Dlg.Process();
        ExitCode=Dlg.GetExitCode();
      }
      if (ExitCode!=22)
      {
        free(TableItem);
        return;
      }
      /* $ 01.07.2001 IS
         �������� ����� �� ������������
      */
      if(!*FindAskDlg[2].Data)             // ���� ������ � ������� �����,
         strcpy(FindAskDlg[2].Data, "*"); // �� �������, ��� ����� ���� "*"

      if(FileMaskForFindFile.Set(FindAskDlg[2].Data, FMF_ADDASTERISK))
           break;
      /* IS $ */
    }
    /* $ 14.12.2000 OT */
    char Buf1 [24];
    char Buf2 [128];
    if (strlen (FindAskDlg[2].Data) > sizeof(FindMask) ){
      memset (Buf1, 0, sizeof(Buf1));
      memset (Buf2, 0, sizeof(Buf2));
      strncpy (Buf1, MSG(MFindFileMasks), sizeof(Buf1)-1);
      sprintf (Buf2,MSG(MEditInputSize), Buf1, sizeof(FindMask)-1);
      Message(MSG_WARNING,1,MSG(MWarning),
        Buf2,
        MSG(MOk));
    }
    strncpy(FindMask,*FindAskDlg[2].Data ? FindAskDlg[2].Data:"*",sizeof(FindMask)-1);
    if (strlen (FindAskDlg[5].Data) > sizeof(FindStr) ){
      memset (Buf1, 0, sizeof(Buf1));
      memset (Buf2, 0, sizeof(Buf2));
      strncpy (Buf1, MSG(MFindFileText), sizeof(Buf1)-1);
      RemoveHighlights(Buf1);
      sprintf (Buf2,MSG(MEditInputSize), Buf1, sizeof(FindStr)-1);
      Message(MSG_WARNING,1,MSG(MWarning),
        Buf2,
        MSG(MOk));
    }
    strncpy(FindStr,FindAskDlg[5].Data,sizeof(FindStr)-1);
    /* OT $ */
    CmpCase=FindAskDlg[10].Selected;
    /* $ 30.07.2000 KM
       ��������� ����������
    */
    WholeWords=FindAskDlg[11].Selected;
    /* KM $ */
    SearchInArchives=FindAskDlg[12].Selected;
    if (FindFoldersChanged)
      Opt.FindFolders=FindAskDlg[13].Selected;
    if (*FindStr)
    {
      strncpy(GlobalSearchString,FindStr,sizeof(GlobalSearchString)-1);
      GlobalSearchCase=CmpCase;
      /* $ 30.07.2000 KM
         ��������� ����������
      */
      GlobalSearchWholeWords=WholeWords;
      /* KM $ */
    }
    if (FindAskDlg[16].Selected)
      SearchMode=SEARCH_ALL;
    if (FindAskDlg[17].Selected)
      SearchMode=SEARCH_ROOT;
    if (FindAskDlg[18].Selected)
      SearchMode=SEARCH_FROM_CURRENT;
    if (FindAskDlg[19].Selected)
      SearchMode=SEARCH_CURRENT_ONLY;
    if (FindAskDlg[20].Selected)
        SearchMode=SEARCH_SELECTED;
    Opt.FileSearchMode=SearchMode;
    LastCmpCase=CmpCase;
    /* $ 30.07.2000 KM
       ��������� ����������
    */
    LastWholeWords=WholeWords;
    /* KM $ */
    LastUseAllTables=UseAllTables;
    LastSearchInArchives=SearchInArchives;
    strncpy(LastFindMask,FindMask,sizeof(LastFindMask)-1);
    strncpy(LastFindStr,FindStr,sizeof(LastFindStr)-1);
    if (*FindStr)
      Editor::SetReplaceMode(FALSE);
  } while (FindFilesProcess());
  CloseHandle(hPluginMutex);
  free(TableItem);
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
}

int FindFiles::GetPluginFile(DWORD ArcIndex, struct PluginPanelItem *PanelItem,
                             char *DestPath, char *ResultName)
{
  HANDLE hPlugin = ArcList[ArcIndex].hPlugin;
  char SaveDir[NM];
  struct OpenPluginInfo Info;
  CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
  strncpy(SaveDir,Info.CurDir,sizeof(SaveDir)-1);
  AddEndSlash(SaveDir);

  CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_SILENT|OPM_FIND);
  SetPluginDirectory(ArcList[ArcIndex].RootPath, hPlugin);
  SetPluginDirectory(PanelItem->FindData.cFileName, hPlugin);

  PluginPanelItem NewItem = *PanelItem;
  char *FileName = PointToName(NewItem.FindData.cFileName);
  if (FileName != NewItem.FindData.cFileName)
    strncpy(NewItem.FindData.cFileName, FileName, sizeof(NewItem.FindData.cFileName));
  int Result = CtrlObject->Plugins.GetFile(hPlugin,&NewItem,DestPath,ResultName,OPM_SILENT|OPM_FIND);

  CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_SILENT|OPM_FIND);
  SetPluginDirectory(ArcList[ArcIndex].RootPath, hPlugin);
  SetPluginDirectory(SaveDir, hPlugin);
  return(Result);
}

long WINAPI FindFiles::FindDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  Dialog* Dlg=(Dialog*)hDlg;
  VMenu *ListBox=Dlg->Item[1].ListPtr;

  switch(Msg)
  {
    case DN_HELP: // � ������ ������ �������� ����� ������, ���... ��������� � �����������
      return !SearchDone?NULL:Param2;

    case DN_MOUSECLICK:
    {
      SMALL_RECT drect,rect;
      Dialog::SendDlgMessage(hDlg,DM_GETDLGRECT,0,(long)&drect);
      Dialog::SendDlgMessage(hDlg,DM_GETITEMPOSITION,1,(long)&rect);
      if (Param1==1 && ((MOUSE_EVENT_RECORD *)Param2)->dwMousePosition.X<drect.Left+rect.Right)
      {
        if (ListBox && ListBox->GetItemCount())
          Dialog::SendDlgMessage(hDlg,DM_CLOSE,6/* [ Go to ] */,0);
        FindDlgProc(hDlg,DN_BTNCLICK,6,0); // emulates a [ Go to ] button pressing
        return TRUE;
      }
      return FALSE;
    }

    case DN_KEY:
    {
      WaitForSingleObject(hDialogMutex,INFINITE);

      if (!StopSearch && Param2==KEY_ESC)
      {
        PauseSearch=TRUE;
        IsProcessAssignMacroKey++; // �������� ���� �������
                                   // �.�. � ���� ������� ������ ������ Alt-F9!
        int LocalRes=TRUE;
		/* $ 04.03.2002 DJ
		   ������� Esc � ������� confirm Esc ����� _��_ ���������
		*/
        if(Opt.Confirm.Esc && Message(MSG_WARNING,2,MSG(MKeyESCWasPressed),
                      MSG(MDoYouWantToStopWork),MSG(MYes),MSG(MNo))!=0)
          LocalRes=FALSE;
		/* DJ $ */
        IsProcessAssignMacroKey--;
        PauseSearch=FALSE;
        StopSearch=LocalRes;
        ReleaseMutex(hDialogMutex);
        return TRUE;
      }

      if (!ListBox)
      {
        ReleaseMutex(hDialogMutex);
        return TRUE;
      }

      while (ListBox->GetCallCount())
        Sleep(10);

      // �������� ����.������� ����� �����������.
      if(Param2 == KEY_CTRLALTSHIFTPRESS || Param2 == KEY_ALTF9)
      {
        IsProcessAssignMacroKey--;
        FrameManager->ProcessKey(Param2);
        IsProcessAssignMacroKey++;
        ReleaseMutex(hDialogMutex);
        return TRUE;
      }

      if (Param1==9 && (Param2==KEY_RIGHT || Param2==KEY_TAB)) // [ Stop ] button
      {
//        while (ListBox->GetCallCount())
//          Sleep(10);
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,5/* [ New search ] */,0);
        ReleaseMutex(hDialogMutex);
        return TRUE;
      }
      else if (Param1==5 && (Param2==KEY_LEFT || Param2==KEY_SHIFTTAB)) // [ New search ] button
      {
//        while (ListBox->GetCallCount())
//          Sleep(10);
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,9/* [ Stop ] */,0);
        ReleaseMutex(hDialogMutex);
        return TRUE;
      }
      else if (Param2==KEY_UP || Param2==KEY_DOWN || Param2==KEY_PGUP ||
               Param2==KEY_PGDN || Param2==KEY_HOME || Param2==KEY_END ||
               Param2==KEY_MSWHEEL_UP || Param2==KEY_MSWHEEL_DOWN)
      {
//        while (ListBox->GetCallCount())
//          Sleep(10);
        ListBox->ProcessKey(Param2);
        ReleaseMutex(hDialogMutex);
        return TRUE;
      }
      else if (Param2==KEY_F3 || Param2==KEY_NUMPAD5 || Param2==KEY_F4)
      {
        if (ListBox->GetItemCount()==0)
        {
          ReleaseMutex(hDialogMutex);
          return TRUE;
        }

        DWORD ItemIndex = (DWORD)ListBox->GetUserData(NULL, 0);
        if (ItemIndex != LIST_INDEX_NONE)
        {
          int RemoveTemp=FALSE;
          char SearchFileName[NM];
          char TempDir[NM];

          if (FindList[ItemIndex].FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
          {
            ReleaseMutex(hDialogMutex);
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
              char *Buffer=new char[MAX_READ];
              FILE *ProcessFile=fopen(FindArcName,"rb");
              if (ProcessFile==NULL)
              {
                delete[] Buffer;
                ReleaseMutex(hDialogMutex);
                return TRUE;
              }
              int ReadSize=fread(Buffer,1,MAX_READ,ProcessFile);
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
                ReleaseMutex(hDialogMutex);
                ArcList[FindList[ItemIndex].ArcIndex].hPlugin = INVALID_HANDLE_VALUE;
                return TRUE;
              }
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
              RemoveDirectory(TempDir);
              ReleaseMutex(hPluginMutex);
              ReleaseMutex(hDialogMutex);
              return FALSE;
            }
            else
              ReleaseMutex(hPluginMutex);
            RemoveTemp=TRUE;
          }
          else
            strncpy(SearchFileName,FindList[ItemIndex].FindData.cFileName,sizeof(SearchFileName)-1);

          DWORD FileAttr;
          if ((FileAttr=GetFileAttributes(SearchFileName))!=(DWORD)-1)
          {
            char OldTitle[512];
            GetConsoleTitle(OldTitle,sizeof(OldTitle));

            if (Param2==KEY_F3 || Param2==KEY_NUMPAD5)
            {
              NamesList ViewList;
              // ������� ��� �����, ������� ����� �������� �����...
              {
                int ListSize=ListBox->GetItemCount();
                DWORD Index;
                for (int I=0;I<ListSize;I++)
                {
                  Index = (DWORD)ListBox->GetUserData(NULL, 0, I);
                  if ((Index != LIST_INDEX_NONE) &&
                      ((FindList[Index].ArcIndex == LIST_INDEX_NONE) ||
                       (ArcList[FindList[Index].ArcIndex].Flags & OPIF_REALNAMES)))
                  {
                    int Length=strlen(FindList[Index].FindData.cFileName);
                    // �� ��������� ����� � ������� � OPIF_REALNAMES
                    if (Length>0 && !(FindList[Index].FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
                      ViewList.AddName(FindList[Index].FindData.cFileName);
                  } /* if */
                } /* for */
                ViewList.SetCurName(FindList[ItemIndex].FindData.cFileName);
              }
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,FALSE,0);
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
              ReleaseMutex(hDialogMutex);
              {
                FileViewer ShellViewer (SearchFileName,FALSE,FALSE,FALSE,-1,NULL,(FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE)?NULL:&ViewList);
                ShellViewer.SetDynamicallyBorn(FALSE);
                ShellViewer.SetEnableF6(TRUE);
                // FindFileArcIndex ������ ����� ������������
                // �� ����� ���� ��� ������.
                if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
                    (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
                  ShellViewer.SetSaveToSaveAs(TRUE);
                IsProcessVE_FindFile++;
                FrameManager->ExecuteModal ();
                IsProcessVE_FindFile--;
                // ���������� ���������� �����
                FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
              }
              WaitForSingleObject(hDialogMutex,INFINITE);

              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
            }
            else
            {
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,FALSE,0);
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);
              ReleaseMutex(hDialogMutex);
              {
                int FramePos=FrameManager->FindFrameByFile(MODALTYPE_EDITOR,SearchFileName);
                int SwitchTo=FALSE;
                if (FramePos!=-1)
                {
                  if (!(*FrameManager)[FramePos]->GetCanLoseFocus(TRUE) ||
                      Opt.Confirm.AllowReedit)
                  {
                    char MsgFullFileName[NM];
                    strncpy(MsgFullFileName,SearchFileName,sizeof(MsgFullFileName)-1);
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
                  FrameManager->ExecuteNonModal();
                  IsProcessVE_FindFile--;
                  // ���������� ���������� �����
                  FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
                }
                else
                {
                  FileEditor ShellEditor (SearchFileName,FALSE,FALSE);
                  ShellEditor.SetDynamicallyBorn(FALSE);
                  ShellEditor.SetEnableF6 (TRUE);
                  // FindFileArcIndex ������ ����� ������������
                  // �� ����� ���� ��� ������.
                  if ((FindList[ItemIndex].ArcIndex != LIST_INDEX_NONE) &&
                      (!(ArcList[FindList[ItemIndex].ArcIndex].Flags & OPIF_REALNAMES)))
                    ShellEditor.SetSaveToSaveAs(TRUE);
                  IsProcessVE_FindFile++;
                  FrameManager->ExecuteModal ();
                  IsProcessVE_FindFile--;
                  // ���������� ���������� �����
                  FrameManager->ProcessKey(KEY_CONSOLE_BUFFER_RESIZE);
                }
              }
              // � ��� ��� ����? �� ���� ��� ������ � DN_KEY ��� ���������...
              WaitForSingleObject(hDialogMutex,INFINITE);
              Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
              Dialog::SendDlgMessage(hDlg,DM_SHOWDIALOG,TRUE,0);
            }
            SetConsoleTitle(OldTitle);
          }
          if (RemoveTemp)
            DeleteFileWithFolder(SearchFileName);
        }
        ReleaseMutex(hDialogMutex);
        return TRUE;
      }
      ReleaseMutex(hDialogMutex);
      return FALSE;
    }
    case DN_BTNCLICK:
    {
      if (Param1==5) // [ New search ] button pressed
      {
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
          return FALSE;

        WaitForSingleObject(hDialogMutex,INFINITE);

        // ������� ����� ������ ��� �� ����� ������ �� �������.
        // ������� ������ ��� [ Panel ]
        if (ListBox->GetItemCount()==0)
        {
          ReleaseMutex(hDialogMutex);
          return (TRUE);
        }
        FindExitIndex = (DWORD)ListBox->GetUserData(NULL, 0);
        if (FindExitIndex != LIST_INDEX_NONE)
          FindExitCode = FIND_EXIT_GOTO;
        ReleaseMutex(hDialogMutex);
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
          return FALSE;

        WaitForSingleObject(hDialogMutex,INFINITE);

        // �� ������ ����� �������� �� � �������, � �����.
        // ����� ��������� ������. ����� �������� ��������, ����� ��
        // ���� �� ������, ����� �� ������� � ������� ����� (� �����-��
        // ����!) � � ���������� ��� ���������.
        if (ListBox->GetItemCount()==0)
        {
          ReleaseMutex(hDialogMutex);
          return (TRUE);
        }
        FindExitCode = FIND_EXIT_PANEL;
        FindExitIndex = (DWORD)ListBox->GetUserData(NULL, 0);
        ReleaseMutex(hDialogMutex);
        return (FALSE);
      }
    }
    case DN_CTLCOLORDLGLIST:
    {
      short ColorArray[10]=
      {
        COL_DIALOGMENUTEXT,
        COL_DIALOGMENUTEXT,
        COL_MENUTITLE,
        COL_DIALOGMENUTEXT,
        COL_DIALOGMENUHIGHLIGHT,
        COL_DIALOGMENUTEXT,
        COL_DIALOGMENUSELECTEDTEXT,
        COL_DIALOGMENUSELECTEDHIGHLIGHT,
        COL_DIALOGMENUSCROLLBAR,
        COL_DIALOGMENUTEXT
      };
      if (Param2)
        for (int I=0;I<Param1;I++)
          ((short *)Param2)[I]=ColorArray[I];
      return TRUE;
    }
    case DN_CLOSE:
    {
      StopSearch=TRUE;
      while (!SearchDone || WriteDataUsed)
        Sleep(10);
      return TRUE;
    }
    /* 10.08.2001 KM
       ��������� �������� ������� ������ ��� ��������� �������� �������.
    */
    case DN_RESIZECONSOLE:
    {
      WaitForSingleObject(hDialogMutex,INFINITE);

      COORD coord=(*(COORD*)Param2);
      SMALL_RECT rect;
      int IncY=coord.Y-DlgHeight-4;
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

      ReleaseMutex(hDialogMutex);
      return TRUE;
    }
    /* KM $ */
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

int FindFiles::FindFilesProcess()
{
  char Title[2*NM];
  char SearchStr[NM];
  hDialogMutex=CreateMutex(NULL,FALSE,NULL);

  if (*FindMask)
    sprintf(Title,"%s: %s",MSG(MFindFileTitle),FindMask);
  else
    sprintf(Title,"%s",MSG(MFindFileTitle));
  if (*FindStr)
  {
    char Temp[NM],FStr[NM];
    strncpy(FStr,FindStr,sizeof(FStr)-1);
    sprintf(Temp," \"%s\"",TruncStrFromEnd(FStr,10));
    sprintf(SearchStr,MSG(MFindSearchingIn),Temp);
  }
  else
    sprintf(SearchStr,MSG(MFindSearchingIn),"");

  /* $ 03.12.2001 DJ
     ���������� ����� ���� ������ � ������������
  */
  static struct DialogData FindDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,72,DLG_HEIGHT-2,0,0,0,0,Title,
  /* 01 */DI_LISTBOX,4,2,71,14,0,0,DIF_LISTNOBOX,0,(char*)0,
  /* 02 */DI_TEXT,-1,15,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 03 */DI_TEXT,5,16,0,0,0,0,DIF_SHOWAMPERSAND,0,SearchStr,
  /* 04 */DI_TEXT,3,17,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 05 */DI_BUTTON,0,18,0,0,0,0,DIF_CENTERGROUP,0,(char *)MFindNewSearch,
  /* 06 */DI_BUTTON,0,18,0,0,1,0,DIF_CENTERGROUP,1,(char *)MFindGoTo,
  /* 07 */DI_BUTTON,0,18,0,0,0,0,DIF_CENTERGROUP,0,(char *)MFindView,
  /* 08 */DI_BUTTON,0,18,0,0,0,0,DIF_CENTERGROUP,0,(char *)MFindPanel,
  /* 09 */DI_BUTTON,0,18,0,0,0,0,DIF_CENTERGROUP,0,(char *)MFindStop
  };
  /* DJ $ */
  MakeDialogItems(FindDlgData,FindDlg);

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  DlgHeight=DLG_HEIGHT;

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

  DlgWidth=FindDlg[0].X2-FindDlg[0].X1-4;
  Dialog *pDlg=new Dialog(FindDlg,sizeof(FindDlg)/sizeof(FindDlg[0]),FindDlgProc);
  hDlg=(HANDLE)pDlg;
  pDlg->SetDynamicallyBorn(TRUE);
  pDlg->SetHelp("FindFile");
  pDlg->SetPosition(-1,-1,76,DLG_HEIGHT+IncY);
  // ���� �� �������� ������, � �� ������������� ��������� �����������
  // ������ ��� ������ � ������ �������� �� �����������
  pDlg->Show();

  LastFoundNumber=0;
  SearchDone=FALSE;
  StopSearch=FALSE;
  PauseSearch=FALSE;
  WriteDataUsed=FALSE;
  FindFileCount=FindDirCount=0;
  FindExitIndex = LIST_INDEX_NONE;
  FindExitCode = FIND_EXIT_NONE;
  *FindMessage=*LastDirName=FindMessageReady=FindCountReady=0;

  // ����� ��� ������ � ������� ���������� � ���� ������
  if (_beginthread(WriteDialogData,0,NULL)==(unsigned long)-1)
    return FALSE;

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

  IsProcessAssignMacroKey++; // �������� ��� ����. �������
  pDlg->Process();
  IsProcessAssignMacroKey--;

  CloseHandle(hDialogMutex);

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
        char *FileName=FindList[i].FindData.cFileName;
        int Length=strlen(FileName);
        if (Length>0)
        // ��������� ������, ���� ��� ������
        {
          // ��� �������� � ������������ ������� ������� ��� ����� �� ��� ������.
          // ������ ���� ������ ������ �����.
          int IsArchive = ((FindList[i].ArcIndex != LIST_INDEX_NONE) &&
                          !(ArcList[FindList[i].ArcIndex].Flags&OPIF_REALNAMES));
          // ��������� ������ ����� ��� ����� �������
          /* $ 13.11.2001 VVM
            ! ��. ������� �����, ���� �� ������... */
          if (IsArchive || Opt.FindFolders ||
              !(FindList[i].FindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
          /* VVM $ */
          {
            if (IsArchive)
              strncpy(FileName,ArcList[FindList[i].ArcIndex].ArcName,sizeof(FileName)-1);
            PluginPanelItem *pi=&PanelItems[ItemsNumber++];
            memset(pi,0,sizeof(*pi));
            pi->FindData=FindList[i].FindData;
            if (IsArchive)
              pi->FindData.dwFileAttributes = 0;
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
          strncpy(ArcName,ArcList[FindList[FindExitIndex].ArcIndex].ArcName,sizeof(ArcName)-1);
          if (FindPanel->GetType()!=FILE_PANEL)
            FindPanel=CtrlObject->Cp()->ChangePanel(FindPanel,FILE_PANEL,TRUE,TRUE);
          strncpy(ArcPath,ArcName,sizeof(ArcPath)-1);
          *PointToName(ArcPath)=0;
          FindPanel->SetCurDir(ArcPath,TRUE);
          hPlugin=((FileList *)FindPanel)->OpenFilePlugin(ArcName,FALSE);
          if (hPlugin==(HANDLE)-2)
            hPlugin = INVALID_HANDLE_VALUE;
        } /* if */
        if (hPlugin != INVALID_HANDLE_VALUE)
        {
          char *StartName = PointToName(FileName);
          Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
          if (SearchMode==SEARCH_ROOT || SearchMode==SEARCH_ALL)
            CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_FIND);
          SetPluginDirectory(FileName, hPlugin);
          ActivePanel->Update(UPDATE_KEEP_SELECTION);
          if (!ActivePanel->GoToFile(StartName))
            ActivePanel->GoToFile(FileName);
          ActivePanel->Show();
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
          strncpy(SetName,NamePtr,sizeof(SetName)-1);
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


void FindFiles::SetPluginDirectory(char *DirName, HANDLE hPlugin)
{
  char Name[NM],*StartName,*EndName;
  strncpy(Name,DirName,sizeof(Name)-1);
  StartName=Name;
  while ((EndName=strchr(StartName,'\\'))!=NULL)
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
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif
void _cdecl FindFiles::PrepareFilesList(void *Param)
{
  WIN32_FIND_DATA FindData;
  char FullName[NM],Root[NM];

  DWORD DiskMask=FarGetLogicalDrives();
  CtrlObject->CmdLine->GetCurDir(Root);

  for (int CurrentDisk=0;DiskMask!=0;CurrentDisk++,DiskMask>>=1)
  {
    if (SearchMode==SEARCH_ALL)
    {
      if ((DiskMask & 1)==0)
        continue;
      sprintf(Root,"%c:\\",'A'+CurrentDisk);
      int DriveType=GetDriveType(Root);
      if (DriveType==DRIVE_REMOVABLE || DriveType==DRIVE_CDROM)
        if (DiskMask==1)
          break;
        else
          continue;
    }
    else
      if (SearchMode==SEARCH_ROOT)
        GetPathRoot(Root,Root);

    ScanTree ScTree(FALSE,SearchMode!=SEARCH_CURRENT_ONLY);

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
        if ((FileAttr & FILE_ATTRIBUTE_DIRECTORY)==0 || strcmp(SelName,"..")==0 ||
            strcmp(SelName,".")==0)
          continue;
        strncpy(CurRoot,Root,sizeof(CurRoot)-1);
        AddEndSlash(CurRoot);
        strcat(CurRoot,SelName);
      }
      else
        strncpy(CurRoot,Root,sizeof(CurRoot)-1);

      ScTree.SetFindPath(CurRoot,"*.*");

      strncpy(FindMessage,CurRoot,sizeof(FindMessage)-1);
      FindMessage[sizeof(FindMessage)-1]=0;
      FindMessageReady=TRUE;

      while (!StopSearch && ScTree.GetNextName(&FindData,FullName))
      {
        while (PauseSearch)
          Sleep(10);

        if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          strncpy(FindMessage,FullName,sizeof(FindMessage)-1);
          FindMessage[sizeof(FindMessage)-1]=0;
          FindMessageReady=TRUE;
        }

        if (IsFileIncluded(NULL,FullName,FindData.dwFileAttributes))
          AddMenuRecord(FullName,&FindData);

        if (SearchInArchives)
          ArchiveSearch(FullName);
      }
      if (SearchMode!=SEARCH_SELECTED)
        break;
    }
    if (SearchMode!=SEARCH_ALL)
      break;
  }

  while (!StopSearch && FindMessageReady)
    Sleep(10);
  sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);
  SearchDone=TRUE;
  FindMessageReady=TRUE;
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


void FindFiles::ArchiveSearch(char *ArcName)
{
  char *Buffer=new char[MAX_READ];
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
  int ReadSize=fread(Buffer,1,MAX_READ,ProcessFile);
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
      char SaveDirName[NM];
      int SaveListCount = FindListCount;
      strncpy(SaveDirName, LastDirName, NM);
      *LastDirName = 0;
      PreparePluginList((void *)1);
      WaitForSingleObject(hPluginMutex,INFINITE);
      CtrlObject->Plugins.ClosePlugin(ArcList[FindFileArcIndex].hPlugin);
      ArcList[FindFileArcIndex].hPlugin = INVALID_HANDLE_VALUE;
      ReleaseMutex(hPluginMutex);
      if (SaveListCount == FindListCount)
        strncpy(LastDirName, SaveDirName, NM);
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
    /* $ 17.01.2002 VVM
      ! ��������� ������ � ������� � ������ ������� � ������ -
        ���� � ������� ������� ���� ������������ */
    if ((FileAttr & FILE_ATTRIBUTE_DIRECTORY) && (Opt.FindFolders==0))
//        ((hPlugin == INVALID_HANDLE_VALUE) ||
//        (ArcList[FindFileArcIndex].Flags & OPIF_FINDFOLDERS)==0))
      return FALSE;
    /* VVM $ */

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
          RemoveDirectory(TempDir);
          break;
        }
        else
          ReleaseMutex(hPluginMutex);
        RemoveTemp=TRUE;
      }
      else
        strncpy(SearchFileName,FullName,sizeof(SearchFileName)-1);
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


//void FindFiles::AddMenuRecord(char *FullName, char *Path, WIN32_FIND_DATA *FindData)
void FindFiles::AddMenuRecord(char *FullName, WIN32_FIND_DATA *FindData)
{
  char MenuText[NM],FileText[NM],SizeText[30];
  char Date[30],DateStr[30],TimeStr[30];
  struct MenuItem ListItem;
  int i;

  Dialog* Dlg=(Dialog*)hDlg;
  VMenu *ListBox=Dlg->Item[1].ListPtr;
  if (!ListBox)
    return;

  WaitForSingleObject(hDialogMutex,INFINITE);

  memset(&ListItem,0,sizeof(ListItem));

  sprintf(SizeText,"%10u",FindData->nFileSizeLow);
  char *DisplayName=FindData->cFileName;
  if ((FindFileArcIndex != LIST_INDEX_NONE) &&
      (ArcList[FindFileArcIndex].Flags & OPIF_REALNAMES))
    DisplayName=PointToName(DisplayName);

  sprintf(FileText," %-30.30s %10.10s",DisplayName,SizeText);
  ConvertDate(&FindData->ftLastWriteTime,DateStr,TimeStr,5);
  sprintf(Date,"    %s   %s",DateStr,TimeStr);
  strcat(FileText,Date);
  sprintf(MenuText," %-*.*s",DlgWidth-3,DlgWidth-3,FileText);

  for (i=0;FullName[i]!=0;i++)
  {
    if (FullName[i]=='\x1')
      FullName[i]='\\';
  }

  char PathName[2*NM];
  /* $ 16.01.2002 VVM
    ! ��� ����� ������ ���� ���������� � FullName, ��� ��� ��� ��������� ������ */
//  if (Path)
//  {
//    for (i=0;Path[i]!=0;i++)
//    {
//      if (Path[i]=='\x1')
//        Path[i]='\\';
//    }
//    strcpy(PathName,Path);
//  }
//  else
//  {
    strncpy(PathName,FullName,sizeof(PathName)-1);
    PathName[sizeof(PathName)-1]=0;
    if ((FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
      *PointToName(PathName)=0;
//  }
  /* VVM $ */
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
      while (ListBox->GetCallCount())
        Sleep(10);
      // � ������ VVM ������� ���������� � ������ ������� LIST_INDEX_NONE �� ������ �������
      ListBox->SetUserData((void*)LIST_INDEX_NONE,sizeof(LIST_INDEX_NONE),ListBox->AddItem(&ListItem));
      ListItem.Flags&=~LIF_DISABLE;
      /* DJ $ */
    }
    strncpy(LastDirName,PathName,sizeof(LastDirName)-1);
    if ((FindFileArcIndex != LIST_INDEX_NONE) &&
        (!(ArcList[FindFileArcIndex].Flags & OPIF_REALNAMES)) &&
        (ArcList[FindFileArcIndex].ArcName) &&
        (*ArcList[FindFileArcIndex].ArcName))
    {
      char ArcPathName[NM*2];
      sprintf(ArcPathName,"%s:%s",ArcList[FindFileArcIndex].ArcName,*PathName=='.' ? "\\":PathName);
      strncpy(PathName,ArcPathName,sizeof(PathName)-1);
    }
    strncpy(SizeText,MSG(MFindFileFolder),sizeof(SizeText)-1);
    sprintf(FileText,"%-50.50s     <%6.6s>",TruncPathStr(PathName,50),SizeText);
    sprintf(ListItem.Name,"%-*.*s",DlgWidth-2,DlgWidth-2,FileText);

    DWORD ItemIndex = AddFindListItem(FindData);
    if (ItemIndex != LIST_INDEX_NONE)
    {
      // ������� ������ � FindData. ��� ��� �� �����
      memset(&FindList[ItemIndex].FindData,0,sizeof(FindList[ItemIndex].FindData));
      // ���������� LastDirName, �.�. PathName ��� ����� ���� ��������
      strncpy(FindList[ItemIndex].FindData.cFileName, LastDirName,
              sizeof(FindList[ItemIndex].FindData.cFileName)-1);
      // �������� ������� � ��������, ���-�� �� �� ��� ������ :)
      FindList[ItemIndex].FindData.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
      if (FindFileArcIndex != LIST_INDEX_NONE)
        FindList[ItemIndex].ArcIndex = FindFileArcIndex;

      while (ListBox->GetCallCount())
        Sleep(10);
      ListBox->SetUserData((void*)ItemIndex,sizeof(ItemIndex),
                           ListBox->AddItem(&ListItem));
    }
  }

  if ((FindData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
  {
    DWORD ItemIndex = AddFindListItem(FindData);
    if (ItemIndex != LIST_INDEX_NONE)
    {
      strncpy(FindList[ItemIndex].FindData.cFileName, FullName,
              sizeof(FindList[ItemIndex].FindData.cFileName)-1);
      if (FindFileArcIndex != LIST_INDEX_NONE)
        FindList[ItemIndex].ArcIndex = FindFileArcIndex;
    }
    strncpy(ListItem.Name,MenuText,sizeof(ListItem.Name)-1);
    /* $ 17.01.2002 VVM
      ! �������� ����� �� � ���������, � � ������. ���� �� �������� ��������� */
//    ListItem.SetSelect(!FindFileCount);

    while (ListBox->GetCallCount())
      Sleep(10);

    int ListPos = ListBox->AddItem(&ListItem);
    ListBox->SetUserData((void*)ItemIndex,sizeof(ItemIndex), ListPos);
    // ������� ��� �������� - � ������.
    if (!FindFileCount)
      ListBox->SetSelectPos(ListPos, -1);
    /* VVM $ */
    FindFileCount++;
  }
  else
    FindDirCount++;

  LastFoundNumber++;
  FindCountReady=TRUE;
  ReleaseMutex(hDialogMutex);
}


int FindFiles::LookForString(char *Name)
{
  FILE *SrcFile;
  char Buf[32768],SaveBuf[32768],CmpStr[sizeof(FindStr)];
  int Length,ReadSize;
  if ((Length=strlen(FindStr))==0)
    return(TRUE);
  HANDLE FileHandle=CreateFile(Name,GENERIC_READ|GENERIC_WRITE,
         FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
  if (FileHandle==INVALID_HANDLE_VALUE)
    FileHandle=CreateFile(Name,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,
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
  strncpy(CmpStr,FindStr,sizeof(CmpStr)-1);
  if (!CmpCase)
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

    while (1)
    {
      if (DecodeTableNum>0 && !UnicodeSearch)
        memcpy(Buf,SaveBuf,ReadSize);
      if (UnicodeSearch)
      {
        WideCharToMultiByte(CP_OEMCP,0,(LPCWSTR)SaveBuf,ReadSize/2,Buf,ReadSize,NULL,NULL);
        ReadSize/=2;
      }
      else
        if (UseDecodeTable || DecodeTableNum>0)
          for (int I=0;I<ReadSize;I++)
            Buf[I]=TableSet.DecodeTable[Buf[I]];
      if (!CmpCase)
        LocalUpperBuf(Buf,ReadSize);
      int CheckSize=ReadSize-Length+1;
      /* $ 30.07.2000 KM
         ��������� "Whole words" � ������
      */
      for (int I=0;I<CheckSize;I++)
      {
        int cmpResult;
        if (WholeWords)
        {
          int locResultLeft=FALSE;
          int locResultRight=FALSE;
          if (!FirstIteration)
          {
            if (isspace(Buf[I]) || iseol(Buf[I]))
              locResultLeft=TRUE;
            if (RealReadSize!=sizeof(Buf) && I+1+Length>=RealReadSize)
              locResultRight=TRUE;
            else
              if (isspace(Buf[I+1+Length]) || iseol(Buf[I+1+Length]))
                locResultRight=TRUE;

            if (!locResultLeft)
              if (strchr(Opt.WordDiv,Buf[I])!=NULL)
                locResultLeft=TRUE;
            if (!locResultRight)
              if (strchr(Opt.WordDiv,Buf[I+1+Length])!=NULL)
                locResultRight=TRUE;

            cmpResult=locResultLeft && locResultRight && CmpStr[0]==Buf[I+1]
              && (Length==1 || CmpStr[1]==Buf[I+2]
              && (Length==2 || memcmp(CmpStr+2,&Buf[I+3],Length-2)==0));
          }
          else
          {
            FirstIteration=FALSE;

            if (RealReadSize!=sizeof(Buf) && I+Length>=RealReadSize)
              locResultRight=TRUE;
            else
              if (isspace(Buf[I+Length]) || iseol(Buf[I+Length]))
                locResultRight=TRUE;

            if (!locResultRight)
              if (strchr(Opt.WordDiv,Buf[I+1+Length])!=NULL)
                locResultRight=TRUE;

            cmpResult=locResultRight && CmpStr[0]==Buf[I]
              && (Length==1 || CmpStr[1]==Buf[I+1]
              && (Length==2 || memcmp(CmpStr+2,&Buf[I+2],Length-2)==0));
          }
        }
        else
        {
          cmpResult=CmpStr[0]==Buf[I] && (Length==1 || CmpStr[1]==Buf[I+1]
            && (Length==2 || memcmp(CmpStr+2,&Buf[I+2],Length-2)==0));
        }
        if (cmpResult)
        {
          if (TimeRead)
            SetFileTime(FileHandle,NULL,&LastAccess,NULL);
          fclose(SrcFile);
          return(TRUE);
        }
      }
      /* KM $ */
      if (UseAllTables)
      {
        if (PrepareTable(&TableSet,DecodeTableNum++))
        {
          strncpy(CmpStr,FindStr,sizeof(CmpStr)-1);
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
    }

    if (RealReadSize==sizeof(Buf))
    {
      /* $ 30.07.2000 KM
         ��������� offset ��� ������ ������ ����� � ������ WordDiv
      */
      int NewPos;
      if (UnicodeSearch)
        NewPos=ftell(SrcFile)-2*(Length+1);
      else
        NewPos=ftell(SrcFile)-(Length+1);
      fseek(SrcFile,Max(NewPos,0),SEEK_SET);
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

  Sleep(200);
  *PluginSearchPath=0;
  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  /* $ 15.10.2001 VVM */
  HANDLE hPlugin=ArcList[FindFileArcIndex].hPlugin;
  struct OpenPluginInfo Info;
  CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
  strncpy(SaveDir,Info.CurDir,sizeof(SaveDir)-1);
  WaitForSingleObject(hPluginMutex,INFINITE);
  if (SearchMode==SEARCH_ROOT || SearchMode==SEARCH_ALL)
    CtrlObject->Plugins.SetDirectory(hPlugin,"\\",OPM_FIND);
  ReleaseMutex(hPluginMutex);
  RecurseLevel=0;
  ScanPluginTree(hPlugin,ArcList[FindFileArcIndex].Flags);
  /* VVM $ */
  WaitForSingleObject(hPluginMutex,INFINITE);
  if (SearchMode==SEARCH_ROOT || SearchMode==SEARCH_ALL)
    CtrlObject->Plugins.SetDirectory(hPlugin,SaveDir,OPM_FIND);
  ReleaseMutex(hPluginMutex);
  while (!StopSearch && FindMessageReady)
    Sleep(10);
  if (Param==NULL)
  {
    sprintf(FindMessage,MSG(MFindDone),FindFileCount,FindDirCount);
    FindMessageReady=TRUE;
    SearchDone=TRUE;
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

  if ((FindFileArcIndex != LIST_INDEX_NONE) &&
      (ArcList[FindFileArcIndex].Flags & OPIF_REALNAMES))
  {
    qsort((void *)PanelData,ItemCount,sizeof(*PanelData),SortItems);
  }

  if (SearchMode!=SEARCH_SELECTED || RecurseLevel!=1)
  {
    for (int I=0;I<ItemCount && !StopSearch;I++)
    {
      while (PauseSearch)
        Sleep(10);

      PluginPanelItem *CurPanelItem=PanelData+I;
      char *CurName=CurPanelItem->FindData.cFileName;
      char FullName[2*NM];
      if (strcmp(CurName,".")==0 || strcmp(CurName,"..")==0)
        continue;
//      char AddPath[2*NM];
      if (Flags & OPIF_REALNAMES)
      {
        strncpy(FullName,CurName,sizeof(FullName)-1);
//        strcpy(AddPath,CurName);
//        *PointToName(AddPath)=0;
      }
      else
      {
        sprintf(FullName,"%s%s",PluginSearchPath,CurName);
//        strcpy(AddPath,PluginSearchPath);
      }

      if (CurPanelItem->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      {
        strncpy(FindMessage,FullName,sizeof(FindMessage)-1);
        FindMessage[sizeof(FindMessage)-1]=0;
        for (int I=0;FindMessage[I]!=0;I++)
          if (FindMessage[I]=='\x1')
            FindMessage[I]='\\';
        FindMessageReady=TRUE;
      }

      if (IsFileIncluded(CurPanelItem,CurName,CurPanelItem->FindData.dwFileAttributes))
        AddMenuRecord(FullName,&CurPanelItem->FindData);

      if (SearchInArchives && (hPlugin != INVALID_HANDLE_VALUE) && (Flags & OPIF_REALNAMES))
        ArchiveSearch(FullName);
    }
  }
  if (SearchMode!=SEARCH_CURRENT_ONLY)
  {
    for (int I=0;I<ItemCount && !StopSearch;I++)
    {
      PluginPanelItem *CurPanelItem=PanelData+I;
      char *CurName=CurPanelItem->FindData.cFileName;
      if ((CurPanelItem->FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
          strcmp(CurName,".")!=0 && strcmp(CurName,"..")!=0 &&
          (SearchMode!=SEARCH_SELECTED || RecurseLevel!=1 ||
          CtrlObject->Cp()->ActivePanel->IsSelected(CurName)))
      {
        WaitForSingleObject(hPluginMutex,INFINITE);
        if (strchr(CurName,'\x1')==NULL && CtrlObject->Plugins.SetDirectory(hPlugin,CurName,OPM_FIND))
        {
          ReleaseMutex(hPluginMutex);
          strcat(PluginSearchPath,CurName);
          if (strlen(PluginSearchPath)<NM-2)
          {
            strcat(PluginSearchPath,"\x1");
            ScanPluginTree(hPlugin, Flags);
            *strrchr(PluginSearchPath,'\x1')=0;
          }
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

void FindFiles::WriteDialogData(void *Param)
{
  FarDialogItemData ItemData;
  char DataStr[NM];
  Dialog* Dlg=(Dialog*)hDlg;

  WriteDataUsed=TRUE;
  while(1)
  {
    VMenu *ListBox=Dlg->Item[1].ListPtr;
    /* $ 11.12.2001 VVM
      - �� ������ ������ ��� �������� �����-������� */
    if (ListBox && !PauseSearch && !ScreenSaverActive)
    /* VVM $ */
    {
      WaitForSingleObject(hDialogMutex,INFINITE);

      if (BreakMainThread)
        StopSearch=TRUE;

      if (FindCountReady)
      {
        sprintf(DataStr," %s: %d ",MSG(MFindFound),FindFileCount+FindDirCount);
        ItemData.PtrData=DataStr;
        ItemData.PtrLength=strlen(DataStr);

        while (ListBox->GetCallCount())
          Sleep(10);
        Dialog::SendDlgMessage(hDlg,DM_SETTEXT,2,(long)&ItemData);
        FindCountReady=FALSE;
      }
      if (FindMessageReady)
      {
        char SearchStr[NM];
        if (*FindStr)
        {
          char Temp[NM],FStr[NM];
          strncpy(FStr,FindStr,sizeof(FStr)-1);
          sprintf(Temp," \"%s\"",TruncStrFromEnd(FStr,10));
          sprintf(SearchStr,MSG(MFindSearchingIn),Temp);
        }
        else
          sprintf(SearchStr,MSG(MFindSearchingIn),"");
        int Wid1=strlen(SearchStr);
        int Wid2=DlgWidth-strlen(SearchStr)-1;

        if (SearchDone)
        {
          while (ListBox->GetCallCount())
            Sleep(10);
          Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

          strncpy(DataStr,MSG(MFindCancel),sizeof(DataStr)-1);
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
          while (ListBox->GetCallCount())
            Sleep(10);
          Dialog::SendDlgMessage(hDlg,DM_SETTEXT,3,(long)&ItemData);
        }
        FindMessageReady=FALSE;
      }

      if (LastFoundNumber && ListBox)
      {
        LastFoundNumber=0;
        while (ListBox->GetCallCount())
          Sleep(10);
        Dialog::SendDlgMessage(hDlg,DM_SHOWITEM,1,1);
      }
      ReleaseMutex(hDialogMutex);
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
  LPFINDLIST NewList = (LPFINDLIST)realloc(FindList, (FindListCapacity + Delta) * sizeof(FINDLIST));
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
  LPARCLIST NewList = (LPARCLIST)realloc(ArcList, (ArcListCapacity + Delta) * sizeof(ARCLIST));
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

DWORD FindFiles::AddArcListItem(char *ArcName, HANDLE hPlugin,
                                DWORD dwFlags, char *RootPath)
{
  if ((ArcListCount == ArcListCapacity) &&
      (!ArcListGrow()))
    return(LIST_INDEX_NONE);
  strncpy(ArcList[ArcListCount].ArcName, NullToEmpty(ArcName),
          sizeof(ArcList[ArcListCount].ArcName)-1);
  ArcList[ArcListCount].hPlugin = hPlugin;
  ArcList[ArcListCount].Flags = dwFlags;
  strncpy(ArcList[ArcListCount].RootPath, NullToEmpty(RootPath),
          sizeof(ArcList[ArcListCount].RootPath)-1);
  AddEndSlash(ArcList[ArcListCount].RootPath);
  return(ArcListCount++);
}

void FindFiles::ClearAllLists()
{
  if (FindList)
    free(FindList);
  if (ArcList)
    free(ArcList);
  FindList = NULL;
  FindListCapacity = FindListCount = 0;
  ArcList = NULL;
  ArcListCapacity = ArcListCount = 0;
  FindFileArcIndex = LIST_INDEX_NONE;
}
