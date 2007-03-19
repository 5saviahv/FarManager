#ifndef __FILELIST_HPP__
#define __FILELIST_HPP__
/*
filelist.hpp

�������� ������ - ����� �������

*/

#include "global.hpp"
#include "panel.hpp"
#include "dizlist.hpp"

class FileFilter;

struct FileListItem
{
  char Selected;
  char PrevSelected;
  char ShowFolderSize;
  char ShortNamePresent;
  struct HighlightDataColor Colors;

  DWORD NumberOfLinks;
  DWORD UserFlags;
  DWORD_PTR UserData;

  int Position;
  int SortGroup;
  wchar_t *DizText;
  char DeleteDiz;
  string strOwner;
  wchar_t **CustomColumnData;
  int CustomColumnNumber;
  DWORD CRC32;

  //BUGBUG!!
  DWORD FileAttr;
  FILETIME CreationTime;
  FILETIME AccessTime;
  FILETIME WriteTime;

  unsigned __int64 UnpSize;
  unsigned __int64 PackSize;

  string strName;
  string strShortName;

  void Clear ()
  {
  	Selected = 0;
  	PrevSelected = 0;
    ShowFolderSize = 0;
    ShortNamePresent = 0;

    memset (&Colors, 0, sizeof (HighlightDataColor));

	NumberOfLinks = 0;
	UserFlags = 0;
    UserData = 0;

	Position = 0;
	SortGroup = 0;

	DizText = NULL;
	DeleteDiz = 0;

	strOwner = L"";

	CustomColumnData = NULL;
	CustomColumnNumber = 0;

	CRC32 = 0;

	FileAttr = 0;

	memset (&CreationTime, 0, sizeof (CreationTime));
	memset (&AccessTime, 0, sizeof (AccessTime));
	memset (&WriteTime, 0, sizeof (WriteTime));

	UnpSize = 0;
	PackSize = 0;

	strName = L"";
	strShortName = L"";
  }

  FileListItem& operator=(const FileListItem &fliCopy)
  {
  	Selected = fliCopy.Selected;
  	PrevSelected = fliCopy.PrevSelected;
    ShowFolderSize = fliCopy.ShowFolderSize;
    ShortNamePresent = fliCopy.ShortNamePresent;

    memcpy (&Colors, &fliCopy.Colors, sizeof (HighlightDataColor));

	NumberOfLinks = fliCopy.NumberOfLinks;
	UserFlags = fliCopy.UserFlags;
    UserData = fliCopy.UserData;

	Position = fliCopy.Position;
	SortGroup = fliCopy.SortGroup;

	DizText = fliCopy.DizText;
	DeleteDiz = fliCopy.DeleteDiz;

	strOwner = fliCopy.strOwner;

	CustomColumnData = fliCopy.CustomColumnData;
	CustomColumnNumber = fliCopy.CustomColumnNumber;

	CRC32 = fliCopy.CRC32;

	FileAttr = fliCopy.FileAttr;

	memcpy (&CreationTime, &fliCopy.CreationTime, sizeof (CreationTime));
	memcpy (&AccessTime, &fliCopy.AccessTime, sizeof (AccessTime));
	memcpy (&WriteTime, &fliCopy.WriteTime, sizeof (WriteTime));

	UnpSize = fliCopy.UnpSize;
	PackSize = fliCopy.PackSize;

	strName = fliCopy.strName;
	strShortName = fliCopy.strShortName;

	return *this;
  }
};

struct PluginsStackItem
{
  HANDLE hPlugin;
  string strHostFile;
  int Modified;
  int PrevViewMode;
  int PrevSortMode;
  int PrevSortOrder;
  int PrevNumericSort;
  struct PanelViewSettings PrevViewSettings;
};

struct PrevDataItem
{
  struct FileListItem **PrevListData;
  long PrevFileCount;
  string strPrevName;
  long PrevTopFile;
};

