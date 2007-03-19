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

    KeyBar TreeKeyBar;     // ������
    int ModalMode;
    int IsFullScreen;
    int IsStandalone;
    int  PrevMacroMode;        // ���������� ����� �������

    string strNewFolder;
    string strLastName;

  private:
    void DrawEdit();
    virtual void DisplayObject();
    void SetCoords();

  public:
    FolderTree(string &strResultFolder,int ModalMode,int IsStandalone=TRUE,int IsFullScreen=TRUE);
    virtual ~FolderTree();

  public:
    virtual int ProcessKey(int Key);
    virtual int ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent);

    virtual void InitKeyBar(void);
    virtual void OnChangeFocus(int focus); // ���������� ��� ����� ������
    virtual void SetScreenPosition();
    virtual void ResizeConsole();
    /* $ ������� ��� ���� CtrlAltShift OT */
    virtual int  FastHide();

    virtual const wchar_t *GetTypeName() {return L"[FolderTree]";}
    virtual int GetTypeAndName(string &strType, string &strName);
    virtual int GetType() { return MODALTYPE_FINDFOLDER; }

};


#endif  // __FOLDERTREE_HPP__
