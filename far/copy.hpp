#ifndef __SHELLCOPY_HPP__
#define __SHELLCOPY_HPP__
/*
copy.hpp

class ShellCopy - ����������� ������

*/

/* Revision: 1.07 30.05.2001 $ */

/*
Modify:
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

class Panel;

enum COPY_CODES {
  COPY_CANCEL,
  COPY_NEXT,
  COPY_FAILURE,
  COPY_SUCCESS,
  COPY_SUCCESS_MOVE
};

#define FCOPY_COPYTONUL      0x00000001 // ������� ����������� � NUL
#define FCOPY_CURRENTONLY    0x00000002 //
#define FCOPY_ONLYNEWERFILES 0x00000004 // Copy only newer files
#define FCOPY_CREATESYMLINK  0x00000004
#define FCOPY_OVERWRITENEXT  0x00000008
#define FCOPY_LINK           0x00000010
#define FCOPY_MOVE           0x00000040
#define FCOPY_DIZREAD        0x00000080
#define FCOPY_COPYSECURITY   0x00000100
#define FCOPY_NOSHOWMSGLINK  0x00000200
#define FCOPY_VOLMOUNT       0x00000400

class ShellCopy
{
  private:
    DWORD Flags;

    Panel *SrcPanel,*AnotherPanel;
    int PanelMode,SrcPanelMode;

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

    char RenamedName[NM],CopiedName[NM];
    int OvrMode,ReadOnlyOvrMode,ReadOnlyDelMode;
    long TotalFiles;
    int SrcDriveType;
    char SrcDriveRoot[NM];
    int SelectedFolderNameLength;

  private:
    COPY_CODES CopyFileTree(char *Dest);
    COPY_CODES ShellCopyOneFile(char *Src,WIN32_FIND_DATA *SrcData,char *Dest,
                                int KeepPathPos,int Rename);
    int ShellCopyFile(char *SrcName,WIN32_FIND_DATA *SrcData,char *DestName,
                      DWORD DestAttr,int Append);
    int ShellSystemCopy(char *SrcName,char *DestName,WIN32_FIND_DATA *SrcData);
    void ShellCopyMsg(char *Src,char *Dest,int Flags);
    int ShellCopyConvertWildcards(char *Src,char *Dest);
    int DeleteAfterMove(char *Name,int Attr);
    void SetDestDizPath(char *DestPath);
    int AskOverwrite(WIN32_FIND_DATA *SrcData,char *DestName,
        DWORD DestAttr,int SameName,int Rename,int AskAppend,
        int &Append,int &RetCode);
    int GetSecurity(char *FileName,SECURITY_ATTRIBUTES *sa);
    int SetSecurity(char *FileName,SECURITY_ATTRIBUTES *sa);
    int IsSameDisk(char *SrcPath,char *DestPath);
    bool CalcTotalSize();
    int CmpFullNames(char *Src,char *Dest);
    BOOL LinkRules(DWORD *Flags7,DWORD* Flags5,int* Selected5,char *SrcDir,char *DstDir,struct CopyDlgParam *CDP);

  public:
    ShellCopy(Panel *SrcPanel,int Move,int Link,int CurrentOnly,int Ask,
              int &ToPlugin,char *PluginDestPath);
    ~ShellCopy();

  public:
    static void ShowBar(int64 WrittenSize,int64 TotalSize,bool TotalBar);
    static void ShowTitle(int FirstTime);
    static long WINAPI CopyDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2);
    static int MkSymLink(char *SelName,char *Dest,DWORD Flags);
};


#endif	// __SHELLCOPY_HPP__
