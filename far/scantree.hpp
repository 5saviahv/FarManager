#ifndef __SCANTREE_HPP__
#define __SCANTREE_HPP__
/*
scantree.hpp

������������ �������� �������� �, �����������, ������������ ��
������� ���� ������

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

class ScanTree
{
  private:
    void Init();

    HANDLE FindHandle[NM/2];
    int SecondPass[NM/2];
    int FindHandleCount;
    int RetUpDir;
    int Recurse;
    int SecondDirName;
    char FindPath[2*NM];
    char FindMask[NM];
  public:
    ScanTree(int RetUpDir,int Recurse=1);
    ScanTree::~ScanTree();
    void SetFindPath(char *Path,char *Mask);
    int GetNextName(WIN32_FIND_DATA *fdata,char *FullName);
    void SkipDir();
    int IsDirSearchDone() {return(SecondDirName);};
};


#endif	// __SCANTREE_HPP__
