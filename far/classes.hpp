#ifndef __CLASSES_HPP__
#define __CLASSES_HPP__
/*
classes.hpp

��������� ��� ����ᮢ

*/

/* Revision: 1.04 29.04.2001 $ */

/*
Modify:
  29.04.2001 ��
    + ����७�� NWZ �� ����类��
  20.03.2001 tran
    + constitle.hpp
  24.01.2001 SVS
    + farqueue.hpp - 蠡��� ��।�.
  17.07.2000 OT
    + ���⮫���� ���� ��� ࠧࠡ��� "�����" ।����
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class Panel;
class Modal;
class FileEditor;
class Viewer;
class VMenu;
class History;

#define MSG(ID) FarMSG(ID)

#include "farqueue.hpp"
#include "int64.hpp"
#include "language.hpp"
#include "plugins.hpp"
#include "savescr.hpp"
#include "baseinp.hpp"
#include "scrobj.hpp"
#include "macro.hpp"
#include "dizlist.hpp"
#include "grpsort.hpp"
#include "filter.hpp"
#include "panel.hpp"
#include "filelist.hpp"
#include "treelist.hpp"
#include "qview.hpp"
#include "infolist.hpp"
#include "edit.hpp"
#include "cmdline.hpp"
#include "keybar.hpp"
#include "menubar.hpp"
#include "namelist.hpp"
#include "viewer.hpp"
#include "hilight.hpp"
#include "poscache.hpp"
#include "window.hpp"
#include "manager.hpp"
#include "modal.hpp"
#include "filepanels.hpp"
#include "ctrlobj.hpp"
#include "foldtree.hpp"
#include "fileview.hpp"
#include "vmenu.hpp"
#include "hmenu.hpp"
#include "dialog.hpp"
#include "scantree.hpp"
#include "struct.hpp"
#if defined(EDITOR2)
 #include "editor2.hpp"
#else defined(EDITOR2)
 #include "editor.hpp"
#endif defined(EDITOR2)
#include "fileedit.hpp"
#include "help.hpp"
#include "copy.hpp"
#include "chgprior.hpp"
#include "savefpos.hpp"
#include "lockscrn.hpp"
#include "rdrwdsk.hpp"
#include "grabber.hpp"
#include "findfile.hpp"
#include "plognmn.hpp"
#include "filestr.hpp"
#include "scrbuf.hpp"
#include "history.hpp"
#include "chgmmode.hpp"
#include "constitle.hpp"

#endif	// __CLASSES_HPP__
