/*
panel.cpp

Parent class ��� �������

*/

/* Revision: 1.115 13.10.2003 $ */

/*
Modify:
  13.10.2003 SVS
    - BugZ#973 - panels QuickSearch wish
  09.10.2003 SVS
    - BugZ#858 - �������� � [Drive]
  12.09.2003 SVS
    ! ��������� ������ Panel::SetCurPath().
  04.06.2003 SVS
    ! ������ ������� '**' ��� ��������� FastFind
  08.05.2003 SVS
    - BugZ#888 - ��� ������ ���������� � ����� ����
  02.05.2003 SVS
    - BugZ#727 - [wish] ���������� Ctrl-R � ���� ������ ������
      ������ ���������:
      "...����� Ctrl-R ��� ����� ������� �� A:, ���� �� �������������
      ������ ����� �� ����� �� �������� (��������, FTP)..."
  05.03.2003 SVS
    ! ���������� _SVS
  23.02.2003 SVS
    + BugZ#727 - [wish] ���������� Ctrl-R � ���� ������ ������
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
  03.02.2003 SVS
    - BugZ#787 - ����� ������������ QuickSearch ��� ���������� �������.
  20.01.2003 SVS
    + ����� �� F1 � FastFind ���������������� ������� ������
  15.01.2003 SVS
    ! ������������� Fastind �� ������� Alt-� � ��� � ���
  11.12.2002 VVM
    - ��� �������� �������� ����� ��� �� � 9� ������ ����� �������.
  10.12.2002 SVS
    + ProcessDelDisk() - ������ ������ ��������, ��������� �� VMenu ��� ����, ����� �����
      ������������ ����!
    - BugZ#721 - �������� ��������� ��� �������� ������������� �����
  09.11.2002 SVS
    - Bug: � ������� Ctrl-L Ctrl-U - ����� ����.
      ������� ����� ������������� ViewSettings.FullScreen � 0
  27.09.2002 SVS
    - BugZ#640 - ���������
  22.01.2002 IS
    ! ���������� ��������� ����� �������� (���� ������ ������)
  21.01.2002 IS
    + ������ ����������� �� ���������� ������� �������� � ���� ������ ������
  18.06.2002 SVS
    + Panel::IfGoHome()
    ! ��� �� Panel::ProcessDelDisk(), ���������� �� ���������� CD �������
      � ���������� KEY_DEL ���� ������ ������. ���� �� ��������� �����������
      ����������� ������������ ��� ����, ����� ������ �������� � �����������
    ! � ������� ��������� ���������� StartManager, ���������� �� ������
      "������� ��� ���������?". ��� ��������� � Panel::SetCurPath() ��������
      ��������, ����� ��� ������ FAR`� ����� ��� �� ���� (������ CD �����).
      ��� ���� �� ������ �������� � ������ ���� �����, ������ �������
      ��� (�.�. ������� ��� �� �������, �� ��������� ��������, �����
      � ����� �������� ��� �������� ������� ���� ����� �����... � �������
      ��� �� ��������� - �������� ���� ������, ���� GPF)
  24.05.2002 SVS
    + ������������ Numpad-������
  22.05.2002 SVS
    ! Opt.CloseCDGate + IsDiskInDrive() ����� ������������� CD
  15.05.2002 SVS
    ! ������ ������� ���������� �������� ���������� �������������
      ������� �������
  11.04.2002 SVS
    + FCTL_GET[ANOTHER]PANELSHORTINFO
    + �������� ���� IsBadWritePtr(Param,sizeof(struct PanelInfo))
  22.03.2002 DJ
    ! ��������� �� BoundsChecker
  20.03.2002 SVS
    ! GetCurrentDirectory -> FarGetCurDir
  19.03.2002 DJ
    ! ��� ��������� FCTL_GET[ANOTHER]PANELINFO ��������� ������, ����
      ���� ������ ��������
  21.02.2002 SVS
    + ��������� ������, ��� WC ;-) ���� CD-���� �������� � �� ��� ������
      ����� ����� �� ���� ������� Enter, ��... ������� ������ �����, � �����
      ������ ������� ���������� ;-)
  15.02.2002 IS
    - �� ������� ��������� ������� � SetCurPath
  09.02.2002 VVM
    ! ����� ���� Del �� ������ - �� ���� �������� �������.
  06.02.2002 tran
    ! Bugz#208 - �������� ��� ��� - ������ ������ ���� FILE_PANEL
      � �� TREE, INFO, QUICK � ��.
  05.02.2002 SVS
    ! BugZ#208 - ������ ������ ���� ������� ��� "�������� Update", �����
      ����������� ������
  04.02.2002 SVS
    ! ��������� ��� BugZ#208 - ������ ������� Update, ���� ������ ��
      ���������� � ���� ���������.
  31.01.2002 SVS
    - BugZ#208 - ������������ ������� � ������.
  19.01.2002 VVM
    ! ��� �������� ���������� ����� �� ���� �������� �� ���������� �����
  18.01.2002 VVM
    ! ��������� �� setdisk() - ���������� FarChDir
  14.01.2002 IS
    ! chdir -> FarChDir
  28.12.2001 DJ
    ! ��������� Del � ���� ������ �������� � ��������� �������
  14.12.2001 IS
    ! stricmp -> LocalStricmp
  14.12.2001 VVM
    ! �������� � NeedUpdatePanel
  12.12.2001 DJ
    ! �������� ������ � GETPANELINFO
  06.12.2001 SVS
    ! PrepareDiskPath() - ����� ���.�������� - ������������ ������ ������
    ! �� ����� ������ ������ FCTL_SETANOTHERPANELDIR � FCTL_SETPANELDIR ��
      �������� PrepareDiskPath(), ��� �������� ����� (��������� ������ �����
      �������)
  03.12.2001 DJ
    - �������� PrepareDiskPath() ��� ����, ����������� � FCTL_SETPANELDIR
      � FCTL_SETANOTHERPANELDIR
  27.11.2001 SVS
    + GetCurBaseName() ������ �� ���� ��� ��������� ������� ��� ��������
      � ������ ����������� ���������� �������, �.�. ��� ������ ��������
      ����-����� � �����.
  26.11.2001 SVS
    ! ������� PrepareDiskPath() ��� �������������� ����.
  19.11.2001 OT
    - ����������� ��������� ������ ������������� �������. 115 � 116 ����
  12.11.2001 SVS
    ! ����� 1041 �� ������ ������.
  08.11.2001 SVS
    - Shift-Enter �� ������ �������� ����� ���, �� ������ �� ���������
  01.11.2001 SVS
    ! ������ Opt.CPAJHefuayor ;-(
  30.10.2001 SVS
    + HEFUAYOR: ���� ������ ����������� � �������� � �������� �������� �����,
      ��������� � ����������, ��... �������� ��� ��������. ���� ������������
      � �� ��������� ���� ���������
  29.10.2001 SVS
    ! ��������� Panel::SetCurPath()
  24.10.2001 SVS
    ! ������� ����������� � SetCurPath()
  05.10.2001 SVS
    ! ��� ������ �������� ������ �������� � Panel::SetCurPath()
      ��� ��������� ������, � �� �����...
    ! ������� ������� Panel::MakeListFile() � fnparse.cpp - ��� �� �����
  03.10.2001 IS
    + ���������� ������� ���� ��� ������ ������
  01.10.2001 IS
    - ������������� ������� ��� �������� ������� �����
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  26.09.2001 SVS
    + Panel::NeedUpdatePanel() - ����� �� ��������� ������ � ������ ������
      ��������� Opt.AutoUpdateLimit
  24.09.2001 SVS
    ! ������� ����������� (���������� ����)
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  24.07.2001 SVS
    + Opt.PgUpChangeDisk
  23.07.2001 SVS
    ! � ������� FastFind() ��� ��������� WaitInFastFind++ � WaitInFastFind--
      ��� ����� ��� ����� (���� ���������� ��� ��� �� ��� � ��������)
  23.07.2001 SVS
    ! ������ ������� ����� ����������� ������� � ���� ������ �����
      �� ��������� �������� ������� (+1) ����� � �������� "MB" �� "M"
  22.07.2001 SVS
    + ��������� ������� CtrlPgUp � ���� ������ ������ ����� ��� ����.
    + Shift-Enter � ���� ������ ������ �������� ��������� ��� ������� �����
  18.07.2001 OT
    ! VFMenu
  21.06.2001 tran
    ! ����� ���� AltF1,F1,esc
  17.06.2001 KM
    ! ���������� WRAPMODE � ����.
  14.06.2001 KM
    + ��������� ��������� ���������� ���������, ������������
      ������� ���������� ������ ��� ��� ��������, ��� � ���
      ��������� ������. ��� ���������� ���������� �����������
      �� FAR.
  06.06.2001 SVS
    ! Mix/Max
  03.06.2001 SVS
    ! ��������� � ����� � ���������� UserData � VMenu
  21.05.2001 DJ
    ! � ����� � ���������� ������ ���� ����������� ���� ���������� ���������
      �������� ������� �������
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
  12.05.2001 DJ
    - FCTL_REDRAW[ANOTHER]PANEL �������������� ������ �����, ����� ������
      �������� �������� �������
  10.05.2001 SVS
    - ���������: ��� �������� �������� ����� � ���� "������ �����" �����
      ������������ ������� (����� ������).
  09.05.2001 OT
    - ����������� Panel::Show
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
  30.04.2001 DJ
    - �� ������ ������� �� Esc ������� ������ �����, ��������� �� quick view
      ������
  28.04.2001 VVM
    + GetSubstName() ��������� ��� ��������
  27.04.2001 SVS
    + �.�. ��� ������� �������� ��������� "����������" ����������,
      �� ������� ��������� Ins ��� CD - "������� ����"
  26.04.2001 VVM
    ! ������ ����� 547
  24.04.2001 SVS
    + ���������� ������ PanelInfo.Flags
    - !@AFQ! � ����� ������ ����� ����� "E:\\file.txt", �.�. 2 �����.
  24.04.2001 VVM
    - ��� ��� ����� ������� ����������
  22.04.2001 SVS
    ! ��������� ������ ����� ����� 547
    + ������� ��� NT/2000 - ��������� Del �� CDROOM ��������� ����.
      ����������� ���� �� ��� :-)
  19.04.2001 SVS
    - �� �������� SUBST-���� - ��������� ������������ �������� DriveType
      � ������ �������� "�� ����� �������� cd-rom ��� ������� ����"
  09.04.2001 SVS
    ! �������� � ������� �������.
  02.04.2001 SVS
    ! DRIVE_SUSTITUTE -> DRIVE_SUBSTITUTE
  02.04.2001 VVM
    ! ������� �� ������ ������ �����...
  30.03.2001 SVS
    ! GetLogicalDrives ������� �� FarGetLogicalDrives() � ����� � �������
      �������� �� ��������� �������� "�������".
  28.03.2001 VVM
    + ��������� Opt.RememberLogicalDrives.
  27.03.2001 SVS
    + Shift-F1 �� ������ ������� � ���� ������ ������ ���� ������� ����...
  16.03.2001 SVS
    + ��������� ���������� � ���� ���������� � ������ ������������� ��������
      ���������� �����.
    ! ����� ����, ���������� ���������� � RemoteName ������� � �������
      DriveLocalToRemoteName()
  15.03.2001 SVS
    ! Del � ���� ������
  15.03.2001 IS
    + ���������� �������������� ������, � �� ������ '#', ��� ������, ���� �����
      �� �������� � ���� ������ ����� ������ 9 ����
  12.03.2001 SVS
    ! ��������� � ����� � ����������� � ������ int64
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  27.02.2001 VVM
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
  26.02.2001 VVM
    - ������ ����������� �����
  26.02.2001 VVM
    ! ��������� NULL ����� OpenPlugin
  14.02.2001 SVS
    ! �������������� �������� ��� MakeListFile - ������������
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  22.01.2001 VVM
    - ������� ���������� �������� ���������� StartSortOrder
  09.01.2001 SVS
    - ��� KEY_XXX_BASE ����� ��������� 0x01
  05.01.2001 SVS
    + ��������� ������� SUBST ����
    + ������� ����, ��� ��� - SUBST-����
  14.12.2000 SVS
    + ��������� ������� Eject ��� ������� ������ � ���� ������ ���������.
  11.11.2000 SVS
    ! FarMkTemp() - ������� (��� ������ - �� ������, �� ��� �� ������� :-(((
  11.11.2000 SVS
    ! ���������� ����������� FarMkTemp()
  24.09.2000 SVS
    ! ����������� CtrlObject->MainKeyBar (������, ����:
       Ctr-Alt-Shift, �����, Alt-������, �������� ������� �����,
       ������ ��������� Ctrl � Shift - ���� �������� ������ �� �����.
       ������, ���� ������ Esc - KeyBar �� ������������)
  08.09.2000 VVM
    + ��������� ������
      FCTL_SETSORTMODE, FCTL_SETANOTHERSORTMODE
      FCTL_SETSORTORDER, FCTL_SETANOTHERSORTORDER
  07.09.2000 SVS
    ! ��� ���� ���������� (� ������ AT) ��� Bug#12:
      ��� �����, �� ���� ����� �� ����, ���� � ��� ������� ���� - UNC
      "\\server\share\"
  06.09.2000 tran
    - ����� ���, ������ ���� �����:
       1. strncpy �� ���������� 0 � ����� ������
       2. GetDriveType ���������� ���������, ��� ������ ����.
  05.09.2000 SVS
    - Bug#12 -   ��� �������� �������� ����� �� Del � ������ �� ����
        ��� ���������� ���������� ��������� ����. ���� �� ������.
        �� ctrl-r ��������� �� ���������.
  21.07.2000 IG
    - Bug 21 (��������� ����� Ctrl-Q, Tab, F3, Esc ��� ������)
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "panel.hpp"
#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "vmenu.hpp"
#include "filepanels.hpp"
#include "cmdline.hpp"
#include "chgmmode.hpp"
#include "chgprior.hpp"
#include "edit.hpp"
#include "treelist.hpp"
#include "filelist.hpp"
#include "dialog.hpp"
#include "savescr.hpp"
#include "manager.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"
#include "array.hpp"
#include "lockscrn.hpp"
#include "help.hpp"

static int DragX,DragY,DragMove;
static Panel *SrcDragPanel;
static SaveScreen *DragSaveScr=NULL;
static char DragName[NM];

static unsigned char VerticalLine=0x0B3;

static int MessageRemoveConnection(char Letter, int &UpdateProfile);

/* $ 21.08.2002 IS
   ����� ��� �������� ������ ������� � ���� ������ ������
*/
class ChDiskPluginItem
{
  public:
   MenuItem Item;
   unsigned int HotKey;
   ChDiskPluginItem():HotKey(0)
   {
     memset(&Item,0,sizeof(Item));
   }
   bool operator==(const ChDiskPluginItem &rhs) const;
   int operator<(const ChDiskPluginItem &rhs) const;
   const ChDiskPluginItem& operator=(const ChDiskPluginItem &rhs);
   ~ChDiskPluginItem()
   {
   }
};

