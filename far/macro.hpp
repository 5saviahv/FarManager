#ifndef __KEYMACRO_HPP__
#define __KEYMACRO_HPP__
/*
macro.hpp

������

*/

/* Revision: 1.09 25.03.2001 $ */

/*
Modify:
  25.04.2001 SVS
    ! ��� �஢�ન 䫠��� ��� ���� ����ᮢ �뭥ᥭ � �㭪樨 Check* -
      ᫨誮� ����� �������饣��� ���� :-(
  08.03.2001 SVS
    + �㭪�� MkTextSequence - �ନ஢���� ��ப����� �।�⠢����� Sequence
  22.01.2001 SVS
    + �㭪�� MkRegKeyName - �ନ஢���� ����� ���� � ॥���.
  17.01.2001 SVS
    + �㭪樨 ����祭�� ������ ����� � ࠧ��� ��� ���ய�᫥����⥫쭮���:
       int GetIndex(int Key, int Mode);
       int GetRecordSize(int Key, int Mode);
  04.01.2001 SVS
    ! ������� ReadMacros � GetMacroSettings
    + �㭪�� AssignMacroKey
    ! 㤠���� ������� struct TKeyNames
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
  DWORD *Buffer;
};

class KeyMacro
{
  private:
    /* $ 10.09.2000 SVS
      ! �㭪�� ReadMacros ����� �������⥫�� ��㬥���
    */
    class LockScreen *LockScr;

    struct MacroRecord *Macros;
    int MacrosNumber;
    // ⨯ ����� - � �맮��� ������� ����஥� ���...
    // 0 - ��� �����, 1 - ����� ������, 2 - �맮� ������� ����஥�
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
    int ReadMacros(int ReadMode, char *Buffer, int BufferSize);
    DWORD AssignMacroKey();
    int GetMacroSettings(int Key,DWORD &Flags);
    void InitVars();
    void ReleaseTempBuffer(); // 㤠���� �६���� ����

    // �� ��ப����� �।�⠢����� ����� ᤥ���� MacroRecord
    int ParseMacroString(struct MacroRecord *CurMacro,char *BufPtr);
    DWORD SwitchFlags(DWORD& Flags,DWORD Value);
    static long WINAPI AssignMacroDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    char *MkRegKeyName(int IdxMacro,char *RegKeyName);

    BOOL CheckEditSelected(DWORD CurFlags);
    BOOL CheckPanel(int PanelMode,DWORD CurFlags);
    BOOL CheckCmdLine(int CmdLength,DWORD Flags);
    BOOL CheckFileFolder(Panel *ActivePanel,DWORD CurFlags);
    BOOL CheckAll(DWORD CurFlags);

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

    // �㭪�� ����祭�� ������ �㦭��� ����� � ���ᨢ�
    int GetIndex(int Key, int Mode);
    // ����祭�� ࠧ���, ����������� 㪠����� ����ᮬ
    int GetRecordSize(int Key, int Mode);

    static char* GetSubKey(int Mode);
    static int   GetSubKey(char *Mode);
    static char *MkTextSequence(DWORD *Buffer,int BufferSize);
};

#endif	// __KEYMACRO_HPP__
