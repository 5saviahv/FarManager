/*
plugapi.cpp

API, ��������� �������� (�������, ����, ...)

*/

/* Revision: 1.97 11.10.2001 $ */

/*
Modify:
  11.10.2001 IS
    + ��������� EF_DELETEONCLOSE
  08.10.2001 OT
    ! ������ ������������ ������ � ��������� ������
  07.10.2001 SVS
    - ���� � ��� ����������� ��������!
  27.09.2001 IS
    - ����� ������ ��� ������������� strncpy
  22.09.2001 OT
    ����� Viewer � Editor �� ���� ������� ���������� ����-�� � background window
  21.09.2001 SVS
    - ����. ��� ������ ���, ���� ������ �������� Message ��� Dialog, ��
      ���� �������, �.�. Frame ��� �������� ����. � ����� ������� ��������
      �� NULL ��� Lock/Unlock
  20.09.2001 SVS
    ! ��������� ������� � ����������, ��������� �� �������� "��������
      ������������ ������� ������"
      � ��� �� ���� ������ ���� �� �������???
  16.09.2001 SVS
    ! ����������� ����������
  15.09.2001 tran
    + ACTL_GETFARHWND
  09.09.2001 IS
    + ��������� VF_DISABLEHISTORY/EF_DISABLEHISTORY - ������ ����� ���������
      ���������� ����� ����� � �������.
  08.09.2001 IS
    ! ������ ��� ����� ����������� � ������� ���������/��������������
      ��� ������������� ������� ��� Editor/Viewer
  13.08.2001 SKV
    + FCTL_GETCMDLINESELECTION,FCTL_SETCMDLINESELECTION
  13.08.2001 SVS
    - ����� � ������� ��� ���������� �������� PluginNumber � �������
      GetPluginDirList() ��� ������, ���� hPlugin=INVALID_HANDLE_VALUE
  07.08.2001 IS
    + ���� � FarCharTable: ��� ������� ���������� �������� �� ������������
      ���������� ������� ��������� CharTableSet ����������� ������� ��� OEM.
    ! FarCharTable: ������ �������� ������ �� const, �.�. �� �������� � �������.
  05.08.2001 SVS
    - ���� � ������� ����� � �������� �� ��������
    ! ������� �� �������� �������� (���������) ��������
  01.08.2001 SVS
    + �������� ������� ���� � ������ ������� (��� ���� ������)!
  31.07.2001 IS
    + ��������� � ����� � FarDialogEx, FarMenuFn, FarMessageFn:
      1. ����� ���� F*_CUSTOMNAME
      2. ������� ������������� ��� ������ � *HelpTopic=='#'
      3. ���������, ��� ������ ���, ���� ��� ���� ������.
  31.07.2001 IS
    - ���: ���� ������� ����������� � ������ ������� ��
      ctrl-key, alt-key ��� shift-key, ���� ���� ��� ����������
      ����� �� ���� ������� � BreakKeys, � ������� ���� �����
      ��������� ������� �� ������ key.
    ! ��������� const (FarGetMsgFn)
  31.07.2001 SVS
    + ��������� ������ �� �������: ~Text~@#Path#Topic@
  27.07.2001 SVS
    + ����� ��� �������� ���������� � ACTL_POSTKEYSEQUENCE
    + ����� ����������� ������� ��� ������ GetPluginDirList
      ��������� �� �������� ������, �.�. hPlugin=INVALID_HANDLE_VALUE.
      �� �������� � Control
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  19.07.2001 OT
    ������ ��� � ������������
  18.07.2001 OT
    VFMenu
  16.07.2001 SVS
    + ��������� FarMenuItemEx � FarMenu
  26.06.2001 SVS
    ! __except -> EXCEPT
  26.06.2001 SKV
    + ACTL_COMMIT
  25.06.2001 IS
   ! ��������� const
  21.06.2001 SVS
    ! ACTL_POSTSEQUENCEKEY -> ACTL_POSTKEYSEQUENCE - (� ����� ������ eng)
    ! SequenceKey           -> KeySequence
    ! � ACTL_GETWINDOWINFO ������� �������� Param �� NULL
  20.06.2001 SVS
    ! ACTL_PROCESSSEQUENCEKEY -> ACTL_POSTSEQUENCEKEY, � �.�. ��������
      ���� ������� ������ ������������ �� Macro API
  19.06.2001 IS
    - ���: �� �������� ��������������� ��������� ����� 268.
  06.06.2001 SVS
    - �� ����� ���������� ������� � ���� �������������� BreakKeys.
  05.06.2001 tran
    + ACTL_GETWINDOWCOUNT,ACTL_GETWINDOWINFO,ACTL_SETCURRENTWINDOW
  04.06.2001 SVS
    ! ����� �� ������ Checked ������� � ����. ����������.
  03.06.2001 SVS
    ! ��������� � ����� � ���������� UserData � VMenu
  31.05.2001 OT
    - ExitCode � ������������ � ����������� ������������� plugin.hlp
  27.05.2001 OT
    - ����������� ������� ������������� FileView � FileEdit � FarViewer() � FarEditor()
  26.05.2001 OT
    - ����������� ������ ������� � NFZ
  21.05.2001 DJ
    + FDLG_NONMODAL
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  16.05.2001 DJ
    ! proof-of-concept
  16.05.2001 SVS
    + #include "farexcpt.hpp"
  15.05.2001 OT
    ! NWZ -> NFZ
  14.05.2001 SVS
    ! FDLG_SMALLDILAOG -> FDLG_SMALLDIALOG
  12.05.2001 DJ
    + EF_ENABLE_F6, VF_ENABLE_F6
  12.05.2001 DJ
    ! ����� SaveScr ��� ������ ���������/������ �� �������
  10.05.2001 SVS
    + � FarDialogEx() ��������� ��������� ������ FDLG_WARNING � FDLG_SMALLDILAOG.
  06.05.2001 DJ
    ! �������� #include
  06.05.2001 ��
    ! �������������� Window � Frame :)
  05.05.2001 DJ
    + �������� NWZ
  29.04.2001 ��
    + ��������� NWZ �� ����������
  01.05.2001 SVS
    - ������ PluginNumber � FarDialogEx() ������� ItemsNumber :-(((
  28.04.2001 SVS
    + ��������� ���������� � FarDialogEx() - ����������� ����� ����� ���
      ������ � ��������, ��� �����.
  11.09.2001 SVS
    - FarMessageFn(): ���� ���������� ���� FMSG_ERRORTYPE, ItemsNumber ������ 1,
      ���� ������ ��������� ���������� ���������.
    - FarMessageFn(): ������� ��������� ���� #530 - ����������.
  09.04.2001 SVS
    ! ��������� �� ��������� ���������
  08.04.2001 SVS
    ! ��������� � FarShowHelp
  28.03.2001 SVS
    ! ACTL_GETFARVERSION ���������� ����� ������, � �� TRUE - ��� ����������
    ! ACTL_KEYMACRO, ACTL_EJECTMEDIA, ACTL_WAITKEY, ACTL_CONSOLEMODE -
      �������� Param �� NULL.
    ! FarGetMsgFn - �������� �� "����������" CtrlObject (�������� �� NULL)
  26.03.2001 SVS
    + ��������� ��������� ����� FHELP_USECONTENTS
  22.03.2001 tran 1.42
    ! ������ ��� � FarMessageFn/FMSG_ALLINONE
  21.03.2001 VVM
    + ��������� ����� EF_CREATENEW
  28.02.2001 IS
    ! "CtrlObject->CmdLine." -> "CtrlObject->CmdLine->"
  11.02.2001 SVS
    ! �������� ������������� ��� � FarDialogEx
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  28.01.2001 SVS
    ! ��������� ������� ������� FarMessageFn()
  23.01.2001 SVS
    ! �������� ���������� � FarDialogEx()
  21.01.2001 SVS
    + ACTL_PROCESSSEQUENCEKEY
  24.12.2000 SVS
    ! ��������� "MCMD_PLAYSTRING"
    ! ��������� MCMD_LOADALL � MCMD_SAVEALL - �� �������� �� ����� ������
      ��� "��������������� �������
  23.12.2000 SVS
    + MCMD_PLAYSTRING - "���������" ������ (������ � ��� ����, ��� � �������)
  21.12.2000 SVS
    + ACTL_KEYMACRO - ������� �������� KeyMacro API
  18.12.2000 SVS
    ! ��������� � FarShowHelp
  14.12.2000 SVS
    + ACTL_EJECTMEDIA
  13.12.2000 SVS
    ! FarDialogItem.Data - ����������� strcpy �������� �� memmove
      (�������� ������ ������������)
  04.12.2000 SVS
    + ACTL_GETCOLOR - �������� ������������ ����
    + ACTL_GETARRAYCOLOR - �������� ���� ������ ������
  17.11.2000 SVS
    ! "������� ������ �������" - ���� � ������� ShowHelp ��������� :-(
  11.11.2000 SVS
    ! FarMkTemp() - ������� (��� ������ - �� ������, �� ��� �� ������� :-(((
  11.11.2000 SVS
    ! ���������� ����������� FarMkTemp()
  03.11.2000 OT
    ! �������� �������� ������������� ��������
  02.11.2000 OT
    ! �������� �������� �� ����� ������, ����������� ��� ��� �����.
  05.10.2000 SVS
   - ���� � ������� ����� (FHELP_CUSTOMFILE)
  27.09.2000 SVS
   + FarViewerControl
  18.09.2000 SVS
    ! ������� FarDialogEx ����� 2 �������������� ��������� (Future)
  12.09.2000 SVS
    + ���������� ������ FHELP_* ��� ������ ������.
  08.09.2000 VVM
    + ��������� ������
      FCTL_SETSORTMODE, FCTL_SETANOTHERSORTMODE
      FCTL_SETSORTORDER, FCTL_SETANOTHERSORTORDER
  30.08.2000 SVS
    ! ��� ������� ������� ���� FMI_GETFARMSGID
  29.08.2000 SVS
    + ��� ������� �������� ����� �������, ���������� ���� ������. ������
      ��� ��� ����, ����� ��������� ������������� � Dialog API �� DN_HELP
  29.08.2000 SVS
    ! ���� PluginStartupInfo.GetMsg(?,N|FMI_GETFARMSGID), �� �������������, ���
      ����� ������������ "������" �� ������ far*.lng
  24.08.2000 SVS
    + ACTL_WAITKEY - ������� ������������ (��� �����) �������
  23.08.2000 SVS
    ! ��� Flags ��������� � ������ ���� -> DWORD.
      ��������������:
        * �������   FarMenuFn, FarMessageFn, FarShowHelp
        * ��������� FarListItem, FarDialogItem
  22.08.2000 SVS
    ! ��������� �������� ������ �� FarText.
  18.08.2000 tran 1.12
    + Flags parameter in FarShowHelp
  09.08.2000 tran 1.11
    ! ACTL_GETSYSWORDDIV ��� Param==NULL ������ ���������� ����� ������
  03.08.2000 SVS
    + ACTL_GETSYSWORDDIV �������� ������ � ��������� ������������� ����
  01.08.2000 SVS
    ! FARDIALOGPROC -> FARWINDOWPROC
  28.07.2000 SVS
    ! � ����� � ���������� SendDlgMessage � ������ Dialog
      ������ ��������� ���������!
  25.07.2000 SVS
    + ���������� ������������ FulScreen <-> Windowed (ACTL_CONSOLEMODE)
  23.07.2000 SVS
    + ������� FarDefDlgProc ��������� ������� �� ���������
    + ������� FarSendDlgMessage - ������� ��������� �������
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  12.07.2000 IS
    + �������� ������ ��������� � FarEditor (������ ��� ��������������) �
      �������� _������������_ ���������, ���� ���� ��������������� ����.
  11.07.2000 SVS
    ! ��������� ��� ����������� ���������� ��� BC & VC
  05.07.2000 IS
    + ������� FarAdvControl
    + ������� ACTL_GETFARVERSION (� FarAdvControl)
  03.07.2000 IS
    + ������� ������ ������
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "plugin.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "struct.hpp"
#include "keys.hpp"
#include "lang.hpp"
#include "help.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "cmdline.hpp"
#include "scantree.hpp"
#include "rdrwdsk.hpp"
#include "fileview.hpp"
#include "fileedit.hpp"
#include "plugins.hpp"
#include "savescr.hpp"
#include "manager.hpp"
#include "ctrlobj.hpp"
#include "frame.hpp"
#include "scrbuf.hpp"
#include "farexcpt.hpp"

// declare in plugins.cpp
extern int KeepUserScreen;
extern char DirToSet[NM];


void ScanPluginDir();

/* $ 12.09.2000 SVS
  + ���������� ������ ��� ������ ������.
*/
/* $ 18.08.2000 tran
   + Flags parameter */