bool ChDiskPluginItem::operator==(const ChDiskPluginItem &rhs) const
{
  return HotKey==rhs.HotKey &&
    !LocalStricmp(Item.Name,rhs.Item.Name) &&
    Item.UserData==rhs.Item.UserData;
}

int ChDiskPluginItem::operator<(const ChDiskPluginItem &rhs) const
{
  if(HotKey==rhs.HotKey)
    return LocalStricmp(Item.Name,rhs.Item.Name)<0;
  else if(HotKey && rhs.HotKey)
    return HotKey < rhs.HotKey;
  else
    return HotKey && !rhs.HotKey;
}

const ChDiskPluginItem& ChDiskPluginItem::operator=(const ChDiskPluginItem &rhs)
{
  Item=rhs.Item;
  HotKey=rhs.HotKey;
  return *this;
}
/* IS $ */


Panel::Panel()
{
  _OT(SysLog("[%p] Panel::Panel()", this));
  Focus=0;
  *CurDir=0;
  PanelMode=NORMAL_PANEL;
  PrevViewMode=VIEW_3;
  EnableUpdate=TRUE;
  DragX=DragY=-1;
  SrcDragPanel=NULL;
  ModalMode=0;
  ViewSettings.ColumnCount=0;
  ViewSettings.FullScreen=0;
  ProcessingPluginCommand=0;
};


Panel::~Panel()
{
  _OT(SysLog("[%p] Panel::~Panel()", this));
  EndDrag();
}


void Panel::SetViewMode(int ViewMode)
{
  PrevViewMode=ViewMode;
  Panel::ViewMode=ViewMode;
};


void Panel::ChangeDirToCurrent()
{
  char NewDir[NM];
  FarGetCurDir(sizeof(NewDir),NewDir);
  SetCurDir(NewDir,TRUE);
}


void Panel::ChangeDisk()
{
  int Pos,FirstCall=TRUE;
  if (CurDir[0]!=0 && CurDir[1]==':')
    Pos=toupper(CurDir[0])-'A';
  else
    Pos=getdisk();
  while (Pos!=-1)
  {
    Pos=ChangeDiskMenu(Pos,FirstCall);
    FirstCall=FALSE;
  }
}


