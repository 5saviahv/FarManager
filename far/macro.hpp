#if defined(DMACRO2)
#include "macro2.hpp"
#else
#ifndef __KEYMACRO_HPP__
#define __KEYMACRO_HPP__
/*
macro.hpp

�������

*/

/* Revision: 1.18 14.09.2001 $ */

/*
Modify:
  14.09.2001 SVS
    - BugZ#9 - ���������
  07.09.2001 SVS
    + CheckCurMacroFlags() - �������� ������ �������� _������������_ �������.
  15.08.2001 SVS
    ! ��������� - ��� ����������� ���� (�� ������ macro2.?pp)
  09.08.2001 SVS
    + IfCondition() - ������ TRUE/FALSE � ����������� �� �������
  22.06.2001 SVS
    + GetMacroPlainText()
  20.06.2001 SVS
    ! �������� ������� ��������� � ����� ����������� �� ����������:
      PlayKeyMacro -> PostTempKeyMacro
    ! TempMacroType ������ �� �������������, �.�. ��� Temp-�������� ��� �����
      ������ ����������� ������������������.
  23.05.2001 SVS
    ! IndexMode - ��������� ������: ������ ������ - ������, ������ - ����������.
  23.05.2001 SVS
    + Sort()
    + IndexMode - ������ ������ �������� � Macros
  16.05.2001 SVS
    + GetCurRecord() - ��� �����
  06.05.2001 DJ
    ! �������� #include
  25.04.2001 SVS
    ! ��� �������� ������ ��� ������ �������� ������� � ������� Check* -
      ������� ����� �������������� ���� :-(
  08.03.2001 SVS
    + ������� MkTextSequence - ������������ ���������� ������������� Sequence
  22.01.2001 SVS
    + ������� MkRegKeyName - ������������ ����� ����� � �������.
  17.01.2001 SVS
    + ������� ��������� ������� ������� � ������� ��� �����������������������:
       int GetIndex(int Key, int Mode);
       int GetRecordSize(int Key, int Mode);
  04.01.2001 SVS
    ! ������� ReadMacros � GetMacroSettings
    + ������� AssignMacroKey
    ! ������� ��������� struct TKeyNames
  26.12.2000 SVS
    + SwitchFlags()
  23.12.2000 SVS
    + int ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr)
    + int PlayKeyMacro(struct MacroRecord *MRec)
    + int PlayKeyMacro(char *KeyBuffer)
  21.12.2000 SVS
    ! ��������� MacroRecord ���������� �� struct.hpp � "�����"
    ! ������� KeyToText ������� �� �������������
  10.09.2000 SVS
    ! ������� ReadMacros ����� �������������� ���������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/
#include "farconst.hpp"

class Panel;

struct MacroRecord
{
  DWORD Flags;
  int   Key;
  int   BufferSize;
  DWORD *Buffer;
};

/* $TODO:
    1. ������� IndexMode[], Sort()
    2. �� Macros �������
       struct MacroRecord *Macros[MACRO_LAST];
*/
class KeyMacro
{
  private:
    // ��� ������ - � ������� ������� �������� ���...
    // 0 - ��� ������, 1 - ������� ������, 2 - ����� ������� ��������
    int Recording;
    int Executing;
    int InternalInput;
    int IsRedrawEditor;

    int Mode;
    int ExecMacroPos;
    int ExecKeyPos;
    int StartMode;
    int StartMacroPos;

    int MacrosNumber;
    struct MacroRecord *Macros;

    int IndexMode[MACRO_LAST][2];

    int RecBufferSize;
    DWORD *RecBuffer;

    struct MacroRecord *TempMacro; // ��������� ����� ��� 1 �����

    class LockScreen *LockScr;

  private:
    int ReadMacros(int ReadMode, char *Buffer, int BufferSize);
    DWORD AssignMacroKey();
    int GetMacroSettings(int Key,DWORD &Flags);
    void InitVars();
    void ReleaseTempBuffer(); // ������� ��������� �����

    // �� ���������� ������������� ������� ������� MacroRecord
    int ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr);
    DWORD SwitchFlags(DWORD& Flags,DWORD Value);
    static long WINAPI AssignMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    char *MkRegKeyName(int IdxMacro,char *RegKeyName);

    BOOL CheckEditSelected(DWORD CurFlags);
    BOOL CheckPanel(int PanelMode,DWORD CurFlags);
    BOOL CheckCmdLine(int CmdLength,DWORD Flags);
    BOOL CheckFileFolder(Panel *ActivePanel,DWORD CurFlags);
    BOOL CheckAll(DWORD CurFlags);
    void Sort(void);
    BOOL IfCondition(DWORD Key,DWORD Flags,DWORD Code);

  public:
    KeyMacro();
    ~KeyMacro();

  public:
    int ProcessKey(int Key);
    int GetKey();
    int PeekKey();

    int  IsRecording() {return(Recording);};
    int  IsExecuting() {return(Executing);};
    void SetMode(int Mode) {KeyMacro::Mode=Mode;};
    int  GetMode() {return(Mode);};

    void RunStartMacro();

    // ��������� ��������� ��������� ������������� �������
    int PostTempKeyMacro(char *KeyBuffer);
    // ��������� ��������� ������ (�������� �������������)
    int PostTempKeyMacro(struct MacroRecord *MRec);

    int  LoadMacros();
    void SaveMacros();

    int GetStartIndex(int Mode) {return IndexMode[Mode<MACRO_LAST?Mode:MACRO_LAST][0];}
    // ������� ��������� ������� ������� ������� � �������
    int GetIndex(int Key, int Mode);
    // ��������� �������, ����������� ��������� ��������
    int GetRecordSize(int Key, int Mode);

    char *GetMacroPlainText(char *Dest);

    void SetRedrawEditor(int Sets){IsRedrawEditor=Sets;}

    // �������� ������ � ������� (���������� ������)
    int GetCurRecord(struct MacroRecord* RBuf=NULL,int *KeyPos=NULL);
    // ��������� ����� �������� ������������ �������.
    BOOL CheckCurMacroFlags(DWORD Flags);

    static char* GetSubKey(int Mode);
    static int   GetSubKey(char *Mode);
    static char *MkTextSequence(DWORD *Buffer,int BufferSize);
};

#endif	// __KEYMACRO_HPP__
#endif // defined(DMACRO2)