/* $ 03.07.2000 IS
  ������� ������ ������
*/
BOOL WINAPI FarShowHelp(const char *ModuleName,
                        const char *HelpTopic,DWORD Flags)
{
  if (!HelpTopic)
    HelpTopic="Contents";

  DWORD OFlags=Flags;
  Flags&=~(FHELP_NOSHOWERROR|FHELP_USECONTENTS);
  char Path[2*NM],Topic[512];
  char *Mask=NULL;

  // ��������� � ������ ������ ���� �� ������������ � � ��� ������,
  // ���� ����� FHELP_FARHELP...
  if((Flags&FHELP_FARHELP) || *HelpTopic==':')
    strcpy(Topic,HelpTopic+((*HelpTopic == ':')?1:0));
  else
  {
    if(ModuleName)
    {
      // FHELP_SELFHELP=0 - ���������� ������ ���-� ��� Info.ModuleName
      //                   � �������� ����� �� ����� ���������� �������
      /* $ 17.11.2000 SVS
         � �������� FHELP_SELFHELP ����� ����? ��������� - 0
         � ����� ����� ��������� ����, ��� ������� �� �������� :-(
      */
      if(Flags == FHELP_SELFHELP || (Flags&(FHELP_CUSTOMFILE|FHELP_CUSTOMPATH)))
      {
        strcpy(Path,ModuleName);
        if(Flags == FHELP_SELFHELP || (Flags&(FHELP_CUSTOMFILE)))
        {
          Mask=PointToName(Path);
          if(Flags&FHELP_CUSTOMFILE)
          {
            memmove(Mask+1,Mask,strlen(Mask)+1);
            *Mask++=0;
          }
          else
          {
            *Mask=0;
            Mask=NULL;
          }
        }
      }
      else
        return FALSE;
      /* SVS $*/

      sprintf(Topic,HelpFormatLink,Path,HelpTopic);
    }
    else
      return FALSE;
  }
  {
    Help Hlp (Topic,Mask,OFlags);
    if(Hlp.GetError())
      return FALSE;
  }
  return TRUE;
}
/* IS $ */
/* tran 18.08.2000 $ */
/* SVS 12.09.2000 $ */