int  Panel::ChangeDiskMenu(int Pos,int FirstCall)
{
  struct MenuItem ChDiskItem;
  char DiskType[100],RootDir[10],DiskLetter[50];
  DWORD Mask,DiskMask;
  int DiskCount,Focus,I,J;
  int ShowSpecial=FALSE, SetSelected=FALSE;

  memset(&ChDiskItem,0,sizeof(ChDiskItem));
  *DiskLetter=0;

  _tran(SysLog("Panel::ChangeDiskMenu(), Pos=%i, FirstCall=%i",Pos,FirstCall));
  Mask=FarGetLogicalDrives();

  for (DiskMask=Mask,DiskCount=0;DiskMask!=0;DiskMask>>=1)
    DiskCount+=DiskMask & 1;

  int UserDataSize=0;
  DWORD UserData=NULL;
  {
    _tran(SysLog("create VMenu ChDisk"));
    VMenu ChDisk(MSG(MChangeDriveTitle),NULL,0,ScrY-Y1-3);
    ChDisk.SetFlags(VMENU_NOTCENTER);
    if ( this==CtrlObject->Cp()->LeftPanel){
      ChDisk.SetFlags(VMENU_LEFTMOST);
    }
    ChDisk.SetHelp("DriveDlg");
    /* $ 17.06.2001 KM
       ! ���������� WRAPMODE � ����.
    */
    ChDisk.SetFlags(VMENU_WRAPMODE);
    /* KM $ */

    char MenuText[NM];
    int DriveType,MenuLine;
    int LabelWidth=Max(11,(int)strlen(MSG(MChangeDriveLabelAbsent)));

    /* $ 02.04.2001 VVM
      ! ������� �� ������ ������ �����... */
    for (DiskMask=Mask,MenuLine=I=0;DiskMask!=0;DiskMask>>=1,I++)
    {
      if (DiskMask & 1)
      {
        sprintf(MenuText,"&%c: ",'A'+I);
        sprintf(RootDir,"%c:\\",'A'+I);
        DriveType = GetDriveType(RootDir);
        if (Opt.ChangeDriveMode & DRIVE_SHOW_TYPE)
        {
          static struct TypeMessage{
            int DrvType;
            int FarMsg;
          } DrTMsg[]={
            {DRIVE_REMOVABLE,MChangeDriveRemovable},
            {DRIVE_FIXED,MChangeDriveFixed},
            {DRIVE_REMOTE,MChangeDriveNetwork},
            {DRIVE_CDROM,MChangeDriveCDROM},
            {DRIVE_RAMDISK,MChangeDriveRAM},
          };
          for(J=0; J < sizeof(DrTMsg)/sizeof(DrTMsg[1]); ++J)
            if(DrTMsg[J].DrvType == DriveType)
            {
              strcpy(DiskType,MSG(DrTMsg[J].FarMsg));
              break;
            }

          if(J >= sizeof(DrTMsg)/sizeof(DrTMsg[1]))
          {
            sprintf(DiskType,"%*s",strlen(MSG(MChangeDriveFixed)),"");
          }
          /* 05.01.2001 SVS
             + ���������� ��� Subst-��� �����
          */
          {
            char LocalName[8],SubstName[NM];
            sprintf(LocalName,"%c:",*RootDir);
            if(GetSubstName(DriveType,LocalName,SubstName,sizeof(SubstName)))
            {
              strcpy(DiskType,MSG(MChangeDriveSUBST));
              DriveType=DRIVE_SUBSTITUTE;
            }
          }
          /* SVS $ */
          strcat(MenuText,DiskType);
        }

        int ShowDisk = (DriveType!=DRIVE_REMOVABLE || (Opt.ChangeDriveMode & DRIVE_SHOW_REMOVABLE)) &&
                       (DriveType!=DRIVE_CDROM || (Opt.ChangeDriveMode & DRIVE_SHOW_CDROM));
        if (Opt.ChangeDriveMode & (DRIVE_SHOW_LABEL|DRIVE_SHOW_FILESYSTEM))
        {
          char VolumeName[NM],FileSystemName[NM];
          *VolumeName=*FileSystemName=0;
          if (ShowDisk && !GetVolumeInformation(RootDir,VolumeName,sizeof(VolumeName),NULL,NULL,NULL,FileSystemName,sizeof(FileSystemName)))
          {
            strcpy(VolumeName,MSG(MChangeDriveLabelAbsent));
            ShowDisk=FALSE;
          }
          if (Opt.ChangeDriveMode & DRIVE_SHOW_LABEL)
          {
            /* $ 01.10.2001 IS ����� ������� � ����� */
            TruncStrFromEnd(VolumeName,LabelWidth);
            /* IS $ */
            sprintf(MenuText+strlen(MenuText),"%c%-*s",VerticalLine,LabelWidth,VolumeName);
          }
          if (Opt.ChangeDriveMode & DRIVE_SHOW_FILESYSTEM)
            sprintf(MenuText+strlen(MenuText),"%c%-8.8s",VerticalLine,FileSystemName);
        }

        if (Opt.ChangeDriveMode & DRIVE_SHOW_SIZE)
        {
          char TotalText[NM],FreeText[NM];
          *TotalText=*FreeText=0;
          int64 TotalSize,TotalFree,UserFree;
          if (ShowDisk && GetDiskSize(RootDir,&TotalSize,&TotalFree,&UserFree))
          {
            /* $ 10.05.2001 SVS
               ������ �������������� ������ ��� ���������� �������� ����� :-(
            */
            sprintf(TotalText,"%6d %1.1s",(TotalSize/(1024*1024)).PLow(),MSG(MChangeDriveMb));
//            FileSizeToStr(TotalText,TotalSize.PHigh(),TotalSize.PLow(),8,0,1);
            sprintf(FreeText,"%6d %1.1s",(UserFree/(1024*1024)).PLow(),MSG(MChangeDriveMb));
//            FileSizeToStr(FreeText,UserFree.PHigh(),UserFree.PLow(),8,0,1);
            /* SVS $ */
          }
          sprintf(MenuText+strlen(MenuText),"%c%-8s%c%-8s",VerticalLine,TotalText,VerticalLine,FreeText);
        }

        if (Opt.ChangeDriveMode & DRIVE_SHOW_NETNAME)
        {
          char RemoteName[NM];
          DriveLocalToRemoteName(DriveType,*RootDir,RemoteName);
          if(*RemoteName)
          {
            strcat(MenuText,"  ");
            strcat(MenuText,RemoteName);
          }
          ShowSpecial=TRUE;
        }

        if (FirstCall)
        {
          ChDiskItem.SetSelect(I==Pos);
          if(!SetSelected)
            SetSelected=(I==Pos);
        }
        else if(Pos < DiskCount)
        {
          ChDiskItem.SetSelect(MenuLine==Pos);
          if(!SetSelected)
            SetSelected=(MenuLine==Pos);
        }

        strncpy(ChDiskItem.Name,MenuText,sizeof(ChDiskItem.Name)-1);
        if (strlen(MenuText)>4)
          ShowSpecial=TRUE;

        UserData=MAKELONG(MAKEWORD('A'+I,0),DriveType);
        ChDisk.SetUserData((char*)UserData,sizeof(UserData),
                 ChDisk.AddItem(&ChDiskItem));
        MenuLine++;
      } // if (DiskMask & 1)
    } // for
    /* VVM $ */

    /* $ 21.01.2002 IS
       ������ ����������� �� ���������� ������� �������� � ���� ������(!)
    */
    /* $ 22.01.2002 IS
       ���������� ��������� ����� ��������
    */
    int PluginMenuItemsCount=0;
    if (Opt.ChangeDriveMode & DRIVE_SHOW_PLUGINS)
    {
      int UsedNumbers[10];
      memset(UsedNumbers,0,sizeof(UsedNumbers));
      TArray<ChDiskPluginItem> MPItems, MPItemsNoHotkey;
      ChDiskPluginItem OneItem;
      /* $ 15.03.2001 IS
           ������ �������������� �������, ��� ������, ����� ��������,
           ����������� ����� � ����, ������ 9.
      */
      char *AdditionalHotKey=MSG(MAdditionalHotKey);
      int AHKPos=0,                           // ������ � ������ �������
          AHKSize=strlen(AdditionalHotKey);   /* ��� �������������� ������ ��
                                                 ������� ������� */
      /* IS $ */

      int PluginNumber=0, PluginItem; // IS: �������� - �������� � ������� �������
      int PluginTextNumber, ItemPresent, HotKey, Done=FALSE;
      char PluginText[100];

      while(!Done)
      {
        for (PluginItem=0;;++PluginItem)
        {
          if (!CtrlObject->Plugins.GetDiskMenuItem(PluginNumber,PluginItem,
              ItemPresent,PluginTextNumber,PluginText,sizeof(PluginText)))
          {
            Done=TRUE;
            break;
          }
          if (!ItemPresent)
            break;

          if(!PluginTextNumber) // IS: ����������, �������� �����
            HotKey=-1; // "-1" -  ������� ����������
          else
          {
            if(PluginTextNumber<10) // IS: ����� ������ ����
            {
              // IS: ��������, � �� ����� �� ������
              // IS: ���� �����, �� ����� ������ ��� � ������ ������ - ����,
              // IS: � �� �� ����������
              if(UsedNumbers[PluginTextNumber])
              {
                PluginTextNumber=0;
                while (PluginTextNumber<10 && UsedNumbers[PluginTextNumber])
                  PluginTextNumber++;
              }
              UsedNumbers[PluginTextNumber%10]=1;
            }

            if(PluginTextNumber<10)
              HotKey=PluginTextNumber+'0';
            else if(AHKPos<AHKSize)
              HotKey=AdditionalHotKey[AHKPos];
            else
              HotKey=0;
          }

          /* $ 22.08.2002 IS
               ���������� �������������� ������, � �� ������ '#', ��� ������.
          */
          *MenuText=0;
          if(HotKey<0)
            strncpy(MenuText,ShowSpecial?PluginText:"",
              sizeof(MenuText)-1-4); // -4 - ������� ��� ������
          else if(PluginTextNumber<10)
          {
            sprintf(MenuText,"&%c: %s", HotKey,
              ShowSpecial ? PluginText:"");
          }
          else if(AHKPos<AHKSize)
          {
            sprintf(MenuText,"&%c: %s", HotKey,
              ShowSpecial ? PluginText:"");
            ++AHKPos;
          }
          else if(ShowSpecial) // IS: �� ��������� ������ ������!
          {
            HotKey=0;
            sprintf(MenuText,"   %s", PluginText);
          }
          /* IS $ */
          if(HotKey>-1 && *MenuText) // IS: �� ��������� ������ ������!
          {
            strncpy(OneItem.Item.Name,MenuText,sizeof(ChDiskItem.Name)-1);
            OneItem.Item.UserDataSize=0;
            OneItem.Item.UserData=(char*)MAKELONG(PluginNumber,PluginItem);
            OneItem.HotKey=HotKey;
            if(!MPItems.addItem(OneItem))
            {
              Done=TRUE;
              break;
            }
          }
          else if(HotKey<0) // IS: ���������� ����������� ������� �� �����
          {
            strncpy(OneItem.Item.Name,MenuText,sizeof(ChDiskItem.Name)-1);
            OneItem.Item.UserDataSize=0;
            OneItem.Item.UserData=(char*)MAKELONG(PluginNumber,PluginItem);
            OneItem.HotKey=HotKey;
            if(!MPItemsNoHotkey.addItem(OneItem))
            {
              Done=TRUE;
              break;
            }
          }
        } // END: for (PluginItem=0;;++PluginItem)

        ++PluginNumber;
      }

      // IS: ������ ���������� ���������� �����������
      PluginTextNumber=0;
      for(int i=0;;++i)
      {
        ChDiskPluginItem *item=MPItemsNoHotkey.getItem(i);
        if(item)
        {
          if(UsedNumbers[PluginTextNumber])
          {
            while (PluginTextNumber<10 && UsedNumbers[PluginTextNumber])
              PluginTextNumber++;
          }
          UsedNumbers[PluginTextNumber%10]=1;

          *MenuText=0;
          if(PluginTextNumber<10)
          {
            item->HotKey=PluginTextNumber+'0';
            sprintf(MenuText,"&%c: %s", item->HotKey, item->Item.Name);
          }
          else if(AHKPos<AHKSize)
          {
            item->HotKey=AdditionalHotKey[AHKPos];
            sprintf(MenuText,"&%c: %s", item->HotKey, item->Item.Name);
            ++AHKPos;
          }
          else if(ShowSpecial) // IS: �� ��������� ������ ������!
          {
            item->HotKey=0;
            sprintf(MenuText,"   %s", item->Item.Name);
          }

          strncpy(item->Item.Name, MenuText, sizeof(item->Item.Name)-1);
          if(*item->Item.Name && !MPItems.addItem(*item))
            break;
        }
        else
          break;
      }

      MPItems.Sort();
      MPItems.Pack(); // ������� �����
      PluginMenuItemsCount=MPItems.getSize();
      if(PluginMenuItemsCount)
      {
        memset(&ChDiskItem,0,sizeof(ChDiskItem));
        ChDiskItem.Flags|=LIF_SEPARATOR;
        ChDiskItem.UserDataSize=0;
        ChDisk.AddItem(&ChDiskItem);

        ChDiskItem.Flags&=~LIF_SEPARATOR;
        for (int I=0;I<PluginMenuItemsCount;++I)
        {
          if(Pos > DiskCount && !SetSelected)
          {
            MPItems.getItem(I)->Item.SetSelect(DiskCount+I+1==Pos);
            if(!SetSelected)
              SetSelected=DiskCount+I+1==Pos;
          }
          ChDisk.AddItem(&MPItems.getItem(I)->Item);
        }
      }
    }
    /* IS 22.08.2002 $ */
    /* IS 21.08.2002 $ */

    int X=X1+5;
    if (this==CtrlObject->Cp()->RightPanel && IsFullScreen() && X2-X1>40)
      X=(X2-X1+1)/2+5;
    int Y=(ScrY+1-(DiskCount+PluginMenuItemsCount+5))/2;
    if (Y<1) Y=1;
    ChDisk.SetPosition(X,Y,0,0);

    if (Y<3)
      ChDisk.SetBoxType(SHORT_DOUBLE_BOX);

    _tran(SysLog(" call ChDisk.Show"));
    ChDisk.Show();

    while (!ChDisk.Done())
    {
      //_D(SysLog("ExitCode=%i",ChDisk.GetExitCode()));
      int SelPos=ChDisk.GetSelectPos();
      int Key;
      {
        ChangeMacroMode MacroMode(MACRO_DISKS);
        Key=ChDisk.ReadInput();
      }
      switch(Key)
      {
        // Shift-Enter � ���� ������ ������ �������� ��������� ��� ������� �����
        case KEY_SHIFTENTER:
          if (SelPos<DiskCount)
          {
            if ((UserData=(DWORD)ChDisk.GetUserData(NULL,0)) != NULL)
            {
              char DosDeviceName[16];
              sprintf(DosDeviceName,"%c:\\",LOBYTE(LOWORD(UserData)));
              Execute(DosDeviceName,FALSE,TRUE,TRUE);
            }
          }
          break;
        case KEY_CTRLPGUP:  case KEY_CTRLNUMPAD9:
          if(Opt.PgUpChangeDisk)
            return -1;
          break;
        /* $ 27.04.2001 SVS
           �.�. ��� ������� �������� ��������� "����������" ����������,
           �� ������� ��������� Ins ��� CD - "������� ����"
        */
        case KEY_INS:       case KEY_NUMPAD0:
          if (SelPos<DiskCount)// && WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
          {
//            char MsgText[200], LocalName[50];
            if ((UserData=(DWORD)ChDisk.GetUserData(NULL,0)) != NULL)
            {
              DriveType=HIWORD(UserData);
              if(DriveType == DRIVE_CDROM /* || DriveType == DRIVE_REMOVABLE*/)
              {
                SaveScreen SvScrn;
                EjectVolume(LOBYTE(LOWORD(UserData)),EJECT_LOAD_MEDIA);
                return(SelPos);
              }
            }
          }
          break;
        /* SVS $ */
        case KEY_DEL:
          if (SelPos<DiskCount)
          {
            /* $ 28.12.2001 DJ
               ��������� Del �������� � ��������� �������
            */
            if ((UserData=(DWORD)ChDisk.GetUserData(NULL,0)) != NULL)
            {
              if(HIWORD(UserData) == DRIVE_REMOVABLE || HIWORD(UserData) == DRIVE_CDROM)
              {
                // ������ ������� ������ ����
                if(!EjectVolume(LOBYTE(LOWORD(UserData)),EJECT_NO_MESSAGE))
                {
                  // ���������� ��������� �������
                  int CMode=GetMode();
                  int AMode=CtrlObject->Cp()->GetAnotherPanel (this)->GetMode();
                  char TmpCDir[NM], TmpADir[NM];
                  GetCurDir (TmpCDir);
                  CtrlObject->Cp()->GetAnotherPanel (this)->GetCurDir (TmpADir);

                  // �������� ����, ����� ���� � ����������� ���� ����� ����
                  // (���� ���� ������� ������ ������)
                  ChDisk.Hide();
                  ChDisk.LockRefresh(); // ... � �������� �� �����������.

                  // "���� �� �������������"
                  int DoneEject=FALSE;
                  while(!DoneEject)
                  {
                    // "��������� ����" - �������� ��� ������������� � �������� �������
                    // TODO: � ���� �������� ������� - CD? ;-)
                    IfGoHome(LOBYTE(LOWORD(UserData)));
                    // ��������� ������� ���������� ��� ������ ���������
                    int ResEject=EjectVolume(LOBYTE(LOWORD(UserData)),EJECT_NO_MESSAGE);
                    if(!ResEject)
                    {
                      // ����������� ���� - ��� ������� ��� �� ����� ������ � ������.
                      if (AMode != PLUGIN_PANEL)
                        CtrlObject->Cp()->GetAnotherPanel (this)->SetCurDir (TmpADir, FALSE);
                      if (CMode != PLUGIN_PANEL)
                        SetCurDir (TmpCDir, FALSE);

                      // ... � ������� ����� �...
                      char MsgText[200];
                      sprintf(MsgText,MSG(MChangeCouldNotEjectMedia),LOBYTE(LOWORD(UserData)));
                      SetLastError(ERROR_DRIVE_LOCKED); // ...� "The disk is in use or locked by another process."
                      DoneEject=Message(MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),MsgText,MSG(MRetry),MSG(MCancel))!=0;
                    }
                    else
                      DoneEject=TRUE;
                  }

                  // "��������" ������ ������ ������
                  ChDisk.UnlockRefresh();
                  ChDisk.Show();
                }
              }
              else
              {
                int Code = ProcessDelDisk (LOBYTE(LOWORD(UserData)), HIWORD(UserData), &ChDisk);
                if (Code != DRIVE_DEL_FAIL)
                {
                  // "BugZ#640 - ���������" - ������� �����.
                  ScrBuf.Lock(); // �������� ������ ����������
                  FrameManager->ResizeAllFrame();
                  FrameManager->PluginCommit(); // ��������.
                  ScrBuf.Unlock(); // ��������� ����������

                  // 19.01.2002 VVM  + ���� ���� ��� ��������� - � ����� � ���������
                  return (((DiskCount-SelPos)==1) && (SelPos > 0) && (Code != DRIVE_DEL_EJECT))?SelPos-1:SelPos;
                }
              }
            }
            /* DJ $ */
          }
          break;
        case KEY_CTRL1:
        case KEY_RCTRL1:
          Opt.ChangeDriveMode^=DRIVE_SHOW_TYPE;
          return(SelPos);
        case KEY_CTRL2:
        case KEY_RCTRL2:
          Opt.ChangeDriveMode^=DRIVE_SHOW_NETNAME;
          return(SelPos);
        case KEY_CTRL3:
        case KEY_RCTRL3:
          Opt.ChangeDriveMode^=DRIVE_SHOW_LABEL;
          return(SelPos);
        case KEY_CTRL4:
        case KEY_RCTRL4:
          Opt.ChangeDriveMode^=DRIVE_SHOW_FILESYSTEM;
          return(SelPos);
        case KEY_CTRL5:
        case KEY_RCTRL5:
          Opt.ChangeDriveMode^=DRIVE_SHOW_SIZE;
          return(SelPos);
        case KEY_CTRL6:
        case KEY_RCTRL6:
          Opt.ChangeDriveMode^=DRIVE_SHOW_REMOVABLE;
          return(SelPos);
        case KEY_CTRL7:
        case KEY_RCTRL7:
          Opt.ChangeDriveMode^=DRIVE_SHOW_PLUGINS;
          return(SelPos);
        case KEY_CTRL8:
        case KEY_RCTRL8:
          Opt.ChangeDriveMode^=DRIVE_SHOW_CDROM;
          return(SelPos);
        /* $ 27.03.2001 SVS
          Shift-F1 �� ������ ������� � ���� ������ ������ ���� ������� ����...
        */
        case KEY_SHIFTF1:
          if (SelPos>DiskCount)
          {
            // �������� ������ �����, ������� �������� � CommandsMenu()
            if ((UserData=(DWORD)ChDisk.GetUserData(NULL,0)) != NULL)
              FarShowHelp(CtrlObject->Plugins.PluginsData[LOWORD(UserData)].ModuleName,
                    NULL,FHELP_SELFHELP|FHELP_NOSHOWERROR|FHELP_USECONTENTS);
          }
          break;
        /* SVS $ */
        case KEY_CTRLR:
          return(SelPos);

        /* $ 21.06.2001 tran
           ���� �������...
           ����� ������� � ������� ������� ��������*/
        case KEY_F1:
          {
//            SaveScreen s;
            ChDisk.ProcessInput();
          }
          break;
        /* tran 21.06.2001 $ */
        default:
          ChDisk.ProcessInput();
          break;
      }
      /* $ 05.09.2000 SVS
        Bug#12 -   ��� �������� �������� ����� �� Del � ������ �� ����
               ��� ���������� ���������� ��������� ����. ���� �� ������.
               �� ctrl-r ��������� �� ���������.
               ����� �����, ���� �� �� ���� �������� �� ���� ���� ������
               ������� ����
      */
      /* $ 06.09.2000 tran
         ����� ���, ������ ���� �����:
         1. strncpy �� ���������� 0 � ����� ������
         2. GetDriveType ���������� ���������, ��� ������ ����.
      */
      /* $ 07.09.2000 SVS
         ��� ���� ���������� (� ������ AT):
             ��� �����, �� ���� ����� �� ����, ���� � ��� ������� ���� - UNC
             "\\server\share\"
      */
      /* $ 30.04.2001 DJ
         � ��� ���� ����������: �� ���� ����� �� ����, ���� ��� �������
         �� quick view panel (� ��� CurDir ������)
      */
      if (ChDisk.Done() && ChDisk.Modal::GetExitCode()<0 && *CurDir && strncmp(CurDir,"\\\\",2)!=0)
      {
        char RootDir[10];
        strncpy(RootDir,CurDir,3);
        RootDir[3]=0;
        if (GetDriveType(RootDir)==DRIVE_NO_ROOT_DIR)
          ChDisk.ClearDone();
      }
      /* DJ $ */
      /* SVS $ */
      /* tran $ */
      /* SVS $ */
    } // while (!Done)
    if (ChDisk.Modal::GetExitCode()<0)
      return(-1);
    {
      UserDataSize=ChDisk.Modal::GetExitCode()>DiskCount?2:3;
      UserData=(DWORD)ChDisk.GetUserData(NULL,0);
    }
  }

  if(Opt.CloseCDGate && UserData != NULL && HIWORD(UserData) == DRIVE_CDROM && UserDataSize == 3)
  {
    sprintf(RootDir,"%c:",LOBYTE(LOWORD(UserData)));
    if(!IsDiskInDrive(RootDir))
    {
      if(!EjectVolume(LOBYTE(LOWORD(UserData)),EJECT_READY|EJECT_NO_MESSAGE))
      {
        SaveScreen SvScrn;
        Message(0,0,"",MSG(MChangeWaitingLoadDisk));
        EjectVolume(LOBYTE(LOWORD(UserData)),EJECT_LOAD_MEDIA|EJECT_NO_MESSAGE);
      }
    }
  }

  if (ProcessPluginEvent(FE_CLOSE,NULL))
    return(-1);

  ScrBuf.Flush();
  INPUT_RECORD rec;
  PeekInputRecord(&rec);

  if (UserDataSize==3)
  {
    while (1)
    {
      int NumDisk=LOBYTE(LOWORD(UserData))-'A';
      char MsgStr[NM],NewDir[NM];
      sprintf(NewDir,"%c:",LOBYTE(LOWORD(UserData)));
      FarChDir(NewDir);
      CtrlObject->CmdLine->GetCurDir(NewDir);
      if (toupper(*NewDir)==LOBYTE(LOWORD(UserData)))
        FarChDir(NewDir);
      if (getdisk()!=NumDisk)
      {
        char RootDir[NM];
        sprintf(RootDir,"%c:\\",LOBYTE(LOWORD(UserData)));
        FarChDir(RootDir);
        if (getdisk()==NumDisk)
          break;
      }
      else
        break;
      sprintf(MsgStr,MSG(MChangeDriveCannotReadDisk),LOBYTE(LOWORD(UserData)));
      if (Message(MSG_WARNING,2,MSG(MError),MsgStr,MSG(MRetry),MSG(MCancel))!=0)
        return(-1);
    }
    char NewCurDir[NM];
    FarGetCurDir(sizeof(NewCurDir),NewCurDir);
    // BugZ#208. ���� ���� ���������, �� ������ �� ������.
    //_tran(SysLog("PanelMode=%i (%i), CurDir=[%s], NewCurDir=[%s]",PanelMode,GetType(),
    //            CurDir,NewCurDir);)

    if(PanelMode == NORMAL_PANEL && GetType()==FILE_PANEL && !LocalStricmp(CurDir,NewCurDir) && IsVisible())
    {
      // � ����� �� ������ ����� Update????
      Update(UPDATE_KEEP_SELECTION);
    }
    else
    {
      Focus=GetFocus();
      Panel *NewPanel=CtrlObject->Cp()->ChangePanel(this,FILE_PANEL,TRUE,FALSE);
      NewPanel->SetCurDir(NewCurDir,TRUE);
      NewPanel->Show();
      if (Focus || !CtrlObject->Cp()->GetAnotherPanel(this)->IsVisible())
        NewPanel->SetFocus();
    }
  }
  else
    if (UserDataSize==2)
    {
      HANDLE hPlugin=CtrlObject->Plugins.OpenPlugin(LOWORD(UserData),OPEN_DISKMENU,HIWORD(UserData));
      if (hPlugin!=INVALID_HANDLE_VALUE)
      {
        Focus=GetFocus();
        Panel *NewPanel=CtrlObject->Cp()->ChangePanel(this,FILE_PANEL,TRUE,TRUE);
        NewPanel->SetPluginMode(hPlugin,"");
        NewPanel->Update(0);
        NewPanel->Show();
        if (Focus || !CtrlObject->Cp()->GetAnotherPanel(NewPanel)->IsVisible())
          NewPanel->SetFocus();
      }
    }
  return(-1);
}

