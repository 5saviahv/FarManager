#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

������������ ����� ����������� file panels, viewers, editors

*/

/* Revision: 1.04 04.05.2001 $ */

/*
Modify:
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

class Manager
{
  private:

    void ActivateNextWindow();

    Window **WindowList;
    Window *DestroyedWindow;

    int  EndLoop;

    int  WindowCount,
         WindowListSize;
    int  WindowPos;
    int  UpdateRequired;

    int  NextViewer;
    char NextName[NM];
    int  NextPos;

    INPUT_RECORD LastInputRecord;

    void SetCurrentWindow (Window *NewCurWindow);

  public:

    Manager();
    ~Manager();

    void AddWindow(Window *NewWindow);
    void DestroyWindow(Window *Killed);
    int ExecuteModal(Window &ModalWindow);

    void NextWindow(int Increment);
    void SelectWindow(); // show window menu (F12)

    void CloseAll();
    /* $ 29.12.2000 IS
         ������ CloseAll, �� ��������� ����������� ����������� ������ � ����,
         ���� ������������ ��������� ������������� ����.
         ���������� TRUE, ���� ��� ������� � ����� �������� �� ����.
    */
    BOOL ExitAll();
    /* IS $ */
    BOOL IsAnyWindowModified(int Activate);

    int  GetWindowCount() {return(WindowCount);};
    void GetWindowTypesCount(int &Viewers,int &Editors);
    int  GetWindowCountByType(int Type);

    BOOL IsPanelsActive(); // ������������ ��� ������� WaitInMainLoop

    void SetWindowPos(int NewPos);

    int  FindWindowByFile(int ModalType,char *FileName);

    void ShowBackground();

    void SetNextWindow(int Viewer,char *Name,long Pos);

    // new methods
    void EnterMainLoop();
    void ProcessMainLoop();
    void ExitMainLoop(int Ask);
    int ProcessKey(int key);
    int ProcessMouse(MOUSE_EVENT_RECORD *me);

    void PluginsMenu(); // �������� ���� �� F11
    void CheckExited();

    Window *CurrentWindow;  // ������� �����,
                          // ����������� � ������, �� ����� ���� �� ��������
    int    EnableSwitch;  // ��������� �� ������������ �� ������

    INPUT_RECORD *GetLastInputRecord() { return &LastInputRecord; }
};

#endif  // __MANAGER_HPP__
