#ifndef __FARCONST_HPP__
#define __FARCONST_HPP__
/*
farconst.hpp

ᮤ�ন� �� enum, #define, etc

*/

/* Revision: 1.28 25.04.2001 $ */

/*
Modify:
  25.04.2001 SVS
    + MODALTREE_FREE
  13.04.2001 VVM
    + ��ࠡ�⪠ ����ᨪ� ��誨 ��� 2000.
  04.04.2001 SVS
    + MACRO_INFOPANEL,MACRO_QVIEWPANEL,MACRO_TREEPANEL,MACRO_LAST
      MACRO_LAST ������ �ᥣ�� ����� ��᫥����. ��஫����� � 横���.
  02.04.2001 SVS
    ! DRIVE_SUSTITUTE -> DRIVE_SUBSTITUTE
  02.04.2001 VVM
    + DRIVE_NOT_INIT. �ᯮ������ �� ������ ���� ��᪮�.
  16.03.2001 SVS
    + FlagsNameAndPassword
  26.02.2001 VVM
    + STATUS_INVALIDFUNCTIONRESULT - ����� �㭪�� ���㫠 �������⨬�� ���祭��
  25.02.2001 VVM
    + ����� ��� ReadDiz() - RDF_*
  11.02.2001 SVS
    + *_MASK_SIZE - ࠧ���� ��� ��᮪
  28.01.2001 SVS
    + SKEY_VK_KEYS - ��॥堫 �� plugin.hpp
  25.01.2001 SVS
    + STATUS_STRUCTWRONGFILLED - ����� Count>0 � **Ptr=NULL
  22.01.2001 SVS
    + EXCEPT_PROCESSVIEWERINPUT - �� ���饥.
  05.01.2001 SVS
    + DRIVE_SUSTITUTE - �� �� SUBST-��᪨.
  25.12.2000 SVS
    ! MACRO_ ��॥堫� �� plugin.hpp :-)
  23.12.2000 SVS
    ! MACRO_ ��॥堫� � plugin.hpp
  21.12.2000 SVS
    ! ��������� � ����⠭�� MACRO_*
  07.12.2000 SVS
    ! farversion.hpp �᪫�祭 �� �������������
  31.10.2000 SVS
    + EXCEPT_GETOPENPLUGININFO_DATA
  23.10.2000 SVS
    + EXCEPT_GETPLUGININFO_DATA
  20.10.2000 SVS
    ! FILE_ATTRIBUTE_REPARSE_POINT ��७�ᥭ � headers.hpp
  17.10.2000 SVS
    + ����������! ����⠭�� EXCEPT_* � ����� TRY
  27.09.2000 SVS
    +CASR_* ��������� Ctrl-Alt-Shift ��� AllCtrlAltShiftRule
  15.07.2000 SVS
    + ����⠭�� ��� ���� �� VVM - �뭥ᥭ� �� usermenu.cpp
  12.07.2000 SVS
    + ����⠭�� ��� WrapMode �� ���.
  11.07.2000 SVS
    ! ��������� ��� ���������� �������樨 ��� BC & VC
  10.07.2000 tran
    ! 㢥��祭� MAXSCRY � 120 �� 300
      ���� ��� ���200 �� ������ �� Viewer
      ⠬ �� ����⠭� �ᯮ������ � ����⢥ ࠧ��� ���ᨢ�
      ������� ��ப.
      㢥��稢��� ��� �� ����� ��᫠...
  29.06.2000 tran
    ! ����� ������ �� ���-������㥬��� farversion.hpp
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#define TreeFileName "Tree.Far"
#define LocalMenuFileName "FarMenu.Ini"
#define TreeCacheFolderName "Tree.Cache"
#define PluginsFolderName "Plugins"
#define LangFileMask "*.lng"
#define HelpFileMask "*.hlf"

typedef unsigned char    UBYTE;
typedef unsigned short   UWORD;
typedef unsigned long    UDWORD;

#define  Min(x,y) (((x)<(y)) ? (x):(y))
#define  Max(x,y) (((x)>(y)) ? (x):(y))
#undef isspace
#define  isspace(x) ((x)==' ' || (x)=='\t')
#define  iseol(x) ((x)=='\r' || (x)=='\n')

#define  FALSE        0
#define  TRUE         1

/* $ 10.07.2000 tran
   ! modified MAXSCRY from 120 to 300
   on win200, with Console height FAR work, but trap on viewer... */
#define  MAXSCRY     300
/* tran 10.07.2000 $ */

#define  NM          260

#define  DEFAULT_SORT_GROUP 10000

