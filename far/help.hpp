#ifndef __HELP_HPP__
#define __HELP_HPP__
/*
help.hpp

������

*/

/* Revision: 1.02 01.09.2000 $ */

/*
Modify:
  01.09.2000 SVS
    + CtrlColorChar - ����! ��� ᯥ�ᨬ����-ᨬ���� - ��� ��ਡ�⮢
    + CurColor - ⥪�騩 梥� ���ᮢ��
  28.06.2000 tran
    - NT Console resize bug
      adding SetScreenPosition method
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Help:public Modal
{
  private:
    char *HelpData;
    char HelpTopic[512];
    char SelTopic[512];
    char HelpPath[NM];

    /* $ 01.09.2000 SVS
      CurColor - ⥪�騩 梥� ���ᮢ��
    */
    int CurColor;
    /* SVS $ */

    int TopLevel;
    int PrevFullScreen;
    int StrCount,FixCount,FixSize;
    int TopStr;
    int CurX,CurY;
    int ShowPrev;
    int DisableOut;
    int TopicFound;
    int PrevMacroMode;

    /* $ 01.09.2000 SVS
      CtrlColorChar - ����! ��� ᯥ�ᨬ����-ᨬ���� - ��� ��ਡ�⮢
    */
    BYTE CtrlColorChar;
    /* SVS $ */

  private:
    void DisplayObject();
    void ReadHelp();
    void AddLine(char *Line);
    void HighlightsCorrection(char *Str);
    void FastShow();
    void OutString(char *Str);
    int StringLen(char *Str);
    void CorrectPosition();
    int IsReferencePresent();
    void MoveToReference(int Forward,int CurScreen);
    void ReadPluginsHelp();

  public:
    Help(char *Topic);
    Help(char *Topic,int &ShowPrev,int PrevFullScreen);
    ~Help();

  public:
    void Hide();
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    static int GetFullScreenMode();
    static void SetFullScreenMode(int Mode);
    static int PluginPanelHelp(HANDLE hPlugin);

    /* $ 28.06.2000 tran
       NT Console resize - resize help */
    virtual void SetScreenPosition();
    /* tran $ */
};

#endif	// __HELP_HPP__
