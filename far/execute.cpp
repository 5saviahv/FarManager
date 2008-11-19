/*
execute.cpp

"�����������" ��������.

*/

#include "headers.hpp"
#pragma hdrstop

#include "farqueue.hpp"
#include "filepanels.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "plugin.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"
#include "savescr.hpp"
#include "chgprior.hpp"
#include "global.hpp"
#include "cmdline.hpp"
#include "panel.hpp"
#include "fn.hpp"
#include "rdrwdsk.hpp"
#include "udlist.hpp"

static const char strSystemExecutor[]="System\\Executor";

// ��������� ����� �� �������� GetFileInfo, �������� ����������� ���������� �
// ���� PE-������
/* 14.06.2002 VVM
  + ���������� ��������� IMAGE_SUBSYSTEM_*
    ���� ������� �������� */

// ��� ������ �� ��������� IMAGE_SUBSYTEM_UNKNOWN ��������
// "���� �� �������� �����������".
// ��� DOS-���������� ��������� ��� ���� �������� �����.
#define IMAGE_SUBSYSTEM_DOS_EXECUTABLE  255

static int IsCommandPEExeGUI(const char *FileName,DWORD& ImageSubsystem)
{
  //_SVS(CleverSysLog clvrSLog("IsCommandPEExeGUI()"));
  //_SVS(SysLog("Param: FileName='%s'",FileName));
//  char NameFile[NM];
  HANDLE hFile;
  int Ret=FALSE;
  ImageSubsystem = IMAGE_SUBSYSTEM_UNKNOWN;

  if((hFile=FAR_CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL)) != INVALID_HANDLE_VALUE)
  {
    unsigned __int64 FileSize;
    DWORD ReadSize;
    IMAGE_DOS_HEADER dos_head;

    FAR_GetFileSize(hFile,&FileSize);

    BOOL RetReadFile=ReadFile(hFile,&dos_head,sizeof(IMAGE_DOS_HEADER),&ReadSize,NULL);

    if(RetReadFile && dos_head.e_magic == IMAGE_DOS_SIGNATURE)
    {
      Ret=TRUE;
      ImageSubsystem = IMAGE_SUBSYSTEM_DOS_EXECUTABLE;
      /*  ���� �������� ����� �� �������� 18h (OldEXE - MZ) >= 40h,
      �� �������� ����� � 3Ch �������� ��������� ��������� Windows. */
      /* 31.07.2003 VVM
        ! ������� ���� MSDN - ����� ������� �� ����� */
//      if (dos_head.e_lfarlc >= 0x40)
      /* VVM $ */
      {
        DWORD signature;
        #include <pshpack1.h>
        struct __HDR
        {
           DWORD signature;
           IMAGE_FILE_HEADER _head;
           union
           {
             IMAGE_OPTIONAL_HEADER32 opt_head32;
             IMAGE_OPTIONAL_HEADER64 opt_head64;
           };
           // IMAGE_SECTION_HEADER section_header[];  /* actual number in NumberOfSections */
        } header, *pheader;
        #include <poppack.h>

        if(SetFilePointer(hFile,dos_head.e_lfanew,NULL,FILE_BEGIN) != -1)
        {
          // ������ ��������� ���������
          if(ReadFile(hFile,&header,sizeof(struct __HDR),&ReadSize,NULL))
          {
            signature=header.signature;
            pheader=&header;

            if(signature == IMAGE_NT_SIGNATURE) // PE
            {
               if (header.opt_head32.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                 ImageSubsystem = header.opt_head64.Subsystem;
               else
                 ImageSubsystem = header.opt_head32.Subsystem;
            }
//            {
//              IsPEGUI=1;
//              IsPEGUI|=(header.opt_head.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI)?2:0;
//            }
            else if((WORD)signature == IMAGE_OS2_SIGNATURE) // NE
            {
              /*
                 NE,  ���...  � ��� ���������� ��� ��� ������?

                 Andrzej Novosiolov <andrzej@se.kiev.ua>
                 AN> ��������������� �� ����� "Target operating system" NE-���������
                 AN> (1 ���� �� �������� 0x36). ���� ��� Windows (�������� 2, 4) - �������������
                 AN> GUI, ���� OS/2 � ������ �������� (��������� ��������) - ������������� �������.
              */
              BYTE ne_exetyp=((IMAGE_OS2_HEADER *)pheader)->ne_exetyp;
              if(ne_exetyp == 2 || ne_exetyp == 4)
                ImageSubsystem=IMAGE_SUBSYSTEM_WINDOWS_GUI;
            }
          }
          else
          {
            ; // ������ ����� � ������� ���������� ��������� ;-(
          }
        }
        else
        {
          ; // ������ �������� ���� ���� � �����, �.�. dos_head.e_lfanew
            // ������ ������� � �������������� ����� (�������� ��� ������
            // ���� DOS-����
        }
      }
/*      else
      {
        ; // ��� ������� EXE, �� �� �������� EXE
      }
*/
    }
    else
    {
      if(!RetReadFile)
        ; // ������ ������
      else
        ; // ��� �� ����������� ���� - � ���� ���� ��������� MZ, ��������, NLM-������
        // TODO: ����� ����� ��������� POSIX �������, �������� "/usr/bin/sh"
    }
    CloseHandle(hFile);
  }

  return Ret;
}
/* VVM $ */

