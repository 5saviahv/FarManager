/*
help.cpp

������

*/

#include "headers.hpp"
#pragma hdrstop

#include "help.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "colors.hpp"
#include "plugin.hpp"
#include "scantree.hpp"
#include "savescr.hpp"
#include "manager.hpp"
#include "ctrlobj.hpp"
#include "BlockExtKey.hpp"
#include "macroopcode.hpp"


// ���� ��������
class CallBackStack
{
  private:
    struct ListNode
    {
      ListNode *Next;

      DWORD Flags;             // �����
      int   TopStr;            // ����� ������� ������� ������ ����
      int   CurX,CurY;         // ���������� (???)

      char  *HelpTopic;        // ������� �����
      char  *HelpPath;         // ���� � ������
      char  *SelTopic;         // ������� ���������
      char  *HelpMask;         // �����

      ListNode(const struct StackHelpData *Data, ListNode* n=NULL)
      {
        HelpTopic=xf_strdup(Data->HelpTopic);
        HelpPath=xf_strdup(Data->HelpPath);
        SelTopic=xf_strdup(Data->SelTopic);
        HelpMask=xf_strdup(Data->HelpMask);
        Flags=Data->Flags;
        TopStr=Data->TopStr;
        CurX=Data->CurX;
        CurY=Data->CurY;

        Next=n;
      }
      ~ListNode()
      {
        if(HelpTopic) xf_free(HelpTopic);
        if(HelpPath)  xf_free(HelpPath);
        if(SelTopic)  xf_free(SelTopic);
        if(HelpMask)  xf_free(HelpMask);
      }
    };

    ListNode *topOfStack;

  public:
    CallBackStack() {topOfStack=NULL;};
   ~CallBackStack() {ClearStack();};

  public:
    void ClearStack();
    BOOL isEmpty() const {return topOfStack==NULL;};

    void Push(const struct StackHelpData *Data);
    int Pop(struct StackHelpData *Data=NULL);

    void PrintStack(const char *Title);
};


#define MAX_HELP_STRING_LENGTH 300

static const char *PluginContents="__PluginContents__";
#if defined(WORK_HELP_DOCUMS)
static const char *DocumentContents="__DocumentContents__";
#endif
static const char *HelpOnHelpTopic=":Help";
static const char *HelpContents="Contents";

static int RunURL(char *Protocol, char *URLPath);

Help::Help(const char *Topic, const char *Mask,DWORD Flags)
{
  /* $ OT �� ��������� ��� ����� ��������� ����������*/
  SetDynamicallyBorn(FALSE);
  CanLoseFocus=FALSE;
  PrevMacroMode=CtrlObject->Macro.GetMode();
  CtrlObject->Macro.SetMode(MACRO_HELP);
  FullHelpPathName[0]=0;

  ErrorHelp=TRUE;
  IsNewTopic=TRUE;

  MouseDown = FALSE;

  Stack=new CallBackStack;

  memset(&StackData,0,sizeof(StackData));
  StackData.Flags=Flags;

  /* $ 01.09.2000 SVS
     ��������� �� ��������� ������� ���� ���������...
  */
  CurColor=COL_HELPTEXT;
  /* SVS $ */

  /* $ 27.11.2001 DJ
     �� ������� ����������������
  */
  CtrlTabSize = 8;
  /* DJ $ */

  xstrncpy(StackData.HelpMask,NullToEmpty(Mask),sizeof(StackData.HelpMask)-1); // �������� ����� �����

  KeyBarVisible = TRUE;  // �������� ���������� ������
  TopScreen=new SaveScreen;
  HelpData=NULL;
  strcpy(StackData.HelpTopic,Topic);
  *StackData.HelpPath=0;
  if (Opt.FullScreenHelp)
    SetPosition(0,0,ScrX,ScrY);
  else
    SetPosition(4,2,ScrX-4,ScrY-2);

  if(!ReadHelp(StackData.HelpMask) && (Flags&FHELP_USECONTENTS))
  {
    strcpy(StackData.HelpTopic,Topic);
    if(*StackData.HelpTopic == HelpBeginLink)
    {
      char *Ptr=strrchr(StackData.HelpTopic,HelpEndLink);
      if(Ptr)
        strcpy(++Ptr,HelpContents);
    }
    *StackData.HelpPath=0;
    ReadHelp(StackData.HelpMask);
  }


  if (HelpData!=NULL)
  {
    ScreenObject::Flags.Clear(FHELPOBJ_ERRCANNOTOPENHELP);
    InitKeyBar();
    MacroMode = MACRO_HELP;
    MoveToReference(1,1);
    FrameManager->ExecuteModal (this);//OT
  }
  else
  {
    ErrorHelp=TRUE;
    if(!(Flags&FHELP_NOSHOWERROR))
    {
      if(!ScreenObject::Flags.Check(FHELPOBJ_ERRCANNOTOPENHELP))
      {
        BlockExtKey blockExtKey;
        Message(MSG_WARNING,1,MSG(MHelpTitle),MSG(MHelpTopicNotFound),StackData.HelpTopic,MSG(MOk));
      }
      ScreenObject::Flags.Clear(FHELPOBJ_ERRCANNOTOPENHELP);
    }
  }

#if defined(WORK_HELP_FIND)
  xstrncpy((char *)LastSearchStr,GlobalSearchString,sizeof(LastSearchStr));
  LastSearchPos=0;
  LastSearchCase=GlobalSearchCase;
  LastSearchWholeWords=GlobalSearchWholeWords;
  LastSearchReverse=GlobalSearchReverse;
#endif

}

Help::~Help()
{
  CtrlObject->Macro.SetMode(PrevMacroMode);
  SetRestoreScreenMode(FALSE);

  if(HelpData)     xf_free(HelpData);
  if(Stack)        delete Stack;
  if(TopScreen)    delete TopScreen;

#if defined(WORK_HELP_FIND)
  KeepInitParameters();
#endif
}


#if defined(WORK_HELP_FIND)
void Help::KeepInitParameters()
{
  strcpy(GlobalSearchString,(char *)LastSearchStr);
  LastSearchPos=0;
  GlobalSearchCase=LastSearchCase;
  GlobalSearchWholeWords=LastSearchWholeWords;
  GlobalSearchReverse=LastSearchReverse;
}
#endif


void Help::Hide()
{
  ScreenObject::Hide();
}


