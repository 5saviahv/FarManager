/*
delete.cpp

�������� 䠩���

*/

/* Revision: 1.12 13.03.2001 $ */

/*
Modify:
  13.03.2001 SVS
    - 㤠����� ᨬ����� � ��২�� �ॢ�� ���३ �ਣ�����!!!!!!
  12.03.2001 SVS
    + Opt.DeleteSymbolWipe -> Opt.WipeSymbol
  12.03.2001 SVS
    + Opt.DeleteSymbolWipe ᨬ��� �������⥫� ��� "ZAP-����樨"
  07.03.2001 SVS
    - ������� ���� � ��� :-)))
  05.01.2001 SVS
    ! � ����ᨬ��� �� �᫠ �⠢�� �㦭�� ����砭�� ��� 㤠�����
  05.01.2001 IS
    ! ��ᬥ⨪� � ᮮ�饭��� - ࠧ�� ᮮ�饭�� � ����ᨬ��� �� ⮣�,
      ����� � ᪮�쪮 ����⮢ �뤥����.
  28.11.2000 SVS
    + ���ᯥ稬 ���४��� ࠡ��� � SymLink (�.�. "Directory Junctions")
  11.11.2000 SVS
    ! ��ᬥ⨪�: "FarTmpXXXXXX" �������� �� ��६����� FarTmpXXXXXX
    - ��ࠢ��� ������让 ��� � �㭪��� Wipe*
  03.11.2000 OT
    ! �������� �஢�ન �����頥���� ���祭��
  02.11.2000 OT
    ! �������� �஢�ન �� ����� ����, �⢥������� ��� ��� 䠩��.
  13.07.2000 SVS
    ! ������� ���४樨 �� �ᯮ�짮����� new/delete/realloc
  11.07.2000 SVS
    ! ��������� ��� ���������� �������樨 ��� BC & VC
  25.06.2000 SVS
    ! �����⮢�� Master Copy
    ! �뤥����� � ����⢥ ᠬ����⥫쭮�� �����
*/

#include "headers.hpp"
#pragma hdrstop

/* $ 30.06.2000 IS
   �⠭����� ���������
*/
#include "internalheaders.hpp"
/* IS $ */


static void ShellDeleteMsg(char *Name,int Flags);
static int AskDeleteReadOnly(char *Name,DWORD Attr);
static int ShellRemoveFile(char *Name,char *ShortName,int Wipe);
static int ERemoveDirectory(char *Name,char *ShortName,int Wipe);
static int RemoveToRecycleBin(char *Name);
static int WipeFile(char *Name);
static int WipeDirectory(char *Name);

static int ReadOnlyDeleteMode,DeleteAllFolders;

