#ifndef __PANEL_HPP__
#define __PANEL_HPP__
/*
panel.hpp

Parent class ��� �������

*/

/* Revision: 1.23 18.05.2004 $ */

/*
Modify:
  18.05.2004 SVS
    + Set/GetNumericSort()
    + ���� ������ NumericSort
    ! �� ��������� PanelViewSettings ������ NumericSort
  07.01.2004 SVS
    + FastFindProcessName() - ������ ����� ����� :-)
  11.07.2003 SVS
    + NumericSort
  13.01.2003 SVS
    + PanelViewSettings.FolderAlignExtensions
  21.12.2002 SVS
    + UPDATE_DRAW_MESSAGE - "���������� ������� ������������ � ����"
  10.12.2002 SVS
    + ProcessDelDisk() - ������ ������ ��������, ��������� �� VMenu ��� ����, ����� �����
      ������������ ����!
  18.06.2002 SVS
    + Panel::IfGoHome()
  08.04.2002 IS
    ! ������� const
  19.03.2002 DJ
    + UpdateIfRequired(), UPDATE_IGNORE_VISIBLE
  14.02.2002 VVM
    ! UpdateIfChanged ��������� �� ������� Force, � �������� �� UIC_*
  09.02.2002 VVM
    ! ProcessDelDisk ����������
      enum {DRIVE_DEL_FAIL, DRIVE_DEL_SUCCESS, DRIVE_DEL_EJECT}
  28.12.2001 DJ
    ! ��������� Del � ���� ������ �������� � ��������� �������
  27.11.2001 SVS
    + GetCurBaseName() ������ �� ���� ��� ��������� ������� ��� ��������
      � ������ ����������� ���������� �������, �.�. ��� ������ ��������
      ����-����� � �����.
  26.09.2001 SVS
    + Panel::NeedUpdatePanel() - ����� �� ��������� ������ � ������ ������
      ��������� Opt.AutoUpdateLimit
  09.08.2001 SVS
    + virtual long GetFileCount() ��� ���� �������� :-)
  22.06.2001 SKV
    + �������� Force � UpdateIfChanged.
  06.05.2001 DJ
    ! �������� #include
  30.04.2001 DJ
    + UpdateKeyBar() - ��������� key bar titles
  25.04.2001 SVS
    + GetRealSelCount() - ������ ������������ ��� ��������.
  24.04.2001 VVM
    + ������� ����� ����������.
  25.02.2001 VVM
    + ���. �������� � ReadDiz - dwFlags
      �� ������ ����� ���� ����� ����
      RDF_NO_UPDATE - �� ��������� GetFindData.
  14.02.2001 SVS
    ! �������������� �������� ��� MakeListFile - ������������
  09.02.2001 IS
    + Get(Set)SelectedFirstMode
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "scrobj.hpp"
#include "farconst.hpp"
#include "struct.hpp"
#include "int64.hpp"

class DizList;

struct PanelViewSettings
{
  unsigned int ColumnType[20];
  int ColumnWidth[20];
  int ColumnCount;
  unsigned int StatusColumnType[20];
  int StatusColumnWidth[20];
  int StatusColumnCount;
  int FullScreen;
  int AlignExtensions;
  int FolderAlignExtensions;
  int FolderUpperCase;
  int FileLowerCase;
  int FileUpperToLowerCase;
  int CaseSensitiveSort;
};

enum {FILE_PANEL,TREE_PANEL,QVIEW_PANEL,INFO_PANEL};

enum {UNSORTED,BY_NAME,BY_EXT,BY_MTIME,BY_CTIME,BY_ATIME,BY_SIZE,BY_DIZ,
      BY_OWNER,BY_COMPRESSEDSIZE,BY_NUMLINKS};

enum {VIEW_0=0,VIEW_1,VIEW_2,VIEW_3,VIEW_4,VIEW_5,VIEW_6,VIEW_7,VIEW_8,VIEW_9};

enum {DRIVE_SHOW_TYPE=1,DRIVE_SHOW_NETNAME=2,DRIVE_SHOW_LABEL=4,
      DRIVE_SHOW_FILESYSTEM=8,DRIVE_SHOW_SIZE=16,DRIVE_SHOW_REMOVABLE=32,
      DRIVE_SHOW_PLUGINS=64,DRIVE_SHOW_CDROM=128};

enum {UPDATE_KEEP_SELECTION=1,UPDATE_SECONDARY=2,UPDATE_IGNORE_VISIBLE=4,UPDATE_DRAW_MESSAGE=8};

enum {NORMAL_PANEL,PLUGIN_PANEL};

enum {DRIVE_DEL_FAIL, DRIVE_DEL_SUCCESS, DRIVE_DEL_EJECT};

enum {UIC_UPDATE_NORMAL, UIC_UPDATE_FORCE, UIC_UPDATE_FORCE_NOTIFICATION};

class VMenu;
class Edit;
class Panel:public ScreenObject
{
  protected:
    char CurDir[NM];
    int Focus;
    int Type;
    int EnableUpdate;
    int PanelMode;
    int SortMode;
    int SortOrder;
    int SortGroups;
    int PrevViewMode,ViewMode;
    long CurTopFile;
    long CurFile;
    int ShowShortNames;
    int NumericSort;
    int DisableOut;
    int ModalMode;
    int PluginCommand;
    BYTE PluginParam[1024];

  public:
    struct PanelViewSettings ViewSettings;
    int ProcessingPluginCommand;

  private:
    int ChangeDiskMenu(int Pos,int FirstCall);
    /* $ 28.12.2001 DJ
       ��������� Del � ���� ������
    */
    int ProcessDelDisk (char Drive, int DriveType,VMenu *ChDiskMenu);
    /* DJ $ */
    void FastFindShow(int FindX,int FindY);
    void FastFindProcessName(Edit *FindEdit,const char *Src,char *LastName,char *Name);
    void DragMessage(int X,int Y,int Move);

  protected:
    void FastFind(int FirstKey);
    void DrawSeparator(int Y);
    void ShowScreensCount();
    int IsDragging();

  public:
    Panel();
    virtual ~Panel();

  public:
    virtual void SetCurDir(char *NewDir,int ClosePlugin);
    virtual void ChangeDirToCurrent();
    virtual void GetCurDir(char *CurDir);
    virtual int GetSelCount() {return(0);};
    virtual int GetRealSelCount() {return(0);};
    virtual int GetSelName(char *Name,int &FileAttr,char *ShortName=NULL) {return(FALSE);};
    virtual void UngetSelName() {};
    virtual void ClearLastGetSelection() {};
    virtual long GetLastSelectedSize(int64 *Size) {return(-1);};
    virtual int GetLastSelectedItem(struct FileListItem *LastItem) {return(0);};
    virtual int GetCurName(char *Name,char *ShortName);
    virtual int GetCurBaseName(char *Name,char *ShortName);
    virtual int GetFileName(char *Name,int Pos,int &FileAttr) {return(FALSE);};
    virtual int GetCurrentPos() {return(0);};
    virtual void SetFocus();
    virtual void KillFocus();
    virtual void Update(int Mode) {};
    /*$ 22.06.2001 SKV
      �������� ��� ������������� ������� ���������� Update.
      ������������ ��� Update ����� ���������� �������.
    */
    virtual int UpdateIfChanged(int UpdateMode) {return(0);};
    /* SKV$*/
    /* $ 19.03.2002 DJ
       UpdateIfRequired() - ��������, ���� ������ ��� �������� ��-�� ����,
       ��� ������ ��������
    */
    virtual void UpdateIfRequired() {};
    /* DJ $ */
    virtual void CloseChangeNotification() {};
    virtual int FindPartName(char *Name,int Next) {return(FALSE);}
    virtual int GoToFile(const char *Name) {return(TRUE);};
    virtual int IsSelected(char *Name) {return(FALSE);};
    /* $ 09.02.2001 IS
       ������� ������������/���������� ��������� ������
       "���������� ����� ������"
    */
    virtual void SetSelectedFirstMode(int) {};
    virtual int GetSelectedFirstMode(void) {return 0;};
    /* IS $ */
    int GetMode() {return(PanelMode);};
    void SetMode(int Mode) {PanelMode=Mode;};
    int GetModalMode() {return(ModalMode);};
    void SetModalMode(int ModalMode) {Panel::ModalMode=ModalMode;};
    int GetViewMode() {return(ViewMode);};
    virtual void SetViewMode(int ViewMode);
    virtual int GetPrevViewMode() {return(PrevViewMode);};
    void SetPrevViewMode(int PrevViewMode) {Panel::PrevViewMode=PrevViewMode;};
    virtual int GetPrevSortMode() {return(SortMode);};
    virtual int GetPrevSortOrder() {return(SortOrder);};
    int GetSortMode() {return(SortMode);};
    int GetNumericSort() { return NumericSort; }
    void SetNumericSort(int Mode) { NumericSort=Mode; }
    virtual void SetSortMode(int SortMode) {Panel::SortMode=SortMode;};
    int GetSortOrder() {return(SortOrder);};
    void SetSortOrder(int SortOrder) {Panel::SortOrder=SortOrder;};
    /* $ 24.04.2001 VVM
      �������� ������� ���������� �� ������ */
    virtual void ChangeSortOrder(int NewOrder) {SetSortOrder(NewOrder);};
    /* VVM $ */
    int GetSortGroups() {return(SortGroups);};
    void SetSortGroups(int SortGroups) {Panel::SortGroups=SortGroups;};
    int GetShowShortNamesMode() {return(ShowShortNames);};
    void SetShowShortNamesMode(int Mode) {ShowShortNames=Mode;};
    void InitCurDir(char *CurDir);
    virtual void CloseFile() {};
    virtual void UpdateViewPanel() {};
    virtual void CompareDir() {};
    virtual void MoveToMouse(MOUSE_EVENT_RECORD *MouseEvent) {};
    virtual void ClearSelection() {};
    virtual void SaveSelection() {};
    virtual void RestoreSelection() {};
    virtual void SortFileList(int KeepPosition) {};
    virtual void EditFilter() {};
    virtual void ReadDiz(struct PluginPanelItem *ItemList=NULL,int ItemLength=0, DWORD dwFlags=0) {};
    virtual void DeleteDiz(char *Name,char *ShortName) {};
    virtual void GetDizName(char *DizName) {};
    virtual void FlushDiz() {};
    virtual void CopyDiz(char *Name,char *ShortName,char *DestName,
                 char *DestShortName,DizList *DestDiz) {};
    virtual int IsFullScreen() {return(FALSE);};
    virtual int IsDizDisplayed() {return(FALSE);};
    virtual int IsColumnDisplayed(int Type) {return(FALSE);};
    virtual void SetReturnCurrentFile(int Mode) {};
    virtual void QViewDelTempName() {};
    virtual void GetPluginInfo(struct PluginInfo *Info) {};
    virtual void GetOpenPluginInfo(struct OpenPluginInfo *Info) {};
    virtual void SetPluginMode(HANDLE hPlugin,char *PluginFile) {};
    virtual void SetPluginModified() {};
    virtual int ProcessPluginEvent(int Event,void *Param) {return(FALSE);};
    virtual HANDLE GetPluginHandle() {return(INVALID_HANDLE_VALUE);};
    virtual void SetTitle();

    virtual void IfGoHome(char Drive){};

    /* $ 30.04.2001 DJ
       ������� ���������� ��� ���������� �������; ���� ���������� FALSE,
       ������������ ����������� ������
    */
    virtual BOOL UpdateKeyBar() { return FALSE; };
    /* DJ $ */
    virtual long GetFileCount() {return 0;}

    static void EndDrag();
    void Hide();
    void Show();
    void SetPluginCommand(int Command,void *Param);
    int PanelProcessMouse(MOUSE_EVENT_RECORD *MouseEvent,int &RetCode);
    void ChangeDisk();
    int GetFocus() {return(Focus);};
    int GetType() {return(Type);};
    void SetUpdateMode(int Mode) {EnableUpdate=Mode;};
    int MakeListFile(char *ListFileName,int ShortNames,char *Modifers=NULL);
    int SetCurPath();

    BOOL NeedUpdatePanel(Panel *AnotherPanel);
};

#endif  // __PANEL_HPP__
