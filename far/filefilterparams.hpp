#ifndef __FILEFILTERPARAMS_HPP__
#define __FILEFILTERPARAMS_HPP__
/*
filefilterparams.hpp

��������� ��������� �������

*/

#include "plugin.hpp"
#include "struct.hpp"
#include "CFileMask.hpp"
#include "bitflags.hpp"

struct FileListItem;

#define FILEFILTER_MASK_SIZE 2048

enum FileFilterFlags
{
  FFF_RPANELINCLUDE = 1,
  FFF_RPANELEXCLUDE = 2,
  FFF_LPANELINCLUDE = 4,
  FFF_LPANELEXCLUDE = 8,
  FFF_FINDFILEINCLUDE = 16,
  FFF_FINDFILEEXCLUDE = 32,
  FFF_COPYINCLUDE = 64,
  FFF_COPYEXCLUDE = 128,
};

enum FDateType
{
  FDATE_MODIFIED=0,
  FDATE_CREATED,
  FDATE_OPENED,

  FDATE_COUNT, // ������ ��������� !!!
};

enum FSizeType
{
  FSIZE_INBYTES=0,
  FSIZE_INKBYTES,
  FSIZE_INMBYTES,
  FSIZE_INGBYTES,

  FSIZE_COUNT, // ������ ��������� !!!
};

class FileFilterParams
{
  private:

    char m_Title[512];

    struct
    {
      DWORD Used;
      char Mask[FILEFILTER_MASK_SIZE];
      CFileMask FilterMask; // ��������� ���������������� �����.
    } FMask;

    struct
    {
      DWORD Used;
      FDateType DateType;
      FILETIME DateAfter;
      FILETIME DateBefore;
    } FDate;

    struct
    {
      DWORD Used;
      FSizeType SizeType;
      __int64 SizeAbove; // ����� ������ ����� ������ � SizeType ��� -1 ��� �����
      __int64 SizeBelow; // ����� ������ ����� ������ � SizeType ��� -1 ��� �����
      unsigned __int64 SizeAboveReal; // ����� ������ ����� ������ � ������
      unsigned __int64 SizeBelowReal; // ����� ������ ����� ������ � ������
    } FSize;

    struct
    {
      DWORD Used;
      DWORD AttrSet;
      DWORD AttrClear;
    } FAttr;

    struct
    {
      HighlightDataColor Colors;
      int SortGroup;
      bool bContinueProcessing;
    } FHighlight;

  public:

    BitFlags Flags; // ����� �������

  public:

    FileFilterParams();

    const FileFilterParams &operator=(const FileFilterParams &FF);

    void SetTitle(const char *Title);
    void SetMask(DWORD Used, const char *Mask);
    void SetDate(DWORD Used, DWORD DateType, FILETIME DateAfter, FILETIME DateBefore);
    void SetSize(DWORD Used, DWORD SizeType, __int64 SizeAbove, __int64 SizeBelow);
    void SetAttr(DWORD Used, DWORD AttrSet, DWORD AttrClear);
    void SetColors(HighlightDataColor *Colors);
    void SetSortGroup(int SortGroup) { FHighlight.SortGroup = SortGroup; }
    void SetContinueProcessing(bool bContinueProcessing) { FHighlight.bContinueProcessing = bContinueProcessing; }

    const char *GetTitle() const;
    DWORD GetMask(const char **Mask) const;
    DWORD GetDate(DWORD *DateType, FILETIME *DateAfter, FILETIME *DateBefore) const;
    DWORD GetSize(DWORD *SizeType, __int64 *SizeAbove, __int64 *SizeBelow) const;
    DWORD GetAttr(DWORD *AttrSet, DWORD *AttrClear) const;
    void  GetColors(HighlightDataColor *Colors) const;
    int   GetMarkChar() const;
    int   GetSortGroup() const { return FHighlight.SortGroup; }
    bool  GetContinueProcessing() const { return FHighlight.bContinueProcessing; }

    // ������ ����� ���������� "�������" � ������ ��� �����������:
    // �������� �� ���� fd ��� ������� �������������� �������.
    // ���������� true  - ��������;
    //            false - �� ��������.
    bool FileInFilter(WIN32_FIND_DATA *fd);
    bool FileInFilter(FileListItem *fli);
};

bool FileFilterConfig(FileFilterParams *FF, bool ColorConfig=false);

//���������������� ������� ��� �������� ����� ���� ��������� ��������.
void MenuString(char *dest, FileFilterParams *FF, bool bHighightType=false, bool bPanelType=false, const char *FMask=NULL, const char *Title=NULL);

#endif //__FILEFILTERPARAMS_HPP__
