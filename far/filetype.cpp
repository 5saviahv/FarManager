/*
filetype.cpp

������ � ������������ ������

*/

/* Revision: 1.38 20.03.2002 $ */

/*
Modify:
  20.03.2002 IS
    ! ������� const (��-�� PrepareOSIfExist)
  18.03.2002 SVS
    ! ������������ ��������� (��� �������)
  05.03.2002 DJ
    - ��������� ������ ������ � SubstFileName
  14.02.2002 VVM
    ! UpdateIfChanged ��������� �� ������� Force, � �������� �� UIC_*
  11.02.2002 SVS
    ! ��������� ������ ����� �� 25.01.2002 - ������ ����.
  30.01.2002 SVS
    - ������ ���������� �� ����������� �����.
  29.01.2002 SVS
    - BugZ#272 - ����������� � ����������� ������
  25.01.2002 SVS
    - ���� � �����������, ���� � ����������� ������ ������������ �������� "@"
      ������ �������... ������ ������� � ������������...
  28.10.2001 tran 1.30
    - ��� � ������ ����, ��� ������ ������ ����� ������ ��������������
      �� ���, ��� ����.
  14.08.2001 SVS
    - �������� � ���������� - ����� ��� ��������� � �� ����� ������� ������
      ������������ �������
  13.08.2001 SVS
    - �������� � ���������� - ��� ��� �������� ��� ����� �������
  03.08.2001 SVS
    ! ��������� ������� �������
    ! ��� ���� ������ ���������� � ������� ��������� ���������� ������������
      ���� ����������� ���� ������.
  02.08.2001 IS
    ! ��� ��������� ���������, ����������� � �����������, �������� � �����
      �����.
    + ������ � ������ ��������� � �����������: alt-f3, alt-f4, ctrl-pgdn
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  18.07.2001 OT
    ! VFMenu
  06.07.2001 IS
    + ������ � ����������� ������ ����� ������������ ����� ����������, �����
      ������ ����� ����� � �������, ����� ������ ����������� �� ������������,
      ����� ������������ ����� � ������� � �������� ����������� �����.
    - ����� ���������� ��� ������ �� ������������� ����� ������ ���� "*.*"
      (��� �����-��, �����, ����� ���-������).
  25.06.2001 IS
    ! ��������� const
  18.06.2001 SVS
    ! SubstFileName � ReplaceVariables ��������� � ����� ������ fnparce.cpp
  06.06.2001 SVS
    ! Mix/Max
  21.05.2001 SVS
    ! ��������� MENU_ - � ����
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
  17.05.2001 SVS
    ! ��� ����������� !? ��� ������ �������������� �������� ����
      DIF_USELASTHISTORY - ��� ���������� ��������� ���� ��� �������� :-)
  07.05.2001 SVS
    ! SysLog(); -> _D(SysLog());
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  24.04.2001 DJ
    * ��������� @ � ����������� IF EXIST
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  27.02.2001 VVM
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
  14.02.2001 SVS
    ! ������������ ��� !@! � !$! - AFQS
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  14.01.2001 SVS
    + ��������������� ��������� ������ ����������
    + ������������ !&, !&~ � !^
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  01.11.2000 IS
    - ��� ����� � ������ !$! ������ ���� ��������
  02.09.2000 tran
    - !@!, !#!@! bug
  01.08.2000 SVS
    ! ���������, �������� ������� � ���������� DialogItem
    ! � usermenu ����������� !?<title>?<init>! � ����������� ���������� �����!
  21.07.2000 IG
    - Bug 15 (�� �������� �������� executable.exe !.!?ext:?!)
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "fn.hpp"
#include "global.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "dialog.hpp"
#include "vmenu.hpp"
#include "plognmn.hpp"
#include "ctrlobj.hpp"
#include "cmdline.hpp"
#include "history.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "rdrwdsk.hpp"
#include "savescr.hpp"
#include "CFileMask.hpp"

static int DeleteTypeRecord(int DeletePos);
static int EditTypeRecord(int EditPos,int TotalRecords,int NewRec);
static int GetDescriptionWidth();

static unsigned char VerticalLine=0x0B3;

/* $ 02.08.2001 IS
     ��� ������ ��������� ��������� ��� ����������
*/
struct FileTypeStrings
{
  char *Help,*HelpModify,
  *TypeFmt, *Execute, *Desc, *Mask, *View, *Edit,
  *AltExec, *AltView, *AltEdit;
};
const FileTypeStrings FTS=
{
  "FileAssoc","FileAssocModify",
  "Associations\\Type%d","Execute","Description","Mask","View","Edit",
  "AltExec","AltView","AltEdit"
};
/* IS $ */