/* $ 28.12.2001 DJ
   ��������� Del � ���� ������
*/

int Panel::ProcessDelDisk (char Drive, int DriveType,VMenu *ChDiskMenu)
{
  char MsgText[200];
  int UpdateProfile=CONNECT_UPDATE_PROFILE;
  BOOL Processed=FALSE;

  char DiskLetter [3];
  DiskLetter[0] = Drive;
  DiskLetter[1] = ':';
  DiskLetter[2] = 0;

  if(DriveType == DRIVE_REMOTE && MessageRemoveConnection(Drive,UpdateProfile))
    Processed=TRUE;

  // <�������>
  if(Processed)
  {
    LockScreen LckScr;
    // ���� �� ��������� �� ��������� ����� - ������ � ����, ����� �� ������
    // ��������
    IfGoHome(Drive);
    FrameManager->ResizeAllFrame();
    FrameManager->GetCurrentFrame()->Show();
    ChDiskMenu->Show();
  }
  // </�������>

  /* $ 05.01.2001 SVS
     ������� ������� SUBST-�����.
  */
  if(DriveType == DRIVE_SUBSTITUTE)
  {
    if(!DelSubstDrive(DiskLetter))
      return DRIVE_DEL_SUCCESS;
    else
    {
      int LastError=GetLastError();
      sprintf(MsgText,MSG(MChangeDriveCannotDelSubst),DiskLetter);
      if (LastError==ERROR_OPEN_FILES || LastError==ERROR_DEVICE_IN_USE)
        if (Message(MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),MsgText,
                "\x1",MSG(MChangeDriveOpenFiles),
                MSG(MChangeDriveAskDisconnect),MSG(MOk),MSG(MCancel))==0)
        {
          if(!DelSubstDrive(DiskLetter))
            return DRIVE_DEL_SUCCESS;
        }
        else
          return DRIVE_DEL_FAIL;
      Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),MsgText,MSG(MOk));
    }
    return DRIVE_DEL_FAIL; // ����. � ������� ��� ����� ��� ��� ����...
  }
  /* SVS $ */

  if(Processed)
  {
    if (WNetCancelConnection2(DiskLetter,UpdateProfile,FALSE)==NO_ERROR)
      return DRIVE_DEL_SUCCESS;
    else
    {
      int LastError=GetLastError();
      sprintf(MsgText,MSG(MChangeDriveCannotDisconnect),DiskLetter);
      if (LastError==ERROR_OPEN_FILES || LastError==ERROR_DEVICE_IN_USE)
        if (Message(MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),MsgText,
                "\x1",MSG(MChangeDriveOpenFiles),
                MSG(MChangeDriveAskDisconnect),MSG(MOk),MSG(MCancel))==0)
        {
          if (WNetCancelConnection2(DiskLetter,UpdateProfile,TRUE)==NO_ERROR)
            return DRIVE_DEL_SUCCESS;
        }
        else
          return DRIVE_DEL_FAIL;
      char RootDir[50];
      sprintf(RootDir,"%c:\\",*DiskLetter);
      if (GetDriveType(RootDir)==DRIVE_REMOTE)
        Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),MsgText,MSG(MOk));
    }
    return DRIVE_DEL_FAIL;
  }
  return DRIVE_DEL_FAIL;
}
/* DJ $ */

