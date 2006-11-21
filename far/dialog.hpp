#ifndef __DIALOG_HPP__
#define __DIALOG_HPP__
/*
dialog.hpp

����� ������� Dialog.

������������ ��� ����������� ��������� ��������.
�������� ����������� �� ������ Frame.

*/

/* Revision: 1.82 07.10.2005 $ */

/*
Modify:
  07.10.2005 SVS
    + ��� MACRO (��� "���������" ������� � ��������� ������):
        GetAllItem()
        GetAllItemCount()
        GetDlgFocusPos()
  24.04.2005 AY
    ! GCC
  22.03.2005 SVS
    + DMODE_BEGINLOOP
  29.01.2005 WARP
    ! ��������� cleanup (��. 01920.vmenu_dialog_cleanup.txt)
  27.12.2004 WARP
    ! ������ ����������� ������ ��� ����������� ���� ������� Dialog & VMenu
  25.12.2004 WARP
    ! ������������� �������� (��������� ��. 01894.DialogCenter.txt)
  08.12.2004 WARP
    ! ���� ��� ������ #1. ��������� 01864.FindFile.txt
  06.07.2004 SVS
    + CheckHighlights ��� Macro II
  28.02.2004 SVS
    ! DLGEDITLILE_ -> DLGEDITLINE_
    + DLGITEMINTERNALFLAGS
  19.02.2004 SVS
    ! ���� DMODE_MOUSELIST �������, �.�. ��� ���� ������: ���������� ��������
      ������ ���� �� ������ ��������, � �� ����� �������
  18.12.2003 SVS
    + DMOUSEBUTTON_??? - �� ����� ������ �����������
  23.10.2003 SVS
    ! Dialog::GetDialogTitle ���������� const
    + CRITICAL_SECTION CSection
  30.05.2003 SVS
    + DMODE_CLICKOUTSIDE - ���� ������� ���� ��� �������?
  19.05.2003 SVS
    + ������� � DialogItem - SelStart, SelEnd. �������� �� ����������
      ���������� ��������� � ������� ��������������
  25.02.2003 SVS
    - BugZ#811 - ��������� ��� ������������� ��������� ����
      ����������� �������� � BugZ#806. ������� ���� DMODE_MSGINTERNAL, �������
      ����� ������������ � Message().
      �� �������� ����� ����� �������! ������ ��� ��� � ������� ���������
      FarDialogMessage. ������ ��� ���� �������������� �������� �� �����...
      (�������, ��� �� ������ ������� ��� ����� �������, ����� ����� ���-��������
      ����� ����).
  17.10.2002 SVS
    + ������� � Do_ProcessNextCtrl() �������� ��� ��������������
      ���������� 2 ��������� (������� � ������)
  30.09.2002 SVS
    ! SelectFromComboBox ����� ������ �������� ���� struct DialogItem
  23.09.2002 SVS
    + DialogItem ����� IFlags - ��� ���������� ����.
  20.09.2002 SVS
    + ����� DMODE_NODRAWSHADOW � DMODE_NODRAWPANEL
  10.09.2002 SVS
    ! DialogItem �������� � FarDialogItem
    + ��������� ���������� ��������� ������� (����� �������������� ���
      �������� � ��������� �������)
  17.08.2002 SVS
    ! ��������� CtrlDown �������� �� ����������� ���������� � ���������
      ������� ProcessOpenComboBox, �.�. ����� ProcessKey(KEY_CTRLDOWN)
      �� �������� ����������� �������� � �������� ������������� ��������
  18.05.2002 SVS
    ! OwnsItems -> DMODE_OWNSITEMS
  15.05.2002 SVS
    ! ������ Edit ����� ����� ����� DlgEdit
  06.05.2002 SVS
    + InitDialog() - ������������� �������. ���������������� � Process()
      ��� ��������������, �� �� ������ Show()
    ! InitDialogObjects() ��������� � privat
  29.04.2002 SVS
    + ProcessRadioButton
  26.04.2002 SVS
    - BugZ#484 - Addons\Macros\Space.reg (��� ��������� �������)
  22.04.2002 KM
    ! ������� ������� OnDestroy - �������� ������������.
  08.04.2002 SVS
   + CtlColorDlgItem()
  11.03.2002 SVS
   ! ������� ������������... � ��������� �����.
     �� ����� ���������������� �� �����������, �.�. ��������� DialogItem
     ��������� � � ������ ���� �� �������.
  13.02.2002 SVS
   ! ������ �������� � IsKeyHighlighted() - const
  11.02.2002 SVS
   ! OriginalListItems � ���������... - ����� DlgProc
  05.02.2002 SVS
   + DialogItem.OriginalListItems
  21.01.2002 SVS
   ! �������� ������ ��� UserControl
  08.01.2002 SVS
   + SetListMouseReaction()
  21.12.2001 SVS
   ! unsigned char -> short ��� ��������� � ���������� DialogItem � DialogData
   + LenStrItem() - ������ �� ���� ������ � ������ ����� DIF_SHOWAMPERSAND
  09.12.2001 DJ
   + ProcessLastHistory() - ��������� DIF_USELASTHISTORY
  04.12.2001 SVS
   + ProcessCenterGroup() - �������� ��������� ��������� � ������ DIF_CENTERGROUP
  21.11.2001 SVS
   + ������������� (����� I)
   ! ��������� � ��������� DialogItem: +DialogItem.ID, +DialogItem.AutoCount
     DialogItem.ID - ���� ����������������� ����, �� ����� ��� ����� ��������
     ��������������� ��������!
  12.11.2001 SVS
   ! SelectFromEditHistory() ���������� ��������.
   ! SelectOnEntry() ����� ���.�������� - �������� ��� �� ��������.
  12.11.2001 OT
   - VC ������ ������...
  08.11.2001 SVS
   ! ������� � ���� BitFlags - ���������� ������� �������� ������ �������
  06.11.2001 SVS
   ! ������� ���.�������� struct DialogItem *CurItem � �������
     SelectFromEditHistory()
  01.11.2001 SVS
   ! MakeDialogItems �������� � dialog.hpp �� farconst.hpp
  15.08.2001 SVS
   + DMODE_MOUSEEVENT
  23.07.2001 OT
   -  ����������� ��������� ���� � ����� MA � ��������
  22.07.2001 SVS
   ! ������������ ��������� ������� SelectFromComboBox() - ���� � ���
     �������� ��������� �� �� ������� ����� ������, ��� ���...
  21.07.2001 KM
   ! ���������� FindFiles ������ ������� ��� ������� � ����� Item.
  12.07.2001 OT
   - ����������� �������� (����� 816) F11->F4->Esc-> :(
  11.07.2001 OT
   ! ������� CtrlAltShift � Manager
  09.07.2001 OT
   - ����������� MacroMode ��� ��������
  23.06.2001 KM
   + ������� ������������ ��������/�������� ���������� � �������
     � ��������� ������� ����������/���������� ���������� � �������.
   + ���������� DropDownOpened ��� �������� ������� ���������� � �������.
  04.06.2001 SVS
   ! HISTORY_COUNT -> farconst.hpp
   ! AddToEditHistory() - �������� ��� ������ ������ ����� �������.
  30.05.2001 KM
   + SetItemRect - ������� ��� ��������� �������� �/��� ���������
     ����� � �������.
  23.05.2001 SVS
   - �������� � �������� ��������� � ����. �������� 4-� �������� �
     IsKeyHighlighted - ������� ���������� (�� ��������� = -1)
  19.05.2001 DJ
   + OwnsItems
  17.05.2001 DJ
   ! Dialog ����������� �� Frame
   + CloseDialog()
  14.05.2001 SVS
   ! DMODE_SMALLDILAOG -> DMODE_SMALLDIALOG
  12.05.2001 SVS
   ! ��������� ������ �������� � SelectFromComboBox();
   ! ������� SelectFromComboBox() ������ ���������� ��� ��������.
   + DialogItem.ListPtr - ��� DI_COMBOBOX
  10.05.2001 SVS
   + FDLG_SMALLDILAOG - �� �������� "���������"
   ! SetWarningStyle �������
   ! ������� SetDialogMode ���������� � public ������.
  06.05.2001 DJ
   + �������� #include
  28.04.2001 SVS
   + GetItemRect() - �������� ���������� �����.
  12.04.2001 SVS
   ! ������� AddToEditHistory ������ ���������� ��������� ��������
     ���������� ������ � �������
  12.04.2001 SVS
   + CheckDialogCoord() - �������� � ������������� ��������� �������
  23.03.2001 SVS
   ! � ������� ConvertItem() ����� �������� InternalCall - ������
     ������������ ������ ��� DN_EDITCHANGE
  13.02.2001 SVS
   + �������������� �������� � FindInEditForAC, SelectFromEditHistory,
     AddToEditHistory � SelectFromComboBox - MaxLen - ������������ ������
     ������ ����������.
  24.09.2000 SVS
   + DMODE_ALTDRAGGED - ��� �������� ������� �� Alt-�������
  08.09.2000 SVS
   + ����� ������� DMODE_OLDSTYLE - ������ � ������ �����.
   + ������� SelectOnEntry - ��������� ������ ��������������
     (��������� ����� DIF_SELECTONENTRY)
  30.08.2000 SVS
   + ����� ������� DMODE_SHOW - ������ �����?
   + ����� Hide()
  29.08.2000 SVS
   ! ��� ������� ���� ������ �� �������� ���������...
     ������, ����� ����� ����������� �������!
  24.08.2000 SVS
   + InitDialogObjects() ����� �������� - ��� ���������� ���������������
     ���������
  23.08.2000 SVS
   ! ��������� ��� DataDialog.
   + ���������� ������ FocusPos - ������ �������� ����� ������� � ������
   ! ���������� IsCanMove, InitObjects, CreateObjects, WarningStyle, Dragged
     ������� -> ������� ����� � DialogMode
   ! ������ LV ������ �� �������������.
   + CheckDialogMode - ������� �������� ����� DialogMode
  22.08.2000 SVS
   ! � ���� ���������� ������ �� ��� :-((
     IsMovedDialog -> IsCanMove
     SetModeMoving -> SetMoveEnable
     GetModeMoving -> GetMoveEnable
   ! ShowDialog - �������������� �������� - ����� ������� ������������
  18.08.2000 SVS
   + ���� IsEnableRedraw - �����������/����������� ����������� �������
   + DialogMode - ����� �������� ������ �������
  11.08.2000 SVS
   + ������, ������������� ��� ����������� ���������� �������
   + ��� ����, ����� ������� DMSG_CLOSE ����� �������������� Process
  10.08.2000 SVS
   + ���������� IsMovedDialog - ����� �� ������� ������ :-)
   + ������� ��������� IsMovedDialog
  09.08.2000 KM 1.09
   + ���������� ������� �������� �� ����� ����������� �������.
     ������ ����� �������� ������������� 1.09 - ���� ��� ��������.
  01.08.2000 SVS
   - ���������� ������ lastKey ������� �� ������������� :-)
  31.07.2000 tran & SVS
   + ���������� ������ Dragged - ���� �����������
     � ����� OldX*, OldY*,
     ����� - AdjustEditPos(int dx,int dy) - ������������ ���������� Edit"��
   + ���������� ����, ��� ��� ����������� ����������� �������
  28.07.2000 SVS
   + ���������� ������ InitParam - ������ ��������, ����������
     � ������.
   ! ������ InitDialogObjects ���������� ID �������� � ������� �����
   + ������� ChangeFocus2
     �������� ����� ����� ����� ����� ����������.
   ! �������� Edit *EditLine � ������� FindInEditForAC ����� �������!
   ! FindInEditHistory -> FindInEditForAC
     ����� ��� � �������, ��� � � ComboBox`� (����� �� ������� ����)
   ! SelectFromComboBox ����� �������������� �������� � ���, �����
     ��������������� item � ���� �� ������� � ����������� �� ������� �����
   + ������� IsFocused, ������������ - "����� �� ������� �������
     ����� ����� �����"
   ! IsEdit ���� Static-�������!
   + ������� ������� ��������� ������� SendDlgMessage
   + ������� ConvertItem - �������������� �� ����������� �������������
     � FarDialogItem � �������
  26.07.2000 SVS
   + FindInEditHistory: ����� ��������� ��������� � �������
  25.07.2000 SVS
   + Private: lastKey - ��� AutoComplit ��������� �������
   + �������������� �������� � SelectFromEditHistory ��� ���������
     ������ ������� � ������� (���� ��� ������������� ������ �����)
  25.07.2000 SVS
   ! ����� �������� � ������������
  23.07.2000 SVS
   + ���� ������� � ���������� :-)
   + ������� ��������� ������� (�� ���������) - ������ ����� :-)
   ! ������� ����� ������������
  18.07.2000 SVS
    + ������� SelectFromComboBox ��� ������ �� DI_COMBOBOX
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "frame.hpp"
#include "plugin.hpp"
#include "vmenu.hpp"
#include "bitflags.hpp"
#include "CriticalSections.hpp"

// ����� �������� ������ �������
#define DMODE_INITOBJECTS   0x00000001 // �������� ����������������?
#define DMODE_CREATEOBJECTS 0x00000002 // ������� (Edit,...) �������?
#define DMODE_WARNINGSTYLE  0x00000004 // Warning Dialog Style?
#define DMODE_DRAGGED       0x00000008 // ������ ���������?
#define DMODE_ISCANMOVE     0x00000010 // ����� �� ������� ������?
#define DMODE_ALTDRAGGED    0x00000020 // ������ ��������� �� Alt-�������?
#define DMODE_SMALLDIALOG   0x00000040 // "�������� ������"
#define DMODE_DRAWING       0x00001000 // ������ ��������?
#define DMODE_KEY           0x00002000 // ���� ������� ������?
#define DMODE_SHOW          0x00004000 // ������ �����?
#define DMODE_MOUSEEVENT    0x00008000 // ����� �������� MouseMove � ����������?
#define DMODE_RESIZED       0x00010000 //
#define DMODE_ENDLOOP       0x00020000 // ����� ����� ��������� �������?
#define DMODE_BEGINLOOP     0x00040000 // ������ ����� ��������� �������?
#define DMODE_OWNSITEMS     0x00080000 // ���� TRUE, Dialog ����������� ������ Item � �����������
#define DMODE_NODRAWSHADOW  0x00100000 // �� �������� ����?
#define DMODE_NODRAWPANEL   0x00200000 // �� �������� ��������?
#define DMODE_CLICKOUTSIDE  0x20000000 // ���� ������� ���� ��� �������?
#define DMODE_MSGINTERNAL   0x40000000 // ���������� Message?
#define DMODE_OLDSTYLE      0x80000000 // ������ � ������ (�� 1.70) �����

#define DIMODE_REDRAW       0x00000001 // ��������� �������������� ���������� �����?

// ����� ��� ������� ConvertItem
#define CVTITEM_TOPLUGIN    0
#define CVTITEM_FROMPLUGIN  1

#define DMOUSEBUTTON_LEFT   0x00000001
#define DMOUSEBUTTON_RIGHT  0x00000002

enum DLGEDITLINEFLAGS {
  DLGEDITLINE_CLEARSELONKILLFOCUS = 0x00000001, // ��������� ���������� ����� ��� ������ ������ �����
  DLGEDITLINE_SELALLGOTFOCUS      = 0x00000002, // ��������� ���������� ����� ��� ��������� ������ �����
  DLGEDITLINE_NOTSELONGOTFOCUS    = 0x00000004, // �� ��������������� ��������� ������ �������������� ��� ��������� ������ �����
  DLGEDITLINE_NEWSELONGOTFOCUS    = 0x00000008, // ��������� ��������� ��������� ����� ��� ��������� ������
  DLGEDITLINE_GOTOEOLGOTFOCUS     = 0x00000010, // ��� ��������� ������ ����� ����������� ������ � ����� ������
  DLGEDITLINE_PERSISTBLOCK        = 0x00000020, // ���������� ����� � ������� �����
  DLGEDITLINE_AUTOCOMPLETE        = 0x00000040, // �������������� � ������� �����
  DLGEDITLINE_AUTOCOMPLETECTRLEND = 0x00000040, // ��� �������������� ������������ ����������� Ctrl-End
  DLGEDITLINE_HISTORY             = 0x00000100, // ������� � ������� ����� ��������
};


enum DLGITEMINTERNALFLAGS {
  DLGIIF_LISTREACTIONFOCUS        = 0x00000001, // MouseReaction ��� ��������� ��������
  DLGIIF_LISTREACTIONNOFOCUS      = 0x00000002, // MouseReaction ��� �� ��������� ��������
  DLGIIF_EDITPATH                 = 0x00000004, // ����� Ctrl-End � ������ �������������� ����� �������� �� ���� �������������� ������������ ����� � ���������� � ������ �� �������
};


#define MakeDialogItems(Data,Item) \
  struct DialogItem Item[sizeof(Data)/sizeof(Data[0])]; \
  Dialog::DataToItem(Data,Item,sizeof(Data)/sizeof(Data[0]));


// ���������, ����������� ������������� ��� DIF_AUTOMATION
// �� ������ ����� - ����������� - ����������� ������ � ��������� ��� CheckBox
struct DialogItemAutomation{
  WORD ID;                    // ��� ����� ��������...
  DWORD Flags[3][2];          // ...��������� ��� ��� �����
                              // [0] - Unchecked, [1] - Checked, [2] - 3Checked
                              // [][0] - Set, [][1] - Skip
};

// ������ ��� DI_USERCONTROL
class DlgUserControl{
  public:
    COORD CursorPos;
    int   CursorVisible,CursorSize;

  public:
    DlgUserControl(){CursorSize=CursorPos.X=CursorPos.Y=-1;CursorVisible=0;}
   ~DlgUserControl(){};
};

/*
��������� ���� ������� ������� - ��������� �������������.
��� �������� ��� FarDialogItem (�� ����������� ObjPtr)
*/
struct DialogItem
{
  int Type;
  int X1,Y1,X2,Y2;
  int Focus;
  union
  {
    int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
    int  ListPos;
    CHAR_INFO *VBuf;
  };
  DWORD Flags;
  int DefaultButton;
  union {
    char Data[512];
    struct {
      DWORD PtrFlags;
      int   PtrLength;
      void *PtrData;
      char  PtrTail[1];
    } Ptr;
  };

