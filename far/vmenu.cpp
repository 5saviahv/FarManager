/*
vmenu.cpp

������� ������������ ����
  � ��� ��:
    * ������ � DI_COMBOBOX
    * ...
*/

/* Revision: 1.29 03.06.2001 $ */

/*
Modify:
  03.06.2001 SVS
    ! ��������� � ����� � ���������� UserData � VMenu
    + GetPosition() - ���������� �������� ������� �����.
    + GetUserDataSize() - �������� ������ ������
    + SetUserData() - ������������� ������ � ������ ����
    ! GetUserData() - ���������� ��������� �� ���� ������
  30.05.2001 OT
    - �������� � ���������� VMenu. � ����� ����� Frame *FrameFromLaunched
      ������������ ��� �����, ������ ��� ���� �����������.
      ����� ����� �� �� ���������������, ����� ��� �� ������ :)
  25.05.2001 DJ
    + SetColor()
  23.05.2001 SVS
    - �������� � �������� ��������� � ���� (Part II) - �� ���� �������������
      AmpPos ��� ���������� �������, �� ��� � ������� ����� ������� �
      ��������... �����.
  23.05.2001 SVS
    - �������� � �������� ��������� � ����.
  22.05.2001 SVS
    ! �� ������� �������� �� ������� ������� '&', ��� ������ ������ ���
      ����������.
  21.05.2001 DJ
    - ������ ������� �������� � VMenu::ChangeFlags()
  21.05.2001 SVS
    ! VMENU_DRAWBACKGROUND -> VMENU_DISABLEDRAWBACKGROUND
    ! MENU_* ��������
    ! DialogStyle -> VMENU_WARNDIALOG
    ! struct MenuData
      ���� Selected, Checked � Separator ������������� � DWORD Flags
    ! struct MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
  18.05.2001 SVS
    ! UpdateRequired -> VMENU_UPDATEREQUIRED
    ! DrawBackground -> VMENU_DRAWBACKGROUND
    ! WrapMode -> VMENU_WRAPMODE
    ! ShowAmpersand -> VMENU_SHOWAMPERSAND
    + ������� InsertItem(), FindItem(), UpdateRequired(), GetVMenuInfo()
  17.05.2001 SVS
    + UpdateItem() - �������� �����
    + FarList2MenuItem() \ ������� �������������� "����-����"
    + MenuItem2FarList() /
    ! ��������� ������ ������ ��� ������ - ��� ���������� ������������ ������
    ! ��� �������������� ����� ���� ����, ��� ������ ������� �������� �� ��������
  15.05.2001 KM
    - �� ������� ���� DIF_CHECKED � DI_LISTBOX
    + ��������� ����������� ��������� � DI_LISTBOX � ������ DIF_CHECKED
      ������������ ������ (� ������� ����� Flags), ������� �����
      �������������� � �������� Check mark, ��������� �������:
      MacroMenuItems[i].Flags|=(S[0]=='~')?LIF_CHECKED|'-':0;
  12.05.2001 SVS
    + AddItem(char *NewStrItem);
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
    + AddItem, ���������� ���, ��� �������� ���� struct FarList - ���
      ���������� ���� � �������� :-)
    + SortItems() - ����� �� - ��� ��������
    * ������� ��� ������������� �������� ��� GetItemPtr() � ����� ������
      �������� ������� - Item
  06.05.2001 DJ
    ! �������� #include
  27.04.2001 VVM
    + ��������� KEY_MSWHEEL_XXXX
    + � ���� ������� ������� ������ ���������� ������� �����
  09.04.2001 SVS
    ! ��������� �� ��������� ���������
  20.02.2001 SVS
    + ��������� ������� SetSelectPos() - ����������� ������ � ������
      Disabled & Separator
    ! ��������� � ���������� � "�������" :-) � ������ �������� SetSelectPos()
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
    ! ������������� �������� ����������� ���������� - ������� ��������� �
      ������, ����� ������� � ����������� �����
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  11.12.2000 tran
    + ��������� ����� �� ������ ������� ����
  20.09.2000 SVS
    + ������� GetItemPtr - �������� ��������� �� ������ Item.
  29.08.2000 tran 1.09
    - BUG � �� ������� \0 � ����� ������ � GetUserData
  01.08.2000 SVS
    + � ShowMenu �������� ��������, ���������� - ������� �� �������
      �������������� ��� �� ������ ������� ;-)
    - Bug � ������������, ���� �������� NULL ��� Title
    ! ListBoxControl -> VMFlags
    + ������� �������� N ������� ����
    + ������� ��������� ���� (�� ���������)
    + ������� ������� ��������� ����
    ! ������� ����� ������������ ��� �������� �������-����������� � ��������!
  28.07.2000 SVS
    + ��������� �������� �������� (� ����������) � �������, ��������� �
      ����������:
      SetColors();
      GetColors();
  23.07.2000 SVS
    + ���� ������� � ���������� :-)
    ! AlwaysScrollBar ������� �� ListBoxControl
    ! ���� �������� ������ ��� ����, ��� ListBoxControl ��� �������
  18.07.2000 SVS
    ! ������� ����� ������������ (�������� isAlwaysScrollBar) � ������
      ������������� scrollbar � DI_COMBOBOX (� � ������� - DI_LISTBOX)
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  06.07.2000 tran
    + mouse support for menu scrollbar
  29.06.2000 SVS
    ! ���������� ScrollBar � ���� ���� �������� ����� ShowMenuScrollbar
  28.06.2000 tran
    + ������������ �������� � ���� ��� �������������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "vmenu.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "keys.hpp"
#include "colors.hpp"
#include "chgprior.hpp"
#include "dialog.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "manager.hpp"

/* $ 18.07.2000 SVS
   ! ������� ����� ������������ (isListBoxControl) � ������ �������������
     scrollbar � DI_LISTBOX & DI_COMBOBOX
*/
VMenu::VMenu(char *Title,       // ��������� ����
             struct MenuData *Data, // ������ ����
             int ItemCount,     // ���������� ������� ����
             int MaxHeight,     // ������������ ������
             DWORD Flags,       // ����� ScrollBar?
             FARWINDOWPROC Proc,    // ����������
             Dialog *ParentDialog)  // �������� ��� ListBox
{
  int I;
  VMenu::VMFlags=Flags;
/* SVS $ */

/*& 28.05.2001 OT ��������� ����������� ����� �� ����� ������� ���� */
  FrameFromLaunched=FrameManager->GetCurrentFrame();
  FrameFromLaunched->LockRefresh();
/* OT &*/

  VMenu::ParentDialog=ParentDialog;
  VMFlags|=VMENU_UPDATEREQUIRED|VMENU_WRAPMODE;
  VMFlags&=~VMENU_SHOWAMPERSAND;
  CallCount=0;
  TopPos=0;
  SaveScr=NULL;

  if(!Proc) // ������� ������ ���� ������!!!
    Proc=(FARWINDOWPROC)VMenu::DefMenuProc;
  VMenuProc=Proc;

  if (Title!=NULL)
    strcpy(VMenu::Title,Title);
  else
    *VMenu::Title=0;

  *BottomTitle=0;
  VMenu::Item=NULL;
  VMenu::ItemCount=0;

  for (I=0; I < ItemCount; I++)
  {
    struct MenuItem NewItem;
    memset(&NewItem,0,sizeof(NewItem));
    if ((unsigned int)Data[I].Name < MAX_MSG)
      strcpy(NewItem.Name,MSG((unsigned int)Data[I].Name));
    else
      strcpy(NewItem.Name,Data[I].Name);
    NewItem.AmpPos=-1;
    NewItem.Flags=Data[I].Flags;
    AddItem(&NewItem);
  }

  VMenu::MaxHeight=MaxHeight;
  BoxType=DOUBLE_BOX;
  /* $ 01.08.2000 SVS
   - Bug � ������������, ���� �������� NULL ��� Title
  */
  MaxLength=strlen(VMenu::Title)+2;
  /* SVS $ */
  for (SelectPos=0,I=0;I<ItemCount;I++)
  {
    int Length=strlen(Item[I].Name);
    if (Length>MaxLength)
      MaxLength=Length;
    if (Item[I].Flags&LIF_SELECTED)
      SelectPos=I;
  }
  /* $ 28.07.2000 SVS
     ��������� ���� �� ���������
  */
  SetColors(NULL);
  /* SVS $*/
  if (!(VMenu::VMFlags&VMENU_LISTBOX) && CtrlObject!=NULL)
  {
    PrevMacroMode=CtrlObject->Macro.GetMode();
    if (PrevMacroMode!=MACRO_MAINMENU)
      CtrlObject->Macro.SetMode(MACRO_OTHER);
  }
}


