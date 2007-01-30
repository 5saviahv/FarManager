/*
filefilterparams.cpp

��������� ��������� �������

*/

#include "headers.hpp"
#pragma hdrstop

#include "colors.hpp"
#include "CFileMask.hpp"
#include "global.hpp"
#include "fn.hpp"
#include "lang.hpp"
#include "keys.hpp"
#include "ctrlobj.hpp"
#include "dialog.hpp"
#include "filelist.hpp"
#include "filefilterparams.hpp"

FileFilterParams::FileFilterParams()
{
  *m_Title=0;
  SetMask(1,"*");
  SetSize(0,FSIZE_INBYTES,_i64(-1),_i64(-1));
  memset(&FDate,0,sizeof(FDate));
  memset(&FAttr,0,sizeof(FAttr));
  memset(&m_Colors,0,sizeof(m_Colors));
  m_SortGroup=DEFAULT_SORT_GROUP;
  Flags.ClearAll();
}

const FileFilterParams &FileFilterParams::operator=(const FileFilterParams &FF)
{
  SetTitle(FF.GetTitle());
  const char *Mask;
  FF.GetMask(&Mask);
  SetMask(FF.GetMask(NULL),Mask);
  memcpy(&FSize,&FF.FSize,sizeof(FSize));
  memcpy(&FDate,&FF.FDate,sizeof(FDate));
  memcpy(&FAttr,&FF.FAttr,sizeof(FAttr));
  FF.GetColors(&m_Colors);
  m_SortGroup=FF.GetSortGroup();
  Flags.Flags=FF.Flags.Flags;
  return *this;
}

void FileFilterParams::SetTitle(const char *Title)
{
  xstrncpy(m_Title,Title,sizeof(m_Title)-1);
}

void FileFilterParams::SetMask(DWORD Used, const char *Mask)
{
  FMask.Used = Used;
  xstrncpy(FMask.Mask,Mask,sizeof(FMask.Mask)-1);

  /* ��������� %PATHEXT% */
  char CheckMask[FILEFILTER_MASK_SIZE];
  xstrncpy(CheckMask,Mask,sizeof(CheckMask)-1);
  char *Ptr;
  // ��������
  if((Ptr=strchr(CheckMask,'%')) != NULL && !strnicmp(Ptr,"%PATHEXT%",9))
  {
    int IQ1=(*(Ptr+9) == ',')?10:9, offsetPtr=Ptr-CheckMask;
    // ���� ����������� %pathext%, �� ������� � �����...
    memmove(Ptr,Ptr+IQ1,strlen(Ptr+IQ1)+1);

    char Tmp1[FILEFILTER_MASK_SIZE], *pSeparator;
    xstrncpy(Tmp1, CheckMask, sizeof(Tmp1)-1);
    pSeparator=strchr(Tmp1, EXCLUDEMASKSEPARATOR);
    if(pSeparator)
    {
      Ptr=Tmp1+offsetPtr;
      if(Ptr>pSeparator) // PATHEXT ��������� � ������ ����������
        Add_PATHEXT(CheckMask); // ��������� ��, ���� ����.
      else
      {
        char Tmp2[FILEFILTER_MASK_SIZE];
        xstrncpy(Tmp2, pSeparator+1,sizeof(Tmp2)-1);
        *pSeparator=0;
        Add_PATHEXT(Tmp1);
        sprintf(CheckMask, "%s|%s", Tmp1, Tmp2);
      }
    }
    else
      Add_PATHEXT(CheckMask); // ��������� ��, ���� ����.
  }

  // �������� �� ���������� ������� �������� �������
  if ((*CheckMask==0) || (!FMask.FilterMask.Set(CheckMask,FMF_SILENT)))
  {
    xstrncpy(FMask.Mask,"*",sizeof(FMask.Mask)-1);
    FMask.FilterMask.Set(FMask.Mask,FMF_SILENT);
  }
}

void FileFilterParams::SetDate(DWORD Used, DWORD DateType, FILETIME DateAfter, FILETIME DateBefore)
{
  FDate.Used=Used;
  FDate.DateType=(FDateType)DateType;
  if (DateType>=FDATE_COUNT)
    FDate.DateType=FDATE_MODIFIED;
  FDate.DateAfter=DateAfter;
  FDate.DateBefore=DateBefore;
}

void FileFilterParams::SetSize(DWORD Used, DWORD SizeType, __int64 SizeAbove, __int64 SizeBelow)
{
  FSize.Used=Used;
  FSize.SizeType=(FSizeType)SizeType;
  if (SizeType>=FSIZE_COUNT)
    FSize.SizeType=FSIZE_INBYTES;
  FSize.SizeAbove=SizeAbove;
  FSize.SizeBelow=SizeBelow;
  FSize.SizeAboveReal=SizeAbove;
  FSize.SizeBelowReal=SizeBelow;
  switch (FSize.SizeType)
  {
    case FSIZE_INBYTES:
      // ������ ����� � ������, ������ ������ �� ������.
      break;
    case FSIZE_INKBYTES:
      // ������ ����� � ����������, �������� ��� � �����.
      // !!! �������� �� ���������� ������������� �������� �� �������� !!!
      FSize.SizeAboveReal<<=10;
      FSize.SizeBelowReal<<=10;
      break;
    case FSIZE_INMBYTES:
      // ����� // ������ ����� � ����������, �������� ��� � �����.
      // !!! �������� �� ���������� ������������� �������� �� �������� !!!
      FSize.SizeAboveReal<<=20;
      FSize.SizeBelowReal<<=20;
      break;
    case FSIZE_INGBYTES:
      // ����� // ������ ����� � ����������, �������� ��� � �����.
      // !!! �������� �� ���������� ������������� �������� �� �������� !!!
      FSize.SizeAboveReal<<=30;
      FSize.SizeBelowReal<<=30;
      break;
  }
}

void FileFilterParams::SetAttr(DWORD Used, DWORD AttrSet, DWORD AttrClear)
{
  FAttr.Used=Used;
  FAttr.AttrSet=AttrSet;
  FAttr.AttrClear=AttrClear;
}

void FileFilterParams::SetColors(HighlightDataColor *Colors)
{
  memcpy(&m_Colors,Colors,sizeof(m_Colors));
}

const char *FileFilterParams::GetTitle() const
{
  return m_Title;
}

