/*
editor.cpp

��������

*/

#include "headers.hpp"
#pragma hdrstop

#include "editor.hpp"
#include "edit.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "macroopcode.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "poscache.hpp"
#include "chgprior.hpp"
#include "filestr.hpp"
#include "dialog.hpp"
#include "fileedit.hpp"
#include "savescr.hpp"
#include "scrbuf.hpp"
#include "farexcpt.hpp"

static struct CharTableSet InitTableSet;

static int ReplaceMode,ReplaceAll;

static int EditorID=0;

// struct EditorUndoData
enum {UNDO_NONE=0,UNDO_EDIT,UNDO_INSSTR,UNDO_DELSTR};

Editor::Editor(ScreenObject *pOwner,bool DialogUsed)
{
  _KEYMACRO(SysLog("Editor::Editor()"));
  _KEYMACRO(SysLog(1));
  /* $ 26.02.2001 IS
       �������������� ���������� ����� ����� ;)
  */
  memcpy(&EdOpt, &Opt.EdOpt, sizeof(EditorOptions));
  /* IS $ */

  SetOwner (pOwner);

  if(DialogUsed)
    Flags.Set(FEDITOR_DIALOGMEMOEDIT);
  /* $ 26.10.2003 KM
     ���� ���������� ���������� ����� ������ 16-������ �����, �����
     ������������� GlobalSearchString � ������, ��� ��� �������� ������ �
     16-������ �������������.
  */
  if (GlobalSearchHex)
  {
    int LenSearchStr=sizeof(LastSearchStr);
    Transform(LastSearchStr,LenSearchStr,GlobalSearchString,'S');
  }
  else
    xstrncpy((char *)LastSearchStr,GlobalSearchString,sizeof(LastSearchStr)-1);
  /* KM $ */

  LastSearchCase=GlobalSearchCase;
  /* $ 03.08.2000 KM
     ���������� ��� ������ "Whole words"
  */
  LastSearchWholeWords=GlobalSearchWholeWords;
  /* KM $ */
  LastSearchReverse=GlobalSearchReverse;
  memcpy(&TableSet,&InitTableSet,sizeof(TableSet));
  UseDecodeTable=EditorInitUseDecodeTable;
  TableNum=EditorInitTableNum;
  AnsiText=EditorInitAnsiText;

  if (AnsiText && TableNum==0)
  {
    int UseUnicode=FALSE;
    GetTable(&TableSet,TRUE,TableNum,UseUnicode);
    UseDecodeTable=TRUE;
  }

  Pasting=0;
  NumLine=0;
  NumLastLine=0;
  LastChangeStrPos=0;
  BlockStart=NULL;
  BlockStartLine=0;

  AddString (NULL, 0);

  // $ 12.01.2002 IS - �� ��������� ����� ������ ��� ��� ����� ����� \r\n, ������� ������ ������� �����, �������� ��� ����.
  strcpy(GlobalEOL,DOS_EOL_fmt);

  /* $ 03.12.2001 IS ������ ������ undo ������ ����� �������� */
  UndoData=static_cast<EditorUndoData*>(xf_malloc(EdOpt.UndoSize*sizeof(EditorUndoData)));
  if(UndoData)
    memset(UndoData,0,EdOpt.UndoSize*sizeof(EditorUndoData));
  /* IS $ */
  UndoDataPos=0;
  StartLine=StartChar=-1;
  BlockUndo=FALSE;
  VBlockStart=NULL;
  memset(&SavePos,0xff,sizeof(SavePos));
  MaxRightPos=0;
  UndoSavePos=0;
  StackPos=0;
  NewStackPos=FALSE;

  Editor::EditorID=::EditorID++;
  Flags.Set(FEDITOR_OPENFAILED); // ��, ����. ���� �� ��� �� ������,
                                  // ��� ������ ������� ������� �������� ��������
  HostFileEditor=NULL;
}


Editor::~Editor()
{
  //_SVS(SysLog("[%p] Editor::~Editor()",this));
  FreeAllocatedData();

  KeepInitParameters();

  _KEYMACRO(SysLog(-1));
  _KEYMACRO(SysLog("Editor::~Editor()"));
}

void Editor::FreeAllocatedData()
{
  //_SVS(CleverSysLog Clev("Editor::FreeAllocatedData()"));
  //_SVS(DWORD I=0;SysLog("TopList=%p, EndList=%p _heapchk() = %d",TopList, EndList,_heapchk()));
  //_SVS(TRY)
  {
    while (EndList!=NULL)
    {
      Edit *m_prev=EndList->m_prev;
      delete EndList;
      EndList=m_prev;
     //_SVS(I++);
    }
  }
  //_SVS(EXCEPT(EXCEPTION_EXECUTE_HANDLER){SysLog("I=%d EndList=%p{%p,%p} _heapchk() = %d",I,EndList,EndList->m_next,EndList->m_prev,_heapchk());});

  //_SVS(SysLog("I=%d) _heapchk() = %d",I,_heapchk()));

  /* $ 03.12.2001 IS
     UndoData - ���������
  */
  if(UndoData)
  {
    for (int I=0;I<EdOpt.UndoSize;++I)
      if (UndoData[I].Type!=UNDO_NONE && UndoData[I].Str!=NULL)
        delete UndoData[I].Str;
    xf_free(UndoData);
    UndoData=NULL;
  }
  /* IS $ */

  ClearStackBookmarks();
}

void Editor::KeepInitParameters()
{
  /* $ 26.10.2003 KM
     ! �������������� GlobalSearchString � ������ ����������� 16-������� ������,
       � ����� ���� �� ������ �� ������ ��� ���� ������� ������ �� ����������
       ����� ������ � ���������.
  */
  // ���������� ���������� ����� ������ 16-������ ������?
  if (GlobalSearchHex)
  {
    // ��! ����� ��������, ���������� �� LastSearchStr � ��������� ������������� GlobalSearchString...
    char SearchStr[2*NM];
    int LenSearchStr=sizeof(SearchStr);
    Transform((unsigned char *)SearchStr,LenSearchStr,(char *)GlobalSearchString,'S');

    // LastSearchStr ���������� �� ���������� ������������� GlobalSearchString
    if (memcmp(LastSearchStr,SearchStr,LenSearchStr)!=0)
    {
      // ��! ����������, ������ ������������� ����� �� ���������, �������
      // ������������� ��� �������� � 16-������ �������������.
      int LenSearchStr=sizeof(GlobalSearchString);
      Transform((unsigned char *)GlobalSearchString,LenSearchStr,(char *)LastSearchStr,'X');
    }
  }
  else
    strcpy(GlobalSearchString,(char *)LastSearchStr);
  /* KM $ */

  GlobalSearchCase=LastSearchCase;
  /* $ 03.08.2000 KM
    ����� ���������� ��� ������ "Whole words"
  */
  GlobalSearchWholeWords=LastSearchWholeWords;
  /* KM $ */
  GlobalSearchReverse=LastSearchReverse;
  memcpy(&InitTableSet,&TableSet,sizeof(InitTableSet));
  EditorInitUseDecodeTable=UseDecodeTable;
  EditorInitTableNum=TableNum;
  EditorInitAnsiText=AnsiText;
}


// TODO: ����� ��� ����� ������ � FileEditor. !!!
int Editor::ReadFile(const char *Name,int &UserBreak)
{
  FILE *EditFile;
  int Count=0,LastLineCR=0,MessageShown=FALSE;

  UserBreak=0;
  Flags.Clear(FEDITOR_OPENFAILED);

/* Name ��� � ������ �������!!!
  if (ConvertNameToFull(Name,FileName, sizeof(FileName)) >= sizeof(FileName))
  {
    Flags.Set(FEDITOR_OPENFAILED);
    return FALSE;
  }
*/

  DWORD FileFlags=FILE_FLAG_SEQUENTIAL_SCAN;
  if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    FileFlags|=FILE_FLAG_POSIX_SEMANTICS;

  HANDLE hEdit=FAR_CreateFile(Name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FileFlags,NULL);

  if (hEdit==INVALID_HANDLE_VALUE && WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
    hEdit=FAR_CreateFile(Name,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);

  if (hEdit==INVALID_HANDLE_VALUE)
  {
    int LastError=GetLastError();
    SetLastError(LastError);
    if (LastError!=ERROR_FILE_NOT_FOUND && LastError!=ERROR_PATH_NOT_FOUND)
    {
      UserBreak=-1;
      Flags.Set(FEDITOR_OPENFAILED);
    }
    return(FALSE);
  }

  int EditHandle=_open_osfhandle((intptr_t)hEdit,O_BINARY);
  if (EditHandle==-1)
  {
    CloseHandle(hEdit);
    return(FALSE);
  }

  if ((EditFile=fdopen(EditHandle,"rb"))==NULL)
  {
    _close(EditHandle);
    return(FALSE);
  }

  if (GetFileType(hEdit)!=FILE_TYPE_DISK)
  {
    fclose(EditFile);
    SetLastError(ERROR_INVALID_NAME);
    UserBreak=-1;
    Flags.Set(FEDITOR_OPENFAILED);
    return(FALSE);
  }

  /* $ 29.11.2000 SVS
   + �������� �� ���������� ���������� ������ �����, �����
     �������� ����� ����� ������ � ���������������� �������� ���������
     ����� �� ��������������
  */
  if(EdOpt.FileSizeLimitLo || EdOpt.FileSizeLimitHi)
  {
    unsigned __int64 RealSizeFile;
    if ( FAR_GetFileSize(hEdit, &RealSizeFile) )
    {
      unsigned __int64 NeedSizeFile=MKUINT64(EdOpt.FileSizeLimitHi,EdOpt.FileSizeLimitLo);
      if(RealSizeFile > NeedSizeFile)
      {
        char TempBuf[2][128];
        char TempBuf2[2][64];
        // ������ = 8 - ��� �����... � Kb � ����...
        FileSizeToStr(TempBuf2[0],RealSizeFile,8);
        FileSizeToStr(TempBuf2[1],NeedSizeFile,8);
        sprintf(TempBuf[0],MSG(MEditFileLong),RemoveExternalSpaces(TempBuf2[0]));
        sprintf(TempBuf[1],MSG(MEditFileLong2),RemoveExternalSpaces(TempBuf2[1]));
        if(Message(MSG_WARNING,2,MSG(MEditTitle),
                    Name,
                    TempBuf[0],
                    TempBuf[1],
                    MSG(MEditROOpen),
                    MSG(MYes),MSG(MNo)))
        {
          fclose(EditFile);
          SetLastError(ERROR_OPEN_FAILED);
          UserBreak=1;
          Flags.Set(FEDITOR_OPENFAILED);
          return(FALSE);
        }
      }
    }
  }

  // $ 29.11.2000 SVS - ���� ���� ����� ������� ReadOnly ��� System ��� Hidden, �� ����� ����� ���� - ����������� ����������.
  // $ 03.12.2000 SVS - System ��� Hidden - �������� ��������
  // $ 15.12.2000 SVS - �������� ������� ��������� ��, ��� ������� GetFileAttributes() :-)
  {
    DWORD FileAttributes=HostFileEditor?HostFileEditor->GetFileAttributes(Name):(DWORD)-1;
    if((EdOpt.ReadOnlyLock&1) &&
       FileAttributes != -1 &&
       (FileAttributes &
          (FILE_ATTRIBUTE_READONLY|
             /* Hidden=0x2 System=0x4 - ������������� �� 2-� ���������,
                ������� ��������� ����� 0110.0000 �
                �������� �� ���� ����� => 0000.0110 � ��������
                �� ����� ������ ��������  */
             ((EdOpt.ReadOnlyLock&0x60)>>4)
          )
       )
     )
      Flags.Swap(FEDITOR_LOCKMODE);
  }

  // Mantis#0000516: ���������� ������� � ���������...
  // ... �������� ��������� �����, ����� ���� ������� � ����������� �������.
  bool CursorShow=true;
  int __Visible, __Size;
  GetCursorType(__Visible, __Size);

  {
    ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
    GetFileString GetStr(EditFile);
    //SaveScreen SaveScr;
    NumLastLine=0;
    *GlobalEOL=0;
    char *Str;
    int StrLength,GetCode;

    clock_t StartTime=clock();

    if (EdOpt.AutoDetectTable)
    {
      UseDecodeTable=DetectTable(EditFile,&TableSet,TableNum);
      AnsiText=FALSE;
    }

    while ((GetCode=GetStr.GetString(&Str,StrLength))!=0)
    {
      if (GetCode==-1)
      {
        fclose(EditFile);
        SetPreRedrawFunc(NULL);
        return(FALSE);
      }
      LastLineCR=0;

      if ((++Count & 0xfff)==0 && clock()-StartTime>500)
      {
        if (CheckForEscSilent())
        {
          if ( ConfirmAbortOp() )
          {
            UserBreak = 1;
            fclose(EditFile);
            SetPreRedrawFunc(NULL);
            return FALSE;
          }
          MessageShown=FALSE;
        }
        if (!MessageShown)
        {
          CursorShow=false;

          SetCursorType(FALSE,0);
          SetPreRedrawFunc(Editor::PR_EditorShowMsg);
          EditorShowMsg(MSG(MEditTitle),MSG(MEditReading),Name);
          MessageShown=TRUE;
        }
      }

      char *CurEOL;
      int Offset = StrLength > 3 ? StrLength - 3 : 0;
      if (!LastLineCR &&
          (
            (CurEOL=(char *)memchr(Str+Offset,'\r',StrLength-Offset))!=NULL ||
            (CurEOL=(char *)memchr(Str+Offset,'\n',StrLength-Offset))!=NULL
          )
         )
      {
        xstrncpy(GlobalEOL,CurEOL,sizeof(GlobalEOL)-1);
        GlobalEOL[sizeof(GlobalEOL)-1]=0;
        LastLineCR=1;
      }

      if(!AddString (Str, StrLength))
      {
        fclose(EditFile);
        SetPreRedrawFunc(NULL);
        return(FALSE);
      }

    }
    SetPreRedrawFunc(NULL);

    if (LastLineCR)
      AddString ("", 0);
  }
  if (NumLine>0)
    NumLastLine--;
  if (NumLastLine==0)
    NumLastLine=1;
  fclose(EditFile);

  if(!CursorShow)
    SetCursorType(__Visible, __Size);

  if (StartLine==-2)
  {
    Edit *CurPtr=TopList;
    long TotalSize=0;
    while (CurPtr!=NULL && CurPtr->m_next!=NULL)
    {
      const char *SaveStr,*EndSeq;
      int Length;
      CurPtr->GetBinaryString(&SaveStr,&EndSeq,Length);
      TotalSize+=Length+(int)strlen(EndSeq);
      if (TotalSize>StartChar)
        break;
      CurPtr=CurPtr->m_next;
      NumLine++;
    }
    TopScreen=CurLine=CurPtr;
    if (EdOpt.SavePos && CtrlObject!=NULL)
    {
      unsigned int Line,ScreenLine,LinePos,LeftPos=0;
      unsigned int Table=0;

      {
        char *ptrPluginData=NULL;
        if (HostFileEditor && *HostFileEditor->GetPluginData())
          ptrPluginData=HostFileEditor->GetPluginData();

        char *CacheName=(char *)alloca((ptrPluginData?strlen(ptrPluginData):0)+strlen(Name)+NM);

        if (ptrPluginData)
        {
          strcpy(CacheName,ptrPluginData);
          strcat(CacheName,PointToName(Name));
        }
        else
        {
          strcpy(CacheName,Name);
          for(int i=0;CacheName[i];i++)
          {
            if(CacheName[i]=='/')
              CacheName[i]='\\';
          }
        }

        {
          struct TPosCache32 PosCache={0};
          if(Opt.EdOpt.SaveShortPos)
          {
            PosCache.Position[0]=SavePos.Line;
            PosCache.Position[1]=SavePos.Cursor;
            PosCache.Position[2]=SavePos.ScreenLine;
            PosCache.Position[3]=SavePos.LeftPos;
          }
          CtrlObject->EditorPosCache->GetPosition(CacheName,&PosCache);
          Line=PosCache.Param[0];
          ScreenLine=PosCache.Param[1];
          LinePos=PosCache.Param[2];
          LeftPos=PosCache.Param[3];
          Table=PosCache.Param[4];
        }
      }

      //_D(SysLog("after Get cache, LeftPos=%i",LeftPos));
      if((int)Line < 0) Line=0;
      if((int)ScreenLine < 0) ScreenLine=0;
      if((int)LinePos < 0) LinePos=0;
      if((int)LeftPos < 0) LeftPos=0;
      if((int)Table < 0) Table=0;

      Flags.Change(FEDITOR_TABLECHANGEDBYUSER,(Table!=0));

      _SVS(SysLog("[%d] Table=%d",__LINE__,Table));
      switch(Table)
      {
        case 0:
          break;
        case 1:
          AnsiText=UseDecodeTable=0;
          break;
        case 2:
          {
            AnsiText=TRUE;
            UseDecodeTable=TRUE;
            TableNum=0;
            int UseUnicode=FALSE;
            GetTable(&TableSet,TRUE,TableNum,UseUnicode);
          }
          break;
        default:
          AnsiText=0;
          UseDecodeTable=1;
          TableNum=Table-2;
          PrepareTable(&TableSet,Table-3);
          break;
      }

      if (NumLine==Line-ScreenLine)
      {
        Lock ();
        for (unsigned int I=0;I<ScreenLine;I++)
          ProcessKey(KEY_DOWN);
        CurLine->SetTabCurPos(LinePos);
        Unlock ();
      }
      //_D(SysLog("Setleftpos to %i",LeftPos));
      CurLine->SetLeftPos(LeftPos);
    }
  }
  else
  {
    if (StartLine!=-1 || EdOpt.SavePos && CtrlObject!=NULL)
    {
      unsigned int Line,ScreenLine,LinePos,LeftPos=0;
      if (StartLine!=-1)
      {
        Line=StartLine-1;
        ScreenLine=ObjHeight/2; //ScrY
        if (ScreenLine>Line)
          ScreenLine=Line;
        LinePos=(StartChar>0) ? StartChar-1:0;
      }
      else
      {
        unsigned int Table=0;

        {
          char *ptrPluginData=NULL;
          if (HostFileEditor && *HostFileEditor->GetPluginData())
            ptrPluginData=HostFileEditor->GetPluginData();

          char *CacheName=(char *)alloca((ptrPluginData?strlen(ptrPluginData):0)+strlen(Name)+NM);

          if (ptrPluginData)
          {
            strcpy(CacheName,ptrPluginData);
            strcat(CacheName,PointToName(Name));
          }
          else
          {
            strcpy(CacheName,Name);
            for(int i=0;CacheName[i];i++)
            {
              if(CacheName[i]=='/')CacheName[i]='\\';
            }
          }

          {
            struct TPosCache32 PosCache={0};
            if(Opt.EdOpt.SaveShortPos)
            {
              PosCache.Position[0]=SavePos.Line;
              PosCache.Position[1]=SavePos.Cursor;
              PosCache.Position[2]=SavePos.ScreenLine;
              PosCache.Position[3]=SavePos.LeftPos;
            }
            CtrlObject->EditorPosCache->GetPosition(CacheName,&PosCache);
            Line=PosCache.Param[0];
            ScreenLine=PosCache.Param[1];
            LinePos=PosCache.Param[2];
            LeftPos=PosCache.Param[3];
            Table=PosCache.Param[4];
          }
        }

        //_D(SysLog("after Get cache 2, LeftPos=%i",LeftPos));
        if((int)Line < 0) Line=0;
        if((int)ScreenLine < 0) ScreenLine=0;
        if((int)LinePos < 0) LinePos=0;
        if((int)LeftPos < 0) LeftPos=0;
        if((int)Table < 0) Table=0;

        Flags.Change(FEDITOR_TABLECHANGEDBYUSER,(Table!=0));

        _SVS(SysLog("[%d] Table=%d",__LINE__,Table));
        switch(Table)
        {
          case 0:
            break;
          case 1:
            AnsiText=UseDecodeTable=0;
            break;
          case 2:
            {
              AnsiText=TRUE;
              UseDecodeTable=TRUE;
              TableNum=0;
              int UseUnicode=FALSE;
              GetTable(&TableSet,TRUE,TableNum,UseUnicode);
            }
            break;
          default:
            AnsiText=0;
            UseDecodeTable=1;
            TableNum=Table-2;
            PrepareTable(&TableSet,Table-3);
            break;
        }

      }
      if (ScreenLine>static_cast<DWORD>(ObjHeight))//ScrY
        ScreenLine=ObjHeight;//ScrY;
      if (Line>=ScreenLine)
      {
        Lock ();
        GoToLine(Line-ScreenLine);
        TopScreen=CurLine;
        for (DWORD I=0;I<ScreenLine;I++)
          ProcessKey(KEY_DOWN);
        CurLine->SetTabCurPos(LinePos);
        //_D(SysLog("Setleftpos 2 to %i",LeftPos));
        CurLine->SetLeftPos(LeftPos);
        Unlock ();
      }
    }
  }
  _SVS(SysLog("[%d] UseDecodeTable=%d",UseDecodeTable));

  if (UseDecodeTable)
    for (Edit *CurPtr=TopList;CurPtr!=NULL;CurPtr=CurPtr->m_next)
      CurPtr->SetTables(&TableSet);
  /* $ 25.07.2001 IS
       ������������� ������� ����� ������� ��������, �.�. ������� ������
       �������� �� ������������ (UseDecodeTable==FALSE)
  */
  else
    TableNum=0;
  /* IS $ */

  //CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
  //_D(SysLog("%08d EE_READ",__LINE__));
  //CtrlObject->Plugins.ProcessEditorEvent(EE_READ,NULL);
  //_SVS(SysLog("Editor::ReadFile _heapchk() = %d",_heapchk()));
  return(TRUE);
}

// �������������� �� ������ � ������ (������ � OEM!)
int Editor::ReadData(LPCSTR SrcBuf,int SizeSrcBuf)
{
#if defined(PROJECT_DI_MEMOEDIT)
  int Count=0,LastLineCR=0;

  {
    NumLastLine=0;
    *GlobalEOL=0;
    char *Str;
    int StrLength,GetCode;

    while ((GetCode=GetStr.GetString(&Str,StrLength))!=0)
    {
      if (GetCode==-1)
      {
        return(FALSE);
      }
      LastLineCR=0;

      char *CurEOL;
      int Offset = StrLength > 3 ? StrLength - 3 : 0;
      if (!LastLineCR && ((CurEOL=(char *)memchr(Str+Offset,'\r',StrLength-Offset))!=NULL ||
          (CurEOL=(char *)memchr(Str+Offset,'\n',StrLength-Offset))!=NULL))
      {
        xstrncpy(GlobalEOL,CurEOL,sizeof(GlobalEOL)-1);
        GlobalEOL[sizeof(GlobalEOL)-1]=0;
        LastLineCR=1;
      }

      AddString (Str, StrLength);
    }

    if (LastLineCR)
      AddString (NULL,0);
  }

  if (NumLine>0)
    NumLastLine--;

  if (NumLastLine==0)
    NumLastLine=1;

  AnsiText=TableNum=0;

#endif
  return(TRUE);
}

/*
  Editor::SaveData - �������������� �� ������ � �����

    DestBuf     - ���� ��������� (���������� �����������!)
    SizeDestBuf - ������ ����������
    TextFormat  - ��� �������� �����
*/
int Editor::SaveData(char **DestBuf,int& SizeDestBuf,int TextFormat)
{
#if defined(PROJECT_DI_MEMOEDIT)
  char *PDest=NULL;
  SizeDestBuf=0; // ����� ������ = 0

  // ���������� EOL
  switch(TextFormat)
  {
    case 1:
      strcpy(GlobalEOL,DOS_EOL_fmt);
      break;
    case 2:
      strcpy(GlobalEOL,UNIX_EOL_fmt);
      break;
    case 3:
      strcpy(GlobalEOL,MAC_EOL_fmt);
      break;
    case 4:
      strcpy(GlobalEOL,WIN_EOL_fmt);
      break;
  }

  int StrLength=0;
  const char *SaveStr, *EndSeq;
  int Length;

  // ��������� ���������� ����� � ����� ������ ������ (����� �� ������� realloc)
  Edit *CurPtr=TopList;

  DWORD AllLength=0;
  while (CurPtr!=NULL)
  {
    CurPtr->GetBinaryString(SaveStr,&EndSeq,Length);
    // ���������� �������� �����
    if (*EndSeq==0 && CurPtr->m_next!=NULL)
      EndSeq=*GlobalEOL ? GlobalEOL:DOS_EOL_fmt;

    if (TextFormat!=0 && *EndSeq!=0)
    {
      if (TextFormat==1)
        EndSeq=DOS_EOL_fmt;
      else if (TextFormat==2)
        EndSeq=UNIX_EOL_fmt;
      else if (TextFormat==3)
        EndSeq=MAC_EOL_fmt;
      else
        EndSeq=WIN_EOL_fmt;

      CurPtr->SetEOL(EndSeq);
    }
    AllLength+=Length+strlen(EndSeq)+16;
  }

  char *MemEditStr=(char *)malloc(sizeof(char) * AllLength);

  if(MemEditStr)
  {
    *MemEditStr=0;
    PDest=MemEditStr;
    // �������� �� ������ �����
    CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->GetBinaryString(SaveStr,&EndSeq,Length);

      strcpy(PDest,SaveStr);
      strcat(PDest,EndSeq);
      PDest+=strlen(PDest);

      CurPtr=CurPtr->m_next;
    }
    SizeDestBuf=strlen(MemEditStr);
    DestBuf=&MemEditStr;
    return TRUE;
  }
  else
    return FALSE;
#else
  return TRUE;
#endif
}


void Editor::DisplayObject()
{
  ShowEditor(FALSE);
}


