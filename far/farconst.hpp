#ifndef __FARCONST_HPP__
#define __FARCONST_HPP__
/*
farconst.hpp

�������� ��� enum, #define, etc

*/

/* Revision: 1.71 25.08.2003 $ */

/*
Modify:
  25.08.2003 SVS
    ! �� SendKeysToPlugins, �� NoSendKeysToPlugins, ����� ������ �� ����������.
  30.07.2003 SVS
    ! ������ MFLAGS_INSIDEPLUGIN � MFLAGS_NOINSIDEPLUGIN ������ ���� ���� MFLAGS_SENDKEYSTOPLUGINS,
      � ����������� � NT � ��������
    ! ������� �������������
  25.07.2003 SVS
    + MacroRecordAndExecuteType - ���� �������� ��� KeyMacro::GetCurRecord()
  15.07.2003 SVS
    + MFLAGS_INSIDEPLUGIN � MFLAGS_NOINSIDEPLUGIN
  15.06.2003 SVS
    + �� ���� ��������� ������� AsciiToUnicode � OEMToUnicode
  03.01.2003 SVS
    + CHAR_WCHAR
  10.12.2002 SVS
    + MSG_KILLSAVESCREEN
  14.07.2002 IS & SVS
    ! IS> PluginsFolderName, HelpFileMask � HelpFormatLinkModule ������ ��
      IS> "#define", � "const char *" - ��� ���������� ������� ������
      SVS> ...� ��������� � global.?pp
  18.06.2002 SVS
    + ���� �������� CHECKFOLDERCONST ��� CheckFolder()
  18.05.2002 SVS
    ! ����������� ���������� ��� BC 5.5
  15.05.2002 SVS
    + SKEY_IDLE - ������ VK_ ������� FOCUS
  04.04.2002 SVS
    + FFTMODE
  22.02.2002 SVS
    ! �������� FAR_INT64
  10.01.2002 SVS
    + XC_EXISTS - ��� �������� ��� ���������
  25.12.2001 SVS
    + HelpFormatLinkModule
  24.12.2001 SVS
    - Bug - ��� ����. ����� ��������� HelpFormatLink
  01.11.2001 SVS
    ! MakeDialogItems �������� � dialog.hpp �� farconst.hpp
  26.10.2001 VVM
    + MOUSE_ANY_BUTTON_PRESSED
  16.10.2001 SVS
    + ADDSPACEFORPSTRFORMESSAGE - ��� Message (������ ����� 16)
  21.10.2001 SVS
    ! PREREDRAWFUNC � PISDEBUGGERPRESENT ��������� �� global.hpp
  16.10.2001 SVS
    + �������-��������������: UnicodeToAscii() � UnicodeToOEM()
  16.09.2001 SVS
    ! ����������� ����������
  07.09.2001 SVS
    ! ����� ��� �������� MFLAGS_* ��������� �� macro.cpp
  02.08.2001 IS
    + ��� ���������� ������:
      FILETYPE_ALTEXEC, FILETYPE_ALTVIEW, FILETYPE_ALTEDIT
  01.08.2001 SVS
    + HelpBeginLink, HelpFormatLink - ������ ��� �������� ������ �� ���� ������.
  31.07.2001 SVS
    + MACRO_MENU
  24.07.2001 SVS
    - �������� ���������� ��� VC
  24.07.2001 IS
    ! isspace � iseol ������ �� �������, � inline �������
  22.07.2001 SVS
    + ����������� SysID ��� Network Browse ������� - SYSID_NETWORK
  26.06.2001 SVS
    ! __except -> EXCEPT
  25.06.2001 SVS
    + SEARCHSTRINGBUFSIZE - ������ ������ ��� ������ ������.
  20.06.2001 SVS
    ! SKEY_NOTMACROS �������� �� plugin.hpp
  06.06.2001 SVS
    + BOOKMARK_COUNT
    ! Min/Max �������� �� inline-�������
  04.06.2001 SVS
    + HISTORY_COUNT - ������ �������
  31.05.2001 OT
    ! ��������� ��� FileEdit
  23.05.2001 SVS
    ! ����������� ��� MACRO_OTHER
  21.05.2001 SVS
    ! ��������� MENU_ - � ����
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 SVS
    ! ������ ����������� ���
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  28.04.2001 SVS
    + EXCEPT_OPENPLUGIN_FINDLIST, EXCEPT_FARDIALOG
  26.04.2001 DJ
    + ����� ��������� ��� UserMenu EC_COMMAND_SELECTED
  25.04.2001 SVS
    + MODALTREE_FREE
  13.04.2001 VVM
    + ��������� �������� ����� ��� 2000.
  04.04.2001 SVS
    + MACRO_INFOPANEL,MACRO_QVIEWPANEL,MACRO_TREEPANEL,MACRO_LAST
      MACRO_LAST ������ ������ ������ ���������. ������������� � ������.
  02.04.2001 SVS
    ! DRIVE_SUSTITUTE -> DRIVE_SUBSTITUTE
  02.04.2001 VVM
    + DRIVE_NOT_INIT. ������������ ��� ������ ���� ������.
  16.03.2001 SVS
    + FlagsNameAndPassword
  26.02.2001 VVM
    + STATUS_INVALIDFUNCTIONRESULT - ����� ������� ������� ������������ ��������
  25.02.2001 VVM
    + ����� ��� ReadDiz() - RDF_*
  11.02.2001 SVS
    + *_MASK_SIZE - ������� ��� �����
  28.01.2001 SVS
    + SKEY_VK_KEYS - �������� �� plugin.hpp
  25.01.2001 SVS
    + STATUS_STRUCTWRONGFILLED - ����� Count>0 � **Ptr=NULL
  22.01.2001 SVS
    + EXCEPT_PROCESSVIEWERINPUT - �� �������.
  05.01.2001 SVS
    + DRIVE_SUSTITUTE - ��� ��� SUBST-�����.
  25.12.2000 SVS
    ! MACRO_ ��������� �� plugin.hpp :-)
  23.12.2000 SVS
    ! MACRO_ ��������� � plugin.hpp
  21.12.2000 SVS
    ! ��������� � ���������� MACRO_*
  07.12.2000 SVS
    ! farversion.hpp �������� �� �������������
  31.10.2000 SVS
    + EXCEPT_GETOPENPLUGININFO_DATA
  23.10.2000 SVS
    + EXCEPT_GETPLUGININFO_DATA
  20.10.2000 SVS
    ! FILE_ATTRIBUTE_REPARSE_POINT ��������� � headers.hpp
  17.10.2000 SVS
    + ����������! ��������� EXCEPT_* � ������ TRY
  27.09.2000 SVS
    +CASR_* ��������� Ctrl-Alt-Shift ��� AllCtrlAltShiftRule
  15.07.2000 SVS
    + ��������� ��� ���� �� VVM - �������� �� usermenu.cpp
  12.07.2000 SVS
    + ��������� ��� WrapMode �� �������.
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  10.07.2000 tran
    ! ��������� MAXSCRY � 120 �� 300
      ����� ��� ���200 ��� ������ �� Viewer
      ��� ��� ��������� ������������ � �������� ������� �������
      ������� �����.
      ����������� ���� �� ����� ������...
  29.06.2000 tran
    ! ������ ������� �� ����-������������� farversion.hpp
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#define HelpBeginLink '<'
#define HelpEndLink '>'
#define HelpFormatLink "<%s\\>%s"

typedef unsigned char    UBYTE;
typedef unsigned short   UWORD;
typedef unsigned long    UDWORD;

typedef union {
  __int64 i64;
  struct {
    DWORD LowPart;
    LONG  HighPart;
  } Part;
} FAR_INT64;


//#define  Min(x,y) (((x)<(y)) ? (x):(y))
template <class T>
inline const T&Min(const T &a, const T &b) { return a<b?a:b; }

//#define  Max(x,y) (((x)>(y)) ? (x):(y))
template <class T>
inline const T&Max(const T &a, const T &b) { return a>b?a:b; }

inline int IsSpace(int x) { return x==' ' || x=='\t'; }
inline int IsEol(int x)  { return x=='\r' || x=='\n'; }

#define  FALSE        0
#define  TRUE         1

#define  NM          260

#define  DEFAULT_SORT_GROUP 10000
#define  SEARCHSTRINGBUFSIZE 512

// ���� �����
enum {NO_BOX,SINGLE_BOX,SHORT_SINGLE_BOX,DOUBLE_BOX,SHORT_DOUBLE_BOX};

enum {
  MSG_WARNING        =0x00000001,
  MSG_ERRORTYPE      =0x00000002,
  MSG_KEEPBACKGROUND =0x00000004,
  MSG_DOWN           =0x00000008,
  MSG_LEFTALIGN      =0x00000010,
  MSG_KILLSAVESCREEN =0x00000020,
};

/* $ 02.08.2001 IS
     ����� ��������� ��� alt-f3, alt-f4 � ctrl-pgdn
*/
// ������ � ������������ ������
enum {
  FILETYPE_EXEC,       // Enter
  FILETYPE_VIEW,       // F3
  FILETYPE_EDIT,       // F4
  FILETYPE_ALTEXEC,    // Ctrl-PgDn
  FILETYPE_ALTVIEW,    // Alt-F3
  FILETYPE_ALTEDIT     // Alt-F4
};
/* IS $ */

