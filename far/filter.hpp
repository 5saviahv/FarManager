#ifndef __PANELFILTER_HPP__
#define __PANELFILTER_HPP__
/*
filter.hpp

������ (Ctrl-I)

*/

/* Revision: 1.02 01.06.2001 $ */

/*
Modify:
  01.07.2001 IS
    + #include "CFileMask.hpp"
    ! ��������� const
  06.05.2001 DJ
    ! �������� #include
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "CFileMask.hpp"

class VMenu;
class Panel;

class PanelFilter
{
  private:
    void SaveFilterFile();
    int SaveFilterData();
    int EditRecord(char *Title,char *Masks);
    int ShowFilterMenu(int Pos,int FirstCall,int *NeedUpdate);
    void AddMasks(const char *Masks,int Exclude);
    void ProcessSelection(VMenu *FilterList);
    void SaveFilters();
    Panel *HostPanel;
    /* $ 01.07.2001 IS
       ������� ������ ��� ������ � �������
    */
    CFileMask IncludeMask, ExcludeMask;
    char *IncludeMaskStr, *ExcludeMaskStr;
    bool IncludeMaskIsOK, ExcludeMaskIsOK;
    /* IS $ */
  public:
    PanelFilter(Panel *HostPanel);
    ~PanelFilter();
    static void InitFilter();
    static void CloseFilter();
    static void SwapFilter();
    static void SaveSelection();
    void FilterEdit();
    int CheckName(const char *Name);
    bool IsEnabled();
};
#endif //__PANELFILTER_HPP__