int Help::ReadHelp(const char *Mask)
{
  char ReadStr[2*MAX_HELP_STRING_LENGTH];
  char SplitLine[2*MAX_HELP_STRING_LENGTH+8],*Ptr;
  int Formatting=TRUE,RepeatLastLine,PosTab,BreakProcess;
  const int MaxLength=X2-X1-1;
  char TabSpace[32];

  char Path[NM],*TopicPtr;
  if (*StackData.HelpTopic==HelpBeginLink)
  {
    strcpy(Path,StackData.HelpTopic+1);
    if ((TopicPtr=strchr(Path,HelpEndLink))==NULL)
      return FALSE;
    strcpy(StackData.HelpTopic,TopicPtr+1);
    *TopicPtr=0;
    DeleteEndSlash(Path,true);
    AddEndSlash(Path);
    strcpy(StackData.HelpPath,Path);
  }
  else
    strcpy(Path,*StackData.HelpPath ? StackData.HelpPath:FarPath);

  if (!strcmp(StackData.HelpTopic,PluginContents))
  {
    FullHelpPathName[0]=0;
    ReadDocumentsHelp(HIDX_PLUGINS);
    return TRUE;
  }

#if defined(WORK_HELP_DOCUMS)
  if (!strcmp(StackData.HelpTopic,DocumentContents))
  {
    FullHelpPathName[0]=0;
    ReadDocumentsHelp(HIDX_DOCUMS);
    return TRUE;
  }
#endif

  FILE *HelpFile=Language::OpenLangFile(Path,(!*Mask?HelpFileMask:Mask),Opt.HelpLanguage,FullHelpPathName);

  if (HelpFile==NULL)
  {
    ErrorHelp=TRUE;
    if(!ScreenObject::Flags.Check(FHELPOBJ_ERRCANNOTOPENHELP))
    {
      ScreenObject::Flags.Set(FHELPOBJ_ERRCANNOTOPENHELP);
      if(!(StackData.Flags&FHELP_NOSHOWERROR))
      {
        BlockExtKey blockExtKey;
        Message(MSG_WARNING,1,MSG(MHelpTitle),MSG(MCannotOpenHelp),Mask,MSG(MOk));
      }
    }
    return FALSE;
  }

  *ReadStr=0;
  if (Language::GetOptionsParam(HelpFile,"TabSize",ReadStr))
  {
    CtrlTabSize=atoi(ReadStr);
  }
  if(CtrlTabSize < 0 || CtrlTabSize > 16)
    CtrlTabSize=Opt.HelpTabSize;

  *ReadStr=0;
  if (Language::GetOptionsParam(HelpFile,"CtrlColorChar",ReadStr))
  {
    xstrncpy(CtrlColorChar,ReadStr,sizeof(CtrlColorChar)-1);
  }
  else
    CtrlColorChar[0]=0;


  *ReadStr=0;
  if (Language::GetOptionsParam(HelpFile,"CtrlStartPosChar",ReadStr))
  {
    xstrncpy(CtrlStartPosChar,ReadStr,sizeof(CtrlStartPosChar)-1);
  }
  else
    CtrlStartPosChar[0] = '\0';


  /* $ 29.11.2001 DJ
     ��������, ���� ��� �������� � PluginContents
  */
  if (!Language::GetLangParam (HelpFile,"PluginContents",CurPluginContents, NULL))
    *CurPluginContents = '\0';
  /* DJ $ */

  *SplitLine=0;
  if (HelpData)
    xf_free(HelpData);
  HelpData=NULL;
  StrCount=0;
  FixCount=0;
  TopicFound=0;
  RepeatLastLine=FALSE;
  BreakProcess=FALSE;
  int NearTopicFound=0;
  char PrevSymbol=0;

  memset(TabSpace,' ',sizeof(TabSpace)-1);
  TabSpace[sizeof(TabSpace)-1]=0;

  StartPos = -1;
  LastStartPos = -1;

  int RealMaxLength;

  while (TRUE)
  {
    if ( StartPos != -1 )
      RealMaxLength = MaxLength-StartPos;
    else
      RealMaxLength = MaxLength;

    if (!RepeatLastLine && !BreakProcess && fgets(ReadStr,sizeof(ReadStr)/2,HelpFile)==NULL)
    {
      if (StringLen(SplitLine)<MaxLength)
      {
        if (*SplitLine)
          AddLine(SplitLine);
      }
      else
      {
        *ReadStr=0;
        RepeatLastLine=TRUE;
        continue;
      }
      break;
    }
    RepeatLastLine=FALSE;

    while((Ptr=strchr(ReadStr,'\t')) != NULL)
    {
      *Ptr=' ';
      PosTab=Ptr-ReadStr+1;
      if(CtrlTabSize > 1) // ������� ��������� �� ���� ���������
        InsertString(ReadStr,PosTab,TabSpace, CtrlTabSize - (PosTab % CtrlTabSize));
      if(strlen(ReadStr) > sizeof(ReadStr)/2)
        break;
    }

    RemoveTrailingSpaces(ReadStr);

    if ( *CtrlStartPosChar && strstr (ReadStr, CtrlStartPosChar) )
    {
        char Line[MAX_HELP_STRING_LENGTH];
        int Length = strstr (ReadStr, CtrlStartPosChar)-ReadStr;

        xstrncpy (Line, ReadStr, Length);
        LastStartPos = StringLen (Line);;

        strcpy (ReadStr+Length, ReadStr+Length+strlen(CtrlStartPosChar));
    }

    if (TopicFound)
    {
      HighlightsCorrection(ReadStr);
    }

    if (*ReadStr=='@' && !BreakProcess)
    {
      if (TopicFound)
      {
        if (strcmp(ReadStr,"@+")==0)
        {
          Formatting=TRUE;
          PrevSymbol=0;
          continue;
        }
        if (strcmp(ReadStr,"@-")==0)
        {
          Formatting=FALSE;
          PrevSymbol=0;
          continue;
        }
        if (*SplitLine)
        {
          BreakProcess=TRUE;
          *ReadStr=0;
          PrevSymbol=0;
          goto m1;
        }
        break;
      }
      else
        if (LocalStricmp(ReadStr+1,StackData.HelpTopic)==0)
        {
          TopicFound=1;
          NearTopicFound=1;
        }
    }
    else
    {
m1:
      if(!*ReadStr && BreakProcess && !*SplitLine)
        break;
      if (TopicFound)
      {
        /* $<text> � ������ ������, ����������� ����
           ���������� �� �������������� ������� ������
           ���� ���� ��������� ������ ����� ����� ������ ����������� ����...
        */
        if ( NearTopicFound )
        {
          StartPos = -1;
          LastStartPos = -1;
        }


        if (*ReadStr=='$' && NearTopicFound && (PrevSymbol == '$' || PrevSymbol == '@'))
        {
          AddLine(ReadStr+1);
          FixCount++;
        }
        else
        {
          NearTopicFound=0;
          if (*ReadStr==0 || !Formatting)
          {
            if (*SplitLine)
            {
              if (StringLen(SplitLine)<RealMaxLength)
              {
                AddLine(SplitLine);

                *SplitLine=0;
                if (StringLen(ReadStr)<RealMaxLength)
                {
                  AddLine(ReadStr);

                  LastStartPos = -1;
                  StartPos = -1;

                  continue;
                }
              }
              else
                RepeatLastLine=TRUE;
            }
            else if(*ReadStr)
            {
              if (StringLen(ReadStr)<RealMaxLength)
              {
                AddLine(ReadStr);
                continue;
              }
            }
            else if(!*ReadStr && !*SplitLine)
            {
              AddLine("");
              continue;
            }
          }

          if (*ReadStr && IsSpace(*ReadStr) && Formatting)
          {
            if (StringLen(SplitLine)<RealMaxLength)
            {
              if (*SplitLine)
              {
                AddLine(SplitLine);
                StartPos = -1;
              }

              strcpy(SplitLine,ReadStr);

              *ReadStr=0;
              continue;
            }
            else
              RepeatLastLine=TRUE;
          }

          if (!RepeatLastLine)
          {
            if (*SplitLine)
              strcat(SplitLine," ");
            strcat(SplitLine,ReadStr);
          }

          if (StringLen(SplitLine)<RealMaxLength)
          {
            if(!*ReadStr && BreakProcess)
              goto m1;
            continue;
          }

          int Splitted=0;

          for (int I=strlen(SplitLine)-1;I>0;I--)
          {
            if (I>0 && SplitLine[I]=='~' && SplitLine[I-1]=='~')
            {
              I--;
              continue;
            }
            if (I>0 && SplitLine[I]=='~' && SplitLine[I-1]!='~')
            {
              do {
                I--;
              } while (I>0 && SplitLine[I]!='~');
              continue;
            }
            if (SplitLine[I]==' ')
            {
              SplitLine[I]=0;
              if (StringLen(SplitLine)<RealMaxLength)
              {
                AddLine(SplitLine);
                memmove(SplitLine+1,SplitLine+I+1,strlen(SplitLine+I+1)+1);
                *SplitLine=' ';
                HighlightsCorrection(SplitLine);
                Splitted=TRUE;
                break;
              }
              else
                SplitLine[I]=' ';
            }
          }
          if (!Splitted)
          {
            AddLine(SplitLine);
            *SplitLine=0;
          }
          else
            StartPos = LastStartPos;
        }
      }
      if(BreakProcess)
      {
        if(*SplitLine)
          goto m1;
        break;
      }
    }
    PrevSymbol=*ReadStr;
  }
  AddLine("");

  fclose(HelpFile);
  FixSize=FixCount+(FixCount!=0);
  ErrorHelp=FALSE;
  if(IsNewTopic)
  {
    StackData.CurX=StackData.CurY=0;
    StackData.TopStr=0;
  }
  return TopicFound != 0;
}


void Help::AddLine(const char *Line)
{
  char *NewHelpData=(char *)xf_realloc(HelpData,(StrCount+1)*MAX_HELP_STRING_LENGTH);
  if (NewHelpData==NULL)
    return;
  HelpData=NewHelpData;

  char *HelpStr = HelpData+StrCount*MAX_HELP_STRING_LENGTH;

  memset (HelpStr, 0, MAX_HELP_STRING_LENGTH);

  if ( StartPos != -1 )
  {
    for (DWORD i = 0; i < StartPos; i++)
      HelpStr[i] = ' ';

    xstrncpy(HelpStr+StartPos, (*Line == ' ')?Line+1:Line, MAX_HELP_STRING_LENGTH-1);
  }
  else
    xstrncpy(HelpStr, Line, MAX_HELP_STRING_LENGTH-1);

  StrCount++;
}

void Help::AddTitle(const char *Title)
{
  char IndexHelpTitle[100];
  sprintf(IndexHelpTitle,"^ #%s#",Title);
  AddLine(IndexHelpTitle);
}

void Help::HighlightsCorrection(char *Str)
{
  int I,Count;
  for (I=0,Count=0;Str[I]!=0;I++)
    if (Str[I]=='#')
      Count++;
  if ((Count & 1) && *Str!='$')
  {
    memmove(Str+1,Str,strlen(Str)+1);
    *Str='#';
  }
}