enum DIZUPDATETYPE {
  DIZ_NOT_UPDATE,
  DIZ_UPDATE_IF_DISPLAYED,
  DIZ_UPDATE_ALWAYS
};

// *** Macros ***
// ������� �������� �������� (������ ����������)
enum MACROMODEAREA {
  MACRO_OTHER,         // ����� ����������� ������ � ������, ������������ ����
  MACRO_SHELL,         // �������� ������
  MACRO_VIEWER,        // ���������� ��������� ���������
  MACRO_EDITOR,        // ��������
  MACRO_DIALOG,        // �������
  MACRO_SEARCH,        // ������� ����� � �������
  MACRO_DISKS,         // ���� ������ ������
  MACRO_MAINMENU,      // �������� ����
  MACRO_MENU,          // ������ ����
  MACRO_HELP,          // ������� ������
  MACRO_INFOPANEL,     // �������������� ������
  MACRO_QVIEWPANEL,    // ������ �������� ���������
  MACRO_TREEPANEL,     // ������ ������ �����

  MACRO_LAST           // ������ ���� ������ ���������! ������������ � ������
};

// ����� ��� ��������
#define MFLAGS_MODEMASK            0x0000FFFF // ����� ��� ��������� ������� �������� (������� ������ ����������) �������
#define MFLAGS_DISABLEOUTPUT       0x00010000 // �������� ���������� ������ �� ����� ���������� �������
#define MFLAGS_RUNAFTERFARSTART    0x00020000 // ���� ������ ����������� ��� ������ ����
#define MFLAGS_EMPTYCOMMANDLINE    0x00040000 // ���������, ���� ��������� ����� �����
#define MFLAGS_NOTEMPTYCOMMANDLINE 0x00080000 // ���������, ���� ��������� ����� �� �����
#define MFLAGS_NOFILEPANELS        0x00100000 // ���������, ���� ��� ���������� ������
#define MFLAGS_NOPLUGINPANELS      0x00200000 // ���������, ���� ��� �������� ������
#define MFLAGS_NOFOLDERS           0x00400000 // ���������, ���� ������� ������ "����"
#define MFLAGS_NOFILES             0x00800000 // ���������, ���� ������� ������ "�����"
#define MFLAGS_REUSEMACRO          0x02000000 // ��������� ������������� �������� (����� ������� �� �������)
#define MFLAGS_SELECTION           0x04000000 // ���������, ���� ���� ���������
#define MFLAGS_NOSELECTION         0x08000000 // ���������, ���� ���� ��� ���������
#define MFLAGS_NOSENDKEYSTOPLUGINS 0x10000000 // �� ���������� ������� �� ����� ������/��������������� �������
#define MFLAGS_RUNAFTERFARSTART2   0x40000000 // ������� ����, ��� ������ ��������� ��� ����������.
#define MFLAGS_DISABLEMACRO        0x80000000 // ���� ������ ��������

