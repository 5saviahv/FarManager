/*
macroopcode.hpp

OpCode ��� ��������

*/

/* Revision: 1.17 07.10.2005 $ */

/*
Modify:
  07.10.2005 SVS
    ! Editor.CurStr -> Editor.Value. ��� ������ �����
    + Dlg.GetValue()
  05.10.2005 SVS
    + Editor.CurStr - ���������� ������� ������
  19.09.2005 SVS
    + MCODE_V_PPANEL_DRIVETYPE, MCODE_V_APANEL_DRIVETYPE
  05.07.2005 SVS
    + ������� � ������� - OldVal=Editor.Set(Index,NewVal)
    + Editor.FileName - ��� �������������� �����
    + Viewer.FileName - ��� ���������������� �����
  06.04.2005 SVS
    + MCODE_F_MSAVE       // b=msave(var)
    + MCODE_C_APANEL_LFN  // �� �������� ������ ������� �����?
    + MCODE_C_PPANEL_LFN  // �� ��������� ������ ������� �����?
  01.04.2005 SVS
    + MCODE_F_PANELITEM
  02.03.2005 SVS
    + MCODE_V_FAR_WIDTH
  15.02.2005 SVS
    + MCODE_F_ITOA
  14.02.2005 SVS
    + MCODE_V_APANEL_OPIFLAGS, MCODE_V_PPANEL_OPIFLAGS, MCODE_V_CMDLINE_VALUE
  08.12.2004 SVS
    + Dlg.ItemCount, Dlg.CurPos, CmdLine.ItemCount, CmdLine.CurPos
  11.11.2004 SVS
    + [A|P]Panel.UNCPath
    ! [A|P]Panel.Count ������������ � [A|P]Panel.ItemCount
    + � ���������� � [A|P]Panel.ItemCount 2 ����� �����:
        "ItemCount" - ����� ��������� � ������� �������
        "CurPos" - ������� ������ � ������� ������� (������� � 1)
    + ��� ������������, ��� ���������:
        "Editor.CurLine"  - ������� ������ � ��������� (������� � 1)
        "Editor.Lines"    - ���������� �����
        "Editor.CurPos"   - ������� ������� ������� � ������ (������� � 1)
  10.11.2004 SVS
    + [A|P]Panel.Count, [A|P]Panel.CurPos
  09.11.2004 SVS
    + MCODE_V_APANEL_TYPE, MCODE_V_PPANEL_TYPE, MCODE_C_APANEL_FILEPANEL, MCODE_C_PPANEL_FILEPANEL
  10.09.2004 SVS
    + UCase (MCODE_F_UCASE), LCase (MCODE_F_UCASE)
  05.08.2004 SVS
    + MCODE_V_VIEWERSTATE, MCODE_F_FSPLIT, MCODE_F_MSGBOX, MCODE_C_CMDLINE_EMPTY, MCODE_C_CMDLINE_SELECTED, MCODE_V_DLGITEMTYPE
  02.08.2004 SVS
    + MCODE_C_CMDLINE_BOF, MCODE_C_CMDLINE_EOF
  07.07.2004 SVS & AN
    + ���
*/

#ifndef __MACROOPCODE_HPP__
#define __MACROOPCODE_HPP__

#include "keys.hpp"

enum MACRO_OP_CODE {
  /* ************************************************************************* */
  MCODE_OP_EXIT=KEY_MACRO_OP_BASE,  // ������������� ��������� ���������� �����������������������

  MCODE_OP_JMP,                     // Jumps..
  MCODE_OP_JZ,
  MCODE_OP_JNZ,
  MCODE_OP_JLT,
  MCODE_OP_JLE,
  MCODE_OP_JGT,
  MCODE_OP_JGE,

  MCODE_OP_EXPR,                    // ������� ������ (������������� �����)
  MCODE_OP_DOIT,                    // ������� ����� (������� ����� - Result)
  MCODE_OP_SAVE,                    // ������������ ����������. ��� ���������� ��������� DWORD (��� � $Text).
  MCODE_OP_SAVEREPCOUNT,
  MCODE_OP_PUSHINT,                 // �������� �������� �� ����. ����
  MCODE_OP_PUSHSTR,                 // �������� - ��������� DWORD
  MCODE_OP_PUSHVAR,                 // ��� ��������� ������� (��� � $Text)

  MCODE_OP_REP,                     // $rep - ������� ������ �����
  MCODE_OP_END,                     // $end - ������� ����� �����/�������

  MCODE_OP_IF,                      // ������-�� ��� ������ � �������
  MCODE_OP_ELSE,                    // �� ������� ������� :)
  MCODE_OP_WHILE,

  // ����������� ��������
  MCODE_OP_NEGATE,
  MCODE_OP_NOT,

