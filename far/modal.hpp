#ifndef __MODAL_HPP__
#define __MODAL_HPP__
/*
modal.hpp

Parent class ��� �������� ��ꥪ⮢

*/

/* Revision: 1.02 29.04.2001 $ */

/*
Modify:
  29.04.2001 ��
    + ����७�� NWZ �� ����类��
  29.06.2000 tran
    - (NT Console resize bug)
      adding virtual method SetScreenPosition
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Modal:public ScreenObject
{
  private:
    int     ReadKey,
            WriteKey;
    KeyBar *ModalKeyBar;

  protected:
    INPUT_RECORD ReadRec;
    char HelpTopic[512];
    int  ExitCode;
    int  EndLoop;
    int  EnableSwitch;

  public:
    Modal();
    virtual void GetDialogObjectsData() {};
    int Done();
    void ClearDone();
    int  GetExitCode();
    void SetExitCode(int Code);

    int  GetEnableSwitch() {return(EnableSwitch);};
    void SetEnableSwitch(int Mode) {EnableSwitch=Mode;};

    virtual void Process();

    int  ReadInput();
    void WriteInput(int Key);
    void ProcessInput();

    void SetHelp(char *Topic);
    void ShowHelp();

    void SetKeyBar(KeyBar *ModalKeyBar);
    void UpdateKeyBar();
    virtual void RedrawKeyBar() { Modal::UpdateKeyBar(); };
    int  KeyBarVisible;

    virtual int GetTypeAndName(char *Type,char *Name) {return(MODALTYPE_VIRTUAL);};
    virtual int IsFileModified() {return(FALSE);};
    /* $ 28.06.2000 tran
       (NT Console resize bug) adding virtual method */
    virtual void SetScreenPosition();
    /* tran $ */
    virtual char *GetTypeName() {return "Modal";};

    virtual void OnDestroy() {};  // ��뢠���� ��। 㭨�⮦����� ����
    virtual void OnCreate() {};   // ��뢠���� ��। ᮧ������ ����
    virtual void OnChangeFocus(int focus) {}; // ��뢠���� �� ᬥ�� 䮪��


    int MacroMode;
    int Focus;
};


#endif  //__MODAL_HPP__


