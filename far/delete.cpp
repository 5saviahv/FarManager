/*
delete.cpp

�������� ������

*/

/* Revision: 1.44 22.03.2002 $ */

/*
Modify:
  22.03.2002 SVS
    ! ������� DeleteFileWithFolder(), DeleteDirTree() �� mix.cpp �
      delete.cpp ��� ����� �� �����.
  18.03.2002 SVS
    - "Broke link" - ���������� ��������� ������ (����� �� �� �������)
  01.03.2002 SVS
    ! ���� ������ ���� ������� �������� ���������� ����� - FarMkTempEx
  22.02.2002 SVS
    - Bug in panels refreshing after cancelling directory delete
  13.02.2002 SVS
    ! ������ ���������
  24.01.2002 VVM
    ! ��� �������� � ������� � ������ - ������� �� ���� ��� ��
  19.01.2002 VVM
    ! bug#253 - ������� �������, � �� ����� �������� ������� ��� ��������� SetPreredrawFunc()
  15.01.2002 SVS
    - ���� - ��� �������� �������� �� ���������� ���� ����, ��� ��
      ��������������� ���������� ���������� ��������, � ��� ����� ���� ������
      OS ������� �� ���� ������������.
  14.01.2002 IS
    ! chdir -> FarChDir
  08.11.2001 SVS
    ! ��������� ������ - ������� �������.
  06.11.2001 SVS
    ! ������ ������ ��� �������� ������ � ����������� ��������� �����������
      �������� � ������� ���������� (� ������ SF), ������� � min 30 ��������.
  24.10.2001 SVS
    - ������ ���� MSG_KEEPBACKGROUND ��� ������.
  23.10.2001 SVS
    ! ������� ��������� �� ������ ������ �������� ��������������� �����
  22.10.2001 SVS
    - �������� � ����������� ����� ��������� CALLBACK-������� (����� 1 ������
      �������� - �������� ����� ����������)
  21.10.2001 SVS
    + CALLBACK-������� ��� ���������� �� BugZ#85
  01.10.2001 IS
    ! ����������� ������� ����� �������� ��������, ����� ������ ��� ������� ��
      ���������
  26.09.2001 SVS
    + Opt.AutoUpdateLimit -  ���� ����� ���������� �� ��������� ���������
      ������ (���� �� ���������� �� ����� ��������).
  25.07.2001 IS
    ! ��� �������� ������ ��������� ����� �� ��� � ������ (�� 820).
  19.07.2001 SVS
    ! ������ 826-�� �� ������ ������ (�� ������� VVM)
  18.07.2001 VVM
    ! ��� �������� ����������� �� �� ������ ������, � �� ����� ������� MDeleting + 8
  13.07.2001 SVS
    - "����� � ������" ;-(
  13.07.2001 IS
    ! �������� ���������, ���������� ��� ���������� �����, � �������� ��� ���
      ������ TruncPathStr
  11.07.2001 OT
    ������� CtrlAltShift � Manager
  09.07.2001 SVS
    ! ��������� ������� ������ ����� ���������� �����.
  19.06.2001 SVS
    ! �������� � ������� ������ ���  FIXED-������
  06.06.2001 SVS
    ! Mix/Max
  31.05.2001 OT
    - ��������� MessageBox �� ����� ��������.
  31.05.2001 SVS
    - �� ����������� ������ ����� �������� Unlink
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  24.04.2001 SVS
    ! ��� symlink`� �� ����� �������������� ������������� �� ��������
  14.03.2001 SVS
    - �������� ������ ���� �������� ������� SHFileOperation(),
      ���� ���� ��������� � �������.
  13.03.2001 SVS
    + ��������� "��������" ������ - Part I
  13.03.2001 SVS
    - �������� �������� � ������� ������� ������� ���������!!!!!!
  12.03.2001 SVS
    + Opt.DeleteSymbolWipe -> Opt.WipeSymbol
  12.03.2001 SVS
    + Opt.DeleteSymbolWipe ������ ����������� ��� "ZAP-��������"
  07.03.2001 SVS
    - ������� ���� � ���� :-)))
  05.01.2001 SVS
    ! � ����������� �� ����� ������ ������ ��������� ��� ��������
  05.01.2001 IS
    ! ��������� � ���������� - ������ ��������� � ����������� �� ����,
      ����� � ������� ��������� ��������.
  28.11.2000 SVS
    + ��������� ���������� ������ � SymLink (�.�. "Directory Junctions")
  11.11.2000 SVS
    ! ���������: "FarTmpXXXXXX" �������� �� ���������� FarTmpXXXXXX
    - ��������� ��������� ��� � �������� Wipe*
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
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

#include "global.hpp"
#include "lang.hpp"
#include "fn.hpp"
#include "flink.hpp"
#include "panel.hpp"
#include "chgprior.hpp"
#include "filepanels.hpp"
#include "scantree.hpp"
#include "treelist.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "filelist.hpp"
#include "manager.hpp"

static void ShellDeleteMsg(char *Name);
static int AskDeleteReadOnly(char *Name,DWORD Attr);
static int ShellRemoveFile(char *Name,char *ShortName,int Wipe);
static int ERemoveDirectory(char *Name,char *ShortName,int Wipe);
static int RemoveToRecycleBin(char *Name);
static int WipeFile(char *Name);
static int WipeDirectory(char *Name);
static void ShellDeleteUpdatePanels(Panel *SrcPanel,BOOL NeedSetUpADir);
static void PR_ShellDeleteMsg(void);
static int CheckUpdateAnotherPanel(Panel *SrcPanel,char *SelName);

static int ReadOnlyDeleteMode,DeleteAllFolders;

enum {DELETE_SUCCESS,DELETE_YES,DELETE_SKIP,DELETE_CANCEL};

void ShellDelete(Panel *SrcPanel,int Wipe)
{
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  WIN32_FIND_DATA FindData;
  char DeleteFilesMsg[300],SelName[NM],SelShortName[NM],DizName[NM];
  char FullName[2058];
  int SelCount,FileAttr,UpdateDiz;
  int DizPresent;
  int Ret;
  BOOL NeedUpdate=TRUE, NeedSetUpADir=FALSE;

  int Opt_DeleteToRecycleBin=Opt.DeleteToRecycleBin;

/*& 31.05.2001 OT ��������� ����������� �������� ������*/
  Frame *FrameFromLaunched=FrameManager->GetCurrentFrame();
  FrameFromLaunched->LockRefresh();
