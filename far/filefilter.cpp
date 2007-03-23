/*
filefilter.cpp

�������� ������

*/

#include "headers.hpp"
#pragma hdrstop

#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "filepanels.hpp"
#include "panel.hpp"
#include "vmenu.hpp"
#include "scantree.hpp"
#include "filefilter.hpp"
#include "array.hpp"
#include "filelist.hpp"

static int _cdecl ExtSort(const void *el1,const void *el2);

static TPointerArray<FileFilterParams> FilterData, TempFilterData;
static FileFilterParams FoldersFilter;
static BitFlags FolderFlags; //����� ��� ���������� ���������� �����

FileFilter::FileFilter(Panel *HostPanel, enumFileFilterType FilterType)
{
  m_HostPanel=HostPanel;
  m_FilterType=FilterType;
}

FileFilter::~FileFilter()
{
}

bool FileFilter::FilterEdit()
{
  struct MenuItemEx ListItem;
  int ExitCode;
  bool bNeedUpdate=false;
  VMenu FilterList(L"",NULL,0,ScrY-6);

  {
    DWORD Inc,Exc;
    GetIncludeExcludeFlags(Inc,Exc);
    if (FolderFlags.Check(Inc))
      FilterList.SetTitle(UMSG(MFilterTitle_IncFolders));
    else if (FolderFlags.Check(Exc))
      FilterList.SetTitle(UMSG(MFilterTitle_ExcFolders));
    else
      FilterList.SetTitle(UMSG(MFilterTitle_FilterFolders));
  }

  FilterList.SetHelp(L"FiltersMenu");
  FilterList.SetPosition(-1,-1,0,0);
  FilterList.SetBottomTitle(UMSG(MFilterBottom));
  FilterList.SetFlags(VMENU_SHOWAMPERSAND|VMENU_WRAPMODE);

  for (unsigned int i=0; i<FilterData.getCount(); i++)
  {
    ListItem.Clear();
    MenuString(ListItem.strName,FilterData.getItem(i));

    if(i == 0)
      ListItem.Flags|=LIF_SELECTED;

    int Check = GetCheck(FilterData.getItem(i));
    if (Check)
      ListItem.SetCheck(Check);

    FilterList.AddItemW(&ListItem);
  }

  ListItem.Clear();
  if (FilterData.getCount()==0)
    ListItem.Flags|=LIF_SELECTED;
  FilterList.AddItemW(&ListItem);

  wchar_t *ExtPtr=NULL;
  int ExtCount=0;

  {
    DWORD Inc,Exc;
    GetIncludeExcludeFlags(Inc,Exc);
    for (unsigned int i=0; i<TempFilterData.getCount(); i++)
    {
      //AY: ����� ���������� ������ �� ��������� ���� �������
      //(��� ������� ���� ������ �� ������) ������� ������� � ������� ������� ����
      if (!TempFilterData.getItem(i)->Flags.Check(Inc|Exc))
        continue;
      const wchar_t *FMask;
      TempFilterData.getItem(i)->GetMask(&FMask);
      string strMask = FMask;
      UnquoteW(strMask);
      if(!ParseAndAddMasks(&ExtPtr,strMask,0,ExtCount,GetCheck(TempFilterData.getItem(i))))
        break;
    }
  }

  ListItem.Clear();
  ListItem.Flags|=LIF_SEPARATOR;
  FilterList.AddItemW(&ListItem);

  ListItem.Clear();
  FoldersFilter.SetTitle(UMSG(MFolderFileType));
  MenuString(ListItem.strName,&FoldersFilter);
  int Check = GetCheck(&FoldersFilter);
  if (Check)
    ListItem.SetCheck(Check);
  FilterList.AddItemW(&ListItem);

  if (m_HostPanel->GetMode()==NORMAL_PANEL)
  {
    string strCurDir, strFileName;
    FAR_FIND_DATA_EX fdata;

    m_HostPanel->GetCurDirW(strCurDir);

    ScanTree ScTree(FALSE,FALSE);
    ScTree.SetFindPathW(strCurDir,L"*.*");
    while (ScTree.GetNextNameW(&fdata,strFileName))
      if(!ParseAndAddMasks(&ExtPtr,fdata.strFileName,fdata.dwFileAttributes,ExtCount,0))
        break;
  }
  else
  {
    string strFileName;
    int FileAttr;
    for (int i=0; m_HostPanel->GetFileNameW(strFileName,i,FileAttr); i++)
      if(!ParseAndAddMasks(&ExtPtr,strFileName,FileAttr,ExtCount,0))
        break;
  }

  far_qsort((void *)ExtPtr,ExtCount,NM*sizeof(wchar_t),ExtSort);

  ListItem.Clear();

  for (int i=0; i<ExtCount; i++)
  {
    wchar_t *CurExtPtr=ExtPtr+i*NM;
    MenuString(ListItem.strName,NULL,false,true,CurExtPtr,UMSG(MPanelFileType));
    ListItem.SetCheck(CurExtPtr[wcslen(CurExtPtr)+1]);
    FilterList.SetUserData(CurExtPtr,0,FilterList.AddItemW(&ListItem));
  }
  xf_free(ExtPtr);

  FilterList.Show();

  while (!FilterList.Done())
  {
    int Key=FilterList.ReadInput();

    if (Key==KEY_ADD)
      Key=L'+';
    else if (Key==KEY_SUBTRACT)
      Key=L'-';

    switch(Key)
    {
      case KEY_SPACE:
      case L'+':
      case L'-':
      case KEY_BS:
      {
        int SelPos=FilterList.GetSelectPos();

        if (SelPos==FilterData.getCount())
          break;

        int Check=FilterList.GetSelection(SelPos),NewCheck;
        if (Key==L'-')
          NewCheck=(Check==L'-') ? 0:L'-';
        else if (Key==L'+')
          NewCheck=(Check==L'+') ? 0:L'+';
        else if (Key==KEY_BS)
          NewCheck=0;
        else
          NewCheck=Check ? 0:L'+';

        FilterList.SetSelection(NewCheck,SelPos);
        FilterList.SetSelectPos(SelPos,1);
        FilterList.SetUpdateRequired(TRUE);
        FilterList.FastShow();
        FilterList.ProcessKey(KEY_DOWN);
        break;
      }

      case KEY_SHIFTSUBTRACT:
      case KEY_SHIFTBS:
      {
        for (int I=0; I < FilterList.GetItemCount(); I++)
        {
          FilterList.SetSelection(FALSE, I);
        }
        FilterList.SetUpdateRequired(TRUE);
        FilterList.FastShow();
        if (Key!=KEY_SHIFTSUBTRACT)
          break;
      }
      case KEY_CTRLF:
      {
        DWORD Inc,Exc;
        GetIncludeExcludeFlags(Inc,Exc);
        if (Key==KEY_CTRLF)
        {
          if (m_FilterType == FFT_SELECT)
            FolderFlags.Swap(Exc);
          else
            FolderFlags.Swap(Inc);
        }
        else
        {
          if (m_FilterType == FFT_SELECT)
            FolderFlags.Set(Exc);
          else
            FolderFlags.Set(Inc);
        }
        if (FolderFlags.Check(Inc))
          FilterList.SetTitle(UMSG(MFilterTitle_IncFolders));
        else if (FolderFlags.Check(Exc))
          FilterList.SetTitle(UMSG(MFilterTitle_ExcFolders));
        else
          FilterList.SetTitle(UMSG(MFilterTitle_FilterFolders));
        FilterList.SetUpdateRequired(TRUE);
        FilterList.SetPosition(-1,-1,0,0);
        FilterList.Show();
        bNeedUpdate=true;
        break;
      }

      case KEY_F4:
      {
        int SelPos=FilterList.GetSelectPos();
        if (SelPos<(int)FilterData.getCount())
        {
          if (FileFilterConfig(FilterData.getItem(SelPos)))
          {
            ListItem.Clear();
            MenuString(ListItem.strName,FilterData.getItem(SelPos));
            int Check = GetCheck(FilterData.getItem(SelPos));
            if (Check)
              ListItem.SetCheck(Check);

            FilterList.DeleteItem(SelPos);
            FilterList.AddItemW(&ListItem,SelPos);

            FilterList.AdjustSelectPos();
            FilterList.SetSelectPos(SelPos,1);
            FilterList.SetUpdateRequired(TRUE);
            FilterList.FastShow();
            bNeedUpdate=true;
          }
        }
        else if (SelPos>(int)FilterData.getCount())
        {
          MessageW(MSG_WARNING,1,UMSG(MFilterTitle),UMSG(MCanEditCustomFilterOnly),UMSG(MOk));
        }
        break;
      }

      case KEY_INS:
      case KEY_F5:
      {
        int SelPos=FilterList.GetSelectPos();
        int SelPos2=SelPos+1;

        if (SelPos>(int)FilterData.getCount())
          SelPos=FilterData.getCount();

        FileFilterParams *NewFilter = FilterData.insertItem(SelPos);
        if (!NewFilter)
          break;

        if (Key==KEY_F5)
        {
          if (SelPos2 < (int)FilterData.getCount())
          {
            *NewFilter = *FilterData.getItem(SelPos2);

            NewFilter->SetTitle(L"");
            NewFilter->Flags.ClearAll();
          }
          else if (SelPos2 == (FilterData.getCount()+2))
          {
            *NewFilter = FoldersFilter;

            NewFilter->SetTitle(L"");
            NewFilter->Flags.ClearAll();
          }
          else if (SelPos2 > (int)(FilterData.getCount()+2))
          {
            wchar_t Mask[NM];
            FilterList.GetUserData(Mask,sizeof(Mask),SelPos2-1);

            NewFilter->SetMask(1,Mask);
            //���� ������� ��� ������ ��� ������, ����� �� ������ � ��� ���������
            NewFilter->SetAttr(1,0,FILE_ATTRIBUTE_DIRECTORY);
          }
          else
          {
            FilterData.deleteItem(SelPos);
            break;
          }
        }
        else
        {
          //AY: ��� ������ ����� ������ �� ����� ����� ������� ���� �� ����� ������ ��� ������
          NewFilter->SetAttr(1,0,FILE_ATTRIBUTE_DIRECTORY);
        }

        if (FileFilterConfig(NewFilter))
        {
          ListItem.Clear();
          MenuString(ListItem.strName,NewFilter);

          FilterList.AddItemW(&ListItem,SelPos);

          FilterList.AdjustSelectPos();
          FilterList.SetSelectPos(SelPos,1);
          FilterList.SetPosition(-1,-1,0,0);
          FilterList.Show();
          bNeedUpdate=true;
        }
        else
          FilterData.deleteItem(SelPos);

        break;
      }

      case KEY_DEL:
      {
        int SelPos=FilterList.GetSelectPos();
        if (SelPos<(int)FilterData.getCount())
        {
          string strQuotedTitle;
          strQuotedTitle.Format(L"\"%s\"",FilterData.getItem(SelPos)->GetTitle());
          if (MessageW(0,2,UMSG(MFilterTitle),UMSG(MAskDeleteFilter),
                       (const wchar_t *)strQuotedTitle,UMSG(MDelete),UMSG(MCancel))==0)
          {
            FilterData.deleteItem(SelPos);

            FilterList.DeleteItem(SelPos);

            FilterList.AdjustSelectPos();
            FilterList.SetSelectPos(SelPos,1);
            FilterList.SetPosition(-1,-1,0,0);
            FilterList.Show();
            bNeedUpdate=true;
          }
        }
        else if (SelPos>(int)FilterData.getCount())
        {
          MessageW(MSG_WARNING,1,UMSG(MFilterTitle),UMSG(MCanDeleteCustomFilterOnly),UMSG(MOk));
        }
        break;
      }

      case KEY_CTRLUP:
      case KEY_CTRLDOWN:
      {
        int SelPos=FilterList.GetSelectPos();
        if (SelPos<(int)FilterData.getCount() && !(Key==KEY_CTRLUP && SelPos==0) && !(Key==KEY_CTRLDOWN && SelPos==FilterData.getCount()-1))
        {
          int NewPos = SelPos + (Key == KEY_CTRLDOWN ? 1 : -1);
          MenuItemEx CurItem, NextItem;
          CurItem = *FilterList.GetItemPtr(SelPos);
          NextItem = *FilterList.GetItemPtr(NewPos);

          FilterData.swapItems(NewPos,SelPos);

          if (NewPos<SelPos)
          {
            FilterList.DeleteItem(NewPos,2);
            FilterList.AddItemW(&CurItem,NewPos);
            FilterList.AddItemW(&NextItem,SelPos);
          }
          else
          {
            FilterList.DeleteItem(SelPos,2);
            FilterList.AddItemW(&NextItem,SelPos);
            FilterList.AddItemW(&CurItem,NewPos);
          }

          FilterList.AdjustSelectPos();
          FilterList.SetSelectPos(NewPos,1);
          FilterList.SetUpdateRequired(TRUE);
          FilterList.FastShow();
          bNeedUpdate=true;
        }
        break;
      }

      default:
      {
        FilterList.ProcessInput();
      }
    }
  }

  ExitCode=FilterList.Modal::GetExitCode();

  if (ExitCode!=-1)
    ProcessSelection(&FilterList);

  if (ExitCode!=-1 || bNeedUpdate)
  {
    SaveFilters(false);
    if (m_FilterType == FFT_PANEL)
    {
      m_HostPanel->Update(UPDATE_KEEP_SELECTION);
      m_HostPanel->Redraw();
    }
  }

  return (ExitCode!=-1);
}

