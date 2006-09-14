#ifndef __GETFILESTRING_HPP__
#define __GETFILESTRING_HPP__
/*
filestr.hpp

����� GetFileString

*/

/* Revision: 1.02 25.05.2006 $ */

/*
Modify:
  09.04.2001 SVS
    ! stdio.h ��� � ��� ����������.
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

class GetFileString
{
  private:
    char ReadBuf[8192];
    wchar_t wReadBuf[8192];
    int ReadPos,ReadSize;
    char *Str;
    wchar_t *wStr;
    int m_nStrLength;
    FILE *SrcFile;

    int GetAnsiString(char **DestStr,int &Length);
    int GetUnicodeString(wchar_t **DestStr,int &Length);
    int GetReverseUnicodeString(wchar_t **DestStr,int &Length);

  public:
    GetFileString(FILE *SrcFile);
    ~GetFileString();
    int GetStringW(wchar_t **DestStr, int nCodePage, int &Length);
};

#endif  // __GETFILESTRING_HPP__
