#ifndef __DIALOG_HPP__
#define __DIALOG_HPP__
/*
dialog.hpp

����� ������� Dialog.

������������ ��� ����������� ��������� ��������.
�������� ����������� �� ������ Frame.

*/

/* Revision: 1.35 12.07.2001 $ */

/*
Modify:
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
#define DMODE_OLDSTYLE      0x80000000 // ������ � ������ (�� 1.70) �����

// ����� ��� ������� ConvertItem
#define CVTITEM_TOPLUGIN    0
#define CVTITEM_FROMPLUGIN  1

/* $ 01.08.2000 SVS
  � �������� DialogI* ��������:
  union {
    unsigned int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
  } Addons;

*/
// for class Dialog
/*
��������� ���� ������� ������� - ��������� �������������.
��� �������� ��� FarDialogItem (�� ����������� ObjPtr)
*/
/* $ 12.08.2000 KM
   �������������� ����, ���������� ����� �����
*/
/* $ 08.12.2000 SVS
   Data "���������" � ����� ����������
*/
struct DialogItem
{
  unsigned char Type;
  unsigned char X1,Y1,X2,Y2;
  unsigned char Focus;
  union {
    unsigned int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
    CHAR_INFO *VBuf;
  };
  DWORD Flags;
  unsigned char DefaultButton;
  union {
    char Data[512];
    int  ListPos;
    struct {
      DWORD PtrFlags;
      int   PtrLength;
      void *PtrData;
      char  PtrTail[1];
    } Ptr;
  };
  void *ObjPtr;
  VMenu *ListPtr;
};
/* SVS $ */

/*
��������� ���� ������� ������� - ��� ���������� �������
��������� ����������� ��������� InitDialogItem (��. "Far PlugRinG
Russian Help Encyclopedia of Developer")
*/
struct DialogData
{
  unsigned char Type;
  unsigned char X1,Y1,X2,Y2;
  unsigned char Focus;
  union {
    unsigned int Selected;
    char *History;
    char *Mask;
    struct FarList *ListItems;
    CHAR_INFO *VBuf;
  };
  DWORD Flags;
  unsigned char DefaultButton;
  char *Data;
};
/* SVS $*/
/* KM $*/

class Edit;

class Dialog: public Frame
{
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
    DWORD DialogMode;       // ����� �������� ������ �������
    /* SVS $ */
    /* $ 11.08.2000 SVS
      + ������, ������������� ��� ����������� ���������� �������
    */
    long DataDialog;            // ������������� ����� ��������,
                                //   ���������� � �����������
    /* SVS $ */
    struct DialogItem *Item;    // ������ ��������� �������
    int ItemCount;              // ���������� ��������� �������

    char OldConsoleTitle[512];  // ���������� ��������� �������
    int DialogTooLong;          //
    int PrevMacroMode;          // ���������� ����� �����

    FARWINDOWPROC DlgProc;      // ������� ��������� �������

    /* $ 31.07.2000 tran
       ���������� ��� ����������� ������� */
    int  OldX1,OldX2,OldY1,OldY2;
    /* tran 31.07.2000 $ */

    /* $ 17.05.2001 DJ */
    char *HelpTopic;
    int  EndLoop;
    /* DJ $ */

    /* $ 19.05.2001 DJ
       ���� true, Dialog ����������� ������ Item � �����������
    */
    int OwnsItems;
    /* DJ $ */
    /* $ 23.06.2001 KM
       + �������� ������ ���������� � �������:
         TRUE - ������, FALSE - ������.
    */
    volatile int DropDownOpened;
    /* KM $ */

  private:
    /* $ 18.08.2000 SVS
      + SetDialogMode - ���������� ������� �������� ������ �������
    */
    void SkipDialogMode(DWORD Flags){ DialogMode&=~Flags; }
    /* SVS $ */
    /* $ 23.08.2000 SVS
       + �������� �����
    */
    int CheckDialogMode(DWORD Flags){ return(DialogMode&Flags); }
    /* SVS $ */

