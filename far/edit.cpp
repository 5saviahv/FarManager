/*
edit.cpp

���������� ��������� ������ ��������������

*/

/* Revision: 1.114 14.04.2004 $ */

/*
Modify:
  14.04.2004 SVS
    - BugZ#1053 - ���������� � $Text
  12.01.2004 SVS
    ! ������������� KEY_MACROSELWORD
  12.01.2004 IS
   - ���: ������� "������ �� ������", ����� ����������� ���� �������� �������
     � ������ ������ 128. �������: ��� ��������� � WordDiv ����������
     IsWordDiv, � �� strchr
  15.12.2003 SVS
    - BugZ#1002 - ESPT_SETWORDDIV �� ������ �� ������� �� ������
  11.11.2003 SKV
    - shift-end � edit-� � ������������ ������������
      ������� ��� ������������ �������������
  26.10.2003 KM
    ! ��������� ����������� ������������� ������ � ��������.
    ! ������������� ������� �� ��������� � ������ �����
  20.10.2003 SVS
    ! ��������������
        KEY_MACRO_EDITSELECTED -> KEY_MACRO_SELECTED
    + ��������� KEY_MACRO_EMPTY, KEY_MACRO_EOF � KEY_MACRO_BOF
  20.10.2003 SVS
    ! ��������� ������� ��� ������� ����
  08.10.2003 SVS
    ! ������ ����������� "��� ��������"
    ! � Edit::Select() ���� ��� ���������� == -1, ������ ���
      � ������������.
  26.09.2003 SVS
    ! ��������������
      GetMacroPlainText        -> GetPlainText
      GetMacroPlainTextSize    -> GetPlainTextSize
    ! ��������� � ��������� �����������
  21.09.2003 KM
    ! ��������� ������ � ������.
    + ���������� EDMASK_HEX ��� ����� ������ ����������������� ��������.
  17.09.2003 SVS
    - ����������� "��������" � ������� ��� ������� KEY_MACROPLAINTEXT � KEY_MACRODATE.
      �� ���� ��� ����, ��� $Date ����� ���� ��� ����������
  12.09.2003 SVS
    ! ����������� ������� ����� ��� ������� Plain-text �� �������.
      ��� ����� ������������� ����� ��������
      CtrlObject->Macro.GetMacroPlainTextSize(), ����� ��������
      ������ ������.
  08.09.2003 SVS
    + ��������� KEY_MACROPLAINTEXT
  03.09.2003 SVS
    ! ��� ������������� ����� ���� QUOTEDNAME_
  25.08.2003 SVS
    + Opt.QuotedName - ��������� ����� ������/����� � �������
  08.05.2003 SVS
    - ��� ���������� UseNumpad KEY_SHIFTINS �� ��������� ������� KEY_SHIFTNUMPAD0
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  11.12.2002 SVS
    - � ProcessCtrlQ ������� ������� ������� ��� KEY_CONSOLE_BUFFER_RESIZE
  05.12.2002 SVS
    ! �������� ����� MkStrFTime(), �������� ������� ����
  12.11.2002 DJ
    ! ������� ������ ����������� � ������, ����� ������� ����������� �����
      �� ����������
  04.11.2002 SVS
    ! ��� ReturnAltValue �� ������ ������ ��� �������.
      ��� ��������� Alt-212 ������ ����� � ������ ���������
  16.10.2002 SKV
    - ������� � DeleteBlock
  14.10.2002 SKV
    ! ���������
  17.09.2002 SKV
    - ������ ���������
  04.09.2002 SVS
    ! � ������� SetInputMask �������� ����� �������� const +
      ��������� ����������� ����� ������� SetInputMask
  19.08.2002 SVS
    + EOL_TYPE_CHARS (� ������ IS)
  12.07.2002 SVS
    ! �� ���������� Unchanged-��������� � ������� ����� ��� Ctrl-Ins
  04.07.2002 SKV
    - Bugz#512 - ������� :)
  02.07.2002 SKV
    - Bugz#512 - Tab+PersistentBlocks+Selection
  25.06.2002 SVS
    ! ���������:  BitFlags::Skip -> BitFlags::Clear
  24.06.2002 SKV
    - ������ �������� �� ������ ������.
  06.06.2002 SVS
    ! ��������� ������������ ������� � Edit::SetBinaryString, ���� ��������� MaxLength
  24.05.2002 SVS
    + ������������ Numpad-������
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
    ! ����� - ������ � ���� ��������������� ����������
  29.04.2002 SVS
    - �� ���� ������� ���� BugZ#488 (������)
  29.04.2002 SVS
    - BugZ#488 - Shift=enter
  27.04.2002 SVS
    ! 8192 -> MAXSIZE_SHORTCUTDATA
  02.04.2002 SVS
    - BugZ#425 - DI_EDIT and DIF_SELECTONENTRY
  22.03.2002 DJ
    + ��������� KEY_MEDIT_ISSELECTED
  18.03.2002 SVS
    ! ���������, � ����� � ��������� Opt.Dialogs
  14.03.2002 SVS
    - Ctrl-V != Shift-Ins
    ! ������� ������������� optimize (����� �� ������ ;-))
  07.03.2002 SVS
    - BugZ#340 - edit association
      ��� ������� ������� ��� ����� �� ��������� ����� ������������
      ������� (���� MaxLength != -1)
  04.02.2002 SVS
    ! �������� ��������������� ������� IsShiftKey()
  28.01.2002 SVS
    ! ���... ��... �������� � ����������... ���� ��� ���� �����, �����������
      BugZ#271 - ������� ��������. �� � ���� ��������� ����������� ���������...
    + ProcessInsPath() - ������� ���� � ������ ������� �������
  28.01.2002 SVS
    - BugZ#271 - ��������: ��������� Shift-Enter
  21.01.2002 SVS
    - BugZ#258 - Alt-1...Alt-30
    + ����������� ������ � �������� � ��������.
  16.01.2002 SVS
    - BugZ#246 - RCtrl-Digit ��� ������������ ������.
  12.01.2002 IS
    ! ��������� const
  11.01.2002 SVS
    - �� ��������� ���� � ��������� ��� �������.
  26.12.2001 SVS
    + ���� ������ ��� Overtype ������
  21.12.2001 SVS
    + KEY_MACROSELWORD
  21.12.2001 SVS
    - BugZ#187 - Ctrl-X �� �������� � ��������
  14.12.2001 IS
    ! ��������� const
  07.12.2001 SVS
    - ������� ������� �������� ����� ��� �������� (�������) ������ ������.
      (� ������ � ���� BugZ#161)
  27.11.2001 DJ
    - ��������� memory allocation conflict ��� ColorList
  23.11.2001 SVS
    ! IsDialogParent ����� ����� 3 ��������: EDPARENT_*
    + ������ �������� "if(Mask[...)" ������� ������� CheckCharMask()
  15.11.2001 OT
    "����������" ����������� ����������� ����������� (������� 97)
  13.11.2001 SVS
    - Bug: Shift-F4 � ������ ���-���� ���� ������� ������ ������ ����� � �������.
  08.11.2001 SVS
    - BugZ#97: ������� � �������� �����������/�������������� ���
  28.10.2001 SVS
    ! �������� � ������ ����������� ������� �� ������� ����� (�� �� ��� �
      � �������)
  24.10.2001 SVS
    - �� ����� ��� �����? ���� � ��, �������� ������������� ������ (?) ��
      ������� ����� �����? ��� [Ctrl]ShiftEnter � Ctrl*Bracket ����� ������
      ��������� ������(!) ���� ��� �� ����������.
  07.10.2001 VVM
    + CTRL+SHIFT+ENTER ��������� ���� � ��������� ������.
  14.09.2001 SKV
    - ��� ctrl-q ���� �������� DeleteBlock, � �� RecurseProcessKey(KEY_DEL); :)
  05.09.2001 SVS
    ! ��� ������������ ������ ������� ���������
  31.07.2001 KM
    ! ��� ���������� ������� ����������� ������
      � ������ �������.
  24.07.2001 IS
    ! ������ �������� �� ' ' � '\t' �� ����� isspace
  19.07.2001 KM
    - ��� NT ������ �����.
  16.07.2001 KM
    - ������ ����� �*�� � ������ ������� w9x ��� ��� �������
      ��������� ������� ���� ������ ���������� "��������"
      ������� ���.
  10.07.2001 IS
    - ���: � ��������� �� ctrl-], ctrl-[, ctrl-shift-]  � ctrl-shift-[
      ������ ����������� ������ � ��������� OEM, ���� ���� ������� ���������
      ���� != OEM
  22.06.2001 SVS
    ! ��������� �������� � Ctrl-Q - ������ ����� ���� (���
      ��������������� ������) ��������� ������ � ������ ������� �������
    ! ������� ����� ���� �� KEY_MACRO*
    + ��������� ��������� KEY_MACRODATE
    + ProcessInsDate()
  08.06.2001 SVS
    - ���� ��� ��������� ������ � ������������� ClearFlag - �����, ����,
      �� ���� ��������� ����� �������� �� ��������, � ��� �� ����
      "Text(&ShowStr[LeftPos]);" � ���� � ��� :-(
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  13.04.2001 OT
    - ���������� �������� (�� �� �����) � ��������� ����� � ���������
  13.04.2001 SVS
    ! ��������� Ctrl-Q �������� � ��������� ������� ProcessCtrlQ(), �.�.
      ������������ � editor.cpp
  21.02.2001 IS
    ! Opt.TabSize -> Opt.EdOpt.TabSize
      Opt.EditorPersistentBlocks -> Opt.EdOpt.PersistentBlocks
      Opt.EditorDelRemovesBlocks -> Opt.EdOpt.DelRemovesBlocks
  15.02.2001 IS
    ! Opt.EditorDelRemovesBlocks -> DelRemovesBlocks
      Opt.EditorPersistentBlocks -> PersistentBlocks
  14.02.2001 IS
    + �� ������ ��������� �������� TabSize, � ������� ����������� ������
      "Opt.TabSize" �� "TabSize"
  13.02.2001 VVM
    + ��������� SHIFT+SPACE
  23.01.2001 SVS
    + �� ������ ������ �������� Str � ����������� ��� ������������ ������
    ! � ���� ���� �� ������������� ������ :-((
  04.01.2001 SVS
    ! ��������� ���������� � �������������� � "����� ������" - ���������� 2 :-)
      ����, ������, ������� - � �������� � ��� ������� ���� �� �������� :-(
  26.12.2000 SVS
    ! KEY_DTDAY   -> KEY_MACRODAY
      KEY_DTMONTH -> KEY_MACROMONTH
      KEY_DTYEAR  -> KEY_MACROYEAR
   21.12.2000 SVS
    + ��������� �������������:
      KEY_DTDAY - ������� ���� ������ - "$Day"
      KEY_DTMONTH - ������� ����� - "$Month"
      KEY_DTYEAR - ������� ��� - "$Year"
   15.12.2000 OT
    - ����������� ���� � (KEY_TAB && OverType)
   10.12.2000 tran & OT
    ! KEY_SHIFTBS ������� �� KEY_CTRLSHIFTBS (tran)
    - ����������� ���� � KEY_SHIFTTAB (OT)
   13.12.2000 SVS
    + �������������� �������� � �������  Xlat()
   10.12.2000 IS
    ! ������������ ��� Xlat ������ �� �����, �� ������� ����� ������, ��� ��
      �����, ��� ��������� ����� ������� ������� �� 1 ������
    - ������ �� �������� ����������� ��-�� ����, ��� ���� SelStart==SelEnd
   25.11.2000 IS
    + ���� ��� ���������, �� ���������� ������� �����. ����� ������������ ��
      ������ ����������� ������ ������������.
   15.11.2000 KM 1.23 (����� ������)
    + ������� KeyMatchedMask ��������� ������������� �������
      ���� �������� �� ��������� �������� �����.
    ! ������ Str �� ����� ���� ������� ����� � ��� Str ����� ��������
      MaxLength, ������ ����� �����.
    - ��������� ��� �� �������� ������ �������� � ������ � ������.
    ! ������ ���������� ����� �������� � �������� ������.
    + ��������� CtrlDel � ������� � ������.
   20.10.2000 SVS
    ! ��������� ��� �����.
   16.10.2000 tran
    + ���� ����� MaxLength, �� ��������� ������ �� ������ ���
      ��������
   24.09.2000 SVS $
    + ������� Xlat - ������������� �� �������� QWERTY <-> ������
   18.09.2000 SVS
    - ��� Mask - ������ ��������������� ������� :-)
   15.09.2000 SVS
    - Bug - �������� ������� ������ -> Shift-Del ������� ��� ������
           ��� ������ ���� ������ ��� UnChanged ���������
   11.09.2000 SVS 1.19
    ! ���� Opt.DlgEULBsClear = 1, �� BS � �������� ��� UnChanged ������
      ������� ����� ������ �����, ��� � Del
   08.09.2000 SVS 1.18
    ��� UnChanget ������:
    ! Shift-Del - ��������� ������ � ����� � ������� ��������������
    ! KEY_BS ��� �� ��� � KEY_DEL ������� ��� ������ (����� �� � �������
      �������������� � ��������, � ��������� ��������� KEY_BS ��� � ������ -
      �������� �������, ��� �������� �����).
   07.09.2000 KM
    - ����������� ����� ��� ������ �� ����� ������.
   24.08.2000 SVS
    ! � DropDowList`� ��������� �� ������ ��������� - �� ��� ������� �����
      ���� ���� ������ ������
   23.08.2000 SVS
    - ����������� ���������� ���� ��� :-(((
   13.08.2000 KM
    + ������� GetNextCursorPos - ��������� ��������� ������� � ������
      � ������ Mask.
    + ������� RefreshStrByMask - ��������� ���������� ������ �� ��������� �����
      �����.
    ! ����� ���������, ���������� ����������� �������� � ������
      ����� �� �������� �����, ����������� � ���������� Mask.
   15.08.2000 SVS
    + � DropDowList`� ��������� �� ������ ��������� - �� ��� ������� �����
   03.08.2000 KM
    ! � ������� Search �������� ������� �������� int WholeWords.
    ! ������ � ���� ������� ����������� ����������� ������ ����� ����.
   03.08.2000 SVS
    ! WordDiv -> Opt.WordDiv
   28.07.2000 SVS
    ! � ������� ApplyColor() SelColor ����� ���� � �������� ������.
    + ���������� ������ ColorUnChanged (��� �������)
    ! SetObjectColor ����� �������������� �������� ��� ��������� ColorUnChanged
  26.08.2000 tran
    + DropDownBox �����
  26.07.2000 SVS
    - Bugs #??
      � ������� ����� ��� ���������� ����� �������� BS � ������
      ���������� �������� ����� (��� � ���������) ��������:
       - ������ ����� �������� ������
       - ��������� ����� �����
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  07.07.2000 SVS
    + �������������� ���� WordDiv ��������� ������ � global.cpp �
      ������� �� ������� (����� ��� ��������������)
  04.07.2000 IG
    - � ������� ������������ ��������� �� ShiftIns (bug8)
  03.07.2000 tran
    + ��������� SHIFT_BS - �������� �� ������ ������
    - Bug #10 ( ^[,^], ShiftEnter �� ������� ��������� �����)
    + ReadOnly ����
    + Ctrl-L ����������� ReadOnly ����
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "edit.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "plugin.hpp"
#include "keys.hpp"
#include "editor.hpp"
#include "ctrlobj.hpp"
#include "filepanels.hpp"
#include "filelist.hpp"
#include "panel.hpp"
#include "scrbuf.hpp"

static int EditOutDisabled=0;
static int EditEncodeDisabled=0;
static int Recurse=0;

enum {EOL_NONE,EOL_CR,EOL_LF,EOL_CRLF};
static const char *EOL_TYPE_CHARS[]={"","\r","\n","\r\n"};

// �������������� �����
#define EDMASK_ANY   'X' // ��������� ������� � ������ ����� ����� ������;
#define EDMASK_DSS   '#' // ��������� ������� � ������ ����� �����, ������ � ���� ������;
#define EDMASK_DIGIT '9' // ��������� ������� � ������ ����� ������ �����;
#define EDMASK_ALPHA 'A' // ��������� ������� � ������ ����� ������ �����.
#define EDMASK_HEX   'H' // ��������� ������� � ������ ����� ����������������� �������.

Edit::Edit()
{
  /* $ 13.07.2000 SVS
     ���, ��� ������ - ��� ����������� ���������� ������ �������� �����
     realloc, � �����:
        Str=new char[1];
     ����� malloc :-)
  */
  Str=(char*) xf_malloc(1);
  /* SVS $ */
  StrSize=0;
  WordDiv=Opt.WordDiv;
  *Str=0;
  /* $ 12.08.2000 KM
     ��������� ����� ����� � ���������� ��������� �������
  */
  Mask=NULL;
  PrevCurPos=0;
  /* KM $ */
  CurPos=0;
  CursorPos=0;
  CursorSize=-1;
  TableSet=NULL;
  LeftPos=0;
  MaxLength=-1;
  SelStart=-1;
  SelEnd=0;
  Flags.Set(FEDITLINE_EDITBEYONDEND);
  Color=F_LIGHTGRAY|B_BLACK;
  SelColor=F_WHITE|B_BLACK;
  /* $ 28.07.2000 SVS
     �������������� ���������� ��� ��������� � ��������
  */
  ColorUnChanged=COL_DIALOGEDITUNCHANGED;
  /* SVS $*/
  EndType=EOL_NONE;
  ColorList=NULL;
  ColorCount=0;
  /* $ 21.02.2001 IS
       ������ ��������� �� ��������� ����� Opt.EdOpt.TabSize;
  */
  TabSize=Opt.EdOpt.TabSize;
  /* IS $ */
  /* $ 21.02.2001 IS
       ������������� ���������� ���������� �� ���������
  */
  Flags.Change(FEDITLINE_DELREMOVESBLOCKS,Opt.EdOpt.DelRemovesBlocks);
  Flags.Change(FEDITLINE_PERSISTENTBLOCKS,Opt.EdOpt.PersistentBlocks);
  /* IS $ */
}


