#ifndef __VMENU_HPP__
#define __VMENU_HPP__
/*
vmenu.hpp

������� ������������ ����
  � ��� ��:
    * ������ � DI_COMBOBOX
    * ������ � DI_LISTBOX
    * ...

*/

/* Revision: 1.41 31.05.2002 $ */

/*
Modify:
  31.05.2002 SVS
    + SetMaxHeight()
  18.05.2002 SVS
    ! MouseDown -> VMENU_MOUSEDOWN
  28.04.2002 KM
    + VMENU_COMBOBOX
    + ���� ����� ���� ���� MODALTYPE_VMENU � MODALTYPE_COMBOBOX
  13.04.2002 KM
    - ??? � �� ����� ����� � ������ ���� ���� SaveScr,
      ���� � ScreenObj �� ��� ����. ��������� SaveScr ��
      ScreenObj ������� ���������� �� ������������� ���
      ���������� �������.
  21.02.2002 DJ
    ! ���������� ��������� �������, ������� �����, �� �� ������� ������
    + ������� ������������� ������� ������� � ����
  13.02.2002 SVS
    + ���� ���������� ������������� ����� ������� � CheckKeyHighlighted()
    + MenuItem.NamePtr
  11.02.2002 SVS
    + ���� AccelKey � MenuData � MenuItem
    + BitFlags
    ! � ������� UpdateItem() �������� ������ ���� ���� FarListUpdate
  01.12.2001 DJ
    - ���������� MenuItem::SetCheck()
  02.12.2001 KM
    + VMOldFlags
  30.11.2001 DJ
    - �������� VMENU_COLOR_COUNT ��������� � ������������ � �����������������
  06.11.2001 SVS
    ! VMENU_REVERSIHLIGHT -> VMENU_REVERSEHIGHLIGHT
  01.11.2001 SVS
    + ������� ��� "����" - GetType*()
  13.10.2001 VVM
    ! ������ ���� �� ��������� �� ���������� ������� �����, ���� ������� ���� ������ �� � ����.
  10.10.2001 IS
    ! ��������� const
  24.08.2001 VVM
    + void SetExitCode(int Code) - �������� ������� �� ������ Modal::
  31.07.2001 KM
    + ��������, � �� ����������� �� ����� ����� � ���...
      GetCallCount().
  26.07.2001 OT
    ����������� ��������� ShiftF10-F1-AltF9
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  18.07.2001 OT
    ! ����� ����� VFMenu. ��������� ���������, ���������� ��������� ����
  30.06.2001 KM
    + SetSelectPos(struct FarListPos *)
  + GetSelectPos(struct FarListPos *)
  29.06.2001 SVS
    + ����� �������� � FindItem - �����
  25.06.2001 IS
    ! ��������� const
  14.06.2001 SVS
    ! ����� -> VMENU_COLOR_COUNT
  10.06.2001 SVS
    + FindItem � ����� �����������.
  04.06.2001 SVS
    ! ��������� ��������� MenuItem
  03.06.2001 KM
    + ������� SetTitle, GetTitle, GetBottomTitle.
  03.06.2001 SVS
    ! ��������� MenuItem
    + GetPosition() - ���������� �������� ������� �����.
    + GetUserDataSize() - �������� ������ ������
    + SetUserData() - ������������� ������ � ������ ����
    ! GetUserData() - ���������� ��������� �� ���� ������
  30.05.2001 OT
    - �������� � ���������� VMenu. � ����� ����� Frame *FrameFromLaunched
      ������������ ��� �����, ������ ��� ���� �����������.
      ����� ����� �� �� ���������������, ����� ��� �� ������ :)
  25.05.2001 DJ
   + SetOneColor()
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
   + UpdateItem()
   + FarList2MenuItem()
   + MenuItem2FarList()
   + MenuItem.AmpPos - ������� ��������������� ��������� - ��� ����, �����
     ������� ��������� ����� �� ������, ������� �� ���������!
  12.05.2001 SVS
   + AddItem(char *NewStrItem);
  07.05.2001 SVS
   + AddItem, ���������� ���, ��� �������� ���� struct FarList - ���
     ���������� ���� � �������� :-)
   + SortItems() - ����� �� - ��� ��������
   * ������� ��� ������������� �������� ��� GetItemPtr() � ����� ������
     �������� ������� - Item
  06.05.2001 DJ
   ! �������� #include
  20.01.2001 SVS
   + SetSelectPos() - ���������� ������� �������!
  20.09.2000 SVS
   + ������� GetItemPtr - �������� ��������� �� ������ Item.
  01.08.2000 SVS
   + � ShowMenu �������� ��������, ���������� - ������� �� �������
     �������������� ��� �� ������ ������� ;-)
   ! ����� ������������
   ! ListBoxControl -> VMFlags
   + ����� ��� ��������� Flags � ������������
   + ������� ��������� ���� (�� ���������)
   + ������� ������� ��������� ����
   + ������� �������� N ������� ����
   ! ������� ����� ������������ ��� �������� �������-�����������!
  28.07.2000 SVS
   + ��������� �������� �������� (� ����������) � �������, ��������� �
     ����������:
  22.07.2000 SVS
   !  AlwaysScrollBar ������� �� ListBoxControl
  18.07.2000 SVS
    + ��������� ���������� ������ AlwaysScrollBar, ���������������
      ��� ����������� (������, �� ���� ����������!) � ���������
      DI_LISTBOX & DI_COMBOBOX
    ! � ����� � ���� ������� ����� ������������ ������.
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "modal.hpp"
#include "plugin.hpp"
#include "manager.hpp"
#include "frame.hpp"
#include "bitflags.hpp"

/* $ 30.11.2001 DJ
   �������� ��������� � ������������ � �����������������
*/

