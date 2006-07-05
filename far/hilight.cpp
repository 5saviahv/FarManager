/*
hilight.cpp

Files highlighting

*/

/* Revision: 1.49 05.07.2006 $ */

/*
Modify:
  05.07.2006 IS
    - warnings
  24.02.2006 AY
    - ��������
  09.02.2006 AY
    - ������� �������� I, T, $ � ������ � ���� ���������.
    - ������ �������� ��������� ������� ���������.
    - ���������� ��������� ������� ��������� ���� ���� ����� ��������� � �������.
  06.08.2004 SKV
    ! see 01825.MSVCRT.txt
  22.09.2003 SVS
    - � ���������� ������ ���� ������ CtrlUp �� ����� �������
      ��������� ��� ���������� ������! (�� ������� ���� ���������)
  20.02.2003 SVS
    ! ������� strcmp(FooBar,"..") �� TestParentFolderName(FooBar)
  22.01.2003 IS
    ! � ���� ������ ��� ������� ������ ���������� � ��������, ����� �����
      ���� �������� ������ �� �������
  21.01.2003 SVS
    + xf_malloc,xf_realloc,xf_free - ������� ������ malloc,realloc,free
      ������� ������ ������� � ����������� ������ xf_* ������ �������.
  27.11.2002 SVS
    - BugZ#771 - ��������� ������ �� ���������
  17.09.2002 SVS
    - Bug#639 - ��� ������ ��� �������� ��������� ��������
  06.09.2002 SVS
    ! �������� � ����������� ����� ���������� ��� ����������� (�������� 7z)
  11.06.2002 SVS
    - ������� �������� � ���������� (��. �������� � 01439.Hilight.txt)
  24.05.2002 SVS
    + ������������ Numpad-������
  22.03.2002 SVS
    - strcpy - Fuck!
  14.12.2001 SVS
    - BugZ#178 - ��� ������ SaveHiData() �����  Opt.AutoSaveSetup
  23.11.2001 SVS
    + GetHiColor(), ���������� � ����� �������� FileListItem
  21.11.2001 SVS
    - ���� � ����������� - ���� ������ �� ��������, �������� ��� RO � �������
      ������, �� ����� ������� ������ ������� ����������... ������ ������
      ����� :-(
    ! ��������� DIF_AUTOMATION
  25.09.2001 IS
    ! ��������� ��������� ������� ��� �������� � ���� ����.
    + ����� ��������� ������ ����� ��� ���������� ������ ���������
      (��������� IgnoreMask)
    ! �������� ��������� ��������� ��� �������� �� ���������� � �������
      ��������� (GetHiColor): ���� �������� Path ����� NULL, �� ����� �
      ������� �� ������������, ����������� ������ ��������, ������ � ����
      ������ ����� ���������� ����� ��� �����, � ������� ����� ���������
      �� �������.
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
#include "filelist.hpp"
#include "savescr.hpp"
#include "ctrlobj.hpp"
#include "scrbuf.hpp"

#define HIGHLIGHT_MASK_SIZE      2048

static struct HighlightDataColor WorkColor;

/* $ 25.09.2001 IS
     ��� ������ ��������� ��������� ��� "��������� ������"
*/
struct HighlightStrings
{
  char *IncludeAttributes,*ExcludeAttributes,*Mask,*IgnoreMask,
       *NormalColor,*SelectedColor,*CursorColor,*SelectedCursorColor,
       *MarkChar,*HighlightEdit,*HighlightList;
};
const HighlightStrings HLS=
{
  "IncludeAttributes","ExcludeAttributes","Mask","IgnoreMask",
  "NormalColor","SelectedColor","CursorColor","SelectedCursorColor",
  "MarkChar","HighlightEdit","HighlightList"
};
/* IS $ */

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
  if(HiData) xf_free(HiData); HiData=NULL;
  HiDataCount=0;
  char RegKey[80],Mask[HIGHLIGHT_MASK_SIZE];
  char *Ptr=MkRegKeyHighlightName(RegKey); // Ptr ��������� �� ������ ����� :-)
  ReWriteWorkColor();
  while (1)
  {
    itoa(HiDataCount,Ptr,10);
    if (!GetRegKey(RegKey,HLS.Mask,Mask,"",sizeof(Mask)))
      break;
    struct HighlightData *NewHiData;
    struct HighlightData HData={0};
    HData.IgnoreMask=GetRegKey(RegKey,HLS.IgnoreMask,FALSE);
    if(AddMask(&HData,Mask,HData.IgnoreMask))
    {
      if ((NewHiData=(struct HighlightData *)xf_realloc(HiData,sizeof(*HiData)*(HiDataCount+1)))==NULL)
      {
        DeleteMask(&HData);
        break;
      }
      HiData=NewHiData;
      HData.IncludeAttr=GetRegKey(RegKey,HLS.IncludeAttributes,0);
      HData.ExcludeAttr=GetRegKey(RegKey,HLS.ExcludeAttributes,0)&(~HData.IncludeAttr);
      HData.Colors.Color=(BYTE)GetRegKey(RegKey,HLS.NormalColor,(DWORD)WorkColor.Color);
      HData.Colors.SelColor=(BYTE)GetRegKey(RegKey,HLS.SelectedColor,(DWORD)WorkColor.SelColor);
      HData.Colors.CursorColor=(BYTE)GetRegKey(RegKey,HLS.CursorColor,(DWORD)WorkColor.CursorColor);
      HData.Colors.CursorSelColor=(BYTE)GetRegKey(RegKey,HLS.SelectedCursorColor,(DWORD)WorkColor.CursorSelColor);
      HData.Colors.MarkChar=(BYTE)GetRegKey(RegKey,HLS.MarkChar,0);
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
BOOL HighlightFiles::AddMask(struct HighlightData *Dest,char *Mask,BOOL IgnoreMask,struct HighlightData *Src)
{
  char *Ptr, *OPtr;

  if(Src)
  {
    // ������ ��� �������� - OriginalMasks
    if((OPtr=(char *)xf_malloc(strlen(Mask)+1)) == NULL)
      return FALSE;
    memmove(Dest,Src,sizeof(struct HighlightData));
  }
  else
  {
    // ������ ��� �������� - OriginalMasks
    if((OPtr=(char *)xf_realloc(Dest->OriginalMasks,strlen(Mask)+1)) == NULL)
      return FALSE;
  }
  /* ��������� %PATHEXT% */
  strcpy(OPtr,Mask); // ��������� ��������.
  // ��������
  if((Ptr=strchr(Mask,'%')) != NULL && !strnicmp(Ptr,"%PATHEXT%",9))
  {
    int IQ1=(*(Ptr+9) == ',')?10:9, offsetPtr=Ptr-Mask;
    // ���� ����������� %pathext%, �� ������� � �����...
    memmove(Ptr,Ptr+IQ1,strlen(Ptr+IQ1)+1);

    char Tmp1[HIGHLIGHT_MASK_SIZE], *pSeparator;
    xstrncpy(Tmp1, Mask,sizeof(Tmp1)-1);
    pSeparator=strchr(Tmp1, EXCLUDEMASKSEPARATOR);
    if(pSeparator)
    {
      Ptr=Tmp1+offsetPtr;
      if(Ptr>pSeparator) // PATHEXT ��������� � ������ ����������
        Add_PATHEXT(Mask); // ��������� ��, ���� ����.
      else
      {
        char Tmp2[HIGHLIGHT_MASK_SIZE];
        xstrncpy(Tmp2, pSeparator+1,sizeof(Tmp2)-1);
        *pSeparator=0;
        Add_PATHEXT(Tmp1);
        sprintf(Mask, "%s|%s", Tmp1, Tmp2);
      }
    }
    else
      Add_PATHEXT(Mask); // ��������� ��, ���� ����.
  }
  /* $ 25.09.2001 IS
     ���� IgnoreMask, �� �� �������� ������ ��� ����� CFileMask, �.�. �� ���
     �� �����.
  */
  // ������ ��� ������� �����
  CFileMask *FMasks=NULL;
  if(!IgnoreMask)
  {
    if((FMasks=new CFileMask) == NULL)
    {
      xf_free(OPtr);
      return FALSE;
    }

    if(!FMasks->Set(Mask, FMF_SILENT)) // �������� ������������ �����
    {
      delete FMasks;
      xf_free(OPtr);
      return FALSE;
    }
  }
  Dest->IgnoreMask=IgnoreMask;
  /* IS $ */

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
    xf_free(CurHighlightData->OriginalMasks);
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
    xf_free(HiData);
  }
  HiData=NULL;
  HiDataCount=0;
}

/* $ 01.05.2001 DJ
   ���������������� ������ �������� Masks
*/
/* $ 06.07.2001 IS ������ "�������" ����� ���������� ��������������� ����� */
/* $ 25.09.2001 IS
   �������� ��������� ��������� ��� �������� �� ���������� � �������
   ���������: ���� �������� Path ����� NULL, �� ����� � ������� ��
   ������������, ����������� ������ ��������, ������ � ���� ������ �����
   ���������� ����� ��� �����, � ������� ����� ��������� �� �������.
*/
void HighlightFiles::GetHiColor(char *Path,int Attr,
                                struct HighlightDataColor *Colors)
{
  struct FileListItem FileItem;
  if(Path)
    xstrncpy(FileItem.Name,Path,sizeof(FileItem.Name)-1);
  else
    *FileItem.Name=0;
  FileItem.FileAttr=Attr;
  GetHiColor(&FileItem,1);
  memcpy(Colors,&FileItem.Colors,sizeof(struct HighlightDataColor));
}
/* IS $ */
/* IS $ */
/* DJ $ */

void HighlightFiles::GetHiColor(struct FileListItem *FileItem,int FileCount)
{
  if(!FileItem || !FileCount)
    return;

  struct HighlightData *CurHiData;
  struct HighlightDataColor Colors;
  int I, FCnt;
  ReWriteWorkColor(&Colors);
  //Path=Path?Path:""; // ���� Path==NULL, �� �������, ��� ��� ������ ������

  for(FCnt=0; FCnt < FileCount; ++FCnt,++FileItem)
  {
    DWORD Attr=FileItem->FileAttr;

    char *Path=FileItem->Name;
    if(!*Path) Path=NULL;
    memcpy(&FileItem->Colors,&Colors,sizeof(struct HighlightDataColor));
    for (CurHiData=HiData,I=0; I < HiDataCount;I++, ++CurHiData)
    {
      if ((Attr & CurHiData->IncludeAttr)==CurHiData->IncludeAttr &&
          (Attr & CurHiData->ExcludeAttr)==0)
      {
        if(CurHiData->IgnoreMask || (Path && CurHiData->FMasks->Compare(Path)))
        {
          memcpy(&FileItem->Colors,&CurHiData->Colors,sizeof(struct HighlightDataColor));
          break;
        }
      }
    }
  }
}

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

  /* $ 22.01.2003 IS
     ������ ��� ������� ������ ���������� � ��������, ����� ����� ����
     �������� ������ �� �������
  */
  char MarkChar[]="\" \"";
  int I, Short=1;
  // ������� �������� - � ���� �� ������� ������� ������ � ���� ������?
  for (I=0;I<HiDataCount;I++)
  {
    if(HiData[I].Colors.MarkChar)
    {
      Short=0;
      break;
    }
  }
  // ���� �������� ������� � ���� ���, �� ������� ��� ��� ���� ������ 1 ����������
  const char *emptyMarkChar=Short?" ":"   ";
  for (I=0;I<HiDataCount;I++)
  {
    struct HighlightData *CurHiData=&HiData[I];
    MarkChar[1]=CurHiData->Colors.MarkChar;
    sprintf(HiMenuItem.Name,"%s %c %c%c%c%c%c%c%c%c%c%c %c %c%c%c%c%c%c%c%c%c%c %c %.54s",
      // ������� ����� ������� � ��������
      (CurHiData->Colors.MarkChar?MarkChar:emptyMarkChar),
  /* IS $ */
      VerticalLine,

      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_READONLY) ? 'R':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':
        ((CurHiData->IncludeAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':'.'),
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_DIRECTORY) ? 'F':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_REPARSE_POINT) ? 'L':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_SPARSE_FILE) ? '$':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_TEMPORARY) ? 'T':'.',
      (CurHiData->IncludeAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) ? 'I':'.',

      VerticalLine,

      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_READONLY) ? 'R':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_HIDDEN) ? 'H':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_SYSTEM) ? 'S':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_ARCHIVE) ? 'A':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_COMPRESSED) ? 'C':
        ((CurHiData->ExcludeAttr & FILE_ATTRIBUTE_ENCRYPTED)?'E':'.'),
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_DIRECTORY) ? 'F':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_REPARSE_POINT) ? 'L':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_SPARSE_FILE) ? '$':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_TEMPORARY) ? 'T':'.',
      (CurHiData->ExcludeAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) ? 'I':'.',

      VerticalLine,

      /* $ 25.09.2001 IS �� ������ �����, ���� ��� ������������ */
      CurHiData->IgnoreMask?" ":GetMask(I));
      /* IS $ */
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
  HiMenu.SetHelp(HLS.HighlightList);
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
            HiData=(struct HighlightData *)xf_realloc(HiData,sizeof(*HiData)*(HiDataCount+1));
            NeedUpdate=TRUE;
          }
          break;
        case KEY_ENTER:
        case KEY_F4:
          if (SelectPos>=HiMenu.GetItemCount()-1)
            break;
        case KEY_INS: case KEY_NUMPAD0:
          if (EditRecord(SelectPos,Key == KEY_INS))
            NeedUpdate=TRUE;
          break;
        case KEY_F5:
          if (SelectPos < HiMenu.GetItemCount()-1)
          {
            if(DupHighlightData(HiData+SelectPos,GetMask(SelectPos),HiData[SelectPos].IgnoreMask,SelectPos))
              NeedUpdate=TRUE;
          }
          break;
        case KEY_CTRLUP: case KEY_CTRLNUMPAD8:
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
          HiMenu.ProcessInput();
          break;

        case KEY_CTRLDOWN: case KEY_CTRLNUMPAD2:
          if (SelectPos < HiMenu.GetItemCount()-2)
          {
            struct HighlightData HData;
            memcpy(&HData,HiData+SelectPos,sizeof(struct HighlightData));
            memcpy(HiData+SelectPos,HiData+SelectPos+1,sizeof(struct HighlightData));
            memcpy(HiData+SelectPos+1,&HData,sizeof(struct HighlightData));
            HiMenu.SetSelection(++SelectPos);
            NeedUpdate=TRUE;
          }
          HiMenu.ProcessInput();
          break;

        default:
          HiMenu.ProcessInput();
          break;
      }
      // ������������� �����!
      if(NeedUpdate)
      {
         ScrBuf.Lock(); // �������� ������ ����������
         HiMenu.Hide();
         if(Opt.AutoSaveSetup)
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
    SetRegKey(RegKey,HLS.Mask,GetMask(I));
    SetRegKey(RegKey,HLS.IgnoreMask,CurHiData->IgnoreMask);
    SetRegKey(RegKey,HLS.IncludeAttributes,CurHiData->IncludeAttr);
    SetRegKey(RegKey,HLS.ExcludeAttributes,CurHiData->ExcludeAttr);
    SetRegKey(RegKey,HLS.NormalColor,(DWORD)CurHiData->Colors.Color);
    SetRegKey(RegKey,HLS.SelectedColor,(DWORD)CurHiData->Colors.SelColor);
    SetRegKey(RegKey,HLS.CursorColor,(DWORD)CurHiData->Colors.CursorColor);
    SetRegKey(RegKey,HLS.SelectedCursorColor,(DWORD)CurHiData->Colors.CursorSelColor);
    SetRegKey(RegKey,HLS.MarkChar,(DWORD)CurHiData->Colors.MarkChar);
  }
  for (I=HiDataCount;I<StartHiDataCount;I++)
  {
    itoa(I,Ptr,10);
    DeleteRegKey(RegKey);
  }
}