Edit::~Edit()
{
  if (ColorList)
    xf_free (ColorList);
  if (Mask)
    xf_free(Mask);
  if(Str)
    xf_free(Str);
}


void Edit::DisplayObject()
{
  /* $ 26.07.2000 tran
    + dropdown style */
  if (Flags.Check(FEDITLINE_DROPDOWNBOX))
  {
    Flags.Clear(FEDITLINE_CLEARFLAG);  // ��� ����-���� ��� �� ����� �������� unchanged text
    SelStart=0;
    SelEnd=StrSize; // � ����� ������� ��� ��� �������� -
                    //    ���� �� ���������� �� ������� Edit
  }
  /* tran 26.07.2000 $ */

  /* $ 12.08.2000 KM
     ���������� ������ ��������� ������� � ������ � ������ Mask.
  */
  int Value=(PrevCurPos>CurPos)?-1:1;
  CurPos=GetNextCursorPos(CurPos,Value);
  /* KM $ */
  FastShow();

  if (EditOutDisabled)
    return;
  /* $ 19.07.2001 KM
     - ��� NT ������ �����.
  */
  /* $ 16.07.2001 KM
     - ������ ����� �*�� � ������ ������� w9x ��� ��� �������
       ��������� ������� ���� ������ ���������� "��������"
       ������� ���.
  */
  if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
  {
    ::SetCursorType(TRUE,99);
    ::SetCursorType(TRUE,CursorSize);
  }
  /* KM $ */
  /* KM $ */

  /* $ 26.07.2000 tran
     ��� DropDownBox ������ ���������
     �� ���� ���� - ���������� �� �� ����� ������� ����� */
  if (Flags.Check(FEDITLINE_DROPDOWNBOX))
    ::SetCursorType(0,10);
  else
  {
    if (Flags.Check(FEDITLINE_OVERTYPE))
    {
      int NewCursorSize=IsWindowed()?
       (Opt.CursorSize[2]?Opt.CursorSize[2]:99):
       (Opt.CursorSize[3]?Opt.CursorSize[3]:99);
      ::SetCursorType(1,CursorSize==-1?NewCursorSize:CursorSize);
    }
    else
      ::SetCursorType(1,CursorSize);
  }
  MoveCursor(X1+CursorPos-LeftPos,Y1);
}


void Edit::SetCursorType(int Visible,int Size)
{
  Flags.Change(FEDITLINE_CURSORVISIBLE,Visible);
  CursorSize=Size;
  ::SetCursorType(Visible,Size);
}

void Edit::GetCursorType(int &Visible,int &Size)
{
  Visible=Flags.Check(FEDITLINE_CURSORVISIBLE);
  Size=CursorSize;
}

/* $ 12.08.2000 KM
   ���������� ������ ��������� ������� � ������ � ������ Mask.
*/
int Edit::GetNextCursorPos(int Position,int Where)
{
  int Result=Position;

  if (Mask && *Mask && (Where==-1 || Where==1))
  {
    int i;
    int PosChanged=FALSE;
    int MaskLen=strlen(Mask);
    for (i=Position;i<MaskLen && i>=0;i+=Where)
    {
      if (CheckCharMask(Mask[i]))
      {
        Result=i;
        PosChanged=TRUE;
        break;
      }
    }
    if (!PosChanged)
    {
      for (i=Position;i>=0;i--)
      {
        if (CheckCharMask(Mask[i]))
        {
          Result=i;
          PosChanged=TRUE;
          break;
        }
      }
    }
    if (!PosChanged)
    {
      for (i=Position;i<MaskLen;i++)
      {
        if (CheckCharMask(Mask[i]))
        {
          Result=i;
          break;
        }
      }
    }
  }
  return Result;
}
/* KM $ */

void Edit::FastShow()
{
  int EditLength;
  if (!Flags.Check(FEDITLINE_EDITBEYONDEND) && CurPos>StrSize && StrSize>=0)
    CurPos=StrSize;
  EditLength=ObjWidth;
  if (MaxLength!=-1)
  {
    if (StrSize>MaxLength)
    {
      Str[MaxLength]=0;
      StrSize=MaxLength;
    }
    if (CurPos>MaxLength-1)
      CurPos=MaxLength>0 ? (MaxLength-1):0;
  }
  int TabCurPos=GetTabCurPos();
  /* $ 31.07.2001 KM
    ! ��� ���������� ������� ����������� ������
      � ������ �������.
  */
  if (!Flags.Check(FEDITLINE_DROPDOWNBOX))
  {
    if (TabCurPos-LeftPos>EditLength-1)
      LeftPos=TabCurPos-EditLength+1;
    if (TabCurPos<LeftPos)
      LeftPos=TabCurPos;
  }
  /* KM $ */
  GotoXY(X1,Y1);
  int TabSelStart=(SelStart==-1) ? -1:RealPosToTab(SelStart);
  int TabSelEnd=(SelEnd<0) ? -1:RealPosToTab(SelEnd);

  /* $ 17.08.2000 KM
     ���� ���� �����, ������� ���������� ������, �� ����
     ��� "����������" ������� � �����, �� ���������� ����������
     ������ ��������� �������������� � Str
  */
  if (Mask && *Mask)
    RefreshStrByMask();
  /* KM $ */
#ifdef SHITHAPPENS
  ReplaceSpaces(0);
#endif
  if (!Flags.Check(FEDITLINE_CONVERTTABS) && memchr(Str,'\t',StrSize)!=NULL)
  {
    char *SaveStr;
    /* $ 04.07.2002 SKV
      ��������� ���� ����� ��������� ...
    */
    int SaveSelStart=SelStart;
    int SaveSelEnd=SelEnd;
    /* SKV $ */
    int SaveStrSize=StrSize,SaveCurPos=CurPos;
    if ((SaveStr=new char[StrSize+1])==NULL)
      return;
    memcpy(SaveStr,Str,StrSize);
    ReplaceTabs();
    CursorPos=CurPos;
    if (CurPos-LeftPos>EditLength-1)
      LeftPos=CurPos-EditLength+1;
    if (!EditOutDisabled)
      ShowString(Str,TabSelStart,TabSelEnd);
    memcpy(Str,SaveStr,SaveStrSize);
    Str[SaveStrSize]=0;
    /* $ 13.07.2000 SVS
       ��� �� �������� ����� new[]...
    */
    delete[] SaveStr;
    /* SVS $*/
    StrSize=SaveStrSize;
    CurPos=SaveCurPos;
    /* $ 04.07.2002 SKV
      ... � ������������.
    */
    SelStart=SaveSelStart;
    SelEnd=SaveSelEnd;
    /* SKV $ */
    Str=(char *)xf_realloc(Str,StrSize+1);
  }
  else
  {
    if (!EditOutDisabled)
      ShowString(Str,TabSelStart,TabSelEnd);
    CursorPos=CurPos;
  }
  /* $ 26.07.2000 tran
     ��� ����-���� ����� ��� �� ����� */
  if ( !Flags.Check(FEDITLINE_DROPDOWNBOX) )
      ApplyColor();
  /* tran 26.07.2000 $ */

#ifdef SHITHAPPENS
  ReplaceSpaces(1);
#endif
}


