#ifndef __SCANTREE_HPP__
#define __SCANTREE_HPP__
/*
scantree.hpp

������������ �������� �������� �, �����������, ������������ ��
������� ���� ������

*/

/* Revision: 1.05 27.12.2002 $ */

/*
Modify:
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

#define SF_FILES_FIRST 0x00000001   // ������������ ������� �� ��� �������. ������� �����, ����� ��������

class ScanTree
{
  private:
    HANDLE FindHandle[NM/2];
    int SecondPass[NM/2];
    int FindHandleCount;
    int RetUpDir;
    int Recurse;
    int SecondDirName;
    char FindPath[2*NM];
    char FindMask[NM];
    DWORD ScanFlags;

  private:
    void Init();

  public:
    ScanTree(int RetUpDir,int Recurse=1);
    ~ScanTree();

  public:
    void SetFindPath(const char *Path,const char *Mask, const DWORD NewScanFlags = SF_FILES_FIRST);
    int GetNextName(WIN32_FIND_DATA *fdata,char *FullName, size_t BufSize);
    void SkipDir();
    int IsDirSearchDone() {return(SecondDirName);};
};


#endif	// __SCANTREE_HPP__