/* OT &*/

  DeleteAllFolders=!Opt.Confirm.DeleteFolder;

  UpdateDiz=(Opt.Diz.UpdateMode==DIZ_UPDATE_ALWAYS ||
             SrcPanel->IsDizDisplayed() &&
             Opt.Diz.UpdateMode==DIZ_UPDATE_IF_DISPLAYED);

  if ((SelCount=SrcPanel->GetSelCount())==0)
    goto done;

  // �������� � ������� ������ ���  FIXED-������
  {
    char Root[1024];
//    char FSysNameSrc[NM];
    SrcPanel->GetSelName(NULL,FileAttr);
    SrcPanel->GetSelName(SelName,FileAttr);
    ConvertNameToFull(SelName,Root, sizeof(Root));
    GetPathRoot(Root,Root);
//_SVS(SysLog("Del: SelName='%s' Root='%s'",SelName,Root));
    if(GetDriveType(Root) != DRIVE_FIXED)
      Opt.DeleteToRecycleBin=0;
  }

  if (SelCount==1)
  {
    SrcPanel->GetSelName(NULL,FileAttr);
    SrcPanel->GetSelName(SelName,FileAttr);
    if (strcmp(SelName,"..")==0 || *SelName==0)
    {
      NeedUpdate=FALSE;
      goto done;
    }
    strcpy(DeleteFilesMsg,SelName);
    TruncPathStr(DeleteFilesMsg,Min((int)sizeof(DeleteFilesMsg),ScrX-16));
  }
  else
  /* $ 05.01.2001 SVS
     � ����������� �� ����� ������ ������ ���������*/
  {
  /* $ 05.01.2001 IS
  ������ "������" ����� ����������� - "���������"
  */
    char *Ends;
    char StrItems[16];
    itoa(SelCount,StrItems,10);
    int LenItems=strlen(StrItems);
    if((LenItems >= 2 && StrItems[LenItems-2] == '1') ||
           StrItems[LenItems-1] >= '5' ||
           StrItems[LenItems-1] == '0')
      Ends=MSG(MAskDeleteItemsS);
    else if(StrItems[LenItems-1] == '1')
      Ends=MSG(MAskDeleteItems0);
    else
      Ends=MSG(MAskDeleteItemsA);
    sprintf(DeleteFilesMsg,MSG(MAskDeleteItems),SelCount,Ends);
  /* IS $ */
  }
  /* SVS $ */
  Ret=1;

  /* $ 13.02.2001 SVS
     ��������� "��������" ������
  */
  if((FileAttr & FILE_ATTRIBUTE_REPARSE_POINT) && SelCount==1)
  {
    char JuncName[NM];
    if(GetJunctionPointInfo(DeleteFilesMsg,JuncName,sizeof(JuncName)))
    {
      TruncPathStr(JuncName+4,sizeof(JuncName)-4);

      //SetMessageHelp("?????");
      Ret=Message(0,3,MSG(MDeleteTitle),
                DeleteFilesMsg,
                MSG(MAskDeleteLink),
                JuncName+4,
                MSG(MDeleteLinkDelete),MSG(MDeleteLinkUnlink),MSG(MCancel));

      if(Ret == 1)
      {
        ConvertNameToFull(SelName, JuncName, sizeof(JuncName));
        if(Opt.Confirm.Delete)
        {
          ; //  ;-%
        }
        if((NeedSetUpADir=CheckUpdateAnotherPanel(SrcPanel,SelName)) != -1) //JuncName?
        {
          DeleteJunctionPoint(JuncName);
          ShellDeleteUpdatePanels(SrcPanel,NeedSetUpADir);
        }
        goto done;
      }
      if(Ret != 0)
        goto done;
    }
  }
  /* SVS $ */

  if (Ret && (Opt.Confirm.Delete || SelCount>1 || (FileAttr & FA_DIREC)))
  {
    char *DelMsg;
    /* $ 05.01.2001 IS
       ! ��������� � ���������� - ������ ��������� � ����������� �� ����,
         ����� � ������� ��������� ��������.
    */
    BOOL folder=(FileAttr & FA_DIREC);

    if (SelCount==1)
    {
      if (Wipe && !(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT))
        DelMsg=MSG(folder?MAskWipeFolder:MAskWipeFile);
      else
      {
        if (Opt.DeleteToRecycleBin && !(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT))
          DelMsg=MSG(folder?MAskDeleteRecycleFolder:MAskDeleteRecycleFile);
        else
          DelMsg=MSG(folder?MAskDeleteFolder:MAskDeleteFile);
      }
    }
    else
    {
      if (Wipe && !(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT))
        DelMsg=MSG(MAskWipe);
      else
        if (Opt.DeleteToRecycleBin && !(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT))
          DelMsg=MSG(MAskDeleteRecycle);
        else
          DelMsg=MSG(MAskDelete);
    }
    /* IS $ */
    if (Message(0,2,MSG(MDeleteTitle),DelMsg,DeleteFilesMsg,MSG(MDelete),MSG(MCancel))!=0)
    {
      NeedUpdate=FALSE;
      goto done;
    }
  }

  if (Opt.Confirm.Delete && SelCount>1)
  {
    //SaveScreen SaveScr;
    SetCursorType(FALSE,0);
    if (Message(MSG_WARNING,2,MSG(MDeleteFilesTitle),MSG(MAskDelete),
                DeleteFilesMsg,MSG(MDeleteFileAll),MSG(MDeleteFileCancel))!=0)
    {
      NeedUpdate=FALSE;
      goto done;
    }
    SetPreRedrawFunc(PR_ShellDeleteMsg);
    ShellDeleteMsg("");
  }

  if (UpdateDiz)
    SrcPanel->ReadDiz();

  SrcPanel->GetDizName(DizName);
  DizPresent=(*DizName && GetFileAttributes(DizName)!=0xFFFFFFFF);

  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));
  SetFarTitle(MSG(MDeletingTitle));

  if((NeedSetUpADir=CheckUpdateAnotherPanel(SrcPanel,SelName)) == -1)
    goto done;

  if (SrcPanel->GetType()==TREE_PANEL)
    FarChDir("\\");

  {
    int Cancel=0;
    //SaveScreen SaveScr;
    SetCursorType(FALSE,0);
    SetPreRedrawFunc(PR_ShellDeleteMsg);
    ShellDeleteMsg("");

    ReadOnlyDeleteMode=-1;

    SrcPanel->GetSelName(NULL,FileAttr);
    while (SrcPanel->GetSelName(SelName,FileAttr,SelShortName) && !Cancel)
    {
      if (CheckForEsc())
        break;
      int Length=strlen(SelName);
      if (Length==0 || SelName[0]=='\\' && Length<2 ||
          SelName[1]==':' && Length<4)
        continue;
      if (FileAttr & FA_DIREC)
      {
        if (!DeleteAllFolders)
        {
//          ConvertNameToFull(SelName,FullName, sizeof(FullName));
          if (ConvertNameToFull(SelName,FullName, sizeof(FullName)) >= sizeof(FullName)){
            goto done;
          }
          if (IsFolderNotEmpty(FullName))
          {
            int MsgCode=0;
            /* $ 13.07.2001 IS ������� ���, ���� ��� ����������� � ��������� */
            char MsgFullName[NM];
            strcpy(MsgFullName, FullName);
            TruncPathStr(MsgFullName, ScrX-16);
            // ��� symlink`� �� ����� �������������
            if(!(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT))
               MsgCode=Message(MSG_DOWN|MSG_WARNING,4,MSG(MDeleteFolderTitle),
                  MSG(MDeleteFolderConfirm),MsgFullName,
                    MSG(MDeleteFileDelete),MSG(MDeleteFileAll),
                    MSG(MDeleteFileSkip),MSG(MDeleteFileCancel));
            /* IS $ */
            if (MsgCode<0 || MsgCode==3)
            {
              NeedSetUpADir=FALSE;
              break;
            }
            if (MsgCode==1)
              DeleteAllFolders=1;
            if (MsgCode==2)
              continue;
          }
        }

        bool SymLink=(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT)!=0;
        if (!SymLink && (!Opt.DeleteToRecycleBin || Wipe))
        {
          char FullName[NM];
          ScanTree ScTree(TRUE);
          ScTree.SetFindPath(SelName,"*.*");
          while (ScTree.GetNextName(&FindData,FullName))
          {
            if (CheckForEsc())
            {
              Cancel=1;
              break;
            }
            ShellDeleteMsg(FullName);
            char ShortName[NM];
            strcpy(ShortName,FullName);
            if (*FindData.cAlternateFileName)
              strcpy(PointToName(ShortName),FindData.cAlternateFileName);
            if (FindData.dwFileAttributes & FA_DIREC)
            {
              /* $ 28.11.2000 SVS
                 ��������� ���������� ������ � SymLink
                 (�.�. "Directory Junctions")
              */
              if(FindData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
              {
                if (FindData.dwFileAttributes & FA_RDONLY)
                  SetFileAttributes(FullName,0);
                if (ERemoveDirectory(FullName,ShortName,Wipe))
                {
                  TreeList::DelTreeName(FullName);
                  if (UpdateDiz)
                    SrcPanel->DeleteDiz(FullName,SelShortName);
                }
                continue;
              }
              /* SVS $ */
              if (!DeleteAllFolders && !ScTree.IsDirSearchDone() && IsFolderNotEmpty(FullName))
              {
                /* $ 13.07.2001 IS
                     ������� ���, ���� ��� ����������� � ���������
                */
                char MsgFullName[NM];
                strcpy(MsgFullName, FullName);
                TruncPathStr(MsgFullName, ScrX-16);
                int MsgCode=Message(MSG_DOWN|MSG_WARNING,4,MSG(MDeleteFolderTitle),
                      MSG(MDeleteFolderConfirm),MsgFullName,
                      MSG(MDeleteFileDelete),MSG(MDeleteFileAll),
                      MSG(MDeleteFileSkip),MSG(MDeleteFileCancel));
                /* IS $ */
                if (MsgCode<0 || MsgCode==3)
                {
                  Cancel=1;
                  break;
                }
                if (MsgCode==1)
                  DeleteAllFolders=1;
                if (MsgCode==2)
                {
                  ScTree.SkipDir();
                  continue;
                }
              }

              if (ScTree.IsDirSearchDone())
              {
                if (FindData.dwFileAttributes & FA_RDONLY)
                  SetFileAttributes(FullName,0);
                if (ERemoveDirectory(FullName,ShortName,Wipe))
                  TreeList::DelTreeName(FullName);
              }
            }
            else
            {
              int AskCode=AskDeleteReadOnly(FullName,FindData.dwFileAttributes);
              if (AskCode==DELETE_CANCEL)
              {
                Cancel=1;
                break;
              }
              if (AskCode==DELETE_YES)
                if (ShellRemoveFile(FullName,ShortName,Wipe)==DELETE_CANCEL)
                {
                  Cancel=1;
                  break;
                }
            }
          }
        }

        if (!Cancel)
        {
          ShellDeleteMsg(SelName);
          if (FileAttr & FA_RDONLY)
            SetFileAttributes(SelName,0);
          int DeleteCode;
          // ������ ����� ������������, � ���� ������, ��� ��������
          // �������� � ������� ������� ������� ���������.
          if (SymLink || !Opt.DeleteToRecycleBin || Wipe)
            DeleteCode=ERemoveDirectory(SelName,SelShortName,Wipe);
          else
          {
            DeleteCode=RemoveToRecycleBin(SelName);
            if (!DeleteCode)// && WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
              Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),
                      MSG(MCannotDeleteFolder),SelName,MSG(MOk));
          }
          if (DeleteCode)
          {
            TreeList::DelTreeName(SelName);
            if (UpdateDiz)
              SrcPanel->DeleteDiz(SelName,SelShortName);
          }
        }
      }
      else
      {
        ShellDeleteMsg(SelName);
        int AskCode=AskDeleteReadOnly(SelName,FileAttr);
        if (AskCode==DELETE_CANCEL)
          break;
        if (AskCode==DELETE_YES)
        {
          int DeleteCode=ShellRemoveFile(SelName,SelShortName,Wipe);
          if (DeleteCode==DELETE_SUCCESS && UpdateDiz)
            SrcPanel->DeleteDiz(SelName,SelShortName);
          if (DeleteCode==DELETE_CANCEL)
            break;
        }
      }
    }
  }

  if (UpdateDiz)
    if (DizPresent==(*DizName && GetFileAttributes(DizName)!=0xFFFFFFFF))
      SrcPanel->FlushDiz();

  SetConsoleTitle(OldTitle);

