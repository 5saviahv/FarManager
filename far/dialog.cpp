/*
dialog.cpp

����� �������

*/

/* Revision: 1.91 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  04.05.2001 SVS
    ! ������� �� ����� ���� �� DI_LISTBOX ;-) - ����� ���� FarDialogItem.ListPos
    - ���� � DN_INIT* - ��������� "�� ������� ����", �.�. �� �����������
      ������� ����������� �������.
    - ������ ����� �������� DialogEdit->FastShow() � ������� �������������?
  28.04.2001 SVS
   + ������� GetItemRect() ��������� ��������� �����.
   - DN_MOUSECLICK ���������� �� �������, ���� ����� ������ ������ � ������.
  27.04.2001 SVS
   - �� �������� ���������������� � ������� � � ����������� - ������
     MaxLen, ������ sizeof(MaxLen) :-((
  27.04.2001 VVM
   + ��������� KEY_MSWHEEL_XXXX
   - ����� ������� ������ ��� ���������.
  26.04.2001 SVS
   - �������� � DM_SETTEXTLENGTH: ���������� ������������ ������ � ���
     ������, ���� �� ��� �� ���������.
  25.04.2001 SVS
   + ��������� �� ������ DM_SETTEXTLENGTH & DIF_VAREDIT
  24.04.2001 SVS
   ! ������� ������ ��� ��������� ������.
   ! ���� � ����� �� ������� DN_MOUSECLICK (Param=-1) ���������� �������
     ������ TRUE, �� ������ �� ����� ������. ���� ������������
     {DN_MOUSECLICK,-1} ��� ������� FALSE, �� ������ �� ���������.
   ! ���� ����� ��� �������� - �������������� ������ ����� � ������ �������
     ��������� ������� �� ����� ��������
  23.04.2001 SVS
   - ����� ������� ����� DN_EDITCHANGE ��� ������ �� �������.
  22.04.2001 SVS
   + DIF_SHOWAMPERSAND ��� DI_BUTTON, DI_CHECKBOX, DI_RADIOBUTTON
  16.04.2001 SVS
   + Tab � ������ ������� - ������ Enter
   ! ����������� � �������������� ������ ���� ����� DN_EDITCHANGE (imho)
  12.04.2001 SVS
   + DM_ADDHISTORY - �������� ������ � �������
   + DIF_MANUALADDHISTORY - ��������� � ������� ������ "�������"
   ! ������� AddToEditHistory ������ ���������� ��������� ��������
     ���������� ������ � �������
  12.04.2001 SVS
   ! �������������� �������� ��� DM_SETDLGITEM �� ����� ���� �������.
     ������ ���� ������ - ���� ��� �� ������ � ������������. ����� ��� ����.
   + CheckDialogCoord() - �������� � ������������� ��������� �������
   ! �� ��� ���������� �������� ������������� ������� ��� DM_MOVEDIALOG
     ��� ����� ���������� = -1, � ������ ���������� ��������.
  02.04.2001 SVS
   + �������� ����� ������ RO ��� ���� ����� � ����������
  23.03.2001 SVS
   ! � ������� ConvertItem() ����� �������� InternalCall - ������
     ������������ ������ ��� DN_EDITCHANGE
  22.03.2001 SVS
   - ������� ���������� ��������� DN_EDITCHANGE
  05.03.2001 SVS
   - ���� � ������� :-(
  21.02.2001 IS
   ! Opt.EditorPersistentBlocks -> Opt.EdOpt.PersistentBlocks
  21.02.2001 IS
   - ��������� �� ������ ������ � SelectFromEditHistory (����������� �� � ����,
     �� ����������� ��!)
  20.02.2001 SVS
   ! ��������� ��������� IsKeyHighlighted � ��������� Alt- ��
     ������� ��� ��������
  20.02.2001 SVS
   ! ��������� ��������� ������� ������ � ������ Disabled & Hidden...
  20.02.2001 SVS
   - ���� � SelectFromComboBox
  13.02.2001 SVS
   ! HISTORY_COUNT - ��������� ��� ������� �������.
   + �������������� �������� � FindInEditForAC, SelectFromEditHistory,
     AddToEditHistory � SelectFromComboBox - MaxLen - ������������ ������
     ������ ����������.
   + DI_COMBOBOX ���� ����� �������� � DIF_VAREDIT
   ! ��������� "�������" ���������� � DefDlgProc(DN_DRAWDIALOG,1)
   ! ���� ������� ���������� � ����� �� DN_DRAWDIALOG ������� FALSE,
     �� ������ �������������� �� �����
   ! ���� ������� ���������� � ����� �� DN_DRAWDLGITEM ������� FALSE,
     �� ������� ������� �������������� �� �����
  12.02.2001 SVS
   + ������� �� ������� DOUBLE_CLICK - �������� ����� ������� ������������� :-)
     AN> ...�������� �� �� �������� ������������� ����� ������.
     AN> ��� ������ ���������� ������� �� (MouseEvent.dwEventFlags==DOUBLE_CLICK).
  11.02.2001 SVS
   ! DIF_VAREDIT - ������ ��� DI_EDIT!!!
  11.02.2001 SVS
   ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  23.01.2001 SVS
   ! �������� ������� DeleteDialogObjects() � GetDialogObjectsData()
   + ������� ��������� � DataToItem � ConvertItem
   ! ������� �������� �� NULL �������� ObjPtr
  04.01.2001 SVS
   - Bug - DM_KEY �� ���������� �� ��, ��� ���������� ���������������� �������
  04.01.2001 SVS
   - Bug ��� ������������� DM_SETDLGITEM - ������� �������������� ����� �����
  28.12.2000 SVS
   + ��������� ��������� Opt.HotkeyRules
  25.12.2000 SVS
   - ����� ������� ������� �� ������� ��� DM_GETTEXTPTR
  21.12.2000 SVS
   ! Ctr-Break ������ ��������������, �.�. ��� ������� �� ����
     ��� ������ ����������.
   + DIF_SHOWAMPERSAND ��� DI_*BOX
   + DM_GETTEXTPTR, DM_SETTEXTPTR
  15.12.2000 SVS
   ! ��� ����������� ������� ��������� ��������� "�����������" ����.
   ! ����� ������ �������� �����������
  14.12.2000 SVS
   ! ���������� ������ (��������� :-)
   + ������ ������� ������� ��� DIF_DISABLE
  08.12.2000 SVS
   ! DM_SET(GET)TEXT - Param2 ��������� �� ��������� FarDialogItemData
  06.12.2000 SVS
   - ���������� ���� � SelectOnEntry() - ��� ������� ����������
     �� �������� ���������!!!
   ! ��������� �������� ���� ��� ������� �������� � DefDlgProc
   ! ������� ������ ������:
     1) DI_BUTTON: ���� �� ������ ����� � ������ "�� ��� ��������"
                   (DIF_BTNNOCLOSE), �� ������ �� ������, ����� -
                   ���������� ������� ������.
     2) ���� ������ Enter �� �� DI_BUTTON, �� ������������ ��� � ������
        ��� �������, ��� ���� ������ � Default=1 � � ���� ������ ��
        ��������� ���� DIF_BTNNOCLOSE.
  05.12.2000 IS
   ! ��� ������ � �����, ��� ���������, ������������� ��� ���,
     � ������� ������
  04.12.2000 SVS
   - ������� � DIF_NOFOCUS :-(
   - ��� �������� � ���������� ����� - ��������� ������ ������ :-(
   ! ��� DI_USERCONTROL - ��� ����������� Data ������������ memmove()
  04.12.2000 SVS
   ! �������� � ������� �����:
     "Ctrl-A Alt-N Enter - ����� ����� �� �������������� � ���� ����."
   ! �������������� - ����� �� �������� �� ����� ������������ ��������.
   ! ����������� ������� ConvertItem() � DataToItem() - � �����������
     ����� ���������� ���������� � ������� ��� (MSVC - ��� ��� ������ :-(
   + DIF_3STATE - 3-� ��������� CheckBox
   ! ������� �� ��� ������ ���� DIF_DROPDOWNLIST - ������ ��� DI_COMBOBOX.
   - ��������� �� ������ ����������� � ���� ����������� ��������
   ! ��� DI_PSWEDIT � DI_FIXEDIT ��������� DIF_EDITEXPAND �� �����
   ! ��������� Disabled
  03.12.2000 IS
   ! �� �������������, ���� ����� ������� ���� ������������ �������.
     �������� ��� �������, �����������, ������ � ����������� �������.
  21.11.2000 SVS
   - �� ��������� ��������� ������ � ������������� ���������
  08.11.2000 SVS
   ! ������� �������� ����� ������� ��� hotkey (������������ ��������)
  04.11.2000 SVS
   + �������� �� �������������� ������� ��� XLat-�������������
  26.10.2000 SVS
   ! DM_SETEDITPOS/DM_GETEDITPOS -> DM_SETCURSORPOS/DM_GETCURSORPOS
   + ������ � �������� ��� DI_USERCONTROL � ������������ ��������.
  23.10.2000 SVS
   + DM_SETEDITPOS, DM_GETEDITPOS -
      ���������������� ������� � ������� ��������������.
  20.10.2000 SVS
   + DM_GETFOCUS - �������� ID �������� �������� ����� �����
  16.10.2000 tran 1.47
   + ��� EDIT ����� ������������ ����������� � 511 ��������
  27.09.2000 SVS
   ! Alt-Up/Down/Left/Right - ����� (����� � ������� �� ������������
     � MultiEdit)
   ! Ctrl-Alt-Shift - ���������, ���� ����.
  24.09.2000 SVS
   + �������� ������� - Alt-�������
   + ����� ������� Xlat
  22.09.2000 SVS
   ! ��������� AutoComplete ��� ���������� ������.
  20.09.2000 SVS
   ! Enter � ������� ����� (����� DIF_EDITOR) ��������� ������.
  18.09.2000 SVS
   + DIF_READONLY - ���� ��� ����� ��������������
      (����! ��� ����� ��������������).
  18.09.2000 SVS
   ! ��������� ��� SelectOnEntry
   ! ����� �� ������ ���� ������ (������ �� �������� �� �����������)!
  14.09.2000 SVS
   + ���� DIF_LISTNOAMPERSAND. �� ��������� ��� DI_LISTBOX
      ������������ ���� MENU_SHOWAMPERSAND. ���� ���� ��������� �����
      ���������
  12.09.2000 SVS
   ! ������ ��������� ��� ������ � DefaultButton=1:
     ����� ������ ���������� �� ����� ������� ���������� ��������� DM_CLOSE.
   ! ���������� �������� � BackSpace � DIF_EDITOR
   ! ������ ��������, ���� Del ������ � ������� �������, ��� �����
     ������ (DIF_EDITOR)
  11.09.2000 SVS
   + Ctrl-U � ������� ����� ������� ������� �����
  09.09.2000 SVS
   + DIF_NOFOCUS - ������� �� �������� ������ ����� (�����������)
   + ����� ������� DMODE_OLDSTYLE - ������ � ������ �����.
   ! �������� �� �������, ����� ��� �������, ������ ������ � ������ �����.
   + ������� SelectOnEntry - ��������� ������ ��������������
     (��������� ����� DIF_SELECTONENTRY)
  08.09.2000 SVS
   - ���� �������, �� DM_SETFOCUS ����� ��� � ������� :-)
   ! ��������� ��� DN_MOUSECLICK
  31.08.2000 SVS
   + DM_ENABLE (�� ��������� ����� :-)
   - ���� � ������� ������ ������.
  30.08.2000 SVS
   + ����� Hide()
   + ����� ������� DMODE_SHOW - ������ �����?
   ! ��������� ��� IsEnableRedraw
   + DM_MOVEDIALOG - ����������� �������.
   ! ��������� ������ ��� ComboBox(DowpDownList)
  29.08.2000 SVS
   ! ��� ������� ���� ������ �� �������� ���������...
     ������, ����� ����� ����������� �������!
  29.08.2000 SVS
   - ������ ����������� �����-��� - ������� ProcessHighlighting
     MY> ������ � ��������� ����� �������. � ���� � ���� ������� �������.
     MY> ������ � ��������� ���� ������, ���� Alt-������ ����� �
     MY> ��������� ������������� �����.
  28.08.2000 SVS
   - ��� ��� ������ (��� �� ��������������!) :-)
  25.08.2000 SVS
   + DM_GETDLGRECT - �������� ���������� ����������� ����
   ! ��������� ��� DN_MOUSECLICK
  25.08.2000 SVS
   ! ���������, ������������ ������ ����� - �� �� ��� ��������, ����.
  24.08.2000 SVS
   + InitDialogObjects ����� �������� - ��� ���������� ���������������
     ���������
  24.08.2000 SVS
   ! ���������� ����!!!! (������ ������ �����)... ��� ��� ComboBox
     ����� ����� �� �����...
   + CtrlAltShift - ��������/�������� ������...
   + ������� DI_USERCONTROL - ���������� ���������� ������.
  24.08.2000 SVS
   ! ����������� ������ - � ������� �� �������...
     ������������ � ChangeFocus()
  23.08.2000 SVS
   ! ��������� ��������� DMSG_* -> DM_ (�����) & DN_ (������)
   + DM_KEY        - �������/�������� �������(�)
   + DM_GETDLGDATA - ����� ������ �������.
   + DM_SETDLGDATA - ���������� ������ �������.
   + DM_SHOWDIALOG - ��������/�������� ������
   + ���������� ������ FocusPos - ������ �������� ����� ������� � ������
   ! ���������� IsCanMove, InitObjects, CreateObjects, WarningStyle, Dragged
     ������� -> ������� ����� � DialogMode
   + ��������� ������ ��������� ;-)
  22.08.2000 SVS
   ! � ���� ���������� ������ �� ��� :-((
     IsMovedDialog -> IsCanMove
   ! DMSG_PAINT -> DMSG_DRAWDIALOG
   ! DMSG_DRAWITEM -> DMSG_DRAWDLGITEM
   ! DMSG_CHANGELIST -> DMSG_LISTCHANGE
   ! ShowDialog - �������������� �������� - ����� ������� ������������
  21.08.2000 SVS 1.23
   ! �������� ��������� ������ - �������� � ����� :-)))
   ! ������ ���������� � ���� ����� ������ ���� DefDlgProc � SendDlgMessage
     (��� ��� ������ :-)
   + ����� ���������� DI_LISTBOX ������� �� ��������� �������� ���������
   ! DIF_HISTORY ����� ����� ������� ���������, ��� DIF_MASKEDIT
   - ���� � AutoComplete ��� ���������� ���������� ������
   ! DMSG_CHANGEITEM -> DMSG_EDITCHANGE
   + DMSG_BTNCLICK
  12.08.2000 KM 1.20
   + ���������� ������ DI_FIXEDIT � ������ DIF_MASKEDIT ��� ���������
     ����� � ������ �����.
  18.08.2000 SVS
   + DialogMode - ����� �������� ������ �������
   + ���� IsEnableRedraw - �����������/����������� ����������� �������
   + ���������: DMSG_ENABLEREDRAW, DMSG_MOUSECLICK
   + DI_BUTTON ���� ������ ����� ����� DIF_SETCOLOR
   + ���� ��� DI_BUTTON - DIF_BTNNOCLOSE - "������ �� ��� �������� �������"
   - ���� �� ����� ��������� �������� "���", �� ������ ������������, �.�.
     �� ��� ������� ���� ������.
  15.08.2000 SVS
   ! ��� DropDownList ����� ������������ �� �����.
   + ������� ���, ����� ����� ������ � DropDownList ������ ����������� ���.
  11.08.2000 SVS
   + ������, ������������� ��� ����������� ���������� �������
   + ��� ����, ����� ������� DMSG_CLOSE ����� �������������� Process
   ! ��������� ��� DMSG_CLOSE
  11.08.2000 KM 1.18
   ! ����� ����������� ���, ������������ ������������ ����������� �������
     ������. ����������� �� � ������� ������� ������ � ���� ���� :)
   ! ����-���� ���������� �������� ������ �������. ����: ����� ������� �������
     ������ �� ������� ����������� ������ ����������� �� ���������� �� ��� ���
     ���� �� ���������� ���� �������� �������, ����� ���� � �����������
     ����������� ������ ����� ������, ��� ���� � ����� ������������� ������� :)
   ! ��������: ���� ����� ������� ������� ����, �� �������� ����
     ������ ����� ������, ����� ����������� ������ (������ ��������� ��� �����),
     �� ��-�� ����, ��� � ����� ������� PrevMouseX � PrevMouseY ��� �� ����������,
     ����� ������������� ��������� ������ �������.
  10.08.2000 SVS
   + ���������� IsMovedDialog - ����� �� ������� ������ :-)
   + ������� ��������� IsMovedDialog
  09.08.2000 tran 1.16
   - ������ "�������"
  09.08.2000 KM
   ! ��� ��������� ������ ����������� ������� ���������
     ���������� ��������� �������. ��� ���������.
   ! ���������� ����������� ������� ������, ������-�� �� �������
     ���������� :). ������ ����������� ����� ����������, ��� ������������
     ������� �� ������.
   ! ��� ������ �� ���� ������ ��������, ����, �� �����������, ����������
     ����������.

     ����� �������� ������� ������� - ����� ����
     ��������� �����.
  07.08.2000 SVS
   + � ������� ������������ ��������� ��� ListBox ������!
  04.08.2000 SVS
    + FarListItems.CountItems -> FarListItems.ItemsNumber
  03.08.2000 tran
   + ������� �������� ������� - �������� �� ������ �����
     �������� - ����������� �������� �� ������� ������ ��������� �� ���
     ����� ������ �����...
     �������� ��������� - �� ����� ������� �����
     � MOVE � ���� ����� ���� ����� � �����������.
     ������ ��� MOVE ������������� "//# "
  01.08.2000 SVS
   ! History ������ ������ ����� ScrollBar, �.�. ���� ������� �����
     � ComboBox`�, ������ � ����.
   - ���������� ������ lastKey ������� �� ������������� :-)
   + ������� ListBox
   - ��������� ������ � AutoComplete
   ! � History ������ ��������� �������� (��� DIF_EXPAND...) �����
     ����������� �����!
   + ��� �� ����� ���� DIF_USELASTHISTORY � �������� ������ �����,
     �� ��������������� ������ �������� �� History
   - ��������� ����������� ��� DI_PSWEDIT ����� History...
     ...��� �� ������ �������... � ��� ��������� ������, �.�.
  31.07.2000 tran & SVS
   + ����������� ������� �� ������ ���������. Ctrl-F5 �������� �����
     �����������. ��������� ����������� - "Move" � ����� ������� ����
  28.07.2000 SVS
   ! �������� Edit *EditLine � ������� FindInEditForAC ����� �������!
   - ��������� ��� � ���������������:
       ...���� � ������� �� F7 - "templates". ��� F7, ������� shift+t=T � ...
       ������� ��������� t. � ����� ������� ������� ������ ���������� �
       ����� ��������� � ��������� ����...
   - ���� ������ �� �������� �� ������ ��������� � �������,
     �� �������� ������ �� ���� ������������, � ���������
     ����� �� ������ �������������� �������� ;-)
   + AutoComplite: ��� DI_COMBOBOX.
   ! SelectFromComboBox ����� �������������� �������� � ���, �����
     ��������������� item � ���� �� ������� � ����������� �� ������� �����
   ! FindInEditHistory -> FindInEditForAC
     ����� ��� � �������, ��� � � ComboBox`� (����� �� ������� ����)
   + ������� IsFocused, ������������ - "����� �� ������� �������
     ����� ����� �����"
   + ������� ConvertItem - �������������� �� ����������� �������������
     � FarDialogItem � �������
   + ��������� ���������� ���������:
        DMSG_INITDIALOG, DMSG_ENTERIDLE, DMSG_HELP, DMSG_PAINT,
        DMSG_SETREDRAW, DMSG_DRAWITEM, DMSG_GETDLGITEM, DMSG_KILLFOCUS,
        DMSG_GOTFOCUS, DMSG_SETFOCUS, DMSG_GETTEXTLENGTH, DMSG_GETTEXT,
        DMSG_CTLCOLORDIALOG, DMSG_CTLCOLORDLGITEM, DMSG_CTLCOLORDLGLIST,
        DMSG_SETTEXTLENGTH, DMSG_SETTEXT, DMSG_CHANGEITEM, DMSG_HOTKEY,
        DMSG_CLOSE,
  26.07.2000 SVS
   + �� �������-�� - ������������ ��������������� ComboBox
  26.07.2000 SVS
   + AutoComplite: ��� DIF_HISTORY.
  25.07.2000 SVS
   + ����� �������� � ������������
  23.07.2000 SVS
   + ���� ������� � ���������� :-)
   + ������� ����� ����������� - ������� � ���� ������� ���������
   ! ��������� ��������� "SavedDialogHistory\\%s",
     "Locked%d" � "Line%d" ������� ��������������.
   + ������� ��������� ������� (�� ���������) DefDlgProc() - ������ ����� :-)
  19.07.2000 SVS
   ! "...� ��������� ������ ���� ������� home shift+end del
     ���� �� ���������..."
     DEL � ������, ������� DIF_EDITOR, ������� ��� ����� ���������...
  18.07.2000 SVS
   + ��������� �������� DI_COMBOBOX (���� ��� ��� ��������������)
   + �������-���������� ������ �� ������ - SelectFromComboBox
  11.07.2000 SVS
   ! ��������� ��� ����������� ���������� ��� BC & VC
  05.07.2000 SVS
   + ��������� �������� �� ���� DIF_EDITEXPAND - ���������� ����������
     ����� � �������� ������� DI_EDIT
  25.06.2000 SVS
   ! ���������� Master Copy
   ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "dialog.hpp"
#include "lang.hpp"
#include "fn.hpp"
#include "global.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "chgprior.hpp"
#include "vmenu.hpp"

static char fmtLocked[]="Locked%d";
static char fmtLine[]  ="Line%d";
static char fmtSavedDialogHistory[]="SavedDialogHistory\\%s";
static char *CheckBox3State=NULL;

//////////////////////////////////////////////////////////////////////////
/* Public:
   ����������� ������ Dialog
*/
Dialog::Dialog(struct DialogItem *Item,int ItemCount,
               FARWINDOWPROC DlgProc,long InitParam)
{
  /* $ 04.12.2000 SVS
     ���� ���� - �������� ������, �������������� 3-� ��������� CheckBox
  */
  if(!CheckBox3State)
    CheckBox3State=MSG(MCheckBox2State);
  /* SVS $ */
  /* $ 29.08.2000 SVS
    ����� �������, ���������� ������ (-1 = Main)
  */
  PluginNumber=-1;
  /* SVS $ */
  /* $ 23.08.2000 SVS
    ������ �������.
  */
  DialogMode=0;
  /* SVS $ */
  /* $ 11.08.2000 SVS
    + ������, ������������� ��� ����������� ���������� �������
  */
  Dialog::DataDialog=InitParam;
  /* SVS $ */
  DialogTooLong=0;
  /* $ 10.08.2000 SVS
     ���������� ������� ����� �������
  */
  SetDialogMode(DMODE_ISCANMOVE);
  /* SVS $ */
  /* $ 18.08.2000 SVS
  */
  /*
    + ���� IsEnableRedraw - �����������/����������� ����������� �������
      =0 - ���������, ������ �������� - �� ��������������
        ����� ���������� ��������� DMSG_ENABLEREDRAW, �� ���� ����
        ��� Param1=TRUE �������������, ��� Param1 = FALSE - �����������
  */
  IsEnableRedraw=0;

  FocusPos=-1;
  PrevFocusPos=-1;

  if(!DlgProc) // ������� ������ ���� ������!!!
  {
    DlgProc=(FARWINDOWPROC)Dialog::DefDlgProc;
    // ����� ������ � ������ ����� - ����� ���� ����!
    SetDialogMode(DMODE_OLDSTYLE);
  }
  Dialog::DlgProc=DlgProc;

  Dialog::Item=Item;
  Dialog::ItemCount=ItemCount;

  if (CtrlObject!=NULL)
  {
    // �������� ����. ����� �����.
    PrevMacroMode=CtrlObject->Macro.GetMode();
    // ��������� ����� � �������� :-)
    CtrlObject->Macro.SetMode(MACRO_DIALOG);
  }

  // ���������� ���������� ��������� �������
  GetConsoleTitle(OldConsoleTitle,sizeof(OldConsoleTitle));
}


