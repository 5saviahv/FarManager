/*
dialog.cpp

����� �������

*/

/* Revision: 1.48 20.10.2000 $ */

/*
Modify:
  20.10.2000 SVS
    + DM_GETFOCUS - ������� ID ����� ����饣� 䮪�� �����
  16.10.2000 tran 1.47
   + ��� EDIT ����� ���⠢����� ��࠭�祭�� � 511 ᨬ�����
  27.09.2000 SVS
   ! Alt-Up/Down/Left/Right - �ࠫ (�⮡� � ���饬 �� ���ᥪ�����
     � MultiEdit)
   ! Ctrl-Alt-Shift - ॠ���㥬, �᫨ ����.
  24.09.2000 SVS
   + �������� ������� - Alt-��५��
   + �맮� �㭪樨 Xlat
  22.09.2000 SVS
   ! ��筥��� AutoComplete �� ����ﭭ�� ������.
  20.09.2000 SVS
   ! Enter � ��ப�� ����� (�஬� DIF_EDITOR) �����蠥� ������.
  18.09.2000 SVS
   + DIF_READONLY - 䫠� ��� ��ப ।���஢����
      (����! ��� ��ப ।���஢����).
  18.09.2000 SVS
   ! ��筥��� ��� SelectOnEntry
   ! ��᪠ �� ������ ���� ���⮩ (��ப� �� �஡���� �� ���뢠����)!
  14.09.2000 SVS
   + ���� DIF_LISTNOAMPERSAND. �� 㬮�砭�� ��� DI_LISTBOX
      ���⠢����� 䫠� MENU_SHOWAMPERSAND. ��� 䫠� �������� ⠪��
      ���������
  12.09.2000 SVS
   ! ������ ��������� ��� ������ � DefaultButton=1:
     ����� ������ ������ᨬ� �� �⨫� ������� ���樨��� ᮮ�饭�� DM_CLOSE.
   ! ��ࠢ�塞 ����� � BackSpace � DIF_EDITOR
   ! ��蠥� �஡����, �᫨ Del ������ � ����樨 ����襩, 祬 �����
     ��ப� (DIF_EDITOR)
  11.09.2000 SVS
   + Ctrl-U � ��ப�� ����� ᭨���� ������ �����
  09.09.2000 SVS
   + DIF_NOFOCUS - ����� �� ����砥� 䮪�� ����� (��������ன)
   + �⨫� ������� DMODE_OLDSTYLE - ������ � ��஬ �⨫�.
   ! �뢠���� �� �������, ⪭� ��� �������, ᬮ��� ⮫쪮 � ��஬ �⨫�.
   + �㭪�� SelectOnEntry - �뤥����� ��ப� ।���஢����
     (��ࠡ�⪠ 䫠�� DIF_SELECTONENTRY)
  08.09.2000 SVS
   - �᫨ ���⪮, � DM_SETFOCUS �த� ��� � ࠡ�⠫ :-)
   ! ��筥��� ��� DN_MOUSECLICK
  31.08.2000 SVS
   + DM_ENABLE (�� ��������� ��⮢ :-)
   - ���� � �맮��� 䠩��� �����.
  30.08.2000 SVS
   + ��⮤ Hide()
   + ����� ������� DMODE_SHOW - ������ �����?
   ! ��筥��� ��� IsEnableRedraw
   + DM_MOVEDIALOG - ��६�饭�� �������.
   ! ��������� 梥⮢ ��� ComboBox(DowpDownList)
  29.08.2000 SVS
   ! �� ������� ⥬� ����� �� �������� ��楤���...
     ����, �㦭� ����� �ନ஢��� ���⥭�!
  29.08.2000 SVS
   - ���� ��樠��� ����-��� - �㭪�� ProcessHighlighting
     MY> ����� � ��������� �⠫� �������. � ���� � ���� ����稥 ������.
     MY> �室�� � ����ன�� 祣� 㣮���, ���� Alt-�㦭�� �㪢� �
     MY> ������ ࠧ����ࠧ�� ��.
  28.08.2000 SVS
   - ��� �� �ਢ�� (��� �� �����⥫쭮��!) :-)
  25.08.2000 SVS
   + DM_GETDLGRECT - ������� ���न���� ����������� ����
   ! ��筥��� ��� DN_MOUSECLICK
  25.08.2000 SVS
   ! ��筥���, �⭮�⥫쭮 䮪�� ����� - �� � �� ᪠��窨, ����.
  24.08.2000 SVS
   + InitDialogObjects ����� ��ࠬ��� - ��� �롮�筮� २��樠����樨
     ����⮢
  24.08.2000 SVS
   ! ��७����� ���!!!! (����� ⮫쪮 ��⮬)... �� �� ComboBox
     �⮣� ���� �� �����...
   + CtrlAltShift - ������/�������� ������...
   + ������� DI_USERCONTROL - ���ᮢ��� ���������� ������.
  24.08.2000 SVS
   ! ����᪠� �訡�� - � 䮪�ᮢ �� ���浪�...
     �����ࠫ�� � ChangeFocus()
  23.08.2000 SVS
   ! ��筥��� ��⥣�਩ DMSG_* -> DM_ (��ᠣ) & DN_ (����)
   + DM_KEY        - ��᫠��/������� �������(�)
   + DM_GETDLGDATA - ����� ����� �������.
   + DM_SETDLGDATA - ��⠭����� ����� �������.
   + DM_SHOWDIALOG - ��������/������ ������
   + ��६����� ����� FocusPos - �ᥣ�� �����⭮ ����� ����� � 䮪��
   ! ��६���� IsCanMove, InitObjects, CreateObjects, WarningStyle, Dragged
     㤠���� -> ��⮢� 䫠�� � DialogMode
   + ������⢮ ������ ��筥��� ;-)
  22.08.2000 SVS
   ! � ���� ������᪨� ����� �� ��� :-((
     IsMovedDialog -> IsCanMove
   ! DMSG_PAINT -> DMSG_DRAWDIALOG
   ! DMSG_DRAWITEM -> DMSG_DRAWDLGITEM
   ! DMSG_CHANGELIST -> DMSG_LISTCHANGE
   ! ShowDialog - �������⥫�� ��ࠬ��� - ����� ����� ���ᮢ뢠��
  21.08.2000 SVS 1.23
   ! ���ᠭ�� ᮮ�饭�� �࠭� - ᬮ��� � 奫�� :-)))
   ! ���묨 ��᫥����� � �⮬ 䠩�� ������ ���� DefDlgProc � SendDlgMessage
     (��� ⠪ 㤮��� :-)
   + ��। ���ᮢ��� DI_LISTBOX ��ᨬ �� ��������� 梥⮢�� ��ਡ�⮢
   ! DIF_HISTORY ����� ����� ��᮪�� �ਮ���, 祬 DIF_MASKEDIT
   - ��� � AutoComplete �� ����祭��� ����ﭭ�� ������
   ! DMSG_CHANGEITEM -> DMSG_EDITCHANGE
   + DMSG_BTNCLICK
  12.08.2000 KM 1.20
   + ���������� ࠡ��� DI_FIXEDIT � 䫠��� DIF_MASKEDIT ��� ��⠭����
     ��᪨ � ��ப� �����.
  18.08.2000 SVS
   + DialogMode - ����� ⥪�饣� ०��� �������
   + ���� IsEnableRedraw - ࠧ���騩/������騩 ����ᮢ�� �������
   + ����饭��: DMSG_ENABLEREDRAW, DMSG_MOUSECLICK
   + DI_BUTTON ⮦� ⥯��� ����� ����� DIF_SETCOLOR
   + ���� ��� DI_BUTTON - DIF_BTNNOCLOSE - "������ �� ��� ������� �������"
   - �᫨ �� ��室 ��楤�� �⢥⨫� "���", � ������ ��横�������, �.�.
     �� �� ��襭 䫠� ��室�.
  15.08.2000 SVS
   ! ��� DropDownList 梥� ��ࠡ��뢠�� �� �����.
   + ������� ⠪, �⮡� ⪭� ��誮� � DropDownList ᯨ᮪ ���뢠��� ᠬ.
  11.08.2000 SVS
   + �����, ᯥ���᪨� ��� �����⭮�� ������� �������
   + ��� ⮣�, �⮡� ��᫠�� DMSG_CLOSE �㦭� ��८�।����� Process
   ! ��筥��� ��� DMSG_CLOSE
  11.08.2000 KM 1.18
   ! ��࠭ �㡫����騩 ���, ��ࠢ���騩 �����४⭮� ��६�饭�� �������
     ��誮�. ����뢠���� �� � ���॥� ᤥ���� ���稪 � ���� ���� :)
   ! ����-���� ��।����� �᪠��� ��誮� �������. �뫮: ��᫥ ��ࢮ�� ������
     ��誮� �� ������� �⮡ࠦ���� ०��� ��६�饭�� �� ����砫��� �� �� ���
     ���� �� ��稭����� ᠬ� �������� �������, ��᫥ 祣� � �ந�室���
     �⮡ࠦ���� ��砫� �⮣� ०���, �� ���� � ��୥ ���������᪨ ����୮ :)
   ! ���䠪�: �᫨ ��᫥ ��ࢮ�� ����᪠ ��, �� ��ଥ�� ����
     ������ ����� ������, �⮡� ��६����� ������ (������ ��室���� ��� �����),
     � ��-�� ⮣�, �� � �⮬� ������� PrevMouseX � PrevMouseY ��� �� ��।�����,
     ⠪�� ����।���� ����砫�� ��릮� �������.
  10.08.2000 SVS
   + ��६����� IsMovedDialog - ����� �� ������� ������ :-)
   + �㭪�� ��⠭���� IsMovedDialog
  09.08.2000 tran 1.16
   - �࠭� "ᠫ����"
  09.08.2000 KM
   ! �� ����祭�� ०��� ��६�饭�� ������� ���������
     �⪫�祭�� �����饣� �����. ��� ��ᬥ⨪�.
   ! ���ࠢ���� ��६�饭�� ������� ��誮�, ���३-� �� ���⥫
     ��ࠢ���� :). ������ ��६�饭�� �⠫� ���४��, ��� �ப��뢠���
     ������� �� �࠭�.
   ! �� ��室� �� �ࠩ �࠭� ��������, ⥭�, �� ����������, �த������
     �ᮢ�����.

     ����� ।��樨 ��⠫�� �०��� - ����� �뫮
     ��������� �����.
  07.08.2000 SVS
   + � �㭪樨 ��ࠢ������� ���ন��� �� ListBox ���뫨!
  04.08.2000 SVS
    + FarListItems.CountItems -> FarListItems.ItemsNumber
  03.08.2000 tran
   + ��設� ����� ������� - 墠⠭�� �� ���⮥ ����
     �������� - ��࠭�祭�� ����室� �� �࠭��� �࠭� ���⠫��� �� ���
     ���� ������ ��...
     �������� �������� - �� 㣫�� ���� �����
     � MOVE � 㣫� ����� ���� �� � ����ࠦ����.
     ��ப� �� MOVE ��������७� "//# "
  01.08.2000 SVS
   ! History ⥯��� ������ ����� ScrollBar, �.�. ��� ����� �����
     � ComboBox`�, ������ � ����.
   - ��६����� ����� lastKey 㤠���� �� ������������� :-)
   + ����� ListBox
   - ������让 ���祪 � AutoComplete
   ! � History ������ �������� ���祭�� (��� DIF_EXPAND...) ��।
     ���७��� �।�!
   + ��� �� �⮨� 䫠� DIF_USELASTHISTORY � ������� ��ப� �����,
     � ����⠭�������� ��ࢮ� ���祭�� �� History
   - �⪫�祭� ����������� ��� DI_PSWEDIT ����� History...
     ...�� �� ���뫮 �������... � ��� ����饭�� �����, �.�.
  31.07.2000 tran & SVS
   + ��६�饭�� ������� �� �࠭� �����蠬�. Ctrl-F5 ����砥� ०��
     ��६�饭��. �������� ��६�饭�� - "Move" � ����� ���孥� 㣫�
  28.07.2000 SVS
   ! ��६��� Edit *EditLine � �㭪樨 FindInEditForAC ��䨣 ���㦥�!
   - ������让 ��� � ��⮧����襭���:
       ...���� � ���� �� F7 - "templates". ��� F7, ������� shift+t=T � ...
       ������ �������� t. � �⮣� ������� ������� ⮫쪮 ����࠭��� ��
       ��᫥ �����쪮� � ��࠭��� ����...
   - �᫨ ������ �� ���⠢�� �� ������ ������ � 䮪�ᮬ,
     � �ਤ���� ᠬ��� �� �⮬ �����������, � ���⠢���
     䮪�� �� ��ࢮ� �ࠧ㬨⥫쭮� ����� ;-)
   + AutoComplite: ��� DI_COMBOBOX.
   ! SelectFromComboBox ����� �������⥫�� ��ࠬ��� � ⥬, �⮡�
     ����樮��஢��� item � ���� � ᯨ᪮� � ᮮ⢥�ᢨ� � ��ப�� �����
   ! FindInEditHistory -> FindInEditForAC
     ���� ��� � ���ਨ, ⠪ � � ComboBox`� (�⮡� �� ������� ����)
   + �㭪�� IsFocused, ��।������ - "����� �� ����� �������
     ����� 䮪�� �����"
   + �㭪�� ConvertItem - �८�ࠧ������ �� ����७���� �।�⠢�����
     � FarDialogItem � ���⭮
   + �����஥ ������⢮ ᮮ�饭��:
        DMSG_INITDIALOG, DMSG_ENTERIDLE, DMSG_HELP, DMSG_PAINT,
        DMSG_SETREDRAW, DMSG_DRAWITEM, DMSG_GETDLGITEM, DMSG_KILLFOCUS,
        DMSG_GOTFOCUS, DMSG_SETFOCUS, DMSG_GETTEXTLENGTH, DMSG_GETTEXT,
        DMSG_CTLCOLORDIALOG, DMSG_CTLCOLORDLGITEM, DMSG_CTLCOLORDLGLIST,
        DMSG_SETTEXTLENGTH, DMSG_SETTEXT, DMSG_CHANGEITEM, DMSG_HOTKEY,
        DMSG_CLOSE,
  26.07.2000 SVS
   + �� �������-� - ����������� ��।����㥬� ComboBox
  26.07.2000 SVS
   + AutoComplite: ��� DIF_HISTORY.
  25.07.2000 SVS
   + ���� ��ࠬ��� � ���������
  23.07.2000 SVS
   + ��� ६�ப � ��室����� :-)
   + ������� �맮� ��������� - ������� � ���� �㭪樨 ��ࠡ�⪨
   ! ��ப��� ����⠭�� "SavedDialogHistory\\%s",
     "Locked%d" � "Line%d" ᤥ���� �����������묨.
   + �㭪�� ��ࠡ�⪨ ������� (�� 㬮�砭��) DefDlgProc() - ����� ���� :-)
  19.07.2000 SVS
    ! "...� ।���� ������ ���� ������ home shift+end del
      ���� �� 㤠�����..."
      DEL � �⥬��, ������ DIF_EDITOR, ࠡ�⠫ ��� ��� �뤥�����...
  18.07.2000 SVS
    + ��ࠡ�⪠ ����� DI_COMBOBOX (���� �� �� ।����㥬���)
    + �㭪��-��ࠡ��稪 �롮� �� ᯨ᪠ - SelectFromComboBox
  11.07.2000 SVS
    ! ��������� ��� ���������� �������樨 ��� BC & VC
  05.07.2000 SVS
    + ��������� �஢�ઠ �� 䫠� DIF_EDITEXPAND - ���७�� ��६�����
      �।� � ����� ������� DI_EDIT
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


static char fmtLocked[]="Locked%d";
static char fmtLine[]  ="Line%d";
static char fmtSavedDialogHistory[]="SavedDialogHistory\\%s";

//////////////////////////////////////////////////////////////////////////
/* Public:
   ��������� ����� Dialog
*/
Dialog::Dialog(struct DialogItem *Item,int ItemCount,
               FARWINDOWPROC DlgProc,long InitParam)
{
  /* $ 29.08.2000 SVS
    ����� �������, �맢��襣� ������ (-1 = Main)
  */
  PluginNumber=-1;
  /* SVS $ */
  /* $ 23.08.2000 SVS
    ������ �������.
  */
  DialogMode=0;
  /* SVS $ */
  /* $ 11.08.2000 SVS
    + �����, ᯥ���᪨� ��� �����⭮�� ������� �������
  */
  Dialog::DataDialog=InitParam;
  /* SVS $ */
  DialogTooLong=0;
  /* $ 10.08.2000 SVS
     ����砫쭮 ������� ����� �᪠��
  */
  SetDialogMode(DMODE_ISCANMOVE);
  /* SVS $ */
  /* $ 18.08.2000 SVS
  */
  /*
    + ���� IsEnableRedraw - ࠧ���騩/������騩 ����ᮢ�� �������
      =0 - ࠧ�襭�, ��㣨� ���祭�� - �� ����ᮢ뢠��
        ����� ���뫠���� ᮮ�饭�� DMSG_ENABLEREDRAW, � ��� 䫠�
        �� Param1=TRUE 㢥��稢�����, �� Param1 = FALSE - 㬥��蠥���
  */
  IsEnableRedraw=0;

  FocusPos=-1;
  PrevFocusPos=-1;

  if(!DlgProc) // �㭪�� ������ ���� �ᥣ��!!!
  {
    DlgProc=(FARWINDOWPROC)Dialog::DefDlgProc;
    // ����� ������ � ��஬ �⨫� - ��⥬ ��� 䠪�!
    SetDialogMode(DMODE_OLDSTYLE);
  }
  Dialog::DlgProc=DlgProc;

  Dialog::Item=Item;
  Dialog::ItemCount=ItemCount;

  if (CtrlObject!=NULL)
  {
    // �������� �।. ०�� ����.
    PrevMacroMode=CtrlObject->Macro.GetMode();
    // ������� �㤥� � �������� :-)
    CtrlObject->Macro.SetMode(MACRO_DIALOG);
  }

  // ���������� �।��騩 ��������� ���᮫�
  GetConsoleTitle(OldConsoleTitle,sizeof(OldConsoleTitle));
}