void FileFilter::GetIncludeExcludeFlags(DWORD &Inc, DWORD &Exc)
{
  if (m_FilterType == FFT_PANEL)
  {
    if (m_HostPanel==CtrlObject->Cp()->RightPanel)
    {
      Inc = FFF_RPANELINCLUDE;
      Exc = FFF_RPANELEXCLUDE;
    }
    else
    {
      Inc = FFF_LPANELINCLUDE;
      Exc = FFF_LPANELEXCLUDE;
    }
  }
  else if (m_FilterType == FFT_COPY)
  {
    Inc = FFF_COPYINCLUDE;
    Exc = FFF_COPYEXCLUDE;
  }
  else if (m_FilterType == FFT_FINDFILE)
  {
    Inc = FFF_FINDFILEINCLUDE;
    Exc = FFF_FINDFILEEXCLUDE;
  }
  else
  {
    Inc = FFF_SELECTINCLUDE;
    Exc = FFF_SELECTEXCLUDE;
  }
}

int FileFilter::GetCheck(FileFilterParams *FFP)
{
  DWORD Inc,Exc;
  GetIncludeExcludeFlags(Inc,Exc);

  if (FFP->Flags.Check(Inc))
    return L'+';
  else if (FFP->Flags.Check(Exc))
    return L'-';

  return 0;
}

