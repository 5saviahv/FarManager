#ifndef __FAREXCPT_HPP__
#define __FAREXCPT_HPP__
/*
exception.cpp

��� ��� ����������

*/

/* Revision: 1.04 25.01.2002 $ */

/*
Modify:
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

#define FLOG_SYSINFO     0x00000001 // ���������� � �������
#define FLOG_REGISTERS   0x00000002 // ������� ��������� ���������
#define FLOG_EXCEPTION   0x00000004 // ��� ����������
#define FLOG_STACK       0x00000008 // "������������ ����"
#define FLOG_PLUGIN      0x00000010 // ���������� � �������
#define FLOG_FARAREA     0x00000020 // "��� �� ������ ���������?"
#define FLOG_MACRO       0x00000040 // �������
#define FLOG_RAWDARA     0x00000080 // ������������ ������
#define FLOG_FAULTCODE   0x00000100 // ����� ����
#define FLOG_PLUGINSINFO 0x00000200 // ���������� � ��������
#define FLOG_ALL         0xFFFFFFFF

enum {
  RTYPE_HEADER       =0,// ��������� �����
  RTYPE_SYSINFO      =1,// ���������� � �������
  RTYPE_CONTEXT      =2,// ������� ��������� ���������
  RTYPE_EXCEPTION    =3,// ��� ����������
  RTYPE_FAULTCODE    =4,// ����� ����
  RTYPE_STACK        =5,// "������������ ����"
  RTYPE_PLUGIN       =6,// ���������� � �������
  RTYPE_FARAREA      =7,// "��� �� ������ ���������?"
  RTYPE_MACRO        =8,// �������
  RTYPE_RAWDARA      =9,// ������������ ������
};

/*

��������� ����� "farevent.dmp"

FILEHEADER FileHeader;

Record {
    RECHEADER RecHeader;  // ���������
    // RECHEADER Rec[0];  // ����������� RecHeader.RecordsCount
} [FileHeader.CountRecords];

*/

struct FILEHEADER{
  DWORD ID;               // 'FLOG'
  DWORD CountRecords;     // ���������� �������
  DWORD Version;          // ������ "��������"
};

struct RECHEADER{         // ��������� �������
  WORD TypeRec;           // ��� ������
  WORD SizeRec;           // ������ ������ Data (����), ���� =0 - Data �����������
  // Data                 // ������ �������� SizeRec
};

struct DUMPHEADER{        // ��������� �����
  WORD  TypeRec;          // ��� ������ = RTYPE_HEADER
  WORD  SizeRec;          // ������ ������ = sizeof(struct DUMPHEADER)-sizeof(WORD)*2
  DWORD DumpFlags;        // �������������� ����� (���� =0)
  DWORD DumpSize;         // ����� ������ �������� �����
  SYSTEMTIME DumpTime;    // the system time is expressed in Coordinated Universal Time (UTC))
  WORD  RecordsCount;     // ����� ���������� �������� RECHEADER �� ����������� ���������
  WORD  CountException;   // �� ��� - ���������� ������� ��� ����������
  WORD  CountStack;       // �� ��� - ���������� ������� ��� ����
  WORD  Reserved[5];      // ������
};

struct SYSINFOHEADER{     // ���������� � �������
  WORD  TypeRec;          // ��� ������ = RTYPE_SYSINFO
  WORD  SizeRec;          // ������ ������ = sizeof(struct DUMPHEADER)-sizeof(WORD)*2
  OSVERSIONINFO WinVer;   // ������ ������
  DWORD FARVersion;       // ������ FAR Manager � ������� FAR_VERSION
  DWORD Reserved[3];      // ������
};

struct CONTEXTRECORD{     // ������� ��������� ���������
  WORD  TypeRec;          // ��� ������ = RTYPE_CONTEXT
  WORD  SizeRec;          // ������ ������ = sizeof(CONTEXT)
  CONTEXT Regs;           // �������� ����������
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x0550)
  //
  // This section is specified/returned if the ContextFlags word
  // contains the flag CONTEXT_EXTENDED_REGISTERS.
  // The format and contexts are processor specific
  //
#define MAXIMUM_SUPPORTED_EXTENSION     512
#define CONTEXT_EXTENDED_REGISTERS  (CONTEXT_i386 | 0x00000020L) // cpu specific extensions
  BYTE    ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];
#endif
};

struct MACRORECORD{       // �������
  WORD  TypeRec;          // ��� ������ = RTYPE_MACRO
  WORD  SizeRec;          // ������ ������
  WORD  MacroStatus;      // 0 - �� � ������ �����, 1 - Recording, 2 - Executing
  WORD  MacroPos;         // ������� ������� � MacroKeyBuffer
  DWORD MacroFlags;       // ����� - ������� ����� = MACRO_AREA
  DWORD MacroKey;         // ����������� ������������
  DWORD MacroBufferSize;  // ������ ����������������������� MacroKeyBuffer
  // DWORD MacroKeyBuffer[0];// �����-������������������
};

