#ifndef __CHANGEPRIORITY_HPP__
#define __CHANGEPRIORITY_HPP__
/*
chgprior.hpp

class ChangePriority

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class ChangePriority
{
  private:
    int SavePriority;
  public:
    ChangePriority(int NewPriority);
    ~ChangePriority();
};

#endif	// __CHANGEPRIORITY_HPP__