done:
  SetPreRedrawFunc(NULL);
  Opt.DeleteToRecycleBin=Opt_DeleteToRecycleBin;
/*& 31.05.2001 OT ��������� ����������� ������ */
  FrameFromLaunched->UnlockRefresh();
/* OT &*/
  /* $ 01.10.2001 IS ������������, ����� �� ���� ���������� */
  if(NeedUpdate)
  {
    ShellDeleteUpdatePanels(SrcPanel,NeedSetUpADir);
  }
  /* IS $ */
}


void ShellDeleteUpdatePanels(Panel *SrcPanel,BOOL NeedSetUpADir)
{
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  int AnotherType=AnotherPanel->GetType();
  if (AnotherType!=QVIEW_PANEL)
  {
    if(NeedSetUpADir)
    {
      char CurDir[2048];
      SrcPanel->GetCurDir(CurDir);
      AnotherPanel->SetCurDir(CurDir,TRUE);
      AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    }
    else
    {
      if(AnotherPanel->NeedUpdatePanel(SrcPanel))
      {
        AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
  //      AnotherPanel->Redraw();
      }
    }
  }
  SrcPanel->Update(UPDATE_KEEP_SELECTION);
//  SrcPanel->Redraw();
  if (AnotherType==QVIEW_PANEL)
  {
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
//    AnotherPanel->Redraw();
  }
  CtrlObject->Cp()->Redraw();
}

