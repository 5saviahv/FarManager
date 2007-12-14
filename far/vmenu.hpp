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

#include "modal.hpp"
#include "plugin.hpp"
#include "manager.hpp"
#include "frame.hpp"
#include "bitflags.hpp"
#include "CriticalSections.hpp"

#define VMENU_COLOR_COUNT  10

// �������� �������� - ������� � ������� ������
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

enum VMENU_FLAGS_TYPE{
  VMENU_ALWAYSSCROLLBAR       =0x00000100, // ������ ���������� ���������
  VMENU_LISTBOX               =0x00000200, // ��� ������ � �������
  VMENU_SHOWNOBOX             =0x00000400, // �������� ��� �����
  VMENU_AUTOHIGHLIGHT         =0x00000800, // ������������� �������� ������ ���������
  VMENU_REVERSEHIGHLIGHT      =0x00001000, // ... ������ � �����
  VMENU_UPDATEREQUIRED        =0x00002000, // ���� ���������� �������� (������������)
  VMENU_DISABLEDRAWBACKGROUND =0x00004000, // �������� �� ��������
  VMENU_WRAPMODE              =0x00008000, // ����������� ������ (��� �����������)
  VMENU_SHOWAMPERSAND         =0x00010000, // ������ '&' ���������� AS IS
  VMENU_WARNDIALOG            =0x00020000, //
  VMENU_NOTCENTER             =0x00040000, // �� ���������
  VMENU_LEFTMOST              =0x00080000, // "������� �����" - ���������� �� 5 ������� ������ �� ������ (X1 => (ScrX+1)/2+5)
  VMENU_NOTCHANGE             =0x00100000, //
  VMENU_LISTHASFOCUS          =0x00200000, // ���� �������� ������� � ������� � ����� �����
  VMENU_COMBOBOX              =0x00400000, // ���� �������� ����������� � �������������� ���������� ��-�������.
  VMENU_MOUSEDOWN             =0x00800000, //
  VMENU_CHANGECONSOLETITLE    =0x01000000, //
  VMENU_SELECTPOSNONE         =0x02000000, //
  VMENU_MOUSEREACTION         =0x04000000, // ����������� �� �������� ����? (���������� ������� ��� ����������� ������� ����?)
  VMENU_DISABLED              =0x80000000, //
};

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
  short Len[2];                  // ������� 2-� ������
  short Idx2;                    // ������ 2-� �����

  int   ShowPos;

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

  DWORD SetSelect(int Value){ if(Value) Flags|=LIF_SELECTED; else Flags&=~LIF_SELECTED; return Flags;}
  DWORD SetDisable(int Value){ if(Value) Flags|=LIF_DISABLE; else Flags&=~LIF_DISABLE; return Flags;}
};

class ConsoleTitle;

class VMenu: public Modal
{
#ifdef _MSC_VER
#pragma warning(disable:4250)
#endif //_MSC_VER
  private:
    char Title[100];
    char BottomTitle[100];
    int SelectPos;
    int TopPos;
    int MaxHeight;
    int MaxLength;
    int BoxType;
    int PrevCursorVisible;
    int PrevCursorSize;
    int PrevMacroMode;
    /* $ 18.07.2000 SVS
       + ����������, ���������� �� ����������� scrollbar �
         DI_LISTBOX & DI_COMBOBOX
    */
    BitFlags VMFlags;
    BitFlags VMOldFlags;
    /* SVS $ */

    Dialog *ParentDialog;
    int DialogItemID;
    FARWINDOWPROC VMenuProc;      // ������� ��������� ����

    short RLen[2];                // �������� ������� 2-� �������
    CRITICAL_SECTION CSection;
    ConsoleTitle *OldTitle;     // ���������� ���������

    CriticalSection CS;

  protected:
    /* $ 13.04.2002 KM
      - ??? � �� ����� ����� ����� ���� ���� SaveScr,
        ���� � ScreenObj ��� ���� ���� ����.
    */
//    SaveScreen *SaveScr;
    /* KM $ */
    struct MenuItem *Item;
    int ItemCount;

    int LastAddedItem;
    BYTE Colors[VMENU_COLOR_COUNT];

  public:
    Frame *FrameFromLaunched;

  private:
    virtual void DisplayObject();
    void ShowMenu(int IsParent=0);
    void DrawTitles();
    int  GetItemPosition(int Position);
    static int _SetUserData(struct MenuItem *PItem,const void *Data,int Size);
    static void* _GetUserData(struct MenuItem *PItem,void *Data,int Size);
    BOOL CheckKeyHiOrAcc(DWORD Key,int Type,int Translate);
    BOOL CheckHighlights(BYTE Chr);
    char GetHighlights(const struct MenuItem *_item);

