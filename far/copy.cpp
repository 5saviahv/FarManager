/*
copy.cpp

����������� ������

*/

/* Revision: 1.71 23.03.2002 $ */

/*
Modify:
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
#include "foldtree.hpp"
#include "treelist.hpp"
#include "chgprior.hpp"
#include "scantree.hpp"
#include "savescr.hpp"
#include "manager.hpp"

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

static DWORD WINAPI CopyProgressRoutine(LARGE_INTEGER TotalFileSize,
       LARGE_INTEGER TotalBytesTransferred,LARGE_INTEGER StreamSize,
       LARGE_INTEGER StreamBytesTransferred,DWORD dwStreamNumber,
       DWORD dwCallbackReason,HANDLE hSourceFile,HANDLE hDestinationFile,
       LPVOID lpData);

static int BarX,BarY,BarLength;

static int64 TotalCopySize,CurCopySize;
static bool ShowTotalCopySize;
static int StaticMove;
static char TotalCopySizeText[32];

static int64 StartCopySizeEx;
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
};


ShellCopy::ShellCopy(Panel *SrcPanel,        // �������� ������ (��������)
                     int Move,               // =1 - �������� Move
                     int Link,               // =1 - Sym/Hard Link
                     int CurrentOnly,        // =1 - ������ ������� ����, ��� ��������
                     int Ask,                // =1 - �������� ������?
                     int &ToPlugin,          // =?
                     char *PluginDestPath)   // =?
{
  struct CopyDlgParam CDP;
  /* $ 03.08.2001 IS
       CopyDlgValue - � ���� ���������� ������ �������� ������� �� �������,
       ������ ��� ���������� �������� ����������, � CopyDlg[2].Data �� �������.
  */
  char CopyStr[100],SelName[NM],DestDir[NM],InitDestDir[NM],CopyDlgValue[NM];
  /* IS $ */
  char SelNameShort[NM];
  int DestPlugin;
  int AddSlash=FALSE;

  // ***********************************************************************
  // *** ��������������� ��������
  // ***********************************************************************
  // ����� ��������� ������ OS

  _tran(SysLog("[%p] ShellCopy::ShellCopy() ",this));

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
    return;
  }
  _tran(SysLog("[%p] ShellCopy::ShellCopy() 1",this));

  memset(&CDP,0,sizeof(CDP));
  CDP.IsDTSrcFixed=-1;

  if ((CDP.SelCount=SrcPanel->GetSelCount())==0)
    return;
  _tran(SysLog("[%p] ShellCopy::ShellCopy() 2",this));

  *SelName=*RenamedName=*CopiedName=0;

  if (CDP.SelCount==1)
  {
    SrcPanel->GetSelName(NULL,CDP.FileAttr);
    SrcPanel->GetSelName(SelName,CDP.FileAttr);
    if (strcmp(SelName,"..")==NULL)
      return;
  }
  _tran(SysLog("[%p] ShellCopy::ShellCopy() 3",this));

  /* $ 26.05.2001 OT ��������� ����������� ������� �� ����� ����������� */
  _tran(SysLog("call (*FrameManager)[0]->LockRefresh()"));
  (*FrameManager)[0]->LockRefresh();
  /* OT $ */

  // ������ ������ ������� �� �������
  GetRegKey("System", "CopyBufferSize", CopyBufferSize, COPY_BUFFER_SIZE);
  if (CopyBufferSize == 0)
    CopyBufferSize = COPY_BUFFER_SIZE;

  CDP.thisClass=this;
  CDP.AltF10=0;
  CDP.FolderPresent=0;
  CDP.FilesPresent=0;

  ShellCopy::Flags=0;
  ShellCopy::Flags|=Move?FCOPY_MOVE:0;
  ShellCopy::Flags|=Link?FCOPY_LINK:0;
  ShellCopy::Flags|=CurrentOnly?FCOPY_CURRENTONLY:0;

  TotalFiles=0;
  ShowTotalCopySize=Opt.CopyShowTotal != 0;
  TotalCopySize=CurCopySize=0;
  *TotalCopySizeText=0;
  SelectedFolderNameLength=0;
  DestPlugin=ToPlugin;
  ToPlugin=FALSE;
  *SrcDriveRoot=0;
  SrcDriveType=0;
  StaticMove=Move;

  ShellCopy::SrcPanel=SrcPanel;
  AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  PanelMode=DestPlugin ? AnotherPanel->GetMode():NORMAL_PANEL;
  SrcPanelMode=SrcPanel->GetMode();

  // ***********************************************************************
  // *** Prepare Dialog Controls
  // ***********************************************************************
  const char *HistoryName="Copy";
  /* $ 03.08.2001 IS ������� ����� �����: ����������������� */
  static struct DialogData CopyDlgData[]={
  /* 00 */  DI_DOUBLEBOX,3,1,72,10,0,0,0,0,(char *)MCopyDlgTitle,
  /* 01 */  DI_TEXT,5,2,0,2,0,0,0,0,(char *)MCMLTargetTO,
  /* 02 */  DI_EDIT,5,3,70,3,1,(DWORD)HistoryName,DIF_HISTORY|DIF_EDITEXPAND|DIF_USELASTHISTORY,0,"",
  /* 03 */  DI_TEXT,3,4,0,4,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */  DI_CHECKBOX,5,5,0,5,0,0,0,0,(char *)MCopySecurity,
  /* 05 */  DI_CHECKBOX,5,6,0,6,0,0,0,0,(char *)MCopyOnlyNewerFiles,
  /* 06 */  DI_CHECKBOX,5,7,0,7,0,0,0,0,(char *)MCopyMultiActions,
  /* 07 */  DI_TEXT,3,8,0,8,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 08 */  DI_BUTTON,0,9,0,9,0,0,DIF_CENTERGROUP,1,(char *)MCopyDlgCopy,
  /* 09 */  DI_BUTTON,0,9,0,9,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MCopyDlgTree,
  /* 10 */  DI_BUTTON,0,9,0,9,0,0,DIF_CENTERGROUP,0,(char *)MCopyDlgCancel,
  /* 11 */  DI_TEXT,5,2,0,2,0,0,DIF_SHOWAMPERSAND,0,"",
  };
  MakeDialogItems(CopyDlgData,CopyDlg);

  CopyDlg[6].Selected=Opt.MultiCopy;
  /* IS $ */

  if (CDP.SelCount==1)
  { // SelName & FileAttr ��� ��������� (��. � ����� ������ �������)

    // ���� ������� � �� ����, �� ������������, ��� ����� ������� �������
    if(Link && NT5 && (CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && PanelMode == NORMAL_PANEL)
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
  sprintf(CopyDlg[11].Data,"%.65s",CopyStr);

  // ��������� ���������
  strcpy(CopyDlg[5].Data,MSG(Link?MCopySymLink:MCopyOnlyNewerFiles));
  strcpy(CopyDlg[0].Data,MSG(Move?MMoveDlgTitle :(Link?MLinkDlgTitle:MCopyDlgTitle)));
  strcpy(CopyDlg[8].Data,MSG(Move?MCopyDlgRename:(Link?MCopyDlgLink:MCopyDlgCopy)));

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
  else if(PanelMode == PLUGIN_PANEL)
  {
    // ���� ��������������� ������ - ������, �� �������� OnlyNewer
    CDP.CopySecurity=CDP.OnlyNewerFiles=0;
    CopyDlg[4].Selected=CopyDlg[5].Selected=0;
    CopyDlg[5].Flags|=DIF_DISABLE;
    CopyDlg[4].Flags|=DIF_DISABLE;
  }

  AnotherPanel->GetCurDir(DestDir);

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
    switch(PanelMode)
    {
      case NORMAL_PANEL:
        if ((*DestDir==0 || !AnotherPanel->IsVisible()) && CDP.SelCount==1)
          strcpy(CopyDlg[2].Data,SelName);
        else
        {
          AddEndSlash(strcpy(CopyDlg[2].Data,DestDir));
        }
        CDP.PluginFormat[0]=0;
        break;
      case PLUGIN_PANEL:
        {
          struct OpenPluginInfo Info;
          AnotherPanel->GetOpenPluginInfo(&Info);
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
    char SrcDir[NM];
    int Selected5=CopyDlg[5].Selected;
    if(CDP.SelCount > 1 && !CDP.FilesPresent && CDP.FolderPresent)
      Selected5=1;
    SrcPanel->GetCurDir(SrcDir);
    if(!LinkRules(&CopyDlg[8].Flags,
                  &CopyDlg[5].Flags,
                  &Selected5,
                  SrcDir,
                  CopyDlg[2].Data,
                  &CDP))
      return;
    CopyDlg[5].Selected=Selected5;
  }

  // ����������� ������� " to"
  CopyDlg[1].X1=CopyDlg[1].X2=CopyDlg[11].X1+strlen(RemoveTrailingSpaces(CopyDlg[11].Data));

  // ***********************************************************************
  // *** ����� � ��������� �������
  // ***********************************************************************
  if (Ask)
  {
    Dialog Dlg(CopyDlg,sizeof(CopyDlg)/sizeof(CopyDlg[0]),CopyDlgProc,(long)&CDP);
    Dlg.SetHelp(Link?"HardSymLink":"CopyFiles");
    Dlg.SetPosition(-1,-1,76,12);

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
      if(DlgExitCode == 8)
      {
        /* $ 03.08.2001 IS
           �������� ������� �� ������� � �������� �� ������ � ����������� ��
           ��������� ����� �����������������
        */
        strcpy(CopyDlgValue,CopyDlg[2].Data);
        Opt.MultiCopy=CopyDlg[6].Selected;
        if(!Opt.MultiCopy) // ��������� multi*
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
        if(DestList.Set(CopyDlgValue))
          break;
        else
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MWarning),
                  MSG(MCopyIncorrectTargetList), MSG(MOk));
      }
      else
        break;
    }
    /* IS $ */
    if(DlgExitCode == 10 || DlgExitCode < 0 || (CopyDlg[8].Flags&DIF_DISABLE))
    {
      if (DestPlugin)
        ToPlugin=-1;
      return;
    }
  }
  // ***********************************************************************
  // *** ������ ���������� ������ ����� �������
  // ***********************************************************************
  ShellCopy::Flags|=CopyDlg[4].Selected?FCOPY_COPYSECURITY:0;
  ShellCopy::Flags|=CopyDlg[5].Selected?FCOPY_ONLYNEWERFILES:0;

  if (DestPlugin && !strcmp(CopyDlg[2].Data,InitDestDir))
  {
    ToPlugin=TRUE;
    return;
  }

  _tran(SysLog("[%p] ShellCopy::ShellCopy() 4",this));

  // ���������� ������� ����������� � NUL
  ShellCopy::Flags|=(!stricmp(CopyDlg[2].Data,"nul") || !stricmp(CopyDlg[2].Data,"con"))?FCOPY_COPYTONUL:0;

  if(CDP.SelCount==1 || (ShellCopy::Flags&FCOPY_COPYTONUL))
    AddSlash=FALSE; //???

  if (DestPlugin==2)
  {
    if (*PluginDestPath!=NULL)
      strcpy(PluginDestPath,CopyDlg[2].Data);
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
  AnotherPanel->CloseFile();
  *DestDizPath=0;
  SrcPanel->SaveSelection();

  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));

  ShowTitle(TRUE);
  SetFarTitle(Move ? MSG(MCopyMovingTitle):MSG(MCopyCopyingTitle));

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
        CopyBufSize=1024; // �������� � 1�
        ReadOnlyDelMode=ReadOnlyOvrMode=OvrMode=SkipMode=-1;

        DestList.Start();
        while(NULL!=(NamePtr=DestList.GetNext()))
        {
          if(DestList.IsEmpty()) // ����� ������ ������� ��������� � ��������� Move.
          {
            ShellCopy::Flags|=Move?FCOPY_MOVE:0; // ������ ��� ��������� ��������
            ShellCopy::Flags|=FCOPY_COPYLASTTIME;
          }

          strcpy(NameTmp, NamePtr);

          // ���� ���������� ��������� ������ 1 � ����� ��� ���� �������, �� ������
          // ������ ���, ����� �� ����� ��� '\\'
          if(AddSlash)
              AddEndSlash(NameTmp);

          // ��� ����������� ������ ������� ������ ������� "������"
          if (CDP.SelCount==1 && Move && strpbrk(NameTmp,":\\")==NULL)
            ShowTotalCopySize=FALSE;

          if(Move) // ��� ����������� "�����" ��� �� ����������� ��� "���� �� �����"
          {
            char SrcDir[NM];
            SrcPanel->GetCurDir(SrcDir);
            if(IsSameDisk(SrcDir,NameTmp))
              ShowTotalCopySize=FALSE;
          }

          // ������� ���� ��� ����
          *DestDizPath=0;
          ShellCopy::Flags&=~FCOPY_DIZREAD;

          // �������� ���������
          SrcPanel->SaveSelection();

          // ���������� - ���� ������ �����������
          SetPreRedrawFunc(ShellCopy::PR_ShellCopyMsg);
          int I=CopyFileTree(NameTmp);
          SetPreRedrawFunc(NULL);

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
              if (Move && !DestReadOnly)
                SrcPanel->FlushDiz();
            DestDiz.Flush(DestDizPath);
          }
        }
      }
  }
  /* IS $ */

  // ***********************************************************************
  // *** �������������� ������ ��������
  // *** ���������������/�����/��������
  // ***********************************************************************

  SetConsoleTitle(OldTitle);

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
  if (CDP.SelCount==1 && *RenamedName)
    SrcPanel->GoToFile(RenamedName);

  AnotherPanel->SortFileList(TRUE);
  AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);

  if(SrcPanelMode == PLUGIN_PANEL)
    SrcPanel->SetPluginModified();

  CtrlObject->Cp()->Redraw();