/* $ 25.04.2001 DJ
   ��������� @ � IF EXIST: �������, ������� ��������� ������� �� ������
   � IF EXIST � ������ @ � ���������� TRUE, ���� ������� IF EXIST
   ��������, � FALSE � ��������� ������/
*/

BOOL ExtractIfExistCommand (char *CommandText)
{
  const char *PtrCmd=PrepareOSIfExist(CommandText);
  if(PtrCmd)
  {
    if(!*PtrCmd) // ��! ������� �� ���������!!!
                 // (��������, ���� ������������� ������, � ��� �����)
                 // �����-�� ������� ��������� ���� ����!
      return FALSE;
    // �������� "if exist"
    if (*CommandText == '@')
      memmove(CommandText+1,PtrCmd,strlen(PtrCmd)+1);
    else
      memmove(CommandText,PtrCmd,strlen(PtrCmd)+1);
  }
  return TRUE;
}

/* DJ $ */


/* $ 14.01.2001 SVS
   ������� �����������������.
   ���� ����������� "IF" � ��� �����������, �� �������
   ���������� � ������

   ���������� ��� F3, F4 - ����������
   Enter � ��� ������ - ����������.
*/
/* $ 06.07.2001
   + ���������� CFileMask ������ GetCommaWord, ���� ����� ���������� ����, ���
     ����� ������������ ����� ����������
   - ����� ���������� ��� ������ �� ������������� ����� ������ ���� "*.*"
     (��� �����-��, �����, ����� ���-������)
*/
int ProcessLocalFileTypes(char *Name,char *ShortName,int Mode,int AlwaysWaitFinish)
{
  CFileMask FMask; // ��� ������ � ������� ������
  char Commands[32][512],Descriptions[32][128],Command[4096];
  int NumCommands[32];
  int CommandCount=0;
  for (int I=0;;I++)
  {
    char RegKey[80],Mask[512];
    sprintf(RegKey,FTS.TypeFmt,I);
    if (!GetRegKey(RegKey,FTS.Mask,Mask,"",sizeof(Mask)))
      break;
    if(FMask.Set(Mask, FMF_SILENT))
    {
      char NewCommand[4096];
      if(FMask.Compare(Name))
      {
        switch(Mode)
        {
          case FILETYPE_EXEC:
            GetRegKey(RegKey,FTS.Execute,NewCommand,"",sizeof(NewCommand));
            break;
          case FILETYPE_VIEW:
            GetRegKey(RegKey,FTS.View,NewCommand,"",sizeof(NewCommand));
            break;
          case FILETYPE_EDIT:
            GetRegKey(RegKey,FTS.Edit,NewCommand,"",sizeof(NewCommand));
            break;
          /* $ 02.08.2001 IS ����� �������: alt-f3, alt-f4, ctrl-pgdn */
          case FILETYPE_ALTEXEC:
            GetRegKey(RegKey,FTS.AltExec,NewCommand,"",sizeof(NewCommand));
            break;
          case FILETYPE_ALTVIEW:
            GetRegKey(RegKey,FTS.AltView,NewCommand,"",sizeof(NewCommand));
            break;
          case FILETYPE_ALTEDIT:
            GetRegKey(RegKey,FTS.AltEdit,NewCommand,"",sizeof(NewCommand));
            break;
          /* IS $ */
          default:
            *NewCommand=0; // ������� �� ������ ��������
        }

        if (*NewCommand && CommandCount<sizeof(Commands)/sizeof(Commands[0]))
        {
          strcpy(Command,NewCommand);
          strcpy(Commands[CommandCount],Command);
          GetRegKey(RegKey,FTS.Desc,Descriptions[CommandCount],"",sizeof(Descriptions[0]));
          CommandCount++;
        }
      }
    }
  }
  if (CommandCount==0)
    return(FALSE);
  if (CommandCount>1)
  {
    struct MenuItem TypesMenuItem;
    VMenu TypesMenu(MSG(MSelectAssocTitle),NULL,0,ScrY-4);
    TypesMenu.SetHelp(FTS.Help);
    TypesMenu.SetFlags(VMENU_WRAPMODE);
    TypesMenu.SetPosition(-1,-1,0,0);

    int DizWidth=GetDescriptionWidth();
    int ActualCmdCount=0; // ������������ ���������� � ����

    for (int I=0;I<CommandCount;I++)
    {
      char CommandText[512],MenuText[512];
      memset(&TypesMenuItem,0,sizeof(TypesMenuItem));
      strcpy(CommandText,Commands[I]);
      SubstFileName(CommandText,sizeof (CommandText),Name,ShortName,NULL,NULL,TRUE);

      // ��� "�����������", ������ �������� ������� "if exist"
      /* $ 25.04.2001 DJ
         ��������� @ � IF EXIST
      */
      if (!ExtractIfExistCommand (CommandText))
        continue;
      /* DJ $ */

      // �������� ������ ������������ ������� �� ������� Commands
      NumCommands[ActualCmdCount++]=I;

      if (DizWidth==0)
        *MenuText=0;
      else
      {
        char Title[4096];
        if (*Descriptions[I])
        {
          strcpy(Title,Descriptions[I]);
          SubstFileName(Title,sizeof (Title), Name,ShortName,NULL,NULL,TRUE);
        }
        else
          *Title=0;
        char *PtrAmp;
        int Ampersand=(PtrAmp=strchr(Title,'&'))!=NULL;
        if(DizWidth+Ampersand > ScrX/2 && PtrAmp && PtrAmp-Title > DizWidth)
          Ampersand=0;
        sprintf(MenuText,"%-*.*s %c ",DizWidth+Ampersand,DizWidth+Ampersand,Title,VerticalLine);
      }
      TruncStr(CommandText,Min(ScrX,(int)sizeof(TypesMenuItem.Name)-1)-DizWidth-14);
      strcat(MenuText,CommandText);
      TruncStr(MenuText,sizeof(TypesMenuItem.Name)-1);
      strcpy(TypesMenuItem.Name,MenuText);
      TypesMenuItem.SetSelect(I==0);
      TypesMenu.AddItem(&TypesMenuItem);
    }
    if(!ActualCmdCount)
      return(TRUE);
    TypesMenu.Process();
    int ExitCode=TypesMenu.Modal::GetExitCode();
    if (ExitCode<0)
      return(TRUE);
    strcpy(Command,Commands[NumCommands[ExitCode]]);
  }

  /* $ 02.09.2000 tran
     [NM] -> [NM*2] */
  char ListName[NM*2],ShortListName[NM*2];
  {
    int PreserveLFN=SubstFileName(Command,sizeof (Command), Name,ShortName,ListName,ShortListName);

    // ����� ��� "�����������", ������ �������� ������� "if exist"
    /* $ 25.04.2001 DJ
       ��������� @ � IF EXIST
    */
    if (!ExtractIfExistCommand (Command))
      return TRUE;
    /* DJ $ */
    PreserveLongName PreserveName(ShortName,PreserveLFN);
    if (*Command)
      if (*Command!='@')
      {
        CtrlObject->CmdLine->ExecString(Command,AlwaysWaitFinish);
        if (!AlwaysWaitFinish)
          CtrlObject->CmdHistory->AddToHistory(Command);
      }
      else
      {
#if 1
        SaveScreen SaveScr;
        CtrlObject->Cp()->LeftPanel->CloseFile();
        CtrlObject->Cp()->RightPanel->CloseFile();
        Execute(Command+1,AlwaysWaitFinish);
#else
        // ����� ���� ���� � ����������� (�... ����� ������
        // �� ������� "@type !@!" �������� � ������)
        // ������� �� �������� � CommandLine::CmdExecute()
        {
          RedrawDesktop RdrwDesktop(TRUE);
          Execute(Command+1,AlwaysWaitFinish);
          ScrollScreen(1); // �����������, ����� ���������� RedrawDesktop
                           // ����������� ����� ������ ��������� ������ ������.
        }
        CtrlObject->Cp()->LeftPanel->UpdateIfChanged(UIC_UPDATE_FORCE);
        CtrlObject->Cp()->RightPanel->UpdateIfChanged(UIC_UPDATE_FORCE);
        CtrlObject->Cp()->Redraw();
#endif
      }
  }
  /* $ 02.09.2000 tran
     remove 4 files, not 2*/
  if (*ListName)
    remove(ListName);
  if (*(ListName+NM))
    remove(ListName+NM);
  if (*ShortListName)
    remove(ShortListName);
  if (*(ShortListName+NM))
    remove(ShortListName+NM);
  /* tran $ */
  return(TRUE);
}
/* IS $ */
/* SVS $ */