static int CheckUpdateAnotherPanel(Panel *SrcPanel,char *SelName)
{
  char AnotherCurDir[2048];
  char FullName[2058];
  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  AnotherPanel->CloseFile();
  if(AnotherPanel->GetMode() == NORMAL_PANEL)
  {
    AnotherPanel->GetCurDir(AnotherCurDir);
    if (ConvertNameToFull(SelName,FullName, sizeof(FullName)) >= sizeof(FullName))
      return -1;
    if(strstr(AnotherCurDir,FullName))
    {
      ((FileList*)AnotherPanel)->CloseChangeNotification();
      return TRUE;
    }
  }
  return FALSE;
}

static void PR_ShellDeleteMsg(void)
{
  ShellDeleteMsg((char*)PreRedrawParam.Param1);
}

void ShellDeleteMsg(char *Name)
{
  static int Width=30;
  int WidthTemp;
  char OutFileName[NM];

  if(Name && *Name)
    WidthTemp=Max((int)strlen(Name),(int)30);
  else
    Width=WidthTemp=30;

  if(WidthTemp > WidthNameForMessage)
    WidthTemp=WidthNameForMessage; // ������ ������ - 38%
  if(WidthTemp >= sizeof(OutFileName)-4)
    WidthTemp=sizeof(OutFileName)-5;
  if(Width < WidthTemp)
    Width=WidthTemp;

  strncpy(OutFileName,Name,sizeof(OutFileName)-1);
  TruncPathStr(OutFileName,Width);
  CenterStr(OutFileName,OutFileName,Width+4);

  Message(0,0,MSG(MDeleteTitle),MSG(MDeleting),OutFileName);
  PreRedrawParam.Param1=Name;
}


