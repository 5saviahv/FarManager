/*
setattr.cpp

��⠭���� ��ਡ�⮢ 䠩���

*/

/* Revision: 1.18 28.02.2001 $ */

/*
Modify:
  28.02.2001 SVS
    - ���� � Win2K � �������᪫�祭�ﬨ ���⮣� � ���஢������ ��ਡ��
    + ���⠢�塞 ��������� ���᮫� �� �६� ����� ��⠭���� ��ਡ�⮢
    + � ��ᠣ� ����� ��⠭���� �⮡ࠦ��� ⥪�酅 䠩��.
  30.01.2001 SVS
    ! ᭨���� 3-state, �᫨ "���� �� ��� ��� ��祣�"
      �� �᪫�祭��� ����, �᫨ ���� ������ �।� ��ꥪ⮢
  23.01.2001 SVS
    + ������� ��⨬���樨 ���� :-)
  22.01.2001 SVS
    ! ShellSetFileAttributes ⥯��� �����頥� १���� � ���� TRUE ��� FALSE
    + �᫨ �� ���������� ������, � ��ᬮ�ਬ �� OPIF_REALNAMES
  22.01.2001 SVS
    + ����� ��⥫���㠫쭮�� ������� ��⠭���� ��ਡ�⮢ !!!! :-)))
      ������, ��� ���� Multi, �᫨ ���� ����� ���騥 ��ਡ���, �
      ��� ����砫쭮 ���樠���������� ��� ���� - ���� [x] ���� [ ] ����
      [*] ��� ���� �᫨ "�� ��"
  14.01.2001 SVS
    + ��ࠡ�⪠ ����, �᫨ ��� SymLink
  04.01.2001 SVS
    - ���� � ������� 䠩��� - ��८�⨬���஢�� ;-(
  03.01.2001 SVS
    ! �᪮ਬ ����� �� ��� "����ࠡ�⪨" �������� ��ਡ��
    - ���� � ���室��� ����� ����஫���
  03.01.2001 SVS
    ! ���� ����� ������� ��ਡ�⮢ - ���� ��⥫���㠫�� ������ ��
      �� ��砨 ����� :-)
  30.12.2000 SVS
    ! �㭪樨 ��� ࠡ��� � 䠩���묨 ��ਡ�⠬� �뭥ᥭ� � fileattr.cpp
  21.12.2000 SVS
    ! �᫨ ����� ����, � ����祭�� "Process subfolders" �� ��頥�
      ������� � ��ਡ�⠬�.
  14.12.2000 SVS
    ! �����뢠�� �������騥 ��ਡ���, �� ������ �� ������㯭묨.
  24.11.2000 SVS
    + �ࠢ��� �� ��� ��⠭���� ��ਡ�⮢ �� ��⠫���
  16.11.2000 SVS
    ! ���ᨢ� ��� ��᮪ ����� ����ﭭ� ���� �ய�᪨ - ������ ��� static
  11.11.2000 SVS
    - "᫮�����" � �ਯ⮢����� :-))))
  02.11.2000 SVS
    - ��ࠢ�塞 ���� :-)
  20.10.2000 SVS
    + ���� ��ਡ�� Encripted (NTFS/Win2K)
  14.08.2000 KM
    ! �������� ���樠������ ������� � ���⮬ ���������⥩ Mask.
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */

int OriginalCBAttr0[16]; // ���祭�� CheckBox`�� �� ������ ���� �������
int OriginalCBAttr[16]; // ���祭�� CheckBox`�� �� ������ ���� �������
int OriginalCBAttr2[16]; //
DWORD OriginalCBFlag[16];

static int ReadFileTime(FILETIME *FileTime,char *SrcDate,char *SrcTime)
{
  FILETIME ft;
  SYSTEMTIME st;
  int DateN[3],TimeN[3],DigitCount,I;
  DateN[0]=DateN[1]=DateN[2]=0;
  for (I=0;I<sizeof(DateN)/sizeof(DateN[0]) && *SrcDate;I++)
  {
    DateN[I]=atoi(SrcDate);
    while (isdigit(*SrcDate))
      SrcDate++;
    while (*SrcDate && !isdigit(*SrcDate))
      SrcDate++;
  }
  TimeN[0]=TimeN[1]=TimeN[2]=0;
  for (DigitCount=I=0;I<sizeof(TimeN)/sizeof(TimeN[0]) && *SrcTime;I++)
  {
    TimeN[I]=atoi(SrcTime);
    while (isdigit(*SrcTime))
    {
      SrcTime++;
      DigitCount++;
    }
    while (*SrcTime && !isdigit(*SrcTime))
      SrcTime++;
  }
  if (DigitCount==0)
    return(FALSE);
  switch(GetDateFormat())
  {
    case 0:
      st.wMonth=DateN[0];
      st.wDay=DateN[1];
      st.wYear=DateN[2];;
      break;
    case 1:
      st.wDay=DateN[0];
      st.wMonth=DateN[1];
      st.wYear=DateN[2];;
      break;
    default:
      st.wYear=DateN[0];;
      st.wMonth=DateN[1];
      st.wDay=DateN[2];
      break;
  }
  if (st.wDay==0 || st.wMonth==0)
    return(FALSE);
  st.wHour=TimeN[0];
  st.wMinute=TimeN[1];
  st.wSecond=TimeN[2];
  st.wDayOfWeek=st.wMilliseconds=0;
  if (st.wYear<100)
    if (st.wYear<80)
      st.wYear+=2000;
    else
      st.wYear+=1900;
  SystemTimeToFileTime(&st,&ft);
  LocalFileTimeToFileTime(&ft,FileTime);
  return(TRUE);
}


