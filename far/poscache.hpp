#ifndef __FILEPOSITIONCACHE_HPP__
#define __FILEPOSITIONCACHE_HPP__
/*
poscache.hpp

��� ����権 � 䠩��� ��� viewer/editor

*/

/* Revision: 1.01 24.09.2000 $ */

/*
Modify:
  24.09.2000 SVS
    + ����� �� ��࠭����/����⠭������� ����権 � 䠩�� �� RCtrl+<N>
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#define MAX_POSITIONS 64

class FilePositionCache
{
  private:
    int IsMemory;
    char *Names;
    unsigned int *Positions;
    long *ShortPos;
    int CurPos;

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