DWORD FileFilterParams::GetMask(const char **Mask) const
{
  if (Mask)
    *Mask=FMask.Mask;
  return FMask.Used;
}

DWORD FileFilterParams::GetDate(DWORD *DateType, FILETIME *DateAfter, FILETIME *DateBefore) const
{
  if (DateType)
    *DateType=FDate.DateType;
  if (DateAfter)
    *DateAfter=FDate.DateAfter;
  if (DateBefore)
    *DateBefore=FDate.DateBefore;
  return FDate.Used;
}

DWORD FileFilterParams::GetSize(DWORD *SizeType, __int64 *SizeAbove, __int64 *SizeBelow) const
{
  if (SizeType)
    *SizeType=FSize.SizeType;
  if (SizeAbove)
    *SizeAbove=FSize.SizeAbove;
  if (SizeBelow)
    *SizeBelow=FSize.SizeBelow;
  return FSize.Used;
}

DWORD FileFilterParams::GetAttr(DWORD *AttrSet, DWORD *AttrClear) const
{
  if (AttrSet)
    *AttrSet=FAttr.AttrSet;
  if (AttrClear)
    *AttrClear=FAttr.AttrClear;
  return FAttr.Used;
}

void FileFilterParams::GetColors(HighlightDataColor *Colors) const
{
  memcpy(Colors,&m_Colors,sizeof(m_Colors));
}

int FileFilterParams::GetMarkChar() const
{
  return m_Colors.MarkChar;
}

bool FileFilterParams::FileInFilter(FileListItem *fli)
{
  WIN32_FIND_DATA fd;

  fd.dwFileAttributes=fli->FileAttr;
  fd.ftCreationTime=fli->CreationTime;
  fd.ftLastAccessTime=fli->AccessTime;
  fd.ftLastWriteTime=fli->WriteTime;
  fd.nFileSizeHigh=fli->UnpSizeHigh;
  fd.nFileSizeLow=fli->UnpSize;
  fd.dwReserved0=fli->PackSizeHigh;
  fd.dwReserved1=fli->PackSize;
  xstrncpy(fd.cFileName,fli->Name,sizeof(fd.cFileName)-1);
  xstrncpy(fd.cAlternateFileName,fli->ShortName,sizeof(fd.cAlternateFileName)-1);

  return FileInFilter(&fd);
}

bool FileFilterParams::FileInFilter(WIN32_FIND_DATA *fd)
{
  // ������ ��������?
  //if (fd==NULL)
    //return false;

  // ����� �������� ����� ����� �������?
  if (FMask.Used)
  {
    // ���� �� �������� ��� ����� �������� � �������?
    if (!FMask.FilterMask.Compare(fd->cFileName))
      // �� ���������� ���� ����
      return false;
  }

  // ����� �������� ������� ����� �������?
  if (FSize.Used)
  {
    // ����������� ������ �� ���� DWORD � ����������� __int64
    unsigned __int64 fsize=(unsigned __int64)fd->nFileSizeLow|((unsigned __int64)fd->nFileSizeHigh<<32);

    if (FSize.SizeAbove != _i64(-1))
    {
      if (fsize < FSize.SizeAboveReal)      // ������ ����� ������ ������������ ������������ �� �������?
        return false;                       // �� ���������� ���� ����
    }

    if (FSize.SizeBelow != _i64(-1))
    {

      if (fsize > FSize.SizeBelowReal)      // ������ ����� ������ ������������� ������������ �� �������?
        return false;                       // �� ���������� ���� ����
    }
  }

  // ����� �������� ������� ����� �������?
  if (FDate.Used)
  {
    // ����������� FILETIME � ����������� __int64
    unsigned __int64 &after=(unsigned __int64 &)FDate.DateAfter;
    unsigned __int64 &before=(unsigned __int64 &)FDate.DateBefore;

    if (after!=_ui64(0) || before!=_ui64(0))
    {
      unsigned __int64 ftime=_ui64(0);

      switch (FDate.DateType)
      {
        case FDATE_MODIFIED:
          (unsigned __int64 &)ftime=(unsigned __int64 &)fd->ftLastWriteTime;
          break;
        case FDATE_CREATED:
          (unsigned __int64 &)ftime=(unsigned __int64 &)fd->ftCreationTime;
          break;
        case FDATE_OPENED:
          (unsigned __int64 &)ftime=(unsigned __int64 &)fd->ftLastAccessTime;
          break;
      }

      // ���� �������� ������������� ��������� ����?
      if (after!=_ui64(0))
        // ���� ����� ������ ��������� ���� �� �������?
        if (ftime<after)
          // �� ���������� ���� ����
          return false;

      // ���� �������� ������������� �������� ����?
      if (before!=_ui64(0))
        // ���� ����� ������ �������� ���� �� �������?
        if (ftime>before)
          return false;
    }
  }

  // ����� �������� ��������� ����� �������?
  if (FAttr.Used)
  {
    // �������� ��������� ����� �� ������������� ���������
    if ((fd->dwFileAttributes & FAttr.AttrSet) != FAttr.AttrSet)
      return false;

    // �������� ��������� ����� �� ������������� ���������
    if (fd->dwFileAttributes & FAttr.AttrClear)
      return false;
  }

  // ��! ���� �������� ��� ��������� � ����� ������� � �������������
  // � ��������� ��� ������� ��������.
  return true;
}

void GetFileDateAndTime(const char *Src,unsigned *Dst,int Separator)
{
  char Temp[32], Digit[16],*PtrDigit;
  int I;

  xstrncpy(Temp,Src,sizeof(Temp)-1);
  Dst[0]=Dst[1]=Dst[2]=(unsigned)-1;
  I=0;
  const char *Ptr=Temp;
  while((Ptr=GetCommaWord(Ptr,Digit,Separator)) != NULL)
  {
    PtrDigit=Digit;
    while (*PtrDigit && !isdigit(*PtrDigit))
      PtrDigit++;
    if(*PtrDigit)
      Dst[I]=atoi(PtrDigit);
    ++I;
  }
}

