#ifndef __GROUPSORT_HPP__
#define __GROUPSORT_HPP__
/*
grpsort.hpp

������ ����������

*/

/* Revision: 1.01 23.04.2001 $ */

/*
Modify:
  23.04.2001 SVS
    ! ���! ����� ��<��� �� %PATHEXT% - �� ��� ����������� � ��, ���
      ����� - ������ ��������.
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

class GroupSort
{
  private:
    struct GroupSortData *GroupData;
    int GroupCount;

  private:
    int EditGroupsMenu(int Pos);
    char *GetMask(int Idx);
    BOOL AddMask(struct GroupSortData *Dest,char *Mask,int Group);
    void DeleteMask(struct GroupSortData *CurGroupData);

  public:
    GroupSort();
    ~GroupSort();

  public:
    int GetGroup(char *Path);
    void EditGroups();
};

#endif	// __GROUPSORT_HPP__
