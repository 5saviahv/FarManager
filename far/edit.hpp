#ifndef __EDIT_HPP__
#define __EDIT_HPP__
/*
edit.hpp

���������� ��������� ������ ��������������

*/

/* Revision: 1.28 23.12.2004 $ */

/*
Modify:
  23.12.2004 WARP
    ! 3-� ����������� ExpandTab (������ ���������������� ������������ ����������� � USE_OLDEXPANDTABS)
  14.12.2003 SVS
    + SetWordDiv(), WordDiv - ��� ���������, �� ��������� - Opt.WordDiv
  08.09.2003 SVS
    + ProcessInsPlainText() - ������� plain-text`�
  17.09.2002 SKV
    + GetConvertTabs()
  04.09.2002 SVS
    ! ��������� EditList ������� �� editor.cpp � edit.hpp
    ! � ������� SetInputMask �������� ����� �������� const
  18.05.2002 SVS
    ! ����� - ������ � ���� ��������������� ����������
  28.01.2002 SVS
    + ProcessInsPath() - ������� ���� � ������ ������� �������
  21.01.2002 SVS
    + ����������� ������ � �������� � �������� (CursorVisible,CursorSize).
  12.01.2002 IS
    ! ��������� const
  14.12.2001 IS
    ! ��������� const
  28.11.2001 SVS
    + GetClearFlag() - �������� ��������� ����� ClearFlag
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
#include "bitflags.hpp"

#ifdef USE_OLDEXPANDTABS

// ������� ���� (����� 0xFF) ������� ������� ScreenObject!!!
enum FLAGS_CLASS_EDITLINE{
  FEDITLINE_MARKINGBLOCK         = 0x00000100,
  FEDITLINE_DROPDOWNBOX          = 0x00000200,
  FEDITLINE_CLEARFLAG            = 0x00000400,
  FEDITLINE_PASSWORDMODE         = 0x00000800,
  FEDITLINE_EDITBEYONDEND        = 0x00001000,
  FEDITLINE_EDITORMODE           = 0x00002000,
  FEDITLINE_OVERTYPE             = 0x00004000,
  FEDITLINE_DELREMOVESBLOCKS     = 0x00008000,  // Del ������� ����� (Opt.EditorDelRemovesBlocks)
  FEDITLINE_PERSISTENTBLOCKS     = 0x00010000,  // ���������� ����� (Opt.EditorPersistentBlocks)
  FEDITLINE_CONVERTTABS          = 0x00020000,
  FEDITLINE_READONLY             = 0x00040000,
  FEDITLINE_CURSORVISIBLE        = 0x00080000,
  // ���� �� ���� �� FEDITLINE_PARENT_ �� ������ (��� ������� ���), �� Edit
  // ���� �� � ������� �������.
  FEDITLINE_PARENT_SINGLELINE    = 0x00100000,  // ������� ������ ����� � �������
  FEDITLINE_PARENT_MULTILINE     = 0x00200000,  // ��� �������� Memo-Edit (DI_EDITOR ��� DIF_MULTILINE)
  FEDITLINE_PARENT_EDITOR        = 0x00400000,  // "������" ������� ��������
};

#else

//�������� ����� (��������� ����� FEDITLINE_CONVERTTABS)

// ������� ���� (����� 0xFF) ������� ������� ScreenObject!!!
enum FLAGS_CLASS_EDITLINE{
  FEDITLINE_MARKINGBLOCK         = 0x00000100,
  FEDITLINE_DROPDOWNBOX          = 0x00000200,
  FEDITLINE_CLEARFLAG            = 0x00000400,
  FEDITLINE_PASSWORDMODE         = 0x00000800,
  FEDITLINE_EDITBEYONDEND        = 0x00001000,
  FEDITLINE_EDITORMODE           = 0x00002000,
  FEDITLINE_OVERTYPE             = 0x00004000,
  FEDITLINE_DELREMOVESBLOCKS     = 0x00008000,  // Del ������� ����� (Opt.EditorDelRemovesBlocks)
  FEDITLINE_PERSISTENTBLOCKS     = 0x00010000,  // ���������� ����� (Opt.EditorPersistentBlocks)
//  FEDITLINE_CONVERTTABS          = 0x00020000,
  FEDITLINE_READONLY             = 0x00040000,
  FEDITLINE_CURSORVISIBLE        = 0x00080000,
  // ���� �� ���� �� FEDITLINE_PARENT_ �� ������ (��� ������� ���), �� Edit
  // ���� �� � ������� �������.
  FEDITLINE_PARENT_SINGLELINE    = 0x00100000,  // ������� ������ ����� � �������
  FEDITLINE_PARENT_MULTILINE     = 0x00200000,  // ��� �������� Memo-Edit (DI_EDITOR ��� DIF_MULTILINE)
  FEDITLINE_PARENT_EDITOR        = 0x00400000,  // "������" ������� ��������
};

#endif


class Dialog;
class Editor;

class Edit:public ScreenObject
{
  friend class Dialog;
  friend class Editor;

  private:
    char  *Str;
    int    StrSize;
    int    MaxLength;

    char  *Mask;             // 12.08.2000 KM - ���������� ��� �������� ����� �����

    struct CharTableSet *TableSet;
    struct ColorItem *ColorList;
    int    ColorCount;

    int    Color;
    int    SelColor;
    int    ColorUnChanged;   // 28.07.2000 SVS - ��� �������

    int    LeftPos;
    int    CurPos;
    int    PrevCurPos;       // 12.08.2000 KM - ���������� ��������� �������

    int    TabSize;          // 14.02.2001 IS - ������ ��������� - �� ��������� ����� Opt.TabSize;

#ifndef USE_OLDEXPANDTABS
    int    TabExpandMode;
#endif

    int    SelStart;
    int    SelEnd;

    int    EndType;

    int    CursorSize;
    int    CursorPos;
    const char *WordDiv;

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
    int ProcessInsPlainText(void);
    int CheckCharMask(char Chr);
    int ProcessInsPath(int Key,int PrevSelStart=-1,int PrevSelEnd=0);

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
    void SetDelRemovesBlocks(int Mode) {Flags.Change(FEDITLINE_DELREMOVESBLOCKS,Mode);}
    int  GetDelRemovesBlocks(void) {return Flags.Check(FEDITLINE_DELREMOVESBLOCKS); }

    void SetPersistentBlocks(int Mode) {Flags.Change(FEDITLINE_PERSISTENTBLOCKS,Mode);}
    int  GetPersistentBlocks(void) {return Flags.Check(FEDITLINE_PERSISTENTBLOCKS); }
    /* IS $ */
    void  GetString(char *Str,int MaxSize);
    const char* GetStringAddr();
    void  SetString(const char *Str);
    void  SetBinaryString(const char *Str,int Length);
    void  GetBinaryString(const char *&Str,const char **EOL,int &Length);
    void  SetEOL(const char *EOL);
    int   GetSelString(char *Str,int MaxSize);
    int   GetLength();
    void  InsertString(const char *Str);
    void  InsertBinaryString(const char *Str,int Length);
    /* $ 03.08.2000 KM
       ���������� ��������� WholeWords ��� ������ ����� ����
       � ������� Search.
    */
    int   Search(char *Str,int Position,int Case,int WholeWords,int Reverse);
    /* KM $ */
    void  SetClearFlag(int Flag) {Flags.Change(FEDITLINE_CLEARFLAG,Flag);}
    int   GetClearFlag(void) {return Flags.Check(FEDITLINE_CLEARFLAG);}
    void  SetCurPos(int NewPos) {CurPos=NewPos;PrevCurPos=NewPos;}
    int   GetCurPos() {return(CurPos);}
    int   GetTabCurPos();
    int   GetLeftPos() {return(LeftPos);}
    void  SetLeftPos(int NewPos) {LeftPos=NewPos;}
    void  SetTabCurPos(int NewPos);
    void  SetPasswordMode(int Mode) {Flags.Change(FEDITLINE_PASSWORDMODE,Mode);};
    void  SetMaxLength(int Length) {MaxLength=Length;};
    /* $ 28.07.2000 SVS
       ��������� ������������� �������� ������ ��� ������������ Dialod API
    */
    int   GetMaxLength() {return MaxLength;};
    /* SVS $ */
    /* $ 12.08.2000 KM
       ������� ��������� � ��������� ����� �����
    */
    void  SetInputMask(const char *InputMask);
    char* GetInputMask() {return Mask;}
    /* KM $ */
    void  SetOvertypeMode(int Mode) {Flags.Change(FEDITLINE_OVERTYPE,Mode);};
    int   GetOvertypeMode() {return Flags.Check(FEDITLINE_OVERTYPE);};