//////////////////////////////////////////////////////////////////////////
/* Public, Virtual:
   �������� ����� Dialog
*/
Dialog::~Dialog()
{
  INPUT_RECORD rec;

  GetDialogObjectsData();
  DeleteDialogObjects();

  if (CtrlObject!=NULL)
    CtrlObject->Macro.SetMode(PrevMacroMode);

  Hide();
  ScrBuf.Flush();

  PeekInputRecord(&rec);
  SetConsoleTitle(OldConsoleTitle);
}


//////////////////////////////////////////////////////////////////////////
/* Public, Virtual:
   ����� ���祭�� ���न��� ���� ������� � �맮� �㭪樨
   ScreenObject::Show() ��� �뢮�� ������� �� �࠭.
*/
void Dialog::Show()
{
  if (X1 == -1) // ������ 業��஢���� ������� �� ��ਧ��⠫�?
  {             //   X2 �� �⮬ = �ਭ� �������.
    X1=(ScrX - X2 + 1)/2;

    if (X1 <= 0) // �ਭ� ������� ����� �ਭ� �࠭�?
    {
      DialogTooLong=X2-1;
      X1=0;
      X2=ScrX;
    }
    else
      X2+=X1-1;
  }

  if (Y1 == -1) // ������ 業��஢���� ������� �� ���⨪���?
  {             //   Y2 �� �⮬ = ���� �������.
    Y1=(ScrY - Y2 + 1)/2;

    if (Y1>1)
      Y1--;
    if (Y1>5)
      Y1--;
    if (Y1<0)
    {
       Y1=0;
       Y2=ScrY;
    }
    else
      Y2+=Y1-1;
  }
  // ��뢠�� DisplayObject()
  ScreenObject::Show();
}

/* $ 30.08.2000 SVS
  ���� ���墠� ������ �㭪樨 - �ࠢ����� ����������...
*/
void Dialog::Hide()
{
  ScreenObject::Hide();
  SkipDialogMode(DMODE_SHOW);
}
/* SVS $*/

//////////////////////////////////////////////////////////////////////////
/* Private, Virtual:
   ���樠������ ��ꥪ⮢ � �뢮� ������� �� �࠭.
*/
void Dialog::DisplayObject()
{
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  Shadow();              // "�������" ⥭�

  if (!CheckDialogMode(DMODE_INITOBJECTS))      // ᠬ�������� ��ਠ��, �����
  {                      //  ������ ���樠���������� �� ��ࢮ� �맮��.
    /* $ 28.07.2000 SVS
       ������ ��楤��, �� � ��� �� ��!
    */
    if(DlgProc((HANDLE)this,DN_INITDIALOG,InitDialogObjects(),DataDialog))
    {
      // �� ࠧ��, �.�. ����� ����� ���� ��������
      InitDialogObjects();
    }
    // �� ��ꥪ�� �ந��樠����஢���!
    SetDialogMode(DMODE_INITOBJECTS);
  }

  ShowDialog();          // "����㥬" ������.
}


//////////////////////////////////////////////////////////////////////////
/* Public:
   ���樠������ ����⮢ �������.
*/
/* $ 28.07.2000 SVS
   ������ InitDialogObjects �����頥� ID �����
   � 䮪�ᮬ �����
*/
/* $ 24.08.2000 SVS
  InitDialogObjects ����� ��ࠬ��� - ��� �롮�筮� २��樠����樨
  ����⮢. ID = -1 - ��ᠥ�� ��� ��ꥪ⮢
*/
int Dialog::InitDialogObjects(int ID)
{
  int I, J, TitleSet;
  int Length,StartX;
  int Type;
  struct DialogItem *CurItem;
  int InitItemCount;

  if(ID+1 > ItemCount)
    return -1;

  if(ID == -1) // ���樠�����㥬 ��?
  {
    ID=0;
    InitItemCount=ItemCount;
  }
  else
  {
    InitItemCount=ID+1;
  }


  // �।���⥫�� 横� �� ������ ������ � ��������� ���᮫�
  for(I=ID, TitleSet=0; I < InitItemCount; I++)
  {
    CurItem=&Item[I];

    // ��� ������ �� ����� �⨫� "�����뢠�� ��������� ������ ��� ᪮���"
    //  ������� �� ᠬ� ᪮���
    if (CurItem->Type==DI_BUTTON &&
        (CurItem->Flags & DIF_NOBRACKETS)==0 &&
        *CurItem->Data != '[')
    {
      char BracketedTitle[200];
      sprintf(BracketedTitle,"[ %s ]",CurItem->Data);
      strcpy(CurItem->Data,BracketedTitle);
    }

    // �� ��ࢮ�� �����襬��� "⥪���" ��⠭���� ��������� ���᮫�!
    if (!TitleSet &&             // �� �᫮���, �� �� �� ��⠭��������
         (CurItem->Type==DI_TEXT ||
          CurItem->Type==DI_DOUBLEBOX ||
          CurItem->Type==DI_SINGLEBOX))
      for (J=0;CurItem->Data[J]!=0;J++)
        if (LocalIsalpha(CurItem->Data[J]))
        {
          SetFarTitle(CurItem->Data+J);
          TitleSet=TRUE;
          break;
        }

     // �।���⥫�� ���� 䮪��
     if(FocusPos == -1 &&
        IsFocused(CurItem->Type) &&
        CurItem->Focus &&
        !(CurItem->Flags&(DIF_DISABLE|DIF_NOFOCUS)))
       FocusPos=I; // �������� ���� 䮪��� �����
     CurItem->Focus=0; // ��ᨬ ��� ���, �⮡� �� ���������,
                       //   �� 䮪�ᮢ - ��� � ���窠 䠭⨪��
  }

  // ����� �� 䮪�� ����� - ⥯���, �᫨ "�㤮" ���뫮 ���⠢���
  // ��� �� ����, � �⠢�� �� ���� ���室�騩
  if(FocusPos == -1)
  {
    for (I=0; I < ItemCount; I++) // �� �ᥬ!!!!
    {
      CurItem=&Item[I];
      if(IsFocused(CurItem->Type) && !(CurItem->Flags&(DIF_DISABLE|DIF_NOFOCUS)))
      {
        FocusPos=I;
        break;
      }
    }
  }
  if(FocusPos == -1) // �� �� �७� ᥡ� - ��� �� ������
  {                  //   ����� � ������������ 䮪��
     FocusPos=0;     // 㡨���, ����
  }

  // �� ��� � ���ࠫ��� ��!
  Item[FocusPos].Focus=1;

  // �᫨ �㤥� ।����, � ��易⥫쭮 �㤥� �뤥���.
  SelectOnEntry(FocusPos);

  // � ⥯��� �� ᭠砫� � �� ������ �ணࠬ��...
  for (I=ID; I < InitItemCount; I++)
  {
    CurItem=&Item[I];
    Type=CurItem->Type;

    // ��᫥����⥫쭮 ������� ������ � 䫠��� DIF_CENTERGROUP
    // � ���������� ���⨪��쭮� ����樥� ���� ��業��஢��� � �������.
    // �� ���न���� X �� �����. ������ �ᯮ�짮���� ��� 業��஢����
    // ��㯯 ������.
    if ((CurItem->Flags & DIF_CENTERGROUP) &&
        (I==0 ||
        (Item[I-1].Flags & DIF_CENTERGROUP)==0 ||
        Item[I-1].Y1!=CurItem->Y1))
    {
      Length=0;

      for (J=I; J < ItemCount &&
                (Item[J].Flags & DIF_CENTERGROUP) &&
                Item[J].Y1==Item[I].Y1; J++)
      {
        Length+=HiStrlen(Item[J].Data);

        if (Item[J].Type==DI_BUTTON && *Item[J].Data!=' ')
          Length+=2;
      }

      if (Item[I].Type==DI_BUTTON && *Item[I].Data!=' ')
        Length-=2;

      StartX=(X2-X1+1-Length)/2;

      if (StartX<0)
        StartX=0;

      for (J=I; J < ItemCount &&
                (Item[J].Flags & DIF_CENTERGROUP) &&
                Item[J].Y1==Item[I].Y1; J++)
      {
        Item[J].X1=StartX;
        StartX+=HiStrlen(Item[J].Data);

        if (Item[J].Type==DI_BUTTON && *Item[J].Data!=' ')
          StartX+=2;
      }
    }
    /* $ 01.08.2000 SVS
       ����� ListBox
    */
    if (Type==DI_LISTBOX)
    {
      if (!CheckDialogMode(DMODE_CREATEOBJECTS))
        CurItem->ObjPtr=new VMenu(NULL,NULL,0,CurItem->Y2-CurItem->Y1+1,
                               VMENU_ALWAYSSCROLLBAR|VMENU_LISTBOX,NULL/*,this*/);

      VMenu *ListBox=(VMenu *)CurItem->ObjPtr;

      if(ListBox)
      {
        // 㤠��� �� �⥬�
        ListBox->DeleteItems();

        struct MenuItem ListItem;
        /* $ 13.09.2000 SVS
           + ���� DIF_LISTNOAMPERSAND. �� 㬮�砭�� ��� DI_LISTBOX &
             DI_COMBOBOX ���⠢����� 䫠� MENU_SHOWAMPERSAND. ��� 䫠�
             �������� ⠪�� ���������
        */
        if(!(CurItem->Flags&DIF_LISTNOAMPERSAND))
          ListBox->SetFlags(MENU_SHOWAMPERSAND);
        /* SVS $*/
        ListBox->SetPosition(X1+CurItem->X1,Y1+CurItem->Y1,
                             X1+CurItem->X2,Y1+CurItem->Y2);
        ListBox->SetBoxType(SHORT_SINGLE_BOX);

        struct FarList *List=CurItem->ListItems;
        if(List && List->Items)
        {
          struct FarListItem *Items=List->Items;
          for (J=0; J < List->ItemsNumber; J++)
          {
            ListItem.Separator=Items[J].Flags&LIF_SEPARATOR;
            ListItem.Selected=Items[J].Flags&LIF_SELECTED;
            ListItem.Checked=Items[J].Flags&LIF_CHECKED;
            strcpy(ListItem.Name,Items[J].Text);
            strcpy(ListItem.UserData,Items[J].Text);
            ListItem.UserDataSize=strlen(Items[J].Text);

            ListBox->AddItem(&ListItem);
          }
        }
      }
    }
    /* SVS $*/
    // "।�����" - ࠧ����� �ᮡ�...
    if (IsEdit(Type))
    {
      if (!CheckDialogMode(DMODE_CREATEOBJECTS))
        CurItem->ObjPtr=new Edit;

      Edit *DialogEdit=(Edit *)CurItem->ObjPtr;
      /* $ 26.07.2000 SVS
         �� �������-� - ����������� ��।����㥬� ComboBox
      */
      if (CurItem->Flags & DIF_DROPDOWNLIST)
      {
         DialogEdit->DropDownBox=1;
      }
      /* SVS $ */
      /* $ 18.09.2000 SVS
         ReadOnly!
      */
      if (CurItem->Flags & DIF_READONLY)
      {
         DialogEdit->ReadOnly=1;
      }
      /* SVS $ */
      /* $ 15.10.2000 tran
        ��ப� ।����஢���� ������ ����� ���ᨬ� � 511 ᨬ����� */
      DialogEdit->SetMaxLength(511);
      /* tran $ */
      DialogEdit->SetPosition(X1+CurItem->X1,Y1+CurItem->Y1,
                              X1+CurItem->X2,Y1+CurItem->Y2);
      DialogEdit->SetObjectColor(
         FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGEDIT:COL_DIALOGEDIT),
         FarColorToReal(COL_DIALOGEDITSELECTED));
      if (CurItem->Type==DI_PSWEDIT)
      {
        DialogEdit->SetPasswordMode(TRUE);
        /* $ 01.08.2000 SVS
          ...�� �� ���뫮 �������... � ��� ����饭�� �����, �.�.
        */
        CurItem->Flags&=~DIF_HISTORY;
        /* SVS $ */
      }

      if (Type==DI_FIXEDIT)
      {
        /* $ 21.08.2000 SVS
           DIF_HISTORY ����� ����� ��᮪�� �ਮ���, 祬 DIF_MASKEDIT
        */
        if(CurItem->Flags&DIF_HISTORY)
          CurItem->Flags&=~DIF_MASKEDIT;
        /* SVS $ */
        // �᫨ DI_FIXEDIT, � ����� �ࠧ� �⠢���� �� ������...
        //   ��-�� - �뫮 �����㬥��஢���� :-)
        DialogEdit->SetMaxLength(CurItem->X2-CurItem->X1+1);
        DialogEdit->SetOvertypeMode(TRUE);
        /* $ 12.08.2000 KM
           �᫨ ⨯ ��ப� ����� DI_FIXEDIT � ��⠭����� 䫠� DIF_MASKEDIT
           � �����⮩ ��ࠬ��� CurItem->Mask, � ��뢠�� ����� �㭪��
           ��� ��⠭���� ��᪨ � ��ꥪ� Edit.
        */
        /* $ 18.09.2000 SVS
          ��᪠ �� ������ ���� ���⮩ (��ப� �� �஡���� �� ���뢠����)!
        */
        if ((CurItem->Flags & DIF_MASKEDIT) && CurItem->Mask)
        {
          char *Ptr=CurItem->Mask;
          while(*Ptr && *Ptr == ' ') ++Ptr;
          if(*Ptr)
            DialogEdit->SetInputMask(CurItem->Mask);
          else
          {
            CurItem->Mask=NULL;
            CurItem->Flags&=~DIF_MASKEDIT;
          }
        }
        /* SVS $ */
        /* KM $ */
      }
      else
        // "����-।����"
        // ��᫥����⥫쭮 ��।������ ���� ����� (edit controls),
        // ����騥 ��� 䫠� ��㯯������� � ।���� � ������������
        // ��⠢�� � 㤠����� ��ப
        if (!(CurItem->Flags & DIF_EDITOR))
        {
          DialogEdit->SetEditBeyondEnd(FALSE);
          DialogEdit->SetClearFlag(1);
        }

      /* $ 01.08.2000 SVS
         ��� �� �⮨� 䫠� DIF_USELASTHISTORY � ������� ��ப� �����,
         � ����⠭�������� ��ࢮ� ���祭�� �� History
      */
      if((CurItem->Flags&(DIF_HISTORY|DIF_USELASTHISTORY)) == (DIF_HISTORY|DIF_USELASTHISTORY) &&
         !CurItem->Data[0])
      {
        char RegKey[80],KeyValue[80];
        sprintf(RegKey,fmtSavedDialogHistory,(char*)CurItem->History);
        GetRegKey(RegKey,"Line0",CurItem->Data,"",sizeof(CurItem->Data));
      }
      /* SVS $ */

      /* $ 18.03.2000 SVS
         �᫨ �� ComBoBox � ����� �� ��⠭������, � ��६ �� ᯨ᪠
         �� �᫮���, �� ��� ���� �� �㭪⮢ ����� Selected != 0
      */
      if (Type==DI_COMBOBOX && CurItem->Data[0] == 0 && CurItem->ListItems)
      {
        struct FarListItem *ListItems=CurItem->ListItems->Items;
        int Length=CurItem->ListItems->ItemsNumber;

        for (J=0; J < Length; J++)
        {
          if(ListItems[J].Flags & LIF_SELECTED)
          {
            // ��६ ⮫쪮 ���� �㭪� ��� ������ ।���஢����
            strcpy(CurItem->Data, ListItems[J].Text);
            break;
          }
        }
      }
      /* SVS $ */
      DialogEdit->SetString(CurItem->Data);

      if (Type==DI_FIXEDIT)
        DialogEdit->SetCurPos(0);

      DialogEdit->FastShow();
    }
  }

  // �� ��ꥪ�� ᮧ����!
  SetDialogMode(DMODE_CREATEOBJECTS);
  return I;
}
/* 24.08.2000 SVS $ */