VMenu::~VMenu()
{
  if (!(VMenu::VMFlags&VMENU_LISTBOX) && CtrlObject!=NULL)
    CtrlObject->Macro.SetMode(PrevMacroMode);
  Hide();
  DeleteItems();
/*& 28.05.2001 OT ��������� ����������� ������, � ������� ����������� ��� ���� */
  FrameFromLaunched->UnlockRefresh();
/* OT &*/
}


void VMenu::DeleteItems()
{
  /* $ 13.07.2000 SVS
     �� ��� �� ������� ������ ������ ����� new :-)
  */
  if(Item)
  {
    for(int I=0; I < ItemCount; ++I)
      if(Item[I].UserDataSize > sizeof(Item[I].UserData) &&
         Item[I].UserData &&
         !(Item[I].Flags&LIF_PTRDATA))
        free(Item[I].UserData);
    free(Item);
  }
  /* SVS $ */
  Item=NULL;
  ItemCount=0;
}


void VMenu::Hide()
{
  while (CallCount>0)
    Sleep(10);
  CallCount++;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  if(!(VMenu::VMFlags&VMENU_LISTBOX) && SaveScr)
  {
    delete SaveScr;
    SaveScr=NULL;
    ScreenObject::Hide();
  }

  VMFlags|=VMENU_UPDATEREQUIRED;
  CallCount--;
}


void VMenu::Show()
{
  while (CallCount>0)
    Sleep(10);
  CallCount++;
  if(VMenu::VMFlags&VMENU_LISTBOX)
    BoxType=VMenu::VMFlags&VMENU_SHOWNOBOX?NO_BOX:SHORT_DOUBLE_BOX;

  int AutoCenter=FALSE,AutoHeight=FALSE;
  if (X1==-1)
  {
    X1=(ScrX-MaxLength-4)/2;
    AutoCenter=TRUE;
  }
  if (X1<2)
    X1=2;
  if (X2<=0)
    X2=X1+MaxLength+4;
  if (!AutoCenter && X2>ScrX-4+2*(BoxType==SHORT_DOUBLE_BOX || BoxType==SHORT_SINGLE_BOX))
  {
    X1+=ScrX-4-X2;
    X2=ScrX-4;
    if (X1<2)
      X1=2;
  }
  if (X2>ScrX-2)
    X2=ScrX-2;
  if (Y1==-1)
  {
    if (MaxHeight!=0 && MaxHeight<ItemCount)
      Y1=(ScrY-MaxHeight-2)/2;
    else
      if ((Y1=(ScrY-ItemCount-2)/2)<0)
        Y1=0;
    AutoHeight=TRUE;
  }
  if (Y2<=0)
    if (MaxHeight!=0 && MaxHeight<ItemCount)
      Y2=Y1+MaxHeight+1;
    else
      Y2=Y1+ItemCount+1;
  if (Y2>ScrY)
    Y2=ScrY;
  if (AutoHeight && Y1<3 && Y2>ScrY-3)
  {
    Y1=2;
    Y2=ScrY-2;
  }
  if (X2>X1 && Y2>Y1)
  {
//_SVS(SysLog("VMenu::Show()"));
    if(!(VMenu::VMFlags&VMENU_LISTBOX))
      ScreenObject::Show();
    else
    {
      VMFlags|=VMENU_UPDATEREQUIRED;
      DisplayObject();
    }
  }
  CallCount--;
}

