/*
strmix.cpp

���� ������ ��������������� ������� �� ������ �� ��������

*/

/* Revision: 1.29 19.11.2001 $ */

/*
Modify:
  19.11.2001 SVS
    + ReplaceStrings - ������ ���������
  26.10.2001 SVS
    ! � HiStrlen() �����! ��������� Str �� NULL
  23.10.2001 SVS
    - ��������� ����� � CenterStr()
  22.10.2001 SVS
    ! strncpy() ;-(
  21.10.2001 SVS
    + Addeded WordWrap
  07.10.2001 SVS
    + InsertString()
  01.10.2001 IS
    + TruncStrFromEnd - �������� ������ � ������ �������
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  26.09.2001 SVS
    + DeleteEndSlash (� ������ IS)
    ! AddEndSlash ����� ������������ ��� BOOL
  03.08.2001 IS
    ! InsertQuote ������ �� static
  24.07.2001 IS
    ! ������ �������� �� '\r' � '\n' �� ����� iseol
  11.07.2001 SVS
    ! HiStrlen � RemoveChar - �������������� �������� - ��� ������, ���� '&&'
      "������� �� ��� ��� ��������� ������ ���� ������"
  06.07.2001 IS
    ! ����� CopyMaskStr, ����� ������� ��������
  02.07.2001 IS
    ! ����� ��������� '-' � '^' �� QuoteSpace. ��� ����������� �������, ��
      ����� �� ����� � �������. ���� ��������� - �������� ����.
  25.06.2001 IS
    ! ��������� const
  06.06.2001 VVM
    ! ����-���� ���������� Unquote()
  05.06.2001 VVM
    + ��������� Unquote()
  07.05.2001 DJ
    * ���������������� ����� CmpName() �������� � ��� ���� � �����������
      �������
    + CopyMaskStr()
  06.05.2001 DJ
    ! �������� #include
  22.04.2001 SVS
    + Opt.QuotedSymbols - ����������� ��� QuoteSpace()
    ! ������� ����������� � Unquote
  13.04.2001 SVS
    ! ���������� QuoteSpaceOnly & QuoteSpace - ����� ��� ������� ��
      ��������������� ������� InsertQuote � ��������� �������� �� �����������
      ������� � ������ � � ����� ������.
  08.04.2001 SVS
    ! GetCommaWord() - �������������� �������� - �����������, �� ��������� = ','
  20.03.2001 SVS
    + FileSizeToStr - ������� �������������� ������� ����� �... �������������
      ��� - ������� �� FileList::ShowList()
  13.03.2001 SVS
    ! GetPathRoot ��������� � flink.cpp :-)
  12.03.2001 SVS
    ! ��������� � ����� � ����������� � ������ int64
  06.03.2001 SVS
    ! ������� ����������� � TruncStr() - ����������� �� ������� ������ new[]
    ! ������� ����������� � InsertCommas() - ����������� �� ������� sprintf()
  05.03.2001 SVS
    ! ������� ����������� � ������� QuoteSpace � QuoteSpaceOnly -
      ����� ������ malloc()
  28.02.2001 SVS
    ! CenterStr ���������� ��������� �� Dest
  22.02.2001 IS
    + RemoveChar - ������� ������ �� ������
    ! RemoveHighlights(Str) ��� ������ (� fn.hpp) - �������� RemoveChar(Str,'&')
  02.02.2001 IS
    + ������� RemoveUnprintableCharacters - �������� ��������� ����������
      ������� � ������. � ��������� ������ �������������� ������ cr � lf.
  05.01.2001 SVS
    ! ��������� � �������� ���������������� ������
    + ������� InsertCommas, PointToName, GetPathRoot, CmpName, ConvertWildcards,
      QuoteSpace, QuoteSpaceOnly, TruncStr, TruncPathStr, Remove???Spaces,
      HiStrlen, AddEndSlash, NullToEmpty, CenterStr, GetCommaWord,
      RemoveHighlights, IsCaseMixed, IsCaseLower, Unquote,
      ��������� �� mix.cpp
*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "global.hpp"
#include "lang.hpp"

char *InsertCommas(unsigned long Number,char *Dest)
{
  ultoa(Number,Dest,10);
  for (int I=strlen(Dest)-4;I>=0;I-=3)
    if (Dest[I])
    {
      memmove(Dest+I+2,Dest+I+1,strlen(Dest+I));
      Dest[I+1]=',';
    }
  return Dest;
}


char* InsertCommas(int64 li,char *Dest)
{
  if (li<1000000000)
    InsertCommas(li.PLow(),Dest);
  else
  {
    li.itoa(Dest);
    for (int I=strlen(Dest)-4;I>=0;I-=3)
      if (Dest[I])
      {
        memmove(Dest+I+2,Dest+I+1,strlen(Dest+I));
        Dest[I+1]=',';
      }
  }
  return Dest;
}

char* WINAPI PointToName(char *Path)
{
  char *NamePtr=Path;
  while (*Path)
  {
    if (*Path=='\\' || *Path=='/' || *Path==':' && Path==NamePtr+1)
      NamePtr=Path+1;
    Path++;
  }
  return(NamePtr);
}

int CmpName(const char *pattern,const char *string,int skippath)
{
  char stringc,patternc,rangec;
  int match;
  static int depth=0;

  if (skippath)
    string=PointToName(const_cast<char*>(string));

  for (;; ++string)
  {
    /* $ 01.05.2001 DJ
       ���������� ���������� ������
    */
    stringc=LocalUpperFast(*string);
    patternc=LocalUpperFast(*pattern++);
    /* DJ $ */
    switch (patternc)
    {
      case 0:
        return(stringc==0);
      case '?':
        if (stringc == 0)
          return(FALSE);

        break;
      case '*':
        if (!*pattern)
          return(TRUE);

        /* $ 01.05.2001 DJ
           ���������������� ����� �������� � ��� ���� � �����������
           �������
        */
        if (*pattern=='.')
        {
          if (pattern[1]=='*' && pattern[2]==0 && depth==0)
            return(TRUE);
          if (strpbrk (pattern, "*?[") == NULL)
          {
            const char *dot = strrchr (string, '.');
            if (pattern[1] == 0)
              return (dot==NULL || dot[1]==0);
            const char *patdot = strchr (pattern+1, '.');
            if (patdot != NULL && dot == NULL)
              return(FALSE);
            if (patdot == NULL && dot != NULL)
              return(LocalStricmp (pattern+1,dot+1) == 0);
          }
        }
        /* DJ $ */

        while (*string)
        {
          depth++;
          int CmpCode=CmpName(pattern,string++,FALSE);
          depth--;
          if (CmpCode)
            return(TRUE);
        }
        return(FALSE);
      case '[':
        if (strchr(pattern,']')==NULL)
        {
          if (patternc != stringc)
            return (FALSE);
          break;
        }
        if (*pattern && *(pattern+1)==']')
        {
          if (*pattern!=*string)
            return(FALSE);
          pattern+=2;
          break;
        }
        match = 0;
        while ((rangec = LocalUpper(*pattern++))!=0)
        {
          if (rangec == ']')
            if (match)
              break;
            else
              return(FALSE);
          if (match)
            continue;
          if (rangec == '-' && *(pattern - 2) != '[' && *pattern != ']')
          {
            match = (stringc <= LocalUpper(*pattern) &&
                     LocalUpper(*(pattern - 2)) <= stringc);
            pattern++;
          }
          else
            match = (stringc == rangec);
        }
        if (rangec == 0)
          return(FALSE);
        break;
      default:
        if (patternc != stringc)
          if (patternc=='.' && stringc==0 && !CmpNameSearchMode)
            return(*pattern!='.' && CmpName(pattern,string));
          else
            return(FALSE);
        break;
    }
  }
}