  WORD ID;
  BitFlags IFlags;
  int AutoCount;   // �������������
  struct DialogItemAutomation* AutoPtr;
  DWORD_PTR UserData; // ��������������� ������

  // ������
  void *ObjPtr;
  VMenu *ListPtr;
  DlgUserControl *UCData;

  int SelStart;
  int SelEnd;
};

/*
��������� ���� ������� ������� - ��� ���������� �������
��������� ����������� ��������� InitDialogItem (��. "Far PlugRinG
Russian Help Encyclopedia of Developer")
*/
struct DialogData
{
  WORD  Type;
  short X1,Y1,X2,Y2;
  BYTE  Focus;
  union {
    unsigned int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
    int  ListPos;
    CHAR_INFO *VBuf;
  };
  DWORD Flags;
  BYTE  DefaultButton;
  char *Data;
};

struct FarDialogMessage{
  HANDLE hDlg;
  int    Msg;
  int    Param1;
  LONG_PTR   Param2;
};

class DlgEdit;
class ConsoleTitle;

class Dialog: public Frame
{
  /* $ 21.07.2001 KM
    ! ���������� FindFiles ������ ������� ��� ������� � ����� Item.
  */
  friend class FindFiles;
  /* KM $ */
  private:
    /* $ 29.08.2000 SVS
       + ����� �������, ��� ������������ HelpTopic
    */
    int PluginNumber;
    /* SVS $ */
    /* $ 23.08.2000 SVS
       + ���������� ������ FocusPos
    */
    int FocusPos;               // ������ �������� ����� ������� � ������
    /* SVS $ */
    int PrevFocusPos;           // ������ �������� ����� ������� ��� � ������
    /* $ 18.08.2000 SVS
      + ���� IsEnableRedraw - �����������/����������� ����������� �������
      + DialogMode - ����� �������� ������ �������
    */
    int IsEnableRedraw;         // ��������� ����������� �������? ( 0 - ���������)
    BitFlags DialogMode;        // ����� �������� ������ �������
    /* SVS $ */
    /* $ 11.08.2000 SVS
      + ������, ������������� ��� ����������� ���������� �������
    */
    long DataDialog;            // ������������� ����� ��������,
                                //   ���������� � �����������
    /* SVS $ */
    struct DialogItem *Item;    // ������ ��������� �������
    int ItemCount;              // ���������� ��������� �������

