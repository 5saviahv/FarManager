#ifndef __HIGHLIGHTFILES_HPP__
#define __HIGHLIGHTFILES_HPP__
/*
hilight.hpp

Files highlighting

*/

/* Revision: 1.05 12.07.2001 $ */

/*
Modify:
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

/* $ 06.07.2001 IS
   ������ "�������" ����� ���������� ��������������� �����
*/
struct HighlightData
{
  char *OriginalMasks;
  CFileMask *FMasks;
  unsigned int IncludeAttr;
  unsigned int ExcludeAttr;
  unsigned int Color,SelColor,CursorColor,CursorSelColor,MarkChar;
};
/* IS $ */

class HighlightFiles
{
  private:
    struct HighlightData *HiData;
    int HiDataCount;
    int StartHiDataCount;

  private:
    void SaveHiData();
    int EditRecord(int RecPos,int New);
    /* $ 07.07.2000 IS
      � ��� ������� � ����� ���������� ������������, ����� ������������ ���
      �������� ��� �������������� �������� ��������� ������ �� ���������
    */
    void InitHighlightFiles();
    /* IS $ */
    void ClearData();
    int  DupHighlightData(struct HighlightData *Data,char *Mask,int RecPos);

    char *GetMask(int Idx);
    BOOL AddMask(struct HighlightData *Dest,char *Mask,struct HighlightData *Src=NULL);
    void DeleteMask(struct HighlightData *CurHighlightData);

  public:
    HighlightFiles();
    ~HighlightFiles();

  public:
    void GetHiColor(char *Path,int Attr,unsigned char &Color,
                    unsigned char &SelColor,unsigned char &CursorColor,
                    unsigned char &CursorSelColor,unsigned char &MarkChar);
    void HiEdit(int MenuPos);
};

// ���� ������� � hilight.cpp
char *MkRegKeyHighlightName(char *RegKey);

#endif  // __HIGHLIGHTFILES_HPP__