void Editor::ShowEditor(int CurLineOnly)
{
  if ( Locked () )
    return;
  Edit *CurPtr;
  int LeftPos,CurPos,Y;

//_SVS(SysLog("Enter to ShowEditor, CurLineOnly=%i",CurLineOnly));
  /*$ 10.08.2000 skv
    To make sure that CurEditor is set to required value.
  */
  if(!Flags.Check(FEDITOR_DIALOGMEMOEDIT))
    CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
  /* skv$*/

  /* 17.04.2002 skv
    ��� � ������ �� ����� ��� Alt-F9 � ����� �������� �����.
    ���� �� ������ ���� ��������� �����, � ���� ����� ������,
    �����������������.
  */

  if(!EdOpt.AllowEmptySpaceAfterEof)
  {

    while(CalcDistance(TopScreen,NULL,Y2-Y1)<Y2-Y1)
    {
      if(TopScreen->m_prev)
      {
        TopScreen=TopScreen->m_prev;
      }
      else
      {
        break;
      }
    }
  }
  /*
    ���� ������ ����� �������� "�� �������",
    �������� ����� ��� ������, � ��
    ������ ������� � �����.
  */

  while (CalcDistance(TopScreen,CurLine,-1)>=Y2-Y1+1)
  {
    TopScreen=TopScreen->m_next;
    //DisableOut=TRUE;
    //ProcessKey(KEY_UP);
    //DisableOut=FALSE;
  }


  /* skv $ */

  CurPos=CurLine->GetTabCurPos();
  if (!EdOpt.CursorBeyondEOL)
  {
    MaxRightPos=CurPos;
    //CurLine->SetPosition(X1,0,X2,0);
    int RealCurPos=CurLine->GetCurPos();
    int Length=CurLine->GetLength();

    if (RealCurPos>Length)
    {
      CurLine->SetCurPos(Length);
      CurLine->SetLeftPos(0);
      //_D(SysLog("call CurLine->FastShow()"));
      //CurLine->FastShow();
      CurPos=CurLine->GetTabCurPos();
    }
  }

  if (!Pasting)
  {
    /*$ 10.08.2000 skv
      Don't send EE_REDRAW while macro is being executed.
      Send EE_REDRAW with param=2 if text was just modified.

    */
    _SYS_EE_REDRAW(CleverSysLog Clev("Editor::ShowEditor()"));
    if(!ScrBuf.GetLockCount())
    {
      /*$ 13.09.2000 skv
        EE_REDRAW 1 and 2 replaced.
      */
      if(Flags.Check(FEDITOR_JUSTMODIFIED))
      {
        Flags.Clear(FEDITOR_JUSTMODIFIED);
        if(!Flags.Check(FEDITOR_DIALOGMEMOEDIT))
        {
          _SYS_EE_REDRAW(SysLog("Call ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE)"));
          CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_CHANGE);
        }
      }
      else
      {
        if(!Flags.Check(FEDITOR_DIALOGMEMOEDIT))
        {
          _SYS_EE_REDRAW(SysLog("Call ProcessEditorEvent(EE_REDRAW,%s)",(CurLineOnly?"EEREDRAW_LINE":"EEREDRAW_ALL")));
          CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,CurLineOnly?EEREDRAW_LINE:EEREDRAW_ALL);
        }
      }
      /* skv$*/
    }
    _SYS_EE_REDRAW(else SysLog("ScrBuf Locked !!!"));
    /* skv$*/
  }

  if (!CurLineOnly)
  {
    LeftPos=CurLine->GetLeftPos();
#if 1
    // ������ ����������������� �����!
    if(CurPos+LeftPos < X2 )
      LeftPos=0;
    else if(CurLine->X2 < X2)
      LeftPos=CurLine->GetLength()-CurPos;
    if(LeftPos < 0)
      LeftPos=0;
#endif

    for (CurPtr=TopScreen,Y=Y1;Y<=Y2;Y++)
      if (CurPtr!=NULL)
      {
        CurPtr->SetEditBeyondEnd(TRUE);
        CurPtr->SetPosition(X1,Y,X2,Y);
        CurPtr->SetTables(UseDecodeTable ? &TableSet:NULL);
        CurPtr->SetLeftPos(LeftPos);
        CurPtr->SetTabCurPos(CurPos);
        CurPtr->FastShow();
        CurPtr->SetEditBeyondEnd(EdOpt.CursorBeyondEOL);
        CurPtr=CurPtr->m_next;
      }
      else
      {
        //GotoXY(X1,Y);
        //SetColor(COL_EDITORTEXT);
        //mprintf("%*s",ObjWidth,"");
        SetScreen(X1,Y,X2,Y,' ',COL_EDITORTEXT); //??
      }
  }

  CurLine->SetOvertypeMode(Flags.Check(FEDITOR_OVERTYPE));
  CurLine->Show();

  // ��������� ������������� �����
  if (VBlockStart!=NULL && VBlockSizeX>0 && VBlockSizeY>0)
  {
    int CurScreenLine=NumLine-CalcDistance(TopScreen,CurLine,-1);
    LeftPos=CurLine->GetLeftPos();

    for (CurPtr=TopScreen,Y=Y1;Y<=Y2;Y++)
    {
      if (CurPtr!=NULL)
      {
        if (CurScreenLine>=VBlockY && CurScreenLine<VBlockY+VBlockSizeY)
        {
          int BlockX1=VBlockX-LeftPos+X1;
          int BlockX2=VBlockX+VBlockSizeX-1-LeftPos+X1;
          if (BlockX1<X1)
            BlockX1=X1;
          if (BlockX2>X2)
            BlockX2=X2;
          if (BlockX1<=X2 && BlockX2>=X1)
            ChangeBlockColor(BlockX1,Y,BlockX2,Y,COL_EDITORSELECTEDTEXT);
        }
        CurPtr=CurPtr->m_next;
        CurScreenLine++;
      }
    }
  }

  if(HostFileEditor) HostFileEditor->ShowStatus();
//_SVS(SysLog("Exit from ShowEditor"));
}


/*$ 10.08.2000 skv
  Wrapper for Modified.
  Set JustModified every call to 1
  to track any text state change.
  Even if state==0, this can be
  last UNDO.
*/
void Editor::TextChanged(int State)
{
  Flags.Change(FEDITOR_MODIFIED,State);
  Flags.Set(FEDITOR_JUSTMODIFIED);
}
/* skv$*/


__int64 Editor::VMProcess(int OpCode,void *vParam,__int64 iParam)
{
  int CurPos=CurLine->GetCurPos();
  switch(OpCode)
  {
    case MCODE_C_EMPTY:
      return (__int64)(!CurLine->m_next && !CurLine->m_prev); //??
    case MCODE_C_EOF:
      return (__int64)(!CurLine->m_next && CurPos>=CurLine->GetLength());
    case MCODE_C_BOF:
      return (__int64)(!CurLine->m_prev && CurPos==0);
    case MCODE_C_SELECTED:
      return (__int64)(BlockStart || VBlockStart?TRUE:FALSE);
    case MCODE_V_EDITORCURPOS:
      return (__int64)(CurLine->GetTabCurPos()+1);
    case MCODE_V_EDITORREALPOS:
      return (__int64)(CurLine->GetCurPos()+1);
    case MCODE_V_EDITORCURLINE:
      return (__int64)(NumLine+1);
    case MCODE_V_ITEMCOUNT:
    case MCODE_V_EDITORLINES:
      return (__int64)NumLastLine;

    // ������ �� ��������� ����������
    case MCODE_F_BM_ADD:
      return AddStackBookmark();
    case MCODE_F_BM_CLEAR:
      return ClearStackBookmarks();
    case MCODE_F_BM_NEXT:
      return NextStackBookmark();
    case MCODE_F_BM_PREV:
      return PrevStackBookmark();
    case MCODE_F_BM_STAT:
      // ���� ���, �.�. BM.Stat(0) ���������� ����������
      return GetStackBookmarks(NULL);

    case MCODE_F_BM_GET:                   // N=BM.Get(Idx,M) - ���������� ���������� ������ (M==0) ��� ������� (M==1) �������� � �������� (Idx=1...)
    {
      __int64 Ret=_i64(-1);
      long Val[1];
      EditorBookMarks ebm={0};
      int iMode=(int)((INT_PTR)vParam);
      switch(iMode)
      {
        case 0: ebm.Line=Val;  break;
        case 1: ebm.Cursor=Val; break;
        case 2: ebm.LeftPos=Val; break;
        case 3: ebm.ScreenLine=Val; break;
        default: iMode=-1; break;
      }

      if(iMode >= 0 && GetStackBookmark((int)iParam-1,&ebm))
        Ret=(__int64)((DWORD)Val[0]+1);
      return Ret;
    }

    case MCODE_F_BM_DEL:                   // N=BM.Del(Idx) - ������� �������� � ��������� �������� (x=1...), 0 - ������� ������� ��������
      return DeleteStackBookmark(PointerToStackBookmark((int)iParam-1));
  }
  return _i64(0);
}