//////////////////////////////////////////////////////////////////////////
/* Public, Virtual:
   ���������� ������ Dialog
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
   ������ �������� ��������� ���� ������� � ����� �������
   ScreenObject::Show() ��� ������ ������� �� �����.
*/
void Dialog::Show()
{
  if (!CheckDialogMode(DMODE_INITOBJECTS))      // ��������������� �������, �����
  {                      //  �������� ���������������� ��� ������ ������.
    /* $ 28.07.2000 SVS
       ������ ���������, ��� � ��� ��� ��!
    */
    CheckDialogCoord();
    if(DlgProc((HANDLE)this,DN_INITDIALOG,InitDialogObjects(),DataDialog))
    {
      // ��� �����, �.�. ������ ����� ���� ��������
      InitDialogObjects();
    }
    // ��� ������� �������������������!
    SetDialogMode(DMODE_INITOBJECTS);
  }
  CheckDialogCoord();
  ScreenObject::Show();
}

void Dialog::CheckDialogCoord(void)
{
  if (X1 == -1) // ������ ������������� ������� �� �����������?
  {             //   X2 ��� ���� = ������ �������.
    X1=(ScrX - X2 + 1)/2;

    if (X1 <= 0) // ������ ������� ������ ������ ������?
    {
      DialogTooLong=X2-1;
      X1=0;
      X2=ScrX;
    }
    else
      X2+=X1-1;
  }

  if (Y1 == -1) // ������ ������������� ������� �� ���������?
  {             //   Y2 ��� ���� = ������ �������.
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
}

/* $ 30.08.2000 SVS
  ���� ��������� ������ ������� - ���������� ����������...
*/
void Dialog::Hide()
{
  ScreenObject::Hide();
  SkipDialogMode(DMODE_SHOW);
}
/* SVS $*/

//////////////////////////////////////////////////////////////////////////
/* Private, Virtual:
   ������������� �������� � ����� ������� �� �����.
*/
void Dialog::DisplayObject()
{
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  Shadow();              // "�������" ����
  ShowDialog();          // "��������" ������.
}


//////////////////////////////////////////////////////////////////////////
/* Public:
   ������������� ��������� �������.
*/
/* $ 28.07.2000 SVS
   ������ InitDialogObjects ���������� ID ��������
   � ������� �����
*/
/* $ 24.08.2000 SVS
  InitDialogObjects ����� �������� - ��� ���������� ���������������
  ���������. ID = -1 - ������� ���� ��������
*/
int Dialog::InitDialogObjects(int ID)
{
  int I, J, TitleSet;
  int Length,StartX;
  int Type;
  struct DialogItem *CurItem;
  int InitItemCount;
  DWORD ItemFlags;

  if(ID+1 > ItemCount)
    return -1;

  if(ID == -1) // �������������� ���?
  {
    ID=0;
    InitItemCount=ItemCount;
  }
  else
  {
    InitItemCount=ID+1;
  }

  /* 04.01.2001 SVS
     ���� FocusPos � �������� � ������� ����������, �� ���� �������. */
  if(FocusPos >= 0 && FocusPos < ItemCount &&
     (Item[FocusPos].Flags&(DIF_DISABLE|DIF_NOFOCUS|DIF_HIDDEN)))
    FocusPos = -1; // ����� ������ �������!
  /* SVS $ */

  // ��������������� ���� �� ������ ������ � ��������� �������
  for(I=ID, TitleSet=0; I < InitItemCount; I++)
  {
    CurItem=&Item[I];
    ItemFlags=CurItem->Flags;

    // ��� ������ �� ������ ����� "���������� ��������� ������ ��� ������"
    //  ������� ���� ����� ������
    if (CurItem->Type==DI_BUTTON &&
        (ItemFlags & DIF_NOBRACKETS)==0 &&
        *CurItem->Data != '[')
    {
      char BracketedTitle[200];
      sprintf(BracketedTitle,"[ %s ]",CurItem->Data);
      strcpy(CurItem->Data,BracketedTitle);
    }

    // �� ������� ����������� "������" ��������� ��������� �������!
    if (!TitleSet &&             // ��� �������, ��� ��� �� �������������
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

     // ��������������� ���� ������
     if(FocusPos == -1 &&
        IsFocused(CurItem->Type) &&
        CurItem->Focus &&
        !(ItemFlags&(DIF_DISABLE|DIF_NOFOCUS|DIF_HIDDEN)))
       FocusPos=I; // �������� ������ �������� �������
     CurItem->Focus=0; // ������� ��� ����, ����� �� ���������,
                       //   ��� ������� - ��� � ������� ��������
  }

  // ����� ��� ����� ����� - ������, ���� "����" ������ ���������
  // ���� �� ����, �� ������ �� ������ ����������
  if(FocusPos == -1)
  {
    for (I=0; I < ItemCount; I++) // �� ����!!!!
    {
      CurItem=&Item[I];
      if(IsFocused(CurItem->Type) &&
         !(CurItem->Flags&(DIF_DISABLE|DIF_NOFOCUS|DIF_HIDDEN)))
      {
        FocusPos=I;
        break;
      }
    }
  }
  if(FocusPos == -1) // �� �� ����� ���� - ��� �� ������
  {                  //   �������� � ������������ ������
     FocusPos=0;     // ������, ����
  }

  // �� ��� � ��������� ��!
  Item[FocusPos].Focus=1;

  // � ������ ��� ������� � �� ������ ���������...
  for (I=ID; I < InitItemCount; I++)
  {
    CurItem=&Item[I];
    Type=CurItem->Type;
    ItemFlags=CurItem->Flags;

    // ��������������� ����������� �������� � ������ DIF_CENTERGROUP
    // � ���������� ������������ �������� ����� �������������� � �������.
    // �� ���������� X �� �����. ������ ������������ ��� �������������
    // ����� ������.
    if ((ItemFlags & DIF_CENTERGROUP) &&
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
       ������� ListBox
    */
    if (Type==DI_LISTBOX)
    {
      if (!CheckDialogMode(DMODE_CREATEOBJECTS))
        CurItem->ObjPtr=new VMenu(NULL,NULL,0,CurItem->Y2-CurItem->Y1+1,
                               VMENU_ALWAYSSCROLLBAR|VMENU_LISTBOX,NULL/*,this*/);

      VMenu *ListBox=(VMenu *)CurItem->ObjPtr;

      if(ListBox)
      {
        // ������ ��� �����
        ListBox->DeleteItems();

        struct MenuItem ListItem={0};
        /* $ 13.09.2000 SVS
           + ���� DIF_LISTNOAMPERSAND. �� ��������� ��� DI_LISTBOX &
             DI_COMBOBOX ������������ ���� MENU_SHOWAMPERSAND. ���� ����
             ��������� ����� ���������
        */
        if(!(ItemFlags&DIF_LISTNOAMPERSAND))
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
            ListItem.Disabled=Items[J].Flags&LIF_DISABLE;
            // ����� ����� �������� �������� �� LIF_PTRDATA!!!
            ListItem.Flags=0;
            ListItem.PtrData=NULL;
            if(Items[J].Flags&LIF_PTRDATA)
            {
              strncpy(ListItem.Name,Items[J].Ptr.PtrData,sizeof(ListItem.Name));
              ListItem.UserDataSize=Items[J].Ptr.PtrLength;
              if(Items[J].Ptr.PtrLength > sizeof(ListItem.UserData))
              {
                ListItem.PtrData=Items[J].Ptr.PtrData;
                ListItem.Flags=1;
              }
              else
                memmove(ListItem.UserData,Items[J].Ptr.PtrData,Items[J].Ptr.PtrLength);
            }
            else
            {
              strncpy(ListItem.Name,Items[J].Text,sizeof(ListItem.Name));
              strncpy(ListItem.UserData,Items[J].Text,sizeof(ListItem.UserData));
              ListItem.UserDataSize=strlen(Items[J].Text);
            }

            ListBox->AddItem(&ListItem);
          }
        }
      }
    }
    /* SVS $*/
    // "���������" - �������� ������...
    if (IsEdit(Type))
    {
      if (!CheckDialogMode(DMODE_CREATEOBJECTS))
        CurItem->ObjPtr=new Edit;

      Edit *DialogEdit=(Edit *)CurItem->ObjPtr;
      /* $ 26.07.2000 SVS
         �� �������-�� - ������������ ��������������� ComboBox
      */
      /* $ 30.11.200 SVS
         ������� �� ��� ������ ���� DIF_DROPDOWNLIST
      */
      if ((ItemFlags & DIF_DROPDOWNLIST) && Type == DI_COMBOBOX)
      {
         DialogEdit->DropDownBox=1;
      }
      /* SVS $ */
      /* SVS $ */
      /* $ 18.09.2000 SVS
         ReadOnly!
      */
      if (ItemFlags & DIF_READONLY)
      {
         DialogEdit->ReadOnly=1;
      }
      /* SVS $ */
      /* $ 15.10.2000 tran
        ������ ���������������� ������ ����� �������� � 511 �������� */
      // ���������� ������������ ������ � ��� ������, ���� �� ��� �� ���������
      if(DialogEdit->GetMaxLength() == -1)
      {
        if((CurItem->Type==DI_EDIT || CurItem->Type==DI_COMBOBOX) &&
           (ItemFlags&DIF_VAREDIT))
          DialogEdit->SetMaxLength(CurItem->Ptr.PtrLength);
        else
          DialogEdit->SetMaxLength(511);
      }
      /* tran $ */
      DialogEdit->SetPosition(X1+CurItem->X1,Y1+CurItem->Y1,
                              X1+CurItem->X2,Y1+CurItem->Y2);
      DialogEdit->SetObjectColor(
         FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
             ((ItemFlags&DIF_DISABLE)?COL_WARNDIALOGEDITDISABLED:COL_WARNDIALOGEDIT):
             ((ItemFlags&DIF_DISABLE)?COL_DIALOGEDITDISABLED:COL_DIALOGEDIT)),
         FarColorToReal((ItemFlags&DIF_DISABLE)?COL_DIALOGEDITDISABLED:COL_DIALOGEDITSELECTED));
      if (CurItem->Type==DI_PSWEDIT)
      {
        DialogEdit->SetPasswordMode(TRUE);
        /* $ 01.08.2000 SVS
          ...��� �� ������ �������... � ��� ��������� ������, �.�.
        */
        ItemFlags&=~DIF_HISTORY;
        /* SVS $ */
      }

      if (Type==DI_FIXEDIT)
      {
        /* $ 21.08.2000 SVS
           DIF_HISTORY ����� ����� ������� ���������, ��� DIF_MASKEDIT
        */
        if(ItemFlags&DIF_HISTORY)
          ItemFlags&=~DIF_MASKEDIT;
        /* SVS $ */
        // ���� DI_FIXEDIT, �� ������ ����� �������� �� ������...
        //   ��-�� - ���� ������������������ :-)
        DialogEdit->SetMaxLength(CurItem->X2-CurItem->X1+1);
        DialogEdit->SetOvertypeMode(TRUE);
        /* $ 12.08.2000 KM
           ���� ��� ������ ����� DI_FIXEDIT � ���������� ���� DIF_MASKEDIT
           � �������� �������� CurItem->Mask, �� �������� ����� �������
           ��� ��������� ����� � ������ Edit.
        */
        /* $ 18.09.2000 SVS
          ����� �� ������ ���� ������ (������ �� �������� �� �����������)!
        */
        if ((ItemFlags & DIF_MASKEDIT) && CurItem->Mask)
        {
          char *Ptr=CurItem->Mask;
          while(*Ptr && *Ptr == ' ') ++Ptr;
          if(*Ptr)
            DialogEdit->SetInputMask(CurItem->Mask);
          else
          {
            CurItem->Mask=NULL;
            ItemFlags&=~DIF_MASKEDIT;
          }
        }
        /* SVS $ */
        /* KM $ */
      }
      else
        // "����-��������"
        // ��������������� ������������ ���� ����� (edit controls),
        // ������� ���� ���� ������������ � �������� � ������������
        // ������� � �������� �����
        if (!(ItemFlags & DIF_EDITOR))
        {
          DialogEdit->SetEditBeyondEnd(FALSE);
          DialogEdit->SetClearFlag(1);
        }

      /* $ 01.08.2000 SVS
         ��� �� ����� ���� DIF_USELASTHISTORY � �������� ������ �����,
         �� ��������������� ������ �������� �� History
      */
      if((ItemFlags&(DIF_HISTORY|DIF_USELASTHISTORY)) == (DIF_HISTORY|DIF_USELASTHISTORY))
      {
        char RegKey[80];
        char *PtrData;
        int PtrLength;
        if((CurItem->Type==DI_EDIT || CurItem->Type==DI_COMBOBOX) && (ItemFlags&DIF_VAREDIT))
        {
          PtrData  =(char *)CurItem->Ptr.PtrData;
          PtrLength=CurItem->Ptr.PtrLength;
        }
        else
        {
          PtrData  =CurItem->Data;
          PtrLength=sizeof(CurItem->Data);
        }
        if(!PtrData[0])
        {
          sprintf(RegKey,fmtSavedDialogHistory,(char*)CurItem->History);
          GetRegKey(RegKey,"Line0",PtrData,"",PtrLength);
        }
      }
      /* SVS $ */
      if((ItemFlags&DIF_MANUALADDHISTORY) && !(ItemFlags&DIF_HISTORY))
        ItemFlags&=~DIF_MANUALADDHISTORY; // ������� �����.

      /* $ 18.03.2000 SVS
         ���� ��� ComBoBox � ������ �� �����������, �� ����� �� ������
         ��� �������, ��� ���� ���� �� ������� ����� Selected != 0
      */
      if (Type==DI_COMBOBOX &&
          (!(ItemFlags&DIF_VAREDIT) && CurItem->Data[0] == 0 ||
            (ItemFlags&DIF_VAREDIT) && *(char*)CurItem->Ptr.PtrData == 0) &&
          CurItem->ListItems)
      {
        struct FarListItem *ListItems=CurItem->ListItems->Items;
        int Length=CurItem->ListItems->ItemsNumber;

        for (J=0; J < Length; J++)
        {
          if(ListItems[J].Flags & LIF_SELECTED)
          {
            // ����� ������ ������ ����� ��� ������� ��������������
            if(ItemFlags&DIF_VAREDIT)
            {
              if(ListItems[J].Flags&LIF_PTRDATA)
                strncpy((char *)CurItem->Ptr.PtrData, ListItems[J].Ptr.PtrData,CurItem->Ptr.PtrLength);
              else
                strncpy((char *)CurItem->Ptr.PtrData, ListItems[J].Text,CurItem->Ptr.PtrLength);
            }
            else
            {
              if(ListItems[J].Flags&LIF_PTRDATA)
                strncpy((char *)CurItem->Data, ListItems[J].Ptr.PtrData,sizeof(CurItem->Data));
              else
                strcpy(CurItem->Data, ListItems[J].Text);
            }
            break;
          }
        }
      }
      /* SVS $ */
      if((CurItem->Type==DI_EDIT || CurItem->Type==DI_COMBOBOX) &&
         (ItemFlags&DIF_VAREDIT))
        DialogEdit->SetString((char *)CurItem->Ptr.PtrData);
      else
        DialogEdit->SetString(CurItem->Data);

      if (Type==DI_FIXEDIT)
        DialogEdit->SetCurPos(0);
    }
    if (Type == DI_USERCONTROL)
    {
      if (!CheckDialogMode(DMODE_CREATEOBJECTS))
        CurItem->ObjPtr=new COORD; // ���� ����������� ��������� ��������� �������
      ((COORD *)(CurItem->ObjPtr))->X=-1;
      ((COORD *)(CurItem->ObjPtr))->Y=-1;
    }

    CurItem->Flags=ItemFlags;
  }
  // ���� ����� ��������, �� ����������� ����� �������.
  SelectOnEntry(FocusPos);

  // ��� ������� �������!
  SetDialogMode(DMODE_CREATEOBJECTS);
  return FocusPos;
}
/* 24.08.2000 SVS $ */


