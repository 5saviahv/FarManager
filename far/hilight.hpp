#ifndef __HIGHLIGHTFILES_HPP__
#define __HIGHLIGHTFILES_HPP__
/*
hilight.hpp

Files highlighting

*/

/* Revision: 1.02 12.07.2000 $ */

/*
Modify:
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
    void SaveHiData();
    int EditRecord(int RecPos,int New);
    struct HighlightData *HiData;
    int HiDataCount;
    int StartHiDataCount;
    /* $ 07.07.2000 IS
      � ��� �㭪�� � �뭥� ᮤ�ন��� ���������, �⮡� �ᯮ�짮���� ���
      ����୮ �� ����⠭������� ���祭�� ���᪨ 䠩��� �� 㬮�砭��
    */
    void InitHighlightFiles();
    /* IS $ */
    void ClearData();
  public:
    HighlightFiles();
    ~HighlightFiles();
    void GetHiColor(char *Path,int Attr,unsigned char &Color,
                    unsigned char &SelColor,unsigned char &CursorColor,
                    unsigned char &CursorSelColor,unsigned char &MarkChar);
    void HiEdit(int MenuPos);
};

#endif	// __HIGHLIGHTFILES_HPP__