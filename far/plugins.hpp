#ifndef __PLUGINS_HPP__
#define __PLUGINS_HPP__
/*
plugins.hpp

������ � ��������� (������ �������, ���-��� ������ � flplugin.cpp)

*/

/* Revision: 1.10 06.05.2001 $ */

/*
Modify:
  06.05.2001 DJ
    ! �������� #include
  04.05.2001 OT
    + ������� ������������� ���� �������� �� F11 (NWZ)
      ��������� PluginSet::CommandsMenu()
  26.03.2001 SVS
    + �������������� �������� � CommandsMenu() - HistoryName
  31.10.2000 SVS
    + ������� TestOpenPluginInfo - �������� �� �������� ���������� ��������
      ������
  23.10.2000 SVS
    + ������� TestPluginInfo - �������� �� �������� ���������� �������� ������
  12.10.2000 tran 1.05
    + DumpPluginsInfo()
  27.09.2000 SVS
    + ������� CallPlugin - ����� ������ �� ID � ���������
    + CurViewer
    + pProcessViewerEvent
  21.09.2000 SVS
    + ���� SysID - ��������� ������������� �������
  01.09.2000 tran 1.02
    + PluginsSet::LoadPluginsFromCache()
  03.08.2000 tran 1.01
    + GetFarMinVersion
      � ��� ����� ������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "language.hpp"

class SaveScreen;
class Editor;
class Viewer;

typedef void (WINAPI *PLUGINSETSTARTUPINFO)(struct PluginStartupInfo *Info);
typedef HANDLE (WINAPI *PLUGINOPENPLUGIN)(int OpenFrom,int Item);
typedef HANDLE (WINAPI *PLUGINOPENFILEPLUGIN)(char *Name,const unsigned char *Data,int DataSize);
typedef void (WINAPI *PLUGINCLOSEPLUGIN)(HANDLE hPlugin);
typedef void (WINAPI *PLUGINGETPLUGININFO)(struct PluginInfo *Info);
typedef void (WINAPI *PLUGINGETOPENPLUGININFO)(HANDLE hPlugin,struct OpenPluginInfo *Info);
typedef int (WINAPI *PLUGINGETFINDDATA)(HANDLE hPlugin,struct PluginPanelItem **pPanelItem,int *pItemsNumber,int OpMode);
typedef void (WINAPI *PLUGINFREEFINDDATA)(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber);
typedef int (WINAPI *PLUGINGETVIRTUALFINDDATA)(HANDLE hPlugin,struct PluginPanelItem **pPanelItem,int *pItemsNumber,char *Path);
typedef void (WINAPI *PLUGINFREEVIRTUALFINDDATA)(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber);
typedef int (WINAPI *PLUGINSETDIRECTORY)(HANDLE hPlugin,char *Dir,int OpMode);
typedef int (WINAPI *PLUGINGETFILES)(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int Move,char *DestPath,int OpMode);
typedef int (WINAPI *PLUGINPUTFILES)(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int Move,int OpMode);
typedef int (WINAPI *PLUGINDELETEFILES)(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
typedef int (WINAPI *PLUGINMAKEDIRECTORY)(HANDLE hPlugin,char *Name,int OpMode);
typedef int (WINAPI *PLUGINPROCESSHOSTFILE)(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
typedef int (WINAPI *PLUGINSETFINDLIST)(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber);
typedef int (WINAPI *PLUGINPROCESSKEY)(HANDLE hPlugin,int Key,unsigned int ControlState);
typedef int (WINAPI *PLUGINPROCESSEVENT)(HANDLE hPlugin,int Event,void *Param);
typedef int (WINAPI *PLUGINPROCESSEDITOREVENT)(int Event,void *Param);
typedef int (WINAPI *PLUGINCONFIGURE)(int ItemNumber);
typedef void (WINAPI *PLUGINEXITFAR)();
typedef int (WINAPI *PLUGINCOMPARE)(HANDLE hPlugin,struct PluginPanelItem *Item1,struct PluginPanelItem *Item2,unsigned int Mode);
typedef int (WINAPI *PLUGINPROCESSEDITORINPUT)(INPUT_RECORD *Rec);
typedef int (WINAPI *PLUGINMINFARVERSION)();
/* $ 27.09.2000 SVS
   ������� �� �������
*/
typedef int (WINAPI *PLUGINPROCESSVIEWEREVENT)(int Event,void *Param);
/* SVS $ */