void Help::DisplayObject()
{
  if(!TopScreen)
    TopScreen=new SaveScreen;
  if (!TopicFound)
  {
    if(!ErrorHelp) // ���� ��� ������, �� ��� �������������� ������
    {              // � �������� ��������� �������� � ����������� ����.
      ErrorHelp=TRUE;
      if(!(StackData.Flags&FHELP_NOSHOWERROR))
      {
        BlockExtKey blockExtKey;
        Message(MSG_WARNING,1,MSG(MHelpTitle),MSG(MHelpTopicNotFound),StackData.HelpTopic,MSG(MOk));
      }
      ProcessKey(KEY_ALTF1);
    }
    return;
  }
  SetCursorType(0,10);
  if (*StackData.SelTopic==0)
    MoveToReference(1,1);
  FastShow();
  if (!Opt.FullScreenHelp)
  {
    HelpKeyBar.SetPosition(0,ScrY,ScrX,ScrY);
    if(Opt.ShowKeyBar)
       HelpKeyBar.Show();
  }
  else
    HelpKeyBar.Hide();
}


void Help::FastShow()
{
  int I;

  /* $ 29.11.2001 DJ
     ��������� ����� -> � ��������� �������
  */
  if (!Locked())
    DrawWindowFrame();
  /* DJ $ */

  CorrectPosition();
  *StackData.SelTopic=0;
  /* $ 01.09.2000 SVS
     ��������� �� ��������� ������� ���� ���������...
     ����� ����� ���� ���������� � ����������� ����������
  */
  CurColor=COL_HELPTEXT;
  /* SVS $ */
  for (I=0;I<Y2-Y1-1;I++)
  {
    int StrPos;
    if (I<FixCount)
      StrPos=I;
    else
      if (I==FixCount && FixCount>0)
      {
        if (!Locked())
        {
          GotoXY(X1,Y1+I+1);
          SetColor(COL_HELPBOX);
          ShowSeparator(X2-X1+1,1);
        }
        continue;
      }
      else
      {
        StrPos=I+StackData.TopStr;
        if (FixCount>0)
          StrPos--;
      }
    if (StrPos<StrCount)
    {
      char *OutStr=HelpData+StrPos*MAX_HELP_STRING_LENGTH;
      if (*OutStr=='^')
      {
        GotoXY(X1+(X2-X1+1-StringLen(OutStr))/2,Y1+I+1);
        OutStr++;
      }
      else
        GotoXY(X1+1,Y1+I+1);
      OutString(OutStr);
    }
  }

  const int ScrollLength=Y2-Y1-FixSize-1;
  if (!Locked() && StrCount-FixCount > ScrollLength)
  {
    int Scrolled=StrCount-FixCount-ScrollLength;
    SetColor(COL_HELPSCROLLBAR);
    ScrollBar(X2,Y1+FixSize+1,ScrollLength,StackData.TopStr,Scrolled);
  }
}

/* $ 29.11.2001 DJ
   �������� �� FastShow; �������� ����� ����, ��� � ��� ����
*/

void Help::DrawWindowFrame()
{
  SetScreen(X1,Y1,X2,Y2,' ',COL_HELPTEXT);
  Box(X1,Y1,X2,Y2,COL_HELPBOX,DOUBLE_BOX);
  SetColor(COL_HELPBOXTITLE);

  char HelpTitleBuf [256];
  strcpy (HelpTitleBuf, MSG(MHelpTitle));
  strcat (HelpTitleBuf, " - ");
  if (*CurPluginContents)
    strcat (HelpTitleBuf, CurPluginContents);
  else
    strcat (HelpTitleBuf, "FAR");
  /* $ 03.12.2001 DJ
     ������� ������� ���������
  */
  TruncStrFromEnd(HelpTitleBuf,X2-X1-3);
  /* DJ $ */
  GotoXY(X1+(X2-X1+1-strlen(HelpTitleBuf)-2)/2,Y1);
  mprintf(" %s ",HelpTitleBuf);
}

/* DJ $ */

/* $ 01.09.2000 SVS
  ����� ������ CtrlColorChar & CurColor
*/
void Help::OutString(const char *Str)
{
  char OutStr[512];
  const char *StartTopic=NULL;
  int OutPos=0,Highlight=0,Topic=0;
  while (OutPos<sizeof(OutStr)-10)
  {
    if (Str[0]=='~' && Str[1]=='~' ||
        Str[0]=='#' && Str[1]=='#' ||
        Str[0]=='@' && Str[1]=='@' ||
        (*CtrlColorChar && Str[0]==*CtrlColorChar && Str[1]==*CtrlColorChar)
       )
    {
      OutStr[OutPos++]=*Str;
      Str+=2;
      continue;
    }

    if (*Str=='~' || ((*Str=='#' || *Str == *CtrlColorChar) && !Topic) /*|| *Str==HelpBeginLink*/ || *Str==0)
    {
      OutStr[OutPos]=0;
      if (Topic)
      {
        int RealCurX,RealCurY;
        RealCurX=X1+StackData.CurX+1;
        RealCurY=Y1+StackData.CurY+FixSize+1;
        if (WhereY()==RealCurY && RealCurX>=WhereX() &&
                RealCurX<WhereX()+(Str-StartTopic)-1)
        {
          SetColor(COL_HELPSELECTEDTOPIC);
          if (Str[1]=='@')
          {
            xstrncpy(StackData.SelTopic,Str+2,sizeof(StackData.SelTopic)-1);
            char *EndPtr=strchr(StackData.SelTopic,'@');
            /* $ 25.08.2000 SVS
               �����, ��� ����� ���� ����� �������: @@ ��� \@
               ���� ������� ������ ��� URL!
            */
            if (EndPtr!=NULL)
            {
              if(*(EndPtr+1) == '@')
              {
                memmove(EndPtr,EndPtr+1,strlen(EndPtr)+1);
                EndPtr++;
              }
              EndPtr=strchr(EndPtr,'@');
              if (EndPtr!=NULL)
                *EndPtr=0;
            }
            /* SVS $ */
          }
        }
        else
          SetColor(COL_HELPTOPIC);
      }
      else
        if (Highlight)
          SetColor(COL_HELPHIGHLIGHTTEXT);
        else
          SetColor(CurColor);
      /* $ 24.09.2001 VVM
        ! ������� ������� ������ ��� ������. ����� ����� ������ ��� ������� �������... */
      if (static_cast<int>(strlen(OutStr) + WhereX()) > X2)
        OutStr[X2 - WhereX()] = 0;
      /* VVM $ */
      if (Locked())
        GotoXY(WhereX()+strlen(OutStr),WhereY());
      else
        Text(OutStr);
      OutPos=0;
    }

    if (*Str==0)
      break;

    if (*Str=='~')
    {
      if (!Topic)
        StartTopic=Str;
      Topic=!Topic;
      Str++;
      continue;
    }
    if (*Str=='@')
    {
      /* $ 25.08.2000 SVS
         �����, ��� ����� ���� ����� �������: @@
         ���� ������� ������ ��� URL!
      */
      while (*Str)
        if (*(++Str)=='@' && *(Str-1)!='@')
          break;
      /* SVS $ */
      Str++;
      continue;
    }
    if (*Str=='#')
    {
      Highlight=!Highlight;
      Str++;
      continue;
    }
    if (*Str == *CtrlColorChar)
    {
      WORD Chr;

      Chr=(BYTE)Str[1];
      if(Chr == '-') // "\-" - ���������� ���������� ����
      {
        Str+=2;
        CurColor=COL_HELPTEXT;
        continue;
      }
      if(isxdigit(Chr) && isxdigit(Str[2]))
      {
        WORD Attr;

        if(Chr >= '0' && Chr <= '9') Chr-='0';
        else { Chr&=~0x20; Chr=Chr-'A'+10; }
        Attr=(Chr<<4)&0x00F0;

        // next char
        Chr=Str[2];
        if(Chr >= '0' && Chr <= '9') Chr-='0';
        else { Chr&=~0x20; Chr=Chr-'A'+10; }
        Attr|=(Chr&0x000F);
        CurColor=Attr;
        Str+=3;
        continue;
      }
    }

    OutStr[OutPos++]=*(Str++);
  }
  if (!Locked() && WhereX()<X2)
  {
    SetColor(CurColor);
    mprintf("%*s",X2-WhereX(),"");
  }
}


int Help::StringLen(const char *Str)
{
  int Length=0;
  while (*Str)
  {
    if (Str[0]=='~' && Str[1]=='~' ||
        Str[0]=='#' && Str[1]=='#' ||
        Str[0]=='@' && Str[1]=='@' ||
        (*CtrlColorChar && Str[0]==*CtrlColorChar && Str[1]==*CtrlColorChar)
       )
    {
      Length++;
      Str+=2;
      continue;
    }
    if (*Str=='@')
    {
      /* $ 25.08.2000 SVS
         �����, ��� ����� ���� ����� �������: @@
         ���� ������� ������ ��� URL!
      */
      while (*Str)
        if (*(++Str)=='@' && *(Str-1)!='@')
          break;
      /* SVS $ */
      Str++;
      continue;
    }
    /* $ 01.09.2000 SVS
       ����� ���� ������������ \XX ��� \-
    */
    if(*Str == *CtrlColorChar)
    {
      if(Str[1] == '-')
      {
        Str+=2;
        continue;
      }

      if(isxdigit(Str[1]) && isxdigit(Str[2]))
      {
        Str+=3;
        continue;
      }
    }
    /* SVS $ */

    if (*Str!='#' && *Str!='~')
      Length++;
    Str++;
  }
  return(Length);
}


