#ifndef __KEYMACRO_HPP__
#define __KEYMACRO_HPP__
/*
macro.hpp

������

*/

/* Revision: 1.04 26.12.2000 $ */

/*
Modify:
  26.12.2000 SVS
    + SwitchFlags()
  23.12.2000 SVS
    + int ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr)
    + int PlayKeyMacro(struct MacroRecord *MRec)
    + int PlayKeyMacro(char *KeyBuffer)
  21.12.2000 SVS
    ! ������� MacroRecord ��७�ᥭ� �� struct.hpp � "ᦠ�"
    ! �㭪�� KeyToText 㤠���� �� �������������
  10.09.2000 SVS
    ! �㭪�� ReadMacros ����� �������⥫�� ��㬥���
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

struct MacroRecord
{
  DWORD Flags;
  int   Key;
  int   BufferSize;
  int  *Buffer;
};

class KeyMacro
{
  private:
    /* $ 10.09.2000 SVS
      ! �㭪�� ReadMacros ����� �������⥫�� ��㬥���
    */
    struct TKeyNames{
      char Name[32];
      int  Code;
    };

    class LockScreen *LockScr;

    struct MacroRecord *Macros;
    int MacrosNumber;
    int Recording;
    int *RecBuffer;
    int RecBufferSize;
    int Executing;
    int ExecMacroPos;
    int ExecKeyPos;
    int InternalInput;
    int Mode;
    int StartMode;
    int StartMacroPos;

    struct MacroRecord *TempMacro; // �६���� ���� ��� 1 ����
    int TempMacroType;             // ⨯ �⮣� ����

  private:
    int  ReadMacros(int ReadMode,struct TKeyNames *KeyNames,
                    int CountKeyNames, char *Buffer, int BufferSize);
    int GetMacroSettings(int &DisableOutput,int &RunAfterStart,
                         int &EmptyCommandLine,int &NotEmptyCommandLine,
                         int &FilePanels,int &PluginPanels);
    void InitVars();
    void ReleaseTempBuffer(); // 㤠���� �६���� ����

    // �� ��ப����� �।�⠢����� ����� ᤥ���� MacroRecord
    int ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr);
    DWORD SwitchFlags(DWORD& Flags,DWORD Value);

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

    // "�����" ��ப���� �।�⠢����� �����
    int PlayKeyMacro(char *KeyBuffer);
    // "�����" ४�� (����୮� �।�⠢�����)
    int PlayKeyMacro(struct MacroRecord *MRec);

    int  LoadMacros();
    void SaveMacros();

    static char* GetSubKey(int Mode);
    static int   GetSubKey(char *Mode);
};

#endif	// __KEYMACRO_HPP__