enum enumHighlightEditRecords
{
  ID_HER_TITLE = 0,
  ID_HER_MATCHMASK,
  ID_HER_MASKEDIT,
  ID_HER_SEPARATOR1,
  ID_HER_ATTRBR,
  ID_HER_ATTRBH,
  ID_HER_ATTRBS,
  ID_HER_ATTRBA,
  ID_HER_ATTRBC,
  ID_HER_ATTRBE,
  ID_HER_ATTRBF,
  ID_HER_ATTRBL,
  ID_HER_ATTRBSP,
  ID_HER_ATTRBT,
  ID_HER_ATTRBNI,
  ID_HER_SEPARATOR2,
  ID_HER_MARK_TITLE,
  ID_HER_MARKEDIT,
  ID_HER_SEPARATOR3,
  ID_HER_NORMAL,
  ID_HER_SELECTED,
  ID_HER_CURSOR,
  ID_HER_SELECTEDCURSOR,
  ID_HER_COLOREXAMPLE,
  ID_HER_SEPARATOR4,
  ID_HER_OK,
  ID_HER_CANCEL
};

void HighlightDlgUpdateUserControl(CHAR_INFO *VBufColorExample, struct HighlightDataColor &Colors)
{
  const char *ptr;
  DWORD Color;
  DWORD Default=F_BLACK|B_BLACK;
  for (int j=0; j<4; j++)
  {
    switch (j)
    {
      case 0:
        Color=(DWORD)Colors.Color;
        if (Color==Default)
          Color=(DWORD)Palette[COL_PANELTEXT-COL_FIRSTPALETTECOLOR];
        break;
      case 1:
        Color=(DWORD)Colors.SelColor;
        if (Color==Default)
          Color=(DWORD)Palette[COL_PANELSELECTEDTEXT-COL_FIRSTPALETTECOLOR];
        break;
      case 2:
        Color=(DWORD)Colors.CursorColor;
        if (Color==Default)
          Color=(DWORD)Palette[COL_PANELCURSOR-COL_FIRSTPALETTECOLOR];
        break;
      case 3:
        Color=(DWORD)Colors.CursorSelColor;
        if (Color==Default)
          Color=(DWORD)Palette[COL_PANELSELECTEDCURSOR-COL_FIRSTPALETTECOLOR];
        break;
    }
    if (Colors.MarkChar)
      ptr=MSG(MHighlightExample2);
    else
      ptr=MSG(MHighlightExample1);
    for (int k=0; k<15; k++)
    {
      VBufColorExample[15*j+k].Char.AsciiChar=ptr[k];
      VBufColorExample[15*j+k].Attributes=static_cast<WORD>(Color);
    }
    if (Colors.MarkChar)
      VBufColorExample[15*j+1].Char.AsciiChar=Colors.MarkChar;
    VBufColorExample[15*j].Attributes=Palette[COL_PANELBOX-COL_FIRSTPALETTECOLOR];
    VBufColorExample[15*j+14].Attributes=Palette[COL_PANELBOX-COL_FIRSTPALETTECOLOR];
  }
}

