#ifndef __VMENU_HPP__
#define __VMENU_HPP__
/*
vmenu.hpp

���筮� ���⨪��쭮� ����

*/

/* Revision: 1.02 22.07.2000 $ */

/*
Modify:
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

class VMenu:public Modal
{
  private:
    void DisplayObject();
    void ShowMenu();
    SaveScreen *SaveScr;
    struct MenuItem *Item;
    int ItemCount;
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
    void DeleteItems();
    void FastShow() {ShowMenu();}
    void Show();
    void Hide();
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    int AddItem(struct MenuItem *NewItem);
    void SetBottomTitle(char *BottomTitle);
    void SetDialogStyle(int Style) {DialogStyle=Style;};
    void SetUpdateRequired(int SetUpdate) {UpdateRequired=SetUpdate;};
    void SetBoxType(int BoxType);
    void SetFlags(unsigned int Flags);
    int GetUserData(void *Data,int Size,int Position=-1);
    int GetSelection(int Position=-1);
    void SetSelection(int Selection,int Position=-1);
    int GetSelectPos();
    int GetItemCount() {return(ItemCount);};
    void AssignHighlights(int Reverse);
};

#endif	// __VMENU_HPP__