int Editor::ProcessKey(int Key)
{
  if (Key==KEY_IDLE)
  {
    if (Opt.ViewerEditorClock && HostFileEditor!=NULL && HostFileEditor->IsFullScreen())
      ShowTime(FALSE);
    return(TRUE);
  }

  if (Key==KEY_NONE)
    return(TRUE);

  _KEYMACRO(CleverSysLog SL("Editor::ProcessKey()"));
  _KEYMACRO(SysLog("Key=%s",_FARKEY_ToName(Key)));

  int CurPos,CurVisPos,I;
  CurPos=CurLine->GetCurPos();
  CurVisPos=GetLineCurPos();

  int isk=IsShiftKey(Key);
  _SVS(SysLog("[%d] isk=%d",__LINE__,isk));
  //if ((!isk || CtrlObject->Macro.IsExecuting()) && !isk && !Pasting)
//  if (!isk && !Pasting && !(Key >= KEY_MACRO_BASE && Key <= KEY_MACRO_ENDBASE))
  if (!isk && !Pasting && !(Key >= KEY_MACRO_BASE && Key <= KEY_MACRO_ENDBASE || Key>=KEY_OP_BASE && Key <=KEY_OP_ENDBASE))
  {
    _SVS(SysLog("[%d] BlockStart=(%d,%d)",__LINE__,BlockStart,VBlockStart));
    if (BlockStart!=NULL || VBlockStart!=NULL)
    {
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
    }
    if ((BlockStart!=NULL || VBlockStart!=NULL) && !EdOpt.PersistentBlocks)
//    if (BlockStart!=NULL || VBlockStart!=NULL && !EdOpt.PersistentBlocks)
    {
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      if (!EdOpt.PersistentBlocks)
      {
        static int UnmarkKeys[]={
           KEY_LEFT,      KEY_NUMPAD4,
           KEY_RIGHT,     KEY_NUMPAD6,
           KEY_HOME,      KEY_NUMPAD7,
           KEY_END,       KEY_NUMPAD1,
           KEY_UP,        KEY_NUMPAD8,
           KEY_DOWN,      KEY_NUMPAD2,
           KEY_PGUP,      KEY_NUMPAD9,
           KEY_PGDN,      KEY_NUMPAD3,
           KEY_CTRLHOME,  KEY_CTRLNUMPAD7,
           KEY_CTRLPGUP,  KEY_CTRLNUMPAD9,
           KEY_CTRLEND,   KEY_CTRLNUMPAD1,
           KEY_CTRLPGDN,  KEY_CTRLNUMPAD3,
           KEY_CTRLLEFT,  KEY_CTRLNUMPAD4,
           KEY_CTRLRIGHT, KEY_CTRLNUMPAD7,
           KEY_CTRLUP,    KEY_CTRLNUMPAD8,
           KEY_CTRLDOWN,  KEY_CTRLNUMPAD2,
           KEY_CTRLN,
           KEY_CTRLE,
           KEY_CTRLS,
        };
        for (int I=0;I<sizeof(UnmarkKeys)/sizeof(UnmarkKeys[0]);I++)
          if (Key==UnmarkKeys[I])
          {
            UnmarkBlock();
            break;
          }
      }
      else
      {
        int StartSel,EndSel;
//        Edit *BStart=!BlockStart?VBlockStart:BlockStart;
//        BStart->GetRealSelection(StartSel,EndSel);
        BlockStart->GetRealSelection(StartSel,EndSel);
        _SVS(SysLog("[%d] PersistentBlocks! StartSel=%d, EndSel=%d",__LINE__,StartSel,EndSel));
        if (StartSel==-1 || StartSel==EndSel)
          UnmarkBlock();
      }
    }
  }

  if (Key==KEY_ALTD)
    Key=KEY_CTRLK;

  // ������ � ����������
  if (Key>=KEY_CTRL0 && Key<=KEY_CTRL9)
    return GotoBookmark(Key-KEY_CTRL0);
  if (Key>=KEY_CTRLSHIFT0 && Key<=KEY_CTRLSHIFT9)
    Key=Key-KEY_CTRLSHIFT0+KEY_RCTRL0;
  if (Key>=KEY_RCTRL0 && Key<=KEY_RCTRL9)
    return SetBookmark(Key-KEY_RCTRL0);

  int SelStart=0,SelEnd=0;
  int SelFirst=FALSE;
  int SelAtBeginning=FALSE;

  /* $ 05.11.2003 SKV

  */
  EditorBlockGuard _bg(*this,&Editor::UnmarkEmptyBlock);
  /* SKV $ */

  switch(Key)
  {
    case KEY_SHIFTLEFT:    case KEY_SHIFTRIGHT:
    case KEY_SHIFTUP:      case KEY_SHIFTDOWN:
    case KEY_SHIFTHOME:    case KEY_SHIFTEND:
    case KEY_SHIFTNUMPAD4: case KEY_SHIFTNUMPAD6:
    case KEY_SHIFTNUMPAD8: case KEY_SHIFTNUMPAD2:
    case KEY_SHIFTNUMPAD7: case KEY_SHIFTNUMPAD1:
    case KEY_CTRLSHIFTLEFT:  case KEY_CTRLSHIFTNUMPAD4:   /* 12.11.2002 DJ */
    {
      _KEYMACRO(CleverSysLog SL("Editor::ProcessKey(KEY_SHIFT*)"));
      _SVS(SysLog("[%d] SelStart=%d, SelEnd=%d",__LINE__,SelStart,SelEnd));
      UnmarkEmptyBlock(); // ������ ���������, ���� ��� ������ ����� 0
      _bg.needCheckUnmark=true;
      CurLine->GetRealSelection(SelStart,SelEnd);
      if(Flags.Check(FEDITOR_CURPOSCHANGEDBYPLUGIN))
      {
        if(SelStart!=-1 && (CurPos<SelStart || // ���� ������ �� ���������
           (SelEnd!=-1 && (CurPos>SelEnd ||    // ... ����� ���������
            (CurPos>SelStart && CurPos<SelEnd)))) &&
           CurPos<CurLine->GetLength()) // ... ������ ��������
          Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
        Flags.Clear(FEDITOR_CURPOSCHANGEDBYPLUGIN);
      }

      _SVS(SysLog("[%d] SelStart=%d, SelEnd=%d",__LINE__,SelStart,SelEnd));
      if (!Flags.Check(FEDITOR_MARKINGBLOCK))
      /* IS $ */
      {
        UnmarkBlock();
        Flags.Set(FEDITOR_MARKINGBLOCK);
        BlockStart=CurLine;
        BlockStartLine=NumLine;
        SelFirst=TRUE;
        SelStart=SelEnd=CurPos;
      }
      else
      {
        SelAtBeginning=CurLine==BlockStart && CurPos==SelStart;
        if(SelStart==-1)
        {
          SelStart=SelEnd=CurPos;
        }
      }
      _SVS(SysLog("[%d] SelStart=%d, SelEnd=%d",__LINE__,SelStart,SelEnd));
    }
  }

  switch(Key)
  {
    case KEY_CTRLSHIFTPGUP:   case KEY_CTRLSHIFTNUMPAD9:
    case KEY_CTRLSHIFTHOME:   case KEY_CTRLSHIFTNUMPAD7:
    {
      Lock ();
      Pasting++;
      while (CurLine!=TopList)
      {
        ProcessKey(KEY_SHIFTPGUP);
      }

      if(Key == KEY_CTRLSHIFTHOME || Key == KEY_CTRLSHIFTNUMPAD7)
        ProcessKey(KEY_SHIFTHOME);

      Pasting--;
      Unlock ();

      Show();
      return(TRUE);
    }

    case KEY_CTRLSHIFTPGDN:   case KEY_CTRLSHIFTNUMPAD3:
    case KEY_CTRLSHIFTEND:    case KEY_CTRLSHIFTNUMPAD1:
    {
      Lock ();
      Pasting++;
      while (CurLine!=EndList)
      {
        ProcessKey(KEY_SHIFTPGDN);
      }
      /* $ 06.02.2002 IS
         ������������� ������� ���� ����, ��� ������� �������� ��������.
         ��� ����:
           ��� ���������� "ProcessKey(KEY_SHIFTPGDN)" (��. ���� ����)
           ������� ������� (� ���� ������ - �������) ����� �������
           ECTL_SETPOSITION, � ���������� ���� ������������ ����
           FEDITOR_CURPOSCHANGEDBYPLUGIN. � ��� ��������� KEY_SHIFTEND
           ��������� � �������� ������ ���������� � ����, ��� ������ �� ���
           ���������� ���������� KEY_SHIFTPGDN.
      */
      Flags.Clear(FEDITOR_CURPOSCHANGEDBYPLUGIN);

      if(Key == KEY_CTRLSHIFTEND || Key == KEY_CTRLSHIFTNUMPAD1)
        ProcessKey(KEY_SHIFTEND);

      Unlock ();
      Pasting--;

      Show();
      return(TRUE);
    }

    case KEY_SHIFTPGUP:       case KEY_SHIFTNUMPAD9:
    {
      Pasting++;
      Lock ();
      for (I=Y1;I<Y2;I++)
      {
        ProcessKey(KEY_SHIFTUP);
        if(!EdOpt.CursorBeyondEOL)
        {
          if(CurLine->GetCurPos()>CurLine->GetLength())
          {
            CurLine->SetCurPos(CurLine->GetLength());
          }
        }
      }
      Pasting--;
      Unlock ();

      Show();
      return(TRUE);
    }

    case KEY_SHIFTPGDN:       case KEY_SHIFTNUMPAD3:
    {
      Pasting++;
      Lock ();
      for (I=Y1;I<Y2;I++)
      {
        ProcessKey(KEY_SHIFTDOWN);
        if(!EdOpt.CursorBeyondEOL)
        {
          if(CurLine->GetCurPos()>CurLine->GetLength())
          {
            CurLine->SetCurPos(CurLine->GetLength());
          }
        }
      }
      Pasting--;
      Unlock ();

      Show();
      return(TRUE);
    }

    case KEY_SHIFTHOME:       case KEY_SHIFTNUMPAD7:
    {
      Pasting++;
      Lock ();
      if(SelAtBeginning)
      {
        CurLine->Select(0,SelEnd);
      }
      else
      {
        if(SelStart==0)
        {
          CurLine->Select(-1,0);
        }
        else
        {
          CurLine->Select(0,SelStart);
        }
      }
      ProcessKey(KEY_HOME);
      Pasting--;
      Unlock ();

      Show();
      return(TRUE);
    }

    case KEY_SHIFTEND:    case KEY_SHIFTNUMPAD1:
    {
      {
        int LeftPos=CurLine->GetLeftPos();
        Pasting++;
        Lock ();

        int CurLength=CurLine->GetLength();

        if(!SelAtBeginning || SelFirst)
        {
          CurLine->Select(SelStart,CurLength);
        }else
        {
          if(SelEnd!=-1)
            CurLine->Select(SelEnd,CurLength);
          else
            CurLine->Select(CurLength,-1);
        }

        /* $ 17.01.2002 SKV
          ��� ��� �� ��� FastShow LeftPos �� ���������� � ����� ������.
        */
        CurLine->ObjWidth=X2-X1;
        /* SKV$*/

        ProcessKey(KEY_END);

        Pasting--;
        Unlock ();


        /* $ 13.9.2001 SKV
          ������ LeftPos ���������� ������ � FastShow :-\
        */
        if(EdOpt.PersistentBlocks)
          Show();
        else
        {
          CurLine->FastShow();
          ShowEditor(LeftPos==CurLine->GetLeftPos());
        }
        /* SKV$*/
      }
      return(TRUE);
    }

    case KEY_SHIFTLEFT:  case KEY_SHIFTNUMPAD4:
    {
      _SVS(CleverSysLog SL("case KEY_SHIFTLEFT"));
      if (CurPos==0 && CurLine->m_prev==NULL)return TRUE;
      if (CurPos==0) //������ � ������ ������
      {
        if(SelAtBeginning) //������ � ������ �����
        {
          BlockStart=CurLine->m_prev;
          CurLine->m_prev->Select(CurLine->m_prev->GetLength(),-1);
        }
        else // ������ � ����� �����
        {
          CurLine->Select(-1,0);
          CurLine->m_prev->GetRealSelection(SelStart,SelEnd);
          CurLine->m_prev->Select(SelStart,CurLine->m_prev->GetLength());
        }
      }
      else
      {
        if(SelAtBeginning || SelFirst)
        {
          CurLine->Select(SelStart-1,SelEnd);
        }
        else
        {
          CurLine->Select(SelStart,SelEnd-1);
        }
      }
      int LeftPos=CurLine->GetLeftPos();
      Edit *OldCur=CurLine;
      int _OldNumLine=NumLine;
      Pasting++;
      ProcessKey(KEY_LEFT);
      Pasting--;

      if(_OldNumLine!=NumLine)
      {
        BlockStartLine=NumLine;
      }

      ShowEditor(OldCur==CurLine && LeftPos==CurLine->GetLeftPos());
      return(TRUE);
    }

    case KEY_SHIFTRIGHT:  case KEY_SHIFTNUMPAD6:
    {
      _SVS(CleverSysLog SL("case KEY_SHIFTRIGHT"));
      if(CurLine->m_next==NULL && CurPos==CurLine->GetLength() && !EdOpt.CursorBeyondEOL)
      {
        return TRUE;
      }

      if(SelAtBeginning)
      {
        CurLine->Select(SelStart+1,SelEnd);
      }
      else
      {
        CurLine->Select(SelStart,SelEnd+1);
      }
      Edit *OldCur=CurLine;
      int OldLeft=CurLine->GetLeftPos();
      Pasting++;
      ProcessKey(KEY_RIGHT);
      Pasting--;
      if(OldCur!=CurLine)
      {
        if(SelAtBeginning)
        {
          OldCur->Select(-1,0);
          BlockStart=CurLine;
          BlockStartLine=NumLine;
        }
        else
        {
          OldCur->Select(SelStart,-1);
        }
      }
      ShowEditor(OldCur==CurLine && OldLeft==CurLine->GetLeftPos());
      return(TRUE);
    }

    case KEY_CTRLSHIFTLEFT:  case KEY_CTRLSHIFTNUMPAD4:
    {
      _SVS(CleverSysLog SL("case KEY_CTRLSHIFTLEFT"));
      _SVS(SysLog("[%d] Pasting=%d, SelEnd=%d",__LINE__,Pasting,SelEnd));
      {
        int SkipSpace=TRUE;
        Pasting++;
        /* $ 23.12.2000 OT */
        Lock ();

        /* OT $ */

        int CurPos;
        while (1)
        {
          const char *Str;
          int Length;
          CurLine->GetBinaryString(&Str,NULL,Length);
          /* $ 12.11.2002 DJ
             ��������� ���������� ������ Ctrl-Shift-Left �� ������ ������
          */
          CurPos=CurLine->GetCurPos();
          if (CurPos>Length)
          {
            int SelStartPos = CurPos;
            CurLine->ProcessKey(KEY_END);
            CurPos=CurLine->GetCurPos();
            if (CurLine->SelStart >= 0)
            {
              if (!SelAtBeginning)
                CurLine->Select(CurLine->SelStart, CurPos);
              else
                CurLine->Select(CurPos, CurLine->SelEnd);
            }
            else
              CurLine->Select(CurPos, SelStartPos);
          }
          /* DJ $ */
          if (CurPos==0)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos-1]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos-1]))
          /* IS $ */
          /* SVS $ */
            if (SkipSpace)
            {
              ProcessKey(KEY_SHIFTLEFT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_SHIFTLEFT);
        }
        Pasting--;
        /* $ 23.12.2000 OT */
        Unlock ();

        Show();
        /* OT $ */
      }
      return(TRUE);
    }

    case KEY_CTRLSHIFTRIGHT:  case KEY_CTRLSHIFTNUMPAD6:
    {
      _SVS(CleverSysLog SL("case KEY_CTRLSHIFTRIGHT"));
      _SVS(SysLog("[%d] Pasting=%d, SelEnd=%d",__LINE__,Pasting,SelEnd));
      {
        int SkipSpace=TRUE;
        Pasting++;
        Lock ();


        int CurPos;
        while (1)
        {
          const char *Str;
          int Length;
          CurLine->GetBinaryString(&Str,NULL,Length);
          CurPos=CurLine->GetCurPos();
          if (CurPos>=Length)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos]))
          /* IS $ */
          /* SVS $ */
            if (SkipSpace)
            {
              ProcessKey(KEY_SHIFTRIGHT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_SHIFTRIGHT);
        }
        Pasting--;
        Unlock ();

        Show();
      }
      return(TRUE);
    }

    case KEY_SHIFTDOWN:  case KEY_SHIFTNUMPAD2:
    {
      if (CurLine->m_next==NULL)return TRUE;
      CurPos=CurLine->RealPosToTab(CurPos);
      if(SelAtBeginning)//������� ���������
      {
        if(SelEnd==-1)
        {
          CurLine->Select(-1,0);
          BlockStart=CurLine->m_next;
          BlockStartLine=NumLine+1;
        }
        else
        {
          CurLine->Select(SelEnd,-1);
        }
        CurLine->m_next->GetRealSelection(SelStart,SelEnd);
        if(SelStart!=-1)SelStart=CurLine->m_next->RealPosToTab(SelStart);
        if(SelEnd!=-1)SelEnd=CurLine->m_next->RealPosToTab(SelEnd);
        if(SelStart==-1)
        {
          SelStart=0;
          SelEnd=CurPos;
        }
        else
        {
          if(SelEnd!=-1 && SelEnd<CurPos)
          {
            SelStart=SelEnd;
            SelEnd=CurPos;
          }
          else
          {
            SelStart=CurPos;
          }
        }
        if(SelStart!=-1)SelStart=CurLine->m_next->TabPosToReal(SelStart);
        if(SelEnd!=-1)SelEnd=CurLine->m_next->TabPosToReal(SelEnd);
        /*if(!EdOpt.CursorBeyondEOL && SelEnd>CurLine->m_next->GetLength())
        {
          SelEnd=CurLine->m_next->GetLength();
        }
        if(!EdOpt.CursorBeyondEOL && SelStart>CurLine->m_next->GetLength())
        {
          SelStart=CurLine->m_next->GetLength();
        }*/
      }
      else //��������� ���������
      {
        CurLine->Select(SelStart,-1);
        SelStart=0;
        SelEnd=CurPos;
        if(SelStart!=-1)SelStart=CurLine->m_next->TabPosToReal(SelStart);
        if(SelEnd!=-1)SelEnd=CurLine->m_next->TabPosToReal(SelEnd);
      }

      if(!EdOpt.CursorBeyondEOL && SelEnd > CurLine->m_next->GetLength())
      {
        SelEnd=CurLine->m_next->GetLength();
      }

      if(!EdOpt.CursorBeyondEOL && SelStart > CurLine->m_next->GetLength())
      {
        SelStart=CurLine->m_next->GetLength();
      }

//      if(!SelStart && !SelEnd)
//        CurLine->m_next->Select(-1,0);
//      else
        CurLine->m_next->Select(SelStart,SelEnd);

      Down();
      Show();
      return(TRUE);
    }

    case KEY_SHIFTUP: case KEY_SHIFTNUMPAD8:
    {
      if (CurLine->m_prev==NULL) return 0;
      if(SelAtBeginning || SelFirst) // ��������� ���������
      {
        CurLine->Select(0,SelEnd);
        SelStart=CurLine->RealPosToTab(CurPos);
        if(!EdOpt.CursorBeyondEOL &&
            CurLine->m_prev->TabPosToReal(SelStart)>CurLine->m_prev->GetLength())
        {
          SelStart=CurLine->m_prev->RealPosToTab(CurLine->m_prev->GetLength());
        }
        SelStart=CurLine->m_prev->TabPosToReal(SelStart);
        CurLine->m_prev->Select(SelStart,-1);
        BlockStart=CurLine->m_prev;
        BlockStartLine=NumLine-1;
      }
      else // ������� ���������
      {
        CurPos=CurLine->RealPosToTab(CurPos);
        if(SelStart==0)
        {
          CurLine->Select(-1,0);
        }
        else
        {
          CurLine->Select(0,SelStart);
        }
        CurLine->m_prev->GetRealSelection(SelStart,SelEnd);
        if(SelStart!=-1)SelStart=CurLine->m_prev->RealPosToTab(SelStart);
        if(SelStart!=-1)SelEnd=CurLine->m_prev->RealPosToTab(SelEnd);
        if(SelStart==-1)
        {
          BlockStart=CurLine->m_prev;
          BlockStartLine=NumLine-1;
          SelStart=CurLine->m_prev->TabPosToReal(CurPos);
          SelEnd=-1;
        }
        else
        {
          if(CurPos<SelStart)
          {
            SelEnd=SelStart;
            SelStart=CurPos;
          }
          else
          {
            SelEnd=CurPos;
          }

          SelStart=CurLine->m_prev->TabPosToReal(SelStart);
          SelEnd=CurLine->m_prev->TabPosToReal(SelEnd);

          if(!EdOpt.CursorBeyondEOL && SelEnd>CurLine->m_prev->GetLength())
          {
            SelEnd=CurLine->m_prev->GetLength();
          }

          if(!EdOpt.CursorBeyondEOL && SelStart>CurLine->m_prev->GetLength())
          {
            SelStart=CurLine->m_prev->GetLength();
          }
        }
        CurLine->m_prev->Select(SelStart,SelEnd);
      }
      Up();
      Show();
      return(TRUE);
    }

    case KEY_CTRLADD:
    {
      Copy(TRUE);
      return(TRUE);
    }

    case KEY_CTRLA:
    {
      UnmarkBlock();
      SelectAll();
      return(TRUE);
    }

    case KEY_CTRLU:
    {
      UnmarkBlock();
      return(TRUE);
    }

    case KEY_CTRLC:
    case KEY_CTRLINS:    case KEY_CTRLNUMPAD0:
    {
      if (/*!EdOpt.PersistentBlocks && */BlockStart==NULL && VBlockStart==NULL)
      {
        BlockStart=CurLine;
        BlockStartLine=NumLine;
        CurLine->AddSelect(0,-1);
        Show();
      }
      Copy(FALSE);
      return(TRUE);
    }

    case KEY_CTRLP:
    case KEY_CTRLM:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
        return TRUE;
      if (BlockStart!=NULL || VBlockStart!=NULL)
      {
        int SelStart,SelEnd;
        CurLine->GetSelection(SelStart,SelEnd);

        Pasting++;
        int OldUsedInternalClipboard=UsedInternalClipboard;
        UsedInternalClipboard=1;
        ProcessKey(Key==KEY_CTRLP ? KEY_CTRLINS:KEY_SHIFTDEL);

        /* $ 10.04.2001 SVS
          ^P/^M - ����������� ��������: ������ ��� CurPos ������ ���� ">=",
           � �� "������".
        */
        if (Key==KEY_CTRLM && SelStart!=-1 && SelEnd!=-1)
          if (CurPos>=SelEnd)
            CurLine->SetCurPos(CurPos-(SelEnd-SelStart));
          else
            CurLine->SetCurPos(CurPos);
        /* SVS $ */
        ProcessKey(KEY_SHIFTINS);
        Pasting--;
        FAR_EmptyClipboard();
        UsedInternalClipboard=OldUsedInternalClipboard;

        /*$ 08.02.2001 SKV
          �� �������� � pasting'��, ������� redraw �������� �� ����.
          ������� ���.
        */
        Show();
        /* SKV$*/
      }
      return(TRUE);
    }

    case KEY_CTRLX:
    case KEY_SHIFTNUMDEL:
    case KEY_SHIFTDECIMAL:
    case KEY_SHIFTDEL:
    {
      Copy(FALSE);
    }
    case KEY_CTRLD:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
        return TRUE;
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      DeleteBlock();
      Show();
      return(TRUE);
    }

    case KEY_CTRLV:
    case KEY_SHIFTINS: case KEY_SHIFTNUMPAD0:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
        return TRUE;
      /* $ 10.04.2001 SVS
         ������ Pasting ��������� :-(
      */
      Pasting++;
      if (!EdOpt.PersistentBlocks && VBlockStart==NULL)
        DeleteBlock();

      Paste();
      // MarkingBlock=(VBlockStart==NULL);
      Flags.Change(FEDITOR_MARKINGBLOCK,(VBlockStart==NULL));
      Flags.Clear(FEDITOR_MARKINGVBLOCK);
      if (!EdOpt.PersistentBlocks)
        UnmarkBlock();
      Pasting--;
      Show();
      return(TRUE);
      /* SVS $ */
    }

    case KEY_LEFT: case KEY_NUMPAD4:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      if (CurPos==0 && CurLine->m_prev!=NULL)
      {
        Up();
        Show();
        CurLine->ProcessKey(KEY_END);
        Show();
      }
      else
      {
        int LeftPos=CurLine->GetLeftPos();
        CurLine->ProcessKey(KEY_LEFT);
        ShowEditor(LeftPos==CurLine->GetLeftPos());
      }
      return(TRUE);
    }

    case KEY_INS: case KEY_NUMPAD0:
    {
      Flags.Swap(FEDITOR_OVERTYPE);
      Show();
      return(TRUE);
    }

    case KEY_NUMDEL:
    case KEY_DEL:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        // Del � ����� ��������� ������� ������ �� �������, ������� �� ������������...
        if(!CurLine->m_next && CurPos>=CurLine->GetLength() && BlockStart==NULL && VBlockStart==NULL)
          return TRUE;
        /* $ 07.03.2002 IS
           ������ ���������, ���� ���� ��� ����� ������
        */
        if(!Pasting)
          UnmarkEmptyBlock();
        /* IS $ */
        if (!Pasting && EdOpt.DelRemovesBlocks && (BlockStart!=NULL || VBlockStart!=NULL))
          DeleteBlock();
        else
        {
          AddUndoData(CurLine->GetStringAddr(),CurLine->GetEOL(),NumLine,
                      CurLine->GetCurPos(),UNDO_EDIT);
          if (CurPos>=CurLine->GetLength())
          {
            if (CurLine->m_next==NULL)
              CurLine->SetEOL("");
            else
            {
              int SelStart,SelEnd,NextSelStart,NextSelEnd;
              int Length=CurLine->GetLength();
              CurLine->GetSelection(SelStart,SelEnd);
              CurLine->m_next->GetSelection(NextSelStart,NextSelEnd);

              const char *Str;
              int NextLength;
              CurLine->m_next->GetBinaryString(&Str,NULL,NextLength);
              CurLine->InsertBinaryString(Str,NextLength);
              CurLine->SetEOL(CurLine->m_next->GetEOL());
              CurLine->SetCurPos(CurPos);

              BlockUndo++;
              DeleteString(CurLine->m_next,TRUE,NumLine+1);
              BlockUndo--;

              if (NextSelStart!=-1)
                if (SelStart==-1)
                {
                  CurLine->Select(Length+NextSelStart,NextSelEnd==-1 ? -1:Length+NextSelEnd);
                  BlockStart=CurLine;
                  BlockStartLine=NumLine;
                }
                else
                  CurLine->Select(SelStart,NextSelEnd==-1 ? -1:Length+NextSelEnd);

            }
          }
          else
            CurLine->ProcessKey(KEY_DEL);
          /*$ 10.08.2000 skv
            Modified->TextChanged
          */
          TextChanged(1);
          /* skv $*/
        }
        Show();
      }
      return(TRUE);
    }

    case KEY_BS:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        // Bs � ����� ������ ������� ������ �� �������, ������ �� ����� ����������
        if(!CurLine->m_prev && !CurPos && BlockStart==NULL && VBlockStart==NULL)
          return TRUE;
        /*$ 10.08.2000 skv
          Modified->TextChanged
        */
        TextChanged(1);
        /* skv $*/
        /* $ 11.10.2000 SVS
           Bs ������� ���� ��� ��, ��� � Del
        */
        int IsDelBlock=FALSE;
        if(EdOpt.BSLikeDel)
        {
          if (!Pasting && EdOpt.DelRemovesBlocks && (BlockStart!=NULL || VBlockStart!=NULL))
            IsDelBlock=TRUE;
        }
        else
        {
          if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
            IsDelBlock=TRUE;
        }
        if (IsDelBlock)
        /* SVS $ */
          DeleteBlock();
        else
          if (CurPos==0 && CurLine->m_prev!=NULL)
          {
            Pasting++;
            Up();
            CurLine->ProcessKey(KEY_CTRLEND);
            ProcessKey(KEY_DEL);
            Pasting--;
          }
          else
          {
            AddUndoData(CurLine->GetStringAddr(),CurLine->GetEOL(),NumLine,
                        CurLine->GetCurPos(),UNDO_EDIT);
            CurLine->ProcessKey(KEY_BS);
          }

        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLBS:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        /*$ 10.08.2000 skv
          Modified->TextChanged
        */
        TextChanged(1);
        /* skv $*/
        if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
          DeleteBlock();
        else
          if (CurPos==0 && CurLine->m_prev!=NULL)
            ProcessKey(KEY_BS);
          else
          {
            AddUndoData(CurLine->GetStringAddr(),CurLine->GetEOL(),NumLine,
                        CurLine->GetCurPos(),UNDO_EDIT);
            CurLine->ProcessKey(KEY_CTRLBS);
          }
        Show();
      }
      return(TRUE);
    }

    case KEY_UP: case KEY_NUMPAD8:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int PrevMaxPos=MaxRightPos;
        Edit *LastTopScreen=TopScreen;
        Up();
        if (TopScreen==LastTopScreen)
          ShowEditor(TRUE);
        else
          Show();
        if (PrevMaxPos>CurLine->GetTabCurPos())
        {
          CurLine->SetTabCurPos(PrevMaxPos);
          CurLine->FastShow();
          CurLine->SetTabCurPos(PrevMaxPos);
          Show();
        }
      }
      return(TRUE);
    }

    case KEY_DOWN: case KEY_NUMPAD2:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int PrevMaxPos=MaxRightPos;
        Edit *LastTopScreen=TopScreen;
        Down();
        if (TopScreen==LastTopScreen)
          ShowEditor(TRUE);
        else
          Show();
        if (PrevMaxPos>CurLine->GetTabCurPos())
        {
          CurLine->SetTabCurPos(PrevMaxPos);
          CurLine->FastShow();
          CurLine->SetTabCurPos(PrevMaxPos);
          Show();
        }
      }
      return(TRUE);
    }

    /* $ 27.04.2001 VVM
      + ��������� ������ ����� */
    case KEY_MSWHEEL_UP:
    case (KEY_MSWHEEL_UP | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaEdit;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_CTRLUP);
      return(TRUE);
    }

    case KEY_MSWHEEL_DOWN:
    case (KEY_MSWHEEL_DOWN | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsWheelDeltaEdit;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_CTRLDOWN);
      return(TRUE);
    }
    /* VVM $ */

    case KEY_MSWHEEL_LEFT:
    case (KEY_MSWHEEL_LEFT | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsHWheelDeltaEdit;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_LEFT);
      return TRUE;
    }

    case KEY_MSWHEEL_RIGHT:
    case (KEY_MSWHEEL_RIGHT | KEY_ALT):
    {
      int Roll = Key & KEY_ALT?1:Opt.MsHWheelDeltaEdit;
      for (int i=0; i<Roll; i++)
        ProcessKey(KEY_RIGHT);
      return TRUE;
    }

    case KEY_CTRLUP:  case KEY_CTRLNUMPAD8:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      ScrollUp();
      Show();
      return(TRUE);
    }

    case KEY_CTRLDOWN: case KEY_CTRLNUMPAD2:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      ScrollDown();
      Show();
      return(TRUE);
    }

    case KEY_PGUP:     case KEY_NUMPAD9:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      for (I=Y1;I<Y2;I++)
        ScrollUp();
      Show();
      return(TRUE);
    }

    case KEY_PGDN:    case KEY_NUMPAD3:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      for (I=Y1;I<Y2;I++)
        ScrollDown();
      Show();
      return(TRUE);
    }

    case KEY_CTRLHOME:  case KEY_CTRLNUMPAD7:
    case KEY_CTRLPGUP:  case KEY_CTRLNUMPAD9:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int StartPos=CurLine->GetTabCurPos();
        NumLine=0;
        TopScreen=CurLine=TopList;
        if (Key==KEY_CTRLHOME)
          CurLine->SetCurPos(0);
        else
          CurLine->SetTabCurPos(StartPos);
        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLEND:   case KEY_CTRLNUMPAD1:
    case KEY_CTRLPGDN:  case KEY_CTRLNUMPAD3:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        int StartPos=CurLine->GetTabCurPos();
        NumLine=NumLastLine-1;
        CurLine=EndList;
        for (TopScreen=CurLine,I=Y1;I<Y2 && TopScreen->m_prev!=NULL;I++)
        {
          TopScreen->SetPosition(X1,I,X2,I);
          TopScreen=TopScreen->m_prev;
        }
        CurLine->SetLeftPos(0);
        if (Key==KEY_CTRLEND)
        {
          CurLine->SetCurPos(CurLine->GetLength());
          CurLine->FastShow();
        }
        else
          CurLine->SetTabCurPos(StartPos);
        Show();
      }
      return(TRUE);
    }

    case KEY_NUMENTER:
    case KEY_ENTER:
    {
      if (Pasting || !ShiftPressed || CtrlObject->Macro.IsExecuting())
      {
        if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
          DeleteBlock();
        Flags.Set(FEDITOR_NEWUNDO);
        InsertString();
        CurLine->FastShow();
        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLN:
    {
      Flags.Set(FEDITOR_NEWUNDO);
      while (CurLine!=TopScreen)
      {
        CurLine=CurLine->m_prev;
        NumLine--;
      }
      CurLine->SetCurPos(CurPos);
      Show();
      return(TRUE);
    }

    case KEY_CTRLE:
    {
      {
        Flags.Set(FEDITOR_NEWUNDO);
        Edit *CurPtr=TopScreen;
        int CurLineFound=FALSE;
        for (I=Y1;I<Y2;I++)
        {
          if (CurPtr->m_next==NULL)
            break;
          if (CurPtr==CurLine)
            CurLineFound=TRUE;
          if (CurLineFound)
            NumLine++;
          CurPtr=CurPtr->m_next;
        }
        CurLine=CurPtr;
        CurLine->SetCurPos(CurPos);
        Show();
      }
      return(TRUE);
    }

    case KEY_CTRLL:
    {
      Flags.Swap(FEDITOR_LOCKMODE);
      if(HostFileEditor) HostFileEditor->ShowStatus();
      return(TRUE);
    }

    case KEY_CTRLY:
    {
      DeleteString(CurLine,FALSE,NumLine);
      Show();
      return(TRUE);
    }

    case KEY_F7:
    {
      int ReplaceMode0=ReplaceMode;
      int ReplaceAll0=ReplaceAll;
      ReplaceMode=ReplaceAll=FALSE;
      if(!Search(FALSE))
      {
        ReplaceMode=ReplaceMode0;
        ReplaceAll=ReplaceAll0;
      }
      return(TRUE);
    }

    case KEY_CTRLF7:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        int ReplaceMode0=ReplaceMode;
        int ReplaceAll0=ReplaceAll;
        ReplaceMode=TRUE;
        ReplaceAll=FALSE;
        if(!Search(FALSE))
        {
          ReplaceMode=ReplaceMode0;
          ReplaceAll=ReplaceAll0;
        }
      }
      return(TRUE);
    }

    case KEY_SHIFTF7:
    {
      /* $ 20.09.2000 SVS
         ��� All ����� ������� Shift-F7 ������� ����� ��������...
      */
      //ReplaceAll=FALSE;
      /* $ 07.05.2001 IS
         ������� � ����� "Shift-F7 ���������� _�����_"
      */
      //ReplaceMode=FALSE;
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      Search(TRUE);
      return(TRUE);
    }

    case KEY_F8:
    {
      Flags.Set(FEDITOR_TABLECHANGEDBYUSER);
      if ((AnsiText=!AnsiText)!=0)
      {
        int UseUnicode=FALSE;
        GetTable(&TableSet,TRUE,TableNum,UseUnicode);
      }
      TableNum=0;
      UseDecodeTable=AnsiText;
      SetStringsTable();
      if (HostFileEditor) HostFileEditor->ChangeEditKeyBar();
      Show();
      return(TRUE);
    }

    case KEY_SHIFTF8:
    {
      {
        int UseUnicode=FALSE;
        int GetTableCode=GetTable(&TableSet,FALSE,TableNum,UseUnicode);
        if (GetTableCode!=-1)
        {
          Flags.Set(FEDITOR_TABLECHANGEDBYUSER);
          UseDecodeTable=GetTableCode;
          AnsiText=FALSE;
          SetStringsTable();
          if (HostFileEditor) HostFileEditor->ChangeEditKeyBar();
          Show();
        }
      }
      return(TRUE);
    }

    case KEY_F11:
    {
/*
      if(!Flags.Check(FEDITOR_DIALOGMEMOEDIT))
      {
        CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
        if (CtrlObject->Plugins.CommandsMenu(MODALTYPE_EDITOR,0,"Editor"))
          *PluginTitle=0;
        Show();
      }
*/
      return(TRUE);
    }

    case KEY_ALTBS:
    case KEY_CTRLZ:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        /*$ 10.08.2000 skv
          Without this group undo, like undo of 'delete block' operation
          will be animated.
        */
        Lock ();
        Undo();
        Unlock ();
        /* skv$*/
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTF8:
    {
      {
        /* $ 05.07.2000 tran
           + ����������� ���������� �� ������ �� ������, �� � �� ������� */
        /* $ 21.07.2000 tran
           ��� ������ ������� */
        GoToPosition();
        /* tran 21.07.2000 $ */
        /* tran 05.07.2000 $ */
        // <GOTO_UNMARK:1>
        if (!EdOpt.PersistentBlocks)
          UnmarkBlock();
        // </GOTO_UNMARK>
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTU:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        BlockLeft();
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTI:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        BlockRight();
        Show();
      }
      return(TRUE);
    }

    case KEY_ALTSHIFTLEFT:  case KEY_ALTSHIFTNUMPAD4:
    case KEY_ALTLEFT:
    {
      if (CurPos==0)
        return(TRUE);
      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */
      Pasting++;
      {
        int Delta=CurLine->GetTabCurPos()-CurLine->RealPosToTab(CurPos-1);
        if (CurLine->GetTabCurPos()>VBlockX)
          VBlockSizeX-=Delta;
        else
        {
          VBlockX-=Delta;
          VBlockSizeX+=Delta;
        }
        /* $ 25.07.2000 tran
           ������� ���� 22 - ��������� ��� �������� �� ������� ����� */
        if ( VBlockSizeX<0 )
        {
            VBlockSizeX=-VBlockSizeX;
            VBlockX-=VBlockSizeX;
        }
        /* tran 25.07.2000 $ */
        ProcessKey(KEY_LEFT);
      }
      Pasting--;
      Show();
      //_D(SysLog("VBlockX=%i, VBlockSizeX=%i, GetLineCurPos=%i",VBlockX,VBlockSizeX,GetLineCurPos()));
      //_D(SysLog("~~~~~~~~~~~~~~~~ KEY_ALTLEFT END, VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      return(TRUE);
    }

    case KEY_ALTSHIFTRIGHT:  case KEY_ALTSHIFTNUMPAD6:
    case KEY_ALTRIGHT:
    {
      /* $ 23.10.2000 tran
         ������ GetTabCurPos ���� �������� GetCurPos -
         ���������� �������� ������� � �������� ������
         � ���� ��������� ������� �������� � �������� ������*/
      if (!EdOpt.CursorBeyondEOL && CurLine->GetCurPos()>=CurLine->GetLength())
        return(TRUE);
      /* tran 23.10.2000 $ */

      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */

      /* $ 21.07.2000 tran
         bug22 - �����������
      */
      //_D(SysLog("---------------- KEY_ALTRIGHT, getLineCurPos=%i",GetLineCurPos()));
      Pasting++;
      {
        int Delta;
        /* $ 18.07.2000 tran
             ������ � ������ ������, ����� alt-right, alt-pagedown,
             ��������� ���� ������� � 1 �������, ����� ��� alt-right
             ��������� ���������
        */
        int VisPos=CurLine->RealPosToTab(CurPos),
            NextVisPos=CurLine->RealPosToTab(CurPos+1);
        //_D(SysLog("CurPos=%i, VisPos=%i, NextVisPos=%i",
        //    CurPos,VisPos, NextVisPos); //,CurLine->GetTabCurPos()));

        Delta=NextVisPos-VisPos;
         //_D(SysLog("Delta=%i",Delta));
        /* tran $ */

        if (CurLine->GetTabCurPos()>=VBlockX+VBlockSizeX)
          VBlockSizeX+=Delta;
        else
        {
          VBlockX+=Delta;
          VBlockSizeX-=Delta;
        }
        /* $ 25.07.2000 tran
           ������� ���� 22 - ��������� ��� �������� �� ������� ����� */
        if ( VBlockSizeX<0 )
        {
            VBlockSizeX=-VBlockSizeX;
            VBlockX-=VBlockSizeX;
        }
        /* tran 25.07.2000 $ */
        ProcessKey(KEY_RIGHT);
        //_D(SysLog("VBlockX=%i, VBlockSizeX=%i, GetLineCurPos=%i",VBlockX,VBlockSizeX,GetLineCurPos()));
      }
      Pasting--;
      Show();
      //_D(SysLog("~~~~~~~~~~~~~~~~ KEY_ALTRIGHT END, VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      /* tran 21.07.2000 $ */

      return(TRUE);
    }

  /* $ 29.06.2000 IG
      + CtrlAltLeft, CtrlAltRight ��� ������������ ������
  */
    case KEY_CTRLALTLEFT: case KEY_CTRLALTNUMPAD4:
    {
      {
        int SkipSpace=TRUE;
        Pasting++;
        /* $ 23.12.2000 OT */
        Lock ();

        /* OT $ */
        while (1)
        {
          const char *Str;
          int Length;
          CurLine->GetBinaryString(&Str,NULL,Length);
          int CurPos=CurLine->GetCurPos();
          if (CurPos>Length)
          {
            CurLine->ProcessKey(KEY_END);
            CurPos=CurLine->GetCurPos();
          }
          if (CurPos==0)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos-1]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos-1]))
          /* IS $ */
          /* SVS $ */
            if (SkipSpace)
            {
              ProcessKey(KEY_ALTSHIFTLEFT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_ALTSHIFTLEFT);
        }
        Pasting--;
        /* $ 23.12.2000 OT */
        Unlock ();

        Show();
        /* OT $ */
      }
      return(TRUE);
    }

    case KEY_CTRLALTRIGHT: case KEY_CTRLALTNUMPAD6:
    {
      {
        int SkipSpace=TRUE;
        Pasting++;
        Lock ();

        while (1)
        {
          const char *Str;
          int Length;
          CurLine->GetBinaryString(&Str,NULL,Length);
          int CurPos=CurLine->GetCurPos();
          if (CurPos>=Length)
            break;
          /* $ 03.08.2000 SVS
            ! WordDiv -> Opt.WordDiv
          */
          /* $ 12.01.2004 IS
             ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
             ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
          */
          if (IsSpace(Str[CurPos]) ||
              IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,EdOpt.WordDiv,Str[CurPos]))
          /* IS $ */
          /* SVS $*/
            if (SkipSpace)
            {
              ProcessKey(KEY_ALTSHIFTRIGHT);
              continue;
            }
            else
              break;
          SkipSpace=FALSE;
          ProcessKey(KEY_ALTSHIFTRIGHT);
        }
        Pasting--;
        Unlock ();

        Show();
      }
      return(TRUE);
    }
    /* IG $ */

    case KEY_ALTSHIFTUP:    case KEY_ALTSHIFTNUMPAD8:
    case KEY_ALTUP:
    {
      if (CurLine->m_prev==NULL)
        return(TRUE);
      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */

      if (!EdOpt.CursorBeyondEOL && VBlockX>=CurLine->m_prev->GetLength())
        return(TRUE);
      Pasting++;
      if (NumLine>VBlockY)
        VBlockSizeY--;
      else
      {
        VBlockY--;
        VBlockSizeY++;
        VBlockStart=VBlockStart->m_prev;
        BlockStartLine--;
      }
      ProcessKey(KEY_UP);
      /* $ 21.07.2000 tran
         �������� ������� �������� ����� */
      AdjustVBlock(CurVisPos);
      /* tran 21.07.2000 $ */
      Pasting--;
      Show();
      //_D(SysLog("~~~~~~~~ ALT_PGUP, VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      return(TRUE);
    }

    case KEY_ALTSHIFTDOWN:  case KEY_ALTSHIFTNUMPAD2:
    case KEY_ALTDOWN:
    {
      if (CurLine->m_next==NULL)
        return(TRUE);
      /* $ 21.07.2000 tran
         ��� ����� � BeginVBlockMarking */
      if (!Flags.Check(FEDITOR_MARKINGVBLOCK))
        BeginVBlockMarking();
      /* tran 21.07.2000 $ */
      if (!EdOpt.CursorBeyondEOL && VBlockX>=CurLine->m_next->GetLength())
        return(TRUE);
      Pasting++;
      if (NumLine>=VBlockY+VBlockSizeY-1)
        VBlockSizeY++;
      else
      {
        VBlockY++;
        VBlockSizeY--;
        VBlockStart=VBlockStart->m_next;
        BlockStartLine++;
      }
      ProcessKey(KEY_DOWN);
      /* $ 21.07.2000 tran
         �������� ������� �������� ����� */
      AdjustVBlock(CurVisPos);
      /* tran 21.07.2000 $ */
      Pasting--;
      Show();
      //_D(SysLog("~~~~ Key_AltDOWN: VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
      return(TRUE);
    }

    case KEY_ALTSHIFTHOME: case KEY_ALTSHIFTNUMPAD7:
    case KEY_ALTHOME:
    {
      Pasting++;
      Lock ();
      while (CurLine->GetCurPos()>0)
        ProcessKey(KEY_ALTSHIFTLEFT);
      Unlock ();
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_ALTSHIFTEND: case KEY_ALTSHIFTNUMPAD1:
    case KEY_ALTEND:
    {
      Pasting++;
      Lock ();
      if (CurLine->GetCurPos()<CurLine->GetLength())
        while (CurLine->GetCurPos()<CurLine->GetLength())
          ProcessKey(KEY_ALTSHIFTRIGHT);
      if (CurLine->GetCurPos()>CurLine->GetLength())
        while (CurLine->GetCurPos()>CurLine->GetLength())
          ProcessKey(KEY_ALTSHIFTLEFT);
      Unlock ();
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_ALTSHIFTPGUP: case KEY_ALTSHIFTNUMPAD9:
    case KEY_ALTPGUP:
    {
      Pasting++;
      Lock ();
      for (I=Y1;I<Y2;I++)
        ProcessKey(KEY_ALTSHIFTUP);
      Unlock ();
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_ALTSHIFTPGDN: case KEY_ALTSHIFTNUMPAD3:
    case KEY_ALTPGDN:
    {
      Pasting++;
      Lock ();
      for (I=Y1;I<Y2;I++)
        ProcessKey(KEY_ALTSHIFTDOWN);
      Unlock ();
      Pasting--;
      Show();
      return(TRUE);
    }

    case KEY_CTRLALTPGUP: case KEY_CTRLALTNUMPAD9:
    case KEY_CTRLALTHOME: case KEY_CTRLALTNUMPAD7:
    {
      Lock ();
      Pasting++;
      while (CurLine!=TopList)
      {

        ProcessKey(KEY_ALTUP);
      }
      Pasting--;
      Unlock ();

      Show();
      return(TRUE);
    }

    case KEY_CTRLALTPGDN:  case KEY_CTRLALTNUMPAD3:
    case KEY_CTRLALTEND:   case KEY_CTRLALTNUMPAD1:
    {
      Lock ();
      Pasting++;
      while (CurLine!=EndList)
      {

        ProcessKey(KEY_ALTDOWN);
      }
      Pasting--;
      Unlock ();

      Show();
      return(TRUE);
    }

    case KEY_CTRLALTBRACKET:       // �������� ������� (UNC) ���� �� ����� ������
    case KEY_CTRLALTBACKBRACKET:   // �������� ������� (UNC) ���� �� ������ ������
    case KEY_ALTSHIFTBRACKET:      // �������� ������� (UNC) ���� �� �������� ������
    case KEY_ALTSHIFTBACKBRACKET:  // �������� ������� (UNC) ���� �� ��������� ������
    case KEY_CTRLBRACKET:          // �������� ���� �� ����� ������
    case KEY_CTRLBACKBRACKET:      // �������� ���� �� ������ ������
    case KEY_CTRLSHIFTBRACKET:     // �������� ���� �� �������� ������
    case KEY_CTRLSHIFTBACKBRACKET: // �������� ���� �� ��������� ������

    case KEY_CTRLSHIFTNUMENTER:
    case KEY_SHIFTNUMENTER:
    case KEY_CTRLSHIFTENTER:
    case KEY_SHIFTENTER:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        Pasting++;
        TextChanged(1);
        if (!EdOpt.PersistentBlocks && BlockStart!=NULL)
        {
          Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
          DeleteBlock();
        }
        AddUndoData(CurLine->GetStringAddr(),CurLine->GetEOL(),NumLine,
                        CurLine->GetCurPos(),UNDO_EDIT);
        CurLine->ProcessKey(Key);
        Pasting--;
        Show();
      }
      return(TRUE);
    }

    /* $ 11.04.2001 SVS
       ��������� ��������� Ctrl-Q
    */
    case KEY_CTRLQ:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        Flags.Set(FEDITOR_PROCESSCTRLQ);
        if(HostFileEditor) HostFileEditor->ShowStatus();
        Pasting++;
        TextChanged(1);
        if (!EdOpt.PersistentBlocks && BlockStart!=NULL)
        {
          Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
          DeleteBlock();
        }
        AddUndoData(CurLine->GetStringAddr(),CurLine->GetEOL(),NumLine,
                        CurLine->GetCurPos(),UNDO_EDIT);
        CurLine->ProcessCtrlQ();
        Flags.Clear(FEDITOR_PROCESSCTRLQ);
        Pasting--;
        Show();
      }
      return(TRUE);
    }
    /* SVS $ */

    case KEY_OP_SELWORD:
    {
      int OldCurPos=CurPos;
      int SStart, SEnd;
      Pasting++;
      Lock ();

      UnmarkBlock();
      CalcWordFromString(CurLine->GetStringAddr(),CurPos,&SStart,&SEnd,UseDecodeTable?&TableSet:NULL,EdOpt.WordDiv);
      CurLine->Select(SStart,SEnd+(SEnd < CurLine->StrSize?1:0));

      Flags.Set(FEDITOR_MARKINGBLOCK);
      BlockStart=CurLine;
      BlockStartLine=NumLine;
      //SelFirst=TRUE;
      SelStart=SStart;
      SelEnd=SEnd;

      //CurLine->ProcessKey(MCODE_OP_SELWORD);

      CurPos=OldCurPos; // ���������� �������
      Pasting--;
      Unlock ();

      Show();
      return TRUE;
    }

    case KEY_OP_DATE:
    case KEY_OP_PLAINTEXT:
    {
      if (!Flags.Check(FEDITOR_LOCKMODE))
      {
        const char *Fmt = eStackAsString();
        int SizeMacroText = 16+(Fmt && *Fmt ? (int)strlen(Fmt) : (int)strlen(Opt.DateFormat));
        if(Key == KEY_OP_PLAINTEXT)
          SizeMacroText=(int)strlen(Fmt)+1;
        SizeMacroText*=4+1;
        char *TStr=(char*)alloca(SizeMacroText);
        if(!TStr)
          return FALSE;
        if(Key == KEY_OP_PLAINTEXT)
          strcpy(TStr,Fmt);
        if(Key == KEY_OP_PLAINTEXT || MkStrFTime(TStr,SizeMacroText,Fmt))
        {
          char *Ptr=TStr;
          while(*Ptr) // ������� 0x0A �� 0x0D �� �������� Paset ;-)
          {
            if(*Ptr == 10)
              *Ptr=13;
            ++Ptr;
          }

          Pasting++;
          //_SVS(SysLogDump(Fmt,0,TStr,strlen(TStr),NULL));
          TextChanged(1);
          BOOL IsBlock=VBlockStart || BlockStart;
          if (!EdOpt.PersistentBlocks && IsBlock)
          {
            Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
            DeleteBlock();
          }
          //AddUndoData(CurLine->GetStringAddr(),CurLine->GetEOL(),NumLine,
          //              CurLine->GetCurPos(),UNDO_EDIT);
          Paste(TStr);
          //if (!EdOpt.PersistentBlocks && IsBlock)
          UnmarkBlock();
          Pasting--;
          Show();
        }
      }
      return(TRUE);
    }

    default:
    {
      {
        if ((Key==KEY_CTRLDEL || Key==KEY_CTRLNUMDEL || Key==KEY_CTRLDECIMAL || Key==KEY_CTRLT) && CurPos>=CurLine->GetLength())
        {
         /*$ 08.12.2000 skv
           - CTRL-DEL � ������ ������ ��� ���������� ����� �
             ���������� EditorDelRemovesBlocks
         */
          int save=EdOpt.DelRemovesBlocks;
          EdOpt.DelRemovesBlocks=0;
          int ret=ProcessKey(KEY_DEL);
          EdOpt.DelRemovesBlocks=save;
          return ret;
          /* skv$*/
        }

        if (!Pasting && !EdOpt.PersistentBlocks && BlockStart!=NULL)
          if (Key>=32 && Key<256 || Key==KEY_ADD || Key==KEY_SUBTRACT ||
              Key==KEY_MULTIPLY || Key==KEY_DIVIDE || Key==KEY_TAB)
          {
            DeleteBlock();
            /* $ 19.09.2002 SKV
              ������ ����.
              ����� ���� ��� ������� ��������� ��������
              ����� � ������ ����� �� ��������� � ���������
              ���������� ���� ����� �������.
            */
            Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
            /* SKV $ */
            Show();
          }

        int SkipCheckUndo=(Key==KEY_RIGHT     || Key==KEY_NUMPAD6     ||
                           Key==KEY_CTRLLEFT  || Key==KEY_CTRLNUMPAD4 ||
                           Key==KEY_CTRLRIGHT || Key==KEY_CTRLNUMPAD6 ||
                           Key==KEY_HOME      || Key==KEY_NUMPAD7     ||
                           Key==KEY_END       || Key==KEY_NUMPAD1     ||
                           Key==KEY_CTRLS);

        if (Flags.Check(FEDITOR_LOCKMODE) && !SkipCheckUndo)
          return(TRUE);

        if ((Key==KEY_CTRLLEFT || Key==KEY_CTRLNUMPAD4) && CurLine->GetCurPos()==0)
        {
          Pasting++;
          ProcessKey(KEY_LEFT);
          Pasting--;
          /* $ 24.9.2001 SKV
            fix ���� � ctrl-left � ������ ������
            � ����� � ��������������� �������� �����.
          */
          ShowEditor(FALSE);
          //if(!Flags.Check(FEDITOR_DIALOGMEMOEDIT)){
          //  CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
          //  _D(SysLog("%08d EE_REDRAW",__LINE__));
          //  CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL);
          // }
          /* SKV$*/
          return(TRUE);
        }

        if (((!EdOpt.CursorBeyondEOL && (Key==KEY_RIGHT || Key==KEY_NUMPAD6)) || Key==KEY_CTRLRIGHT || Key==KEY_CTRLNUMPAD6) &&
            CurLine->GetCurPos()>=CurLine->GetLength() &&
            CurLine->m_next!=NULL)
        {
          Pasting++;
          ProcessKey(KEY_HOME);
          ProcessKey(KEY_DOWN);
          Pasting--;
          if(!Flags.Check(FEDITOR_DIALOGMEMOEDIT))
          {
            CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
            //_D(SysLog("%08d EE_REDRAW",__LINE__));
            _SYS_EE_REDRAW(SysLog("Editor::ProcessKey[%d](!EdOpt.CursorBeyondEOL): EE_REDRAW(EEREDRAW_ALL)",__LINE__));
            CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_ALL);
          }
          /*$ 03.02.2001 SKV
            � �� EEREDRAW_ALL �� ������, � �� ����� ����
            ������ ������� ����� ����������������.
          */
          ShowEditor(0);
          /* SKV$*/
          return(TRUE);
        }

        const char *Str;
        char *CmpStr=0;
        int Length,CurPos;

        CurLine->GetBinaryString(&Str,NULL,Length);
        CurPos=CurLine->GetCurPos();

        if (Key<256 && CurPos>0 && Length==0)
        {
          Edit *PrevLine=CurLine->m_prev;
          while (PrevLine!=NULL && PrevLine->GetLength()==0)
            PrevLine=PrevLine->m_prev;
          if (PrevLine!=NULL)
          {
            int TabPos=CurLine->GetTabCurPos();
            CurLine->SetCurPos(0);
            const char *PrevStr=NULL;
            int PrevLength=0;
            PrevLine->GetBinaryString(&PrevStr,NULL,PrevLength);
            for (int I=0;I<PrevLength && IsSpace(PrevStr[I]);I++)
            {
              int NewTabPos=CurLine->GetTabCurPos();
              if (NewTabPos==TabPos)
                break;
              if (NewTabPos>TabPos)
              {
                CurLine->ProcessKey(KEY_BS);
                while (CurLine->GetTabCurPos()<TabPos)
                  CurLine->ProcessKey(' ');
                break;
              }
              if (NewTabPos<TabPos)
                CurLine->ProcessKey(PrevStr[I]);
            }
            CurLine->SetTabCurPos(TabPos);
          }
        }

        if (!SkipCheckUndo)
        {
          CurLine->GetBinaryString(&Str,NULL,Length);
          CurPos=CurLine->GetCurPos();
          CmpStr=new char[Length+1];
          memcpy(CmpStr,Str,Length);
          CmpStr[Length]=0;
        }

        int LeftPos=CurLine->GetLeftPos();

        /* $ 24.09.2000 SVS
           ����� ������� Xlat
        */
        /* $ 04.11.2000 SVS
           �������� �� �������������� �������
        */
        /* $ 25.11.2000 IS
           ������ Xlat �������� ���� ��� ���������� ���������
        */
        if((Opt.XLat.XLatEditorKey && Key == Opt.XLat.XLatEditorKey ||
            Opt.XLat.XLatAltEditorKey && Key == Opt.XLat.XLatAltEditorKey) ||
           Key == KEY_OP_XLAT)
        /* IS  $ */
        {
          Xlat();
          Show();
          return TRUE;
        }
        /* SVS $ */
        /* SVS $ */

        // <comment> - ��� ��������� ��� ���������� ������ ������ ������ ��� Ctrl-K
        int PreSelStart,PreSelEnd;
        CurLine->GetSelection(PreSelStart,PreSelEnd);
        // </comment>

        //AY: ��� ��� �� ��� FastShow LeftPos �� ���������� � ����� ������.
        CurLine->ObjWidth=X2-X1+1;

        if (CurLine->ProcessKey(Key))
        {
          int SelStart,SelEnd;
          /* $ 17.09.2002 SKV
            ���� ��������� � �������� �����,
            � ������ ������, � �������� tab, ������� ����������
            �� �������, ��������� ������. ��� ����.
          */
          if(Key==KEY_TAB && CurLine->GetConvertTabs() &&
             BlockStart!=NULL && BlockStart!=CurLine)
          {
            CurLine->GetSelection(SelStart,SelEnd);
            CurLine->Select(SelStart==-1?-1:0,SelEnd);
          }
          /* SKV $ */
          if (!SkipCheckUndo)
          {
            const char *NewCmpStr;
            int NewLength;
            CurLine->GetBinaryString(&NewCmpStr,NULL,NewLength);
            if (NewLength!=Length || memcmp(CmpStr,NewCmpStr,Length)!=0)
            {
              AddUndoData(CmpStr,CurLine->GetEOL(),NumLine,CurPos,UNDO_EDIT); // EOL? - CurLine->GetEOL()  GlobalEOL   ""
              /*$ 10.08.2000 skv
                Modified->TextChanged
              */
              TextChanged(1);
              /* skv $*/
            }
            delete[] CmpStr;
          }
          // <Bug 794>
          // ���������� ������ ������ � ��������� ������ � ������
          if(Key == KEY_CTRLK && EdOpt.PersistentBlocks)
          {
             if(CurLine==BlockStart)
             {
               if(CurPos)
               {
                 CurLine->GetSelection(SelStart,SelEnd);
                 // 1. ���� �� ������ ������ (CurPos ��� ����� � ������, ��� SelStart)
                 if(SelEnd == -1 && PreSelStart > CurPos || SelEnd > CurPos)
                   SelStart=SelEnd=-1; // � ���� ������ ������� ���������
                 // 2. CurPos ������ �����
                 else if(SelEnd == -1 && PreSelEnd > CurPos && SelStart < CurPos)
                   SelEnd=PreSelEnd;   // � ���� ������ ������� ����
                 // 3. ���� ������� ����� �� CurPos ��� ��������� ����� ����� (��. ����)
                 if(SelEnd >= CurPos || SelStart==-1)
                   CurLine->Select(SelStart,CurPos);
               }
               else
               {
                 CurLine->Select(-1,-1);
                 BlockStart=BlockStart->m_next;
               }
             }
             else // ����� ������ !!! ���� ���������� ���� ���������� ������� (�� �������), �� ���� ��������... ����� ��������...
             {
               // ������ ��� ��������� ������ (� ��������� �� ���)
               Edit *CurPtrBlock=BlockStart,*CurPtrBlock2=BlockStart;
               while (CurPtrBlock!=NULL)
               {
                 CurPtrBlock->GetRealSelection(SelStart,SelEnd);
                 if (SelStart==-1)
                   break;
                 CurPtrBlock2=CurPtrBlock;
                 CurPtrBlock=CurPtrBlock->m_next;
               }

               if(CurLine==CurPtrBlock2)
               {
                 if(CurPos)
                 {
                   CurLine->GetSelection(SelStart,SelEnd);
                   CurLine->Select(SelStart,CurPos);
                 }
                 else
                 {
                   CurLine->Select(-1,-1);
                   CurPtrBlock2=CurPtrBlock2->m_next;
                 }
               }

             }
          }
          // </Bug 794>

          ShowEditor(LeftPos==CurLine->GetLeftPos());
          return(TRUE);
        }
        else
          if (!SkipCheckUndo)
            delete[] CmpStr;
        if (VBlockStart!=NULL)
          Show();
      }
      return(FALSE);
    }
  }
}


