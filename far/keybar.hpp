#ifndef __KEYBAR_HPP__
#define __KEYBAR_HPP__
/*
keybar.hpp

Keybar

*/

/* Revision: 1.05 30.04.2001 $ */

/*
Modify:
  30.04.2001 DJ
    ! ��� ����� ���������� :-) ����� ���� ������������� ���. ��������� API
      ��������.
  28.04.2001 VVM
    + ProcessKey() �������.
  04.04.2001 SVS
    - ����������� �� "���������" :-)
    ! ����� "�����" - �������� ����� ���������� CtrlShiftState
  07.08.2000 SVS
    + ��������� ������ Label - ������� Change(Group,...)
  02.08.2000 SVS
    + �������������� ����������
      CtrlShiftName, AltShiftName, CtrlAltName
    + � ���� ����������� - �������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

/* $ 07.08.2000 SVS
   ������ �����
*/
enum {
  KBL_MAIN=0,
  KBL_SHIFT,
  KBL_CTRL,
  KBL_ALT,
  KBL_CTRLSHIFT,
  KBL_ALTSHIFT,
  KBL_CTRLALT,

  /* $ 30.04.2001 DJ */
  KBL_GROUP_COUNT
  /* DJ $ */
};
/* SVS $ */

const int KEY_COUNT = 12;

/* $ 30.04.2001 DJ
   �������� typedef; ��������� ������ ����������, ���� ���������� ��
   ������������ ���� � ������
*/

typedef char KeyBarTitle [10];
typedef KeyBarTitle KeyBarTitleGroup [KEY_COUNT];

class KeyBar: public ScreenObject
{
  private:
    BaseInput *Owner;

    KeyBarTitleGroup KeyTitles [KBL_GROUP_COUNT];
    int KeyCounts [KBL_GROUP_COUNT];

    int AltState,CtrlState,ShiftState;
    int DisableMask;

  private:
    void DisplayObject();

  public:
    KeyBar();
    void SetOwner(BaseInput *Owner);
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);

    void SetGroup(int Group,char **Key,int KeyCount);
    void ClearGroup(int Group);

    void Set(char **Key,int KeyCount)            { SetGroup (KBL_MAIN, Key, KeyCount); }
    void SetShift(char **Key,int KeyCount)       { SetGroup (KBL_SHIFT, Key, KeyCount); }
    void SetAlt(char **Key,int KeyCount)         { SetGroup (KBL_ALT, Key, KeyCount); }
    void SetCtrl(char **Key,int KeyCount)        { SetGroup (KBL_CTRL, Key, KeyCount); }
    /* $ 02.08.2000 SVS
       �������������� ����������
    */
    void SetCtrlShift(char **Key,int KeyCount)   { SetGroup (KBL_CTRLSHIFT, Key, KeyCount); }
    void SetAltShift(char **Key,int KeyCount)    { SetGroup (KBL_ALTSHIFT, Key, KeyCount); }
    void SetCtrlAlt(char **Key,int KeyCount)     { SetGroup (KBL_CTRLALT, Key, KeyCount); }
    /* SVS $*/

    void SetDisableMask(int Mask);
    void Change(char *NewStr,int Pos)            { Change (KBL_MAIN, NewStr, Pos); }

    /* $ 07.08.2000 SVS
       ��������� ������ Label
    */
    void Change(int Group,char *NewStr,int Pos);
    /* SVS $ */

    /* $ 30.04.2001 DJ
       ��������� ��������� ������ ������ ����� LNG ��� ��������� ������
    */
    void SetAllGroup (int Group, int StartIndex, int Count);
    /* DJ $ */

    void RedrawIfChanged();
};

/* DJ $ */

#endif	// __KEYBAR_HPP__