void FileFilter::ProcessSelection(VMenu *FilterList)
{
  DWORD Inc,Exc;
  GetIncludeExcludeFlags(Inc,Exc);

  FileFilterParams *CurFilterData;
  for (int i=0,j=0; i < FilterList->GetItemCount(); i++)
  {
    int Check=FilterList->GetSelection(i);

    CurFilterData=NULL;

    if (i < (int)FilterData.getCount())
    {
      CurFilterData = FilterData.getItem(i);
    }
    else if (i == (FilterData.getCount() + 2))
    {
      CurFilterData = &FoldersFilter;
    }
    else if (i > (int)(FilterData.getCount() + 2))
    {
      const wchar_t *FMask;
      wchar_t Mask[NM];
      string strMask1;
      FilterList->GetUserData(Mask,sizeof(Mask),i);

      //AY: ��� ��� � ���� �� ���������� ������ �� ��������� ���� �������
      //������� ������� � ������� ������� ���� � TempFilterData ������
      //����� ��������� ����� ������� ���� ��� ������� � ���� ���� ��
      //��� ��� ���� ������� � ������ � ��� ��� TempFilterData
      //� ���� ������� � ���� ������������� �� �������� �� �������
      //��������� ���� �� ���� ���������� � ������.
      strMask1 = Mask;
      UnquoteW(strMask1);
      while ((CurFilterData=TempFilterData.getItem(j))!=NULL)
      {
        string strMask2;
        CurFilterData->GetMask(&FMask);
        strMask2 = FMask;
        UnquoteW(strMask2);
        if (LocalStricmpW(strMask1,strMask2)<1)
          break;
        j++;
      }

      if (CurFilterData)
      {
        if (!LocalStricmpW(Mask,FMask))
        {
          if (!Check && !CurFilterData->Flags.Check(~(Inc|Exc)))
          {
            TempFilterData.deleteItem(j);
            continue;
          }
          else
            j++;
        }
        else
          CurFilterData=NULL;
      }

      if (Check && !CurFilterData)
      {
        FileFilterParams *NewFilter = TempFilterData.insertItem(j);

        if (NewFilter)
        {
          NewFilter->SetMask(1,Mask);
          //���� ������� ��� ������ ��� ������, ����� �� ������ � ��� ���������
          NewFilter->SetAttr(1,0,FILE_ATTRIBUTE_DIRECTORY);

          j++;
          CurFilterData = NewFilter;
        }
        else
          continue;
      }
    }

    if (!CurFilterData)
      continue;

    CurFilterData->Flags.Clear(Inc|Exc);
    if (Check==L'+')
      CurFilterData->Flags.Set(Inc);
    else if (Check==L'-')
      CurFilterData->Flags.Set(Exc);
  }
}