void Help::CorrectPosition()
{
  if (StackData.CurX>X2-X1-2)
    StackData.CurX=X2-X1-2;
  if (StackData.CurX<0)
    StackData.CurX=0;
  if (StackData.CurY>Y2-Y1-2-FixSize)
  {
    StackData.TopStr+=StackData.CurY-(Y2-Y1-2-FixSize);
    StackData.CurY=Y2-Y1-2-FixSize;
  }
  if (StackData.CurY<0)
  {
    StackData.TopStr+=StackData.CurY;
    StackData.CurY=0;
  }
  if (StackData.TopStr>StrCount-FixCount-(Y2-Y1-1-FixSize))
    StackData.TopStr=StrCount-FixCount-(Y2-Y1-1-FixSize);
  if (StackData.TopStr<0)
    StackData.TopStr=0;
}

#if defined(WORK_HELP_FIND)
/* SVS:
   ��������!!!!!
   ��� ������� ������ ����� (����� �������!)
   ���� � ���� ������ ��� ������� �� ��������� - ������� ������ ;-)
*/
int Help::Search(int Next)
{
  unsigned char SearchStr[SEARCHSTRINGBUFSIZE]
  char MsgStr[512];
  int SearchLength,Case,WholeWords,ReverseSearch,Match;

  if (Next && *LastSearchStr==0)
    return TRUE;

  Match=FALSE;
  xstrncpy((char *)SearchStr,(char *)LastSearchStr,sizeof(SearchStr));
  Case=LastSearchCase;
  WholeWords=LastSearchWholeWords;
  ReverseSearch=LastSearchReverse;

  if (!Next)
    if(!GetSearchReplaceString(FALSE,SearchStr,sizeof(SearchStr),
                   NULL,0,NULL,NULL,
                   NULL/*&Case*/,NULL/*&WholeWords*/,NULL/*&ReverseSearch*/))
      return FALSE;

  xstrncpy((char *)LastSearchStr,(char *)SearchStr,sizeof(LastSearchStr));
  LastSearchCase=Case;
  LastSearchWholeWords=WholeWords;
  LastSearchReverse=ReverseSearch;

  if ((SearchLength=strlen((char *)SearchStr))==0)
    return TRUE;
  else
  {
    SaveScreen SaveScr;
    SetCursorType(FALSE,0);
    char Buf[8192];

    sprintf(MsgStr,"\"%s\"",SearchStr);
    Message(0,0,MSG(MHelpSearchTitle),MSG(MHelpSearchingFor),MsgStr);

    if (!Case)
      for (int I=0;I<SearchLength;I++)
        SearchStr[I]=LocalUpper(SearchStr[I]);
/*
    //if(!ReadHelp(HelpMask))
    //fseek(ViewFile,LastSearchPos,SEEK_SET);

    while (!Match)
    {

    }
*/
  }
  if (!Match)
  {
    Message(MSG_DOWN|MSG_WARNING,1,MSG(MHelpFindTitle),
              MSG(MHelpSearchCannotFind),MsgStr,MSG(MOk));
    return FALSE;
  }

  // ������ � ���������� ������.
  return TRUE;
}
#endif

int Help::ProcessKey(int Key)
{
  if (*StackData.SelTopic==0)
    StackData.CurX=StackData.CurY=0;

  switch(Key)
  {
    case MCODE_V_HELPFILENAME: // Help.FileName
       return (int)(DWORD_PTR)FullHelpPathName;
    case MCODE_V_HELPTOPIC: // Help.Topic
       return (int)(DWORD_PTR)StackData.HelpTopic;
    case MCODE_V_HELPSELTOPIC: // Help.SELTopic
       return (int)(DWORD_PTR)StackData.SelTopic;
  }

  switch(Key)
  {
    case KEY_NONE:
    case KEY_IDLE:
    {
      break;
    }

    case KEY_F5:
    {
      Opt.FullScreenHelp=!Opt.FullScreenHelp;
      ResizeConsole();
      return(TRUE);
    }

    case KEY_ESC:
    case KEY_F10:
    {
      FrameManager->DeleteFrame();
      SetExitCode (XC_QUIT);
      return(TRUE);
    }

    case KEY_HOME:        case KEY_NUMPAD7:
    case KEY_CTRLHOME:    case KEY_CTRLNUMPAD7:
    case KEY_CTRLPGUP:    case KEY_CTRLNUMPAD9:
    {
      StackData.CurX=StackData.CurY=0;
      StackData.TopStr=0;
      FastShow();
      if (*StackData.SelTopic==0)
        MoveToReference(1,1);
      return(TRUE);
    }

    case KEY_END:         case KEY_NUMPAD1:
    case KEY_CTRLEND:     case KEY_CTRLNUMPAD1:
    case KEY_CTRLPGDN:    case KEY_CTRLNUMPAD3:
    {
      StackData.CurX=StackData.CurY=0;
      StackData.TopStr=StrCount;
      FastShow();
      if (*StackData.SelTopic==0)
      {
        StackData.CurX=0;
        StackData.CurY=Y2-Y1-2-FixSize;
        MoveToReference(0,1);
      }
      return(TRUE);
    }

    case KEY_UP:          case KEY_NUMPAD8:
    {
      if (StackData.TopStr>0)
      {
        StackData.TopStr--;
        if (StackData.CurY<Y2-Y1-2-FixSize)
        {
          StackData.CurX=X2-X1-2;
          StackData.CurY++;
        }
        FastShow();
        if (*StackData.SelTopic==0)
          MoveToReference(0,1);
      }
      else
        ProcessKey(KEY_SHIFTTAB);
      return(TRUE);
    }

    case KEY_DOWN:        case KEY_NUMPAD2:
    {
      if (StackData.TopStr<StrCount-FixCount-(Y2-Y1-1-FixSize))
      {
        StackData.TopStr++;
        if (StackData.CurY>0)
          StackData.CurY--;
        StackData.CurX=0;
        FastShow();
        if (*StackData.SelTopic==0)
          MoveToReference(1,1);
      }
      else
        ProcessKey(KEY_TAB);
      return(TRUE);
    }

    /* $ 26.07.2001 VVM
      + � ������ ������� �� 1 */
    /* $ 07.05.2001 DJ
      + ��������� KEY_MSWHEEL_XXXX */
    case KEY_MSWHEEL_UP:
    case (KEY_MSWHEEL_UP | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaHelp;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_UP);
      return(TRUE);
    }
    case KEY_MSWHEEL_DOWN:
    case (KEY_MSWHEEL_DOWN | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaHelp;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_DOWN);
      return(TRUE);
    }
    /* DJ $ */
    /* VVM $ */

    case KEY_PGUP:      case KEY_NUMPAD9:
    {
      StackData.CurX=StackData.CurY=0;
      StackData.TopStr-=Y2-Y1-2-FixSize;
      FastShow();
      if (*StackData.SelTopic==0)
      {
        StackData.CurX=StackData.CurY=0;
        MoveToReference(1,1);
      }
      return(TRUE);
    }

    case KEY_PGDN:      case KEY_NUMPAD3:
    {
      {
        int PrevTopStr=StackData.TopStr;
        StackData.TopStr+=Y2-Y1-2-FixSize;
        FastShow();
        if (StackData.TopStr==PrevTopStr)
        {
          ProcessKey(KEY_CTRLPGDN);
          return(TRUE);
        }
        else
          StackData.CurX=StackData.CurY=0;
        MoveToReference(1,1);
      }
      return(TRUE);
    }

    case KEY_RIGHT:   case KEY_NUMPAD6:
    case KEY_TAB:
    {
      MoveToReference(1,0);
      return(TRUE);
    }

    case KEY_LEFT:    case KEY_NUMPAD4:
    case KEY_SHIFTTAB:
    {
      MoveToReference(0,0);
      return(TRUE);
    }

    case KEY_F1:
    {
      // �� ������� SelTopic, ���� � ��� � Help on Help
      if(LocalStricmp(StackData.HelpTopic,HelpOnHelpTopic)!=0)
      {
        Stack->Push(&StackData);
        IsNewTopic=TRUE;
        JumpTopic(HelpOnHelpTopic);
        IsNewTopic=FALSE;
        ErrorHelp=FALSE;
      }
      return(TRUE);
    }

    case KEY_SHIFTF1:
    {
      //   �� ������� SelTopic, ���� � ��� � ���� Contents
      if(LocalStricmp(StackData.HelpTopic,HelpContents)!=0)
      {
        Stack->Push(&StackData);
        IsNewTopic=TRUE;
        JumpTopic(HelpContents);
        ErrorHelp=FALSE;
        IsNewTopic=FALSE;
      }
      return(TRUE);
    }

    case KEY_SHIFTF2:
    {
      //   �� ������� SelTopic, ���� � ��� � PluginContents
      if(LocalStricmp(StackData.HelpTopic,PluginContents)!=0)
      {
        Stack->Push(&StackData);
        IsNewTopic=TRUE;
        JumpTopic(PluginContents);
        ErrorHelp=FALSE;
        IsNewTopic=FALSE;
      }
      return(TRUE);
    }