static void IncludeExcludeAttrib(int FocusPos,struct DialogItem *Item, int FocusPosSet, int FocusPosSkip)
{
  if(FocusPos == FocusPosSet && Item[FocusPosSet].Selected && Item[FocusPosSkip].Selected)
    Item[FocusPosSkip].Selected=0;
  if(FocusPos == FocusPosSkip && Item[FocusPosSkip].Selected && Item[FocusPosSet].Selected)
    Item[FocusPosSet].Selected=0;
}


static void EmptyDialog(struct DialogItem *AttrDlg,int ClrAttr,int SelCount1)
{
  if(ClrAttr)
  {
    AttrDlg[4].Selected=
    AttrDlg[5].Selected=
    AttrDlg[6].Selected=
    AttrDlg[7].Selected=
    AttrDlg[8].Selected=
    AttrDlg[9].Selected=SelCount1?0:2;
  }

  AttrDlg[16].Data[0]=
  AttrDlg[17].Data[0]=
  AttrDlg[19].Data[0]=
  AttrDlg[20].Data[0]=
  AttrDlg[22].Data[0]=
  AttrDlg[23].Data[0]='\0';
}

/* $ 22.11.2000 SVS
   ���������� �����
*/
static void FillFileldDir(char *SelName,int FileAttr,
                          struct DialogItem *AttrDlg,
                          int SetAttr)
{
  HANDLE FindHandle;
  WIN32_FIND_DATA FindData;
  if ((FindHandle=FindFirstFile(SelName,&FindData))!=INVALID_HANDLE_VALUE)
  {
    FindClose(FindHandle);
    ConvertDate(&FindData.ftLastWriteTime, AttrDlg[16].Data,AttrDlg[17].Data,8,FALSE,FALSE,TRUE,TRUE);
    ConvertDate(&FindData.ftCreationTime,  AttrDlg[19].Data,AttrDlg[20].Data,8,FALSE,FALSE,TRUE,TRUE);
    ConvertDate(&FindData.ftLastAccessTime,AttrDlg[22].Data,AttrDlg[23].Data,8,FALSE,FALSE,TRUE,TRUE);
  }
  if(SetAttr)
  {
    AttrDlg[4].Selected=(FileAttr & FA_RDONLY)!=0;
    AttrDlg[5].Selected=(FileAttr & FA_ARCH)!=0;
    AttrDlg[6].Selected=(FileAttr & FA_HIDDEN)!=0;
    AttrDlg[7].Selected=(FileAttr & FA_SYSTEM)!=0;
    AttrDlg[8].Selected=(FileAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
    AttrDlg[9].Selected=(FileAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;
  }
}
/* SVS $ */


// ��ࠡ��稪 ������� - ���� �� �⫮� ����⨩ �㦭�� ������.
long WINAPI SetAttrDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  if(Msg == DN_BTNCLICK)
  {
    if(Param1 >= 4 && Param2 <= 9)
    {
      OriginalCBAttr[Param1-4] = Param2;
      OriginalCBAttr2[Param1-4] = 0;
    }
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

int ShellSetFileAttributes(Panel *SrcPanel)
{
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
/*MSetAttrJunction
00                                             00
01   ������������� Attributes ������������ͻ   01
02   �     Change file attributes for      �   02
03   �                 foo                 �   03
04   �          Link: blach blach          � < 04 <<
04   �������������������������������������Ķ   05
05   � [ ] Read only                       �   06
06   � [ ] Archive                         �   07
07   � [ ] Hidden                          �   08
08   � [ ] System                          �   09
09   � [ ] Compressed                      �   10
10   � [ ] Encrypted                       �   11
11   �������������������������������������Ķ   12
12   � [x] Process subfolders              �   13
13   �������������������������������������Ķ   14
14   �  File time      DD.MM.YYYY hh:mm:ss �   15
15   � Modification      .  .       :  :   �   16
16   � Creation          .  .       :  :   �   17
17   � Last access       .  .       :  :   �   18
18   �                 [ Current ]         �   19
19   �������������������������������������Ķ   20
20   �         [ Set ]  [ Cancel ]         �   21
21   �������������������������������������ͼ   22
22                                             23
*/
  static struct DialogData AttrDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,41,21,0,0,0,0,(char *)MSetAttrTitle,
  /* 01 */DI_TEXT,-1,2,0,0,0,0,0,0,(char *)MSetAttrFor,
  /* 02 */DI_TEXT,-1,3,0,0,0,0,DIF_SHOWAMPERSAND,0,"",
  /* 03 */DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */DI_CHECKBOX,5, 5,0,0,1,0,DIF_3STATE,0,(char *)MSetAttrRO,
  /* 05 */DI_CHECKBOX,5, 6,0,0,0,0,DIF_3STATE,0,(char *)MSetAttrArchive,
  /* 06 */DI_CHECKBOX,5, 7,0,0,0,0,DIF_3STATE,0,(char *)MSetAttrHidden,
  /* 07 */DI_CHECKBOX,5, 8,0,0,0,0,DIF_3STATE,0,(char *)MSetAttrSystem,
  /* 08 */DI_CHECKBOX,5, 9,0,0,0,0,DIF_3STATE,0,(char *)MSetAttrCompressed,
  /* 09 */DI_CHECKBOX,5,10,0,0,0,0,DIF_3STATE,0,(char *)MSetAttrEncrypted,
  /* 10 */DI_TEXT,3,11,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 11 */DI_CHECKBOX,5,12,0,0,0,0,DIF_DISABLE,0,(char *)MSetAttrSubfolders,
  /* 12 */DI_TEXT,3,13,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 13 */DI_TEXT,6,14,0,0,0,0,DIF_BOXCOLOR,0,(char *)MSetAttrFileTime,
  /* 14 */DI_TEXT,21,14,0,0,0,0,0,0,"",
  /* 15 */DI_TEXT,    5,15,0,0,0,0,0,0,(char *)MSetAttrModification,
  /* 16 */DI_FIXEDIT,21,15,30,15,0,0,DIF_MASKEDIT,0,"",
  /* 17 */DI_FIXEDIT,32,15,39,15,0,0,DIF_MASKEDIT,0,"",
  /* 18 */DI_TEXT,    5,16,0,0,0,0,0,0,(char *)MSetAttrCreation,
  /* 19 */DI_FIXEDIT,21,16,30,16,0,0,DIF_MASKEDIT,0,"",
  /* 20 */DI_FIXEDIT,32,16,39,16,0,0,DIF_MASKEDIT,0,"",
  /* 21 */DI_TEXT,    5,17,0,0,0,0,0,0,(char *)MSetAttrLastAccess,
  /* 22 */DI_FIXEDIT,21,17,30,17,0,0,DIF_MASKEDIT,0,"",
  /* 23 */DI_FIXEDIT,32,17,39,17,0,0,DIF_MASKEDIT,0,"",
  /* 24 */DI_BUTTON,21,18,0,0,0,0,0,0,(char *)MSetAttrCurrent,
  /* 25 */DI_TEXT,3,19,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 26 */DI_BUTTON,0,20,0,0,0,0,DIF_CENTERGROUP,1,(char *)MSetAttrSet,
  /* 27 */DI_BUTTON,0,20,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel,
  /* 28 */DI_TEXT,-1,4,0,0,0,0,DIF_SHOWAMPERSAND,0,"",
  };
  MakeDialogItems(AttrDlgData,AttrDlg);
  int DlgCountItems=sizeof(AttrDlgData)/sizeof(AttrDlgData[0])-1;

  DWORD FileSystemFlags;
  int SelCount, I, J;

  if((SelCount=SrcPanel->GetSelCount())==0)
    return 0;

  if (SrcPanel->GetMode()==PLUGIN_PANEL)
  {
    struct OpenPluginInfo Info;
    HANDLE hPlugin=SrcPanel->GetPluginHandle();
    if(hPlugin == INVALID_HANDLE_VALUE)
      return 0;

    CtrlObject->Plugins.GetOpenPluginInfo(hPlugin,&Info);
    if(!(Info.Flags & OPIF_REALNAMES))
      return 0;
  }

  FileSystemFlags=0;
  if (GetVolumeInformation(NULL,NULL,0,NULL,NULL,&FileSystemFlags,NULL,0))
  {
    if (!(FileSystemFlags & FS_FILE_COMPRESSION))
      AttrDlg[8].Flags|=DIF_DISABLE;

    if (!IsCryptFileASupport || !(FileSystemFlags & FS_FILE_ENCRYPTION))
      AttrDlg[9].Flags|=DIF_DISABLE;
  }

  {
    char SelName[NM];
    int FileAttr;
    SaveScreen SaveScr;

    SrcPanel->GetSelName(NULL,FileAttr);
    SrcPanel->GetSelName(SelName,FileAttr);

    if (SelCount==0 || SelCount==1 && strcmp(SelName,"..")==0)
      return 0;

    int FocusPos;
    int NewAttr;
    int FolderPresent=FALSE, JunctionPresent=FALSE;
    char TimeText[6][100];

    int DateSeparator=GetDateSeparator();
    int TimeSeparator=GetTimeSeparator();
    static char DMask[20],TMask[20];

    sprintf(TMask,"99%c99%c99",TimeSeparator,TimeSeparator);
    switch(GetDateFormat())
    {
      case 0:
        sprintf(AttrDlg[14].Data,MSG(MSetAttrTimeTitle1),DateSeparator,DateSeparator,TimeSeparator,TimeSeparator);
        sprintf(DMask,"99%c99%c9999",DateSeparator,DateSeparator);
        break;
      case 1:
        sprintf(AttrDlg[14].Data,MSG(MSetAttrTimeTitle2),DateSeparator,DateSeparator,TimeSeparator,TimeSeparator);
        sprintf(DMask,"99%c99%c9999",DateSeparator,DateSeparator);
        break;
      default:
        sprintf(AttrDlg[14].Data,MSG(MSetAttrTimeTitle3),DateSeparator,DateSeparator,TimeSeparator,TimeSeparator);
        sprintf(DMask,"9999%c99%c99",DateSeparator,DateSeparator);
        break;
    }

    AttrDlg[16].Mask=DMask;
    AttrDlg[17].Mask=TMask;
    AttrDlg[19].Mask=DMask;
    AttrDlg[20].Mask=TMask;
    AttrDlg[22].Mask=DMask;
    AttrDlg[23].Mask=TMask;

    if (SelCount==1)
    {
      if((FileAttr & FA_DIREC))
      {
        AttrDlg[11].Flags&=~DIF_DISABLE;
        AttrDlg[11].Selected=Opt.SetAttrFolderRules == 1?0:1;
        if(Opt.SetAttrFolderRules)
        {
          FillFileldDir(SelName,FileAttr,AttrDlg,1);
          // 㡨ࠥ� 3-State
          for(I=4; I <= 9; ++I)
            AttrDlg[I].Flags&=~DIF_3STATE;
        }
        FolderPresent=TRUE;

        // ��ࠡ�⪠ ����, �᫨ ��� SymLink
        if(FileAttr&FILE_ATTRIBUTE_REPARSE_POINT)
        {
          char JuncName[NM*2];
          DWORD LenJunction=GetJunctionPointInfo(SelName,JuncName,sizeof(JuncName));
          if(LenJunction)
          {
             //"\??\D:\Junc\Src\"
             sprintf(AttrDlg[28].Data,MSG(MSetAttrJunction),TruncPathStr(JuncName+4,29));
             AttrDlg[0].Y2++;
             for(I=3; I  < DlgCountItems; ++I)
             {
               AttrDlg[I].Y1++;
               AttrDlg[I].Y2++;
             }
             DlgCountItems++;
             JunctionPresent=TRUE;
          }
        }
      }
      else
      {
        // 㡨ࠥ� 3-State
        for(I=4; I <= 9; ++I)
          AttrDlg[I].Flags&=~DIF_3STATE;
      }

      strcpy(AttrDlg[2].Data,SelName);
      TruncStr(AttrDlg[2].Data,30);

      AttrDlg[4].Selected=(FileAttr & FA_RDONLY)!=0;
      AttrDlg[5].Selected=(FileAttr & FA_ARCH)!=0;
      AttrDlg[6].Selected=(FileAttr & FA_HIDDEN)!=0;
      AttrDlg[7].Selected=(FileAttr & FA_SYSTEM)!=0;
      AttrDlg[8].Selected=(FileAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
      AttrDlg[9].Selected=(FileAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;

      {
        HANDLE FindHandle;
        WIN32_FIND_DATA FindData;
        if ((FindHandle=FindFirstFile(SelName,&FindData))!=INVALID_HANDLE_VALUE)
        {
          FindClose(FindHandle);
          ConvertDate(&FindData.ftLastWriteTime,AttrDlg[16].Data,AttrDlg[17].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(&FindData.ftCreationTime,AttrDlg[19].Data,AttrDlg[20].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(&FindData.ftLastAccessTime,AttrDlg[22].Data,AttrDlg[23].Data,8,FALSE,FALSE,TRUE,TRUE);
        }
      }
      strcpy(TimeText[0],AttrDlg[16].Data);
      strcpy(TimeText[1],AttrDlg[17].Data);
      strcpy(TimeText[2],AttrDlg[19].Data);
      strcpy(TimeText[3],AttrDlg[20].Data);
      strcpy(TimeText[4],AttrDlg[22].Data);
      strcpy(TimeText[5],AttrDlg[23].Data);
    }
    else
    {
      EmptyDialog(AttrDlg,1,0);

      strcpy(AttrDlg[2].Data,MSG(MSetAttrSelectedObjects));
      // ���⠢�� -1 - ��⮬ ��⥬ ��� 䠪� :-)
      for(I=4; I <= 9; ++I)
        AttrDlg[I].Selected=0;

      // �஢�ઠ - ���� �� �।� �뤥������ - ��⠫���?
      // ⠪ �� �஢�ઠ �� ��ਡ���
      J=0;
      SrcPanel->GetSelName(NULL,FileAttr);
      while (SrcPanel->GetSelName(SelName,FileAttr))
      {
        if(!J && (FileAttr & FA_DIREC))
        {
          FolderPresent=TRUE;
          AttrDlg[11].Flags&=~DIF_DISABLE;
          J++;
        }
        AttrDlg[4].Selected+=(FileAttr & FA_RDONLY)?1:0;
        AttrDlg[5].Selected+=(FileAttr & FA_ARCH)?1:0;
        AttrDlg[6].Selected+=(FileAttr & FA_HIDDEN)?1:0;
        AttrDlg[7].Selected+=(FileAttr & FA_SYSTEM)?1:0;
        AttrDlg[8].Selected+=(FileAttr & FILE_ATTRIBUTE_COMPRESSED)?1:0;
        AttrDlg[9].Selected+=(FileAttr & FILE_ATTRIBUTE_ENCRYPTED)?1:0;
      }
      SrcPanel->GetSelName(NULL,FileAttr);
      SrcPanel->GetSelName(SelName,FileAttr);
      // ���⠢�� "����।������" ��� �, �� �㦭�
      for(I=4; I <= 9; ++I)
      {
        J=AttrDlg[I].Selected;
        // ᭨���� 3-state, �᫨ "���� �� ��� ��� ��祣�"
        // �� �᪫�祭��� ����, �᫨ ���� ������ �।� ��ꥪ⮢
        if((!J || J >= SelCount) && !FolderPresent)
          AttrDlg[I].Flags&=~DIF_3STATE;

        AttrDlg[I].Selected=(J >= SelCount)?1:(!J?0:2);
      }
    }

    // �������� ���ﭨ� ��४���⥫��.
    for(I=4; I <= 9; ++I)
    {
      OriginalCBAttr0[I-4]=OriginalCBAttr[I-4]=AttrDlg[I].Selected;
      OriginalCBAttr2[I-4]=-1;
      OriginalCBFlag[I-4]=AttrDlg[I].Flags;
    }

    if (SelCount==1 && (FileAttr & FA_DIREC)==0)
    {
      int NewAttr;

      {
        Dialog Dlg(AttrDlg,DlgCountItems);
        Dlg.SetHelp("FileAttrDlg");
        Dlg.SetPosition(-1,-1,45,JunctionPresent?24:23);

        while (1)
        {
          Dlg.Show();
          while (!Dlg.Done())
          {
            Dlg.ReadInput();
            Dlg.ProcessInput();
            FocusPos=Dialog::SendDlgMessage((HANDLE)&Dlg,DM_GETFOCUS,0,0);
            if(((FileSystemFlags & (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION))==
                 (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION)) &&
               (FocusPos == 8 || FocusPos == 9))
            {
              IncludeExcludeAttrib(FocusPos,AttrDlg,8,9);
              Dlg.FastShow();
            }
          }
          Dlg.GetDialogObjectsData();
          if (Dlg.GetExitCode()!=24)
            break;
          FILETIME ft;
          GetSystemTimeAsFileTime(&ft);
          ConvertDate(&ft,AttrDlg[16].Data,AttrDlg[17].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(&ft,AttrDlg[19].Data,AttrDlg[20].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(&ft,AttrDlg[22].Data,AttrDlg[23].Data,8,FALSE,FALSE,TRUE,TRUE);
          if (AttrDlg[24].Focus)
          {
            AttrDlg[24].Focus=0;
            AttrDlg[16].Focus=1;
          }
          Dlg.ClearDone();
          Dlg.InitDialogObjects();
        }

        if (Dlg.GetExitCode()!=26)
          return 0;

        Dlg.Hide();
      }

      NewAttr=FileAttr & FA_DIREC;
      if (AttrDlg[4].Selected)        NewAttr|=FA_RDONLY;
      if (AttrDlg[5].Selected)        NewAttr|=FA_ARCH;
      if (AttrDlg[6].Selected)        NewAttr|=FA_HIDDEN;
      if (AttrDlg[7].Selected)        NewAttr|=FA_SYSTEM;
      if (AttrDlg[8].Selected)        NewAttr|=FILE_ATTRIBUTE_COMPRESSED;
      if (AttrDlg[9].Selected)        NewAttr|=FILE_ATTRIBUTE_ENCRYPTED;

      Message(0,0,MSG(MSetAttrTitle),MSG(MSetAttrSetting));

      if (!strcmp(TimeText[0],AttrDlg[16].Data) ||
          !strcmp(TimeText[1],AttrDlg[17].Data) ||
          !strcmp(TimeText[2],AttrDlg[19].Data) ||
          !strcmp(TimeText[3],AttrDlg[20].Data) ||
          !strcmp(TimeText[4],AttrDlg[22].Data) ||
          !strcmp(TimeText[5],AttrDlg[23].Data))
      {
        FILETIME LastWriteTime,CreationTime,LastAccessTime;
        int SetWriteTime,SetCreationTime,SetLastAccessTime;
        SetWriteTime=ReadFileTime(&LastWriteTime,AttrDlg[16].Data,AttrDlg[17].Data);
        SetCreationTime=ReadFileTime(&CreationTime,AttrDlg[19].Data,AttrDlg[20].Data);
        SetLastAccessTime=ReadFileTime(&LastAccessTime,AttrDlg[22].Data,AttrDlg[23].Data);
        ESetFileTime(SelName,SetWriteTime ? &LastWriteTime:NULL,
                     SetCreationTime ? &CreationTime:NULL,
                     SetLastAccessTime ? &LastAccessTime:NULL,FileAttr);
      }

//      if(NewAttr != (FileAttr & (~FA_DIREC))) // �㦭� �� ��-���� ������???
      {
        if((NewAttr&FILE_ATTRIBUTE_COMPRESSED) && !(FileAttr&FILE_ATTRIBUTE_COMPRESSED))
          ESetFileCompression(SelName,1,FileAttr);
        else if(!(NewAttr&FILE_ATTRIBUTE_COMPRESSED) && (FileAttr&FILE_ATTRIBUTE_COMPRESSED))
          ESetFileCompression(SelName,0,FileAttr);
        else if((NewAttr&FILE_ATTRIBUTE_ENCRYPTED) && !(FileAttr&FILE_ATTRIBUTE_ENCRYPTED))
          ESetFileEncryption(SelName,1,FileAttr);
        else if(!(NewAttr&FILE_ATTRIBUTE_ENCRYPTED) && (FileAttr&FILE_ATTRIBUTE_ENCRYPTED))
          ESetFileEncryption(SelName,0,FileAttr);
        ESetFileAttributes(SelName,NewAttr&(~(FILE_ATTRIBUTE_ENCRYPTED|FILE_ATTRIBUTE_COMPRESSED)));
      }
    }

    /* Multi *********************************************************** */
    else
    {
      int SetAttr,ClearAttr,Cancel=0;
      char OldConsoleTitle[NM], OutFileName[72],TmpFileName[72];

//      EmptyDialog(AttrDlg,1,SelCount==1);

      {
        int RefreshNeed=FALSE;
        Dialog Dlg(AttrDlg,DlgCountItems,SetAttrDlgProc);
        Dlg.SetHelp("FileAttrDlg");
        Dlg.SetPosition(-1,-1,45,JunctionPresent?24:23);

        Dlg.Show();
        while (1)
        {
          int Sel11=AttrDlg[11].Selected;
          int Sel8=AttrDlg[8].Selected;
          int Sel9=AttrDlg[9].Selected;
          while (!Dlg.Done())
          {
            Dlg.ReadInput();
            Dlg.ProcessInput();
            FocusPos=Dialog::SendDlgMessage((HANDLE)&Dlg,DM_GETFOCUS,0,0);
            // ��ࠡ�⠥� �������᪫�祭��
            if(((FileSystemFlags & (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION))==
                 (FS_FILE_COMPRESSION|FS_FILE_ENCRYPTION)) &&
               (FocusPos == 8 || FocusPos == 9))
            {
              if(FocusPos == 8 && Sel8 != AttrDlg[8].Selected) // ����ﭨ� ����������?
              {
                if(AttrDlg[8].Selected == 1 && AttrDlg[9].Selected)
                   AttrDlg[9].Selected=0;
                else if(AttrDlg[8].Selected == 2)
                   AttrDlg[9].Selected=2;
                RefreshNeed=TRUE;
              }
              else if(FocusPos == 9 && Sel9 != AttrDlg[9].Selected) // ����ﭨ� ����������?
              {
                if(AttrDlg[9].Selected == 1 && AttrDlg[8].Selected)
                   AttrDlg[8].Selected=0;
                else if(AttrDlg[9].Selected == 2)
                   AttrDlg[8].Selected=2;
                RefreshNeed=TRUE;
              }
              Sel9=AttrDlg[9].Selected;
              Sel8=AttrDlg[8].Selected;
            }

            // �᫨ ᭨���� ��ਡ��� ��� SubFolders
            // ��� ��᮪ �ᥣ�� ࠡ�⠥� �᫨ ���� ��� �� ���� �����
            // ���� 11-� ������㯥� � �ᥣ�� ���.
            if(FocusPos == 11)
            {
              if(SelCount==1) // ��⠫�� �������筮!
              {
                if(Sel11 != AttrDlg[11].Selected) // ����ﭨ� ����������?
                {
//                  EmptyDialog(AttrDlg,1,1);
                  // 㡨ࠥ� 3-State
                  for(I=4; I <= 9; ++I)
                  {
                    if(!AttrDlg[11].Selected) // �﫨?
                    {
                      AttrDlg[I].Selected=OriginalCBAttr[I-4];
                      AttrDlg[I].Flags&=~DIF_3STATE;
                    }
                    else                      // ��⠭�����?
                    {
                      AttrDlg[I].Flags|=DIF_3STATE;
                      if(OriginalCBAttr2[I-4] == -1)
                        AttrDlg[I].Selected=2;
                    }
                  }
                  if(!AttrDlg[11].Selected)
                    FillFileldDir(SelName,FileAttr,AttrDlg,0);
                  RefreshNeed=TRUE;
                }
              }
              else  // ����� ��ꥪ⮢
              {
                if(Sel11 != AttrDlg[11].Selected) // ����ﭨ� ����������?
                {
//                  EmptyDialog(AttrDlg,1,0);
                  for(I=4; I <= 9; ++I)
                  {
                    if(!AttrDlg[11].Selected) // �﫨?
                    {
                      AttrDlg[I].Selected=OriginalCBAttr[I-4];
                      AttrDlg[I].Flags=OriginalCBFlag[I-4];
                    }
                    else                      // ��⠭�����?
                    {
                      if(OriginalCBAttr2[I-4] == -1)
                      {
                        AttrDlg[I].Flags|=DIF_3STATE;
                        AttrDlg[I].Selected=2;
                      }
                    }
                  }
                  RefreshNeed=TRUE;
                }
              }
              Sel11=AttrDlg[11].Selected;
            }

            if(RefreshNeed)
            {
              RefreshNeed=FALSE;
              Dlg.InitDialogObjects();
              Dlg.Show();
            }
          }
          Dlg.GetDialogObjectsData();
          if (Dlg.GetExitCode()!=24)
            break;
          FILETIME ft;
          GetSystemTimeAsFileTime(&ft);
          ConvertDate(&ft,AttrDlg[16].Data,AttrDlg[17].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(&ft,AttrDlg[19].Data,AttrDlg[20].Data,8,FALSE,FALSE,TRUE,TRUE);
          ConvertDate(&ft,AttrDlg[22].Data,AttrDlg[23].Data,8,FALSE,FALSE,TRUE,TRUE);
          if (AttrDlg[24].Focus)
          {
            AttrDlg[24].Focus=0;
            AttrDlg[16].Focus=1;
          }
          Dlg.ClearDone();
          Dlg.InitDialogObjects();
          Dlg.Show();
        }

        if (Dlg.GetExitCode()!=26)
          return 0;
      }

      CtrlObject->GetAnotherPanel(SrcPanel)->CloseFile();

      FILETIME LastWriteTime,CreationTime,LastAccessTime;
      int SetWriteTime,SetCreationTime,SetLastAccessTime;
      SetWriteTime=ReadFileTime(&LastWriteTime,AttrDlg[16].Data,AttrDlg[17].Data);
      SetCreationTime=ReadFileTime(&CreationTime,AttrDlg[19].Data,AttrDlg[20].Data);
      SetLastAccessTime=ReadFileTime(&LastAccessTime,AttrDlg[22].Data,AttrDlg[23].Data);

      SetAttr=0;  ClearAttr=0;

      if (AttrDlg[4].Selected == 1)         SetAttr|=FA_RDONLY;
      else if (!AttrDlg[4].Selected)        ClearAttr|=FA_RDONLY;
      if (AttrDlg[5].Selected == 1)         SetAttr|=FA_ARCH;
      else if (!AttrDlg[5].Selected)        ClearAttr|=FA_ARCH;
      if (AttrDlg[6].Selected == 1)         SetAttr|=FA_HIDDEN;
      else if (!AttrDlg[6].Selected)        ClearAttr|=FA_HIDDEN;
      if (AttrDlg[7].Selected == 1)         SetAttr|=FA_SYSTEM;
      else if (!AttrDlg[7].Selected)        ClearAttr|=FA_SYSTEM;

      if (AttrDlg[8].Selected == 1)
      {
        SetAttr|=FILE_ATTRIBUTE_COMPRESSED;
        ClearAttr|=FILE_ATTRIBUTE_ENCRYPTED;
      }
      else if (!AttrDlg[8].Selected)
        ClearAttr|=FILE_ATTRIBUTE_COMPRESSED;
      else if (AttrDlg[9].Selected == 1)
      {
        SetAttr|=FILE_ATTRIBUTE_ENCRYPTED;
        ClearAttr|=FILE_ATTRIBUTE_COMPRESSED;
      }
      else if (!AttrDlg[9].Selected)
        ClearAttr|=FILE_ATTRIBUTE_ENCRYPTED;

      // ������� ��������� ����
      GetConsoleTitle(OldConsoleTitle,sizeof(OldConsoleTitle));
      SetFarTitle(MSG(MSetAttrTitle));

      SrcPanel->GetSelName(NULL,FileAttr);
      while (SrcPanel->GetSelName(SelName,FileAttr) && !Cancel)
      {
//SysLog("SelName='%s'\n\tFileAttr =0x%08X\n\tSetAttr  =0x%08X\n\tClearAttr=0x%08X\n\tResult   =0x%08X",
// SelName,FileAttr,SetAttr,ClearAttr,((FileAttr|SetAttr)&(~ClearAttr)));
        Message(0,0,MSG(MSetAttrTitle),MSG(MSetAttrSetting),
          CenterStr(TruncPathStr(strcpy(OutFileName,SelName),40),OutFileName,44));

        if (CheckForEsc())
          break;

        if(SetWriteTime || SetCreationTime || SetLastAccessTime)
          if (!ESetFileTime(SelName,SetWriteTime ? &LastWriteTime:NULL,
                       SetCreationTime ? &CreationTime:NULL,
                       SetLastAccessTime ? &LastAccessTime:NULL,FileAttr))
            break;

        if(((FileAttr|SetAttr)&(~ClearAttr)) != FileAttr)
        {
          if (AttrDlg[8].Selected == 1) // -E +C
          {
            if (!ESetFileCompression(SelName,1,FileAttr))
              break; // ��㤠� ᦠ�� :-(
          }
          else if (AttrDlg[9].Selected == 1) // +E -C
          {
            if (!ESetFileEncryption(SelName,1,FileAttr))
              break; // ��㤠� ����஢��� :-(
          }
          else //???
          if (AttrDlg[8].Selected == 0) // -C ?E
          {
            if (!ESetFileCompression(SelName,0,FileAttr))
              break; // ��㤠� ࠧ���� :-(
          }
          else if (AttrDlg[9].Selected == 0) // ?C -E
          {
            if (!ESetFileEncryption(SelName,0,FileAttr))
              break; // ��㤠� ࠧ��஢��� :-(
          }

          if (!ESetFileAttributes(SelName,(FileAttr|SetAttr)&(~ClearAttr)))
            break;
        }

        if ((FileAttr & FA_DIREC) && AttrDlg[11].Selected)
        {
          char FullName[NM];
          ScanTree ScTree(FALSE);
          WIN32_FIND_DATA FindData;

          ScTree.SetFindPath(SelName,"*.*");
          while (ScTree.GetNextName(&FindData,FullName))
          {
            Message(0,0,MSG(MSetAttrTitle),MSG(MSetAttrSetting),
              CenterStr(TruncPathStr(strcpy(OutFileName,FullName),40),OutFileName,44));
            if (CheckForEsc())
            {
              Cancel=1;
              break;
            }
            if (!ESetFileTime(FullName,SetWriteTime ? &LastWriteTime:NULL,
                         SetCreationTime ? &CreationTime:NULL,
                         SetLastAccessTime ? &LastAccessTime:NULL,
                         FindData.dwFileAttributes))
            {
              Cancel=1;
              break;
            }
            if(((FindData.dwFileAttributes|SetAttr)&(~ClearAttr)) !=
                 FindData.dwFileAttributes)
            {
              if (AttrDlg[8].Selected == 1) // -E +C
              {
                if (!ESetFileCompression(FullName,1,FindData.dwFileAttributes))
                {
                  Cancel=1;
                  break; // ��㤠� ᦠ�� :-(
                }
              }
              else if (AttrDlg[9].Selected == 1) // +E -C
              {
                if (!ESetFileEncryption(FullName,1,FindData.dwFileAttributes))
                {
                  Cancel=1;
                  break; // ��㤠� ����஢��� :-(
                }
              }
              else //???
              if (!AttrDlg[8].Selected) // -C ?E
              {
                if (!ESetFileCompression(FullName,0,FindData.dwFileAttributes))
                {
                  Cancel=1;
                  break; // ��㤠� ࠧ���� :-(
                }
              }
              else if (!AttrDlg[9].Selected) // ?C -E
              {
                if (!ESetFileEncryption(FullName,0,FindData.dwFileAttributes))
                {
                  Cancel=1;
                  break; // ��㤠� ࠧ��஢��� :-(
                }
              }
              if (!ESetFileAttributes(FullName,(FindData.dwFileAttributes|SetAttr)&(~ClearAttr)))
              {
                Cancel=1;
                break;
              }
            }
          }
        }
      } // END: while (SrcPanel->GetSelName(...))
      SetConsoleTitle(OldConsoleTitle);
    }
  }

  SrcPanel->SaveSelection();
  SrcPanel->Update(UPDATE_KEEP_SELECTION);
  SrcPanel->ClearSelection();
  SrcPanel->Redraw();
  Panel *AnotherPanel=CtrlObject->GetAnotherPanel(SrcPanel);
  AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
  AnotherPanel->Redraw();
  return 1;
}