#define VMENU_COLOR_COUNT  10

/* DJ $ */

/* $ 28.07.2000 SVS
   �������� �������� - ������� � ������� ������
*/
enum{
  VMenuColorBody=0,      // ��������
  VMenuColorBox=1,       // �����
  VMenuColorTitle=2,     // ��������� - ������� � ������
  VMenuColorText=3,      // ����� ������
  VMenuColorHilite=4,    // HotKey
  VMenuColorSeparator=5, // separator
  VMenuColorSelected=6,  // ���������
  VMenuColorHSelect=7,   // ��������� - HotKey
  VMenuColorScrollBar=8, // ScrollBar
  VMenuColorDisabled=9,  // Disabled
};
/* SVS */

#define VMENU_ALWAYSSCROLLBAR       0x00000100
#define VMENU_LISTBOX               0x00000200
#define VMENU_SHOWNOBOX             0x00000400
#define VMENU_AUTOHIGHLIGHT         0x00000800
#define VMENU_REVERSEHIGHLIGHT      0x00001000
#define VMENU_UPDATEREQUIRED        0x00002000
#define VMENU_DISABLEDRAWBACKGROUND 0x00004000
#define VMENU_WRAPMODE              0x00008000
#define VMENU_SHOWAMPERSAND         0x00010000
#define VMENU_WARNDIALOG            0x00020000
#define VMENU_NOTCENTER             0x00040000
#define VMENU_LEFTMOST              0x00080000
#define VMENU_NOTCHANGE             0x00100000
/* $ 21.02.2002 DJ
   ���� �������� ������� � ������� � ����� �����
*/
#define VMENU_LISTHASFOCUS          0x00200000
/* DJ $ */
/* $ 28.04.2002 KM
    ���� �������� ����������� � ��������������
    ���������� ��-�������.
*/
#define VMENU_COMBOBOX              0x00400000
/* KM $ */
#define VMENU_MOUSEDOWN             0x00800000

class Dialog;
class SaveScreen;

struct MenuItem
{
  DWORD  Flags;                  // ����� ������
  union {
    char  Name[128];              // ����� ������
    char *NamePtr;
  };
  DWORD  AccelKey;
  int    UserDataSize;           // ������ ���������������� ������
  union {                        // ���������������� ������:
    char  *UserData;             // - ���������!
    char   Str4[4];              // - strlen(������)+1 <= 4
  };

  short AmpPos;                  // ������� ��������������� ���������
  short Len[2];		             // ������� 2-� ������
  short Idx2;		             // ������ 2-� �����

  /* $ 01.12.2001 DJ
     �������� ���, ������ ��������� ������������� ���
  */
  DWORD SetCheck(int Value)
  {
    if(Value)
    {
      Flags|=LIF_CHECKED;
      Flags &= ~0xFFFF;
      if(Value!=1) Flags|=Value&0xFFFF;
    }
    else
      Flags&=~(0xFFFF|LIF_CHECKED);
    return Flags;
  }
  /* DJ $ */