enum {FILE_PANEL,TREE_PANEL,QVIEW_PANEL,INFO_PANEL};
enum {UNSORTED,BY_NAME,BY_EXT,BY_MTIME,BY_CTIME,BY_ATIME,BY_SIZE,BY_DIZ,
      BY_OWNER,BY_COMPRESSEDSIZE,BY_NUMLINKS};
enum {VIEW_0=0,VIEW_1,VIEW_2,VIEW_3,VIEW_4,VIEW_5,VIEW_6,VIEW_7,VIEW_8,VIEW_9};
// ⨯� ࠬ��
enum {NO_BOX,SINGLE_BOX,SHORT_SINGLE_BOX,DOUBLE_BOX,SHORT_DOUBLE_BOX};
enum {MSG_WARNING=1,MSG_ERRORTYPE=2,MSG_KEEPBACKGROUND=4,MSG_DOWN=8,
      MSG_LEFTALIGN=16};
enum {FILETYPE_EXEC,FILETYPE_VIEW,FILETYPE_EDIT};
enum {DRIVE_SHOW_TYPE=1,DRIVE_SHOW_NETNAME=2,DRIVE_SHOW_LABEL=4,
      DRIVE_SHOW_FILESYSTEM=8,DRIVE_SHOW_SIZE=16,DRIVE_SHOW_REMOVABLE=32,
      DRIVE_SHOW_PLUGINS=64,DRIVE_SHOW_CDROM=128};
enum {UPDATE_KEEP_SELECTION=1,UPDATE_SECONDARY=2};

#undef SEARCH_ALL
enum {SEARCH_ALL=0,SEARCH_ROOT,SEARCH_FROM_CURRENT,SEARCH_CURRENT_ONLY,
      SEARCH_SELECTED};
enum {MODALTYPE_VIEWER,MODALTYPE_EDITOR};

enum {DIZ_NOT_UPDATE,DIZ_UPDATE_IF_DISPLAYED,DIZ_UPDATE_ALWAYS};

// struct EditorUndoData
enum {UNDO_NONE=0,UNDO_EDIT,UNDO_INSSTR,UNDO_DELSTR};

enum {
  MACRO_SHELL,MACRO_VIEWER,MACRO_EDITOR,MACRO_DIALOG,MACRO_SEARCH,
  MACRO_DISKS,MACRO_MAINMENU,MACRO_HELP,
  MACRO_INFOPANEL,MACRO_QVIEWPANEL,MACRO_TREEPANEL,
  /* $ 05.04.2001 VVM
    ! ������ ���� �ᥣ�� ��᫥����. �ᯮ������ � 横��� */
  MACRO_LAST,
  /* VVM $ */
  MACRO_OTHER=0x1000
};

#define MakeDialogItems(Data,Item) \
  struct DialogItem Item[sizeof(Data)/sizeof(Data[0])]; \
  Dialog::DataToItem(Data,Item,sizeof(Data)/sizeof(Data[0]));

// for filelist
enum {ARCHIVE_NONE,ARCHIVE_RAR,ARCHIVE_ZIP,ARCHIVE_ARJ,ARCHIVE_LZH};

#define MAX_MSG 5000

// for class Panel
enum {
  COLUMN_MARK        =  0x80000000,
  COLUMN_NAMEONLY    =  0x40000000,
  COLUMN_RIGHTALIGN  =  0x20000000,
  COLUMN_FORMATTED   =  0x10000000,
  COLUMN_COMMAS      =  0x08000000,
  COLUMN_THOUSAND    =  0x04000000,
  COLUMN_BRIEF       =  0x02000000,
  COLUMN_MONTH       =  0x01000000,
};

// for class Panel
enum {MODALTREE_ACTIVE=1,MODALTREE_PASSIVE=2,MODALTREE_FREE=3};
enum {NORMAL_PANEL,PLUGIN_PANEL};



// from plugins.hpp
enum {PLUGIN_FARGETFILE,PLUGIN_FARGETFILES,PLUGIN_FARPUTFILES,
      PLUGIN_FARDELETEFILES,PLUGIN_FARMAKEDIRECTORY,PLUGIN_FAROTHER};


      // for class ShellCopy
enum COPY_CODES {COPY_CANCEL,COPY_NEXT,COPY_FAILURE,COPY_SUCCESS,
                 COPY_SUCCESS_MOVE};

// for class VMenu
enum {MENU_SHOWAMPERSAND=1,MENU_WRAPMODE=2,MENU_DISABLEDRAWBACKGROUND=4};


// from delete.cpp
enum {DELETE_SUCCESS,DELETE_YES,DELETE_SKIP,DELETE_CANCEL};