int ProcessGlobalFileTypes(char *Name,int AlwaysWaitFinish)
{
  char Value[80],*ExtPtr;
  LONG ValueSize;
  if ((ExtPtr=strrchr(Name,'.'))==NULL)
    return(FALSE);
  ValueSize=sizeof(Value);
  if (RegQueryValue(HKEY_CLASSES_ROOT,(LPCTSTR)ExtPtr,(LPTSTR)Value,&ValueSize)!=ERROR_SUCCESS)
    return(FALSE);
  if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion<4)
  {
    char AssocStr[2*NM],ExpAssocStr[2*NM],*ChPtr;
    strcat(Value,"\\shell\\open\\command");
    HKEY hKey;
    if (RegOpenKey(HKEY_CLASSES_ROOT,Value,&hKey)!=ERROR_SUCCESS)
      return(FALSE);
    ValueSize=sizeof(AssocStr);
#if defined(__BORLANDC__) && (__BORLANDC__ <= 0x520)
    if (RegQueryValueEx(hKey,"",NULL,NULL,(LPTSTR)AssocStr,(LPDWORD)&ValueSize)!=ERROR_SUCCESS)
#else
    if (RegQueryValueEx(hKey,"",NULL,NULL,(unsigned char *)AssocStr,(LPDWORD)&ValueSize)!=ERROR_SUCCESS)
#endif
      return(FALSE);
    RegCloseKey(hKey);
    ExpandEnvironmentStrings(AssocStr,ExpAssocStr,sizeof(ExpAssocStr));
    if ((ChPtr=strstr(ExpAssocStr,"%*"))!=NULL)
    {
      char TmpStr[512];
      strcpy(TmpStr,ChPtr+2);
      strcpy(ChPtr,TmpStr);
    }
    if ((ChPtr=strstr(ExpAssocStr,"%1"))!=NULL)
    {
      char TmpStr[512];
      strcpy(TmpStr,ChPtr+2);
      strcpy(ChPtr,Name);
      strcat(ExpAssocStr,TmpStr);
    }
    else
    {
      char ExecStr[NM];
      if (FindExecutable(Name,"",ExecStr)<=(HINSTANCE)32)
        return(FALSE);
      sprintf(ExpAssocStr,"%s %s",QuoteSpace(ExecStr),QuoteSpace(Name));
    }

    if (*ExpAssocStr!='"')
      for (int I=0;ExpAssocStr[I]!=0;I++)
        if (strnicmp(&ExpAssocStr[I],".exe",4)==0 &&
            (ExpAssocStr[I+4]==' ' || ExpAssocStr[I+4]=='/'))
        {
          int SpacePresent=0;
          for (int J=0;J<I;J++)
            if (ExpAssocStr[J]==' ')
            {
              SpacePresent=1;
              break;
            }
          if (SpacePresent)
          {
            char NewStr[2*NM];
            strncpy(NewStr,ExpAssocStr,I+4);
            NewStr[I+4]=0;
            QuoteSpace(NewStr);
            strcat(NewStr,&ExpAssocStr[I+4]);
            strcpy(ExpAssocStr,NewStr);
            QuoteSpace(ExpAssocStr);
          }
          break;
        }
    CtrlObject->CmdLine->ExecString(ExpAssocStr,AlwaysWaitFinish);
  }
  else
  {
/*
    char FullName[2*NM];
    ConvertNameToFull(Name,FullName, sizeof(FullName));
    if (FullName[0]=='\\' && FullName[1]=='\\')
      strcpy(Name,FullName);
    else
      if ((strstr(Name,"ftp")!=NULL || strstr(Name,"www")!=NULL) && PointToName(Name)==Name)
      {
        sprintf(FullName,".\\%s",Name);
        strcpy(Name,FullName);
      }

    QuoteSpace(Name);
*/
    CtrlObject->CmdLine->ExecString(Name,AlwaysWaitFinish,2,FALSE);
    if (!AlwaysWaitFinish)
    {
      char QuotedName[2*NM];
      strcpy(QuotedName,Name);
      QuoteSpace(QuotedName);
      CtrlObject->CmdHistory->AddToHistory(QuotedName);
    }
  }
  return(TRUE);
}

