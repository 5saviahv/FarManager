#ifndef __PLUGINS_HPP__
#define __PLUGINS_HPP__
/*
plugins.hpp

������ � ��������� (������ �������, ���-��� ������ � flplugin.cpp)

*/

#include "language.hpp"
#include "bitflags.hpp"
#include "unicodestring.hpp"
#include "struct.hpp"
#include "plugin.hpp"
#include "plclass.hpp"

extern const wchar_t *FmtPluginsCache_PluginDW;
extern const wchar_t *FmtPluginsCache_PluginDExportW;
extern const wchar_t *FmtDiskMenuStringDW;
extern const wchar_t *FmtDiskMenuNumberDW;
extern const wchar_t *FmtPluginMenuStringDW;
extern const wchar_t *FmtPluginConfigStringDW;


class SaveScreen;
class FileEditor;
class Viewer;
class Frame;
class Panel;


// ����� ��� ���� Plugin.WorkFlags
enum PLUGINITEMWORKFLAGS{
  PIWF_CACHED        = 0x00000001, // ����������
  PIWF_PRELOADED     = 0x00000002, //
  PIWF_DONTLOADAGAIN = 0x00000004, // �� ��������� ������ �����, �������� �
                                   //   ���������� �������� ��������� ������ ����
};

// ����� ��� ���� Plugin.FuncFlags - ���������� �������
enum PLUGINITEMCALLFUNCFLAGS{
  PICFF_LOADED               = 0x00000001, // DLL �������� ;-)
  PICFF_SETSTARTUPINFO       = 0x00000002, //
  PICFF_OPENPLUGIN           = 0x00000004, //
  PICFF_OPENFILEPLUGIN       = 0x00000008, //
  PICFF_CLOSEPLUGIN          = 0x00000010, //
  PICFF_GETPLUGININFO        = 0x00000020, //
  PICFF_GETOPENPLUGININFO    = 0x00000040, //
  PICFF_GETFINDDATA          = 0x00000080, //
  PICFF_FREEFINDDATA         = 0x00000100, //
  PICFF_GETVIRTUALFINDDATA   = 0x00000200, //
  PICFF_FREEVIRTUALFINDDATA  = 0x00000400, //
  PICFF_SETDIRECTORY         = 0x00000800, //
  PICFF_GETFILES             = 0x00001000, //
  PICFF_PUTFILES             = 0x00002000, //
  PICFF_DELETEFILES          = 0x00004000, //
  PICFF_MAKEDIRECTORY        = 0x00008000, //
  PICFF_PROCESSHOSTFILE      = 0x00010000, //
  PICFF_SETFINDLIST          = 0x00020000, //
  PICFF_CONFIGURE            = 0x00040000, //
  PICFF_EXITFAR              = 0x00080000, //
  PICFF_PROCESSKEY           = 0x00100000, //
  PICFF_PROCESSEVENT         = 0x00200000, //
  PICFF_PROCESSEDITOREVENT   = 0x00400000, //
  PICFF_COMPARE              = 0x00800000, //
  PICFF_PROCESSEDITORINPUT   = 0x01000000, //
  PICFF_MINFARVERSION        = 0x02000000, //
  PICFF_PROCESSVIEWEREVENT   = 0x04000000, //

  // PICFF_PANELPLUGIN - ������ ������� ������������ � �������� "��� ������"
  PICFF_PANELPLUGIN          = PICFF_OPENFILEPLUGIN|
                               PICFF_GETFINDDATA|
                               PICFF_FREEFINDDATA|
                               PICFF_GETVIRTUALFINDDATA|
                               PICFF_FREEVIRTUALFINDDATA|
                               PICFF_SETDIRECTORY|
                               PICFF_GETFILES|
                               PICFF_PUTFILES|
                               PICFF_DELETEFILES|
                               PICFF_MAKEDIRECTORY|
                               PICFF_PROCESSHOSTFILE|
                               PICFF_SETFINDLIST|
                               PICFF_PROCESSKEY|
                               PICFF_PROCESSEVENT|
                               PICFF_COMPARE|
                               PICFF_GETOPENPLUGININFO,
};

// ����� ��� ���� PluginManager.Flags
enum PLUGINSETFLAGS{
  PSIF_ENTERTOOPENPLUGIN        = 0x00000001, // ��������� � ������ OpenPlugin
  PSIF_DIALOG                   = 0x00000002, // ���� ������ � ��������
  PSIF_PLUGINSLOADDED           = 0x80000000, // ������ ���������
};

class PluginManager
{
public:

	BitFlags Flags;        // ����� �������� ��������
	DWORD Reserved;        // � ������� ��� ����� ���� ������ ������� ������

	Plugin **PluginsData;
	int    PluginsCount;

	Plugin *CurPluginItem;

	FileEditor *CurEditor;
	Viewer *CurViewer;     // 27.09.2000 SVS: ��������� �� ������� Viewer

private:

	void LoadIfCacheAbsent();
	void ReadUserBackgound(SaveScreen *SaveScr);
	int GetHotKeyRegKey(Plugin *pPlugin,int ItemNumber,string &strRegKey);
	BOOL TestPluginInfo(Plugin *Item,PluginInfo *Info);
	BOOL TestOpenPluginInfo(Plugin *Item,OpenPluginInfo *Info);

public:

	PluginManager();
	~PluginManager();

public:

	int LoadPlugin (const wchar_t *lpwszModuleName, const FAR_FIND_DATA_EX *fdata = NULL, Plugin **pLoadedPlugin = NULL);
	int UnloadPlugin (Plugin *pPlugin, DWORD dwException, bool bRemove = false);

	Plugin *GetPlugin (const wchar_t *lpwszModuleName);

	int AddPlugin (Plugin *pPlugin);
	int RemovePlugin (Plugin *pPlugin);

	void LoadPlugins();
	void LoadPluginsFromCache();

	BOOL IsPluginsLoaded() {return Flags.Check(PSIF_PLUGINSLOADDED);}

	void Configure(int StartPos=0);
	void ConfigureCurrent(Plugin *pPlugin,int INum);
	int CommandsMenu(int ModalType,int StartPos,const wchar_t *HistoryName=NULL);
	// $ 21.08.2002 IS - �������� PluginTextSize, ����� �����, ������� �����
	int GetDiskMenuItem(Plugin *pPlugin,int PluginItem,int &ItemPresent,
	int &PluginTextNumber, string &strPluginText);

	int UseFarCommand(HANDLE hPlugin,int CommandType);
	void ReloadLanguage();
	void DiscardCache();
	int ProcessCommandLine(const wchar_t *Command,Panel *Target=NULL);

	// $ .09.2000 SVS - ������� CallPlugin - ����� ������ �� ID � ��������� OpenFrom = OPEN_*
	int CallPlugin(DWORD SysID,int OpenFrom, void *Data);
	Plugin *FindPlugin(DWORD SysID);

	void SetFlags(DWORD NewFlags) { Flags.Set(NewFlags); }
	void SkipFlags(DWORD NewFlags) { Flags.Clear(NewFlags); }
	BOOL CheckFlags(DWORD NewFlags) { return Flags.Check(NewFlags); }

//api functions

public:

	HANDLE OpenPlugin(Plugin *pPlugin,int OpenFrom,INT_PTR Item);
	HANDLE OpenFilePlugin(const wchar_t *Name, const unsigned char *Data, int DataSize, int OpMode);
	HANDLE OpenFindListPlugin(const PluginPanelItem *PanelItem,int ItemsNumber);
	void ClosePlugin(HANDLE hPlugin);
	int GetPluginInfo(Plugin *pPlugin, PluginInfo *Info);
	void GetOpenPluginInfo(HANDLE hPlugin, OpenPluginInfo *Info);
	int GetFindData(HANDLE hPlugin,PluginPanelItem **pPanelItem,int *pItemsNumber,int Silent);
	void FreeFindData(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber);
	int GetVirtualFindData(HANDLE hPlugin,PluginPanelItem **pPanelItem,int *pItemsNumber,const wchar_t *Path);
	void FreeVirtualFindData(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber);
	int SetDirectory(HANDLE hPlugin,const wchar_t *Dir,int OpMode);
	int GetFile(HANDLE hPlugin,PluginPanelItem *PanelItem,const wchar_t *DestPath,string &strResultName,int OpMode);
	int GetFiles(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber,int Move,const wchar_t *DestPath,int OpMode);
	int PutFiles(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber,int Move,int OpMode);
	int DeleteFiles(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
	int MakeDirectory(HANDLE hPlugin,const wchar_t *Name,int OpMode);
	int ProcessHostFile(HANDLE hPlugin,PluginPanelItem *PanelItem,int ItemsNumber,int OpMode);
	int ProcessKey(HANDLE hPlugin,int Key,unsigned int ControlState);
	int ProcessEvent(HANDLE hPlugin,int Event,void *Param);
	int Compare(HANDLE hPlugin,const PluginPanelItem *Item1,const PluginPanelItem *Item2,unsigned int Mode);
	int ProcessEditorInput(INPUT_RECORD *Rec);
	int ProcessEditorEvent(int Event,void *Param);
	int ProcessViewerEvent(int Event,void *Param);

friend class Plugin;
};

#endif  // __PLUGINS_HPP__
