#ifndef __FINDFILES_HPP__
#define __FINDFILES_HPP__
/*
findfile.hpp

����� (Alt-F7)

*/

/* Revision: 1.01 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#undef SEARCH_ALL
enum {SEARCH_ALL=0,SEARCH_ROOT,SEARCH_FROM_CURRENT,SEARCH_CURRENT_ONLY,
  SEARCH_SELECTED};

class SaveScreen;

class FindFiles
{
  private:
    int FindFilesProcess();
    void SetPluginDirectory(char *FileName);
    SaveScreen *FindSaveScr;
  public:
    FindFiles();
    ~FindFiles();
};


#endif	// __FINDFILES_HPP__