/* $ 05.07.2000 IS
  �������, ������� ����� ����������� � � ���������, � � �������, �...
*/
#ifndef _MSC_VER
#pragma warn -par
#endif
int WINAPI FarAdvControl(int ModuleNumber, int Command, void *Param)
{
 switch(Command)
 {
    case ACTL_GETFARVERSION:
      if(Param)
        *(DWORD*)Param=FAR_VERSION;
      return FAR_VERSION;
    /* $ 25.07.2000 SVS
       + ���������� ������������ FulScreen <-> Windowed (ACTL_CONSOLEMODE)
       mode = -2 - �������� ������� ���������
              -1 - ��� ������
               0 - Windowed
               1 - FulScreen
       Return
               0 - Windowed
               1 - FulScreen
    */
    case ACTL_CONSOLEMODE:
      return FarAltEnter(Param?*(int*)Param:-2);
    /* SVS $ */

    /* $ 03.08.2000 SVS
       ��������� ������ � ������������� ����
       ���������� ������ ���������� ������ ��� '\0'
       ������������ ������ ��������� ������ = 80 � �������������� '\0'
       ������ ���������� �� �� �������, � �� Opt.
    */
    case ACTL_GETSYSWORDDIV:
      /* $ 09.08.2000 tran
       + if param==NULL, plugin ����� ������ ������ ����� ������  */
      if ( Param )
          strcpy((char *)Param,Opt.WordDiv);
      /* tran 09.08.2000 $ */
      return strlen(Opt.WordDiv);
    /* SVS $ */

    /* $ 24.08.2000 SVS
       ������� ������������ (��� �����) �������
       (int)Param - ���������� ��� �������, ������� �������, ��� -1
       ���� ��� ����� ����� ������� �����.
       ���������� 0;
    */
    case ACTL_WAITKEY:
      WaitKey(Param?(int)Param:-1);
      return 0;
    /* SVS $ */

    /* $ 04.12.2000 SVS
      ACTL_GETCOLOR - �������� ������������ ���� �� ������, �������������
       � farcolor.hpp
      (int)Param - ������.
      Return - �������� ����� ��� -1 ���� ������ �������.
    */
    case ACTL_GETCOLOR:
      if((int)Param < SizeArrayPalette && (int)Param >= 0)
        return (int)((unsigned int)Palette[(int)Param]);
      return -1;
    /* SVS $ */

    /* $ 04.12.2000 SVS
      ACTL_GETARRAYCOLOR - �������� ���� ������ ������
      Param - ��������� �� ������ ��� NULL - ����� �������� ������ ������
      Return - ������ �������.
    */
    case ACTL_GETARRAYCOLOR:
      if(Param)
        memmove(Param,Palette,SizeArrayPalette);
      return SizeArrayPalette;
    /* SVS $ */

    /* $ 14.12.2000 SVS
      ACTL_EJECTMEDIA - ������� ���� �� �������� ����������
      Param - ��������� �� ��������� ActlEjectMedia
      Return - TRUE - �������� ����������, FALSE - ������.
    */
    case ACTL_EJECTMEDIA:
      return Param?EjectVolume((char)((ActlEjectMedia*)Param)->Letter,
                               ((ActlEjectMedia*)Param)->Flags):FALSE;

    /* $ 21.12.2000 SVS
       Macro API
    */
    case ACTL_KEYMACRO:
    {
      if(CtrlObject && Param) // ��� ������� �� ���� ������.
      {
        KeyMacro& Macro=CtrlObject->Macro; //??
        struct ActlKeyMacro *KeyMacro=(struct ActlKeyMacro*)Param;
        switch(KeyMacro->Command)
        {
          case MCMD_LOADALL: // �� ������� � ������ ��� � ���������� �����������
            if(Macro.IsRecording() || Macro.IsExecuting())
              return FALSE;
            return Macro.LoadMacros();

          case MCMD_SAVEALL: // �� ������ ���� � �������
            if(Macro.IsRecording() || Macro.IsExecuting())
              return FALSE;
            Macro.SaveMacros();
            return TRUE;
#if 0
          /* $ 23.12.2000 SVS
               MCMD_PLAYSTRING - "���������" ������ (������ � ��� ����,
               ��� � �������)
             Param - ��������� �� ��������� struct ActlKeyMacro.
          */
          case MCMD_PLAYSTRING:
            if(KeyMacro->Str.KeyBuffer && *KeyMacro->Str.KeyBuffer)
               return Macro.PlayKeyMacro(KeyMacro->Str.KeyBuffer);
            return FALSE;
#endif
        }
      }
      return FALSE;
    }

    case ACTL_POSTKEYSEQUENCE:
      if(CtrlObject && Param && ((struct KeySequence*)Param)->Count > 0)
      {
        struct MacroRecord MRec;
        MRec.Flags=(((struct KeySequence*)Param)->Flags)<<16;
        MRec.Key=0;
        MRec.BufferSize=((struct KeySequence*)Param)->Count;
        MRec.Buffer=((struct KeySequence*)Param)->Sequence;
        return CtrlObject->Macro.PostTempKeyMacro(&MRec);
#if 0
        // ���� ����� - ��� ���������� �������������
        {
          //CtrlObject->Macro.PostTempKeyMacro(&MRec);
          for(int I=0; I < MRec.BufferSize; ++I)
          {
            int Key=MRec.Buffer[I];
            if(CtrlObject->Macro.ProcessKey(Key))
            {
              while((Key=CtrlObject->Macro.GetKey()) != 0)
              {
                FrameManager->ProcessKey(Key);
              }
            }
            else
              FrameManager->ProcessKey(Key);
            FrameManager->PluginCommit();
          }
          return TRUE;
        }
#endif
      }
      return FALSE;

    /* $ 05.06.2001 tran
       ����� ACTL_ ��� ������ � �������� */
    case ACTL_GETWINDOWINFO:
        if(Param)
        {
            WindowInfo *wi=(WindowInfo*)Param;
            Frame *f=FrameManager->operator[](wi->Pos);
            if ( f==NULL )
                return FALSE;
            f->GetTypeAndName(wi->TypeName,wi->Name);
            wi->Type=f->GetType();
            wi->Modified=f->IsFileModified();
            wi->Current=f==FrameManager->GetCurrentFrame();
            return TRUE;
        }
        break;
    case ACTL_GETWINDOWCOUNT:
        {
            return FrameManager->GetFrameCount();
        }
    case ACTL_SETCURRENTWINDOW:
        {
            int pos=(int)Param;

            if ( FrameManager->operator[](pos)!=NULL )
            {
                FrameManager->ActivateFrame(pos);
                return TRUE;
            }
            return FALSE;
        }
    /* tran 05.06.2001 $ */
    /*$ 26.06.2001 SKV
      ��� ����������� ������ � ACTL_SETCURRENTWINDOW
      (� ����� ��� ��� ���� � �������)
    */
    case ACTL_COMMIT:
        if(FrameManager)
        {
            return FrameManager->PluginCommit();
        }
        return FALSE;
    /* SKV$*/
    /* $ 15.09.2001 tran
       ���������� �������� */
    case ACTL_GETFARHWND:
        return (int)hFarWnd;
    /* tran $ */
 }
 return FALSE;
}
#ifndef _MSC_VER
#pragma warn +par
#endif
/* IS $ */