int AskDeleteReadOnly(char *Name,DWORD Attr)
{
  int MsgCode;
  if ((Attr & FA_RDONLY)==0)
    return(DELETE_YES);
  if (ReadOnlyDeleteMode!=-1)
    MsgCode=ReadOnlyDeleteMode;
  else
  {
    /* $ 13.07.2001 IS ������� ���, ���� ��� ����������� � ��������� */
    char MsgName[NM];
    strcpy(MsgName, Name);
    TruncPathStr(MsgName, ScrX-16);
    MsgCode=Message(MSG_DOWN|MSG_WARNING,5,MSG(MWarning),MSG(MDeleteRO),MsgName,
            MSG(MAskDeleteRO),MSG(MDeleteFileDelete),MSG(MDeleteFileAll),
            MSG(MDeleteFileSkip),MSG(MDeleteFileSkipAll),
            MSG(MDeleteFileCancel));
    /* IS $ */
  }
  switch(MsgCode)
  {
    case 1:
      ReadOnlyDeleteMode=1;
      break;
    case 2:
      return(DELETE_SKIP);
    case 3:
      ReadOnlyDeleteMode=3;
      return(DELETE_SKIP);
    case -1:
    case -2:
    case 4:
      return(DELETE_CANCEL);
  }
  SetFileAttributes(Name,0);
  return(DELETE_YES);
}