bool FileFilter::FileInFilter(FileListItem *fli)
{
  FAR_FIND_DATA fd;

  fd.dwFileAttributes=fli->FileAttr;
  fd.ftCreationTime=fli->CreationTime;
  fd.ftLastAccessTime=fli->AccessTime;
  fd.ftLastWriteTime=fli->WriteTime;
  fd.nFileSize=fli->UnpSize;
  fd.nPackSize=fli->PackSize;
  fd.lpwszFileName=(wchar_t *)(const wchar_t *)fli->strName;
  fd.lpwszAlternateFileName=(wchar_t *)(const wchar_t *)fli->strShortName;

  return FileInFilter(&fd);
}

bool FileFilter::FileInFilter(const FAR_FIND_DATA_EX *fde)
{
  FAR_FIND_DATA fd;

  fd.dwFileAttributes=fde->dwFileAttributes;
  fd.ftCreationTime=fde->ftCreationTime;
  fd.ftLastAccessTime=fde->ftLastAccessTime;
  fd.ftLastWriteTime=fde->ftLastWriteTime;
  fd.nFileSize=fde->nFileSize;
  fd.nPackSize=fde->nPackSize;
  fd.lpwszFileName=(wchar_t *)(const wchar_t *)fde->strFileName;
  fd.lpwszAlternateFileName=(wchar_t *)(const wchar_t *)fde->strAlternateFileName;

  return FileInFilter(&fd);
}