void Edit::ShowString(char *ShowStr,int TabSelStart,int TabSelEnd)
{
  int EditLength=ObjWidth;
  if (Flags.Check(FEDITLINE_PASSWORDMODE))
  {
    char *PswStr=new char[StrSize+1];
    if (PswStr==NULL)
      return;
    memset(PswStr,'*',StrSize);
    PswStr[StrSize]=0;
    Flags.Clear(FEDITLINE_PASSWORDMODE);
    ShowString(PswStr,TabSelStart,TabSelEnd);
    Flags.Set(FEDITLINE_PASSWORDMODE);
    /* $ 13.07.2000 SVS
       ��� �� �������� ����� new[]...
    */
    delete[] PswStr;
    /* SVS $*/
    return;
  }
  char *SaveStr=NULL;
  if (TableSet)
  {
    SaveStr=new char[StrSize+1];
    if (SaveStr==NULL)
      return;
    memcpy(SaveStr,Str,StrSize);
    DecodeString(ShowStr,(unsigned char*)TableSet->DecodeTable,StrSize);
  }
  if (memchr(Str,0,StrSize)!=0)
  {
    if (SaveStr==NULL)
    {
      SaveStr=new char[StrSize+1];
      if (SaveStr==NULL)
        return;
      memcpy(SaveStr,Str,StrSize);
    }
    for (int I=0;I<StrSize;I++)
      if (Str[I]==0)
        Str[I]=' ';
  }

  SetColor(Color);

  if (TabSelStart==-1)
  {
    if (Flags.Check(FEDITLINE_CLEARFLAG) && LeftPos<StrSize)
    {
      SetColor(ColorUnChanged);
      /* $ 21.09.2003 KM
         ��������� ������ � ������
      */
      int Len,Size;
      if (Mask && *Mask)
      {
        char *ShortStr=new char[StrSize+1];
        if (ShortStr==NULL)
          return;
        strncpy(ShortStr,ShowStr,StrSize);
        Len=strlen(RemoveTrailingSpaces(ShortStr));
        delete[] ShortStr;
        Size=Len;
      }
      else
      {
        Len=strlen(&ShowStr[LeftPos]);
        Size=StrSize;
      }
      if(Len > EditLength)
        Len=EditLength;
      mprintf("%-*.*s",Len,Len,&ShowStr[LeftPos]);
      SetColor(Color);

      int BlankLength=EditLength-(Size-LeftPos);
      /* KM $ */

      if (BlankLength > 0)
      {
        mprintf("%*s",BlankLength,"");
      }
    }
    else
      mprintf("%-*.*s",EditLength,EditLength,LeftPos>StrSize ? "":&ShowStr[LeftPos]);
  }
  else
  {
    char *OutStr=new char[EditLength+1];
    if (OutStr==NULL)
      return;
    if ((TabSelStart-=LeftPos)<0)
      TabSelStart=0;
    int AllString=(TabSelEnd==-1);
    if (AllString)
      TabSelEnd=EditLength;
    else
      if ((TabSelEnd-=LeftPos)<0)
        TabSelEnd=0;
    sprintf(OutStr,"%-*.*s",EditLength,EditLength,LeftPos>StrSize ? "":&ShowStr[LeftPos]);
    /* $ 24.08.2000 SVS
       ! � DropDowList`� ��������� �� ������ ��������� - �� ��� ������� �����
         ���� ���� ������ ������
    */
    if (TabSelStart>=EditLength /*|| !AllString && TabSelStart>=StrSize*/ ||
        TabSelEnd<TabSelStart)
    {
      if(Flags.Check(FEDITLINE_DROPDOWNBOX))
      {
        SetColor(SelColor);
        mprintf("%*s",X2-X1+1,OutStr);
      }
      else
        Text(OutStr);
    }
    /* SVS $ */
    else
    {
      mprintf("%.*s",TabSelStart,OutStr);
      SetColor(SelColor);
      /* $ 15.08.2000 SVS
         + � DropDowList`� ��������� �� ������ ��������� - �� ��� ������� �����
      */
      if(!Flags.Check(FEDITLINE_DROPDOWNBOX))
      {
        mprintf("%.*s",TabSelEnd-TabSelStart,OutStr+TabSelStart);
        if (TabSelEnd<EditLength)
        {
          //SetColor(Flags.Check(FEDITLINE_CLEARFLAG) ? SelColor:Color);
          SetColor(Color);
          Text(OutStr+TabSelEnd);
        }
      }
      else
      {
        mprintf("%*s",X2-X1+1,OutStr);
      }
      /* SVS $*/
    }
    /* $ 13.07.2000 SVS
       ��� �� �������� ����� new[]...
    */
    delete[] OutStr;
    /* SVS $*/
  }
  if (SaveStr)
  {
    memcpy(Str,SaveStr,StrSize);
    /* $ 13.07.2000 SVS
       ��� �� �������� ����� new[]...
    */
    delete[] SaveStr;
    /* SVS $*/
  }
}


int Edit::RecurseProcessKey(int Key)
{
  Recurse++;
  int RetCode=ProcessKey(Key);
  Recurse--;
  return(RetCode);
}


// ������� ������� ������ ��������� - �� ��������� �� ���� ������
int Edit::ProcessInsPath(int Key,int PrevSelStart,int PrevSelEnd)
{
  int RetCode=FALSE;
  char PathName[4096];

  if (Key>=KEY_RCTRL0 && Key<=KEY_RCTRL9) // ��������?
  {
    char PluginModule[NM],PluginFile[NM],PluginData[MAXSIZE_SHORTCUTDATA];
    if (GetShortcutFolder(Key,PathName,PluginModule,PluginFile,PluginData))
    {
      RetCode=TRUE;
    }
  }
  else // ����/�����?
  {
    int NeedRealName=FALSE;
    switch(Key)
    {
      case KEY_CTRLALTBRACKET:       // �������� ������� (UNC) ���� �� ����� ������
      case KEY_CTRLALTBACKBRACKET:   // �������� ������� (UNC) ���� �� ������ ������
      case KEY_ALTSHIFTBRACKET:      // �������� ������� (UNC) ���� �� �������� ������
      case KEY_ALTSHIFTBACKBRACKET:  // �������� ������� (UNC) ���� �� ��������� ������
        NeedRealName=TRUE;
      case KEY_CTRLBRACKET:          // �������� ���� �� ����� ������
      case KEY_CTRLBACKBRACKET:      // �������� ���� �� ������ ������
      case KEY_CTRLSHIFTBRACKET:     // �������� ���� �� �������� ������
      case KEY_CTRLSHIFTBACKBRACKET: // �������� ���� �� ��������� ������

      case KEY_CTRLSHIFTENTER:       // ������� ���� � ����.������
      case KEY_SHIFTENTER:           // ������� ���� � �����.������
      {
        Panel *SrcPanel;
        switch(Key)
        {
          case KEY_CTRLALTBRACKET:
          case KEY_CTRLBRACKET:
            SrcPanel=CtrlObject->Cp()->LeftPanel;
            break;
          case KEY_CTRLALTBACKBRACKET:
          case KEY_CTRLBACKBRACKET:
            SrcPanel=CtrlObject->Cp()->RightPanel;
            break;
          case KEY_SHIFTENTER:
          case KEY_ALTSHIFTBRACKET:
          case KEY_CTRLSHIFTBRACKET:
            SrcPanel=CtrlObject->Cp()->ActivePanel;
            break;
          case KEY_CTRLSHIFTENTER:
          case KEY_ALTSHIFTBACKBRACKET:
          case KEY_CTRLSHIFTBACKBRACKET:
            SrcPanel=CtrlObject->Cp()->GetAnotherPanel(CtrlObject->Cp()->ActivePanel);
            break;
        }

        if (SrcPanel!=NULL)
        {
          if(Key == KEY_SHIFTENTER || Key == KEY_CTRLSHIFTENTER)
          {
            char ShortFileName[NM];
            SrcPanel->GetCurName(PathName,ShortFileName);
            if(SrcPanel->GetShowShortNamesMode()) // ����� ���������� ���� :-)
              strcpy(PathName,ShortFileName);
          }
          else
          {
            SrcPanel->GetCurDir(PathName);
            if (SrcPanel->GetMode()!=PLUGIN_PANEL)
            {
              if(NeedRealName)
              {
                char uni[1024];
                DWORD uniSize = sizeof(uni);
                if (WNetGetUniversalName(PathName, UNIVERSAL_NAME_INFO_LEVEL,
                                             &uni, &uniSize) == NOERROR)
                {
                  UNIVERSAL_NAME_INFO *lpuni = (UNIVERSAL_NAME_INFO *)&uni;
                  strncpy(PathName, lpuni->lpUniversalName, sizeof(PathName)-1);
                }
                ConvertNameToReal(PathName,PathName, sizeof(PathName));
              }
              if (SrcPanel->GetShowShortNamesMode())
                ConvertNameToShort(PathName,PathName);
            }
            else
            {
              FileList *SrcFilePanel=(FileList *)SrcPanel;
              struct OpenPluginInfo Info;
              CtrlObject->Plugins.GetOpenPluginInfo(SrcFilePanel->GetPluginHandle(),&Info);
              strcat(FileList::AddPluginPrefix(SrcFilePanel,PathName),NullToEmpty(Info.CurDir));
            }
            AddEndSlash(PathName);
            if(Opt.QuotedName&QUOTEDNAME_INSERT)
              QuoteSpace(PathName);
          }

          RetCode=TRUE;
        }
      }
      break;
    }
  }

  // ���� ���-���� ����������, ������ ��� � ������� (PathName)
  if(RetCode)
  {
    if (Flags.Check(FEDITLINE_CLEARFLAG))
    {
      LeftPos=0;
      SetString("");
    }

    if (PrevSelStart!=-1)
    {
      SelStart=PrevSelStart;
      SelEnd=PrevSelEnd;
    }

    if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS))
      DeleteBlock();

    if(TableSet)
       EncodeString(PathName,(unsigned char*)TableSet->EncodeTable,
                    strlen(PathName));
    char *Ptr=PathName;
    for (;*Ptr;Ptr++)
      InsertKey(*Ptr);
    Flags.Clear(FEDITLINE_CLEARFLAG);
  }

  return RetCode;
}


