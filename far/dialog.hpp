#ifndef __DIALOG_HPP__
#define __DIALOG_HPP__
/*
dialog.hpp

����� ������� Dialog.

�।�����祭 ��� �⮡ࠦ���� �������� ��������.
������� �ந������ �� ����� Modal.

*/

/* Revision: 1.20 13.02.2001 $ */

/*
Modify:
  13.02.2001 SVS
   + �������⥫�� ��ࠬ��� � FindInEditForAC, SelectFromEditHistory,
     AddToEditHistory � SelectFromComboBox - MaxLen - ���ᨬ���� ࠧ���
     ��ப� �����祭��.
  24.09.2000 SVS
   + DMODE_ALTDRAGGED - �� �������� ������� �� Alt-��५��
  08.09.2000 SVS
   + �⨫� ������� DMODE_OLDSTYLE - ������ � ��஬ �⨫�.
   + �㭪�� SelectOnEntry - �뤥����� ��ப� ।���஢����
     (��ࠡ�⪠ 䫠�� DIF_SELECTONENTRY)
  30.08.2000 SVS
   + ����� ������� DMODE_SHOW - ������ �����?
   + ��⮤ Hide()
  29.08.2000 SVS
   ! �� ������� ⥬� ����� �� �������� ��楤���...
     ����, �㦭� ����� �ନ஢��� ���⥭�!
  24.08.2000 SVS
   + InitDialogObjects() ����� ��ࠬ��� - ��� �롮�筮� २��樠����樨
     ����⮢
  23.08.2000 SVS
   ! ��������� ��� DataDialog.
   + ��६����� ����� FocusPos - �ᥣ�� �����⭮ ����� ����� � 䮪��
   ! ��६���� IsCanMove, InitObjects, CreateObjects, WarningStyle, Dragged
     㤠���� -> ��⮢� 䫠�� � DialogMode
   ! ���ᨢ LV 㤠��� �� �������������.
   + CheckDialogMode - �㭪�� �஢�ન 䫠�� DialogMode
  22.08.2000 SVS
   ! � ���� ������᪨� ����� �� ��� :-((
     IsMovedDialog -> IsCanMove
     SetModeMoving -> SetMoveEnable
     GetModeMoving -> GetMoveEnable
   ! ShowDialog - �������⥫�� ��ࠬ��� - ����� ����� ���ᮢ뢠��
  18.08.2000 SVS
   + ���� IsEnableRedraw - ࠧ���騩/������騩 ����ᮢ�� �������
   + DialogMode - ����� ⥪�饣� ०��� �������
  11.08.2000 SVS
   + �����, ᯥ���᪨� ��� �����⭮�� ������� �������
   + ��� ⮣�, �⮡� ��᫠�� DMSG_CLOSE �㦭� ��८�।����� Process
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

// ����� ⥪�饣� ०��� �������
#define DMODE_INITOBJECTS	0x00000001 // ������ ���樠����஢���?
#define DMODE_CREATEOBJECTS	0x00000002 // ��ꥪ�� (Edit,...) ᮧ����?
#define DMODE_WARNINGSTYLE	0x00000004 // Warning Dialog Style?
#define DMODE_DRAGGED		0x00000008 // ������ ���������?
#define DMODE_ISCANMOVE		0x00000010 // ����� �� ������� ������?
#define DMODE_ALTDRAGGED	0x00000020 // ������ ��������� �� Alt-��५��?
#define DMODE_DRAWING		0x00001000 // ������ ������?
#define DMODE_KEY		0x00002000 // ���� ���뫪� ������?
#define DMODE_SHOW              0x00004000 // ������ �����?
#define DMODE_OLDSTYLE		0x80000000 // ������ � ��஬ (�� 1.70) �⨫�

// ����� ��� �㭪樨 ConvertItem
#define CVTITEM_TOPLUGIN	0
#define CVTITEM_FROMPLUGIN	1

// ࠧ��� ���ਨ...
#define HISTORY_COUNT 16

class Dialog:public Modal
{
  private:
    /* $ 29.08.2000 SVS
       + ����� �������, ��� �ନ஢���� HelpTopic
    */
    int PluginNumber;
    /* SVS $ */
    /* $ 23.08.2000 SVS
       + ��६����� ����� FocusPos
    */
    int FocusPos;               // �ᥣ�� �����⭮ ����� ����� � 䮪��
    /* SVS $ */
    int PrevFocusPos;           // �ᥣ�� �����⭮ ����� ����� �� � 䮪��
    /* $ 18.08.2000 SVS
      + ���� IsEnableRedraw - ࠧ���騩/������騩 ����ᮢ�� �������
      + DialogMode - ����� ⥪�饣� ०��� �������
    */
    int IsEnableRedraw;         // ����襭� ����ᮢ�� �������? ( 0 - ࠧ�襭�)
    DWORD DialogMode;		// ����� ⥪�饣� ०��� �������
    /* SVS $ */
    /* $ 11.08.2000 SVS
      + �����, ᯥ���᪨� ��� �����⭮�� ������� �������
    */
    long DataDialog;            // ��ࢮ��砫쭮 ����� ��ࠬ���,
                                //   ��।���� � ���������
    /* SVS $ */
    struct DialogItem *Item;    // ���ᨢ ����⮢ �������
    int ItemCount;              // ������⢮ ����⮢ �������

    char OldConsoleTitle[512];  // �।��騩 ��������� ���᮫�
    int DialogTooLong;          //
    int PrevMacroMode;          // �।��騩 ०�� ����

    FARWINDOWPROC DlgProc;      // �㭪�� ��ࠡ�⪨ �������

    /* $ 31.07.2000 tran
       ��६���� ��� ��६�饭�� ������� */
    int  OldX1,OldX2,OldY1,OldY2;
    /* tran 31.07.2000 $ */

  private:
    /* $ 18.08.2000 SVS
      + SetDialogMode - ��ࠢ����� 䫠���� ⥪�饣� ०��� �������
    */
    void SetDialogMode(DWORD Flags){ DialogMode|=Flags; }
    void SkipDialogMode(DWORD Flags){ DialogMode&=~Flags; }
    /* SVS $ */
    /* $ 23.08.2000 SVS
       + �஢�ઠ 䫠��
    */
    int CheckDialogMode(DWORD Flags){ return(DialogMode&Flags); }
    /* SVS $ */

    void DisplayObject();
    void DeleteDialogObjects();
    /* $ 22.08.2000 SVS
      ! ShowDialog - �������⥫�� ��ࠬ��� - ����� ����� ���ᮢ뢠��
    */
    void ShowDialog(int ID=-1);
    /* SVS $ */

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
    void SelectFromEditHistory(Edit *EditLine,char *HistoryName,char *Str,int MaxLen);
    /* SVS $ */
    /* $ 18.07.2000 SVS
       + �㭪�� SelectFromComboBox ��� �롮� �� DI_COMBOBOX
    */
    void SelectFromComboBox(Edit *EditLine,struct FarList *List,char *Str,int MaxLen);
    /* SVS $ */
    /* $ 26.07.2000 SVS
       AutoComplite: ���� �室����� �����ப� � ���ਨ
    */
    int FindInEditForAC(int TypeFind,void *HistoryName,char *FindStr,int MaxLen);
    /* SVS $ */
    void AddToEditHistory(char *AddStr,char *HistoryName,int MaxLen);
    int ProcessHighlighting(int Key,int FocusPos,int Translate);

    /* $ 08.09.2000 SVS
      �㭪�� SelectOnEntry - �뤥����� ��ப� ।���஢����
      ��ࠡ�⪠ 䫠�� DIF_SELECTONENTRY
    */
    void SelectOnEntry(int Pos);
    /* SVS $ */


  public:
    Dialog(struct DialogItem *Item,int ItemCount,FARWINDOWPROC DlgProc=NULL,long Param=NULL);
    ~Dialog();

  public:
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void Show();
    /* $ 30.08.2000 SVS
       ������� ���墠��� Hide()
    */
    void Hide();
    /* SVS $ */
    void FastShow() {ShowDialog();}
    /* $ 28.07.2000 SVS
       ������ InitDialogObjects �����頥� ID �����
       � 䮪�ᮬ �����
    */
    /* $ 24.08.2000 SVS
       InitDialogObjects ����� ��ࠬ��� - ��� �롮�筮� २��樠����樨
       ����⮢
    */
    int  InitDialogObjects(int ID=-1);
    /* 24.08.2000 SVS $ */
    /* SVS $ */
    void GetDialogObjectsData();
    void SetWarningStyle(int Style) {
      if(Style) SetDialogMode(DMODE_WARNINGSTYLE);
      else      SkipDialogMode(DMODE_WARNINGSTYLE);
    };

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
       ����� � ���. ����묨 ������� �������
    */
    void SetDialogData(long NewDataDialog);
    long GetDialogData(void) {return DataDialog;};
    /* SVS $ */

    /* $ 11.08.2000 SVS
       ��� ⮣�, �⮡� ��᫠�� DMSG_CLOSE �㦭� ��८�।����� Process
    */
    void Process();
    /* SVS $ */
    /* $ 29.08.2000 SVS
       + ��⠭����� ����� �������, ��� �ନ஢���� HelpTopic
    */
    void SetPluginNumber(int NewPluginNumber){PluginNumber=NewPluginNumber;}
    /* SVS $ */
};

#endif // __DIALOG_HPP__
