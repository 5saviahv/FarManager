/*
mkdir.cpp

�������� ��������

*/

/* Revision: 1.13 18.03.2002 $ */

/*
Modify:
  18.03.2002 SVS
    + ������� ������� ������� � ������ (FIB_NOAMPERSAND)
  07.12.2001 IS
    + C������� ���������� ��������� �� ��� ������ �����������.
  26.09.2001 SVS
    + Opt.AutoUpdateLimit -  ���� ����� ���������� �� ��������� ���������
      ������ (���� �� ���������� �� ����� ��������).
  24.07.2001 IS
    ! ��������� ����������� ��������� SVS. �������������� ������������ ����
      ������� �� �������� ����� ��������, ��� �������, � ����� ��� �������� �
      ��������. ������ ���������� � ������� ���������� ������, ���� ������ ��
      �������� ������������ � _�������_
  23.07.2001 SVS
    ! ��������� ��������� ��������� �������� ���������
  05.06.2001 IS
    + ������ ����������� ����� VVM
  05.06.2001 VVM
    + ������ ������� � ����������� ��������
  04.06.2001 IS
    + �������� ��������� ��������� ��������� �� ���� ���
  02.06.2001 KM
    + ��������� �����������: ��������� ������ [ OK ] � [ Cancel ]
      � ������ �������� ��������
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  31.07.2000 SVS
    ! �������� ���������� ����� � ������� �������� ��������
  25.07.2000 IG
    - Bug 24 (�� �������������� ������, ����� ���������� ���������� �������� ����������)
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "treelist.hpp"
#include "ctrlobj.hpp"
#include "plugin.hpp"
#include "udlist.hpp"

/* $ 04.06.2001 IS
     �������� ��������� ��������� ��������� �� ���� ���
*/
void ShellMakeDir(Panel *SrcPanel)
{
  /* $ 02.06.2001 KM
     + ��������� �����������: ��������� ������ [ OK ] � [ Cancel ]
       � ������ �������� ��������
  */
  char DirName[NM];
  *DirName=0;
  UserDefinedList DirList;

  /* $ 07.12.2001 IS
     �������� ���������� ��������� �� ��� ������ �����������
  */
  BOOL MultiMakeDir=Opt.MultiMakeDir;
  for(;;)
  {
    if (!GetString(MSG(MMakeFolderTitle),MSG(MCreateFolder),"NewFolder",
         DirName,DirName,sizeof(DirName),"MakeFolder",
         FIB_NOAMPERSAND|FIB_BUTTONS|FIB_EXPANDENV|FIB_CHECKBOX,&MultiMakeDir,
         MSG(MMultiMakeDir)))
      return;

    Opt.MultiMakeDir=MultiMakeDir;

    // ��� �� ������ �������� ���������� ��������, �������
    // ���������� � �������! ����� ������� �� ���������
    // ����� ������� � �������
    if(Opt.MultiMakeDir && strpbrk(DirName,";,\"") == NULL)
       QuoteSpaceOnly(DirName);

    if(!Opt.MultiMakeDir)   // ����� ������� ������ ���� �������
    {
      Unquote(DirName);     // ������ ��� ������ �������
      InsertQuote(DirName); // ������� � �������, �.�. ����� ���� �����������
    }

    if(DirList.Set(DirName)) break;
    else Message(MSG_DOWN|MSG_WARNING,1,MSG(MWarning),
                 MSG(MIncorrectDirList), MSG(MOk));
  }
  /* IS $ */
  /* KM $ */
  *DirName=0;
  const char *OneDir;
  DirList.Start();
  while(NULL!=(OneDir=DirList.GetNext()))
  {
    strncpy(DirName, OneDir, sizeof(DirName)-1);
    //Unquote(DirName);
    if (Opt.CreateUppercaseFolders && !IsCaseMixed(DirName))
      LocalStrupr(DirName);

    int Length=strlen(DirName);
    /* $ 25.07.2000 IG
       Bug 24 (�� �������������� ������, ����� ���������� ����������
       �������� ����������)
    */
    while (Length>0 && DirName[Length-1]==' ')
      Length--;
    DirName[Length]=0;
    if (Length>0 && (DirName[Length-1]=='/' || DirName[Length-1]=='\\'))
      DirName[Length-1]=0;

    char bSuccess = 0;

    int Error=FALSE;

    for (char *ChPtr=DirName;*ChPtr!=0;ChPtr++)
      if (*ChPtr=='\\' || *ChPtr=='/')
      {
        *ChPtr=0;
        if (CreateDirectory(DirName,NULL))
        {
          TreeList::AddTreeName(DirName);
          bSuccess = 1;
        }
        *ChPtr='\\';
      }

    while (!CreateDirectory(DirName,NULL))
    {
      int LastError=GetLastError();
      if (LastError==ERROR_ALREADY_EXISTS || LastError==ERROR_BAD_PATHNAME ||
          LastError==ERROR_INVALID_NAME)
      {
        Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,1,MSG(MError),MSG(MCannotCreateFolder),DirName,MSG(MOk));
        if (bSuccess) break;
          else return;
      }
      else
        if (Message(MSG_DOWN|MSG_WARNING|MSG_ERRORTYPE,2,MSG(MError),MSG(MCannotCreateFolder),DirName,MSG(MRetry),MSG(MCancel))!=0)
          if (bSuccess) break;
            else return;
    }
    /* IG $ */
    TreeList::AddTreeName(DirName);
  }

  SrcPanel->Update(UPDATE_KEEP_SELECTION);

  if(*DirName)
  {
    char *Slash=strchr(DirName,'\\');
    if (Slash!=NULL)
      *Slash=0;
    SrcPanel->GoToFile(DirName);
  }
  SrcPanel->Redraw();

  Panel *AnotherPanel=CtrlObject->Cp()->GetAnotherPanel(SrcPanel);
  int AnotherType=AnotherPanel->GetType();
  if(AnotherPanel->NeedUpdatePanel(SrcPanel) || AnotherType==QVIEW_PANEL)
  {
    AnotherPanel->Update(UPDATE_KEEP_SELECTION|UPDATE_SECONDARY);
    AnotherPanel->Redraw();
  }
}
/* IS $ */