#else

  SrcPanel->Update(UPDATE_KEEP_SELECTION);
  if (CDP.SelCount==1 && *RenamedName)
    SrcPanel->GoToFile(RenamedName);

  SrcPanel->Redraw();

  AnotherPanel->SortFileList(TRUE);
  AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
  AnotherPanel->Redraw();
#endif
}


long WINAPI ShellCopy::CopyDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
#define DM_CALLTREE (DM_USER+1)
  struct CopyDlgParam *DlgParam;
  struct FarDialogItem DItem;
  DlgParam=(struct CopyDlgParam *)Dialog::SendDlgMessage(hDlg,DM_GETDLGDATA,0,0);

  switch(Msg)
  {
    case DN_BTNCLICK:
    {
      if(Param1 == 9)
      {
        Dialog::SendDlgMessage(hDlg,DM_CALLTREE,0,0);
        return FALSE;
      }
      else if(Param1 == 8)
      {
        Dialog::SendDlgMessage(hDlg,DM_CLOSE,8,0);
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
      return FALSE;
    }

    case DN_EDITCHANGE:
      if(Param1 == 2)
      {
        char SrcDir[NM];
        struct FarDialogItem DItem4,DItem5,DItem8;
        DlgParam->thisClass->SrcPanel->GetCurDir(SrcDir);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,4,(long)&DItem4);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,5,(long)&DItem5);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,8,(long)&DItem8);

        // ��� �������� �����?
        if((DlgParam->thisClass->Flags)&FCOPY_LINK)
        {
          DlgParam->thisClass->LinkRules(&DItem8.Flags,
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
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,8,(long)&DItem8);
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
      BOOL MultiCopy=Dialog::SendDlgMessage(hDlg,DM_GETCHECK,6,0)==BSTATE_CHECKED;
      char NewFolder[NM], OldFolder[NM];
      *NewFolder = 0;
      Dialog::SendDlgMessage(hDlg,DM_GETTEXTPTR,2,(long)OldFolder);
      if(DlgParam->AltF10 == 2)
      {
        strcpy(NewFolder, OldFolder);
        if(MultiCopy)
        {
          UserDefinedList DestList;
          if(DestList.Set(OldFolder))
          {
            DestList.Start();
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

BOOL ShellCopy::LinkRules(DWORD *Flags8,DWORD* Flags5,int* Selected5,
                         char *SrcDir,char *DstDir,struct CopyDlgParam *CDP)
{
  char Root[1024];
  *Flags8|=DIF_DISABLE; // �������� �����!
  *Flags5|=DIF_DISABLE;

  if(DstDir && DstDir[0] == '\\' && DstDir[1] == '\\')
  {
    *Selected5=0;
    return TRUE;
  }
//_SVS(SysLog("\n---"));
  // �������� ������ ���� � ��������� � ���������
  if(CDP->IsDTSrcFixed == -1)
  {
    char FSysNameSrc[NM];
    ConvertNameToFull(SrcDir,Root, sizeof(Root));
    GetPathRoot(Root,Root);
//_SVS(SysLog("SrcDir=%s",SrcDir));
//_SVS(SysLog("Root=%s",Root));
    CDP->IsDTSrcFixed=GetDriveType(Root);
    CDP->IsDTSrcFixed=CDP->IsDTSrcFixed == DRIVE_FIXED || CDP->IsDTSrcFixed == DRIVE_CDROM;
    GetVolumeInformation(Root,NULL,0,NULL,NULL,&CDP->FileSystemFlagsSrc,FSysNameSrc,sizeof(FSysNameSrc));
    CDP->FSysNTFS=!stricmp(FSysNameSrc,"NTFS")?TRUE:FALSE;
//_SVS(SysLog("FSysNameSrc=%s",FSysNameSrc));
  }

  // 1. ���� �������� ��������� �� �� ���������� �����
  if(CDP->IsDTSrcFixed)
  {
    char FSysNameDst[NM];
    DWORD FileSystemFlagsDst;

    ConvertNameToFull(DstDir,Root, sizeof(Root));
    GetPathRoot(Root,Root);
    if(GetFileAttributes(Root) == -1)
      return TRUE;

    //GetVolumeInformation(Root,NULL,0,NULL,NULL,&FileSystemFlagsDst,FSysNameDst,sizeof(FSysNameDst));

    // 3. ���� �������� ��������� �� �� ���������� �����
    if(GetDriveType(Root) == DRIVE_FIXED &&
       GetVolumeInformation(Root,NULL,0,NULL,NULL,&FileSystemFlagsDst,FSysNameDst,sizeof(FSysNameDst)) &&
       // 4. ���� �������� ��������� �� ���������� �����, �������� �� NTFS
       !stricmp(FSysNameDst,"NTFS")
      )
    {
      int SameDisk=IsSameDisk(SrcDir,DstDir);

      if(CDP->SelCount == 1)
      {
        if(CDP->FolderPresent) // Folder?
        {
          // . ���� �������� ��������� �� ���������� ����� NTFS, �� �� �������������� repase point
          if(NT5 &&
//             (CDP->FileSystemFlagsSrc&FILE_SUPPORTS_REPARSE_POINTS) &&
             (FileSystemFlagsDst&FILE_SUPPORTS_REPARSE_POINTS))
          {
            *Flags5 &=~ DIF_DISABLE;
            // ��� �������� �� ��������, ����� �� ������ ���� �� ������� �� ������
            // ����� �... ����� ����� � ��������.
            if(*Selected5 || (!*Selected5 && SameDisk))
               *Flags8 &=~ DIF_DISABLE;
          }
          else if(NT && !NT5 && SameDisk)
          {
            *Selected5=0;
            *Flags8 &=~ DIF_DISABLE;
          }
          else
            *Selected5=0;
        }
        else if(SameDisk && CDP->FSysNTFS) // ��� ����!
        {
          *Selected5=0;
          *Flags8 &=~ DIF_DISABLE;
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
              *Flags8 &=~ DIF_DISABLE;
            }
          }
          else if(NT && !NT5 && SameDisk)
          {
            *Selected5=0;
            *Flags8 &=~ DIF_DISABLE;
          }

          if(CDP->FilesPresent && SameDisk && CDP->FSysNTFS)
          {
//            *Selected5=0;
            *Flags8 &=~ DIF_DISABLE;
          }
        }
        else if(SameDisk && CDP->FSysNTFS) // ��� ����!
        {
          *Selected5=0;
          *Flags8 &=~ DIF_DISABLE;
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
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  //SaveScreen SaveScr;
  DWORD DestAttr;
  _tran(SysLog("[%p] ShellCopy::CopyFileTree()",this));

  char SelName[NM],SelShortName[NM];
  int Length,FileAttr;

  SetCursorType(FALSE,0);

  ShellCopy::Flags&=~(FCOPY_STREAMSKIP|FCOPY_STREAMALL);

  if(TotalCopySize == 0)
  {
    *TotalCopySizeText=0;
    /* $ 19.12.2001 VVM
      ! �� ��������� �������� ��� �������� ������ */
    if (ShowTotalCopySize && !(ShellCopy::Flags&FCOPY_LINK) && !CalcTotalSize())
      return COPY_FAILURE;
  }
  else
    CurCopySize=0;


/* $ 30.01.2001 VVM
    + �������� ����� ������. */
  if (ShowCopyTime)
    CopyStartTime = clock();
  CopyTime = 0;
  LastShowTime = 0;
/* VVM $ */

  ShellCopyMsg("","",MSG_LEFTALIGN);

  if ((Length=strlen(Dest))==0 || strcmp(Dest,".")==0)
    return COPY_FAILURE; //????
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
          return COPY_FAILURE;
        }
      }
    }
    DestAttr=GetFileAttributes(Dest);
  }

  int SameDisk=FALSE;
  if (ShellCopy::Flags&FCOPY_MOVE)
  {
    char SrcDir[NM];
    SrcPanel->GetCurDir(SrcDir);
    SameDisk=IsSameDisk(SrcDir,Dest);
  }

  SrcPanel->GetSelName(NULL,FileAttr);
  while (SrcPanel->GetSelName(SelName,FileAttr,SelShortName))
  {
    if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && strpbrk(Dest,"*?")!=NULL)
    {
      char FullDest[NM];
      strcpy(FullDest,Dest);
      ShellCopyConvertWildcards(SelName,FullDest);
      DestAttr=GetFileAttributes(FullDest);
    }

    char DestPath[NM];
    strcpy(DestPath,Dest);
    HANDLE FindHandle;
    WIN32_FIND_DATA SrcData;
    int CopyCode,KeepPathPos;

    ShellCopy::Flags&=~FCOPY_OVERWRITENEXT;

    if (*SrcDriveRoot==0 || LocalStrnicmp(SelName,SrcDriveRoot,strlen(SrcDriveRoot))!=0)
    {
      GetPathRoot(SelName,SrcDriveRoot);
      SrcDriveType=GetDriveType(strchr(SelName,'\\')!=NULL ? SrcDriveRoot:NULL);
    }

    if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
      SelectedFolderNameLength=strlen(SelName);
    else
      SelectedFolderNameLength=0;

    if ((FindHandle=FindFirstFile(SelName,&SrcData))==INVALID_HANDLE_VALUE)
    {
      CopyTime+= (clock() - CopyStartTime);
      if (Message(MSG_DOWN|MSG_WARNING,2,MSG(MError),MSG(MCopyCannotFind),
              SelName,MSG(MSkip),MSG(MCancel))==1)
      {
        return COPY_FAILURE;
      }
      CopyStartTime = clock();
      int64 SubSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
      TotalCopySize-=SubSize;
      continue;
    }
    FindClose(FindHandle);

    if((SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
       (ShellCopy::Flags&FCOPY_CREATESYMLINK)
      )
    {
      //_SVS(SysLog("\n---"));

      switch(MkSymLink(SelName,Dest,ShellCopy::Flags))
      {
        case 2: break;
        case 1: continue;
        case 0: return COPY_FAILURE;
      }
    }

    KeepPathPos=PointToName(SelName)-SelName;

    if ((ShellCopy::Flags&FCOPY_MOVE))
    {
      if (KeepPathPos!=0 && PointToName(Dest)==Dest)
      {
        strcpy(DestPath,SelName);
        strcpy(DestPath+KeepPathPos,Dest);
        SameDisk=TRUE;
      }
      if (!SameDisk)
        CopyCode=COPY_FAILURE;
      else
      {
        CopyCode=ShellCopyOneFile(SelName,&SrcData,DestPath,KeepPathPos,1);
        if (CopyCode==COPY_SUCCESS_MOVE)
        {
          if (*DestDizPath)
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
          continue;
        }
        if (CopyCode==COPY_CANCEL)
        {
          return COPY_CANCEL;
        }
        if (CopyCode==COPY_NEXT)
        {
          int64 SubSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
          TotalCopySize-=SubSize;
          continue;
        }
        if (!(ShellCopy::Flags&FCOPY_MOVE) || CopyCode==COPY_FAILURE)
          ShellCopy::Flags|=FCOPY_OVERWRITENEXT;
      }
    }

    if (!(ShellCopy::Flags&FCOPY_MOVE) || CopyCode==COPY_FAILURE)
    {
      CopyCode=ShellCopyOneFile(SelName,&SrcData,Dest,KeepPathPos,0);
      ShellCopy::Flags&=~FCOPY_OVERWRITENEXT;
      if (CopyCode==COPY_CANCEL)
      {
        return COPY_CANCEL;
      }
      if (CopyCode!=COPY_SUCCESS)
      {
        int64 SubSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
        TotalCopySize-=SubSize;
        continue;
      }
    }

    if (CopyCode==COPY_SUCCESS && !(ShellCopy::Flags&FCOPY_COPYTONUL) && *DestDizPath)
    {
      if (*CopiedName==0)
        strcpy(CopiedName,SelName);
      SrcPanel->CopyDiz(SelName,SelShortName,CopiedName,CopiedName,&DestDiz);
    }

    if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      int SubCopyCode;
      char SubName[NM],FullName[NM];
      ScanTree ScTree(TRUE);

      strcpy(SubName,SelName);
      strcat(SubName,"\\");
      if (DestAttr==(DWORD)-1)
        KeepPathPos=strlen(SubName);
      ScTree.SetFindPath(SubName,"*.*");
      while (ScTree.GetNextName(&SrcData,FullName))
      {
        int AttemptToMove=FALSE;
        if ((ShellCopy::Flags&FCOPY_MOVE) && SameDisk && (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0)
        {
          AttemptToMove=TRUE;
          int SubMoveCode=ShellCopyOneFile(FullName,&SrcData,Dest,KeepPathPos,1);
          if (SubMoveCode==COPY_CANCEL)
          {
            return COPY_CANCEL;
          }
          if (SubMoveCode==COPY_NEXT)
          {
            int64 SubSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
            TotalCopySize-=SubSize;
            continue;
          }
          if (SubMoveCode==COPY_SUCCESS_MOVE)
            continue;
        }
        int SaveOvrMode;
        if (AttemptToMove)
        {
          SaveOvrMode=OvrMode;
          OvrMode=1;
        }
        SubCopyCode=ShellCopyOneFile(FullName,&SrcData,Dest,KeepPathPos,0);
        if (AttemptToMove)
          OvrMode=SaveOvrMode;
        if (SubCopyCode==COPY_CANCEL)
        {
          return COPY_CANCEL;
        }
        if (SubCopyCode==COPY_NEXT)
        {
          int64 SubSize(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
          TotalCopySize-=SubSize;
        }
        if ((ShellCopy::Flags&FCOPY_MOVE) && SubCopyCode==COPY_SUCCESS)
          if (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
          {
            if (ScTree.IsDirSearchDone())
            {
              if (SrcData.dwFileAttributes & FA_RDONLY)
                SetFileAttributes(FullName,0);
              if (RemoveDirectory(FullName))
                TreeList::DelTreeName(FullName);
            }
          }
          else
            if (DeleteAfterMove(FullName,SrcData.dwFileAttributes)==COPY_CANCEL)
            {
              return COPY_CANCEL;
            }
      }
      if ((ShellCopy::Flags&FCOPY_MOVE) && CopyCode==COPY_SUCCESS)
      {
        if (FileAttr & FA_RDONLY)
          SetFileAttributes(SelName,0);
        if (RemoveDirectory(SelName))
        {
          TreeList::DelTreeName(SelName);
          if (*DestDizPath)
            SrcPanel->DeleteDiz(SelName,SelShortName);
        }
      }
    }
    else
      if ((ShellCopy::Flags&FCOPY_MOVE) && CopyCode==COPY_SUCCESS)
      {
        int DeleteCode;
        if ((DeleteCode=DeleteAfterMove(SelName,FileAttr))==COPY_CANCEL)
          return COPY_CANCEL;
        if (DeleteCode==COPY_SUCCESS && *DestDizPath)
          SrcPanel->DeleteDiz(SelName,SelShortName);
      }
    if ((!(ShellCopy::Flags&FCOPY_CURRENTONLY)) && (ShellCopy::Flags&FCOPY_COPYLASTTIME))
    {
      SrcPanel->ClearLastGetSelection();
    }
  }
  _tran(SysLog("[%p] ShellCopy::CopyFileTree() end",this));
  return COPY_SUCCESS; //COPY_SUCCESS_MOVE???
}

COPY_CODES ShellCopy::ShellCopyOneFile(char *Src,WIN32_FIND_DATA *SrcData,
                                       char *Dest,int KeepPathPos,int Rename)
{
  char DestPath[2*NM];
  DWORD DestAttr;
  HANDLE FindHandle;
  WIN32_FIND_DATA DestData;
  int SameName=0,Append=0;

  *RenamedName=*CopiedName=0;

  if (CheckForEsc())
    return(COPY_CANCEL);

  strcpy(DestPath,Dest);

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

  if (*NamePtr==0 || strcmp(NamePtr,"..")==0)
    DestAttr=FILE_ATTRIBUTE_DIRECTORY;

  *DestData.cFileName=0;

  if (DestAttr==(DWORD)-1 && (FindHandle=FindFirstFile(DestPath,&DestData))!=INVALID_HANDLE_VALUE)
  {
    FindClose(FindHandle);
    DestAttr=DestData.dwFileAttributes;
  }

  if (DestAttr!=(DWORD)-1 && (DestAttr & FILE_ATTRIBUTE_DIRECTORY))
  {
    int CmpCode;
    if ((CmpCode=CmpFullNames(Src,DestPath))!=0)
    {
      SameName=1;
      if (CmpCode==2 || !Rename)
      {
        CopyTime+= (clock() - CopyStartTime);
        SetMessageHelp("ErrCopyItSelf");
        Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),MSG(MCannotCopyFolderToItself1),
                Src,MSG(MCannotCopyFolderToItself2),MSG(MOk));
        CopyStartTime = clock();
        return(COPY_CANCEL);
      }
    }

    if (!SameName)
    {
      char *PathPtr;
      int Length=strlen(DestPath);
      if (DestPath[Length-1]!='\\' && DestPath[Length-1]!=':')
        strcat(DestPath,"\\");
      PathPtr=Src+KeepPathPos;
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
    }
  }

  if (stricmp(DestPath,"nul")!=0 && stricmp(DestPath,"prn")!=0)
    SetDestDizPath(DestPath);

  ShellCopyMsg(Src,DestPath,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);

  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    // �������� ���������� ��������� �� ������
    switch(CheckStreams(Src,DestPath))
    {
      case COPY_NEXT: return COPY_NEXT;
      case COPY_CANCEL: return COPY_CANCEL;
    }

    if (SrcData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
      if (!Rename)
        strcpy(CopiedName,PointToName(DestPath));
      if (DestAttr!=(DWORD)-1)
      {
        if ((DestAttr & FILE_ATTRIBUTE_DIRECTORY) && !SameName)
        {
          DWORD SetAttr=SrcData->dwFileAttributes;
          if (SrcDriveType==DRIVE_CDROM && Opt.ClearReadOnly && (SetAttr & FA_RDONLY))
            SetAttr&=~FA_RDONLY;
//_SVS(SysLog("SetAttr=0x%08X, DestAttr=0x%08X",SetAttr,DestAttr));
          if (SetAttr!=DestAttr)
            ShellSetAttr(DestPath,SetAttr);
          char SrcFullName[NM];
          if (ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName)){
            return(COPY_NEXT);
          }
          return(strcmp(DestPath,SrcFullName)==0 ? COPY_NEXT:COPY_SUCCESS);
        }
        int Type=GetFileTypeByName(DestPath);
        if (Type==FILE_TYPE_CHAR || Type==FILE_TYPE_PIPE)
          return(Rename ? COPY_NEXT:COPY_SUCCESS);
      }

      if (Rename)
      {
        char SrcFullName[NM],DestFullName[NM];
        if (ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName)){
          return(COPY_NEXT);
        }
        /* $ 18.07.2001 VVM
          + �������� �������������, ���� �� ������� */
        while (1)
        {
          if (MoveFile(Src,DestPath))
          {
            if (PointToName(DestPath)==DestPath)
              strcpy(RenamedName,DestPath);
            else
              strcpy(CopiedName,PointToName(DestPath));
//          ConvertNameToFull(DestPath,DestFullName, sizeof(DestFullName));
            if (ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName)){
              return(COPY_NEXT);
            }
            TreeList::RenTreeName(SrcFullName,DestFullName);
            return(SameName ? COPY_NEXT:COPY_SUCCESS_MOVE);
          }
          else
          /* $ 18.07.2001 VVM
            + ��������, ��� ������, ���� �� ������ ������������� ������� */
          {
            int MsgCode = Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                                  MSG(MCopyCannotRenameFolder),Src,MSG(MCopyRetry),
                                  MSG(MCopyIgnore),MSG(MCopyCancel));
            switch (MsgCode)
            {
              case 0:  continue;
              case 1:  return (COPY_FAILURE);
              default: return (COPY_CANCEL);
            } /* switch */
          } /* else */
        } /* while */
        /* VVM $ */
      }

      SECURITY_ATTRIBUTES sa;
      if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(Src,&sa))
        return(COPY_CANCEL);
      while (!CreateDirectory(DestPath,(ShellCopy::Flags&FCOPY_COPYSECURITY) ? &sa:NULL))
      {
        int MsgCode;
        CopyTime+= (clock() - CopyStartTime);
        MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                        MSG(MCopyCannotCreateFolder),DestPath,MSG(MCopyRetry),
                        MSG(MCopySkip),MSG(MCopyCancel));
        CopyStartTime = clock();
        if (MsgCode!=0)
          return((MsgCode==-2 || MsgCode==2) ? COPY_CANCEL:COPY_NEXT);
      }
      DWORD SetAttr=SrcData->dwFileAttributes;
      if (SrcDriveType==DRIVE_CDROM && Opt.ClearReadOnly && (SetAttr & FA_RDONLY))
        SetAttr&=~FA_RDONLY;
      if(!ShellSetAttr(DestPath,SetAttr))
      {
         RemoveDirectory(DestPath);
         return COPY_CANCEL;
      }
      TreeList::AddTreeName(DestPath);
      return(COPY_SUCCESS);
    }

    if (DestAttr!=(DWORD)-1 && (DestAttr & FILE_ATTRIBUTE_DIRECTORY)==0)
    {
      if (SrcData->nFileSizeLow==DestData.nFileSizeLow)
      {
        int CmpCode;
        if ((CmpCode=CmpFullNames(Src,DestPath))!=0)
        {
          SameName=1;
          if (CmpCode==2 || !Rename)
          {
            CopyTime+= (clock() - CopyStartTime);
            Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),MSG(MCannotCopyFileToItself1),
                    Src,MSG(MCannotCopyFileToItself2),MSG(MOk));
            CopyStartTime = clock();
            return(COPY_CANCEL);
          }
        }
      }

      int RetCode, AskCode;
      CopyTime+= (clock() - CopyStartTime);
      AskCode = AskOverwrite(SrcData,DestPath,DestAttr,SameName,Rename,1,Append,RetCode);
      CopyStartTime = clock();
      if (!AskCode)
        return((COPY_CODES)RetCode);
    }
  }
  else
  {
    if (SrcData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      return COPY_SUCCESS;
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

  while (1)
  {
    int CopyCode;
    int64 SaveCopySize=CurCopySize;
    int64 SaveTotalSize=TotalCopySize;
    if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && Rename)
    {
      int MoveCode,AskDelete;
      if (WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT)
        if (!Append && DestAttr!=(DWORD)-1 && !SameName)
          remove(DestPath);
      if (!Append)
      {
        if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
          MoveCode=MoveFileEx(Src,DestPath,SameName ? MOVEFILE_COPY_ALLOWED:MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
        else
          MoveCode=MoveFile(Src,DestPath);
        if (!MoveCode && GetLastError()==ERROR_NOT_SAME_DEVICE)
          return(COPY_FAILURE);
        if (ShowTotalCopySize && MoveCode)
        {
          int64 AddSize(SrcData->nFileSizeHigh,SrcData->nFileSizeLow);
          CurCopySize+=AddSize;
          ShowBar(CurCopySize,TotalCopySize,true);
          ShowTitle(FALSE);
        }
        AskDelete=0;
      }
      else
      {
        CopyCode=ShellCopyFile(Src,SrcData,DestPath,(DWORD)-1,Append);
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
            return(COPY_CANCEL);
          case COPY_NEXT:
            return(COPY_NEXT);
        }
        AskDelete=1;
      }

      if (MoveCode)
      {
        if (DestAttr==(DWORD)-1 || (DestAttr & FILE_ATTRIBUTE_DIRECTORY)==0)
          if (PointToName(DestPath)==DestPath)
            strcpy(RenamedName,DestPath);
          else
            strcpy(CopiedName,PointToName(DestPath));
        if (SrcDriveType==DRIVE_CDROM && Opt.ClearReadOnly &&
            (SrcData->dwFileAttributes & FA_RDONLY))
          ShellSetAttr(DestPath,SrcData->dwFileAttributes & ~FA_RDONLY);
        TotalFiles++;
        if (AskDelete && DeleteAfterMove(Src,SrcData->dwFileAttributes)==COPY_CANCEL)
          return(COPY_CANCEL);
        return(COPY_SUCCESS_MOVE);
      }
    }
    else
    {
      if ((CopyCode=ShellCopyFile(Src,SrcData,DestPath,DestAttr,Append))==COPY_SUCCESS)
      {
        strcpy(CopiedName,PointToName(DestPath));
        if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
        {
          if (SrcDriveType==DRIVE_CDROM && Opt.ClearReadOnly &&
              (SrcData->dwFileAttributes & FA_RDONLY))
            ShellSetAttr(DestPath,SrcData->dwFileAttributes & ~FA_RDONLY);
          if (DestAttr!=(DWORD)-1 && LocalStricmp(CopiedName,DestData.cFileName)==0 &&
              strcmp(CopiedName,DestData.cFileName)!=0)
            MoveFile(DestPath,DestPath);
        }
        TotalFiles++;
        return(COPY_SUCCESS);
      }
      else
        if (CopyCode==COPY_CANCEL || CopyCode==COPY_NEXT)
          return((COPY_CODES)CopyCode);
    }
    //????
    if(CopyCode == COPY_FAILUREREAD)
      return COPY_FAILURE;
    //????

    char Msg1[2*NM],Msg2[2*NM];
    sprintf(Msg1,(ShellCopy::Flags&FCOPY_LINK) ? MSG(MCannotLink):
                   (ShellCopy::Flags&FCOPY_MOVE) ? MSG(MCannotMove):
                     MSG(MCannotCopy),
            Src);
    sprintf(Msg2,MSG(MCannotCopyTo),DestPath);
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
                        Msg1,Msg2,MSG(MCopyRetry),MSG(MCopySkip),
                        MSG(MCopySkipAll),MSG(MCopyCancel));
        CopyStartTime = clock();
      }
      switch(MsgCode)
      {
        case -1:
        case  1:
          return COPY_NEXT;
        case  2:
          SkipMode=1;
          return COPY_NEXT;
        case -2:
        case  3:
          return COPY_CANCEL;
      }
      /* KM $ */
    }
    CurCopySize=SaveCopySize;
    TotalCopySize=SaveTotalSize;
    int RetCode, AskCode;
    CopyTime+= (clock() - CopyStartTime);
    AskCode = AskOverwrite(SrcData,DestPath,DestAttr,SameName,Rename,1,Append,RetCode);
    CopyStartTime = clock();
    if (!AskCode)
      return((COPY_CODES)RetCode);
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
  ((ShellCopy*)PreRedrawParam.Param1)->ShellCopyMsg((char*)PreRedrawParam.Param2,(char*)PreRedrawParam.Param3,PreRedrawParam.Flags&(~MSG_KEEPBACKGROUND));
}

