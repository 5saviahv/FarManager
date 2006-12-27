/*
strmix.cpp

���� ������ ��������������� ������� �� ������ �� ��������

*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "global.hpp"
#include "plugin.hpp"
#include "lang.hpp"

char *InsertCommas(const unsigned long &Number,char *Dest)
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

char* InsertCommas(const unsigned __int64  &LI,char *Dest)
{
  _i64toa(LI,Dest,10);
  for (int I=strlen(Dest)-4;I>=0;I-=3)
    if (Dest[I])
    {
      memmove(Dest+I+2,Dest+I+1,strlen(Dest+I));
      Dest[I+1]=',';
    }
  return Dest;
}

char* WINAPI PointToName(char *Path)
{
  if(!Path)
    return NULL;

  char *NamePtr=Path;
  while (*Path)
  {
    if (*Path=='\\' || *Path=='/' || *Path==':' && Path==NamePtr+1)
      NamePtr=Path+1;
    Path++;
  }
  return(NamePtr);
}

const char *WINAPI PointToName(const char *Path)
{
  if(!Path)
    return NULL;

  const char *NamePtr=Path;
  while (*Path)
  {
    if (*Path=='\\' || *Path=='/' || *Path==':' && Path==NamePtr+1)
      NamePtr=Path+1;
    Path++;
  }
  return(NamePtr);
}

/* $ 20.03.2002 IS
   ������ PointToName, ������ ��� ����� ����
   "name\" (������������ �� ����) ���������� ��������� �� name, � �� �� ������
   ������
*/
char* WINAPI PointToFolderNameIfFolder(const char *Path)
{
  if(!Path)
    return NULL;

  const char *NamePtr=Path, *prevNamePtr=Path;
  while (*Path)
  {
    if (*Path=='\\' || *Path=='/' ||
        *Path==':' && Path==NamePtr+1)
    {
      prevNamePtr=NamePtr;
      NamePtr=Path+1;
    }
    ++Path;
  }
  return const_cast<char*>((*NamePtr)?NamePtr:prevNamePtr);
}
/* IS $ */


/* 06.12.2005 AY
   ������ PointToName ������ ������� \\computer\share
   ��� ��� ����� �� ���� a:\ � �������������� �� ���������� �� ����
   ��� �� ���.
*/
char* WINAPI PointToNameUNC(char *Path)
{
  if(!Path)
    return NULL;

  if ((Path[0]=='/' || Path[0]=='\\') && (Path[1]=='/' || Path[1]=='\\'))
  {
    Path+=2;
    for (int i=0; i<2; i++)
    {
      while (*Path && *Path!='/' && *Path!='\\')
        Path++;
      if (*Path)
        Path++;
    }
  }

  char *NamePtr=Path;
  while (*Path)
  {
    if (*Path=='\\' || *Path=='/' || *Path==':' && Path==NamePtr+1)
      NamePtr=Path+1;
    Path++;
  }
  return(NamePtr);
}


/* $ 10.05.2003 IS
   + �������� CmpName �� ���� ������ �������� skippath ������
   - ������: *Name*.* �� �������� Name
*/

// IS: ��� �������� ���� ������� ��������� � ������, �� ������������
// IS: "�������" ����� �� ��� �������, � CmpName (�� ���� �����������
// IS: ����� CmpName_Body)
int CmpName_Body(const char *pattern,const char *string)
{
  unsigned char stringc,patternc,rangec;
  int match;

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
          if (pattern[1]=='*' && pattern[2]==0)
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
          if (CmpName(pattern,string++,FALSE))
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

// IS: ������� ��� �������� ����, ������������ ��
int CmpName(const char *pattern,const char *string,int skippath)
{
  if (skippath)
    string=PointToName(const_cast<char*>(string));
  return CmpName_Body(pattern,string);
}

/* IS 10.05.2003 $ */

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

  xstrncpy(Src, src, sizeof(Src)-1);

  if (SelectedFolderNameLength!=0)
  {
    strcpy(PartAfterFolderName,Src+SelectedFolderNameLength);
    Src[SelectedFolderNameLength]=0;
  }

  SrcNamePtr=PointToName(Src);

  int BeforeNameLength=DestNamePtr==Dest ? SrcNamePtr-Src:0;
  xstrncpy(PartBeforeName,Src,BeforeNameLength);
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
    {
      if (MaxLength>3)
      {
        char *MovePos = Str+Length-MaxLength+3;
        memmove(Str+3,MovePos,strlen(MovePos)+1);
        memcpy(Str,"...",3);
      }
      Str[MaxLength]=0;
    }
  }
  return(Str);
}