/* $ 28.07.2000 SVS
   ����������� ������� � ������ VMenu::Colors[] -
      �������� ��������� �� VMenu::Colors[]
*/
void VMenu::DisplayObject()
{
//_SVS(SysLog("VMFlags&VMENU_UPDATEREQUIRED=%d",VMFlags&VMENU_UPDATEREQUIRED));
  if (!(VMFlags&VMENU_UPDATEREQUIRED))
    return;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  VMFlags&=~VMENU_UPDATEREQUIRED;
  ExitCode=-1;

  if (!(VMenu::VMFlags&VMENU_LISTBOX) && SaveScr==NULL)
  {
    if (!(VMFlags&VMENU_DISABLEDRAWBACKGROUND) && !(BoxType==SHORT_DOUBLE_BOX || BoxType==SHORT_SINGLE_BOX))
      SaveScr=new SaveScreen(X1-2,Y1-1,X2+4,Y2+2);
    else
      SaveScr=new SaveScreen(X1,Y1,X2+2,Y2+1);
  }
  if (!(VMFlags&VMENU_DISABLEDRAWBACKGROUND))
  {
    /* $ 23.07.2000 SVS
       ���� ��� ListBox �������
    */
    if (BoxType==SHORT_DOUBLE_BOX || BoxType==SHORT_SINGLE_BOX)
    {
      Box(X1,Y1,X2,Y2,VMenu::Colors[1],BoxType);
      if(!(VMenu::VMFlags&(VMENU_LISTBOX|VMENU_ALWAYSSCROLLBAR)))
      {
        MakeShadow(X1+2,Y2+1,X2+1,Y2+1);
        MakeShadow(X2+1,Y1+1,X2+2,Y2+1);
      }
    }
    else
    {
      SetScreen(X1-2,Y1-1,X2+2,Y2+1,' ',VMenu::Colors[0]);
      if(!(VMenu::VMFlags&(VMENU_LISTBOX|VMENU_ALWAYSSCROLLBAR)))
      {
        MakeShadow(X1,Y2+2,X2+3,Y2+2);
        MakeShadow(X2+3,Y1,X2+4,Y2+2);
      }
      if (BoxType!=NO_BOX)
        Box(X1,Y1,X2,Y2,VMenu::Colors[1],BoxType);
    }
    /* SVS $*/
  }
  if(!(VMenu::VMFlags&VMENU_LISTBOX))
  {
    if (*Title)
    {
      GotoXY(X1+(X2-X1-1-strlen(Title))/2,Y1);
      SetColor(VMenu::Colors[2]);
      mprintf(" %s ",Title);
    }
    if (*BottomTitle)
    {
      GotoXY(X1+(X2-X1-1-strlen(BottomTitle))/2,Y2);
      SetColor(VMenu::Colors[2]);
      mprintf(" %s ",BottomTitle);
    }
  }
  SetCursorType(0,10);
  ShowMenu(TRUE);
}
/* SVS $ */


/* $ 28.07.2000 SVS
   ����������� ������� � ������ VMenu::Colors[] -
      �������� ��������� �� VMenu::Colors[]
*/
void VMenu::ShowMenu(int IsParent)
{
  char TmpStr[1024];
//_SVS(SysLog("VMenu::ShowMenu()"));
  unsigned char BoxChar[2],BoxChar2[2];
  int Y,I;
  if (ItemCount==0 || X2<=X1 || Y2<=Y1)
    return;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  BoxChar2[1]=BoxChar[1]=0;

  if(!IsParent && (VMenu::VMFlags&VMENU_LISTBOX))
  {
    BoxType=VMenu::VMFlags&VMENU_SHOWNOBOX?NO_BOX:SHORT_SINGLE_BOX;
    SetScreen(X1,Y1,X2,Y2,' ',VMenu::Colors[0]);
    if (BoxType!=NO_BOX)
      Box(X1,Y1,X2,Y2,VMenu::Colors[1],BoxType);
  }

  switch(BoxType)
  {
    case NO_BOX:
      *BoxChar=' ';
      break;
    case SINGLE_BOX:
    case SHORT_SINGLE_BOX:
      *BoxChar=0x0B3;
      break;
    case DOUBLE_BOX:
    case SHORT_DOUBLE_BOX:
      *BoxChar=0x0BA;
      break;
  }
  if (SelectPos<ItemCount)
    Item[SelectPos].Flags|=LIF_SELECTED;
  if (SelectPos>TopPos+Y2-Y1-2)
    TopPos=SelectPos-(Y2-Y1-2);
  if (SelectPos<TopPos)
    TopPos=SelectPos;
  for (Y=Y1+1,I=TopPos;Y<Y2;Y++,I++)
  {
    GotoXY(X1,Y);
    if (I<ItemCount)
      if (Item[I].Flags&LIF_SEPARATOR)
      {
        int SepWidth=X2-X1+1;
        char *Ptr=TmpStr+1;
        MakeSeparator(SepWidth,TmpStr,
          BoxType==NO_BOX?0:(BoxType==SINGLE_BOX||BoxType==SHORT_SINGLE_BOX?2:1));

        if (I>0 && I<ItemCount-1 && SepWidth>3)
          for (unsigned int J=0;Ptr[J+3]!=0;J++)
          {
            if (Item[I-1].Name[J]==0)
              break;
            if (Item[I-1].Name[J]==0x0B3)
            {
              int Correction=0;
              if (!(VMFlags&VMENU_SHOWAMPERSAND) && memchr(Item[I-1].Name,'&',J)!=NULL)
                Correction=1;
              if (strlen(Item[I+1].Name)>=J && Item[I+1].Name[J]==0x0B3)
                Ptr[J-Correction+2]=0x0C5;
              else
                Ptr[J-Correction+2]=0x0C1;
            }
          }
        Text(X1,Y,VMenu::Colors[1],TmpStr);
      }
      else
      {
        SetColor(VMenu::Colors[1]);
        Text((char*)BoxChar);
        GotoXY(X2,Y);
        Text((char*)BoxChar);
        if (Item[I].Flags&LIF_SELECTED)
          SetColor(VMenu::Colors[6]);
        else
          SetColor(VMenu::Colors[(Item[I].Flags&LIF_DISABLE?9:3)]);
        GotoXY(X1+1,Y);
        char Check=' ';
        if (Item[I].Flags&LIF_CHECKED)
          if (!(Item[I].Flags&0x0000FFFF))
            Check=0x0FB;
          else
            Check=(char)Item[I].Flags&0x0000FFFF;

        sprintf(TmpStr,"%c %.*s",Check,X2-X1-3,Item[I].Name);
        { // ��������� ������ ������ ��� ������!!!
          // ��� ���������� ������������ ������!!!
          char *TabPtr;
          while ((TabPtr=strchr(TmpStr,'\t'))!=NULL)
            *TabPtr=' ';
        }
        int Col;

        if(!(Item[I].Flags&LIF_DISABLE))
        {
          if (Item[I].Flags&LIF_SELECTED)
              Col=VMenu::Colors[7];
          else
              Col=VMenu::Colors[4];
        }
        else
          Col=VMenu::Colors[9];
        if(VMFlags&VMENU_SHOWAMPERSAND)
          Text(TmpStr);
        else
        {
          short AmpPos=Item[I].AmpPos+2;
//_SVS(SysLog(">>> AmpPos=%d (%d) TmpStr='%s'",AmpPos,Item[I].AmpPos,TmpStr));
          if(AmpPos >= 2 && TmpStr[AmpPos] != '&')
          {
            memmove(TmpStr+AmpPos+1,TmpStr+AmpPos,strlen(TmpStr+AmpPos)+1);
            TmpStr[AmpPos]='&';
          }
//_SVS(SysLog("<<< AmpPos=%d TmpStr='%s'",AmpPos,TmpStr));
          HiText(TmpStr,Col);
        }

        mprintf("%*s",X2-WhereX(),"");
      }
    else
    {
      SetColor(VMenu::Colors[1]);
      Text((char*)BoxChar);
      GotoXY(X2,Y);
      Text((char*)BoxChar);
      GotoXY(X1+1,Y);
      SetColor(VMenu::Colors[3]);
      mprintf("%*s",X2-X1-1,"");
    }
  }
  /* $ 28.06.2000 tran
       ���������� �������� ���� ������� � ���� ������ ���
       ��� ������
     $ 29.06.2000 SVS
       ���������� ScrollBar � ���� ���� �������� ����� Opt.ShowMenuScrollbar
     $ 18.07.2000 SVS
       + ������ ������� scrollbar ��� DI_LISTBOX & DI_COMBOBOX � �����������
         ��� ������������� ����
  */
  if (((VMenu::VMFlags&(VMENU_LISTBOX|VMENU_ALWAYSSCROLLBAR)) ||
       Opt.ShowMenuScrollbar) &&
      (Y2-Y1-1)<ItemCount)
  {
    SetColor(VMenu::Colors[8]);
    ScrollBar(X2,Y1+1,Y2-Y1-1,SelectPos,ItemCount);
  }
  /* 18.07.2000 SVS $ */
  /* SVS $ */
  /* tran $ */
}
/* 28.07.2000 SVS $ */