int Edit::ProcessKey(int Key)
{
  switch(Key)
  {
    case KEY_MACRO_EMPTY:
      return GetLength()==0;
    case KEY_MACRO_SELECTED:
      return SelStart != -1 && SelStart < SelEnd;
    case KEY_MACRO_EOF:
      return CurPos >= StrSize;
    case KEY_MACRO_BOF:
      return CurPos==0;
  }

  int I;
  switch(Key)
  {
    case KEY_ADD:
      Key='+';
      break;
    case KEY_SUBTRACT:
      Key='-';
      break;
    case KEY_MULTIPLY:
      Key='*';
      break;
    case KEY_DIVIDE:
      Key='/';
      break;
    case KEY_CTRLC:
      Key=KEY_CTRLINS;
      break;
    case KEY_CTRLV:
      Key=KEY_SHIFTINS;
      break;
    case KEY_CTRLX:
      Key=KEY_SHIFTDEL;
      break;
  }

  int PrevSelStart=-1,PrevSelEnd=0;

  /* $ 25.07.2000 tran
     ��� ����-����, ��� Ctrl-l �� ����� */
  /* $ 03.07.2000 tran
     + ��������� Ctrl-L ��� ������������� ��������� ReadOnly  */
  if ( !Flags.Check(FEDITLINE_DROPDOWNBOX) && Key==KEY_CTRLL )
  {
    Flags.Swap(FEDITLINE_READONLY);
  }
  /* tran 03.07.2000 $ */
  /* tran 25.07.2000 $ */


  /* $ 26.07.2000 SVS
     Bugs #??
       � ������� ����� ��� ���������� ����� �������� BS � ������
       ���������� �������� ����� (��� � ���������) ��������:
         - ������ ����� �������� ������
         - ��������� ����� �����
  */
  if (((Key==KEY_BS || Key==KEY_DEL) && Flags.Check(FEDITLINE_DELREMOVESBLOCKS) || Key==KEY_CTRLD) &&
      !Flags.Check(FEDITLINE_EDITORMODE) && SelStart!=-1 && SelStart<SelEnd)
  {
    DeleteBlock();
    Show();
    return(TRUE);
  }
  /* SVS $ */

  /* $ 04.07.2000 IG
     ��������� ��������� �� ������ ������� (bug8) */
  if (!ShiftPressed && !CtrlObject->Macro.IsExecuting() &&
  /* IG $ */
      !IsShiftKey(Key) && !Recurse &&
      Key!=KEY_SHIFT && Key!=KEY_CTRL && Key!=KEY_ALT && Key!=KEY_RCTRL && Key!=KEY_RALT && Key!=KEY_NONE)
  {
    Flags.Clear(FEDITLINE_MARKINGBLOCK);
    if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS) && !(Key==KEY_CTRLINS || Key==KEY_CTRLNUMPAD0) &&
        Key!=KEY_SHIFTDEL && !Flags.Check(FEDITLINE_EDITORMODE) && Key != KEY_CTRLQ &&
        !(Key == KEY_SHIFTINS || Key == KEY_SHIFTNUMPAD0)) //Key != KEY_SHIFTINS) //??
    {
      /* $ 12.11.2002 DJ
         ����� ����������, ���� ������ �� ����������?
      */
      if (SelStart != -1 || SelEnd != 0)
      {
        PrevSelStart=SelStart;
        PrevSelEnd=SelEnd;
        Select(-1,0);
        Show();
      }
      /* DJ $ */
    }

  }
  if (!EditEncodeDisabled && Key<256 && TableSet && !ReturnAltValue)
    Key=TableSet->EncodeTable[Key];

  /* $ 11.09.2000 SVS
     ���� Opt.DlgEULBsClear = 1, �� BS � �������� ��� UnChanged ������
     ������� ����� ������ �����, ��� � Del
  */
  if (((Opt.Dialogs.EULBsClear && Key==KEY_BS) || Key==KEY_DEL) &&
     Flags.Check(FEDITLINE_CLEARFLAG) && CurPos>=StrSize)
    Key=KEY_CTRLY;
  /* SVS $ */
  /* $ 15.09.2000 SVS
     Bug - �������� ������� ������ -> Shift-Del ������� ��� ������
           ��� ������ ���� ������ ��� UnChanged ���������
  */
  if(Key == KEY_SHIFTDEL && Flags.Check(FEDITLINE_CLEARFLAG) && CurPos>=StrSize && SelStart==-1)
  {
    SelStart=0;
    SelEnd=StrSize;
  }
  /* SVS $ */

  if (Flags.Check(FEDITLINE_CLEARFLAG) && (Key<256 && Key!=KEY_BS || Key==KEY_CTRLBRACKET ||
      Key==KEY_CTRLBACKBRACKET || Key==KEY_CTRLSHIFTBRACKET ||
      Key==KEY_CTRLSHIFTBACKBRACKET || Key==KEY_SHIFTENTER))
  {
    LeftPos=0;
    SetString("");
    Show();
  }

  // ����� - ����� ������� ������� �����/������
  if(ProcessInsPath(Key,PrevSelStart,PrevSelEnd))
  {
    Show();
    return TRUE;
  }

  if (Key!=KEY_NONE && Key!=KEY_IDLE && Key!=KEY_SHIFTINS && Key!=KEY_SHIFTNUMPAD0 && Key!=KEY_CTRLINS &&
      (Key<KEY_F1 || Key>KEY_F12) && Key!=KEY_ALT && Key!=KEY_SHIFT &&
      Key!=KEY_CTRL && Key!=KEY_RALT && Key!=KEY_RCTRL &&
      (Key<KEY_ALT_BASE || Key>=KEY_ALT_BASE+256) &&
      (Key<KEY_MACRO_BASE || (Key>KEY_MACRO_ENDBASE && !(Key&MCODE_OP_SENDKEY))) && Key!=KEY_CTRLQ)
  {
    Flags.Clear(FEDITLINE_CLEARFLAG);
    Show();
  }


  switch(Key)
  {
    case KEY_SHIFTLEFT: case KEY_SHIFTNUMPAD4:
    {
      if (CurPos>0)
      {
        RecurseProcessKey(KEY_LEFT);
        if (!Flags.Check(FEDITLINE_MARKINGBLOCK))
        {
          Select(-1,0);
          Flags.Set(FEDITLINE_MARKINGBLOCK);
        }
        if (SelStart!=-1 && SelStart<=CurPos)
          Select(SelStart,CurPos);
        else
        {
          int EndPos=CurPos+1;
          int NewStartPos=CurPos;
          if (EndPos>StrSize)
            EndPos=StrSize;
          if (NewStartPos>StrSize)
            NewStartPos=StrSize;
          AddSelect(NewStartPos,EndPos);
        }
        Show();
      }
      return(TRUE);
    }

    case KEY_SHIFTRIGHT: case KEY_SHIFTNUMPAD6:
    {
      if (!Flags.Check(FEDITLINE_MARKINGBLOCK))
      {
        Select(-1,0);
        Flags.Set(FEDITLINE_MARKINGBLOCK);
      }
      if (SelStart!=-1 && SelEnd==-1 || SelEnd>CurPos)
      {
        if (CurPos+1==SelEnd)
          Select(-1,0);
        else
          Select(CurPos+1,SelEnd);
      }
      else
        AddSelect(CurPos,CurPos+1);
      RecurseProcessKey(KEY_RIGHT);
      return(TRUE);
    }

    case KEY_CTRLSHIFTLEFT: case KEY_CTRLSHIFTNUMPAD4:
    {
      /* $ 15.08.2000 KM */
      if (CurPos>StrSize)
      {
        PrevCurPos=CurPos;
        CurPos=StrSize;
      }
      /* KM $ */
      if (CurPos>0)
        RecurseProcessKey(KEY_SHIFTLEFT);

      /* $ 12.01.2004 IS
         ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
         ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
      */
      while (CurPos>0 && !(!IsWordDiv(TableSet, WordDiv, Str[CurPos]) &&
             IsWordDiv(TableSet, WordDiv,Str[CurPos-1]) && !IsSpace(Str[CurPos])))
      {
        /* $ 18.08.2000 KM
           ������� ����� �� ����� �������� CurPos-1 �� �����������
           � WordDiv.
        */
//        if (!IsSpace(Str[CurPos]) && IsSpace(Str[CurPos-1]))
        if (!IsSpace(Str[CurPos]) && (IsSpace(Str[CurPos-1]) ||
             IsWordDiv(TableSet, WordDiv, Str[CurPos-1])))
          break;
        /* KM $ */
        RecurseProcessKey(KEY_SHIFTLEFT);
      }
      /* IS $ */
      Show();
      return(TRUE);
    }

    case KEY_CTRLSHIFTRIGHT: case KEY_CTRLSHIFTNUMPAD6:
    {
      if (CurPos>=StrSize)
        return(FALSE);
      RecurseProcessKey(KEY_SHIFTRIGHT);
      /* $ 12.01.2004 IS
         ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
         ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
      */
      while (CurPos<StrSize && !(IsWordDiv(TableSet, WordDiv, Str[CurPos]) &&
             !IsWordDiv(TableSet, WordDiv, Str[CurPos-1])))
      {
        /* $ 18.08.2000 KM
           ������� ����� �� ����� �������� CurPos-1 �� �����������
           � WordDiv.
        */
//        if (!IsSpace(Str[CurPos]) && IsSpace(Str[CurPos-1]))
        if (!IsSpace(Str[CurPos]) && (IsSpace(Str[CurPos-1]) || IsWordDiv(TableSet, WordDiv, Str[CurPos-1])))
          break;
        /* KM $ */
        RecurseProcessKey(KEY_SHIFTRIGHT);
        if (MaxLength!=-1 && CurPos==MaxLength-1)
          break;
      }
      /* IS $ */
      Show();
      return(TRUE);
    }

    case KEY_SHIFTHOME:  case KEY_SHIFTNUMPAD7:
    {
      DisableEditOut(TRUE);
      while (CurPos>0)
        RecurseProcessKey(KEY_SHIFTLEFT);
      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_SHIFTEND:  case KEY_SHIFTNUMPAD1:
    {
      DisableEditOut(TRUE);

      /* $ 21.09.2003 KM
         ��������� ������ � ������
      */
      int Len;

      if (Mask && *Mask)
      {
        char *ShortStr=new char[StrSize+1];
        if (ShortStr==NULL)
          return FALSE;
        strncpy(ShortStr,Str,StrSize);
        Len=strlen(RemoveTrailingSpaces(ShortStr));
        delete[] ShortStr;
      }
      else
        Len=StrSize;

      int LastCurPos=CurPos;
      while (CurPos<Len/*StrSize*/)
      {
        RecurseProcessKey(KEY_SHIFTRIGHT);
        if(LastCurPos==CurPos)break;
        LastCurPos=CurPos;
      }
      /* KM $ */

      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_BS:
    {
      if (CurPos<=0)
        return(FALSE);
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */
      CurPos--;
      if (CurPos<=LeftPos)
      {
        LeftPos-=15;
        if (LeftPos<0)
          LeftPos=0;
      }
      if (!RecurseProcessKey(KEY_DEL))
        Show();
      return(TRUE);
    }

    /* $ 10.12.2000 tran
       KEY_SHIFTBS ������� �� KEY_CTRLSHIFTBS*/
    /* $ 03.07.2000 tran
       + KEY_SHIFTBS - ������ �� ������� �� ������ ������ */
    case KEY_CTRLSHIFTBS:
    {
      /* tran $ */
      /* $ 19.08.2000 KM
         ������� ������ �������� �������� �� ������ ������.
         ������ �������� �������� � � ������ �����.
      */
      int i;
      for (i=CurPos;i>=0;i--)
        RecurseProcessKey(KEY_BS);
      Show();
      return(TRUE);
    }
      /* KM $ */
    /* tran 03.07.2000 $ */

    case KEY_CTRLBS:
    {
      /* $ 15.08.2000 KM */
      if (CurPos>StrSize)
      {
        PrevCurPos=CurPos;
        CurPos=StrSize;
      }
      /* KM $ */
      DisableEditOut(TRUE);
//      while (CurPos>0 && IsSpace(Str[CurPos-1]))
//        RecurseProcessKey(KEY_BS);
      while (1)
      {
        int StopDelete=FALSE;
        if (CurPos>1 && IsSpace(Str[CurPos-1])!=IsSpace(Str[CurPos-2]))
          StopDelete=TRUE;
        RecurseProcessKey(KEY_BS);
        if (CurPos==0 || StopDelete)
          break;
        /* $ 12.01.2004 IS
           ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
           ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
        */
        if (IsWordDiv(TableSet, WordDiv,Str[CurPos-1]))
        /* IS $ */
          break;
      }
      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_CTRLQ:
    {
      EditOutDisabled++;
      if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS) && (SelStart != -1 || Flags.Check(FEDITLINE_CLEARFLAG)))
        RecurseProcessKey(KEY_DEL);
      ProcessCtrlQ();
      EditOutDisabled--;
      Show();
      return(TRUE);
    }

#if defined(MOUSEKEY)
    case KEY_MACROSELWORD:
    {
      int OldCurPos=CurPos;
      int SStart, SEnd;
      PrevSelStart=SelStart;
      PrevSelEnd=SelEnd;

      if(CurPos >= SelStart && CurPos <= SelEnd)
      { // �������� ��� ������ ��� ��������� ������� �����
        Select(0,StrSize);
      }
      else
      {
        int SStart, SEnd;
        CalcWordFromString(Str,CurPos,&SStart,&SEnd,TableSet,WordDiv);
        Select(SStart,++SEnd);
      }
      CurPos=OldCurPos; // ���������� �������
      Show();
      return TRUE;
    }
#endif

    case KEY_MACRO_DATE:
    case KEY_MACRO_PLAINTEXT:
    {
      if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS))
      {
        if(SelStart != -1) // BugZ#1053 - ���������� � $Text
          RecurseProcessKey(KEY_DEL);
      }
      if(Key == KEY_MACRO_DATE)
        ProcessInsDate();
      else
        ProcessInsPlainText();
      Show();
      return TRUE;
    }

    case KEY_CTRLT:
    case KEY_CTRLDEL:
    {
      if (CurPos>=StrSize)
        return(FALSE);
      DisableEditOut(TRUE);
//      while (CurPos<StrSize && IsSpace(Str[CurPos]))
//        RecurseProcessKey(KEY_DEL);
      /* $ 19.08.2000 KM
         �������� ���� �������� �� ��������, ����
         ������������ ����� �����.
      */
      if (Mask && *Mask)
      {
        /* $ 12.11.2000 KM
           ������� ��� ��� �������� ����� ������
           � ������ �����.
        */
        int MaskLen=strlen(Mask);
        int ptr=CurPos;
        while(ptr<MaskLen)
        {
          ptr++;
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (!CheckCharMask(Mask[ptr]) ||
             (IsSpace(Str[ptr]) && !IsSpace(Str[ptr+1])) ||
             (IsWordDiv(TableSet, WordDiv, Str[ptr])))
          /* IS $ */
            break;
        }
        for (int i=0;i<ptr-CurPos;i++)
          RecurseProcessKey(KEY_DEL);
        /* KM $ */
      }
      else
      {
        while (1)
        {
          int StopDelete=FALSE;
          if (CurPos<StrSize-1 && IsSpace(Str[CurPos]) && !IsSpace(Str[CurPos+1]))
            StopDelete=TRUE;
          RecurseProcessKey(KEY_DEL);
          if (CurPos>=StrSize || StopDelete)
            break;
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsWordDiv(TableSet, WordDiv, Str[CurPos]))
          /* IS $ */
            break;
        }
      }
      DisableEditOut(FALSE);
      Show();
      return(TRUE);
    }

    case KEY_CTRLY:
    {
      /* $ 25.07.2000 tran
         + DropDown style */
      /* $ 03.07.2000 tran
         + ��������� ReadOnly */
      if (Flags.Check(FEDITLINE_READONLY|FEDITLINE_DROPDOWNBOX))
        return (TRUE);
      /* tran 03.07.2000 $ */
      /* tran 25.07.2000 $ */
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */
      LeftPos=CurPos=0;
      *Str=0;
      StrSize=0;
      Str=(char *)xf_realloc(Str,1);
      Select(-1,0);
      Show();
      return(TRUE);
    }

    case KEY_CTRLK:
    {
      /* $ 25.07.2000 tran
         + DropDown style */
      /* $ 03.07.2000 tran
         + ��������� ReadOnly */
      if (Flags.Check(FEDITLINE_READONLY|FEDITLINE_DROPDOWNBOX))
        return (TRUE);
      /* tran 03.07.2000 $ */
      /* tran 25.07.2000 $ */
      if (CurPos>=StrSize)
        return(FALSE);
      if (!Flags.Check(FEDITLINE_EDITBEYONDEND))
      {
        if (CurPos<SelEnd)
          SelEnd=CurPos;
        if (SelEnd<SelStart)
        {
          SelEnd=0;
          SelStart=-1;
        }
      }
      Str[CurPos]=0;
      StrSize=CurPos;
      Str=(char *)xf_realloc(Str,StrSize+1);
      Show();
      return(TRUE);
    }

    case KEY_HOME:        case KEY_NUMPAD7:
    case KEY_CTRLHOME:    case KEY_CTRLNUMPAD7:
    {
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */
      CurPos=0;
      Show();
      return(TRUE);
    }

    case KEY_END:         case KEY_NUMPAD1:
    case KEY_CTRLEND:     case KEY_CTRLNUMPAD1:
    {
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */

      /* $ 21.09.2003 KM
         ��������� ������ � ������
      */
      if (Mask && *Mask)
      {
        char *ShortStr=new char[StrSize+1];
        if (ShortStr==NULL)
          return FALSE;
        strncpy(ShortStr,Str,StrSize);
        CurPos=strlen(RemoveTrailingSpaces(ShortStr));
        delete[] ShortStr;
      }
      else
        CurPos=StrSize;
      /* KM $ */

      Show();
      return(TRUE);
    }

    case KEY_LEFT:        case KEY_NUMPAD4:
    case KEY_CTRLS:
    {
      if (CurPos>0)
      {
        /* $ 15.08.2000 KM */
        PrevCurPos=CurPos;
        /* KM $ */
        CurPos--;
        Show();
      }
      return(TRUE);
    }

    case KEY_RIGHT:       case KEY_NUMPAD6:
    case KEY_CTRLD:
    {
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */

      /* $ 21.09.2003 KM
         ��������� ������ � ������
      */
      if (Mask && *Mask)
      {
        char *ShortStr=new char[StrSize+1];
        if (ShortStr==NULL)
          return FALSE;
        strncpy(ShortStr,Str,StrSize);
        int Len=strlen(RemoveTrailingSpaces(ShortStr));
        delete[] ShortStr;
        if (Len>CurPos)
          CurPos++;
      }
      else
        CurPos++;
      /* KM $ */

      Show();
      return(TRUE);
    }

    case KEY_INS:         case KEY_NUMPAD0:
    {
      Flags.Swap(FEDITLINE_OVERTYPE);
      Show();
      return(TRUE);
    }

    case KEY_DEL:
    {
      /* $ 25.07.2000 tran
         + DropDown style */
      /* $ 03.07.2000 tran
         + ��������� ReadOnly */
      if (Flags.Check(FEDITLINE_READONLY|FEDITLINE_DROPDOWNBOX))
        return (TRUE);
      /* tran 03.07.2000 $ */
      /* tran 25.07.2000 $ */
      if (CurPos>=StrSize)
        return(FALSE);
      if (SelStart!=-1)
      {
        if (SelEnd!=-1 && CurPos<SelEnd)
          SelEnd--;
        if (CurPos<SelStart)
          SelStart--;
        if (SelEnd!=-1 && SelEnd<=SelStart)
        {
          SelStart=-1;
          SelEnd=0;
        }
      }
      /* $ 16.08.2000 KM
         ������ � ������.
      */
      if (Mask && *Mask)
      {
        int MaskLen=strlen(Mask);
        int i,j;
        for (i=CurPos,j=CurPos;i<MaskLen;i++)
        {
          if (CheckCharMask(Mask[i+1]))
          {
            while(!CheckCharMask(Mask[j]) && j<MaskLen)
              j++;
            Str[j]=Str[i+1];
            j++;
          }
        }
        Str[j]=' ';
      }
      else
      {
        memmove(Str+CurPos,Str+CurPos+1,StrSize-CurPos);
        StrSize--;
        Str=(char *)xf_realloc(Str,StrSize+1);
      }
      /* KM $ */
      Show();
      return(TRUE);
    }

    case KEY_CTRLLEFT:  case KEY_CTRLNUMPAD4:
    {
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */
      if (CurPos>StrSize)
        CurPos=StrSize;
      if (CurPos>0)
        CurPos--;
      /* $ 12.01.2004 IS
         ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
         ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
      */
      while (CurPos>0 && !(!IsWordDiv(TableSet, WordDiv, Str[CurPos]) &&
             IsWordDiv(TableSet, WordDiv, Str[CurPos-1]) && !IsSpace(Str[CurPos])))
      /* IS $ */
      {
        if (!IsSpace(Str[CurPos]) && IsSpace(Str[CurPos-1]))
          break;
        CurPos--;
      }
      Show();
      return(TRUE);
    }

    case KEY_CTRLRIGHT:   case KEY_CTRLNUMPAD6:
    {
      if (CurPos>=StrSize)
        return(FALSE);
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */

      /* $ 21.09.2003 KM
         ��������� ������ � ������
      */
      int Len;
      if (Mask && *Mask)
      {
        char *ShortStr=new char[StrSize+1];
        if (ShortStr==NULL)
          return FALSE;
        strncpy(ShortStr,Str,StrSize);
        Len=strlen(RemoveTrailingSpaces(ShortStr));
        delete[] ShortStr;
        if (Len>CurPos)
          CurPos++;
      }
      else
      {
        Len=StrSize;
        CurPos++;
      }

      /* $ 12.01.2004 IS
         ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
         ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
      */
      while (CurPos<Len/*StrSize*/ && !(IsWordDiv(TableSet, WordDiv,Str[CurPos]) &&
             !IsWordDiv(TableSet, WordDiv, Str[CurPos-1])))
      /* IS $ */
      /* KM $ */
      {
        if (!IsSpace(Str[CurPos]) && IsSpace(Str[CurPos-1]))
          break;
        CurPos++;
      }
      Show();
      return(TRUE);
    }

    case KEY_SHIFTDEL:
    {
      if (SelStart==-1 || SelStart>=SelEnd)
        return(FALSE);
      RecurseProcessKey(KEY_CTRLINS);
      DeleteBlock();
      Show();
      return(TRUE);
    }

    case KEY_CTRLINS:     case KEY_CTRLNUMPAD0:
    {
      if (!Flags.Check(FEDITLINE_PASSWORDMODE))
        if (SelStart==-1 || SelStart>=SelEnd)
        {
          /* $ 26.10.2003 KM
             ! ��������� ����������� ������������� ������ � ��������.
          */
          if (Mask && *Mask)
          {
            char *ShortStr=new char[StrSize+1];
            if (ShortStr==NULL)
              return FALSE;
            strncpy(ShortStr,Str,StrSize);
            RemoveTrailingSpaces(ShortStr);
            CopyToClipboard(ShortStr);
            delete[] ShortStr;
          }
          else
            CopyToClipboard(Str);
          /* KM $ */
        }
        else
          if (SelEnd<=StrSize)
          {
            int Ch=Str[SelEnd];
            Str[SelEnd]=0;
            CopyToClipboard(Str+SelStart);
            Str[SelEnd]=Ch;
          }
      return(TRUE);
    }

    case KEY_SHIFTINS:    case KEY_SHIFTNUMPAD0:
    {
        /* $ 15.10.2000 tran
           ���� ������ ����� ���� ������������ �����
           �� �� ��������� ������ �� ������ ��*/
        char *ClipText;
        if (MaxLength==-1)
            ClipText=PasteFromClipboard();
        else
            ClipText=PasteFromClipboardEx(MaxLength);
        /* tran $ */
        if (ClipText==NULL)
          return(TRUE);
        if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS)){
          DeleteBlock();
        }

        for (I=strlen(Str)-1;I>=0 && IsEol(Str[I]);I--)
          Str[I]=0;
        for (I=0;ClipText[I];I++)
          if (IsEol(ClipText[I]))
          {
            if (IsEol(ClipText[I+1]))
              memmove(&ClipText[I],&ClipText[I+1],strlen(&ClipText[I+1])+1);
            if (ClipText[I+1]==0)
              ClipText[I]=0;
            else
              ClipText[I]=' ';
          }
        if (Flags.Check(FEDITLINE_CLEARFLAG))
        {
          LeftPos=0;
          SetString(ClipText);
          Flags.Clear(FEDITLINE_CLEARFLAG);
        }
        else
          InsertString(ClipText);
        /* $ 13.07.2000 SVS
           � PasteFromClipboard ������ �������� ����� new[]
        */
        if(ClipText)
          delete[] ClipText;
        /* SVS $ */
        Show();
      return(TRUE);
    }

    case KEY_SHIFTTAB:
    {
        /* $ 15.08.2000 KM */
        PrevCurPos=CurPos;
        /* KM $ */
        /* $ 12.12.2000 OT KEY_SHIFTTAB Bug Fix*/
        CursorPos-=(CursorPos-1) % TabSize+1;
        SetTabCurPos(CursorPos);
        /* OT $ */
        Show();
      return(TRUE);
    }

    /* $ 13.02.2001 VVM
      + ��������� SHIFT+SPACE */
    case KEY_SHIFTSPACE:
      Key = KEY_SPACE;
    /* VVM $ */
    default:
    {
//      _D(SysLog("Key=0x%08X",Key));

      if (Key==KEY_NONE || Key==KEY_IDLE || Key==KEY_ENTER || Key>=256)
        break;
      if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS))
      {
        if (PrevSelStart!=-1)
        {
          SelStart=PrevSelStart;
          SelEnd=PrevSelEnd;
        }
        DeleteBlock();
      }
      if (InsertKey(Key))
      {
        if (Key==KEY_TAB && Flags.Check(FEDITLINE_CONVERTTABS))
          ReplaceTabs();
        Show();
      }
      return(TRUE);
    }
  }
  return(FALSE);
}