void ShellDelete(Panel *SrcPanel,int Wipe)
{
  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);
  WIN32_FIND_DATA FindData;
  char DeleteFilesMsg[300],SelName[NM],SelShortName[NM],DizName[NM];
  int SelCount,FileAttr,UpdateDiz;

  DeleteAllFolders=!Opt.Confirm.DeleteFolder;

  UpdateDiz=(Opt.Diz.UpdateMode==DIZ_UPDATE_ALWAYS ||
             SrcPanel->IsDizDisplayed() &&
             Opt.Diz.UpdateMode==DIZ_UPDATE_IF_DISPLAYED);

  if ((SelCount=SrcPanel->GetSelCount())==0)
    return;

  if (SelCount==1)
  {
    SrcPanel->GetSelName(NULL,FileAttr);
    SrcPanel->GetSelName(SelName,FileAttr);
    if (strcmp(SelName,"..")==0 || *SelName==0)
      return;
    strcpy(DeleteFilesMsg,SelName);
  }
  else
  /* $ 05.01.2001 SVS
     � ����ᨬ��� �� �᫠ �⠢�� �㦭�� ����砭��*/
  {
  /* $ 05.01.2001 IS
  ����� "䠩���" ��襬 ����ࠫ쭮� - "����⮢"
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

  if (Opt.Confirm.Delete || SelCount>1 || (FileAttr & FA_DIREC))
  {
    char *DelMsg;
    /* $ 05.01.2001 IS
       ! ��ᬥ⨪� � ᮮ�饭��� - ࠧ�� ᮮ�饭�� � ����ᨬ��� �� ⮣�,
         ����� � ᪮�쪮 ����⮢ �뤥����.
    */
    BOOL folder=(FileAttr & FA_DIREC);
    if (SelCount==1)
    {

      if (Wipe)
        DelMsg=MSG(folder?MAskWipeFolder:MAskWipeFile);
      else
        if (Opt.DeleteToRecycleBin && !(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT))
          DelMsg=MSG(folder?MAskDeleteRecycleFolder:MAskDeleteRecycleFile);
        else
          DelMsg=MSG(folder?MAskDeleteFolder:MAskDeleteFile);
    }
    else
    {
      if (Wipe)
        DelMsg=MSG(MAskWipe);
      else
        if (Opt.DeleteToRecycleBin && !(FileAttr & FILE_ATTRIBUTE_REPARSE_POINT))
          DelMsg=MSG(MAskDeleteRecycle);
        else
          DelMsg=MSG(MAskDelete);
    }
    /* IS $ */
    if (Message(0,2,MSG(MDeleteTitle),DelMsg,DeleteFilesMsg,MSG(MDelete),MSG(MCancel))!=0)
      return;
  }

  if (Opt.Confirm.Delete && SelCount>1)
  {
    SaveScreen SaveScr;
    SetCursorType(FALSE,0);
    ShellDeleteMsg("",0);
    if (Message(MSG_DOWN|MSG_WARNING,2,MSG(MDeleteFilesTitle),MSG(MAskDelete),
                DeleteFilesMsg,MSG(MDeleteFileAll),MSG(MDeleteFileCancel))!=0)
      return;
  }

  if (UpdateDiz)
    SrcPanel->ReadDiz();

  SrcPanel->GetDizName(DizName);
  int DizPresent=(*DizName && GetFileAttributes(DizName)!=0xFFFFFFFF);

  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));
  SetFarTitle(MSG(MDeletingTitle));

  CtrlObject->GetAnotherPanel(SrcPanel)->CloseFile();

  if (SrcPanel->GetType()==TREE_PANEL)
    chdir("\\");

  {
    int Cancel=0;
    SaveScreen SaveScr;
    SetCursorType(FALSE,0);
    ShellDeleteMsg("",0);

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
          char FullName[NM];
//          ConvertNameToFull(SelName,FullName, sizeof(FullName));
          if (ConvertNameToFull(SelName,FullName, sizeof(FullName)) >= sizeof(FullName)){
            return;
          }
          if (IsFolderNotEmpty(FullName))
          {
            int MsgCode=Message(MSG_DOWN|MSG_WARNING,4,MSG(MDeleteFolderTitle),
                  MSG(MDeleteFolderConfirm),FullName,MSG(MDeleteFileDelete),
                  MSG(MDeleteFileAll),MSG(MDeleteFileSkip),MSG(MDeleteFileCancel));
            if (MsgCode<0 || MsgCode==3)
              break;
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
            ShellDeleteMsg(FullName,MSG_KEEPBACKGROUND);
            char ShortName[NM];
            strcpy(ShortName,FullName);
            if (*FindData.cAlternateFileName)
              strcpy(PointToName(ShortName),FindData.cAlternateFileName);
            if (FindData.dwFileAttributes & FA_DIREC)
            {
              /* $ 28.11.2000 SVS
                 ���ᯥ稬 ���४��� ࠡ��� � SymLink
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
                int MsgCode=Message(MSG_DOWN|MSG_WARNING,4,MSG(MDeleteFolderTitle),
                      MSG(MDeleteFolderConfirm),FullName,
                      MSG(MDeleteFileDelete),MSG(MDeleteFileAll),
                      MSG(MDeleteFileSkip),MSG(MDeleteFileCancel));
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
          ShellDeleteMsg(SelName,MSG_KEEPBACKGROUND);
          if (FileAttr & FA_RDONLY)
            SetFileAttributes(SelName,0);
          int DeleteCode;
          // ��䨣� ����� �뤥�뢠����, � ���� �����, �� 㤠�����
          // ᨬ����� � ��২�� �ॢ�� ���३ �ਣ�����.
          if (SymLink || !Opt.DeleteToRecycleBin || Wipe)
            DeleteCode=ERemoveDirectory(SelName,SelShortName,Wipe);
          else
          {
            DeleteCode=RemoveToRecycleBin(SelName);
            if (!DeleteCode && WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
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
        ShellDeleteMsg(SelName,MSG_KEEPBACKGROUND);
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

  Panel *AnotherPanel=CtrlObject->GetAnotherPanel(SrcPanel);
  int AnotherType=AnotherPanel->GetType();
  if (AnotherType!=QVIEW_PANEL)
  {
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    AnotherPanel->Redraw();
  }
  SrcPanel->Update(UPDATE_KEEP_SELECTION);
  SrcPanel->Redraw();
  if (AnotherType==QVIEW_PANEL)
  {
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    AnotherPanel->Redraw();
  }
}


void ShellDeleteMsg(char *Name,int Flags)
{
  char DelName[NM];
  CenterStr(Name,DelName,30);
  TruncStr(DelName,30);
  Message(Flags,0,MSG(MDeleteTitle),MSG(MDeleting),DelName);
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
    MsgCode=Message(MSG_DOWN|MSG_WARNING,5,MSG(MWarning),MSG(MDeleteRO),Name,
            MSG(MAskDeleteRO),MSG(MDeleteFileDelete),MSG(MDeleteFileAll),
            MSG(MDeleteFileSkip),MSG(MDeleteFileSkipAll),
            MSG(MDeleteFileCancel));
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
    MsgCode=Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,3,MSG(MError),
                    MSG(MCannotDeleteFile),Name,MSG(MDeleteRetry),
                    MSG(MDeleteSkip),MSG(MDeleteCancel));

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
    if (Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),
                MSG(MCannotDeleteFolder),Name,MSG(MDeleteRetry),
                MSG(MDeleteCancel))!=0)
      break;
  }
  return(RetCode);
}


int RemoveToRecycleBin(char *Name)
{
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
  int RetCode=SHFileOperation(&fop)==0 && !fop.fAnyOperationsAborted;
  SetFileApisToOEM();
  return(RetCode);
}


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
  memset(Buf,(BYTE)Opt.WipeSymbol,BufSize); // �ᯮ��㥬 ᨬ��� �������⥫�
  DWORD Written;
  while (FileSize>0)
  {
    DWORD WriteSize=Min(BufSize,FileSize);
    WriteFile(WipeHandle,Buf,WriteSize,&Written,NULL);
    FileSize-=WriteSize;
  }
  WriteFile(WipeHandle,Buf,BufSize,&Written,NULL);
  /* $ 13.07.2000 SVS
       ࠧ � �맢��� new[], � � �ਤ��� � delete[] ����... */
  delete[] Buf;
  /* SVS $ */
  SetFilePointer(WipeHandle,0,NULL,FILE_BEGIN);
  SetEndOfFile(WipeHandle);
  CloseHandle(WipeHandle);
  char TempName[NM];
  strcpy(TempName,FarTmpXXXXXX);
  mktemp(TempName);
  MoveFile(Name,TempName);
  return(DeleteFile(TempName));
}


int WipeDirectory(char *Name)
{
  char TempName[NM];
  strcpy(TempName,FarTmpXXXXXX);
  mktemp(TempName);
  MoveFile(Name,TempName);
  return(RemoveDirectory(TempName));
}