BOOL VMenu::UpdateRequired(void)
{
  return (ItemCount>=TopPos && ItemCount<TopPos+Y2-Y1);
}


int VMenu::InsertItem(struct FarListInsert *NewItem)
{
  if(NewItem)
  {
    struct MenuItem MItem;
    return AddItem(FarList2MenuItem(NewItem->Item,&MItem),NewItem->Index);
  }
  return -1;
}

int VMenu::AddItem(struct MenuItem *NewItem,int PosAdd)
{
  while (CallCount>0)
    Sleep(10);
  CallCount++;

  struct MenuItem *NewPtr;
  int Length;

  if((DWORD)PosAdd >= (DWORD)ItemCount)
    PosAdd=ItemCount;
  if (UpdateRequired())
    VMFlags|=VMENU_UPDATEREQUIRED;

  if ((ItemCount & 255)==0)
  {
    if ((NewPtr=(struct MenuItem *)realloc(Item,sizeof(struct MenuItem)*(ItemCount+256+1)))==NULL)
      return(0);
    Item=NewPtr;
  }
  if(PosAdd < ItemCount)
  {
    memmove(Item+PosAdd+1,Item+PosAdd,sizeof(struct MenuItem)*(ItemCount-PosAdd)); //??
  }
  Item[PosAdd]=*NewItem;
  Length=strlen(Item[PosAdd].Name);
  if (Length>MaxLength)
    MaxLength=Length;
  if (Item[PosAdd].Flags&LIF_SELECTED)
    SelectPos=PosAdd;
  if(Item[PosAdd].Flags&0x0000FFFF)
  {
    if((Item[PosAdd].Flags&0x0000FFFF) == 1)
      Item[PosAdd].Flags&=~0x0000FFFF;
    Item[PosAdd].Flags|=LIF_CHECKED;
  }
  Item[PosAdd].AmpPos=-1;
  {
    char *ChPtr=strchr(Item[PosAdd].Name,'&');
    if (ChPtr!=NULL)// && !(VMFlags&VMENU_SHOWAMPERSAND))
      Item[PosAdd].AmpPos=ChPtr-Item[PosAdd].Name;
  }

  if(VMFlags&(VMENU_AUTOHIGHLIGHT|VMENU_REVERSIHLIGHT))
    AssignHighlights(VMFlags&VMENU_REVERSIHLIGHT);
//  if(VMenu::VMFlags&VMENU_LISTBOXSORT)
//    SortItems(0);
  CallCount--;
  return(ItemCount++);
}

int  VMenu::AddItem(char *NewStrItem)
{
  struct FarList FarList0;
  struct FarListItem FarListItem0;
  int LenNewStrItem=0;
  if(NewStrItem)
    LenNewStrItem=strlen(NewStrItem);
  if(LenNewStrItem >= sizeof(FarListItem0.Text))
  {
    FarListItem0.Flags=LIF_PTRDATA;
    FarListItem0.Ptr.PtrLength=LenNewStrItem;
    FarListItem0.Ptr.PtrData=NewStrItem;
  }
  else
  {
    FarListItem0.Flags=0;
    if(!LenNewStrItem || NewStrItem[0] == 0x1)
    {
      FarListItem0.Flags=LIF_SEPARATOR;
      FarListItem0.Text[0]=0;
    }
    else
      strcpy(FarListItem0.Text,NewStrItem);
  }
  FarList0.ItemsNumber=1;
  FarList0.Items=&FarListItem0;
  return VMenu::AddItem(&FarList0);
}

