#ifndef __KEYMACRO_HPP__
#define __KEYMACRO_HPP__
/*
macro.hpp

�������

*/

/* Revision: 1.26 12.09.2003 $ */

/*
Modify:
  12.09.2003 SVS
    + ��������� ������� KeyMacro::GetMacroPlainTextSize() - ������ plain-text
  15.07.2003 SVS
    + KeyMacro::CheckInsidePlugin() - "�� ������ �������?"
    + KeyMacro::DropProcess() - �������� ������� ����������� ������.
  02.05.2003 SVS
    - BugZ#790 - �������������� ������� ����� ����� ��������� ��� ����������?
    + IsExecutingLastKey() - ������ �������� ��... "��� ��������� ������� ������������?"
  19.08.2002 SVS
    + KeyMacro::KeyFromBuffer() - ����� �� ��� ��������� ������ ������� ��
      ������
  02.06.2002 SVS
    ! ��������� const
    ! ParseMacroString ����� public
  12.04.2002 SVS
    ! ������ #if/#endif - ����� �� ������ MAK-����� (��������������� ����)
    ! SaveMacros - ���� ��������
  03.03.2002 SVS
    + TempMacroNumber - ���������� �������� �� ��������� �������
  10.12.2001 SVS
    + IsDsableOutput() - �������� �� "��������������"
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
    2. �� MacroPROM �������
       struct MacroRecord *MacroPROM[MACRO_LAST];
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

    // �.�. MacroPROM (Programmable Read-Only Memory) -
    // ���� "�����" ������ ������ ��� ������ �������� (��������� ������),
    // � ��������� ����� MacroRAM
    int MacroPROMCount;
    struct MacroRecord *MacroPROM;

    // �.�. MacroRAM - ������� ����������
    int MacroRAMCount;
    struct MacroRecord *MacroRAM;

    int IndexMode[MACRO_LAST][2];

    int RecBufferSize;
    DWORD *RecBuffer;


    class LockScreen *LockScr;

  private:
    int ReadMacros(int ReadMode, char *Buffer, int BufferSize);
    DWORD AssignMacroKey();
    int GetMacroSettings(int Key,DWORD &Flags);
    void InitVars(BOOL InitedRAM=TRUE);
    void InitVarsPROM();
    void ReleaseTempBuffer(BOOL All=FALSE); // ������� ��������� �����

    DWORD SwitchFlags(DWORD& Flags,DWORD Value);
    static long WINAPI AssignMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    char *MkRegKeyName(int IdxMacro,char *RegKeyName);

    BOOL CheckEditSelected(DWORD CurFlags);
    BOOL CheckInsidePlugin(DWORD CurFlags);
    BOOL CheckPanel(int PanelMode,DWORD CurFlags);
    BOOL CheckCmdLine(int CmdLength,DWORD Flags);
    BOOL CheckFileFolder(Panel *ActivePanel,DWORD CurFlags);
    BOOL CheckAll(DWORD CurFlags);
    void Sort(void);
    BOOL IfCondition(DWORD Key,DWORD Flags,DWORD Code);
    DWORD KeyFromBuffer(struct MacroRecord *MR,int KeyPos);

  public:
    KeyMacro();
    ~KeyMacro();

  public:
    int ProcessKey(int Key);
    int GetKey();
    int PeekKey();

    int  IsRecording() {return(Recording);};
    int  IsExecuting() {return(Executing);};
    int  IsExecutingLastKey();
    int  IsDsableOutput() {return CheckCurMacroFlags(MFLAGS_DISABLEOUTPUT);};
    void SetMode(int Mode) {KeyMacro::Mode=Mode;};
    int  GetMode() {return(Mode);};

    void RunStartMacro();

    // ��������� ��������� ��������� ������������� �������
    int PostTempKeyMacro(char *KeyBuffer);
    // ��������� ��������� ������ (�������� �������������)
    int PostTempKeyMacro(struct MacroRecord *MRec);

    int  LoadMacros(BOOL InitedRAM=TRUE);
    void SaveMacros(BOOL AllSaved=TRUE);

    int GetStartIndex(int Mode) {return IndexMode[Mode<MACRO_LAST?Mode:MACRO_LAST][0];}
    // ������� ��������� ������� ������� ������� � �������
    int GetIndex(int Key, int Mode);
    // ��������� �������, ����������� ��������� ��������
    int GetRecordSize(int Key, int Mode);

    char *GetMacroPlainText(char *Dest);
    int   GetMacroPlainTextSize();

    void SetRedrawEditor(int Sets){IsRedrawEditor=Sets;}

    // �������� ������ � ������� (���������� ������)
    int GetCurRecord(struct MacroRecord* RBuf=NULL,int *KeyPos=NULL);
    // ��������� ����� �������� ������������ �������.
    BOOL CheckCurMacroFlags(DWORD Flags);

    static char* GetSubKey(int Mode);
    static int   GetSubKey(char *Mode);
    static char *MkTextSequence(DWORD *Buffer,int BufferSize);
    // �� ���������� ������������� ������� ������� MacroRecord
    int ParseMacroString(struct MacroRecord *CurMacro,const char *BufPtr);
    void DropProcess();
};

#endif	// __KEYMACRO_HPP__