    void DisplayObject();
    void DeleteDialogObjects();
    /* $ 22.08.2000 SVS
      ! ShowDialog - �������������� �������� - ����� ������� ������������
    */
    void ShowDialog(int ID=-1);
    /* SVS $ */

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
    void SelectFromEditHistory(Edit *EditLine,char *HistoryName,char *Str,int MaxLen);
    /* SVS $ */
    /* $ 18.07.2000 SVS
       + ������� SelectFromComboBox ��� ������ �� DI_COMBOBOX
    */
    int SelectFromComboBox(Edit *EditLine,VMenu *List,char *Str,int MaxLen);
    /* SVS $ */
    /* $ 26.07.2000 SVS
       AutoComplite: ����� ��������� ��������� � �������
    */
    int FindInEditForAC(int TypeFind,void *HistoryName,char *FindStr,int MaxLen);
    /* SVS $ */
    int AddToEditHistory(char *AddStr,char *HistoryName);
    int ProcessHighlighting(int Key,int FocusPos,int Translate);

    /* $ 08.09.2000 SVS
      ������� SelectOnEntry - ��������� ������ ��������������
      ��������� ����� DIF_SELECTONENTRY
    */
    void SelectOnEntry(int Pos);
    /* SVS $ */

    void CheckDialogCoord(void);
    BOOL GetItemRect(int I,RECT& Rect);

    /* $ 19.05.2001 DJ
       ���������� ��������� ������� (����� ������� ������ ��� ������)
    */
    char *GetDialogTitle();
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

  public:
    Dialog(struct DialogItem *Item,int ItemCount,FARWINDOWPROC DlgProc=NULL,long Param=NULL);
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
    /* $ 24.08.2000 SVS
       InitDialogObjects ����� �������� - ��� ���������� ���������������
       ���������
    */
    int  InitDialogObjects(int ID=-1);
    /* 24.08.2000 SVS $ */
    /* SVS $ */
    void GetDialogObjectsData();

    void SetDialogMode(DWORD Flags){ DialogMode|=Flags; }

    /* $ 28.07.2000 SVS
       + ������� ConvertItem - �������������� �� ����������� �������������
        � FarDialogItem � �������
    */
    static void ConvertItem(int FromPlugin,struct FarDialogItem *Item,struct DialogItem *Data,
                           int Count,BOOL InternalCall=FALSE);
    /* SVS $ */
    static void DataToItem(struct DialogData *Data,struct DialogItem *Item,
                           int Count);
    static int IsKeyHighlighted(char *Str,int Key,int Translate,int AmpPos=-1);
    /* $ 23.07.2000 SVS
       ������� ��������� ������� (�� ���������)
    */
    static long WINAPI DefDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    /* SVS $ */
    /* $ 28.07.2000 SVS
       ������� ������� ��������� �������
    */
    static long WINAPI SendDlgMessage(HANDLE hDlg,int Msg,int Param1,long Param2);
    /* SVS $ */

    /* $ 31.07.2000 tran
       ����� ��� ����������� ������� */
    void AdjustEditPos(int dx,int dy);
    /* tran 31.07.2000 $ */

    /* $ 09.08.2000 KM
       ���������� �������, ������� ��������� ���������
       ��������� �� ������ � ������ �����������.
    */
    int IsMoving() {return CheckDialogMode(DMODE_DRAGGED);}
    /* KM $ */
    /* $ 10.08.2000 SVS
       ����� �� ������� ������ :-)
    */
    void SetModeMoving(int IsMoving) {
      if(IsMoving)
        SetDialogMode(DMODE_ISCANMOVE);
      else
        SkipDialogMode(DMODE_ISCANMOVE);
    };
    int  GetModeMoving(void) {return CheckDialogMode(DMODE_ISCANMOVE);};
    /* SVS $ */
    /* $ 11.08.2000 SVS
       ������ � ���. ������� ���������� �������
    */
    void SetDialogData(long NewDataDialog);
    long GetDialogData(void) {return DataDialog;};
    /* SVS $ */

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
    int Done() const
      { return EndLoop; }
    void ClearDone();
    virtual void SetExitCode (int Code);

    void CloseDialog();
    /* DJ $ */

    /* $ 19.05.2001 DJ */
    void SetOwnsItems (int AOwnsItems) { AOwnsItems = OwnsItems; }
    virtual int GetTypeAndName(char *Type,char *Name);
    virtual int GetType() { return MODALTYPE_DIALOG; }
    /* DJ $ */

    /* $ 20.05.2001 DJ */
    virtual void OnChangeFocus (int Focus);
    /* DJ $ */
    int GetMacroMode();

/* $ ������� ��� ���� CtrlAltShift OT */
    int FastHide();
    void ResizeConsole();
    void OnDestroy();

    bool Resized;


};

#endif // __DIALOG_HPP__
