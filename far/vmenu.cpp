/*
vmenu.cpp

������� ������������ ����
  � ��� ��:
    * ������ � DI_COMBOBOX
    * ������ � DI_LISTBOX
    * ...
*/

/* Revision: 1.73 13.02.2002 $ */

/*
Modify:
  13.02.2002 SVS
    ! ������� �����������...
    + ���� ���������� ������������� ����� ������� � CheckKeyHighlighted()
    + MIF_USETEXTPTR - ��� ����� TextPtr
  11.02.2002 SVS
    + ���� AccelKey � MenuData � MenuItem
    + BitFlags
    ! � ������� UpdateItem() �������� ������ ���� ���� FarListUpdate
    ! ��������� ����� ����� ����
  26.12.2001 SVS
    - ���� ��� ������ �����������, �� ������ ���������� ��������-�����
      ������ �������.
  21.12.2001 SVS
    - �� ���������� ���� VMENU_LISTBOX ��� ��������� ���������, ��-�� ����
      ������ ���� ���������������� ������ � X1=0
  02.12.2001 KM
    + ������� VMENU_SHOWAMPERSAND ������������ � AssignHighlights
      ��� ���������� ������ ShowMenu ������� ���������� ������ �����
      � ���������� VMOldFlags, � ��������� ������ ���� � �������
      ������������� DI_LISTBOX ��� ����� DIF_LISTNOAMPERSAND, ��
      ���������� ������������ � ������ ������ ���� ��� �� ����������
      ShowMenu.
  30.11.2001 DJ
    - �� ������� ���������������� MaxLength
  14.11.2001 SVS
    ! ��������� ���������������� ��� TopPos=-1 - �������� ������
  12.11.2001 SVS
    ! ��������� ���������.
  06.11.2001 SVS
    ! VMENU_REVERSIHLIGHT -> VMENU_REVERSEHIGHLIGHT
    ! struct FarListInsert.
         ���� Index ������ �������� ���������� ��������� ������, ��
         ����������� ����� ����� �������� � ����� ������.
         ���� Index ������ ����, �� ����������� ����� ����� ��������
         � ������ ������.
  05.11.2001 SVS
    ! ������� ������������-����������� �� ������ "�������������" ������
  01.11.2001 SVS
    + ������� ��� "����" - GetType*()
  30.10.2001 SVS
    - ������ ���������� ����� :-) � VMenu::FindItem() - ������ ��������
      �����.
  13.10.2001 VVM
    ! ������ ���� �� ��������� �� ���������� ������� �����, ���� ������� ���� ������ �� � ����.
  12.10.2001 VVM
    ! ����������� �������� �� ����������� � DROPDOWNLIST-�
  10.10.2001 IS
    ! ��������� const
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  16.09.2001 SVS
    - BugZ#26: ������������ ��������� ������ ������ �������
      (�� ���������� ������� ���������� ��������� :-)
  12.09.2001 SVS
    - BugZ#10: F6, Ctrl-Down - "������" ����� �� �� ������� ������.
               ��� � ������ ����? ��� ������� - ���, ������ "������������"
               ��������� ������� �� ������.
  09.09.2001 SVS
    ! �������� ��������� �� ������ ���� (����� �� ������ ����������!)
  05.09.2001 SVS
    ! ��������� ��������� �� ������ ���� (����� �� ������ ����������?)
  14.08.2001 SVS
    ! ��������� ��������� ��������� ��� ����������������
  07.08.2001 SVS
    - ������� � ������� ����� ����� �������� ���������.
  06.08.2001 SVS
    - ���� � MaxLength - ������ �������� �� ����� �����
  01.08.2001 KM
    ! ��� ����� ��������� �����������.
      �������� ���������/��������� CallCount
      � ShowMenu.
  31.07.2001 SVS
    ! ��������� ����������� (� ������ KM)
    ! MACRO_OTHER -> MACRO_MENU
  26.07.2001 OT
    - ��������� ResizeConsole()
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  22.07.2001 KM
    ! ����������� ���������� �������� �� PgUp/PgDn
      � ������������� ������ VMENU_SHOWNOBOX (NO_BOX)
  22.07.2001 KM
    - �� �������������� ��� ����� ��� ������ ������
      ShowMenu � ���������� TRUE, ��� ������ ��������
      ��������� ����.
  21.07.2001 KM
    ! ����������� ��������� ���� � ������ VMENU_SHOWNOBOX.
    ! ����������� ��������� ���� � ���� � ������ VMENU_SHOWNOBOX.

      ������ DI_LISTBOX � ������������ ������ DIF_LISTNOBOX � ����
      � ������ VMENU_SHOWNOBOX �������� ��� ������ �������������� ��������
      ������ ������, ��� ������������� ��������� ������������ DI_LISTBOX �
      ��������, �� �������� ��������� ����� ������ ������� ������ ������ ��
      ������.
  19.07.2001 OT
    - VFMenu - ����������� �����������
  18.07.2001 OT
    + ����� ����� VFMenu
  11.07.2001 SVS
    + Shift-F1 �� ����� � F1 ����� �������� ����. ��� �� ��� ������, ���� ��
      � �������� ������ ��������� F1 (����������� ���� � ����� �������� ��
      ����� ����������)
  30.06.2001 KM
    ! �������� �����������: LIFIND_NOPATTER -> LIFIND_NOPATTERN
  + GetSelectPos(struct FarListPos *)
  + SetSelectPos(struct FarListPos *)
    ! ��������� ��������� � ������� UpdateRequired: ������ ��� ����������
      TRUE � ��� �������, ��� ��������� ���� VMENU_UPDATEREQUIRED
  29.06.2001 SVS
    + ����� �������� � FindItem - �����
    + LIFIND_NOPATTER - ������ (��� ����� �������� ����) ������������ ���
      ������ � ������
  25.06.2001 IS
    ! ��������� const
  14.06.2001 SVS
    - ��������� ������� ������ �������� � ������ 0 �����.
  13.06.2001 SVS
    - ���������� �����.
  12.06.2001 KM
    - ����������� �������� ������� GetUserData. ������, ��� ������
      � ���� ����� ��������� �� � UserData, � ������ � Name, ��-��
      ���� �� ������� ����� �� ���������� (��������� �����).
  10.06.2001 SVS
    + FindItem � ����� ����������� - ��� �������� ������� ������� � ������.
    - �� ������� �����, �.�. �������������� ������� '&'
  05.06.2001 KM
    - ���������� �� ��������� ������� � Set*Title.
  04.06.2001 SVS
    - �������� ���� � DeleteItem :-(
  04.06.2001 SVS
    ! "����� � �������� �����" - ��� ��� ���� 706. � ����� ��������� �������.
  03.06.2001 KM
    + ������� SetTitle, GetTitle, GetBottomTitle.
    ! ����� DI_LISTBOX'� ����������� �������� ���������.
    ! ������ ��������� ��������� ����� VMENU_WRAPMODE, � ���������
      ������ ��� �������� ���� ��������� �������� _������_, ���
      �� ������ ������.
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
#include "lang.hpp"
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
VMenu::VMenu(const char *Title,       // ��������� ����
             struct MenuData *Data, // ������ ����
             int ItemCount,     // ���������� ������� ����
             int MaxHeight,     // ������������ ������
             DWORD Flags,       // ����� ScrollBar?
             FARWINDOWPROC Proc,    // ����������
             Dialog *ParentDialog)  // �������� ��� ListBox
{
  int I;
  SetDynamicallyBorn(false);

  MouseDown = FALSE;
  VMenu::VMFlags.Set(Flags);
/* SVS $ */

