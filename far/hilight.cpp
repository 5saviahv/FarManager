/*
hilight.cpp

Files highlighting

*/

/* Revision: 1.25 06.07.2001 $ */

/*
Modify:
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

#define HIGHLIGHT_MASK_SIZE      2048

HighlightFiles::HighlightFiles()
{
 /* $ 07.07.2000 IS
   ���� ��� ������� � ��������� �������, ����� ����� ���� ��� ������������
   ��������
 */
 InitHighlightFiles();
 /* IS $ */
}

void HighlightFiles::InitHighlightFiles()
{
  HiData=NULL;
  HiDataCount=0;
  char RegKey[80],Mask[HIGHLIGHT_MASK_SIZE];
  char *Ptr=MkRegKeyHighlightName(RegKey); // Ptr ��������� �� ������ ����� :-)
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
      HData.ExcludeAttr=GetRegKey(RegKey,"ExcludeAttributes",0);
      HData.Color=GetRegKey(RegKey,"NormalColor",0);
      HData.SelColor=GetRegKey(RegKey,"SelectedColor",0);
      HData.CursorColor=GetRegKey(RegKey,"CursorColor",0);
      HData.CursorSelColor=GetRegKey(RegKey,"SelectedCursorColor",0);
      HData.MarkChar=GetRegKey(RegKey,"MarkChar",0);
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
void HighlightFiles::GetHiColor(char *Path,int Attr,unsigned char &Color,
     unsigned char &SelColor,unsigned char &CursorColor,
     unsigned char &CursorSelColor,unsigned char &MarkChar)
{
  Color=SelColor=CursorColor=CursorSelColor=MarkChar=0;
  for (int I=0;I<HiDataCount;I++)
  {
    struct HighlightData *CurHiData=&HiData[I];
    if ((Attr & CurHiData->IncludeAttr)==CurHiData->IncludeAttr &&
        (Attr & CurHiData->ExcludeAttr)==0)
    {
      Path=Path?Path:""; // ���� Path==NULL, �� �������, ��� ��� ������ ������
      if(CurHiData->FMasks->Compare(Path))
        {
          Color=CurHiData->Color;
          SelColor=CurHiData->SelColor;
          CursorColor=CurHiData->CursorColor;
          CursorSelColor=CurHiData->CursorSelColor;
          MarkChar=CurHiData->MarkChar;
          return;
        }
    }
  }
}
/* IS $ */
/* DJ $ */


void HighlightFiles::HiEdit(int MenuPos)
{
  struct MenuItem HiMenuItem;
  memset(&HiMenuItem,0,sizeof(HiMenuItem));

  {
    unsigned char VerticalLine=0x0B3;
    VMenu HiMenu(MSG(MHighlightTitle),NULL,0,ScrY-4);
    HiMenu.SetHelp("Highlight");
    HiMenu.SetFlags(VMENU_WRAPMODE|VMENU_SHOWAMPERSAND);
    HiMenu.SetPosition(-1,-1,0,0);
    HiMenu.SetBottomTitle(MSG(MHighlightBottom));
    for (int I=0;I<HiDataCount;I++)
    {
      struct HighlightData *CurHiData=&HiData[I];
      sprintf(HiMenuItem.Name,"%c %c %c%c%c%c%c%c%c %c %c%c%c%c%c%c%c %c %.60s",
        (CurHiData->MarkChar?CurHiData->MarkChar:' '), // ������� ����� �������

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
      HiMenu.AddItem(&HiMenuItem);
    }
    *HiMenuItem.Name=0;
    HiMenuItem.SetSelect(HiDataCount==MenuPos);
    HiMenu.AddItem(&HiMenuItem);

    {
      int NeedUpdate;
      HiMenu.Show();
      while (1)
      {
        Panel *LeftPanel=CtrlObject->Cp()->LeftPanel;
        Panel *RightPanel=CtrlObject->Cp()->RightPanel;
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
              LeftPanel->Update(UPDATE_KEEP_SELECTION);
              LeftPanel->Redraw();
              RightPanel->Update(UPDATE_KEEP_SELECTION);
              RightPanel->Redraw();
              HiEdit(0);
              return;
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
            default:
              HiMenu.ProcessInput();
              break;
          }
          // ������������� �����!
          if(NeedUpdate)
          {
             HiMenu.Hide();
             SaveHiData();
             LeftPanel->Update(UPDATE_KEEP_SELECTION);
             LeftPanel->Redraw();
             RightPanel->Update(UPDATE_KEEP_SELECTION);
             RightPanel->Redraw();
             HiEdit(SelectPos);
             return;
          }
        }
        if (HiMenu.GetExitCode()!=-1)
        {
          HiMenu.ClearDone();
          HiMenu.WriteInput(KEY_F4);
          continue;
        }
        break;
      }
    }
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
    SetRegKey(RegKey,"NormalColor",CurHiData->Color);
    SetRegKey(RegKey,"SelectedColor",CurHiData->SelColor);
    SetRegKey(RegKey,"CursorColor",CurHiData->CursorColor);
    SetRegKey(RegKey,"SelectedCursorColor",CurHiData->CursorSelColor);
    SetRegKey(RegKey,"MarkChar",CurHiData->MarkChar);
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

static void UncheckCheckbox (HANDLE hDlg, int ItemID)
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
        switch (Param1)
        {
          case 23:
            GetColorDialog(EditData->Color);
            break;
          case 24:
            GetColorDialog(EditData->SelColor);
            break;
          case 25:
            GetColorDialog(EditData->CursorColor);
            break;
          case 26:
            GetColorDialog(EditData->CursorSelColor);
            break;
          case 31:
          case 32:
            return FALSE;
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
  int ExitCode=0;
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

  *HiEditDlg[29].Data=EditData.MarkChar;

  /* $ 18.05.2001 DJ
     ��������� ���������������� � ������ ���������� � ���������� �������
  */
  Dialog Dlg(HiEditDlg,sizeof(HiEditDlg)/sizeof(HiEditDlg[0]),HighlightDlgProc,(long) &EditData);
  Dlg.SetHelp("Highlight");
  Dlg.SetPosition(-1,-1,76,23);
  /* $ 06.07.2001 IS
     �������� ����� �� ������������
  */
  CFileMask FMask;
  for(;;)
  {
    Dlg.ClearDone();
    Dlg.Process();
    if (Dlg.GetExitCode() != 31) return(FALSE);
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

  EditData.MarkChar=*HiEditDlg[29].Data;

  if (!New && RecPos<HiDataCount)
  {
    if(!AddMask(HiData+RecPos,Mask,&EditData))
      return FALSE;
  }
  if (New)
  {
    struct HighlightData *NewHiData;
    struct HighlightData HData={0};

    if(!AddMask(&HData,Mask,&EditData))
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
  }
  return(TRUE);
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
