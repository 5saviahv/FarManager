#ifndef __EDIT_HPP__
#define __EDIT_HPP__
/*
edit.hpp

��������� �����筮� ��ப� ।���஢����

*/

/* Revision: 1.11 14.02.2001 $ */

/*
Modify:
  14.02.2001 IS
    + ������ ⠡��樨 �࠭���� � TabSize, �����㫨஢��� �� ����� �� �����
      GetTabSize, SetTabSize
  13.12.2000 SVS
    + �������⥫�� ��ࠬ��� � �㭪樨  Xlat()
  16.11.2000 KM & SVS
    + KeyMatchedMask - �஢����: �������� �� ᨬ��� � ࠧ����
      �������� ᨬ�����, �ய�᪠���� ��᪮�
    ! ����⨪� (�� SVS :-)
  24.09.2000 SVS
    + ����� Editor - ���
    + �㭪�� Xlat - ��४���஢�� �� �ਭ樯� QWERTY <-> ������
  18.09.2000 SVS
    + ����� Dialog ���� "��㣮�" (�.�. ������ࠢ�� ᮢ������楬)
  12.08.2000 KM 1.06
    + ���� �㭪樨 SetInputMask � GetInputMask ��� ��⠭���� � ����祭�� ��᪨ �����.
    + ����� ��६����� Mask, ����� �࠭�� ���� ����� ��� ������� ��ꥪ� Edit.
    + ����� �㭪�� GetNextCursorPos, �������� ᫥���饥 ��������� �����
      � ��ப� ����� � ���⮬ Mask.
    + ����� �㭪�� RefreshStrByMask.
  03.08.2000 KM
    ! �������� �室�� ��ࠬ���� �㭪樨 Search - �������� ��ࠬ��� int WholeWords.
    ! ���������� � ���� �㭪樮���쭮�� ��宦����� 楫�� ᫮�
  28.07.2000 SVS
    + ����祭�� ���ᨬ��쭮�� ���祭�� ��ப� ��� ���ॡ���⥩
       Dialod API - GetMaxLength()
    + �㭪�� ����祭�� ⥪��� Color,SelColor
    + ��६����� ����� ColorUnChanged (��� �������)
    + �㭪�� GetObjectColorUnChanged ����祭�� ⥪�饣� ColorUnChanged
    ! SetObjectColor ����� �������⥫�� ��ࠬ��� ��� ��⠭���� ColorUnChanged
  25.07.2000 tran
    + DropDownBox style
  21.07.2000 tran
    + ReplaceSpaces()
  03.07.2000 tran
    + ReadOnly style
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Dialog;
class Editor;

class Edit:public ScreenObject
{
  friend class Dialog;
  friend class Editor;

  private:
    char  *Str;
    /* $ 12.08.2000 KM
       ��६����� ��� �࠭���� ��᪨ �����
    */
    char  *Mask;
    /* KM $ */
    struct CharTableSet *TableSet;
    struct ColorItem *ColorList;
    int    ColorCount;
    int    StrSize;
    /* $ 12.08.2000 KM
       ��������� ��६����� ��� �࠭���� �।��饣� ��������� �����
    */
    int    CurPos,PrevCurPos;
    /* KM $ */
    /* $ 28.07.2000 SVS
      + ColorUnChanged (��� �������)
    */
    int    Color,SelColor,ColorUnChanged;
    /* SVS $ */
    int    LeftPos;
    int    ConvertTabs;
    int    CursorPos;
    int    EndType;
    int    MaxLength;
    int    SelStart,SelEnd;
    char   MarkingBlock;
    char   ClearFlag;
    char   PasswordMode;
    char   EditBeyondEnd;
    char   Overtype;
    char   EditorMode;
    /* $ 03.07.2000 tran
       + ReadOnly style*/
    int    ReadOnly;
    /* tran 03.07.2000 $ */
    /* $ 14.02.2001 IS
         ������ ⠡��樨 - �� 㬮�砭�� ࠢ�� Opt.TabSize;
    */
    int	   TabSize;
    /* IS $ */

  private:
    void   DisplayObject();
    void   ShowString(char *ShowStr,int TabSelStart,int TabSelEnd);
    int    InsertKey(int Key);
    int    RecurseProcessKey(int Key);
    void   DeleteBlock();
    void   ApplyColor();
    /* $ 12.08.2000 KM
       ����७��� �㭪�� ����� ����� Mask � �����頥�
       ᫥���饥 ��������� ��������� ����� � ��ப� �����,
       + �㭪�� ���������� ᮤ�ন��� Str �� �᭮����� Mask.
    */
    int    GetNextCursorPos(int Position,int Where);
    void   RefreshStrByMask(int InitMode=FALSE);
    /* KM $ */
    /* $ 15.11.2000 KM
       �஢����: �������� �� ᨬ��� � ࠧ����
       �������� ᨬ�����, �ய�᪠���� ��᪮�
    */
    int KeyMatchedMask(int Key);
    /* KM $ */

  public:
    Edit();
    ~Edit();

  public:
    void  Show() {DisplayObject();}
    void  FastShow();
    int   ProcessKey(int Key);
    int   ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    /* $ 28.07.2000 SVS
       ! �㭪�� ��⠭���� ⥪��� Color,SelColor � ColorUnChanged!
    */
    void  SetObjectColor(int Color,int SelColor=0xf,int ColorUnChanged=COL_DIALOGEDITUNCHANGED);
    //   + �㭪�� ����祭�� ⥪��� Color,SelColor
    long  GetObjectColor() {return MAKELONG(Color,SelColor);}
    int   GetObjectColorUnChanged() {return ColorUnChanged;}
    /* SVS $*/
    /* $ 14.02.2001 IS
         �㭪樨 �⥭��/��⠭������� ࠧ��� ⠡��樨
    */
    void SetTabSize(int NewSize) { TabSize=NewSize; }
    int  GetTabSize(void) {return TabSize; }
    /* IS $ */
    void  GetString(char *Str,int MaxSize);
    char* GetStringAddr();
    void  SetString(char *Str);
    void  SetBinaryString(char *Str,int Length);
    void  GetBinaryString(char **Str,char **EOL,int &Length);
    void  SetEOL(char *EOL);
    int   GetSelString(char *Str,int MaxSize);
    int   GetLength();
    void  InsertString(char *Str);
    void  InsertBinaryString(char *Str,int Length);
    /* $ 03.08.2000 KM
       ���������� ��ࠬ��� WholeWords ��� ���᪠ 楫�� ᫮�
       � �㭪�� Search.
    */
    int   Search(char *Str,int Position,int Case,int WholeWords,int Reverse);
    /* KM $ */
    void  SetClearFlag(int Flag) {ClearFlag=Flag;}
    void  SetCurPos(int NewPos) {CurPos=NewPos;PrevCurPos=NewPos;}
    int   GetCurPos() {return(CurPos);}
    int   GetTabCurPos();
    int   GetLeftPos() {return(LeftPos);}
    void  SetLeftPos(int NewPos) {LeftPos=NewPos;}
    void  SetTabCurPos(int NewPos);
    void  SetPasswordMode(int Mode) {PasswordMode=Mode;};
    void  SetMaxLength(int Length) {MaxLength=Length;};
    /* $ 28.07.2000 SVS
       ����祭�� ���ᨬ��쭮�� ���祭�� ��ப� ��� ���ॡ���⥩ Dialod API
    */
    int   GetMaxLength() {return MaxLength;};
    /* SVS $ */
    /* $ 12.08.2000 KM
       �㭪樨 ��⠭���� � ����祭�� ��᪨ �����
    */
    void  SetInputMask(char *InputMask);
    char* GetInputMask() {return Mask;}
    /* KM $ */
    void  SetOvertypeMode(int Mode) {Overtype=Mode;};
    int   GetOvertypeMode() {return(Overtype);};
    void  SetConvertTabs(int Mode) {ConvertTabs=Mode;};
    int   RealPosToTab(int Pos);
    int   TabPosToReal(int Pos);
    void  SetTables(struct CharTableSet *TableSet);
    void  Select(int Start,int End);
    void  AddSelect(int Start,int End);
    void  GetSelection(int &Start,int &End);
    void  GetRealSelection(int &Start,int &End);
    void  SetEditBeyondEnd(int Mode) {EditBeyondEnd=Mode;};
    void  SetEditorMode(int Mode) {EditorMode=Mode;};
    void  ReplaceTabs();
    void  AddColor(struct ColorItem *col);
    int   DeleteColor(int ColorPos);
    int   GetColor(struct ColorItem *col,int Item);

#ifdef SHITHAPPENS
    void ReplaceSpaces(int i);
#endif
    /* $ 25.07.2000 tran
       + DropDownBox style */
    int DropDownBox;
    /* tran 25.07.2000 $ */
    /* $ 24.09.2000 SVS $
      �㭪�� Xlat - ��४���஢�� �� �ਭ樯� QWERTY <-> ������
    */
    /* $ 13.12.2000 SVS
       �������⥫�� ��ࠬ��� � �㭪樨  Xlat()
    */
    void Xlat(BOOL All=FALSE);
    /* SVS $ */
    /* SVS $ */

    static void DisableEditOut(int Disable);
    static void DisableEncode(int Disable);
};

#endif  // __EDIT_HPP__
