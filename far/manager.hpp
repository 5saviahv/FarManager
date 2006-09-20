#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

������������ ����� ����������� file panels, viewers, editors

*/

/* Revision: 1.38 20.09.2006 $ */

/*
Modify:
  20.09.2006 SVS
    + Manager::SwapTwoFrame()
  25.10.2005 SVS
    ! �������� � ProcessKey �� int, � DWORD
  02.02.2005 SVS
    ! ShowBackground() ������ ���������� TRUE/FALSE
  11.11.2004 SVS
    + Manager::GetTopModal() - ���������� top-����� ��� ��� �����, ���� � ������ ���� �������
  10.12.2002 SVS
    + ManagerClass_Dump() - ���� ������!
  26.09.2002 SVS
    + ResizeAllFrame()
  18.06.2002 SVS
    + � ������� ��������� ���������� StartManager, ���������� �� ������
      "������� ��� ���������?"
  22.05.2002 SKV
    + SemiModalBackFrames ������� �� �������������
  15.05.2002 SKV
    + ������� �������.
    + ������ SemiModalBackFrame'��
  15.05.2002 SVS
    ! ������� ����������� ����� Frame::InitKeyBar � ����� ��� ��������
      ��� ���� Frame � ������ Manager::InitKeyBar.
  13.04.2002 KM
    ! ResizeAllModal - ��� ������ ResizeConsole ��� ����
      NextModal � ���������� ������.
  07.04.2002 KM
    ! RedraFramesInProcess -> IsRedrawFramesInProcess
      ��������� � global.cpp
  04.04.2002 KM
    + RedrawFramesInProcess - ������� �����������
      ���� �������. ��������� ��� ��������������
      ������������� ��������� ������� ��� ����
      ���������������� �������, ����� ��������.
  25.03.2002 SVS
    + ManagerIsDown()
  21.02.2002 SVS
    + ResetLastInputRecord() - ���������� ����� ���������� �������.
  11.10.2001 IS
    + CountFramesWithName
  04.10.2001 OT
    ������ ������������ ������ � ��������� ������
  19.07.2001 OT
    ���������� ����� ����� � ������ ���� Unmodalize��� + ���� ������������
  18.07.2001 OT
    VFMenu
  11.07.2001 OT
    ������� CtrlAltShift � Manager
  26.06.2001 SKV
    + PluginCommit(); (ACTL_COMMIT)
  06.06.2001 OT
    - ���������� �����-�� � ExecuteFrame()...
  28.05.2001 OT
    ! RefreshFrame() �� ��������� ��������� ������� �����
  26.05.2001 OT
    + ����� ������ ExecuteComit(), ExecuteFrame(), IndexOfStack()
    + ����� ���� Frame *ExecutedFrame�
    - ��������� ������ � ����� ���� Destruct
    - �������� ������ ModalSaveState()
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
#if defined(SYSLOG)
    friend void ManagerClass_Dump(char *Title,const Manager *m,FILE *fp);
#endif
  private:
    Frame **ModalStack;     // ���� ��������� �������
    int ModalStackCount;    // ������ ����� ��������� �������
    int ModalStackSize;     // ����� ����� ��������� �������

    Frame **FrameList;       // ������� ��������� �������
    int  FrameCount;         // ������ ����������� �������
    int  FrameListSize;      // ������ ������ ��� ����������� �������
    int  FramePos;           // ������ ������� ������������ ������. �� �� ������ ��������� � CurrentFrame
                             // ������� ����������� ����� ����� �������� � ������� FrameManager->GetBottomFrame();

    /*$ ����������� �� ... */
    Frame *InsertedFrame;   // �����, ������� ����� �������� � ����� ����������� �������
    Frame *DeletedFrame;    // �����, �������������� ��� �������� �� ��������� �������, �� ���������� �����, ���� ��������� (�������� ��� �� ���, �� ���)
    Frame *ActivatedFrame;  // �����, ������� ���������� ������������ ����� ����� ������ ���������
    Frame *RefreshedFrame;  // �����, ������� ����� ������ ��������, �.�. ������������
    Frame *ModalizedFrame;  // �����, ������� ���������� � "�������" � �������� ������������ ������
    Frame *UnmodalizedFrame;// �����, ������������ �� "�������" ������������ ������
    Frame *DeactivatedFrame;// �����, ������� ��������� �� ���������� �������� �����
    Frame *ExecutedFrame;   // �����, �������� ��������� ����� ����� ��������� �� ������� ���������� ������

    Frame *CurrentFrame;     // ������� �����. �� ����� ����������� ��� � ����������� �������, ��� � � ��������� �����
                             // ��� ����� �������� � ������� FrameManager->GetCurrentFrame();
    /* $ 15.05.2002 SKV
      ������ ��� ������.
    */
    /*Frame **SemiModalBackFrames;
    int SemiModalBackFramesCount;
    int SemiModalBackFramesSize;*/
    /* SKV $ */

    /* $ 15.05.2002 SKV
      ��� ��� ���� ����������, ��� � �� ���� ��������,
      ������ ������� ��������� editor/viewer'��.
      ĸ����� ���  ���� ������� ����� ������� ExecuteModal.
      � ��������� ������, ��� ��� ExecuteModal ����������
      1) �� ������ ��� ��������� ������� (��� ��� �� �������������),
      2) �� ������ ��� editor/viewer'��.
    */
    int ModalEVCount;
    /* SKV $ */

    int  EndLoop;            // ������� ������ �� �����
    int  StartManager;
    INPUT_RECORD LastInputRecord;

    int ModalExitCode;

  private:
    void StartupMainloop();
    Frame *FrameMenu(); //    ������ void SelectFrame(); // show window menu (F12)

    BOOL Commit();         // ��������� ���������� �� ���������� � ������� � ����� �������
                           // ��� � ����� �������� ����, ���� ������ ���� �� ���������� ������� �� NULL
    // �������, "����������� ����������" - Commit'a
    // ������ ���������� �� ������ �� ���� � �� ������ ����
    void RefreshCommit();  //
    void DeactivateCommit(); //
    void ActivateCommit(); //
    void UpdateCommit();   // ����������� �����, ����� ����� �������� ���� ����� �� ������
    void InsertCommit();
    void DeleteCommit();
    void ExecuteCommit();
    void ModalizeCommit();
    void UnmodalizeCommit();

    int GetModalExitCode();

    /*void AddSemiModalBackFrame(Frame* frame);
    BOOL IsSemiModalBackFrame(Frame *frame);
    void RemoveSemiModalBackFrame(Frame* frame);*/

  public:
    Manager();
    ~Manager();

  public:
    // ��� ������� ����� ��������� �������� ����������� �� ������ ����� ����
    // ��� ��� �� ����������� ���������� � ���, ��� ����� ����� ������� � �������� ��� ��������� ������ Commit()
    void InsertFrame(Frame *NewFrame, int Index=-1);
    void DeleteFrame(Frame *Deleted=NULL);
    void DeleteFrame(int Index);
    void DeactivateFrame (Frame *Deactivated,int Direction);
    void SwapTwoFrame (int Direction);
    void ActivateFrame (Frame *Activated);
    void ActivateFrame (int Index);
    void RefreshFrame(Frame *Refreshed=NULL);
    void RefreshFrame(int Index);

    //! ������� ��� ������� ��������� �������.
    void ExecuteFrame(Frame *Executed);


    //! ������ � ����� ���� ��������� �������
    void ExecuteModal (Frame *Executed=NULL);
    //! ��������� ����������� ����� � ��������� ������
    void ExecuteNonModal();
    //! �������� ����, ��� ����������� ����� ��������� ��� � �� ������� �����.
    BOOL ifDoubleInstance(Frame* frame);

    //!  �������, ������� �������� � �������� ���������� ������.
    //  ������ ������������ ������ ��� �������� ��������� � ������� ���������� �������� ���� VFMenu
    void ModalizeFrame (Frame *Modalized=NULL, int Mode=TRUE);
    void UnmodalizeFrame (Frame *Unmodalized);

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

    /*$ 26.06.2001 SKV
    ��� ������ ����� ACTL_COMMIT
    */
    BOOL PluginCommit();
    /* SKV$*/

    /* $ 11.10.2001 IS
       ���������� ���������� ������� � ��������� ������.
    */
    int CountFramesWithName(const char *Name, BOOL IgnoreCase=TRUE);
    /* IS $ */

    BOOL IsPanelsActive(); // ������������ ��� ������� WaitInMainLoop
    void SetFramePos(int NewPos);
    int  FindFrameByFile(int ModalType,char *FileName,char *Dir=NULL);
    BOOL ShowBackground();

    void EnterMainLoop();
    void ProcessMainLoop();
    void ExitMainLoop(int Ask);
    int ProcessKey(DWORD key);
    int ProcessMouse(MOUSE_EVENT_RECORD *me);

    void PluginsMenu(); // �������� ���� �� F11
    /* $ 10.05.2001 DJ */
    void SwitchToPanels();
    /* DJ $ */

    INPUT_RECORD *GetLastInputRecord() { return &LastInputRecord; }
    void ResetLastInputRecord() { LastInputRecord.EventType=0; }

    /* $ 12.05.2001 DJ */
    Frame *GetCurrentFrame() { return CurrentFrame; }
    /* DJ $ */

    Frame *operator[](int Index);

    /* $ 19.05.2001 DJ
       operator[] (Frame *) -> IndexOf
    */
    int IndexOf(Frame *Frame);
    /* DJ $ */

    int IndexOfStack(Frame *Frame);
    int HaveAnyFrame();

/* $ ������� ��� ���� CtrlAltShift OT */
    void ImmediateHide();
    /* $ 13.04.2002 KM
      ��� ������ ResizeConsole ��� ���� NextModal �
      ���������� ������.
    */
    void ResizeAllModal(Frame *ModalFrame);
    /* KM $ */
    Frame *GetBottomFrame() { return (*this)[FramePos]; }

    BOOL ManagerIsDown() {return EndLoop;}
    BOOL ManagerStarted() {return StartManager;}

    void InitKeyBar(void);

    /* $ 15.05.2002 SKV
      ��� ��� ����� ��� � ������ ������,
      � ���������� �������� �� �������������,
      �� ����� ��� ������ ���.
    */
    void EnterModalEV(){ModalEVCount++;}
    void ExitModalEV(){ModalEVCount--;}
    BOOL InModalEV(){return ModalEVCount!=0;}
    /* SKV $ */
    void ResizeAllFrame();

    // ���������� top-����� ��� ��� �����, ���� � ������ ���� �������
    Frame* GetTopModal();
};

extern Manager *FrameManager;

#endif  // __MANAGER_HPP__
