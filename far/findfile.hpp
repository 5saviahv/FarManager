#ifndef __FINDFILES_HPP__
#define __FINDFILES_HPP__
/*
findfile.hpp

���� (Alt-F7)

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

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