enum {NAME_COLUMN=0,SIZE_COLUMN,PACKED_COLUMN,DATE_COLUMN,TIME_COLUMN,
      MDATE_COLUMN,CDATE_COLUMN,ADATE_COLUMN,ATTR_COLUMN,DIZ_COLUMN,
      OWNER_COLUMN,NUMLINK_COLUMN,
      CUSTOM_COLUMN0,CUSTOM_COLUMN1,CUSTOM_COLUMN2,CUSTOM_COLUMN3,
      CUSTOM_COLUMN4,CUSTOM_COLUMN5,CUSTOM_COLUMN6,CUSTOM_COLUMN7,
      CUSTOM_COLUMN8,CUSTOM_COLUMN9};

class FileList:public Panel
{
  private:
    FileFilter *Filter;
    DizList Diz;
    int DizRead;
    /* $ 09.11.2001 IS
         ����������� � ����������� ������, ������� ������������ ��� ������
         �����, ������� �� ���������� � ������. �� ��������� - �������� ������.
    */
    wchar_t openBracket[2], closeBracket[2];
    /* IS $ */
    string strPluginDizName;
    struct FileListItem **ListData;
    long FileCount;
    HANDLE hPlugin;
    struct PrevDataItem **PrevDataStack;
    int PrevDataStackSize;
    PluginsStackItem **PluginsStack;
    int PluginsStackSize;
    HANDLE hListChange;
    long UpperFolderTopFile,LastCurFile;
    long ReturnCurrentFile;
    long SelFileCount;
    long GetSelPosition,LastSelPosition;
    long TotalFileCount;
    unsigned __int64 SelFileSize;
    unsigned __int64 TotalFileSize;
    unsigned __int64 FreeDiskSize;
    clock_t LastUpdateTime;
    int Height,Columns;

    int ColumnsInGlobal;

    int LeftPos;
    int ShiftSelection;
    int MouseSelection;
    int SelectedFirst;
    /* $ 11.09.2000 SVS
       ���������� IsEmpty, ����������� �� ��������� ������ �������
    */
    int IsEmpty;
    /* SVS $ */
    int AccessTimeUpdateRequired;

    int DataToDeleteCount;
    int UpdateRequired,UpdateRequiredMode;
    int SortGroupsRead;
    int InternalProcessKey;

    BOOL Is_FS_NTFS;

  private:
    /* $ 09.02.2001 IS
       ������� ������������/���������� ��������� ������
       "���������� ����� ������"
    */
    virtual void SetSelectedFirstMode(int Mode);
    virtual int GetSelectedFirstMode(void) {return SelectedFirst;};
    /* IS $ */
    virtual void DisplayObject();
    void DeleteListData(struct FileListItem **(&ListData),long &FileCount);
    void Up(int Count);
    void Down(int Count);
    void Scroll(int Count);
    void CorrectPosition();
    void ShowFileList(int Fast);
    void ShowList(int ShowStatus,int StartColumn);
    void SetShowColor(int Position, int ColorType=HIGHLIGHTCOLORTYPE_FILE);
    int  GetShowColor(int Position, int ColorType);
    void ShowSelectedSize();
    void ShowTotalSize(struct OpenPluginInfoW &Info);
    int ConvertNameW (const wchar_t *SrcName, string &strDest, int MaxLength, int RightAlign, int ShowStatus, DWORD dwFileAttr);

    void Select(struct FileListItem *SelPtr,int Selection);
    void SelectFiles(int Mode);
    void ProcessEnter(int EnableExec,int SeparateWindow);
    /* $ 09.04.2001 SVS
       ChangeDir ���������� FALSE, e��� �������� ������ ���� �������
    */
    BOOL ChangeDirW(const wchar_t *NewDir,BOOL IsUpdated=TRUE);
    /* SVS $ */
    void CountDirSize(DWORD PluginFlags);
    /* $ 19.03.2002 DJ
       IgnoreVisible - ��������, ���� ���� ������ ��������
    */
    void ReadFileNames(int KeepSelection, int IgnoreVisible, int DrawMessage);
    void UpdatePlugin(int KeepSelection, int IgnoreVisible);
    /* DJ $ */
    void MoveSelection(struct FileListItem **FileList,long FileCount,
                       struct FileListItem **OldList,long OldFileCount);
    virtual int GetSelCount();
    virtual int GetSelNameW(string *strName,int &FileAttr,string *strShortName=NULL,FAR_FIND_DATA_EX *fd=NULL);
    virtual void UngetSelName();
    virtual void ClearLastGetSelection();

