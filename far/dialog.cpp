/*
dialog.cpp

����� �������

*/

/* Revision: 1.12 01.08.2000 $ */

/*
Modify:
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

/* Public:
   ��������� ����� Dialog
*/
Dialog::Dialog(struct DialogItem *Item,int ItemCount,
               FARWINDOWPROC DlgProc,long InitParam)
{
  CreateObjects=FALSE;
  InitObjects=FALSE;
  DialogTooLong=FALSE;
  WarningStyle=0;

  if(!DlgProc) // �㭪�� ������ ���� �ᥣ��!!!
    DlgProc=(FARWINDOWPROC)Dialog::DefDlgProc;
  Dialog::DlgProc=DlgProc;

  Dialog::InitParam=InitParam;
  Dialog::Item=Item;
  Dialog::ItemCount=ItemCount;

  Dragged=0;

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


/* Private, Virtual:
   ���樠������ ��ꥪ⮢ � �뢮� ������� �� �࠭.
*/
void Dialog::DisplayObject()
{
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  Shadow();              // "�������" ⥭�

  if (!InitObjects)      // ᠬ�������� ��ਠ��, �����
  {                      //  ������ ���樠���������� �� ��ࢮ� �맮��.
    /* $ 28.07.2000 SVS
       ������ ��楤��, �� � ��� �� ��!
    */
    DlgProc((HANDLE)this,DMSG_INITDIALOG,InitDialogObjects(),InitParam);
    // �� ࠧ��, �.�. ����� ����� ���� ��������
    InitDialogObjects();
    /* SVS $ */
    InitObjects=TRUE;    // �� ��ꥪ�� �ந��樠����஢���!
  }

  ShowDialog();          // "����㥬" ������.
}


/* Public:
   ���樠������ ����⮢ �������.
*/
/* $ 28.07.2000 SVS
   ������ InitDialogObjects �����頥� ID �����
   � 䮪�ᮬ �����
*/
int Dialog::InitDialogObjects()
{
  int I, J, TitleSet;
  int Length,StartX;
  int FocusSet, Type;
  struct DialogItem *CurItem;

  // �।���⥫�� 横� �� ������ ������ � ��������� ���᮫�
  for(I=0, FocusSet=0, TitleSet=0; I < ItemCount; I++)
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
     if(IsFocused(CurItem->Type) && CurItem->Focus)
       FocusSet++;
  }

  // � ⥯��� �� ᭠砫� � �� ������ �ணࠬ��...
  for (I=0; I < ItemCount; I++)
  {
    CurItem=&Item[I];
    Type=CurItem->Type;

    // �᫨ 䮪�� �� ��⠭����� - ������!
    if(!FocusSet && IsFocused(Type))
    {
      FocusSet++;
      CurItem->Focus=1;
    }
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
      if (!CreateObjects)
        CurItem->ObjPtr=new VMenu(NULL,NULL,0,CurItem->Y2-CurItem->Y1+1,
                               VMENU_ALWAYSSCROLLBAR|VMENU_LISTBOX,NULL);

      VMenu *ListBox=(VMenu *)CurItem->ObjPtr;

      if(ListBox)
      {
        // 㤠��� �� �⥬�
        ListBox->DeleteItems();

        struct MenuItem ListItem;
        ListBox->SetFlags(MENU_SHOWAMPERSAND);
        ListBox->SetPosition(X1+CurItem->X1,Y1+CurItem->Y1,
                             X1+CurItem->X2,Y1+CurItem->Y2);
        ListBox->SetBoxType(SHORT_SINGLE_BOX);

        struct FarListItems *List=CurItem->ListItems;
        if(List && List->Items)
        {
          struct FarListItem *Items=List->Items;
          for (J=0; J < List->CountItems; J++)
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
      if (!CreateObjects)
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
      DialogEdit->SetPosition(X1+CurItem->X1,Y1+CurItem->Y1,
                              X1+CurItem->X2,Y1+CurItem->Y2);
      DialogEdit->SetObjectColor(
         FarColorToReal(WarningStyle ? COL_WARNDIALOGEDIT:COL_DIALOGEDIT),
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
        // �᫨ DI_FIXEDIT, � ����� �ࠧ� �⠢���� �� ������...
        //   ��-�� - �뫮 �����㬥��஢���� :-)
        DialogEdit->SetMaxLength(CurItem->X2-CurItem->X1+1);
        DialogEdit->SetOvertypeMode(TRUE);
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
        struct FarListItem *ListItems=
                   ((struct FarListItems *)CurItem->ListItems)->Items;
        int Length=((struct FarListItems *)CurItem->ListItems)->CountItems;

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

  /* $ 28.07.2000 SVS
     - �᫨ ������ �� ���⠢�� �� ������ ������ � 䮪�ᮬ,
       � �ਤ���� ᠬ��� �� �⮬ �����������, � ���⠢���
       䮪�� �� ��ࢮ� �ࠧ㬨⥫쭮� ����� ;-)
  */
  for (I=0;I<ItemCount;I++)
    if (Item[I].Focus)
      break;
  if(I == ItemCount)
    I=ChangeFocus(0,1,0);
  /* SVS $ */

  // �� ��ꥪ�� ᮧ����
  CreateObjects=TRUE;
  return I;
}


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

/* Private:
   ���ᮢ�� ����⮢ ������� �� �࠭�.
*/
void Dialog::ShowDialog()
{
  struct DialogItem *CurItem;
  int X,Y;
  int I;
  unsigned long Attr;

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  /* $ 28.07.2000 SVS
     ��। ���ᮢ��� ������� ���뫠�� ᮮ�饭�� � ��ࠡ��稪
  */
  DlgProc((HANDLE)this,DMSG_PAINT,0,0);
  /* SVS $ */

  /* $ 28.07.2000 SVS
     ��। ���ᮢ��� �������� ���� �������...
  */
  Attr=DlgProc((HANDLE)this,DMSG_CTLCOLORDIALOG,0,WarningStyle ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT);
  SetScreen(X1,Y1,X2,Y2,' ',Attr);
  /* SVS $ */

  for (I=0; I < ItemCount; I++)
  {
    CurItem=&Item[I];

    /* $ 28.07.2000 SVS
       ��। ���ᮢ��� ������� ����� ���뫠�� ᮮ�饭��
       ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
    */
    Dialog::SendDlgMessage((HANDLE)this,DMSG_DRAWITEM,I,0);
    /* SVS $ */
    /* $ 28.07.2000 SVS
       ��। ���ᮢ��� ������� ����� ������� ���᭨� ������ ���ᮢ��
    */
    switch(CurItem->Type)
    {
      case DI_SINGLEBOX:
      case DI_DOUBLEBOX:
        Attr=MAKELONG(
          MAKEWORD(FarColorToReal(WarningStyle ? COL_WARNDIALOGBOXTITLE:COL_DIALOGBOXTITLE), // Title LOBYTE
                 FarColorToReal(WarningStyle ? COL_WARNDIALOGHIGHLIGHTTEXT:COL_DIALOGHIGHLIGHTTEXT)),// HiText HIBYTE
          MAKEWORD(FarColorToReal(WarningStyle ? COL_WARNDIALOGBOX:COL_DIALOGBOX), // Box LOBYTE
                 0)                                               // HIBYTE
        );
        Attr=DlgProc((HANDLE)this,DMSG_CTLCOLORDLGITEM,I,Attr);

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

      case DI_TEXT:
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
            Attr=WarningStyle ? COL_WARNDIALOGBOX:COL_DIALOGBOX;
          else
            Attr=WarningStyle ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT;

        Attr=MAKELONG(
           MAKEWORD(FarColorToReal(Attr),
                   FarColorToReal(WarningStyle ? COL_WARNDIALOGHIGHLIGHTTEXT:COL_DIALOGHIGHLIGHTTEXT)), // HIBYTE HiText
             0);
        Attr=DlgProc((HANDLE)this,DMSG_CTLCOLORDLGITEM,I,Attr);
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

      case DI_VTEXT:
        if (CurItem->Flags & DIF_BOXCOLOR)
          Attr=WarningStyle ? COL_WARNDIALOGBOX:COL_DIALOGBOX;
        else
          if (CurItem->Flags & DIF_SETCOLOR)
            Attr=(CurItem->Flags & DIF_COLORMASK);
          else
            Attr=(WarningStyle ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT);

        Attr=DlgProc((HANDLE)this,DMSG_CTLCOLORDLGITEM,I,FarColorToReal(Attr));
        SetColor(Attr&0xFF);
        GotoXY(X1+CurItem->X1,Y1+CurItem->Y1);
        VText(CurItem->Data);
        break;

      /* $ 18.07.2000 SVS
         + ��ࠡ�⪠ ����� DI_COMBOBOX
      */
      case DI_EDIT:
      case DI_FIXEDIT:
      case DI_PSWEDIT:
      case DI_COMBOBOX:
      {
        Edit *EditPtr=(Edit *)(CurItem->ObjPtr);

        Attr=EditPtr->GetObjectColor();
        Attr=MAKEWORD(FarColorToReal(Attr&0xFF),FarColorToReal(HIWORD(Attr)));
        Attr=MAKELONG(Attr, // EditLine (Lo=Color, Hi=Selected)
           MAKEWORD(FarColorToReal(EditPtr->GetObjectColorUnChanged()), // EditLine - UnChanched Color
           FarColorToReal(COL_DIALOGTEXT) // HistoryLetter
           ));
        Attr=DlgProc((HANDLE)this,DMSG_CTLCOLORDLGITEM,I,Attr);

        EditPtr->SetObjectColor(Attr&0xFF,HIBYTE(LOWORD(Attr)),LOBYTE(HIWORD(Attr)));

        if (CurItem->Focus)
        {
          SetCursorType(1,-1);
          EditPtr->Show();
        }
        else
          EditPtr->FastShow();

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

      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      case DI_LISTBOX:
      {
        VMenu *ListBox=(VMenu *)(CurItem->ObjPtr);
        if(ListBox)
        {
          if (CurItem->Focus)
            ListBox->Show();
          else
            ListBox->FastShow();
        }
        break;
      }
      /* 01.08.2000 SVS $ */

      case DI_CHECKBOX:
      case DI_RADIOBUTTON:
        if (CurItem->Flags & DIF_SETCOLOR)
          Attr=(CurItem->Flags & DIF_COLORMASK);
        else
          Attr=(WarningStyle ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT);

        Attr=MAKEWORD(FarColorToReal(Attr),
             FarColorToReal(WarningStyle ? COL_WARNDIALOGHIGHLIGHTTEXT:COL_DIALOGHIGHLIGHTTEXT)); // HiText
        Attr=DlgProc((HANDLE)this,DMSG_CTLCOLORDLGITEM,I,Attr);

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
          SetCursorType(1,-1);
          MoveCursor(X1+CurItem->X1+1,Y1+CurItem->Y1);
        }

        break;

      case DI_BUTTON:
        GotoXY(X1+CurItem->X1,Y1+CurItem->Y1);

        if (CurItem->Focus)
        {
          SetCursorType(0,10);
          Attr=MAKEWORD(
             FarColorToReal(WarningStyle ? COL_WARNDIALOGSELECTEDBUTTON:COL_DIALOGSELECTEDBUTTON), // TEXT
             FarColorToReal(WarningStyle ? COL_WARNDIALOGHIGHLIGHTSELECTEDBUTTON:COL_DIALOGHIGHLIGHTSELECTEDBUTTON)); // HiText
        }
        else
        {
          Attr=MAKEWORD(
             FarColorToReal(WarningStyle ? COL_WARNDIALOGBUTTON:COL_DIALOGBUTTON), // TEXT
             FarColorToReal(WarningStyle ? COL_WARNDIALOGHIGHLIGHTBUTTON:COL_DIALOGHIGHLIGHTBUTTON)); // HiText
        }
        Attr=DlgProc((HANDLE)this,DMSG_CTLCOLORDLGITEM,I,Attr);
        SetColor(Attr&0xFF);
        HiText(CurItem->Data,HIBYTE(LOWORD(Attr)));
        break;

    } // end switch(...
    /* 28.07.2000 SVS $ */
  } // end for (I=...

  /* $ 31.07.2000 SVS
     ����稬 �������� ��६�饭��...
  */
  if ( Dragged ) // �᫨ ������ �᪠����
  {
    Text(0,0,0xCE,"Move");
  }
  /* SVS $ */
}

/* Public, Virtual:
   ��ࠡ�⪠ ������ �� ����������.
   ��४�뢠�� BaseInput::ProcessKey.
*/
int Dialog::ProcessKey(int Key)
{
  int FocusPos=0,I,J;
  char Str[1024];
  char *PtrStr;
  Edit *CurEditLine;

  /* $ 31.07.2000 tran
     + ��६�饭�� ������� �� �࠭� */
  if ( Dragged ) // �᫨ ������ �᪠����
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
            Show();
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
            Show();
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
            Show();
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
            Show();
            break;
        case KEY_ENTER:
        case KEY_CTRLF5:
            Dragged=0;
            Show();
            PutText(0,0,3,0,LV);
            break;
        case KEY_ESC:
            Hide();
            AdjustEditPos(OldX1-X1,OldY1-Y1);
            X1=OldX1;
            X2=OldX2;
            Y1=OldY1;
            Y2=OldY2;
            Show();
            PutText(0,0,3,0,LV);
            Dragged=0;
            break;
    }
    return (TRUE);
  }

  if (Key == KEY_CTRLF5)
  {
    // ����砥� 䫠� � ���������� ���न����
    Dragged=1;
    OldX1=X1; OldX2=X2; OldY1=Y1; OldY2=Y2;
    GetText(0,0,3,0,LV);
    Show();
    return (TRUE);
  }
  /* tran 31.07.2000 $ */

  if (Key==KEY_NONE || Key==KEY_IDLE)
  {
    /* $ 28.07.2000 SVS
       ��।���� ��� 䠪� � ��ࠡ��稪 :-)
    */
    DlgProc((HANDLE)this,DMSG_ENTERIDLE,0,0);
    /* SVS $ */
    return(FALSE);
  }

  for (I=0;I<ItemCount;I++)
    if (Item[I].Focus)
    {
      FocusPos=I;
      break;
    }

  int Type=Item[FocusPos].Type;

  switch(Key)
  {
    case KEY_F1:
      /* $ 28.07.2000 SVS
         ��। �뢮��� ������� ���뫠�� ᮮ�饭�� � ��ࠡ��稪
         � �᫨ ���㫨 �� ����, � �뢮��� ���᪠���
      */
      PtrStr=(char*)DlgProc((HANDLE)this,DMSG_HELP,FocusPos,(long)&HelpTopic[0]);
      if(PtrStr && *PtrStr)
      {
        SetHelp(PtrStr);
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
          Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,I-1,0);
          Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,I,0);
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
      EndLoop=TRUE;
      if (Type==DI_BUTTON)
      {
        Item[FocusPos].Selected=1;
        ExitCode=FocusPos;
      }
      else
        for (I=0;I<ItemCount;I++)
          if (Item[I].DefaultButton)
          {
            if (!IsEdit(Item[I].Type))
              Item[I].Selected=1;
            ExitCode=I;
          }
      if (ExitCode==-1)
        ExitCode=FocusPos;
      return(TRUE);

    case KEY_ESC:
    case KEY_BREAK:
    case KEY_F10:
      EndLoop=TRUE;
      ExitCode=(Key==KEY_BREAK) ? -2:-1;
      return(TRUE);

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
        Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,FocusPos,0);
        /* SVS $ */
        ShowDialog();
        return(TRUE);
      }
      if (Type==DI_RADIOBUTTON)
      {
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
            Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,I,0);
          ++I;
          /* SVS $ */
        } while (I<ItemCount && Item[I].Type==DI_RADIOBUTTON && (Item[I].Flags & DIF_GROUP)==0);
        Item[FocusPos].Selected=1;
        /* $ 28.07.2000 SVS
          �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
          ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
        */
        Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,FocusPos,0);
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
        Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,FocusPos,0);
        /* SVS $ */
        return(TRUE);
      }
      return(TRUE);

    case KEY_HOME:
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
          Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,FocusPos,0);
          Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,I,0);
          /* SVS $ */
          ShowDialog();
          return(TRUE);
        }
      return(TRUE);

    case KEY_LEFT:
    case KEY_RIGHT:
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
      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
    case KEY_PGDN:
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
      /* $ 01.08.2000 SVS
         ����� ListBox
      */
      if(Type == DI_LISTBOX)
      {
        ((VMenu *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* SVS $ */
      if (IsEdit(Type))
      {
        if (Item[FocusPos].Flags & DIF_EDITOR)
          switch(Key)
          {
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
              Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,FocusPos,0);
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
                Edit *edt=(Edit *)Item[FocusPos].ObjPtr;
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
                  Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,FocusPos,0);
                  /* SVS $ */
                  ShowDialog();
                  return(TRUE);
                }
                else if (CurPos>=Length)
                {
                  Edit *edt_1=(Edit *)Item[FocusPos+1].ObjPtr;
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
        if (((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key))
        {
          /* $ 26.07.2000 SVS
             AutoComplite: �᫨ ��⠭����� DIF_HISTORY
                 � ࠧ�襭� ��⮧����襭��!.
          */
          if(Opt.AutoComplete && Key < 256 && Key != KEY_BS && Key != KEY_DEL &&
             ((Item[FocusPos].Flags & DIF_HISTORY) || Type == DI_COMBOBOX)
            )
          {
            Edit *cb=((Edit *)(Item[FocusPos].ObjPtr));
            int SelStart, SelEnd, IX;

            /* $ 01.08.2000 SVS
               ������让 ���祪 � AutoComplete
            */
            int CurPos=cb->GetCurPos();
            /* SVS $*/
            //text to search for
            cb->GetString(Str,sizeof(Str));
            cb->GetSelection(SelStart,SelEnd);
            if(SelStart <= 0)
              SelStart=sizeof(Str);
            else
              SelStart++;

            SelEnd=strlen(Str);
            //find the string in the list
            if (FindInEditForAC(Type == DI_COMBOBOX,(void *)Item[FocusPos].Selected,Str))
            {
              cb->SetString(Str);
              cb->Select(SelEnd,sizeof(Str)); //select the appropriate text
              /* $ 01.08.2000 SVS
                 ������让 ���祪 � AutoComplete
              */
              cb->SetCurPos(CurPos); // SelEnd
              /* SVS $*/
              /* $ 28.07.2000 SVS
                �� ��������� ���ﭨ� ������� ����� ���뫠�� ᮮ�饭��
                ���।�⢮� �㭪樨 SendDlgMessage - � ��� �������� ��!
              */
              Dialog::SendDlgMessage((HANDLE)this,DMSG_CHANGEITEM,FocusPos,0);
              /* SVS $ */
              Redraw();
            }
          }
          /* SVS $ */
          return(TRUE);
        }
      }

      if (ProcessHighlighting(Key,FocusPos,FALSE))
        return(TRUE);

      return(ProcessHighlighting(Key,FocusPos,TRUE));
  }
}


/* Public, Virtual:
   ��ࠡ�⪠ ������ �� "���".
   ��४�뢠�� BaseInput::ProcessMouse.
*/
int Dialog::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  int FocusPos=0,I;
  int MsX,MsY;
  int Type;

  if (MouseEvent->dwButtonState==0)
    return(FALSE);

  for (I=0;I<ItemCount;I++)
    if (Item[I].Focus)
    {
      FocusPos=I;
      break;
    }

  MsX=MouseEvent->dwMousePosition.X;
  MsY=MouseEvent->dwMousePosition.Y;
  if (MsX<X1 || MsY<Y1 || MsX>X2 || MsY>Y2)
  {
    if (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
      ProcessKey(KEY_ESC);
    else
      ProcessKey(KEY_ENTER);
    return(TRUE);
  }

  if (MouseEvent->dwEventFlags==0 && (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
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
          Edit *EditLine=(Edit *)(Item[I].ObjPtr);
          if (EditLine->ProcessMouse(MouseEvent))
          {
            EditLine->SetClearFlag(0);
            ChangeFocus2(FocusPos,I);
            ShowDialog();
            return(TRUE);
          }
          else
          {
            int EditX1,EditY1,EditX2,EditY2;
            EditLine->GetPosition(EditX1,EditY1,EditX2,EditY2);
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
    // ��� MOUSE-��६�頫��:
    //   � �������� � ⮬ ��砥, �᫨ ���� �� ������ �� ��⨢�� ������
    //
    }
  }
  return(FALSE);
}


/* Private:
   ������� 䮪�� ����� (�������⢨� �����蠬�
     KEY_TAB, KEY_SHIFTTAB, KEY_UP, KEY_DOWN,
   � ⠪ �� Alt-HotKey)
*/
/* $ 28.07.2000 SVS
   ����᮪ ��� ᮮ�饭�� DMSG_KILLFOCUS & DMSG_SETFOCUS
*/
int Dialog::ChangeFocus(int FocusPos,int Step,int SkipGroup)
{
  int Type,OrigFocusPos=FocusPos;
  int FucusPosNeed=-1;
  // � �㭪�� ��ࠡ�⪨ ������� ����� ��।��� ᮮ�饭��,
  //   �� ����� - LostFocus() - ���� 䮪�� �����.
  if(InitObjects)
    FucusPosNeed=DlgProc((HANDLE)this,DMSG_KILLFOCUS,FocusPos,0);
  if(FucusPosNeed != -1 && IsFocused(Item[FucusPosNeed].Type))
    FocusPos=FucusPosNeed;
  else
  {
    while (1)
    {
      FocusPos+=Step;
      if (FocusPos>=ItemCount)
        FocusPos=0;
      if (FocusPos<0)
        FocusPos=ItemCount-1;

      Type=Item[FocusPos].Type;

      if (Type==DI_LISTBOX || Type==DI_BUTTON || Type==DI_CHECKBOX || IsEdit(Type))
        break;
      if (Type==DI_RADIOBUTTON && (!SkipGroup || Item[FocusPos].Selected))
        break;

      // 㡨ࠥ� ��横������� � ��᫥���騬 �����ᠭ��� :-)
      if(OrigFocusPos == FocusPos)
        break;
    }
  }

  // � �㭪�� ��ࠡ�⪨ ������� ����� ��।��� ᮮ�饭��,
  //   �� ����� GotFocus() - ����稫 䮪�� �����.
  // ������㥬 �����頥��� �㭪樥� ������� ���祭��
  if(InitObjects)
    DlgProc((HANDLE)this,DMSG_GOTFOCUS,FocusPos,0);
  return(FocusPos);
}

/* $ 28.07.2000 SVS
   Private:
   ������� 䮪�� ����� ����� ���� ����⠬�.
   �뭥ᥭ �⤥�쭮 � ⥬, �⮡� ��ࠡ���� DMSG_KILLFOCUS & DMSG_SETFOCUS
*/
int Dialog::ChangeFocus2(int KillFocusPos,int SetFocusPos)
{
  int FucusPosNeed=-1;
  if(InitObjects)
    FucusPosNeed=DlgProc((HANDLE)this,DMSG_KILLFOCUS,KillFocusPos,0);
  if(FucusPosNeed != -1 && IsFocused(Item[FucusPosNeed].Type))
    SetFocusPos=FucusPosNeed;

  Item[KillFocusPos].Focus=0;
  Item[SetFocusPos].Focus=1;

  if(InitObjects)
    DlgProc((HANDLE)this,DMSG_GOTFOCUS,SetFocusPos,0);
  return(SetFocusPos);
}
/* SVS $ */
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
  if(!FromPlugin)
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

/* $ 28.07.2000 SVS
   �㭪��, ��।������ - "����� �� ����� ������� ����� 䮪�� �����"
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
         Type==DI_LISTBOX);
}
/* SVS $ */

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
    struct FarListItem *ListItems=((struct FarListItems *)HistoryName)->Items;
    int Count=((struct FarListItems *)HistoryName)->CountItems;

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
    if(DlgProc((HANDLE)this,DMSG_CTLCOLORDLGLIST,
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


/* Public, Static:

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


/* Private:

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
        if(!DlgProc((HANDLE)this,DMSG_HOTKEY,I,Key))
          break; // ᪠���� �� �த������ ��ࠡ���...
        /* SVS $ */

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
        Item[FocusPos].Focus=0;
        Item[I].Focus=1;
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
        ShowDialog();
        return(TRUE);
      }
  }
  return(FALSE);
}

/* Private:
   ������塞 �믠���騩 ᯨ᮪ ��� ComboBox
*/
/*
   $ 18.07.2000 SVS
   �㭪��-��ࠡ��稪 �롮� �� ᯨ᪠ � ��⠭����...
*/
void Dialog::SelectFromComboBox(
         Edit *EditLine,                   // ��ப� ।���஢����
         struct FarListItems *List,    // ᯨ᮪ ��ப
         char *IStr)
{
  char Str[512];
  struct MenuItem ComboBoxItem;
  struct FarListItem *ListItems=List->Items;
  int EditX1,EditY1,EditX2,EditY2;
  int I,Dest;
  {
    // ᮧ����� ���⮣� ���⨪��쭮�� ����
    //  � ��易⥫�� ������� ScrollBar
    VMenu ComboBoxMenu("",NULL,0,8,VMENU_ALWAYSSCROLLBAR);

    EditLine->GetPosition(EditX1,EditY1,EditX2,EditY2);
    if (EditX2-EditX1<20)
      EditX2=EditX1+20;
    if (EditX2>ScrX)
      EditX2=ScrX;
    ComboBoxMenu.SetFlags(MENU_SHOWAMPERSAND);
    ComboBoxMenu.SetPosition(EditX1,EditY1+1,EditX2,0);
    ComboBoxMenu.SetBoxType(SHORT_SINGLE_BOX);

    // ���������� �㭪⮢ ����
    /* ��᫥���� �㭪� ᯨ᪠ - ��࠭���⥫� - � ��� Tetx[0]
       ������ ���� ࠢ�� '\0'
    */
    for (Dest=I=0;I < List->CountItems;I++)
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
    if(DlgProc((HANDLE)this,DMSG_CTLCOLORDLGLIST,
                    sizeof(Colors)/sizeof(Colors[0]),(long)Colors))
      ComboBoxMenu.SetColors(Colors);
    /* SVS $ */

    ComboBoxMenu.Show();
    while (!ComboBoxMenu.Done())
    {
      int Key=ComboBoxMenu.ReadInput();
      // ����� ����� �������� ��-� ᢮�, ���ਬ��,
      //  ��ࠡ��� multiselect ComboBox
      ComboBoxMenu.ProcessInput();
    }

    int ExitCode=ComboBoxMenu.GetExitCode();
    if (ExitCode<0)
      return;
    ComboBoxMenu.GetUserData(Str,sizeof(Str),ExitCode);
    /* �������� ⥪�饥 ���ﭨ� */
    for (I=0;ListItems[I].Text[0];I++)
      ListItems[I].Flags&=~LIF_SELECTED;
    ListItems[ComboBoxMenu.GetSelectPos()].Flags|=LIF_SELECTED;
  }
  EditLine->SetString(Str);
  EditLine->SetLeftPos(0);
  Redraw();
}
/* SVS $ */

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
    /* ����饭�� DMSG_INITDIALOG ���뫠���� ��楤�� ��ࠡ�⪨ �������
       ��᫥ ⮣�, ��� �뫨 �ந��樠����஢��� �� �ࠢ���騥 ������
       �������, �� �� ⮣�, ��� ��� �⠫� ������. � �⢥� �� ������ ᮮ�饭��
       ��楤�� ��ࠡ�⪨ ������� ���樠������� ����� ����� � �����஥
       ���४⭮� ��砫쭮 ���ﭨ�. ���ਬ��, ��� ����� ��������� ����
       ᯨ᪠ ����⠬�, ����� ��⮬ ��ᬮ��� ���짮��⥫�...
       Param1 = ID �����, ����� ������ 䮪�� ����� �� 㬮�砭��.
       Param2 = ������᪨� �����, ��।���� �㭪樨 DialogEx.
       Return = TRUE -  �᫨ ��楤�� ��ࠡ�⪨ ������� ᠬ� ��⠭�����
                        䮪�� ����� �� ������� �����.
                FALSE - �㤥� ��⠭����� 䮪�� ����� �� �����, 㪠�����
                        � Param1
    */
    case DMSG_INITDIALOG:
      return TRUE;

    case DMSG_CLOSE:
      return TRUE;

    /* ����饭�� DMSG_KILLFOCUS ��।����� ��楤�� ��ࠡ�⪨ �������
       �����।�⢥��� ��। ���३ ��������୮�� 䮪�� ����⮬
       �������.
       Param1 = ID �����, ����饣� 䮪�� �����.
       Param2 = 0
       Return = -1 - "�����ᥭ � ���३ 䮪��".
               >=0 - ����� ��������� �����, ���஬� �⨬ ��।��� 䮪��.
    */
    case DMSG_KILLFOCUS:
      return -1;

    /* ����饭�� DMSG_GOTFOCUS ���뫠���� ��楤�� ��ࠡ�⪨ �������
       ��᫥ ⮣�, ��� ����� ������� ����稫 ���������� 䮪�� �����.
       �� ���� ���䨪�樮���� ᮮ�饭��!
       Param1 = ID ����� ����稢襣� 䮪�� �����.
       Param2 = 0
       Return = 0
    */
    case DMSG_GOTFOCUS:
      return 0;

    /* ����饭�� DMSG_HELP ��।����� � ��ࠡ��稪 ������� ��। �뢮���
       ⥬� �����. �� ᮮ�饭�� �������� �ࠢ���� ������� ⥬� �����
       �� �஢�� �⤥�쭮�� ����� �������.
       Param1 = ID ����� �������, ����騩 䮪�� ����� (⥪�騩 �����)
       Param2 = ���� ��ப� ⥬� ���᪠���, �易���� � ����� ��������,
                ����� �।���������� ��������.
       Return = ���� ��ப� ⥬� ���᪠���, �易���� � ����� ��������,
                ����� �㤥� �뢥���.
                �᫨ ���㫨 NULL, � ⥬� ����� �뢮������ �� �㤥�.
    */
    case DMSG_HELP:
      return Param2;

    /* ����饭�� DMSG_PAINT ���뫠���� ��। ���ᮢ��� �ᥣ� �������.
       Param1 = 0
       Param2 = 0
       Return = 0
    */
    case DMSG_PAINT:
      return 0;

    /* ����饭�� DMSG_CTLCOLORDIALOG ���뫠���� � �㭪�� ��ࠡ�⪨ �������
       ��। ���ᮢ��� �������� ���� �������.
       ����饭�� ��室�� �ࠧ� ��᫥ DMSG_PAINT.
       Param1 = 0
       Param2 = ��ਡ��� (梥�_䮭�+梥�_⥪��), � �ᯮ�짮������ ������
                ��ࠡ��稪 ������� ��� ���ᮢ��� �������� �������.
       Return = ��ਡ��� (梥�_䮭�+梥�_⥪��), � �ᯮ�짮������ ������
                ��ࠡ��稪 ������� ������ ���ᮢ��� �������� �������.
    */
    case DMSG_CTLCOLORDIALOG:
      return Param2;

    /* ����饭�� DMSG_CTLCOLORDLGITEM ���뫠���� ��楤�� ��ࠡ�⪨ �������
       ��। ���ᮢ��� �����⭮�� ����� �������. � �⢥� �� �� ᮮ�饭��
       ��楤�� ��ࠡ�⪨ ������� ����� ��⠭����� ᢮� ��ਡ��� (梥� ⥪��
       � 䮭�) ��� ��������� �����.
       Param1 = ID ������, ����� �㤥� ���ᮢ��.
       Param2 = ��ਡ��� (梥�_䮭�+梥�_⥪��), � �ᯮ�짮������ ������
                ��ࠡ��稪 ������� ��� ���ᮢ��� �����.
       Return = ��ਡ��� (梥�_䮭�+梥�_⥪��), � �ᯮ�짮������ ������
                �㤥� ���ᮢ�� �����.
    */
    case DMSG_CTLCOLORDLGITEM:
      return Param2;

    /* ����饭�� DMSG_CTLCOLORDLGLIST ���뫠���� ��楤�� ��ࠡ�⪨ �������
       ��। ���ᮢ��� ᯨ᪠ (DI_COMBOBOX, DI_LISTBOX, DIF_HISTORY). � �⢥�
       �� �� ᮮ�饭�� ��楤�� ��ࠡ�⪨ ������� ����� ��⠭����� ᢮�
       ��ਡ��� (梥� ⥪�� � 䮭�) ��� ��������� �����.
       Param1 = ������⢮ ����⮢ ��।�������� ���ᨢ� ��ਡ�⮢.
       Param2 = �����⥫� �� ���ᨢ ��ਡ�⮢ (梥�_䮭�+梥�_⥪��), �
                �ᯮ�짮������ ������ ��ࠡ��稪 ������� ��� ���ᮢ���
                ᯨ᮪:
                ListColorBody=0,      // ��������
                ListColorBox=1,       // ࠬ��
                ListColorTitle=2,     // ��������� - ���孨� � ������
                ListColorText=3,      // ����� �㭪�
                ListColorHilite=4,    // HotKey
                ListColorSeparator=5, // separator
                ListColorSelected=6,  // ��࠭��
                ListColorHSelect=7,   // ��࠭�� - HotKey
                ListColorScrollBar=8  // ScrollBar
       Return = TRUE - �᫨ ��ਡ��� ��������
                FALSE - ��⠢��� �� ��� ����.
    */
    case DMSG_CTLCOLORDLGLIST:
      return FALSE;

    /* ����饭�� DMSG_ENTERIDLE ���뫠���� � ��楤��� ��ࠡ�⪨ �������,
       ����� �室�� � 宫��⮥ ���ﭨ�. ���������� ���� �室�� �
       ���ﭨ� ��������, ����� ��� ������� ᮮ�饭��.
       Param1 = 0
       Param2 = 0
       Return = 0
    */
    case DMSG_ENTERIDLE:
      return 0;
  }

  // �।���⥫쭮 �஢�ਬ...
  if(Param1 >= Dlg->ItemCount)
    return 0;

  CurItem=&Dlg->Item[Param1];
  Type=CurItem->Type;

  Ptr=CurItem->Data;

  switch(Msg)
  {
    /* ����饭�� DMSG_DRAWITEM ���뫠���� ��ࠡ��稪� ������� ��। ���ᮢ���
       ����� �������.
       Param1 = ID ����� �������, ����� �㤥� ���ᮢ��.
       Param2 = �����⥫� �� �������� FarDialogItem, ����뢠���� ����� ���
                ���ᮢ��.
       Return = 0
    */
    case DMSG_DRAWITEM:
      return 0;

    /* ����饭�� DMSG_HOTKEY ���뫠���� � ��ࠡ��稪 �������, �����
       ���짮��⥫� ����� Alt-�㪢�.
       Param1 = ID ����� �������, �� ����� ����� �롮�.
       Param2 = ����७��� ��� ������.
       Return = TRUE - ᮣ��ᥭ - �த������ ��ࠡ���
                FALSE - �� �த������ ����� �����.
    */
    case DMSG_HOTKEY:
      return TRUE;

    /* ����饭�� DMSG_CHANGEITEM �����頥� ��ࠡ��稪 �� ��������� ���ﭨ�
       ����� ������� - ����� ᨬ��� � ���� ।���஢����, ��४��稫�
       CheckBox (RadioButton),...
       Param1 = ID ����� �������.
       Param2 = �����⥫� �� �������� FarDialogItem, ����뢠���� ����� ���
                ���ᮢ��.
       Return = TRUE - ࠧ���� ��������� (� ᮮ⢥��⢥��� ���ᮢ�� ⮣�,
                �� ���짮��⥫� ���)
                FALSE - ������� ���������.
    */
    case DMSG_CHANGEITEM:
      return TRUE;


    /* ����饭�� LMSG_CHANGELIST �����頥� ��ࠡ��稪 �� ��������� ���ﭨ�
       ����� ᯨ᪠ (DI_COMBOBOX, DI_LISTBOX, DIF_HISTORY).
       Param1 = ID ����� (DI_COMBOBOX, DI_LISTBOX, DIF_HISTORY).
       Param2 = �����⥫� �� �������� FarListItems, ����뢠����
                ��������� �����.
       Return = TRUE - ࠧ���� ��������� (� ᮮ⢥��⢥��� ���ᮢ�� ⮣�,
                �� ���짮��⥫� ���)
                FALSE - ������� ���������.
       ����� �� ᮮ�襭�� �ନ��� ����� � �㭪樨 �������!
    */
    case DMSG_CHANGELIST:
      return TRUE;
  }

  return 0;
}
/* SVS $ */

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

  CurItem=&Dlg->Item[Param1];
  Type=CurItem->Type;
  Ptr=CurItem->Data;

  switch(Msg)
  {
    /* ����饭�� DMSG_SETTEXT ���뫠���� �⠭���⭮�� ��ࠡ��稪� ������� ���
       ��⠭���� ��ப� ����� ��� ��������� ����⮢ DI_CHECKBOX, DI_TEXT,...
       � ����� ���祭��.
       Param1 = ID �ॡ㥬��� ����� �������.
       Param2 = ���� ��ப�, ᮤ�ঠ饩 ⥪��. ���祭�� �⮣� ��ࠬ��� ࠢ���
                NULL ����������.
       Return = ������ ��⠭�������� ������ +1 ��� ᨬ���� "����� ��ப�" ('\0')
    */
    case DMSG_SETTEXT:
      if(Param2) // �᫨ ����� NULL, � �� �� ���� ᯮᮡ ������� ࠧ���
      {
        switch(Type)
        {
          case DI_BUTTON:
          case DI_TEXT:
          case DI_VTEXT:
          case DI_SINGLEBOX:
          case DI_DOUBLEBOX:
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
        Dlg->InitDialogObjects(); // ��२��樠�����㥬 ������ �������
        return strlen((char *)Param2)+1;
      }
      return 0;

    /* ����饭�� DMSG_SETTEXTLENGTH ���뫠���� �⠭���⭮�� ��ࠡ��稪�
       ������� ��� ��⠭���� ���ᨬ��쭮�� ࠧ��� ।����㥬�� ��ப�
       ��ப�...
       Param1 = ID ����� ������� (���������� ⮫쪮 �� DI_COMBOBOX -
                ��� 䫠�� DIF_DROPDOWNLIST, DI_EDIT, DI_PSWEDIT, DI_FIXEDIT)
       Param2 = �ॡ㥬� ࠧ���
       Return = �।��饥 ���祭�� ࠧ��� ।����㥬�� ��ப�
                ��� 0 � ��砥 �訡��.
    */
    case DMSG_SETTEXTLENGTH:
      if(IsEdit(Type) && !(CurItem->Flags & DIF_DROPDOWNLIST))
      {
        Param1=((Edit *)(CurItem->ObjPtr))->GetMaxLength();
        ((Edit *)(CurItem->ObjPtr))->SetMaxLength(Param2);
        Dlg->InitDialogObjects(); // ��२��樠�����㥬 ������ �������
        return Param1;
      }
      return 0;

    /* ����饭�� LMSG_CHANGELIST �����頥� ��ࠡ��稪 �� ��������� ���ﭨ�
       ����� ᯨ᪠ (DI_COMBOBOX, DI_LISTBOX, DIF_HISTORY).
       Param1 = ID ����� (DI_COMBOBOX, DI_LISTBOX, DIF_HISTORY).
       Param2 = �����⥫� �� �������� FarListItems, ����뢠����
                ��������� �����.
       Return = TRUE - ࠧ���� ��������� (� ᮮ⢥��⢥��� ���ᮢ�� ⮣�,
                �� ���짮��⥫� ���)
                FALSE - ������� ���������.
       ����� �� ᮮ�襭�� �ନ��� ����� � �㭪樨 �������!
    */
    case DMSG_CHANGELIST:
      return Dlg->DlgProc(hDlg,Msg,Param1,(long)&CurItem->ListItems);

    /* ����饭�� DMSG_CHANGEITEM �����頥� ��ࠡ��稪 �� ��������� ���ﭨ�
       ����� ������� - ����� ᨬ��� � ���� ।���஢����, ��४��稫�
       CheckBox (RadioButton),...
       Param1 = ID ����� �������.
       Param2 = �����⥫� �� �������� FarDialogItem, ����뢠���� ���������
                �����.
       Return = TRUE - ࠧ���� ��������� (� ᮮ⢥��⢥��� ���ᮢ�� ⮣�,
                �� ���짮��⥫� ���)
                FALSE - ������� ���������.
       ����� �� ᮮ�襭�� �ନ��� ����� � �㭪樨 �������!
    */
    case DMSG_CHANGEITEM:
      // �८�ࠧ㥬 ����� ���!
      Dialog::ConvertItem(0,&PluginDialogItem,CurItem,1);
      I=Dlg->DlgProc(hDlg,Msg,Param1,(long)&PluginDialogItem);
      Dialog::ConvertItem(1,&PluginDialogItem,CurItem,1);
      return I;

    /* ����饭�� DMSG_DRAWITEM ���뫠���� ��ࠡ��稪� ������� ��। ���ᮢ���
       ����� �������.
       Param1 = ID ����� �������, ����� �㤥� ���ᮢ��.
       Param2 = �����⥫� �� �������� FarDialogItem, ����뢠���� ����� ���
                ���ᮢ��.
       Return = 0
       ����� �� ᮮ�襭�� �ନ��� ����� � �㭪樨 �������!
    */
    case DMSG_DRAWITEM:
      // �८�ࠧ㥬 ����� ���!
      Dialog::ConvertItem(0,&PluginDialogItem,CurItem,1);
      Dlg->DlgProc(hDlg,Msg,Param1,(long)&PluginDialogItem);
      Dialog::ConvertItem(1,&PluginDialogItem,CurItem,1);
      return 0;

    /* ������ ���뫠�� ᮮ�饭�� DMSG_SETREDRAW �⠭���⭮�� ��ࠡ��稪�
       ������� ��� ����ᮢ�� �� ��������� ����.
       Param1 = 0
       Param2 = 0
       Return = 0
    */
    case DMSG_SETREDRAW:
      if(Dlg->InitObjects)
        Dlg->Show();
      return 0;

    /* ����饭�� DMSG_SETFOCUS ��⠭�������� ���������� 䮪�� �� �������
       ����� �������.
       Param1 = ID �����, ����� ������ ������� 䮪�� �����.
       Param2 = 0
       Return = FALSE - �� ����� �� ����� ����� 䮪�� �����
                TRUE  - 䮪�� ����� �ᯥ譮 ��⠭����� (� �����મ�!)
    */
    case DMSG_SETFOCUS:
      if(!Dialog::IsFocused(Type))
        return FALSE;
      Dlg->ChangeFocus(Param1,1,0);
      return TRUE;

    /* ����饭�� DMSG_GETTEXT �������� ������� ᮤ�ন��� ��ப ����� ���
       ���������� ����⮢ DI_CHECKBOX, DI_TEXT,...
       Param1 = ID �ॡ㥬��� �����
       Param2 = ����⥫� �� ��ப� �ਥ����.
                �᫨ Param2 = NULL - �� �� ���� ᯮᮡ ������� ࠧ���
                ������ (�. DMSG_GETTEXTLENGTH)
       Return = ������ ������ +1 ��� ᨬ���� "����� ��ப�" ('\0')
    */
    case DMSG_GETTEXT:
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
      // ᫥����⥫쭮 �ࠧ� ������ ��� "case DMSG_GETTEXTLENGTH"!!!

    /* ����饭�� DMSG_GETTEXTLENGTH ���뫠���� �⠭���⭮�� ��ࠡ��稪� ������� ��� ����祭�� ࠧ��� ��ப�...
       Param1 = ID �ॡ㥬��� �����
       Param2 = 0
       Return = ������ ������ (� ��⮬ ����筮�� ���)
    */
    case DMSG_GETTEXTLENGTH:
      Len=strlen(Ptr)+1;
      switch(Type)
      {
        case DI_BUTTON:
          if (!(CurItem->Flags & DIF_NOBRACKETS))
            Len-=4;
          break;

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

    /* ����饭�� DMSG_GETDLGITEM ���뫠���� �⠭���⭮�� ��ࠡ��稪�
       ������� ��� ����祭�� ������ ���ଠ樨 � �������� �����.
       Param1 = ID ����� �������
       Param2 = �����⥫� �� �������� FarDialogItem
       Return = TRUE - ����� ᪮��஢���!
    */
    case DMSG_GETDLGITEM:
      if(Param2)
      {
        Dialog::ConvertItem(0,(struct FarDialogItem *)Param2,CurItem,1);
        if(IsEdit(Type))
        {
          ((Edit *)(CurItem->ObjPtr))->GetString(Str,sizeof(Str));
          strcpy((char *)Param2,Str);
        }
        else
          strcpy(((struct FarDialogItem *)Param2)->Data,CurItem->Data);
        return TRUE;
      }
      return FALSE;

    /* ����饭�� DMSG_SETDLGITEM ���뫠���� �⠭���⭮�� ��ࠡ��稪� �������
       ��� ��������� ���ଠ樨 � �������� �����.
       Param1 = ID ����� �������
       Param2 = �����⥫� �� �������� FarDialogItem
       Return = TRUE - ����� ��ࠡ�⠭�!
    */
    case DMSG_SETDLGITEM:
      if(Param2)
      {
        Dialog::ConvertItem(TRUE,(struct FarDialogItem *)Param2,CurItem,1);
        CurItem->Type=Type;
        // �� ࠧ��, �.�. ����� ����� ���� ��������
        Dlg->InitDialogObjects();
        return TRUE;
      }
      return FALSE;
  }

  // ��, �� ᠬ� �� ��ࠡ��뢠�� - ���뫠�� �� ��ࠡ��� ��ࠡ��稪�.
  return Dlg->DlgProc(hDlg,Msg,Param1,Param2);
}
/* SVS $ */


/* $ 31.07.2000 tran
   + �㭪�� ���ࠢ������� ���न��� edit ����ᮢ */
void Dialog::AdjustEditPos(int dx, int dy)
{
  struct DialogItem *CurItem;
  int I;
  int x1,x2,y1,y2;

  Edit *DialogEdit;
  for (I=0; I < ItemCount; I++)
  {
    CurItem=&Item[I];
    if (IsEdit(CurItem->Type))
    {
       DialogEdit=(Edit *)CurItem->ObjPtr;
       DialogEdit->GetPosition(x1,y1,x2,y2);
       x1+=dx;
       x2+=dx;
       y1+=dy;
       y2+=dy;
       DialogEdit->SetPosition(x1,y1,x2,y2);
    }
  }
}
/* tran 31.07.2000 $ */