int Editor::ProcessMouse(MOUSE_EVENT_RECORD *MouseEvent)
{
  Edit *NewPtr;
  int NewDist,Dist;
/* $ 28.12.2000 VVM
  + ������ ������ ������� ������������ ���� ������ */
  if ((MouseEvent->dwButtonState & 3)!=0)
  {
    Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
    if ((!EdOpt.PersistentBlocks) && (BlockStart!=NULL || VBlockStart!=NULL))
    {
      UnmarkBlock();
      Show();
    } /* if */
  } /* if */

  if (CurLine->ProcessMouse(MouseEvent))
  {
    if(HostFileEditor) HostFileEditor->ShowStatus();
    if (VBlockStart!=NULL)
      Show();
    else
    {
      if(!Flags.Check(FEDITOR_DIALOGMEMOEDIT))
      {
        CtrlObject->Plugins.CurEditor=HostFileEditor; // this;
        _SYS_EE_REDRAW(SysLog("Editor::ProcessMouse[%08d] ProcessEditorEvent(EE_REDRAW,EEREDRAW_LINE)",__LINE__));
        CtrlObject->Plugins.ProcessEditorEvent(EE_REDRAW,EEREDRAW_LINE);
      }
    }
    return(TRUE);
  }

  if ((MouseEvent->dwButtonState & 3)==0)
    return(FALSE);
/* VVM $ */

  // scroll up
  //_SVS(SysLog(">>>>>> Y1-1=%d, MouseEvent->dwMousePosition.Y=%d, dwEventFlags=%x",Y1-1,MouseEvent->dwMousePosition.Y,MouseEvent->dwEventFlags));
  if (MouseEvent->dwMousePosition.Y==Y1-1)
  {
    while (IsMouseButtonPressed() && MouseY==Y1-1)
      ProcessKey(KEY_UP);
    return(TRUE);
  }

  // scroll down
  //_SVS(SysLog(">>>>>> Y2+1=%d, MouseEvent->dwMousePosition.Y=%d, dwEventFlags=%x",Y2+1,MouseEvent->dwMousePosition.Y,MouseEvent->dwEventFlags));
  if (MouseEvent->dwMousePosition.Y==Y2+1)
  {
    while (IsMouseButtonPressed() && MouseY==Y2+1)
      ProcessKey(KEY_DOWN);
    return(TRUE);
  }

  if (MouseEvent->dwMousePosition.X<X1 || MouseEvent->dwMousePosition.X>X2 ||
      MouseEvent->dwMousePosition.Y<Y1 || MouseEvent->dwMousePosition.Y>Y2)
    return(FALSE);

  NewDist=MouseEvent->dwMousePosition.Y-Y1;
  NewPtr=TopScreen;
  while (NewDist-- && NewPtr->m_next)
    NewPtr=NewPtr->m_next;

  Dist=CalcDistance(TopScreen,NewPtr,-1)-CalcDistance(TopScreen,CurLine,-1);

  if (Dist>0)
    while (Dist--)
      Down();
  else
    while (Dist++)
      Up();

  CurLine->ProcessMouse(MouseEvent);
  Show();
  return(TRUE);
}


int Editor::CalcDistance(Edit *From,Edit *To,int MaxDist)
{
  int Distance=0;
  while (From!=To && From->m_next!=NULL && (MaxDist==-1 || MaxDist-- > 0))
  {
    Distance++;
    From=From->m_next;
  }
  return(Distance);
}



void Editor::DeleteString(Edit *DelPtr,int DeleteLast,int UndoLine)
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;
  /* $ 16.12.2000 OT
     CtrlY �� ��������� ������ � ���������� ������������ ������ �� ������ ��������� */
  if (VBlockStart!=NULL && NumLine<VBlockY+VBlockSizeY)
    if (NumLine<VBlockY)
    {
      if (VBlockY>0)
      {
        VBlockY--;
        BlockStartLine--;
      }
    }
    else
      if (--VBlockSizeY<=0)
        VBlockStart=NULL;
  /* OT $ */

  /*$ 10.08.2000 skv
    Modified->TextChanged
  */
  TextChanged(1);
  /* skv $*/
  if (DelPtr->m_next==NULL && (!DeleteLast || DelPtr->m_prev==NULL))
  {
    AddUndoData(DelPtr->GetStringAddr(),DelPtr->GetEOL(),UndoLine,
                DelPtr->GetCurPos(),UNDO_EDIT);
    DelPtr->SetString("");
    return;
  }

  for (int I=0;I<sizeof(SavePos.Line)/sizeof(SavePos.Line[0]);I++)
    if (SavePos.Line[I]!=0xffffffff && UndoLine<static_cast<int>(SavePos.Line[I]))
      SavePos.Line[I]--;

  if (StackPos)
  {
    struct InternalEditorStackBookMark *sb_temp = StackPos, *sb_new;

    while(sb_temp->prev)
      sb_temp=sb_temp->prev;
    while (sb_temp)
    {
      sb_new = sb_temp->next;
      if (UndoLine < static_cast<int>(sb_temp->Line))
        sb_temp->Line--;
      else
      {
        if (UndoLine == static_cast<int>(sb_temp->Line))
          DeleteStackBookmark(sb_temp);
      }
      sb_temp = sb_new;
    }
  }

  NumLastLine--;

  if (CurLine==DelPtr)
  {
    int LeftPos,CurPos;
    CurPos=DelPtr->GetTabCurPos();
    LeftPos=DelPtr->GetLeftPos();
    if (DelPtr->m_next!=NULL)
      CurLine=DelPtr->m_next;
    else
    {
      CurLine=DelPtr->m_prev;
      /* $ 04.11.2002 SKV
        ����� ��� ���� ��� ���������, ����� ������� ������ ���� ��������.
      */
      NumLine--;
      /* SKV $ */
    }
    CurLine->SetLeftPos(LeftPos);
    CurLine->SetTabCurPos(CurPos);
  }

  if (DelPtr->m_prev)
  {
    DelPtr->m_prev->m_next=DelPtr->m_next;
    if (DelPtr==EndList)
      EndList=EndList->m_prev;
  }
  if (DelPtr->m_next!=NULL)
    DelPtr->m_next->m_prev=DelPtr->m_prev;
  if (DelPtr==TopScreen)
    if (TopScreen->m_next!=NULL)
      TopScreen=TopScreen->m_next;
    else
      TopScreen=TopScreen->m_prev;
  if (DelPtr==TopList)
    TopList=TopList->m_next;
  if (DelPtr==BlockStart)
  {
    BlockStart=BlockStart->m_next;
    // Mantis#0000316: �� �������� ����������� ������
    if(BlockStart && !BlockStart->IsSelection())
       BlockStart=NULL;
  }
  if (DelPtr==VBlockStart)
    VBlockStart=VBlockStart->m_next;
  if (UndoLine!=-1)
    AddUndoData(DelPtr->GetStringAddr(),DelPtr->GetEOL(),UndoLine,0,UNDO_DELSTR);
  delete DelPtr;
}


void Editor::InsertString()
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;
  /*$ 10.08.2000 skv
    There is only one return - if new will fail.
    In this case things are realy bad.
    Move TextChanged to the end of functions
    AFTER all modifications are made.
  */