// �� ����� ����� (�� ��� ����������) �������� ������� ���������
// ������������� ��������� �������� �������-����������
// (����� �� ����� ����������)
char* GetShellAction(const char *FileName,DWORD& ImageSubsystem,DWORD& Error)
{
  //_SVS(CleverSysLog clvrSLog("GetShellAction()"));
  //_SVS(SysLog("Param: FileName='%s'",FileName));

  char Value[1024];
  char NewValue[2048];
  const char *ExtPtr;
  char *RetPtr;
  LONG ValueSize;
  const char command_action[]="\\command";

  Error=ERROR_SUCCESS;
  ImageSubsystem = IMAGE_SUBSYSTEM_UNKNOWN;

  if ((ExtPtr=strrchr(FileName,'.'))==NULL)
    return(NULL);

  ValueSize=sizeof(Value);
  *Value=0;

  if (RegQueryValue(HKEY_CLASSES_ROOT,(LPCTSTR)ExtPtr,(LPTSTR)Value,&ValueSize)!=ERROR_SUCCESS)
    return(NULL);

  strcat(Value,"\\shell");
//_SVS(SysLog("[%d] Value='%s'",__LINE__,Value));

  HKEY hKey;
  if (RegOpenKey(HKEY_CLASSES_ROOT,Value,&hKey)!=ERROR_SUCCESS)
    return(NULL);

  static char Action[512];

  *Action=0;
  ValueSize=sizeof(Action);
  LONG RetQuery = RegQueryValueEx(hKey,"",NULL,NULL,(unsigned char *)Action,(LPDWORD)&ValueSize);
  strcat(Value,"\\");
//_SVS(SysLog("[%d] Action='%s' Value='%s'",__LINE__,Action,Value));

  if (RetQuery == ERROR_SUCCESS)
  {
    UserDefinedList ActionList(0,0,ULF_UNIQUE);

    RetPtr=(*Action==0 ? NULL:Action);
    /* $ 03.10.2002 VVM
      + ������ � ����� ������ ����� ���� ���������. */
    const char *ActionPtr;

    LONG RetEnum = ERROR_SUCCESS;
    if (RetPtr != NULL && ActionList.Set(Action))
    {
      HKEY hOpenKey;

      ActionList.Reset();
      while (RetEnum == ERROR_SUCCESS && (ActionPtr = ActionList.GetNext()) != NULL)
      {
        xstrncpy(NewValue, Value, sizeof(NewValue) - 1);
        xstrncat(NewValue, ActionPtr, sizeof(NewValue) - 1);
        xstrncat(NewValue, command_action, sizeof(NewValue) - 1);
        if (RegOpenKey(HKEY_CLASSES_ROOT,NewValue,&hOpenKey)==ERROR_SUCCESS)
        {
          RegCloseKey(hOpenKey);
          xstrncat(Value, ActionPtr, sizeof(Value) - 1);
          RetPtr = xstrncpy(Action,ActionPtr,sizeof(Action)-1);
          RetEnum = ERROR_NO_MORE_ITEMS;
        } /* if */
      } /* while */
    } /* if */
    else
      xstrncat(Value,Action, sizeof(Value) - 1);
    /* VVM $ */

//_SVS(SysLog("[%d] Value='%s'",__LINE__,Value));
    if(RetEnum != ERROR_NO_MORE_ITEMS) // ���� ������ �� �����, ��...
      RetPtr=NULL;
  }
  else
  {
    // This member defaults to "Open" if no verb is specified.
    // �.�. ���� �� ������� NULL, �� ��������������� ������� "Open"
      RetPtr=NULL;
//    strcat(Value,"\\open");
  }

  // ���� RetPtr==NULL - �� �� ����� default action.
  // ��������� - ���� �� ������ ���-������ � ����� ����������
  if (RetPtr==NULL)
  {
    LONG RetEnum = ERROR_SUCCESS;
    DWORD dwIndex = 0;
    DWORD dwKeySize = 0;
    FILETIME ftLastWriteTime;
    HKEY hOpenKey;

    // ������� �������� "open"...
    strcpy(Action,"open");
    xstrncpy(NewValue, Value, sizeof(NewValue) - 1);
    xstrncat(NewValue, Action, sizeof(NewValue) - 1);
    xstrncat(NewValue, command_action, sizeof(NewValue) - 1);
    if (RegOpenKey(HKEY_CLASSES_ROOT,NewValue,&hOpenKey)==ERROR_SUCCESS)
    {
      RegCloseKey(hOpenKey);
      xstrncat(Value, Action, sizeof(Value) - 1);
      RetPtr = Action;
      RetEnum = ERROR_NO_MORE_ITEMS;
//_SVS(SysLog("[%d] Action='%s' Value='%s'",__LINE__,Action,Value));
    } /* if */

    // ... � ������ ��� ���������, ���� "open" ����
    while (RetEnum == ERROR_SUCCESS)
    {
      dwKeySize = sizeof(Action);
      RetEnum = RegEnumKeyEx(hKey, dwIndex++, Action, &dwKeySize, NULL, NULL, NULL, &ftLastWriteTime);
      if (RetEnum == ERROR_SUCCESS)
      {
        // �������� ������� "�������" � ����� �����
        xstrncpy(NewValue, Value, sizeof(NewValue) - 1);
        xstrncat(NewValue, Action, sizeof(NewValue) - 1);
        xstrncat(NewValue, command_action, sizeof(NewValue) - 1);
        if (RegOpenKey(HKEY_CLASSES_ROOT,NewValue,&hOpenKey)==ERROR_SUCCESS)
        {
          RegCloseKey(hOpenKey);
          xstrncat(Value, Action, sizeof(Value) - 1);
          RetPtr = Action;
          RetEnum = ERROR_NO_MORE_ITEMS;
        } /* if */
      } /* if */
    } /* while */
//_SVS(SysLog("[%d] Action='%s' Value='%s'",__LINE__,Action,Value));
  } /* if */
  RegCloseKey(hKey);

  if (RetPtr != NULL)
  {
    xstrncat(Value,command_action, sizeof(Value) - 1);

    // � ������ �������� �������� ����������� �����
    if (RegOpenKey(HKEY_CLASSES_ROOT,Value,&hKey)==ERROR_SUCCESS)
    {
      ValueSize=sizeof(NewValue);
      RetQuery=RegQueryValueEx(hKey,"",NULL,NULL,(unsigned char *)NewValue,(LPDWORD)&ValueSize);
      RegCloseKey(hKey);
      if(RetQuery == ERROR_SUCCESS && *NewValue)
      {
        char *Ptr;
        ExpandEnvironmentStr(NewValue,NewValue,sizeof(NewValue));
        // �������� ��� ������
        if (*NewValue=='\"')
        {
          FAR_OemToChar(NewValue+1,NewValue);
          if ((Ptr=strchr(NewValue,'\"'))!=NULL)
            *Ptr=0;
        }
        else
        {
          FAR_OemToChar(NewValue,NewValue);
          if ((Ptr=strpbrk(NewValue," \t/"))!=NULL)
            *Ptr=0;
        }
        IsCommandPEExeGUI(NewValue,ImageSubsystem);
      }
      else
      {
        Error=ERROR_NO_ASSOCIATION;
        RetPtr=NULL;
      }
    }
  }

//_SVS(SysLog("[%d] Action='%s' Value='%s'",__LINE__,Action,Value));
  return RetPtr;
}

