/*
strmix.cpp

��� ࠧ��� �ᯮ����⥫��� �㭪権 �� ࠡ�� � ��ப���

*/

/* Revision: 1.05 12.03.2001 $ */

/*
Modify:
  12.03.2001 SVS
    ! ���४�� � �裡 � ��������ﬨ � ����� int64
  06.03.2001 SVS
    ! ������� ��⨬���樨 � TruncStr() - ������塞�� �� ��譥�� �맮�� new[]
    ! ������� ��⨬���樨 � InsertCommas() - ������塞�� �� ��譥�� sprintf()
  05.03.2001 SVS
    ! ������� ��⨬���樨 � ����� QuoteSpace � QuoteSpaceOnly -
      �ࠫ ��譨� malloc()
  28.02.2001 SVS
    ! CenterStr �����頥� 㪠��⥫� �� Dest
  22.02.2001 IS
    + RemoveChar - 㤠��� ᨬ��� �� ��ப�
    ! RemoveHighlights(Str) ��� ����� (� fn.hpp) - ��뢠�� RemoveChar(Str,'&')
  02.02.2001 IS
    + �㭪�� RemoveUnprintableCharacters - ������� �஡����� �������
      ᨬ���� � ��ப�. � �����騩 ������ ��ࠡ��뢠���� ⮫쪮 cr � lf.
  05.01.2001 SVS
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
    + �㭪樨 InsertCommas, PointToName, GetPathRoot, CmpName, ConvertWildcards,
      QuoteSpace, QuoteSpaceOnly, TruncStr, TruncPathStr, Remove???Spaces,
      HiStrlen, AddEndSlash, NullToEmpty, CenterStr, GetCommaWord,
      RemoveHighlights, IsCaseMixed, IsCaseLower, Unquote,
      ��॥堫� �� mix.cpp
*/

#include "headers.hpp"
#pragma hdrstop
#include "internalheaders.hpp"

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


void WINAPI GetPathRoot(char *Path,char *Root)
{
  char TempRoot[NM],*ChPtr;
  strncpy(TempRoot,Path,NM);
  if (*TempRoot==0)
    strcpy(TempRoot,"\\");
  else
    if (TempRoot[0]=='\\' && TempRoot[1]=='\\')
    {
      if ((ChPtr=strchr(TempRoot+2,'\\'))!=NULL)
        if ((ChPtr=strchr(ChPtr+1,'\\'))!=NULL)
          *(ChPtr+1)=0;
        else
          strcat(TempRoot,"\\");
    }
    else
      if ((ChPtr=strchr(TempRoot,'\\'))!=NULL)
        *(ChPtr+1)=0;
      else
        if ((ChPtr=strchr(TempRoot,':'))!=NULL)
          strcpy(ChPtr+1,"\\");
  strncpy(Root,TempRoot,NM);
}