#if defined(WORK_HELP_DOCUMS)
    case KEY_SHIFTF3: // ��� "����������" :-)
    {
      //   �� ������� SelTopic, ���� � ��� � DocumentContents
      if(LocalStricmp(StackData.HelpTopic,DocumentContents)!=0)
      {
        Stack->Push(&StackData);
        IsNewTopic=TRUE;
        JumpTopic(DocumentContents);
        ErrorHelp=FALSE;
        IsNewTopic=FALSE;
      }
      return(TRUE);
    }
#endif

    case KEY_ALTF1:
    case KEY_BS:
    {
      // ���� ���� �������� ���� - ������� �� �����
      if(!Stack->isEmpty())
      {
        Stack->Pop(&StackData);
        JumpTopic(StackData.HelpTopic);
        ErrorHelp=FALSE;
        return(TRUE);
      }
      return ProcessKey(KEY_ESC);
    }

    case KEY_ENTER:
    {
      if (*StackData.SelTopic && LocalStricmp(StackData.HelpTopic,StackData.SelTopic)!=0)
      {
        Stack->Push(&StackData);
        IsNewTopic=TRUE;
        if (!JumpTopic())
        {
          Stack->Pop(&StackData);
          ReadHelp(StackData.HelpMask); // ������ ��, ��� ����������.
        }
        ErrorHelp=FALSE;
        IsNewTopic=FALSE;
      }
      return(TRUE);
    }

#if defined(WORK_HELP_FIND)
    case KEY_F7:
    {
      Search(0);
      return(TRUE);
    }

    case KEY_SHIFTF7:
    {
      Search(1);
      return(TRUE);
    }
#endif

  }
  return(FALSE);
}

int Help::JumpTopic(const char *JumpTopic)
{
  char NewTopic[512];
  char *p;

  Stack->PrintStack(JumpTopic);

  if(JumpTopic)
    strcpy(StackData.SelTopic,JumpTopic);
  /* $ 14.07.2002 IS
       ��� �������� �� ������� ���������� ������ ������ ���������� ����,
       ���� ��� ��������.
  */
  // ���� ������ �� ������ ����, ���� ������������� � ���� ��, �� ���� �����
  // ��������� ���������� ����, �� ������� ���
  if(*StackData.SelTopic==HelpBeginLink &&
     NULL!=(p=strchr(StackData.SelTopic+2,HelpEndLink))&&
     !PathMayBeAbsolute(StackData.SelTopic+1) &&
     *StackData.HelpPath)
  {
    char FullPath[sizeof(NewTopic)*2];
    xstrncpy(NewTopic, StackData.SelTopic+1,p-StackData.SelTopic-1);
    strcpy(FullPath,StackData.HelpPath);

    // ������ _���_ �������� ����� � ������� ����
    int Len=strlen(FullPath)-1;
    while(Len>-1 && FullPath[Len]=='\\')
    {
      FullPath[Len]=0;
      --Len;
    }
    if(Len<0)
      Len=0;
    else
      ++Len;
    FullPath[Len]='\\';
    FullPath[Len+1]=0;

    strcat(FullPath,NewTopic+((*NewTopic=='\\' || *NewTopic=='/')?1:0));
    BOOL addSlash=DeleteEndSlash(FullPath);
    if(ConvertNameToFull(FullPath,NewTopic,sizeof(NewTopic))<sizeof(NewTopic))
    {
      sprintf(FullPath,addSlash?HelpFormatLink:HelpFormatLinkModule,
        NewTopic,p+1);
      xstrncpy(StackData.SelTopic,FullPath,sizeof(StackData.SelTopic));
    }
  }
  /* IS 14.07.2002 $ */
//_SVS(SysLog("JumpTopic() = SelTopic=%s",StackData.SelTopic));
  // URL ��������� - ��� ���� ��� ������ :-)))
  {
    strcpy(NewTopic,StackData.SelTopic);
    p=strchr(NewTopic,':');
    if(p && NewTopic[0] != ':') // �������� ��������������� URL
    {
      *p=0;
      if(RunURL(NewTopic,StackData.SelTopic))
        return(FALSE);
      *p=':';
    }
  }
  // � ��� ������ ���������...

//_SVS(SysLog("JumpTopic() = SelTopic=%s, StackData.HelpPath=%s",StackData.SelTopic,StackData.HelpPath));
  if (*StackData.HelpPath && *StackData.SelTopic!=HelpBeginLink && strcmp(StackData.SelTopic,HelpOnHelpTopic)!=0)
  {
    if (*StackData.SelTopic==':')
    {
      strcpy(NewTopic,StackData.SelTopic+1);
      StackData.Flags&=~FHELP_CUSTOMFILE;
    }
    else if(StackData.Flags&FHELP_CUSTOMFILE)
      strcpy(NewTopic,StackData.SelTopic);
    else
      sprintf(NewTopic,HelpFormatLink,StackData.HelpPath,StackData.SelTopic);
  }
  else
  {
    strcpy(NewTopic,StackData.SelTopic+(!strcmp(StackData.SelTopic,HelpOnHelpTopic)?1:0));
  }

  // ������ ������ �� .DLL
  p=strrchr(NewTopic,HelpEndLink);
  if(p)
  {
    if(*(p-1) != '\\')
    {
      char *p2=p;
      while(p >= NewTopic)
      {
        if(*p == '\\')
        {
//          ++p;
          if(*p)
          {
            StackData.Flags|=FHELP_CUSTOMFILE;
            strcpy(StackData.HelpMask,p+1);
            *strrchr(StackData.HelpMask,HelpEndLink)=0;
          }
          memmove(p,p2,strlen(p2)+1);
          p=strrchr(StackData.HelpMask,'.');
          if(p && stricmp(p,".hlf"))
            *StackData.HelpMask=0;
          break;
        }
        --p;
      }
    }
    else
    {
      StackData.Flags&=~FHELP_CUSTOMFILE;
      StackData.Flags|=FHELP_CUSTOMPATH;
    }
  }

//_SVS(SysLog("HelpMask=%s NewTopic=%s",StackData.HelpMask,NewTopic));
  if(*StackData.SelTopic != ':' &&
     LocalStricmp(StackData.SelTopic,PluginContents)
#if defined(WORK_HELP_DOCUMS)
     && LocalStricmp(StackData.SelTopic,DocumentContents)
#endif
    )
  {
    if(!(StackData.Flags&FHELP_CUSTOMFILE) && strrchr(NewTopic,HelpEndLink))
    {
      if(StackData.HelpMask)
        *StackData.HelpMask=0;
    }
  }
  else
  {
    if(StackData.HelpMask)
      *StackData.HelpMask=0;
  }
  strcpy(StackData.HelpTopic,NewTopic);
  if(!(StackData.Flags&FHELP_CUSTOMFILE))
    *StackData.HelpPath=0;
  if(!ReadHelp(StackData.HelpMask))
  {
    strcpy(StackData.HelpTopic,NewTopic);
    if(*StackData.HelpTopic == HelpBeginLink)
    {
      char *Ptr=strrchr(StackData.HelpTopic,HelpEndLink);
      if(Ptr)
        strcpy(++Ptr,HelpContents);
    }
    *StackData.HelpPath=0;
    ReadHelp(StackData.HelpMask);
  }
  ScreenObject::Flags.Clear(FHELPOBJ_ERRCANNOTOPENHELP);

  if (!HelpData)
  {
    ErrorHelp=TRUE;
    if(!(StackData.Flags&FHELP_NOSHOWERROR))
    {
      BlockExtKey blockExtKey;
      Message(MSG_WARNING,1,MSG(MHelpTitle),MSG(MHelpTopicNotFound),StackData.HelpTopic,MSG(MOk));
    }
    return FALSE;
  }
//  ResizeConsole();
  if(IsNewTopic
    || !LocalStricmp(StackData.SelTopic,PluginContents) // ��� ���������� ������� :-((
#if defined(WORK_HELP_DOCUMS)
    || !LocalStricmp(StackData.SelTopic,DocumentContents)
#endif
    )
    MoveToReference(1,1);

  //FrameManager->ImmediateHide();
  FrameManager->RefreshFrame();

  return TRUE;
}



