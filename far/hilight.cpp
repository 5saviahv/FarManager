/*
hilight.cpp

Files highlighting

*/

/* Revision: 1.31 05.09.2001 $ */

/*
Modify:
  05.08.2001 SVS
    ! ������� ����������� � ������ �������� ����� HighlightDataColor
    + ������� ReWriteWorkColor - ����� �� ������� (����� ���� 0x00 ��������
      "����� �� ����" :-), �.�. �� ����� �������� ���� �� ���������)
    ! SetHighlighting() ��������� �� main.cpp
  26.07.2001 SVS
    ! VFMenu ��������� ��� �����
  22.07.2001 SVS
    ! ����������� �� ���������
  18.07.2001 OT
    VFMenu
  13.07.2001 SVS
    + Ctrl-Up/Ctrl-Down � ������ - �������� ����� � ���� ������.
  12.07.2001 SVS
    + F5 - ����������� ������� ������
    + ������� ������������ - DupHighlightData()
  06.07.2001 IS
    + ������ � ��������� ������ ����� ������������ ����� ����������, �����
      ������ ����� ����� � �������, ����� ������ ����������� �� ������������,
      ����� ������������ ����� � ������� � �������� ����������� �����.
  04.06.2001 SVS
    ! ��������� ���������� DN_BTNCLICK
  21.05.2001 SVS
    ! struct MenuData|MenuItem
      ���� Selected, Checked, Separator � Disabled ������������� � DWORD Flags
    ! ��������� MENU_ - � ����
  18.05.2001 DJ
    ! HighlightFiles::EditRecord() ��������� � �������������� �������-
      ����������� �������
  07.05.2001 DJ
    ! �����������
  06.05.2001 DJ
    ! �������� #include
  29.04.2001 ��
    + ��������� NWZ �� ����������
  23.04.2001 SVS
    ! ���! ����� ��<��� �� %PATHEXT% - �� ��� ����������� � ��, ���
      ����� - ������ ��������.
  08.04.2001 SVS
    ! ��������� �� ������������ ���������� �����. � ����! ������ ��������
      �� ����� ����.
  06.04.2001 SVS
    ! ��� �� ������� PATHEXT ������� � ��������� ������� ExpandPATHEXT()
  04.04.2001 SVS
    - �� ����������������� ������ Mask � HighlightFiles::EditRecord()
      ������� ������ ���������� ����� � ������ ����� ��� �����.
    ! ������� ������������ ���� � HighlightFiles::HiEdit()
  03.04.2001 SVS
    ! ��-�� �������� ������� ���������� %pathext% - ������� ��������� ����.
    + ����� ������� ������� � ����
  02.04.2001 VVM
    + � ������ ����� �������� ���������� ���������
  01.03.2001 SVS
    ! ������� ����� "Highlight" � "Colors\Highlight"
  27.02.2001 VVM
    ! �������, ��������� �� ������� ��������
      /[\x01-\x08\x0B-\x0C\x0E-\x1F\xB0-\xDF\xF8-\xFF]/
      ���������� � ����.
  26.02.2001 SVS
    - ����� ��� �������������� ���������������� ������...
  12.02.2001 SVS
    + ������� ClearData - ������� HiData
    - ���������� ������ ������ (����� 440-��)
  11.02.2001 SVS
    ! �������� DIF_VAREDIT ��������� ��������� ������ ��� ����� ��
      HIGHLIGHT_MASK_SIZE ��������
  11.02.2001 SVS
    ! ��������� ��������� ���� � ����� � ����������� � ��������� MenuItem
  14.01.2001 SVS
    + ����� ��� reparse point
  24.11.2000 SVS
    - ��� Encrypted ������ EditData.ExcludeAttr ����� IncludeAttr :-(
  30.10.2000 SVS
    - �� ������������� ����� ������ � Files Highlighting!
  20.10.2000 SVS
    ! �������� ������� Enctripted � ������� ������ ����������������
      ��� Include & Exclude ���������.
  13.07.2000 SVS
    ! ��������� ��������� ��� ������������� new/delete/realloc
  07.07.2000 IS
    + ���� ������ ctrl+r � ����, �� ������������ �������� �� ���������.
  25.06.2000 SVS
    ! ���������� Master Copy
    ! ��������� � �������� ���������������� ������
*/

#include "headers.hpp"
#pragma hdrstop