#ifdef USE_OLDEXPANDTABS
    void  SetConvertTabs(int Mode) {Flags.Change(FEDITLINE_CONVERTTABS,Mode);};
    int   GetConvertTabs() {return Flags.Check(FEDITLINE_CONVERTTABS);};
#else
    void  SetConvertTabs(int Mode) { TabExpandMode = Mode;};
    int   GetConvertTabs() {return TabExpandMode;};
#endif

    int   RealPosToTab(int Pos);
    int   TabPosToReal(int Pos);
    void  SetTables(struct CharTableSet *TableSet);
    void  Select(int Start,int End);
    void  AddSelect(int Start,int End);
    void  GetSelection(int &Start,int &End);
    void  GetRealSelection(int &Start,int &End);
    void  SetEditBeyondEnd(int Mode) {Flags.Change(FEDITLINE_EDITBEYONDEND,Mode);};
    void  SetEditorMode(int Mode) {Flags.Change(FEDITLINE_EDITORMODE,Mode);};
    void  ReplaceTabs();

#ifndef USE_OLDEXPANDTABS
    void  InsertTab ();
#endif

    void  AddColor(struct ColorItem *col);
    int   DeleteColor(int ColorPos);
    int   GetColor(struct ColorItem *col,int Item);

#ifdef SHITHAPPENS
    void ReplaceSpaces(int i);
#endif
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

    void SetDialogParent(DWORD Sets);
    void SetCursorType(int Visible,int Size);
    void GetCursorType(int &Visible,int &Size);
    int  GetReadOnly() {return Flags.Check(FEDITLINE_READONLY);}
    void SetReadOnly(int NewReadOnly) {Flags.Change(FEDITLINE_READONLY,NewReadOnly);}
    int  GetDropDownBox() {return Flags.Check(FEDITLINE_DROPDOWNBOX);}
    void SetDropDownBox(int NewDropDownBox) {Flags.Change(FEDITLINE_DROPDOWNBOX,NewDropDownBox);}
    void SetWordDiv(const char *WordDiv){Edit::WordDiv=WordDiv;}
};


struct EditList
{
  struct EditList *Prev;
  struct EditList *Next;
  Edit EditLine;
};

#endif  // __EDIT_HPP__