    ConsoleTitle *OldTitle;     // ���������� ���������
    int DialogTooLong;          //
    int PrevMacroMode;          // ���������� ����� �����

    FARWINDOWPROC DlgProc;      // ������� ��������� �������

    /* $ 31.07.2000 tran
       ���������� ��� ����������� ������� */
    int  OldX1,OldX2,OldY1,OldY2;
    /* tran 31.07.2000 $ */

    /* $ 17.05.2001 DJ */
    char *HelpTopic;
    /* DJ $ */
    /* $ 23.06.2001 KM
       + �������� ������ ���������� � �������:
         TRUE - ������, FALSE - ������.
    */
    volatile int DropDownOpened;
    /* KM $ */
    CriticalSection CS;

    int RealWidth, RealHeight;

  private:
    void DisplayObject();
    void DeleteDialogObjects();
    int  LenStrItem(int ID,char *Str=NULL);
    /* $ 22.08.2000 SVS
      ! ShowDialog - �������������� �������� - ����� ������� ������������
        ID=-1 - ���������� ���� ������
    */
    void ShowDialog(int ID=-1);
    /* SVS $ */

    DWORD CtlColorDlgItem(int ItemPos,int Type,int Focus,DWORD Flags);
    /* $ 28.07.2000 SVS
       + �������� ����� ����� ����� ����� ����������.
         ������� �������� ��� ����, ����� ���������� DMSG_KILLFOCUS & DMSG_SETFOCUS
    */
    int ChangeFocus2(int KillFocusPos,int SetFocusPos);
    /* SVS $ */
    int ChangeFocus(int FocusPos,int Step,int SkipGroup);
    static int IsEdit(int Type);
    /* $ 28.07.2000 SVS
       �������, ������������ - "����� �� ������� ������� ����� ����� �����"
    */
    static int IsFocused(int Type);
    /* SVS $ */
    /* $ 26.07.2000 SVS
      + �������������� �������� � SelectFromEditHistory ��� ���������
       ������ ������� � ������� (���� ��� ������������� ������ �����)
    */
    BOOL SelectFromEditHistory(struct DialogItem *CurItem,DlgEdit *EditLine,char *HistoryName,char *Str,int MaxLen);
    /* SVS $ */
    /* $ 18.07.2000 SVS
       + ������� SelectFromComboBox ��� ������ �� DI_COMBOBOX
    */
    int SelectFromComboBox(struct DialogItem *CurItem,DlgEdit*EditLine,VMenu *List,int MaxLen);
    /* SVS $ */
    /* $ 26.07.2000 SVS
       AutoComplite: ����� ��������� ��������� � �������
    */
    int FindInEditForAC(int TypeFind,void *HistoryName,char *FindStr,int MaxLen);
    /* SVS $ */
    int AddToEditHistory(char *AddStr,char *HistoryName);

