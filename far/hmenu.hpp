#ifndef __HMENU_HPP__
#define __HMENU_HPP__
/*
hmenu.hpp

��ਧ��⠫쭮� ����

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class HMenu:public Modal
{
  private:
    void DisplayObject();
    void ShowMenu();
    void ProcessSubMenu(struct MenuData *Data,int DataCount,char *SubMenuHelp,
                        int X,int Y,int &Position);
    VMenu *SubMenu;
    struct HMenuData *Item;
    int SelectPos;
    int ItemCount;
    int VExitCode;
    int ItemX[16];
  public:
    HMenu(struct HMenuData *Item,int ItemCount);
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void GetExitCode(int &ExitCode,int &VExitCode);
};


#endif	// __HMENU_HPP__
