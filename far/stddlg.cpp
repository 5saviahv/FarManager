/*
stddlg.cpp

���� ������ ����������� ��������

*/

/* Revision: 1.24 10.06.2002 $ */

/*
Modify:
  10.06.2002 SVS
    + DIF_EDITPATH (FIB_EDITPATH)
  10.05.2002 SVS
    ! ������� � ���� ����� Dialog (��� ������ ��������)
  29.04.2002 SVS
    ! ������� "������� ���" � ������� GetString �� 12.03.2001, �.�.
      ������ ��� �������� �� ������ �������.
  13.02.2002 SVS
    + FIB_NOAMPERSAND
  07.12.2001 IS
    + � GetString ����� ��������� CheckBox
  05.12.2001 SVS
    ! �������� ��������� ��������� ���������� �� ���� ������.
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  14.09.2001 SVS
    ! ����������� ����������
  09.09.2001 SVS
    + GetMenuHotKey()
  01.08.2001 SVS
    ! ���� ���� �� ���� �� ��������� ���� ���������� �������
      GetSearchReplaceString ����� NULL, �� ���������� ����������� ��������
  26.06.2001 SVS
    ! __except -> EXCEPT
  25.06.2001 IS
    ! ��������� const
  11.06.2001 SVS
    ! ����� ��������� � GetSearchReplaceString() - ����������� ������� �������
  16.05.2001 SVS
    ! DumpExceptionInfo ������� �� xfilter
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  06.05.2001 DJ
    ! �������� #include
  16.03.2001 SVS
    + GetNameAndPassword();
  13.03.2001 SVS
    - � ���������� ����� ������� �������� ������� - �� ���� ������ ��������
      � ���������.
  12.03.2001 SVS
    ! ������� ��� � ������� GetString :-)
  12.02.2001 SVS
    ! Ops. ���� � GetString :-)
  11.02.2001 SVS
    ! ��������� � GetString � ������ ����� DIF_VAREDIT
  28.01.2001 SVS
    ! DumpExeptionInfo -> DumpExceptionInfo ;-)
  23.01.2001 SVS
    + ������� ������� ��������� :-)
  23.01.2001 SVS
    - �� ��� � ������ ���� � ������� ������/������ :-(
  21.01.2001 SVS
    ! ��������� � �������� ���������������� ������
    + ������� GetString ��������� �� mix.cpp
    + GetSearchReplaceString - ������������� �� editor.cpp
*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "dialog.hpp"
#include "ctrlobj.hpp"
#include "farexcpt.hpp"

/*
  ������� GetSearchReplaceString ������� ������ ������ ��� ������, ���������
  �� ������������ ������ � � ������ ��������� ���������� ������� ����������
  TRUE.
  ���������:
    IsReplaceMode
      TRUE  - ���� ����� ��������
      FALSE - ���� ����� ������

    SearchStr
      ��������� �� ������ ������.
      ��������� ��������� ������� ��������� � ��� ��.

    ReplaceStr,
      ��������� �� ������ ������.
      ��������� ��������� ������� ��������� � ��� ��.
      ��� ������, ���� IsReplaceMode=FALSE ����� ���� ����� NULL

    TextHistoryName
      ��� ������� ������ ������.
      ���� ����������� � NULL, �� �� ���������
      ����������� �������� "SearchText"
      ���� ����������� � ������ ������, �� ������� ������� �� �����

    ReplaceHistoryName
      ��� ������� ������ ������.
      ���� ����������� � NULL, �� �� ���������
      ����������� �������� "ReplaceText"
      ���� ����������� � ������ ������, �� ������� ������� �� �����

    *Case
      ��������� �� ����������, ����������� �� �������� ����� "Case sensitive"
      ���� = NULL, �� ����������� �������� 0 (������������ �������)

    *WholeWords
      ��������� �� ����������, ����������� �� �������� ����� "Whole words"
      ���� = NULL, �� ����������� �������� 0 (� ��� ����� � ���������)

    *Reverse
      ��������� �� ����������, ����������� �� �������� ����� "Reverse search"
      ���� = NULL, �� ����������� �������� 0 (������ �����)

  ������������ ��������:
    TRUE  - ������������ ���������� ���� ���������
    FALSE - ������������ ��������� �� ������� (Esc)
*/
int WINAPI GetSearchReplaceString(
         int IsReplaceMode,
         unsigned char *SearchStr,
         int LenSearchStr,
         unsigned char *ReplaceStr,
         int LenReplaceStr,
         const char *TextHistoryName,
         const char *ReplaceHistoryName,
         int *Case,
         int *WholeWords,
         int *Reverse)
{
  if(!SearchStr || (IsReplaceMode && !ReplaceStr))
    return FALSE;
  static const char *TextHistoryName0    ="SearchText",
                    *ReplaceHistoryName0 ="ReplaceText";

  int HeightDialog, I;

  if(!TextHistoryName)
    TextHistoryName=TextHistoryName0;
  if(!ReplaceHistoryName)
    ReplaceHistoryName=ReplaceHistoryName0;

  /* $ 03.08.2000 KM
     ���������� checkbox'�� � ������� ��� ������ ����� ����
  */
  if (IsReplaceMode)
  {
/*
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345
00
01   +----------------------------- Replace ------------------------------+
02   | Search for                                                         |
03   |                                                                   |
04   | Replace with                                                       |
05   |                                                                   |
06   +--------------------------------------------------------------------+
07   | [ ] Case sensitive                                                 |
08   | [ ] Whole words                                                    |
09   | [ ] Reverse search                                                 |
10   +--------------------------------------------------------------------+
11   |                      [ Replace ]  [ Cancel ]                       |
12   +--------------------------------------------------------------------+
13
*/
    static struct DialogData ReplaceDlgData[]={
    /*  0 */DI_DOUBLEBOX,3,1,72,12,0,0,0,0,(char *)MEditReplaceTitle,
    /*  1 */DI_TEXT,5,2,0,0,0,0,0,0,(char *)MEditSearchFor,
    /*  2 */DI_EDIT,5,3,70,3,1,(DWORD)TextHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,0,"",
    /*  3 */DI_TEXT,5,4,0,0,0,0,0,0,(char *)MEditReplaceWith,
    /*  4 */DI_EDIT,5,5,70,3,0,(DWORD)ReplaceHistoryName,DIF_HISTORY/*|DIF_USELASTHISTORY*/,0,"",
    /*  5 */DI_TEXT,3,6,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    /*  6 */DI_CHECKBOX,5,7,0,0,0,0,0,0,(char *)MEditSearchCase,
    /*  7 */DI_CHECKBOX,5,8,0,0,0,0,0,0,(char *)MEditSearchWholeWords,
    /*  8 */DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MEditSearchReverse,
    /*  9 */DI_TEXT,3,10,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    /* 10 */DI_BUTTON,0,11,0,0,0,0,DIF_CENTERGROUP,1,(char *)MEditReplaceReplace,
    /* 11 */DI_BUTTON,0,11,0,0,0,0,DIF_CENTERGROUP,0,(char *)MEditSearchCancel
    };
    /* KM $ */
    HeightDialog=14;
    MakeDialogItems(ReplaceDlgData,ReplaceDlg);
    if(!*TextHistoryName)
    {
      ReplaceDlg[2].History=0;
      ReplaceDlg[2].Flags&=~DIF_HISTORY;
    }
    if(!*ReplaceHistoryName)
    {
      ReplaceDlg[4].History=0;
      ReplaceDlg[4].Flags&=~DIF_HISTORY;
    }

    strncpy(ReplaceDlg[2].Data,(char *)SearchStr,sizeof(ReplaceDlg[2].Data)-1);
    strncpy(ReplaceDlg[4].Data,(char *)ReplaceStr,sizeof(ReplaceDlg[4].Data)-1);

    if(Case)
      ReplaceDlg[6].Selected=*Case;
    else
    {
      HeightDialog--;
      ReplaceDlg[0].Y2--;
      ReplaceDlg[6].Type=DI_TEXT;
      ReplaceDlg[6].Data[0]=0;
      for(I=7; I < sizeof(ReplaceDlg)/sizeof(ReplaceDlg[0]); ++I)
      {
        ReplaceDlg[I].Y1--;
        ReplaceDlg[I].Y2--;
      }
    }

    if(WholeWords)
      ReplaceDlg[7].Selected=*WholeWords;
    else
    {
      HeightDialog--;
      ReplaceDlg[0].Y2--;
      ReplaceDlg[7].Type=DI_TEXT;
      ReplaceDlg[7].Data[0]=0;
      for(I=8; I < sizeof(ReplaceDlg)/sizeof(ReplaceDlg[0]); ++I)
      {
        ReplaceDlg[I].Y1--;
        ReplaceDlg[I].Y2--;
      }
    }

    if(Reverse)
      ReplaceDlg[8].Selected=*Reverse;
    else
    {
      HeightDialog--;
      ReplaceDlg[0].Y2--;
      ReplaceDlg[8].Type=DI_TEXT;
      ReplaceDlg[8].Data[0]=0;
      for(I=9; I < sizeof(ReplaceDlg)/sizeof(ReplaceDlg[0]); ++I)
      {
        ReplaceDlg[I].Y1--;
        ReplaceDlg[I].Y2--;
      }
    }

    // ��� �� ����� 2 �������������� �����
    if(HeightDialog == 11)
    {
      for(I=9; I < sizeof(ReplaceDlg)/sizeof(ReplaceDlg[0]); ++I)
      {
        ReplaceDlg[I].Y1--;
        ReplaceDlg[I].Y2--;
      }
    }
    {
      Dialog Dlg(ReplaceDlg,sizeof(ReplaceDlg)/sizeof(ReplaceDlg[0]));
      Dlg.SetPosition(-1,-1,76,HeightDialog);
      Dlg.Process();
      if (Dlg.GetExitCode()!=10)
        return FALSE;
    }
    strncpy((char *)SearchStr,ReplaceDlg[2].Data,LenSearchStr-1);
    strncpy((char *)ReplaceStr,ReplaceDlg[4].Data,LenReplaceStr-1);
    if(Case)       *Case=ReplaceDlg[6].Selected;
    if(WholeWords) *WholeWords=ReplaceDlg[7].Selected;
    if(Reverse)    *Reverse=ReplaceDlg[8].Selected;
  }
  else
  {
/*
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345
00
01   +------------------------------ Search ------------------------------+
02   | Search for                                                         |
03   |                                                                   |
04   +--------------------------------------------------------------------+
05   | [ ] Case sensitive                                                 |
06   | [ ] Whole words                                                    |
07   | [ ] Reverse search                                                 |
08   +--------------------------------------------------------------------+
09   |                       [ Search ]  [ Cancel ]                       |
10   +--------------------------------------------------------------------+
*/
    static struct DialogData SearchDlgData[]={
    /*  0 */DI_DOUBLEBOX,3,1,72,10,0,0,0,0,(char *)MEditSearchTitle,
    /*  1 */DI_TEXT,5,2,0,0,0,0,0,0,(char *)MEditSearchFor,
    /*  2 */DI_EDIT,5,3,70,3,1,(DWORD)TextHistoryName,DIF_HISTORY|DIF_USELASTHISTORY,0,"",
    /*  3 */DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    /*  4 */DI_CHECKBOX,5,5,0,0,0,0,0,0,(char *)MEditSearchCase,
    /*  5 */DI_CHECKBOX,5,6,0,0,0,0,0,0,(char *)MEditSearchWholeWords,
    /*  6 */DI_CHECKBOX,5,7,0,0,0,0,0,0,(char *)MEditSearchReverse,
    /*  7 */DI_TEXT,3,8,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
    /*  8 */DI_BUTTON,0,9,0,0,0,0,DIF_CENTERGROUP,1,(char *)MEditSearchSearch,
    /*  9 */DI_BUTTON,0,9,0,0,0,0,DIF_CENTERGROUP,0,(char *)MEditSearchCancel
    };
    MakeDialogItems(SearchDlgData,SearchDlg);
    HeightDialog=12;

    if(!*TextHistoryName)
    {
      SearchDlg[2].History=0;
      SearchDlg[2].Flags&=~DIF_HISTORY;
    }

    strncpy(SearchDlg[2].Data,(char *)SearchStr,sizeof(SearchDlg[2].Data)-1);

    if(Case)
      SearchDlg[4].Selected=*Case;
    else
    {
      HeightDialog--;
      SearchDlg[0].Y2--;
      SearchDlg[4].Type=DI_TEXT;
      SearchDlg[4].Data[0]=0;
      for(I=5; I < sizeof(SearchDlgData)/sizeof(SearchDlgData[0]); ++I)
      {
        SearchDlg[I].Y1--;
        SearchDlg[I].Y2--;
      }
    }

    if(WholeWords)
      SearchDlg[5].Selected=*WholeWords;
    else
    {
      HeightDialog--;
      SearchDlg[0].Y2--;
      SearchDlg[5].Type=DI_TEXT;
      SearchDlg[5].Data[0]=0;
      for(I=6; I < sizeof(SearchDlgData)/sizeof(SearchDlgData[0]); ++I)
      {
        SearchDlg[I].Y1--;
        SearchDlg[I].Y2--;
      }
    }

    if(Reverse)
      SearchDlg[6].Selected=*Reverse;
    else
    {
      HeightDialog--;
      SearchDlg[0].Y2--;
      SearchDlg[6].Type=DI_TEXT;
      SearchDlg[6].Data[0]=0;
      for(I=7; I < sizeof(SearchDlgData)/sizeof(SearchDlgData[0]); ++I)
      {
        SearchDlg[I].Y1--;
        SearchDlg[I].Y2--;
      }
    }

    // ��� �� ����� 2 �������������� �����
    if(HeightDialog == 9)
    {
      for(I=7; I < sizeof(SearchDlgData)/sizeof(SearchDlgData[0]); ++I)
      {
        SearchDlg[I].Y1--;
        SearchDlg[I].Y2--;
      }
    }
    {
      Dialog Dlg(SearchDlg,sizeof(SearchDlg)/sizeof(SearchDlg[0]));
      Dlg.SetPosition(-1,-1,76,HeightDialog);
      Dlg.Process();
      if (Dlg.GetExitCode()!=8)
        return FALSE;
    }
    strncpy((char *)SearchStr,SearchDlg[2].Data,LenSearchStr-1);
    if(ReplaceStr) *ReplaceStr=0;
    if(Case)       *Case=SearchDlg[4].Selected;
    if(WholeWords) *WholeWords=SearchDlg[5].Selected;
    if(Reverse)    *Reverse=SearchDlg[6].Selected;
  }
  return TRUE;
}

