#ifndef __HELP_HPP__
#define __HELP_HPP__
/*
help.hpp

������

*/

/* Revision: 1.30 17.07.2005 $ */

/*
Modify:
  17.07.2005 SVS
    ! ����� DHELP2
  18.12.2004 WARP
    ! ������������ �������� ������ � .hlf ������ (BugZ#1084)
  13.10.2003 SVS
    ! ��������� ��� ����������������� CtrlColorChar
  03.06.2003 SVS
    + HELPMODE_CLICKOUTSIDE  - ���� ������� ���� ��� �����?
  22.04.2003 SVS
    + FHELPOBJ_ERRCANNOTOPENHELP - ��������� ������
  14.07.2002 IS
    ! ��������� const
  24.12.2001 SVS
    ! HelpMask ��������� � StackHelpData.
    + ���������� ������ � ����� (�������, ����� ������)
  29.11.2001 DJ
    ! ��������� ����� ����� �������� � ��������� �������
    + ������ PluginContents ��� �������� �����
  26.11.2001 VVM
    ! ������ ���� �� ��������� �� ���������� ������� �����, ���� ������� ���� ������ �� � �����.
  01.11.2001 SVS
    + ������� ��� "����" - GetType*()
  01.10.2001 SVS
    + CtrlTabSize - �����! ������ ��������� - ������ �� �������!
  07.08.2001 SVS
    ! ��������� - ��� ����������� ���� (�� ������ help2.?pp)
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

#define HELPMODE_CLICKOUTSIDE  0x20000000 // ���� ������� ���� ��� �����?

struct StackHelpData
{
  DWORD Flags;                  // �����
  int   TopStr;                 // ����� ������� ������� ������ ����
  int   CurX,CurY;              // ���������� (???)

  char  HelpMask[NM];           // �������� �����
  char  HelpPath[NM];           // ���� � ������
  char  HelpTopic[512];         // ������� �����
  char  SelTopic[512];          // ���������� ����� (???)
};

enum HELPDOCUMENTSHELPTYPE{
  HIDX_PLUGINS,                 // ������ ��������
  HIDX_DOCUMS,                  // ������ ����������
};

enum {
  FHELPOBJ_ERRCANNOTOPENHELP  = 0x80000000,
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
    int   MouseDown;

    int   DisableOut;           // TRUE - �� �������� �� �����
    char  CtrlColorChar[16];    // CtrlColorChar - �����! ��� �����������-
                                //   ������� - ��� ���������
    int   CurColor;             // CurColor - ������� ���� ���������
    int   CtrlTabSize;          // CtrlTabSize - �����! ������ ���������

    int   PrevMacroMode;        // ���������� ����� �������

    /* $ 29.11.2001 DJ
       ������ PluginContents (��� ����������� � ���������)
    */
    char CurPluginContents[NM];
    /* DJ $ */

    DWORD LastStartPos;
    DWORD StartPos;

    char  CtrlStartPosChar[16];

#if defined(WORK_HELP_FIND)
  private:
    DWORD LastSearchPos;
    unsigned char LastSearchStr[SEARCHSTRINGBUFSIZE];
    int LastSearchCase,LastSearchWholeWords,LastSearchReverse;

  private:
    int Search(int Next);
    void KeepInitParameters();
#endif

  private:
    void DisplayObject();
    int  ReadHelp(const char *Mask=NULL);
    void AddLine(const char *Line);
    void AddTitle(const char *Title);
    void HighlightsCorrection(char *Str);
    void FastShow();
    /* $ 29.11.2001 DJ
       �������� �� FastShow
    */
    void DrawWindowFrame();
    /* DJ $ */
    void OutString(const char *Str);
    int  StringLen(const char *Str);
    void CorrectPosition();
    int  IsReferencePresent();
    void MoveToReference(int Forward,int CurScreen);
    void ReadDocumentsHelp(int TypeIndex);
    int  JumpTopic(const char *JumpTopic=NULL);

  public:
    Help(const char *Topic,const char *Mask=NULL,DWORD Flags=0);
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

    virtual const char *GetTypeName() {return "[Help]";}
    virtual int GetTypeAndName(char *Type,char *Name);
    virtual int GetType() { return MODALTYPE_HELP; }

    static char *MkTopic(int PluginNumber,const char *HelpTopic,char *Topic);
};

#endif  // __HELP_HPP__
