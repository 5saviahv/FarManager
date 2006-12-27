#ifndef __HELP_HPP__
#define __HELP_HPP__
/*
help.hpp

������

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
    char  FullHelpPathName[512];

    struct StackHelpData StackData;
    char *HelpData;             // "����" � ������.
    int   StrCount;             // ���������� ����� � ����
    int   FixCount;             // ���������� ����� ���������������� �������
    int   FixSize;              // ������ ���������������� �������
    int   TopicFound;           // TRUE - ����� ������
    int   IsNewTopic;           // ��� ����� �����?
    int   MouseDown;

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