  public:
    VMenu(const char *Title,
          struct MenuData *Data,int ItemCount,
          int MaxHeight=0,
          DWORD Flags=0,
          FARWINDOWPROC Proc=NULL,
          Dialog *ParentDialog=NULL);

    virtual ~VMenu();

  public:
    void FastShow() {ShowMenu();}
    virtual void Show();
    virtual void Hide();

    void SetTitle(const char *Title);
    char *GetTitle(char *Dest,int Size);
    const char *GetPtrTitle() { return Title; }


    void SetBottomTitle(const char *BottomTitle);
    char *GetBottomTitle(char *Dest,int Size);
    void SetDialogStyle(int Style) {VMFlags.Change(VMENU_WARNDIALOG,Style);SetColors(NULL);}
    void SetUpdateRequired(int SetUpdate) {VMFlags.Change(VMENU_UPDATEREQUIRED,SetUpdate);}
    void SetBoxType(int BoxType);

    void SetFlags(DWORD Flags){ VMFlags.Set(Flags); }
    void ClearFlags(DWORD Flags){ VMFlags.Clear(Flags); }
    int  CheckFlags(DWORD Flags){ return VMFlags.Check(Flags); }
    DWORD ChangeFlags(DWORD Flags,BOOL Status) {return VMFlags.Change(Flags,Status);}

    void AssignHighlights(int Reverse);

    void SetColors(struct FarListColors *Colors=NULL);
    void GetColors(struct FarListColors *Colors);

    void SetOneColor (int Index, short Color);

    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    virtual __int64 VMProcess(int OpCode,void *vParam=NULL,__int64 iParam=0);

    BOOL UpdateRequired(void);

    void DeleteItems();
    int  DeleteSelectedItems();
    int  DeleteItem(int ID,int Count=1);

    int  AddItem(const struct MenuItem *NewItem,int PosAdd=0x7FFFFFFF);
    int  AddItem(const struct FarList *NewItem);
    int  AddItem(const char *NewStrItem);

    int  InsertItem(const struct FarListInsert *NewItem);
    int  UpdateItem(const struct FarListUpdate *NewItem);
    int  FindItem(const struct FarListFind *FindItem);
    int  FindItem(int StartIndex,const char *Pattern,DWORD Flags=0);

    int  GetItemCount() {return(ItemCount);};

    void *GetUserData(void *Data,int Size,int Position=-1);
    int  GetUserDataSize(int Position=-1);
    int  SetUserData(void *Data,int Size=0,int Position=-1);

    int  GetSelectPos() {return VMFlags.Check(VMENU_SELECTPOSNONE)?-1:SelectPos;}
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

    virtual void Process();
    virtual void ResizeConsole();

    /* $ 20.09.2000 SVS
      + ������� GetItemPtr - �������� ��������� �� ������ Item.
    */
    struct MenuItem *GetItemPtr(int Position=-1);
    /* SVS $*/

    void SortItems(int Direction=0,int Offset=0,BOOL SortForDataDWORD=FALSE);
    BOOL GetVMenuInfo(struct FarListInfo* Info);

    virtual const char *GetTypeName() {return "[VMenu]";};
    virtual int GetTypeAndName(char *Type,char *Name);
    /* $ 28.04.2002 KM
        ���� ����� ���� ���� MODALTYPE_VMENU � MODALTYPE_COMBOBOX
    */
    virtual int GetType() { return CheckFlags(VMENU_COMBOBOX)?MODALTYPE_COMBOBOX:MODALTYPE_VMENU; }
    /* KM $ */
    void SetMaxHeight(int NewMaxHeight);

    int GetVDialogItemID() const {return DialogItemID;};
    void SetVDialogItemID(int NewDialogItemID) {DialogItemID=NewDialogItemID;};

  public:
    static struct MenuItem *FarList2MenuItem(const struct FarListItem *Item,struct MenuItem *ListItem);
    static struct FarListItem *MenuItem2FarList(const struct MenuItem *ListItem,struct FarListItem *Item);

    static LONG_PTR WINAPI DefMenuProc(HANDLE hVMenu,int Msg,int Param1,LONG_PTR Param2);
    static LONG_PTR WINAPI SendMenuMessage(HANDLE hVMenu,int Msg,int Param1,LONG_PTR Param2);

};


#endif  // __VMENU_HPP__
