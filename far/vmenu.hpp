#ifndef __VMENU_HPP__
#define __VMENU_HPP__
/*
vmenu.hpp

������� ������������ ����
  � ��� ��:
    * ������ � DI_COMBOBOX
    * ...

*/

/* Revision: 1.08 07.05.2001 $ */

/*
Modify:
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
  VMenuColorScrollBar=8  // ScrollBar
};
/* SVS */

/* $ 01.08.2000 SVS
   ��������� ��� ������ - ��� ������������
*/
#define VMENU_ALWAYSSCROLLBAR 0x0100
#define VMENU_LISTBOX	      0x0200
#define VMENU_SHOWNOBOX       0x0400
//#define VMENU_LISTBOXSORT	  0x0800
/* SVS $ */

class Dialog;
class SaveScreen;

struct MenuItem
{
  DWORD Flags;
  char Name[128];
  unsigned char Selected;
  unsigned char Checked;
  unsigned char Separator;
  unsigned char Disabled;
  char  UserData[sizeof(WIN32_FIND_DATA)+NM+10];
  int   UserDataSize;
  char *PtrData;
};


struct MenuData
{
  char *Name;
  unsigned char Selected;
  unsigned char Checked;
  unsigned char Separator;
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
    int DrawBackground,BoxType,WrapMode,ShowAmpersand;
    int UpdateRequired;
    int DialogStyle;
    int AutoHighlight;
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
    short Colors[9];
    /* SVS */

  private:
    void DisplayObject();
    void ShowMenu(int IsParent=0);

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
    VMenu(char *Title,
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

    void SetBottomTitle(char *BottomTitle);
    void SetDialogStyle(int Style) {DialogStyle=Style;SetColors(NULL);};
    void SetUpdateRequired(int SetUpdate) {UpdateRequired=SetUpdate;};
    void SetBoxType(int BoxType);
    void SetFlags(unsigned int Flags);

    void AssignHighlights(int Reverse);
    void SetColors(short *Colors=NULL);
    void GetColors(short *Colors);

    int  ProcessKey(int Key);
    int  ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);

    void DeleteItems();
    /* $ 01.08.2000 SVS
       ������� �������� N ������� ����
    */
    int  DeleteItem(int ID,int Count=1);
    /* SVS $ */
    int  AddItem(struct MenuItem *NewItem);
    int  AddItem(struct FarList *NewItem);
    int  GetItemCount() {return(ItemCount);};
    int  GetUserData(void *Data,int Size,int Position=-1);
    int  GetSelectPos();
    int  SetSelectPos(int Pos,int Direct);
    int  GetSelection(int Position=-1);
    void SetSelection(int Selection,int Position=-1);

    /* $ 20.09.2000 SVS
      + ������� GetItemPtr - �������� ��������� �� ������ Item.
    */
    struct MenuItem *GetItemPtr(int Position=-1);
    /* SVS $*/

    void SortItems(int Direction=0);

    /* $ 01.08.2000 SVS
       ������� ��������� ���� (�� ���������)
    */
    static long WINAPI DefMenuProc(HANDLE hVMenu,int Msg,int Param1,long Param2);
    // ������� ������� ��������� ����
    static long WINAPI SendMenuMessage(HANDLE hVMenu,int Msg,int Param1,long Param2);
    /* SVS $ */
};

#endif	// __VMENU_HPP__
