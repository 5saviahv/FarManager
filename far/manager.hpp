#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

��४��祭�� ����� ��᪮�쪨�� file panels, viewers, editors

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Manager
{
  private:
    void ActivateNextWindow();

    Modal **ModalList;
    int ModalCount;
    int ModalPos;
    int NextViewer;
    char NextName[NM];
    int NextPos;
    int UpdateRequired;
  public:
    Manager();
    void AddModal(Modal *NewModal);
    void NextModal(int Increment);
    void CloseAll();
    BOOL IsAnyModalModified(int Activate);
    void SelectModal();
    int GetModalCount() {return(ModalCount);};
    void GetModalTypesCount(int &Viewers,int &Editors);
    void SetModalPos(int NewPos);
    int FindModalByFile(int ModalType,char *FileName);
    void ShowBackground();
    void SetNextWindow(int Viewer,char *Name,long Pos);
};

#endif	// __MANAGER_HPP__

