#ifndef __LANGUAGE_HPP__
#define __LANGUAGE_HPP__
/*
language.hpp

������ � LNG-�������

*/

#include "farconst.hpp"

class VMenu;

class Language
{
  private:
    char **MsgAddr;
    char *MsgList;
    long MsgSize;
    int MsgCount;
    char MessageFile[NM];

  private:
    void ConvertString(char *Src,char *Dest);
    BOOL CheckMsgId(int MsgId);
    void Free();

  public:
    Language();
    ~Language();

  public:
    int Init(char *Path,int CountNeed=-1);
    void Close();
    char* GetMsg(int MsgId);

    static FILE* OpenLangFile(const char *Path,const char *Mask,const char *Language,char *FileName,BOOL StrongLang=FALSE);
    static int GetLangParam(FILE *SrcFile,char *ParamName,char *Param1,char *Param2);
    /* $ 01.09.2000 SVS
      + ����� �����, ��� ��������� ���������� ��� .Options
        .Options <KeyName>=<Value>
    */
    static int GetOptionsParam(FILE *SrcFile,char *KeyName,char *Value);
    /* SVS $ */
    static int Select(int HelpLanguage,VMenu **MenuPtr);
};

extern Language Lang;

#endif  // __LANGUAGE_HPP__