// ��������� Ctrl-Q
int Edit::ProcessCtrlQ(void)
{
  INPUT_RECORD rec;
  DWORD Key;

  while (1)
  {
    Key=GetInputRecord(&rec);
    if (Key!=KEY_NONE && Key!=KEY_IDLE && rec.Event.KeyEvent.uChar.AsciiChar)
      break;

    if(Key==KEY_CONSOLE_BUFFER_RESIZE)
    {
      int Dis=EditOutDisabled;
      EditOutDisabled=0;
      Show();
      EditOutDisabled=Dis;
    }
  }
/*
  EditOutDisabled++;
  if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS))
  {
    DeleteBlock();
  }
  else
    Flags.Clear(FEDITLINE_CLEARFLAG);
  EditOutDisabled--;
*/
  return InsertKey(rec.Event.KeyEvent.uChar.AsciiChar);
}

int Edit::ProcessInsDate(void)
{
  int SizeMacroText=CtrlObject->Macro.GetPlainTextSize()+8;
  SizeMacroText+=8+(!SizeMacroText?strlen(Opt.DateFormat):0);
  SizeMacroText*=4;
  char *TStr=(char*)alloca(SizeMacroText);
  char *Fmt=(char*)alloca(SizeMacroText);
  if(!TStr || !Fmt)
    return FALSE;

  CtrlObject->Macro.GetPlainText(Fmt);
  if(MkStrFTime(TStr,SizeMacroText,Fmt))
  {
    InsertString(TStr);
    return TRUE;
  }
  return FALSE;
}