/*
  ������������ ��� ������� �������� ��������� � �������
*/
void ProcessExternal(char *Command,char *Name,char *ShortName,int AlwaysWaitFinish)
{
  char ExecStr[4096],FullExecStr[4096];
  char ListName[NM*2],ShortListName[NM*2];
  char FullName[4096],FullShortName[NM];
  strcpy(ExecStr,Command);
  strcpy(FullExecStr,Command);
  {
    int PreserveLFN=SubstFileName(ExecStr,sizeof (ExecStr), Name,ShortName,ListName,ShortListName);
    // ����� ��� "�����������", ������ �������� ������� "if exist"
    /* $ 25.04.2001 DJ
       ��������� @ � IF EXIST
    */
    if (!ExtractIfExistCommand (ExecStr))
      return;
    /* DJ $ */

    PreserveLongName PreserveName(ShortName,PreserveLFN);

//    ConvertNameToFull(Name,FullName, sizeof(FullName));
    if (ConvertNameToFull(Name,FullName, sizeof(FullName)) >= sizeof(FullName)){
      return;
    }
    ConvertNameToShort(FullName,FullShortName);
    SubstFileName(FullExecStr,sizeof (FullExecStr), FullName,FullShortName,ListName,ShortListName);
    // ����� ��� "�����������", ������ �������� ������� "if exist"
    /* $ 25.04.2001 DJ
       ��������� @ � IF EXIST
    */
    if (!ExtractIfExistCommand (FullExecStr))
      return;
    /* DJ $ */
    CtrlObject->ViewHistory->AddToHistory(FullExecStr,MSG(MHistoryExt),(AlwaysWaitFinish&1)+2);

    if (*ExecStr!='@')
      CtrlObject->CmdLine->ExecString(ExecStr,AlwaysWaitFinish);
    else
    {
      SaveScreen SaveScr;
      CtrlObject->Cp()->LeftPanel->CloseFile();
      CtrlObject->Cp()->RightPanel->CloseFile();
      Execute(ExecStr+1,AlwaysWaitFinish);
    }
  }
  if (*ListName)
    remove(ListName);
  if (ListName[NM])
    remove(ListName+NM);
  if (*ShortListName)
    remove(ShortListName);
  if (ShortListName[NM])
    remove(ShortListName+NM);
}

