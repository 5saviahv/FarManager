#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

��४��祭�� ����� ��᪮�쪨�� file panels, viewers, editors

*/

/* Revision: 1.02 29.12.2000 $ */

/*
Modify:
  29.12.2000 IS
    + ��⮤ ExitAll
  28.06.2000 tran
    - NT Console resize bug
      add class member ActiveModal
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
    /* $ 29.12.2000 IS
         ������ CloseAll, �� ࠧ�蠥� �த������� �����業��� ࠡ��� � ��,
         �᫨ ���짮��⥫� �த����� ।���஢��� 䠩�.
         �����頥� TRUE, �᫨ �� ����뫨 � ����� ��室��� �� ��.
    */
    BOOL ExitAll();
    /* IS $ */
    BOOL IsAnyModalModified(int Activate);
    void SelectModal();
    int GetModalCount() {return(ModalCount);};
    void GetModalTypesCount(int &Viewers,int &Editors);
    void SetModalPos(int NewPos);
    int FindModalByFile(int ModalType,char *FileName);
    void ShowBackground();
    void SetNextWindow(int Viewer,char *Name,long Pos);
    /* $ 28.06.2000 tran
       add class memer ActiveModal */
    Modal *ActiveModal;
    /* tran $ */
};

#endif  // __MANAGER_HPP__