int VMenu::AddItem(struct FarList *List)
{
  if(List && List->Items)
  {
    struct MenuItem MItem;
    struct FarListItem *FItem=List->Items;
    for (int J=0; J < List->ItemsNumber; J++, ++FItem)
      AddItem(FarList2MenuItem(FItem,&MItem));
  }
  return ItemCount;
}

int VMenu::UpdateItem(struct FarList *NewItem)
{
  if(NewItem && NewItem->Items && (DWORD)NewItem->ItemsNumber < (DWORD)ItemCount)
  {
    struct MenuItem MItem;
    memcpy(Item+NewItem->ItemsNumber,FarList2MenuItem(NewItem->Items,&MItem),sizeof(struct MenuItem));
    return TRUE;
  }
  return FALSE;
}

struct FarListItem *VMenu::MenuItem2FarList(struct MenuItem *MItem,
                                            struct FarListItem *FItem)
{
  if(FItem && MItem)
  {
    memset(FItem,0,sizeof(struct FarListItem));
    FItem->Flags|=MItem->Flags;

    if(!(MItem->Flags&LIF_PTRDATA)) // != LIF_PTRDATA
    {
      strncpy(FItem->Text,MItem->Name,sizeof(FItem->Text)); // MItem->UserData?
      // ��������� �� &
//      short AmpPos=MItem->AmpPos;
//      if(AmpPos >= 0)
//        memmove(FItem->Text+AmpPos,FItem->Text+AmpPos+1,strlen(FItem->Text+AmpPos+1));
    }
    else
    {
      // ����� ����� �������� �������� �� LIF_PTRDATA!!!
      FItem->Ptr.PtrLength=MItem->UserDataSize;
      FItem->Ptr.PtrData=MItem->UserData;
      FItem->Flags|=LIF_PTRDATA;
    }
    return FItem;
  }
  return NULL;
}

struct MenuItem *VMenu::FarList2MenuItem(struct FarListItem *FItem,
                                         struct MenuItem *MItem)
{
  if(FItem && MItem)
  {
    memset(MItem,0,sizeof(struct MenuItem));
    MItem->Flags=FItem->Flags;
    if(FItem->Flags&LIF_PTRDATA)
    {
      memcpy(MItem->Name,FItem->Ptr.PtrData,sizeof(MItem->Name));
      MItem->UserDataSize=FItem->Ptr.PtrLength;
      MItem->UserData=FItem->Ptr.PtrData;
    }
    else
      strncpy(MItem->Name,FItem->Text,sizeof(MItem->Name));
    // � ����� ���� ��������� AmpPos????
    return MItem;
  }
  return NULL;
}

/* $ 01.08.2000 SVS
   ������� �������� N ������� ����
*/
int VMenu::DeleteItem(int ID,int Count)
{
  if(ID > 0)
  {
    if(ID >= ItemCount)
      return ItemCount;
    if(ID+Count >= ItemCount)
      Count=ItemCount-ID; //???
  }
  else // ���� ID < 0, �� �������������, ��� ���� ������� � ����� ������
  {
    if(ItemCount+ID < 0)
      return ItemCount;
    else
    {
      ID+=ItemCount;
      if(ID+Count >= ItemCount)
        Count=ItemCount-ID; //???
    }
  }
  if(Count <= 0)
    return ItemCount;

  while (CallCount>0)
    Sleep(10);
  CallCount++;


  if ((ID >= TopPos && ID < TopPos+Y2-Y1) ||
      (ID+Count >= TopPos && ID+Count < TopPos+Y2-Y1) //???
     )
    VMFlags|=VMENU_UPDATEREQUIRED;

  // ������� ������� ������, ���� ������ �� ������ �� ����
  for(int I=0; I < Count; ++I)
  {
    struct MenuItem *PtrItem=Item+ID+I;
    if(PtrItem->UserData && !(PtrItem->Flags&LIF_PTRDATA))
        free(PtrItem->UserData);
  }
  // � ��� ������ �����������
  memmove(Item+ID,Item+ID+Count,sizeof(struct MenuItem)*Count); //???

  if (Item[ID].Flags&LIF_SELECTED)
  {
    if(ID == ItemCount)
      SelectPos--;
  }

  ItemCount-=Count;

  CallCount--;
  return(ItemCount);
}
/* SVS $ */