    /* $ 09.12.2001 DJ
       ��������� DIF_USELASTHISTORY
    */
    void ProcessLastHistory (struct DialogItem *CurItem, int MsgIndex);
    /* DJ $ */

    int ProcessHighlighting(int Key,int FocusPos,int Translate);
    BOOL CheckHighlights(BYTE Chr);

    /* $ 08.09.2000 SVS
      ������� SelectOnEntry - ��������� ������ ��������������
      ��������� ����� DIF_SELECTONENTRY
    */
    void SelectOnEntry(int Pos,BOOL Selected);
    /* SVS $ */

    void CheckDialogCoord(void);
    BOOL GetItemRect(int I,RECT& Rect);

    /* $ 19.05.2001 DJ
       ���������� ��������� ������� (����� ������� ������ ��� ������)
    */
    const char *GetDialogTitle();
    /* DJ $ */

    /* $ 30.05.2000 KM
       ������ ���������� ��� ������ ����� �������.
    */
    BOOL SetItemRect(int ID,SMALL_RECT *Rect);
    /* KM $ */

    /* $ 23.06.2001 KM
       + ������� ������������ ��������/�������� ���������� � �������
         � ��������� ������� ����������/���������� ���������� � �������.
    */
    volatile void SetDropDownOpened(int Status){ DropDownOpened=Status; }
    volatile int GetDropDownOpened(){ return DropDownOpened; }
    /* KM $ */