int CmpName(char *pattern,char *string,int skippath)
{
  char stringc,patternc,rangec;
  int match;
  static int depth=0;

  if (skippath)
    string=PointToName(string);

  for (;; ++string)
  {
    stringc=LocalUpper(*string);
    patternc=LocalUpper(*pattern++);
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

        if (*pattern=='.')
        {
          if (pattern[1]=='*' && pattern[2]==0 && depth==0)
            return(TRUE);
          char *dot=strchr(string,'.');
          if (pattern[1]==0)
            return (dot==NULL || dot[1]==0);
          if (dot!=NULL)
          {
            string=dot;
            if (strpbrk(pattern,"*?[")==NULL && strchr(string+1,'.')==NULL)
              return(LocalStricmp(pattern+1,string+1)==0);
          }
        }

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
    ������� ������ ����� �� ��᪥
    (��� �� ShellCopy::ShellCopyConvertWildcards)
*/
// �� �᭮�� ����� 䠩�� (Src) � ��᪨ (Dest) ������㥬 ����� ���
// SelectedFolderNameLength - ����� ��⠫���. ���ਬ��, ����
// ��⠫�� dir1, � � ��� 䠩� file1. �㦭� ᣥ���஢��� ��� �� ��᪥ ��� dir1.
// ��ࠬ���� ����� ���� ᫥���騬�: Src="dir1", SelectedFolderNameLength=0
// ��� Src="dir1\\file1", � SelectedFolderNameLength=4 (����� "dir1")
int ConvertWildcards(char *Src,char *Dest, int SelectedFolderNameLength)
{
  char WildName[2*NM],*CurWildPtr,*DestNamePtr,*SrcNamePtr;
  char PartBeforeName[NM],PartAfterFolderName[NM];
  DestNamePtr=PointToName(Dest);
  strcpy(WildName,DestNamePtr);
  if (strchr(WildName,'*')==NULL && strchr(WildName,'?')==NULL)
    return(FALSE);

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

char* QuoteSpace(char *Str)
{
  if (*Str=='-' || *Str=='^' || strpbrk(Str," &+,")!=NULL)
  {
    unsigned l = strlen(Str);
    memmove(Str+1,Str,l);
    *Str = Str[l+1] = '\"';
    Str[l+2] = 0;
  }
  return(Str);
}


char* WINAPI QuoteSpaceOnly(char *Str)
{
  if(Str)
  {
    if (strchr(Str,' ')!=NULL)
    {
      unsigned l = strlen(Str);
      memmove(Str+1,Str,l);
      *Str = Str[l+1] = '\"';
      Str[l+2] = 0;
    }
  }
  return(Str);
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
           �� � ����� ࠧ � �맢��� new[], � � �ਤ��� � delete[] ����... */
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
    + �������⥫쭠� �㭪�� ��ࠡ�⪨ ��ப: RemoveExternalSpaces
    ! �㭪樨 Remove*Spaces �������� char*
*/
// 㤠���� ����騥 �஡���
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


// 㤠���� ������ �஡���
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

// 㤠���� �஡��� ᭠�㦨
char* WINAPI RemoveExternalSpaces(char *Str)
{
  return RemoveTrailingSpaces(RemoveLeadingSpaces(Str));
}
/* SVS $ */

/* $ 02.02.2001 IS
   ������� �஡����� ������� ᨬ���� � ��ப�. � �����騩 ������
   ��ࠡ��뢠���� ⮫쪮 cr � lf.
*/
char* WINAPI RemoveUnprintableCharacters(char *Str)
{
 char *p=Str;
 while(*p)
 {
   if('\n'==*p || '\r'==*p) *p=' ';
   p++;
 }
 return RemoveExternalSpaces(Str);
}
/* IS $ */

// ������� ᨬ��� Target �� ��ப� Str (�����!)
char *RemoveChar(char *Str,char Target)
{
  char *Ptr = Str, *StrBegin = Str, Chr;
  while((Chr=*Str++) != 0)
  {
    if(Chr == Target)
      continue;
    *Ptr++ = Chr;
  }
  *Ptr = '\0';
  return StrBegin;
}

int HiStrlen(char *Str)
{
  int Length=0;
  while (*Str)
  {
    if (*Str!='&')
      Length++;
    Str++;
  }
  return(Length);
}


int WINAPI AddEndSlash(char *Path)
{
  int Result=0;
  if(Path)
  {
    /* $ 06.12.2000 IS
      ! ������ �㭪�� ࠡ�⠥� � ������ ������ ᫥襩, ⠪�� �ந�室��
        ��������� 㦥 �������饣� ����筮�� ᫥� �� ⠪��, �����
        ����砥��� ��.
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
    Result = 1;
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
     else *end=c;
    }
    /* IS $ */
  }
  return Result;
}


char *NullToEmpty(char *Str)
{
  return (Str==NULL) ? "":Str;
}


char* CenterStr(char *Src,char *Dest,int Length)
{
  char TempSrc[512];
  int SrcLength=strlen(Src);
  strcpy(TempSrc,Src);
  if (SrcLength>=Length)
    strcpy(Dest,TempSrc);
  else
  {
    int Space=(Length-SrcLength)/2;
    sprintf(Dest,"%*s%s%*s",Space,"",TempSrc,Length-Space-SrcLength,"");
  }
  return Dest;
}


char *GetCommaWord(char *Src,char *Word)
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
    if (*Src==',' && !SkipBrackets)
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


/* $ 28.06.2000 IS
  ������ �㭪�� Unquote 㡨ࠥ� ��� ��砫�� � ������⥫�� ����窨
*/
/* $ 25.07.2000 SVS
   �맮� WINAPI
*/
void WINAPI Unquote(char *Str)
{
 if(Str)
  {
   if(int Length=lstrlen(Str))
   {
    /*㡨ࠥ� ������⥫�� ����窨*/
    Length--;
    while(Str[Length]=='\"')
    {
     Str[Length]='\0';
     if(!Length)break;
     Length--;
    }
    /*㡨ࠥ� ��砫�� ����窨*/
    char *start=Str;
    while(*start=='\"') start++;
    if(start!=Str) memcpy(Str,start,Length+Str-start+2);
   }
  }
}
/* IS $ */
