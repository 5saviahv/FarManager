#ifndef __LANGUAGE_HPP__
#define __LANGUAGE_HPP__
/*
language.hpp

����� � LNG-䠩����

*/

/* Revision: 1.00 25.06.2000 $ */

/*
Modify:
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#ifndef __STDIO_H
#include <stdio.h>
#endif

class Language
{
  private:
    void ConvertString(char *Src,char *Dest);
    char **MsgAddr;
    char *MsgList;
    long MsgSize;
    int MsgCount;
    char MessageFile[NM];
  public:
    Language();
    int Init(char *Path);
    void Close();
    char* GetMsg(int MsgId);
    static FILE* OpenLangFile(char *Path,char *Mask,char *Language,char *FileName);
    static int GetLangParam(FILE *SrcFile,char *ParamName,char *Param1,char *Param2);
    static int Select(int HelpLanguage,VMenu **MenuPtr);
};

#endif	// __LANGUAGE_HPP__