int WINAPI FarMenuFn(int PluginNumber,int X,int Y,int MaxHeight,
           DWORD Flags,const char *Title,const char *Bottom,
           const char *HelpTopic, const int *BreakKeys,int *BreakCode,
           const struct FarMenuItem *Item, int ItemsNumber)
{
  int I;

  if (DisablePluginsOutput)
    return(-1);
  int ExitCode;
  {
    VMenu FarMenu(Title,NULL,0,MaxHeight);
    FarMenu.SetPosition(X,Y,0,0);
    if (BreakCode!=NULL)
      *BreakCode=-1;

    {
      char Topic[512];
      if(Help::MkTopic(PluginNumber,HelpTopic,Topic))
        FarMenu.SetHelp(Topic);
    }

    if (Bottom!=NULL)
      FarMenu.SetBottomTitle(Bottom);

    struct MenuItem CurItem;
    memset(&CurItem,0,sizeof(CurItem));

    if(Flags&FMENU_USEEXT)
    {
      struct FarMenuItemEx *ItemEx=(struct FarMenuItemEx*)Item;
      for (I=0;I<ItemsNumber;I++)
      {
        CurItem.Flags=ItemEx[I].Flags;
        strncpy(CurItem.Name,ItemEx[I].Text,sizeof(CurItem.Name)-1);
        FarMenu.AddItem(&CurItem);
      }
    }
    else
      for (I=0;I<ItemsNumber;I++)
      {
        CurItem.Flags=Item[I].Checked?(LIF_CHECKED|(Item[I].Checked&0xFFFF)):0;
        CurItem.Flags|=Item[I].Selected?LIF_SELECTED:0;
        CurItem.Flags|=Item[I].Separator?LIF_SEPARATOR:0;
        strncpy(CurItem.Name,Item[I].Text,sizeof(CurItem.Name)-1);
        FarMenu.AddItem(&CurItem);
      }

    DWORD MenuFlags=0;
    if (Flags & FMENU_SHOWAMPERSAND)
      MenuFlags|=VMENU_SHOWAMPERSAND;
    if (Flags & FMENU_WRAPMODE)
      MenuFlags|=VMENU_WRAPMODE;
    if (Flags & FMENU_AUTOHIGHLIGHT)
      FarMenu.AssignHighlights(FALSE);
    if (Flags & FMENU_REVERSEAUTOHIGHLIGHT)
      FarMenu.AssignHighlights(TRUE);
    FarMenu.SetFlags(MenuFlags);
    FarMenu.Show();
    while (!FarMenu.Done())
    {
      INPUT_RECORD ReadRec;
      int ReadKey=GetInputRecord(&ReadRec);
      if (ReadRec.EventType==MOUSE_EVENT)
        FarMenu.ProcessMouse(&ReadRec.Event.MouseEvent);
      else
        if (ReadKey!=KEY_NONE)
        {
          if (BreakKeys!=NULL)
            for (int I=0;BreakKeys[I]!=0;I++)
            {
              if(CtrlObject->Macro.IsExecuting())
              {
                int VirtKey,ControlState;
                TranslateKeyToVK(ReadKey,VirtKey,ControlState,&ReadRec);
              }
              if (ReadRec.Event.KeyEvent.wVirtualKeyCode==(BreakKeys[I] & 0xffff))
              {
                DWORD Flags=BreakKeys[I]>>16;
                /* $ 31.07.2001 IS
                   - ���: ���� ������� ����������� � ������ ������� ��
                     ctrl-key, alt-key ��� shift-key, ���� ���� ��� ����������
                     ����� �� ���� ������� � BreakKeys, � ������� ���� �����
                     ��������� ������� �� ������ key. �������: ��������� ����
                     ����� �� �������, �.�. ���������� ��� ������ �����.
                */
                DWORD RealFlags=ReadRec.Event.KeyEvent.dwControlKeyState &
                      (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED|
                      LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED|SHIFT_PRESSED);

                int Accept;
                if(RealFlags) // ������ shift, ctrl ��� alt
                {
                   Accept=FALSE; // �.�. ���� ������ �� ��������
                   if(Flags) // ������ ���� �������� � ������ ctrl|alt|shift
                   {
                     if ((Flags & PKF_CONTROL) &&
                         (RealFlags & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED)))
                       Accept=TRUE;
                     if ((Flags & PKF_ALT) &&
                         (RealFlags & (LEFT_ALT_PRESSED|RIGHT_ALT_PRESSED)))
                       Accept=TRUE;
                     if ((Flags & PKF_SHIFT) && (RealFlags & SHIFT_PRESSED))
                       Accept=TRUE;
                   }
                }
                else
                   Accept=!Flags;  // TRUE ������, ���� ��� �� ����� ���������
                                   // ������ � ctrl|alt|shift
                /* IS $ */
                if (Accept)
                {
                  if (BreakCode!=NULL)
                    *BreakCode=I;
                  FarMenu.Hide();
//                  CheckScreenLock();
                  return(FarMenu.GetSelectPos());
                }
              }
            }
          FarMenu.ProcessKey(ReadKey);
        }
    }
    ExitCode=FarMenu.Modal::GetExitCode();
  }
//  CheckScreenLock();
  return(ExitCode);
}

/* $ 23.07.2000 SVS
   ������� ��� ������������ �������
*/
// ������� FarDefDlgProc ��������� ������� �� ���������
long WINAPI FarDefDlgProc(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  if(hDlg)  // ��������� ������ ����� ��� hDlg=0
    return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
  return 0;
}

/* $ 28.07.2000 SVS
    ! � ����� � ���������� SendDlgMessage � ������ Dialog
      ������ ��������� ���������!
*/
// ������� ��������� �������
long WINAPI FarSendDlgMessage(HANDLE hDlg,int Msg,int Param1,long Param2)
{
  if(hDlg) // ��������� ������ ����� ��� hDlg=0
    return Dialog::SendDlgMessage(hDlg,Msg,Param1,Param2);
  return 0;
}
/* SVS $ */

int WINAPI FarDialogFn(int PluginNumber,int X1,int Y1,int X2,int Y2,
           const char *HelpTopic,struct FarDialogItem *Item,int ItemsNumber)
{
  return FarDialogEx(PluginNumber,X1,Y1,X2,Y2,HelpTopic,Item,ItemsNumber,0,0,NULL,0);
}

/* $   13.12.2000 SVS
   ! FarDialogItem.Data - ����������� strcpy �������� �� memmove
   (�������� ������ ������������)
*/
#ifndef _MSC_VER
#pragma warn -par
#endif
int WINAPI FarDialogEx(int PluginNumber,int X1,int Y1,int X2,int Y2,
           const char *HelpTopic,struct FarDialogItem *Item,int ItemsNumber,
           DWORD Reserved, DWORD Flags,
           FARWINDOWPROC DlgProc,long Param)

{
  if (DisablePluginsOutput || ItemsNumber <= 0 || !Item)
    return(-1);

  if(PluginNumber >= CtrlObject->Plugins.PluginsCount)
    return(-1); // � ���������.

  struct DialogItem *InternalItem=new DialogItem[ItemsNumber];

  if(!InternalItem)
    return -1;

  int ExitCode=-1;

  struct PluginItem *CurPlugin=&CtrlObject->Plugins.PluginsData[PluginNumber];

  memset(InternalItem,0,sizeof(DialogItem)*ItemsNumber);

  if(Opt.ExceptRules)
  {
    TRY {
      Dialog::ConvertItem(CVTITEM_FROMPLUGIN,Item,InternalItem,ItemsNumber);
    }
    EXCEPT (xfilter(EXCEPT_FARDIALOG,
                     GetExceptionInformation(),CurPlugin,0))
    {
      delete[] InternalItem;
      CtrlObject->Plugins.UnloadPlugin(*CurPlugin); // ���� �� �������, ���������
      return -1;
    }
  }
  else
    Dialog::ConvertItem(CVTITEM_FROMPLUGIN,Item,InternalItem,ItemsNumber);

  Frame *frame;
  if((frame=FrameManager->GetBottomFrame()) != NULL)
    frame->LockRefresh(); // ������� ���������� ������

  {
    Dialog FarDialog(InternalItem,ItemsNumber,DlgProc,Param);
    FarDialog.SetPosition(X1,Y1,X2,Y2);

    if(Flags & FDLG_WARNING)
      FarDialog.SetDialogMode(DMODE_WARNINGSTYLE);
    if(Flags & FDLG_SMALLDIALOG)
      FarDialog.SetDialogMode(DMODE_SMALLDIALOG);
    if(Flags & FDLG_NONMODAL)
      FarDialog.SetCanLoseFocus(TRUE);
    FarDialog.SetOwnsItems(TRUE);
    FarDialog.SetHelp(HelpTopic);

    /* IS $ */
    /* $ 29.08.2000 SVS
       �������� ����� ������� - ������ � �������� ��� ������������ HelpTopic
    */
    FarDialog.SetPluginNumber(PluginNumber);
    /* SVS $ */

    if(Opt.ExceptRules)
    {
      TRY
      {
        FarDialog.Process();
        Dialog::ConvertItem(CVTITEM_TOPLUGIN,Item,InternalItem,ItemsNumber);
        ExitCode=FarDialog.GetExitCode();
      }
      EXCEPT (xfilter(EXCEPT_FARDIALOG,
                       GetExceptionInformation(),CurPlugin,1))
      {
        ;
      }
    }
    else
    {
      FarDialog.Process();
      Dialog::ConvertItem(CVTITEM_TOPLUGIN,Item,InternalItem,ItemsNumber);
      ExitCode=FarDialog.GetExitCode();
    }
  }

  delete[] InternalItem;

  if((frame=FrameManager->GetBottomFrame()) != NULL)
    frame->UnlockRefresh(); // ������ ����� :-)
//  CheckScreenLock();
  FrameManager->RefreshFrame(); //??
  return(ExitCode);
}
#ifndef _MSC_VER
#pragma warn +par
#endif
/* SVS 13.12.2000 $ */
/* SVS $ */

