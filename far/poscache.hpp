#ifndef __FILEPOSITIONCACHE_HPP__
#define __FILEPOSITIONCACHE_HPP__
/*
poscache.hpp

��� ����権 � 䠩��� ��� viewer/editor

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#define MAX_POSITIONS 64

class FilePositionCache
{
  private:
    char Names[MAX_POSITIONS][3*NM];
    unsigned int Positions1[MAX_POSITIONS],Positions2[MAX_POSITIONS];
    unsigned int Positions3[MAX_POSITIONS],Positions4[MAX_POSITIONS];
    unsigned int Positions5[MAX_POSITIONS];
    int CurPos;
  public:
    FilePositionCache();
    void AddPosition(char *Name,unsigned int Position1,unsigned int Position2,
                     unsigned int Position3,unsigned int Position4,
                     unsigned int Position5);
    void GetPosition(char *Name,unsigned int &Position1,unsigned int &Position2,
                     unsigned int &Position3,unsigned int &Position4,
                     unsigned int &Position5);
    void Read(char *Key);
    void Save(char *Key);
};


#endif	// __FILEPOSITIONCACHE_HPP__

