#ifndef __FILEPOSITIONCACHE_HPP__
#define __FILEPOSITIONCACHE_HPP__
/*
poscache.hpp

��� ������� � ������ ��� viewer/editor

*/

/* Revision: 1.03 17.06.2001 $ */

/*
Modify:
  17.06.2001 IS
    + include "udlist.hpp"
  02.04.2001 VVM
    + int FindPosition()
  24.09.2000 SVS
    + ������ �� ����������/�������������� ������� � ����� �� RCtrl+<N>
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "udlist.hpp"

#define MAX_POSITIONS 64

class FilePositionCache
{
  private:
    int IsMemory;
    char *Names;
    unsigned int *Positions;
    long *ShortPos;
    int CurPos;

    int FindPosition(char *Name);
  public:
    FilePositionCache();
   ~FilePositionCache();

  public:
    void AddPosition(char *Name,unsigned int Position1,unsigned int Position2,
                     unsigned int Position3,unsigned int Position4,
                     unsigned int Position5,
                     long *PosLine,long *PosCursor,long *PosScreenLine,
                     long *PosLeftPos);
    void GetPosition(char *Name,unsigned int &Position1,unsigned int &Position2,
                     unsigned int &Position3,unsigned int &Position4,
                     unsigned int &Position5,
                     long *PosLine,long *PosCursor,long *PosScreenLine,
                     long *PosLeftPos);
    void Read(char *Key);
    void Save(char *Key);
};


#endif	// __FILEPOSITIONCACHE_HPP__