const char* WINAPI FarGetMsgFn(int PluginNumber,int MsgId)
{
  return(CtrlObject?CtrlObject->Plugins.FarGetMsg(PluginNumber,MsgId):"");
}

char* PluginsSet::FarGetMsg(int PluginNumber,int MsgId)
{
  if (PluginNumber<PluginsCount)
  {
    struct PluginItem *CurPlugin=&PluginsData[PluginNumber];
    char Path[NM];
    strcpy(Path,CurPlugin->ModuleName);
    *PointToName(Path)=0;
    if (CurPlugin->Lang.Init(Path))
      return(CurPlugin->Lang.GetMsg(MsgId));
  }
  return("");
}

/* $ 28.01.2001 SVS
   ! ��������� ������� ������� FarMessageFn()
*/

#define MAXMSG  15
#ifdef __BORLANDC__
#pragma warn -aus
#endif
int WINAPI FarMessageFn(int PluginNumber,DWORD Flags,const char *HelpTopic,
                        const char * const *Items,int ItemsNumber,
                        int ButtonsNumber)
{
  if (DisablePluginsOutput)
    return(-1);

  if ((!(Flags&(FMSG_ALLINONE|FMSG_ERRORTYPE)) && ItemsNumber<2) || !Items)
    return(-1);

  const char *MsgItems[MAXMSG];
  char *SingleItems=NULL;
  int I;

  memset(MsgItems,0,sizeof(MsgItems));
  switch(Flags&0x000F0000)
  {
    case FMSG_MB_OK:
      ButtonsNumber=1;
      break;
    case FMSG_MB_YESNO:
    case FMSG_MB_RETRYCANCEL:
    case FMSG_MB_OKCANCEL:
      ButtonsNumber=2;
      break;
    case FMSG_MB_YESNOCANCEL:
    case FMSG_MB_ABORTRETRYIGNORE:
      ButtonsNumber=3;
      break;
  }

  if(Flags&FMSG_ALLINONE)
  {
    char *Msg;
    SingleItems=(char *)malloc(sizeof(char)*(strlen((char *)Items)+2));
    if(!SingleItems)
      return -1;
    MsgItems[ItemsNumber=0]=Msg=strcpy(SingleItems,(char *)Items);
    // ������ ���������� ����� � �������� �� ������
    while ((Msg = strchr(Msg, '\n')) != NULL)
    {
      *Msg='\0';
      if(ItemsNumber+1 == (sizeof(MsgItems)/sizeof(MsgItems[0])))
        break;

      if(*++Msg == '\0')
        break;

      MsgItems[++ItemsNumber]=Msg;
    }
    ItemsNumber++;

    if((Flags&0x000F0000) && ItemsNumber+ButtonsNumber >= MAXMSG)
      ItemsNumber=MAXMSG-ButtonsNumber;
    for(I=ItemsNumber; I < MAXMSG; ++I)
      MsgItems[I]=NULL;
  }
  else
  {
    if((Flags&0x000F0000) && ItemsNumber+ButtonsNumber >= sizeof(MsgItems)/sizeof(MsgItems[0]))
      ItemsNumber=sizeof(MsgItems)/sizeof(MsgItems[0])-ButtonsNumber-1;
    for (I=0;I<ItemsNumber;I++)
      MsgItems[I]=Items[I];
  }

  /* $ 22.03.2001 tran
     ItemsNumber++ -> ++ItemsNumber
     �������� ��������� ������� */
  switch(Flags&0x000F0000)
  {
    case FMSG_MB_OK:
      MsgItems[ItemsNumber++]=MSG(MOk);
      break;
    case FMSG_MB_OKCANCEL:
      MsgItems[ItemsNumber++]=MSG(MOk);
      MsgItems[ItemsNumber++]=MSG(MCancel);
      break;
    case FMSG_MB_ABORTRETRYIGNORE:
      MsgItems[ItemsNumber++]=MSG(MAbort);
      MsgItems[ItemsNumber++]=MSG(MRetry);
      MsgItems[ItemsNumber++]=MSG(MIgnore);
      break;
    case FMSG_MB_YESNO:
      MsgItems[ItemsNumber++]=MSG(MYes);
      MsgItems[ItemsNumber++]=MSG(MNo);
      break;
    case FMSG_MB_YESNOCANCEL:
      MsgItems[ItemsNumber++]=MSG(MYes);
      MsgItems[ItemsNumber++]=MSG(MNo);
      MsgItems[ItemsNumber++]=MSG(MCancel);
      break;
    case FMSG_MB_RETRYCANCEL:
      MsgItems[ItemsNumber++]=MSG(MRetry);
      MsgItems[ItemsNumber++]=MSG(MCancel);
      break;
  }
  /* tran $ */

  {
    char Topic[512];
    if(Help::MkTopic(PluginNumber,HelpTopic,Topic))
      SetMessageHelp(Topic);
  }
  /* $ 29.08.2000 SVS
     �������� ����� ������� - ������ � �������� ��� ������������ HelpTopic
  */
  Frame *frame;
  if((frame=FrameManager->GetBottomFrame()) != NULL)
    frame->LockRefresh(); // ������� ���������� ������
  int MsgCode=Message(Flags,ButtonsNumber,MsgItems[0],MsgItems[1],
              MsgItems[2],MsgItems[3],MsgItems[4],MsgItems[5],MsgItems[6],
              MsgItems[7],MsgItems[8],MsgItems[9],MsgItems[10],MsgItems[11],
              MsgItems[12],MsgItems[13],MsgItems[14],PluginNumber);
  if((frame=FrameManager->GetBottomFrame()) != NULL)
    frame->UnlockRefresh(); // ������ ����� :-)
  /* SVS $ */
//  CheckScreenLock();
  if(SingleItems)
    free(SingleItems);

  return(MsgCode);
}
/* SVS $ */
#ifdef __BORLANDC__
#pragma warn .aus
#endif


