#ifndef __PRESERVELONGNAME_HPP__
#define __PRESERVELONGNAME_HPP__
/*
plognmn.hpp

class PreserveLongName

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class PreserveLongName
{
  private:
    char SaveLongName[NM],SaveShortName[NM];
    int Preserve;
  public:
    PreserveLongName(char *ShortName,int Preserve);
    ~PreserveLongName();
};



#endif	// __PRESERVELONGNAME_HPP__