  DWORD SetSelect(int Value){ if(Value) Flags|=LIF_SELECTED; else Flags&=~LIF_SELECTED; return Flags;}
  DWORD SetDisable(int Value){ if(Value) Flags|=LIF_DISABLE; else Flags&=~LIF_DISABLE; return Flags;}
  char  operator[](int Pos) const;
                     // ����� ������� �� ���, ��� � ��� union ;-)
  char* PtrName();
};

struct MenuData
{
  char *Name;
  DWORD Flags;
  DWORD AccelKey;

  /* $ 01.12.2001 DJ
     �������� ���, ������ ��������� ������������� ���
  */
  DWORD SetCheck(int Value)
  {
    if(Value)
    {
      Flags &= ~0xFFFF;
      Flags|=((Value&0xFFFF)|LIF_CHECKED);
    }
    else
      Flags&=~(0xFFFF|LIF_CHECKED);
    return Flags;
  }
  /* DJ $ */

  DWORD SetSelect(int Value){ if(Value) Flags|=LIF_SELECTED; else Flags&=~LIF_SELECTED; return Flags;}
  DWORD SetDisable(int Value){ if(Value) Flags|=LIF_DISABLE; else Flags&=~LIF_DISABLE; return Flags;}
};

class VMenu: virtual public Modal, virtual public Frame
{
#ifdef _MSC_VER
#pragma warning(disable:4250)
#endif _MSC_VER
  private:
    char Title[100];
    char BottomTitle[100];
    int SelectPos;
    int TopPos;
    int MaxHeight;
    int MaxLength;
    int BoxType;
    int CallCount;
    int PrevMacroMode;
    /* $ 18.07.2000 SVS
       + ����������, ���������� �� ����������� scrollbar �
         DI_LISTBOX & DI_COMBOBOX
    */
    BitFlags VMFlags;
    BitFlags VMOldFlags;
    /* SVS $ */

    /* $ 01.08.2000 SVS
       + ��� LisBox - �������� � ���� �������
       + ���������� ����!
    */
    Dialog *ParentDialog;
    FARWINDOWPROC VMenuProc;      // ������� ��������� ����
    /* SVS $ */

    short RLen[2];	              // �������� ������� 2-� �������

  protected:
    /* $ 13.04.2002 KM
      - ??? � �� ����� ����� ����� ���� ���� SaveScr,
        ���� � ScreenObj ��� ���� ���� ����.
    */
//    SaveScreen *SaveScr;
    /* KM $ */
    struct MenuItem *Item;
    int ItemCount;
    /* $ 28.07.2000 SVS
       �������� ��������
    */
    short Colors[VMENU_COLOR_COUNT];
    /* SVS */

  public:
    Frame *FrameFromLaunched;

  private:
    void DisplayObject();
    void ShowMenu(int IsParent=0);
    int  GetPosition(int Position);
    static int _SetUserData(struct MenuItem *PItem,const void *Data,int Size);
    static void* _GetUserData(struct MenuItem *PItem,void *Data,int Size);
    BOOL CheckKeyHiOrAcc(DWORD Key,int Type,int Translate);

  public:
    /* $ 18.07.2000 SVS
       ! ������� ����� ������������ � ������ ������������� scrollbar �
         DI_LISTBOX & DI_COMBOBOX
         �� ��������� - ������� �� �������� ������ scrollbar � ����,
         �.�. �� ���������. ��� ������ ��������� (DI_LISTBOX & DI_COMBOBOX)
         �������� isListBoxControl ������ ���� ����� TRUE.
    */
    /* $ 01.08.2000 SVS
       ������� ����� ������������ ��� �������� �������-����������� � ��������!
    */
    VMenu(const char *Title,
          struct MenuData *Data,int ItemCount,
          int MaxHeight=0,
          DWORD Flags=0,
          FARWINDOWPROC Proc=NULL,
          Dialog *ParentDialog=NULL);
    /* 01.08.2000 SVS $ */
    /* SVS $ */
    ~VMenu();

  public:
    void FastShow() {ShowMenu();}
    void Show();
    void Hide();