#if 0
char* WINAPI TruncPathStr(char *Str,int MaxLength)
{
  if(Str)
  {
    char *Root=NULL;
    if (Str[0] && Str[1]==':' && Str[2]=='\\')
      Root=Str+3;
    else if (Str[0]=='\\' && Str[1]=='\\')
    {
      if((Root=strchr(Str+2,'\\'))!=NULL)
        if((Root=strchr(Root+1,'\\'))!=NULL)
           Root++;
    }
    if (Root==NULL || Root-Str+5>MaxLength)
      return(TruncStr(Str,MaxLength));

    int Length=strlen(Str);
    if (Length > MaxLength)
    {
      char *MovePos=Root+Length-MaxLength+3;
      memmove(Root+3,MovePos,strlen(MovePos)+1);
      memcpy(Root,"...",3);
    }
  }
  return(Str);
}
#else
char* WINAPI TruncPathStr(char *Str, int MaxLength)
{
  if (Str)
  {
    int nLength = strlen (Str);

    if (nLength > MaxLength)
    {
      char *lpStart = NULL;

      if ( *Str && (Str[1] == ':') && (Str[2] == '\\') )
         lpStart = Str+3;
      else
      {
        if ( (Str[0] == '\\') && (Str[1] == '\\') )
        {
          if ( (lpStart = strchr (Str+2, '\\')) != NULL )
            if ( (lpStart = strchr (lpStart+1, '\\')) != NULL )
              lpStart++;
        }
      }

      if ( !lpStart || (lpStart-Str > MaxLength-5) )
        return TruncStr (Str, MaxLength);

      char *lpInPos = lpStart+3+(nLength-MaxLength);
      memmove (lpStart+3, lpInPos, strlen (lpInPos)+1);
      memcpy (lpStart, "...", 3);
    }
  }

  return Str;
}
#endif

/* $ 07.07.2000 SVS
    + �������������� ������� ��������� �����: RemoveExternalSpaces
    ! ������� Remove*Spaces ���������� char*
*/
// ������� ������� �������
char* WINAPI RemoveLeadingSpaces(char *Str)
{
  char *ChPtr;
  if((ChPtr=Str) == 0)
    return NULL;

  for (; IsSpace(*ChPtr); ChPtr++)
         ;
  if (ChPtr!=Str)
    memmove(Str,ChPtr,strlen(ChPtr)+1);
  return Str;
}


// ������� �������� �������
char* WINAPI RemoveTrailingSpaces(char *Str)
{
  if(!Str)
    return NULL;
  if (*Str == '\0')
    return Str;

  char *ChPtr;
  int I;

  for (ChPtr=Str+(I=strlen((char *)Str)-1); I >= 0; I--, ChPtr--)
    if (IsSpace(*ChPtr) || IsEol(*ChPtr))
      *ChPtr=0;
    else
      break;

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
   if(IsEol(*p)) *p=' ';
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
  return AddEndSlash(Path,0);
}