//////////////////////////////////////////////////////////////////////////
/* Private:
   ����祭�� ������ � 㤠����� "।���஢"
*/
void Dialog::DeleteDialogObjects()
{
  int I;

  for (I=0; I < ItemCount; I++)
    if (IsEdit(Item[I].Type))
    {
      ((Edit *)(Item[I].ObjPtr))->GetString(Item[I].Data,sizeof(Item[I].Data));
      /*$ 05.07.2000 SVS $
          �஢�ઠ - ��� ����� �।�������� ���७�� ��६����� �।�?
      */
      if(Item[I].Flags&DIF_EDITEXPAND)
         ExpandEnvironmentStr(Item[I].Data, Item[I].Data,sizeof(Item[I].Data));
      /* SVS */
      delete (Edit *)(Item[I].ObjPtr);
    }
    else if(Item[I].Type == DI_LISTBOX && Item[I].ObjPtr)
      delete (VMenu *)(Item[I].ObjPtr);
}


//////////////////////////////////////////////////////////////////////////
/* Public:
   ���࠭�� ���祭�� �� ����� ।���஢����.
   �� ��⠭�������� 䫠�� DIF_HISTORY, ��࠭�� ����� � ॥���.
*/
void Dialog::GetDialogObjectsData()
{
  int I;
  for (I=0; I < ItemCount; I++)
    if (IsEdit(Item[I].Type))
    {
      ((Edit *)(Item[I].ObjPtr))->GetString(Item[I].Data,sizeof(Item[I].Data));
      if (ExitCode>=0 && (Item[I].Flags & DIF_HISTORY) && Item[I].History && Opt.DialogsEditHistory)
        AddToEditHistory(Item[I].Data,Item[I].History);
      /* $ 01.08.2000 SVS
         ! � History ������ �������� ���祭�� (��� DIF_EXPAND...) ��।
          ���७��� �।�!
      */
      /*$ 05.07.2000 SVS $
      �஢�ઠ - ��� ����� �।�������� ���७�� ��६����� �।�?
      �.�. �㭪�� GetDialogObjectsData() ����� ��뢠���� ᠬ����⥫쭮
      �� ���� �஢����!*/
      if(Item[I].Flags&DIF_EDITEXPAND)
         ExpandEnvironmentStr(Item[I].Data, Item[I].Data,sizeof(Item[I].Data));
      /* SVS $ */
      /* 01.08.2000 SVS $ */
    }
}


//////////////////////////////////////////////////////////////////////////
/* $ 22.08.2000 SVS
  ! ShowDialog - �������⥫�� ��ࠬ��� - ����� ����� ���ᮢ뢠��
*/
/* Private:
   ���ᮢ�� ����⮢ ������� �� �࠭�.
*/
void Dialog::ShowDialog(int ID)
{
  struct DialogItem *CurItem;
  int X,Y;
  int I,DrawItemCount;
  unsigned long Attr;

  /* $ 18.08.2000 SVS
     �᫨ �� ࠧ�襭� ���ᮢ��, � �뢠������.
  */
  if(IsEnableRedraw ||                 // ࠧ�襭� ���ᮢ�� ?
     (ID+1 > ItemCount) ||             // � ����� � ࠬ��� ������������?
     CheckDialogMode(DMODE_DRAWING))   // ������ ������?
    return;
  /* SVS $ */

  SetDialogMode(DMODE_DRAWING);  // ������ ������!!!

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  if(ID == -1) // ��㥬 ��?
  {
    /* $ 28.07.2000 SVS
       ��। ���ᮢ��� ������� ���뫠�� ᮮ�饭�� � ��ࠡ��稪
    */
      DlgProc((HANDLE)this,DN_DRAWDIALOG,0,0);
    /* SVS $ */

    /* $ 28.07.2000 SVS
       ��। ���ᮢ��� �������� ���� �������...
    */
    Attr=DlgProc((HANDLE)this,DN_CTLCOLORDIALOG,0,CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT);
    SetScreen(X1,Y1,X2,Y2,' ',Attr);
    /* SVS $ */
  }

  if(ID == -1) // ��㥬 ��?
  {
    ID=0;
    DrawItemCount=ItemCount;
  }
  else
  {
    DrawItemCount=ID+1;
  }

  for (I=ID; I < DrawItemCount; I++)
  {
    CurItem=&Item[I];

    /* $ 28.07.2000 SVS
       ��। ���ᮢ��� ������� ����� ���뫠�� ᮮ�饭��
       ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
    */
    Dialog::SendDlgMessage((HANDLE)this,DN_DRAWDLGITEM,I,0);
    /* SVS $ */
    /* $ 28.07.2000 SVS
       ��। ���ᮢ��� ������� ����� ������� ���᭨� ������ ���ᮢ��
    */
    switch(CurItem->Type)
    {
/* ***************************************************************** */
      case DI_USERCONTROL:
        if(CurItem->VBuf)
        {
          PutText(X1+CurItem->X1,Y1+CurItem->Y1,X1+CurItem->X2,Y1+CurItem->Y2,CurItem->VBuf);
        }
        break; //㦥 ����ᮢ��� :-)))

/* ***************************************************************** */
      case DI_SINGLEBOX:
      case DI_DOUBLEBOX:
      {
        Attr=MAKELONG(
          MAKEWORD(FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGBOXTITLE:COL_DIALOGBOXTITLE), // Title LOBYTE
                 FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGHIGHLIGHTTEXT:COL_DIALOGHIGHLIGHTTEXT)),// HiText HIBYTE
          MAKEWORD(FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGBOX:COL_DIALOGBOX), // Box LOBYTE
                 0)                                               // HIBYTE
        );
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);

        Box(X1+CurItem->X1,Y1+CurItem->Y1,X1+CurItem->X2,Y1+CurItem->Y2,
            LOBYTE(HIWORD(Attr)),
            (CurItem->Type==DI_SINGLEBOX) ? SINGLE_BOX:DOUBLE_BOX);

        if (*CurItem->Data)
        {
          char Title[200];
          int XB;

          sprintf(Title," %s ",CurItem->Data);
          XB=X1+CurItem->X1+(CurItem->X2-CurItem->X1+1-HiStrlen(Title))/2;

          if (CurItem->Flags & DIF_LEFTTEXT && X1+CurItem->X1+1 < XB)
            XB=X1+CurItem->X1+1;

          SetColor(Attr&0xFF);
          GotoXY(XB,Y1+CurItem->Y1);
          HiText(Title,HIBYTE(LOWORD(Attr)));
        }
        break;
      }

/* ***************************************************************** */
      case DI_TEXT:
      {
        if (CurItem->X1==(unsigned char)-1)
          X=(X2-X1+1-HiStrlen(CurItem->Data))/2;
        else
          X=CurItem->X1;

        if (CurItem->Y1==(unsigned char)-1)
          Y=(Y2-Y1+1)/2;
        else
          Y=CurItem->Y1;


        if (CurItem->Flags & DIF_SETCOLOR)
          Attr=CurItem->Flags & DIF_COLORMASK;
        else
          if (CurItem->Flags & DIF_BOXCOLOR)
            Attr=CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGBOX:COL_DIALOGBOX;
          else
            Attr=CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT;

        Attr=MAKELONG(
           MAKEWORD(FarColorToReal(Attr),
                   FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGHIGHLIGHTTEXT:COL_DIALOGHIGHLIGHTTEXT)), // HIBYTE HiText
             0);
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);
        SetColor(Attr&0xFF);

        if (CurItem->Flags & DIF_SEPARATOR)
        {
          GotoXY(X1+3,Y1+Y);
          if (DialogTooLong)
            ShowSeparator(DialogTooLong-5);
          else
            ShowSeparator(X2-X1-5);
        }

        GotoXY(X1+X,Y1+Y);

        if (CurItem->Flags & DIF_SHOWAMPERSAND)
          Text(CurItem->Data);
        else
          HiText(CurItem->Data,HIBYTE(LOWORD(Attr)));

        break;
      }

/* ***************************************************************** */
      case DI_VTEXT:
      {
        if (CurItem->Flags & DIF_BOXCOLOR)
          Attr=CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGBOX:COL_DIALOGBOX;
        else
          if (CurItem->Flags & DIF_SETCOLOR)
            Attr=(CurItem->Flags & DIF_COLORMASK);
          else
            Attr=(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT);

        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,FarColorToReal(Attr));
        SetColor(Attr&0xFF);
        GotoXY(X1+CurItem->X1,Y1+CurItem->Y1);
        VText(CurItem->Data);
        break;
      }

/* ***************************************************************** */
      /* $ 18.07.2000 SVS
         + ��ࠡ�⪠ ����� DI_COMBOBOX
      */
      case DI_EDIT:
      case DI_FIXEDIT:
      case DI_PSWEDIT:
      case DI_COMBOBOX:
      {
        Edit *EditPtr=(Edit *)(CurItem->ObjPtr);

        /* $ 15.08.2000 SVS
           ! ��� DropDownList 梥� ��ࠡ��뢠�� �� �����
        */
        /* $ 30.08.2000 SVS
           ! "����, ����� ��, ��� �� ���ࠢ�����" :-)
        */
        Attr=EditPtr->GetObjectColor();
        if(CurItem->Type == DI_COMBOBOX && (CurItem->Flags & DIF_DROPDOWNLIST))
        {
          DWORD AAA=Attr&0xFF;
          Attr=MAKEWORD(FarColorToReal(AAA),
                 FarColorToReal((!CurItem->Focus)?COL_DIALOGEDIT:COL_DIALOGEDITSELECTED));
          Attr=MAKELONG(Attr, // EditLine (Lo=Color, Hi=Selected)
            MAKEWORD(FarColorToReal(AAA), // EditLine - UnChanched Color
            FarColorToReal(COL_DIALOGTEXT) // HistoryLetter
           ));
        }
        else
        {
          Attr=MAKEWORD(FarColorToReal(Attr&0xFF),FarColorToReal(COL_DIALOGEDITSELECTED));
          Attr=MAKELONG(Attr, // EditLine (Lo=Color, Hi=Selected)
             MAKEWORD(FarColorToReal(EditPtr->GetObjectColorUnChanged()), // EditLine - UnChanched Color
             FarColorToReal(COL_DIALOGTEXT) // HistoryLetter
             ));
        }
        /* SVS $ */
        /* SVS $ */
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);

        EditPtr->SetObjectColor(Attr&0xFF,HIBYTE(LOWORD(Attr)),LOBYTE(HIWORD(Attr)));

        if (CurItem->Focus)
        {
          /* $ 09.08.2000 KM
             �⪫�祭�� �����饣� ����� �� ��६�饭�� �������
          */
          if (!CheckDialogMode(DMODE_DRAGGED))
            SetCursorType(1,-1);
          EditPtr->Show();
          /* KM $ */
          SelectOnEntry(I);
        }
        else
          EditPtr->FastShow();

        /* $ 09.08.2000 KM
           �⪫�祭�� �����饣� ����� �� ��६�饭�� �������
        */
        if (CheckDialogMode(DMODE_DRAGGED))
          SetCursorType(FALSE,0);
        /* KM $ */

        if (CurItem->History &&
             ((CurItem->Flags & DIF_HISTORY) &&
              Opt.DialogsEditHistory
              || CurItem->Type == DI_COMBOBOX))
        {
          int EditX1,EditY1,EditX2,EditY2;

          EditPtr->GetPosition(EditX1,EditY1,EditX2,EditY2);
          //Text((CurItem->Type == DI_COMBOBOX?"\x1F":"\x19"));
          Text(EditX2+1,EditY1,HIBYTE(HIWORD(Attr)),"");
        }
        break;
        /* SVS $ */
      }

/* ***************************************************************** */
      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      case DI_LISTBOX:
      {
        VMenu *ListBox=(VMenu *)(CurItem->ObjPtr);
        if(ListBox)
        {
          /* $ 21.08.2000 SVS
             ��। ���ᮢ��� ��ᨬ �� ��������� 梥⮢�� ��ਡ�⮢
          */
          short Colors[9];
          ListBox->GetColors(Colors);
          if(DlgProc((HANDLE)this,DN_CTLCOLORDLGLIST,
                          sizeof(Colors)/sizeof(Colors[0]),(long)Colors))
            ListBox->SetColors(Colors);
          /* SVS $ */
          if (CurItem->Focus)
            ListBox->Show();
          else
            ListBox->FastShow();
        }
        break;
      }
      /* 01.08.2000 SVS $ */

/* ***************************************************************** */
      case DI_CHECKBOX:
      case DI_RADIOBUTTON:
      {
        if (CurItem->Flags & DIF_SETCOLOR)
          Attr=(CurItem->Flags & DIF_COLORMASK);
        else
          Attr=(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT);

        Attr=MAKEWORD(FarColorToReal(Attr),
             FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGHIGHLIGHTTEXT:COL_DIALOGHIGHLIGHTTEXT)); // HiText
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);

        SetColor(Attr&0xFF);

        GotoXY(X1+CurItem->X1,Y1+CurItem->Y1);

        if (CurItem->Type==DI_CHECKBOX)
          mprintf("[%c] ",CurItem->Selected ? 'x':' ');
        else
          if (CurItem->Flags & DIF_MOVESELECT)
            mprintf(" %c ",CurItem->Selected ? '\07':' ');
          else
            mprintf("(%c) ",CurItem->Selected ? '\07':' ');

        HiText(CurItem->Data,HIBYTE(LOWORD(Attr)));

        if (CurItem->Focus)
        {
          /* $ 09.08.2000 KM
             �⪫�祭�� �����饣� ����� �� ��६�饭�� �������
          */
          if (!CheckDialogMode(DMODE_DRAGGED))
            SetCursorType(1,-1);
          MoveCursor(X1+CurItem->X1+1,Y1+CurItem->Y1);
          /* KM $ */
        }

        break;
      }

/* ***************************************************************** */
      case DI_BUTTON:
      {
        GotoXY(X1+CurItem->X1,Y1+CurItem->Y1);

        /* $ 18.08.2000 SVS
           + DI_BUTTON ⮦� ⥯��� ����� ����� DIF_SETCOLOR
        */
        if (CurItem->Focus)
        {
          SetCursorType(0,10);
          Attr=MAKEWORD(
             (CurItem->Flags & DIF_SETCOLOR)?(CurItem->Flags & DIF_COLORMASK):
               FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGSELECTEDBUTTON:COL_DIALOGSELECTEDBUTTON), // TEXT
             FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGHIGHLIGHTSELECTEDBUTTON:COL_DIALOGHIGHLIGHTSELECTEDBUTTON)); // HiText
        }
        else
        {
          Attr=MAKEWORD(
             (CurItem->Flags & DIF_SETCOLOR)?(CurItem->Flags & DIF_COLORMASK):
               FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGBUTTON:COL_DIALOGBUTTON), // TEXT
             FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGHIGHLIGHTBUTTON:COL_DIALOGHIGHLIGHTBUTTON)); // HiText
        }
        /* SVS $ */
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);
        SetColor(Attr&0xFF);
        HiText(CurItem->Data,HIBYTE(LOWORD(Attr)));
        break;
      }