//  TextChanged(1);
  /* skv $*/
  Edit *NewString;
  Edit *SrcIndent=NULL;
  int SelStart,SelEnd;
  int CurPos;
  int NewLineEmpty=TRUE;

  NewString = CreateString (NULL, 0);

  if ( !NewString )
    return;

  NewString->SetTables(UseDecodeTable ? &TableSet:NULL);
  NewString->SetEditBeyondEnd(EdOpt.CursorBeyondEOL);
  NewString->m_prev=CurLine;
  NewString->m_next=CurLine->m_next;
  if (CurLine->m_next)
    CurLine->m_next->m_prev=NewString;
  CurLine->m_next=NewString;

  int Length;
  const char *CurLineStr;
  const char *EndSeq;
  CurLine->GetBinaryString(&CurLineStr,&EndSeq,Length);

  /* $ 13.01.2002 IS
     ���� �� ��� ��������� ��� ����� ������, �� ������� ��� ����� ������
     � ��� ����� DOS_EOL_fmt � ��������� ��� ����.
  */
  if (!*EndSeq)
      CurLine->SetEOL(*GlobalEOL?GlobalEOL:DOS_EOL_fmt);
  /* IS $ */

  CurPos=CurLine->GetCurPos();
  CurLine->GetSelection(SelStart,SelEnd);

  for (int I=0;I<sizeof(SavePos.Line)/sizeof(SavePos.Line[0]);I++)
    if (SavePos.Line[I]!=0xffffffff &&
        (NumLine<static_cast<int>(SavePos.Line[I]) || NumLine==SavePos.Line[I] && CurPos==0))
      SavePos.Line[I]++;

  if (StackPos)
  {
    struct InternalEditorStackBookMark *sb_temp = StackPos;

    while(sb_temp->prev)
      sb_temp=sb_temp->prev;
    while(sb_temp)
    {
      if (NumLine < static_cast<int>(sb_temp->Line) || NumLine==static_cast<int>(sb_temp->Line) && CurPos==0)
        sb_temp->Line++;
      sb_temp=sb_temp->next;
    }
  }

  int IndentPos=0;

  if (EdOpt.AutoIndent && !Pasting)
  {
    Edit *PrevLine=CurLine;
    while (PrevLine!=NULL)
    {
      const char *Str;
      int Length,Found=FALSE;
      PrevLine->GetBinaryString(&Str,NULL,Length);
      for (int I=0;I<Length;I++)
        /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
        if (!IsSpace(Str[I]))
        /* IS $ */
        {
          PrevLine->SetCurPos(I);
          IndentPos=PrevLine->GetTabCurPos();
          SrcIndent=PrevLine;
          Found=TRUE;
          break;
        }
      if (Found)
        break;
      PrevLine=PrevLine->m_prev;
    }
  }

  int SpaceOnly=TRUE;

  if (CurPos<Length)
  {
    /* $ 30.08.2000 tran
       ��������������� ���, ��� ������.
    */
    /* $ 17.07.2000 tran
       - �������������� ���, ��� �� ������*/
    if (IndentPos>0)
      for (int I=0;I<CurPos;I++)
        if (!IsSpace(CurLineStr[I]))
        {
          SpaceOnly=FALSE;
          break;
        }
    /* tran 30.08.2000 $ */

    NewString->SetBinaryString(&CurLineStr[CurPos],Length-CurPos);
    /* $ 17.07.2000 tran
       ��� �� ��������� ����� ������, ���� �� �� ��� ��� ������ ����� ��������
    */
    for ( int i0=0; i0<Length-CurPos; i0++ )
    {
        if (!IsSpace(CurLineStr[i0+CurPos]))
        {
            NewLineEmpty=FALSE;
            break;
        }
    }
    /* tran 17.07.2000 $ */

    AddUndoData(CurLine->GetStringAddr(),EndList==CurLine?"":CurLine->GetEOL(),NumLine,
                CurLine->GetCurPos(),UNDO_EDIT);
    BlockUndo++;
    AddUndoData(NULL,EndList==CurLine?"":GlobalEOL,NumLine+1,0,UNDO_INSSTR); // EOL? - CurLine->GetEOL()  GlobalEOL   ""
    BlockUndo--;

    char *NewCurLineStr = (char *) xf_malloc(CurPos+1);
    if (!NewCurLineStr)
      return;
    memcpy(NewCurLineStr,CurLineStr,CurPos);
    NewCurLineStr[CurPos]=0;
    int StrSize=CurPos;

    if (EdOpt.AutoIndent && NewLineEmpty)
    {
      RemoveTrailingSpaces(NewCurLineStr);
      StrSize=(int)strlen(NewCurLineStr);
    }

    CurLine->SetBinaryString(NewCurLineStr,StrSize);
    CurLine->SetEOL(EndSeq);

    xf_free(NewCurLineStr);
  }
  else
  {
    NewString->SetString("");
    AddUndoData(NULL,"",NumLine+1,0,UNDO_INSSTR);// EOL? - CurLine->GetEOL()  GlobalEOL   ""
  }

  if (VBlockStart!=NULL && NumLine<VBlockY+VBlockSizeY)
    if (NumLine<VBlockY)
    {
      VBlockY++;
      BlockStartLine++;
    }
    else
      VBlockSizeY++;

  if (SelStart!=-1 && (SelEnd==-1 || CurPos<SelEnd))
  {
    if (CurPos>=SelStart)
    {
      CurLine->Select(SelStart,-1);
      NewString->Select(0,SelEnd==-1 ? -1:SelEnd-CurPos);
    }
    else
    {
      CurLine->Select(-1,0);
      NewString->Select(SelStart-CurPos,SelEnd==-1 ? -1:SelEnd-CurPos);
      BlockStart=NewString;
      BlockStartLine++;
    }
  }
  else
    if (BlockStart!=NULL && NumLine<BlockStartLine)
      BlockStartLine++;

  NewString->SetEOL(EndSeq);

  CurLine->SetCurPos(0);
  if (CurLine==EndList)
    EndList=NewString;
  NumLastLine++;
  Down();

  if (IndentPos>0)
  {
    int OrgIndentPos=IndentPos;
    ShowEditor(FALSE);

    CurLine->GetBinaryString(&CurLineStr,NULL,Length);

    if (SpaceOnly)
    {
      int Decrement=0;
      for (int I=0;I<IndentPos && I<Length;I++)
      {
        if (!IsSpace(CurLineStr[I]))
          break;

        if (CurLineStr[I]==' ')
          Decrement++;
        else
        {
          int TabPos=CurLine->RealPosToTab(I);
          Decrement+=EdOpt.TabSize - (TabPos % EdOpt.TabSize);
        }
      }
      IndentPos-=Decrement;
    }

    if (IndentPos>0)
    {
      if (CurLine->GetLength()!=0 || !EdOpt.CursorBeyondEOL)
      {
        CurLine->ProcessKey(KEY_HOME);

        int SaveOvertypeMode=CurLine->GetOvertypeMode();
        CurLine->SetOvertypeMode(FALSE);

        const char *PrevStr=NULL;
        int PrevLength=0;

        if (SrcIndent)
        {
          SrcIndent->GetBinaryString(&PrevStr,NULL,PrevLength);
        }

        for (int I=0;CurLine->GetTabCurPos()<IndentPos;I++)
        {
          if (SrcIndent!=NULL && I<PrevLength && IsSpace(PrevStr[I]))
          {
            CurLine->ProcessKey(PrevStr[I]);
          }
          else
          {
            CurLine->ProcessKey(KEY_SPACE);
          }
        }
        while (CurLine->GetTabCurPos()>IndentPos)
          CurLine->ProcessKey(KEY_BS);

        CurLine->SetOvertypeMode(SaveOvertypeMode);
      }
      CurLine->SetTabCurPos(IndentPos);
    }

    CurLine->GetBinaryString(&CurLineStr,NULL,Length);
    CurPos=CurLine->GetCurPos();
    if (SpaceOnly)
    {
      int NewPos=0;
      for (int I=0;I<Length;I++)
      {
        NewPos=I;
        if (!IsSpace(CurLineStr[I]))
          break;
      }
      if (NewPos>OrgIndentPos)
        NewPos=OrgIndentPos;
      if (NewPos>CurPos)
        CurLine->SetCurPos(NewPos);
    }
  }

  TextChanged(1);
}



void Editor::Down()
{
  _SVS(CleverSysLog SL("Editor::Down()"));
  //TODO: "�������" - ���� ������ "!Flags.Check(FSCROBJ_VISIBLE)", �� ������� ���� �� ��������� ������� ������
  Edit *CurPtr;
  int LeftPos,CurPos,Y;
  if (CurLine->m_next==NULL)
    return;
  _SVS(SysLog("TopScreen=%p CurLine=%p",TopScreen, CurLine));
  for (Y=0,CurPtr=TopScreen;CurPtr && CurPtr!=CurLine;CurPtr=CurPtr->m_next)
  {
    _SVS(SysLog("  Y=%02d CurPtr=%p",Y,CurPtr));
    Y++;
  }
  _SVS(SysLog("Y=%d Y2(%d)-Y1(%d)=%d",Y,Y2,Y1,Y2-Y1));
  if (Y>=Y2-Y1)
    TopScreen=TopScreen->m_next;
  _SVS(SysLog("TopScreen=%p",TopScreen));
  CurPos=CurLine->GetTabCurPos();
  LeftPos=CurLine->GetLeftPos();
  CurLine=CurLine->m_next;
  NumLine++;
  CurLine->SetLeftPos(LeftPos);
  CurLine->SetTabCurPos(CurPos);
}


void Editor::ScrollDown()
{
  //TODO: "�������" - ���� ������ "!Flags.Check(FSCROBJ_VISIBLE)", �� ������� ���� �� ��������� ������� ������
  int LeftPos,CurPos;
  if (CurLine->m_next==NULL || TopScreen->m_next==NULL)
    return;
  if (!EdOpt.AllowEmptySpaceAfterEof && CalcDistance(TopScreen,EndList,Y2-Y1)<Y2-Y1)
  {
    Down();
    return;
  }
  TopScreen=TopScreen->m_next;
  CurPos=CurLine->GetTabCurPos();
  LeftPos=CurLine->GetLeftPos();
  CurLine=CurLine->m_next;
  NumLine++;
  CurLine->SetLeftPos(LeftPos);
  CurLine->SetTabCurPos(CurPos);
}


void Editor::Up()
{
  //TODO: "�������" - ���� ������ "!Flags.Check(FSCROBJ_VISIBLE)", �� ������� ���� �� ��������� ������� ������
  int LeftPos,CurPos;
  if (CurLine->m_prev==NULL)
    return;

  if (CurLine==TopScreen)
    TopScreen=TopScreen->m_prev;

  CurPos=CurLine->GetTabCurPos();
  LeftPos=CurLine->GetLeftPos();
  CurLine=CurLine->m_prev;
  NumLine--;
  CurLine->SetLeftPos(LeftPos);
  CurLine->SetTabCurPos(CurPos);
}


void Editor::ScrollUp()
{
  //TODO: "�������" - ���� ������ "!Flags.Check(FSCROBJ_VISIBLE)", �� ������� ���� �� ��������� ������� ������
  int LeftPos,CurPos;
  if (CurLine->m_prev==NULL)
    return;
  if (TopScreen->m_prev==NULL)
  {
    Up();
    return;
  }

  TopScreen=TopScreen->m_prev;
  CurPos=CurLine->GetTabCurPos();
  LeftPos=CurLine->GetLeftPos();
  CurLine=CurLine->m_prev;
  NumLine--;
  CurLine->SetLeftPos(LeftPos);
  CurLine->SetTabCurPos(CurPos);
}

/* $ 21.01.2001 SVS
   ������� ������/������ ������� �� Editor::Search
   � ��������� ������� GetSearchReplaceString
   (���� stddlg.cpp)
*/
BOOL Editor::Search(int Next)
{
  Edit *CurPtr,*TmpPtr;
  unsigned char SearchStr[SEARCHSTRINGBUFSIZE],ReplaceStr[SEARCHSTRINGBUFSIZE];
  static char LastReplaceStr[SEARCHSTRINGBUFSIZE];
  static int LastSuccessfulReplaceMode=0;
  char MsgStr[514];
  const char *TextHistoryName="SearchText",*ReplaceHistoryName="ReplaceText";
  /* $ 03.08.2000 KM
     ����� ����������
  */
  int CurPos,Count,Case,WholeWords,ReverseSearch,Match,NewNumLine,UserBreak;
  /* KM $ */
  if (Next && *LastSearchStr==0)
    return TRUE;

  xstrncpy((char *)SearchStr,(char *)LastSearchStr,sizeof(SearchStr)-1);
  xstrncpy((char *)ReplaceStr,(char *)LastReplaceStr,sizeof(ReplaceStr)-1);
  Case=LastSearchCase;
  WholeWords=LastSearchWholeWords;
  ReverseSearch=LastSearchReverse;

  if (!Next)
    if(!GetSearchReplaceString(ReplaceMode,SearchStr,sizeof(SearchStr),
                   ReplaceStr,sizeof(ReplaceStr),
                   TextHistoryName,ReplaceHistoryName,
                   &Case,&WholeWords,&ReverseSearch))
      return FALSE;

  xstrncpy((char *)LastSearchStr,(char *)SearchStr,sizeof(LastSearchStr)-1);
  xstrncpy((char *)LastReplaceStr,(char *)ReplaceStr,sizeof(LastReplaceStr)-1);
  LastSearchCase=Case;
  LastSearchWholeWords=WholeWords;
  LastSearchReverse=ReverseSearch;

  if (*SearchStr==0)
    return TRUE;

  LastSuccessfulReplaceMode=ReplaceMode;

  if (!EdOpt.PersistentBlocks)
    UnmarkBlock();

  {
    //SaveScreen SaveScr;

    int SearchLength=(int)strlen((char *)SearchStr);

    sprintf(MsgStr,"\"%s\"",SearchStr);
    SetCursorType(FALSE,-1);
    SetPreRedrawFunc(Editor::PR_EditorShowMsg);
    EditorShowMsg(MSG(MEditSearchTitle),MSG(MEditSearchingFor),MsgStr);

    Count=0;
    Match=0;
    UserBreak=0;
    CurPos=CurLine->GetCurPos();
    /* $ 16.10.2000 tran
       CurPos ������������� ��� ��������� ������ */
    /* $ 28.11.2000 SVS
       "�, ��� �� ������ - ��� �������� ���� ���������" :-)
       ����� ��������� ����� ��������������
    */
    /* $ 21.12.2000 SVS
       - � ���������� ����������� ���� ������ �������� ������� ���
         ������� EditorF7Rules
    */
    /* $ 10.06.2001 IS
       - ���: �����-�� ��� ����������� _���������_ ������ �������������� �� ���
         _������_.
    */
    /* $ 09.11.2001 IS
         ��������� �����, ����.
         ����� ������, �.�. �� ������������� �����������
    */
    if( !ReverseSearch && ( Next || (EdOpt.F7Rules && !ReplaceMode) ) )
        CurPos++;
    /* IS $ */
    /* IS $ */
    /* SVS $ */
    /* SVS $ */
    /* tran $ */

    NewNumLine=NumLine;
    CurPtr=CurLine;
    BOOL MessageShown=FALSE;

    while (CurPtr!=NULL)
    {
      if ((++Count & 0xfff)==0)
      {
        if( CheckForEscSilent() )
        {
          if ( ConfirmAbortOp() )
          {
            UserBreak=TRUE;
            break;
          }
          MessageShown=FALSE;
        }

        if (!MessageShown)
        {
          sprintf(MsgStr,"\"%s\"",SearchStr);
          SetCursorType(FALSE,-1);
          SetPreRedrawFunc(Editor::PR_EditorShowMsg);
          EditorShowMsg(MSG(MEditSearchTitle),MSG(MEditSearchingFor),MsgStr);
          MessageShown=TRUE;
        }
      }

      /* $ 03.08.2000 KM
         ���������� ������ ��������� � ������� ������
      */
      if (CurPtr->Search((char *)SearchStr,CurPos,Case,WholeWords,ReverseSearch))
      /* KM $ */
      {
        int Skip=FALSE;
        /* $ 24.01.2003 KM
           ! �� ��������� ������ �������� �� ����� ������ ��
             ����� ������������ ������.
        */
        /* $ 15.04.2003 VVM
           �������� �� �������� � �������� �� ���������� �������� ������ */
        int FromTop=(ScrY-2)/4;
        if (FromTop<0 || FromTop>=((ScrY-5)/2-2))
          FromTop=0;
        /* VVM $ */

        TmpPtr=CurLine=CurPtr;
        for (int i=0;i<FromTop;i++)
        {
          if (TmpPtr->m_prev)
            TmpPtr=TmpPtr->m_prev;
          else
            break;
        }
        TopScreen=TmpPtr;
        /* KM $ */

        NumLine=NewNumLine;

        int LeftPos=CurPtr->GetLeftPos();
        int TabCurPos=CurPtr->GetTabCurPos();
        if (ObjWidth>8 && TabCurPos-LeftPos+SearchLength>ObjWidth-8)
          CurPtr->SetLeftPos(TabCurPos+SearchLength-ObjWidth+8);

        if (ReplaceMode)
        {
          int MsgCode=0;
          if (!ReplaceAll)
          {
            Show();
            int CurX,CurY;
            GetCursorPos(CurX,CurY);
            GotoXY(CurX,CurY);
            SetColor(COL_EDITORSELECTEDTEXT);
            const char *Str=CurPtr->GetStringAddr()+CurPtr->GetCurPos();
            char *TmpStr=new char[SearchLength+1];
            xstrncpy(TmpStr,Str,SearchLength);
            TmpStr[SearchLength]=0;
            if (UseDecodeTable)
              DecodeString(TmpStr,(unsigned char *)TableSet.DecodeTable);
            Text(TmpStr);
            delete[] TmpStr;

            char QSearchStr[SEARCHSTRINGBUFSIZE+4],QReplaceStr[SEARCHSTRINGBUFSIZE+4];
            sprintf(QSearchStr,"\"%s\"",LastSearchStr);
            sprintf(QReplaceStr,"\"%s\"",LastReplaceStr);
            MsgCode=Message(0,4,MSG(MEditReplaceTitle),MSG(MEditAskReplace),
              QSearchStr,MSG(MEditAskReplaceWith),QReplaceStr,
              MSG(MEditReplace),MSG(MEditReplaceAll),MSG(MEditSkip),MSG(MEditCancel));
            if (MsgCode==1)
              ReplaceAll=TRUE;
            if (MsgCode==2)
              Skip=TRUE;
            if (MsgCode<0 || MsgCode==3)
            {
              UserBreak=TRUE;
              break;
            }
          }
          if (MsgCode==0 || MsgCode==1)
          {
            Pasting++;
            /*$ 15.08.2000 skv
              If Replace string doesn't contain control symbols (tab and return),
              processed with fast method, otherwise use improved old one.
            */
            if(strchr((char*)ReplaceStr,'\t') || strchr((char*)ReplaceStr,13))
            {
              int SaveOvertypeMode=Flags.Check(FEDITOR_OVERTYPE);
              Flags.Set(FEDITOR_OVERTYPE);
              CurLine->SetOvertypeMode(TRUE);
              //int CurPos=CurLine->GetCurPos();
              int I;
              for (I=0;SearchStr[I]!=0 && ReplaceStr[I]!=0;I++)
              {
                int Ch=ReplaceStr[I];
                if (Ch==KEY_TAB)
                {
                  Flags.Clear(FEDITOR_OVERTYPE);
                  CurLine->SetOvertypeMode(FALSE);
                  ProcessKey(KEY_DEL);
                  ProcessKey(KEY_TAB);
                  Flags.Set(FEDITOR_OVERTYPE);
                  CurLine->SetOvertypeMode(TRUE);
                  continue;
                }
                /* $ 24.05.2002 SKV
                  ���� ��������� �� Enter, �� overtype �� �����.
                  ����� ������� ������� ��, ��� ��������.
                */
                if(Ch==0x0d)
                {
                  ProcessKey(KEY_DEL);
                }
                /* SKV $ */
                if (Ch!=KEY_BS && !(Ch==KEY_DEL || Ch==KEY_NUMDEL))
                  ProcessKey(Ch);
              }
              if(SearchStr[I]==0)
              {
                Flags.Clear(FEDITOR_OVERTYPE);
                CurLine->SetOvertypeMode(FALSE);
                for (;ReplaceStr[I]!=0;I++)
                {
                  int Ch=ReplaceStr[I];
                  if (Ch!=KEY_BS && !(Ch==KEY_DEL || Ch==KEY_NUMDEL))
                    ProcessKey(Ch);
                }
              }else
              {
                for (;SearchStr[I]!=0;I++)
                {
                  ProcessKey(KEY_DEL);
                }
              }
              int Cnt=0;
              char *Tmp=(char*)ReplaceStr;
              while((Tmp=strchr(Tmp,13)) != NULL)
              {
                Cnt++;
                Tmp++;
              }
              if(Cnt>0)
              {
                CurPtr=CurLine;
                NewNumLine+=Cnt;
              }
              Flags.Change(FEDITOR_OVERTYPE,SaveOvertypeMode);
            }
            else
            {
              /* Fast method */
              const char *Str,*Eol;
              int StrLen,NewStrLen;
              int SStrLen=(int)strlen((char*)SearchStr),
                  RStrLen=(int)strlen((char*)ReplaceStr);
              CurLine->GetBinaryString(&Str,&Eol,StrLen);
              int EolLen=(int)strlen((char*)Eol);
              NewStrLen=StrLen;
              NewStrLen-=SStrLen;
              NewStrLen+=RStrLen;
              NewStrLen+=EolLen;
              char *NewStr=new char[NewStrLen+1];
              int CurPos=CurLine->GetCurPos();
              memcpy(NewStr,Str,CurPos);
              memcpy(NewStr+CurPos,ReplaceStr,RStrLen);
              /*$ 07.09.2000 skv
                If table set need to encode string.
              */
              if(UseDecodeTable)
              {
                EncodeString(NewStr+CurPos,(unsigned char*)TableSet.EncodeTable,RStrLen);
              }
              /* skv$*/
              memcpy(NewStr+CurPos+RStrLen,Str+CurPos+SStrLen,StrLen-CurPos-SStrLen);
              memcpy(NewStr+NewStrLen-EolLen,Eol,EolLen);
              AddUndoData(CurLine->GetStringAddr(),CurLine->GetEOL(),NumLine,
                          CurLine->GetCurPos(),UNDO_EDIT);
              CurLine->SetBinaryString(NewStr,NewStrLen);
              CurLine->SetCurPos(CurPos+RStrLen);
              delete [] NewStr;

              TextChanged(1);
            }
            /* skv$*/

            //AY: � ���� ��� ������� ���������� � ��� �������� � �� �����������
            //���������������� ��� Replace
            //if (ReverseSearch)
              //CurLine->SetCurPos(CurPos);

            Pasting--;
          }
        }
        Match=1;
        if (!ReplaceMode)
          break;
        CurPos=CurLine->GetCurPos();
        if (Skip)
          if (!ReverseSearch)
            CurPos++;
      }
      else
        if (ReverseSearch)
        {
          CurPtr=CurPtr->m_prev;
          if (CurPtr==NULL)
            break;
          CurPos=CurPtr->GetLength();
          NewNumLine--;
        }
        else
        {
          CurPos=0;
          CurPtr=CurPtr->m_next;
          NewNumLine++;
        }
    }
    SetPreRedrawFunc(NULL);
  }

  Show();
  if (!Match && !UserBreak)
    Message(MSG_DOWN|MSG_WARNING,1,MSG(MEditSearchTitle),MSG(MEditNotFound),
            MsgStr,MSG(MOk));
   return TRUE;
}
/* SVS $ */

void Editor::Paste(char *Src)
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;

  char *ClipText=Src;
  BOOL IsDeleteClipText=FALSE;

  if(!ClipText)
  {
    if ((ClipText=PasteFormatFromClipboard(FAR_VerticalBlock))!=NULL)
    {
      VPaste(ClipText);
      return;
    }
    if ((ClipText=PasteFromClipboard())==NULL)
      return;
    IsDeleteClipText=TRUE;
  }

  if (*ClipText)
  {
    Flags.Set(FEDITOR_NEWUNDO);
    if (UseDecodeTable)
      EncodeString(ClipText,(unsigned char *)TableSet.EncodeTable);
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/
    int SaveOvertype=Flags.Check(FEDITOR_OVERTYPE);
    UnmarkBlock();
    Pasting++;
    Lock ();

    if (Flags.Check(FEDITOR_OVERTYPE))
    {
      Flags.Clear(FEDITOR_OVERTYPE);
      CurLine->SetOvertypeMode(FALSE);
    }
    BlockStart=CurLine;
    BlockStartLine=NumLine;
    /* $ 19.05.2001 IS
       ������� �������� ���������� ����������� ��������� (������� ������ ����
       ��������� � ������ ������ ��� �����������) � �������.
    */
    int StartPos=CurLine->GetCurPos(),
        oldAutoIndent=EdOpt.AutoIndent;

    for (int I=0;ClipText[I]!=0;)
    {
      if (ClipText[I]==10 || ClipText[I]==13)
      {
        CurLine->Select(StartPos,-1);
        StartPos=0;
        EdOpt.AutoIndent=FALSE;
        ProcessKey(KEY_ENTER);
        BlockUndo=TRUE;
        if (ClipText[I]==13 && ClipText[I+1]==10)
          I++;
        I++;
      }
      else
      {
        if(EdOpt.AutoIndent)       // ������ ������ ������� ���, �����
        {                          // �������� ����������
          /* $ 25.05.2001 IS
                ��������� ���������� ������� � ���� � ���������,
                �������� �� oem
          */
          ProcessKey(UseDecodeTable?TableSet.DecodeTable[(unsigned)ClipText[I]]:ClipText[I]);
          /* IS $ */
          I++;
          StartPos=CurLine->GetCurPos();
          if(StartPos) StartPos--;
        }

        int Pos=I;
        while (ClipText[Pos]!=0 && ClipText[Pos]!=10 && ClipText[Pos]!=13)
          Pos++;
        if (Pos>I)
        {
          const char *Str;
          int Length,CurPos;
          CurLine->GetBinaryString(&Str,NULL,Length);
          CurPos=CurLine->GetCurPos();
          AddUndoData(Str,CurLine->GetEOL(),NumLine,CurPos,UNDO_EDIT); // EOL? - CurLine->GetEOL()  GlobalEOL   ""
          BlockUndo=TRUE;
          CurLine->InsertBinaryString(&ClipText[I],Pos-I);
        }
        I=Pos;
      }
    }

    EdOpt.AutoIndent=oldAutoIndent;

    CurLine->Select(StartPos,CurLine->GetCurPos());
    /* IS $ */

    if (SaveOvertype)
    {
      Flags.Set(FEDITOR_OVERTYPE);
      CurLine->SetOvertypeMode(TRUE);
    }

    Pasting--;
    Unlock ();
  }
  /* $ 07.05.2001 IS �������� �� � PasteFromClipboard ��� new [] */
  if(IsDeleteClipText)
    delete [] ClipText;
  /* IS $ */
  BlockUndo=FALSE;
}


void Editor::Copy(int Append)
{
  if (VBlockStart!=NULL)
  {
    VCopy(Append);
    return;
  }

  Edit *CurPtr=BlockStart;
  char *CopyData=NULL;
  long DataSize=0,PrevSize=0;

  if (Append)
  {
    CopyData=PasteFromClipboard();
    if (CopyData!=NULL)
      PrevSize=DataSize=(long)strlen(CopyData);
  }

  while (CurPtr!=NULL)
  {
    int StartSel,EndSel;
    int Length=CurPtr->GetLength()+1;
    CurPtr->GetSelection(StartSel,EndSel);
    if (StartSel==-1)
      break;
    char *NewPtr=(char *)xf_realloc(CopyData,DataSize+Length+2);
    if (NewPtr==NULL)
    {
      delete CopyData;
      CopyData=NULL;
      break;
    }
    CopyData=NewPtr;
    CurPtr->GetSelString(CopyData+DataSize,Length);
    DataSize+=(long)strlen(CopyData+DataSize);
    if (EndSel==-1)
    {
      strcpy(CopyData+DataSize,DOS_EOL_fmt);
      DataSize+=2;
    }
    CurPtr=CurPtr->m_next;
  }

  if (CopyData!=NULL)
  {
    if (UseDecodeTable)
      DecodeString(CopyData+PrevSize,(unsigned char *)TableSet.DecodeTable);
    CopyToClipboard(CopyData);
    delete CopyData;
  }
}


