#ifndef __FILEEDITOR_HPP__
#define __FILEEDITOR_HPP__
/*
fileedit.hpp

������஢���� 䠩�� - �����ன�� ��� editor.cpp

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class FileEditor:public Modal
{
  private:
    void Process();
    void Show();
    void DisplayObject();

    Editor FEdit;
    KeyBar EditKeyBar;
    char FileName[NM];
    char FullFileName[NM];
    char StartDir[NM];
    char NewTitle[NM];
    int FullScreen;
    int ExitCode;
  public:
    FileEditor(char *Name,int CreateNewFile,int EnableSwitch,
               int StartLine=-1,int StartChar=-1,int DisableHistory=FALSE,
               char *PluginData=NULL);
    FileEditor(char *Name,int CreateNewFile,int EnableSwitch,
               int StartLine,int StartChar,char *Title,
               int X1,int Y1,int X2,int Y2);
    void Init(char *Name,int CreateNewFile,int EnableSwitch,
              int StartLine,int StartChar,int DisableHistory,char *PluginData);
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);
    int GetTypeAndName(char *Type,char *Name);
    void ShowConsoleTitle();
    int IsFileChanged() {return(FEdit.IsFileChanged());};
    int IsFileModified() {return(FEdit.IsFileModified());};
    int GetExitCode();
};

#endif	// __FILEEDITOR_HPP__