#include "colors.hpp"
#include "struct.hpp"
#include "hilight.hpp"
#include "fn.hpp"
#include "global.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "vmenu.hpp"
#include "dialog.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"

#define HIGHLIGHT_MASK_SIZE      2048

static struct HighlightDataColor WorkColor;

HighlightFiles::HighlightFiles()
{
  HiData=NULL;
  /* $ 07.07.2000 IS
    ���� ��� ������� � ��������� �������, ����� ����� ���� ��� ������������
    ��������
  */
  InitHighlightFiles();
  /* IS $ */
}

void HighlightFiles::InitHighlightFiles()
{
  if(HiData) free(HiData); HiData=NULL;
  HiDataCount=0;
  char RegKey[80],Mask[HIGHLIGHT_MASK_SIZE];
  char *Ptr=MkRegKeyHighlightName(RegKey); // Ptr ��������� �� ������ ����� :-)
  ReWriteWorkColor();
  while (1)
  {
    itoa(HiDataCount,Ptr,10);
    if (!GetRegKey(RegKey,"Mask",Mask,"",sizeof(Mask)))
      break;
    struct HighlightData *NewHiData;
    struct HighlightData HData={0};
    if(AddMask(&HData,Mask))
    {
      if ((NewHiData=(struct HighlightData *)realloc(HiData,sizeof(*HiData)*(HiDataCount+1)))==NULL)
      {
        DeleteMask(&HData);
        break;
      }
      HiData=NewHiData;
      HData.IncludeAttr=GetRegKey(RegKey,"IncludeAttributes",0);
      HData.ExcludeAttr=GetRegKey(RegKey,"ExcludeAttributes",0)&(~HData.IncludeAttr);
      HData.Colors.Color=(BYTE)GetRegKey(RegKey,"NormalColor",(DWORD)WorkColor.Color);
      HData.Colors.SelColor=(BYTE)GetRegKey(RegKey,"SelectedColor",(DWORD)WorkColor.SelColor);
      HData.Colors.CursorColor=(BYTE)GetRegKey(RegKey,"CursorColor",(DWORD)WorkColor.CursorColor);
      HData.Colors.CursorSelColor=(BYTE)GetRegKey(RegKey,"SelectedCursorColor",(DWORD)WorkColor.CursorSelColor);
      HData.Colors.MarkChar=(BYTE)GetRegKey(RegKey,"MarkChar",0);
      memcpy(HiData+HiDataCount,&HData,sizeof(struct HighlightData));
      HiDataCount++;
    }
    else
      break;
  }
  StartHiDataCount=HiDataCount;
}


HighlightFiles::~HighlightFiles()
{
  ClearData();
}

/* $ 06.07.2001 IS "�������" ����� ������ � ��� ��� */
// ������� �������� - �����, ������� ������������� � ������������ �� ������
char *HighlightFiles::GetMask(int Idx)
{
  return (HiData+Idx)->OriginalMasks;
}
/* IS $ */

/* $ 01.05.2001 DJ
   ���������������� ������ �������� � Masks
*/
/* $ 06.07.2001 IS
   ������ "�������" ����� ���������� ��������������� �����
*/
BOOL HighlightFiles::AddMask(struct HighlightData *Dest,char *Mask,struct HighlightData *Src)
{
  char *Ptr, *OPtr;
  /* ��������� %PATHEXT% */
  // ������ ��� �������� - OriginalMasks
  if((OPtr=(char *)realloc(Dest->OriginalMasks,strlen(Mask)+1)) == NULL)
    return FALSE;
  strcpy(OPtr,Mask); // ��������� ��������.
  // ��������
  if((Ptr=strchr(Mask,'%')) != NULL && !strnicmp(Ptr,"%PATHEXT%",9))
  {
    int IQ1=(*(Ptr+9) == ',')?10:9, offsetPtr=Ptr-Mask;
    // ���� ����������� %pathext%, �� ������� � �����...
    memmove(Ptr,Ptr+IQ1,strlen(Ptr+IQ1)+1);

    char Tmp1[HIGHLIGHT_MASK_SIZE], *pSeparator;
    strcpy(Tmp1, Mask);
    pSeparator=strchr(Tmp1, EXCLUDEMASKSEPARATOR);
    if(pSeparator)
    {
      Ptr=Tmp1+offsetPtr;
      if(Ptr>pSeparator) // PATHEXT ��������� � ������ ����������
        Add_PATHEXT(Mask); // ��������� ��, ���� ����.
      else
      {
        char Tmp2[HIGHLIGHT_MASK_SIZE];
        strcpy(Tmp2, pSeparator+1);
        *pSeparator=0;
        Add_PATHEXT(Tmp1);
        sprintf(Mask, "%s|%s", Tmp1, Tmp2);
      }
    }
    else
      Add_PATHEXT(Mask); // ��������� ��, ���� ����.
  }
  // ������ ��� ������� �����
  CFileMask *FMasks;
  if((FMasks=new CFileMask) == NULL)
  {
    free(OPtr);
    return FALSE;
  }

  if(!FMasks->Set(Mask, FMF_SILENT)) // �������� ������������ �����
  {
    delete FMasks;
    free(OPtr);
    return FALSE;
  }

  if(Src)
    memmove(Dest,Src,sizeof(struct HighlightData));

  // ����������� ������ �� �����.
  Dest->FMasks=FMasks;
  Dest->OriginalMasks=OPtr;
  return TRUE;
}
/* IS $ */
/* DJ $ */

