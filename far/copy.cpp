/*
copy.cpp

����������� ������

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
#include "filefilter.hpp"

/* ����� ����� �������� ������������ */
extern long WaitUserTime;
/* ��� ����, ��� �� ����� ��� �������� ������������ ������, � remaining/speed ��� */
static long OldCalcTime;

/* �������� ��� ���������� ��������-����. */
#define COPY_TIMEOUT 200

// ������ � ������ �������
#define DLG_HEIGHT 16
#define DLG_WIDTH 76

#define SDDATA_SIZE   64000

enum {COPY_BUFFER_SIZE  = 0x10000};

/* $ 30.01.2001 VVM
   + ��������� ��� ������ ������
   + ������� ��������� */
enum {
  COPY_RULE_NUL    = 0x0001,
  COPY_RULE_FILES  = 0x0002,
};

static int TotalFilesToProcess;

static int ShowCopyTime;
static clock_t CopyStartTime;
static clock_t LastShowTime;
/* VVM $ */
static int OrigScrX,OrigScrY;

static DWORD WINAPI CopyProgressRoutine(LARGE_INTEGER TotalFileSize,
       LARGE_INTEGER TotalBytesTransferred,LARGE_INTEGER StreamSize,
       LARGE_INTEGER StreamBytesTransferred,DWORD dwStreamNumber,
       DWORD dwCallbackReason,HANDLE hSourceFile,HANDLE hDestinationFile,
       LPVOID lpData);

static int BarX,BarY,BarLength;

static unsigned __int64 TotalCopySize, TotalCopiedSize; // ����� ��������� �����������
static unsigned __int64 CurCopiedSize;                  // ������� ��������� �����������
static unsigned __int64 TotalSkippedSize;               // ����� ������ ����������� ������
static unsigned __int64 TotalCopiedSizeEx;
static int   CountTarget;                    // ����� �����.
static int CopySecurityCopy=-1;
static int CopySecurityMove=-1;
static bool ShowTotalCopySize;
static int StaticMove;
static char TotalCopySizeText[32];
static ConsoleTitle *StaticCopyTitle=NULL;
static BOOL NT5, NT;

/* $ 15.04.2005 KM
   ��������� �� ������ ������� ��������
*/
static FileFilter *Filter;
static int UseFilter=FALSE;
/* KM $*/

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

enum enumShellCopy {
  ID_SC_TITLE=0,
  ID_SC_TARGETTITLE,
  ID_SC_TARGETEDIT,
  ID_SC_SEPARATOR1,
  ID_SC_ACTITLE,
  ID_SC_ACLEAVE,
  ID_SC_ACCOPY,
  ID_SC_ACINHERIT,
  ID_SC_SEPARATOR2,
  ID_SC_ONLYNEWER,
  ID_SC_COPYSYMLINK,
  ID_SC_MULTITARGET,
  ID_SC_SEPARATOR3,
  ID_SC_USEFILTER,
  ID_SC_SEPARATOR4,
  ID_SC_BTNCOPY,
  ID_SC_BTNTREE,
  ID_SC_BTNFILTER,
  ID_SC_BTNCANCEL,
  ID_SC_SOURCEFILENAME,
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

  char CopyStr[100];
  char SelNameShort[NM],SelName[NM];

  int DestPlugin;
  int AddSlash=FALSE;

  Filter=NULL;
  sddata=NULL;

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

  // �������� ������ �������
  Filter=new FileFilter(SrcPanel,FFT_COPY);

  sddata=new char[SDDATA_SIZE]; // Security 16000?

  /* $ 26.05.2001 OT ��������� ����������� ������� �� ����� ����������� */
  _tran(SysLog("call (*FrameManager)[0]->LockRefresh()"));
  (*FrameManager)[0]->Lock();
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

  ShowTotalCopySize=Opt.CMOpt.CopyShowTotal != 0;

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

  int SizeBuffer=2048;
  if(DestPanelMode == PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    DestPanel->GetOpenPluginInfo(&Info);
    int LenCurDir=(int)strlen(NullToEmpty(Info.CurDir));
    if(SizeBuffer < LenCurDir)
      SizeBuffer=LenCurDir;
  }
  SizeBuffer+=NM; // ������� :-)

  /* $ 03.08.2001 IS
       CopyDlgValue - � ���� ���������� ������ �������� ������� �� �������,
       ������ ��� ���������� �������� ����������, � CopyDlg[2].Data �� �������.
  */
  char *CopyDlgValue=(char *)alloca(SizeBuffer);
  char *Dlg2Value=(char *)alloca(SizeBuffer);
  char *DestDir=(char *)alloca(SizeBuffer);
  char *InitDestDir=(char *)alloca(SizeBuffer);
  char *SrcDir=(char *)alloca(SizeBuffer);

  *Dlg2Value=0;

  // ***********************************************************************
  // *** Prepare Dialog Controls
  // ***********************************************************************
  const char *HistoryName="Copy";
  /* $ 03.08.2001 IS ������� ����� �����: ����������������� */
  static struct DialogData CopyDlgData[]={
  /* 00 */  DI_DOUBLEBOX,3,1,DLG_WIDTH-4,DLG_HEIGHT-2,0,0,0,0,(char *)MCopyDlgTitle,
  /* 01 */  DI_TEXT,5,2,0,2,0,0,0,0,(char *)MCMLTargetTO,
  /* 02 */  DI_EDIT,5,3,70,3,1,(DWORD_PTR)HistoryName,DIF_HISTORY|DIF_VAREDIT|DIF_EDITEXPAND|DIF_USELASTHISTORY/*|DIF_EDITPATH*/,0,"",
  /* 03 */  DI_TEXT,3,4,0,4,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */  DI_TEXT,5,5,0,5,0,0,0,0,(char *)MCopySecurity,
  /* 05 */  DI_RADIOBUTTON,5,5,0,5,0,0,DIF_GROUP,0,(char *)MCopySecurityLeave,
  /* 06 */  DI_RADIOBUTTON,5,5,0,5,0,0,0,0,(char *)MCopySecurityCopy,
  /* 07 */  DI_RADIOBUTTON,5,5,0,5,0,0,0,0,(char *)MCopySecurityInherit,
  /* 08 */  DI_TEXT,3,6,0,6,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 09 */  DI_CHECKBOX,5,7,0,7,0,0,0,0,(char *)MCopyOnlyNewerFiles,
  /* 10 */  DI_CHECKBOX,5,8,0,8,0,0,0,0,(char *)MCopySymLinkContents,
  /* 11 */  DI_CHECKBOX,5,9,0,9,0,0,0,0,(char *)MCopyMultiActions,
  /* 12 */  DI_TEXT,3,10,0,10,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 13 */  DI_CHECKBOX,5,11,0,11,0,0,0,0,(char *)MCopyUseFilter,
  /* 14 */  DI_TEXT,3,12,0,12,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 15 */  DI_BUTTON,0,13,0,13,0,0,DIF_CENTERGROUP,1,(char *)MCopyDlgCopy,
  /* 16 */  DI_BUTTON,0,13,0,13,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MCopyDlgTree,
  /* 17 */  DI_BUTTON,0,13,0,13,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MCopySetFilter,
  /* 18 */  DI_BUTTON,0,13,0,13,0,0,DIF_CENTERGROUP,0,(char *)MCopyDlgCancel,
  /* 19 */  DI_TEXT,5,2,0,2,0,0,DIF_SHOWAMPERSAND,0,"",
  };
  MakeDialogItems(CopyDlgData,CopyDlg);

  CopyDlg[ID_SC_MULTITARGET].Selected=Opt.CMOpt.MultiCopy;

  // ������������ ������. KM
  CopyDlg[ID_SC_USEFILTER].Selected=UseFilter;

  CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData=Dlg2Value;
  CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrLength=SizeBuffer;

  CopyDlg[ID_SC_ACLEAVE].X1 = CopyDlg[ID_SC_ACTITLE].X1 + (int)strlen(CopyDlg[ID_SC_ACTITLE].Data) - (strchr(CopyDlg[ID_SC_ACTITLE].Data, '&')?1:0) + 1;
  CopyDlg[ID_SC_ACCOPY].X1 = CopyDlg[ID_SC_ACLEAVE].X1 + (int)strlen(CopyDlg[ID_SC_ACLEAVE].Data) - (strchr(CopyDlg[ID_SC_ACLEAVE].Data, '&')?1:0) + 5;
  CopyDlg[ID_SC_ACINHERIT].X1 = CopyDlg[ID_SC_ACCOPY].X1 + (int)strlen(CopyDlg[ID_SC_ACCOPY].Data) - (strchr(CopyDlg[ID_SC_ACCOPY].Data, '&')?1:0) + 5;

  if(Link)
  {
    CopyDlg[ID_SC_COPYSYMLINK].Selected=0;
    CopyDlg[ID_SC_COPYSYMLINK].Flags|=DIF_DISABLE;
    CDP.CopySecurity=1;
  }
  else if(Move)  // ������ ��� �������
  {
    //   2 - Default
    //   1 - Copy access rights
    //   0 - Inherit access rights
    CDP.CopySecurity=2;

    // ������� ����� "Inherit access rights"?
    // CSO_MOVE_SETINHERITSECURITY - ���������� ����
    if((Opt.CMOpt.CopySecurityOptions&CSO_MOVE_SETINHERITSECURITY) == CSO_MOVE_SETINHERITSECURITY)
      CDP.CopySecurity=0;
    else if (Opt.CMOpt.CopySecurityOptions&CSO_MOVE_SETCOPYSECURITY)
      CDP.CopySecurity=1;

    // ������ ���������� �����������?
    if(CopySecurityMove != -1 && Opt.CMOpt.CopySecurityOptions&CSO_MOVE_SESSIONSECURITY)
      CDP.CopySecurity=CopySecurityMove;
    else
      CopySecurityMove=CDP.CopySecurity;
  }
  else // ������ ��� �����������
  {
    //   2 - Default
    //   1 - Copy access rights
    //   0 - Inherit access rights
    CDP.CopySecurity=2;

    // ������� ����� "Inherit access rights"?
    // CSO_COPY_SETINHERITSECURITY - ���������� ����
    if((Opt.CMOpt.CopySecurityOptions&CSO_COPY_SETINHERITSECURITY) == CSO_COPY_SETINHERITSECURITY)
      CDP.CopySecurity=0;
    else if (Opt.CMOpt.CopySecurityOptions&CSO_COPY_SETCOPYSECURITY)
      CDP.CopySecurity=1;

    // ������ ���������� �����������?
    if(CopySecurityCopy != -1 && Opt.CMOpt.CopySecurityOptions&CSO_COPY_SESSIONSECURITY)
      CDP.CopySecurity=CopySecurityCopy;
    else
      CopySecurityCopy=CDP.CopySecurity;
  }

  // ��� ������ ����������
  if(CDP.CopySecurity)
  {
    if(CDP.CopySecurity == 1)
    {
      ShellCopy::Flags|=FCOPY_COPYSECURITY;
      CopyDlg[ID_SC_ACCOPY].Selected=1;
    }
    else
    {
      ShellCopy::Flags|=FCOPY_LEAVESECURITY;
      CopyDlg[ID_SC_ACLEAVE].Selected=1;
    }
  }
  else
  {
    ShellCopy::Flags&=~(FCOPY_COPYSECURITY|FCOPY_LEAVESECURITY);
    CopyDlg[ID_SC_ACINHERIT].Selected=1;
  }