/* $ 25.08.2000 SVS
   ! ������� GetString ����� ��� �������������� ����� (FIB_BUTTONS) ����������
     ��������� � ������ <Ok> & <Cancel>
*/
/* $ 01.08.2000 SVS
  ! ������� ����� ������ GetString ����� ���� �������� ��� ���� ������
*/
/* $ 31.07.2000 SVS
   ! ������� GetString ����� ��� ���� �������� - ��������� �� ���������� �����!
*/
// ������� ��� ��������� ��� Shift-F4 Shift-Enter ��� ���������� Shift ;-)
static long WINAPI GetStringDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
/*
  if(Msg == DM_KEY)
  {
//    char KeyText[50];
//    KeyToText(Param2,KeyText);
//    _D(SysLog("%s (0x%08X) ShiftPressed=%d",KeyText,Param2,ShiftPressed));
    if(ShiftPressed && Param2 == KEY_ENTER && !CtrlObject->Macro.IsExecuting())
    {
      DWORD Arr[1]={KEY_SHIFTENTER};
      Dialog::SendDlgMessage(hDlg,Msg,Param1,(long)Arr);
      return TRUE;
    }
  }
*/
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

/* $ 07.12.2001 IS
   + ��������� ����������������� ���-�����
*/
int WINAPI GetString(const char *Title,const char *Prompt,
                     const char *HistoryName,const char *SrcText,
    char *DestText,int DestLength,const char *HelpTopic,DWORD Flags,
    int *CheckBoxValue,const char *CheckBoxText)
{
  int Substract=5; // �������������� �������� :-)
  int ExitCode;
  bool addCheckBox=Flags&FIB_CHECKBOX && CheckBoxValue && CheckBoxText;
  int offset=addCheckBox?2:0;
/*
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345
|0                                                                             |
|1   +------------------------------- Title -------------------------------+   |
|2   | Prompt                                                              |   |
|3   | *******************************************************************|   |
|4   +---------------------------------------------------------------------+   |
|5   + [x] CheckBox                                                        +   |
|6   +---------------------------------------------------------------------+   |
|7   |                         [ Ok ]   [ Cancel ]                         |   |
|8   +---------------------------------------------------------------------+   |
|9                                                                             |
*/
  static struct DialogData StrDlgData[]=
  {
/*      Type          X1 Y1 X2  Y2 Focus Flags             DefaultButton
                                      Selected               Data
*/
/* 0 */ DI_DOUBLEBOX, 3, 1, 72, 4, 0, 0, 0,                0,"",
/* 1 */ DI_TEXT,      5, 2,  0, 0, 0, 0, DIF_SHOWAMPERSAND,0,"",
/* 2 */ DI_EDIT,      5, 3, 70, 3, 1, 0, 0,                1,"",
/* 3 */ DI_TEXT,      0, 4,  0, 0, 0, 0, DIF_SEPARATOR,    0,"",
/* 4 */ DI_CHECKBOX,  5, 5,  0, 0, 0, 0, 0,                0,"",
/* 5 */ DI_TEXT,      0, 6,  0, 0, 0, 0, DIF_SEPARATOR,    0,"",
/* 6 */ DI_BUTTON,    0, 7,  0, 0, 0, 0, DIF_CENTERGROUP,  0,"",
/* 7 */ DI_BUTTON,    0, 7,  0, 0, 0, 0, DIF_CENTERGROUP,  0,""
  };
  MakeDialogItems(StrDlgData,StrDlg);

  if(addCheckBox)
  {
    Substract-=2;
    StrDlg[0].Y2+=2;
    StrDlg[4].Selected=(*CheckBoxValue)?TRUE:FALSE;
    strcpy(StrDlg[4].Data,CheckBoxText);
  }

  if(Flags&FIB_BUTTONS)
  {
    Substract-=3;
    StrDlg[0].Y2+=2;
    StrDlg[2].DefaultButton=0;
    StrDlg[4+offset].DefaultButton=1;
    StrDlg[5+offset].Y1=StrDlg[4+offset].Y1=5+offset;
    StrDlg[4+offset].Type=StrDlg[5+offset].Type=DI_BUTTON;
    StrDlg[4+offset].Flags=StrDlg[5+offset].Flags=DIF_CENTERGROUP;
    strcpy(StrDlg[4+offset].Data,FarMSG(MOk));
    strcpy(StrDlg[5+offset].Data,FarMSG(MCancel));
  }

  if(Flags&FIB_EXPANDENV)
  {
    StrDlg[2].Flags|=DIF_EDITEXPAND;
  }
  if(Flags&FIB_EDITPATH)
  {
    StrDlg[2].Flags|=DIF_EDITPATH;
  }

  if (HistoryName!=NULL)
  {
    StrDlg[2].Selected=(int)HistoryName;
    /* $ 09.08.2000 SVS
       ���� ��� ������������� ���� �������� �� ������� �������� ��������!!!
    */
    StrDlg[2].Flags|=DIF_HISTORY|(Flags&FIB_NOUSELASTHISTORY?0:DIF_USELASTHISTORY);
    /* SVS $ */
  }

  if (Flags&FIB_PASSWORD)
    StrDlg[2].Type=DI_PSWEDIT;

  if(Title)
    strcpy(StrDlg[0].Data,Title);
  if(Prompt)
  {
    strcpy(StrDlg[1].Data,Prompt);
    if(Flags&FIB_NOAMPERSAND)
      StrDlg[1].Flags&=~DIF_SHOWAMPERSAND;
  }
  if(DestLength > 511 && !(Flags&FIB_PASSWORD))
  {
    StrDlg[2].Flags|=DIF_VAREDIT;
    StrDlg[2].Ptr.PtrTail[0]=StrDlg[2].Ptr.PtrFlags=0;
    if(SrcText)
      memmove(DestText,SrcText,(strlen(SrcText)+1>DestLength?DestLength:strlen(SrcText)+1));
    StrDlg[2].Ptr.PtrData=DestText;
    StrDlg[2].Ptr.PtrLength=DestLength;
  }
  else
  {
    if(SrcText)
      strncpy(StrDlg[2].Data,SrcText,sizeof(StrDlg[2].Data)-1);
    StrDlg[2].Data[sizeof(StrDlg[2].Data)-1]=0;
  }

  {
    Dialog Dlg(StrDlg,sizeof(StrDlg)/sizeof(StrDlg[0])-Substract,GetStringDlgProc);
    Dlg.SetPosition(-1,-1,76,offset+((Flags&FIB_BUTTONS)?8:6));

    if (HelpTopic!=NULL)
      Dlg.SetHelp(HelpTopic);
#if 0
    if(Opt.ExceptRules)
    {
      TRY{
        Dlg.Process();
      }
      EXCEPT ( xfilter(EXCEPT_FARDIALOG,
                       GetExceptionInformation(),NULL,1)) // NULL=???
      {
        return FALSE;
      }
    }
    else
#endif
    {
      Dlg.Process();
    }
    ExitCode=Dlg.GetExitCode();
  }

  if (DestLength >= 1 && (ExitCode == 2 || ExitCode == 4 ||
      (addCheckBox && ExitCode == 6))
     )
  {
    if(!(Flags&FIB_ENABLEEMPTY) &&
       (!(StrDlg[2].Flags&DIF_VAREDIT) && *StrDlg[2].Data==0 ||
        (StrDlg[2].Flags&DIF_VAREDIT) && *(char *)StrDlg[2].Ptr.PtrData==0
       )
      )
      return(FALSE);
    if(!(StrDlg[2].Flags&DIF_VAREDIT))
    {
      strncpy(DestText,StrDlg[2].Data,DestLength-1);
      DestText[DestLength-1]=0;
    }
    if(addCheckBox)
      *CheckBoxValue=StrDlg[4].Selected;
    return(TRUE);
  }
  return(FALSE);
}
/* IS $ */
/* SVS $*/
/* 01.08.2000 SVS $*/
/* 25.08.2000 SVS $*/