int Edit::ProcessInsPlainText(void)
{
  int SizeMacroText=CtrlObject->Macro.GetPlainTextSize();
  if(SizeMacroText)
  {
    SizeMacroText+=8;
    char *TStr=(char*)alloca(SizeMacroText);
    if(!TStr)
      return FALSE;

    CtrlObject->Macro.GetPlainText(TStr);
    InsertString(TStr);
    return TRUE;
  }
  return FALSE;
}

int Edit::InsertKey(int Key)
{
  char *NewStr;
  /* $ 25.07.2000 tran
     + drop-down */
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if (Flags.Check(FEDITLINE_READONLY|FEDITLINE_DROPDOWNBOX))
    return (TRUE);
  /* tran 03.07.2000 $ */
  /* $ 15.08.2000 KM
     ������ � ������ �����.
  */
  if (Key==KEY_TAB && Flags.Check(FEDITLINE_OVERTYPE))
  {
    PrevCurPos=CurPos;
    /* $ 14.12.2000 OT KEY_TAB Bug Fix*/
    CursorPos+=TabSize - (CursorPos % TabSize);
    SetTabCurPos(CursorPos);
    /* OT $ */

    return(TRUE);
  }
  if (Mask && *Mask)
  {
    int MaskLen=strlen(Mask);
    if (CurPos<MaskLen)
    {
      /* $ 15.11.2000 KM
         ����� ����� ���� � ������� ������� KeyMatchedMask,
         ����������� ���������� ������� �� ���� �� �����.
      */
      /* KM $*/
      if (KeyMatchedMask(Key))
      {
        if (!Flags.Check(FEDITLINE_OVERTYPE))
        {
          int i=MaskLen-1;
          while(!CheckCharMask(Mask[i]) && i>CurPos)
            i--;

          for (int j=i;i>CurPos;i--)
          {
            if (CheckCharMask(Mask[i]))
            {
              while(!CheckCharMask(Mask[j-1]))
              {
                if (j<=CurPos)
                  /* $ 15.11.2000 KM
                     ������ continue �� break
                  */
                  break;
                  /* KM $ */
                j--;
              }
              Str[i]=Str[j-1];
              j--;
            }
          }
        }
        PrevCurPos=CurPos;
        Str[CurPos++]=Key;
      }
      else
      {
        // ����� ������� ��� "����� ������ �� �����", �������� ��� SetAttr - ������ '.'
        ;// char *Ptr=strchr(Mask+CurPos,Key);
      }
    }
    else if (CurPos<StrSize)
    {
      PrevCurPos=CurPos;
      Str[CurPos++]=Key;
    }
  }
  else
  {
    if(MaxLength == -1 || StrSize < MaxLength)
    {
      if (CurPos>=StrSize)
      {
        if ((NewStr=(char *)xf_realloc(Str,CurPos+2))==NULL)
          return(FALSE);
        Str=NewStr;
        sprintf(&Str[StrSize],"%*s",CurPos-StrSize,"");
        //memset(Str+StrSize,' ',CurPos-StrSize);Str[CurPos+1]=0;
        StrSize=CurPos+1;
      }
      else
        if (!Flags.Check(FEDITLINE_OVERTYPE))
          StrSize++;
      if ((NewStr=(char *)xf_realloc(Str,StrSize+1))==NULL)
        return(TRUE);
      Str=NewStr;

      if (!Flags.Check(FEDITLINE_OVERTYPE))
      {
        memmove(Str+CurPos+1,Str+CurPos,StrSize-CurPos);
        if (SelStart!=-1)
        {
          if (SelEnd!=-1 && CurPos<SelEnd)
            SelEnd++;
          if (CurPos<SelStart)
            SelStart++;
        }
      }
      PrevCurPos=CurPos;
      Str[CurPos++]=Key;
    }
    else if (Flags.Check(FEDITLINE_OVERTYPE))
    {
      if(CurPos < StrSize)
      {
        PrevCurPos=CurPos;
        Str[CurPos++]=Key;
      }
    }
    else
      MessageBeep(MB_ICONHAND);
  }
  /* KM $ */
  Str[StrSize]=0;
  return(TRUE);
}

/* $ 28.07.2000 SVS
   ! ����� �������������� �������� ��� ��������� ColorUnChanged
*/
void Edit::SetObjectColor(int Color,int SelColor,int ColorUnChanged)
{
  Edit::Color=Color;
  Edit::SelColor=SelColor;
  Edit::ColorUnChanged=ColorUnChanged;
}
/* SVS $ */


void Edit::GetString(char *Str,int MaxSize)
{
  strncpy(Str,Edit::Str,MaxSize-1);
  Str[MaxSize-1]=0;
}


const char* Edit::GetStringAddr()
{
  return(Edit::Str);
}


/* $ 25.07.2000 tran
   ����������:
   � ���� ������ DropDownBox �� ��������������
   ��� ������ ���� ����� ���������� ��� ��������� �� ������� */
void Edit::SetString(const char *Str)
{
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if ( Flags.Check(FEDITLINE_READONLY) )
    return;
  /* tran 03.07.2000 $ */
  Select(-1,0);
  SetBinaryString(Str,strlen(Str));
}


void Edit::SetEOL(const char *EOL)
{
  if (EOL[0]=='\r')
    if (EOL[1]=='\n')
      EndType=EOL_CRLF;
    else
      EndType=EOL_CR;
  else
    if (EOL[0]=='\n')
      EndType=EOL_LF;
    else
      EndType=EOL_NONE;
}


/* $ 25.07.2000 tran
   ����������:
   � ���� ������ DropDownBox �� ��������������
   ��� �� ���������� ������ �� SetString � �� ������ Editor
   � Dialog �� ����� �� ���������� */
void Edit::SetBinaryString(const char *Str,int Length)
{
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if ( Flags.Check(FEDITLINE_READONLY) )
    return;
  /* tran 03.07.2000 $ */

  // ��������� ������������ �������, ���� ��������� MaxLength
  if(MaxLength != -1 && Length > MaxLength)
  {
    Length=MaxLength; // ??
  }

  if (Length>0 && !Flags.Check(FEDITLINE_PARENT_SINGLELINE))
  {
    if (Str[Length-1]=='\r')
    {
      EndType=EOL_CR;
      Length--;
    }
    else
    {
      if (Str[Length-1]=='\n')
      {
        Length--;
        if (Length>0 && Str[Length-1]=='\r')
        {
          EndType=EOL_CRLF;
          Length--;
        }
        else
          EndType=EOL_LF;
      }
      else
        EndType=EOL_NONE;
    }
  }
  /* $ 15.08.2000 KM
     ������ � ������
  */
  /* $ 12.11.2000 KM
     ����� ����� ���� �� SVS ����������� ����� ������.
     �� �� ������� �������.
  */
  CurPos=0;
  if (Mask && *Mask)
  {
    /* $ 21.09.2003 KM
       ������� ������ � ������.
    */
    RefreshStrByMask(TRUE);
    /* KM $ */

    /* $ 26.10.2003 KM
       ! ������������� ������� �� ��������� � ������ �����
    */
    for (int i=0,j=0;j<strlen(Mask),j<Length;)
    {
      if (CheckCharMask(Mask[i]))
      {
        int goLoop=FALSE;
        if (KeyMatchedMask(Str[j]))
          InsertKey(Str[j]);
        else
          goLoop=TRUE;
        j++;
        if (goLoop) continue;
      }
      else
      {
        PrevCurPos=CurPos;
        CurPos++;
      }
      i++;
    }
    /* KM $ */

    /* ����� ���������� ������� (*Str==0), �.�. ��� ������� ������
       ������ �������� ����� ����� SetBinaryString("",0)
       �.�. ����� ������� �� ���������� "�������������" ������ � ������
    */
    RefreshStrByMask(*Str==0);
  }
  /* KM $ */
  else
  {
    char *NewStr=(char *)xf_realloc(Edit::Str,Length+1);
    if (NewStr==NULL)
      return;
    Edit::Str=NewStr;
    StrSize=Length;
    memcpy(Edit::Str,Str,Length);
    Edit::Str[Length]=0;
    if (Flags.Check(FEDITLINE_CONVERTTABS))
      ReplaceTabs();
    PrevCurPos=CurPos;
    CurPos=StrSize;
  }
  /* KM $ */
}