void ShellCopy::ShellCopyMsg(char *Src,char *Dest,int Flags)
{
  char FilesStr[100],BarStr[100],SrcName[NM],DestName[NM];

  _tran(SysLog("[%p] ShellCopy::ShellCopyMsg()",this));
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
                    "",MSG(MCopyScanning),DestName,"","",BarStr,"");
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
    ShowBar(0,0,false);
    if (ShowTotalCopySize)
    {
      ShowBar(CurCopySize,TotalCopySize,true);
      ShowTitle(FALSE);
    }
  }
  PreRedrawParam.Flags=Flags;
  PreRedrawParam.Param1=this;
  PreRedrawParam.Param2=Src;
  PreRedrawParam.Param3=Dest;
}


int ShellCopy::ShellCopyConvertWildcards(char *Src,char *Dest)
{
  char WildName[2*NM],*CurWildPtr,*DestNamePtr,*SrcNamePtr;
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

int ShellCopy::DeleteAfterMove(char *Name,int Attr)
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
    SetFileAttributes(Name,0);
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


int ShellCopy::ShellCopyFile(char *SrcName,WIN32_FIND_DATA *SrcData,
                             char *DestName,DWORD DestAttr,int Append)
{
  if ((ShellCopy::Flags&FCOPY_LINK))
    return(MkLink(SrcName,DestName) ? COPY_SUCCESS:COPY_FAILURE);
  if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && Opt.UseSystemCopy && !Append)
  {
    if (!Opt.CopyOpened)
    {
      HANDLE SrcHandle=CreateFile(SrcName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
      if (SrcHandle==INVALID_HANDLE_VALUE)
        return(COPY_FAILURE);
      CloseHandle(SrcHandle);
    }
    return(ShellSystemCopy(SrcName,DestName,SrcData));
  }

  SECURITY_ATTRIBUTES sa;
  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(SrcName,&sa))
    return(COPY_CANCEL);
  int OpenMode=FILE_SHARE_READ;
  if (Opt.CopyOpened)
    OpenMode|=FILE_SHARE_WRITE;
  HANDLE SrcHandle=CreateFile(SrcName,GENERIC_READ,OpenMode,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
  if (SrcHandle==INVALID_HANDLE_VALUE)
    return(COPY_FAILURE);

  HANDLE DestHandle;
  DWORD AppendPos=0;

  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    if (DestAttr!=(DWORD)-1 && !Append)
      remove(DestName);
    DestHandle=CreateFile(DestName,GENERIC_WRITE,FILE_SHARE_READ,
                          (ShellCopy::Flags&FCOPY_COPYSECURITY) ? &sa:NULL,
                          Append ? OPEN_EXISTING:CREATE_ALWAYS,
                          SrcData->dwFileAttributes|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
    if (DestHandle==INVALID_HANDLE_VALUE)
    {
      DWORD LastError=GetLastError();
      CloseHandle(SrcHandle);
      SetLastError(LastError);
      return(COPY_FAILURE);
    }

    if (Append && (AppendPos=SetFilePointer(DestHandle,0,NULL,FILE_END))==0xFFFFFFFF)
    {
      DWORD LastError=GetLastError();
      CloseHandle(SrcHandle);
      CloseHandle(DestHandle);
      SetLastError(LastError);
      return(COPY_FAILURE);
    }
  }

  int64 WrittenSize(0,0);
  while (1)
  {
    if (CheckForEsc())
    {
      CloseHandle(SrcHandle);
      if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
      {
        if (Append)
        {
          SetFilePointer(DestHandle,AppendPos,NULL,FILE_BEGIN);
          SetEndOfFile(DestHandle);
        }
        CloseHandle(DestHandle);
        if (!Append)
        {
          SetFileAttributes(DestName,0);
          DeleteFile(DestName);
        }
      }
      return(COPY_CANCEL);
    }
    DWORD BytesRead,BytesWritten;

    /* $ 23.10.2000 VVM
       + ������������ ����� ����������� */

    if (CopyBufSize < CopyBufferSize)
      StartTime=clock();
    UINT OldErrMode=SetErrorMode(SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);
    while (!ReadFile(SrcHandle,CopyBuffer,CopyBufSize,&BytesRead,NULL))
    {
      CopyTime+= (clock() - CopyStartTime);
      int MsgCode = Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                            MSG(MCopyReadError),SrcName,
                            MSG(MRetry),MSG(MCancel));
      CopyStartTime = clock();
      if (MsgCode==0)
        continue;
      DWORD LastError=GetLastError();
      CloseHandle(SrcHandle);
      if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
      {
        if (Append)
        {
          SetFilePointer(DestHandle,AppendPos,NULL,FILE_BEGIN);
          SetEndOfFile(DestHandle);
        }
        CloseHandle(DestHandle);
        if (!Append)
        {
          SetFileAttributes(DestName,0);
          DeleteFile(DestName);
        }
      }
      ShowBar(0,0,false);
      ShowTitle(FALSE);
      SetLastError(LastError);
      SetErrorMode(OldErrMode);
      // return COPY_FAILUREREAD;
      return COPY_FAILURE;
    }
    SetErrorMode(OldErrMode);
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
              CopyStartTime = clock();
              if (MsgCode==2)
              {
                CloseHandle(SrcHandle);
                if (!Append)
                {
                  SetFileAttributes(DestName,0);
                  DeleteFile(DestName);
                }
                return(COPY_FAILURE);
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
          AskCode = AskOverwrite(SrcData,DestName,0xFFFFFFFF,FALSE,((ShellCopy::Flags&FCOPY_MOVE)?TRUE:FALSE),1,Append,RetCode);
          CopyStartTime = clock();
          if (!AskCode)
          {
            CloseHandle(SrcHandle);
            return(COPY_CANCEL);
          }
          char DestDir[NM],*ChPtr;
          strcpy(DestDir,DestName);
          if ((ChPtr=strrchr(DestDir,'\\'))!=NULL)
          {
            *ChPtr=0;
            CreatePath(DestDir);
          }
          DestHandle=CreateFile(DestName,GENERIC_WRITE,FILE_SHARE_READ,NULL,
                                Append ? OPEN_EXISTING:CREATE_ALWAYS,
                                SrcData->dwFileAttributes|FILE_FLAG_SEQUENTIAL_SCAN,NULL);

          if (DestHandle==INVALID_HANDLE_VALUE ||
              Append && SetFilePointer(DestHandle,0,NULL,FILE_END)==0xFFFFFFFF)
          {
            DWORD LastError=GetLastError();
            CloseHandle(SrcHandle);
            CloseHandle(DestHandle);
            SetLastError(LastError);
            return(COPY_FAILURE);
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
            SetFilePointer(DestHandle,AppendPos,NULL,FILE_BEGIN);
            SetEndOfFile(DestHandle);
          }
          CloseHandle(DestHandle);
          if (!Append)
          {
            SetFileAttributes(DestName,0);
            DeleteFile(DestName);
          }
          ShowBar(0,0,false);
          ShowTitle(FALSE);
          SetLastError(LastError);
          if (SplitSkipped)
            return(COPY_NEXT);
          return(SplitCancelled ? COPY_CANCEL:COPY_FAILURE);
        }
        break;
      }
    }
    else
    {
      BytesWritten=BytesRead; // �� ������� ���������� ���������� ���������� ����
    }

    if ((CopyBufSize < CopyBufferSize) && (BytesWritten==CopyBufSize))
    {
      StopTime=clock();
      if ((StopTime - StartTime) < 1000)
      {
        CopyBufSize*=2;
        if (CopyBufSize > CopyBufferSize)
          CopyBufSize=CopyBufferSize;
      }
    } /* if */
    /* VVM $ */

    WrittenSize+=BytesWritten;
    int64 FileSize(SrcData->nFileSizeHigh,SrcData->nFileSizeLow);
    ShowBar(WrittenSize,FileSize,false);
    if (ShowTotalCopySize)
    {
      CurCopySize+=BytesWritten;
      ShowBar(CurCopySize,TotalCopySize,true);
      ShowTitle(FALSE);
    }
  }
  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    SetFileTime(DestHandle,NULL,NULL,&SrcData->ftLastWriteTime);
    CloseHandle(SrcHandle);
    CloseHandle(DestHandle);

    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS &&
        (SrcData->dwFileAttributes & (FA_HIDDEN|FA_SYSTEM|FA_RDONLY)))
      ShellSetAttr(DestName,SrcData->dwFileAttributes);
  }
  else
    CloseHandle(SrcHandle);

  return(COPY_SUCCESS);
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