// ������������� ����
static DWORD _CorrectFastFindKbdLayout(INPUT_RECORD *rec,DWORD Key)
{
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT && (Key&KEY_ALT))// && Key!=(KEY_ALT|0x3C))
  {
    // // _SVS(SysLog("_CorrectFastFindKbdLayout>>> %s | %s",_FARKEY_ToName(Key),_INPUT_RECORD_Dump(rec)));
    if(rec->Event.KeyEvent.uChar.AsciiChar && (Key&KEY_MASKF) != rec->Event.KeyEvent.uChar.AsciiChar) //???
      Key=(Key&0xFFFFFF00)|rec->Event.KeyEvent.uChar.AsciiChar;   //???
    // // _SVS(SysLog("_CorrectFastFindKbdLayout<<< %s | %s",_FARKEY_ToName(Key),_INPUT_RECORD_Dump(rec)));
  }
  return Key;
}

void Panel::FastFind(int FirstKey)
{
  // // _SVS(CleverSysLog Clev("Panel::FastFind"));
  INPUT_RECORD rec;
  char LastName[NM],Name[NM];
  int Key,KeyToProcess=0;
  *LastName=0;
  WaitInFastFind++;
  {
    int FindX=Min(X1+9,ScrX-22);
    int FindY=Min(Y2,ScrY-2);
    ChangeMacroMode MacroMode(MACRO_SEARCH);
    SaveScreen SaveScr(FindX,FindY,FindX+21,FindY+2);
    FastFindShow(FindX,FindY);
    Edit FindEdit;
    FindEdit.SetPosition(FindX+2,FindY+1,FindX+19,FindY+1);
    FindEdit.SetEditBeyondEnd(FALSE);
    FindEdit.SetObjectColor(COL_DIALOGEDIT);
    FindEdit.Show();

    while (!KeyToProcess)
    {
      if (FirstKey)
      {
        FirstKey=_CorrectFastFindKbdLayout(FrameManager->GetLastInputRecord(),FirstKey);
        // // _SVS(SysLog("Panel::FastFind  FirstKey=%s  %s",_FARKEY_ToName(FirstKey),_INPUT_RECORD_Dump(FrameManager->GetLastInputRecord())));
        // // _SVS(SysLog("if (FirstKey)"));
        Key=FirstKey;
      }
      else
      {
        // // _SVS(SysLog("else if (FirstKey)"));
        Key=GetInputRecord(&rec);
        if (rec.EventType==MOUSE_EVENT)
        {
          if ((rec.Event.MouseEvent.dwButtonState & 3)==0)
            continue;
          else
            Key=KEY_ESC;
        }
        else if (rec.EventType==KEY_EVENT)
        {
          // ��� ������� ������������� ������������...
          if(Key==KEY_CTRLV || Key==KEY_SHIFTINS || Key==KEY_SHIFTNUMPAD0)
          {
            char *ClipText=PasteFromClipboard();
            if(ClipText && *ClipText)
            {
              if(strlen(ClipText) <= NM*2) // ������� �������� ����������� �� ������...
              {
                char *Ptr=(char *)xf_malloc(strlen(ClipText)*2+8);
                if(Ptr)
                {
                  // ... �������������� �������� ������������������ �� "�����<space>"
                  char *PtrClipText=ClipText, *PtrPtr=Ptr;
                  while(*PtrClipText)
                  {
                    *PtrPtr++=*PtrClipText;
                    *PtrPtr++=' ';
                    ++PtrClipText;
                  }
                  *PtrPtr=0;
                  CtrlObject->Macro.PostNewMacro(Ptr,0);
                  xf_free(Ptr);
                }
              }
              delete[] ClipText;
            }
            continue;
          }
          Key=_CorrectFastFindKbdLayout(&rec,Key);
        }
      }
      if (Key==KEY_ESC || Key==KEY_F10)
      {
        KeyToProcess=KEY_NONE;
        break;
      }
      // // _SVS(if (!FirstKey) SysLog("Panel::FastFind  Key=%s  %s",_FARKEY_ToName(Key),_INPUT_RECORD_Dump(&rec)));
      if (Key>=KEY_ALT_BASE+0x01 && Key<=KEY_ALT_BASE+255)
        Key=tolower(Key-KEY_ALT_BASE);
      if (Key>=KEY_ALTSHIFT_BASE+0x01 && Key<=KEY_ALTSHIFT_BASE+255)
        Key=tolower(Key-KEY_ALTSHIFT_BASE);

      if (Key==KEY_MULTIPLY)
        Key='*';

      switch (Key)
      {
        case KEY_F1:
        {
          FindEdit.Hide();
          SaveScr.RestoreArea();
          {
            Help Hlp ("FastFind");
          }
          FindEdit.Show();
          FastFindShow(FindX,FindY);
          break;
        }
        case KEY_CTRLENTER:
          FindPartName(Name,TRUE);
          FindEdit.Show();
          FastFindShow(FindX,FindY);
          break;
        case KEY_NONE:
        case KEY_IDLE:
          break;
        default:
          if ((Key<32 || Key>=256) && Key!=KEY_BS && Key!=KEY_CTRLY &&
              Key!=KEY_CTRLBS && Key!=KEY_ALT && Key!=KEY_SHIFT &&
              Key!=KEY_CTRL && Key!=KEY_RALT && Key!=KEY_RCTRL &&
              !(Key==KEY_CTRLINS||Key==KEY_CTRLNUMPAD0) && !(Key==KEY_SHIFTINS||Key==KEY_SHIFTNUMPAD0))
          {
            KeyToProcess=Key;
            break;
          }

          if (FindEdit.ProcessKey(Key))
          {
            FindEdit.GetString(Name,sizeof(Name));

            // ������ ������� '**'
            int LenName=strlen(Name);
            if(LenName > 1 && Name[LenName-1] == '*' && Name[LenName-2] == '*')
            {
              Name[LenName-1]=0;
              FindEdit.SetString(Name);
            }
            /* $ 09.04.2001 SVS
               �������� � ������� �������.
               ��������� � 00573.ChangeDirCrash.txt
            */
            if(*Name == '"')
            {
              memmove(Name,Name+1,sizeof(Name)-1);
              Name[strlen(Name)-1]=0;
              FindEdit.SetString(Name);
            }
            /* SVS $ */
            if (FindPartName(Name,FALSE))
              strcpy(LastName,Name);
            else
            {
              if(CtrlObject->Macro.IsExecuting()) // ���� ������� ��������...
                CtrlObject->Macro.DropProcess(); // ... �� ������� ������������
              FindEdit.SetString(LastName);
              strcpy(Name,LastName);
            }
            FindEdit.Show();
            FastFindShow(FindX,FindY);
          }
          break;
      }
      FirstKey=0;
    }
  }
  WaitInFastFind--;
  Show();
  CtrlObject->MainKeyBar->Redraw();
  ScrBuf.Flush();
  Panel *ActivePanel=CtrlObject->Cp()->ActivePanel;
  if (KeyToProcess==KEY_ENTER && ActivePanel->GetType()==TREE_PANEL)
    ((TreeList *)ActivePanel)->ProcessEnter();
  else
    CtrlObject->Cp()->ProcessKey(KeyToProcess);
}


