#ifndef __CONSTITLE_HPP__
#define __CONSTITLE_HPP__
/*
constitle.hpp

��������� �������

*/

/* Revision: 1.02 01.04.2002 $ */

/*
Modify:
  01.04.2002 SVS
    ! ��� ���������
  14.05.2001 SVS
    + ������� ����������� - �� ��������� title=NULL
  20.03.2001 tran
    ! created
*/


class ConsoleTitle
{
  private:
    char OldTitle[512];

  public:
    ConsoleTitle(char *title=NULL);
    ~ConsoleTitle();

  public:
    void Set(char *fmt,...);

};

#endif