/* $ 17.05.2001 DJ
   ��������� ���������������� (������ ��������� � ����� ����� �������)
*/

static long WINAPI HighlightDlgProc(HANDLE hDlg, int Msg, int Param1, long Param2)
{
  switch (Msg)
  {
    case DN_BTNCLICK:
    case DN_MOUSECLICK:
      if((Msg==DN_BTNCLICK && Param1 >= ID_HER_NORMAL && Param1 <= ID_HER_SELECTEDCURSOR)
         || (Msg==DN_MOUSECLICK && Param1==ID_HER_COLOREXAMPLE && ((MOUSE_EVENT_RECORD *)Param2)->dwButtonState==FROM_LEFT_1ST_BUTTON_PRESSED))
      {
        HighlightData *EditData = (HighlightData *) Dialog::SendDlgMessage (hDlg, DM_GETDLGDATA, 0, 0);
        unsigned int Color;
        if (Msg==DN_MOUSECLICK)
          Param1 = ID_HER_NORMAL + ((MOUSE_EVENT_RECORD *)Param2)->dwMousePosition.Y;
        switch (Param1)
        {
          case ID_HER_NORMAL:
            Color=(DWORD)EditData->Colors.Color;
            break;
          case ID_HER_SELECTED:
            Color=(DWORD)EditData->Colors.SelColor;
            break;
          case ID_HER_CURSOR:
            Color=(DWORD)EditData->Colors.CursorColor;
            break;
          case ID_HER_SELECTEDCURSOR:
            Color=(DWORD)EditData->Colors.CursorSelColor;
            break;
        }
        GetColorDialog(Color,true);
        switch (Param1)
        {
          case ID_HER_NORMAL:
            EditData->Colors.Color=(BYTE)Color;
            break;
          case ID_HER_SELECTED:
            EditData->Colors.SelColor=(BYTE)Color;
            break;
          case ID_HER_CURSOR:
            EditData->Colors.CursorColor=(BYTE)Color;
            break;
          case ID_HER_SELECTEDCURSOR:
            EditData->Colors.CursorSelColor=(BYTE)Color;
            break;
        }
        FarDialogItem MarkChar, ColorExample;
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_HER_MARKEDIT,(long)&MarkChar);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_HER_COLOREXAMPLE,(long)&ColorExample);
        EditData->Colors.MarkChar=*MarkChar.Data.Data;
        HighlightDlgUpdateUserControl(ColorExample.Param.VBuf,EditData->Colors);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_HER_COLOREXAMPLE,(long)&ColorExample);
        return TRUE;
      }
      break;

    case DN_EDITCHANGE:
      if (Param1 == ID_HER_MARKEDIT)
      {
        HighlightData *EditData = (HighlightData *) Dialog::SendDlgMessage (hDlg, DM_GETDLGDATA, 0, 0);
        FarDialogItem *MarkChar, ColorExample;
        MarkChar=(FarDialogItem *)Param2;
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_HER_COLOREXAMPLE,(long)&ColorExample);
        EditData->Colors.MarkChar=*(MarkChar->Data.Data);
        HighlightDlgUpdateUserControl(ColorExample.Param.VBuf,EditData->Colors);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_HER_COLOREXAMPLE,(long)&ColorExample);
        return TRUE;
      }
  }
  return Dialog::DefDlgProc (hDlg, Msg, Param1, Param2);
}