/* $ 06.07.2001 IS ������ "�������" ����� ���������� ��������������� ����� */
void HighlightFiles::DeleteMask(struct HighlightData *CurHighlightData)
{
  if(CurHighlightData->FMasks)
  {
    delete CurHighlightData->FMasks;
    CurHighlightData->FMasks=NULL;
  }
  if(CurHighlightData->OriginalMasks)
  {
    free(CurHighlightData->OriginalMasks);
    CurHighlightData->OriginalMasks=NULL;
  }
}
/* IS $ */

void HighlightFiles::ClearData()
{
  if(HiData)
  {
    for(int I=0; I < HiDataCount; ++I)
      DeleteMask(HiData+I);
    free(HiData);
  }
  HiData=NULL;
  HiDataCount=0;
}

/* $ 01.05.2001 DJ
   ���������������� ������ �������� Masks
*/
/* $ 06.07.2001 IS ������ "�������" ����� ���������� ��������������� ����� */
void HighlightFiles::GetHiColor(char *Path,int Attr,
                                struct HighlightDataColor *Colors)
{
  struct HighlightData *CurHiData=HiData;
  int I;
  ReWriteWorkColor(Colors);
  Path=Path?Path:""; // ���� Path==NULL, �� �������, ��� ��� ������ ������

  for (I=0; I < HiDataCount;I++, ++CurHiData)
  {
    if ((Attr & CurHiData->IncludeAttr)==CurHiData->IncludeAttr &&
        (Attr & CurHiData->ExcludeAttr)==0)
    {
      if(CurHiData->FMasks->Compare(Path))
      {
        memcpy(Colors,&CurHiData->Colors,sizeof(struct HighlightDataColor));
        return;
      }
    }
  }
}
/* IS $ */
/* DJ $ */

void HighlightFiles::ReWriteWorkColor(struct HighlightDataColor *Colors)
{
#if 0
  WorkColor.Color=FarColorToReal(COL_PANELTEXT);
  WorkColor.SelColor=FarColorToReal(COL_PANELSELECTEDTEXT);
  WorkColor.CursorColor=FarColorToReal(COL_PANELCURSOR);
  WorkColor.CursorSelColor=FarColorToReal(COL_PANELSELECTEDCURSOR);
  WorkColor.MarkChar=0;
  if(Colors)
    *Colors=WorkColor;
#else
  if(Colors)
    memset(Colors,0,sizeof(struct HighlightDataColor));
#endif
}

void HighlightFiles::FillMenu(VMenu *HiMenu,int MenuPos)
{
  struct MenuItem HiMenuItem;
  unsigned char VerticalLine=0x0B3;

  HiMenu->DeleteItems();
  memset(&HiMenuItem,0,sizeof(HiMenuItem));

  for (int I=0;I<HiDataCount;I++)
  {
    struct HighlightData *CurHiData=&HiData[I];
    sprintf(HiMenuItem.Name,"%c %c %c%c%c%c%c%c%c %c %c%c%c%c%c%c%c %c %.60s",
      (CurHiData->Colors.MarkChar?CurHiData->Colors.MarkChar:' '), // ������� ����� �������

      VerticalLine,

      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_READONLY) ? 'R':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':
        ((CurHiData->IncludeAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':'.'),
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_DIRECTORY) ? 'F':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_REPARSE_POINT) ? 'L':'.',

      VerticalLine,

      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_READONLY) ? 'R':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':
        ((CurHiData->ExcludeAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':'.'),
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_DIRECTORY) ? 'F':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_REPARSE_POINT) ? 'L':'.',

      VerticalLine,

      GetMask(I));
    HiMenuItem.SetSelect(I==MenuPos);
    HiMenu->AddItem(&HiMenuItem);
  }
  *HiMenuItem.Name=0;
  HiMenuItem.SetSelect(HiDataCount==MenuPos);
  HiMenu->AddItem(&HiMenuItem);
}

