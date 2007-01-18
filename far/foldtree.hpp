#ifndef __FOLDERTREE_HPP__
#define __FOLDERTREE_HPP__
/*
foldtree.hpp

����� �������� �� Alt-F10

*/


#include "frame.hpp"
#include "keybar.hpp"
#include "farconst.hpp"

class TreeList;
class Edit;
class SaveScreen;

class FolderTree:public Frame
{
  private:
    TreeList *Tree;
    Edit *FindEdit;
    //SaveScreen *TopScreen;

    KeyBar TreeKeyBar;     // ������
    int ModalMode;
    int IsFullScreen;
    int IsStandalone;
    char NewFolder[NM];
    char LastName[NM];

    int  PrevMacroMode;        // ���������� ����� �������

  private:
    void DrawEdit();
    virtual void DisplayObject();
    void SetCoords();


  public:
    FolderTree(char *ResultFolder,int ModalMode,int IsPanel=TRUE,int IsFullScreen=TRUE);
    ~FolderTree();

  public:
    int ProcessKey(int Key);
    int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);

    void InitKeyBar(void);
    void OnChangeFocus(int focus); // ���������� ��� ����� ������
    virtual void SetScreenPosition();
    void ResizeConsole();
    /* $ ������� ��� ���� CtrlAltShift OT */
    int  FastHide();
    void Hide();

    virtual const char *GetTypeName() {return "[FolderTree]";}
    virtual int GetTypeAndName(char *Type,char *Name);
    virtual int GetType() { return MODALTYPE_FINDFOLDER; }

};


#endif  // __FOLDERTREE_HPP__
