#ifndef __SHELLCOPY_HPP__
#define __SHELLCOPY_HPP__
/*
copy.hpp

class ShellCopy - ����������� ������

*/

/* Revision: 1.26 22.09.2003 $ */

/*
Modify:
  22.09.2003 SVS
    + FCOPY_UPDATEPPANEL ���������� �������� ��������� ������
    + CheckUpdatePanel(); ���������� ���� FCOPY_UPDATEPPANEL
  08.07.2003 SVS
    + CheckNulOrCon()
  30.05.2003 SVS
    + FCOPY_COPYSYMLINKCONTENTS - ���������� ���������� ������������ ������?
      ���� ������ ����. ������ ��� �� �������!
  30.04.2003 VVM
    ! ShowBar() ���������� TRUE/FALSE
  17.03.2003 SVS
    + FCOPY_SKIPSETATTRFLD - ������������ �������������� ���� ���������
      ����������� ��������� dest-���������
  25.05.2002 IS
    + MoveFileThroughTemp - ������� �������������� �����, ������� ���������
      ���� ��� ������, ����� ��������������� ������ � ���� (�� ������
      ������������� SVS ������� - ������� ������ MoveFile).
    ! ����� FCOPY_RENAMESAME, �.�. �� ������������
    ! ��������� const � ������
  24.05.2002 SVS
    + FCOPY_RENAMESAME - �������������� ���� abcdefghi.txt -> abcdef~1.txt,
      ����� ��� ��� ���� � ����� ����
  26.04.2002 SVS
    - BugZ#484 - Addons\Macros\Space.reg (��� ��������� �������)
  01.04.2002 SVS
    ! ��������� ;-)
  28.03.2002 SVS
    ! ������� const
  02.03.2002 KM
    + SkipMode - ������� ��� ����������� ���������� ������
  12.02.2002 SVS
    + COPY_FAILUREREAD
  06.12.2001 VVM
    + FCOPY_COPYLASTTIME
  21.10.2001 SVS
    + CALLBACK-������� ��� ���������� �� BugZ#85
  17.10.2001 SVS
    ! ��������� const
  16.10.2001 SVS
    + CheckStreams() - �������� ������� �������
  03.08.2001 IS
    ! ����� "#ifndef COPY_NOMULTICOPY", �.�. ������ ��������������
      �������������� �� ����������� ������.
  19.06.2001 SVS
    + ShellSetAttr - �������� ������ SetFileAttributes
  02.06.2001 IS
    ! #define FCOPY* -> enum
      ��, �� ����� ������� (��������� ������ �������)
    + #include "udlist" & DestList
  30.05.2001 SVS
    ! ShellCopy::CreatePath �������� �� ������ � �������� �������
    + CopyDlgProc()
    + MkSymLink() - ��� ��������� �������
    ! ������� �������� ������ ������� �� ���� ������ ��������� ����������
      �� �����.
    + LinkRules() - ���������� ������� ��� ��������
  06.05.2001 DJ
    ! �������� #include
  01.01.2001 VVM
    + ���������� CopyBufferSize -  ������ ������ ��� �����������
  14.12.2000 SVS
    + CopyToNUL - ������� ����������� � NUL
  23.10.2000 VVM
    + ������������ ����� ����������� - ������� ����������
  21.10.2000 SVS
    + ���������� Copy_Buffer_Size -  ������ ������ ��� �����������
  04.08.2000 SVS
    + ����� "Only newer file(s)"
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "dizlist.hpp"
#include "int64.hpp"
#include "udlist.hpp"

class Panel;

enum COPY_CODES {
  COPY_CANCEL,
  COPY_NEXT,
  COPY_FAILURE,
  COPY_FAILUREREAD,
  COPY_SUCCESS,
  COPY_SUCCESS_MOVE
};

enum COPY_FLAGS {
  FCOPY_COPYTONUL               = 0x00000001, // ������� ����������� � NUL
  FCOPY_CURRENTONLY             = 0x00000002, // ������ ������?
  FCOPY_ONLYNEWERFILES          = 0x00000004, // Copy only newer files
  FCOPY_CREATESYMLINK           = 0x00000004, // �������� ��������
  FCOPY_OVERWRITENEXT           = 0x00000008, // Overwrite all
  FCOPY_LINK                    = 0x00000010, // �������� �����
  FCOPY_MOVE                    = 0x00000040, // �������/��������������
  FCOPY_DIZREAD                 = 0x00000080, //
  FCOPY_COPYSECURITY            = 0x00000100, // [x] Copy access rights
  FCOPY_NOSHOWMSGLINK           = 0x00000200, // �� ���������� ������ ��� ���������
  FCOPY_VOLMOUNT                = 0x00000400, // �������� ������������� ����
  FCOPY_STREAMSKIP              = 0x00000800, // ������
  FCOPY_STREAMALL               = 0x00001000, // ������
  FCOPY_SKIPSETATTRFLD          = 0x00002000, // ������ �� �������� ������� �������� ��� ��������� - ����� ������ Skip All
  FCOPY_COPYSYMLINKCONTENTS     = 0x00004000, // ���������� ���������� ������������ ������?
  FCOPY_COPYLASTTIME            = 0x10000000, // ��� ����������� � ��������� ��������� ��������������� ��� ����������.
  FCOPY_UPDATEPPANEL            = 0x80000000, // ���������� �������� ��������� ������
};

class ConsoleTitle;

class ShellCopy
{
  private:
    DWORD Flags;

    Panel *SrcPanel;
    int    SrcPanelMode;
    int    SrcDriveType;
    char   SrcDriveRoot[NM];
    char   SrcFSName[16];
    DWORD  SrcFSFlags;

    Panel *DestPanel;
    int    DestPanelMode;
    int    DestDriveType;
    char   DestDriveRoot[NM];
    char   DestFSName[16];
    DWORD  DestFSFlags;

    char sddata[16000]; // Security

    DizList DestDiz;
    char DestDizPath[2*NM];

    /* $ 23.10.2000 VVM
       + ������������ ����� ����������� - ������� ���������� */
    char *CopyBuffer;
    int CopyBufSize;
    int CopyBufferSize;
    clock_t StartTime;
    clock_t StopTime;
    /* VVM $ */

    char RenamedName[NM];
    char CopiedName[NM];

    /* $ 02.03.2002 KM
      + ����� �������� SkipMode - ��� �������� ��� �����������
        ���������� ������.
    */
    int OvrMode;
    int ReadOnlyOvrMode;
    int ReadOnlyDelMode;
    int SkipMode;
    /* KM $ */

    long TotalFiles;
    int SelectedFolderNameLength;

    UserDefinedList DestList;  // �������� ������ �����
    ConsoleTitle *CopyTitle;   // ���������

  private:
    COPY_CODES CopyFileTree(char *Dest);
    COPY_CODES ShellCopyOneFile(const char *Src,
                                const WIN32_FIND_DATA &SrcData,
                                const char *Dest,
                                int KeepPathPos, int Rename);
    COPY_CODES CheckStreams(const char *Src,const char *DestPath);
    int  ShellCopyFile(const char *SrcName,const WIN32_FIND_DATA &SrcData,
                       const char *DestName, DWORD DestAttr,int Append);
    int  ShellSystemCopy(const char *SrcName,const char *DestName,const WIN32_FIND_DATA &SrcData);
    void ShellCopyMsg(const char *Src,const char *Dest,int Flags);
    int  ShellCopyConvertWildcards(const char *Src,char *Dest);
    int  DeleteAfterMove(const char *Name,int Attr);
    void SetDestDizPath(const char *DestPath);
    int  AskOverwrite(const WIN32_FIND_DATA &SrcData,const char *DestName,
                      DWORD DestAttr,int SameName,int Rename,int AskAppend,
                      int &Append,int &RetCode);
    int  GetSecurity(const char *FileName,SECURITY_ATTRIBUTES &sa);
    int  SetSecurity(const char *FileName,const SECURITY_ATTRIBUTES &sa);
    int  IsSameDisk(const char *SrcPath,const char *DestPath);
    bool CalcTotalSize();
    int  CmpFullNames(const char *Src,const char *Dest);
    BOOL LinkRules(DWORD *Flags7,DWORD* Flags5,int* Selected5,char *SrcDir,char *DstDir,struct CopyDlgParam *CDP);
    int  ShellSetAttr(const char *Dest,DWORD Attr);
    /* $ 25.05.2002 IS
       ������� �������������� �����, ������� ��������� ���� ��� ������, �����
       ��������������� ������ � ���� (�� ������ ������������� SVS ������� -
       ������� ������ MoveFile). ���������� TRUE - �����, FALSE - �������.
    */
    BOOL MoveFileThroughTemp(const char *Src, const char *Dest);
    /* IS $ */

    BOOL CheckNulOrCon(const char *Src);
    void CheckUpdatePanel(); // ���������� ���� FCOPY_UPDATEPPANEL

  public:
    ShellCopy(Panel *SrcPanel,int Move,int Link,int CurrentOnly,int Ask,
              int &ToPlugin,char *PluginDestPath);
    ~ShellCopy();

  public:
    static int  ShowBar(int64 WrittenSize,int64 TotalSize,bool TotalBar);
    static void ShowTitle(int FirstTime);
    static long WINAPI CopyDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    static int  MkSymLink(const char *SelName,const char *Dest,DWORD Flags);
    static void PR_ShellCopyMsg(void);
};


#endif  // __SHELLCOPY_HPP__