int ShellRemoveFile(char *Name,char *ShortName,int Wipe)
{
  while (1)
  {
    if (Wipe)
    {
      if (WipeFile(Name) || WipeFile(ShortName))
        break;
    }
    else
      if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT && WinVer.dwMajorVersion<4 ||
          !Opt.DeleteToRecycleBin)
      {
/*
        if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_NT)
        {
          HANDLE hDelete=CreateFile(Name,GENERIC_WRITE,0,NULL,OPEN_EXISTING,
                 FILE_FLAG_DELETE_ON_CLOSE|FILE_FLAG_POSIX_SEMANTICS,NULL);
          if (hDelete!=INVALID_HANDLE_VALUE && CloseHandle(hDelete))
            break;
        }
*/
        if (DeleteFile(Name) || DeleteFile(ShortName))
          break;
      }
      else
        if (RemoveToRecycleBin(Name))
          break;
    int MsgCode;
    /* $ 13.07.2001 IS ������� ���, ���� ��� ����������� � ��������� */
    char MsgName[NM];
    strcpy(MsgName, Name);
    TruncPathStr(MsgName, ScrX-16);
    MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                    MSG(MCannotDeleteFile),MsgName,MSG(MDeleteRetry),
                    MSG(MDeleteSkip),MSG(MDeleteCancel));
    /* IS */
    switch(MsgCode)
    {
      case -1:
      case -2:
      case 2:
        return(DELETE_CANCEL);
      case 1:
        return(DELETE_SKIP);
    }
  }
  return(DELETE_SUCCESS);
}