  if (CDP.SelCount==1)
  { // SelName & FileAttr ��� ��������� (��. � ����� ������ �������)

    // ���� ������� � �� ����, �� ������������, ��� ����� ������� �������
    if(Link && NT5 && (CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && DestPanelMode == NORMAL_PANEL)
    {
      CDP.OnlyNewerFiles=CopyDlg[ID_SC_ONLYNEWER].Selected=1;
      CDP.FolderPresent=TRUE;
    }
    else
      CDP.OnlyNewerFiles=CopyDlg[ID_SC_ONLYNEWER].Selected=0;

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

    // ���� �������� ��������� ����, �� ��������� ������������ ������
    if (!(CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY))
    {
      CopyDlg[ID_SC_USEFILTER].Selected=0;
      CopyDlg[ID_SC_USEFILTER].Flags|=DIF_DISABLE;
    }
  }
  else // �������� ���������!
  {
    int NOper=MCopyFiles;
         if (Move) NOper=MMoveFiles;
    else if (Link) NOper=MLinkFiles;

    // ��������� ����� - ��� ���������
    char StrItems[32];
    itoa(CDP.SelCount,StrItems,10);
    int LenItems=(int)strlen(StrItems);
    int NItems=MCMLItemsA;
    if((LenItems >= 2 && StrItems[LenItems-2] == '1') ||
        StrItems[LenItems-1] >= '5' ||
        StrItems[LenItems-1] == '0')
      NItems=MCMLItemsS;
    else if(StrItems[LenItems-1] == '1')
      NItems=MCMLItems0;
    sprintf(CopyStr,MSG(NOper),CDP.SelCount,MSG(NItems));
  }
  sprintf(CopyDlg[ID_SC_SOURCEFILENAME].Data,"%.65s",CopyStr);

  // ��������� ���������
  strcpy(CopyDlg[ID_SC_ONLYNEWER].Data,MSG(Link?MCopySymLink:MCopyOnlyNewerFiles));
  strcpy(CopyDlg[ID_SC_TITLE].Data,MSG(Move?MMoveDlgTitle :(Link?MLinkDlgTitle:MCopyDlgTitle)));
  strcpy(CopyDlg[ID_SC_BTNCOPY].Data,MSG(Move?MCopyDlgRename:(Link?MCopyDlgLink:MCopyDlgCopy)));

  if (Link)
  {
    // ���������� ����� ��� �������, ���� OS < NT2000.
    if(!NT5 || ((CurrentOnly || CDP.SelCount==1) && !(CDP.FileAttr&FILE_ATTRIBUTE_DIRECTORY)))
    {
      CopyDlg[ID_SC_ONLYNEWER].Flags|=DIF_DISABLE;
      CDP.OnlyNewerFiles=CopyDlg[ID_SC_ONLYNEWER].Selected=0;
    }
    // ���������� ����� ��� ����������� �����.
    CopyDlg[ID_SC_ACCOPY].Flags|=DIF_DISABLE;
    CopyDlg[ID_SC_ACINHERIT].Flags|=DIF_DISABLE;
    CopyDlg[ID_SC_ACLEAVE].Flags|=DIF_DISABLE;
  }
  else if(DestPanelMode == PLUGIN_PANEL)
  {
    // ���� ��������������� ������ - ������, �� �������� OnlyNewer //?????
    CDP.CopySecurity=2;
    CDP.OnlyNewerFiles=0;
    CopyDlg[ID_SC_ONLYNEWER].Selected=0;
    CopyDlg[ID_SC_ACCOPY].Selected=0;
    CopyDlg[ID_SC_ACINHERIT].Selected=0;
    CopyDlg[ID_SC_ACLEAVE].Selected=1;
    CopyDlg[ID_SC_ONLYNEWER].Flags|=DIF_DISABLE;
    CopyDlg[ID_SC_ACCOPY].Flags|=DIF_DISABLE;
    CopyDlg[ID_SC_ACINHERIT].Flags|=DIF_DISABLE;
    CopyDlg[ID_SC_ACLEAVE].Flags|=DIF_DISABLE;
  }

  DestPanel->GetCurDir(DestDir);
  SrcPanel->GetCurDir(SrcDir);

  if (CurrentOnly)
  /* $ 23.03.2002 IS
     ��� ����������� ������ �������� ��� �������� ����� ��� ��� � �������,
     ���� ��� �������� �����������.
  */
  {
    strcpy((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,SelName);
    if(strpbrk((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,",;"))
    {
      Unquote((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);     // ������ ��� ������ �������
      InsertQuote((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData); // ������� � �������, �.�. ����� ���� �����������
    }
  }
  /* IS $ */
  else
    switch(DestPanelMode)
    {
      case NORMAL_PANEL:
        if ((*DestDir==0 || !DestPanel->IsVisible() || !stricmp(SrcDir,DestDir)) && CDP.SelCount==1)
          strcpy((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,SelName);
        else
        {
          AddEndSlash(strcpy((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,DestDir));
        }
        CDP.PluginFormat[0]=0;
        /* $ 19.07.2003 IS
           ���� ���� �������� �����������, �� ������� �� � �������, ���� �� ��������
           ������ ��� F5, Enter � �������, ����� ������������ ������� MultiCopy
        */
        if(strpbrk((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,",;"))
        {
          Unquote((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);     // ������ ��� ������ �������
          InsertQuote((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData); // ������� � �������, �.�. ����� ���� �����������
        }
        /* IS $ */
        break;
      case PLUGIN_PANEL:
        {
          struct OpenPluginInfo Info;
          DestPanel->GetOpenPluginInfo(&Info);
          /* $ 14.08.2000 SVS
             ������, ��������� �� 40 ��������... :-(
             � ����� ������������ ((char *)CopyDlg[2].Ptr.PtrData) �� ������ ���������...
             "%.40s:" -> "%s:"
          */
          sprintf((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,"%s:",NullToEmpty(Info.Format));
          /* SVS $ */
          while (strlen((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData)<2)
            strcat((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,":");
          strupr(xstrncpy(CDP.PluginFormat,(char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,sizeof(CDP.PluginFormat)-1));
        }
        break;
    }
  strcpy(InitDestDir,(char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);

  // ��� �������
  WIN32_FIND_DATA fd;

  //TODO: �������� ����� ���������� ������������ - ���� ����� ���������� � �������
  SrcPanel->GetSelName(NULL,CDP.FileAttr);
  while(SrcPanel->GetSelName(SelName,CDP.FileAttr,NULL,&fd))
  {
    if (UseFilter)
    {
      if (!Filter->FileInFilter(&fd))
        continue;
    }

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
    int SelectedSymLink=CopyDlg[ID_SC_ONLYNEWER].Selected;
    if(CDP.SelCount > 1 && !CDP.FilesPresent && CDP.FolderPresent)
      SelectedSymLink=1;
    if(!LinkRules(&CopyDlg[ID_SC_BTNCOPY].Flags,
                  &CopyDlg[ID_SC_ONLYNEWER].Flags,
                  &SelectedSymLink,
                  SrcDir,
                  (char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,
                  &CDP))
    {
      _LOGCOPYR(SysLog("return -> %d LinkRules() == 0",__LINE__));
      return;
    }
    CopyDlg[ID_SC_ONLYNEWER].Selected=SelectedSymLink;
  }

  // ����������� ������� " to"
  CopyDlg[ID_SC_TARGETTITLE].X1=CopyDlg[ID_SC_TARGETTITLE].X2=CopyDlg[ID_SC_SOURCEFILENAME].X1+(int)strlen(RemoveTrailingSpaces(CopyDlg[ID_SC_SOURCEFILENAME].Data));

  /* $ 15.06.2002 IS
     ��������� ����������� ������ - � ���� ������ ������ �� ������������,
     �� ���������� ��� ����� ����������������. ���� ���������� ���������
     ���������� ������ �����, �� ������� ������.
  */
  if(!Ask)
  {
    strcpy(CopyDlgValue,(char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);
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
    Dialog Dlg(CopyDlg,sizeof(CopyDlg)/sizeof(CopyDlg[0]),CopyDlgProc,(LONG_PTR)&CDP);
    Dlg.SetHelp(Link?"HardSymLink":"CopyFiles");
    Dlg.SetPosition(-1,-1,DLG_WIDTH,DLG_HEIGHT);

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

      //������ �������� ������� ��� ������� ����� ����� ������ �� �������
      Filter->UpdateCurrentTime();

      if(DlgExitCode == ID_SC_BTNCOPY)
      {
        /* $ 03.08.2001 IS
           �������� ������� �� ������� � �������� �� ������ � ����������� ��
           ��������� ����� �����������������
        */
        strcpy(CopyDlgValue,(char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);
        Opt.CMOpt.MultiCopy=CopyDlg[ID_SC_MULTITARGET].Selected;
        if(!Opt.CMOpt.MultiCopy || !strpbrk((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,",;")) // ��������� multi*
        {
           // ������ �������, ������ �������
           RemoveTrailingSpaces((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);
           Unquote((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);
           RemoveTrailingSpaces(CopyDlgValue);
           Unquote(CopyDlgValue);

           // ������� �������, ����� "������" ������ ��������������� ���
           // ����������� �� ������� ������������ � ����
           InsertQuote(CopyDlgValue);
        }
        /* IS $ */
        if(DestList.Set(CopyDlgValue) && !strpbrk(CopyDlgValue,ReservedFilenameSymbols))
        {
          // ��������� ������� ������������� �������. KM
          UseFilter=CopyDlg[ID_SC_USEFILTER].Selected;
          break;
        }
        else
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MWarning),MSG(MCopyIncorrectTargetList), MSG(MOk));
      }
      else
        break;
    }

    /* IS $ */
    if(DlgExitCode == ID_SC_BTNCANCEL || DlgExitCode < 0 || (CopyDlg[ID_SC_BTNCOPY].Flags&DIF_DISABLE))
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
  ShellCopy::Flags&=~FCOPY_COPYPARENTSECURITY;
  if(CopyDlg[ID_SC_ACCOPY].Selected)
  {
    ShellCopy::Flags|=FCOPY_COPYSECURITY;
  }
  else if(CopyDlg[ID_SC_ACINHERIT].Selected)
  {
    ShellCopy::Flags&=~(FCOPY_COPYSECURITY|FCOPY_LEAVESECURITY);
  }
  else
  {
    ShellCopy::Flags|=FCOPY_LEAVESECURITY;
  }

  if(Opt.CMOpt.UseSystemCopy)
    ShellCopy::Flags|=FCOPY_USESYSTEMCOPY;
  else
    ShellCopy::Flags&=~FCOPY_USESYSTEMCOPY;

  if(!(ShellCopy::Flags&(FCOPY_COPYSECURITY|FCOPY_LEAVESECURITY)))
    ShellCopy::Flags|=FCOPY_COPYPARENTSECURITY;

  CDP.CopySecurity=ShellCopy::Flags&FCOPY_COPYSECURITY?1:(ShellCopy::Flags&FCOPY_LEAVESECURITY?2:0);

  // � ����� ������ ��������� ���������� ����������� (�� ��� Link, �.�. ��� Link ��������� ��������� - "������!")
  if(!Link)
  {
    if(Move)
      CopySecurityMove=CDP.CopySecurity;
    else
      CopySecurityCopy=CDP.CopySecurity;
  }

  ShellCopy::Flags|=CopyDlg[ID_SC_ONLYNEWER].Selected?FCOPY_ONLYNEWERFILES:0;
  ShellCopy::Flags|=CopyDlg[ID_SC_COPYSYMLINK].Selected?FCOPY_COPYSYMLINKCONTENTS:0;

  if (DestPlugin && !strcmp((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData,InitDestDir))
  {
    ToPlugin=1;
    _LOGCOPYR(SysLog("return -> %d",__LINE__));
    return;
  }

  int WorkMove=Move;

  _LOGCOPYR(SysLog("CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData='%s'",(char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData));
  if(CheckNulOrCon((char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData))
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
    if (PluginDestPath)
      strcpy(PluginDestPath,(char *)CopyDlg[ID_SC_TARGETEDIT].Ptr.PtrData);
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

  // TODO: Posix - bugbug
  for (int I=0;CopyDlgValue[I]!=0;I++)
    if (CopyDlgValue[I]=='/')
      CopyDlgValue[I]='\\';

 // ����� �� ���������� ����� �����������?
  ShowCopyTime = Opt.CMOpt.CopyTimeRule & ((ShellCopy::Flags&FCOPY_COPYTONUL)?COPY_RULE_NUL:COPY_RULE_FILES);

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
        char *NameTmp=new char[SizeBuffer];

        // ������������������ ���������� � ����� ������ (BugZ#171)
//        CopyBufSize = COPY_BUFFER_SIZE; // �������� � 1�
        CopyBufSize = CopyBufferSize;
        ReadOnlyDelMode=ReadOnlyOvrMode=OvrMode=SkipEncMode=SkipMode=-1;

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
        {
          CopyStartTime = clock();
          WaitUserTime = OldCalcTime = 0;
        }

        CopyTitle = new ConsoleTitle(NULL);
        StaticCopyTitle=CopyTitle;

        if(CountTarget > 1)
          StaticMove=WorkMove=0;

        while(NULL!=(NamePtr=DestList.GetNext()))
        {
          CurCopiedSize=0;
          CopyTitle->Set(Move ? MSG(MCopyMovingTitle):MSG(MCopyCopyingTitle));

          strncpy(NameTmp, NamePtr,SizeBuffer-1);
          _LOGCOPYR(SysLog("NamePtr='%s', Move=%d",NamePtr,WorkMove));

          if(isalpha(NameTmp[0]) && NameTmp[1]==':' && !NameTmp[2])
            PrepareDiskPath(NameTmp,SizeBuffer,true);

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

          // Mantis#45: ���������� ������� ����������� ������ �� ����� � NTFS �� FAT � ����� ��������� ����
          {
            char FileSysName[NM],RootDir[NM*2];
            ConvertNameToFull(NameTmp,RootDir, sizeof(RootDir));
            GetPathRoot(RootDir,RootDir);
            if (GetVolumeInformation(RootDir,NULL,0,NULL,NULL,NULL,FileSysName,sizeof(FileSysName)))
              if(strcmp(FileSysName,"NTFS"))
                ShellCopy::Flags|=FCOPY_COPYSYMLINKCONTENTS;
          }
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

        delete[] NameTmp;
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
    LenRenamedName=(int)strlen(RenamedName);
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

LONG_PTR WINAPI ShellCopy::CopyDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2)
{
#define DM_CALLTREE (DM_USER+1)
  struct CopyDlgParam *DlgParam;
  DlgParam=(struct CopyDlgParam *)Dialog::SendDlgMessage(hDlg,DM_GETDLGDATA,0,0);

  switch(Msg)
  {
    case DN_BTNCLICK:
    {
      if (Param1==ID_SC_USEFILTER) // "Use filter"
      {
        UseFilter=(int)Param2;
        return TRUE;
      }
      if(Param1 == ID_SC_BTNTREE) // Tree
      {
        Dialog::SendDlgMessage(hDlg,DM_CALLTREE,0,0);
        return FALSE;
      }
      else if(Param1 == ID_SC_BTNCOPY)
      {
        Dialog::SendDlgMessage(hDlg,DM_CLOSE,ID_SC_BTNCOPY,0);
      }
      else if(Param1 == ID_SC_ONLYNEWER && ((DlgParam->thisClass->Flags)&FCOPY_LINK))
      {
        // ����������� ��� ����� �������� ������������ � ������ ����� :-))
        struct FarDialogItem DItemTargetEdit;
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_SC_TARGETEDIT,(LONG_PTR)&DItemTargetEdit);
        Dialog::SendDlgMessage(hDlg,DN_EDITCHANGE,ID_SC_TARGETEDIT,(LONG_PTR)&DItemTargetEdit);
      }
      else if (Param1==ID_SC_BTNFILTER) // Filter
      {
        Filter->FilterEdit();
        return TRUE;
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
        struct FarDialogItem DItemACCopy,DItemACInherit,DItemACLeave,DItemOnlyNewer,DItemBtnCopy;
        int LenCurDirName=DlgParam->thisClass->SrcPanel->GetCurDir(NULL);
        char *SrcDir=(char *)alloca(LenCurDirName+16);
        DlgParam->thisClass->SrcPanel->GetCurDir(SrcDir);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_SC_ACCOPY,(LONG_PTR)&DItemACCopy);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_SC_ACINHERIT,(LONG_PTR)&DItemACInherit);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_SC_ACLEAVE,(LONG_PTR)&DItemACLeave);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_SC_ONLYNEWER,(LONG_PTR)&DItemOnlyNewer);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_SC_BTNCOPY,(LONG_PTR)&DItemBtnCopy);

        // ��� �������� �����?
        if((DlgParam->thisClass->Flags)&FCOPY_LINK)
        {
          DlgParam->thisClass->LinkRules(&DItemBtnCopy.Flags,
                    &DItemOnlyNewer.Flags,
                    &DItemOnlyNewer.Param.Selected,
                    SrcDir,
                    ((struct FarDialogItem *)Param2)->Data.Ptr.PtrData,DlgParam);
        }
        else // ������� Copy/Move
        {
          struct FarDialogItem *DItemTargetEdit=(struct FarDialogItem *)Param2;
          char *Buf=(char*)alloca(DItemTargetEdit->Data.Ptr.PtrLength);
          strupr(strcpy(Buf,DItemTargetEdit->Data.Ptr.PtrData));
          if(*DlgParam->PluginFormat && strstr(Buf,DlgParam->PluginFormat))
          {
            DItemACCopy.Flags|=DIF_DISABLE;
            DItemACInherit.Flags|=DIF_DISABLE;
            DItemACLeave.Flags|=DIF_DISABLE;
            DItemOnlyNewer.Flags|=DIF_DISABLE;
            DlgParam->OnlyNewerFiles=DItemOnlyNewer.Param.Selected;
            DlgParam->CopySecurity=0;
            if (DItemACCopy.Param.Selected)
              DlgParam->CopySecurity=1;
            else if (DItemACLeave.Param.Selected)
              DlgParam->CopySecurity=2;
            DItemACCopy.Param.Selected=0;
            DItemACInherit.Param.Selected=0;
            DItemACLeave.Param.Selected=1;
            DItemOnlyNewer.Param.Selected=0;
          }
          else
          {
            DItemACCopy.Flags&=~DIF_DISABLE;
            DItemACInherit.Flags&=~DIF_DISABLE;
            DItemACLeave.Flags&=~DIF_DISABLE;
            DItemOnlyNewer.Flags&=~DIF_DISABLE;
            DItemOnlyNewer.Param.Selected=DlgParam->OnlyNewerFiles;
            DItemACCopy.Param.Selected=0;
            DItemACInherit.Param.Selected=0;
            DItemACLeave.Param.Selected=0;
            if (DlgParam->CopySecurity == 1)
            {
              DItemACCopy.Param.Selected=1;
            }
            else if (DlgParam->CopySecurity == 2)
            {
              DItemACLeave.Param.Selected=1;
            }
            else
              DItemACInherit.Param.Selected=1;
          }
        }

        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_SC_ACCOPY,(LONG_PTR)&DItemACCopy);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_SC_ACINHERIT,(LONG_PTR)&DItemACInherit);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_SC_ACLEAVE,(LONG_PTR)&DItemACLeave);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_SC_ONLYNEWER,(LONG_PTR)&DItemOnlyNewer);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_SC_BTNCOPY,(LONG_PTR)&DItemBtnCopy);
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
      BOOL MultiCopy=Dialog::SendDlgMessage(hDlg,DM_GETCHECK,ID_SC_MULTITARGET,0)==BSTATE_CHECKED;
      struct FarDialogItem DItemTargetEdit;
      Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_SC_TARGETEDIT,(LONG_PTR)&DItemTargetEdit);

      char *NewFolder=(char*)alloca(DItemTargetEdit.Data.Ptr.PtrLength);
      char *OldFolder=(char*)DItemTargetEdit.Data.Ptr.PtrData;

      *NewFolder = 0;
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
              xstrncpy(NewFolder, NamePtr, DItemTargetEdit.Data.Ptr.PtrLength-1);
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
               FALSE,FALSE);
        }
        if (*NewFolder)
        {
          AddEndSlash(NewFolder);
          if(MultiCopy) // �����������������
          {
            // ������� �������, ���� ��� �������� �������� �������-�����������
            if(strpbrk(NewFolder,";,"))
              InsertQuote(NewFolder);

            int len=(int)strlen(OldFolder), newlen=(int)strlen(NewFolder), addSep=0;
            addSep=len>0;
            if(len+newlen+addSep < DItemTargetEdit.Data.Ptr.PtrLength)// ������������ ����� ������
            {
              if(addSep)
                OldFolder[len++]=';'; // ������� ����������� � ��������� ������
              strcpy(OldFolder+len, NewFolder);
            }
            strcpy(NewFolder, OldFolder);
          }
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_SC_TARGETEDIT,(LONG_PTR)NewFolder);
          Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,ID_SC_TARGETEDIT,0);
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
    xstrncpy(Root,SrcDir,sizeof(Root));
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

    xstrncpy(Root,DstDir,sizeof(Root));
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
  _tran(SysLog("[%p] ShellCopy::~ShellCopy(), CopyBufer=%p",this,CopyBuffer));
  if ( CopyBuffer )
    delete[] CopyBuffer;

  // $ 26.05.2001 OT ��������� ����������� �������
  _tran(SysLog("call (*FrameManager)[0]->UnlockRefresh()"));
  (*FrameManager)[0]->Unlock();
  (*FrameManager)[0]->Refresh();

  if(sddata)
    delete[] sddata;

  if(Filter) // ��������� ������ �������
    delete Filter;
}


COPY_CODES ShellCopy::CopyFileTree(char *Dest)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::CopyFileTree()"));
  _LOGCOPYR(SysLog("Params: Dest='%s'",Dest));
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  //SaveScreen SaveScr;
  DWORD DestAttr=(DWORD)-1;
  _tran(SysLog("[%p] ShellCopy::CopyFileTree()",this));

  char SelName[NM],SelShortName[NM];
  int Length,FileAttr;

  if ((Length=(int)strlen(Dest))==0 || strcmp(Dest,".")==0)
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

    if (*SrcDriveRoot==0 || LocalStrnicmp(SelName,SrcDriveRoot,(int)strlen(SrcDriveRoot))!=0)
    {
      GetPathRoot(SelName,SrcDriveRoot);
      SrcDriveType=FAR_GetDriveType(strchr(SelName,'\\')!=NULL ? SrcDriveRoot:NULL);
      if(GetFileAttributes(SrcDriveRoot) != -1)
        if(!GetVolumeInformation(SrcDriveRoot,NULL,0,NULL,NULL,&SrcFSFlags,SrcFSName,sizeof(SrcFSName)))
          *SrcFSName=0;
      _LOGCOPYR(SysLog("SrcDriveRoot='%s', SrcDriveType=%d, SrcFSFlags=0x%08X, SrcFSName='%s'",SrcDriveRoot,SrcDriveType,SrcFSFlags,SrcFSName));
    }

    if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
      SelectedFolderNameLength=(int)strlen(SelName);
    else
      SelectedFolderNameLength=0;

    // "�������" � ������ ���� ������� - �������� ������ �������, ���������� �� �����
    if(DestDriveType == DRIVE_REMOTE || SrcDriveType == DRIVE_REMOTE)
      ShellCopy::Flags|=FCOPY_COPYSYMLINKCONTENTS;

    KeepPathPos=(int)(PointToName(SelName)-SelName);
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
        strcat(DestPath,SelName);
        ShellCopy::ShellCopyMsg(SelName,DestPath,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);
        if (Message(MSG_DOWN|MSG_WARNING,2,MSG(MError),MSG(MCopyCannotFind),
                SelName,MSG(MSkip),MSG(MCancel))==1)
        {
          _LOGCOPYR(SysLog("return COPY_FAILURE -> MCopyCannotFind"));
          return COPY_FAILURE;
        }
        /* $ 23.03.2002 VVM
          ! ������ ���, �.�. ��������� SrcData ���������� */
  //      unsigned __int64 SubSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
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
          unsigned __int64 CurSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
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
        unsigned __int64 CurSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
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

    // Mantis#44 - ������ ������ ��� ����������� ������ �� �����
    // ���� ������� (��� ����� ���������� �������) - �������� ���������� ����������...
    if ((SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
        (
          !(SrcData.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) ||
          (SrcData.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) && (ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS)
        )
       )
    {
      _LOGCOPYR(CleverSysLog Clev("if(SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)"));

      // �������, �� �������� � ���������� ������: ���� ����������� ������ �������
      // �������� ��� ���������� ������� ��� ���� ����������� �������� � ����������.
      int TryToCopyTree=FALSE,FilesInDir=0;

      int SubCopyCode;
      char SubName[NM],FullName[NM];
      ScanTree ScTree(TRUE,TRUE,ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS);

      strcpy(SubName,SelName);
      strcat(SubName,"\\");
      if (DestAttr==(DWORD)-1)
        KeepPathPos=(int)strlen(SubName);

      int NeedRename=!((SrcData.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) && (ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS) && (ShellCopy::Flags&FCOPY_MOVE));
      int AttemptToMove=FALSE;
      int SaveOvrMode=OvrMode;

      ScTree.SetFindPath(SubName,"*.*",FSCANTREE_FILESFIRST);

      while (ScTree.GetNextName(&SrcData,FullName, sizeof (FullName)-1))
      {
        _LOGCOPYR(SysLog("FullName='%s', SameDisk=%d",FullName,SameDisk));

        // ���� ������� ����������� ������� � ���������� ��� ���������� �������
        TryToCopyTree=TRUE;

        /* 23.04.2005 KM
           �������� � ������� �������� �� ��������, ��� ���������� ���, � ����
           ��-�� �������� ������� �� ����������� � ���������, ��� � ��� ���������
           ������ � ������ �� ������ �������. � ��������� �������� ����� � ShellCopyOneFile,
           ���������� ���� �� ����� ��������� � ������ �����.
        */
        if (UseFilter && (SrcData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
          // ������ ���������� ������� ������������ - ���� ������� ������� �
          // ������� ��� ������������, �� ������� ���������� � ��� � �� ���
          // ����������.
          if (!Filter->FileInFilter((WIN32_FIND_DATA *) &SrcData))
          {
            ScTree.SkipDir();
            continue;
          }
          else
          {
            // ��-�� ���� ��������� ��� Move ��������� ������������� ������� ��
            // ��������� ������� �������, � ������ �������� �� ���������� ����
            // � ������ ����� ������ ��� ���������
            if (!(ShellCopy::Flags&FCOPY_MOVE) || SameDisk || !ScTree.IsDirSearchDone())
              continue;
            if(FilesInDir) goto remove_moved_directory;
          }
        }
        /* KM $ */

        AttemptToMove=FALSE;
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
              unsigned __int64 CurSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
              TotalCopiedSize = TotalCopiedSize - CurCopiedSize + CurSize;
              TotalSkippedSize = TotalSkippedSize + CurSize - CurCopiedSize;
              continue;
            }

            case COPY_SUCCESS_MOVE:
            {
              FilesInDir++;
              continue;
            }

            case COPY_SUCCESS:
              if(!NeedRename) // ������� ��� ����������� ����������� ������� � ������ "���������� ���������� ���..."
              {
                unsigned __int64 CurSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
                TotalCopiedSize = TotalCopiedSize - CurCopiedSize + CurSize;
                TotalSkippedSize = TotalSkippedSize + CurSize - CurCopiedSize;
                FilesInDir++;
                continue;     // ...  �.�. �� ��� �� ������, � �����������, �� ���, �� ���� �������� �������� � ���� ������
              }
          }
        }

        SaveOvrMode=OvrMode;

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
          unsigned __int64 CurSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
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
remove_moved_directory:
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

          FilesInDir++;
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

      /* $ 23.04.2005 KM
         ���� ��������� ������� ����������� �������� � ���������� ���
         ���������� �������, �� �� ���� ����������� �� ������ �����,
         ������� ������ SelName � ��������-��������.
      */
      if (UseFilter && TryToCopyTree && !FilesInDir)
      {
        strcat(DestPath,SelName);
        FAR_RemoveDirectory(DestPath);
      }
      /* KM $ */
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
  DWORD DestAttr=(DWORD)-1;
  HANDLE FindHandle=INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA DestData={0};
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

  int IsSetSecuty=FALSE;

  if (CheckForEscSilent())
  {
    if (ConfirmAbortOp())
    {
      _LOGCOPYR(SysLog("return COPY_CANCEL -> %d Pressed ESC",__LINE__));
      return(COPY_CANCEL);
    }
  }

  if (UseFilter)
  {
    if (!Filter->FileInFilter((WIN32_FIND_DATA *) &SrcData))
      return COPY_NEXT;
  }

  strcpy(DestPath,Dest);

  SetPreRedrawFunc(ShellCopy::PR_ShellCopyMsg);
  ShellCopyConvertWildcards(Src,DestPath);

  char *NamePtr=PointToName(DestPath);

  DestAttr=(DWORD)-1;

  if (DestPath[0]=='\\' && DestPath[1]=='\\')
  {
    char Root[NM];
    GetPathRoot(DestPath,Root);
    int RootLength=(int)strlen(Root);

    if (RootLength>0 && Root[RootLength-1]=='\\')
      Root[RootLength-1]=0;

    if (strcmp(DestPath,Root)==0)
      DestAttr=FILE_ATTRIBUTE_DIRECTORY;
  }

  _LOGCOPYR(SysLog("NamePtr='%s', TestParentFolderName()=>%d",NamePtr,TestParentFolderName(NamePtr)));
  if (*NamePtr==0 || TestParentFolderName(NamePtr))
    DestAttr=FILE_ATTRIBUTE_DIRECTORY;

  *DestData.cFileName=0;

  if (DestAttr==(DWORD)-1)
  {
    GetFileWin32FindData(DestPath,&DestData);
    DestAttr=DestData.dwFileAttributes;
  }
  //if (DestAttr==(DWORD)-1 && (FindHandle=FindFirstFile(DestPath,&DestData))!=INVALID_HANDLE_VALUE)
  //{
  //  FindClose(FindHandle);
  //  DestAttr=DestData.dwFileAttributes;
  //}
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
        SetMessageHelp("ErrCopyItSelf");
        Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),MSG(MCannotCopyFolderToItself1),
                Src,MSG(MCannotCopyFolderToItself2),MSG(MOk));
        _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
        return(COPY_CANCEL);
      }
    }
    _LOGCOPYR(else SysLog("%d CmpFullNames() == 0",__LINE__));

    if (!SameName)
    {
      int Length=(int)strlen(DestPath);

      if (DestPath[Length-1]!='\\' && DestPath[Length-1]!=':')
        strcat(DestPath,"\\");

      const char *PathPtr=Src+KeepPathPos;

      if (*PathPtr && KeepPathPos==0 && PathPtr[1]==':')
        PathPtr+=2;

      if (*PathPtr=='\\')
        PathPtr++;

      /* $ 23.04.2005 KM
          ��������� �������� � ������� ���������� ���������� ����������� ���������,
          ����� �� ������� ������ �������� ��-�� ���������� � ������ ������,
          �� �������� �������� ����� ����������� ��� ����������� �����, ���������
          � ������, � ��� ����� ������ �������� ����������� �������� � ���������
          �� �� ����������� �������.
      */
      if (UseFilter)
      {
        char OldPath[2*NM],NewPath[2*NM];
        const char *path=PathPtr,*p1=NULL;

        while (p1=strchr(path,'\\'))
        {
          DWORD FileAttr=(DWORD)-1;
          WIN32_FIND_DATA FileData={0};

          xstrncpy(OldPath,Src,p1-Src);

          GetFileWin32FindData(OldPath,&FileData);
          FileAttr=FileData.dwFileAttributes;
          #if 0
          // ������ ������ ������� � ������ ��� ��������
          if ((FindHandle=FindFirstFile(OldPath,&FileData))==INVALID_HANDLE_VALUE)
            // ������ ��� �� ���������, �� ����� �������, ������� ��� �����������
            // (���� ��� �� �������� � �� ������ �����������)
            FileAttr=(DWORD)-1;
          else
          {
            FindClose(FindHandle);
            // ���� ����, � ����� ��� ��������
            FileAttr=FileData.dwFileAttributes;
          }
          #endif

          // �������� ��� ��������, ������� ������ ����� �������, ���� ������� ��� ��� ���
          xstrncpy(NewPath,DestPath,sizeof(NewPath)-1);
          xstrncpy(NewPath+strlen(DestPath),PathPtr,p1-PathPtr);

          // ������ �������� ��� ���, �������� ���
          if ((FindHandle=FindFirstFile(NewPath,&FileData))==INVALID_HANDLE_VALUE)
          {
            int CopySecurity = ShellCopy::Flags&FCOPY_COPYSECURITY;
            SECURITY_ATTRIBUTES sa;

            if ((CopySecurity) && !GetSecurity(OldPath,sa))
              CopySecurity = FALSE;

            // ���������� �������� ��������
            if (CreateDirectory(NewPath,CopySecurity?&sa:NULL))
            {
              // ���������, ������� �������
              if (FileAttr!=(DWORD)-1)
                // ������ ��������� ��������. ����������������� �������� �� ���������
                // ��� ��� �������� "�����", �� ���� �������� � ��������� ��������
                ShellSetAttr(NewPath,FileAttr);
            }
            else
            {
              // ��-��-��. ������� �� ������ �������! ������ ����� ��������!
              int MsgCode;
              MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                              MSG(MCopyCannotCreateFolder),NewPath,MSG(MCopyRetry),
                              MSG(MCopySkip),MSG(MCopyCancel));

              if (MsgCode!=0)
              {
                // �� ��� �, ��� ���� ������ ��� ������� �������� ��������, ������� ������
                _LOGCOPYR(SysLog("return %d -> %d",((MsgCode==-2 || MsgCode==2) ? COPY_CANCEL:COPY_NEXT),__LINE__));
                return((MsgCode==-2 || MsgCode==2) ? COPY_CANCEL:COPY_NEXT);
              }

              // ������� ����� ���������� ������� �������� ��������. �� ����� � ������ ����
              continue;
            }
          }
          else
            // �����. ������� ��� ������� - ���������. ������ ���������� ����������� ������.
            FindClose(FindHandle);

          // �� ����� �� �������� �����
          if (*p1=='\\')
            p1++;

          // ������ ��������� ����� � ����� �������� �� �������� ������,
          // ��� ���� ����� ��������� �, ��������, ������� ��������� �������,
          // ����������� � ���������� ����� �����.
          path=p1;
        }
      }
      /* KM $ */

      strcat(DestPath,PathPtr);

      if ((FindHandle=FindFirstFile(DestPath,&DestData))==INVALID_HANDLE_VALUE)
        DestAttr=(DWORD)-1;
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
        _LOGCOPYR(SysLog("%d Rename",__LINE__));
        char SrcFullName[NM*2],DestFullName[NM*2];

        if (ConvertNameToFull(Src,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
        {
          _LOGCOPYR(SysLog("return COPY_NEXT -> %d ConvertNameToFull()-but",__LINE__));
          return(COPY_NEXT);
        }

        SECURITY_ATTRIBUTES sa;
        _LOGCOPYR(ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)));
        _LOGCOPYR(SysLog("%d call GetSecurity ('%s')",__LINE__,Dest));
        _LOGCOPYR(SysLog("%d DestFullName='%s'",__LINE__,DestFullName));
        _LOGCOPYR(SysLog("%d SrcFullName ='%s'",__LINE__,SrcFullName));
        //_LOGCOPYR(SysLog("%d CmpFullPath => %d",__LINE__,CmpFullPath(SrcFullName,DestFullName)));
        _LOGCOPYR(SysLog("%d CmpFullPath => %d",__LINE__,CmpFullPath(Src,Dest)));

        // ��� Move ��� ���������� ������ ������� ��������, ����� �������� ��� ���������
        if (!(ShellCopy::Flags&(FCOPY_COPYSECURITY|FCOPY_LEAVESECURITY)))
        {
          IsSetSecuty=FALSE;
          if(CmpFullPath(Src,Dest)) // � �������� ������ �������� ������ �� ������
            IsSetSecuty=FALSE;
          else if(GetFileAttributes(Dest) == (DWORD)-1) // ���� �������� ���...
          {
            // ...�������� ��������� ��������
            if(GetSecurity(GetParentFolder(Dest,DestFullName,sizeof(DestFullName)-1),sa))
              IsSetSecuty=TRUE;
          }
          else if(GetSecurity(Dest,sa)) // ����� �������� ��������� Dest`�
            IsSetSecuty=TRUE;
        }

        /* $ 18.07.2001 VVM
          + �������� �������������, ���� �� ������� */
        while (1)
        {
          // $ 25.05.2002 IS ��������� ��������� RenameToShortName ��� ��������.
          _LOGCOPYR(SysLog("%d Move '%s' => '%s'",__LINE__,Src,DestPath));
          BOOL SuccessMove=RenameToShortName?MoveFileThroughTemp(Src,DestPath):FAR_MoveFile(Src,DestPath);
          _LOGCOPYR(SysLog("%d SuccessMove=%d",__LINE__,SuccessMove));

          if (SuccessMove)
          {
            if(IsSetSecuty)// && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT && !strcmp(DestFSName,"NTFS"))
              //if(!(ShellCopy::Flags&FCOPY_CURRENTONLY))
                SetRecursiveSecurity(DestPath,sa);

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
            switch (Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                            MSG(MCopyCannotRenameFolder),Src,MSG(MCopyRetry),
                            MSG(MCopyIgnore),MSG(MCopyCancel)))
            {
              case 0:  continue;
              case 1:
              {
                // ���� ������� "ignore" - ���������� ������� ������ � ��������� ���� ����������
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
//                  if (ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
//                    return(COPY_NEXT);
//                  TreeList::RenTreeName(SrcFullName,DestFullName);
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
              }
              default:
                _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
                return (COPY_CANCEL);
            } /* switch */
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
        MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                        MSG(MCopyCannotCreateFolder),DestPath,MSG(MCopyRetry),
                        MSG(MCopySkip),MSG(MCopyCancel));

        if (MsgCode!=0)
        {
          _LOGCOPYR(SysLog("return %d -> %d",((MsgCode==-2 || MsgCode==2) ? COPY_CANCEL:COPY_NEXT),__LINE__));
          return((MsgCode==-2 || MsgCode==2) ? COPY_CANCEL:COPY_NEXT);
        }
      }

      DWORD SetAttr=SrcData.dwFileAttributes;

      if (IsDriveTypeCDROM(SrcDriveType) && Opt.ClearReadOnly && (SetAttr & FA_RDONLY))
        SetAttr&=~FA_RDONLY;

      if((SetAttr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
      {
        // �� ����� ���������� ����������, ���� ������� � �������
        // � ������������ FILE_ATTRIBUTE_ENCRYPTED (� �� ��� ����� ��������� ����� CreateDirectory)
        // �.�. ���������� ������ ���.
        if(GetFileAttributes(DestPath)&FILE_ATTRIBUTE_ENCRYPTED)
          SetAttr&=~FILE_ATTRIBUTE_COMPRESSED;

        if(SetAttr&FILE_ATTRIBUTE_COMPRESSED)
        {
          while(1)
          {
            int MsgCode=ESetFileCompression(DestPath,1,0,SkipMode);
            if(MsgCode)
            {
              if(MsgCode == SETATTR_RET_SKIP)
                ShellCopy::Flags|=FCOPY_SKIPSETATTRFLD;
              else if(MsgCode == SETATTR_RET_SKIPALL)
              {
                ShellCopy::Flags|=FCOPY_SKIPSETATTRFLD;
                this->SkipMode=SETATTR_RET_SKIP;
              }
              break;
            }
            if(MsgCode != SETATTR_RET_OK)
              return (MsgCode==SETATTR_RET_SKIP || MsgCode==SETATTR_RET_SKIPALL) ? COPY_NEXT:COPY_CANCEL;
          }
        }
#if 0
        else if(SetAttr&FILE_ATTRIBUTE_ENCRYPTED)
        {
          while(1)
          {
            int MsgCode=ESetFileEncryption(DestPath,1,0);
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
          MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,4,MSG(MError),
                          MSG(MCopyCannotChangeFolderAttr),DestPath,
                          MSG(MCopyRetry),MSG(MCopySkip),MSG(MCopySkipAll),MSG(MCopyCancel));

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
      else if( !(ShellCopy::Flags & FCOPY_SKIPSETATTRFLD) && ((SetAttr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY))
      {
        while(!ShellSetAttr(DestPath,SetAttr))
        {
          int MsgCode;
          MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,4,MSG(MError),
                          MSG(MCopyCannotChangeFolderAttr),DestPath,
                          MSG(MCopyRetry),MSG(MCopySkip),MSG(MCopySkipAll),MSG(MCopyCancel));

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
              Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),MSG(MCannotCopyFileToItself1),
                      Src,MSG(MCannotCopyFileToItself2),MSG(MOk));
              _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
              return(COPY_CANCEL);
            }
          }
        }

        int RetCode;
        if (!AskOverwrite(SrcData,DestPath,DestAttr,SameName,Rename,((ShellCopy::Flags&FCOPY_LINK)?0:1),Append,RetCode))
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
    int CopyCode=0;
//    unsigned __int64 SaveCopiedSize=CurCopiedSize;
    unsigned __int64 SaveTotalSize=TotalCopiedSize;
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

        SECURITY_ATTRIBUTES sa;
        IsSetSecuty=FALSE;

        // ��� Move ��� ���������� ������ ������� ��������, ����� �������� ��� ���������
        if (Rename && !(ShellCopy::Flags&(FCOPY_COPYSECURITY|FCOPY_LEAVESECURITY)))
        {
          if(CmpFullPath(Src,Dest)) // � �������� ������ �������� ������ �� ������
            IsSetSecuty=FALSE;
          else if(GetFileAttributes(Dest) == (DWORD)-1) // ���� �������� ���...
          {
            char DestFullName[NM*2];
            // ...�������� ��������� ��������
            if(GetSecurity(GetParentFolder(Dest,DestFullName,sizeof(DestFullName)-1),sa))
              IsSetSecuty=TRUE;
          }
          else if(GetSecurity(Dest,sa)) // ����� �������� ��������� Dest`�
            IsSetSecuty=TRUE;
        }


        /* $ 25.05.2002 IS
           ��������� ��������� RenameToShortName ��� ��������.
        */
        if(RenameToShortName)
          MoveCode=MoveFileThroughTemp(SrcFullName, DestPath);
        else
        {
          if (WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT || !strcmp(DestFSName,"NWFS"))
            MoveCode=FAR_MoveFile(SrcFullName,DestPath);
          else
            MoveCode=FAR_MoveFileEx(SrcFullName,DestPath,SameName ? MOVEFILE_COPY_ALLOWED:MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
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
        else
        {
          if (IsSetSecuty)
            SetSecurity(DestPath,sa);
        }

        if (NWFS_Attr)
          SetFileAttributes(DestPath,SrcData.dwFileAttributes);

        if (ShowTotalCopySize && MoveCode)
        {
          unsigned __int64 AddSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
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
          ShellSetAttr(DestPath,SrcData.dwFileAttributes & (~FA_RDONLY));

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
            FAR_MoveFile(DestPath,DestPath);
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
    InsertQuote(strcpy(Msg1,Src));
    InsertQuote(strcpy(Msg2,DestPath));
    {
      int MsgCode;
      if((SrcData.dwFileAttributes&FILE_ATTRIBUTE_ENCRYPTED))
      {
//        if (SkipMode!=-1)
//          MsgCode=SkipMode;
        if (SkipEncMode!=-1)// && SkipMode!=-1)
        {
          MsgCode=SkipEncMode;
          if(SkipEncMode == 1)
            ShellCopy::Flags|=FCOPY_DECRYPTED_DESTINATION;
        }
        else
        {
          if(_localLastError == 5)
          {
            #define ERROR_EFS_SERVER_NOT_TRUSTED     6011L
            ;//SetLastError(_localLastError=(DWORD)0x80090345L);//SEC_E_DELEGATION_REQUIRED);
            SetLastError(_localLastError=ERROR_EFS_SERVER_NOT_TRUSTED);
          }
          MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,5,MSG(MError),
                          MSG(MsgMCannot),
                          Msg1,
                          MSG(MCannotCopyTo),
                          Msg2,
                          MSG(MCopyDecrypt),
                          MSG(MCopyDecryptAll),
                          MSG(MCopySkip),
                          MSG(MCopySkipAll),
                          MSG(MCopyCancel));

          switch(MsgCode)
          {
            case  0:
              _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
              ShellCopy::Flags|=FCOPY_DECRYPTED_DESTINATION;
              break;//return COPY_NEXT;
            case  1:
              SkipEncMode=1;
              ShellCopy::Flags|=FCOPY_DECRYPTED_DESTINATION;
              _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
              break;//return COPY_NEXT;
            case  2:
              _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
              return COPY_NEXT;
            case  3:
              SkipMode=1;
              _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
              return COPY_NEXT;
            case -1:
            case -2:
            case  4:
              _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
              return COPY_CANCEL;
          }
        }
      }
      else
      {
        if (SkipMode!=-1)
          MsgCode=SkipMode;
        else
        {
          MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,4,MSG(MError),
                          MSG(MsgMCannot),
                          Msg1,
                          MSG(MCannotCopyTo),
                          Msg2,
                          MSG(MCopyRetry),
                          MSG(MCopySkip),
                          MSG(MCopySkipAll),
                          MSG(MCopyCancel));
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
      }
    }

//    CurCopiedSize=SaveCopiedSize;
    TotalCopiedSize=SaveTotalSize;
    int RetCode;
    if (!AskOverwrite(SrcData,DestPath,DestAttr,SameName,Rename,((ShellCopy::Flags&FCOPY_LINK)?0:1),Append,RetCode))
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
  if(!(ShellCopy::Flags&FCOPY_USESYSTEMCOPY) && NT && AscStreams)
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
  #define BAR_SIZE  46
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
    int TotalLength=(int)strlen(TotalMsg);
    memcpy(BarStr+(strlen(BarStr)-TotalLength+1)/2,TotalMsg,TotalLength);
//    *FilesStr=0;

    sprintf (FilesStr, MSG(MCopyProcessedTotal),TotalFiles, TotalFilesToProcess);
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
      LastShowTime = 0;
      WaitUserTime = OldCalcTime = 0;
    }
    /* VVM $ */
  }

  if (Src!=NULL)
  {
    sprintf(SrcName,"%-46s",Src);
    TruncPathStr(SrcName,46);
  }
  sprintf(DestName,"%-46s",Dest);
  TruncPathStr(DestName,46);

  SetMessageHelp("CopyFiles");
  if (Src==NULL)
    Message(Flags,0,(ShellCopy::Flags&FCOPY_MOVE) ? MSG(MMoveDlgTitle):
                       MSG(MCopyDlgTitle),
                       "",MSG(MCopyScanning),
                       DestName,"","",BarStr,"");
  else
  {
    int Move = ShellCopy::Flags&FCOPY_MOVE;

    if ( ShowTotalCopySize )
    {
      if ( ShowCopyTime )
        Message(Flags, 0, MSG(Move?MMoveDlgTitle:MCopyDlgTitle),MSG(Move?MCopyMoving:MCopyCopying),SrcName,MSG(MCopyTo),DestName,"",BarStr,"",Bar,FilesStr,Bar,"");
      else
        Message(Flags, 0, MSG(Move?MMoveDlgTitle:MCopyDlgTitle),MSG(Move?MCopyMoving:MCopyCopying),SrcName,MSG(MCopyTo),DestName,"",BarStr,"",Bar,FilesStr);
    }
    else
    {
      if ( ShowCopyTime )
        Message(Flags, 0, MSG(Move?MMoveDlgTitle:MCopyDlgTitle),MSG(Move?MCopyMoving:MCopyCopying),SrcName,MSG(MCopyTo),DestName,"",BarStr,FilesStr,Bar,"");
      else
        Message(Flags, 0, MSG(Move?MMoveDlgTitle:MCopyDlgTitle),MSG(Move?MCopyMoving:MCopyCopying),SrcName,MSG(MCopyTo),DestName,"",BarStr,FilesStr);
    }
  }

  int MessageX1,MessageY1,MessageX2,MessageY2;
  GetMessagePosition(MessageX1,MessageY1,MessageX2,MessageY2);
  BarX=MessageX1+5;
  BarY=MessageY1+6;
  BarLength=MessageX2-MessageX1-9-5; //-5 ��� ���������

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
  xstrncpy(Src,SrcName,sizeof(Src)-1);
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

  int BeforeNameLength=DestNamePtr==Dest ? (int)(SrcNamePtr-Src):0;
  xstrncpy(PartBeforeName,Src,BeforeNameLength);
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
    if (ReadOnlyDelMode!=-1)
      MsgCode=ReadOnlyDelMode;
    else
      MsgCode=Message(MSG_DOWN|MSG_WARNING,5,MSG(MWarning),
              MSG(MCopyFileRO),Name,MSG(MCopyAskDelete),
              MSG(MCopyDeleteRO),MSG(MCopyDeleteAllRO),
              MSG(MCopySkipRO),MSG(MCopySkipAllRO),MSG(MCopyCancelRO));
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

  while((Attr&FA_DIREC)?!FAR_RemoveDirectory(Name):remove(Name)!=0)
  {
    int MsgCode;
    MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                    MSG(MCannotDeleteFile),Name,MSG(MDeleteRetry),
                    MSG(MDeleteSkip),MSG(MDeleteCancel));
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

  if((SrcData.dwFileAttributes&FILE_ATTRIBUTE_ENCRYPTED) &&
     !CheckDisksProps(SrcName,DestName,CHECKEDPROPS_ISDST_ENCRYPTION)
    )
  {
    int MsgCode;
    if (SkipEncMode!=-1)
    {
      MsgCode=SkipEncMode;
      if(SkipEncMode == 1)
        ShellCopy::Flags|=FCOPY_DECRYPTED_DESTINATION;
    }
    else
    {
      char Msg1[2*NM];
      SetMessageHelp("WarnCopyEncrypt");
      InsertQuote(xstrncpy(Msg1,SrcName,sizeof(Msg1)-1));
      MsgCode=Message(MSG_DOWN|MSG_WARNING,3,MSG(MWarning),
                      MSG(MCopyEncryptWarn1),
                      Msg1,
                      MSG(MCopyEncryptWarn2),
                      MSG(MCopyEncryptWarn3),
                      MSG(MCopyIgnore),MSG(MCopyIgnoreAll),MSG(MCopyCancel));
    }

    switch(MsgCode)
    {
      case  0:
        _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
        ShellCopy::Flags|=FCOPY_DECRYPTED_DESTINATION;
        break;//return COPY_NEXT;
      case  1:
        SkipEncMode=1;
        ShellCopy::Flags|=FCOPY_DECRYPTED_DESTINATION;
        _LOGCOPYR(SysLog("return COPY_NEXT -> %d",__LINE__));
        break;//return COPY_NEXT;
      case -1:
      case -2:
      case  2:
        _LOGCOPYR(SysLog("return COPY_CANCEL -> %d",__LINE__));
        return COPY_CANCEL;
    }
  }

  if (!(ShellCopy::Flags&FCOPY_COPYTONUL) && (ShellCopy::Flags&FCOPY_USESYSTEMCOPY) && !Append)
  {
    //if(!(WinVer.dwMajorVersion >= 5 && WinVer.dwMinorVersion > 0) && (ShellCopy::Flags&FCOPY_DECRYPTED_DESTINATION))
    if(!(SrcData.dwFileAttributes&FILE_ATTRIBUTE_ENCRYPTED) ||
        (SrcData.dwFileAttributes&FILE_ATTRIBUTE_ENCRYPTED) &&
          (WinVer.dwMajorVersion >= 5 && WinVer.dwMinorVersion > 0 ||
          !(ShellCopy::Flags&(FCOPY_DECRYPTED_DESTINATION)))
      )
    {
      if (!Opt.CMOpt.CopyOpened)
      {
        HANDLE SrcHandle=FAR_CreateFile (
            SrcName,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_SEQUENTIAL_SCAN,
            NULL
            );

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
  }

  SECURITY_ATTRIBUTES sa;
  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(SrcName,sa))
  {
    _LOGCOPYR(SysLog("return COPY_CANCEL -> %d GetSecurity() == 0",__LINE__));
    return COPY_CANCEL;
  }
  int OpenMode=FILE_SHARE_READ;
  if (Opt.CMOpt.CopyOpened)
    OpenMode|=FILE_SHARE_WRITE;
  HANDLE SrcHandle= FAR_CreateFile (
      SrcName,
      GENERIC_READ,
      OpenMode,
      NULL,
      OPEN_EXISTING,
      FILE_FLAG_SEQUENTIAL_SCAN,
      NULL
      );
  if (SrcHandle==INVALID_HANDLE_VALUE && Opt.CMOpt.CopyOpened)
  {
    _localLastError=GetLastError();
    SetLastError(_localLastError);
    if ( _localLastError == ERROR_SHARING_VIOLATION )
    {
      SrcHandle = FAR_CreateFile (
          SrcName,
          GENERIC_READ,
          FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
          NULL,
          OPEN_EXISTING,
          FILE_FLAG_SEQUENTIAL_SCAN,
          NULL
          );
    }
  }
  if (SrcHandle == INVALID_HANDLE_VALUE )
  {
    _localLastError=GetLastError();
    SetLastError(_localLastError);
    _LOGCOPYR(SysLog("return COPY_FAILURE -> %d if (SrcHandle==INVALID_HANDLE_VALUE)",__LINE__));
    return COPY_FAILURE;
  }

  HANDLE DestHandle=INVALID_HANDLE_VALUE;
  DWORD AppendPos=0;
  LONG  AppendPosHigh=0;

  if(!(ShellCopy::Flags&FCOPY_COPYTONUL))
  {
    //if (DestAttr!=(DWORD)-1 && !Append) //��� ��� ������ ����������� ������ ����������
      //remove(DestName);
    DestHandle=FAR_CreateFile (
        DestName,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        (ShellCopy::Flags&FCOPY_COPYSECURITY) ? &sa:NULL,
        (Append ? OPEN_EXISTING:CREATE_ALWAYS),
        SrcData.dwFileAttributes&(~((ShellCopy::Flags&(FCOPY_DECRYPTED_DESTINATION))?FILE_ATTRIBUTE_ENCRYPTED|FILE_FLAG_SEQUENTIAL_SCAN:FILE_FLAG_SEQUENTIAL_SCAN)),
        NULL
        );

    ShellCopy::Flags&=~FCOPY_DECRYPTED_DESTINATION;
    if (DestHandle==INVALID_HANDLE_VALUE)
    {
      _localLastError=GetLastError();
      CloseHandle(SrcHandle);
      SetLastError(_localLastError);
      _LOGCOPYR(SysLog("return COPY_FAILURE -> %d CreateFile=-1, LastError=%d (0x%08X)",__LINE__,_localLastError,_localLastError));
      return COPY_FAILURE;
    }

    if (Append)
    {
      AppendPos=SetFilePointer(DestHandle,0,&AppendPosHigh,FILE_END);
      _localLastError=GetLastError();
      if(AppendPos == (DWORD)0xFFFFFFFF && _localLastError != NO_ERROR)
      {
        CloseHandle(SrcHandle);
        CloseHandle(DestHandle);
        SetLastError(_localLastError);
        _LOGCOPYR(SysLog("return COPY_FAILURE -> %d SetFilePointer() == -1, LastError=%d (0x%08X)",__LINE__,_localLastError,_localLastError));
        return COPY_FAILURE;
      }
      SetLastError(_localLastError);
    }
  }

//  unsigned __int64 WrittenSize=0;
  int   AbortOp = FALSE;
  UINT  OldErrMode=SetErrorMode(SEM_NOOPENFILEERRORBOX|SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);
  unsigned __int64 FileSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);

  while (1)
  {
    BOOL IsChangeConsole=OrigScrX != ScrX || OrigScrY != ScrY;
    if (CheckForEscSilent())
    {
      AbortOp = ConfirmAbortOp();
      IsChangeConsole=TRUE; // !!! ������ ���; ��� ����, ����� ��������� �����
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
      int MsgCode = Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                            MSG(MCopyReadError),SrcName,
                            MSG(MRetry),MSG(MCancel));
      ShellCopy::PR_ShellCopyMsg();
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
      SetLastError(_localLastError=LastError);
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
              int MsgCode=Message(MSG_DOWN|MSG_WARNING,4,MSG(MError),
                                  MSG(MErrorInsufficientDiskSpace),DestName,
                                  MSG(MSplit),MSG(MSkip),MSG(MRetry),MSG(MCancel));
              ShellCopy::PR_ShellCopyMsg();
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
                      int MsgCode = Message(MSG_DOWN|MSG_WARNING,2,MSG(MWarning),
                                            MSG(MCopyErrorDiskFull),DestName,
                                            MSG(MRetry),MSG(MCancel));
                      ShellCopy::PR_ShellCopyMsg();
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
          int RetCode;
          if (!AskOverwrite(SrcData,DestName,0xFFFFFFFF,FALSE,((ShellCopy::Flags&FCOPY_MOVE)?TRUE:FALSE),((ShellCopy::Flags&FCOPY_LINK)?0:1),Append,RetCode))
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
          DestHandle=FAR_CreateFile (
              DestName,
              GENERIC_WRITE,
              FILE_SHARE_READ,
              NULL,
              (Append ? OPEN_EXISTING:CREATE_ALWAYS),
              SrcData.dwFileAttributes|FILE_FLAG_SEQUENTIAL_SCAN,
              NULL
              );

          if (DestHandle==INVALID_HANDLE_VALUE ||
              Append && SetFilePointer(DestHandle,0,NULL,FILE_END)==0xFFFFFFFF)
          {
            DWORD LastError=GetLastError();
            CloseHandle(SrcHandle);
            CloseHandle(DestHandle);
            SetErrorMode(OldErrMode);
            SetLastError(_localLastError=LastError);
            _LOGCOPYR(SysLog("return COPY_FAILURE -> %d",__LINE__));
            return COPY_FAILURE;
          }
        }
        else
        {
          if (!SplitCancelled && !SplitSkipped &&
              Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
              MSG(MCopyWriteError),DestName,MSG(MRetry),MSG(MCancel))==0)
          {
            continue;
          }
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
          SetLastError(_localLastError=LastError);
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

    // TODO: ����� ������� Compressed???
    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS &&
        (SrcData.dwFileAttributes & (FA_HIDDEN|FA_SYSTEM|FA_RDONLY)))
      ShellSetAttr(DestName,SrcData.dwFileAttributes&(~((ShellCopy::Flags&(FCOPY_DECRYPTED_DESTINATION))?FILE_ATTRIBUTE_ENCRYPTED:0)));
    ShellCopy::Flags&=~FCOPY_DECRYPTED_DESTINATION;
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
int ShellCopy::ShowBar(unsigned __int64 WrittenSize,unsigned __int64 TotalSize,bool TotalBar)
{
  // // _LOGCOPYR(CleverSysLog clv("ShellCopy::ShowBar"));
  // // _LOGCOPYR(SysLog("WrittenSize=%Ld ,TotalSize=%Ld, TotalBar=%d",WrittenSize,TotalSize,TotalBar));
  if (!ShowTotalCopySize || TotalBar)
    LastShowTime = clock();
/* $ 30.01.2001 VVM
    + ��������� ������� */
  unsigned __int64 OldWrittenSize = WrittenSize;
  unsigned __int64 OldTotalSize = TotalSize;
/* VVM $ */
  //WrittenSize=WrittenSize>>8;
  //TotalSize=TotalSize>>8;

  int Length;
  if (WrittenSize>TotalSize)
    WrittenSize=TotalSize;
  if (!TotalSize)
    Length=BarLength;
  else
    Length=(int)(WrittenSize*BarLength/TotalSize);

  char ProgressBar[100];
  memset(ProgressBar,0x0B0,BarLength);
  ProgressBar[BarLength]=0;
  if (TotalSize)
    memset(ProgressBar,0x0DB,Length);
  SetColor(COL_DIALOGTEXT);
  GotoXY(BarX,BarY+(TotalBar ? 2:0));
  Text(ProgressBar);

  GotoXY(BarX+BarLength,BarY+(TotalBar ? 2:0));

  char Percents[32];
  sprintf (Percents, "%4d%%", ToPercent64 (WrittenSize, TotalSize));

  Text (Percents);

/* $ 30.01.2001 VVM
    + ���������� ����� �����������,���������� ����� � ������� ��������. */
  // // _LOGCOPYR(SysLog("!!!!!!!!!!!!!! ShowCopyTime=%d ,ShowTotalCopySize=%d, TotalBar=%d",ShowCopyTime,ShowTotalCopySize,TotalBar));
  if (ShowCopyTime && (!ShowTotalCopySize || TotalBar))
  {
    unsigned long WorkTime = clock() - CopyStartTime;
    unsigned __int64 SizeLeft = OldTotalSize - OldWrittenSize;
    if (SizeLeft < 0)
      SizeLeft = 0;

    unsigned long CalcTime = OldCalcTime;
    if (WaitUserTime != -1) // -1 => ��������� � �������� �������� ������ �����
      OldCalcTime = CalcTime = WorkTime - WaitUserTime;
    WorkTime /= 1000;
    CalcTime /= 1000;

    unsigned long TimeLeft;
    char TimeStr[100];
    char c[2];
    c[1]=0;

    if (OldTotalSize == 0 || WorkTime == 0)
      sprintf(TimeStr,MSG(MCopyTimeInfo), " ", " ", 0, " ");
    else
    {
      if (TotalBar)
        OldWrittenSize = OldWrittenSize - TotalSkippedSize;
      unsigned long CPS = static_cast<unsigned long>(CalcTime?OldWrittenSize/CalcTime:0);
      TimeLeft = static_cast<unsigned long>((CPS)?SizeLeft/CPS:0);
      c[0]=' ';
      if (CPS > 99999) {
        c[0]='K';
        CPS = CPS/1024;
      }
      if (CPS > 99999) {
        c[0]='M';
        CPS = CPS/1024;
      }
      if (CPS > 99999) {
        c[0]='G';
        CPS = CPS/1024;
      }
      char WorkTimeStr[32];
      char TimeLeftStr[32];
      GetTimeText(WorkTime, WorkTimeStr);
      GetTimeText(TimeLeft, TimeLeftStr);
      sprintf(TimeStr,MSG(MCopyTimeInfo), WorkTimeStr, TimeLeftStr, CPS, c);
    }
    GotoXY(BarX,BarY+(TotalBar?6:4));
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
  WIN32_FIND_DATA DestData={0};
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
        unsigned __int64 SrcSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
        char SrcSizeText[20];
        _i64toa(SrcSize,SrcSizeText,10);
        unsigned __int64 DestSize=MKUINT64(DestData.nFileSizeHigh,DestData.nFileSizeLow);
        char DestSizeText[20];
        _i64toa(DestSize,DestSizeText,10);

        char DateText[20],TimeText[20];
        ConvertDate(SrcData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(SrcFileStr,"%-17s %11.11s %s %s",MSG(MCopySource),SrcSizeText,DateText,TimeText);
        ConvertDate(DestData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(DestFileStr,"%-17s %11.11s %s %s",MSG(MCopyDest),DestSizeText,DateText,TimeText);

        SetMessageHelp("CopyFiles");
        MsgCode=Message(MSG_DOWN|MSG_WARNING,AskAppend?(AskAppend==1?7:6):5,MSG(MWarning),
                MSG(MCopyFileExist),DestName,"\x1",SrcFileStr,DestFileStr,
                "\x1",MSG(MCopyOverwrite),MSG(MCopyOverwriteAll),
                MSG(MCopySkipOvr),MSG(MCopySkipAllOvr),
                AskAppend?(AskAppend==1?MSG(MCopyAppend):MSG(MCopyResume)):MSG(MCopyCancelOvr),
                AskAppend?(AskAppend==1?MSG(MCopyAppendAll):MSG(MCopyCancelOvr)):NULL,
                AskAppend==1?MSG(MCopyCancelOvr):NULL);
        if((!AskAppend && MsgCode==4)||AskAppend>1 && MsgCode==5)
          MsgCode=6;
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
    case 5:
      Append=TRUE;
      OvrMode=5;
      RetCode=COPY_NEXT;
      break;
    case -1:
    case -2:
    case 6:
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

        unsigned __int64 SrcSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
        char SrcSizeText[20];
        _i64toa(SrcSize,SrcSizeText,10);
        unsigned __int64 DestSize=MKUINT64(DestData.nFileSizeHigh,DestData.nFileSizeLow);
        char DestSizeText[20];
        _i64toa(DestSize,DestSizeText,10);

        ConvertDate(SrcData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(SrcFileStr,"%-17s %11.11s %s %s",MSG(MCopySource),SrcSizeText,DateText,TimeText);
        ConvertDate(DestData.ftLastWriteTime,DateText,TimeText,8,FALSE,FALSE,TRUE,TRUE);
        sprintf(DestFileStr,"%-17s %11.11s %s %s",MSG(MCopyDest),DestSizeText,DateText,TimeText);

        SetMessageHelp("CopyFiles");
        MsgCode=Message(MSG_DOWN|MSG_WARNING,AskAppend?(AskAppend==1?7:6):5,MSG(MWarning),
                MSG(MCopyFileRO),DestName,"\x1",SrcFileStr,DestFileStr,
                "\x1",MSG(MCopyOverwrite),MSG(MCopyOverwriteAll),
                MSG(MCopySkipOvr),MSG(MCopySkipAllOvr),
                AskAppend?(AskAppend==1?MSG(MCopyAppend):MSG(MCopyResume)):MSG(MCopyCancelOvr),
                AskAppend?(AskAppend==1?MSG(MCopyAppendAll):MSG(MCopyCancelOvr)):NULL,
                AskAppend==1?MSG(MCopyCancelOvr):NULL);
        if((!AskAppend && MsgCode==4)||AskAppend>1 && MsgCode==5)
          MsgCode=6;
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
      case 5:
        Append=TRUE;
        ReadOnlyOvrMode=5;
        RetCode=COPY_NEXT;
        break;
      case -1:
      case -2:
      case 6:
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
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::GetSecurity()"));
  _LOGCOPYR(SysLog("Params: FileName='%s'",FileName));
  char AnsiName[NM];
  SECURITY_INFORMATION si=DACL_SECURITY_INFORMATION;
  SECURITY_DESCRIPTOR *sd=(SECURITY_DESCRIPTOR *)sddata;
  DWORD Needed;
  SetFileApisTo(APIS2ANSI);
  FAR_OemToChar(FileName,AnsiName);
  BOOL RetSec=GetFileSecurity(AnsiName,si,sd,SDDATA_SIZE,&Needed);
  int LastError=GetLastError();
  _LOGCOPYR(SysLog("LastError=%u Attr=0x%08X",LastError,GetFileAttributes(AnsiName)));
  SetFileApisTo(APIS2OEM);
  if (!RetSec)
  {
    sd=NULL;
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
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::SetSecurity()"));
  _LOGCOPYR(SysLog("Params: FileName='%s'",FileName));
  char AnsiName[NM];
  SECURITY_INFORMATION si=DACL_SECURITY_INFORMATION;
  SetFileApisTo(APIS2ANSI);
  FAR_OemToChar(FileName,AnsiName);
  BOOL RetSec=SetFileSecurity(AnsiName,si,(PSECURITY_DESCRIPTOR)sa.lpSecurityDescriptor);
  int LastError=GetLastError();
  _LOGCOPYR(SysLog("LastError=%u",LastError));
  SetFileApisTo(APIS2OEM);
  if (!RetSec)
  {
    if (LastError!=ERROR_SUCCESS && LastError!=ERROR_FILE_NOT_FOUND &&
        LastError!=ERROR_CALL_NOT_IMPLEMENTED &&
        Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                MSG(MCannotSetSecurity),FileName,MSG(MOk),MSG(MCancel))==1)
      return(FALSE);
  }
  return(TRUE);
}


BOOL ShellCopySecuryMsg(const char *Name)
{
  static clock_t PrepareSecuryStartTime=0;
  static int Width=30;
  int WidthTemp;
  char OutFileName[NM];

  if (Name == NULL || *Name == 0 || (static_cast<DWORD>(clock() - PrepareSecuryStartTime) > Opt.ShowTimeoutDACLFiles))
  {
    if(Name && *Name)
    {
      PrepareSecuryStartTime = clock();     // ������ ���� �������� ������
      WidthTemp=Max((int)strlen(Name),(int)30);
    }
    else
      Width=WidthTemp=30;

    if(WidthTemp > WidthNameForMessage)
      WidthTemp=WidthNameForMessage; // ������ ������ - 38%
    if(WidthTemp >= sizeof(OutFileName)-4)
      WidthTemp=sizeof(OutFileName)-5;
    if(Width < WidthTemp)
      Width=WidthTemp;

    xstrncpy(OutFileName,Name,sizeof(OutFileName)-1);
    TruncPathStr(OutFileName,Width);
    CenterStr(OutFileName,OutFileName,Width+4);

    Message(0,0,MSG(MMoveDlgTitle),MSG(MCopyPrepareSecury),OutFileName);

    if(CheckForEscSilent())
    {
      if(ConfirmAbortOp())
        return FALSE;
    }
  }
  PreRedrawParam.Param1=static_cast<void*>(const_cast<char*>(Name));
  return TRUE;
}

static void PR_ShellCopySecuryMsg(void)
{
  ShellCopySecuryMsg(static_cast<const char*>(PreRedrawParam.Param1));
}

int ShellCopy::SetRecursiveSecurity(const char *FileName,const SECURITY_ATTRIBUTES &sa)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::SetRecursiveSecurity()"));
  _LOGCOPYR(SysLog("Params: FileName='%s'",FileName));
  if(SetSecurity(FileName,sa))
  {
    if(::GetFileAttributes(FileName)&FILE_ATTRIBUTE_DIRECTORY)
    {
      SaveScreen SaveScr;
      PREREDRAWFUNC OldPreRedrawFunc=PreRedrawFunc;
      //SetCursorType(FALSE,0);
      SetPreRedrawFunc(PR_ShellCopySecuryMsg);
      //ShellCopySecuryMsg("");

      char FullName[NM*2];
      WIN32_FIND_DATA SrcData;
      ScanTree ScTree(TRUE,TRUE,ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS);
      ScTree.SetFindPath(FileName,"*.*",FSCANTREE_FILESFIRST);
      while (ScTree.GetNextName(&SrcData,FullName, sizeof (FullName)-1))
      {
        if(!ShellCopySecuryMsg(FullName))
          break;
        if(!SetSecurity(FullName,sa))
        {
          SetPreRedrawFunc(OldPreRedrawFunc);
          return FALSE;
        }
      }
      SetPreRedrawFunc(OldPreRedrawFunc);
    }
    return TRUE;
  }
  _LOGCOPYR(else SysLog("[%d] SetSecurity failed",__LINE__));
  return FALSE;
}


int ShellCopy::ShellSystemCopy(const char *SrcName,const char *DestName,const WIN32_FIND_DATA &SrcData)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::ShellSystemCopy()"));
  _LOGCOPYR(SysLog("Params: SrcName='%s', DestName='%s'",SrcName, DestName));
  _LOGCOPYR(WIN32_FIND_DATA_Dump("",SrcData));
  {
    _LOGCOPYR(static char Root[1024]);
    _LOGCOPYR(char lpRootPathName[NM]="");
    _LOGCOPYR(char lpVolumeNameBuffer[NM]="");
    _LOGCOPYR(char lpFileSystemNameBuffer[NM]="");
    _LOGCOPYR(DWORD lpVolumeSerialNumber=0);
    _LOGCOPYR(DWORD lpMaximumComponentLength=0);
    _LOGCOPYR(DWORD lpFileSystemFlags=0);
    _LOGCOPYR(GetPathRoot(SrcName,Root));
    _LOGCOPYR(GetVolumeInformation(Root,lpVolumeNameBuffer,sizeof(lpVolumeNameBuffer),
                                             &lpVolumeSerialNumber,&lpMaximumComponentLength,&lpFileSystemFlags,
                                             lpFileSystemNameBuffer,sizeof(lpFileSystemNameBuffer)));
    _LOGCOPYR(GetVolumeInformation_Dump("Src",Root,lpVolumeNameBuffer,sizeof(lpVolumeNameBuffer),
                                             lpVolumeSerialNumber,lpMaximumComponentLength,lpFileSystemFlags,
                                             lpFileSystemNameBuffer,sizeof(lpFileSystemNameBuffer),NULL));
    _LOGCOPYR(GetPathRoot(DestName,Root));
    _LOGCOPYR(GetVolumeInformation(Root,lpVolumeNameBuffer,sizeof(lpVolumeNameBuffer),
                                             &lpVolumeSerialNumber,&lpMaximumComponentLength,&lpFileSystemFlags,
                                             lpFileSystemNameBuffer,sizeof(lpFileSystemNameBuffer)));
    _LOGCOPYR(GetVolumeInformation_Dump("Dest",Root,lpVolumeNameBuffer,sizeof(lpVolumeNameBuffer),
                                             lpVolumeSerialNumber,lpMaximumComponentLength,lpFileSystemFlags,
                                             lpFileSystemNameBuffer,sizeof(lpFileSystemNameBuffer),NULL));
  }

  SECURITY_ATTRIBUTES sa;
  if ((ShellCopy::Flags&FCOPY_COPYSECURITY) && !GetSecurity(SrcName,sa))
    return(COPY_CANCEL);

  //// // _LOGCOPYR(SysLog("[%p] ShellCopy::ShellSystemCopy('%s','%s',..)",this,SrcName,DestName));
  ShellCopyMsg(SrcName,DestName,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);

  if (Init_CopyFileEx())
  {
    BOOL Cancel=0;
    TotalCopiedSizeEx=TotalCopiedSize;
#ifndef COPY_FILE_ALLOW_DECRYPTED_DESTINATION
#define COPY_FILE_ALLOW_DECRYPTED_DESTINATION 0x00000008
#endif
    // COPY_FILE_ALLOW_DECRYPTED_DESTINATION
    if (!FAR_CopyFileEx(SrcName,DestName,(void *)CopyProgressRoutine,NULL,&Cancel,
         ShellCopy::Flags&(FCOPY_DECRYPTED_DESTINATION)?COPY_FILE_ALLOW_DECRYPTED_DESTINATION:0))
    {
      ShellCopy::Flags&=~FCOPY_DECRYPTED_DESTINATION;
      return (_localLastError=GetLastError())==ERROR_REQUEST_ABORTED ? COPY_CANCEL:COPY_FAILURE;
    }
    ShellCopy::Flags&=~FCOPY_DECRYPTED_DESTINATION;
  }
  else
  {
    if (ShowTotalCopySize)
    {
      unsigned __int64 AddSize=MKUINT64(SrcData.nFileSizeHigh,SrcData.nFileSizeLow);
      TotalCopiedSize += AddSize;
      CurCopiedSize = AddSize;
      ShowBar(TotalCopiedSize,TotalCopySize,true);
      ShowTitle(FALSE);
    }
    // ����� ��... ����� ���� ������ ���, ��� �� ����� �� �������� ���� CopyFileExA
    if (!FAR_CopyFile(SrcName,DestName,FALSE))
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

  unsigned __int64 TransferredSize=MKUINT64(TotalBytesTransferred.u.HighPart,TotalBytesTransferred.u.LowPart);
  unsigned __int64 TotalSize=MKUINT64(TotalFileSize.u.HighPart,TotalFileSize.u.LowPart);

  int AbortOp = FALSE;
  BOOL IsChangeConsole=OrigScrX != ScrX || OrigScrY != ScrY;
  if (CheckForEscSilent())
  {
    // // _LOGCOPYR(SysLog("2='%s'/0x%08X  3='%s'/0x%08X  Flags=0x%08X",(char*)PreRedrawParam.Param2,PreRedrawParam.Param2,(char*)PreRedrawParam.Param3,PreRedrawParam.Param3,PreRedrawParam.Flags));
    AbortOp = ConfirmAbortOp();
    IsChangeConsole=TRUE; // !!! ������ ���; ��� ����, ����� ��������� �����
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
  return CheckDisksProps(SrcPath,DestPath,CHECKEDPROPS_ISSAMEDISK);
}


bool ShellCopy::CalcTotalSize()
{
  char SelName[NM],SelShortName[NM];
  int FileAttr;

  // ��� �������
  WIN32_FIND_DATA fd;

  TotalCopySize=CurCopiedSize=0;
  TotalFilesToProcess = 0;

  ShellCopyMsg(NULL,"",MSG_LEFTALIGN);

  SrcPanel->GetSelName(NULL,FileAttr);
  while (SrcPanel->GetSelName(SelName,FileAttr,SelShortName,&fd))
  {
    if ((FileAttr&FILE_ATTRIBUTE_REPARSE_POINT) && !(ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS))
        continue;

    if (FileAttr & FILE_ATTRIBUTE_DIRECTORY)
    {
      {
        unsigned long DirCount,FileCount,ClusterSize;
        unsigned __int64 FileSize,CompressedSize,RealFileSize;
        ShellCopyMsg(NULL,SelName,MSG_LEFTALIGN|MSG_KEEPBACKGROUND);
        if (!GetDirInfo("",SelName,DirCount,FileCount,FileSize,CompressedSize,
                        RealFileSize,ClusterSize,0xffffffff,
                        Filter,
                        (ShellCopy::Flags&FCOPY_COPYSYMLINKCONTENTS?GETDIRINFO_SCANSYMLINK:0)|
                        (UseFilter?GETDIRINFO_USEFILTER:0)))
        {
          ShowTotalCopySize=false;
          return(false);
        }
        TotalCopySize+=FileSize;
        TotalFilesToProcess += FileCount;
      }
    }
    else
    {
      if (UseFilter)
      {
        if (!Filter->FileInFilter(&fd))
          continue;
      }

      unsigned __int64 FileSize;
      if (SrcPanel->GetLastSelectedSize((__int64*)&FileSize)!=-1)
      {
        TotalCopySize+=FileSize;
        TotalFilesToProcess++;
      }
    }
  }
  // TODO: ��� ��� ��������, ����� "����� = ����� ������ * ���������� �����"
  TotalCopySize=TotalCopySize*(__int64)CountTarget;

  InsertCommas(TotalCopySize,TotalCopySizeText,sizeof(TotalCopySizeText));
  return(true);
}


void ShellCopy::ShowTitle(int FirstTime)
{
  if (ShowTotalCopySize && !FirstTime)
  {
    StaticCopyTitle->Set("{%d%%} %s",ToPercent64(TotalCopiedSize,TotalCopySize),StaticMove ? MSG(MCopyMovingTitle):MSG(MCopyCopyingTitle));
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
  for (I=(int)strlen(SrcFullName)-1;I>0 && SrcFullName[I]=='.';I--)
    SrcFullName[I]=0;
  DeleteEndSlash(SrcFullName);
  _LOGCOPYR(SysLog("SrcFullName='%s'",SrcFullName));
  for (I=(int)strlen(DestFullName)-1;I>0 && DestFullName[I]=='.';I--)
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

int ShellCopy::CmpFullPath(const char *Src,const char *Dest)
{
  _LOGCOPYR(CleverSysLog Clev("ShellCopy::CmpFullPath()"));
  _LOGCOPYR(SysLog("Params: Src='%s', Dest='%s'",Src, Dest));
  char SrcFullName[1024],DestFullName[1024];
  int I;

  GetParentFolder(Src,SrcFullName, sizeof(SrcFullName));
  GetParentFolder(Dest,DestFullName, sizeof(DestFullName));

  // ������ ����� �� ����
  for (I=(int)strlen(SrcFullName)-1;I>0 && SrcFullName[I]=='.';I--)
    SrcFullName[I]=0;
  DeleteEndSlash(SrcFullName);
  _LOGCOPYR(SysLog("SrcFullName='%s'",SrcFullName));
  for (I=(int)strlen(DestFullName)-1;I>0 && DestFullName[I]=='.';I--)
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

char *ShellCopy::GetParentFolder(const char *Src, char *Dest, int LenDest)
{
  char DestFullName[2048];
  if (ConvertNameToReal(Src,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
    return NULL;
  char *Ptr=strrchr(DestFullName,'\\');
  if(Ptr)
    *Ptr=0;
  strncpy(Dest,DestFullName,LenDest);
  return Dest;
}

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
    xstrncpy(SelOnlyName,SelName,sizeof(SelOnlyName)-1);
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
      if (ConvertNameToReal(SelName,SrcFullName, sizeof(SrcFullName)) >= sizeof(SrcFullName))
        return 0;
    _LOGCOPYR(SysLog("Src: ConvertNameToReal('%s','%s')",SelName,SrcFullName));

    if (ConvertNameToFull(Dest,DestFullName, sizeof(DestFullName)) >= sizeof(DestFullName))
      return 0;
    _LOGCOPYR(SysLog("Dst: ConvertNameToFull('%s','%s')",Dest,DestFullName));

//    char *EndDestFullName=DestFullName+strlen(DestFullName);
    if(DestFullName[strlen(DestFullName)-1] == '\\')
    {
      if(!(Flags&FCOPY_VOLMOUNT))
      {
        // AddEndSlash(DestFullName);
        xstrncat(DestFullName,PtrSelName,sizeof(DestFullName)-1);
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
    if(JSAttr != -1) // ���������� �����?
    {
      if((JSAttr&FILE_ATTRIBUTE_DIRECTORY)!=FILE_ATTRIBUTE_DIRECTORY)
      {
        if(!(Flags&FCOPY_NOSHOWMSGLINK))
        {
          Message(MSG_DOWN|MSG_WARNING,1,MSG(MError),
                MSG(MCopyCannotCreateJunctionToFile),
                DestFullName,MSG(MOk));
        }
        return 0;
      }

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
  if((Attr&FILE_ATTRIBUTE_COMPRESSED) && !(Attr&FILE_ATTRIBUTE_ENCRYPTED))
  {
    int Ret=ESetFileCompression(Dest,1,Attr&(~FILE_ATTRIBUTE_COMPRESSED),SkipMode);
    if(Ret==SETATTR_RET_ERROR)
    {
      _LOGCOPYR(SysLog("return 0 -> %d ESetFileCompression('%s',1,0) == 0",__LINE__,Dest));
      return FALSE;
    }
    else if(Ret==SETATTR_RET_SKIPALL)
      this->SkipMode=SETATTR_RET_SKIP;
  }
  // ��� �����������/�������� ���������� FILE_ATTRIBUTE_ENCRYPTED
  // ��� ��������, ���� �� ����
  if (GetInfoSuccess && (FileSystemFlagsDst&FILE_SUPPORTS_ENCRYPTION) &&
     (Attr&(FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_DIRECTORY)) == (FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_DIRECTORY))
  {
    int Ret=ESetFileEncryption(Dest,1,0,SkipMode);
    if (Ret==SETATTR_RET_ERROR)
    {
      _LOGCOPYR(SysLog("return 0 -> %d ESetFileEncryption('%s',1,0) == 0",__LINE__,Dest));
      return FALSE;
    }
    else if(Ret==SETATTR_RET_SKIPALL)
      SkipMode=SETATTR_RET_SKIP;
  }
  _LOGCOPYR(SysLog("return 1 -> %d",__LINE__));
  return TRUE;
  /* VVM $ */
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