  // ���������� ��������
  MCODE_OP_LT,
  MCODE_OP_LE,
  MCODE_OP_GT,
  MCODE_OP_GE,
  MCODE_OP_EQ,
  MCODE_OP_NE,

  MCODE_OP_ADD,
  MCODE_OP_SUB,
  MCODE_OP_MUL,
  MCODE_OP_DIV,

  MCODE_OP_AND,
  MCODE_OP_OR,
  MCODE_OP_BITAND,
  MCODE_OP_BITOR,
  MCODE_OP_BITXOR,


  /* ************************************************************************* */
  MCODE_OP_XLAT,
  MCODE_OP_DATE,
  MCODE_OP_PLAINTEXT,

  MCODE_OP_MACROMODE,               // ������� ����� ���������� ������ �� �����
  MCODE_OP_SWITCHKBD,               // ����������� ��������� ����������
  MCODE_OP_ICLIP,                   // ���������� ��� ������� ��������

#if defined(MOUSEKEY)
  MCODE_OP_SELWORD,
#endif


  /* ************************************************************************* */
  // �������
  MCODE_F_NOFUNC=KEY_MACRO_F_BASE,
  MCODE_F_ABS,
  MCODE_F_MIN,
  MCODE_F_MAX,
  MCODE_F_IIF,
  MCODE_F_SUBSTR,
  MCODE_F_INDEX,
  MCODE_F_RINDEX,
  MCODE_F_LEN,
  MCODE_F_STRING,
  MCODE_F_INT,
  MCODE_F_DATE,
  MCODE_F_XLAT,                     // �������� XLat: Param=0 ��� 1
  MCODE_F_MENU_CHECKHOTKEY,         // ���� ����� �����?
  MCODE_F_ENVIRON,                  // �������� �������� ���������� �����
  MCODE_F_FEXIST,                   // �������� ������������� �����/��������
  MCODE_F_FATTR,                    // ���������� �������� �������� �������
  MCODE_F_FSPLIT,                   // ���������� �������� ���������� ����
  MCODE_F_MSGBOX,                   // MsgBox("Title","Text",flags)
  MCODE_F_UCASE,                    // UpperCase
  MCODE_F_LCASE,                    // LowerCase
  MCODE_F_ITOA,                     //
  MCODE_F_PANELITEM,                // V=PanelItem(Panel,Index,TypeInfo)
  MCODE_F_MSAVE,                    // b=msave(var)
  MCODE_F_EDITOR_SET,               // N=Editor.Set(N,Var)
  MCODE_F_DLG_GETVALUE,             // V=Dlg.GetValue(ID,N)

  /* ************************************************************************* */
  // ������� ���������� - ��������� ���������
  MCODE_C_DISABLEOUTPUT=KEY_MACRO_C_BASE,// ����� ��������?
  MCODE_C_WINDOWEDMODE,             // ������� �����?
  MCODE_C_BOF,                      // ������ �����/��������� ��������?
  MCODE_C_EOF,                      // ����� �����/��������� ��������?
  MCODE_C_EMPTY,                    // ���.������ �����?
  MCODE_C_SELECTED,                 // ���������� ���� ����?
  MCODE_C_ICLIP,                    // ���������� ��� ������� ��������
  MCODE_C_ROOTFOLDER,               // ������ MCODE_C_APANEL_ROOT ��� �������� ������

  MCODE_C_APANEL_BOF,               // ������ ���������  ��������?
  MCODE_C_PPANEL_BOF,               // ������ ���������� ��������?
  MCODE_C_APANEL_EOF,               // ����� ���������  ��������?
  MCODE_C_PPANEL_EOF,               // ����� ���������� ��������?
  MCODE_C_APANEL_ISEMPTY,           // �������� ������:  �����?
  MCODE_C_PPANEL_ISEMPTY,           // ��������� ������: �����?
  MCODE_C_APANEL_SELECTED,          // �������� ������:  ���������� �������� ����?
  MCODE_C_PPANEL_SELECTED,          // ��������� ������: ���������� �������� ����?
  MCODE_C_APANEL_ROOT,              // ��� �������� ������� �������� ������?
  MCODE_C_PPANEL_ROOT,              // ��� �������� ������� ��������� ������?
  MCODE_C_APANEL_VISIBLE,           // �������� ������:  ������?
  MCODE_C_PPANEL_VISIBLE,           // ��������� ������: ������?
  MCODE_C_APANEL_PLUGIN,            // �������� ������:  ����������?
  MCODE_C_PPANEL_PLUGIN,            // ��������� ������: ����������?
  MCODE_C_APANEL_FILEPANEL,         // �������� ������:  ��������?
  MCODE_C_PPANEL_FILEPANEL,         // ��������� ������: ��������?
  MCODE_C_APANEL_FOLDER,            // �������� ������:  ������� ������� �������?
  MCODE_C_PPANEL_FOLDER,            // ��������� ������: ������� ������� �������?
  MCODE_C_APANEL_LEFT,              // �������� ������ �����?
  MCODE_C_PPANEL_LEFT,              // ��������� ������ �����?
  MCODE_C_APANEL_LFN,               // �� �������� ������ ������� �����?
  MCODE_C_PPANEL_LFN,               // �� ��������� ������ ������� �����?

