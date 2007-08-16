#ifndef __FILEEDITOR_HPP__
#define __FILEEDITOR_HPP__
/*
fileedit.hpp

�������������� ����� - ���������� ��� editor.cpp

*/

#include "frame.hpp"
#include "editor.hpp"
#include "keybar.hpp"

class NamesList;

// ���� �������� Editor::SaveFile()
enum {
    SAVEFILE_ERROR   = 0,         // �������� ���������, �� ����������
    SAVEFILE_SUCCESS = 1,         // ���� ������� ���������, ���� ��������� ���� �� ����
    SAVEFILE_CANCEL  = 2          // ���������� ��������, �������� �� ���������
};

// ��� ���������
enum FEOPMODEEXISTFILE{
  FEOPMODE_QUERY        =0x00000000,
  FEOPMODE_NEWIFOPEN    =0x10000000,
  FEOPMODE_USEEXISTING  =0x20000000,
  FEOPMODE_BREAKIFOPEN  =0x30000000,
  FEOPMODE_RELOAD       =0x40000000,
};

enum FFILEEDIT_FLAGS{
  FFILEEDIT_NEW                   = 0x00010000,  // ���� ���� ����������! ����� ��� ��� ������ �������! ���� ������ � ��� ���.
  FFILEEDIT_REDRAWTITLE           = 0x00020000,  // ����� ��������� ���������?
  FFILEEDIT_FULLSCREEN            = 0x00040000,  // ������������� �����?
  FFILEEDIT_DISABLEHISTORY        = 0x00080000,  // ��������� ������ � �������?
  FFILEEDIT_ENABLEF6              = 0x00100000,  // ������������� �� ������ �����?
  FFILEEDIT_SAVETOSAVEAS          = 0x00200000,  // $ 17.08.2001 KM  ��������� ��� ������ �� AltF7.
                                                 //   ��� �������������� ���������� ����� �� ������ ���
                                                 //   ������� F2 ������� ����� ShiftF2.
  FFILEEDIT_SAVEWQUESTIONS        = 0x00400000,  // ��������� ��� ��������
  FFILEEDIT_LOCKED                = 0x00800000,  // �������������?
  FFILEEDIT_OPENFAILED            = 0x01000000,  // ���� ������� �� �������
  FFILEEDIT_DELETEONCLOSE         = 0x02000000,  // ������� � ����������� ���� ������ � ��������� (���� ��� ����)
  FFILEEDIT_DELETEONLYFILEONCLOSE = 0x04000000,  // ������� � ����������� ������ ����
  FFILEEDIT_CANNEWFILE            = 0x10000000,  // ����������� ����� ����?
  FFILEEDIT_SERVICEREGION         = 0x20000000,  // ������������ ��������� �������
};


class FileEditor : public Frame
{
private:

    Editor *m_editor;
    KeyBar EditKeyBar;

    NamesList *EditNamesList;

    string strFileName;
    string strFullFileName;

    string strStartDir;

    string strTitle;
    string strPluginTitle;

    string strPluginData;

    FAR_FIND_DATA_EX FileInfo;

    char AttrStr[4];               // 13.02.2001 IS - ���� �������� ����� ���������, ����� �� ��������� �� ����� ���
    DWORD FileAttributes;          // 12.02.2001 IS - ���� �������� �������� ����� ��� ��������, ���������� ���-������...
    BOOL  FileAttributesModified;  // 04.11.2003 SKV - ���� �� ��������������� ��������� ��� save

    DWORD SysErrorCode;

    bool m_bClosing;               // 28.04.2005 AY: true ����� �������� ������������ (�.�. � �����������)

    bool bEE_READ_Sent;

    bool m_bSignatureFound;