    void ProcessCenterGroup(void);
    int ProcessRadioButton(int);

    /* $ 24.08.2000 SVS
       InitDialogObjects ����� �������� - ��� ���������� ���������������
       ���������
    */
    int  InitDialogObjects(int ID=-1);
    /* 24.08.2000 SVS $ */

    int ProcessOpenComboBox(int Type,struct DialogItem *CurItem,int CurFocusPos);
    int ProcessMoveDialog(DWORD Key);

    int Do_ProcessTab(int Next);
    int Do_ProcessNextCtrl(int Next,BOOL IsRedraw=TRUE);
    int Do_ProcessFirstCtrl();
    int Do_ProcessSpace();

    LONG_PTR CallDlgProc (int nMsg, int nParam1, LONG_PTR nParam2);

  public:
    Dialog(struct DialogItem *Item,int ItemCount,FARWINDOWPROC DlgProc=NULL,LONG_PTR Param=0);
    ~Dialog();

  public:
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void Show();
    /* $ 30.08.2000 SVS
       ������� ����������� Hide()
    */
    void Hide();
    /* SVS $ */
    void FastShow() {ShowDialog();}
    /* $ 28.07.2000 SVS
       ������ InitDialogObjects ���������� ID ��������
       � ������� �����
    */
    /* SVS $ */
    void GetDialogObjectsData();