  MCODE_C_CMDLINE_BOF,              // ������ � ������ cmd-������ ��������������?
  MCODE_C_CMDLINE_EOF,              // ������ � ����� cmd-������ ��������������?
  MCODE_C_CMDLINE_EMPTY,            // ���.������ �����?
  MCODE_C_CMDLINE_SELECTED,         // � ���.������ ���� ��������� �����?

  /* ************************************************************************* */
  // �� ������� ����������
  MCODE_V_APANEL_CURRENT=KEY_MACRO_V_BASE,// APanel.Current - ��� ����� �� �������� ������
  MCODE_V_PPANEL_CURRENT,           // PPanel.Current - ��� ����� �� ��������� ������
  MCODE_V_APANEL_SELCOUNT,          // APanel.SelCount - �������� ������:  ����� ���������� ���������
  MCODE_V_PPANEL_SELCOUNT,          // PPanel.SelCount - ��������� ������: ����� ���������� ���������
  MCODE_V_APANEL_PATH,              // APanel.Path - �������� ������:  ���� �� ������
  MCODE_V_PPANEL_PATH,              // PPanel.Path - ��������� ������: ���� �� ������
  MCODE_V_APANEL_UNCPATH,           // APanel.UNCPath - �������� ������:  UNC-���� �� ������
  MCODE_V_PPANEL_UNCPATH,           // PPanel.UNCPath - ��������� ������: UNC-���� �� ������
  MCODE_V_APANEL_WIDTH,             // APanel.Width - �������� ������:  ������ ������
  MCODE_V_PPANEL_WIDTH,             // PPanel.Width - ��������� ������: ������ ������
  MCODE_V_APANEL_TYPE,              // ��� �������� ������
  MCODE_V_PPANEL_TYPE,              // ��� ��������� ������
  MCODE_V_APANEL_ITEMCOUNT,         // APanel.ItemCount - �������� ������:  ����� ���������
  MCODE_V_PPANEL_ITEMCOUNT,         // PPanel.ItemCount - ��������� ������: ����� ���������
  MCODE_V_APANEL_CURPOS,            // APanel.CurPos - �������� ������:  ������� ������
  MCODE_V_PPANEL_CURPOS,            // PPanel.CurPos - ��������� ������: ������� ������
  MCODE_V_APANEL_OPIFLAGS,          // APanel.OPIFlags - �������� ������: ����� ��������� �������
  MCODE_V_PPANEL_OPIFLAGS,          // PPanel.OPIFlags - ��������� ������: ����� ��������� �������
  MCODE_V_APANEL_DRIVETYPE,         // APanel.DriveType - �������� ������: ��� �������
  MCODE_V_PPANEL_DRIVETYPE,         // PPanel.DriveType - ��������� ������: ��� �������
  MCODE_V_FAR_WIDTH,                // Far.Width - ������ ����������� ����
  MCODE_V_ITEMCOUNT,                // ItemCount - ����� ��������� � ������� �������
  MCODE_V_CURPOS,                   // CurPos - ������� ������ � ������� �������
  MCODE_V_EDITORFILENAME,           // Editor.FileName - ��� �������������� �����
  MCODE_V_EDITORLINES,              // Editor.Lines - ���������� ����� � ���������
  MCODE_V_EDITORCURLINE,            // Editor.CurLine - ������� ����� � ��������� (� ���������� � Count)
  MCODE_V_EDITORCURPOS,             // Editor.CurPos - ������� ���. � ���������
  MCODE_V_EDITORSTATE,              // Editor.State
  MCODE_V_EDITORVALUE,              // Editor.Value - ���������� ������� ������
  MCODE_V_DLGITEMTYPE,              // Dlg.ItemType
  MCODE_V_DLGITEMCOUNT,             // Dlg.ItemCount
  MCODE_V_DLGCURPOS,                // Dlg.CurPos
  MCODE_V_VIEWERFILENAME,           // Viewer.FileName - ��� ���������������� �����
  MCODE_V_VIEWERSTATE,              // Viewer.State

  MCODE_V_CMDLINE_ITEMCOUNT,        // CmdLine.ItemCount
  MCODE_V_CMDLINE_CURPOS,           // CmdLine.CurPos
  MCODE_V_CMDLINE_VALUE,            // CmdLine.Value
};

typedef enum MACRO_OP_CODE TFunction;


#endif // __MACROOPCODE_HPP__
