#ifndef __HELP_HPP__
#define __HELP_HPP__
/*
help.hpp

������

*/

/* Revision: 1.18 05.08.2001 $ */

/*
Modify:
  05.08.2001 SVS
    + AddTitle() - �������� ����.
  01.08.2001 SVS
    + MkTopic() - �������� ������
    ! ReadPluginsHelp() ������������� � ReadDocumentsHelp, �.�.
      �������������� ���������� ������ ������� �� ������ �� ��������
  22.07.2001 SVS
    ! ��������� number two - �������� ��� �������� �������� ���������
      ������������ ����� (�� ����, ���)
  20.07.2001 SVS
    ! PluginPanelHelp ��������� � �������� (�� ����� �� �����)
    ! ������� �� ������������� Get/Set-FullScreenMode
  20.07.2001 SVS
    ! "�������� �������" Help API. Part I.
  11.07.2001 OT
    ! ������� CtrlAltShift � Manager
  31.05.2001 OT
    + ResizeConsole() - ��� ������� �� ������������ ������ �������.
  15.05.2001 OT
    ! NWZ -> NFZ
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  12.04.2001 SVS
    + ���������� �������� Mask, ����������� � �����������
    + �������� ����������� Mask � ����������� � ShowPrev
  26.03.2001 SVS
    ! ReadHelp ���������� TRUE/FALSE
  30.12.2000 SVS
    + KeyBar � Help`�
  18.12.2000 SVS
    + �������������� �������� � ������������ - DWORD Flags.
    + ���� ������ - Flags
  12.09.2000 SVS
    + ��������� � ������� ReadHelp � ������������, �������� ����� ������
      ������.
    + GetError() - ���������� ErrorHelp.
  01.09.2000 SVS
    + CtrlColorChar - �����! ��� �����������-������� - ��� ���������
    + CurColor - ������� ���� ���������
  28.06.2000 tran
    - NT Console resize bug
      adding SetScreenPosition method
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "frame.hpp"
#include "keybar.hpp"
#include "farconst.hpp"

class CallBackStack;

struct StackHelpData
{
  DWORD Flags;                  // �����
  int   TopStr;                 // ����� ������� ������� ������ ����
  int   CurX,CurY;              // ���������� (???)

  char  HelpPath[NM];           // ���� � ������
  char  HelpTopic[512];         // ������� �����
  char  SelTopic[512];          // ���������� ����� (???)
};

enum HELPDOCUMENTSHELPTYPE{
  HIDX_PLUGINS,                 // ������ ��������
  HIDX_DOCUMS,                  // ������ ����������
};

class Help:public Frame
{
  private:
    BOOL  ErrorHelp;            // TRUE - ������! �������� - ��� ������ ������
    SaveScreen *TopScreen;      // ������� ���������� ��� ������
    KeyBar      HelpKeyBar;     // ������
    CallBackStack *Stack;       // ���� ��������

    struct StackHelpData StackData;
    char *HelpData;             // "����" � ������.
    int   StrCount;             // ���������� ����� � ����
    int   FixCount;             // ���������� ����� ���������������� �������
    int   FixSize;              // ������ ���������������� �������
    int   TopicFound;           // TRUE - ����� ������
    int   IsNewTopic;           // ��� ����� �����?
    char *HelpMask;             // �������� �����, ���������� � �����������

    int   DisableOut;           // TRUE - �� �������� �� �����
    BYTE  CtrlColorChar;        // CtrlColorChar - �����! ��� �����������-
                                //   ������� - ��� ���������
    int   CurColor;             // CurColor - ������� ���� ���������

    int   PrevMacroMode;        // ���������� ����� �������

  private:
    void DisplayObject();
    int  ReadHelp(char *Mask=NULL);
    void AddLine(char *Line);
    void AddTitle(char *Title);
    void HighlightsCorrection(char *Str);
    void FastShow();
    void OutString(char *Str);
    int  StringLen(char *Str);
    void CorrectPosition();
    int  IsReferencePresent();
    void MoveToReference(int Forward,int CurScreen);
    void ReadDocumentsHelp(int TypeIndex);
    int  JumpTopic(const char *JumpTopic=NULL);

  public:
    Help(char *Topic,char *Mask=NULL,DWORD Flags=0);
    ~Help();

  public:
    void Hide();
    int  ProcessKey(int Key);
    int  ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void InitKeyBar(void);
    BOOL GetError() {return ErrorHelp;}
    /* $ 28.06.2000 tran NT Console resize - resize help */
    virtual void SetScreenPosition();
    void OnChangeFocus(int focus); // ���������� ��� ����� ������
    void ResizeConsole();
    /* $ ������� ��� ���� CtrlAltShift OT */
    int  FastHide();

    static char *MkTopic(int PluginNumber,const char *HelpTopic,char *Topic);
};

#endif	// __HELP_HPP__