void HighlightFiles::HiEdit(int MenuPos)
{
  VMenu HiMenu(MSG(MHighlightTitle),NULL,0,ScrY-4);
  HiMenu.SetHelp("HighlightList");
  HiMenu.SetFlags(VMENU_WRAPMODE|VMENU_SHOWAMPERSAND);
  HiMenu.SetPosition(-1,-1,0,0);
  HiMenu.SetBottomTitle(MSG(MHighlightBottom));

  FillMenu(&HiMenu,MenuPos);

  int NeedUpdate;
  Panel *LeftPanel=CtrlObject->Cp()->LeftPanel;
  Panel *RightPanel=CtrlObject->Cp()->RightPanel;

  HiMenu.Show();
  while (1)
  {
    while (!HiMenu.Done())
    {
      int SelectPos=HiMenu.GetSelectPos();
      int ItemCount=HiMenu.GetItemCount();
      int Key;

      NeedUpdate=FALSE;
      Key=HiMenu.ReadInput();
      switch(Key)
      {
        /* $ 07.07.2000 IS
          ���� ������ ctrl+r, �� ������������ �������� �� ���������.
        */
        case KEY_CTRLR:
          if (Message(MSG_WARNING,2,MSG(MHighlightTitle),
                        MSG(MHighlightWarning),MSG(MHighlightAskRestore),
                        MSG(MYes),MSG(MCancel))!=0)
             break;
          DeleteKeyTree(RegColorsHighlight);
          SetHighlighting();
          HiMenu.Hide();
          ClearData();
          InitHighlightFiles();
          NeedUpdate=TRUE;
          break;
        /* IS $ */
        case KEY_DEL:
          if (SelectPos<HiMenu.GetItemCount()-1)
          {
            if (Message(MSG_WARNING,2,MSG(MHighlightTitle),
                        MSG(MHighlightAskDel),GetMask(SelectPos),
                        MSG(MDelete),MSG(MCancel))!=0)
              break;
            DeleteMask(HiData+SelectPos);
            for (int I=SelectPos+1;I<ItemCount;I++)
              HiData[I-1]=HiData[I];
            HiDataCount--;
            HiData=(struct HighlightData *)realloc(HiData,sizeof(*HiData)*(HiDataCount+1));
            NeedUpdate=TRUE;
          }
          break;
        case KEY_ENTER:
        case KEY_F4:
          if (SelectPos>=HiMenu.GetItemCount()-1)
            break;
        case KEY_INS:
          if (EditRecord(SelectPos,Key == KEY_INS))
            NeedUpdate=TRUE;
          break;
        case KEY_F5:
          if (SelectPos < HiMenu.GetItemCount()-1)
          {
            if(DupHighlightData(HiData+SelectPos,GetMask(SelectPos),SelectPos))
              NeedUpdate=TRUE;
          }
          break;
        case KEY_CTRLUP:
          if (SelectPos > 0 && SelectPos < HiMenu.GetItemCount()-1)
          {
            struct HighlightData HData;
            memcpy(&HData,HiData+SelectPos,sizeof(struct HighlightData));
            memcpy(HiData+SelectPos,HiData+SelectPos-1,sizeof(struct HighlightData));
            memcpy(HiData+SelectPos-1,&HData,sizeof(struct HighlightData));
            HiMenu.SetSelection(--SelectPos);
            NeedUpdate=TRUE;
            break;
          }
        case KEY_CTRLDOWN:
          if (SelectPos < HiMenu.GetItemCount()-2)
          {
            struct HighlightData HData;
            memcpy(&HData,HiData+SelectPos,sizeof(struct HighlightData));
            memcpy(HiData+SelectPos,HiData+SelectPos+1,sizeof(struct HighlightData));
            memcpy(HiData+SelectPos+1,&HData,sizeof(struct HighlightData));
            HiMenu.SetSelection(++SelectPos);
            NeedUpdate=TRUE;
          }

        default:
          HiMenu.ProcessInput();
          break;
      }
      // ������������� �����!
      if(NeedUpdate)
      {
         ScrBuf.Lock(); // �������� ������ ����������
         HiMenu.Hide();
         SaveHiData();
         //FrameManager->RefreshFrame(); // ��������

         LeftPanel->Update(UPDATE_KEEP_SELECTION);
         LeftPanel->Redraw();
         RightPanel->Update(UPDATE_KEEP_SELECTION);
         RightPanel->Redraw();

         FillMenu(&HiMenu,MenuPos=SelectPos);
         HiMenu.Show();
         ScrBuf.Unlock(); // ��������� ����������
      }
    }
    if (HiMenu.Modal::GetExitCode()!=-1)
    {
      HiMenu.ClearDone();
      HiMenu.WriteInput(KEY_F4);
      continue;
    }
    break;
  }
}


