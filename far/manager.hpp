#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

������������ ����� ����������� file panels, viewers, editors

*/

/* Revision: 1.03 29.04.2001 $ */

/*
Modify:
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

    Modal **ModalList,
          **ActiveList;

    int  ModalCount,
         ModalSizeList,
         ActiveListCount;
    int  ModalPos;
    int  NextViewer;
    char NextName[NM];
    int  NextPos;
    int  UpdateRequired;

  public:

    Manager();
    ~Manager();

    void AddModal(Modal *NewModal);
    void ExecuteModal(Modal *ExecModal);
    void DestroyModal(Modal *KilledModal);

    void NextModal(int Increment);
    void SelectModal(); // show window menu (F12)

    void PushActive();
    void PopActive();

    void CloseAll();
    /* $ 29.12.2000 IS
         ������ CloseAll, �� ��������� ����������� ����������� ������ � ����,
         ���� ������������ ��������� ������������� ����.
         ���������� TRUE, ���� ��� ������� � ����� �������� �� ����.
    */
    BOOL ExitAll();
    /* IS $ */
    BOOL IsAnyModalModified(int Activate);

    int  GetModalCount() {return(ModalCount);};
    void GetModalTypesCount(int &Viewers,int &Editors);
    int  GetModalCountByType(int Type);

    BOOL IsPanelsActive(); // ������������ ��� ������� WaitInMainLoop

    void SetModalPos(int NewPos);

    int  FindModalByFile(int ModalType,char *FileName);

    void ShowBackground();

    void SetNextWindow(int Viewer,char *Name,long Pos);

    // new methods
    int ProcessKey(int key);
    int ProcessMouse(MOUSE_EVENT_RECORD *me);

    void PluginsMenu(); // �������� ���� �� F11
    void CheckExited();

    Modal *ActiveModal;   // �������� �����, � ��� ����� � ������
                          // ����� �� ���� � ������
    Modal *CurrentModal;  // ������� �����,
                          // ����������� � ������, �� ����� ���� �� ��������
    int    EnableSwitch;  // ��������� �� ������������ �� ������
};

#endif  // __MANAGER_HPP__