/* $ 09.10.2000 IS
    ��������� ������ ����� �� �����
    (���� �� ShellCopy::ShellCopyConvertWildcards)
*/
// �� ������ ����� ����� (Src) � ����� (Dest) ���������� ����� ���
// SelectedFolderNameLength - ����� ��������. ��������, ����
// ������� dir1, � � ��� ���� file1. ����� ������������� ��� �� ����� ��� dir1.
// ��������� ����� ���� ����������: Src="dir1", SelectedFolderNameLength=0
// ��� Src="dir1\\file1", � SelectedFolderNameLength=4 (����� "dir1")
int ConvertWildcards(const char *src,char *Dest, int SelectedFolderNameLength)
{
  char WildName[2*NM], Src[NM], *CurWildPtr,*DestNamePtr,*SrcNamePtr;
  char PartBeforeName[NM],PartAfterFolderName[NM];
  DestNamePtr=PointToName(Dest);
  strcpy(WildName,DestNamePtr);
  if (strchr(WildName,'*')==NULL && strchr(WildName,'?')==NULL)
    return(FALSE);

  strncpy(Src, src, sizeof(Src)-1);

  if (SelectedFolderNameLength!=0)
  {
    strcpy(PartAfterFolderName,Src+SelectedFolderNameLength);
    Src[SelectedFolderNameLength]=0;
  }

  SrcNamePtr=PointToName(Src);

  int BeforeNameLength=DestNamePtr==Dest ? SrcNamePtr-Src:0;
  strncpy(PartBeforeName,Src,BeforeNameLength);
  PartBeforeName[BeforeNameLength]=0;

  char *SrcNameDot=strrchr(SrcNamePtr,'.');
  CurWildPtr=WildName;
  while (*CurWildPtr)
    switch(*CurWildPtr)
    {
      case '?':
        CurWildPtr++;
        if (*SrcNamePtr)
          *(DestNamePtr++)=*(SrcNamePtr++);
        break;
      case '*':
        CurWildPtr++;
        while (*SrcNamePtr)
        {
          if (*CurWildPtr=='.' && SrcNameDot!=NULL && strchr(CurWildPtr+1,'.')==NULL)
          {
            if (SrcNamePtr==SrcNameDot)
              break;
          }
          else
            if (*SrcNamePtr==*CurWildPtr)
              break;
          *(DestNamePtr++)=*(SrcNamePtr++);
        }
        break;
      case '.':
        CurWildPtr++;
        *(DestNamePtr++)='.';
        if (strpbrk(CurWildPtr,"*?")!=NULL)
          while (*SrcNamePtr)
            if (*(SrcNamePtr++)=='.')
              break;
        break;
      default:
        *(DestNamePtr++)=*(CurWildPtr++);
        if (*SrcNamePtr && *SrcNamePtr!='.')
          SrcNamePtr++;
        break;
    }

  *DestNamePtr=0;
  if (DestNamePtr!=Dest && *(DestNamePtr-1)=='.')
    *(DestNamePtr-1)=0;
  if (*PartBeforeName)
  {
    strcat(PartBeforeName,Dest);
    strcpy(Dest,PartBeforeName);
  }
  if (SelectedFolderNameLength!=0)
    strcat(Src,PartAfterFolderName);
  return(TRUE);
}
/* IS $ */