int VMenu::ProcessKey(int Key)
{
  int I;

  if (Key==KEY_NONE || Key==KEY_IDLE)
    return(FALSE);

  VMFlags|=VMENU_UPDATEREQUIRED;
  if (ItemCount==0)
    if (Key!=KEY_F1 && Key!=KEY_F10 && Key!=KEY_ESC)
    {
      ExitCode=-1;
      return(FALSE);
    }

  while (CallCount>0)
    Sleep(10);
  CallCount++;

  switch(Key)
  {
    case KEY_F1:
      if(VMenu::ParentDialog)
        VMenu::ParentDialog->ProcessKey(Key);
      else
        ShowHelp();
      break;
    case KEY_ENTER:
      if(VMenu::ParentDialog)
        VMenu::ParentDialog->ProcessKey(Key);
      else
      {
        EndLoop=TRUE;
        ExitCode=SelectPos;
      }
      break;
    case KEY_ESC:
    case KEY_F10:
      if(VMenu::ParentDialog)
        VMenu::ParentDialog->ProcessKey(Key);
      else
      {
        EndLoop=TRUE;
        ExitCode=-1;
      }
      break;
    case KEY_HOME:
    case KEY_CTRLHOME:
    case KEY_CTRLPGUP:
      SelectPos=SetSelectPos(0,1);
      ShowMenu(TRUE);
      break;
    case KEY_END:
    case KEY_CTRLEND:
    case KEY_CTRLPGDN:
      SelectPos=SetSelectPos(ItemCount-1,-1);
      ShowMenu(TRUE);
      break;
    case KEY_PGUP:
      if((I=SelectPos-(Y2-Y1-1)) < 0)
        I=0;
      SelectPos=SetSelectPos(I,1);
      ShowMenu(TRUE);
      break;
    case KEY_PGDN:
      if((I=SelectPos+(Y2-Y1-1)) >= ItemCount)
        I=ItemCount-1;
      SelectPos=SetSelectPos(I,-1);
      ShowMenu(TRUE);
      break;
    /* $ 27.04.2001 VVM
      + ��������� KEY_MSWHEEL_XXXX */
    case KEY_MSWHEEL_UP:
    /* VVM $ */
    case KEY_LEFT:
    case KEY_UP:
      SelectPos=SetSelectPos(SelectPos-1,-1);
      ShowMenu(TRUE);
      break;
    /* $ 27.04.2001 VVM
      + ��������� KEY_MSWHEEL_XXXX */
    case KEY_MSWHEEL_DOWN:
    /* VVM $ */
    case KEY_RIGHT:
    case KEY_DOWN:
      SelectPos=SetSelectPos(SelectPos+1,1);
      ShowMenu(TRUE);
      break;
    case KEY_TAB:
    case KEY_SHIFTTAB:
      if(VMenu::ParentDialog)
      {
        VMenu::ParentDialog->ProcessKey(Key);
        break;
      }
    default:
      for (I=0;I<ItemCount;I++)
        if (Dialog::IsKeyHighlighted(Item[I].Name,Key,FALSE,Item[I].AmpPos))
        {
          Item[SelectPos].Flags&=~LIF_SELECTED;
          Item[I].Flags|=LIF_SELECTED;
          SelectPos=I;
          ShowMenu(TRUE);
          if(!VMenu::ParentDialog)
          {
            ExitCode=I;
            EndLoop=TRUE;
          }
          break;
        }
      if (!EndLoop)
        for (I=0;I<ItemCount;I++)
          if (Dialog::IsKeyHighlighted(Item[I].Name,Key,TRUE,Item[I].AmpPos))
            {
              Item[SelectPos].Flags&=~LIF_SELECTED;
              Item[I].Flags|=LIF_SELECTED;
              SelectPos=I;
              ShowMenu(TRUE);
              if(!VMenu::ParentDialog)
              {
                ExitCode=I;
                EndLoop=TRUE;
              }
              break;
            }
      CallCount--;
      return(FALSE);
  }
  CallCount--;
  return(TRUE);
}

// ����������� ������ � ������ Disabled & Separator
int VMenu::SetSelectPos(int Pos,int Direct)
{
  int OrigPos=Pos, Pass=0;

  do{
    if (Pos<0)
    {
      if (VMFlags&VMENU_WRAPMODE)
        Pos=ItemCount-1;
      else
        Pos=0;
    }

    if (Pos>=ItemCount)
    {
      if (VMFlags&VMENU_WRAPMODE)
        Pos=0;
      else
        Pos=ItemCount-1;
    }

    if(!(Item[Pos].Flags&LIF_SEPARATOR) && !(Item[Pos].Flags&LIF_DISABLE))
      break;

    Pos+=Direct;

    if(Pass)
      return SelectPos;

    if(OrigPos == Pos) // ���� ������� - ������ �� ������� :-(
      Pass++;
  } while (1);

  Item[SelectPos].Flags&=~LIF_SELECTED;
  Item[Pos].Flags|=LIF_SELECTED;
  return Pos;
}


int VMenu::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  int MsPos,MsX,MsY;
  int XX2;

  VMFlags|=VMENU_UPDATEREQUIRED;
  if (ItemCount==0)
  {
    ExitCode=-1;
    return(FALSE);
  }

  /* $ 27.04.2001 VVM
    + ������� ������� ������� ������ �� ����� */
  if (MouseEvent->dwButtonState & 4)
  {
    ProcessKey(KEY_ENTER);
    return(TRUE);
  }
  /* VVM $ */

  MsX=MouseEvent->dwMousePosition.X;
  MsY=MouseEvent->dwMousePosition.Y;
  /* $ 06.07.2000 tran
     + mouse support for menu scrollbar
  */

  int SbY1=Y1+1, SbY2=Y2-1;

  XX2=X2;
  if (Opt.ShowMenuScrollbar && (Y2-Y1-1)<ItemCount)
    XX2--;  // ��������� �������, � ������� ���� ������ �� ����� ����

  if (Opt.ShowMenuScrollbar && MsX==X2 && (Y2-Y1-1)<ItemCount &&
      (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) )
  {
    if (MsY==SbY1)
    {
      while (IsMouseButtonPressed())
      {
        /* $ 11.12.2000 tran
           ��������� ����� �� ������ ������� ����
        */
        if (SelectPos!=0)
            ProcessKey(KEY_UP);
        ShowMenu(TRUE);
        /* tran $ */
      }
      return(TRUE);
    }
    if (MsY==SbY2)
    {
      while (IsMouseButtonPressed())
      {
        /* $ 11.12.2000 tran
           ��������� ����� �� ������ ������� ����
        */
        if (SelectPos!=ItemCount-1)
            ProcessKey(KEY_DOWN);
        /* tran $ */
        ShowMenu(TRUE);
      }
      return(TRUE);
    }
    if (MsY>SbY1 && MsY<SbY2)
    {
      int SbHeight=Y2-Y1-2;
      int Delta;
      MsPos=(ItemCount-1)*(MsY-Y1)/(SbHeight);
      if(MsPos >= ItemCount)
      {
        MsPos=ItemCount-1;
        Delta=-1;
      }
      if(MsPos < 0)
      {
        MsPos=0;
        Delta=1;
      }
      if(!(Item[MsPos].Flags&LIF_SEPARATOR) && !(Item[MsPos].Flags&LIF_DISABLE))
        SelectPos=SetSelectPos(MsPos,Delta); //??
      ShowMenu(TRUE);
      return(TRUE);
    }
  }
  /* tran 06.07.2000 $ */

  // dwButtonState & 3 - Left & Right button
  if (BoxType!=NO_BOX && (MouseEvent->dwButtonState & 3) && MsX>X1 && MsX<X2)
  {
    if (MsY==Y1)
    {
      while (MsY==Y1 && SelectPos>0 && IsMouseButtonPressed())
        ProcessKey(KEY_UP);
      return(TRUE);
    }
    if (MsY==Y2)
    {
      while (MsY==Y2 && SelectPos<ItemCount-1 && IsMouseButtonPressed())
        ProcessKey(KEY_DOWN);
      return(TRUE);
    }
  }

  while (CallCount>0)
    Sleep(10);

  /* $ 06.07.2000 tran
     + mouse support for menu scrollbar
     */
  if (MsX>X1 && MsX<XX2 && MsY>Y1 && MsY<Y2)
  /* tran 06.07.2000 $ */
  {
    MsPos=TopPos+MsY-Y1-1;
    if (MsPos<ItemCount && !(Item[MsPos].Flags&LIF_SEPARATOR) && !(Item[MsPos].Flags&LIF_DISABLE))
    {
      if (MouseX!=PrevMouseX || MouseY!=PrevMouseY || MouseEvent->dwEventFlags==0)
      {
        Item[SelectPos].Flags&=~LIF_SELECTED;
        Item[MsPos].Flags|=LIF_SELECTED;
        SelectPos=MsPos;
        ShowMenu(TRUE);
      }
      if (MouseEvent->dwEventFlags==0 && (MouseEvent->dwButtonState & 3)==0)
        ProcessKey(KEY_ENTER);
    }
    return(TRUE);
  }
  else
    if (BoxType!=NO_BOX && (MouseEvent->dwButtonState & 3) && MouseEvent->dwEventFlags==0)
    {
      ProcessKey(KEY_ESC);
      return(TRUE);
    }

  return(FALSE);
}