    void SetTitle(const char *Title);
    char *GetTitle(char *Dest,int Size);
    void SetBottomTitle(const char *BottomTitle);
    char *GetBottomTitle(char *Dest,int Size);
    void SetDialogStyle(int Style) {VMFlags.Change(VMENU_WARNDIALOG,Style);SetColors(NULL);}
    void SetUpdateRequired(int SetUpdate) {VMFlags.Change(VMENU_UPDATEREQUIRED,SetUpdate);}
    void SetBoxType(int BoxType);

    void SetFlags(DWORD Flags){ VMFlags.Set(Flags); }
    void SkipFlags(DWORD Flags){ VMFlags.Skip(Flags); }
    int  CheckFlags(DWORD Flags){ return VMFlags.Check(Flags); }
    DWORD ChangeFlags(DWORD Flags,BOOL Status) {return VMFlags.Change(Flags,Status);}

    void AssignHighlights(int Reverse);
    void SetColors(short *Colors=NULL);
    void GetColors(short *Colors);

    /* $ 25.05.2001 DJ */
    void SetOneColor (int Index, short Color);
    /* DJ $ */

    int  ProcessKey(int Key);
    int  ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);

    BOOL UpdateRequired(void);

    void DeleteItems();
    /* $ 01.08.2000 SVS
       ������� �������� N ������� ����
    */
    int  DeleteItem(int ID,int Count=1);
    /* SVS $ */

    int  AddItem(const struct MenuItem *NewItem,int PosAdd=0x7FFFFFFF);
    int  AddItem(const struct FarList *NewItem);
    int  AddItem(const char *NewStrItem);

    int  InsertItem(const struct FarListInsert *NewItem);
    int  UpdateItem(const struct FarListUpdate *NewItem);
    int  FindItem(const struct FarListFind *FindItem);
    int  FindItem(int StartIndex,const char *Pattern,DWORD Flags=0);

    int  GetItemCount() {return(ItemCount);};
    /* $ 31.07.2001 KM
      + ��������, � �� ����������� �� ����� ����� � ���...
    */
    int  GetCallCount() { return CallCount; };
    /* KM $ */

    void *GetUserData(void *Data,int Size,int Position=-1);
    int  GetUserDataSize(int Position=-1);
    int  SetUserData(void *Data,int Size=0,int Position=-1);

    int  GetSelectPos() {return SelectPos;}
    int  GetSelectPos(struct FarListPos *ListPos);
    int  SetSelectPos(int Pos,int Direct);
    int  SetSelectPos(struct FarListPos *ListPos);
    int  GetSelection(int Position=-1);
    void SetSelection(int Selection,int Position=-1);
    /* $ 21.02.2002 DJ
       �������, ����������� ������������ ������� ������� � ������ SELECTED
    */
    void AdjustSelectPos();
    /* DJ $ */

    void Process();
    void ResizeConsole();

    /* $ 20.09.2000 SVS
      + ������� GetItemPtr - �������� ��������� �� ������ Item.
    */
    struct MenuItem *GetItemPtr(int Position=-1);
    /* SVS $*/

    void SortItems(int Direction=0);
    BOOL GetVMenuInfo(struct FarListInfo* Info);

    static struct MenuItem *FarList2MenuItem(const struct FarListItem *Item,struct MenuItem *ListItem);
    static struct FarListItem *MenuItem2FarList(const struct MenuItem *ListItem,struct FarListItem *Item);

    /* $ 01.08.2000 SVS
       ������� ��������� ���� (�� ���������)
    */
    static long WINAPI DefMenuProc(HANDLE hVMenu,int Msg,int Param1,long Param2);
    // ������� ������� ��������� ����
    static long WINAPI SendMenuMessage(HANDLE hVMenu,int Msg,int Param1,long Param2);
    /* SVS $ */

    void SetExitCode(int Code) {Modal::SetExitCode(Code);}

    virtual const char *GetTypeName() {return "[VMenu]";};
    virtual int GetTypeAndName(char *Type,char *Name);
    /* $ 28.04.2002 KM
        ���� ����� ���� ���� MODALTYPE_VMENU � MODALTYPE_COMBOBOX
    */
    virtual int GetType() { return CheckFlags(VMENU_COMBOBOX)?MODALTYPE_COMBOBOX:MODALTYPE_VMENU; }
    /* KM $ */
    void SetMaxHeight(int NewMaxHeight);
};


#endif  // __VMENU_HPP__