    int m_codepage; //BUGBUG

private:
    virtual void DisplayObject();
    int  ProcessQuitKey(int FirstSave,BOOL NeedQuestion=TRUE);
    BOOL UpdateFileList();
    /* $ 10.10.2001 IS ��������� DeleteOnClose */
    /* $ 14.06.2002 IS
        DeleteOnClose ���� int:
          0 - �� ������� ������
          1 - ������� ���� � �������
          2 - ������� ������ ����
    */
    void SetDeleteOnClose(int NewMode);
    int ReProcessKey(int Key,int CalledFromControl=TRUE);

public:
    FileEditor(const wchar_t *Name, int codepage, DWORD InitFlags,int StartLine=-1,int StartChar=-1,const wchar_t *PluginData=NULL,int OpenModeExstFile=FEOPMODE_QUERY);
    /* $ 14.06.2002 IS
       DeleteOnClose ���� int:
         0 - �� ������� ������
         1 - ������� ���� � �������
         2 - ������� ������ ����
    */
    FileEditor(const wchar_t *Name, int codepage, DWORD InitFlags,int StartLine,int StartChar,const wchar_t *Title,int X1,int Y1,int X2,int Y2,int DeleteOnClose=0,int OpenModeExstFile=FEOPMODE_QUERY);
    /* IS $ */
    /* $ 07.05.2001 DJ */
    virtual ~FileEditor();
    /* DJ $ */


public:
    /* $ 14.06.2002 IS
       DeleteOnClose ���� int:
         0 - �� ������� ������
         1 - ������� ���� � �������
         2 - ������� ������ ����
    */
	void Init(
			const wchar_t *Name,
			int codepage,
			const wchar_t *Title,
			DWORD InitFlags,
			int StartLine,
			int StartChar,
			const wchar_t *PluginData,
			int DeleteOnClose,
			int OpenModeExstFile
			);

    virtual void InitKeyBar(void);                            // $ 07.08.2000 SVS - ������� ������������� KeyBar Labels
    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    virtual int VMProcess(int OpCode,void *vParam=NULL,__int64 iParam=0);
    virtual void ShowConsoleTitle();
    int IsFileChanged() {return(m_editor->IsFileChanged());};
    virtual int IsFileModified() {return(m_editor->IsFileModified());};

    virtual void SetScreenPosition();                         // $ 28.06.2000 tran - NT Console resize - resize editor

    virtual int GetTypeAndName(string &strType, string &strName);
    virtual const wchar_t *GetTypeName(){return L"[FileEdit]";};
    virtual int GetType() { return MODALTYPE_EDITOR; }


    virtual void OnDestroy();                         // $ 10.05.2001 DJ
    void SetNamesList (NamesList *Names);             // $ 07.05.2001 DJ

    virtual int GetCanLoseFocus(int DynamicMode=FALSE);

    virtual int FastHide();                                   // $ OT - ������� ��� ���� CtrlAltShift

    void SetEnableF6 (int AEnableF6) { Flags.Change(FFILEEDIT_ENABLEF6,AEnableF6); InitKeyBar(); }  // $ 10.05.2001 DJ
    /* $ 17.08.2001 KM
      ��������� ��� ������ �� AltF7. ��� �������������� ���������� ����� ��
      ������ ��� ������� F2 ������� ����� ShiftF2.
    */
    void SetSaveToSaveAs(int ToSaveAs) { Flags.Change(FFILEEDIT_SAVETOSAVEAS,ToSaveAs); InitKeyBar(); }
    /* KM $ */

    /* $ 08.12.2001 OT
      ���������� ������� ����, �������� �� ���� ���������
      ������������ ��� �������� ������� ���������� � ������� �� CtrlF10*/
    BOOL isTemporary();
    virtual void ResizeConsole();
    virtual void Show();

    int LoadFile(const wchar_t *Name, int &UserBreak);

    //TextFormat � Codepage ������������ ������, ���� bSaveAs = true!
    int SaveFile(const wchar_t *Name, int Ask, bool bSaveAs, int TextFormat = 0, int Codepage = 0);

    int EditorControl(int Command,void *Param);
    void SetPluginTitle(const wchar_t *PluginTitle);
    void SetTitle(const wchar_t *Title);
    virtual void GetTitle(string &Title,int SubLen=-1,int TruncSize=0);
    BOOL SetFileName(const wchar_t *NewFileName);
    int ProcessEditorInput(INPUT_RECORD *Rec);
    void SetLockEditor(BOOL LockMode);
    BOOL IsFullScreen(){return Flags.Check(FFILEEDIT_FULLSCREEN);}
    void ChangeEditKeyBar();
    void ShowStatus();

    DWORD GetFileAttributes(const wchar_t *Name);                 // $ 13.02.2001 IS - ������� ������ ����������� ������� �� win32 api

    void SetPluginData(const wchar_t *PluginData);
    const wchar_t *GetPluginData(void){return (const wchar_t*)strPluginData;};

    void GetEditorOptions(struct EditorOptions& EdOpt);
    void SetEditorOptions(struct EditorOptions& EdOpt);

    bool LoadFromCache (EditorCacheParams *pp);
    void SaveToCache ();

    void SetCodePage (int codepage); //BUGBUG
};

bool dlgOpenEditor (string &strFileName, int &codepage);

#endif  // __FILEEDITOR_HPP__
