#if !defined(EDITOR2)

#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__
/*
editor.hpp

��������

*/

/* Revision: 1.22 25.06.2001 $ */

/*
Modify:
  25.06.2001 SVS
    - ������� ���� ��� ����� � ���������
  06.06.2001 SVS
    + EDITOR_UNDO_COUNT - "�� ����� � ��������" (��� ����������� :-)
    ! SavePos* �������� �� SavePos (����� ���������� - InternalEditorBookMark)
  03.06.2001 OT
    - �� ���������� StatusLine ����� DrawLine � ���������
  27.05.2001 DJ
    + ��������� ��� ����� �������� Editor::SaveFile()
  07.05.2001 SVS
    ! Search ������ ���������� TRUE/FALSE
  06.05.2001 DJ
    ! �������� #include
  28.03.2001 SVS
    + �������������� �������� ��� SaveFile() - SaveAs
  27.02.2001 SVS
    + *CharCodeBase() - �� ������ ���� ������
  26.02.2001 IS
    ! ����� ��������������� ���������� �������� ���������������� ��
      EditorOptions. ���� ���� ��� ����� �������, �� � ���-�� ��������� :)
    + SetAutoIndent/GetAutoIndent
      SetAutoDetectTable/GetAutoDetectTable
      SetCursorBeyondEOL/GetCursorBeyondEOL
      SetBSLikeDel/GetBSLikeDel
  15.02.2001 IS
    + ��������� ����������, � ������� ������������ ��, ��� ��������� �
      ���������� ���������:
      DelRemovesBlocks - "Del ������� �����"
      PersistentBlocks - "���������� �����"
    + ������� ��� ���������� �� ����������:
      SetDelRemovesBlocks/GetDelRemovesBlocks
      SetPersistentBlocks/GetPersistentBlocks
  15.02.2001 IS
    - ���� ������� SetTabSize ��������� � editor.cpp
    + �� ������ "������� ������ ���������" �������� ���������� ConvertTabs
    + GetConvertTabs � SetConvertTabs
  14.02.2001 IS
    + ������ ��������� �������� � TabSize, �������������� �� ����� ��� ������
      GetTabSize, SetTabSize
  13.02.2001 IS
    + ���������� AttrStr
    + ������� GetFileAttributes;
  12.02.2001 IS
    + FileAttributes
  24.09.2000 SVS
    + ������� Xlat
  10.08.2000 skv
    + ��������� int JustModied � void TextChanged(state);
  30.07.2000 KM 1.06
    + LastSearchWholeWords
  21.07.2000 tran
    - ��� ����� ������  - Bug22
  21.07.2000 tran
    ! GotoLine ����� ��� ������ void � ���������� GoToPosition
  17.07.2000 OT
    + ���������� ����� ��� ���������� "�����" ���������
  14.07.2000 tran
    ! ������� GetRowCol ������ ������� ������
  05.07.2000 tran
    ! ������� ��� �������� � GoToLine() � 'void ' �� 'int'
      ������������ �������� - ��� �������, ��������� �������������
      ������������ ������ � ����� ����� - � ��������� Alt-F8
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "scrobj.hpp"
#include "struct.hpp"
#include "plugin.hpp"
#include "farconst.hpp"

class FileEditor;
class KeyBar;

#define EDITOR_UNDO_COUNT   64

struct InternalEditorBookMark{
  long Line[BOOKMARK_COUNT];
  long Cursor[BOOKMARK_COUNT];
  long ScreenLine[BOOKMARK_COUNT];
  long LeftPos[BOOKMARK_COUNT];
};

struct EditorUndoData
{
  int Type;
  int UndoNext;
  int StrPos;
  int StrNum;
  char *Str;
};

/* $ 27.05.2001 DJ
   ���� �������� Editor::SaveFile()
*/

enum {
    SAVEFILE_ERROR   = 0,         // �������� ���������, �� ����������
    SAVEFILE_SUCCESS = 1,         // ���� ������� ���������, ���� ��������� ���� �� ����
    SAVEFILE_CANCEL  = 2          // ���������� ��������, �������� �� ���������
};

/* DJ $ */

class Editor:public ScreenObject
{
  private:
    KeyBar *EditKeyBar;
    struct EditList *TopList,*EndList,*TopScreen,*CurLine;
    struct EditorUndoData UndoData[EDITOR_UNDO_COUNT];
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
         ���� �������� �������� ����� ��� ��������, ���������� ���-������...
    */
    DWORD FileAttributes;
    /* IS $ */
    /* $ 13.02.2001 IS
         ���� �������� ����� ���������, ����� �� ��������� �� ����� ���
    */
    char AttrStr[4];
    /* IS $ */
    /* $ 26.02.2001 IS
         ���� �������� ������ ��������� � � ���������� ����� ������������ ���,
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

    unsigned char LastSearchStr[SEARCHSTRINGBUFSIZE];
    /* $ 30.07.2000 KM
       ����� ���������� ��� ������ "Whole words"
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

    struct InternalEditorBookMark SavePos;

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
    BOOL Search(int Next);
    /* $ 05.07.2000 tran
       ! ������� ��� �������� � GoToLine() � 'void ' �� 'int'
       ������������ �������� - ��� �������, ��������� �������������
       ������������ ������ � ����� ����� - � ��������� Alt-F8
    */
    /* $ 21.07.2000 tran
       GotoLine ����� ��� ������ void
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
         ������� ������ ����������� ������� �� win32 api
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
    void SetPluginTitle(char *PluginTitle);
    int EditorControl(int Command,void *Param);
    int ProcessEditorInput(INPUT_RECORD *Rec);
    void SetHostFileEditor(FileEditor *Editor) {HostFileEditor=Editor;};
    static int IsShiftKey(int Key);
    static void SetReplaceMode(int Mode);

    /* $ 26.02.2001 IS
         ������� ������/������������ ������� �������� ��������������
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
       ��� ����� ������*/
    int  GetLineCurPos();
    void BeginVBlockMarking();
    void AdjustVBlock(int PrevX);
    /* tran 21.07.2000 $ */
    void Xlat();
};

#endif // __EDITOR_HPP__
#endif //defined(EDITOR2)
