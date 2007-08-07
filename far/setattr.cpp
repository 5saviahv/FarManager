/*
setattr.cpp

��������� ��������� ������

*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "flink.hpp"
#include "global.hpp"
#include "lang.hpp"
#include "dialog.hpp"
#include "chgprior.hpp"
#include "scantree.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "constitle.hpp"


#define DM_SETATTR      (DM_USER+1)

enum {
  SETATTR_TITLE=0,

  SETATTR_NAME=2,

  SETATTR_RO=4,
  SETATTR_ARCHIVE=5,
  SETATTR_HIDDEN=6,
  SETATTR_SYSTEM=7,
  SETATTR_COMPRESSED=8,
  SETATTR_ENCRYPTED=9,
  SETATTR_INDEXED=10,
  SETATTR_SPARSE=11,
  SETATTR_TEMP=12,

  SETATTR_SUBFOLDERS=14,

  SETATTR_TITLEDATE=16,
  SETATTR_MODIFICATION=17,
  SETATTR_MDATE=18,
  SETATTR_MTIME=19,
  SETATTR_CREATION=20,
  SETATTR_CDATE=21,
  SETATTR_CTIME=22,
  SETATTR_LASTACCESS=23,
  SETATTR_ADATE=24,
  SETATTR_ATIME=25,
  SETATTR_ORIGINAL=26,
  SETATTR_CURRENT=27,
  SETATTR_BLANK=28,

  SETATTR_SET=30,
  SETATTR_CANCEL=31,
  SETATTR_TITLELINK=32,
};

const char FmtMask1[]="99%c99%c99";
const char FmtMask2[]="99%c99%c9999";
const char FmtMask3[]="9999%c99%c99";

struct SetAttrDlgParam{
  BOOL  Plugin;
  DWORD FileSystemFlags;
  int ModeDialog;
  int DateFormat;
  char *SelName;
  int OriginalCBAttr[16]; // �������� CheckBox`�� �� ������ ������ �������
  int OriginalCBAttr2[16]; //
  DWORD OriginalCBFlag[16];
  int OStateF_12, OStateC_8, OStateE_9;
  int OLastWriteTime,OCreationTime,OLastAccessTime;
  char FSysName[NM];
};

static int IsFileWritable(const char *Name, DWORD FileAttr, BOOL IsShowErrMsg, int Msg);
static void PR_ShellSetFileAttributesMsg(void);
void ShellSetFileAttributesMsg(char *Name);

// ���������� ������� - ���� ��� ����� ������� ������ ������.
LONG_PTR WINAPI SetAttrDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2)
{
  int FocusPos,I;
  int StateC_8, StateE_9, StateF_12;
  struct SetAttrDlgParam *DlgParam;

  DlgParam=(struct SetAttrDlgParam *)Dialog::SendDlgMessage(hDlg,DM_GETDLGDATA,0,0);
  switch(Msg)
  {
    case DN_BTNCLICK:
      if(Param1 >= SETATTR_RO && Param1 <= SETATTR_TEMP || Param1 == SETATTR_SUBFOLDERS)
      {
        DlgParam->OriginalCBAttr[Param1-SETATTR_RO] = (int)Param2;
        DlgParam->OriginalCBAttr2[Param1-SETATTR_RO] = 0;

        FocusPos=(int)Dialog::SendDlgMessage(hDlg,DM_GETFOCUS,0,0);
        StateC_8=(int)Dialog::SendDlgMessage(hDlg,DM_GETCHECK,SETATTR_COMPRESSED,0);
        StateE_9=(int)Dialog::SendDlgMessage(hDlg,DM_GETCHECK,SETATTR_ENCRYPTED,0);
        StateF_12=(int)Dialog::SendDlgMessage(hDlg,DM_GETCHECK,SETATTR_SUBFOLDERS,0);

        if(!DlgParam->ModeDialog) // =0 - ���������
        {
          if(((DlgParam->FileSystemFlags & (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION))==
               (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION)) &&
             (FocusPos == SETATTR_COMPRESSED || FocusPos == SETATTR_ENCRYPTED))
          {
              if(FocusPos == SETATTR_COMPRESSED && /*StateC_8 &&*/ StateE_9)
                Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_ENCRYPTED,BSTATE_UNCHECKED);
              if(FocusPos == SETATTR_ENCRYPTED && /*StateE_9 &&*/ StateC_8)
                Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_COMPRESSED,BSTATE_UNCHECKED);
          }
        }
        else // =1|2 Multi
        {
          // ���������� ����������������
          if(((DlgParam->FileSystemFlags & (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION))==
               (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION)) &&
             (FocusPos == SETATTR_COMPRESSED || FocusPos == SETATTR_ENCRYPTED))
          {
            if(FocusPos == SETATTR_COMPRESSED && DlgParam->OStateC_8 != StateC_8) // ��������� ����������?
            {
              if(StateC_8 == BSTATE_CHECKED && StateE_9)
                Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_ENCRYPTED,BSTATE_UNCHECKED);
              else if(StateC_8 == BSTATE_3STATE)
                Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_ENCRYPTED,BSTATE_3STATE);
            }
            else if(FocusPos == SETATTR_ENCRYPTED && DlgParam->OStateE_9 != StateE_9) // ��������� ����������?
            {
              if(StateE_9 == BSTATE_CHECKED && StateC_8)
                Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_COMPRESSED,BSTATE_UNCHECKED);
              else if(StateE_9 == BSTATE_3STATE)
                Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_COMPRESSED,BSTATE_3STATE);
            }

            // ��� ���� ��������
            if(FocusPos == SETATTR_COMPRESSED && /* DlgParam->OStateC_8 && */ StateE_9)
              Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_ENCRYPTED,BSTATE_UNCHECKED);
            if(FocusPos == SETATTR_ENCRYPTED && /* DlgParam->OStateE_9 && */ StateC_8)
              Dialog::SendDlgMessage(hDlg,DM_SETCHECK,SETATTR_COMPRESSED,BSTATE_UNCHECKED);

            DlgParam->OStateE_9=StateE_9;
            DlgParam->OStateC_8=StateC_8;
          }

          // ���� ������� �������� ��� SubFolders
          // ���� ����� ������ �������� ���� ���� ���� �� ���� �����
          // ����� 12-� ���������� � ������ ����.
          if(FocusPos == SETATTR_SUBFOLDERS)
          {
            if(DlgParam->ModeDialog==1) // ������� ����������!
            {
              if(DlgParam->OStateF_12 != StateF_12) // ��������� ����������?
              {
                // ������� 3-State
                for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
                {
                  if(!StateF_12) // �����?
                  {
                    Dialog::SendDlgMessage(hDlg,DM_SET3STATE,I,FALSE);
                    Dialog::SendDlgMessage(hDlg,DM_SETCHECK,I,DlgParam->OriginalCBAttr[I-SETATTR_RO]);
                  }
                  else                      // ����������?
                  {
                    Dialog::SendDlgMessage(hDlg,DM_SET3STATE,I,TRUE);
                    if(DlgParam->OriginalCBAttr2[I-SETATTR_RO] == -1)
                      Dialog::SendDlgMessage(hDlg,DM_SETCHECK,I,BSTATE_3STATE);
                  }
                }
                if(Opt.SetAttrFolderRules)
                {
                  HANDLE FindHandle;
                  WIN32_FIND_DATA FindData;
                  FindHandle=FindFirstFile(DlgParam->SelName,&FindData);
                  FindClose(FindHandle);

                  if (FindHandle!=INVALID_HANDLE_VALUE)
                  {
                    if(!StateF_12)
                    {
                      if(!DlgParam->OLastWriteTime)
                      {
                        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_MODIFICATION,(LONG_PTR)&FindData.ftLastWriteTime);
                        DlgParam->OLastWriteTime=0;
                      }
                      if(!DlgParam->OCreationTime)
                      {
                        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_CREATION,(LONG_PTR)&FindData.ftCreationTime);
                        DlgParam->OCreationTime=0;
                      }
                      if(!DlgParam->OLastAccessTime)
                      {
                        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_LASTACCESS,(LONG_PTR)&FindData.ftLastAccessTime);
                        DlgParam->OLastAccessTime=0;
                      }
                    }
                    else
                    {
                      if(!DlgParam->OLastWriteTime)
                      {
                        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_MODIFICATION,0);
                        DlgParam->OLastWriteTime=0;
                      }
                      if(!DlgParam->OCreationTime)
                      {
                        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_CREATION,0);
                        DlgParam->OCreationTime=0;
                      }
                      if(!DlgParam->OLastAccessTime)
                      {
                        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_LASTACCESS,0);
                        DlgParam->OLastAccessTime=0;
                      }
                    }
                  }

                }
              }
            }
            else  // ����� ��������
            {
              if(DlgParam->OStateF_12 != StateF_12) // ��������� ����������?
              {
                for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
                {
                  if(!StateF_12) // �����?
                  {
                    Dialog::SendDlgMessage(hDlg,DM_SET3STATE,I,
                              ((DlgParam->OriginalCBFlag[I-SETATTR_RO]&DIF_3STATE)?TRUE:FALSE));
                    Dialog::SendDlgMessage(hDlg,DM_SETCHECK,I,DlgParam->OriginalCBAttr[I-SETATTR_RO]);
                  }
                  else                      // ����������?
                  {
                    if(DlgParam->OriginalCBAttr2[I-SETATTR_RO] == -1)
                    {
                      Dialog::SendDlgMessage(hDlg,DM_SET3STATE,I,TRUE);
                      Dialog::SendDlgMessage(hDlg,DM_SETCHECK,I,BSTATE_3STATE);
                    }
                  }
                }
              }
            }
            DlgParam->OStateF_12=StateF_12;
          }
        }
        return TRUE;
      }
      // Set Original? / Set All? / Clear All?
      else if(Param1 == SETATTR_ORIGINAL)
      {
        HANDLE FindHandle;
        WIN32_FIND_DATA FindData;
        DlgParam=(struct SetAttrDlgParam *)Dialog::SendDlgMessage(hDlg,DM_GETDLGDATA,0,0);
        if ((FindHandle=FindFirstFile(DlgParam->SelName,&FindData))!=INVALID_HANDLE_VALUE)
        {
          FindClose(FindHandle);
          Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_MODIFICATION,(LONG_PTR)&FindData.ftLastWriteTime);
          Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_CREATION,(LONG_PTR)&FindData.ftCreationTime);
          Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_LASTACCESS,(LONG_PTR)&FindData.ftLastAccessTime);
          DlgParam->OLastWriteTime=DlgParam->OCreationTime=DlgParam->OLastAccessTime=0;
        }
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,SETATTR_MDATE,0);
        return TRUE;
      }
      else if(Param1 == SETATTR_CURRENT || Param1 == SETATTR_BLANK)
      {
        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_MODIFICATION,Param1 == SETATTR_CURRENT?-1:0);
        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_CREATION,Param1 == SETATTR_CURRENT?-1:0);
        Dialog::SendDlgMessage(hDlg,DM_SETATTR,SETATTR_LASTACCESS,Param1 == SETATTR_CURRENT?-1:0);
        DlgParam->OLastWriteTime=DlgParam->OCreationTime=DlgParam->OLastAccessTime=1;
        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,SETATTR_MDATE,0);
        return TRUE;
      }
      break;

    case DN_MOUSECLICK:
     {
       //_SVS(SysLog("Msg=DN_MOUSECLICK Param1=%d Param2=%d",Param1,Param2));
       if(Param1 >= SETATTR_MODIFICATION && Param1 <= SETATTR_ATIME)
       {
         if(((MOUSE_EVENT_RECORD*)Param2)->dwEventFlags == DOUBLE_CLICK)
         {
           // ����� ��������� �������� "��������"
           Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
           Dialog::SendDlgMessage(hDlg,DM_SETATTR,Param1,-1);
         }
         if(Param1 == SETATTR_MODIFICATION || Param1 == SETATTR_CREATION || Param1 == SETATTR_LASTACCESS)
           Param1++;
         Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,Param1,0);
         return TRUE;
       }
     }
     break;

    case DN_EDITCHANGE:
    {
      if(Param1 >= SETATTR_MDATE && Param1 <= SETATTR_ATIME)
      {
             if(Param1 == SETATTR_MDATE || Param1 == SETATTR_MTIME) { DlgParam->OLastWriteTime=1;}
        else if(Param1 == SETATTR_CDATE || Param1 == SETATTR_CTIME) { DlgParam->OCreationTime=1;}
        else if(Param1 == SETATTR_ADATE || Param1 == SETATTR_ATIME) { DlgParam->OLastAccessTime=1;}
      }
      break;
    }

    case DM_SETATTR:
      {
        FILETIME ft;
        char Date[16],Time[16];
        int Set1, Set2;
        if(Param2) // Set?
        {
          if(Param2==-1)
            GetSystemTimeAsFileTime(&ft);
          else
            ft=*(FILETIME *)Param2;
          ConvertDate(ft,Date,Time,8,FALSE,FALSE,TRUE,TRUE);
        }
        else if(!Param2) // Clear
        {
           Date[0]=Time[0]=0;
        }

        // ������ �� �����, ��� ��� ����
             if(Param1 == SETATTR_MODIFICATION) { Set1=SETATTR_MDATE; Set2=SETATTR_MTIME; DlgParam->OLastWriteTime=1;}
        else if(Param1 == SETATTR_CREATION) { Set1=SETATTR_CDATE; Set2=SETATTR_CTIME; DlgParam->OCreationTime=1;}
        else if(Param1 == SETATTR_LASTACCESS) { Set1=SETATTR_ADATE; Set2=SETATTR_ATIME; DlgParam->OLastAccessTime=1;}
        else if(Param1 == SETATTR_MDATE || Param1 == SETATTR_CDATE || Param1 == SETATTR_ADATE) { Set1=Param1; Set2=-1; }
        else { Set1=-1; Set2=Param1; }

        if(Set1 != -1)
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,Set1,(LONG_PTR)Date);
        if(Set2 != -1)
          Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,Set2,(LONG_PTR)Time);
        return TRUE;
      }
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