void Panel::FastFindShow(int FindX,int FindY)
{
  SetColor(COL_DIALOGTEXT);
  GotoXY(FindX+1,FindY+1);
  Text(" ");
  GotoXY(FindX+20,FindY+1);
  Text(" ");
  Box(FindX,FindY,FindX+21,FindY+2,COL_DIALOGBOX,DOUBLE_BOX);
  GotoXY(FindX+7,FindY);
  SetColor(COL_DIALOGBOXTITLE);
  Text(MSearchFileTitle);
}


void Panel::SetFocus()
{
  if (CtrlObject->Cp()->ActivePanel!=this)
  {
    CtrlObject->Cp()->ActivePanel->KillFocus();
    CtrlObject->Cp()->ActivePanel=this;
  }

  if (!GetFocus())
  {
    CtrlObject->Cp()->RedrawKeyBar();
    Focus=TRUE;
    Redraw();
    FarChDir(CurDir);
  }
}


void Panel::KillFocus()
{
  Focus=FALSE;
  Redraw();
}


int  Panel::PanelProcessMouse(MOUSE_EVENT_RECORD *MouseEvent,int &RetCode)
{
  RetCode=TRUE;
  if (!ModalMode && MouseEvent->dwMousePosition.Y==0)
    if (MouseEvent->dwMousePosition.X==ScrX)
    {
      if (Opt.ScreenSaver && (MouseEvent->dwButtonState & 3)==0)
      {
        EndDrag();
        ScreenSaver(TRUE);
        return(TRUE);
      }
    }
    else
      if ((MouseEvent->dwButtonState & 3)!=0 && MouseEvent->dwEventFlags==0)
      {
        EndDrag();
        if (MouseEvent->dwMousePosition.X==0)
          CtrlObject->Cp()->ProcessKey(KEY_CTRLO);
        else
          ShellOptions(0,MouseEvent);
        return(TRUE);
      }

  if (!IsVisible() ||
      (MouseEvent->dwMousePosition.X<X1 || MouseEvent->dwMousePosition.X>X2 ||
      MouseEvent->dwMousePosition.Y<Y1 || MouseEvent->dwMousePosition.Y>Y2))
  {
    RetCode=FALSE;
    return(TRUE);
  }

  if (DragX!=-1)
  {
    if ((MouseEvent->dwButtonState & 3)==0)
    {
      EndDrag();
      if (MouseEvent->dwEventFlags==0 && SrcDragPanel!=this)
      {
        MoveToMouse(MouseEvent);
        Redraw();
        SrcDragPanel->ProcessKey(DragMove ? KEY_DRAGMOVE:KEY_DRAGCOPY);
      }
      return(TRUE);
    }
    if (MouseEvent->dwMousePosition.Y<=Y1 || MouseEvent->dwMousePosition.Y>=Y2 ||
        !CtrlObject->Cp()->GetAnotherPanel(SrcDragPanel)->IsVisible())
    {
      EndDrag();
      return(TRUE);
    }
    if ((MouseEvent->dwButtonState & 2) && MouseEvent->dwEventFlags==0)
      DragMove=!DragMove;
    if (MouseEvent->dwButtonState & 1)
      if ((abs(MouseEvent->dwMousePosition.X-DragX)>15 || SrcDragPanel!=this) &&
          !ModalMode)
      {
        if (SrcDragPanel->GetSelCount()==1 && DragSaveScr==NULL)
        {
          SrcDragPanel->GoToFile(DragName);
          SrcDragPanel->Show();
        }
        DragMessage(MouseEvent->dwMousePosition.X,MouseEvent->dwMousePosition.Y,DragMove);
        return(TRUE);
      }
      else
      {
        delete DragSaveScr;
        DragSaveScr=NULL;
      }
  }

  if ((MouseEvent->dwButtonState & 3)==0)
    return(TRUE);
  if ((MouseEvent->dwButtonState & 1) && MouseEvent->dwEventFlags==0 &&
      X2-X1<ScrX)
  {
    int FileAttr;
    MoveToMouse(MouseEvent);
    GetSelName(NULL,FileAttr);
    if (GetSelName(DragName,FileAttr) && !TestParentFolderName(DragName))
    {
      SrcDragPanel=this;
      DragX=MouseEvent->dwMousePosition.X;
      DragY=MouseEvent->dwMousePosition.Y;
      DragMove=ShiftPressed;
    }
  }
  return(FALSE);
}


int  Panel::IsDragging()
{
  return(DragSaveScr!=NULL);
}


void Panel::EndDrag()
{
  delete DragSaveScr;
  DragSaveScr=NULL;
  DragX=DragY=-1;
}


void Panel::DragMessage(int X,int Y,int Move)
{
  char DragMsg[NM],SelName[NM];
  int SelCount,MsgX,Length;

  if ((SelCount=SrcDragPanel->GetSelCount())==0)
    return;

  if (SelCount==1)
  {
    char CvtName[NM];
    int FileAttr;
    SrcDragPanel->GetSelName(NULL,FileAttr);
    SrcDragPanel->GetSelName(SelName,FileAttr);
    strcpy(CvtName,PointToName(SelName));
    QuoteSpace(CvtName);
    strcpy(SelName,CvtName);
  }
  else
    sprintf(SelName,MSG(MDragFiles),SelCount);

  if (Move)
    sprintf(DragMsg,MSG(MDragMove),SelName);
  else
    sprintf(DragMsg,MSG(MDragCopy),SelName);
  if ((Length=strlen(DragMsg))+X>ScrX)
  {
    MsgX=ScrX-Length;
    if (MsgX<0)
    {
      MsgX=0;
      /* $ 01.10.2001 IS ������� � �����, ����� �������� ���.�������� */
      Length=strlen(TruncStrFromEnd(DragMsg,ScrX));
      /* IS $ */
    }
  }
  else
    MsgX=X;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  delete DragSaveScr;
  DragSaveScr=new SaveScreen(MsgX,Y,MsgX+Length-1,Y);
  GotoXY(MsgX,Y);
  SetColor(COL_PANELDRAGTEXT);
  Text(DragMsg);
}