int Help::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  if (HelpKeyBar.ProcessMouse(MouseEvent))
    return TRUE;

  if (MouseEvent->dwEventFlags==MOUSE_MOVED &&
     (MouseEvent->dwButtonState & MOUSE_ANY_BUTTON_PRESSED)==0)
    return(FALSE);
  /* $ 26.10.2001 VVM
    + ������� ������� ������� ������ �� ����� */
  if (MouseEvent->dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
  {
    ProcessKey(KEY_ENTER);
    return(TRUE);
  }
  /* VVM $ */

  int MsX,MsY;
  MsX=MouseEvent->dwMousePosition.X;
  MsY=MouseEvent->dwMousePosition.Y;
  if ((MsX<X1 || MsY<Y1 || MsX>X2 || MsY>Y2) && MouseEventFlags != MOUSE_MOVED)
  {
    if (Flags.Check(HELPMODE_CLICKOUTSIDE))
    {
      // ���������� ���� ���������� ����� �� ��� ������� ������
      if(PreMouseEventFlags != DOUBLE_CLICK)
        ProcessKey(KEY_ESC);
    }
    if (MouseEvent->dwButtonState)
      Flags.Set(HELPMODE_CLICKOUTSIDE);
    return(TRUE);
  }

  if (MouseX==X2 && (MouseEvent->dwButtonState & 1))
  {
    int ScrollY=Y1+FixSize+1;
    int Height=Y2-Y1-FixSize-1;
    if (MouseY==ScrollY)
    {
      while (IsMouseButtonPressed())
        ProcessKey(KEY_UP);
      return(TRUE);
    }
    if (MouseY==ScrollY+Height-1)
    {
      while (IsMouseButtonPressed())
        ProcessKey(KEY_DOWN);
      return(TRUE);
    }
  }

  /* $ 15.03.2002 DJ
     ���������� ������ � �������� ����������
  */
  if (MouseX == X2)
  {
    int ScrollY=Y1+FixSize+1;
    int Height=Y2-Y1-FixSize-1;
    if (StrCount > Height)
    {
      while (IsMouseButtonPressed())
      {
        if(MouseY > ScrollY && MouseY < ScrollY+Height+1)
        {
          StackData.CurX=StackData.CurY=0;
          StackData.TopStr=(MouseY-ScrollY-1) * (StrCount-FixCount-Height+1) / (Height-2);
          FastShow();
        }
      }
      return TRUE;
    }
  }
  /* DJ $ */

  // DoubliClock - ��������/���������� ����.
  if (MouseEvent->dwEventFlags==DOUBLE_CLICK &&
      (MouseEvent->dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) &&
      MouseEvent->dwMousePosition.Y<Y1+1+FixSize)
  {
    ProcessKey(KEY_F5);
    return(TRUE);
  }
  if (MouseEvent->dwMousePosition.Y<Y1+1+FixSize)
  {
    while (IsMouseButtonPressed() && MouseY<Y1+1+FixSize)
      ProcessKey(KEY_UP);
    return(TRUE);
  }
  if (MouseEvent->dwMousePosition.Y>=Y2)
  {
    while (IsMouseButtonPressed() && MouseY>=Y2)
      ProcessKey(KEY_DOWN);
    return(TRUE);
  }
  StackData.CurX=MouseEvent->dwMousePosition.X-X1-1;
  StackData.CurY=MouseEvent->dwMousePosition.Y-Y1-1-FixSize;
  FastShow();
  /* $ 26.11.2001 VVM
    + ��������� ������� ������� ����� � ������ � ���� ������ ����������� ��� ���������� */
  if (MouseEvent->dwEventFlags==0 &&
     (MouseEvent->dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED|RIGHTMOST_BUTTON_PRESSED)))
    MouseDown = TRUE;
  if (MouseEvent->dwEventFlags==0 &&
     (MouseEvent->dwButtonState & (FROM_LEFT_1ST_BUTTON_PRESSED|RIGHTMOST_BUTTON_PRESSED))==0 &&
      MouseDown &&
      *StackData.SelTopic)
  {
    MouseDown = FALSE;
    ProcessKey(KEY_ENTER);
  }
  /* VVM $ */
//  if ((MouseEvent->dwButtonState & 3)==0 && *StackData.SelTopic)
//    ProcessKey(KEY_ENTER);
  return(TRUE);
}


int Help::IsReferencePresent()
{
  CorrectPosition();
  int StrPos=FixCount+StackData.TopStr+StackData.CurY;
  /* $ 19.09.2000 OT
    ������ ��� ��������� �����
    */
  if (StrPos >= StrCount) {
    return FALSE;
  }
  /* OT 19.09.2000 $ */
  char *OutStr=HelpData+StrPos*MAX_HELP_STRING_LENGTH;
  return (strchr(OutStr,'@')!=NULL && strchr(OutStr,'~')!=NULL);
}


void Help::MoveToReference(int Forward,int CurScreen)
{
  int StartSelection=*StackData.SelTopic;
  int SaveCurX=StackData.CurX;
  int SaveCurY=StackData.CurY;
  int SaveTopStr=StackData.TopStr;
  BOOL ReferencePresent;

  *StackData.SelTopic=0;
  Lock ();

  if(!ErrorHelp) while (*StackData.SelTopic==0)
  {
    ReferencePresent=IsReferencePresent();
    if (Forward)
    {
      if (StackData.CurX==0 && !ReferencePresent)
        StackData.CurX=X2-X1-2;
      if (++StackData.CurX >= X2-X1-2)
      {
        StartSelection=0;
        StackData.CurX=0;
        StackData.CurY++;
        if (StackData.TopStr+StackData.CurY>=StrCount-FixCount ||
            CurScreen && StackData.CurY>Y2-Y1-2-FixSize)
          break;
      }
    }
    else
    {
      if (StackData.CurX==X2-X1-2 && !ReferencePresent)
        StackData.CurX=0;
      if (--StackData.CurX < 0)
      {
        StartSelection=0;
        StackData.CurX=X2-X1-2;
        StackData.CurY--;
        if (StackData.TopStr+StackData.CurY<0 ||
            CurScreen && StackData.CurY<0)
          break;
      }
    }

    FastShow();

    if (*StackData.SelTopic==0)
      StartSelection=0;
    else
    {
      // ��������� �������, ��������� ���� �� ��� ������ :-)
      if(ReferencePresent && CurScreen)
        StartSelection=0;

      if (StartSelection)
        *StackData.SelTopic=0;
    }
  }
  Unlock ();
  if (*StackData.SelTopic==0)
  {
    StackData.CurX=SaveCurX;
    StackData.CurY=SaveCurY;
    StackData.TopStr=SaveTopStr;
  }
  FastShow();
}