void ShellCopy::ShowBar(int64 WrittenSize,int64 TotalSize,bool TotalBar)
{
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
  if (ShowCopyTime &&
      (!ShowTotalCopySize || TotalBar) &&
      (clock() - LastShowTime > 1000))
  {
    LastShowTime = clock();
    CopyTime+= (clock() - CopyStartTime);
    CopyStartTime = clock();
    int WorkTime = CopyTime/1000;
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
/* VVM $ */
}


void ShellCopy::SetDestDizPath(char *DestPath)
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


int ShellCopy::AskOverwrite(WIN32_FIND_DATA *SrcData,char *DestName,
               DWORD DestAttr,int SameName,int Rename,int AskAppend,
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
      char SrcFileStr[512],DestFileStr[512],DateText[20],TimeText[20];
      ConvertDate(&SrcData->ftLastWriteTime,DateText,TimeText,8);

      int64 SrcSize(SrcData->nFileSizeHigh,SrcData->nFileSizeLow);
      char SrcSizeText[20];
      SrcSize.itoa(SrcSizeText);
      int64 DestSize(DestData.nFileSizeHigh,DestData.nFileSizeLow);
      char DestSizeText[20];
      DestSize.itoa(DestSizeText);
      /* $ 04.08.2000 SVS
         ����� "Only newer file(s)"
      */
      if((ShellCopy::Flags&FCOPY_ONLYNEWERFILES))
      {
        // ������� �����
        int RetCompare=CompareFileTime(&DestData.ftLastWriteTime,&SrcData->ftLastWriteTime);
        if(RetCompare < 0)
          MsgCode=0;
        else
          MsgCode=2;
      }
      else
      {
        sprintf(SrcFileStr,"%-17s %11.11s %s %s",MSG(MCopySource),SrcSizeText,DateText,TimeText);
        ConvertDate(&DestData.ftLastWriteTime,DateText,TimeText,8);
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

        int64 SrcSize(SrcData->nFileSizeHigh,SrcData->nFileSizeLow);
        char SrcSizeText[20];
        SrcSize.itoa(SrcSizeText);
        int64 DestSize(DestData.nFileSizeHigh,DestData.nFileSizeLow);
        char DestSizeText[20];
        DestSize.itoa(DestSizeText);

        ConvertDate(&SrcData->ftLastWriteTime,DateText,TimeText,8);
        sprintf(SrcFileStr,"%-17s %11.11s %s %s",MSG(MCopySource),SrcSizeText,DateText,TimeText);
        ConvertDate(&DestData.ftLastWriteTime,DateText,TimeText,8);
        sprintf(DestFileStr,"%-17s %11.11s %s %s",MSG(MCopyDest),DestSizeText,DateText,TimeText);

        SetMessageHelp("CopyFiles");
        MsgCode=Message(MSG_DOWN|MSG_WARNING,6,MSG(MWarning),
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
    SetFileAttributes(DestName,0);
  return(TRUE);
}


int ShellCopy::GetSecurity(char *FileName,SECURITY_ATTRIBUTES *sa)
{
  char AnsiName[NM];
  SECURITY_INFORMATION si=DACL_SECURITY_INFORMATION;
  SECURITY_DESCRIPTOR *sd=(SECURITY_DESCRIPTOR *)sddata;
  DWORD Needed;
  OemToChar(FileName,AnsiName);
  if (!GetFileSecurity(AnsiName,si,sd,sizeof(sddata),&Needed))
  {
    sd=NULL;
    int LastError=GetLastError();
    if (LastError!=ERROR_SUCCESS && LastError!=ERROR_FILE_NOT_FOUND &&
        LastError!=ERROR_CALL_NOT_IMPLEMENTED &&
        Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                MSG(MCannotGetSecurity),FileName,MSG(MOk),MSG(MCancel))==1)
      return(FALSE);
  }
  sa->nLength=sizeof(SECURITY_ATTRIBUTES);
  sa->lpSecurityDescriptor=sd;
  sa->bInheritHandle=FALSE;
  return(TRUE);
}


int ShellCopy::SetSecurity(char *FileName,SECURITY_ATTRIBUTES *sa)
{
  char AnsiName[NM];
  SECURITY_INFORMATION si=DACL_SECURITY_INFORMATION;
  OemToChar(FileName,AnsiName);
  if (!SetFileSecurity(AnsiName,si,sa->lpSecurityDescriptor))
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

int ShellCopy::ShellSystemCopy(char *SrcName,char *DestName,WIN32_FIND_DATA *SrcData)
{
  static COPYFILEEX pCopyFileEx=NULL;
  static int LoadAttempt=FALSE;

  if (!LoadAttempt && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
  {
    HMODULE hKernel=GetModuleHandle("KERNEL32.DLL");
    if (hKernel)
      pCopyFileEx=(COPYFILEEX)GetProcAddress(hKernel,"CopyFileExA");
    LoadAttempt=TRUE;
  }


  SECURITY_ATTRIBUTES sa;
  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(SrcName,&sa))
    return(COPY_CANCEL);

  ShellCopyMsg(SrcName,DestName,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);
  if (pCopyFileEx)
  {
    BOOL Cancel=0;
    StartCopySizeEx=CurCopySize;
    if (!pCopyFileEx(SrcName,DestName,CopyProgressRoutine,NULL,&Cancel,0))
      return(GetLastError()==ERROR_REQUEST_ABORTED ? COPY_CANCEL:COPY_FAILURE);
  }
  else
  {
    if (ShowTotalCopySize)
    {
      int64 AddSize(SrcData->nFileSizeHigh,SrcData->nFileSizeLow);
      CurCopySize+=AddSize;
      ShowBar(CurCopySize,TotalCopySize,true);
      ShowTitle(FALSE);
    }
    if (!CopyFile(SrcName,DestName,FALSE))
      return(COPY_FAILURE);
  }

  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !SetSecurity(DestName,&sa))
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
  if (ShowTotalCopySize && dwStreamNumber==1)
  {
    int64 AddSize(TotalBytesTransferred.u.HighPart,TotalBytesTransferred.u.LowPart);
    CurCopySize=StartCopySizeEx+AddSize;
    ShellCopy::ShowBar(CurCopySize,TotalCopySize,true);
    ShellCopy::ShowTitle(FALSE);
  }
  int64 TransferredSize(TotalBytesTransferred.u.HighPart,TotalBytesTransferred.u.LowPart);
  int64 TotalSize(TotalFileSize.u.HighPart,TotalFileSize.u.LowPart);
  ShellCopy::ShowBar(TransferredSize,TotalSize,false);
  return(CheckForEsc() ? PROGRESS_CANCEL:PROGRESS_CONTINUE);
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


int ShellCopy::IsSameDisk(char *SrcPath,char *DestPath)
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

  TotalCopySize=CurCopySize=0;

  ShellCopyMsg(NULL,"",MSG_LEFTALIGN);

  SrcPanel->GetSelName(NULL,FileAttr);
  while (SrcPanel->GetSelName(SelName,FileAttr,SelShortName))
    if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
    {
      unsigned long DirCount,FileCount,ClusterSize;
      int64 FileSize,CompressedSize,RealFileSize;
      ShellCopyMsg(NULL,SelName,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);
      if (!GetDirInfo("",SelName,DirCount,FileCount,FileSize,CompressedSize,
                      RealFileSize,ClusterSize,0xffffffff,FALSE))
      {
        ShowTotalCopySize=false;
        return(false);
      }
      TotalCopySize+=FileSize;
    }
    else
    {
      int64 FileSize;
      if (SrcPanel->GetLastSelectedSize(&FileSize)!=-1)
        TotalCopySize+=FileSize;
    }

  InsertCommas(TotalCopySize,TotalCopySizeText);
  return(true);
}


void ShellCopy::ShowTitle(int FirstTime)
{
  static char PrevTitle[200];
  char Title[200];
  if (FirstTime)
    *PrevTitle=0;
  strcpy(Title,StaticMove ? MSG(MCopyMovingTitle):MSG(MCopyCopyingTitle));

  if (ShowTotalCopySize && !FirstTime)
  {
    int64 CopySize=CurCopySize>>8,TotalSize=TotalCopySize>>8;
    /* $ 03.07.2000 IS
      ���������� �������� ������� ��� �����������/��������
    ����:
    char Percent[50];
    sprintf(Percent," {%d%%}",ToPercent(CopySize.LowPart,TotalSize.LowPart));
    strcat(Title,Percent);
    */
    char Percent[200];
    sprintf(Percent,"{%d%%} %s",ToPercent(CopySize.PLow(),TotalSize.PLow()),Title);
    strcpy(Title,Percent);
    /* IS $ */
  }
  if (strcmp(PrevTitle,Title)!=0)
  {
    SetFarTitle(Title);
    strcpy(PrevTitle,Title);
  }
}


int ShellCopy::CmpFullNames(char *Src,char *Dest)
{
  char SrcFullName[1024],DestFullName[1024];
  int I;

  if (ConvertNameToReal(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
    return(2);
  if (ConvertNameToReal(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
    return(2);

  for (I=strlen(SrcFullName)-1;I>0 && SrcFullName[I]=='.';I--)
    SrcFullName[I]=0;

  if(SrcFullName[strlen(SrcFullName)-1] == '\\')
    SrcFullName[strlen(SrcFullName)-1]=0;

  for (I=strlen(DestFullName)-1;I>0 && DestFullName[I]=='.';I--)
    DestFullName[I]=0;

  if(DestFullName[strlen(DestFullName)-1] == '\\')
    DestFullName[strlen(DestFullName)-1]=0;

//_SVS(SysLog("\nSrcFullName ='%s'\nDestFullName='%s'",SrcFullName,DestFullName));
  if (LocalStricmp(SrcFullName,DestFullName)!=0)
    return(0);

  return(strcmp(PointToName(SrcFullName),PointToName(DestFullName))==0 ? 2:1);
}

// ����� ��� �������� SymLink ��� ���������.
int ShellCopy::MkSymLink(const char *SelName,const char *Dest,DWORD Flags)
{
  if (Flags&(FCOPY_LINK|FCOPY_VOLMOUNT))
  {
    char SrcFullName[NM], DestFullName[NM];
    char MsgBuf[NM],MsgBuf2[NM];

    if(SelName[1] == ':' && SelName[2] == 0) // C:
    {
//      if(Flags&FCOPY_VOLMOUNT)
      {
        strcpy(SrcFullName,SelName);
        strcat(SrcFullName,"\\");
      }
/*
    ��� ����� - �� ����� ����� ���������!
    �.�. ���� � �������� SelName �������� "C:", �� � ���� ����� ����������
    ��������� ���� ����� - � symlink`� �� volmount
*/
      Flags&=~FCOPY_LINK;
      Flags|=FCOPY_VOLMOUNT;
    }
    else
      if (ConvertNameToFull(SelName,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
        return 0;
//_SVS(SysLog("Src: ConvertNameToFull('%s','%s')",SelName,SrcFullName));

    if (ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
      return 0;
//_SVS(SysLog("Dst: ConvertNameToFull('%s','%s')",Dest,DestFullName));

//    char *EndDestFullName=DestFullName+strlen(DestFullName);
    if(DestFullName[strlen(DestFullName)-1] == '\\' && !(Flags&FCOPY_VOLMOUNT))
    {
//      AddEndSlash(DestFullName);
      strcat(DestFullName,SelName);
    }

    if(Flags&FCOPY_VOLMOUNT)
    {
      AddEndSlash(SrcFullName);
      AddEndSlash(DestFullName);
    }

    int JSAttr=GetFileAttributes(DestFullName);
//_SVS(SysLog("DestFullName='%s' JSAttr=0x%08X",DestFullName,JSAttr));
    if(JSAttr != -1 && (JSAttr&FILE_ATTRIBUTE_DIRECTORY)==FILE_ATTRIBUTE_DIRECTORY) // ���������� �����?
    {
      if(IsFolderNotEmpty(DestFullName)) // � ������?
      {
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
          strcat(DestFullName,SelName);

        int JSAttr=GetFileAttributes(DestFullName);
//_SVS(SysLog("AddEndSlash: DestFullName='%s' JSAttr=0x%08X",DestFullName,JSAttr));
        if(JSAttr != -1) // � ����� ���� ����???
        {
//_SVS(SysLog("Ops!"));
          if(IsFolderNotEmpty(DestFullName)) // � ������?
          {
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
            return 0; // ���������� � ����
          }
        }
        else // �������.
        {
          if (CreateDirectory(DestFullName,NULL))
            TreeList::AddTreeName(DestFullName);
          else
            CreatePath(DestFullName);
//_SVS(SysLog("Create folder '%s'",DestFullName));
        }
        if(GetFileAttributes(DestFullName) == -1) // ���. ��� ����� ���� �����.
        {
          if(!(Flags&FCOPY_NOSHOWMSGLINK))
          {
            Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),
                      MSG(MCopyCannotCreateFolder),
                      DestFullName,MSG(MOk));
          }
          return 0;
        }
      }
    }
    else
    {
      if (CreateDirectory(DestFullName,NULL))
        TreeList::AddTreeName(DestFullName);
      else
        CreatePath(DestFullName);
//_SVS(SysLog("Create folder '%s'",DestFullName));
      if(GetFileAttributes(DestFullName) == -1) // ���. ��� ����� ���� �����.
      {
        if(!(Flags&FCOPY_NOSHOWMSGLINK))
        {
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
                   MSG(MCopyCannotCreateFolder),DestFullName,MSG(MOk));
        }
        return 0;
      }
    }
//_SVS(SysLog("('%s','%s')",SrcFullName,DestFullName));
    if(Flags&FCOPY_LINK)
    {
      if(CreateJunctionPoint(SrcFullName,DestFullName))
      {
//_SVS(SysLog("Ok: CreateJunctionPoint('%s','%s')",SrcFullName,DestFullName));
        return 1;
      }
      else
      {
        if(!(Flags&FCOPY_NOSHOWMSGLINK))
        {
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
                 MSG(MCopyCannotCreateJunction),DestFullName,MSG(MOk));
//_SVS(SysLog("Fail: CreateJunctionPoint('%s','%s')",SrcFullName,DestFullName));
        }
        return 0;
      }
    }
    else if(Flags&FCOPY_VOLMOUNT)
    {
      int ResultVol=CreateVolumeMountPoint(SrcFullName,DestFullName);
      if(!ResultVol)
      {
//_SVS(SysLog("Ok: CreateVolumeMountPoint('%s','%s')",SrcFullName,DestFullName));
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
//_SVS(SysLog("Fail: CreateVolumeMountPoint('%s','%s')",SrcFullName,DestFullName));
        return 0;
      }
    }
  }
  return 2;
}

/*
  �������� ������ SetFileAttributes() ���
  ����������� ����������� ���������
*/
int ShellCopy::ShellSetAttr(char *Dest,DWORD Attr)
{
  char Root[1024];
  char FSysNameDst[NM];
  DWORD FileSystemFlagsDst;

  ConvertNameToFull(Dest,Root, sizeof(Root));
  GetPathRoot(Root,Root);
  if(GetFileAttributes(Root) == -1)
    return FALSE;

//_SVS(SysLog("Copy: 0x%08X Dest='%s' Root='%s'",Attr,Dest,Root));
  if(GetVolumeInformation(Root,NULL,0,NULL,NULL,&FileSystemFlagsDst,FSysNameDst,sizeof(FSysNameDst)))
  {
//_SVS(SysLog("Copy: %s (0x%08X) %c%c",FSysNameDst,FileSystemFlagsDst,(FileSystemFlagsDst&FS_FILE_COMPRESSION?'C':'.'),(FileSystemFlagsDst&FILE_SUPPORTS_ENCRYPTION?'E':'.')));
     if(!(FileSystemFlagsDst&FS_FILE_COMPRESSION))
       Attr&=~FILE_ATTRIBUTE_COMPRESSED;
     if(!(FileSystemFlagsDst&FILE_SUPPORTS_ENCRYPTION))
       Attr&=~FILE_ATTRIBUTE_ENCRYPTED;
    SetFileAttributes(Dest,Attr);
    // ��� �����������/�������� ���������� FILE_ATTRIBUTE_ENCRYPTED
    // ��� ��������, ���� �� ����
    if((FileSystemFlagsDst&FILE_SUPPORTS_ENCRYPTION) &&
        (Attr&(FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_DIRECTORY)) == (FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_DIRECTORY))
      if(!ESetFileEncryption(Dest,1,0))
        return FALSE;
    return TRUE;
  }
  return FALSE;
}