void Panel::GetCurDir(char *CurDir)
{
  strcpy(CurDir,Panel::CurDir);
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif
void Panel::SetCurDir(char *CurDir,int ClosePlugin)
{
  PrepareDiskPath(strcpy(Panel::CurDir,CurDir),sizeof(Panel::CurDir)-1);
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


void Panel::InitCurDir(char *CurDir)
{
  PrepareDiskPath(strcpy(Panel::CurDir,CurDir),sizeof(Panel::CurDir)-1);
}


/* $ 14.06.2001 KM
   + ��������� ��������� ���������� ���������, ������������
     ������� ���������� ������ ��� ��� ��������, ��� � ���
     ��������� ������. ��� ���������� ���������� �����������
     �� FAR.
*/
/* $ 05.10.2001 SVS
   ! ������� ��� ������ �������� ������ �������� ��� ��������� ������,
     � �� �����...
     � �� ����� �����-�� ����������...
*/
/* $ 14.01.2002 IS
   ! ����� ��������� ���������� ���������, ������ ��� ��� ������������
     � FarChDir, ������� ������ ������������ � ��� ��� ������������
     �������� ��������.
*/
int  Panel::SetCurPath()
{
  if (GetMode()==PLUGIN_PANEL)
    return TRUE;

  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  if (AnotherPanel->GetType()!=PLUGIN_PANEL)
  {
    if (isalpha(AnotherPanel->CurDir[0]) && AnotherPanel->CurDir[1]==':' &&
        toupper(AnotherPanel->CurDir[0])!=toupper(CurDir[0]))
    {
      // ������� ��������� ���������� ��������� ��� ��������� ������
      // (��� �������� ����� ����, ����� ������ ��� ��������� �������
      // �� ������������)
      FarChDir(AnotherPanel->CurDir,FALSE);
    }
  }

  if (!FarChDir(CurDir) || GetFileAttributes(CurDir)==0xFFFFFFFF)
  {
   // ����� �� ����� :-)
#if 1
    while(!FarChDir(CurDir))
    {
      BOOL IsChangeDisk=FALSE;
      char Root[1024];
      GetPathRoot(CurDir,Root);
      if(GetDriveType(Root) == DRIVE_REMOVABLE && !IsDiskInDrive(Root))
        IsChangeDisk=TRUE;
      else
      {
        int Result=CheckFolder(CurDir);
        if(Result == CHKFLD_NOTACCESS)
        {
          if(FarChDir(Root))
            SetCurDir(Root,TRUE);
          else
            IsChangeDisk=TRUE;
        }
        else if(Result == CHKFLD_NOTFOUND)
        {
          if(CheckShortcutFolder(CurDir,sizeof(CurDir)-1,FALSE,TRUE))
          {
            if(FarChDir(CurDir))
              SetCurDir(CurDir,TRUE);
            else
              IsChangeDisk=TRUE;
          }
          else
            IsChangeDisk=TRUE;
        }
      }
      if(IsChangeDisk)
        ChangeDisk();
    }
#else
    do{
      BOOL IsChangeDisk=FALSE;
      char Root[1024];
      GetPathRoot(CurDir,Root);
      if(GetDriveType(Root) == DRIVE_REMOVABLE && !IsDiskInDrive(Root))
        IsChangeDisk=TRUE;
      else if(CheckFolder(CurDir) == CHKFLD_NOTACCESS)
      {
        if(FarChDir(Root))
          SetCurDir(Root,TRUE);
        else
          IsChangeDisk=TRUE;
      }
      if(IsChangeDisk)
        ChangeDisk();
    } while(!FarChDir(CurDir));
#endif
    return FALSE;
  }
  return TRUE;
}
/* IS $ */
/* SVS $ */
/* KM $ */


void Panel::Hide()
{
  ScreenObject::Hide();
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  if (AnotherPanel->IsVisible())
  {
    if (AnotherPanel->GetFocus())
      if (AnotherPanel->GetType()==FILE_PANEL && AnotherPanel->IsFullScreen() ||
          GetType()==FILE_PANEL && IsFullScreen())
        AnotherPanel->Show();
  }
}


void Panel::Show()
{
  /* $ 09.05.2001 OT */
//  SavePrevScreen();
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  if (AnotherPanel->IsVisible() && !GetModalMode())
  {
  /* $ 09.05.2001 OT */
    if (SaveScr) {
      SaveScr->AppendArea(AnotherPanel->SaveScr);
    }
    if (AnotherPanel->GetFocus())
    {
      if (AnotherPanel->IsFullScreen())
      {
        SetVisible(TRUE);
        return;
      }
      if (GetType()==FILE_PANEL && IsFullScreen())
      {
        ScreenObject::Show();
        AnotherPanel->Show();
        return;
      }
    }
  }
  ScreenObject::Show();
  /* $ 03.10.2001 IS ���������� ������� ���� */
    if (Opt.ShowMenuBar)
      CtrlObject->TopMenuBar->Show();
  /* IS $ */
  ShowScreensCount();
}


void Panel::DrawSeparator(int Y)
{
  if (Y<Y2)
  {
    SetColor(COL_PANELBOX);
    GotoXY(X1,Y);
    ShowSeparator(X2-X1+1,1);
  }
}


void Panel::ShowScreensCount()
{
  if (Opt.ShowScreensNumber && X1==0)
  {
    /* $ 19.05.2001 DJ
       ���������� ��� ������ ��������
    */
    int Viewers=FrameManager->GetFrameCountByType (MODALTYPE_VIEWER);
    int Editors=FrameManager->GetFrameCountByType (MODALTYPE_EDITOR);
    int Dialogs=FrameManager->GetFrameCountByType (MODALTYPE_DIALOG);
    if (Viewers>0 || Editors>0 || Dialogs > 0)
    {
      char ScreensText[100];
      sprintf(ScreensText,"[%d", Viewers);
      if (Editors > 0)
        sprintf (ScreensText+strlen (ScreensText), "+%d", Editors);
      if (Dialogs > 0)
        sprintf (ScreensText+strlen (ScreensText), ",%d", Dialogs);
      strcat (ScreensText, "]");
      GotoXY(Opt.ShowColumnTitles ? X1:X1+2,Y1);
      SetColor(COL_PANELSCREENSNUMBER);
      Text(ScreensText);
    }
    /* DJ $ */
  }
}


void Panel::SetTitle()
{
  if (GetFocus())
  {
    char TitleDir[NM];
    if (*CurDir)
      sprintf(TitleDir,"{%s}",CurDir);
    else
    {
      char CmdText[512];
      /* $ 21.07.2000 IG
         Bug 21 (��������� ����� Ctrl-Q, Tab, F3, Esc ��� ������)
      */
      CtrlObject->CmdLine->GetCurDir(CmdText);
      /* IG $*/
      sprintf(TitleDir,"{%s}",CmdText);
    }
    strcpy(LastFarTitle,TitleDir);
    SetFarTitle(TitleDir);
  }
}


void Panel::SetPluginCommand(int Command,void *Param)
{
  ProcessingPluginCommand++;
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(this);
  PluginCommand=Command;
  switch(Command)
  {
    case FCTL_SETVIEWMODE:
    case FCTL_SETANOTHERVIEWMODE:
      if (Param!=NULL)
      {
        int Mode=*(int *)Param;
        if (Mode>=0 && Mode<=9)
        {
          Panel *DestPanel=(Command==FCTL_SETVIEWMODE) ? this:AnotherPanel;
          if (DestPanel!=NULL)
            DestPanel->SetViewMode(Mode);
        }
      }
      break;
    case FCTL_SETSORTMODE:
    case FCTL_SETANOTHERSORTMODE:
      if (Param!=NULL)
      {
        int Mode=*(int *)Param;
        if ((Mode>SM_DEFAULT) && (Mode<=SM_NUMLINKS))
        {
          Panel *DestPanel=(Command==FCTL_SETSORTMODE) ? this:AnotherPanel;
          if (DestPanel!=NULL)
          // �������� �� 1 ��-�� SM_DEFAULT
            DestPanel->SetSortMode(--Mode);
        }
      }
      break;
    case FCTL_SETSORTORDER:
    case FCTL_SETANOTHERSORTORDER:
      if (Param!=NULL)
      {
        /* $ 22.01.2001 VVM
           - ������� ���������� �������� ���������� StartSortOrder */
        int Order = (*(int *)Param) ? -1:1;
        /* VVM $ */
        Panel *DestPanel=(Command==FCTL_SETSORTORDER) ? this:AnotherPanel;
        if (DestPanel!=NULL)
        /* $ 24.04.2001 VVVM
           ������������ ������� ChangeSortOrder() */
          DestPanel->ChangeSortOrder(Order);
        /* VVM $ */
      }
      break;
    case FCTL_CLOSEPLUGIN:
      strncpy((char *)PluginParam,NullToEmpty((char *)Param),sizeof(PluginParam)-1);
      //if(Opt.CPAJHefuayor)
      //  CtrlObject->Plugins.ProcessCommandLine((char *)PluginParam);
      break;
    case FCTL_GETPANELINFO:
    case FCTL_GETANOTHERPANELINFO:
    case FCTL_GETPANELSHORTINFO:
    case FCTL_GETANOTHERPANELSHORTINFO:
    {
      if(Param == NULL || IsBadWritePtr(Param,sizeof(struct PanelInfo)))
        break;
      struct PanelInfo *Info=(struct PanelInfo *)Param;
      memset(Info,0,sizeof(*Info));
      Panel *DestPanel=(Command == FCTL_GETPANELINFO || Command == FCTL_GETPANELSHORTINFO) ? this:AnotherPanel;
      if (DestPanel==NULL)
        break;
      /* $ 19.03.2002 DJ
         ��������� ������� ������
      */
      DestPanel->UpdateIfRequired();
      /* DJ $ */
      switch(DestPanel->GetType())
      {
        case FILE_PANEL:
          Info->PanelType=PTYPE_FILEPANEL;
          break;
        case TREE_PANEL:
          Info->PanelType=PTYPE_TREEPANEL;
          break;
        case QVIEW_PANEL:
          Info->PanelType=PTYPE_QVIEWPANEL;
          break;
        case INFO_PANEL:
          Info->PanelType=PTYPE_INFOPANEL;
          break;
      }
      Info->Plugin=DestPanel->GetMode()==PLUGIN_PANEL;
      int X1,Y1,X2,Y2;
      DestPanel->GetPosition(X1,Y1,X2,Y2);
      Info->PanelRect.left=X1;
      Info->PanelRect.top=Y1;
      Info->PanelRect.right=X2;
      Info->PanelRect.bottom=Y2;
      Info->Visible=DestPanel->IsVisible();
      Info->Focus=DestPanel->GetFocus();
      Info->ViewMode=DestPanel->GetViewMode();
      Info->SortMode=SM_UNSORTED-UNSORTED+DestPanel->GetSortMode();
      DestPanel->GetCurDir(Info->CurDir);
      /* $ 24.04.2001 SVS
         ���������� ������ PanelInfo.Flags
      */
      {
        static struct {
          int *Opt;
          DWORD Flags;
        } PFLAGS[]={
          {&Opt.ShowHidden,PFLAGS_SHOWHIDDEN},
          {&Opt.Highlight,PFLAGS_HIGHLIGHT},
        };

        DWORD Flags=0;

        for(int I=0; I < sizeof(PFLAGS)/sizeof(PFLAGS[0]); ++I)
          if(*(PFLAGS[I].Opt) != 0)
            Flags|=PFLAGS[I].Flags;

        Flags|=DestPanel->GetSortOrder()<0?PFLAGS_REVERSESORTORDER:0;
        Flags|=DestPanel->GetSortGroups()?PFLAGS_USESORTGROUPS:0;
        Flags|=DestPanel->GetSelectedFirstMode()?PFLAGS_SELECTEDFIRST:0;

        Info->Flags=Flags;
      }
      /* SVS $ */
      if (DestPanel->GetType()==FILE_PANEL)
      {
        FileList *DestFilePanel=(FileList *)DestPanel;
        static int Reenter=0;
        if (!Reenter && Info->Plugin)
        {
          Reenter++;
          struct OpenPluginInfo PInfo;
          DestFilePanel->GetOpenPluginInfo(&PInfo);
          strcpy(Info->CurDir,PInfo.CurDir);
          /* $ 12.12.2001 DJ
             ���������� �����
          */
          if (PInfo.Flags & OPIF_REALNAMES)
            Info->Flags |= PFLAGS_REALNAMES;
          if (!(PInfo.Flags & OPIF_USEHIGHLIGHTING))
            Info->Flags &= ~PFLAGS_HIGHLIGHT;
          /* DJ $ */
          Reenter--;
        }
        DestFilePanel->PluginGetPanelInfo(Info,(Command == FCTL_GETPANELINFO || Command == FCTL_GETANOTHERPANELINFO)?TRUE:FALSE);
      }
      /* $ 12.12.2001 DJ
         �� ������������ ������ - ������ �������� �����
      */
      if (!Info->Plugin)
          Info->Flags |= PFLAGS_REALNAMES;
      /* DJ $ */
      break;
    }

    case FCTL_SETSELECTION:
    case FCTL_SETANOTHERSELECTION:
      {
        Panel *DestPanel=(Command==FCTL_SETSELECTION) ? this:AnotherPanel;
        if (DestPanel!=NULL && DestPanel->GetType()==FILE_PANEL)
          ((FileList *)DestPanel)->PluginSetSelection((struct PanelInfo *)Param);
        break;
      }
    case FCTL_UPDATEPANEL:
      Update(Param==NULL ? 0:UPDATE_KEEP_SELECTION);
      break;
    case FCTL_UPDATEANOTHERPANEL:
      AnotherPanel->Update(Param==NULL ? 0:UPDATE_KEEP_SELECTION);
      if (AnotherPanel!=NULL && AnotherPanel->GetType()==QVIEW_PANEL)
        UpdateViewPanel();
      break;
    case FCTL_REDRAWPANEL:
      {
        struct PanelRedrawInfo *Info=(struct PanelRedrawInfo *)Param;
        if (Info!=NULL)
        {
          CurFile=Info->CurrentItem;
          CurTopFile=Info->TopPanelItem;
        }
        /* $ 12.05.2001 DJ
           ���������������� ������ � ��� ������, ���� �� - ������� �����
        */
        if (CtrlObject->Cp()->IsTopFrame())
          Redraw();
        /* DJ */
      }
      break;
    case FCTL_REDRAWANOTHERPANEL:
      if (AnotherPanel!=NULL)
      {
        struct PanelRedrawInfo *Info=(struct PanelRedrawInfo *)Param;
        if (Info!=NULL)
        {
          AnotherPanel->CurFile=Info->CurrentItem;
          AnotherPanel->CurTopFile=Info->TopPanelItem;
        }
        /* $ 12.05.2001 DJ
           ���������������� ������ � ��� ������, ���� �� - ������� �����
        */
        if (CtrlObject->Cp()->IsTopFrame())
          AnotherPanel->Redraw();
        /* DJ */
      }
      break;
    /* $ 03.12.2001 DJ
       ������������� ����
    */
    case FCTL_SETANOTHERPANELDIR:
    {
      if (Param!=NULL && AnotherPanel!=NULL)
        AnotherPanel->SetCurDir((char *)Param,TRUE);
      break;
    }

    case FCTL_SETPANELDIR:
    {
      if (Param!=NULL)
        SetCurDir((char *)Param,TRUE);
      break;
    }
    /* DJ $ */
  }
  ProcessingPluginCommand--;
}


int Panel::GetCurName(char *Name,char *ShortName)
{
  *Name=*ShortName=0;
  return(FALSE);
}

int Panel::GetCurBaseName(char *Name,char *ShortName)
{
  *Name=*ShortName=0;
  return(FALSE);
}

static int MessageRemoveConnection(char Letter, int &UpdateProfile)
{
  int Len1, Len2, Len3,Len4;
  BOOL IsPersistent;
  char MsgText[NM];
/*
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345
0
1   +-------- ���������� �������� ���������� --------+
2   | �� ������ ������� ���������� � ����������� C:? |
3   | �� ���������� %c: ��������� �������            |
4   | \\host\share                                   |
6   +------------------------------------------------+
7   | [ ] ��������������� ��� ����� � �������        |
8   +------------------------------------------------+
9   |              [ �� ]   [ ������ ]               |
10  +------------------------------------------------+
11
*/
  static struct DialogData DCDlgData[]=
  {
/*      Type          X1 Y1 X2  Y2 Focus Flags             DefaultButton
                                      Selected               Data
*/
/* 0 */ DI_DOUBLEBOX, 3, 1, 72, 9, 0, 0, 0,                0,"",
/* 1 */ DI_TEXT,      5, 2,  0, 0, 0, 0, DIF_SHOWAMPERSAND,0,"",
/* 2 */ DI_TEXT,      5, 3,  0, 0, 0, 0, DIF_SHOWAMPERSAND,0,"",
/* 3 */ DI_TEXT,      5, 4,  0, 0, 0, 0, DIF_SHOWAMPERSAND,0,"",
/* 4 */ DI_TEXT,      0, 5,  0, 6, 0, 0, DIF_SEPARATOR,    0,"",
/* 5 */ DI_CHECKBOX,  5, 6, 70, 5, 0, 0, 0,                0,"",
/* 6 */ DI_TEXT,      0, 7,  0, 6, 0, 0, DIF_SEPARATOR,    0,"",
/* 7 */ DI_BUTTON,    0, 8,  0, 0, 1, 0, DIF_CENTERGROUP,  1,"",
/* 8 */ DI_BUTTON,    0, 8,  0, 0, 0, 0, DIF_CENTERGROUP,  0,""
  };
  MakeDialogItems(DCDlgData,DCDlg);

  Len1=strlen(strcpy(DCDlg[0].Data,MSG(MChangeDriveDisconnectTitle)));
  sprintf(MsgText,MSG(MChangeDriveDisconnectQuestion),Letter);
  Len2=strlen(strcpy(DCDlg[1].Data,MsgText));
  sprintf(MsgText,MSG(MChangeDriveDisconnectMapped),Letter);
  Len4=strlen(strcpy(DCDlg[2].Data,MsgText));
  Len3=strlen(strcpy(DCDlg[5].Data,MSG(MChangeDriveDisconnectReconnect)));


  Len1=Max(Len1,Max(Len2,Max(Len3,Len4)));

  strcpy(DCDlg[3].Data,TruncPathStr(DriveLocalToRemoteName(DRIVE_REMOTE,Letter,MsgText),Len1));

  strcpy(DCDlg[7].Data,MSG(MYes));
  strcpy(DCDlg[8].Data,MSG(MCancel));

  // ��������� - ��� ���� ���������� �������� ��� ���?
  // ���� ����� � ������� HKCU\Network\���������� ���� - ���
  //   ���� ���������� �����������.
  {
    HKEY hKey;
    IsPersistent=TRUE;
    if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
      sprintf(MsgText,"Network\\%c",toupper(Letter));
    else
      sprintf(MsgText,"Network\\Persistent\\%c",toupper(Letter));
    if(RegOpenKeyEx(HKEY_CURRENT_USER,MsgText,0,KEY_QUERY_VALUE,&hKey)!=ERROR_SUCCESS)
    {
      DCDlg[5].Flags|=DIF_DISABLE;
      DCDlg[5].Selected=0;
      IsPersistent=FALSE;
    }
    else
      DCDlg[5].Selected=Opt.ChangeDriveDisconnetMode;
    RegCloseKey(hKey);
  }

  // ������������� ������� ������� - ��� �������
  DCDlg[0].X2=DCDlg[0].X1+Len1+3;

  int ExitCode=7;
  if(Opt.Confirm.RemoveConnection)
  {
    Dialog Dlg(DCDlg,sizeof(DCDlg)/sizeof(DCDlg[0]));
    Dlg.SetPosition(-1,-1,DCDlg[0].X2+4,11);
    Dlg.SetHelp("DisconnectDrive");
    Dlg.Process();
    ExitCode=Dlg.GetExitCode();
  }
  UpdateProfile=DCDlg[5].Selected?0:CONNECT_UPDATE_PROFILE;
  if(IsPersistent)
    Opt.ChangeDriveDisconnetMode=DCDlg[5].Selected;
  return ExitCode == 7;
}

BOOL Panel::NeedUpdatePanel(Panel *AnotherPanel)
{
  /* ��������, ���� ���������� ��������� � ���� ��������� */
  if ((!Opt.AutoUpdateLimit || GetFileCount() <= Opt.AutoUpdateLimit) &&
      LocalStricmp(AnotherPanel->CurDir,CurDir)==0)
    return TRUE;
  return FALSE;
}
