#ifndef __EDIT_HPP__
#define __EDIT_HPP__
/*
edit.hpp

���������� ��������� ������ ��������������

*/

/* Revision: 1.17 23.11.2001 $ */

/*
Modify:
  23.11.2001 SVS
    ! IsDialogParent ����� ����� 3 ��������: EDPARENT_*
    + ������ �������� "if(Mask[...)" ������� ������� CheckCharMask()
  08.11.2001 SVS
    + IsDialogParent - � �������� � ��� ���?
  22.06.2001 SVS
    + ProcessInsDate()
  06.05.2001 DJ
    ! �������� #include
  13.04.2001 SVS
    ! ��������� Ctrl-Q �������� � ��������� ������� ProcessCtrlQ(), �.�.
      ������������ � editor.cpp
  15.02.2001 IS
    + ��������� ����������, � ������� ������������ ��, ��� ��������� �
      ���������� ���������:
      DelRemovesBlocks - "Del ������� �����"
      PersistentBlocks - "���������� �����"
    + ������� ��� ���������� �� ����������:
      SetDelRemovesBlocks/GetDelRemovesBlocks
      SetPersistentBlocks/GetPersistentBlocks
  14.02.2001 IS
    + ������ ��������� �������� � TabSize, �������������� �� ����� ��� ������
      GetTabSize, SetTabSize
  13.12.2000 SVS
    + �������������� �������� � �������  Xlat()
  16.11.2000 KM & SVS
    + KeyMatchedMask - ���������: �������� �� ������ � �����������
      �������� ��������, ������������ ������
    ! �������� (�� SVS :-)
  24.09.2000 SVS
    + ����� Editor - ����
    + ������� Xlat - ������������� �� �������� QWERTY <-> ������
  18.09.2000 SVS
    + ����� Dialog �������� "������" (�.�. ������������ ������������)
  12.08.2000 KM 1.06
    + ����� ������� SetInputMask � GetInputMask ��� ��������� � ��������� ����� �����.
    + ����� ���������� Mask, ������� ������ ����� ����� ��� ������� ������� Edit.
    + ����� ������� GetNextCursorPos, ����������� ��������� ��������� �������
      � ������ ����� � ������ Mask.
    + ����� ������� RefreshStrByMask.
  03.08.2000 KM
    ! �������� ������� ��������� ������� Search - �������� �������� int WholeWords.
    ! ���������� � ����� ���������������� ���������� ����� ����
  28.07.2000 SVS
    + ��������� ������������� �������� ������ ��� ������������
       Dialod API - GetMaxLength()
    + ������� ��������� ������� Color,SelColor
    + ���������� ������ ColorUnChanged (��� �������)
    + ������� GetObjectColorUnChanged ��������� �������� ColorUnChanged
    ! SetObjectColor ����� �������������� �������� ��� ��������� ColorUnChanged
  25.07.2000 tran
    + DropDownBox style
  21.07.2000 tran
    + ReplaceSpaces()
  03.07.2000 tran
    + ReadOnly style
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "scrobj.hpp"
#include "colors.hpp"

// ��������� ��� Edit::IsDialogParent
enum {
  EDPARENT_NONE=0,       // ��� ���� �� � ������� �������
  EDPARENT_SINGLELINE=1, // ������� ������ ����� � �������
  EDPARENT_MULTILINE=2,  // ��� �������� Memo-Edit (DI_EDITOR ��� DIF_MULTILINE)
};

class Dialog;
class Editor;

class Edit:public ScreenObject
{
  friend class Dialog;
  friend class Editor;

  private:
    char  *Str;
    /* $ 12.08.2000 KM
       ���������� ��� �������� ����� �����
    */
    char  *Mask;
    /* KM $ */
    struct CharTableSet *TableSet;
    struct ColorItem *ColorList;
    int    ColorCount;
    int    StrSize;
    /* $ 12.08.2000 KM
       ��������� ���������� ��� �������� ����������� ��������� �������
    */
    int    CurPos;
    int    PrevCurPos;
    /* KM $ */
    /* $ 28.07.2000 SVS
      + ColorUnChanged (��� �������)
    */
    int    Color;
    int    SelColor;
    int    ColorUnChanged;
    /* SVS $ */
    int    LeftPos;
    int    ConvertTabs;
    int    CursorPos;
    int    EndType;
    int    MaxLength;
    int    SelStart;
    int    SelEnd;
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
         ������ ��������� - �� ��������� ����� Opt.TabSize;
    */
    int    TabSize;
    /* IS $ */
    /* $ 15.02.2001 IS
         ��������� ����� �� �������� ��������� ������ ������������ ��������
    */
    int    DelRemovesBlocks; // Del ������� ����� (Opt.EditorDelRemovesBlocks)
    int    PersistentBlocks; // ���������� ����� (Opt.EditorPersistentBlocks)
    /* IS $ */

    int    IsDialogParent;   // ������� �������������� ������ � �������

  private:
    void   DisplayObject();
    void   ShowString(char *ShowStr,int TabSelStart,int TabSelEnd);
    int    InsertKey(int Key);
    int    RecurseProcessKey(int Key);
    void   DeleteBlock();
    void   ApplyColor();
    /* $ 12.08.2000 KM
       ���������� ������� ������� ������ Mask � ����������
       ��������� ��������� ��������� ������� � ������ �����,
       + ������� ����������� ���������� Str �� ��������� Mask.
    */
    int    GetNextCursorPos(int Position,int Where);
    void   RefreshStrByMask(int InitMode=FALSE);
    /* KM $ */
    /* $ 15.11.2000 KM
       ���������: �������� �� ������ � �����������
       �������� ��������, ������������ ������
    */
    int KeyMatchedMask(int Key);
    /* KM $ */

    int ProcessCtrlQ(void);
    int ProcessInsDate(void);
    int CheckCharMask(char Chr);

  public:
    Edit();
    ~Edit();

  public:
    void  Show() {DisplayObject();}
    void  FastShow();
    int   ProcessKey(int Key);
    int   ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    /* $ 28.07.2000 SVS
       ! ������� ��������� ������� Color,SelColor � ColorUnChanged!
    */
    void  SetObjectColor(int Color,int SelColor=0xf,int ColorUnChanged=COL_DIALOGEDITUNCHANGED);
    //   + ������� ��������� ������� Color,SelColor
    long  GetObjectColor() {return MAKELONG(Color,SelColor);}
    int   GetObjectColorUnChanged() {return ColorUnChanged;}
    /* SVS $*/
    /* $ 14.02.2001 IS
         ������� ������/������������ ������� ���������
    */
    void SetTabSize(int NewSize) { TabSize=NewSize; }
    int  GetTabSize(void) {return TabSize; }
    /* IS $ */
    /* $ 15.02.2001 IS
         ������� ������/������������ ������� �������� ��������������
    */
    void SetDelRemovesBlocks(int NewMode) { DelRemovesBlocks=NewMode; }
    int  GetDelRemovesBlocks(void) {return DelRemovesBlocks; }

    void SetPersistentBlocks(int NewMode) { PersistentBlocks=NewMode; }
    int  GetPersistentBlocks(void) {return PersistentBlocks; }
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
       ���������� ��������� WholeWords ��� ������ ����� ����
       � ������� Search.
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
       ��������� ������������� �������� ������ ��� ������������ Dialod API
    */
    int   GetMaxLength() {return MaxLength;};
    /* SVS $ */
    /* $ 12.08.2000 KM
       ������� ��������� � ��������� ����� �����
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
      ������� Xlat - ������������� �� �������� QWERTY <-> ������
    */
    /* $ 13.12.2000 SVS
       �������������� �������� � �������  Xlat()
    */
    void Xlat(BOOL All=FALSE);
    /* SVS $ */
    /* SVS $ */

    static void DisableEditOut(int Disable);
    static void DisableEncode(int Disable);

    void SetDialogParent(int Sets) {IsDialogParent=Sets;}
    int  GetDialogParent() {return IsDialogParent;}
};

#endif  // __EDIT_HPP__