/*& 28.05.2001 OT ��������� ����������� ����� �� ����� ������� ���� */
//  FrameFromLaunched=FrameManager->GetCurrentFrame();
//  FrameFromLaunched->LockRefresh();
/* OT &*/

  VMenu::ParentDialog=ParentDialog;
  /* $ 03.06.2001 KM
     ! ������ ��������� ��������� ����� VMENU_WRAPMODE, � ���������
       ������ ��� �������� ���� ��������� �������� _������_, ���
       �� ������ ������.
  */
  VMFlags.Set(VMENU_UPDATEREQUIRED);
  /* KM $ */
  VMFlags.Skip(VMENU_SHOWAMPERSAND);
  CallCount=0;
  TopPos=0;
  SaveScr=NULL;

  if(!Proc) // ������� ������ ���� ������!!!
    Proc=(FARWINDOWPROC)VMenu::DefMenuProc;
  VMenuProc=Proc;

  if (Title!=NULL)
    strncpy(VMenu::Title,Title, sizeof(VMenu::Title)-1);
  else
    *VMenu::Title=0;

  *BottomTitle=0;
  VMenu::Item=NULL;
  VMenu::ItemCount=0;

  /* $ 01.08.2000 SVS
   - Bug � ������������, ���� �������� NULL ��� Title
  */
  /* $ 30.11.2001 DJ
     �������������� ����� ���, ��� ��������� ������
  */
  MaxLength=strlen(VMenu::Title)+2;
  /* DJ $ */
  /* SVS $ */

  RLen[0]=RLen[1]=0; // �������� ������� 2-� �������

  struct MenuItem NewItem;
  for (I=0; I < ItemCount; I++)
  {
    memset(&NewItem,0,sizeof(NewItem));
    if ((unsigned int)Data[I].Name < MAX_MSG)
      strncpy(NewItem.Name,MSG((unsigned int)Data[I].Name),sizeof(NewItem.Name)+1);
    else
      strncpy(NewItem.Name,Data[I].Name,sizeof(NewItem.Name)+1);
    //NewItem.AmpPos=-1;
    NewItem.AccelKey=Data[I].AccelKey;
    NewItem.Flags=Data[I].Flags;
    AddItem(&NewItem);
  }

  VMenu::MaxHeight=MaxHeight;
  BoxType=DOUBLE_BOX;
  for (SelectPos=0,I=0;I<ItemCount;I++)
  {
    int Length=strlen(Item[I].Name);
    if (Length>MaxLength)
      MaxLength=Length;
    if (Item[I].Flags&LIF_SELECTED)
      SelectPos=I;
  }
  if(MaxLength > ScrX-8)
    MaxLength=ScrX-8;
  /* $ 28.07.2000 SVS
     ��������� ���� �� ���������
  */
  SetColors(NULL);
  /* SVS $*/
  if (!VMFlags.Check(VMENU_LISTBOX) && CtrlObject!=NULL)
  {
    PrevMacroMode=CtrlObject->Macro.GetMode();
    if (PrevMacroMode!=MACRO_MAINMENU &&
        PrevMacroMode!=MACRO_DIALOG)
      CtrlObject->Macro.SetMode(MACRO_MENU);
  }
  if (!VMFlags.Check(VMENU_LISTBOX))
    FrameManager->ModalizeFrame(this);
}


VMenu::~VMenu()
{
  if (!VMFlags.Check(VMENU_LISTBOX) && CtrlObject!=NULL)
    CtrlObject->Macro.SetMode(PrevMacroMode);
  Hide();
  DeleteItems();
/*& 28.05.2001 OT ��������� ����������� ������, � ������� ����������� ��� ���� */
//  FrameFromLaunched->UnlockRefresh();
/* OT &*/
  if (!VMFlags.Check(VMENU_LISTBOX))
  {
    FrameManager->UnmodalizeFrame(this);
    FrameManager->RefreshFrame();
  }
}

void VMenu::Hide()
{
  while (CallCount>0)
    Sleep(10);
  CallCount++;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  if(!VMFlags.Check(VMENU_LISTBOX) && SaveScr)
  {
    delete SaveScr;
    SaveScr=NULL;
    ScreenObject::Hide();
  }

  Y2=-1;
//  X2=-1;

  VMFlags.Set(VMENU_UPDATEREQUIRED);
  CallCount--;
}


