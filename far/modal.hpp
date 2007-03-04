#ifndef __MODAL_HPP__
#define __MODAL_HPP__
/*
modal.hpp

Parent class ��� ��������� ��������

*/

#include "unicodestring.hpp"
#include "frame.hpp"

class Modal: public Frame
{
  private:
    int     ReadKey,
            WriteKey;
    typedef ScreenObject inherited;
  protected:
    INPUT_RECORD ReadRec;
    string strHelpTopic;
    int  ExitCode;
    int  EndLoop;

  public:
    Modal();
    virtual void GetDialogObjectsData() {};
    int Done();
    void ClearDone();
    int  GetExitCode();
    void SetExitCode(int Code);

    virtual void Process();

    int  ReadInput();
    void WriteInput(int Key);
    void ProcessInput();

    void SetHelp(const wchar_t *Topic);
    void ShowHelp();
//    void SetScreenPosition(){inherited::SetScreenPosition();}

};


#endif  //__MODAL_HPP__