int WINAPI FarControl(HANDLE hPlugin,int Command,void *Param)
{
  if (CtrlObject->Cp()->LeftPanel==NULL || CtrlObject->Cp()->RightPanel==NULL)
    return(0);

  switch(Command)
  {
    case FCTL_CLOSEPLUGIN:
      strcpy(DirToSet,NullToEmpty((char *)Param));
    case FCTL_GETPANELINFO:
    case FCTL_GETANOTHERPANELINFO:
    case FCTL_UPDATEPANEL:
    case FCTL_UPDATEANOTHERPANEL:
    case FCTL_REDRAWPANEL:
    case FCTL_REDRAWANOTHERPANEL:
    case FCTL_SETPANELDIR:
    case FCTL_SETANOTHERPANELDIR:
    case FCTL_SETSELECTION:
    case FCTL_SETANOTHERSELECTION:
    case FCTL_SETVIEWMODE:
    case FCTL_SETANOTHERVIEWMODE:
/* $ VVM 08.09.2000
   + ����� ���������� �� �������
*/
    case FCTL_SETSORTMODE:
    case FCTL_SETANOTHERSORTMODE:
    case FCTL_SETSORTORDER:
    case FCTL_SETANOTHERSORTORDER:
/* VVM $ */
      {
        if (hPlugin==INVALID_HANDLE_VALUE)
        {
          CtrlObject->Cp()->ActivePanel->SetPluginCommand(Command,Param);
          return(TRUE);
        }
        HANDLE hInternal;
        Panel *LeftPanel=CtrlObject->Cp()->LeftPanel;
        Panel *RightPanel=CtrlObject->Cp()->RightPanel;
        int Processed=FALSE;
        if (LeftPanel!=NULL && LeftPanel->GetMode()==PLUGIN_PANEL)
        {
          hInternal=((struct PluginHandle *)LeftPanel->GetPluginHandle())->InternalHandle;
          if (hPlugin==hInternal)
          {
            LeftPanel->SetPluginCommand(Command,Param);
            Processed=TRUE;
          }
        }
        if (RightPanel!=NULL && RightPanel->GetMode()==PLUGIN_PANEL)
        {
          hInternal=((struct PluginHandle *)RightPanel->GetPluginHandle())->InternalHandle;
          if (hPlugin==hInternal)
          {
            RightPanel->SetPluginCommand(Command,Param);
            Processed=TRUE;
          }
        }
        return(Processed);
      }
    case FCTL_GETCMDLINE:
      CtrlObject->CmdLine->GetString((char *)Param,1024);
      return(TRUE);
    case FCTL_SETCMDLINE:
    case FCTL_INSERTCMDLINE:
      if (Command==FCTL_SETCMDLINE)
        CtrlObject->CmdLine->SetString((char *)Param);
      else
        CtrlObject->CmdLine->InsertString((char *)Param);
      CtrlObject->CmdLine->Redraw();
      return(TRUE);
    case FCTL_SETCMDLINEPOS:
      CtrlObject->CmdLine->SetCurPos(*(int *)Param);
      CtrlObject->CmdLine->Redraw();
      return(TRUE);
    case FCTL_GETCMDLINEPOS:
      *(int *)Param=CtrlObject->CmdLine->GetCurPos();
      return(TRUE);
    case FCTL_GETCMDLINESELECTION:
      CtrlObject->CmdLine->GetSelString((char *)Param,1024);
      return TRUE;
    case FCTL_SETCMDLINESELECTION:
    {
      CmdLineSelect *sel=(CmdLineSelect*)Param;
      CtrlObject->CmdLine->Select(sel->SelStart,sel->SelEnd);
      CtrlObject->CmdLine->Redraw();
      return TRUE;
    }
    case FCTL_SETUSERSCREEN:
      if (CtrlObject->Cp()->LeftPanel==NULL || CtrlObject->Cp()->RightPanel==NULL)
        return(FALSE);
      KeepUserScreen=TRUE;
      CtrlObject->Cp()->LeftPanel->ProcessingPluginCommand++;
      CtrlObject->Cp()->RightPanel->ProcessingPluginCommand++;
      ScrBuf.FillBuf();
      SaveScreen SaveScr;
      {
        RedrawDesktop Redraw;
        CtrlObject->CmdLine->Hide();
        SaveScr.RestoreArea(FALSE);
      }
      CtrlObject->Cp()->LeftPanel->ProcessingPluginCommand--;
      CtrlObject->Cp()->RightPanel->ProcessingPluginCommand--;
      return(TRUE);
  }
  return(FALSE);
}


HANDLE WINAPI FarSaveScreen(int X1,int Y1,int X2,int Y2)
{
  if (X2==-1)
    X2=ScrX;
  if (Y2==-1)
    Y2=ScrY;
  return((HANDLE)(new SaveScreen(X1,Y1,X2,Y2,FALSE)));
}


void WINAPI FarRestoreScreen(HANDLE hScreen)
{
  if (hScreen==NULL)
    ScrBuf.FillBuf();
  if (hScreen)
    delete (SaveScreen *)hScreen;
}


int WINAPI FarGetDirList(const char *Dir,struct PluginPanelItem **pPanelItem,
                  int *pItemsNumber)
{
  PluginPanelItem *ItemsList=NULL;
  int ItemsNumber=0;
  SaveScreen SaveScr;
  clock_t StartTime=clock();
  int MsgOut=0;

  *pItemsNumber=0;
  *pPanelItem=NULL;

  ScanTree ScTree(FALSE);
  WIN32_FIND_DATA FindData;
  char FullName[NM],DirName[NM];
//  ConvertNameToFull(Dir,DirName, sizeof(DirName));
  if (ConvertNameToFull(Dir,DirName, sizeof(DirName)) >= sizeof(DirName)){
    return FALSE;
  }
  ScTree.SetFindPath(DirName,"*.*");
  *PointToName(DirName)=0;
  int DirLength=strlen(DirName);
  while (ScTree.GetNextName(&FindData,FullName))
  {
    if ((ItemsNumber & 31)==0)
    {
      if (CheckForEsc())
      {
        /* $ 13.07.2000 SVS
           ������� ���� ����� realloc, ������ � free
        */
        if(ItemsList) free(ItemsList);
        /* SVS $ */
        return(FALSE);
      }
      if (!MsgOut && clock()-StartTime > 500)
      {
        SetCursorType(FALSE,0);
        Message(MSG_DOWN,0,"",MSG(MPreparingList));
        MsgOut=1;
      }
      ItemsList=(PluginPanelItem *)realloc(ItemsList,sizeof(*ItemsList)*(ItemsNumber+32+1));
      if (ItemsList==NULL)
      {
        *pItemsNumber=0;
        return(FALSE);
      }
    }
    memset(&ItemsList[ItemsNumber],0,sizeof(*ItemsList));
    ItemsList[ItemsNumber].FindData=FindData;
    strcpy(ItemsList[ItemsNumber].FindData.cFileName,FullName+DirLength);
    ItemsNumber++;
  }
  *pPanelItem=ItemsList;
  *pItemsNumber=ItemsNumber;
  return(TRUE);
}


static struct PluginPanelItem *PluginDirList;
static int DirListItemsNumber;
static char PluginSearchPath[NM*16];
static int StopSearch;
static HANDLE hDirListPlugin;
static int PluginSearchMsgOut;
static struct
{
  PluginPanelItem *Addr;
  int ItemsNumber;
} DirListNumbers[16];

