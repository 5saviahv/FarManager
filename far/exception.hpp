#ifndef __FAREXCPT_HPP__
#define __FAREXCPT_HPP__
/*
exception.cpp

��� ��� ����������

*/

/* Revision: 1.06 19.02.2002 $ */

/*
Modify:
  19.02.2002 SVS
    ! ��� ������ ����� :-)
  25.01.2002 SVS
    ! �������������, ���� ��� :-((
    ! FAULTCODERECORD.Code = 128 ����
    ! ������ pXXXXXX �������� ������ ����� (FuncFlags)
    ! ���� ������ ��������� ;-)
  22.01.2002 SVS
    ! ��������� � PLUGINRECORD
  11.07.2001 SVS
    + FARAREARECORD.ScrWH - ������� ������ - ������, ������
  16.05.2001 SVS
    ! ��������� ���������������� ������� EVENTPROC � ��������� WriteEvent
    + PLUGINSINFORECORD
  16.05.2001 SVS
    ! Created
*/

#include "plugins.hpp"

#if defined(__BORLANDC__)
  #pragma option -a2
#elif defined(__GNUC__) || (defined(__WATCOMC__) && (__WATCOMC__ < 1100)) || defined(__LCC__)
  #pragma pack(2)
#else
  #pragma pack(push,2)
#endif

#define FAR_LOG_VERSION  1

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                  \
      ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
      ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

#define FLOG_SYSINFO     0x00000001 // ���������� � �������
#define FLOG_EXCEPTION   0x00000002 // ��� ����������
#define FLOG_PLUGIN      0x00000004 // ���������� � �������
#define FLOG_FARAREA     0x00000008 // "��� �� ������ ���������?"
#define FLOG_MACRO       0x00000010 // �������
#define FLOG_RAWDARA     0x00000020 // ������������ ������
#define FLOG_PLUGINSINFO 0x80000000 // ���������� � ��������
#define FLOG_ALL         0xFFFFFFFF

enum FARRECORDTYPE{
  RTYPE_SYSINFO      =MAKEFOURCC('S','Y','S','T'),// ���������� � �������
  RTYPE_EXCEPTION    =MAKEFOURCC('E','X','C','T'),// ��� ����������
  RTYPE_PLUGIN       =MAKEFOURCC('C','P','L','G'),// ���������� � ������� �������
  RTYPE_FARAREA      =MAKEFOURCC('A','R','E','A'),// "��� �� ������ ���������?"
  RTYPE_MACRO        =MAKEFOURCC('M','A','C','R'),// �������
  RTYPE_RAWDARA      =MAKEFOURCC('R','A','W','D'),// ������������ ������
};

struct RECHEADER{         // ��������� �������
  DWORD TypeRec;          // ��� ������
  DWORD SizeRec;          // ������ ���������
  struct RECHEADER *Next; // ��������� ������� � ������
  // Data                 // ������ �������� SizeRec
};

struct SYSINFOHEADER{     // ���������� � �������
  DWORD TypeRec;          // ��� ������ = RTYPE_SYSINFO
  DWORD SizeRec;          // ������ ������ = sizeof(struct DUMPHEADER)-sizeof(WORD)*2
  struct RECHEADER *Next; // ��������� ������� � ������
  DWORD DumpFlags;        // �������������� ����� (���� =0)
  DWORD FARVersion;       // ������ FAR Manager � ������� FAR_VERSION
  SYSTEMTIME DumpTime;    // the system time is expressed in Coordinated Universal Time (UTC))
  OSVERSIONINFO WinVer;   // ������ ������
};

struct PLUGINRECORD{      // ���������� � �������
  DWORD TypeRec;          // ��� ������ = RTYPE_PLUGIN
  DWORD SizeRec;          // ������
  struct RECHEADER *Next; // ��������� ������� � ������

  DWORD WorkFlags;      // ������� ����� �������� �������
  DWORD FuncFlags;      // ������� ����� ����.������� ������� (��� ���� - ��� � �������)
  DWORD CallFlags;      // ������� ����� ������ ����.������� �������

  short CachePos;       // ������� � ����
  DWORD SysID;

  struct {
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD    nFileSizeHigh;
    DWORD    nFileSizeLow;
    DWORD    dwReserved0;
    DWORD    dwReserved1;
    char     cFileName[MAX_PATH];
    char     cAlternateFileName[14];
  } FindData;

