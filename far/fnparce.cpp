/*
fnparce.cpp

������ �������� ����������

*/

/* Revision: 1.22 25.03.2005 $ */

/*
Modify:
  25.03.2005 WARP
    - ������������ �������� ������!
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  01.03.2004 SVS
    ! ������� FAR_OemTo* � FAR_CharTo* ������ ����������� WinAPI-�������
      (����� �� ������� + ������� ������������ ������ 1 ����)
  18.12.2003 SVS
    ! �������� ����������� ������
  28.10.2003 SVS
    ! ������� ������������ + ����������� "!?$<history>$<title>?<init>!"
  05.09.2003 SVS
    - ����������� ������������ ����-������ � ������� ������� (���������� "!@F!")
      �� ������ ������� TempPanel. ������ ���� ���������� ����� �� ������������
      ������.
  05.03.2003 SVS
    ! ���������� _SVS
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  13.12.2002 SVS
    ! �� ������� SubstFileName ������ ������������ ������� � ��������� ���������.
      ��� �������� ������� ������� �����������... � � �����������
      ������ ����������� �������
  30.09.2002 VVM
    - ������ strcpy ����� ���������� strncpy.
  12.08.2002 SVS
    - ������ � ������������ ������ � ������������ �������.
  15.04.2002 SVS
    - Dialog Dlg(DlgData,DlgSize); ����� � ������ ����� � ������� ReplaceVariables()
  05.03.2002 DJ
    - ������ � ������������� ������
  01.03.2002 SVS
    ! ���� ������ ���� ������� �������� ���������� ����� - FarMkTempEx
  26.01.2002 VVM
    ! ��� ��������� "!&" ������ � ������ ������ �� ��������.
  25.01.2002 SVS
    - ���� ��� ������ ����, ����� ������� ����������� !@! � !$! - ������
      ������� �� ��� ����.
  07.12.2001 SVS
    - BugZ#149 - ���������� !& ������ ������� ������ � ���������
      � ������ ����� ������ ������� ��� � ��������.
  05.10.2001 SVS
    ! ������� ������� Panel::MakeListFile() �� panel.cpp - ����� �� �����
  25.07.2001 IS
    ! ��� !` �� ������ ������� �����.
  29.06.2001 IS
    ! ���������� ������� ��������� ������� - ������� ���, ��� ����� ��������� �
      "quote"
  22.06.2001 SVS
    ! ������� ��������� �������
  19.06.2001 SVS
    ! �������� ������ ������ ������� SubstFileName() - ��������, � ���� ��
      ������� '!' � ������.
  18.06.2001 SVS
    + Created - ������� � ��������� ������... ��� �������� :-)
*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "panel.hpp"
#include "ctrlobj.hpp"
#include "flink.hpp"
#include "cmdline.hpp"
#include "filepanels.hpp"
#include "dialog.hpp"
#include "global.hpp"
#include "lang.hpp"

struct TSubstData
{
  // ��������� ������� SubstFileName
  char *Name;           // ������� ���
  char *ShortName;      // �������� ���
  char *ListName;       // ������� ��� �����-������
  char *ShortListName;  // �������� ��� �����-������

  // ��������� ����������
  char AnotherName[NM];
  char AnotherShortName[NM];
  char NameOnly[NM];
  char ShortNameOnly[NM];
  char AnotherNameOnly[NM];
  char AnotherShortNameOnly[NM];
  char CmdDir[NM];
  int  PreserveLFN;
  int  PassivePanel;

  Panel *AnotherPanel;
  Panel *ActivePanel;
};


static int IsReplaceVariable(char *str,int *scr = NULL,
                                int *end = NULL,
                                int *beg_scr_break = NULL,
                                int *end_scr_break = NULL,
                                int *beg_txt_break = NULL,
                                int *end_txt_break = NULL);
static int ReplaceVariables(char *Str,struct TSubstData *PSubstData);
static char *_SubstFileName(char *CurStr,struct TSubstData *PSubstData,char *TempStr,int MaxTempStrSize);

// Str=if exist !#!\!^!.! far:edit < diff -c -p "!#!\!^!.!" !\!.!

static char *_SubstFileName(char *CurStr,struct TSubstData *PSubstData,char *TmpStr,int MaxTempStrSize)
{
  //_SVS(CleverSysLog clv("*** _SubstFileName ***"));

//  char TmpName[NM+2];
//  char TmpShortName[NM+2];
//  char *DirBegin=NULL; // ������ ����� �������� (!?)
#if 0
  // "������" ����������� ������
  if(*CurStr == '(')
  {
    if(*++CurStr != '(')
    {
      char *PCurStr=strchr(CurStr,')');
      if(PCurStr)
      {
        *PCurStr=0;
        _SubstFileName(CurStr,PSubstData,TmpStr,MaxTempStrSize);
        CurStr=PCurStr+1;
      }
    }
    return CurStr;
  }
#endif

  // ���������� ������������� ����������/����������� ������.
  if (strncmp(CurStr,"!#",2)==0)
  {
    CurStr+=2;
    PSubstData->PassivePanel=TRUE;
    //_SVS(SysLog("PassivePanel=TRUE '%s'",CurStr));
    return CurStr;
  }

  if (strncmp(CurStr,"!^",2)==0)
  {
    CurStr+=2;
    PSubstData->PassivePanel=FALSE;
    //_SVS(SysLog("PassivePanel=FALSE '%s'",CurStr));
    return CurStr;
  }

  // !! ������ '!'
  if (strncmp(CurStr,"!!",2)==0 && CurStr[2] != '?')
  {
    strncat(TmpStr,"!",MaxTempStrSize-1);
    CurStr+=2;
    //_SVS(SysLog("!! TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !.!      ������� ��� ����� � �����������
  if (strncmp(CurStr,"!.!",3)==0 && CurStr[3] != '?')
  {
    strncat(TmpStr,PSubstData->PassivePanel ? PSubstData->AnotherName:PSubstData->Name, MaxTempStrSize-1);
    CurStr+=3;
    //_SVS(SysLog("!.! TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !~       �������� ��� ����� ��� ����������
  if (strncmp(CurStr,"!~",2)==0)
  {
    strncat(TmpStr,PSubstData->PassivePanel ? PSubstData->AnotherShortNameOnly:PSubstData->ShortNameOnly, MaxTempStrSize-1);
    CurStr+=2;
    //_SVS(SysLog("!~ TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !`  ������� ���������� ����� ��� �����
  if (strncmp(CurStr,"!`",2)==0)
  {
    char *Ext;
    if(CurStr[2] == '~')
    {
      Ext=strrchr((PSubstData->PassivePanel ? PSubstData->AnotherShortName:PSubstData->ShortName),'.');
      CurStr+=3;
    }
    else
    {
      Ext=strrchr((PSubstData->PassivePanel ? PSubstData->AnotherName:PSubstData->Name),'.');
      CurStr+=2;
    }
    if(Ext && *Ext)
      strncat(TmpStr,++Ext, MaxTempStrSize-1);
    //_SVS(SysLog("!` TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !\!.!  ������ ��� �����
  /*
  if (strncmp(CurStr,"!\\!.!",5)==0 && CurStr[5] != '?')
  {
    char CurDir[NM];
    char *FileName=PSubstData->PassivePanel ? PSubstData->AnotherName:PSubstData->Name;
    if (strpbrk(FileName,"\\:")==NULL)
    {
      if (PSubstData->PassivePanel)
        PSubstData->AnotherPanel->GetCurDir(CurDir);
      else
        strcpy(CurDir,PSubstData->CmdDir);
      AddEndSlash(CurDir);
    }
    else
      *CurDir=0;
    strncat(CurDir,FileName,sizeof (CurDir)-1);
    CurStr+=5;
//    if(!DirBegin) DirBegin=TmpStr+strlen(TmpStr);
    strncat(TmpStr,CurDir, MaxTempStrSize-1);
    //_SVS(SysLog("!\\!.! TmpStr=[%s]",TmpStr));
    return CurStr;
  }
  */

  // !& !&~  ������ ������ ����������� ��������.
  if (!strncmp(CurStr,"!&~",3) && CurStr[3] != '?' ||
      !strncmp(CurStr,"!&",2) && CurStr[2] != '?')
  {
    char FileNameL[NM],ShortNameL[NM];
    Panel *WPanel=PSubstData->PassivePanel?PSubstData->AnotherPanel:PSubstData->ActivePanel;
    int FileAttrL;
    int ShortN0=FALSE;
    int CntSkip=2;
    if(CurStr[2] == '~')
    {
      ShortN0=TRUE;
      CntSkip++;
    }
    WPanel->GetSelName(NULL,FileAttrL);
    int First = TRUE;
    while (WPanel->GetSelName(FileNameL,FileAttrL,ShortNameL))
    {
      if (ShortN0)
        strcpy(FileNameL,ShortNameL);
      else // � ������ ��� �� ������ ������� ��� � ��������.
        QuoteSpaceOnly(FileNameL);
// ��� ����� ��� ��� ����� - �����/�������...
//   ���� ����� ����� - ��������������� :-)
//          if(FileAttrL & FA_DIREC)
//            AddEndSlash(FileNameL);
      // � ����� �� ��� ������ � ����� ������?
      if (First)
        First = FALSE;
      else
        strncat(TmpStr," ", MaxTempStrSize-1);
      strncat(TmpStr,FileNameL, MaxTempStrSize-1);
      /* $ 05.03.2002 DJ
         ���� � ����� ������ �� ������ - ������ �� �����
      */
      if (strlen (TmpStr) >= MaxTempStrSize-1)
        break;
      /* DJ $ */
    }
    CurStr+=CntSkip;
    //_SVS(SysLog("!& TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !@  ��� �����, ����������� ����� ���������� ������
  //<Skeleton 2003 10 25>
  // !$!      ��� �����, ����������� �������� ����� ���������� ������
  // ���� ���� ���������� ���� ��� ������� ��� !@! ��� � !$!
  //������-�� (�� ������������ �������������� ��� ��) - � !$! ����� ����������� ������������ Q � A
  // �� ����� ����:)
  //<Skeleton>
  if (strncmp(CurStr,"!@",2)==0
        //<Skeleton 2003 10 25>
        || (strncmp(CurStr,"!$",2)==0))
        //<Skeleton>
  {
    char Modifers[32]="", *Ptr;

    //<Skeleton 2003 10 25>
    char * ListName = PSubstData->ListName;
    bool ShortN0 = FALSE;

    if (CurStr[1] == '$')
    {
      ShortN0 = TRUE;
      ListName = PSubstData->ShortListName;
    }
    //<Skeleton>

    if((Ptr=strchr(CurStr+2,'!')) != NULL)
    {
      if(Ptr[1] != '?')
      {
        *Ptr=0;
        xstrncpy(Modifers,CurStr+2,sizeof(Modifers)-1);
        /* $ 02.09.2000 tran
           !@!, !#!@! bug */
        if(ListName!=NULL)
        {
          if ( PSubstData->PassivePanel && ( ListName[NM] || PSubstData->AnotherPanel->MakeListFile(ListName+NM,FALSE,Modifers)))
          {
            //<Skeleton 2003 10 25>
            if (ShortN0)
            {
              /* $ 01.11.2000 IS
                 ��� ����� � ������ ������ ������ ���� ��������
              */
              ConvertNameToShort(ListName+NM,ListName+NM);
              /* IS $ */
            }
            //<Skeleton>
            strncat(TmpStr,ListName+NM, MaxTempStrSize-1);
          }
          if ( !PSubstData->PassivePanel && (*ListName || PSubstData->ActivePanel->MakeListFile(ListName,FALSE,Modifers)))
          {
            //<Skeleton 2003 10 25>
            if (ShortN0)
            {
              /* $ 01.11.2000 IS
                 ��� ����� � ������ ������ ������ ���� ��������
              */
              ConvertNameToShort(ListName,ListName);
              /* IS $ */
            }
            //<Skeleton>
            strncat(TmpStr,ListName, MaxTempStrSize-1);
          }
        }
        else
        {
          strncat(TmpStr,CurStr, MaxTempStrSize-1);
          strncat(TmpStr,Modifers, MaxTempStrSize-1);
          strncat(TmpStr,"!", MaxTempStrSize-1);
        }
        /* tran $ */
        CurStr+=Ptr-CurStr+1;
        return CurStr;
      }
    }
  }
#if 0
  // !$!      ��� �����, ����������� �������� ����� ���������� ������
  if (strncmp(CurStr,"!$",2)==0)
  {
    char Modifers[32]="", *Ptr;

    if((Ptr=strchr(CurStr+2,'!')) != NULL)
    {
      if(Ptr[1] != '?')
      {
        *Ptr=0;
        xstrncpy(Modifers,CurStr+2,MaxTempStrSize-1);
        /* $ 02.09.2000 tran
           !@!, !#!@! bug */
        if(PSubstData->ShortListName!=NULL)
        {
          if ( PSubstData->PassivePanel && (PSubstData->ShortListName[NM] || PSubstData->AnotherPanel->MakeListFile(PSubstData->ShortListName+NM,TRUE,Modifers)))
          {
            /* $ 01.11.2000 IS
               ��� ����� � ������ ������ ������ ���� ��������
            */
            ConvertNameToShort(PSubstData->ShortListName+NM,PSubstData->ShortListName+NM);
            /* IS $ */
            strncat(TmpStr,PSubstData->ShortListName+NM, MaxTempStrSize-1);
          }
          if ( !PSubstData->PassivePanel && (*PSubstData->ShortListName || PSubstData->ActivePanel->MakeListFile(PSubstData->ShortListName,TRUE,Modifers)))
          {
            /* $ 01.11.2000 IS
               ��� ����� � ������ ������ ������ ���� ��������
            */
            ConvertNameToShort(PSubstData->ShortListName,PSubstData->ShortListName);
            /* IS $ */
            strncat(TmpStr,PSubstData->ShortListName, MaxTempStrSize-1);
          }
          /* tran $ */
        }
        else
        {
          strncat(TmpStr,CurStr, MaxTempStrSize-1);
          strncat(TmpStr,Modifers, MaxTempStrSize-1);
          strncat(TmpStr,"!", MaxTempStrSize-1);
        }
        CurStr+=Ptr-CurStr+1;
        //_SVS(SysLog("!$! TmpStr=[%s]",TmpStr));
        return CurStr;
      }
    }
  }
#endif

  // !-!      �������� ��� ����� � �����������
  if (strncmp(CurStr,"!-!",3)==0 && CurStr[3] != '?')
  {
    strncat(TmpStr,PSubstData->PassivePanel ? PSubstData->AnotherShortName:PSubstData->ShortName, MaxTempStrSize-1);
    CurStr+=3;
    //_SVS(SysLog("!-! TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !+!      ���������� !-!, �� ���� ������� ��� ����� �������
  //          ����� ���������� �������, FAR ����������� ���
  if (strncmp(CurStr,"!+!",3)==0 && CurStr[3] != '?')
  {
    strncat(TmpStr,PSubstData->PassivePanel ? PSubstData->AnotherShortName:PSubstData->ShortName, MaxTempStrSize-1);
    CurStr+=3;
    PSubstData->PreserveLFN=TRUE;
    //_SVS(SysLog("!+! TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !:       ������� ����
  //<Skeleton 2003 10 26>
  // ���� - �������� ������ �� \\serv\share
  //<Skeleton>
  if (strncmp(CurStr,"!:",2)==0)
  {
    char CurDir[NM];
    char RootDir[NM];
    if (*PSubstData->Name && PSubstData->Name[1]==':')
      strcpy(CurDir,PSubstData->Name);
    else
      if (PSubstData->PassivePanel)
        PSubstData->AnotherPanel->GetCurDir(CurDir);
      else
        strcpy(CurDir,PSubstData->CmdDir);

    GetPathRoot(CurDir,RootDir);
    DeleteEndSlash(RootDir);
    strncat(TmpStr,RootDir, MaxTempStrSize-1);
    CurStr+=2;
    //_SVS(SysLog("!: TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !\       ������� ����
  //<Skeleton 2003 10 26>
  // !/       �������� ��� �������� ����
  // ���� ���� ���������� ���� ��� ������� ��� !\ ��� � !/
  //<Skeleton>
  if (strncmp(CurStr,"!\\",2)==0
     //<Skeleton 2003 10 26>
     || (strncmp(CurStr,"!/",2)==0))
     //<Skeleton>
  {
    char CurDir[NM];
    //<Skeleton 2003 10 26>
    bool ShortN0 = FALSE;

    if (CurStr[1] == '/')
    {
      ShortN0 = TRUE;
    }
    //<Skeleton>
    if (PSubstData->PassivePanel)
      PSubstData->AnotherPanel->GetCurDir(CurDir);
    else
      strcpy(CurDir,PSubstData->CmdDir);

    if (ShortN0)
        ConvertNameToShort(CurDir,CurDir);
    AddEndSlash(CurDir);
    CurStr+=2;
    if (*CurStr=='!')
    {
//      strcpy(TmpName,PSubstData->Name);
//      strcpy(TmpShortName,PSubstData->ShortName);
      if (strpbrk(PSubstData->Name,"\\:")!=NULL)
        *CurDir=0;
    }
//    if(!DirBegin) DirBegin=TmpStr+strlen(TmpStr);
    strncat(TmpStr,CurDir, MaxTempStrSize-1);
    //_SVS(SysLog("!\\ TmpStr=[%s] CurDir=[%s]",TmpStr, CurDir));
    return CurStr;
  }
#if 0
  // !/       �������� ��� �������� ����
  if (strncmp(CurStr,"!/",2)==0)
  {
    char CurDir[NM];
    if (PSubstData->PassivePanel)
      PSubstData->AnotherPanel->GetCurDir(CurDir);
    else
      strcpy(CurDir,PSubstData->CmdDir);
    ConvertNameToShort(CurDir,CurDir);
    AddEndSlash(CurDir);
    CurStr+=2;
    if (*CurStr=='!')
    {
//      strcpy(TmpName,PSubstData->Name);
//      strcpy(TmpShortName,PSubstData->ShortName);
      if (strpbrk(PSubstData->Name,"\\:")!=NULL)
      {
//        if (PointToName(PSubstData->ShortName)==PSubstData->ShortName)
//        {
//          strcpy(TmpShortName,CurDir);
//          AddEndSlash(TmpShortName);
//          strcat(TmpShortName,PSubstData->ShortName);
//        }
        *CurDir=0;
      }
    }
//    if(!DirBegin) DirBegin=TmpStr+strlen(TmpStr);
    strncat(TmpStr,CurDir, MaxTempStrSize-1);
    //_SVS(SysLog("!/ TmpStr=[%s]",TmpStr));
    return CurStr;
  }
#endif

  // !?<title>?<init>!
  if (strncmp(CurStr,"!?",2)==0 && strchr(CurStr+2,'!')!=NULL)
  {
#if 0
    // "������" ����������� ������
    char *Ptr=(CurStr+=2), *Ptr2;
    char TempStr2[2048];

    strcat(TmpStr,"!?");
    while(*Ptr)
    {
      if(*Ptr == '(' && Ptr[1] != '(')
      {
        Ptr2=Ptr+1;
        *TempStr2=0;
        while(*Ptr)
        {
          if(*Ptr == ')' && Ptr[1] != ')')
          {
            *Ptr=0;
            while (*Ptr2)
            {
              if(*Ptr2 == '!')
              {
                Ptr2=_SubstFileName(Ptr2,PSubstData,TempStr2,sizeof(TempStr2));
                strncat(TmpStr,TempStr2,strlen(TempStr2));
              }
              else
              {
                strncat(TmpStr,Ptr2,1);
                Ptr2++;
              }
            }
            *Ptr=')';
            break;
          }
          Ptr++;
        }
        if(*Ptr == 0)
          break;
      }
      else if(*Ptr == '!')
      {
        strcat(TmpStr,"!");
        break;
      }
      else if(*Ptr == '?')
      {
        strcat(TmpStr,"?");
      }
      else
        strncat(TmpStr,Ptr,1);
      Ptr++;
    }
    CurStr=Ptr;
#else
    //<Skeleton 2003 11 22>
    //char *NewCurStr=strchr(CurStr+2,'!')+1;
    //strncat(TmpStr,CurStr,NewCurStr-CurStr);
    //CurStr=NewCurStr;

    int j;
    int i = IsReplaceVariable(CurStr);
    if (i == -1)  // if bad format string
    {             // skip 1 char
      j = 1;
    }
    else
    {
      j = i + 1;
    }
    strncat(TmpStr,CurStr,j);
    CurStr += j;
    //<Skeleton>
#endif
    //_SVS(SysLog("!? TmpStr=[%s]",TmpStr));
    return CurStr;
  }

  // !        ������� ��� ����� ��� ����������
  if (*CurStr=='!')
  {
//    if(!DirBegin) DirBegin=TmpStr+strlen(TmpStr);
    strncat(TmpStr,PointToName(PSubstData->PassivePanel ? PSubstData->AnotherNameOnly:PSubstData->NameOnly), MaxTempStrSize-1);
    CurStr++;
    //_SVS(SysLog("! TmpStr=[%s]",TmpStr));
  }

  return CurStr;
}

/*
  SubstFileName()
  �������������� ������������ ���������� ������ � �������� ��������

  ������� ListName � ShortListName ������� ����� ������ NM*2 !!!
*/
int SubstFileName(char *Str,            // �������������� ������
                  int   StrSize,        // /* $ 05.03.2002 DJ */ ������ ������ �������������� ������ /* DJ $ */
                  char *Name,           // ������� ���
                  char *ShortName,      // �������� ���
                  char *ListName,       // ������� ��� �����-������
                  char *ShortListName,  // �������� ��� �����-������
                  int   IgnoreInput,    // TRUE - �� ��������� "!?<title>?<init>!"
                  char *CmdLineDir)     // ������� ����������
{
  //_SVS(CleverSysLog clv(Str));

  // ���������� ����� ������-������� (���� �����)
  if (ListName!=NULL)
  {
    *ListName=0;
    ListName[NM]=0;
  }

  if (ShortListName!=NULL)
  {
    *ShortListName=0;
    ShortListName[NM]=0;
  }

  /* $ 19.06.2001 SVS
    ��������! ��� �������������� ������������, �� ���������� �� "!",
    ����� ����� ���� ������ ��� �������� ���� �������� ������� (���������
    ����������������!)
  */
  if(!strchr(Str,'!'))
    return FALSE;
  /* SVS $ */

  //<Skeleton 2003 11 18>
  char TmpStr2[10240];
  //<Skeleton>

  struct TSubstData SubstData, *PSubstData=&SubstData;
  char *ChPtr;

  // ������� ���� ������� - 10240, �� ����� ����� ��������� ������... (see below)
  char TmpStr[10240];
  char *CurStr;

  // <PreProcess>
  *TmpStr=0; // ���� �����.

  PSubstData->Name=Name;                    // ������� ���
  PSubstData->ShortName=ShortName;          // �������� ���
  PSubstData->ListName=ListName;            // ������� ��� �����-������
  PSubstData->ShortListName=ShortListName;  // �������� ��� �����-������

  // ���� ��� �������� �������� �� ������...
  if (CmdLineDir!=NULL)
    strcpy(PSubstData->CmdDir,CmdLineDir);
  else // ...������� � ���.������
    CtrlObject->CmdLine->GetCurDir(PSubstData->CmdDir);


  // �������������� ������� ��������� "���������" :-)
  strcpy(PSubstData->NameOnly,Name);
  if ((ChPtr=strrchr(PSubstData->NameOnly,'.'))!=NULL)
    *ChPtr=0;

  strcpy(PSubstData->ShortNameOnly,ShortName);
  if ((ChPtr=strrchr(PSubstData->ShortNameOnly,'.'))!=NULL)
    *ChPtr=0;

  PSubstData->ActivePanel=CtrlObject->Cp()->ActivePanel;
  PSubstData->AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(PSubstData->ActivePanel);
  PSubstData->AnotherPanel->GetCurName(PSubstData->AnotherName,PSubstData->AnotherShortName);
  strcpy(PSubstData->AnotherNameOnly,PSubstData->AnotherName);
  if ((ChPtr=strrchr(PSubstData->AnotherNameOnly,'.'))!=NULL)
    *ChPtr=0;

  strcpy(PSubstData->AnotherShortNameOnly,PSubstData->AnotherShortName);
  if ((ChPtr=strrchr(PSubstData->AnotherShortNameOnly,'.'))!=NULL)
    *ChPtr=0;

  PSubstData->PreserveLFN=FALSE;
  PSubstData->PassivePanel=FALSE; // ������������� ���� ���� ��� �������� ������!
  // </PreProcess>

  //_SVS(int Pass=1);

  //<Skeleton 2003 11 18>
  if (strlen(Str) <= sizeof(TmpStr2)-1)       //we need TmpStr2  because need spase for replace !??!
    strcpy(TmpStr2,Str);
  else
    xstrncpy(TmpStr2,Str,sizeof(TmpStr2)-1);

  if (!IgnoreInput)
    ReplaceVariables(TmpStr2,PSubstData);

  //CurStr=Str;
  CurStr=TmpStr2;

  while (*CurStr)
  {
    //_SVS(SysLog("***** Pass=%d",Pass));
    if(*CurStr == '!')
      CurStr=_SubstFileName(CurStr,PSubstData,TmpStr,sizeof(TmpStr)-strlen(TmpStr)-1);
    else                                           //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ��� ������ ��������!
    {
      strncat(TmpStr,CurStr,1);
      CurStr++;
    }
    //_SVS(++Pass);
  }

  //��������� ������ ������ ��������� ������ ����������� ����� while
  //if (!IgnoreInput)
  //  ReplaceVariables(TmpStr);
  //<Skeleton>

  xstrncpy(Str,TmpStr,StrSize-1);

  //_SVS(SysLog("[%s]\n",Str));
  return(PSubstData->PreserveLFN);
}

int ReplaceVariables(char *Str,struct TSubstData *PSubstData)
{
  const int MaxSize=20;
  char *StartStr=Str;

  if (*Str=='\"')
    while (*Str && *Str!='\"')
      Str++;

  struct DialogItem *DlgData=NULL;
  int DlgSize=0;
  int StrPos[128],StrEndPos[128],StrPosSize=0;

  while (*Str && DlgSize<MaxSize)
  {
    if (*(Str++)!='!')
      continue;
    if (*(Str++)!='?')
      continue;

    //<Skeleton 2003 11 20>
    //if (strchr(Str,'!')==NULL)  //<---------�������� ��� �� ������
    //  return 0;                 // �������� ����� ���������� ������� ����������� ������
                                  // ��������� �� �������
    int scr,end, beg_t,end_t,beg_s,end_s;
    scr = end = beg_t = end_t = beg_s = end_s = 0;
    int ii = IsReplaceVariable(Str-2,&scr,&end,&beg_t,&end_t,&beg_s,&end_s);
    if (ii == -1)
    {
      ::free(DlgData);
      *StartStr=0;
      return 0;
    }

    StrEndPos[StrPosSize] = (Str - StartStr - 2) + ii ; //+1
    StrPos[StrPosSize++]=Str-StartStr-2;
    //<Skeleton>

    DlgData=(struct DialogItem *)xf_realloc(DlgData,(DlgSize+2)*sizeof(*DlgData));
    memset(&DlgData[DlgSize],0,2*sizeof(*DlgData));
    DlgData[DlgSize].Type=DI_TEXT;
    DlgData[DlgSize].X1=5;
    DlgData[DlgSize].Y1=DlgSize+2;
    DlgData[DlgSize+1].Type=DI_EDIT;
    DlgData[DlgSize+1].X1=5;
    DlgData[DlgSize+1].X2=70;
    DlgData[DlgSize+1].Y1=DlgSize+3;
    DlgData[DlgSize+1].Flags|=DIF_HISTORY|DIF_USELASTHISTORY;

    char HistoryName[MaxSize][20];
    int HistoryNumber=DlgSize/2;
    sprintf(HistoryName[HistoryNumber],"UserVar%d",HistoryNumber);
    /* $ 01.08.2000 SVS
       + .History
    */
    DlgData[DlgSize+1].Selected=(int)HistoryName[HistoryNumber];
    /* SVS $*/

    if (DlgSize==0)
    {
      DlgData[DlgSize+1].Focus=1;
      DlgData[DlgSize+1].DefaultButton=1;
    }

    char Title[256];
    //<Skeleton 2003 11 22>
    //xstrncpy(Title,Str,sizeof(Title)-1);
    //*strchr(Title,'!')=0;
    //Str+=strlen(Title)+1;
    //char *SrcText=strchr(Title,'?');
    //if (SrcText!=NULL)
    //{
    //  *SrcText=0;
    //  xstrncpy(DlgData[DlgSize+1].Data.Data,SrcText+1,sizeof(DlgData[DlgSize+1].Data.Data)-1);
    //}

    char Title2[512];
    char Title3[512];
    char Txt[512];

    // ��������� �������� ���� ����� �������� �������� - ���� ����
    *Title = *Title2 = *Title3 = 0;

    if (scr > 2)          // if between !? and ? exist some
      strncat(Title,Str,scr-2);

    //strcpy(DlgData[DlgSize].Data.Data,Title);

    char *t = Title;

    if (strlen(Title)>0)
    {
      if (Title[0] == '$')        // begin of history name
      {
        char *p = &Title[1];
        char *p1 = strchr(p,'$');
        if (p1)
        {
          *p1 = 0;
          t = ++p1;
          strcpy(HistoryName[HistoryNumber],p);
        }
      }
    }

    char *Title1 = NULL;
    Title1 = (char *)xf_realloc(Title1,10240);
    char *Txt1 = NULL;
    Txt1 = (char *)xf_realloc(Txt1,10240);
    char * tmp_t = NULL;
    tmp_t = (char *)xf_realloc(tmp_t,10240);

    *Title1 = 0;
    *Txt1 = 0;
    *tmp_t = 0;


    if ((beg_t == -1) || (end_t == -1))
    {
      //
    }
    else if ((end_t - beg_t) > 1) //if between ( and ) exist some
    {
      int hist_correct = 0;
      if (t != Title) // Title contain name of history
        hist_correct = t - Title; //offset for jump over history

      strncat(Title1,Title+hist_correct,beg_t-2-hist_correct);   // !?$zz$xxxx(fffff)ddddd
                                                                 //       ^  ^

      strncat(Title2,Title+(end_t-2)+1,scr-end_t-1); // !?$zz$xxxx(fffff)ddddd
                                                     //                  ^   ^

      strncat(Title3,Title+(beg_t-2)+1,end_t-beg_t-1);  // !?$zz$xxxx(ffffff)ddddd
                                                        //            ^    ^
      char *CurStr= Title3;
      while (*CurStr)
      {
        if (*CurStr == '!')
          CurStr=_SubstFileName(CurStr,PSubstData,tmp_t,10240-strlen(tmp_t)-1);
        else                                           //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ��� ������ ��������!
        {
          strncat(tmp_t,CurStr,1);
          CurStr++;
        }
      }

      strcat(Title1,tmp_t);
      strcat(Title1,Title2);
      t = Title1;
    }
    //do it - ���� ����� ��� ��� �������� � �������������
    xstrncpy(DlgData[DlgSize].Data,t,sizeof(DlgData[DlgSize].Data)-1);

    // ��������� ���� ����� �������� �������� - ���� ����
    char *s = Txt;
    *Txt = 0;
    *Title2 = 0;
    *Title3 = 0;
    *tmp_t = 0;

    if ((end-scr) > 1)  //if between ? and ! exist some
      strncat(Txt,(Str-2)+scr+1,(end-scr)-1);

    if ((beg_s == -1) || (end_s == -1))
    {
      //
    }
    else if ((end_s - beg_s) > 1) //if between ( and ) exist some
    {
      strncat(Txt1,Txt,beg_s-scr-1);   // !?$zz$xxxx(fffff)ddddd?rrrr(pppp)qqqqq!
                                       //                        ^  ^

      strncat(Title2,Txt+(end_s-scr),end-end_s-1); // !?$zz$xxxx(fffff)ddddd?rrrr(pppp)qqqqq!
                                                   //                                  ^   ^

      strncat(Title3,Txt+(beg_s-scr),end_s-beg_s-1);  // !?$zz$xxxx(ffffff)ddddd?rrrr(pppp)qqqqq!
                                                      //                              ^  ^

      char *CurStr= Title3;
      while (*CurStr)
      {
        if (*CurStr == '!')
          CurStr=_SubstFileName(CurStr,PSubstData,tmp_t,10240-strlen(tmp_t)-1);
        else                                           //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ��� ������ ��������!
        {
          strncat(tmp_t,CurStr,1);
          CurStr++;
        }
      }

      strcat(Txt1,tmp_t);
      strcat(Txt1,Title2);
      s = Txt1;
    }
    xstrncpy(DlgData[DlgSize+1].Data,s,sizeof(DlgData[DlgSize+1].Data)-1);


    xf_free(Title1);
    xf_free(Txt1);
    xf_free(tmp_t);
    //<Skeleton>

    /* $ 01.08.2000 SVS
       "���������" ���������
    */
    ExpandEnvironmentStr(DlgData[DlgSize].Data,DlgData[DlgSize].Data,sizeof(DlgData[DlgSize].Data));
    /* SVS $*/
    DlgSize+=2;
  }
  if (DlgSize==0)
    return 0;
  DlgData=(struct DialogItem *)xf_realloc(DlgData,(DlgSize+1)*sizeof(*DlgData));
  memset(&DlgData[DlgSize],0,sizeof(*DlgData));
  DlgData[DlgSize].Type=DI_DOUBLEBOX;
  DlgData[DlgSize].X1=3;
  DlgData[DlgSize].Y1=1;
  DlgData[DlgSize].X2=72;
  DlgData[DlgSize].Y2=DlgSize+2;
  DlgSize++;

  int ExitCode;
  {
    Dialog Dlg(DlgData,DlgSize);
    Dlg.SetPosition(-1,-1,76,DlgSize+3);
    Dlg.Process();
    ExitCode=Dlg.GetExitCode();
  }

  if (ExitCode==-1)
  {
    xf_free(DlgData);
    *StartStr=0;
    return 0;
  }

  char TmpStr[4096];
  *TmpStr=0;
  for (Str=StartStr;*Str!=0;Str++)
  {
    int Replace=-1;
    //<Skeleton 2003 11 22>
    int end_pos=0;
    for (int I=0;I<StrPosSize;I++)
      if (Str-StartStr==StrPos[I])
      {
        Replace=I;
        end_pos = StrEndPos[I];
        break;
      }
    //<Skeleton>
    if (Replace!=-1)
    {
      strcat(TmpStr,DlgData[Replace*2+1].Data);

      //<Skeleton 2003 11 20>
      //Str=strchr(Str+1,'!');
      Str = StartStr+end_pos;
      //<Skeleton>
    }
    else
      strncat(TmpStr,Str,1);
  }
  strcpy(StartStr,TmpStr);
  /* $ 01.08.2000 SVS
     ����� "��������" Enter "���������" ������
  */
  ExpandEnvironmentStr(TmpStr,StartStr,sizeof(DlgData[0].Data));
  /* SVS $ */
  /* $ 13.07.2000 SVS
     ������ ��� �� realloc
  */
  xf_free(DlgData);
  /* SVS $ */
  return 1;
}

int Panel::MakeListFile(char *ListFileName,int ShortNames,char *Modifers)
{
  FILE *ListFile;

  if (!FarMkTempEx(ListFileName) || (ListFile=fopen(ListFileName,"wb"))==NULL)
  {
    Message(MSG_WARNING,1,MSG(MError),MSG(MCannotCreateListFile),MSG(MCannotCreateListTemp),MSG(MOk));
    return(FALSE);
  }

  char FileName[NM*2],ShortName[NM];
  int FileAttr;
  GetSelName(NULL,FileAttr);
  while (GetSelName(FileName,FileAttr,ShortName))
  {
    if (ShortNames)
      strcpy(FileName,ShortName);

    if(Modifers && *Modifers)
    {
      if(strchr(Modifers,'F') && PointToName(FileName) == FileName) // 'F' - ������������ ������ ����;
      {
        char TempFileName[NM*2];
        strcpy(TempFileName,CurDir);
        sprintf(TempFileName,"%s%s%s",CurDir,(CurDir[strlen(CurDir)-1] != '\\'?"\\":""),FileName);
        if (ShortNames)
          ConvertNameToShort(TempFileName,TempFileName);
        strcpy(FileName,TempFileName);
      }
      if(strchr(Modifers,'Q')) // 'Q' - ��������� ����� � ��������� � �������;
        QuoteSpaceOnly(FileName);
      if(strchr(Modifers,'A')) // 'A' - ������������ ANSI ���������.
        FAR_OemToChar(FileName,FileName);

      if(strchr(Modifers,'S')) // 'S' - ������������ '/' ������ '\' � ����� ������;
      {
        int I,Len=strlen(FileName);
        for(I=0; I < Len; ++I)
          if(FileName[I] == '\\')
            FileName[I]='/';
      }
    }
//_D(SysLog("%s[%s] %s",__FILE__,Modifers,FileName));
    if (fprintf(ListFile,"%s\r\n",FileName)==EOF)
    {
      fclose(ListFile);
      remove(ListFileName);
      Message(MSG_WARNING,1,MSG(MError),MSG(MCannotCreateListFile),MSG(MCannotCreateListWrite),MSG(MOk));
      return(FALSE);
    }
  }
  if (fclose(ListFile)==EOF)
  {
    clearerr(ListFile);
    fclose(ListFile);
    remove(ListFileName);
    Message(MSG_WARNING,1,MSG(MError),MSG(MCannotCreateListFile),MSG(MOk));
    return(FALSE);
  }
  return(TRUE);
}

static int IsReplaceVariable(char *str,int *scr,
                                int *end,
                                int *beg_scr_break,
                                int *end_scr_break,
                                int *beg_txt_break,
                                int *end_txt_break)
// ��� ����� ������ - ����-�e 4 ��������� - ��� �������� �� str
// ������ ������ � ������ ��������, ����� ���� ������, ������ ������ � ������ ���������� ����������, �� � ����� �����.
// ������ ��� ������� ������ (������� � ��������� �����) ��� �������� ������:
// i = IsReplaceVariable(str) - ���� ��� ���� ������ ��������� ��������� ������ � ������ ?!
// ���  i - ��� ������, ������� ���� ���������, ���� �������� �� ����� ! ��������� !??!
{
  char *s      = str;
  char *scrtxt = str;

  int count_scob = 0;
  int second_count_scob = 0;

  bool was_quest_and_asterics = false; //  ?!
  bool was_quest = false;         //  ?
  bool was_asterics = false;      //  !

  bool in_firstpart_was_scob = false;
  char *beg_firstpart_scob = NULL;
  char *end_firstpart_scob = NULL;

  bool in_secondpart_was_scob = false;
  char *beg_secondpart_scob = NULL;
  char *end_secondpart_scob = NULL;

  if (!s)
    return -1;

  if (strncmp(s,"!?",2) == 0)
    s = s + 2;
  else
    return -1;
  //
  while (true)    // analize from !? to ?
  {
    if (!*s)
      return -1;

    if (*s == '(')
    {
      if (in_firstpart_was_scob)
      {
        //return -1;
      }
      else
      {
        in_firstpart_was_scob = true;
        beg_firstpart_scob = s;     //remember wher is first break
      }
      count_scob += 1;
    }
    else if (*s == ')')
    {
      count_scob -= 1;
      if (count_scob == 0)
      {
        if (!end_firstpart_scob)
          end_firstpart_scob = s;   //remember wher is last break
      }
      else if (count_scob < 0)
        return -1;
    }
    else if ((*s == '?') && ((!beg_firstpart_scob && !end_firstpart_scob) || (beg_firstpart_scob && end_firstpart_scob)))
    {
      was_quest = true;
    }

    s++;
    if (was_quest) break;
  }
  if (count_scob != 0) return -1;
  scrtxt = s - 1; //remember s for return

  while (true)    //analize from ? or !
  {
    if (!*s)
      return -1;

    if (*s == '(')
    {
      if (in_secondpart_was_scob)
      {
        //return -1;
      }
      else
      {
        in_secondpart_was_scob = true;
        beg_secondpart_scob = s;    //remember wher is first break
      }
      second_count_scob += 1;
    }
    else if (*s == ')')
    {
      second_count_scob -= 1;
      if (second_count_scob == 0)
      {
        if (!end_secondpart_scob)
          end_secondpart_scob = s;  //remember wher is last break
      }
      else if (second_count_scob < 0)
        return -1;
    }
    else if ((*s == '!') && ((!beg_secondpart_scob && !end_secondpart_scob) || (beg_secondpart_scob && end_secondpart_scob)))
    {
      was_asterics = true;
    }

    s++;
    if (was_asterics) break;
  }
  if (second_count_scob != 0) return -1;

  //
  if (scr != NULL)
    *scr = scrtxt - str;
  if (end != NULL)
    *end = (s - str) - 1;

  if (in_firstpart_was_scob)
  {
    if (beg_scr_break != NULL)
      *beg_scr_break = beg_firstpart_scob - str;
    if (end_scr_break != NULL)
      *end_scr_break = end_firstpart_scob - str;
  }
  else
  {
    if (beg_scr_break != NULL)
      *beg_scr_break = -1;
    if (end_scr_break != NULL)
      *end_scr_break = -1;
  }

  if (in_secondpart_was_scob)
  {
    if (beg_txt_break != NULL)
      *beg_txt_break = beg_secondpart_scob - str;
    if (end_txt_break != NULL)
      *end_txt_break = end_secondpart_scob - str;
  }
  else
  {
    if (beg_txt_break != NULL)
      *beg_txt_break = -1;
    if (end_txt_break != NULL)
      *end_txt_break = -1;
  }

  return ((s - str) - 1);
}