char *InsertQuote(char *Str)
{
  unsigned l = strlen(Str);
  if(*Str != '"')
  {
    memmove(Str+1,Str,++l);
    *Str='"';
  }
  if(Str[l-1] != '"')
  {
    Str[l++] = '\"';
    Str[l] = 0;
  }
  return Str;
}

char* QuoteSpace(char *Str)
{
  /* $ 02.07.2001 IS
     � ������ ����� � ������� ����� � '^' � '-'?? ���������� �� �����������
     �������...
  */
  if (/**Str=='-' || *Str=='^' || */strpbrk(Str,Opt.QuotedSymbols)!=NULL)
    InsertQuote(Str);
  /* IS $ */
  return(Str);
}

char* WINAPI QuoteSpaceOnly(char *Str)
{
  if (Str && strchr(Str,' ')!=NULL)
    InsertQuote(Str);
  return(Str);
}

char* WINAPI TruncStrFromEnd(char *Str, int MaxLength)
{
  if(Str)
  {
    int Length = strlen(Str);
    if (Length > MaxLength)
    {
      if(MaxLength>3) memcpy(Str+MaxLength-3,"...",3);
      Str[MaxLength]=0;
    }
  }
  return Str;
}

char* WINAPI TruncStr(char *Str,int MaxLength)
{
  if(Str)
  {
    int Length;
    if (MaxLength<0)
      MaxLength=0;
    if ((Length=strlen(Str))>MaxLength)
#if 0
      if (MaxLength>3)
      {
        char *TmpStr=new char[MaxLength+5];
        sprintf(TmpStr,"...%s",Str+Length-MaxLength+3);
        strcpy(Str,TmpStr);
        /* $ 13.07.2000 SVS
           �� � ����� ��� �� ������� new[], �� � ������� � delete[] ����... */
        delete[] TmpStr;
        /* SVS $ */
      }
      else
        Str[MaxLength]=0;
#else
    {
      if (MaxLength>3)
      {
        memmove(Str+3,Str+Length-MaxLength+3,MaxLength);
        memcpy(Str,"...",3);
      }
      Str[MaxLength]=0;
    }
#endif
  }
  return(Str);
}


