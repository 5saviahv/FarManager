#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

������������ ����� ����������� file panels, viewers, editors

*/

/* Revision: 1.13 21.05.2001 $ */

/*
Modify:
  21.05.2001 OT
    + ��������� RefreshedFrame
  21.05.2001 DJ
    ! ������ �������������; � ����� � ���������� ������ ���� �������
      �������� GetFrameTypesCount(); ���������� �������� ������
  16.05.2001 DJ
    ! ����������� ExecuteModal()
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 OT
    ! ��������� ������� ������ ���������� ReplaceFrame (��� ������������ � ��������)
  12.05.2001 DJ
    ! FrameManager ������� �� CtrlObject, ������� ExecuteModalPtr,
      ReplaceCurrentFrame ������� �� ReplaceFrame, GetCurrentFrame()
  10.05.2001 DJ
    + SwitchToPanels(), ModalStack, ModalSaveState(), ExecuteModalPtr()
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

class Manager
{
  private:
    Frame **FrameList;
    /* $ 16.05.2001 DJ */
    Frame **ModalStack;
    int ModalStackSize, ModalStackCount;
    /* DJ $ */

    /*$ ����������� �� ... */
    Frame *InsertedFrame;
    Frame *DeletedFrame;   // DestroyedFrame
    Frame *ActivatedFrame; // ���������� �� ��, ����� �����
//    Frame *UpdatedFrame;   // FrameToReplace
//    Frame *UpdatingFrame;  //
    Frame *RefreshedFrame;
    Frame *ModalizedFrame;
    Frame *DeactivatedFrame;
    /* OT $*/
    /* $ 21.05.2001 DJ */
    Frame *FrameToDestruct;  // ���������� �������� ��� ���������� ������� OnChangeFocus(0)
    /* DJ $ */
      
    Frame *CurrentFrame;     // ������� �����

    int DisableDelete;

    int  EndLoop;
    int  FrameCount,
         FrameListSize;
    int  FramePos;

    INPUT_RECORD LastInputRecord;

    void ModalSaveState();

  private:

    void StartupMainloop();
    void FrameMenu(); //    ������ void SelectFrame(); // show window menu (F12)

    // ���������� ���������
    BOOL Commit();
    void RefreshCommit();
    void ActivateCommit();
    void UpdateCommit();
    void InsertCommit();
    void DeleteCommit();
    /* $ 21.05.2001 DJ */
    void DestructCommit();
    /* DJ $ */

  public:
    Manager();
    ~Manager();

  public:
    // ��� ������� ���������� �� �������� ����
    void InsertFrame(Frame *NewFrame, int Index=-1);
    void DeleteFrame(Frame *Deleted=NULL);
    void DeleteFrame(int Index);
    void ModalizeFrame (Frame *Modalized=NULL, int Mode=TRUE); // ������ ExecuteModal
    void DeactivateFrame (Frame *Deactivated,int Direction);
    void ActivateFrame (Frame *Activated);
    void ActivateFrame (int Index);  //������ ActivateFrameByPos (int NewPos);
    void RefreshFrame(Frame *Refreshed);
    void RefreshFrame(int Index);


    int ExecuteModal (Frame &ModalFrame);

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
    int  GetFrameCountByType(int Type);

    BOOL IsPanelsActive(); // ������������ ��� ������� WaitInMainLoop
    void SetFramePos(int NewPos);
    int  FindFrameByFile(int ModalType,char *FileName,char *Dir=NULL);
    void ShowBackground();

    // new methods
    void EnterMainLoop();
    void ProcessMainLoop();
    void ExitMainLoop(int Ask);
    int ProcessKey(int key);
    int ProcessMouse(MOUSE_EVENT_RECORD *me);

    void PluginsMenu(); // �������� ���� �� F11
    /* $ 10.05.2001 DJ */
    void SwitchToPanels();
    /* DJ $ */

    INPUT_RECORD *GetLastInputRecord() { return &LastInputRecord; }

    /* $ 12.05.2001 DJ */
    Frame *GetCurrentFrame() { return CurrentFrame; }
    /* DJ $ */

    /*$ 13.05.2001 OT */
    Frame *operator[](int Index);

    /* $ 19.05.2001 DJ
       operator[] (Frame *) -> IndexOf
    */
    int IndexOf(Frame *Frame);
    /* DJ $ */
};

extern Manager *FrameManager;

#endif  // __MANAGER_HPP__