void StrToDateTime(const char *CDate,const char *CTime,FILETIME &ft, int DateFormat, int DateSeparator, int TimeSeparator)
{
  unsigned DateN[3],TimeN[3];
  SYSTEMTIME st;
  FILETIME lft;

  // ����������� �������� ������������� ���� � �����
  GetFileDateAndTime(CDate,DateN,DateSeparator);
  GetFileDateAndTime(CTime,TimeN,TimeSeparator);
  if(DateN[0] == -1 || DateN[1] == -1 || DateN[2] == -1)
  {
    // ������������ ������� ���� ������, ������ ������� ���� � �����.
    memset(&ft,0,sizeof(ft));
    return;
  }

  memset(&st,0,sizeof(st));

  // "�������"
  switch(DateFormat)
  {
    case 0:
      st.wMonth=DateN[0]!=(unsigned)-1?DateN[0]:0;
      st.wDay  =DateN[1]!=(unsigned)-1?DateN[1]:0;
      st.wYear =DateN[2]!=(unsigned)-1?DateN[2]:0;
      break;
    case 1:
      st.wDay  =DateN[0]!=(unsigned)-1?DateN[0]:0;
      st.wMonth=DateN[1]!=(unsigned)-1?DateN[1]:0;
      st.wYear =DateN[2]!=(unsigned)-1?DateN[2]:0;
      break;
    default:
      st.wYear =DateN[0]!=(unsigned)-1?DateN[0]:0;
      st.wMonth=DateN[1]!=(unsigned)-1?DateN[1]:0;
      st.wDay  =DateN[2]!=(unsigned)-1?DateN[2]:0;
      break;
  }
  st.wHour   = TimeN[0]!=(unsigned)-1?(TimeN[0]):0;
  st.wMinute = TimeN[1]!=(unsigned)-1?(TimeN[1]):0;
  st.wSecond = TimeN[2]!=(unsigned)-1?(TimeN[2]):0;

  if (st.wYear<100)
    if (st.wYear<80)
      st.wYear+=2000;
    else
      st.wYear+=1900;

  // �������������� � "������������" ������
  SystemTimeToFileTime(&st,&lft);
  LocalFileTimeToFileTime(&lft,&ft);
  return;
}

enum enumFileFilterConfig {
    ID_FF_TITLE,

    ID_FF_NAME,
    ID_FF_NAMEEDIT,

    ID_FF_SEPARATOR1,

    ID_FF_MATCHMASK,
    ID_FF_MASKEDIT,

    ID_FF_SEPARATOR2,

    ID_FF_MATCHSIZE,
    ID_FF_SIZEDIVIDER,
    ID_FF_SIZEFROM,
    ID_FF_SIZEFROMEDIT,
    ID_FF_SIZETO,
    ID_FF_SIZETOEDIT,

    ID_FF_MATCHDATE,
    ID_FF_DATETYPE,
    ID_FF_DATEAFTER,
    ID_FF_DATEAFTEREDIT,
    ID_FF_TIMEAFTEREDIT,
    ID_FF_DATEBEFORE,
    ID_FF_DATEBEFOREEDIT,
    ID_FF_TIMEBEFOREEDIT,
    ID_FF_CURRENT,
    ID_FF_BLANK,

    ID_FF_SEPARATOR3,
    ID_FF_VSEPARATOR1,

    ID_FF_MATCHATTRIBUTES,
    ID_FF_READONLY,
    ID_FF_ARCHIVE,
    ID_FF_HIDDEN,
    ID_FF_SYSTEM,
    ID_FF_DIRECTORY,
    ID_FF_COMPRESSED,
    ID_FF_ENCRYPTED,
    ID_FF_NOTINDEXED,
    ID_FF_SPARSE,
    ID_FF_TEMP,
    ID_FF_REPARSEPOINT,

    ID_HER_SEPARATOR3,
    ID_HER_MARKEDIT,
    ID_HER_MARK_TITLE,
    ID_HER_MARKTRANSPARENT,
    ID_HER_NORMAL,
    ID_HER_SELECTED,
    ID_HER_CURSOR,
    ID_HER_SELECTEDCURSOR,
    ID_HER_COLOREXAMPLE,

    ID_FF_SEPARATOR4,

    ID_FF_OK,
    ID_FF_RESET,
    ID_FF_CANCEL
};

void HighlightDlgUpdateUserControl(CHAR_INFO *VBufColorExample, struct HighlightDataColor &Colors)
{
  const char *ptr;
  DWORD Color;
  const DWORD FarColor[] = {COL_PANELTEXT,COL_PANELSELECTEDTEXT,COL_PANELCURSOR,COL_PANELSELECTEDCURSOR};
  for (int j=0; j<4; j++)
  {
    Color=(DWORD)(Colors.Color[HIGHLIGHTCOLORTYPE_FILE][j]&0x00FF);
    if (!Color)
      Color=FarColorToReal(FarColor[j]);
    if (Colors.MarkChar&0x00FF)
      ptr=MSG(MHighlightExample2);
    else
      ptr=MSG(MHighlightExample1);
    for (int k=0; k<15; k++)
    {
      VBufColorExample[15*j+k].Char.AsciiChar=ptr[k];
      VBufColorExample[15*j+k].Attributes=Color;
    }
    if (Colors.MarkChar)
      VBufColorExample[15*j+1].Char.AsciiChar=Colors.MarkChar&0x00FF;
    VBufColorExample[15*j].Attributes=FarColorToReal(COL_PANELBOX);
    VBufColorExample[15*j+14].Attributes=FarColorToReal(COL_PANELBOX);
  }
}