int ERemoveDirectory(char *Name,char *ShortName,int Wipe)
{
  int RetCode;
  while (1)
  {
    if (Wipe)
    {
      if ((RetCode=WipeDirectory(Name))!=0 || (RetCode=WipeDirectory(ShortName))!=0)
        break;
    }
    else
      if ((RetCode=RemoveDirectory(Name))!=0 || (RetCode=RemoveDirectory(ShortName))!=0)
        break;
    /* $ 13.07.2001 IS ������� ���, ���� ��� ����������� � ��������� */
    char MsgName[NM];
    strcpy(MsgName, Name);
    TruncPathStr(MsgName, ScrX-16);
    if (Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                MSG(MCannotDeleteFolder),MsgName,MSG(MDeleteRetry),
                MSG(MDeleteCancel))!=0)
    /* IS $ */
      break;
  }
  return(RetCode);
}

/* 14.03.2001 SVS
   �������� ������ ���� �������� ������� SHFileOperation(),
   ���� ���� ��������� � �������.
*/
int RemoveToRecycleBin(char *Name)
{
  static struct {
    DWORD SHError;
    DWORD LCError;
  }
  SHErrorCode2LastErrorCode[]=
  {
    {SE_ERR_FNF,ERROR_FILE_NOT_FOUND},
    {SE_ERR_PNF,ERROR_PATH_NOT_FOUND},
    {SE_ERR_ACCESSDENIED,ERROR_ACCESS_DENIED},
    {SE_ERR_OOM,ERROR_OUTOFMEMORY},
    {SE_ERR_DLLNOTFOUND,ERROR_FILE_NOT_FOUND},
    {SE_ERR_SHARE,ERROR_SHARING_VIOLATION},
    {SE_ERR_NOASSOC,ERROR_BAD_COMMAND},
  };

  SHFILEOPSTRUCT fop;
  char FullName[NM+1];
//  ConvertNameToFull(Name,FullName, sizeof(FullName));
  if (ConvertNameToFull(Name,FullName, sizeof(FullName)) >= sizeof(FullName)){
    return 1;
  }

  OemToChar(FullName,FullName);
  FullName[strlen(FullName)+1]=0;

  memset(&fop,0,sizeof(fop)); // ������� �������� :-)
  fop.wFunc=FO_DELETE;
  fop.pFrom=FullName;
  fop.fFlags=FOF_NOCONFIRMATION|FOF_SILENT;
  if (Opt.DeleteToRecycleBin)
    fop.fFlags|=FOF_ALLOWUNDO;
  SetFileApisToANSI();
  DWORD RetCode=SHFileOperation(&fop);
  SetFileApisToOEM();
  if(RetCode)
  {
    for(int I=0; I < sizeof(SHErrorCode2LastErrorCode)/sizeof(SHErrorCode2LastErrorCode[0]); ++I)
      if(SHErrorCode2LastErrorCode[I].SHError == RetCode)
      {
        SetLastError(SHErrorCode2LastErrorCode[I].LCError);
        return FALSE;
      }
  }
  RetCode=!fop.fAnyOperationsAborted;
  return(RetCode);
}
/* SVS $ */