char* WINAPI TruncPathStr(char *Str,int MaxLength)
{
  if(Str)
  {
    char *Root=NULL;
    if (Str[0]!=0 && Str[1]==':' && Str[2]=='\\')
      Root=Str+3;
    else
      if (Str[0]=='\\' && Str[1]=='\\' && (Root=strchr(Str+2,'\\'))!=NULL &&
          (Root=strchr(Root+1,'\\'))!=NULL)
        Root++;
    if (Root==NULL || Root-Str+5>MaxLength)
      return(TruncStr(Str,MaxLength));
    int Length=strlen(Str);
    if (Length>MaxLength)
    {
      char *MovePos=Root+Length-MaxLength+3;
      memmove(Root+3,MovePos,strlen(MovePos)+1);
      memcpy(Root,"...",3);
    }
  }
  return(Str);
}

/* $ 07.07.2000 SVS
    + �������������� ������� ��������� �����: RemoveExternalSpaces
    ! ������� Remove*Spaces ���������� char*
*/
// ������� ������� �������
char* WINAPI RemoveLeadingSpaces(char *Str)
{
  char *ChPtr;
  if(Str)
  {
    for (ChPtr=Str;isspace(*ChPtr);ChPtr++)
           ;
    if (ChPtr!=Str)
      memmove(Str,ChPtr,strlen(ChPtr)+1);
  }
  return Str;
}


// ������� �������� �������
char* WINAPI RemoveTrailingSpaces(char *Str)
{
  if(Str)
  {
    for (int I=strlen((char *)Str)-1;I>=0;I--)
      if (isspace(Str[I]) || iseol(Str[I]))
        Str[I]=0;
      else
        break;
  }
  return Str;
}

// ������� ������� �������
char* WINAPI RemoveExternalSpaces(char *Str)
{
  return RemoveTrailingSpaces(RemoveLeadingSpaces(Str));
}
/* SVS $ */

/* $ 02.02.2001 IS
   �������� ��������� ���������� ������� � ������. � ��������� ������
   �������������� ������ cr � lf.
*/
char* WINAPI RemoveUnprintableCharacters(char *Str)
{
 char *p=Str;
 while(*p)
 {
   if(iseol(*p)) *p=' ';
   p++;
 }
 return RemoveExternalSpaces(Str);
}
/* IS $ */

// ������� ������ Target �� ������ Str (�����!)
char *RemoveChar(char *Str,char Target,BOOL Dup)
{
  char *Ptr = Str, *StrBegin = Str, Chr;
  while((Chr=*Str++) != 0)
  {
    if(Chr == Target)
    {
      if(Dup && *Str == Target)
      {
        *Ptr++ = Chr;
        ++Str;
      }
      continue;
    }
    *Ptr++ = Chr;
  }
  *Ptr = '\0';
  return StrBegin;
}

int HiStrlen(const char *Str,BOOL Dup)
{
  int Length=0;

  if(Str && *Str)
    while (*Str)
    {
      if (*Str!='&')
        Length++;
      else
        if(Dup && Str[1] == '&')
        {
          Length++;
          ++Str;
        }
      Str++;
    }
  return(Length);
}


BOOL WINAPI AddEndSlash(char *Path)
{
  BOOL Result=FALSE;
  if(Path)
  {
    /* $ 06.12.2000 IS
      ! ������ ������� �������� � ������ ������ ������, ����� ����������
        ��������� ��� ������������� ��������� ����� �� �����, �������
        ����������� ����.
    */
    char *end=Path;
    int Slash=0, BackSlash=0;
    while(*end)
    {
     Slash+=(*end=='\\');
     BackSlash+=(*end=='/');
     end++;
    }
    int Length=end-Path;
    char c=(Slash<BackSlash)?'/':'\\';
    Result=TRUE;
    if (Length==0)
    {
       *end=c;
       end[1]=0;
    }
    else
    {
     end--;
     if (*end!='\\' && *end!='/')
     {
       end[1]=c;
       end[2]=0;
     }
     else
       *end=c;
    }
    /* IS $ */
  }
  return Result;
}