void Help::ReadDocumentsHelp(int TypeIndex)
{
  if(HelpData)
    xf_free(HelpData);
  HelpData=NULL;
  /* $ 29.11.2001 DJ
     ��� �� ������ -> ������ CurPluginContents
  */
  *CurPluginContents = '\0';
  /* DJ $ */

  StrCount=0;
  FixCount=1;
  FixSize=2;
  StackData.TopStr=0;
  TopicFound=TRUE;
  StackData.CurX=StackData.CurY=0;
  CtrlColorChar[0]=0;

  char *PtrTitle=0, *ContentsName=0;
  char Path[NM],FullFileName[NM],*Slash;
  char EntryName[512],HelpLine[512],SecondParam[512];

  switch(TypeIndex)
  {
    case HIDX_PLUGINS:
      PtrTitle=MSG(MPluginsHelpTitle);
      ContentsName="PluginContents";
      break;
#if defined(WORK_HELP_DOCUMS)
    case HIDX_DOCUMS:
      PtrTitle=MSG(MDocumentsHelpTitle);
      ContentsName="DocumentContents";
      break;
#endif
  }

  AddTitle(PtrTitle);

  /* TODO:
     1. ����� (��� "����������") �� ������ � �������� Documets, ��
        � � ��������
  */
  int OldStrCount=StrCount;
  switch(TypeIndex)
  {
    case HIDX_PLUGINS:
    {
      for (int I=0;I<CtrlObject->Plugins.PluginsCount;I++)
      {
        strcpy(Path,CtrlObject->Plugins.PluginsData[I].ModuleName);
        if ((Slash=strrchr(Path,'\\'))!=NULL)
//          *++Slash=0;
          *Slash=0;
        FILE *HelpFile=Language::OpenLangFile(Path,HelpFileMask,Opt.HelpLanguage,FullFileName);
        if (HelpFile!=NULL)
        {
          if (Language::GetLangParam(HelpFile,ContentsName,EntryName,SecondParam))
          {
            if (*SecondParam)
              sprintf(HelpLine,"   ~%s,%s~@" HelpFormatLink "@",EntryName,SecondParam,Path,HelpContents);
            else
              sprintf(HelpLine,"   ~%s~@" HelpFormatLink "@",EntryName,Path,HelpContents);
            AddLine(HelpLine);
          }

          fclose(HelpFile);
        }
      }
      break;
    }

#if defined(WORK_HELP_DOCUMS)
    case HIDX_DOCUMS:
    {
      // � ��������.
      for (int I=0;I<CtrlObject->Plugins.PluginsCount;I++)
      {
        strcpy(Path,CtrlObject->Plugins.PluginsData[I].ModuleName);
        if ((Slash=strrchr(Path,'\\'))!=NULL)
          *++Slash=0;
        FILE *HelpFile=Language::OpenLangFile(Path,HelpFileMask,Opt.HelpLanguage,FullFileName);
        if (HelpFile!=NULL)
        {
          if (Language::GetLangParam(HelpFile,ContentsName,EntryName,SecondParam))
          {
            if (*SecondParam)
              sprintf(HelpLine,"   ~%s,%s~@<%s>%s@",EntryName,SecondParam,FullFileName,HelpContents);
            else
              sprintf(HelpLine,"   ~%s~@<%s>%s@",EntryName,FullFileName,HelpContents);
            AddLine(HelpLine);
          }

          fclose(HelpFile);
        }
      }

      // � ����������.
      {
        WIN32_FIND_DATA FindData;
        char FMask[NM], *PtrPath;
        AddEndSlash(strcpy(Path,FarPath));
        strcat(Path,"Doc");
        ScanTree ScTree(FALSE,FALSE);
        ScTree.SetFindPath(Path,HelpFileMask);
        while (ScTree.GetNextName(&FindData,FullFileName))
        {
          if((PtrPath=strrchr(FullFileName,'\\')) != NULL)
          {
            xstrncpy(FMask,PtrPath+1,sizeof(FMask)-1);
            *++PtrPath=0;
          }
          else
            xstrncpy(FMask,HelpFileMask,sizeof(FMask)-1);

          FILE *HelpFile=Language::OpenLangFile(Path,FMask,Opt.HelpLanguage,FullFileName,TRUE);
          if (HelpFile!=NULL)
          {
            if (Language::GetLangParam(HelpFile,ContentsName,EntryName,SecondParam))
            {
              if (*SecondParam)
                sprintf(HelpLine,"   ~%s,%s~@<%s>%s@",EntryName,SecondParam,FullFileName,HelpContents);
              else
                sprintf(HelpLine,"   ~%s~@<%s>%s@",EntryName,FullFileName,HelpContents);

              AddLine(HelpLine);
            }
            fclose(HelpFile);
          }
        }
      }
      StackData.Flags|=FHELP_CUSTOMFILE;
      break;
    }
#endif
  }
  // ��������� �� ��������
  far_qsort(HelpData+OldStrCount*MAX_HELP_STRING_LENGTH,StrCount-OldStrCount,MAX_HELP_STRING_LENGTH,(int (__cdecl*)(const void *,const void *))LCStricmp);
  /* $ 26.06.2000 IS
   ���������� ����� � ������ �� f1, shift+f2, end (������� ��������� IG)
  */
  AddLine("");
  /* IS $ */
}

// ������������ ������ � ������ ������ ��������
char *Help::MkTopic(int PluginNumber,const char *HelpTopic,char *Topic)
{
  *Topic=0;
  if (HelpTopic && *HelpTopic)
  {
    if (*HelpTopic==':')
      strcpy(Topic,HelpTopic+1);
    else
    {
      if(PluginNumber != -1 && *HelpTopic!=HelpBeginLink)
      {
         sprintf(Topic,HelpFormatLinkModule,
                CtrlObject->Plugins.PluginsData[PluginNumber].ModuleName,
                HelpTopic);
      }
      else
        xstrncpy(Topic,HelpTopic,511);

      if(*Topic==HelpBeginLink)
      {
        char *Ptr, *Ptr2;
        if((Ptr=strchr(Topic,HelpEndLink)) == NULL)
          *Topic=0;
        else
        {
          if(!Ptr[1]) // ���� ��� ������� ��...
            strcat(Topic,HelpContents); // ... ������ ������� �������� ����.

          /* � ��� ������ ���������...
             ������ ����� ���� :
               "<FullPath>Topic" ��� "<FullModuleName>Topic"
             ��� ������ "FullPath" ���� ������ ������������� ������!
             �.�. �� ������� ��� ��� - ��� ������ ��� ����!
          */
          Ptr2=Ptr-1;
          if(*Ptr2 != '\\') // ��� ��� ������?
          {
            // ������ ������ ��� ������� ��� :-)
            if((Ptr2=strrchr(Topic,'\\')) == NULL) // ��! ����� �����-�� :-(
              *Topic=0;
          }
          if(*Topic)
          {
            /* $ 21.08.2001 KM
              - ������� ���������� ����� � ������ ������ �������,
                � ������� ���� ��� ������ ������ ������������� "/".
            */
            memmove(Ptr2+1,Ptr,strlen(Ptr)+1); //???
            /* KM $ */

            // � ��� ����� ������ ��� �� �������� Help API!
          }
        }
      }
    }
  }
  return *Topic?Topic:NULL;
}


/* $ 28.06.2000 tran
 (NT Console resize)
 resize help*/
void Help::SetScreenPosition()
{
  if (Opt.FullScreenHelp)
  {
    HelpKeyBar.Hide();
    SetPosition(0,0,ScrX,ScrY);
  }
  else
    SetPosition(4,2,ScrX-4,ScrY-2);
  Show();
}
/* tran $ */

/* $ 30.12.2000 SVS
  ������� ������������� KeyBar Labels
*/
void Help::InitKeyBar(void)
{
  char *FHelpKeys[]={MSG(MHelpF1),MSG(MHelpF2),MSG(MHelpF3),MSG(MHelpF4),MSG(MHelpF5),MSG(MHelpF6),MSG(MHelpF7),MSG(MHelpF8),MSG(MHelpF9),MSG(MHelpF10),MSG(MHelpF11),MSG(MHelpF12)};
  char *FHelpShiftKeys[]={MSG(MHelpShiftF1),MSG(MHelpShiftF2),MSG(MHelpShiftF3),MSG(MHelpShiftF4),MSG(MHelpShiftF5),MSG(MHelpShiftF6),MSG(MHelpShiftF7),MSG(MHelpShiftF8),MSG(MHelpShiftF9),MSG(MHelpShiftF10),MSG(MHelpShiftF11),MSG(MHelpShiftF12)};
  char *FHelpAltKeys[]={MSG(MHelpAltF1),MSG(MHelpAltF2),MSG(MHelpAltF3),MSG(MHelpAltF4),MSG(MHelpAltF5),MSG(MHelpAltF6),MSG(MHelpAltF7),MSG(MHelpAltF8),MSG(MHelpAltF9),MSG(MHelpAltF10),MSG(MHelpAltF11),MSG(MHelpAltF12)};
  char *FHelpCtrlKeys[]={MSG(MHelpCtrlF1),MSG(MHelpCtrlF2),MSG(MHelpCtrlF3),MSG(MHelpCtrlF4),MSG(MHelpCtrlF5),MSG(MHelpCtrlF6),MSG(MHelpCtrlF7),MSG(MHelpCtrlF8),MSG(MHelpCtrlF9),MSG(MHelpCtrlF10),MSG(MHelpCtrlF11),MSG(MHelpCtrlF12)};
//  char *FHelpAltShiftKeys[]={MSG(MHelpAltShiftF1),MSG(MHelpAltShiftF2),MSG(MHelpAltShiftF3),MSG(MHelpAltShiftF4),MSG(MHelpAltShiftF5),MSG(MHelpAltShiftF6),MSG(MHelpAltShiftF7),MSG(MHelpAltShiftF8),MSG(MHelpAltShiftF9),MSG(MHelpAltShiftF10),MSG(MHelpAltShiftF11),MSG(MHelpAltShiftF12)};
//  char *FHelpCtrlShiftKeys[]={MSG(MHelpCtrlShiftF1),MSG(MHelpCtrlShiftF2),MSG(MHelpCtrlShiftF3),MSG(MHelpCtrlShiftF4),MSG(MHelpCtrlShiftF5),MSG(MHelpCtrlShiftF6),MSG(MHelpCtrlShiftF7),MSG(MHelpCtrlShiftF8),MSG(MHelpCtrlShiftF9),MSG(MHelpCtrlShiftF10),MSG(MHelpCtrlShiftF11),MSG(MHelpCtrlShiftF12)};
//  char *FHelpCtrlAltKeys[]={MSG(MHelpCtrlAltF1),MSG(MHelpCtrlAltF2),MSG(MHelpCtrlAltF3),MSG(MHelpCtrlAltF4),MSG(MHelpCtrlAltF5),MSG(MHelpCtrlAltF6),MSG(MHelpCtrlAltF7),MSG(MHelpCtrlAltF8),MSG(MHelpCtrlAltF9),MSG(MHelpCtrlAltF10),MSG(MHelpCtrlAltF11),MSG(MHelpCtrlAltF12)};
  char *FHelpAltShiftKeys[]={"","","","","","","","","","","",""};
  char *FHelpCtrlShiftKeys[]={"","","","","","","","","","","",""};
  char *FHelpCtrlAltKeys[]={"","","","","","","","","","","",""};

  // ������ ������ � ���� �����
#if !defined(WORK_HELP_DOCUMS)
  FHelpShiftKeys[3-1][0]=0;
#endif
#if !defined(WORK_HELP_FIND)
  FHelpKeys[7-1][0]=0;
  FHelpShiftKeys[7-1][0]=0;
#endif

  HelpKeyBar.Set(FHelpKeys,sizeof(FHelpKeys)/sizeof(FHelpKeys[0]));
  HelpKeyBar.SetShift(FHelpShiftKeys,sizeof(FHelpShiftKeys)/sizeof(FHelpShiftKeys[0]));
  HelpKeyBar.SetAlt(FHelpAltKeys,sizeof(FHelpAltKeys)/sizeof(FHelpAltKeys[0]));
  HelpKeyBar.SetCtrl(FHelpCtrlKeys,sizeof(FHelpCtrlKeys)/sizeof(FHelpCtrlKeys[0]));
  HelpKeyBar.SetCtrlAlt(FHelpCtrlAltKeys,sizeof(FHelpCtrlAltKeys)/sizeof(FHelpCtrlAltKeys[0]));
  HelpKeyBar.SetCtrlShift(FHelpCtrlShiftKeys,sizeof(FHelpCtrlShiftKeys)/sizeof(FHelpCtrlShiftKeys[0]));
  HelpKeyBar.SetAltShift(FHelpAltShiftKeys,sizeof(FHelpAltShiftKeys)/sizeof(FHelpAltShiftKeys[0]));

  SetKeyBar(&HelpKeyBar);
}
/* SVS $ */

