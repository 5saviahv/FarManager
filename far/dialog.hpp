#ifndef __DIALOG_HPP__
#define __DIALOG_HPP__
/*
dialog.hpp

����� ������� Dialog.

�।�����祭 ��� �⮡ࠦ���� �������� ��������.
������� �ந������ �� ����� Modal.

*/

/* Revision: 1.10 10.08.2000 $ */

/*
Modify:
  10.08.2000 SVS
   + ��६����� IsMovedDialog - ����� �� ������� ������ :-)
   + �㭪�� ��⠭���� IsMovedDialog
  09.08.2000 KM 1.09
   + ���������� �㭪樨 �஢�ન �� ०�� ��६�饭�� �������.
     ���� ����� ।��樨 ����⢨⥫쭮 1.09 - ���� �� �ய�饭.
  01.08.2000 SVS
   - ��६����� ����� lastKey 㤠���� �� ������������� :-)
  31.07.2000 tran & SVS
   + ��६����� ����� Dragged - 䫠� ��६�饭��
     � ⠪�� OldX*, OldY*,
     ��⮤ - AdjustEditPos(int dx,int dy) - ���ࠢ������ ���न���� Edit"��
   + ���࠭���� ⮣�, �� ��� �������஬ ��६�饭�� �������
  28.07.2000 SVS
   + ��६����� ����� InitParam - �࠭�� ��ࠬ���, ��।����
     � ������.
   ! ������ InitDialogObjects �����頥� ID ����� � 䮪�ᮬ �����
   + �㭪�� ChangeFocus2
     ������� 䮪�� ����� ����� ���� ����⠬�.
   ! ��६��� Edit *EditLine � �㭪樨 FindInEditForAC ��䨣 ���㦥�!
   ! FindInEditHistory -> FindInEditForAC
     ���� ��� � ���ਨ, ⠪ � � ComboBox`� (�⮡� �� ������� ����)
   ! SelectFromComboBox ����� �������⥫�� ��ࠬ��� � ⥬, �⮡�
     ����樮��஢��� item � ���� � ᯨ᪮� � ᮮ⢥�ᢨ� � ��ப�� �����
   + �㭪�� IsFocused, ��।������ - "����� �� ����� �������
     ����� 䮪�� �����"
   ! IsEdit �⠫ Static-��⮤��!
   + �㭪�� ���뫪� ᮮ�饭�� ������� SendDlgMessage
   + �㭪�� ConvertItem - �८�ࠧ������ �� ����७���� �।�⠢�����
     � FarDialogItem � ���⭮
  26.07.2000 SVS
   + FindInEditHistory: ���� �室����� �����ப� � ���ਨ
  25.07.2000 SVS
   + Private: lastKey - ��� AutoComplit ��᫥���� ������
   + �������⥫�� ��ࠬ��� � SelectFromEditHistory ��� �뤥�����
     �㦭�� ����樨 � ���ਨ (�᫨ ��� ᮮ⢥����� ��ப� �����)
  25.07.2000 SVS
   ! ���� ��ࠬ��� � ���������
  23.07.2000 SVS
   + ��� ६�ப � ��室����� :-)
   + �㭪�� ��ࠡ�⪨ ������� (�� 㬮�砭��) - ����� ���� :-)
   ! ������� �맮� ���������
  18.07.2000 SVS
    + �㭪�� SelectFromComboBox ��� �롮� �� DI_COMBOBOX
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Dialog:public Modal
{
  private:
    struct DialogItem *Item;    // ���ᨢ ����⮢ �������
    int ItemCount;              // ������⢮ ����⮢ �������

    char OldConsoleTitle[512];  // �।��騩 ��������� ���᮫�
    int InitObjects;            // ������ ���樠����஢���?
    int CreateObjects;          // ��ꥪ�� (Edit,...) ᮧ����?
    int WarningStyle;           // TRUE - Warning Dialog Style
    int DialogTooLong;          //
    int PrevMacroMode;          // �।��騩 ०�� ����
    long InitParam;		// ��ࠬ���, ��।���� � ���������
    FARWINDOWPROC DlgProc;      // �㭪�� ��ࠡ�⪨ �������

    /* $ 31.07.2000 tran
       ��६���� ��� ��६�饭�� ������� */
    int  Dragged;
    int  OldX1,OldX2,OldY1,OldY2;
    /* tran 31.07.2000 $ */
    /* $ 10.08.2000 SVS
       ����� �� ������� ������ :-)
    */
    int IsMovedDialog;
    /* SVS $ */

    /* $ 31.07.2000 SVS
       ���࠭���� ⮣�, �� ��� �������஬ ��६�饭�� �������
    */
    CHAR_INFO LV[4];
    /* SVS $ */

  private:
    void DisplayObject();
    void DeleteDialogObjects();
    void ShowDialog();

    /* $ 28.07.2000 SVS
       + ������� 䮪�� ����� ����� ���� ����⠬�.
         �뭥ᥭ �⤥�쭮 ��� ⮣�, �⮡� ��ࠡ���� DMSG_KILLFOCUS & DMSG_SETFOCUS
    */
    int ChangeFocus2(int KillFocusPos,int SetFocusPos);
    /* SVS $ */
    int ChangeFocus(int FocusPos,int Step,int SkipGroup);
    static int IsEdit(int Type);
    /* $ 28.07.2000 SVS
       �㭪��, ��।������ - "����� �� ����� ������� ����� 䮪�� �����"
    */
    static int IsFocused(int Type);
    /* SVS $ */
    /* $ 26.07.2000 SVS
      + �������⥫�� ��ࠬ��� � SelectFromEditHistory ��� �뤥�����
       �㦭�� ����樨 � ���ਨ (�᫨ ��� ᮮ⢥����� ��ப� �����)
    */
    void SelectFromEditHistory(Edit *EditLine,char *HistoryName,char *Str);
    /* SVS $ */
    /* $ 18.07.2000 SVS
       + �㭪�� SelectFromComboBox ��� �롮� �� DI_COMBOBOX
    */
    void SelectFromComboBox(Edit *EditLine,struct FarListItems *HistoryName,char *Str=NULL);
    /* SVS $ */
    /* $ 26.07.2000 SVS
       AutoComplite: ���� �室����� �����ப� � ���ਨ
    */
    int FindInEditForAC(int TypeFind,void *HistoryName,char *FindStr);
    /* SVS $ */
    void AddToEditHistory(char *AddStr,char *HistoryName);
    int ProcessHighlighting(int Key,int FocusPos,int Translate);

  public:
    Dialog(struct DialogItem *Item,int ItemCount,FARWINDOWPROC DlgProc=NULL,long Param=NULL);
    ~Dialog();

  public:
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void Show();
    void FastShow() {ShowDialog();}
    /* $ 28.07.2000 SVS
       ������ InitDialogObjects �����頥� ID �����
       � 䮪�ᮬ �����
    */
    int  InitDialogObjects();
    /* SVS $ */
    void GetDialogObjectsData();
    void SetWarningStyle(int Style) {WarningStyle=Style;};

    /* $ 28.07.2000 SVS
       + �㭪�� ConvertItem - �८�ࠧ������ �� ����७���� �।�⠢�����
        � FarDialogItem � ���⭮
    */
    static void ConvertItem(int FromPlugin,struct FarDialogItem *Item,struct DialogItem *Data,
                           int Count);
    /* SVS $ */
    static void DataToItem(struct DialogData *Data,struct DialogItem *Item,
                           int Count);
    static int IsKeyHighlighted(char *Str,int Key,int Translate);
    /* $ 23.07.2000 SVS
       �㭪�� ��ࠡ�⪨ ������� (�� 㬮�砭��)
    */
    static long WINAPI DefDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    /* SVS $ */
    /* $ 28.07.2000 SVS
       �㭪�� ���뫪� ᮮ�饭�� �������
    */
    static long WINAPI SendDlgMessage(HANDLE hDlg,int Msg,int Param1,long Param2);
    /* SVS $ */

    /* $ 31.07.2000 tran
       ��⮤ ��� ��६�饭�� ������� */
    void AdjustEditPos(int dx,int dy);
    /* tran 31.07.2000 $ */

    /* $ 09.08.2000 KM
       ���������� �㭪樨, ����� �������� �஢����
       ��室���� �� ������ � ०��� ��६�饭��.
    */
    int IsMoving() {return Dragged;}
    /* KM $ */
    /* $ 10.08.2000 SVS
       ����� �� ������� ������ :-)
    */
    void SetModeMoving(int IsMoving) {IsMovedDialog=IsMoving;};
    int  GetModeMoving(void) {return IsMovedDialog;};
    /* SVS $ */
};

#endif // __DIALOG_HPP__