static int FillFileTypesMenu(VMenu *TypesMenu,int MenuPos)
{
  int NumLine=0;
  int DizWidth=GetDescriptionWidth();
  struct MenuItem TypesMenuItem;

  TypesMenu->DeleteItems();
  while (1)
  {
    char RegKey[80],Mask[512],MenuText[512];
    sprintf(RegKey,FTS.TypeFmt,NumLine);
    memset(&TypesMenuItem,0,sizeof(TypesMenuItem));
    if (!GetRegKey(RegKey,FTS.Mask,Mask,"",sizeof(Mask)))
      break;
    if (DizWidth==0)
      *MenuText=0;
    else
    {
      char Title[256],Description[128];
      GetRegKey(RegKey,FTS.Desc,Description,"",sizeof(Description));
      if (*Description)
        strcpy(Title,Description);
      else
        *Title=0;
      char *PtrAmp;
      int Ampersand=(PtrAmp=strchr(Title,'&'))!=NULL;
      if(DizWidth+Ampersand > ScrX/2 && PtrAmp && PtrAmp-Title > DizWidth)
        Ampersand=0;
      sprintf(MenuText,"%-*.*s %c ",DizWidth+Ampersand,DizWidth+Ampersand,Title,VerticalLine);
    }
    TruncStr(Mask,Min(ScrX,(int)sizeof(TypesMenuItem.Name)-1)-DizWidth-14);
    strcat(MenuText,Mask);
    TruncStr(MenuText,sizeof(TypesMenuItem.Name)-1);
    strcpy(TypesMenuItem.Name,MenuText);
    TypesMenuItem.SetSelect(NumLine==MenuPos);
    TypesMenu->AddItem(&TypesMenuItem);
    NumLine++;
  }
  *TypesMenuItem.Name=0;
  TypesMenuItem.SetSelect(NumLine==MenuPos);
  TypesMenu->AddItem(&TypesMenuItem);
  return NumLine;
}