// from edit.cpp
enum {EOL_NONE,EOL_CR,EOL_LF,EOL_CRLF};

// from filelist.cpp
enum SELECT_MODES {SELECT_INVERT,SELECT_INVERTALL,SELECT_ADD,SELECT_REMOVE,
     SELECT_ADDEXT,SELECT_REMOVEEXT,SELECT_ADDNAME,SELECT_REMOVENAME};


// from flmodes.cpp
enum {NAME_COLUMN=0,SIZE_COLUMN,PACKED_COLUMN,DATE_COLUMN,TIME_COLUMN,
      MDATE_COLUMN,CDATE_COLUMN,ADATE_COLUMN,ATTR_COLUMN,DIZ_COLUMN,
      OWNER_COLUMN,NUMLINK_COLUMN,
      CUSTOM_COLUMN0,CUSTOM_COLUMN1,CUSTOM_COLUMN2,CUSTOM_COLUMN3,
      CUSTOM_COLUMN4,CUSTOM_COLUMN5,CUSTOM_COLUMN6,CUSTOM_COLUMN7,
      CUSTOM_COLUMN8,CUSTOM_COLUMN9};

// from scrsaver.cpp
enum {STAR_NONE,STAR_NORMAL,STAR_PLANET};

/* $ 12.07.2000 SVS
   + ����⠭�� ��� WrapMode �� ���.
*/
enum {VIEW_UNWRAP=0,VIEW_WRAP=1, VIEW_WORDWRAP=2};
/* SVS $ */

/* $ 15.07.2000 SVS
   + ����⠭�� ��� ���� �� VVM - �뭥ᥭ� �� usermenu.cpp
*/
// VVM> ������ ������ ���� (Menu mode) � ���� ��室� �� ���� (Exit codes)
enum {MM_LOCAL=0,           // �����쭮� ����
      MM_FAR=1,             // ���� �� ��⠫��� ����
      MM_MAIN=2};           // ������� ����

enum {EC_CLOSE_LEVEL = -1,  // ��� �� ���� �� ���� �஢��� �����
      EC_CLOSE_MENU  =-2,   // ��� �� ���� �� SHIFT+F10
      EC_PARENT_MENU =-3,   // �������� ���� த�⥫�᪮�� ��⠫���
      EC_MAIN_MENU   =-4};  // �������� ������� ����

/* SVS $ */

/* $ 27.09.2000 SVS
  +CASR_* ��������� Ctrl-Alt-Shift ��� AllCtrlAltShiftRule
*/
enum {
  CASR_PANEL  = 0x0001,
  CASR_EDITOR = 0x0002,
  CASR_VIEWER = 0x0004,
  CASR_HELP   = 0x0008,
  CASR_DIALOG = 0x0010,
};
/* SVS $ */

#define SYSID_PRINTMANAGER	0x6E614D50

/* $ 17.10.2000 SVS
   ����������!
*/
enum EceptFunctionsType{
  EXCEPT_SETSTARTUPINFO,
  EXCEPT_GETVIRTUALFINDDATA,
  EXCEPT_OPENPLUGIN,
  EXCEPT_OPENFILEPLUGIN,
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
};

/* $ 25.02.2001 VVM
  + ����� ��� ReadDiz() */
enum ReadDizFlags {
  RDF_NO_UPDATE		= 0x00000001UL,
};
/* VVM $ */

#define STATUS_STRUCTWRONGFILLED       0xE0001000
#define STATUS_INVALIDFUNCTIONRESULT   0xE0002000

#if defined(__BORLANDC__)
#define TRY	try
#else
#define TRY	__try
#endif
/* SVS $ */

#define DRIVE_SUBSTITUTE    0x0F
#define DRIVE_NOT_INIT      0xFF


#define SKEY_VK_KEYS        0x40000000


// ࠧ���� ��� ����� �����
#define HIGHLIGHT_MASK_SIZE      2048
#define GROUPSORT_MASK_SIZE      2048
#define PANELFILTER_MASK_SIZE    2048

// ��� ������� GetNameAndPassword()
enum FlagsNameAndPassword{
  GNP_USELAST      = 0x00000001UL, // �ᯮ�짮���� ��᫥���� �������� �����
  GNP_NOOEMTOCHAR  = 0x00000002UL, // �� �������஢��� ��� � ��஫� OEM->CHAR
};

/* $ 13.04.2001 VVM
  + 2000 ���뫠�� ���᮫쭮�� ���� ᮡ�⨥ � dwEventFlags == MOUSE_WHEELED */
#define MOUSE_WHEELED 0x0004
/* VVM $ */

#endif // __FARCONST_HPP__
