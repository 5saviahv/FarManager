#ifndef __MANAGER_HPP__
#define __MANAGER_HPP__
/*
manager.hpp

������������ ����� ����������� file panels, viewers, editors
*/
/*
Copyright (c) 1996 Eugene Roshal
Copyright (c) 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

class Frame;

class Manager
{
#if defined(SYSLOG)
    friend void ManagerClass_Dump(const wchar_t *Title,const Manager *m,FILE *fp);
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
    int CountFramesWithName(const wchar_t *Name, BOOL IgnoreCase=TRUE);
    /* IS $ */

    BOOL IsPanelsActive(); // ������������ ��� ������� WaitInMainLoop
    void SetFramePos(int NewPos);
    int  FindFrameByFile(int ModalType,const wchar_t *FileName,const wchar_t *Dir=NULL);
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