LONG_PTR WINAPI FileFilterConfigDlgProc(HANDLE hDlg,int Msg,int Param1,LONG_PTR Param2)
{
  switch(Msg)
  {
    case DN_BTNCLICK:
    {
      if (Param1==ID_FF_CURRENT || Param1==ID_FF_BLANK) //Current � Blank
      {
        FILETIME ft;
        char Date[16],Time[16];

        if (Param1==ID_FF_CURRENT)
        {
          GetSystemTimeAsFileTime(&ft);
          ConvertDate(ft,Date,Time,8,FALSE,FALSE,TRUE);
        }
        else
          *Date=*Time=0;

        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_DATEAFTEREDIT,(LONG_PTR)Date);
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_TIMEAFTEREDIT,(LONG_PTR)Time);
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_DATEBEFOREEDIT,(LONG_PTR)Date);
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_TIMEBEFOREEDIT,(LONG_PTR)Time);

        Dialog::SendDlgMessage(hDlg,DM_SETFOCUS,ID_FF_DATEAFTEREDIT,0);
        COORD r;
        r.X=r.Y=0;
        Dialog::SendDlgMessage(hDlg,DM_SETCURSORPOS,ID_FF_DATEAFTEREDIT,(LONG_PTR)&r);

        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
      }
      else if (Param1==ID_FF_RESET) // Reset
      {
        // ������� �������
        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,FALSE,0);

        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_MASKEDIT,(LONG_PTR)"*");
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_SIZEFROMEDIT,(LONG_PTR)"");
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_SIZETOEDIT,(LONG_PTR)"");
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_DATEAFTEREDIT,(LONG_PTR)"");
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_TIMEAFTEREDIT,(LONG_PTR)"");
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_DATEBEFOREEDIT,(LONG_PTR)"");
        Dialog::SendDlgMessage(hDlg,DM_SETTEXTPTR,ID_FF_TIMEBEFOREEDIT,(LONG_PTR)"");

        /* 14.06.2004 KM
           ������� BSTATE_UNCHECKED �� BSTATE_3STATE, � ������
           ������ ��� ����� ��������, �.�. ��������� ��������
        */
        for(int I=ID_FF_READONLY; I <= ID_FF_REPARSEPOINT; ++I)
          Dialog::SendDlgMessage(hDlg,DM_SETCHECK,I,BSTATE_3STATE);

        // 6, 13 - ������� � ������
        struct FarListPos LPos={0,0};
        Dialog::SendDlgMessage(hDlg,DM_LISTSETCURPOS,ID_FF_SIZEDIVIDER,(LONG_PTR)&LPos);
        Dialog::SendDlgMessage(hDlg,DM_LISTSETCURPOS,ID_FF_DATETYPE,(LONG_PTR)&LPos);

        Dialog::SendDlgMessage(hDlg,DM_SETCHECK,ID_FF_MATCHMASK,BSTATE_CHECKED);
        Dialog::SendDlgMessage(hDlg,DM_SETCHECK,ID_FF_MATCHSIZE,BSTATE_UNCHECKED);
        Dialog::SendDlgMessage(hDlg,DM_SETCHECK,ID_FF_MATCHDATE,BSTATE_UNCHECKED);
        Dialog::SendDlgMessage(hDlg,DM_SETCHECK,ID_FF_MATCHATTRIBUTES,BSTATE_UNCHECKED);

        Dialog::SendDlgMessage(hDlg,DM_ENABLEREDRAW,TRUE,0);
      }
    }
    case DN_MOUSECLICK:
      if((Msg==DN_BTNCLICK && Param1 >= ID_HER_NORMAL && Param1 <= ID_HER_SELECTEDCURSOR)
         || (Msg==DN_MOUSECLICK && Param1==ID_HER_COLOREXAMPLE && ((MOUSE_EVENT_RECORD *)Param2)->dwButtonState==FROM_LEFT_1ST_BUTTON_PRESSED))
      {
        if (Msg==DN_MOUSECLICK)
          Param1 = ID_HER_NORMAL + ((MOUSE_EVENT_RECORD *)Param2)->dwMousePosition.Y;

        HighlightDataColor *EditData = (HighlightDataColor *) Dialog::SendDlgMessage (hDlg, DM_GETDLGDATA, 0, 0);

        unsigned int Color=(unsigned int)EditData->Color[Param1-ID_HER_NORMAL];
        GetColorDialog(Color,true,true);
        EditData->Color[HIGHLIGHTCOLORTYPE_FILE][Param1-ID_HER_NORMAL]=(WORD)Color;

        FarDialogItem MarkChar, ColorExample;
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_HER_MARKEDIT,(LONG_PTR)&MarkChar);
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_HER_COLOREXAMPLE,(LONG_PTR)&ColorExample);
        EditData->MarkChar=*MarkChar.Data.Data;
        HighlightDlgUpdateUserControl(ColorExample.Param.VBuf,*EditData);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_HER_COLOREXAMPLE,(LONG_PTR)&ColorExample);
        return TRUE;
      }
      break;

    case DN_EDITCHANGE:
      if (Param1 == ID_HER_MARKEDIT)
      {
        HighlightDataColor *EditData = (HighlightDataColor *) Dialog::SendDlgMessage (hDlg, DM_GETDLGDATA, 0, 0);
        FarDialogItem *MarkChar, ColorExample;
        MarkChar=(FarDialogItem *)Param2;
        Dialog::SendDlgMessage(hDlg,DM_GETDLGITEM,ID_HER_COLOREXAMPLE,(LONG_PTR)&ColorExample);
        EditData->MarkChar=*(MarkChar->Data.Data);
        HighlightDlgUpdateUserControl(ColorExample.Param.VBuf,*EditData);
        Dialog::SendDlgMessage(hDlg,DM_SETDLGITEM,ID_HER_COLOREXAMPLE,(LONG_PTR)&ColorExample);
        return TRUE;
      }
  }
  return Dialog::DefDlgProc(hDlg,Msg,Param1,Param2);
}