/*
  ����������� ������ ����� ������.
  ����� ��� ���������� ���������� ������ � ������.

  Name      - ���� ����� ������� ����� (max 256 ��������!!!)
  Password  - ���� ����� ������� ������ (max 256 ��������!!!)
  Title     - ��������� ������� (����� ���� NULL)
  HelpTopic - ���� ������ (����� ���� NULL)
  Flags     - ����� (GNP_*)
*/
int WINAPI GetNameAndPassword(char *Title,char *UserName,char *Password,char *HelpTopic,DWORD Flags)
{
  static char LastName[256],LastPassword[256];
  int ExitCode;
/*
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345
|0                                                                             |
|1   +------------------------------- Title -------------------------------+   |
|2   | User name                                                           |   |
|3   | *******************************************************************|   |
|4   | User password                                                       |   |
|5   | ******************************************************************* |   |
|6   +---------------------------------------------------------------------+   |
|7   |                         [ Ok ]   [ Cancel ]                         |   |
|8   +---------------------------------------------------------------------+   |
|9                                                                             |
*/
  static struct DialogData PassDlgData[]=
  {
/* 0 */ DI_DOUBLEBOX,  3, 1,72, 8,0,0,0,0,"",
/* 1 */ DI_TEXT,       5, 2, 0, 0,0,0,0,0,"",
/* 2 */ DI_EDIT,       5, 3,70, 3,1,0,DIF_USELASTHISTORY|DIF_HISTORY,0,"",
/* 3 */ DI_TEXT,       5, 4, 0, 0,0,0,0,0,"",
/* 4 */ DI_PSWEDIT,    5, 5,70, 3,0,0,0,0,"",
/* 5 */ DI_TEXT,       3, 6, 0, 0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
/* 6 */ DI_BUTTON,     0, 7, 0, 0,0,0,DIF_CENTERGROUP,1,"",
/* 7 */ DI_BUTTON,     0, 7, 0, 0,0,0,DIF_CENTERGROUP,0,""
  };
  MakeDialogItems(PassDlgData,PassDlg);

  strcpy(PassDlg[1].Data,MSG(MNetUserName));
  strcpy(PassDlg[3].Data,MSG(MNetUserPassword));
  strcpy(PassDlg[6].Data,MSG(MOk));
  strcpy(PassDlg[7].Data,MSG(MCancel));
  if (Title!=NULL)
    strncpy(PassDlg[0].Data,Title,sizeof(PassDlg[0].Data)-1);
  strncpy(PassDlg[2].Data,(Flags&GNP_USELAST)?LastName:UserName,sizeof(LastName)-1);
  strncpy(PassDlg[4].Data,(Flags&GNP_USELAST)?LastPassword:Password,sizeof(LastPassword)-1);

  {
    Dialog Dlg(PassDlg,sizeof(PassDlg)/sizeof(PassDlg[0]));
    Dlg.SetPosition(-1,-1,76,10);

    if (HelpTopic!=NULL)
      Dlg.SetHelp(HelpTopic);

    Dlg.Process();
    ExitCode=Dlg.GetExitCode();
  }

  if (ExitCode!=6)
    return(FALSE);

  // ���������� ������.
  strcpy(LastName,strncpy(UserName,PassDlg[2].Data,sizeof(LastName)-1));
  strcpy(LastPassword,strncpy(Password,PassDlg[4].Data,sizeof(LastPassword)-1));

  // Convert Name and Password to Ansi
  if(!(Flags&GNP_NOOEMTOCHAR))
  {
    OemToChar(UserName,UserName);
    OemToChar(Password,Password);
  }
  return(TRUE);
}