void Edit::GetBinaryString(const char *&Str,const char **EOL,int &Length)
{
  Str=Edit::Str;
  if (EOL!=NULL)
    *EOL=EOL_TYPE_CHARS[EndType];
  Length=StrSize;
}


int Edit::GetSelString(char *Str,int MaxSize)
{
  if (SelStart==-1 || SelEnd!=-1 && SelEnd<=SelStart ||
      SelStart>=StrSize)
  {
    *Str=0;
    return(FALSE);
  }
  int CopyLength;
  if (SelEnd==-1)
    CopyLength=MaxSize-1;
  else
    CopyLength=Min(MaxSize-1,SelEnd-SelStart);
  strncpy(Str,Edit::Str+SelStart,CopyLength);
  Str[CopyLength]=0;
  return(TRUE);
}


void Edit::InsertString(const char *Str)
{
  /* $ 25.07.2000 tran
     + drop-down */
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if (Flags.Check(FEDITLINE_READONLY|FEDITLINE_DROPDOWNBOX))
    return;
  /* tran 03.07.2000 $ */
  /* tran 25.07.2000 $ */

  Select(-1,0);
  InsertBinaryString(Str,strlen(Str));
}


void Edit::InsertBinaryString(const char *Str,int Length)
{
  char *NewStr;

  /* $ 25.07.2000 tran
     + drop-down */
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if (Flags.Check(FEDITLINE_READONLY|FEDITLINE_DROPDOWNBOX))
    return;
  /* tran 03.07.2000 $ */

  Flags.Clear(FEDITLINE_CLEARFLAG);

  /* $ 18.08.2000 KM
     ������������ ����� �����.
  */
  if (Mask && *Mask)
  {
    int Pos=CurPos;
    int MaskLen=strlen(Mask);
    if (Pos<MaskLen)
    {
      int StrLen=(MaskLen-Pos>Length)?Length:MaskLen-Pos;
      /* $ 15.11.2000 KM
         ������� ����������� ��� ���������� ������ PasteFromClipboard
         � ������ � ������
      */
      for (int i=Pos,j=0;j<StrLen+Pos;)
      {
        if (CheckCharMask(Mask[i]))
        {
          int goLoop=FALSE;
          if (KeyMatchedMask(Str[j]))
            InsertKey(Str[j]);
          else
            goLoop=TRUE;
          j++;
          if (goLoop) continue;
        }
        else
        {
          PrevCurPos=CurPos;
          CurPos++;
        }
        i++;
      }
      /* KM $ */
    }
    RefreshStrByMask();
  }
  else
  {
    if(MaxLength != -1 && StrSize+Length > MaxLength)
    {
      // ��������� ������������ �������, ���� ��������� MaxLength
      if(StrSize < MaxLength)
      {
        Length=MaxLength-StrSize;
      }
    }

    if(MaxLength == -1 || StrSize+Length <= MaxLength)
    {
      if (CurPos>StrSize)
      {
        if ((NewStr=(char *)xf_realloc(Edit::Str,CurPos+1))==NULL)
          return;
        Edit::Str=NewStr;
        sprintf(&Edit::Str[StrSize],"%*s",CurPos-StrSize,"");
        //memset(Edit::Str+StrSize,' ',CurPos-StrSize);Edit::Str[CurPos+1]=0;
        StrSize=CurPos;
      }

      int TmpSize=StrSize-CurPos;
      char *TmpStr=new char[TmpSize+16];
      if(!TmpStr)
        return;

      memcpy(TmpStr,&Edit::Str[CurPos],TmpSize);

      StrSize+=Length;
      if ((NewStr=(char *)xf_realloc(Edit::Str,StrSize+1))==NULL)
      {
        delete[] TmpStr;
        return;
      }
      Edit::Str=NewStr;
      memcpy(&Edit::Str[CurPos],Str,Length);
      /* $ 15.08.2000 KM */
      PrevCurPos=CurPos;
      /* KM $ */
      CurPos+=Length;
      memcpy(Edit::Str+CurPos,TmpStr,TmpSize);
      Edit::Str[StrSize]=0;
      /* $ 13.07.2000 SVS
         ��� �� �������� ����� new[]...
      */
      delete[] TmpStr;
      /* SVS $*/

      if (Flags.Check(FEDITLINE_CONVERTTABS))
        ReplaceTabs();
    }
    else
      MessageBeep(MB_ICONHAND);
  }
  /* KM $ */
}


int Edit::GetLength()
{
  return(StrSize);
}


/* $ 12.08.2000 KM */
// ������� ��������� ����� ����� � ������ Edit
void Edit::SetInputMask(const char *InputMask)
{
  if (Mask)
    xf_free(Mask);

  if (InputMask && *InputMask)
  {
    if((Mask=strdup(InputMask)) == NULL)
      return;
    RefreshStrByMask(TRUE);
  }
  else
    Mask=NULL;
}


// ������� ���������� ��������� ������ ����� �� ����������� Mask
void Edit::RefreshStrByMask(int InitMode)
{
  int i;
  if (Mask && *Mask)
  {
    int MaskLen=strlen(Mask);
    /* $12.11.2000 KM
       ��������� ��������� � ������ � ������.
       ������ Str �� ����� ���� ������� Mask �
       MaxLength ����� ����� ����� �����.
    */
    if (StrSize!=MaskLen)
    {
      char *NewStr=(char *)xf_realloc(Str,MaskLen+1);
      if (NewStr==NULL)
        return;
      Str=NewStr;
      for (i=StrSize;i<MaskLen;i++)
        Str[i]=' ';
      StrSize=MaxLength=MaskLen;
      Str[StrSize]=0;
    }
    /* KM $ */
    for (i=0;i<MaskLen;i++)
    {
      if (InitMode)
        Str[i]=' ';
      if (!CheckCharMask(Mask[i]))
        Str[i]=Mask[i];
    }
  }
}
/* KM $ */


int Edit::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  if ((MouseEvent->dwButtonState & 3)==0)
    return(FALSE);
  if (MouseEvent->dwMousePosition.X<X1 || MouseEvent->dwMousePosition.X>X2 ||
      MouseEvent->dwMousePosition.Y!=Y1)
    return(FALSE);
  //SetClearFlag(0); // ����� ������ ��� ��������� � ������ �����-������?
  SetTabCurPos(MouseEvent->dwMousePosition.X - X1 + LeftPos);
  /* $ 05.09.2001 SVS
    ��� ������������ ������ ������� ���������
    � ��� �� � ������?????
  */
  if (!Flags.Check(FEDITLINE_PERSISTENTBLOCKS))
    Select(-1,0);
  /* SVS $ */
  Show();
  return(TRUE);
}


/* $ 03.08.2000 KM
   ������� ������ �������� ��-�� �������������
   ���������� ������ ����� ����.
*/
int Edit::Search(char *Str,int Position,int Case,int WholeWords,int Reverse)
{
  int I,J,Length=strlen(Str);
  if (Reverse)
  {
    Position--;
    if (Position>=StrSize)
      Position=StrSize-1;
    if (Position<0)
      return(FALSE);
  }
  if (Position<StrSize && *Str)
    for (I=Position;Reverse && I>=0 || !Reverse && I<StrSize;Reverse ? I--:I++)
    {
      for (J=0;;J++)
      {
        if (Str[J]==0)
        {
          CurPos=I;
          return(TRUE);
        }
        /* $ 03.08.2000 KM
           �������� ����� ���� ��� ������ ��� ������ ����� ����
        */
        if (WholeWords)
        {
          int ChLeft,ChRight;
          int locResultLeft=FALSE;
          int locResultRight=FALSE;

          ChLeft=(TableSet==NULL) ? Edit::Str[I-1]:TableSet->DecodeTable[Edit::Str[I-1]];
          /* $ 07.09.2000 KM
             ����������� ����� ��� ������ �� ����� ������.
          */
          if (I>0)
            locResultLeft=(IsSpace(ChLeft) || strchr(WordDiv,ChLeft)!=NULL);
          else
            locResultLeft=TRUE;
          /* KM $ */
          if (I+Length<StrSize)
          {
            ChRight=(TableSet==NULL) ? Edit::Str[I+Length]:TableSet->DecodeTable[Edit::Str[I+Length]];
            locResultRight=(IsSpace(ChRight) || strchr(WordDiv,ChRight)!=NULL);
          }
          else
            locResultRight=TRUE;
          if (!locResultLeft || !locResultRight)
            break;
        }
        /* $ KM */
        int Ch=(TableSet==NULL) ? Edit::Str[I+J]:TableSet->DecodeTable[Edit::Str[I+J]];
        if (Case)
        {
          if (Ch!=Str[J])
            break;
        }
        else
        {
          if (LocalUpper(Ch)!=LocalUpper(Str[J]))
            break;
        }
      }
    }
  return(FALSE);
}
/* KM $ */


void Edit::ReplaceTabs()
{
  char *TabPtr;
  int Pos,S;
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if ( Flags.Check(FEDITLINE_READONLY) )
    return;
  /* tran 03.07.2000 $ */

  while ((TabPtr=(char *)memchr(Str,'\t',StrSize))!=NULL)
  {
    Pos=TabPtr-Str;
    S=TabSize-((TabPtr-Str) % TabSize);
    if(SelStart!=-1)
    {
      if(Pos<=SelStart)
      {
        SelStart+=S-(Pos==SelStart?0:1);
      }
      if(SelEnd!=-1 && Pos<SelEnd)
      {
        SelEnd+=S-1;
      }
    }
    int PrevStrSize=StrSize;
    StrSize+=S-1;
    if (CurPos>Pos)
      CurPos+=S-1;
    Str=(char *)xf_realloc(Str,StrSize+1);
    TabPtr=Str+Pos;
    memmove(TabPtr+S,TabPtr+1,PrevStrSize-Pos);
    memset(TabPtr,' ',S);
#ifdef SHITHAPPENS
    memset(TabPtr,0x01,S);
    *TabPtr=0x02;
#endif
    Str[StrSize]=0;
  }
}

#ifdef SHITHAPPENS
void Edit::ReplaceSpaces(int i)
{
  char *TabPtr;
  int Pos,S;
  char a,b;
  if ( i==0 )
  {
    a=' '; b=0xFA;
  }
  else
  {
    b=' '; a=0xFA;
  }
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if ( Flags.Check(FEDITLINE_READONLY) )
    return;
  /* tran 03.07.2000 $ */

  while ((TabPtr=(char *)memchr(Str,a,StrSize))!=NULL)
  {
    *TabPtr=b;
  }
}
#endif


int Edit::GetTabCurPos()
{
  return(RealPosToTab(CurPos));
}


void Edit::SetTabCurPos(int NewPos)
{
  /* $ 21.09.2003 KM
     ��������� ������ � ������
  */
  int Pos;

  if (Mask && *Mask)
  {
    char *ShortStr=new char[StrSize+1];
    if (ShortStr==NULL)
      return;
    strncpy(ShortStr,Str,StrSize);
    Pos=strlen(RemoveTrailingSpaces(ShortStr));
    delete[] ShortStr;
    if (NewPos>Pos)
      NewPos=Pos;
  }
  /* KM $ */

  CurPos=TabPosToReal(NewPos);
}


int Edit::RealPosToTab(int Pos)
{
  int TabPos,I;

  if (Flags.Check(FEDITLINE_CONVERTTABS) || memchr(Str,'\t',StrSize)==NULL)
    return(Pos);

  for (TabPos=0,I=0;I<Pos;I++)
  {
    if (I>=StrSize)
    {
      TabPos+=Pos-I;
      break;
    }
    if (Str[I]=='\t')
      TabPos+=TabSize - (TabPos % TabSize);
    else
      TabPos++;
  }
  return(TabPos);
}