void Editor::DeleteBlock()
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;

  if (VBlockStart!=NULL)
  {
    DeleteVBlock();
    return;
  }

  Edit *CurPtr=BlockStart;

  int UndoNext=FALSE;

  while (CurPtr!=NULL)
  {
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/
    int StartSel,EndSel;
    /* $ 17.09.2002 SKV
      ������ �� Real ��� � ������ ��������� �� ������ ������.
    */
    CurPtr->GetRealSelection(StartSel,EndSel);
    if(EndSel!=-1 && EndSel>CurPtr->GetLength())
      EndSel=-1;
    /* SKV $ */
    if (StartSel==-1)
      break;
    if (StartSel==0 && EndSel==-1)
    {
      Edit *NextLine=CurPtr->m_next;
      BlockUndo=UndoNext;
      DeleteString(CurPtr,FALSE,BlockStartLine);
      UndoNext=TRUE;
      if (BlockStartLine<NumLine)
        NumLine--;
      if (NextLine!=NULL)
      {
        CurPtr=NextLine;
        continue;
      }
      else
        break;
    }
    int Length=CurPtr->GetLength();
    if (StartSel!=0 || EndSel!=0)
    {
      BlockUndo=UndoNext;
      AddUndoData(CurPtr->GetStringAddr(),CurPtr->GetEOL(),BlockStartLine,
                  CurPtr->GetCurPos(),UNDO_EDIT);
      UndoNext=TRUE;
    }
    /* $ 17.09.2002 SKV
      ����� ��� ��������� �� ������ ������.
      InsertBinaryString ������� trailing space'��
    */
    if(StartSel>Length)
    {
      Length=StartSel;
      CurPtr->SetCurPos(Length);
      CurPtr->InsertBinaryString("",0);
    }
    /* SKV $ */
    const char *CurStr,*EndSeq;
    CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);
    // ������ ����� realloc, ������� ��� malloc.
    char *TmpStr=(char*)xf_malloc(Length+3);
    memcpy(TmpStr,CurStr,Length);
    TmpStr[Length]=0;
    int DeleteNext=FALSE;
    if (EndSel==-1)
    {
      EndSel=Length;
      if (CurPtr->m_next!=NULL)
        DeleteNext=TRUE;
    }
    memmove(TmpStr+StartSel,TmpStr+EndSel,strlen(TmpStr+EndSel)+1);
    int CurPos=StartSel;
/*    if (CurPos>=StartSel)
    {
      CurPos-=(EndSel-StartSel);
      if (CurPos<StartSel)
        CurPos=StartSel;
    }
*/
    Length-=EndSel-StartSel;
    if (DeleteNext)
    {
      const char *NextStr,*EndSeq;
      int NextLength,NextStartSel,NextEndSel;
      CurPtr->m_next->GetSelection(NextStartSel,NextEndSel);
      if (NextStartSel==-1)
        NextEndSel=0;
      if (NextEndSel==-1)
        EndSel=-1;
      else
      {
        CurPtr->m_next->GetBinaryString(&NextStr,&EndSeq,NextLength);
        NextLength-=NextEndSel;
        TmpStr=(char *)xf_realloc(TmpStr,Length+NextLength+3);
        memcpy(TmpStr+Length,NextStr+NextEndSel,NextLength);
        Length+=NextLength;
      }
      if (CurLine==CurPtr->m_next)
      {
        CurLine=CurPtr;
        NumLine--;
      }

      BlockUndo=UndoNext;
      if (CurLine==CurPtr && CurPtr->m_next!=NULL && CurPtr->m_next==TopScreen)
      {
        CurLine=CurPtr->m_next;
        NumLine++;
      }
      DeleteString(CurPtr->m_next,FALSE,BlockStartLine+1);
      UndoNext=TRUE;
      if (BlockStartLine+1<NumLine)
        NumLine--;
    }
    int EndLength=(int)strlen(EndSeq);
    memcpy(TmpStr+Length,EndSeq,EndLength);
    Length+=EndLength;
    TmpStr[Length]=0;
    CurPtr->SetBinaryString(TmpStr,Length);
    xf_free(TmpStr);
    CurPtr->SetCurPos(CurPos);
    if (DeleteNext && EndSel==-1)
    {
      CurPtr->Select(CurPtr->GetLength(),-1);
    }
    else
    {
      CurPtr->Select(-1,0);
      CurPtr=CurPtr->m_next;
      BlockStartLine++;
    }
  }
  BlockStart=NULL;
  BlockUndo=FALSE;
}


void Editor::UnmarkBlock()
{
  if (BlockStart==NULL && VBlockStart==NULL)
    return;
  VBlockStart=NULL;
  _SVS(SysLog("[%d] Editor::UnmarkBlock()",__LINE__));
  Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
  while (BlockStart!=NULL)
  {
    int StartSel,EndSel;
    BlockStart->GetSelection(StartSel,EndSel);
    if (StartSel==-1)
    {
      /* $ 24.06.2002 SKV
        ���� � ������� ������ ��� ���������,
        ��� ��� �� ������ ��� �� � �����.
        ��� ����� ���� ������ ������ :)
      */
      if(BlockStart->m_next)
      {
        BlockStart->m_next->GetSelection(StartSel,EndSel);
        if(StartSel==-1)
        {
          break;
        }
      }else break;
      /* SKV $ */
    }
    BlockStart->Select(-1,0);
    BlockStart=BlockStart->m_next;
  }
  BlockStart=NULL;
  Show();
}

void Editor::UnmarkEmptyBlock()
{
  _SVS(SysLog("[%d] Editor::UnmarkEmptyBlock()",__LINE__));
  if(BlockStart || VBlockStart)  // ������������ ���������
  {
    int Lines=0,StartSel,EndSel;
    Edit *Block=BlockStart;
    if(VBlockStart)
    {
      if(VBlockSizeX)
        Lines=VBlockSizeY;
    }
    else while(Block) // ��������� �� ���� ���������� �������
    {
      Block->GetRealSelection(StartSel,EndSel);
      if (StartSel==-1)
        break;
      if(StartSel!=EndSel)// �������� �������-�� ��������
      {
        ++Lines;           // �������� ������� �������� �����
        break;
      }
      Block=Block->m_next;
    }
    if(!Lines)             // ���� �������� ���� �������� � ������, ��
      UnmarkBlock();       // ���������� �������� ������ � ������ ���������
  }
}

void Editor::GoToLine(int Line)
{
  if (Line != NumLine)
  {
    bool bReverse = false;
    int LastNumLine=NumLine;
    int CurScrLine=CalcDistance(TopScreen,CurLine,-1);

    if (Line < NumLine)
    {
      if (Line > NumLine/2)
      {
        bReverse = true;
      }
      else
      {
        CurLine = TopList;
        NumLine = 0;
      }
    }
    else
    {
      if (Line > (NumLine + (NumLastLine-NumLine)/2))
      {
        bReverse = true;
        CurLine = EndList;
        NumLine = NumLastLine-1;
      }
    }

    if (bReverse)
    {
      for ( ; NumLine>Line && CurLine->m_prev!=NULL; NumLine--)
        CurLine=CurLine->m_prev;
    }
    else
    {
      for ( ; NumLine<Line && CurLine->m_next!=NULL; NumLine++)
        CurLine=CurLine->m_next;
    }

    CurScrLine+=NumLine-LastNumLine;

    if (CurScrLine<0 || CurScrLine>Y2-Y1)
      TopScreen=CurLine;
  }

// <GOTO_UNMARK:2>
//  if (!EdOpt.PersistentBlocks)
//     UnmarkBlock();
// </GOTO_UNMARK>

  Show();
  return;
}

void Editor::GoToPosition()
{
  int NewLine, NewCol;
  int LeftPos=CurLine->GetTabCurPos()+1;
  int CurPos;
  CurPos=CurLine->GetCurPos();

  const char *LineHistoryName="LineNumber";
  static struct DialogData GoToDlgData[]=
  {
    DI_DOUBLEBOX,3,1,21,3,0,0,0,0,(char *)MEditGoToLine,
    DI_EDIT,5,2,19,2,1,(DWORD_PTR)LineHistoryName,DIF_HISTORY|DIF_USELASTHISTORY|DIF_NOAUTOCOMPLETE,1,"",
  };
  MakeDialogItems(GoToDlgData,GoToDlg);
  /* $ 01.08.2000 tran
    PrevLine ������ �� ����� - USELASTHISTORY ����� */
  //  static char PrevLine[40]={0};

  //  strcpy(GoToDlg[1].Data,PrevLine);
  Dialog Dlg(GoToDlg,sizeof(GoToDlg)/sizeof(GoToDlg[0]));
  Dlg.SetPosition(-1,-1,25,5);
  Dlg.SetHelp("EditorGotoPos");
  Dlg.Process();

  /* $ 06.05.2002 KM
      ������� ShadowSaveScr ��� �������������� ���������
      �����������.
  */
  Dialog::SendDlgMessage((HANDLE)&Dlg,DM_KILLSAVESCREEN,0,0);
  /* KM $ */

    // tran: was if (Dlg.GetExitCode()!=1 || !isdigit(*GoToDlg[1].Data))
  if (Dlg.GetExitCode()!=1 )
      return ;
  // �������� ����� ��������� �������� � ������� ������ ������ FAR`�
  //  xstrncpy(PrevLine,GoToDlg[1].Data,sizeof(PrevLine));

  GetRowCol(GoToDlg[1].Data,&NewLine,&NewCol);

  //_D(SysLog("GoToPosition: NewLine=%i, NewCol=%i",NewLine,NewCol));
  GoToLine(NewLine);

  if ( NewCol == -1)
  {
    CurLine->SetTabCurPos(CurPos);
    CurLine->SetLeftPos(LeftPos);
  }
  else
    CurLine->SetTabCurPos(NewCol);

// <GOTO_UNMARK:3>
//  if (!EdOpt.PersistentBlocks)
//     UnmarkBlock();
// </GOTO_UNMARK>

  Show();
  return ;
}


void Editor::GetRowCol(char *argv,int *row,int *col)
{
  int x=0xffff,y,l;
  char *argvx=0;
  int LeftPos=CurLine->GetTabCurPos() + 1;

  // ��� �� �� �������� "�����" ������ - ������ ��, ��� �� ����� ;-)
  // "�������" ��� ������� �������.
  RemoveExternalSpaces(argv);

  // �������� ������ ��������� ������ �����������
  // � ������� ������
  l=(int)strcspn(argv,",:;. ");
  // ���� ����������� ����, �� l=strlen(argv)

  if(l < static_cast<int>(strlen(argv))) // ��������: "row,col" ��� ",col"?
  {
    argv[l]='\0'; // ������ ����������� ��������� "����� ������" :-)
    argvx=argv+l+1;
    x=atoi(argvx);
  }
  y=atoi(argv);
  /* $ 14.07.2000 tran
    + ������� �� �������� */
  if ( strchr(argv,'%')!=0 )
    y=NumLastLine * y / 100;
  /* tran $ */

  /* $ 21.07.2000 tran
     ��������� ��������������� */
  if ( argv[0]=='-' || argv[0]=='+' )
    y=NumLine+y+1;
  if ( argvx )
  {
    if ( argvx[0]=='-' || argvx[0]=='+' )
    {
        x=LeftPos+x;
    }
  }

  /* tran 21.07.2000 $ */

  // ������ ������� ��������� �����
  *row=y;
  if ( x!=0xffff )
    *col=x;
  else
    /* $ 28.03.2001 VVM
      ! �����-�� ���������� 1. � ���������� ������... */
    *col=LeftPos;
    /* VVM $ */


  (*row)--;
  if (*row< 0)   // ���� ����� ",Col"
     *row=NumLine;  //   �� ��������� �� ������� ������ � �������
  (*col)--;
  if (*col< -1)
     *col=-1;
  return ;
}

void Editor::AddUndoData(const char *Str,const char *Eol,int StrNum,int StrPos,int Type)
{
  int PrevUndoDataPos;

  if (Flags.Check(FEDITOR_DISABLEUNDO) || !UndoData) // ��� ���� ������� Undo?
    return;

  if (StrNum==-1)    // ������� �����?
    StrNum=NumLine;

  if ((PrevUndoDataPos=UndoDataPos-1)<0)
    PrevUndoDataPos=EdOpt.UndoSize-1;

  if (!Flags.Check(FEDITOR_NEWUNDO) &&
      Type==UNDO_EDIT &&
      UndoData[PrevUndoDataPos].Type==UNDO_EDIT &&
      StrNum==UndoData[PrevUndoDataPos].StrNum &&
      (
        abs(StrPos-UndoData[PrevUndoDataPos].StrPos)<=1 ||
        abs(StrPos-LastChangeStrPos)<=1)
      )
  {
    LastChangeStrPos=StrPos;
    return;
  }

  Flags.Clear(FEDITOR_NEWUNDO);
  if (UndoData[UndoDataPos].Type!=UNDO_NONE && UndoData[UndoDataPos].Str!=NULL)
    delete[] UndoData[UndoDataPos].Str;

  UndoData[UndoDataPos].Type=Type;
  UndoData[UndoDataPos].UndoNext=BlockUndo;
  UndoData[UndoDataPos].StrPos=StrPos;
  UndoData[UndoDataPos].StrNum=StrNum;
  xstrncpy(UndoData[UndoDataPos].EOL,Eol?Eol:"",sizeof(UndoData[UndoDataPos].EOL)-1);

  if (Str!=NULL)
  {
    UndoData[UndoDataPos].Str=new char[strlen(Str)+1];
    if (UndoData[UndoDataPos].Str!=NULL)
      strcpy(UndoData[UndoDataPos].Str,Str);
  }
  else
    UndoData[UndoDataPos].Str=NULL;

  if (++UndoDataPos==EdOpt.UndoSize)
    UndoDataPos=0;

  if (UndoDataPos==UndoSavePos)
    Flags.Set(FEDITOR_UNDOOVERFLOW);
}

void Editor::Undo()
{
  if(!UndoData)
    return;

  int NewPos=UndoDataPos-1;

  if (NewPos<0)
    NewPos=EdOpt.UndoSize-1;

  if (UndoData[NewPos].Type==UNDO_NONE)
    return;

  UnmarkBlock();
  UndoDataPos=NewPos;
  TextChanged(1);
  /* $ 30.03.2002 IS
     ������ ���� ������������ FEDITOR_WASCHANGED, �.�. ��� �����, ��� � �����,
     ������ ���������� ����� ��� ��������� ����� ��������������� _�� �����_, �
     �� ��������� � ����� �������� FEDITOR_MODIFIED. ��������� � �����
     ������������� � ������ �����, �� �� ��������������� ���������, ������ ���
     ��� ��������������� � "TextChanged(1)" - ��. ����.
  */
  Flags.Set(/*FEDITOR_WASCHANGED|*/FEDITOR_DISABLEUNDO);

  GoToLine(UndoData[UndoDataPos].StrNum);

  switch(UndoData[UndoDataPos].Type)
  {
    case UNDO_INSSTR:
      DeleteString(CurLine,TRUE,NumLine>0 ? NumLine-1:NumLine);
      break;

    case UNDO_DELSTR:
      Pasting++;
      if (NumLine<UndoData[UndoDataPos].StrNum)
      {
        ProcessKey(KEY_END);
        ProcessKey(KEY_ENTER);
      }
      else
      {
        ProcessKey(KEY_HOME);
        ProcessKey(KEY_ENTER);
        ProcessKey(KEY_UP);
      }
      Pasting--;
      if (UndoData[UndoDataPos].Str!=NULL)
      {
        CurLine->SetString(UndoData[UndoDataPos].Str);
        CurLine->SetEOL(UndoData[UndoDataPos].EOL); // ���������� ������������� ����������, �.�. SetString �������� Edit::SetBinaryString �... ������ �� ������
      }
      break;

    case UNDO_EDIT:
      if (UndoData[UndoDataPos].Str!=NULL)
      {
        CurLine->SetString(UndoData[UndoDataPos].Str);
        CurLine->SetEOL(UndoData[UndoDataPos].EOL);
      }
      CurLine->SetCurPos(UndoData[UndoDataPos].StrPos);
      break;
  }

  if (UndoData[UndoDataPos].Str!=NULL)
    delete[] UndoData[UndoDataPos].Str;

  UndoData[UndoDataPos].Type=UNDO_NONE;

  if (UndoData[UndoDataPos].UndoNext)
    Undo();

  if (!Flags.Check(FEDITOR_UNDOOVERFLOW) && UndoDataPos==UndoSavePos)
    TextChanged(0);

  Flags.Clear(FEDITOR_DISABLEUNDO);
}

void Editor::SelectAll()
{
  Edit *CurPtr;
  BlockStart=TopList;
  BlockStartLine=0;
  for (CurPtr=TopList;CurPtr!=NULL;CurPtr=CurPtr->m_next)
    if (CurPtr->m_next!=NULL)
      CurPtr->Select(0,-1);
    else
      CurPtr->Select(0,CurPtr->GetLength());
  Show();
}


void Editor::SetStartPos(int LineNum,int CharNum)
{
  StartLine=LineNum==0 ? 1:LineNum;
  StartChar=CharNum==0 ? 1:CharNum;
}


int Editor::IsFileChanged()
{
  return(Flags.Check(FEDITOR_MODIFIED|FEDITOR_WASCHANGED));
}


int Editor::IsFileModified()
{
  return(Flags.Check(FEDITOR_MODIFIED));
}

// ������������ � FileEditor
long Editor::GetCurPos()
{
  Edit *CurPtr=TopList;
  long TotalSize=0;
  while (CurPtr!=TopScreen)
  {
    const char *SaveStr,*EndSeq;
    int Length;
    CurPtr->GetBinaryString(&SaveStr,&EndSeq,Length);
    TotalSize+=Length+(int)strlen(EndSeq);
    CurPtr=CurPtr->m_next;
  }
  return(TotalSize);
}


void Editor::SetStringsTable()
{
  Edit *CurPtr=TopList;
  while (CurPtr!=NULL)
  {
    CurPtr->SetTables(UseDecodeTable ? &TableSet:NULL);
    CurPtr=CurPtr->m_next;
  }
}


void Editor::BlockLeft()
{
  if (VBlockStart!=NULL)
  {
    VBlockShift(TRUE);
    return;
  }
  Edit *CurPtr=BlockStart;
  int LineNum=BlockStartLine;
/* $ 14.02.2001 VVM
    + ��� ���������� ����� AltU/AltI �������� ������� ������� */
  int MoveLine = 0;
  if (CurPtr==NULL)
  {
    MoveLine = 1;
    CurPtr = CurLine;
    LineNum = NumLine;
  }
/* VVM $ */
  while (CurPtr!=NULL)
  {
    int StartSel,EndSel;
    CurPtr->GetSelection(StartSel,EndSel);
    /* $ 14.02.2001 VVM
      + ����� ��� - ������� ��� ������������ */
    if (MoveLine) {
      StartSel = 0; EndSel = -1;
    }
    /* VVM $ */
    if (StartSel==-1)
      break;

    int Length=CurPtr->GetLength();
    char *TmpStr=new char[Length+EdOpt.TabSize+5];

    const char *CurStr,*EndSeq;
    CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);

    Length--;
    if (*CurStr==' ')
      memcpy(TmpStr,CurStr+1,Length);
    else
      if (*CurStr=='\t')
      {
        memset(TmpStr,' ',EdOpt.TabSize-1);
        memcpy(TmpStr+EdOpt.TabSize-1,CurStr+1,Length);
        Length+=EdOpt.TabSize-1;
      }

    /* $ 24.07.2001 IS IsSpace ��� ����� � ��������� */
    if ((EndSel==-1 || EndSel>StartSel) && IsSpace(*CurStr))
    /* IS $ */
    {
      int EndLength=(int)strlen(EndSeq);
      memcpy(TmpStr+Length,EndSeq,EndLength);
      Length+=EndLength;
      TmpStr[Length]=0;
      AddUndoData(CurStr,CurPtr->GetEOL(),LineNum,0,UNDO_EDIT);// EOL? - CurLine->GetEOL()  GlobalEOL   ""
      BlockUndo=TRUE;
      int CurPos=CurPtr->GetCurPos();
      CurPtr->SetBinaryString(TmpStr,Length);
      CurPtr->SetCurPos(CurPos>0 ? CurPos-1:CurPos);
      /* $ 14.02.2001 VVM
        + �������� ������ ���� ������� ���� */
      if (!MoveLine)
      /* VVM $ */
        CurPtr->Select(StartSel>0 ? StartSel-1:StartSel,EndSel>0 ? EndSel-1:EndSel);
      /*$ 10.08.2000 skv
        Modified->TextChanged
      */
      TextChanged(1);
      /* skv $*/
    }

    delete[] TmpStr;
    CurPtr=CurPtr->m_next;
    LineNum++;
    MoveLine = 0;
  }
  BlockUndo=FALSE;
}


void Editor::BlockRight()
{
  if (VBlockStart!=NULL)
  {
    VBlockShift(FALSE);
    return;
  }
  Edit *CurPtr=BlockStart;
  int LineNum=BlockStartLine;
/* $ 14.02.2001 VVM
    + ��� ���������� ����� AltU/AltI �������� ������� ������� */
  int MoveLine = 0;
  if (CurPtr==NULL)
  {
    MoveLine = 1;
    CurPtr = CurLine;
    LineNum = NumLine;
  }
/* VVM $ */
  while (CurPtr!=NULL)
  {
    int StartSel,EndSel;
    CurPtr->GetSelection(StartSel,EndSel);
    /* $ 14.02.2001 VVM
      + ����� ��� - ������� ��� ������������ */
    if (MoveLine) {
      StartSel = 0; EndSel = -1;
    }
    /* VVM $ */
    if (StartSel==-1)
      break;

    int Length=CurPtr->GetLength();
    char *TmpStr=new char[Length+5];

    const char *CurStr,*EndSeq;
    CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);
    *TmpStr=' ';
    memcpy(TmpStr+1,CurStr,Length);
    Length++;

    if (EndSel==-1 || EndSel>StartSel)
    {
      int EndLength=(int)strlen(EndSeq);
      memcpy(TmpStr+Length,EndSeq,EndLength);
      TmpStr[Length+EndLength]=0;
      AddUndoData(CurStr,CurPtr->GetEOL(),LineNum,0,UNDO_EDIT);// EOL? - CurLine->GetEOL()  GlobalEOL   ""
      BlockUndo=TRUE;
      int CurPos=CurPtr->GetCurPos();
      if (Length>1)
        CurPtr->SetBinaryString(TmpStr,Length+EndLength);
      CurPtr->SetCurPos(CurPos+1);
      /* $ 14.02.2001 VVM
        + �������� ������ ���� ������� ���� */
      if (!MoveLine)
      /* VVM $ */
        CurPtr->Select(StartSel>0 ? StartSel+1:StartSel,EndSel>0 ? EndSel+1:EndSel);
      /*$ 10.08.2000 skv
        Modified->TextChanged
      */
      TextChanged(1);
      /* skv $*/
    }

    delete[] TmpStr;
    CurPtr=CurPtr->m_next;
    LineNum++;
    MoveLine = 0;
  }
  BlockUndo=FALSE;
}


void Editor::DeleteVBlock()
{
  if (Flags.Check(FEDITOR_LOCKMODE) || VBlockSizeX<=0 || VBlockSizeY<=0)
    return;

  int UndoNext=FALSE;

  if (!EdOpt.PersistentBlocks)
  {
    Edit *CurPtr=CurLine;
    Edit *NewTopScreen=TopScreen;
    while (CurPtr!=NULL)
    {
      if (CurPtr==VBlockStart)
      {
        TopScreen=NewTopScreen;
        CurLine=CurPtr;
        CurPtr->SetTabCurPos(VBlockX);
        break;
      }
      NumLine--;
      if (NewTopScreen==CurPtr && CurPtr->m_prev!=NULL)
        NewTopScreen=CurPtr->m_prev;
      CurPtr=CurPtr->m_prev;
    }
  }

  Edit *CurPtr=VBlockStart;

  for (int Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->m_next)
  {
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/

    int TBlockX=CurPtr->TabPosToReal(VBlockX);
    int TBlockSizeX=CurPtr->TabPosToReal(VBlockX+VBlockSizeX)-
                    CurPtr->TabPosToReal(VBlockX);

    const char *CurStr,*EndSeq;
    int Length;
    CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);
    if (TBlockX>=Length)
      continue;

    BlockUndo=UndoNext;
    AddUndoData(CurPtr->GetStringAddr(),CurPtr->GetEOL(),BlockStartLine+Line,
                CurPtr->GetCurPos(),UNDO_EDIT);
    UndoNext=TRUE;

    char *TmpStr=new char[Length+3];
    int CurLength=TBlockX;
    memcpy(TmpStr,CurStr,TBlockX);
    if (Length>TBlockX+TBlockSizeX)
    {
      int CopySize=Length-(TBlockX+TBlockSizeX);
      memcpy(TmpStr+CurLength,CurStr+TBlockX+TBlockSizeX,CopySize);
      CurLength+=CopySize;
    }
    int EndLength=(int)strlen(EndSeq);
    memcpy(TmpStr+CurLength,EndSeq,EndLength);
    CurLength+=EndLength;
    TmpStr[CurLength]=0;

    int CurPos=CurPtr->GetCurPos();
    CurPtr->SetBinaryString(TmpStr,CurLength);
    if (CurPos>TBlockX)
    {
      CurPos-=TBlockSizeX;
      if (CurPos<TBlockX)
        CurPos=TBlockX;
    }
    CurPtr->SetCurPos(CurPos);
    delete[] TmpStr;
  }

  VBlockStart=NULL;
  BlockUndo=FALSE;
}

void Editor::VCopy(int Append)
{
  Edit *CurPtr=VBlockStart;
  char *CopyData=NULL;
  long DataSize=0,PrevSize=0;

  if (Append)
  {
    CopyData=PasteFormatFromClipboard(FAR_VerticalBlock);
    if (CopyData!=NULL)
      PrevSize=DataSize=(long)strlen(CopyData);
    else
    {
      CopyData=PasteFromClipboard();
      if (CopyData!=NULL)
        PrevSize=DataSize=(long)strlen(CopyData);
    }
  }

  for (int Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->m_next)
  {
    int TBlockX=CurPtr->TabPosToReal(VBlockX);
    int TBlockSizeX=CurPtr->TabPosToReal(VBlockX+VBlockSizeX)-
                    CurPtr->TabPosToReal(VBlockX);
    const char *CurStr,*EndSeq;
    int Length;
    CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);

    int AllocSize=Max(DataSize+Length+3,DataSize+TBlockSizeX+3);
    char *NewPtr=(char *)xf_realloc(CopyData,AllocSize);
    if (NewPtr==NULL)
    {
      delete CopyData;
      CopyData=NULL;
      break;
    }
    CopyData=NewPtr;

    if (Length>TBlockX)
    {
      int CopySize=Length-TBlockX;
      if (CopySize>TBlockSizeX)
        CopySize=TBlockSizeX;
      memcpy(CopyData+DataSize,CurStr+TBlockX,CopySize);
      if (CopySize<TBlockSizeX)
        memset(CopyData+DataSize+CopySize,' ',TBlockSizeX-CopySize);
    }
    else
      memset(CopyData+DataSize,' ',TBlockSizeX);

    DataSize+=TBlockSizeX;


    strcpy(CopyData+DataSize,DOS_EOL_fmt);
    DataSize+=2;
  }

  if (CopyData!=NULL)
  {
    if (UseDecodeTable)
      DecodeString(CopyData+PrevSize,(unsigned char *)TableSet.DecodeTable);
    CopyToClipboard(CopyData);
    CopyFormatToClipboard(FAR_VerticalBlock,CopyData);
    delete CopyData;
  }
}

