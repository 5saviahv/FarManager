#ifndef __HISTORY_HPP__
#define __HISTORY_HPP__
/*
history.hpp

������� (Alt-F8, Alt-F11, Alt-F12)

*/

/* Revision: 1.08 25.01.2002 $ */

/*
Modify:
  25.01.2002 SVS
    ! ���������� Name ������ ������������. ������ � ������ - max 511 ����.
      � NT - ������� ����. �.�. ���������� ����� ����� ������ � ������.
  16.01.2002 VVM
    + AddToHistory - ����� ��������
      SaveForbid - ������������� ��������� ������ ����������� ������.
      ������������ �� ������ �������
  15,11,2001 SVS
    + ��� �������.
  08.11.2001 SVS
    ! ������ ������� - ������ �������� ����� :-)
  04.06.2001 SVS
    ! 64 -> HISTORY_COUNT
  06.05.2001 DJ
    ! �������� #include
  09.01.2001 SVS
    - ���� � CmdHistoryRule=1
  24.09.2000 SVS
    + SetFirst() - ��� CmdHistoryRule
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "farconst.hpp"

enum{
  HISTORYTYPE_CMD,
  HISTORYTYPE_FOLDER,
  HISTORYTYPE_VIEW,
};

#define HISTORY_TITLESIZE 32

struct HistoryRecord
{
  int   Type;
  char  Title[HISTORY_TITLESIZE];
  char *Name;
};

class History
{
  private:
    char RegKey[256];
    unsigned int LastPtr,CurLastPtr;
    unsigned int LastPtr0,CurLastPtr0;
    int EnableAdd,RemoveDups,KeepSelectedPos;
    int TypeHistory;
    int *EnableSave,SaveTitle,SaveType;
    int LastSimilar;
    int ReturnSimilarTemplate;
    struct HistoryRecord LastStr[HISTORY_COUNT];

  private:
    void AddToHistoryLocal(char *Str,char *Title,int Type);
    void FreeHistory(BOOL FreeMemody=FALSE);

  public:
    History(int TypeHistory,char *RegKey,int *EnableSave,int SaveTitle,int SaveType);
   ~History();

  public:
    void AddToHistory(char *Str,char *Title=NULL,int Type=0,int SaveForbid=0);
    BOOL ReadHistory();
    BOOL SaveHistory();
    int  Select(char *Title,char *HelpTopic,char *Str,int &Type,char *ItemTitle=NULL);
    void GetPrev(char *Str);
    void GetNext(char *Str);
    void SetFirst() {LastPtr=LastPtr0;CurLastPtr=CurLastPtr0;}
    void GetSimilar(char *Str,int LastCmdPartLength);
    void SetAddMode(int EnableAdd,int RemoveDups,int KeepSelectedPos);
};

#endif  // __HISTORY_HPP__