struct PLUGINRECORD{      // ���������� � �������
  WORD  TypeRec;          // ��� ������ = RTYPE_PLUGIN
  WORD  SizeRec;          // ������ ������

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
    char     cFileName[NM];
    char     cAlternateFileName[14];
  } FindData;

  DWORD Reserved[4];    // ������ :-)

  DWORD SizeModuleName;
  //char ModuleName[0];
};

struct PLUGINSINFORECORD{ // ���������� � ��������
  WORD  TypeRec;          // ��� ������ = FLOG_PLUGINSINFO
  WORD  SizeRec;          // ������ ������
  DWORD PluginsCount;     // ���������� �������
}; // ������ �� ���� �������� ���� PluginsCount ������� PLUGINRECORD

struct EXCEPTIONRECORD{   // ��� ����������
  WORD  TypeRec;          // ��� ������ = RTYPE_EXCEPTION
  WORD  SizeRec;          // ������ ������
  WORD  CurItem;          // ������ ������ ���������� � ������� �����
                          //   (���� ��� �������� ��������)
  WORD  Reserved;
  // ����������� EXCEPTION_RECORD
  DWORD ExceptionCode;    // ��� ����������
  DWORD ExceptionFlags;   // ����� ����������
  DWORD ExceptionAddress; // ����� ����������
  DWORD Section;          //  ������  \ Fault address - ���������� �
  DWORD Offset;           //  ��������/                 ExceptionAddress
  DWORD NumberParameters; // ���������� ���������� ���. ����������
  DWORD ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS]; // ���. ����������
  DWORD SizeModuleName;   // ������ ����� ������
  //char  ModuleName[0];    // ���� SizeModuleName=0, �� ����� ���� ����.
};

struct FAULTCODERECORD{   // ����� ����
  WORD  TypeRec;          // ��� ������ = RTYPE_FAULTCODE
  WORD  SizeRec;          // ������ ������
  DWORD ExceptionAddress; // ����� ����������
  DWORD SuccessCode;      // =0 - ������ � Code ���������
  BYTE  Code[128];        // ������ 64 ����� ����� ��� ������, ��� ��� ����
                          // 64-� ���� - ��� � ���� �� ����� �����!
                          // ��� 128 ����� ����� ���� ��������� 0, ���� ��
                          // ���������� ������� ������ �� ������ (��� �������)
  DWORD SuccessData;
  BYTE  Data[128];
};

struct STACKRECORD{       // "������������ ����"
  WORD  TypeRec;          // ��� ������ = RTYPE_STACK
  WORD  SizeRec;          // ������ ������
  WORD  CurItem;          // ������ ������ ����� � ������� �����
  WORD  Reserved;
  DWORD EIP;              // �����
  DWORD EBP;
  DWORD Section;          // ������
  DWORD Offset;           // ��������

  DWORD SizeModuleName;   // ������ ����� ������
  // char  ModuleName[0];   // ���� SizeModuleName=0, �� ����� ���� ����.
};


struct FARAREARECORD{     // "��� �� ������ ���������?"
  WORD  TypeRec;          // ��� ������ = RTYPE_FARAREA
  WORD  SizeRec;          // ������ ������
  DWORD ObjectType;       // ��, ��� ���������� CtrlObject->Cp()->GetType()
  COORD ScrWH;            // ������� ������ - ������, ������

  DWORD Reserved[20];
};

struct RAWDARARECORD{     // ������������ ������
  WORD  TypeRec;          // ��� ������ = RTYPE_RAWDARA
  WORD  SizeRec;          // ������ ������
  DWORD RawFlags;         // �������������� ����� ��� ���������������� :-)
  //BYTE Data[0];         // ���� SizeRec=0, �� ����� ���� ����
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

/* ������� ��� ������ ������ �������� ��� ������ ������ ������ :-)
   ���������� �� ��� ���, ���� �� ������ FALSE.
   ������� ������ ��������� � ��������� SizeRec ������ ���������� ������.
   Iteration - ����� ��������� �������� (���������� � 0)
*/
typedef BOOL (WINAPI *EVENTPROC)(HANDLE hFile,DWORD *SizeRec,int Iteration);

int WriteEvent(DWORD DumpType, // FLOG_*
               EXCEPTION_POINTERS *xp,
               struct PluginItem *Module,
               void *RawData,DWORD RawDataSize,
               DWORD RawDataFlags=0,
               EVENTPROC CallBackProc=NULL);
int xfilter(
    int From,                 // ������: 0 = OpenPlugin, 1 = OpenFilePlugin
    EXCEPTION_POINTERS *xp,   // ������ ��������
    struct PluginItem *Module,// ������, ��������� � ����������.
    DWORD Flags);             // �������������� ����� - ���� ������ ����
                              //        0x1 - ���������� ��� ��������?

#endif // __FAREXCPT_HPP__
