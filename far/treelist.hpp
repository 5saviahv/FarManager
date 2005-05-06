#ifndef __TREELIST_HPP__
#define __TREELIST_HPP__
/*
treelist.hpp

Tree panel

*/

/* Revision: 1.22 06.05.2005 $ */

/*
Modify:
  06.05.2005 SVS
    ! ???::GetCurDir() ������ ���������� ������ ����, ��� ����
      ��� �������� ����� ���� ����� NULL. ������� ��� ����, �����
      ��� �� �������� ���� ������.
  23.04.2005 KM
    ! ������������� ������� � GetSelName
  01.04.2005 SVS
    + GetItem()
  10.03.2005 SVS
    + � FindFile() � GoToFile() ������ �������� - ������ ������ �� ����� �����
    + CreateTreeFileName() - ��������� ��� ������� Tree.far
  03.03.2005 SVS
    ! � ������� FindPartName() �������� ������ �������� - ����������� ������.
  14.02.2005 SVS
    + � TreeList ��������� ������� GetFileName(), FindFile()
  10.11.2004 SVS
    + TreeList::GetFileCount()
  28.10.2004 SVS
    + "������������" ���� - TreeFileName() � TreeCacheFolderName()
      � ���������� ����� ����������� TODO �� ���� ��������!
    ! ��������� ���������� ������� � ���������
  16.10.2004 SVS
    + GetNextNavPos() � GetPrevNavPos()
  11.07.2003 SVS
    + NumericSort
  08.04.2002 IS
    ! ��������� const
  12.02.2002 SVS
    + Scroll()
  24.12.2001 VVM
    ! GetCurName � ������ ��������� ��� virtual!!!
  11.12.2001 SVS
    + ���� ������ � ��������
  24.10.2001 SVS
    - ���� � ����������� ��� ������ ������ �� ������� �����������
  24.10.2001 VVM
    + TreeIsPrepared - ��������������� ����� ������ ������ � �����/�����.
  22.10.2001 SVS
    ! ReadTree() ���������� TRUE/FALSE
  21.10.2001 SVS
    + PR_MsgReadTree
  06.05.2001 DJ
    ! �������� #include
  25.04.2001 SVS
    + SetRootDir()
  05.04.2001 VVM
    + ������������ �������� � ����� MACRO_TREEPANEL
  16.10.2000 tran
    + MustBeCached(Root) - �������, ������������ ������������� �����������
      ������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "panel.hpp"

struct TreeItem
{
  char Name[NM];         // ��� �����
  int Last[NM/2];        // ?
  int Depth;             // ������� �����������
};

enum TREELIST_FLAGS{
  FTREELIST_TREEISPREPARED          = 0x00010000,
  FTREELIST_UPDATEREQUIRED          = 0x00020000,
  FTREELIST_ISPANEL                 = 0x00040000,
};

class TreeList: public Panel
{
  private:
    int PrevMacroMode;
    struct TreeItem *ListData;
    char Root[NM];
    long TreeCount;
    long WorkDir;
    long GetSelPosition;
    int CaseSensitiveSort;
    int NumericSort;
    int ExitCode; // ��������� ������ ��� ������, ���������� �� ������!

  private:
    void SetMacroMode(int Restore = FALSE);
    void DisplayObject();
    void DisplayTree(int Fast);
    void DisplayTreeName(char *Name,int Pos);
    void Up(int Count);
    void Down(int Count);
    void Scroll(int Count);
    void CorrectPosition();
    void FillLastData();
    int CountSlash(char *Str);
    int SetDirPosition(char *NewDir);
    void GetRoot();
    Panel* GetRootPanel();
    void SyncDir();
    void SaveTreeFile();
    int ReadTreeFile();
    int GetSelCount();
    int GetSelName(char *Name,int &FileAttr,char *ShortName=NULL,WIN32_FIND_DATA *fd=NULL);
    void DynamicUpdateKeyBar();
    int GetNextNavPos();
    int GetPrevNavPos();
    static char *MkTreeFileName(const char *RootDir,char *Dest,int DestSize);
    static char *MkTreeCacheFolderName(const char *RootDir,char *Dest,int DestSize);
    static char *CreateTreeFileName(const char *Path,char *Dest,int DestSize);

  private:
    static int MsgReadTree(int TreeCount,int &FirstCall);
    static int GetCacheTreeName(char *Root,char *Name,int CreateDir);

  public:
    TreeList(int IsPanel=TRUE);
    ~TreeList();

  public:
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
//    void KillFocus();
    void Update(int Mode);
    int  ReadTree();
    void SetCurDir(char *NewDir,int ClosePlugin);
    void SetRootDir(char *NewRootDir);
    int GetCurDir(char *CurDir);
    virtual int GetCurName(char *Name,char *ShortName);
    void UpdateViewPanel();
    void MoveToMouse(MOUSE_EVENT_RECORD *MouseEvent);
    int FindPartName(char *Name,int Next,int Direct=1);
    int GoToFile(const char *Name,BOOL OnlyPartName=FALSE);
    int FindFile(const char *Name,BOOL OnlyPartName=FALSE);
    void ProcessEnter();
    int GetExitCode() {return ExitCode;}
    virtual long GetFileCount() {return TreeCount;}
    virtual int GetFileName(char *Name,int Pos,int &FileAttr);

    virtual void SetTitle();
    virtual void SetFocus();
    virtual void KillFocus();
    virtual BOOL UpdateKeyBar();
    virtual BOOL GetItem(int Index,void *Dest);

  public:
    static void AddTreeName(char *Name);
    static void DelTreeName(char *Name);
    static void RenTreeName(char *SrcName,char *DestName);
    static void ReadSubTree(char *Path);
    static void ClearCache(int EnableFreeMem);
    static void ReadCache(char *TreeRoot);
    static void FlushCache();

    static int MustBeCached(char *Root); // $ 16.10.2000 tran - �������, ������������� ������������� ����������� �����
    static void PR_MsgReadTree(void);
};

#endif  // __TREELIST_HPP__