BOOL WINAPI DeleteEndSlash(char *Path)
{
  BOOL Result=FALSE;
  if(Path)
  {
    int Length=strlen(Path)-1;
    if (Length >= 0 && Path[Length]=='\\')
    {
      Path[Length]=0;
      Result = TRUE;
    }
  }
  return Result;
}

char *NullToEmpty(char *Str)
{
  return (Str==NULL) ? "":Str;
}

const char *NullToEmpty(const char *Str)
{
  return (Str==NULL) ? "":Str;
}


char* CenterStr(char *Src,char *Dest,int Length)
{
  char TempSrc[1024];
  strncpy(TempSrc,Src,sizeof(TempSrc)-1);
  int SrcLength=strlen(Src);
  if (SrcLength >= Length)
    strncpy(Dest,TempSrc,Length-1);
  else
  {
    int Space=(Length-SrcLength)/2;
    sprintf(Dest,"%*s%s%*s",Space,"",TempSrc,Length-Space-SrcLength,"");
  }
  return Dest;
}


/* $ 08.04.2001 SVS
  + �������������� �������� - �����������, �� ��������� = ','
*/
const char *GetCommaWord(const char *Src,char *Word,char Separator)
{
  int WordPos,SkipBrackets;
  if (*Src==0)
    return(NULL);
  SkipBrackets=FALSE;
  for (WordPos=0;*Src!=0;Src++,WordPos++)
  {
    if (*Src=='[' && strchr(Src+1,']')!=NULL)
      SkipBrackets=TRUE;
    if (*Src==']')
      SkipBrackets=FALSE;
    if (*Src==Separator && !SkipBrackets)
    {
      Word[WordPos]=0;
      Src++;
      while (isspace(*Src))
        Src++;
      return(Src);
    }
    else
      Word[WordPos]=*Src;
  }
  Word[WordPos]=0;
  return(Src);
}
/* SVS $ */

int IsCaseMixed(char *Str)
{
  while (*Str && !LocalIsalpha(*Str))
    Str++;
  int Case=LocalIslower(*Str);
  while (*(Str++))
    if (LocalIsalpha(*Str) && LocalIslower(*Str)!=Case)
      return(TRUE);
  return(FALSE);
}


int IsCaseLower(char *Str)
{
  for (;*Str!=0;Str++)
    if (LocalIsalpha(*Str) && !LocalIslower(*Str))
      return(FALSE);
  return(TRUE);
}


/* $ 08.06.2001 VVM
  + ����� ������� _���_ �������. � ��� �� ����? */
/* $ 05.06.2001 VVM
  ������ �� ����� ���� �� ������ :)
  � ������� ���� ������� � ������ ������ */
/* $ 28.06.2000 IS
  ������ ������� Unquote ������� ��� ��������� � �������������� �������
*/
/* $ 25.07.2000 SVS
   ����� WINAPI
*/
void WINAPI Unquote(char *Str)
{
  if (!Str)
    return;
  while (*Str)
  {
    if (*Str=='\"')
      strcpy(Str, Str+1);
    else
      Str++;
  }
}
/* IS $ */
/* VVM $ */
/* VVM $ */

/* FileSizeToStr()
   �������������� ������� ����� � ������������� ���.
*/
char* WINAPI FileSizeToStr(char *DestStr,DWORD SizeHigh, DWORD Size, int Width, int ViewFlags)
{
  char OutStr[64];
  char Str[100];

  int Commas=(ViewFlags & COLUMN_COMMAS);

  if (SizeHigh)
  {
    DWORD SizeMb=SizeHigh*4096+Size/(1024*1024);
    if (Commas)
      InsertCommas(SizeMb,Str);
    else
      sprintf(Str,"%u",SizeMb);
    sprintf(OutStr,"%s %1.1s",Str,MSG(MListMb));
    sprintf(DestStr,"%*.*s",Width,Width,OutStr);
    return DestStr;
  }
  if (Commas)
    InsertCommas(Size,Str);
  else
    sprintf(Str,"%u",Size);
  int SizeLength=strlen(Str);
  if (SizeLength<=Width || Width<5)
    sprintf(DestStr,"%*.*s",Width,Width,Str);
  else
  {
    char KbStr[100],MbStr[100];
    int Divider;
    Width-=2;
    strcpy(KbStr,MSG(MListKb));
    strcpy(MbStr,MSG(MListMb));
    if (ViewFlags & COLUMN_THOUSAND)
    {
      Divider=1000;
      LocalStrlwr(KbStr);
      LocalStrlwr(MbStr);
    }
    else
    {
      Divider=1024;
      LocalStrupr(KbStr);
      LocalStrupr(MbStr);
    }
    Size/=Divider;
    if (Commas)
      InsertCommas(Size,Str);
    else
      sprintf(Str,"%u",Size);
    if (strlen(Str)<=Width)
      sprintf(DestStr,"%*.*s %1.1s",Width,Width,Str,KbStr);
    else
    {
      Size/=Divider;
      if (Commas)
        InsertCommas(Size,Str);
      else
        sprintf(Str,"%u",Size);
      sprintf(DestStr,"%*.*s %1.1s",Width,Width,Str,MbStr);
    }
  }
  return DestStr;
}

