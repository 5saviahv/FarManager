#ifndef __KEYMACRO_HPP__
#define __KEYMACRO_HPP__
/*
macro.hpp

�������

*/

/* Revision: 1.31 28.10.2003 $ */

/*
Modify:
  28.10.2003 SVS
    ! Executing -> MacroState.Executing
  15.10.2003 SVS
    + GetMacroKeyInfo - ���������� �� ��������� ������������.
    + ����������/�������������� ��������������.
  04.10.2003 SVS
    ! ���� ��������� - ��� �������� ��. 01715.Macro.txt
  26.09.2003 SVS
    ! ��������������
      GetMacroPlainText        -> GetPlainText
      GetMacroPlainTextSize    -> GetPlainTextSize
  22.09.2003 SVS
    + KeyMacro::KeyToBuffer() - ��������� ��� � �����
    ! KeyMacro::IfCondition() - ���������� int
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

#define STACKLEVEL	16

struct MacroState
{
  int Executing;
  int MacroPC;
  int ExecLIBPos;
  int MacroWORKCount;
  struct MacroRecord *MacroWORK; // �.�. ������� ����������
};

/* $TODO:
    1. ������� IndexMode[], Sort()
    2. �� MacroLIB �������
       struct MacroRecord *MacroLIB[MACRO_LAST];
*/
class KeyMacro
{
  private:
    DWORD MacroVersion;
    // ��� ������ - � ������� ������� �������� ���...
    // 0 - ��� ������, 1 - ������� ������, 2 - ����� ������� ��������
    int Recording;
    int InternalInput;
    int IsRedrawEditor;

    int Mode;
    int StartMode;

    struct MacroState Work;
    struct MacroState PCStack[STACKLEVEL];
    int CurPCStack;

    // ���� "�����" ������ ������ ��� ������ �������� (��������� ������),
    // � ��������� ����� MacroWORK
    int MacroLIBCount;
    struct MacroRecord *MacroLIB;

    int IndexMode[MACRO_LAST][2];

    int RecBufferSize;
    DWORD *RecBuffer;

    class LockScreen *LockScr;

  private:
    int ReadMacros(int ReadMode, char *Buffer, int BufferSize);
    DWORD AssignMacroKey();
    int GetMacroSettings(int Key,DWORD &Flags);
    void InitInternalVars(BOOL InitedRAM=TRUE);
    void InitInternalLIBVars();
    void ReleaseWORKBuffer(BOOL All=FALSE); // ������� ��������� �����

    DWORD SwitchFlags(DWORD& Flags,DWORD Value);
    char *MkRegKeyName(int IdxMacro,char *RegKeyName);

    BOOL CheckEditSelected(DWORD CurFlags);
    BOOL CheckInsidePlugin(DWORD CurFlags);
    BOOL CheckPanel(int PanelMode,DWORD CurFlags, BOOL IsPassivePanel);
    BOOL CheckCmdLine(int CmdLength,DWORD Flags);
    BOOL CheckFileFolder(Panel *ActivePanel,DWORD CurFlags, BOOL IsPassivePanel);
    BOOL CheckAll(int CheckMode,DWORD CurFlags);
    void Sort(void);
    int  IfCondition(DWORD Key,DWORD Flags,DWORD Code);
    DWORD GetOpCode(struct MacroRecord *MR,int PC);
    DWORD SetOpCode(struct MacroRecord *MR,int PC,DWORD OpCode);

  private:
    static long WINAPI AssignMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    static long WINAPI ParamMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);

  public:
    KeyMacro();
    ~KeyMacro();

  public:
    int ProcessKey(int Key);
    int GetKey();
    int PeekKey();

    int PushState();
    int PopState();
    int GetLevelState(){return CurPCStack;};

    int  IsRecording() {return(Recording);};
    int  IsExecuting() {return(Work.Executing);};
    int  IsExecutingLastKey();
    int  IsDsableOutput() {return CheckCurMacroFlags(MFLAGS_DISABLEOUTPUT);};
    void SetMode(int Mode) {KeyMacro::Mode=Mode;};
    int  GetMode() {return(Mode);};

    void RunStartMacro();

    // ��������� ��������� ��������� ������������� �������
    int PostNewMacro(char *PlainText,DWORD Flags=0);
    // ��������� ��������� ������ (�������� �������������)
    int PostNewMacro(struct MacroRecord *MRec,BOOL NeedAddSendFlag=0);

    int  LoadMacros(BOOL InitedRAM=TRUE);
    void SaveMacros(BOOL AllSaved=TRUE);

    int GetStartIndex(int Mode) {return IndexMode[Mode<MACRO_LAST?Mode:MACRO_LAST][0];}
    // ������� ��������� ������� ������� ������� � �������
    int GetIndex(int Key, int Mode);
    // ��������� �������, ����������� ��������� ��������
    int GetRecordSize(int Key, int Mode);

    char *GetPlainText(char *Dest);
    int   GetPlainTextSize();

    void SetRedrawEditor(int Sets){IsRedrawEditor=Sets;}

    // �������� ������ � ������� (���������� ������)
    int GetCurRecord(struct MacroRecord* RBuf=NULL,int *KeyPos=NULL);
    // ��������� ����� �������� ������������ �������.
    BOOL CheckCurMacroFlags(DWORD Flags);

    static char* GetSubKey(int Mode);
    static int   GetSubKey(char *Mode);
    static int   GetMacroKeyInfo(int Mode,int Pos,char *KeyName,char *Description,int DescriptionSize);
    static char *MkTextSequence(DWORD *Buffer,int BufferSize);
    // �� ���������� ������������� ������� ������� MacroRecord
    int ParseMacroString(struct MacroRecord *CurMacro,const char *BufPtr);
    void DropProcess();
};

#endif	// __KEYMACRO_HPP__