void EditFileTypes(int MenuPos)
{
  int NumLine;
  int m;
  BOOL MenuModified;

  VMenu TypesMenu(MSG(MAssocTitle),NULL,0,ScrY-4);
  TypesMenu.SetHelp(FTS.Help);
  TypesMenu.SetFlags(VMENU_WRAPMODE);
  TypesMenu.SetPosition(-1,-1,0,0);
  TypesMenu.SetBottomTitle(MSG(MAssocBottom));

  {
    while (1)
    {
      MenuModified=TRUE;
      while (!TypesMenu.Done())
      {
        if (MenuModified==TRUE)
        {
          TypesMenu.Hide();
          NumLine=FillFileTypesMenu(&TypesMenu,MenuPos);
          TypesMenu.SetPosition(-1,-1,-1,-1);
          TypesMenu.Show();
          MenuModified=FALSE;
        }
        MenuPos=TypesMenu.GetSelectPos();
        switch(TypesMenu.ReadInput())
        {
          case KEY_DEL:
            if (MenuPos<NumLine)
              DeleteTypeRecord(MenuPos);
            MenuModified=TRUE;
            break;
          case KEY_INS:
            EditTypeRecord(MenuPos,NumLine,1);
            MenuModified=TRUE;
            break;
          case KEY_ENTER:
          case KEY_F4:
            if (MenuPos<NumLine)
              EditTypeRecord(MenuPos,NumLine,0);
            MenuModified=TRUE;
            break;
          default:
            TypesMenu.ProcessInput();
            break;
        }
      }
      /* $ 28.10.2001 tran
         �������� ������������ �������� */
      m=TypesMenu.Modal::GetExitCode();
      if (m!=-1)
      {
        /* $ 28.10.2001 tran
           � ���������� ��� - ����� ����� ������� ����� hotkey */
        MenuPos=m;
        /* tran $ */
        TypesMenu.ClearDone();
        TypesMenu.WriteInput(KEY_F4);
        continue;
      }
      break;
    }
  }
}


int DeleteTypeRecord(int DeletePos)
{
  char RecText[200],ItemName[200],RegKey[80];
  sprintf(RegKey,FTS.TypeFmt,DeletePos);
  GetRegKey(RegKey,FTS.Mask,RecText,"",sizeof(RecText));
  sprintf(ItemName,"\"%s\"",RecText);
  if (Message(MSG_WARNING,2,MSG(MAssocTitle),MSG(MAskDelAssoc),
              ItemName,MSG(MDelete),MSG(MCancel))!=0)
    return(FALSE);
  DeleteKeyRecord(FTS.TypeFmt,DeletePos);
  return(TRUE);
}


