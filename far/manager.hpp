#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

+����������� ����� ����������� file panels, viewers, editors

*/

/* Revision: 1.05 06.05.2001 $ */

/*
Modify:
  06.05.2001 ++
    ! +������������� Window � Frame :)
  04.05.2001 DJ
    + ������� � ��������� NWZ
  29.04.2001 ++
    + +�������� NWZ �� +���������
  29.12.2000 IS
    + +���� ExitAll
  28.06.2000 tran
    - NT Console resize bug
      add class member ActiveModal
  25.06.2000 SVS
    ! +��������� Master Copy
    ! +�������� � ������� ���������������� ������
*/

class Manager
{
  private:

    void ActivateNextFrame();

    Frame **FrameList;
    Frame *DestroyedFrame;

    int  EndLoop;

    int  FrameCount,
         FrameListSize;
    int  FramePos;
    int  UpdateRequired;

    int  NextViewer;
    char NextName[NM];
    int  NextPos;

    INPUT_RECORD LastInputRecord;

    void SetCurrentFrame (Frame *NewCurFrame);

  public:

    Manager();
    ~Manager();

    void AddFrame(Frame *NewFrame);
    void DestroyFrame(Frame *Killed);
    int ExecuteModal(Frame &ModalFrame);

    void NextFrame(int Increment);
    void SelectFrame(); // show window menu (F12)

    void CloseAll();
    /* $ 29.12.2000 IS
         +����� CloseAll, �� ��������� ����������� ����������� ������ � ���,
         ���� ����������� ��������� ����������� ����.
         +������� TRUE, ���� �� ������� � ����� ������ �� ���.
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

    void SetNextFrame(int Viewer,char *Name,long Pos);

    // new methods
    void EnterMainLoop();
    void ProcessMainLoop();
    void ExitMainLoop(int Ask);
    int ProcessKey(int key);
    int ProcessMouse(MOUSE_EVENT_RECORD *me);

    void PluginsMenu(); // ������� ���� �� F11
    void CheckExited();

    Frame *CurrentFrame;  // ������� �����,
                          // ���������� � ������, �� ����� 񻲼 �� ��������
    int    EnableSwitch;  // ��������� �� ������������ �� ������

    INPUT_RECORD *GetLastInputRecord() { return &LastInputRecord; }
};

#endif  // __MANAGER_HPP__