int Edit::TabPosToReal(int Pos)
{
  int TabPos,I;

  if (Flags.Check(FEDITLINE_CONVERTTABS) || memchr(Str,'\t',StrSize)==NULL)
    return(Pos);

  for (TabPos=0,I=0;TabPos<Pos;I++)
  {
    if (Str[I]=='\t')
    {
      int NewTabPos=TabPos+TabSize - (TabPos % TabSize);
      if (NewTabPos>Pos)
        break;
      TabPos=NewTabPos;
    }
    else
      /* $ 13.04.2001 OT */
      if (I>StrSize)
      /* OT */
      {
        I+=Pos-TabPos;
        break;
      }
      else
        TabPos++;
  }
  return(I);
}


void Edit::Select(int Start,int End)
{
  SelStart=Start;
  SelEnd=End;
/* $ 24.06.2002 SKV
   ���� ������ ��������� �� ������ ������, ���� ��������� �����.
   17.09.2002 ��������� �������. ���������.
*/
  if (SelEnd<SelStart && SelEnd!=-1)
/* SKV $ */
  {
    SelStart=-1;
    SelEnd=0;
  }
  if (SelStart==-1 && SelEnd==-1)
  {
    SelStart=-1;
    SelEnd=0;
  }
//  if (SelEnd>StrSize)
//    SelEnd=StrSize;
}


void Edit::AddSelect(int Start,int End)
{
  if (Start<SelStart || SelStart==-1)
    SelStart=Start;
  if (End==-1 || End>SelEnd && SelEnd!=-1)
    SelEnd=End;
  if (SelEnd>StrSize)
    SelEnd=StrSize;
  if (SelEnd<SelStart && SelEnd!=-1)
  {
    SelStart=-1;
    SelEnd=0;
  }
}


void Edit::GetSelection(int &Start,int &End)
{
  /* $ 17.09.2002 SKV
    ���� ����, ��� ��� ��������� ������ OO design'�,
    ��� ��� ��� � �������� �����.
  */
/*  if (SelEnd>StrSize+1)
    SelEnd=StrSize+1;
  if (SelStart>StrSize+1)
    SelStart=StrSize+1;*/
  /* SKV $ */

  Start=SelStart;
  End=SelEnd;

  if (End>StrSize)
    End=-1;//StrSize;
  if (Start>StrSize)
    Start=StrSize;
}


void Edit::GetRealSelection(int &Start,int &End)
{
  Start=SelStart;
  End=SelEnd;
}


void Edit::DisableEditOut(int Disable)
{
  EditOutDisabled=Disable;
}

void Edit::DisableEncode(int Disable)
{
  EditEncodeDisabled=Disable;
}


void Edit::SetTables(struct CharTableSet *TableSet)
{
  Edit::TableSet=TableSet;
};

void Edit::DeleteBlock()
{
  /* $ 25.07.2000 tran
     + drop-down */
  /* $ 03.07.2000 tran
     + ��������� ReadOnly */
  if (Flags.Check(FEDITLINE_READONLY|FEDITLINE_DROPDOWNBOX))
    return;
  /* tran 03.07.2000 $ */
  /* tran 25.07.2000 $ */

  if (SelStart==-1 || SelStart>=SelEnd)
    return;
  /* $ 15.08.2000 KM
     ���� Mask
  */
  PrevCurPos=CurPos;
  if (Mask && *Mask)
  {
    for (int i=SelStart;i<SelEnd;i++)
    {
      if (CheckCharMask(Mask[i]))
        Str[i]=' ';
    }
    /* $ 18.09.2000 SVS
      ��� Mask - ������ ��������������� ������� :-)
    */
    CurPos=SelStart;
    /* SVS $*/
  }
  else
  {
    int From=SelStart,To=SelEnd;
    if(From>StrSize)From=StrSize;
    if(To>StrSize)To=StrSize;
    memmove(Str+From,Str+To,StrSize-To+1);
    StrSize-=To-From;
    if (CurPos>From)
      if (CurPos<To)
        CurPos=From;
      else
        CurPos-=To-From;
    Str=(char *)xf_realloc(Str,StrSize+1);
  }

  /* KM $ */
  SelStart=-1;
  SelEnd=0;
  Flags.Clear(FEDITLINE_MARKINGBLOCK);
  // OT: �������� �� ������������ �������� ������ ��� �������� � �������
  if (Flags.Check((FEDITLINE_PARENT_SINGLELINE|FEDITLINE_PARENT_MULTILINE)))
  {
    if (LeftPos>CurPos)
      LeftPos=CurPos;
  }
}


void Edit::AddColor(struct ColorItem *col)
{
  if ((ColorCount & 15)==0)
    ColorList=(ColorItem *)xf_realloc(ColorList,(ColorCount+16)*sizeof(*ColorList));
  ColorList[ColorCount++]=*col;
}


int Edit::DeleteColor(int ColorPos)
{
  int Src;
  if (ColorCount==0)
    return(FALSE);
  int Dest=0;
  for (Src=0;Src<ColorCount;Src++)
    if (ColorPos!=-1 && ColorList[Src].StartPos!=ColorPos)
    {
      if (Dest!=Src)
        ColorList[Dest]=ColorList[Src];
      Dest++;
    }
  int DelCount=ColorCount-Dest;
  ColorCount=Dest;
  if (ColorCount==0)
  {
    xf_free (ColorList);
    ColorList=NULL;
  }
  return(DelCount!=0);
}


int Edit::GetColor(struct ColorItem *col,int Item)
{
  if (Item>=ColorCount)
    return(FALSE);
  *col=ColorList[Item];
  return(TRUE);
}


void Edit::ApplyColor()
{
  int Col,I,SelColor0;
  for (Col=0;Col<ColorCount;Col++)
  {
    struct ColorItem *CurItem=ColorList+Col;
    int Start=RealPosToTab(CurItem->StartPos)-LeftPos;
    int End=RealPosToTab(CurItem->EndPos)-LeftPos;
    CHAR_INFO TextData[1024];
    if (Start<=X2 && End>=X1)
    {
      if (Start<X1)
        Start=X1;
      if (End>X2)
        End=X2;
      int Length=End-Start+1;
      if (Length>0 && Length<sizeof(TextData))
      {
        ScrBuf.Read(Start,Y1,End,Y1,TextData);
        /* $ 28.07.2000 SVS
           SelColor ����� ���� � �������� ������.
        */
        SelColor0=SelColor;
        if(SelColor >= COL_FIRSTPALETTECOLOR)
          SelColor0=Palette[SelColor-COL_FIRSTPALETTECOLOR];
        for (I=0;I<Length;I++)
          if (TextData[I].Attributes!=SelColor0)
            TextData[I].Attributes=CurItem->Color;
        /* SVS $ */
        ScrBuf.Write(Start,Y1,TextData,Length);
      }
    }
  }
}

/* $ 24.09.2000 SVS $
  ������� Xlat - ������������� �� �������� QWERTY <-> ������
*/
/* $ 13.12.2000 SVS
   �������������� �������� � �������  Xlat()
*/
void Edit::Xlat(BOOL All)
{
  /* $ 13.12.2000 SVS
     ��� CmdLine - ���� ��� ���������, ����������� ��� ������
  */
  if(All && SelStart == -1 && SelEnd == 0)
  {
    ::Xlat(Str,0,strlen(Str),TableSet,Opt.XLat.Flags);
    Show();
    return;
  }
  /* SVS $ */
  /* $ 10.10.2000 IS
     - ������ �� �������� ����������� ��-�� ����, ��� ���� SelStart==SelEnd
  */
  if(SelStart != -1 && SelStart != SelEnd)
  /* IS $ */
  {
    if(SelEnd == -1)
      SelEnd=strlen(Str);
    ::Xlat(Str,SelStart,SelEnd,TableSet,Opt.XLat.Flags);
    Show();
  }
/* $ 25.11.2000 IS
   ���� ��� ���������, �� ���������� ������� �����. ����� ������������ ��
   ������ ����������� ������ ������������.
*/
  else
  {
   /* $ 10.12.2000 IS
      ������������ ������ �� �����, �� ������� ����� ������, ��� �� �����, ���
      ��������� ����� ������� ������� �� 1 ������
   */
   int start=CurPos, end, StrSize=strlen(Str);
   BOOL DoXlat=TRUE;

   /* $ 12.01.2004 IS
      ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
      ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
   */
   if(IsWordDiv(TableSet,Opt.XLat.WordDivForXlat,Str[start]))
   {
      if(start) start--;
      DoXlat=(!IsWordDiv(TableSet,Opt.XLat.WordDivForXlat,Str[start]));
   }

   if(DoXlat)
   {
    while(start>=0 && !IsWordDiv(TableSet,Opt.XLat.WordDivForXlat,Str[start]))
      start--;
    start++;
    end=start+1;
    while(end<StrSize && !IsWordDiv(TableSet,Opt.XLat.WordDivForXlat,Str[end]))
      end++;
    ::Xlat(Str,start,end,TableSet,Opt.XLat.Flags);
    Show();
   }
   /* 12.01.2004 IS $*/
   /* 10.12.2000 IS $ */
  }
/* IS $ */
}
/* SVS $ */


/* $ 15.11.2000 KM
   ���������: �������� �� ������ � �����������
   �������� ��������, ������������ ������
*/
int Edit::KeyMatchedMask(int Key)
{
  int Inserted=FALSE;
  if (Mask[CurPos]==EDMASK_ANY)
    Inserted=TRUE;
  else if (Mask[CurPos]==EDMASK_DSS && (isdigit(Key) || Key==' ' || Key=='-'))
    Inserted=TRUE;
  /* $ 15.11.2000 KM
     ������ ���������� �������� � �������� �����.
  */
  else if (Mask[CurPos]==EDMASK_DIGIT && (isdigit(Key)))
    Inserted=TRUE;
  /* KM $ */
  else if (Mask[CurPos]==EDMASK_ALPHA && LocalIsalpha(Key))
    Inserted=TRUE;
  /* $ 20.09.2003 KM
     ��������� ��������� hex-��������.
  */
  else if (Mask[CurPos]==EDMASK_HEX && (isdigit(Key) || (LocalUpper(Key)>='A' && LocalUpper(Key)<='F') || (LocalUpper(Key)>='a' && LocalUpper(Key)<='f')))
    Inserted=TRUE;
  /* KM $ */

  return Inserted;
}
/* KM $ */

int Edit::CheckCharMask(char Chr)
{
  return (Chr==EDMASK_ANY || Chr==EDMASK_DIGIT || Chr==EDMASK_DSS || Chr==EDMASK_ALPHA || Chr==EDMASK_HEX)?TRUE:FALSE;
}

void Edit::SetDialogParent(DWORD Sets)
{
  if((Sets&(FEDITLINE_PARENT_SINGLELINE|FEDITLINE_PARENT_MULTILINE)) == (FEDITLINE_PARENT_SINGLELINE|FEDITLINE_PARENT_MULTILINE) ||
     (Sets&(FEDITLINE_PARENT_SINGLELINE|FEDITLINE_PARENT_MULTILINE)) == 0)
    Flags.Clear(FEDITLINE_PARENT_SINGLELINE|FEDITLINE_PARENT_MULTILINE);
  else if(Sets&FEDITLINE_PARENT_SINGLELINE)
  {
    Flags.Clear(FEDITLINE_PARENT_MULTILINE);
    Flags.Set(FEDITLINE_PARENT_SINGLELINE);
  }
  else if(Sets&FEDITLINE_PARENT_MULTILINE)
  {
    Flags.Clear(FEDITLINE_PARENT_SINGLELINE);
    Flags.Set(FEDITLINE_PARENT_MULTILINE);
  }
}