void HighlightFiles::SaveHiData()
{
  int I;
  char RegKey[80];
  char *Ptr=MkRegKeyHighlightName(RegKey);
  for (I=0;I<HiDataCount;I++)
  {
    struct HighlightData *CurHiData=&HiData[I];
    itoa(I,Ptr,10);
    SetRegKey(RegKey,"Mask",GetMask(I));
    SetRegKey(RegKey,"IncludeAttributes",CurHiData->IncludeAttr);
    SetRegKey(RegKey,"ExcludeAttributes",CurHiData->ExcludeAttr);
    SetRegKey(RegKey,"NormalColor",(DWORD)CurHiData->Colors.Color);
    SetRegKey(RegKey,"SelectedColor",(DWORD)CurHiData->Colors.SelColor);
    SetRegKey(RegKey,"CursorColor",(DWORD)CurHiData->Colors.CursorColor);
    SetRegKey(RegKey,"SelectedCursorColor",(DWORD)CurHiData->Colors.CursorSelColor);
    SetRegKey(RegKey,"MarkChar",(DWORD)CurHiData->Colors.MarkChar);
  }
  for (I=HiDataCount;I<StartHiDataCount;I++)
  {
    itoa(I,Ptr,10);
    DeleteRegKey(RegKey);
  }
}

/* $ 17.05.2001 DJ
   ��������� ���������������� (������ ��������� � ����� ����� �������)
*/

static void UncheckCheckbox(HANDLE hDlg, int ItemID)
{
  FarDialogItem DlgItem;
  Dialog::SendDlgMessage (hDlg, DM_GETDLGITEM, ItemID, (long) &DlgItem);
  if (DlgItem.Selected)
  {
    DlgItem.Selected = 0;
    Dialog::SendDlgMessage (hDlg, DM_SETDLGITEM, ItemID, (long) &DlgItem);
  }
}

static long WINAPI HighlightDlgProc(HANDLE hDlg, int Msg, int Param1, long Param2)
{
  switch (Msg)
  {
    case DN_BTNCLICK:
      if (Param1 >= 5 && Param1 <= 21 && Param2)
      {
        // ���������� ����������������
        if (Param1 >= 5 && Param1 <= 12)
          UncheckCheckbox (hDlg, Param1 + 9);
        else if (Param1 >= 14 && Param1 <= 21)
          UncheckCheckbox (hDlg, Param1 - 9);
      }
      else {
        HighlightData *EditData = (HighlightData *) Dialog::SendDlgMessage (hDlg, DM_GETDLGDATA, 0, 0);
        unsigned int Color;
        switch (Param1)
        {
          case 23:
            Color=(DWORD)EditData->Colors.Color;
            break;
          case 24:
            Color=(DWORD)EditData->Colors.SelColor;
            break;
          case 25:
            Color=(DWORD)EditData->Colors.CursorColor;
            break;
          case 26:
            Color=(DWORD)EditData->Colors.CursorSelColor;
            break;
          case 31:
          case 32:
            return FALSE;
        }
        GetColorDialog(Color);
        switch (Param1)
        {
          case 23:
            EditData->Colors.Color=(BYTE)Color;
            break;
          case 24:
            EditData->Colors.SelColor=(BYTE)Color;
            break;
          case 25:
            EditData->Colors.CursorColor=(BYTE)Color;
            break;
          case 26:
            EditData->Colors.CursorSelColor=(BYTE)Color;
            break;
        }
      }
      return TRUE;
  }
  return Dialog::DefDlgProc (hDlg, Msg, Param1, Param2);
}

/* DJ $ */