bool FileFilterConfig(FileFilterParams *FF, bool ColorConfig)
{
/*
    00000000001111111111222222222233333333334444444444555555555566666666667777777777
    01234567890123456789012345678901234567890123456789012345678901234567890123456789
00
01     +-------------------------- ������ �������� --------------------------+
02     | �������� �������:                                                   |
03     | "�����-�� ��������                                                "|
04     +-------------------------- ��������� ����� --------------------------+
05     | [ ] ���������� � ������ (�������)                                   |
06     |   *.*                                                              |
07     +------------------------------+--------------------------------------+
08     | [x] ������ �                 � [x] ����/�����                       |
09     |              "����������   "�               "�����������         "|
10     |   ������ ��� �����: "      " �   ������� �:  "  .  .   " "  :  :  " |
11     |   ������ ��� �����: "      " �   ����������: "  .  .   " "  :  :  " |
12     |                              �                [ ������� ] [ ����� ] |
13     +------------------------------+--------------------------------------+
14     | [ ] ��������                                                        |
15     |   [?] ������ ��� ������  [?] �������           [?] �����������      |
16     |   [?] ��������           [?] ������            [?] ���������        |
17     |   [?] �������            [?] �������������     [?] ������. �����    |
18     |   [?] ���������          [?] ���������������                        |
19     +---------------------------------------------------------------------+
20     |                  [ �� ]  [ �������� ]  [ ������ ]                   |
21     +---------------------------------------------------------------------+
22
23
24
25
*/

  // ��������� �����.
  CFileMask FileMask;
  int I;
  const char VerticalLine[] = {0x0C2,0x0B3,0x0B3,0x0B3,0x0B3,0x0C1,0};
  // ����� ��� ����� �������� �����
  const char DigitMask[] = "99999999999999999999";
  // ������� ��� ����� ������
  const char FilterMasksHistoryName[] = "FilterMasks";
  // ����� ��� ������� ���������
  char DateMask[16],DateStrAfter[16],DateStrBefore[16];
  char TimeMask[16],TimeStrAfter[16],TimeStrBefore[16];

  // ����������� ���������� ���� � ������� � �������.
  int DateSeparator=GetDateSeparator();
  int TimeSeparator=GetTimeSeparator();
  int DateFormat=GetDateFormat();

  switch(DateFormat)
  {
    case 0:
      // ����� ���� ��� �������� DD.MM.YYYY � MM.DD.YYYY
      sprintf(DateMask,"99%c99%c9999",DateSeparator,DateSeparator);
      break;
    case 1:
      // ����� ���� ��� �������� DD.MM.YYYY � MM.DD.YYYY
      sprintf(DateMask,"99%c99%c9999",DateSeparator,DateSeparator);
      break;
    default:
      // ����� ���� ��� ������� YYYY.MM.DD
      sprintf(DateMask,"9999%c99%c99",DateSeparator,DateSeparator);
      break;
  }
  // ����� �������
  sprintf(TimeMask,"99%c99%c99",TimeSeparator,TimeSeparator);

  struct DialogData FilterDlgData[]=
  {
    DI_DOUBLEBOX,3,1,73,20,0,0,DIF_SHOWAMPERSAND,0,(char *)MFileFilterTitle,

    DI_TEXT,5,2,0,2,1,0,0,0,(char *)MFileFilterName,
    DI_EDIT,5,3,71,3,0,0,0,0,"",

    DI_TEXT,0,4,0,4,0,0,DIF_SEPARATOR,0,"",

    DI_CHECKBOX,5,5,0,5,0,0,DIF_AUTOMATION,0,(char *)MFileFilterMatchMask,
    DI_EDIT,7,6,71,6,0,(DWORD_PTR)FilterMasksHistoryName,DIF_HISTORY|DIF_VAREDIT,0,"",

    DI_TEXT,0,7,0,7,0,0,DIF_SEPARATOR,0,"",

    DI_CHECKBOX,5,8,0,8,0,0,DIF_AUTOMATION,0,(char *)MFileFilterSize,
    DI_COMBOBOX,20,8,36,8,0,0,DIF_DROPDOWNLIST|DIF_LISTNOAMPERSAND,0,"",
    DI_TEXT,7,9,11,9,0,0,0,0,(char *)MFileFilterSizeFrom,
    DI_FIXEDIT,20,9,36,9,0,(DWORD_PTR)DigitMask,DIF_MASKEDIT,0,"",
    DI_TEXT,7,10,11,10,0,0,0,0,(char *)MFileFilterSizeTo,
    DI_FIXEDIT,20,10,36,10,0,(DWORD_PTR)DigitMask,DIF_MASKEDIT,0,"",

    DI_CHECKBOX,40,8,0,8,0,0,DIF_AUTOMATION,0,(char *)MFileFilterDate,
    DI_COMBOBOX,58,8,71,8,0,0,DIF_DROPDOWNLIST|DIF_LISTNOAMPERSAND,0,"",
    DI_TEXT,42,9,48,9,0,0,0,0,(char *)MFileFilterAfter,
    DI_FIXEDIT,53,9,62,9,0,(DWORD_PTR)DateMask,DIF_MASKEDIT,0,"",
    DI_FIXEDIT,64,9,71,9,0,(DWORD_PTR)TimeMask,DIF_MASKEDIT,0,"",
    DI_TEXT,42,10,48,10,0,0,0,0,(char *)MFileFilterBefore,
    DI_FIXEDIT,53,10,62,10,0,(DWORD_PTR)DateMask,DIF_MASKEDIT,0,"",
    DI_FIXEDIT,64,10,71,10,0,(DWORD_PTR)TimeMask,DIF_MASKEDIT,0,"",
    DI_BUTTON,0,11,0,11,0,0,DIF_BTNNOCLOSE,0,(char *)MFileFilterCurrent,
    DI_BUTTON,0,11,71,11,0,0,DIF_BTNNOCLOSE,0,(char *)MFileFilterBlank,

    DI_TEXT,0,12,0,12,0,0,DIF_SEPARATOR,0,"",

    DI_VTEXT,38,7,38,12,0,0,0,0,(char *)VerticalLine,

    DI_CHECKBOX, 5,13,0,13,0,0,DIF_AUTOMATION,0,(char *)MFileFilterAttr,
    DI_CHECKBOX, 7,14,0,14,0,0,DIF_3STATE,0,(char *)MFileFilterAttrR,
    DI_CHECKBOX, 7,15,0,15,0,0,DIF_3STATE,0,(char *)MFileFilterAttrA,
    DI_CHECKBOX, 7,16,0,16,0,0,DIF_3STATE,0,(char *)MFileFilterAttrH,
    DI_CHECKBOX, 7,17,0,17,0,0,DIF_3STATE,0,(char *)MFileFilterAttrS,
    DI_CHECKBOX,29,14,0,14,0,0,DIF_3STATE,0,(char *)MFileFilterAttrD,
    DI_CHECKBOX,29,15,0,15,0,0,DIF_3STATE,0,(char *)MFileFilterAttrC,
    DI_CHECKBOX,29,16,0,16,0,0,DIF_3STATE,0,(char *)MFileFilterAttrE,
    DI_CHECKBOX,29,17,0,17,0,0,DIF_3STATE,0,(char *)MFileFilterAttrNI,
    DI_CHECKBOX,51,14,0,14,0,0,DIF_3STATE,0,(char *)MFileFilterAttrSparse,
    DI_CHECKBOX,51,15,0,15,0,0,DIF_3STATE,0,(char *)MFileFilterAttrT,
    DI_CHECKBOX,51,16,0,16,0,0,DIF_3STATE,0,(char *)MFileFilterAttrReparse,

    DI_TEXT,-1,15,0,15,0,0,DIF_BOXCOLOR|DIF_SEPARATOR,0,(char *)MHighlightColors,
    DI_FIXEDIT,5,16,5,16,0,0,0,0,"",
    DI_TEXT,7,16,0,16,0,0,0,0,(char *)MHighlightMarkChar,
    DI_CHECKBOX,0,16,0,16,0,0,0,0,(char *)MHighlightTransparentMarkChar,
    DI_BUTTON,5,17,0,17,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightNormal,
    DI_BUTTON,5,18,0,18,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightSelected,
    DI_BUTTON,5,19,0,19,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightCursor,
    DI_BUTTON,5,20,0,20,0,0,DIF_BTNNOCLOSE,0,(char *)MHighlightSelectedCursor,
    DI_USERCONTROL,73-15-1,17,73-2,20,0,0,DIF_NOFOCUS,0,"",

    DI_TEXT,0,18,0,18,0,0,DIF_SEPARATOR,0,"",

    DI_BUTTON,0,19,0,19,0,0,DIF_CENTERGROUP,1,(char *)MFileFilterOk,
    DI_BUTTON,0,19,0,19,0,0,DIF_CENTERGROUP|DIF_BTNNOCLOSE,0,(char *)MFileFilterReset,
    DI_BUTTON,0,19,0,19,0,0,DIF_CENTERGROUP,0,(char *)MFileFilterCancel,
  };

  MakeDialogItems(FilterDlgData,FilterDlg);

  if (ColorConfig)
  {
    FilterDlg[ID_FF_TITLE].Y2+=3;

    for (int i=ID_FF_NAME; i<=ID_FF_SEPARATOR1; i++)
      FilterDlg[i].Flags|=DIF_HIDDEN;

    for (int i=ID_FF_MATCHMASK; i<=ID_FF_REPARSEPOINT; i++)
    {
      FilterDlg[i].Y1-=3;
      FilterDlg[i].Y2-=3;
    }

    for (int i=ID_FF_SEPARATOR4; i<=ID_FF_CANCEL; i++)
    {
      FilterDlg[i].Y1+=3;
      FilterDlg[i].Y2+=3;
    }
  }
  else
  {
    for (int i=ID_HER_SEPARATOR3; i<=ID_HER_COLOREXAMPLE; i++)
      FilterDlg[i].Flags|=DIF_HIDDEN;
  }

  //FilterDlg[ID_FF_SIZEDIVIDER].X1=FilterDlg[ID_FF_MATCHSIZE].X1+strlen(FilterDlg[ID_FF_MATCHSIZE].Data)-(strchr(FilterDlg[ID_FF_MATCHSIZE].Data,'&')?1:0)+5;
  //FilterDlg[ID_FF_SIZEDIVIDER].X2+=FilterDlg[ID_FF_SIZEDIVIDER].X1;
  //FilterDlg[ID_FF_DATETYPE].X1=FilterDlg[ID_FF_MATCHDATE].X1+strlen(FilterDlg[ID_FF_MATCHDATE].Data)-(strchr(FilterDlg[ID_FF_MATCHDATE].Data,'&')?1:0)+5;
  //FilterDlg[ID_FF_DATETYPE].X2+=FilterDlg[ID_FF_DATETYPE].X1;

  FilterDlg[ID_FF_BLANK].X1=FilterDlg[ID_FF_BLANK].X2-strlen(FilterDlg[ID_FF_BLANK].Data)+(strchr(FilterDlg[ID_FF_BLANK].Data,'&')?1:0)-3;
  FilterDlg[ID_FF_CURRENT].X2=FilterDlg[ID_FF_BLANK].X1-2;
  FilterDlg[ID_FF_CURRENT].X1=FilterDlg[ID_FF_CURRENT].X2-strlen(FilterDlg[ID_FF_CURRENT].Data)+(strchr(FilterDlg[ID_FF_CURRENT].Data,'&')?1:0)-3;

  FilterDlg[ID_HER_MARKTRANSPARENT].X1=FilterDlg[ID_HER_MARK_TITLE].X1+strlen(FilterDlg[ID_HER_MARK_TITLE].Data)-(strchr(FilterDlg[ID_HER_MARK_TITLE].Data,'&')?1:0)+1;

  CHAR_INFO VBufColorExample[15*4];
  HighlightDataColor Colors;

  FF->GetColors(&Colors);
  memset(VBufColorExample,0,sizeof(VBufColorExample));
  HighlightDlgUpdateUserControl(VBufColorExample,Colors);
  FilterDlg[ID_HER_COLOREXAMPLE].VBuf=VBufColorExample;

  *FilterDlg[ID_HER_MARKEDIT].Data=Colors.MarkChar&0x00FF;
  FilterDlg[ID_HER_MARKTRANSPARENT].Selected=(Colors.MarkChar&0xFF00?1:0);

  xstrncpy(FilterDlg[ID_FF_NAMEEDIT].Data,FF->GetTitle(),sizeof(FilterDlg[ID_FF_NAMEEDIT].Data));

  const char *FMask;
  FilterDlg[ID_FF_MATCHMASK].Selected=FF->GetMask(&FMask);
  char Mask[FILEFILTER_MASK_SIZE];
  xstrncpy(Mask,FMask,sizeof(Mask)-1);
  FilterDlg[ID_FF_MASKEDIT].Ptr.PtrData=Mask;
  FilterDlg[ID_FF_MASKEDIT].Ptr.PtrLength=sizeof(Mask)-1;
  FilterDlg[ID_FF_MASKEDIT].Ptr.PtrFlags=0;
  if (!FilterDlg[ID_FF_MATCHMASK].Selected)
    FilterDlg[ID_FF_MASKEDIT].Flags|=DIF_DISABLE;

  // ���� ��� ����������: ����� - ���������
  FarList SizeList;
  FarListItem TableItemSize[FSIZE_COUNT];
  // ��������� ������ ���������� ��� ���� �������
  SizeList.Items=TableItemSize;
  SizeList.ItemsNumber=FSIZE_COUNT;

  memset(TableItemSize,0,sizeof(TableItemSize));
  for(int i=0; i < FSIZE_COUNT; ++i)
    xstrncpy(TableItemSize[i].Text,MSG(MFileFilterSizeInBytes+i),sizeof(TableItemSize[i].Text)-1);

  DWORD SizeType;
  __int64 SizeAbove, SizeBelow;
  FilterDlg[ID_FF_MATCHSIZE].Selected=FF->GetSize(&SizeType,&SizeAbove,&SizeBelow);
  FilterDlg[ID_FF_SIZEDIVIDER].ListItems=&SizeList;
  TableItemSize[SizeType].Flags=LIF_SELECTED;

  if (SizeAbove != _i64(-1))
    _ui64toa(SizeAbove,FilterDlg[ID_FF_SIZEFROMEDIT].Data,10);

  if (SizeBelow != _i64(-1))
    _ui64toa(SizeBelow,FilterDlg[ID_FF_SIZETOEDIT].Data,10);

  if (!FilterDlg[ID_FF_MATCHSIZE].Selected)
    for(I=ID_FF_SIZEDIVIDER; I <= ID_FF_SIZETOEDIT; ++I)
      FilterDlg[I].Flags|=DIF_DISABLE;

  // ���� ��� ���������� ������� �����
  FarList DateList;
  FarListItem TableItemDate[FDATE_COUNT];
  // ��������� ������ ����� ��� �����
  DateList.Items=TableItemDate;
  DateList.ItemsNumber=FDATE_COUNT;

  memset(TableItemDate,0,sizeof(TableItemDate));
  for(int i=0; i < FDATE_COUNT; ++i)
    xstrncpy(TableItemDate[i].Text,MSG(MFileFilterModified+i),sizeof(TableItemDate[i].Text)-1);

  DWORD DateType;
  FILETIME DateAfter, DateBefore;
  FilterDlg[ID_FF_MATCHDATE].Selected=FF->GetDate(&DateType,&DateAfter,&DateBefore);
  FilterDlg[ID_FF_DATETYPE].ListItems=&DateList;
  TableItemDate[DateType].Flags=LIF_SELECTED;

  ConvertDate(DateAfter,FilterDlg[ID_FF_DATEAFTEREDIT].Data,FilterDlg[ID_FF_TIMEAFTEREDIT].Data,8,FALSE,FALSE,TRUE);
  ConvertDate(DateBefore,FilterDlg[ID_FF_DATEBEFOREEDIT].Data,FilterDlg[ID_FF_TIMEBEFOREEDIT].Data,8,FALSE,FALSE,TRUE);

  if (!FilterDlg[ID_FF_MATCHDATE].Selected)
    for(I=ID_FF_DATETYPE; I <= ID_FF_BLANK; ++I)
      FilterDlg[I].Flags|=DIF_DISABLE;

  DWORD AttrSet, AttrClear;
  FilterDlg[ID_FF_MATCHATTRIBUTES].Selected=FF->GetAttr(&AttrSet,&AttrClear);
  FilterDlg[ID_FF_READONLY].Selected=(AttrSet & FILE_ATTRIBUTE_READONLY?1:AttrClear & FILE_ATTRIBUTE_READONLY?0:2);
  FilterDlg[ID_FF_ARCHIVE].Selected=(AttrSet & FILE_ATTRIBUTE_ARCHIVE?1:AttrClear & FILE_ATTRIBUTE_ARCHIVE?0:2);
  FilterDlg[ID_FF_HIDDEN].Selected=(AttrSet & FILE_ATTRIBUTE_HIDDEN?1:AttrClear & FILE_ATTRIBUTE_HIDDEN?0:2);
  FilterDlg[ID_FF_SYSTEM].Selected=(AttrSet & FILE_ATTRIBUTE_SYSTEM?1:AttrClear & FILE_ATTRIBUTE_SYSTEM?0:2);
  FilterDlg[ID_FF_COMPRESSED].Selected=(AttrSet & FILE_ATTRIBUTE_COMPRESSED?1:AttrClear & FILE_ATTRIBUTE_COMPRESSED?0:2);
  FilterDlg[ID_FF_ENCRYPTED].Selected=(AttrSet & FILE_ATTRIBUTE_ENCRYPTED?1:AttrClear & FILE_ATTRIBUTE_ENCRYPTED?0:2);
  FilterDlg[ID_FF_DIRECTORY].Selected=(AttrSet & FILE_ATTRIBUTE_DIRECTORY?1:AttrClear & FILE_ATTRIBUTE_DIRECTORY?0:2);
  FilterDlg[ID_FF_NOTINDEXED].Selected=(AttrSet & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED?1:AttrClear & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED?0:2);
  FilterDlg[ID_FF_SPARSE].Selected=(AttrSet & FILE_ATTRIBUTE_SPARSE_FILE?1:AttrClear & FILE_ATTRIBUTE_SPARSE_FILE?0:2);
  FilterDlg[ID_FF_TEMP].Selected=(AttrSet & FILE_ATTRIBUTE_TEMPORARY?1:AttrClear & FILE_ATTRIBUTE_TEMPORARY?0:2);
  FilterDlg[ID_FF_REPARSEPOINT].Selected=(AttrSet & FILE_ATTRIBUTE_REPARSE_POINT?1:AttrClear & FILE_ATTRIBUTE_REPARSE_POINT?0:2);

  if (!FilterDlg[ID_FF_MATCHATTRIBUTES].Selected)
  {
    for(I=ID_FF_READONLY; I <= ID_FF_REPARSEPOINT; ++I)
      FilterDlg[I].Flags|=DIF_DISABLE;
  }

  Dialog Dlg(FilterDlg,sizeof(FilterDlg)/sizeof(FilterDlg[0]),FileFilterConfigDlgProc,(LONG_PTR)&Colors);

  Dlg.SetHelp("OpFilter");
  Dlg.SetPosition(-1,-1,FilterDlg[ID_FF_TITLE].X2+4,FilterDlg[ID_FF_TITLE].Y2+2);

  Dlg.SetAutomation(ID_FF_MATCHMASK,ID_FF_MASKEDIT,DIF_DISABLE,0,0,DIF_DISABLE);

  Dlg.SetAutomation(ID_FF_MATCHSIZE,ID_FF_SIZEDIVIDER,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHSIZE,ID_FF_SIZEFROM,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHSIZE,ID_FF_SIZEFROMEDIT,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHSIZE,ID_FF_SIZETO,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHSIZE,ID_FF_SIZETOEDIT,DIF_DISABLE,0,0,DIF_DISABLE);

  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_DATETYPE,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_DATEAFTER,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_DATEAFTEREDIT,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_TIMEAFTEREDIT,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_DATEBEFORE,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_DATEBEFOREEDIT,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_TIMEBEFOREEDIT,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_CURRENT,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHDATE,ID_FF_BLANK,DIF_DISABLE,0,0,DIF_DISABLE);

  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_READONLY,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_ARCHIVE,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_HIDDEN,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_SYSTEM,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_COMPRESSED,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_ENCRYPTED,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_NOTINDEXED,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_SPARSE,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_TEMP,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_REPARSEPOINT,DIF_DISABLE,0,0,DIF_DISABLE);
  Dlg.SetAutomation(ID_FF_MATCHATTRIBUTES,ID_FF_DIRECTORY,DIF_DISABLE,0,0,DIF_DISABLE);

  for (;;)
  {
    Dlg.ClearDone();
    Dlg.Process();
    int ExitCode=Dlg.GetExitCode();

    if (ExitCode==ID_FF_OK) // Ok
    {
      // ���� �������� ������������� ����� �� ���������, ����� ������� � ������
      if (FilterDlg[ID_FF_MATCHMASK].Selected && !FileMask.Set((char *)FilterDlg[ID_FF_MASKEDIT].Ptr.PtrData,0))
        continue;

      if (FilterDlg[ID_HER_MARKTRANSPARENT].Selected)
        Colors.MarkChar|=0xFF00;
      else
        Colors.MarkChar&=0x00FF;

      FF->SetColors(&Colors);

      FF->SetTitle(FilterDlg[ID_FF_NAMEEDIT].Data);

      FF->SetMask(FilterDlg[ID_FF_MATCHMASK].Selected,
                  (char *)FilterDlg[ID_FF_MASKEDIT].Ptr.PtrData);

      if(!*RemoveExternalSpaces(FilterDlg[ID_FF_SIZEFROMEDIT].Data))
        SizeAbove=_i64(-1);
      else
        SizeAbove=_atoi64(FilterDlg[ID_FF_SIZEFROMEDIT].Data);

      if(!*RemoveExternalSpaces(FilterDlg[ID_FF_SIZETOEDIT].Data))
        SizeBelow=_i64(-1);
      else
        SizeBelow=_atoi64(FilterDlg[ID_FF_SIZETOEDIT].Data);

      FF->SetSize(FilterDlg[ID_FF_MATCHSIZE].Selected,
                  FilterDlg[ID_FF_SIZEDIVIDER].ListPos,
                  SizeAbove,
                  SizeBelow);

      StrToDateTime(FilterDlg[ID_FF_DATEAFTEREDIT].Data,FilterDlg[ID_FF_TIMEAFTEREDIT].Data,DateAfter,DateFormat,DateSeparator,TimeSeparator);
      StrToDateTime(FilterDlg[ID_FF_DATEBEFOREEDIT].Data,FilterDlg[ID_FF_TIMEBEFOREEDIT].Data,DateBefore,DateFormat,DateSeparator,TimeSeparator);

      FF->SetDate(FilterDlg[ID_FF_MATCHDATE].Selected,
                  FilterDlg[ID_FF_DATETYPE].ListPos,
                  DateAfter,
                  DateBefore);

      AttrSet=0;
      AttrClear=0;

      AttrSet|=(FilterDlg[ID_FF_READONLY].Selected==1?FILE_ATTRIBUTE_READONLY:0);
      AttrSet|=(FilterDlg[ID_FF_ARCHIVE].Selected==1?FILE_ATTRIBUTE_ARCHIVE:0);
      AttrSet|=(FilterDlg[ID_FF_HIDDEN].Selected==1?FILE_ATTRIBUTE_HIDDEN:0);
      AttrSet|=(FilterDlg[ID_FF_SYSTEM].Selected==1?FILE_ATTRIBUTE_SYSTEM:0);
      AttrSet|=(FilterDlg[ID_FF_COMPRESSED].Selected==1?FILE_ATTRIBUTE_COMPRESSED:0);
      AttrSet|=(FilterDlg[ID_FF_ENCRYPTED].Selected==1?FILE_ATTRIBUTE_ENCRYPTED:0);
      AttrSet|=(FilterDlg[ID_FF_DIRECTORY].Selected==1?FILE_ATTRIBUTE_DIRECTORY:0);
      AttrSet|=(FilterDlg[ID_FF_NOTINDEXED].Selected==1?FILE_ATTRIBUTE_NOT_CONTENT_INDEXED:0);
      AttrSet|=(FilterDlg[ID_FF_SPARSE].Selected==1?FILE_ATTRIBUTE_SPARSE_FILE:0);
      AttrSet|=(FilterDlg[ID_FF_TEMP].Selected==1?FILE_ATTRIBUTE_TEMPORARY:0);
      AttrSet|=(FilterDlg[ID_FF_REPARSEPOINT].Selected==1?FILE_ATTRIBUTE_REPARSE_POINT:0);
      AttrClear|=(FilterDlg[ID_FF_READONLY].Selected==0?FILE_ATTRIBUTE_READONLY:0);
      AttrClear|=(FilterDlg[ID_FF_ARCHIVE].Selected==0?FILE_ATTRIBUTE_ARCHIVE:0);
      AttrClear|=(FilterDlg[ID_FF_HIDDEN].Selected==0?FILE_ATTRIBUTE_HIDDEN:0);
      AttrClear|=(FilterDlg[ID_FF_SYSTEM].Selected==0?FILE_ATTRIBUTE_SYSTEM:0);
      AttrClear|=(FilterDlg[ID_FF_COMPRESSED].Selected==0?FILE_ATTRIBUTE_COMPRESSED:0);
      AttrClear|=(FilterDlg[ID_FF_ENCRYPTED].Selected==0?FILE_ATTRIBUTE_ENCRYPTED:0);
      AttrClear|=(FilterDlg[ID_FF_DIRECTORY].Selected==0?FILE_ATTRIBUTE_DIRECTORY:0);
      AttrClear|=(FilterDlg[ID_FF_NOTINDEXED].Selected==0?FILE_ATTRIBUTE_NOT_CONTENT_INDEXED:0);
      AttrClear|=(FilterDlg[ID_FF_SPARSE].Selected==0?FILE_ATTRIBUTE_SPARSE_FILE:0);
      AttrClear|=(FilterDlg[ID_FF_TEMP].Selected==0?FILE_ATTRIBUTE_TEMPORARY:0);
      AttrClear|=(FilterDlg[ID_FF_REPARSEPOINT].Selected==0?FILE_ATTRIBUTE_REPARSE_POINT:0);

      FF->SetAttr(FilterDlg[ID_FF_MATCHATTRIBUTES].Selected,
                  AttrSet,
                  AttrClear);

      return true;
    }
    else
      break;
  }

  return false;
}