    virtual unsigned __int64 GetLastSelectedSize ();
    virtual int GetLastSelectedItem(struct FileListItem *LastItem);

    virtual int GetCurNameW(string &strName, string &strShortName);
    virtual int GetCurBaseNameW(string &strName, string &strShortName);

    void PushPlugin(HANDLE hPlugin,const wchar_t *HostFile);
    int PopPlugin(int EnableRestoreViewMode);
    void CopyNames(int FillPathName=FALSE,int UNC=FALSE);
    void SelectSortMode();
    void ApplyCommand();
    void DescribeFiles();
    void CreatePluginItemList(struct PluginPanelItemW *(&ItemList),int &ItemNumber,BOOL AddTwoDot=TRUE);
    void DeletePluginItemList(struct PluginPanelItemW *(&ItemList),int &ItemNumber);
    HANDLE OpenPluginForFile(const wchar_t *FileName,DWORD FileAttr=0);
    int PreparePanelView(struct PanelViewSettings *PanelView);
    int PrepareColumnWidths(unsigned int *ColumnTypes,int *ColumnWidths,
                            int &ColumnCount,int FullScreen);
    void PrepareViewSettings(int ViewMode,struct OpenPluginInfoW *PlugInfo);

    void PluginDelete();
    void PutDizToPlugin(FileList *DestPanel,struct PluginPanelItemW *ItemList,
                        int ItemNumber,int Delete,int Move,DizList *SrcDiz,
                        DizList *DestDiz);
    void PluginGetFiles(const wchar_t *DestPath,int Move);
    void PluginToPluginFiles(int Move);
    void PluginHostGetFiles();
    void PluginPutFilesToNew();
    // ���������� ��, ��� ���������� PutFiles
    int PluginPutFilesToAnother(int Move,Panel *AnotherPanel);
    void ProcessPluginCommand();
    void PluginClearSelection(struct PluginPanelItemW *ItemList,int ItemNumber);
    void ProcessCopyKeys(int Key);
    void ReadSortGroups();
    void AddParentPoint(struct FileListItem *CurPtr,long CurFilePos);
    int  ProcessOneHostFile(int Idx);

    static void TextToViewSettings(const wchar_t *ColumnTitles,const wchar_t *ColumnWidths,
           unsigned int *ViewColumnTypes,int *ViewColumnWidths,int &ColumnCount);
    static void ViewSettingsToText(unsigned int *ViewColumnTypes,
           int *ViewColumnWidths,int ColumnCount,string &strColumnTitles,
           string &strColumnWidths);

  public:
    FileList();
    virtual ~FileList();

  public:
    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    virtual void MoveToMouse(MOUSE_EVENT_RECORD *MouseEvent);
    virtual void SetFocus();
    virtual void Update(int Mode);
    /*$ 22.06.2001 SKV
      �������� ��� ������������� ������� ���������� Update.
      ������������ ��� Update ����� ���������� �������.
    */
    virtual int UpdateIfChanged(int UpdateMode);
    /* SKV$*/

    /* $ 19.03.2002 DJ
       UpdateIfRequired() - ��������, ���� ������ ��� �������� ��-�� ����,
       ��� ������ ��������
    */
    virtual void UpdateIfRequired();
    /* DJ $ */