/* ***************************************************************** */
    } // end switch(...
    /* 28.07.2000 SVS $ */
  } // end for (I=...

  /* $ 31.07.2000 SVS
     ����稬 �������� ��६�饭��...
  */
  if ( CheckDialogMode(DMODE_DRAGGED) ) // �᫨ ������ �᪠����
  {
    /* $ 03.08.2000 tran
       �뢮� ⥪�� � 㣫� ����� �ਢ����� � �訡��� ����ࠦ����
       1) ����� ������ ��६�頥��� � 㣮�
       2) ����� ������ ��६�頥��� �� 㣫�
       ᤥ��� �뢮� ����� ����祪 �� 㣫�� */
    //Text(0,0,0xCE,"Move");
    Text(X1,Y1,0xCE,"\\");
    Text(X1,Y2,0xCE,"/");
    Text(X2,Y1,0xCE,"/");
    Text(X2,Y2,0xCE,"\\");
  }
  /* SVS $ */


  SkipDialogMode(DMODE_DRAWING);  // ����� ���ᮢ�� �������!!!
  SetDialogMode(DMODE_SHOW); // ������ �� �࠭�!
}
/* SVS 22.08.2000 $ */


//////////////////////////////////////////////////////////////////////////
/* Public, Virtual:
   ��ࠡ�⪠ ������ �� ����������.
   ��४�뢠�� BaseInput::ProcessKey.
*/
int Dialog::ProcessKey(int Key)
{
  int I,J;
  char Str[1024];
  char *PtrStr;
  Edit *CurEditLine;

  /* $ 31.07.2000 tran
     + ��६�饭�� ������� �� �࠭� */
  if (CheckDialogMode(DMODE_DRAGGED)) // �᫨ ������ �᪠����
  {
    int rr=1;
    switch (Key)
    {
        case KEY_CTRLLEFT:
            rr=10;
        case KEY_LEFT:
            Hide();
            for ( I=0; I<rr; I++ )
                if ( X1>0 )
                {
                    X1--;
                    X2--;
                    AdjustEditPos(-1,0);
                }
            if(!CheckDialogMode(DMODE_ALTDRAGGED)) Show();
            break;
        case KEY_CTRLRIGHT:
            rr=10;
        case KEY_RIGHT:
            Hide();
            for ( I=0; I<rr; I++ )
                if ( X2<ScrX )
                {
                    X1++;
                    X2++;
                    AdjustEditPos(1,0);
                }
            if(!CheckDialogMode(DMODE_ALTDRAGGED)) Show();
            break;
        case KEY_PGUP:
        case KEY_CTRLUP:
            rr=5;
        case KEY_UP:
            Hide();
            for ( I=0; I<rr; I++ )
                if ( Y1>0 )
                {
                    Y1--;
                    Y2--;
                    AdjustEditPos(0,-1);
                }
            if(!CheckDialogMode(DMODE_ALTDRAGGED)) Show();
            break;
        case KEY_PGDN:
        case KEY_CTRLDOWN:
            rr=5;
        case KEY_DOWN:
            Hide();
            for ( I=0; I<rr; I++ )
                if ( Y2<ScrY )
                {
                    Y1++;
                    Y2++;
                    AdjustEditPos(0,1);
                }
            if(!CheckDialogMode(DMODE_ALTDRAGGED)) Show();
            break;
        case KEY_ENTER:
        case KEY_CTRLF5:
            SkipDialogMode(DMODE_DRAGGED); // �����稬 ��������!
            if(!CheckDialogMode(DMODE_ALTDRAGGED)) Show();
            break;
        case KEY_ESC:
            Hide();
            AdjustEditPos(OldX1-X1,OldY1-Y1);
            X1=OldX1;
            X2=OldX2;
            Y1=OldY1;
            Y2=OldY2;
            SkipDialogMode(DMODE_DRAGGED);
            if(!CheckDialogMode(DMODE_ALTDRAGGED)) Show();
            break;
    }
    if(CheckDialogMode(DMODE_ALTDRAGGED))
    {
      SkipDialogMode(DMODE_DRAGGED|DMODE_ALTDRAGGED);
      Show();
    }
    return (TRUE);
  }
  /* $ 10.08.2000 SVS
     �������, �᫨ ࠧ�襭�! (IsCanMove)
  */
  if (Key == KEY_CTRLF5 && CheckDialogMode(DMODE_ISCANMOVE))
  /* SVS 10.08.2000 $*/
  {
    // ����砥� 䫠� � ���������� ���न����
    SetDialogMode(DMODE_DRAGGED);
    OldX1=X1; OldX2=X2; OldY1=Y1; OldY2=Y2;
    //# GetText(0,0,3,0,LV);
    Show();
    return (TRUE);
  }
  /* tran 31.07.2000 $ */

  if (Key==KEY_NONE || Key==KEY_IDLE)
  {
    /* $ 28.07.2000 SVS
       ��।���� ��� 䠪� � ��ࠡ��稪 :-)
    */
    DlgProc((HANDLE)this,DN_ENTERIDLE,0,0);
    /* SVS $ */
    return(FALSE);
  }

  // "���� ������ �� �, �� ��� ��������..."
  if(Key == KEY_CTRLALTSHIFTPRESS && CheckDialogMode(DMODE_SHOW))
  {
    if(Opt.AllCtrlAltShiftRule & CASR_DIALOG)
    {
      Hide();
      WaitKey(KEY_CTRLALTSHIFTRELEASE);
      Show();
    }
    return(TRUE);
  }

  int Type=Item[FocusPos].Type;

  if(!CheckDialogMode(DMODE_KEY))
    DlgProc((HANDLE)this,DM_KEY,FocusPos,Key);

  // �������� ��⨬�����
  if(Type==DI_CHECKBOX)
  {
    if((Key == KEY_ADD      && !Item[FocusPos].Selected) ||
       (Key == KEY_SUBTRACT &&  Item[FocusPos].Selected))
    Key=KEY_SPACE;
  }
  else if(Key == KEY_ADD)
    Key='+';
  else if(Key == KEY_SUBTRACT)
    Key='-';

  switch(Key)
  {
    case KEY_F1:
      /* $ 28.07.2000 SVS
         ��। �뢮��� ������� ���뫠�� ᮮ�饭�� � ��ࠡ��稪
         � �᫨ ���㫨 �� ����, � �뢮��� ���᪠���
      */
      PtrStr=(char*)DlgProc((HANDLE)this,DN_HELP,FocusPos,(long)&HelpTopic[0]);
      if(PtrStr && *PtrStr)
      {
        /* $ 31.08.2000 SVS
           - ���� � �맮��� 䠩��� �����.
        */
        if(PluginNumber != -1)
        {
          /* $ 29.08.2000 SVS
             ! �� ������� ⥬� ����� �� �������� ��楤���...
               ����, �㦭� ����� �ନ஢��� ���⥭�!
          */
          if (*PtrStr==':')       // Main Topic?
            strcpy(Str,PtrStr+1);
          else if (*PtrStr=='#')  // 㦥 ��ନ஢���?
            strcpy(Str,PtrStr);
          else                    // ���� �ନ஢���...
          {
            strcpy(&Str[512],CtrlObject->Plugins.PluginsData[PluginNumber].ModuleName);
            *PointToName(&Str[512])=0;
            sprintf(Str,"#%s#%s",&Str[512],PtrStr);
          }
          /* SVS $ */
        }
        else
          strcpy(Str,PtrStr);

        SetHelp(Str);
        /* SVS $ */
        ShowHelp();
      }
      /* SVS $ */
      return(TRUE);

    case KEY_TAB:
    case KEY_SHIFTTAB:
// ����� � 䮪�ᮬ ������� �������!!!
      if (Item[FocusPos].Flags & DIF_EDITOR)
      {
        I=FocusPos;
        while (Item[I].Flags & DIF_EDITOR)
          I=ChangeFocus(I,(Key==KEY_TAB) ? 1:-1,TRUE);
      }
      else
      {
        I=ChangeFocus(FocusPos,(Key==KEY_TAB) ? 1:-1,TRUE);
        if (Key==KEY_SHIFTTAB)
          while (I>0 && (Item[I].Flags & DIF_EDITOR)!=0 &&
                 (Item[I-1].Flags & DIF_EDITOR)!=0 &&
                 ((Edit *)Item[I].ObjPtr)->GetLength()==0)
            I--;
      }
      ChangeFocus2(FocusPos,I);
      ShowDialog();
      return(TRUE);

    case KEY_CTRLENTER:
      EndLoop=TRUE;
      for (I=0;I<ItemCount;I++)
        if (Item[I].DefaultButton)
        {
          if (!IsEdit(Item[I].Type))
            Item[I].Selected=1;
          ExitCode=I;
          return(TRUE);
        }
      if(!CheckDialogMode(DMODE_OLDSTYLE))
      {
        EndLoop=FALSE; // ⮫쪮 �᫨ ����
        return TRUE; // ������ ����� �� 祣�
      }

    case KEY_ENTER:
      if (Item[FocusPos].Flags & DIF_EDITOR)
      {
        int EditorLastPos;
        for (EditorLastPos=I=FocusPos;I<ItemCount;I++)
          if (IsEdit(Item[I].Type) && (Item[I].Flags & DIF_EDITOR))
            EditorLastPos=I;
          else
            break;
        if (((Edit *)(Item[EditorLastPos].ObjPtr))->GetLength()!=0)
          return(TRUE);
        for (I=EditorLastPos;I>FocusPos;I--)
        {
          int CurPos;
          if (I==FocusPos+1)
            CurPos=((Edit *)(Item[I-1].ObjPtr))->GetCurPos();
          else
            CurPos=0;
          ((Edit *)(Item[I-1].ObjPtr))->GetString(Str,sizeof(Str));
          int Length=strlen(Str);
          ((Edit *)(Item[I].ObjPtr))->SetString(CurPos>=Length ? "":Str+CurPos);
          if (CurPos<Length)
            Str[CurPos]=0;
          ((Edit *)(Item[I].ObjPtr))->SetCurPos(0);
          ((Edit *)(Item[I-1].ObjPtr))->SetString(Str);
          /* $ 28.07.2000 SVS
            �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
            ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
          */
          Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,I-1,0);
          Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,I,0);
          /* SVS $ */
        }
        if (EditorLastPos>FocusPos)
        {
          ((Edit *)(Item[FocusPos].ObjPtr))->SetCurPos(0);
          ProcessKey(KEY_DOWN);
        }
        else
          ShowDialog();
        return(TRUE);
      }
      else if (Type==DI_BUTTON)
      {
        /* $ 21.08.2000 SVS
           ��� �ࠡ��뢠���, �᫨ ����� �� ������
        */
        Item[FocusPos].Selected=1;
        // ᮮ�饭�� - "������ �������"
        Dialog::SendDlgMessage((HANDLE)this,DN_BTNCLICK,FocusPos,0);
        /* $ 18.08.2000 SVS
           + ���� ��� DI_BUTTON - DIF_BTNNOCLOSE - "������ �� ��� ������� �������"
        */
        if((Item[FocusPos].Flags&DIF_BTNNOCLOSE))
        {
//          ShowDialog(); //???
          return(TRUE);
        }
        /* SVS $ */
        /* SVS 21.08.2000 $ */
        /* $ 12.09.2000 SVS
          ! ������ ��������� ��� ������ � DefaultButton=1
        */
        if(CheckDialogMode(DMODE_OLDSTYLE) || Item[FocusPos].DefaultButton)
        {
          ExitCode=FocusPos;
          EndLoop=TRUE;
        }
        /* SVS $ */
      }
      else if(IsEdit(Type) || CheckDialogMode(DMODE_OLDSTYLE))
      {
        for (I=0;I<ItemCount;I++)
          if (Item[I].DefaultButton)
          {
            if (!IsEdit(Item[I].Type))
              Item[I].Selected=1;
            ExitCode=I;
          }

        EndLoop=TRUE;
        if (ExitCode==-1)
          ExitCode=FocusPos;
      }
      return(TRUE);

    case KEY_ESC:
    case KEY_BREAK:
    case KEY_F10:
      EndLoop=TRUE;
      ExitCode=(Key==KEY_BREAK) ? -2:-1;
      return(TRUE);