void VMenu::SetBottomTitle(char *BottomTitle)
{
  int Length;
  VMFlags|=VMENU_UPDATEREQUIRED;
  strncpy(VMenu::BottomTitle,BottomTitle,sizeof(VMenu::BottomTitle));
  Length=strlen(BottomTitle)+2;
  if (Length > MaxLength)
    MaxLength=Length;
}


void VMenu::SetBoxType(int BoxType)
{
  VMenu::BoxType=BoxType;
}

int VMenu::GetPosition(int Position)
{
  int DataPos=(Position==-1) ? SelectPos : Position;
  if (DataPos>=ItemCount)
    DataPos=ItemCount-1;
  return DataPos;
}

// ������������� � ����� ������.
int VMenu::SetUserData(void *Data,   // ������
                       int Size,     // ������, ���� =0 �� ��������������, ��� � Data-������
                       int Position) // ����� �����
{
  if (ItemCount==0)
    return(0);
  while (CallCount>0)
    Sleep(10);
  CallCount++;

  int DataPos=GetPosition(Position);
  struct MenuItem *PItem=Item+DataPos;
  BYTE *PtrData=NULL;

  if(!(PItem->Flags&LIF_PTRDATA))
  {
    if(PItem->UserDataSize > sizeof(PItem->UserData) && PItem->UserData)
      free(PItem->UserData);
    PItem->UserDataSize=0;
    PItem->UserData=NULL;

    if(Data)
    {
      if(Size && Size <= sizeof(PItem->UserData)) // ���� � 4 ����� �������, ��...
      {
        PtrData=(BYTE*)Data;
        PItem->UserDataSize=Size;
      }
      else
      {
        if(!Size)
          Size=strlen((char*)Data)+1;
        if((PtrData=(BYTE*)malloc(Size)) != NULL)
        {
          PItem->UserDataSize=Size;
          memcpy(PtrData,Data,PItem->UserDataSize);
        }
      }
    }
  }
  else
  {
    if((PItem->UserDataSize=Size) == 0)
      PtrData=NULL;
    else
      PtrData=(BYTE*)Data;
  }

  PItem->UserData=PtrData;

  CallCount--;
  return(PItem->UserDataSize);
}

int VMenu::GetUserDataSize(int Position)
{
  if (ItemCount==0)
    return(0);
  while (CallCount>0)
    Sleep(10);
  CallCount++;

  int DataPos=GetPosition(Position);
  int DataSize=Item[DataPos].UserDataSize;

  CallCount--;
  return(DataSize);
}

// �������� ������
void* VMenu::GetUserData(void *Data,int Size,int Position)
{
  BYTE *PtrData=NULL;
  if (ItemCount)
  {
    while (CallCount>0)
      Sleep(10);
    CallCount++;

    struct MenuItem *PItem=Item+GetPosition(Position);
    int DataSize=PItem->UserDataSize;
    PtrData=PItem->UserData;

    if(!(PItem->Flags&LIF_PTRDATA) && Size>0 && Data!=NULL && DataSize>0)
      memmove(Data,PtrData,Min(Size,DataSize));

    CallCount--;
  }
  return(PtrData);
}


int VMenu::GetSelection(int Position)
{
  if (ItemCount==0)
    return(0);
  while (CallCount>0)
    Sleep(10);

  int DataPos=GetPosition(Position);
  if (Item[DataPos].Flags&LIF_SEPARATOR)
    return(0);
  return(Item[DataPos].Flags&LIF_CHECKED);
}


void VMenu::SetSelection(int Selection,int Position)
{
  while (CallCount>0)
    Sleep(10);
  if (ItemCount==0)
    return;
  Item[GetPosition(Position)].SetCheck(Selection);
}

// ������� GetItemPtr - �������� ��������� �� ������ Item.
struct MenuItem *VMenu::GetItemPtr(int Position)
{
  if (ItemCount==0)
    return NULL;
  while (CallCount>0)
    Sleep(10);
  return Item+GetPosition(Position);
}

