#ifndef __MODAL_HPP__
#define __MODAL_HPP__
/*
modal.hpp

Parent class ��� ��������� ��������

*/

/* Revision: 1.03 05.05.2001 $ */

/*
Modify:
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  29.06.2000 tran
    - (NT Console resize bug)
      adding virtual method SetScreenPosition
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

class Modal:public ScreenObject
{
  private:
    int     ReadKey,
            WriteKey;

  protected:
    INPUT_RECORD ReadRec;
    char HelpTopic[512];
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

    void SetHelp(char *Topic);
    void ShowHelp();

    /* $ 28.06.2000 tran
       (NT Console resize bug) adding virtual method */
    virtual void SetScreenPosition();
    /* tran $ */
};


#endif  //__MODAL_HPP__
