#ifndef __VMENU_HPP__
#define __VMENU_HPP__
/*
vmenu.hpp

���筮� ���⨪��쭮� ����
  � ⠪ ��:
    * ᯨ᮪ � DI_COMBOBOX
    * ...

*/

/* Revision: 1.03 28.07.2000 $ */

/*
Modify:
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
    int ListBoxControl;
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
    void ShowMenu();

  public:
    /* $ 18.07.2000 SVS
       ! ������� �맮� ��������� � ��⮬ ����室����� scrollbar �
         DI_LISTBOX & DI_COMBOBOX
         �� 㬮�砭�� - ������ �� ����஥� ������ scrollbar � ����,
         �.�. �� �ॡ����. ��� ������ ����⮢ (DI_LISTBOX & DI_COMBOBOX)
         ��ࠬ��� isListBoxControl ������ ���� ࠢ�� TRUE.
    */
    VMenu(char *Title,struct MenuData *Data,int ItemCount,int MaxHeight=0,int isListBoxControl=FALSE);
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
    int  AddItem(struct MenuItem *NewItem);
    int  GetItemCount() {return(ItemCount);};
    int  GetUserData(void *Data,int Size,int Position=-1);
    int  GetSelectPos();
    int  GetSelection(int Position=-1);
    void SetSelection(int Selection,int Position=-1);
};

#endif	// __VMENU_HPP__