/* DJ $ */

/* $ 25.09.2001 IS
     ��������� IgnoreMask
*/
int HighlightFiles::EditRecord(int RecPos,int New)
{
  const char *HistoryName="Masks";
  static struct DialogData HiEditDlgData[]={
  /* 00 */DI_DOUBLEBOX,3,1,65,20,0,0,0,0,(char *)MHighlightEditTitle,
  /* 01 */DI_CHECKBOX,5,2,0,0,0,0,DIF_AUTOMATION,0,(char *)MHighlightMasks,
  /* 02 */DI_EDIT,5,3,63,3,1,(DWORD)HistoryName,DIF_HISTORY|DIF_VAREDIT,0,"",
  /* 03 */DI_TEXT,-1,4,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,(char *)MHighlightIncExcTitle,
  /* 04 */DI_CHECKBOX,5,5,0,0,0,0,DIF_3STATE,0,(char *)MHighlightRO,
  /* 05 */DI_CHECKBOX,5,6,0,0,0,0,DIF_3STATE,0,(char *)MHighlightHidden,
  /* 06 */DI_CHECKBOX,5,7,0,0,0,0,DIF_3STATE,0,(char *)MHighlightSystem,
  /* 07 */DI_CHECKBOX,5,8,0,0,0,0,DIF_3STATE,0,(char *)MHighlightArchive,
  /* 08 */DI_CHECKBOX,5,9,0,0,0,0,DIF_3STATE,0,(char *)MHighlightCompressed,
  /* 09 */DI_CHECKBOX,5,10,0,0,0,0,DIF_3STATE,0,(char *)MHighlightEncrypted,
  /* 10 */DI_CHECKBOX,35,5,0,0,0,0,DIF_3STATE,0,(char *)MHighlightFolder,
  /* 11 */DI_CHECKBOX,35,6,0,0,0,0,DIF_3STATE,0,(char *)MHighlightJunction,
  /* 12 */DI_CHECKBOX,35,7,0,0,0,0,DIF_3STATE,0,(char *)MHighlightSparse,
  /* 13 */DI_CHECKBOX,35,8,0,0,0,0,DIF_3STATE,0,(char *)MHighlightTemporary,
  /* 14 */DI_CHECKBOX,35,9,0,0,0,0,DIF_3STATE,0,(char *)MHighlightNotIndexed,
  /* 15 */DI_TEXT,3,11,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 16 */DI_TEXT,7,12,0,0,0,0,0,0,(char *)MHighlightMarkChar,
  /* 17 */DI_FIXEDIT,5,12,5,12,0,0,0,0,"",
  /* 18 */DI_TEXT,-1,13,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,(char *)MHighlightColors,
  /* 19 */DI_BUTTON,5,14,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightNormal,
  /* 20 */DI_BUTTON,5,15,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightSelected,
  /* 21 */DI_BUTTON,5,16,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightCursor,
  /* 22 */DI_BUTTON,5,17,0,0,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightSelectedCursor,
  /* 23 */DI_USERCONTROL,65-15-1,14,65-2,17,0,0,DIF_NOFOCUS,0,"",
  /* 24 */DI_TEXT,3,18,0,0,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,"",
  /* 25 */DI_BUTTON,0,19,0,0,0,0,DIF_CENTERGROUP,1,(char *)MOk,
  /* 26 */DI_BUTTON,0,19,0,0,0,0,DIF_CENTERGROUP,0,(char *)MCancel
  };
  MakeDialogItems(HiEditDlgData,HiEditDlg);
  struct HighlightData EditData;
  char Mask[HIGHLIGHT_MASK_SIZE], *Ptr;
  CHAR_INFO VBufColorExample[15*4];

  *Mask=0;

  if (!New && RecPos<HiDataCount)
  {
    EditData=HiData[RecPos];
    if((Ptr=GetMask(RecPos)) != NULL)
      strcpy(Mask,Ptr);
  }
  else
    memset(&EditData,0,sizeof(EditData));

  memset(VBufColorExample,0,sizeof(VBufColorExample));
  HighlightDlgUpdateUserControl(VBufColorExample,EditData.Colors);
  HiEditDlg[ID_HER_COLOREXAMPLE].VBuf=VBufColorExample;

  if(FALSE==(HiEditDlg[ID_HER_MATCHMASK].Selected=!EditData.IgnoreMask))
     HiEditDlg[ID_HER_MASKEDIT].Flags|=DIF_DISABLE;

  HiEditDlg[ID_HER_MASKEDIT].Ptr.PtrData=Mask;
  HiEditDlg[ID_HER_MASKEDIT].Ptr.PtrLength=sizeof(Mask);

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_READONLY)!=0)
    HiEditDlg[ID_HER_ATTRBR].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_READONLY)==0)
    HiEditDlg[ID_HER_ATTRBR].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_HIDDEN)!=0)
    HiEditDlg[ID_HER_ATTRBH].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_HIDDEN)==0)
    HiEditDlg[ID_HER_ATTRBH].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_SYSTEM)!=0)
    HiEditDlg[ID_HER_ATTRBS].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_SYSTEM)==0)
    HiEditDlg[ID_HER_ATTRBS].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_ARCHIVE)!=0)
    HiEditDlg[ID_HER_ATTRBA].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_ARCHIVE)==0)
    HiEditDlg[ID_HER_ATTRBA].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_COMPRESSED)!=0)
    HiEditDlg[ID_HER_ATTRBC].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_COMPRESSED)==0)
    HiEditDlg[ID_HER_ATTRBC].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_ENCRYPTED)!=0)
    HiEditDlg[ID_HER_ATTRBE].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_ENCRYPTED)==0)
    HiEditDlg[ID_HER_ATTRBE].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_DIRECTORY)!=0)
    HiEditDlg[ID_HER_ATTRBF].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_DIRECTORY)==0)
    HiEditDlg[ID_HER_ATTRBF].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_REPARSE_POINT)!=0)
    HiEditDlg[ID_HER_ATTRBL].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_REPARSE_POINT)==0)
    HiEditDlg[ID_HER_ATTRBL].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_SPARSE_FILE)!=0)
    HiEditDlg[ID_HER_ATTRBSP].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_SPARSE_FILE)==0)
    HiEditDlg[ID_HER_ATTRBSP].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_TEMPORARY)!=0)
    HiEditDlg[ID_HER_ATTRBT].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_TEMPORARY)==0)
    HiEditDlg[ID_HER_ATTRBT].Selected=2;

  if ((EditData.IncludeAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)!=0)
    HiEditDlg[ID_HER_ATTRBNI].Selected=1;
  else if ((EditData.ExcludeAttr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)==0)
    HiEditDlg[ID_HER_ATTRBNI].Selected=2;

  *HiEditDlg[ID_HER_MARKEDIT].Data=EditData.Colors.MarkChar;

  /* $ 18.05.2001 DJ
     ��������� ���������������� � ������ ���������� � ���������� �������
  */
  {
    Dialog Dlg(HiEditDlg,sizeof(HiEditDlg)/sizeof(HiEditDlg[0]),HighlightDlgProc,(long) &EditData);
    Dlg.SetHelp(HLS.HighlightEdit);
    Dlg.SetPosition(-1,-1,69,22);
    Dlg.SetAutomation(1,2,DIF_DISABLE,0,0,DIF_DISABLE);


    /* $ 06.07.2001 IS
       �������� ����� �� ������������
    */
    CFileMask FMask;
    for(;;)
    {
      Dlg.ClearDone();
      Dlg.Process();
      if (Dlg.GetExitCode() != ID_HER_OK)
        return(FALSE);
      if((FALSE!=(EditData.IgnoreMask=!HiEditDlg[ID_HER_MATCHMASK].Selected)))
      {
        if (!*Mask)
          strcpy(Mask, "*"); // ��� ������� � �� ��������� �������������
        break; // �� ��������� ����� ������ ���
      }
      if (*(char *)HiEditDlg[ID_HER_MASKEDIT].Ptr.PtrData==0)
        return(FALSE);
      if(FMask.Set(static_cast<char *>(HiEditDlg[ID_HER_MASKEDIT].Ptr.PtrData), 0))
        break;
    }
    /* IS $ */
  }
  /* DJ $ */
  EditData.IncludeAttr=EditData.ExcludeAttr=0;

  if (HiEditDlg[ID_HER_ATTRBR].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_READONLY;
  else if (HiEditDlg[ID_HER_ATTRBR].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_READONLY;

  if (HiEditDlg[ID_HER_ATTRBH].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_HIDDEN;
  else if (HiEditDlg[ID_HER_ATTRBH].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_HIDDEN;

  if (HiEditDlg[ID_HER_ATTRBS].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_SYSTEM;
  else if (HiEditDlg[ID_HER_ATTRBS].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_SYSTEM;

  if (HiEditDlg[ID_HER_ATTRBA].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_ARCHIVE;
  else if (HiEditDlg[ID_HER_ATTRBA].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_ARCHIVE;


  if (HiEditDlg[ID_HER_ATTRBE].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_ENCRYPTED;
  else if (HiEditDlg[ID_HER_ATTRBE].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_ENCRYPTED;
  if (HiEditDlg[ID_HER_ATTRBC].Selected==1)
  {
    EditData.IncludeAttr|=FILE_ATTRIBUTE_COMPRESSED;
    EditData.IncludeAttr&=~FILE_ATTRIBUTE_ENCRYPTED;
  }
  else if (HiEditDlg[ID_HER_ATTRBC].Selected==0)
  {
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_COMPRESSED;
    EditData.ExcludeAttr&=~FILE_ATTRIBUTE_COMPRESSED;
  }

  if (HiEditDlg[ID_HER_ATTRBF].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_DIRECTORY;
  else if (HiEditDlg[ID_HER_ATTRBF].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_DIRECTORY;

  if (HiEditDlg[ID_HER_ATTRBL].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_REPARSE_POINT;
  else if (HiEditDlg[ID_HER_ATTRBL].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_REPARSE_POINT;

  if (HiEditDlg[ID_HER_ATTRBSP].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_SPARSE_FILE;
  else if (HiEditDlg[ID_HER_ATTRBSP].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_SPARSE_FILE;

  if (HiEditDlg[ID_HER_ATTRBT].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_TEMPORARY;
  else if (HiEditDlg[ID_HER_ATTRBT].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_TEMPORARY;

  if (HiEditDlg[ID_HER_ATTRBNI].Selected==1)
    EditData.IncludeAttr|=FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;
  else if (HiEditDlg[ID_HER_ATTRBNI].Selected==0)
    EditData.ExcludeAttr|=FILE_ATTRIBUTE_NOT_CONTENT_INDEXED;

  EditData.Colors.MarkChar=*HiEditDlg[ID_HER_MARKEDIT].Data;

  if (!New && RecPos<HiDataCount)
  {
    if(!AddMask(HiData+RecPos,Mask,EditData.IgnoreMask,&EditData))
      return FALSE;
  }
  if (New)
    DupHighlightData(&EditData,Mask,EditData.IgnoreMask,RecPos);
  return(TRUE);
}
/* IS $ */

int HighlightFiles::DupHighlightData(struct HighlightData *EditData,char *Mask,BOOL IgnoreMask,int RecPos)
{
  struct HighlightData *NewHiData;
  struct HighlightData HData={0};
  char TmpMask[HIGHLIGHT_MASK_SIZE];

  xstrncpy(TmpMask,Mask,sizeof(TmpMask)-1);
  if(!AddMask(&HData,TmpMask,IgnoreMask,EditData))
    return FALSE;

  if ((NewHiData=(struct HighlightData *)xf_realloc(HiData,sizeof(*HiData)*(HiDataCount+1)))==NULL)
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
  /* 0 */ "*.*",
  /* 1 */ "",
  /* 2 */ "*.rar,*.zip,*.[zj],*.[bg7]z,*.[bg]zip,*.tar,*.t[ag]z,*.ar[cj],*.r[0-9][0-9],*.a[0-9][0-9],*.bz2,*.cab,*.msi,*.jar,*.lha,*.lzh,*.ha,*.ac[bei],*.pa[ck],*.rk,*.cpio,*.rpm,*.zoo,*.hqx,*.sit,*.ice,*.uc2,*.ain,*.imp,*.777,*.ufa,*.boa,*.bs[2a],*.sea,*.hpk,*.ddi,*.x2,*.rkv,*.[lw]sz,*.h[ay]p,*.lim,*.sqz,*.chz",
  /* 3 */ "*.bak,*.tmp",                                                                                                                                                                                //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ -> ����� � ���������? ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
          /* $ 25.09.2001  IS
              ��� ����� ��� ���������: ������������ ��� ��������, ����� ���, ���
              �������� ������������� (�� ����� - ��� �����).
          */
  /* 4 */ "*.*|..", // ����� ��� ���������
  /* 5 */ "..",     // ����� �������� ���������� ��� ������� �����
          /* IS $ */
  };
  /* $ 06.07.2001 IS
     ������ �������� ������ - ���������� ������ OriginalMasks
  */
  struct HighlightData  StdHighlightData[]=
  { /*
             OriginalMask                        NormalColor       SelectedCursorColor
                                IncludeAttributes       SelectedColor     MarkChar
                       FMasks           ExcludeAttributes     CursorColor             */
     /* 0 */{Masks[0], NULL, 0, 0x0002, 0x0000, {0x13, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00}},
     /* 1 */{Masks[0], NULL, 0, 0x0004, 0x0000, {0x13, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00}},
     /* 2 */{Masks[4], NULL, 0, 0x0010, 0x0000, {0x1F, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00}},
     /* 3 */{Masks[5], NULL, 0, 0x0010, 0x0000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
     /* 4 */{CmdExt,   NULL, 0, 0x0000, 0x0000, {0x1A, 0x00, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00}},
     /* 5 */{Masks[2], NULL, 0, 0x0000, 0x0000, {0x1D, 0x00, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00}},
     /* 6 */{Masks[3], NULL, 0, 0x0000, 0x0000, {0x16, 0x00, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00}},
            // ��� ��������� ��� ��������� �� ��� �������, ������� ������ ��������������
            // ��� ����� ����� (��������, ������ ������ � "far navigator")
     /* 7 */{Masks[0], NULL, 1, 0x0010, 0x0000, {0x1F, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00}},
  };

  // ��� NT ��������� CMD
  if(WinVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
    strcat(CmdExt,",*.cmd");

  Ptr=MkRegKeyHighlightName(RegKey);
  for(I=0; I < sizeof(StdHighlightData)/sizeof(StdHighlightData[0]); ++I)
  {
    itoa(I,Ptr,10);
    SetRegKey(RegKey,HLS.Mask,StdHighlightData[I].OriginalMasks);
    SetRegKey(RegKey,HLS.IgnoreMask,StdHighlightData[I].IgnoreMask);
    if(StdHighlightData[I].IncludeAttr)
      SetRegKey(RegKey,HLS.IncludeAttributes,StdHighlightData[I].IncludeAttr);
    if(StdHighlightData[I].ExcludeAttr)
      SetRegKey(RegKey,HLS.ExcludeAttributes,StdHighlightData[I].ExcludeAttr);
    if(StdHighlightData[I].Colors.Color)
      SetRegKey(RegKey,HLS.NormalColor,StdHighlightData[I].Colors.Color);
    if(StdHighlightData[I].Colors.SelColor)
      SetRegKey(RegKey,HLS.SelectedColor,StdHighlightData[I].Colors.SelColor);
    if(StdHighlightData[I].Colors.CursorColor)
      SetRegKey(RegKey,HLS.CursorColor,StdHighlightData[I].Colors.CursorColor);
    if(StdHighlightData[I].Colors.CursorSelColor)
      SetRegKey(RegKey,HLS.SelectedCursorColor,StdHighlightData[I].Colors.CursorSelColor);
    if(StdHighlightData[I].Colors.MarkChar)
      SetRegKey(RegKey,HLS.MarkChar,StdHighlightData[I].Colors.MarkChar);
  }
}