BOOL Dialog::GetItemRect(int I,RECT& Rect)
{
  if(I >= ItemCount)
    return FALSE;

  struct DialogItem *CurItem=&Item[I];
  DWORD ItemFlags=CurItem->Flags;
  int Type=CurItem->Type;

  Rect.left=(int)CurItem->X1;
  Rect.top=(int)CurItem->Y1;
  Rect.right=(int)CurItem->X2;
  Rect.bottom=(int)CurItem->Y2;

  switch(Type)
  {
    case DI_TEXT:
      if (CurItem->X1==(unsigned char)-1)
        Rect.left=(X2-X1+1-((ItemFlags & DIF_SHOWAMPERSAND)?
                                 strlen(CurItem->Data):
                                 HiStrlen(CurItem->Data)))/2;
      if(Rect.left < 0)
        Rect.left=0;

      if (CurItem->Y1==(unsigned char)-1)
        Rect.top=(Y2-Y1+1)/2;

      if(Rect.top < 0)
        Rect.top=0;

      if (ItemFlags & DIF_SEPARATOR)
      {
        Rect.bottom=Rect.top;
        Rect.left=3;
        Rect.right=X2-X1-5; //???
        break;
      }

    case DI_BUTTON:
      Rect.bottom=Rect.top;
      Rect.right=Rect.left+((ItemFlags & DIF_SHOWAMPERSAND)?
                                 strlen(CurItem->Data):
                                 HiStrlen(CurItem->Data));
      break;

    case DI_CHECKBOX:
    case DI_RADIOBUTTON:
      Rect.bottom=Rect.top;
      Rect.right=Rect.left+((ItemFlags & DIF_SHOWAMPERSAND)?
                                 strlen(CurItem->Data):
                                 HiStrlen(CurItem->Data))+
                                 (Type == DI_CHECKBOX?4:
                                   (ItemFlags & DIF_MOVESELECT?3:4)
                                 );
      break;

    case DI_VTEXT:
      Rect.right=Rect.left;
      Rect.bottom=Rect.top+strlen(CurItem->Data);
      break;

    case DI_COMBOBOX:
    case DI_EDIT:
    case DI_FIXEDIT:
    case DI_PSWEDIT:
      Rect.bottom=Rect.top;
      break;
  }
  return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/* Private:
   ��������� ������ � �������� "����������"
*/
void Dialog::DeleteDialogObjects()
{
  int I;
  struct DialogItem *CurItem;

  for (I=0; I < ItemCount; I++)
  {
    if((CurItem=Item+I)->ObjPtr)
      switch(CurItem->Type)
      {
        case DI_EDIT:
        case DI_FIXEDIT:
        case DI_PSWEDIT:
        case DI_COMBOBOX:
          delete (Edit *)(CurItem->ObjPtr);
          break;
        case DI_LISTBOX:
          delete (VMenu *)(CurItem->ObjPtr);
          break;
        case DI_USERCONTROL:
          delete (COORD *)(CurItem->ObjPtr);
          break;
      }
   }
}


//////////////////////////////////////////////////////////////////////////
/* Public:
   ��������� �������� �� ����� ��������������.
   ��� ������������� ����� DIF_HISTORY, ��������� ������ � �������.
*/
void Dialog::GetDialogObjectsData()
{
  int I;
  struct DialogItem *CurItem;

  for (I=0; I < ItemCount; I++)
  {
    if((CurItem=Item+I)->ObjPtr)
      switch(CurItem->Type)
      {
        case DI_EDIT:
        case DI_FIXEDIT:
        case DI_PSWEDIT:
        case DI_COMBOBOX:
        {
          char *PtrData;
          int PtrLength;
          Edit *EditPtr=(Edit *)(CurItem->ObjPtr);
          // ���������� ������
          if((CurItem->Type==DI_EDIT || CurItem->Type==DI_COMBOBOX) &&
             (CurItem->Flags&DIF_VAREDIT))
          {
            PtrData  =(char *)CurItem->Ptr.PtrData;
            PtrLength=CurItem->Ptr.PtrLength;
          }
          else
          {
            PtrData  =CurItem->Data;
            PtrLength=sizeof(CurItem->Data);
          }

          // ������� ������
          EditPtr->GetString(PtrData,PtrLength);

          if (ExitCode>=0 &&
              (CurItem->Flags & DIF_HISTORY) &&
              !(CurItem->Flags & DIF_MANUALADDHISTORY) && // ��� ������� �� ���������
              CurItem->History &&
              Opt.DialogsEditHistory)
            AddToEditHistory(PtrData,CurItem->History,PtrLength);
          /* $ 01.08.2000 SVS
             ! � History ������ ��������� �������� (��� DIF_EXPAND...) �����
              ����������� �����!
          */
          /*$ 05.07.2000 SVS $
          �������� - ���� ������� ������������ ���������� ���������� �����?
          �.�. ������� GetDialogObjectsData() ����� ���������� ��������������
          �� ���� ���������!*/
          /* $ 04.12.2000 SVS
            ! ��� DI_PSWEDIT � DI_FIXEDIT ��������� DIF_EDITEXPAND �� �����
             (DI_FIXEDIT ����������� ��� ������ ���� ���� �����)
          */
          if((CurItem->Flags&DIF_EDITEXPAND) &&
              CurItem->Type != DI_PSWEDIT &&
              CurItem->Type != DI_FIXEDIT)
             ExpandEnvironmentStr(PtrData, PtrData,PtrLength-1);
          /* SVS $ */
          /* SVS $ */
          /* 01.08.2000 SVS $ */
          break;
        }

        case DI_LISTBOX:
        {
          VMenu *VMenuPtr=(VMenu *)(CurItem->ObjPtr);
          CurItem->ListPos=VMenuPtr->GetSelectPos();
          break;
        }

        /**/
      }
   }
}


//////////////////////////////////////////////////////////////////////////
/* $ 22.08.2000 SVS
  ! ShowDialog - �������������� �������� - ����� ������� ������������
*/
/* Private:
   ��������� ��������� ������� �� ������.
*/
void Dialog::ShowDialog(int ID)
{
  struct DialogItem *CurItem;
  int X,Y;
  int I,DrawItemCount;
  unsigned long Attr;

  /* $ 18.08.2000 SVS
     ���� �� ��������� ���������, �� ����������.
  */
  if(IsEnableRedraw ||                 // ��������� ���������� ?
     (ID+1 > ItemCount) ||             // � ����� � ������ ������������?
     CheckDialogMode(DMODE_DRAWING) || // ������ ��������?
     !CheckDialogMode(DMODE_INITOBJECTS))
    return;
  /* SVS $ */

  SetDialogMode(DMODE_DRAWING);  // ������ ��������!!!

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  if(ID == -1) // ������ ���?
  {
    /* $ 28.07.2000 SVS
       ����� ����������� ������� �������� ��������� � ����������
    */
    if(!DlgProc((HANDLE)this,DN_DRAWDIALOG,0,0))
    {
      SkipDialogMode(DMODE_DRAWING);  // ����� ��������� �������!!!
      return;
    }
    /* SVS $ */

    /* $ 28.07.2000 SVS
       ����� ����������� �������� ���� �������...
    */
    Attr=DlgProc((HANDLE)this,DN_CTLCOLORDIALOG,0,
        CheckDialogMode(DMODE_WARNINGSTYLE) ? COL_WARNDIALOGTEXT:COL_DIALOGTEXT);
    SetScreen(X1,Y1,X2,Y2,' ',Attr);
    /* SVS $ */
  }

  if(ID == -1) // ������ ���?
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

    if(CurItem->Flags&DIF_HIDDEN)
      continue;

    /* $ 28.07.2000 SVS
       ����� ����������� ������� �������� �������� ���������
       ����������� ������� SendDlgMessage - � ��� �������� ���!
    */
    if(!Dialog::SendDlgMessage((HANDLE)this,DN_DRAWDLGITEM,I,0))
       continue;
    /* SVS $ */
    /* $ 28.07.2000 SVS
       ����� ����������� ������� �������� ������� ������� �������� ���������
    */
    switch(CurItem->Type)
    {
/* ***************************************************************** */
      case DI_USERCONTROL:
        if(CurItem->VBuf)
        {
          PutText(X1+CurItem->X1,Y1+CurItem->Y1,X1+CurItem->X2,Y1+CurItem->Y2,CurItem->VBuf);
          // �� ������� ����������� ������, ���� �� ���������������.
          if(((COORD *)(CurItem->ObjPtr))->X != -1 &&
             ((COORD *)(CurItem->ObjPtr))->Y != -1 &&
             FocusPos == I)
          {
             MoveCursor(
                ((COORD *)(CurItem->ObjPtr))->X+CurItem->X1+X1,
                ((COORD *)(CurItem->ObjPtr))->Y+CurItem->Y1+Y1
             );
          }
        }
        break; //��� ����������� :-)))

/* ***************************************************************** */
      case DI_SINGLEBOX:
      case DI_DOUBLEBOX:
      {
        Attr=MAKELONG(
          MAKEWORD(FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                      ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGBOXTITLE):
                      ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGBOXTITLE)), // Title LOBYTE
                 FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                      ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGHIGHLIGHTTEXT):
                      ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGHIGHLIGHTTEXT))),// HiText HIBYTE
          MAKEWORD(FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                  ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGBOX):
                  ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGBOX)), // Box LOBYTE
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
          //HiText(Title,HIBYTE(LOWORD(Attr)));
          if (CurItem->Flags & DIF_SHOWAMPERSAND)
            Text(Title);
          else
            HiText(Title,HIBYTE(LOWORD(Attr)));
        }
        break;
      }

