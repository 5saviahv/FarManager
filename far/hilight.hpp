#ifndef __HIGHLIGHTFILES_HPP__
#define __HIGHLIGHTFILES_HPP__
/*
hilight.hpp

Files highlighting

*/

/* Revision: 1.09 14.12.2001 $ */

/*
Modify:
  14.12.2001 SVS
    ! SaveHiData() private -> public ��� ������ � SaveConfig()
  23.11.2001 SVS
    + GetHiColor(), ���������� � ����� �������� FileListItem
  25.09.2001 IS
    + HighlightData.IgnoreMask
    + �������� IgnoreMask � AddMask � � DupHighlightData
  12.07.2001 SVS
    ! ������ ����� Color* � ������� HighlightData ������������
      ��������� HighlightDataColor
    ! �����������, ��������� ���������� ���������� � GetHiColor()
    + ������� ReWriteWorkColor - ����� �� �������
  12.07.2001 SVS
    + ������� ������������ - DupHighlightData()
  06.07.2001 IS
    + � HighlightData ���������� ������ Masks (������� �����) ���������������
      �����.
    ! � HighlightData OriginalMasks ���� ����� ������ ������.
  23.04.2001 SVS
    ! ���! ����� ��<��� �� %PATHEXT% - �� ��� ����������� � ��, ���
      ����� - ������ ��������.
  12.02.2001 SVS
    + ������� ClearData - ������� HiData
  07.07.2000 IS
    + ����� ������� InitHighlightFiles, � ������� � ����� ����������
      ������������. �����, ����� �������� ������������ ���� � ��� �� ���.
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "CFileMask.hpp"
#include "struct.hpp"

/* $ 06.07.2001 IS
   ������ "�������" ����� ���������� ��������������� �����
*/
struct HighlightData
{
  char *OriginalMasks;
  CFileMask *FMasks;
  /* $ 25.09.2001 IS
       ���� TRUE, �� ����� ������ ������������ � ��������� ���� ������ ��
       ���������.
  */
  BOOL IgnoreMask;
  /* IS $ */
  unsigned int IncludeAttr;
  unsigned int ExcludeAttr;
  struct HighlightDataColor Colors;
};
/* IS $ */

class VMenu;
struct FileListItem;

class HighlightFiles
{
  private:
    struct HighlightData *HiData;
    int HiDataCount;
    int StartHiDataCount;

  private:
    int EditRecord(int RecPos,int New);
    /* $ 07.07.2000 IS
      � ��� ������� � ����� ���������� ������������, ����� ������������ ���
      �������� ��� �������������� �������� ��������� ������ �� ���������
    */
    void InitHighlightFiles();
    /* IS $ */
    void ClearData();
    int  DupHighlightData(struct HighlightData *Data,char *Mask,BOOL IgnoreMask,int RecPos);

    char *GetMask(int Idx);
    BOOL AddMask(struct HighlightData *Dest,char *Mask,BOOL IgnoreMask,struct HighlightData *Src=NULL);
    void DeleteMask(struct HighlightData *CurHighlightData);
    void FillMenu(VMenu *HiMenu,int MenuPos);

  public:
    HighlightFiles();
    ~HighlightFiles();

  public:
    void GetHiColor(char *Path,int Attr,
                    struct HighlightDataColor *Colors);
    void GetHiColor(struct FileListItem *FileItem,int FileCount);
    void HiEdit(int MenuPos);

    void SaveHiData();

    static void ReWriteWorkColor(struct HighlightDataColor *Colors=NULL);
};

// ���� ������� � hilight.cpp
char *MkRegKeyHighlightName(char *RegKey);

#endif  // __HIGHLIGHTFILES_HPP__