/* $ 25.08.2000 SVS
   ������ URL-������... ;-)
   ��� ���� ��� ������... ���?
   ������:
     0 - ��� �� URL ������ (�� ������)
     1 - CreateProcess ������ FALSE
     2 - ��� ��

   ��������� (��������):
     Protocol="mailto"
     URLPath ="mailto:vskirdin@mail.ru?Subject=Reversi"
*/
static int RunURL(char *Protocol, char *URLPath)
{
  int EditCode=0;
  if(Protocol && *Protocol && URLPath && *URLPath && (Opt.HelpURLRules&0xFF))
  {
    char *Buf=(char*)xf_malloc(2048);
    if(Buf)
    {
      HKEY hKey;
      DWORD Disposition, DataSize=250;
      strcpy(Buf,Protocol);
      strcat(Buf,"\\shell\\open\\command");
      if(RegOpenKeyEx(HKEY_CLASSES_ROOT,Buf,0,KEY_READ,&hKey) == ERROR_SUCCESS)
      {
        Disposition=RegQueryValueEx(hKey,"",0,&Disposition,(LPBYTE)Buf,&DataSize);
        ExpandEnvironmentStr(Buf, Buf, 2048);
        RegCloseKey(hKey);
        if(Disposition == ERROR_SUCCESS)
        {
          char *pp=strrchr(Buf,'%');
          if(pp) *pp='\0'; else strcat(Buf," ");

          // ������ ��� ������ � ������ ~~
          pp=URLPath;
          while(*pp && (pp=strstr(pp,"~~")) != NULL)
          {
            memmove(pp,pp+1,strlen(pp+1)+1);
            ++pp;
          }
          // ������ ��� ������ � ������ ##
          pp=URLPath;
          while(*pp && (pp=strstr(pp,"##")) != NULL)
          {
            memmove(pp,pp+1,strlen(pp+1)+1);
            ++pp;
          }

          Disposition=0;
          if(Opt.HelpURLRules == 2 || Opt.HelpURLRules == 2+256)
          {
            BlockExtKey blockExtKey;
            Disposition=Message(MSG_WARNING,2,MSG(MHelpTitle),
                        MSG(MHelpActivatorURL),
                        Buf,
                        MSG(MHelpActivatorFormat),
                        URLPath,
                        "\x01",
                        MSG(MHelpActivatorQ),
                        MSG(MYes),MSG(MNo));
          }
          EditCode=2; // ��� Ok!
          if(Disposition == 0)
          {
            /*
              ���� ����� ���������� ������ � ������������ ������
              ���� ��� ����� ���������� - �� ����� ���� ���������!!!!!
            */
            if(Opt.HelpURLRules < 256) // SHELLEXECUTEEX_METHOD
            {
#if 0
              SHELLEXECUTEINFO sei;

              FAR_OemToChar(URLPath,Buf);
              memset(&sei,0,sizeof(sei));
              sei.cbSize=sizeof(sei);
              sei.fMask=SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_DDEWAIT;
              sei.lpFile=RemoveExternalSpaces(Buf);
              sei.nShow=SW_SHOWNORMAL;
              SetFileApisTo(APIS2ANSI);
              if(ShellExecuteEx(&sei))
                EditCode=1;
              SetFileApisTo(APIS2OEM);
#else
              FAR_OemToChar(URLPath,Buf);
              SetFileApisTo(APIS2ANSI);
              EditCode=ShellExecute(0, 0, RemoveExternalSpaces(Buf), 0, 0, SW_SHOWNORMAL)?1:2;
              SetFileApisTo(APIS2OEM);
#endif
            }
            else
            {
              STARTUPINFO si={0};
              PROCESS_INFORMATION pi={0};
              si.cb=sizeof(si);
              strcat(Buf,URLPath);
              FAR_OemToChar(Buf,Buf);
              SetFileApisTo(APIS2ANSI); //????
              if(!CreateProcess(NULL,Buf,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi))
                 EditCode=1;
              SetFileApisTo(APIS2OEM); //????
            }
          }
        }
      }
      xf_free(Buf);
    }
  }
  return EditCode;
}

void Help::OnChangeFocus(int Focus)
{
  if (Focus)
  {
    DisplayObject();
  }
}

void Help::ResizeConsole()
{
  int OldIsNewTopic=IsNewTopic;
  BOOL ErrCannotOpenHelp=ScreenObject::Flags.Check(FHELPOBJ_ERRCANNOTOPENHELP);
  ScreenObject::Flags.Set(FHELPOBJ_ERRCANNOTOPENHELP);
  IsNewTopic=FALSE;
  delete TopScreen;
  TopScreen=NULL;

  Hide();
  if (Opt.FullScreenHelp)
  {
    HelpKeyBar.Hide();
    SetPosition(0,0,ScrX,ScrY);
  }
  else
    SetPosition(4,2,ScrX-4,ScrY-2);
  ReadHelp(StackData.HelpMask);
  ErrorHelp=FALSE;
//  StackData.CurY--; // ��� ���� ������� (����� ���� ����� ���!)
  StackData.CurX--;
  MoveToReference(1,1);
  IsNewTopic=OldIsNewTopic;
  ScreenObject::Flags.Change(FHELPOBJ_ERRCANNOTOPENHELP,ErrCannotOpenHelp);
  FrameManager->ImmediateHide();
  FrameManager->RefreshFrame();
}

int Help::FastHide()
{
  return Opt.AllCtrlAltShiftRule & CASR_HELP;
}


int Help::GetTypeAndName(char *Type,char *Name)
{
  if(Type)
    strcpy(Type,MSG(MHelpType));
  if(Name)
    xstrncpy(Name,FullHelpPathName,NM-1);
  return(MODALTYPE_HELP);
}

/* ------------------------------------------------------------------ */
void CallBackStack::ClearStack()
{
  while(!isEmpty())
    Pop();
}

int CallBackStack::Pop(struct StackHelpData *Dest)
{
  if(!isEmpty())
  {
    ListNode *oldTop = topOfStack;
    topOfStack = topOfStack->Next;
    if(Dest!=NULL)
    {
      strcpy(Dest->HelpTopic,oldTop->HelpTopic);
      strcpy(Dest->HelpPath,oldTop->HelpPath);
      strcpy(Dest->SelTopic,oldTop->SelTopic);
      strcpy(Dest->HelpMask,oldTop->HelpMask);

      Dest->Flags=oldTop->Flags;
      Dest->TopStr=oldTop->TopStr;
      Dest->CurX=oldTop->CurX;
      Dest->CurY=oldTop->CurY;
    }
    delete oldTop;
    return TRUE;
  }
  return FALSE;
}

void CallBackStack::Push(const struct StackHelpData *Data)
{
  topOfStack=new ListNode(Data,topOfStack);
}

void CallBackStack::PrintStack(const char *Title)
{
#if defined(SYSLOG)
  int I=0;
  ListNode *Ptr = topOfStack;
  SysLog("Return Stack (%s)",Title);
  SysLog(1);
  while(Ptr)
  {
    SysLog("%03d HelpTopic='%s' HelpPath='%s' HelpMask='%s'",I++,Ptr->HelpTopic,Ptr->HelpPath,Ptr->HelpMask);
    Ptr=Ptr->Next;
  }
  SysLog(-1);
#endif
}
