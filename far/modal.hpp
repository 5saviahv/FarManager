#ifndef __MODAL_HPP__
#define __MODAL_HPP__
/*
modal.hpp

Parent class ��� ��������� ��������

*/

/* Revision: 1.08 25.04.2002 $ */

/*
Modify:
  25.04.2002 IS
    ! ��������� const
  26.07.2001 OT
    ������������� ����������� (������ ����������)
  18.07.2001 OT
    VFMenu
  14.06.2001 OT
    ! "����" ;-)
  06.05.2001 DJ
    ! �������� #include
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

#include "scrobj.hpp"

class Modal: virtual public ScreenObject
{
  private:
    int     ReadKey,
            WriteKey;
    typedef ScreenObject inherited;
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

    void SetHelp(const char *Topic);
    void ShowHelp();
//    void SetScreenPosition(){inherited::SetScreenPosition();}

};


#endif  //__MODAL_HPP__
