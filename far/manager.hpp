#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

������������ ����� ����������� file panels, viewers, editors

*/

/* Revision: 1.06 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
    + ReplaceCurrentFrame(), ActivateFrameByPos()
  06.05.2001 ��
    ! �������������� Window � Frame :)
  04.05.2001 DJ
    + ������� � ��������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  29.12.2000 IS
    + ����� ExitAll
  28.06.2000 tran
    - NT Console resize bug
      add class member ActiveModal
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

class Frame;

#include "farconst.hpp"

class Manager
{
  private:
    Frame **FrameList;
    Frame *DestroyedFrame;
    Frame *FrameToReplace;

    int  EndLoop;

    int  FrameCount,
         FrameListSize;
    int  FramePos;
    int  UpdateRequired;

    INPUT_RECORD LastInputRecord;

  public:
    Frame *CurrentFrame;  // ������� �����,
                          // ����������� � ������, �� ����� ���� �� ��������
    int    EnableSwitch;  // ��������� �� ������������ �� ������

  private:
    void ActivateNextFrame();

    void SetCurrentFrame (Frame *NewCurFrame);
    void SelectFrame(); // show window menu (F12)

  public:
    Manager();
    ~Manager();

  public:
    void AddFrame(Frame *NewFrame);
    void DestroyFrame(Frame *Killed);
    void ReplaceCurrentFrame (Frame *NewFrame);
    int ExecuteModal(Frame &ModalFrame);

    void NextFrame(int Increment);
    void ActivateFrameByPos (int NewPos);

    void CloseAll();
    /* $ 29.12.2000 IS
         ������ CloseAll, �� ��������� ����������� ����������� ������ � ����,
         ���� ������������ ��������� ������������� ����.
         ���������� TRUE, ���� ��� ������� � ����� �������� �� ����.
    */
    BOOL ExitAll();
    /* IS $ */
    BOOL IsAnyFrameModified(int Activate);

    int  GetFrameCount() {return(FrameCount);};
    void GetFrameTypesCount(int &Viewers,int &Editors);
    int  GetFrameCountByType(int Type);

    BOOL IsPanelsActive(); // ������������ ��� ������� WaitInMainLoop

    void SetFramePos(int NewPos);

    int  FindFrameByFile(int ModalType,char *FileName);

    void ShowBackground();

    // new methods
    void EnterMainLoop();
    void ProcessMainLoop();
    void ExitMainLoop(int Ask);
    int ProcessKey(int key);
    int ProcessMouse(MOUSE_EVENT_RECORD *me);

    void PluginsMenu(); // �������� ���� �� F11
    void CheckExited();

    INPUT_RECORD *GetLastInputRecord() { return &LastInputRecord; }
};

#endif  // __MANAGER_HPP__