// ���� �������� ��� KeyMacro::GetCurRecord()
enum MACRORECORDANDEXECUTETYPE{
  MACROMODE_NOMACRO          =0,  // �� � ������ �����
  MACROMODE_EXECUTING        =1,  // ����������: ��� �������� ������� ����
  MACROMODE_EXECUTING_COMMON =2,  // ����������: � ��������� ������� ����
  MACROMODE_RECORDING        =3,  // ������: ��� �������� ������� ����
  MACROMODE_RECORDING_COMMON =4,  // ������: � ��������� ������� ����
};

// **************************************************

// for filelist
enum {ARCHIVE_NONE,ARCHIVE_RAR,ARCHIVE_ZIP,ARCHIVE_ARJ,ARCHIVE_LZH};

#define MAX_MSG 5000

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

// from plugins.hpp
enum {
  PLUGIN_FARGETFILE,
  PLUGIN_FARGETFILES,
  PLUGIN_FARPUTFILES,
  PLUGIN_FARDELETEFILES,
  PLUGIN_FARMAKEDIRECTORY,
  PLUGIN_FAROTHER
};

enum {
  MODALTREE_ACTIVE  =1,
  MODALTREE_PASSIVE =2,
  MODALTREE_FREE    =3
};

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

#define SYSID_PRINTMANAGER      0x6E614D50
#define SYSID_NETWORK           0x5774654E


