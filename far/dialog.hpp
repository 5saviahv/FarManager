#ifndef __DIALOG_HPP__
#define __DIALOG_HPP__
/*
dialog.hpp

����� ������� Dialog.

������������ ��� ����������� ��������� ��������.
�������� ����������� �� ������ Frame.

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
  DLGIIF_COMBOBOXNOREDRAWEDIT     = 0x00000008, // �� ������������� ������ �������������� ��� ���������� � �����
  DLGIIF_COMBOBOXEVENTKEY         = 0x00000010, // �������� ������� ���������� � ���������� ����. ��� ��������� ����������
  DLGIIF_COMBOBOXEVENTMOUSE       = 0x00000020, // �������� ������� ���� � ���������� ����. ��� ��������� ����������
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
    DWORD_PTR Reserved;
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
    DWORD_PTR Reserved;
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
    LONG_PTR DataDialog;        // ������������� ����� ��������,
                                //   ���������� � �����������
    /* SVS $ */
    struct DialogItem *Item;    // ������ ��������� �������
    int ItemCount;              // ���������� ��������� �������

    ConsoleTitle *OldTitle;     // ���������� ���������
    int DialogTooLong;          //
    int PrevMacroMode;          // ���������� ����� �����

    FARWINDOWPROC RealDlgProc;  // ������� ��������� �������

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
    virtual void DisplayObject();
    void DeleteDialogObjects();
    int  LenStrItem(int ID,char *Str=NULL);
    /* $ 22.08.2000 SVS
      ! ShowDialog - �������������� �������� - ����� ������� ������������
        ID=-1 - ���������� ���� ������
    */
    void ShowDialog(int ID=-1);
    /* SVS $ */

    LONG_PTR CtlColorDlgItem(int ItemPos,int Type,int Focus,DWORD Flags);
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
    virtual ~Dialog();

  public:
    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    virtual __int64 VMProcess(int OpCode,void *vParam=NULL,__int64 iParam=0);
    virtual void Show();
    /* $ 30.08.2000 SVS
       ������� ����������� Hide()
    */
    virtual void Hide();
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
    void SetDialogData(LONG_PTR NewDataDialog);
    LONG_PTR GetDialogData(void) {return DataDialog;};
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

    virtual int GetMacroMode();

    /* $ ������� ��� ���� CtrlAltShift OT */
    virtual int FastHide();
    virtual void ResizeConsole();
//  virtual void OnDestroy();

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

    LONG_PTR WINAPI DlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2);

    virtual void SetPosition(int X1,int Y1,int X2,int Y2);
};

#endif // __DIALOG_HPP__