bool FileFilter::FileInFilter(const FAR_FIND_DATA *fd)
{
  DWORD Inc,Exc;
  GetIncludeExcludeFlags(Inc,Exc);

  if ((fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
  {
    if (FolderFlags.Check(Inc))
      return true;
    if (FolderFlags.Check(Exc))
      return false;
  }

  bool flag=false;
  bool bInc=false;
  bool bExc=false;
  FileFilterParams *CurFilterData;

  for (unsigned int i=0; i<FilterData.getCount(); i++)
  {
    CurFilterData = FilterData.getItem(i);

    if (CurFilterData->Flags.Check(Inc|Exc))
    {
      flag = flag || CurFilterData->Flags.Check(Inc);
      if (CurFilterData->FileInFilter(fd))
        CurFilterData->Flags.Check(Inc)?bInc=true:bExc=true;
    }
  }

  if (FoldersFilter.Flags.Check(Inc|Exc))
  {
    flag = flag || FoldersFilter.Flags.Check(Inc);
    if (FoldersFilter.FileInFilter(fd))
      FoldersFilter.Flags.Check(Inc)?bInc=true:bExc=true;
  }

  for (unsigned int i=0; i<TempFilterData.getCount(); i++)
  {
    CurFilterData = TempFilterData.getItem(i);

    if (CurFilterData->Flags.Check(Inc|Exc))
    {
      flag = flag || CurFilterData->Flags.Check(Inc);
      if (CurFilterData->FileInFilter(fd))
        CurFilterData->Flags.Check(Inc)?bInc=true:bExc=true;
    }
  }

  if (bExc) return false;
  if (bInc) return true;
  return !flag;
}

bool FileFilter::IsEnabledOnPanel()
{
  if (m_FilterType != FFT_PANEL)
    return false;

  DWORD Inc,Exc;
  GetIncludeExcludeFlags(Inc,Exc);

  for (unsigned int i=0; i<FilterData.getCount(); i++)
  {
    if (FilterData.getItem(i)->Flags.Check(Inc|Exc))
      return true;
  }

  if (FoldersFilter.Flags.Check(Inc|Exc))
    return true;

  for (unsigned int i=0; i<TempFilterData.getCount(); i++)
  {
    if (TempFilterData.getItem(i)->Flags.Check(Inc|Exc))
      return true;
  }

  return false;
}

void FileFilter::InitFilter()
{
  FilterData.Free();
  TempFilterData.Free();

  string strRegKey;
  string strTitle, strMask;

  while (1)
  {
    strRegKey.Format(L"Filters\\Filter%d",FilterData.getCount());

    if (!GetRegKeyW(strRegKey,L"Title",strTitle,L""))
      break;

    FileFilterParams *NewFilter = FilterData.addItem();

    if (NewFilter)
    {
      //��������� �������� ������� ��� ���� ��� ����� ���������� ���������
      //��������� ������ ������ ����.

      NewFilter->SetTitle(strTitle);

      GetRegKeyW(strRegKey,L"Mask",strMask,L"");
      NewFilter->SetMask((DWORD)GetRegKeyW(strRegKey,L"UseMask",1),
                         strMask);

      FILETIME DateAfter, DateBefore;
      GetRegKeyW(strRegKey,L"DateAfter",(BYTE *)&DateAfter,NULL,sizeof(DateAfter));
      GetRegKeyW(strRegKey,L"DateBefore",(BYTE *)&DateBefore,NULL,sizeof(DateBefore));
      NewFilter->SetDate((DWORD)GetRegKeyW(strRegKey,L"UseDate",0),
                         (DWORD)GetRegKeyW(strRegKey,L"DateType",0),
                         DateAfter,
                         DateBefore);

      NewFilter->SetSize((DWORD)GetRegKeyW(strRegKey,L"UseSize",0),
                         (DWORD)GetRegKeyW(strRegKey,L"SizeType",0),
                         GetRegKey64W(strRegKey,L"SizeAbove",(unsigned __int64)_i64(-1)),
                         GetRegKey64W(strRegKey,L"SizeBelow",(unsigned __int64)_i64(-1)));

      NewFilter->SetAttr((DWORD)GetRegKeyW(strRegKey,L"UseAttr",1),
                         (DWORD)GetRegKeyW(strRegKey,L"AttrSet",0),
                         (DWORD)GetRegKeyW(strRegKey,L"AttrClear",FILE_ATTRIBUTE_DIRECTORY));

      NewFilter->Flags.Set((DWORD)GetRegKeyW(strRegKey,L"Flags",0));
    }
    else
      break;
  }

  while (1)
  {
    strRegKey.Format(L"Filters\\PanelMask%d",TempFilterData.getCount());

    if (!GetRegKeyW(strRegKey,L"Mask",strMask,L""))
      break;

    FileFilterParams *NewFilter = TempFilterData.addItem();

    if (NewFilter)
    {
      NewFilter->SetMask(1,strMask);
      //���� ������� ��� ������ ��� ������, ����� �� ������ � ��� ���������
      NewFilter->SetAttr(1,0,FILE_ATTRIBUTE_DIRECTORY);

      NewFilter->Flags.Set((DWORD)GetRegKeyW(strRegKey,L"Flags",0));
    }
    else
      break;
  }

  FoldersFilter.SetMask(0,L"");
  FoldersFilter.SetAttr(1,FILE_ATTRIBUTE_DIRECTORY,0);
  FoldersFilter.Flags.Set((DWORD)GetRegKeyW(L"Filters",L"FoldersFilterFlags",0));

  FolderFlags.Set((DWORD)GetRegKeyW(L"Filters",L"FolderFlags",FFF_RPANELINCLUDE|FFF_LPANELINCLUDE|FFF_FINDFILEINCLUDE|FFF_COPYINCLUDE|FFF_SELECTEXCLUDE));
}


void FileFilter::CloseFilter()
{
  FilterData.Free();
  TempFilterData.Free();
}

void FileFilter::SaveFilters(bool SaveAll)
{
  string strRegKey;

  DeleteKeyTreeW(L"Filters");

  FileFilterParams *CurFilterData;

  for (unsigned int i=0; i<FilterData.getCount(); i++)
  {
    strRegKey.Format(L"Filters\\Filter%d",i);

    CurFilterData = FilterData.getItem(i);

    SetRegKeyW(strRegKey,L"Title",CurFilterData->GetTitle());

    const wchar_t *Mask;
    SetRegKeyW(strRegKey,L"UseMask",CurFilterData->GetMask(&Mask));
    SetRegKeyW(strRegKey,L"Mask",Mask);


    DWORD DateType;
    FILETIME DateAfter, DateBefore;
    SetRegKeyW(strRegKey,L"UseDate",CurFilterData->GetDate(&DateType, &DateAfter, &DateBefore));
    SetRegKeyW(strRegKey,L"DateType",DateType);
    SetRegKeyW(strRegKey,L"DateAfter",(BYTE *)&DateAfter,sizeof(DateAfter));
    SetRegKeyW(strRegKey,L"DateBefore",(BYTE *)&DateBefore,sizeof(DateBefore));


    DWORD SizeType;
    __int64 SizeAbove, SizeBelow;
    SetRegKeyW(strRegKey,L"UseSize",CurFilterData->GetSize(&SizeType, &SizeAbove, &SizeBelow));
    SetRegKeyW(strRegKey,L"SizeType",SizeType);
    SetRegKey64W(strRegKey,L"SizeAbove",SizeAbove);
    SetRegKey64W(strRegKey,L"SizeBelow",SizeBelow);


    DWORD AttrSet, AttrClear;
    SetRegKeyW(strRegKey,L"UseAttr",CurFilterData->GetAttr(&AttrSet, &AttrClear));
    SetRegKeyW(strRegKey,L"AttrSet",AttrSet);
    SetRegKeyW(strRegKey,L"AttrClear",AttrClear);

    SetRegKeyW(strRegKey,L"Flags",SaveAll ? CurFilterData->Flags.Flags : 0);
  }

  if (SaveAll)
  {
    for (unsigned int i=0; i<TempFilterData.getCount(); i++)
    {
      strRegKey.Format(L"Filters\\PanelMask%d",i);

      CurFilterData = TempFilterData.getItem(i);

      const wchar_t *Mask;
      CurFilterData->GetMask(&Mask);
      SetRegKeyW(strRegKey,L"Mask",Mask);

      SetRegKeyW(strRegKey,L"Flags",CurFilterData->Flags.Flags);
    }

    SetRegKeyW(L"Filters",L"FoldersFilterFlags",FoldersFilter.Flags.Flags);

    SetRegKeyW(L"Filters",L"FolderFlags",FolderFlags.Flags);
  }
}

void FileFilter::SwapPanelFlags(FileFilterParams *CurFilterData)
{
  DWORD flags=0;

  if (CurFilterData->Flags.Check(FFF_LPANELINCLUDE))
  {
    flags|=FFF_RPANELINCLUDE;
  }
  if (CurFilterData->Flags.Check(FFF_RPANELINCLUDE))
  {
    flags|=FFF_LPANELINCLUDE;
  }
  if (CurFilterData->Flags.Check(FFF_LPANELEXCLUDE))
  {
    flags|=FFF_RPANELEXCLUDE;
  }
  if (CurFilterData->Flags.Check(FFF_RPANELEXCLUDE))
  {
    flags|=FFF_LPANELEXCLUDE;
  }

  CurFilterData->Flags.Clear(FFF_RPANELEXCLUDE|FFF_LPANELEXCLUDE|FFF_RPANELINCLUDE|FFF_LPANELINCLUDE);
  CurFilterData->Flags.Set(flags);
}

void FileFilter::SwapFilter()
{
  for (unsigned int i=0; i<FilterData.getCount(); i++)
    SwapPanelFlags(FilterData.getItem(i));

  SwapPanelFlags(&FoldersFilter);

  for (unsigned int i=0; i<TempFilterData.getCount(); i++)
    SwapPanelFlags(TempFilterData.getItem(i));

  DWORD flags=0;
  if (FolderFlags.Check(FFF_LPANELINCLUDE))
  {
    flags|=FFF_RPANELINCLUDE;
  }
  if (FolderFlags.Check(FFF_RPANELINCLUDE))
  {
    flags|=FFF_LPANELINCLUDE;
  }
  FolderFlags.Clear(FFF_RPANELINCLUDE|FFF_LPANELINCLUDE);
  FolderFlags.Set(flags);
}

int FileFilter::ParseAndAddMasks(wchar_t **ExtPtr,const wchar_t *FileName,DWORD FileAttr,int& ExtCount,int Check)
{
  if (!wcscmp(FileName,L".") || TestParentFolderNameW(FileName) || (FileAttr & FA_DIREC))
    return -1;

  const wchar_t *DotPtr=wcsrchr(FileName,L'.');
  string strMask;
  /* $ 01.07.2001 IS
     ���� ����� �������� ����������� (',' ��� ';'), �� ������� �� �
     �������
  */
  if (DotPtr==NULL)
    strMask = L"*.";
  else if(wcspbrk(DotPtr,L",;"))
    strMask.Format(L"\"*%s\"",DotPtr);
  else
    strMask.Format(L"*%s",DotPtr);
  /* IS $ */

  // ������� �����...
  unsigned int Cnt=ExtCount;
  if(lfind((const void *)(const wchar_t *)strMask,(void *)*ExtPtr,&Cnt,NM*sizeof(wchar_t),ExtSort))
    return -1;

  // ... � ����� ��� ��������� ������!
  wchar_t *NewPtr;
  if ((NewPtr=(wchar_t *)xf_realloc(*ExtPtr,NM*(ExtCount+1)*sizeof(wchar_t))) == NULL)
    return 0;
  *ExtPtr=NewPtr;

  NewPtr=*ExtPtr+ExtCount*NM;
  xwcsncpy(NewPtr,strMask,NM-2);

  NewPtr=NewPtr+wcslen(NewPtr)+1;
  *NewPtr=Check;

  ExtCount++;

  return 1;
}

int _cdecl ExtSort(const void *el1,const void *el2)
{
  return LocalStricmpW((const wchar_t *)el1,(const wchar_t *)el2);
}