/* $ 25.02.2001 VVM
  + ����� ��� ReadDiz() */
enum ReadDizFlags {
  RDF_NO_UPDATE         = 0x00000001UL,
};
/* VVM $ */

#define STATUS_STRUCTWRONGFILLED       0xE0001000
#define STATUS_INVALIDFUNCTIONRESULT   0xE0002000

#if defined(__BORLANDC__)
#define TRY      try
#else
#define TRY   __try
#endif
#define EXCEPT __except

#define DRIVE_SUBSTITUTE    0x0F
#define DRIVE_NOT_INIT      0xFF


enum {
  SKEY_VK_KEYS           = 0x40000000,
  SKEY_IDLE              = 0x80000000,
  SKEY_NOTMACROS         = 0x00000001,
};

// ������� ��� ����� �����
#define PANELFILTER_MASK_SIZE    2048

// ��� ������� GetNameAndPassword()
enum FlagsNameAndPassword{
  GNP_USELAST      = 0x00000001UL, // ������������ ��������� ��������� ������
  GNP_NOOEMTOCHAR  = 0x00000002UL, // �� �������������� ��� � ������ OEM->CHAR
};

/* $ 13.04.2001 VVM
  + 2000 �������� ����������� ���� ������� � dwEventFlags == MOUSE_WHEELED */
#define MOUSE_WHEELED 0x0004
/* VVM $ */

///

/* $ 27.05.2001 DJ         30.05.2001 OT
   ��������� ��� ExitCode - ���������� ��
*/

enum {
    XC_QUIT                = (unsigned long) -777,
    XC_OPEN_ERROR          = 0,
    XC_MODIFIED            = 1,
    XC_NOT_MODIFIED        = 2,
    XC_LOADING_INTERRUPTED = 3,
    XC_EXISTS              = 4,
};

/* DJ $ */


/* $ 11.08.2000 tran
   ������ ��� ������ ������������� */
#define ENABLE_SWITCH TRUE
#define DISABLE_SWITCH FALSE
///

// ������ ������� - ��� ��� ������ History, ��� � ��� ��������!
#define HISTORY_COUNT    64
// ���������� �������� � ���������/������� �� ���� �������
#define BOOKMARK_COUNT   10

#define UnicodeToAscii(src,dst,lendst)  WideCharToMultiByte(CP_ACP,0,(src),-1,(dst),(lendst),NULL,FALSE)
#define UnicodeToOEM(src,dst,lendst)    WideCharToMultiByte(CP_OEMCP,0,(src),-1,(dst),(lendst),NULL,FALSE)
#define AsciiToUnicode(src,dst,lendst)  MultiByteToWideChar(CP_ACP,0,(src),-1,(dst),(lendst))
#define OEMToUnicode(src,dst,lendst)    MultiByteToWideChar(CP_OEMCP,0,(src),-1,(dst),(lendst))

typedef void (*PREREDRAWFUNC)(void);
typedef BOOL (WINAPI *PISDEBUGGERPRESENT)(VOID);

#define ADDSPACEFORPSTRFORMESSAGE 16

#define MOUSE_ANY_BUTTON_PRESSED (FROM_LEFT_1ST_BUTTON_PRESSED|RIGHTMOST_BUTTON_PRESSED|FROM_LEFT_2ND_BUTTON_PRESSED|FROM_LEFT_3RD_BUTTON_PRESSED|FROM_LEFT_4TH_BUTTON_PRESSED)

enum FFTMODE{
  FFTM_BREAKLONGWORD = 0x00000001,
};

enum CHECKFOLDERCONST{ // for CheckFolder()
  CHKFLD_NOTACCESS = -1,
  CHKFLD_EMPTY     =  0,
  CHKFLD_NOTEMPTY  =  1,
};

typedef union {
  WCHAR UnicodeChar;
  CHAR  AsciiChar;
} CHAR_WCHAR;

#endif // __FARCONST_HPP__