int WINAPI FarGetPluginDirList(int PluginNumber,HANDLE hPlugin,
                  const char *Dir,struct PluginPanelItem **pPanelItem,
                  int *pItemsNumber)
{
  {
    if (strcmp(Dir,".")==0 || strcmp(Dir,"..")==0)
      return(FALSE);
    SaveScreen SaveScr;

    {
      char DirName[512];
      strncpy(DirName,Dir,sizeof(DirName)-1);
      TruncStr(DirName,30);
      CenterStr(DirName,DirName,30);
      SetCursorType(FALSE,0);
      Message(0,0,"",MSG(MPreparingList),DirName);
      PluginSearchMsgOut=FALSE;

      static struct PluginHandle DirListPlugin;

      // � �� ����� �� ������ ���������� �� ������� ������?
      if (hPlugin==INVALID_HANDLE_VALUE)
      {
        DirListPlugin=*(struct PluginHandle *)
                       CtrlObject->Cp()->ActivePanel->GetPluginHandle();
      }
      else
      {
        DirListPlugin.PluginNumber=PluginNumber;
        DirListPlugin.InternalHandle=hPlugin;
      }
      hDirListPlugin=(HANDLE)&DirListPlugin;
      StopSearch=FALSE;
      *pItemsNumber=DirListItemsNumber=0;
      *pPanelItem=PluginDirList=NULL;
      struct OpenPluginInfo Info;
      CtrlObject->Plugins.GetOpenPluginInfo(hDirListPlugin,&Info);
      char PrevDir[NM];
      strcpy(PrevDir,Info.CurDir);
      if (CtrlObject->Plugins.SetDirectory(hDirListPlugin,Dir,OPM_FIND))
      {
        strcpy(PluginSearchPath,Dir);
        strcat(PluginSearchPath,"\x1");
        ScanPluginDir();
        *pPanelItem=PluginDirList;
        *pItemsNumber=DirListItemsNumber;
        CtrlObject->Plugins.SetDirectory(hDirListPlugin,"..",OPM_FIND);
        PluginPanelItem *PanelData=NULL;
        int ItemCount=0;
        if (CtrlObject->Plugins.GetFindData(hDirListPlugin,&PanelData,&ItemCount,OPM_FIND))
          CtrlObject->Plugins.FreeFindData(hDirListPlugin,PanelData,ItemCount);
        struct OpenPluginInfo NewInfo;
        CtrlObject->Plugins.GetOpenPluginInfo(hDirListPlugin,&NewInfo);
        if (LocalStricmp(PrevDir,NewInfo.CurDir)!=0)
          CtrlObject->Plugins.SetDirectory(hDirListPlugin,PrevDir,OPM_FIND);
      }
    }
  }

  if (!StopSearch)
    for (int I=0;I<sizeof(DirListNumbers)/sizeof(DirListNumbers[0]);I++)
      if (DirListNumbers[I].Addr==NULL)
      {
        DirListNumbers[I].Addr=*pPanelItem;
        DirListNumbers[I].ItemsNumber=*pItemsNumber;
        break;
      }
  return(!StopSearch);
}


void ScanPluginDir()
{
  int I;
  PluginPanelItem *PanelData=NULL;
  int ItemCount=0;
  if (CheckForEsc())
    StopSearch=TRUE;

  char DirName[NM];
  strncpy(DirName,PluginSearchPath,sizeof(DirName)-1);
  DirName[sizeof(DirName)-1]=0;
  for (I=0;DirName[I]!=0;I++)
    if (DirName[I]=='\x1')
      DirName[I]=DirName[I+1]==0 ? 0:'\\';
  TruncStr(DirName,30);
  CenterStr(DirName,DirName,30);

  Message(MSG_KEEPBACKGROUND,0,"",MSG(MPreparingList),DirName);

  if (StopSearch || !CtrlObject->Plugins.GetFindData(hDirListPlugin,&PanelData,&ItemCount,OPM_FIND))
    return;
  struct PluginPanelItem *NewList=(struct PluginPanelItem *)realloc(PluginDirList,1+sizeof(*PluginDirList)*(DirListItemsNumber+ItemCount));
  if (NewList==NULL)
  {
    StopSearch=TRUE;
    return;
  }
  PluginDirList=NewList;
  for (I=0;I<ItemCount && !StopSearch;I++)
  {
    PluginPanelItem *CurPanelItem=PanelData+I;
    if ((CurPanelItem->FindData.dwFileAttributes & FA_DIREC)==0)
    {
      char FullName[2*NM+1];
      sprintf(FullName,"%.*s%.*s",NM,PluginSearchPath,NM,CurPanelItem->FindData.cFileName);
      for (int I=0;FullName[I]!=0;I++)
        if (FullName[I]=='\x1')
          FullName[I]='\\';
      PluginPanelItem *DestItem=PluginDirList+DirListItemsNumber;
      *DestItem=*CurPanelItem;
      if (CurPanelItem->UserData && (CurPanelItem->Flags & PPIF_USERDATA))
      {
        DWORD Size=*(DWORD *)CurPanelItem->UserData;
        /* $ 13.07.2000 SVS
           ������ new ����� ������������ malloc
        */
        DestItem->UserData=(DWORD)malloc(Size);
        /* SVS $*/
        memcpy((void *)DestItem->UserData,(void *)CurPanelItem->UserData,Size);
      }

      strncpy(DestItem->FindData.cFileName,FullName,sizeof(DestItem->FindData.cFileName)-1);
      DirListItemsNumber++;
    }
  }
  for (I=0;I<ItemCount && !StopSearch;I++)
  {
    PluginPanelItem *CurPanelItem=PanelData+I;
    if ((CurPanelItem->FindData.dwFileAttributes & FA_DIREC) &&
        strcmp(CurPanelItem->FindData.cFileName,".")!=0 &&
        strcmp(CurPanelItem->FindData.cFileName,"..")!=0)

    {
      struct PluginPanelItem *NewList=(struct PluginPanelItem *)realloc(PluginDirList,sizeof(*PluginDirList)*(DirListItemsNumber+1));
      if (NewList==NULL)
      {
        StopSearch=TRUE;
        return;
      }
      PluginDirList=NewList;
      char FullName[2*NM+1];
      sprintf(FullName,"%.*s%.*s",NM,PluginSearchPath,NM,CurPanelItem->FindData.cFileName);
      for (int I=0;FullName[I]!=0;I++)
        if (FullName[I]=='\x1')
          FullName[I]='\\';
      PluginDirList[DirListItemsNumber]=*CurPanelItem;
      strncpy(PluginDirList[DirListItemsNumber].FindData.cFileName,FullName,sizeof(PluginDirList[0].FindData.cFileName)-1);
      DirListItemsNumber++;
      if (CtrlObject->Plugins.SetDirectory(hDirListPlugin,CurPanelItem->FindData.cFileName,OPM_FIND))
      {
        strcat(PluginSearchPath,CurPanelItem->FindData.cFileName);
        strcat(PluginSearchPath,"\x1");
        if (strlen(PluginSearchPath)<sizeof(PluginSearchPath)-NM)
          ScanPluginDir();
        *strrchr(PluginSearchPath,'\x1')=0;
        char *NamePtr=strrchr(PluginSearchPath,'\x1');
        if (NamePtr!=NULL)
          *(NamePtr+1)=0;
        else
          *PluginSearchPath=0;
        if (!CtrlObject->Plugins.SetDirectory(hDirListPlugin,"..",OPM_FIND))
        {
          StopSearch=TRUE;
          break;
        }
      }
    }
  }
  CtrlObject->Plugins.FreeFindData(hDirListPlugin,PanelData,ItemCount);
}


void WINAPI FarFreeDirList(const struct PluginPanelItem *PanelItem)
{
  if (PanelItem==NULL)
    return;
  int ItemsNumber=0;
  int I;
  for (I=0;I<sizeof(DirListNumbers)/sizeof(DirListNumbers[0]);I++)
    if (DirListNumbers[I].Addr==PanelItem)
    {
      DirListNumbers[I].Addr=NULL;
      ItemsNumber=DirListNumbers[I].ItemsNumber;
      break;
    }

  for (I=0;I<ItemsNumber;I++)
  {
    const PluginPanelItem *CurPanelItem=PanelItem+I;
    if (CurPanelItem->UserData && (CurPanelItem->Flags & PPIF_USERDATA))
      /* $ 13.07.2000 SVS
        ��� ������� ������������ malloc
      */
      free((void *)CurPanelItem->UserData);
      /* SVS $*/
  }
  /* $ 13.07.2000 SVS
    ��� ������� ������������ realloc
  */
  free(static_cast<void*>(const_cast<PluginPanelItem *>(PanelItem)));
  /* SVS $*/
}