/* ������ ���������� ������� �������
   ���� �� ������� ������ ������� (������ ���������), ���� ��
   ��������� "HotKey".
   ���� HotKey=NULL ���� RegKey=NULL, �� �� ��� �����!

   !!! ���� ����� �������� ��� �������� ������ ����� ������� ������ !!!

   Return: TRUE  - ��� ���
           FALSE - �������� ���������� ������
*/
BOOL WINAPI GetMenuHotKey(char *HotKey,          // ������, ����� ���� =NULL
                          int LenHotKey,         // ����� ������ (���. = 1)
                          char *DlgHotKeyTitle,  // ��������� �������
                          char *DlgHotKeyText,   // prompt ����������
                          char *HelpTopic,       // ���� ������, ����� ���� =NULL
                          char *RegKey,          // ����, ������ ����� ��������, ����� ���� =NULL
                          char *RegValueName)    // �������� ��������� �� �������, ����� ���� =NULL
{
  int ExitCode;
/*
�================ Assign plugin hot key =================�
� Enter hot key (letter or digit)                        �
� _                                                      �
L========================================================-
*/
  static struct DialogData PluginDlgData[]=
  {
    /* 00 */DI_DOUBLEBOX,3,1,60,4,0,0,0,0,"",
    /* 01 */DI_TEXT,5,2,0,0,0,0,0,0,"",
    /* 02 */DI_FIXEDIT,5,3,5,3,1,0,0,1,""
  };

  if(DlgHotKeyTitle) PluginDlgData[0].Data=(char*)DlgHotKeyTitle;
  if(DlgHotKeyText)  PluginDlgData[1].Data=(char*)DlgHotKeyText;

  MakeDialogItems(PluginDlgData,PluginDlg);

  if(RegKey && *RegKey)
    GetRegKey(RegKey,RegValueName,PluginDlg[2].Data,"",sizeof(PluginDlg[2].Data));
  else if(HotKey)
    strcpy(PluginDlg[2].Data,HotKey);
  else
    PluginDlg[2].Data[0]=0;

  PluginDlg[2].X2+=LenHotKey-1; // ��������, ���� ����

  {
    Dialog Dlg(PluginDlg,sizeof(PluginDlg)/sizeof(PluginDlg[0]));
    if(HelpTopic)
      Dlg.SetHelp(HelpTopic);
    Dlg.SetPosition(-1,-1,64,6);
    Dlg.Process();
    ExitCode=Dlg.GetExitCode();
  }

  if (ExitCode==2)
  {
    PluginDlg[2].Data[LenHotKey]=0;
    if(RegKey && *RegKey)
    {
      RemoveLeadingSpaces(PluginDlg[2].Data);
      if (*PluginDlg[2].Data==0)
        SetRegKey(RegKey,RegValueName,"");
        //DeleteRegKey(RegKey);
      else
        SetRegKey(RegKey,RegValueName,PluginDlg[2].Data);
    }

    if(HotKey) // ���������, ���� ����
      strcpy(HotKey,PluginDlg[2].Data);
    return TRUE;
  }
  return FALSE;
}