/* $ 02.08.2001 IS
   ���������� ����� ������� (��� alt-f3, alt-f4, ctrl-pgdn)
*/
int EditTypeRecord(int EditPos,int TotalRecords,int NewRec)
{
  const char *HistoryName="Masks";

  static struct DialogData EditDlgData[]={
/* 00 */ DI_DOUBLEBOX,3,1,72,21,0,0,0,0,(char *)MFileAssocTitle,
/* 01 */ DI_TEXT,5,2,0,0,0,0,0,0,(char *)MFileAssocMasks,
/* 02 */ DI_EDIT,5,3,70,3,1,(DWORD)HistoryName,DIF_HISTORY,0,"",
/* 03 */ DI_TEXT,5,4,0,0,0,0,0,0,(char *)MFileAssocDescr,
/* 04 */ DI_EDIT,5,5,70,3,0,0,0,0,"",
/* 05 */ DI_TEXT,3,6,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
/* 06 */ DI_TEXT,5,7,0,0,0,0,0,0,(char *)MFileAssocExec,
/* 07 */ DI_EDIT,5,8,70,3,0,0,0,0,"",
/* 08 */ DI_TEXT,5,9,0,0,0,0,0,0,(char *)MFileAssocAltExec,
/* 09 */ DI_EDIT,5,10,70,3,0,0,0,0,"",
/* 10 */ DI_TEXT,5,11,0,0,0,0,0,0,(char *)MFileAssocView,
/* 11 */ DI_EDIT,5,12,70,3,0,0,0,0,"",
/* 12 */ DI_TEXT,5,13,0,0,0,0,0,0,(char *)MFileAssocAltView,
/* 13 */ DI_EDIT,5,14,70,3,0,0,0,0,"",
/* 14 */ DI_TEXT,5,15,0,0,0,0,0,0,(char *)MFileAssocEdit,
/* 15 */ DI_EDIT,5,16,70,3,0,0,0,0,"",
/* 16 */ DI_TEXT,5,17,0,0,0,0,0,0,(char *)MFileAssocAltEdit,
/* 17 */ DI_EDIT,5,18,70,3,0,0,0,0,"",
/* 18 */ DI_TEXT,3,19,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
/* 19 */ DI_BUTTON,0,20,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
/* 20 */ DI_BUTTON,0,20,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(EditDlgData,EditDlg);

  char RegKey[80];
  sprintf(RegKey,FTS.TypeFmt,EditPos);
  if (!NewRec)
  {
    GetRegKey(RegKey,FTS.Mask,EditDlg[2].Data,"",sizeof(EditDlg[2].Data));
    GetRegKey(RegKey,FTS.Desc,EditDlg[4].Data,"",sizeof(EditDlg[4].Data));
    GetRegKey(RegKey,FTS.Execute,EditDlg[7].Data,"",sizeof(EditDlg[7].Data));
    GetRegKey(RegKey,FTS.AltExec,EditDlg[9].Data,"",sizeof(EditDlg[9].Data));
    GetRegKey(RegKey,FTS.View,EditDlg[11].Data,"",sizeof(EditDlg[11].Data));
    GetRegKey(RegKey,FTS.AltView,EditDlg[13].Data,"",sizeof(EditDlg[13].Data));
    GetRegKey(RegKey,FTS.Edit,EditDlg[15].Data,"",sizeof(EditDlg[15].Data));
    GetRegKey(RegKey,FTS.AltEdit,EditDlg[17].Data,"",sizeof(EditDlg[17].Data));
  }

  {
    Dialog Dlg(EditDlg,sizeof(EditDlg)/sizeof(EditDlg[0]));
    Dlg.SetHelp(FTS.HelpModify);
    Dlg.SetPosition(-1,-1,76,23);
    /* $ 06.07.2001 IS
       ��������� �������� ����� ������ �� ������������
    */
    CFileMask FMask;
    for(;;)
    {
      Dlg.ClearDone();
      Dlg.Process();
      if (Dlg.GetExitCode()!=19 || *EditDlg[2].Data==0)
        return(FALSE);
      if(FMask.Set(EditDlg[2].Data, 0))
        break;
    }
    /* IS $ */
  }

  if (NewRec)
    InsertKeyRecord(FTS.TypeFmt,EditPos,TotalRecords);
  SetRegKey(RegKey,FTS.Mask,EditDlg[2].Data);
  SetRegKey(RegKey,FTS.Desc,EditDlg[4].Data);
  SetRegKey(RegKey,FTS.Execute,EditDlg[7].Data);
  SetRegKey(RegKey,FTS.AltExec,EditDlg[9].Data);
  SetRegKey(RegKey,FTS.View,EditDlg[11].Data);
  SetRegKey(RegKey,FTS.AltView,EditDlg[13].Data);
  SetRegKey(RegKey,FTS.Edit,EditDlg[15].Data);
  SetRegKey(RegKey,FTS.AltEdit,EditDlg[17].Data);

  return(TRUE);
}
/* IS $ */

int GetDescriptionWidth()
{
  int Width=0,NumLine=0;
  while (1)
  {
    char RegKey[80],Mask[512],Description[128];
    sprintf(RegKey,FTS.TypeFmt,NumLine);
    if (!GetRegKey(RegKey,FTS.Mask,Mask,"",sizeof(Mask)))
      break;
    GetRegKey(RegKey,FTS.Desc,Description,"",sizeof(Description));
    int CurWidth=HiStrlen(Description);
    if (CurWidth>Width)
      Width=CurWidth;
    NumLine++;
  }
  if (Width>ScrX/2)
    Width=ScrX/2;
  return(Width);
}
