#ifndef __VIEWER_HPP__
#define __VIEWER_HPP__
/*
viewer.hpp

Internal viewer

*/

/* Revision: 1.09 14.09.2000 $ */

/*
Modify:
  14.06.2000 SVS
    + ��६����� FirstWord - ��ࢮ� ᫮�� �� 䠩��
      (��� ��⮮�।������ Unicode)
  12.09.2000 SVS
    + ������� ��६����� TypeWrap. ������
      Wrap - ����ﭨ� (Wrap/UnWrap) �
      TypeWrap - ⨯ (Wrap/WWrap)
  30.07.2000 KM 1.07
    + LastSearchWholeWords
  19.07.2000 tran 1/06
    + Viewer::Width, ::XX2
  18.07.2000 tran 1.05
    * ������� ⨯ ��ࠬ��� � SetFilePos()
      �� unsigned
  12.07.2000 tran
    ! OutStr are dynamic, new, delete,
      and sizeof(OutStr[i]) changed to MAX_VIEWLINEB
  12.07.2000 SVS
    - ��-�� 㢥��祭�� ����� ��ப� �� 0x800 �뫥⠫ FAR
      �� Alt-F7. ����⨬ MAX_VIEWLINE �� 1024 (0x400)
  10.07.2000 tran
    + 㢥��祭�� ����� ��ப� - � 512 �� MAX_VIEWLINE
      MAX_VIEWLINEB = MAX_VIEWLINE + 16
  04.07.2000 tran
    + 'warning' parameter in OpenFile() method
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

/* $ 12.07.2000 SVS
  - ��-�� 㢥��祭�� ����� ��ப� �� 0x800 �뫥⠫ FAR
    �� Alt-F7. ����⨬ MAX_VIEWLINE �� 1024 (0x400)
*/
#define MAX_VIEWLINE  0x800 // 0x400
#define MAX_VIEWLINEB 0x80f // 0x40f
/* SVS $ */

class Viewer:public ScreenObject
{
  private:
    /* $ 14.06.2000 SVS
      + ��६����� FirstWord - ��ࢮ� ᫮�� �� 䠩��
      (��� ��⮮�।������ Unicode)
    */
    WORD FirstWord;
    /* SVS $ */

    NamesList ViewNamesList;
    KeyBar *ViewKeyBar;
    /* $ 12.07.2000 tran
     dymanic alloc memory for OutStr */
    char *OutStr[MAXSCRY+1]; //[MAX_VIEWLINEB];
    /* tran 12.07.2000 $ */
    int StrFilePos[MAXSCRY+1];
    char FileName[NM];
    char FullFileName[NM];
    FILE *ViewFile;
    WIN32_FIND_DATA ViewFindData;

    unsigned char LastSearchStr[256];
    /* $ 30.07.2000 KM
       ����� ��६����� ��� ���᪠
    */
    int LastSearchCase,LastSearchWholeWords,LastSearchReverse,LastSearchHex;
    /* KM $ */

    struct CharTableSet TableSet;
    int UseDecodeTable,TableNum,AnsiText;
    int Unicode;

    /* $ 12.09.2000 SVS
       ������� ��६����� TypeWrap. ������
       Wrap - ����ﭨ� (Wrap/UnWrap) �
       TypeWrap - ⨯ (Wrap/WWrap)
    */
    int Wrap, TypeWrap, Hex;
    /* SVS $*/

    unsigned long FilePos;
    unsigned long SecondPos;
    unsigned long LastScrPos;
    unsigned long FileSize;
    unsigned long LastSelPos;
    int LeftPos;
    int LastPage;
    int CRSym;
    int SelectPos,SelectSize;
    int ViewY1;
    int ShowStatusLine,HideCursor;
    char TempViewName[NM];
    char Title[512];
    char PluginData[NM*2];
    int TableChangedByUser;
    int ReadStdin;
    int InternalKey;

    unsigned long SavePosAddr[10];
    int SavePosLeft[10];

    unsigned long UndoAddr[128];
    int UndoLeft[128];
    int LastKeyUndo;
    /* $ 19.07.2000 tran
       ����� ��६�����, �ᯮ������ �� ���� �ਭ� �� �஫��� */
    int Width,XX2;
    /* tran 19.07.2000 $ */

  private:
    void DisplayObject();
    void Up();
    void ShowHex();
    void ShowUp();
    void ShowStatus();
    void ReadString(char *Str,int MaxSize,int StrSize,int &SelPos,int &SelSize);
    int CalcStrSize(char *Str,int Length);
    void ChangeViewKeyBar();
    void SetCRSym();
    void Search(int Next,int FirstChar);
    void ConvertToHex(char *SearchStr,int &SearchLength);
    int HexToNum(int Hex);
    int vread(char *Buf,int Size,FILE *SrcFile);
    int vseek(FILE *SrcFile,unsigned long Offset,int Whence);
    unsigned long vtell(FILE *SrcFile);
    int vgetc(FILE *SrcFile);
    void GoTo();
    void SetFileSize();

  public:
    Viewer();
    ~Viewer();


  public:
    /* $ 04.07.2000 tran
       + 'warning' parameter */
    int OpenFile(char *Name,int warning);
    /* tran $ */
    void SetViewKeyBar(KeyBar *ViewKeyBar);
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void SetStatusMode(int Mode);
    void EnableHideCursor(int HideCursor);
    int GetWrapMode();
    void SetWrapMode(int Wrap);
    void KeepInitParameters();
    void GetFileName(char *Name);
    void ShowConsoleTitle();
    void SetTempViewName(char *Name);
    void SetTitle(char *Title);
    unsigned long GetFilePos();
    /* $ 18.07.2000 tran - change 'long' to 'unsigned long' */
    void SetFilePos(unsigned long Pos);
    void SetPluginData(char *PluginData);
    void SetNamesList(NamesList *List);
};

#endif // __VIEWER_HPP__