// �������� � ������� Pos � Str ������ InsStr (�������� InsSize ����)
// ���� InsSize = 0, ��... ��������� ��� ������ InsStr
// ���������� ��������� �� Str
char *InsertString(char *Str,int Pos,const char *InsStr,int InsSize)
{
  int InsLen=strlen(InsStr);
  if(InsSize && InsSize > InsLen)
    InsLen=InsSize;
  memmove(Str+Pos+InsLen, Str+Pos, strlen(Str+Pos)+1);
  memcpy(Str+Pos, InsStr, InsLen);
  return Str;
}

// �������� � ������ Str Count ��������� ��������� FindStr �� ��������� ReplStr
// ���� Count < 0 - �������� "�� ������ ������"
// Return - ���������� �����
int ReplaceStrings(char *Str,const char *FindStr,const char *ReplStr,int Count)
{
  int I=0, J=0;
  int LenReplStr=strlen(ReplStr);
  int LenFindStr=strlen(FindStr);

  while(I <= strlen(Str)-LenFindStr)
  {
    if(memcmp(Str+I, FindStr, LenFindStr) == 0)
    {
      if(LenReplStr > LenFindStr)
        memmove(Str+I+(LenReplStr-LenFindStr),Str+I,strlen(Str+I)+1); // >>
      else if(LenReplStr < LenFindStr)
        memmove(Str+I,Str+I+(LenFindStr-LenReplStr),strlen(Str+I+(LenFindStr-LenReplStr))+1); //??
      memcpy(Str+I,ReplStr,LenReplStr);
      I += LenReplStr;
      if(Count > 0 && ++J == Count)
        break;
    }
    else
      I++;
  }
  return J;
}