struct PluginItem
{
  char ModuleName[NM];
  HMODULE hModule;
  WIN32_FIND_DATA FindData;
  Language Lang;
  /* $ 21.09.2000 SVS
     ���� - ��������� ������������� �������
     ������ ������ ��� ��������, �������� ���
     Network      = 0x5774654E (NetW)
     PrintManager = 0x6E614D50 (PMan)  SYSID_PRINTMANAGER
  */
  DWORD SysID;
  /* SVS $ */
  int Cached;
  int CachePos;
  int EditorPlugin;
  /* $ 03.08.2000 tran
     ���� - �� ��������� ������ �����
     �������� � ���������� �������� ��������� ������ ���� */
  int DontLoadAgain;
  /* tran 03.08.2000 $ */
  char RootKey[512];
  PLUGINSETSTARTUPINFO pSetStartupInfo;
  PLUGINOPENPLUGIN pOpenPlugin;
  PLUGINOPENFILEPLUGIN pOpenFilePlugin;
  PLUGINCLOSEPLUGIN pClosePlugin;
  PLUGINGETPLUGININFO pGetPluginInfo;
  PLUGINGETOPENPLUGININFO pGetOpenPluginInfo;
  PLUGINGETFINDDATA pGetFindData;
  PLUGINFREEFINDDATA pFreeFindData;
  PLUGINGETVIRTUALFINDDATA pGetVirtualFindData;
  PLUGINFREEVIRTUALFINDDATA pFreeVirtualFindData;
  PLUGINSETDIRECTORY pSetDirectory;
  PLUGINGETFILES pGetFiles;
  PLUGINPUTFILES pPutFiles;
  PLUGINDELETEFILES pDeleteFiles;
  PLUGINMAKEDIRECTORY pMakeDirectory;
  PLUGINPROCESSHOSTFILE pProcessHostFile;
  PLUGINSETFINDLIST pSetFindList;
  PLUGINCONFIGURE pConfigure;
  PLUGINEXITFAR pExitFAR;
  PLUGINPROCESSKEY pProcessKey;
  PLUGINPROCESSEVENT pProcessEvent;
  PLUGINPROCESSEDITOREVENT pProcessEditorEvent;
  PLUGINCOMPARE pCompare;
  PLUGINPROCESSEDITORINPUT pProcessEditorInput;
  PLUGINMINFARVERSION pMinFarVersion;
  PLUGINPROCESSVIEWEREVENT pProcessViewerEvent;
};

class PluginsSet
{
  public:
    struct PluginItem *PluginsData;
    int PluginsCount;

    Editor *CurEditor;
    /* $ 27.09.2000 SVS
       ����������� ������� Viewer
    */
    Viewer *CurViewer;
    /* SVS $*/

  private:
    int LoadPlugin(struct PluginItem &CurPlugin,int ModuleNumber,int Init);
    void SetPluginStartupInfo(struct PluginItem &CurPlugin,int ModuleNumber);
    int PreparePlugin(int PluginNumber);
    int GetCacheNumber(char *FullName,WIN32_FIND_DATA *FindData,int CachePos);
    int SavePluginSettings(struct PluginItem &CurPlugin,WIN32_FIND_DATA &FindData);
    void LoadIfCacheAbsent();
    void ReadUserBackgound(SaveScreen *SaveScr);
    int GetHotKeyRegKey(int PluginNumber,int ItemNumber,char *RegKey);
    BOOL TestPluginInfo(struct PluginItem& Item,struct PluginInfo *Info);
    BOOL TestOpenPluginInfo(struct PluginItem& Item,struct OpenPluginInfo *Info);

  public:
    PluginsSet();
    ~PluginsSet();

