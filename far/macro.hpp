#ifndef __KEYMACRO_HPP__
#define __KEYMACRO_HPP__
/*
macro.hpp

������

*/

/* Revision: 1.01 10.09.2000 $ */

/*
Modify:
  10.09.2000 SVS
    ! �㭪�� ReadMacros ����� �������⥫�� ��㬥���
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

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

    void ReadMacros(int ReadMode,struct TKeyNames *KeyNames,
                    int CountKeyNames, char *Buffer, int BufferSize);
    /* SVS $ */
    void KeyToText(int Key,char *KeyName);
    int GetMacroSettings(int &DisableOutput,int &RunAfterStart,
                         int &EmptyCommandLine,int &NotEmptyCommandLine);

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
  public:
    KeyMacro();
    ~KeyMacro();
    int ProcessKey(int Key);
    int GetKey();
    int PeekKey();
    int IsRecording() {return(Recording);};
    int IsExecuting() {return(Executing);};
    void SaveMacros();
    void SetMode(int Mode) {KeyMacro::Mode=Mode;};
    int GetMode() {return(Mode);};
    void RunStartMacro();
};

#endif	// __KEYMACRO_HPP__