static void PR_ShellSetFileAttributesMsg(void)
{
  ShellSetFileAttributesMsg((char *)PreRedrawParam.Param1);
}

void ShellSetFileAttributesMsg(char *Name)
{
  static int Width=54;
  int WidthTemp;
  char OutFileName[NM];

  if(Name && *Name)
    WidthTemp=Max((int)strlen(Name),(int)54);
  else
    Width=WidthTemp=54;

  if(WidthTemp > WidthNameForMessage)
    WidthTemp=WidthNameForMessage; // ������ ������ - 38%
  if(WidthTemp >= sizeof(OutFileName)-4)
    WidthTemp=sizeof(OutFileName)-5;

  if(Width < WidthTemp)
    Width=WidthTemp;

  if(Name && *Name)
  {
    xstrncpy(OutFileName,Name,sizeof(OutFileName)-1);
    TruncPathStr(OutFileName,Width);
    CenterStr(OutFileName,OutFileName,Width+4);
  }
  else
  {
    *OutFileName=0;
    CenterStr(OutFileName,OutFileName,Width+4); // �������������� ������ ������ (���!)
  }
  Message(0,0,MSG(MSetAttrTitle),MSG(MSetAttrSetting),OutFileName);
  PreRedrawParam.Param1=Name;
}

int ShellSetFileAttributes(Panel *SrcPanel)
{
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
/*MSetAttrJunction
00                                             00
01   +------------ Attributes -------------+   01
02   |     Change file attributes for      |   02
03   |                 foo                 |   03
04   |          Link: blach blach          | < 04 <<
05   +-------------------------------------+   05
06   | [ ] Read only                       |   06
07   | [ ] Archive                         |   07
08   | [ ] Hidden                          |   08
09   | [ ] System                          |   09
10   | [ ] Compressed                      |   10
11   | [ ] Encrypted                       |   11
12   | [ ] Indexed                         |   12
13   +-------------------------------------+   13
14   | [x] Process subfolders              |   14
15   +-------------------------------------+   15
16   |  File time      DD.MM.YYYY hh:mm:ss |   16
17   | Modification      .  .       :  :   |   17
18   | Creation          .  .       :  :   |   18
19   | Last access       .  .       :  :   |   19
20   | [ Original ]  [ Current ] [ Blank ] |   10
21   +-------------------------------------+   21
22   |         [ Set ]  [ Cancel ]         |   22
23   +-------------------------------------+   23
24                                             24
*/
  static struct DialogData AttrDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,65,20,0,0,0,0,(char *)MSetAttrTitle,
  /* 01 */DI_TEXT,-1,2,0,2,0,0,0,0,(char *)MSetAttrFor,
  /* 02 */DI_TEXT,-1,3,0,3,0,0,DIF_SHOWAMPERSAND,0,"",
  /* 03 */DI_TEXT,3,4,0,4,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */DI_CHECKBOX,5, 5,0,5,1,0,DIF_3STATE,0,(char *)MSetAttrRO,
  /* 05 */DI_CHECKBOX,5, 6,0,6,0,0,DIF_3STATE,0,(char *)MSetAttrArchive,
  /* 06 */DI_CHECKBOX,5, 7,0,7,0,0,DIF_3STATE,0,(char *)MSetAttrHidden,
  /* 07 */DI_CHECKBOX,5, 8,0,8,0,0,DIF_3STATE,0,(char *)MSetAttrSystem,
  /* 08 */DI_CHECKBOX,5, 9,0,9,0,0,DIF_3STATE,0,(char *)MSetAttrCompressed,
  /* 09 */DI_CHECKBOX,35, 5,0,5,0,0,DIF_3STATE,0,(char *)MSetAttrEncrypted,
  /* 10 */DI_CHECKBOX,35, 6,0,6,0,0,DIF_3STATE|DIF_DISABLE,0,(char *)MSetAttrNotIndexed,
  /* 11 */DI_CHECKBOX,35, 7,0,7,0,0,DIF_3STATE|DIF_DISABLE,0,(char *)MSetAttrSparse,
  /* 12 */DI_CHECKBOX,35, 8,0,8,0,0,DIF_3STATE|DIF_DISABLE,0,(char *)MSetAttrTemp,
  /* 13 */DI_TEXT,3,10,0,10,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 14 */DI_CHECKBOX,5,11,0,11,0,0,DIF_DISABLE,0,(char *)MSetAttrSubfolders,
  /* 15 */DI_TEXT,3,12,0,12,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 16 */DI_TEXT,45,13,0,13,0,0,0,0,"",
  /* 17 */DI_TEXT,    5,14,0,14,0,0,0,0,(char *)MSetAttrModification,
  /* 18 */DI_FIXEDIT,45,14,54,14,0,0,DIF_MASKEDIT,0,"",
  /* 19 */DI_FIXEDIT,56,14,63,14,0,0,DIF_MASKEDIT,0,"",
  /* 20 */DI_TEXT,    5,15,0,15,0,0,0,0,(char *)MSetAttrCreation,
  /* 21 */DI_FIXEDIT,45,15,54,15,0,0,DIF_MASKEDIT,0,"",
  /* 22 */DI_FIXEDIT,56,15,63,15,0,0,DIF_MASKEDIT,0,"",
  /* 23 */DI_TEXT,    5,16,0,16,0,0,0,0,(char *)MSetAttrLastAccess,
  /* 24 */DI_FIXEDIT,45,16,54,16,0,0,DIF_MASKEDIT,0,"",
  /* 25 */DI_FIXEDIT,56,16,63,16,0,0,DIF_MASKEDIT,0,"",
  /* 26 */DI_BUTTON,0,17,0,17,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MSetAttrOriginal,
  /* 27 */DI_BUTTON,0,17,0,17,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MSetAttrCurrent,
  /* 28 */DI_BUTTON,0,17,0,17,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MSetAttrBlank,
  /* 29 */DI_TEXT,3,18,0,18,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 30 */DI_BUTTON,0,19,0,19,0,0,DIF_CENTERGROUP,1,(char *)MSetAttrSet,
  /* 31 */DI_BUTTON,0,19,0,19,0,0,DIF_CENTERGROUP,0,(char *)MCancel,
  /* 32 */DI_TEXT,-1,4,0,4,0,0,DIF_SHOWAMPERSAND,0,"",
  };
  MakeDialogItems(AttrDlgData,AttrDlg);
  int DlgCountItems=sizeof(AttrDlgData)/sizeof(AttrDlgData[0])-1;

  int SelCount, I, J;
  struct SetAttrDlgParam DlgParam;

  if((SelCount=SrcPanel->GetSelCount())==0)
    return 0;

  memset(&DlgParam,0,sizeof(DlgParam));

  if (SrcPanel->GetMode()==PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    HANDLE hPlugin=SrcPanel->GetPluginHandle();
    if(hPlugin == INVALID_HANDLE_VALUE)
      return 0;

    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
    if(!(Info.Flags & OPIF_REALNAMES))
    {
      AttrDlg[SETATTR_SET].Flags|=DIF_DISABLE;
      DlgParam.Plugin=TRUE;
    }
  }

  DlgParam.FileSystemFlags=0;

  _SVS(char lpRootPathName[NM]="");
  _SVS(char lpVolumeNameBuffer[NM]="");
  _SVS(char lpFileSystemNameBuffer[NM]="");
  _SVS(DWORD lpVolumeSerialNumber=0);
  _SVS(DWORD lpMaximumComponentLength=0);
  _SVS(DWORD lpFileSystemFlags=0);
  _SVS(GetCurrentDirectory(sizeof(lpRootPathName),lpRootPathName));
  _SVS(GetPathRoot(lpRootPathName,lpRootPathName));
  _SVS(GetVolumeInformation(lpRootPathName,lpVolumeNameBuffer,sizeof(lpVolumeNameBuffer),
                                           &lpVolumeSerialNumber,&lpMaximumComponentLength,&lpFileSystemFlags,
                                           lpFileSystemNameBuffer,sizeof(lpFileSystemNameBuffer)));
  _SVS(GetVolumeInformation_Dump("SetAttr",lpRootPathName,lpVolumeNameBuffer,sizeof(lpVolumeNameBuffer),
                                           lpVolumeSerialNumber,lpMaximumComponentLength,lpFileSystemFlags,
                                           lpFileSystemNameBuffer,sizeof(lpFileSystemNameBuffer),NULL));

  if(DlgParam.Plugin)
  {
    AttrDlg[SETATTR_COMPRESSED].Flags|=DIF_DISABLE;
    AttrDlg[SETATTR_ENCRYPTED].Flags|=DIF_DISABLE;
  }
  else
  {
    //char lpRootPathName[NM];
    //GetCurrentDirectory(sizeof(lpRootPathName),lpRootPathName);
    //GetPathRoot(lpRootPathName,lpRootPathName);
    if (GetVolumeInformation(NULL,NULL,0,NULL,NULL,&DlgParam.FileSystemFlags,DlgParam.FSysName,sizeof(DlgParam.FSysName)))
    {
      if (!(DlgParam.FileSystemFlags & FS_FILE_COMPRESSION))
        AttrDlg[SETATTR_COMPRESSED].Flags|=DIF_DISABLE;

      if (!IsCryptFileASupport || !(DlgParam.FileSystemFlags & FS_FILE_ENCRYPTION))
        AttrDlg[SETATTR_ENCRYPTED].Flags|=DIF_DISABLE;

      if(!strcmp(DlgParam.FSysName,"NTFS"))
        AttrDlg[SETATTR_INDEXED].Flags&=~DIF_DISABLE;

    }
  }

  {
    char SelName[NM];
    int FileAttr;
    FILETIME LastWriteTime,CreationTime,LastAccessTime;
    int SetWriteTime,SetCreationTime,SetLastAccessTime;
    int SetWriteTimeRetCode=TRUE;
    WIN32_FIND_DATA FindData;

    //SaveScreen SaveScr;

    SrcPanel->GetSelName(NULL,FileAttr);
    SrcPanel->GetSelName(SelName,FileAttr,NULL,&FindData);
    if (SelCount==0 || SelCount==1 && TestParentFolderName(SelName))
      return 0;

//    int NewAttr;
    int FolderPresent=FALSE, JunctionPresent=FALSE;

    int DateSeparator=GetDateSeparator();
    int TimeSeparator=GetTimeSeparator();
    static char DMask[20],TMask[20];

    sprintf(TMask,FmtMask1,TimeSeparator,TimeSeparator);
    switch(DlgParam.DateFormat=GetDateFormat())
    {
      case 0:
        sprintf(AttrDlg[SETATTR_TITLEDATE].Data,MSG(MSetAttrTimeTitle1),DateSeparator,DateSeparator,TimeSeparator,TimeSeparator);
        sprintf(DMask,FmtMask2,DateSeparator,DateSeparator);
        break;
      case 1:
        sprintf(AttrDlg[SETATTR_TITLEDATE].Data,MSG(MSetAttrTimeTitle2),DateSeparator,DateSeparator,TimeSeparator,TimeSeparator);
        sprintf(DMask,FmtMask2,DateSeparator,DateSeparator);
        break;
      default:
        sprintf(AttrDlg[SETATTR_TITLEDATE].Data,MSG(MSetAttrTimeTitle3),DateSeparator,DateSeparator,TimeSeparator,TimeSeparator);
        sprintf(DMask,FmtMask3,DateSeparator,DateSeparator);
        break;
    }

    AttrDlg[SETATTR_MDATE].Mask=DMask;
    AttrDlg[SETATTR_MTIME].Mask=TMask;
    AttrDlg[SETATTR_CDATE].Mask=DMask;
    AttrDlg[SETATTR_CTIME].Mask=TMask;
    AttrDlg[SETATTR_ADATE].Mask=DMask;
    AttrDlg[SETATTR_ATIME].Mask=TMask;

    if (SelCount==1)
    {
      if((FileAttr & FA_DIREC))
      {
        if(!DlgParam.Plugin && SelName[strlen(SelName)-1] != '\\')
        {
          AddEndSlash(SelName);
          FileAttr=GetFileAttributes(SelName);
          SelName[strlen(SelName)-1]=0;
        }
        //_SVS(SysLog("SelName=%s  FileAttr=0x%08X",SelName,FileAttr));
        AttrDlg[SETATTR_SUBFOLDERS].Flags&=~DIF_DISABLE;
        AttrDlg[SETATTR_SUBFOLDERS].Selected=Opt.SetAttrFolderRules == 1?0:1;
        if(Opt.SetAttrFolderRules)
        {
          if(DlgParam.Plugin)
          {
            ConvertDate(FindData.ftLastWriteTime, AttrDlg[SETATTR_MDATE].Data,AttrDlg[SETATTR_MTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
            ConvertDate(FindData.ftCreationTime,  AttrDlg[SETATTR_CDATE].Data,AttrDlg[SETATTR_CTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
            ConvertDate(FindData.ftLastAccessTime,AttrDlg[SETATTR_ADATE].Data,AttrDlg[SETATTR_ATIME].Data,8,FALSE,FALSE,TRUE,TRUE);
          }
          else
          {
            HANDLE FindHandle;
            if ((FindHandle=FindFirstFile(SelName,&FindData))!=INVALID_HANDLE_VALUE)
            {
              FindClose(FindHandle);
              ConvertDate(FindData.ftLastWriteTime, AttrDlg[SETATTR_MDATE].Data,AttrDlg[SETATTR_MTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
              ConvertDate(FindData.ftCreationTime,  AttrDlg[SETATTR_CDATE].Data,AttrDlg[SETATTR_CTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
              ConvertDate(FindData.ftLastAccessTime,AttrDlg[SETATTR_ADATE].Data,AttrDlg[SETATTR_ATIME].Data,8,FALSE,FALSE,TRUE,TRUE);
            }
          }
          AttrDlg[SETATTR_RO].Selected=(FileAttr & FA_RDONLY)!=0;
          AttrDlg[SETATTR_ARCHIVE].Selected=(FileAttr & FA_ARCH)!=0;
          AttrDlg[SETATTR_HIDDEN].Selected=(FileAttr & FA_HIDDEN)!=0;
          AttrDlg[SETATTR_SYSTEM].Selected=(FileAttr & FA_SYSTEM)!=0;
          AttrDlg[SETATTR_COMPRESSED].Selected=(FileAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
          AttrDlg[SETATTR_ENCRYPTED].Selected=(FileAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;
          AttrDlg[SETATTR_INDEXED].Selected=(FileAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)!=0;
          AttrDlg[SETATTR_SPARSE].Selected=(FileAttr & FILE_ATTRIBUTE_SPARSE_FILE)!=0;
          AttrDlg[SETATTR_TEMP].Selected=(FileAttr & FILE_ATTRIBUTE_TEMPORARY)!=0;

          // ������� 3-State
          for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
            AttrDlg[I].Flags&=~DIF_3STATE;
        }
        FolderPresent=TRUE;
      }
      else
      {
        // ������� 3-State
        for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
          AttrDlg[I].Flags&=~DIF_3STATE;
      }

      // ��������� ������, ���� ��� SymLink
      if(FileAttr&FILE_ATTRIBUTE_REPARSE_POINT)
      {
        char JuncName[NM*2];
        DWORD LenJunction=GetJunctionPointInfo(SelName,JuncName,sizeof(JuncName));
        //"\??\D:\Junc\Src\" ��� "\\?\Volume{..."
        int offset = 0;
        if (!strncmp(JuncName,"\\??\\",4))
          offset = 4;

        AttrDlg[SETATTR_TITLE].Y2++;
        for(I=3; I  < DlgCountItems; ++I)
        {
          AttrDlg[I].Y1++;
          if (AttrDlg[I].Y2)
            AttrDlg[I].Y2++;
        }
        DlgCountItems++;
        JunctionPresent=TRUE;

        int ID_Msg, Width;
        if(!strncmp(JuncName+offset,"Volume{",7))
        {
          char JuncRoot[NM*2];
          JuncRoot[0]=JuncRoot[1]=0;
          GetPathRootOne(JuncName+offset,JuncRoot);
          if(JuncRoot[1] == ':')
            strcpy(JuncName+offset,JuncRoot);
          ID_Msg=MSetAttrVolMount;
          Width=38;
        }
        else
        {
          ID_Msg=MSetAttrJunction;
          Width=52;
        }

        sprintf(AttrDlg[SETATTR_TITLELINK].Data,MSG(ID_Msg),
              (LenJunction?
                  TruncPathStr(JuncName+offset,Width):
                  MSG(MSetAttrUnknownJunction)));

        /* $ 11.09.2001 SVS
            ��������� �� ������ ������������ �������� ������� �������� ��
            NTFS.
        */
        DlgParam.FileSystemFlags=0;
        GetPathRoot(SelName,JuncName);
        if (GetVolumeInformation(JuncName,NULL,0,NULL,NULL,&DlgParam.FileSystemFlags,NULL,0))
        {
          if (!(DlgParam.FileSystemFlags & FS_FILE_COMPRESSION))
            AttrDlg[SETATTR_COMPRESSED].Flags|=DIF_DISABLE;

          if (!IsCryptFileASupport || !(DlgParam.FileSystemFlags & FS_FILE_ENCRYPTION))
            AttrDlg[SETATTR_ENCRYPTED].Flags|=DIF_DISABLE;
        }
        /* SVS $ */
      }

      strcpy(AttrDlg[SETATTR_NAME].Data,SelName);
      TruncStr(AttrDlg[SETATTR_NAME].Data,54);

      AttrDlg[SETATTR_RO].Selected=(FileAttr & FA_RDONLY)!=0;
      AttrDlg[SETATTR_ARCHIVE].Selected=(FileAttr & FA_ARCH)!=0;
      AttrDlg[SETATTR_HIDDEN].Selected=(FileAttr & FA_HIDDEN)!=0;
      AttrDlg[SETATTR_SYSTEM].Selected=(FileAttr & FA_SYSTEM)!=0;
      AttrDlg[SETATTR_COMPRESSED].Selected=(FileAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
      AttrDlg[SETATTR_ENCRYPTED].Selected=(FileAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;
      AttrDlg[SETATTR_INDEXED].Selected=(FileAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)!=0;
      AttrDlg[SETATTR_SPARSE].Selected=(FileAttr & FILE_ATTRIBUTE_SPARSE_FILE)!=0;
      AttrDlg[SETATTR_TEMP].Selected=(FileAttr & FILE_ATTRIBUTE_TEMPORARY)!=0;

      if(DlgParam.Plugin)
      {
        ConvertDate(FindData.ftLastWriteTime,AttrDlg[SETATTR_MDATE].Data,AttrDlg[SETATTR_MTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
        ConvertDate(FindData.ftCreationTime,AttrDlg[SETATTR_CDATE].Data,AttrDlg[SETATTR_CTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
        ConvertDate(FindData.ftLastAccessTime,AttrDlg[SETATTR_ADATE].Data,AttrDlg[SETATTR_ATIME].Data,8,FALSE,FALSE,TRUE,TRUE);
      }
      else
      {
        HANDLE FindHandle;
        if ((FindHandle=FindFirstFile(SelName,&FindData))!=INVALID_HANDLE_VALUE)
        {
          FindClose(FindHandle);
          ConvertDate(FindData.ftLastWriteTime,AttrDlg[SETATTR_MDATE].Data,AttrDlg[SETATTR_MTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(FindData.ftCreationTime,AttrDlg[SETATTR_CDATE].Data,AttrDlg[SETATTR_CTIME].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(FindData.ftLastAccessTime,AttrDlg[SETATTR_ADATE].Data,AttrDlg[SETATTR_ATIME].Data,8,FALSE,FALSE,TRUE,TRUE);
        }
      }
    }
    else
    {
      AttrDlg[SETATTR_RO].Selected=AttrDlg[SETATTR_ARCHIVE].Selected=AttrDlg[SETATTR_HIDDEN].Selected=
      AttrDlg[SETATTR_SYSTEM].Selected=AttrDlg[SETATTR_COMPRESSED].Selected=AttrDlg[SETATTR_ENCRYPTED].Selected=
      AttrDlg[SETATTR_INDEXED].Selected=AttrDlg[SETATTR_SPARSE].Selected=AttrDlg[SETATTR_TEMP].Selected=2;
      AttrDlg[SETATTR_MDATE].Data[0]=AttrDlg[SETATTR_MTIME].Data[0]=AttrDlg[SETATTR_CDATE].Data[0]=
      AttrDlg[SETATTR_CTIME].Data[0]=AttrDlg[SETATTR_ADATE].Data[0]=AttrDlg[SETATTR_ATIME].Data[0]='\0';

      AttrDlg[SETATTR_ORIGINAL].Flags|=DIF_HIDDEN;
      AttrDlg[SETATTR_ORIGINAL].Flags&=~DIF_CENTERGROUP;

      strcpy(AttrDlg[SETATTR_NAME].Data,MSG(MSetAttrSelectedObjects));
      // �������� -1 - ����� ����� ���� ���� :-)
      for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
        AttrDlg[I].Selected=0;

      // �������� - ���� �� ����� ���������� - ��������?
      // ��� �� �������� �� ��������
      J=0;
      SrcPanel->GetSelName(NULL,FileAttr);
      while (SrcPanel->GetSelName(SelName,FileAttr,NULL,&FindData))
      {
        if(!J && (FileAttr & FA_DIREC))
        {
          FolderPresent=TRUE;
          AttrDlg[SETATTR_SUBFOLDERS].Flags&=~DIF_DISABLE;
          J++;
        }
        AttrDlg[SETATTR_RO].Selected+=(FileAttr & FA_RDONLY)?1:0;
        AttrDlg[SETATTR_ARCHIVE].Selected+=(FileAttr & FA_ARCH)?1:0;
        AttrDlg[SETATTR_HIDDEN].Selected+=(FileAttr & FA_HIDDEN)?1:0;
        AttrDlg[SETATTR_SYSTEM].Selected+=(FileAttr & FA_SYSTEM)?1:0;
        AttrDlg[SETATTR_COMPRESSED].Selected+=(FileAttr & FILE_ATTRIBUTE_COMPRESSED)?1:0;
        AttrDlg[SETATTR_ENCRYPTED].Selected+=(FileAttr & FILE_ATTRIBUTE_ENCRYPTED)?1:0;
        AttrDlg[SETATTR_INDEXED].Selected+=(FileAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)?1:0;
        AttrDlg[SETATTR_SPARSE].Selected+=(FileAttr & FILE_ATTRIBUTE_SPARSE_FILE)?1:0;
        AttrDlg[SETATTR_TEMP].Selected+=(FileAttr & FILE_ATTRIBUTE_TEMPORARY)?1:0;
      }
      SrcPanel->GetSelName(NULL,FileAttr);
      SrcPanel->GetSelName(SelName,FileAttr,NULL,&FindData);
      // �������� "�������������" ��� ��, ��� �����
      for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
      {
        J=AttrDlg[I].Selected;
        // ������� 3-state, ���� "���� ��� ��� ��� ������"
        // �� ����������� ������, ���� ���� ������ ����� ��������
        if((!J || J >= SelCount) && !FolderPresent)
          AttrDlg[I].Flags&=~DIF_3STATE;

        AttrDlg[I].Selected=(J >= SelCount)?1:(!J?0:2);
      }
    }

    // ��������� ��� ��������� ��� � 1.65?
    if(FolderPresent && !Opt.SetAttrFolderRules)
    {
      AttrDlg[SETATTR_SUBFOLDERS].Selected=1;
      AttrDlg[SETATTR_MDATE].Data[0]=AttrDlg[SETATTR_MTIME].Data[0]=AttrDlg[SETATTR_CDATE].Data[0]=
      AttrDlg[SETATTR_CTIME].Data[0]=AttrDlg[SETATTR_ADATE].Data[0]=AttrDlg[SETATTR_ATIME].Data[0]='\0';
      for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
      {
        AttrDlg[I].Selected=2;
        AttrDlg[I].Flags|=DIF_3STATE;
      }
    }

    // �������� ��������� ��������������.
    for(I=SETATTR_RO; I <= SETATTR_TEMP; ++I)
    {
      DlgParam.OriginalCBAttr[I-SETATTR_RO]=AttrDlg[I].Selected;
      DlgParam.OriginalCBAttr2[I-SETATTR_RO]=-1;
      DlgParam.OriginalCBFlag[I-SETATTR_RO]=AttrDlg[I].Flags;
    }

    DlgParam.ModeDialog=((SelCount==1 && (FileAttr & FA_DIREC)==0)?0:(SelCount==1?1:2));
    DlgParam.SelName=SelName;
    DlgParam.OStateF_12=AttrDlg[SETATTR_SUBFOLDERS].Selected;
    DlgParam.OStateC_8=AttrDlg[SETATTR_COMPRESSED].Selected;
    DlgParam.OStateE_9=AttrDlg[SETATTR_ENCRYPTED].Selected;

    // <Dialog>
    {
      Dialog Dlg(AttrDlg,DlgCountItems,SetAttrDlgProc,(LONG_PTR)&DlgParam);
      Dlg.SetHelp("FileAttrDlg");                 //  ^ - ��� ��������� ������!
      Dlg.SetPosition(-1,-1,69,JunctionPresent?23:22);
      Dlg.Process();
      if (Dlg.GetExitCode()!=SETATTR_SET)
        return 0;
    }
    // </Dialog>

    SetPreRedrawFunc(PR_ShellSetFileAttributesMsg);
    ShellSetFileAttributesMsg(SelCount==1?SelName:NULL);

    if (SelCount==1 && (FileAttr & FA_DIREC)==0)
    {
      if(IsFileWritable(SelName,FileAttr,TRUE,MSetAttrCannotFor) == 1)
      {
        int NewAttr;
        NewAttr=FileAttr & FA_DIREC;
        if (AttrDlg[SETATTR_RO].Selected)              NewAttr|=FA_RDONLY;
        if (AttrDlg[SETATTR_ARCHIVE].Selected)         NewAttr|=FA_ARCH;
        if (AttrDlg[SETATTR_HIDDEN].Selected)          NewAttr|=FA_HIDDEN;
        if (AttrDlg[SETATTR_SYSTEM].Selected)          NewAttr|=FA_SYSTEM;
        if (AttrDlg[SETATTR_COMPRESSED].Selected)      NewAttr|=FILE_ATTRIBUTE_COMPRESSED;
        if (AttrDlg[SETATTR_ENCRYPTED].Selected)       NewAttr|=FILE_ATTRIBUTE_ENCRYPTED;
        if (AttrDlg[SETATTR_INDEXED].Selected)         NewAttr|=FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;

        /*
        AY: �� � ����� ���������� �� ��������
        if(!(AttrDlg[SETATTR_SPARSE].Flags&DIF_DISABLE))
          if (AttrDlg[SETATTR_SPARSE].Selected)
            NewAttr|=FILE_ATTRIBUTE_SPARSE_FILE;
        if(!(AttrDlg[SETATTR_TEMP].Flags&DIF_DISABLE))
          if (AttrDlg[SETATTR_TEMP].Selected)
            NewAttr|=FILE_ATTRIBUTE_TEMPORARY;
        */


        SetWriteTime=DlgParam.OLastWriteTime && ReadFileTime(0,SelName,FileAttr,&LastWriteTime,AttrDlg[SETATTR_MDATE].Data,AttrDlg[SETATTR_MTIME].Data);
        SetCreationTime=DlgParam.OCreationTime && ReadFileTime(1,SelName,FileAttr,&CreationTime,AttrDlg[SETATTR_CDATE].Data,AttrDlg[SETATTR_CTIME].Data);
        SetLastAccessTime=DlgParam.OLastAccessTime && ReadFileTime(2,SelName,FileAttr,&LastAccessTime,AttrDlg[SETATTR_ADATE].Data,AttrDlg[SETATTR_ATIME].Data);
  //_SVS(SysLog("\n\tSetWriteTime=%d\n\tSetCreationTime=%d\n\tSetLastAccessTime=%d",SetWriteTime,SetCreationTime,SetLastAccessTime));
        if(SetWriteTime || SetCreationTime || SetLastAccessTime)
          SetWriteTimeRetCode=ESetFileTime(SelName,
                                           (SetWriteTime ? &LastWriteTime:NULL),
                                           (SetCreationTime ? &CreationTime:NULL),
                                           (SetLastAccessTime ? &LastAccessTime:NULL),
                                           FileAttr);
        else
          SetWriteTimeRetCode=TRUE;

  //      if(NewAttr != (FileAttr & (~FA_DIREC))) // ����� �� ���-���� ������???
        if(SetWriteTimeRetCode == 1) // ���� ����� ������� ���������...
        {
          if((NewAttr&FILE_ATTRIBUTE_COMPRESSED) && !(FileAttr&FILE_ATTRIBUTE_COMPRESSED))
            ESetFileCompression(SelName,1,FileAttr);
          else if(!(NewAttr&FILE_ATTRIBUTE_COMPRESSED) && (FileAttr&FILE_ATTRIBUTE_COMPRESSED))
            ESetFileCompression(SelName,0,FileAttr);

          if((NewAttr&FILE_ATTRIBUTE_ENCRYPTED) && !(FileAttr&FILE_ATTRIBUTE_ENCRYPTED))
            ESetFileEncryption(SelName,1,FileAttr);
          else if(!(NewAttr&FILE_ATTRIBUTE_ENCRYPTED) && (FileAttr&FILE_ATTRIBUTE_ENCRYPTED))
            ESetFileEncryption(SelName,0,FileAttr);

          ESetFileAttributes(SelName,NewAttr&(~(FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_COMPRESSED)));
        }
      }
    }

    /* Multi *********************************************************** */
    else
    {
      int RetCode=1;
      ConsoleTitle *SetAttrTitle= new ConsoleTitle(MSG(MSetAttrTitle));
      int SetAttr,ClearAttr,Cancel=0;
      CtrlObject->Cp()->GetAnotherPanel(SrcPanel)->CloseFile();

      SetAttr=0;  ClearAttr=0;

      if (AttrDlg[SETATTR_RO].Selected == 1)         SetAttr|=FA_RDONLY;
      else if (!AttrDlg[SETATTR_RO].Selected)        ClearAttr|=FA_RDONLY;
      if (AttrDlg[SETATTR_ARCHIVE].Selected == 1)         SetAttr|=FA_ARCH;
      else if (!AttrDlg[SETATTR_ARCHIVE].Selected)        ClearAttr|=FA_ARCH;
      if (AttrDlg[SETATTR_HIDDEN].Selected == 1)         SetAttr|=FA_HIDDEN;
      else if (!AttrDlg[SETATTR_HIDDEN].Selected)        ClearAttr|=FA_HIDDEN;
      if (AttrDlg[SETATTR_SYSTEM].Selected == 1)         SetAttr|=FA_SYSTEM;
      else if (!AttrDlg[SETATTR_SYSTEM].Selected)        ClearAttr|=FA_SYSTEM;

      if (AttrDlg[SETATTR_COMPRESSED].Selected == 1)
      {
        SetAttr|=FILE_ATTRIBUTE_COMPRESSED;
        ClearAttr|=FILE_ATTRIBUTE_ENCRYPTED;
      }
      else if (!AttrDlg[SETATTR_COMPRESSED].Selected)
        ClearAttr|=FILE_ATTRIBUTE_COMPRESSED;

      if (AttrDlg[SETATTR_ENCRYPTED].Selected == 1)
      {
        SetAttr|=FILE_ATTRIBUTE_ENCRYPTED;
        ClearAttr|=FILE_ATTRIBUTE_COMPRESSED;
      }
      else if (!AttrDlg[SETATTR_ENCRYPTED].Selected)
        ClearAttr|=FILE_ATTRIBUTE_ENCRYPTED;

      if (AttrDlg[SETATTR_INDEXED].Selected == 1)        SetAttr|=FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
      else if (!AttrDlg[SETATTR_INDEXED].Selected)       ClearAttr|=FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;

      /*
      AY: �� � ����� ���������� �� ��������
      if(!(AttrDlg[SETATTR_SPARSE].Flags&DIF_DISABLE))
      {
        if (AttrDlg[SETATTR_SPARSE].Selected == 1)        SetAttr|=FILE_ATTRIBUTE_SPARSE_FILE;
        else if (!AttrDlg[SETATTR_SPARSE].Selected)       ClearAttr|=FILE_ATTRIBUTE_SPARSE_FILE;
      }

      if(!(AttrDlg[SETATTR_TEMP].Flags&DIF_DISABLE))
      {
        if (AttrDlg[SETATTR_TEMP].Selected == 1)        SetAttr|=FILE_ATTRIBUTE_TEMPORARY;
        else if (!AttrDlg[SETATTR_TEMP].Selected)       ClearAttr|=FILE_ATTRIBUTE_TEMPORARY;
      }
      */

      SrcPanel->GetSelName(NULL,FileAttr);

      while (SrcPanel->GetSelName(SelName,FileAttr,NULL,&FindData) && !Cancel)
      {
//_SVS(SysLog("SelName='%s'\n\tFileAttr =0x%08X\n\tSetAttr  =0x%08X\n\tClearAttr=0x%08X\n\tResult   =0x%08X",
//    SelName,FileAttr,SetAttr,ClearAttr,((FileAttr|SetAttr)&(~ClearAttr))));
        ShellSetFileAttributesMsg(SelName);

        if (CheckForEsc())
          break;

        RetCode=IsFileWritable(SelName,FileAttr,TRUE,MSetAttrCannotFor);
        if(!RetCode)
          break;
        if(RetCode == 2)
          continue;

        SetWriteTime=DlgParam.OLastWriteTime && ReadFileTime(0,SelName,FileAttr,&LastWriteTime,AttrDlg[SETATTR_MDATE].Data,AttrDlg[SETATTR_MTIME].Data);
        SetCreationTime=DlgParam.OCreationTime && ReadFileTime(1,SelName,FileAttr,&CreationTime,AttrDlg[SETATTR_CDATE].Data,AttrDlg[SETATTR_CTIME].Data);
        SetLastAccessTime=DlgParam.OLastAccessTime && ReadFileTime(2,SelName,FileAttr,&LastAccessTime,AttrDlg[SETATTR_ADATE].Data,AttrDlg[SETATTR_ATIME].Data);
        if(!(FileAttr&FILE_ATTRIBUTE_REPARSE_POINT) && (SetWriteTime || SetCreationTime || SetLastAccessTime))
        {
          //if(strstr(DlgParam.FSysName,"FAT") && (FileAttr&FA_DIREC))
          //  RetCode=1;
          //else
            RetCode=ESetFileTime(SelName,
                 (SetWriteTime ? &LastWriteTime:NULL),
                 (SetCreationTime ? &CreationTime:NULL),
                 (SetLastAccessTime ? &LastAccessTime:NULL),
                 FileAttr);
          if(!RetCode)
            break;
          if(RetCode == 2)
            continue;
        }
        if(((FileAttr|SetAttr)&(~ClearAttr)) != FileAttr)
        {
          if (AttrDlg[SETATTR_COMPRESSED].Selected != 2)
          {
            RetCode=ESetFileCompression(SelName,AttrDlg[SETATTR_COMPRESSED].Selected,FileAttr);
            if(!RetCode) // ������� ����� :-(
              break;
            if(RetCode == 2)
              continue;
          }
          if (AttrDlg[SETATTR_ENCRYPTED].Selected != 2) // +E -C
          {
            if(AttrDlg[SETATTR_COMPRESSED].Selected != 1)
            {
              RetCode=ESetFileEncryption(SelName,AttrDlg[SETATTR_ENCRYPTED].Selected,FileAttr);
              if(!RetCode) // ������� ����������� :-(
                break;
              if(RetCode == 2)
                continue;
            }
          }
          RetCode=ESetFileAttributes(SelName,((FileAttr|SetAttr)&(~ClearAttr)));
          if(!RetCode)
            break;
          if(RetCode == 2)
            continue;
        }

        if ((FileAttr & FA_DIREC) && AttrDlg[SETATTR_SUBFOLDERS].Selected)
        {
          char FullName[NM];
          ScanTree ScTree(FALSE,TRUE,-1,TRUE);

          ScTree.SetFindPath(SelName,"*.*");
          while (ScTree.GetNextName(&FindData,FullName, sizeof (FullName)-1))
          {
            ShellSetFileAttributesMsg(FullName);
            if (CheckForEsc())
            {
              Cancel=1;
              break;
            }

            RetCode=IsFileWritable(FullName,FindData.dwFileAttributes,TRUE,MSetAttrCannotFor);
            if(!RetCode)
            {
              Cancel=1;
              break;
            }
            if(RetCode == 2)
              continue;

            SetWriteTime=DlgParam.OLastWriteTime && ReadFileTime(0,FullName,FindData.dwFileAttributes,&LastWriteTime,AttrDlg[SETATTR_MDATE].Data,AttrDlg[SETATTR_MTIME].Data);
            SetCreationTime=DlgParam.OCreationTime && ReadFileTime(1,FullName,FindData.dwFileAttributes,&CreationTime,AttrDlg[SETATTR_CDATE].Data,AttrDlg[SETATTR_CTIME].Data);
            SetLastAccessTime=DlgParam.OLastAccessTime && ReadFileTime(2,FullName,FindData.dwFileAttributes,&LastAccessTime,AttrDlg[SETATTR_ADATE].Data,AttrDlg[SETATTR_ATIME].Data);
            if(!(FindData.dwFileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) && (SetWriteTime || SetCreationTime || SetLastAccessTime))
            {
              //if(strstr(DlgParam.FSysName,"FAT") && (FileAttr&FA_DIREC))
              //  RetCode=1;
              //else
                RetCode=ESetFileTime(FullName,SetWriteTime ? &LastWriteTime:NULL,
                           SetCreationTime ? &CreationTime:NULL,
                           SetLastAccessTime ? &LastAccessTime:NULL,
                           FindData.dwFileAttributes);
              if(RetCode == 0)
              {
                Cancel=1;
                break;
              }
              if(RetCode == 2)
                continue;
            }
            if(((FindData.dwFileAttributes|SetAttr)&(~ClearAttr)) !=
                 FindData.dwFileAttributes)
            {
              if (AttrDlg[SETATTR_COMPRESSED].Selected != 2)
              {
                RetCode=ESetFileCompression(FullName,AttrDlg[SETATTR_COMPRESSED].Selected,FindData.dwFileAttributes);
                if(RetCode == 0)
                {
                  Cancel=1;
                  break; // ������� ����� :-(
                }
                if(RetCode == 2)
                  continue;
              }
              if (AttrDlg[SETATTR_ENCRYPTED].Selected != 2) // +E -C
              {
                if(AttrDlg[SETATTR_COMPRESSED].Selected != 1)
                {
                  RetCode=ESetFileEncryption(FullName,AttrDlg[SETATTR_ENCRYPTED].Selected,FindData.dwFileAttributes);
                  if (RetCode == 0)
                  {
                    Cancel=1;
                    break; // ������� ����������� :-(
                  }
                  if(RetCode == 2)
                    continue;
                }
              }
              RetCode=ESetFileAttributes(FullName,(FindData.dwFileAttributes|SetAttr)&(~ClearAttr));
              if (RetCode == 0)
              {
                Cancel=1;
                break;
              }
              if(RetCode == 2)
                continue;
            }
          }
        }
      } // END: while (SrcPanel->GetSelName(...))
      delete SetAttrTitle;
    }
    SetPreRedrawFunc(NULL);
  }

  SrcPanel->SaveSelection();
  SrcPanel->Update(UPDATE_KEEP_SELECTION);
  SrcPanel->ClearSelection();
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
  CtrlObject->Cp()->Redraw();
  return 1;
}

// ���������� 0 - ������, 1 - ��, 2 - Skip
static int IsFileWritable(const char *Name, DWORD FileAttr, BOOL IsShowErrMsg, int Msg)
{
  if ((FileAttr & FA_DIREC) && WinVer.dwPlatformId!=VER_PLATFORM_WIN32_NT)
    return 1;

  while (1)
  {
    if (FileAttr & FA_RDONLY)
      SetFileAttributes(Name,FileAttr & ~FA_RDONLY);

    HANDLE hFile=FAR_CreateFile(Name,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,(FileAttr & FA_DIREC) ? FILE_FLAG_BACKUP_SEMANTICS:0,NULL);
    BOOL Writable=TRUE;
    if(hFile == INVALID_HANDLE_VALUE)
      Writable=FALSE;
    else
      CloseHandle(hFile);

    DWORD LastError=GetLastError();
    if (FileAttr & FA_RDONLY)
      SetFileAttributes(Name,FileAttr);
    SetLastError(LastError);

    if (Writable)
      break;

    int Code;
    if(IsShowErrMsg)
        Code=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                     MSG(Msg),(char *)Name,
                     MSG(MHRetry),MSG(MHSkip),MSG(MHCancel));
    else
       return 0;

    if (Code<0)
      return 0;
    if(Code == 1)
      return 2;
    if(Code == 2)
      return 0;
  }
  return 1;
}
