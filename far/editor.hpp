#if !defined(EDITOR2)

#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__
/*
editor.hpp

�������

*/

/* Revision: 1.16 28.03.2001 $ */

/*
Modify:
  28.03.2001 SVS
    + �������⥫�� ��ࠬ��� ��� SaveFile() - SaveAs
  27.02.2001 SVS
    + *CharCodeBase() - �� ������ ���� �뢮��
  26.02.2001 IS
    ! ����� ᠬ����⥫��� ��६����� �������� ᮮ⢥�����騬� ��
      EditorOptions. ���� �뫮 �� �ࠧ� ᤥ����, �� � ��-� ��ମ��� :)
    + SetAutoIndent/GetAutoIndent
      SetAutoDetectTable/GetAutoDetectTable
      SetCursorBeyondEOL/GetCursorBeyondEOL
      SetBSLikeDel/GetBSLikeDel
  15.02.2001 IS
    + ������� ��६����, � ������ ������������ �, �� �࠭����� �
      ����ன��� ।����:
      DelRemovesBlocks - "Del 㤠��� �����"
      PersistentBlocks - "����ﭭ� �����"
    + �㭪樨 ��� �ࠢ����� �� ���ﭨ��:
      SetDelRemovesBlocks/GetDelRemovesBlocks
      SetPersistentBlocks/GetPersistentBlocks
  15.02.2001 IS
    - ���� �㭪樨 SetTabSize ��॥堫� � editor.cpp
    + �� ०��� "�஡��� ����� ⠡��樨" �⢥砥� ��६����� ConvertTabs
    + GetConvertTabs � SetConvertTabs
  14.02.2001 IS
    + ������ ⠡��樨 �࠭���� � TabSize, �����㫨஢��� �� ����� �� �����
      GetTabSize, SetTabSize
  13.02.2001 IS
    + ��६����� AttrStr
    + �㭪�� GetFileAttributes;
  12.02.2001 IS
    + FileAttributes
  24.09.2000 SVS
    + �㭪�� Xlat
  10.08.2000 skv
    + ��������� int JustModied � void TextChanged(state);
  30.07.2000 KM 1.06
    + LastSearchWholeWords
  21.07.2000 tran
    - �� ����� ��⮤�  - Bug22
  21.07.2000 tran
    ! GotoLine �⠫� ��� ࠭�� void � ���������� GoToPosition
  17.07.2000 OT
    + ���⮫���� ���� ��� ࠧࠡ��� "�����" ।����
  14.07.2000 tran
    ! �㭪�� GetRowCol ᤥ��� ��⮤�� �����
  05.07.2000 tran
    ! ������� ⨯ ������ � GoToLine() � 'void ' �� 'int'
      �����頥��� ���祭�� - �� �������, ��������� ���짮��⥫��
      �ᯮ������ ⮫쪮 � ����� ���� - � ��ࠡ�⪥ Alt-F8
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Editor:public ScreenObject
{
  private:
    KeyBar *EditKeyBar;
    struct EditList *TopList,*EndList,*TopScreen,*CurLine;
    struct EditorUndoData UndoData[64];
    int UndoDataPos;
    int UndoOverflow;
    int UndoSavePos;
    int LastChangeStrPos;
    char FileName[NM];
    int NumLastLine,NumLine;
    int Modified;
    /*$ 10.08.2000 skv
      need to send EE_REDRAW 2.
      set to 1 by TextChanged, no matter what is value of State.
    */
    int JustModified;
    /* skv $*/
    /* $ 12.02.2001 IS
         � �������� ��ਡ��� 䠩�� �� ����⨨, �ਣ������ ���-�����...
    */
    DWORD FileAttributes;
    /* IS $ */
    /* $ 13.02.2001 IS
         � �������� �㪢� ��ਡ�⮢, �⮡� �� ������� �� ����� ࠧ
    */
    char AttrStr[4];
    /* IS $ */
    /* $ 26.02.2001 IS
         � �������� ࠧ��� ⠡��樨 � � ���쭥�襬 �㤥� �ᯮ�짮���� ���,
         � �� Opt.TabSize
    */
    struct EditorOptions EdOpt;
    /* IS $ */
    int WasChanged;
    int Overtype;
    int DisableOut;
    int Pasting;
    int MarkingBlock;
    char GlobalEOL[10];
    struct EditList *BlockStart;
    int BlockStartLine;

    struct EditList *VBlockStart;
    int VBlockX;
    int VBlockSizeX;
    int VBlockY;
    int VBlockSizeY;
    int MarkingVBlock;

    int DisableUndo;
    int NewUndo;
    int LockMode;
    int BlockUndo;

    int MaxRightPos;

    unsigned char LastSearchStr[256];
    /* $ 30.07.2000 KM
       ����� ��६����� ��� ���᪠ "Whole words"
    */
    int LastSearchCase,LastSearchWholeWords,LastSearchReverse;
    /* $ KM */

    struct CharTableSet TableSet;
    int UseDecodeTable,TableNum,AnsiText;
    int StartLine,StartChar;

    int TableChangedByUser;

    char Title[512];
    char PluginData[NM*2];

    char PluginTitle[512];

    long SavePosLine[10];
    long SavePosCursor[10];
    long SavePosScreenLine[10];
    long SavePosLeftPos[10];

    int EditorID;
    bool OpenFailed;

    FileEditor *HostFileEditor;

  private:
    void DisplayObject();
    void ShowEditor(int CurLineOnly);
    void ShowStatus();
    void DeleteString(struct EditList *DelPtr,int DeleteLast,int UndoLine);
    void InsertString();
    void Up();
    void Down();
    void ScrollDown();
    void ScrollUp();
    void Search(int Next);
    /* $ 05.07.2000 tran
       ! ������� ⨯ ������ � GoToLine() � 'void ' �� 'int'
       �����頥��� ���祭�� - �� �������, ��������� ���짮��⥫��
       �ᯮ������ ⮫쪮 � ����� ���� - � ��ࠡ�⪥ Alt-F8
    */
    /* $ 21.07.2000 tran
       GotoLine �⠫� ��� ࠭�� void
       � ���������� GoToPosition
       */

    void GoToLine(int Line);
    void GoToPosition();
    /* tran 21.07.2000 $ */

    /* tran 05.07.2000 $ */

    /* $ 10.08.2000 skv
      Call this when text changed to set Modified to
      specified State and JustModified to 1
    */
    void TextChanged(int State);
    /* skv $*/

    /* $ 13.02.2001 IS
         ����⪠ ����� ����������� �㭪樨 �� win32 api
    */
    DWORD GetFileAttributes(LPCTSTR);
    /* IS $ */
    int  CalcDistance(struct EditList *From,struct EditList *To,int MaxDist);
    void Paste();
    void Copy(int Append);
    void DeleteBlock();
    void UnmarkBlock();
    void ChangeEditKeyBar();
    void AddUndoData(char *Str,int StrNum,int StrPos,int Type);
    void Undo();
    void SelectAll();
    void SetStringsTable();
    void BlockLeft();
    void BlockRight();
    void DeleteVBlock();
    void VCopy(int Append);
    void VPaste(char *ClipText);
    void VBlockShift(int Left);
    struct EditList * GetStringByNumber(int DestLine);

  public:
    Editor();
    ~Editor();

  public:
    int ReadFile(char *Name,int &UserBreak);
    int SaveFile(char *Name,int Ask,int TextFormat,int SaveAs);
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    void SetEditKeyBar(KeyBar *EditKeyBar);
    void KeepInitParameters();
    void SetStartPos(int LineNum,int CharNum);
    int IsFileModified();
    int IsFileChanged();
    void SetTitle(char *Title);
    long GetCurPos();
    void SetPluginData(char *PluginData);
    int EditorControl(int Command,void *Param);
    int ProcessEditorInput(INPUT_RECORD *Rec);
    void SetHostFileEditor(FileEditor *Editor) {HostFileEditor=Editor;};
    static int IsShiftKey(int Key);
    static void SetReplaceMode(int Mode);

    /* $ 26.02.2001 IS
         �㭪樨 �⥭��/��⠭������� ⥪��� ����஥� ।���஢����
    */
    void SetTabSize(int NewSize);
    int  GetTabSize(void) const {return EdOpt.TabSize; }

    void SetConvertTabs(int NewMode);
    int  GetConvertTabs(void) const {return EdOpt.ExpandTabs; }

    void SetDelRemovesBlocks(int NewMode);
    int  GetDelRemovesBlocks(void) const {return EdOpt.DelRemovesBlocks; }

    void SetPersistentBlocks(int NewMode);
    int  GetPersistentBlocks(void) const {return EdOpt.PersistentBlocks; }

    void SetAutoIndent(int NewMode) { EdOpt.AutoIndent=NewMode; }
    int  GetAutoIndent(void) const {return EdOpt.AutoIndent; }

    void SetAutoDetectTable(int NewMode) { EdOpt.AutoDetectTable=NewMode; }
    int  GetAutoDetectTable(void) const {return EdOpt.AutoDetectTable; }

    void SetCursorBeyondEOL(int NewMode);
    int  GetCursorBeyondEOL(void) const {return EdOpt.CursorBeyondEOL; }

    void SetBSLikeDel(int NewMode) { EdOpt.BSLikeDel=NewMode; }
    int  GetBSLikeDel(void) const {return EdOpt.BSLikeDel; }
    /* IS $ */

    void SetCharCodeBase(int NewMode) { EdOpt.CharCodeBase=NewMode%3; }
    int  GetCharCodeBase(void) const {return EdOpt.CharCodeBase; }

    /* $ tran 14.07.2000
      + goto to percent support */
    void GetRowCol(char *argv,int *row,int *col);
    /* tran 14.07.2000 $ */

    /* $ 21.07.2000 tran
       �� ����� ��⮤�*/
    int  GetLineCurPos();
    void BeginVBlockMarking();
    void AdjustVBlock(int PrevX);
    /* tran 21.07.2000 $ */
    void Xlat();
};

struct EditList
{
  struct EditList *Prev;
  struct EditList *Next;
  Edit EditLine;
};

#endif // __EDITOR_HPP__
#endif //defined(EDITOR2)