/* ***************************************************************** */
      case DI_TEXT:
      {
        if (CurItem->X1==(unsigned char)-1)
          X=(X2-X1+1-((CurItem->Flags & DIF_SHOWAMPERSAND)?HiStrlen(CurItem->Data):strlen(CurItem->Data)))/2;
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
            Attr=CheckDialogMode(DMODE_WARNINGSTYLE) ?
                  ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGBOX):
                  ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGBOX);
          else
            Attr=CheckDialogMode(DMODE_WARNINGSTYLE) ?
                  ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGTEXT):
                  ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGTEXT);

        Attr=MAKELONG(
           MAKEWORD(FarColorToReal(Attr),
                   FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                      ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGHIGHLIGHTTEXT):
                      ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGHIGHLIGHTTEXT))), // HIBYTE HiText
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
          Attr=CheckDialogMode(DMODE_WARNINGSTYLE) ?
                   ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGBOX):
                   ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGBOX);
        else
          if (CurItem->Flags & DIF_SETCOLOR)
            Attr=(CurItem->Flags & DIF_COLORMASK);
          else
            Attr=(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                   ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGTEXT):
                   ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGTEXT));

        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,FarColorToReal(Attr));
        SetColor(Attr&0xFF);
        GotoXY(X1+CurItem->X1,Y1+CurItem->Y1);
        VText(CurItem->Data);
        break;
      }

/* ***************************************************************** */
      /* $ 18.07.2000 SVS
         + ��������� �������� DI_COMBOBOX
      */
      case DI_EDIT:
      case DI_FIXEDIT:
      case DI_PSWEDIT:
      case DI_COMBOBOX:
      {
        Edit *EditPtr=(Edit *)(CurItem->ObjPtr);
        if(!EditPtr)
          break;

        /* $ 15.08.2000 SVS
           ! ��� DropDownList ����� ������������ �� �����
        */
        /* $ 30.08.2000 SVS
           ! "�����, ������ ��, ��� �� �����������" :-)
        */
        Attr=EditPtr->GetObjectColor();
        if(CurItem->Type == DI_COMBOBOX && (CurItem->Flags & DIF_DROPDOWNLIST))
        {
          DWORD AAA=Attr&0xFF;
          Attr=MAKEWORD(FarColorToReal(AAA),
                 FarColorToReal((!CurItem->Focus)?
                  (CheckDialogMode(DMODE_WARNINGSTYLE) ?
                    ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGEDITDISABLED:COL_WARNDIALOGEDIT):
                    ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGEDITDISABLED:COL_DIALOGEDIT)
                  ):
                  (CheckDialogMode(DMODE_WARNINGSTYLE) ?
                    ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGEDITDISABLED:COL_DIALOGEDITSELECTED):
                    ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGEDITDISABLED:COL_DIALOGEDITSELECTED)
                  )
                 )
                );
          Attr=MAKELONG(Attr, // EditLine (Lo=Color, Hi=Selected)
            MAKEWORD(FarColorToReal(AAA), // EditLine - UnChanched Color
            FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
               ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGTEXT):
               ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGTEXT)
              )) // HistoryLetter
           );
        }
        else
        {
          Attr=MAKEWORD(FarColorToReal(Attr&0xFF),
            (CheckDialogMode(DMODE_WARNINGSTYLE) ?
              FarColorToReal(((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGEDITDISABLED:COL_WARNDIALOGEDIT)):
              FarColorToReal(((CurItem->Flags&DIF_DISABLE)?COL_DIALOGEDITDISABLED:COL_DIALOGEDITSELECTED))
            )
          );
          Attr=MAKELONG(Attr, // EditLine (Lo=Color, Hi=Selected)
             MAKEWORD(FarColorToReal(EditPtr->GetObjectColorUnChanged()), // EditLine - UnChanched Color
             FarColorToReal(((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGTEXT)) // HistoryLetter
             ));
        }
        /* SVS $ */
        /* SVS $ */
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);

        EditPtr->SetObjectColor(Attr&0xFF,HIBYTE(LOWORD(Attr)),LOBYTE(HIWORD(Attr)));

        if (CurItem->Focus)
        {
          /* $ 09.08.2000 KM
             ���������� ��������� ������� ��� ����������� �������
          */
          if (!CheckDialogMode(DMODE_DRAGGED))
            SetCursorType(1,-1);
          SelectOnEntry(I);
          EditPtr->Show();
          /* KM $ */
        }
        else
        {
          EditPtr->FastShow();
          EditPtr->SetLeftPos(0);
        }

        /* $ 09.08.2000 KM
           ���������� ��������� ������� ��� ����������� �������
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
          Text(EditX2+1,EditY1,HIBYTE(HIWORD(Attr)),"\x19");
        }
        break;
        /* SVS $ */
      }

/* ***************************************************************** */
      /* $ 01.08.2000 SVS
         ������� ListBox
      */
      case DI_LISTBOX:
      {
        VMenu *ListBox=(VMenu *)(CurItem->ObjPtr);
        if(ListBox)
        {
          /* $ 21.08.2000 SVS
             ����� ���������� ������� �� ��������� �������� ���������
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
          Attr=(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                  ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGTEXT):
                  ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGTEXT));

        Attr=MAKEWORD(FarColorToReal(Attr),
             FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                   ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGHIGHLIGHTTEXT):
                   ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGHIGHLIGHTTEXT))); // HiText
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);

        SetColor(Attr&0xFF);

        GotoXY(X1+CurItem->X1,Y1+CurItem->Y1);

        if (CurItem->Type==DI_CHECKBOX)
          mprintf("[%c] ",CurItem->Selected ?
             (((CurItem->Flags&DIF_3STATE) && CurItem->Selected == 2)?
                *CheckBox3State:'x'):' ');
        else
          if (CurItem->Flags & DIF_MOVESELECT)
            mprintf(" %c ",CurItem->Selected ? '\07':' ');
          else
            mprintf("(%c) ",CurItem->Selected ? '\07':' ');

        if (CurItem->Flags & DIF_SHOWAMPERSAND)
          Text(CurItem->Data);
        else
          HiText(CurItem->Data,HIBYTE(LOWORD(Attr)));

        if (CurItem->Focus)
        {
          /* $ 09.08.2000 KM
             ���������� ��������� ������� ��� ����������� �������
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
           + DI_BUTTON ���� ������ ����� ����� DIF_SETCOLOR
        */
        if (CurItem->Focus)
        {
          SetCursorType(0,10);
          Attr=MAKEWORD(
             (CurItem->Flags & DIF_SETCOLOR)?(CurItem->Flags & DIF_COLORMASK):
               FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                   ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGSELECTEDBUTTON):
                   ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGSELECTEDBUTTON)), // TEXT
             FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                   ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGHIGHLIGHTSELECTEDBUTTON):
                   ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGHIGHLIGHTSELECTEDBUTTON))); // HiText
        }
        else
        {
          Attr=MAKEWORD(
             (CurItem->Flags & DIF_SETCOLOR)?(CurItem->Flags & DIF_COLORMASK):
               FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                      ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGBUTTON):
                      ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGBUTTON)), // TEXT
             FarColorToReal(CheckDialogMode(DMODE_WARNINGSTYLE) ?
                      ((CurItem->Flags&DIF_DISABLE)?COL_WARNDIALOGDISABLED:COL_WARNDIALOGHIGHLIGHTBUTTON):
                      ((CurItem->Flags&DIF_DISABLE)?COL_DIALOGDISABLED:COL_DIALOGHIGHLIGHTBUTTON))); // HiText
        }
        /* SVS $ */
        Attr=DlgProc((HANDLE)this,DN_CTLCOLORDLGITEM,I,Attr);
        SetColor(Attr&0xFF);
        if (CurItem->Flags & DIF_SHOWAMPERSAND)
          Text(CurItem->Data);
        else
          HiText(CurItem->Data,HIBYTE(LOWORD(Attr)));
        break;
      }

/* ***************************************************************** */
    } // end switch(...
    /* 28.07.2000 SVS $ */
  } // end for (I=...

  /* $ 31.07.2000 SVS
     ������� ��������� �����������...
  */
  if ( CheckDialogMode(DMODE_DRAGGED) ) // ���� ������ ���������
  {
    DlgProc((HANDLE)this,DN_DRAWDIALOG,1,0);
  }
  /* SVS $ */

  SkipDialogMode(DMODE_DRAWING);  // ����� ��������� �������!!!
  SetDialogMode(DMODE_SHOW); // ������ �� ������!
}
/* SVS 22.08.2000 $ */


