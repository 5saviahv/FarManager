#ifndef __GROUPSORT_HPP__
#define __GROUPSORT_HPP__
/*
grpsort.hpp

��㯯� ���஢��

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class GroupSort
{
  private:
    int EditGroupsMenu(int Pos);
    struct GroupSortData *GroupData;
    int GroupCount;
  public:
    GroupSort();
    ~GroupSort();
    int GetGroup(char *Path);
    void EditGroups();
};

#endif	// __GROUPSORT_HPP__