int WipeFile(char *Name)
{
  DWORD FileSize;
  HANDLE WipeHandle;
  SetFileAttributes(Name,0);
  WipeHandle=CreateFile(Name,GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_FLAG_WRITE_THROUGH|FILE_FLAG_SEQUENTIAL_SCAN,NULL);
  if (WipeHandle==INVALID_HANDLE_VALUE)
    return(FALSE);
  if ((FileSize=GetFileSize(WipeHandle,NULL))==0xFFFFFFFF)
  {
    CloseHandle(WipeHandle);
    return(FALSE);
  }
  const int BufSize=65536;
  char *Buf=new char[BufSize];
  memset(Buf,(BYTE)Opt.WipeSymbol,BufSize); // ���������� ������ �����������
  DWORD Written;
  while (FileSize>0)
  {
    DWORD WriteSize=Min((DWORD)BufSize,FileSize);
    WriteFile(WipeHandle,Buf,WriteSize,&Written,NULL);
    FileSize-=WriteSize;
  }
  WriteFile(WipeHandle,Buf,BufSize,&Written,NULL);
  /* $ 13.07.2000 SVS
       ��� �� ������� new[], �� � ������� � delete[] ����... */
  delete[] Buf;
  /* SVS $ */
  SetFilePointer(WipeHandle,0,NULL,FILE_BEGIN);
  SetEndOfFile(WipeHandle);
  CloseHandle(WipeHandle);
  char TempName[NM];
  MoveFile(Name,FarMkTempEx(TempName,NULL,FALSE));
  return(DeleteFile(TempName));
}


int WipeDirectory(char *Name)
{
  char TempName[NM];
  MoveFile(Name,FarMkTempEx(TempName,NULL,FALSE));
  return(RemoveDirectory(TempName));
}

int DeleteFileWithFolder(const char *FileName)
{
  char FolderName[NM],*Slash;
  SetFileAttributes(FileName,0);
  remove(FileName);
  strcpy(FolderName,FileName);
  if ((Slash=strrchr(FolderName,'\\'))!=NULL)
    *Slash=0;
  return(RemoveDirectory(FolderName));
}


void DeleteDirTree(char *Dir)
{
  if (*Dir==0 || (Dir[0]=='\\' || Dir[0]=='/') && Dir[1]==0 ||
      Dir[1]==':' && (Dir[2]=='\\' || Dir[2]=='/') && Dir[3]==0)
    return;
  char FullName[NM];
  WIN32_FIND_DATA FindData;
  ScanTree ScTree(TRUE);

  ScTree.SetFindPath(Dir,"*.*");
  while (ScTree.GetNextName(&FindData,FullName))
  {
    SetFileAttributes(FullName,0);
    if (FindData.dwFileAttributes & FA_DIREC)
    {
      if (ScTree.IsDirSearchDone())
        RemoveDirectory(FullName);
    }
    else
      DeleteFile(FullName);
  }
  SetFileAttributes(Dir,0);
  RemoveDirectory(Dir);
}