void Editor::VPaste(char *ClipText)
{
  if (Flags.Check(FEDITOR_LOCKMODE))
    return;

  if (*ClipText)
  {
    Flags.Set(FEDITOR_NEWUNDO);
    TextChanged(1);
    int SaveOvertype=Flags.Check(FEDITOR_OVERTYPE);
    UnmarkBlock();
    Pasting++;
    Lock ();

    if (Flags.Check(FEDITOR_OVERTYPE))
    {
      Flags.Clear(FEDITOR_OVERTYPE);
      CurLine->SetOvertypeMode(FALSE);
    }

    VBlockStart=CurLine;
    BlockStartLine=NumLine;

    int StartPos=CurLine->GetTabCurPos();

    VBlockX=StartPos;
    VBlockSizeX=0;
    VBlockY=NumLine;
    VBlockSizeY=0;

    Edit *SavedTopScreen=TopScreen;


    for (int I=0;ClipText[I]!=0;I++)
      if (ClipText[I]!=13 && ClipText[I+1]!=10)
        ProcessKey(ClipText[I]);
      else
      {
        BlockUndo=TRUE;
        int CurWidth=CurLine->GetTabCurPos()-StartPos;
        if (CurWidth>VBlockSizeX)
          VBlockSizeX=CurWidth;
        VBlockSizeY++;
        if (CurLine->m_next==NULL)
        {
          if (ClipText[I+2]!=0)
          {
            ProcessKey(KEY_END);
            ProcessKey(KEY_ENTER);
            /* $ 19.05.2001 IS
               �� ��������� ������� �����, ����� ��� �� ���� �� ������, �
               ������ - ��� ���������� ����������� ������ ��������� �� �����,
               ��� ���� ��������� � � ������ �����.
            */
            if(!EdOpt.AutoIndent)
              for (int I=0;I<StartPos;I++)
                ProcessKey(' ');
            /* IS $ */
          }
        }
        else
        {
          ProcessKey(KEY_DOWN);
          CurLine->SetTabCurPos(StartPos);
          CurLine->SetOvertypeMode(FALSE);
        }
        I++;
        continue;
      }

    int CurWidth=CurLine->GetTabCurPos()-StartPos;
    if (CurWidth>VBlockSizeX)
      VBlockSizeX=CurWidth;
    if (VBlockSizeY==0)
      VBlockSizeY++;

    if (SaveOvertype)
    {
      Flags.Set(FEDITOR_OVERTYPE);
      CurLine->SetOvertypeMode(TRUE);
    }

    TopScreen=SavedTopScreen;
    CurLine=VBlockStart;
    NumLine=BlockStartLine;
    CurLine->SetTabCurPos(StartPos);


    Pasting--;
    Unlock ();
  }
  delete ClipText;
  BlockUndo=FALSE;
}


void Editor::VBlockShift(int Left)
{
  if (Flags.Check(FEDITOR_LOCKMODE) || Left && VBlockX==0 || VBlockSizeX<=0 || VBlockSizeY<=0)
    return;

  Edit *CurPtr=VBlockStart;

  int UndoNext=FALSE;

  for (int Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->m_next)
  {
    /*$ 10.08.2000 skv
      Modified->TextChanged
    */
    TextChanged(1);
    /* skv $*/

    int TBlockX=CurPtr->TabPosToReal(VBlockX);
    int TBlockSizeX=CurPtr->TabPosToReal(VBlockX+VBlockSizeX)-
                    CurPtr->TabPosToReal(VBlockX);

    const char *CurStr,*EndSeq;
    int Length;
    CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);
    if (TBlockX>Length)
      continue;
    if (Left && CurStr[TBlockX-1]=='\t' ||
        !Left && TBlockX+TBlockSizeX<Length && CurStr[TBlockX+TBlockSizeX]=='\t')
    {
      CurPtr->ReplaceTabs();
      CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);
      TBlockX=CurPtr->TabPosToReal(VBlockX);
      TBlockSizeX=CurPtr->TabPosToReal(VBlockX+VBlockSizeX)-
                  CurPtr->TabPosToReal(VBlockX);
    }


    BlockUndo=UndoNext;
    AddUndoData(CurPtr->GetStringAddr(),CurPtr->GetEOL(),BlockStartLine+Line,
                CurPtr->GetCurPos(),UNDO_EDIT);
    UndoNext=TRUE;

    int StrLength=Max(Length,TBlockX+TBlockSizeX+!Left);
    char *TmpStr=new char[StrLength+3];
    memset(TmpStr,' ',StrLength);
    memcpy(TmpStr,CurStr,Length);

    if (Left)
    {
      int Ch=TmpStr[TBlockX-1];
      for (int I=TBlockX;I<TBlockX+TBlockSizeX;I++)
        TmpStr[I-1]=TmpStr[I];
      TmpStr[TBlockX+TBlockSizeX-1]=Ch;
    }
    else
    {
      int Ch=TmpStr[TBlockX+TBlockSizeX];
      for (int I=TBlockX+TBlockSizeX-1;I>=TBlockX;I--)
        TmpStr[I+1]=TmpStr[I];
      TmpStr[TBlockX]=Ch;
    }

    while (StrLength>0 && TmpStr[StrLength-1]==' ')
      StrLength--;
    int EndLength=(int)strlen(EndSeq);
    memcpy(TmpStr+StrLength,EndSeq,EndLength);
    StrLength+=EndLength;
    TmpStr[StrLength]=0;

    CurPtr->SetBinaryString(TmpStr,StrLength);
    delete[] TmpStr;
  }
  VBlockX+=Left ? -1:1;
  CurLine->SetTabCurPos(Left ? VBlockX:VBlockX+VBlockSizeX);
}


int Editor::EditorControl(int Command,void *Param)
{
  int I;
  _ECTLLOG(CleverSysLog SL("Editor::EditorControl()"));
  _ECTLLOG(SysLog("Command=%s Param=[%d/0x%08X]",_ECTL_ToName(Command),Param,Param));
  switch(Command)
  {
    case ECTL_GETSTRING:
    {
      struct EditorGetString *GetString=(struct EditorGetString *)Param;
      if(GetString && !IsBadReadPtr(GetString,sizeof(struct EditorGetString)))
      {
        Edit *CurPtr=GetStringByNumber(GetString->StringNumber);
        if (!CurPtr)
        {
          _ECTLLOG(SysLog("struct EditorGetString => GetStringByNumber(%d) return NULL",GetString->StringNumber));
          return(FALSE);
        }
        //CurPtr->GetBinaryString(GetString->StringText,
        //                      &const_cast<const char*>(GetString->StringEOL),
        //                      GetString->StringLength);
        CurPtr->GetBinaryString(const_cast<const char **>(&GetString->StringText),
                                const_cast<const char **>(&GetString->StringEOL),
                                GetString->StringLength);
        GetString->SelStart=-1;
        GetString->SelEnd=0;
        int DestLine=GetString->StringNumber;
        if (DestLine==-1)
          DestLine=NumLine;
        if (BlockStart!=NULL)
        {
          /* $ 12.11.2002 DJ
             ������ ��������� ����������
          */
          CurPtr->GetRealSelection(GetString->SelStart,GetString->SelEnd);
          /* DJ $ */
        }
        else if (VBlockStart!=NULL && DestLine>=VBlockY && DestLine<VBlockY+VBlockSizeY)
        {
          GetString->SelStart=CurPtr->TabPosToReal(VBlockX);
          GetString->SelEnd=GetString->SelStart+
                            CurPtr->TabPosToReal(VBlockX+VBlockSizeX)-
                            CurPtr->TabPosToReal(VBlockX);
        }
        _ECTLLOG(char *LinDump=(GetString->StringEOL?(char *)_SysLog_LinearDump(GetString->StringEOL,strlen(GetString->StringEOL)):NULL));
        _ECTLLOG(SysLog("struct EditorGetString{"));
        _ECTLLOG(SysLog("  StringNumber    =%d",GetString->StringNumber));
        _ECTLLOG(SysLog("  StringText      ='%s'",GetString->StringText));
        _ECTLLOG(SysLog("  StringEOL       ='%s'",GetString->StringEOL?LinDump:"(null)"));
        _ECTLLOG(SysLog("  StringLength    =%d",GetString->StringLength));
        _ECTLLOG(SysLog("  SelStart        =%d",GetString->SelStart));
        _ECTLLOG(SysLog("  SelEnd          =%d",GetString->SelEnd));
        _ECTLLOG(SysLog("}"));
        _ECTLLOG(if(LinDump)xf_free(LinDump));
        return(TRUE);
      }
      break;
    }

    case ECTL_INSERTSTRING:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      else
      {
        int Indent=Param!=NULL && *(int *)Param!=FALSE;
        if (!Indent)
          Pasting++;
        Flags.Set(FEDITOR_NEWUNDO);
        InsertString();
        Show();
        if (!Indent)
          Pasting--;
      }
      return(TRUE);
    }

    case ECTL_INSERTTEXT:
    {
      if(!Param)
        return FALSE;

      _ECTLLOG(SysLog("(char *)Param='%s'",(char *)Param));
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      else
      {
        char *Str=(char *)Param;
        Pasting++;
        Lock ();

        while (*Str)
          ProcessKey(*(Str++));

        Unlock ();
        Pasting--;
      }
      return(TRUE);
    }

    case ECTL_SETSTRING:
    {
      struct EditorSetString *SetString=(struct EditorSetString *)Param;

      if(!SetString || IsBadReadPtr(SetString,sizeof(struct EditorSetString)))
        break;

      _ECTLLOG(SysLog("struct EditorSetString{"));
      _ECTLLOG(SysLog("  StringNumber    =%d",SetString->StringNumber));
      _ECTLLOG(SysLog("  StringText      ='%s'",SetString->StringText));
      _ECTLLOG(SysLog("  StringEOL       ='%s'",SetString->StringEOL?_SysLog_LinearDump((LPBYTE)SetString->StringEOL,strlen(SetString->StringEOL)):"(null)"));
      _ECTLLOG(SysLog("  StringLength    =%d",SetString->StringLength));
      _ECTLLOG(SysLog("}"));

      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        break;
      }
      else
      {
        /* $ 06.08.2002 IS
           ��������� ������������ StringLength � ������ FALSE, ���� ��� ������
           ����.
        */
        int Length=SetString->StringLength;
        if(Length < 0)
        {
          _ECTLLOG(SysLog("SetString->StringLength < 0"));
          return(FALSE);
        }

        Edit *CurPtr=GetStringByNumber(SetString->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",SetString->StringNumber));
          return(FALSE);
        }

        const char *EOL=SetString->StringEOL==NULL ? GlobalEOL:SetString->StringEOL;
        /* IS 06.08.2002 IS $ */
        int LengthEOL=(int)strlen(EOL);
        char *NewStr=(char*)xf_malloc(Length+LengthEOL+1);
        if (NewStr==NULL)
        {
          _ECTLLOG(SysLog("xf_malloc(%d) return NULL",Length+LengthEOL+1));
          return(FALSE);
        }

        int DestLine=SetString->StringNumber;
        if (DestLine==-1)
          DestLine=NumLine;

        memcpy(NewStr,SetString->StringText,Length);
        memcpy(NewStr+Length,EOL,LengthEOL);
        AddUndoData(CurPtr->GetStringAddr(),CurPtr->GetEOL(),DestLine,
                    CurPtr->GetCurPos(),UNDO_EDIT);

        int CurPos=CurPtr->GetCurPos();
        CurPtr->SetBinaryString(NewStr,Length+LengthEOL);
        CurPtr->SetCurPos(CurPos);
        TextChanged(1);    // 10.08.2000 skv - Modified->TextChanged
        xf_free(NewStr);
      }
      return(TRUE);
    }

    case ECTL_DELETESTRING:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      DeleteString(CurLine,FALSE,NumLine);
      return(TRUE);
    }

    case ECTL_DELETECHAR:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return(FALSE);
      }
      Pasting++;
      ProcessKey(KEY_DEL);
      Pasting--;
      return(TRUE);
    }

    case ECTL_GETINFO:
    {
      struct EditorInfo *Info=(struct EditorInfo *)Param;
      if(Info && !IsBadWritePtr(Info,sizeof(struct EditorInfo)))
      {
        memset(Info,0,sizeof(*Info));
        Info->EditorID=Editor::EditorID;
        Info->FileName="";
        Info->WindowSizeX=ObjWidth;
        Info->WindowSizeY=Y2-Y1+1;
        Info->TotalLines=NumLastLine;
        Info->CurLine=NumLine;
        Info->CurPos=CurLine->GetCurPos();
        Info->CurTabPos=CurLine->GetTabCurPos();
        Info->TopScreenLine=NumLine-CalcDistance(TopScreen,CurLine,-1);
        Info->LeftPos=CurLine->GetLeftPos();
        Info->Overtype=Flags.Check(FEDITOR_OVERTYPE);
        Info->BlockType=BTYPE_NONE;
        if (BlockStart!=NULL)
          Info->BlockType=BTYPE_STREAM;
        if (VBlockStart!=NULL)
          Info->BlockType=BTYPE_COLUMN;
        Info->BlockStartLine=Info->BlockType==BTYPE_NONE ? 0:BlockStartLine;
        Info->AnsiMode=AnsiText;
        Info->TableNum=UseDecodeTable ? TableNum-1:-1;
        //Info->Options=0;
        if (EdOpt.ExpandTabs == EXPAND_ALLTABS)
          Info->Options|=EOPT_EXPANDALLTABS;
        if (EdOpt.ExpandTabs == EXPAND_NEWTABS)
          Info->Options|=EOPT_EXPANDONLYNEWTABS;
        if (EdOpt.PersistentBlocks)
          Info->Options|=EOPT_PERSISTENTBLOCKS;
        if (EdOpt.DelRemovesBlocks)
          Info->Options|=EOPT_DELREMOVESBLOCKS;
        if (EdOpt.AutoIndent)
          Info->Options|=EOPT_AUTOINDENT;
        if (EdOpt.SavePos)
          Info->Options|=EOPT_SAVEFILEPOSITION;
        if (EdOpt.AutoDetectTable)
          Info->Options|=EOPT_AUTODETECTTABLE;
        if (EdOpt.CursorBeyondEOL)
          Info->Options|=EOPT_CURSORBEYONDEOL;
        Info->TabSize=EdOpt.TabSize;
        Info->BookMarkCount=BOOKMARK_COUNT;
        Info->CurState=Flags.Check(FEDITOR_LOCKMODE)?ECSTATE_LOCKED:0;
        Info->CurState|=!Flags.Check(FEDITOR_MODIFIED)?ECSTATE_SAVED:0;
        Info->CurState|=Flags.Check(FEDITOR_MODIFIED|FEDITOR_WASCHANGED)?ECSTATE_MODIFIED:0;
        return TRUE;
      }
      _ECTLLOG(SysLog("Error: Param == NULL or IsBadWritePtr(Param,sizeof(struct EditorInfo))"));
      break;
    }

    case ECTL_SETPOSITION:
    {
      // "������� ���� �����..."
      if(Param && !IsBadReadPtr(Param,sizeof(struct EditorSetPosition)))
      {
        // ...� ��� ������ ���������� � ���, ��� ���������
        struct EditorSetPosition *Pos=(struct EditorSetPosition *)Param;
        _ECTLLOG(SysLog("struct EditorSetPosition{"));
        _ECTLLOG(SysLog("  CurLine       = %d",Pos->CurLine));
        _ECTLLOG(SysLog("  CurPos        = %d",Pos->CurPos));
        _ECTLLOG(SysLog("  CurTabPos     = %d",Pos->CurTabPos));
        _ECTLLOG(SysLog("  TopScreenLine = %d",Pos->TopScreenLine));
        _ECTLLOG(SysLog("  LeftPos       = %d",Pos->LeftPos));
        _ECTLLOG(SysLog("  Overtype      = %d",Pos->Overtype));
        _ECTLLOG(SysLog("}"));

        Lock ();

        int CurPos=CurLine->GetCurPos();

        // �������� ���� �� ��������� ��� (���� ����)
        if ((Pos->CurLine >= 0 || Pos->CurPos >= 0)&&
            (Pos->CurLine!=NumLine || Pos->CurPos!=CurPos))
          Flags.Set(FEDITOR_CURPOSCHANGEDBYPLUGIN);

        if (Pos->CurLine >= 0) // �������� ������
        {
          if (Pos->CurLine==NumLine-1)
            Up();
          else
            if (Pos->CurLine==NumLine+1)
              Down();
            else
              GoToLine(Pos->CurLine);
        }

        if (Pos->TopScreenLine >= 0 && Pos->TopScreenLine<=NumLine)
        {
          TopScreen=CurLine;
          for (int I=NumLine;I>0 && NumLine-I<Y2-Y1 && I!=Pos->TopScreenLine;I--)
            TopScreen=TopScreen->m_prev;
        }

        if (Pos->CurPos >= 0)
          CurLine->SetCurPos(Pos->CurPos);

        if (Pos->CurTabPos >= 0)
          CurLine->SetTabCurPos(Pos->CurTabPos);

        if (Pos->LeftPos >= 0)
          CurLine->SetLeftPos(Pos->LeftPos);

        /* $ 30.08.2001 IS
           ��������� ������ ����� ���������� �����, � ��������� ������ ��������
           �����, �.�. ��������������� ������, ��� ����� �������, � ����� ����
           ��������������, � ���������� ���� �������� �������������� ���������.
        */
        if (Pos->Overtype >= 0)
        {
          Flags.Change(FEDITOR_OVERTYPE,Pos->Overtype);
          CurLine->SetOvertypeMode(Flags.Check(FEDITOR_OVERTYPE));
        }
        /* IS $ */

        Unlock ();
        return TRUE;
      }
      _ECTLLOG(SysLog("Error: Param == NULL or IsBadReadPtr(Param,sizeof(struct EditorSetPosition))"));
      break;
    }

    case ECTL_SELECT:
    {
      struct EditorSelect *Sel=(struct EditorSelect *)Param;
      if(Sel && !IsBadReadPtr(Sel,sizeof(struct EditorSelect)))
      {
        _ECTLLOG(SysLog("struct EditorSelect{"));
        _ECTLLOG(SysLog("  BlockType     =%s (%d)",(Sel->BlockType==BTYPE_NONE?"BTYPE_NONE":(Sel->BlockType==BTYPE_STREAM?"":(Sel->BlockType==BTYPE_COLUMN?"BTYPE_COLUMN":"BTYPE_?????"))),Sel->BlockType));
        _ECTLLOG(SysLog("  BlockStartLine=%d",Sel->BlockStartLine));
        _ECTLLOG(SysLog("  BlockStartPos =%d",Sel->BlockStartPos));
        _ECTLLOG(SysLog("  BlockWidth    =%d",Sel->BlockWidth));
        _ECTLLOG(SysLog("  BlockHeight   =%d",Sel->BlockHeight));
        _ECTLLOG(SysLog("}"));

        UnmarkBlock();
        if (Sel->BlockType==BTYPE_NONE)
          return(TRUE);

        Edit *CurPtr=GetStringByNumber(Sel->BlockStartLine);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",Sel->BlockStartLine));
          return(FALSE);
        }

        if (Sel->BlockType==BTYPE_STREAM)
        {
          BlockStart=CurPtr;
          if((BlockStartLine=Sel->BlockStartLine) == -1)
            BlockStartLine=NumLine;

          for (I=0;I<Sel->BlockHeight;I++)
          {
            int SelStart=(I==0) ? Sel->BlockStartPos:0;
            int SelEnd=(I<Sel->BlockHeight-1) ? -1:Sel->BlockStartPos+Sel->BlockWidth;
            CurPtr->Select(SelStart,SelEnd);
            CurPtr=CurPtr->m_next;
            if (CurPtr==NULL)
              return(FALSE);
          }
        }
        if (Sel->BlockType==BTYPE_COLUMN)
        {
          VBlockStart=CurPtr;
          if((BlockStartLine=Sel->BlockStartLine) == -1)
            BlockStartLine=NumLine;

          if (Sel->BlockWidth==-1)
            return(FALSE);

          VBlockX=Sel->BlockStartPos;
          if((VBlockY=Sel->BlockStartLine) == -1)
             VBlockY=NumLine;
          VBlockSizeX=Sel->BlockWidth;
          VBlockSizeY=Sel->BlockHeight;
        }
        return(TRUE);
      }
      break;
    }

    case ECTL_REDRAW:
    {
      Show();
      ScrBuf.Flush();
      return(TRUE);
    }

    case ECTL_TABTOREAL:
    {
      if(Param && !IsBadReadPtr(Param,sizeof(struct EditorConvertPos)))
      {
        struct EditorConvertPos *ecp=(struct EditorConvertPos *)Param;
        Edit *CurPtr=GetStringByNumber(ecp->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",ecp->StringNumber));
          return(FALSE);
        }
        ecp->DestPos=CurPtr->TabPosToReal(ecp->SrcPos);
        _ECTLLOG(SysLog("struct EditorConvertPos{"));
        _ECTLLOG(SysLog("  StringNumber =%d",ecp->StringNumber));
        _ECTLLOG(SysLog("  SrcPos       =%d",ecp->SrcPos));
        _ECTLLOG(SysLog("  DestPos      =%d",ecp->DestPos));
        _ECTLLOG(SysLog("}"));
        return(TRUE);
      }
      break;
    }

    case ECTL_REALTOTAB:
    {
      if(Param && !IsBadReadPtr(Param,sizeof(struct EditorConvertPos)))
      {
        struct EditorConvertPos *ecp=(struct EditorConvertPos *)Param;
        Edit *CurPtr=GetStringByNumber(ecp->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",ecp->StringNumber));
          return(FALSE);
        }
        ecp->DestPos=CurPtr->RealPosToTab(ecp->SrcPos);
        _ECTLLOG(SysLog("struct EditorConvertPos{"));
        _ECTLLOG(SysLog("  StringNumber =%d",ecp->StringNumber));
        _ECTLLOG(SysLog("  SrcPos       =%d",ecp->SrcPos));
        _ECTLLOG(SysLog("  DestPos      =%d",ecp->DestPos));
        _ECTLLOG(SysLog("}"));
        return(TRUE);
      }
      break;
    }

    case ECTL_EXPANDTABS:
    {
      if (Flags.Check(FEDITOR_LOCKMODE))
      {
        _ECTLLOG(SysLog("FEDITOR_LOCKMODE!"));
        return FALSE;
      }
      else
      {
        int StringNumber=*(int *)Param;
        Edit *CurPtr=GetStringByNumber(StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",StringNumber));
          return FALSE;
        }
        AddUndoData(CurPtr->GetStringAddr(),CurPtr->GetEOL(),StringNumber,
                    CurPtr->GetCurPos(),UNDO_EDIT);
        CurPtr->ReplaceTabs();
      }
      return TRUE;
    }

    // TODO: ���� DI_MEMOEDIT �� ����� ����� ��������, �� ������ ����������� � FileEditor::EditorControl(), � ������� - ����� ������
    case ECTL_ADDCOLOR:
    {
      if(Param && !IsBadReadPtr(Param,sizeof(struct EditorColor)))
      {
        struct EditorColor *col=(struct EditorColor *)Param;
        _ECTLLOG(SysLog("struct EditorColor{"));
        _ECTLLOG(SysLog("  StringNumber=%d",col->StringNumber));
        _ECTLLOG(SysLog("  ColorItem   =%d (0x%08X)",col->ColorItem,col->ColorItem));
        _ECTLLOG(SysLog("  StartPos    =%d",col->StartPos));
        _ECTLLOG(SysLog("  EndPos      =%d",col->EndPos));
        _ECTLLOG(SysLog("  Color       =%d (0x%08X)",col->Color,col->Color));
        _ECTLLOG(SysLog("}"));

        struct ColorItem newcol;
        newcol.StartPos=col->StartPos+(col->StartPos!=-1?X1:0);
        newcol.EndPos=col->EndPos+X1;
        newcol.Color=col->Color;
        Edit *CurPtr=GetStringByNumber(col->StringNumber);
        if (CurPtr==NULL)
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL",col->StringNumber));
          return FALSE;
        }
        if (col->Color==0)
          return(CurPtr->DeleteColor(newcol.StartPos));
        CurPtr->AddColor(&newcol);
        return TRUE;
      }
      break;
    }

    // TODO: ���� DI_MEMOEDIT �� ����� ����� ��������, �� ������ ����������� � FileEditor::EditorControl(), � ������� - ����� ������
    case ECTL_GETCOLOR:
    {
      if(Param && !IsBadReadPtr(Param,sizeof(struct EditorColor)))
      {
        struct EditorColor *col=(struct EditorColor *)Param;
        Edit *CurPtr=GetStringByNumber(col->StringNumber);
        if (!CurPtr || IsBadWritePtr(col,sizeof(struct EditorColor)))
        {
          _ECTLLOG(SysLog("GetStringByNumber(%d) return NULL or IsBadWritePtr(col,sizeof(struct EditorColor)",col->StringNumber));
          return FALSE;
        }
        struct ColorItem curcol;
        if (!CurPtr->GetColor(&curcol,col->ColorItem))
        {
          _ECTLLOG(SysLog("GetColor() return NULL"));
          return FALSE;
        }
        col->StartPos=curcol.StartPos-X1;
        col->EndPos=curcol.EndPos-X1;
        col->Color=curcol.Color;
        _ECTLLOG(SysLog("struct EditorColor{"));
        _ECTLLOG(SysLog("  StringNumber=%d",col->StringNumber));
        _ECTLLOG(SysLog("  ColorItem   =%d (0x%08X)",col->ColorItem,col->ColorItem));
        _ECTLLOG(SysLog("  StartPos    =%d",col->StartPos));
        _ECTLLOG(SysLog("  EndPos      =%d",col->EndPos));
        _ECTLLOG(SysLog("  Color       =%d (0x%08X)",col->Color,col->Color));
        _ECTLLOG(SysLog("}"));
        return TRUE;
      }
      break;
    }

    /*$ 07.09.2000 skv
      New ECTL parameter
    */
    // ������ ����������� � FileEditor::EditorControl()
    case ECTL_PROCESSKEY:
    {
      _ECTLLOG(SysLog("Key = %s",_FARKEY_ToName((DWORD)Param)));
      ProcessKey((int)(INT_PTR)Param);
      return TRUE;
    }
    /* skv$*/
    /* $ 16.02.2001 IS
         ��������� ��������� ���������� �������� ���������. Param ��������� ��
         ��������� EditorSetParameter
    */
    case ECTL_SETPARAM:
    {
      struct EditorSetParameter *espar=(struct EditorSetParameter *)Param;
      if(espar && !IsBadReadPtr(espar,sizeof(struct EditorSetParameter)))
      {
        int rc=TRUE;
        _ECTLLOG(SysLog("struct EditorSetParameter{"));
        _ECTLLOG(SysLog("  Type        =%s",_ESPT_ToName(espar->Type)));
        switch(espar->Type)
        {
          case ESPT_GETWORDDIV:
            _ECTLLOG(SysLog("  cParam      =(%p)",espar->Param.cParam));
            if(!IsBadWritePtr(espar->Param.cParam,sizeof(EdOpt.WordDiv)))
              xstrncpy(espar->Param.cParam,EdOpt.WordDiv,sizeof(EdOpt.WordDiv)-1);
            else
              rc=FALSE;
            break;
          case ESPT_SETWORDDIV:
            _ECTLLOG(SysLog("  cParam      =[%s]",espar->Param.cParam));
            SetWordDiv((!espar->Param.cParam || !*espar->Param.cParam)?Opt.WordDiv:espar->Param.cParam);
            break;
          case ESPT_TABSIZE:
            _ECTLLOG(SysLog("  iParam      =%d",espar->Param.iParam));
            SetTabSize(espar->Param.iParam);
            break;
          case ESPT_EXPANDTABS:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetConvertTabs(espar->Param.iParam);
            break;
          case ESPT_AUTOINDENT:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetAutoIndent(espar->Param.iParam);
            break;
          case ESPT_CURSORBEYONDEOL:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetCursorBeyondEOL(espar->Param.iParam);
            break;
          case ESPT_CHARCODEBASE:
            _ECTLLOG(SysLog("  iParam      =%s",(espar->Param.iParam==0?"0 (Oct)":(espar->Param.iParam==1?"1 (Dec)":(espar->Param.iParam==2?"2 (Hex)":"?????")))));
            SetCharCodeBase(espar->Param.iParam);
            break;
          /* $ 07.08.2001 IS ������� ��������� �� ������� */
          case ESPT_CHARTABLE:
          {
            _ECTLLOG(if(espar->Param.iParam <= 3)SysLog("  iParam      =%s",(espar->Param.iParam==1?"1 (OEM)":(espar->Param.iParam==2?"2 (ANSI)":"3 (table '0')"))));
            _ECTLLOG(else SysLog("  iParam      =%d",espar->Param.iParam));
            int UseUnicode=FALSE;
            /*  $ 04.11.2001 IS
                ��� ��������� ����� ������� ������� ��� ��� ����
                (������ �������� oem)
            */
            int oldAnsiText(AnsiText), oldUseDecodeTable(UseDecodeTable),
                oldTableNum(TableNum), oldChangedByUser(Flags.Check(FEDITOR_TABLECHANGEDBYUSER));

            AnsiText=espar->Param.iParam==2,
            UseDecodeTable=espar->Param.iParam>1,
            TableNum=UseDecodeTable?espar->Param.iParam-3:-1;
            Flags.Set(FEDITOR_TABLECHANGEDBYUSER);

            if(AnsiText)
               rc=GetTable(&TableSet,TRUE,TableNum,UseUnicode);
            else if(UseDecodeTable)
               rc=PrepareTable(&TableSet, TableNum);

            /* $ 07.03.2002 IS
               ��� ����, ����� �� Shift-F8 ��������� ���������� ����� ��������
               ������� ������ �� 1, �.�. ��� ��������� ���� �� � 0, � � 1.
            */
            if(rc)
               ++TableNum;
            else
            /* IS $ */
            {
              Flags.Change(FEDITOR_TABLECHANGEDBYUSER,oldChangedByUser);
              TableNum=oldTableNum;
              UseDecodeTable=oldUseDecodeTable;
              AnsiText=oldAnsiText;
            }
            /* IS $ */

            SetStringsTable();
            if (HostFileEditor) HostFileEditor->ChangeEditKeyBar();
            Show();
          }
          /* IS $ */
          /* $ 29.10.2001 IS ��������� ��������� "��������� ������� �����" */
          case ESPT_SAVEFILEPOSITION:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            SetSavePosMode(espar->Param.iParam, -1);
            break;
          /* IS $ */
          /* $ 23.03.2002 IS ���������/�������� ��������� ����� */
          case ESPT_LOCKMODE:
            _ECTLLOG(SysLog("  iParam      =%s",espar->Param.iParam?"On":"Off"));
            Flags.Change(FEDITOR_LOCKMODE, espar->Param.iParam);
            break;
          /* IS $ */
          default:
            _ECTLLOG(SysLog("}"));
            return FALSE;
        }
        _ECTLLOG(SysLog("}"));
        return rc;
      }
      return  FALSE;
    }
    /* IS $ */
    /* $ 04.04.2002 IS
       ������ ���� ��������� "�������������� ��������� �����"
    */
    case ECTL_TURNOFFMARKINGBLOCK:
    {
      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      return TRUE;
    }
    /* IS $ */

    case ECTL_DELETEBLOCK:
    {
      if (Flags.Check(FEDITOR_LOCKMODE) || !(VBlockStart || BlockStart))
      {
        _ECTLLOG(if(Flags.Check(FEDITOR_LOCKMODE))SysLog("FEDITOR_LOCKMODE!"));
        _ECTLLOG(if(!(VBlockStart || BlockStart))SysLog("Not selected block!"));
        return FALSE;
      }

      Flags.Clear(FEDITOR_MARKINGVBLOCK|FEDITOR_MARKINGBLOCK);
      DeleteBlock();
      Show();
      return TRUE;
    }
  }
  return FALSE;
}