/*
    case KEY_ADD:
      if (Type==DI_CHECKBOX && !Item[FocusPos].Selected)
        ProcessKey(KEY_SPACE);
      else
        ProcessKey('+');
      return(TRUE);

    case KEY_SUBTRACT:
      if (Type==DI_CHECKBOX && Item[FocusPos].Selected)
        ProcessKey(KEY_SPACE);
      else
        ProcessKey('-');
      return(TRUE);
*/
    case KEY_SPACE:
      if (Type==DI_BUTTON)
        return(ProcessKey(KEY_ENTER));
      if (Type==DI_CHECKBOX)
      {
        Item[FocusPos].Selected =! Item[FocusPos].Selected;
        /* $ 28.07.2000 SVS
          �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
           ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
        */
        if(!Dialog::SendDlgMessage((HANDLE)this,DN_BTNCLICK,FocusPos,Item[FocusPos].Selected))
          Item[FocusPos].Selected =! Item[FocusPos].Selected;
        /* SVS $ */
        ShowDialog();
        return(TRUE);
      }
      if (Type==DI_RADIOBUTTON)
      {
        int PrevRB;
        for (I=FocusPos;;I--)
          if (Item[I].Type==DI_RADIOBUTTON && (Item[I].Flags & DIF_GROUP) ||
              I==0 || Item[I-1].Type!=DI_RADIOBUTTON)
            break;
        do
        {
          /* $ 28.07.2000 SVS
            �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
            ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
          */
          J=Item[I].Selected;
          Item[I].Selected=0;
          if(J)
          {
            PrevRB=I;
          }
          ++I;
          /* SVS $ */
        } while (I<ItemCount && Item[I].Type==DI_RADIOBUTTON &&
                 (Item[I].Flags & DIF_GROUP)==0);

        Item[FocusPos].Selected=1;
        /* $ 28.07.2000 SVS
          �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
          ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
        */
        if(!Dialog::SendDlgMessage((HANDLE)this,DN_BTNCLICK,FocusPos,PrevRB))
        {
           // ��୥� �����, �᫨ ���짮��⥫� �� ���⥫...
           Item[PrevRB].Selected=1;
           Item[FocusPos].Selected=0;
        }
        /* SVS $ */
        ShowDialog();
        return(TRUE);
      }
      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        /* $ 28.07.2000 SVS
          �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
          ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
        */
        Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
        /* SVS $ */
        return(TRUE);
      }
      return(TRUE);

    case KEY_HOME:
      // ��� user-⨯� �뢠������
      if(Type == DI_USERCONTROL)
        return TRUE;

      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }

      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      if(Type == DI_LISTBOX)
      {
        ((VMenu *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* SVS $ */

      for (I=0;I<ItemCount;I++)
        if (IsFocused(Item[I].Type))
        {
          ChangeFocus2(FocusPos,I);
          /* $ 28.07.2000 SVS
            �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
            ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
          */
          //Dialog::SendDlgMessage((HANDLE)this,DN_CHANGEITEM,FocusPos,0);
          //Dialog::SendDlgMessage((HANDLE)this,DN_CHANGEITEM,I,0);
          /* SVS $ */
          ShowDialog();
          return(TRUE);
        }
      return(TRUE);

    case KEY_LEFT:
    case KEY_RIGHT:
      // ��� user-⨯� �뢠������
      if(Type == DI_USERCONTROL)
        return TRUE;

      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      if(Type == DI_LISTBOX)
      {
        ((VMenu *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* SVS $ */
      {
        int MinDist=1000,MinPos;
        for (I=0;I<ItemCount;I++)
          if (I!=FocusPos && (IsEdit(Item[I].Type) || Item[I].Type==DI_CHECKBOX ||
              Item[I].Type==DI_RADIOBUTTON) && Item[I].Y1==Item[FocusPos].Y1)
          {
            int Dist=Item[I].X1-Item[FocusPos].X1;
            if (Key==KEY_LEFT && Dist<0 || Key==KEY_RIGHT && Dist>0)
              if (abs(Dist)<MinDist)
              {
                MinDist=abs(Dist);
                MinPos=I;
              }
          }
          if (MinDist<1000)
          {
            ChangeFocus2(FocusPos,MinPos);
            if (Item[MinPos].Flags & DIF_MOVESELECT)
              ProcessKey(KEY_SPACE);
            else
              ShowDialog();
            return(TRUE);
          }
      }

    case KEY_UP:
    case KEY_DOWN:
      // ��� user-⨯� �뢠������
      if(Type == DI_USERCONTROL)
        return TRUE;

      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      if(Type == DI_LISTBOX)
      {
        ((VMenu *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* SVS $ */
      {
        int PrevPos=0;
        if (Item[FocusPos].Flags & DIF_EDITOR)
          PrevPos=((Edit *)(Item[FocusPos].ObjPtr))->GetCurPos();
        I=ChangeFocus(FocusPos,(Key==KEY_LEFT || Key==KEY_UP) ? -1:1,FALSE);
        Item[FocusPos].Focus=0;
        Item[I].Focus=1;
        ChangeFocus2(FocusPos,I);
        if (Item[I].Flags & DIF_EDITOR)
          ((Edit *)(Item[I].ObjPtr))->SetCurPos(PrevPos);
        if (Item[I].Flags & DIF_MOVESELECT)
          ProcessKey(KEY_SPACE);
        else
          ShowDialog();
      }
      return(TRUE);

    case KEY_END:
      // ��� user-⨯� �뢠������
      if(Type == DI_USERCONTROL)
        return TRUE;

      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
    case KEY_PGDN:
      // ��� user-⨯� �뢠������
      if(Type == DI_USERCONTROL)
        return TRUE;

      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      if(Type == DI_LISTBOX)
      {
        ((VMenu *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* SVS $ */
      else if (!(Item[FocusPos].Flags & DIF_EDITOR))
      {
        for (I=0;I<ItemCount;I++)
          if (Item[I].DefaultButton)
          {
            ChangeFocus2(FocusPos,I);
            ShowDialog();
            return(TRUE);
          }
      }
      else
      {
        ProcessKey(KEY_TAB);
        ProcessKey(KEY_UP);
      }
      return(TRUE);

    case KEY_CTRLUP:
    case KEY_CTRLDOWN:
      // ��� user-⨯� �뢠������
      if(Type == DI_USERCONTROL)
        return TRUE;

      CurEditLine=((Edit *)(Item[FocusPos].ObjPtr));
      if (IsEdit(Type) &&
           (Item[FocusPos].Flags & DIF_HISTORY) &&
           Opt.DialogsEditHistory &&
           Item[FocusPos].History)
      /* $ 26.07.2000 SVS
         ��।��� �, �� � ��ப� ����� � �㭪�� �롮� �� ���ਨ
         ��� �뤥����� �㦭��� �㭪� � ���ਨ.
      */
      {
        CurEditLine->GetString(Str,sizeof(Str));
        SelectFromEditHistory(CurEditLine,Item[FocusPos].History,Str);
      }
      /* SVS $ */
      /* $ 18.07.2000 SVS
         + ��ࠡ�⪠ DI_COMBOBOX - �롮� �� ᯨ᪠!
      */
      else if(Type == DI_COMBOBOX && Item[FocusPos].ListItems)
      {
        CurEditLine->GetString(Str,sizeof(Str));
        SelectFromComboBox(CurEditLine,
                      Item[FocusPos].ListItems,Str);
      }
      /* SVS $ */
      return(TRUE);

    default:
      // ��� user-⨯� �뢠������
      if(Type == DI_USERCONTROL)
        return TRUE;

      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      if(Type == DI_LISTBOX)
      {
        ((VMenu *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* SVS $ */

      /* $ 21.08.2000 SVS
         Autocomplete �� ����ﭭ�� ������ � ������� ��⨬���樨 ;-)
      */
      if (IsEdit(Type))
      {
        Edit *edt=(Edit *)Item[FocusPos].ObjPtr;
        int SelStart, SelEnd;

        /* $ 11.09.2000 SVS
           Ctrl-U � ��ப�� ����� ᭨���� ������ �����
        */
        if(Key == KEY_CTRLU)
        {
          edt->SetClearFlag(0);
          edt->Select(-1,0);
          edt->Show();
          return TRUE;
        }
        /* SVS $ */

        if (Item[FocusPos].Flags & DIF_EDITOR)
          switch(Key)
          {
            /* $ 12.09.2000 SVS
              ��ࠢ�塞 ����� � BackSpace � DIF_EDITOR
            */
            case KEY_BS:
            {
              int CurPos=edt->GetCurPos();
              if(!edt->GetCurPos())
              {
                if(FocusPos > 0 &&
                   (Item[FocusPos-1].Flags&DIF_EDITOR))
                {
                  Edit *edt_1=(Edit *)Item[FocusPos-1].ObjPtr;
                  edt_1->GetString(Str,sizeof(Str));
                  CurPos=strlen(Str);
                  edt->GetString(Str+CurPos,sizeof(Str)-CurPos);
                  edt_1->SetString(Str);
                  for (I=FocusPos+1;I<ItemCount;I++)
                    if (Item[I].Flags & DIF_EDITOR)
                    {
                      if (I>FocusPos)
                      {
                        ((Edit *)(Item[I].ObjPtr))->GetString(Str,sizeof(Str));
                        ((Edit *)(Item[I-1].ObjPtr))->SetString(Str);
                      }
                      ((Edit *)(Item[I].ObjPtr))->SetString("");
                    }
                    else
                      break;
                   ProcessKey(KEY_UP);
                   edt_1->SetCurPos(CurPos);
                }
              }
              else
              {
                edt->ProcessKey(Key);
              }
              Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
              ShowDialog();
              return(TRUE);
            }
            /* SVS $ */

            case KEY_CTRLY:
              for (I=FocusPos;I<ItemCount;I++)
                if (Item[I].Flags & DIF_EDITOR)
                {
                  if (I>FocusPos)
                  {
                    ((Edit *)(Item[I].ObjPtr))->GetString(Str,sizeof(Str));
                    ((Edit *)(Item[I-1].ObjPtr))->SetString(Str);
                  }
                  ((Edit *)(Item[I].ObjPtr))->SetString("");
                }
                else
                  break;
              /* $ 28.07.2000 SVS
                �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
                ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
              */
              Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
              /* SVS $ */
              ShowDialog();
              return(TRUE);

            case KEY_DEL:
              /* $ 19.07.2000 SVS
                 ! "...� ।���� ������ ���� ������ home shift+end del
                   ���� �� 㤠�����..."
                   DEL � �⥬��, ������ DIF_EDITOR, ࠡ�⠫ ��� ���
                   �뤥�����...
              */
              if (FocusPos<ItemCount+1 && (Item[FocusPos+1].Flags & DIF_EDITOR))
              {
                int CurPos=edt->GetCurPos();
                int Length=edt->GetLength();
                int SelStart, SelEnd;

                edt->GetSelection(SelStart, SelEnd);
                edt->GetString(Str,sizeof(Str));
                int LengthStr=strlen(Str);
                if(SelStart > -1)
                {
                  memmove(&Str[SelStart],&Str[SelEnd],Length-SelEnd+1);
                  edt->SetString(Str);
                  edt->SetCurPos(SelStart);
                  /* $ 28.07.2000 SVS
                    �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
                    ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
                  */
                  Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
                  /* SVS $ */
                  ShowDialog();
                  return(TRUE);
                }
                else if (CurPos>=Length)
                {
                  Edit *edt_1=(Edit *)Item[FocusPos+1].ObjPtr;
                  /* $ 12.09.2000 SVS
                     ��蠥� �஡����, �᫨ Del ������ � ����樨
                     ����襩, 祬 ����� ��ப�
                  */
                  if (CurPos > Length)
                  {
                    LengthStr=CurPos;
                    memset(Str+Length,' ',CurPos-Length);
                  }
                  /* SVS $*/
                  edt_1->GetString(Str+LengthStr,sizeof(Str)-LengthStr);
                  edt_1->SetString(Str);
                  ProcessKey(KEY_CTRLY);
                  edt->SetCurPos(CurPos);
                  ShowDialog();
                  return(TRUE);
                }
              }
              break;
              /* SVS $*/
            case KEY_PGUP:
              ProcessKey(KEY_SHIFTTAB);
              ProcessKey(KEY_DOWN);
              return(TRUE);
          }

        /* $ 24.09.2000 SVS
           �맮� �㭪樨 Xlat
        */
        if(Opt.XLatDialogKey && Key == Opt.XLatDialogKey)
        {
          edt->Xlat();
          return TRUE;
        }
        /* SVS $ */

        if (edt->ProcessKey(Key))
        {
          /* $ 26.07.2000 SVS
             AutoComplite: �᫨ ��⠭����� DIF_HISTORY
                 � ࠧ�襭� ��⮧����襭��!.
          */
          if(Opt.AutoComplete && Key < 256 && Key != KEY_BS && Key != KEY_DEL &&
             ((Item[FocusPos].Flags & DIF_HISTORY) || Type == DI_COMBOBOX)
            )
          {
            /* $ 01.08.2000 SVS
               ������让 ���祪 � AutoComplete
            */
            int CurPos=edt->GetCurPos();
            /* SVS $*/
            //text to search for
            edt->GetString(Str,sizeof(Str));
            edt->GetSelection(SelStart,SelEnd);
            if(SelStart <= 0)
              SelStart=sizeof(Str);
            else
              SelStart++;

            // � ��� �� ����ﭭ�� ������ ���⪠ ��ப� ��� ������
            //  ���� �᫨ ���� ����祭 � ��񤪥... :-)
            if(Opt.EditorPersistentBlocks)
            {
              // ������ �� ���������� :-)
              if(CurPos <= SelEnd)
              {
                Str[CurPos]=0;
                edt->Select(CurPos,sizeof(Str)); //select the appropriate text
                edt->DeleteBlock();
                edt->FastShow();
              }
            }
            SelEnd=strlen(Str);
            //find the string in the list
            if (FindInEditForAC(Type == DI_COMBOBOX,
                         (void *)Item[FocusPos].Selected,Str))
            {
              edt->SetString(Str);
              edt->Select(SelEnd,sizeof(Str)); //select the appropriate text
              /* $ 01.08.2000 SVS
                 ������让 ���祪 � AutoComplete
              */
              edt->SetCurPos(CurPos); // SelEnd
              /* SVS $*/
              /* $ 28.07.2000 SVS
                �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
                ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
              */
              Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
              /* SVS $ */
              Redraw();
            }
          }
          /* SVS $ */
          return(TRUE);
        }
        /* SVS 21.08.2000 $ */
      }

      if (ProcessHighlighting(Key,FocusPos,FALSE))
        return(TRUE);

      return(ProcessHighlighting(Key,FocusPos,TRUE));
  }
}


//////////////////////////////////////////////////////////////////////////
/* Public, Virtual:
   ��ࠡ�⪠ ������ �� "���".
   ��४�뢠�� BaseInput::ProcessMouse.
*/
/* $ 18.08.2000 SVS
   + DN_MOUSECLICK
*/
int Dialog::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  int I, J;
  int MsX,MsY;
  int Type;

  if (MouseEvent->dwButtonState==0)
    return(FALSE);
  if(!CheckDialogMode(DMODE_SHOW))
    return FALSE;

  MsX=MouseEvent->dwMousePosition.X;
  MsY=MouseEvent->dwMousePosition.Y;
  if (MsX<X1 || MsY<Y1 || MsX>X2 || MsY>Y2)
  {
    DlgProc((HANDLE)this,DN_MOUSECLICK,-1,(long)MouseEvent);
    /* $ 09.09.2000 SVS
       ��⥬ DMODE_OLDSTYLE - �뢠���� �� �������, ⪭� ��� �������
       ᬮ��� ⮫쪮 � ��஬ �⨫�.
    */
    if(CheckDialogMode(DMODE_OLDSTYLE))
    {
      if (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
        ProcessKey(KEY_ESC);
      else
        ProcessKey(KEY_ENTER);
    }
    else
      MessageBeep(MB_ICONHAND);
    /* SVS $ */
    return(TRUE);
  }

  if (MouseEvent->dwEventFlags==0)
  {
    /* $ 21.08.2000 SVS
       DN_MOUSECLICK - ��ࢨ筮.
    */
    for (I=0;I<ItemCount;I++)
    {
      int IX1=Item[I].X1+X1,
          IY1=Item[I].Y1+Y1,
          IX2=Item[I].X2+X1,
          IY2=Item[I].Y2+Y1;
      BOOL Send_DN=TRUE;

      if(MsX >= IX1 && MsY >= IY1 && MsY <= IY2 && MsX <= IX2)
      {
        switch(Item[I].Type)
        {
          case DI_SINGLEBOX:
          case DI_DOUBLEBOX:
            if(((MsX == IX1 || MsX == IX2) && MsY >= IY1 && MsY <= IY2) || // vert
               ((MsY == IY1 || MsY == IY2) && MsX >= IX1 && MsX <= IX2) )   // hor
                break;
            Send_DN=FALSE;
            break;
          case DI_USERCONTROL:
            // ��� user-⨯� �����⮢�� ���न���� ���
            MouseEvent->dwMousePosition.X-=IX1;
            MouseEvent->dwMousePosition.Y-=IY1;
            break;
        }
        if(Send_DN)
          if((J=DlgProc((HANDLE)this,DN_MOUSECLICK,I,(long)MouseEvent)))
            return TRUE;

        if(Item[I].Type == DI_USERCONTROL)
        {
           ChangeFocus2(FocusPos,I);
           ShowDialog();
           return(TRUE);
        }
      }
    }
    /* SVS $ */

    if((MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
    {
      for (I=0;I<ItemCount;I++)
      {
        Type=Item[I].Type;
        if (MsX>=X1+Item[I].X1)
        {
          /* $ 01.08.2000 SVS
             ����� ListBox
          */
          if(Type == DI_LISTBOX    &&
              MsY >= Y1+Item[I].Y1 &&
              MsY <= Y1+Item[I].Y2 &&
              MsX <= X1+Item[I].X2)
          {
            if(FocusPos != I)
              ChangeFocus2(FocusPos,I);
            ShowDialog();
            ((VMenu *)(Item[I].ObjPtr))->ProcessMouse(MouseEvent);
            return(TRUE);
          }
          /* SVS $ */

          if (IsEdit(Type))
          {
            /* $ 15.08.2000 SVS
               + ������� ⠪, �⮡� ⪭� ��誮� � DropDownList
                 ᯨ᮪ ���뢠��� ᠬ.
               ���� ������� ������� - ����� ᯨ᮪ ����� � ��
               ��让 ��ॢ������� �� ��㣮� �����, � ᯨ᮪ ����뢠����
               �� ���室� ॠ�쭮�� �� 㪠����� ����� ������� �� �ந�室��
            */
            int EditX1,EditY1,EditX2,EditY2;
            Edit *EditLine=(Edit *)(Item[I].ObjPtr);
            EditLine->GetPosition(EditX1,EditY1,EditX2,EditY2);

            if(MsY==EditY1 && Type == DI_COMBOBOX &&
               (Item[I].Flags & DIF_DROPDOWNLIST) &&
               MsX >= EditX1 && MsX <= EditX2+1)
            {
              EditLine->SetClearFlag(0);
              ChangeFocus2(FocusPos,I);
              ShowDialog();
              ProcessKey(KEY_CTRLDOWN);
              return(TRUE);
            }
            /* SVS $ */

            if (EditLine->ProcessMouse(MouseEvent))
            {
              EditLine->SetClearFlag(0);
              ChangeFocus2(FocusPos,I);
              ShowDialog();
              return(TRUE);
            }
            else
            {
              /* $ 18.07.2000 SVS
                 + �஢�ઠ �� ⨯ ����� DI_COMBOBOX
              */
              if (MsX==EditX2+1 && MsY==EditY1 && Item[I].History &&
                  ((Item[I].Flags & DIF_HISTORY) && Opt.DialogsEditHistory
                   || Type == DI_COMBOBOX))
              /* SVS $ */
              {
                ChangeFocus2(FocusPos,I);
                ProcessKey(KEY_CTRLDOWN);
                return(TRUE);
              }
            }
          }
          if (Type==DI_BUTTON &&
              MsY==Y1+Item[I].Y1 &&
              MsX < X1+Item[I].X1+HiStrlen(Item[I].Data))
          {
            ChangeFocus2(FocusPos,I);
            ShowDialog();
            while (IsMouseButtonPressed())
              ;
            if (MouseX <  X1 ||
                MouseX >  X1+Item[I].X1+HiStrlen(Item[I].Data)+4 ||
                MouseY != Y1+Item[I].Y1)
            {
              ChangeFocus2(FocusPos,I);
              ShowDialog();
              return(TRUE);
            }
            ProcessKey(KEY_ENTER);
            return(TRUE);
          }

          if ((Type == DI_CHECKBOX ||
               Type == DI_RADIOBUTTON) &&
              MsY==Y1+Item[I].Y1 &&
              MsX < (X1+Item[I].X1+HiStrlen(Item[I].Data)+4-((Item[I].Flags & DIF_MOVESELECT)!=0)))
          {
            ChangeFocus2(FocusPos,I);
            ProcessKey(KEY_SPACE);
            return(TRUE);
          }
        }
      } // for (I=0;I<ItemCount;I++)
      // ��� MOUSE-��६�頫��:
      //   � �������� � ⮬ ��砥, �᫨ ���� �� ������ �� ��⨢�� ������
      //
      /* $ 10.08.2000 SVS
         �������, �᫨ ࠧ�襭�! (IsCanMove)
      */
      if (CheckDialogMode(DMODE_ISCANMOVE))
      {
        /* $ 03.08.2000 tran
           �� ࠧ �������� - � �㤥� ��६���� */
        //SetDialogMode(DMODE_DRAGGED);
        OldX1=X1; OldX2=X2; OldY1=Y1; OldY2=Y2;
        MsX=MouseX;
        MsY=MouseY;
        /* $ 11.08.2000 KM
           ��६�����, ���������� ��ࢮ� ��������� � ��楤���
           ��६�饭�� �������.
        */
        static int First=TRUE;
        /* KM $ */
        while (1)
        {
            int mb=IsMouseButtonPressed();
            /* $ 09.08.2000 tran
               - ����� "ᠫ����" :) */
            /* $ 11.08.2000 KM
               �᫨ ���� ࠧ ��諮 ᮮ�饭�� � ����⮩ ������ ���,
               � ����⢥��� �� ��室�� � ��楤��� ��६�饭��, �⮡�
               ���४⭮ �⮡ࠧ��� ����祭�� �⮣� ०��� �ࠧ� ��, �
               �� ��᫥ ��砫� ��������.
            */
            if ( mb==1 && MouseX==MsX && MouseY==MsY && !First )
                continue;
            MsX=MouseX;
            MsY=MouseY;
            /* KM $ */
            /* tran 09.08.2000 $ */

            int mx,my;
//            SysLog("MouseMove:(), MouseX=%i, MousePrevX=%i,MouseY=%i, MousePrevY=%i",MouseX,PrevMouseX,MouseY,PrevMouseY);
            if ( mb==1 ) // left key, still dragging
            {
                Hide();
                /* 11.08.2000 KM
                   ���䠪�: �᫨ ��᫥ ��ࢮ�� ����᪠ ��, �� ��ଥ�� ����
                   ������ ����� ������, �⮡� ��६����� ������, � ��-��
                   ⮣�, �� � �⮬� ������� PrevMouseX � PrevMouseY ��� �� ��।�����,
                   ⠪�� ����।���� ����砫�� ��릮� �������. ���⮬� �� ��ࢮ��
                   �室� � ०�� ��६�饭�� ���樠�����㥬 ��६���� 0.
                */
                if (First)
                  mx=my=0;
                else
                {
                  mx=MouseX-PrevMouseX;
                  my=MouseY-PrevMouseY;
                }
                /* KM $ */
                if ( X1+mx>=0 && X2+mx<=ScrX )
                {
                    X1+=mx;
                    X2+=mx;
                    AdjustEditPos(mx,0);
                }
                if ( Y1+my>=0 && Y2+my<=ScrY )
                {
                    Y1+=my;
                    Y2+=my;
                    AdjustEditPos(0,my);
                }
                Show();
            }
            else if (mb==2) // right key, abort
            {
                Hide();
                AdjustEditPos(OldX1-X1,OldY1-Y1);
                X1=OldX1;
                X2=OldX2;
                Y1=OldY1;
                Y2=OldY2;
                /* $ 11.08.2000 KM
                   �� ��室� �� ०��� ��६�饭�� �����⮢��
                   ��६����� ��� ��᫥���饣� ���४⭮�� �室�.
                */
                First=TRUE;
                /* KM $ */
                SkipDialogMode(DMODE_DRAGGED);
                Show();
                break;
            }
            else  // release key, drop dialog
            {
                /* $ 11.08.2000 KM
                   �� ��室� �� ०��� ��६�饭�� �����⮢��
                   ��६����� ��� ��᫥���饣� ���४⭮�� �室�.
                */
                First=TRUE;
                /* KM $ */
                SkipDialogMode(DMODE_DRAGGED);
                Show();
                break;
            }
            /* $ 11.08.2000 KM
               ��� 墠��, ���� ࠧ �� 㦥 ��室���...
            */
            First=FALSE;
            /* KM $ */
        }// while (1)
        /* tran 03.08.2000 $ */
      }
      /* SVS 10.08.2000 $*/
    }
  }
  return(FALSE);
}
/* SVS 18.08.2000 $ */


//////////////////////////////////////////////////////////////////////////
/* Private:
   ������� 䮪�� ����� (�������⢨� �����蠬�
     KEY_TAB, KEY_SHIFTTAB, KEY_UP, KEY_DOWN,
   � ⠪ �� Alt-HotKey)
*/
/* $ 28.07.2000 SVS
   ����᮪ ��� ᮮ�饭�� DN_KILLFOCUS & DN_SETFOCUS
*/
/* $ 24.08.2000 SVS
   ������� ��� DI_USERCONTROL
*/
int Dialog::ChangeFocus(int FocusPos,int Step,int SkipGroup)
{
  int Type,OrigFocusPos=FocusPos;
//  int FucusPosNeed=-1;
  // � �㭪�� ��ࠡ�⪨ ������� ����� ��।��� ᮮ�饭��,
  //   �� ����� - LostFocus() - ���� 䮪�� �����.
//  if(CheckDialogMode(DMODE_INITOBJECTS))
//    FucusPosNeed=DlgProc((HANDLE)this,DN_KILLFOCUS,FocusPos,0);
//  if(FucusPosNeed != -1 && IsFocused(Item[FucusPosNeed].Type))
//    FocusPos=FucusPosNeed;
//  else
  {
    while (1)
    {
      FocusPos+=Step;
      if (FocusPos>=ItemCount)
        FocusPos=0;
      if (FocusPos<0)
        FocusPos=ItemCount-1;

      Type=Item[FocusPos].Type;

      if(!(Item[FocusPos].Flags&DIF_NOFOCUS))
      {
        if (Type==DI_LISTBOX || Type==DI_BUTTON || Type==DI_CHECKBOX || IsEdit(Type) || Type==DI_USERCONTROL)
          break;
        if (Type==DI_RADIOBUTTON && (!SkipGroup || Item[FocusPos].Selected))
          break;
      }
      // 㡨ࠥ� ��横������� � ��᫥���騬 �����ᠭ��� :-)
      if(OrigFocusPos == FocusPos)
        break;
    }
  }

//  Dialog::FocusPos=FocusPos;
  // � �㭪�� ��ࠡ�⪨ ������� ����� ��।��� ᮮ�饭��,
  //   �� ����� GotFocus() - ����稫 䮪�� �����.
  // ������㥬 �����頥��� �㭪樥� ������� ���祭��
//  if(CheckDialogMode(DMODE_INITOBJECTS))
//    DlgProc((HANDLE)this,DN_GOTFOCUS,FocusPos,0);
  return(FocusPos);
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   Private:
   ������� 䮪�� ����� ����� ���� ����⠬�.
   �뭥ᥭ �⤥�쭮 � ⥬, �⮡� ��ࠡ���� DN_KILLFOCUS & DM_SETFOCUS
*/
int Dialog::ChangeFocus2(int KillFocusPos,int SetFocusPos)
{
  int FucusPosNeed=-1;
  if(!(Item[SetFocusPos].Flags&DIF_NOFOCUS))
  {
    if(CheckDialogMode(DMODE_INITOBJECTS))
      FucusPosNeed=DlgProc((HANDLE)this,DN_KILLFOCUS,KillFocusPos,0);

    if(FucusPosNeed != -1 && IsFocused(Item[FucusPosNeed].Type))
      SetFocusPos=FucusPosNeed;

    if(Item[SetFocusPos].Flags&DIF_NOFOCUS)
       SetFocusPos=KillFocusPos;

    Item[KillFocusPos].Focus=0;
    Item[SetFocusPos].Focus=1;

    Dialog::PrevFocusPos=Dialog::FocusPos;
    Dialog::FocusPos=SetFocusPos;
    if(CheckDialogMode(DMODE_INITOBJECTS))
      DlgProc((HANDLE)this,DN_GOTFOCUS,SetFocusPos,0);
  }
  else
    SetFocusPos=KillFocusPos;

  return(SetFocusPos);
}
/* SVS $ */

/* $ 08.09.2000 SVS
  �㭪�� SelectOnEntry - �뤥����� ��ப� ।���஢����
  ��ࠡ�⪠ 䫠�� DIF_SELECTONENTRY
*/
void Dialog::SelectOnEntry(int Pos)
{
  if(IsEdit(Item[Pos].Type) &&
     (Item[Pos].Flags&DIF_SELECTONENTRY)
//     && PrevFocusPos != -1 && PrevFocusPos != Pos
    )
  {
    Edit *edt=(Edit *)Item[Pos].ObjPtr;
    edt->Select(0,edt->GetLength());
  }
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   Public, Static:
   + �㭪�� ConvertItem - ���⭮� �८�ࠧ������ ����⮢ ������� ��
   ����७���� �।�⠢����� �� ���譨�
*/
void Dialog::ConvertItem(int FromPlugin,
                         struct FarDialogItem *Item,struct DialogItem *Data,
                         int Count)
{
  int I;
  if(FromPlugin == CVTITEM_TOPLUGIN)
    for (I=0; I < Count; I++)
    {
      Item[I].Type=Data[I].Type;
      Item[I].X1=Data[I].X1;
      Item[I].Y1=Data[I].Y1;
      Item[I].X2=Data[I].X2;
      Item[I].Y2=Data[I].Y2;
      Item[I].Focus=Data[I].Focus;
      Item[I].Selected=Data[I].Selected;
      Item[I].Flags=Data[I].Flags;
      Item[I].DefaultButton=Data[I].DefaultButton;
      strcpy(Item[I].Data,Data[I].Data);
    }
  else
    for (I=0; I < Count; I++)
    {
      Data[I].Type=Item[I].Type;
      Data[I].X1=Item[I].X1;
      Data[I].Y1=Item[I].Y1;
      Data[I].X2=Item[I].X2;
      Data[I].Y2=Item[I].Y2;
      Data[I].Focus=Item[I].Focus;
      Data[I].Selected=Item[I].Selected;
      Data[I].Flags=Item[I].Flags;
      Data[I].DefaultButton=Item[I].DefaultButton;
      strcpy(Data[I].Data,Item[I].Data);
    }
}
/* SVS $ */

//////////////////////////////////////////////////////////////////////////
/* Public, Static:
   �८�ࠧ�� ����� �� ������ ������� �� ����७���
   �।�⠢�����. �������祭 �㭪樨 InitDialogItems (�. "Far PlugRinG
   Russian Help Encyclopedia of Developer")
*/
void Dialog::DataToItem(struct DialogData *Data,struct DialogItem *Item,
                        int Count)
{
  int I;
  for (I=0;I<Count;I++)
  {
    Item[I].Type=Data[I].Type;
    Item[I].X1=Data[I].X1;
    Item[I].Y1=Data[I].Y1;
    Item[I].X2=Data[I].X2;
    Item[I].Y2=Data[I].Y2;
    Item[I].Focus=Data[I].Focus;
    Item[I].Selected=Data[I].Selected;
    Item[I].Flags=Data[I].Flags;
    Item[I].DefaultButton=Data[I].DefaultButton;
    if ((unsigned int)Data[I].Data<MAX_MSG)
      strcpy(Item[I].Data,MSG((unsigned int)Data[I].Data));
    else
      strcpy(Item[I].Data,Data[I].Data);
    Item[I].ObjPtr=NULL;
  }
}


//////////////////////////////////////////////////////////////////////////
/* Private:
   �஢���� ⨯ ����� ������� �� �।��� ��ப� �����
   (DI_EDIT, DI_FIXEDIT, DI_PSWEDIT) � � ��砥 �ᯥ� �����頥� TRUE
*/
/* $ 18.07.2000 SVS
   ! ����� DI_COMBOBOX �⭮���� � ��⥣�ਨ ��ப���� ।���஢...
*/
int Dialog::IsEdit(int Type)
{
  return(Type==DI_EDIT ||
         Type==DI_FIXEDIT ||
         Type==DI_PSWEDIT ||
         Type == DI_COMBOBOX);
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   �㭪��, ��।������ - "����� �� ����� ������� ����� 䮪�� �����"
*/
/* $ 24.08.2000 SVS
   ������� ��� DI_USERCONTROL
*/
int Dialog::IsFocused(int Type)
{
  return(Type==DI_EDIT ||
         Type==DI_FIXEDIT ||
         Type==DI_PSWEDIT ||
         Type==DI_COMBOBOX ||
         Type==DI_BUTTON ||
         Type==DI_CHECKBOX ||
         Type==DI_RADIOBUTTON ||
         Type==DI_LISTBOX ||
         Type==DI_USERCONTROL);
}
/* 24.08.2000 SVS $ */
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 26.07.2000 SVS
   AutoComplite: ���� �室����� �����ப� � ���ਨ
*/
/* $ 28.07.2000 SVS
   ! ��६��� Edit *EditLine ��䨣 ���㦥�!
*/
int Dialog::FindInEditForAC(int TypeFind,void *HistoryName,char *FindStr)
{
  char Str[1024];
  int I, Count;

  if(!TypeFind)
  {
    char RegKey[80],KeyValue[80];
    sprintf(RegKey,fmtSavedDialogHistory,(char*)HistoryName);
    // ��ᬮ�� �㭪⮢ ���ਨ
    for (I=0; I < 16; I++)
    {
      sprintf(KeyValue,fmtLine,I);
      GetRegKey(RegKey,KeyValue,Str,"",sizeof(Str));
      if (!LocalStrnicmp(Str,FindStr,strlen(FindStr)))
        break;
    }
    if (I == 16)
      return FALSE;
    /* $ 28.07.2000 SVS
       �������� �㪮��� �� ���ࠣ�����, � ������塞 �������饥.
    */
    strcat(FindStr,&Str[strlen(FindStr)]);
    /* SVS $ */
  }
  else
  {
    struct FarListItem *ListItems=((struct FarList *)HistoryName)->Items;
    int Count=((struct FarList *)HistoryName)->ItemsNumber;

    for (I=0; I < Count ;I++)
    {
      if (!LocalStrnicmp(ListItems[I].Text,FindStr,strlen(FindStr)))
        break;
    }
    if (I  == Count)
      return FALSE;
    strcat(FindStr,&ListItems[I].Text[strlen(FindStr)]);
  }
  return TRUE;
}
/*  SVS $ */


//////////////////////////////////////////////////////////////////////////
/* Private:
   ������塞 �믠���騩 ᯨ᮪ �� ���ਨ
*/
/* $ 26.07.2000 SVS
  + �������⥫�� ��ࠬ��� � SelectFromEditHistory ��� �뤥�����
   �㦭�� ����樨 � ���ਨ (�᫨ ��� ᮮ⢥����� ��ப� �����)
*/
void Dialog::SelectFromEditHistory(Edit *EditLine,char *HistoryName,char *IStr)
/* SVS $ */
{
  char RegKey[80],KeyValue[80],Str[512];
  int I,Dest;
  int Checked;

  sprintf(RegKey,fmtSavedDialogHistory,HistoryName);
  {
    // ᮧ����� ���⮣� ���⨪��쭮�� ����
    VMenu HistoryMenu("",NULL,0,8,VMENU_ALWAYSSCROLLBAR);

    struct MenuItem HistoryItem;
    int EditX1,EditY1,EditX2,EditY2;
    int ItemsCount;

    EditLine->GetPosition(EditX1,EditY1,EditX2,EditY2);
    if (EditX2-EditX1<20)
      EditX2=EditX1+20;
    if (EditX2>ScrX)
      EditX2=ScrX;

    HistoryItem.Checked=HistoryItem.Separator=0;
    HistoryMenu.SetFlags(MENU_SHOWAMPERSAND);
    HistoryMenu.SetPosition(EditX1,EditY1+1,EditX2,0);
    HistoryMenu.SetBoxType(SHORT_SINGLE_BOX);

    // ���������� �㭪⮢ ����
    ItemsCount=0;
    for (Dest=I=0; I < 16; I++)
    {
      sprintf(KeyValue,fmtLine,I);
      GetRegKey(RegKey,KeyValue,Str,"",sizeof(Str));
      if (*Str==0)
        continue;

      sprintf(KeyValue,fmtLocked,I);

      GetRegKey(RegKey,KeyValue,(int)Checked,0);
      HistoryItem.Checked=Checked;
      /* $ 26.07.2000 SVS
         ���⠢�� Selected �� ������ ᮢ������� ��ப� ����� � ���ਨ
      */
      if((HistoryItem.Selected=(!Dest && !strcmp(IStr,Str))?TRUE:FALSE) == TRUE)
         Dest++;
      /* SVS $ */
      strncpy(HistoryItem.Name,Str,sizeof(HistoryItem.Name)-1);
      HistoryItem.Name[sizeof(HistoryItem.Name)-1]=0;
      strncpy(HistoryItem.UserData,Str,sizeof(HistoryItem.UserData));
      HistoryItem.UserDataSize=strlen(Str)+1;
      HistoryMenu.AddItem(&HistoryItem);
      ItemsCount++;
    }
    if (ItemsCount==0)
      return;

    /* $ 28.07.2000 SVS
       ��। ���ᮢ��� ��ᨬ �� ��������� 梥⮢�� ��ਡ�⮢
    */
    short Colors[9];
    HistoryMenu.GetColors(Colors);
    if(DlgProc((HANDLE)this,DN_CTLCOLORDLGLIST,
                    sizeof(Colors)/sizeof(Colors[0]),(long)Colors))
      HistoryMenu.SetColors(Colors);
    /* SVS $ */
    HistoryMenu.Show();
    while (!HistoryMenu.Done())
    {
      int Key=HistoryMenu.ReadInput();

      // Del ��頥� ����� ������.
      if (Key==KEY_DEL)
      {
        int Locked;
        for (I=0,Dest=0;I<16;I++)
        {
          sprintf(KeyValue,fmtLine,I);
          GetRegKey(RegKey,KeyValue,Str,"",sizeof(Str));
          DeleteRegValue(RegKey,KeyValue);
          sprintf(KeyValue,fmtLocked,I);
          GetRegKey(RegKey,KeyValue,Locked,0);
          DeleteRegValue(RegKey,KeyValue);

          // ����祭�� �㭪�� ���ਨ �� 㤠������
          if (Locked)
          {
            sprintf(KeyValue,fmtLine,Dest);
            SetRegKey(RegKey,KeyValue,Str);
            sprintf(KeyValue,fmtLocked,Dest);
            SetRegKey(RegKey,KeyValue,TRUE);
            Dest++;
          }
        }
        HistoryMenu.Hide();
        SelectFromEditHistory(EditLine,HistoryName,IStr);
        return;
      }

      // Ins ���頥� �㭪� ���ਨ �� 㤠�����.
      if (Key==KEY_INS)
      {
        sprintf(KeyValue,fmtLocked,HistoryMenu.GetSelectPos());
        if (!HistoryMenu.GetSelection())
        {
          HistoryMenu.SetSelection(TRUE);
          SetRegKey(RegKey,KeyValue,1);
        }
        else
        {
          HistoryMenu.SetSelection(FALSE);
          DeleteRegValue(RegKey,KeyValue);
        }
        HistoryMenu.SetUpdateRequired(TRUE);
        HistoryMenu.Redraw();
        continue;
      }
      HistoryMenu.ProcessInput();
    }

    int ExitCode=HistoryMenu.GetExitCode();
    if (ExitCode<0)
      return;
    HistoryMenu.GetUserData(Str,sizeof(Str),ExitCode);
  }
  EditLine->SetString(Str);
  EditLine->SetLeftPos(0);
  Redraw();
}


//////////////////////////////////////////////////////////////////////////
/* Private:
   ����� � ���ਥ� - ���������� � reorder ᯨ᪠
*/
void Dialog::AddToEditHistory(char *AddStr,char *HistoryName)
{
  int LastLine=15,FirstLine=16, I, Locked;

  if (*AddStr==0)
    return;

  char RegKey[80],SrcKeyValue[80],DestKeyValue[80],Str[512];
  sprintf(RegKey,fmtSavedDialogHistory,HistoryName);

  for (I=0; I < 16; I++)
  {
    sprintf(SrcKeyValue,fmtLocked,I);
    GetRegKey(RegKey,SrcKeyValue,Locked,0);
    if (!Locked)
    {
      FirstLine=I;
      break;
    }
  }

  for (I=0; I < 16; I++)
  {
    sprintf(SrcKeyValue,fmtLine,I);
    GetRegKey(RegKey,SrcKeyValue,Str,"",sizeof(Str));
    if (strcmp(Str,AddStr)==0)
    {
      LastLine=I;
      break;
    }
  }

  if (FirstLine<=LastLine)
  {
    for (int Src=LastLine-1;Src>=FirstLine;Src--)
    {
      sprintf(SrcKeyValue,fmtLocked,Src);
      GetRegKey(RegKey,SrcKeyValue,Locked,0);
      if (Locked)
        continue;
      for (int Dest=Src+1;Dest<=LastLine;Dest++)
      {
        sprintf(DestKeyValue,fmtLocked,Dest);
        GetRegKey(RegKey,DestKeyValue,Locked,0);
        if (!Locked)
        {
          sprintf(SrcKeyValue,fmtLine,Src);
          GetRegKey(RegKey,SrcKeyValue,Str,"",sizeof(Str));
          sprintf(DestKeyValue,fmtLine,Dest);
          SetRegKey(RegKey,DestKeyValue,Str);
          break;
        }
      }
    }
    char FirstLineKeyValue[20];
    sprintf(FirstLineKeyValue,fmtLine,FirstLine);
    SetRegKey(RegKey,FirstLineKeyValue,AddStr);
  }
}


//////////////////////////////////////////////////////////////////////////
/* Public, Static:
   �஢�ઠ �� HotKey
*/
int Dialog::IsKeyHighlighted(char *Str,int Key,int Translate)
{
  if ((Str=strchr(Str,'&'))==NULL)
    return(FALSE);
  int UpperStrKey=LocalUpper(Str[1]);
  if (Key<256)
    return(UpperStrKey==LocalUpper(Key) ||
           Translate && UpperStrKey==LocalUpper(LocalKeyToKey(Key)));
  if (Key>=KEY_ALT0 && Key<=KEY_ALT9)
    return(Key-KEY_ALT0+'0'==UpperStrKey);
  if (Key>=KEY_ALTA && Key<=KEY_ALT_BASE+255)
  {
    int AltKey=Key-KEY_ALTA+'A';
    return(UpperStrKey==LocalUpper(AltKey) ||
           Translate && UpperStrKey==LocalUpper(LocalKeyToKey(AltKey)));
  }
  return(FALSE);
}


//////////////////////////////////////////////////////////////////////////
/* Private:
   �᫨ �����㫨 Alt-???
*/
int Dialog::ProcessHighlighting(int Key,int FocusPos,int Translate)
{
  int I;
  for (I=0;I<ItemCount;I++)
  {
    if (!IsEdit(Item[I].Type) && (Item[I].Flags & DIF_SHOWAMPERSAND)==0)
      if (IsKeyHighlighted(Item[I].Data,Key,Translate))
      {
        int DisableSelect=FALSE;
        /* $ 28.07.2000 SVS
           ����騬 � ��稢饬�� 䠪� ��楤�� ��ࠡ�⪨ �������
        */
        if(!DlgProc((HANDLE)this,DN_HOTKEY,I,Key))
          break; // ᪠���� �� �த������ ��ࠡ���...
        /* SVS $ */

        //???? ����� �஢���� �� ������ DI_COMBOBOX!!!
        if (I>0 && Item[I].Type==DI_TEXT && IsEdit(Item[I-1].Type) &&
            Item[I].Y1==Item[I-1].Y1 && Item[I].Y1!=Item[I+1].Y1)
        {
          I=ChangeFocus(I,-1,FALSE);
          DisableSelect=TRUE;
        }
        else
          if (Item[I].Type==DI_TEXT || Item[I].Type==DI_VTEXT ||
              Item[I].Type==DI_SINGLEBOX || Item[I].Type==DI_DOUBLEBOX)
          {
            I=ChangeFocus(I,1,FALSE);
            DisableSelect=TRUE;
          }
        /* $ 29.08.2000 SVS
           - ���� ��樠��� ����-��� - �㭪�� ProcessHighlighting
           MY> ����� � ��������� �⠫� �������. � ���� � ���� ����稥 ������.
           MY> �室�� � ����ன�� 祣� 㣮���, ���� Alt-�㦭�� �㪢� �
           MY> ������ ࠧ����ࠧ�� ��.

           � ���稪 ���� ���뢠��� :-)))
        */
        ChangeFocus2(FocusPos,I);
        /* SVS $ */
        if ((Item[I].Type==DI_CHECKBOX || Item[I].Type==DI_RADIOBUTTON) &&
            (!DisableSelect || (Item[I].Flags & DIF_MOVESELECT)))
        {
          ProcessKey(KEY_SPACE);
          return(TRUE);
        }
        if (Item[I].Type==DI_BUTTON)
        {
          ProcessKey(KEY_ENTER);
          return(TRUE);
        }
        // �� ComboBox`� - "�뢠������" ��᫥���� //????
        if (Item[I].Type==DI_COMBOBOX)
        {
          ProcessKey(KEY_CTRLDOWN);
          return(TRUE);
        }
        ShowDialog();
        return(TRUE);
      }
  }
  return(FALSE);
}


//////////////////////////////////////////////////////////////////////////
/* Private:
   ������塞 �믠���騩 ᯨ᮪ ��� ComboBox
*/
/*
   $ 18.07.2000 SVS
   �㭪��-��ࠡ��稪 �롮� �� ᯨ᪠ � ��⠭����...
*/
void Dialog::SelectFromComboBox(
         Edit *EditLine,                   // ��ப� ।���஢����
         struct FarList *List,    // ᯨ᮪ ��ப
         char *IStr)
{
  char Str[512];
  struct MenuItem ComboBoxItem;
  struct FarListItem *ListItems=List->Items;
  int EditX1,EditY1,EditX2,EditY2;
  int I,Dest;

  // ᮧ����� ���⮣� ���⨪��쭮�� ����
  //  � ��易⥫�� ������� ScrollBar
  VMenu ComboBoxMenu("",NULL,0,8,VMENU_ALWAYSSCROLLBAR,NULL/*,this*/);

  EditLine->GetPosition(EditX1,EditY1,EditX2,EditY2);
  if (EditX2-EditX1<20)
    EditX2=EditX1+20;
  if (EditX2>ScrX)
    EditX2=ScrX;
#if 0
  if(!(Item[FocusPos].Flags&DIF_LISTNOAMPERSAND))
#endif
    ComboBoxMenu.SetFlags(MENU_SHOWAMPERSAND);
  ComboBoxMenu.SetPosition(EditX1,EditY1+1,EditX2,0);
  ComboBoxMenu.SetBoxType(SHORT_SINGLE_BOX);

  // ���������� �㭪⮢ ����
  /* ��᫥���� �㭪� ᯨ᪠ - ��࠭���⥫� - � ��� Tetx[0]
     ������ ���� ࠢ�� '\0'
  */
  for (Dest=I=0;I < List->ItemsNumber;I++)
  {
    /* $ 28.07.2000 SVS
       ���⠢�� Selected �� ������ ᮢ������� ��ப� ����� � ᯨ᪠
    */

    if(IStr && *IStr)
    {
      if((ComboBoxItem.Selected=(!Dest && !strcmp(IStr,ListItems[I].Text))?TRUE:FALSE) == TRUE)
         Dest++;
    }
    else
       ComboBoxItem.Selected=ListItems[I].Flags&LIF_SELECTED;

    ComboBoxItem.Separator=ListItems[I].Flags&LIF_SEPARATOR;
    ComboBoxItem.Checked=ListItems[I].Flags&LIF_CHECKED;
    /* 01.08.2000 SVS $ */
    /* SVS $ */
    strcpy(ComboBoxItem.Name,ListItems[I].Text);
    strcpy(ComboBoxItem.UserData,ListItems[I].Text);
    ComboBoxItem.UserDataSize=strlen(ListItems[I].Text);
    ComboBoxMenu.AddItem(&ComboBoxItem);
  }

  /* $ 28.07.2000 SVS
     ��। ���ᮢ��� ��ᨬ �� ��������� 梥⮢�� ��ਡ�⮢
  */
  short Colors[9];
  ComboBoxMenu.GetColors(Colors);
  if(DlgProc((HANDLE)this,DN_CTLCOLORDLGLIST,
                  sizeof(Colors)/sizeof(Colors[0]),(long)Colors))
    ComboBoxMenu.SetColors(Colors);
  /* SVS $ */

  ComboBoxMenu.Show();
//  Dest=ComboBoxMenu.GetSelectPos();
  while (!ComboBoxMenu.Done())
  {
    int Key=ComboBoxMenu.ReadInput();
    // ����� ����� �������� ��-� ᢮�, ���ਬ��,
//    I=ComboBoxMenu.GetSelectPos();
//    if(I != Dest)
//    {
//      Dest=I;
//      DlgProc((HANDLE)this,DN_LISTCHANGE,,(long)List);
//    }
    //  ��ࠡ��� multiselect ComboBox
    ComboBoxMenu.ProcessInput();
  }

  int ExitCode=ComboBoxMenu.GetExitCode();
  if (ExitCode<0)
    return;
  /* �������� ⥪�饥 ���ﭨ� */
  for (I=0; I < List->ItemsNumber; I++)
    ListItems[I].Flags&=~LIF_SELECTED;
  ListItems[ExitCode].Flags|=LIF_SELECTED;
  ComboBoxMenu.GetUserData(Str,sizeof(Str),ExitCode);

  EditLine->SetString(Str);
  EditLine->SetLeftPos(0);
  Redraw();
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 31.07.2000 tran
   + �㭪�� ���ࠢ������� ���न��� edit ����ᮢ */
/* $ 07.08.2000 SVS
   + � �� ListBox ���뫨?*/
void Dialog::AdjustEditPos(int dx, int dy)
{
  struct DialogItem *CurItem;
  int I;
  int x1,x2,y1,y2;

  ScreenObject *DialogEdit;
  for (I=0; I < ItemCount; I++)
  {
    CurItem=&Item[I];
    if (IsEdit(CurItem->Type) || CurItem->Type == DI_LISTBOX)
    {
       DialogEdit=(ScreenObject *)CurItem->ObjPtr;
       DialogEdit->GetPosition(x1,y1,x2,y2);
       x1+=dx;
       x2+=dx;
       y1+=dy;
       y2+=dy;
       DialogEdit->SetPosition(x1,y1,x2,y2);
    }
  }
}
/* SVS $ */
/* tran 31.07.2000 $ */


//////////////////////////////////////////////////////////////////////////
/* $ 11.08.2000 SVS
   ����� � ���. ����묨 ������� �������
   ���� ���⮥ ����஢���� (��᢮����)
*/
void Dialog::SetDialogData(long NewDataDialog)
{
  DataDialog=NewDataDialog;
}
/* SVS $ */

//////////////////////////////////////////////////////////////////////////
/* $ 11.08.2000 SVS
   + ��� ⮣�, �⮡� ��᫠�� DM_CLOSE �㦭� ��८�।����� Process
*/
void Dialog::Process()
{
  do{
    Modal::Process();
    if(DlgProc((HANDLE)this,DM_CLOSE,ExitCode,0) || ExitCode == -2)
      break;
    /* $ 18.08.2000 SVS
       ���-���, � �����-� 䫠� ���뫨 8-=(((
    */
    ClearDone();
    /* SVS $ */
  }while(1);
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   �㭪�� ��ࠡ�⪨ ������� (�� 㬮�砭��)
   ��� ������ �� �㭪�� � ���� ��᫥���� �㡥��� ��ࠡ�⪨ �������.
   �.�. ����� ������ ���� ��� ��ࠡ�⪠ ���� ᮮ�饭��!!!
*/
long WINAPI Dialog::DefDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  Dialog* Dlg=(Dialog*)hDlg;
  struct DialogItem *CurItem;
  char *Ptr, Str[1024];
  int Len, Type, I;

  if(!Dlg)
    return 0;

  switch(Msg)
  {
    case DN_INITDIALOG:
      return FALSE; // ��������� �� �뫮!

    case DM_CLOSE:
      return TRUE;  // ᮣ��ᥭ � �����⨥�

    case DN_KILLFOCUS:
      return -1;    // "�����ᥭ � ���३ 䮪��"

    case DN_GOTFOCUS:
      return 0;     // always 0

    case DN_HELP:
      return Param2; // �� ��।���, � �...

    case DN_DRAWDIALOG:
      return 0;

    case DN_CTLCOLORDIALOG:
      return Param2;

    case DN_CTLCOLORDLGITEM:
      return Param2;

    case DN_CTLCOLORDLGLIST:
      return FALSE;

    case DN_ENTERIDLE:
      return 0;     // always 0
  }

  // �।���⥫쭮 �஢�ਬ...
  if(Param1 >= Dlg->ItemCount)
    return 0;

  CurItem=&Dlg->Item[Param1];
  Type=CurItem->Type;

  Ptr=CurItem->Data;

  switch(Msg)
  {
    case DN_MOUSECLICK:
      return 0;

    case DN_DRAWDLGITEM:
      return 0;

    case DN_HOTKEY:
      return TRUE;

    case DN_EDITCHANGE:
      return TRUE;

    case DN_BTNCLICK:
      return TRUE;

    case DN_LISTCHANGE:
      return TRUE;

    /* $ 23.08.2000 SVS
       + ������� �������(�)
    */
    case DM_KEY:
      return FALSE;
    /* SVS $ */
  }

  return 0;
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   ���뫪� ᮮ�饭�� �������
   ������� ᮮ�饭�� �� �㭪�� ��ࠡ��뢠�� ᠬ�, �� ��।���� �ࠢ�����
   ��ࠡ��稪� �������.
*/
long WINAPI Dialog::SendDlgMessage(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  Dialog* Dlg=(Dialog*)hDlg;
  struct DialogItem *CurItem;
  char *Ptr, Str[1024];
  int Len, Type, I;
  struct FarDialogItem PluginDialogItem;

  if(!Dlg)
    return 0;
  // �।���⥫쭮 �஢�ਬ...
  if(Param1 >= Dlg->ItemCount)
    return 0;

//  CurItem=&Dlg->Item[Param1];
  CurItem=Dlg->Item+Param1;
  Type=CurItem->Type;
  Ptr=CurItem->Data;

  switch(Msg)
  {
    case DM_SETTEXT:
      if(Param2) // �᫨ ����� NULL, � �� �� ���� ᯮᮡ ������� ࠧ���
      {
        switch(Type)
        {
          case DI_USERCONTROL:
          case DI_TEXT:
          case DI_VTEXT:
          case DI_SINGLEBOX:
          case DI_DOUBLEBOX:
            strncpy(Ptr,(char *)Param2,512);
            if(Dlg->CheckDialogMode(DMODE_SHOW))
            {
              Dlg->ShowDialog(Param1);
              ScrBuf.Flush();
            }
            return strlen((char *)Param2)+1;

          case DI_BUTTON:
          case DI_CHECKBOX:
          case DI_RADIOBUTTON:
            strcpy(Ptr,(char *)Param2);
            break;

          case DI_COMBOBOX:
          case DI_EDIT:
          case DI_PSWEDIT:
          case DI_FIXEDIT:
            ((Edit *)(CurItem->ObjPtr))->SetString((char *)Param2);
            ((Edit *)(CurItem->ObjPtr))->Select(-1,-1); // ᭨���� �뤥�����
            break;

          case DI_LISTBOX: // ���� �� �ண��� - �� ॠ�������
            return 0;

          default:  // ���ࠧ㬥����, �� ��⠫���
            return 0;
        }
        Dlg->InitDialogObjects(Param1); // ��२��樠�����㥬 ������ �������
        if(Dlg->CheckDialogMode(DMODE_SHOW)) // �����筮 �� �⮣�????!!!!
        {
          Dlg->ShowDialog(Param1);
          ScrBuf.Flush();
        }
        return strlen((char *)Param2)+1;
      }
      return 0;

    case DM_SETTEXTLENGTH:
      if(IsEdit(Type) && !(CurItem->Flags & DIF_DROPDOWNLIST))
      {
        Param1=((Edit *)(CurItem->ObjPtr))->GetMaxLength();
        ((Edit *)(CurItem->ObjPtr))->SetMaxLength(Param2);
        Dlg->InitDialogObjects(Param1); // ��२��樠�����㥬 ������ �������
        return Param1;
      }
      return 0;

    case DN_LISTCHANGE:
      return Dlg->DlgProc(hDlg,Msg,Param1,(long)&CurItem->ListItems);

    case DN_EDITCHANGE:
      // �८�ࠧ㥬 ����� ���!
      Dialog::ConvertItem(CVTITEM_TOPLUGIN,&PluginDialogItem,CurItem,1);
      I=Dlg->DlgProc(hDlg,Msg,Param1,(long)&PluginDialogItem);
      Dialog::ConvertItem(CVTITEM_FROMPLUGIN,&PluginDialogItem,CurItem,1);
      return I;

    case DN_BTNCLICK:
      return Dlg->DlgProc(hDlg,Msg,Param1,Param2);

    case DN_DRAWDLGITEM:
      // �८�ࠧ㥬 ����� ���!
      Dialog::ConvertItem(CVTITEM_TOPLUGIN,&PluginDialogItem,CurItem,1);
      Dlg->DlgProc(hDlg,Msg,Param1,(long)&PluginDialogItem);
      Dialog::ConvertItem(CVTITEM_FROMPLUGIN,&PluginDialogItem,CurItem,1);
      return 0;

    case DM_SETREDRAW:
      if(Dlg->CheckDialogMode(DMODE_INITOBJECTS))
        Dlg->Show();
      return 0;

    /* $ 08.09.2000 SVS
      - �᫨ ���⪮, � DM_SETFOCUS �த� ��� � ࠡ�⠫ :-)
    */
    case DM_SETFOCUS:
//      if(!Dialog::IsFocused(Dlg->Item[Param1].Type))
//        return FALSE;
      if(Dlg->ChangeFocus2(Dlg->FocusPos,Param1) == Param1)
      {
        Dlg->ShowDialog();
        return TRUE;
      }
      return FALSE;
    /* SVS $ */

    /* $ 20.10.2000 SVS
      ������� ID 䮪��
    */
    case DM_GETFOCUS:
      return Dlg->FocusPos;
    /* SVS $ */

    case DM_GETTEXT:
      if(Param2) // �᫨ ����� NULL, � �� �� ���� ᯮᮡ ������� ࠧ���
      {
        switch(Type)
        {
          case DI_BUTTON:
            Len=strlen(Ptr);
            if (!(CurItem->Flags & DIF_NOBRACKETS))
            {
              Ptr+=2;
              Len-=4;
            }
            memmove((char *)Param2,Ptr,Len);
            ((char *)Param2)[Len]=0;
            break;

          case DI_USERCONTROL:
          case DI_TEXT:
          case DI_VTEXT:
          case DI_SINGLEBOX:
          case DI_DOUBLEBOX:
          case DI_CHECKBOX:
          case DI_RADIOBUTTON:
            strcpy((char *)Param2,Ptr);
            break;

          case DI_COMBOBOX:
          case DI_EDIT:
          case DI_PSWEDIT:
          case DI_FIXEDIT:
            ((Edit *)(CurItem->ObjPtr))->GetString(Str,sizeof(Str));
            strcpy((char *)Param2,Str);
            break;

          case DI_LISTBOX: // ���� �� �ண��� - �� ॠ�������
            *(char *)Param2='\0';
            break;

          default:  // ���ࠧ㬥����, �� ��⠫���
            *(char *)Param2='\0';
            break;
        }
        return strlen((char *)Param2)+1;
      }
      // ����� ��諥��� �� �⠢�� return, �.�. �⨬ ������� ࠧ���
      // ᫥����⥫쭮 �ࠧ� ������ ��� "case DM_GETTEXTLENGTH"!!!

    case DM_GETTEXTLENGTH:
      Len=strlen(Ptr)+1;
      switch(Type)
      {
        case DI_BUTTON:
          if (!(CurItem->Flags & DIF_NOBRACKETS))
            Len-=4;
          break;

        case DI_USERCONTROL:
        case DI_TEXT:
        case DI_VTEXT:
        case DI_SINGLEBOX:
        case DI_DOUBLEBOX:
        case DI_CHECKBOX:
        case DI_RADIOBUTTON:
          break;

        case DI_COMBOBOX:
        case DI_EDIT:
        case DI_PSWEDIT:
        case DI_FIXEDIT:
          Len=((Edit *)(CurItem->ObjPtr))->GetLength();

        case DI_LISTBOX: // ���� �� �ண��� - �� ॠ�������
          Len=0;
          break;

        default:
          Len=0;
          break;
      }
      return Len;

    case DM_GETDLGITEM:
      if(Param2)
      {
        Dialog::ConvertItem(CVTITEM_TOPLUGIN,(struct FarDialogItem *)Param2,CurItem,1);
/*
        if(IsEdit(Type))
        {
          ((Edit *)(CurItem->ObjPtr))->GetString(Str,sizeof(Str));
          strcpy((char *)Param2,Str);
        }
        else
          strcpy(((struct FarDialogItem *)Param2)->Data,CurItem->Data);
*/
        return TRUE;
      }
      return FALSE;

    case DM_SETDLGITEM:
      if(Param2)
      {
        Dialog::ConvertItem(CVTITEM_FROMPLUGIN,(struct FarDialogItem *)Param2,CurItem,1);
        CurItem->Type=Type;
        // �� ࠧ��, �.�. ����� ����� ���� ��������
        Dlg->InitDialogObjects(Param1);
        if(Dlg->CheckDialogMode(DMODE_SHOW))
        {
          Dlg->ShowDialog(Param1);
          ScrBuf.Flush();
        }
        return TRUE;
      }
      return FALSE;


    /* $ 18.08.2000 SVS
       + ����襭��/����饭�� ���ᮢ�� �������
    */
    case DM_ENABLEREDRAW:
      if(Param1)
        Dlg->IsEnableRedraw++;
      else
        Dlg->IsEnableRedraw--;

      if(!Dlg->IsEnableRedraw)
        if(Dlg->CheckDialogMode(DMODE_INITOBJECTS))
          Dlg->Show();
      return 0;
    /* SVS $ */

    /* $ 23.08.2000 SVS
       + ��������/������ ������.
    */
    case DM_SHOWDIALOG:
//      if(!Dlg->IsEnableRedraw)
      {
        if(Param1)
        {
          if(!Dlg->IsVisible())
            Dlg->Show();
        }
        else
        {
          if(Dlg->IsVisible())
            Dlg->Hide();
        }
      }
      return 0;
    /* SVS $ */

    /* $ 23.08.2000 SVS
       + ��⠭�����/����� ����� �������.
    */
    case DM_SETDLGDATA:
    {
      long PrewDataDialog=Dlg->DataDialog;
      Dlg->DataDialog=Param2;
      return PrewDataDialog;
    }

    case DM_GETDLGDATA:
    {
      return Dlg->DataDialog;
    }
    /* SVS $ */

    /* $ 23.08.2000 SVS
       + ��᫠�� �������(�)
    */
    case DM_KEY:
    {
      int *KeyArray=(int*)Param2;
      Dlg->SetDialogMode(DMODE_KEY);
      for(I=0; I < Param1; ++I)
        Dlg->ProcessKey(KeyArray[I]);
      Dlg->SkipDialogMode(DMODE_KEY);
      return 0;
    }
    /* SVS $ */

    /* $ 23.08.2000 SVS
       + �ਭ㤨⥫쭮 ������� ������
    */
    case DM_CLOSE:
      if(Param1 == -1)
        Dlg->ExitCode=Dlg->FocusPos;
      else
        Dlg->ExitCode=Param1;
      Dlg->EndLoop=TRUE;
      return TRUE;  // ᮣ��ᥭ � �����⨥�
    /* SVS $ */

    /* $ 25.08.2000 SVS
        + ������� ���न���� ����������� ����
    */
    case DM_GETDLGRECT:
    {
      if(Param2)
      {
        int x1,y1,x2,y2;
        Dlg->GetPosition(x1,y1,x2,y2);
        ((SMALL_RECT*)Param2)->Left=x1;
        ((SMALL_RECT*)Param2)->Top=y1;
        ((SMALL_RECT*)Param2)->Right=x2;
        ((SMALL_RECT*)Param2)->Bottom=y2;
        return TRUE;
      }
      return FALSE;
    }
    /* SVS $ */

    /* $ 30.08.2000 SVS
        + �ணࠬ���� ��६�饭�� �������
    */
    case DM_MOVEDIALOG:
    {
      int W1,H1;

      W1=Dlg->X2-Dlg->X1;
      H1=Dlg->Y2-Dlg->Y1;
      // ��࠭���
      Dlg->OldX1=Dlg->X1;
      Dlg->OldY1=Dlg->Y1;
      Dlg->OldX2=Dlg->X2;
      Dlg->OldY2=Dlg->Y2;
      // ��६��⨫�
      if(Param1)   // ��᮫�⭮?
      {
        Dlg->X1=((COORD*)Param2)->X;
        Dlg->Y1=((COORD*)Param2)->Y;
      }
      else         // ����� �⭮�⥫쭮
      {
        Dlg->X1+=((COORD*)Param2)->X;
        Dlg->Y1+=((COORD*)Param2)->Y;
      }

      // �஢�ਫ� � ᪮�४�஢���
      if(Dlg->X1 < 0)         Dlg->X1=0;
      if(Dlg->Y1 < 0)         Dlg->Y1=0;
      if(Dlg->X1+W1 >= ScrX)  Dlg->X1=ScrX-W1; //?
      if(Dlg->Y1+H1 >= ScrY)  Dlg->Y1=ScrY-H1; //?
      Dlg->X2=Dlg->X1+W1;
      Dlg->Y2=Dlg->Y1+H1;
      ((COORD*)Param2)->X=Dlg->X1;
      ((COORD*)Param2)->Y=Dlg->Y1;

      I=Dlg->IsVisible();
      if(I) Dlg->Hide();
      // �ਭ﫨.
      Dlg->AdjustEditPos(Dlg->X1-Dlg->OldX1,Dlg->Y1-Dlg->OldY1);
      if(I) Dlg->Show(); // ⮫쪮 �᫨ ������ �� �����

      return Param2;
    }
    /* SVS $ */

    /* $ 31.08.2000 SVS
        + ��४��祭��/����祭�� ���ﭨ� Enable/Disable �����
    */
    case DM_ENABLE:
    {
      DWORD PrevFlags=CurItem->Flags;
      if(Param2 != -1)
      {
         if(Param2)
           CurItem->Flags&=~DIF_DISABLE;
         else
           CurItem->Flags|=DIF_DISABLE;
      }
      return (PrevFlags&DIF_DISABLE)?FALSE:TRUE;
    }
    /* SVS $ */
  }

  // ��, �� ᠬ� �� ��ࠡ��뢠�� - ���뫠�� �� ��ࠡ��� ��ࠡ��稪�.
  return Dlg->DlgProc(hDlg,Msg,Param1,Param2);
}
/* SVS $ */

//////////////////////////////////////////////////////////////////////////

