/*
stddlg.cpp

��� ࠧ��� �⠭������ ��������

*/

/* Revision: 1.03 23.01.2001 $ */

/*
Modify:
  28.01.2001 SVS
    ! DumpExeptionInfo -> DumpExceptionInfo ;-)
  23.01.2001 SVS
    + ������� ������� �᥯設� :-)
  23.01.2001 SVS
    - �� ��� � ��ࢠ� ���� � ������� ���᪠/������ :-(
  21.01.2001 SVS
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
    + �㭪�� GetString ��॥堫� �� mix.cpp
    + GetSearchReplaceString - �८�ࠧ����� �� editor.cpp
*/

#include "headers.hpp"
#pragma hdrstop
#include "internalheaders.hpp"


/*
  �㭪�� GetSearchReplaceString �뢮��� ������ ���᪠ ��� ������, �ਭ�����
  �� ���짮��⥫� ����� � � ��砥 �ᯥ譮�� �믮������ ������� �����頥�
  TRUE.
  ��ࠬ����:
    IsReplaceMode
      TRUE  - �᫨ �⨬ ��������
      FALSE - �᫨ �⨬ �᪠��

    SearchStr
      �����⥫� �� ��ப� ���᪠.
      ������� ��ࠡ�⪨ ������� �������� � ��� ��.

    ReplaceStr,
      �����⥫� �� ��ப� ������.
      ������� ��ࠡ�⪨ ������� �������� � ��� ��.
      ��� ����, �᫨ IsReplaceMode=FALSE ����� ���� ࠢ�� NULL

    TextHistoryName
      ��� ���ਨ ��ப� ���᪠.
      �᫨ ��⠭������ � NULL, � �� 㬮�砭��
      �ਭ������� ���祭�� "SearchText"
      �᫨ ��⠭������ � ������ ��ப�, � ����� ������ �� �㤥�

    ReplaceHistoryName
      ��� ���ਨ ��ப� ������.
      �᫨ ��⠭������ � NULL, � �� 㬮�砭��
      �ਭ������� ���祭�� "ReplaceText"
      �᫨ ��⠭������ � ������ ��ப�, � ����� ������ �� �㤥�

    *Case
      �����⥫� �� ��६�����, 㪠�뢠���� �� ���祭�� ��樨 "Case sensitive"
      �᫨ = NULL, � �ਭ������� ���祭�� 0 (�����஢��� ॣ����)

    *WholeWords
      �����⥫� �� ��६�����, 㪠�뢠���� �� ���祭�� ��樨 "Whole words"
      �᫨ = NULL, � �ਭ������� ���祭�� 0 (� ⮬ �᫥ � �����ப�)

    *Reverse
      �����⥫� �� ��६�����, 㪠�뢠���� �� ���祭�� ��樨 "Reverse search"
      �᫨ = NULL, � �ਭ������� ���祭�� 0 (��אַ� ����)

  �����頥��� ���祭��:
    TRUE  - ���짮��⥫� ���⢥न� ᢮� ����ਭ��
    FALSE - ���짮��⥫� �⪠����� �� ������� (Esc)
*/
int WINAPI GetSearchReplaceString(
         int IsReplaceMode,
         unsigned char *SearchStr,
         unsigned char *ReplaceStr,
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

  if(!TextHistoryName)
    TextHistoryName=TextHistoryName0;
  if(!ReplaceHistoryName)
    ReplaceHistoryName=ReplaceHistoryName0;

  /* $ 03.08.2000 KM
     ���������� checkbox'�� � ������� ��� ���᪠ 楫�� ᫮�
  */
  if (IsReplaceMode)
  {
/*
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345
00
01   ������������������������������ Replace �����������������������������ͻ
02   � Search for                                                         �
03   �                                                                   �
04   � Replace with                                                       �
05   �                                                                   �
06   ��������������������������������������������������������������������Ķ
07   � [ ] Case sensitive                                                 �
08   � [ ] Whole words                                                    �
09   � [ ] Reverse search                                                 �
10   ��������������������������������������������������������������������Ķ
11   �                      [ Replace ]  [ Cancel ]                       �
12   ��������������������������������������������������������������������ͼ
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

    strncpy(ReplaceDlg[2].Data,(char *)SearchStr,sizeof(ReplaceDlg[2].Data));
    strncpy(ReplaceDlg[4].Data,(char *)ReplaceStr,sizeof(ReplaceDlg[4].Data));
    ReplaceDlg[6].Selected=Case?*Case:0;
    ReplaceDlg[7].Selected=WholeWords?*WholeWords:0;
    ReplaceDlg[8].Selected=Reverse?*Reverse:0;

    Dialog Dlg(ReplaceDlg,sizeof(ReplaceDlg)/sizeof(ReplaceDlg[0]));
    Dlg.SetPosition(-1,-1,76,14);
    Dlg.Process();
    if (Dlg.GetExitCode()!=10)
      return FALSE;

    strcpy((char *)SearchStr,ReplaceDlg[2].Data);
    strcpy((char *)ReplaceStr,ReplaceDlg[4].Data);
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
01   ������������������������������� Search �����������������������������ͻ
02   � Search for                                                         �
03   �                                                                   �
04   ��������������������������������������������������������������������Ķ
05   � [ ] Case sensitive                                                 �
06   � [ ] Whole words                                                    �
07   � [ ] Reverse search                                                 �
08   ��������������������������������������������������������������������Ķ
09   �                       [ Search ]  [ Cancel ]                       �
10   ��������������������������������������������������������������������ͼ
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

    if(!*TextHistoryName)
    {
      SearchDlg[2].History=0;
      SearchDlg[2].Flags&=~DIF_HISTORY;
    }

    strncpy(SearchDlg[2].Data,(char *)SearchStr,sizeof(SearchDlg[2].Data));
    SearchDlg[4].Selected=Case?*Case:0;
    SearchDlg[5].Selected=WholeWords?*WholeWords:0;
    SearchDlg[6].Selected=Reverse?*Reverse:0;

    Dialog Dlg(SearchDlg,sizeof(SearchDlg)/sizeof(SearchDlg[0]));
    Dlg.SetPosition(-1,-1,76,12);
    Dlg.Process();
    if (Dlg.GetExitCode()!=8)
      return FALSE;

    strcpy((char *)SearchStr,SearchDlg[2].Data);
    if(ReplaceStr) *ReplaceStr=0;
    if(Case)       *Case=SearchDlg[4].Selected;
    if(WholeWords) *WholeWords=SearchDlg[5].Selected;
    if(Reverse)    *Reverse=SearchDlg[6].Selected;
  }
  return TRUE;
}

/* $ 25.08.2000 SVS
   ! �㭪�� GetString ����� �� ᮮ⢥����饬 䫠�� (FIB_BUTTONS) �⮡ࠦ���
     ᥯���� � ������ <Ok> & <Cancel>
*/
/* $ 01.08.2000 SVS
  ! �㭪�� ����� ��ப� GetString ����� ���� ��ࠬ��� ��� ��� 䫠���
*/
/* $ 31.07.2000 SVS
   ! �㭪�� GetString ����� �� ���� ��ࠬ��� - ������� �� ��६���� �।�!
*/
int WINAPI GetString(char *Title,char *Prompt,char *HistoryName,char *SrcText,
    char *DestText,int DestLength,char *HelpTopic,DWORD Flags)
{
  int Substract=3; // �������⥫쭠� ����稭� :-)
  int ExitCode;
/*
  0         1         2         3         4         5         6         7
  0123456789012345678901234567890123456789012345678901234567890123456789012345
�0                                                                             �
�1   �������������������������������� Title ������������������������������ͻ   �
�2   � Prompt                                                              �   �
�3   � ��������������������������������������������������������������������   �
�4   ���������������������������������������������������������������������Ķ   �
�5   �                         [ Ok ]   [ Cancel ]                         �   �
�6   ���������������������������������������������������������������������ͼ   �
�7                                                                             �
*/
  static struct DialogData StrDlgData[]=
  {
/*      Type          X1 Y1 X2  Y2 Focus Flags             DefaultButton
                                      Selected               Data
*/
/* 0 */ DI_DOUBLEBOX, 3, 1, 72, 4, 0, 0, 0,                0,"",
/* 1 */ DI_TEXT,      5, 2,  0, 0, 0, 0, DIF_SHOWAMPERSAND,0,"",
/* 2 */ DI_EDIT,      5, 3, 70, 3, 1, 0, 0,                1,"",
/* 3 */ DI_TEXT,      0, 4,  0, 4, 0, 0, DIF_SEPARATOR,    0,"",
/* 4 */ DI_BUTTON,    0, 5,  0, 0, 0, 0, DIF_CENTERGROUP,  0,"",
/* 5 */ DI_BUTTON,    0, 5,  0, 0, 0, 0, DIF_CENTERGROUP,  0,""
  };
  MakeDialogItems(StrDlgData,StrDlg);

  if(Flags&FIB_BUTTONS)
  {
    Substract=0;
    StrDlg[0].Y2+=2;
    StrDlg[2].DefaultButton=0;
    StrDlg[4].DefaultButton=1;
    strcpy(StrDlg[4].Data,FarMSG(MOk));
    strcpy(StrDlg[5].Data,FarMSG(MCancel));
  }

  if(Flags&FIB_EXPANDENV)
  {
    StrDlg[2].Flags|=DIF_EDITEXPAND;
  }

  if (HistoryName!=NULL)
  {
    StrDlg[2].Selected=(int)HistoryName;
    /* $ 09.08.2000 SVS
       䫠� ��� �ᯮ�짮����� �। ���祭�� �� ���ਨ �������� �⤥�쭮!!!
    */
    StrDlg[2].Flags|=DIF_HISTORY|(Flags&FIB_NOUSELASTHISTORY?0:DIF_USELASTHISTORY);
    /* SVS $ */
  }

  if (Flags&FIB_PASSWORD)
    StrDlg[2].Type=DI_PSWEDIT;

  if(Title)
    strcpy(StrDlg[0].Data,Title);
  if(Prompt)
    strcpy(StrDlg[1].Data,Prompt);
  if(SrcText)
    strncpy(StrDlg[2].Data,SrcText,sizeof(StrDlg[2].Data));
  StrDlg[2].Data[sizeof(StrDlg[2].Data)-1]=0;

  TRY{
    Dialog Dlg(StrDlg,sizeof(StrDlg)/sizeof(StrDlg[0])-Substract);
    Dlg.SetPosition(-1,-1,76,(Flags&FIB_BUTTONS)?8:6);

    if (HelpTopic!=NULL)
      Dlg.SetHelp(HelpTopic);

    Dlg.Process();
    ExitCode=Dlg.GetExitCode();
  }
  __except (DumpExceptionInfo(GetExceptionInformation()))
  {
    return FALSE;
  }

  if (DestLength >= 1 && (ExitCode == 2 || ExitCode == 4))
  {
    if(!(Flags&FIB_ENABLEEMPTY) && *StrDlg[2].Data==0)
    {
      return(FALSE);
    }
    strncpy(DestText,StrDlg[2].Data,DestLength);
    DestText[DestLength-1]=0;
    return(TRUE);
  }
  return(FALSE);
}
/* SVS $*/
/* 01.08.2000 SVS $*/
/* 25.08.2000 SVS $*/