int Editor::SetBookmark(DWORD Pos)
{
  if(Pos < BOOKMARK_COUNT)
  {
    SavePos.Line[Pos]=NumLine;
    SavePos.Cursor[Pos]=CurLine->GetCurPos();
    SavePos.LeftPos[Pos]=CurLine->GetLeftPos();
    SavePos.ScreenLine[Pos]=CalcDistance(TopScreen,CurLine,-1);
    return TRUE;
  }
  return FALSE;
}

int Editor::GotoBookmark(DWORD Pos)
{
  if(Pos < BOOKMARK_COUNT)
  {
    if (SavePos.Line[Pos]!=0xffffffff)
    {
      GoToLine(SavePos.Line[Pos]);
      CurLine->SetCurPos(SavePos.Cursor[Pos]);
      CurLine->SetLeftPos(SavePos.LeftPos[Pos]);
      TopScreen=CurLine;
      for (DWORD I=0;I<SavePos.ScreenLine[Pos] && TopScreen->m_prev!=NULL;I++)
        TopScreen=TopScreen->m_prev;
      if (!EdOpt.PersistentBlocks)
        UnmarkBlock();
      Show();
    }
    return TRUE;
  }
  return FALSE;
}

int Editor::RestoreStackBookmark()
{
  if(StackPos && (StackPos->Line!=NumLine || StackPos->Cursor!=CurLine->GetCurPos()))
  {
    GoToLine(StackPos->Line);
    CurLine->SetCurPos(StackPos->Cursor);
    CurLine->SetLeftPos(StackPos->LeftPos);
    TopScreen=CurLine;
    for (DWORD I=0;I<StackPos->ScreenLine && TopScreen->m_prev!=NULL;I++)
      TopScreen=TopScreen->m_prev;
    if (!EdOpt.PersistentBlocks)
      UnmarkBlock();
    Show();
    return TRUE;
  }
  return FALSE;
}

int Editor::AddStackBookmark()
{
  struct InternalEditorStackBookMark *sb_old=StackPos,*sb_new;

  if (StackPos && StackPos->next)
  {
    StackPos=StackPos->next;
    StackPos->prev=0;
    ClearStackBookmarks();
    StackPos=sb_old;
    StackPos->next = 0;
  }

  if (StackPos && StackPos->Line==NumLine && StackPos->Cursor==CurLine->GetCurPos())
    return TRUE;

  sb_new = (InternalEditorStackBookMark*) xf_malloc (sizeof (InternalEditorStackBookMark));

  if (sb_new)
  {
    if (StackPos)
      StackPos->next=sb_new;
    StackPos=sb_new;
    StackPos->prev=sb_old;
    StackPos->next=0;
    StackPos->Line=NumLine;
    StackPos->Cursor=CurLine->GetCurPos();
    StackPos->LeftPos=CurLine->GetLeftPos();
    StackPos->ScreenLine=CalcDistance(TopScreen,CurLine,-1);
    NewStackPos = TRUE; // When go prev bookmark, we must save current
    return TRUE;
  }
  return FALSE;
}

int Editor::PrevStackBookmark()
{
  if (StackPos)
  {
    if (NewStackPos) // Save last position in new bookmark
    {
      AddStackBookmark();
      NewStackPos = FALSE;
    }

    if (StackPos->prev) // If not first bookmark - go
    {
      StackPos=StackPos->prev;
    }
    return RestoreStackBookmark();
  }
  return FALSE;
}

int Editor::NextStackBookmark()
{
  if (StackPos)
  {
    if (StackPos->next) // If not last bookmark - go
    {
      StackPos=StackPos->next;
    }
    return RestoreStackBookmark();
  }
  return FALSE;
}

int Editor::ClearStackBookmarks()
{
  if (StackPos)
  {
    struct InternalEditorStackBookMark *sb_prev = StackPos->prev, *sb_next;

    while(StackPos)
    {
      sb_next = StackPos->next;
      xf_free (StackPos);
      StackPos = sb_next;
    }

    StackPos = sb_prev;

    while(StackPos)
    {
      sb_prev = StackPos->prev;
      xf_free (StackPos);
      StackPos = sb_prev;
    }

    NewStackPos = FALSE;
  }

  return TRUE;
}

InternalEditorStackBookMark* Editor::PointerToStackBookmark(int iIdx)
{
  InternalEditorStackBookMark *sb_temp=StackPos;
  if (iIdx!=-1 && sb_temp)
  {
    while(sb_temp->prev)
      sb_temp=sb_temp->prev;
    for (int i=0;i!=iIdx && sb_temp;i++)
      sb_temp=sb_temp->next;
  }
  return sb_temp;
}

int Editor::DeleteStackBookmark(InternalEditorStackBookMark *sb_delete)
{
  if (sb_delete)
  {
    if (sb_delete->next)
      sb_delete->next->prev=sb_delete->prev;
    if (sb_delete->prev)
      sb_delete->prev->next=sb_delete->next;
    if (StackPos==sb_delete)
      StackPos=(sb_delete->next)?sb_delete->next:sb_delete->prev;
    xf_free (sb_delete);
    return TRUE;
  }
  return FALSE;
}

int Editor::GetStackBookmark(int iIdx,EditorBookMarks *Param)
{
  InternalEditorStackBookMark *sb_temp = PointerToStackBookmark(iIdx);

  if (sb_temp && Param && !IsBadWritePtr(Param,sizeof(EditorBookMarks)))
  {
    if(Param->Line)       Param->Line[0]       =sb_temp->Line;
    if(Param->Cursor)     Param->Cursor[0]     =sb_temp->Cursor;
    if(Param->LeftPos)    Param->LeftPos[0]    =sb_temp->LeftPos;
    if(Param->ScreenLine) Param->ScreenLine[0] =sb_temp->ScreenLine;
    return TRUE;
  }
  return FALSE;
}

int Editor::GetStackBookmarks(EditorBookMarks *Param)
{
  InternalEditorStackBookMark *sb_temp=StackPos, *sb_start;
  int iCount=0;

  if (sb_temp)
  {
    for (;sb_temp->prev;iCount++)
      sb_temp=sb_temp->prev;
    sb_start=sb_temp;
    for (sb_temp=StackPos;sb_temp;iCount++)
      sb_temp=sb_temp->next;

    if (Param && !IsBadReadPtr(Param,sizeof(EditorBookMarks)))
    {
      bool blLine = Param->Line && !IsBadWritePtr(Param->Line,iCount*sizeof(long));
      bool blCursor = Param->Cursor && !IsBadWritePtr(Param->Cursor,iCount*sizeof(long));
      bool blLeftPos = Param->LeftPos && !IsBadWritePtr(Param->LeftPos,iCount*sizeof(long));
      bool blScreenLine = Param->ScreenLine && !IsBadWritePtr(Param->ScreenLine,iCount*sizeof(long));

      if (blLine || blCursor || blLeftPos || blScreenLine)
      {
        sb_temp=sb_start;
        for(int i=0;i<iCount;i++)
        {
          if (blLine) Param->Line[i]=sb_temp->Line;
          if (blCursor) Param->Cursor[i]=sb_temp->Cursor;
          if (blLeftPos) Param->LeftPos[i]=sb_temp->LeftPos;
          if (blScreenLine) Param->ScreenLine[i]=sb_temp->ScreenLine;
          sb_temp=sb_temp->next;
        }
      }
      else iCount=0;
    }
  }
  return iCount;
}

Edit * Editor::GetStringByNumber(int DestLine)
{
  if (DestLine==NumLine || DestLine<0)
    return(CurLine);
  if (DestLine>NumLastLine)
    return(NULL);

  if (DestLine>NumLine)
  {
    Edit *CurPtr=CurLine;
    for (int Line=NumLine;Line<DestLine;Line++)
    {
      CurPtr=CurPtr->m_next;
      if (CurPtr==NULL)
        return(NULL);
    }
    return(CurPtr);
  }

  if (DestLine<NumLine && DestLine>NumLine/2)
  {
    Edit *CurPtr=CurLine;
    for (int Line=NumLine;Line>DestLine;Line--)
    {
      CurPtr=CurPtr->m_prev;
      if (CurPtr==NULL)
        return(NULL);
    }
    return(CurPtr);
  }

  {
    Edit *CurPtr=TopList;
    for (int Line=0;Line<DestLine;Line++)
    {
      CurPtr=CurPtr->m_next;
      if (CurPtr==NULL)
        return(NULL);
    }
    return(CurPtr);
  }
}

void Editor::SetReplaceMode(int Mode)
{
  ::ReplaceMode=Mode;
}

int Editor::GetLineCurPos()
{
  return CurLine->GetTabCurPos();
}

void Editor::BeginVBlockMarking()
{
    UnmarkBlock();
    VBlockStart=CurLine;
    VBlockX=CurLine->GetTabCurPos();
    VBlockSizeX=0;
    VBlockY=NumLine;
    VBlockSizeY=1;
    Flags.Set(FEDITOR_MARKINGVBLOCK);
    BlockStartLine=NumLine;
    //_D(SysLog("BeginVBlockMarking, set vblock to  VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
}

void Editor::AdjustVBlock(int PrevX)
{
    int x=GetLineCurPos();
    int c2;

    //_D(SysLog("AdjustVBlock, x=%i,   vblock is VBlockY=%i:%i, VBlockX=%i:%i, PrevX=%i",x,VBlockY,VBlockSizeY,VBlockX,VBlockSizeX,PrevX));
    if ( x==VBlockX+VBlockSizeX)  // ������ �� ���������, ������� ��������� ���
        return;
    if ( x>VBlockX )  // ������ ������ ������ �����
    {
        VBlockSizeX=x-VBlockX;
        //_D(SysLog("x>VBlockX");
    }
    else if ( x<VBlockX ) // ������ ������ �� ������ �����
    {
        c2=VBlockX;
        if ( PrevX>VBlockX )    // ���������� ������, � ������ �����
        {
            VBlockX=x;
            VBlockSizeX=c2-x;   // ������ ����
        }
        else      // ���������� ����� � ������ ��� ������ �����
        {
            VBlockX=x;
            VBlockSizeX+=c2-x;  // ��������� ����
        }
        //_D(SysLog("x<VBlockX"));
    }
    else if (x==VBlockX && x!=PrevX)
    {
        VBlockSizeX=0;  // ������ � 0, ������ �������� ���� �� ���������
        //_D(SysLog("x==VBlockX && x!=PrevX"));
    }
    // ����������
    //   ������ x>VBLockX+VBlockSizeX �� ����� ����
    //   ������ ��� ������ ������� ����� �� ���������, �� �� ������

    //_D(SysLog("AdjustVBlock, changed vblock  VBlockY=%i:%i, VBlockX=%i:%i",VBlockY,VBlockSizeY,VBlockX,VBlockSizeX));
}


/* $ 24.09.2000 SVS
  ������������� Xlat
*/
void Editor::Xlat()
{
  Edit *CurPtr;
  int Line;
  BOOL DoXlat=FALSE;

  if (VBlockStart!=NULL)
  {
    CurPtr=VBlockStart;

    for (Line=0;CurPtr!=NULL && Line<VBlockSizeY;Line++,CurPtr=CurPtr->m_next)
    {
      int TBlockX=CurPtr->TabPosToReal(VBlockX);
      int TBlockSizeX=CurPtr->TabPosToReal(VBlockX+VBlockSizeX)-
                      CurPtr->TabPosToReal(VBlockX);
      const char *CurStr,*EndSeq;
      int Length;
      CurPtr->GetBinaryString(&CurStr,&EndSeq,Length);
      int CopySize=Length-TBlockX;
      if (CopySize>TBlockSizeX)
         CopySize=TBlockSizeX;
      AddUndoData(CurPtr->GetStringAddr(),CurPtr->GetEOL(),BlockStartLine+Line,0,UNDO_EDIT);
      BlockUndo=TRUE;
      ::Xlat(CurPtr->Str,TBlockX,TBlockX+CopySize,CurPtr->TableSet,Opt.XLat.Flags);
    }
    DoXlat=TRUE;
  }
  else
  {
    Line=0;
    CurPtr=BlockStart;
    /* $ 25.11.2000 IS
         ���� ��� ���������, �� ���������� ������� �����. ����� ������������ ��
         ������ ����������� ������ ������������.
    */
    if(CurPtr!=NULL)
    {
      while (CurPtr!=NULL)
      {
        int StartSel,EndSel;
        CurPtr->GetSelection(StartSel,EndSel);
        if (StartSel==-1)
          break;
        if(EndSel == -1)
          EndSel=(int)strlen(CurPtr->Str);
        AddUndoData(CurPtr->GetStringAddr(),CurPtr->GetEOL(),BlockStartLine+Line,0,UNDO_EDIT);
        ::Xlat(CurPtr->Str,StartSel,EndSel,CurPtr->TableSet,Opt.XLat.Flags);
        BlockUndo=TRUE;
        Line++;
        CurPtr=CurPtr->m_next;
      }
      DoXlat=TRUE;
    }
    else
    {
      CurPtr=CurLine;

      char *Str=CurPtr->Str;
      int start=CurPtr->GetCurPos(), end, StrSize=(int)strlen(Str);
      /* $ 10.12.2000 IS
         ������������ ������ �� �����, �� ������� ����� ������, ��� �� �����,
         ��� ��������� ����� ������� ������� �� 1 ������
      */
      DoXlat=TRUE;

      /* $ 12.01.2004 IS
         ��� ��������� � WordDiv ���������� IsWordDiv, � �� strchr, �.�.
         ������� ��������� ����� ���������� �� ��������� WordDiv (������� OEM)
      */
      if(IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[start]))
      {
         if(start) start--;
         DoXlat=(!IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[start]));
      }

      if(DoXlat)
      {
        while(start>=0 && !IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[start]))
          start--;
        start++;
        end=start+1;
        while(end<StrSize && !IsWordDiv((AnsiText || UseDecodeTable)?&TableSet:NULL,Opt.XLat.WordDivForXlat,Str[end]))
          end++;
        AddUndoData(Str,CurPtr->GetEOL(),NumLine,start,UNDO_EDIT);
        ::Xlat(Str,start,end,CurPtr->TableSet,Opt.XLat.Flags);
      }
      /* 12.01.2004 IS */
     /* IS $ */
    }
    /* IS $ */
  }
  BlockUndo=FALSE;
  if(DoXlat)
    TextChanged(1);
}
/* SVS $ */

/* $ 15.02.2001 IS
     ����������� � ���������� �� ������ ����� ������������ �����.
     ����� ���� ���������� �� ������� ���������, �� ��� ��, imho,
     ������ �� ��������.
*/
//������� ������ ���������
void Editor::SetTabSize(int NewSize)
{
  if (NewSize<1 || NewSize>512)
    NewSize=8;
  if(NewSize!=EdOpt.TabSize) /* ������ ������ ��������� ������ � ��� ������, ���� ��
                          �� ����� ���� ��������� */
  {
    EdOpt.TabSize=NewSize;
    Edit *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->SetTabSize(NewSize);
      CurPtr=CurPtr->m_next;
    }
  }
}

// ������� ����� ������� ������ ���������
// �������� ����������, ������, �.�. ������� �� ��������� ������� �� ���������
void Editor::SetConvertTabs(int NewMode)
{
  if(NewMode!=EdOpt.ExpandTabs) /* ������ ����� ������ � ��� ������, ���� ��
                              �� ����� ���� ��������� */
  {
    EdOpt.ExpandTabs=NewMode;
    Edit *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->SetConvertTabs(NewMode);

      if ( NewMode == EXPAND_ALLTABS )
        CurPtr->ReplaceTabs();

      CurPtr=CurPtr->m_next;
    }
  }
}
/* IS $ */

/* $ 15.02.2001 IS
     + ������ ������������ :) ������� ��������� EdOpt.DelRemovesBlocks �
       EdOpt.PersistentBlocks
*/
void Editor::SetDelRemovesBlocks(int NewMode)
{
  if(NewMode!=EdOpt.DelRemovesBlocks)
  {
    EdOpt.DelRemovesBlocks=NewMode;
    Edit *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->SetDelRemovesBlocks(NewMode);
      CurPtr=CurPtr->m_next;
    }
  }
}

void Editor::SetPersistentBlocks(int NewMode)
{
  if(NewMode!=EdOpt.PersistentBlocks)
  {
    EdOpt.PersistentBlocks=NewMode;
    Edit *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->SetPersistentBlocks(NewMode);
      CurPtr=CurPtr->m_next;
    }
  }
}
/* IS $ */

/* $ 26.02.2001 IS
     "������ �� ��������� ������"
*/
void Editor::SetCursorBeyondEOL(int NewMode)
{
  if(NewMode!=EdOpt.CursorBeyondEOL)
  {
    EdOpt.CursorBeyondEOL=NewMode;
    Edit *CurPtr=TopList;
    while (CurPtr!=NULL)
    {
      CurPtr->SetEditBeyondEnd(NewMode);
      CurPtr=CurPtr->m_next;
    }
  }
  /* $ 16.10.2001 SKV
    ���� ������������� ���� ���� ���� �����,
    �� ��-�� ���� ����� ��������� ������� �����
    ��� ��������� ������������ ������.
  */
  if(EdOpt.CursorBeyondEOL)
  {
    MaxRightPos=0;
  }
  /* SKV$*/
}
/* IS $ */

/* $ 29.10.200 IS
     ������ � ����������� "��������� ������� �����" �
     "��������� ��������" ����� ����� �������� �� alt-shift-f9.
*/
void Editor::GetSavePosMode(int &SavePos, int &SaveShortPos)
{
   SavePos=EdOpt.SavePos;
   SaveShortPos=EdOpt.SaveShortPos;
}

// ����������� � �������� �������� ��������� "-1" ��� ���������,
// ������� �� ����� ������
void Editor::SetSavePosMode(int SavePos, int SaveShortPos)
{
   if(SavePos!=-1)
      EdOpt.SavePos=SavePos;
   if(SaveShortPos!=-1)
      EdOpt.SaveShortPos=SaveShortPos;
}
/* IS $ */

void Editor::EditorShowMsg(const char *Title,const char *Msg, const char* Name)
{
  Message(0,0,Title,Msg,Name);
  PreRedrawParam.Param1=(void *)Title;
  PreRedrawParam.Param2=(void *)Msg;
  PreRedrawParam.Param3=(void *)Name;
}

void Editor::PR_EditorShowMsg(void)
{
  Editor::EditorShowMsg((char*)PreRedrawParam.Param1,(char*)PreRedrawParam.Param2,(char*)PreRedrawParam.Param3);
}

Edit *Editor::CreateString (const char *lpszStr, int nLength)
{
  Edit *pEdit = new Edit (this);

  if ( pEdit )
  {
    pEdit->m_next = NULL;
    pEdit->m_prev = NULL;
    pEdit->SetTabSize (EdOpt.TabSize);
    pEdit->SetPersistentBlocks (EdOpt.PersistentBlocks);
    pEdit->SetConvertTabs (EdOpt.ExpandTabs);
    if ( lpszStr )
      pEdit->SetBinaryString (lpszStr, nLength);
    //pEdit->SetPosition(X1,0,X2,0);
    pEdit->SetCurPos (0);
    pEdit->SetObjectColor (COL_EDITORTEXT,COL_EDITORSELECTEDTEXT);
    pEdit->SetEditorMode (TRUE);
    pEdit->SetWordDiv (EdOpt.WordDiv);
  }

  return pEdit;
}

bool Editor::AddString (const char *lpszStr, int nLength)
{
  Edit *pNewEdit = CreateString (lpszStr, nLength);

  if ( !pNewEdit )
    return false;

  if ( !TopList || !NumLastLine ) //???
    TopList = EndList = TopScreen = CurLine = pNewEdit;
  else
  {
    Edit *PrevPtr;

    EndList->m_next = pNewEdit;

    PrevPtr = EndList;
    EndList = EndList->m_next;
    EndList->m_prev = PrevPtr;
    EndList->m_next = NULL;
  }

  NumLastLine++;

  return true;
}


void Editor::SetDialogParent(DWORD Sets)
{
}

void Editor::SetOvertypeMode(int Mode)
{
}

int Editor::GetOvertypeMode()
{
  return 0;
}

void Editor::SetEditBeyondEnd(int Mode)
{
}

void Editor::SetClearFlag(int Flag)
{
}

int Editor::GetClearFlag(void)
{
  return 0;
}

int Editor::GetCurCol()
{
  return CurLine->GetCurPos();
}

void Editor::SetCurPos(int NewCol, int NewRow)
{
  Lock ();
  GoToLine(NewRow);
  CurLine->SetTabCurPos(NewCol);
  //CurLine->SetLeftPos(LeftPos); ???
  Unlock ();
}

void Editor::SetCursorType(int Visible,int Size)
{
  CurLine->SetCursorType(Visible,Size); //???
}

void Editor::GetCursorType(int &Visible,int &Size)
{
  CurLine->GetCursorType(Visible,Size); //???
}

void Editor::SetObjectColor(int Color,int SelColor,int ColorUnChanged)
{
  for (Edit *CurPtr=TopList;CurPtr!=NULL;CurPtr=CurPtr->m_next) //???
    CurPtr->SetObjectColor (Color,SelColor,ColorUnChanged);
}