void VMenu::AssignHighlights(int Reverse)
{
  char Used[256];
  memset(Used,0,sizeof(Used));
  int I;
  for (I=Reverse ? ItemCount-1:0;I>=0 && I<ItemCount;I+=Reverse ? -1:1)
  {
    char *Name=Item[I].Name;
    char *ChPtr=strchr(Name,'&');
    if (ChPtr!=NULL && !(VMFlags&VMENU_SHOWAMPERSAND))
    {
      Used[LocalUpper(ChPtr[1])]=TRUE;
      Used[LocalLower(ChPtr[1])]=TRUE;
      Item[I].AmpPos=ChPtr-Item[I].Name;
    }
//_SVS(SysLog("Pre:   Item[I].AmpPos=%d Item[I].Name='%s'",Item[I].AmpPos,Item[I].Name));
  }
  for (I=Reverse ? ItemCount-1:0;I>=0 && I<ItemCount;I+=Reverse ? -1:1)
  {
    char *Name=Item[I].Name;
    char *ChPtr=strchr(Name,'&');
    if (ChPtr==NULL || (VMFlags&VMENU_SHOWAMPERSAND))
      for (int J=0;Name[J]!=0;J++)
        if (Name[J]=='&' || !Used[Name[J]] && LocalIsalphanum(Name[J]))
        {
          Used[Name[J]]=TRUE;
          Used[LocalUpper(Name[J])]=TRUE;
          Used[LocalLower(Name[J])]=TRUE;
          //memmove(Name+J+1,Name+J,strlen(Name+J)+1);
          //Name[J]='&';
          Item[I].AmpPos=J;
//_SVS(SysLog("Post:   Item[I].AmpPos=%d Item[I].Name='%s'",Item[I].AmpPos,Item[I].Name));
          break;
        }
  }
  VMFlags|=VMENU_AUTOHIGHLIGHT|(Reverse?VMENU_REVERSIHLIGHT:0);
  VMFlags&=~VMENU_SHOWAMPERSAND;
}

/* $ 28.07.2000 SVS

*/
void VMenu::SetColors(short *Colors)
{
  int I;
  if(Colors)
    for(I=0; I < sizeof(VMenu::Colors)/sizeof(VMenu::Colors[0]); ++I)
      VMenu::Colors[I]=Colors[I];
  else
  {
    int DialogStyle=CheckFlags(VMENU_WARNDIALOG);
    VMenu::Colors[0]=DialogStyle ? COL_DIALOGMENUTEXT:COL_MENUTEXT;
    VMenu::Colors[1]=DialogStyle ? COL_DIALOGMENUTEXT:COL_MENUBOX;
    VMenu::Colors[2]=COL_MENUTITLE;
    VMenu::Colors[3]=DialogStyle ? COL_DIALOGMENUTEXT:COL_MENUTEXT;
    VMenu::Colors[4]=DialogStyle ? COL_DIALOGMENUHIGHLIGHT:COL_MENUHIGHLIGHT;
    VMenu::Colors[5]=DialogStyle ? COL_DIALOGMENUTEXT:COL_MENUBOX;
    VMenu::Colors[6]=DialogStyle ? COL_DIALOGMENUSELECTEDTEXT:COL_MENUSELECTEDTEXT;
    VMenu::Colors[7]=DialogStyle ? COL_DIALOGMENUSELECTEDHIGHLIGHT:COL_MENUSELECTEDHIGHLIGHT;
    VMenu::Colors[8]=DialogStyle ? COL_DIALOGMENUSCROLLBAR: COL_MENUSCROLLBAR;
    VMenu::Colors[9]=DialogStyle ? COL_DIALOGLISTDISABLED: COL_MENUDISABLEDTEXT;
  }
}

void VMenu::GetColors(short *Colors)
{
  int I;
  for(I=0; I < sizeof(VMenu::Colors)/sizeof(VMenu::Colors[0]); ++I)
  {
    Colors[I]=VMenu::Colors[I];
  }
}

/* SVS $*/

/* $ 25.05.2001 DJ
   ��������� ������ �����
*/

void VMenu::SetOneColor (int Index, short Color)
{
  if (Index >= 0 && Index < sizeof(Colors) / sizeof (Colors [0]))
    Colors [Index]=Color;
}

/* DJ $ */

#ifndef _MSC_VER
#pragma warn -par
#endif
// ������� ��������� ���� (�� ���������)
long WINAPI VMenu::DefMenuProc(HANDLE hVMenu,int Msg,int Param1,long Param2)
{
  return 0;
}
#ifndef _MSC_VER
#pragma warn +par
#endif

#ifndef _MSC_VER
#pragma warn -par
#endif
// ������� ������� ��������� ����
long WINAPI VMenu::SendMenuMessage(HANDLE hVMenu,int Msg,int Param1,long Param2)
{
  if(hVMenu)
    return ((VMenu*)hVMenu)->VMenuProc(hVMenu,Msg,Param1,Param2);
  return 0;
}
#ifndef _MSC_VER
#pragma warn +par
#endif

static int _cdecl SortItem(const struct MenuItem *el1,
                           const struct MenuItem *el2,
                           const int *Direction)
{
  int Res=strcmp(el1->Name,el2->Name);
  return(*Direction==0?Res:(Res<0?1:(Res>0?-1:0)));
}

// ���������� ��������� ������
void VMenu::SortItems(int Direction)
{
  typedef int (*qsortex_fn)(void*,void*,void*);
  qsortex((char *)Item,
          ItemCount,
          sizeof(*Item),
          (qsortex_fn)SortItem,
          &Direction);
  VMFlags|=VMENU_UPDATEREQUIRED;
}

// return Pos || -1
int VMenu::FindItem(struct FarListFind *FindItem)
{
  if(FindItem && (DWORD)FindItem->StartIndex < (DWORD)ItemCount)
  {
    char *Pattern=FindItem->Pattern;
    for(int I=FindItem->StartIndex;I < ItemCount;I++)
      if(CmpName(Pattern,Item[I].Name,0))
        return I;
  }
  return -1;
}


BOOL VMenu::GetVMenuInfo(struct FarListInfo* Info)
{
  if(Info)
  {
    Info->Flags=VMFlags;
    Info->ItemsNumber=ItemCount;
    Info->SelectPos=SelectPos;
    Info->TopPos=TopPos;
    Info->MaxHeight=MaxHeight;
    Info->MaxLength=MaxLength;
    memset(&Info->Reserved,0,sizeof(Info->Reserved));
    return TRUE;
  }
  return FALSE;
}

DWORD VMenu::ChangeFlags(DWORD Flags,BOOL Status)
{
  if(Status)
    VMFlags|=Flags;
  else
    VMFlags&=~Flags;
  return VMFlags;
}