    void SetDialogMode(DWORD Flags){ DialogMode.Set(Flags); }

    /* $ 28.07.2000 SVS
       + ������� ConvertItem - �������������� �� ����������� �������������
        � FarDialogItem � �������
    */
    static void ConvertItem(int FromPlugin,struct FarDialogItem *Item,struct DialogItem *Data,
                           int Count,BOOL InternalCall=FALSE);
    /* SVS $ */
    static void DataToItem(struct DialogData *Data,struct DialogItem *Item,
                           int Count);
    static int IsKeyHighlighted(const char *Str,int Key,int Translate,int AmpPos=-1);

    /* $ 31.07.2000 tran
       ����� ��� ����������� ������� */
    void AdjustEditPos(int dx,int dy);
    /* tran 31.07.2000 $ */

    /* $ 09.08.2000 KM
       ���������� �������, ������� ��������� ���������
       ��������� �� ������ � ������ �����������.
    */
    int IsMoving() {return DialogMode.Check(DMODE_DRAGGED);}
    /* KM $ */
    /* $ 10.08.2000 SVS
       ����� �� ������� ������ :-)
    */
    void SetModeMoving(int IsMoving) { DialogMode.Change(DMODE_ISCANMOVE,IsMoving);}
    int  GetModeMoving(void) {return DialogMode.Check(DMODE_ISCANMOVE);}
    /* SVS $ */
    /* $ 11.08.2000 SVS
       ������ � ���. ������� ���������� �������
    */
    void SetDialogData(long NewDataDialog);
    long GetDialogData(void) {return DataDialog;};
    /* SVS $ */