/*
 ������ PrepareExecuteModule �������� ����� ����������� ������ (� �.�. � ��
 %PATHEXT%). � ������ ������ �������� � Command ������, ������������� ��
 ����������� ������ �� ��������� ��������, �������� ��������� � Dest �
 �������� ��������� ��������� PE �� �������� (����� ��������� �������
 � ��������� ���� � �� ����� ����������).
 � ������ ������� Dest �� �����������!
 Return: TRUE/FALSE - �����/�� �����
*/
/* $ 14.06.2002 VVM
 ������� � ������� ���������� ��� �������������. ������ �� ������.
 � ��������� ������ �� ����, �.�. ��� ��������� �� ������� ������
*/
int WINAPI PrepareExecuteModule(const char *Command,char *Dest,int DestSize,DWORD& ImageSubsystem)
{
  //_SVS(CleverSysLog clvrSLog("PrepareExecuteModule()"));
  //_SVS(SysLog("Param: Command='%s'",Command));
  int Ret, I;
  char FileName[4096],FullName[4096], *Ptr;
  // int IsQuoted=FALSE;
  // int IsExistExt=FALSE;

  // ����� ������� �����! ������� �������,  � ����� ��������� �����.
  static char StdExecuteExt[NM]=".BAT;.CMD;.EXE;.COM;";
  static const char RegPath[]="SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\";
  static int PreparePrepareExt=FALSE;

  if(!PreparePrepareExt) // ���������������������� �����
  {
    // ���� ���������� %PATHEXT% ��������...
    if((I=GetEnvironmentVariable("PATHEXT",FullName,sizeof(FullName)-1)) != 0)
    {
      FullName[I]=0;
      // ������� ������� �� PATHEXT
      static char const * const StdExecuteExt0[4]={".BAT;",".CMD;",".EXE;",".COM;"};
      for(I=0; I < sizeof(StdExecuteExt0)/sizeof(StdExecuteExt0[0]); ++I)
        ReplaceStrings(FullName,StdExecuteExt0[I],"",-1);
    }

    Ptr=strcat(StdExecuteExt,strcat(FullName,";")); //!!!
    StdExecuteExt[strlen(StdExecuteExt)]=0;
    while(*Ptr)
    {
      if(*Ptr == ';')
        *Ptr=0;
      ++Ptr;
    }
    PreparePrepareExt=TRUE;
  }

  /* ����� "����������" �� �������, ������� ������ ����������� ��������,
     ��������, ��������� ���������� ������� ���.����������.
  */
  static char ExcludeCmds[4096]={0};
  static int PrepareExcludeCmds=FALSE;
  if(GetRegKey(strSystemExecutor,"Type",0))
  {
    if (!PrepareExcludeCmds)
    {
      GetRegKey(strSystemExecutor,"ExcludeCmds",(char*)ExcludeCmds,"",sizeof(ExcludeCmds));
      Ptr=strcat(ExcludeCmds,";"); //!!!
      ExcludeCmds[strlen(ExcludeCmds)]=0;
      while(*Ptr)
      {
        if(*Ptr == ';')
          *Ptr=0;
        ++Ptr;
      }
      PrepareExcludeCmds=TRUE;
    }
  }
  else
  {
    *ExcludeCmds=0;
    PrepareExcludeCmds=FALSE;
  }

  ImageSubsystem = IMAGE_SUBSYSTEM_UNKNOWN; // GUIType ������ ������� ���������������� � FALSE
  Ret=FALSE;

  /* $ 14.06.2002 VVM
     ��� ������ ������ ���������� ��� �������. ����� ������ ��� ������ ����/����
  // �������� ��� ������
  if (*Command=='\"')
  {
    FAR_OemToChar(Command+1,FullName);
    if ((Ptr=strchr(FullName,'\"'))!=NULL)
      *Ptr=0;
    IsQuoted=TRUE;
  }
  else
  {
    FAR_OemToChar(Command,FullName);
    if ((Ptr=strpbrk(FullName," \t/|><"))!=NULL)
      *Ptr=0;
  } VVM $ */

  if(!*Command) // ��� ��, ���� ��... �������� �������� :-(
    return 0;

  FAR_OemToChar(Command,FileName);

  // ������� ������ - ������� ����������
  {
    char *Ptr=ExcludeCmds;
    while(*Ptr)
    {
      if(!LocalStricmp(FileName,Ptr))
      {
        ImageSubsystem = IMAGE_SUBSYSTEM_WINDOWS_CUI;
        return TRUE;
      }
      Ptr+=strlen(Ptr)+1;
    }
  }

  // IsExistExt - ���� ����� ���� (����������), �� ����� �������������� ��
  // �����.
  // IsExistExt=strrchr(FullName,'.')!=NULL;

  SetFileApisTo(APIS2ANSI);

  {
    char *FilePart;
    char *PtrFName=strrchr(PointToName(strcpy(FullName,FileName)),'.');
    char *WorkPtrFName=0;
    if(!PtrFName)
      WorkPtrFName=FullName+strlen(FullName);

    char *PtrExt=StdExecuteExt;
    while(*PtrExt) // ������ ������ - � ������� ��������
    {
      if(!PtrFName)
        strcpy(WorkPtrFName,PtrExt);
      DWORD dwFileAttr = GetFileAttributes(FullName);
      if ((dwFileAttr != INVALID_FILE_ATTRIBUTES) && !(dwFileAttr & FILE_ATTRIBUTE_DIRECTORY))
      {
        // GetFullPathName - ��� �����, �.�. ���� ������ � date.exe
        // � ������� ��������, �� ������ ������ ������� �� ��������
        // cmd.exe �� ����� �� �������� ������� ���������� date
        GetFullPathName(FullName,sizeof(FullName),FullName,&FilePart);

        Ret=TRUE;
        break;
      }
      PtrExt+=strlen(PtrExt)+1;
    }

    if(!Ret) // ������ ������ - �� �������� SearchPath
    {
      /* $ 26.09.2003 VVM
        ! ������� ������ �� ���������� PATH, � �� ����� ����� */
      char PathEnv[4096];
      if (GetEnvironmentVariable("PATH",PathEnv,sizeof(PathEnv)-1) != 0)
      {
        PtrExt=StdExecuteExt;
        while(*PtrExt)
        {
          if(!PtrFName)
            strcpy(WorkPtrFName,PtrExt);
          if(SearchPath(PathEnv,FullName,PtrExt,sizeof(FullName),FullName,&FilePart))
          {
            Ret=TRUE;
            break;
          }
          PtrExt+=strlen(PtrExt)+1;
        }
      }

      if (!Ret)
      {
        PtrExt=StdExecuteExt;
        while(*PtrExt)
        {
          if(!PtrFName)
            strcpy(WorkPtrFName,PtrExt);
          if(SearchPath(NULL,FullName,PtrExt,sizeof(FullName),FullName,&FilePart))
          {
            Ret=TRUE;
            break;
          }
          PtrExt+=strlen(PtrExt)+1;
        }
      }
      /* VVM $ */

      if (!Ret && Opt.ExecuteUseAppPath) // ������ ������ - ����� � ������ � "App Paths"
      {
        // � ������ Command ������� ����������� ������ �� ������ ����, �������
        // ������� �� SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths
        // ������� ������� � HKCU, ����� - � HKLM
        HKEY hKey;
        HKEY RootFindKey[2]={HKEY_CURRENT_USER,HKEY_LOCAL_MACHINE};

        for(I=0; I < sizeof(RootFindKey)/sizeof(RootFindKey[0]); ++I)
        {
          sprintf(FullName,"%s%s",RegPath,FileName);
          if (RegOpenKeyEx(RootFindKey[I], FullName, 0,KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
          {
            DWORD Type, DataSize=sizeof(FullName);
            RegQueryValueEx(hKey,"", 0, &Type, (LPBYTE)FullName,&DataSize);
            RegCloseKey(hKey);
            /* $ 03.10.2001 VVM ���������� ���������� ��������� */
            strcpy(FileName, FullName);
            ExpandEnvironmentStrings(FileName,FullName,sizeof(FullName));
            Unquote(FullName);
            Ret=TRUE;
            break;
          }
        }

        if (!Ret && Opt.ExecuteUseAppPath)
        /* $ 14.06.2002 VVM
           �� ����� - ��������� � ����������� */
        {
          PtrExt=StdExecuteExt;
          while(*PtrExt && !Ret)
          {
            for(I=0; I < sizeof(RootFindKey)/sizeof(RootFindKey[0]); ++I)
            {
              sprintf(FullName,"%s%s%s",RegPath,FileName,PtrExt);
              if (RegOpenKeyEx(RootFindKey[I], FullName, 0,KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
              {
                DWORD Type, DataSize=sizeof(FullName);
                RegQueryValueEx(hKey,"", 0, &Type, (LPBYTE)FullName,&DataSize);
                RegCloseKey(hKey);
                /* $ 03.10.2001 VVM ���������� ���������� ��������� */
                strcpy(FileName, FullName);
                ExpandEnvironmentStrings(FileName,FullName,sizeof(FullName));
                Unquote(FullName);
                Ret=TRUE;
                break;
              }
            } /* for */
            PtrExt+=strlen(PtrExt)+1;
          }
        } /* if */
      } /* if */
    }
  }

  if(Ret) // ��������� "�������" ������
  {
    // char TempStr[4096];
    // ������� ��������...
    IsCommandPEExeGUI(FullName,ImageSubsystem);
    /* $ 14.06.2002 VVM
       �� ���� ������� - ����� ��� ������� - ��� � �������...
    QuoteSpaceOnly(FullName);
    QuoteSpaceOnly(FileName);
      VVM $ */

    // ��� ������, ����� ����������� ������:
    /* $ 14.06.2002 VVM
       ������ - ���������� ������ � ����� �����...
    if(strpbrk(FullName,"()"))
      IsExistExt=FALSE;
      VVM $ */

    // xstrncpy(TempStr,Command,sizeof(TempStr)-1);
    FAR_CharToOem(FullName,FullName);
    // FAR_CharToOem(FileName,FileName);
    // ReplaceStrings(TempStr,FileName,FullName);
    if(!DestSize)
      DestSize=(int)strlen(FullName);
    // if(Dest && IsExistExt)
    if (Dest)
      xstrncpy(Dest,FullName,DestSize);
  }

  SetFileApisTo(APIS2OEM);
  return(Ret);
}

/* $ 14.06.2002 VVM
   �������� ��� �������, �.�. �� ����� �� �������� */
#ifdef ADD_GUI_CHECK
DWORD IsCommandExeGUI(const char *Command)
{
  char FileName[4096],FullName[4096],*EndName,*FilePart;

  if (*Command=='\"')
  {
    FAR_OemToChar(Command+1,FullName);
    if ((EndName=strchr(FullName,'\"'))!=NULL)
      *EndName=0;
  }
  else
  {
    FAR_OemToChar(Command,FullName);
    if ((EndName=strpbrk(FullName," \t/"))!=NULL)
      *EndName=0;
  }
  int GUIType=0;

  /* $ 07.09.2001 VVM
    + ���������� ���������� ��������� */
  ExpandEnvironmentStrings(FullName,FileName,sizeof(FileName));
  /* VVM $ */

  SetFileApisTo(APIS2ANSI);
  /*$ 18.09.2000 skv
    + to allow execution of c.bat in current directory,
      if gui program c.exe exists somewhere in PATH,
      in FAR's console and not in separate window.
      for(;;) is just to prevent multiple nested ifs.
  */
  for(;;)
  {
    if(BatchFileExist(FileName,FullName,sizeof(FullName)-1))
      break;
  /* skv$*/

    if (SearchPath(NULL,FileName,".exe",sizeof(FullName),FullName,&FilePart))
    {
      SHFILEINFO sfi;
      DWORD ExeType=SHGetFileInfo(FullName,0,&sfi,sizeof(sfi),SHGFI_EXETYPE);
      GUIType=HIWORD(ExeType)>=0x0300 && HIWORD(ExeType)<=0x1000 &&
              /* $ 13.07.2000 IG
                 � VC, ������, ������ ������� ���: 0x4550 == 'PE', ����
                 ������ �������� ���������.
              */
              HIBYTE(ExeType)=='E' && (LOBYTE(ExeType)=='N' || LOBYTE(ExeType)=='P');
              /* IG $ */
    }
/*$ 18.09.2000 skv
    little trick.
*/
    break;
  }
  /* skv$*/
  SetFileApisTo(APIS2OEM);
  return(GUIType);
}
#endif
/* VVM $ */

/* ������� ��� ����������� ���� ��� ��������� ������
   ���� ���� �� ��������� ������ ��� �������� �� DriveLetter:
   ��� ���������� �� ���� �������� � Win9x
*/
void SetCurrentDirectoryForPassivePanel(const char *Comspec,const char *CmdStr)
{
  Panel *PassivePanel=CtrlObject->Cp()->GetAnotherPanel(CtrlObject->Cp()->ActivePanel);

  if (PassivePanel->GetType()==FILE_PANEL)
  {
    //for (int I=0;CmdStr[I]!=0;I++)
    //{
      //if (isalpha(CmdStr[I]) && CmdStr[I+1]==':' && CmdStr[I+2]!='\\')
      //{
        char SavePath[NM],PanelPath[NM],SetPathCmd[NM*2];
        FarGetCurDir(sizeof(SavePath),SavePath);
        PassivePanel->GetCurDir(PanelPath);
        sprintf(SetPathCmd,"%s /C chdir %s",Comspec,QuoteSpace(PanelPath));
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        memset (&si, 0, sizeof (STARTUPINFO));
        si.cb = sizeof (si);
        CreateProcess(NULL,SetPathCmd,NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&si,&pi);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        FarChDir(SavePath);
        //break;
      //}
    //}
  }
}

/* �������-��������� ������� ���������
   ���������� -1 � ������ ������ ���...
*/
int Execute(const char *CmdStr,    // ���.������ ��� ����������
            int AlwaysWaitFinish,  // ����� ���������� ��������?
            int SeparateWindow,    // ��������� � ��������� ����? =2 ��� ������ ShellExecuteEx()
            int DirectRun,         // ��������� ��������? (��� CMD)
            int FolderRun)         // ��� ������?
{
  int nResult = -1;

  bool bIsNT = (WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT);

  char NewCmdStr[4096];
  char NewCmdPar[4096];
  char ExecLine[4096];

  memset (&NewCmdStr, 0, sizeof (NewCmdStr));
  memset (&NewCmdPar, 0, sizeof (NewCmdPar));

  PartCmdLine (
          CmdStr,
          NewCmdStr,
          sizeof(NewCmdStr),
          NewCmdPar,
          sizeof(NewCmdPar)
          );

  /* $ 05.04.2005 AY: ��� �� ���������, ���� ������� ������ ������ ������,
                      ��� ������ � ������ PartCmdLine.
  if(*NewCmdPar)
    RemoveExternalSpaces(NewCmdPar);
  AY $ */

  DWORD dwAttr = GetFileAttributes(NewCmdStr);

  if ( SeparateWindow == 1 )
  {
      if ( !*NewCmdPar && dwAttr != -1 && (dwAttr & FILE_ATTRIBUTE_DIRECTORY) )
      {
          ConvertNameToFull(NewCmdStr,NewCmdStr,sizeof(NewCmdStr));
          SeparateWindow=2;
          FolderRun=TRUE;
      }
  }


  SHELLEXECUTEINFO seInfo;
  memset (&seInfo, 0, sizeof (seInfo));

  seInfo.cbSize = sizeof (SHELLEXECUTEINFO);

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  memset (&si, 0, sizeof (STARTUPINFO));

  si.cb = sizeof (si);

  char Comspec[NM] = {0};
  GetEnvironmentVariable("COMSPEC", Comspec, sizeof(Comspec));

  if ( !*Comspec && (SeparateWindow != 2) )
  {
    Message(MSG_WARNING, 1, MSG(MWarning), MSG(MComspecNotFound), MSG(MErrorCancelled), MSG(MOk));
    return -1;
  }

  int Visible, Size;

  GetCursorType(Visible,Size);
  SetInitialCursorType();

  int PrevLockCount=ScrBuf.GetLockCount();
  ScrBuf.SetLockCount(0);

  ChangePriority ChPriority(THREAD_PRIORITY_NORMAL);

  int ConsoleCP = GetConsoleCP();
  int ConsoleOutputCP = GetConsoleOutputCP();

  FlushInputBuffer();
  ChangeConsoleMode(InitialConsoleMode);

  CONSOLE_SCREEN_BUFFER_INFO sbi={0,};
  GetConsoleScreenBufferInfo(hConOut,&sbi);

  char OldTitle[512];
  GetConsoleTitle (OldTitle, sizeof(OldTitle));

  if (WinVer.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS && *Comspec)
    SetCurrentDirectoryForPassivePanel(Comspec,CmdStr);

  DWORD dwSubSystem;
  DWORD dwError = 0;

  HANDLE hProcess = NULL, hThread = NULL;

  if(FolderRun && SeparateWindow==2)
    AddEndSlash(NewCmdStr); // ����, ����� ShellExecuteEx "�������" BAT/CMD/��.�����, �� �� �������
  else
  {
    PrepareExecuteModule(NewCmdStr,NewCmdStr,sizeof(NewCmdStr)-1,dwSubSystem);

    if(/*!*NewCmdPar && */ dwSubSystem == IMAGE_SUBSYSTEM_UNKNOWN)
    {
      DWORD Error=0, dwSubSystem2=0;
      char *ExtPtr=strrchr(NewCmdStr,'.');

      if(ExtPtr &&
        !(stricmp(ExtPtr,".exe")==0 || stricmp(ExtPtr,".com")==0 ||
          IsBatchExtType(ExtPtr))
        )
        if(GetShellAction(NewCmdStr,dwSubSystem2,Error) && Error != ERROR_NO_ASSOCIATION)
          dwSubSystem=dwSubSystem2;
    }

    if ( dwSubSystem == IMAGE_SUBSYSTEM_WINDOWS_GUI )
      SeparateWindow = 2;
  }

  ScrBuf.Flush ();

  if ( SeparateWindow == 2 )
  {
    FAR_OemToChar (NewCmdStr, NewCmdStr);
    FAR_OemToChar (NewCmdPar, NewCmdPar);

    seInfo.lpFile = NewCmdStr;
    seInfo.lpParameters = NewCmdPar;
    seInfo.nShow = SW_SHOWNORMAL;

    seInfo.lpVerb = (dwAttr&FILE_ATTRIBUTE_DIRECTORY)?NULL:GetShellAction((char *)NewCmdStr, dwSubSystem, dwError);
    //seInfo.lpVerb = "open";
    seInfo.fMask = SEE_MASK_FLAG_NO_UI|SEE_MASK_FLAG_DDEWAIT|SEE_MASK_NOCLOSEPROCESS;

    if ( !dwError )
    {
      SetFileApisTo(APIS2ANSI);

      if ( ShellExecuteEx (&seInfo) )
      {
        hProcess = seInfo.hProcess;
        StartExecTime=clock();
      }
      else
        dwError = GetLastError ();

      SetFileApisTo(APIS2OEM);
    }

    FAR_CharToOem (NewCmdStr, NewCmdStr);
    FAR_CharToOem (NewCmdPar, NewCmdPar);
  }
  else
  {
    char FarTitle[2048];
    if(!Opt.ExecuteFullTitle)
      xstrncpy(FarTitle,CmdStr,sizeof(FarTitle)-1);
    else
    {
      xstrncpy(FarTitle,NewCmdStr,sizeof(FarTitle)-1);
      if (*NewCmdPar)
      {
        xstrncat(FarTitle," ",sizeof(FarTitle)-1);
        xstrncat(FarTitle,NewCmdPar,sizeof(FarTitle)-1);
      }
    }

    if ( bIsNT )
      SetConsoleTitle(FarTitle);
    FAR_OemToChar(FarTitle,FarTitle);
    if ( !bIsNT )
      SetConsoleTitle(FarTitle);
    if (SeparateWindow)
      si.lpTitle=FarTitle;

    QuoteSpace (NewCmdStr);

    strcpy (ExecLine, Comspec);
    strcat (ExecLine, " /C ");

    bool bDoubleQ = false;

    if ( bIsNT && strpbrk (NewCmdStr, "&<>()@^|") )
      bDoubleQ = true;

    if ( (bIsNT && *NewCmdPar) || bDoubleQ )
      strcat (ExecLine, "\"");

    strcat (ExecLine, NewCmdStr);

    if ( *NewCmdPar )
    {
      strcat (ExecLine, " ");
      strcat (ExecLine, NewCmdPar);
    }

    if ( (bIsNT && *NewCmdPar) || bDoubleQ)
      strcat (ExecLine, "\"");

    // // ������� ������ � ����� ����� � 4NT ��� ������ �������
    SetRealColor (COL_COMMANDLINEUSERSCREEN);

    if ( CreateProcess (
        NULL,
        ExecLine,
        NULL,
        NULL,
        false,
        SeparateWindow?CREATE_NEW_CONSOLE|CREATE_DEFAULT_ERROR_MODE:CREATE_DEFAULT_ERROR_MODE,
        NULL,
        NULL,
        &si,
        &pi
        ) )
     {
       hProcess = pi.hProcess;
       hThread = pi.hThread;

       StartExecTime=clock();
     }
    else
       dwError = GetLastError ();
  }

  if ( !dwError )
  {
    if ( hProcess )
    {
      ScrBuf.Flush ();

//      char s[100];

//      sprintf (s, "%d %d", AlwaysWaitFinish, SeparateWindow);

//      MessageBox (0, s, s, MB_OK);

      if ( AlwaysWaitFinish || !SeparateWindow )
      {
        if ( Opt.ConsoleDetachKey == 0 )
          WaitForSingleObject(hProcess,INFINITE);
        else
        {
          /*$ 12.02.2001 SKV
            ����� ����� ;)
            ��������� ��������� ������� �� ���������������� ��������.
            ������� ������� � System/ConsoleDetachKey
          */
          HANDLE hHandles[2];
          HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
          HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

          INPUT_RECORD ir[256];
          DWORD rd;

          int vkey=0,ctrl=0;
          TranslateKeyToVK(Opt.ConsoleDetachKey,vkey,ctrl,NULL);
          int alt=ctrl&PKF_ALT;
          int shift=ctrl&PKF_SHIFT;
          ctrl=ctrl&PKF_CONTROL;

          hHandles[0] = hProcess;
          hHandles[1] = hInput;

          bool bAlt, bShift, bCtrl;
          DWORD dwControlKeyState;

          while( WaitForMultipleObjects (
              2,
              hHandles,
              FALSE,
              INFINITE
              ) != WAIT_OBJECT_0
              )
          {
            if ( PeekConsoleInput(hHandles[1],ir,256,&rd) && rd)
            {
              int stop=0;

              for(DWORD i=0;i<rd;i++)
              {
                PINPUT_RECORD pir=&ir[i];

                if(pir->EventType==KEY_EVENT)
                {
                  dwControlKeyState = pir->Event.KeyEvent.dwControlKeyState;

                  bAlt = (dwControlKeyState & LEFT_ALT_PRESSED) || (dwControlKeyState & RIGHT_ALT_PRESSED);
                  bCtrl = (dwControlKeyState & LEFT_CTRL_PRESSED) || (dwControlKeyState & RIGHT_CTRL_PRESSED);
                  bShift = (dwControlKeyState & SHIFT_PRESSED) != 0;

                  if ( vkey==pir->Event.KeyEvent.wVirtualKeyCode &&
                     (alt ?bAlt:!bAlt) &&
                     (ctrl ?bCtrl:!bCtrl) &&
                     (shift ?bShift:!bShift) )
                  {
                    HICON hSmallIcon=NULL,hLargeIcon=NULL;

                    if ( hFarWnd )
                    {
                      hSmallIcon = CopyIcon((HICON)SendMessage(hFarWnd,WM_SETICON,0,(LPARAM)0));
                      hLargeIcon = CopyIcon((HICON)SendMessage(hFarWnd,WM_SETICON,1,(LPARAM)0));
                    }

                    ReadConsoleInput(hInput,ir,256,&rd);

                    /*
                      �� ����� ������� CloseConsole, ������, ��� ��� ��������
                      ConsoleMode �� ���, ��� ��� �� ������� Far'�,
                      ���� ���������� ���������� ����� � �� �������.
                    */
                    CloseHandle(hInput);
                    CloseHandle(hOutput);

                    delete KeyQueue;
                    KeyQueue=NULL;

                    FreeConsole();
                    AllocConsole();

                    if ( hFarWnd ) // ���� ���� ����� HOTKEY, �� ������ ������ ��� ������.
                      SendMessage(hFarWnd,WM_SETHOTKEY,0,(LPARAM)0);

                    SetConsoleScreenBufferSize(hOutput,sbi.dwSize);
                    SetConsoleWindowInfo(hOutput,TRUE,&sbi.srWindow);
                    SetConsoleScreenBufferSize(hOutput,sbi.dwSize);

                    Sleep(100);
                    InitConsole(0);

                    hFarWnd = 0;
                    InitDetectWindowedMode();

                    if ( hFarWnd )
                    {
                      if ( Opt.SmallIcon )
                      {
                        char FarName[NM];
                        GetModuleFileName(NULL,FarName,sizeof(FarName));
                        ExtractIconEx(FarName,0,&hLargeIcon,&hSmallIcon,1);
                      }

                      if ( hLargeIcon != NULL )
                        SendMessage (hFarWnd,WM_SETICON,1,(LPARAM)hLargeIcon);

                      if ( hSmallIcon != NULL )
                        SendMessage (hFarWnd,WM_SETICON,0,(LPARAM)hSmallIcon);
                    }

                    stop=1;
                    break;
                  }
                }
              }

              if ( stop )
                break;
            }

            Sleep(100);
          }
        }
      }

//      MessageBox (0, "close", "asd", MB_OK);

      ScrBuf.FillBuf();

      CloseHandle (hProcess);
    }

     if ( hThread )
       CloseHandle (hThread);

    nResult = 0;
  }
  else
  {
    char OutStr[1024];

    if( Opt.ExecuteShowErrorMessage )
    {
      SetMessageHelp("ErrCannotExecute");

      xstrncpy(OutStr,NewCmdStr,sizeof(OutStr)-1);
      Unquote(OutStr);

      Message(MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),MSG(MCannotExecute),OutStr,MSG(MOk));
    }
    else
    {
      ScrBuf.Flush ();

      sprintf(OutStr,MSG(MExecuteErrorMessage),NewCmdStr);
      char *PtrStr=FarFormatText(OutStr,ScrX,OutStr,sizeof(OutStr),"\n",0);
      printf(PtrStr);
      ScrBuf.FillBuf();
    }

  }

  SetFarConsoleMode(TRUE);

  /* �������������� ��������� �������, �.�. SetCursorType ������ �� �������
      ���������� ����� �����������, ������� � ������ ������ ������� �����.
  */
  SetCursorType(Visible,Size);
  SetRealCursorType(Visible,Size);

  SetConsoleTitle(OldTitle);

  /* ���� ���� �������� ������� �������, ��������
     mode con lines=50 cols=100
     �� ��� �� ���� �� ��������� ������� �������.
     ��� ����� ���� ���� ��������� ������ ��� :-)
  */
  GenerateWINDOW_BUFFER_SIZE_EVENT(-1,-1); //����...

  if( Opt.RestoreCPAfterExecute )
  {
    // ����������� CP-������� ����� ���������� �����
    SetConsoleCP(ConsoleCP);
    SetConsoleOutputCP(ConsoleOutputCP);
  }


  return nResult;
}


int CommandLine::CmdExecute(char *CmdLine,int AlwaysWaitFinish,
                            int SeparateWindow,int DirectRun)
{
  LastCmdPartLength=-1;
  if (!SeparateWindow && CtrlObject->Plugins.ProcessCommandLine(CmdLine))
  {
    /* $ 12.05.2001 DJ
       �������� ������ ���� �������� ������� �������
    */
    if (CtrlObject->Cp()->IsTopFrame())
    {
      //CmdStr.SetString("");
      GotoXY(X1,Y1);
      mprintf("%*s",X2-X1+1,"");
      Show();
      ScrBuf.Flush();
    }
    return(-1);
  }
  int Code;
  /* 21.11.2001 VVM
    ! � ��������� ��� �������� � ����������� ����.
      ����� �� ������ ������� ����� :) */
  {
    CONSOLE_SCREEN_BUFFER_INFO sbi0,sbi1;
    GetConsoleScreenBufferInfo(hConOut,&sbi0);
    {
      RedrawDesktop Redraw(TRUE);

      ScrollScreen(1);
      MoveCursor(X1,Y1);
      if (CurDir[0] && CurDir[1]==':')
        FarChDir(CurDir);
      CmdStr.SetString("");
      if ((Code=ProcessOSCommands(CmdLine,SeparateWindow)) == TRUE)
        Code=-1;
      else
      {
        char TempStr[2048];
        xstrncpy(TempStr,CmdLine,sizeof(TempStr)-1);
        if(Code == -1)
          ReplaceStrings(TempStr,"/","\\",-1);
        Code=Execute(TempStr,AlwaysWaitFinish,SeparateWindow,DirectRun);
      }

      GetConsoleScreenBufferInfo(hConOut,&sbi1);
      if(!(sbi0.dwSize.X == sbi1.dwSize.X && sbi0.dwSize.Y == sbi1.dwSize.Y))
        CtrlObject->CmdLine->CorrectRealScreenCoord();

      //if(Code != -1)
      {
        int CurX,CurY;
        GetCursorPos(CurX,CurY);
        if (CurY>=Y1-1)
          ScrollScreen(Min(CurY-Y1+2,2/*Opt.ShowKeyBar ? 2:1*/));
      }
    }

    if(!Flags.Check(FCMDOBJ_LOCKUPDATEPANEL))
      ShellUpdatePanels(CtrlObject->Cp()->ActivePanel,FALSE);
    /*else
    {
      CtrlObject->Cp()->LeftPanel->UpdateIfChanged(UIC_UPDATE_FORCE);
      CtrlObject->Cp()->RightPanel->UpdateIfChanged(UIC_UPDATE_FORCE);
      CtrlObject->Cp()->Redraw();
    }*/
  }
  /* VVM $ */
  ScrBuf.Flush();
  return(Code);
}


/* 20.03.2002 IS
   "if [not] exist" ������ ������ � ������� ������
   PrepareOSIfExist ������ ��������� � ���������� const
*/
/* $ 14.01.2001 SVS
   + � ProcessOSCommands ��������� ���������
     "IF [NOT] EXIST filename command"
     "IF [NOT] DEFINED variable command"

   ��� ������� ������������� ��� ��������� ���������� IF`�
   CmdLine - ������ ������ ����
     if exist file if exist file2 command
   Return - ��������� �� "command"
            ������ ������ - ������� �� ���������
            NULL - �� ������� "IF" ��� ������ � �����������, ��������
                   �� exist, � exist ��� ����������� �������.

   DEFINED - ������� EXIST, �� ��������� � ����������� �����

   �������� ������ (CmdLine) �� ��������������!!! - �� ��� ���� ��������� const
                                                    IS 20.03.2002 :-)
*/
const char* WINAPI PrepareOSIfExist(const char *CmdLine)
{
  if(!CmdLine || !*CmdLine)
    return NULL;

  char Cmd[1024];
  const char *PtrCmd=CmdLine, *CmdStart;
  int Not=FALSE;
  int Exist=0; // ������� ������� ����������� "IF [NOT] EXIST filename command"
               // > 0 - ���� ����� �����������

  /* $ 25.04.2001 DJ
     ��������� @ � IF EXIST
  */
  if (*PtrCmd == '@')
  {
    // ����� @ ������������; �� ������� � ���������� ����� �������
    // ExtractIfExistCommand � filetype.cpp
    PtrCmd++;
    while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd;
  }
  /* DJ $ */
  while(1)
  {
    if (!PtrCmd || !*PtrCmd || memicmp(PtrCmd,"IF ",3))
      break;

    PtrCmd+=3; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;

    if (memicmp(PtrCmd,"NOT ",4)==0)
    {
      Not=TRUE;
      PtrCmd+=4; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;
    }

    if (*PtrCmd && !memicmp(PtrCmd,"EXIST ",6))
    {
      PtrCmd+=6; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;
      CmdStart=PtrCmd;

      /* $ 25.04.01 DJ
         ��������� ������� ������ ����� ����� � IF EXIST
      */
      BOOL InQuotes=FALSE;
      while (*PtrCmd)
      {
        if (*PtrCmd == '\"')
          InQuotes = !InQuotes;
        else if (*PtrCmd == ' ' && !InQuotes)
          break;
        PtrCmd++;
      }

      if(PtrCmd && *PtrCmd && *PtrCmd == ' ')
      {
        char ExpandedStr[8192];
        memmove(Cmd,CmdStart,PtrCmd-CmdStart+1);
        Cmd[PtrCmd-CmdStart]=0;
        Unquote(Cmd);
//_SVS(SysLog("Cmd='%s'",Cmd));
        if (ExpandEnvironmentStr(Cmd,ExpandedStr,sizeof(ExpandedStr))!=0)
        {
          char FullPath[8192]="";
          if(!(Cmd[1] == ':' || (Cmd[0] == '\\' && Cmd[1]=='\\') || ExpandedStr[1] == ':' || (ExpandedStr[0] == '\\' && ExpandedStr[1]=='\\')))
          {
            if(CtrlObject)
              CtrlObject->CmdLine->GetCurDir(FullPath);
            else
              FarGetCurDir(sizeof(FullPath),FullPath);
            AddEndSlash(FullPath);
          }
          strcat(FullPath,ExpandedStr);
          DWORD FileAttr=(DWORD)-1;
          if(strpbrk(&ExpandedStr[PathPrefix(ExpandedStr)?4:0],"*?")) // ��� �����?
          {
            WIN32_FIND_DATA wfd;
            if(GetFileWin32FindData(FullPath,&wfd))
              FileAttr=wfd.dwFileAttributes;
          }
          else
          {
            ConvertNameToFull(FullPath, FullPath, sizeof(FullPath));
            FileAttr=GetFileAttributes(FullPath);
          }
//_SVS(SysLog("%08X FullPath=%s",FileAttr,FullPath));
          if(FileAttr != (DWORD)-1 && !Not || FileAttr == (DWORD)-1 && Not)
          {
            while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd;
            Exist++;
          }
          else
            return "";
        }
      }
      /* DJ $ */
    }
    // "IF [NOT] DEFINED variable command"
    else if (*PtrCmd && !memicmp(PtrCmd,"DEFINED ",8))
    {
      PtrCmd+=8; while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd; if(!*PtrCmd) break;
      CmdStart=PtrCmd;
      if(*PtrCmd == '"')
        PtrCmd=strchr(PtrCmd+1,'"');

      if(PtrCmd && *PtrCmd)
      {
        PtrCmd=strchr(PtrCmd,' ');
        if(PtrCmd && *PtrCmd && *PtrCmd == ' ')
        {
          char ExpandedStr[8192];
          memmove(Cmd,CmdStart,PtrCmd-CmdStart+1);
          Cmd[PtrCmd-CmdStart]=0;
          DWORD ERet=GetEnvironmentVariable(Cmd,ExpandedStr,sizeof(ExpandedStr));
//_SVS(SysLog(Cmd));
          if(ERet && !Not || !ERet && Not)
          {
            while(*PtrCmd && IsSpace(*PtrCmd)) ++PtrCmd;
            Exist++;
          }
          else
            return "";
        }
      }
    }
  }
  return Exist?PtrCmd:NULL;
}
/* SVS $ */
/* IS $ */

int CommandLine::ProcessOSCommands(char *CmdLine,int SeparateWindow)
{
  Panel *SetPanel;
  int Length;

  SetPanel=CtrlObject->Cp()->ActivePanel;

  if (SetPanel->GetType()!=FILE_PANEL && CtrlObject->Cp()->GetAnotherPanel(SetPanel)->GetType()==FILE_PANEL)
    SetPanel=CtrlObject->Cp()->GetAnotherPanel(SetPanel);

  RemoveTrailingSpaces(CmdLine);

  if (!SeparateWindow && isalpha(CmdLine[0]) && CmdLine[1]==':' && CmdLine[2]==0)
  {
    char NewDir[10];
    sprintf(NewDir,"%c:",toupper(CmdLine[0]));
    FarChDir(CmdLine);
    if (getdisk()!=NewDir[0]-'A')
    {
      strcat(NewDir,"\\");
      FarChDir(NewDir);
    }
    SetPanel->ChangeDirToCurrent();
    return(TRUE);
  }

  // SET [����������=[������]]
  if (strnicmp(CmdLine,"SET ",4)==0)
  {
    char Cmd[1024];
#if 0
    // ������� ��� "SET /P variable=[promptString]"
    int Offset=4, NeedInput=FALSE;
    char *ParamP=strchr(CmdLine,'/');
    if (ParamP && (ParamP[1] == 'P' || ParamP[1] == 'p') && ParamP[2] == ' ')
    {
      Offset=ParamP-CmdLine+3;
      NeedInput=TRUE;
    }

    xstrncpy(Cmd,CmdLine+Offset,sizeof(Cmd)-1);

    char *Value=strchr(Cmd,'=');
    if (Value==NULL)
      return(FALSE);

    *Value=0;
    if(NeedInput)
    {
      Offset=Value-Cmd+1;
      if(!::GetString("",Value+1,"PromptSetEnv","",Value+1,sizeof(Cmd)-Offset-1,NULL,FIB_ENABLEEMPTY))
        return TRUE;
    }
#else
    xstrncpy(Cmd,CmdLine+4,sizeof(Cmd)-1);
    char *Value=strchr(Cmd,'=');
    if (Value==NULL)
      return(FALSE);

    *Value=0;
#endif
    FAR_OemToChar(Cmd, Cmd);

    if (Value[1]==0)
      SetEnvironmentVariable(Cmd,NULL);
    else
    {
      char ExpandedStr[8192];
      /* $ 17.06.2001 IS
         ! ��������� ExpandEnvironmentStr, �.�. ��� ��������� �������� �
           �������� �������.
         + ������������ ������ ����� SetEnvironmentVariable �� OEM � ANSI
      */
      if (ExpandEnvironmentStr(Value+1,ExpandedStr,sizeof(ExpandedStr))!=0)
      {
        // ���������� ��������� ������ ���� � ANSI???
        FAR_OemToChar(ExpandedStr, ExpandedStr);
        SetEnvironmentVariable(Cmd,ExpandedStr);
      }
      /* IS $ */
    }
    return(TRUE);
  }

  if (!memicmp(CmdLine,"REM ",4) || !memicmp(CmdLine,"::",2))
  {
    return TRUE;
  }

  if (!memicmp(CmdLine,"CLS",3))
  {
    if(CmdLine[3])
      return FALSE;

    ClearScreen(COL_COMMANDLINEUSERSCREEN);
    return TRUE;
  }

  /*
  Displays or sets the active code page number.
  CHCP [nnn]
    nnn   Specifies a code page number (Dec or Hex).
  Type CHCP without a parameter to display the active code page number.
  */
  if (!memicmp(CmdLine,"CHCP",4))
  {
    if(CmdLine[4] == 0 || !(CmdLine[4] == ' ' || CmdLine[4] == '\t'))
      return(FALSE);

    char *Ptr=RemoveExternalSpaces(CmdLine+5), Chr;

    if(!isdigit(*Ptr))
      return FALSE;

    while((Chr=*Ptr) != 0)
    {
      if(!isdigit(Chr))
        break;
      ++Ptr;
    }
    UINT cp=(UINT)strtol(CmdLine+5,&Ptr,10);
    BOOL r1=SetConsoleCP(cp);
    BOOL r2=SetConsoleOutputCP(cp);
    if(r1 && r2) // ���� ��� ���, �� ���  �...
    {
      InitRecodeOutTable(cp);
      LocalUpperInit();
      InitLCIDSort();
      InitKeysArray();
      CtrlObject->Cp()->Redraw();
      ScrBuf.Flush();
      return TRUE;
    }
    else  // ��� ������ ������� chcp ���� ������ ;-)
     return FALSE;
  }

  /* $ 14.01.2001 SVS
     + � ProcessOSCommands ��������� ���������
       "IF [NOT] EXIST filename command"
       "IF [NOT] DEFINED variable command"
  */
  if (memicmp(CmdLine,"IF ",3)==0)
  {
    const char *PtrCmd=PrepareOSIfExist(CmdLine);
    // ����� PtrCmd - ��� ������� �������, ��� IF
    if(PtrCmd && *PtrCmd && CtrlObject->Plugins.ProcessCommandLine(PtrCmd))
    {
      //CmdStr.SetString("");
      GotoXY(X1,Y1);
      mprintf("%*s",X2-X1+1,"");
      Show();
      return TRUE;
    }
    return FALSE;
  }
  /* SVS $ */

  /* $ 16.04.2002 DJ
     ���������� ���������, ���� ����� Shift-Enter
  */
  if (!SeparateWindow &&  /* DJ $ */
      (strnicmp(CmdLine,"CD",Length=2)==0 || strnicmp(CmdLine,"CHDIR",Length=5)==0) &&
      (IsSpace(CmdLine[Length]) || CmdLine[Length]=='\\' || CmdLine[Length]=='/' || TestParentFolderName(CmdLine+Length)))
  {
    int ChDir=(Length==5);

    while (IsSpace(CmdLine[Length]))
      Length++;

    char ExpandedDir[8192];
    xstrncpy(ExpandedDir,&CmdLine[Length],sizeof(ExpandedDir)-1);

    Unquote(ExpandedDir);
    ExpandEnvironmentStr(ExpandedDir,ExpandedDir,sizeof(ExpandedDir));

    // ������������� ����� ����� �� "���������"
    if(ExpandedDir[1] == ':' && isalpha(ExpandedDir[0]))
      ExpandedDir[0]=toupper(ExpandedDir[0]);

    if(SetPanel->GetMode()!=PLUGIN_PANEL && ExpandedDir[0] == '~' && !ExpandedDir[1] && GetFileAttributes(ExpandedDir) == (DWORD)-1)
    {
      GetRegKey(strSystemExecutor,"~",(char*)ExpandedDir,FarPath,sizeof(ExpandedDir)-1);
      DeleteEndSlash(ExpandedDir);
    }

    if(strpbrk(&ExpandedDir[PathPrefix(ExpandedDir)?4:0],"?*")) // ��� �����?
    {
      WIN32_FIND_DATA wfd;
      if(GetFileWin32FindData(ExpandedDir,&wfd))
      {
        char *Ptr=strrchr(ExpandedDir,'\\');
        if (!Ptr)
          Ptr=strrchr(ExpandedDir,'/');
        if(Ptr)
          *++Ptr=0;
        else
          *ExpandedDir=0;
        xstrncat(ExpandedDir,wfd.cFileName,sizeof(ExpandedDir)-1);
      }
    }

    // \\?\UNC\<hostname>\<sharename>\<dirname> , � ����� cd \\.\<disk>:\<dirname>

    /* $ 15.11.2001 OT
      ������� ��������� ���� �� ����� "�������" ����������.
      ���� �� ���, �� ����� �������� ������, ��� ��� ���������� ���������
    */
    DWORD DirAtt=GetFileAttributes(ExpandedDir);
    if (DirAtt!=INVALID_FILE_ATTRIBUTES && (DirAtt & FILE_ATTRIBUTE_DIRECTORY) && PathMayBeAbsolute(ExpandedDir))
    {
      ReplaceStrings(ExpandedDir,"/","\\",-1);
      SetPanel->SetCurDir(ExpandedDir,TRUE);
      return TRUE;
    }
    /* OT $ */

    /* $ 20.09.2002 SKV
      ��� ��������� ����������� ��������� ����� ������� ���:
      cd net:server � cd ftp://server/dir
      ��� ��� ��� �� �� ������� �������� �
      cd s&r:, cd make: � �.�., ������� � �����
      �������� �� ����� �������� ���������.
    */
    /*
    if (CtrlObject->Plugins.ProcessCommandLine(ExpandedDir))
    {
      //CmdStr.SetString("");
      GotoXY(X1,Y1);
      mprintf("%*s",X2-X1+1,"");
      Show();
      return(TRUE);
    }
    */
    /* SKV $ */

    if (SetPanel->GetType()==FILE_PANEL && SetPanel->GetMode()==PLUGIN_PANEL)
    {
      SetPanel->SetCurDir(ExpandedDir,ChDir);
      return(TRUE);
    }

    //if (ExpandEnvironmentStr(ExpandedDir,ExpandedDir,sizeof(ExpandedDir))!=0)
    {
      if (!FarChDir(ExpandedDir))
        return(FALSE);
    }

    SetPanel->ChangeDirToCurrent();
    if (!SetPanel->IsVisible())
      SetPanel->SetTitle();

    return(TRUE);
  }
  return(FALSE);
}

// ��������� "��� ������?"
BOOL IsBatchExtType(const char *ExtPtr)
{
  char *PtrBatchType=Opt.ExecuteBatchType;
  while(*PtrBatchType)
  {
    if(stricmp(ExtPtr,PtrBatchType)==0)
      return TRUE;
    PtrBatchType+=strlen(PtrBatchType)+1;
  }

  return FALSE;
}

// ������ ����������? (� ������ ������ ��� - ����������� ����������)
BOOL BatchFileExist(const char *FileName,char *DestName,int SizeDestName)
{
  char *PtrBatchType=Opt.ExecuteBatchType;
  BOOL Result=FALSE;

  char *FullName=(char*)alloca(strlen(FileName)+64);
  if(FullName)
  {
    strcpy(FullName,FileName);
    char *FullNameExt=FullName+strlen(FullName);

    while(*PtrBatchType)
    {
      strcat(FullNameExt,PtrBatchType);

      if(GetFileAttributes(FullName)!=-1)
      {
        xstrncpy(DestName,FullName,SizeDestName);
        Result=TRUE;
        break;
      }

      PtrBatchType+=strlen(PtrBatchType)+1;
    }
  }

  return Result;
}
