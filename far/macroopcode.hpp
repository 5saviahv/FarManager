/*
macroopcode.hpp

OpCode ��� ��������

*/

/* Revision: 1.01 02.08.2004 $ */

/*
Modify:
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
  MCODE_OP_SAVE,                    // ������������ ����������. ��� ����������
                                    // ��������� DWORD (��� � $Text).
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


  /* ************************************************************************* */
  // ������� ���������� - ��������� ���������
  MCODE_C_DISABLEOUTPUT=KEY_MACRO_C_BASE,// ����� ��������?
  MCODE_C_WINDOWEDMODE,             // ������� �����?
  MCODE_C_SELECTED,                 // ���������� ���� ����?
  MCODE_C_EOF,                      // ����� �����/��������� ��������?
  MCODE_C_BOF,                      // ������ �����/��������� ��������?
  MCODE_C_EMPTY,                    // ���.������ �����?
  MCODE_C_ICLIP,                    // ���������� ��� ������� ��������
  MCODE_C_ROOTFOLDER,               // ������ MCODE_C_APANEL_ROOT ��� �������� ������

  MCODE_C_APANEL_ISEMPTY,           // �������� ������:  �����?
  MCODE_C_PPANEL_ISEMPTY,           // ��������� ������: �����?
  MCODE_C_APANEL_BOF,               // ������ ���������  ��������?
  MCODE_C_PPANEL_BOF,               // ������ ���������� ��������?
  MCODE_C_APANEL_EOF,               // ����� ���������  ��������?
  MCODE_C_PPANEL_EOF,               // ����� ���������� ��������?
  MCODE_C_APANEL_ROOT,              // ��� �������� ������� �������� ������?
  MCODE_C_PPANEL_ROOT,              // ��� �������� ������� ��������� ������?
  MCODE_C_APANEL_VISIBLE,           // �������� ������:  ������?
  MCODE_C_PPANEL_VISIBLE,           // ��������� ������: ������?
  MCODE_C_APANEL_PLUGIN,            // �������� ������:  ����������?
  MCODE_C_PPANEL_PLUGIN,            // ��������� ������: ����������?
  MCODE_C_APANEL_FOLDER,            // �������� ������:  ������� ������� �������?
  MCODE_C_PPANEL_FOLDER,            // ��������� ������: ������� ������� �������?
  MCODE_C_APANEL_SELECTED,          // �������� ������:  ���������� �������� ����?
  MCODE_C_PPANEL_SELECTED,          // ��������� ������: ���������� �������� ����?
  MCODE_C_APANEL_LEFT,              // �������� ������ �����?
  MCODE_C_PPANEL_LEFT,              // ��������� ������ �����?

  MCODE_C_CMDLINE_BOF,              // ������ � ������ cmd-������ ��������������?
  MCODE_C_CMDLINE_EOF,              // ������ � ������ cmd-������ ��������������?

  /* ************************************************************************* */
  // �� ������� ����������
  MCODE_V_APANEL_CURRENT=KEY_MACRO_V_BASE,// APanel.Current - ��� ����� �� �������� ������
  MCODE_V_PPANEL_CURRENT,           // PPanel.Current - ��� ����� �� ��������� ������
  MCODE_V_APANEL_SELCOUNT,          // APanel.Sel�ount - �������� ������:  ����� ���������� ���������
  MCODE_V_PPANEL_SELCOUNT,          // PPanel.Sel�ount - ��������� ������: ����� ���������� ���������
  MCODE_V_APANEL_PATH,              // APanel.Path - �������� ������:  ���� �� ������
  MCODE_V_PPANEL_PATH,              // PPanel.Path - ��������� ������: ���� �� ������
  MCODE_V_APANEL_WIDTH,             // APanel.Path - �������� ������:  ���� �� ������
  MCODE_V_PPANEL_WIDTH,             // PPanel.Path - ��������� ������: ���� �� ������
  MCODE_V_EDITORSTATE,
};

typedef enum MACRO_OP_CODE TFunction;


#endif // __MACROOPCODE_HPP__