    virtual int SendKeyToPlugin(DWORD Key,BOOL Pred=FALSE);
    void CreateChangeNotification(int CheckTree);
    virtual void CloseChangeNotification();
    virtual void SortFileList(int KeepPosition);
    virtual void SetViewMode(int ViewMode);
    virtual void SetSortMode(int SortMode);
    virtual void ChangeSortOrder(int NewOrder);
    virtual void SetCurDirW(const wchar_t *NewDir,int ClosePlugin);
    virtual int GetPrevSortMode();
    virtual int GetPrevSortOrder();
    virtual int GetPrevViewMode();
    virtual int GetPrevNumericSort();
    HANDLE OpenFilePlugin(const wchar_t *FileName,int PushPrev);
    virtual int GetFileNameW(string &strName,int Pos,int &FileAttr);
    virtual int GetCurrentPos();
    virtual int FindPartName(const wchar_t *Name,int Next,int Direct=1);
    int FindFile(const char *Name,BOOL OnlyPartName=FALSE);

    virtual int GoToFileW(const wchar_t *Name,BOOL OnlyPartName=FALSE);
    virtual int FindFileW(const wchar_t *Name,BOOL OnlyPartName=FALSE);

    int IsSelected(char *Name);
    virtual int IsSelectedW(const wchar_t *Name);

    virtual int FindFirstW(const wchar_t *Name);
    virtual int FindNextW(int StartPos, const wchar_t *Name);

    void ProcessHostFile();
    virtual void UpdateViewPanel();
    virtual void CompareDir();
    virtual void ClearSelection();
    virtual void SaveSelection();
    virtual void RestoreSelection();
    virtual void EditFilter();
    virtual void ReadDiz(struct PluginPanelItemW *ItemList=NULL,int ItemLength=0, DWORD dwFlags=0);
    virtual void DeleteDiz(const wchar_t *Name, const wchar_t *ShortName);
    virtual void FlushDiz();
    virtual void GetDizName(string &strDizName);
    virtual void CopyDiz(const wchar_t *Name, const wchar_t *ShortName, const wchar_t *DestName,
                 const wchar_t *DestShortName,DizList *DestDiz);
    virtual int IsFullScreen();
    int IsCaseSensitive();
    virtual int IsDizDisplayed();
    virtual int IsColumnDisplayed(int Type);
    virtual int GetColumnsCount(){ return Columns;};
    virtual void SetReturnCurrentFile(int Mode);
    virtual void GetPluginInfo(struct PluginInfoW *Info);
    virtual void GetOpenPluginInfo(struct OpenPluginInfoW *Info);
    virtual void SetPluginMode(HANDLE hPlugin,const wchar_t *PluginFile);
    void PluginGetPanelInfo(struct PanelInfo *Info,int FullInfo=TRUE);
    void PluginSetSelection(struct PanelInfo *Info);
    virtual void SetPluginModified();
    virtual int ProcessPluginEvent(int Event,void *Param);
    virtual void SetTitle();
    //virtual void GetTitle(string &Title,int SubLen=-1,int TruncSize=0);
    int PluginPanelHelp(HANDLE hPlugin);
    virtual long GetFileCount() {return FileCount;}

    string &CreateFullPathNameW(const wchar_t *Name,const wchar_t *ShortName,DWORD FileAttr, string &strDest,int UNC,int ShortNameAsIs=TRUE);


    virtual BOOL GetItem(int Index,void *Dest);
    /* $ 30.04.2001 DJ
       �������� UpdateKeyBar()
    */
    virtual BOOL UpdateKeyBar();
    /* DJ $ */
    void UpdateColorItems(void);

    virtual void IfGoHomeW(wchar_t Drive);

    /* 14.05.2002 VVM
      + �������� ����� ���������� ���������� ������ */
    void ResetLastUpdateTime() {LastUpdateTime = 0;}
    /* VVM $ */
    virtual HANDLE GetPluginHandle();
    virtual int GetRealSelCount();
    static void SetFilePanelModes();
    static void SavePanelModes();
    static void ReadPanelModes();
    static int FileNameToPluginItem(const wchar_t *Name,PluginPanelItemW *pi);
    static void FileListToPluginItem(struct FileListItem *fi,struct PluginPanelItemW *pi);
    static void PluginToFileListItem(struct PluginPanelItemW *pi,struct FileListItem *fi);
    static int IsModeFullScreen(int Mode);
    static string &AddPluginPrefix(FileList *SrcPanel,string &strPrefix);
};

#endif  // __FILELIST_HPP__
