#ifndef __GRABBER_HPP__
#define __GRABBER_HPP__
/*
grabber.hpp

Screen grabber

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Grabber:Modal
{
  private:
    void DisplayObject();
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void CopyGrabbedArea(int Append);

    SaveScreen *SaveScr;
    struct GrabberArea PrevArea,GArea;
    int ResetArea;
    int PrevMacroMode;
  public:
    Grabber();
    ~Grabber();
};


#endif	// __GRABBER_HPP__