  DWORD Reserved[2];    // ������ :-)

  DWORD SizeModuleName;
  //char ModuleName[0];
};

struct EXCEPTIONRECORD{   // ��� ����������
  DWORD TypeRec;          // ��� ������ = RTYPE_EXCEPTION
  DWORD SizeRec;          // ������ ������
  struct RECHEADER *Next; // ��������� ������� � ������

  EXCEPTION_POINTERS *Exception;
};

struct MACRORECORD{       // �������
  DWORD TypeRec;          // ��� ������ = RTYPE_MACRO
  DWORD SizeRec;          // ������
  struct RECHEADER *Next; // ��������� ������� � ������
  WORD  MacroStatus;      // 0 - �� � ������ �����, 1 - Recording, 2 - Executing
  WORD  MacroPos;         // ������� ������� � MacroKeyBuffer
  DWORD MacroFlags;       // ����� - ������� ����� = MACRO_AREA
  DWORD MacroKey;         // ����������� ������������
  DWORD MacroBufferSize;  // ������ ����������������������� MacroKeyBuffer
  // DWORD MacroKeyBuffer[0];// �����-������������������
};

struct FARAREARECORD{     // "��� �� ������ ���������?"
  DWORD TypeRec;          // ��� ������ = RTYPE_FARAREA
  DWORD SizeRec;          // ������ ������
  struct RECHEADER *Next; // ��������� ������� � ������
  DWORD ObjectType;       // ��, ��� ���������� CtrlObject->Cp()->GetType()
  COORD ScrWH;            // ������� ������ - ������, ������
};

enum {
  RAWTYPE_BINARY =0,
  RAWTYPE_TEXT   =1,
};

struct RAWDARARECORD{     // ������������ ������
  DWORD TypeRec;          // ��� ������ = RTYPE_RAWDARA
  DWORD SizeRec;          // ������ ������
  struct RECHEADER *Next; // ��������� ������� � ������
  DWORD RawFlags;         // �������������� ����� ��� ���������������� :-)
  DWORD RawType;          // ��� ������ = RAWTYPE_BINARY, RAWTYPE_TEXT
  DWORD SizeData;         // ������ ������������ ������
  //BYTE Data[0];         // ���� SizeRec=0, �� ����� ���� ����
};


//�������� ��������� ��� �������� ������� � ������
//����� ��������� ������
struct COMBINE_RECORD {
  RECHEADER Header;
  char      Data[ 1 /*SizeRec*/ ];
};
//�������� ��������� ��� ������� � ����������� ������
//����� ������
struct MACRORECORD_t : public MACRORECORD {
  DWORD MacroKeyBuffer[ 1 /*MacroBufferSize*/ ];
};
struct PLUGINRECORD_t : public PLUGINRECORD {
  char ModuleName[ 1 /*SizeModuleName*/ ];
};
struct RAWDARARECORD_t : public RAWDARARECORD {
  LPBYTE RawDataPtr;  //The pointer to allocated raw data can be placed here
};

struct FARExceptionState {
  DWORD               StructSize;
  DWORD               Version;       // ������ "��������"

  //FAR additional error info
  char RecomendedDumpFileName[MAXPATH];
  const char         *RootKey;

  //FAR error context
  struct RECHEADER   *Head;
};

#if defined(__BORLANDC__)
  #pragma option -a.
#elif defined(__GNUC__) || (defined(__WATCOMC__) && (__WATCOMC__ < 1100)) || defined(__LCC__)
  #pragma pack()
#else
  #pragma pack(pop)
#endif

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

typedef BOOL (WINAPI *FARPROCESSEVENT)(struct FARExceptionState * Context);

int WriteEvent(DWORD DumpType, // FLOG_*
               EXCEPTION_POINTERS *xp=NULL,
               struct PluginItem *Module=NULL,
               void *RawData=NULL,DWORD RawDataSize=0,
               DWORD RawDataFlags=0,DWORD RawType=RAWTYPE_BINARY);

int xfilter(
    int From,                 // ������: 0 = OpenPlugin, 1 = OpenFilePlugin
    EXCEPTION_POINTERS *xp,   // ������ ��������
    struct PluginItem *Module,// ������, ��������� � ����������.
    DWORD Flags);             // �������������� ����� - ���� ������ ����
                              //        0x1 - ���������� ��� ��������?

#endif // __FAREXCPT_HPP__