//////////////////////////////////////////////////////////////////////////
/* Public, Virtual:
   ��������� ������ �� ����������.
   ����������� BaseInput::ProcessKey.
*/
int Dialog::ProcessKey(int Key)
{
  int I,J;
  char Str[1024];
  char *PtrStr;
  Edit *CurEditLine;

  if (Key==KEY_NONE || Key==KEY_IDLE)
  {
    /* $ 28.07.2000 SVS
       ��������� ���� ���� � ���������� :-)
    */
    DlgProc((HANDLE)this,DN_ENTERIDLE,0,0);
    /* SVS $ */
    return(FALSE);
  }

  /* $ 31.07.2000 tran
     + ����������� ������� �� ������ */
  if (CheckDialogMode(DMODE_DRAGGED)) // ���� ������ ���������
  {
    int rr=1;
    /* $ 15.12.2000 SVS
       ��� ����������� ������� ��������� ��������� "�����������" ����.
    */
    switch (Key)
    {
        case KEY_CTRLLEFT:
        case KEY_CTRLHOME:
        case KEY_HOME:
            rr=Key == KEY_CTRLLEFT?10:X1;
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
        case KEY_CTRLEND:
        case KEY_END:
            rr=Key == KEY_CTRLRIGHT?10:abs(X1-(ScrX - (X2-X1+1)))+1;
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
        case KEY_CTRLPGUP:
        case KEY_CTRLUP:
            rr=Key == KEY_CTRLUP?5:Y1;
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
        case KEY_CTRLDOWN:
        case KEY_CTRLPGDN:
        case KEY_PGDN:
            rr=Key == KEY_CTRLDOWN? 5: abs(Y1-(ScrY - (Y2-Y1+1)))+1;
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
            SkipDialogMode(DMODE_DRAGGED); // �������� ��������!
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
    /* SVS $ */
    if(CheckDialogMode(DMODE_ALTDRAGGED))
    {
      SkipDialogMode(DMODE_DRAGGED|DMODE_ALTDRAGGED);
      Show();
    }
    return (TRUE);
  }
  /* $ 10.08.2000 SVS
     �������, ���� ���������! (IsCanMove)
  */
  if (Key == KEY_CTRLF5 && CheckDialogMode(DMODE_ISCANMOVE))
  /* SVS 10.08.2000 $*/
  {
    // �������� ���� � ���������� ����������
    SetDialogMode(DMODE_DRAGGED);
    OldX1=X1; OldX2=X2; OldY1=Y1; OldY2=Y2;
    //# GetText(0,0,3,0,LV);
    Show();
    return (TRUE);
  }
  /* tran 31.07.2000 $ */

  // "���� ������� �� ��, ��� ��� ��������..."
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
    if(DlgProc((HANDLE)this,DM_KEY,FocusPos,Key))
      return TRUE;

  // ��������� �����������
  if(Type==DI_CHECKBOX)
  {
    if(!(Item[FocusPos].Flags&DIF_3STATE))
    {
      if((Key == KEY_ADD      && !Item[FocusPos].Selected) ||
         (Key == KEY_SUBTRACT &&  Item[FocusPos].Selected))
       Key=KEY_SPACE;
    }
    /*
      ���� else �� �����, �.�. ���� ������� ����� ����������...
    */
  }
  else if(Key == KEY_ADD)
    Key='+';
  else if(Key == KEY_SUBTRACT)
    Key='-';
  else if(Key == KEY_MULTIPLY)
    Key='*';

  if(Type == DI_LISTBOX)
  {
    switch(Key)
    {
      case KEY_HOME:
      case KEY_LEFT:
      case KEY_RIGHT:
      case KEY_UP:
      case KEY_DOWN:
      case KEY_PGUP:
      case KEY_PGDN:
        VMenu *List=(VMenu *)Item[FocusPos].ObjPtr;
        int CurListPos=List->GetSelectPos();
        int CheckedListItem=List->GetSelection(-1);
        List->ProcessKey(Key);
        int NewListPos=List->GetSelectPos();
        if(NewListPos != CurListPos)
          if(!DlgProc((HANDLE)this,DN_LISTCHANGE,FocusPos,NewListPos))
          {
            List->SetSelection(CheckedListItem,CurListPos);
            ShowDialog(FocusPos);
          }
        return(TRUE);
    }
  }

  switch(Key)
  {
    case KEY_F1:
      /* $ 28.07.2000 SVS
         ����� ������� ������� �������� ��������� � ����������
         � ���� ������� ��� ����, �� ������� ���������
      */
      PtrStr=(char*)DlgProc((HANDLE)this,DN_HELP,FocusPos,(long)&HelpTopic[0]);
      if(PtrStr && *PtrStr)
      {
        /* $ 31.08.2000 SVS
           - ���� � ������� ������ ������.
        */
        if(PluginNumber != -1)
        {
          /* $ 29.08.2000 SVS
             ! ��� ������� ���� ������ �� �������� ���������...
               ������, ����� ����� ����������� �������!
          */
          if (*PtrStr==':')       // Main Topic?
            strcpy(Str,PtrStr+1);
          else if (*PtrStr=='#')  // ��� ������������?
            strcpy(Str,PtrStr);
          else                    // ���� �����������...
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
// ����� � ������� ������� �������!!!
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
        EndLoop=FALSE; // ������ ���� ����
        return TRUE; // ������ ������ �� ����
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
            ��� ��������� ��������� ������� �������� �������� ���������
            ����������� ������� SendDlgMessage - � ��� �������� ���!
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
           ��� ������������, ���� ����� �� ������
        */
        Item[FocusPos].Selected=1;
        // ��������� - "������ ��������"
        Dialog::SendDlgMessage((HANDLE)this,DN_BTNCLICK,FocusPos,0);
        /* $ 06.12.2000 SVS
           ���� �� ������ ����� � ������ "�� ��� ��������" (DIF_BTNNOCLOSE), ��
           ������������, ����� - ���������� ������� ������.
        */
        if(!CheckDialogMode(DMODE_OLDSTYLE) && (Item[FocusPos].Flags&DIF_BTNNOCLOSE))
          return(TRUE);

        ExitCode=FocusPos;
        EndLoop=TRUE;
        /* SVS $ */
        /* SVS $ */
      }
#if 0
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
#else
      else
      {
        ExitCode=-1;
        for (I=0;I<ItemCount;I++)
          if (Item[I].DefaultButton && !(Item[I].Flags&DIF_BTNNOCLOSE))
          {
            if (!IsEdit(Item[I].Type))
              Item[I].Selected=1;
            ExitCode=I;
          }
      }

      EndLoop=TRUE;
      if (ExitCode==-1)
        ExitCode=FocusPos;
#endif
      return(TRUE);

    case KEY_ESC:
    case KEY_BREAK:
    case KEY_F10:
      EndLoop=TRUE;
      ExitCode=(Key==KEY_BREAK) ? -2:-1;
      return(TRUE);

    /* $ 04.12.2000 SVS
       3-� ��������� ���������
       ��� �������� ���� ������� ������ � ������, ���� �������
       ����� ���� DIF_3STATE
    */
    case KEY_ADD:
    case KEY_SUBTRACT:
    case KEY_MULTIPLY:
      if (Type==DI_CHECKBOX)
      {
        int CHKState=
           (Key == KEY_ADD?1:
            (Key == KEY_SUBTRACT?0:
             ((Key == KEY_MULTIPLY)?2:
              Item[FocusPos].Selected)));
        if(Item[FocusPos].Selected != CHKState)
          if(Dialog::SendDlgMessage((HANDLE)this,DN_BTNCLICK,FocusPos,CHKState))
          {
             Item[FocusPos].Selected=CHKState;
             ShowDialog();
          }
      }
      return(TRUE);
    /* SVS 22.11.2000 $ */

    case KEY_SPACE:
      if (Type==DI_BUTTON)
        return(ProcessKey(KEY_ENTER));
      if (Type==DI_CHECKBOX)
      {
        /* $ 04.12.2000 SVS
           3-� ��������� ���������
        */
        int OldSelected=Item[FocusPos].Selected;

        if(Item[FocusPos].Flags&DIF_3STATE)
          (++Item[FocusPos].Selected)%=3;
        else
          Item[FocusPos].Selected = !Item[FocusPos].Selected;
        /* $ 28.07.2000 SVS
          ��� ��������� ��������� ������� �������� �������� ���������
           ����������� ������� SendDlgMessage - � ��� �������� ���!
        */
        if(!Dialog::SendDlgMessage((HANDLE)this,DN_BTNCLICK,FocusPos,Item[FocusPos].Selected))
          Item[FocusPos].Selected = OldSelected;
        /* SVS $ */
        /* SVS 04.12.2000 $ */
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
            ��� ��������� ��������� ������� �������� �������� ���������
            ����������� ������� SendDlgMessage - � ��� �������� ���!
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
          ��� ��������� ��������� ������� �������� �������� ���������
          ����������� ������� SendDlgMessage - � ��� �������� ���!
        */
        if(!Dialog::SendDlgMessage((HANDLE)this,DN_BTNCLICK,FocusPos,PrevRB))
        {
           // ������ �����, ���� ������������ �� �������...
           Item[PrevRB].Selected=1;
           Item[FocusPos].Selected=0;
        }
        /* SVS $ */
        ShowDialog();
        return(TRUE);
      }
      if (IsEdit(Type))
      {
        /* $ 28.07.2000 SVS
          ��� ��������� ��������� ������� �������� �������� ���������
          ����������� ������� SendDlgMessage - � ��� �������� ���!
        */
        if(((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key))
          Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
        /* SVS $ */
        return(TRUE);
      }
      return(TRUE);

    case KEY_HOME:
      // ��� user-���� ����������
      if(Type == DI_USERCONTROL)
        return TRUE;

      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }

      for (I=0;I<ItemCount;I++)
        if (IsFocused(Item[I].Type))
        {
          ChangeFocus2(FocusPos,I);
          /* $ 28.07.2000 SVS
            ��� ��������� ��������� ������� �������� �������� ���������
            ����������� ������� SendDlgMessage - � ��� �������� ���!
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
      // ��� user-���� ����������
      if(Type == DI_USERCONTROL)
        return TRUE;

      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
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
      // ��� user-���� ����������
      if(Type == DI_USERCONTROL)
        return TRUE;

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
      // ��� user-���� ����������
      if(Type == DI_USERCONTROL)
        return TRUE;

      if (IsEdit(Type))
      {
        ((Edit *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
    case KEY_PGDN:
      // ��� user-���� ����������
      if(Type == DI_USERCONTROL)
        return TRUE;

      if (!(Item[FocusPos].Flags & DIF_EDITOR))
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

    /* $ 27.04.2001 VVM
      + ��������� ������ ����� */
    case KEY_MSWHEEL_UP:
    case KEY_MSWHEEL_DOWN:
    /* VVM $ */
    case KEY_CTRLUP:
    case KEY_CTRLDOWN:
      // ��� user-���� ����������
      if(Type == DI_USERCONTROL)
        return TRUE;

      CurEditLine=((Edit *)(Item[FocusPos].ObjPtr));
      if (IsEdit(Type) &&
           (Item[FocusPos].Flags & DIF_HISTORY) &&
           Opt.DialogsEditHistory &&
           Item[FocusPos].History)
      /* $ 26.07.2000 SVS
         �������� ��, ��� � ������ ����� � ������� ������ �� �������
         ��� ��������� ������� ������ � �������.
      */
      {
        char *PStr=Str;
        int MaxLen=sizeof(Item[FocusPos].Data);
        if(Item[FocusPos].Flags&DIF_VAREDIT)
        {
          MaxLen=Item[FocusPos].Ptr.PtrLength;
          if((PStr=(char*)malloc(MaxLen+1)) == NULL)
            return TRUE;//???
        }
        /* $ 27.04.2001 SVS
           ���%$@#&^%$&$%*%^$*^%$*^%$*^%$&*
           ����: sizeof(MaxLen) ;-( - ��� ���� ������ ������.
        */
        CurEditLine->GetString(PStr,MaxLen);
        /* SVS $ */
        SelectFromEditHistory(CurEditLine,Item[FocusPos].History,PStr,MaxLen);
        Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
        if(Item[FocusPos].Flags&DIF_VAREDIT)
          free(PStr);
      }
      /* SVS $ */
      /* $ 18.07.2000 SVS
         + ��������� DI_COMBOBOX - ����� �� ������!
      */
      else if(Type == DI_COMBOBOX && Item[FocusPos].ListItems)
      {
        char *PStr=Str;
        int MaxLen=sizeof(Item[FocusPos].Data);
        if(Item[FocusPos].Flags&DIF_VAREDIT)
        {
          MaxLen=Item[FocusPos].Ptr.PtrLength;
          if((PStr=(char*)malloc(MaxLen+1)) == NULL)
            return TRUE;//???
        }
        CurEditLine->GetString(PStr,MaxLen);
        SelectFromComboBox(CurEditLine,
                      Item[FocusPos].ListItems,PStr,MaxLen);
        Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
        if(Item[FocusPos].Flags&DIF_VAREDIT)
          free(PStr);
      }
      /* SVS $ */
      return(TRUE);

    default:
      // ��� user-���� ����������
      if(Type == DI_USERCONTROL)
        return TRUE;

      /* $ 01.08.2000 SVS
         ������� ListBox
      */
      if(Type == DI_LISTBOX)
      {
        ((VMenu *)(Item[FocusPos].ObjPtr))->ProcessKey(Key);
        return(TRUE);
      }
      /* SVS $ */

      /* $ 21.08.2000 SVS
         Autocomplete ��� ���������� ������ � ������� ����������� ;-)
      */
      if (IsEdit(Type))
      {
        Edit *edt=(Edit *)Item[FocusPos].ObjPtr;
        int SelStart, SelEnd;

        if(Key == KEY_CTRLL) // �������� ����� ������ RO ��� ���� ����� � ����������
          return TRUE;

        /* $ 11.09.2000 SVS
           Ctrl-U � ������� ����� ������� ������� �����
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
              ���������� �������� � BackSpace � DIF_EDITOR
            */
            case KEY_BS:
            {
              int CurPos=edt->GetCurPos();
              /* $ 21.11.2000 SVS
                 �� ��������� ��������� ������ � ������������� ���������
              */
              // � ������ ������????
              if(!edt->GetCurPos())
              {
                // � "����" ���� DIF_EDITOR?
                if(FocusPos > 0 && (Item[FocusPos-1].Flags&DIF_EDITOR))
                {
                  // ��������� � ����������� �...
                  Edit *edt_1=(Edit *)Item[FocusPos-1].ObjPtr;
                  edt_1->GetString(Str,sizeof(Str));
                  CurPos=strlen(Str);
                  edt->GetString(Str+CurPos,sizeof(Str)-CurPos);
                  edt_1->SetString(Str);

                  for (I=FocusPos+1;I<ItemCount;I++)
                  {
                    if (Item[I].Flags & DIF_EDITOR)
                    {
                      if (I>FocusPos)
                      {
                        ((Edit *)(Item[I].ObjPtr))->GetString(Str,sizeof(Str));
                        ((Edit *)(Item[I-1].ObjPtr))->SetString(Str);
                      }
                      ((Edit *)(Item[I].ObjPtr))->SetString("");
                    }
                    else // ���, ������  FocusPos ��� ���� ��������� �� DIF_EDITOR
                    {
                      ((Edit *)(Item[I-1].ObjPtr))->SetString("");
                      break;
                    }
                  }
                  ProcessKey(KEY_UP);
                  edt_1->SetCurPos(CurPos);
                }
              }
              /* SVS $ */
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
                ��� ��������� ��������� ������� �������� �������� ���������
                ����������� ������� SendDlgMessage - � ��� �������� ���!
              */
              Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
              /* SVS $ */
              ShowDialog();
              return(TRUE);

            case KEY_DEL:
              /* $ 19.07.2000 SVS
                 ! "...� ��������� ������ ���� ������� home shift+end del
                   ���� �� ���������..."
                   DEL � ������, ������� DIF_EDITOR, ������� ��� �����
                   ���������...
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
                    ��� ��������� ��������� ������� �������� �������� ���������
                    ����������� ������� SendDlgMessage - � ��� �������� ���!
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
                     ������ ��������, ���� Del ������ � �������
                     �������, ��� ����� ������
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
           ����� ������� Xlat
        */
        /* $ 04.11.2000 SVS
           �������� �� �������������� �������
        */
        if(Opt.XLat.XLatDialogKey && Key == Opt.XLat.XLatDialogKey ||
           Opt.XLat.XLatAltDialogKey && Key == Opt.XLat.XLatAltDialogKey)
        {
          edt->Xlat();
          Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
          return TRUE;
        }
        /* SVS $ */
        /* SVS $ */

        if (edt->ProcessKey(Key))
        {
          int RedrawNeed=FALSE;
          /* $ 26.07.2000 SVS
             AutoComplite: ���� ���������� DIF_HISTORY
                 � ��������� ��������������!.
          */
          /* $ 04.12.2000 SVS
            �������������� - ����� �� �������� �� ����� ������������ ��������.
          */
          if(!CtrlObject->Macro.IsExecuting() &&
             Opt.AutoComplete && Key < 256 && Key != KEY_BS && Key != KEY_DEL &&
             ((Item[FocusPos].Flags & DIF_HISTORY) || Type == DI_COMBOBOX)
            )
          {
            /* $ 05.12.2000 IS
               ��� ������ � ������� ������ ;)
            */
            int MaxLen=sizeof(Item[FocusPos].Data);
            char *PStr=Str;
            if(Item[FocusPos].Flags & DIF_VAREDIT)
            {
              MaxLen=Item[FocusPos].Ptr.PtrLength;
              if((PStr=(char*)malloc(MaxLen+1)) == NULL)
                return TRUE; //???
            }
            int DoAutoComplete=TRUE;
            int CurPos=edt->GetCurPos();
            edt->GetString(PStr,MaxLen);
            int len=strlen(PStr);
            edt->GetSelection(SelStart,SelEnd);
            if(SelStart < 0 || SelStart==SelEnd)
                SelStart=len;
            else
                SelStart++;

            if(CurPos<SelStart) DoAutoComplete=FALSE;
            if(SelStart<SelEnd && SelEnd<len) DoAutoComplete=FALSE;

            if(Opt.EdOpt.PersistentBlocks)
            {
              if(DoAutoComplete && CurPos <= SelEnd)
              {
                PStr[CurPos]=0;
                edt->Select(CurPos,MaxLen); //select the appropriate text
                edt->DeleteBlock();
                edt->FastShow();
              }
            }
            /* IS $ */

            SelEnd=strlen(PStr);

            //find the string in the list
            /* $ 03.12.2000 IS
                 ��������� ���� DoAutoComplete
            */
            if (DoAutoComplete && FindInEditForAC(Type == DI_COMBOBOX,
                         (void *)Item[FocusPos].Selected,PStr,MaxLen))
            /* IS $ */
            {
//SysLog("Coplete: Str=%s SelStart=%d SelEnd=%d CurPos=%d",Str,SelStart,SelEnd, CurPos);
              edt->SetString(PStr);
              edt->Select(SelEnd,MaxLen); //select the appropriate text
              //edt->Select(CurPos,sizeof(Str)); //select the appropriate text
              /* $ 01.08.2000 SVS
                 ��������� ������ � AutoComplete
              */
              edt->SetCurPos(CurPos); // SelEnd
              RedrawNeed=TRUE;
            }
            if(Item[FocusPos].Flags & DIF_VAREDIT)
              free(PStr);
          }
          /* SVS 03.12.2000 $ */
          Dialog::SendDlgMessage((HANDLE)this,DN_EDITCHANGE,FocusPos,0);
          /* SVS $ */
          if(RedrawNeed)
            Redraw(); // ����������� ������ ���� ����� DN_EDITCHANGE (imho)
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
   ��������� ������ �� "����".
   ����������� BaseInput::ProcessMouse.
*/
/* $ 18.08.2000 SVS
   + DN_MOUSECLICK
*/
int Dialog::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  int I, J;
  int MsX,MsY;
  int Type;
  RECT Rect;

  if (MouseEvent->dwButtonState==0)
    return(FALSE);
  if(!CheckDialogMode(DMODE_SHOW))
    return FALSE;

  MsX=MouseEvent->dwMousePosition.X;
  MsY=MouseEvent->dwMousePosition.Y;

  if (MsX<X1 || MsY<Y1 || MsX>X2 || MsY>Y2)
  {
    if(!DlgProc((HANDLE)this,DN_MOUSECLICK,-1,(long)MouseEvent))
    {
      if (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
        ProcessKey(KEY_ESC);
      else if (MouseEvent->dwButtonState & RIGHTMOST_BUTTON_PRESSED)
        ProcessKey(KEY_ENTER);
    }
    return(TRUE);
  }

//SysLog("Ms (%d,%d)",MsX,MsY);
  if (MouseEvent->dwEventFlags==0 || MouseEvent->dwEventFlags==DOUBLE_CLICK)
  {
    // ������ ���� - ��� �� ����������� �����.
    for (I=0; I < ItemCount;I++)
    {
      if(Item[I].Flags&(DIF_DISABLE|DIF_HIDDEN))
        continue;

      GetItemRect(I,Rect);
      Rect.left+=X1;  Rect.top+=Y1;
      Rect.right+=X1; Rect.bottom+=Y1;
//SysLog("? %2d) Rect (%2d,%2d) (%2d,%2d) '%s'",I,Rect.left,Rect.top,Rect.right,Rect.bottom,Item[I].Data);

      if(MsX >= Rect.left && MsY >= Rect.top && MsX <= Rect.right && MsY <= Rect.bottom)
      {
//SysLog("+ %2d) Rect (%2d,%2d) (%2d,%2d) '%s'",I,Rect.left,Rect.top,Rect.right,Rect.bottom,Item[I].Data);
        // ��� ���������� :-)
        if(Item[I].Type == DI_SINGLEBOX || Item[I].Type == DI_DOUBLEBOX)
        {
          // ���� �� �����, ��...
          if(((MsX == Rect.left || MsX == Rect.right) && MsY >= Rect.top && MsY <= Rect.bottom) || // vert
             ((MsY == Rect.top  || MsY == Rect.bottom) && MsX >= Rect.left && MsX <= Rect.right) )   // hor
          {
            if(DlgProc((HANDLE)this,DN_MOUSECLICK,I,(long)MouseEvent))
              return TRUE;
          }
          else
            continue;
        }

        if(Item[I].Type == DI_USERCONTROL)
        {
          // ��� user-���� ���������� ���������� ����
          MouseEvent->dwMousePosition.X-=Rect.left;
          MouseEvent->dwMousePosition.Y-=Rect.top;
        }

        if(DlgProc((HANDLE)this,DN_MOUSECLICK,I,(long)MouseEvent))
          return TRUE;

        if(Item[I].Type == DI_USERCONTROL)
        {
           ChangeFocus2(FocusPos,I);
           ShowDialog();
           return(TRUE);
        }
        break;
      }
    }

    if((MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED))
    {
      for (I=0;I<ItemCount;I++)
      {
        /* $ 04.12.2000 SVS
           ��������� �� ������ ����������� � ���� ����������� ��������
        */
        if(Item[I].Flags&(DIF_DISABLE|DIF_HIDDEN))
          continue;
        /* SVS $ */
        Type=Item[I].Type;
        if (MsX>=X1+Item[I].X1)
        {
          /* $ 01.08.2000 SVS
             ������� ListBox
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
               + ������� ���, ����� ����� ������ � DropDownList
                 ������ ����������� ���.
               ���� ��������� ���������� - ����� ������ ������� � ��
               ����� ������������ �� ������ �������, �� ������ �����������
               �� �������� ��������� �� ��������� ������� ������� �� ����������
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
                 + �������� �� ��� �������� DI_COMBOBOX
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
      // ��� MOUSE-�����������:
      //   ���� �������� � ��� ������, ���� ���� �� ������ �� �������� ��������
      //
      /* $ 10.08.2000 SVS
         �������, ���� ���������! (IsCanMove)
      */
      if (CheckDialogMode(DMODE_ISCANMOVE))
      {
        /* $ 03.08.2000 tran
           �� ��� �������� - �� ����� ���������� */
        //SetDialogMode(DMODE_DRAGGED);
        OldX1=X1; OldX2=X2; OldY1=Y1; OldY2=Y2;
        // �������� delta ����� �������� � Left-Top ����������� ����
        MsX=abs(X1-MouseX);
        MsY=abs(Y1-MouseY);
        while (1)
        {
            int mb=IsMouseButtonPressed();
            /* $ 15.12.2000 SVS
               ����� ������ �������� �����������
            */
            int mx,my,X0,Y0;
            if ( mb==1 ) // left key, still dragging
            {
                Hide();
                X0=X1;
                Y0=Y1;
                if(MouseX==PrevMouseX)
                  mx=X1;
                else
                  mx=MouseX-MsX;
                if(MouseY==PrevMouseY)
                  my=Y1;
                else
                  my=MouseY-MsY;

                if(mx >= 0 && mx+(X2-X1)<=ScrX)
                {
                  X2=mx+(X2-X1);
                  X1=mx;
                  AdjustEditPos(X1-X0,0); //?
                }
                if(my >= 0 && my+(Y2-Y1)<=ScrY)
                {
                  Y2=my+(Y2-Y1);
                  Y1=my;
                  AdjustEditPos(0,Y1-Y0); //?
                }
                Show();
            }
            /* SVS $ */
            else if (mb==2) // right key, abort
            {
                Hide();
                AdjustEditPos(OldX1-X1,OldY1-Y1);
                X1=OldX1;
                X2=OldX2;
                Y1=OldY1;
                Y2=OldY2;
                SkipDialogMode(DMODE_DRAGGED);
                Show();
                break;
            }
            else  // release key, drop dialog
            {
                SkipDialogMode(DMODE_DRAGGED);
                Show();
                break;
            }
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
   �������� ����� ����� (����������� ���������
     KEY_TAB, KEY_SHIFTTAB, KEY_UP, KEY_DOWN,
   � ��� �� Alt-HotKey)
*/
/* $ 28.07.2000 SVS
   ������� ��� ��������� DN_KILLFOCUS & DN_SETFOCUS
*/
/* $ 24.08.2000 SVS
   ������� ��� DI_USERCONTROL
*/
int Dialog::ChangeFocus(int FocusPos,int Step,int SkipGroup)
{
  int Type,OrigFocusPos=FocusPos;
//  int FucusPosNeed=-1;
  // � ������� ��������� ������� ����� �������� ���������,
  //   ��� ������� - LostFocus() - ������ ����� �����.
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

      if(!(Item[FocusPos].Flags&(DIF_NOFOCUS|DIF_DISABLE|DIF_HIDDEN)))
      {
        if (Type==DI_LISTBOX || Type==DI_BUTTON || Type==DI_CHECKBOX || IsEdit(Type) || Type==DI_USERCONTROL)
          break;
        if (Type==DI_RADIOBUTTON && (!SkipGroup || Item[FocusPos].Selected))
          break;
      }
      // ������� ������������ � ����������� ����������� :-)
      if(OrigFocusPos == FocusPos)
        break;
    }
  }

//  Dialog::FocusPos=FocusPos;
  // � ������� ��������� ������� ����� �������� ���������,
  //   ��� ������� GotFocus() - ������� ����� �����.
  // ���������� ������������ �������� ������� ��������
//  if(CheckDialogMode(DMODE_INITOBJECTS))
//    DlgProc((HANDLE)this,DN_GOTFOCUS,FocusPos,0);
  return(FocusPos);
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   Private:
   �������� ����� ����� ����� ����� ����������.
   ������� �������� � ���, ����� ���������� DN_KILLFOCUS & DM_SETFOCUS
*/
int Dialog::ChangeFocus2(int KillFocusPos,int SetFocusPos)
{
  int FucusPosNeed=-1;
  if(!(Item[SetFocusPos].Flags&(DIF_NOFOCUS|DIF_DISABLE|DIF_HIDDEN)))
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
  ������� SelectOnEntry - ��������� ������ ��������������
  ��������� ����� DIF_SELECTONENTRY
*/
void Dialog::SelectOnEntry(int Pos)
{
  if(IsEdit(Item[Pos].Type) &&
     (Item[Pos].Flags&DIF_SELECTONENTRY)
//     && PrevFocusPos != -1 && PrevFocusPos != Pos
    )
  {
    Edit *edt=(Edit *)Item[Pos].ObjPtr;
    if(edt)
      edt->Select(0,edt->GetLength());
  }
}
/* SVS $ */

/* $ 04.12.2000 SVS
   ! ����������� ������� ConvertItem() � DataToItem() - � �����������
     ����� ���������� ���������� � ������� ��� (MSVC - ��� ��� ������ :-(
*/

//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   Public, Static:
   + ������� ConvertItem - �������� �������������� ��������� ������� ��
   ����������� ������������� �� �������
*/
void Dialog::ConvertItem(int FromPlugin,
                         struct FarDialogItem *Item,struct DialogItem *Data,
                         int Count,BOOL InternalCall)
{
  int I;
  if(!Item || !Data)
    return;

  char *PtrData;
  int PtrLength;
  Edit *EditPtr;

  if(FromPlugin == CVTITEM_TOPLUGIN)
    for (I=0; I < Count; I++, ++Item, ++Data)
    {
      Item->Type=Data->Type;
      Item->X1=Data->X1;
      Item->Y1=Data->Y1;
      Item->X2=Data->X2;
      Item->Y2=Data->Y2;
      Item->Focus=Data->Focus;
      Item->Selected=Data->Selected;
      Item->Flags=Data->Flags;
      Item->DefaultButton=Data->DefaultButton;
      if(InternalCall)
      {
        if(Dialog::IsEdit(Data->Type) && (EditPtr=(Edit *)(Data->ObjPtr)) != NULL)
        {
          // �������� ��������
          if((Data->Type==DI_EDIT || Data->Type==DI_COMBOBOX) &&
             (Data->Flags&DIF_VAREDIT))
          {
            PtrData  =(char *)Data->Ptr.PtrData;
            PtrLength=Data->Ptr.PtrLength;
          }
          else
          {
            PtrData  =Data->Data;
            PtrLength=sizeof(Data->Data);
          }
          EditPtr->GetString(PtrData,PtrLength);
        }
      }
      {
          TRY{
            memmove(Item->Data,Data->Data,sizeof(Item->Data));
          }
          __except (EXCEPTION_EXECUTE_HANDLER)
          {
            ;
          }
      }
    }
  else
    for (I=0; I < Count; I++, ++Item, ++Data)
    {
      Data->Type=Item->Type;
      Data->X1=Item->X1;
      Data->Y1=Item->Y1;
      Data->X2=Item->X2;
      Data->Y2=Item->Y2;
      Data->Focus=Item->Focus;
      Data->Selected=Item->Selected;
      Data->Flags=Item->Flags;
      Data->DefaultButton=Item->DefaultButton;
      {
         TRY{
           memmove(Data->Data,Item->Data,sizeof(Data->Data));
         }
         __except (EXCEPTION_EXECUTE_HANDLER)
         {
           ;
         }
      }
      /* ���� ����� ����� �������� ����� ���������� ��������.
      �� �������� ������ ������ � ����� �� DN_EDITCHANGE
      if(InternalCall)
      {
        if(Dialog::IsEdit(Data->Type) && (EditPtr=(Edit *)(Data->ObjPtr)) != NULL)
        {
          // �������
          if((Data->Type==DI_EDIT || Data->Type==DI_COMBOBOX) &&
             (Data->Flags&DIF_VAREDIT))
          {
            PtrData  =(char *)Data->Ptr.PtrData;
            PtrLength=Data->Ptr.PtrLength;
          }
          else
          {
            PtrData  =Data->Data;
            PtrLength=sizeof(Data->Data);
          }
          EditPtr->SetString(PtrData);
        }
      }
      */
    }
}
/* SVS $ */

//////////////////////////////////////////////////////////////////////////
/* Public, Static:
   ����������� ������ �� ��������� ������� �� ����������
   �������������. ���������� ������� InitDialogItems (��. "Far PlugRinG
   Russian Help Encyclopedia of Developer")
*/
void Dialog::DataToItem(struct DialogData *Data,struct DialogItem *Item,
                        int Count)
{
  int I;

  if(!Item || !Data)
    return;

  for (I=0;I<Count;I++, ++Item, ++Data)
  {
    Item->Type=Data->Type;
    Item->X1=Data->X1;
    Item->Y1=Data->Y1;
    Item->X2=Data->X2;
    Item->Y2=Data->Y2;
    Item->Focus=Data->Focus;
    Item->Selected=Data->Selected;
    Item->Flags=Data->Flags;
    Item->DefaultButton=Data->DefaultButton;
    if ((unsigned int)Data->Data<MAX_MSG)
      strcpy(Item->Data,MSG((unsigned int)Data->Data));
    else
    {
      TRY{
        memmove(Item->Data,Data->Data,sizeof(Item->Data));
      }
      __except (EXCEPTION_EXECUTE_HANDLER)
      {
        ;
      }
    }
    Item->ObjPtr=NULL;
  }
}
/* SVS 04.12.2000 $ */


//////////////////////////////////////////////////////////////////////////
/* Private:
   ��������� ��� �������� ������� �� ������� ������ �����
   (DI_EDIT, DI_FIXEDIT, DI_PSWEDIT) � � ������ ������ ���������� TRUE
*/
/* $ 18.07.2000 SVS
   ! ������� DI_COMBOBOX ��������� � ��������� ��������� ����������...
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
   �������, ������������ - "����� �� ������� ������� ����� ����� �����"
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
   AutoComplite: ����� ��������� ��������� � �������
*/
/* $ 28.07.2000 SVS
   ! �������� Edit *EditLine ����� �������!
*/
int Dialog::FindInEditForAC(int TypeFind,void *HistoryName,char *FindStr,int MaxLen)
{
  char *Str;
  int I, Count, LenFindStr=strlen(FindStr);

  if(!TypeFind)
  {
    char RegKey[80],KeyValue[80];
    if((Str=(char*)malloc(MaxLen+1)) == NULL)
      return FALSE;
    sprintf(RegKey,fmtSavedDialogHistory,(char*)HistoryName);
    // �������� ������� �������
    for (I=0; I < HISTORY_COUNT; I++)
    {
      sprintf(KeyValue,fmtLine,I);
      GetRegKey(RegKey,KeyValue,Str,"",MaxLen);
      if (!LocalStrnicmp(Str,FindStr,LenFindStr))
        break;
    }
    if (I == HISTORY_COUNT)
    {
      free(Str);
      return FALSE;
    }
    /* $ 28.07.2000 SVS
       ��������� ������� �� �����������, � ��������� �����������.
    */
//SysLog("FindInEditForAC()  FindStr=%s Str=%s",FindStr,&Str[strlen(FindStr)]);
    strncat(FindStr,&Str[LenFindStr],MaxLen-LenFindStr);
    /* SVS $ */
    free(Str);
  }
  else
  {
    struct FarListItem *ListItems=((struct FarList *)HistoryName)->Items;
    int Count=((struct FarList *)HistoryName)->ItemsNumber;

    for (I=0; I < Count ;I++)
    {
      if (!LocalStrnicmp(
        ((ListItems[I].Flags&LIF_PTRDATA)?
            ListItems[I].Ptr.PtrData:
            ListItems[I].Text),
        FindStr,LenFindStr))
        break;
    }
    if (I  == Count)
      return FALSE;

    if(ListItems[I].Flags&LIF_PTRDATA)
    {
      // �������� "������������" - ����� �� ������� �� ������� ������.
      if(ListItems[I].Ptr.PtrLength < LenFindStr)
        strncat(FindStr,&ListItems[I].Ptr.PtrData[LenFindStr],MaxLen-LenFindStr);
    }
    else
    {
      if(sizeof(ListItems[I].Text) < LenFindStr)
        strncat(FindStr,&ListItems[I].Text[LenFindStr],MaxLen-LenFindStr);
    }
  }
  return TRUE;
}
/*  SVS $ */

//////////////////////////////////////////////////////////////////////////
/* Private:
   ��������� ���������� ������ ��� ComboBox
*/
/*
   $ 18.07.2000 SVS
   �������-���������� ������ �� ������ � ���������...
*/
void Dialog::SelectFromComboBox(
         Edit *EditLine,                   // ������ ��������������
         struct FarList *List,    // ������ �����
         char *IStr,
         int MaxLen)
{
  char *Str;
  struct MenuItem ComboBoxItem={0};
  struct FarListItem *ListItems=List->Items;
  int EditX1,EditY1,EditX2,EditY2;
  int I,Dest;

  if((Str=(char*)malloc(MaxLen)) != NULL)
  {
    // �������� ������� ������������� ����
    //  � ������������ ������� ScrollBar
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

    // ���������� ������� ����
    /* ��������� ����� ������ - �������������� - � ��� Tetx[0]
       ������ ���� ����� '\0'
    */
    for (Dest=I=0;I < List->ItemsNumber;I++)
    {
      memset(&ComboBoxItem,0,sizeof(ComboBoxItem));

      /* $ 28.07.2000 SVS
         �������� Selected ��� ������ ���������� ������ ����� � ������
      */

      if(IStr && *IStr && !(ListItems[I].Flags&LIF_DISABLE))
      {
        if((ComboBoxItem.Selected=(!Dest &&
         !strncmp(IStr,
           ((ListItems[I].Flags&LIF_PTRDATA)?ListItems[I].Ptr.PtrData:ListItems[I].Text),
           (ListItems[I].Flags&LIF_PTRDATA)?ListItems[I].Ptr.PtrLength:sizeof(ListItems[I].Text)))?
           TRUE:FALSE) == TRUE)
           Dest++;
      }
      else
         ComboBoxItem.Selected=ListItems[I].Flags&LIF_SELECTED;

      ComboBoxItem.Separator=ListItems[I].Flags&LIF_SEPARATOR;
      ComboBoxItem.Checked=ListItems[I].Flags&LIF_CHECKED;
      ComboBoxItem.Disabled=ListItems[I].Flags&LIF_DISABLE;
      /* 01.08.2000 SVS $ */
      /* SVS $ */
      if(ListItems[I].Flags&LIF_PTRDATA)
      {
        // � ����� ��� ���� ������ � ������ �������?
        if(ListItems[I].Ptr.PtrLength < sizeof(ComboBoxItem.UserData))
        {
          strncpy(ComboBoxItem.Name,ListItems[I].Ptr.PtrData,sizeof(ComboBoxItem.Name)-1);
          strcpy(ComboBoxItem.UserData,ListItems[I].Ptr.PtrData);
        }
        else
        {
          ComboBoxItem.PtrData=ListItems[I].Ptr.PtrData;
          ComboBoxItem.Flags=1;
        }
        ComboBoxItem.UserDataSize=strlen(ListItems[I].Ptr.PtrData);
      }
      else
      {
        strcpy(ComboBoxItem.Name,ListItems[I].Text);
        strcpy(ComboBoxItem.UserData,ListItems[I].Text);
        ComboBoxItem.UserDataSize=strlen(ListItems[I].Text);
      }
      ComboBoxMenu.AddItem(&ComboBoxItem);
    }

    /* $ 28.07.2000 SVS
       ����� ���������� ������� �� ��������� �������� ���������
    */
    short Colors[9];
    ComboBoxMenu.GetColors(Colors);
    if(DlgProc((HANDLE)this,DN_CTLCOLORDLGLIST,
                    sizeof(Colors)/sizeof(Colors[0]),(long)Colors))
      ComboBoxMenu.SetColors(Colors);
    /* SVS $ */

    ComboBoxMenu.Show();

    Dest=ComboBoxMenu.GetSelectPos();
    while (!ComboBoxMenu.Done())
    {
      int Key=ComboBoxMenu.ReadInput();
      // ����� ����� �������� ���-�� ����, ��������,
      I=ComboBoxMenu.GetSelectPos();
      if(I != Dest)
      {
        if(!DlgProc((HANDLE)this,DN_LISTCHANGE,FocusPos,I))
          ComboBoxMenu.SetSelectPos(Dest,Dest<I?-1:1); //????
        else
          Dest=I;
      }
      //  ��������� multiselect ComboBox

      ComboBoxMenu.ProcessInput();
    }

    int ExitCode=ComboBoxMenu.GetExitCode();
    if (ExitCode<0)
      return;
    /* �������� ������� ��������� */
    for (I=0; I < List->ItemsNumber; I++)
      ListItems[I].Flags&=~LIF_SELECTED;
    ListItems[ExitCode].Flags|=LIF_SELECTED;
    ComboBoxMenu.GetUserData(Str,MaxLen,ExitCode);

    EditLine->SetString(Str);
    EditLine->SetLeftPos(0);
    Redraw();
    free(Str);
  }
}
/* SVS $ */

//////////////////////////////////////////////////////////////////////////
/* Private:
   ��������� ���������� ������ �� �������
*/
/* $ 26.07.2000 SVS
  + �������������� �������� � SelectFromEditHistory ��� ���������
   ������ ������� � ������� (���� ��� ������������� ������ �����)
*/
void Dialog::SelectFromEditHistory(Edit *EditLine,
                                   char *HistoryName,
                                   char *IStr,
                                   int MaxLen)
/* SVS $ */
/* $ 21.02.2001 IS
     ��������� �� ������ ������ (����������� �� � ����, �� ����������� ��!)
*/
{
  char RegKey[80],KeyValue[80],*Str[HISTORY_COUNT]={0};
  int I,Dest;
  int Checked;
  int Final=FALSE;

  if(!EditLine)
    return;

  sprintf(RegKey,fmtSavedDialogHistory,HistoryName);
  while(1)
  {
    // �������� ������� ������������� ����
    VMenu HistoryMenu("",NULL,0,8,VMENU_ALWAYSSCROLLBAR);

    struct MenuItem HistoryItem;
    int EditX1,EditY1,EditX2,EditY2;
    int ItemsCount;

    EditLine->GetPosition(EditX1,EditY1,EditX2,EditY2);
    if (EditX2-EditX1<20)
      EditX2=EditX1+20;
    if (EditX2>ScrX)
      EditX2=ScrX;

    memset(&HistoryItem,0,sizeof(HistoryItem));
    HistoryMenu.SetFlags(MENU_SHOWAMPERSAND);
    HistoryMenu.SetPosition(EditX1,EditY1+1,EditX2,0);
    HistoryMenu.SetBoxType(SHORT_SINGLE_BOX);

    // ���������� ������� ����
    ItemsCount=0;
    for (Dest=I=0; I < HISTORY_COUNT; I++)
    {
      if((Str[I]=(char*)malloc(MaxLen+1)) == NULL)
        break;

      memset(&HistoryItem,0,sizeof(HistoryItem));

      sprintf(KeyValue,fmtLine,I);
      GetRegKey(RegKey,KeyValue,Str[I],"",MaxLen);
      if (*Str[I]==0)
        continue;

      sprintf(KeyValue,fmtLocked,I);

      GetRegKey(RegKey,KeyValue,(int)Checked,0);
      HistoryItem.Checked=Checked;
      /* $ 26.07.2000 SVS
         �������� Selected ��� ������ ���������� ������ ����� � �������
      */
      if((HistoryItem.Selected=(!Dest && !strcmp(IStr,Str[I]))?TRUE:FALSE) == TRUE)
         Dest++;
      /* SVS $ */
      strncpy(HistoryItem.Name,Str[I],sizeof(HistoryItem.Name)-1);
      if(MaxLen < sizeof(HistoryItem.UserData))
        strncpy(HistoryItem.UserData,Str[I],sizeof(HistoryItem.UserData));
      else
      {
        HistoryItem.PtrData=Str[I];
        HistoryItem.Flags=1;
      }
      HistoryItem.UserDataSize=strlen(Str[I])+1;
      HistoryMenu.AddItem(&HistoryItem);
      ItemsCount++;
    }
    if (ItemsCount==0)
      {
        Final=TRUE;
        break;
      }

    /* $ 28.07.2000 SVS
       ����� ���������� ������� �� ��������� �������� ���������
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

      // Del ������� ������� ������.
      if (Key==KEY_DEL)
      {
        int Locked;
        for (I=0,Dest=0; I < HISTORY_COUNT;I++)
        {
          sprintf(KeyValue,fmtLine,I);
          GetRegKey(RegKey,KeyValue,Str[I],"",MaxLen);
          DeleteRegValue(RegKey,KeyValue);
          sprintf(KeyValue,fmtLocked,I);
          GetRegKey(RegKey,KeyValue,Locked,0);
          DeleteRegValue(RegKey,KeyValue);

          // ���������� ������ ������� �� ���������
          if (Locked)
          {
            sprintf(KeyValue,fmtLine,Dest);
            SetRegKey(RegKey,KeyValue,Str[I]);
            sprintf(KeyValue,fmtLocked,Dest);
            SetRegKey(RegKey,KeyValue,TRUE);
            Dest++;
          }
        }
        HistoryMenu.Hide();
        SelectFromEditHistory(EditLine,HistoryName,IStr,MaxLen);
        Final=TRUE;
        break;
      }

      // Ins �������� ����� ������� �� ��������.
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

      // Tab � ������ ������� - ������ Enter
      if (Key==KEY_TAB)
      {
        HistoryMenu.ProcessKey(KEY_ENTER);
        continue;
      }

      // ���� ���� �������� DN_LISTCHANGE

      HistoryMenu.ProcessInput();
    }

    if(Final) break;

    int ExitCode=HistoryMenu.GetExitCode();
    if (ExitCode<0)
      {
        Final=TRUE;
        break;
      }

    HistoryMenu.GetUserData(Str[0],MaxLen,ExitCode);

    break;
  }

  if(!Final && Str[0])
  {
    EditLine->SetString(Str[0]);
    EditLine->SetLeftPos(0);
    Redraw();
  }

  for (I=0; I < HISTORY_COUNT; I++)
    if(Str[I])
      free(Str[I]);
}
/* IS $ */

//////////////////////////////////////////////////////////////////////////
/* Private:
   ������ � �������� - ���������� � reorder ������
*/
int Dialog::AddToEditHistory(char *AddStr,char *HistoryName,int MaxLen)
{
  int LastLine=HISTORY_COUNT-1,FirstLine=HISTORY_COUNT, I, Locked;
  char *Str;

  if (*AddStr==0)
    return FALSE;

  if((Str=(char*)malloc(MaxLen+1)) == NULL)
    return FALSE;

  char RegKey[80],SrcKeyValue[80],DestKeyValue[80];
  sprintf(RegKey,fmtSavedDialogHistory,HistoryName);

  for (I=0; I < HISTORY_COUNT; I++)
  {
    sprintf(SrcKeyValue,fmtLocked,I);
    GetRegKey(RegKey,SrcKeyValue,Locked,0);
    if (!Locked)
    {
      FirstLine=I;
      break;
    }
  }

  for (I=0; I < HISTORY_COUNT; I++)
  {
    sprintf(SrcKeyValue,fmtLine,I);
    GetRegKey(RegKey,SrcKeyValue,Str,"",MaxLen);
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
          GetRegKey(RegKey,SrcKeyValue,Str,"",MaxLen);
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
  free(Str);
  return TRUE;
}


//////////////////////////////////////////////////////////////////////////
/* Public, Static:
   �������� �� HotKey
*/
/* $ 20.02.2001 SVS
   ��������� ��������� IsKeyHighlighted � ��������� Alt- ��
   ������� ��� ��������*/
int Dialog::IsKeyHighlighted(char *Str,int Key,int Translate)
{
  if ((Str=strchr(Str,'&'))==NULL)
    return(FALSE);
  int UpperStrKey=LocalUpper(Str[1]);
  /* $ 08.11.2000 SVS
     ������� �������� ����� ������ ��� hotkey (������������ ��������)
  */
  /* 28.12.2000 SVS
    + ��������� ��������� Opt.HotkeyRules */
  if (Key < 256)
  {
    int KeyToKey=LocalKeyToKey(Key);
    return(UpperStrKey == LocalUpper(Key) ||
      Translate &&
      (!Opt.HotkeyRules && UpperStrKey==LocalUpper(KeyToKey) ||
        Opt.HotkeyRules && LocalKeyToKey(UpperStrKey)==KeyToKey));
  }

  if(Key&KEY_ALT)
  {
    int AltKey=Key&(~KEY_ALT);
    if(AltKey < 256)
    {
      if (AltKey >= '0' && AltKey <= '9')
        return(AltKey==UpperStrKey);

      int AltKeyToKey=LocalKeyToKey(AltKey);
      if (AltKey > ' ' && AltKey <= 255)
  //         (AltKey=='-'  || AltKey=='/' || AltKey==','  || AltKey=='.' ||
  //          AltKey=='\\' || AltKey=='=' || AltKey=='['  || AltKey==']' ||
  //          AltKey==':'  || AltKey=='"' || AltKey=='~'))
      {
        return(UpperStrKey==LocalUpper(AltKey) ||
               Translate &&
               (!Opt.HotkeyRules && UpperStrKey==LocalUpper(AltKeyToKey) ||
                  Opt.HotkeyRules && LocalKeyToKey(UpperStrKey)==AltKeyToKey));
      }
    }
  }
  /* SVS $*/
  /* SVS $*/
  return(FALSE);
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* Private:
   ���� �������� Alt-???
*/
int Dialog::ProcessHighlighting(int Key,int FocusPos,int Translate)
{
  int I, Type;
  DWORD Flags;
  for (I=0;I<ItemCount;I++)
  {
    Type=Item[I].Type;
    Flags=Item[I].Flags;

    if ((!IsEdit(Type) || (Type == DI_COMBOBOX && (Flags&DIF_DROPDOWNLIST))) &&
        (Flags & (DIF_SHOWAMPERSAND|DIF_DISABLE|DIF_HIDDEN))==0)
      if (IsKeyHighlighted(Item[I].Data,Key,Translate))
      {
        int DisableSelect=FALSE;

        // ���� ���: Edit(���� �������) � DI_TEXT � ���� ������, ��...
        if (I>0 &&
            Type==DI_TEXT &&                              // DI_TEXT
            IsEdit(Item[I-1].Type) &&                     // � ��������
            Item[I].Y1==Item[I-1].Y1 &&                   // � ��� � ���� ������
            (I+1 < ItemCount && Item[I].Y1!=Item[I+1].Y1)) // ...� ��������� ������� � ������ ������
        {
          if((Item[I-1].Flags&(DIF_DISABLE|DIF_HIDDEN)) != 0) // � �� ����������
             break;
          // ������� � ����������� ����� ��������� ��������� �������
          if(!DlgProc((HANDLE)this,DN_HOTKEY,I,Key))
            break; // ������� �� ���������� ���������...
          I=ChangeFocus(I,-1,FALSE);
          DisableSelect=TRUE;
        }
        else if (Item[I].Type==DI_TEXT      || Item[I].Type==DI_VTEXT ||
                 Item[I].Type==DI_SINGLEBOX || Item[I].Type==DI_DOUBLEBOX)
        {
          if(I+1 < ItemCount && // ...� ��������� �������
            (Item[I+1].Flags&(DIF_DISABLE|DIF_HIDDEN)) != 0) // � �� ����������
             break;
          // ������� � ����������� ����� ��������� ��������� �������
          if(!DlgProc((HANDLE)this,DN_HOTKEY,I,Key))
            break; // ������� �� ���������� ���������...
          I=ChangeFocus(I,1,FALSE);
          DisableSelect=TRUE;
        }
        /* $ 29.08.2000 SVS
           - ������ ����������� �����-��� - ������� ProcessHighlighting
           MY> ������ � ��������� ����� �������. � ���� � ���� ������� �������.
           MY> ������ � ��������� ���� ������, ���� Alt-������ ����� �
           MY> ��������� ������������� �����.

           � ������ ������ ���������� :-)))
        */
        // ������� � ����������� ����� ��������� ��������� �������
        if(!DlgProc((HANDLE)this,DN_HOTKEY,I,Key))
          break; // ������� �� ���������� ���������...
        ChangeFocus2(FocusPos,I);
        /* SVS $ */
        if ((Item[I].Type==DI_CHECKBOX || Item[I].Type==DI_RADIOBUTTON) &&
            (!DisableSelect || (Item[I].Flags & DIF_MOVESELECT)))
        {
          ProcessKey(KEY_SPACE);
          return(TRUE);
        }
        else if (Item[I].Type==DI_BUTTON)
        {
          ProcessKey(KEY_ENTER);
          return(TRUE);
        }
        // ��� ComboBox`� - "����������" ��������� //????
        else if (Item[I].Type==DI_COMBOBOX)
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
/* $ 31.07.2000 tran
   + ������� ������������� ��������� edit ������� */
/* $ 07.08.2000 SVS
   + � ��� ListBox ������?*/
void Dialog::AdjustEditPos(int dx, int dy)
{
  struct DialogItem *CurItem;
  int I;
  int x1,x2,y1,y2;

  if(!CheckDialogMode(DMODE_CREATEOBJECTS))
    return;

  ScreenObject *DialogEdit;
  for (I=0; I < ItemCount; I++)
  {
    CurItem=&Item[I];
    if (CurItem->ObjPtr && (IsEdit(CurItem->Type) || CurItem->Type == DI_LISTBOX))
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
   ������ � ���. ������� ���������� �������
   ���� ������� ����������� (����������)
*/
void Dialog::SetDialogData(long NewDataDialog)
{
  DataDialog=NewDataDialog;
}
/* SVS $ */

//////////////////////////////////////////////////////////////////////////
/* $ 11.08.2000 SVS
   + ��� ����, ����� ������� DM_CLOSE ����� �������������� Process
*/
void Dialog::Process()
{
  do{
    Modal::Process();
    /* $ 21.12.2000 SVS
       Ctr-Break ������ ��������������, �.�. ��� ������� �� ����
       ��� ������ ����������.
    */
    if(DlgProc((HANDLE)this,DM_CLOSE,ExitCode,0))// || ExitCode == -2)
      break;
    /* SVS $ */
    /* $ 18.08.2000 SVS
       ���-���, � ��������-�� ���� ������ 8-=(((
    */
    ClearDone();
    /* SVS $ */
  }while(1);
}
/* SVS $ */


//////////////////////////////////////////////////////////////////////////
/* $ 28.07.2000 SVS
   ������� ��������� ������� (�� ���������)
   ��� ������ ��� ������� � �������� ��������� ������� ��������� �������.
   �.�. ����� ������ ���� ��� ��������� ���� ���������!!!
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
      return FALSE; // ��������� �� ����!

    case DM_CLOSE:
      return TRUE;  // �������� � ���������

    case DN_KILLFOCUS:
      return -1;    // "�������� � ������� ������"

    case DN_GOTFOCUS:
      return 0;     // always 0

    case DN_HELP:
      return Param2; // ��� ��������, �� �...

    case DN_DRAWDIALOG:
    {
      if(Param1 == 1)  // ����� ���������� "�������"?
      {
        /* $ 03.08.2000 tran
           ����� ������ � ���� ����� ��������� � ������� �����������
           1) ����� ������ ������������ � ����
           2) ����� ������ ������������ �� ����
           ������ ����� ������� ������� �� ����� */
        Text(Dlg->X1,Dlg->Y1,0xCE,"\\");
        Text(Dlg->X1,Dlg->Y2,0xCE,"/");
        Text(Dlg->X2,Dlg->Y1,0xCE,"/");
        Text(Dlg->X2,Dlg->Y2,0xCE,"\\");
      }
      return TRUE;
    }

    case DN_CTLCOLORDIALOG:
      return Param2;

    case DN_CTLCOLORDLGITEM:
      return Param2;

    case DN_CTLCOLORDLGLIST:
      return FALSE;

    case DN_ENTERIDLE:
      return 0;     // always 0
  }

  // �������������� ��������...
  if(Param1 >= Dlg->ItemCount)
    return 0;

  CurItem=&Dlg->Item[Param1];
  Type=CurItem->Type;

  Ptr=CurItem->Data;

  switch(Msg)
  {
    case DN_MOUSECLICK:
      return FALSE;

    case DN_DRAWDLGITEM:
      return TRUE;

    case DN_HOTKEY:
      return TRUE;

    case DN_EDITCHANGE:
      return TRUE;

    case DN_BTNCLICK:
      return TRUE;

    case DN_LISTCHANGE:
      return TRUE;

    /* $ 23.08.2000 SVS
       + �������� �������(�)
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
   ������� ��������� �������
   ��������� ��������� ��� ������� ������������ ����, �� ��������� ����������
   ����������� �������.
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
  // �������������� ��������...
  if(Param1 >= Dlg->ItemCount)
    return 0;

//  CurItem=&Dlg->Item[Param1];
  CurItem=Dlg->Item+Param1;
  Type=CurItem->Type;
  Ptr=CurItem->Data;

  switch(Msg)
  {
    case DM_ADDHISTORY:
      if(Param2 &&
         (Type==DI_EDIT || Type==DI_FIXEDIT) &&
         (CurItem->Flags & DIF_HISTORY))
      {
        return Dlg->AddToEditHistory((char*)Param2,CurItem->History,strlen((char*)Param2)+1);
      }
      return FALSE;
    /* $ 23.10.2000 SVS
       ��������/���������� ������� � ������� ��������������
    */
    case DM_GETCURSORPOS:
      if(!CurItem->ObjPtr || !Param2)
        return FALSE;
      if (IsEdit(Type))
      {
        ((COORD*)Param2)->X=((Edit *)(CurItem->ObjPtr))->GetCurPos();
        ((COORD*)Param2)->Y=0;
        return TRUE;
      }
      else if(Type == DI_USERCONTROL)
      {
        ((COORD*)Param2)->X=((COORD*)(CurItem->ObjPtr))->X;
        ((COORD*)Param2)->Y=((COORD*)(CurItem->ObjPtr))->Y;
        return TRUE;
      }
      return FALSE;

    case DM_SETCURSORPOS:
      if(!CurItem->ObjPtr)
        return FALSE;
      if (IsEdit(Type))
      {
        ((Edit *)(CurItem->ObjPtr))->SetCurPos(((COORD*)Param2)->X);
        return TRUE;
      }
      else if(Type == DI_USERCONTROL)
      {
        // �����, ��� ���������� ��� ����� �������� ������ �������������!
        //  � ���������� � 0,0
        COORD Coord=*(COORD*)Param2;
        Coord.X+=CurItem->X1;
        if(Coord.X > CurItem->X2)
          Coord.X=CurItem->X2;

        Coord.Y+=CurItem->Y1;
        if(Coord.Y > CurItem->Y2)
          Coord.Y=CurItem->Y2;

        // ��������
        ((COORD*)(CurItem->ObjPtr))->X=Coord.X-CurItem->X1;
        ((COORD*)(CurItem->ObjPtr))->Y=Coord.Y-CurItem->Y1;
        // ���������� ���� ����
        if(Dlg->CheckDialogMode(DMODE_SHOW) && Dlg->FocusPos == Param1)
        {
           // ���-�� ���� ���� ������ :-)
           MoveCursor(Coord.X+Dlg->X1,Coord.Y+Dlg->Y1); // ???
           Dlg->ShowDialog(); //???
        }
        return TRUE;
      }
      return FALSE;
    /* SVS $ */


    case DN_LISTCHANGE:
    {
      return Dlg->DlgProc(hDlg,Msg,Param1,Param2);
    }

    case DN_EDITCHANGE:
    {
      Dialog::ConvertItem(CVTITEM_TOPLUGIN,&PluginDialogItem,CurItem,1,TRUE);
      if((I=Dlg->DlgProc(hDlg,Msg,Param1,(long)&PluginDialogItem)) == TRUE)
        Dialog::ConvertItem(CVTITEM_FROMPLUGIN,&PluginDialogItem,CurItem,1,TRUE);
      return I;
    }

    case DN_BTNCLICK:
      return Dlg->DlgProc(hDlg,Msg,Param1,Param2);

    case DN_DRAWDLGITEM:
      // ����������� ������ ���!
      Dialog::ConvertItem(CVTITEM_TOPLUGIN,&PluginDialogItem,CurItem,1);
      I=Dlg->DlgProc(hDlg,Msg,Param1,(long)&PluginDialogItem);
      Dialog::ConvertItem(CVTITEM_FROMPLUGIN,&PluginDialogItem,CurItem,1);
      return I;

    case DM_SETREDRAW:
      if(Dlg->CheckDialogMode(DMODE_INITOBJECTS))
        Dlg->Show();
      return 0;

    /* $ 08.09.2000 SVS
      - ���� �������, �� DM_SETFOCUS ����� ��� � ������� :-)
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
      �������� ID ������
    */
    case DM_GETFOCUS:
      return Dlg->FocusPos;
    /* SVS $ */

    case DM_GETTEXTPTR:
      if(Param2)
      {
        struct FarDialogItemData IData;
        IData.PtrData=(char *)Param2;
        IData.PtrLength=0;
        return Dialog::SendDlgMessage(hDlg,DM_GETTEXT,Param1,(long)&IData);
      }

    case DM_GETTEXT:
      if(Param2) // ���� ����� NULL, �� ��� ��� ���� ������ �������� ������
      {
        struct FarDialogItemData *did=(struct FarDialogItemData*)Param2;
        Len=0;
        switch(Type)
        {
          case DI_COMBOBOX:
          case DI_EDIT:
          case DI_PSWEDIT:
          case DI_FIXEDIT:
            if(!CurItem->ObjPtr)
              break;
            ((Edit *)(CurItem->ObjPtr))->GetString(Str,sizeof(Str));
            Ptr=Str;

          case DI_TEXT:
          case DI_VTEXT:
          case DI_SINGLEBOX:
          case DI_DOUBLEBOX:
          case DI_CHECKBOX:
          case DI_RADIOBUTTON:
          case DI_BUTTON:

            Len=strlen(Ptr)+1;
            if (!(CurItem->Flags & DIF_NOBRACKETS) && Type == DI_BUTTON)
            {
              Ptr+=2;
              Len-=4;
            }

            if(!did->PtrLength)
              did->PtrLength=Len;
            else if(Len > did->PtrLength)
              Len=did->PtrLength;

            if(Len > 0 && did->PtrData)
            {
              memmove(did->PtrData,Ptr,Len);
              did->PtrData[Len]=0;
            }
            break;

          case DI_USERCONTROL:
            did->PtrLength=CurItem->Ptr.PtrLength;
            did->PtrData=(char*)CurItem->Ptr.PtrData;
            break;

          case DI_LISTBOX: // ���� �� ������� - �� ����������
          {
            if(!CurItem->ObjPtr)
              break;
            VMenu *VMenuPtr=(VMenu *)(CurItem->ObjPtr);
            did->PtrLength=VMenuPtr->GetUserData(did->PtrData,did->PtrLength,-1);
            break;
          }

          default:  // �������������, ��� ��������
            did->PtrLength=0;
            break;
        }
        return Len;
      }
      // ����� ��������� �� ������ return, �.�. ����� �������� ������
      // ������������� ����� ������ ���� "case DM_GETTEXTLENGTH"!!!

    case DM_GETTEXTLENGTH:
      switch(Type)
      {
        case DI_BUTTON:
          Len=strlen(Ptr)+1;
          if (!(CurItem->Flags & DIF_NOBRACKETS))
            Len-=4;
          break;

        case DI_USERCONTROL:
          Len=CurItem->Ptr.PtrLength;
          break;

        case DI_TEXT:
        case DI_VTEXT:
        case DI_SINGLEBOX:
        case DI_DOUBLEBOX:
        case DI_CHECKBOX:
        case DI_RADIOBUTTON:
          Len=strlen(Ptr)+1;
          break;

        case DI_COMBOBOX:
        case DI_EDIT:
        case DI_PSWEDIT:
        case DI_FIXEDIT:
          if(CurItem->ObjPtr)
            Len=((Edit *)(CurItem->ObjPtr))->GetLength();

        case DI_LISTBOX:
          Len=0;
          if(CurItem->ObjPtr)
          {
            VMenu *VMenuPtr=(VMenu *)(CurItem->ObjPtr);
            Len=VMenuPtr->GetUserData(NULL,0,-1);
          }
          break;

        default:
          Len=0;
          break;
      }
      return Len;

    case DM_SETTEXTPTR:
    {
      if(!Param2)
        return 0;

      struct FarDialogItemData IData;
      IData.PtrData=(char *)Param2;
      IData.PtrLength=strlen(IData.PtrData);
      return Dialog::SendDlgMessage(hDlg,DM_SETTEXT,Param1,(long)&IData);
    }


    case DM_SETTEXT:
      if(Param2)
      {
        struct FarDialogItemData *did=(struct FarDialogItemData*)Param2;
        Len=0;
        switch(Type)
        {
          case DI_USERCONTROL:
            CurItem->Ptr.PtrLength=did->PtrLength;
            CurItem->Ptr.PtrData=did->PtrData;
            return CurItem->Ptr.PtrLength;

          case DI_TEXT:
          case DI_VTEXT:
          case DI_SINGLEBOX:
          case DI_DOUBLEBOX:
            if((Len=did->PtrLength) == NULL)
            {
              strncpy(Ptr,(char *)did->PtrData,511);
              Len=strlen(Ptr)+1;
            }
            else
            {
              if((unsigned)did->PtrLength > 511)
                Len=511;
              memmove(Ptr,(char *)did->PtrData,Len);
              Ptr[Len]=0;
            }
            if(Dlg->CheckDialogMode(DMODE_SHOW))
            {
              Dlg->ShowDialog(Param1);
              ScrBuf.Flush();
            }
            return Len;

          case DI_BUTTON:
          case DI_CHECKBOX:
          case DI_RADIOBUTTON:
            if((Len=did->PtrLength) == NULL)
            {
              strncpy(Ptr,(char *)did->PtrData,511);
              Len=strlen(Ptr)+1;
            }
            else
            {
              if((unsigned)did->PtrLength > 511)
                Len=511;
              memmove(Ptr,(char *)did->PtrData,Len);
              Ptr[Len]=0;
            }
            break;

          case DI_COMBOBOX:
          case DI_EDIT:
          case DI_PSWEDIT:
          case DI_FIXEDIT:
            if((Len=did->PtrLength) == NULL)
            {
              strncpy(Ptr,(char *)did->PtrData,511);
              Len=strlen(Ptr)+1;
            }
            else
            {
              if((unsigned)did->PtrLength > 511)
                Len=511;
              memmove(Ptr,(char *)did->PtrData,Len);
              Ptr[Len]=0;
            }
            if(CurItem->ObjPtr)
            {
              ((Edit *)(CurItem->ObjPtr))->SetString((char *)Ptr);
              ((Edit *)(CurItem->ObjPtr))->Select(-1,-1); // ������� ���������
            }
            break;

          case DI_LISTBOX: // ���� �� ������� - �� ����������
            return 0;

          default:  // �������������, ��� ��������
            return 0;
        }
        Dlg->InitDialogObjects(Param1); // ������������������ �������� �������
        if(Dlg->CheckDialogMode(DMODE_SHOW)) // ���������� �� �����????!!!!
        {
          Dlg->ShowDialog(Param1);
          ScrBuf.Flush();
        }
        return strlen((char *)Ptr)+1; //???
      }
      return 0;

    case DM_SETTEXTLENGTH:
      if((Type==DI_EDIT || Type==DI_PSWEDIT ||
          (Type==DI_COMBOBOX && !(CurItem->Flags & DIF_DROPDOWNLIST))) &&
         CurItem->ObjPtr)
      {
        int MaxLen=((Edit *)(CurItem->ObjPtr))->GetMaxLength();

        if((CurItem->Type==DI_EDIT || CurItem->Type==DI_COMBOBOX) &&
           (CurItem->Flags&DIF_VAREDIT))
          CurItem->Ptr.PtrLength=Param2; //???
        else if(Param2 > 511)
          Param2=511;

        ((Edit *)(CurItem->ObjPtr))->SetMaxLength(Param2);

        //if (CheckDialogMode(DMODE_INITOBJECTS)) //???
        Dlg->InitDialogObjects(Param1); // ������������������ �������� �������
        return MaxLen;
      }
      return 0;

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
      if(Param2 &&
         Type == ((struct FarDialogItem *)Param2)->Type) // ���� ������ ������ ���
      {
        Dialog::ConvertItem(CVTITEM_FROMPLUGIN,(struct FarDialogItem *)Param2,CurItem,1);
        CurItem->Type=Type;
        // ��� �����, �.�. ������ ����� ���� ��������
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
       + ����������/���������� ��������� �������
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
       + ��������/�������� ������.
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
       + ����������/����� ������ �������.
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
       + ������� �������(�)
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
       + ������������� ������� ������
    */
    case DM_CLOSE:
      if(Param1 == -1)
        Dlg->ExitCode=Dlg->FocusPos;
      else
        Dlg->ExitCode=Param1;
      Dlg->EndLoop=TRUE;
      return TRUE;  // �������� � ���������
    /* SVS $ */

    /* $ 25.08.2000 SVS
        + �������� ���������� ����������� ����
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
        + ����������� ����������� �������
    */
    case DM_MOVEDIALOG:
    {
      int W1,H1;

      W1=Dlg->X2-Dlg->X1;
      H1=Dlg->Y2-Dlg->Y1;
      // ���������
      Dlg->OldX1=Dlg->X1;
      Dlg->OldY1=Dlg->Y1;
      Dlg->OldX2=Dlg->X2;
      Dlg->OldY2=Dlg->Y2;
      // �����������
      if(Param1)   // ���������?
      {
        Dlg->X1=((COORD*)Param2)->X;
        Dlg->Y1=((COORD*)Param2)->Y;
//        if(Dlg->X1 == -1 || Dlg->Y1 == -1)
        Dlg->CheckDialogCoord();
      }
      else         // ������ ������������
      {
        Dlg->X1+=((COORD*)Param2)->X;
        Dlg->Y1+=((COORD*)Param2)->Y;
      }

      // ��������� � ���������������
      if(Dlg->X1 < 0)         Dlg->X1=0;
      if(Dlg->Y1 < 0)         Dlg->Y1=0;
      if(Dlg->X1+W1 >= ScrX)  Dlg->X1=ScrX-W1; //?
      if(Dlg->Y1+H1 >= ScrY)  Dlg->Y1=ScrY-H1; //?
      Dlg->X2=Dlg->X1+W1;
      Dlg->Y2=Dlg->Y1+H1;
      ((COORD*)Param2)->X=Dlg->X1;
      ((COORD*)Param2)->Y=Dlg->Y1;

      I=Dlg->IsVisible();// && Dlg->CheckDialogMode(DMODE_INITOBJECTS);
      if(I) Dlg->Hide();
      // �������.
      Dlg->AdjustEditPos(Dlg->X1-Dlg->OldX1,Dlg->Y1-Dlg->OldY1);
      if(I) Dlg->Show(); // ������ ���� ������ ��� �����

      return Param2;
    }
    /* SVS $ */

    /* $ 31.08.2000 SVS
        + ������������/��������� ��������� Enable/Disable ��������
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

    /* $ 03.01.2001 SVS
        + ��������/������ �������
        Param2: -1 - �������� ���������
                 0 - ��������
                 1 - ��������
        Return:  ���������� ���������
    */
    case DM_SHOWITEM:
    {
      DWORD PrevFlags=CurItem->Flags;
      if(Param2 != -1)
      {
         if(Param2)
           CurItem->Flags&=~DIF_HIDDEN;
         else
           CurItem->Flags|=DIF_HIDDEN;
      }
      return (PrevFlags&DIF_HIDDEN)?FALSE:TRUE;
    }
    /* SVS $ */
  }

  // ���, ��� ���� �� ������������ - �������� �� ��������� �����������.
  return Dlg->DlgProc(hDlg,Msg,Param1,Param2);
}
/* SVS $ */

//////////////////////////////////////////////////////////////////////////