void VMenu::Show()
{
//  while (CallCount>0)
//    Sleep(10);
  CallCount++;
  if(VMFlags.Check(VMENU_LISTBOX))
    BoxType=VMFlags.Check(VMENU_SHOWNOBOX)?NO_BOX:SHORT_DOUBLE_BOX;

  int AutoCenter=FALSE,AutoHeight=FALSE;

  if (X1==-1)
  {
    X1=(ScrX-MaxLength-4)/2;
    AutoCenter=TRUE;
  }
  if(VMFlags.Check(VMENU_LISTBOX))
  {
    if(X1<0)
      X1=0;
    if (X2<=0)
      X2=X1+MaxLength+4;
  }
  else
  {
    if(X1<2)
      X1=2;
    if (X2<=0)
      X2=X1+MaxLength+4;
  }
  if (!AutoCenter && X2 > ScrX-4+2*(BoxType==SHORT_DOUBLE_BOX || BoxType==SHORT_SINGLE_BOX))
  {
    X1+=ScrX-4-X2;
    X2=ScrX-4;
    if (X1<2)
    {
      X1=2;
      X2=ScrX-2;
    }
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
    if(!VMFlags.Check(VMENU_LISTBOX))
      ScreenObject::Show();
//      Show();
    else
    {
      VMFlags.Set(VMENU_UPDATEREQUIRED);
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
//_SVS(SysLog("VMFlags&VMENU_UPDATEREQUIRED=%d",VMFlags.Check(VMENU_UPDATEREQUIRED)));
//  if (!(VMFlags&VMENU_UPDATEREQUIRED))
//    return;
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  VMFlags.Skip(VMENU_UPDATEREQUIRED);
  Modal::ExitCode=-1;

  if (!VMFlags.Check(VMENU_LISTBOX) && SaveScr==NULL)
  {
    if (!VMFlags.Check(VMENU_DISABLEDRAWBACKGROUND) && !(BoxType==SHORT_DOUBLE_BOX || BoxType==SHORT_SINGLE_BOX))
      SaveScr=new SaveScreen(X1-2,Y1-1,X2+4,Y2+2);
    else
      SaveScr=new SaveScreen(X1,Y1,X2+2,Y2+1);
  }
  if (!VMFlags.Check(VMENU_DISABLEDRAWBACKGROUND))
  {
    /* $ 23.07.2000 SVS
       ���� ��� ListBox �������
    */
    if (BoxType==SHORT_DOUBLE_BOX || BoxType==SHORT_SINGLE_BOX)
    {
      Box(X1,Y1,X2,Y2,VMenu::Colors[1],BoxType);
      if(!VMFlags.Check(VMENU_LISTBOX|VMENU_ALWAYSSCROLLBAR))
      {
        MakeShadow(X1+2,Y2+1,X2+1,Y2+1);
        MakeShadow(X2+1,Y1+1,X2+2,Y2+1);
      }
    }
    else
    {
      /* $ 21.07.2001 KM
       ! ����������� ��������� ���� � ������ VMENU_SHOWNOBOX.
      */
      if (BoxType!=NO_BOX)
        SetScreen(X1-2,Y1-1,X2+2,Y2+1,' ',VMenu::Colors[0]);
      else
        SetScreen(X1,Y1,X2,Y2,' ',VMenu::Colors[0]);
      /* KM $ */
      if(!VMFlags.Check(VMENU_LISTBOX|VMENU_ALWAYSSCROLLBAR))
      {
        MakeShadow(X1,Y2+2,X2+3,Y2+2);
        MakeShadow(X2+3,Y1,X2+4,Y2+2);
      }
      if (BoxType!=NO_BOX)
        Box(X1,Y1,X2,Y2,VMenu::Colors[1],BoxType);
    }
    /* SVS $*/
  }
  /* $ 03.06.2001 KM
     ! ����� DI_LISTBOX'� ����������� �������� ���������.
  */
  int WidthTitle=MaxLength;
  if (*Title)
  {
    if((WidthTitle=strlen(Title)) > MaxLength)
      WidthTitle=MaxLength-1;
    GotoXY(X1+(X2-X1-1-WidthTitle)/2,Y1);
    SetColor(VMenu::Colors[2]);
    mprintf(" %*.*s ",WidthTitle,WidthTitle,Title);
  }
  if (*BottomTitle)
  {
    if((WidthTitle=strlen(BottomTitle)) > MaxLength)
      WidthTitle=MaxLength-1;
    GotoXY(X1+(X2-X1-1-WidthTitle)/2,Y2);
    SetColor(VMenu::Colors[2]);
    mprintf(" %*.*s ",WidthTitle,WidthTitle,BottomTitle);
  }
  /* KM $ */
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
  CallCount++;

//_SVS(SysLog("VMenu::ShowMenu()"));
  char TmpStr[1024];
  unsigned char BoxChar[2],BoxChar2[2];
  int Y,I;
  if (ItemCount==0 || X2<=X1 || Y2<=Y1)
  {
    CallCount--;
    return;
  }
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  BoxChar2[1]=BoxChar[1]=0;

  /* $ 22.07.2001 KM
   - �� �������������� ��� ����� ��� ������ ������
     ShowMenu � ���������� TRUE, ��� ������ ��������
     ��������� ����.
  */
  if (VMFlags.Check(VMENU_LISTBOX))
    BoxType=VMFlags.Check(VMENU_SHOWNOBOX)?NO_BOX:SHORT_DOUBLE_BOX;
  /* KM $ */
  if(!IsParent && VMFlags.Check(VMENU_LISTBOX))
  {
    BoxType=VMFlags.Check(VMENU_SHOWNOBOX)?NO_BOX:SHORT_SINGLE_BOX;
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
      *BoxChar=0x0B3; // |
      break;
    case DOUBLE_BOX:
    case SHORT_DOUBLE_BOX:
      *BoxChar=0x0BA; // ||
      break;
  }
  if (SelectPos<ItemCount)
    Item[SelectPos].Flags|=LIF_SELECTED;

  /* $ 02.12.2001 KM
     ! ��������������, ���� �����, �������� "�������" �������.
  */
  if(VMFlags.Check(VMENU_AUTOHIGHLIGHT|VMENU_REVERSEHIGHLIGHT))
    AssignHighlights(VMFlags.Check(VMENU_REVERSEHIGHLIGHT));
  /* KM $ */

  /* $ 21.07.2001 KM
   ! ����������� ��������� ���� � ������ VMENU_SHOWNOBOX.
  */
  if (SelectPos>TopPos+((BoxType!=NO_BOX)?Y2-Y1-2:Y2-Y1))
    TopPos=SelectPos-((BoxType!=NO_BOX)?Y2-Y1-2:Y2-Y1);
  if (SelectPos<TopPos)
    TopPos=SelectPos;

  char *NamePtr;
  for (Y=Y1+((BoxType!=NO_BOX)?1:0),I=TopPos;Y<((BoxType!=NO_BOX)?Y2:Y2+1);Y++,I++)
  /* KM $ */
  {
    GotoXY(X1,Y);
    if (I<ItemCount)
    {
      if (Item[I].Flags&LIF_SEPARATOR)
      {
        int SepWidth=X2-X1+1;
        char *Ptr=TmpStr+1;
        MakeSeparator(SepWidth,TmpStr,
          BoxType==NO_BOX?0:(BoxType==SINGLE_BOX||BoxType==SHORT_SINGLE_BOX?2:1));

        if (I>0 && I<ItemCount-1 && SepWidth>3)
          for (unsigned int J=0;Ptr[J+3]!=0;J++)
          {
            if (Item[I-1][J]==0)
              break;
            if (Item[I-1][J]==0x0B3)
            {
              int Correction=0;
              if (!VMFlags.Check(VMENU_SHOWAMPERSAND) && memchr(Item[I-1].PtrName(),'&',J)!=NULL)
                Correction=1;
              if (strlen(Item[I+1].PtrName())>=J && Item[I+1][J]==0x0B3)
                Ptr[J-Correction+2]=0x0C5;
              else
                Ptr[J-Correction+2]=0x0C1;
            }
          }
        Text(X1,Y,VMenu::Colors[1],TmpStr);
        if (*Item[I].PtrName())
        {
          int ItemWidth=strlen(Item[I].PtrName());
          GotoXY(X1+(X2-X1-1-ItemWidth)/2,Y);
          mprintf(" %*.*s ",ItemWidth,ItemWidth,Item[I].PtrName());
        }
      }
      else
      {
        /* $ 21.07.2001 KM
         ! ����������� ��������� ���� � ������ VMENU_SHOWNOBOX.
        */
        if (BoxType!=NO_BOX)
        {
          SetColor(VMenu::Colors[1]);
          Text((char*)BoxChar);
          GotoXY(X2,Y);
          Text((char*)BoxChar);
        }
        if ((Item[I].Flags&LIF_SELECTED) && !(Item[I].Flags&LIF_DISABLE))
          SetColor(VMenu::Colors[6]);
        else
          SetColor(VMenu::Colors[(Item[I].Flags&LIF_DISABLE?9:3)]);
        if (BoxType!=NO_BOX)
          GotoXY(X1+1,Y);
        else
          GotoXY(X1,Y);
        /* KM $ */
        char Check=' ';
        if (Item[I].Flags&LIF_CHECKED)
          if (!(Item[I].Flags&0x0000FFFF))
            Check=0x0FB;
          else
            Check=(char)Item[I].Flags&0x0000FFFF;

        sprintf(TmpStr,"%c %.*s",Check,X2-X1-3,Item[I].PtrName());
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
        if(VMFlags.Check(VMENU_SHOWAMPERSAND))
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
    }
    else
    {
      /* $ 21.07.2001 KM
       ! ����������� ��������� ���� � ������ VMENU_SHOWNOBOX.
      */
      if (BoxType!=NO_BOX)
      {
        SetColor(VMenu::Colors[1]);
        Text((char*)BoxChar);
        GotoXY(X2,Y);
        Text((char*)BoxChar);
        GotoXY(X1+1,Y);
      }
      else
        GotoXY(X1,Y);
      SetColor(VMenu::Colors[3]);
      mprintf("%*s",((BoxType!=NO_BOX)?X2-X1-1:X2-X1),"");
      /* KM $ */
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
  /* $ 21.07.2001 KM
   ! ����������� ��������� ���� � ������ VMENU_SHOWNOBOX.
  */
  if (VMFlags.Check(VMENU_LISTBOX|VMENU_ALWAYSSCROLLBAR) || Opt.ShowMenuScrollbar)
  {
    if (((BoxType!=NO_BOX)?Y2-Y1-1:Y2-Y1+1)<ItemCount)
    {
      SetColor(VMenu::Colors[8]);
      if (BoxType!=NO_BOX)
        ScrollBar(X2,Y1+1,Y2-Y1-1,SelectPos,ItemCount);
      else
        ScrollBar(X2,Y1,Y2-Y1+1,SelectPos,ItemCount);
    }
  }
  /* KM $ */
  /* 18.07.2000 SVS $ */
  /* SVS $ */
  /* tran $ */
  CallCount--;
}
/* 28.07.2000 SVS $ */

BOOL VMenu::UpdateRequired(void)
{
  return ((ItemCount>=TopPos && ItemCount<TopPos+Y2-Y1) || VMFlags.Check(VMENU_UPDATEREQUIRED));
}

BOOL VMenu::CheckKeyHiOrAcc(DWORD Key,int Type,int Translate)
{
  int I, Result;
  struct MenuItem *CurItem;
  for (CurItem=Item,I=0; I < ItemCount; I++, ++CurItem)
  {
    if((!Type && CurItem->AccelKey && Key == CurItem->AccelKey) ||
       (Type && Dialog::IsKeyHighlighted(CurItem->PtrName(),Key,Translate,CurItem->AmpPos))
      )
    {
      Item[SelectPos].Flags&=~LIF_SELECTED;
      CurItem->Flags|=LIF_SELECTED;
      SelectPos=I;
      ShowMenu(TRUE);
      if(!VMenu::ParentDialog)
      {
        Modal::ExitCode=I;
        EndLoop=TRUE;
      }
      break;
    }
  }
  return EndLoop==TRUE;
}

int VMenu::ProcessKey(int Key)
{
  int I;

  if (Key==KEY_NONE || Key==KEY_IDLE)
    return(FALSE);

  VMFlags.Set(VMENU_UPDATEREQUIRED);
  if (ItemCount==0)
    if (Key!=KEY_F1 && Key!=KEY_SHIFTF1 && Key!=KEY_F10 && Key!=KEY_ESC)
    {
      Modal::ExitCode=-1;
      return(FALSE);
    }

  while (CallCount>0)
    Sleep(10);
  CallCount++;

  switch(Key)
  {
    case KEY_ENTER:
      if(VMenu::ParentDialog)
        VMenu::ParentDialog->ProcessKey(Key);
      else
      {
        EndLoop=TRUE;
        Modal::ExitCode=SelectPos;
      }
      break;
    case KEY_ESC:
    case KEY_F10:
      if(VMenu::ParentDialog)
        VMenu::ParentDialog->ProcessKey(Key);
      else
      {
        EndLoop=TRUE;
        Modal::ExitCode=-1;
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
      /* $ 22.07.2001 KM
       ! ����������� ���������� �������� �� PgUp/PgDn
         � ������������� ������ VMENU_SHOWNOBOX (NO_BOX)
      */
      if((I=SelectPos-((BoxType!=NO_BOX)?Y2-Y1-1:Y2-Y1)) < 0)
        I=0;
      SelectPos=SetSelectPos(I,1);
      ShowMenu(TRUE);
      break;
    case KEY_PGDN:
      if((I=SelectPos+((BoxType!=NO_BOX)?Y2-Y1-1:Y2-Y1)) >= ItemCount)
        I=ItemCount-1;
      SelectPos=SetSelectPos(I,-1);
      ShowMenu(TRUE);
      break;
      /* KM $ */
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
      if(!CheckKeyHiOrAcc(Key,0,0))
      {
        if(Key == KEY_SHIFTF1 || Key == KEY_F1)
        {
          if(VMenu::ParentDialog)
            VMenu::ParentDialog->ProcessKey(Key);
          else
            ShowHelp();
          break;
        }
        else
        {
          if(!CheckKeyHiOrAcc(Key,1,FALSE))
            CheckKeyHiOrAcc(Key,1,TRUE);
        }
      }
      CallCount--;
      return(FALSE);
  }
  CallCount--;
  return(TRUE);
}

int VMenu::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  int MsPos,MsX,MsY;
  int XX2;

  VMFlags.Set(VMENU_UPDATEREQUIRED);
  if (ItemCount==0)
  {
    Modal::ExitCode=-1;
    return(FALSE);
  }

  /* $ 27.04.2001 VVM
    + ������� ������� ������� ������ �� ����� */
  if (MouseEvent->dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
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

  /* $ 21.07.2001 KM
   ! ����������� ��������� ���� � ���� � ������ VMENU_SHOWNOBOX.
  */
  int SbY1=((BoxType!=NO_BOX)?Y1+1:Y1), SbY2=((BoxType!=NO_BOX)?Y2-1:Y2);

  XX2=X2;

  /* $ 12.10.2001 VVM
    ! ���� �� � ��� ���������? */
  int ShowScrollBar = FALSE;
  if (VMFlags.Check(VMENU_LISTBOX|VMENU_ALWAYSSCROLLBAR) || Opt.ShowMenuScrollbar)
    ShowScrollBar = TRUE;
  /* VVM $ */

  if (ShowScrollBar && ((BoxType!=NO_BOX)?Y2-Y1-1:Y2-Y1+1)<ItemCount)
    XX2--;  // ��������� �������, � ������� ���� ������ �� ����� ����

  if (ShowScrollBar && MsX==X2 && ((BoxType!=NO_BOX)?Y2-Y1-1:Y2-Y1+1)<ItemCount &&
      (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) )
  /* KM $ */
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
  /* $ 21.07.2001 KM
   ! ����������� ��������� ���� � ���� � ������ VMENU_SHOWNOBOX.
  */
  if ((BoxType!=NO_BOX)?
      (MsX>X1 && MsX<XX2 && MsY>Y1 && MsY<Y2):
      (MsX>=X1 && MsX<=XX2 && MsY>=Y1 && MsY<=Y2))
  /* KM $ */
  /* tran 06.07.2000 $ */
  {
    /* $ 21.07.2001 KM
     ! ����������� ��������� ���� � ���� � ������ VMENU_SHOWNOBOX.
    */
    MsPos=TopPos+((BoxType!=NO_BOX)?MsY-Y1-1:MsY-Y1);
    /* KM $ */
    if (MsPos<ItemCount && !(Item[MsPos].Flags&LIF_SEPARATOR) && !(Item[MsPos].Flags&LIF_DISABLE))
    {
      if (MouseX!=PrevMouseX || MouseY!=PrevMouseY || MouseEvent->dwEventFlags==0)
      {
        Item[SelectPos].Flags&=~LIF_SELECTED;
        Item[MsPos].Flags|=LIF_SELECTED;
        SelectPos=MsPos;
        ShowMenu(TRUE);
      }
      /* $ 13.10.2001 VVM
        + ��������� ������� ������� ����� � ������ � ���� ������ ����������� ��� ���������� */
      if (MouseEvent->dwEventFlags==0 &&
         (MouseEvent->dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED|RIGHTMOST_BUTTON_PRESSED)))
        MouseDown = TRUE;
      if (MouseEvent->dwEventFlags==0 &&
         (MouseEvent->dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED|RIGHTMOST_BUTTON_PRESSED))==0 &&
          MouseDown)
      {
        MouseDown = FALSE;
        ProcessKey(KEY_ENTER);
      }
      /* VVM $ */
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


void VMenu::DeleteItems()
{
  /* $ 13.07.2000 SVS
     �� ��� �� ������� ������ ������ ����� new :-)
  */
  if(Item)
  {
    for(int I=0; I < ItemCount; ++I)
      if(Item[I].UserDataSize > sizeof(Item[I].UserData) && Item[I].UserData)
        free(Item[I].UserData);
    free(Item);
  }
  /* SVS $ */
  Item=NULL;
  ItemCount=0;
  SelectPos=TopPos=0;
  MaxLength=strlen(VMenu::Title)+2;
  if(MaxLength > ScrX-8)
    MaxLength=ScrX-8;
}


/* $ 01.08.2000 SVS
   ������� �������� N ������� ����
*/
int VMenu::DeleteItem(int ID,int Count)
{
  if(ID < 0 || ID >= ItemCount || Count <= 0)
    return ItemCount;
  if(ID+Count > ItemCount)
    Count=ItemCount-ID; //???
  if(Count <= 0)
    return ItemCount;

  while (CallCount>0)
    Sleep(10);
  CallCount++;

  // ������� ������� ������, ���� ������ �� ������ �� ����
  for(int I=0; I < Count; ++I)
  {
    struct MenuItem *PtrItem=Item+ID+I;
    if(PtrItem->UserDataSize > sizeof(PtrItem->UserData) && PtrItem->UserData)
      free(PtrItem->UserData);
  }

  // � ��� ������ �����������
  if(ItemCount > 1)
    memmove(Item+ID,Item+ID+Count,sizeof(struct MenuItem)*(ItemCount-(ID+Count))); //???

  // ��������� ������� �������
  if(SelectPos >= ID && SelectPos < ID+Count)
  {
    SelectPos=ID;
    if(ID+Count == ItemCount)
      SelectPos--;
  }
  if(SelectPos < 0)
    SelectPos=0;

  ItemCount-=Count;

  if(SelectPos < TopPos || SelectPos > TopPos+Y2-Y1 || TopPos >= ItemCount)
  {
    TopPos=0;
    VMFlags.Set(VMENU_UPDATEREQUIRED);
  }

  // ����� �� �������� �����?
  if ((ID >= TopPos && ID < TopPos+Y2-Y1) ||
      (ID+Count >= TopPos && ID+Count < TopPos+Y2-Y1)) //???
  {
    VMFlags.Set(VMENU_UPDATEREQUIRED);
  }

  CallCount--;
  return(ItemCount);
}
/* SVS $ */

int VMenu::AddItem(const struct MenuItem *NewItem,int PosAdd)
{
  while (CallCount>0)
    Sleep(10);
  CallCount++;

  struct MenuItem *NewPtr;
  int Length;

  if(PosAdd >= ItemCount)
    PosAdd=ItemCount;

  if (UpdateRequired())
    VMFlags.Set(VMENU_UPDATEREQUIRED);

  if ((ItemCount & 255)==0)
  {
    if ((NewPtr=(struct MenuItem *)realloc(Item,sizeof(struct MenuItem)*(ItemCount+256+1)))==NULL)
      return(0);
    Item=NewPtr;
  }

  // ���� < 0 - ���������� ������ � ������� �������, �.� ������� ������
  if(PosAdd < 0)
    PosAdd=0;

  if(PosAdd < ItemCount)
    memmove(Item+PosAdd+1,Item+PosAdd,sizeof(struct MenuItem)*(ItemCount-PosAdd)); //??

  Item[PosAdd]=*NewItem;
  Length=strlen(Item[PosAdd].PtrName());
  if (Length>MaxLength)
    MaxLength=Length;
  if(MaxLength > ScrX-8)
    MaxLength=ScrX-8;
  if (Item[PosAdd].Flags&LIF_SELECTED)
    SelectPos=PosAdd;
  if(Item[PosAdd].Flags&0x0000FFFF)
  {
    Item[PosAdd].Flags|=LIF_CHECKED;
    if((Item[PosAdd].Flags&0x0000FFFF) == 1)
      Item[PosAdd].Flags&=0xFFFF0000;
  }
  Item[PosAdd].AmpPos=-1;

  // ���������� ��������
  int I=0, J=0;
  char Chr;
  const char *NamePtr=Item[PosAdd].PtrName();
  while((Chr=NamePtr[I]) != 0)
  {
    if(Chr != '&' && Chr != '\t')
      J++;
    else
    {
      if(Chr != '&')
        Item[PosAdd].Idx2=++J;
      else
        Item[PosAdd].AmpPos=J;
    }
    ++I;
  }

  Item[PosAdd].Len[0]=strlen(NamePtr)-Item[PosAdd].Idx2; //??
  if(Item[PosAdd].Idx2)
    Item[PosAdd].Len[1]=strlen(&NamePtr[Item[PosAdd].Idx2]);

  // ��������� ����� ��������
  if(RLen[0] < Item[PosAdd].Len[0])
    RLen[0]=Item[PosAdd].Len[0];
  if(RLen[1] < Item[PosAdd].Len[1])
    RLen[1]=Item[PosAdd].Len[1];

  if(VMFlags.Check(VMENU_AUTOHIGHLIGHT|VMENU_REVERSEHIGHLIGHT))
    AssignHighlights(VMFlags.Check(VMENU_REVERSEHIGHLIGHT));
//  if(VMFlags.Check(VMENU_LISTBOXSORT))
//    SortItems(0);
  CallCount--;
  return(ItemCount++);
}

int  VMenu::AddItem(const char *NewStrItem)
{
  struct FarList FarList0;
  struct FarListItem FarListItem0;

  memset(&FarListItem0,0,sizeof(FarListItem0));
  if(!NewStrItem || NewStrItem[0] == 0x1)
  {
    FarListItem0.Flags=LIF_SEPARATOR;
    strncpy(FarListItem0.Text,NewStrItem+1,sizeof(FarListItem0.Text)-2);
  }
  else
  {
    strncpy(FarListItem0.Text,NewStrItem,sizeof(FarListItem0.Text)-1);
  }
  FarList0.ItemsNumber=1;
  FarList0.Items=&FarListItem0;
  return VMenu::AddItem(&FarList0);
}

int VMenu::AddItem(const struct FarList *List)
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

int VMenu::UpdateItem(const struct FarListUpdate *NewItem)
{
  if(NewItem && (DWORD)NewItem->Index < (DWORD)ItemCount)
  {
    struct MenuItem MItem;
    // ��������� ������... �� ����� �������� ;-)
    struct MenuItem *PItem=Item+NewItem->Index;
    if(PItem->UserDataSize > sizeof(PItem->UserData) && PItem->UserData)
      free(PItem->UserData);

    memcpy(PItem,FarList2MenuItem(&NewItem->Item,&MItem),sizeof(struct MenuItem));
    return TRUE;
  }
  return FALSE;
}

int VMenu::InsertItem(const struct FarListInsert *NewItem)
{
  if(NewItem)
  {
    struct MenuItem MItem;
    return AddItem(FarList2MenuItem(&NewItem->Item,&MItem),NewItem->Index);
  }
  return -1;
}

int VMenu::GetUserDataSize(int Position)
{
  if (ItemCount==0)
    return(0);
  while (CallCount>0)
    Sleep(10);
  CallCount++;

  int DataSize=Item[GetPosition(Position)].UserDataSize;

  CallCount--;
  return(DataSize);
}

int VMenu::_SetUserData(struct MenuItem *PItem,
                       const void *Data,   // ������
                       int Size)     // ������, ���� =0 �� ��������������, ��� � Data-������
{
  if(PItem->UserDataSize > sizeof(PItem->UserData) && PItem->UserData)
    free(PItem->UserData);

  PItem->UserDataSize=0;
  PItem->UserData=NULL;

  if(Data)
  {
    int SizeReal=Size;

    // ���� Size=0, �� ���������������, ��� � Data ��������� ASCIIZ ������
    if(!Size)
      SizeReal=strlen((const char*)Data)+1;

    // ���� ������ ������ Size=0 ��� Size ������ 4 ���� (sizeof(void*))
    if(!Size ||
        Size > sizeof(PItem->UserData)) // ���� � 4 ����� �� �������, ��...
    {
      // ������ ������ 4 ����?
      if(SizeReal > sizeof(PItem->UserData))
      {
        // ...������ �������� ������ ������.
        if((PItem->UserData=(char*)malloc(SizeReal)) != NULL)
        {
          PItem->UserDataSize=SizeReal;
          memcpy(PItem->UserData,Data,SizeReal);
        }
      }
      else // ��� ������ ���������� � 4 �����!
      {
        PItem->UserDataSize=SizeReal;
        memcpy(PItem->Str4,Data,SizeReal);
      }
    }
    else // ��. ������ ���������� � 4 �����...
    {
      PItem->UserDataSize=0;         // ������� ����, ��� ������ ���� ���, ����
      PItem->UserData=(char*)Data;   // ��� ���������� � 4 �����
    }
  }
  return(PItem->UserDataSize);
}

void* VMenu::_GetUserData(struct MenuItem *PItem,void *Data,int Size)
{
  int DataSize=PItem->UserDataSize;
  char *PtrData=PItem->UserData; // PtrData ��������: ���� ��������� �� ���-�� ����
                                 // 4 �����!
  /* $ 12.06.2001 KM
     - ����������� �������� �������. ������, ��� ������ � ����
       ����� ���� � ������� MenuItem.Name
  */
  if (Size > 0 && Data != NULL)
  {
    if (PtrData) // ������ ����?
    {
      // ��������� ������ 4 ����?
      if(DataSize > sizeof(PItem->UserData))
      {
        memmove(Data,PtrData,Min(Size,DataSize));
      }
      else if(DataSize > 0) // � ������ �� ������ ����? �.�. ���� � UserData
      {                     // ���� ������ �� 4 ���� (UserDataSize ��� ���� > 0)
        memmove(Data,PItem->Str4,Min(Size,DataSize));
      }
      // else � �����... � PtrData ��� ��������� �����!
    }
    else // ... ������ ���, ������ ����� ��� ������!
    {
      PtrData=PItem->PtrName();
      if(PItem->Flags&MIF_USETEXTPTR)
        memmove(Data,PtrData,Min(Size,(int)strlen(PtrData)));
      else
        memmove(Data,PItem->Name,Min(Size,(int)sizeof(PItem->Name)));
    }
  }
  /* KM $ */
  return(PtrData);
}

struct FarListItem *VMenu::MenuItem2FarList(const struct MenuItem *MItem,
                                            struct FarListItem *FItem)
{
  if(FItem && MItem)
  {
    memset(FItem,0,sizeof(struct FarListItem));
    FItem->Flags=MItem->Flags&(~MIF_USETEXTPTR); //??
    strncpy(FItem->Text,((struct MenuItem *)MItem)->PtrName(),sizeof(FItem->Text)-1);
//    FItem->AccelKey=MItem->AccelKey;
    //??????????????????
    //   FItem->UserData=MItem->UserData;
    //   FItem->UserDataSize=MItem->UserDataSize;
    //??????????????????
    return FItem;
  }
  return NULL;
}

struct MenuItem *VMenu::FarList2MenuItem(const struct FarListItem *FItem,
                                         struct MenuItem *MItem)
{
  if(FItem && MItem)
  {
    memset(MItem,0,sizeof(struct MenuItem));
    MItem->Flags=FItem->Flags;
//    MItem->AccelKey=FItem->AccelKey;
    strncpy(MItem->Name,FItem->Text,sizeof(MItem->Name)-1);
    MItem->Flags&=~MIF_USETEXTPTR;
    //VMenu::_SetUserData(MItem,FItem->UserData,FItem->UserDataSize); //???
    // � ����� ���� ��������� AmpPos????
    return MItem;
  }
  return NULL;
}

// �������� ������� ������� � ������� ������� �����
int VMenu::GetSelectPos(struct FarListPos *ListPos)
{
  ListPos->SelectPos=GetSelectPos();
  ListPos->TopPos=TopPos;
  return ListPos->SelectPos;
}

// ���������� ������ � ������� ����
int VMenu::SetSelectPos(struct FarListPos *ListPos)
{
  int Ret=SetSelectPos(ListPos->SelectPos,1);
  int OldTopPos=TopPos;
  if(Ret > -1)
  {
    TopPos=ListPos->TopPos;
    if(ListPos->TopPos == -1)
    {
      if(ItemCount < MaxHeight)
        TopPos=0;
      else
      {
        TopPos=Ret-MaxHeight/2;
        if(TopPos+MaxHeight > ItemCount)
          TopPos=ItemCount-MaxHeight;
      }
    }
  }
  return Ret;
}

// ����������� ������ � ������ Disabled & Separator
int VMenu::SetSelectPos(int Pos,int Direct)
{
  if(!Item || !ItemCount)
    return -1;

  int OrigPos=Pos, Pass=0;

  do{
    if (Pos<0)
    {
      if (VMFlags.Check(VMENU_WRAPMODE))
        Pos=ItemCount-1;
      else
        Pos=0;
    }

    if (Pos>=ItemCount)
    {
      if (VMFlags.Check(VMENU_WRAPMODE))
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
  SelectPos=Pos;
  /* $ 01.07.2001 KM
    ����� �����, ��� ������� ���������� ��� ����������� (������
    ������ �� "�������", ��� ������� ����������).
  */
  VMFlags.Set(VMENU_UPDATEREQUIRED);
  /* KM $ */
  return Pos;
}

void VMenu::SetTitle(const char *Title)
{
  int Length;
  VMFlags.Set(VMENU_UPDATEREQUIRED);
  Title=NullToEmpty(Title);
  strncpy(VMenu::Title,Title,sizeof(VMenu::Title)-1);
  Length=strlen(Title)+2;
  if (Length > MaxLength)
    MaxLength=Length;
  if(MaxLength > ScrX-8)
    MaxLength=ScrX-8;
}


char *VMenu::GetTitle(char *Dest,int Size)
{
  if (Dest && *VMenu::Title)
    return strncpy(Dest,VMenu::Title,Size-1);
  return NULL;
}


void VMenu::SetBottomTitle(const char *BottomTitle)
{
  int Length;
  VMFlags.Set(VMENU_UPDATEREQUIRED);
  BottomTitle=NullToEmpty(BottomTitle);
  strncpy(VMenu::BottomTitle,BottomTitle,sizeof(VMenu::BottomTitle)-1);
  Length=strlen(BottomTitle)+2;
  if (Length > MaxLength)
    MaxLength=Length;
  if(MaxLength > ScrX-8)
    MaxLength=ScrX-8;
}


char *VMenu::GetBottomTitle(char *Dest,int Size)
{
  if (Dest && *VMenu::BottomTitle)
    return strncpy(Dest,VMenu::BottomTitle,Size-1);
  return NULL;
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


int VMenu::GetSelection(int Position)
{
  if (ItemCount==0)
    return(0);
  while (CallCount>0)
    Sleep(10);

  int DataPos=GetPosition(Position);
  if (Item[DataPos].Flags&LIF_SEPARATOR)
    return(0);
  int Checked=Item[DataPos].Flags&0xFFFF;
  return((Item[DataPos].Flags&LIF_CHECKED)?(Checked?Checked:1):0);
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

  /* $ 02.12.2001 KM
     + ������� VMENU_SHOWAMPERSAND ������������ ��� ����������
       ������ ShowMenu ������� ���������� ������ �����, � ���������
       ������ ���� � ������� ������������� DI_LISTBOX ��� �����
       DIF_LISTNOAMPERSAND, �� ���������� ������������ � ������
       ������ ���� ��� �� ���������� ShowMenu.
  */
  if (VMFlags.Check(VMENU_SHOWAMPERSAND))
    VMOldFlags.Set(VMENU_SHOWAMPERSAND);
  if (VMOldFlags.Check(VMENU_SHOWAMPERSAND))
    VMFlags.Set(VMENU_SHOWAMPERSAND);
  /* KM $ */
  int I, Delta=Reverse ? -1:1;
  for (I=(Reverse ? ItemCount-1:0); I >= 0 && I < ItemCount; I+=Delta)
  {
    const char *Name=Item[I].PtrName();
    const char *ChPtr=strchr(Name,'&');
    if (ChPtr!=NULL && !VMFlags.Check(VMENU_SHOWAMPERSAND))
    {
      Used[LocalUpper(ChPtr[1])]=TRUE;
      Used[LocalLower(ChPtr[1])]=TRUE;
      Item[I].AmpPos=ChPtr-Name;
    }
//_SVS(SysLog("Pre:   Item[I].AmpPos=%d Item[I].Name='%s'",Item[I].AmpPos,Item[I].PtrName()));
  }
  for (I=Reverse ? ItemCount-1:0;I>=0 && I<ItemCount;I+=Reverse ? -1:1)
  {
    const char *Name=Item[I].PtrName();
    const char *ChPtr=strchr(Name,'&');
    if (ChPtr==NULL || VMFlags.Check(VMENU_SHOWAMPERSAND))
      for (int J=0; Name[J]; J++)
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
  VMFlags.Set(VMENU_AUTOHIGHLIGHT|(Reverse?VMENU_REVERSEHIGHLIGHT:0));
  VMFlags.Skip(VMENU_SHOWAMPERSAND);
}

/* $ 28.07.2000 SVS

*/
void VMenu::SetColors(short *Colors)
{
  if(Colors)
    memmove(VMenu::Colors,Colors,sizeof(VMenu::Colors));
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
  memmove(Colors,VMenu::Colors,sizeof(VMenu::Colors));
}

/* SVS $*/

/* $ 25.05.2001 DJ
   ��������� ������ �����
*/

void VMenu::SetOneColor (int Index, short Color)
{
  if ((DWORD)Index < sizeof(Colors) / sizeof (Colors [0]))
    Colors [Index]=Color;
}

/* DJ $ */

static int _cdecl SortItem(const struct MenuItem *el1,
                           const struct MenuItem *el2,
                           const int *Direction)
{
  int Res=strcmp(((struct MenuItem *)el1)->PtrName(),((struct MenuItem *)el2)->PtrName());
  return(*Direction==0?Res:(Res<0?1:(Res>0?-1:0)));
}

// ���������� ��������� ������
void VMenu::SortItems(int Direction)
{
  typedef int (*qsortex_fn)(const void*,const void*,void*);
  qsortex((char *)Item,
          ItemCount,
          sizeof(*Item),
          (qsortex_fn)SortItem,
          &Direction);
  VMFlags.Set(VMENU_UPDATEREQUIRED);
}

// return Pos || -1
int VMenu::FindItem(const struct FarListFind *FItem)
{
  return FindItem(FItem->StartIndex,FItem->Pattern,FItem->Flags);
}

int VMenu::FindItem(int StartIndex,const char *Pattern,DWORD Flags)
{
  char TmpBuf[130];
  if((DWORD)StartIndex < (DWORD)ItemCount)
  {
    const char *NamePtr;
    int LenPattern=strlen(Pattern);
    for(int I=StartIndex;I < ItemCount;I++)
    {
      NamePtr=Item[I].PtrName();
      int LenNamePtr=strlen(NamePtr);
      memcpy(TmpBuf,NamePtr,Min((int)LenNamePtr+1,(int)sizeof(TmpBuf)));
      if(Flags&LIFIND_NOPATTERN)
      {
        if(!LocalStrnicmp(RemoveChar(TmpBuf,'&'),Pattern,Max(LenPattern,LenNamePtr)))
          return I;
      }
      else
      {
        if(CmpName(Pattern,RemoveChar(TmpBuf,'&'),1))
          return I;
      }
    }
  }
  return -1;
}

BOOL VMenu::GetVMenuInfo(struct FarListInfo* Info)
{
  if(Info)
  {
    Info->Flags=VMFlags.Flags;
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
  int DataSize=VMenu::_SetUserData(Item+GetPosition(Position),Data,Size);
  CallCount--;
  return DataSize;
}

// �������� ������
void* VMenu::GetUserData(void *Data,int Size,int Position)
{
  void *PtrData=NULL;
  if (ItemCount)
  {
    while (CallCount>0)
      Sleep(10);
    CallCount++;
    PtrData=VMenu::_GetUserData(Item+GetPosition(Position),Data,Size);
    CallCount--;
  }
  return(PtrData);
}

void VMenu::Process()
{
  Modal::Process();
}

void VMenu::ResizeConsole()
{
  SaveScr->Discard();
  delete SaveScr;
  SaveScr=NULL;
  if (this->CheckFlags(VMENU_NOTCHANGE))
  {
    return;
  }
  ObjWidth=ObjHeight=0;
  if (!this->CheckFlags(VMENU_NOTCENTER))
  {
    Y2=X2=Y1=X1=-1;
  }
  else
  {
    X1=5;
    if (!this->CheckFlags(VMENU_LEFTMOST) && ScrX>40)
    {
      X1=(ScrX+1)/2+5;
    }
    Y1=(ScrY+1-(this->ItemCount+5))/2;
    if (Y1<1) Y1=1;
    X2=Y2=0;
  }
}

int VMenu::GetTypeAndName(char *Type,char *Name)
{
  if(Type)
    strcpy(Type,MSG(MVMenuType));
  if(Name)
    strcpy(Name,Title);
  return(MODALTYPE_VMENU);
}


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

char MenuItem::operator[](int Pos) const
{
  if(Flags&MIF_USETEXTPTR)
    return (!NamePtr || Pos > strlen(NamePtr))?0:NamePtr[Pos];
  return (Pos > strlen(Name))?0:Name[Pos];
}

char* MenuItem::PtrName()
{
  return ((Flags&MIF_USETEXTPTR)!=0&&NamePtr)?NamePtr:Name;
}
