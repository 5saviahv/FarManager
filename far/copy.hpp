#ifndef __SHELLCOPY_HPP__
#define __SHELLCOPY_HPP__
/*
copy.hpp

class ShellCopy - ����஢���� 䠩���

*/

/* Revision: 1.03 23.10.2000 $ */

/*
Modify:
  23.10.2000 VVM
   + �������᪨� ���� ����஢���� - ࠡ�稥 ��६����
  21.10.2000 SVS
    + ��६����� Copy_Buffer_Size -  ࠧ��� ���� ��� ����஢����
  04.08.2000 SVS
    + ���� "Only newer file(s)"
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class ShellCopy
{
  private:
    char sddata[16000];
    DizList DestDiz;
    int DizRead;
    char DestDizPath[2*NM];
    Panel *SrcPanel,*AnotherPanel;
    char *CopyBuffer;
    /* $ 23.10.2000 VVM
       + �������᪨� ���� ����஢���� - ࠡ�稥 ��६���� */
    int CopyBufSize;
    clock_t StartTime;
    clock_t StopTime;
    /* VVM $ */
    char RenamedName[NM],CopiedName[NM];
    int PanelMode,SrcPanelMode;
    int OvrMode,ReadOnlyOvrMode,ReadOnlyDelMode;
    int Move;
    int Link;
    int CurrentOnly;
    /* $ 04.08.2000 SVS
       Copy only newer files
    */
    int OnlyNewerFiles;
    /* SVS $ */
    int CopySecurity;
    long TotalFiles;
    int SrcDriveType;
    char SrcDriveRoot[NM];
    int SelectedFolderNameLength;
    int OverwriteNext;

  private:
    void CopyFileTree(char *Dest);
    COPY_CODES ShellCopyOneFile(char *Src,WIN32_FIND_DATA *SrcData,char *Dest,
                                int KeepPathPos,int Rename);
    int ShellCopyFile(char *SrcName,WIN32_FIND_DATA *SrcData,char *DestName,
                      DWORD DestAttr,int Append);
    int ShellSystemCopy(char *SrcName,char *DestName,WIN32_FIND_DATA *SrcData);
    void ShellCopyMsg(char *Src,char *Dest,int Flags);
    int ShellCopyConvertWildcards(char *Src,char *Dest);
    void CreatePath(char *Path);
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

  public:
    ShellCopy(Panel *SrcPanel,int Move,int Link,int CurrentOnly,int Ask,
              int &ToPlugin,char *PluginDestPath);
    ~ShellCopy();

  public:
    static void ShowBar(int64 WrittenSize,int64 TotalSize,bool TotalBar);
    static void ShowTitle(int FirstTime);
};


#endif	// __SHELLCOPY_HPP__
