#ifndef __SCANTREE_HPP__
#define __SCANTREE_HPP__
/*
scantree.hpp

������������ �������� �������� �, �����������, ������������ ��
������� ���� ������

*/

/* Revision: 1.06 01.06.2003 $ */

/*
Modify:
  01.06.2003 SVS
    ! ��������� �� BitFlags
  27.12.2002 VVM
    + ����� �������� ScanFlags. ������ �����. ���� ��� ������ ���� SF_FILES_FIRST.
  23.06.2002 SVS
    ! ������� ������� ;-)
  26.03.2002 DJ
    ! GetNextName() ��������� ������ ������ ��� ����� �����
  25.06.2001 IS
    ! ��������� const
  06.05.2001 DJ
    ! �������� #include
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "farconst.hpp"
#include "bitflags.hpp"

enum{
  FSCANTREE_RETUPDIR      = 0x00000001, // = FRS_RETUPDIR
  FSCANTREE_RECUR         = 0x00000002, // = FRS_RECUR
  FSCANTREE_SCANJUNCTION  = 0x00000004, // = FRS_SCANJUNCTION
  FSCANTREE_SECONDDIRNAME = 0x00000080,

  FSCANTREE_FILESFIRST    = 0x00010000, // ������������ ������� �� ��� �������. ������� �����, ����� ��������
};

class ScanTree
{
  private:
    BitFlags Flags;
    HANDLE FindHandle[NM/2];
    int SecondPass[NM/2];
    int FindHandleCount;
    char FindPath[4*NM];
    char FindMask[NM];

  private:
    void Init();

  public:
    ScanTree(int RetUpDir,int Recurse=1,int ScanJunction=-1);
    ~ScanTree();

  public:
    // 3-� �������� - ����� �� �������� �����
    void SetFindPath(const char *Path,const char *Mask, const DWORD NewScanFlags = FSCANTREE_FILESFIRST);
    int GetNextName(WIN32_FIND_DATA *fdata,char *FullName, size_t BufSize);
    void SkipDir();
    int IsDirSearchDone() {return Flags.Check(FSCANTREE_SECONDDIRNAME);};
};


#endif	// __SCANTREE_HPP__