BOOL AddEndSlash(char *Path,char TypeSlash)
{
  BOOL Result=FALSE;
  if(Path)
  {
    /* $ 06.12.2000 IS
      ! ������ ������� �������� � ������ ������ ������, ����� ����������
        ��������� ��� ������������� ��������� ����� �� �����, �������
        ����������� ����.
    */
    char *end;
    int Slash=0, BackSlash=0;
    if(!TypeSlash)
    {
      end=Path;
      while(*end)
      {
       Slash+=(*end=='\\');
       BackSlash+=(*end=='/');
       end++;
      }
    }
    else
    {
      end=Path+strlen(Path);
      if(TypeSlash == '\\')
        Slash=1;
      else
        BackSlash=1;
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

BOOL WINAPI DeleteEndSlash(char *Path,bool allendslash)
{
  BOOL Ret=FALSE;
  if(Path)
  {
    char *PtrEndPath=Path+(strlen(Path)-1);
    while(PtrEndPath >= Path)
    {
      if(*PtrEndPath == '\\')
      {
        Ret=TRUE;
        *PtrEndPath=0;
        if(!allendslash)
          return Ret;
      }
      else
        return Ret;
      PtrEndPath--;
    }
  }
  return Ret;
}

/*
char *NullToEmpty(char *Str)
{
  return (Str==NULL) ? "":Str;
}
*/

const char *NullToEmpty(const char *Str)
{
  return (Str==NULL) ? "":Str;
}

char* CenterStr(char *Src,char *Dest,int Length)
{
  char TempSrc[1024];
  xstrncpy(TempSrc,Src,sizeof(TempSrc)-1);
  int SrcLength=strlen(Src);
  if (SrcLength >= Length)
    /* ����� �� ���� �������� 1 �� �����, �.�. strlen �� ��������� \0
       � �� �������� ���������� ������ */
    xstrncpy(Dest,TempSrc,Length);
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
      while (IsSpace(*Src))
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
  char *Dst=Str;
  while (*Str)
  {
    if (*Str!='\"')
      *Dst++=*Str;
    Str++;
  }
  *Dst=0;
}
/* IS $ */
/* VVM $ */
/* VVM $ */

void UnquoteExternal(char *Str)
{
  if (!Str)
    return;
  if (*Str == '\"' && Str[strlen(Str)-1] == '\"')
  {
    Str[strlen(Str)-1]=0;
    memmove(Str,Str+1,strlen(Str));
  }
}

/* FileSizeToStr()
   �������������� ������� ����� � ������������� ���.
*/
#define MAX_KMGTBSTR_SIZE 16
static char KMGTbStr[5][2][MAX_KMGTBSTR_SIZE]={0};

void __PrepareKMGTbStr(void)
{
  for(int I=0; I < 5; ++I)
  {
    xstrncpy(KMGTbStr[I][0],MSG(MListBytes+I),MAX_KMGTBSTR_SIZE-1);
    strcpy(KMGTbStr[I][1],KMGTbStr[I][0]);
    LocalStrlwr(KMGTbStr[I][0]);
    LocalStrupr(KMGTbStr[I][1]);
  }
}

char* WINAPI FileSizeToStr(char *DestStr,unsigned __int64 Size, int Width, int ViewFlags)
{
  char Str[100];
  DWORD Divider;
  int IndexDiv, IndexB;

  // ���������������� �����������
  if(KMGTbStr[0][0][0] == 0)
  {
    __PrepareKMGTbStr();
  }

  int Commas=(ViewFlags & COLUMN_COMMAS);
  int FloatSize=(ViewFlags & COLUMN_FLOATSIZE);
  int Economic=(ViewFlags & COLUMN_ECONOMIC);
  int UseMinSizeIndex=(ViewFlags & COLUMN_MINSIZEINDEX);
  int MinSizeIndex=(ViewFlags & COLUMN_MINSIZEINDEX_MASK)+1;
  int ShowBytesIndex=(ViewFlags & COLUMN_SHOWBYTESINDEX);

  if (ViewFlags & COLUMN_THOUSAND)
  {
    Divider=1000;
    IndexDiv=0;
  }
  else
  {
    Divider=1024;
    IndexDiv=1;
  }

  unsigned __int64 Sz=Size, Divider2=Divider/2,Divider64=Divider, OldSize;

  if (FloatSize)
  {
    unsigned __int64 Divider64F=1, Divider64F_mul=1000, Divider64F2=1, Divider64F2_mul=Divider;
    //������������ ��� �� 1000 �� ���� ������� ���������� �� Divider
    //�������� 999 bytes ��������� ��� 999 � ��� 1000 bytes ��� ��������� ��� 0.97 K
    for (IndexB=0; IndexB<4; IndexB++)
    {
      if (Sz < Divider64F*Divider64F_mul)
        break;
      Divider64F = Divider64F*Divider64F_mul;
      Divider64F2  = Divider64F2*Divider64F2_mul;
    }
    if (IndexB==0)
    {
      sprintf(Str,"%d",(int)Sz);
    }
    else
    {
      Sz = (OldSize=Sz) / Divider64F2;
      OldSize = (OldSize % Divider64F2) / (Divider64F2 / Divider64F2_mul);
      DWORD Decimal = (DWORD)((double)(OldSize&_i64(0xFFFFFFFF))/(double)Divider*100.0);
      sprintf(Str,"%d.%02d",(int)Sz,Decimal);
    }
    if (IndexB>0 || ShowBytesIndex)
    {
      Width-=(Economic?1:2);
      if (Width<0)
        Width=0;
      if (Economic)
        sprintf(DestStr,"%*.*s%1.1s",Width,Width,Str,KMGTbStr[IndexB][IndexDiv]);
      else
        sprintf(DestStr,"%*.*s %1.1s",Width,Width,Str,KMGTbStr[IndexB][IndexDiv]);
    }
    else
      sprintf(DestStr,"%*.*s",Width,Width,Str);

    return DestStr;
  }

  if (Commas)
    InsertCommas(Sz,Str);
  else
    sprintf(Str,"%I64d",Sz);

  if ((!UseMinSizeIndex && strlen(Str)<=static_cast<size_t>(Width)) || Width<5)
  {
    if (ShowBytesIndex)
    {
      Width-=(Economic?1:2);
      if (Width<0)
        Width=0;
      if (Economic)
        sprintf(DestStr,"%*.*s%1.1s",Width,Width,Str,KMGTbStr[0][IndexDiv]);
      else
        sprintf(DestStr,"%*.*s %1.1s",Width,Width,Str,KMGTbStr[0][IndexDiv]);
    }
    else
      sprintf(DestStr,"%*.*s",Width,Width,Str);
  }
  else
  {
    Width-=(Economic?1:2);
    IndexB=0;
    do{
      //Sz=(Sz+Divider2)/Divider64;
      Sz = (OldSize=Sz) / Divider64;
      if ((OldSize % Divider64) > Divider2)
        ++Sz;

      IndexB++;

      if (Commas)
        InsertCommas(Sz,Str);
      else
        sprintf(Str,"%I64d",Sz);
    } while((UseMinSizeIndex && IndexB<MinSizeIndex) || (strlen(Str) > static_cast<size_t>(Width)));

    if (Economic)
      sprintf(DestStr,"%*.*s%1.1s",Width,Width,Str,KMGTbStr[IndexB][IndexDiv]);
    else
      sprintf(DestStr,"%*.*s %1.1s",Width,Width,Str,KMGTbStr[IndexB][IndexDiv]);
  }

  return DestStr;
}

// �������� � ������� Pos � Str ������ InsStr (�������� InsSize ����)
// ���� InsSize = 0, ��... ��������� ��� ������ InsStr
// ���������� ��������� �� Str
char *InsertString(char *Str,int Pos,const char *InsStr,int InsSize)
{
  int InsLen=strlen(InsStr);
  if(InsSize && InsSize < InsLen)
    InsLen=InsSize;
  memmove(Str+Pos+InsLen, Str+Pos, strlen(Str+Pos)+1);
  memcpy(Str+Pos, InsStr, InsLen);
  return Str;
}

// �������� � ������ Str Count ��������� ��������� FindStr �� ��������� ReplStr
// ���� Count < 0 - �������� "�� ������ ������"
// Return - ���������� �����
int ReplaceStrings(char *Str,const char *FindStr,const char *ReplStr,int Count,BOOL IgnoreCase)
{
  int I=0, J=0, Res;
  int LenReplStr=strlen(ReplStr);
  int LenFindStr=strlen(FindStr);
  int L=strlen(Str);

  while(I <= L-LenFindStr)
  {
    Res=IgnoreCase?memicmp(Str+I, FindStr, LenFindStr):memcmp(Str+I, FindStr, LenFindStr);
    if(Res == 0)
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
    L=strlen(Str);
  }
  return J;
}

/*
From PHP 4.x.x
����������� �������� ����� �� �������� ������, ���������
�������������� ������. ���������� ������ SrcText ��������
� �������, �������� ���������� Width. ������ ������� ���
������ ������ Break.

��������� �� ������ � �������������� �����.

���� �������� Flahs & FFTM_BREAKLONGWORD, �� ������ ������
������������� �� �������� ������. ��� ���� � ��� ���� �����,
������� ������ �������� ������, �� ��� ����� ��������� �� �����.

Example 1.
FarFormatText("������ ������, ������� ����� ������� �� ��������� ����� �� ������ � 20 ��������.", 20 ,Dest, "\n", 0);
���� ������ ������:
---
������ ������,
������� �����
������� ��
��������� ����� ��
������ � 20
��������.
---

Example 2.
FarFormatText( "��� ������ �������� ��������������������������� ������ �����", 9, Dest, NULL, FFTM_BREAKLONGWORD);
���� ������ ������:

---
���
������
��������
���������
���������
���������
������
�����
---

*/
char *WINAPI FarFormatText(const char *SrcText,     // ��������
                           int Width,               // �������� ������
                           char *DestText,          // ��������
                           int MaxLen,              // ������������ ������� ���������
                           const char* Break,       // ����, ���� = NULL, �� ����������� '\n'
                           DWORD Flags)             // ���� �� FFTM_*
{
  const char *breakchar;
  breakchar = Break?Break:"\n";

  if(!SrcText || !*SrcText)
    return NULL;

  if(!strpbrk(SrcText,breakchar) && strlen(SrcText) <= static_cast<size_t>(Width))
  {
    if(MaxLen > 0 && DestText)
      xstrncpy(DestText,SrcText,MaxLen-1);
    return DestText;
  }

  long i=0, l=0, pgr=0, last=0;
  char *newtext;

  const char *text= SrcText;
  long linelength = Width;
  int breakcharlen = strlen(breakchar);
  int docut = Flags&FFTM_BREAKLONGWORD?1:0;

  /* Special case for a single-character break as it needs no
     additional storage space */

  if (breakcharlen == 1 && docut == 0)
  {
    newtext = xf_strdup (text);
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
    newtext = (char*)xf_malloc(strlen(SrcText) * (breakcharlen+1)+1);
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

  if(DestText && MaxLen > 0)
    xstrncpy(DestText,newtext,MaxLen-1);
  xf_free(newtext);
  return DestText;
}

/* $ 12.01.2004 IS
   + ������� ��� ������ ������� � ������������� ����� � ������ �������
     ���������
*/
// ��������� - �������� �� ������ ������������ ����� (������ TRUE, ���� ��)
// ���������:
//   TableSet - ��������� �� ������� ������������� (���� �����������,
//              �� ��������� - OEM)
//   WordDiv  - ����� ������������ ����� � ��������� OEM
//   Chr      - ����������� ������
BOOL IsWordDiv(const struct CharTableSet *TableSet, const char *WordDiv, unsigned char Chr)
{
  return NULL!=strchr(WordDiv, TableSet?TableSet->DecodeTable[Chr]:Chr);
}
/* IS $ */

#if defined(MOUSEKEY)
/*
  Ptr=CalcWordFromString(Str,I,&Start,&End);
  xstrncpy(Dest,Ptr,End-Start+1);
  Dest[End-Start+1]=0;

// ���������:
//   TableSet - ��������� �� ������� ������������� (���� �����������,
//              �� ��������� - OEM)
//   WordDiv  - ����� ������������ ����� � ��������� OEM
  ���������� ��������� �� ������ �����
*/
const char * const CalcWordFromString(const char *Str,int CurPos,int *Start,int *End,const struct CharTableSet *TableSet, const char *WordDiv0)
{
  int I, J, StartWPos, EndWPos;
  DWORD DistLeft, DistRight;
  int StrSize=strlen(Str);
  char WordDiv[512];
  xstrncpy(WordDiv,WordDiv0,sizeof(WordDiv)-5);
  strcat(WordDiv," \t\n\r");

  if(IsWordDiv(TableSet,WordDiv,Str[CurPos]))
  {
    // ��������� ��������� - ���� ������, ��� ����� ����� - ����� ��� ������
    I=J=CurPos;

    // ������ �����
    DistLeft=-1;
    while(I >= 0 && IsWordDiv(TableSet,WordDiv,Str[I]))
    {
      DistLeft++;
      I--;
    }
    if(I < 0)
      DistLeft=-1;

    // ������ ������
    DistRight=-1;
    while(J < StrSize && IsWordDiv(TableSet,WordDiv,Str[J]))
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
    if(IsWordDiv(TableSet,WordDiv,Str[StartWPos]))
    {
      StartWPos++;
      break;
    }
    else
      StartWPos--;
  while(EndWPos < StrSize)
    if(IsWordDiv(TableSet,WordDiv,Str[EndWPos]))
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

BOOL TestParentFolderName(const char *Name)
{
  return Name[0] == '.' && Name[1] == '.' && !Name[2];
}


int __digit_cnt_0(const char* s, const char** beg)
{
  int n = 0;
  while(*s == '0') s++;
  *beg = s;
  while(isdigit(*s)) { s++; n++; }
  return n;
}

int __cdecl NumStrcmp(const char *s1, const char *s2)
{
  const char *ts1 = s1, *ts2 = s2;
  while(*s1 && *s2)
  {
    if(isdigit(*s1) && isdigit(*s2))
    {
       // ����� ����� ����� ��� ������� �����
       int dig_len1 = __digit_cnt_0(s1, &s1);
       int dig_len2 = __digit_cnt_0(s2, &s2);
       // ���� ���� ������� �������, ������ ��� � ������! :)
       if(dig_len1 != dig_len2)
         return dig_len1 - dig_len2;
       // ����� ���������, ������������...
       while(isdigit(*s1) && isdigit(*s2))
       {
          if(*s1 != *s2)
            return *s1 - *s2;
          s1++; s2++;
       }
       if(*s1 == 0)
         break;
    }
    if(*s1 != *s2)
      return *s1 - *s2; // ����� ������ ������
    s1++; s2++;
  }
  if(*s1 == *s2)
    return strlen(ts2)-strlen(ts1);
  return *s1 - *s2;
}
