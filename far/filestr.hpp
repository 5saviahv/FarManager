#ifndef __GETFILESTRING_HPP__
#define __GETFILESTRING_HPP__
/*
filestr.hpp

����� GetFileString

*/

/* Revision: 1.01 09.04.2001 $ */

/*
Modify:
  09.04.2001 SVS
    ! stdio.h 㦥 � ⠪ ����砥���.
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

class GetFileString
{
  private:
    char ReadBuf[8192];
    int ReadPos,ReadSize;
    char *Str;
    int StrLength;
    FILE *SrcFile;
  public:
    GetFileString(FILE *SrcFile);
    ~GetFileString();
    int GetString(char **DestStr,int &Length);
};

#endif	// __GETFILESTRING_HPP__
