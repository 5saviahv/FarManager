#ifndef __HIGHLIGHTFILES_HPP__
#define __HIGHLIGHTFILES_HPP__
/*
hilight.hpp

Files highlighting

*/

/* Revision: 1.03 23.04.2001 $ */

/*
Modify:
  23.04.2001 SVS
    ! ���! ���� ��<�� �� %PATHEXT% - � �� ।����㥬 � �, ��
      �� - ࠧ�� ��魮��.
  12.02.2001 SVS
    + �㭪�� ClearData - ���⪠ HiData
  07.07.2000 IS
    + ����� �㭪�� InitHighlightFiles, � ������ � �뭥� ᮤ�ন���
      ���������. �㦭�, �⮡� ����୮ �ᯮ�짮���� ���� � �� �� ���.
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/


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
      � ��� �㭪�� � �뭥� ᮤ�ন��� ���������, �⮡� �ᯮ�짮���� ���
      ����୮ �� ����⠭������� ���祭�� ���᪨ 䠩��� �� 㬮�砭��
    */
    void InitHighlightFiles();
    /* IS $ */
    void ClearData();

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

#endif	// __HIGHLIGHTFILES_HPP__