    void InitDialog(void);
    /* $ 11.08.2000 SVS
       ��� ����, ����� ������� DMSG_CLOSE ����� �������������� Process
    */
    void Process();
    /* SVS $ */
    /* $ 29.08.2000 SVS
       + ���������� ����� �������, ��� ������������ HelpTopic
    */
    void SetPluginNumber(int NewPluginNumber){PluginNumber=NewPluginNumber;}
    /* SVS $ */

    /* $ 17.05.2001 DJ */
    void SetHelp(const char *Topic);
    void ShowHelp();
    int Done() { return DialogMode.Check(DMODE_ENDLOOP); }
    void ClearDone();
    virtual void SetExitCode (int Code);

    void CloseDialog();
    /* DJ $ */

    /* $ 19.05.2001 DJ */
    // void SetOwnsItems (int AOwnsItems) { AOwnsItems = OwnsItems; } !!!!!!! :-)
    void SetOwnsItems (int AOwnsItems) { DialogMode.Change(DMODE_OWNSITEMS,AOwnsItems); }

    virtual int GetTypeAndName(char *Type,char *Name);
    virtual int GetType() { return MODALTYPE_DIALOG; }
    virtual const char *GetTypeName() {return "[Dialog]";};
    /* DJ $ */

    int GetMacroMode();

    /* $ ������� ��� ���� CtrlAltShift OT */
    int FastHide();
    void ResizeConsole();
//    void OnDestroy();

    // For MACRO
    const struct DialogItem *GetAllItem(){return Item;};
    int GetAllItemCount(){return ItemCount;};              // ���������� ��������� �������
    int GetDlgFocusPos(){return FocusPos;};


    int SetAutomation(WORD IDParent,WORD id,
                        DWORD UncheckedSet,DWORD UncheckedSkip,
                        DWORD CheckedSet,DWORD CheckedSkip,
                        DWORD Checked3Set=0,DWORD Checked3Skip=0);

    /* $ 23.07.2000 SVS: ������� ��������� ������� (�� ���������) */
    static LONG_PTR WINAPI DefDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2);
    /* $ 28.07.2000 SVS: ������� ������� ��������� ������� */
    static LONG_PTR WINAPI SendDlgMessage(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2);

    virtual void SetPosition(int X1,int Y1,int X2,int Y2);
};

#endif // __DIALOG_HPP__
