#ifndef __VMENU_HPP__
#define __VMENU_HPP__
/*
vmenu.hpp

������� ������������ ����
  � ��� ��:
    * ������ � DI_COMBOBOX
    * ...

*/

/* Revision: 1.21 29.06.2001 $ */

/*
Modify:
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

#define VMENU_COLOR_COUNT  16

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
#define VMENU_LISTBOX	            0x00000200
#define VMENU_SHOWNOBOX             0x00000400
#define VMENU_AUTOHIGHLIGHT	        0x00000800
#define VMENU_REVERSIHLIGHT	        0x00001000
#define VMENU_UPDATEREQUIRED        0x00002000
#define VMENU_DISABLEDRAWBACKGROUND 0x00004000
#define VMENU_WRAPMODE              0x00008000
#define VMENU_SHOWAMPERSAND         0x00010000
#define VMENU_WARNDIALOG            0x00020000

class Dialog;
class SaveScreen;


struct MenuItem
{
  DWORD  Flags;                  // ����� ������
  char   Name[130];              // ����� ������
  short  AmpPos;                 // ������� ��������������� ���������
  int    UserDataSize;           // ������ ���������������� ������
  union {                        // ���������������� ������:
    char  *UserData;             // - ���������!
    char   Str4[4];              // - strlen(������)+1 <= 4
  };

  DWORD SetCheck(int Value){ if(Value) {Flags|=LIF_CHECKED; if(Value!=1) Flags|=Value&0xFFFF;} else Flags&=~(0xFFFF|LIF_CHECKED); return Flags;}
  DWORD SetSelect(int Value){ if(Value) Flags|=LIF_SELECTED; else Flags&=~LIF_SELECTED; return Flags;}
  DWORD SetDisable(int Value){ if(Value) Flags|=LIF_DISABLE; else Flags&=~LIF_DISABLE; return Flags;}
};

struct MenuData
{
  char *Name;
  DWORD Flags;

  DWORD SetCheck(int Value){ if(Value) Flags|=((Value&0xFFFF)|LIF_CHECKED); else Flags&=~(0xFFFF|LIF_CHECKED); return Flags;}
  DWORD SetSelect(int Value){ if(Value) Flags|=LIF_SELECTED; else Flags&=~LIF_SELECTED; return Flags;}
  DWORD SetDisable(int Value){ if(Value) Flags|=LIF_DISABLE; else Flags&=~LIF_DISABLE; return Flags;}
};

class VMenu: public Modal
{
  private:
    SaveScreen *SaveScr;
    char Title[100];
    char BottomTitle[100];
    int SelectPos,TopPos;
    int MaxHeight;
    int MaxLength;
    int BoxType;
    int CallCount;
    int PrevMacroMode;
    /* $ 18.07.2000 SVS
       + ����������, ���������� �� ����������� scrollbar �
         DI_LISTBOX & DI_COMBOBOX
    */
    DWORD VMFlags;
    /* SVS $ */

    /* $ 01.08.2000 SVS
       + ��� LisBox - �������� � ���� �������
       + ���������� ����!
    */
    Dialog *ParentDialog;
    FARWINDOWPROC VMenuProc;      // ������� ��������� ����
    /* SVS $ */

  protected:
    struct MenuItem *Item;
    int ItemCount;
    /* $ 28.07.2000 SVS
       �������� ��������
    */
    short Colors[VMENU_COLOR_COUNT];
    /* SVS */

  private:
    void DisplayObject();
    void ShowMenu(int IsParent=0);
    int  GetPosition(int Position);

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
    void FastShow() {ShowMenu();}
    void Show();
    void Hide();

    void SetTitle(const char *Title);
    char *GetTitle(char *Dest,int Size);
    void SetBottomTitle(const char *BottomTitle);
    char *GetBottomTitle(char *Dest,int Size);
    void SetDialogStyle(int Style) {ChangeFlags(VMENU_WARNDIALOG,Style);SetColors(NULL);}
    void SetUpdateRequired(int SetUpdate) {ChangeFlags(VMENU_UPDATEREQUIRED,SetUpdate);}
    void SetBoxType(int BoxType);

    void SetFlags(DWORD Flags){ VMFlags|=Flags; }
    void SkipFlags(DWORD Flags){ VMFlags&=~Flags; }
    int  CheckFlags(DWORD Flags){ return(VMFlags&Flags); }
    DWORD ChangeFlags(DWORD Flags,BOOL Status);

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

    int  AddItem(struct MenuItem *NewItem,int PosAdd=-1);
    int  AddItem(struct FarList *NewItem);
    int  AddItem(char *NewStrItem);

    int  InsertItem(struct FarListInsert *NewItem);
    int  UpdateItem(struct FarList *NewItem);
    int  FindItem(struct FarListFind *FindItem);
    int  FindItem(int StartIndex,char *Pattern,DWORD Flags=0);

    int  GetItemCount() {return(ItemCount);};

    void *GetUserData(void *Data,int Size,int Position=-1);
    int  GetUserDataSize(int Position=-1);
    int  SetUserData(void *Data,int Size=0,int Position=-1);

    int  GetSelectPos() {return SelectPos;}
    int  SetSelectPos(int Pos,int Direct);
    int  GetSelection(int Position=-1);
    void SetSelection(int Selection,int Position=-1);

    /* $ 20.09.2000 SVS
      + ������� GetItemPtr - �������� ��������� �� ������ Item.
    */
    struct MenuItem *GetItemPtr(int Position=-1);
    /* SVS $*/

    void SortItems(int Direction=0);
    BOOL GetVMenuInfo(struct FarListInfo* Info);

    static struct MenuItem *FarList2MenuItem(struct FarListItem *Items,struct MenuItem *ListItem);
    static struct FarListItem *MenuItem2FarList(struct MenuItem *ListItem,struct FarListItem *Items);

    /* $ 01.08.2000 SVS
       ������� ��������� ���� (�� ���������)
    */
    static long WINAPI DefMenuProc(HANDLE hVMenu,int Msg,int Param1,long Param2);
    // ������� ������� ��������� ����
    static long WINAPI SendMenuMessage(HANDLE hVMenu,int Msg,int Param1,long Param2);
    /* SVS $ */

    Frame *FrameFromLaunched;
};

#endif	// __VMENU_HPP__
