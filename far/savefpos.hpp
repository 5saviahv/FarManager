#ifndef __SAVEFILEPOS_HPP__
#define __SAVEFILEPOS_HPP__
/*
savefpos.hpp

class SaveFilePos

*/

/* Revision: 1.01 08.08.2000 $ */

/*
Modify:
  08.08.2000 tran 1.01
    ! long -> int64
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class SaveFilePos
{
  private:
    FILE *SaveFile;
    int64 SavePos;    
  public:
    SaveFilePos(FILE *SaveFile);
    ~SaveFilePos();
};


#endif  // __SAVEFILEPOS_HPP__

