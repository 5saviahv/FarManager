#ifndef __VMENU_HPP__
#define __VMENU_HPP__
/*
vmenu.hpp

���筮� ���⨪��쭮� ����
  � ⠪ ��:
    * ᯨ᮪ � DI_COMBOBOX
    * ...

*/

/* Revision: 1.04 01.08.2000 $ */

/*
Modify:
  01.08.2000 SVS
   + � ShowMenu �������� ��ࠬ���, ᮮ���騩 - �맢��� �� �㭪��
     ᠬ����⥫쭮 ��� �� ��㣮� �㭪樨 ;-)
   ! �맮� ���������
   ! ListBoxControl -> VMFlags
   + ����� ��� ��ࠬ��� Flags � ���������
   + �㭪�� ��ࠡ�⪨ ���� (�� 㬮�砭��)
   + �㭪�� ���뫪� ᮮ�饭�� ����
   + �㭪�� 㤠����� N �㭪⮢ ����
   ! ������� �맮� ��������� ��� 㪠����� �㭪樨-��ࠡ��稪�!
  28.07.2000 SVS
   + ��������� 梥⮢� ��ਡ��� (� ��६�����) � �㭪樨, �易��� �
     ��ਡ�⠬�:
  22.07.2000 SVS
   !  AlwaysScrollBar ������� �� ListBoxControl
  18.07.2000 SVS
    + ��������� ��६����� ����� AlwaysScrollBar, �।�����祭���
      ��� �⮡ࠦ���� (�ᥣ��, �� ��� ���������!) � ������
      DI_LISTBOX & DI_COMBOBOX
    ! � �裡 � �⨬ ������� �맮� ��������� �����.
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

/* $ 28.07.2000 SVS
   ���⮢� ��ਡ��� - ������� � ���ᨢ� 梥⮢
*/
enum{
  VMenuColorBody=0,      // ��������
  VMenuColorBox=1,       // ࠬ��
  VMenuColorTitle=2,     // ��������� - ���孨� � ������
  VMenuColorText=3,      // ����� �㭪�
  VMenuColorHilite=4,    // HotKey
  VMenuColorSeparator=5, // separator
  VMenuColorSelected=6,  // ��࠭��
  VMenuColorHSelect=7,   // ��࠭�� - HotKey
  VMenuColorScrollBar=8  // ScrollBar
};
/* SVS */

/* $ 01.08.2000 SVS
   ����⠭�� ��� 䫠��� - ��� ���������
*/
#define VMENU_ALWAYSSCROLLBAR 0x0100
#define VMENU_LISTBOX	      0x0200
/* SVS $ */

class Dialog;

class VMenu:public Modal
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
       + ��६�����, �⢥���� �� �⮡ࠦ���� scrollbar �
         DI_LISTBOX & DI_COMBOBOX
    */
    DWORD VMFlags;
    /* SVS $ */

    /* $ 01.08.2000 SVS
       + ��� LisBox - த�⥫� � ���� �������
       + ��ࠡ��稪 ����!
    */
    Dialog *ParentDialog;
    FARWINDOWPROC VMenuProc;      // �㭪�� ��ࠡ�⪨ ����
    /* SVS $ */

  protected:
    struct MenuItem *Item;
    int ItemCount;
    /* $ 28.07.2000 SVS
       ���⮢� ��ਡ���
    */
    short Colors[9];
    /* SVS */

  private:
    void DisplayObject();
    void ShowMenu(int IsParent=0);

  public:
    /* $ 18.07.2000 SVS
       ! ������� �맮� ��������� � ��⮬ ����室����� scrollbar �
         DI_LISTBOX & DI_COMBOBOX
         �� 㬮�砭�� - ������ �� ����஥� ������ scrollbar � ����,
         �.�. �� �ॡ����. ��� ������ ����⮢ (DI_LISTBOX & DI_COMBOBOX)
         ��ࠬ��� isListBoxControl ������ ���� ࠢ�� TRUE.
    */
    /* $ 01.08.2000 SVS
       ������� �맮� ��������� ��� 㪠����� �㭪樨-��ࠡ��稪� � த�⥫�!
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
       �㭪�� 㤠����� N �㭪⮢ ����
    */
    int  DeleteItem(int ID,int Count=1);
    /* SVS $ */
    int  AddItem(struct MenuItem *NewItem);
    int  GetItemCount() {return(ItemCount);};
    int  GetUserData(void *Data,int Size,int Position=-1);
    int  GetSelectPos();
    int  GetSelection(int Position=-1);
    void SetSelection(int Selection,int Position=-1);

    /* $ 01.08.2000 SVS
       �㭪�� ��ࠡ�⪨ ���� (�� 㬮�砭��)
    */
    static long WINAPI DefMenuProc(HANDLE hVMenu,int Msg,int Param1,long Param2);
    // �㭪�� ���뫪� ᮮ�饭�� ����
    static long WINAPI SendMenuMessage(HANDLE hVMenu,int Msg,int Param1,long Param2);
    /* SVS $ */
};

#endif	// __VMENU_HPP__