#if defined(__BORLANDC__)
#pragma warn -par
#endif
int WINAPI FarViewer(const char *FileName,const char *Title,
                     int X1,int Y1,int X2, int Y2,DWORD Flags)
{
  if (X2==-1)
    X2=ScrX;
  if (Y2==-1)
    Y2=ScrY;
  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));
  /* $ 09.09.2001 IS */
  int DisableHistory=(Flags & VF_DISABLEHISTORY)?TRUE:FALSE;
  /* IS $ */
  if (Flags & VF_NONMODAL)
  {
    /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
    FileViewer *Viewer=new FileViewer(FileName,TRUE,DisableHistory,Title,X1,Y1,X2,Y2);
    /* IS $ */
    if(!Viewer)
      return FALSE;
    if (Flags & VF_DELETEONCLOSE)
      Viewer->SetTempViewName(FileName);
    /* $ 12.05.2001 DJ */
    Viewer->SetEnableF6 ((Flags & VF_ENABLE_F6) != 0);
    /* DJ $ */
    FrameManager->ExecuteNonModal();
  }
  else
  {
    /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
    FileViewer Viewer (FileName,FALSE,DisableHistory,Title,X1,Y1,X2,Y2);
    /* IS $ */
    /* $ 28.05.2001 �� ��������� �����, ������� ����� ����� ������� ��������� ���� */
    Viewer.SetDynamicallyBorn(false);
    FrameManager->ExecuteModal();
    if (Flags & VF_DELETEONCLOSE)
      Viewer.SetTempViewName(FileName);
    /* $ 12.05.2001 DJ */
    Viewer.SetEnableF6 ((Flags & VF_ENABLE_F6) != 0);
    /* DJ $ */
    SetConsoleTitle(OldTitle);
    if (!Viewer.GetExitCode()){
      return FALSE;
    }
  }
  return(TRUE);
}


int WINAPI FarEditor(const char *FileName,const char *Title,
                     int X1,int Y1,int X2,
                     int Y2,DWORD Flags,int StartLine,int StartChar)
{
  if (X2==-1)
    X2=ScrX;
  if (Y2==-1)
    Y2=ScrY;
  char OldTitle[512];
  GetConsoleTitle(OldTitle,sizeof(OldTitle));
  /* $ 12.07.2000 IS
   �������� ������ ��������� (������ ��� ��������������) � ��������
   ������������ ���������, ���� ���� ��������������� ����
  */
  int ExitCode;
  /* $ 21.03.2001 VVM
    + ��������� ����� EF_CREATENEW */
  int CreateNew = (Flags & EF_CREATENEW)?TRUE:FALSE;
  /* VVM $ */
  /* $ 09.09.2001 IS */
  int DisableHistory=(Flags & EF_DISABLEHISTORY)?TRUE:FALSE;
  int DeleteOnClose=(Flags & EF_DELETEONCLOSE)?TRUE:FALSE;
  /* IS $ */
  if (Flags & EF_NONMODAL)
  {
    ExitCode=FALSE;
    /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
    FileEditor *Editor=new FileEditor(FileName,CreateNew,TRUE,StartLine,StartChar,Title,X1,Y1,X2,Y2,DisableHistory,DeleteOnClose);
    /* IS $ */
    if (Editor)
    {
      /* $ 12.05.2001 DJ */
      Editor->SetEnableF6 ((Flags & EF_ENABLE_F6) != 0);
      /* DJ $ */
      FrameManager->ExecuteNonModal();
      ExitCode=TRUE;
    }
  }
  else
  {
   /* 09.09.2001 IS ! ������� ��� ����� � �������, ���� ����������� */
   FileEditor Editor(FileName,CreateNew,FALSE,StartLine,StartChar,Title,X1,Y1,X2,Y2,DisableHistory,DeleteOnClose);
   /* IS $ */
   Editor.SetDynamicallyBorn(false);
   /* $ 12.05.2001 DJ */
   Editor.SetEnableF6 ((Flags & EF_ENABLE_F6) != 0);
   /* DJ $ */
   SetConsoleTitle(OldTitle);
   FrameManager->ExecuteModal();
   ExitCode = Editor.GetExitCode();
   if (ExitCode && ExitCode != XC_LOADING_INTERRUPTED){
     if (Editor.IsFileChanged()){
       ExitCode = XC_MODIFIED;
     } else {
       ExitCode = XC_NOT_MODIFIED;
     }
   }
  }
  return ExitCode;
  /* IS $ */
}
#if defined(__BORLANDC__)
#pragma warn +par
#endif


int WINAPI FarCmpName(const char *pattern,const char *string,int skippath)
{
  return(CmpName(pattern,string,skippath));
}


int WINAPI FarCharTable(int Command,char *Buffer,int BufferSize)
{
  if (Command==FCT_DETECT)
  {
    char DataFileName[NM];
    FILE *DataFile;
    strcpy(DataFileName,Opt.TempPath);
    strcat(DataFileName,FarTmpXXXXXX);
    /* $ 19.06.2001
       - ���: �� �������� ���������������.
         ��, ����, ����� �� �� return -1 ������������� � 268??
    */
    if (mktemp(DataFileName)==NULL || (DataFile=fopen(DataFileName,"w+b"))==NULL)
      return(-1);
    /* IS $ */
    fwrite(Buffer,1,BufferSize,DataFile);
    fseek(DataFile,0,SEEK_SET);
    CharTableSet TableSet;
    int TableNum;
    int DetectCode=DetectTable(DataFile,&TableSet,TableNum);
    fclose(DataFile);
    remove(DataFileName);
    return(DetectCode ? TableNum-1:-1);
  }
  if (BufferSize!=sizeof(CharTableSet))
    return(-1);
  /* $ 07.08.2001 IS
       ��� ������� �������� ��������� ������� ��� OEM
  */
  CharTableSet *CTS=reinterpret_cast<CharTableSet*>(Buffer);
  if (!PrepareTable(CTS,Command))
  {
    for(unsigned int i=0;i<256;++i)
    {
      CTS->EncodeTable[i]=CTS->DecodeTable[i]=i;
      CTS->UpperTable[i]=LocalUpper(i);
      CTS->LowerTable[i]=LocalLower(i);
    }
    strcpy(CTS->TableName,MSG(MGetTableNormalText));
    return(-1);
  }
  /* IS $ */
  return(Command);
}


void WINAPI FarText(int X,int Y,int Color,const char *Str)
{
  if (Str==NULL)
  {
    int PrevLockCount=ScrBuf.GetLockCount();
    ScrBuf.SetLockCount(0);
    ScrBuf.Flush();
    ScrBuf.SetLockCount(PrevLockCount);
  }
  else
  {
    /* $ 22.08.2000 SVS
       ��������� �������� ������ �� FarText.
    */
    Text(X,Y,Color,Str);
    /* SVS $ */
  }
}


int WINAPI FarEditorControl(int Command,void *Param)
{
  if (CtrlObject->Plugins.CurEditor==NULL)
    return(0);
  return(CtrlObject->Plugins.CurEditor->EditorControl(Command,Param));
}

/* $ 27.09.2000 SVS
  ���������� ��������
*/
int WINAPI FarViewerControl(int Command,void *Param)
{
  if (CtrlObject->Plugins.CurViewer==NULL)
    return(0);
  return(CtrlObject->Plugins.CurViewer->ViewerControl(Command,Param));
}
/* SVS $ */