  public:
    void LoadPlugins();
    void LoadPluginsFromCache();
    HANDLE OpenPlugin(int PluginNumber,int OpenFrom,int Item);
    HANDLE OpenFilePlugin(char *Name,const unsigned char *Data,int DataSize);
    HANDLE OpenFindListPlugin(PluginPanelItem *PanelItem,int ItemsNumber);
    void ClosePlugin(HANDLE hPlugin);
    int GetPluginInfo(int PluginNumber,struct PluginInfo *Info);
    void GetOpenPluginInfo(HANDLE hPlugin,struct OpenPluginInfo *Info);
    int GetFindData(HANDLE hPlugin,PluginPanelItem **pPanelItem,int *pItemsNumber,int Silent);
    void FreeFindData(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber);
    int GetVirtualFindData(HANDLE hPlugin,PluginPanelItem **pPanelItem,int *pItemsNumber,char *Path);
    void FreeVirtualFindData(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber);
    int SetDirectory(HANDLE hPlugin,char *Dir,int OpMode);
    int GetFile(HANDLE hPlugin,struct PluginPanelItem *PanelItem,char *DestPath,char *ResultName,int OpMode);
    int GetFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int Move,char *DestPath,int OpMode);
    int PutFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int Move,int OpMode);
    int DeleteFiles(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
    int MakeDirectory(HANDLE hPlugin,char *Name,int OpMode);
    int ProcessHostFile(HANDLE hPlugin,struct PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
    int ProcessKey(HANDLE hPlugin,int Key,unsigned int ControlState);
    int ProcessEvent(HANDLE hPlugin,int Event,void *Param);
    int Compare(HANDLE hPlugin,struct PluginPanelItem *Item1,struct PluginPanelItem *Item2,unsigned int Mode);
    int ProcessEditorInput(INPUT_RECORD *Rec);
    void ProcessEditorEvent(int Event,void *Param);
    void ProcessViewerEvent(int Event,void *Param);
    void SendExit();
    char* FarGetMsg(int PluginNumber,int MsgId);
    void Configure();
    int CommandsMenu(int ModalType,int StartPos,char *HistoryName=NULL);
    int GetDiskMenuItem(int PluginNumber,int PluginItem,int &ItemPresent,int &PluginTextNumber,char *PluginText);
    int UseFarCommand(HANDLE hPlugin,int CommandType);
    void ReloadLanguage();
    void DiscardCache();
    int ProcessCommandLine(char *Command);

    /* $ 03.08.2000 tran
       ����� ������ ��� �������� ����������� ������ */
    int  CheckMinVersion(struct PluginItem &CurPlg);
    void UnloadPlugin(struct PluginItem &CurPlg);
    void ShowMessageAboutIllegialPluginVersion(char* plg,int required);
    /* tran 03.08.2000 $ */

    /* $ .09.2000 SVS
      ������� CallPlugin - ����� ������ �� ID � ���������
      OpenFrom = OPEN_*
    */
    int CallPlugin(DWORD SysID,int OpenFrom, void *Data);
    int FindPlugin(DWORD SysID);
    /* SVS $ */

    /* $ 12.10.2000 tran
      new method - for debugging purpose */
    void DumpPluginsInfo();
    /* tran $ */
};

/* $ 17.10.2000 SVS
   ����������!
*/
enum ExceptFunctionsType{
  EXCEPT_SETSTARTUPINFO,
  EXCEPT_GETVIRTUALFINDDATA,
  EXCEPT_OPENPLUGIN,
  EXCEPT_OPENFILEPLUGIN,
  EXCEPT_OPENPLUGIN_FINDLIST,
  EXCEPT_CLOSEPLUGIN,
  EXCEPT_GETPLUGININFO,
  EXCEPT_GETPLUGININFO_DATA,
  EXCEPT_GETOPENPLUGININFO,
  EXCEPT_GETOPENPLUGININFO_DATA,
  EXCEPT_GETFINDDATA,
  EXCEPT_FREEFINDDATA,
  EXCEPT_FREEVIRTUALFINDDATA,
  EXCEPT_SETDIRECTORY,
  EXCEPT_GETFILES,
  EXCEPT_PUTFILES,
  EXCEPT_DELETEFILES,
  EXCEPT_MAKEDIRECTORY,
  EXCEPT_PROCESSHOSTFILE,
  EXCEPT_SETFINDLIST,
  EXCEPT_CONFIGURE,
  EXCEPT_EXITFAR,
  EXCEPT_PROCESSKEY,
  EXCEPT_PROCESSEVENT,
  EXCEPT_PROCESSEDITOREVENT,
  EXCEPT_COMPARE,
  EXCEPT_PROCESSEDITORINPUT,
  EXCEPT_MINFARVERSION,
  EXCEPT_PROCESSVIEWEREVENT,
  EXCEPT_PROCESSVIEWERINPUT,
  EXCEPT_FARDIALOG,
};



#endif  // __PLUGINS_HPP__