/*
From PHP 4.x.x
����������� ����� �� ��������� ���������� ��������, ��������� ��������������
������. ���������� ������ SrcText �������� � �������, �������� ����������
Width. ������ ������� ��� ������ ��������� Break.

���� �������� Cut ���������� � 1, �� ������ ������ ������������� �� ��������
������. ��� ���� � ��� ���� �����, ������� ������ �������� ������, �� ���
����� ��������� �� �����.

Example 1.
wordwrap( "The quick brown fox jumped over the lazy dog.", 20 ,"\n", 0);
���� ������ ���������:
---
The quick brown fox
jumped over the lazy dog.
---

Example 2.
wordwrap( "A very long woooooooooooord.", 8, "\n", 1);
���� ������ ���������:

---
A very
long
wooooooo
ooooord.
---

*/
char *WINAPI WordWrap(const char *SrcText,int Width,
                      char *DestText,int MaxLen,
                      const char* Break, int Cut)
{
  long i=0, l=0, pgr=0, linelength=0, last=0, breakcharlen, docut=0;
  const char *text, *breakchar;
  char *newtext;

  text = SrcText;
  linelength = Width;
  breakchar = Break; // "\n"
  breakcharlen = strlen(Break);
  docut = Cut;

  /* Special case for a single-character break as it needs no
     additional storage space */

  if (breakcharlen == 1 && docut == 0)
  {
    newtext = strdup (text);
    if(!newtext)
      return NULL;

    while (newtext[i] != '\0')
    {
      /* prescan line to see if it is greater than linelength */
      l = 0;
      while (newtext[i+l] != breakchar[0])
      {
        if (newtext[i+l] == '\0')
        {
          l--;
          break;
        }
        l++;
      }
      if (l >= linelength)
      {
        pgr = l;
        l = linelength;
        /* needs breaking; work backwards to find previous word */
        while (l >= 0)
        {
          if (newtext[i+l] == ' ')
          {
            newtext[i+l] = breakchar[0];
            break;
          }
          l--;
        }
        if (l == -1)
        {
          /* couldn't break is backwards, try looking forwards */
          l = linelength;
          while (l <= pgr)
          {
            if(newtext[i+l] == ' ')
            {
              newtext[i+l] = breakchar[0];
              break;
            }
            l++;
          }
        }
      }
      i += l+1;
    }
  }
  else
  {
    /* Multiple character line break */
    newtext = (char*)malloc(strlen(SrcText) * (breakcharlen+1)+1);
    if(!newtext)
      return NULL;

    newtext[0] = '\0';

    i = 0;
    while (text[i] != '\0')
    {
      /* prescan line to see if it is greater than linelength */
      l = 0;
      while (text[i+l] != '\0')
      {
        if (text[i+l] == breakchar[0])
        {
          if (breakcharlen == 1 || strncmp(text+i+l, breakchar, breakcharlen)==0)
            break;
        }
        l++;
      }
      if (l >= linelength)
      {
        pgr = l;
        l = linelength;

        /* needs breaking; work backwards to find previous word */
        while (l >= 0)
        {
          if (text[i+l] == ' ')
          {
            strncat(newtext, text+last, i+l-last);
            strcat(newtext, breakchar);
            last = i + l + 1;
            break;
          }
          l--;
        }
        if (l == -1)
        {
          /* couldn't break it backwards, try looking forwards */
          l = linelength - 1;
          while (l <= pgr)
          {
            if (docut == 0)
            {
              if (text[i+l] == ' ')
              {
                strncat(newtext, text+last, i+l-last);
                strcat(newtext, breakchar);
                last = i + l + 1;
                break;
              }
            }
            if (docut == 1)
            {
              if (text[i+l] == ' ' || l > i-last)
              {
                strncat(newtext, text+last, i+l-last+1);
                strcat(newtext, breakchar);
                last = i + l + 1;
                break;
              }
            }
            l++;
          }
        }
        i += l+1;
      }
      else
      {
        i += (l ? l : 1);
      }
    }

    if (i+l > last)
    {
      strcat(newtext, text+last);
    }
  }
  strncpy(DestText,newtext,MaxLen-1);
  free(newtext);
  return DestText;
}


#if defined(MOUSEKEY)
/*
  Ptr=CalcWordFromString(Str,I,&Start,&End);
  strncpy(Dest,Ptr,End-Start+1);
  Dest[End-Start+1]=0;

  ���������� ��������� �� ������ �����
*/
const char * const CalcWordFromString(const char *Str,int CurPos,int *Start,int *End)
{
  int I, J, StartWPos, EndWPos;
  DWORD DistLeft, DistRight;
  int StrSize=strlen(Str);
  char WordDiv[sizeof(Opt.WordDiv)+16];
  strcpy(WordDiv,Opt.WordDiv);
  strcat(WordDiv," \t\n\r");

  if(strchr(WordDiv,Str[CurPos]))
  {
    // ��������� ��������� - ���� ������, ��� ����� ����� - ����� ��� ������
    I=J=CurPos;

    // ������ �����
    DistLeft=-1;
    while(I >= 0 && strchr(WordDiv,Str[I]))
    {
      DistLeft++;
      I--;
    }
    if(I < 0)
      DistLeft=-1;

    // ������ ������
    DistRight=-1;
    while(J < StrSize && strchr(WordDiv,Str[J]))
    {
      DistRight++;
      J++;
    }
    if(J >= StrSize)
      DistRight=-1;

    if(DistLeft > DistRight) // ?? >=
      EndWPos=StartWPos=J;
    else
      EndWPos=StartWPos=I;
  }
  else // ����� ��� ���, �.�. ����� �� �������
    EndWPos=StartWPos=CurPos;

  while(StartWPos >= 0)
    if(strchr(WordDiv,Str[StartWPos]))
    {
      StartWPos++;
      break;
    }
    else
      StartWPos--;
  while(EndWPos < StrSize)
    if(strchr(WordDiv,Str[EndWPos]))
    {
      EndWPos--;
      break;
    }
    else
      EndWPos++;

  if(StartWPos < 0)
    StartWPos=0;
  if(EndWPos >= StrSize)
    EndWPos=StrSize;

  *Start=StartWPos;
  *End=EndWPos;

  return Str+StartWPos;
}
#endif