int HighlightFiles::EditRecord(int RecPos,int New)
{
  const char *HistoryName="Masks";
  static struct DialogData HiEditDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,72,21,0,0,0,0,(char *)MHighlightEditTitle,
  /* 01 */DI_TEXT,5,2,0,0,0,0,0,0,(char *)MHighlightMasks,
  /* 02 */DI_EDIT,5,3,70,3,1,(DWORD)HistoryName,DIF_HISTORY|DIF_VAREDIT,0,"",
  /* 03 */DI_TEXT,3,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 04 */DI_TEXT,5,5,0,0,0,0,DIF_BOXCOLOR,0,(char *)MHighlightIncludeAttr,
  /* 05 */DI_CHECKBOX,5,6,0,0,0,0,0,0,(char *)MHighlightRO,
  /* 06 */DI_CHECKBOX,5,7,0,0,0,0,0,0,(char *)MHighlightHidden,
  /* 07 */DI_CHECKBOX,5,8,0,0,0,0,0,0,(char *)MHighlightSystem,
  /* 08 */DI_CHECKBOX,5,9,0,0,0,0,0,0,(char *)MHighlightArchive,
  /* 09 */DI_CHECKBOX,5,10,0,0,0,0,0,0,(char *)MHighlightCompressed,
  /* 10 */DI_CHECKBOX,5,11,0,0,0,0,0,0,(char *)MHighlightEncrypted,
  /* 11 */DI_CHECKBOX,5,12,0,0,0,0,0,0,(char *)MHighlightFolder,
  /* 12 */DI_CHECKBOX,5,13,0,0,0,0,0,0,(char *)MHighlightJunction,
  /* 13 */DI_TEXT,37,5,0,0,0,0,DIF_BOXCOLOR,0,(char *)MHighlightExcludeAttr,
  /* 14 */DI_CHECKBOX,37,6,0,0,0,0,0,0,(char *)MHighlightRO,
  /* 15 */DI_CHECKBOX,37,7,0,0,0,0,0,0,(char *)MHighlightHidden,
  /* 16 */DI_CHECKBOX,37,8,0,0,0,0,0,0,(char *)MHighlightSystem,
  /* 17 */DI_CHECKBOX,37,9,0,0,0,0,0,0,(char *)MHighlightArchive,
  /* 18 */DI_CHECKBOX,37,10,0,0,0,0,0,0,(char *)MHighlightCompressed,
  /* 19 */DI_CHECKBOX,37,11,0,0,0,0,0,0,(char *)MHighlightEncrypted,
  /* 20 */DI_CHECKBOX,37,12,0,0,0,0,0,0,(char *)MHighlightFolder,
  /* 21 */DI_CHECKBOX,37,13,0,0,0,0,0,0,(char *)MHighlightJunction,
  /* 22 */DI_TEXT,-1,14,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,(char *)MHighlightColors,
  /* 23 */DI_BUTTON,5,15,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightNormal,
  /* 24 */DI_BUTTON,5,16,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightSelected,
  /* 25 */DI_BUTTON,37,15,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightCursor,
  /* 26 */DI_BUTTON,37,16,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightSelectedCursor,
  /* 27 */DI_TEXT,3,17,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 28 */DI_TEXT,7,18,0,0,0,0,0,0,(char *)MHighlightMarkChar,
  /* 29 */DI_FIXEDIT,5,18,5,18,0,0,0,0,"",
  /* 30 */DI_TEXT,3,19,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 31 */DI_BUTTON,0,20,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
  /* 32 */DI_BUTTON,0,20,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(HiEditDlgData,HiEditDlg);
  struct HighlightData EditData;
  char Mask[HIGHLIGHT_MASK_SIZE], *Ptr;

  *Mask=0;

  if (!New && RecPos<HiDataCount)
  {
    EditData=HiData[RecPos];
    if((Ptr=GetMask(RecPos)) != NULL)
      strcpy(Mask,Ptr);
  }
  else
    memset(&EditData,0,sizeof(EditData));

  HiEditDlg[2].Ptr.PtrData=Mask;
  HiEditDlg[2].Ptr.PtrLength=sizeof(Mask);

  HiEditDlg[5].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_READONLY)!=0;
  HiEditDlg[6].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_HIDDEN)!=0;
  HiEditDlg[7].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_SYSTEM)!=0;
  HiEditDlg[8].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_ARCHIVE)!=0;
  HiEditDlg[9].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
  HiEditDlg[10].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;
  HiEditDlg[11].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_DIRECTORY)!=0;
  HiEditDlg[12].Selected=(EditData.IncludeAttr & FILE_ATTRIBUTE_REPARSE_POINT)!=0;

  HiEditDlg[14].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_READONLY)!=0;
  HiEditDlg[15].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_HIDDEN)!=0;
  HiEditDlg[16].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_SYSTEM)!=0;
  HiEditDlg[17].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_ARCHIVE)!=0;
  HiEditDlg[18].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_COMPRESSED)!=0;
  HiEditDlg[19].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0;
  HiEditDlg[20].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_DIRECTORY)!=0;
  HiEditDlg[21].Selected=(EditData.ExcludeAttr & FILE_ATTRIBUTE_REPARSE_POINT)!=0;

  *HiEditDlg[29].Data=EditData.Colors.MarkChar;

  /* $ 18.05.2001 DJ
     ��������� ���������������� � ������ ���������� � ���������� �������
  */
  Dialog Dlg(HiEditDlg,sizeof(HiEditDlg)/sizeof(HiEditDlg[0]),HighlightDlgProc,(long) &EditData);
  Dlg.SetHelp("HighlightEdit");
  Dlg.SetPosition(-1,-1,76,23);
  /* $ 06.07.2001 IS
     �������� ����� �� ������������
  */
  CFileMask FMask;
  for(;;)
  {
    Dlg.ClearDone();
    Dlg.Process();
    if (Dlg.GetExitCode() != 31)
      return(FALSE);
    if (*(char *)HiEditDlg[2].Ptr.PtrData==0)
      return(FALSE);
    if(FMask.Set(static_cast<char *>(HiEditDlg[2].Ptr.PtrData), 0))
      break;
  }
  /* IS $ */
  /* DJ $ */

  EditData.IncludeAttr=EditData.ExcludeAttr=0;
  if (HiEditDlg[5].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_READONLY;
  if (HiEditDlg[6].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_HIDDEN;
  if (HiEditDlg[7].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_SYSTEM;
  if (HiEditDlg[8].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_ARCHIVE;
  if (HiEditDlg[9].Selected)
  {
    EditData.IncludeAttr|=FILE_ATTRIBUTE_COMPRESSED;
    EditData.IncludeAttr&=~FILE_ATTRIBUTE_ENCRYPTED;
  }
  else if (HiEditDlg[10].Selected)
  {
    EditData.IncludeAttr&=~FILE_ATTRIBUTE_COMPRESSED;
    EditData.IncludeAttr|=FILE_ATTRIBUTE_ENCRYPTED;
  }
  if (HiEditDlg[11].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_DIRECTORY;
  if (HiEditDlg[12].Selected)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_REPARSE_POINT;

  if (HiEditDlg[14].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_READONLY;
  if (HiEditDlg[15].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_HIDDEN;
  if (HiEditDlg[16].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_SYSTEM;
  if (HiEditDlg[17].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_ARCHIVE;
  if (HiEditDlg[18].Selected)
  {
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_COMPRESSED;
    EditData.ExcludeAttr&=~FILE_ATTRIBUTE_ENCRYPTED;
  }
  else if (HiEditDlg[19].Selected)
  {
    EditData.ExcludeAttr&=~FILE_ATTRIBUTE_COMPRESSED;
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_ENCRYPTED;
  }
  if (HiEditDlg[20].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_DIRECTORY;
  if (HiEditDlg[21].Selected)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_REPARSE_POINT;

  EditData.Colors.MarkChar=*HiEditDlg[29].Data;

  if (!New && RecPos<HiDataCount)
  {
    if(!AddMask(HiData+RecPos,Mask,&EditData))
      return FALSE;
  }
  if (New)
    DupHighlightData(&EditData,Mask,RecPos);
  return(TRUE);
}

int HighlightFiles::DupHighlightData(struct HighlightData *EditData,char *Mask,int RecPos)
{
  struct HighlightData *NewHiData;
  struct HighlightData HData={0};

  if(!AddMask(&HData,Mask,EditData))
    return FALSE;

  if ((NewHiData=(struct HighlightData *)realloc(HiData,sizeof(*HiData)*(HiDataCount+1)))==NULL)
  {
    DeleteMask(&HData);
    return(FALSE);
  }

  HiDataCount++;
  HiData=NewHiData;
  for (int I=HiDataCount-1;I>RecPos;I--)
    HiData[I]=HiData[I-1];
  memcpy(HiData+RecPos,&HData,sizeof(struct HighlightData));
  return TRUE;
}

/*
 ��������� ��� ����� � �������;  ���������� ��������� �� ����� ������
 ����������:
  char RegKey[80];
  char *Ptr=MkRegKeyHighlightName(RegKey);
  for(I=0;...)
  {
    itoa(I,Ptr,10);
  }
*/
char *MkRegKeyHighlightName(char *RegKey)
{
  return RegKey+strlen(strcat(strcpy(RegKey,RegColorsHighlight),"\\Group"));
}


void SetHighlighting()
{
  if (CheckRegKey(RegColorsHighlight))
    return;

  int I;
  char RegKey[80], *Ptr;
  // ����� �������� %PATHEXT%, � HighlightFiles::GetHiColor() ��� �����������
  // �������.
  char CmdExt[512]="*.exe,*.com,*.bat,%PATHEXT%";
  static char *Masks[]={
    "*.*",
    CmdExt,
    "*.rar,*.r[0-9][0-9],*.ar[cj],*.a[0-9][0-9],*.j,*.ac[bei],*.zip,*.z,*.jar,*.ice,*.lha,*.lzh,*.ain,*.imp,*.777,*.ufa,*.boa,*.bs[2a],*.cab,*.chz,*.ha,*.h[ay]p,*.hpk,*.lim,*.[lw]sz,*.pa[ck],*.rk,*.rkv,*.rpm,*.sqz,*.bz,*.bz2,*.bzip,*.gz,*.tar,*.t[ag]z,*.uc2,*.x2,*.zoo,*.hqx,*.sea,*.sit,*.uue,*.xxe,*.ddi,*.tdr,*.xdf",
    "*.bak,*.tmp",
    /* $ 07.07.2001  IS
       ��� ����� ��� ���������: ������������ ��� ��������, ����� ���, ���
       �������� ������������� (�� ����� - ��� �����).
    */
    "*.*|..", // ����� ��� ���������
    /* IS $ */
  };
  /* $ 06.07.2001 IS
     ������ �������� ������ - ���������� ������ OriginalMasks
  */
  struct HighlightData  StdHighlightData[]=
  { /*
     OriginalMask              NormalColor       SelectedCursorColor
               IncludeAttributes       SelectedColor     MarkChar
                       ExcludeAttributes     CursorColor             */
    {Masks[0], NULL, 0x0002, 0x0000, {0x13, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {Masks[0], NULL, 0x0004, 0x0000, {0x13, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {Masks[4], NULL, 0x0010, 0x0000, {0x1F, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {Masks[1], NULL, 0x0000, 0x0000, {0x1A, 0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {Masks[2], NULL, 0x0000, 0x0000, {0x1D, 0x00, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00}},
    {Masks[3], NULL, 0x0000, 0x0000, {0x16, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00}},
  };

  // ��� NT ��������� CMD
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
    strcat(CmdExt,",*.cmd");

  Ptr=MkRegKeyHighlightName(RegKey);
  for(I=0; I < sizeof(StdHighlightData)/sizeof(StdHighlightData[0]); ++I)
  {
    itoa(I,Ptr,10);
    SetRegKey(RegKey,"Mask",StdHighlightData[I].OriginalMasks);
  /* IS $ */
    if(StdHighlightData[I].IncludeAttr)
      SetRegKey(RegKey,"IncludeAttributes",StdHighlightData[I].IncludeAttr);
    if(StdHighlightData[I].ExcludeAttr)
      SetRegKey(RegKey,"ExcludeAttributes",StdHighlightData[I].ExcludeAttr);
    if(StdHighlightData[I].Colors.Color)
      SetRegKey(RegKey,"NormalColor",StdHighlightData[I].Colors.Color);
    if(StdHighlightData[I].Colors.SelColor)
      SetRegKey(RegKey,"SelectedColor",StdHighlightData[I].Colors.SelColor);
    if(StdHighlightData[I].Colors.CursorColor)
      SetRegKey(RegKey,"CursorColor",StdHighlightData[I].Colors.CursorColor);
    if(StdHighlightData[I].Colors.CursorSelColor)
      SetRegKey(RegKey,"SelectedCursorColor",StdHighlightData[I].Colors.CursorSelColor);
    if(StdHighlightData[I].Colors.MarkChar)
      SetRegKey(RegKey,"MarkChar",StdHighlightData[I].Colors.MarkChar);
  }
}
