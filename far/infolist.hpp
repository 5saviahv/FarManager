#ifndef __INFOLIST_HPP__
#define __INFOLIST_HPP__
/*
infolist.hpp

���ଠ樮���� ������

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class InfoList:public Panel
{
  private:
    void DisplayObject();
    void ShowDirDescription();
    void ShowPluginDescription();
    void PrintText(char *Str);
    void PrintText(int MsgID);
    void PrintInfo(char *Str);
    void PrintInfo(int MsgID);
    char DizFileName[NM];
  public:
    InfoList();
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void Update(int Mode) {Redraw();};
};



#endif	// __INFOLIST_HPP__

